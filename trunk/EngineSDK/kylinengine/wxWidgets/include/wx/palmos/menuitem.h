///////////////////////////////////////////////////////////////////////////////
// Name:        wx/palmos/menuitem.h
// Purpose:     wxMenuItem class
// Author:      William Osborne - minimal working wxPalmOS port
// Modified by:
// Created:     10/13/04
// RCS-ID:      $Id: menuitem.h,v 1.3 2005/03/10 19:58:58 ABX Exp $
// Copyright:   (c) William Osborne
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef   _MENUITEM_H
#define   _MENUITEM_H

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "menuitem.h"
#endif

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if wxUSE_OWNER_DRAWN
    #include  "wx/ownerdrw.h"   // base class
#endif

// ----------------------------------------------------------------------------
// wxMenuItem: an item in the menu, optionally implements owner-drawn behaviour
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxMenuItem : public wxMenuItemBase
#if wxUSE_OWNER_DRAWN
                             , public wxOwnerDrawn
#endif
{
public:
    // ctor & dtor
    wxMenuItem(wxMenu *parentMenu = (wxMenu *)NULL,
               int id = wxID_SEPARATOR,
               const wxString& name = wxEmptyString,
               const wxString& help = wxEmptyString,
               wxItemKind kind = wxITEM_NORMAL,
               wxMenu *subMenu = (wxMenu *)NULL);
    virtual ~wxMenuItem();

    // override base class virtuals
    virtual void SetText(const wxString& strName);
    virtual void SetCheckable(bool checkable);

    virtual void Enable(bool bDoEnable = TRUE);
    virtual void Check(bool bDoCheck = TRUE);
    virtual bool IsChecked() const;

    // unfortunately needed to resolve ambiguity between
    // wxMenuItemBase::IsCheckable() and wxOwnerDrawn::IsCheckable()
    bool IsCheckable() const { return wxMenuItemBase::IsCheckable(); }

    // mark item as belonging to the given radio group
    void SetAsRadioGroupStart();
    void SetRadioGroupStart(int start);
    void SetRadioGroupEnd(int end);

    // compatibility only, don't use in new code
    wxMenuItem(wxMenu *parentMenu,
               int id,
               const wxString& text,
               const wxString& help,
               bool isCheckable,
               wxMenu *subMenu = (wxMenu *)NULL);

private:
    // common part of all ctors
    void Init();

    // the positions of the first and last items of the radio group this item
    // belongs to or -1: start is the radio group start and is valid for all
    // but first radio group items (m_isRadioGroupStart == FALSE), end is valid
    // only for the first one
    union
    {
        int start;
        int end;
    } m_radioGroup;

    // does this item start a radio group?
    bool m_isRadioGroupStart;

    DECLARE_DYNAMIC_CLASS_NO_COPY(wxMenuItem)
};

#endif  //_MENUITEM_H
