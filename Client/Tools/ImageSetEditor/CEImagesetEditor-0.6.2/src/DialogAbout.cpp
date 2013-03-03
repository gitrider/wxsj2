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
#include "DialogAbout.h"

#include "Config.h"

#include <CEGUIVersion.h>

#define BORDER            5

//--------------------------------------------------------------------------------
BEGIN_EVENT_TABLE( DialogAbout, wxDialog )
    EVT_BUTTON( ID_OK_BUTTON, DialogAbout::OnClickOk )
END_EVENT_TABLE()

//--------------------------------------------------------------------------------------
DialogAbout::DialogAbout( wxWindow* parent )
        : wxDialog( parent, wxID_ANY, wxT( "About CEImagesetEditor" ), wxDefaultPosition, wxSize( 400, 300 ) )
{
    // build version string
    wxString ver;
    ver.Printf( wxT( "Version %s (built with CEGUI %d.%d.%d)"),
                wxT( PACKAGE_VERSION ),
                CEGUI_VERSION_MAJOR,
                CEGUI_VERSION_MINOR,
                CEGUI_VERSION_PATCH );

    wxStaticText * title = new wxStaticText( this, wxID_ANY, wxT( "CEImagesetEditor" ) );
    wxStaticText* version = new wxStaticText( this, wxID_ANY, ver );
    wxTextCtrl* about = new wxTextCtrl( this, wxID_ANY,
                                        wxT( "CEImagesetEditor is the official imageset editor tool of Crazy Eddie's GUI System.\n\n" )
                                        wxT( "Project website: http://www.cegui.org.uk/\n" )
                                        wxT( "Report bugs at http://mantis.cegui.org.uk/\n\n" )
                                        wxT( "Martin Fleurent:\n" )
                                        wxT( "\tOriginal version code.\n\n" )
                                        wxT( "Paul D. Turner:\n" )
                                        wxT( "\tProject co-ordination.\n" )
                                        wxT( "\tInitial code cleanup work.\n" )
                                        wxT( "\tCEGUI 0.5.x compatibility updates.\n" )
                                        wxT( "\tElasticBox custom widget.\n\n" )
                                        wxT( "Frederico Jeronimo:\n" )
                                        wxT( "\tMSVC++ projects (replaced premake)\n" )
                                        wxT( "\tWin32 static config updates.\n\n" )
                                        wxT( "Tomas Lindquist Olsen:\n" )
                                        wxT( "\tAdditional Win32 testing.\n" )
                                        wxT( "\tpremake project generation support.\n\n" )
                                        wxT( "(C) 2005-2008 CEGUI Team.\n" )
                                        wxT( "Contact: team@cegui.org.uk" )
                                        , wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY | wxTE_RICH | wxTE_RICH2 );

    wxButton* ok = new wxButton( this, ID_OK_BUTTON, wxT( "&OK" ) );

    // further init controls
    wxFont font = title->GetFont();
    font.SetWeight( wxBOLD );
    title->SetFont( font );

    ok->SetDefault();

    // Size the components vertically
    wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );
    sizer->Add( title, 0, wxEXPAND | wxALL, BORDER );
    sizer->Add( version, 0, wxEXPAND | wxALL, BORDER );
    sizer->Add( about, 1, wxEXPAND | wxALL, BORDER );
    sizer->Add( ok, 0, wxALL, BORDER );

    SetSizer( sizer );

    // centre dialog
    Centre();

    Show( true );
}

//--------------------------------------------------------------------------------------
void DialogAbout::OnClickOk( wxCommandEvent& event )
{

    Close();
}
