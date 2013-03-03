/////////////////////////////////////////////////////////////////////////////
// Name:        wx/gtk/bmpbutton.h
// Purpose:
// Author:      Robert Roebling
// Id:          $Id: bmpbuttn.h,v 1.29 2004/10/18 13:12:39 VS Exp $
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#ifndef __BMPBUTTONH__
#define __BMPBUTTONH__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface
#endif

// ----------------------------------------------------------------------------
// wxBitmapButton
// ----------------------------------------------------------------------------

class wxBitmapButton: public wxBitmapButtonBase
{
public:
    wxBitmapButton() { Init(); }

    wxBitmapButton(wxWindow *parent,
                   wxWindowID id,
                   const wxBitmap& bitmap,
                   const wxPoint& pos = wxDefaultPosition,
                   const wxSize& size = wxDefaultSize,
                   long style = wxBU_AUTODRAW,
                   const wxValidator& validator = wxDefaultValidator,
                   const wxString& name = wxButtonNameStr)
    {
        Init();

        Create(parent, id, bitmap, pos, size, style, validator, name);
    }

    bool Create(wxWindow *parent,
                wxWindowID id,
                const wxBitmap& bitmap,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxBU_AUTODRAW,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = wxButtonNameStr);

    void SetLabel( const wxString &label );
    wxString GetLabel() const;
    virtual void SetLabel( const wxBitmap& bitmap ) { SetBitmapLabel(bitmap); }

    virtual void SetDefault();
    virtual bool Enable(bool enable = TRUE);

    // implementation
    // --------------

    void HasFocus();
    void NotFocus();
    void StartSelect();
    void EndSelect();
    void DoApplyWidgetStyle(GtkRcStyle *style);

    bool         m_hasFocus:1;
    bool         m_isSelected:1;

protected:
    virtual void OnSetBitmap();
    virtual wxSize DoGetBestSize() const;

    void Init();

private:
    DECLARE_DYNAMIC_CLASS(wxBitmapButton)
};

#endif // __BMPBUTTONH__
