///////////////////////////////////////////////////////////////////////////////
// Name:        src/common/dircmn.cpp
// Purpose:     wxDir methods common to all implementations
// Author:      Vadim Zeitlin
// Modified by:
// Created:     19.05.01
// RCS-ID:      $Id: dircmn.cpp,v 1.19 2005/05/17 12:19:07 JS Exp $
// Copyright:   (c) 2001 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// License:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

/* this is done in platform-specific files
#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "dir.h"
#endif
*/

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/string.h"
    #include "wx/log.h"
    #include "wx/intl.h"
    #include "wx/filefn.h"
#endif //WX_PRECOMP

#include "wx/arrstr.h"
#include "wx/dir.h"

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxDirTraverser
// ----------------------------------------------------------------------------

wxDirTraverseResult
wxDirTraverser::OnOpenError(const wxString& WXUNUSED(dirname))
{
    return wxDIR_IGNORE;
}

// ----------------------------------------------------------------------------
// wxDir::HasFiles() and HasSubDirs()
// ----------------------------------------------------------------------------

// dumb generic implementation

bool wxDir::HasFiles(const wxString& spec)
{
    wxString s;
    return GetFirst(&s, spec, wxDIR_FILES | wxDIR_HIDDEN);
}

// we have a (much) faster version for Unix
#if (defined(__CYGWIN__) && defined(__WINDOWS__)) || !defined(__UNIX_LIKE__) || defined(__WXMAC__) || defined(__EMX__) || defined(__WINE__)

bool wxDir::HasSubDirs(const wxString& spec)
{
    wxString s;
    return GetFirst(&s, spec, wxDIR_DIRS | wxDIR_HIDDEN);
}

#endif // !Unix

// ----------------------------------------------------------------------------
// wxDir::Traverse()
// ----------------------------------------------------------------------------

size_t wxDir::Traverse(wxDirTraverser& sink,
                       const wxString& filespec,
                       int flags) const
{
    wxCHECK_MSG( IsOpened(), (size_t)-1,
                 _T("dir must be opened before traversing it") );

    // the total number of files found
    size_t nFiles = 0;

    // the name of this dir with path delimiter at the end
    wxString prefix = GetName();
    prefix += wxFILE_SEP_PATH;

    // first, recurse into subdirs
    if ( flags & wxDIR_DIRS )
    {
        wxString dirname;
        for ( bool cont = GetFirst(&dirname, wxEmptyString, wxDIR_DIRS | (flags & wxDIR_HIDDEN) );
              cont;
              cont = cont && GetNext(&dirname) )
        {
            const wxString fulldirname = prefix + dirname;

            switch ( sink.OnDir(fulldirname) )
            {
                default:
                    wxFAIL_MSG(_T("unexpected OnDir() return value") );
                    // fall through

                case wxDIR_STOP:
                    cont = false;
                    break;

                case wxDIR_CONTINUE:
                    {
                        wxDir subdir;

                        // don't give the error messages for the directories
                        // which we can't open: there can be all sorts of good
                        // reason for this (e.g. insufficient privileges) and
                        // this shouldn't be treated as an error -- instead
                        // let the user code decide what to do
                        bool ok;
                        do
                        {
                            wxLogNull noLog;
                            ok = subdir.Open(fulldirname);
                            if ( !ok )
                            {
                                // ask the user code what to do
                                bool tryagain;
                                switch ( sink.OnOpenError(fulldirname) )
                                {
                                    default:
                                        wxFAIL_MSG(_T("unexpected OnOpenError() return value") );
                                        // fall through

                                    case wxDIR_STOP:
                                        cont = false;
                                        // fall through

                                    case wxDIR_IGNORE:
                                        tryagain = false;
                                        break;

                                    case wxDIR_CONTINUE:
                                        tryagain = true;
                                }

                                if ( !tryagain )
                                    break;
                            }
                        }
                        while ( !ok );

                        if ( ok )
                        {
                            nFiles += subdir.Traverse(sink, filespec, flags);
                        }
                    }
                    break;

                case wxDIR_IGNORE:
                    // nothing to do
                    ;
            }
        }
    }

    // now enum our own files
    if ( flags & wxDIR_FILES )
    {
        flags &= ~wxDIR_DIRS;

        wxString filename;
        bool cont = GetFirst(&filename, filespec, flags);
        while ( cont )
        {
            wxDirTraverseResult res = sink.OnFile(prefix + filename);
            if ( res == wxDIR_STOP )
                break;

            wxASSERT_MSG( res == wxDIR_CONTINUE,
                          _T("unexpected OnFile() return value") );

            nFiles++;

            cont = GetNext(&filename);
        }
    }

    return nFiles;
}

// ----------------------------------------------------------------------------
// wxDir::GetAllFiles()
// ----------------------------------------------------------------------------

class wxDirTraverserSimple : public wxDirTraverser
{
public:
    wxDirTraverserSimple(wxArrayString& files) : m_files(files) { }

    virtual wxDirTraverseResult OnFile(const wxString& filename)
    {
        m_files.push_back(filename);
        return wxDIR_CONTINUE;
    }

    virtual wxDirTraverseResult OnDir(const wxString& WXUNUSED(dirname))
    {
        return wxDIR_CONTINUE;
    }

private:
    wxArrayString& m_files;

    DECLARE_NO_COPY_CLASS(wxDirTraverserSimple)
};

/* static */
size_t wxDir::GetAllFiles(const wxString& dirname,
                          wxArrayString *files,
                          const wxString& filespec,
                          int flags)
{
    wxCHECK_MSG( files, (size_t)-1, _T("NULL pointer in wxDir::GetAllFiles") );

    size_t nFiles = 0;

    wxDir dir(dirname);
    if ( dir.IsOpened() )
    {
        wxDirTraverserSimple traverser(*files);

        nFiles += dir.Traverse(traverser, filespec, flags);
    }

    return nFiles;
}

