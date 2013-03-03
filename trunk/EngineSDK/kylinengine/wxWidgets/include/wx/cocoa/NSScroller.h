///////////////////////////////////////////////////////////////////////////////
// Name:        wx/cocoa/NSScroller.h
// Purpose:     wxCocoaNSScroller class
// Author:      David Elliott
// Modified by:
// Created:     2004/04/27
// RCS-ID:      $Id: NSScroller.h,v 1.1 2004/06/23 15:26:35 DE Exp $
// Copyright:   (c) 2004 David Elliott
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_COCOA_NSSCROLLER_H__
#define _WX_COCOA_NSSCROLLER_H__

#include "wx/hashmap.h"
#include "wx/cocoa/ObjcAssociate.h"
#include "wx/cocoa/ObjcRef.h"

DECLARE_WXCOCOA_OBJC_CLASS(NSScroller);

WX_DECLARE_OBJC_HASHMAP(NSScroller);

class wxCocoaNSScroller
{
    WX_DECLARE_OBJC_INTERFACE_HASHMAP(NSScroller);
public:
    void AssociateNSScroller(WX_NSScroller cocoaNSScroller);
    inline void DisassociateNSScroller(WX_NSScroller cocoaNSScroller)
    {
        if(cocoaNSScroller)
            sm_cocoaHash.erase(cocoaNSScroller);
    }

    virtual void Cocoa_wxNSScrollerAction(void) = 0;
protected:
    static const wxObjcAutoRefFromAlloc<struct objc_object*> sm_cocoaTarget;
};

#endif // _WX_COCOA_NSSCROLLER_H__
