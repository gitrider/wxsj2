///////////////////////////////////////////////////////////////////////////////
// Name:        menuitem.h
// Purpose:     wxMenuItem class
// Author:      Vadim Zeitlin
// Modified by:
// Created:     11.11.97
// RCS-ID:      $Id: menuitem.h,v 1.18 2005/04/27 01:11:59 DW Exp $
// Copyright:   (c) 1998 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef   _MENUITEM_H
#define   _MENUITEM_H

#ifdef __GNUG__
#pragma interface "menuitem.h"
#endif

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include "wx/setup.h"

// an exception to the general rule that a normal header doesn't include other
// headers - only because ownerdrw.h is not always included and I don't want
// to write #ifdef's everywhere...
#if wxUSE_OWNER_DRAWN
    #include  "wx/ownerdrw.h"
#endif

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// wxMenuItem: an item in the menu, optionally implements owner-drawn behaviour
// ----------------------------------------------------------------------------
class WXDLLEXPORT wxMenuItem: public wxMenuItemBase
#if wxUSE_OWNER_DRAWN
                            , public wxOwnerDrawn
#endif
{
public:
    //
    // ctor & dtor
    //
    wxMenuItem( wxMenu*         pParentMenu = NULL
               ,int             nId = wxID_SEPARATOR
               ,const wxString& rStrName = wxEmptyString
               ,const wxString& rWxHelp = wxEmptyString
               ,wxItemKind      eKind = wxITEM_NORMAL
               ,wxMenu*         pSubMenu = NULL
              );

    //
    // Depricated, do not use in new code
    //
    wxMenuItem( wxMenu*         pParentMenu
               ,int             vId
               ,const wxString& rsText
               ,const wxString& rsHelp
               ,bool            bIsCheckable
               ,wxMenu*         pSubMenu = (wxMenu *)NULL
              );
    virtual ~wxMenuItem();

    //
    // Override base class virtuals
    //
    virtual void SetText(const wxString& rStrName);
    virtual void SetCheckable(bool bCheckable);

    virtual void Enable(bool bDoEnable = TRUE);
    virtual void Check(bool bDoCheck = TRUE);
    virtual bool IsChecked(void) const;

    //
    // Unfortunately needed to resolve ambiguity between
    // wxMenuItemBase::IsCheckable() and wxOwnerDrawn::IsCheckable()
    //
    bool IsCheckable(void) const { return wxMenuItemBase::IsCheckable(); }

    //
    // The id for a popup menu is really its menu handle (as required by
    // ::AppendMenu() API), so this function will return either the id or the
    // menu handle depending on what we're
    //
    int GetRealId(void) const;

    //
    // Mark item as belonging to the given radio group
    //
    void SetAsRadioGroupStart(void);
    void SetRadioGroupStart(int nStart);
    void SetRadioGroupEnd(int nEnd);

    //
    // All OS/2PM Submenus and menus have one of these
    //
    MENUITEM                        m_vMenuData;

private:
    void Init();

    //
    // The positions of the first and last items of the radio group this item
    // belongs to or -1: start is the radio group start and is valid for all
    // but first radio group items (m_isRadioGroupStart == FALSE), end is valid
    // only for the first one
    //
    union
    {
        int m_nStart;
        int m_nEnd;
    } m_vRadioGroup;

    //
    // Does this item start a radio group?
    //
    bool                            m_bIsRadioGroupStart;

    DECLARE_DYNAMIC_CLASS(wxMenuItem)
}; // end of CLASS wxMenuItem

#endif  //_MENUITEM_H
