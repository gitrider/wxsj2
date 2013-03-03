///////////////////////////////////////////////////////////////////////////////
// Name:        wx/unix/stdpaths.h
// Purpose:     wxStandardPaths for Unix systems
// Author:      Vadim Zeitlin
// Modified by:
// Created:     2004-10-19
// RCS-ID:      $Id: stdpaths.h,v 1.3 2005/05/31 09:18:55 JS Exp $
// Copyright:   (c) 2004 Vadim Zeitlin <vadim@wxwindows.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_UNIX_STDPATHS_H_
#define _WX_UNIX_STDPATHS_H_

// ----------------------------------------------------------------------------
// wxStandardPaths
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_BASE wxStandardPaths : public wxStandardPathsBase
{
public:
    // set the program installation directory which is /usr/local by default
    //
    // under some systems (currently only Linux) the program directory can be
    // determined automatically but for portable programs you should always set
    // it explicitly
    void SetInstallPrefix(const wxString& prefix);

    // get the program installation prefix
    //
    // if the prefix had been previously by SetInstallPrefix, returns that
    // value, otherwise tries to determine it automatically (Linux only right
    // now) and returns /usr/local if it failed
    wxString GetInstallPrefix() const;


    // implement base class pure virtuals
    virtual wxString GetConfigDir() const;
    virtual wxString GetUserConfigDir() const;
    virtual wxString GetDataDir() const;
    virtual wxString GetLocalDataDir() const;
    virtual wxString GetUserDataDir() const;
    virtual wxString GetPluginsDir() const;

private:
    wxString m_prefix;
};

#endif // _WX_UNIX_STDPATHS_H_

