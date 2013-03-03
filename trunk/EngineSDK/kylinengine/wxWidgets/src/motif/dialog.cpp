/////////////////////////////////////////////////////////////////////////////
// Name:        dialog.cpp
// Purpose:     wxDialog class
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id: dialog.cpp,v 1.52 2005/02/06 17:38:30 MBN Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "dialog.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __VMS
#define XtDisplay XTDISPLAY
#define XtWindow XTWINDOW
#define XtParent XTPARENT
#define XtScreen XTSCREEN
#endif

#include "wx/dialog.h"
#include "wx/utils.h"
#include "wx/app.h"
#include "wx/settings.h"
#include "wx/evtloop.h"

#ifdef __VMS__
#pragma message disable nosimpint
#endif
#include <Xm/Xm.h>

#include <X11/Shell.h>
#if XmVersion >= 1002
#include <Xm/XmAll.h>
#endif
#include <Xm/MwmUtil.h>
#include <Xm/Label.h>
#include <Xm/BulletinB.h>
#include <Xm/Frame.h>
#include <Xm/Text.h>
#include <Xm/DialogS.h>
#include <Xm/FileSB.h>
#include <Xm/RowColumn.h>
#include <Xm/LabelG.h>
#include <Xm/AtomMgr.h>
#if   XmVersion > 1000
#include <Xm/Protocols.h>
#endif
#ifdef __VMS__
#pragma message enable nosimpint
#endif

#include "wx/motif/private.h"

// A stack of modal_showing flags, since we can't rely
// on accessing wxDialog::m_modalShowing within
// wxDialog::Show in case a callback has deleted the wxDialog.
// static wxList wxModalShowingStack;

// Lists to keep track of windows, so we can disable/enable them
// for modal dialogs
wxList wxModalDialogs;
extern wxList wxModelessWindows;  // Frames and modeless dialogs
extern wxList wxPendingDelete;

#define wxUSE_INVISIBLE_RESIZE 1

IMPLEMENT_DYNAMIC_CLASS(wxDialog, wxTopLevelWindow)

BEGIN_EVENT_TABLE(wxDialog, wxTopLevelWindow)
  EVT_BUTTON(wxID_OK, wxDialog::OnOK)
  EVT_BUTTON(wxID_APPLY, wxDialog::OnApply)
  EVT_BUTTON(wxID_CANCEL, wxDialog::OnCancel)
  EVT_CHAR_HOOK(wxDialog::OnCharHook)
  EVT_SYS_COLOUR_CHANGED(wxDialog::OnSysColourChanged)
  EVT_CLOSE(wxDialog::OnCloseWindow)
END_EVENT_TABLE()


wxDialog::wxDialog()
{
    m_modalShowing = false;
    m_eventLoop = NULL;
    m_backgroundColour = wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE);
}

bool wxDialog::Create(wxWindow *parent, wxWindowID id,
                      const wxString& title,
                      const wxPoint& pos,
                      const wxSize& size,
                      long style,
                      const wxString& name)
{
    SetExtraStyle(GetExtraStyle() | wxTOPLEVEL_EX_DIALOG);

    if( !wxTopLevelWindow::Create( parent, id, title, pos, size, style,
                                   name ) )
        return false;

    m_modalShowing = false;
    m_eventLoop = NULL;

    m_backgroundColour = wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE);
    m_foregroundColour = *wxBLACK;

    Widget dialogShell = (Widget) m_mainWidget;
    Widget shell = XtParent(dialogShell) ;

    SetTitle( title );

    m_font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
    ChangeFont(false);

    // Can't remember what this was about... but I think it's necessary.
    if (wxUSE_INVISIBLE_RESIZE)
    {
        if (pos.x > -1)
            XtVaSetValues(dialogShell, XmNx, pos.x,
            NULL);
        if (pos.y > -1)
            XtVaSetValues(dialogShell, XmNy, pos.y,
            NULL);

        if (size.x > -1)
            XtVaSetValues(dialogShell, XmNwidth, size.x, NULL);
        if (size.y > -1)
            XtVaSetValues(dialogShell, XmNheight, size.y, NULL);
    }

    // Positioning of the dialog doesn't work properly unless the dialog
    // is managed, so we manage without mapping to the screen.
    // To show, we map the shell (actually it's parent).
    if (!wxUSE_INVISIBLE_RESIZE)
        XtVaSetValues(shell, XmNmappedWhenManaged, False, NULL);

    if (!wxUSE_INVISIBLE_RESIZE)
    {
        XtManageChild(dialogShell);
        SetSize(pos.x, pos.y, size.x, size.y);
    }
    XtAddEventHandler(dialogShell,ExposureMask,False,
        wxUniversalRepaintProc, (XtPointer) this);

    ChangeBackgroundColour();

    return true;
}

bool wxDialog::DoCreate( wxWindow* parent, wxWindowID id,
                         const wxString& title,
                         const wxPoint& pos,
                         const wxSize& size,
                         long style,
                         const wxString& name )
{
    Widget parentWidget = (Widget) 0;
    if( parent )
        parentWidget = (Widget) parent->GetTopWidget();
    if( !parent )
        parentWidget = (Widget) wxTheApp->GetTopLevelWidget();

    wxASSERT_MSG( (parentWidget != (Widget) 0),
                  "Could not find a suitable parent shell for dialog." );

    Arg args[2];
    XtSetArg (args[0], XmNdefaultPosition, False);
    XtSetArg (args[1], XmNautoUnmanage, False);
    Widget dialogShell =
        XmCreateBulletinBoardDialog( parentWidget,
                                     wxConstCast(name.c_str(), char),
                                     args, 2);
    m_mainWidget = (WXWidget) dialogShell;

    // We don't want margins, since there is enough elsewhere.
    XtVaSetValues( dialogShell,
                   XmNmarginHeight,   0,
                   XmNmarginWidth,    0,
                   XmNresizePolicy, XmRESIZE_NONE,
                   NULL ) ;

    XtTranslations ptr ;
    XtOverrideTranslations(dialogShell,
        ptr = XtParseTranslationTable("<Configure>: resize()"));
    XtFree((char *)ptr);

    XtRealizeWidget(dialogShell);

    wxAddWindowToTable( (Widget)m_mainWidget, this );

    return true;
}

void wxDialog::SetModal(bool flag)
{
#ifdef __VMS
#pragma message disable codcauunr
#endif
   if ( flag )
        m_windowStyle |= wxDIALOG_MODAL ;
    else
        if ( m_windowStyle & wxDIALOG_MODAL )
            m_windowStyle -= wxDIALOG_MODAL ;

        wxModelessWindows.DeleteObject(this);
        if (!flag)
            wxModelessWindows.Append(this);
#ifdef __VMS
#pragma message enable codcauunr
#endif
}

wxDialog::~wxDialog()
{
    m_isBeingDeleted = true;

    delete m_eventLoop;

    if (m_mainWidget)
    {
        XtRemoveEventHandler((Widget) m_mainWidget, ExposureMask, False,
                             wxUniversalRepaintProc, (XtPointer) this);
    }

    m_modalShowing = false;
    if (!wxUSE_INVISIBLE_RESIZE && m_mainWidget)
    {
        XtUnmapWidget((Widget) m_mainWidget);
    }

    PreDestroy();
    DoDestroy();
}

void wxDialog::DoDestroy()
{
    if( m_mainWidget )
    {
        wxDeleteWindowFromTable( (Widget)m_mainWidget );
        XtDestroyWidget( (Widget)m_mainWidget );
    }
}

// By default, pressing escape cancels the dialog
void wxDialog::OnCharHook(wxKeyEvent& event)
{
    if (event.m_keyCode == WXK_ESCAPE)
    {
        // Behaviour changed in 2.0: we'll send a Cancel message
        // to the dialog instead of Close.
        wxCommandEvent cancelEvent(wxEVT_COMMAND_BUTTON_CLICKED, wxID_CANCEL);
        cancelEvent.SetEventObject( this );
        GetEventHandler()->ProcessEvent(cancelEvent);

        return;
    }
    // We didn't process this event.
    event.Skip();
}

void wxDialog::DoSetSize(int x, int y, int width, int height, int sizeFlags)
{
    XtVaSetValues((Widget) m_mainWidget, XmNresizePolicy, XmRESIZE_ANY, NULL);
    wxWindow::DoSetSize(x, y, width, height, sizeFlags);
    XtVaSetValues((Widget) m_mainWidget, XmNresizePolicy, XmRESIZE_NONE, NULL);
}

void wxDialog::DoSetClientSize(int width, int height)
{
    wxWindow::SetSize(-1, -1, width, height);
}

void wxDialog::SetTitle(const wxString& title)
{
    wxTopLevelWindow::SetTitle( title );

    if( !title.empty() )
    {
        wxXmString str( title );
        XtVaSetValues( (Widget)m_mainWidget,
                       XmNtitle, title.c_str(),
                       XmNdialogTitle, str(), // Roberto Cocchi
                       XmNiconName, title.c_str(),
                       NULL );
    }
}

bool wxDialog::Show( bool show )
{
    if( !wxWindowBase::Show( show ) )
        return false;

    m_isShown = show;

    if (show)
    {
        if (!wxUSE_INVISIBLE_RESIZE)
            XtMapWidget(XtParent((Widget) m_mainWidget));
        else
            XtManageChild((Widget)m_mainWidget) ;

        XRaiseWindow( XtDisplay( (Widget)m_mainWidget ), 
                      XtWindow( (Widget)m_mainWidget) );

    }
    else
    {
        if (!wxUSE_INVISIBLE_RESIZE)
            XtUnmapWidget(XtParent((Widget) m_mainWidget));
        else
            XtUnmanageChild((Widget)m_mainWidget) ;

        XFlush(XtDisplay((Widget)m_mainWidget));
        XSync(XtDisplay((Widget)m_mainWidget), False);
    }

    return true;
}

// Shows a dialog modally, returning a return code
int wxDialog::ShowModal()
{
    m_windowStyle |= wxDIALOG_MODAL;

    Show(true);

    // after the event loop ran, the widget might already have been destroyed
    WXDisplay* display = (WXDisplay*)XtDisplay( (Widget)m_mainWidget );

    if (m_modalShowing)
        return 0;
    m_eventLoop = new wxEventLoop;

    m_modalShowing = true;
    XtAddGrab((Widget) m_mainWidget, True, False);

    m_eventLoop->Run();

    // Now process all events in case they get sent to a destroyed dialog
    wxFlushEvents( display );

    delete m_eventLoop;
    m_eventLoop = NULL;

    // TODO: is it safe to call this, if the dialog may have been deleted
    // by now? Probably only if we're using delayed deletion of dialogs.
    return GetReturnCode();
}

void wxDialog::EndModal(int retCode)
{
    if (!m_modalShowing)
        return;

    SetReturnCode(retCode);

    // Strangely, we don't seem to need this now.
    //    XtRemoveGrab((Widget) m_mainWidget);

    Show(false);

    m_modalShowing = false;
    m_eventLoop->Exit();
}

// Standard buttons
void wxDialog::OnOK(wxCommandEvent& WXUNUSED(event))
{
    if ( Validate() && TransferDataFromWindow() )
    {
        if ( IsModal() )
            EndModal(wxID_OK);
        else
        {
            SetReturnCode(wxID_OK);
            this->Show(false);
        }
    }
}

void wxDialog::OnApply(wxCommandEvent& WXUNUSED(event))
{
    if (Validate())
        TransferDataFromWindow();
    // TODO probably need to disable the Apply button until things change again
}

void wxDialog::OnCancel(wxCommandEvent& WXUNUSED(event))
{
    if ( IsModal() )
        EndModal(wxID_CANCEL);
    else
    {
        SetReturnCode(wxID_CANCEL);
        this->Show(false);
    }
}

void wxDialog::OnCloseWindow(wxCloseEvent& WXUNUSED(event))
{
    // We'll send a Cancel message by default,
    // which may close the dialog.
    // Check for looping if the Cancel event handler calls Close().

    // Note that if a cancel button and handler aren't present in the dialog,
    // nothing will happen when you close the dialog via the window manager, or
    // via Close().
    // We wouldn't want to destroy the dialog by default, since the dialog may have been
    // created on the stack.
    // However, this does mean that calling dialog->Close() won't delete the dialog
    // unless the handler for wxID_CANCEL does so. So use Destroy() if you want to be
    // sure to destroy the dialog.
    // The default OnCancel (above) simply ends a modal dialog, and hides a modeless dialog.

    static wxList closing;

    if ( closing.Member(this) )
        return;

    closing.Append(this);

    wxCommandEvent cancelEvent(wxEVT_COMMAND_BUTTON_CLICKED, wxID_CANCEL);
    cancelEvent.SetEventObject( this );
    GetEventHandler()->ProcessEvent(cancelEvent); // This may close the dialog

    closing.DeleteObject(this);
}

// Destroy the window (delayed, if a managed window)
bool wxDialog::Destroy()
{
    if (!wxPendingDelete.Member(this))
        wxPendingDelete.Append(this);
    return true;
}

void wxDialog::OnSysColourChanged(wxSysColourChangedEvent& WXUNUSED(event))
{
    SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE));
    Refresh();
}

void wxDialog::ChangeFont(bool keepOriginalSize)
{
    wxWindow::ChangeFont(keepOriginalSize);
}

void wxDialog::ChangeBackgroundColour()
{
    if (GetMainWidget())
        wxDoChangeBackgroundColour(GetMainWidget(), m_backgroundColour);
}

void wxDialog::ChangeForegroundColour()
{
    if (GetMainWidget())
        wxDoChangeForegroundColour(GetMainWidget(), m_foregroundColour);
}
