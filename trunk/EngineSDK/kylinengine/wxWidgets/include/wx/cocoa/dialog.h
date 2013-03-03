/////////////////////////////////////////////////////////////////////////////
// Name:        wx/cocoa/dialog.h
// Purpose:     wxDialog class
// Author:      David Elliott
// Modified by:
// Created:     2002/12/15
// RCS-ID:      $Id:
// Copyright:   David Elliott
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_COCOA_DIALOG_H_
#define _WX_COCOA_DIALOG_H_

#include "wx/defs.h"
// NOTE: we don't need panel.h, but other things expect it to be included
#include "wx/panel.h"
#include "wx/cocoa/NSPanel.h"

WXDLLEXPORT_DATA(extern const wxChar*) wxDialogNameStr;

// ========================================================================
// wxDialog
// ========================================================================
class WXDLLEXPORT wxDialog : public wxDialogBase, protected wxCocoaNSPanel
{
    DECLARE_DYNAMIC_CLASS(wxDialog)
    DECLARE_EVENT_TABLE()
    WX_DECLARE_COCOA_OWNER(NSPanel,NSWindow,NSWindow)
// ------------------------------------------------------------------------
// initialization
// ------------------------------------------------------------------------
public:
    wxDialog() { Init(); }

    // Constructor with a modal flag, but no window id - the old convention
    wxDialog(wxWindow *parent,
            const wxString& title, bool modal,
            int x = -1, int y= -1, int width = 500, int height = 500,
            long style = wxDEFAULT_DIALOG_STYLE,
            const wxString& name = wxDialogNameStr)
    {
        Init();
        long modalStyle = modal ? wxDIALOG_MODAL : wxDIALOG_MODELESS ;
        Create(parent, -1, title, wxPoint(x, y), wxSize(width, height),
               style | modalStyle, name);
    }

    // Constructor with no modal flag - the new convention.
    wxDialog(wxWindow *parent, wxWindowID winid,
             const wxString& title,
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize,
             long style = wxDEFAULT_DIALOG_STYLE,
             const wxString& name = wxDialogNameStr)
    {
        Init();
        Create(parent, winid, title, pos, size, style, name);
    }

    bool Create(wxWindow *parent, wxWindowID winid,
                const wxString& title,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxDEFAULT_DIALOG_STYLE,
                const wxString& name = wxDialogNameStr);

    ~wxDialog();
protected:
    void Init();

// ------------------------------------------------------------------------
// Cocoa specifics
// ------------------------------------------------------------------------
protected:
    virtual void CocoaDelegate_windowWillClose(void);
    virtual bool Cocoa_canBecomeMainWindow(bool &canBecome)
    {   canBecome = true; return true; }

// ------------------------------------------------------------------------
// Implementation
// ------------------------------------------------------------------------
public:
    virtual bool Show(bool show = true);

    void SetModal(bool flag);
    virtual bool IsModal() const { return m_isModal; }
    bool m_isModal;

    // For now, same as Show(TRUE) but returns return code
    virtual int ShowModal();

    // may be called to terminate the dialog with the given return code
    virtual void EndModal(int retCode);

// ------------------------------------------------------------------------
// Event handlers
// ------------------------------------------------------------------------
protected:
    void OnCloseWindow(wxCloseEvent& event);
    // Standard buttons
    void OnOK(wxCommandEvent& event);
    void OnApply(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);
};

#endif // _WX_COCOA_DIALOG_H_
