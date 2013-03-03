/////////////////////////////////////////////////////////////////////////////
// Name:        colour.h
// Purpose:     wxColour class
// Author:      David Webster
// Modified by:
// Created:     10/13/99
// RCS-ID:      $Id: colour.h,v 1.12 2004/05/23 20:51:38 JS Exp $
// Copyright:   (c) David Webster
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_COLOUR_H_
#define _WX_COLOUR_H_

#include "wx/object.h"

// Colour
class WXDLLEXPORT wxColour: public wxObject
{
public:
    //
    // Ctors
    //

    //
    // Default
    //
    wxColour();

    //
    // from RGB
    //
    wxColour( unsigned char cRed
             ,unsigned char cGreen
             ,unsigned char cBlue
            );

    wxColour( unsigned long colRGB ) { Set(colRGB); }

    //
    // Implicit conversion from the colour name
    //
    wxColour(const wxString& rColourName) { InitFromName(rColourName); }
    wxColour(const wxChar *zColourName) { InitFromName(zColourName); }

    //
    // Copy ctors and assignment operators
    //
    wxColour(const wxColour& rCol);
    wxColour(const wxColour* pCol);
    wxColour&operator = (const wxColour& rCol);

    //
    // Dtor
    //
    ~wxColour();

    //
    // Set functions
    //
    void Set( unsigned char cRed
             ,unsigned char cGreen
             ,unsigned char cBlue
            );
    void Set(unsigned long lColRGB)
    {
        //
        // We don't need to know sizeof(long) here because we assume that the three
        // least significant bytes contain the R, G and B values
        //
        Set( (unsigned char)lColRGB
            ,(unsigned char)(lColRGB >> 8)
            ,(unsigned char)(lColRGB >> 16)
           );
    }
    void Set(const wxString& rsColour)
    {
        InitFromName(rsColour);
    }

    //
    // Accessors
    //
    bool Ok(void) const {return m_bIsInit; }

    unsigned char Red(void) const { return m_cRed; }
    unsigned char Green(void) const { return m_cGreen; }
    unsigned char Blue(void) const { return m_cBlue; }

    //
    // Comparison
    //
    bool operator == (const wxColour& rColour) const
    {
        return (m_bIsInit == rColour.m_bIsInit
                && m_cRed   == rColour.m_cRed
                && m_cGreen == rColour.m_cGreen
                && m_cBlue  == rColour.m_cBlue
               );
    }

    bool operator != (const wxColour& rColour) const { return !(*this == rColour); }

    WXCOLORREF GetPixel(void) const { return m_vPixel; };

    void InitFromName(const wxString& rCol);

private:

    // Helper function
    void Init();

    bool                            m_bIsInit;
    unsigned char                   m_cRed;
    unsigned char                   m_cBlue;
    unsigned char                   m_cGreen;

public:
    WXCOLORREF                      m_vPixel ;
private:
  DECLARE_DYNAMIC_CLASS(wxColour)
}; // end of class wxColour

#endif
  // _WX_COLOUR_H_
