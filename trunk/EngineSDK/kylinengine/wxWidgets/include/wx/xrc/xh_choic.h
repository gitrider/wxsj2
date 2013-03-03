/////////////////////////////////////////////////////////////////////////////
// Name:        xh_choic.h
// Purpose:     XML resource handler for wxChoice
// Author:      Bob Mitchell
// Created:     2000/03/21
// RCS-ID:      $Id: xh_choic.h,v 1.5 2005/01/07 21:27:18 VS Exp $
// Copyright:   (c) 2000 Bob Mitchell and Verant Interactive
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_XH_CHOIC_H_
#define _WX_XH_CHOIC_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "xh_choic.h"
#endif

#include "wx/xrc/xmlres.h"

class WXDLLIMPEXP_XRC wxChoiceXmlHandler : public wxXmlResourceHandler
{
DECLARE_DYNAMIC_CLASS(wxChoiceXmlHandler)
public:
    wxChoiceXmlHandler();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
private:
    bool m_insideBox;
    wxArrayString strList;
};


#endif // _WX_XH_CHOIC_H_
