///////////////////////////////////////////////////////////////////////////////
// Name:        statbar.cpp
// Purpose:     native implementation of wxStatusBar (optional)
// Author:      Stefan Csomor
// Modified by: 
// Created:     1998-01-01
// RCS-ID:      $Id: statbrma.cpp,v 1.24 2005/03/30 15:47:29 JS Exp $
// Copyright:   (c) 1998 Stefan Csomor
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "statbrma.h"
#endif

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include "wx/wxprec.h"

#include "wx/statusbr.h"
#include "wx/dc.h"
#include "wx/dcclient.h"

BEGIN_EVENT_TABLE(wxStatusBarMac, wxStatusBarGeneric)
    EVT_PAINT(wxStatusBarMac::OnPaint)
END_EVENT_TABLE()

#ifdef __WXMAC__
#include "wx/mac/private.h"
#include "wx/toplevel.h"
#endif

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxStatusBarMac class
// ----------------------------------------------------------------------------

wxStatusBarMac::wxStatusBarMac()
{
    SetParent(NULL);
}

wxStatusBarMac::~wxStatusBarMac()
{
}

bool wxStatusBarMac::Create(wxWindow *parent, wxWindowID id,
                            long style ,
                            const wxString& name)
{
    if( !wxStatusBarGeneric::Create( parent , id , style , name ) )
        return FALSE ;
    
    if ( parent->MacGetTopLevelWindow()->MacGetMetalAppearance() )
        MacSetBackgroundBrush( wxNullBrush ) ;    
    
    // normal system font is too tall for fitting into the standard height
    SetWindowVariant( wxWINDOW_VARIANT_SMALL ) ;
    
    return TRUE ;
}

void wxStatusBarMac::DrawFieldText(wxDC& dc, int i)
{
    int leftMargin = 2;
    int w, h ;
    GetSize( &w , &h ) ;
    wxRect rect;
    GetFieldRect(i, rect);
    
    if ( !MacIsReallyHilited()  )
    {
        dc.SetTextForeground( wxColour( 0x80 , 0x80 , 0x80 ) ) ;
    }
    
    wxString text(GetStatusText(i));
    
    long x, y;
    
    dc.GetTextExtent(text, &x, &y);
    
    int xpos = rect.x + leftMargin + 1 ;
    int ypos = 1 ;
    
    if ( MacGetTopLevelWindow()->MacGetMetalAppearance()  )
        ypos++ ;
        
    dc.SetClippingRegion(rect.x, 0, rect.width, h);
    
    dc.DrawText(text, xpos, ypos);
    
    dc.DestroyClippingRegion();
}

void wxStatusBarMac::DrawField(wxDC& dc, int i)
{
    DrawFieldText(dc, i);
}

void wxStatusBarMac::SetStatusText(const wxString& text, int number)
{
    wxCHECK_RET( (number >= 0) && (number < m_nFields),
        _T("invalid status bar field index") );
    
    if ( m_statusStrings[number] == text )
        return ;
    
    m_statusStrings[number] = text;
    wxRect rect;
    GetFieldRect(number, rect);
    int w, h ;
    GetSize( &w , &h ) ;
    rect.y=0;
    rect.height = h ;
    Refresh( TRUE , &rect ) ;
    Update();
}

void wxStatusBarMac::OnPaint(wxPaintEvent& WXUNUSED(event) )
{
  	wxPaintDC dc(this);
  	dc.Clear() ;

    int major,minor;
    wxGetOsVersion( &major, &minor );
    int w, h ;
    GetSize( &w , &h ) ;

	if ( MacIsReallyHilited() )
	{
		wxPen white( *wxWHITE , 1 , wxSOLID ) ;
        if (major >= 10 ) 
        {
            //Finder statusbar border color: (Project builder similar is 9B9B9B)
            if ( MacGetTopLevelWindow()->MacGetMetalAppearance() )
                dc.SetPen(wxPen(wxColour(0x40,40,40) ,1,wxSOLID)) ;
            else
                dc.SetPen(wxPen(wxColour(0xB1,0xB1,0xB1),1,wxSOLID));  
        }
        else
        {
            wxPen black( *wxBLACK , 1 , wxSOLID ) ;
            dc.SetPen(black);
    	}
		dc.DrawLine(0, 0 ,
		       w , 0);
		dc.SetPen(white);
		dc.DrawLine(0, 1 ,
		       w , 1);
	}
	else
	{
        if (major >= 10) 
            //Finder statusbar border color: (Project builder similar is 9B9B9B)
            dc.SetPen(wxPen(wxColour(0xB1,0xB1,0xB1),1,wxSOLID)); 
        else
            dc.SetPen(wxPen(wxColour(0x80,0x80,0x80),1,wxSOLID));

		dc.DrawLine(0, 0 ,
		       w , 0);
	}

	int i;
	if ( GetFont().Ok() )
		dc.SetFont(GetFont());
	dc.SetBackgroundMode(wxTRANSPARENT);

	for ( i = 0; i < m_nFields; i ++ )
		DrawField(dc, i);
}

void wxStatusBarMac::MacHiliteChanged()
{
    Refresh() ;
    Update() ;
}
