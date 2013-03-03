/////////////////////////////////////////////////////////////////////////////
// Name:        cocoa/NSTableView.mm
// Purpose:     wxCocoaNSTableView / wxCocoaNSTableDataSource
// Author:      David Elliott
// Modified by:
// Created:     2003/08/05
// RCS-ID:      $Id: NSTableView.mm,v 1.3 2004/05/23 15:10:39 JS Exp $
// Copyright:   (c) 2003 David Elliott
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

#include "wx/cocoa/ObjcPose.h"

#include "wx/cocoa/NSTableDataSource.h"
#include "wx/cocoa/NSTableView.h"
#import <AppKit/NSTableView.h>

// ----------------------------------------------------------------------------
// globals
// ----------------------------------------------------------------------------
WX_IMPLEMENT_OBJC_INTERFACE_HASHMAP(NSTableView)

// ============================================================================
// @class wxCocoaNSTableDataSource
// ============================================================================
@implementation wxCocoaNSTableDataSource : NSObject

- (int)numberOfRowsInTableView: (NSTableView *)tableView
{
    wxCocoaNSTableView *wxView = wxCocoaNSTableView::GetFromCocoa(tableView);
    wxCHECK_MSG(wxView, 0, wxT("No associated wx object"));
    return wxView->CocoaDataSource_numberOfRows();
}

- (id)tableView:(NSTableView *)tableView
    objectValueForTableColumn: (NSTableColumn *)tableColumn
    row: (int)rowIndex
{
    wxCocoaNSTableView *wxView = wxCocoaNSTableView::GetFromCocoa(tableView);
    wxCHECK_MSG(wxView, nil, wxT("No associated wx object"));
    return wxView->CocoaDataSource_objectForTableColumn(tableColumn,rowIndex);
}

@end // implementation wxCocoaNSTableDataSource

