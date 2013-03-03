/////////////////////////////////////////////////////////////////////////////
// Name:        xh_scwin.h
// Purpose:     XML resource handler for wxScrolledWindow
// Author:      Vaclav Slavik
// Created:     2002/10/18
// RCS-ID:      $Id: xh_scwin.h,v 1.5 2005/01/07 21:27:19 VS Exp $
// Copyright:   (c) 2002 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_XH_SCWIN_H_
#define _WX_XH_SCWIN_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "xh_scwin.h"
#endif

#include "wx/xrc/xmlres.h"
#include "wx/defs.h"



class WXDLLIMPEXP_XRC wxScrolledWindowXmlHandler : public wxXmlResourceHandler
{
DECLARE_DYNAMIC_CLASS(wxScrolledWindowXmlHandler)
public:
    wxScrolledWindowXmlHandler();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};


#endif // _WX_XH_SCWIN_H_
