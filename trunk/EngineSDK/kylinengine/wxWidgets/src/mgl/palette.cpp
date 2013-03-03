/////////////////////////////////////////////////////////////////////////////
// Name:        palette.cpp
// Author:      Vaclav Slavik
// Created:     2001/03/11
// Id:          $Id: palette.cpp,v 1.7 2004/05/23 20:52:46 JS Exp $
// Copyright:   (c) 2001-2002 SciTech Software, Inc. (www.scitechsoft.com)
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "palette.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/palette.h"
#include <mgraph.h>


//-----------------------------------------------------------------------------
// wxPalette
//-----------------------------------------------------------------------------

class wxPaletteRefData: public wxObjectRefData
{
    public:
        wxPaletteRefData(void);
        ~wxPaletteRefData(void);
    
        int m_count;
        palette_t *m_entries;
};

wxPaletteRefData::wxPaletteRefData()
{
    m_count = 0;
    m_entries = NULL;
}

wxPaletteRefData::~wxPaletteRefData()
{
    delete[] m_entries;
}

//-----------------------------------------------------------------------------

#define M_PALETTEDATA ((wxPaletteRefData *)m_refData)

IMPLEMENT_DYNAMIC_CLASS(wxPalette,wxGDIObject)

wxPalette::wxPalette()
{
    m_refData = NULL;
}

wxPalette::wxPalette(int n, const unsigned char *red, const unsigned char *green, const unsigned char *blue)
{
    Create(n, red, green, blue);
}

wxPalette::wxPalette(const wxPalette& palette)
{
    Ref(palette);
}

wxPalette::~wxPalette()
{
}

wxPalette& wxPalette::operator = (const wxPalette& palette)
{
    if (*this == palette) return (*this);
    Ref(palette);
    return *this;
}

bool wxPalette::operator == (const wxPalette& palette) const
{
    return m_refData == palette.m_refData;
}

bool wxPalette::operator != (const wxPalette& palette) const
{
    return m_refData != palette.m_refData;
}

bool wxPalette::Ok(void) const
{
    return (m_refData != NULL);
}

bool wxPalette::Create(int n,
                       const unsigned char *red,
                       const unsigned char *green, 
                       const unsigned char *blue)
{
    UnRef();
    m_refData = new wxPaletteRefData();
    
    M_PALETTEDATA->m_count = n;    
    M_PALETTEDATA->m_entries = new palette_t[n];

    palette_t *e = M_PALETTEDATA->m_entries;
    for (int i = 0; i < n; i++, e++)
    {
        e->red = red[i];
        e->green = green[i];
        e->blue = blue[i];
        e->alpha = 0;
    }

    return TRUE;
}

int wxPalette::GetPixel(const unsigned char red,
                        const unsigned char green,
                        const unsigned char blue) const
{
    if (!m_refData) return FALSE;

	int closest = 0;
	double d,distance = 1000.0; // max. dist is 256

    palette_t *e = M_PALETTEDATA->m_entries;
    for (int i = 0; i < M_PALETTEDATA->m_count; i++, e++)
    {
        if ((d = 0.299 * abs(red - e->red) +
                 0.587 * abs(green - e->green) +
                 0.114 * abs(blue - e->blue)) < distance) {
            distance = d;
            closest = i;
        }
    }
	return closest;
}

bool wxPalette::GetRGB(int pixel, 
                       unsigned char *red,
                       unsigned char *green, 
                       unsigned char *blue) const
{
    if (!m_refData) return FALSE;
    if (pixel >= M_PALETTEDATA->m_count) return FALSE;
    
    palette_t& p = M_PALETTEDATA->m_entries[pixel];
    if (red) *red = p.red;
    if (green) *green = p.green;
    if (blue) *blue = p.blue;
    return TRUE;
}

int wxPalette::GetColoursCount() const
{
    wxCHECK_MSG( Ok(), 0, wxT("invalid palette") );   
    return M_PALETTEDATA->m_count;
}

palette_t *wxPalette::GetMGLpalette_t() const
{
    wxCHECK_MSG( Ok(), NULL, wxT("invalid palette") );   
    return M_PALETTEDATA->m_entries;
}

