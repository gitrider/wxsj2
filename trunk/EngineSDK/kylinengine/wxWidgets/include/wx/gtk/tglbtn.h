/////////////////////////////////////////////////////////////////////////////
// Name:        wx/gtk/tglbtn.h
// Purpose:     Declaration of the wxToggleButton class, which implements a
//              toggle button under wxGTK.
// Author:      John Norris, minor changes by Axel Schlueter
// Modified by:
// Created:     08.02.01
// RCS-ID:      $Id: tglbtn.h,v 1.7 2004/11/22 15:55:28 JS Exp $
// Copyright:   (c) 2000 Johnny C. Norris II
// License:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_GTK_TOGGLEBUTTON_H_
#define _WX_GTK_TOGGLEBUTTON_H_

#include "wx/bitmap.h"

//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------

class wxToggleButton;
class wxToggleBitmapButton;

//-----------------------------------------------------------------------------
// global data
//-----------------------------------------------------------------------------

extern const wxChar *wxCheckBoxNameStr;

//-----------------------------------------------------------------------------
// wxToggleBitmapButton
//-----------------------------------------------------------------------------

class wxToggleBitmapButton: public wxControl
{
public:
    // construction/destruction
    wxToggleBitmapButton() {}
    wxToggleBitmapButton(wxWindow *parent,
                   wxWindowID id,
                   const wxBitmap& label,
                   const wxPoint& pos = wxDefaultPosition,
                   const wxSize& size = wxDefaultSize,
                   long style = 0,
                   const wxValidator& validator = wxDefaultValidator,
                   const wxString& name = wxCheckBoxNameStr)
    {
        Create(parent, id, label, pos, size, style, validator, name);
    }

    // Create the control
    bool Create(wxWindow *parent,
                wxWindowID id,
                const wxBitmap& label,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize, long style = 0,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = wxCheckBoxNameStr);

    // Get/set the value
    void SetValue(bool state);
    bool GetValue() const;

    // Set the label
    void SetLabel(const wxBitmap& label);
    bool Enable(bool enable = TRUE);

    static wxVisualAttributes
    GetClassDefaultAttributes(wxWindowVariant variant = wxWINDOW_VARIANT_NORMAL);

    // implementation
    bool      m_blockEvent;
    wxBitmap  m_bitmap;

    void OnSetBitmap();
    void DoApplyWidgetStyle(GtkRcStyle *style);
    bool IsOwnGtkWindow(GdkWindow *window);

    virtual void OnInternalIdle();
    virtual wxSize DoGetBestSize() const;

private:
    DECLARE_DYNAMIC_CLASS(wxToggleBitmapButton)
};

//-----------------------------------------------------------------------------
// wxToggleButton
//-----------------------------------------------------------------------------

class wxToggleButton: public wxControl
{
public:
    // construction/destruction
    wxToggleButton() {}
    wxToggleButton(wxWindow *parent,
                   wxWindowID id,
                   const wxString& label,
                   const wxPoint& pos = wxDefaultPosition,
                   const wxSize& size = wxDefaultSize,
                   long style = 0,
                   const wxValidator& validator = wxDefaultValidator,
                   const wxString& name = wxCheckBoxNameStr)
    {
        Create(parent, id, label, pos, size, style, validator, name);
    }

    // Create the control
    bool Create(wxWindow *parent,
                wxWindowID id,
                const wxString& label,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize, long style = 0,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = wxCheckBoxNameStr);

    // Get/set the value
    void SetValue(bool state);
    bool GetValue() const;

    // Set the label
    void SetLabel(const wxString& label);
    bool Enable(bool enable = TRUE);

    static wxVisualAttributes
    GetClassDefaultAttributes(wxWindowVariant variant = wxWINDOW_VARIANT_NORMAL);
 
    // implementation
    bool m_blockEvent;

    void DoApplyWidgetStyle(GtkRcStyle *style);
    bool IsOwnGtkWindow(GdkWindow *window);

    virtual void OnInternalIdle();
    virtual wxSize DoGetBestSize() const;

private:
    DECLARE_DYNAMIC_CLASS(wxToggleButton)
};

#endif // _WX_GTK_TOGGLEBUTTON_H_

