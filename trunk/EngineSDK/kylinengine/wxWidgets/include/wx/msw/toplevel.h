///////////////////////////////////////////////////////////////////////////////
// Name:        wx/msw/toplevel.h
// Purpose:     wxTopLevelWindowMSW is the MSW implementation of wxTLW
// Author:      Vadim Zeitlin
// Modified by:
// Created:     20.09.01
// RCS-ID:      $Id: toplevel.h,v 1.32 2005/03/19 12:06:49 JS Exp $
// Copyright:   (c) 2001 SciTech Software, Inc. (www.scitechsoft.com)
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_MSW_TOPLEVEL_H_
#define _WX_MSW_TOPLEVEL_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "toplevel.h"
#endif

// ----------------------------------------------------------------------------
// wxTopLevelWindowMSW
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxTopLevelWindowMSW : public wxTopLevelWindowBase
{
public:
    // constructors and such
    wxTopLevelWindowMSW() { Init(); }

    wxTopLevelWindowMSW(wxWindow *parent,
                        wxWindowID id,
                        const wxString& title,
                        const wxPoint& pos = wxDefaultPosition,
                        const wxSize& size = wxDefaultSize,
                        long style = wxDEFAULT_FRAME_STYLE,
                        const wxString& name = wxFrameNameStr)
    {
        Init();

        (void)Create(parent, id, title, pos, size, style, name);
    }

    bool Create(wxWindow *parent,
                wxWindowID id,
                const wxString& title,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxDEFAULT_FRAME_STYLE,
                const wxString& name = wxFrameNameStr);

    virtual ~wxTopLevelWindowMSW();

    // implement base class pure virtuals
    virtual void Maximize(bool maximize = true);
    virtual bool IsMaximized() const;
    virtual void Iconize(bool iconize = true);
    virtual bool IsIconized() const;
    virtual void SetIcon(const wxIcon& icon);
    virtual void SetIcons(const wxIconBundle& icons );
    virtual void Restore();

#ifndef __WXWINCE__
    virtual bool SetShape(const wxRegion& region);
#endif // __WXWINCE__
    virtual void RequestUserAttention(int flags = wxUSER_ATTENTION_INFO);

    virtual bool Show(bool show = true);

    virtual bool ShowFullScreen(bool show, long style = wxFULLSCREEN_ALL);
    virtual bool IsFullScreen() const { return m_fsIsShowing; }

    // wxMSW only: EnableCloseButton(false) may be used to remove the "Close"
    // button from the title bar
    bool EnableCloseButton(bool enable = true);

    // implementation from now on
    // --------------------------

    // event handlers
    void OnActivate(wxActivateEvent& event);

    // called by wxWindow whenever it gets focus
    void SetLastFocus(wxWindow *win) { m_winLastFocused = win; }
    wxWindow *GetLastFocus() const { return m_winLastFocused; }

#if defined(__SMARTPHONE__) && defined(__WXWINCE__)
    virtual void SetLeftMenu(int id = wxID_ANY, const wxString& label = wxEmptyString, wxMenu *subMenu = NULL);
    virtual void SetRightMenu(int id = wxID_ANY, const wxString& label = wxEmptyString, wxMenu *subMenu = NULL);
    bool HandleCommand(WXWORD id, WXWORD cmd, WXHWND control);
#endif // __SMARTPHONE__ && __WXWINCE__

protected:
    // common part of all ctors
    void Init();

    // create a new frame, return false if it couldn't be created
    bool CreateFrame(const wxString& title,
                     const wxPoint& pos,
                     const wxSize& size);

    // create a new dialog using the given dialog template from resources,
    // return false if it couldn't be created
    bool CreateDialog(const void *dlgTemplate,
                      const wxString& title,
                      const wxPoint& pos,
                      const wxSize& size);

    // common part of Iconize(), Maximize() and Restore()
    void DoShowWindow(int nShowCmd);

    // translate wxWidgets flags to Windows ones
    virtual WXDWORD MSWGetStyle(long flags, WXDWORD *exstyle) const;

    // choose the right parent to use with CreateWindow()
    virtual WXHWND MSWGetParent() const;

    // is the window currently iconized?
    bool m_iconized;

    // should the frame be maximized when it will be shown? set by Maximize()
    // when it is called while the frame is hidden
    bool m_maximizeOnShow;

    // Data to save/restore when calling ShowFullScreen
    long                  m_fsStyle; // Passed to ShowFullScreen
    wxRect                m_fsOldSize;
    long                  m_fsOldWindowStyle;
    bool                  m_fsIsMaximized;
    bool                  m_fsIsShowing;

    // the last focused child: we restore focus to it on activation
    wxWindow             *m_winLastFocused;

#if defined(__SMARTPHONE__) && defined(__WXWINCE__)
    class ButtonMenu
    {
    public:
        ButtonMenu();
        ~ButtonMenu();

        void SetButton(int id = wxID_ANY,
                       const wxString& label  = wxEmptyString,
                       wxMenu *subMenu = NULL);

        bool IsAssigned() const {return m_assigned;}
        bool IsMenu() const {return m_menu!=NULL;}

        int GetId() const {return m_id;}
        wxMenu* GetMenu() const {return m_menu;}
        wxString GetLabel() {return m_label;}

        static wxMenu *DuplicateMenu(wxMenu *menu);

    protected:
        int      m_id;
        wxString m_label;
        wxMenu  *m_menu;
        bool     m_assigned;
    };

    ButtonMenu               m_LeftButton;
    ButtonMenu               m_RightButton;
    HWND                     m_MenuBarHWND;

    void ReloadButton(ButtonMenu& button, UINT menuID);
    void ReloadAllButtons();
#endif // __SMARTPHONE__ && __WXWINCE__

    DECLARE_EVENT_TABLE()
    DECLARE_NO_COPY_CLASS(wxTopLevelWindowMSW)
};

#endif // _WX_MSW_TOPLEVEL_H_

