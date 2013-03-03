/////////////////////////////////////////////////////////////////////////////
// Name:        msw/tbar95.cpp
// Purpose:     wxToolBar
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id: tbar95.cpp,v 1.153 2005/05/27 09:14:58 JS Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "tbar95.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/frame.h"
    #include "wx/log.h"
    #include "wx/intl.h"
    #include "wx/dynarray.h"
    #include "wx/settings.h"
    #include "wx/bitmap.h"
    #include "wx/dcmemory.h"
    #include "wx/control.h"
#endif

#if wxUSE_TOOLBAR && wxUSE_TOOLBAR_NATIVE && !defined(__SMARTPHONE__)

#include "wx/toolbar.h"
#include "wx/sysopt.h"
#include "wx/image.h"

#include "wx/msw/private.h"

#if wxUSE_UXTHEME
#include "wx/msw/uxtheme.h"
#endif

// include <commctrl.h> "properly"
#include "wx/msw/wrapcctl.h"

#include "wx/app.h"         // for GetComCtl32Version

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// these standard constants are not always defined in compilers headers

// Styles
#ifndef TBSTYLE_FLAT
    #define TBSTYLE_LIST            0x1000
    #define TBSTYLE_FLAT            0x0800
#endif

#ifndef TBSTYLE_TRANSPARENT
    #define TBSTYLE_TRANSPARENT     0x8000
#endif

#ifndef TBSTYLE_TOOLTIPS
    #define TBSTYLE_TOOLTIPS        0x0100
#endif

// Messages
#ifndef TB_GETSTYLE
    #define TB_SETSTYLE             (WM_USER + 56)
    #define TB_GETSTYLE             (WM_USER + 57)
#endif

#ifndef TB_HITTEST
    #define TB_HITTEST              (WM_USER + 69)
#endif

#ifndef TB_GETMAXSIZE
    #define TB_GETMAXSIZE           (WM_USER + 83)
#endif

// these values correspond to those used by comctl32.dll
#define DEFAULTBITMAPX   16
#define DEFAULTBITMAPY   15
#define DEFAULTBUTTONX   24
#define DEFAULTBUTTONY   24
#define DEFAULTBARHEIGHT 27

// ----------------------------------------------------------------------------
// wxWin macros
// ----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxToolBar, wxControl)

/*
    TOOLBAR PROPERTIES
        tool
            bitmap
            bitmap2
            tooltip
            longhelp
            radio (bool)
            toggle (bool)
        separator
        style ( wxNO_BORDER | wxTB_HORIZONTAL)
        bitmapsize
        margins
        packing
        separation

        dontattachtoframe
*/

BEGIN_EVENT_TABLE(wxToolBar, wxToolBarBase)
    EVT_MOUSE_EVENTS(wxToolBar::OnMouseEvent)
    EVT_SYS_COLOUR_CHANGED(wxToolBar::OnSysColourChanged)
    EVT_ERASE_BACKGROUND(wxToolBar::OnEraseBackground)
END_EVENT_TABLE()

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

class wxToolBarTool : public wxToolBarToolBase
{
public:
    wxToolBarTool(wxToolBar *tbar,
                  int id,
                  const wxString& label,
                  const wxBitmap& bmpNormal,
                  const wxBitmap& bmpDisabled,
                  wxItemKind kind,
                  wxObject *clientData,
                  const wxString& shortHelp,
                  const wxString& longHelp)
        : wxToolBarToolBase(tbar, id, label, bmpNormal, bmpDisabled, kind,
                            clientData, shortHelp, longHelp)
    {
        m_nSepCount = 0;
    }

    wxToolBarTool(wxToolBar *tbar, wxControl *control)
        : wxToolBarToolBase(tbar, control)
    {
        m_nSepCount = 1;
    }

    virtual void SetLabel(const wxString& label)
    {
        if ( label == m_label )
            return;

        wxToolBarToolBase::SetLabel(label);

        // we need to update the label shown in the toolbar because it has a
        // pointer to the internal buffer of the old label
        //
        // TODO: use TB_SETBUTTONINFO
    }

    // set/get the number of separators which we use to cover the space used by
    // a control in the toolbar
    void SetSeparatorsCount(size_t count) { m_nSepCount = count; }
    size_t GetSeparatorsCount() const { return m_nSepCount; }

private:
    size_t m_nSepCount;

    DECLARE_NO_COPY_CLASS(wxToolBarTool)
};


// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxToolBarTool
// ----------------------------------------------------------------------------

wxToolBarToolBase *wxToolBar::CreateTool(int id,
                                         const wxString& label,
                                         const wxBitmap& bmpNormal,
                                         const wxBitmap& bmpDisabled,
                                         wxItemKind kind,
                                         wxObject *clientData,
                                         const wxString& shortHelp,
                                         const wxString& longHelp)
{
    return new wxToolBarTool(this, id, label, bmpNormal, bmpDisabled, kind,
                             clientData, shortHelp, longHelp);
}

wxToolBarToolBase *wxToolBar::CreateTool(wxControl *control)
{
    return new wxToolBarTool(this, control);
}

// ----------------------------------------------------------------------------
// wxToolBar construction
// ----------------------------------------------------------------------------

void wxToolBar::Init()
{
    m_hBitmap = 0;
    m_disabledImgList = NULL;

    m_nButtons = 0;

    m_defaultWidth = DEFAULTBITMAPX;
    m_defaultHeight = DEFAULTBITMAPY;

    m_pInTool = 0;
}

bool wxToolBar::Create(wxWindow *parent,
                       wxWindowID id,
                       const wxPoint& pos,
                       const wxSize& size,
                       long style,
                       const wxString& name)
{
    // common initialisation
    if ( !CreateControl(parent, id, pos, size, style, wxDefaultValidator, name) )
        return false;

    // MSW-specific initialisation
    if ( !MSWCreateToolbar(pos, size) )
        return false;

    wxSetCCUnicodeFormat(GetHwnd());

    // set up the colors and fonts
    SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
    SetFont(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT));

    // workaround for flat toolbar on Windows XP classic style
#if wxUSE_UXTHEME
    if ( style & wxTB_FLAT )
    {
        // Testing for an active theme appears to be unnecessary (see comments in patch 1204217).
        // Disabling the test brings back separator lines.
        // However, the separators can look ugly and distracting, especially between controls,
        // so I'm restoring the test and removing the separators again - JACS
#if 1
        wxUxThemeEngine *p = wxUxThemeEngine::Get();
        if ( !p || !p->IsThemeActive() )
#endif            
        {
            DWORD dwToolbarStyle;

            dwToolbarStyle = (DWORD)::SendMessage(GetHwnd(), TB_GETSTYLE, 0, 0L );

            if ((dwToolbarStyle & TBSTYLE_FLAT) == 0)
            {
                dwToolbarStyle |= TBSTYLE_FLAT;
                ::SendMessage(GetHwnd(), TB_SETSTYLE, 0, (LPARAM)dwToolbarStyle );
            }
        }
    }
#endif

    return true;
}

bool wxToolBar::MSWCreateToolbar(const wxPoint& pos, const wxSize& size)
{
    if ( !MSWCreateControl(TOOLBARCLASSNAME, wxEmptyString, pos, size) )
        return false;

    // toolbar-specific post initialisation
    ::SendMessage(GetHwnd(), TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), 0);

    return true;
}

void wxToolBar::Recreate()
{
    const HWND hwndOld = GetHwnd();
    if ( !hwndOld )
    {
        // we haven't been created yet, no need to recreate
        return;
    }

    // get the position and size before unsubclassing the old toolbar
    const wxPoint pos = GetPosition();
    const wxSize size = GetSize();

    UnsubclassWin();

    if ( !MSWCreateToolbar(pos, size) )
    {
        // what can we do?
        wxFAIL_MSG( _T("recreating the toolbar failed") );

        return;
    }

    // reparent all our children under the new toolbar
    for ( wxWindowList::compatibility_iterator node = m_children.GetFirst();
          node;
          node = node->GetNext() )
    {
        wxWindow *win = node->GetData();
        if ( !win->IsTopLevel() )
            ::SetParent(GetHwndOf(win), GetHwnd());
    }

    // only destroy the old toolbar now -- after all the children had been
    // reparented
    ::DestroyWindow(hwndOld);

    // it is for the old bitmap control and can't be used with the new one
    if ( m_hBitmap )
    {
        ::DeleteObject((HBITMAP) m_hBitmap);
        m_hBitmap = 0;
    }

    if ( m_disabledImgList )
    {
        delete m_disabledImgList;
        m_disabledImgList = NULL;
    }

    Realize();
    UpdateSize();
}

wxToolBar::~wxToolBar()
{
    // we must refresh the frame size when the toolbar is deleted but the frame
    // is not - otherwise toolbar leaves a hole in the place it used to occupy
    wxFrame *frame = wxDynamicCast(GetParent(), wxFrame);
    if ( frame && !frame->IsBeingDeleted() )
    {
        frame->SendSizeEvent();
    }

    if ( m_hBitmap )
    {
        ::DeleteObject((HBITMAP) m_hBitmap);
    }

    delete m_disabledImgList;
}

wxSize wxToolBar::DoGetBestSize() const
{
    wxSize sizeBest;

    SIZE size;
    if ( !::SendMessage(GetHwnd(), TB_GETMAXSIZE, 0, (LPARAM)&size) )
    {
        // maybe an old (< 0x400) Windows version? try to approximate the
        // toolbar size ourselves
        sizeBest = GetToolSize();
        sizeBest.y += 2 * ::GetSystemMetrics(SM_CYBORDER); // Add borders
        sizeBest.x *= GetToolsCount();

        // reverse horz and vertical components if necessary
        if ( HasFlag(wxTB_VERTICAL) )
        {
            int t = sizeBest.x;
            sizeBest.x = sizeBest.y;
            sizeBest.y = t;
        }
    }
    else
    {
        sizeBest.x = size.cx;
        sizeBest.y = size.cy;
    }

    CacheBestSize(sizeBest);
    return sizeBest;
}

WXDWORD wxToolBar::MSWGetStyle(long style, WXDWORD *exstyle) const
{
    // toolbars never have border, giving one to them results in broken
    // appearance
    WXDWORD msStyle = wxControl::MSWGetStyle
                      (
                        (style & ~wxBORDER_MASK) | wxBORDER_NONE, exstyle
                      );

    // always include this one, it never hurts and setting it later only if we
    // do have tooltips wouldn't work
    msStyle |= TBSTYLE_TOOLTIPS;

    if ( style & (wxTB_FLAT | wxTB_HORZ_LAYOUT) )
    {
        // static as it doesn't change during the program lifetime
        static int s_verComCtl = wxApp::GetComCtl32Version();

        // comctl32.dll 4.00 doesn't support the flat toolbars and using this
        // style with 6.00 (part of Windows XP) leads to the toolbar with
        // incorrect background colour - and not using it still results in the
        // correct (flat) toolbar, so don't use it there
        if ( s_verComCtl > 400 && s_verComCtl < 600 )
        {
            msStyle |= TBSTYLE_FLAT | TBSTYLE_TRANSPARENT;
        }

        if ( s_verComCtl >= 470 && style & wxTB_HORZ_LAYOUT )
        {
            msStyle |= TBSTYLE_LIST;
        }
    }

    if ( style & wxTB_NODIVIDER )
        msStyle |= CCS_NODIVIDER;

    if ( style & wxTB_NOALIGN )
        msStyle |= CCS_NOPARENTALIGN;

    if ( style & wxTB_VERTICAL )
        msStyle |= CCS_VERT;

    return msStyle;
}

// ----------------------------------------------------------------------------
// adding/removing tools
// ----------------------------------------------------------------------------

bool wxToolBar::DoInsertTool(size_t WXUNUSED(pos), wxToolBarToolBase *tool)
{
    // nothing special to do here - we really create the toolbar buttons in
    // Realize() later
    tool->Attach(this);

    InvalidateBestSize();
    return true;
}

bool wxToolBar::DoDeleteTool(size_t pos, wxToolBarToolBase *tool)
{
    // the main difficulty we have here is with the controls in the toolbars:
    // as we (sometimes) use several separators to cover up the space used by
    // them, the indices are not the same for us and the toolbar

    // first determine the position of the first button to delete: it may be
    // different from pos if we use several separators to cover the space used
    // by a control
    wxToolBarToolsList::compatibility_iterator node;
    for ( node = m_tools.GetFirst(); node; node = node->GetNext() )
    {
        wxToolBarToolBase *tool2 = node->GetData();
        if ( tool2 == tool )
        {
            // let node point to the next node in the list
            node = node->GetNext();

            break;
        }

        if ( tool2->IsControl() )
        {
            pos += ((wxToolBarTool *)tool2)->GetSeparatorsCount() - 1;
        }
    }

    // now determine the number of buttons to delete and the area taken by them
    size_t nButtonsToDelete = 1;

    // get the size of the button we're going to delete
    RECT r;
    if ( !::SendMessage(GetHwnd(), TB_GETITEMRECT, pos, (LPARAM)&r) )
    {
        wxLogLastError(_T("TB_GETITEMRECT"));
    }

    int width = r.right - r.left;

    if ( tool->IsControl() )
    {
        nButtonsToDelete = ((wxToolBarTool *)tool)->GetSeparatorsCount();

        width *= nButtonsToDelete;
    }

    // do delete all buttons
    m_nButtons -= nButtonsToDelete;
    while ( nButtonsToDelete-- > 0 )
    {
        if ( !::SendMessage(GetHwnd(), TB_DELETEBUTTON, pos, 0) )
        {
            wxLogLastError(wxT("TB_DELETEBUTTON"));

            return false;
        }
    }

    tool->Detach();

    // and finally reposition all the controls after this button (the toolbar
    // takes care of all normal items)
    for ( /* node -> first after deleted */ ; node; node = node->GetNext() )
    {
        wxToolBarToolBase *tool2 = node->GetData();
        if ( tool2->IsControl() )
        {
            int x;
            wxControl *control = tool2->GetControl();
            control->GetPosition(&x, NULL);
            control->Move(x - width, wxDefaultCoord);
        }
    }

    InvalidateBestSize();
    return true;
}

void wxToolBar::CreateDisabledImageList()
{
    // as we can't use disabled image list with older versions of comctl32.dll,
    // don't even bother creating it
    if ( wxTheApp->GetComCtl32Version() >= 470 )
    {
        // search for the first disabled button img in the toolbar, if any
        for ( wxToolBarToolsList::compatibility_iterator
                node = m_tools.GetFirst(); node; node = node->GetNext() )
        {
            wxToolBarToolBase *tool = node->GetData();
            wxBitmap bmpDisabled = tool->GetDisabledBitmap();
            if ( bmpDisabled.Ok() )
            {
                m_disabledImgList = new wxImageList
                                        (
                                            m_defaultWidth,
                                            m_defaultHeight,
                                            bmpDisabled.GetMask() != NULL,
                                            GetToolsCount()
                                        );
                return;
            }
        }

        // we don't have any disabled bitmaps
    }

    m_disabledImgList = NULL;
}

bool wxToolBar::Realize()
{
    const size_t nTools = GetToolsCount();
    if ( nTools == 0 )
    {
        // nothing to do
        return true;
    }

    const bool isVertical = HasFlag(wxTB_VERTICAL);

    bool doRemap, doRemapBg, doTransparent;
#ifdef __WXWINCE__
    doRemapBg = false;
    doRemap = false;
    doTransparent = false;
#else
    if (wxSystemOptions::GetOptionInt(wxT("msw.remap")) == 2)
    {
        doRemapBg = doRemap = false;
        doTransparent = true;
    }
    else
    {   doRemap = !wxSystemOptions::HasOption(wxT("msw.remap"))
            || wxSystemOptions::GetOptionInt(wxT("msw.remap")) == 1;
        doRemapBg = !doRemap;
        doTransparent = false;
    }
#endif

    // delete all old buttons, if any
    for ( size_t pos = 0; pos < m_nButtons; pos++ )
    {
        if ( !::SendMessage(GetHwnd(), TB_DELETEBUTTON, 0, 0) )
        {
            wxLogDebug(wxT("TB_DELETEBUTTON failed"));
        }
    }

    // First, add the bitmap: we use one bitmap for all toolbar buttons
    // ----------------------------------------------------------------

    wxToolBarToolsList::compatibility_iterator node;
    int bitmapId = 0;

    wxSize sizeBmp;
    if ( HasFlag(wxTB_NOICONS) )
    {
        // no icons, don't leave space for them
        sizeBmp.x =
        sizeBmp.y = 0;
    }
    else // do show icons
    {
        // if we already have a bitmap, we'll replace the existing one --
        // otherwise we'll install a new one
        HBITMAP oldToolBarBitmap = (HBITMAP)m_hBitmap;

        sizeBmp.x = m_defaultWidth;
        sizeBmp.y = m_defaultHeight;

        const wxCoord totalBitmapWidth = m_defaultWidth * nTools,
                      totalBitmapHeight = m_defaultHeight;

        // Create a bitmap and copy all the tool bitmaps to it
        wxMemoryDC dcAllButtons;
        wxBitmap bitmap(totalBitmapWidth, totalBitmapHeight);
        dcAllButtons.SelectObject(bitmap);
#ifdef __WXWINCE__
        dcAllButtons.SetBackground(wxBrush(wxColour(192,192,192)));
#else
        if (doTransparent)
            dcAllButtons.SetBackground(*wxTRANSPARENT_BRUSH);
        else
            dcAllButtons.SetBackground(*wxLIGHT_GREY_BRUSH);
#endif
        dcAllButtons.Clear();

        m_hBitmap = bitmap.GetHBITMAP();
        HBITMAP hBitmap = (HBITMAP)m_hBitmap;

#ifndef __WXWINCE__
        if (doRemapBg)
        {
            dcAllButtons.SelectObject(wxNullBitmap);

            // Even if we're not remapping the bitmap
            // content, we still have to remap the background.
            hBitmap = (HBITMAP)MapBitmap((WXHBITMAP) hBitmap,
                totalBitmapWidth, totalBitmapHeight);

            dcAllButtons.SelectObject(bitmap);


        }
#endif // !__WXWINCE__

        // the button position
        wxCoord x = 0;

        // the number of buttons (not separators)
        int nButtons = 0;

        CreateDisabledImageList();
        for ( node = m_tools.GetFirst(); node; node = node->GetNext() )
        {
            wxToolBarToolBase *tool = node->GetData();
            if ( tool->IsButton() )
            {
                const wxBitmap& bmp = tool->GetNormalBitmap();

                const int w = bmp.GetWidth();
                const int h = bmp.GetHeight();

                if ( bmp.Ok() )
                {
                    int xOffset = wxMax(0, (m_defaultWidth - w)/2);
                    int yOffset = wxMax(0, (m_defaultHeight - h)/2);

                    // notice the last parameter: do use mask
                    dcAllButtons.DrawBitmap(bmp, x + xOffset, yOffset, true);
                }
                else
                {
                    wxFAIL_MSG( _T("invalid tool button bitmap") );
                }

                // also deal with disabled bitmap if we want to use them
                if ( m_disabledImgList )
                {
                    wxBitmap bmpDisabled = tool->GetDisabledBitmap();
#if wxUSE_IMAGE
                    if ( !bmpDisabled.Ok() )
                    {
                        // no disabled bitmap specified but we still need to
                        // fill the space in the image list with something, so
                        // we grey out the normal bitmap
                        wxImage imgGreyed;
                        wxCreateGreyedImage(bmp.ConvertToImage(), imgGreyed);

                        // we need to have light grey background colour for
                        // MapBitmap() to work correctly
                        for ( int y = 0; y < h; y++ )
                        {
                            for ( int x = 0; x < w; x++ )
                            {
                                if ( imgGreyed.IsTransparent(x, y) )
                                    imgGreyed.SetRGB(x, y,
                                                     wxLIGHT_GREY->Red(),
                                                     wxLIGHT_GREY->Green(),
                                                     wxLIGHT_GREY->Blue());
                            }
                        }

                        bmpDisabled = wxBitmap(imgGreyed);
                    }
#endif // wxUSE_IMAGE

                    MapBitmap(bmpDisabled.GetHBITMAP(), w, h);

                    m_disabledImgList->Add(bmpDisabled);
                }

                // still inc width and number of buttons because otherwise the
                // subsequent buttons will all be shifted which is rather confusing
                // (and like this you'd see immediately which bitmap was bad)
                x += m_defaultWidth;
                nButtons++;
            }
        }

        dcAllButtons.SelectObject(wxNullBitmap);

        // don't delete this HBITMAP!
        bitmap.SetHBITMAP(0);

        if (doRemap)
        {
            // Map to system colours
            hBitmap = (HBITMAP)MapBitmap((WXHBITMAP) hBitmap,
                totalBitmapWidth, totalBitmapHeight);


        }

        bool addBitmap = true;

        if ( oldToolBarBitmap )
        {
#ifdef TB_REPLACEBITMAP
            if ( wxApp::GetComCtl32Version() >= 400 )
            {
                TBREPLACEBITMAP replaceBitmap;
                replaceBitmap.hInstOld = NULL;
                replaceBitmap.hInstNew = NULL;
                replaceBitmap.nIDOld = (UINT) oldToolBarBitmap;
                replaceBitmap.nIDNew = (UINT) hBitmap;
                replaceBitmap.nButtons = nButtons;
                if ( !::SendMessage(GetHwnd(), TB_REPLACEBITMAP,
                                    0, (LPARAM) &replaceBitmap) )
                {
                    wxFAIL_MSG(wxT("Could not replace the old bitmap"));
                }

                ::DeleteObject(oldToolBarBitmap);

                // already done
                addBitmap = false;
            }
            else
#endif // TB_REPLACEBITMAP
            {
                // we can't replace the old bitmap, so we will add another one
                // (awfully inefficient, but what else to do?) and shift the bitmap
                // indices accordingly
                addBitmap = true;

                bitmapId = m_nButtons;
            }
        }

        if ( addBitmap ) // no old bitmap or we can't replace it
        {
            TBADDBITMAP addBitmap;
            addBitmap.hInst = 0;
            addBitmap.nID = (UINT) hBitmap;
            if ( ::SendMessage(GetHwnd(), TB_ADDBITMAP,
                               (WPARAM) nButtons, (LPARAM)&addBitmap) == -1 )
            {
                wxFAIL_MSG(wxT("Could not add bitmap to toolbar"));
            }
        }

        if ( m_disabledImgList )
        {
            HIMAGELIST oldImageList = (HIMAGELIST)
                ::SendMessage(GetHwnd(),
                              TB_SETDISABLEDIMAGELIST,
                              0,
                              (LPARAM)GetHimagelistOf(m_disabledImgList));

            // delete previous image list if any
            if ( oldImageList )
                ::DeleteObject( oldImageList );
        }
    }

    // don't call SetToolBitmapSize() as we don't want to change the values of
    // m_defaultWidth/Height
    if ( !::SendMessage(GetHwnd(), TB_SETBITMAPSIZE, 0,
                        MAKELONG(sizeBmp.x, sizeBmp.y)) )
    {
        wxLogLastError(_T("TB_SETBITMAPSIZE"));
    }

    // Next add the buttons and separators
    // -----------------------------------

    TBBUTTON *buttons = new TBBUTTON[nTools];

    // this array will hold the indices of all controls in the toolbar
    wxArrayInt controlIds;

    bool lastWasRadio = false;
    int i = 0;
    for ( node = m_tools.GetFirst(); node; node = node->GetNext() )
    {
        wxToolBarToolBase *tool = node->GetData();

        // don't add separators to the vertical toolbar with old comctl32.dll
        // versions as they didn't handle this properly
        if ( isVertical && tool->IsSeparator() &&
                wxApp::GetComCtl32Version() <= 472 )
        {
            continue;
        }


        TBBUTTON& button = buttons[i];

        wxZeroMemory(button);

        bool isRadio = false;
        switch ( tool->GetStyle() )
        {
            case wxTOOL_STYLE_CONTROL:
                button.idCommand = tool->GetId();
                // fall through: create just a separator too

            case wxTOOL_STYLE_SEPARATOR:
                button.fsState = TBSTATE_ENABLED;
                button.fsStyle = TBSTYLE_SEP;
                break;

            case wxTOOL_STYLE_BUTTON:
                if ( !HasFlag(wxTB_NOICONS) )
                    button.iBitmap = bitmapId;

                if ( HasFlag(wxTB_TEXT) )
                {
                    const wxString& label = tool->GetLabel();
                    if ( !label.empty() )
                    {
                        button.iString = (int)label.c_str();
                    }
                }

                button.idCommand = tool->GetId();

                if ( tool->IsEnabled() )
                    button.fsState |= TBSTATE_ENABLED;
                if ( tool->IsToggled() )
                    button.fsState |= TBSTATE_CHECKED;

                switch ( tool->GetKind() )
                {
                    case wxITEM_RADIO:
                        button.fsStyle = TBSTYLE_CHECKGROUP;

                        if ( !lastWasRadio )
                        {
                            // the first item in the radio group is checked by
                            // default to be consistent with wxGTK and the menu
                            // radio items
                            button.fsState |= TBSTATE_CHECKED;

                            if (tool->Toggle(true))
                            {
                                DoToggleTool(tool, true);
                            }
                        }
                        else if (tool->IsToggled())
                        {
                            wxToolBarToolsList::compatibility_iterator nodePrev = node->GetPrevious();
                            int prevIndex = i - 1;
                            while ( nodePrev )
                            {
                                TBBUTTON& prevButton = buttons[prevIndex];
                                wxToolBarToolBase *tool = nodePrev->GetData();
                                if ( !tool->IsButton() || tool->GetKind() != wxITEM_RADIO )
                                    break;
                                
                                if ( tool->Toggle(false) )
                                {
                                    DoToggleTool(tool, false);
                                }
                                prevButton.fsState = TBSTATE_ENABLED;
                                nodePrev = nodePrev->GetPrevious();
                                prevIndex--;
                            }                            
                        }

                        isRadio = true;
                        break;

                    case wxITEM_CHECK:
                        button.fsStyle = TBSTYLE_CHECK;
                        break;

                    default:
                        wxFAIL_MSG( _T("unexpected toolbar button kind") );
                        // fall through

                    case wxITEM_NORMAL:
                        button.fsStyle = TBSTYLE_BUTTON;
                }

                bitmapId++;
                break;
        }

        lastWasRadio = isRadio;

        i++;
    }

    if ( !::SendMessage(GetHwnd(), TB_ADDBUTTONS, (WPARAM)i, (LPARAM)buttons) )
    {
        wxLogLastError(wxT("TB_ADDBUTTONS"));
    }

    delete [] buttons;

    // Deal with the controls finally
    // ------------------------------

    // adjust the controls size to fit nicely in the toolbar
    int y = 0;
    size_t index = 0;
    for ( node = m_tools.GetFirst(); node; node = node->GetNext(), index++ )
    {
        wxToolBarToolBase *tool = node->GetData();

        // we calculate the running y coord for vertical toolbars so we need to
        // get the items size for all items but for the horizontal ones we
        // don't need to deal with the non controls
        bool isControl = tool->IsControl();
        if ( !isControl && !isVertical )
            continue;

        // note that we use TB_GETITEMRECT and not TB_GETRECT because the
        // latter only appeared in v4.70 of comctl32.dll
        RECT r;
        if ( !::SendMessage(GetHwnd(), TB_GETITEMRECT,
                            index, (LPARAM)(LPRECT)&r) )
        {
            wxLogLastError(wxT("TB_GETITEMRECT"));
        }

        if ( !isControl )
        {
            // can only be control if isVertical
            y += r.bottom - r.top;

            continue;
        }

        wxControl *control = tool->GetControl();

        wxSize size = control->GetSize();

        // the position of the leftmost controls corner
        int left = wxDefaultCoord;

        // TB_SETBUTTONINFO message is only supported by comctl32.dll 4.71+
#ifdef TB_SETBUTTONINFO
        // available in headers, now check whether it is available now
        // (during run-time)
        if ( wxApp::GetComCtl32Version() >= 471 )
        {
            // set the (underlying) separators width to be that of the
            // control
            TBBUTTONINFO tbbi;
            tbbi.cbSize = sizeof(tbbi);
            tbbi.dwMask = TBIF_SIZE;
            tbbi.cx = (WORD)size.x;
            if ( !::SendMessage(GetHwnd(), TB_SETBUTTONINFO,
                                tool->GetId(), (LPARAM)&tbbi) )
            {
                // the id is probably invalid?
                wxLogLastError(wxT("TB_SETBUTTONINFO"));
            }
        }
        else
#endif // comctl32.dll 4.71
        // TB_SETBUTTONINFO unavailable
        {
            // try adding several separators to fit the controls width
            int widthSep = r.right - r.left;
            left = r.left;

            TBBUTTON tbb;
            wxZeroMemory(tbb);
            tbb.idCommand = 0;
            tbb.fsState = TBSTATE_ENABLED;
            tbb.fsStyle = TBSTYLE_SEP;

            size_t nSeparators = size.x / widthSep;
            for ( size_t nSep = 0; nSep < nSeparators; nSep++ )
            {
                if ( !::SendMessage(GetHwnd(), TB_INSERTBUTTON,
                                    index, (LPARAM)&tbb) )
                {
                    wxLogLastError(wxT("TB_INSERTBUTTON"));
                }

                index++;
            }

            // remember the number of separators we used - we'd have to
            // delete all of them later
            ((wxToolBarTool *)tool)->SetSeparatorsCount(nSeparators);

            // adjust the controls width to exactly cover the separators
            control->SetSize((nSeparators + 1)*widthSep, wxDefaultCoord);
        }

        // position the control itself correctly vertically
        int height = r.bottom - r.top;
        int diff = height - size.y;
        if ( diff < 0 )
        {
            // the control is too high, resize to fit
            control->SetSize(wxDefaultCoord, height - 2);

            diff = 2;
        }

        int top;
        if ( isVertical )
        {
            left = 0;
            top = y;

            y += height + 2*GetMargins().y;
        }
        else // horizontal toolbar
        {
            if ( left == wxDefaultCoord )
                left = r.left;

            top = r.top;
        }

        control->Move(left, top + (diff + 1) / 2);
    }

    // the max index is the "real" number of buttons - i.e. counting even the
    // separators which we added just for aligning the controls
    m_nButtons = index;

    if ( !isVertical )
    {
        if ( m_maxRows == 0 )
        {
            // if not set yet, only one row
            SetRows(1);
        }
    }
    else if ( m_nButtons > 0 ) // vertical non empty toolbar
    {
        if ( m_maxRows == 0 )
        {
            // if not set yet, have one column
            SetRows(m_nButtons);
        }
    }

    InvalidateBestSize();
    return true;
}

// ----------------------------------------------------------------------------
// message handlers
// ----------------------------------------------------------------------------

bool wxToolBar::MSWCommand(WXUINT WXUNUSED(cmd), WXWORD id)
{
    wxToolBarToolBase *tool = FindById((int)id);
    if ( !tool )
        return false;

    bool toggled = false; // just to suppress warnings

    if ( tool->CanBeToggled() )
    {
        LRESULT state = ::SendMessage(GetHwnd(), TB_GETSTATE, id, 0);
        toggled = (state & TBSTATE_CHECKED) != 0;

        // ignore the event when a radio button is released, as this doesn't
        // seem to happen at all, and is handled otherwise
        if ( tool->GetKind() == wxITEM_RADIO && !toggled )
            return true;

        tool->Toggle(toggled);
        UnToggleRadioGroup(tool);
    }

    // OnLeftClick() can veto the button state change - for buttons which
    // may be toggled only, of couse
    if ( !OnLeftClick((int)id, toggled) && tool->CanBeToggled() )
    {
        // revert back
        tool->Toggle(!toggled);

        ::SendMessage(GetHwnd(), TB_CHECKBUTTON, id, MAKELONG(!toggled, 0));
    }

    return true;
}

bool wxToolBar::MSWOnNotify(int WXUNUSED(idCtrl),
                            WXLPARAM lParam,
                            WXLPARAM *WXUNUSED(result))
{
#if wxUSE_TOOLTIPS
    // First check if this applies to us
    NMHDR *hdr = (NMHDR *)lParam;

    // the tooltips control created by the toolbar is sometimes Unicode, even
    // in an ANSI application - this seems to be a bug in comctl32.dll v5
    UINT code = hdr->code;
    if ( (code != (UINT) TTN_NEEDTEXTA) && (code != (UINT) TTN_NEEDTEXTW) )
        return false;

    HWND toolTipWnd = (HWND)::SendMessage((HWND)GetHWND(), TB_GETTOOLTIPS, 0, 0);
    if ( toolTipWnd != hdr->hwndFrom )
        return false;

    LPTOOLTIPTEXT ttText = (LPTOOLTIPTEXT)lParam;
    int id = (int)ttText->hdr.idFrom;

    wxToolBarToolBase *tool = FindById(id);
    if ( !tool )
        return false;

    return HandleTooltipNotify(code, lParam, tool->GetShortHelp());
#else
    wxUnusedVar(lParam);

    return false;
#endif
}

// ----------------------------------------------------------------------------
// toolbar geometry
// ----------------------------------------------------------------------------

void wxToolBar::SetToolBitmapSize(const wxSize& size)
{
    wxToolBarBase::SetToolBitmapSize(size);

    ::SendMessage(GetHwnd(), TB_SETBITMAPSIZE, 0, MAKELONG(size.x, size.y));
}

void wxToolBar::SetRows(int nRows)
{
    if ( nRows == m_maxRows )
    {
        // avoid resizing the frame uselessly
        return;
    }

    // TRUE in wParam means to create at least as many rows, FALSE -
    // at most as many
    RECT rect;
    ::SendMessage(GetHwnd(), TB_SETROWS,
                  MAKEWPARAM(nRows, !(GetWindowStyle() & wxTB_VERTICAL)),
                  (LPARAM) &rect);

    m_maxRows = nRows;

    UpdateSize();
}

// The button size is bigger than the bitmap size
wxSize wxToolBar::GetToolSize() const
{
    // TB_GETBUTTONSIZE is supported from version 4.70
#if defined(_WIN32_IE) && (_WIN32_IE >= 0x300 ) \
    && !( defined(__GNUWIN32__) && !wxCHECK_W32API_VERSION( 1, 0 ) ) \
    && !defined (__DIGITALMARS__)
    if ( wxApp::GetComCtl32Version() >= 470 )
    {
        DWORD dw = ::SendMessage(GetHwnd(), TB_GETBUTTONSIZE, 0, 0);

        return wxSize(LOWORD(dw), HIWORD(dw));
    }
    else
#endif // comctl32.dll 4.70+
    {
        // defaults
        return wxSize(m_defaultWidth + 8, m_defaultHeight + 7);
    }
}

static
wxToolBarToolBase *GetItemSkippingDummySpacers(const wxToolBarToolsList& tools,
                                               size_t index )
{
    wxToolBarToolsList::compatibility_iterator current = tools.GetFirst();

    for ( ; current ; current = current->GetNext() )
    {
        if ( index == 0 )
            return current->GetData();

        wxToolBarTool *tool = (wxToolBarTool *)current->GetData();
        size_t separators = tool->GetSeparatorsCount();

        // if it is a normal button, sepcount == 0, so skip 1 item (the button)
        // otherwise, skip as many items as the separator count, plus the
        // control itself
        index -= separators ? separators + 1 : 1;
    }

    return 0;
}

wxToolBarToolBase *wxToolBar::FindToolForPosition(wxCoord x, wxCoord y) const
{
    POINT pt;
    pt.x = x;
    pt.y = y;
    int index = (int)::SendMessage(GetHwnd(), TB_HITTEST, 0, (LPARAM)&pt);
    // MBN: when the point ( x, y ) is close to the toolbar border
    //      TB_HITTEST returns m_nButtons ( not -1 )
    if ( index < 0 || (size_t)index >= m_nButtons )
    {
        // it's a separator or there is no tool at all there
        return (wxToolBarToolBase *)NULL;
    }

    // if comctl32 version < 4.71 wxToolBar95 adds dummy spacers
#if defined(_WIN32_IE) && (_WIN32_IE >= 0x400 )
    if ( wxApp::GetComCtl32Version() >= 471 )
    {
        return m_tools.Item((size_t)index)->GetData();
    }
    else
#endif
    {
        return GetItemSkippingDummySpacers( m_tools, (size_t) index );
    }
}

void wxToolBar::UpdateSize()
{
    // the toolbar size changed
    ::SendMessage(GetHwnd(), TB_AUTOSIZE, 0, 0);

    // we must also refresh the frame after the toolbar size (possibly) changed
    wxFrame *frame = wxDynamicCast(GetParent(), wxFrame);
    if ( frame )
    {
        frame->SendSizeEvent();
    }
}

// ----------------------------------------------------------------------------
// toolbar styles
// ---------------------------------------------------------------------------

void wxToolBar::SetWindowStyleFlag(long style)
{
    // the style bits whose changes force us to recreate the toolbar
    static const long MASK_NEEDS_RECREATE = wxTB_TEXT | wxTB_NOICONS;

    const long styleOld = GetWindowStyle();

    wxToolBarBase::SetWindowStyleFlag(style);

    // don't recreate an empty toolbar: not only this is unnecessary, but it is
    // also fatal as we'd then try to recreate the toolbar when it's just being
    // created
    if ( GetToolsCount() &&
            (style & MASK_NEEDS_RECREATE) != (styleOld & MASK_NEEDS_RECREATE) )
    {
        // to remove the text labels, simply re-realizing the toolbar is enough
        // but I don't know of any way to add the text to an existing toolbar
        // other than by recreating it entirely
        Recreate();
    }
}

// ----------------------------------------------------------------------------
// tool state
// ----------------------------------------------------------------------------

void wxToolBar::DoEnableTool(wxToolBarToolBase *tool, bool enable)
{
    ::SendMessage(GetHwnd(), TB_ENABLEBUTTON,
                  (WPARAM)tool->GetId(), (LPARAM)MAKELONG(enable, 0));
}

void wxToolBar::DoToggleTool(wxToolBarToolBase *tool, bool toggle)
{
    ::SendMessage(GetHwnd(), TB_CHECKBUTTON,
                  (WPARAM)tool->GetId(), (LPARAM)MAKELONG(toggle, 0));
}

void wxToolBar::DoSetToggle(wxToolBarToolBase *WXUNUSED(tool), bool WXUNUSED(toggle))
{
    // VZ: AFAIK, the button has to be created either with TBSTYLE_CHECK or
    //     without, so we really need to delete the button and recreate it here
    wxFAIL_MSG( _T("not implemented") );
}

// ----------------------------------------------------------------------------
// event handlers
// ----------------------------------------------------------------------------

// Responds to colour changes, and passes event on to children.
void wxToolBar::OnSysColourChanged(wxSysColourChangedEvent& event)
{
    wxRGBToColour(m_backgroundColour, ::GetSysColor(COLOR_BTNFACE));

    // Remap the buttons
    Realize();

    // Relayout the toolbar
    int nrows = m_maxRows;
    m_maxRows = 0;      // otherwise SetRows() wouldn't do anything
    SetRows(nrows);

    Refresh();

    // let the event propagate further
    event.Skip();
}

void wxToolBar::OnMouseEvent(wxMouseEvent& event)
{
    if (event.Leaving() && m_pInTool)
    {
        OnMouseEnter( -1 );
        event.Skip();
        return;
    }

    if ( event.RightDown() )
    {
        // find the tool under the mouse
        wxCoord x,y;
        event.GetPosition(&x, &y);

        wxToolBarToolBase *tool = FindToolForPosition(x, y);
        OnRightClick(tool ? tool->GetId() : -1, x, y);
    }
    else
    {
        event.Skip();
    }
}

// This handler is required to allow the toolbar to be set to a non-default
// colour: for example, when it must blend in with a notebook page.
void wxToolBar::OnEraseBackground(wxEraseEvent& event)
{
    wxColour bgCol = GetBackgroundColour();
    if (!bgCol.Ok())
    {
        event.Skip();
        return;
    }

    // notice that this 'dumb' implementation may cause flicker for some of the
    // controls in which case they should intercept wxEraseEvent and process it
    // themselves somehow

    RECT rect;
    ::GetClientRect(GetHwnd(), &rect);

    HBRUSH hBrush = ::CreateSolidBrush(wxColourToRGB(bgCol));

    HDC hdc = GetHdcOf((*event.GetDC()));

#ifndef __WXWINCE__
    int mode = ::SetMapMode(hdc, MM_TEXT);
#endif

    ::FillRect(hdc, &rect, hBrush);
    ::DeleteObject(hBrush);

#ifndef __WXWINCE__
    ::SetMapMode(hdc, mode);
#endif
}

bool wxToolBar::HandleSize(WXWPARAM WXUNUSED(wParam), WXLPARAM lParam)
{
    // calculate our minor dimension ourselves - we're confusing the standard
    // logic (TB_AUTOSIZE) with our horizontal toolbars and other hacks
    RECT r;
    if ( ::SendMessage(GetHwnd(), TB_GETITEMRECT, 0, (LPARAM)&r) )
    {
        int w, h;

        if ( GetWindowStyle() & wxTB_VERTICAL )
        {
            w = r.right - r.left;
            if ( m_maxRows )
            {
                w *= (m_nButtons + m_maxRows - 1)/m_maxRows;
            }
            h = HIWORD(lParam);
        }
        else
        {
            w = LOWORD(lParam);
            if (HasFlag( wxTB_FLAT ))
                h = r.bottom - r.top - 3;
            else
                h = r.bottom - r.top;
            if ( m_maxRows )
            {
                // FIXME: hardcoded separator line height...
                h += HasFlag(wxTB_NODIVIDER) ? 4 : 6;
                h *= m_maxRows;
            }
        }

        if ( MAKELPARAM(w, h) != lParam )
        {
            // size really changed
            SetSize(w, h);
        }

        // message processed
        return true;
    }

    return false;
}

bool wxToolBar::HandlePaint(WXWPARAM wParam, WXLPARAM lParam)
{
    // erase any dummy separators which we used for aligning the controls if
    // any here

    // first of all, do we have any controls at all?
    wxToolBarToolsList::compatibility_iterator node;
    for ( node = m_tools.GetFirst(); node; node = node->GetNext() )
    {
        if ( node->GetData()->IsControl() )
            break;
    }

    if ( !node )
    {
        // no controls, nothing to erase
        return false;
    }

    // prepare the DC on which we'll be drawing
    wxClientDC dc(this);
    dc.SetBrush(wxBrush(GetBackgroundColour(), wxSOLID));
    dc.SetPen(*wxTRANSPARENT_PEN);

    RECT r;
    if ( !::GetUpdateRect(GetHwnd(), &r, FALSE) )
    {
        // nothing to redraw anyhow
        return false;
    }

    wxRect rectUpdate;
    wxCopyRECTToRect(r, rectUpdate);

    dc.SetClippingRegion(rectUpdate);

    // draw the toolbar tools, separators &c normally
    wxControl::MSWWindowProc(WM_PAINT, wParam, lParam);

    // for each control in the toolbar find all the separators intersecting it
    // and erase them
    //
    // NB: this is really the only way to do it as we don't know if a separator
    //     corresponds to a control (i.e. is a dummy one) or a real one
    //     otherwise
    for ( node = m_tools.GetFirst(); node; node = node->GetNext() )
    {
        wxToolBarToolBase *tool = node->GetData();
        if ( tool->IsControl() )
        {
            // get the control rect in our client coords
            wxControl *control = tool->GetControl();
            wxRect rectCtrl = control->GetRect();

            // iterate over all buttons
            TBBUTTON tbb;
            int count = ::SendMessage(GetHwnd(), TB_BUTTONCOUNT, 0, 0);
            for ( int n = 0; n < count; n++ )
            {
                // is it a separator?
                if ( !::SendMessage(GetHwnd(), TB_GETBUTTON,
                                    n, (LPARAM)&tbb) )
                {
                    wxLogDebug(_T("TB_GETBUTTON failed?"));

                    continue;
                }

                if ( tbb.fsStyle != TBSTYLE_SEP )
                    continue;

                // get the bounding rect of the separator
                RECT r;
                if ( !::SendMessage(GetHwnd(), TB_GETITEMRECT,
                                    n, (LPARAM)&r) )
                {
                    wxLogDebug(_T("TB_GETITEMRECT failed?"));

                    continue;
                }

                // does it intersect the control?
                wxRect rectItem;
                wxCopyRECTToRect(r, rectItem);
                if ( rectCtrl.Intersects(rectItem) )
                {
                    // yes, do erase it!
                    dc.DrawRectangle(rectItem);

                    // Necessary in case we use a no-paint-on-size
                    // style in the parent: the controls can disappear
                    control->Refresh(false);
                }
            }
        }
    }

    return true;
}

void wxToolBar::HandleMouseMove(WXWPARAM WXUNUSED(wParam), WXLPARAM lParam)
{
    wxCoord x = GET_X_LPARAM(lParam),
            y = GET_Y_LPARAM(lParam);
    wxToolBarToolBase* tool = FindToolForPosition( x, y );

    // cursor left current tool
    if( tool != m_pInTool && !tool )
    {
        m_pInTool = 0;
        OnMouseEnter( -1 );
    }

    // cursor entered a tool
    if( tool != m_pInTool && tool )
    {
        m_pInTool = tool;
        OnMouseEnter( tool->GetId() );
    }
}

WXLRESULT wxToolBar::MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam)
{
    switch ( nMsg )
    {
        case WM_MOUSEMOVE:
            // we don't handle mouse moves, so always pass the message to
            // wxControl::MSWWindowProc (HandleMouseMove just calls OnMouseEnter)
            HandleMouseMove(wParam, lParam);
            break;

        case WM_SIZE:
            if ( HandleSize(wParam, lParam) )
                return 0;
            break;

#ifndef __WXWINCE__
        case WM_PAINT:
            if ( HandlePaint(wParam, lParam) )
                return 0;
#endif
    }

    return wxControl::MSWWindowProc(nMsg, wParam, lParam);
}

// ----------------------------------------------------------------------------
// private functions
// ----------------------------------------------------------------------------

WXHBITMAP wxToolBar::MapBitmap(WXHBITMAP bitmap, int width, int height)
{
    MemoryHDC hdcMem;

    if ( !hdcMem )
    {
        wxLogLastError(_T("CreateCompatibleDC"));

        return bitmap;
    }

    SelectInHDC bmpInHDC(hdcMem, (HBITMAP)bitmap);

    if ( !bmpInHDC )
    {
        wxLogLastError(_T("SelectObject"));

        return bitmap;
    }

    wxCOLORMAP *cmap = wxGetStdColourMap();

    for ( int i = 0; i < width; i++ )
    {
        for ( int j = 0; j < height; j++ )
        {
            COLORREF pixel = ::GetPixel(hdcMem, i, j);

            for ( size_t k = 0; k < wxSTD_COL_MAX; k++ )
            {
                COLORREF col = cmap[k].from;
                if ( abs(GetRValue(pixel) - GetRValue(col)) < 10 &&
                     abs(GetGValue(pixel) - GetGValue(col)) < 10 &&
                     abs(GetBValue(pixel) - GetBValue(col)) < 10 )
                {
                    ::SetPixel(hdcMem, i, j, cmap[k].to);
                    break;
                }
            }
        }
    }

    return bitmap;

    // VZ: I leave here my attempts to map the bitmap to the system colours
    //     faster by using BitBlt() even though it's broken currently - but
    //     maybe someone else can finish it? It should be faster than iterating
    //     over all pixels...
#if 0
    MemoryHDC hdcMask, hdcDst;
    if ( !hdcMask || !hdcDst )
    {
        wxLogLastError(_T("CreateCompatibleDC"));

        return bitmap;
    }

    // create the target bitmap
    HBITMAP hbmpDst = ::CreateCompatibleBitmap(hdcDst, width, height);
    if ( !hbmpDst )
    {
        wxLogLastError(_T("CreateCompatibleBitmap"));

        return bitmap;
    }

    // create the monochrome mask bitmap
    HBITMAP hbmpMask = ::CreateBitmap(width, height, 1, 1, 0);
    if ( !hbmpMask )
    {
        wxLogLastError(_T("CreateBitmap(mono)"));

        ::DeleteObject(hbmpDst);

        return bitmap;
    }

    SelectInHDC bmpInDst(hdcDst, hbmpDst),
                bmpInMask(hdcMask, hbmpMask);

    // for each colour:
    for ( n = 0; n < NUM_OF_MAPPED_COLOURS; n++ )
    {
        // create the mask for this colour
        ::SetBkColor(hdcMem, ColorMap[n].from);
        ::BitBlt(hdcMask, 0, 0, width, height, hdcMem, 0, 0, SRCCOPY);

        // replace this colour with the target one in the dst bitmap
        HBRUSH hbr = ::CreateSolidBrush(ColorMap[n].to);
        HGDIOBJ hbrOld = ::SelectObject(hdcDst, hbr);

        ::MaskBlt(hdcDst, 0, 0, width, height,
                  hdcMem, 0, 0,
                  hbmpMask, 0, 0,
                  MAKEROP4(PATCOPY, SRCCOPY));

        (void)::SelectObject(hdcDst, hbrOld);
        ::DeleteObject(hbr);
    }

    ::DeleteObject((HBITMAP)bitmap);

    return (WXHBITMAP)hbmpDst;
#endif // 0
}

#endif // wxUSE_TOOLBAR && Win95

