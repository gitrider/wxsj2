///////////////////////////////////////////////////////////////////////////////
// Name:        msw/caret.h
// Purpose:     wxCaret class - the MSW implementation of wxCaret
// Author:      Vadim Zeitlin
// Modified by:
// Created:     23.05.99
// RCS-ID:      $Id: caret.h,v 1.8 2004/08/20 12:03:03 ABX Exp $
// Copyright:   (c) wxWidgets team
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_CARET_H_
#define _WX_CARET_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "caret.h"
#endif

class WXDLLEXPORT wxCaret : public wxCaretBase
{
public:
    wxCaret() { Init(); }
        // create the caret of given (in pixels) width and height and associate
        // with the given window
    wxCaret(wxWindow *window, int width, int height)
    {
        Init();

        (void)Create(window, width, height);
    }
        // same as above
    wxCaret(wxWindowBase *window, const wxSize& size)
    {
        Init();

        (void)Create(window, size);
    }

    // process wxWindow notifications
    virtual void OnSetFocus();
    virtual void OnKillFocus();

protected:
    void Init()
    {
        wxCaretBase::Init();

        m_hasCaret = false;
    }

    // override base class virtuals
    virtual void DoMove();
    virtual void DoShow();
    virtual void DoHide();
    virtual void DoSize();

    // helper function which creates the system caret
    bool MSWCreateCaret();

private:
    bool m_hasCaret;

    DECLARE_NO_COPY_CLASS(wxCaret)
};

#endif // _WX_CARET_H_


