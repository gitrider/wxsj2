/////////////////////////////////////////////////////////////////////////////
// Name:        xh_notbk.h
// Purpose:     XML resource handler for wxNotebook
// Author:      Vaclav Slavik
// RCS-ID:      $Id: xh_notbk.h,v 1.5 2005/01/07 21:27:19 VS Exp $
// Copyright:   (c) 2000 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_XH_NOTBK_H_
#define _WX_XH_NOTBK_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "xh_notbk.h"
#endif

#include "wx/xrc/xmlres.h"

#if wxUSE_NOTEBOOK

class WXDLLEXPORT wxNotebook;

class WXDLLIMPEXP_XRC wxNotebookXmlHandler : public wxXmlResourceHandler
{
DECLARE_DYNAMIC_CLASS(wxNotebookXmlHandler)
public:
    wxNotebookXmlHandler();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);

private:
    bool m_isInside;
    wxNotebook *m_notebook;
};

#endif

#endif // _WX_XH_NOTBK_H_
