///////////////////////////////////////////////////////////////////////////////
//  For project details and authors, refer to README and AUTHORS files
//
//  This file is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This file is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
//  To view the licence online, go to: http://www.gnu.org/copyleft/gpl.html
////////////////////////////////////////////////////////////////////////////////

#include "Config.h"

#include "CEImagesetEditor.h"
#include "EditorGLCanvas.h"
#include "PropertiesPanel.h"
#include "EditorView.h"
#include "CEGUIHelper.h"
#include "ElasticBox.h"
#include "ElasticBoxWindowRenderer.h"

// CEGUI includes
#include <CEGUIWindowFactoryManager.h>
#include <CEGUIWindowRendererManager.h>
#include <CEGUISchemeManager.h>
#include <CEGUIImagesetManager.h>
#include <CEGUIWindowManager.h>
#include <CEGUIImageset.h>
#include <CEGUIScheme.h>
#include <CEGUIGlobalEventSet.h>
#include <CEGUIDefaultResourceProvider.h>
#include <CEGUIPropertyHelper.h>
#include <CEGUIExceptions.h>

#include <RendererModules/OpenGLGUIRenderer/opengltexture.h>

// wx includes
#undef wxUSE_STDPATHS
#define wxUSE_STDPATHS 1
#include <wx/stdpaths.h>

#define S_(X) #X
#define STRINGIZE(X) S_(X)

using namespace CEGUI;

//-----------------------------------------------------------------------
BEGIN_EVENT_TABLE( EditorGLCanvas, wxGLCanvas )
    EVT_SIZE ( EditorGLCanvas::OnResize )
    EVT_PAINT   ( EditorGLCanvas::OnPaint )
    EVT_ERASE_BACKGROUND( EditorGLCanvas::OnErase )
    EVT_MOTION  ( EditorGLCanvas::OnMouseMotion )
    EVT_LEFT_DOWN   ( EditorGLCanvas::OnLeftDown )
    EVT_LEFT_UP ( EditorGLCanvas::OnLeftUp )
    EVT_RIGHT_DOWN  ( EditorGLCanvas::OnRightDown )
    EVT_RIGHT_UP    ( EditorGLCanvas::OnRightUp )
    EVT_MIDDLE_DOWN  ( EditorGLCanvas::OnMiddleDown )
    EVT_MOUSEWHEEL ( EditorGLCanvas::OnMouseWheel )
    EVT_KEY_DOWN    ( EditorGLCanvas::OnKeyDown )
    EVT_KEY_UP  ( EditorGLCanvas::OnKeyUp )
    EVT_SCROLLWIN( EditorGLCanvas::OnScrollWin )
END_EVENT_TABLE()

//-----------------------------------------------------------------------
EditorGLCanvas::EditorGLCanvas( EditorView* v, wxWindow* parent, const wxPoint& pos, const wxSize& size ) :
    wxGLCanvas( parent, static_cast<const wxGLContext *>( 0 ), -1, pos, size , wxSUNKEN_BORDER | wxVSCROLL | wxHSCROLL ),
    m_view( v ),
    m_GUISystem( 0 ),
    m_GUIRenderer( 0 ),
    m_imagesetStaticImage( 0 ),
    m_useBigSteps( true ),
    m_zoomFactor( 1.0f ),
    m_realWidth( 0 ),
    m_realHeight( 0 ),
    m_scrollPosX( 0 ),
    m_scrollDocX( 0 ),
    m_scrollPageX( 0 ),
    m_scrollPosY( 0 ),
    m_scrollDocY( 0 ),
    m_scrollPageY( 0 )
{
    // Init one-time-only stuff
    SetCurrent();
    glShadeModel( GL_SMOOTH );
    glEnable( GL_DEPTH_TEST );

    // we will use a cross-hair cursor
    SetCursor( wxCURSOR_CROSS );

    // Setup the CEGUI system
    initialiseCEGUI();
}

//-----------------------------------------------------------------------
EditorGLCanvas::~EditorGLCanvas()
{
    // cleanup CEGUI.
    cleanupCEGUI();
}

//-----------------------------------------------------------------------
void EditorGLCanvas::setImage( const wxString& name )
{
    // This could take some time, enter 'busy' state
    wxWindowDisabler wd; wxBusyCursor bc;

    // delete old imageset used for the main image file display
    if ( ImagesetManager::getSingleton().isImagesetPresent ( "__auto_editor_imageset__" ) )
        ImagesetManager::getSingleton().destroyImageset( "__auto_editor_imageset__" );

    // reset all image information
    m_realWidth = 0;
    m_realHeight = 0;
    m_imagesetStaticImage->setProperty( "Image", "" );

    // load new image file as an imageset
    if ( !name.IsEmpty() )
    {
        try
        {
            Imageset* imageset =
                ImagesetManager::getSingleton().createImagesetFromImageFile(
                    "__auto_editor_imageset__",
                    CEGUIHelper::ToCEGUIString( name ) /*,"__work_dir__"*/);

            imageset->setAutoScalingEnabled( false );

            // set image to our main 'StaticImage' widget (so it is displayed)
            assert ( imageset->isImageDefined ( "full_image" ) ) ;
            m_imagesetStaticImage->setProperty( "Image", "set:__auto_editor_imageset__ image:full_image" );

            // update details we hold about the current source image
            const Image& img( imageset->getImage( "full_image" ) );
            CEGUI::Size sizeImage = img.getSize();
            m_realWidth = sizeImage.d_width;
            m_realHeight = sizeImage.d_height;

            // a nice OpenGL renderer abuse hack to change the filtering on
            // the texture for the image.  Man... it feels good.  Ahhh! :-D
            GLuint tex = static_cast<CEGUI::OpenGLTexture*>(imageset->getTexture())->getOGLTexid();
            glBindTexture(GL_TEXTURE_2D, tex);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        }
        catch ( CEGUI::Exception& e)
        {
            wxLogError ( wxT( "EditorGLCanvas::setImage - Error: %s" ),
                         CEGUIHelper::ToWXString( e.getMessage() ).c_str() );
        }
    }

    // mute global events so that document is not changed if the regions get
    // resized or moved due to a new image.
    CEGUI::GlobalEventSet::getSingleton().setMutedState( true );
    // set main image size.
    m_imagesetStaticImage->setSize(
        UVector2( UDim( 0, m_realWidth ), UDim( 0, m_realHeight ) ) );

    // do we even have a doc?
    if ( isDocumentValid() )
    {
        EditorDocument* doc = (EditorDocument*)m_view->GetDocument();

        // for each region window, re-sync it's size with what the doc
        // holds for it.
        for (size_t i = 0; i < m_imagesetStaticImage->getChildCount(); ++i)
        {
            CEGUI::Window* wnd = m_imagesetStaticImage->getChildAtIdx( i );

            if (wnd->testClassName("ElasticBox"))
            {
                wxString name( CEGUIHelper::ToWXString( wnd->getName() ) );
                // extract the correct size from the document
                wxRect area( doc->getRegionArea( name ) );
                // set this area for the region
                setRegionArea( name, area );
            }
        }
    }

    // unmute global events again.
    CEGUI::GlobalEventSet::getSingleton().setMutedState( false );

    // update zoom
    computeZoomFactor( 0 );

    Render();
}

//-----------------------------------------------------------------------
void EditorGLCanvas::changeSize( int newWidth, int newHeight )
{
    // update the OpenGL viewport area
    glViewport(
        0,
        0,
        static_cast<GLsizei>( newWidth ),
        static_cast<GLsizei>( newHeight ) );

    // Calculate aspect ratio
    gluPerspective(
        45.0f,
        static_cast<GLfloat>( newWidth ) / static_cast<GLfloat>( newHeight ),
        0.1f,
        100.0f );

    // Inform CEGUI renderer of change
    m_GUIRenderer->setDisplaySize( CEGUI::Size( static_cast<float>( newWidth ),
                                                static_cast<float>( newHeight ) ) );

    // force a full redraw regardless
    m_GUISystem->signalRedraw();
}

//-----------------------------------------------------------------------
void EditorGLCanvas::OnResize( wxSizeEvent& event )
{
    // We need the client size
    int clientWidth, clientHeight;
    GetClientSize( &clientWidth, &clientHeight );

    changeSize( clientWidth, clientHeight );

    updateScrollbars();
}

//-----------------------------------------------------------------------
void EditorGLCanvas::initialiseCEGUI()
{
    // Initialise OpenGL renderer and the CEGUI system
    m_GUIRenderer = new OpenGLRenderer( 0 );
    m_GUISystem = new System( m_GUIRenderer );

    // keep logging to a minimum unless we're debugging
    #if defined (DEBUG) || defined (_DEBUG)
        Logger::getSingleton().setLoggingLevel ( Informative );
    #else
        Logger::getSingleton().setLoggingLevel ( Errors );
    #endif

    // Add support to CEGUI for our custom 'ElasticBox' widget
    WindowFactoryManager::getSingleton().addFactory( &getElasticBoxFactory() );
    WindowRendererManager::getSingleton().addFactory( &getElasticBoxWRFactory() );

    DefaultResourceProvider* rp = static_cast<DefaultResourceProvider*>
        (m_GUISystem->getResourceProvider());

    // build string where our required files can be found.
    String dataDir = "";//(CEGUIHelper::ToCEGUIString(wxStandardPaths::Get().GetResourcesDir()));
    if ( !dataDir.empty() && ( dataDir[dataDir.length() - 1] != '/' ) )
            dataDir += '/';

    // on Windows, the datafiles here will be in a 'data' subdir
    #if defined(__WIN32__) || defined (_WIN32)
        dataDir += "data/";
    #endif

    // set the data path
    rp->setResourceGroupDirectory("editor_data", "./data/");
    rp->setDefaultResourceGroup( "editor_data" );

	//rp->setResourceGroupDirectory("__work_dir__", "../resource/interface/image/");
	

    // Load CEGUI scheme used in the editor
    SchemeManager::getSingleton().loadScheme( "CEImagesetEditor.scheme" );

    // Setup some subscriptions on the global event set
    GlobalEventSet& ges = CEGUI::GlobalEventSet::getSingleton();
    ges.subscribeEvent(
        "ElasticBox/SetNorthSouthCursor",
        Event::Subscriber(&EditorGLCanvas::handleRegionNorthSouthCursor, this));
    ges.subscribeEvent(
        "ElasticBox/SetEastWestCursor",
        Event::Subscriber(&EditorGLCanvas::handleRegionEastWestCursor, this));
    ges.subscribeEvent(
        "ElasticBox/SetNorthEastSouthWestCursor",
        Event::Subscriber(&EditorGLCanvas::handleRegionNorthEastSouthWestCursor, this));
    ges.subscribeEvent(
        "ElasticBox/SetNorthWestSouthEastCursor",
        Event::Subscriber(&EditorGLCanvas::handleRegionNorthWestSouthEastCursor, this));
    ges.subscribeEvent(
        "ElasticBox/SetMoveCursor",
        Event::Subscriber(&EditorGLCanvas::handleRegionMoveCursor, this));
    ges.subscribeEvent(
        "ElasticBox/SetNormalCursor",
        Event::Subscriber(&EditorGLCanvas::handleRegionNormalCursor, this));

    // we subscribe these globally since it allows us to quickly 'mute' these
    // events for all regions when we need to.
    ges.subscribeEvent(
        "Window/Moved",
        CEGUI::Event::Subscriber(&EditorGLCanvas::handleRegionModified, this));
    ges.subscribeEvent(
        "Window/Sized",
        CEGUI::Event::Subscriber(&EditorGLCanvas::handleRegionModified, this));

    // reset default resource group so we do not go searching in our data directory
    // when user tries to load in some files.
    rp->setDefaultResourceGroup( "" );

    createCEGUIWindows();

    // set CEGUI to initial state required by the editor
    resetCEGUI();
}

//-----------------------------------------------------------------------
void EditorGLCanvas::cleanupCEGUI()
{
    delete m_GUISystem;
    delete m_GUIRenderer;
    m_GUISystem = 0;
    m_GUIRenderer = 0;
}

//-----------------------------------------------------------------------
void EditorGLCanvas::createCEGUIWindows()
{
    // Get window manager which we wil use for a few jobs here.
    WindowManager& winMgr = WindowManager::getSingleton();

    try
    {
        m_imagesetStaticImage = winMgr.createWindow( "CEImagesetEditor/StaticImage", "__editor_static_image__" );
        m_imagesetStaticImage->setMaxSize( UVector2( UDim( 0.0f, 20480.0f ), UDim( 0.0f, 20480.0f ) ) );
        m_imagesetStaticImage->setPosition( UVector2( UDim( 0.0f, 0.0f ), UDim( 0.0f, 0.0f ) ) );
        m_imagesetStaticImage->setSize(
            UVector2( UDim( 0, m_realWidth ), UDim( 0, m_realHeight ) ) );
        m_GUISystem->setGUISheet( m_imagesetStaticImage );
    }
    catch ( Exception& guiException )
    {
        wxLogError ( wxT( "%s" ), CEGUIHelper::ToWXString( guiException.getMessage() ).c_str() );
    }
}

//-----------------------------------------------------------------------
bool EditorGLCanvas::resetCEGUI()
{
    // Cleanup regions
    deleteAllRegions();

    // reset image size to 0
    m_realWidth = 0;
    m_realHeight = 0;
    m_imagesetStaticImage->setSize(
        UVector2( UDim( 0, m_realWidth ), UDim( 0, m_realHeight ) ) );

    return true;
}

//-----------------------------------------------------------------------
void EditorGLCanvas::selectRegion( const wxString& name )
{
    if ( m_imagesetStaticImage->isChild( CEGUIHelper::ToCEGUIString( name ) ) )
    {
        CEGUI::Window* selectedWin = m_imagesetStaticImage->getChild( CEGUIHelper::ToCEGUIString( name ) );

        if ( !selectedWin->isActive() )
        {
            selectedWin->activate();
            Render();
        }
    }
}

//-----------------------------------------------------------------------
void EditorGLCanvas::renameRegion( const wxString& currentname, const wxString& newname )
{
    if ( m_imagesetStaticImage->isChild( CEGUIHelper::ToCEGUIString( currentname ) ) )
    {
        CEGUI::Window* toolbox = m_imagesetStaticImage->getChild( CEGUIHelper::ToCEGUIString( currentname ) );
        toolbox->rename( CEGUIHelper::ToCEGUIString( newname ) );
        selectRegion( newname );
    }
}

//-----------------------------------------------------------------------
void EditorGLCanvas::addRegion( const wxString& name, const wxRect& dim )
{
    ElasticBox* toolbox = static_cast<ElasticBox*>(
        WindowManager::getSingleton().createWindow( "CEImagesetEditor/ImageRegion", CEGUIHelper::ToCEGUIString( name ) ) );

    float posx = 0, posy = 0, width = 0, height = 0;

    if  ( ( m_realWidth != 0 ) && ( m_realHeight != 0) )
    {
        posx = static_cast<float>( dim.GetX() ) * m_zoomFactor;
        posy = static_cast<float>( dim.GetY() ) * m_zoomFactor;
        width = static_cast<float>( dim.GetWidth() ) * m_zoomFactor;
        height = static_cast<float>( dim.GetHeight() ) * m_zoomFactor;
    }
    // set size and position (all relative co-ords)
    toolbox->setPosition( UVector2( UDim( 0.0f, posx ), UDim( 0.0f, posy ) ) );
    toolbox->setSize( UVector2( UDim( 0.0f, width ), UDim( 0.0f, height ) ) );
    // set standard options we use for these ElasticBox widgets
    toolbox->setSizingBorderThickness( 3.0f );
    toolbox->setScaleSnap( m_zoomFactor );
    // Add this new box to the main image window.
    m_imagesetStaticImage->addChildWindow( toolbox );

    // subscibe some events...
    toolbox->subscribeEvent(
        ElasticBox::EventActivated,
        CEGUI::Event::Subscriber( &EditorGLCanvas::handleRegionActivated, this ) );

    toolbox->subscribeEvent(
        ElasticBox::EventDeactivated,
        CEGUI::Event::Subscriber( &EditorGLCanvas::handleRegionDeactivated, this ) );

    toolbox->subscribeEvent(
        ElasticBox::EventMouseEnters,
        CEGUI::Event::Subscriber(&EditorGLCanvas::handleRegionMouseEnter, this));

    toolbox->subscribeEvent(
        ElasticBox::EventMouseLeaves,
        CEGUI::Event::Subscriber(&EditorGLCanvas::handleRegionMouseLeave, this));

    // do this last, so that events handlers are called
    toolbox->activate();

    // Update display.
    Render();
}

//-----------------------------------------------------------------------
void EditorGLCanvas::setRegionArea( const wxString& name, const wxRect& dim )
{
    if ( m_imagesetStaticImage->isChild( CEGUIHelper::ToCEGUIString( name ) ) )
    {
        CEGUI::Window * win = m_imagesetStaticImage->getChild( CEGUIHelper::ToCEGUIString( name ) );

        float posx = static_cast<float>( dim.GetX() ) * m_zoomFactor;
        float posy = static_cast<float>( dim.GetY() ) * m_zoomFactor;
        float width = static_cast<float>( dim.GetWidth() ) * m_zoomFactor;
        float height = static_cast<float>( dim.GetHeight() ) * m_zoomFactor;

        win->setArea( URect( UDim( 0.0f, posx ),
                             UDim( 0.0f, posy ),
                             UDim( 0.0f, posx + width ),
                             UDim( 0.0f, posy + height ) ) );
    }
}

//-----------------------------------------------------------------------
void EditorGLCanvas::deleteRegion( const wxString& name )
{
    if ( m_imagesetStaticImage->isChild( CEGUIHelper::ToCEGUIString( name ) ) )
    {
        m_imagesetStaticImage->getChild( CEGUIHelper::ToCEGUIString( name ) )->destroy();
        Render();
    }
}

//-----------------------------------------------------------------------
void EditorGLCanvas::deleteAllRegions()
{
    typedef std::vector<CEGUI::Window*> DeleteList;
    DeleteList deletables;

    // collect together all the ElasticBoxes attached:
    for (size_t i = 0; i < m_imagesetStaticImage->getChildCount(); ++i)
    {
        CEGUI::Window* wnd = m_imagesetStaticImage->getChildAtIdx( i );

        if (wnd->testClassName("ElasticBox"))
            deletables.push_back(wnd);
    }

    // delete all the ElasticBoxes
    for (DeleteList::iterator it = deletables.begin(); it != deletables.end(); ++it)
        // Yes, yes, breaking my own rules, I know :-p
        (*it)->destroy();

    Render();
}

//-----------------------------------------------------------------------
void EditorGLCanvas::OnMouseMotion( wxMouseEvent& event )
{
    this->SetFocus();

    // see if we want to drag the editor image view around
    if ( event.Dragging() && event.MiddleIsDown() )
    {
        // extract mouse location
        wxPoint pt( event.GetPosition() );
        // calculate movement
        wxPoint change = m_dragPoint - pt;
        // update the drag point
        m_dragPoint = pt;

        // set new scrollbar positions
        m_scrollPosX += change.x;
        m_scrollPosY += change.y;
        updateScrollbars();
        // make sure the image position is in sync
        updateImageScrollPosition();
    }
    // not dragging, so do a normal CEGUI input injection.
    else
    {
        // tell CEGUI about the event
        System::getSingleton().injectMousePosition( event.GetX(), event.GetY() );

        // Check this, because we might get this event during document closing
        if ( isDocumentValid() &&
            ( event.GetX() != m_lastMouseX || event.GetY() != m_lastMouseY ) )
        {
            m_lastMouseX = event.GetX();
            m_lastMouseY = event.GetY();

            // updating statusbar with the mouse position on the imageset ( computing offset and zoom)
            float mousePosX = m_imagesetStaticImage->getXPosition().asAbsolute( m_imagesetStaticImage->getParentPixelWidth() );
            float mousePosY = m_imagesetStaticImage->getYPosition().asAbsolute( m_imagesetStaticImage->getParentPixelHeight() );
            mousePosX = ( m_lastMouseX - mousePosX ) / m_zoomFactor;
            mousePosY = ( m_lastMouseY - mousePosY ) / m_zoomFactor;
            m_view->updateMouseStatusBar(
                static_cast<int>( mousePosX ),
                static_cast<int>( mousePosY ) );

            // updating statusbar with the and zoom and the imageset size
            m_view->updateDocStatusBar( m_zoomFactor, m_realWidth, m_realHeight );
        }
    }

    Render();
}

//-----------------------------------------------------------------------
void EditorGLCanvas::OnMouseWheel( wxMouseEvent& event )
{
    // Check this, because we might get this event during document closing
    if ( isDocumentValid() )
    {
        int MouseWheelDirection = event.GetWheelRotation();
        computeZoomFactor( MouseWheelDirection );
        m_view->updateDocStatusBar( m_zoomFactor, m_realWidth, m_realHeight );
        Render();
    }

}

//-----------------------------------------------------------------------
void EditorGLCanvas::OnLeftDown( wxMouseEvent& event )
{
    // Check this, because we might get this event during document closing
    if ( isDocumentValid() )
    {
        // tell CEGUI about the event
        System::getSingleton().injectMouseButtonDown( CEGUI::LeftButton );
        Render();
    }

    event.Skip();
}

//-----------------------------------------------------------------------
void EditorGLCanvas::OnLeftUp( wxMouseEvent& event )
{
    // Check this, because we might get this event during document closing
    if ( isDocumentValid() )
    {
        // tell CEGUI about the event
        System::getSingleton().injectMouseButtonUp( CEGUI::LeftButton );
        Render();
    }

    event.Skip();
}

//-----------------------------------------------------------------------
void EditorGLCanvas::OnRightDown( wxMouseEvent& event )
{
    // Check this, because we might get this event during document closing
    if ( isDocumentValid() )
    {
        // tell CEGUI about the event
        System::getSingleton().injectMouseButtonDown( CEGUI::RightButton );
        Render();
    }

    event.Skip();
}

//-----------------------------------------------------------------------
void EditorGLCanvas::OnRightUp( wxMouseEvent& event )
{
    // Check this, because we might get this event during document closing
    if ( isDocumentValid() )
    {
        // tell CEGUI about the event
        System::getSingleton().injectMouseButtonUp( CEGUI::RightButton );
        Render();
    }

    event.Skip();
}

//-----------------------------------------------------------------------
void EditorGLCanvas::OnMiddleDown( wxMouseEvent& event )
{
    // initialise point where dragging started
    m_dragPoint = event.GetPosition();
    event.Skip();
}

//-----------------------------------------------------------------------
void EditorGLCanvas::computeZoomFactor( int MouseWheelDirection )
{
    // Logger::getSingleton().logEvent("imagesetSize '" + AppliConfigFile +"'.", Standard);
    if ( MouseWheelDirection > 0 )
        m_zoomFactor += 0.125;
    else if ( MouseWheelDirection < 0 )
        m_zoomFactor -= 0.125;

    // extrem down limit verif
    if ( m_zoomFactor < 0.25 )
        m_zoomFactor = 0.25;

    // extrem up limit verif
    if ( m_zoomFactor > 10.0 )
        m_zoomFactor = 10.0;

    float width = m_zoomFactor * m_realWidth;
    float height = m_zoomFactor * m_realHeight;

    // mute events to stop us from generating a lot of unrequired noise
    CEGUI::GlobalEventSet::getSingleton().setMutedState( true );
    // update the size of the main image
    m_imagesetStaticImage->setSize(
        UVector2( cegui_absdim( width ), cegui_absdim( height ) ) );

    // set snap scale for all defined image regions
    for (size_t i = 0; i < m_imagesetStaticImage->getChildCount(); ++i)
    {
        CEGUI::Window* wnd = m_imagesetStaticImage->getChildAtIdx( i );

        if (wnd->testClassName("ElasticBox"))
        {
            float old_zoom = static_cast<ElasticBox*>(wnd)->getScaleSnap();
            static_cast<ElasticBox*>(wnd)->setScaleSnap( m_zoomFactor );

            URect area( wnd->getArea() );
            // cancel out old zoom scaling
            area.d_min.d_x.d_offset /= old_zoom;
            area.d_min.d_y.d_offset /= old_zoom;
            area.d_max.d_x.d_offset /= old_zoom;
            area.d_max.d_y.d_offset /= old_zoom;
            // apply new zoom scaling
            area.d_min.d_x.d_offset *= m_zoomFactor;
            area.d_min.d_y.d_offset *= m_zoomFactor;
            area.d_max.d_x.d_offset *= m_zoomFactor;
            area.d_max.d_y.d_offset *= m_zoomFactor;
            // update window rect for new zoom factor
            wnd->setArea(area);
        }
    }

    // allow events to fire again.
    CEGUI::GlobalEventSet::getSingleton().setMutedState( false );

    updateScrollbars();
    // Here we invoke our nasty hack to work around a wxMac refresh issue.
    GetMainFrame()->Refresh();
}

//-----------------------------------------------------------------------
bool EditorGLCanvas::Reset()
{
    return resetCEGUI();
}

//-----------------------------------------------------------------------
void EditorGLCanvas::Render()
{
    SetCurrent();

    // clear colour buffer
    glClear( GL_COLOR_BUFFER_BIT );

    // CEGUI rendering
    renderImageset();

    glFlush();
    SwapBuffers();
}

//-----------------------------------------------------------------------
bool EditorGLCanvas::isDocumentValid() const
{
    return (m_view && m_view->GetDocument());
}

//-----------------------------------------------------------------------
void EditorGLCanvas::OnPaint( wxPaintEvent& event )
{
    // We need to do this to avoid a flood of paint events
    wxPaintDC paintDC( this );

    Render();
}

//-----------------------------------------------------------------------
void EditorGLCanvas::renderImageset()
{
    if ( CEGUI::System::getSingletonPtr() )
    {
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();

        // Render current cegui windows
        CEGUI::System::getSingleton().renderGUI();
    }
}

//-----------------------------------------------------------------------
void EditorGLCanvas::OnErase( wxEraseEvent& event )
{
}

//-----------------------------------------------------------------------
void EditorGLCanvas::OnKeyDown( wxKeyEvent& event )
{
}

//-----------------------------------------------------------------------
void EditorGLCanvas::OnKeyUp( wxKeyEvent& event )
{
    // Control key is used to change selection by keys to small steps (when you're near your target location.)
    if ( event.ControlDown() )
    {
        m_useBigSteps = true;
    }
}

//-----------------------------------------------------------------------
void EditorGLCanvas::OnScrollWin( wxScrollWinEvent& event )
{
    int orientation = event.GetOrientation();
    int adjustment, page, range, position;

    if ( orientation == wxVERTICAL )
    {
        page = m_scrollPageY;
        range = m_scrollDocY;
        position = m_scrollPosY;
    }
    else
    {
        page = m_scrollPageX;
        range = m_scrollDocX;
        position = m_scrollPosX;
    }

    wxEventType evtype = event.GetEventType();

    if ( evtype == wxEVT_SCROLLWIN_TOP )
        adjustment = -position;
    else if ( evtype == wxEVT_SCROLLWIN_BOTTOM )
        adjustment = range - page - position;
    else if ( evtype == wxEVT_SCROLLWIN_LINEDOWN )
        adjustment = 1;
    else if ( evtype == wxEVT_SCROLLWIN_LINEUP )
        adjustment = -1;
    else if ( evtype == wxEVT_SCROLLWIN_PAGEDOWN )
        adjustment = page;
    else if ( evtype == wxEVT_SCROLLWIN_PAGEUP )
        adjustment = -page;
    else if ( evtype == wxEVT_SCROLLWIN_THUMBTRACK )
        adjustment = event.GetPosition() - position;
    else if ( evtype == wxEVT_SCROLLWIN_THUMBRELEASE )
        adjustment = event.GetPosition() - position;
    else
        adjustment = 0;

    position += adjustment;

    if ( orientation == wxVERTICAL )
        m_scrollPosY = position;
    else
        m_scrollPosX = position;

    updateScrollbars();
    updateImageScrollPosition();
    event.Skip();
}

//-----------------------------------------------------------------------
void EditorGLCanvas::setBackgroundColour( const wxColour& colour )
{
    glClearColor(
        colour.Red() / 255.0f,
        colour.Green() / 255.0f,
        colour.Blue() / 255.0f,
        1.0f
    );
    Render();
}

//-----------------------------------------------------------------------
void EditorGLCanvas::updateScrollbars()
{
    // update internal tracking of this information.
    GetClientSize( &m_scrollPageX, &m_scrollPageY );
    m_scrollDocX = static_cast<int>( m_realWidth * m_zoomFactor );
    m_scrollDocY = static_cast<int>( m_realHeight * m_zoomFactor );

    // update scrolly bars
    SetScrollbar( wxVERTICAL, m_scrollPosY, m_scrollPageY, m_scrollDocY );
    SetScrollbar( wxHORIZONTAL, m_scrollPosX, m_scrollPageX, m_scrollDocX );

    // finally, update our local copies (saves doing our own constraints)
    m_scrollPosX = GetScrollPos( wxHORIZONTAL );
    m_scrollPosY = GetScrollPos( wxVERTICAL );
}

//-----------------------------------------------------------------------
void EditorGLCanvas::updateImageScrollPosition()
{
    m_imagesetStaticImage->setPosition( UVector2(
        UDim( 0, 0 - m_scrollPosX ), UDim( 0, 0 - m_scrollPosY ) ) );

    Render();
}

//-----------------------------------------------------------------------
bool EditorGLCanvas::handleRegionActivated( const CEGUI::EventArgs& e )
{
    const ActivationEventArgs& args = static_cast<const ActivationEventArgs&>(e);
    ElasticBox* win = static_cast<ElasticBox*>( args.window );

    if ( win )
    {
        win->setAlpha( 0.75f );

        // Check this, because we might get this event during document closing
        if ( isDocumentValid() )
        {
            wxString name = CEGUIHelper::ToWXString( win->getName() );
            m_view->getPropsPanel()->selectRegion( name );
        }
    }

    // event was handled
    return true;
}

//-----------------------------------------------------------------------
bool EditorGLCanvas::handleRegionDeactivated( const CEGUI::EventArgs& e )
{
    const ActivationEventArgs& args = static_cast<const ActivationEventArgs&>(e);
    ElasticBox* win = static_cast<ElasticBox*>( args.window );

    if ( win )
    {
        win->setAlpha( 0.3f );
    }

    // event was handled
    return true;
}

//-----------------------------------------------------------------------
bool EditorGLCanvas::handleRegionMouseEnter( const EventArgs& e )
{
    const WindowEventArgs& wea = static_cast<const WindowEventArgs&>(e);
    ElasticBox* win = static_cast<ElasticBox*>( wea.window );

    if ( win )
        win->activate();

    // event was handled
    return true;
}

//-----------------------------------------------------------------------
bool EditorGLCanvas::handleRegionMouseLeave( const EventArgs& e )
{
    const WindowEventArgs& wea = static_cast<const WindowEventArgs&>(e);
    ElasticBox* win = static_cast<ElasticBox*>( wea.window );

    if ( win )
        win->deactivate();

    // do this so we do not get stuck with a 'sizing' type cursor
    SetCursor( wxCURSOR_CROSS );

    // event was handled
    return true;
}

//-----------------------------------------------------------------------
bool EditorGLCanvas::handleRegionModified( const EventArgs& e )
{
    // Check this, because we might get this event during document closing
    if ( isDocumentValid() )
    {
        CEGUI::Window* wnd = static_cast<const WindowEventArgs&>(e).window;

        if ( wnd && wnd->testClassName( "ElasticBox" ) )
        {
            wxString name = CEGUIHelper::ToWXString( wnd->getName() );
            float posx    = wnd->getXPosition().d_offset / m_zoomFactor;
            float posy    = wnd->getYPosition().d_offset / m_zoomFactor;
            float width   = wnd->getWidth().d_offset / m_zoomFactor;
            float height  = wnd->getHeight().d_offset / m_zoomFactor;

            EditorDocument* document = static_cast<EditorDocument*>( m_view->GetDocument() );
            document->setRegionArea(name, wxRect(
                static_cast<int>( posx ),
                static_cast<int>( posy ),
                static_cast<int>( width ),
                static_cast<int>( height ) ), true );

            // event was handled
            return true;
        }
    }

    // event was not handled
    return false;
}

//-----------------------------------------------------------------------
bool EditorGLCanvas::handleRegionNorthSouthCursor(const CEGUI::EventArgs& e)
{
    SetCursor( wxCURSOR_SIZENS );
    return true;
}

//-----------------------------------------------------------------------
bool EditorGLCanvas::handleRegionEastWestCursor(const CEGUI::EventArgs& e)
{
    SetCursor( wxCURSOR_SIZEWE );
    return true;
}

//-----------------------------------------------------------------------
bool EditorGLCanvas::handleRegionNorthEastSouthWestCursor(const CEGUI::EventArgs& e)
{
    SetCursor( wxCURSOR_SIZENESW );
    return true;
}

//-----------------------------------------------------------------------
bool EditorGLCanvas::handleRegionNorthWestSouthEastCursor(const CEGUI::EventArgs& e)
{
    SetCursor( wxCURSOR_SIZENWSE );
    return true;
}

//-----------------------------------------------------------------------
bool EditorGLCanvas::handleRegionMoveCursor(const CEGUI::EventArgs& e)
{
    SetCursor( wxCURSOR_CROSS );
    return true;
}

//-----------------------------------------------------------------------
bool EditorGLCanvas::handleRegionNormalCursor(const CEGUI::EventArgs& e)
{
    SetCursor( wxCURSOR_CROSS );
    return true;
}
