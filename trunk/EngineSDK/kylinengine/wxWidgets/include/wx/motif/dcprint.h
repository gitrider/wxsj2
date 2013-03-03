/////////////////////////////////////////////////////////////////////////////
// Name:        dcprint.h
// Purpose:     wxPrinterDC class
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id: dcprint.h,v 1.7 2005/02/06 17:38:20 MBN Exp $
// Copyright:   (c) Julian Smart
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_DCPRINT_H_
#define _WX_DCPRINT_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "dcprint.h"
#endif

#include "wx/dc.h"

class WXDLLEXPORT wxPrinterDC: public wxDC
{
public:
    DECLARE_CLASS(wxPrinterDC)
        
    // Create a printer DC
    wxPrinterDC(const wxString& driver, const wxString& device, const wxString& output, bool interactive = true, int orientation = wxPORTRAIT);
    
    ~wxPrinterDC();
};

#endif
// _WX_DCPRINT_H_

