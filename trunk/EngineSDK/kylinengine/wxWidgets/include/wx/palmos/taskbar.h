/////////////////////////////////////////////////////////////////////////
// File:        wx/palmos/taskbar.h
// Purpose:     Defines wxTaskBarIcon class.
// Author:      William Osborne - minimal working wxPalmOS port
// Modified by:
// Created:     10/13/04
// RCS-ID:      $Id: taskbar.h,v 1.3 2005/01/18 21:14:27 ABX Exp $
// Copyright:   (c) William Osborne
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////

#ifndef _TASKBAR_H_
#define _TASKBAR_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "taskbar.h"
#endif

#include "wx/icon.h"

// private helper class:
class WXDLLIMPEXP_ADV wxTaskBarIconWindow;

class WXDLLIMPEXP_ADV wxTaskBarIcon: public wxTaskBarIconBase
{
    DECLARE_DYNAMIC_CLASS_NO_COPY(wxTaskBarIcon)
public:
    wxTaskBarIcon();
    virtual ~wxTaskBarIcon();

// Accessors
    inline bool IsOk() const { return true; }
    inline bool IsIconInstalled() const { return m_iconAdded; }

// Operations
    bool SetIcon(const wxIcon& icon, const wxString& tooltip = wxEmptyString);
    bool RemoveIcon(void);
    bool PopupMenu(wxMenu *menu); //, int x, int y);

// Implementation
protected:
    friend class wxTaskBarIconWindow;
    long WindowProc(unsigned int msg, unsigned int wParam, long lParam);
    void RegisterWindowMessages();

// Data members
protected:
    wxTaskBarIconWindow *m_win;
    bool                 m_iconAdded;
    wxIcon               m_icon;
    wxString             m_strTooltip;
};

#endif
    // _TASKBAR_H_
