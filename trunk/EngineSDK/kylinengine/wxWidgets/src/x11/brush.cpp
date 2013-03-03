/////////////////////////////////////////////////////////////////////////////
// Name:        src/x11/brush.cpp
// Purpose:     wxBrush
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id: brush.cpp,v 1.8 2004/05/23 20:53:29 JS Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "brush.h"
#endif

#include "wx/setup.h"
#include "wx/utils.h"
#include "wx/brush.h"
#include "wx/bitmap.h"
#include "wx/colour.h"

//-----------------------------------------------------------------------------
// wxBrush
//-----------------------------------------------------------------------------

class wxBrushRefData: public wxObjectRefData
{
public:
    wxBrushRefData()
    {
        m_style = 0;
    }
    
    wxBrushRefData( const wxBrushRefData& data )
    {
        m_style = data.m_style;
        m_stipple = data.m_stipple;
        m_colour = data.m_colour;
    }
    
    bool operator == (const wxBrushRefData& data) const
    {
        return (m_style == data.m_style &&
                m_stipple == data.m_stipple &&
                m_colour == data.m_colour);
    }
    
    int       m_style;
    wxColour  m_colour;
    wxBitmap  m_stipple;
};

//-----------------------------------------------------------------------------

#define M_BRUSHDATA ((wxBrushRefData *)m_refData)

IMPLEMENT_DYNAMIC_CLASS(wxBrush,wxGDIObject)

wxBrush::wxBrush( const wxColour &colour, int style )
{
    m_refData = new wxBrushRefData();
    M_BRUSHDATA->m_style = style;
    M_BRUSHDATA->m_colour = colour;
}

wxBrush::wxBrush( const wxBitmap &stippleBitmap )
{
    m_refData = new wxBrushRefData();
    M_BRUSHDATA->m_colour = *wxBLACK;

    M_BRUSHDATA->m_stipple = stippleBitmap;

    if (M_BRUSHDATA->m_stipple.GetMask())
        M_BRUSHDATA->m_style = wxSTIPPLE_MASK_OPAQUE;
    else
        M_BRUSHDATA->m_style = wxSTIPPLE;
}

wxBrush::~wxBrush()
{
    // m_refData unrefed in ~wxObject
}

wxObjectRefData *wxBrush::CreateRefData() const
{
    return new wxBrushRefData;
}

wxObjectRefData *wxBrush::CloneRefData(const wxObjectRefData *data) const
{
    return new wxBrushRefData(*(wxBrushRefData *)data);
}

bool wxBrush::operator == ( const wxBrush& brush ) const
{
    if (m_refData == brush.m_refData) return TRUE;
    
    if (!m_refData || !brush.m_refData) return FALSE;
    
    return ( *(wxBrushRefData*)m_refData == *(wxBrushRefData*)brush.m_refData );
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

void wxBrush::SetColour( const wxColour& col )
{
    AllocExclusive();
    
    M_BRUSHDATA->m_colour = col;
}

void wxBrush::SetColour( unsigned char r, unsigned char g, unsigned char b )
{
    AllocExclusive();
    
    M_BRUSHDATA->m_colour.Set( r, g, b );
}

void wxBrush::SetStyle( int style )
{
    AllocExclusive();
    
    M_BRUSHDATA->m_style = style;
}

void wxBrush::SetStipple( const wxBitmap& stipple )
{
    AllocExclusive();
    
    M_BRUSHDATA->m_stipple = stipple;
    if (M_BRUSHDATA->m_stipple.GetMask())
    {
        M_BRUSHDATA->m_style = wxSTIPPLE_MASK_OPAQUE;
    }
    else
    {
        M_BRUSHDATA->m_style = wxSTIPPLE;
    }
}
