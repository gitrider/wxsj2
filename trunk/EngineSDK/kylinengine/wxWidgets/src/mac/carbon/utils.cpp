/////////////////////////////////////////////////////////////////////////////
// Name:        utils.cpp
// Purpose:     Various utilities
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// RCS-ID:      $Id: utils.cpp,v 1.101 2005/04/19 10:16:28 SC Exp $
// Copyright:   (c) Stefan Csomor
// Licence:       wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
// Note: this is done in utilscmn.cpp now.
// #pragma implementation "utils.h"
#endif

#include "wx/wxprec.h"

#include "wx/utils.h"
#include "wx/app.h"
#include "wx/apptrait.h"

#if wxUSE_GUI
    #include "wx/mac/uma.h"
    #include "wx/font.h"
    #include "wx/toplevel.h"
#else
    #include "wx/intl.h"
#endif

#include <ctype.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "MoreFilesX.h"

#ifndef __DARWIN__
#include <Threads.h>
#include <Sound.h>
#endif

#if wxUSE_GUI
#if TARGET_API_MAC_OSX
#include <CoreServices/CoreServices.h>
#else
#include <DriverServices.h>
#include <Multiprocessing.h>
#endif

#include <ATSUnicode.h>
#include <TextCommon.h>
#include <TextEncodingConverter.h>
#endif // wxUSE_GUI

#include  "wx/mac/private.h"  // includes mac headers

#if defined(__MWERKS__) && wxUSE_UNICODE
    #include <wtime.h>
#endif

// ---------------------------------------------------------------------------
// code used in both base and GUI compilation
// ---------------------------------------------------------------------------

// our OS version is the same in non GUI and GUI cases
static int DoGetOSVersion(int *majorVsn, int *minorVsn)
{
    long theSystem ;

    // are there x-platform conventions ?

    Gestalt(gestaltSystemVersion, &theSystem) ;
    if (minorVsn != NULL) {
        *minorVsn = (theSystem & 0xFF ) ;
    }
    if (majorVsn != NULL) {
        *majorVsn = (theSystem >> 8 ) ;
    }
#ifdef __DARWIN__
    return wxMAC_DARWIN;
#else
    return wxMAC;
#endif
}


#if wxUSE_BASE

// ----------------------------------------------------------------------------
// debugging support
// ----------------------------------------------------------------------------

#if defined(__WXDEBUG__) && defined(__WXMAC__) && !defined(__DARWIN__) && defined(__MWERKS__) && (__MWERKS__ >= 0x2400)

// MetroNub stuff doesn't seem to work in CodeWarrior 5.3 Carbon builds...

#ifndef __MetroNubUtils__
#include "MetroNubUtils.h"
#endif

#ifndef __GESTALT__
#include <Gestalt.h>
#endif

#if TARGET_API_MAC_CARBON

    #include <CodeFragments.h>

    extern "C" long CallUniversalProc(UniversalProcPtr theProcPtr, ProcInfoType procInfo, ...);

    ProcPtr gCallUniversalProc_Proc = NULL;

#endif

static MetroNubUserEntryBlock*    gMetroNubEntry = NULL;

static long fRunOnce = false;

/* ---------------------------------------------------------------------------
        IsMetroNubInstalled
   --------------------------------------------------------------------------- */

Boolean IsMetroNubInstalled()
{
    if (!fRunOnce)
    {
        long result, value;

        fRunOnce = true;
        gMetroNubEntry = NULL;

        if (Gestalt(gestaltSystemVersion, &value) == noErr && value < 0x1000)
        {
            /* look for MetroNub's Gestalt selector */
            if (Gestalt(kMetroNubUserSignature, &result) == noErr)
            {

            #if TARGET_API_MAC_CARBON
                if (gCallUniversalProc_Proc == NULL)
                {
                    CFragConnectionID   connectionID;
                    Ptr                 mainAddress;
                    Str255              errorString;
                    ProcPtr             symbolAddress;
                    OSErr               err;
                    CFragSymbolClass    symbolClass;

                    symbolAddress = NULL;
                    err = GetSharedLibrary("\pInterfaceLib", kPowerPCCFragArch, kFindCFrag,
                                           &connectionID, &mainAddress, errorString);

                    if (err != noErr)
                    {
                        gCallUniversalProc_Proc = NULL;
                        goto end;
                    }

                    err = FindSymbol(connectionID, "\pCallUniversalProc",
                                    (Ptr *) &gCallUniversalProc_Proc, &symbolClass);

                    if (err != noErr)
                    {
                        gCallUniversalProc_Proc = NULL;
                        goto end;
                    }
                }
            #endif

                {
                    MetroNubUserEntryBlock* block = (MetroNubUserEntryBlock *)result;

                    /* make sure the version of the API is compatible */
                    if (block->apiLowVersion <= kMetroNubUserAPIVersion &&
                        kMetroNubUserAPIVersion <= block->apiHiVersion)
                        gMetroNubEntry = block;        /* success! */
                }

            }
        }
    }

end:

#if TARGET_API_MAC_CARBON
    return (gMetroNubEntry != NULL && gCallUniversalProc_Proc != NULL);
#else
    return (gMetroNubEntry != NULL);
#endif
}

/* ---------------------------------------------------------------------------
        IsMWDebuggerRunning                                            [v1 API]
   --------------------------------------------------------------------------- */

Boolean IsMWDebuggerRunning()
{
    if (IsMetroNubInstalled())
        return CallIsDebuggerRunningProc(gMetroNubEntry->isDebuggerRunning);
    else
        return false;
}

/* ---------------------------------------------------------------------------
        AmIBeingMWDebugged                                            [v1 API]
   --------------------------------------------------------------------------- */

Boolean AmIBeingMWDebugged()
{
    if (IsMetroNubInstalled())
        return CallAmIBeingDebuggedProc(gMetroNubEntry->amIBeingDebugged);
    else
        return false;
}

extern bool WXDLLEXPORT wxIsDebuggerRunning()
{
    return IsMWDebuggerRunning() && AmIBeingMWDebugged();
}

#else

extern bool WXDLLEXPORT wxIsDebuggerRunning()
{
    return false;
}

#endif // defined(__WXMAC__) && !defined(__DARWIN__) && (__MWERKS__ >= 0x2400)


#ifndef __DARWIN__
// defined in unix/utilsunx.cpp for Mac OS X

// get full hostname (with domain name if possible)
bool wxGetFullHostName(wxChar *buf, int maxSize)
{
    return wxGetHostName(buf, maxSize);
}

// Get hostname only (without domain name)
bool wxGetHostName(wxChar *buf, int maxSize)
{
    // Gets Chooser name of user by examining a System resource.

    const short kComputerNameID = -16413;

    short oldResFile = CurResFile() ;
    UseResFile(0);
    StringHandle chooserName = (StringHandle)::GetString(kComputerNameID);
    UseResFile(oldResFile);

    if (chooserName && *chooserName)
    {
        HLock( (Handle) chooserName ) ;
        wxString name = wxMacMakeStringFromPascal( *chooserName ) ;
        HUnlock( (Handle) chooserName ) ;
        ReleaseResource( (Handle) chooserName ) ;
        wxStrncpy( buf , name , maxSize - 1 ) ;
    }
    else
        buf[0] = 0 ;

    return true;
}

// Get user ID e.g. jacs
bool wxGetUserId(wxChar *buf, int maxSize)
{
  return wxGetUserName( buf , maxSize ) ;
}

const wxChar* wxGetHomeDir(wxString *pstr)
{
    *pstr = wxMacFindFolder(  (short) kOnSystemDisk, kPreferencesFolderType, kDontCreateFolder ) ;
    return pstr->c_str() ;
}

// Get user name e.g. Stefan Csomor
bool wxGetUserName(wxChar *buf, int maxSize)
{
    // Gets Chooser name of user by examining a System resource.

    const short kChooserNameID = -16096;

    short oldResFile = CurResFile() ;
    UseResFile(0);
    StringHandle chooserName = (StringHandle)::GetString(kChooserNameID);
    UseResFile(oldResFile);

    if (chooserName && *chooserName)
    {
        HLock( (Handle) chooserName ) ;
        wxString name = wxMacMakeStringFromPascal( *chooserName ) ;
        HUnlock( (Handle) chooserName ) ;
        ReleaseResource( (Handle) chooserName ) ;
        wxStrncpy( buf , name , maxSize - 1 ) ;
    }
    else
        buf[0] = 0 ;

    return true;
}

int wxKill(long pid, wxSignal sig , wxKillError *rc, int flags)
{
    // TODO
    return 0;
}

WXDLLEXPORT bool wxGetEnv(const wxString& var, wxString *value)
{
    // TODO : under classic there is no environement support, under X yes
    return false ;
}

// set the env var name to the given value, return true on success
WXDLLEXPORT bool wxSetEnv(const wxString& var, const wxChar *value)
{
    // TODO : under classic there is no environement support, under X yes
    return false ;
}

//
// Execute a program in an Interactive Shell
//
bool wxShell(const wxString& command)
{
    // TODO
    return false;
}

// Shutdown or reboot the PC
bool wxShutdown(wxShutdownFlags wFlags)
{
    // TODO
    return false;
}

wxPowerType wxGetPowerType()
{
    // TODO
    return wxPOWER_UNKNOWN;
}

wxBatteryState wxGetBatteryState()
{
    // TODO
    return wxBATTERY_UNKNOWN_STATE;
}

// Get free memory in bytes, or -1 if cannot determine amount (e.g. on UNIX)
wxMemorySize wxGetFreeMemory()
{
    return (wxMemorySize)FreeMem() ;
}

#ifndef __DARWIN__

void wxMicroSleep(unsigned long microseconds)
{
    AbsoluteTime wakeup = AddDurationToAbsolute( microseconds * durationMicrosecond , UpTime());
    MPDelayUntil( & wakeup);
}

void wxMilliSleep(unsigned long milliseconds)
{
    AbsoluteTime wakeup = AddDurationToAbsolute( milliseconds, UpTime());
    MPDelayUntil( & wakeup);
}

void wxSleep(int nSecs)
{
    wxMilliSleep(1000*nSecs);
}

#endif

// Consume all events until no more left
void wxFlushEvents()
{
}

#endif // !__DARWIN__

// Emit a beeeeeep
void wxBell()
{
    SysBeep(30);
}

wxToolkitInfo& wxConsoleAppTraits::GetToolkitInfo()
{
    static wxToolkitInfo info;
    info.os = DoGetOSVersion(&info.versionMajor, &info.versionMinor);
    info.name = _T("wxBase");
    return info;
}

#endif // wxUSE_BASE

#if wxUSE_GUI

wxToolkitInfo& wxGUIAppTraits::GetToolkitInfo()
{
    static wxToolkitInfo info;
    info.os = DoGetOSVersion(&info.versionMajor, &info.versionMinor);
    info.shortName = _T("mac");
    info.name = _T("wxMac");
#ifdef __WXUNIVERSAL__
    info.shortName << _T("univ");
    info.name << _T("/wxUniversal");
#endif
    return info;
}

// Reading and writing resources (eg WIN.INI, .Xdefaults)
#if wxUSE_RESOURCES
bool wxWriteResource(const wxString& section, const wxString& entry, const wxString& value, const wxString& file)
{
    // TODO
    return false;
}

bool wxWriteResource(const wxString& section, const wxString& entry, float value, const wxString& file)
{
    wxString buf;
    buf.Printf(wxT("%.4f"), value);

    return wxWriteResource(section, entry, buf, file);
}

bool wxWriteResource(const wxString& section, const wxString& entry, long value, const wxString& file)
{
    wxString buf;
    buf.Printf(wxT("%ld"), value);

    return wxWriteResource(section, entry, buf, file);
}

bool wxWriteResource(const wxString& section, const wxString& entry, int value, const wxString& file)
{
    wxString buf;
    buf.Printf(wxT("%d"), value);

    return wxWriteResource(section, entry, buf, file);
}

bool wxGetResource(const wxString& section, const wxString& entry, char **value, const wxString& file)
{
    // TODO
    return false;
}

bool wxGetResource(const wxString& section, const wxString& entry, float *value, const wxString& file)
{
    char *s = NULL;
    bool succ = wxGetResource(section, entry, (char **)&s, file);
    if (succ)
    {
        *value = (float)strtod(s, NULL);
        delete[] s;
        return true;
    }
    else return false;
}

bool wxGetResource(const wxString& section, const wxString& entry, long *value, const wxString& file)
{
    char *s = NULL;
    bool succ = wxGetResource(section, entry, (char **)&s, file);
    if (succ)
    {
        *value = strtol(s, NULL, 10);
        delete[] s;
        return true;
    }
    else return false;
}

bool wxGetResource(const wxString& section, const wxString& entry, int *value, const wxString& file)
{
    char *s = NULL;
    bool succ = wxGetResource(section, entry, (char **)&s, file);
    if (succ)
    {
        *value = (int)strtol(s, NULL, 10);
        delete[] s;
        return true;
    }
    else return false;
}
#endif // wxUSE_RESOURCES

int gs_wxBusyCursorCount = 0;
extern wxCursor    gMacCurrentCursor ;
wxCursor        gMacStoredActiveCursor ;

// Set the cursor to the busy cursor for all windows
void wxBeginBusyCursor(wxCursor *cursor)
{
    if (gs_wxBusyCursorCount++ == 0)
    {
        gMacStoredActiveCursor = gMacCurrentCursor ;
        cursor->MacInstall() ;
    }
    //else: nothing to do, already set
}

// Restore cursor to normal
void wxEndBusyCursor()
{
    wxCHECK_RET( gs_wxBusyCursorCount > 0,
        wxT("no matching wxBeginBusyCursor() for wxEndBusyCursor()") );

    if (--gs_wxBusyCursorCount == 0)
    {
        gMacStoredActiveCursor.MacInstall() ;
        gMacStoredActiveCursor = wxNullCursor ;
    }
}

// true if we're between the above two calls
bool wxIsBusy()
{
    return (gs_wxBusyCursorCount > 0);
}

#endif // wxUSE_GUI

#if wxUSE_BASE

wxString wxMacFindFolder( short        vol,
              OSType       folderType,
              Boolean      createFolder)
{
    FSRef fsRef ;
    wxString strDir ;

    if ( FSFindFolder( vol, folderType, createFolder, &fsRef) == noErr)
        strDir = wxMacFSRefToPath( &fsRef ) + wxFILE_SEP_PATH ;

    return strDir ;
}

#endif // wxUSE_BASE

#if wxUSE_GUI

// Check whether this window wants to process messages, e.g. Stop button
// in long calculations.
bool wxCheckForInterrupt(wxWindow *wnd)
{
    // TODO
    return false;
}

void wxGetMousePosition( int* x, int* y )
{
    Point pt ;

    GetMouse( &pt ) ;
    LocalToGlobal( &pt ) ;
    *x = pt.h ;
    *y = pt.v ;
};

// Return true if we have a colour display
bool wxColourDisplay()
{
    return true;
}

// Returns depth of screen
int wxDisplayDepth()
{
    Rect globRect ;
    SetRect(&globRect, -32760, -32760, 32760, 32760);
    GDHandle    theMaxDevice;

    int theDepth = 8;
    theMaxDevice = GetMaxDevice(&globRect);
    if (theMaxDevice != nil)
        theDepth = (**(**theMaxDevice).gdPMap).pixelSize;

    return theDepth ;
}

// Get size of display
void wxDisplaySize(int *width, int *height)
{
    BitMap screenBits;
    GetQDGlobalsScreenBits( &screenBits );

    if (width != NULL) {
        *width = screenBits.bounds.right - screenBits.bounds.left  ;
    }
    if (height != NULL) {
        *height = screenBits.bounds.bottom - screenBits.bounds.top ;
    }
}

void wxDisplaySizeMM(int *width, int *height)
{
    wxDisplaySize(width, height);
    // on mac 72 is fixed (at least now ;-)
    float cvPt2Mm = 25.4 / 72;

    if (width != NULL) {
        *width = int( *width * cvPt2Mm );
    }
    if (height != NULL) {
        *height = int( *height * cvPt2Mm );
    }
}

void wxClientDisplayRect(int *x, int *y, int *width, int *height)
{
    Rect r ;
    GetAvailableWindowPositioningBounds( GetMainDevice() , &r ) ;
    if ( x )
        *x = r.left ;
    if ( y )
        *y = r.top ;
    if ( width )
        *width = r.right - r.left ;
    if ( height )
        *height = r.bottom - r.top ;
}

wxWindow* wxFindWindowAtPoint(const wxPoint& pt)
{
    return wxGenericFindWindowAtPoint(pt);
}

#endif // wxUSE_GUI

#if wxUSE_BASE

wxString wxGetOsDescription()
{
#ifdef WXWIN_OS_DESCRIPTION
    // use configure generated description if available
    return wxString(wxT("MacOS (")) + wxT(WXWIN_OS_DESCRIPTION) + wxString(wxT(")"));
#else
    return wxT("MacOS") ; //TODO:define further
#endif
}

#ifndef __DARWIN__
wxChar *wxGetUserHome (const wxString& user)
{
    // TODO
    return NULL;
}

bool wxGetDiskSpace(const wxString& path, wxLongLong *pTotal, wxLongLong *pFree)
{
    if ( path.empty() )
        return false;

    wxString p = path ;
    if (p[0u] == ':' ) {
      p = wxGetCwd() + p ;
    }

    int pos = p.Find(':') ;
    if ( pos != wxNOT_FOUND ) {
      p = p.Mid(1,pos) ;
    }

    p = p + wxT(":") ;

    OSErr err = noErr ;

    FSRef fsRef ;
    err = wxMacPathToFSRef( p , &fsRef ) ;
    if ( noErr == err )
    {
        FSVolumeRefNum vRefNum ;
        err = FSGetVRefNum( &fsRef , &vRefNum ) ;
        if ( noErr == err )
        {
            UInt64 freeBytes , totalBytes ;
            err = FSGetVInfo( vRefNum , NULL , &freeBytes , &totalBytes ) ;
            if ( noErr == err )
            {
                if ( pTotal )
                    *pTotal = wxLongLong( totalBytes ) ;
                if ( pFree )
                    *pFree = wxLongLong( freeBytes ) ;
            }
        }
    }

    return err == noErr ;
}
#endif // !__DARWIN__

//---------------------------------------------------------------------------
// wxMac Specific utility functions
//---------------------------------------------------------------------------

void wxMacStringToPascal( const wxString&from , StringPtr to )
{
    wxCharBuffer buf = from.mb_str( wxConvLocal ) ;
    int len = strlen(buf) ;

    if ( len > 255 )
        len = 255 ;
    to[0] = len ;
    memcpy( (char*) &to[1] , buf , len ) ;
}

wxString wxMacMakeStringFromPascal( ConstStringPtr from )
{
    return wxString( (char*) &from[1] , wxConvLocal , from[0] ) ;
}


// ----------------------------------------------------------------------------
// Common Event Support
// ----------------------------------------------------------------------------


extern ProcessSerialNumber gAppProcess ;

void wxMacWakeUp()
{
    ProcessSerialNumber psn ;
    Boolean isSame ;
    psn.highLongOfPSN = 0 ;
    psn.lowLongOfPSN = kCurrentProcess ;
    SameProcess( &gAppProcess , &psn , &isSame ) ;
    if ( isSame )
    {
#if TARGET_CARBON
        static wxMacCarbonEvent s_wakeupEvent ;
        OSStatus err = noErr ;
        if ( !s_wakeupEvent.IsValid() )
        {
           err = s_wakeupEvent.Create( 'WXMC', 'WXMC', GetCurrentEventTime(),
                        kEventAttributeNone ) ;
        }
        if ( err == noErr )
        {
            if ( IsEventInQueue( GetMainEventQueue() , s_wakeupEvent ) )
                return ;
            s_wakeupEvent.SetTime(0) ;
            err = PostEventToQueue(GetMainEventQueue(), s_wakeupEvent,
                                  kEventPriorityHigh);
        }
#else
        PostEvent( nullEvent , 0 ) ;
#endif
    }
    else
    {
        WakeUpProcess( &gAppProcess ) ;
    }
}

#endif // wxUSE_BASE

#if wxUSE_GUI

// ----------------------------------------------------------------------------
// Native Struct Conversions
// ----------------------------------------------------------------------------


void wxMacRectToNative( const wxRect *wx , Rect *n ) 
{
    n->left = wx->x ;
    n->top = wx->y ;
    n->right = wx->x + wx->width ;
    n->bottom = wx->y + wx->height ;
}

void wxMacNativeToRect( const Rect *n , wxRect* wx ) 
{
    wx->x = n->left ;
    wx->y = n->top ;
    wx->width = n->right - n->left ;
    wx->height = n->bottom - n->top ;
}

void wxMacPointToNative( const wxPoint* wx , Point *n ) 
{
    n->h = wx->x ;
    n->v = wx->y ;
}

void wxMacNativeToPoint( const Point *n , wxPoint* wx ) 
{
    wx->x = n->h ;
    wx->y = n->v ;
}

// ----------------------------------------------------------------------------
// Carbon Event Support
// ----------------------------------------------------------------------------


OSStatus wxMacCarbonEvent::GetParameter(EventParamName inName, EventParamType inDesiredType, UInt32 inBufferSize, void * outData)
{
    return ::GetEventParameter( m_eventRef , inName , inDesiredType , NULL , inBufferSize , NULL , outData ) ;
}

OSStatus wxMacCarbonEvent::SetParameter(EventParamName inName, EventParamType inType, UInt32 inBufferSize, const void * inData)
{
    return ::SetEventParameter( m_eventRef , inName , inType , inBufferSize , inData ) ;
}

// ----------------------------------------------------------------------------
// Control Access Support
// ----------------------------------------------------------------------------

wxMacControl::wxMacControl(wxWindow* peer , bool isRootControl )
{
    Init() ;
    m_peer = peer ;
    m_isRootControl = isRootControl ;
    m_isCompositing = peer->MacGetTopLevelWindow()->MacUsesCompositing() ;
}

wxMacControl::wxMacControl( wxWindow* peer , ControlRef control ) 
{
    Init() ;
    m_peer = peer ;
    m_isCompositing = peer->MacGetTopLevelWindow()->MacUsesCompositing() ;
    m_controlRef = control ;
}

wxMacControl::wxMacControl( wxWindow* peer , WXWidget control )
{
    Init() ;
    m_peer = peer ;
    m_isCompositing = peer->MacGetTopLevelWindow()->MacUsesCompositing() ;
    m_controlRef = (ControlRef) control ;
}

wxMacControl::~wxMacControl()
{
}

void wxMacControl::Init()
{
    m_peer = NULL ;
    m_controlRef = NULL ;
    m_needsFocusRect = false ;
    m_isCompositing = false ;
    m_isRootControl = false ;
}

void wxMacControl::Dispose()
{
    ::DisposeControl( m_controlRef ) ;
    m_controlRef = NULL ;
}

void wxMacControl::SetReference( SInt32 data )
{
    SetControlReference( m_controlRef , data ) ;
}

OSStatus wxMacControl::GetData(ControlPartCode inPartCode , ResType inTag , Size inBufferSize , void * inOutBuffer , Size * outActualSize ) const
{
    return ::GetControlData( m_controlRef , inPartCode , inTag , inBufferSize , inOutBuffer , outActualSize ) ;
}

OSStatus wxMacControl::GetDataSize(ControlPartCode inPartCode , ResType inTag , Size * outActualSize ) const
{
    return ::GetControlDataSize( m_controlRef , inPartCode , inTag , outActualSize ) ;
}

OSStatus wxMacControl::SetData(ControlPartCode inPartCode , ResType inTag , Size inSize , const void * inData)
{
    return ::SetControlData( m_controlRef , inPartCode , inTag , inSize , inData ) ;
}

OSStatus wxMacControl::SendEvent(  EventRef event , OptionBits inOptions )
{
#if TARGET_API_MAC_OSX
    return SendEventToEventTargetWithOptions( event,
        HIObjectGetEventTarget(  (HIObjectRef) m_controlRef ), inOptions );
#else
    #pragma unused(inOptions)
    return SendEventToEventTarget(event,GetControlEventTarget( m_controlRef ) ) ;
#endif
}

OSStatus wxMacControl::SendHICommand( HICommand &command , OptionBits inOptions )
{
    wxMacCarbonEvent event( kEventClassCommand , kEventCommandProcess ) ;
    event.SetParameter<HICommand>(kEventParamDirectObject,command) ;
    return SendEvent( event , inOptions ) ;
}

OSStatus wxMacControl::SendHICommand( UInt32 commandID , OptionBits inOptions  )
{
    HICommand command ;
    memset( &command, 0 , sizeof(command) ) ;
    command.commandID = commandID ;
    return SendHICommand( command , inOptions ) ;
}

void wxMacControl::Flash( ControlPartCode part , UInt32 ticks )
{
    HiliteControl( m_controlRef , part ) ;
    unsigned long finalTicks ;
    Delay( ticks , &finalTicks ) ;
    HiliteControl( m_controlRef , kControlNoPart ) ;
}

SInt32 wxMacControl::GetValue() const
{
    return ::GetControl32BitValue( m_controlRef ) ;
}

SInt32 wxMacControl::GetMaximum() const
{
    return ::GetControl32BitMaximum( m_controlRef ) ;
}

SInt32 wxMacControl::GetMinimum() const
{
    return ::GetControl32BitMinimum( m_controlRef ) ;
}

void wxMacControl::SetValue( SInt32 v )
{
    ::SetControl32BitValue( m_controlRef , v ) ;
}

void wxMacControl::SetMinimum( SInt32 v )
{
    ::SetControl32BitMinimum( m_controlRef , v ) ;
}

void wxMacControl::SetMaximum( SInt32 v )
{
    ::SetControl32BitMaximum( m_controlRef , v ) ;
}

void wxMacControl::SetValueAndRange( SInt32 value , SInt32 minimum , SInt32 maximum )
{
    ::SetControl32BitMinimum( m_controlRef , minimum ) ;
    ::SetControl32BitMaximum( m_controlRef , maximum ) ;
    ::SetControl32BitValue( m_controlRef , value ) ;
}

OSStatus wxMacControl::SetFocus( ControlFocusPart focusPart )
{
    return SetKeyboardFocus(  GetControlOwner( m_controlRef )  ,
        m_controlRef , focusPart ) ;
}

bool wxMacControl::HasFocus() const
{
    ControlRef control ;
    GetKeyboardFocus( GetUserFocusWindow() , &control ) ;
    return control == m_controlRef ;
}

void wxMacControl::SetNeedsFocusRect( bool needs ) 
{
    m_needsFocusRect = needs ;
}

bool wxMacControl::NeedsFocusRect() const
{
    return m_needsFocusRect ;
}

void wxMacControl::VisibilityChanged(bool shown)
{
}

void wxMacControl::SuperChangedPosition()
{
}

void wxMacControl::SetFont( const wxFont & font , const wxColour& foreground , long windowStyle )
{
    m_font = font ;
    ControlFontStyleRec fontStyle;
    if ( font.MacGetThemeFontID() != kThemeCurrentPortFont )
    {
        switch( font.MacGetThemeFontID() )
        {
            case kThemeSmallSystemFont :    fontStyle.font = kControlFontSmallSystemFont ;  break ;
            case 109 /*mini font */ :       fontStyle.font = -5 ;                           break ;
            case kThemeSystemFont :         fontStyle.font = kControlFontBigSystemFont ;    break ;
            default :                       fontStyle.font = kControlFontBigSystemFont ;    break ;
        }
        fontStyle.flags = kControlUseFontMask ;
    }
    else
    {
        fontStyle.font = font.MacGetFontNum() ;
        fontStyle.style = font.MacGetFontStyle() ;
        fontStyle.size = font.MacGetFontSize() ;
        fontStyle.flags = kControlUseFontMask | kControlUseFaceMask | kControlUseSizeMask ;
    }

    fontStyle.just = teJustLeft ;
    fontStyle.flags |= kControlUseJustMask ;
    if ( ( windowStyle & wxALIGN_MASK ) & wxALIGN_CENTER_HORIZONTAL )
        fontStyle.just = teJustCenter ;
    else if ( ( windowStyle & wxALIGN_MASK ) & wxALIGN_RIGHT )
        fontStyle.just = teJustRight ;


    // we only should do this in case of a non-standard color, as otherwise 'disabled' controls
    // won't get grayed out by the system anymore

    if ( foreground != *wxBLACK )
    {
        fontStyle.foreColor = MAC_WXCOLORREF(foreground.GetPixel() ) ;
        fontStyle.flags |= kControlUseForeColorMask ;
    }

    ::SetControlFontStyle( m_controlRef , &fontStyle );
}

void wxMacControl::SetBackground( const wxBrush &WXUNUSED(brush) )
{
    // TODO
    // setting up a color proc is not recommended anymore
}

void wxMacControl::SetRange( SInt32 minimum , SInt32 maximum )
{
    ::SetControl32BitMinimum( m_controlRef , minimum ) ;
    ::SetControl32BitMaximum( m_controlRef , maximum ) ;
}

short wxMacControl::HandleKey(  SInt16 keyCode,  SInt16 charCode, EventModifiers modifiers )
{
    return HandleControlKey( m_controlRef , keyCode , charCode , modifiers ) ;
}

void wxMacControl::SetActionProc( ControlActionUPP   actionProc )
{
    SetControlAction( m_controlRef , actionProc ) ;
}

void wxMacControl::SetViewSize( SInt32 viewSize )
{
    SetControlViewSize(m_controlRef , viewSize ) ;
}

SInt32 wxMacControl::GetViewSize() const
{
    return GetControlViewSize( m_controlRef ) ;
}

bool wxMacControl::IsVisible() const
{
    return IsControlVisible( m_controlRef ) ;
}

void wxMacControl::SetVisibility( bool visible , bool redraw )
{
    SetControlVisibility( m_controlRef , visible , redraw ) ;
}

bool wxMacControl::IsEnabled() const
{
#if TARGET_API_MAC_OSX
    return IsControlEnabled( m_controlRef ) ;
#else
    return IsControlActive( m_controlRef ) ;
#endif
}

bool wxMacControl::IsActive() const
{
    return IsControlActive( m_controlRef ) ;
}

void wxMacControl::Enable( bool enable )
{
#if TARGET_API_MAC_OSX
    if ( enable )
        EnableControl( m_controlRef ) ;
    else
        DisableControl( m_controlRef ) ;
#else
    if ( enable )
        ActivateControl( m_controlRef ) ;
    else
        DeactivateControl( m_controlRef ) ;
#endif
}

void wxMacControl::SetDrawingEnabled( bool enable )
{
#if TARGET_API_MAC_OSX
    HIViewSetDrawingEnabled( m_controlRef , enable ) ;
#endif
}

#if TARGET_API_MAC_OSX
bool wxMacControl::GetNeedsDisplay() const
{
#if TARGET_API_MAC_OSX
    if ( m_isCompositing )
    {
        return HIViewGetNeedsDisplay( m_controlRef ) ;
   	}
   	else
#endif
	{
		if ( !IsVisible() )
            return false ;
			
	    Rect controlBounds ; 
	    GetControlBounds( m_controlRef, &controlBounds ) ;
	    RgnHandle rgn = NewRgn() ;
		GetWindowRegion ( GetControlOwner( m_controlRef ) , kWindowUpdateRgn , rgn ) ;
		Boolean intersect = RectInRgn ( &controlBounds , rgn ) ;
		DisposeRgn( rgn ) ;
		return intersect ;
	}

}
#endif

void wxMacControl::SetNeedsDisplay( RgnHandle where )
{
    if ( !IsVisible() )
        return ;
        
#if TARGET_API_MAC_OSX
    if ( m_isCompositing )
    {
        HIViewSetNeedsDisplayInRegion( m_controlRef , where , true ) ;
    }
    else
#endif
    {
        Rect controlBounds ; 
        GetControlBounds( m_controlRef, &controlBounds ) ;
        RgnHandle update = NewRgn() ;
        CopyRgn( where , update ) ;
        OffsetRgn( update , controlBounds.left , controlBounds.top ) ;
        InvalWindowRgn( GetControlOwner( m_controlRef) , update ) ; 
    }
}

void wxMacControl::SetNeedsDisplay( Rect* where )
{
    if ( !IsVisible() )
        return ;

#if TARGET_API_MAC_OSX
    if ( m_isCompositing )
    {
        if ( where != NULL )
        {
            RgnHandle update = NewRgn() ;
            RectRgn( update , where ) ;
            HIViewSetNeedsDisplayInRegion( m_controlRef , update , true ) ;
            DisposeRgn( update ) ;
        }
        else
            HIViewSetNeedsDisplay( m_controlRef , true ) ;
    }
    else
#endif
    {
        Rect controlBounds ; 
        GetControlBounds( m_controlRef, &controlBounds ) ;
        if ( where )
        {
            Rect whereLocal = *where ;
            OffsetRect( &whereLocal , controlBounds.left , controlBounds.top ) ;
            SectRect( &controlBounds , &whereLocal, &controlBounds ) ;
        }
        InvalWindowRect( GetControlOwner( m_controlRef) , &controlBounds ) ; 
    }
}

void  wxMacControl::Convert( wxPoint *pt , wxMacControl *from , wxMacControl *to )
{
#if TARGET_API_MAC_OSX
    if ( from->m_peer->MacGetTopLevelWindow()->MacUsesCompositing() )
    {
        HIPoint hiPoint ;
        hiPoint.x = pt->x ;
        hiPoint.y = pt->y ;
        HIViewConvertPoint( &hiPoint , from->m_controlRef , to->m_controlRef  ) ;
        pt->x = (int)hiPoint.x ;
        pt->y = (int)hiPoint.y ;
    }
    else
#endif
    {
        Rect fromRect ;
        Rect toRect ;
        GetControlBounds( from->m_controlRef , &fromRect ) ;
        GetControlBounds( to->m_controlRef , &toRect ) ;
        if ( from->m_isRootControl )
            fromRect.left = fromRect.top = 0 ;
        if ( to->m_isRootControl )
            toRect.left = toRect.top = 0 ; 
                    
        pt->x = pt->x + fromRect.left - toRect.left ;
        pt->y = pt->y + fromRect.top - toRect.top ;
    }
}

void wxMacControl::SetRect( Rect *r )
{
#if TARGET_API_MAC_OSX
    if ( m_isCompositing )
    {
        //A HIRect is actually a CGRect on OSX - which consists of two structures -
        //CGPoint and CGSize, which have two floats each
        HIRect hir = { { r->left , r->top }, { r->right - r->left , r->bottom - r->top } } ;
        HIViewSetFrame ( m_controlRef , &hir ) ;
        // eventuall we might have to do a SetVisibility( false , true ) ;
        // before and a SetVisibility( true , true ) ; after
    }
    else
#endif
    {
        bool vis = IsVisible() ;
        if ( vis )
        {
            Rect former ;
            GetControlBounds( m_controlRef , &former ) ;
            InvalWindowRect( GetControlOwner( m_controlRef ) , &former ) ;
        }
        
        Rect controlBounds = *r ;
        
        // since the rect passed in is always (even in non-compositing) relative
        // to the (native) parent, we have to adjust to window relative here
        wxMacControl* parent = m_peer->GetParent()->GetPeer() ;
        if( parent->m_isRootControl == false )
        {
            Rect superRect ;
            GetControlBounds( parent->m_controlRef , &superRect ) ;
            OffsetRect( &controlBounds , superRect.left , superRect.top ) ;
        }
        
        SetControlBounds( m_controlRef , &controlBounds ) ;
        if ( vis )
        {
            InvalWindowRect( GetControlOwner( m_controlRef ) , &controlBounds ) ;
        }
    }
}

void wxMacControl::GetRect( Rect *r )
{
    GetControlBounds( m_controlRef , r ) ;
    if ( m_isCompositing == false )
    {
        // correct the case of the root control 
        if ( m_isRootControl )
        {
            WindowRef wr = GetControlOwner( m_controlRef ) ;
            GetWindowBounds( wr , kWindowContentRgn , r ) ;
            r->right -= r->left ;
            r->bottom -= r->top ;
            r->left = 0 ;
            r->top = 0 ;
        }
        else 
        {
            wxMacControl* parent = m_peer->GetParent()->GetPeer() ;
            if( parent->m_isRootControl == false )
            {
                Rect superRect ;
                GetControlBounds( parent->m_controlRef , &superRect ) ;
                OffsetRect( r , -superRect.left , -superRect.top ) ;
            }
        }
    }
}

void wxMacControl::GetRectInWindowCoords( Rect *r )
{
    UMAGetControlBoundsInWindowCoords( m_controlRef , r ) ;
}

void wxMacControl::GetBestRect( Rect *r )
{
    short   baselineoffset ;
    GetBestControlRect( m_controlRef , r , &baselineoffset ) ;
}

void wxMacControl::SetTitle( const wxString &title )
{
    wxFontEncoding encoding;

    if ( m_font.Ok() )
        encoding = m_font.GetEncoding();
    else
        encoding = wxFont::GetDefaultEncoding();

    UMASetControlTitle(  m_controlRef , title , encoding ) ;
}

void wxMacControl::GetFeatures( UInt32 * features )
{
    GetControlFeatures( m_controlRef , features ) ;
}

OSStatus wxMacControl::GetRegion( ControlPartCode partCode , RgnHandle region )
{
    OSStatus err = GetControlRegion( m_controlRef , partCode , region ) ;
    if ( m_isCompositing == false )
    {
        if ( !m_isRootControl )
        {
            Rect r ;
            GetControlBounds(m_controlRef, &r ) ;
            if ( !EmptyRgn( region ) )
                OffsetRgn( region , -r.left , -r.top ) ; 
        }
    }
    return err ;
}

OSStatus wxMacControl::SetZOrder( bool above , wxMacControl* other )
{
#if TARGET_API_MAC_OSX
    return HIViewSetZOrder( m_controlRef,above ? kHIViewZOrderAbove : kHIViewZOrderBelow,
       (other != NULL) ? other->m_controlRef : NULL) ;
#else
    return 0 ;
#endif
}


#if TARGET_API_MAC_OSX
// SetNeedsDisplay would not invalidate the children
static void InvalidateControlAndChildren( HIViewRef control )
{
    HIViewSetNeedsDisplay( control , true ) ;
    UInt16 childrenCount = 0 ;
    OSStatus err = CountSubControls( control , &childrenCount ) ;
    if ( err == errControlIsNotEmbedder )
        return ;
    wxASSERT_MSG( err == noErr , wxT("Unexpected error when accessing subcontrols") ) ;

    for ( UInt16 i = childrenCount ; i >=1  ; --i )
    {
        HIViewRef child ;
        err = GetIndexedSubControl( control , i , & child ) ;
        if ( err == errControlIsNotEmbedder )
            return ;
        InvalidateControlAndChildren( child ) ;
    }
}
#endif

void wxMacControl::InvalidateWithChildren()
{
#if TARGET_API_MAC_OSX
    InvalidateControlAndChildren( m_controlRef ) ;
#endif
}

void wxMacControl::ScrollRect( wxRect *r , int dx , int dy ) 
{
	wxASSERT( r != NULL ) ;
#if TARGET_API_MAC_OSX
	if ( m_isCompositing )
	{
		HIRect scrollarea = CGRectMake( r->x , r->y , r->width , r->height) ;
        HIViewScrollRect ( m_controlRef , &scrollarea , dx ,dy ) ;
	}
	else
#endif
	{
		Rect bounds ;
		GetControlBounds( m_controlRef , &bounds ) ;
		bounds.left += r->x ;
		bounds.top += r->y ;
		bounds.bottom = bounds.top + r->height ;
		bounds.right = bounds.left + r->width ;
		wxMacWindowClipper clip( m_peer ) ;
		RgnHandle updateRgn = NewRgn() ;
		::ScrollRect( &bounds , dx , dy , updateRgn ) ;
		InvalWindowRgn( GetControlOwner( m_controlRef )  ,  updateRgn ) ;
	}
}


// SetNeedsDisplay would not invalidate the children

//
// Databrowser
//

OSStatus wxMacControl::SetSelectionFlags( DataBrowserSelectionFlags options )
{
    return SetDataBrowserSelectionFlags( m_controlRef , options ) ;
}

OSStatus wxMacControl::AddListViewColumn( DataBrowserListViewColumnDesc *columnDesc,
        DataBrowserTableViewColumnIndex position )
{
    return AddDataBrowserListViewColumn( m_controlRef , columnDesc, position ) ;
}

OSStatus wxMacControl::AutoSizeListViewColumns()
{
    return AutoSizeDataBrowserListViewColumns(m_controlRef) ;
}

OSStatus wxMacControl::SetHasScrollBars( bool horiz , bool vert )
{
    return SetDataBrowserHasScrollBars( m_controlRef , horiz , vert ) ;
}

OSStatus wxMacControl::SetTableViewHiliteStyle( DataBrowserTableViewHiliteStyle hiliteStyle )
{
    return SetDataBrowserTableViewHiliteStyle( m_controlRef , hiliteStyle ) ;
}

OSStatus wxMacControl::SetListViewHeaderBtnHeight(UInt16 height)
{
    return SetDataBrowserListViewHeaderBtnHeight( m_controlRef ,height ) ;
}

OSStatus wxMacControl::SetCallbacks(const DataBrowserCallbacks *  callbacks)
{
    return SetDataBrowserCallbacks( m_controlRef , callbacks ) ;
}

OSStatus wxMacControl::UpdateItems( DataBrowserItemID container, UInt32 numItems,
        const DataBrowserItemID* items,
        DataBrowserPropertyID preSortProperty,
        DataBrowserPropertyID propertyID )
{
    return UpdateDataBrowserItems( m_controlRef , container, numItems, items, preSortProperty, propertyID ) ;
}

bool wxMacControl::IsItemSelected( DataBrowserItemID item )
{
    return IsDataBrowserItemSelected( m_controlRef , item ) ;
}

OSStatus wxMacControl::AddItems( DataBrowserItemID container, UInt32 numItems,
            const DataBrowserItemID* items,
            DataBrowserPropertyID preSortProperty )
{
    return AddDataBrowserItems( m_controlRef , container, numItems, items, preSortProperty ) ;
}

OSStatus wxMacControl::RemoveItems( DataBrowserItemID container, UInt32 numItems,
            const DataBrowserItemID* items,
            DataBrowserPropertyID preSortProperty )
{
    return RemoveDataBrowserItems( m_controlRef , container, numItems, items, preSortProperty ) ;
}

OSStatus wxMacControl::RevealItem( DataBrowserItemID item,
            DataBrowserPropertyID propertyID,
            DataBrowserRevealOptions options )
{
    return RevealDataBrowserItem( m_controlRef , item , propertyID , options ) ;
}

OSStatus wxMacControl::SetSelectedItems(UInt32 numItems,
            const DataBrowserItemID * items,
            DataBrowserSetOption operation )
{
    return SetDataBrowserSelectedItems( m_controlRef , numItems , items, operation ) ;
}

OSStatus wxMacControl::GetSelectionAnchor( DataBrowserItemID * first, DataBrowserItemID * last )
{
    return GetDataBrowserSelectionAnchor( m_controlRef , first , last ) ;
}

//
// Tab Control
//

OSStatus wxMacControl::SetTabEnabled( SInt16 tabNo , bool enable )
{
    return ::SetTabEnabled( m_controlRef , tabNo , enable ) ;
}

//
// Quartz Support
//

#ifdef __WXMAC_OSX__
// snippets from Sketch Sample from Apple :

#define	kGenericRGBProfilePathStr       "/System/Library/ColorSync/Profiles/Generic RGB Profile.icc"
/*
    This function locates, opens, and returns the profile reference for the calibrated 
    Generic RGB color space. It is up to the caller to call CMCloseProfile when done
    with the profile reference this function returns.
*/
CMProfileRef wxMacOpenGenericProfile(void)
{
    static CMProfileRef cachedRGBProfileRef = NULL;
    
    // we only create the profile reference once
    if (cachedRGBProfileRef == NULL)
    {
		CMProfileLocation 	loc;
	
		loc.locType = cmPathBasedProfile;
		strcpy(loc.u.pathLoc.path, kGenericRGBProfilePathStr);
	
		verify_noerr( CMOpenProfile(&cachedRGBProfileRef, &loc) );
    }

    if (cachedRGBProfileRef)
    {
		// clone the profile reference so that the caller has their own reference, not our cached one
		CMCloneProfileRef(cachedRGBProfileRef);   
    }

    return cachedRGBProfileRef;
}

/*
    Return the generic RGB color space. This is a 'get' function and the caller should
    not release the returned value unless the caller retains it first. Usually callers
    of this routine will immediately use the returned colorspace with CoreGraphics
    so they typically do not need to retain it themselves.
    
    This function creates the generic RGB color space once and hangs onto it so it can
    return it whenever this function is called.
*/

CGColorSpaceRef wxMacGetGenericRGBColorSpace()
{
    static CGColorSpaceRef genericRGBColorSpace = NULL;

	if (genericRGBColorSpace == NULL)
	{
		CMProfileRef genericRGBProfile = wxMacOpenGenericProfile();
	
		if (genericRGBProfile)
		{
			genericRGBColorSpace = CGColorSpaceCreateWithPlatformColorSpace(genericRGBProfile);
			wxASSERT_MSG( genericRGBColorSpace != NULL, wxT("couldn't create the generic RGB color space") ) ;
			
			// we opened the profile so it is up to us to close it
			CMCloseProfile(genericRGBProfile); 
		}
	}
    return genericRGBColorSpace;
}
#endif

#endif // wxUSE_GUI

