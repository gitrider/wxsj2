/////////////////////////////////////////////////////////////////////////////
// Name:        wx/msw/statbmp.h
// Purpose:     wxStaticBitmap class for wxMSW
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id: statbmp.h,v 1.33 2005/02/24 08:26:46 JS Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_STATBMP_H_
#define _WX_STATBMP_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "statbmp.h"
#endif

#include "wx/control.h"
#include "wx/icon.h"
#include "wx/bitmap.h"

extern WXDLLEXPORT_DATA(const wxChar*) wxStaticBitmapNameStr;

// a control showing an icon or a bitmap
class WXDLLEXPORT wxStaticBitmap : public wxStaticBitmapBase
{
public:
    wxStaticBitmap() { Init(); }

    wxStaticBitmap(wxWindow *parent,
                   wxWindowID id,
                   const wxGDIImage& label,
                   const wxPoint& pos = wxDefaultPosition,
                   const wxSize& size = wxDefaultSize,
                   long style = 0,
                   const wxString& name = wxStaticBitmapNameStr)
    {
        Init();

        Create(parent, id, label, pos, size, style, name);
    }

    bool Create(wxWindow *parent,
                wxWindowID id,
                const wxGDIImage& label,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxString& name = wxStaticBitmapNameStr);

    virtual ~wxStaticBitmap() { Free(); }

    virtual void SetIcon(const wxIcon& icon) { SetImage(&icon); }
    virtual void SetBitmap(const wxBitmap& bitmap) { SetImage(&bitmap); }

    // assert failure is provoked by an attempt to get an icon from bitmap or
    // vice versa
    wxIcon GetIcon() const
    {
        wxASSERT_MSG( m_isIcon, _T("no icon in this wxStaticBitmap") );

        return *(wxIcon *)m_image;
    }

    wxBitmap GetBitmap() const
    {
        wxASSERT_MSG( !m_isIcon, _T("no bitmap in this wxStaticBitmap") );

        return *(wxBitmap *)m_image;
    }

    // implementation only from now on
    // -------------------------------

    // implement base class virtuals
    virtual WXLRESULT MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam);

protected:
    virtual wxBorder GetDefaultBorder() const;
    virtual wxSize DoGetBestSize() const;
    virtual WXDWORD MSWGetStyle(long style, WXDWORD *exstyle) const;

    // ctor/dtor helpers
    void Init() { m_isIcon = true; m_image = NULL; }
    void Free();

    // true if icon/bitmap is valid
    bool ImageIsOk() const;

    void SetImage(const wxGDIImage* image);
    void SetImageNoCopy( wxGDIImage* image );

    // we can have either an icon or a bitmap
    bool m_isIcon;
    wxGDIImage *m_image;

private:
    DECLARE_DYNAMIC_CLASS(wxStaticBitmap)
    DECLARE_NO_COPY_CLASS(wxStaticBitmap)
};

#endif
    // _WX_STATBMP_H_
