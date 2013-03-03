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

#ifdef __linux__
# include <unistd.h>
#endif

#include <wx/dir.h>
#include <wx/colordlg.h>

#include "EditorFrame.h"
#include "EditorGLCanvas.h" 
#include "DialogResourceGroups.h"
// Utility class for convertion between CEGUI- and wxWidgets types
#include "CEGUIHelper.h"

// Toolbar icons
#include "bitmaps/new.xpm"
#include "bitmaps/open.xpm"
#include "bitmaps/save.xpm"
#include "bitmaps/cut.xpm"
#include "bitmaps/copy.xpm"
#include "bitmaps/paste.xpm"
#include "bitmaps/delete.xpm"
#include "bitmaps/help.xpm"

// ID codes for the menu items and top-level UI components
const int ID_MENU_BACKGROUND_COLOR = 12000;
const int ID_MENU_RESOURCE_GROUPS = 12001;

const int ID_SPLITTER_FRAME = 11000;

using namespace CEGUI;

IMPLEMENT_CLASS( EditorFrame, wxDocParentFrame )

//-----------------------------------------------------------------------
BEGIN_EVENT_TABLE( EditorFrame, wxDocParentFrame )
    EVT_PAINT( EditorFrame::OnPaint )
    EVT_MENU( ID_MENU_BACKGROUND_COLOR, EditorFrame::OnChangeBackgroundColour )
    EVT_MENU( ID_MENU_RESOURCE_GROUPS, EditorFrame::OnEditResourceGroups )
    EVT_PAINT( EditorFrame::OnPaint )
END_EVENT_TABLE()

//-----------------------------------------------------------------------
EditorFrame::EditorFrame( wxDocManager* manager, wxFrame* frame, wxWindowID id, const wxString& title,
                          const wxPoint& pos, const wxSize& size, const long type ) :
    wxDocParentFrame( manager, frame, id, title, pos, size, type ),
    m_document( 0 ),
    m_propsPanel( 0 ),
    m_glcanvasImageset( 0 ),
    m_resGrpsEditor( 0 ),
    m_backgroundColour( 255, 170, 200 )
{
    // Add a menu bar, a toolbar and a statusbar
    AttachMenubar();
    AttachToolbar();
    CreateStatusBar( 2 );

    // Create the main dialog
    createContent();

    // pre-create the resource groups editor
    m_resGrpsEditor = new DialogResourceGroups(
        this, 0, wxT( "Resource Groups Editor" ), wxDefaultPosition, wxSize( 512, 480 ) );
}

//-----------------------------------------------------------------------
EditorFrame::~EditorFrame()
{
}

//-----------------------------------------------------------------------
void EditorFrame::AttachMenubar()
{
    wxMenuBar * menu_bar = new wxMenuBar;

    wxMenu* menu_file = new wxMenu;
    menu_file->Append( wxID_NEW, wxT( "New\tCtrl-N" ), wxT( "Create a new document." ) );
    menu_file->Append( wxID_OPEN, wxT( "Load...\tCtrl-L" ), wxT( "Open an existing document." ) );
    menu_file->Append( wxID_SAVEAS, wxT( "Save...\tCtrl-S" ), wxT( "Save the active document." ) );
    menu_file->Append( wxID_CLOSE, wxT( "&Close \tCtrl-K" ), wxT( "Close the active document." ) );
    menu_file->AppendSeparator();
    menu_file->Append( wxID_EXIT, wxT( "Quit\tCtrl-Q" ), wxT( "Quit the application; prompts to save document." ) );
    menu_bar->Append( menu_file, wxT( "File" ) );

    wxMenu* menu_options = new wxMenu;
    menu_options->Append( ID_MENU_BACKGROUND_COLOR, wxT( "Set Background colour..." ), wxT( "choice the colour of the background." ) );
    menu_options->AppendSeparator();
    menu_options->Append( ID_MENU_RESOURCE_GROUPS, wxT( "Resource Groups..." ), wxT( "View and edit currently defined resource groups and directories." ) );
    menu_bar->Append( menu_options, wxT( "Options" ) );

    wxMenu* menu_help = new wxMenu;
    menu_help->Append( wxID_HELP, wxT( "Help..." ), wxT( "" ) );
    menu_help->Append( wxID_ABOUT, wxT( "About..." ), wxT( "Display program information, version number and copyright." ) );
    menu_bar->Append( menu_help, wxT( "?" ) );

    SetMenuBar( menu_bar );
}

//-----------------------------------------------------------------------
void EditorFrame::AttachToolbar()
{
    // Create toolbar with icons-only
    wxToolBar * toolBar = new wxToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL ); // |wxNO_BORDER
    //
    toolBar->AddTool( wxID_NEW, wxBitmap ( new_xpm ) );
    toolBar->AddTool( wxID_OPEN, wxBitmap ( open_xpm ) );
    toolBar->AddTool( wxID_SAVE, wxBitmap ( save_xpm ) );
    toolBar->AddSeparator();
    toolBar->AddTool( wxID_CUT, wxBitmap ( cut_xpm ) );
    toolBar->AddTool( wxID_COPY, wxBitmap ( copy_xpm ) );
    toolBar->AddTool( wxID_PASTE, wxBitmap ( paste_xpm ) );
    toolBar->AddTool( wxID_CLEAR, wxBitmap ( delete_xpm ) );
    toolBar->AddSeparator();
    toolBar->AddTool( wxID_ABOUT, wxBitmap ( help_xpm ) );
    // Don't forget this one!
    toolBar->Realize();
    SetToolBar( toolBar );
}

//-----------------------------------------------------------------------
void EditorFrame::SetDocument( EditorDocument* document )
{
    m_document = document;
}

//-----------------------------------------------------------------------
void EditorFrame::createContent()
{

    m_splitter = new wxSplitterWindow( this, ID_SPLITTER_FRAME, wxDefaultPosition, wxDefaultSize, wxSP_NOBORDER );
    m_splitter->SetSashGravity( 0.0 );

    // spitter child, left part
    m_propsPanel = new PropertiesPanel( m_splitter, wxDefaultPosition, wxSize( 300, 100 ) );
    m_propsPanel->createContent();
    m_propsPanel->Disable();
}

//-----------------------------------------------------------------------
EditorGLCanvas* EditorFrame::AttachCanvas( EditorView* view )
{
    // Create the GLCanvas. This apprauch provides MDI support maybe in the future
    EditorGLCanvas* canvas = new EditorGLCanvas( view, m_splitter, wxDefaultPosition, wxDefaultSize );
    // add the canvas to the frame
    // Tell it about the initial size
    m_splitter->SplitVertically( m_propsPanel, canvas, 250 );
    canvas->ClearBackground();
    canvas->Disable();
    canvas->setBackgroundColour( m_backgroundColour );

    // Copy into member
    m_glcanvasImageset = canvas;

    return canvas;
}

//-----------------------------------------------------------------------
void EditorFrame::OnChangeBackgroundColour( wxCommandEvent& WXUNUSED( event ) )
{
    wxColourData colourBackground;
    colourBackground.SetColour( m_backgroundColour );

    // Show colour selector
    wxColourDialog dialog( this, &colourBackground );
    if ( dialog.ShowModal() == wxID_OK )
    {
        m_backgroundColour = dialog.GetColourData().GetColour();

        // set the colour on the canvas
        m_glcanvasImageset->setBackgroundColour( m_backgroundColour );
    }
}

//-----------------------------------------------------------------------
void EditorFrame::OnEditResourceGroups( wxCommandEvent& event )
{
    // the DialogResourceGroups class is basically a self-contained
    // mini-editor for resource groups, so all we have to do is
    // invoke the dialog each time it's asked for.
    m_resGrpsEditor->invoke();
}

//-----------------------------------------------------------------------
void EditorFrame::OnPaint( wxPaintEvent& event )
{
    wxPaintDC paintDC( this );
    
    /*
     * There appears to be an issue with wxMac that causes the wxGLCanvas not to
     * get updated correctly until the containing wxFrame gets resized, to work
     * around this issue (at least for now) we manually trigger a no-op resize
     * so that other size effects occur correctly.  Of in other words, this is
     * a nasty hack!
     */
    if (m_glcanvasImageset)
        //m_glcanvasImageset->SetSize(m_glcanvasImageset->GetSize());
        SetSize(GetSize());
}

//-----------------------------------------------------------------------
