/////////////////////////////////////////////////////////////////////////////
// Name:        brush.cpp
// Purpose:
// Author:      Vaclav Slavik
// Id:          $Id: brush.cpp,v 1.9 2004/05/23 20:52:45 JS Exp $
// Copyright:   (c) 2001-2002 SciTech Software, Inc. (www.scitechsoft.com)
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "brush.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/brush.h"
#include "wx/mgl/private.h"
#include "wx/mgl/dcmemory.h"


// ---------------------------------------------------------------------------
// helper functions
// ---------------------------------------------------------------------------

// This function converts wxBitmap into pixpattern24_t representation
// (used by wxBrush and wxPen)

void wxBitmapToPixPattern(const wxBitmap& bitmap, 
                          pixpattern24_t *pix, pattern_t *mask)
{
    wxMemoryDC mem;
    MGLDevCtx *dc;
    int x, y;
    
    if ( pix != NULL )
    {
        mem.SelectObject(bitmap);
        dc = mem.GetMGLDC();
        wxCurrentDCSwitcher curDC(dc);
        dc->beginPixel();
        for (y = 0; y < 8; y++)
            for (x = 0; x < 8; x++)
                dc->unpackColorFast(dc->getPixelFast(x, y), 
                                    pix->p[y][x][2],
                                    pix->p[y][x][1], 
                                    pix->p[y][x][0]);
        dc->endPixel();
    }

    if ( mask && bitmap.GetMask() )
    {
        mem.SelectObject(*bitmap.GetMask()->GetBitmap());
        dc = mem.GetMGLDC();
        wxCurrentDCSwitcher curDC(dc);
        dc->beginPixel();
        for (y = 0; y < 8; y++)
        {
            mask->p[y] = 0;
            for (x = 0; x < 8; x++)
                if ( dc->getPixelFast(x, y) != 0 )
                    mask->p[y] |= 1 << (7 - x);
        }
        dc->endPixel();
    }
}


//-----------------------------------------------------------------------------
// wxBrush
//-----------------------------------------------------------------------------

class wxBrushRefData: public wxObjectRefData
{
public:
    wxBrushRefData();
    wxBrushRefData(const wxBrushRefData& data);

    int            m_style;
    wxColour       m_colour;
    wxBitmap       m_stipple;
    pixpattern24_t m_pixPattern;
    pattern_t      m_maskPattern;
};

wxBrushRefData::wxBrushRefData()
{
    m_style = 0;

    int x, y, c;
    for (y = 0; y < 8; y++)
        for (x = 0; x < 8; x++)
            for (c = 0; c < 3; c++)
                m_pixPattern.p[y][x][c] = 0;
    for (y = 0; y < 8; y++)
        m_maskPattern.p[y] = 0;
}

wxBrushRefData::wxBrushRefData(const wxBrushRefData& data)
{
    m_style = data.m_style;
    m_stipple = data.m_stipple;
    m_colour = data.m_colour;

    int x, y, c;
    for (y = 0; y < 8; y++)
        for (x = 0; x < 8; x++)
            for (c = 0; c < 3; c++)
                m_pixPattern.p[y][x][c] = data.m_pixPattern.p[y][x][c];
    for (y = 0; y < 8; y++)
        m_maskPattern.p[y] = data.m_maskPattern.p[y];
}

//-----------------------------------------------------------------------------

#define M_BRUSHDATA ((wxBrushRefData *)m_refData)

IMPLEMENT_DYNAMIC_CLASS(wxBrush,wxGDIObject)

wxBrush::wxBrush(const wxColour &colour, int style)
{
    m_refData = new wxBrushRefData();
    M_BRUSHDATA->m_style = style;
    M_BRUSHDATA->m_colour = colour;
}

wxBrush::wxBrush(const wxBitmap &stippleBitmap)
{
    wxCHECK_RET( stippleBitmap.Ok(), _T("invalid bitmap") );
    wxCHECK_RET( stippleBitmap.GetWidth() == 8 && stippleBitmap.GetHeight() == 8, 
                  _T("stipple bitmap must be 8x8") );

    m_refData = new wxBrushRefData();
    M_BRUSHDATA->m_colour = *wxBLACK;
    
    M_BRUSHDATA->m_stipple = stippleBitmap;
    wxBitmapToPixPattern(stippleBitmap, &(M_BRUSHDATA->m_pixPattern), 
                                        &(M_BRUSHDATA->m_maskPattern));

    if (M_BRUSHDATA->m_stipple.GetMask())
		M_BRUSHDATA->m_style = wxSTIPPLE_MASK_OPAQUE;
	else
		M_BRUSHDATA->m_style = wxSTIPPLE;
}

wxBrush::wxBrush(const wxBrush &brush)
{
    Ref(brush);
}

wxBrush& wxBrush::operator = (const wxBrush& brush)
{
    if (*this == brush) return (*this);
    Ref(brush);
    return *this;
}

bool wxBrush::operator == (const wxBrush& brush) const
{
    return m_refData == brush.m_refData;
}

bool wxBrush::operator != (const wxBrush& brush) const
{
    return m_refData != brush.m_refData;
}

bool wxBrush::Ok() const
{
    return ((m_refData) && M_BRUSHDATA->m_colour.Ok());
}

int wxBrush::GetStyle() const
{
    if (m_refData == NULL)
    {
        wxFAIL_MSG( wxT("invalid brush") );
        return 0;
    }

    return M_BRUSHDATA->m_style;
}

wxColour &wxBrush::GetColour() const
{
    if (m_refData == NULL)
    {
        wxFAIL_MSG( wxT("invalid brush") );
        return wxNullColour;
    }

    return M_BRUSHDATA->m_colour;
}

wxBitmap *wxBrush::GetStipple() const
{
    if (m_refData == NULL)
    {
        wxFAIL_MSG( wxT("invalid brush") );
        return &wxNullBitmap;
    }

    return &M_BRUSHDATA->m_stipple;
}

void* wxBrush::GetMaskPattern() const
{
    wxCHECK_MSG( Ok(), NULL, wxT("invalid brush") );

    return (void*)&(M_BRUSHDATA->m_maskPattern);
}

void* wxBrush::GetPixPattern() const
{
    wxCHECK_MSG( Ok(), NULL, wxT("invalid brush") );

    return (void*)&(M_BRUSHDATA->m_pixPattern);
}

void wxBrush::SetColour(const wxColour& col)
{
    AllocExclusive();
    M_BRUSHDATA->m_colour = col;
}

void wxBrush::SetColour(unsigned char r, unsigned char g, unsigned char b)
{
    AllocExclusive();
    M_BRUSHDATA->m_colour.Set(r, g, b);
}

void wxBrush::SetStyle( int style )
{
    AllocExclusive();
    M_BRUSHDATA->m_style = style;
}

void wxBrush::SetStipple(const wxBitmap& stipple)
{
    AllocExclusive();

    wxCHECK_RET( stipple.Ok(), _T("invalid bitmap") );
    wxCHECK_RET( stipple.GetWidth() == 8 && stipple.GetHeight() == 8, 
                  _T("stipple bitmap must be 8x8") );

    M_BRUSHDATA->m_stipple = stipple;
    wxBitmapToPixPattern(stipple, &(M_BRUSHDATA->m_pixPattern),
                                  &(M_BRUSHDATA->m_maskPattern));

    if (M_BRUSHDATA->m_stipple.GetMask())
        M_BRUSHDATA->m_style = wxSTIPPLE_MASK_OPAQUE;
    else
        M_BRUSHDATA->m_style = wxSTIPPLE;
}

wxObjectRefData *wxBrush::CreateRefData() const
{
    return new wxBrushRefData;
}

wxObjectRefData *wxBrush::CloneRefData(const wxObjectRefData *data) const
{
    return new wxBrushRefData(*(wxBrushRefData *)data);
}

