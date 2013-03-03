/////////////////////////////////////////////////////////////////////////////
// Name:        wx/mac/classic/msgdlg.h
// Purpose:     wxMessageDialog class. Use generic version if no
//              platform-specific implementation.
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// RCS-ID:      $Id: msgdlg.h,v 1.4 2005/03/11 15:33:34 ABX Exp $
// Copyright:   (c) Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_MSGBOXDLG_H_
#define _WX_MSGBOXDLG_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "msgdlg.h"
#endif

#include "wx/setup.h"
#include "wx/dialog.h"

/*
 * Message box dialog
 */

WXDLLEXPORT_DATA(extern const wxChar*) wxMessageBoxCaptionStr;

class WXDLLEXPORT wxMessageDialog: public wxDialog, public wxMessageDialogBase
{
    DECLARE_DYNAMIC_CLASS(wxMessageDialog)

protected:
    wxString    m_caption;
    wxString    m_message;
    wxWindow *  m_parent;
public:
    wxMessageDialog(wxWindow *parent,
                    const wxString& message,
                    const wxString& caption = wxMessageBoxCaptionStr,
                    long style = wxOK|wxCENTRE,
                    const wxPoint& pos = wxDefaultPosition);

    int ShowModal();

    // not supported for message dialog, RR
    virtual void DoSetSize(int WXUNUSED(x), int WXUNUSED(y),
                           int WXUNUSED(width), int WXUNUSED(height),
                           int WXUNUSED(sizeFlags) = wxSIZE_AUTO) {}

};

#endif
    // _WX_MSGBOXDLG_H_
