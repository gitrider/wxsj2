/////////////////////////////////////////////////////////////////////////////
// Name:        filedlg.h
// Purpose:     wxFileDialog class
// Author:      David Webster
// Modified by:
// Created:     10/05/99
// RCS-ID:      $Id: filedlg.h,v 1.11 2004/05/23 20:51:38 JS Exp $
// Copyright:   (c) David Webster
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_FILEDLG_H_
#define _WX_FILEDLG_H_

//-------------------------------------------------------------------------
// wxFileDialog
//-------------------------------------------------------------------------

class WXDLLEXPORT wxFileDialog: public wxFileDialogBase
{
DECLARE_DYNAMIC_CLASS(wxFileDialog)
public:
    wxFileDialog( wxWindow*       pParent
                 ,const wxString& rsMessage = wxFileSelectorPromptStr
                 ,const wxString& rsDefaultDir = wxEmptyString
                 ,const wxString& rsDefaultFile = wxEmptyString
                 ,const wxString& rsWildCard = wxFileSelectorDefaultWildcardStr
                 ,long            lStyle = 0
                 ,const wxPoint&  rPos = wxDefaultPosition
                );

    virtual void GetPaths(wxArrayString& rasPath) const;

    int ShowModal();

protected:
    wxArrayString m_fileNames;
}; // end of CLASS wxFileDialog

#endif // _WX_FILEDLG_H_
