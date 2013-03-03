/////////////////////////////////////////////////////////////////////////////
// Name:        palette.h
// Purpose:     wxPalette class
// Author:      David Webster
// Modified by:
// Created:     10/12/99
// RCS-ID:      $Id: palette.h,v 1.10 2004/12/08 17:42:54 ABX Exp $
// Copyright:   (c) David Webster
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_PALETTE_H_
#define _WX_PALETTE_H_

#include "wx/gdiobj.h"
#include "wx/os2/private.h"

class WXDLLEXPORT wxPalette;

class WXDLLEXPORT wxPaletteRefData: public wxGDIRefData
{
    friend class WXDLLEXPORT wxPalette;
public:
    wxPaletteRefData();
    ~wxPaletteRefData();
// protected:
    WXHPALETTE                      m_hPalette;
    HPS                             m_hPS;
}; // end of CLASS wxPaletteRefData

#define M_PALETTEDATA ((wxPaletteRefData *)m_refData)

class WXDLLEXPORT wxPalette: public wxPaletteBase
{
    DECLARE_DYNAMIC_CLASS(wxPalette)

public:
    wxPalette();
    inline wxPalette(const wxPalette& rPalette) { Ref(rPalette); }

    wxPalette( int                  n
              ,const unsigned char* pRed
              ,const unsigned char* pGreen
              ,const unsigned char* pBlue
             );
    ~wxPalette();

    bool Create( int                  n
                ,const unsigned char* pRed
                ,const unsigned char* pGreen
                ,const unsigned char* pBlue
               );
    int  GetPixel( const unsigned char cRed
                  ,const unsigned char cGreen
                  ,const unsigned char cBlue
                 ) const;
    bool GetRGB( int            nPixel
                ,unsigned char* pRed
                ,unsigned char* pGreen
                ,unsigned char* pBlue
               ) const;

    virtual bool Ok(void) const { return (m_refData != NULL) ; }

    inline wxPalette& operator = (const wxPalette& rPalette) { if (*this == rPalette) return (*this); Ref(rPalette); return *this; }
    inline bool       operator == (const wxPalette& rPalette) const
        { return m_refData == rPalette.m_refData; }
    inline bool       operator != (const wxPalette& rPalette) const
        { return m_refData != rPalette.m_refData; }

    virtual bool FreeResource(bool bForce = false);

    inline WXHPALETTE GetHPALETTE(void) const { return (M_PALETTEDATA ? M_PALETTEDATA->m_hPalette : 0); }
    void              SetHPALETTE(WXHPALETTE hPalette);
    void              SetPS(HPS hPS);
}; // end of CLASS wxPalette

#endif
    // _WX_PALETTE_H_

