/////////////////////////////////////////////////////////////////////////////
// Name:        univ/bmpbuttn.cpp
// Purpose:     wxBitmapButton implementation
// Author:      Vadim Zeitlin
// Modified by:
// Created:     25.08.00
// RCS-ID:      $Id: bmpbuttn.cpp,v 1.14 2005/02/10 17:25:07 ABX Exp $
// Copyright:   (c) 2000 SciTech Software, Inc. (www.scitechsoft.com)
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "univbmpbuttn.h"
#endif

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_BMPBUTTON

#ifndef WX_PRECOMP
    #include "wx/dc.h"
    #include "wx/bmpbuttn.h"
    #include "wx/validate.h"
#endif

#include "wx/univ/renderer.h"

// ============================================================================
// implementation
// ============================================================================

IMPLEMENT_DYNAMIC_CLASS(wxBitmapButton, wxButton)

BEGIN_EVENT_TABLE(wxBitmapButton, wxButton)
    EVT_SET_FOCUS(wxBitmapButton::OnSetFocus)
    EVT_KILL_FOCUS(wxBitmapButton::OnKillFocus)
END_EVENT_TABLE()

// ----------------------------------------------------------------------------
// wxBitmapButton
// ----------------------------------------------------------------------------

bool wxBitmapButton::Create(wxWindow *parent,
                            wxWindowID id,
                            const wxBitmap& bitmap,
                            const wxPoint &pos,
                            const wxSize &size,
                            long style,
                            const wxValidator& validator,
                            const wxString &name)
{
    // we add wxBU_EXACTFIT because the bitmap buttons are not the standard
    // ones and so shouldn't be forced to be of the standard size which is
    // typically too big for them
    if ( !wxButton::Create(parent, id, bitmap, wxEmptyString,
                           pos, size, style | wxBU_EXACTFIT, validator, name) )
        return false;

    m_bmpNormal = bitmap;

    return true;
}

void wxBitmapButton::OnSetBitmap()
{
    wxBitmap bmp;
    if ( !IsEnabled() )
    {
        bmp = m_bmpDisabled;
    }
    else if ( IsPressed() )
    {
        bmp = m_bmpSelected;
    }
    else if ( IsFocused() )
    {
        bmp = m_bmpFocus;
    }
    else
    {
        bmp = m_bmpNormal;
    }

    ChangeBitmap(bmp);
}

bool wxBitmapButton::ChangeBitmap(const wxBitmap& bmp)
{
    wxBitmap bitmap = bmp.Ok() ? bmp : m_bmpNormal;
    if ( bitmap != m_bitmap )
    {
        m_bitmap = bitmap;

        return true;
    }

    return false;
}

bool wxBitmapButton::Enable(bool enable)
{
    if ( !wxButton::Enable(enable) )
        return false;

    if ( !enable && ChangeBitmap(m_bmpDisabled) )
        Refresh();

    return true;
}

bool wxBitmapButton::SetCurrent(bool doit)
{
    ChangeBitmap(doit ? m_bmpFocus : m_bmpNormal);

    return wxButton::SetCurrent(doit);
}

void wxBitmapButton::OnSetFocus(wxFocusEvent& event)
{
    if ( ChangeBitmap(m_bmpFocus) )
        Refresh();

    event.Skip();
}

void wxBitmapButton::OnKillFocus(wxFocusEvent& event)
{
    if ( ChangeBitmap(m_bmpNormal) )
        Refresh();

    event.Skip();
}

void wxBitmapButton::Press()
{
    ChangeBitmap(m_bmpSelected);

    wxButton::Press();
}

void wxBitmapButton::Release()
{
    ChangeBitmap(IsFocused() ? m_bmpFocus : m_bmpNormal);

    wxButton::Release();
}

#endif // wxUSE_BMPBUTTON

