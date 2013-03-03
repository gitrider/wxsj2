/////////////////////////////////////////////////////////////////////////////
// Name:        helpxxxx.cpp
// Purpose:     Help system: native implementation
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// RCS-ID:      $Id: helpxxxx.cpp,v 1.3 2004/05/23 20:52:40 JS Exp $
// Copyright:   (c) Stefan Csomor
// Licence:       wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "helpxxxx.h"
#endif

#include "wx/stubs/helpxxxx.h"

#include <string.h>

#if !USE_SHARED_LIBRARY
IMPLEMENT_DYNAMIC_CLASS(wxXXXXHelpController, wxHelpControllerBase)
#endif

wxXXXXHelpController::wxXXXXHelpController()
{
    m_helpFile = "";
}

wxXXXXHelpController::~wxXXXXHelpController()
{
}

bool wxXXXXHelpController::Initialize(const wxString& filename)
{
    m_helpFile = filename;
    // TODO any other inits
    return TRUE;
}

bool wxXXXXHelpController::LoadFile(const wxString& file)
{
    m_helpFile = file;
    // TODO
    return TRUE;
}

bool wxXXXXHelpController::DisplayContents()
{
    // TODO
    return FALSE;
}

bool wxXXXXHelpController::DisplaySection(int section)
{
    // TODO
    return FALSE;
}

bool wxXXXXHelpController::DisplayBlock(long block)
{
    // TODO
    return FALSE;
}

bool wxXXXXHelpController::KeywordSearch(const wxString& k,
                                         wxHelpSearchMode WXUNUSED(mode))
{
    if (m_helpFile == "") return FALSE;

    // TODO
    return FALSE;
}

// Can't close the help window explicitly in WinHelp
bool wxXXXXHelpController::Quit()
{
    // TODO
    return FALSE;
}

void wxXXXXHelpController::OnQuit()
{
}

