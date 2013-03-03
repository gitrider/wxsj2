///////////////////////////////////////////////////////////////////////////////
// Name:        wx/wince/chkconf.h
// Purpose:     WinCE-specific configuration options checks
// Author:      Vadim Zeitlin
// Modified by:
// Created:     2005-03-07
// RCS-ID:      $Id: chkconf.h,v 1.6 2005/05/23 11:10:17 JS Exp $
// Copyright:   (c) 2005 Vadim Zeitlin <vadim@wxwindows.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_MSW_WINCE_CHKCONF_H_
#define _WX_MSW_WINCE_CHKCONF_H_

// Standard SDK lacks a few things, forcefully disable them
#ifdef WCE_PLATFORM_STANDARDSDK
    // no shell functions support
    #undef wxUSE_STDPATHS
    #define wxUSE_STDPATHS 0
#endif // WCE_PLATFORM_STANDARDSDK

// DDE doesn't exist under WinCE and wxIPC is DDE-based under MSW
#undef wxUSE_IPC
#define wxUSE_IPC 0

// metafiles are not supported neither
#undef wxUSE_ENH_METAFILE
#define wxUSE_ENH_METAFILE 0

#undef wxUSE_METAFILE
#define wxUSE_METAFILE 0

// eVC doesn't support SEH
#undef wxUSE_ON_FATAL_EXCEPTION
#define wxUSE_ON_FATAL_EXCEPTION 0

// media stuff not supported under CE
#undef wxUSE_MEDIACTRL
#define wxUSE_MEDIACTRL 0

#undef wxUSE_WXHTML_HELP
#define wxUSE_WXHTML_HELP 0

// libtiff and regex apparently don't compile with eVC (to check with eVC4?)
// and they're disabled for WinCE in build/bakefiles/{tiff|regex}.bkl so can't
// be enabled here
#undef wxUSE_LIBTIFF
#define wxUSE_LIBTIFF 0

#undef wxUSE_REGEX
#define wxUSE_REGEX 0

#undef wxUSE_DEBUGREPORT
#define wxUSE_DEBUGREPORT 0

// Disable features which don't make sense for MS Smartphones
// (due to pointer device usage, limited controls, file system)
#if defined(__SMARTPHONE__)
    #undef wxUSE_LISTBOOK
    #define wxUSE_LISTBOOK 0

    #undef wxUSE_NOTEBOOK
    #define wxUSE_NOTEBOOK 0

    #undef wxUSE_STATUSBAR
    #define wxUSE_STATUSBAR 0
#endif // __SMARTPHONE__

#endif // _WX_MSW_WINCE_CHKCONF_H_

