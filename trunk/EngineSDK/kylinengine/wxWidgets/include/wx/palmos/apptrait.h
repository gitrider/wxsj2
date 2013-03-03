///////////////////////////////////////////////////////////////////////////////
// Name:        wx/palmos/apptrait.h
// Purpose:     class implementing wxAppTraits for Palm OS
// Author:      William Osborne - minimal working wxPalmOS port
// Modified by:
// Created:     10/13/04
// RCS-ID:      $Id: apptrait.h,v 1.3 2005/01/18 21:14:22 ABX Exp $
// Copyright:   (c) William Osborne
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_PALM_APPTRAIT_H_
#define _WX_PALM_APPTRAIT_H_

// ----------------------------------------------------------------------------
// wxGUI/ConsoleAppTraits: must derive from wxAppTraits, not wxAppTraitsBase
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_BASE wxConsoleAppTraits : public wxConsoleAppTraitsBase
{
public:
    virtual void *BeforeChildWaitLoop();
    virtual void AlwaysYield();
    virtual void AfterChildWaitLoop(void *data);

    virtual bool DoMessageFromThreadWait();
};

#if wxUSE_GUI

class WXDLLIMPEXP_CORE wxGUIAppTraits : public wxGUIAppTraitsBase
{
public:
    virtual void *BeforeChildWaitLoop();
    virtual void AlwaysYield();
    virtual void AfterChildWaitLoop(void *data);

    virtual bool DoMessageFromThreadWait();
    virtual wxToolkitInfo& GetToolkitInfo();
};

#endif // wxUSE_GUI

#endif // _WX_PALM_APPTRAIT_H_

