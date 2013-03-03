/////////////////////////////////////////////////////////////////////////////
// Name:        wx/dirdlg.h
// Purpose:     wxDirDialog base class
// Author:      Robert Roebling
// Modified by:
// Created:
// Copyright:   (c) Robert Roebling
// RCS-ID:      $Id: dirdlg.h,v 1.39 2005/05/04 18:51:57 JS Exp $
// Licence:     wxWindows Licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_DIRDLG_H_BASE_
#define _WX_DIRDLG_H_BASE_

#if wxUSE_DIRDLG

#include "wx/dialog.h"

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

extern WXDLLEXPORT_DATA(const wxChar*) wxDirDialogNameStr;
extern WXDLLEXPORT_DATA(const wxChar*) wxDirDialogDefaultFolderStr;
extern WXDLLEXPORT_DATA(const wxChar*) wxDirSelectorPromptStr;

#ifdef __WXWINCE__
    #define wxDD_DEFAULT_STYLE \
        (wxDEFAULT_DIALOG_STYLE | wxDD_NEW_DIR_BUTTON)
#else
    #define wxDD_DEFAULT_STYLE \
        (wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxDD_NEW_DIR_BUTTON)
#endif

/*
    The interface (TODO: make the other classes really derive from it!) is
    something like this:

class WXDLLEXPORT wxDirDialogBase : public wxDialog
{
public:
    wxDirDialogBase(wxWindow *parent,
                    const wxString& title = wxFileSelectorPromptStr,
                    const wxString& defaultPath = wxEmptyString,
                    long style = wxDD_DEFAULT_STYLE,
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& sz = wxDefaultSize,
                    const wxString& name = _T("dirdialog"));

    void SetMessage(const wxString& message);
    void SetPath(const wxString& path);
    void SetStyle(long style);

    wxString GetMessage() const;
    wxString GetPath() const;
    long GetStyle() const;
};

*/

// Universal and non-port related switches with need for generic implementation
#if defined(__WXMSW__) && (defined(__WXUNIVERSAL__) || \
                           defined(__SALFORDC__)    || \
                           !wxUSE_OLE               || \
                           (defined (__GNUWIN32__) && !wxUSE_NORLANDER_HEADERS))

    #include "wx/generic/dirdlgg.h"
    #define wxDirDialog wxGenericDirDialog

// MS PocketPC or MS Smartphone
#elif defined(__WXMSW__) && defined(__WXWINCE__) && !defined(__HANDHELDPC__)

    #include "wx/generic/dirdlgg.h"
    #define wxDirDialog wxGenericDirDialog

// Native MSW
#elif defined(__WXMSW__)

    #include "wx/msw/dirdlg.h"

// Native Mac
#elif defined(__WXMAC__)

    #include "wx/mac/dirdlg.h"

// Other ports use generic implementation
#elif defined(__WXMOTIF__) || \
      defined(__WXGTK__)   || \
      defined(__WXX11__)   || \
      defined(__WXMGL__)   || \
      defined(__WXCOCOA__) || \
      defined(__WXPM__)

    #include "wx/generic/dirdlgg.h"
    #define wxDirDialog wxGenericDirDialog

#endif

// ----------------------------------------------------------------------------
// common ::wxDirSelector() function
// ----------------------------------------------------------------------------

WXDLLEXPORT wxString
wxDirSelector(const wxString& message = wxDirSelectorPromptStr,
              const wxString& defaultPath = wxEmptyString,
              long style = wxDD_DEFAULT_STYLE,
              const wxPoint& pos = wxDefaultPosition,
              wxWindow *parent = NULL);

#endif // wxUSE_DIRDLG

#endif
    // _WX_DIRDLG_H_BASE_
