///////////////////////////////////////////////////////////////////////////////
// Name:        wx/unix/apptrait.h
// Purpose:     standard implementations of wxAppTraits for Unix
// Author:      Vadim Zeitlin
// Modified by:
// Created:     23.06.2003
// RCS-ID:      $Id: apptrait.h,v 1.7 2004/11/12 21:20:46 DE Exp $
// Copyright:   (c) 2003 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_UNIX_APPTRAIT_H_
#define _WX_UNIX_APPTRAIT_H_

// ----------------------------------------------------------------------------
// wxGUI/ConsoleAppTraits: must derive from wxAppTraits, not wxAppTraitsBase
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxConsoleAppTraits : public wxConsoleAppTraitsBase
{
public:
    virtual bool CreateEndProcessPipe(wxExecuteData& execData);
    virtual bool IsWriteFDOfEndProcessPipe(wxExecuteData& execData, int fd);
    virtual void DetachWriteFDOfEndProcessPipe(wxExecuteData& execData);
    virtual int WaitForChild(wxExecuteData& execData);

    virtual wxToolkitInfo& GetToolkitInfo();
};

#if wxUSE_GUI

class WXDLLEXPORT wxGUIAppTraits : public wxGUIAppTraitsBase
{
public:
    virtual bool CreateEndProcessPipe(wxExecuteData& execData);
    virtual bool IsWriteFDOfEndProcessPipe(wxExecuteData& execData, int fd);
    virtual void DetachWriteFDOfEndProcessPipe(wxExecuteData& execData);
    virtual int WaitForChild(wxExecuteData& execData);

#if defined(__WXMAC__) || defined(__WXCOCOA__)
    virtual wxStandardPathsBase& GetStandardPaths();
#endif
    virtual wxToolkitInfo& GetToolkitInfo();
};

#endif // wxUSE_GUI

#endif // _WX_UNIX_APPTRAIT_H_

