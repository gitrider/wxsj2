/////////////////////////////////////////////////////////////////////////////
// Name:        xh_unkwn.h
// Purpose:     XML resource handler for unkown widget
// Author:      Vaclav Slavik
// Created:     2000/03/05
// RCS-ID:      $Id: xh_unkwn.h,v 1.5 2005/01/07 21:27:20 VS Exp $
// Copyright:   (c) 2000 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_XH_UNKWN_H_
#define _WX_XH_UNKWN_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "xh_unkwn.h"
#endif

#include "wx/xrc/xmlres.h"


class WXDLLIMPEXP_XRC wxUnknownWidgetXmlHandler : public wxXmlResourceHandler
{
DECLARE_DYNAMIC_CLASS(wxUnknownWidgetXmlHandler)
public:
    wxUnknownWidgetXmlHandler();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};


#endif // _WX_XH_UNKWN_H_
