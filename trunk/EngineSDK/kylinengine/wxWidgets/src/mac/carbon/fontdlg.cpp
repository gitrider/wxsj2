/////////////////////////////////////////////////////////////////////////////
// Name:        fontdlg.cpp
// Purpose:     wxFontDialog class for carbon 10.2+.
// Author:      Ryan Norton
// Modified by:
// Created:     1998-01-01
// RCS-ID:      $Id: fontdlg.cpp,v 1.23 2005/05/10 06:28:21 SC Exp $
// Copyright:   (c) Ryan Norton
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ===========================================================================
// declarations
// ===========================================================================

// ---------------------------------------------------------------------------
// headers
// ---------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "fontdlg.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/cmndata.h"
#endif

#include "wx/fontdlg.h"
    

#if !USE_NATIVE_FONT_DIALOG_FOR_MACOSX

#undef wxFontDialog

#include "wx/mac/fontdlg.h"

IMPLEMENT_DYNAMIC_CLASS(wxFontDialog, wxDialog)

#include "wx/mac/private.h"

// ---------------------------------------------------------------------------
// wxFontDialog stub for mac OS's without a native font dialog
// ---------------------------------------------------------------------------

wxFontDialog::wxFontDialog()
{
    m_dialogParent = NULL;
}

wxFontDialog::wxFontDialog(wxWindow *parent, const wxFontData&  data)
{
    Create(parent, data);
}

wxFontDialog::~wxFontDialog()
{
    // empty
}

void wxFontDialog::SetData(wxFontData& fontdata)
{
    m_fontData = fontdata;
}

bool wxFontDialog::Create(wxWindow *parent, const wxFontData& data)
{
    m_dialogParent = parent;

    m_fontData = data;

    // TODO: you may need to do dialog creation here, unless it's
    // done in ShowModal.
    return TRUE;
}

bool wxFontDialog::IsShown() const
{
    return false;
}

int wxFontDialog::ShowModal()
{
    // TODO: show (maybe create) the dialog
    return wxID_CANCEL;
}

#endif // 10.2+
