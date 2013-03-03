/////////////////////////////////////////////////////////////////////////////
// Name:        icon.h
// Purpose:     wxIcon class
// Author:      David Webster
// Modified by:
// Created:     10/09/99
// RCS-ID:      $Id: icon.h,v 1.13 2004/11/11 18:59:36 VZ Exp $
// Copyright:   (c) David Webster
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_ICON_H_
#define _WX_ICON_H_

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include "wx/bitmap.h"
#include "wx/os2/gdiimage.h"

#define wxIconRefDataBase   wxGDIImageRefData
#define wxIconBase          wxGDIImage

class WXDLLEXPORT wxIconRefData: public wxIconRefDataBase
{
public:
    wxIconRefData() { };
    virtual ~wxIconRefData() { Free(); }

    virtual void Free();
}; // end of

// ---------------------------------------------------------------------------
// Icon
// ---------------------------------------------------------------------------

class WXDLLEXPORT wxIcon: public wxIconBase
{
public:
    wxIcon();

    // Copy constructors
    inline wxIcon(const wxIcon& icon) { Ref(icon); }

    wxIcon( const char bits[]
           ,int        nWidth
           ,int        nHeight
          );
    inline wxIcon(const char** ppData) { CreateIconFromXpm(ppData); }
    inline wxIcon(char** ppData) { CreateIconFromXpm((const char**)ppData); }
    wxIcon( const wxString& rName
           ,long            lFlags = wxBITMAP_TYPE_ICO_RESOURCE
           ,int             nDesiredWidth = -1
           ,int             nDesiredHeight = -1
          );
    wxIcon(const wxIconLocation& loc)
    {
        LoadFile(loc.GetFileName(), wxBITMAP_TYPE_ICO);
    }

    ~wxIcon();

    bool LoadFile( const wxString& rName
                  ,long            lFlags = wxBITMAP_TYPE_ICO_RESOURCE
                  ,int             nDesiredWidth = -1
                  ,int             nDesiredHeight = -1
                 );

    inline wxIcon& operator = (const wxIcon& rIcon)
       { if (*this != rIcon) Ref(rIcon); return *this; }
    inline bool operator == (const wxIcon& rIcon) const
       { return m_refData == rIcon.m_refData; }
    inline bool operator != (const wxIcon& rIcon) const
       { return m_refData != rIcon.m_refData; }

    wxIconRefData *GetIconData() const { return (wxIconRefData *)m_refData; }

    inline void SetHICON(WXHICON hIcon) { SetHandle((WXHANDLE)hIcon); }
    inline WXHICON GetHICON() const { return (WXHICON)GetHandle(); }
    inline bool    IsXpm(void) const { return m_bIsXpm; };
    inline const wxBitmap& GetXpmSrc(void) const { return m_vXpmSrc; }

    void CopyFromBitmap(const wxBitmap& rBmp);
protected:
    virtual wxGDIImageRefData* CreateData() const
    {
        return new wxIconRefData;
    }
    void    CreateIconFromXpm(const char **ppData);

private:
    bool                            m_bIsXpm;
    wxBitmap                        m_vXpmSrc;

    DECLARE_DYNAMIC_CLASS(wxIcon)
};

#endif
    // _WX_ICON_H_
