/////////////////////////////////////////////////////////////////////////////
// Name:        src/cocoa/icon.mm
// Purpose:     wxIcon class
// Author:      David Elliott
// Modified by:
// Created:     2003/08/11
// RCS-ID:      $Id: icon.mm,v 1.4 2004/05/23 15:10:39 JS Exp $
// Copyright:   (c) 2003 David Elliott
// Licence:   	wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/icon.h"
#endif //WX_PRECOMP
#include "wx/bitmap.h"

#include "wx/cocoa/autorelease.h"

#import <AppKit/NSImage.h>

// ========================================================================
// wxIconRefData
// ========================================================================
class wxIconRefData: public wxGDIRefData
{
    friend class wxIcon;
public:
    wxIconRefData();
    wxIconRefData( const wxIconRefData& data );
    virtual ~wxIconRefData();

protected:
    int                 m_width;
    int                 m_height;
    int                 m_depth;
    bool                m_ok;
    int                 m_numColors;
    int                 m_quality;
    WX_NSImage          m_cocoaNSImage;
};

#define M_ICONDATA ((wxIconRefData *)m_refData)

wxIconRefData::wxIconRefData()
{
    m_ok = FALSE;
    m_width = 0;
    m_height = 0;
    m_depth = 0;
    m_quality = 0;
    m_numColors = 0;
    m_cocoaNSImage = nil;
}

wxIconRefData::wxIconRefData( const wxIconRefData& data)
{
    m_width = data.m_width;
    m_height = data.m_height;
    m_depth = data.m_depth;
    m_ok = data.m_ok;
    m_numColors = data.m_numColors;
    m_quality = data.m_quality;
    m_cocoaNSImage = [data.m_cocoaNSImage copyWithZone:nil];
}

wxIconRefData::~wxIconRefData()
{
    wxAutoNSAutoreleasePool pool;
    [m_cocoaNSImage release];
    m_cocoaNSImage = NULL;
}


// ========================================================================
// wxIcon
// ========================================================================
IMPLEMENT_DYNAMIC_CLASS(wxIcon, wxGDIObject)

wxIcon::wxIcon()
{
}

wxIcon::wxIcon(const char WXUNUSED(bits)[], int WXUNUSED(width), int WXUNUSED(height))
{
}

wxIcon::wxIcon(const wxString& icon_file, int flags,
    int desiredWidth, int desiredHeight)

{
    LoadFile(icon_file, (wxBitmapType)flags, desiredWidth, desiredHeight);
}

wxIcon::~wxIcon()
{
}

bool wxIcon::CreateFromXpm(const char **xpm)
{
    wxBitmap bitmap(xpm);
    CopyFromBitmap(bitmap);
    return Ok();
}

bool wxIcon::LoadFile(const wxString& filename, wxBitmapType type,
    int desiredWidth, int desiredHeight)
{
    UnRef();

    m_refData = new wxIconRefData;
    M_ICONDATA->m_width = 5;
    M_ICONDATA->m_height = 5;
    M_ICONDATA->m_cocoaNSImage = [[NSImage alloc] initWithSize:NSMakeSize(5,5)];
    M_ICONDATA->m_ok = true;
    M_ICONDATA->m_numColors = 0;
    M_ICONDATA->m_quality = 0;

    return FALSE;
}

void wxIcon::CopyFromBitmap(const wxBitmap& bitmap)
{
    UnRef();
    m_refData = new wxIconRefData;
    M_ICONDATA->m_width = bitmap.GetWidth();
    M_ICONDATA->m_height = bitmap.GetHeight();
    wxAutoNSAutoreleasePool pool;
    M_ICONDATA->m_cocoaNSImage = [bitmap.GetNSImage(true) retain];
    M_ICONDATA->m_ok = bitmap.Ok();
    M_ICONDATA->m_numColors = 0;
    M_ICONDATA->m_quality = 0;
}

bool wxIcon::Ok() const
{
    return m_refData && M_ICONDATA->m_ok;
}

int wxIcon::GetWidth() const
{
    if(!m_refData)
        return 0;
    return M_ICONDATA->m_width;
}

int wxIcon::GetHeight() const
{
    if(!m_refData)
        return 0;
    return M_ICONDATA->m_height;
}

WX_NSImage wxIcon::GetNSImage() const
{
    if(!M_ICONDATA)
        return nil;
    return M_ICONDATA->m_cocoaNSImage;
}

