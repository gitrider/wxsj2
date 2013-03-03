/////////////////////////////////////////////////////////////////////////////
// Name:        common/appcmn.cpp
// Purpose:     wxAppConsole and wxAppBase methods common to all platforms
// Author:      Vadim Zeitlin
// Modified by:
// Created:     18.10.99
// RCS-ID:      $Id: appcmn.cpp,v 1.103 2005/04/12 17:52:01 RN Exp $
// Copyright:   (c) Vadim Zeitlin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ---------------------------------------------------------------------------
// headers
// ---------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "appbase.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#if defined(__BORLANDC__)
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/app.h"
    #include "wx/bitmap.h"
    #include "wx/intl.h"
    #include "wx/list.h"
    #include "wx/log.h"
    #include "wx/msgdlg.h"
    #include "wx/bitmap.h"
    #include "wx/confbase.h"
#endif

#include "wx/apptrait.h"
#include "wx/cmdline.h"
#include "wx/evtloop.h"
#include "wx/msgout.h"
#include "wx/thread.h"
#include "wx/utils.h"
#include "wx/ptr_scpd.h"

#if defined(__WXMSW__)
    #include  "wx/msw/private.h"  // includes windows.h for LOGFONT
#endif

#if wxUSE_FONTMAP
    #include "wx/fontmap.h"
#endif // wxUSE_FONTMAP

// DLL options compatibility check:
#include "wx/build.h"
WX_CHECK_BUILD_OPTIONS("wxCore")


// ----------------------------------------------------------------------------
// wxEventLoopPtr
// ----------------------------------------------------------------------------

// this defines wxEventLoopPtr
wxDEFINE_TIED_SCOPED_PTR_TYPE(wxEventLoop);

// ============================================================================
// wxAppBase implementation
// ============================================================================

// ----------------------------------------------------------------------------
// initialization
// ----------------------------------------------------------------------------

wxAppBase::wxAppBase()
{
    m_topWindow = (wxWindow *)NULL;
    m_useBestVisual = false;
    m_isActive = true;

#if wxUSE_EVTLOOP_IN_APP
    m_mainLoop = NULL;
#endif // wxUSE_EVTLOOP_IN_APP

    // We don't want to exit the app if the user code shows a dialog from its
    // OnInit() -- but this is what would happen if we set m_exitOnFrameDelete
    // to Yes initially as this dialog would be the last top level window.
    // OTOH, if we set it to No initially we'll have to overwrite it with Yes
    // when we enter our OnRun() because we do want the default behaviour from
    // then on. But this would be a problem if the user code calls
    // SetExitOnFrameDelete(false) from OnInit().
    //
    // So we use the special "Later" value which is such that
    // GetExitOnFrameDelete() returns false for it but which we know we can
    // safely (i.e. without losing the effect of the users SetExitOnFrameDelete
    // call) overwrite in OnRun()
    m_exitOnFrameDelete = Later;
}

bool wxAppBase::Initialize(int& argc, wxChar **argv)
{
    if ( !wxAppConsole::Initialize(argc, argv) )
        return false;

#if wxUSE_THREADS
    wxPendingEventsLocker = new wxCriticalSection;
#endif

    wxInitializeStockLists();
    wxInitializeStockObjects();

    wxBitmap::InitStandardHandlers();

    return true;
}

// ----------------------------------------------------------------------------
// cleanup
// ----------------------------------------------------------------------------

wxAppBase::~wxAppBase()
{
    // this destructor is required for Darwin
}

void wxAppBase::CleanUp()
{
    // clean up all the pending objects
    DeletePendingObjects();

    // and any remaining TLWs (they remove themselves from wxTopLevelWindows
    // when destroyed, so iterate until none are left)
    while ( !wxTopLevelWindows.empty() )
    {
        // do not use Destroy() here as it only puts the TLW in pending list
        // but we want to delete them now
        delete wxTopLevelWindows.GetFirst()->GetData();
    }

    // undo everything we did in Initialize() above
    wxBitmap::CleanUpHandlers();

    wxDeleteStockObjects();

    wxDeleteStockLists();

    delete wxTheColourDatabase;
    wxTheColourDatabase = NULL;

    delete wxPendingEvents;
    wxPendingEvents = NULL;

#if wxUSE_THREADS
    delete wxPendingEventsLocker;
    wxPendingEventsLocker = NULL;

    #if wxUSE_VALIDATORS
        // If we don't do the following, we get an apparent memory leak.
        ((wxEvtHandler&) wxDefaultValidator).ClearEventLocker();
    #endif // wxUSE_VALIDATORS
#endif // wxUSE_THREADS
}

#if wxUSE_CMDLINE_PARSER

// ----------------------------------------------------------------------------
// GUI-specific command line options handling
// ----------------------------------------------------------------------------

#define OPTION_THEME   _T("theme")
#define OPTION_MODE    _T("mode")

void wxAppBase::OnInitCmdLine(wxCmdLineParser& parser)
{
    // first add the standard non GUI options
    wxAppConsole::OnInitCmdLine(parser);

    // the standard command line options
    static const wxCmdLineEntryDesc cmdLineGUIDesc[] =
    {
#ifdef __WXUNIVERSAL__
        {
            wxCMD_LINE_OPTION,
            wxEmptyString,
            OPTION_THEME,
            gettext_noop("specify the theme to use"),
            wxCMD_LINE_VAL_STRING,
            0x0
        },
#endif // __WXUNIVERSAL__

#if defined(__WXMGL__)
        // VS: this is not specific to wxMGL, all fullscreen (framebuffer) ports
        //     should provide this option. That's why it is in common/appcmn.cpp
        //     and not mgl/app.cpp
        {
            wxCMD_LINE_OPTION,
            wxEmptyString,
            OPTION_MODE,
            gettext_noop("specify display mode to use (e.g. 640x480-16)"),
            wxCMD_LINE_VAL_STRING,
            0x0
        },
#endif // __WXMGL__

        // terminator
        {
            wxCMD_LINE_NONE,
            wxEmptyString,
            wxEmptyString,
            wxEmptyString,
            wxCMD_LINE_VAL_NONE,
            0x0
        }
    };

    parser.SetDesc(cmdLineGUIDesc);
}

bool wxAppBase::OnCmdLineParsed(wxCmdLineParser& parser)
{
#ifdef __WXUNIVERSAL__
    wxString themeName;
    if ( parser.Found(OPTION_THEME, &themeName) )
    {
        wxTheme *theme = wxTheme::Create(themeName);
        if ( !theme )
        {
            wxLogError(_("Unsupported theme '%s'."), themeName.c_str());
            return false;
        }

        // Delete the defaultly created theme and set the new theme.
        delete wxTheme::Get();
        wxTheme::Set(theme);
    }
#endif // __WXUNIVERSAL__

#if defined(__WXMGL__)
    wxString modeDesc;
    if ( parser.Found(OPTION_MODE, &modeDesc) )
    {
        unsigned w, h, bpp;
        if ( wxSscanf(modeDesc.c_str(), _T("%ux%u-%u"), &w, &h, &bpp) != 3 )
        {
            wxLogError(_("Invalid display mode specification '%s'."), modeDesc.c_str());
            return false;
        }

        if ( !SetDisplayMode(wxVideoMode(w, h, bpp)) )
            return false;
    }
#endif // __WXMGL__

    return wxAppConsole::OnCmdLineParsed(parser);
}

#endif // wxUSE_CMDLINE_PARSER

// ----------------------------------------------------------------------------
// main event loop implementation
// ----------------------------------------------------------------------------

int wxAppBase::MainLoop()
{
#if wxUSE_EVTLOOP_IN_APP
    wxEventLoopTiedPtr mainLoop(&m_mainLoop, new wxEventLoop);

    return m_mainLoop->Run();
#else // !wxUSE_EVTLOOP_IN_APP
    return 0;
#endif // wxUSE_EVTLOOP_IN_APP/!wxUSE_EVTLOOP_IN_APP
}

void wxAppBase::ExitMainLoop()
{
#if wxUSE_EVTLOOP_IN_APP
    // we should exit from the main event loop, not just any currently active
    // (e.g. modal dialog) event loop
    if ( m_mainLoop && m_mainLoop->IsRunning() )
    {
        m_mainLoop->Exit(0);
    }
#endif // wxUSE_EVTLOOP_IN_APP
}

bool wxAppBase::Pending()
{
#if wxUSE_EVTLOOP_IN_APP
    // use the currently active message loop here, not m_mainLoop, because if
    // we're showing a modal dialog (with its own event loop) currently the
    // main event loop is not running anyhow
    wxEventLoop * const loop = wxEventLoop::GetActive();

    return loop && loop->Pending();
#else // wxUSE_EVTLOOP_IN_APP
    return false;
#endif // wxUSE_EVTLOOP_IN_APP/!wxUSE_EVTLOOP_IN_APP
}

bool wxAppBase::Dispatch()
{
#if wxUSE_EVTLOOP_IN_APP
    // see comment in Pending()
    wxEventLoop * const loop = wxEventLoop::GetActive();

    return loop && loop->Dispatch();
#else // wxUSE_EVTLOOP_IN_APP
    return true;
#endif // wxUSE_EVTLOOP_IN_APP/!wxUSE_EVTLOOP_IN_APP
}

// ----------------------------------------------------------------------------
// OnXXX() hooks
// ----------------------------------------------------------------------------

bool wxAppBase::OnInitGui()
{
#ifdef __WXUNIVERSAL__
    if ( !wxTheme::Get() && !wxTheme::CreateDefault() )
        return false;
#endif // __WXUNIVERSAL__

    return true;
}

int wxAppBase::OnRun()
{
    // see the comment in ctor: if the initial value hasn't been changed, use
    // the default Yes from now on
    if ( m_exitOnFrameDelete == Later )
    {
        m_exitOnFrameDelete = Yes;
    }
    //else: it has been changed, assume the user knows what he is doing

    return MainLoop();
}

int wxAppBase::OnExit()
{
#ifdef __WXUNIVERSAL__
    delete wxTheme::Set(NULL);
#endif // __WXUNIVERSAL__

    return wxAppConsole::OnExit();
}

void wxAppBase::Exit()
{
    ExitMainLoop();
}

wxAppTraits *wxAppBase::CreateTraits()
{
    return new wxGUIAppTraits;
}

// ----------------------------------------------------------------------------
// misc
// ----------------------------------------------------------------------------

void wxAppBase::SetActive(bool active, wxWindow * WXUNUSED(lastFocus))
{
    if ( active == m_isActive )
        return;

    m_isActive = active;

    wxActivateEvent event(wxEVT_ACTIVATE_APP, active);
    event.SetEventObject(this);

    (void)ProcessEvent(event);
}

void wxAppBase::DeletePendingObjects()
{
    wxList::compatibility_iterator node = wxPendingDelete.GetFirst();
    while (node)
    {
        wxObject *obj = node->GetData();

        delete obj;

        if (wxPendingDelete.Member(obj))
            wxPendingDelete.Erase(node);

        // Deleting one object may have deleted other pending
        // objects, so start from beginning of list again.
        node = wxPendingDelete.GetFirst();
    }
}

// Returns true if more time is needed.
bool wxAppBase::ProcessIdle()
{
    wxIdleEvent event;
    bool needMore = false;
    wxWindowList::compatibility_iterator node = wxTopLevelWindows.GetFirst();
    while (node)
    {
        wxWindow* win = node->GetData();
        if (SendIdleEvents(win, event))
            needMore = true;
        node = node->GetNext();
    }

    event.SetEventObject(this);
    (void) ProcessEvent(event);
    if (event.MoreRequested())
        needMore = true;

    wxUpdateUIEvent::ResetUpdateTime();

    return needMore;
}

// Send idle event to window and all subwindows
bool wxAppBase::SendIdleEvents(wxWindow* win, wxIdleEvent& event)
{
    bool needMore = false;

    win->OnInternalIdle();

    if (wxIdleEvent::CanSend(win))
    {
        event.SetEventObject(win);
        win->GetEventHandler()->ProcessEvent(event);

        if (event.MoreRequested())
            needMore = true;
    }
    wxWindowList::compatibility_iterator node = win->GetChildren().GetFirst();
    while ( node )
    {
        wxWindow *child = node->GetData();
        if (SendIdleEvents(child, event))
            needMore = true;

        node = node->GetNext();
    }

    return needMore;
}

void wxAppBase::OnIdle(wxIdleEvent& WXUNUSED(event))
{
    // If there are pending events, we must process them: pending events
    // are either events to the threads other than main or events posted
    // with wxPostEvent() functions
    // GRG: I have moved this here so that all pending events are processed
    //   before starting to delete any objects. This behaves better (in
    //   particular, wrt wxPostEvent) and is coherent with wxGTK's current
    //   behaviour. Changed Feb/2000 before 2.1.14
    ProcessPendingEvents();

    // 'Garbage' collection of windows deleted with Close().
    DeletePendingObjects();

#if wxUSE_LOG
    // flush the logged messages if any
    wxLog::FlushActive();
#endif // wxUSE_LOG

}

// ----------------------------------------------------------------------------
// wxGUIAppTraitsBase
// ----------------------------------------------------------------------------

#if wxUSE_LOG

wxLog *wxGUIAppTraitsBase::CreateLogTarget()
{
#if wxUSE_LOGGUI
    return new wxLogGui;
#else
    // we must have something!
    return new wxLogStderr;
#endif
}

#endif // wxUSE_LOG

wxMessageOutput *wxGUIAppTraitsBase::CreateMessageOutput()
{
    // The standard way of printing help on command line arguments (app --help)
    // is (according to common practice):
    //     - console apps: to stderr (on any platform)
    //     - GUI apps: stderr on Unix platforms (!)
    //                 message box under Windows and others
#ifdef __UNIX__
    return new wxMessageOutputStderr;
#else // !__UNIX__
    // wxMessageOutputMessageBox doesn't work under Motif
    #ifdef __WXMOTIF__
        return new wxMessageOutputLog;
    #else
        return new wxMessageOutputMessageBox;
    #endif
#endif // __UNIX__/!__UNIX__
}

#if wxUSE_FONTMAP

wxFontMapper *wxGUIAppTraitsBase::CreateFontMapper()
{
    return new wxFontMapper;
}

#endif // wxUSE_FONTMAP

wxRendererNative *wxGUIAppTraitsBase::CreateRenderer()
{
    // use the default native renderer by default
    return NULL;
}

#ifdef __WXDEBUG__

bool wxGUIAppTraitsBase::ShowAssertDialog(const wxString& msg)
{
    // under MSW we prefer to use the base class version using ::MessageBox()
    // even if wxMessageBox() is available because it has less chances to
    // double fault our app than our wxMessageBox()
#if defined(__WXMSW__) || !wxUSE_MSGDLG
    return wxAppTraitsBase::ShowAssertDialog(msg);
#else // wxUSE_MSGDLG
    // this message is intentionally not translated -- it is for
    // developpers only
    wxString msgDlg(msg);
    msgDlg += wxT("\nDo you want to stop the program?\n")
              wxT("You can also choose [Cancel] to suppress ")
              wxT("further warnings.");

    switch ( wxMessageBox(msgDlg, wxT("wxWidgets Debug Alert"),
                          wxYES_NO | wxCANCEL | wxICON_STOP ) )
    {
        case wxYES:
            wxTrap();
            break;

        case wxCANCEL:
            // no more asserts
            return true;

        //case wxNO: nothing to do
    }

    return false;
#endif // !wxUSE_MSGDLG/wxUSE_MSGDLG
}

#endif // __WXDEBUG__

bool wxGUIAppTraitsBase::HasStderr()
{
    // we consider that under Unix stderr always goes somewhere, even if the
    // user doesn't always see it under GUI desktops
#ifdef __UNIX__
    return true;
#else
    return false;
#endif
}

void wxGUIAppTraitsBase::ScheduleForDestroy(wxObject *object)
{
    if ( !wxPendingDelete.Member(object) )
        wxPendingDelete.Append(object);
}

void wxGUIAppTraitsBase::RemoveFromPendingDelete(wxObject *object)
{
    wxPendingDelete.DeleteObject(object);
}

#if wxUSE_SOCKETS

#if defined(__WINDOWS__)
    #include "wx/msw/gsockmsw.h"
#elif defined(__UNIX__) || defined(__DARWIN__) || defined(__OS2__)
    #include "wx/unix/gsockunx.h"
#elif defined(__WXMAC__)
  #include <MacHeaders.c>
  #define OTUNIXERRORS 1
  #include <OpenTransport.h>
  #include <OpenTransportProviders.h>
  #include <OpenTptInternet.h>

  #include "wx/mac/gsockmac.h"
#else
    #error "Must include correct GSocket header here"
#endif

GSocketGUIFunctionsTable* wxGUIAppTraitsBase::GetSocketGUIFunctionsTable()
{
#if defined(__WXMAC__) && !defined(__DARWIN__)
    // NB: wxMac CFM does not have any GUI-specific functions in gsocket.c and
    //     so it doesn't need this table at all
    return NULL;
#else // !__WXMAC__ || __DARWIN__
    static GSocketGUIFunctionsTableConcrete table;
    return &table;
#endif // !__WXMAC__ || __DARWIN__
}

#endif

