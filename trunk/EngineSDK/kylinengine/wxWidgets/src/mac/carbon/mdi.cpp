/////////////////////////////////////////////////////////////////////////////
// Name:        mdi.cpp
// Purpose:     MDI classes
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// RCS-ID:      $Id: mdi.cpp,v 1.34 2005/05/10 06:28:22 SC Exp $
// Copyright:   (c) Stefan Csomor
// Licence:       wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "mdi.h"
#endif

#include "wx/wxprec.h"

#if wxUSE_MDI

#include "wx/mdi.h"
#include "wx/menu.h"
#include "wx/settings.h"
#include "wx/log.h"

#include "wx/mac/private.h"
#include "wx/mac/uma.h"

extern wxWindowList wxModelessWindows;

IMPLEMENT_DYNAMIC_CLASS(wxMDIParentFrame, wxFrame)
IMPLEMENT_DYNAMIC_CLASS(wxMDIChildFrame, wxFrame)
IMPLEMENT_DYNAMIC_CLASS(wxMDIClientWindow, wxWindow)

BEGIN_EVENT_TABLE(wxMDIParentFrame, wxFrame)
  EVT_ACTIVATE(wxMDIParentFrame::OnActivate)
  EVT_SYS_COLOUR_CHANGED(wxMDIParentFrame::OnSysColourChanged)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(wxMDIClientWindow, wxWindow)
  EVT_SCROLL(wxMDIClientWindow::OnScroll)
END_EVENT_TABLE()

static const wxChar *TRACE_MDI = _T("mdi");

static const int IDM_WINDOWTILE  = 4001;
static const int IDM_WINDOWTILEHOR  = 4001;
static const int IDM_WINDOWCASCADE = 4002;
static const int IDM_WINDOWICONS = 4003;
static const int IDM_WINDOWNEXT = 4004;
static const int IDM_WINDOWTILEVERT = 4005;
static const int IDM_WINDOWPREV = 4006;

// This range gives a maximum of 500 MDI children. Should be enough :-)
static const int wxFIRST_MDI_CHILD = 4100;
static const int wxLAST_MDI_CHILD = 4600;

// Status border dimensions
static const int wxTHICK_LINE_BORDER = 3;

// Parent frame

wxMDIParentFrame::wxMDIParentFrame()
{
    m_clientWindow = NULL;
    m_currentChild = NULL;
    m_windowMenu = (wxMenu*) NULL;
    m_parentFrameActive = TRUE;
}

bool wxMDIParentFrame::Create(wxWindow *parent,
           wxWindowID id,
           const wxString& title,
           const wxPoint& pos,
           const wxSize& size,
           long style,
           const wxString& name)
{
    m_clientWindow = NULL;
    m_currentChild = NULL;
    
    // this style can be used to prevent a window from having the standard MDI
    // "Window" menu
    if ( style & wxFRAME_NO_WINDOW_MENU )
    {
        m_windowMenu = (wxMenu *)NULL;
        style -= wxFRAME_NO_WINDOW_MENU ;
    }
    else // normal case: we have the window menu, so construct it
    {
        m_windowMenu = new wxMenu;
        
        m_windowMenu->Append(IDM_WINDOWCASCADE, wxT("&Cascade"));
        m_windowMenu->Append(IDM_WINDOWTILEHOR, wxT("Tile &Horizontally"));
        m_windowMenu->Append(IDM_WINDOWTILEVERT, wxT("Tile &Vertically"));
        m_windowMenu->AppendSeparator();
        m_windowMenu->Append(IDM_WINDOWICONS, wxT("&Arrange Icons"));
        m_windowMenu->Append(IDM_WINDOWNEXT, wxT("&Next"));
    }
    
    wxFrame::Create( parent , id , title , pos , size , style , name ) ;
    m_parentFrameActive = TRUE;
    
    OnCreateClient();
    
    return TRUE;
}

wxMDIParentFrame::~wxMDIParentFrame()
{
    DestroyChildren();
    // already deleted by DestroyChildren()
    m_clientWindow = NULL ;
    
    delete m_windowMenu;
}


void wxMDIParentFrame::SetMenuBar(wxMenuBar *menu_bar)
{
    wxFrame::SetMenuBar( menu_bar ) ;
}

void wxMDIParentFrame::GetRectForTopLevelChildren(int *x, int *y, int *w, int *h)
{
    if(x)
        *x = 0;
    if(y)
        *y = 0;
    wxDisplaySize(w,h);
}

void wxMDIParentFrame::MacActivate(long timestamp, bool activating)
{
    wxLogTrace(TRACE_MDI, wxT("MDI PARENT=%p MacActivate(0x%08lx,%s)"),this,timestamp,activating?wxT("ACTIV"):wxT("deact"));
    if(activating)
    {
        if(s_macDeactivateWindow && s_macDeactivateWindow->GetParent()==this)
        {
            wxLogTrace(TRACE_MDI, wxT("child had been scheduled for deactivation, rehighlighting"));
            UMAHighlightAndActivateWindow((WindowRef)s_macDeactivateWindow->MacGetWindowRef(), true);
            wxLogTrace(TRACE_MDI, wxT("done highliting child"));
            s_macDeactivateWindow = NULL;
        }
        else if(s_macDeactivateWindow == this)
        {
            wxLogTrace(TRACE_MDI, wxT("Avoided deactivation/activation of this=%p"), this);
            s_macDeactivateWindow = NULL;
        }
        else // window to deactivate is NULL or is not us or one of our kids
        {
            // activate kid instead
            if(m_currentChild)
                m_currentChild->MacActivate(timestamp,activating);
            else
                wxFrame::MacActivate(timestamp,activating);
        }
    }
    else
    {
        // We were scheduled for deactivation, and now we do it.
        if(s_macDeactivateWindow==this)
        {
            s_macDeactivateWindow = NULL;
            if(m_currentChild)
                m_currentChild->MacActivate(timestamp,activating);
            wxFrame::MacActivate(timestamp,activating);
        }
        else // schedule ourselves for deactivation
        {
            if(s_macDeactivateWindow)
                wxLogTrace(TRACE_MDI, wxT("window=%p SHOULD have been deactivated, oh well!"),s_macDeactivateWindow);
            wxLogTrace(TRACE_MDI, wxT("Scheduling delayed MDI Parent deactivation"));
            s_macDeactivateWindow = this;
        }
    }
}

void wxMDIParentFrame::OnActivate(wxActivateEvent& event)
{
    event.Skip();
}

// Returns the active MDI child window
wxMDIChildFrame *wxMDIParentFrame::GetActiveChild() const
{
    return m_currentChild ;
}

// Create the client window class (don't Create the window,
// just return a new class)
wxMDIClientWindow *wxMDIParentFrame::OnCreateClient()
{
    m_clientWindow = new wxMDIClientWindow( this );
    return m_clientWindow;
}

// Responds to colour changes, and passes event on to children.
void wxMDIParentFrame::OnSysColourChanged(wxSysColourChangedEvent& event)
{
    // TODO
    
    // Propagate the event to the non-top-level children
    wxFrame::OnSysColourChanged(event);
}

// MDI operations
void wxMDIParentFrame::Cascade()
{
    // TODO
}

void wxMDIParentFrame::Tile(wxOrientation WXUNUSED(orient))
{
    // TODO
}

void wxMDIParentFrame::ArrangeIcons()
{
    // TODO
}

void wxMDIParentFrame::ActivateNext()
{
    // TODO
}

void wxMDIParentFrame::ActivatePrevious()
{
    // TODO
}

bool wxMDIParentFrame::Show( bool show )
{
    // don't really show the MDI frame unless it has any children other than
    // MDI children as it is pretty useless in this case

    if ( show )
    {
        // TODO: check for other children
        if(!GetToolBar())
            Move(-10000, -10000);
    }

    if ( !wxFrame::Show(show) )
        return false;

    return true;
}

// Child frame

wxMDIChildFrame::wxMDIChildFrame()
{
    Init() ;
}
void wxMDIChildFrame::Init()
{
}

bool wxMDIChildFrame::Create(wxMDIParentFrame *parent,
                             wxWindowID id,
                             const wxString& title,
                             const wxPoint& pos,
                             const wxSize& size,
                             long style,
                             const wxString& name)
{
    SetName(name);
    
    if ( id > -1 )
        m_windowId = id;
    else
        m_windowId = (int)NewControlId();
    
    if (parent) parent->AddChild(this);
    
    MacCreateRealWindow( title, pos , size , MacRemoveBordersFromStyle(style) , name ) ;
    
    SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_APPWORKSPACE));

    wxModelessWindows.Append(this);
    return FALSE;
}

wxMDIChildFrame::~wxMDIChildFrame()
{
    wxMDIParentFrame *mdiparent = wxDynamicCast(m_parent, wxMDIParentFrame);
    wxASSERT(mdiparent);
    if(mdiparent->m_currentChild == this)
        mdiparent->m_currentChild = NULL;
    DestroyChildren();
}

void wxMDIChildFrame::SetMenuBar(wxMenuBar *menu_bar)
{
    return wxFrame::SetMenuBar( menu_bar ) ;
}

void wxMDIChildFrame::MacActivate(long timestamp, bool activating)
{
    wxLogTrace(TRACE_MDI, wxT("MDI child=%p  MacActivate(0x%08lx,%s)"),this,timestamp,activating?wxT("ACTIV"):wxT("deact"));
    wxMDIParentFrame *mdiparent = wxDynamicCast(m_parent, wxMDIParentFrame);
    wxASSERT(mdiparent);
    if(activating)
    {
        if(s_macDeactivateWindow == m_parent)
        {
            wxLogTrace(TRACE_MDI, wxT("parent had been scheduled for deactivation, rehighlighting"));
            UMAHighlightAndActivateWindow((WindowRef)s_macDeactivateWindow->MacGetWindowRef(), true);
            wxLogTrace(TRACE_MDI, wxT("done highliting parent"));
            s_macDeactivateWindow = NULL;
        }
        else if((mdiparent->m_currentChild==this) || !s_macDeactivateWindow)
            mdiparent->wxFrame::MacActivate(timestamp,activating);
        
        if(mdiparent->m_currentChild && mdiparent->m_currentChild!=this)
            mdiparent->m_currentChild->wxFrame::MacActivate(timestamp,false);
        mdiparent->m_currentChild = this;

        if(s_macDeactivateWindow==this)
        {
            wxLogTrace(TRACE_MDI, wxT("Avoided deactivation/activation of this=%p"),this);
            s_macDeactivateWindow=NULL;
        }
        else
            wxFrame::MacActivate(timestamp, activating);
    }
    else
    {
        // We were scheduled for deactivation, and now we do it.
        if(s_macDeactivateWindow==this)
        {
            s_macDeactivateWindow = NULL;
            wxFrame::MacActivate(timestamp,activating);
            if(mdiparent->m_currentChild==this)
                mdiparent->wxFrame::MacActivate(timestamp,activating);
        }
        else // schedule ourselves for deactivation
        {
            if(s_macDeactivateWindow)
                wxLogTrace(TRACE_MDI, wxT("window=%p SHOULD have been deactivated, oh well!"),s_macDeactivateWindow);
            wxLogTrace(TRACE_MDI, wxT("Scheduling delayed deactivation"));
            s_macDeactivateWindow = this;
        }
    }
}

// MDI operations
void wxMDIChildFrame::Maximize()
{
    wxFrame::Maximize() ;
}

void wxMDIChildFrame::Restore()
{
    wxFrame::Restore() ;
}

void wxMDIChildFrame::Activate()
{
}

//-----------------------------------------------------------------------------
// wxMDIClientWindow
//-----------------------------------------------------------------------------

wxMDIClientWindow::wxMDIClientWindow()
{
}

wxMDIClientWindow::~wxMDIClientWindow()
{
    DestroyChildren();
}

bool wxMDIClientWindow::CreateClient(wxMDIParentFrame *parent, long style)
{
    if ( !wxWindow::Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, style))
        return FALSE;
    
    wxModelessWindows.Append(this);
    return TRUE;
}

// Get size *available for subwindows* i.e. excluding menu bar.
void wxMDIClientWindow::DoGetClientSize(int *x, int *y) const
{
    wxDisplaySize( x , y ) ;
}

// Explicitly call default scroll behaviour
void wxMDIClientWindow::OnScroll(wxScrollEvent& event)
{
}

#endif

