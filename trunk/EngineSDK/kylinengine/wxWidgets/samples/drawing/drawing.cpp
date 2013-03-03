/////////////////////////////////////////////////////////////////////////////
// Name:        drawing.cpp
// Purpose:     shows and tests wxDC features
// Author:      Robert Roebling
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id: drawing.cpp,v 1.78 2005/05/31 09:19:26 JS Exp $
// Copyright:   (c) Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(__APPLE__)
    #pragma implementation
    #pragma interface
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/colordlg.h"
#include "wx/image.h"
#include "wx/artprov.h"

// ----------------------------------------------------------------------------
// ressources
// ----------------------------------------------------------------------------

// the application icon
#if defined(__WXGTK__) || defined(__WXMOTIF__) || defined(__WXMAC__) || defined(__WXMGL__) || defined(__WXX11__)
    #include "mondrian.xpm"
#endif

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// what do we show on screen (there are too many shapes to put them all on
// screen simultaneously)
enum ScreenToShow
{
    Show_Default,
    Show_Text,
    Show_Lines,
    Show_Brushes,
    Show_Polygons,
    Show_Mask,
    Show_Ops,
    Show_Regions,
    Show_Circles
};

// ----------------------------------------------------------------------------
// global variables
// ----------------------------------------------------------------------------

static wxBitmap *gs_bmpNoMask = NULL,
                *gs_bmpWithColMask = NULL,
                *gs_bmpMask = NULL,
                *gs_bmpWithMask = NULL,
                *gs_bmp4 = NULL,
                *gs_bmp4_mono = NULL,
                *gs_bmp36 = NULL;

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

// Define a new application type, each program should derive a class from wxApp
class MyApp : public wxApp
{
public:
    // override base class virtuals
    // ----------------------------

    // this one is called on application startup and is a good place for the app
    // initialization (doing it here and not in the ctor allows to have an error
    // return: if OnInit() returns false, the application terminates)
    virtual bool OnInit();

    virtual int OnExit() { DeleteBitmaps(); return 0; }

protected:
    void DeleteBitmaps();

    bool LoadImages();
};

class MyCanvas;

// Define a new frame type: this is going to be our main frame
class MyFrame : public wxFrame
{
public:
    // ctor(s)
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

    // event handlers (these functions should _not_ be virtual)
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnClip(wxCommandEvent& event);
    void OnShow(wxCommandEvent &event);
    void OnOption(wxCommandEvent &event);

#if wxUSE_COLOURDLG
    wxColour SelectColour();
#endif // wxUSE_COLOURDLG
    void PrepareDC(wxDC& dc);

    int         m_backgroundMode;
    int         m_textureBackground;
    int         m_mapMode;
    double      m_xUserScale;
    double      m_yUserScale;
    int         m_xLogicalOrigin;
    int         m_yLogicalOrigin;
    bool        m_xAxisReversed,
                m_yAxisReversed;
    wxColour    m_colourForeground,    // these are _text_ colours
                m_colourBackground;
    wxBrush     m_backgroundBrush;
    MyCanvas   *m_canvas;

private:
    // any class wishing to process wxWidgets events must use this macro
    DECLARE_EVENT_TABLE()
};

// define a scrollable canvas for drawing onto
class MyCanvas: public wxScrolledWindow
{
public:
    MyCanvas( MyFrame *parent );

    void OnPaint(wxPaintEvent &event);
    void OnMouseMove(wxMouseEvent &event);

    void ToShow(ScreenToShow show) { m_show = show; Refresh(); }

    // set or remove the clipping region
    void Clip(bool clip) { m_clip = clip; Refresh(); }

protected:
    void DrawTestLines( int x, int y, int width, wxDC &dc );
    void DrawTestPoly(wxDC& dc);
    void DrawTestBrushes(wxDC& dc);
    void DrawText(wxDC& dc);
    void DrawImages(wxDC& dc);
    void DrawWithLogicalOps(wxDC& dc);
    void DrawRegions(wxDC& dc);
    void DrawCircles(wxDC& dc);
    void DrawDefault(wxDC& dc);

    void DrawRegionsHelper(wxDC& dc, wxCoord x, bool firstTime);

private:
    MyFrame *m_owner;

    ScreenToShow m_show;
    wxBitmap     m_smile_bmp;
    wxIcon       m_std_icon;
    bool         m_clip;

    DECLARE_EVENT_TABLE()
};

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
    // menu items
    File_Quit = 1,
    File_About,

    MenuShow_First,
    File_ShowDefault = MenuShow_First,
    File_ShowText,
    File_ShowLines,
    File_ShowBrushes,
    File_ShowPolygons,
    File_ShowMask,
    File_ShowOps,
    File_ShowRegions,
    File_ShowCircles,
    MenuShow_Last = File_ShowCircles,

    File_Clip,

    MenuOption_First,

    MapMode_Text = MenuOption_First,
    MapMode_Lometric,
    MapMode_Twips,
    MapMode_Points,
    MapMode_Metric,

    UserScale_StretchHoriz,
    UserScale_ShrinkHoriz,
    UserScale_StretchVertic,
    UserScale_ShrinkVertic,
    UserScale_Restore,

    AxisMirror_Horiz,
    AxisMirror_Vertic,

    LogicalOrigin_MoveDown,
    LogicalOrigin_MoveUp,
    LogicalOrigin_MoveLeft,
    LogicalOrigin_MoveRight,
    LogicalOrigin_Set,
    LogicalOrigin_Restore,

#if wxUSE_COLOURDLG
    Colour_TextForeground,
    Colour_TextBackground,
    Colour_Background,
#endif // wxUSE_COLOURDLG
    Colour_BackgroundMode,
    Colour_TextureBackgound,

    MenuOption_Last = Colour_TextureBackgound
};

// ----------------------------------------------------------------------------
// event tables and other macros for wxWidgets
// ----------------------------------------------------------------------------


// Create a new application object: this macro will allow wxWidgets to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also declares the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)
IMPLEMENT_APP(MyApp)

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

bool MyApp::LoadImages()
{
    gs_bmpNoMask = new wxBitmap;
    gs_bmpWithColMask = new wxBitmap;
    gs_bmpMask = new wxBitmap;
    gs_bmpWithMask = new wxBitmap;
    gs_bmp4 = new wxBitmap;
    gs_bmp4_mono = new wxBitmap;
    gs_bmp36 = new wxBitmap;

    wxPathList pathList;
    pathList.Add(_T("."));
    pathList.Add(_T(".."));

    wxString path = pathList.FindValidPath(_T("pat4.bmp"));
    if ( !path )
        return false;

    /* 4 colour bitmap */
    gs_bmp4->LoadFile(path, wxBITMAP_TYPE_BMP);
    /* turn into mono-bitmap */
    gs_bmp4_mono->LoadFile(path, wxBITMAP_TYPE_BMP);
    wxMask* mask4 = new wxMask(*gs_bmp4_mono, *wxBLACK);
    gs_bmp4_mono->SetMask(mask4);

    path = pathList.FindValidPath(_T("pat36.bmp"));
    if ( !path )
        return false;
    gs_bmp36->LoadFile(path, wxBITMAP_TYPE_BMP);
    wxMask* mask36 = new wxMask(*gs_bmp36, *wxBLACK);
    gs_bmp36->SetMask(mask36);

    path = pathList.FindValidPath(_T("image.bmp"));
    if ( !path )
        return false;
    gs_bmpNoMask->LoadFile(path, wxBITMAP_TYPE_BMP);
    gs_bmpWithMask->LoadFile(path, wxBITMAP_TYPE_BMP);
    gs_bmpWithColMask->LoadFile(path, wxBITMAP_TYPE_BMP);

    path = pathList.FindValidPath(_T("mask.bmp"));
    if ( !path )
        return false;
    gs_bmpMask->LoadFile(path, wxBITMAP_TYPE_BMP);

    wxMask *mask = new wxMask(*gs_bmpMask, *wxBLACK);
    gs_bmpWithMask->SetMask(mask);

    mask = new wxMask(*gs_bmpWithColMask, *wxWHITE);
    gs_bmpWithColMask->SetMask(mask);

    return true;
}

// `Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
    // Create the main application window
    MyFrame *frame = new MyFrame(_T("Drawing sample"),
                                 wxPoint(50, 50), wxSize(550, 340));

    // Show it and tell the application that it's our main window
    frame->Show(true);
    SetTopWindow(frame);

    if ( !LoadImages() )
    {
        wxLogError(wxT("Can't load one of the bitmap files needed ")
                   wxT("for this sample from the current or parent ")
                   wxT("directory, please copy them there."));

        // stop here
        DeleteBitmaps();

        return false;
    }

    // ok, continue
    return true;
}

void MyApp::DeleteBitmaps()
{
    delete gs_bmpNoMask;
    delete gs_bmpWithColMask;
    delete gs_bmpMask;
    delete gs_bmpWithMask;
    delete gs_bmp4;
    delete gs_bmp4_mono;
    delete gs_bmp36;

    gs_bmpNoMask = NULL;
    gs_bmpWithColMask = NULL;
    gs_bmpMask = NULL;
    gs_bmpWithMask = NULL;
    gs_bmp4 = NULL;
    gs_bmp4_mono = NULL;
    gs_bmp36 = NULL;
}

// ----------------------------------------------------------------------------
// MyCanvas
// ----------------------------------------------------------------------------

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(MyCanvas, wxScrolledWindow)
    EVT_PAINT  (MyCanvas::OnPaint)
    EVT_MOTION (MyCanvas::OnMouseMove)
END_EVENT_TABLE()

#include "smile.xpm"

MyCanvas::MyCanvas(MyFrame *parent)
        : wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                           wxHSCROLL | wxVSCROLL | wxNO_FULL_REPAINT_ON_RESIZE)
{
    m_owner = parent;
    m_show = Show_Default;
    m_smile_bmp = wxBitmap(smile_xpm);
    m_std_icon = wxArtProvider::GetIcon(wxART_INFORMATION);
    m_clip = false;
}

void MyCanvas::DrawTestBrushes(wxDC& dc)
{
    static const wxCoord WIDTH = 200;
    static const wxCoord HEIGHT = 80;

    wxCoord x = 10,
            y = 10;

    dc.SetBrush(wxBrush(*wxGREEN, wxSOLID));
    dc.DrawRectangle(x, y, WIDTH, HEIGHT);
    dc.DrawText(_T("Solid green"), x + 10, y + 10);

    y += HEIGHT;
    dc.SetBrush(wxBrush(*wxRED, wxCROSSDIAG_HATCH));
    dc.DrawRectangle(x, y, WIDTH, HEIGHT);
    dc.DrawText(_T("Hatched red"), x + 10, y + 10);

    y += HEIGHT;
    dc.SetBrush(wxBrush(*gs_bmpMask));
    dc.DrawRectangle(x, y, WIDTH, HEIGHT);
    dc.DrawText(_T("Stipple mono"), x + 10, y + 10);

    y += HEIGHT;
    dc.SetBrush(wxBrush(*gs_bmpNoMask));
    dc.DrawRectangle(x, y, WIDTH, HEIGHT);
    dc.DrawText(_T("Stipple colour"), x + 10, y + 10);
}

void MyCanvas::DrawTestPoly(wxDC& dc)
{
    wxBrush brushHatch(*wxRED, wxFDIAGONAL_HATCH);
    dc.SetBrush(brushHatch);

    wxPoint star[5];
    star[0] = wxPoint(100, 60);
    star[1] = wxPoint(60, 150);
    star[2] = wxPoint(160, 100);
    star[3] = wxPoint(40, 100);
    star[4] = wxPoint(140, 150);

    dc.DrawText(_T("You should see two (irregular) stars below, the left one ")
                _T("hatched"), 10, 10);
    dc.DrawText(_T("except for the central region and the right ")
                _T("one entirely hatched"), 10, 30);
    dc.DrawText(_T("The third star only has a hatched outline"), 10, 50);

    dc.DrawPolygon(WXSIZEOF(star), star, 0, 30);
    dc.DrawPolygon(WXSIZEOF(star), star, 160, 30, wxWINDING_RULE);

    wxPoint star2[10];
    star2[0] = wxPoint(0, 100);
    star2[1] = wxPoint(-59, -81);
    star2[2] = wxPoint(95, 31);
    star2[3] = wxPoint(-95, 31);
    star2[4] = wxPoint(59, -81);
    star2[5] = wxPoint(0, 80);
    star2[6] = wxPoint(-47, -64);
    star2[7] = wxPoint(76, 24);
    star2[8] = wxPoint(-76, 24);
    star2[9] = wxPoint(47, -64);
    int count[2] = {5, 5};

    dc.DrawPolyPolygon(WXSIZEOF(count), count, star2, 450, 150);
}

void MyCanvas::DrawTestLines( int x, int y, int width, wxDC &dc )
{
    dc.SetPen( wxPen( wxT("black"), width, wxSOLID) );
    dc.SetBrush( *wxRED_BRUSH );
    dc.DrawText(wxString::Format(wxT("Testing lines of width %d"), width), x + 10, y - 10);
    dc.DrawRectangle( x+10, y+10, 100, 190 );

    dc.DrawText(_T("Solid/dot/short dash/long dash/dot dash"), x + 150, y + 10);
    dc.SetPen( wxPen( wxT("black"), width, wxSOLID) );
    dc.DrawLine( x+20, y+20, 100, y+20 );
    dc.SetPen( wxPen( wxT("black"), width, wxDOT) );
    dc.DrawLine( x+20, y+30, 100, y+30 );
    dc.SetPen( wxPen( wxT("black"), width, wxSHORT_DASH) );
    dc.DrawLine( x+20, y+40, 100, y+40 );
    dc.SetPen( wxPen( wxT("black"), width, wxLONG_DASH) );
    dc.DrawLine( x+20, y+50, 100, y+50 );
    dc.SetPen( wxPen( wxT("black"), width, wxDOT_DASH) );
    dc.DrawLine( x+20, y+60, 100, y+60 );

    dc.DrawText(_T("Misc hatches"), x + 150, y + 70);
    dc.SetPen( wxPen( wxT("black"), width, wxBDIAGONAL_HATCH) );
    dc.DrawLine( x+20, y+70, 100, y+70 );
    dc.SetPen( wxPen( wxT("black"), width, wxCROSSDIAG_HATCH) );
    dc.DrawLine( x+20, y+80, 100, y+80 );
    dc.SetPen( wxPen( wxT("black"), width, wxFDIAGONAL_HATCH) );
    dc.DrawLine( x+20, y+90, 100, y+90 );
    dc.SetPen( wxPen( wxT("black"), width, wxCROSS_HATCH) );
    dc.DrawLine( x+20, y+100, 100, y+100 );
    dc.SetPen( wxPen( wxT("black"), width, wxHORIZONTAL_HATCH) );
    dc.DrawLine( x+20, y+110, 100, y+110 );
    dc.SetPen( wxPen( wxT("black"), width, wxVERTICAL_HATCH) );
    dc.DrawLine( x+20, y+120, 100, y+120 );

    dc.DrawText(_T("User dash"), x + 150, y + 140);
    wxPen ud( wxT("black"), width, wxUSER_DASH );
    wxDash dash1[6];
    dash1[0] = 8;  // Long dash  <---------+
    dash1[1] = 2;  // Short gap            |
    dash1[2] = 3;  // Short dash           |
    dash1[3] = 2;  // Short gap            |
    dash1[4] = 3;  // Short dash           |
    dash1[5] = 2;  // Short gap and repeat +
    ud.SetDashes( 6, dash1 );
    dc.SetPen( ud );
    dc.DrawLine( x+20, y+140, 100, y+140 );
    dash1[0] = 5;  // Make first dash shorter
    ud.SetDashes( 6, dash1 );
    dc.SetPen( ud );
    dc.DrawLine( x+20, y+150, 100, y+150 );
    dash1[2] = 5;  // Make second dash longer
    ud.SetDashes( 6, dash1 );
    dc.SetPen( ud );
    dc.DrawLine( x+20, y+160, 100, y+160 );
    dash1[4] = 5;  // Make third dash longer
    ud.SetDashes( 6, dash1 );
    dc.SetPen( ud );
    dc.DrawLine( x+20, y+170, 100, y+170 );
}

void MyCanvas::DrawDefault(wxDC& dc)
{
    // mark the origin
    dc.DrawCircle(0, 0, 10);

#if !wxMAC_USE_CORE_GRAPHICS
    // GetPixel and FloodFill not supported by Mac OS X CoreGraphics
    // (FloodFill uses Blit from a non-wxMemoryDC)
    //flood fill using brush, starting at 1,1 and replacing whatever colour we find there
    dc.SetBrush(wxBrush(wxColour(128,128,0), wxSOLID));

    wxColour tmpColour ;
    dc.GetPixel(1,1, &tmpColour);
    dc.FloodFill(1,1, tmpColour, wxFLOOD_SURFACE);
#endif

    dc.DrawCheckMark(5, 80, 15, 15);
    dc.DrawCheckMark(25, 80, 30, 30);
    dc.DrawCheckMark(60, 80, 60, 60);

    // this is the test for "blitting bitmap into DC damages selected brush" bug
    wxCoord rectSize = m_std_icon.GetWidth() + 10;
    wxCoord x = 100;
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush( *wxGREEN_BRUSH );
    dc.DrawRectangle(x, 10, rectSize, rectSize);
    dc.DrawBitmap(m_std_icon, x + 5, 15, true);
    x += rectSize + 10;
    dc.DrawRectangle(x, 10, rectSize, rectSize);
    dc.DrawIcon(m_std_icon, x + 5, 15);
    x += rectSize + 10;
    dc.DrawRectangle(x, 10, rectSize, rectSize);

    // test for "transparent" bitmap drawing (it intersects with the last
    // rectangle above)
    //dc.SetBrush( *wxTRANSPARENT_BRUSH );

    if (m_smile_bmp.Ok())
        dc.DrawBitmap(m_smile_bmp, x + rectSize - 20, rectSize - 10, true);

    dc.SetBrush( *wxBLACK_BRUSH );
    dc.DrawRectangle( 0, 160, 1000, 300 );

    // draw lines
    wxBitmap bitmap(20,70);
    wxMemoryDC memdc;
    memdc.SelectObject( bitmap );
    memdc.SetBrush( *wxBLACK_BRUSH );
    memdc.SetPen( *wxWHITE_PEN );
    memdc.DrawRectangle(0,0,20,70);
    memdc.DrawLine( 10,0,10,70 );

    // to the right
    wxPen pen = *wxRED_PEN;
    memdc.SetPen(pen);
    memdc.DrawLine( 10, 5,10, 5 );
    memdc.DrawLine( 10,10,11,10 );
    memdc.DrawLine( 10,15,12,15 );
    memdc.DrawLine( 10,20,13,20 );

/*
    memdc.SetPen(*wxRED_PEN);
    memdc.DrawLine( 12, 5,12, 5 );
    memdc.DrawLine( 12,10,13,10 );
    memdc.DrawLine( 12,15,14,15 );
    memdc.DrawLine( 12,20,15,20 );
*/

    // same to the left
    memdc.DrawLine( 10,25,10,25 );
    memdc.DrawLine( 10,30, 9,30 );
    memdc.DrawLine( 10,35, 8,35 );
    memdc.DrawLine( 10,40, 7,40 );

    // XOR draw lines
    dc.SetPen(*wxWHITE_PEN);
    memdc.SetLogicalFunction( wxINVERT );
    memdc.SetPen( *wxWHITE_PEN );
    memdc.DrawLine( 10,50,10,50 );
    memdc.DrawLine( 10,55,11,55 );
    memdc.DrawLine( 10,60,12,60 );
    memdc.DrawLine( 10,65,13,65 );

    memdc.DrawLine( 12,50,12,50 );
    memdc.DrawLine( 12,55,13,55 );
    memdc.DrawLine( 12,60,14,60 );
    memdc.DrawLine( 12,65,15,65 );

    memdc.SelectObject( wxNullBitmap );
    dc.DrawBitmap( bitmap, 10, 170 );
    wxImage image = bitmap.ConvertToImage();
    image.Rescale( 60,210 );
    bitmap = wxBitmap(image);
    dc.DrawBitmap( bitmap, 50, 170 );

    // test the rectangle outline drawing - there should be one pixel between
    // the rect and the lines
    dc.SetPen(*wxWHITE_PEN);
    dc.SetBrush( *wxTRANSPARENT_BRUSH );
    dc.DrawRectangle(150, 170, 49, 29);
    dc.DrawRectangle(200, 170, 49, 29);
    dc.SetPen(*wxWHITE_PEN);
    dc.DrawLine(250, 210, 250, 170);
    dc.DrawLine(260, 200, 150, 200);

    // test the rectangle filled drawing - there should be one pixel between
    // the rect and the lines
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush( *wxWHITE_BRUSH );
    dc.DrawRectangle(300, 170, 49, 29);
    dc.DrawRectangle(350, 170, 49, 29);
    dc.SetPen(*wxWHITE_PEN);
    dc.DrawLine(400, 170, 400, 210);
    dc.DrawLine(300, 200, 410, 200);

    // a few more tests of this kind
    dc.SetPen(*wxRED_PEN);
    dc.SetBrush( *wxWHITE_BRUSH );
    dc.DrawRectangle(300, 220, 1, 1);
    dc.DrawRectangle(310, 220, 2, 2);
    dc.DrawRectangle(320, 220, 3, 3);
    dc.DrawRectangle(330, 220, 4, 4);

    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush( *wxWHITE_BRUSH );
    dc.DrawRectangle(300, 230, 1, 1);
    dc.DrawRectangle(310, 230, 2, 2);
    dc.DrawRectangle(320, 230, 3, 3);
    dc.DrawRectangle(330, 230, 4, 4);

    // and now for filled rect with outline
    dc.SetPen(*wxRED_PEN);
    dc.SetBrush( *wxWHITE_BRUSH );
    dc.DrawRectangle(500, 170, 49, 29);
    dc.DrawRectangle(550, 170, 49, 29);
    dc.SetPen(*wxWHITE_PEN);
    dc.DrawLine(600, 170, 600, 210);
    dc.DrawLine(500, 200, 610, 200);

    // test the rectangle outline drawing - there should be one pixel between
    // the rect and the lines
    dc.SetPen(*wxWHITE_PEN);
    dc.SetBrush( *wxTRANSPARENT_BRUSH );
    dc.DrawRoundedRectangle(150, 270, 49, 29, 6);
    dc.DrawRoundedRectangle(200, 270, 49, 29, 6);
    dc.SetPen(*wxWHITE_PEN);
    dc.DrawLine(250, 270, 250, 310);
    dc.DrawLine(150, 300, 260, 300);

    // test the rectangle filled drawing - there should be one pixel between
    // the rect and the lines
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush( *wxWHITE_BRUSH );
    dc.DrawRoundedRectangle(300, 270, 49, 29, 6);
    dc.DrawRoundedRectangle(350, 270, 49, 29, 6);
    dc.SetPen(*wxWHITE_PEN);
    dc.DrawLine(400, 270, 400, 310);
    dc.DrawLine(300, 300, 410, 300);

    // Added by JACS to demonstrate bizarre behaviour.
    // With a size of 70, we get a missing red RHS,
    // and the height is too small, so we get yellow
    // showing. With a size of 40, it draws as expected:
    // it just shows a white rectangle with red outline.
    int totalWidth = 70;
    int totalHeight = 70;
    wxBitmap bitmap2(totalWidth, totalHeight);

    wxMemoryDC memdc2;
    memdc2.SelectObject(bitmap2);

    wxColour clr(255, 255, 0);
    wxBrush yellowBrush(clr, wxSOLID);
    memdc2.SetBackground(yellowBrush);
    memdc2.Clear();

    wxPen yellowPen(clr, 1, wxSOLID);

    // Now draw a white rectangle with red outline. It should
    // entirely eclipse the yellow background.
    memdc2.SetPen(*wxRED_PEN);
    memdc2.SetBrush(*wxWHITE_BRUSH);

    memdc2.DrawRectangle(0, 0, totalWidth, totalHeight);

    memdc2.SetPen(wxNullPen);
    memdc2.SetBrush(wxNullBrush);
    memdc2.SelectObject(wxNullBitmap);

    dc.DrawBitmap(bitmap2, 500, 270);

    // Repeat, but draw directly on dc
    // Draw a yellow rectangle filling the bitmap

    x = 600; int y = 270;
    dc.SetPen(yellowPen);
    dc.SetBrush(yellowBrush);
    dc.DrawRectangle(x, y, totalWidth, totalHeight);

    // Now draw a white rectangle with red outline. It should
    // entirely eclipse the yellow background.
    dc.SetPen(*wxRED_PEN);
    dc.SetBrush(*wxWHITE_BRUSH);

    dc.DrawRectangle(x, y, totalWidth, totalHeight);
}

void MyCanvas::DrawText(wxDC& dc)
{
    // set underlined font for testing
    dc.SetFont( wxFont(12, wxMODERN, wxNORMAL, wxNORMAL, true) );
    dc.DrawText( _T("This is text"), 110, 10 );
    dc.DrawRotatedText( _T("That is text"), 20, 10, -45 );

    // use wxSWISS_FONT and not wxNORMAL_FONT as the latter can't be rotated
    // under Win9x (it is not TrueType)
    dc.SetFont( *wxSWISS_FONT );

    wxString text;
    dc.SetBackgroundMode(wxTRANSPARENT);

    for ( int n = -180; n < 180; n += 30 )
    {
        text.Printf(wxT("     %d rotated text"), n);
        dc.DrawRotatedText(text , 400, 400, n);
    }

    dc.SetFont( wxFont( 18, wxSWISS, wxNORMAL, wxNORMAL ) );

    dc.DrawText( _T("This is Swiss 18pt text."), 110, 40 );

    long length;
    long height;
    long descent;
    dc.GetTextExtent( _T("This is Swiss 18pt text."), &length, &height, &descent );
    text.Printf( wxT("Dimensions are length %ld, height %ld, descent %ld"), length, height, descent );
    dc.DrawText( text, 110, 80 );

    text.Printf( wxT("CharHeight() returns: %d"), dc.GetCharHeight() );
    dc.DrawText( text, 110, 120 );

    dc.DrawRectangle( 100, 40, 4, height );

    // test the logical function effect
    wxCoord y = 150;
    dc.SetLogicalFunction(wxINVERT);
    dc.DrawText( _T("There should be no text below"), 110, 150 );
    dc.DrawRectangle( 110, y, 100, height );

    // twice drawn inverted should result in invisible
    y += height;
    dc.DrawText( _T("Invisible text"), 110, y );
    dc.DrawRectangle( 110, y, 100, height );
    dc.DrawText( _T("Invisible text"), 110, y );
    dc.DrawRectangle( 110, y, 100, height );
    dc.SetLogicalFunction(wxCOPY);

    y += height;
    dc.DrawRectangle( 110, y, 100, height );
    dc.DrawText( _T("Visible text"), 110, y );
}

static const struct
{
    const wxChar *name;
    int           rop;
} rasterOperations[] =
{
    { wxT("wxAND"),          wxAND           },
    { wxT("wxAND_INVERT"),   wxAND_INVERT    },
    { wxT("wxAND_REVERSE"),  wxAND_REVERSE   },
    { wxT("wxCLEAR"),        wxCLEAR         },
    { wxT("wxCOPY"),         wxCOPY          },
    { wxT("wxEQUIV"),        wxEQUIV         },
    { wxT("wxINVERT"),       wxINVERT        },
    { wxT("wxNAND"),         wxNAND          },
    { wxT("wxNO_OP"),        wxNO_OP         },
    { wxT("wxOR"),           wxOR            },
    { wxT("wxOR_INVERT"),    wxOR_INVERT     },
    { wxT("wxOR_REVERSE"),   wxOR_REVERSE    },
    { wxT("wxSET"),          wxSET           },
    { wxT("wxSRC_INVERT"),   wxSRC_INVERT    },
    { wxT("wxXOR"),          wxXOR           },
};

void MyCanvas::DrawImages(wxDC& dc)
{
    dc.DrawText(_T("original image"), 0, 0);
    dc.DrawBitmap(*gs_bmpNoMask, 0, 20, 0);
    dc.DrawText(_T("with colour mask"), 0, 100);
    dc.DrawBitmap(*gs_bmpWithColMask, 0, 120, true);
    dc.DrawText(_T("the mask image"), 0, 200);
    dc.DrawBitmap(*gs_bmpMask, 0, 220, 0);
    dc.DrawText(_T("masked image"), 0, 300);
    dc.DrawBitmap(*gs_bmpWithMask, 0, 320, true);

    int cx = gs_bmpWithColMask->GetWidth(),
        cy = gs_bmpWithColMask->GetHeight();

    wxMemoryDC memDC;
    for ( size_t n = 0; n < WXSIZEOF(rasterOperations); n++ )
    {
        wxCoord x = 120 + 150*(n%4),
                y =  20 + 100*(n/4);

        dc.DrawText(rasterOperations[n].name, x, y - 20);
        memDC.SelectObject(*gs_bmpWithColMask);
        dc.Blit(x, y, cx, cy, &memDC, 0, 0, rasterOperations[n].rop, true);
    }
}

void MyCanvas::DrawWithLogicalOps(wxDC& dc)
{
    static const wxCoord w = 60;
    static const wxCoord h = 60;

    // reuse the text colour here
    dc.SetPen(wxPen(m_owner->m_colourForeground, 1, wxSOLID));
    dc.SetBrush(*wxTRANSPARENT_BRUSH);

    size_t n;
    for ( n = 0; n < WXSIZEOF(rasterOperations); n++ )
    {
        wxCoord x = 20 + 150*(n%4),
                y = 20 + 100*(n/4);

        dc.DrawText(rasterOperations[n].name, x, y - 20);
        dc.SetLogicalFunction(rasterOperations[n].rop);
        dc.DrawRectangle(x, y, w, h);
        dc.DrawLine(x, y, x + w, y + h);
        dc.DrawLine(x + w, y, x, y + h);
    }

    // now some filled rectangles
    dc.SetBrush(wxBrush(m_owner->m_colourForeground, wxSOLID));

    for ( n = 0; n < WXSIZEOF(rasterOperations); n++ )
    {
        wxCoord x = 20 + 150*(n%4),
                y = 500 + 100*(n/4);

        dc.DrawText(rasterOperations[n].name, x, y - 20);
        dc.SetLogicalFunction(rasterOperations[n].rop);
        dc.DrawRectangle(x, y, w, h);
    }
}

void MyCanvas::DrawCircles(wxDC& dc)
{
    int x = 100,
        y = 100,
        r = 20;

    dc.DrawText(_T("Some circles"), 0, y);
    dc.DrawCircle(x, y, r);
    dc.DrawCircle(x + 2*r, y, r);
    dc.DrawCircle(x + 4*r, y, r);

    y += 2*r;
    dc.DrawText(_T("And ellipses"), 0, y);
    dc.DrawEllipse(x - r, y, 2*r, r);
    dc.DrawEllipse(x + r, y, 2*r, r);
    dc.DrawEllipse(x + 3*r, y, 2*r, r);

    y += 2*r;
    dc.DrawText(_T("And arcs"), 0, y);
    dc.DrawArc(x - r, y, x + r, y, x, y);
    dc.DrawArc(x + 4*r, y, x + 2*r, y, x + 3*r, y);
    dc.DrawArc(x + 5*r, y, x + 5*r, y, x + 6*r, y);

    y += 2*r;
    dc.DrawEllipticArc(x - r, y, 2*r, r, 0, 90);
    dc.DrawEllipticArc(x + r, y, 2*r, r, 90, 180);
    dc.DrawEllipticArc(x + 3*r, y, 2*r, r, 180, 270);
    dc.DrawEllipticArc(x + 5*r, y, 2*r, r, 270, 360);
}

void MyCanvas::DrawRegions(wxDC& dc)
{
    dc.DrawText(_T("You should see a red rect partly covered by a cyan one ")
                _T("on the left"), 10, 5);
    dc.DrawText(_T("and 5 smileys from which 4 are partially clipped on the right"),
                10, 5 + dc.GetCharHeight());
    dc.DrawText(_T("The second copy should be identical but right part of it ")
                _T("should be offset by 10 pixels."),
                10, 5 + 2*dc.GetCharHeight());

    DrawRegionsHelper(dc, 10, true);
    DrawRegionsHelper(dc, 350, false);
}

void MyCanvas::DrawRegionsHelper(wxDC& dc, wxCoord x, bool firstTime)
{
    wxCoord y = 100;

    dc.DestroyClippingRegion();
    dc.SetBrush( *wxWHITE_BRUSH );
    dc.SetPen( *wxTRANSPARENT_PEN );
    dc.DrawRectangle( x, y, 310, 310 );

    dc.SetClippingRegion( x + 10, y + 10, 100, 270 );

    dc.SetBrush( *wxRED_BRUSH );
    dc.DrawRectangle( x, y, 310, 310 );

    dc.SetClippingRegion( x + 10, y + 10, 100, 100 );

    dc.SetBrush( *wxCYAN_BRUSH );
    dc.DrawRectangle( x, y, 310, 310 );

    dc.DestroyClippingRegion();

    wxRegion region(x + 110, y + 20, 100, 270);
#if !defined(__WXMOTIF__) && !defined(__WXMAC__)
    if ( !firstTime )
        region.Offset(10, 10);
#endif
    dc.SetClippingRegion(region);

    dc.SetBrush( *wxGREY_BRUSH );
    dc.DrawRectangle( x, y, 310, 310 );

    if (m_smile_bmp.Ok())
    {
        dc.DrawBitmap( m_smile_bmp, x + 150, y + 150, true );
        dc.DrawBitmap( m_smile_bmp, x + 130, y + 10,  true );
        dc.DrawBitmap( m_smile_bmp, x + 130, y + 280, true );
        dc.DrawBitmap( m_smile_bmp, x + 100, y + 70,  true );
        dc.DrawBitmap( m_smile_bmp, x + 200, y + 70,  true );
    }
}

void MyCanvas::OnPaint(wxPaintEvent &WXUNUSED(event))
{
    wxPaintDC dc(this);
    PrepareDC(dc);

    m_owner->PrepareDC(dc);

    dc.SetBackgroundMode( m_owner->m_backgroundMode );
    if ( m_owner->m_backgroundBrush.Ok() )
        dc.SetBackground( m_owner->m_backgroundBrush );
    if ( m_owner->m_colourForeground.Ok() )
        dc.SetTextForeground( m_owner->m_colourForeground );
    if ( m_owner->m_colourBackground.Ok() )
        dc.SetTextBackground( m_owner->m_colourBackground );

    if ( m_owner->m_textureBackground) {
        if ( ! m_owner->m_backgroundBrush.Ok() ) {
            wxColour clr(0,128,0);
            wxBrush b(clr, wxSOLID);
            dc.SetBackground(b);
        }
    }

    if ( m_clip )
        dc.SetClippingRegion(100, 100, 100, 100);

    dc.Clear();

    if ( m_owner->m_textureBackground )
    {
        dc.SetPen(*wxMEDIUM_GREY_PEN);
        for ( int i = 0; i < 200; i++ )
            dc.DrawLine(0, i*10, i*10, 0);
    }

    switch ( m_show )
    {
        case Show_Default:
            DrawDefault(dc);
            break;

        case Show_Circles:
            DrawCircles(dc);
            break;

        case Show_Regions:
            DrawRegions(dc);
            break;

        case Show_Text:
            DrawText(dc);
            break;

        case Show_Lines:
            DrawTestLines( 0, 100, 0, dc );
            DrawTestLines( 0, 320, 1, dc );
            DrawTestLines( 0, 540, 2, dc );
            DrawTestLines( 0, 760, 6, dc );
            break;

        case Show_Brushes:
            DrawTestBrushes(dc);
            break;

        case Show_Polygons:
            DrawTestPoly(dc);
            break;

        case Show_Mask:
            DrawImages(dc);
            break;

        case Show_Ops:
            DrawWithLogicalOps(dc);
            break;
    }
}

void MyCanvas::OnMouseMove(wxMouseEvent &event)
{
#if wxUSE_STATUSBAR
    wxClientDC dc(this);
    PrepareDC(dc);
    m_owner->PrepareDC(dc);

    wxPoint pos = event.GetPosition();
    long x = dc.DeviceToLogicalX( pos.x );
    long y = dc.DeviceToLogicalY( pos.y );
    wxString str;
    str.Printf( wxT("Current mouse position: %d,%d"), (int)x, (int)y );
    m_owner->SetStatusText( str );
#else
    wxUnusedVar(event);
#endif // wxUSE_STATUSBAR
}

// ----------------------------------------------------------------------------
// MyFrame
// ----------------------------------------------------------------------------

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU      (File_Quit,     MyFrame::OnQuit)
    EVT_MENU      (File_About,    MyFrame::OnAbout)
    EVT_MENU      (File_Clip,     MyFrame::OnClip)

    EVT_MENU_RANGE(MenuShow_First,   MenuShow_Last,   MyFrame::OnShow)

    EVT_MENU_RANGE(MenuOption_First, MenuOption_Last, MyFrame::OnOption)
END_EVENT_TABLE()

// frame constructor
MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
       : wxFrame((wxFrame *)NULL, wxID_ANY, title, pos, size,
                 wxDEFAULT_FRAME_STYLE | wxNO_FULL_REPAINT_ON_RESIZE)
{
    // set the frame icon
    SetIcon(wxICON(mondrian));

    wxMenu *menuFile = new wxMenu;
    menuFile->Append(File_ShowDefault, _T("&Default screen\tF1"));
    menuFile->Append(File_ShowText, _T("&Text screen\tF2"));
    menuFile->Append(File_ShowLines, _T("&Lines screen\tF3"));
    menuFile->Append(File_ShowBrushes, _T("&Brushes screen\tF4"));
    menuFile->Append(File_ShowPolygons, _T("&Polygons screen\tF5"));
    menuFile->Append(File_ShowMask, _T("&Mask screen\tF6"));
    menuFile->Append(File_ShowOps, _T("&ROP screen\tF7"));
    menuFile->Append(File_ShowRegions, _T("Re&gions screen\tF8"));
    menuFile->Append(File_ShowCircles, _T("&Circles screen\tF9"));
    menuFile->AppendSeparator();
    menuFile->AppendCheckItem(File_Clip, _T("&Clip\tCtrl-C"), _T("Clip/unclip drawing"));
    menuFile->AppendSeparator();
    menuFile->Append(File_About, _T("&About...\tCtrl-A"), _T("Show about dialog"));
    menuFile->AppendSeparator();
    menuFile->Append(File_Quit, _T("E&xit\tAlt-X"), _T("Quit this program"));

    wxMenu *menuMapMode = new wxMenu;
    menuMapMode->Append( MapMode_Text, _T("&TEXT map mode") );
    menuMapMode->Append( MapMode_Lometric, _T("&LOMETRIC map mode") );
    menuMapMode->Append( MapMode_Twips, _T("T&WIPS map mode") );
    menuMapMode->Append( MapMode_Points, _T("&POINTS map mode") );
    menuMapMode->Append( MapMode_Metric, _T("&METRIC map mode") );

    wxMenu *menuUserScale = new wxMenu;
    menuUserScale->Append( UserScale_StretchHoriz, _T("Stretch &horizontally\tCtrl-H") );
    menuUserScale->Append( UserScale_ShrinkHoriz, _T("Shrin&k horizontally\tCtrl-G") );
    menuUserScale->Append( UserScale_StretchVertic, _T("Stretch &vertically\tCtrl-V") );
    menuUserScale->Append( UserScale_ShrinkVertic, _T("&Shrink vertically\tCtrl-W") );
    menuUserScale->AppendSeparator();
    menuUserScale->Append( UserScale_Restore, _T("&Restore to normal\tCtrl-0") );

    wxMenu *menuAxis = new wxMenu;
    menuAxis->AppendCheckItem( AxisMirror_Horiz, _T("Mirror horizontally\tCtrl-M") );
    menuAxis->AppendCheckItem( AxisMirror_Vertic, _T("Mirror vertically\tCtrl-N") );

    wxMenu *menuLogical = new wxMenu;
    menuLogical->Append( LogicalOrigin_MoveDown, _T("Move &down\tCtrl-D") );
    menuLogical->Append( LogicalOrigin_MoveUp, _T("Move &up\tCtrl-U") );
    menuLogical->Append( LogicalOrigin_MoveLeft, _T("Move &right\tCtrl-L") );
    menuLogical->Append( LogicalOrigin_MoveRight, _T("Move &left\tCtrl-R") );
    menuLogical->AppendSeparator();
    menuLogical->Append( LogicalOrigin_Set, _T("Set to (&100, 100)\tShift-Ctrl-1") );
    menuLogical->Append( LogicalOrigin_Restore, _T("&Restore to normal\tShift-Ctrl-0") );

    wxMenu *menuColour = new wxMenu;
#if wxUSE_COLOURDLG
    menuColour->Append( Colour_TextForeground, _T("Text &foreground...") );
    menuColour->Append( Colour_TextBackground, _T("Text &background...") );
    menuColour->Append( Colour_Background, _T("Background &colour...") );
#endif // wxUSE_COLOURDLG
    menuColour->AppendCheckItem( Colour_BackgroundMode, _T("&Opaque/transparent\tCtrl-B") );
    menuColour->AppendCheckItem( Colour_TextureBackgound, _T("Draw textured back&ground\tCtrl-T") );

    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, _T("&File"));
    menuBar->Append(menuMapMode, _T("&Mode"));
    menuBar->Append(menuUserScale, _T("&Scale"));
    menuBar->Append(menuAxis, _T("&Axis"));
    menuBar->Append(menuLogical, _T("&Origin"));
    menuBar->Append(menuColour, _T("&Colours"));

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);

#if wxUSE_STATUSBAR
    CreateStatusBar(2);
    SetStatusText(_T("Welcome to wxWidgets!"));
#endif // wxUSE_STATUSBAR

    m_mapMode = wxMM_TEXT;
    m_xUserScale = 1.0;
    m_yUserScale = 1.0;
    m_xLogicalOrigin = 0;
    m_yLogicalOrigin = 0;
    m_xAxisReversed =
    m_yAxisReversed = false;
    m_backgroundMode = wxSOLID;
    m_colourForeground = *wxRED;
    m_colourBackground = *wxBLUE;
    m_textureBackground = false;

    m_canvas = new MyCanvas( this );
    m_canvas->SetScrollbars( 10, 10, 100, 240 );
}

// event handlers

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // true is to force the frame to close
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxString msg;
    msg.Printf( wxT("This is the about dialog of the drawing sample.\n")
                wxT("This sample tests various primitive drawing functions\n")
                wxT("(without any attempts to prevent flicker).\n")
                wxT("Copyright (c) Robert Roebling 1999")
              );

    wxMessageBox(msg, _T("About Drawing"), wxOK | wxICON_INFORMATION, this);
}

void MyFrame::OnClip(wxCommandEvent& event)
{
    m_canvas->Clip(event.IsChecked());
}

void MyFrame::OnShow(wxCommandEvent& event)
{
    m_canvas->ToShow((ScreenToShow)(event.GetId() - MenuShow_First));
}

void MyFrame::OnOption(wxCommandEvent& event)
{
    switch (event.GetId())
    {
        case MapMode_Text:
            m_mapMode = wxMM_TEXT;
            break;
        case MapMode_Lometric:
            m_mapMode = wxMM_LOMETRIC;
            break;
        case MapMode_Twips:
            m_mapMode = wxMM_TWIPS;
            break;
        case MapMode_Points:
            m_mapMode = wxMM_POINTS;
            break;
        case MapMode_Metric:
            m_mapMode = wxMM_METRIC;
            break;

        case LogicalOrigin_MoveDown:
            m_yLogicalOrigin += 10;
            break;
        case LogicalOrigin_MoveUp:
            m_yLogicalOrigin -= 10;
            break;
        case LogicalOrigin_MoveLeft:
            m_xLogicalOrigin += 10;
            break;
        case LogicalOrigin_MoveRight:
            m_xLogicalOrigin -= 10;
            break;
        case LogicalOrigin_Set:
            m_xLogicalOrigin =
            m_yLogicalOrigin = -100;
            break;
        case LogicalOrigin_Restore:
            m_xLogicalOrigin =
            m_yLogicalOrigin = 0;
            break;

        case UserScale_StretchHoriz:
            m_xUserScale *= 1.10;
            break;
        case UserScale_ShrinkHoriz:
            m_xUserScale /= 1.10;
            break;
        case UserScale_StretchVertic:
            m_yUserScale *= 1.10;
            break;
        case UserScale_ShrinkVertic:
            m_yUserScale /= 1.10;
            break;
        case UserScale_Restore:
            m_xUserScale =
            m_yUserScale = 1.0;
            break;

        case AxisMirror_Vertic:
            m_yAxisReversed = !m_yAxisReversed;
            break;
        case AxisMirror_Horiz:
            m_xAxisReversed = !m_xAxisReversed;
            break;

#if wxUSE_COLOURDLG
        case Colour_TextForeground:
            m_colourForeground = SelectColour();
            break;
        case Colour_TextBackground:
            m_colourBackground = SelectColour();
            break;
        case Colour_Background:
            {
                wxColour col = SelectColour();
                if ( col.Ok() )
                {
                    m_backgroundBrush.SetColour(col);
                }
            }
            break;
#endif // wxUSE_COLOURDLG

        case Colour_BackgroundMode:
            m_backgroundMode = m_backgroundMode == wxSOLID ? wxTRANSPARENT
                                                           : wxSOLID;
            break;

        case Colour_TextureBackgound:
            m_textureBackground = ! m_textureBackground;
            break;

        default:
            // skip Refresh()
            return;
    }

    m_canvas->Refresh();
}

void MyFrame::PrepareDC(wxDC& dc)
{
    dc.SetLogicalOrigin( m_xLogicalOrigin, m_yLogicalOrigin );
    dc.SetAxisOrientation( !m_xAxisReversed, m_yAxisReversed );
    dc.SetUserScale( m_xUserScale, m_yUserScale );
    dc.SetMapMode( m_mapMode );
}

#if wxUSE_COLOURDLG
wxColour MyFrame::SelectColour()
{
    wxColour col;
    wxColourData data;
    wxColourDialog dialog(this, &data);

    if ( dialog.ShowModal() == wxID_OK )
    {
        col = dialog.GetColourData().GetColour();
    }

    return col;
}
#endif // wxUSE_COLOURDLG

