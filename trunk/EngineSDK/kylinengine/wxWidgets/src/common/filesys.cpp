/////////////////////////////////////////////////////////////////////////////
// Name:        filesys.cpp
// Purpose:     wxFileSystem class - interface for opening files
// Author:      Vaclav Slavik
// Copyright:   (c) 1999 Vaclav Slavik
// CVS-ID:      $Id: filesys.cpp,v 1.60 2005/02/01 19:37:35 ABX Exp $
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "filesys.h"
#endif

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif


#if wxUSE_FILESYSTEM

#include "wx/wfstream.h"
#include "wx/module.h"
#include "wx/filesys.h"
#include "wx/mimetype.h"
#include "wx/filename.h"
#include "wx/log.h"


//--------------------------------------------------------------------------------
// wxFileSystemHandler
//--------------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_CLASS(wxFileSystemHandler, wxObject)


wxString wxFileSystemHandler::GetMimeTypeFromExt(const wxString& location)
{
    wxString ext, mime;
    wxString loc = GetRightLocation(location);
    wxChar c;
    int l = loc.Length(), l2;

    l2 = l;
    for (int i = l-1; i >= 0; i--)
    {
        c = loc[(unsigned int) i];
        if ( c == wxT('#') )
            l2 = i + 1;
        if ( c == wxT('.') )
        {
            ext = loc.Right(l2-i-1);
            break;
        }
        if ( (c == wxT('/')) || (c == wxT('\\')) || (c == wxT(':')) )
            return wxEmptyString;
    }

#if wxUSE_MIMETYPE
    static bool s_MinimalMimeEnsured = false;
    if (!s_MinimalMimeEnsured)
    {
        static const wxFileTypeInfo fallbacks[] =
        {
            wxFileTypeInfo(_T("image/jpeg"),
                           wxEmptyString,
                           wxEmptyString,
                           _T("JPEG image (from fallback)"),
                           _T("jpg"), _T("jpeg"), _T("JPG"), _T("JPEG"), NULL),
            wxFileTypeInfo(_T("image/gif"),
                           wxEmptyString,
                           wxEmptyString,
                           _T("GIF image (from fallback)"),
                           _T("gif"), _T("GIF"), NULL),
            wxFileTypeInfo(_T("image/png"),
                           wxEmptyString,
                           wxEmptyString,
                           _T("PNG image (from fallback)"),
                           _T("png"), _T("PNG"), NULL),
            wxFileTypeInfo(_T("image/bmp"),
                           wxEmptyString,
                           wxEmptyString,
                           _T("windows bitmap image (from fallback)"),
                           _T("bmp"), _T("BMP"), NULL),
            wxFileTypeInfo(_T("text/html"),
                           wxEmptyString,
                           wxEmptyString,
                           _T("HTML document (from fallback)"),
                           _T("htm"), _T("html"), _T("HTM"), _T("HTML"), NULL),
            // must terminate the table with this!
            wxFileTypeInfo()
        };
        wxTheMimeTypesManager->AddFallbacks(fallbacks);
        s_MinimalMimeEnsured = true;
    }

    wxFileType *ft = wxTheMimeTypesManager->GetFileTypeFromExtension(ext);
    if ( !ft || !ft -> GetMimeType(&mime) )
    {
        mime = wxEmptyString;
    }

    delete ft;

    return mime;
#else
    if ( ext.IsSameAs(wxT("htm"), false) || ext.IsSameAs(_T("html"), false) )
        return wxT("text/html");
    if ( ext.IsSameAs(wxT("jpg"), false) || ext.IsSameAs(_T("jpeg"), false) )
        return wxT("image/jpeg");
    if ( ext.IsSameAs(wxT("gif"), false) )
        return wxT("image/gif");
    if ( ext.IsSameAs(wxT("png"), false) )
        return wxT("image/png");
    if ( ext.IsSameAs(wxT("bmp"), false) )
        return wxT("image/bmp");
    return wxEmptyString;
#endif
}



wxString wxFileSystemHandler::GetProtocol(const wxString& location) const
{
    wxString s = wxEmptyString;
    int i, l = location.Length();
    bool fnd = false;

    for (i = l-1; (i >= 0) && ((location[i] != wxT('#')) || (!fnd)); i--) {
        if ((location[i] == wxT(':')) && (i != 1 /*win: C:\path*/)) fnd = true;
    }
    if (!fnd) return wxT("file");
    for (++i; (i < l) && (location[i] != wxT(':')); i++) s << location[i];
    return s;
}


wxString wxFileSystemHandler::GetLeftLocation(const wxString& location) const
{
    int i;
    bool fnd = false;

    for (i = location.Length()-1; i >= 0; i--) {
        if ((location[i] == wxT(':')) && (i != 1 /*win: C:\path*/)) fnd = true;
        else if (fnd && (location[i] == wxT('#'))) return location.Left(i);
    }
    return wxEmptyString;
}

wxString wxFileSystemHandler::GetRightLocation(const wxString& location) const
{
    int i, l = location.Length();
    int l2 = l + 1;

    for (i = l-1;
         (i >= 0) &&
         ((location[i] != wxT(':')) || (i == 1) || (location[i-2] == wxT(':')));
         i--)
    {
        if (location[i] == wxT('#')) l2 = i + 1;
    }
    if (i == 0) return wxEmptyString;
    else return location.Mid(i + 1, l2 - i - 2);
}

wxString wxFileSystemHandler::GetAnchor(const wxString& location) const
{
    wxChar c;
    int l = location.Length();

    for (int i = l-1; i >= 0; i--) {
        c = location[i];
        if (c == wxT('#')) return location.Right(l-i-1);
        else if ((c == wxT('.')) || (c == wxT('/')) || (c == wxT('\\')) || (c == wxT(':'))) return wxEmptyString;
    }
    return wxEmptyString;
}


wxString wxFileSystemHandler::FindFirst(const wxString& WXUNUSED(spec),
                                        int WXUNUSED(flags))
{
    return wxEmptyString;
}

wxString wxFileSystemHandler::FindNext()
{
    return wxEmptyString;
}

//--------------------------------------------------------------------------------
// wxLocalFSHandler
//--------------------------------------------------------------------------------


wxString wxLocalFSHandler::ms_root;

bool wxLocalFSHandler::CanOpen(const wxString& location)
{
    return GetProtocol(location) == wxT("file");
}

wxFSFile* wxLocalFSHandler::OpenFile(wxFileSystem& WXUNUSED(fs), const wxString& location)
{
    // location has Unix path separators
    wxString right = GetRightLocation(location);
    wxFileName fn = wxFileSystem::URLToFileName(right);
    wxString fullpath = ms_root + fn.GetFullPath();

    if (!wxFileExists(fullpath))
        return (wxFSFile*) NULL;

    // we need to check whether we can really read from this file, otherwise
    // wxFSFile is not going to work
    wxFFileInputStream *is = new wxFFileInputStream(fullpath);
    if ( !is->Ok() )
    {
        delete is;
        return (wxFSFile*) NULL;
    }

    return new wxFSFile(is,
                        right,
                        GetMimeTypeFromExt(location),
                        GetAnchor(location)
#if wxUSE_DATETIME
                        ,wxDateTime(wxFileModificationTime(fullpath))
#endif // wxUSE_DATETIME
                        );
}

wxString wxLocalFSHandler::FindFirst(const wxString& spec, int flags)
{
    wxFileName fn = wxFileSystem::URLToFileName(GetRightLocation(spec));
    return wxFindFirstFile(ms_root + fn.GetFullPath(), flags);
}

wxString wxLocalFSHandler::FindNext()
{
    return wxFindNextFile();
}



//-----------------------------------------------------------------------------
// wxFileSystem
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxFileSystem, wxObject)
IMPLEMENT_ABSTRACT_CLASS(wxFSFile, wxObject)


wxList wxFileSystem::m_Handlers;


static wxString MakeCorrectPath(const wxString& path)
{
    wxString p(path);
    wxString r;
    int i, j, cnt;

    cnt = p.Length();
    for (i = 0; i < cnt; i++)
      if (p.GetChar(i) == wxT('\\')) p.GetWritableChar(i) = wxT('/'); // Want to be windows-safe

    if (p.Left(2) == wxT("./")) { p = p.Mid(2); cnt -= 2; }

    if (cnt < 3) return p;

    r << p.GetChar(0) << p.GetChar(1);

    // skip trailing ../.., if any
    for (i = 2; i < cnt && (p.GetChar(i) == wxT('/') || p.GetChar(i) == wxT('.')); i++) r << p.GetChar(i);

    // remove back references: translate dir1/../dir2 to dir2
    for (; i < cnt; i++)
    {
        r << p.GetChar(i);
        if (p.GetChar(i) == wxT('/') && p.GetChar(i-1) == wxT('.') && p.GetChar(i-2) == wxT('.'))
        {
            for (j = r.Length() - 2; j >= 0 && r.GetChar(j) != wxT('/') && r.GetChar(j) != wxT(':'); j--) {}
            if (j >= 0 && r.GetChar(j) != wxT(':'))
            {
                for (j = j - 1; j >= 0 && r.GetChar(j) != wxT('/') && r.GetChar(j) != wxT(':'); j--) {}
                r.Remove(j + 1);
            }
        }
    }

    for (; i < cnt; i++) r << p.GetChar(i);

    return r;
}


void wxFileSystem::ChangePathTo(const wxString& location, bool is_dir)
{
    int i, pathpos = -1;

    m_Path = MakeCorrectPath(location);

    if (is_dir)
    {
        if (m_Path.Length() > 0 && m_Path.Last() != wxT('/') && m_Path.Last() != wxT(':'))
            m_Path << wxT('/');
    }

    else
    {
        for (i = m_Path.Length()-1; i >= 0; i--)
        {
            if (m_Path[(unsigned int) i] == wxT('/'))
            {
                if ((i > 1) && (m_Path[(unsigned int) (i-1)] == wxT('/')) && (m_Path[(unsigned int) (i-2)] == wxT(':')))
                {
                    i -= 2;
                    continue;
                }
                else
                {
                    pathpos = i;
                    break;
                }
            }
            else if (m_Path[(unsigned int) i] == wxT(':')) {
                pathpos = i;
                break;
            }
        }
        if (pathpos == -1)
        {
            for (i = 0; i < (int) m_Path.Length(); i++)
            {
                if (m_Path[(unsigned int) i] == wxT(':'))
                {
                    m_Path.Remove(i+1);
                    break;
                }
            }
            if (i == (int) m_Path.Length())
                m_Path = wxEmptyString;
        }
        else
        {
            m_Path.Remove(pathpos+1);
        }
    }
}



wxFSFile* wxFileSystem::OpenFile(const wxString& location)
{
    wxString loc = MakeCorrectPath(location);
    unsigned i, ln;
    wxChar meta;
    wxFSFile *s = NULL;
    wxList::compatibility_iterator node;

    ln = loc.Length();
    meta = 0;
    for (i = 0; i < ln; i++)
    {
        switch (loc[i])
        {
            case wxT('/') : case wxT(':') : case wxT('#') :
                meta = loc[i];
                break;
        }
        if (meta != 0) break;
    }
    m_LastName = wxEmptyString;

    // try relative paths first :
    if (meta != wxT(':'))
    {
        node = m_Handlers.GetFirst();
        while (node)
        {
            wxFileSystemHandler *h = (wxFileSystemHandler*) node -> GetData();
            if (h->CanOpen(m_Path + loc))
            {
                s = h->OpenFile(*this, m_Path + loc);
                if (s) { m_LastName = m_Path + loc; break; }
            }
            node = node->GetNext();
        }
    }

    // if failed, try absolute paths :
    if (s == NULL)
    {
        node = m_Handlers.GetFirst();
        while (node)
        {
            wxFileSystemHandler *h = (wxFileSystemHandler*) node->GetData();
            if (h->CanOpen(loc))
            {
                s = h->OpenFile(*this, loc);
                if (s) { m_LastName = loc; break; }
            }
            node = node->GetNext();
        }
    }
    return (s);
}



wxString wxFileSystem::FindFirst(const wxString& spec, int flags)
{
    wxList::compatibility_iterator node;
    wxString spec2(spec);

    m_FindFileHandler = NULL;

    for (int i = spec2.Length()-1; i >= 0; i--)
        if (spec2[(unsigned int) i] == wxT('\\')) spec2.GetWritableChar(i) = wxT('/'); // Want to be windows-safe

    node = m_Handlers.GetFirst();
    while (node)
    {
        m_FindFileHandler = (wxFileSystemHandler*) node -> GetData();
        if (m_FindFileHandler -> CanOpen(m_Path + spec2))
            return m_FindFileHandler -> FindFirst(m_Path + spec2, flags);
        node = node->GetNext();
    }

    node = m_Handlers.GetFirst();
    while (node)
    {
        m_FindFileHandler = (wxFileSystemHandler*) node -> GetData();
        if (m_FindFileHandler -> CanOpen(spec2))
            return m_FindFileHandler -> FindFirst(spec2, flags);
        node = node->GetNext();
    }

    return wxEmptyString;
}



wxString wxFileSystem::FindNext()
{
    if (m_FindFileHandler == NULL) return wxEmptyString;
    else return m_FindFileHandler -> FindNext();
}



void wxFileSystem::AddHandler(wxFileSystemHandler *handler)
{
    m_Handlers.Append(handler);
}


void wxFileSystem::CleanUpHandlers()
{
    WX_CLEAR_LIST(wxList, m_Handlers);
}

static const wxString g_unixPathString(wxT("/"));
static const wxString g_nativePathString(wxFILE_SEP_PATH);

// Returns the native path for a file URL
wxFileName wxFileSystem::URLToFileName(const wxString& url)
{
    wxString path = url;

    if ( path.Find(wxT("file://")) == 0 )
    {
        path = path.Mid(7);
    }
    else if ( path.Find(wxT("file:")) == 0 )
    {
        path = path.Mid(5);
    }
    // Remove preceding double slash on Mac Classic
#if defined(__WXMAC__) && !defined(__UNIX__)
    else if ( path.Find(wxT("//")) == 0 )
        path = path.Mid(2);
#endif

    path.Replace(wxT("%25"), wxT("%"));
    path.Replace(wxT("%3A"), wxT(":"));

#ifdef __WXMSW__
    // file urls either start with a forward slash (local harddisk),
    // otherwise they have a servername/sharename notation,
    // which only exists on msw and corresponds to a unc
    if ( path[0u] == wxT('/') && path [1u] != wxT('/'))
    {
        path = path.Mid(1);
    }
    else if ( (url.Find(wxT("file://")) == 0) &&
              (path.Find(wxT('/')) != wxNOT_FOUND) &&
              (path.Length() > 1) && (path[1u] != wxT(':')) )
    {
        path = wxT("//") + path;
    }
#endif

    path.Replace(g_unixPathString, g_nativePathString);

    return wxFileName(path, wxPATH_NATIVE);
}

// Returns the file URL for a native path
wxString wxFileSystem::FileNameToURL(const wxFileName& filename)
{
    wxFileName fn = filename;
    fn.Normalize(wxPATH_NORM_DOTS | wxPATH_NORM_TILDE | wxPATH_NORM_ABSOLUTE);
    wxString url = fn.GetFullPath(wxPATH_NATIVE);

#ifndef __UNIX__
    // unc notation, wxMSW
    if ( url.Find(wxT("\\\\")) == 0 )
    {
        url = wxT("//") + url.Mid(2);
    }
    else
    {
        url = wxT("/") + url;
#ifdef __WXMAC__
        url = wxT("/") + url;
#endif

    }
#endif

    url.Replace(g_nativePathString, g_unixPathString);
    url.Replace(wxT("%"), wxT("%25"));
    url.Replace(wxT(":"), wxT("%3A"));
    url = wxT("file:") + url;
    return url;
}


///// Module:

class wxFileSystemModule : public wxModule
{
    DECLARE_DYNAMIC_CLASS(wxFileSystemModule)

    public:
        virtual bool OnInit()
        {
            wxFileSystem::AddHandler(new wxLocalFSHandler);
            return true;
        }
        virtual void OnExit()
        {
            wxFileSystem::CleanUpHandlers();
        }
};

IMPLEMENT_DYNAMIC_CLASS(wxFileSystemModule, wxModule)

#endif
  // wxUSE_FILESYSTEM



