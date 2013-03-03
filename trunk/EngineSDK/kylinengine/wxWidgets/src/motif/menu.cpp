/////////////////////////////////////////////////////////////////////////////
// Name:        menu.cpp
// Purpose:     wxMenu, wxMenuBar, wxMenuItem
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id: menu.cpp,v 1.50 2005/05/31 11:58:04 ABX Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////


// ============================================================================
// declarations
// ============================================================================

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "menu.h"
#endif

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/defs.h"

#include "wx/menu.h"
#include "wx/menuitem.h"
#include "wx/log.h"
#include "wx/utils.h"
#include "wx/app.h"
#include "wx/frame.h"
#include "wx/settings.h"

#ifdef __VMS__
#pragma message disable nosimpint
#define XtDisplay XTDISPLAY
#define XtWindow XTWINDOW
#endif
#include <Xm/Label.h>
#include <Xm/LabelG.h>
#include <Xm/CascadeBG.h>
#include <Xm/CascadeB.h>
#include <Xm/SeparatoG.h>
#include <Xm/PushBG.h>
#include <Xm/ToggleB.h>
#include <Xm/ToggleBG.h>
#include <Xm/RowColumn.h>
#ifdef __VMS__
#pragma message enable nosimpint
#endif

#include "wx/motif/private.h"

// other standard headers
#include <string.h>

IMPLEMENT_DYNAMIC_CLASS(wxMenu, wxEvtHandler)
IMPLEMENT_DYNAMIC_CLASS(wxMenuBar, wxEvtHandler)

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// Menus
// ----------------------------------------------------------------------------

// Construct a menu with optional title (then use append)
void wxMenu::Init()
{
    // Motif-specific members
    m_numColumns = 1;
    m_menuWidget = (WXWidget) NULL;
    m_popupShell = (WXWidget) NULL;
    m_buttonWidget = (WXWidget) NULL;
    m_menuId = 0;
    m_topLevelMenu  = (wxMenu*) NULL;
    m_ownedByMenuBar = false;

    if ( !m_title.empty() )
    {
        Append(-3, m_title) ;
        AppendSeparator() ;
    }

    m_backgroundColour = wxSystemSettings::GetColour(wxSYS_COLOUR_MENU);
    m_foregroundColour = wxSystemSettings::GetColour(wxSYS_COLOUR_MENUTEXT);
    m_font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
}

// The wxWindow destructor will take care of deleting the submenus.
wxMenu::~wxMenu()
{
    if (m_menuWidget)
    {
        if (m_menuParent)
            DestroyMenu(true);
        else
            DestroyMenu(false);
    }

    // Not sure if this is right
    if (m_menuParent && m_menuBar)
    {
        m_menuParent = NULL;
        //      m_menuBar = NULL;
    }
}

void wxMenu::Break()
{
    m_numColumns++;
}

// function appends a new item or submenu to the menu
wxMenuItem* wxMenu::DoAppend(wxMenuItem *pItem)
{
    if (m_menuWidget)
    {
        // this is a dynamic Append
        pItem->CreateItem(m_menuWidget, GetMenuBar(), m_topLevelMenu);
    }

    if ( pItem->IsSubMenu() )
    {
        pItem->GetSubMenu()->m_topLevelMenu = m_topLevelMenu;
    }

    return wxMenuBase::DoAppend(pItem);
}

wxMenuItem *wxMenu::DoRemove(wxMenuItem *item)
{
    item->DestroyItem(true);

    return wxMenuBase::DoRemove(item);
}

wxMenuItem* wxMenu::DoInsert(size_t pos, wxMenuItem *item)
{
    if ( wxMenuBase::DoInsert(pos, item) )
        return item;

    wxFAIL_MSG(wxT("DoInsert not implemented; or error in wxMenuBase::DoInsert"));

    return NULL;
}

void wxMenu::SetTitle(const wxString& label)
{
    m_title = label;

    wxMenuItemList::compatibility_iterator node = GetMenuItems().GetFirst();
    if ( !node )
        return;

    wxMenuItem *item = node->GetData ();
    Widget widget = (Widget) item->GetButtonWidget();
    if ( !widget )
        return;

    wxXmString title_str(label);
    XtVaSetValues(widget,
                  XmNlabelString, title_str(),
                  NULL);
}

bool wxMenu::ProcessCommand(wxCommandEvent & event)
{
    bool processed = false;

#if wxUSE_MENU_CALLBACK
    // Try a callback
    if (m_callback)
    {
        (void) (*(m_callback)) (*this, event);
        processed = true;
    }
#endif // wxUSE_MENU_CALLBACK

    // Try the menu's event handler
    if ( !processed && GetEventHandler())
    {
        processed = GetEventHandler()->ProcessEvent(event);
    }
    // Try the window the menu was popped up from (and up
    // through the hierarchy)
    if ( !processed && GetInvokingWindow())
        processed = GetInvokingWindow()->ProcessEvent(event);

    return processed;
}

// ----------------------------------------------------------------------------
// Menu Bar
// ----------------------------------------------------------------------------

void wxMenuBar::Init()
{
    m_eventHandler = this;
    m_menuBarFrame = NULL;
    m_mainWidget = (WXWidget) NULL;
    m_backgroundColour = wxSystemSettings::GetColour(wxSYS_COLOUR_MENU);
    m_foregroundColour = wxSystemSettings::GetColour(wxSYS_COLOUR_MENUTEXT);
    m_font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
}

wxMenuBar::wxMenuBar(size_t n, wxMenu *menus[], const wxArrayString& titles, long WXUNUSED(style))
{
    wxASSERT( n == titles.GetCount() );

    Init();

    m_titles = titles;
    for ( size_t i = 0; i < n; i++ )
        m_menus.Append(menus[i]);
}

wxMenuBar::wxMenuBar(size_t n, wxMenu *menus[], const wxString titles[], long WXUNUSED(style))
{
    Init();

    for ( size_t i = 0; i < n; i++ )
    {
        m_menus.Append(menus[i]);
        m_titles.Add(titles[i]);
    }
}

wxMenuBar::~wxMenuBar()
{
    // nothing to do: wxMenuBarBase will delete the menus
}

void wxMenuBar::EnableTop(size_t WXUNUSED(pos), bool WXUNUSED(flag))
{
  //    wxFAIL_MSG("TODO");
//  wxLogWarning("wxMenuBar::EnableTop not yet implemented.");
}

void wxMenuBar::SetLabelTop(size_t pos, const wxString& label)
{
    wxMenu *menu = GetMenu(pos);
    if ( !menu )
        return;

    Widget w = (Widget)menu->GetButtonWidget();
    if (w)
    {
        wxXmString label_str(label);

        XtVaSetValues(w,
                      XmNlabelString, label_str(),
                      NULL);
    }
}

wxString wxMenuBar::GetLabelTop(size_t pos) const
{
    wxMenu *menu = GetMenu(pos);
    if ( menu )
    {
        Widget w = (Widget)menu->GetButtonWidget();
        if (w)
        {
            XmString text;
            XtVaGetValues(w,
                          XmNlabelString, &text,
                          NULL);

            return wxXmStringToString( text );
        }
    }

    return wxEmptyString;
}

bool wxMenuBar::Append(wxMenu * menu, const wxString& title)
{
    wxCHECK_MSG( menu, false, wxT("invalid menu") );
    wxCHECK_MSG( !menu->GetParent() && !menu->GetButtonWidget(), false,
                 wxT("menu already appended") );

    if ( m_menuBarFrame )
    {
        WXWidget w = menu->CreateMenu(this, GetMainWidget(), menu, title, true);
        wxCHECK_MSG( w, false, wxT("failed to create menu") );
        menu->SetButtonWidget(w);
    }

    m_titles.Add(title);

    return wxMenuBarBase::Append(menu, title);
}

bool wxMenuBar::Insert(size_t pos, wxMenu *menu, const wxString& title)
{
    if ( !wxMenuBarBase::Insert(pos, menu, title) )
        return false;

    wxFAIL_MSG(wxT("TODO"));

    return false;
}

wxMenu *wxMenuBar::Replace(size_t pos, wxMenu *menu, const wxString& title)
{
    if ( !wxMenuBarBase::Replace(pos, menu, title) )
        return NULL;

    wxFAIL_MSG(wxT("TODO"));

    return NULL;
}

wxMenu *wxMenuBar::Remove(size_t pos)
{
    wxMenu *menu = wxMenuBarBase::Remove(pos);
    if ( !menu )
        return NULL;

    if ( m_menuBarFrame )
        menu->DestroyMenu(true);

    menu->SetMenuBar(NULL);

    m_titles.RemoveAt(pos);

    return menu;
}

// Find the menu menuString, item itemString, and return the item id.
// Returns -1 if none found.
int wxMenuBar::FindMenuItem (const wxString& menuString, const wxString& itemString) const
{
    char buf1[200];
    char buf2[200];
    wxStripMenuCodes (wxConstCast(menuString.c_str(), char), buf1);

    size_t menuCount = GetMenuCount();
    for (size_t i = 0; i < menuCount; i++)
    {
        wxStripMenuCodes (wxConstCast(m_titles[i].c_str(), char), buf2);
        if (strcmp (buf1, buf2) == 0)
            return m_menus.Item(i)->GetData()->FindItem (itemString);
    }
    return -1;
}

wxMenuItem *wxMenuBar::FindItem(int id, wxMenu ** itemMenu) const
{
    if (itemMenu)
        *itemMenu = NULL;

    wxMenuItem *item = NULL;
    size_t menuCount = GetMenuCount();
    for (size_t i = 0; i < menuCount; i++)
        if ((item = m_menus.Item(i)->GetData()->FindItem(id, itemMenu)))
            return item;
        return NULL;
}

// Create menubar
bool wxMenuBar::CreateMenuBar(wxFrame* parent)
{
    if (m_mainWidget)
    {
        XtVaSetValues((Widget) parent->GetMainWidget(), XmNmenuBar, (Widget) m_mainWidget, NULL);
        /*
        if (!XtIsManaged((Widget) m_mainWidget))
        XtManageChild((Widget) m_mainWidget);
        */
        XtMapWidget((Widget) m_mainWidget);
        return true;
    }

    Widget menuBarW = XmCreateMenuBar ((Widget) parent->GetMainWidget(), "MenuBar", NULL, 0);
    m_mainWidget = (WXWidget) menuBarW;

    size_t menuCount = GetMenuCount();
    for (size_t i = 0; i < menuCount; i++)
    {
        wxMenu *menu = GetMenu(i);
        wxString title(m_titles[i]);
        menu->SetButtonWidget(menu->CreateMenu (this, menuBarW, menu, title, true));

        if (strcmp (wxStripMenuCodes(title), "Help") == 0)
            XtVaSetValues ((Widget) menuBarW, XmNmenuHelpWidget, (Widget) menu->GetButtonWidget(), NULL);

        // tear off menu support
#if (XmVersion >= 1002)
        if ( menu->IsTearOff() )
        {
            XtVaSetValues(GetWidget(menu),
                          XmNtearOffModel, XmTEAR_OFF_ENABLED,
                          NULL);
            Widget tearOff = XmGetTearOffControl(GetWidget(menu));
            wxDoChangeForegroundColour((Widget) tearOff, m_foregroundColour);
            wxDoChangeBackgroundColour((Widget) tearOff, m_backgroundColour, true);
#endif
        }
    }

    SetBackgroundColour(m_backgroundColour);
    SetForegroundColour(m_foregroundColour);
    SetFont(m_font);

    XtVaSetValues((Widget) parent->GetMainWidget(), XmNmenuBar, (Widget) m_mainWidget, NULL);
    XtRealizeWidget ((Widget) menuBarW);
    XtManageChild ((Widget) menuBarW);
    SetMenuBarFrame(parent);

    return true;
}

// Destroy menubar, but keep data structures intact so we can recreate it.
bool wxMenuBar::DestroyMenuBar()
{
    if (!m_mainWidget)
    {
        SetMenuBarFrame((wxFrame*) NULL);
        return false;
    }

    XtUnmanageChild ((Widget) m_mainWidget);
    XtUnrealizeWidget ((Widget) m_mainWidget);

    size_t menuCount = GetMenuCount();
    for (size_t i = 0; i < menuCount; i++)
    {
        wxMenu *menu = GetMenu(i);
        menu->DestroyMenu(true);

    }
    XtDestroyWidget((Widget) m_mainWidget);
    m_mainWidget = (WXWidget) 0;

    SetMenuBarFrame((wxFrame*) NULL);

    return true;
}

// Since PopupMenu under Motif stills grab right mouse button events
// after it was closed, we need to delete the associated widgets to
// allow next PopUpMenu to appear...
void wxMenu::DestroyWidgetAndDetach()
{
    if (GetMainWidget())
    {
        wxMenu *menuParent = GetParent();
        if ( menuParent )
        {
            wxMenuItemList::compatibility_iterator node = menuParent->GetMenuItems().GetFirst();
            while ( node )
            {
                if ( node->GetData()->GetSubMenu() == this )
                {
                    delete node->GetData();
                    menuParent->GetMenuItems().Erase(node);

                    break;
                }

                node = node->GetNext();
            }
        }

        DestroyMenu(true);
    }

    // Mark as no longer popped up
    m_menuId = -1;
}

/*
* Create a popup or pulldown menu.
* Submenus of a popup will be pulldown.
*
*/

WXWidget wxMenu::CreateMenu (wxMenuBar * menuBar, WXWidget parent, wxMenu * topMenu, const wxString& title, bool pullDown)
{
    Widget menu = (Widget) 0;
    Widget buttonWidget = (Widget) 0;
    Arg args[5];
    XtSetArg (args[0], XmNnumColumns, m_numColumns);
    XtSetArg (args[1], XmNpacking, (m_numColumns > 1) ? XmPACK_COLUMN : XmPACK_TIGHT);

    if (!pullDown)
    {
        menu = XmCreatePopupMenu ((Widget) parent, "popup", args, 2);
#if 0
        XtAddCallback(menu,
            XmNunmapCallback,
            (XtCallbackProc)wxMenuPopdownCallback,
            (XtPointer)this);
#endif
    }
    else
    {
        char mnem = wxFindMnemonic (title);
        menu = XmCreatePulldownMenu ((Widget) parent, "pulldown", args, 2);

        wxString title2(wxStripMenuCodes(title));
        wxXmString label_str(title2);
        buttonWidget = XtVaCreateManagedWidget(title2,
#if wxUSE_GADGETS
            xmCascadeButtonGadgetClass, (Widget) parent,
#else
            xmCascadeButtonWidgetClass, (Widget) parent,
#endif
            XmNlabelString, label_str(),
            XmNsubMenuId, menu,
            NULL);

        if (mnem != 0)
            XtVaSetValues (buttonWidget, XmNmnemonic, mnem, NULL);
    }

    m_menuWidget = (WXWidget) menu;

    m_topLevelMenu = topMenu;

    for ( wxMenuItemList::compatibility_iterator node = GetMenuItems().GetFirst();
          node;
          node = node->GetNext() )
    {
        wxMenuItem *item = node->GetData();

        item->CreateItem(menu, menuBar, topMenu);
    }

    SetBackgroundColour(m_backgroundColour);
    SetForegroundColour(m_foregroundColour);
    SetFont(m_font);

    return buttonWidget;
}

// Destroys the Motif implementation of the menu,
// but maintains the wxWidgets data structures so we can
// do a CreateMenu again.
void wxMenu::DestroyMenu (bool full)
{
    for ( wxMenuItemList::compatibility_iterator node = GetMenuItems().GetFirst();
          node;
          node = node->GetNext() )
    {
        wxMenuItem *item = node->GetData();
        item->SetMenuBar((wxMenuBar*) NULL);

        item->DestroyItem(full);
    }

    if (m_buttonWidget)
    {
        if (full)
        {
            XtVaSetValues((Widget) m_buttonWidget, XmNsubMenuId, NULL, NULL);
            XtDestroyWidget ((Widget) m_buttonWidget);
            m_buttonWidget = (WXWidget) 0;
        }
    }
    if (m_menuWidget && full)
    {
        XtDestroyWidget((Widget) m_menuWidget);
        m_menuWidget = (WXWidget) NULL;
    }
}

WXWidget wxMenu::FindMenuItem (int id, wxMenuItem ** it) const
{
    if (id == m_menuId)
    {
        if (it)
            *it = (wxMenuItem*) NULL;
        return m_buttonWidget;
    }

    for ( wxMenuItemList::compatibility_iterator node = GetMenuItems().GetFirst();
          node;
          node = node->GetNext() )
    {
        wxMenuItem *item = node->GetData ();
        if (item->GetId() == id)
        {
            if (it)
                *it = item;
            return item->GetButtonWidget();
        }

        if (item->GetSubMenu())
        {
            WXWidget w = item->GetSubMenu()->FindMenuItem (id, it);
            if (w)
            {
                return w;
            }
        }
    }

    if (it)
        *it = (wxMenuItem*) NULL;
    return (WXWidget) NULL;
}

void wxMenu::SetBackgroundColour(const wxColour& col)
{
    m_backgroundColour = col;
    if (m_menuWidget)
        wxDoChangeBackgroundColour(m_menuWidget, (wxColour&) col);
    if (m_buttonWidget)
        wxDoChangeBackgroundColour(m_buttonWidget, (wxColour&) col, true);

    for ( wxMenuItemList::compatibility_iterator node = GetMenuItems().GetFirst();
          node;
          node = node->GetNext() )
    {
        wxMenuItem* item = node->GetData();
        if (item->GetButtonWidget())
        {
            // This crashes because it uses gadgets
            //            wxDoChangeBackgroundColour(item->GetButtonWidget(), (wxColour&) col, true);
        }
        if (item->GetSubMenu())
            item->GetSubMenu()->SetBackgroundColour((wxColour&) col);
    }
}

void wxMenu::SetForegroundColour(const wxColour& col)
{
    m_foregroundColour = col;
    if (m_menuWidget)
        wxDoChangeForegroundColour(m_menuWidget, (wxColour&) col);
    if (m_buttonWidget)
        wxDoChangeForegroundColour(m_buttonWidget, (wxColour&) col);

    for ( wxMenuItemList::compatibility_iterator node = GetMenuItems().GetFirst();
          node;
          node = node->GetNext() )
    {
        wxMenuItem* item = node->GetData();
        if (item->GetButtonWidget())
        {
            // This crashes because it uses gadgets
            //            wxDoChangeForegroundColour(item->GetButtonWidget(), (wxColour&) col);
        }
        if (item->GetSubMenu())
            item->GetSubMenu()->SetForegroundColour((wxColour&) col);
    }
}

void wxMenu::ChangeFont(bool keepOriginalSize)
{
    // Lesstif 0.87 hangs here, but 0.93 does not
#if !wxCHECK_LESSTIF() || wxCHECK_LESSTIF_VERSION( 0, 93 )
    if (!m_font.Ok() || !m_menuWidget)
        return;

    WXFontType fontType = m_font.GetFontType(XtDisplay((Widget) m_menuWidget));

    XtVaSetValues ((Widget) m_menuWidget,
                   wxFont::GetFontTag(), fontType,
                   NULL);
    if (m_buttonWidget)
    {
        XtVaSetValues ((Widget) m_buttonWidget,
                       wxFont::GetFontTag(), fontType,
                       NULL);
    }

    for ( wxMenuItemList::compatibility_iterator node = GetMenuItems().GetFirst();
          node;
          node = node->GetNext() )
    {
        wxMenuItem* item = node->GetData();
        if (m_menuWidget && item->GetButtonWidget() && m_font.Ok())
        {
            XtVaSetValues ((Widget) item->GetButtonWidget(),
                           wxFont::GetFontTag(), fontType,
                           NULL);
        }
        if (item->GetSubMenu())
            item->GetSubMenu()->ChangeFont(keepOriginalSize);
    }
#endif
}

void wxMenu::SetFont(const wxFont& font)
{
    m_font = font;
    ChangeFont();
}

bool wxMenuBar::SetBackgroundColour(const wxColour& col)
{
    m_backgroundColour = col;
    if (m_mainWidget)
        wxDoChangeBackgroundColour(m_mainWidget, (wxColour&) col);

    size_t menuCount = GetMenuCount();
    for (size_t i = 0; i < menuCount; i++)
        m_menus.Item(i)->GetData()->SetBackgroundColour((wxColour&) col);

    return true;
}

bool wxMenuBar::SetForegroundColour(const wxColour& col)
{
    m_foregroundColour = col;
    if (m_mainWidget)
        wxDoChangeForegroundColour(m_mainWidget, (wxColour&) col);

    size_t menuCount = GetMenuCount();
    for (size_t i = 0; i < menuCount; i++)
        m_menus.Item(i)->GetData()->SetForegroundColour((wxColour&) col);

    return true;
}

void wxMenuBar::ChangeFont(bool WXUNUSED(keepOriginalSize))
{
    // Nothing to do for menubar, fonts are kept in wxMenus
}

bool wxMenuBar::SetFont(const wxFont& font)
{
    m_font = font;
    ChangeFont();

    size_t menuCount = GetMenuCount();
    for (size_t i = 0; i < menuCount; i++)
        m_menus.Item(i)->GetData()->SetFont(font);

    return true;
}

