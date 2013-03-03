/////////////////////////////////////////////////////////////////////////////
// Name:        display.h
// Purpose:     wxDisplay class customization for WXMSW
// Author:      Royce Mitchell III
// Modified by:
// Created:     06/21/02
// RCS-ID:      $Id: display.h,v 1.9 2004/11/07 00:48:36 RN Exp $
// Copyright:   (c) wxWidgets team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "display.h"
#endif

#ifndef _WX_MSW_DISPLAY_H_
#define _WX_MSW_DISPLAY_H_

class WXDLLEXPORT wxDisplay : public wxDisplayBase
{
public:
    // this function may be called *before* using any other wxDisplay methods
    // to tell it to use DirectX functions instead of the standard Windows ones
    static void UseDirectX(bool useDX);

    // create the display object for the given physical display
    wxDisplay(size_t index = 0);

    virtual ~wxDisplay();

    // implement base class pure virtuals
    virtual bool IsOk() const;
    virtual wxRect GetGeometry() const;
    virtual wxString GetName() const;

    virtual wxArrayVideoModes
        GetModes(const wxVideoMode& mode = wxVideoMode()) const;
    virtual wxVideoMode GetCurrentMode() const;
    virtual bool ChangeMode(const wxVideoMode& mode = wxVideoMode());

    virtual bool IsPrimary() const;

private:
    // get the display name to use with EnumDisplaySettings()
    wxString GetNameForEnumSettings() const;

    // we have different implementations using DirectDraw and without it
    wxArrayVideoModes DoGetModesDirectX(const wxVideoMode& modeMatch) const;
    bool DoChangeModeDirectX(const wxVideoMode& mode);

    wxArrayVideoModes DoGetModesWindows(const wxVideoMode& modeMatch) const;
    bool DoChangeModeWindows(const wxVideoMode& mode);


    DECLARE_NO_COPY_CLASS(wxDisplay)
};

#endif // _WX_MSW_DISPLAY_H_
