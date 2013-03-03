#include "WXGraphDialog.h"
#include <wx/textfile.h>
#include "ActorAnimationSettingEditor.h"
#include "WXEffectEditorFrame.h"
#include "EffectObjectEditor.h"
#include "EffectSystem/FairyParticleElement.h"
#include "EffectSystem/FairyMeshElement.h"
#include "EffectSystem/FairyBillboardSetElement.h"
#include "EffectSystem/FairyBeamElement.h"
#include "EffectSystem/FairyProjectorElement.h"
#include "EffectSystem/FairyAnimationEffectInfo.h"
#include "EffectSystem/FairyAnimationSceneLightInfo.h"
#include "EffectSystem/FairyAnimationBulletFlow.h"
#include "EffectSystem/FairyAnimationCameraShake.h"
#include "EffectSystem/FairyAnimationLine.h"
#include "EffectSystem/FairyBulletSystem.h"
#include "EffectSystem/FairyBulletOperator.h"
#include "EffectSystem/FairyBulletTester.h"
#include "SkillObjectEditor.h"
#include "BulletEditor.h"
#include "EffectObjectEditor.h"
#include "EffectSystem/FairyEffect.h"
#include "WXEffectEditorFrame.h"
#include "OgreStringConverter.h"
const int ID_BUTTON_PLAY = wxNewId();
const int ID_BUTTON_STOP = wxNewId();
const int ID_BUTTON_STEP_BACK = wxNewId();
const int ID_BUTTON_STEP_FRONT = wxNewId();
const int ID_CHECKBOX_LOOP = wxNewId();
const int ID_BUTTON_LOOP = wxNewId();
const int ID_SLIDER_ANIM_RATE = wxNewId();


IMPLEMENT_DYNAMIC_CLASS(wxGraphDialog, wxScrolledWindow)

BEGIN_EVENT_TABLE(wxGraphDialog, wxScrolledWindow)
EVT_PAINT(      wxGraphDialog::OnPaint)
EVT_SIZE(       wxGraphDialog::OnSize)
EVT_BUTTON(ID_BUTTON_LOOP, wxGraphDialog::OnLoopButtonDown)
EVT_UPDATE_UI(ID_SLIDER_ANIM_RATE,wxGraphDialog::OnAnimRateUpdate)
EVT_BUTTON(ID_BUTTON_PLAY,wxGraphDialog::OnPlayButtonDown)
EVT_BUTTON(ID_BUTTON_STOP,wxGraphDialog::OnStopButtonDown)
EVT_BUTTON(ID_BUTTON_STEP_BACK,wxGraphDialog::OnStepBackButtonDown)
EVT_BUTTON(ID_BUTTON_STEP_FRONT,wxGraphDialog::OnStepFrontButtonDown)
EVT_TIMER(TIMER, wxGraphDialog::OnTimer)
END_EVENT_TABLE()

wxGraphDialog::wxGraphDialog( wxWindow *parent, wxWindowID id,
								   const wxPoint &pos, const wxSize &size ,const long style)
								   : wxScrolledWindow( parent, id, pos, size, style, _T("test canvas")),
								   mLoopButton(NULL),mLoop(false),mSliderAnimRate(NULL),m_Frame(NULL),
								   mfAnimTimePos(0), mFramePerSecond(25.0f), mTotalFrame(0.0f),mEditType(eGraphEidt_null),
								   mBulletFlow(NULL),mBulletFlowTemplateName("")
{

	Init();

	m_topLabel = new wxGraphTopLabels( this, wxID_ANY, wxDefaultPosition, wxSize(wxDefaultCoord,iTopLabelHeight) );
	wxGraphLeftLabels *right = new wxGraphLeftLabels( this,m_table,wxID_ANY, wxDefaultPosition, wxSize(iLeftLabelWidth,wxDefaultCoord) );
	wxPanel* left_top = new wxPanel(this,wxID_ANY,wxDefaultPosition,wxSize(iLeftLabelWidth,iTopLabelHeight));

	m_canvas = new wxGraphCanvas( this, m_topLabel, right, wxID_ANY, wxDefaultPosition, wxDefaultSize );

	SetTargetWindow( m_canvas );

	SetBackgroundColour( wxT("WHEAT") );

	SetCursor( wxCursor( wxCURSOR_HAND ) );


	wxBoxSizer *mainsizer = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer *topsizer = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer *left_topSizer = new wxBoxSizer( wxHORIZONTAL );

	wxBitmapButton *item6 = new wxBitmapButton( left_top, ID_BUTTON_PLAY, AnimationSettingBitmap( 0 ), wxDefaultPosition, wxDefaultSize);
	left_topSizer->Add( item6, 0, wxALIGN_CENTER|wxALL, 0 );

	wxBitmapButton *item7 = new wxBitmapButton( left_top, ID_BUTTON_STOP, AnimationSettingBitmap( 1 ), wxDefaultPosition, wxDefaultSize);
	left_topSizer->Add( item7, 0, wxALIGN_CENTER|wxALL, 0 );

	wxBitmapButton *item8 = new wxBitmapButton( left_top, ID_BUTTON_STEP_BACK, AnimationSettingBitmap( 2 ), wxDefaultPosition, wxDefaultSize);
	left_topSizer->Add( item8, 0, wxALIGN_CENTER|wxALL, 0 );

	wxBitmapButton *item9 = new wxBitmapButton( left_top, ID_BUTTON_STEP_FRONT, AnimationSettingBitmap( 3 ), wxDefaultPosition, wxDefaultSize);
	left_topSizer->Add( item9, 0, wxALIGN_CENTER|wxALL,0 );

	mLoopButton = new wxBitmapButton(left_top,ID_BUTTON_LOOP,wxBITMAP(LOOP),wxDefaultPosition,wxDefaultSize);
	left_topSizer->Add( mLoopButton, 0, wxALIGN_CENTER|wxALL,0 );
	mLoopButton->SetToolTip(wxT("循环"));
	mLoop = true;

	mSliderAnimRate = new wxSlider(left_top, ID_SLIDER_ANIM_RATE, 5, 0, 10, wxDefaultPosition, wxSize(60,16), wxSL_HORIZONTAL|wxSL_AUTOTICKS );
	left_topSizer->Add( mSliderAnimRate, 0, wxALIGN_CENTER|wxALL, 0 );
	mSliderAnimRate->SetToolTip(wxT("速率"));

	left_topSizer->SetMinSize(wxSize(iLeftLabelWidth,iTopLabelHeight));

	left_top->SetSizer(left_topSizer);



	topsizer->Add(left_top,0,wxEXPAND);
	topsizer->Add( m_topLabel, 1, wxEXPAND );

	mainsizer->Add( topsizer, 0, wxEXPAND );


	

	wxBoxSizer *middlesizer = new wxBoxSizer( wxHORIZONTAL );
	middlesizer->Add( right, 0, wxEXPAND );
	middlesizer->Add( m_canvas, 1, wxEXPAND );

	mainsizer->Add( middlesizer, 1, wxEXPAND );

	mParticleSystem = NULL;
	mParticleSystemTemplateName = "";

	mEffect = NULL;
	mEffectTemplateName = "";

	mSkill = NULL;
	mSkillTemplateName = "";


	SetAutoLayout( true );
	SetSizer( mainsizer );
}
void wxGraphDialog::Init()
{
	iLeftLabelWidth = 200;
	iLeftLabelHeight = 18;
	iTopLabelHeight =  22;


	m_iRulerSize = 0;
	m_fRulerRate =20.0f;
	m_GridLineSpace = 4;
	m_numRows = 0;
	m_CrrentTime = 0.0f;
	m_TimeField = 0.0f;
	m_LabelTextHorizAlign = wxALIGN_LEFT;
	m_LabelTextVertAlign = wxALIGN_CENTRE;

	m_table = new wxGraphTable();
	m_table->SetView( this );

	mTimer.SetOwner(this, TIMER);
	mTimer.Start(100);

}

void wxGraphDialog::OnSize( wxSizeEvent &WXUNUSED(event) )
{
	// We need to override OnSize so that our scrolled
	// window a) does call Layout() to use sizers for
	// positioning the controls but b) does not query
	// the sizer for their size and use that for setting
	// the scrollable area as set that ourselves by
	// calling SetScrollbar() further down.

	Layout();

	AdjustScrollbars();
}

void wxGraphDialog::OnPaint( wxPaintEvent &WXUNUSED(event) )
{
	wxPaintDC dc( this );

	/*
	wxSize size( GetClientSize() );

	long w,h;
	dc.GetTextExtent( wxT("Headline"), &w, &h );

	dc.DrawText( wxT("Headline"), long (size.x / 2 - w / 2), 10 );
	*/
}
void wxGraphDialog::GetLabelTextAlignment( int *horiz, int *vert )
{
	*horiz = m_LabelTextHorizAlign;
	*vert  = m_LabelTextVertAlign;

}
void wxGraphDialog::StringToLines( const wxString& value, wxArrayString& lines )
{
	int startPos = 0;
	int pos;
	wxString eol = wxTextFile::GetEOL( wxTextFileType_Unix );
	wxString tVal = wxTextFile::Translate( value, wxTextFileType_Unix );

	while ( startPos < (int)tVal.Length() )
	{
		pos = tVal.Mid(startPos).Find( eol );
		if ( pos < 0 )
		{
			break;
		}
		else if ( pos == 0 )
		{
			lines.Add( wxEmptyString );
		}
		else
		{
			lines.Add( value.Mid(startPos, pos) );
		}
		startPos += pos+1;
	}
	if ( startPos < (int)value.Length() )
	{
		lines.Add( value.Mid( startPos ) );
	}
}
void wxGraphDialog::DrawTextRectangle( wxDC& dc,
									  const wxString& value,
									  const wxRect& rect,
									  int horizAlign,
									  int vertAlign,
									  int textOrientation )
{
	wxArrayString lines;

	StringToLines( value, lines );


	//Forward to new API.
	DrawTextRectangle(  dc,
		lines,
		rect,
		horizAlign,
		vertAlign,
		textOrientation );

}

void wxGraphDialog::DrawTextRectangle( wxDC& dc,
									  const wxArrayString& lines,
									  const wxRect& rect,
									  int horizAlign,
									  int vertAlign,
									  int textOrientation )
{
	long textWidth, textHeight;
	long lineWidth, lineHeight;
	int nLines;

	dc.SetClippingRegion( rect );

	nLines = lines.GetCount();
	if( nLines > 0 )
	{
		int l;
		float x = 0.0, y = 0.0;

		if( textOrientation == wxHORIZONTAL )
			GetTextBoxSize(dc, lines, &textWidth, &textHeight);
		else
			GetTextBoxSize( dc, lines, &textHeight, &textWidth );

		switch( vertAlign )
		{
		case wxALIGN_BOTTOM:
			if( textOrientation == wxHORIZONTAL )
				y = rect.y + (rect.height - textHeight - 1);
			else
				x = rect.x + rect.width - textWidth;
			break;

		case wxALIGN_CENTRE:
			if( textOrientation == wxHORIZONTAL )
				y = rect.y + ((rect.height - textHeight)/2);
			else
				x = rect.x + ((rect.width - textWidth)/2);
			break;

		case wxALIGN_TOP:
		default:
			if( textOrientation == wxHORIZONTAL )
				y = rect.y + 1;
			else
				x = rect.x + 1;
			break;
		}

		// Align each line of a multi-line label
		for( l = 0; l < nLines; l++ )
		{
			dc.GetTextExtent(lines[l], &lineWidth, &lineHeight);

			switch( horizAlign )
			{
			case wxALIGN_RIGHT:
				if( textOrientation == wxHORIZONTAL )
					x = rect.x + (rect.width - lineWidth - 1);
				else
					y = rect.y + lineWidth + 1;
				break;

			case wxALIGN_CENTRE:
				if( textOrientation == wxHORIZONTAL )
					x = rect.x + ((rect.width - lineWidth)/2);
				else
					y = rect.y + rect.height - ((rect.height - lineWidth)/2);
				break;

			case wxALIGN_LEFT:
			default:
				if( textOrientation == wxHORIZONTAL )
					x = rect.x + 1;
				else
					y = rect.y + rect.height - 1;
				break;
			}

			if( textOrientation == wxHORIZONTAL )
			{
				dc.DrawText( lines[l], (int)x, (int)y );
				y += lineHeight;
			}
			else
			{
				dc.DrawRotatedText( lines[l], (int)x, (int)y, 90.0 );
				x += lineHeight;
			}
		}
	}
	dc.DestroyClippingRegion();
}
void wxGraphDialog::GetTextBoxSize( wxDC& dc,
								   const wxArrayString& lines,
								   long *width, long *height )
{
	long w = 0;
	long h = 0;
	long lineW, lineH;

	size_t i;
	for ( i = 0;  i < lines.GetCount();  i++ )
	{
		dc.GetTextExtent( lines[i], &lineW, &lineH );
		w = wxMax( w, lineW );
		h += lineH;
	}

	*width = w;
	*height = h;
}
void wxGraphDialog::SetCrrentTime(float time)
{
	m_CrrentTime = time;
}

// wxGraphTopLabels

IMPLEMENT_DYNAMIC_CLASS(wxGraphTopLabels,wxWindow)

BEGIN_EVENT_TABLE(wxGraphTopLabels, wxWindow)
EVT_PAINT(      wxGraphTopLabels::OnPaint)
END_EVENT_TABLE()

wxGraphTopLabels::wxGraphTopLabels( wxGraphDialog *parent, wxWindowID id, const wxPoint &pos, const wxSize &size )
: wxWindow( parent, id, pos, size )
{
	m_owner = parent;
	mTime = 0.0f;
}

void wxGraphTopLabels::OnPaint( wxPaintEvent& WXUNUSED(event) )
{
	wxPaintDC dc(this);

	// This is wrong..  it will translate both x and y if the
	// window is scrolled, the label windows are active in one
	// direction only.  Do the action below instead -- RL.
	//m_owner->PrepareDC( dc );

	int xScrollUnits, xOrigin;

	m_owner->GetViewStart( &xOrigin, 0 );
	m_owner->GetScrollPixelsPerUnit( &xScrollUnits, 0 );
	dc.SetDeviceOrigin( -xOrigin * xScrollUnits, 0 );

	wxRect rClientRect = GetClientSize();


	float fRulerRate = m_owner->GetRulerRate();
	int iGridSpace = m_owner->GetGridSpace();
	int iGridCount = floor(fRulerRate/iGridSpace);
	float fRate = fRulerRate/(float)iGridCount;
	int iRulerSize = (rClientRect.width - 4)/fRate;

	int iRight =0;
	int iTop1 = 16;
	int iTop2 = 10;
	int iTop3 = 3;
	char NumBuffer[16];
	wxRect NumTextRect;
	int hAlign, vAlign;
	m_owner->GetLabelTextAlignment( &hAlign, &vAlign );

	float fTimeFieldWidth = fRulerRate* m_owner->GetTimeField();
	wxRect TimeFieldRect = wxRect(wxPoint(6,0),wxSize(fTimeFieldWidth > rClientRect.width ? 
		rClientRect.width : fTimeFieldWidth,rClientRect.height));
	dc.SetBrush(*wxLIGHT_GREY_BRUSH);
	dc.DrawRectangle(TimeFieldRect);
	
	for(int i =0;i<iRulerSize;i++)
	{
		iRight = i* fRate + 6;

		if(i%(5*iGridCount) == 0)
		{
			dc.SetPen( wxPen(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT),1, wxSOLID) );
			dc.DrawLine(iRight,iTop3,iRight,rClientRect.height- 1);
			NumTextRect.x = iRight;
			NumTextRect.y = 1;
			NumTextRect.width = fRulerRate * 10;
			NumTextRect.height = rClientRect.height- 3;
			sprintf(NumBuffer,"%d",i/iGridCount);
			m_owner->DrawTextRectangle(dc,NumBuffer,NumTextRect, hAlign, vAlign);
		}
		else if(i%iGridCount == 0)
		{
			dc.SetPen( wxPen(wxSystemSettings::GetColour(wxSYS_COLOUR_3DDKSHADOW),1, wxSOLID) );
			dc.DrawLine(iRight,iTop2,iRight,rClientRect.height- 1);

		}
		else
		{
			dc.SetPen( wxPen(wxSystemSettings::GetColour(wxSYS_COLOUR_3DDKSHADOW),1, wxSOLID) );
			dc.DrawLine(iRight,iTop1,iRight,rClientRect.height - 1);
		}
	}
	float fCurrentTime = m_owner->GetCrrentTime();
	float fRluerRate = m_owner->GetRulerRate();
	int iRuler = fRluerRate* fCurrentTime + 6;

	//绘制时间线

	dc.SetPen(*wxRED_PEN);
	dc.DrawLine(iRuler,0,iRuler,rClientRect.height - 1);
}
// wxGraphLeftLabels

IMPLEMENT_DYNAMIC_CLASS(wxGraphLeftLabels,wxWindow)

BEGIN_EVENT_TABLE(wxGraphLeftLabels, wxWindow)
EVT_PAINT(wxGraphLeftLabels::OnPaint)
EVT_LEFT_UP(wxGraphLeftLabels::OnLeftUp)
EVT_LEFT_DCLICK(wxGraphLeftLabels::OnDClick)
END_EVENT_TABLE()

wxGraphLeftLabels::wxGraphLeftLabels( wxGraphDialog *parent,wxGraphTable *table,wxWindowID id, const wxPoint &pos, const wxSize &size )
: wxWindow( parent, id, pos, size ),m_owner(parent),m_table(table)
{
}

void wxGraphLeftLabels::OnPaint( wxPaintEvent& WXUNUSED(event) )
{
	wxPaintDC dc(this);

	// This is wrong..  it will translate both x and y if the
	// window is scrolled, the label windows are active in one
	// direction only.  Do the action below instead -- RL.
	//m_owner->PrepareDC( dc );

	int yScrollUnits, yOrigin;

	m_owner->GetViewStart( 0, &yOrigin );
	m_owner->GetScrollPixelsPerUnit( 0, &yScrollUnits );
	dc.SetDeviceOrigin( 0, -yOrigin * yScrollUnits );

	m_owner->DrawRowLabels(dc,m_owner->GetRows());
}
void wxGraphLeftLabels::OnLeftUp(wxMouseEvent &e)
{
	wxCoord x = e.GetX();
	wxCoord y = e.GetY();
	int iSelectIndex = calculateSelectItem(y);
	
	if(iSelectIndex == -1)
		return;
	if(m_table->GetGraphType(iSelectIndex) == wxGraphDialog::eGraphItem_particle_emitter)
	{
		Ogre::ParticleEmitter* pEmitter = (Ogre::ParticleEmitter* )m_table->GetRowData(iSelectIndex);
		//m_owner->GetParentFrame()->GetParticleObjectEditor()->SelectEmitterItem(pEmitter);
	}
	else if(m_table->GetGraphType(iSelectIndex) == wxGraphDialog::eGraphItem_particle_affector)
	{
		Ogre::ParticleAffector* pAffector = (Ogre::ParticleAffector* )m_table->GetRowData(iSelectIndex);
		//m_owner->GetParentFrame()->GetParticleObjectEditor()->SelectAffectorItem(pAffector);
	}
	else if(m_table->GetGraphType(iSelectIndex) > wxGraphDialog::eGraphItem_effect &&
		m_table->GetGraphType(iSelectIndex) < wxGraphDialog::eGraphItem_skill)
	{
		Fairy::EffectElement* pElement = (Fairy::EffectElement* )m_table->GetRowData(iSelectIndex);
		m_owner->GetParentFrame()->GetEffectObjectEditor()->SelectElementItem(pElement);
	}
	else if(m_table->GetGraphType(iSelectIndex) > wxGraphDialog::eGraphItem_skill && 
		m_table->GetGraphType(iSelectIndex) < wxGraphDialog::eGraphItem_bullet )
	{
		Ogre::StringInterface* pElement = (Ogre::StringInterface* )m_table->GetRowData(iSelectIndex);
		m_owner->GetParentFrame()->GetSkillObjectEidtor()->SelectElementItem(pElement);
	}
	else if(m_table->GetGraphType(iSelectIndex) == wxGraphDialog::eGraphItem_bullet_bulletEvent)
	{
		Fairy::BulletEventSystem* pEvent = (Fairy::BulletEventSystem* )m_table->GetRowData(iSelectIndex);
		m_owner->GetParentFrame()->GetBulletEditor()->SelectBulletEvent(pEvent);
	}
	else if(m_table->GetGraphType(iSelectIndex) == wxGraphDialog::eGraphItem_bullet_bulletTester ||
		m_table->GetGraphType(iSelectIndex) == wxGraphDialog::eGraphItem_bullet_bulletOperator)
	{
		Fairy::BulletOperator* pOperator = (Fairy::BulletOperator* )m_table->GetRowData(iSelectIndex);
		m_owner->GetParentFrame()->GetBulletEditor()->SelectBulletOperator(pOperator);
	}
	else if(m_table->GetGraphType(iSelectIndex) == wxGraphDialog::eGraphItem_bullet_bulletSystem)
	{
		Fairy::BulletSystem* pSystem = (Fairy::BulletSystem* )m_table->GetRowData(iSelectIndex);
		m_owner->GetParentFrame()->GetBulletEditor()->SelectBulletSystem(pSystem);
	}
	else if(m_table->GetGraphType(iSelectIndex) == wxGraphDialog::eGraphItem_bullet_animEffect ||
		m_table->GetGraphType(iSelectIndex) == wxGraphDialog::eGraphItem_bullet_cameraShake ||
		m_table->GetGraphType(iSelectIndex) == wxGraphDialog::eGraphItem_bullet_Ribbon ||
		m_table->GetGraphType(iSelectIndex) == wxGraphDialog::eGraphItem_bullet_Sound ||
		m_table->GetGraphType(iSelectIndex) == wxGraphDialog::eGraphItem_bullet_SceneLight||
		m_table->GetGraphType(iSelectIndex) == wxGraphDialog::eGraphItem_bullet_line )
	{
		Ogre::StringInterface* pElement = (Ogre::StringInterface* )m_table->GetRowData(iSelectIndex);
		m_owner->GetParentFrame()->GetBulletEditor()->SelectBulletSystemElement(pElement);
	}

}
void wxGraphLeftLabels::OnDClick(wxMouseEvent& e)
{
	wxCoord x = e.GetX();
	wxCoord y = e.GetY();
	int iSelectIndex = calculateSelectItem(y);

	if(iSelectIndex == -1)
		return;
	bool enable = true;
	if(m_table->GetRowEnable(iSelectIndex))
		enable = false;
	else
		enable = true;

	m_table->SetRowEnable(iSelectIndex,enable);
	if(m_table->GetGraphType(iSelectIndex) > wxGraphDialog::eGraphItem_effect &&
		m_table->GetGraphType(iSelectIndex) < wxGraphDialog::eGraphItem_skill)
	{
		Fairy::Effect* pEffect = m_owner->GetParentFrame()->GetEffectObjectEditor()->getCurrentObjectEffect();
		if(pEffect)
		{
			Fairy::EffectElement* pElement = pEffect->getElement(iSelectIndex);
			pElement->setVisible(enable);
		}
		Fairy::EffectElement* pElement = (Fairy::EffectElement* )m_table->GetRowData(iSelectIndex);
		pElement->setVisible(enable);
	}
	else if(m_table->GetGraphType(iSelectIndex) > wxGraphDialog::eGraphItem_skill )
	{
		Fairy::Skill* pSkill = m_owner->GetParentFrame()->GetSkillObjectEidtor()->getCurrentObjectSkill();
		if(!pSkill)
			return;
	
		//Fairy::EffectElement* pElement = pEffect->getElement(iSelectIndex);
		//pElement->setVisible(enable);
		if(m_table->GetGraphType(iSelectIndex) == wxGraphDialog::eGraphItem_skill_animEffect)
		{
			Fairy::AnimationEffectInfo* pEffectInfo = (Fairy::AnimationEffectInfo*)m_table->GetRowData(iSelectIndex);
			int index = -1;
			for(int i = 0;i<m_owner->mSkill->getNumAnimationEffectInfos();i++)
			{
				if(m_owner->mSkill->getAnimationEffectInfo(i) == pEffectInfo)
				{
					index = i;
					break;
				}
			}
			if(index == -1)
				return;
			Fairy::AnimationEffectInfo* pCurrentEffectInfo = pSkill->getAnimationEffectInfo(index);
			if(pCurrentEffectInfo)
			{
				Fairy::Effect* pEffect = pCurrentEffectInfo->getEffect();
				if(pEffect)
				{
					pEffect->setVisible(enable);
				}

			}
		}

	}
	Refresh();

}
int wxGraphLeftLabels::calculateSelectItem(wxCoord y)
{
	int i= -1;
	i = (y+1)/m_owner->iLeftLabelHeight;
	if(i >= m_table->GetNumberRows())
		i = -1;
	return i;
}
// wxGraphCanvas

IMPLEMENT_DYNAMIC_CLASS(wxGraphCanvas, wxPanel)

BEGIN_EVENT_TABLE(wxGraphCanvas, wxPanel)
EVT_PAINT(      wxGraphCanvas::OnPaint)
END_EVENT_TABLE()

wxGraphCanvas::wxGraphCanvas( wxGraphDialog *parent, wxGraphTopLabels *top, wxGraphLeftLabels *right,
				   wxWindowID id, const wxPoint &pos, const wxSize &size ,const long style)
				   : wxPanel( parent, id, pos, size, style, _T("test canvas") )
{
	m_owner = parent;
	m_topLabels = top;
	m_rightLabels = right;


	SetBackgroundColour( wxT("WHEAT") );

	SetCursor( wxCursor( wxCURSOR_IBEAM ) );
}

void wxGraphCanvas::OnPaint( wxPaintEvent &WXUNUSED(event) )
{
	wxPaintDC dc( this );
	//m_owner->PrepareDC( dc );

	dc.SetPen( *wxBLACK_PEN );

	// OK, let's assume we are a grid control and we have two
	// grid cells. Here in OnPaint we want to know which cell
	// to redraw so that we prevent redrawing cells that don't
	// need to get redrawn. We have one cell at (0,0) and one
	// more at (200,0), both having a size of (100,25).

	// We can query how much the window has been scrolled
	// by calling CalcUnscrolledPosition()

	int scroll_x = 0;
	int scroll_y = 0;
	m_owner->CalcUnscrolledPosition( scroll_x, scroll_y, &scroll_x, &scroll_y );

	// We also need to know the size of the window to see which
	// cells are completely hidden and not get redrawn

	int size_x = 0;
	int size_y = 0;
	GetClientSize( &size_x, &size_y );


	int yScrollUnits, yOrigin;
	int xScrollUnits, xOrigin;

	m_owner->GetViewStart( &xOrigin, &yOrigin );
	m_owner->GetScrollPixelsPerUnit( &xScrollUnits, &yScrollUnits );
	dc.SetDeviceOrigin( -xOrigin * xScrollUnits, -yOrigin * yScrollUnits );

	m_owner->DrawAllGridLines(dc);
	m_owner->DrawGridCellArea(dc);
	//m_owner->DrawTimeLine(dc);
}



void wxGraphCanvas::ScrollWindow( int dx, int dy, const wxRect *rect )
{
	wxPanel::ScrollWindow( dx, dy, rect );
	m_topLabels->ScrollWindow( dx, 0, rect );
	m_rightLabels->ScrollWindow( 0, dy, rect );
}

void wxGraphDialog::DrawGridCellArea( wxDC& dc )
{
	if ( !m_numRows ) return;

	for ( int i = 0; i < m_numRows;  i++ )
	{
		DrawCell( dc, i );

	}

}

//
void wxGraphDialog::DrawAllGridLines( wxDC& dc)
{
	int top, bottom, left, right;
	int cw, ch;
	m_canvas->GetClientSize(&cw, &ch);
	CalcUnscrolledPosition( 0, 0, &left, &top );
	CalcUnscrolledPosition( cw, ch, &right, &bottom );



	int iRulerBottom = ch -1;
	int iGridCount = floor(m_fRulerRate/m_GridLineSpace);
	float fRate = m_fRulerRate/(float)iGridCount;
	m_iRulerSize = (cw-4)/fRate;

	int iRight =0;
	int i;
	for( i=0;i<m_iRulerSize;i++)
	{
		iRight = left + i* fRate + 6;

		
		//绘制实现坐标格
		if(i%(5*iGridCount) == 0)
		{
			dc.SetPen( wxPen(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT),1, wxSOLID) );
			dc.DrawLine(iRight,0,iRight,ch- 1);
		}
		else if(i%iGridCount == 0)
		{
			dc.SetPen( wxPen(wxSystemSettings::GetColour(wxSYS_COLOUR_3DDKSHADOW),1, wxSOLID) );
			dc.DrawLine(iRight,0,iRight,ch- 1);

		}
		else
		{
			dc.SetPen( wxPen(wxSystemSettings::GetColour(wxSYS_COLOUR_3DDKSHADOW),1, wxSOLID) );
			dc.DrawLine(iRight,0,iRight,ch - 1);
		}
	
	}
	//
	dc.SetPen( wxPen(wxSystemSettings::GetColour(wxSYS_COLOUR_3DDKSHADOW),1, wxSOLID) );
	int iHeight=0;
	for( i=0;i <= m_numRows;i++)
	{
		iHeight = i* iLeftLabelHeight;
		dc.DrawLine(0,iHeight,cw,iHeight);

	}


}
//绘制时间线
void wxGraphDialog::DrawTimeLine(wxDC& dc)
{
	int top, bottom, left, right;
	int cw, ch;
	m_canvas->GetClientSize(&cw, &ch);
	CalcUnscrolledPosition( 0, 0, &left, &top );
	CalcUnscrolledPosition( cw, ch, &right, &bottom );

	int iRight = left + m_fRulerRate * m_CrrentTime + 6;

	dc.SetPen( *wxRED_PEN);
	dc.DrawLine(iRight,0,iRight,ch - 1);

}
void wxGraphDialog::DrawCell( wxDC& dc, int row )
{

	if ( iLeftLabelHeight <= 0 )
		return;

	bool isCurrent = row == m_currentCell;

	int cw, ch;
	m_canvas->GetClientSize(&cw, &ch);
	int iGridCount = floor(m_fRulerRate/m_GridLineSpace);
	float fRate = m_fRulerRate/(float)iGridCount;
	m_iRulerSize = (cw-4)/fRate;

	float fStart = m_table->GetStart(row);
	float fEnd = m_table->GetEnd(row);
	int iHeight = iLeftLabelHeight;
	int iTop = iLeftLabelHeight * row;

	wxColor color;
	int iType = m_table->GetGraphType(row);
	if(iType == (int)eGraphItem_particle_emitter)
	{
		dc.SetPen(*wxGREEN_PEN);
		dc.SetBrush(*wxGREEN_BRUSH);
		fStart = 0.0f;
		fEnd = m_iRulerSize;
	}
	else if(iType == (int)eGraphItem_particle_affector)
	{
		dc.SetPen( wxPen(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT),1, wxSOLID) );
		dc.SetBrush( wxBrush(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT),wxSOLID));
		fStart = 0;
		fEnd = m_iRulerSize;
	}
	else if(iType > eGraphItem_effect && iType < eGraphItem_skill)
	{
		dc.SetPen( wxPen(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT),1, wxSOLID) );
		dc.SetBrush( wxBrush(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT),wxSOLID));
		if(mEffect->getLifeTime() < 0.001 && fStart == fEnd)
		{
			fEnd = m_iRulerSize;
		}
		else if(mEffect->getLifeTime() > 0.001 && fStart == fEnd)
		{
			fEnd = mEffect->getLifeTime();
		}
	}
	else if(iType > eGraphItem_skill &&  iType < eGraphItem_bullet)
	{
		dc.SetPen( wxPen(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT),1, wxSOLID) );
		dc.SetBrush( wxBrush(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT),wxSOLID));
		if(fStart == 0)
		{
			fEnd = m_iRulerSize;
		}
		else if(fStart> 0.0f)
		{
			fEnd = m_iRulerSize -fStart;
		}
	}
	else if(iType == eGraphItem_bullet_bulletEvent)
	{
		dc.SetPen( wxPen(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT),1, wxSOLID) );
		dc.SetBrush( wxBrush(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT),wxSOLID));
		if(fStart == 0)
		{
			fEnd = m_iRulerSize;
		}
		else if(fStart> 0.0f)
		{
			fEnd = m_iRulerSize -fStart;
		}
	}
	else if(iType == eGraphItem_bullet_bulletSystem)
	{
		dc.SetPen( wxPen(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT),1, wxSOLID) );
		dc.SetBrush( wxBrush(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT),wxSOLID));
		if(fStart == 0)
		{
			fEnd = m_iRulerSize;
		}
		else if(fStart> 0.0f)
		{
			fEnd = m_iRulerSize -fStart;
		}
	}
	else if(iType == eGraphItem_bullet_bulletTester)
	{
		dc.SetPen(*wxGREEN_PEN);
		dc.SetBrush(*wxGREEN_BRUSH);
		if(fStart == 0)
		{
			fEnd = m_iRulerSize;
		}
		else if(fStart> 0.0f)
		{
			fEnd = m_iRulerSize -fStart;
		}
	}
	else if(iType == eGraphItem_bullet_bulletOperator)
	{
		dc.SetPen(*wxGREEN_PEN);
		dc.SetBrush(*wxGREEN_BRUSH);
		if(fStart == 0)
		{
			fEnd = m_iRulerSize;
		}
		else if(fStart> 0.0f)
		{
			fEnd = m_iRulerSize -fStart;
		}
	}
	else if(iType == eGraphItem_bullet_animEffect || iType == eGraphItem_bullet_Ribbon ||
			iType == eGraphItem_bullet_cameraShake || iType == eGraphItem_bullet_Sound ||
			iType == eGraphItem_bullet_SceneLight || iType == eGraphItem_bullet_line)
	{
		dc.SetPen(*wxGREEN_PEN);
		dc.SetBrush(*wxGREEN_BRUSH);
		if(fStart == 0)
		{
			fEnd = m_iRulerSize;
		}
		else if(fStart> 0.0f)
		{
			fEnd = m_iRulerSize -fStart;
		}
	}
	else 
	{
		dc.SetPen( wxPen(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT),1, wxSOLID) );
		dc.SetBrush( wxBrush(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT),wxSOLID));
	}

	if(fStart < 0.001 && fEnd < 0.001 || fStart >= fEnd)
		return;

	dc.DrawRectangle(wxRect(wxPoint(fStart* m_fRulerRate + 6,iTop+ iHeight/2 -3),wxSize((fEnd - fStart)*m_fRulerRate,6)));

	/*dc.DrawLine( iStart* m_fRulerRate + 6, iTop+ iHeight/2 -3,iEnd * m_fRulerRate + 6, iTop+ iHeight/2 -3 );
	dc.DrawLine( iStart* m_fRulerRate + 6, iTop+ iHeight/2 +3,iEnd * m_fRulerRate + 6, iTop+ iHeight/2 +3 );
	dc.DrawLine( iStart* m_fRulerRate + 6, iTop+ iHeight/2 -5,iStart* m_fRulerRate + 6, iTop+ iHeight/2 +3 );
	dc.DrawLine( iEnd * m_fRulerRate + 7, iTop+ iHeight/2 -5,iEnd * m_fRulerRate + 7, iTop+ iHeight/2 +3 );*/


}

void wxGraphDialog::DrawRowLabels( wxDC& dc ,const int rows)
{
	if ( !m_numRows ) return;
	size_t i;
	size_t numLabels = rows;

	for ( i = 0;  i < numLabels;  i++ )
	{
		DrawRowLabel( dc, i );
	}
}
void wxGraphDialog::DrawRowLabel(wxDC& dc, const int row )
{
	if( row< 0 )
		return;

	wxRect rect;

	int rowTop = row * iLeftLabelHeight;

	int rowBottom = (row + 1) * iLeftLabelHeight;

	dc.SetPen( wxPen(wxSystemSettings::GetColour(wxSYS_COLOUR_3DDKSHADOW),1, wxSOLID) );
	dc.DrawLine( iLeftLabelWidth-1, rowTop,
		iLeftLabelWidth-1, rowBottom );

	dc.DrawLine( 0, rowTop, 0, rowBottom );

	dc.DrawLine( 0, rowBottom, iLeftLabelWidth, rowBottom );

	dc.SetPen( *wxWHITE_PEN );
	dc.DrawLine( 1, rowTop, 1, rowBottom );
	dc.DrawLine( 1, rowTop, iLeftLabelWidth-1, rowTop );

	dc.SetBackgroundMode( wxTRANSPARENT );
	//dc.SetTextForeground( GetLabelTextColour() );
	//dc.SetFont( GetLabelFont() );

	//绘制图标
	wxBitmap IconBitmap;
	int iType = m_table->GetGraphType(row);
	switch(iType)
	{
	case eGraphItem_particle_emitter:
		IconBitmap = wxBITMAP(PARTICLEEMITTER);
		break;
	case eGraphItem_particle_affector:
		IconBitmap = wxBITMAP(PARTICLEAFFECTOR);
		break;
	case eGraphItem_effect_particle:
		IconBitmap = wxBITMAP(PARTICLEITEM);
		break;
	case eGraphItem_effect_mesh:
		IconBitmap = wxBITMAP(MESH);
		break;
	case eGraphItem_effect_billboardSet:
		IconBitmap = wxBITMAP(BILLBOARDSET);
		break;
	case eGraphItem_effect_projector:
		IconBitmap = wxBITMAP(PROJECTOR);
		break;
	case eGraphItem_effect_beam:
		IconBitmap = wxBITMAP(BEAM);
		break;
	case eGraphItem_effect_pointLight:
		IconBitmap = wxBITMAP(POINTLIGHT);
		break;
	case eGraphItem_effect_cylinder:
		IconBitmap = wxBITMAP(CYLINDER);
		break;
	case eGraphItem_skill_animEffect:
		IconBitmap = wxBITMAP(ANIMEFFECT);
		break;
	case eGraphItem_skill_camaerShake:
		IconBitmap = wxBITMAP(CAMERASHAKE);
		break;
	case eGraphItem_skill_bibbon:
		IconBitmap = wxBITMAP(RIBBON);
		break;
	case eGraphItem_skill_Sound:
		IconBitmap = wxBITMAP(SOUND);
		break;
	case eGraphItem_skill_SceneLight:
		IconBitmap = wxBITMAP(SCENELIGHT);
		break;
	case eGraphItem_skill_BulletFlow:
		IconBitmap = wxBITMAP(ANIMBULLETFLOW);
		break;
	case eGraphItem_bullet_bulletSystem:
		IconBitmap = wxBITMAP(BULLETSYSTEM);
		break;
	case eGraphItem_bullet_bulletEvent:
		IconBitmap = wxBITMAP(EVENT);
		break;
	case eGraphItem_bullet_bulletTester:
		IconBitmap = wxBITMAP(TESTER);
		break;
	case eGraphItem_bullet_bulletOperator:
		IconBitmap = wxBITMAP(OPERATOR);
		break;
	case eGraphItem_bullet_animEffect:
		IconBitmap = wxBITMAP(ANIMEFFECT);
		break;
	case eGraphItem_bullet_cameraShake:
		IconBitmap = wxBITMAP(CAMERASHAKE);
		break;
	case eGraphItem_bullet_Ribbon:
		IconBitmap = wxBITMAP(RIBBON);
		break;
	case eGraphItem_bullet_Sound:
		IconBitmap = wxBITMAP(SOUND);
		break;
	case eGraphItem_bullet_SceneLight:
		IconBitmap = wxBITMAP(SCENELIGHT);
		break;
	case eGraphItem_bullet_line:
		IconBitmap = wxBITMAP(LINE);
		break;


	}
	dc.DrawBitmap(IconBitmap,1,rowTop + 1);


	int hAlign, vAlign;
	GetLabelTextAlignment( &hAlign, &vAlign );

	//画enable选择状态
	dc.SetPen( wxPen(wxSystemSettings::GetColour( wxSYS_COLOUR_BTNTEXT ),1, wxSOLID) );
	rect.SetX( 20);
	rect.SetY(  rowTop + iLeftLabelHeight/2-5);
	rect.SetWidth( 10 );
	rect.SetHeight( 10 );
	dc.DrawRectangle(rect);

	if(m_table->GetRowEnable(row))
	{
		dc.SetBrush(wxBrush(wxSystemSettings::GetColour( wxSYS_COLOUR_BTNTEXT )));
		rect.SetX( 21);
		rect.SetY(  rowTop + iLeftLabelHeight/2-4);
		rect.SetWidth( 8 );
		rect.SetHeight( 8 );
		dc.DrawRectangle(rect);
	}
	else
	{
		dc.SetBrush(wxBrush(*wxWHITE));
		rect.SetX( 21);
		rect.SetY(  rowTop + iLeftLabelHeight/2-4);
		rect.SetWidth( 8 );
		rect.SetHeight( 8 );
		dc.DrawRectangle(rect);


	}

	//绘制文字
	rect.SetX( 34);
	rect.SetY( rowTop + 2 );
	rect.SetWidth( iLeftLabelWidth - 20 );
	rect.SetHeight( iLeftLabelHeight - 4 );
	DrawTextRectangle( dc, m_table->GetRowLabelValue( row ), rect, hAlign, vAlign );

}

void wxGraphDialog::InitEffectEditor(Fairy::Effect *pEffect, const Ogre::String &name)
{
	mEffect = pEffect;
	mEffectTemplateName = name;
	m_TimeField = pEffect->getLifeTime();
	m_table->Clear();

	for ( unsigned short i=0; i<pEffect->getNumElements(); ++i )
	{
		Fairy::EffectElement *pElement = pEffect->getElement(i);
		assert (pElement);

		wxString elementType = pElement->getType().c_str();
		Ogre::String Label;
		float fStart = pElement->getStartTime();
		float fEnd = fStart + pElement->getLifeTime();
		int iType= 0;
		if(elementType == "Particle")
		{
			iType = eGraphItem_effect_particle;
			Fairy::ParticleElement* pParticle = dynamic_cast<Fairy::ParticleElement*>(pElement);
			if (pParticle)
			{
				Label =pParticle->getParticleSystemName();
			}
		}
		else if(elementType == "Mesh")
		{
			iType = eGraphItem_effect_mesh;
			Fairy::MeshElement* pMesh = dynamic_cast<Fairy::MeshElement*>(pElement);
			if (pMesh)
			{
				Label =pMesh->getMaterialName();
			}
		}
		else if(elementType == "BillboardSet")
		{
			iType = eGraphItem_effect_billboardSet;
			Fairy::BillboardSetElement* pBillboardSet = dynamic_cast<Fairy::BillboardSetElement*>(pElement);
			if (pBillboardSet)
			{
				Label =pBillboardSet->getBillboardMaterial();
			}
		}
		else if(elementType == "Projector")
		{
			iType = eGraphItem_effect_projector;
			Fairy::ProjectorElement* pProjector = dynamic_cast<Fairy::ProjectorElement*>(pElement);
			if (pProjector)
			{
				Label =pProjector->getMaterialName();
			}
		}
		else if(elementType == "Beam")
		{
			iType = eGraphItem_effect_beam;
			Fairy::BeamElement* pBeam = dynamic_cast<Fairy::BeamElement*>(pElement);
			if (pBeam)
			{
				Label =pBeam->getMaterialName();
			}
		}
		else if(elementType == "PointLight")
		{
			iType = eGraphItem_effect_pointLight;
			Label = "PointLight";
		}
		else if(elementType == "Cylinder")
		{
			iType = eGraphItem_effect_cylinder;
			Label = "Cylinder";
		}
		bool enable = pElement->getVisible();
		//Fairy::Effect* pEffect = m_Frame->GetEffectObjectEditor()->getCurrentObjectEffect();
		//if(pEffect)
		//{
		//	Fairy::EffectElement* pElement = pEffect->getElement(i);
			pElement->setVisible(enable);
		//}
		m_table->AppendRows(Label.c_str(),fStart,fEnd,iType,pElement,enable);
	}
	m_CrrentTime = 0.0f;
	Refresh();
	mEditType = eGraphEidt_effect;

}
void wxGraphDialog::InitSkillEditor(Fairy::Skill* pSkill,const Ogre::String &name)
{
	mSkill = pSkill;
	mSkillTemplateName = name;
	m_TimeField = 0.0f;
	m_table->Clear();
	unsigned short i=0;
	for ( i=0; i<pSkill->getNumAnimationEffectInfos(); ++i )
	{
		Fairy::AnimationEffectInfo* effectInfo = pSkill->getAnimationEffectInfo(i);
		wxString Lable = "EffectInfo";
		float fStart = effectInfo->getAttachTime();
		float fEnd = fStart + 100;
		int iType = eGraphItem_skill_animEffect;
		m_table->AppendRows(Lable,fStart,fEnd,iType,effectInfo);
	}
	for ( i=0; i<pSkill->getNumAnimationCameraShake(); ++i )
	{
		Fairy::AnimationCameraShake* cameraShake = pSkill->getAnimationCameraShake(i);
		wxString Lable = "CameraShake";
		float fStart = cameraShake->getAttachTime();
		float fEnd = fStart + 100;
		int iType = eGraphItem_skill_camaerShake;
		m_table->AppendRows(Lable,fStart,fEnd,iType,cameraShake);
	}
	
	for ( i=0; i<pSkill->getNumAnimationRibbons(); ++i )
	{
		Fairy::AnimationRibbon* ribbon = pSkill->getAnimationRibbon(i);
		wxString Lable = "Ribbon";
		float fStart = ribbon->getAttachTime();
		float fEnd = fStart + 100;
		int iType = eGraphItem_skill_bibbon;
		m_table->AppendRows(Lable,fStart,fEnd,iType,ribbon);
	}
	for (i=0; i < pSkill->getNumAnimationSounds(); ++i )
	{
		Fairy::AnimationSound *sound = pSkill->getAnimationSound(i);
		wxString Lable = "Sound";
		float fStart = sound->getAttachTime();
		float fEnd = fStart + 100;
		int iType = eGraphItem_skill_Sound;
		m_table->AppendRows(Lable,fStart,fEnd,iType,sound);
	}
	for ( i=0; i < pSkill->getNumAnimationSceneLightInfos(); ++i )
	{
		Fairy::AnimationSceneLightInfo *light = pSkill->getAnimationSceneLightInfo(i);
		wxString Lable = "Light";
		float fStart = light->getAttachTime();
		float fEnd = fStart + 100;
		int iType = eGraphItem_skill_SceneLight;
		m_table->AppendRows(Lable,fStart,fEnd,iType,light);
	}
	for ( i=0; i < pSkill->getNumAnimationBulletFlows(); ++i )
	{
		Fairy::AnimationBulletFlow *bulletFlow = pSkill->getAnimationBulletFlow(i);
		wxString Lable = "BulletFlow";
		float fStart = bulletFlow->getAttachTime();
		float fEnd = fStart + 100;
		int iType = eGraphItem_skill_BulletFlow;
		m_table->AppendRows(Lable,fStart,fEnd,iType,bulletFlow);
	}
	m_CrrentTime = 0.0f;
	Refresh();
	mEditType = eGraphEidt_skill;
}
void wxGraphDialog::InitBulletEditor(Fairy::BulletFlowSystem* pBulletFlow,std::map<Ogre::String,Fairy::BulletSystem*>& bulletSystemMap,
									 const Ogre::String &templateName)
{
	mBulletFlow = pBulletFlow;
	mBulletFlowTemplateName = templateName;
	m_table->Clear();
	Fairy::BulletFlowSystem::BulletEventMap::const_iterator it = mBulletFlow->getBulletEventSystemsMap().begin();
	while(it != mBulletFlow->getBulletEventSystemsMap().end())
	{
		Fairy::BulletEventSystem* pEvent = it->second;
		wxString Lable = pEvent->getTemplateName();
		float fStart = 0.0f;
		float fEnd = 100;
		int iType = eGraphItem_bullet_bulletEvent;
		m_table->AppendRows(Lable,fStart,fEnd,iType,pEvent);

		for(int index = 0; index < pEvent->getNumBulletOperator(); index++)
		{
			Fairy::BulletOperator* pOperator= pEvent->getBulletOperator(index);
			Fairy::BulletTester* pTester = dynamic_cast<Fairy::BulletTester*>(pOperator);
			if(pTester)
			{
				Lable = pTester->getType().c_str();
				float fStart = 0.0f;
				if(pTester->getType() == "age")
				{
					fStart = Ogre::StringConverter::parseReal(pTester->getParameter("maxTime"));
				}
				float fEnd = 100;
				int iTesterType = eGraphItem_bullet_bulletTester;
				m_table->AppendRows(Lable,fStart,fEnd,iTesterType,pTester);

			}
			else
			{
				Lable = pOperator->getType().c_str();
				float fStart = 0.0f;
				if(pOperator->getType() == "birth")
				{
					fStart = Ogre::StringConverter::parseReal(pOperator->getParameter("birthtimes"));
				}
				else if(pOperator->getType() == "deleter")
				{
					fStart = Ogre::StringConverter::parseReal(pOperator->getParameter("maturity"));
				}
				float fEnd = 100;
				int iOperatorType = eGraphItem_bullet_bulletOperator;
				m_table->AppendRows(Lable,fStart,fEnd,iOperatorType,pOperator);
			}


		}//while(it != pBulletFlow->getBulletEventSystemsMap().end())
		it ++;
	}

	mBulletSystemMap.clear();
	std::map<Ogre::String,Fairy::BulletSystem*>::iterator bs_it = bulletSystemMap.begin();
	while(bs_it != bulletSystemMap.end())
	{
		mBulletSystemMap.insert(std::map<Ogre::String,Fairy::BulletSystem*>::value_type(bs_it->first,bs_it->second));
		Fairy::BulletSystem* pBulletSystem = bs_it->second;
		wxString Lable = pBulletSystem->getTemplateName();
		float fStart = 0.0f;
		float fEnd = 100;
		int iBulletSystemType = eGraphItem_bullet_bulletSystem;
		m_table->AppendRows(Lable,fStart,fEnd,iBulletSystemType,pBulletSystem);

		unsigned short i=0;
		for ( i=0; i<pBulletSystem->getNumAnimationEffectInfos(); ++i )
		{
			Fairy::AnimationEffectInfo* effectInfo = pBulletSystem->getAnimationEffectInfo(i);
			wxString Lable = "EffectInfo";
			float fStart = effectInfo->getAttachTime();
			float fEnd = fStart + 100;
			int iType = eGraphItem_bullet_animEffect;
			m_table->AppendRows(Lable,fStart,fEnd,iType,effectInfo);
		}
		for ( i=0; i<pBulletSystem->getNumAnimationCameraShake(); ++i )
		{
			Fairy::AnimationCameraShake* cameraShake = pBulletSystem->getAnimationCameraShake(i);
			wxString Lable = "CameraShake";
			float fStart = cameraShake->getAttachTime();
			float fEnd = fStart + 100;
			int iType = eGraphItem_bullet_cameraShake;
			m_table->AppendRows(Lable,fStart,fEnd,iType,cameraShake);
		}
		for ( i=0; i<pBulletSystem->getNumAnimationRibbons(); ++i )
		{
			Fairy::AnimationRibbon* ribbon = pBulletSystem->getAnimationRibbon(i);
			wxString Lable = "Ribbon";
			float fStart = ribbon->getAttachTime();
			float fEnd = fStart + 100;
			int iType = eGraphItem_bullet_Ribbon;
			m_table->AppendRows(Lable,fStart,fEnd,iType,ribbon);
		}
		for (i=0; i < pBulletSystem->getNumAnimationSounds(); ++i )
		{
			Fairy::AnimationSound *sound = pBulletSystem->getAnimationSound(i);
			wxString Lable = "Sound";
			float fStart = sound->getAttachTime();
			float fEnd = fStart + 100;
			int iType = eGraphItem_bullet_Sound;
			m_table->AppendRows(Lable,fStart,fEnd,iType,sound);
		}
		for ( i=0; i < pBulletSystem->getNumAnimationSceneLightInfos(); ++i )
		{
			Fairy::AnimationSceneLightInfo *light = pBulletSystem->getAnimationSceneLightInfo(i);
			wxString Lable = "Light";
			float fStart = light->getAttachTime();
			float fEnd = fStart + 100;
			int iType = eGraphItem_bullet_SceneLight;
			m_table->AppendRows(Lable,fStart,fEnd,iType,light);
		}
		for ( i=0; i < pBulletSystem->getNumAnimationLines(); ++i )
		{
			Fairy::AnimationLine *line = pBulletSystem->getAnimationLine(i);
			wxString Lable = "Line";
			float fStart = line->getAttachTime();
			float fEnd = fStart + 100;
			int iType = eGraphItem_bullet_line;
			m_table->AppendRows(Lable,fStart,fEnd,iType,line);
		}
		

		bs_it ++;
	}
	mEditType = eGraphEdit_bullet;
	m_CrrentTime = 0.0f;
	Refresh();
	
}
void wxGraphDialog::InitAnimTimePosSlider(void)
{
	Fairy::LogicModel* pMObject = m_Frame->GetActorSettingEditor()->GetCurrentObject();
	if(pMObject)
	{
		float len = pMObject->getCurrentAnimationLength();
		mTotalFrame = len * mFramePerSecond;
	}
}

void wxGraphDialog::update()
{
	if(mEditType == eGraphEidt_effect)
	{
		Fairy::Effect* pEffect = m_Frame->GetEffectObjectEditor()->getCurrentObjectEffect();
		if(pEffect)
		{
			if(pEffect->getLifeTime() < 0.001)
				return;
			if(!pEffect->isAlive())
				return;
			m_CrrentTime = pEffect->getCurrentLifeTime();
			m_topLabel->Refresh();
		}
		
	}
}
void wxGraphDialog::SetLoop(bool loop)
{
	if(mLoop)
	{
		mLoop = false;
		mLoopButton->SetBitmapLabel(wxBITMAP(LOOPDISABLED));
	}
	else
	{
		mLoop = true;
		mLoopButton->SetBitmapLabel(wxBITMAP(LOOP));

	}

}
//设置循环播放
void wxGraphDialog::OnLoopButtonDown(wxCommandEvent &e)
{
	if(mLoop)
	{
		mLoop = false;
		mLoopButton->SetBitmapLabel(wxBITMAP(LOOPDISABLED));
	}
	else
	{
		mLoop = true;
		mLoopButton->SetBitmapLabel(wxBITMAP(LOOP));
		
	}
	if(!m_Frame)
		return;

	ActorAnimationSettingEditor* pEditor = m_Frame->GetActorSettingEditor();
	if(pEditor)
	{
		Fairy::LogicModel* pActorObject = pEditor->GetCurrentObject();
		if(pActorObject)
		{
			pActorObject->setAnimationLoop(mLoop);
		}
	}
	
}
//更新动画速度
void wxGraphDialog::OnAnimRateUpdate(wxUpdateUIEvent &e)
{
	if(!mSliderAnimRate || !m_Frame)
		return;
	int iValue = mSliderAnimRate->GetValue();
	float fRate = (float)(iValue) / (float)(mSliderAnimRate->GetMax());
	char buf[16];
	sprintf(buf,"速率 %2.2f",fRate*2 );

	mSliderAnimRate->SetToolTip(buf);
	ActorAnimationSettingEditor* pEditor = m_Frame->GetActorSettingEditor();
	if(pEditor)
	{
		Fairy::LogicModel* pActorObject = pEditor->GetCurrentObject();
		if(pActorObject)
		{
			pActorObject->setGlobalAnimationRate( fRate*2);
		}
	}
	
}

void wxGraphDialog::OnTimer(wxTimerEvent& event)
{
	update();
	/*Fairy::LogicModel* pMObject = m_Frame->GetActorSettingEditor()->GetCurrentObject();
	if(pMObject)
	{
		mfAnimTimePos =  (int)(pMObject->GetCurrentTimePos() * mFramePerSecond) );	
	}*/
}


void wxGraphDialog::OnSliderTimePosUpdate( wxCommandEvent &event )
{
	Fairy::LogicModel* pMObject = m_Frame->GetActorSettingEditor()->GetCurrentObject();
	if(pMObject)
	{
		pMObject->setAnimEditing(true);
		pMObject->setAnimTimePos( mfAnimTimePos / mTotalFrame );
	}
	
}
void wxGraphDialog::OnPlayButtonDown(wxCommandEvent &e)
{
	// 恢复正常的动作播放，包括特效
	Fairy::LogicModel* pMObject = m_Frame->GetActorSettingEditor()->GetCurrentObject();
	if(pMObject)
	{
		pMObject->setAnimEditing(false);
	}
	if(mEditType == eGraphEidt_effect)
	{
		m_Frame->GetEffectObjectEditor()->ResetEffectObject();
		Fairy::Effect* pEffect = m_Frame->GetEffectObjectEditor()->getCurrentObjectEffect();
		if(pEffect)
		{
			for(int i = 0;i < m_table->GetNumberRows(); i++)
			{
				Fairy::EffectElement* pElement = pEffect->getElement(i);
				pElement->setVisible(m_table->GetRowEnable(i));
			}
			
		}
	}
	else if(mEditType == eGraphEidt_skill)
	{
		m_Frame->GetSkillObjectEidtor()->ResetSkillObject();
	}
	
}

void wxGraphDialog::OnStopButtonDown(wxCommandEvent &e)
{
	Fairy::LogicModel* pMObject = m_Frame->GetActorSettingEditor()->GetCurrentObject();
	if(pMObject)
	{
		pMObject->setAnimEditing(true);

		pMObject->setAnimTimePos( 0 );
		mfAnimTimePos = 0;
	}

	if(mEditType == eGraphEidt_effect)
	{
		Fairy::Effect* pEffect = m_Frame->GetEffectObjectEditor()->getCurrentObjectEffect();
		if(pEffect)
		{
		}
	}
	
}

void wxGraphDialog::OnStepBackButtonDown(wxCommandEvent &e)
{
	int newValue = mfAnimTimePos - 1;
	if ( newValue < 0 )
		newValue = mTotalFrame;

	Fairy::LogicModel* pMObject = m_Frame->GetActorSettingEditor()->GetCurrentObject();
	if(pMObject)
	{
		pMObject->setAnimTimePos( newValue / mTotalFrame );
	}
	
}

void wxGraphDialog::OnStepFrontButtonDown(wxCommandEvent &e)
{
	int newValue = mfAnimTimePos + 1;
	if ( newValue > mTotalFrame )
		newValue =0;

	Fairy::LogicModel* pMObject = m_Frame->GetActorSettingEditor()->GetCurrentObject();
	if(pMObject)
	{
		pMObject->setAnimTimePos( newValue / mTotalFrame );
	}
}

IMPLEMENT_ABSTRACT_CLASS( wxGraphTable, wxObject )


wxGraphTable::wxGraphTable()
{
	m_view = (wxGraphDialog *) NULL;
	m_numRows = 0;
}
wxGraphTable::~wxGraphTable()
{
	//delete m_attrProvider;
}

bool wxGraphTable::InsertRows( size_t pos,
							  const wxString& Lable,int start ,int end ,int type,void* data,bool enable)
{
	/*size_t curNumRows = m_data.size();

	if ( pos >= curNumRows )
	{
		return AppendRows( Lable,start,end,type,data,enable);
	}
	m_rowLabels.Insert(Lable,pos);
	m_starts.Insert(start,pos);
	m_ends.Insert(end,pos);
	m_GraphType.Insert(type,pos);
	m_enables.Insert(enable,pos);
	m_data.Insert(data,pos);

	m_numRows = m_data.GetCount();
	m_view->m_numRows = m_numRows;*/
	return true;
}

bool wxGraphTable::AppendRows(const wxString& Lable,float start ,float end ,int type,void* data,bool enable)
{

	tTableItem item;
	item.label = Lable;
	item.start = start;
	item.end = end;
	item.type = type;
	item.data = data;
	item.enable = enable;
	mTables.push_back(item);

	m_numRows = mTables.size();
	m_view->m_numRows = m_numRows;

	return true;
}

wxString wxGraphTable::GetRowLabelValue( int row )
{
	if ( row > mTables.size() - 1 )
	{
		return _T("null");
	}
	return mTables[row].label;
	
}

void wxGraphTable::Clear()
{
	mTables.clear();
	m_numRows = 0;
	m_view->m_numRows = 0;
}

int wxGraphTable::GetNumberRows()
{
	return mTables.size();
}

void wxGraphTable::SetRowEnable(size_t row,bool pEnable)
{
	mTables[row].enable =  pEnable;
}
bool wxGraphTable::GetRowEnable(size_t row)
{
	if ( row > mTables.size() - 1)
		return false;
	return mTables[row].enable;
}

int wxGraphTable::GetGraphType(size_t row)
{
	if ( row > mTables.size() - 1)
		return 0;
	return mTables[row].type;

}
void wxGraphTable::SetGraphType(size_t row,int type)
{
	mTables[row].type = type;

}
void* wxGraphTable::GetRowData(size_t row)
{
	if ( row > mTables.size() - 1)
		return NULL;
	return mTables[row].data;
}
void wxGraphTable::SetRowData(size_t row,void* data)
{

	mTables[row].data = data;
}
void wxGraphTable::SetStart(size_t row,float start)
{
	mTables[row].start = start;
}
void wxGraphTable::SetEnd(size_t row,float end)
{
	mTables[row].end = end;
}
float wxGraphTable::GetStart(size_t row)
{
	if ( row > mTables.size() - 1)
		return -1;

	return mTables[row].start;

}
float wxGraphTable::GetEnd(size_t row)
{
	if ( row > mTables.size() - 1)
		return -1;
	return mTables[row].end;
}
wxBitmap AnimationSettingBitmap( size_t index )
{
	if (index == 0)
	{
		/* XPM */
		static const char *xpm_data[] = {
			/* columns rows colors chars-per-pixel */
			"16 15 2 1",
			"a c Black",
			"b c #FF0000",
			/* pixels */
			"aaaaaaaaaaaaaaaa",
			"aabaaaaaaaaaaaaa",
			"aabbbaaaaaaaaaaa",
			"aabbbbaaaaaaaaaa",
			"aabbbbbbaaaaaaaa",
			"aabbbbbbbaaaaaaa",
			"aabbbbbbbbbaaaaa",
			"aabbbbbbbbbbaaaa",
			"aabbbbbbbbbaaaaa",
			"aabbbbbbbaaaaaaa",
			"aabbbbbbaaaaaaaa",
			"aabbbbbaaaaaaaaa",
			"aabbaaaaaaaaaaaa",
			"aabaaaaaaaaaaaaa",
			"aaaaaaaaaaaaaaaa"
		};
		wxBitmap bitmap( xpm_data );
		return bitmap;
	}
	if (index == 1)
	{
		/* XPM */
		static const char *xpm_data[] = {
			/* columns rows colors chars-per-pixel */
			"16 15 2 1",
			"a c Black",
			"b c #FF0000",
			/* pixels */
			"aaaaaaaaaaaaaaaa",
			"aaaaaaaaaaaaaaaa",
			"aaaaaaaaaaaaaaaa",
			"aaabbbbbbbbbbaaa",
			"aaabbbbbbbbbbaaa",
			"aaabbbbbbbbbbaaa",
			"aaabbbbbbbbbbaaa",
			"aaabbbbbbbbbbaaa",
			"aaabbbbbbbbbbaaa",
			"aaabbbbbbbbbbaaa",
			"aaabbbbbbbbbbaaa",
			"aaabbbbbbbbbbaaa",
			"aaaaaaaaaaaaaaaa",
			"aaaaaaaaaaaaaaaa",
			"aaaaaaaaaaaaaaaa"
		};
		wxBitmap bitmap( xpm_data );
		return bitmap;
	}
	if (index == 2)
	{
		/* XPM */
		static const char *xpm_data[] = {
			/* columns rows colors chars-per-pixel */
			"16 15 2 1",
			"a c Black",
			"b c #FF0000",
			/* pixels */
			"aaaaaaaaaaaaaaaa",
			"aaaaaaaaaaababba",
			"aaaaaaaaabbbabba",
			"aaaaaaabbbbbabba",
			"aaaaaabbbbbbabba",
			"aaaabbbbbbbbabba",
			"aabbbbbbbbbbabba",
			"abbbbbbbbbbbabba",
			"aabbbbbbbbbbabba",
			"aaaabbbbbbbbabba",
			"aaaaaabbbbbbabba",
			"aaaaaaabbbbbabba",
			"aaaaaaaaabbbabba",
			"aaaaaaaaaaababba",
			"aaaaaaaaaaaaaaaa"
		};
		wxBitmap bitmap( xpm_data );
		return bitmap;
	}
	if (index == 3)
	{
		/* XPM */
		static const char *xpm_data[] = {
			/* columns rows colors chars-per-pixel */
			"16 15 2 1",
			"a c Black",
			"b c #FF0000",
			/* pixels */
			"aaaaaaaaaaaaaaaa",
			"abbabaaaaaaaaaaa",
			"abbabbbaaaaaaaaa",
			"abbabbbbbaaaaaaa",
			"abbabbbbbbaaaaaa",
			"abbabbbbbbbbaaaa",
			"abbabbbbbbbbbbaa",
			"abbabbbbbbbbbbba",
			"abbabbbbbbbbbbaa",
			"abbabbbbbbbbaaaa",
			"abbabbbbbbaaaaaa",
			"abbabbbbbaaaaaaa",
			"abbabbbaaaaaaaaa",
			"abbabaaaaaaaaaaa",
			"aaaaaaaaaaaaaaaa"
		};
		wxBitmap bitmap( xpm_data );
		return bitmap;
	}
	return wxNullBitmap;
}

