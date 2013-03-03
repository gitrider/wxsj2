/////////////////////////////////////////////////////////////////////////////
// Name:        dcscreen.cpp
// Purpose:     wxScreenDC class
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// RCS-ID:      $Id: dcscreen.cpp,v 1.18 2005/05/10 06:28:21 SC Exp $
// Copyright:   (c) Stefan Csomor
// Licence:       wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "dcscreen.h"
#endif

#include "wx/wxprec.h"

#include "wx/dcscreen.h"
#include "wx/mac/uma.h"

IMPLEMENT_DYNAMIC_CLASS(wxScreenDC, wxWindowDC)

// Create a DC representing the whole screen
wxScreenDC::wxScreenDC()
{
    m_macPort = CreateNewPort() ;
    GrafPtr port ;
    GetPort( &port ) ;
    SetPort( (GrafPtr) m_macPort ) ;
    Point pt = { 0,0 } ;
    LocalToGlobal( &pt ) ;     
    SetPort( port ) ;
    m_macLocalOrigin.x = -pt.h ;
    m_macLocalOrigin.y = -pt.v ;
#if wxMAC_USE_CORE_GRAPHICS
    m_macLocalOriginInPort = m_macLocalOrigin ;
#endif
    BitMap screenBits;
    GetQDGlobalsScreenBits( &screenBits );
    m_minX = screenBits.bounds.left ;

    SInt16 height ;
    GetThemeMenuBarHeight( &height ) ;
    m_minY = screenBits.bounds.top + height ;

    m_maxX = screenBits.bounds.right  ;
    m_maxY = screenBits.bounds.bottom ;

#if wxMAC_USE_CORE_GRAPHICS
    m_graphicContext = new wxMacCGContext( port ) ;    
#else
    MacSetRectRgn( (RgnHandle) m_macBoundaryClipRgn , m_minX , m_minY , m_maxX , m_maxY ) ;
    OffsetRgn( (RgnHandle) m_macBoundaryClipRgn , m_macLocalOrigin.x , m_macLocalOrigin.y ) ;
    CopyRgn( (RgnHandle) m_macBoundaryClipRgn , (RgnHandle) m_macCurrentClipRgn ) ;
#endif
    m_ok = TRUE ;    
}

wxScreenDC::~wxScreenDC()
{   
#if wxMAC_USE_CORE_GRAPHICS
    delete m_graphicContext ;
    m_graphicContext = NULL ;
#endif

    if ( m_macPort )
        DisposePort( (CGrafPtr) m_macPort ) ;
}

