/////////////////////////////////////////////////////////////////////////////
// Name:        wx/msw/combobox.h
// Purpose:     wxComboBox class
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id: combobox.h,v 1.33 2005/03/18 19:32:14 ABX Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_COMBOBOX_H_
#define _WX_COMBOBOX_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "combobox.h"
#endif

#include "wx/choice.h"

#if wxUSE_COMBOBOX

// ----------------------------------------------------------------------------
// Combobox control
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxComboBox: public wxChoice
{
public:
    wxComboBox() { Init(); }

    wxComboBox(wxWindow *parent, wxWindowID id,
            const wxString& value = wxEmptyString,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            int n = 0, const wxString choices[] = NULL,
            long style = 0,
            const wxValidator& validator = wxDefaultValidator,
            const wxString& name = wxComboBoxNameStr)
    {
        Init();

        Create(parent, id, value, pos, size, n, choices, style, validator, name);
    }
    wxComboBox(wxWindow *parent, wxWindowID id,
            const wxString& value,
            const wxPoint& pos,
            const wxSize& size,
            const wxArrayString& choices,
            long style = 0,
            const wxValidator& validator = wxDefaultValidator,
            const wxString& name = wxComboBoxNameStr)
    {
        Init();

        Create(parent, id, value, pos, size, choices, style, validator, name);
    }

    bool Create(wxWindow *parent,
                wxWindowID id,
                const wxString& value = wxEmptyString,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                int n = 0,
                const wxString choices[] = NULL,
                long style = 0,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = wxComboBoxNameStr);
    bool Create(wxWindow *parent,
                wxWindowID id,
                const wxString& value,
                const wxPoint& pos,
                const wxSize& size,
                const wxArrayString& choices,
                long style = 0,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = wxComboBoxNameStr);

    // List functions: see wxChoice

    // Text field functions
    wxString GetValue() const { return m_value; }
    virtual void SetValue(const wxString& value);

    // Clipboard operations
    virtual void Copy();
    virtual void Cut();
    virtual void Paste();
    virtual bool CanCopy() const;
    virtual bool CanCut() const;
    virtual bool CanPaste() const;
    virtual void SetInsertionPoint(long pos);
    virtual void SetInsertionPointEnd();
    virtual long GetInsertionPoint() const;
    virtual wxTextPos GetLastPosition() const;
    virtual void Replace(long from, long to, const wxString& value);
    virtual void Remove(long from, long to);
    virtual void SetSelection(int n) { wxChoice::SetSelection(n); }
    virtual void SetSelection(long from, long to);
    virtual int GetSelection() const;
    virtual void GetSelection(long* from, long* to) const;
    virtual void SetEditable(bool editable);
    virtual void Clear() { wxChoice::Clear(); m_selectionOld = -1; }

    virtual void Undo() ;
    virtual void Redo() ;
    virtual bool CanUndo() const;
    virtual bool CanRedo() const;
    virtual void SelectAll();
    virtual bool IsEditable() const ;
    virtual bool HasSelection() const;

    // implementation only from now on
    virtual bool MSWCommand(WXUINT param, WXWORD id);
    bool MSWProcessEditMsg(WXUINT msg, WXWPARAM wParam, WXLPARAM lParam);
    virtual WXLRESULT MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam);

    WXHWND GetEditHWND() const;

    // Standard event handling
    void OnCut(wxCommandEvent& event);
    void OnCopy(wxCommandEvent& event);
    void OnPaste(wxCommandEvent& event);
    void OnUndo(wxCommandEvent& event);
    void OnRedo(wxCommandEvent& event);
    void OnDelete(wxCommandEvent& event);
    void OnSelectAll(wxCommandEvent& event);

    void OnUpdateCut(wxUpdateUIEvent& event);
    void OnUpdateCopy(wxUpdateUIEvent& event);
    void OnUpdatePaste(wxUpdateUIEvent& event);
    void OnUpdateUndo(wxUpdateUIEvent& event);
    void OnUpdateRedo(wxUpdateUIEvent& event);
    void OnUpdateDelete(wxUpdateUIEvent& event);
    void OnUpdateSelectAll(wxUpdateUIEvent& event);

protected:
    virtual WXDWORD MSWGetStyle(long style, WXDWORD *exstyle) const;

    // common part of all ctors
    void Init() { m_selectionOld = -1; }


    // the previous selection (see MSWCommand() to see why it is needed)
    int m_selectionOld;

    // the current selection (also see MSWCommand())
    wxString m_value;

private:
    DECLARE_DYNAMIC_CLASS_NO_COPY(wxComboBox)
    DECLARE_EVENT_TABLE()
};

#endif // wxUSE_COMBOBOX
#endif
    // _WX_COMBOBOX_H_
