/////////////////////////////////////////////////////////////////////////////
// Name:        wx/msw/bitmap.h
// Purpose:     wxBitmap class
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id: bitmap.h,v 1.49 2005/03/27 17:47:31 VZ Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_BITMAP_H_
#define _WX_BITMAP_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "bitmap.h"
#endif

#include "wx/msw/gdiimage.h"
#include "wx/gdicmn.h"
#include "wx/palette.h"

class WXDLLEXPORT wxBitmap;
class WXDLLEXPORT wxBitmapHandler;
class WXDLLEXPORT wxBitmapRefData;
class WXDLLEXPORT wxControl;
class WXDLLEXPORT wxCursor;
class WXDLLEXPORT wxDC;
#if wxUSE_WXDIB
class WXDLLEXPORT wxDIB;
#endif
class WXDLLEXPORT wxIcon;
class WXDLLEXPORT wxImage;
class WXDLLEXPORT wxMask;
class WXDLLEXPORT wxPalette;
class WXDLLEXPORT wxPixelDataBase;

// ----------------------------------------------------------------------------
// wxBitmap: a mono or colour bitmap
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxBitmap : public wxGDIImage
{
public:
    // default ctor creates an invalid bitmap, you must Create() it later
    wxBitmap() { Init(); }

    // Copy constructors
    wxBitmap(const wxBitmap& bitmap) : wxGDIImage(bitmap) { Init(); Ref(bitmap); }

    // Initialize with raw data
    wxBitmap(const char bits[], int width, int height, int depth = 1);

    // Initialize with XPM data
    wxBitmap(const char **data) { CreateFromXpm(data); }
    wxBitmap(char **data) { CreateFromXpm((const char **)data); }

    // Load a file or resource
    wxBitmap(const wxString& name, wxBitmapType type = wxBITMAP_TYPE_BMP_RESOURCE);

    // New constructor for generalised creation from data
    wxBitmap(void *data, long type, int width, int height, int depth = 1);

    // Create a new, uninitialized bitmap of the given size and depth (if it
    // is omitted, will create a bitmap compatible with the display)
    //
    // NB: this ctor will create a DIB for 24 and 32bpp bitmaps, use ctor
    //     taking a DC argument if you want to force using DDB in this case
    wxBitmap(int width, int height, int depth = -1);

    // Create a bitmap compatible with the given DC
    wxBitmap(int width, int height, const wxDC& dc);

#if wxUSE_IMAGE && wxUSE_WXDIB
    // Convert from wxImage
    wxBitmap(const wxImage& image, int depth = -1)
        { (void)CreateFromImage(image, depth); }

    // Create a DDB compatible with the given DC from wxImage
    wxBitmap(const wxImage& image, const wxDC& dc)
        { (void)CreateFromImage(image, dc); }
#endif // wxUSE_IMAGE

    // we must have this, otherwise icons are silently copied into bitmaps using
    // the copy ctor but the resulting bitmap is invalid!
    wxBitmap(const wxIcon& icon) { Init(); CopyFromIcon(icon); }

    wxBitmap& operator=(const wxBitmap& bitmap)
    {
        if ( m_refData != bitmap.m_refData )
            Ref(bitmap);
        return *this;
    }

    wxBitmap& operator=(const wxIcon& icon)
    {
        (void)CopyFromIcon(icon);

        return *this;
    }

    wxBitmap& operator=(const wxCursor& cursor)
    {
        (void)CopyFromCursor(cursor);

        return *this;
    }

    virtual ~wxBitmap();

#if wxUSE_IMAGE && wxUSE_WXDIB
    wxImage ConvertToImage() const;
#endif // wxUSE_IMAGE

    // get the given part of bitmap
    wxBitmap GetSubBitmap( const wxRect& rect ) const;

    // copies the contents and mask of the given (colour) icon to the bitmap
    bool CopyFromIcon(const wxIcon& icon);

    // copies the contents and mask of the given cursor to the bitmap
    bool CopyFromCursor(const wxCursor& cursor);

#if wxUSE_WXDIB
    // copies from a device independent bitmap
    bool CopyFromDIB(const wxDIB& dib);
#endif

    virtual bool Create(int width, int height, int depth = -1);
    virtual bool Create(int width, int height, const wxDC& dc);
    virtual bool Create(void *data, long type, int width, int height, int depth = 1);
    virtual bool LoadFile(const wxString& name, long type = wxBITMAP_TYPE_BMP_RESOURCE);
    virtual bool SaveFile(const wxString& name, int type, const wxPalette *cmap = NULL);

    wxBitmapRefData *GetBitmapData() const
        { return (wxBitmapRefData *)m_refData; }

    // raw bitmap access support functions
    void *GetRawData(wxPixelDataBase& data, int bpp);
    void UngetRawData(wxPixelDataBase& data);

#if wxUSE_PALETTE
    wxPalette* GetPalette() const;
    void SetPalette(const wxPalette& palette);
#endif // wxUSE_PALETTE

    wxMask *GetMask() const;
    wxBitmap GetMaskBitmap() const;
    void SetMask(wxMask *mask);

    bool operator==(const wxBitmap& bitmap) const { return m_refData == bitmap.m_refData; }
    bool operator!=(const wxBitmap& bitmap) const { return m_refData != bitmap.m_refData; }

    // these functions are internal and shouldn't be used, they risk to
    // disappear in the future
    bool HasAlpha() const;
    void UseAlpha();

#if WXWIN_COMPATIBILITY_2_4
    // these functions do nothing and are only there for backwards
    // compatibility
    wxDEPRECATED( int GetQuality() const );
    wxDEPRECATED( void SetQuality(int quality) );
#endif // WXWIN_COMPATIBILITY_2_4

    // implementation only from now on
    // -------------------------------

public:
    void SetHBITMAP(WXHBITMAP bmp) { SetHandle((WXHANDLE)bmp); }
    WXHBITMAP GetHBITMAP() const { return (WXHBITMAP)GetHandle(); }

#ifdef __WXDEBUG__
    void SetSelectedInto(wxDC *dc);
    wxDC *GetSelectedInto() const;
#endif // __WXDEBUG__

protected:
    // common part of all ctors
    void Init();

    virtual wxGDIImageRefData *CreateData() const;
    virtual wxObjectRefData *CloneRefData(const wxObjectRefData *data) const;

    // creates the bitmap from XPM data, supposed to be called from ctor
    bool CreateFromXpm(const char **bits);

    // creates an uninitialized bitmap, called from Create()s above
    bool DoCreate(int w, int h, int depth, WXHDC hdc);

#if wxUSE_IMAGE && wxUSE_WXDIB
    // creates the bitmap from wxImage, supposed to be called from ctor
    bool CreateFromImage(const wxImage& image, int depth);

    // creates a DDB from wxImage, supposed to be called from ctor
    bool CreateFromImage(const wxImage& image, const wxDC& dc);

    // common part of the 2 methods above (hdc may be 0)
    bool CreateFromImage(const wxImage& image, int depth, WXHDC hdc);
#endif // wxUSE_IMAGE

private:
#ifdef __WIN32__
    // common part of CopyFromIcon/CopyFromCursor for Win32
    bool CopyFromIconOrCursor(const wxGDIImage& icon);
#endif // __WIN32__

    DECLARE_DYNAMIC_CLASS(wxBitmap)
};

// ----------------------------------------------------------------------------
// wxMask: a mono bitmap used for drawing bitmaps transparently.
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxMask : public wxObject
{
public:
    wxMask();

    // Construct a mask from a bitmap and a colour indicating the transparent
    // area
    wxMask(const wxBitmap& bitmap, const wxColour& colour);

    // Construct a mask from a bitmap and a palette index indicating the
    // transparent area
    wxMask(const wxBitmap& bitmap, int paletteIndex);

    // Construct a mask from a mono bitmap (copies the bitmap).
    wxMask(const wxBitmap& bitmap);

    // construct a mask from the givne bitmap handle
    wxMask(WXHBITMAP hbmp) { m_maskBitmap = hbmp; }

    virtual ~wxMask();

    bool Create(const wxBitmap& bitmap, const wxColour& colour);
    bool Create(const wxBitmap& bitmap, int paletteIndex);
    bool Create(const wxBitmap& bitmap);

    // Implementation
    WXHBITMAP GetMaskBitmap() const { return m_maskBitmap; }
    void SetMaskBitmap(WXHBITMAP bmp) { m_maskBitmap = bmp; }

protected:
    WXHBITMAP m_maskBitmap;

    DECLARE_DYNAMIC_CLASS(wxMask)
};

// ----------------------------------------------------------------------------
// wxBitmapHandler is a class which knows how to load/save bitmaps to/from file
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxBitmapHandler : public wxGDIImageHandler
{
public:
    wxBitmapHandler() { m_type = wxBITMAP_TYPE_INVALID; }
    wxBitmapHandler(const wxString& name, const wxString& ext, long type)
        : wxGDIImageHandler(name, ext, type)
    {
    }

    // keep wxBitmapHandler derived from wxGDIImageHandler compatible with the
    // old class which worked only with bitmaps
    virtual bool Create(wxBitmap *bitmap,
                        void *data,
                        long flags,
                        int width, int height, int depth = 1);
    virtual bool LoadFile(wxBitmap *bitmap,
                          const wxString& name,
                          long flags,
                          int desiredWidth, int desiredHeight);
    virtual bool SaveFile(wxBitmap *bitmap,
                          const wxString& name,
                          int type,
                          const wxPalette *palette = NULL);

    virtual bool Create(wxGDIImage *image,
                        void *data,
                        long flags,
                        int width, int height, int depth = 1);
    virtual bool Load(wxGDIImage *image,
                      const wxString& name,
                      long flags,
                      int desiredWidth, int desiredHeight);
    virtual bool Save(wxGDIImage *image,
                      const wxString& name,
                      int type);

private:
    DECLARE_DYNAMIC_CLASS(wxBitmapHandler)
};

#endif
  // _WX_BITMAP_H_
