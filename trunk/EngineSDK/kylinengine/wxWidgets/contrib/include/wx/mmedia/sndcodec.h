// --------------------------------------------------------------------------
// Name: sndcodec.h
// Purpose:
// Date: 08/11/1999
// Author: Guilhem Lavaux <lavaux@easynet.fr> (C) 1999
// CVSID: $Id: sndcodec.h,v 1.4 2004/11/22 20:05:24 JS Exp $
// License:    wxWindows license
// --------------------------------------------------------------------------
#ifndef _WX_SNDCODEC_H
#define _WX_SNDCODEC_H

#ifdef __GNUG__
#pragma interface "sndcodec.h"
#endif

#include "wx/defs.h"
#include "wx/mmedia/defs.h"
#include "wx/mmedia/sndbase.h"

class WXDLLIMPEXP_MMEDIA wxSoundStreamCodec: public wxSoundStream {
 public:
  wxSoundStreamCodec(wxSoundStream& snd_io);
  ~wxSoundStreamCodec();

  bool StartProduction(int evt);
  bool StopProduction();

  wxUint32 GetBestSize() const;

 protected:
  wxSoundStream *m_sndio;
};

#endif
