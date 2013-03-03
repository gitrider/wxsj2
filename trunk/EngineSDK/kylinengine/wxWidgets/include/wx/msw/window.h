/////////////////////////////////////////////////////////////////////////////
// Name:        wx/msw/window.h
// Purpose:     wxWindow class
// Author:      Julian Smart
// Modified by: Vadim Zeitlin on 13.05.99: complete refont of message handling,
//              elimination of Default(), ...
// Created:     01/02/97
// RCS-ID:      $Id: window.h,v 1.146 2005/05/19 22:17:01 RD Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_WINDOW_H_
#define _WX_WINDOW_H_

// ---------------------------------------------------------------------------
// headers
// ---------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "window.h"
#endif

// [at least] some version of Windows send extra mouse move messages after
// a mouse click or a key press - to temporarily fix this problem, set the
// define below to 1
//
// a better solution should be found later...
#define wxUSE_MOUSEEVENT_HACK 0

// ---------------------------------------------------------------------------
// constants
// ---------------------------------------------------------------------------

#if WXWIN_COMPATIBILITY_2_4
// they're unused by wxWidgets...
enum
{
    wxKEY_SHIFT = 1,
    wxKEY_CTRL  = 2
};
#endif

// ---------------------------------------------------------------------------
// wxWindow declaration for MSW
// ---------------------------------------------------------------------------

class WXDLLEXPORT wxWindowMSW : public wxWindowBase
{
    friend class wxSpinCtrl;
    friend class wxSlider;
    friend class wxRadioBox;
#if defined __VISUALC__ && __VISUALC__ <= 1200
    friend class wxWindowMSW;
#endif
public:
    wxWindowMSW() { Init(); }

    wxWindowMSW(wxWindow *parent,
                wxWindowID id,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxString& name = wxPanelNameStr)
    {
        Init();
        Create(parent, id, pos, size, style, name);
    }

    virtual ~wxWindowMSW();

    bool Create(wxWindow *parent,
                wxWindowID id,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxString& name = wxPanelNameStr);

    // implement base class pure virtuals
    virtual void SetTitle( const wxString& title);
    virtual wxString GetTitle() const;

    virtual void Raise();
    virtual void Lower();

    virtual bool Show( bool show = true );
    virtual bool Enable( bool enable = true );

    virtual void SetFocus();
    virtual void SetFocusFromKbd();

    virtual bool Reparent(wxWindowBase *newParent);

    virtual void WarpPointer(int x, int y);

    virtual void Refresh( bool eraseBackground = true,
                          const wxRect *rect = (const wxRect *) NULL );
    virtual void Update();
    virtual void Freeze();
    virtual void Thaw();

    virtual void SetWindowStyleFlag( long style );
    virtual bool SetCursor( const wxCursor &cursor );
    virtual bool SetFont( const wxFont &font );

    virtual int GetCharHeight() const;
    virtual int GetCharWidth() const;
    virtual void GetTextExtent(const wxString& string,
                               int *x, int *y,
                               int *descent = (int *) NULL,
                               int *externalLeading = (int *) NULL,
                               const wxFont *theFont = (const wxFont *) NULL)
                               const;

#if wxUSE_MENUS_NATIVE
    virtual bool DoPopupMenu( wxMenu *menu, int x, int y );
#endif // wxUSE_MENUS_NATIVE

    virtual void SetScrollbar( int orient, int pos, int thumbVisible,
                               int range, bool refresh = true );
    virtual void SetScrollPos( int orient, int pos, bool refresh = true );
    virtual int GetScrollPos( int orient ) const;
    virtual int GetScrollThumb( int orient ) const;
    virtual int GetScrollRange( int orient ) const;
    virtual void ScrollWindow( int dx, int dy,
                               const wxRect* rect = (wxRect *) NULL );

    virtual bool ScrollLines(int lines);
    virtual bool ScrollPages(int pages);

#if wxUSE_DRAG_AND_DROP
    virtual void SetDropTarget( wxDropTarget *dropTarget );
#endif // wxUSE_DRAG_AND_DROP

    // Accept files for dragging
    virtual void DragAcceptFiles(bool accept);

#if WXWIN_COMPATIBILITY_2_4
    wxDEPRECATED( bool GetUseCtl3D() const );
    wxDEPRECATED( bool GetTransparentBackground() const );
    wxDEPRECATED( void SetTransparent(bool t = true) );
#endif // WXWIN_COMPATIBILITY_2_4

#ifndef __WXUNIVERSAL__
    // Native resource loading (implemented in src/msw/nativdlg.cpp)
    // FIXME: should they really be all virtual?
    virtual bool LoadNativeDialog(wxWindow* parent, wxWindowID& id);
    virtual bool LoadNativeDialog(wxWindow* parent, const wxString& name);
    wxWindow* GetWindowChild1(wxWindowID id);
    wxWindow* GetWindowChild(wxWindowID id);
#endif // __WXUNIVERSAL__

#if wxUSE_HOTKEY
    // install and deinstall a system wide hotkey
    virtual bool RegisterHotKey(int hotkeyId, int modifiers, int keycode);
    virtual bool UnregisterHotKey(int hotkeyId);
#endif // wxUSE_HOTKEY

    // window handle stuff
    // -------------------

    WXHWND GetHWND() const { return m_hWnd; }
    void SetHWND(WXHWND hWnd) { m_hWnd = hWnd; }
    virtual WXWidget GetHandle() const { return GetHWND(); }

    void AssociateHandle(WXWidget handle);
    void DissociateHandle();


    // implementation from now on
    // ==========================

    // event handlers
    // --------------

    void OnPaint(wxPaintEvent& event);
    void OnEraseBackground(wxEraseEvent& event);
#ifdef __WXWINCE__
    void OnInitDialog(wxInitDialogEvent& event);
#endif

public:
    // Windows subclassing
    void SubclassWin(WXHWND hWnd);
    void UnsubclassWin();

    WXFARPROC MSWGetOldWndProc() const { return m_oldWndProc; }
    void MSWSetOldWndProc(WXFARPROC proc) { m_oldWndProc = proc; }

    // return true if the window is of a standard (i.e. not wxWidgets') class
    //
    // to understand why does it work, look at SubclassWin() code and comments
    bool IsOfStandardClass() const { return m_oldWndProc != NULL; }

    wxWindow *FindItem(long id) const;
    wxWindow *FindItemByHWND(WXHWND hWnd, bool controlOnly = false) const;

    // MSW only: true if this control is part of the main control
    virtual bool ContainsHWND(WXHWND WXUNUSED(hWnd)) const { return false; };

    // translate wxWidgets style flags for this control into the Windows style
    // and optional extended style for the corresponding native control
    //
    // this is the function that should be overridden in the derived classes,
    // but you will mostly use MSWGetCreateWindowFlags() below
    virtual WXDWORD MSWGetStyle(long flags, WXDWORD *exstyle = NULL) const ;

    // get the MSW window flags corresponding to wxWidgets ones
    //
    // the functions returns the flags (WS_XXX) directly and puts the ext
    // (WS_EX_XXX) flags into the provided pointer if not NULL
    WXDWORD MSWGetCreateWindowFlags(WXDWORD *exflags = NULL) const
        { return MSWGetStyle(GetWindowStyle(), exflags); }

    // translate wxWidgets coords into Windows ones suitable to be passed to
    // ::CreateWindow()
    //
    // returns true if non default coords are returned, false otherwise
    bool MSWGetCreateWindowCoords(const wxPoint& pos,
                                  const wxSize& size,
                                  int& x, int& y,
                                  int& w, int& h) const;

    // get the HWND to be used as parent of this window with CreateWindow()
    virtual WXHWND MSWGetParent() const;

    // creates the window of specified Windows class with given style, extended
    // style, title and geometry (default values
    //
    // returns true if the window has been created, false if creation failed
    bool MSWCreate(const wxChar *wclass,
                   const wxChar *title = NULL,
                   const wxPoint& pos = wxDefaultPosition,
                   const wxSize& size = wxDefaultSize,
                   WXDWORD style = 0,
                   WXDWORD exendedStyle = 0);

    virtual bool MSWCommand(WXUINT param, WXWORD id);

#ifndef __WXUNIVERSAL__
    // Create an appropriate wxWindow from a HWND
    virtual wxWindow* CreateWindowFromHWND(wxWindow* parent, WXHWND hWnd);

    // Make sure the window style reflects the HWND style (roughly)
    virtual void AdoptAttributesFromHWND();
#endif // __WXUNIVERSAL__

    // Setup background and foreground colours correctly
    virtual void SetupColours();

    // ------------------------------------------------------------------------
    // helpers for message handlers: these perform the same function as the
    // message crackers from <windowsx.h> - they unpack WPARAM and LPARAM into
    // the correct parameters
    // ------------------------------------------------------------------------

    void UnpackCommand(WXWPARAM wParam, WXLPARAM lParam,
                       WXWORD *id, WXHWND *hwnd, WXWORD *cmd);
    void UnpackActivate(WXWPARAM wParam, WXLPARAM lParam,
                        WXWORD *state, WXWORD *minimized, WXHWND *hwnd);
    void UnpackScroll(WXWPARAM wParam, WXLPARAM lParam,
                      WXWORD *code, WXWORD *pos, WXHWND *hwnd);
    void UnpackCtlColor(WXWPARAM wParam, WXLPARAM lParam,
                        WXHDC *hdc, WXHWND *hwnd);
    void UnpackMenuSelect(WXWPARAM wParam, WXLPARAM lParam,
                          WXWORD *item, WXWORD *flags, WXHMENU *hmenu);

    // ------------------------------------------------------------------------
    // internal handlers for MSW messages: all handlers return a boolean value:
    // true means that the handler processed the event and false that it didn't
    // ------------------------------------------------------------------------

    // there are several cases where we have virtual functions for Windows
    // message processing: this is because these messages often require to be
    // processed in a different manner in the derived classes. For all other
    // messages, however, we do *not* have corresponding MSWOnXXX() function
    // and if the derived class wants to process them, it should override
    // MSWWindowProc() directly.

    // scroll event (both horizontal and vertical)
    virtual bool MSWOnScroll(int orientation, WXWORD nSBCode,
                             WXWORD pos, WXHWND control);

    // child control notifications
#ifdef __WIN95__
    virtual bool MSWOnNotify(int idCtrl, WXLPARAM lParam, WXLPARAM *result);
#endif // __WIN95__

    // owner-drawn controls need to process these messages
    virtual bool MSWOnDrawItem(int id, WXDRAWITEMSTRUCT *item);
    virtual bool MSWOnMeasureItem(int id, WXMEASUREITEMSTRUCT *item);

    // the rest are not virtual
    bool HandleCreate(WXLPCREATESTRUCT cs, bool *mayCreate);
    bool HandleInitDialog(WXHWND hWndFocus);
    bool HandleDestroy();

    bool HandlePaint();
    bool HandlePrintClient(WXHDC hDC);
    bool HandleEraseBkgnd(WXHDC hDC);

    bool HandleMinimize();
    bool HandleMaximize();
    bool HandleSize(int x, int y, WXUINT flag);
    bool HandleSizing(wxRect& rect);
    bool HandleGetMinMaxInfo(void *mmInfo);

    bool HandleShow(bool show, int status);
    bool HandleActivate(int flag, bool minimized, WXHWND activate);

    bool HandleCommand(WXWORD id, WXWORD cmd, WXHWND control);

    bool HandleCtlColor(WXHBRUSH *hBrush, WXHDC hdc, WXHWND hWnd);

    bool HandlePaletteChanged(WXHWND hWndPalChange);
    bool HandleQueryNewPalette();
    bool HandleSysColorChange();
    bool HandleDisplayChange();
    bool HandleCaptureChanged(WXHWND gainedCapture);

    bool HandleQueryEndSession(long logOff, bool *mayEnd);
    bool HandleEndSession(bool endSession, long logOff);

    bool HandleSetFocus(WXHWND wnd);
    bool HandleKillFocus(WXHWND wnd);

    bool HandleDropFiles(WXWPARAM wParam);

    bool HandleMouseEvent(WXUINT msg, int x, int y, WXUINT flags);
    bool HandleMouseMove(int x, int y, WXUINT flags);
    bool HandleMouseWheel(WXWPARAM wParam, WXLPARAM lParam);

    bool HandleChar(WXWPARAM wParam, WXLPARAM lParam, bool isASCII = false);
    bool HandleKeyDown(WXWPARAM wParam, WXLPARAM lParam);
    bool HandleKeyUp(WXWPARAM wParam, WXLPARAM lParam);
#if wxUSE_ACCEL
    bool HandleHotKey(WXWPARAM wParam, WXLPARAM lParam);
#endif
#ifdef __WIN32__
    int HandleMenuChar(int chAccel, WXLPARAM lParam);
#endif

    bool HandleQueryDragIcon(WXHICON *hIcon);

    bool HandleSetCursor(WXHWND hWnd, short nHitTest, int mouseMsg);

    // Window procedure
    virtual WXLRESULT MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam);

    // Calls an appropriate default window procedure
    virtual WXLRESULT MSWDefWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam);

    // message processing helpers

    // return false if the message shouldn't be translated/preprocessed but
    // dispatched normally
    virtual bool MSWShouldPreProcessMessage(WXMSG* pMsg);

    // return true if the message was preprocessed and shouldn't be dispatched
    virtual bool MSWProcessMessage(WXMSG* pMsg);

    // return true if the message was translated and shouldn't be dispatched
    virtual bool MSWTranslateMessage(WXMSG* pMsg);

    // called when the window is about to be destroyed
    virtual void MSWDestroyWindow();


    // this function should return the brush to paint the children controls
    // background or 0 if this window doesn't impose any particular background
    // on its children
    //
    // the base class version returns a solid brush if we have a non default
    // background colour or 0 otherwise
    virtual WXHBRUSH MSWGetBgBrushForChild(WXHDC hDC, WXHWND hWnd);

    // return the background brush to use for painting the given window by
    // quering the parent windows via their MSWGetBgBrushForChild() recursively
    //
    // hWndToPaint is normally NULL meaning this window itself, but it can also
    // be a child of this window which is used by the static box and could be
    // potentially useful for other transparent controls
    WXHBRUSH MSWGetBgBrush(WXHDC hDC, WXHWND hWndToPaint = NULL);

    // gives the parent the possibility to draw its children background, e.g.
    // this is used by wxNotebook to do it using DrawThemeBackground()
    //
    // return true if background was drawn, false otherwise
    virtual bool MSWPrintChild(WXHDC WXUNUSED(hDC), wxWindow * WXUNUSED(child))
    {
        return false;
    }


    // Responds to colour changes: passes event on to children.
    void OnSysColourChanged(wxSysColourChangedEvent& event);

    // initialize various fields of wxMouseEvent (common part of MSWOnMouseXXX)
    void InitMouseEvent(wxMouseEvent& event, int x, int y, WXUINT flags);

    // check if mouse is in the window
    bool IsMouseInWindow() const;

    // synthesize a wxEVT_LEAVE_WINDOW event and set m_mouseInWindow to false
    void GenerateMouseLeave();

    // virtual function for implementing internal idle
    // behaviour
    virtual void OnInternalIdle();

protected:
    // the window handle
    WXHWND                m_hWnd;

    // the old window proc (we subclass all windows)
    WXFARPROC             m_oldWndProc;

    // additional (MSW specific) flags
    bool                  m_mouseInWindow:1;
    bool                  m_lastKeydownProcessed:1;

    // the size of one page for scrolling
    int                   m_xThumbSize;
    int                   m_yThumbSize;

#if wxUSE_MOUSEEVENT_HACK
    // the coordinates of the last mouse event and the type of it
    long                  m_lastMouseX,
                          m_lastMouseY;
    int                   m_lastMouseEvent;
#endif // wxUSE_MOUSEEVENT_HACK

    // implement the base class pure virtuals
    virtual void DoClientToScreen( int *x, int *y ) const;
    virtual void DoScreenToClient( int *x, int *y ) const;
    virtual void DoGetPosition( int *x, int *y ) const;
    virtual void DoGetSize( int *width, int *height ) const;
    virtual void DoGetClientSize( int *width, int *height ) const;
    virtual void DoSetSize(int x, int y,
                           int width, int height,
                           int sizeFlags = wxSIZE_AUTO);
    virtual void DoSetClientSize(int width, int height);

    virtual void DoCaptureMouse();
    virtual void DoReleaseMouse();

    // has the window been frozen by Freeze()?
    bool IsFrozen() const { return m_frozenness > 0; }

    // move the window to the specified location and resize it: this is called
    // from both DoSetSize() and DoSetClientSize() and would usually just call
    // ::MoveWindow() except for composite controls which will want to arrange
    // themselves inside the given rectangle
    virtual void DoMoveWindow(int x, int y, int width, int height);

#if wxUSE_TOOLTIPS
    virtual void DoSetToolTip( wxToolTip *tip );

    // process TTN_NEEDTEXT message properly (i.e. fixing the bugs in
    // comctl32.dll in our code -- see the function body for more info)
    bool HandleTooltipNotify(WXUINT code,
                             WXLPARAM lParam,
                             const wxString& ttip);
#endif // wxUSE_TOOLTIPS

    // the helper functions used by HandleChar/KeyXXX methods
    wxKeyEvent CreateKeyEvent(wxEventType evType, int id,
                              WXLPARAM lParam = 0, WXWPARAM wParam = 0) const;


    // default OnEraseBackground() implementation, return true if we did erase
    // the background, false otherwise (i.e. the system should erase it)
    bool DoEraseBackground(WXHDC hDC);

private:
    // common part of all ctors
    void Init();

    // the (non-virtual) handlers for the events
    bool HandleMove(int x, int y);
    bool HandleMoving(wxRect& rect);
    bool HandleJoystickEvent(WXUINT msg, int x, int y, WXUINT flags);

#ifdef __WIN95__
    bool HandleNotify(int idCtrl, WXLPARAM lParam, WXLPARAM *result);
#endif // __WIN95__

    // list of disabled children before last call to our Disable()
    wxWindowList *m_childrenDisabled;

    // number of calls to Freeze() minus number of calls to Thaw()
    unsigned int m_frozenness;

    // current defer window position operation handle (may be NULL)
    WXHANDLE m_hDWP;

    // When deferred positioning is done these hold the pending changes, and
    // are used for the default values if another size/pos changes is done on
    // this window before the group of deferred changes is completed.
    wxPoint     m_pendingPosition;
    wxSize      m_pendingSize;

    DECLARE_DYNAMIC_CLASS(wxWindowMSW)
    DECLARE_NO_COPY_CLASS(wxWindowMSW)
    DECLARE_EVENT_TABLE()
};

// ----------------------------------------------------------------------------
// inline functions
// ----------------------------------------------------------------------------

#if WXWIN_COMPATIBILITY_2_4

inline bool wxWindowMSW::GetUseCtl3D() const { return false; }
inline bool wxWindowMSW::GetTransparentBackground() const { return false; }
inline void wxWindowMSW::SetTransparent(bool WXUNUSED(t)) { }

#endif // WXWIN_COMPATIBILITY_2_4

// ---------------------------------------------------------------------------
// global functions
// ---------------------------------------------------------------------------

// kbd code translation
WXDLLEXPORT int wxCharCodeMSWToWX(int keySym, WXLPARAM lParam = 0);
WXDLLEXPORT WXWORD wxCharCodeWXToMSW(int id, bool *IsVirtual);

// window creation helper class: before creating a new HWND, instantiate an
// object of this class on stack - this allows to process the messages sent to
// the window even before CreateWindow() returns
class wxWindowCreationHook
{
public:
    wxWindowCreationHook(wxWindowMSW *winBeingCreated);
    ~wxWindowCreationHook();
};

// ----------------------------------------------------------------------------
// global objects
// ----------------------------------------------------------------------------

// notice that this hash must be defined after wxWindow declaration as it
// needs to "see" its dtor and not just forward declaration
#include "wx/hash.h"

// pseudo-template HWND <-> wxWindow hash table
#if WXWIN_COMPATIBILITY_2_4
WX_DECLARE_HASH(wxWindow, wxWindowList, wxWinHashTable);
#else
WX_DECLARE_HASH(wxWindowMSW, wxWindowList, wxWinHashTable);
#endif

extern wxWinHashTable *wxWinHandleHash;

#endif
    // _WX_WINDOW_H_
