/////////////////////////////////////////////////////////////////////////////
// Name:        cursor.cpp
// Purpose:     wxCursor class
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// RCS-ID:      $Id: cursor.cpp,v 1.27 2005/03/04 18:33:51 RR Exp $
// Copyright:   (c) Stefan Csomor
// Licence:       wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "cursor.h"
#endif

#include "wx/wxprec.h"

#include "wx/app.h"
#include "wx/cursor.h"
#include "wx/icon.h"
#include "wx/image.h"
#include "wx/xpmdecod.h"

#include "wx/mac/private.h"

#if !USE_SHARED_LIBRARIES
IMPLEMENT_DYNAMIC_CLASS(wxCursor, wxBitmap)
#endif

class WXDLLEXPORT wxCursorRefData: public wxBitmapRefData
{
    DECLARE_NO_COPY_CLASS(wxCursorRefData)
        
    friend class WXDLLEXPORT wxBitmap;
    friend class WXDLLEXPORT wxCursor;
public:
    wxCursorRefData();
    ~wxCursorRefData();

protected:
    WXHCURSOR     m_hCursor;
    bool        m_disposeHandle;
    bool        m_releaseHandle;
    bool        m_isColorCursor ;
    long        m_themeCursor ;
};

#define M_CURSORDATA ((wxCursorRefData *)m_refData)
#define M_CURSORHANDLERDATA ((wxCursorRefData *)bitmap->m_refData)

const short kwxCursorBullseye = 0 ;
const short kwxCursorBlank = 1 ;
const short kwxCursorPencil = 2 ;
const short kwxCursorMagnifier = 3 ;
const short kwxCursorNoEntry = 4 ;
const short kwxCursorPaintBrush = 5 ;
const short kwxCursorPointRight = 6 ;
const short kwxCursorPointLeft = 7 ;
const short kwxCursorQuestionArrow = 8 ;
const short kwxCursorRightArrow = 9 ;
const short kwxCursorSizeNS = 10 ;
const short kwxCursorSize = 11 ;
const short kwxCursorSizeNESW = 12 ;
const short kwxCursorSizeNWSE = 13 ;
const short kwxCursorRoller = 14 ;
const short kwxCursorLast = kwxCursorRoller ;

Cursor gMacCursors[kwxCursorLast+1] =
{

{
{0x0000, 0x03E0, 0x0630, 0x0808, 0x1004, 0x31C6, 0x2362, 0x2222,
0x2362, 0x31C6, 0x1004, 0x0808, 0x0630, 0x03E0, 0x0000, 0x0000},
{0x0000, 0x03E0, 0x07F0, 0x0FF8, 0x1FFC, 0x3FFE, 0x3FFE, 0x3FFE,
0x3FFE, 0x3FFE, 0x1FFC, 0x0FF8, 0x07F0, 0x03E0, 0x0000, 0x0000},
{0x0007, 0x0008}
},

{
{0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
{0x0000, 0x0000}
},

{
{0x00F0, 0x0088, 0x0108, 0x0190, 0x0270, 0x0220, 0x0440, 0x0440,
0x0880, 0x0880, 0x1100, 0x1E00, 0x1C00, 0x1800, 0x1000, 0x0000},
{0x00F0, 0x00F8, 0x01F8, 0x01F0, 0x03F0, 0x03E0, 0x07C0, 0x07C0,
0x0F80, 0x0F80, 0x1F00, 0x1E00, 0x1C00, 0x1800, 0x1000, 0x0000},
{0x000E, 0x0003}
},

{
{0x0000, 0x1E00, 0x2100, 0x4080, 0x4080, 0x4080, 0x4080, 0x2180,
0x1FC0, 0x00E0, 0x0070, 0x0038, 0x001C, 0x000E, 0x0006, 0x0000},
{0x3F00, 0x7F80, 0xFFC0, 0xFFC0, 0xFFC0, 0xFFC0, 0xFFC0, 0x7FC0,
0x3FE0, 0x1FF0, 0x00F8, 0x007C, 0x003E, 0x001F, 0x000F, 0x0007},
{0x0004, 0x0004}
},

{
{0x0000, 0x07E0, 0x1FF0, 0x3838, 0x3C0C, 0x6E0E, 0x6706, 0x6386,
0x61C6, 0x60E6, 0x7076, 0x303C, 0x1C1C, 0x0FF8, 0x07E0, 0x0000},
{0x0540, 0x0FF0, 0x3FF8, 0x3C3C, 0x7E0E, 0xFF0F, 0x6F86, 0xE7C7,
0x63E6, 0xE1F7, 0x70FE, 0x707E, 0x3C3C, 0x1FFC, 0x0FF0, 0x0540},
{0x0007, 0x0007}
},

{
{0x0000, 0x0380, 0x0380, 0x0380, 0x0380, 0x0380, 0x0380, 0x0FE0,
0x1FF0, 0x1FF0, 0x0000, 0x1FF0, 0x1FF0, 0x1550, 0x1550, 0x1550},
{0x07C0, 0x07C0, 0x07C0, 0x07C0, 0x07C0, 0x07C0, 0x0FE0, 0x1FF0,
0x3FF8, 0x3FF8, 0x3FF8, 0x3FF8, 0x3FF8, 0x3FF8, 0x3FF8, 0x3FF8},
{0x000B, 0x0007}
},

{
{0x00C0, 0x0140, 0x0640, 0x08C0, 0x3180, 0x47FE, 0x8001, 0x8001,
0x81FE, 0x8040, 0x01C0, 0x0040, 0x03C0, 0xC080, 0x3F80, 0x0000},
{0x00C0, 0x01C0, 0x07C0, 0x0FC0, 0x3F80, 0x7FFE, 0xFFFF, 0xFFFF,
0xFFFE, 0xFFC0, 0xFFC0, 0xFFC0, 0xFFC0, 0xFF80, 0x3F80, 0x0000},
{0x0006, 0x000F}
},

{
{0x0100, 0x0280, 0x0260, 0x0310, 0x018C, 0x7FE3, 0x8000, 0x8000,
0x7F80, 0x0200, 0x0380, 0x0200, 0x03C0, 0x0107, 0x01F8, 0x0000},
{0x0100, 0x0380, 0x03E0, 0x03F0, 0x01FC, 0x7FFF, 0xFFFF, 0xFFFF,
0xFFFF, 0x03FF, 0x03FF, 0x03FF, 0x03FF, 0x01FF, 0x01F8, 0x0000},
{0x0006, 0x0000}
},

{
{0x0000, 0x4078, 0x60FC, 0x71CE, 0x7986, 0x7C06, 0x7E0E, 0x7F1C,
0x7FB8, 0x7C30, 0x6C30, 0x4600, 0x0630, 0x0330, 0x0300, 0x0000},
{0xC078, 0xE0FC, 0xF1FE, 0xFBFF, 0xFFCF, 0xFF8F, 0xFF1F, 0xFFBE,
0xFFFC, 0xFE78, 0xFF78, 0xEFF8, 0xCFF8, 0x87F8, 0x07F8, 0x0300},
{0x0001, 0x0001}
},

{
{0x0000, 0x0002, 0x0006, 0x000E, 0x001E, 0x003E, 0x007E, 0x00FE,
0x01FE, 0x003E, 0x0036, 0x0062, 0x0060, 0x00C0, 0x00C0, 0x0000},
{0x0003, 0x0007, 0x000F, 0x001F, 0x003F, 0x007F, 0x00FF, 0x01FF,
0x03FF, 0x07FF, 0x007F, 0x00F7, 0x00F3, 0x01E1, 0x01E0, 0x01C0},
{0x0001, 0x000E}
},

{
{0x0000, 0x0080, 0x01C0, 0x03E0, 0x0080, 0x0080, 0x0080, 0x1FFC,
0x1FFC, 0x0080, 0x0080, 0x0080, 0x03E0, 0x01C0, 0x0080, 0x0000},
{0x0080, 0x01C0, 0x03E0, 0x07F0, 0x0FF8, 0x01C0, 0x3FFE, 0x3FFE,
0x3FFE, 0x3FFE, 0x01C0, 0x0FF8, 0x07F0, 0x03E0, 0x01C0, 0x0080},
{0x0007, 0x0008}
},

{
{0x0000, 0x0080, 0x01C0, 0x03E0, 0x0080, 0x0888, 0x188C, 0x3FFE,
0x188C, 0x0888, 0x0080, 0x03E0, 0x01C0, 0x0080, 0x0000, 0x0000},
{0x0080, 0x01C0, 0x03E0, 0x07F0, 0x0BE8, 0x1DDC, 0x3FFE, 0x7FFF,
0x3FFE, 0x1DDC, 0x0BE8, 0x07F0, 0x03E0, 0x01C0, 0x0080, 0x0000},
{0x0007, 0x0008}
},

{
{0x0000, 0x001E, 0x000E, 0x060E, 0x0712, 0x03A0, 0x01C0, 0x00E0,
0x0170, 0x1238, 0x1C18, 0x1C00, 0x1E00, 0x0000, 0x0000, 0x0000},
{0x007F, 0x003F, 0x0E1F, 0x0F0F, 0x0F97, 0x07E3, 0x03E1, 0x21F0,
0x31F8, 0x3A7C, 0x3C3C, 0x3E1C, 0x3F00, 0x3F80, 0x0000, 0x0000},
{0x0006, 0x0009}
},

{
{0x0000, 0x7800, 0x7000, 0x7060, 0x48E0, 0x05C0, 0x0380, 0x0700,
0x0E80, 0x1C48, 0x1838, 0x0038, 0x0078, 0x0000, 0x0000, 0x0000},
{0xFE00, 0xFC00, 0xF870, 0xF0F0, 0xE9F0, 0xC7E0, 0x87C0, 0x0F84,
0x1F8C, 0x3E5C, 0x3C3C, 0x387C, 0x00FC, 0x01FC, 0x0000, 0x0000},
{0x0006, 0x0006}
},

{
{0x0006, 0x000E, 0x001C, 0x0018, 0x0020, 0x0040, 0x00F8, 0x0004,
0x1FF4, 0x200C, 0x2AA8, 0x1FF0, 0x1F80, 0x3800, 0x6000, 0x8000},
{0x000F, 0x001F, 0x003E, 0x007C, 0x0070, 0x00E0, 0x01FC, 0x3FF6,
0x7FF6, 0x7FFE, 0x7FFC, 0x7FF8, 0x3FF0, 0x7FC0, 0xF800, 0xE000},
{0x000A, 0x0006}
},

} ;

wxCursor    gMacCurrentCursor ;

CursHandle wxGetStockCursor( int number )
{
    wxASSERT_MSG( number >= 0 && number <=kwxCursorLast , wxT("invalid stock cursor id") ) ;
    CursHandle c = (CursHandle) NewHandle( sizeof(Cursor) ) ;
    memcpy( *c , &gMacCursors[number] , sizeof( Cursor ) ) ;
    return c ;    
}

wxCursorRefData::wxCursorRefData()
{
    SetWidth( 16 ); 
    SetHeight( 16 );
    m_hCursor = NULL ;
    m_disposeHandle = false ;
    m_releaseHandle = false ;
    m_isColorCursor = false ;
    m_themeCursor = -1 ;
}

wxCursorRefData::~wxCursorRefData()
{
    if ( m_isColorCursor )
    {
        ::DisposeCCursor( (CCrsrHandle) m_hCursor ) ;
    }
    else if ( m_disposeHandle )
    {
        ::DisposeHandle( (Handle ) m_hCursor ) ;
    } 
    else if ( m_releaseHandle )
    {
        // we don't release the resource since it may already
        // be in use again
    }
}

// Cursors
wxCursor::wxCursor()
{
}

wxCursor::wxCursor(const char WXUNUSED(bits)[], int WXUNUSED(width), int WXUNUSED(height),
    int WXUNUSED(hotSpotX), int WXUNUSED(hotSpotY), const char WXUNUSED(maskBits)[])
{
}

wxCursor::wxCursor( const wxImage &image )
{
#if wxUSE_IMAGE
    CreateFromImage( image ) ;
#endif
}

wxCursor::wxCursor(const char **bits) 
{
    (void) CreateFromXpm(bits);
}

wxCursor::wxCursor(char **bits) 
{
    (void) CreateFromXpm((const char **)bits);
}

bool wxCursor::CreateFromXpm(const char **bits)
{
#if wxUSE_IMAGE
    wxCHECK_MSG( bits != NULL, FALSE, wxT("invalid cursor data") )
    wxXPMDecoder decoder;
    wxImage img = decoder.ReadData(bits);
    wxCHECK_MSG( img.Ok(), FALSE, wxT("invalid cursor data") )    
	CreateFromImage( img ) ;
    return TRUE;
#else
    return FALSE;
#endif
}

WXHCURSOR wxCursor::GetHCURSOR() const 
{ 
    return (M_CURSORDATA ? M_CURSORDATA->m_hCursor : 0); 
}

bool wxCursor::Ok() const 
{ 
    return (m_refData != NULL && ( M_CURSORDATA->m_hCursor != NULL || M_CURSORDATA->m_themeCursor != -1 ) ) ; 
}

short GetCTabIndex( CTabHandle colors , RGBColor *col )
{
    short retval = 0 ;
    unsigned long bestdiff = 0xFFFF ;
    for ( int i = 0 ; i < (**colors).ctSize ; ++i )
    {
        unsigned long diff = abs(col->red -  (**colors).ctTable[i].rgb.red ) +
            abs(col->green -  (**colors).ctTable[i].rgb.green ) +
            abs(col->blue -  (**colors).ctTable[i].rgb.blue ) ;
        if ( diff < bestdiff )
        {
            bestdiff = diff ;
            retval = (**colors).ctTable[i].value ;
        }
    }
    return retval ;
}

#if wxUSE_IMAGE

void wxCursor::CreateFromImage(const wxImage & image) 
{
    m_refData = new wxCursorRefData;

    int w = 16;
    int h = 16;

    int hotSpotX = image.GetOptionInt(wxIMAGE_OPTION_CUR_HOTSPOT_X);
    int hotSpotY = image.GetOptionInt(wxIMAGE_OPTION_CUR_HOTSPOT_Y);
    int image_w = image.GetWidth();
    int image_h = image.GetHeight();

    wxASSERT_MSG( hotSpotX >= 0 && hotSpotX < image_w &&
                  hotSpotY >= 0 && hotSpotY < image_h,
                  _T("invalid cursor hot spot coordinates") );

    wxImage image16(image); // final image of correct size
 
    // if image is too small then place it in the center, resize it if too big
    if ((w > image_w) && (h > image_h))
    {
        wxPoint offset((w - image_w)/2, (h - image_h)/2);
        hotSpotX = hotSpotX + offset.x;
        hotSpotY = hotSpotY + offset.y;
    
        image16 = image.Size(wxSize(w, h), offset);
    }
    else if ((w != image_w) || (h != image_h))
    {
        hotSpotX = int(hotSpotX * double(w) / double(image_w)); 
        hotSpotY = int(hotSpotY * double(h) / double(image_h)); 

        image16 = image.Scale(w, h);
    }

    unsigned char * rgbBits = image16.GetData();
    bool bHasMask = image16.HasMask() ;

            
    PixMapHandle pm = (PixMapHandle) NewHandleClear( sizeof (PixMap))  ;
    short extent = 16 ;
    short bytesPerPixel = 1 ;
    short depth = 8 ;
    Rect bounds = { 0 , 0 , extent , extent } ;
    CCrsrHandle ch = (CCrsrHandle) NewHandleClear ( sizeof( CCrsr ) ) ;
    CTabHandle newColors = GetCTable( 8 ) ;
    HandToHand((Handle *) &newColors);
    // set the values to the indices
    for ( int i = 0 ; i < (**newColors).ctSize ; ++i )
    {
        (**newColors).ctTable[i].value = i ;
    }
    HLock( (Handle) ch) ;
    (**ch).crsrType = 0x8001 ; // color cursors
    (**ch).crsrMap = pm ; 
    short bytesPerRow = bytesPerPixel * extent ;

    (**pm).baseAddr = 0; 
    (**pm).rowBytes = bytesPerRow | 0x8000;
    (**pm).bounds = bounds;        
    (**pm).pmVersion = 0;           
    (**pm).packType = 0;            
    (**pm).packSize = 0;            
    (**pm).hRes = 0x00480000; /* 72 DPI default res */
    (**pm).vRes = 0x00480000; /* 72 DPI default res */
    (**pm).pixelSize = depth;
    (**pm).pixelType = 0;
    (**pm).cmpCount = 1; 
    (**pm).cmpSize = depth;
    (**pm).pmTable = newColors;

    (**ch).crsrData = NewHandleClear( extent * bytesPerRow ) ;  
    (**ch).crsrXData = NULL ;
    (**ch).crsrXValid = 0;
    (**ch).crsrXHandle = NULL;

    (**ch).crsrHotSpot.h = hotSpotX ;
    (**ch).crsrHotSpot.v = hotSpotY ;
    (**ch).crsrXTable = 0 ; 
    (**ch).crsrID = GetCTSeed() ;
    
    memset( (**ch).crsr1Data  , 0 , sizeof( Bits16 ) ) ;
    memset( (**ch).crsrMask , 0 , sizeof( Bits16 ) ) ;
    
    unsigned char mr = image16.GetMaskRed() ;
    unsigned char mg = image16.GetMaskGreen() ;
    unsigned char mb = image16.GetMaskBlue() ;
    for ( int y = 0 ; y < h ; ++y )
    {
        short rowbits = 0 ;
        short maskbits = 0 ;
        
        for ( int x = 0 ; x < w ; ++x )
        {
            long pos = (y * w + x) * 3;

            unsigned char r = rgbBits[pos] ;
            unsigned char g = rgbBits[pos+1] ;
            unsigned char b = rgbBits[pos+2] ;
            RGBColor col = { 0xFFFF ,0xFFFF, 0xFFFF } ;
            
            if ( bHasMask && r==mr && g==mg && b==mb )
            {
                // masked area, does not appear anywhere
            }
            else
            {
                if ( (int)r + (int)g + (int)b < 0x0200 )
                {
                    rowbits |= ( 1 << (15-x) ) ;
                }
                maskbits |= ( 1 << (15-x) ) ;
                
                col = *((RGBColor*) wxColor( r , g , b ).GetPixel()) ;
            }
            *((*(**ch).crsrData) + y * bytesPerRow + x) = 
                GetCTabIndex( newColors , &col) ;
        }
        (**ch).crsr1Data[y] = rowbits ;
        (**ch).crsrMask[y] = maskbits ;
    }
    if ( !bHasMask )
    {
        memcpy( (**ch).crsrMask , (**ch).crsr1Data , sizeof( Bits16) ) ;
    }
    
    HUnlock((Handle) ch) ;
    M_CURSORDATA->m_hCursor = ch ;
    M_CURSORDATA->m_isColorCursor = true ;
}

#endif //wxUSE_IMAGE

wxCursor::wxCursor(const wxString& cursor_file, long flags, int hotSpotX, int hotSpotY)
{
    m_refData = new wxCursorRefData;
    if ( flags == wxBITMAP_TYPE_MACCURSOR_RESOURCE )
    {
        Str255 theName ;
		wxMacStringToPascal( cursor_file , theName ) ;
        
        Handle resHandle = ::GetNamedResource( 'crsr' , theName ) ;
        if ( resHandle )
        {
            short theId = -1 ;
            OSType theType ;
            GetResInfo( resHandle , &theId , &theType , theName ) ;
            ReleaseResource( resHandle ) ;
            M_CURSORDATA->m_hCursor = GetCCursor( theId ) ;
            if ( M_CURSORDATA->m_hCursor )
                M_CURSORDATA->m_isColorCursor = true ;
        }
        else
        {       
            Handle resHandle = ::GetNamedResource( 'CURS' , theName ) ;
            if ( resHandle )
            {
                short theId = -1 ;
                OSType theType ;
                GetResInfo( resHandle , &theId , &theType , theName ) ;
                ReleaseResource( resHandle ) ;
                 M_CURSORDATA->m_hCursor = GetCursor( theId ) ;
                if ( M_CURSORDATA->m_hCursor )
                    M_CURSORDATA->m_releaseHandle = true ;
            }
        }
    }
    else
    {
#if wxUSE_IMAGE
        wxImage image ;
        image.LoadFile( cursor_file , flags ) ;
        if( image.Ok() )
        {
            image.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X,hotSpotX ) ;
            image.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y,hotSpotY ) ;
            delete m_refData ;
            CreateFromImage(image) ;
        }
#endif
    }
}

// Cursors by stock number
wxCursor::wxCursor(int cursor_type)
{
    m_refData = new wxCursorRefData;
    
    switch (cursor_type)
    {
    case wxCURSOR_COPY_ARROW:
        M_CURSORDATA->m_themeCursor = kThemeCopyArrowCursor ;
        break;
    case wxCURSOR_WAIT:
        M_CURSORDATA->m_themeCursor = kThemeWatchCursor ;
        break;
    case wxCURSOR_IBEAM:
        M_CURSORDATA->m_themeCursor = kThemeIBeamCursor ;
        break;
    case wxCURSOR_CROSS:
        M_CURSORDATA->m_themeCursor = kThemeCrossCursor;
        break;
    case wxCURSOR_SIZENWSE:
        {
            M_CURSORDATA->m_hCursor = wxGetStockCursor(kwxCursorSizeNWSE);
        }
        break;
    case wxCURSOR_SIZENESW:
        {
            M_CURSORDATA->m_hCursor = wxGetStockCursor(kwxCursorSizeNESW);
        }
        break;
    case wxCURSOR_SIZEWE:
        {
            M_CURSORDATA->m_themeCursor = kThemeResizeLeftRightCursor;
        }
        break;
    case wxCURSOR_SIZENS:
        {
            M_CURSORDATA->m_hCursor = wxGetStockCursor(kwxCursorSizeNS);
        }
        break;
    case wxCURSOR_SIZING:
        {
            M_CURSORDATA->m_hCursor = wxGetStockCursor(kwxCursorSize);
        }
        break;
    case wxCURSOR_HAND:
        {
            M_CURSORDATA->m_themeCursor = kThemePointingHandCursor;
        }
        break;
    case wxCURSOR_BULLSEYE:
        {
            M_CURSORDATA->m_hCursor = wxGetStockCursor(kwxCursorBullseye);
        }
        break;
    case wxCURSOR_PENCIL:
        {
            M_CURSORDATA->m_hCursor = wxGetStockCursor(kwxCursorPencil);
        }
        break;
    case wxCURSOR_MAGNIFIER:
        {
            M_CURSORDATA->m_hCursor = wxGetStockCursor(kwxCursorMagnifier);
        }
        break;
    case wxCURSOR_NO_ENTRY:
        {
            M_CURSORDATA->m_hCursor = wxGetStockCursor(kwxCursorNoEntry);
        }
        break;
    case wxCURSOR_WATCH:
        {
            M_CURSORDATA->m_themeCursor = kThemeWatchCursor;
            break;
        }
    case wxCURSOR_PAINT_BRUSH:
        {
            M_CURSORDATA->m_hCursor = wxGetStockCursor(kwxCursorPaintBrush);
            break;
        }
    case wxCURSOR_POINT_LEFT:
        {
            M_CURSORDATA->m_hCursor = wxGetStockCursor(kwxCursorPointLeft);
            break;
        }
    case wxCURSOR_POINT_RIGHT:
        {
            M_CURSORDATA->m_hCursor = wxGetStockCursor(kwxCursorPointRight);
            break;
        }
    case wxCURSOR_QUESTION_ARROW:
        {
            M_CURSORDATA->m_hCursor = wxGetStockCursor(kwxCursorQuestionArrow);
            break;
        }
    case wxCURSOR_BLANK:
        {
            M_CURSORDATA->m_hCursor = wxGetStockCursor(kwxCursorBlank);
            break;
        }
    case wxCURSOR_RIGHT_ARROW:
        {
            M_CURSORDATA->m_hCursor = wxGetStockCursor(kwxCursorRightArrow);
            break;
        }
    case wxCURSOR_SPRAYCAN:
        {
            M_CURSORDATA->m_hCursor = wxGetStockCursor(kwxCursorRoller);
            break;
        }
    case wxCURSOR_CHAR:
    case wxCURSOR_ARROW:
    case wxCURSOR_LEFT_BUTTON:
    case wxCURSOR_RIGHT_BUTTON:
    case wxCURSOR_MIDDLE_BUTTON:
    default:
        M_CURSORDATA->m_themeCursor = kThemeArrowCursor ;
        break;
    }
    if ( M_CURSORDATA->m_themeCursor == -1 )
        M_CURSORDATA->m_releaseHandle = true ;
}

void wxCursor::MacInstall() const 
{
    gMacCurrentCursor = *this ;
    if ( m_refData && M_CURSORDATA->m_themeCursor != -1 )
    {
        SetThemeCursor( M_CURSORDATA->m_themeCursor ) ;
    }
    else if ( m_refData && M_CURSORDATA->m_hCursor )
    {
        if ( M_CURSORDATA->m_isColorCursor )
            ::SetCCursor( (CCrsrHandle) M_CURSORDATA->m_hCursor ) ;
        else
            ::SetCursor( * (CursHandle) M_CURSORDATA->m_hCursor ) ;
    }
    else
    {
        SetThemeCursor( kThemeArrowCursor ) ;
    }
}

wxCursor::~wxCursor()
{
}

// Global cursor setting
void wxSetCursor(const wxCursor& cursor)
{
    cursor.MacInstall() ;
}


