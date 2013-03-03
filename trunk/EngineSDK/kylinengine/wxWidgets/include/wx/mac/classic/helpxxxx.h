/////////////////////////////////////////////////////////////////////////////
// Name:        helpxxxx.h
// Purpose:     Help system: native implementation for your system. Replace
//              XXXX with suitable name.
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// RCS-ID:      $Id: helpxxxx.h,v 1.3 2004/05/23 20:51:10 JS Exp $
// Copyright:   (c) Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_HELPXXXX_H_
#define _WX_HELPXXXX_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "helpxxxx.h"
#endif

#include "wx/wx.h"

#include "wx/helpbase.h"

class WXDLLEXPORT wxXXXXHelpController: public wxHelpControllerBase
{
  DECLARE_CLASS(wxXXXXHelpController)

 public:
  wxXXXXHelpController();
  ~wxXXXXHelpController();

  // Must call this to set the filename and server name
  virtual bool Initialize(const wxString& file);

  // If file is "", reloads file given  in Initialize
  virtual bool LoadFile(const wxString& file = "");
  virtual bool DisplayContents();
  virtual bool DisplaySection(int sectionNo);
  virtual bool DisplayBlock(long blockNo);
  virtual bool KeywordSearch(const wxString& k,
                             wxHelpSearchMode mode = wxHELP_SEARCH_ALL);

  virtual bool Quit();
  virtual void OnQuit();

  inline wxString GetHelpFile() const { return m_helpFile; }

protected:
  wxString m_helpFile;
};

#endif
    // _WX_HELPXXXX_H_
