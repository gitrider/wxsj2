/////////////////////////////////////////////////////////////////////////////
// Name:        imagtiff.cpp
// Purpose:     wxImage TIFF handler
// Author:      Robert Roebling
// RCS-ID:      $Id: imagtiff.cpp,v 1.32 2005/02/28 16:05:04 VZ Exp $
// Copyright:   (c) Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "imagtiff.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "wx/defs.h"

#if wxUSE_IMAGE && wxUSE_LIBTIFF

#include "wx/imagtiff.h"
#include "wx/bitmap.h"
#include "wx/debug.h"
#include "wx/log.h"
#include "wx/app.h"
extern "C"
{
    #include "tiff.h"
    #include "tiffio.h"
}
#include "wx/filefn.h"
#include "wx/wfstream.h"
#include "wx/intl.h"
#include "wx/module.h"

#ifndef TIFFLINKAGEMODE
  #define TIFFLINKAGEMODE LINKAGEMODE
#endif

//-----------------------------------------------------------------------------
// wxTIFFHandler
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxTIFFHandler,wxImageHandler)

#if wxUSE_STREAMS

extern "C"
{

tsize_t TIFFLINKAGEMODE
_tiffNullProc(thandle_t WXUNUSED(handle),
          tdata_t WXUNUSED(buf),
          tsize_t WXUNUSED(size))
{
    return (tsize_t) -1;
}

tsize_t TIFFLINKAGEMODE
_tiffReadProc(thandle_t handle, tdata_t buf, tsize_t size)
{
    wxInputStream *stream = (wxInputStream*) handle;
    stream->Read( (void*) buf, (size_t) size );
    return stream->LastRead();
}

tsize_t TIFFLINKAGEMODE
_tiffWriteProc(thandle_t handle, tdata_t buf, tsize_t size)
{
    wxOutputStream *stream = (wxOutputStream*) handle;
    stream->Write( (void*) buf, (size_t) size );
    return stream->LastWrite();
}

toff_t TIFFLINKAGEMODE
_tiffSeekIProc(thandle_t handle, toff_t off, int whence)
{
    wxInputStream *stream = (wxInputStream*) handle;
    wxSeekMode mode;
    switch (whence)
    {
        case SEEK_SET: mode = wxFromStart; break;
        case SEEK_CUR: mode = wxFromCurrent; break;
        case SEEK_END: mode = wxFromEnd; break;
        default:       mode = wxFromCurrent; break;
    }

    return (toff_t)stream->SeekI( (wxFileOffset)off, mode );
}

toff_t TIFFLINKAGEMODE
_tiffSeekOProc(thandle_t handle, toff_t off, int whence)
{
    wxOutputStream *stream = (wxOutputStream*) handle;
    wxSeekMode mode;
    switch (whence)
    {
        case SEEK_SET: mode = wxFromStart; break;
        case SEEK_CUR: mode = wxFromCurrent; break;
        case SEEK_END: mode = wxFromEnd; break;
        default:       mode = wxFromCurrent; break;
    }

    return (toff_t)stream->SeekO( (wxFileOffset)off, mode );
}

int TIFFLINKAGEMODE
_tiffCloseProc(thandle_t WXUNUSED(handle))
{
    return 0;  // ?
}

toff_t TIFFLINKAGEMODE
_tiffSizeProc(thandle_t handle)
{
    wxStreamBase *stream = (wxStreamBase*) handle;
    return (toff_t) stream->GetSize();
}

int TIFFLINKAGEMODE
_tiffMapProc(thandle_t WXUNUSED(handle),
             tdata_t* WXUNUSED(pbase),
             toff_t* WXUNUSED(psize))
{
    return 0;
}

void TIFFLINKAGEMODE
_tiffUnmapProc(thandle_t WXUNUSED(handle),
               tdata_t WXUNUSED(base),
               toff_t WXUNUSED(size))
{
}

static void
TIFFwxWarningHandler(const char* module, const char* fmt, va_list ap)
{
    if (module != NULL)
            wxLogWarning(_("tiff module: %s"), module);
    wxLogWarning((wxChar *) fmt, ap);
}

static void
TIFFwxErrorHandler(const char* module, const char* fmt, va_list ap)
{
    if (module != NULL)
            wxLogError(_("tiff module: %s"), module);
    wxVLogError((wxChar *) fmt, ap);
}

} // extern "C"

TIFF*
TIFFwxOpen(wxInputStream &stream, const char* name, const char* mode)
{
    TIFF* tif = TIFFClientOpen(name, mode,
        (thandle_t) &stream,
        _tiffReadProc, _tiffNullProc,
        _tiffSeekIProc, _tiffCloseProc, _tiffSizeProc,
        _tiffMapProc, _tiffUnmapProc);

    return tif;
}

TIFF*
TIFFwxOpen(wxOutputStream &stream, const char* name, const char* mode)
{
    TIFF* tif = TIFFClientOpen(name, mode,
        (thandle_t) &stream,
        _tiffNullProc, _tiffWriteProc,
        _tiffSeekOProc, _tiffCloseProc, _tiffSizeProc,
        _tiffMapProc, _tiffUnmapProc);

    return tif;
}

wxTIFFHandler::wxTIFFHandler()
{
    m_name = wxT("TIFF file");
    m_extension = wxT("tif");
    m_type = wxBITMAP_TYPE_TIF;
    m_mime = wxT("image/tiff");
    TIFFSetWarningHandler((TIFFErrorHandler) TIFFwxWarningHandler);
    TIFFSetErrorHandler((TIFFErrorHandler) TIFFwxErrorHandler);
}

bool wxTIFFHandler::LoadFile( wxImage *image, wxInputStream& stream, bool verbose, int index )
{
    if (index == -1)
        index = 0;

    image->Destroy();

    TIFF *tif = TIFFwxOpen( stream, "image", "r" );

    if (!tif)
    {
        if (verbose)
            wxLogError( _("TIFF: Error loading image.") );

        return false;
    }

    if (!TIFFSetDirectory( tif, (tdir_t)index ))
    {
        if (verbose)
            wxLogError( _("Invalid TIFF image index.") );

        TIFFClose( tif );

        return false;
    }

    uint32 w, h;
    uint32 npixels;
    uint32 *raster;

    TIFFGetField( tif, TIFFTAG_IMAGEWIDTH, &w );
    TIFFGetField( tif, TIFFTAG_IMAGELENGTH, &h );

    npixels = w * h;

    raster = (uint32*) _TIFFmalloc( npixels * sizeof(uint32) );

    if (!raster)
    {
        if (verbose)
            wxLogError( _("TIFF: Couldn't allocate memory.") );

        TIFFClose( tif );

        return false;
    }

    image->Create( (int)w, (int)h );
    if (!image->Ok())
    {
        if (verbose)
            wxLogError( _("TIFF: Couldn't allocate memory.") );

        _TIFFfree( raster );
        TIFFClose( tif );

        return false;
    }

    if (!TIFFReadRGBAImage( tif, w, h, raster, 0 ))
    {
        if (verbose)
            wxLogError( _("TIFF: Error reading image.") );

        _TIFFfree( raster );
        image->Destroy();
        TIFFClose( tif );

        return false;
    }

    bool hasmask = false;

    unsigned char *ptr = image->GetData();
    ptr += w*3*(h-1);
    uint32 pos = 0;

    for (uint32 i = 0; i < h; i++)
    {
        for (uint32 j = 0; j < w; j++)
        {
            unsigned char alpha = (unsigned char)TIFFGetA(raster[pos]);
            if (alpha < 127)
            {
                hasmask = true;
                ptr[0] = image->GetMaskRed();
                ptr++;
                ptr[0] = image->GetMaskGreen();
                ptr++;
                ptr[0] = image->GetMaskBlue();
                ptr++;
            }
            else
            {
                ptr[0] = (unsigned char)TIFFGetR(raster[pos]);
                ptr++;
                ptr[0] = (unsigned char)TIFFGetG(raster[pos]);
                ptr++;
                ptr[0] = (unsigned char)TIFFGetB(raster[pos]);
                ptr++;
            }
            pos++;
        }
        ptr -= 2*w*3; // subtract line we just added plus one line
    }

    _TIFFfree( raster );

    TIFFClose( tif );

    image->SetMask( hasmask );

    return true;
}

int wxTIFFHandler::GetImageCount( wxInputStream& stream )
{
    TIFF *tif = TIFFwxOpen( stream, "image", "r" );

    if (!tif)
        return 0;

    int dircount = 0;  // according to the libtiff docs, dircount should be set to 1 here???
    do {
        dircount++;
    } while (TIFFReadDirectory(tif));

    TIFFClose( tif );

    return dircount;
}

bool wxTIFFHandler::SaveFile( wxImage *image, wxOutputStream& stream, bool verbose )
{
    TIFF *tif = TIFFwxOpen( stream, "image", "w" );

    if (!tif)
    {
        if (verbose)
            wxLogError( _("TIFF: Error saving image.") );

        return false;
    }

    TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH,  (uint32)image->GetWidth());
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, (uint32)image->GetHeight());
    TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

    if ( image->HasOption(wxIMAGE_OPTION_RESOLUTIONX) &&
            image->HasOption(wxIMAGE_OPTION_RESOLUTIONY) )
    {
        TIFFSetField(tif, TIFFTAG_XRESOLUTION,
                        image->GetOptionInt(wxIMAGE_OPTION_RESOLUTIONX));
        TIFFSetField(tif, TIFFTAG_YRESOLUTION,
                        image->GetOptionInt(wxIMAGE_OPTION_RESOLUTIONY));
    }

    int spp = image->GetOptionInt(wxIMAGE_OPTION_SAMPLESPERPIXEL);
    if ( !spp )
        spp = 3;

    int bpp = image->GetOptionInt(wxIMAGE_OPTION_BITSPERSAMPLE);
    if ( !bpp )
        bpp=8;

    int compression = image->GetOptionInt(wxIMAGE_OPTION_COMPRESSION);
    if ( !compression )
        compression=COMPRESSION_LZW;

    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, spp);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bpp);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, spp*bpp == 1 ? PHOTOMETRIC_MINISBLACK
                                                        : PHOTOMETRIC_RGB);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);

    // scanlinesize if determined by spp and bpp
    tsize_t linebytes = (tsize_t)image->GetWidth() * spp * bpp / 8;

    if ( (image->GetWidth() % 8 > 0) && (spp * bpp < 8) )
        linebytes+=1;

    unsigned char *buf;

    if (TIFFScanlineSize(tif) > linebytes || (spp * bpp < 24))
    {
        buf = (unsigned char *)_TIFFmalloc(TIFFScanlineSize(tif));
        if (!buf)
        {
            if (verbose)
                wxLogError( _("TIFF: Couldn't allocate memory.") );

            TIFFClose( tif );

            return false;
        }
    }
    else
    {
        buf = NULL;
    }

    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP,TIFFDefaultStripSize(tif, (uint32) -1));

    uint8 bitmask;

    unsigned char *ptr = image->GetData();
    for ( int row = 0; row < image->GetHeight(); row++ )
    {
        if ( buf )
        {
            if ( spp * bpp > 1 )
            {
                // color image
                memcpy(buf, ptr, image->GetWidth());
            }
            else // black and white image
            {
                for ( int column = 0; column < linebytes; column++ )
                {
                    uint8 reverse = 0;
                    bitmask = 1;
                    for ( int bp = 0; bp < 8; bp++ )
                    {
                        if ( ptr[column*24 + bp*3] > 0 )
                        {
                            // check only red as this is sufficient
                            reverse = reverse | 128 >> bp;
                        }

                        bitmask <<= 1;
                    }

                    buf[column] = reverse;
                }
            }
        }

        if ( TIFFWriteScanline(tif, buf ? buf : ptr, (uint32)row, 0) < 0 )
        {
            if (verbose)
                wxLogError( _("TIFF: Error writing image.") );

            TIFFClose( tif );
            if (buf)
                _TIFFfree(buf);

            return false;
        }

        ptr += image->GetWidth()*3;
    }

    (void) TIFFClose(tif);

    if (buf)
        _TIFFfree(buf);

    return true;
}

bool wxTIFFHandler::DoCanRead( wxInputStream& stream )
{
    unsigned char hdr[2];

    if ( !stream.Read(&hdr[0], WXSIZEOF(hdr)) )
        return false;

    return (hdr[0] == 'I' && hdr[1] == 'I') ||
           (hdr[0] == 'M' && hdr[1] == 'M');
}

#endif  // wxUSE_STREAMS

#endif  // wxUSE_LIBTIFF

