/////////////////////////////////////////////////////////////////////////////
// Name:        dcprint.h
// Purpose:     wxPrinterDC class
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// RCS-ID:      $Id: dcprint.h,v 1.3 2004/05/23 20:51:10 JS Exp $
// Copyright:   (c) Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_DCPRINT_H_
#define _WX_DCPRINT_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "dcprint.h"
#endif

#include "wx/dc.h"
#include "wx/cmndata.h"

class wxNativePrinterDC ;

class WXDLLEXPORT wxPrinterDC: public wxDC
{
 public:
  DECLARE_CLASS(wxPrinterDC)

#if wxUSE_PRINTING_ARCHITECTURE
  // Create a printer DC
  wxPrinterDC(const wxPrintData& printdata );
  ~wxPrinterDC();

    virtual bool StartDoc( const wxString& WXUNUSED(message) ) ;
    virtual void EndDoc(void) ;
    virtual void StartPage(void) ;
    virtual void EndPage(void) ;
    wxPrintData& GetPrintData() { return m_printData; }
    virtual void DoGetSize( int *width, int *height ) const;
    
 protected:
    wxPrintData   m_printData ;
    wxNativePrinterDC* m_nativePrinterDC ;
#endif // wxUSE_PRINTING_ARCHITECTURE
};

#endif
    // _WX_DCPRINT_H_

