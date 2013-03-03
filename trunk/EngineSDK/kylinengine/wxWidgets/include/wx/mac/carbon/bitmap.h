/////////////////////////////////////////////////////////////////////////////
// Name:        bitmap.h
// Purpose:     wxBitmap class
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// RCS-ID:      $Id: bitmap.h,v 1.8 2005/03/01 05:31:37 SC Exp $
// Copyright:   (c) Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_BITMAP_H_
#define _WX_BITMAP_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
  #pragma interface "bitmap.h"
#endif

#include "wx/palette.h"

// Bitmap
class WXDLLEXPORT wxBitmap;
class wxBitmapRefData ;
class WXDLLEXPORT wxBitmapHandler;
class WXDLLEXPORT wxControl;
class WXDLLEXPORT wxCursor;
class WXDLLEXPORT wxDC;
class WXDLLEXPORT wxIcon;
class WXDLLEXPORT wxImage;
class WXDLLEXPORT wxPixelDataBase;

// A mask is a bitmap used for drawing bitmaps
// Internally it is stored as a 8 bit deep memory chunk, 0 = black means the source will be drawn
// 255 = white means the source will not be drawn, no other values will be present
// 8 bit is chosen only for performance reasons, note also that this is the inverse value range
// from alpha, where 0 = invisible , 255 = fully drawn

class WXDLLEXPORT wxMask: public wxObject
{
    DECLARE_DYNAMIC_CLASS(wxMask)
    DECLARE_NO_COPY_CLASS(wxMask)

public:
    wxMask();

    // Construct a mask from a bitmap and a colour indicating
    // the transparent area
    wxMask(const wxBitmap& bitmap, const wxColour& colour);

    // Construct a mask from a mono bitmap (black meaning show pixels, white meaning transparent)
    wxMask(const wxBitmap& bitmap);
    
    // implementation helper only : construct a mask from a 8 bit memory buffer
    wxMask(const wxMemoryBuffer& buf, int width , int height , int bytesPerRow ) ;

    ~wxMask();

    bool Create(const wxBitmap& bitmap, const wxColour& colour);
    bool Create(const wxBitmap& bitmap);
    bool Create(const wxMemoryBuffer& buf, int width , int height , int bytesPerRow ) ;

    // Implementation below

    void Init() ;

    // a 8 bit depth mask 
    void* GetRawAccess() const;
    int GetBytesPerRow() const { return m_bytesPerRow ; }
    // renders/updates native representation when necessary 
    void RealizeNative() ;

    WXHBITMAP GetHBITMAP() const ;


private:
    wxMemoryBuffer m_memBuf ;
    int m_bytesPerRow ;
    int m_width ;
    int m_height ;

    WXHBITMAP m_maskBitmap ;

};

class WXDLLEXPORT wxBitmapHandler: public wxBitmapHandlerBase
{
public:
    wxBitmapHandler() { }
    virtual ~wxBitmapHandler();

    virtual bool Create(wxBitmap *bitmap, void *data, long flags, int width, int height, int depth = 1);
    virtual bool LoadFile(wxBitmap *bitmap, const wxString& name, long flags,
        int desiredWidth, int desiredHeight);
    virtual bool SaveFile(const wxBitmap *bitmap, const wxString& name, int type, const wxPalette *palette = NULL);

private:
    DECLARE_DYNAMIC_CLASS(wxBitmapHandler)
};

#define M_BITMAPHANDLERDATA ((wxBitmapRefData *)bitmap->GetRefData())

class WXDLLEXPORT wxBitmap: public wxBitmapBase
{
    DECLARE_DYNAMIC_CLASS(wxBitmap)

    friend class WXDLLEXPORT wxBitmapHandler;

public:
    wxBitmap(); // Platform-specific

    // Copy constructors
    wxBitmap(const wxBitmap& bitmap)
    { Ref(bitmap); }

    // Initialize with raw data.
    wxBitmap(const char bits[], int width, int height, int depth = 1);

    // Initialize with XPM data
    bool CreateFromXpm(const char **bits);
    wxBitmap(const char **bits);
    wxBitmap(char **bits);

    // Load a file or resource
    wxBitmap(const wxString& name, wxBitmapType type = wxBITMAP_TYPE_PICT_RESOURCE);

    // Constructor for generalised creation from data
    wxBitmap(void *data, wxBitmapType type, int width, int height, int depth = 1);

    // If depth is omitted, will create a bitmap compatible with the display
    wxBitmap(int width, int height, int depth = -1);

    // Convert from wxImage:
    wxBitmap(const wxImage& image, int depth = -1);

    // Convert from wxIcon
    wxBitmap(const wxIcon& icon) { CopyFromIcon(icon); }

    ~wxBitmap();

    wxImage ConvertToImage() const;

    // get the given part of bitmap
    wxBitmap GetSubBitmap( const wxRect& rect ) const;

    virtual bool Create(int width, int height, int depth = -1);
    virtual bool Create(void *data, wxBitmapType type, int width, int height, int depth = 1);
    // virtual bool Create( WXHICON icon) ;
    virtual bool LoadFile(const wxString& name, wxBitmapType type = wxBITMAP_TYPE_BMP_RESOURCE);
    virtual bool SaveFile(const wxString& name, wxBitmapType type, const wxPalette *cmap = NULL) const;

    wxBitmapRefData *GetBitmapData() const
        { return (wxBitmapRefData *)m_refData; }

    // copies the contents and mask of the given (colour) icon to the bitmap
    virtual bool CopyFromIcon(const wxIcon& icon);

    bool Ok() const;
    int GetWidth() const;
    int GetHeight() const;
    int GetDepth() const;
    void SetWidth(int w);
    void SetHeight(int h);
    void SetDepth(int d);
    void SetOk(bool isOk);

#if WXWIN_COMPATIBILITY_2_4
    // these functions do nothing and are only there for backwards
    // compatibility
    wxDEPRECATED( int GetQuality() const );
    wxDEPRECATED( void SetQuality(int quality) );
#endif // WXWIN_COMPATIBILITY_2_4

#if wxUSE_PALETTE
    wxPalette* GetPalette() const;
    void SetPalette(const wxPalette& palette);
#endif // wxUSE_PALETTE

    wxMask *GetMask() const;
    void SetMask(wxMask *mask) ;

    inline wxBitmap& operator = (const wxBitmap& bitmap) { if (*this == bitmap) return (*this); Ref(bitmap); return *this; }
    inline bool operator == (const wxBitmap& bitmap) const { return m_refData == bitmap.m_refData; }
    inline bool operator != (const wxBitmap& bitmap) const { return m_refData != bitmap.m_refData; }

    static void InitStandardHandlers();

    // raw bitmap access support functions, for internal use only
    void *GetRawData(wxPixelDataBase& data, int bpp);
    void UngetRawData(wxPixelDataBase& data);

    // these functions are internal and shouldn't be used, they risk to
    // disappear in the future
    bool HasAlpha() const;
    void UseAlpha();

    // returns the 'native' implementation, a GWorldPtr for the content and one for the mask 
    WXHBITMAP GetHBITMAP( WXHBITMAP * mask = NULL ) const;

#ifdef __WXMAC_OSX__
    // returns a CGImageRef which must released after usage with CGImageRelease
    WXCGIMAGEREF CGImageCreate() const ;
#endif
    // get read only access to the underlying buffer
    void *GetRawAccess() const ;
    // brackets to the underlying OS structure for read/write access
    // makes sure that no cached images will be constructed until terminated
    void *BeginRawAccess() ;
    void EndRawAccess() ;
};
#endif
  // _WX_BITMAP_H_
