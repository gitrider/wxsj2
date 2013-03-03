/////////////////////////////////////////////////////////////////////////////
// Name:        cocoa/stattext.mm
// Purpose:     wxStaticText
// Author:      David Elliott
// Modified by:
// Created:     2003/02/15
// RCS-ID:      $Id: 
// Copyright:   (c) 2003 David Elliott
// Licence:   	wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/app.h"
    #include "wx/stattext.h"
    #include "wx/log.h"
#endif //WX_PRECOMP

#include "wx/cocoa/autorelease.h"
#include "wx/cocoa/string.h"
#include "wx/cocoa/log.h"

#import <Foundation/NSString.h>
#import <AppKit/NSTextField.h>
#include <math.h>

IMPLEMENT_DYNAMIC_CLASS(wxStaticText, wxControl)
BEGIN_EVENT_TABLE(wxStaticText, wxControl)
END_EVENT_TABLE()
WX_IMPLEMENT_COCOA_OWNER(wxStaticText,NSTextField,NSControl,NSView)

bool wxStaticText::Create(wxWindow *parent, wxWindowID winid,
           const wxString& label,
           const wxPoint& pos,
           const wxSize& size,
           long style,
           const wxString& name)
{
    wxAutoNSAutoreleasePool pool;
    if(!CreateControl(parent,winid,pos,size,style,wxDefaultValidator,name))
        return false;
    m_cocoaNSView = NULL;
    SetNSTextField([[NSTextField alloc] initWithFrame:MakeDefaultNSRect(size)]);
    [m_cocoaNSView release];
    [GetNSTextField() setStringValue:wxNSStringWithWxString(label)];
//    [GetNSTextField() setBordered: NO];
    [GetNSTextField() setBezeled: NO];
    [GetNSTextField() setEditable: NO];
    [GetNSTextField() setDrawsBackground: NO];

    [GetNSControl() sizeToFit];
    // Round-up to next integer size
    NSRect nsrect = [m_cocoaNSView frame];
    nsrect.size.width = ceil(nsrect.size.width);
    [m_cocoaNSView setFrameSize: nsrect.size];

    if(m_parent)
        m_parent->CocoaAddChild(this);
    SetInitialFrameRect(pos,size);

    return true;
}

wxStaticText::~wxStaticText()
{
    DisassociateNSTextField(GetNSTextField());
}

void wxStaticText::SetLabel(const wxString& label)
{
    [GetNSTextField() setStringValue:wxNSStringWithWxString(label)];
    NSRect oldFrameRect = [GetNSTextField() frame];
    NSView *superview = [GetNSTextField() superview];
    wxLogTrace(wxTRACE_COCOA_Window_Size, "wxStaticText::SetLabel Old Position: (%d,%d)", GetPosition().x, GetPosition().y);
    [GetNSTextField() sizeToFit];
    NSRect newFrameRect = [GetNSTextField() frame];
    // Ensure new size is an integer so GetSize returns valid data
    newFrameRect.size.height = ceil(newFrameRect.size.height);
    newFrameRect.size.width = ceil(newFrameRect.size.width);
    if(![superview isFlipped])
    {
        newFrameRect.origin.y = oldFrameRect.origin.y + oldFrameRect.size.height - newFrameRect.size.height;
    }
    [GetNSTextField() setFrame:newFrameRect];
    // New origin (wx coords) should always match old origin
    wxLogTrace(wxTRACE_COCOA_Window_Size, "wxStaticText::SetLabel New Position: (%d,%d)", GetPosition().x, GetPosition().y);

    [[GetNSTextField() superview] setNeedsDisplayInRect:oldFrameRect];
    [[GetNSTextField() superview] setNeedsDisplayInRect:newFrameRect];
}

void wxStaticText::Cocoa_didChangeText(void)
{
}

