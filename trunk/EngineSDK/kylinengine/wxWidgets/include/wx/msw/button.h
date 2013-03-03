/////////////////////////////////////////////////////////////////////////////
// Name:        wx/msw/button.h
// Purpose:     wxButton class
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id: button.h,v 1.28 2005/04/10 15:22:56 VZ Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_BUTTON_H_
#define _WX_BUTTON_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "button.h"
#endif

// ----------------------------------------------------------------------------
// Pushbutton
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxButton : public wxButtonBase
{
public:
    wxButton() { }
    wxButton(wxWindow *parent,
             wxWindowID id,
             const wxString& label = wxEmptyString,
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize,
             long style = 0,
             const wxValidator& validator = wxDefaultValidator,
             const wxString& name = wxButtonNameStr)
    {
        Create(parent, id, label, pos, size, style, validator, name);
    }

    bool Create(wxWindow *parent,
                wxWindowID id,
                const wxString& label = wxEmptyString,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = wxButtonNameStr);

    virtual ~wxButton();

    virtual void SetDefault();

    // implementation from now on
    virtual void Command(wxCommandEvent& event);
    virtual WXLRESULT MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam);
    virtual bool MSWCommand(WXUINT param, WXWORD id);

    // coloured buttons support
    virtual bool SetBackgroundColour(const wxColour &colour);
    virtual bool SetForegroundColour(const wxColour &colour);

    virtual bool MSWOnDraw(WXDRAWITEMSTRUCT *item);

private:
    void MakeOwnerDrawn();

protected:
    // send a notification event, return true if processed
    bool SendClickEvent();

    // default button handling
    void SetTmpDefault();
    void UnsetTmpDefault();

    // set or unset BS_DEFPUSHBUTTON style
    static void SetDefaultStyle(wxButton *btn, bool on);

    // usually overridden base class virtuals
    virtual wxSize DoGetBestSize() const;
    virtual WXDWORD MSWGetStyle(long style, WXDWORD *exstyle) const ;

private:
    DECLARE_DYNAMIC_CLASS_NO_COPY(wxButton)
};

#endif
    // _WX_BUTTON_H_
