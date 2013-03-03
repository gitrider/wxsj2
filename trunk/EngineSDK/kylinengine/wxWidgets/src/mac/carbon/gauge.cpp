/////////////////////////////////////////////////////////////////////////////
// Name:        gauge.cpp
// Purpose:     wxGauge class
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// RCS-ID:      $Id: gauge.cpp,v 1.29 2005/05/10 06:28:21 SC Exp $
// Copyright:   (c) Stefan Csomor
// Licence:       wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "gauge.h"
#endif

#include "wx/wxprec.h"

#if wxUSE_GAUGE 

#include "wx/gauge.h"

IMPLEMENT_DYNAMIC_CLASS(wxGauge, wxControl)

#include "wx/mac/uma.h"

bool wxGauge::Create(wxWindow *parent, wxWindowID id,
           int range,
           const wxPoint& pos,
           const wxSize& s,
           long style,
           const wxValidator& validator,
           const wxString& name)
{
    m_macIsUserPane = FALSE ;

    if ( !wxGaugeBase::Create(parent, id, range, pos, s, style & 0xE0FFFFFF, validator, name) )
        return false;

    wxSize size = s ;
    /*
    if ( size.x == wxDefaultCoord && size.y == wxDefaultCoord)
    {
        size = wxSize( 200 , 16 ) ;
    }
    */
    Rect bounds = wxMacGetBoundsForControl( this , pos , size ) ;
    m_peer = new wxMacControl(this) ;
    verify_noerr ( CreateProgressBarControl( MAC_WXHWND(parent->MacGetTopLevelWindowRef()) , &bounds , 
     GetValue() , 0 , GetRange() , false /* not indeterminate */ , m_peer->GetControlRefAddr() ) );

    if ( GetValue() == 0 )
        m_peer->SetData<Boolean>( kControlEntireControl , kControlProgressBarAnimatingTag , (Boolean) false ) ;

    MacPostControlCreate(pos,size) ;
    
    return TRUE;
}

void wxGauge::SetRange(int r)
{
    // we are going via the base class in case there is 
    // some change behind the values by it
    wxGaugeBase::SetRange(r) ;
    if ( m_peer && m_peer->Ok() )
        m_peer->SetMaximum( GetRange() ) ;
}

void wxGauge::SetValue(int pos)
{
    // we are going via the base class in case there is 
    // some change behind the values by it
    wxGaugeBase::SetValue(pos) ;
    if ( m_peer && m_peer->Ok() )
    {
        m_peer->SetValue( GetValue() ) ;
        // we turn off animation in the unnecessary situations as this is eating a lot of CPU otherwise
        Boolean shouldAnimate = ( GetValue() > 0 && GetValue() < GetRange() ) ;
        if ( m_peer->GetData<Boolean>( kControlEntireControl , kControlProgressBarAnimatingTag ) != shouldAnimate )
        {
            m_peer->SetData<Boolean>( kControlEntireControl , kControlProgressBarAnimatingTag , shouldAnimate ) ;
            if ( !shouldAnimate )
            {
                Refresh() ;
            }
        }
    }
}

int wxGauge::GetValue() const
{
/*
    if ( m_peer && m_peer->Ok() )
        return m_peer->GetValue() ;
*/
    return m_gaugePos ;
}

#endif // wxUSE_GAUGE 

