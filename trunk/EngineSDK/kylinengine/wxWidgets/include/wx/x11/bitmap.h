/////////////////////////////////////////////////////////////////////////////
// Name:        x11/bitmap.h
// Purpose:     wxBitmap class
// Author:      Julian Smart, Robert Roebling
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id: bitmap.h,v 1.13 2004/05/23 20:51:52 JS Exp $
// Copyright:   (c) Julian Smart, Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_BITMAP_H_
#define _WX_BITMAP_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "bitmap.h"
#endif

#include "wx/defs.h"
#include "wx/object.h"
#include "wx/string.h"
#include "wx/palette.h"
#include "wx/gdiobj.h"

//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------

class wxMask;
class wxBitmap;
class wxImage;

//-----------------------------------------------------------------------------
// wxMask
//-----------------------------------------------------------------------------

class wxMask: public wxObject
{
public:
    wxMask();
    wxMask( const wxBitmap& bitmap, const wxColour& colour );
    wxMask( const wxBitmap& bitmap, int paletteIndex );
    wxMask( const wxBitmap& bitmap );
    ~wxMask();

    bool Create( const wxBitmap& bitmap, const wxColour& colour );
    bool Create( const wxBitmap& bitmap, int paletteIndex );
    bool Create( const wxBitmap& bitmap );

    // implementation
    WXPixmap GetBitmap() const              { return m_bitmap; }
    void SetBitmap( WXPixmap bitmap )       { m_bitmap = bitmap; }

    WXDisplay *GetDisplay() const           { return m_display; }
    void SetDisplay( WXDisplay *display )   { m_display = display; }

private:
    WXPixmap    m_bitmap;
    WXDisplay  *m_display;

private:
    DECLARE_DYNAMIC_CLASS(wxMask)
};

//-----------------------------------------------------------------------------
// wxBitmap
//-----------------------------------------------------------------------------

class WXDLLEXPORT wxBitmapHandler : public wxBitmapHandlerBase
{
public:
    wxBitmapHandler() : wxBitmapHandlerBase() {}
private:
    DECLARE_DYNAMIC_CLASS(wxBitmapHandler)
};

class wxBitmap: public wxBitmapBase
{
public:
    wxBitmap();
    wxBitmap( int width, int height, int depth = -1 );
    wxBitmap( const char bits[], int width, int height, int depth = 1 );
    wxBitmap( const char **bits ) { (void)CreateFromXpm(bits); }
    wxBitmap( char **bits ) { (void)CreateFromXpm((const char **)bits); }
    wxBitmap( const wxBitmap& bmp );
    wxBitmap( const wxString &filename, wxBitmapType type = wxBITMAP_TYPE_XPM );
    virtual ~wxBitmap();

    wxBitmap& operator = ( const wxBitmap& bmp );
    bool operator == ( const wxBitmap& bmp ) const;
    bool operator != ( const wxBitmap& bmp ) const;
    bool Ok() const;

    static void InitStandardHandlers();

    bool Create(int width, int height, int depth = -1);
    bool Create(void* data, wxBitmapType type,
                int width, int height, int depth = -1);
    // create the wxBitmap using a _copy_ of the pixmap
    bool Create(WXPixmap pixmap);

    int GetHeight() const;
    int GetWidth() const;
    int GetDepth() const;

#if wxUSE_IMAGE
    wxBitmap( const wxImage& image, int depth = -1 ) { (void)CreateFromImage(image, depth); }
    wxImage ConvertToImage() const;
    bool CreateFromImage(const wxImage& image, int depth = -1);
#endif // wxUSE_IMAGE

    // copies the contents and mask of the given (colour) icon to the bitmap
    virtual bool CopyFromIcon(const wxIcon& icon);

    wxMask *GetMask() const;
    void SetMask( wxMask *mask );

    wxBitmap GetSubBitmap( const wxRect& rect ) const;

    bool SaveFile( const wxString &name, wxBitmapType type, const wxPalette *palette = (wxPalette *) NULL ) const;
    bool LoadFile( const wxString &name, wxBitmapType type = wxBITMAP_TYPE_XPM );

    wxPalette *GetPalette() const;
    wxPalette *GetColourMap() const
        { return GetPalette(); };
    virtual void SetPalette(const wxPalette& palette);

    // implementation
    // --------------

    void SetHeight( int height );
    void SetWidth( int width );
    void SetDepth( int depth );
    void SetPixmap( WXPixmap pixmap );
    void SetBitmap( WXPixmap bitmap );

    WXPixmap GetPixmap() const;
    WXPixmap GetBitmap() const;

    WXPixmap GetDrawable() const;

    WXDisplay *GetDisplay() const;

protected:
    bool CreateFromXpm(const char **bits);

private:
    DECLARE_DYNAMIC_CLASS(wxBitmap)
};

#endif
// _WX_BITMAP_H_
