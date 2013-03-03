// Doxygen lines:
//

/*! \mainpage Welcome to the CEImagesetEditor, an imageset editor for the CEGUI library.
 * The CEGUI library can be found here: http://www.cegui.org.uk/
 *
 * This product is written with wxWidgets: http://www.wxwidgets.org/
 *
 * Original code copyright (C) 2006 Martin Fleurent.
 * Currently copyright (C) 2006 The CEGUI team.
 */

////////////////////////////////////////////////////////////////////////////////
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
#include "wx/image.h"
#include "CEImagesetEditor.h"
#include "EditorFrame.h"
#include "EditorDocument.h"
#include "EditorView.h"
#include "CEGUIHelper.h"
#include "DialogAbout.h"

using namespace CEGUI;

//-----------------------------------------------------------------------
EditorFrame* GlobalFrame = 0;

IMPLEMENT_APP( CEImagesetEditor )

//-----------------------------------------------------------------------
BEGIN_EVENT_TABLE(CEImagesetEditor, wxApp)
    EVT_MENU(wxID_ABOUT, CEImagesetEditor::OnAbout)
END_EVENT_TABLE()


//-----------------------------------------------------------------------
CEImagesetEditor::CEImagesetEditor() :
    m_docManager( 0 )
{
}

//-----------------------------------------------------------------------
bool CEImagesetEditor::OnInit( void )
{
    // Log some platform info
    wxLogDebug( wxT( "Default Gui font: %s, %d" ), wxSystemSettings::GetFont( wxSYS_DEFAULT_GUI_FONT ).GetFaceName().c_str(), wxSystemSettings::GetFont( wxSYS_DEFAULT_GUI_FONT ).GetPointSize() );

    // Create a document manager
    m_docManager = new wxDocManager;

    wxImage::AddHandler( new wxPNGHandler );
    // Create a template relating drawing documents to their views
    new wxDocTemplate( m_docManager, wxT( "imageset files" ), wxT( "*.xml;*.imageset" ), wxT( "" ), wxT( "xml" ), wxT( "Editor Document" ), wxT( "Editor View" ),
                       CLASSINFO( EditorDocument ), CLASSINFO( EditorView ) );

    // If we've only got one window, we only get to edit
    // one document at a time.
    m_docManager->SetMaxDocsOpen( 1 );

    try
    {

        // Create Single-Document main frame, just look default position and dimensions
        GlobalFrame = new EditorFrame( m_docManager, 0, wxID_ANY, wxT( "CEImagesetEditor" ), wxDefaultPosition, wxSize( 800, 600 ), wxDEFAULT_FRAME_STYLE );

        // Prepare for the rendering canvas
        // We need to show the frame otherwise the CEGUI opengl renderer won't
        // be able to initialze, because the opengl viewport is not valid then.*/
        GlobalFrame->Centre( wxBOTH );
        GlobalFrame->Show( true );
        // This needs to be in the try-block!
        GlobalFrame->AttachCanvas( 0 );
        SetTopWindow( GlobalFrame );

        return true;
    }
    catch ( CEGUI::Exception & guiException )
    {
        // If something goes wrong, show user
        wxLogError ( wxT( "ImagesetEditor::OnInit - Error: %s" ), CEGUIHelper::ToWXString( guiException.getMessage() ).c_str() );
        return false;
    }

}

//-----------------------------------------------------------------------
int CEImagesetEditor::OnExit()
{
    // Cleanup
    delete m_docManager;
    //delete COptions::GetInstancePtr();
    return 0;
}

//-----------------------------------------------------------------------
void CEImagesetEditor::OnAbout(wxCommandEvent& event)
{
  new DialogAbout(GlobalFrame);
}

//-----------------------------------------------------------------------
EditorFrame *GetMainFrame()
{
    return GlobalFrame;
}
