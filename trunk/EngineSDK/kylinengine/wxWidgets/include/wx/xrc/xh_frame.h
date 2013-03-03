/////////////////////////////////////////////////////////////////////////////
// Name:        xh_frame.h
// Purpose:     XML resource handler for dialogs
// Author:      Vaclav Slavik & Aleks.
// Created:     2000/03/05
// RCS-ID:      $Id: xh_frame.h,v 1.5 2005/01/07 21:27:19 VS Exp $
// Copyright:   (c) 2000 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_XH_FRAME_H_
#define _WX_XH_FRAME_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "xh_frame.h"
#endif

#include "wx/xrc/xmlres.h"

class WXDLLIMPEXP_XRC wxFrameXmlHandler : public wxXmlResourceHandler
{
DECLARE_DYNAMIC_CLASS(wxFrameXmlHandler)
public:
    wxFrameXmlHandler();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};


#endif // _WX_XH_FRAME_H_
