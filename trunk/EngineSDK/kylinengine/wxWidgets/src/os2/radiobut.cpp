/////////////////////////////////////////////////////////////////////////////
// Name:        radiobut.cpp
// Purpose:     wxRadioButton
// Author:      David Webster
// Modified by:
// Created:     10/12/99
// RCS-ID:      $Id: radiobut.cpp,v 1.23 2005/05/14 17:34:11 SN Exp $
// Copyright:   (c) David Webster
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <stdio.h>
#include "wx/setup.h"
#include "wx/radiobut.h"
#include "wx/brush.h"
#include "wx/dcscreen.h"
#include "wx/settings.h"
#endif

#include "wx/os2/private.h"

IMPLEMENT_DYNAMIC_CLASS(wxRadioButton, wxControl)

extern void  wxAssociateWinWithHandle( HWND         hWnd
                                      ,wxWindowOS2* pWin
                                     );

void wxRadioButton::Init()
{
    m_bFocusJustSet = FALSE;
} // end of wxRadioButton::Init

void wxRadioButton::Command (
  wxCommandEvent&                   rEvent
)
{
    SetValue ((rEvent.GetInt() != 0) );
    ProcessCommand (rEvent);
} // end of wxRadioButton::Command

bool wxRadioButton::Create(
  wxWindow*                         pParent
, wxWindowID                        vId
, const wxString&                   rsLabel
, const wxPoint&                    rPos
, const wxSize&                     rSize
, long                              lStyle
, const wxValidator&                rValidator
, const wxString&                   rsName
)
{
    if ( !CreateControl( pParent
                        ,vId
                        ,rPos
                        ,rSize
                        ,lStyle
                        ,rValidator
                        ,rsName))
        return FALSE;

    long                            lSstyle = WS_TABSTOP;

    if (HasFlag(wxRB_GROUP))
        lSstyle |= WS_GROUP;

    //
    // wxRB_SINGLE is a temporary workaround for the following problem: if you
    // have 2 radiobuttons in the same group but which are not consecutive in
    // the dialog, Windows can enter an infinite loop! The simplest way to
    // reproduce it is to create radio button, then a panel and then another
    // radio button: then checking the last button hangs the app.
    //
    // Ideally, we'd detect (and avoid) such situation automatically but for
    // now, as I don't know how to do it, just allow the user to create
    // BS_RADIOBUTTON buttons for such situations.
    //
    lSstyle |= HasFlag(wxRB_SINGLE) ? BS_RADIOBUTTON : BS_AUTORADIOBUTTON;

    if (HasFlag(wxCLIP_SIBLINGS))
        lSstyle |= WS_CLIPSIBLINGS;

    if (!OS2CreateControl( _T("BUTTON")
                          ,lSstyle
                          ,rPos
                          ,rSize
                          ,rsLabel
                          ,0
                         ))
        return FALSE;

    wxAssociateWinWithHandle(m_hWnd, this);
    if (HasFlag(wxRB_GROUP))
        SetValue(TRUE);

    SetFont(*wxSMALL_FONT);
    SetSize( rPos.x
            ,rPos.y
            ,rSize.x
            ,rSize.y
           );
    return TRUE;
} // end of wxRadioButton::Create

wxSize wxRadioButton::DoGetBestSize() const
{
    static int                      snRadioSize = 0;

    if (!snRadioSize)
    {
        wxScreenDC                  vDC;

        vDC.SetFont(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT));
        snRadioSize = vDC.GetCharHeight();
    }

    wxString                        sStr = GetLabel();
    int                             nRadioWidth;
    int                             nRadioHeight;

    if (!sStr.empty())
    {
        GetTextExtent( sStr
                      ,&nRadioWidth
                      ,&nRadioHeight
                     );
        nRadioWidth += snRadioSize + GetCharWidth();
        if (nRadioHeight < snRadioSize)
            nRadioHeight = snRadioSize;
    }
    else
    {
        nRadioWidth  = snRadioSize;
        nRadioHeight = snRadioSize;
    }
    return wxSize( nRadioWidth
                  ,nRadioHeight
                 );
} // end of wxRadioButton::DoGetBestSize

//
// Get single selection, for single choice list items
//
bool wxRadioButton::GetValue() const
{
    return((::WinSendMsg((HWND) GetHWND(), BM_QUERYCHECK, (MPARAM)0L, (MPARAM)0L) != 0));
} // end of wxRadioButton::GetValue

bool wxRadioButton::OS2Command(
  WXUINT                            wParam
, WXWORD                            wId
)
{
    if (wParam != BN_CLICKED)
        return FALSE;

    if (m_bFocusJustSet)
    {
        //
        // See above: we want to ignore this event
        //
        m_bFocusJustSet = FALSE;
    }
    else
    {
        bool                        bIsChecked = GetValue();

        if (HasFlag(wxRB_SINGLE))
        {
            //
            // When we use a "manual" radio button, we have to check the button
            // ourselves -- but it's reset to unchecked state by the user code
            // (presumably when another button is pressed)
            //
            if (!bIsChecked )
                SetValue(TRUE);
        }
        wxCommandEvent              rEvent( wxEVT_COMMAND_RADIOBUTTON_SELECTED
                                           ,m_windowId
                                          );

        rEvent.SetEventObject(this);
        ProcessCommand(rEvent);
    }
    return TRUE;
} // end of wxRadioButton::OS2Command

void wxRadioButton::SetFocus()
{
    // when the radio button receives a WM_SETFOCUS message it generates a
    // BN_CLICKED which is totally unexpected and leads to catastrophic results
    // if you pop up a dialog from the radio button event handler as, when the
    // dialog is dismissed, the focus is returned to the radio button which
    // generates BN_CLICKED which leads to showing another dialog and so on
    // without end!
    //
    // to avoid this, we drop the pseudo BN_CLICKED events generated when the
    // button gains focus
    m_bFocusJustSet = TRUE;

    wxControl::SetFocus();
}

void wxRadioButton::SetLabel(
  const wxString&                   rsLabel
)
{
    wxString                        sLabel = ::wxPMTextToLabel(rsLabel);
    ::WinSetWindowText((HWND)GetHWND(), (const char *)sLabel.c_str());
} // end of wxRadioButton::SetLabel

void wxRadioButton::SetValue(
  bool                              bValue
)
{
    ::WinSendMsg((HWND)GetHWND(), BM_SETCHECK, (MPARAM)bValue, (MPARAM)0);
    if (bValue)
    {
        const wxWindowList&         rSiblings = GetParent()->GetChildren();
        wxWindowList::compatibility_iterator nodeThis = rSiblings.Find(this);

        wxCHECK_RET(nodeThis, _T("radio button not a child of its parent?"));

        //
        // If it's not the first item of the group ...
        //
        if ( !HasFlag(wxRB_GROUP) )
        {
            //
            // ...turn off all radio buttons before this one
            //
            for ( wxWindowList::compatibility_iterator nodeBefore = nodeThis->GetPrevious();
                  nodeBefore;
                  nodeBefore = nodeBefore->GetPrevious() )
            {
                wxRadioButton*      pBtn = wxDynamicCast( nodeBefore->GetData()
                                                         ,wxRadioButton
                                                        );
                if (!pBtn)
                {
                    //
                    // The radio buttons in a group must be consecutive, so there
                    // are no more of them
                    //
                    break;
                }
                pBtn->SetValue(FALSE);
                if (pBtn->HasFlag(wxRB_GROUP))
                {
                    //
                    // Even if there are other radio buttons before this one,
                    // they're not in the same group with us
                    //
                    break;
                }
            }
        }

        //
        // ... and all after this one
        //
        for (wxWindowList::compatibility_iterator nodeAfter = nodeThis->GetNext();
             nodeAfter;
             nodeAfter = nodeAfter->GetNext())
        {
            wxRadioButton*          pBtn = wxDynamicCast( nodeAfter->GetData()
                                                         ,wxRadioButton
                                                        );

            if (!pBtn || pBtn->HasFlag(wxRB_GROUP) )
            {
                //
                // No more buttons or the first button of the next group
                //
                break;
            }
            pBtn->SetValue(FALSE);
        }
    }
} // end of wxRadioButton::SetValue

MRESULT wxRadioButton::OS2WindowProc(
  WXUINT                            uMsg
, WXWPARAM                          wParam
, WXLPARAM                          lParam
)
{
    if (uMsg == WM_SETFOCUS)
    {
        m_bFocusJustSet = TRUE;

        MRESULT                     mRc = wxControl::OS2WindowProc( uMsg
                                                                   ,wParam
                                                                   ,lParam
                                                                  );

        m_bFocusJustSet = FALSE;
        return mRc;
    }
    return wxControl::OS2WindowProc( uMsg
                                    ,wParam
                                    ,lParam
                                   );
} // end of wxRadioButton::OS2WindowProc
