//////////////////////////////////////////////////////////////////////////////
// Name:        zstream.cpp
// Purpose:     Compressed stream classes
// Author:      Guilhem Lavaux
// Modified by: Mike Wetherell
// Created:     11/07/98
// RCS-ID:      $Id: zstream.cpp,v 1.51 2005/05/31 09:19:53 JS Exp $
// Copyright:   (c) Guilhem Lavaux
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "zstream.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
  #pragma hdrstop
#endif

#if wxUSE_ZLIB && wxUSE_STREAMS

#include "wx/zstream.h"
#include "wx/utils.h"
#include "wx/intl.h"
#include "wx/log.h"

// normally, the compiler options should contain -I../zlib, but it is
// apparently not the case for all MSW makefiles and so, unless we use
// configure (which defines __WX_SETUP_H__) or it is explicitly overridden by
// the user (who can define wxUSE_ZLIB_H_IN_PATH), we hardcode the path here
#if defined(__WXMSW__) && !defined(__WX_SETUP_H__) && !defined(wxUSE_ZLIB_H_IN_PATH)
   #include "../zlib/zlib.h"
#else
   #include "zlib.h"
#endif

enum {
    ZSTREAM_BUFFER_SIZE = 16384,
    ZSTREAM_GZIP        = 0x10,     // gzip header
    ZSTREAM_AUTO        = 0x20      // auto detect between gzip and zlib
};

//////////////////////
// wxZlibInputStream
//////////////////////

wxZlibInputStream::wxZlibInputStream(wxInputStream& stream, int flags)
  : wxFilterInputStream(stream)
{
  m_inflate = NULL;
  m_z_buffer = new unsigned char[ZSTREAM_BUFFER_SIZE];
  m_z_size = ZSTREAM_BUFFER_SIZE;
  m_pos = 0;

#if WXWIN_COMPATIBILITY_2_4
  // treat compatibility mode as auto
  m_24compatibilty = flags == wxZLIB_24COMPATIBLE;
  if (m_24compatibilty)
    flags = wxZLIB_AUTO;
#endif

  // if gzip is asked for but not supported...
  if ((flags == wxZLIB_GZIP || flags == wxZLIB_AUTO) && !CanHandleGZip()) {
    if (flags == wxZLIB_AUTO) {
      // an error will come later if the input turns out not to be a zlib
      flags = wxZLIB_ZLIB;
    }
    else {
      wxLogError(_("Gzip not supported by this version of zlib"));
      m_lasterror = wxSTREAM_READ_ERROR;
      return;
    }
  }

  if (m_z_buffer) {
    m_inflate = new z_stream_s;

    if (m_inflate) {
      memset(m_inflate, 0, sizeof(z_stream_s));

      // see zlib.h for documentation on windowBits
      int windowBits = MAX_WBITS;
      switch (flags) {
        case wxZLIB_NO_HEADER:  windowBits = -MAX_WBITS; break;
        case wxZLIB_ZLIB:       windowBits = MAX_WBITS; break;
        case wxZLIB_GZIP:       windowBits = MAX_WBITS | ZSTREAM_GZIP; break;
        case wxZLIB_AUTO:       windowBits = MAX_WBITS | ZSTREAM_AUTO; break;
        default:                wxFAIL_MSG(wxT("Invalid zlib flag"));
      }

      if (inflateInit2(m_inflate, windowBits) == Z_OK)
        return;
    }
  }

  wxLogError(_("Can't initialize zlib inflate stream."));
  m_lasterror = wxSTREAM_READ_ERROR;
}

wxZlibInputStream::~wxZlibInputStream()
{
  inflateEnd(m_inflate);
  delete m_inflate;

  delete [] m_z_buffer;
}

size_t wxZlibInputStream::OnSysRead(void *buffer, size_t size)
{
  wxASSERT_MSG(m_inflate && m_z_buffer, wxT("Inflate stream not open"));

  if (!m_inflate || !m_z_buffer)
    m_lasterror = wxSTREAM_READ_ERROR;
  if (!IsOk() || !size)
    return 0;

  int err = Z_OK;
  m_inflate->next_out = (unsigned char *)buffer;
  m_inflate->avail_out = size;

  while (err == Z_OK && m_inflate->avail_out > 0) {
    if (m_inflate->avail_in == 0 && m_parent_i_stream->IsOk()) {
      m_parent_i_stream->Read(m_z_buffer, m_z_size);
      m_inflate->next_in = m_z_buffer;
      m_inflate->avail_in = m_parent_i_stream->LastRead();
    }
    err = inflate(m_inflate, Z_SYNC_FLUSH);
  }

  switch (err) {
    case Z_OK:
        break;

    case Z_STREAM_END:
      // Unread any data taken from past the end of the deflate stream, so that
      // any additional data can be read from the underlying stream (the crc
      // in a gzip for example)
      if (m_inflate->avail_in) {
        m_parent_i_stream->Ungetch(m_inflate->next_in, m_inflate->avail_in);
        m_inflate->avail_in = 0;
      }
      m_lasterror = wxSTREAM_EOF;
      break;

    case Z_BUF_ERROR:
      // Indicates that zlib was expecting more data, but the parent stream
      // has none. Other than Eof the error will have been already reported
      // by the parent strean,
      m_lasterror = wxSTREAM_READ_ERROR;
      if (m_parent_i_stream->Eof())
#if WXWIN_COMPATIBILITY_2_4
        if (m_24compatibilty)
          m_lasterror = wxSTREAM_EOF;
        else
#endif
          wxLogError(_("Can't read inflate stream: unexpected EOF in underlying stream."));
      break;

    default:
      wxString msg(m_inflate->msg, *wxConvCurrent);
      if (!msg)
        msg = wxString::Format(_("zlib error %d"), err);
      wxLogError(_("Can't read from inflate stream: %s"), msg.c_str());
      m_lasterror = wxSTREAM_READ_ERROR;
  }

  size -= m_inflate->avail_out;
  m_pos += size;
  return size;
}

/* static */ bool wxZlibInputStream::CanHandleGZip()
{
  const char *dot = strchr(zlibVersion(), '.');
  int major = atoi(zlibVersion());
  int minor = dot ? atoi(dot + 1) : 0;
  return major > 1 || (major == 1 && minor >= 2);
}


//////////////////////
// wxZlibOutputStream
//////////////////////

wxZlibOutputStream::wxZlibOutputStream(wxOutputStream& stream,
                                       int level,
                                       int flags)
 : wxFilterOutputStream(stream)
{
  m_deflate = NULL;
  m_z_buffer = new unsigned char[ZSTREAM_BUFFER_SIZE];
  m_z_size = ZSTREAM_BUFFER_SIZE;
  m_pos = 0;

  if ( level == -1 )
  {
    level = Z_DEFAULT_COMPRESSION;
  }
  else
  {
    wxASSERT_MSG(level >= 0 && level <= 9, wxT("wxZlibOutputStream compression level must be between 0 and 9!"));
  }

  // if gzip is asked for but not supported...
  if (flags == wxZLIB_GZIP && !CanHandleGZip()) {
    wxLogError(_("Gzip not supported by this version of zlib"));
    m_lasterror = wxSTREAM_WRITE_ERROR;
    return;
  }

  if (m_z_buffer) {
    m_deflate = new z_stream_s;

    if (m_deflate) {
      memset(m_deflate, 0, sizeof(z_stream_s));
      m_deflate->next_out = m_z_buffer;
      m_deflate->avail_out = m_z_size;

      // see zlib.h for documentation on windowBits
      int windowBits = MAX_WBITS;
      switch (flags) {
        case wxZLIB_NO_HEADER:  windowBits = -MAX_WBITS; break;
        case wxZLIB_ZLIB:       windowBits = MAX_WBITS; break;
        case wxZLIB_GZIP:       windowBits = MAX_WBITS | ZSTREAM_GZIP; break;
        default:                wxFAIL_MSG(wxT("Invalid zlib flag"));
      }

      if (deflateInit2(m_deflate, level, Z_DEFLATED, windowBits,
                       8, Z_DEFAULT_STRATEGY) == Z_OK)
        return;
    }
  }

  wxLogError(_("Can't initialize zlib deflate stream."));
  m_lasterror = wxSTREAM_WRITE_ERROR;
}

bool wxZlibOutputStream::Close()
 {
  DoFlush(true);
   deflateEnd(m_deflate);
   delete m_deflate;

  m_deflate = NULL;
   delete[] m_z_buffer;
  m_z_buffer = NULL;
  return IsOk();
 }

void wxZlibOutputStream::DoFlush(bool final)
{
  if (!m_deflate || !m_z_buffer)
    m_lasterror = wxSTREAM_WRITE_ERROR;
  if (!IsOk())
    return;

  int err = Z_OK;
  bool done = false;

  while (err == Z_OK || err == Z_STREAM_END) {
    size_t len = m_z_size  - m_deflate->avail_out;
    if (len) {
      if (m_parent_o_stream->Write(m_z_buffer, len).LastWrite() != len) {
        m_lasterror = wxSTREAM_WRITE_ERROR;
        wxLogDebug(wxT("wxZlibOutputStream: Error writing to underlying stream"));
        break;
      }
      m_deflate->next_out = m_z_buffer;
      m_deflate->avail_out = m_z_size;
    }

    if (done)
      break;
    err = deflate(m_deflate, final ? Z_FINISH : Z_FULL_FLUSH);
    done = m_deflate->avail_out != 0 || err == Z_STREAM_END;
  }
}

size_t wxZlibOutputStream::OnSysWrite(const void *buffer, size_t size)
{
  wxASSERT_MSG(m_deflate && m_z_buffer, wxT("Deflate stream not open"));

  if (!m_deflate || !m_z_buffer)
    m_lasterror = wxSTREAM_WRITE_ERROR;
  if (!IsOk() || !size)
    return 0;

  int err = Z_OK;
  m_deflate->next_in = (unsigned char *)buffer;
  m_deflate->avail_in = size;

  while (err == Z_OK && m_deflate->avail_in > 0) {
    if (m_deflate->avail_out == 0) {
      m_parent_o_stream->Write(m_z_buffer, m_z_size);
      if (m_parent_o_stream->LastWrite() != m_z_size) {
        m_lasterror = wxSTREAM_WRITE_ERROR;
        wxLogDebug(wxT("wxZlibOutputStream: Error writing to underlying stream"));
        break;
      }

      m_deflate->next_out = m_z_buffer;
      m_deflate->avail_out = m_z_size;
    }

    err = deflate(m_deflate, Z_NO_FLUSH);
  }

  if (err != Z_OK) {
    m_lasterror = wxSTREAM_WRITE_ERROR;
    wxString msg(m_deflate->msg, *wxConvCurrent);
    if (!msg)
      msg = wxString::Format(_("zlib error %d"), err);
    wxLogError(_("Can't write to deflate stream: %s"), msg.c_str());
  }

  size -= m_deflate->avail_in;
  m_pos += size;
  return size;
}

/* static */ bool wxZlibOutputStream::CanHandleGZip()
{
  return wxZlibInputStream::CanHandleGZip();
}

#endif
  // wxUSE_ZLIB && wxUSE_STREAMS

