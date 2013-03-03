///////////////////////////////////////////////////////////////////////////////
// Name:        wx/mediactrl.h
// Purpose:     wxMediaCtrl class
// Author:      Ryan Norton <wxprojects@comcast.net>
// Modified by:
// Created:     11/07/04
// RCS-ID:      $Id: mediactrl.h,v 1.23 2005/05/21 16:45:12 JS Exp $
// Copyright:   (c) Ryan Norton
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// Definitions
// ============================================================================

// ----------------------------------------------------------------------------
// Header guard
// ----------------------------------------------------------------------------
#ifndef _WX_MEDIACTRL_H_
#define _WX_MEDIACTRL_H_

// ----------------------------------------------------------------------------
// Pre-compiled header stuff
// ----------------------------------------------------------------------------
#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "mediactrl.h"
#endif

#include "wx/defs.h"

// ----------------------------------------------------------------------------
// Compilation guard
// ----------------------------------------------------------------------------

#if wxUSE_MEDIACTRL

// ----------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------

#include "wx/control.h"
#include "wx/uri.h"

// ============================================================================
// Declarations
// ============================================================================

// ----------------------------------------------------------------------------
//
// Enumerations
//
// ----------------------------------------------------------------------------

enum wxMediaState
{
    wxMEDIASTATE_STOPPED,
    wxMEDIASTATE_PAUSED,
    wxMEDIASTATE_PLAYING
};

#define wxMEDIABACKEND_DIRECTSHOW   wxT("wxAMMediaBackend")
#define wxMEDIABACKEND_MCI          wxT("wxMCIMediaBackend")
#define wxMEDIABACKEND_QUICKTIME    wxT("wxQTMediaBackend")
#define wxMEDIABACKEND_GSTREAMER    wxT("wxGStreamerMediaBackend")

// ----------------------------------------------------------------------------
//
// wxMediaEvent
//
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_MEDIA wxMediaEvent : public wxNotifyEvent
{
public:
    // ------------------------------------------------------------------------
    // wxMediaEvent Constructor
    //
    // Normal constructor, much the same as wxNotifyEvent
    // ------------------------------------------------------------------------
    wxMediaEvent(wxEventType commandType = wxEVT_NULL, int winid = 0)
        : wxNotifyEvent(commandType, winid)
    {                                       }

    // ------------------------------------------------------------------------
    // wxMediaEvent Copy Constructor
    //
    // Normal copy constructor, much the same as wxNotifyEvent
    // ------------------------------------------------------------------------
    wxMediaEvent(const wxMediaEvent &clone)
            : wxNotifyEvent(clone)
    {                                       }

    // ------------------------------------------------------------------------
    // wxMediaEvent::Clone
    //
    // Allocates a copy of this object.
    // Required for wxEvtHandler::AddPendingEvent
    // ------------------------------------------------------------------------
    virtual wxEvent *Clone() const
    {   return new wxMediaEvent(*this);     }


    // Put this class on wxWidget's RTTI table
    DECLARE_DYNAMIC_CLASS(wxMediaEvent)
};

// ----------------------------------------------------------------------------
//
// wxMediaCtrl
//
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_MEDIA wxMediaCtrl : public wxControl
{
public:
    wxMediaCtrl() : m_imp(NULL), m_bLoaded(false)
    {                                                                   }

    wxMediaCtrl(wxWindow* parent, wxWindowID winid,
                const wxString& fileName = wxEmptyString,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxString& szBackend = wxEmptyString,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = wxT("mediaCtrl"))
                : m_imp(NULL), m_bLoaded(false)
    {   Create(parent, winid, fileName, pos, size, style,
               szBackend, validator, name);                             }

    wxMediaCtrl(wxWindow* parent, wxWindowID winid,
                const wxURI& location,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxString& szBackend = wxEmptyString,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = wxT("mediaCtrl"))
                : m_imp(NULL), m_bLoaded(false)
    {   Create(parent, winid, location, pos, size, style,
               szBackend, validator, name);                             }

    ~wxMediaCtrl();

    bool Create(wxWindow* parent, wxWindowID winid,
                const wxString& fileName = wxEmptyString,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxString& szBackend = wxEmptyString,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = wxT("mediaCtrl"));

    bool Create(wxWindow* parent, wxWindowID winid,
                const wxURI& location,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxString& szBackend = wxEmptyString,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = wxT("mediaCtrl")); //DirectShow only

    bool DoCreate(wxClassInfo* instance,
                wxWindow* parent, wxWindowID winid,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = wxT("mediaCtrl"));

    bool Play();
    bool Pause();
    bool Stop();

    bool Load(const wxString& fileName);


    wxMediaState GetState();

    wxFileOffset Seek(wxFileOffset where, wxSeekMode mode = wxFromStart);
    wxFileOffset Tell(); //FIXME: This should be const
    wxFileOffset Length(); //FIXME: This should be const

    //
    // Unofficial parts of API
    //
    //DirectShow/GStreamer only.  Quicktime too, but somewhat buggy...
    bool Load(const wxURI& location);

    double GetPlaybackRate();           //All but MCI & GStreamer
    bool SetPlaybackRate(double dRate); //All but MCI & GStreamer

    double GetVolume();                 //DirectShow only
    bool   SetVolume(double dVolume);   //DirectShow only

protected:
    static wxClassInfo* NextBackend();

    void OnMediaFinished(wxMediaEvent& evt);
    virtual void DoMoveWindow(int x, int y, int w, int h);
    wxSize DoGetBestSize() const;

    //FIXME:  This is nasty... find a better way to work around
    //inheritance issues
#ifdef __WXMAC__
    friend class wxQTMediaBackend;
#endif
#ifdef __WXCOCOA__
    friend class wxQTMediaBackend;
#endif
    class wxMediaBackend* m_imp;
    bool m_bLoaded;

    DECLARE_DYNAMIC_CLASS(wxMediaCtrl)
};

// ----------------------------------------------------------------------------
//
// wxMediaBackend
//
// Currently an internal class - API stability not guaranteed.
//
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_MEDIA wxMediaBackend : public wxObject
{
public:
    wxMediaBackend()
    {                                   }

    virtual ~wxMediaBackend();

    virtual bool CreateControl(wxControl* WXUNUSED(ctrl),
                               wxWindow* WXUNUSED(parent),
                               wxWindowID WXUNUSED(winid),
                               const wxPoint& WXUNUSED(pos),
                               const wxSize& WXUNUSED(size),
                               long WXUNUSED(style),
                               const wxValidator& WXUNUSED(validator),
                               const wxString& WXUNUSED(name))
    {   return false;                   }

    virtual bool Play()
    {   return false;                   }
    virtual bool Pause()
    {   return false;                   }
    virtual bool Stop()
    {   return false;                   }

    virtual bool Load(const wxString& WXUNUSED(fileName))
    {   return false;                   }
    virtual bool Load(const wxURI& WXUNUSED(location))
    {   return false;                   }

    virtual bool SetPosition(wxLongLong WXUNUSED(where))
    {   return 0;                       }
    virtual wxLongLong GetPosition()
    {   return 0;                       }
    virtual wxLongLong GetDuration()
    {   return 0;                       }

    virtual void Move(int WXUNUSED(x), int WXUNUSED(y),
                      int WXUNUSED(w), int WXUNUSED(h))
    {                                   }
    virtual wxSize GetVideoSize() const
    {   return wxSize(0,0);             }

    virtual double GetPlaybackRate()
    {   return 0.0;                     }
    virtual bool SetPlaybackRate(double WXUNUSED(dRate))
    {   return false;                   }

    virtual wxMediaState GetState()
    {   return wxMEDIASTATE_STOPPED;    }

    virtual double GetVolume()
    {   return 0.0;                     }
    virtual bool SetVolume(double WXUNUSED(dVolume))
    {   return false;                   }

    virtual void RESERVED3() {}
    virtual void RESERVED4() {}
    virtual void RESERVED5() {}
    virtual void RESERVED6() {}
    virtual void RESERVED7() {}
    virtual void RESERVED8() {}
    virtual void RESERVED9() {}

    DECLARE_DYNAMIC_CLASS(wxMediaBackend)
};

//Event ID to give to our events
#define wxMEDIA_FINISHED_ID    13000
#define wxMEDIA_STOP_ID    13001

//Define our event types - we need to call DEFINE_EVENT_TYPE(EVT) later
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_MEDIA, wxEVT_MEDIA_FINISHED, wxMEDIA_FINISHED_ID)
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_MEDIA, wxEVT_MEDIA_STOP,     wxMEDIA_STOP_ID)

//Function type(s) our events need
typedef void (wxEvtHandler::*wxMediaEventFunction)(wxMediaEvent&);

#define wxMediaEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(wxMediaEventFunction, &func)

//Macro for usage with message maps
#define EVT_MEDIA_FINISHED(winid, fn) DECLARE_EVENT_TABLE_ENTRY( wxEVT_MEDIA_FINISHED, winid, wxID_ANY, wxMediaEventHandler(fn), (wxObject *) NULL ),
#define EVT_MEDIA_STOP(winid, fn) DECLARE_EVENT_TABLE_ENTRY( wxEVT_MEDIA_STOP, winid, wxID_ANY, wxMediaEventHandler(fn), (wxObject *) NULL ),

// ----------------------------------------------------------------------------
// End compilation gaurd
// ----------------------------------------------------------------------------
#endif // wxUSE_MEDIACTRL

// ----------------------------------------------------------------------------
// End header guard and header itself
// ----------------------------------------------------------------------------
#endif // _WX_MEDIACTRL_H_


