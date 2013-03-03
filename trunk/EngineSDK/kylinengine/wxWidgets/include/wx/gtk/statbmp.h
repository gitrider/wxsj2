/////////////////////////////////////////////////////////////////////////////
// Name:        wx/gtk/statbmp.h
// Purpose:
// Author:      Robert Roebling
// Id:          $Id: statbmp.h,v 1.19 2004/08/21 22:41:04 VS Exp $
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#ifndef __GTKSTATICBITMAPH__
#define __GTKSTATICBITMAPH__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface
#endif

#include "wx/icon.h"

//-----------------------------------------------------------------------------
// wxStaticBitmap
//-----------------------------------------------------------------------------

class wxStaticBitmap : public wxStaticBitmapBase
{
public:
    wxStaticBitmap();
    wxStaticBitmap( wxWindow *parent,
                    wxWindowID id,
                    const wxBitmap& label,
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxDefaultSize,
                    long style = 0,
                    const wxString& name = wxStaticBitmapNameStr );
    bool Create( wxWindow *parent,
                 wxWindowID id,
                 const wxBitmap& label,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = 0,
                 const wxString& name = wxStaticBitmapNameStr);

    virtual void SetIcon(const wxIcon& icon) { SetBitmap( icon ); }
    virtual void SetBitmap( const wxBitmap& bitmap );
    virtual wxBitmap GetBitmap() const { return m_bitmap; }

    // for compatibility with wxMSW
    wxIcon GetIcon() const
    {
        // don't use wxDynamicCast, icons and bitmaps are really the same thing
        // in wxGTK
        return (const wxIcon &)m_bitmap;
    }

    static wxVisualAttributes
    GetClassDefaultAttributes(wxWindowVariant variant = wxWINDOW_VARIANT_NORMAL);
    
private:
    wxBitmap   m_bitmap;

    DECLARE_DYNAMIC_CLASS(wxStaticBitmap)
};

#endif // __GTKSTATICBITMAPH__
