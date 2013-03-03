/////////////////////////////////////////////////////////////////////////////
// Name:        wx/palmos/accel.h
// Purpose:     wxAcceleratorTable class
// Author:      William Osborne - minimal working wxPalmOS port
// Modified by:
// Created:     10/13/04
// RCS-ID:      $Id: accel.h,v 1.3 2005/01/18 21:14:22 ABX Exp $
// Copyright:   (c) William Osborne
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_ACCEL_H_
#define _WX_ACCEL_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "accel.h"
#endif

// ----------------------------------------------------------------------------
// the accel table has all accelerators for a given window or menu
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxAcceleratorTable : public wxObject
{
public:
    // default ctor
    wxAcceleratorTable();

    // copy ctor
    wxAcceleratorTable(const wxAcceleratorTable& accel) { Ref(accel); }

    // load from .rc resource (Windows specific)
    wxAcceleratorTable(const wxString& resource);

    // initialize from array
    wxAcceleratorTable(int n, const wxAcceleratorEntry entries[]);

    virtual ~wxAcceleratorTable();

    wxAcceleratorTable& operator = (const wxAcceleratorTable& accel) { if ( *this != accel ) Ref(accel); return *this; }

#if WXWIN_COMPATIBILITY_2_4
    bool operator==(const wxAcceleratorTable& accel) const
        { return m_refData == accel.m_refData; }
    bool operator!=(const wxAcceleratorTable& accel) const
        { return !(*this == accel); }
#endif

    bool Ok() const;
    void SetHACCEL(WXHACCEL hAccel);
    WXHACCEL GetHACCEL() const;

    // translate the accelerator, return true if done
    bool Translate(wxWindow *window, WXMSG *msg) const;

private:
    DECLARE_DYNAMIC_CLASS(wxAcceleratorTable)
};

#endif
    // _WX_ACCEL_H_
