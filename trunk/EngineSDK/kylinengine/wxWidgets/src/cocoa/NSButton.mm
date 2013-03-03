/////////////////////////////////////////////////////////////////////////////
// Name:        cocoa/NSButton.cpp
// Purpose:     wxCocoaNSButton
// Author:      David Elliott
// Modified by:
// Created:     2003/01/31
// RCS-ID:      $Id: NSButton.mm,v 1.7 2004/06/24 15:14:33 DE Exp $
// Copyright:   (c) 2003-2004 David Elliott
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/log.h"
#endif // WX_PRECOMP

#include "wx/cocoa/NSButton.h"
#import <AppKit/NSButton.h>

// ----------------------------------------------------------------------------
// globals
// ----------------------------------------------------------------------------
WX_IMPLEMENT_OBJC_INTERFACE_HASHMAP(NSButton)

// ============================================================================
// @class wxNSButtonTarget
// ============================================================================
@interface wxNSButtonTarget : NSObject
{
}

- (void)wxNSButtonAction: (id)sender;
@end // wxNSButtonTarget

@implementation wxNSButtonTarget :  NSObject
- (void)wxNSButtonAction: (id)sender
{
    wxCocoaNSButton *button = wxCocoaNSButton::GetFromCocoa(sender);
    wxCHECK_RET(button,wxT("wxNSButtonAction received without associated wx object"));
    button->Cocoa_wxNSButtonAction();
}

@end // implementation wxNSButtonTarget

// ============================================================================
// class wxCocoaNSButton
// ============================================================================
const wxObjcAutoRefFromAlloc<struct objc_object*> wxCocoaNSButton::sm_cocoaTarget = [[wxNSButtonTarget alloc] init];

void wxCocoaNSButton::AssociateNSButton(WX_NSButton cocoaNSButton)
{
    if(cocoaNSButton)
    {
        sm_cocoaHash.insert(wxCocoaNSButtonHash::value_type(cocoaNSButton,this));
        [cocoaNSButton setTarget: sm_cocoaTarget];
        [cocoaNSButton setAction: @selector(wxNSButtonAction:)];
    }
}

