/////////////////////////////////////////////////////////////////////////////
// Name:        xh_frame.cpp
// Purpose:     XRC resource for dialogs
// Author:      Vaclav Slavik & Aleks.
// Created:     2000/03/05
// RCS-ID:      $Id: xh_frame.cpp,v 1.19 2005/03/28 09:28:08 VS Exp $
// Copyright:   (c) 2000 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "xh_frame.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_XRC

#include "wx/xrc/xh_frame.h"
#include "wx/frame.h"
#include "wx/dialog.h" // to get wxDEFAULT_DIALOG_STYLE
#include "wx/log.h"
#include "wx/intl.h"

IMPLEMENT_DYNAMIC_CLASS(wxFrameXmlHandler, wxXmlResourceHandler)

wxFrameXmlHandler::wxFrameXmlHandler() : wxXmlResourceHandler()
{
    XRC_ADD_STYLE(wxSTAY_ON_TOP);
    XRC_ADD_STYLE(wxCAPTION);
    XRC_ADD_STYLE(wxDEFAULT_DIALOG_STYLE);
    XRC_ADD_STYLE(wxDEFAULT_FRAME_STYLE);
    XRC_ADD_STYLE(wxTHICK_FRAME);
    XRC_ADD_STYLE(wxSYSTEM_MENU);
    XRC_ADD_STYLE(wxRESIZE_BORDER);
    XRC_ADD_STYLE(wxRESIZE_BOX);
    XRC_ADD_STYLE(wxCLOSE_BOX);

    XRC_ADD_STYLE(wxFRAME_NO_TASKBAR);
    XRC_ADD_STYLE(wxFRAME_SHAPED);
    XRC_ADD_STYLE(wxFRAME_TOOL_WINDOW);
    XRC_ADD_STYLE(wxFRAME_FLOAT_ON_PARENT);
    XRC_ADD_STYLE(wxMAXIMIZE_BOX);
    XRC_ADD_STYLE(wxMINIMIZE_BOX);
    XRC_ADD_STYLE(wxSTAY_ON_TOP);

    XRC_ADD_STYLE(wxNO_3D);
    XRC_ADD_STYLE(wxTAB_TRAVERSAL);
    XRC_ADD_STYLE(wxWS_EX_VALIDATE_RECURSIVELY);
    XRC_ADD_STYLE(wxFRAME_EX_METAL);

    AddWindowStyles();
}

wxObject *wxFrameXmlHandler::DoCreateResource()
{
    XRC_MAKE_INSTANCE(frame, wxFrame);

    frame->Create(m_parentAsWindow,
                  GetID(),
                  GetText(wxT("title")),
                  wxDefaultPosition, wxDefaultSize,
                  GetStyle(wxT("style"), wxDEFAULT_FRAME_STYLE),
                  GetName());

    if (HasParam(wxT("size")))
        frame->SetClientSize(GetSize(wxT("size"), frame));
    if (HasParam(wxT("pos")))
        frame->Move(GetPosition());
    if (HasParam(wxT("icon")))
        frame->SetIcon(GetIcon(wxT("icon"), wxART_FRAME_ICON));

    SetupWindow(frame);

    CreateChildren(frame);

    if (GetBool(wxT("centered"), false))
        frame->Centre();

    return frame;
}

bool wxFrameXmlHandler::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxFrame"));
}

#endif // wxUSE_XRC
