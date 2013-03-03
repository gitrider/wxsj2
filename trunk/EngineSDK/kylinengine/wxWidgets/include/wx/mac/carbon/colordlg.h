/////////////////////////////////////////////////////////////////////////////
// Name:        colordlg.h
// Purpose:     wxColourDialog class. Use generic version if no
//              platform-specific implementation.
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// RCS-ID:      $Id: colordlg.h,v 1.4 2004/05/23 20:51:02 JS Exp $
// Copyright:   (c) Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_COLORDLG_H_
#define _WX_COLORDLG_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "colordlg.h"
#endif

#include "wx/setup.h"
#include "wx/dialog.h"
#include "wx/cmndata.h"

/*
 * Platform-specific colour dialog implementation
 */

class WXDLLEXPORT wxColourDialog: public wxDialog
{
DECLARE_DYNAMIC_CLASS(wxColourDialog)
public:
    wxColourDialog();
    wxColourDialog(wxWindow *parent, wxColourData *data = NULL);

    bool Create(wxWindow *parent, wxColourData *data = NULL);

    int ShowModal();
    wxColourData& GetColourData() { return m_colourData; }

protected:
    wxColourData        m_colourData;
    wxWindow*           m_dialogParent;
};

#endif
    // _WX_COLORDLG_H_
