/////////////////////////////////////////////////////////////////////////////
// Name:        print.h
// Purpose:     wxOS2Printer, wxOS2PrintPreview classes
// Author:      David Webster
// Modified by:
// Created:     10/14/99
// RCS-ID:      $Id: printos2.h,v 1.1 2004/10/21 21:32:46 SN Exp $
// Copyright:   (c) David Webster
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_PRINT_H_
#define _WX_PRINT_H_

#include "wx/prntbase.h"

/*
 * Represents the printer: manages printing a wxPrintout object
 */

class WXDLLEXPORT wxOS2Printer: public wxPrinterBase
{
  DECLARE_DYNAMIC_CLASS(wxOS2Printer)

 public:
  wxOS2Printer(wxPrintDialogData *data = NULL);
  ~wxOS2Printer();

  virtual bool Print(wxWindow *parent, wxPrintout *printout, bool prompt = TRUE);
  virtual wxDC* PrintDialog(wxWindow *parent);
  virtual bool Setup(wxWindow *parent);
private:
};

/*
 * wxOS2PrintPreview
 * Programmer creates an object of this class to preview a wxPrintout.
 */

class WXDLLEXPORT wxOS2PrintPreview: public wxPrintPreviewBase
{
  DECLARE_CLASS(wxOS2PrintPreview)

 public:
  wxOS2PrintPreview(wxPrintout *printout, wxPrintout *printoutForPrinting = NULL, wxPrintDialogData *data = NULL);
  wxOS2PrintPreview(wxPrintout *printout, wxPrintout *printoutForPrinting, wxPrintData *data);
  ~wxOS2PrintPreview();

  virtual bool Print(bool interactive);
  virtual void DetermineScaling();
};

#endif
    // _WX_PRINT_H_
