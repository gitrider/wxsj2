///////////////////////////////////////////////////////////////////////////////
// Name:        tests/filename/filename.cpp
// Purpose:     wxFileName unit test
// Author:      Vadim Zeitlin
// Created:     2004-07-25
// RCS-ID:      $Id: filenametest.cpp,v 1.8 2005/05/23 16:53:30 ABX Exp $
// Copyright:   (c) 2004 Vadim Zeitlin
///////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include "testprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
#endif // WX_PRECOMP

#include "wx/filename.h"
#include "wx/filefn.h"

// ----------------------------------------------------------------------------
// test data
// ----------------------------------------------------------------------------

static struct FileNameInfo
{
    const wxChar *fullname;
    const wxChar *volume;
    const wxChar *path;
    const wxChar *name;
    const wxChar *ext;
    bool isAbsolute;
    wxPathFormat format;
} filenames[] =
{
    // Unix file names
    { _T("/usr/bin/ls"), _T(""), _T("/usr/bin"), _T("ls"), _T(""), true, wxPATH_UNIX },
    { _T("/usr/bin/"), _T(""), _T("/usr/bin"), _T(""), _T(""), true, wxPATH_UNIX },
    { _T("~/.zshrc"), _T(""), _T("~"), _T(".zshrc"), _T(""), true, wxPATH_UNIX },
    { _T("../../foo"), _T(""), _T("../.."), _T("foo"), _T(""), false, wxPATH_UNIX },
    { _T("foo.bar"), _T(""), _T(""), _T("foo"), _T("bar"), false, wxPATH_UNIX },
    { _T("~/foo.bar"), _T(""), _T("~"), _T("foo"), _T("bar"), true, wxPATH_UNIX },
    { _T("/foo"), _T(""), _T("/"), _T("foo"), _T(""), true, wxPATH_UNIX },
    { _T("Mahogany-0.60/foo.bar"), _T(""), _T("Mahogany-0.60"), _T("foo"), _T("bar"), false, wxPATH_UNIX },
    { _T("/tmp/wxwin.tar.bz"), _T(""), _T("/tmp"), _T("wxwin.tar"), _T("bz"), true, wxPATH_UNIX },

    // Windows file names
    { _T("foo.bar"), _T(""), _T(""), _T("foo"), _T("bar"), false, wxPATH_DOS },
    { _T("\\foo.bar"), _T(""), _T("\\"), _T("foo"), _T("bar"), false, wxPATH_DOS },
    { _T("c:foo.bar"), _T("c"), _T(""), _T("foo"), _T("bar"), false, wxPATH_DOS },
    { _T("c:\\foo.bar"), _T("c"), _T("\\"), _T("foo"), _T("bar"), true, wxPATH_DOS },
    { _T("c:\\Windows\\command.com"), _T("c"), _T("\\Windows"), _T("command"), _T("com"), true, wxPATH_DOS },
    { _T("\\\\server\\foo.bar"), _T("server"), _T("\\"), _T("foo"), _T("bar"), true, wxPATH_DOS },
    { _T("\\\\server\\dir\\foo.bar"), _T("server"), _T("\\dir"), _T("foo"), _T("bar"), true, wxPATH_DOS },

    // wxFileName support for Mac file names is broken currently
#if 0
    // Mac file names
    { _T("Volume:Dir:File"), _T("Volume"), _T("Dir"), _T("File"), _T(""), true, wxPATH_MAC },
    { _T("Volume:Dir:Subdir:File"), _T("Volume"), _T("Dir:Subdir"), _T("File"), _T(""), true, wxPATH_MAC },
    { _T("Volume:"), _T("Volume"), _T(""), _T(""), _T(""), true, wxPATH_MAC },
    { _T(":Dir:File"), _T(""), _T("Dir"), _T("File"), _T(""), false, wxPATH_MAC },
    { _T(":File.Ext"), _T(""), _T(""), _T("File"), _T(".Ext"), false, wxPATH_MAC },
    { _T("File.Ext"), _T(""), _T(""), _T("File"), _T(".Ext"), false, wxPATH_MAC },
#endif // 0

    // VMS file names
    { _T("device:[dir1.dir2.dir3]file.txt"), _T("device"), _T("dir1.dir2.dir3"), _T("file"), _T("txt"), true, wxPATH_VMS },
    { _T("file.txt"), _T(""), _T(""), _T("file"), _T("txt"), false, wxPATH_VMS },
};

// ----------------------------------------------------------------------------
// test class
// ----------------------------------------------------------------------------

class FileNameTestCase : public CppUnit::TestCase
{
public:
    FileNameTestCase() { }

private:
    CPPUNIT_TEST_SUITE( FileNameTestCase );
        CPPUNIT_TEST( TestConstruction );
        CPPUNIT_TEST( TestSplit );
        CPPUNIT_TEST( TestSetPath );
        CPPUNIT_TEST( TestStrip );
#ifdef __WINDOWS__
        CPPUNIT_TEST( TestShortLongPath );
#endif // __WINDOWS__
    CPPUNIT_TEST_SUITE_END();

    void TestConstruction();
    void TestSplit();
    void TestSetPath();
    void TestStrip();
#ifdef __WINDOWS__
    void TestShortLongPath();
#endif // __WINDOWS__

    DECLARE_NO_COPY_CLASS(FileNameTestCase)
};

// register in the unnamed registry so that these tests are run by default
CPPUNIT_TEST_SUITE_REGISTRATION( FileNameTestCase );

// also include in it's own registry so that these tests can be run alone
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( FileNameTestCase, "FileNameTestCase" );

void FileNameTestCase::TestConstruction()
{
    for ( size_t n = 0; n < WXSIZEOF(filenames); n++ )
    {
        const FileNameInfo& fni = filenames[n];

        wxFileName fn(fni.fullname, fni.format);

        wxString fullname = fn.GetFullPath(fni.format);
        CPPUNIT_ASSERT( fullname == fni.fullname );

        CPPUNIT_ASSERT( fn.Normalize(wxPATH_NORM_ALL, _T(""), fni.format) );

        if ( *fni.volume && *fni.path )
        {
            // check that specifying the volume separately or as part of the
            // path doesn't make any difference
            wxString pathWithVolume = fni.volume;
            pathWithVolume += wxFileName::GetVolumeSeparator(fni.format);
            pathWithVolume += fni.path;

            CPPUNIT_ASSERT( fn == wxFileName(pathWithVolume,
                                             fni.name,
                                             fni.ext,
                                             fni.format) );
        }
    }
}

void FileNameTestCase::TestSplit()
{
    for ( size_t n = 0; n < WXSIZEOF(filenames); n++ )
    {
        const FileNameInfo& fni = filenames[n];
        wxString volume, path, name, ext;
        wxFileName::SplitPath(fni.fullname,
                              &volume, &path, &name, &ext, fni.format);

        CPPUNIT_ASSERT( volume == fni.volume );
        CPPUNIT_ASSERT( path == fni.path );
        CPPUNIT_ASSERT( name == fni.name );
        CPPUNIT_ASSERT( ext == fni.ext );
    }

    // special case of empty extension
    wxFileName fn(_T("foo."));
    CPPUNIT_ASSERT( fn.GetFullPath() == _T("foo.") );
}

void FileNameTestCase::TestSetPath()
{
    wxFileName fn(_T("d:\\test\\foo.bar"), wxPATH_DOS);
    fn.SetPath(_T("c:\\temp"), wxPATH_DOS);
    CPPUNIT_ASSERT( fn.SameAs(wxFileName(_T("c:\\temp\\foo.bar"), wxPATH_DOS)) );

    fn = wxFileName(_T("/usr/bin/ls"), wxPATH_UNIX);
    fn.SetPath(_T("/usr/local/bin"), wxPATH_UNIX);
    CPPUNIT_ASSERT( fn.SameAs(wxFileName(_T("/usr/local/bin/ls"), wxPATH_UNIX)) );
}

wxString wxTestStripExtension(wxString szFile)
{
    wxStripExtension(szFile);
    return szFile;
}

void FileNameTestCase::TestStrip()
{
    //test a crash
    CPPUNIT_ASSERT( wxTestStripExtension( _T("") ) == _T("") );

    //others
    CPPUNIT_ASSERT( wxTestStripExtension( _T(".") ) == _T("") );
    CPPUNIT_ASSERT( wxTestStripExtension( _T(".wav") ) == _T("") );
    CPPUNIT_ASSERT( wxTestStripExtension( _T("good.wav") ) == _T("good") );
    CPPUNIT_ASSERT( wxTestStripExtension( _T("good.wav.wav") ) == _T("good.wav") );
}

#ifdef __WINDOWS__

void FileNameTestCase::TestShortLongPath()
{
    wxFileName fn(_T("C:\\Program Files\\Windows NT\\Accessories\\wordpad.exe"));

    // incredibly enough, GetLongPath() used to return different results during
    // the first and subsequent runs, test for this
    CPPUNIT_ASSERT_EQUAL( fn.GetLongPath(), fn.GetLongPath() );
    CPPUNIT_ASSERT_EQUAL( fn.GetShortPath(), fn.GetShortPath() );
}

#endif // __WINDOWS__
