/////////////////////////////////////////////////////////////////////////////
// Name:        region.h
// Purpose:     wxRegion class
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id: region.h,v 1.10 2005/02/09 15:01:48 JJ Exp $
// Copyright:   (c) Julian Smart, Robert Roebling
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_REGION_H_
#define _WX_REGION_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "region.h"
#endif

#include "wx/list.h"
#include "wx/gdiobj.h"
#include "wx/gdicmn.h"

//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------

class wxRegion;

//-----------------------------------------------------------------------------
// constants
//-----------------------------------------------------------------------------

enum wxRegionContain
{
    wxOutRegion = 0,
    wxPartRegion = 1,
    wxInRegion = 2
};

// So far, for internal use only
enum wxRegionOp
{
   wxRGN_AND,          // Creates the intersection of the two combined regions.
   wxRGN_COPY,         // Creates a copy of the region identified by hrgnSrc1.
   wxRGN_DIFF,         // Combines the parts of hrgnSrc1 that are not part of hrgnSrc2.
   wxRGN_OR,           // Creates the union of two combined regions.
   wxRGN_XOR           // Creates the union of two combined regions except for any overlapping areas.
};

// ----------------------------------------------------------------------------
// wxRegion
// ----------------------------------------------------------------------------

class wxRegion : public wxGDIObject
{
public:
    wxRegion() { }

    wxRegion( wxCoord x, wxCoord y, wxCoord w, wxCoord h )
    {
        InitRect(x, y, w, h);
    }

    wxRegion( const wxPoint& topLeft, const wxPoint& bottomRight )
    {
        InitRect(topLeft.x, topLeft.y,
                 bottomRight.x - topLeft.x, bottomRight.y - topLeft.y);
    }

    wxRegion( const wxRect& rect )
    {
        InitRect(rect.x, rect.y, rect.width, rect.height);
    }

    wxRegion( size_t n, const wxPoint *points, int fillStyle = wxODDEVEN_RULE );

    wxRegion( const wxBitmap& bmp)
    {
        Union(bmp);
    }
    wxRegion( const wxBitmap& bmp,
              const wxColour& transColour, int tolerance = 0)
    {
        Union(bmp, transColour, tolerance);
    }

    ~wxRegion();

    wxRegion( const wxRegion& region ) { Ref(region); }
    wxRegion& operator = ( const wxRegion& region ) { Ref(region); return *this; }

    bool Ok() const { return m_refData != NULL; }

    bool operator == ( const wxRegion& region );
    bool operator != ( const wxRegion& region ) { return !(*this == region); }

    void Clear();

    bool Offset( wxCoord x, wxCoord y );

    bool Union( wxCoord x, wxCoord y, wxCoord width, wxCoord height );
    bool Union( const wxRect& rect );
    bool Union( const wxRegion& region );

    bool Intersect( wxCoord x, wxCoord y, wxCoord width, wxCoord height );
    bool Intersect( const wxRect& rect );
    bool Intersect( const wxRegion& region );

    bool Subtract( wxCoord x, wxCoord y, wxCoord width, wxCoord height );
    bool Subtract( const wxRect& rect );
    bool Subtract( const wxRegion& region );

    bool Xor( wxCoord x, wxCoord y, wxCoord width, wxCoord height );
    bool Xor( const wxRect& rect );
    bool Xor( const wxRegion& region );

    void GetBox( wxCoord& x, wxCoord& y, wxCoord&w, wxCoord &h ) const;
    wxRect GetBox() const ;

    bool Empty() const;
    bool IsEmpty() const { return Empty(); }

    wxRegionContain Contains( wxCoord x, wxCoord y ) const;
    wxRegionContain Contains( wxCoord x, wxCoord y, wxCoord w, wxCoord h ) const;
    wxRegionContain Contains(const wxPoint& pt) const;
    wxRegionContain Contains(const wxRect& rect) const;

    // Convert the region to a B&W bitmap with the white pixels being inside
    // the region.
    wxBitmap ConvertToBitmap() const;

    // Use the non-transparent pixels of a wxBitmap for the region to combine
    // with this region.  First version takes transparency from bitmap's mask,
    // second lets the user specify the colour to be treated as transparent
    // along with an optional tolerance value.
    // NOTE: implemented in common/rgncmn.cpp
    bool Union(const wxBitmap& bmp);
    bool Union(const wxBitmap& bmp,
               const wxColour& transColour, int tolerance = 0);


public:
    WXRegion *GetX11Region() const;

protected:
    // ref counting code
    virtual wxObjectRefData *CreateRefData() const;
    virtual wxObjectRefData *CloneRefData(const wxObjectRefData *data) const;

    // common part of ctors for a rectangle region
    void InitRect(wxCoord x, wxCoord y, wxCoord w, wxCoord h);

private:
    DECLARE_DYNAMIC_CLASS(wxRegion)
};

// ----------------------------------------------------------------------------
// wxRegionIterator: decomposes a region into rectangles
// ----------------------------------------------------------------------------

class wxRegionIterator: public wxObject
{
public:
    wxRegionIterator();
    wxRegionIterator(const wxRegion& region);

    void Reset() { m_current = 0u; }
    void Reset(const wxRegion& region);

    operator bool () const;
    bool HaveRects() const;

    void operator ++ ();
    void operator ++ (int);

    wxCoord GetX() const;
    wxCoord GetY() const;
    wxCoord GetW() const;
    wxCoord GetWidth() const { return GetW(); }
    wxCoord GetH() const;
    wxCoord GetHeight() const { return GetH(); }
    wxRect GetRect() const;

private:
    size_t   m_current;
    wxRegion m_region;

private:
    DECLARE_DYNAMIC_CLASS(wxRegionIterator)
};

#endif
// _WX_REGION_H_
