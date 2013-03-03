/////////////////////////////////////////////////////////////////////////////
// Name:        xh_listb.h
// Purpose:     XML resource handler for wxListbox
// Author:      Bob Mitchell & Vaclav Slavik
// Created:     2000/07/29
// RCS-ID:      $Id: xh_listb.h,v 1.5 2005/01/07 21:27:19 VS Exp $
// Copyright:   (c) 2000 Bob Mitchell & Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_XH_LISTB_H_
#define _WX_XH_LISTB_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "xh_listb.h"
#endif

#include "wx/xrc/xmlres.h"

#if wxUSE_LISTBOX

class WXDLLIMPEXP_XRC wxListBoxXmlHandler : public wxXmlResourceHandler
{
DECLARE_DYNAMIC_CLASS(wxListBoxXmlHandler)
public:
    wxListBoxXmlHandler();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
private:
    bool m_insideBox;
    wxArrayString strList;
};

#endif

#endif // _WX_XH_LISTB_H_
