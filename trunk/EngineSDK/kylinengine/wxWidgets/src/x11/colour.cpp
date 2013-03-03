/////////////////////////////////////////////////////////////////////////////
// Name:        colour.cpp
// Purpose:     wxColour class
// Author:      Julian Smart, Robert Roebling
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id: colour.cpp,v 1.21 2005/02/15 18:43:33 RR Exp $
// Copyright:   (c) Julian Smart, Robert Roebling
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "colour.h"
#endif

#include "wx/gdicmn.h"
#include "wx/app.h"

#include "wx/x11/private.h"

//-----------------------------------------------------------------------------
// wxColour
//-----------------------------------------------------------------------------

class wxColourRefData: public wxObjectRefData
{
public:
    wxColourRefData()
    {
        m_color.red = 0;
        m_color.green = 0;
        m_color.blue = 0;
        m_color.pixel = 0;
        m_colormap = (WXColormap *) NULL;
        m_hasPixel = false;
    }
    wxColourRefData(const wxColourRefData& data):
        wxObjectRefData()
    {
        m_color = data.m_color;
        m_colormap = data.m_colormap;
        m_hasPixel = data.m_hasPixel;
    }

    ~wxColourRefData()
    {
        FreeColour();
    }

    bool operator == (const wxColourRefData& data) const
    {
        return (m_colormap == data.m_colormap &&
                m_hasPixel == data.m_hasPixel &&
                m_color.red == data.m_color.red &&
                m_color.green == data.m_color.green &&
                m_color.blue == data.m_color.blue &&
                m_color.pixel == data.m_color.pixel);
    }

    void FreeColour();
    void AllocColour( WXColormap cmap );

    XColor       m_color;
    WXColormap   m_colormap;
    bool         m_hasPixel;

    friend class wxColour;

    // reference counter for systems with <= 8-Bit display
    static unsigned short colMapAllocCounter[ 256 ];
};

unsigned short wxColourRefData::colMapAllocCounter[ 256 ] =
{
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void wxColourRefData::FreeColour()
{
    if (!m_colormap)
        return;
#if !wxUSE_NANOX
    if ( wxTheApp &&
         (wxTheApp->m_visualInfo->m_visualType == GrayScale ||
          wxTheApp->m_visualInfo->m_visualType == PseudoColor) )
    {
        int idx = m_color.pixel;
        colMapAllocCounter[ idx ] = colMapAllocCounter[ idx ] - 1;

        if (colMapAllocCounter[ idx ] == 0)
        {
            unsigned long pixel = m_color.pixel;
            XFreeColors( wxGlobalDisplay(), (Colormap) m_colormap, &pixel, 1, 0 );
        }
    }
#endif
}

void wxColourRefData::AllocColour( WXColormap cmap )
{
    if (m_hasPixel && (m_colormap == cmap))
        return;

    FreeColour();

#if !wxUSE_NANOX
    if ((wxTheApp->m_visualInfo->m_visualType == GrayScale) ||
        (wxTheApp->m_visualInfo->m_visualType == PseudoColor))
    {
        m_hasPixel = XAllocColor( wxGlobalDisplay(), (Colormap) cmap, &m_color );
        int idx = m_color.pixel;
        colMapAllocCounter[ idx ] = colMapAllocCounter[ idx ] + 1;
    }
    else
#endif
    {
        m_hasPixel = XAllocColor( wxGlobalDisplay(), (Colormap) cmap, &m_color );
    }

    m_colormap = cmap;
}

//-----------------------------------------------------------------------------

#define M_COLDATA ((wxColourRefData *)m_refData)

#define SHIFT (8*(sizeof(short int)-sizeof(char)))

IMPLEMENT_DYNAMIC_CLASS(wxColour,wxGDIObject)

wxColour::wxColour( unsigned char red, unsigned char green, unsigned char blue )
{
    m_refData = new wxColourRefData();
#if wxUSE_NANOX
    M_COLDATA->m_color.red = ((unsigned short)red) ;
    M_COLDATA->m_color.green = ((unsigned short)green) ;
    M_COLDATA->m_color.blue = ((unsigned short)blue) ;
#else
    M_COLDATA->m_color.red = ((unsigned short)red) << SHIFT;
    M_COLDATA->m_color.green = ((unsigned short)green) << SHIFT;
    M_COLDATA->m_color.blue = ((unsigned short)blue) << SHIFT;
#endif
    M_COLDATA->m_color.pixel = 0;
}

/* static */
wxColour wxColour::CreateByName(const wxString& name)
{
    wxColour col;

    Display *dpy = wxGlobalDisplay();
    WXColormap colormap = wxTheApp->GetMainColormap( dpy );
    XColor xcol;
    if ( XParseColor( dpy, (Colormap)colormap, name.mb_str(), &xcol ) )
    {
        wxColourRefData *refData = new wxColourRefData;
        refData->m_colormap = colormap;
        refData->m_color = xcol;
        col.m_refData = refData;
    }

    return col;
}

void wxColour::InitFromName( const wxString &colourName )
{
    // check the cache first
    wxColour col;
    if ( wxTheColourDatabase )
    {
        col = wxTheColourDatabase->Find(colourName);
    }

    if ( !col.Ok() )
    {
        col = CreateByName(colourName);
    }

    if ( col.Ok() )
    {
        *this = col;
    }
    else
    {
        wxFAIL_MSG( wxT("wxColour: couldn't find colour") );
    }
}

wxColour::~wxColour()
{
}

bool wxColour::operator == ( const wxColour& col ) const
{
    if (m_refData == col.m_refData) return true;

    if (!m_refData || !col.m_refData) return false;

    XColor *own = &(((wxColourRefData*)m_refData)->m_color);
    XColor *other = &(((wxColourRefData*)col.m_refData)->m_color);

    return (own->red == other->red)
        && (own->green == other->green)
        && (own->blue == other->blue) ;

}

wxObjectRefData *wxColour::CreateRefData() const
{
    return new wxColourRefData;
}

wxObjectRefData *wxColour::CloneRefData(const wxObjectRefData *data) const
{
    return new wxColourRefData(*(wxColourRefData *)data);
}

void wxColour::Set( unsigned char red, unsigned char green, unsigned char blue )
{
    AllocExclusive();

#if wxUSE_NANOX
    M_COLDATA->m_color.red = ((unsigned short)red) ;
    M_COLDATA->m_color.green = ((unsigned short)green) ;
    M_COLDATA->m_color.blue = ((unsigned short)blue) ;
#else
    M_COLDATA->m_color.red = ((unsigned short)red) << SHIFT;
    M_COLDATA->m_color.green = ((unsigned short)green) << SHIFT;
    M_COLDATA->m_color.blue = ((unsigned short)blue) << SHIFT;
#endif
    M_COLDATA->m_color.pixel = 0;
}

unsigned char wxColour::Red() const
{
    wxCHECK_MSG( Ok(), 0, wxT("invalid colour") );

#if wxUSE_NANOX
    return (unsigned char) M_COLDATA->m_color.red ;
#else
    return (unsigned char)(M_COLDATA->m_color.red >> SHIFT);
#endif
}

unsigned char wxColour::Green() const
{
    wxCHECK_MSG( Ok(), 0, wxT("invalid colour") );

#if wxUSE_NANOX
    return (unsigned char) M_COLDATA->m_color.green ;
#else
    return (unsigned char)(M_COLDATA->m_color.green >> SHIFT);
#endif
}

unsigned char wxColour::Blue() const
{
    wxCHECK_MSG( Ok(), 0, wxT("invalid colour") );

#if wxUSE_NANOX
    return (unsigned char) M_COLDATA->m_color.blue ;
#else
    return (unsigned char)(M_COLDATA->m_color.blue >> SHIFT);
#endif
}

void wxColour::CalcPixel( WXColormap cmap )
{
    wxCHECK_RET( Ok(), wxT("invalid colour") );

    wxCHECK_RET( cmap, wxT("invalid colormap") );

    M_COLDATA->AllocColour( cmap );
}

unsigned long wxColour::GetPixel() const
{
    wxCHECK_MSG( Ok(), 0, wxT("invalid colour") );

    return M_COLDATA->m_color.pixel;
}

WXColor *wxColour::GetColor() const
{
    wxCHECK_MSG( Ok(), (WXColor *) NULL, wxT("invalid colour") );

    return (WXColor*) &M_COLDATA->m_color;
}
