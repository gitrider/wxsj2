/////////////////////////////////////////////////////////////////////////////
// Name:        mac/mimetype.cpp
// Purpose:     classes and functions to manage MIME types
// Author:      Vadim Zeitlin
// Modified by:
// Created:     23.09.98
// RCS-ID:      $Id: mimetype.cpp,v 1.1 2005/04/25 09:54:06 MW Exp $
// Copyright:   (c) 1998 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows licence (part of wxExtra library)
/////////////////////////////////////////////////////////////////////////////

#ifdef    __GNUG__
#pragma implementation "mimetype.h"
#endif

// for compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
  #pragma hdrstop
#endif

#ifndef WX_PRECOMP
  #include "wx/defs.h"
#endif

#ifndef WX_PRECOMP
  #include "wx/string.h"
  #if wxUSE_GUI
    #include "wx/icon.h"
  #endif
#endif //WX_PRECOMP

#if wxUSE_MIMETYPE

#include "wx/log.h"
#include "wx/file.h"
#include "wx/intl.h"
#include "wx/dynarray.h"
#include "wx/confbase.h"

#include "wx/msdos/mimetype.h"

// other standard headers
#include <ctype.h>

// in case we're compiling in non-GUI mode
class WXDLLEXPORT wxIcon;

bool wxFileTypeImpl::SetCommand(const wxString& cmd, const wxString& verb, bool overwriteprompt)
{
    return FALSE;
}

bool wxFileTypeImpl::SetDefaultIcon(const wxString& strIcon, int index)
{
    return FALSE;
}

bool wxFileTypeImpl::GetCommand(wxString *command, const char *verb) const
{
    return FALSE;
}

// @@ this function is half implemented
bool wxFileTypeImpl::GetExtensions(wxArrayString& extensions)
{
    return FALSE;
}

bool wxFileTypeImpl::GetMimeType(wxString *mimeType) const
{
    if ( m_strFileType.Length() > 0 )
    {
        *mimeType = m_strFileType ;
        return TRUE ;
    }
    else
    return FALSE;
}

bool wxFileTypeImpl::GetMimeTypes(wxArrayString& mimeTypes) const
{
    wxString s;
    
    if (GetMimeType(&s))
    {
        mimeTypes.Clear();
        mimeTypes.Add(s);
        return TRUE;
    }
    else 
        return FALSE;
}

bool wxFileTypeImpl::GetIcon(wxIconLocation *WXUNUSED(icon)) const
{
    // no such file type or no value or incorrect icon entry
    return FALSE;
}

bool wxFileTypeImpl::GetDescription(wxString *desc) const
{
    return FALSE;
}

size_t
wxFileTypeImpl::GetAllCommands(wxArrayString * verbs, wxArrayString * commands,
                   const wxFileType::MessageParameters& params) const
{
    wxFAIL_MSG( _T("wxFileTypeImpl::GetAllCommands() not yet implemented") );
    return 0;
}

void
wxMimeTypesManagerImpl::Initialize(int mailcapStyles, const wxString& extraDir)
{
    wxFAIL_MSG( _T("wxMimeTypesManagerImpl::Initialize() not yet implemented") );
}

void
wxMimeTypesManagerImpl::ClearData()
{
    wxFAIL_MSG( _T("wxMimeTypesManagerImpl::ClearData() not yet implemented") );
}

// extension -> file type
wxFileType *
wxMimeTypesManagerImpl::GetFileTypeFromExtension(const wxString& e)
{
    wxString ext = e ;
    ext = ext.Lower() ;
    if ( ext == wxT("txt") )
    {
        wxFileType *fileType = new wxFileType;
        fileType->m_impl->SetFileType(wxT("text/text"));
        fileType->m_impl->SetExt(ext);
        return fileType;
    }
    else if ( ext == wxT("htm") || ext == wxT("html") )
    {
        wxFileType *fileType = new wxFileType;
        fileType->m_impl->SetFileType(wxT("text/html"));
        fileType->m_impl->SetExt(ext);
        return fileType;
    }
    else if ( ext == wxT("gif") )
    {
        wxFileType *fileType = new wxFileType;
        fileType->m_impl->SetFileType(wxT("image/gif"));
        fileType->m_impl->SetExt(ext);
        return fileType;
    }
    else if ( ext == wxT("png" ))
    {
        wxFileType *fileType = new wxFileType;
        fileType->m_impl->SetFileType(wxT("image/png"));
        fileType->m_impl->SetExt(ext);
        return fileType;
    }
    else if ( ext == wxT("jpg" )|| ext == wxT("jpeg") )
    {
        wxFileType *fileType = new wxFileType;
        fileType->m_impl->SetFileType(wxT("image/jpeg"));
        fileType->m_impl->SetExt(ext);
        return fileType;
    }
    else if ( ext == wxT("bmp") )
    {
        wxFileType *fileType = new wxFileType;
        fileType->m_impl->SetFileType(wxT("image/bmp"));
        fileType->m_impl->SetExt(ext);
        return fileType;
    }
    else if ( ext == wxT("tif") || ext == wxT("tiff") )
    {
        wxFileType *fileType = new wxFileType;
        fileType->m_impl->SetFileType(wxT("image/tiff"));
        fileType->m_impl->SetExt(ext);
        return fileType;
    }
    else if ( ext == wxT("xpm") )
    {
        wxFileType *fileType = new wxFileType;
        fileType->m_impl->SetFileType(wxT("image/xpm"));
        fileType->m_impl->SetExt(ext);
        return fileType;
    }
    else if ( ext == wxT("xbm") )
    {
        wxFileType *fileType = new wxFileType;
        fileType->m_impl->SetFileType(wxT("image/xbm"));
        fileType->m_impl->SetExt(ext);
        return fileType;
    }

    // unknown extension
    return NULL;
}

// MIME type -> extension -> file type
wxFileType *
wxMimeTypesManagerImpl::GetFileTypeFromMimeType(const wxString& mimeType)
{
    return NULL;
}

size_t wxMimeTypesManagerImpl::EnumAllFileTypes(wxArrayString& mimetypes)
{
    // VZ: don't know anything about this for Mac
    wxFAIL_MSG( _T("wxMimeTypesManagerImpl::EnumAllFileTypes() not yet implemented") );

    return 0;
}

wxFileType *
wxMimeTypesManagerImpl::Associate(const wxFileTypeInfo& ftInfo)
{
    wxFAIL_MSG( _T("wxMimeTypesManagerImpl::Associate() not yet implemented") );

    return NULL;
}

bool
wxMimeTypesManagerImpl::Unassociate(wxFileType *ft)
{
    return FALSE;
}

#endif // wxUSE_MIMETYPE
