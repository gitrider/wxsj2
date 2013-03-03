/////////////////////////////////////////////////////////////////////////////
// Name:        wx/msw/msgdlg.h
// Purpose:     wxMessageDialog class
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id: msgdlg.h,v 1.12 2005/03/11 15:33:45 ABX Exp $
// Copyright:   (c) Julian Smart
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

extern WXDLLEXPORT_DATA(const wxChar*) wxMessageBoxCaptionStr;

class WXDLLEXPORT wxMessageDialog: public wxDialog, public wxMessageDialogBase
{
DECLARE_DYNAMIC_CLASS(wxMessageDialog)
protected:
    wxString    m_caption;
    wxString    m_message;
    wxWindow *  m_parent;
public:
    wxMessageDialog(wxWindow *parent, const wxString& message, const wxString& caption = wxMessageBoxCaptionStr,
        long style = wxOK|wxCENTRE, const wxPoint& pos = wxDefaultPosition);

    int ShowModal(void);

    DECLARE_NO_COPY_CLASS(wxMessageDialog)
};


#endif
    // _WX_MSGBOXDLG_H_
