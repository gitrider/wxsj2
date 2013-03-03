/////////////////////////////////////////////////////////////////////////////
// Name:        accel.h
// Purpose:     wxAcceleratorTable class
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id: accel.h,v 1.10 2004/05/23 20:51:21 JS Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_ACCEL_H_
#define _WX_ACCEL_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "accel.h"
#endif

#include "wx/object.h"
#include "wx/string.h"
#include "wx/event.h"

class WXDLLEXPORT wxAcceleratorTable: public wxObject
{
    DECLARE_DYNAMIC_CLASS(wxAcceleratorTable)
public:
    wxAcceleratorTable();
    wxAcceleratorTable(const wxString& resource); // Load from .rc resource
    wxAcceleratorTable(int n, const wxAcceleratorEntry entries[]); // Load from array
    
    // Copy constructors
    wxAcceleratorTable(const wxAcceleratorTable& accel) { Ref(accel); }
    wxAcceleratorTable(const wxAcceleratorTable* accel) { if (accel) Ref(*accel); }
    
    ~wxAcceleratorTable();
    
    wxAcceleratorTable& operator = (const wxAcceleratorTable& accel) { if (*this == accel) return (*this); Ref(accel); return *this; }
    bool operator == (const wxAcceleratorTable& accel) { return m_refData == accel.m_refData; }
    bool operator != (const wxAcceleratorTable& accel) { return m_refData != accel.m_refData; }
    
    bool Ok() const;
    
    // Implementation only
    int GetCount() const;
    wxAcceleratorEntry* GetEntries() const;
};

WXDLLEXPORT_DATA(extern wxAcceleratorTable) wxNullAcceleratorTable;

#endif
// _WX_ACCEL_H_
