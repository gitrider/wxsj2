/////////////////////////////////////////////////////////////////////////////
// Name:        dialog.h
// Purpose:     wxDialog class
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id: dialog.h,v 1.24 2005/02/06 17:38:20 MBN Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_DIALOG_H_
#define _WX_DIALOG_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "dialog.h"
#endif

class WXDLLEXPORT wxEventLoop;

// Dialog boxes
class WXDLLEXPORT wxDialog : public wxDialogBase
{
    DECLARE_DYNAMIC_CLASS(wxDialog)

public:
    wxDialog();

    // Constructor with no modal flag - the new convention.
    wxDialog(wxWindow *parent, wxWindowID id,
        const wxString& title,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_DIALOG_STYLE,
        const wxString& name = wxDialogNameStr)
    {
        Create(parent, id, title, pos, size, style, name);
    }

    bool Create(wxWindow *parent, wxWindowID id,
        const wxString& title,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_DIALOG_STYLE,
        const wxString& name = wxDialogNameStr);

    ~wxDialog();

    virtual bool Destroy();

    virtual bool Show(bool show = true);

    void SetTitle(const wxString& title);

    void SetModal(bool flag);

    virtual bool IsModal() const
    { return ((GetWindowStyleFlag() & wxDIALOG_MODAL) == wxDIALOG_MODAL); }

    virtual int ShowModal();
    virtual void EndModal(int retCode);

    // Implementation
    virtual void ChangeFont(bool keepOriginalSize = true);
    virtual void ChangeBackgroundColour();
    virtual void ChangeForegroundColour();
    inline WXWidget GetTopWidget() const { return m_mainWidget; }
    inline WXWidget GetClientWidget() const { return m_mainWidget; }

    // Standard buttons
    void OnOK(wxCommandEvent& event);
    void OnApply(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);

    // Responds to colour changes
    void OnSysColourChanged(wxSysColourChangedEvent& event);

    void OnCharHook(wxKeyEvent& event);
    void OnCloseWindow(wxCloseEvent& event);

private:
    virtual bool DoCreate( wxWindow* parent, wxWindowID id,
                           const wxString& title,
                           const wxPoint& pos,
                           const wxSize& size,
                           long style,
                           const wxString& name );
    virtual void DoDestroy();

    //// Motif-specific
    bool          m_modalShowing;
    wxEventLoop*  m_eventLoop;

protected:
    virtual void DoSetSize(int x, int y,
        int width, int height,
        int sizeFlags = wxSIZE_AUTO);

    virtual void DoSetClientSize(int width, int height);

private:
    DECLARE_EVENT_TABLE()
};

#endif
// _WX_DIALOG_H_
