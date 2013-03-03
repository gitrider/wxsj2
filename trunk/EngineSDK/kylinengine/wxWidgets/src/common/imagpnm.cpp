/////////////////////////////////////////////////////////////////////////////
// Name:        imagpnm.cpp
// Purpose:     wxImage PNM handler
// Author:      Sylvain Bougnoux
// RCS-ID:      $Id: imagpnm.cpp,v 1.25 2005/05/22 15:37:37 JS Exp $
// Copyright:   (c) Sylvain Bougnoux
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "imagpnm.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#  include "wx/setup.h"
#endif

#if wxUSE_IMAGE && wxUSE_PNM

#include "wx/imagpnm.h"
#include "wx/log.h"
#include "wx/intl.h"
#include "wx/txtstrm.h"

//-----------------------------------------------------------------------------
// wxBMPHandler
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxPNMHandler,wxImageHandler)

#if wxUSE_STREAMS

void Skip_Comment(wxInputStream &stream)
{
    wxTextInputStream text_stream(stream);

    if (stream.Peek()==wxT('#'))
    {
        text_stream.ReadLine();
        Skip_Comment(stream);
    }
}

bool wxPNMHandler::LoadFile( wxImage *image, wxInputStream& stream, bool verbose, int WXUNUSED(index) )
{
    wxUint32  width, height;
    wxUint16  maxval;
    char      c(0);

    image->Destroy();

    /*
     * Read the PNM header
     */

    wxBufferedInputStream buf_stream(stream);
    wxTextInputStream text_stream(buf_stream);

    Skip_Comment(buf_stream);
    if (buf_stream.GetC()==wxT('P')) c=buf_stream.GetC();

    switch (c)
    {
        case wxT('2'): // ASCII Grey 
        case wxT('3'): // ASCII RGB
        case wxT('5'): // RAW Grey
        case wxT('6'): break;
        default:
            if (verbose) wxLogError(_("PNM: File format is not recognized."));
            return false;
    }

    text_stream.ReadLine(); // for the \n
    Skip_Comment(buf_stream);
    text_stream >> width >> height ;
    Skip_Comment(buf_stream);
    text_stream >> maxval;

    //cout << line << " " << width << " " << height << " " << maxval << endl;
    image->Create( width, height );
    unsigned char *ptr = image->GetData();
    if (!ptr)
    {
        if (verbose)
           wxLogError( _("PNM: Couldn't allocate memory.") );
        return false;
    }


    if (c=='2') // Ascii GREY 
    {    
        wxUint32 value, size=width*height;
        for (wxUint32 i=0; i<size; ++i)
        {
            value=text_stream.Read32();
            *ptr++=(unsigned char)value; // R
            *ptr++=(unsigned char)value; // G 
            *ptr++=(unsigned char)value; // B
            if ( !buf_stream )
            {
                if (verbose) wxLogError(_("PNM: File seems truncated."));
                return false;
            }
        }
    }
   if (c=='3') // Ascii RBG
      {
        wxUint32 value, size=3*width*height;
        for (wxUint32 i=0; i<size; ++i)
          {
            //this is very slow !!!
            //I wonder how we can make any better ?
            value=text_stream.Read32();
            *ptr++=(unsigned char)value;

            if ( !buf_stream )
              {
                if (verbose) wxLogError(_("PNM: File seems truncated."));
                return false;
              }
          }
      }
   if (c=='5') // Raw GREY
    {    
        wxUint32 size=width*height;
        unsigned char value;
        for (wxUint32 i=0; i<size; ++i)
        {
            buf_stream.Read(&value,1);
            *ptr++=value; // R
            *ptr++=value; // G
            *ptr++=value; // B 
            if ( !buf_stream )
            {
                if (verbose) wxLogError(_("PNM: File seems truncated."));
                return false;
            }
        }
    }
    if (c=='6') // Raw RGB
      buf_stream.Read( ptr, 3*width*height );

    image->SetMask( false );

    const wxStreamError err = buf_stream.GetLastError();
    return err == wxSTREAM_NO_ERROR || err == wxSTREAM_EOF;
}

bool wxPNMHandler::SaveFile( wxImage *image, wxOutputStream& stream, bool WXUNUSED(verbose) )
{
    wxTextOutputStream text_stream(stream);

    //text_stream << "P6" << endl
    //<< image->GetWidth() << " " << image->GetHeight() << endl
    //<< "255" << endl;
    text_stream << wxT("P6\n") << image->GetWidth() << wxT(" ") << image->GetHeight() << wxT("\n255\n");
    stream.Write(image->GetData(),3*image->GetWidth()*image->GetHeight());

    return stream.IsOk();
}

bool wxPNMHandler::DoCanRead( wxInputStream& stream )
{
    Skip_Comment(stream);

    if ( stream.GetC() == 'P' )
    {
        switch (stream.GetC())
        {
            case '3':
            case '6':
                return true;
        }
    }

    return false;
}


#endif // wxUSE_STREAMS

#endif // wxUSE_PNM
