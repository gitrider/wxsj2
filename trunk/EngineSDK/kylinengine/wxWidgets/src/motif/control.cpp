/////////////////////////////////////////////////////////////////////////////
// Name:        control.cpp
// Purpose:     wxControl class
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id: control.cpp,v 1.30 2004/05/23 20:52:48 JS Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "control.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/defs.h"

#include "wx/control.h"
#include "wx/panel.h"
#include "wx/utils.h"

#ifdef __VMS__
#pragma message disable nosimpint
#endif
#include <Xm/Xm.h>
#ifdef __VMS__
#pragma message enable nosimpint
#endif

#include "wx/motif/private.h"

IMPLEMENT_ABSTRACT_CLASS(wxControl, wxWindow)

BEGIN_EVENT_TABLE(wxControl, wxWindow)
END_EVENT_TABLE()

// Item members
wxControl::wxControl()
{
    m_backgroundColour = *wxWHITE;
    m_foregroundColour = *wxBLACK;

    m_inSetValue = false;
}

bool wxControl::Create( wxWindow *parent,
                      wxWindowID id,
                      const wxPoint &pos,
                      const wxSize &size,
                      long style,
                      const wxValidator& validator,
                      const wxString &name)
{
    bool ret = wxWindow::Create(parent, id, pos, size, style, name);

#if wxUSE_VALIDATORS
    SetValidator(validator);
#endif

    return ret;
}

bool wxControl::CreateControl(wxWindow *parent,
                              wxWindowID id,
                              const wxPoint& pos,
                              const wxSize& size,
                              long style,
                              const wxValidator& validator,
                              const wxString& name)
{
    if( !wxControlBase::CreateControl( parent, id, pos, size, style,
                                       validator, name ) )
        return false;

    m_backgroundColour = parent->GetBackgroundColour();
    m_foregroundColour = parent->GetForegroundColour();
    m_font = parent->GetFont();

    return true;
}

void wxControl::SetLabel(const wxString& label)
{
    Widget widget = (Widget) GetLabelWidget() ;
    if (!widget)
        return;

    wxXmString label_str(wxStripMenuCodes(label));

    XtVaSetValues (widget,
        XmNlabelString, label_str(),
        XmNlabelType, XmSTRING,
        NULL);
}

wxString wxControl::GetLabel() const
{
    Widget widget = (Widget) GetLabelWidget() ;
    if (!widget)
        return wxEmptyString;

    XmString text = NULL;
    XtVaGetValues (widget,
        XmNlabelString, &text,
        NULL);

    return wxXmStringToString( text );
}

bool wxControl::ProcessCommand(wxCommandEvent & event)
{
    return GetEventHandler()->ProcessEvent(event);
}

wxSize wxControl::DoGetBestSize() const
{
    Widget w = (Widget)GetTopWidget();

    // Do not return any arbitrary default value...
    wxASSERT_MSG (w, wxT("DoGetBestSize called before creation"));

    XtWidgetGeometry preferred;
    XtQueryGeometry (w, NULL, &preferred);

    return wxSize(preferred.width, preferred.height);
}
