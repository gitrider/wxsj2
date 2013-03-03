/////////////////////////////////////////////////////////////////////////////
// Name:        wx/generic/colrdlgg.h
// Purpose:     wxGenericColourDialog
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id: colrdlgg.h,v 1.18 2005/05/04 18:52:32 JS Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __SMARTCOLORDLGH_G__
#define __SMARTCOLORDLGH_G__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "smtcolrdlgg.h"
#endif

//#include "wx/gdicmn.h"
//#include "wx/cmndata.h"
#include "wx/image.h"
#include "OgreColourValue.h"

#define wxID_ADD_CUSTOM     3000
#define wxID_SEL_BITMAP		3001
#define wxID_ALPHA_SEL		3002
#define wxID_RED_TEXT		3003
#define wxID_GREEN_TEXT		3004
#define wxID_BLUE_TEXT		3005
#define wxID_ALPHA_TEXT		3006
#define wxID_CHECK_APPLY	3007
#define wxID_BRUSH_TYPE		3008

namespace Fairy {
	class SceneManipulator;
}

class wxSmartColourDialog : public wxPanel
{
public:
	wxSmartColourDialog( wxWindow* parent, 
		wxWindowID id = wxID_ANY, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxTAB_TRAVERSAL, wxColourData *data = (wxColourData *) NULL);

    ~wxSmartColourDialog();
	void SetSceneManipulator(Fairy::SceneManipulator* sceneManipulator);

	Fairy::SceneManipulator* GetSceneManipulator(void) const
	{
		return mSceneManipulator;
	}

    bool Create(wxColourData *data = (wxColourData *) NULL);

    wxColourData &GetColourData() { return colourData; }

    // Internal functions

	void PaintCurrentAlphaBmp(wxDC& dc);

    void CalculateMeasurements();
    void CreateWidgets();
    void InitializeColours();

    void PaintBasicColours(wxDC& dc);
    void PaintCustomColours(wxDC& dc);
    void PaintCustomColour(wxDC& dc);
    void PaintHighlight(wxDC& dc, bool draw);
	void PaintSelectColours(wxDC& dc);

	void PaintAlphaHint(wxDC& dc, bool draw);
	void PaintSelCustomHint(wxDC& dc,bool draw);

    void OnBasicColourClick(int which);
    void OnCustomColourClick(int which);  

	void OnSelCustomColor(int x,int y);
	void OnSelColorAlpha(int alpha);

	//void setRGBText(unsigned char r, unsigned char g, unsigned char b, bool bAlphaApply = false);
	void applyCurDiffuseColour(const Ogre::ColourValue& col);
	unsigned char caculateColByBright(unsigned char col, int bright);

	void setCurApplyFlag(bool flag);

	void HSB2RGB(int hue, int sat, int bri, int& r, int& g, int& b);
	void HSB2RGB(int hue, int sat, int bri, float& r, float& g, float& b);
	void RGB2HSB(int r, int g, int b, float& hue, float& sat, float& bri);
	void RGB2HSB(int r, int g, int b, int& hue, int& sat, int& bri);

protected:
	Fairy::SceneManipulator* mSceneManipulator;

	wxCheckBox* m_checkClear;
	wxComboBox* m_brushType;

    wxColourData colourData;

    // Area reserved for grids of colours
    wxRect standardColoursRect;
    wxRect customColoursRect;
    wxRect singleCustomColourRect;
	wxRect alphaColourRect;
	wxRect selectColoursRect;

    // Size of each colour rectangle
    wxPoint smallRectangleSize;

    // For single customizable colour
    wxPoint customRectangleSize;

    // Grid spacing (between rectangles)
    int gridSpacing;

    // Section spacing (between left and right halves of dialog box)
    int sectionSpacing;

    // 48 'standard' colours
    wxColour standardColours[48];

    // 16 'custom' colours
    wxColour customColours[16];

    // Which colour is selected? An index into one of the two areas.
    int colourSelection;
    int whichKind; // 1 for standard colours, 2 for custom colours,

	int currentAlpha;

	int currentX;
	int currentY;

	//wxTextCtrl* m_red;
	//wxTextCtrl* m_green;
	//wxTextCtrl* m_blue;
	//wxTextCtrl* m_alpha;

	//wxBitmap m_coloursBmp;
	wxColour m_adjustColor;

    DECLARE_EVENT_TABLE()
	void OnAddCustom(wxCommandEvent& event);

	void OnApplyToObj(wxCommandEvent& event);

	//void OnOk(wxCommandEvent& event);
	//void OnCancel(wxCommandEvent& event);

	void OnMouseEvent(wxMouseEvent& event);
	void OnPaint(wxPaintEvent& event);

	void OnBrushTypeChanged(wxCommandEvent& event);
};

#endif
