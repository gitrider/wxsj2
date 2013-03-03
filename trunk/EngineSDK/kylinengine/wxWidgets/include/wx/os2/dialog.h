/////////////////////////////////////////////////////////////////////////////
// Name:        dialog.h
// Purpose:     wxDialog class
// Author:      David Webster
// Modified by:
// Created:     10/14/99
// RCS-ID:      $Id: dialog.h,v 1.16 2005/04/27 01:11:58 DW Exp $
// Copyright:   (c) David Webster
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_DIALOG_H_
#define _WX_DIALOG_H_

#include "wx/panel.h"

WXDLLEXPORT_DATA(extern const wxChar*) wxDialogNameStr;

class WXDLLEXPORT wxDialogModalData;

//
// Dialog boxes
//
class WXDLLEXPORT wxDialog: public wxDialogBase
{
public:

  inline wxDialog() { Init(); }

    // full ctor
    wxDialog(wxWindow *parent, wxWindowID id,
             const wxString& title,
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize,
             long style = wxDEFAULT_DIALOG_STYLE,
             const wxString& name = wxDialogNameStr)
    {
        Init();

        (void)Create(parent, id, title, pos, size, style, name);
    }

    bool Create( wxWindow*       pParent
                ,wxWindowID      vId
                ,const wxString& rsTitle
                ,const wxPoint&  rPos = wxDefaultPosition
                ,const wxSize&   rSize = wxDefaultSize
                ,long            lStyle = wxDEFAULT_DIALOG_STYLE
                ,const wxString& rsName = wxDialogNameStr
               );
    virtual ~wxDialog();

    // return true if we're showing the dialog modally
    virtual bool IsModal() const { return m_modalData != NULL; }

    // show the dialog modally and return the value passed to EndModal()
    virtual int ShowModal();

    // may be called to terminate the dialog with the given return code
    virtual void EndModal(int retCode);

    // implementation only from now on
    // -------------------------------

    // override some base class virtuals
    virtual bool Show(bool show = true);

    //
    // Event handlers
    //
    void OnCharHook(wxKeyEvent& rEvent);
    void OnCloseWindow(wxCloseEvent& rEvent);

    //
    // Standard buttons
    //
    void     OnOK(wxCommandEvent& rEvent);
    void     OnApply(wxCommandEvent& rEvent);
    void     OnCancel(wxCommandEvent& rEvent);

    //
    // Responds to colour changes
    //
    void     OnSysColourChanged(wxSysColourChangedEvent& rEvent);

    //
    // Callbacks
    //
    virtual MRESULT OS2WindowProc( WXUINT   uMessage
                                  ,WXWPARAM wParam
                                  ,WXLPARAM lParam
                                 );
    // obsolete methods
    // ----------------

    // Constructor with a modal flag, but no window id - the old convention
    wxDEPRECATED( wxDialog( wxWindow*       pParent
                           ,const wxString& rsTitle
                           ,bool            bModal
                           ,int             nX = -1
                           ,int             nY = -1
                           ,int             nWidth = 500
                           ,int             nHeight = 500
                           ,long            lStyle = wxDEFAULT_DIALOG_STYLE
                           ,const wxString& rsName = wxDialogNameStr
                          ) );

    // just call Show() or ShowModal()
    wxDEPRECATED( void SetModal(bool bFlag) );

    // use IsModal()
    wxDEPRECATED( bool IsModalShowing() const );

protected:
    //
    // find the window to use as parent for this dialog if none has been
    // specified explicitly by the user
    //
    // may return NULL
    //
    wxWindow *FindSuitableParent() const;

    //
    // Common part of all ctors
    //
    void Init(void);

    // end either modal or modeless dialog
    void EndDialog(int rc);

private:
    wxWindow*                       m_pOldFocus;
    bool        m_endModalCalled; // allow for closing within InitDialog

    // this pointer is non-NULL only while the modal event loop is running
    wxDialogModalData *m_modalData;

    //
    // While we are showing a modal dialog we disable the other windows using
    // this object
    //
    class wxWindowDisabler*         m_pWindowDisabler;

    DECLARE_DYNAMIC_CLASS(wxDialog)
    DECLARE_EVENT_TABLE()
    DECLARE_NO_COPY_CLASS(wxDialog)
}; // end of CLASS wxDialog

#endif // _WX_DIALOG_H_

