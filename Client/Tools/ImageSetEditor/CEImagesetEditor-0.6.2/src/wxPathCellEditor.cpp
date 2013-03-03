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

// The portions of this code that relate to the text editing are basically
// taken from the built in wxWidgets text editor for cells: wxGridCellTextEditor.


#include "wxPathCellEditor.h"

#include <wx/wx.h>
#include <wx/log.h>
#include <wx/filename.h>

#if defined(__WXMOTIF__)
 #define WXUNUSED_MOTIF(identifier)  WXUNUSED(identifier)
#else
 #define WXUNUSED_MOTIF(identifier)  identifier
#endif

#if defined(__WXGTK__)
 #define WXUNUSED_GTK(identifier)    WXUNUSED(identifier)
#else
 #define WXUNUSED_GTK(identifier)    identifier
#endif

#define ID_BROWSE_BUTTON 10000
#define ID_DIRECTORY_TEXT 10001

//--------------------------------------------------------------------------------
BEGIN_EVENT_TABLE( wxPathCellEditor, wxEvtHandler )
EVT_BUTTON( ID_BROWSE_BUTTON, wxPathCellEditor::OnBrowseButtonClick )
END_EVENT_TABLE()

//--------------------------------------------------------------------------------
wxPathCellEditor::wxPathCellEditor() :
        m_maxChars( 0 ),
        m_browse( 0 ),
        m_text( 0 ),
        m_sizer( 0 )
{}

//--------------------------------------------------------------------------------
void wxPathCellEditor::Create( wxWindow* parent,
                               wxWindowID id,
                               wxEvtHandler* evtHandler )
{
    m_control = new wxControl( parent, id, wxDefaultPosition, wxDefaultSize, 0 );
    m_browse = new wxButton( m_control, ID_BROWSE_BUTTON, wxT( "..." ), wxDefaultPosition, wxSize( 25, -1 ), 0 );
    m_text = new wxTextCtrl( m_control, ID_DIRECTORY_TEXT, wxEmptyString,
                             wxDefaultPosition, wxDefaultSize
#if defined(__WXMSW__)
                             , wxTE_PROCESS_TAB | wxTE_AUTO_SCROLL | wxNO_BORDER
#endif
                           );

    // set max length allowed in the textctrl, if the parameter was set
    if ( m_maxChars != 0 )
        m_text->SetMaxLength( m_maxChars );

    m_sizer = new wxBoxSizer( wxHORIZONTAL );
    m_sizer->Add( m_text, 1, wxGROW | wxALIGN_CENTER_HORIZONTAL | wxALL, 0 );
    m_sizer->Add( m_browse, 0, wxALIGN_BOTTOM | wxALIGN_CENTER_HORIZONTAL | wxALL, 0 );

    m_control->SetSizer( m_sizer );
    // make sure we hear about events
    m_control->PushEventHandler( this );

    wxGridCellEditor::Create( parent, id, evtHandler );
}

//--------------------------------------------------------------------------------
void wxPathCellEditor::PaintBackground( const wxRect& WXUNUSED( rectCell ),
                                        wxGridCellAttr * WXUNUSED( attr ) )
{
    // as we fill the entire client area,
    // don't do anything here to minimize flicker
}

//--------------------------------------------------------------------------------
void wxPathCellEditor::SetSize( const wxRect& rectOrig )
{
    wxRect rect( rectOrig );

    // Make the edit control large enough to allow for internal margins
    //
    // TODO: remove this if the text ctrl sizing is improved esp. for unix
    //
#if defined(__WXGTK__)

    if ( rect.x != 0 )
    {
        rect.x += 1;
        rect.y += 1;
        rect.width -= 1;
        rect.height -= 1;
    }
#elif defined(__WXMSW__)
    if ( rect.x == 0 )
        rect.x += 2;
    else
        rect.x += 3;

    if ( rect.y == 0 )
        rect.y += 2;
    else
        rect.y += 3;

    rect.width -= 2;
    rect.height -= 2;
#else

    int extra_x = ( rect.x > 2 ) ? 2 : 1;
    int extra_y = ( rect.y > 2 ) ? 2 : 1;

#if defined(__WXMOTIF__)

    extra_x *= 2;
    extra_y *= 2;
#endif

    rect.SetLeft( wxMax( 0, rect.x - extra_x ) );
    rect.SetTop( wxMax( 0, rect.y - extra_y ) );
    rect.SetRight( rect.GetRight() + 2 * extra_x );
    rect.SetBottom( rect.GetBottom() + 2 * extra_y );
#endif

    wxGridCellEditor::SetSize( rect );
    m_control->Layout();
}

//--------------------------------------------------------------------------------
void wxPathCellEditor::BeginEdit( int row, int col, wxGrid* grid )
{
    wxASSERT_MSG( m_control, wxT( "The wxGridCellEditor must be created first!" ) );

    m_startValue = grid->GetTable()->GetValue( row, col );

    DoBeginEdit( m_startValue );
}

//--------------------------------------------------------------------------------
void wxPathCellEditor::DoBeginEdit( const wxString& startValue )
{
    Text()->SetValue( startValue );
    Text()->SetInsertionPointEnd();
    Text()->SetSelection( -1, -1 );
    Text()->SetFocus();
}

//--------------------------------------------------------------------------------
bool wxPathCellEditor::EndEdit( int row, int col, wxGrid* grid )
{
    wxASSERT_MSG( m_control, wxT( "The wxGridCellEditor must be created first!" ) );

    bool changed = false;
    wxString value = Text()->GetValue();
    if ( value != m_startValue )
        changed = true;

    if ( changed )
        grid->GetTable()->SetValue( row, col, value );

    m_startValue = wxEmptyString;

    // No point in setting the text of the hidden control
    //Text()->SetValue(m_startValue);

    return changed;
}

//--------------------------------------------------------------------------------
void wxPathCellEditor::Reset()
{
    wxASSERT_MSG( m_control, wxT( "The wxGridCellEditor must be created first!" ) );

    DoReset( m_startValue );
}

//--------------------------------------------------------------------------------
void wxPathCellEditor::DoReset( const wxString& startValue )
{
    Text()->SetValue( startValue );
    Text()->SetInsertionPointEnd();
}

//--------------------------------------------------------------------------------
bool wxPathCellEditor::IsAcceptedKey( wxKeyEvent& event )
{
    return wxGridCellEditor::IsAcceptedKey( event );
}

//--------------------------------------------------------------------------------
void wxPathCellEditor::StartingKey( wxKeyEvent& event )
{
    // Since this is now happening in the EVT_CHAR event EmulateKeyPress is no
    // longer an appropriate way to get the character into the text control.
    // Do it ourselves instead.  We know that if we get this far that we have
    // a valid character, so not a whole lot of testing needs to be done.

    wxTextCtrl * tc = Text();
    wxChar ch;
    long pos;

#if wxUSE_UNICODE

    ch = event.GetUnicodeKey();
    if ( ch <= 127 )
        ch = ( wxChar ) event.GetKeyCode();
#else

    ch = ( wxChar ) event.GetKeyCode();
#endif

    switch ( ch )
    {
    case WXK_DELETE:
        // delete the character at the cursor
        pos = tc->GetInsertionPoint();
        if ( pos < tc->GetLastPosition() )
            tc->Remove( pos, pos + 1 );
        break;

    case WXK_BACK:
        // delete the character before the cursor
        pos = tc->GetInsertionPoint();
        if ( pos > 0 )
            tc->Remove( pos - 1, pos );
        break;

    default:
        tc->WriteText( ch );
        break;
    }
}

//--------------------------------------------------------------------------------
void wxPathCellEditor::HandleReturn( wxKeyEvent&
                                     WXUNUSED_GTK( WXUNUSED_MOTIF( event ) ) )
{
#if defined(__WXMOTIF__) || defined(__WXGTK__)
    // wxMotif needs a little extra help...
    size_t pos = ( size_t ) ( Text()->GetInsertionPoint() );
    wxString s( Text()->GetValue() );
    s = s.Left( pos ) + wxT( "\n" ) + s.Mid( pos );
    Text()->SetValue( s );
    Text()->SetInsertionPoint( pos );
#else
    // the other ports can handle a Return key press
    //
    event.Skip();
#endif
}

//--------------------------------------------------------------------------------
void wxPathCellEditor::SetParameters( const wxString& params )
{
    if ( !params )
    {
        // reset to default
        m_maxChars = 0;
    }
    else
    {
        long tmp;
        if ( params.ToLong( &tmp ) )
        {
            m_maxChars = ( size_t ) tmp;
        }
        else
        {
            wxLogDebug( _T( "Invalid wxPathCellEditor parameter string '%s' ignored" ), params.c_str() );
        }
    }
}

//--------------------------------------------------------------------------------
wxString wxPathCellEditor::GetValue() const
{
    return Text()->GetValue();
}

//--------------------------------------------------------------------------------
void wxPathCellEditor::OnBrowseButtonClick( wxCommandEvent& event )
{
    wxFileName currpath( m_text->GetValue() );
    wxString dirpath = wxDirSelector( wxT( "Select Directory" ),
                                      currpath.GetPath() );

    // if something was selected
    if ( !dirpath.empty() )
    {
        // append slash if required
        int idx = dirpath.size() - 1;
        if ( dirpath[idx] != '/' )
            dirpath.append( 1, '/' );

        // set new text
        m_text->SetValue( dirpath );
        // put insert point at the end
        m_text->SetInsertionPointEnd();
        // set focus to parent
        m_control->SetFocus();
    }

    event.Skip();
}
