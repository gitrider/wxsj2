///////////////////////////////////////////////////////////////////////////////
// Name:        src/os2/iniconf.cpp
// Purpose:     implementation of wxIniConfig class
// Author:      David Webster
// Modified by:
// Created:     10/09/99
// Copyright:   David Webster
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifndef   WX_PRECOMP
  #include  <wx/string.h>
  #include  <wx/intl.h>
  #include  <wx/event.h>
  #include  <wx/app.h>
  #include  <wx/utils.h>
#endif  //WX_PRECOMP

#include  <wx/dynarray.h>
#include  <wx/log.h>

#include  <wx/config.h>
#if wxUSE_CONFIG

#include  <wx/os2/iniconf.h>

#define INCL_PM
#include  <os2.h>

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// we replace all path separators with this character
#define PATH_SEP_REPLACE  '_'

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// ctor & dtor
// ----------------------------------------------------------------------------

wxIniConfig::wxIniConfig(const wxString& strAppName,
                         const wxString& strVendor,
                         const wxString& localFilename,
                         const wxString& globalFilename,
                         long style)
           : wxConfigBase(!strAppName && wxTheApp ? wxTheApp->GetAppName()
                                               : strAppName,
                          !strVendor ? (wxTheApp ? wxTheApp->GetVendorName()
                                                  : strAppName)
                                      : strVendor,
                          localFilename, globalFilename, style)
{
    m_strLocalFilename = localFilename;
    if (m_strLocalFilename.IsEmpty())
    {
        m_strLocalFilename = GetAppName() + wxT(".ini");
    }

    // append the extension if none given and it's not an absolute file name
    // (otherwise we assume that they know what they're doing)
    if ( !wxIsPathSeparator(m_strLocalFilename[(size_t) 0]) &&
        m_strLocalFilename.Find('.') == wxNOT_FOUND )
    {
        m_strLocalFilename << wxT(".ini");
    }

    // set root path
    SetPath(wxT(""));
}

wxIniConfig::~wxIniConfig()
{
}

// ----------------------------------------------------------------------------
// path management
// ----------------------------------------------------------------------------

void wxIniConfig::SetPath(const wxString& strPath)
{
  wxArrayString aParts;

  if ( strPath.IsEmpty() ) {
    // nothing
  }
  else if ( strPath[(size_t) 0] == wxCONFIG_PATH_SEPARATOR ) {
    // absolute path
    wxSplitPath(aParts, strPath);
  }
  else {
    // relative path, combine with current one
    wxString strFullPath = GetPath();
    strFullPath << wxCONFIG_PATH_SEPARATOR << strPath;
    wxSplitPath(aParts, strFullPath);
  }

  size_t nPartsCount = aParts.Count();
  m_strPath.Empty();
  if ( nPartsCount == 0 ) {
    // go to the root
    m_strGroup = (wxChar*)PATH_SEP_REPLACE;
  }
  else {
    // translate
    m_strGroup = aParts[(size_t) 0];
    for ( size_t nPart = 1; nPart < nPartsCount; nPart++ ) {
      if ( nPart > 1 )
        m_strPath << PATH_SEP_REPLACE;
      m_strPath << aParts[nPart];
    }
  }

  // other functions assume that all this is true, i.e. there are no trailing
  // underscores at the end except if the group is the root one
  wxASSERT( (m_strPath.IsEmpty() || m_strPath.Last() != PATH_SEP_REPLACE) &&
            (m_strGroup == wxString((wxChar)PATH_SEP_REPLACE) ||
             m_strGroup.Last() != PATH_SEP_REPLACE) );
}

const wxString& wxIniConfig::GetPath() const
{
  static wxString s_str;

  // always return abs path
  s_str = wxCONFIG_PATH_SEPARATOR;

  if ( m_strGroup == wxString((wxChar)PATH_SEP_REPLACE) ) {
    // we're at the root level, nothing to do
  }
  else {
    s_str << m_strGroup;
    if ( !m_strPath.IsEmpty() )
      s_str << wxCONFIG_PATH_SEPARATOR;
    for ( const wxChar *p = m_strPath; *p != '\0'; p++ ) {
      s_str << (*p == PATH_SEP_REPLACE ? wxCONFIG_PATH_SEPARATOR : *p);
    }
  }

  return s_str;
}

wxString wxIniConfig::GetPrivateKeyName(const wxString& szKey) const
{
  wxString strKey;

  if ( !m_strPath.IsEmpty() )
    strKey << m_strPath << PATH_SEP_REPLACE;

  strKey << szKey;

  return strKey;
}

wxString wxIniConfig::GetKeyName(const wxString& szKey) const
{
  wxString strKey;

  if (m_strGroup != wxString((wxChar)PATH_SEP_REPLACE))
    strKey << m_strGroup << PATH_SEP_REPLACE;
  if ( !m_strPath.IsEmpty() )
    strKey << m_strPath << PATH_SEP_REPLACE;

  strKey << szKey;

  return strKey;
}

// ----------------------------------------------------------------------------
// enumeration
// ----------------------------------------------------------------------------

// not implemented
bool wxIniConfig::GetFirstGroup(wxString& str, long& lIndex) const
{
  wxFAIL_MSG(wxT("not implemeted"));

  return FALSE;
}

bool wxIniConfig::GetNextGroup (wxString& str, long& lIndex) const
{
  wxFAIL_MSG(wxT("not implemeted"));

  return FALSE;
}

bool wxIniConfig::GetFirstEntry(wxString& str, long& lIndex) const
{
  wxFAIL_MSG(wxT("not implemeted"));

  return FALSE;
}

bool wxIniConfig::GetNextEntry (wxString& str, long& lIndex) const
{
  wxFAIL_MSG(wxT("not implemeted"));

  return FALSE;
}

// ----------------------------------------------------------------------------
// misc info
// ----------------------------------------------------------------------------

// not implemented
size_t wxIniConfig::GetNumberOfEntries(bool bRecursive) const
{
  wxFAIL_MSG(wxT("not implemeted"));

  return (size_t)-1;
}

size_t wxIniConfig::GetNumberOfGroups(bool bRecursive) const
{
  wxFAIL_MSG(wxT("not implemeted"));

  return (size_t)-1;
}

bool wxIniConfig::HasGroup(const wxString& strName) const
{
  wxFAIL_MSG(wxT("not implemeted"));

  return FALSE;
}

bool wxIniConfig::HasEntry(const wxString& strName) const
{
  wxFAIL_MSG(wxT("not implemeted"));

  return FALSE;
}

// is current group empty?
bool wxIniConfig::IsEmpty() const
{
  char szBuf[1024];

//  GetPrivateProfileString(m_strGroup, NULL, "",
//                          szBuf, WXSIZEOF(szBuf), m_strLocalFilename);
  if ( !::IsEmpty(szBuf) )
    return FALSE;

//  GetProfileString(m_strGroup, NULL, "", szBuf, WXSIZEOF(szBuf));
//  if ( !::IsEmpty(szBuf) )
    return FALSE;

  return TRUE;
}

// ----------------------------------------------------------------------------
// read/write
// ----------------------------------------------------------------------------

bool wxIniConfig::Read(const wxString& szKey, wxString *pstr) const
{
  wxConfigPathChanger path(this, szKey);
  wxString strKey = GetPrivateKeyName(path.Name());

  wxChar szBuf[1024]; // @@ should dynamically allocate memory...

  // first look in the private INI file

  // NB: the lpDefault param to GetPrivateProfileString can't be NULL
//  GetPrivateProfileString(m_strGroup, strKey, "",
//                          szBuf, WXSIZEOF(szBuf), m_strLocalFilename);
  if ( ::IsEmpty((PSZ)szBuf) ) {
    // now look in win.ini
    wxString strKey = GetKeyName(path.Name());
//    GetProfileString(m_strGroup, strKey, "", szBuf, WXSIZEOF(szBuf));
  }

  if ( ::IsEmpty((PSZ)szBuf) ) {
    return FALSE;
  }
  else {
    *pstr = szBuf ;
    return TRUE;
  }
}

bool wxIniConfig::Read(const wxString& szKey, wxString *pstr,
                       const wxString& szDefault) const
{
  wxConfigPathChanger path(this, szKey);
  wxString strKey = GetPrivateKeyName(path.Name());

  wxChar szBuf[1024]; // @@ should dynamically allocate memory...

  // first look in the private INI file

  // NB: the lpDefault param to GetPrivateProfileString can't be NULL
//  GetPrivateProfileString(m_strGroup, strKey, "",
//                          szBuf, WXSIZEOF(szBuf), m_strLocalFilename);
  if ( ::IsEmpty((PSZ)szBuf) ) {
    // now look in win.ini
    wxString strKey = GetKeyName(path.Name());
//    GetProfileString(m_strGroup, strKey, "", szBuf, WXSIZEOF(szBuf));
  }

  if ( ::IsEmpty((PSZ)szBuf) ) {
    *pstr = szDefault;
    return FALSE;
  }
  else {
    *pstr = szBuf ;
    return TRUE;
  }
}

bool wxIniConfig::Read(const wxString& szKey, long *pl) const
{
  wxConfigPathChanger path(this, szKey);
  wxString strKey = GetPrivateKeyName(path.Name());

  // hack: we have no mean to know if it really found the default value or
  // didn't find anything, so we call it twice

  static const int nMagic  = 17; // 17 is some "rare" number
  static const int nMagic2 = 28; // arbitrary number != nMagic
  long lVal = 0; // = GetPrivateProfileInt(m_strGroup, strKey, nMagic, m_strLocalFilename);
  if ( lVal != nMagic ) {
    // the value was read from the file
    *pl = lVal;
    return TRUE;
  }

  // is it really nMagic?
//  lVal = GetPrivateProfileInt(m_strGroup, strKey, nMagic2, m_strLocalFilename);
  if ( lVal == nMagic2 ) {
    // the nMagic it returned was indeed read from the file
    *pl = lVal;
    return TRUE;
  }

  // no, it was just returning the default value, so now look in win.ini
//  *pl = GetProfileInt(GetVendorName(), GetKeyName(szKey), *pl);

  return TRUE;
}

bool wxIniConfig::Write(const wxString& szKey, const wxString& szValue)
{
  wxConfigPathChanger path(this, szKey);
  wxString strKey = GetPrivateKeyName(path.Name());

  bool bOk = FALSE; // = WritePrivateProfileString(m_strGroup, strKey,
//                                       szValue, m_strLocalFilename) != 0;

  if ( !bOk )
    wxLogLastError("WritePrivateProfileString");

  return bOk;
}

bool wxIniConfig::Write(const wxString& szKey, long lValue)
{
  // ltoa() is not ANSI :-(
  wxChar szBuf[40];   // should be good for sizeof(long) <= 16 (128 bits)
  wxSprintf(szBuf, wxT("%ld"), lValue);

  return Write(szKey, szBuf);
}

bool wxIniConfig::Flush(bool /* bCurrentOnly */)
{
  // this is just the way it works
//  return WritePrivateProfileString(NULL, NULL, NULL, m_strLocalFilename) != 0;
  return FALSE;
}

// ----------------------------------------------------------------------------
// delete
// ----------------------------------------------------------------------------

bool wxIniConfig::DeleteEntry(const wxString& szKey, bool bGroupIfEmptyAlso)
{
  // passing NULL as value to WritePrivateProfileString deletes the key
//  if ( !Write(szKey, (const char *)NULL) )
//    return FALSE;
  wxConfigPathChanger path(this, szKey);
  wxString strKey = GetPrivateKeyName(path.Name());

//  if (WritePrivateProfileString(m_strGroup, szKey,
//                                         (const char*) NULL, m_strLocalFilename) == 0)
//    return FALSE;

  if ( !bGroupIfEmptyAlso || !IsEmpty() )
    return TRUE;

  // delete the current group too
  bool bOk = FALSE; // = WritePrivateProfileString(m_strGroup, NULL,
//                                       NULL, m_strLocalFilename) != 0;

  if ( !bOk )
    wxLogLastError("WritePrivateProfileString");

  return bOk;
}

bool wxIniConfig::DeleteGroup(const wxString& szKey)
{
  wxConfigPathChanger path(this, szKey);

  // passing NULL as section name to WritePrivateProfileString deletes the
  // whole section according to the docs
  bool bOk = FALSE; // = WritePrivateProfileString(path.Name(), NULL,
  //                                     NULL, m_strLocalFilename) != 0;

  if ( !bOk )
    wxLogLastError("WritePrivateProfileString");

  return bOk;
}

#ifndef MAX_PATH
#define MAX_PATH 256
#endif

bool wxIniConfig::DeleteAll()
{
  // first delete our group in win.ini
//  WriteProfileString(GetVendorName(), NULL, NULL);

  // then delete our own ini file
  wxChar szBuf[MAX_PATH];
  size_t nRc = 0; // = GetWindowsDirectory(szBuf, WXSIZEOF(szBuf));
  if ( nRc == 0 )
  {
    wxLogLastError("GetWindowsDirectory");
  }
  else if ( nRc > WXSIZEOF(szBuf) )
  {
    wxFAIL_MSG(_("buffer is too small for Windows directory."));
  }

  wxString strFile = szBuf;
  strFile << wxT('\\') << m_strLocalFilename;

  if ( !wxRemoveFile(strFile) ) {
    wxLogSysError(_("Can't delete the INI file '%s'"), strFile.c_str());
    return FALSE;
  }

  return TRUE;
}

bool wxIniConfig::RenameEntry(const wxString& oldName, const wxString& newName)
{
    // Not implemented
    return FALSE;
}

bool wxIniConfig::RenameGroup(const wxString& oldName, const wxString& newName)
{
    // Not implemented
    return FALSE;
}

#endif //wxUSE_CONFIG
