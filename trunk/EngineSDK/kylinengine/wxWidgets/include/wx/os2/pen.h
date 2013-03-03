/////////////////////////////////////////////////////////////////////////////
// Name:        wx/os2/pen.h
// Purpose:     wxPen class
// Author:      David Webster
// Modified by:
// Created:     10/10/99
// RCS-ID:      $Id: pen.h,v 1.10 2004/05/23 20:51:39 JS Exp $
// Copyright:   (c) David Webster
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_PEN_H_
#define _WX_PEN_H_

#include "wx/gdiobj.h"
#include "wx/bitmap.h"

typedef long wxPMDash;

class WXDLLEXPORT wxPen;

class WXDLLEXPORT wxPenRefData: public wxGDIRefData
{
    friend class WXDLLEXPORT wxPen;
public:
    wxPenRefData();
    wxPenRefData(const wxPenRefData& rData);
    ~wxPenRefData();

protected:
    int                             m_nWidth;
    int                             m_nStyle;
    int                             m_nJoin;
    int                             m_nCap;
    wxBitmap                        m_vStipple;
    wxColour                        m_vColour;
    WXHPEN                          m_hPen;// in OS/2 GPI this will be the PS the pen is associated with
};

#define M_PENDATA ((wxPenRefData *)m_refData)

// Pen
class WXDLLEXPORT wxPen : public wxGDIObject
{
    DECLARE_DYNAMIC_CLASS(wxPen)
public:
    wxPen();
    wxPen( const wxColour& rColour
          ,int             nWidth = 1
          ,int             nStyle = wxSOLID
         );
    wxPen( const wxBitmap& rStipple
          ,int             nWidth
         );
    inline wxPen(const wxPen& rPen) { Ref(rPen); }
    ~wxPen();

    inline wxPen& operator =  (const wxPen& rPen)
        { if (*this == rPen) return (*this); Ref(rPen); return *this; }
    inline bool   operator == (const wxPen& rPen) const
        { return m_refData == rPen.m_refData; }
    inline bool   operator != (const wxPen& rPen) const
        { return m_refData != rPen.m_refData; }

    virtual bool Ok(void) const { return (m_refData != NULL); }

    //
    // Override in order to recreate the pen
    //
    void SetColour(const wxColour& rColour);
    void SetColour( unsigned char cRed
                   ,unsigned char cGreen
                   ,unsigned char cBlue
                  );

    void SetWidth(int nWidth);
    void SetStyle(int nStyle);
    void SetStipple(const wxBitmap& rStipple);
    void SetDashes( int           nNbDashes
                   ,const wxDash* pDash
                  );
    void SetJoin(int nJoin);
    void SetCap(int nCap);
    void SetPS(HPS hPS);

    inline wxColour& GetColour(void) const { return (M_PENDATA ? M_PENDATA->m_vColour : wxNullColour); };
    inline int       GetWidth(void) const { return (M_PENDATA ? M_PENDATA->m_nWidth : 0); };
    inline int       GetStyle(void) const { return (M_PENDATA ? M_PENDATA->m_nStyle : 0); };
    inline int       GetJoin(void) const { return (M_PENDATA ? M_PENDATA->m_nJoin : 0); };
    inline int       GetCap(void) const { return (M_PENDATA ? M_PENDATA->m_nCap : 0); };
    inline int       GetPS(void) const { return (M_PENDATA ? M_PENDATA->m_hPen : 0); };

    inline wxBitmap* GetStipple(void) const { return (M_PENDATA ? (& M_PENDATA->m_vStipple) : (wxBitmap*) NULL); };

    //
    // Implementation
    //

    //
    // Useful helper: create the brush resource
    //
    bool     RealizeResource(void);
    bool     FreeResource(bool bForce = FALSE);
    WXHANDLE GetResourceHandle(void);
    bool     IsFree(void) const;
    void     Unshare(void);

private:
    LINEBUNDLE                     m_vLineBundle;
    AREABUNDLE                     m_vAreaBundle;
}; // end of CLASS wxPen

extern int wx2os2PenStyle(int nWxStyle);

#endif
    // _WX_PEN_H_
