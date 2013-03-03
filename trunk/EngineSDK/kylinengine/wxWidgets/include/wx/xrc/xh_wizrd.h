/////////////////////////////////////////////////////////////////////////////
// Name:        xh_wizrd.h
// Purpose:     XML resource handler for wxWizard
// Author:      Vaclav Slavik
// Created:     2003/03/02
// RCS-ID:      $Id: xh_wizrd.h,v 1.4 2005/01/07 21:27:20 VS Exp $
// Copyright:   (c) 2000 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_XH_WIZRD_H_
#define _WX_XH_WIZRDL_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "xh_wizrd.h"
#endif

#include "wx/xrc/xmlres.h"

#if wxUSE_WIZARDDLG

#include "wx/wizard.h"

class WXDLLIMPEXP_XRC wxWizardXmlHandler : public wxXmlResourceHandler
{
DECLARE_DYNAMIC_CLASS(wxWizardXmlHandler)
public:
    wxWizardXmlHandler();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);

private:
    wxWizard *m_wizard;
    wxWizardPageSimple *m_lastSimplePage;
};

#endif

#endif // _WX_XH_PANEL_H_
