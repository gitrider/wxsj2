/////////////////////////////////////////////////////////////////////////////
// Name:        xh_wizrd.cpp
// Purpose:     XRC resource for wxWizard
// Author:      Vaclav Slavik
// Created:     2003/03/01
// RCS-ID:      $Id: xh_wizrd.cpp,v 1.9 2005/01/17 19:15:49 JS Exp $
// Copyright:   (c) 2000 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "xh_wizrd.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_XRC && wxUSE_WIZARDDLG

#include "wx/xrc/xh_wizrd.h"
#include "wx/log.h"
#include "wx/wizard.h"

IMPLEMENT_DYNAMIC_CLASS(wxWizardXmlHandler, wxXmlResourceHandler)

wxWizardXmlHandler::wxWizardXmlHandler() : wxXmlResourceHandler()
{
    m_wizard = NULL;
    m_lastSimplePage = NULL;
    XRC_ADD_STYLE(wxWIZARD_EX_HELPBUTTON);
    AddWindowStyles();
}

wxObject *wxWizardXmlHandler::DoCreateResource()
{
    if (m_class == wxT("wxWizard"))
    {
        XRC_MAKE_INSTANCE(wiz, wxWizard)

        long style = GetStyle(wxT("exstyle"), 0);
        if (style != 0)
            wiz->SetExtraStyle(style);
        wiz->Create(m_parentAsWindow,
                    GetID(),
                    GetText(wxT("title")),
                    GetBitmap(),
                    GetPosition());

        wxWizard *old = m_wizard;
        m_wizard = wiz;
        m_lastSimplePage = NULL;
        CreateChildren(wiz, true /*this handler only*/);
        m_wizard = old;
        return wiz;
    }
    else
    {
        wxWizardPage *page = NULL;
        wxUnusedVar(page);

        if (m_class == wxT("wxWizardPageSimple"))
        {
            XRC_MAKE_INSTANCE(p, wxWizardPageSimple)
            p->Create(m_wizard, NULL, NULL, GetBitmap());
            if (m_lastSimplePage)
                wxWizardPageSimple::Chain(m_lastSimplePage, p);
            page = p;
            m_lastSimplePage = p;
        }
        else /*if (m_class == wxT("wxWizardPage"))*/
        {
            wxWizardPage *p = NULL;
            if (m_instance)
                p = wxStaticCast(m_instance, wxWizardPage);
            else
                wxLogError(wxT("wxWizardPage is abstract class, must be subclassed"));
            p->Create(m_wizard, GetBitmap());
            page = p;
        }

        page->SetName(GetName());
        page->SetId(GetID());

        SetupWindow(page);
        CreateChildren(page);
        return page;
    }
}

bool wxWizardXmlHandler::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxWizard")) ||
           (m_wizard != NULL &&
                (IsOfClass(node, wxT("wxWizardPage")) ||
                 IsOfClass(node, wxT("wxWizardPageSimple")))
           );
}

#endif // wxUSE_XRC && wxUSE_WIZARDDLG
