/////////////////////////////////////////////////////////////////////////////
// Name:        wx/palmos/sound.h
// Purpose:     wxSound class
// Author:      William Osborne - minimal working wxPalmOS port
// Modified by:
// Created:     10/13/04
// RCS-ID:      $Id: sound.h,v 1.2 2005/01/18 21:14:26 ABX Exp $
// Copyright:   (c) William Osborne
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_SOUND_H_
#define _WX_SOUND_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "sound.h"
#endif

#if wxUSE_SOUND

#include "wx/object.h"

class WXDLLIMPEXP_ADV wxSound : public wxSoundBase
{
public:
  wxSound();
  wxSound(const wxString& fileName, bool isResource = false);
  wxSound(int size, const wxByte* data);
  ~wxSound();

public:
  // Create from resource or file
  bool  Create(const wxString& fileName, bool isResource = false);
  // Create from data
  bool Create(int size, const wxByte* data);

  bool  IsOk() const { return (m_waveData ? true : false); };

  static void Stop();

protected:
  bool  Free();

  bool DoPlay(unsigned flags) const;

private:
  wxByte* m_waveData;
  int   m_waveLength;
  bool  m_isResource;

    DECLARE_NO_COPY_CLASS(wxSound)
};
#endif
#endif

