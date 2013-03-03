///////////////////////////////////////////////////////////////////////////////
// Name:        mac/dataobj.h
// Purpose:     declaration of the wxDataObject
// Author:      Stefan Csomor (adapted from Robert Roebling's gtk port)
// Modified by:
// Created:     10/21/99
// RCS-ID:      $Id: dataobj.h,v 1.3 2004/05/23 20:51:09 JS Exp $
// Copyright:   (c) 1998, 1999 Vadim Zeitlin, Robert Roebling
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_MAC_DATAOBJ_H_
#define _WX_MAC_DATAOBJ_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "dataobj.h"
#endif

// ----------------------------------------------------------------------------
// wxDataObject is the same as wxDataObjectBase under wxGTK
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxDataObject : public wxDataObjectBase
{
public:
    wxDataObject();
#ifdef __DARWIN__
    ~wxDataObject() { }
#endif

    virtual bool IsSupportedFormat( const wxDataFormat& format, Direction dir = Get ) const;
};

#endif // _WX_MAC_DATAOBJ_H_

