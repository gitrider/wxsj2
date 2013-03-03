/////////////////////////////////////////////////////////////////////////////
// Name:        cocoa/app.h
// Purpose:     wxApp class
// Author:      David Elliott
// Modified by:
// Created:     2002/11/27
// RCS-ID:      $Id: app.h,v 1.21 2004/05/23 20:50:42 JS Exp $
// Copyright:   (c) 2002 David Elliott
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_COCOA_APP_H_
#define _WX_COCOA_APP_H_

// ========================================================================
// wxApp
// ========================================================================
// Represents the application. Derive OnInit and declare
// a new App object to start application
class WXDLLEXPORT wxApp: public wxAppBase
{
    DECLARE_DYNAMIC_CLASS(wxApp)
    DECLARE_EVENT_TABLE()
// ------------------------------------------------------------------------
// initialization
// ------------------------------------------------------------------------
public:
    wxApp();
    virtual ~wxApp() {}

// ------------------------------------------------------------------------
// Cocoa specifics
// ------------------------------------------------------------------------
public:
    inline WX_NSApplication GetNSApplication() { return m_cocoaApp; }
    virtual void CocoaDelegate_applicationWillBecomeActive();
    virtual void CocoaDelegate_applicationDidBecomeActive();
    virtual void CocoaDelegate_applicationWillResignActive();
    virtual void CocoaDelegate_applicationDidResignActive();
protected:
    WX_NSApplication m_cocoaApp;
    struct objc_object *m_cocoaAppDelegate;
    WX_NSThread m_cocoaMainThread;

// ------------------------------------------------------------------------
// Implementation
// ------------------------------------------------------------------------
public:
    // Implement wxAppBase pure virtuals
    virtual void Exit();

    virtual bool Yield(bool onlyIfNeeded = FALSE);
    virtual void WakeUpIdle();
    
    virtual bool Initialize(int& argc, wxChar **argv);
    virtual void CleanUp();
    virtual bool CallOnInit();

    
    virtual bool OnInit();
    virtual bool OnInitGui();

#ifdef __WXDEBUG__
    virtual void OnAssert(const wxChar *file, int line, const wxChar *cond, const wxChar *msg);
    bool IsInAssert() const { return m_isInAssert; }
#endif // __WXDEBUG__

private:
#ifdef __WXDEBUG__
    bool m_isInAssert;
#endif // __WXDEBUG__
};

#endif // _WX_COCOA_APP_H_
