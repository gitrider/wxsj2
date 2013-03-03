/////////////////////////////////////////////////////////////////////////////
// Name:        src/msw/stattext.cpp
// Purpose:     wxStaticText
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id: stattext.cpp,v 1.64 2005/05/18 02:22:59 RD Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "stattext.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#if wxUSE_STATTEXT

#ifndef WX_PRECOMP
    #include "wx/event.h"
    #include "wx/app.h"
    #include "wx/brush.h"
    #include "wx/dcclient.h"
    #include "wx/settings.h"
#endif

#include "wx/stattext.h"
#include "wx/msw/private.h"

#if wxUSE_EXTENDED_RTTI
WX_DEFINE_FLAGS( wxStaticTextStyle )

wxBEGIN_FLAGS( wxStaticTextStyle )
    // new style border flags, we put them first to
    // use them for streaming out
    wxFLAGS_MEMBER(wxBORDER_SIMPLE)
    wxFLAGS_MEMBER(wxBORDER_SUNKEN)
    wxFLAGS_MEMBER(wxBORDER_DOUBLE)
    wxFLAGS_MEMBER(wxBORDER_RAISED)
    wxFLAGS_MEMBER(wxBORDER_STATIC)
    wxFLAGS_MEMBER(wxBORDER_NONE)

    // old style border flags
    wxFLAGS_MEMBER(wxSIMPLE_BORDER)
    wxFLAGS_MEMBER(wxSUNKEN_BORDER)
    wxFLAGS_MEMBER(wxDOUBLE_BORDER)
    wxFLAGS_MEMBER(wxRAISED_BORDER)
    wxFLAGS_MEMBER(wxSTATIC_BORDER)
    wxFLAGS_MEMBER(wxBORDER)

    // standard window styles
    wxFLAGS_MEMBER(wxTAB_TRAVERSAL)
    wxFLAGS_MEMBER(wxCLIP_CHILDREN)
    wxFLAGS_MEMBER(wxTRANSPARENT_WINDOW)
    wxFLAGS_MEMBER(wxWANTS_CHARS)
    wxFLAGS_MEMBER(wxFULL_REPAINT_ON_RESIZE)
    wxFLAGS_MEMBER(wxALWAYS_SHOW_SB )
    wxFLAGS_MEMBER(wxVSCROLL)
    wxFLAGS_MEMBER(wxHSCROLL)

    wxFLAGS_MEMBER(wxST_NO_AUTORESIZE)
    wxFLAGS_MEMBER(wxALIGN_LEFT)
    wxFLAGS_MEMBER(wxALIGN_RIGHT)
    wxFLAGS_MEMBER(wxALIGN_CENTRE)

wxEND_FLAGS( wxStaticTextStyle )

IMPLEMENT_DYNAMIC_CLASS_XTI(wxStaticText, wxControl,"wx/stattext.h")

wxBEGIN_PROPERTIES_TABLE(wxStaticText)
    wxPROPERTY( Label,wxString, SetLabel, GetLabel, wxString() , 0 /*flags*/ , wxT("Helpstring") , wxT("group"))
    wxPROPERTY_FLAGS( WindowStyle , wxStaticTextStyle , long , SetWindowStyleFlag , GetWindowStyleFlag , EMPTY_MACROVALUE, 0 /*flags*/ , wxT("Helpstring") , wxT("group")) // style
wxEND_PROPERTIES_TABLE()

wxBEGIN_HANDLERS_TABLE(wxStaticText)
wxEND_HANDLERS_TABLE()

wxCONSTRUCTOR_6( wxStaticText , wxWindow* , Parent , wxWindowID , Id , wxString , Label , wxPoint , Position , wxSize , Size , long , WindowStyle )
#else
IMPLEMENT_DYNAMIC_CLASS(wxStaticText, wxControl)
#endif

bool wxStaticText::Create(wxWindow *parent,
                          wxWindowID id,
                          const wxString& label,
                          const wxPoint& pos,
                          const wxSize& size,
                          long style,
                          const wxString& name)
{
    if ( !CreateControl(parent, id, pos, size, style, wxDefaultValidator, name) )
        return false;

    if ( !MSWCreateControl(wxT("STATIC"), label, pos, size) )
        return false;

    return true;
}

wxBorder wxStaticText::GetDefaultBorder() const
{
    return wxBORDER_NONE;
}

WXDWORD wxStaticText::MSWGetStyle(long style, WXDWORD *exstyle) const
{
    WXDWORD msStyle = wxControl::MSWGetStyle(style, exstyle);

    // translate the alignment flags to the Windows ones
    //
    // note that both wxALIGN_LEFT and SS_LEFT are equal to 0 so we shouldn't
    // test for them using & operator
    if ( style & wxALIGN_CENTRE )
        msStyle |= SS_CENTER;
    else if ( style & wxALIGN_RIGHT )
        msStyle |= SS_RIGHT;
    else
        msStyle |= SS_LEFT;

    return msStyle;
}

wxSize wxStaticText::DoGetBestSize() const
{
    wxClientDC dc(wx_const_cast(wxStaticText *, this));
    wxFont font(GetFont());
    if (!font.Ok())
        font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
    
    dc.SetFont(font);

    wxCoord widthTextMax, heightTextTotal;
    dc.GetMultiLineTextExtent(GetLabel(), &widthTextMax, &heightTextTotal);

#ifdef __WXWINCE__
    if ( widthTextMax )
        widthTextMax += 2;
#endif // __WXWINCE__

    wxSize best(widthTextMax, heightTextTotal);
    CacheBestSize(best);
    return best;
}

void wxStaticText::DoSetSize(int x, int y, int w, int h, int sizeFlags)
{
    // we need to refresh the window after changing its size as the standard
    // control doesn't always update itself properly
    wxStaticTextBase::DoSetSize(x, y, w, h, sizeFlags);

    Refresh();
}

void wxStaticText::SetLabel(const wxString& label)
{
    wxStaticTextBase::SetLabel(label);

    // adjust the size of the window to fit to the label unless autoresizing is
    // disabled
    if ( !(GetWindowStyle() & wxST_NO_AUTORESIZE) )
    {
        InvalidateBestSize();
        DoSetSize(wxDefaultCoord, wxDefaultCoord, wxDefaultCoord, wxDefaultCoord,
                  wxSIZE_AUTO_WIDTH | wxSIZE_AUTO_HEIGHT);
    }
}


bool wxStaticText::SetFont(const wxFont& font)
{
    bool ret = wxControl::SetFont(font);

    // adjust the size of the window to fit to the label unless autoresizing is
    // disabled
    if ( !(GetWindowStyle() & wxST_NO_AUTORESIZE) )
    {
        InvalidateBestSize();
        DoSetSize(wxDefaultCoord, wxDefaultCoord, wxDefaultCoord, wxDefaultCoord,
                  wxSIZE_AUTO_WIDTH | wxSIZE_AUTO_HEIGHT);
    }

    return ret;
}

#endif // wxUSE_STATTEXT
