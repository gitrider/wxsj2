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

#ifndef _EDITOR_CANVAS_H_
#define _EDITOR_CANVAS_H_


// Include wxWidgets' headers
#include <wx/wx.h> 
// CEGUI headers
#include <CEGUISystem.h>
#include <RendererModules/OpenGLGUIRenderer/openglrenderer.h>

#undef wxUSE_GLCANVAS
#define wxUSE_GLCANVAS 1

#include "wx/glcanvas.h"
#include "EditorDocument.h"


class EditorView;

/** Provides OpenGL output. This canvas is the actual visible part of our "view"
* on the "document". The CEGUI system must be initialised when rendering, which is verified.
*/

class EditorGLCanvas : public wxGLCanvas
{
public:
    /** Constructor.*/
    EditorGLCanvas( EditorView* view, wxWindow* parent, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize );

    /** Destructor.*/
    ~EditorGLCanvas();

    /** Updates the openGL viewport setup.*/
    void changeSize( int newWidth, int newHeight );

    void setBackgroundColour( const wxColour& colour );
    void setImage( const wxString& name );

    // region manipulation
    void selectRegion( const wxString& name );
    void renameRegion( const wxString& currentname, const wxString& newname );
    void addRegion( const wxString& name, const wxRect& dim );
    void setRegionArea( const wxString& name, const wxRect& dim );
    void deleteRegion( const wxString& name );
    void deleteAllRegions();

    void computeZoomFactor( int MouseWheelDirection );

    void setView ( EditorView* aView )
    {
        m_view = aView;
    }

    /** Forces a reset of cegui.*/
    bool Reset();

    /** Forces a re-draw.*/
    void Render();

    /** Returns whether the document is still valid or not.*/
    bool isDocumentValid() const;

private:
    /** The view to which we are attached.*/
    EditorView* m_view;

    /** Pointer to cegui singleton.*/
    CEGUI::System* m_GUISystem;
    /** Pointer to cegui renderer.*/
    CEGUI::OpenGLRenderer* m_GUIRenderer;

    // CEGUI widgets
    CEGUI::Window* m_imagesetStaticImage;

    /** True when big steps should be used when moving selection by key.*/
    bool m_useBigSteps;

    //! Used to track drag motion.
    wxPoint m_dragPoint;

    /** Current document zoom. float because it's as relative value to original size egal 1.*/
    float m_zoomFactor ;

    /** Current canvas width. Used alot so it's cached.*/
    float m_realWidth;

    /** Current canvas height. Used alot so it's cached.*/
    float m_realHeight;

    /** Last known mouseX position.*/
    long m_lastMouseX;

    /** Last known mouseY position.*/
    long m_lastMouseY;

    // Here we have scroll state variables; required because wxWidgets has
    // different behavior on different platforms for wxWindow scrollbars.
    // (wxGTK automated them without intervention, wxMSW did not).
    //! current horizontal scroll position
    int m_scrollPosX;
    //! horizontal size of the 'document' (zoomed image width)
    int m_scrollDocX;
    //! horizontal 'page' size (width of the canvas widow)
    int m_scrollPageX;
    //! current vertical scroll position
    int m_scrollPosY;
    //! vertical size of the 'document' (zoomed image height)
    int m_scrollDocY;
    //! vertical 'page' size (height of the canvas widow)
    int m_scrollPageY;

    /** Renders the current imageset.*/
    void renderImageset();

    /** Initialises the CEGUI core + renderer. We don't cleanup the frame when
    * new view is spawned, so this is a good place to put the CEGUI functionality.*/
    void initialiseCEGUI();

    /** Shut down the CEGUI system, freeing all CEGUI based resources.*/
    void cleanupCEGUI();

    bool resetCEGUI();
    void createCEGUIWindows();


    //! Update the scrollbars
    void updateScrollbars();
    //! update scroll position of the image based on scrollbar values.
    void updateImageScrollPosition();

    // handlers which we use to trigger cursor changes.
    bool handleRegionNorthSouthCursor(const CEGUI::EventArgs& e);
    bool handleRegionEastWestCursor(const CEGUI::EventArgs& e);
    bool handleRegionNorthEastSouthWestCursor(const CEGUI::EventArgs& e);
    bool handleRegionNorthWestSouthEastCursor(const CEGUI::EventArgs& e);
    bool handleRegionMoveCursor(const CEGUI::EventArgs& e);
    bool handleRegionNormalCursor(const CEGUI::EventArgs& e);

    //! handler used to auto activate image regions
    bool handleRegionMouseEnter(const CEGUI::EventArgs& e);
    //! handler used to auto deactivate image regions
    bool handleRegionMouseLeave(const CEGUI::EventArgs& e);
    //! handler used to react when a region is activated
    bool handleRegionActivated( const CEGUI::EventArgs& e );
    //! handler used to react when a region is deactivated
    bool handleRegionDeactivated( const CEGUI::EventArgs& e );
    //! handler used to update properties panel when region changes
    bool handleRegionModified( const CEGUI::EventArgs& e );

    //
    // wxWidgets bits below
    //
    /** Handled wxSizeEvent. Propagate the new resolution to the gui renderer.*/
    void OnResize( wxSizeEvent& event );

    /** Event handler: notifies us that the canvas needs a repaint. We render the current imageset.*/
    void OnPaint( wxPaintEvent& event );

    /** Event handler: notifies us that the canvas needs a refresh of the background.
    * a good moment to draw the current background, if any.*/
    void OnErase( wxEraseEvent& event );

    /** Event handler: we handle mouse input here.*/
    void OnMouseMotion( wxMouseEvent& event );

    /** Event handler: we handle mouse input here.*/
    void OnLeftDown( wxMouseEvent& event );

    /** Event handler: we handle mouse input here.*/
    void OnLeftUp( wxMouseEvent& event );

    /** Event handler: we handle mouse input here.*/
    void OnRightDown( wxMouseEvent& event );

    /** Event handler: we handle mouse input here.*/
    void OnRightUp( wxMouseEvent& event );

    /** Event handler: we handle mouse input here.*/
    void OnMiddleDown( wxMouseEvent& event );

    /** Event handler: we handle mouse input here.*/
    void OnMiddleUp( wxMouseEvent& event );

    /** Event handler: we handle mouse input here.*/
    void OnMouseWheel( wxMouseEvent& event );

    /** Event handler: handles key-down events. We use it to move the current selection.*/
    void OnKeyDown( wxKeyEvent& event );

    /** Event handler: stops moving the current selection.*/
    void OnKeyUp( wxKeyEvent& event );

    /** Event handler: scrollbar changes */
    void OnScrollWin( wxScrollWinEvent& event );

    DECLARE_EVENT_TABLE()
};

#endif // _EDITOR_CANVAS_H_
