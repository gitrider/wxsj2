/////////////////////////////////////////////////////////////////////////////
// Name:        xh_menu.h
// Purpose:     XML resource handler for menus/menubars
// Author:      Vaclav Slavik
// Created:     2000/03/05
// RCS-ID:      $Id: xh_menu.h,v 1.6 2005/01/07 21:27:19 VS Exp $
// Copyright:   (c) 2000 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_XH_MENU_H_
#define _WX_XH_MENU_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "xh_menu.h"
#endif

#include "wx/xrc/xmlres.h"

class WXDLLIMPEXP_XRC wxMenuXmlHandler : public wxXmlResourceHandler
{
DECLARE_DYNAMIC_CLASS(wxMenuXmlHandler)
public:
    wxMenuXmlHandler();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);

private:
    bool m_insideMenu;
};

class WXDLLIMPEXP_XRC wxMenuBarXmlHandler : public wxXmlResourceHandler
{
    DECLARE_DYNAMIC_CLASS(wxMenuBarXmlHandler)
    public:
        wxMenuBarXmlHandler();
        virtual wxObject *DoCreateResource();
        virtual bool CanHandle(wxXmlNode *node);
};


#endif // _WX_XH_MENU_H_
