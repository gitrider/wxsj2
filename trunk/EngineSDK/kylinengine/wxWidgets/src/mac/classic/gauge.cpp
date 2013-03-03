/////////////////////////////////////////////////////////////////////////////
// Name:        gauge.cpp
// Purpose:     wxGauge class
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// RCS-ID:      $Id: gauge.cpp,v 1.5 2004/08/18 15:37:00 DS Exp $
// Copyright:   (c) Stefan Csomor
// Licence:       wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "gauge.h"
#endif

#include "wx/gauge.h"

#if !USE_SHARED_LIBRARY
IMPLEMENT_DYNAMIC_CLASS(wxGauge, wxControl)
#endif

#include "wx/mac/uma.h"

bool wxGauge::Create(wxWindow *parent, wxWindowID id,
           int range,
           const wxPoint& pos,
           const wxSize& s,
           long style,
           const wxValidator& validator,
           const wxString& name)
{
    if ( !wxGaugeBase::Create(parent, id, range, pos, s, style, validator, name) )
        return false;

    wxSize size = s ;
    Rect bounds ;
    Str255 title ;
    m_rangeMax = range ;
    m_gaugePos = 0 ;
    
    if ( size.x == wxDefaultCoord && size.y == wxDefaultCoord)
    {
        size = wxSize( 200 , 16 ) ;
    }
    
    MacPreControlCreate( parent , id ,  wxEmptyString , pos , size ,style & 0xE0FFFFFF /* no borders on mac */ , validator , name , &bounds , title ) ;
    
    m_macControl = (WXWidget) ::NewControl( MAC_WXHWND(parent->MacGetRootWindow()) , &bounds , title , false , 0 , 0 , range, 
        kControlProgressBarProc , (long) this ) ;
    
    MacPostControlCreate() ;
    
    return TRUE;
}

void wxGauge::SetShadowWidth(int w)
{
}

void wxGauge::SetBezelFace(int w)
{
}

void wxGauge::SetRange(int r)
{
    m_rangeMax = r;
    ::SetControl32BitMaximum( (ControlHandle) m_macControl , m_rangeMax ) ;
}

void wxGauge::SetValue(int pos)
{
    m_gaugePos = pos;
       ::SetControl32BitValue( (ControlHandle) m_macControl , m_gaugePos ) ;
}

int wxGauge::GetShadowWidth() const
{
    return 0;
}

int wxGauge::GetBezelFace() const
{
    return 0;
}

int wxGauge::GetRange() const
{
    return m_rangeMax;
}

int wxGauge::GetValue() const
{
    return m_gaugePos;
}

