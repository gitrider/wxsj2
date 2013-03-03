/////////////////////////////////////////////////////////////////////////////
// Name:        xh_html.h
// Purpose:     XML resource handler for wxHtmlWindow
// Author:      Bob Mitchell
// Created:     2000/03/21
// RCS-ID:      $Id: xh_html.h,v 1.5 2005/01/07 21:27:19 VS Exp $
// Copyright:   (c) 2000 Bob Mitchell and Verant Interactive
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_XH_HTML_H_
#define _WX_XH_HTML_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "xh_html.h"
#endif

#include "wx/xrc/xmlres.h"

#include "wx/defs.h"

#if wxUSE_HTML

class WXDLLIMPEXP_XRC wxHtmlWindowXmlHandler : public wxXmlResourceHandler
{
DECLARE_DYNAMIC_CLASS(wxHtmlWindowXmlHandler)
public:
    wxHtmlWindowXmlHandler();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};

#endif

#endif // _WX_XH_SLIDER_H_
