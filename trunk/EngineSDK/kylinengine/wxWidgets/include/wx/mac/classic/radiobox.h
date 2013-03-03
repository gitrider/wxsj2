/////////////////////////////////////////////////////////////////////////////
// Name:        radiobox.h
// Purpose:     wxRadioBox class
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// RCS-ID:      $Id: radiobox.h,v 1.5 2005/02/23 17:04:14 ABX Exp $
// Copyright:   (c) Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_RADIOBOX_H_
#define _WX_RADIOBOX_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "radiobox.h"
#endif

// List box item
class WXDLLEXPORT wxBitmap ;

class WXDLLEXPORT wxRadioButton ;

class WXDLLEXPORT wxRadioBox: public wxControl, public wxRadioBoxBase
{
    DECLARE_DYNAMIC_CLASS(wxRadioBox)
public:
// Constructors & destructor
    wxRadioBox();
    inline wxRadioBox(wxWindow *parent, wxWindowID id, const wxString& title,
             const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
             int n = 0, const wxString choices[] = NULL,
             int majorDim = 0, long style = wxRA_HORIZONTAL,
             const wxValidator& val = wxDefaultValidator, const wxString& name = wxRadioBoxNameStr)
        {
            Create(parent, id, title, pos, size, n, choices, majorDim, style, val, name);
        }
    inline wxRadioBox(wxWindow *parent, wxWindowID id, const wxString& title,
             const wxPoint& pos, const wxSize& size,
             const wxArrayString& choices,
             int majorDim = 0, long style = wxRA_HORIZONTAL,
             const wxValidator& val = wxDefaultValidator,
             const wxString& name = wxRadioBoxNameStr)
     {
         Create(parent, id, title, pos, size, choices,
                majorDim, style, val, name);
     }
    ~wxRadioBox();
    bool Create(wxWindow *parent, wxWindowID id, const wxString& title,
             const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
             int n = 0, const wxString choices[] = NULL,
             int majorDim = 0, long style = wxRA_HORIZONTAL,
             const wxValidator& val = wxDefaultValidator, const wxString& name = wxRadioBoxNameStr);
    bool Create(wxWindow *parent, wxWindowID id, const wxString& title,
             const wxPoint& pos, const wxSize& size,
             const wxArrayString& choices,
             int majorDim = 0, long style = wxRA_HORIZONTAL,
             const wxValidator& val = wxDefaultValidator,
             const wxString& name = wxRadioBoxNameStr);

// Specific functions (in wxWidgets2 reference)
    virtual void SetSelection(int item);
    virtual int GetSelection() const;

    inline virtual int GetCount() const { return m_noItems; } ;

    virtual wxString GetString(int item) const;
    virtual void SetString(int item, const wxString& label) ;

    virtual bool Enable(int item, bool enable = true);
    virtual bool Show(int item, bool show = true);

    virtual int GetColumnCount() const ;
    virtual int GetRowCount() const ;

    virtual bool Enable(bool enable = true);
    virtual wxString GetLabel() const;
    virtual void SetLabel(const wxString& label) ;
    virtual bool Show(bool show = true);

// Other external functions
    void Command(wxCommandEvent& event);
    void SetFocus();

// Other variable access functions
    inline int GetNumberOfRowsOrCols() const { return m_noRowsOrCols; }
    inline void SetNumberOfRowsOrCols(int n) { m_noRowsOrCols = n; }

    void OnRadioButton( wxCommandEvent& event ) ;
protected:
    wxRadioButton        *m_radioButtonCycle;

    int               m_majorDim ;
    int               m_noItems;
    int               m_noRowsOrCols;

// Internal functions
    virtual wxSize DoGetBestSize() const ;
    virtual void DoSetSize(int x, int y,
                           int width, int height,
                           int sizeFlags = wxSIZE_AUTO);

    DECLARE_EVENT_TABLE()
};

#endif
    // _WX_RADIOBOX_H_
