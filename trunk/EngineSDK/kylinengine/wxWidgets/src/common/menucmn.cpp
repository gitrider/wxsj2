///////////////////////////////////////////////////////////////////////////////
// Name:        common/menucmn.cpp
// Purpose:     wxMenu and wxMenuBar methods common to all ports
// Author:      Vadim Zeitlin
// Modified by:
// Created:     26.10.99
// RCS-ID:      $Id: menucmn.cpp,v 1.51 2005/05/21 16:05:26 RR Exp $
// Copyright:   (c) wxWidgets team
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "menubase.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_MENUS

#include <ctype.h>

#ifndef WX_PRECOMP
    #include "wx/intl.h"
    #include "wx/log.h"
    #include "wx/menu.h"
#endif

// ----------------------------------------------------------------------------
// template lists
// ----------------------------------------------------------------------------

#include "wx/listimpl.cpp"

WX_DEFINE_LIST(wxMenuList);
WX_DEFINE_LIST(wxMenuItemList);

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxMenuItem
// ----------------------------------------------------------------------------

wxMenuItemBase::wxMenuItemBase(wxMenu *parentMenu,
                               int id,
                               const wxString& text,
                               const wxString& help,
                               wxItemKind kind,
                               wxMenu *subMenu)
              : m_text(text),
                m_help(help)
{
    wxASSERT_MSG( parentMenu != NULL, wxT("menuitem should have a menu") );

    m_parentMenu  = parentMenu;
    m_subMenu     = subMenu;
    m_isEnabled   = true;
    m_isChecked   = false;
    m_id          = id;
    m_kind        = kind;
    if (m_id == wxID_ANY)
        m_id = wxNewId();
    if (m_id == wxID_SEPARATOR)
        m_kind = wxITEM_SEPARATOR;
}

wxMenuItemBase::~wxMenuItemBase()
{
    delete m_subMenu;
}

#if wxUSE_ACCEL

static inline bool CompareAccelString(const wxString& str, const wxChar *accel)
{
#if wxUSE_INTL
    return str == accel || str == wxGetTranslation(accel);
#else
    return str == accel;
#endif
}

// return wxAcceleratorEntry for the given menu string or NULL if none
// specified
wxAcceleratorEntry *wxGetAccelFromString(const wxString& label)
{
    // wxPrintf( wxT("label %s\n"), label.c_str() );

    // check for accelerators: they are given after '\t'
    int posTab = label.Find(wxT('\t'));
    if ( posTab != wxNOT_FOUND ) {
        // parse the accelerator string
        int keyCode = 0;
        int accelFlags = wxACCEL_NORMAL;
        wxString current;
        for ( size_t n = (size_t)posTab + 1; n < label.Len(); n++ ) {
            if ( (label[n] == '+') || (label[n] == '-') ) {
                if ( CompareAccelString(current, wxTRANSLATE("ctrl")) )
                    accelFlags |= wxACCEL_CTRL;
                else if ( CompareAccelString(current, wxTRANSLATE("alt")) )
                    accelFlags |= wxACCEL_ALT;
                else if ( CompareAccelString(current, wxTRANSLATE("shift")) )
                    accelFlags |= wxACCEL_SHIFT;
                else {
                    // we may have "Ctrl-+", for example, but we still want to
                    // catch typos like "Crtl-A" so only give the warning if we
                    // have something before the current '+' or '-', else take
                    // it as a literal symbol
                    if ( current.empty() )
                    {
                        current += label[n];

                        // skip clearing it below
                        continue;
                    }
                    else
                    {
                        wxLogDebug(wxT("Unknown accel modifier: '%s'"),
                                   current.c_str());
                    }
                }

                current.clear();
            }
            else {
                current += (wxChar) wxTolower(label[n]);
            }
        }

        if ( current.empty() ) {
            wxLogDebug(wxT("No accel key found, accel string ignored."));
        }
        else {
            if ( current.Len() == 1 ) {
                // it's a letter
                keyCode = current[0U];

                // Only call wxToupper if control, alt, or shift is held down,
                // otherwise lower case accelerators won't work.
                if (accelFlags != wxACCEL_NORMAL) {
                    keyCode = wxToupper(keyCode);
                }
            }
            else {
                // is it a function key?
                if ( current[0U] == 'f' && wxIsdigit(current[1U]) &&
                     (current.Len() == 2 ||
                     (current.Len() == 3 && wxIsdigit(current[2U]))) ) {
                    keyCode = WXK_F1 + wxAtoi(current.c_str() + 1) - 1;
                }
                else {
                    // several special cases
                    current.MakeUpper();
                    if ( current == wxT("DEL") )
                        keyCode = WXK_DELETE;
                    else if ( current == wxT("DELETE") )
                        keyCode = WXK_DELETE;
                    else if ( current == wxT("BACK") )
                        keyCode = WXK_BACK;
                    else if ( current == wxT("INS") )
                        keyCode = WXK_INSERT;
                    else if ( current == wxT("INSERT") )
                        keyCode = WXK_INSERT;
                    else if ( current == wxT("ENTER") || current == wxT("RETURN") )
                        keyCode = WXK_RETURN;
                    else if ( current == wxT("PGUP") )
                        keyCode = WXK_PRIOR;
                    else if ( current == wxT("PGDN") )
                        keyCode = WXK_NEXT;
                    else if ( current == wxT("LEFT") )
                        keyCode = WXK_LEFT;
                    else if ( current == wxT("RIGHT") )
                        keyCode = WXK_RIGHT;
                    else if ( current == wxT("UP") )
                        keyCode = WXK_UP;
                    else if ( current == wxT("DOWN") )
                        keyCode = WXK_DOWN;
                    else if ( current == wxT("HOME") )
                        keyCode = WXK_HOME;
                    else if ( current == wxT("END") )
                        keyCode = WXK_END;
                    else if ( current == wxT("SPACE") )
                        keyCode = WXK_SPACE;
                    else if ( current == wxT("TAB") )
                        keyCode = WXK_TAB;
                    else if ( current == wxT("ESC") || current == wxT("ESCAPE") )
                        keyCode = WXK_ESCAPE;
                    else if ( current == wxT("CANCEL") )
                        keyCode = WXK_CANCEL;
                    else if ( current == wxT("CLEAR") )
                        keyCode = WXK_CLEAR;
                    else if ( current == wxT("MENU") )
                        keyCode = WXK_MENU;
                    else if ( current == wxT("PAUSE") )
                        keyCode = WXK_PAUSE;
                    else if ( current == wxT("CAPITAL") )
                        keyCode = WXK_CAPITAL;
                    else if ( current == wxT("SELECT") )
                        keyCode = WXK_SELECT;
                    else if ( current == wxT("PRINT") )
                        keyCode = WXK_PRINT;
                    else if ( current == wxT("EXECUTE") )
                        keyCode = WXK_EXECUTE;
                    else if ( current == wxT("SNAPSHOT") )
                        keyCode = WXK_SNAPSHOT;
                    else if ( current == wxT("HELP") )
                        keyCode = WXK_HELP;
                    else if ( current == wxT("HELP") )
                        keyCode = WXK_HELP;
                    else if ( current == wxT("ADD") )
                        keyCode = WXK_ADD;
                    else if ( current == wxT("SEPARATOR") )
                        keyCode = WXK_SEPARATOR;
                    else if ( current == wxT("SUBTRACT") )
                        keyCode = WXK_SUBTRACT;
                    else if ( current == wxT("DECIMAL") )
                        keyCode = WXK_DECIMAL;
                    else if ( current == wxT("DIVIDE") )
                        keyCode = WXK_DIVIDE;
                    else if ( current == wxT("NUM_LOCK") )
                        keyCode = WXK_NUMLOCK;
                    else if ( current == wxT("SCROLL_LOCK") )
                        keyCode = WXK_SCROLL;
                    else if ( current == wxT("PAGEUP") )
                        keyCode = WXK_PAGEUP;
                    else if ( current == wxT("PAGEDOWN") )
                        keyCode = WXK_PAGEDOWN;
                    else if ( current == wxT("KP_SPACE") )
                        keyCode = WXK_NUMPAD_SPACE;
                    else if ( current == wxT("KP_TAB") )
                        keyCode = WXK_NUMPAD_TAB;
                    else if ( current == wxT("KP_ENTER") )
                        keyCode = WXK_NUMPAD_ENTER;
                    else if ( current == wxT("KP_HOME") )
                        keyCode = WXK_NUMPAD_HOME;
                    else if ( current == wxT("KP_LEFT") )
                        keyCode = WXK_NUMPAD_LEFT;
                    else if ( current == wxT("KP_UP") )
                        keyCode = WXK_NUMPAD_UP;
                    else if ( current == wxT("KP_RIGHT") )
                        keyCode = WXK_NUMPAD_RIGHT;
                    else if ( current == wxT("KP_DOWN") )
                        keyCode = WXK_NUMPAD_DOWN;
                    else if ( current == wxT("KP_PRIOR") )
                        keyCode = WXK_NUMPAD_PRIOR;
                    else if ( current == wxT("KP_PAGEUP") )
                        keyCode = WXK_NUMPAD_PAGEUP;
                    else if ( current == wxT("KP_NEXT;") )
                        keyCode = WXK_NUMPAD_NEXT;
                    else if ( current == wxT("KP_PAGEDOWN") )
                        keyCode = WXK_NUMPAD_PAGEDOWN;
                    else if ( current == wxT("KP_END") )
                        keyCode = WXK_NUMPAD_END;
                    else if ( current == wxT("KP_BEGIN") )
                        keyCode = WXK_NUMPAD_BEGIN;
                    else if ( current == wxT("KP_INSERT") )
                        keyCode = WXK_NUMPAD_INSERT;
                    else if ( current == wxT("KP_DELETE") )
                        keyCode = WXK_NUMPAD_DELETE;
                    else if ( current == wxT("KP_EQUAL") )
                        keyCode = WXK_NUMPAD_EQUAL;
                    else if ( current == wxT("KP_MULTIPLY") )
                        keyCode = WXK_NUMPAD_MULTIPLY;
                    else if ( current == wxT("KP_ADD") )
                        keyCode = WXK_NUMPAD_ADD;
                    else if ( current == wxT("KP_SEPARATOR") )
                        keyCode = WXK_NUMPAD_SEPARATOR;
                    else if ( current == wxT("KP_SUBTRACT") )
                        keyCode = WXK_NUMPAD_SUBTRACT;
                    else if ( current == wxT("KP_DECIMAL") )
                        keyCode = WXK_NUMPAD_DECIMAL;
                    else if ( current == wxT("KP_DIVIDE") )
                        keyCode = WXK_NUMPAD_DIVIDE;
                    else if ( current == wxT("WINDOWS_LEFT") )
                        keyCode = WXK_WINDOWS_LEFT;
                    else if ( current == wxT("WINDOWS_RIGHT") )
                        keyCode = WXK_WINDOWS_RIGHT;
                     else if ( current == wxT("WINDOWS_MENU") )
                        keyCode = WXK_WINDOWS_MENU;
                    else if ( current == wxT("COMMAND") )
                        keyCode = WXK_COMMAND;
                    else if ( current.Left(3) == wxT("KP_") && wxIsdigit(current[3U]) ) {
                    keyCode = WXK_NUMPAD0 + wxAtoi(current.c_str() + 3);  }
                    else if ( current.Left(7) == wxT("SPECIAL") && wxIsdigit(current[7U]) ) {
                    keyCode = WXK_SPECIAL1 + wxAtoi(current.c_str() + 7) - 1;  }
                    else
                    {
                        wxLogDebug(wxT("Unrecognized accel key '%s', accel string ignored."),
                                   current.c_str());
                        return NULL;
                    }
                }
            }
        }

        if ( keyCode ) {
            // we do have something
            return new wxAcceleratorEntry(accelFlags, keyCode);
        }
    }

    return (wxAcceleratorEntry *)NULL;
}

wxAcceleratorEntry *wxMenuItemBase::GetAccel() const
{
    return wxGetAccelFromString(GetText());
}

void wxMenuItemBase::SetAccel(wxAcceleratorEntry *accel)
{
    wxString text = m_text.BeforeFirst(wxT('\t'));
    if ( accel )
    {
        text += wxT('\t');

        int flags = accel->GetFlags();
        if ( flags & wxACCEL_ALT )
            text += wxT("Alt-");
        if ( flags & wxACCEL_CTRL )
            text += wxT("Ctrl-");
        if ( flags & wxACCEL_SHIFT )
            text += wxT("Shift-");

        int code = accel->GetKeyCode();
        switch ( code )
        {
            case WXK_F1:
            case WXK_F2:
            case WXK_F3:
            case WXK_F4:
            case WXK_F5:
            case WXK_F6:
            case WXK_F7:
            case WXK_F8:
            case WXK_F9:
            case WXK_F10:
            case WXK_F11:
            case WXK_F12:
                text << wxT('F') << code - WXK_F1 + 1;
                break;

            // if there are any other keys wxGetAccelFromString() may return,
            // we should process them here

            default:
                if ( wxIsalnum(code) )
                {
                    text << (wxChar)code;

                    break;
                }

                wxFAIL_MSG( wxT("unknown keyboard accel") );
        }
    }

    SetText(text);
}

#endif // wxUSE_ACCEL

bool wxMenuBase::ms_locked = true;

// ----------------------------------------------------------------------------
// wxMenu ctor and dtor
// ----------------------------------------------------------------------------

void wxMenuBase::Init(long style)
{
    m_menuBar = (wxMenuBar *)NULL;
    m_menuParent = (wxMenu *)NULL;

    m_invokingWindow = (wxWindow *)NULL;
    m_style = style;
    m_clientData = (void *)NULL;
    m_eventHandler = this;
}

wxMenuBase::~wxMenuBase()
{
    WX_CLEAR_LIST(wxMenuItemList, m_items);

    // Actually, in GTK, the submenus have to get deleted first.
}

// ----------------------------------------------------------------------------
// wxMenu item adding/removing
// ----------------------------------------------------------------------------

void wxMenuBase::AddSubMenu(wxMenu *submenu)
{
    wxCHECK_RET( submenu, _T("can't add a NULL submenu") );

    submenu->SetParent((wxMenu *)this);
}

wxMenuItem* wxMenuBase::DoAppend(wxMenuItem *item)
{
    wxCHECK_MSG( item, NULL, wxT("invalid item in wxMenu::Append()") );

    m_items.Append(item);
    item->SetMenu((wxMenu*)this);
    if ( item->IsSubMenu() )
    {
        AddSubMenu(item->GetSubMenu());
    }

    return item;
}

wxMenuItem* wxMenuBase::Insert(size_t pos, wxMenuItem *item)
{
    wxCHECK_MSG( item, NULL, wxT("invalid item in wxMenu::Insert") );

    if ( pos == GetMenuItemCount() )
    {
        return DoAppend(item);
    }
    else
    {
        wxCHECK_MSG( pos < GetMenuItemCount(), NULL,
                     wxT("invalid index in wxMenu::Insert") );

        return DoInsert(pos, item);
    }
}

wxMenuItem* wxMenuBase::DoInsert(size_t pos, wxMenuItem *item)
{
    wxCHECK_MSG( item, NULL, wxT("invalid item in wxMenu::Insert()") );

    wxMenuItemList::compatibility_iterator node = m_items.Item(pos);
    wxCHECK_MSG( node, NULL, wxT("invalid index in wxMenu::Insert()") );

    m_items.Insert(node, item);
    item->SetMenu((wxMenu*)this);
    if ( item->IsSubMenu() )
    {
        AddSubMenu(item->GetSubMenu());
    }

    return item;
}

wxMenuItem *wxMenuBase::Remove(wxMenuItem *item)
{
    wxCHECK_MSG( item, NULL, wxT("invalid item in wxMenu::Remove") );

    return DoRemove(item);
}

wxMenuItem *wxMenuBase::DoRemove(wxMenuItem *item)
{
    wxMenuItemList::compatibility_iterator node = m_items.Find(item);

    // if we get here, the item is valid or one of Remove() functions is broken
    wxCHECK_MSG( node, NULL, wxT("bug in wxMenu::Remove logic") );

    // we detach the item, but we do delete the list node (i.e. don't call
    // DetachNode() here!)
    m_items.Erase(node);

    // item isn't attached to anything any more
    item->SetMenu((wxMenu *)NULL);
    wxMenu *submenu = item->GetSubMenu();
    if ( submenu )
    {
        submenu->SetParent((wxMenu *)NULL);
        if ( submenu->IsAttached() )
            submenu->Detach();
    }

    return item;
}

bool wxMenuBase::Delete(wxMenuItem *item)
{
    wxCHECK_MSG( item, false, wxT("invalid item in wxMenu::Delete") );

    return DoDelete(item);
}

bool wxMenuBase::DoDelete(wxMenuItem *item)
{
    wxMenuItem *item2 = DoRemove(item);
    wxCHECK_MSG( item2, false, wxT("failed to delete menu item") );

    // don't delete the submenu
    item2->SetSubMenu((wxMenu *)NULL);

    delete item2;

    return true;
}

bool wxMenuBase::Destroy(wxMenuItem *item)
{
    wxCHECK_MSG( item, false, wxT("invalid item in wxMenu::Destroy") );

    return DoDestroy(item);
}

bool wxMenuBase::DoDestroy(wxMenuItem *item)
{
    wxMenuItem *item2 = DoRemove(item);
    wxCHECK_MSG( item2, false, wxT("failed to delete menu item") );

    delete item2;

    return true;
}

// ----------------------------------------------------------------------------
// wxMenu searching for items
// ----------------------------------------------------------------------------

// Finds the item id matching the given string, wxNOT_FOUND if not found.
int wxMenuBase::FindItem(const wxString& text) const
{
    wxString label = wxMenuItem::GetLabelFromText(text);
    for ( wxMenuItemList::compatibility_iterator node = m_items.GetFirst();
          node;
          node = node->GetNext() )
    {
        wxMenuItem *item = node->GetData();
        if ( item->IsSubMenu() )
        {
            int rc = item->GetSubMenu()->FindItem(label);
            if ( rc != wxNOT_FOUND )
                return rc;
        }

        // we execute this code for submenus as well to alllow finding them by
        // name just like the ordinary items
        if ( !item->IsSeparator() )
        {
            if ( item->GetLabel() == label )
                return item->GetId();
        }
    }

    return wxNOT_FOUND;
}

// recursive search for item by id
wxMenuItem *wxMenuBase::FindItem(int itemId, wxMenu **itemMenu) const
{
    if ( itemMenu )
        *itemMenu = NULL;

    wxMenuItem *item = NULL;
    for ( wxMenuItemList::compatibility_iterator node = m_items.GetFirst();
          node && !item;
          node = node->GetNext() )
    {
        item = node->GetData();

        if ( item->GetId() == itemId )
        {
            if ( itemMenu )
                *itemMenu = (wxMenu *)this;
        }
        else if ( item->IsSubMenu() )
        {
            item = item->GetSubMenu()->FindItem(itemId, itemMenu);
        }
        else
        {
            // don't exit the loop
            item = NULL;
        }
    }

    return item;
}

// non recursive search
wxMenuItem *wxMenuBase::FindChildItem(int id, size_t *ppos) const
{
    wxMenuItem *item = (wxMenuItem *)NULL;
    wxMenuItemList::compatibility_iterator node = GetMenuItems().GetFirst();

    size_t pos;
    for ( pos = 0; node; pos++ )
    {
        if ( node->GetData()->GetId() == id )
        {
            item = node->GetData();

            break;
        }

        node = node->GetNext();
    }

    if ( ppos )
    {
        *ppos = item ? pos : (size_t)wxNOT_FOUND;
    }

    return item;
}

// find by position
wxMenuItem* wxMenuBase::FindItemByPosition(size_t position) const
{
    wxCHECK_MSG( position < m_items.GetCount(), NULL,
                 _T("wxMenu::FindItemByPosition(): invalid menu index") );

    return m_items.Item( position )->GetData();
}

// ----------------------------------------------------------------------------
// wxMenu helpers used by derived classes
// ----------------------------------------------------------------------------

// Update a menu and all submenus recursively. source is the object that has
// the update event handlers defined for it. If NULL, the menu or associated
// window will be used.
void wxMenuBase::UpdateUI(wxEvtHandler* source)
{
    if (GetInvokingWindow())
    {
        // Don't update menus if the parent
        // frame is about to get deleted
        wxWindow *tlw = wxGetTopLevelParent( GetInvokingWindow() );
        if (tlw && wxPendingDelete.Member(tlw))
            return;
    }

    if ( !source && GetInvokingWindow() )
        source = GetInvokingWindow()->GetEventHandler();
    if ( !source )
        source = GetEventHandler();
    if ( !source )
        source = this;

    wxMenuItemList::compatibility_iterator  node = GetMenuItems().GetFirst();
    while ( node )
    {
        wxMenuItem* item = node->GetData();
        if ( !item->IsSeparator() )
        {
            wxWindowID id = item->GetId();
            wxUpdateUIEvent event(id);
            event.SetEventObject( source );

            if ( source->ProcessEvent(event) )
            {
                // if anything changed, update the changed attribute
                if (event.GetSetText())
                    SetLabel(id, event.GetText());
                if (event.GetSetChecked())
                    Check(id, event.GetChecked());
                if (event.GetSetEnabled())
                    Enable(id, event.GetEnabled());
            }

            // recurse to the submenus
            if ( item->GetSubMenu() )
                item->GetSubMenu()->UpdateUI(source);
        }
        //else: item is a separator (which doesn't process update UI events)

        node = node->GetNext();
    }
}

bool wxMenuBase::SendEvent(int id, int checked)
{
    wxCommandEvent event(wxEVT_COMMAND_MENU_SELECTED, id);
    event.SetEventObject(this);
    event.SetInt(checked);

    bool processed = false;

    // Try the menu's event handler
    if ( !processed )
    {
        wxEvtHandler *handler = GetEventHandler();
        if ( handler )
            processed = handler->ProcessEvent(event);
    }

    // Try the window the menu was popped up from (and up through the
    // hierarchy)
    if ( !processed )
    {
        const wxMenuBase *menu = this;
        while ( menu )
        {
            wxWindow *win = menu->GetInvokingWindow();
            if ( win )
            {
                processed = win->GetEventHandler()->ProcessEvent(event);
                break;
            }

            menu = menu->GetParent();
        }
    }

    return processed;
}

// ----------------------------------------------------------------------------
// wxMenu attaching/detaching to/from menu bar
// ----------------------------------------------------------------------------

wxMenuBar* wxMenuBase::GetMenuBar() const
{
    if(GetParent())
        return GetParent()->GetMenuBar();
    return m_menuBar;
}

void wxMenuBase::Attach(wxMenuBarBase *menubar)
{
    // use Detach() instead!
    wxASSERT_MSG( menubar, _T("menu can't be attached to NULL menubar") );

    // use IsAttached() to prevent this from happening
    wxASSERT_MSG( !m_menuBar, _T("attaching menu twice?") );

    m_menuBar = (wxMenuBar *)menubar;
}

void wxMenuBase::Detach()
{
    // use IsAttached() to prevent this from happening
    wxASSERT_MSG( m_menuBar, _T("detaching unattached menu?") );

    m_menuBar = NULL;
}

// ----------------------------------------------------------------------------
// wxMenu functions forwarded to wxMenuItem
// ----------------------------------------------------------------------------

void wxMenuBase::Enable( int id, bool enable )
{
    wxMenuItem *item = FindItem(id);

    wxCHECK_RET( item, wxT("wxMenu::Enable: no such item") );

    item->Enable(enable);
}

bool wxMenuBase::IsEnabled( int id ) const
{
    wxMenuItem *item = FindItem(id);

    wxCHECK_MSG( item, false, wxT("wxMenu::IsEnabled: no such item") );

    return item->IsEnabled();
}

void wxMenuBase::Check( int id, bool enable )
{
    wxMenuItem *item = FindItem(id);

    wxCHECK_RET( item, wxT("wxMenu::Check: no such item") );

    item->Check(enable);
}

bool wxMenuBase::IsChecked( int id ) const
{
    wxMenuItem *item = FindItem(id);

    wxCHECK_MSG( item, false, wxT("wxMenu::IsChecked: no such item") );

    return item->IsChecked();
}

void wxMenuBase::SetLabel( int id, const wxString &label )
{
    wxMenuItem *item = FindItem(id);

    wxCHECK_RET( item, wxT("wxMenu::SetLabel: no such item") );

    item->SetText(label);
}

wxString wxMenuBase::GetLabel( int id ) const
{
    wxMenuItem *item = FindItem(id);

    wxCHECK_MSG( item, wxEmptyString, wxT("wxMenu::GetLabel: no such item") );

    return item->GetText();
}

void wxMenuBase::SetHelpString( int id, const wxString& helpString )
{
    wxMenuItem *item = FindItem(id);

    wxCHECK_RET( item, wxT("wxMenu::SetHelpString: no such item") );

    item->SetHelp( helpString );
}

wxString wxMenuBase::GetHelpString( int id ) const
{
    wxMenuItem *item = FindItem(id);

    wxCHECK_MSG( item, wxEmptyString, wxT("wxMenu::GetHelpString: no such item") );

    return item->GetHelp();
}

// ----------------------------------------------------------------------------
// wxMenuBarBase ctor and dtor
// ----------------------------------------------------------------------------

wxMenuBarBase::wxMenuBarBase()
{
    // not attached yet
    m_menuBarFrame = NULL;
}

wxMenuBarBase::~wxMenuBarBase()
{
    WX_CLEAR_LIST(wxMenuList, m_menus);
}

// ----------------------------------------------------------------------------
// wxMenuBar item access: the base class versions manage m_menus list, the
// derived class should reflect the changes in the real menubar
// ----------------------------------------------------------------------------

wxMenu *wxMenuBarBase::GetMenu(size_t pos) const
{
    wxMenuList::compatibility_iterator node = m_menus.Item(pos);
    wxCHECK_MSG( node, NULL, wxT("bad index in wxMenuBar::GetMenu()") );

    return node->GetData();
}

bool wxMenuBarBase::Append(wxMenu *menu, const wxString& WXUNUSED(title))
{
    wxCHECK_MSG( menu, false, wxT("can't append NULL menu") );

    m_menus.Append(menu);
    menu->Attach(this);

    return true;
}

bool wxMenuBarBase::Insert(size_t pos, wxMenu *menu,
                           const wxString& title)
{
    if ( pos == m_menus.GetCount() )
    {
        return wxMenuBarBase::Append(menu, title);
    }
    else // not at the end
    {
        wxCHECK_MSG( menu, false, wxT("can't insert NULL menu") );

        wxMenuList::compatibility_iterator node = m_menus.Item(pos);
        wxCHECK_MSG( node, false, wxT("bad index in wxMenuBar::Insert()") );

        m_menus.Insert(node, menu);
        menu->Attach(this);

        return true;
    }
}

wxMenu *wxMenuBarBase::Replace(size_t pos, wxMenu *menu,
                               const wxString& WXUNUSED(title))
{
    wxCHECK_MSG( menu, NULL, wxT("can't insert NULL menu") );

    wxMenuList::compatibility_iterator node = m_menus.Item(pos);
    wxCHECK_MSG( node, NULL, wxT("bad index in wxMenuBar::Replace()") );

    wxMenu *menuOld = node->GetData();
    node->SetData(menu);

    menu->Attach(this);
    menuOld->Detach();

    return menuOld;
}

wxMenu *wxMenuBarBase::Remove(size_t pos)
{
    wxMenuList::compatibility_iterator node = m_menus.Item(pos);
    wxCHECK_MSG( node, NULL, wxT("bad index in wxMenuBar::Remove()") );

    wxMenu *menu = node->GetData();
    m_menus.Erase(node);
    menu->Detach();

    return menu;
}

int wxMenuBarBase::FindMenu(const wxString& title) const
{
    wxString label = wxMenuItem::GetLabelFromText(title);

    size_t count = GetMenuCount();
    for ( size_t i = 0; i < count; i++ )
    {
        wxString title2 = GetLabelTop(i);
        if ( (title2 == title) ||
             (wxMenuItem::GetLabelFromText(title2) == label) )
        {
            // found
            return (int)i;
        }
    }

    return wxNOT_FOUND;

}

// ----------------------------------------------------------------------------
// wxMenuBar attaching/detaching to/from the frame
// ----------------------------------------------------------------------------

void wxMenuBarBase::Attach(wxFrame *frame)
{
    wxASSERT_MSG( !IsAttached(), wxT("menubar already attached!") );

    m_menuBarFrame = frame;
}

void wxMenuBarBase::Detach()
{
    wxASSERT_MSG( IsAttached(), wxT("detaching unattached menubar") );

    m_menuBarFrame = NULL;
}

// ----------------------------------------------------------------------------
// wxMenuBar searching for items
// ----------------------------------------------------------------------------

wxMenuItem *wxMenuBarBase::FindItem(int id, wxMenu **menu) const
{
    if ( menu )
        *menu = NULL;

    wxMenuItem *item = NULL;
    size_t count = GetMenuCount(), i;
    wxMenuList::const_iterator it;
    for ( i = 0, it = m_menus.begin(); !item && (i < count); i++, it++ )
    {
        item = (*it)->FindItem(id, menu);
    }

    return item;
}

int wxMenuBarBase::FindMenuItem(const wxString& menu, const wxString& item) const
{
    wxString label = wxMenuItem::GetLabelFromText(menu);

    int i = 0;
    wxMenuList::compatibility_iterator node;
    for ( node = m_menus.GetFirst(); node; node = node->GetNext(), i++ )
    {
        if ( label == wxMenuItem::GetLabelFromText(GetLabelTop(i)) )
            return node->GetData()->FindItem(item);
    }

    return wxNOT_FOUND;
}

// ---------------------------------------------------------------------------
// wxMenuBar functions forwarded to wxMenuItem
// ---------------------------------------------------------------------------

void wxMenuBarBase::Enable(int id, bool enable)
{
    wxMenuItem *item = FindItem(id);

    wxCHECK_RET( item, wxT("attempt to enable an item which doesn't exist") );

    item->Enable(enable);
}

void wxMenuBarBase::Check(int id, bool check)
{
    wxMenuItem *item = FindItem(id);

    wxCHECK_RET( item, wxT("attempt to check an item which doesn't exist") );
    wxCHECK_RET( item->IsCheckable(), wxT("attempt to check an uncheckable item") );

    item->Check(check);
}

bool wxMenuBarBase::IsChecked(int id) const
{
    wxMenuItem *item = FindItem(id);

    wxCHECK_MSG( item, false, wxT("wxMenuBar::IsChecked(): no such item") );

    return item->IsChecked();
}

bool wxMenuBarBase::IsEnabled(int id) const
{
    wxMenuItem *item = FindItem(id);

    wxCHECK_MSG( item, false, wxT("wxMenuBar::IsEnabled(): no such item") );

    return item->IsEnabled();
}

void wxMenuBarBase::SetLabel(int id, const wxString& label)
{
    wxMenuItem *item = FindItem(id);

    wxCHECK_RET( item, wxT("wxMenuBar::SetLabel(): no such item") );

    item->SetText(label);
}

wxString wxMenuBarBase::GetLabel(int id) const
{
    wxMenuItem *item = FindItem(id);

    wxCHECK_MSG( item, wxEmptyString,
                 wxT("wxMenuBar::GetLabel(): no such item") );

    return item->GetText();
}

void wxMenuBarBase::SetHelpString(int id, const wxString& helpString)
{
    wxMenuItem *item = FindItem(id);

    wxCHECK_RET( item, wxT("wxMenuBar::SetHelpString(): no such item") );

    item->SetHelp(helpString);
}

wxString wxMenuBarBase::GetHelpString(int id) const
{
    wxMenuItem *item = FindItem(id);

    wxCHECK_MSG( item, wxEmptyString,
                 wxT("wxMenuBar::GetHelpString(): no such item") );

    return item->GetHelp();
}

#endif // wxUSE_MENUS
