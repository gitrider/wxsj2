/////////////////////////////////////////////////////////////////////////////
// Name:        common/dlgcmn.cpp
// Purpose:     common (to all ports) wxDialog functions
// Author:      Vadim Zeitlin
// Modified by:
// Created:     28.06.99
// RCS-ID:      $Id: dlgcmn.cpp,v 1.41 2005/03/22 10:34:35 JS Exp $
// Copyright:   (c) Vadim Zeitlin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "dialogbase.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/button.h"
    #include "wx/dialog.h"
    #include "wx/dcclient.h"
    #include "wx/intl.h"
    #include "wx/settings.h"
    #include "wx/stattext.h"
    #include "wx/sizer.h"
    #include "wx/button.h"
    #include "wx/containr.h"
#endif

//--------------------------------------------------------------------------
// wxDialogBase
//--------------------------------------------------------------------------

// FIXME - temporary hack in absence of wxtopLevelWindow, should be always used
#ifdef wxTopLevelWindowNative
BEGIN_EVENT_TABLE(wxDialogBase, wxTopLevelWindow)
    WX_EVENT_TABLE_CONTROL_CONTAINER(wxDialogBase)
END_EVENT_TABLE()

WX_DELEGATE_TO_CONTROL_CONTAINER(wxDialogBase)
#endif

void wxDialogBase::Init()
{
    m_returnCode = 0;
    m_affirmativeId = wxID_OK;
    
    // the dialogs have this flag on by default to prevent the events from the
    // dialog controls from reaching the parent frame which is usually
    // undesirable and can lead to unexpected and hard to find bugs
    SetExtraStyle(GetExtraStyle() | wxWS_EX_BLOCK_EVENTS);

#ifdef wxTopLevelWindowNative // FIXME - temporary hack, should be always used!
    m_container.SetContainerWindow(this);
#endif
}

#if wxUSE_STATTEXT // && wxUSE_TEXTCTRL

wxSizer *wxDialogBase::CreateTextSizer( const wxString& message )
{
    bool is_pda = (wxSystemSettings::GetScreenType() <= wxSYS_SCREEN_PDA);

    wxString text = message;

    // I admit that this is complete bogus, but it makes
    // message boxes work for pda screens temporarily..
    int max_width = -1;
    if (is_pda)
    {
        max_width = wxSystemSettings::GetMetric( wxSYS_SCREEN_X ) - 25;
        text += wxT('\n');
    }


    wxBoxSizer *box = new wxBoxSizer( wxVERTICAL );

    // get line height for empty lines
    int y = 0;
    wxFont font( GetFont() );
    if (!font.Ok())
        font = *wxSWISS_FONT;
    GetTextExtent( wxT("H"), (int*)NULL, &y, (int*)NULL, (int*)NULL, &font);

    size_t last_space = 0;
    wxString line;
    for ( size_t pos = 0; pos < text.length(); pos++ )
    {
        switch ( text[pos] )
        {
            case wxT('\n'):
                if (!line.empty())
                {
                    wxStaticText *s = new wxStaticText( this, wxID_ANY, line );
                    box->Add( s );
                    line = wxEmptyString;
                }
                else
                {
                    box->Add( 5, y );
                }
                break;

            case wxT('&'):
                // this is used as accel mnemonic prefix in the wxWidgets
                // controls but in the static messages created by
                // CreateTextSizer() (used by wxMessageBox, for example), we
                // don't want this special meaning, so we need to quote it
                line += wxT('&');

                // fall through to add it normally too

            default:
                if (text[pos] == wxT(' '))
                    last_space = pos;

                line += message[pos];

                if (is_pda)
                {
                    int width = 0;
                    GetTextExtent( line, &width, (int*)NULL, (int*)NULL, (int*)NULL, &font );

                    if (width > max_width)
                    {
                        // exception if there was no previous space
                        if (last_space == 0)
                            last_space = pos;

                        int diff = pos-last_space;
                        int len = line.Len();
                        line.Remove( len-diff, diff );

                        wxStaticText *s = new wxStaticText( this, wxID_ANY, line );
                        box->Add( s );

                        pos = last_space;
                        last_space = 0;
                        line = wxEmptyString;
                    }
                }
        }
    }

    // remaining text behind last '\n'
    if (!line.empty())
    {
        wxStaticText *s2 = new wxStaticText( this, wxID_ANY, line );
        box->Add( s2 );
    }

    return box;
}

#endif // wxUSE_STATTEXT // && wxUSE_TEXTCTRL

#if wxUSE_BUTTON

wxSizer *wxDialogBase::CreateButtonSizer( long flags )
{
#ifdef __SMARTPHONE__
    wxDialog* dialog = (wxDialog*) this;
    if (flags & wxOK){
        dialog->SetLeftMenu(wxID_OK);
    }

    if (flags & wxCANCEL){
        dialog->SetRightMenu(wxID_CANCEL);
    }

    if (flags & wxYES){
        dialog->SetLeftMenu(wxID_YES);
    }

    if (flags & wxNO){
        dialog->SetLeftMenu(wxID_NO);
    }
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    return sizer;
#else
    return CreateStdDialogButtonSizer( flags );
#endif
}

wxStdDialogButtonSizer *wxDialogBase::CreateStdDialogButtonSizer( long flags )
{
    wxStdDialogButtonSizer *sizer = new wxStdDialogButtonSizer();
    wxButton *ok = NULL;
    wxButton *yes = NULL;
    wxButton *no = NULL;

    if (flags & wxOK){
        ok = new wxButton(this, wxID_OK);
        sizer->AddButton(ok);
    }

    if (flags & wxCANCEL){
        wxButton *cancel = new wxButton(this, wxID_CANCEL);
        sizer->AddButton(cancel);
    }

    if (flags & wxYES){
        yes = new wxButton(this, wxID_YES);
        sizer->AddButton(yes);
    }

    if (flags & wxNO){
        no = new wxButton(this, wxID_NO);
        sizer->AddButton(no);
    }

    if (flags & wxHELP){
        wxButton *help = new wxButton(this, wxID_HELP);
        sizer->AddButton(help);
    }

    sizer->Realize();

    if (flags & wxNO_DEFAULT)
    {
        if (no)
        {
            no->SetDefault();
            no->SetFocus();
        }
    }
    else
    {
        if (ok)
        {
            ok->SetDefault();
            ok->SetFocus();
        }
        else if (yes)
        {
            yes->SetDefault();
            yes->SetFocus();
        }
    }
    
    if (flags & wxOK)
        SetAffirmativeId(wxID_OK);
    else if (flags & wxYES)
        SetAffirmativeId(wxID_YES);

    return sizer;
}


#endif // wxUSE_BUTTON
