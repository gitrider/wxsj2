/////////////////////////////////////////////////////////////////////////////
// Name:        app.cpp
// Purpose:     wxApp
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id: app.cpp,v 1.232 2005/04/14 16:04:47 JS Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ===========================================================================
// declarations
// ===========================================================================

// ---------------------------------------------------------------------------
// headers
// ---------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "app.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#if defined(__BORLANDC__)
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/frame.h"
    #include "wx/app.h"
    #include "wx/utils.h"
    #include "wx/gdicmn.h"
    #include "wx/pen.h"
    #include "wx/brush.h"
    #include "wx/cursor.h"
    #include "wx/icon.h"
    #include "wx/palette.h"
    #include "wx/dc.h"
    #include "wx/dialog.h"
    #include "wx/msgdlg.h"
    #include "wx/intl.h"
    #include "wx/dynarray.h"
    #include "wx/wxchar.h"
    #include "wx/icon.h"
    #include "wx/log.h"
#endif

#include "wx/apptrait.h"
#include "wx/filename.h"
#include "wx/module.h"
#include "wx/dynlib.h"

#include "wx/msw/private.h"
#include "wx/msw/ole/oleutils.h"

#if wxUSE_TOOLTIPS
    #include "wx/tooltip.h"
#endif // wxUSE_TOOLTIPS

// OLE is used for drag-and-drop, clipboard, OLE Automation..., but some
// compilers don't support it (missing headers, libs, ...)
#if defined(__GNUWIN32_OLD__) || defined(__SYMANTEC__) || defined(__SALFORDC__)
    #undef wxUSE_OLE

    #define  wxUSE_OLE 0
#endif // broken compilers

#if defined(__POCKETPC__) || defined(__SMARTPHONE__)
#include <aygshell.h>
#endif

#if wxUSE_OLE
    #include <ole2.h>
#endif

#include <string.h>
#include <ctype.h>

#include "wx/msw/wrapcctl.h"

// For MB_TASKMODAL
#ifdef __WXWINCE__
#include "wx/msw/wince/missing.h"
#endif

// For DLLVER_PLATFORM_WINDOWS
#if (!defined(__MINGW32__) || wxCHECK_W32API_VERSION( 2, 0 )) && \
    !defined(__CYGWIN__) && !defined(__DIGITALMARS__) && !defined(__WXWINCE__) && \
    (!defined(_MSC_VER) || (_MSC_VER > 1100))
    #include <shlwapi.h>
#endif

// ---------------------------------------------------------------------------
// global variables
// ---------------------------------------------------------------------------

extern wxList WXDLLEXPORT wxPendingDelete;

#if !defined(__WXMICROWIN__) && !defined(__WXWINCE__)
extern void wxSetKeyboardHook(bool doIt);
#endif

// NB: all "NoRedraw" classes must have the same names as the "normal" classes
//     with NR suffix - wxWindow::MSWCreate() supposes this
#ifdef __WXWINCE__
WXDLLIMPEXP_CORE       wxChar *wxCanvasClassName;
WXDLLIMPEXP_CORE       wxChar *wxCanvasClassNameNR;
#else
WXDLLIMPEXP_CORE const wxChar *wxCanvasClassName        = wxT("wxWindowClass");
WXDLLIMPEXP_CORE const wxChar *wxCanvasClassNameNR      = wxT("wxWindowClassNR");
#endif
WXDLLIMPEXP_CORE const wxChar *wxMDIFrameClassName      = wxT("wxMDIFrameClass");
WXDLLIMPEXP_CORE const wxChar *wxMDIFrameClassNameNoRedraw = wxT("wxMDIFrameClassNR");
WXDLLIMPEXP_CORE const wxChar *wxMDIChildFrameClassName = wxT("wxMDIChildFrameClass");
WXDLLIMPEXP_CORE const wxChar *wxMDIChildFrameClassNameNoRedraw = wxT("wxMDIChildFrameClassNR");

// ----------------------------------------------------------------------------
// private functions
// ----------------------------------------------------------------------------

LRESULT WXDLLEXPORT APIENTRY wxWndProc(HWND, UINT, WPARAM, LPARAM);

// ===========================================================================
// wxGUIAppTraits implementation
// ===========================================================================

// private class which we use to pass parameters from BeforeChildWaitLoop() to
// AfterChildWaitLoop()
struct ChildWaitLoopData
{
    ChildWaitLoopData(wxWindowDisabler *wd_, wxWindow *winActive_)
    {
        wd = wd_;
        winActive = winActive_;
    }

    wxWindowDisabler *wd;
    wxWindow *winActive;
};

void *wxGUIAppTraits::BeforeChildWaitLoop()
{
    /*
       We use a dirty hack here to disable all application windows (which we
       must do because otherwise the calls to wxYield() could lead to some very
       unexpected reentrancies in the users code) but to avoid losing
       focus/activation entirely when the child process terminates which would
       happen if we simply disabled everything using wxWindowDisabler. Indeed,
       remember that Windows will never activate a disabled window and when the
       last childs window is closed and Windows looks for a window to activate
       all our windows are still disabled. There is no way to enable them in
       time because we don't know when the childs windows are going to be
       closed, so the solution we use here is to keep one special tiny frame
       enabled all the time. Then when the child terminates it will get
       activated and when we close it below -- after reenabling all the other
       windows! -- the previously active window becomes activated again and
       everything is ok.
     */
    wxBeginBusyCursor();

    // first disable all existing windows
    wxWindowDisabler *wd = new wxWindowDisabler;

    // then create an "invisible" frame: it has minimal size, is positioned
    // (hopefully) outside the screen and doesn't appear on the taskbar
    wxWindow *winActive = new wxFrame
                    (
                        wxTheApp->GetTopWindow(),
                        wxID_ANY,
                        wxEmptyString,
                        wxPoint(32600, 32600),
                        wxSize(1, 1),
                        wxDEFAULT_FRAME_STYLE | wxFRAME_NO_TASKBAR
                    );
    winActive->Show();

    return new ChildWaitLoopData(wd, winActive);
}

void wxGUIAppTraits::AlwaysYield()
{
    wxYield();
}

void wxGUIAppTraits::AfterChildWaitLoop(void *dataOrig)
{
    wxEndBusyCursor();

    ChildWaitLoopData * const data = (ChildWaitLoopData *)dataOrig;

    delete data->wd;

    // finally delete the dummy frame and, as wd has been already destroyed and
    // the other windows reenabled, the activation is going to return to the
    // window which had had it before
    data->winActive->Destroy();

    // also delete the temporary data object itself
    delete data;
}

bool wxGUIAppTraits::DoMessageFromThreadWait()
{
    // we should return false only if the app should exit, i.e. only if
    // Dispatch() determines that the main event loop should terminate
    return !wxTheApp || wxTheApp->Dispatch();
}

wxToolkitInfo& wxGUIAppTraits::GetToolkitInfo()
{
    static wxToolkitInfo info;
    wxToolkitInfo& baseInfo = wxAppTraits::GetToolkitInfo();
    info.versionMajor = baseInfo.versionMajor;
    info.versionMinor = baseInfo.versionMinor;
    info.os = baseInfo.os;
    info.shortName = _T("msw");
    info.name = _T("wxMSW");
#ifdef __WXUNIVERSAL__
    info.shortName << _T("univ");
    info.name << _T("/wxUniversal");
#endif
    return info;
}

// ===========================================================================
// wxApp implementation
// ===========================================================================

int wxApp::m_nCmdShow = SW_SHOWNORMAL;

// ---------------------------------------------------------------------------
// wxWin macros
// ---------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxApp, wxEvtHandler)

BEGIN_EVENT_TABLE(wxApp, wxEvtHandler)
    EVT_IDLE(wxApp::OnIdle)
    EVT_END_SESSION(wxApp::OnEndSession)
    EVT_QUERY_END_SESSION(wxApp::OnQueryEndSession)
END_EVENT_TABLE()

// class to ensure that wxAppBase::CleanUp() is called if our Initialize()
// fails
class wxCallBaseCleanup
{
public:
    wxCallBaseCleanup(wxApp *app) : m_app(app) { }
    ~wxCallBaseCleanup() { if ( m_app ) m_app->wxAppBase::CleanUp(); }

    void Dismiss() { m_app = NULL; }

private:
    wxApp *m_app;
};

//// Initialize
bool wxApp::Initialize(int& argc, wxChar **argv)
{
    if ( !wxAppBase::Initialize(argc, argv) )
        return false;

    // ensure that base cleanup is done if we return too early
    wxCallBaseCleanup callBaseCleanup(this);

#ifdef __WXWINCE__
    wxString tmp = GetAppName();
    tmp += wxT("ClassName");
    wxCanvasClassName = wxStrdup( tmp.c_str() );
    tmp += wxT("NR");
    wxCanvasClassNameNR = wxStrdup( tmp.c_str() );
    HWND hWnd = FindWindow( wxCanvasClassNameNR, NULL );
    if (hWnd)
    {
        SetForegroundWindow( (HWND)(((DWORD)hWnd)|0x01) );
        return false;
    }
#endif

    // the first thing to do is to check if we're trying to run an Unicode
    // program under Win9x w/o MSLU emulation layer - if so, abort right now
    // as it has no chance to work
#if wxUSE_UNICODE && !wxUSE_UNICODE_MSLU
    if ( wxGetOsVersion() != wxWINDOWS_NT && wxGetOsVersion() != wxWINDOWS_CE && wxGetOsVersion() != wxWINDOWS_SMARTPHONE && wxGetOsVersion() != wxWINDOWS_POCKETPC )
    {
        // note that we can use MessageBoxW() as it's implemented even under
        // Win9x - OTOH, we can't use wxGetTranslation() because the file APIs
        // used by wxLocale are not
        ::MessageBox
        (
         NULL,
         _T("This program uses Unicode and requires Windows NT/2000/XP/CE.\nProgram aborted."),
         _T("wxWidgets Fatal Error"),
         MB_ICONERROR | MB_OK
        );

        return false;
    }
#endif // wxUSE_UNICODE && !wxUSE_UNICODE_MSLU

#if defined(__WIN95__) && !defined(__WXMICROWIN__)
    InitCommonControls();
#endif // __WIN95__

#if defined(__SMARTPHONE__) || defined(__POCKETPC__)
    SHInitExtraControls();
#endif

    wxOleInitialize();

    RegisterWindowClasses();

#if wxUSE_PENWINDOWS
    wxRegisterPenWin();
#endif

    wxWinHandleHash = new wxWinHashTable(wxKEY_INTEGER, 100);

#if !defined(__WXMICROWIN__) && !defined(__WXWINCE__)
    wxSetKeyboardHook(true);
#endif

    callBaseCleanup.Dismiss();

    return true;
}

// ---------------------------------------------------------------------------
// RegisterWindowClasses
// ---------------------------------------------------------------------------

// TODO we should only register classes really used by the app. For this it
//      would be enough to just delay the class registration until an attempt
//      to create a window of this class is made.
bool wxApp::RegisterWindowClasses()
{
    WNDCLASS wndclass;
    wxZeroMemory(wndclass);

    // for each class we register one with CS_(V|H)REDRAW style and one
    // without for windows created with wxNO_FULL_REDRAW_ON_REPAINT flag
    static const long styleNormal = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    static const long styleNoRedraw = CS_DBLCLKS;

    // the fields which are common to all classes
    wndclass.lpfnWndProc   = (WNDPROC)wxWndProc;
    wndclass.hInstance     = wxhInstance;
    wndclass.hCursor       = ::LoadCursor((HINSTANCE)NULL, IDC_ARROW);

    // register the class for all normal windows
    wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    wndclass.lpszClassName = wxCanvasClassName;
    wndclass.style         = styleNormal;

    if ( !RegisterClass(&wndclass) )
    {
        wxLogLastError(wxT("RegisterClass(frame)"));
    }

    // "no redraw" frame
    wndclass.lpszClassName = wxCanvasClassNameNR;
    wndclass.style         = styleNoRedraw;

    if ( !RegisterClass(&wndclass) )
    {
        wxLogLastError(wxT("RegisterClass(no redraw frame)"));
    }

    // Register the MDI frame window class.
    wndclass.hbrBackground = (HBRUSH)NULL; // paint MDI frame ourselves
    wndclass.lpszClassName = wxMDIFrameClassName;
    wndclass.style         = styleNormal;

    if ( !RegisterClass(&wndclass) )
    {
        wxLogLastError(wxT("RegisterClass(MDI parent)"));
    }

    // "no redraw" MDI frame
    wndclass.lpszClassName = wxMDIFrameClassNameNoRedraw;
    wndclass.style         = styleNoRedraw;

    if ( !RegisterClass(&wndclass) )
    {
        wxLogLastError(wxT("RegisterClass(no redraw MDI parent frame)"));
    }

    // Register the MDI child frame window class.
    wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wndclass.lpszClassName = wxMDIChildFrameClassName;
    wndclass.style         = styleNormal;

    if ( !RegisterClass(&wndclass) )
    {
        wxLogLastError(wxT("RegisterClass(MDI child)"));
    }

    // "no redraw" MDI child frame
    wndclass.lpszClassName = wxMDIChildFrameClassNameNoRedraw;
    wndclass.style         = styleNoRedraw;

    if ( !RegisterClass(&wndclass) )
    {
        wxLogLastError(wxT("RegisterClass(no redraw MDI child)"));
    }

    return true;
}

// ---------------------------------------------------------------------------
// UnregisterWindowClasses
// ---------------------------------------------------------------------------

bool wxApp::UnregisterWindowClasses()
{
    bool retval = true;

#ifndef __WXMICROWIN__
    // MDI frame window class.
    if ( !::UnregisterClass(wxMDIFrameClassName, wxhInstance) )
    {
        wxLogLastError(wxT("UnregisterClass(MDI parent)"));

        retval = false;
    }

    // "no redraw" MDI frame
    if ( !::UnregisterClass(wxMDIFrameClassNameNoRedraw, wxhInstance) )
    {
        wxLogLastError(wxT("UnregisterClass(no redraw MDI parent frame)"));

        retval = false;
    }

    // MDI child frame window class.
    if ( !::UnregisterClass(wxMDIChildFrameClassName, wxhInstance) )
    {
        wxLogLastError(wxT("UnregisterClass(MDI child)"));

        retval = false;
    }

    // "no redraw" MDI child frame
    if ( !::UnregisterClass(wxMDIChildFrameClassNameNoRedraw, wxhInstance) )
    {
        wxLogLastError(wxT("UnregisterClass(no redraw MDI child)"));

        retval = false;
    }

    // canvas class name
    if ( !::UnregisterClass(wxCanvasClassName, wxhInstance) )
    {
        wxLogLastError(wxT("UnregisterClass(canvas)"));

        retval = false;
    }

    if ( !::UnregisterClass(wxCanvasClassNameNR, wxhInstance) )
    {
        wxLogLastError(wxT("UnregisterClass(no redraw canvas)"));

        retval = false;
    }
#endif // __WXMICROWIN__

    return retval;
}

void wxApp::CleanUp()
{
    // all objects pending for deletion must be deleted first, otherwise we
    // would crash when they use wxWinHandleHash (and UnregisterWindowClasses()
    // call wouldn't succeed as long as any windows still exist), so call the
    // base class method first and only then do our clean up
    wxAppBase::CleanUp();

#if !defined(__WXMICROWIN__) && !defined(__WXWINCE__)
    wxSetKeyboardHook(false);
#endif

#if wxUSE_PENWINDOWS
    wxCleanUpPenWin();
#endif

    wxOleUninitialize();

    // for an EXE the classes are unregistered when it terminates but DLL may
    // be loaded several times (load/unload/load) into the same process in
    // which case the registration will fail after the first time if we don't
    // unregister the classes now
    UnregisterWindowClasses();

    delete wxWinHandleHash;
    wxWinHandleHash = NULL;
    
#ifdef __WXWINCE__
    free( wxCanvasClassName );
    free( wxCanvasClassNameNR );
#endif
}

// ----------------------------------------------------------------------------
// wxApp ctor/dtor
// ----------------------------------------------------------------------------

wxApp::wxApp()
{
    m_printMode = wxPRINT_WINDOWS;
}

wxApp::~wxApp()
{
    // our cmd line arguments are allocated inside wxEntry(HINSTANCE), they
    // don't come from main(), so we have to free them

    while ( argc )
    {
        // m_argv elements were allocated by wxStrdup()
        free(argv[--argc]);
    }

    // but m_argv itself -- using new[]
    delete [] argv;
}

// ----------------------------------------------------------------------------
// wxApp idle handling
// ----------------------------------------------------------------------------

void wxApp::OnIdle(wxIdleEvent& event)
{
    wxAppBase::OnIdle(event);

#if wxUSE_DC_CACHEING
    // automated DC cache management: clear the cached DCs and bitmap
    // if it's likely that the app has finished with them, that is, we
    // get an idle event and we're not dragging anything.
    if (!::GetKeyState(MK_LBUTTON) && !::GetKeyState(MK_MBUTTON) && !::GetKeyState(MK_RBUTTON))
        wxDC::ClearCache();
#endif // wxUSE_DC_CACHEING
}

void wxApp::WakeUpIdle()
{
    // Send the top window a dummy message so idle handler processing will
    // start up again.  Doing it this way ensures that the idle handler
    // wakes up in the right thread (see also wxWakeUpMainThread() which does
    // the same for the main app thread only)
    wxWindow *topWindow = wxTheApp->GetTopWindow();
    if ( topWindow )
    {
        if ( !::PostMessage(GetHwndOf(topWindow), WM_NULL, 0, 0) )
        {
            // should never happen
            wxLogLastError(wxT("PostMessage(WM_NULL)"));
        }
    }
}

// ----------------------------------------------------------------------------
// other wxApp event hanlders
// ----------------------------------------------------------------------------

void wxApp::OnEndSession(wxCloseEvent& WXUNUSED(event))
{
    if (GetTopWindow())
        GetTopWindow()->Close(true);
}

// Default behaviour: close the application with prompts. The
// user can veto the close, and therefore the end session.
void wxApp::OnQueryEndSession(wxCloseEvent& event)
{
    if (GetTopWindow())
    {
        if (!GetTopWindow()->Close(!event.CanVeto()))
            event.Veto(true);
    }
}

// ----------------------------------------------------------------------------
// miscellaneous
// ----------------------------------------------------------------------------

/* static */
int wxApp::GetComCtl32Version()
{
#if defined(__WXMICROWIN__) || defined(__WXWINCE__)
    return 0;
#else
    // cache the result
    //
    // NB: this is MT-ok as in the worst case we'd compute s_verComCtl32 twice,
    //     but as its value should be the same both times it doesn't matter
    static int s_verComCtl32 = -1;

    if ( s_verComCtl32 == -1 )
    {
        // initally assume no comctl32.dll at all
        s_verComCtl32 = 0;

        // we're prepared to handle the errors
        wxLogNull noLog;

        // do we have it?
        wxDynamicLibrary dllComCtl32(_T("comctl32.dll"), wxDL_VERBATIM);

        // if so, then we can check for the version
        if ( dllComCtl32.IsLoaded() )
        {
#ifndef DLLVER_PLATFORM_WINDOWS
			typedef struct _DllVersionInfo
			{
				DWORD cbSize;
				DWORD dwMajorVersion;                   // Major version
				DWORD dwMinorVersion;                   // Minor version
				DWORD dwBuildNumber;                    // Build number
				DWORD dwPlatformID;                     // DLLVER_PLATFORM_*
			} DLLVERSIONINFO;
			typedef HRESULT (CALLBACK* DLLGETVERSIONPROC)(DLLVERSIONINFO *);
#endif
            // try to use DllGetVersion() if available in _headers_
            wxDYNLIB_FUNCTION( DLLGETVERSIONPROC, DllGetVersion, dllComCtl32 );
            if ( pfnDllGetVersion )
            {
                DLLVERSIONINFO dvi;
                dvi.cbSize = sizeof(dvi);

                HRESULT hr = (*pfnDllGetVersion)(&dvi);
                if ( FAILED(hr) )
                {
                    wxLogApiError(_T("DllGetVersion"), hr);
                }
                else
                {
                    // this is incompatible with _WIN32_IE values, but
                    // compatible with the other values returned by
                    // GetComCtl32Version()
                    s_verComCtl32 = 100*dvi.dwMajorVersion +
                                        dvi.dwMinorVersion;
                }
            }

            // if DllGetVersion() is unavailable either during compile or
            // run-time, try to guess the version otherwise
            if ( !s_verComCtl32 )
            {
                // InitCommonControlsEx is unique to 4.70 and later
                void *pfn = dllComCtl32.GetSymbol(_T("InitCommonControlsEx"));
                if ( !pfn )
                {
                    // not found, must be 4.00
                    s_verComCtl32 = 400;
                }
                else // 4.70+
                {
                    // many symbols appeared in comctl32 4.71, could use any of
                    // them except may be DllInstall()
                    pfn = dllComCtl32.GetSymbol(_T("InitializeFlatSB"));
                    if ( !pfn )
                    {
                        // not found, must be 4.70
                        s_verComCtl32 = 470;
                    }
                    else
                    {
                        // found, must be 4.71 or later
                        s_verComCtl32 = 471;
                    }
                }
            }
        }
    }

    return s_verComCtl32;
#endif // Microwin/!Microwin
}

// Yield to incoming messages

bool wxApp::Yield(bool onlyIfNeeded)
{
    // MT-FIXME
    static bool s_inYield = false;

#if wxUSE_LOG
    // disable log flushing from here because a call to wxYield() shouldn't
    // normally result in message boxes popping up &c
    wxLog::Suspend();
#endif // wxUSE_LOG

    if ( s_inYield )
    {
        if ( !onlyIfNeeded )
        {
            wxFAIL_MSG( wxT("wxYield called recursively" ) );
        }

        return false;
    }

    s_inYield = true;

    // we don't want to process WM_QUIT from here - it should be processed in
    // the main event loop in order to stop it
    MSG msg;
    while ( PeekMessage(&msg, (HWND)0, 0, 0, PM_NOREMOVE) &&
            msg.message != WM_QUIT )
    {
#if wxUSE_THREADS
        wxMutexGuiLeaveOrEnter();
#endif // wxUSE_THREADS

        if ( !wxTheApp->Dispatch() )
            break;
    }

    // if there are pending events, we must process them.
    ProcessPendingEvents();

#if wxUSE_LOG
    // let the logs be flashed again
    wxLog::Resume();
#endif // wxUSE_LOG

    s_inYield = false;

    return true;
}

#if wxUSE_EXCEPTIONS

// ----------------------------------------------------------------------------
// exception handling
// ----------------------------------------------------------------------------

bool wxApp::OnExceptionInMainLoop()
{
    // ask the user about what to do: use the Win32 API function here as it
    // could be dangerous to use any wxWidgets code in this state
    switch (
            ::MessageBox
              (
                NULL,
                _T("An unhandled exception occurred. Press \"Abort\" to \
terminate the program,\r\n\
\"Retry\" to exit the program normally and \"Ignore\" to try to continue."),
                _T("Unhandled exception"),
                MB_ABORTRETRYIGNORE |
                MB_ICONERROR| 
                MB_TASKMODAL
              )
           )
    {
        case IDABORT:
            throw;

        default:
            wxFAIL_MSG( _T("unexpected MessageBox() return code") );
            // fall through

        case IDRETRY:
            return false;

        case IDIGNORE:
            return true;
    }
}

#endif // wxUSE_EXCEPTIONS

// ----------------------------------------------------------------------------
// deprecated event loop functions
// ----------------------------------------------------------------------------

#if WXWIN_COMPATIBILITY_2_4

#include "wx/evtloop.h"

void wxApp::DoMessage(WXMSG *pMsg)
{
    wxEventLoop *evtLoop = wxEventLoop::GetActive();
    if ( evtLoop )
        evtLoop->ProcessMessage(pMsg);
}

bool wxApp::DoMessage()
{
    wxEventLoop *evtLoop = wxEventLoop::GetActive();
    return evtLoop ? evtLoop->Dispatch() : false;
}

bool wxApp::ProcessMessage(WXMSG* pMsg)
{
    wxEventLoop *evtLoop = wxEventLoop::GetActive();
    return evtLoop && evtLoop->PreProcessMessage(pMsg);
}

#endif // WXWIN_COMPATIBILITY_2_4

