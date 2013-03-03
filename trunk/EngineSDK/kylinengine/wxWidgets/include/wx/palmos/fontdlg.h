/////////////////////////////////////////////////////////////////////////////
// Name:        wx/palmos/fontdlg.h
// Purpose:     wxFontDialog class
// Author:      William Osborne - minimal working wxPalmOS port
// Modified by:
// Created:     10/13/04
// RCS-ID:      $Id: fontdlg.h,v 1.2 2005/01/18 21:14:24 ABX Exp $
// Copyright:   (c) William Osborne
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_PALMOS_FONTDLG_H_
#define _WX_PALMOS_FONTDLG_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "fontdlg.h"
#endif

// ----------------------------------------------------------------------------
// wxFontDialog
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxFontDialog : public wxFontDialogBase
{
public:
    wxFontDialog() : wxFontDialogBase() { /* must be Create()d later */ }
    wxFontDialog(wxWindow *parent)
        : wxFontDialogBase(parent) { Create(parent); }
    wxFontDialog(wxWindow *parent, const wxFontData& data)
        : wxFontDialogBase(parent, data) { Create(parent, data); }

    virtual int ShowModal();

    // deprecated interface, don't use
    wxFontDialog(wxWindow *parent, const wxFontData *data)
        : wxFontDialogBase(parent, data) { Create(parent, data); }

protected:
    DECLARE_DYNAMIC_CLASS_NO_COPY(wxFontDialog)
};

#endif
    // _WX_PALMOS_FONTDLG_H_

