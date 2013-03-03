/////////////////////////////////////////////////////////////////////////////
// Name:        filefn.h
// Purpose:     File- and directory-related functions
// Author:      Julian Smart
// Modified by:
// Created:     29/01/98
// RCS-ID:      $Id: filefn.h,v 1.125 2005/05/19 17:03:10 MW Exp $
// Copyright:   (c) 1998 Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef   _FILEFN_H_
#define   _FILEFN_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "filefn.h"
#endif

#include "wx/list.h"
#include "wx/arrstr.h"

#ifdef __WXWINCE__
    #include "wx/msw/wince/time.h"
    #include "wx/msw/private.h"
#else
    #include <time.h>
#endif

#ifdef __WXWINCE__
// Nothing
#elif !defined(__MWERKS__)
    #include <sys/types.h>
    #include <sys/stat.h>
#else
    #ifdef __MACH__
        #include <sys/types.h>
        #include <utime.h>
        #include <sys/stat.h>
        #include <unistd.h>
    #else
        #include <stat.h>
        #include <unistd.h>
        #include <unix.h>
    #endif
#endif

#ifdef __OS2__
// need to check for __OS2__ first since currently both
// __OS2__ and __UNIX__ are defined.
    #include <process.h>
    #include "wx/os2/private.h"
    #include <io.h>
    #ifdef __EMX__
        #include <unistd.h>
    #endif
#elif defined(__UNIX__)
    #include <unistd.h>
    #include <dirent.h>
#endif

#if defined(__WINDOWS__) && !defined(__WXMICROWIN__)
#if !defined( __GNUWIN32__ ) && !defined( __MWERKS__ ) && !defined(__SALFORDC__) && !defined(__WXWINCE__) && !defined(__CYGWIN__)
    #include <direct.h>
    #include <dos.h>
    #include <io.h>
#endif // __WINDOWS__
#endif // native Win compiler

#if defined(__DOS__)
    #ifdef __WATCOMC__
        #include <direct.h>
        #include <dos.h>
        #include <io.h>
    #endif
    #ifdef __DJGPP__
        #include <io.h>
        #include <unistd.h>
    #endif
#endif

#ifdef __BORLANDC__ // Please someone tell me which version of Borland needs
                    // this (3.1 I believe) and how to test for it.
                    // If this works for Borland 4.0 as well, then no worries.
    #include <dir.h>
#endif

#ifdef __SALFORDC__
    #include <dir.h>
    #include <unix.h>
#endif

#ifndef __WXWINCE__
    #include  <fcntl.h>       // O_RDONLY &c
#endif
// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

#ifdef __WXWINCE__
    typedef long off_t;
#else
    // define off_t
    #if !defined(__WXMAC__) || defined(__UNIX__) || defined(__MACH__)
        #include  <sys/types.h>
    #else
        typedef long off_t;
    #endif
#endif

#if (defined(__VISUALC__) && !defined(__WXWINCE__)) || ( defined(__MWERKS__) && defined( __INTEL__) )
    typedef _off_t off_t;
#elif defined(__SYMANTEC__)
    typedef long off_t;
#elif defined(__MWERKS__) && !defined(__INTEL__) && !defined(__MACH__)
    typedef long off_t;
#endif

enum wxSeekMode
{
  wxFromStart,
  wxFromCurrent,
  wxFromEnd
};

enum wxFileKind
{
  wxFILE_KIND_UNKNOWN,
  wxFILE_KIND_DISK,     // a file supporting seeking to arbitrary offsets
  wxFILE_KIND_TERMINAL, // a tty
  wxFILE_KIND_PIPE      // a pipe
};

// ----------------------------------------------------------------------------
// declare our versions of low level file functions: some compilers prepend
// underscores to the usual names, some also have Unicode versions of them
// ----------------------------------------------------------------------------

// Wrappers around Win32 api functions like CreateFile, ReadFile and such
// Implemented in filefnwce.cpp
#if defined( __WXWINCE__)
    typedef __int64 wxFileOffset;
    #define wxFileOffsetFmtSpec _("I64")
    int wxOpen(const wxChar *filename, int oflag, int WXUNUSED(pmode));
    int wxAccess(const wxChar *name, int WXUNUSED(how));
    int wxClose(int fd);
    int wxFsync(int WXUNUSED(fd));
    int wxRead(int fd, void *buf, unsigned int count);
    int wxWrite(int fd, const void *buf, unsigned int count);
    int wxEof(int fd);
    wxFileOffset wxSeek(int fd, wxFileOffset offset, int origin);
    #define wxLSeek wxSeek
    wxFileOffset wxTell(int fd);

    // always Unicode under WinCE
    #define   wxMkDir      _wmkdir
    #define   wxRmDir      _wrmdir
    #define   wxStat       _wstat
    #define   wxStructStat struct _stat

// Microsoft compiler loves underscores, feed them to it
#elif defined( __VISUALC__ ) \
    || ( defined(__MINGW32__) && !defined(__WINE__) && wxCHECK_W32API_VERSION( 0, 5 ) ) \
    || ( defined(__MWERKS__) && defined(__WXMSW__) ) \
    || ( defined(__DMC__) && defined(__WXMSW__) ) \
    || ( defined(__WATCOMC__) && defined(__WXMSW__) )

    // detect compilers which have support for huge files (currently only
    // Digital Mars doesn't)
    #ifndef __WXPALMOS__
    #include "wx/msw/private.h"
    #endif

    #undef wxHAS_HUGE_FILES
    #if defined(__MINGW32__)
        #define wxHAS_HUGE_FILES 1
    #elif defined(__MWERKS__)
        #define wxHAS_HUGE_FILES 0
    #elif defined(__DMC__)
        #define wxHAS_HUGE_FILES 0
    #elif ((_INTEGRAL_MAX_BITS >= 64) || defined(_LARGE_FILES))
        #define wxHAS_HUGE_FILES 1
    #else
        #define wxHAS_HUGE_FILES 0
    #endif

    // functions
    #if defined(__BORLANDC__) || defined(__WATCOMC__)
        #define   _tell        tell
    #endif

    #if wxHAS_HUGE_FILES
        typedef wxLongLong_t wxFileOffset;
        #define wxFileOffsetFmtSpec wxLongLongFmtSpec
    #else
        typedef off_t wxFileOffset;
    #endif

    #define   wxClose      _close

    #if defined(__MWERKS__)
        #if __MSL__ >= 0x6000
            #define wxRead(fd, buf, nCount)  _read(fd, (void *)buf, nCount)
            #define wxWrite(fd, buf, nCount) _write(fd, (void *)buf, nCount)
        #else
            #define wxRead(fd, buf, nCount)\
                  _read(fd, (const char *)buf, nCount)
            #define wxWrite(fd, buf, nCount)\
                  _write(fd, (const char *)buf, nCount)
        #endif
    #else
        #if defined(__DMC__) || defined(__WATCOMC__)
            #define wxRead        ::read
            #define wxWrite       ::write
        #else
            #define wxRead        _read
            #define wxWrite        _write
        #endif
    #endif
    #if wxHAS_HUGE_FILES
        #define   wxSeek      _lseeki64
        #define   wxLseek      _lseeki64
        #define   wxTell       _telli64
    #else
        #define   wxSeek      _lseek
        #define   wxLseek      _lseek
        #define   wxTell       _tell
    #endif
    #define   wxFsync      _commit
    #if defined(__WATCOMC__)
        #define   wxEof        ::eof
    #else
        #define   wxEof        _eof
    #endif

    #if wxUSE_UNICODE
        #if wxUSE_UNICODE_MSLU
            #define   wxOpen       wxMSLU__wopen

            #define   wxAccess     wxMSLU__waccess
            #define   wxMkDir      wxMSLU__wmkdir
            #define   wxRmDir      wxMSLU__wrmdir
            #if wxHAS_HUGE_FILES
                #define   wxStat       wxMSLU__wstati64
            #else
                #define   wxStat       wxMSLU__wstat
            #endif
        #else
            #define   wxOpen       _wopen
            #define   wxAccess     _waccess
            #define   wxMkDir      _wmkdir
            #define   wxRmDir      _wrmdir
            #if wxHAS_HUGE_FILES
                #define   wxStat       _wstati64
            #else
                #define   wxStat       _wstat
            #endif
        #endif
    #else // !wxUSE_UNICODE
        #ifdef __BORLANDC__
            #define   wxOpen       open
        #else
            #define   wxOpen       _open
        #endif
        #define   wxAccess     _access
        #define   wxMkDir      _mkdir
        #ifdef __WATCOMC__
            #define   wxRmDir      rmdir
        #else
            #define   wxRmDir      _rmdir
        #endif
        #if wxHAS_HUGE_FILES
            #define   wxStat       _stati64
        #else
            #define   wxStat       _stat
        #endif
    #endif

    // types: notice that Watcom is the only compiler to have a wide char
    // version of struct stat as well as a wide char stat function variant
    #if wxHAS_HUGE_FILES
        #if wxUSE_UNICODE && defined(__WATCOMC__)
            #define   wxStructStat struct _wstati64
        #else
            #define   wxStructStat struct _stati64
        #endif
    #else
        #if wxUSE_UNICODE && defined(__WATCOMC__)
            #define   wxStructStat struct _wstat
        #else
            #define   wxStructStat struct _stat
        #endif
    #endif

    // constants (unless already defined by the user code)
    #if !defined(__BORLANDC__) && !defined(__WATCOMC__) && !defined(__WXPALMOS__)
        #ifndef O_RDONLY
            #define   O_RDONLY    _O_RDONLY
            #define   O_WRONLY    _O_WRONLY
            #define   O_RDWR      _O_RDWR
            #define   O_EXCL      _O_EXCL
            #define   O_CREAT     _O_CREAT
            #define   O_BINARY    _O_BINARY
        #endif

        #ifndef S_IFMT
            #define   S_IFMT      _S_IFMT
            #define   S_IFDIR     _S_IFDIR
            #define   S_IFREG     _S_IFREG
        #endif
    #endif

    // It's a private define, undefine it so nobody gets tempted to use it
    #undef wxHAS_HUGE_FILES
#else // Unix platforms using configure
    typedef off_t wxFileOffset;
    #ifdef _LARGE_FILES
        #define wxFileOffsetFmtSpec wxLongLongFmtSpec
        wxCOMPILE_TIME_ASSERT( sizeof(off_t) == sizeof(wxLongLong_t),
                                BadFileSizeType );
    #else
        #define wxFileOffsetFmtSpec _T("")
    #endif
    // functions
    #define   wxClose      close
    #define   wxRead       ::read
    #define   wxWrite      ::write
    #define   wxLseek      lseek
    #define   wxSeek       lseek
    #define   wxFsync      commit
    #define   wxEof        eof

    #define   wxMkDir      mkdir
    #define   wxRmDir      rmdir

    #define   wxTell(fd)   lseek(fd, 0, SEEK_CUR)

    #define   wxStructStat struct stat

    #if wxUSE_UNICODE
        #define wxNEED_WX_UNISTD_H
        #if defined(__DMC__)
            typedef unsigned long mode_t;
        #endif
        WXDLLIMPEXP_BASE int wxStat( const wxChar *file_name, wxStructStat *buf );
        WXDLLIMPEXP_BASE int wxAccess( const wxChar *pathname, int mode );
        WXDLLIMPEXP_BASE int wxOpen( const wxChar *pathname, int flags, mode_t mode );
    #else
        #define   wxOpen       open
        #define   wxStat       stat
        #define   wxAccess     access
    #endif
#endif // platforms

#if defined(__VISAGECPP__) && __IBMCPP__ >= 400
//
// VisualAge C++ V4.0 cannot have any external linkage const decs
// in headers included by more than one primary source
//
extern const int wxInvalidOffset;
#else
const int wxInvalidOffset = -1;
#endif

// ----------------------------------------------------------------------------
// functions
// ----------------------------------------------------------------------------
WXDLLIMPEXP_BASE bool wxFileExists(const wxString& filename);

// does the path exist? (may have or not '/' or '\\' at the end)
WXDLLIMPEXP_BASE bool wxDirExists(const wxChar *pszPathName);

WXDLLIMPEXP_BASE bool wxIsAbsolutePath(const wxString& filename);

// Get filename
WXDLLIMPEXP_BASE wxChar* wxFileNameFromPath(wxChar *path);
WXDLLIMPEXP_BASE wxString wxFileNameFromPath(const wxString& path);

// Get directory
WXDLLIMPEXP_BASE wxString wxPathOnly(const wxString& path);

// wxString version
WXDLLIMPEXP_BASE wxString wxRealPath(const wxString& path);

WXDLLIMPEXP_BASE void wxDos2UnixFilename(wxChar *s);

WXDLLIMPEXP_BASE void wxUnix2DosFilename(wxChar *s);

// Strip the extension, in situ
WXDLLIMPEXP_BASE void wxStripExtension(wxChar *buffer);
WXDLLIMPEXP_BASE void wxStripExtension(wxString& buffer);

// Get a temporary filename
WXDLLIMPEXP_BASE wxChar* wxGetTempFileName(const wxString& prefix, wxChar *buf = (wxChar *) NULL);
WXDLLIMPEXP_BASE bool wxGetTempFileName(const wxString& prefix, wxString& buf);

// Expand file name (~/ and ${OPENWINHOME}/ stuff)
WXDLLIMPEXP_BASE wxChar* wxExpandPath(wxChar *dest, const wxChar *path);
WXDLLIMPEXP_BASE bool wxExpandPath(wxString& dest, const wxChar *path);

// Contract w.r.t environment (</usr/openwin/lib, OPENWHOME> -> ${OPENWINHOME}/lib)
// and make (if under the home tree) relative to home
// [caller must copy-- volatile]
WXDLLIMPEXP_BASE wxChar* wxContractPath(const wxString& filename,
                                   const wxString& envname = wxEmptyString,
                                   const wxString& user = wxEmptyString);

// Destructive removal of /./ and /../ stuff
WXDLLIMPEXP_BASE wxChar* wxRealPath(wxChar *path);

// Allocate a copy of the full absolute path
WXDLLIMPEXP_BASE wxChar* wxCopyAbsolutePath(const wxString& path);

// Get first file name matching given wild card.
// Flags are reserved for future use.
#define wxFILE  1
#define wxDIR   2
WXDLLIMPEXP_BASE wxString wxFindFirstFile(const wxChar *spec, int flags = wxFILE);
WXDLLIMPEXP_BASE wxString wxFindNextFile();

// Does the pattern contain wildcards?
WXDLLIMPEXP_BASE bool wxIsWild(const wxString& pattern);

// Does the pattern match the text (usually a filename)?
// If dot_special is true, doesn't match * against . (eliminating
// `hidden' dot files)
WXDLLIMPEXP_BASE bool wxMatchWild(const wxString& pattern,  const wxString& text, bool dot_special = true);

// Concatenate two files to form third
WXDLLIMPEXP_BASE bool wxConcatFiles(const wxString& file1, const wxString& file2, const wxString& file3);

// Copy file1 to file2
WXDLLIMPEXP_BASE bool wxCopyFile(const wxString& file1, const wxString& file2,
                                 bool overwrite = true);

// Remove file
WXDLLIMPEXP_BASE bool wxRemoveFile(const wxString& file);

// Rename file
WXDLLIMPEXP_BASE bool wxRenameFile(const wxString& file1, const wxString& file2);

// Get current working directory.
// If buf is NULL, allocates space using new, else
// copies into buf.
// IMPORTANT NOTE getcwd is know not to work under some releases
// of Win32s 1.3, according to MS release notes!
WXDLLIMPEXP_BASE wxChar* wxGetWorkingDirectory(wxChar *buf = (wxChar *) NULL, int sz = 1000);
// new and preferred version of wxGetWorkingDirectory
// NB: can't have the same name because of overloading ambiguity
WXDLLIMPEXP_BASE wxString wxGetCwd();

// Set working directory
WXDLLIMPEXP_BASE bool wxSetWorkingDirectory(const wxString& d);

// Make directory
WXDLLIMPEXP_BASE bool wxMkdir(const wxString& dir, int perm = 0777);

// Remove directory. Flags reserved for future use.
WXDLLIMPEXP_BASE bool wxRmdir(const wxString& dir, int flags = 0);

// Return the type of an open file
WXDLLIMPEXP_BASE wxFileKind wxGetFileKind(int fd);
WXDLLIMPEXP_BASE wxFileKind wxGetFileKind(FILE *fp);

// compatibility defines, don't use in new code
// consider removal droping 2.4 compatibility
// #if WXWIN_COMPATIBILITY_2_4
wxDEPRECATED( inline bool wxPathExists(const wxChar *pszPathName) );
inline bool wxPathExists(const wxChar *pszPathName)
{
    return wxDirExists(pszPathName);
}
// #endif //WXWIN_COMPATIBILITY_2_4

// ----------------------------------------------------------------------------
// separators in file names
// ----------------------------------------------------------------------------

// between file name and extension
#define wxFILE_SEP_EXT        wxT('.')

// between drive/volume name and the path
#define wxFILE_SEP_DSK        wxT(':')

// between the path components
#define wxFILE_SEP_PATH_DOS   wxT('\\')
#define wxFILE_SEP_PATH_UNIX  wxT('/')
#define wxFILE_SEP_PATH_MAC   wxT(':')
#define wxFILE_SEP_PATH_VMS   wxT('.') // VMS also uses '[' and ']'

// separator in the path list (as in PATH environment variable)
// there is no PATH variable in Classic Mac OS so just use the
// semicolon (it must be different from the file name separator)
// NB: these are strings and not characters on purpose!
#define wxPATH_SEP_DOS        wxT(";")
#define wxPATH_SEP_UNIX       wxT(":")
#define wxPATH_SEP_MAC        wxT(";")

// platform independent versions
#if defined(__UNIX__) && !defined(__OS2__)
  // CYGWIN also uses UNIX settings
  #define wxFILE_SEP_PATH     wxFILE_SEP_PATH_UNIX
  #define wxPATH_SEP          wxPATH_SEP_UNIX
#elif defined(__MAC__)
  #define wxFILE_SEP_PATH     wxFILE_SEP_PATH_MAC
  #define wxPATH_SEP          wxPATH_SEP_MAC
#else   // Windows and OS/2
  #define wxFILE_SEP_PATH     wxFILE_SEP_PATH_DOS
  #define wxPATH_SEP          wxPATH_SEP_DOS
#endif  // Unix/Windows

// this is useful for wxString::IsSameAs(): to compare two file names use
// filename1.IsSameAs(filename2, wxARE_FILENAMES_CASE_SENSITIVE)
#if defined(__UNIX__) && !defined(__DARWIN__) && !defined(__OS2__)
  #define wxARE_FILENAMES_CASE_SENSITIVE  true
#else   // Windows, Mac OS and OS/2
  #define wxARE_FILENAMES_CASE_SENSITIVE  false
#endif  // Unix/Windows

// is the char a path separator?
inline bool wxIsPathSeparator(wxChar c)
{
    // under DOS/Windows we should understand both Unix and DOS file separators
#if ( defined(__UNIX__) && !defined(__OS2__) )|| defined(__MAC__)
    return c == wxFILE_SEP_PATH;
#else
    return c == wxFILE_SEP_PATH_DOS || c == wxFILE_SEP_PATH_UNIX;
#endif
}

// does the string ends with path separator?
WXDLLIMPEXP_BASE bool wxEndsWithPathSeparator(const wxChar *pszFileName);

// split the full path into path (including drive for DOS), name and extension
// (understands both '/' and '\\')
WXDLLIMPEXP_BASE void wxSplitPath(const wxChar *pszFileName,
                             wxString *pstrPath,
                             wxString *pstrName,
                             wxString *pstrExt);

// find a file in a list of directories, returns false if not found
WXDLLIMPEXP_BASE bool wxFindFileInPath(wxString *pStr, const wxChar *pszPath, const wxChar *pszFile);

// Get the OS directory if appropriate (such as the Windows directory).
// On non-Windows platform, probably just return the empty string.
WXDLLIMPEXP_BASE wxString wxGetOSDirectory();

// Get file modification time
WXDLLIMPEXP_BASE time_t wxFileModificationTime(const wxString& filename);

// Parses the wildCard, returning the number of filters.
// Returns 0 if none or if there's a problem,
// The arrays will contain an equal number of items found before the error.
// wildCard is in the form:
// "All files (*)|*|Image Files (*.jpeg *.png)|*.jpg;*.png"
WXDLLIMPEXP_BASE int wxParseCommonDialogsFilter(const wxString& wildCard, wxArrayString& descriptions, wxArrayString& filters);

// ----------------------------------------------------------------------------
// classes
// ----------------------------------------------------------------------------

#ifdef __UNIX__

// set umask to the given value in ctor and reset it to the old one in dtor
class WXDLLIMPEXP_BASE wxUmaskChanger
{
public:
    // change the umask to the given one if it is not -1: this allows to write
    // the same code whether you really want to change umask or not, as is in
    // wxFileConfig::Flush() for example
    wxUmaskChanger(int umaskNew)
    {
        m_umaskOld = umaskNew == -1 ? -1 : (int)umask((mode_t)umaskNew);
    }

    ~wxUmaskChanger()
    {
        if ( m_umaskOld != -1 )
            umask((mode_t)m_umaskOld);
    }

private:
    int m_umaskOld;
};

// this macro expands to an "anonymous" wxUmaskChanger object under Unix and
// nothing elsewhere
#define wxCHANGE_UMASK(m) wxUmaskChanger wxMAKE_UNIQUE_NAME(umaskChanger_)(m)

#else // !__UNIX__

#define wxCHANGE_UMASK(m)

#endif // __UNIX__/!__UNIX__


// Path searching
class WXDLLIMPEXP_BASE wxPathList : public wxStringList
{
public:
    // avoid GCC warning about virtual functions w/o virtual dtor
    virtual ~wxPathList() {}

    // Adds all paths in environment variable
    void AddEnvList(const wxString& envVariable);

    void Add(const wxString& path);
    // Find the first full path for which the file exists
    wxString FindValidPath(const wxString& filename);
    // Find the first full path for which the file exists; ensure it's an
    // absolute path that gets returned.
    wxString FindAbsoluteValidPath(const wxString& filename);
    // Given full path and filename, add path to list
    void EnsureFileAccessible(const wxString& path);
    // Returns true if the path is in the list
    bool Member(const wxString& path);

private:
    // DECLARE_DYNAMIC_CLASS(wxPathList)
};

#endif
  // _WX_FILEFN_H_
