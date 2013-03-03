/////////////////////////////////////////////////////////////////////////////
// Name:        generic/colrdlgg.cpp
// Purpose:     Choice dialogs
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id: colrdlgg.cpp,v 1.53 2005/02/20 03:22:54 KH Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "smtcolrdlgg.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP   
#endif


#include "smtcolrdlgg.h"
#include "OgreStringConverter.h"
#include "SceneManipulator.h"

BEGIN_EVENT_TABLE(wxSmartColourDialog, wxPanel)
    EVT_BUTTON(wxID_ADD_CUSTOM, wxSmartColourDialog::OnAddCustom)
	//EVT_BUTTON(wxID_OK,wxSmartColourDialog::OnOk)
	//EVT_BUTTON(wxID_CANCEL,wxSmartColourDialog::OnCancel)

    EVT_PAINT(wxSmartColourDialog::OnPaint)
    EVT_MOUSE_EVENTS(wxSmartColourDialog::OnMouseEvent)
	EVT_CHECKBOX(wxID_CHECK_APPLY, wxSmartColourDialog::OnApplyToObj)
	EVT_COMBOBOX(wxID_BRUSH_TYPE, OnBrushTypeChanged)
END_EVENT_TABLE()


/*
 * Generic wxColourDialog
 */

// don't change the number of elements (48) in this array, the code below is
// hardcoded to use it
static const wxChar *wxColourDialogNames[] =
{
    wxT("ORANGE"),
    wxT("GOLDENROD"),
    wxT("WHEAT"),
    wxT("SPRING GREEN"),
    wxT("SKY BLUE"),
    wxT("SLATE BLUE"),
    wxT("MEDIUM VIOLET RED"),
    wxT("PURPLE"),

    wxT("RED"),
    wxT("YELLOW"),
    wxT("MEDIUM SPRING GREEN"),
    wxT("PALE GREEN"),
    wxT("CYAN"),
    wxT("LIGHT STEEL BLUE"),
    wxT("ORCHID"),
    wxT("LIGHT MAGENTA"),

    wxT("BROWN"),
    wxT("YELLOW"),
    wxT("GREEN"),
    wxT("CADET BLUE"),
    wxT("MEDIUM BLUE"),
    wxT("MAGENTA"),
    wxT("MAROON"),
    wxT("ORANGE RED"),

    wxT("FIREBRICK"),
    wxT("CORAL"),
    wxT("FOREST GREEN"),
    wxT("AQUAMARINE"),
    wxT("BLUE"),
    wxT("NAVY"),
    wxT("THISTLE"),
    wxT("MEDIUM VIOLET RED"),

    wxT("INDIAN RED"),
    wxT("GOLD"),
    wxT("MEDIUM SEA GREEN"),
    wxT("MEDIUM BLUE"),
    wxT("MIDNIGHT BLUE"),
    wxT("GREY"),
    wxT("PURPLE"),
    wxT("KHAKI"),

    wxT("BLACK"),
    wxT("MEDIUM FOREST GREEN"),
    wxT("KHAKI"),
    wxT("DARK GREY"),
    wxT("SEA GREEN"),
    wxT("LIGHT GREY"),
    wxT("MEDIUM SLATE BLUE"),
    wxT("WHITE")
};

wxSmartColourDialog::wxSmartColourDialog(wxWindow* parent, 
										 wxWindowID id , 
										 const wxPoint& pos, 
										 const wxSize& size, 
										 long style , wxColourData *data)
: wxPanel( parent, id, pos, size, style )
{
  //m_currentAlphaImage = new wxImage(15,180);
  whichKind = 1;
  colourSelection = -1;
  currentAlpha = 0;

  currentX = 0;
  currentY = 0;

  Create(data);

}

wxSmartColourDialog::~wxSmartColourDialog()
{
}

bool wxSmartColourDialog::Create(wxColourData *data)
{
    if (data)
        colourData = *data;

    InitializeColours();
    CalculateMeasurements();
    CreateWidgets();

    return true;
}

void wxSmartColourDialog::OnSelColorAlpha(int alpha)
{
	wxClientDC dc(this);
	PaintAlphaHint(dc, false);
	currentAlpha = alpha;	

	float factor = 240.f-240.f*currentAlpha/255.f;
	m_adjustColor.Set(caculateColByBright(colourData.m_dataColour.Red(),factor), caculateColByBright(colourData.m_dataColour.Green(),factor),caculateColByBright(colourData.m_dataColour.Blue(),factor));
	
	//float factor = 1.0f-currentAlpha/255.f;
	//m_adjustColor.Set(colourData.m_dataColour.Red()*factor,colourData.m_dataColour.Green()*factor,colourData.m_dataColour.Blue()*factor);
	//setRGBText(colourData.m_dataColour.Red(),colourData.m_dataColour.Green(),colourData.m_dataColour.Blue(),true);

	PaintCustomColour(dc);	
	PaintCurrentAlphaBmp(dc);
	PaintAlphaHint(dc, true);
	
	if (mSceneManipulator)
	{
		Ogre::ColourValue diffuse(m_adjustColor.Red()/255.f,m_adjustColor.Green()/255.f,m_adjustColor.Blue()/255.f,1);
		mSceneManipulator->setDiffuseColour(diffuse);
	}
}

void wxSmartColourDialog::OnSelCustomColor(int x, int y)
{	
	wxClientDC dc(this);
	PaintSelCustomHint(dc,false);
	currentX = x;
	currentY = y;
	// Caculate the uv pos and get the colour
	//wxImage image = m_coloursBmp.ConvertToImage();
	//unsigned char r = image.GetRed(x,y);
	//unsigned char g = image.GetGreen(x,y);
	//unsigned char b =  image.GetBlue(x,y);

	int h =239*x/selectColoursRect.width;
	int s = 240-240*y/selectColoursRect.height;
	int l = 120;
	unsigned char r,g,b;
	int red,green,blue;
	HSB2RGB(h,s,l,red,green,blue);
	r = red;
	g = green;
	b = blue;

	colourData.m_dataColour.Set(r,g,b);
	float factor = 240.f-240.f*currentAlpha/255.f;
	m_adjustColor.Set(caculateColByBright(colourData.m_dataColour.Red(),factor), caculateColByBright(colourData.m_dataColour.Green(),factor),caculateColByBright(colourData.m_dataColour.Blue(),factor));
	PaintCustomColour(dc);
	//setRGBText(colourData.m_dataColour.Red(), colourData.m_dataColour.Green(), colourData.m_dataColour.Blue());
	//PaintCurrentAlphaBmp(dc);

	PaintSelectColours(dc);
	PaintSelCustomHint(dc,true);
	PaintAlphaHint(dc, false);
	PaintCurrentAlphaBmp(dc);
	PaintAlphaHint(dc, true);
	if (mSceneManipulator)
	{
		Ogre::ColourValue diffuse(m_adjustColor.Red()/255.f,m_adjustColor.Green()/255.f,m_adjustColor.Blue()/255.f,1);
		mSceneManipulator->setDiffuseColour(diffuse);
	}
}

void wxSmartColourDialog::SetSceneManipulator(Fairy::SceneManipulator* sceneManipulator)
{
	mSceneManipulator = sceneManipulator;
}

// Internal functions
void wxSmartColourDialog::OnMouseEvent(wxMouseEvent& event)
{
  if (event.ButtonDown(1))
  {

    int x = (int)event.GetX();
    int y = (int)event.GetY();

    if ((x >= standardColoursRect.x && x <= (standardColoursRect.x + standardColoursRect.width)) &&
        (y >= standardColoursRect.y && y <= (standardColoursRect.y + standardColoursRect.height)))
    {
      int selX = (int)(x - standardColoursRect.x)/(smallRectangleSize.x + gridSpacing);
      int selY = (int)(y - standardColoursRect.y)/(smallRectangleSize.y + gridSpacing);
      int ptr = (int)(selX + selY*8);
      OnBasicColourClick(ptr);
    }
    else if ((x >= customColoursRect.x && x <= (customColoursRect.x + customColoursRect.width)) &&
        (y >= customColoursRect.y && y <= (customColoursRect.y + customColoursRect.height)))
    {
      int selX = (int)(x - customColoursRect.x)/(smallRectangleSize.x + gridSpacing);
      int selY = (int)(y - customColoursRect.y)/(smallRectangleSize.y + gridSpacing);
      int ptr = (int)(selX + selY*8);
      OnCustomColourClick(ptr);
    }
	else if ((x >= alphaColourRect.x && x <= (alphaColourRect.x + alphaColourRect.width)) &&
		(y >= alphaColourRect.y && y <= (alphaColourRect.y + alphaColourRect.height)))
	{
		int selAlpha = (int)(y - alphaColourRect.y);
		OnSelColorAlpha(selAlpha);
	}
	else if ((x >= selectColoursRect.x && x <= (selectColoursRect.x + selectColoursRect.width)) &&
		(y >= selectColoursRect.y && y <= (selectColoursRect.y + selectColoursRect.height)))
	{
		int selX = (int)(x - selectColoursRect.x);
		int selY = (int)(y - selectColoursRect.y);
		OnSelCustomColor(selX, selY);
	}
    else
        event.Skip();
  }
  else
      event.Skip();
}

void wxSmartColourDialog::OnPaint(wxPaintEvent& event)
{
  wxPaintDC dc(this);

  PaintSelectColours(dc);
  PaintBasicColours(dc);
  PaintCustomColours(dc);
  PaintCustomColour(dc);
  PaintCurrentAlphaBmp(dc);
  PaintHighlight(dc, true);
  PaintAlphaHint(dc, true);
  PaintSelCustomHint(dc,true);
}

void wxSmartColourDialog::CalculateMeasurements()
{
  smallRectangleSize.x = 18;
  smallRectangleSize.y = 14;
  customRectangleSize.x = 200;
  customRectangleSize.y = 76;

  gridSpacing = 6;
  sectionSpacing = 15;

  selectColoursRect.x = 5;
  selectColoursRect.y = 5;
  selectColoursRect.width = 199;
  selectColoursRect.height = 172;

  singleCustomColourRect.x = 5;
  singleCustomColourRect.y = selectColoursRect.height + selectColoursRect.y+2;
  singleCustomColourRect.width = customRectangleSize.x;
  singleCustomColourRect.height = customRectangleSize.y;

  alphaColourRect.x = selectColoursRect.x+selectColoursRect.width+2;
  alphaColourRect.y = selectColoursRect.y;
  alphaColourRect.width = 20;
  alphaColourRect.height = 250;
  
  standardColoursRect.x = 5;
  standardColoursRect.y = singleCustomColourRect.y+singleCustomColourRect.height+15;
  standardColoursRect.width = (8*smallRectangleSize.x) + (7*gridSpacing);
  standardColoursRect.height = (6*smallRectangleSize.y) + (5*gridSpacing);

  customColoursRect.x = standardColoursRect.x;
  customColoursRect.y = standardColoursRect.y + standardColoursRect.height  + 10;
  customColoursRect.width = (8*smallRectangleSize.x) + (7*gridSpacing);
  customColoursRect.height = (2*smallRectangleSize.y) + (1*gridSpacing);
}

void wxSmartColourDialog::CreateWidgets()
{
	wxBeginBusyCursor();

    wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );

    topSizer->Add( 1, 430, 0, wxCENTRE|wxALL, 1 );

	//m_coloursBmp = wxBITMAP(COLORPANEL);

	// textCtrls
	/*wxBoxSizer* colorTextSizer = new wxBoxSizer( wxHORIZONTAL );
	colorTextSizer->Add( 275, 1, 0, wxCENTRE|wxALL, 1 );

	wxGridSizer* textSizer = new wxGridSizer( 2,2,0,0 );

	wxBoxSizer* redSizer = new wxBoxSizer( wxHORIZONTAL );
	wxStaticText* redStatic = new wxStaticText( this, wxID_ANY, wxT("r"), wxDefaultPosition, wxDefaultSize, 0 );
	redSizer->Add( redStatic, 0, wxALL, 5 );
	m_red = new wxTextCtrl( this, wxID_RED_TEXT, wxT("0"), wxDefaultPosition, wxSize(30,20), 0 );
	m_red->Enable(false);
	redSizer->Add( m_red, 0, wxALL, 5 );
	textSizer->Add( redSizer, 1, wxEXPAND, 5 );

	wxBoxSizer* greenSizer = new wxBoxSizer( wxHORIZONTAL );
	wxStaticText* greenStatic = new wxStaticText( this, wxID_ANY, wxT("g"), wxDefaultPosition, wxDefaultSize, 0 );
	greenSizer->Add( greenStatic, 0, wxALL, 5 );
	m_green = new wxTextCtrl( this, wxID_RED_TEXT, wxT("0"), wxDefaultPosition, wxSize(30,20), 0 );
	m_green->Enable(false);
	greenSizer->Add( m_green, 0, wxALL, 5 );
	textSizer->Add( greenSizer, 1, wxEXPAND, 5 );

	wxBoxSizer* blueSizer = new wxBoxSizer( wxHORIZONTAL );
	wxStaticText* blueStatic = new wxStaticText( this, wxID_ANY, wxT("b"), wxDefaultPosition, wxDefaultSize, 0 );
	blueSizer->Add( blueStatic, 0, wxALL, 5 );
	m_blue = new wxTextCtrl( this, wxID_RED_TEXT, wxT("0"), wxDefaultPosition, wxSize(30,20), 0 );
	m_blue->Enable(false);
	blueSizer->Add( m_blue, 0, wxALL, 5 );
	textSizer->Add( blueSizer, 1, wxEXPAND, 5 );

	wxBoxSizer* alphaSizer = new wxBoxSizer( wxHORIZONTAL );
	wxStaticText* alphaStatic = new wxStaticText( this, wxID_ANY, wxT("a"), wxDefaultPosition, wxDefaultSize, 0 );
	alphaSizer->Add( alphaStatic, 0, wxALL, 5 );
	m_alpha = new wxTextCtrl( this, wxID_RED_TEXT, wxT("0"), wxDefaultPosition, wxSize(30,20), 0 );
	m_alpha->Enable(false);
	alphaSizer->Add( m_alpha, 0, wxALL, 5 );
	textSizer->Add( alphaSizer, 1, wxEXPAND, 5 );

	colorTextSizer->Add( textSizer, 0, wxEXPAND | wxALL, 1 );
	topSizer->Add( colorTextSizer, 0, wxEXPAND | wxALL, 10 );*/
	// check box
	wxBoxSizer* checkSizer;
	checkSizer = new wxBoxSizer( wxHORIZONTAL );

	m_checkClear = new wxCheckBox( this, wxID_CHECK_APPLY, wxT("Clear"), wxDefaultPosition, wxDefaultSize, 0 );
	checkSizer->Add( m_checkClear, 0, wxALL|wxCENTRE, 5 );
	
	wxBoxSizer* typeSizer;
	typeSizer = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticText* typeLabel = new wxStaticText(this,wxID_ANY,wxT("Type"), wxDefaultPosition, wxDefaultSize, 0);
	typeSizer->Add( typeLabel, 0, wxALL|wxCENTRE, 5 );

	wxArrayString brushTypes;
	brushTypes.push_back(wxT("TerrainDiffuse"));
	brushTypes.push_back(wxT("ObjectDiffuse"));
	m_brushType = new wxComboBox(this, wxID_BRUSH_TYPE, wxEmptyString, wxDefaultPosition, wxDefaultSize, brushTypes, wxCB_READONLY);
	m_brushType->Select(0);
	typeSizer->Add( m_brushType, 0, wxALL|wxCENTRE, 5);

	checkSizer->Add(typeSizer, 0, wxALL, 5);

	topSizer->Add( checkSizer, 0, wxEXPAND | wxALL, 10 );

    // buttons
	wxStdDialogButtonSizer *buttonsizer = new wxStdDialogButtonSizer();
	/*buttonsizer->Add( new wxButton(this, wxID_OK, _("Ok") ), 0, wxLEFT|wxRIGHT, 10 );
	buttonsizer->Add( new wxButton(this, wxID_CANCEL, _("Cancel") ), 0, wxLEFT|wxRIGHT, 10 );*/
    buttonsizer->Add( new wxButton(this, wxID_ADD_CUSTOM, _("Add to custom colours") ), 0, wxLEFT|wxRIGHT, 10 );
    topSizer->Add( buttonsizer, 0, wxEXPAND | wxALL, 10 );

	this->SetSizer( topSizer );
	this->Layout();

   /* topSizer->SetSizeHints( this );
    topSizer->Fit( this );

    Centre( wxBOTH );*/

    wxEndBusyCursor();
}

void wxSmartColourDialog::InitializeColours(void)
{
    size_t i;

    for (i = 0; i < WXSIZEOF(wxColourDialogNames); i++)
    {
        wxColour col = wxTheColourDatabase->Find(wxColourDialogNames[i]);
        if (col.Ok())
            standardColours[i].Set(col.Red(), col.Green(), col.Blue());
        else
            standardColours[i].Set(0, 0, 0);
    }

    for (i = 0; i < WXSIZEOF(customColours); i++)
    {
        wxColour c = colourData.GetCustomColour(i);
        if (c.Ok())
            customColours[i] = colourData.GetCustomColour(i);
        else
            customColours[i] = wxColour(255, 255, 255);
    }

    wxColour curr = colourData.GetColour();
    if ( curr.Ok() )
    {
        bool initColourFound = false;

        for (i = 0; i < WXSIZEOF(wxColourDialogNames); i++)
        {
            if ( standardColours[i] == curr && !initColourFound )
            {
                whichKind = 1;
                colourSelection = i;
                initColourFound = true;
                break;
            }
        }
        if ( !initColourFound )
        {
            for ( i = 0; i < WXSIZEOF(customColours); i++ )
            {
                if ( customColours[i] == curr )
                {
                    whichKind = 2;
                    colourSelection = i;
                    break;
                }
            }
        }
        colourData.m_dataColour.Set( curr.Red(), curr.Green(), curr.Blue() );
    }
    else
    {
        whichKind = 1;
        colourSelection = 0;
        colourData.m_dataColour.Set( 0, 0, 0 );
    }
}

void wxSmartColourDialog::PaintBasicColours(wxDC& dc)
{
  dc.BeginDrawing();

  int i;
  for (i = 0; i < 6; i++)
  {
    int j;
    for (j = 0; j < 8; j++)
    {
      int ptr = i*8 + j;

      int x = (j*(smallRectangleSize.x+gridSpacing) + standardColoursRect.x);
      int y = (i*(smallRectangleSize.y+gridSpacing) + standardColoursRect.y);

      dc.SetPen(*wxBLACK_PEN);
      wxBrush brush(standardColours[ptr], wxSOLID);
      dc.SetBrush(brush);

      dc.DrawRectangle( x, y, smallRectangleSize.x, smallRectangleSize.y);
    }
  }
  dc.EndDrawing();
}

void wxSmartColourDialog::PaintCustomColours(wxDC& dc)
{
  dc.BeginDrawing();

  int i;
  for (i = 0; i < 2; i++)
  {
    int j;
    for (j = 0; j < 8; j++)
    {
      int ptr = i*8 + j;

      int x = (j*(smallRectangleSize.x+gridSpacing)) + customColoursRect.x;
      int y = (i*(smallRectangleSize.y+gridSpacing)) + customColoursRect.y;

      dc.SetPen(*wxBLACK_PEN);

      wxBrush brush(customColours[ptr], wxSOLID);
      dc.SetBrush(brush);

      dc.DrawRectangle( x, y, smallRectangleSize.x, smallRectangleSize.y);
    }
  }
  dc.EndDrawing();
}

void wxSmartColourDialog::PaintSelectColours(wxDC& dc)
{
	dc.BeginDrawing();
	for (int j = 0; j < selectColoursRect.width; j++)
	{
		for (int i = 0; i < selectColoursRect.height; i++)
		{
			int h =239*j/selectColoursRect.width;
			int s = 240-240*i/selectColoursRect.height;
			int l = 120;
			unsigned char r,g,b;
			int red,green,blue;
			HSB2RGB(h,s,l,red,green,blue);
			r = red;
			g = green;
			b = blue;
			
			int x = selectColoursRect.x+j;
			int y = selectColoursRect.y+i;

			dc.SetPen(*wxTRANSPARENT_PEN);
			wxColour color(r,g,b);

			wxBrush brush(color, wxSOLID);
			dc.SetBrush(brush);

			dc.DrawRectangle( x, y, 1, 1);
		}		
	}
	//dc.DrawBitmap( m_coloursBmp, selectColoursRect.x, selectColoursRect.y);
	dc.EndDrawing();
}

unsigned char wxSmartColourDialog::caculateColByBright(unsigned char col, int bright)
{
	unsigned char outCol;
	if(bright>120)
	{
		outCol = (255.f-col)*(bright-120)/120+col;
	}
	else if (bright<120)
	{
		outCol = col*bright/120;
	}
	else
	{
		outCol = col;
	}
	return outCol;
}

void wxSmartColourDialog::PaintCurrentAlphaBmp(wxDC& dc)
{
	if ( colourSelection < 0 )
		return;

	dc.BeginDrawing();
	
	for (int j = 0; j < 250; j++)
	{
		float alphaValue = j / 255.0f;
		float factor = 240.f-240.f*j/255.f;
		wxColour adjust;
		adjust.Set(caculateColByBright(colourData.m_dataColour.Red(),factor), caculateColByBright(colourData.m_dataColour.Green(),factor),caculateColByBright(colourData.m_dataColour.Blue(),factor));
		unsigned char r = adjust.Red() /** alphaValue +  255* ( 1.0f - alphaValue)*/;
		unsigned char g = adjust.Green()/* * alphaValue + 255 * ( 1.0f - alphaValue)*/;
		unsigned char b = adjust.Blue()/** alphaValue +  255* ( 1.0f - alphaValue)*/;
		//float alphaValue = j / 255.0f;
		//wxColour adjust2;
		//adjust2.Set(colourData.m_dataColour.Red()*alphaValue,colourData.m_dataColour.Green()*alphaValue,colourData.m_dataColour.Blue()*alphaValue);
		//		
		///*unsigned char r = colourData.m_dataColour.Red() * alphaValue +  255* ( 1.0f - alphaValue);
		//unsigned char g = colourData.m_dataColour.Green() * alphaValue + 255 * ( 1.0f - alphaValue);
		//unsigned char b = colourData.m_dataColour.Blue()* alphaValue +  255* ( 1.0f - alphaValue);*/
		//unsigned char r = (colourData.m_dataColour.Red()-adjust2.Red()) * alphaValue +  255* ( 1.0f - alphaValue);
		//unsigned char g = (colourData.m_dataColour.Green()-adjust2.Green()) * alphaValue + 255 * ( 1.0f - alphaValue);
		//unsigned char b = (colourData.m_dataColour.Blue()-adjust2.Blue())* alphaValue +  255* ( 1.0f - alphaValue);

		int x = alphaColourRect.x;
		int y = alphaColourRect.y+j;

		dc.SetPen(*wxTRANSPARENT_PEN);
		wxColour color(r,g,b);

		wxBrush brush(color, wxSOLID);
		dc.SetBrush(brush);

		dc.DrawRectangle( x, y, alphaColourRect.width, 1);
	}
	dc.EndDrawing();
}

void wxSmartColourDialog::PaintHighlight(wxDC& dc, bool draw)
{
  if ( colourSelection < 0 )
      return;

  dc.BeginDrawing();

  // Number of pixels bigger than the standard rectangle size
  // for drawing a highlight
  int deltaX = 2;
  int deltaY = 2;

  if (whichKind == 1)
  {
    // Standard colours
    int y = (int)(colourSelection / 8);
    int x = (int)(colourSelection - (y*8));

    x = (x*(smallRectangleSize.x + gridSpacing) + standardColoursRect.x) - deltaX;
    y = (y*(smallRectangleSize.y + gridSpacing) + standardColoursRect.y) - deltaY;

    if (draw)
      dc.SetPen(*wxBLACK_PEN);
    else
      dc.SetPen(*wxLIGHT_GREY_PEN);

    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.DrawRectangle( x, y, (smallRectangleSize.x + (2*deltaX)), (smallRectangleSize.y + (2*deltaY)));
  }
  else
  {
    // User-defined colours
    int y = (int)(colourSelection / 8);
    int x = (int)(colourSelection - (y*8));

    x = (x*(smallRectangleSize.x + gridSpacing) + customColoursRect.x) - deltaX;
    y = (y*(smallRectangleSize.y + gridSpacing) + customColoursRect.y) - deltaY;

    if (draw)
      dc.SetPen(*wxBLACK_PEN);
    else
      dc.SetPen(*wxLIGHT_GREY_PEN);

    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.DrawRectangle( x, y, (smallRectangleSize.x + (2*deltaX)), (smallRectangleSize.y + (2*deltaY)));
  }

  dc.EndDrawing();
}
void wxSmartColourDialog::PaintAlphaHint(wxDC& dc, bool draw)
{
	dc.BeginDrawing();

	if (draw)
		dc.SetPen(*wxBLACK_PEN);
	else
	{
		wxRect rect = wxRect(alphaColourRect.x+alphaColourRect.width,alphaColourRect.y-5,7,alphaColourRect.height+15);
		Refresh(true,&rect);
		dc.EndDrawing();
		return;
	}

	dc.SetBrush(*wxTRANSPARENT_BRUSH);
	wxPoint ptArray[4];
	ptArray[0] = wxPoint(alphaColourRect.x+alphaColourRect.width+5,alphaColourRect.y+currentAlpha-7);
	ptArray[1] = wxPoint(alphaColourRect.x+alphaColourRect.width,alphaColourRect.y+currentAlpha);
	ptArray[2] = wxPoint(alphaColourRect.x+alphaColourRect.width+5,alphaColourRect.y+currentAlpha+7);
	ptArray[3] = ptArray[0];
	dc.DrawLines(sizeof(ptArray)/sizeof(wxPoint),ptArray,0,0); 
	dc.EndDrawing();
}
void wxSmartColourDialog::PaintSelCustomHint(wxDC& dc, bool draw)
{
	dc.BeginDrawing();

	if (draw)
		dc.SetPen(*wxBLACK_PEN);
	else
		dc.SetPen(*wxLIGHT_GREY_PEN);

	wxPoint ptArray[4][2];
	ptArray[0][0] = wxPoint(selectColoursRect.x+currentX-3, selectColoursRect.y+currentY);
	ptArray[0][1] = wxPoint(selectColoursRect.x+currentX-5, selectColoursRect.y+currentY);
	ptArray[1][0]= wxPoint(selectColoursRect.x+currentX+3, selectColoursRect.y+currentY);
	ptArray[1][1] = wxPoint(selectColoursRect.x+currentX+5, selectColoursRect.y+currentY);
	ptArray[2][0] = wxPoint(selectColoursRect.x+currentX, selectColoursRect.y+currentY-3);
	ptArray[2][1] = wxPoint(selectColoursRect.x+currentX, selectColoursRect.y+currentY-5);
	ptArray[3][0] = wxPoint(selectColoursRect.x+currentX, selectColoursRect.y+currentY+3);
	ptArray[3][1] = wxPoint(selectColoursRect.x+currentX, selectColoursRect.y+currentY+5);

	dc.SetBrush(*wxTRANSPARENT_BRUSH);
	if (currentX>4)
	{
		dc.DrawLines(2,ptArray[0],0,0); 
	}
	if (currentX<selectColoursRect.width-4)
	{
		dc.DrawLines(2,ptArray[1],0,0); 
	}
	if (currentY>4)
	{
		dc.DrawLines(2,ptArray[2],0,0); 
	}
	
	if (currentY<selectColoursRect.height-4)
	{
		dc.DrawLines(2,ptArray[3],0,0); 
	}
	//dc.DrawRectangle( selectColoursRect.x+currentX, selectColoursRect.y+currentY,
	//	4, 4);

	dc.EndDrawing();
}

void wxSmartColourDialog::PaintCustomColour(wxDC& dc)
{
  dc.BeginDrawing();

  dc.SetPen(*wxBLACK_PEN);

  float alphaValue = currentAlpha / 255.0f;
  float factor = 240.f-240.f*currentAlpha/255.f;
  wxColour adjust;
  adjust.Set(caculateColByBright(colourData.m_dataColour.Red(),factor), caculateColByBright(colourData.m_dataColour.Green(),factor),caculateColByBright(colourData.m_dataColour.Blue(),factor));
  unsigned char r = adjust.Red() /** alphaValue +  255* ( 1.0f - alphaValue)*/;
  unsigned char g = adjust.Green() /** alphaValue + 255 * ( 1.0f - alphaValue)*/;
  unsigned char b = adjust.Blue()/** alphaValue +  255* ( 1.0f - alphaValue)*/;
  /*float alphaValue = currentAlpha / 255.0f;
  wxColour adjust2;
  adjust2.Set(colourData.m_dataColour.Red()*alphaValue,colourData.m_dataColour.Green()*alphaValue,colourData.m_dataColour.Blue()*alphaValue);
  unsigned char r = (colourData.m_dataColour.Red()-adjust2.Red()) * alphaValue +  255* ( 1.0f - alphaValue);
  unsigned char g = (colourData.m_dataColour.Green()-adjust2.Green()) * alphaValue + 255 * ( 1.0f - alphaValue);
  unsigned char b = (colourData.m_dataColour.Blue()-adjust2.Blue())* alphaValue +  255* ( 1.0f - alphaValue);*/

  wxColour color(r,g,b);

  wxBrush *brush = new wxBrush(color, wxSOLID);
  dc.SetBrush(*brush);

  dc.DrawRectangle( singleCustomColourRect.x, singleCustomColourRect.y,
                    customRectangleSize.x, customRectangleSize.y);

  dc.SetBrush(wxNullBrush);
  delete brush;

  dc.EndDrawing();
}

//void wxSmartColourDialog::setRGBText(unsigned char r, unsigned char g, unsigned char b,bool bAlphaApply)
//{
//	m_red->SetValue(Ogre::StringConverter::toString(r));
//	m_green->SetValue(Ogre::StringConverter::toString(g));
//	m_blue->SetValue(Ogre::StringConverter::toString(b));
//	if (bAlphaApply)
//	{
//		m_alpha->SetValue(Ogre::StringConverter::toString(currentAlpha));
//	}	
//}

void wxSmartColourDialog::applyCurDiffuseColour(const Ogre::ColourValue& col)
{
	wxClientDC dc(this);
	PaintSelCustomHint(dc,false);
	int h,s,l;
	int adjust_r = col.r*255;
	int adjust_g = col.g*255;
	int adjust_b = col.b*255;
	RGB2HSB(adjust_r,adjust_g,adjust_b,h,s,l);

	currentAlpha = std::min((240-l)*255/240,250);
	currentX = h*selectColoursRect.width/239;
	currentY = (240-s)*selectColoursRect.height/240;

	int r,g,b;
	HSB2RGB(h,s,120,r,g,b);

	colourData.m_dataColour.Set(r,g,b);
	//currentAlpha = col.a *255;
	//setRGBText(col.r*255, col.g*255, col.b*255,true);
	
	PaintCustomColour(dc);
	PaintSelectColours(dc);
	PaintSelCustomHint(dc,true);
	PaintAlphaHint(dc, false);
	PaintCurrentAlphaBmp(dc);
	PaintAlphaHint(dc, true);
	PaintHighlight(dc, true);
}

void wxSmartColourDialog::setCurApplyFlag(bool flag)
{
	m_checkClear->SetValue(flag);
}

void wxSmartColourDialog::OnBasicColourClick(int which)
{
    wxClientDC dc(this);

    PaintHighlight(dc, false);
	PaintSelCustomHint(dc,false);
    whichKind = 1;
    colourSelection = which;

	int h,s,l;
	int adjust_r = standardColours[colourSelection].Red();
	int adjust_g = standardColours[colourSelection].Green();
	int adjust_b = standardColours[colourSelection].Blue();
	RGB2HSB(adjust_r,adjust_g,adjust_b,h,s,l);

	currentAlpha = std::min((240-l)*255/240,250);
	currentX = h*selectColoursRect.width/239;
	currentY = (240-s)*selectColoursRect.height/240;

	int r,g,b;
	HSB2RGB(h,s,120,r,g,b);


    colourData.m_dataColour.Set(r,g,b);

	m_adjustColor.Set(standardColours[colourSelection].Red(),
						standardColours[colourSelection].Green(),
						standardColours[colourSelection].Blue());

	PaintCustomColour(dc);
	PaintSelectColours(dc);
	PaintSelCustomHint(dc,true);
	//setRGBText(colourData.m_dataColour.Red(), colourData.m_dataColour.Green(), colourData.m_dataColour.Blue());
	PaintAlphaHint(dc, false);
	PaintCurrentAlphaBmp(dc);
	PaintAlphaHint(dc, true);
    PaintHighlight(dc, true);

	if (mSceneManipulator)
	{
		Ogre::ColourValue diffuse(m_adjustColor.Red()/255.f,m_adjustColor.Green()/255.f,m_adjustColor.Blue()/255.f,1);
		mSceneManipulator->setDiffuseColour(diffuse);
	}
}

void wxSmartColourDialog::OnCustomColourClick(int which)
{
    wxClientDC dc(this);
    PaintHighlight(dc, false);
	PaintSelCustomHint(dc,false);
    whichKind = 2;
    colourSelection = which;

	int h,s,l;
	int adjust_r = standardColours[colourSelection].Red();
	int adjust_g = standardColours[colourSelection].Green();
	int adjust_b = standardColours[colourSelection].Blue();
	RGB2HSB(adjust_r,adjust_g,adjust_b,h,s,l);

	currentAlpha = std::min((240-l)*255/240,250);
	currentX = h*selectColoursRect.width/239;
	currentY = (240-s)*selectColoursRect.height/240;

	int r,g,b;
	HSB2RGB(h,s,120,r,g,b);


	colourData.m_dataColour.Set(r,g,b);

	m_adjustColor.Set(standardColours[colourSelection].Red(),
						standardColours[colourSelection].Green(),
						standardColours[colourSelection].Blue());

	PaintCustomColour(dc);
	PaintSelectColours(dc);
	PaintSelCustomHint(dc,true);
	//setRGBText(colourData.m_dataColour.Red(), colourData.m_dataColour.Green(), colourData.m_dataColour.Blue());
	PaintAlphaHint(dc, false);
	PaintCurrentAlphaBmp(dc);
	PaintAlphaHint(dc, true);
    PaintHighlight(dc, true);

	if (mSceneManipulator)
	{
		Ogre::ColourValue diffuse(m_adjustColor.Red()/255.f,m_adjustColor.Green()/255.f,m_adjustColor.Blue()/255.f,1);
		mSceneManipulator->setDiffuseColour(diffuse);
	}
}

void wxSmartColourDialog::HSB2RGB(int hue, int sat, int bri, int& r, int& g, int& b)
{
	float red,green,blue;
	HSB2RGB(hue,sat,bri,red,green,blue);

	red = 255.0f*red;
	green = 255.0f*green;
	blue = 255.0f*blue;

	if(red<0)
	{
		red = 0.f;
	}
	else if (red >255.0f)
	{
		red = 255.f;
	}

	if(green<0)
	{
		green = 0.f;
	}
	else if (green >255.0f)
	{
		green = 255.f;
	}

	if(blue<0)
	{
		blue = 0.f;
	}
	else if (blue >255.0f)
	{
		blue = 255.f;
	}

	r = (int)(red+0.5f);
	g = (int)(green+0.5f);
	b = (int)(blue+0.5f);
}

void wxSmartColourDialog::HSB2RGB(int hue, int sat, int bri, float& r, float& g, float& b)
{
	if (hue>239)
	{
		hue = 239;
	}
	else if (hue<0)
	{
		hue = 0;
	}

	if (sat>240)
	{
		sat = 240;
	}
	else if (sat<0)
	{
		sat = 0;
	}

	if (bri>240)
	{
		bri = 240;
	}
	else if (bri<0)
	{
		bri = 0;
	}

	float H = hue/239.f;
	float S = sat/240.f;
	float L = bri/240.f;

	float red = 0;
	float green = 0;
	float blue = 0;
	float d1, d2;
	if (L == 0)
	{
		red = green = blue = 0;
	}
	else
	{
		if (S== 0)
		{
			red = green = blue = L;
		}
		else
		{
			d2 = (L<=0.5f)?L*(1.0f+S):(L+S-L*S);
			d1 = 2.0f*L-d2;
			float* d3 = new float[3];
			d3[0] = H+1.0f/3.0f;
			d3[1] = H;
			d3[2] = H-1.0f/3.0f;
			float* rgb = new float[3];
			rgb[0] = rgb[1] = rgb[2] = 0;
			for (int i = 0; i<3; i++)
			{
				if (d3[i]<0)
				{
					d3[i]+=1.0f;
				}
				if (d3[i]>1.0f)
				{
					d3[i] -=1.0f;
				}

				if(6.0f*d3[i]<1.0f)
				{
					rgb[i] = d1+(d2-d1)*d3[i]*6.0f;
				}
				else
				{
					if(2.0f*d3[i]<1.0f)
					{
						rgb[i] = d2;
					}
					else
					{
						if(3.0f*d3[i]<2.0f)
						{
							rgb[i] = (d1+(d2-d1)*((2.0f/3.0f)-d3[i])*6.0f);
						}
						else
						{
							rgb[i] = d1;
						}
					}
				}
			}

			red		= rgb[0];
			green	= rgb[1];
			blue	= rgb[2];
		}
	}
	r = red;
	g = green;
	b = blue;

}

void wxSmartColourDialog::RGB2HSB(int r, int g, int b, float& hue, float& sat, float& bri)
{
	int min_val = std::min(r, std::min(g,b));
	int max_val = std::max(r, std::max(g,b));

	//bri
	bri = (float)(min_val+max_val)/510;

	//sat
	if (max_val == min_val)
	{
		sat = 0.0f;
	}
	else
	{
		int sum = max_val+min_val;
		if (sum>255)
		{
			sum = 510-sum;
		}
		sat = (float)(max_val-min_val)/sum;
	}

	//hue
	if (max_val == min_val)
	{
		hue = 0.0f;
	}
	else
	{
		float diff = (float)(max_val - min_val);
		float rnorm = (max_val - r)/diff;
		float gnorm = (max_val - g)/diff;
		float bnorm = (max_val - b)/diff;

		if (r == max_val)
		{
			hue = 60.0f*(6.0f+bnorm-gnorm);
		}

		if (g == max_val)
		{
			hue = 60.0f*(2.0f+rnorm-bnorm);
		}

		if (b == max_val)
		{
			hue = 60.0f*(4.0f+gnorm-rnorm);
		}

		if (hue>360.f)
		{
			hue = hue -360.f;
		}
	}
}

void wxSmartColourDialog::RGB2HSB(int r, int g, int b, int& hue, int& sat, int& bri)
{
	float fHue,fSat,fBri;
	RGB2HSB(r,g,b,fHue,fSat,fBri);
	hue = (int)((fHue/360.f)*240+0.5);
	sat = (int)(fSat*241+0.5);
	bri = (int)(fBri*241+0.5);

	if (hue >239)
	{
		hue = 239;
	}

	if (sat >240)
	{
		sat = 240;
	}

	if (bri >240)
	{
		bri = 240;
	}
}


//void wxSmartColourDialog::OnOk(wxCommandEvent& e)
//{
//	if (mSceneManipulator)
//	{
//		Ogre::String str = m_red->GetValue().c_str();
//		float red = Ogre::StringConverter::parseReal(str);
//		str = m_green->GetValue().c_str();
//		float green = Ogre::StringConverter::parseReal(str);
//		str = m_blue->GetValue().c_str();
//		float blue = Ogre::StringConverter::parseReal(str);
//		str = m_alpha->GetValue().c_str();
//		float alpha = Ogre::StringConverter::parseReal(str);
//
//		Ogre::ColourValue colourValue= Ogre::ColourValue(red, green, blue, alpha);
//		colourValue/=255;
//
//		mSceneManipulator->setDiffuseColour(colourValue);
//	}
//	
//}
//
//void wxSmartColourDialog::OnCancel(wxCommandEvent& e)
//{
//
//	if (mSceneManipulator)
//	{
//		Ogre::ColourValue col = mSceneManipulator->getDiffuseColour();
//		applyCurDiffuseColour(col);
//	}	
//}


void wxSmartColourDialog::OnAddCustom(wxCommandEvent& WXUNUSED(event))
{
  wxClientDC dc(this);
  if (whichKind != 2)
  {
    PaintHighlight(dc, false);
    whichKind = 2;
    colourSelection = 0;
    PaintHighlight(dc, true);
  }

  customColours[colourSelection].Set(colourData.m_dataColour.Red(),
                                     colourData.m_dataColour.Green(),
                                     colourData.m_dataColour.Blue());

  colourData.SetCustomColour(colourSelection, customColours[colourSelection]);

  PaintCustomColours(dc);
  //setRGBText(colourData.m_dataColour.Red(), colourData.m_dataColour.Green(), colourData.m_dataColour.Blue());
}

void wxSmartColourDialog::OnApplyToObj(wxCommandEvent& WXUNUSED(event))
{
	if (mSceneManipulator)
	{
		mSceneManipulator->setClearFlag(m_checkClear->GetValue());
	}
}


void wxSmartColourDialog::OnBrushTypeChanged(wxCommandEvent& event)
{
	Ogre::String currentType = m_brushType->GetValue().c_str();
	mSceneManipulator->_getAction("DragTerrainColourAction")->setParameter("switchType", currentType);
}



