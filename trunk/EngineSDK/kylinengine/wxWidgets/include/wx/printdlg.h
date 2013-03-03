/////////////////////////////////////////////////////////////////////////////
// Name:        wx/printdlg.h
// Purpose:     Base header and class for print dialogs
// Author:      Julian Smart
// Modified by:
// Created:
// RCS-ID:      $Id: printdlg.h,v 1.20 2005/05/04 18:52:01 JS Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows Licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_PRINTDLG_H_BASE_
#define _WX_PRINTDLG_H_BASE_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "printdlg.h"
#endif

#include "wx/defs.h"

#if wxUSE_PRINTING_ARCHITECTURE

#include "wx/event.h"
#include "wx/dialog.h"
#include "wx/intl.h"
#include "wx/cmndata.h"


// ---------------------------------------------------------------------------
// wxPrintDialogBase: interface for the dialog for printing
// ---------------------------------------------------------------------------

class WXDLLEXPORT wxPrintDialogBase : public wxDialog
{
public:
    wxPrintDialogBase() { }
    wxPrintDialogBase(wxWindow *parent,
                      wxWindowID id = wxID_ANY,
                      const wxString &title = wxEmptyString,
                      const wxPoint &pos = wxDefaultPosition,
                      const wxSize &size = wxDefaultSize,
                      long style = wxDEFAULT_DIALOG_STYLE);
            
    virtual wxPrintDialogData& GetPrintDialogData() = 0;
    virtual wxPrintData& GetPrintData() = 0;
    virtual wxDC *GetPrintDC() = 0;
    
private:
    DECLARE_ABSTRACT_CLASS(wxPrintDialogBase)
    DECLARE_NO_COPY_CLASS(wxPrintDialogBase)
};

// ---------------------------------------------------------------------------
// wxPrintDialog: the dialog for printing.
// ---------------------------------------------------------------------------

class WXDLLEXPORT wxPrintDialog : public wxObject
{
public:
    wxPrintDialog(wxWindow *parent, wxPrintDialogData* data = NULL);
    wxPrintDialog(wxWindow *parent, wxPrintData* data);
    ~wxPrintDialog();

    virtual int ShowModal();

    virtual wxPrintDialogData& GetPrintDialogData();
    virtual wxPrintData& GetPrintData();
    virtual wxDC *GetPrintDC();
    
private:
    wxPrintDialogBase  *m_pimpl;
    
private:
    DECLARE_DYNAMIC_CLASS(wxPrintDialog)
    DECLARE_NO_COPY_CLASS(wxPrintDialog)
};

// ---------------------------------------------------------------------------
// wxPageSetupDialogBase: interface for the page setup dialog
// ---------------------------------------------------------------------------

class WXDLLEXPORT wxPageSetupDialogBase: public wxDialog
{
public:
    wxPageSetupDialogBase() { }
    wxPageSetupDialogBase(wxWindow *parent,
                      wxWindowID id = wxID_ANY,
                      const wxString &title = wxEmptyString,
                      const wxPoint &pos = wxDefaultPosition,
                      const wxSize &size = wxDefaultSize,
                      long style = wxDEFAULT_DIALOG_STYLE);

    virtual wxPageSetupDialogData& GetPageSetupDialogData() = 0;

private:
    DECLARE_ABSTRACT_CLASS(wxPageSetupDialogBase)
    DECLARE_NO_COPY_CLASS(wxPageSetupDialogBase)
};

// ---------------------------------------------------------------------------
// wxPageSetupDialog: the page setup dialog
// ---------------------------------------------------------------------------

class WXDLLEXPORT wxPageSetupDialog: public wxObject
{
public:
    wxPageSetupDialog(wxWindow *parent, wxPageSetupDialogData *data = NULL);
    ~wxPageSetupDialog();

    int ShowModal();
    wxPageSetupDialogData& GetPageSetupDialogData();
    // old name
    wxPageSetupDialogData& GetPageSetupData();

private:
    wxPageSetupDialogBase  *m_pimpl;
    
private:
    DECLARE_DYNAMIC_CLASS(wxPageSetupDialog)
    DECLARE_NO_COPY_CLASS(wxPageSetupDialog)
};

#endif

#endif
    // _WX_PRINTDLG_H_BASE_
