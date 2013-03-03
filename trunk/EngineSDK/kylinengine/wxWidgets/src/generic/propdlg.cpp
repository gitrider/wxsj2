/////////////////////////////////////////////////////////////////////////////
// Name:        propdlg.cpp
// Purpose:     wxPropertySheetDialog
// Author:      Julian Smart
// Modified by:
// Created:     2005-03-12
// RCS-ID:      $Id: propdlg.cpp,v 1.8 2005/05/23 11:08:41 JS Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "propdlg.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "wx/defs.h"

#ifndef WX_PRECOMP
    #include "wx/button.h"
    #include "wx/sizer.h"
    #include "wx/intl.h"
    #include "wx/log.h"
    #include "wx/msgdlg.h"
#endif

#include "wx/bookctrl.h"
#include "wx/generic/propdlg.h"

//-----------------------------------------------------------------------------
// wxPropertySheetDialog
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxPropertySheetDialog, wxDialog)

BEGIN_EVENT_TABLE(wxPropertySheetDialog, wxDialog)
    EVT_ACTIVATE(wxPropertySheetDialog::OnActivate)
END_EVENT_TABLE()

bool wxPropertySheetDialog::Create(wxWindow* parent, wxWindowID id, const wxString& title, 
                                       const wxPoint& pos, const wxSize& sz, long style,
                                       const wxString& name)
{
    if (!wxDialog::Create(parent, id, title, pos, sz, style|wxCLIP_CHILDREN, name))
        return false;
    
    wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
    SetSizer(topSizer);

    // This gives more space around the edges
    m_innerSizer = new wxBoxSizer( wxVERTICAL );

    int extraSpace = 2;
#if defined(__SMARTPHONE__) || defined(__POCKETPC__)
    extraSpace=0;
#endif
    topSizer->Add(m_innerSizer, 1, wxGROW|wxALL, extraSpace);

    m_bookCtrl = CreateBookCtrl();
    AddBookCtrl(m_innerSizer);

    return true;
}

void wxPropertySheetDialog::Init()
{
    m_innerSizer = NULL;
    m_bookCtrl = NULL;
}

// Layout the dialog, to be called after pages have been created
void wxPropertySheetDialog::LayoutDialog()
{
#if !defined(__SMARTPHONE__) && !defined(__POCKETPC__)
    GetSizer()->Fit(this);
    GetSizer()->SetSizeHints(this);
    Centre(wxBOTH);
#endif
#if defined(__SMARTPHONE__)
    if (m_bookCtrl)
        m_bookCtrl->SetFocus();
#endif
}

// Creates the buttons, if any
void wxPropertySheetDialog::CreateButtons(int flags)
{
#if defined(__SMARTPHONE__)
    // TODO: create a right-click menu with all the other IDs available.
    // Perhaps that could be embedded in CreateButtonSizer() directly.
    SetRightMenu(wxID_CANCEL);
    SetLeftMenu(wxID_OK);
    wxUnusedVar(flags);
#elif defined(__POCKETPC__)
    // Do nothing
    wxUnusedVar(flags);
#else
    wxSizer* sizer = CreateButtonSizer(flags);
    m_innerSizer->Add( sizer, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxLEFT|wxRIGHT, 2);
    m_innerSizer->AddSpacer(2);
#endif
}

// Creates the book control
wxBookCtrlBase* wxPropertySheetDialog::CreateBookCtrl()
{
    int style = wxCLIP_CHILDREN;
#if defined(__POCKETPC__) && wxUSE_NOTEBOOK
    style |= wxNB_BOTTOM|wxNB_FLAT;
#else
    style |= wxBC_DEFAULT;
#endif
    return new wxBookCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, style );
}

// Adds the book control to the inner sizer.
void wxPropertySheetDialog::AddBookCtrl(wxSizer* sizer)
{
#if defined(__POCKETPC__) && wxUSE_NOTEBOOK
    // The book control has to be sized larger than the dialog because of a border bug
    // in WinCE
    int borderSize = -2;
    sizer->Add( m_bookCtrl, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxRIGHT, borderSize );
#else
    sizer->Add( m_bookCtrl, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
#endif
}

void wxPropertySheetDialog::OnActivate(wxActivateEvent& event)
{
#if defined(__SMARTPHONE__)
    // Attempt to focus the choice control: not yet working, but might
    // be a step in the right direction. OnActivate overrides the default
    // handler in toplevel.cpp that sets the focus for the first child of
    // of the dialog (the choicebook).
    if (event.GetActive())
    {
        wxChoicebook* choiceBook = wxDynamicCast(GetBookCtrl(), wxChoicebook);     
        if (choiceBook)
            choiceBook->SetFocus();
    }
    else
#endif
        event.Skip();
}

