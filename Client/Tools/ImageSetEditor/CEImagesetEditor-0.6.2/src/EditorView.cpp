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

// Standard wxWidget includes
#include "wx/wx.h" 
// Utility class for convertion between CEGUI and wxWidgets types
#include "CEGUIHelper.h"
#include "wx/docview.h"
#include "EditorView.h"
#include "CEImagesetEditor.h"
#include "EditorFrame.h"
#include "EditorGLCanvas.h"
#include "PropertiesPanel.h"

IMPLEMENT_DYNAMIC_CLASS( EditorView, wxView )

//-----------------------------------------------------------------------
EditorView::EditorView() :
    m_frame( 0 )
{
}

//-----------------------------------------------------------------------
bool EditorView::OnCreate( wxDocument* document, long WXUNUSED( flags ) )
{
    // Single-window mode
    m_frame = GetMainFrame();
    m_glcanvasImageset = m_frame->getCanvas();
    m_glcanvasImageset->setView ( this );
    m_glcanvasImageset->Reset();
    m_glcanvasImageset->Enable();
    m_propsPanel = m_frame->getPropsPanel();
    m_propsPanel->Enable();
    m_propsPanel->setView ( this );

    // Associate the appropriate frame with this view.
    SetFrame( m_frame );

    // Tell the frame about the document
    m_frame->SetDocument( static_cast<EditorDocument*>( document ) );

    // Make sure the document manager knows that this is the
    // current view.
    Activate( true );

    return true;
}

//-----------------------------------------------------------------------
bool EditorView::OnClose( bool WXUNUSED( deleteWindow ) )
{
    if ( !GetDocument()->Close() )
        return false;

    m_glcanvasImageset->ClearBackground();
    m_glcanvasImageset->setView ( 0 );
    m_glcanvasImageset->Disable();
    m_glcanvasImageset = 0;

    m_propsPanel->reset();
    m_propsPanel->Disable();

    SetFrame( 0 );

    // Tell the frame about the document
    m_frame->SetDocument( 0 );
    m_frame = 0;

    Activate( false );

    return true;
}

//-----------------------------------------------------------------------
void EditorView::OnUpdate( wxView* WXUNUSED( sender ), wxObject* WXUNUSED( hint ) )
{
    if ( m_frame && m_frame->IsShown() )
    {
        //wxLogDebug(wxT("Needs repaint."));
        m_glcanvasImageset->Refresh( false );
        m_glcanvasImageset->Render();
    }
}

//-----------------------------------------------------------------------
void EditorView::OnDraw( wxDC* dc )
{}

//-----------------------------------------------------------------------
void EditorView::onChangeFilename()
{
    updateTitle();
}

//-----------------------------------------------------------------------
void EditorView::onChangeImageFile( const wxString& newfilename , const wxString& trueFileName)
{
    m_propsPanel->setImageFilename( newfilename );
    m_glcanvasImageset->setImage( trueFileName );
}

//-----------------------------------------------------------------------
void EditorView::onChangeImagesetName( const wxString& newname )
{
    m_propsPanel->setImagesetName( newname );
}

//-----------------------------------------------------------------------
void EditorView::onChangeNativeResolution( const wxPoint& newnativeres )
{
    m_propsPanel->setNativeResolution( newnativeres );
}

//-----------------------------------------------------------------------
void EditorView::onChangeAutoScaled( const bool newAutoScaled )
{
    m_propsPanel->setAutoScaled( newAutoScaled );
}

//-----------------------------------------------------------------------
void EditorView::onAddRegion( const wxString& name, const wxRect& dim )
{
    m_propsPanel->addRegion( name, dim );
    m_glcanvasImageset->addRegion( name, dim );
}

//-----------------------------------------------------------------------
void EditorView::onDelRegion( const wxString& name )
{
    m_propsPanel->deleteRegion( name );
    m_glcanvasImageset->deleteRegion( name );
}

//-----------------------------------------------------------------------
void EditorView::onDelAllRegion()
{
    m_propsPanel->deleteAllRegions();
    m_glcanvasImageset->deleteAllRegions();
}

//-----------------------------------------------------------------------
void EditorView::onSetRegionArea( const wxString& name, const wxRect& dim, const bool evt_src )
{
    m_propsPanel->setRegionArea( name, dim );

    if (!evt_src)
        m_glcanvasImageset->setRegionArea( name, dim );
}

//-----------------------------------------------------------------------
void EditorView::onRenameRegion( const wxString& currentname, const wxString& newname )
{
    m_glcanvasImageset->renameRegion( currentname, newname );
    m_propsPanel->renameRegion( currentname, newname );
}

//-----------------------------------------------------------------------
void EditorView::updateTitle()
{
    wxString title ( wxTheApp->GetAppName() );

    if ( GetDocument() )
    {
        title << wxT( "- [" ) << GetDocument()->GetFilename() << wxT( "]" );
    }
    if ( m_frame )
    {
        m_frame->SetTitle( title );
    }
}

//-----------------------------------------------------------------------
void EditorView::updateMouseStatusBar( const float posx, const float posy )
{
    wxChar statusInfo [ 100 ];
    wxSnprintf(
        statusInfo,
        sizeof( statusInfo ),
        wxT( "pos[%d, %d]" ),
        static_cast<int>( posx ),
        static_cast<int>( posy ) ) ;

    if ( m_frame )
        m_frame->GetStatusBar()->SetStatusText( statusInfo, 0 );
}

//-----------------------------------------------------------------------
void EditorView::updateDocStatusBar( const float zoom, const float width, const float height )
{
    wxChar statusInfo [ 100 ];

    wxSnprintf(
        statusInfo,
        sizeof( statusInfo ),
        wxT( "size[%d, %d] zoom[%d]" ),
        static_cast<int>( width ),
        static_cast<int>( height ),
        static_cast<int>( zoom * 100.0f ) );

    if ( m_frame )
        m_frame->GetStatusBar()->SetStatusText( statusInfo, 1 );
}
