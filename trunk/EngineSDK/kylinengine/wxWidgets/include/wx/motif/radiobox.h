/////////////////////////////////////////////////////////////////////////////
// Name:        radiobox.h
// Purpose:     wxRadioBox class
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id: radiobox.h,v 1.26 2005/02/23 16:35:00 ABX Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_MOTIF_RADIOBOX_H_
#define _WX_MOTIF_RADIOBOX_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "radiobox.h"
#endif

#ifndef wxWIDGET_ARRAY_DEFINED
    #define wxWIDGET_ARRAY_DEFINED

    #include "wx/dynarray.h"
    WX_DEFINE_ARRAY_PTR(WXWidget, wxWidgetArray);
#endif // wxWIDGET_ARRAY_DEFINED

#include "wx/arrstr.h"

class WXDLLEXPORT wxRadioBox : public wxControl, public wxRadioBoxBase
{
public:
    wxRadioBox() { Init(); }

    wxRadioBox(wxWindow *parent, wxWindowID id, const wxString& title,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               int n = 0, const wxString choices[] = NULL,
               int majorDim = 0, long style = wxRA_HORIZONTAL,
               const wxValidator& val = wxDefaultValidator,
               const wxString& name = wxRadioBoxNameStr)
    {
        Init();

        Create(parent, id, title, pos, size, n, choices,
               majorDim, style, val, name);
    }

    wxRadioBox(wxWindow *parent, wxWindowID id, const wxString& title,
               const wxPoint& pos,
               const wxSize& size,
               const wxArrayString& choices,
               int majorDim = 0, long style = wxRA_HORIZONTAL,
               const wxValidator& val = wxDefaultValidator,
               const wxString& name = wxRadioBoxNameStr)
    {
        Init();

        Create(parent, id, title, pos, size, choices,
               majorDim, style, val, name);
    }

    ~wxRadioBox();

    bool Create(wxWindow *parent, wxWindowID id, const wxString& title,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                int n = 0, const wxString choices[] = NULL,
                int majorDim = 0, long style = wxRA_HORIZONTAL,
                const wxValidator& val = wxDefaultValidator,
                const wxString& name = wxRadioBoxNameStr);

    bool Create(wxWindow *parent, wxWindowID id, const wxString& title,
                const wxPoint& pos,
                const wxSize& size,
                const wxArrayString& choices,
                int majorDim = 0, long style = wxRA_HORIZONTAL,
                const wxValidator& val = wxDefaultValidator,
                const wxString& name = wxRadioBoxNameStr);

    int FindString(const wxString& s) const;
    void SetSelection(int N);
    int GetSelection() const;

    void SetString(int item, const wxString& label) ;
    wxString GetString(int item) const;
    virtual bool Enable(bool enable = true);
    virtual bool Enable(int item, bool enable = true);
    virtual bool Show(int item, bool show = true);
    virtual bool Show(bool show = true);

    virtual wxString GetStringSelection() const;
    virtual bool SetStringSelection(const wxString& s);
    virtual int GetCount() const { return m_noItems; } ;
    void Command(wxCommandEvent& event);

    int GetColumnCount() const;
    int GetRowCount() const;

    int GetNumberOfRowsOrCols() const { return m_noRowsOrCols; }
    void SetNumberOfRowsOrCols(int n) { m_noRowsOrCols = n; }

    // Implementation
    virtual void ChangeFont(bool keepOriginalSize = true);
    virtual void ChangeBackgroundColour();
    virtual void ChangeForegroundColour();
    const wxWidgetArray& GetRadioButtons() const { return m_radioButtons; }
    void SetSel(int i) { m_selectedButton = i; }
    virtual WXWidget GetLabelWidget() const { return m_labelWidget; }

protected:
    virtual void DoSetSize(int x, int y,
                           int width, int height,
                           int sizeFlags = wxSIZE_AUTO);

    int               m_majorDim;
    int               m_noItems;
    int               m_noRowsOrCols;
    int               m_selectedButton;

    wxWidgetArray     m_radioButtons;
    WXWidget          m_labelWidget;
    wxArrayString     m_radioButtonLabels;

private:
    void Init();

    DECLARE_DYNAMIC_CLASS(wxRadioBox)
};

#endif // _WX_MOTIF_RADIOBOX_H_

