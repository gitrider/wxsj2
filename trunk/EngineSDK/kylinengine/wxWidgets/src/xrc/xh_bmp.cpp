/////////////////////////////////////////////////////////////////////////////
// Name:        xh_bmp.cpp
// Purpose:     XRC resource for wxBitmap and wxIcon
// Author:      Vaclav Slavik
// Created:     2000/09/09
// RCS-ID:      $Id: xh_bmp.cpp,v 1.10 2005/02/10 17:25:11 ABX Exp $
// Copyright:   (c) 2000 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "xh_bmp.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_XRC

#include "wx/xrc/xh_bmp.h"
#include "wx/bitmap.h"

IMPLEMENT_DYNAMIC_CLASS(wxBitmapXmlHandler, wxXmlResourceHandler)

wxBitmapXmlHandler::wxBitmapXmlHandler()
: wxXmlResourceHandler()
{
}

wxObject *wxBitmapXmlHandler::DoCreateResource()
{
    return new wxBitmap(GetBitmap(wxEmptyString));
}

bool wxBitmapXmlHandler::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxBitmap"));
}

IMPLEMENT_DYNAMIC_CLASS(wxIconXmlHandler, wxXmlResourceHandler)

wxIconXmlHandler::wxIconXmlHandler()
: wxXmlResourceHandler()
{
}

wxObject *wxIconXmlHandler::DoCreateResource()
{
    return new wxIcon(GetIcon(wxEmptyString));
}

bool wxIconXmlHandler::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxIcon"));
}

#endif // wxUSE_XRC
