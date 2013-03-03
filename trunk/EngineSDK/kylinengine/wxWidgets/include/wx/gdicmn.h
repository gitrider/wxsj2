/////////////////////////////////////////////////////////////////////////////
// Name:        gdicmn.h
// Purpose:     Common GDI classes, types and declarations
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id: gdicmn.h,v 1.97 2005/05/04 18:51:58 JS Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_GDICMNH__
#define _WX_GDICMNH__

// ---------------------------------------------------------------------------
// headers
// ---------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "gdicmn.h"
#endif

#include "wx/defs.h"
#include "wx/list.h"
#include "wx/string.h"
#include "wx/fontenc.h"
#include "wx/hashmap.h"

// ---------------------------------------------------------------------------
// forward declarations
// ---------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxBitmap;
class WXDLLIMPEXP_CORE wxBrush;
class WXDLLIMPEXP_CORE wxColour;
class WXDLLIMPEXP_CORE wxCursor;
class WXDLLIMPEXP_CORE wxFont;
class WXDLLIMPEXP_CORE wxIcon;
class WXDLLIMPEXP_CORE wxPalette;
class WXDLLIMPEXP_CORE wxPen;
class WXDLLIMPEXP_CORE wxRegion;
class WXDLLIMPEXP_BASE wxString;

// ---------------------------------------------------------------------------
// constants
// ---------------------------------------------------------------------------

// Bitmap flags
enum wxBitmapType
{
    wxBITMAP_TYPE_INVALID,          // should be == 0 for compatibility!
    wxBITMAP_TYPE_BMP,
    wxBITMAP_TYPE_BMP_RESOURCE,
    wxBITMAP_TYPE_RESOURCE = wxBITMAP_TYPE_BMP_RESOURCE,
    wxBITMAP_TYPE_ICO,
    wxBITMAP_TYPE_ICO_RESOURCE,
    wxBITMAP_TYPE_CUR,
    wxBITMAP_TYPE_CUR_RESOURCE,
    wxBITMAP_TYPE_XBM,
    wxBITMAP_TYPE_XBM_DATA,
    wxBITMAP_TYPE_XPM,
    wxBITMAP_TYPE_XPM_DATA,
    wxBITMAP_TYPE_TIF,
    wxBITMAP_TYPE_TIF_RESOURCE,
    wxBITMAP_TYPE_GIF,
    wxBITMAP_TYPE_GIF_RESOURCE,
    wxBITMAP_TYPE_PNG,
    wxBITMAP_TYPE_PNG_RESOURCE,
    wxBITMAP_TYPE_JPEG,
    wxBITMAP_TYPE_JPEG_RESOURCE,
    wxBITMAP_TYPE_PNM,
    wxBITMAP_TYPE_PNM_RESOURCE,
    wxBITMAP_TYPE_PCX,
    wxBITMAP_TYPE_PCX_RESOURCE,
    wxBITMAP_TYPE_PICT,
    wxBITMAP_TYPE_PICT_RESOURCE,
    wxBITMAP_TYPE_ICON,
    wxBITMAP_TYPE_ICON_RESOURCE,
    wxBITMAP_TYPE_ANI,
    wxBITMAP_TYPE_IFF,
    wxBITMAP_TYPE_MACCURSOR,
    wxBITMAP_TYPE_MACCURSOR_RESOURCE,
    wxBITMAP_TYPE_ANY = 50
};

// Standard cursors
enum wxStockCursor
{
    wxCURSOR_NONE,          // should be 0
    wxCURSOR_ARROW,
    wxCURSOR_RIGHT_ARROW,
    wxCURSOR_BULLSEYE,
    wxCURSOR_CHAR,
    wxCURSOR_CROSS,
    wxCURSOR_HAND,
    wxCURSOR_IBEAM,
    wxCURSOR_LEFT_BUTTON,
    wxCURSOR_MAGNIFIER,
    wxCURSOR_MIDDLE_BUTTON,
    wxCURSOR_NO_ENTRY,
    wxCURSOR_PAINT_BRUSH,
    wxCURSOR_PENCIL,
    wxCURSOR_POINT_LEFT,
    wxCURSOR_POINT_RIGHT,
    wxCURSOR_QUESTION_ARROW,
    wxCURSOR_RIGHT_BUTTON,
    wxCURSOR_SIZENESW,
    wxCURSOR_SIZENS,
    wxCURSOR_SIZENWSE,
    wxCURSOR_SIZEWE,
    wxCURSOR_SIZING,
    wxCURSOR_SPRAYCAN,
    wxCURSOR_WAIT,
    wxCURSOR_WATCH,
    wxCURSOR_BLANK,
#ifdef __WXGTK__
    wxCURSOR_DEFAULT, // standard X11 cursor
#endif
#ifdef __WXMAC__
    wxCURSOR_COPY_ARROW , // MacOS Theme Plus arrow
#endif
#ifdef __X__
    // Not yet implemented for Windows
    wxCURSOR_CROSS_REVERSE,
    wxCURSOR_DOUBLE_ARROW,
    wxCURSOR_BASED_ARROW_UP,
    wxCURSOR_BASED_ARROW_DOWN,
#endif // X11

    wxCURSOR_ARROWWAIT,

    wxCURSOR_MAX
};

#ifndef __WXGTK__
    #define wxCURSOR_DEFAULT wxCURSOR_ARROW
#endif

// ---------------------------------------------------------------------------
// macros
// ---------------------------------------------------------------------------

/* Useful macro for creating icons portably, for example:

    wxIcon *icon = new wxICON(mondrian);

  expands into:

    wxIcon *icon = new wxIcon("mondrian");      // On wxMSW
    wxIcon *icon = new wxIcon(mondrian_xpm);    // On wxGTK
 */

#ifdef __WXMSW__
    // Load from a resource
    #define wxICON(X) wxIcon(wxT(#X))
#elif defined(__WXPM__)
    // Load from a resource
    #define wxICON(X) wxIcon(wxT(#X))
#elif defined(__WXMGL__)
    // Initialize from an included XPM
    #define wxICON(X) wxIcon( (const char**) X##_xpm )
#elif defined(__WXGTK__)
    // Initialize from an included XPM
    #define wxICON(X) wxIcon( (const char**) X##_xpm )
#elif defined(__WXMAC__)
    // Initialize from an included XPM
    #define wxICON(X) wxIcon( (const char**) X##_xpm )
#elif defined(__WXMOTIF__)
    // Initialize from an included XPM
    #define wxICON(X) wxIcon( X##_xpm )
#elif defined(__WXX11__)
    // Initialize from an included XPM
    #define wxICON(X) wxIcon( X##_xpm )
#else
    // This will usually mean something on any platform
    #define wxICON(X) wxIcon(wxT(#X))
#endif // platform

/* Another macro: this one is for portable creation of bitmaps. We assume that
   under Unix bitmaps live in XPMs and under Windows they're in ressources.
 */

#if defined(__WXMSW__) || defined(__WXPM__)
    #define wxBITMAP(name) wxBitmap(wxT(#name), wxBITMAP_TYPE_RESOURCE)
#elif defined(__WXGTK__)   || \
      defined(__WXMOTIF__) || \
      defined(__WXX11__)   || \
      defined(__WXMAC__)   || \
      defined(__WXMGL__)   || \
      defined(__WXCOCOA__)
    // Initialize from an included XPM
    #define wxBITMAP(name) wxBitmap( (const char**) name##_xpm )
#else // other platforms
    #define wxBITMAP(name) wxBitmap(name##_xpm, wxBITMAP_TYPE_XPM)
#endif // platform

// ===========================================================================
// classes
// ===========================================================================

// ---------------------------------------------------------------------------
// wxSize
// ---------------------------------------------------------------------------

class WXDLLEXPORT wxSize
{
public:
    // members are public for compatibility, don't use them directly.
    int x, y;

    // constructors
    wxSize() : x(0), y(0) { }
    wxSize(int xx, int yy) : x(xx), y(yy) { }

    // no copy ctor or assignment operator - the defaults are ok

    bool operator==(const wxSize& sz) const { return x == sz.x && y == sz.y; }
    bool operator!=(const wxSize& sz) const { return x != sz.x || y != sz.y; }

    wxSize operator+(const wxSize& sz) { return wxSize(x + sz.x, y + sz.y); }
    wxSize operator-(const wxSize& sz) { return wxSize(x - sz.x, y - sz.y); }
    wxSize operator/(const int i) { return wxSize(x / i, y / i); }
    wxSize operator*(const int i) { return wxSize(x * i, y * i); }

    wxSize& operator+=(const wxSize& sz) { x += sz.x; y += sz.y; return *this; }
    wxSize& operator-=(const wxSize& sz) { x -= sz.x; y -= sz.y; return *this; }
    wxSize& operator/=(const int i) { x /= i; y /= i; return *this; }
    wxSize& operator*=(const int i) { x *= i; y *= i; return *this; }

    void IncTo(const wxSize& sz)
        { if ( sz.x > x ) x = sz.x; if ( sz.y > y ) y = sz.y; }
    void DecTo(const wxSize& sz)
        { if ( sz.x < x ) x = sz.x; if ( sz.y < y ) y = sz.y; }

    // accessors
    void Set(int xx, int yy) { x = xx; y = yy; }
    void SetWidth(int w) { x = w; }
    void SetHeight(int h) { y = h; }

    int GetWidth() const { return x; }
    int GetHeight() const { return y; }

    bool IsFullySpecified() const { return x != wxDefaultCoord && y != wxDefaultCoord; }

    // combine this size with the other one replacing the default (i.e. equal
    // to wxDefaultCoord) components of this object with those of the other
    void SetDefaults(const wxSize& size)
    {
        if ( x == wxDefaultCoord )
            x = size.x;
        if ( y == wxDefaultCoord )
            y = size.y;
    }

    // compatibility
    int GetX() const { return x; }
    int GetY() const { return y; }
};

// ---------------------------------------------------------------------------
// Point classes: with real or integer coordinates
// ---------------------------------------------------------------------------

class WXDLLEXPORT wxRealPoint
{
public:
    double x;
    double y;

    wxRealPoint() : x(0.0), y(0.0) { }
    wxRealPoint(double xx, double yy) : x(xx), y(yy) { }

    wxRealPoint operator+(const wxRealPoint& pt) const { return wxRealPoint(x + pt.x, y + pt.y); }
    wxRealPoint operator-(const wxRealPoint& pt) const { return wxRealPoint(x - pt.x, y - pt.y); }

    bool operator==(const wxRealPoint& pt) const { return x == pt.x && y == pt.y; }
    bool operator!=(const wxRealPoint& pt) const { return x != pt.x || y != pt.y; }
};

class WXDLLEXPORT wxPoint
{
public:
    int x, y;

    wxPoint() : x(0), y(0) { }
    wxPoint(int xx, int yy) : x(xx), y(yy) { }

    // no copy ctor or assignment operator - the defaults are ok

    // comparison
    bool operator==(const wxPoint& p) const { return x == p.x && y == p.y; }
    bool operator!=(const wxPoint& p) const { return !(*this == p); }

    // arithmetic operations (component wise)
    wxPoint operator+(const wxPoint& p) const { return wxPoint(x + p.x, y + p.y); }
    wxPoint operator-(const wxPoint& p) const { return wxPoint(x - p.x, y - p.y); }

    wxPoint& operator+=(const wxPoint& p) { x += p.x; y += p.y; return *this; }
    wxPoint& operator-=(const wxPoint& p) { x -= p.x; y -= p.y; return *this; }

    wxPoint& operator+=(const wxSize& s) { x += s.GetWidth(); y += s.GetHeight(); return *this; }
    wxPoint& operator-=(const wxSize& s) { x -= s.GetWidth(); y -= s.GetHeight(); return *this; }

    wxPoint operator+(const wxSize& s) const { return wxPoint(x + s.GetWidth(), y + s.GetHeight()); }
    wxPoint operator-(const wxSize& s) const { return wxPoint(x - s.GetWidth(), y - s.GetHeight()); }
};

// ---------------------------------------------------------------------------
// wxRect
// ---------------------------------------------------------------------------

class WXDLLEXPORT wxRect
{
public:
    wxRect()
        : x(0), y(0), width(0), height(0)
        { }
    wxRect(int xx, int yy, int ww, int hh)
        : x(xx), y(yy), width(ww), height(hh)
        { }
    wxRect(const wxPoint& topLeft, const wxPoint& bottomRight);
    wxRect(const wxPoint& pt, const wxSize& size)
        : x(pt.x), y(pt.y), width(size.x), height(size.y)
        { }
    wxRect(const wxSize& size)
        : x(0), y(0), width(size.x), height(size.y)
        { }

    // default copy ctor and assignment operators ok

    int GetX() const { return x; }
    void SetX(int xx) { x = xx; }

    int GetY() const { return y; }
    void SetY(int yy) { y = yy; }

    int GetWidth() const { return width; }
    void SetWidth(int w) { width = w; }

    int GetHeight() const { return height; }
    void SetHeight(int h) { height = h; }

    wxPoint GetPosition() const { return wxPoint(x, y); }
    void SetPosition( const wxPoint &p ) { x = p.x; y = p.y; }

    wxSize GetSize() const { return wxSize(width, height); }
    void SetSize( const wxSize &s ) { width = s.GetWidth(); height = s.GetHeight(); }

    bool IsEmpty() const { return (width <= 0) || (height <= 0); }

    wxPoint GetTopLeft() const { return GetPosition(); }
    wxPoint GetLeftTop() const { return GetTopLeft(); }
    void SetTopLeft(const wxPoint &p) { SetPosition(p); }
    void SetLeftTop(const wxPoint &p) { SetTopLeft(p); }

    wxPoint GetBottomRight() const { return wxPoint(GetRight(), GetBottom()); }
    wxPoint GetRightBottom() const { return GetBottomRight(); }
    void SetBottomRight(const wxPoint &p) { SetRight(p.x); SetBottom(p.y); }
    void SetRightBottom(const wxPoint &p) { SetBottomRight(p); }

    int GetLeft()   const { return x; }
    int GetTop()    const { return y; }
    int GetBottom() const { return y + height - 1; }
    int GetRight()  const { return x + width - 1; }

    void SetLeft(int left) { x = left; }
    void SetRight(int right) { width = right - x + 1; }
    void SetTop(int top) { y = top; }
    void SetBottom(int bottom) { height = bottom - y + 1; }

    // operations with rect
    wxRect& Inflate(wxCoord dx, wxCoord dy);
    wxRect& Inflate(wxCoord d) { return Inflate(d, d); }
    wxRect Inflate(wxCoord dx, wxCoord dy) const
    {
        wxRect r = *this;
        r.Inflate(dx, dy);
        return r;
    }

    wxRect& Deflate(wxCoord dx, wxCoord dy) { return Inflate(-dx, -dy); }
    wxRect& Deflate(wxCoord d) { return Inflate(-d); }
    wxRect Deflate(wxCoord dx, wxCoord dy) const
    {
        wxRect r = *this;
        r.Deflate(dx, dy);
        return r;
    }

    void Offset(wxCoord dx, wxCoord dy) { x += dx; y += dy; }
    void Offset(const wxPoint& pt) { Offset(pt.x, pt.y); }

    wxRect& Intersect(const wxRect& rect);
    wxRect Intersect(const wxRect& rect) const
    {
        wxRect r = *this;
        r.Intersect(rect);
        return r;
    }

    wxRect& Union(const wxRect& rect);
    wxRect Union(const wxRect& rect) const
    {
        wxRect r = *this;
        r.Union(rect);
        return r;
    }

    // compare rectangles
    bool operator==(const wxRect& rect) const;
    bool operator!=(const wxRect& rect) const { return !(*this == rect); }

    // return true if the point is (not strcitly) inside the rect
    bool Inside(int x, int y) const;
    bool Inside(const wxPoint& pt) const { return Inside(pt.x, pt.y); }

    // return true if the rectangles have a non empty intersection
    bool Intersects(const wxRect& rect) const;


    // these are like Union() but don't ignore empty rectangles
    wxRect operator+(const wxRect& rect) const;
    wxRect& operator+=(const wxRect& rect)
    {
        *this = *this + rect;
        return *this;
    }


public:
    int x, y, width, height;
};

// ---------------------------------------------------------------------------
// Management of pens, brushes and fonts
// ---------------------------------------------------------------------------

typedef wxInt8 wxDash;

class WXDLLEXPORT wxPenList : public wxList
{
public:
    wxPenList() { }
    ~wxPenList();

    void AddPen(wxPen *pen);
    void RemovePen(wxPen *pen);
    wxPen *FindOrCreatePen(const wxColour& colour, int width, int style);
};

class WXDLLEXPORT wxBrushList : public wxList
{
public:
    wxBrushList() { }
    ~wxBrushList();

    void AddBrush(wxBrush *brush);
    void RemoveBrush(wxBrush *brush);
    wxBrush *FindOrCreateBrush(const wxColour& colour, int style = wxSOLID);
};

class WXDLLEXPORT wxFontList : public wxList
{
public:
    wxFontList() { }
    ~wxFontList();

    void AddFont(wxFont *font);
    void RemoveFont(wxFont *font);
    wxFont *FindOrCreateFont(int pointSize, int family, int style, int weight,
                             bool underline = false,
                             const wxString& face = wxEmptyString,
                             wxFontEncoding encoding = wxFONTENCODING_DEFAULT);
};

WX_DECLARE_STRING_HASH_MAP( wxColour *, wxStringToColourHashMap );

class WXDLLEXPORT wxColourDatabase
{
public:
    wxColourDatabase();
    ~wxColourDatabase();

    // find colour by name or name for the given colour
    wxColour Find(const wxString& name) const;
    wxString FindName(const wxColour& colour) const;

    // add a new colour to the database
    void AddColour(const wxString& name, const wxColour& colour);

    // deprecated, use Find() instead
    wxDEPRECATED( wxColour *FindColour(const wxString& name) );


#ifdef __WXPM__
    // PM keeps its own type of colour table
    long*                           m_palTable;
    size_t                          m_nSize;
#endif

private:
    // load the database with the built in colour values when called for the
    // first time, do nothing after this
    void Initialize();

    wxStringToColourHashMap *m_map;
};

class WXDLLEXPORT wxBitmapList : public wxList
{
public:
    wxBitmapList(){}
    ~wxBitmapList();

    void AddBitmap(wxBitmap *bitmap);
    void RemoveBitmap(wxBitmap *bitmap);
};

class WXDLLEXPORT wxResourceCache: public wxList
{
public:
    wxResourceCache() { }
#if !wxUSE_STL
    wxResourceCache(const unsigned int keyType) : wxList(keyType) { }
#endif
    ~wxResourceCache();
};

// ---------------------------------------------------------------------------
// global variables
// ---------------------------------------------------------------------------

// Lists of GDI objects
extern WXDLLEXPORT_DATA(wxPenList*)   wxThePenList;
extern WXDLLEXPORT_DATA(wxBrushList*)   wxTheBrushList;
extern WXDLLEXPORT_DATA(wxFontList*)    wxTheFontList;
extern WXDLLEXPORT_DATA(wxBitmapList*)  wxTheBitmapList;

// Stock objects
extern WXDLLEXPORT_DATA(wxFont*)      wxNORMAL_FONT;
extern WXDLLEXPORT_DATA(wxFont*)      wxSMALL_FONT;
extern WXDLLEXPORT_DATA(wxFont*)      wxITALIC_FONT;
extern WXDLLEXPORT_DATA(wxFont*)      wxSWISS_FONT;

extern WXDLLEXPORT_DATA(wxPen*)      wxRED_PEN;
extern WXDLLEXPORT_DATA(wxPen*)      wxCYAN_PEN;
extern WXDLLEXPORT_DATA(wxPen*)      wxGREEN_PEN;
extern WXDLLEXPORT_DATA(wxPen*)      wxBLACK_PEN;
extern WXDLLEXPORT_DATA(wxPen*)      wxWHITE_PEN;
extern WXDLLEXPORT_DATA(wxPen*)      wxTRANSPARENT_PEN;
extern WXDLLEXPORT_DATA(wxPen*)      wxBLACK_DASHED_PEN;
extern WXDLLEXPORT_DATA(wxPen*)      wxGREY_PEN;
extern WXDLLEXPORT_DATA(wxPen*)      wxMEDIUM_GREY_PEN;
extern WXDLLEXPORT_DATA(wxPen*)      wxLIGHT_GREY_PEN;

extern WXDLLEXPORT_DATA(wxBrush*)    wxBLUE_BRUSH;
extern WXDLLEXPORT_DATA(wxBrush*)    wxGREEN_BRUSH;
extern WXDLLEXPORT_DATA(wxBrush*)    wxWHITE_BRUSH;
extern WXDLLEXPORT_DATA(wxBrush*)    wxBLACK_BRUSH;
extern WXDLLEXPORT_DATA(wxBrush*)    wxGREY_BRUSH;
extern WXDLLEXPORT_DATA(wxBrush*)    wxMEDIUM_GREY_BRUSH;
extern WXDLLEXPORT_DATA(wxBrush*)    wxLIGHT_GREY_BRUSH;
extern WXDLLEXPORT_DATA(wxBrush*)    wxTRANSPARENT_BRUSH;
extern WXDLLEXPORT_DATA(wxBrush*)    wxCYAN_BRUSH;
extern WXDLLEXPORT_DATA(wxBrush*)    wxRED_BRUSH;

extern WXDLLEXPORT_DATA(wxColour*)    wxBLACK;
extern WXDLLEXPORT_DATA(wxColour*)    wxWHITE;
extern WXDLLEXPORT_DATA(wxColour*)    wxRED;
extern WXDLLEXPORT_DATA(wxColour*)    wxBLUE;
extern WXDLLEXPORT_DATA(wxColour*)    wxGREEN;
extern WXDLLEXPORT_DATA(wxColour*)    wxCYAN;
extern WXDLLEXPORT_DATA(wxColour*)    wxLIGHT_GREY;

// 'Null' objects
extern WXDLLEXPORT_DATA(wxBitmap)     wxNullBitmap;
extern WXDLLEXPORT_DATA(wxIcon)       wxNullIcon;
extern WXDLLEXPORT_DATA(wxCursor)     wxNullCursor;
extern WXDLLEXPORT_DATA(wxPen)        wxNullPen;
extern WXDLLEXPORT_DATA(wxBrush)      wxNullBrush;
extern WXDLLEXPORT_DATA(wxPalette)     wxNullPalette;
extern WXDLLEXPORT_DATA(wxFont)       wxNullFont;
extern WXDLLEXPORT_DATA(wxColour)     wxNullColour;

// Stock cursors types
extern WXDLLEXPORT_DATA(wxCursor*)    wxSTANDARD_CURSOR;
extern WXDLLEXPORT_DATA(wxCursor*)    wxHOURGLASS_CURSOR;
extern WXDLLEXPORT_DATA(wxCursor*)    wxCROSS_CURSOR;

extern WXDLLEXPORT_DATA(wxColourDatabase*)  wxTheColourDatabase;

extern WXDLLEXPORT_DATA(const wxChar*) wxPanelNameStr;

extern WXDLLEXPORT_DATA(const wxSize) wxDefaultSize;
extern WXDLLEXPORT_DATA(const wxPoint) wxDefaultPosition;

// The list of objects which should be deleted
extern WXDLLEXPORT_DATA(wxList) wxPendingDelete;

// ---------------------------------------------------------------------------
// global functions
// ---------------------------------------------------------------------------

// resource management
extern void WXDLLEXPORT wxInitializeStockObjects();
extern void WXDLLEXPORT wxInitializeStockLists();
extern void WXDLLEXPORT wxDeleteStockObjects();
extern void WXDLLEXPORT wxDeleteStockLists();

// is the display colour (or monochrome)?
extern bool WXDLLEXPORT wxColourDisplay();

// Returns depth of screen
extern int WXDLLEXPORT wxDisplayDepth();
#define wxGetDisplayDepth wxDisplayDepth

// get the display size
extern void WXDLLEXPORT wxDisplaySize(int *width, int *height);
extern wxSize WXDLLEXPORT wxGetDisplaySize();
extern void WXDLLEXPORT wxDisplaySizeMM(int *width, int *height);
extern wxSize WXDLLEXPORT wxGetDisplaySizeMM();

// Get position and size of the display workarea
extern void WXDLLEXPORT wxClientDisplayRect(int *x, int *y, int *width, int *height);
extern wxRect WXDLLEXPORT wxGetClientDisplayRect();

// set global cursor
extern void WXDLLEXPORT wxSetCursor(const wxCursor& cursor);

#endif
    // _WX_GDICMNH__
