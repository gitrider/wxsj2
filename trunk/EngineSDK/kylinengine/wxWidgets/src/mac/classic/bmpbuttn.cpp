/////////////////////////////////////////////////////////////////////////////
// Name:        bmpbuttn.cpp
// Purpose:     wxBitmapButton
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// RCS-ID:      $Id: bmpbuttn.cpp,v 1.5 2004/08/18 15:37:00 DS Exp $
// Copyright:   (c) Stefan Csomor
// Licence:       wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "bmpbuttn.h"
#endif

#include "wx/window.h"
#include "wx/bmpbuttn.h"

#if !USE_SHARED_LIBRARY
IMPLEMENT_DYNAMIC_CLASS(wxBitmapButton, wxButton)
#endif

#include "wx/mac/uma.h"
#include "wx/bitmap.h"

bool wxBitmapButton::Create(wxWindow *parent, wxWindowID id, const wxBitmap& bitmap,
           const wxPoint& pos,
           const wxSize& size, long style,
           const wxValidator& validator,
           const wxString& name)
{
    // since bitmapbuttonbase is subclass of button calling wxBitmapButtonBase::Create
    // essentially creates an additional button
    if ( !wxControl::Create(parent, id, pos, size,
                                     style, validator, name) )
        return false;

    m_bmpNormal = bitmap;
 
    if (style & wxBU_AUTODRAW)
    {
        m_marginX = wxDEFAULT_BUTTON_MARGIN;
        m_marginY = wxDEFAULT_BUTTON_MARGIN;
    }
    else
    {
        m_marginX = 0;
        m_marginY = 0;
    }

    int width = size.x;
    int height = size.y;

    if ( bitmap.Ok() )
    {
        wxSize newSize = DoGetBestSize();
        if ( width == -1 )
            width = newSize.x;
        if ( height == -1 )
            height = newSize.y;
    }

    Rect bounds ;
    Str255 title ;
    m_bmpNormal = bitmap;
    wxBitmapRefData * bmap = NULL ;
    
    if ( m_bmpNormal.Ok() )
        bmap = (wxBitmapRefData*) ( m_bmpNormal.GetRefData()) ;
    
    MacPreControlCreate( parent , id ,  wxEmptyString , pos , wxSize( width , height ) ,style, validator , name , &bounds , title ) ;

    m_macControl = (WXWidget) ::NewControl( MAC_WXHWND(parent->MacGetRootWindow()) , &bounds , title , false , 0 , 
        kControlBehaviorOffsetContents + 
            ( bmap && bmap->m_bitmapType == kMacBitmapTypeIcon ? 
            kControlContentCIconHandle : kControlContentPictHandle ) , 0, 
          (( style & wxBU_AUTODRAW ) ? kControlBevelButtonSmallBevelProc : kControlBevelButtonNormalBevelProc ), (long) this ) ;
    wxASSERT_MSG( (ControlHandle) m_macControl != NULL , wxT("No valid mac control") ) ;
    
    ControlButtonContentInfo info ;
    wxMacCreateBitmapButton( &info , m_bmpNormal ) ;
    if ( info.contentType != kControlNoContent )
    {
        ::SetControlData( (ControlHandle) m_macControl , kControlButtonPart , kControlBevelButtonContentTag , sizeof(info) , (char*) &info ) ;
    }
    MacPostControlCreate() ;

    return TRUE;
}

void wxBitmapButton::SetBitmapLabel(const wxBitmap& bitmap)
{
    m_bmpNormal = bitmap;
    InvalidateBestSize();

    ControlButtonContentInfo info ;
    wxMacCreateBitmapButton( &info , m_bmpNormal ) ;
    if ( info.contentType != kControlNoContent )
    {
        ::SetControlData( (ControlHandle) m_macControl , kControlButtonPart , kControlBevelButtonContentTag , sizeof(info) , (char*) &info ) ;
    }
}


wxSize wxBitmapButton::DoGetBestSize() const
{
    wxSize best;
    if (m_bmpNormal.Ok())
    {
        best.x = m_bmpNormal.GetWidth() + 2*m_marginX;
        best.y = m_bmpNormal.GetHeight() + 2*m_marginY;
    }
    return best;
}
