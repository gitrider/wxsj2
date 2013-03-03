/////////////////////////////////////////////////////////////////////////////
// Name:        wx/palmos/colordlg.h
// Purpose:     wxColourDialog class
// Author:      William Osborne - minimal working wxPalmOS port
// Modified by: Wlodzimierz ABX Skiba - real native implementation
// Created:     10/13/04
// RCS-ID:      $Id: colordlg.h,v 1.3 2005/01/18 21:14:23 ABX Exp $
// Copyright:   (c) William Osborne, Wlodzimierz Skiba
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

// ----------------------------------------------------------------------------
// wxColourDialog: dialog for choosing a colours
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxColourDialog : public wxDialog
{
public:
    wxColourDialog();
    wxColourDialog(wxWindow *parent, wxColourData *data = NULL);

    bool Create(wxWindow *parent, wxColourData *data = NULL);

    wxColourData& GetColourData() { return m_colourData; }

    virtual int ShowModal();

protected:
    wxColourData        m_colourData;

    DECLARE_DYNAMIC_CLASS_NO_COPY(wxColourDialog)
};

#endif
    // _WX_COLORDLG_H_
