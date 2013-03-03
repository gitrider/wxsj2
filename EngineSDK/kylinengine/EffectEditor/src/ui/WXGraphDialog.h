
#ifndef __GRAPHDIALOG__
#define __GRAPHDIALOG__


#include "wx/wxprec.h"
#include <wx/checkbox.h>
#include <wx/gdicmn.h>
#include <wx/toolbar.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/statline.h>
#include <wx/scrolwin.h>
#include <wx/sizer.h>
#include <wx/hashmap.h>
#include <wx/panel.h>
#include <wx/arrstr.h>
#include <wx/scrolbar.h>
#include <wx/event.h>
#include <wx/combobox.h>
#include <wx/dynarray.h>
#include <wx/timer.h>
#include <wx/clntdata.h>
#include <wx/msw/dcclient.h>
#include "OgreParticleEmitter.h"
#include "OgreParticleAffector.h"
#include "OgreParticleSystem.h"
#include <vector>

#include "EffectSystem/FairyEffect.h"
#include "EffectSystem/FairyEffectElement.h"
#include "EffectSystem/FairySkill.h"
#include "EffectSystem/FairyBulletFlowSystem.h"
#include "EffectSystem/FairyBulletEventSystem.h"

class wxGraphDialog;
class wxGraphCanvas;
class wxGraphTable;
class wxGraphTopLabels;
class wxGraphLeftLabels;
class EffectEditorFrame;

class  wxGraphTable : public wxObject
{
public:
	
	wxGraphTable();
	~wxGraphTable();
	typedef struct
	{
		wxString label;
		float start;
		float end;
		bool enable;
		int type;
		void* data;
	} tTableItem;

	virtual void Clear();

	virtual bool InsertRows( size_t pos = 0,const wxString& Lable = wxT(""),
		int start = 0,int end = 0,int type = 0,void* data = NULL,bool enable = true);

	virtual bool AppendRows( const wxString& Lable = wxT(""),float start = 0,
		float end = 0,int type = 0,void* data = NULL,bool enable = true);


	virtual int GetNumberRows();
	virtual wxString GetRowLabelValue( int row );

	virtual void SetRowEnable(size_t row,bool enable);
	virtual bool GetRowEnable(size_t row);

	virtual void* GetRowData(size_t row);
	virtual void SetRowData(size_t row,void* data);

	virtual void SetStart(size_t row,float start);
	virtual void SetEnd(size_t row,float end);

	virtual float GetStart(size_t row);
	virtual float GetEnd(size_t row);

	virtual int GetGraphType(size_t row);
	virtual void SetGraphType(size_t row,int type);

	virtual void SetView( wxGraphDialog *grid ) { m_view = grid; }
	virtual wxGraphDialog * GetView() const { return m_view; }

	wxGraphDialog * m_view;
	std::vector<tTableItem> mTables;
	int m_numRows;

	DECLARE_DYNAMIC_CLASS_NO_COPY(wxGraphTable)
};
#include "wx/arrimpl.cpp"

const int TIMER = 10100;

class wxGraphDialog: public wxScrolledWindow
{
	friend class wxGraphLeftLabels;
public:

	enum GraphItemType
	{
		eGraphItem_particle_emitter = 0,
		eGraphItem_particle_affector,

		eGraphItem_effect = 4,
		eGraphItem_effect_particle,
		eGraphItem_effect_mesh,
		eGraphItem_effect_billboardSet,
		eGraphItem_effect_projector,
		eGraphItem_effect_beam,
		eGraphItem_effect_pointLight,
		eGraphItem_effect_cylinder,

		eGraphItem_skill = 16,
		eGraphItem_skill_animEffect,
		eGraphItem_skill_camaerShake,
		eGraphItem_skill_bibbon,
		eGraphItem_skill_Sound,
		eGraphItem_skill_SceneLight,
		eGraphItem_skill_BulletFlow,

		eGraphItem_bullet = 24,
		eGraphItem_bullet_bulletEvent,
		eGraphItem_bullet_bulletTester,
		eGraphItem_bullet_bulletOperator,
		eGraphItem_bullet_bulletSystem,
		eGraphItem_bullet_animEffect,
		eGraphItem_bullet_Ribbon,
		eGraphItem_bullet_cameraShake,
		eGraphItem_bullet_Sound,
		eGraphItem_bullet_SceneLight,
		eGraphItem_bullet_line

	};
	enum GraphEditType
	{
		eGraphEidt_null,
		eGraphEidt_particle,
		eGraphEidt_effect,
		eGraphEidt_skill,
		eGraphEidt_material,
		eGraphEdit_bullet
	};

	wxGraphDialog(){void Init();}
	wxGraphDialog(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,const long style = wxSUNKEN_BORDER);
	~wxGraphDialog(){};

	
	void Init();
	void OnPaint( wxPaintEvent &event );
	void OnSize( wxSizeEvent &event );
	void GetLabelTextAlignment( int *horiz, int *vert);
	void DrawTextRectangle( wxDC& dc, const wxString&, const wxRect&,
		int horizontalAlignment = wxALIGN_LEFT,
		int verticalAlignment = wxALIGN_TOP,
		int textOrientation = wxHORIZONTAL );

	void DrawTextRectangle( wxDC& dc, const wxArrayString& lines, const wxRect&,
		int horizontalAlignment = wxALIGN_LEFT,
		int verticalAlignment = wxALIGN_TOP,
		int textOrientation = wxHORIZONTAL );

	//绘制坐标格
	void DrawAllGridLines( wxDC& dc);
	//绘制item背景
	void DrawGridCellArea( wxDC& dc);
	//绘制时间线
	void DrawTimeLine(wxDC& dc);
	//绘制item
	void DrawCell( wxDC& dc, int row );

	void InitRowHeights();
	void InitEffectEditor(Fairy::Effect *pEffect, const Ogre::String &name);
	void InitSkillEditor(Fairy::Skill* pSkill,const Ogre::String &name);
	void InitBulletEditor(Fairy::BulletFlowSystem* pBulletFlow,std::map<Ogre::String,Fairy::BulletSystem*>& bulletSystemMap,const Ogre::String &templateName);

	void SetParentFrame(EffectEditorFrame* frame){m_Frame = frame;}
	EffectEditorFrame* GetParentFrame(){ return m_Frame;}
	float GetFramePerSecond(){return mFramePerSecond;}

private:
	
	void StringToLines( const wxString& value, wxArrayString& lines );
	void GetTextBoxSize( wxDC& dc,
		const wxArrayString& lines,
		long *width, long *height );

	EffectEditorFrame* m_Frame;

	wxGraphCanvas    *m_canvas;
	wxGraphTable *m_table;
	wxGraphTopLabels* m_topLabel;
	wxGraphLeftLabels* mLeftLabel;

	int m_LabelTextHorizAlign;
	int m_LabelTextVertAlign;
	int m_iRulerSize;
	float m_fRulerRate;
	int m_GridLineSpace;
	int m_currentCell;
	float m_CrrentTime;
	float m_TimeField;
public:

	int iLeftLabelWidth;
	int iLeftLabelHeight;
	int iTopLabelHeight;
	

	int m_numRows;

	float GetRulerRate(){return m_fRulerRate;}
	void SetRulerRate(float rate){m_fRulerRate = rate; }
	float GetGridSpace(){return m_GridLineSpace;}
	void SetGridSpace(float space){m_GridLineSpace = space; }
	float GetTimeField(){return m_TimeField;}
	void SetTimeField(float field){m_TimeField = field;}

	float GetCrrentTime(){return m_CrrentTime;}
	void SetCrrentTime(float time);

	int GetRows(){return m_numRows;}

	wxGraphTable * GetTable() const { return m_table; }
	bool SetTable( wxGraphTable *table){m_table = table;}

	void DrawRowLabels( wxDC& dc ,const int rows);
	void DrawRowLabel(wxDC& dc, const int row );

	wxTimer   mTimer;


public:
	void SetLoop(bool loop);
	bool GetLoop(){return false;}

	void OnLoopButtonDown(wxCommandEvent &e);
	void OnAnimRateUpdate(wxUpdateUIEvent &e);
	void OnPlayButtonDown(wxCommandEvent &e);
	void OnStopButtonDown(wxCommandEvent &e);
	void OnStepBackButtonDown(wxCommandEvent &e);
	void OnStepFrontButtonDown(wxCommandEvent &e);

	void OnSliderTimePosUpdate( wxCommandEvent &event );

	void OnTimer(wxTimerEvent& event);

	void InitAnimTimePosSlider(void);

	void update();


protected:
	Ogre::ParticleSystem *mParticleSystem;
	Ogre::String mParticleSystemTemplateName;

	Fairy::Effect *mEffect;
	Ogre::String mEffectTemplateName;

	Fairy::Skill *mSkill;
	Ogre::String mSkillTemplateName;
	wxSlider  *mSliderAnimRate;

	Fairy::BulletFlowSystem* mBulletFlow;
	Ogre::String mBulletFlowTemplateName;
	std::map<Ogre::String,Fairy::BulletSystem*> mBulletSystemMap;

	wxBitmapButton* mLoopButton;
	bool mLoop;

	float mFramePerSecond;
	float mTotalFrame;

	float  mfAnimTimePos;

	GraphEditType mEditType;

	DECLARE_DYNAMIC_CLASS(wxGraphDialog)
	DECLARE_EVENT_TABLE()
};

// wxGraphTopLabels

class wxGraphTopLabels: public wxWindow
{
public:
	wxGraphTopLabels() {}
	wxGraphTopLabels( wxGraphDialog *parent,wxWindowID id, const wxPoint &pos, const wxSize &size );

	void OnPaint( wxPaintEvent &event );

private:
	float mTime;
	wxGraphDialog   *m_owner;
	DECLARE_DYNAMIC_CLASS(wxGraphTopLabels)
	DECLARE_EVENT_TABLE()
};

// wxGraphLeftLabels

class wxGraphLeftLabels: public wxWindow
{
public:
	wxGraphLeftLabels() {}
	wxGraphLeftLabels( wxGraphDialog *parent,wxGraphTable *table,wxWindowID id, const wxPoint &pos, const wxSize &size );

	void OnPaint( wxPaintEvent &event );
	void OnLeftUp(wxMouseEvent& e);
	void OnDClick(wxMouseEvent& e);
	int calculateSelectItem(wxCoord y);

private:
	wxGraphDialog   *m_owner;
	wxGraphTable *m_table;
	DECLARE_DYNAMIC_CLASS(wxGraphLeftLabels)
	DECLARE_EVENT_TABLE()
};

// wxGraphCanvas

class wxGraphCanvas: public wxPanel
{
public:
	wxGraphCanvas(){};
	wxGraphCanvas( wxGraphDialog *parent, wxGraphTopLabels *top, wxGraphLeftLabels *right,
		wxWindowID id, const wxPoint &pos, const wxSize &size,const long style = wxNO_BORDER);
	~wxGraphCanvas(){};
	void OnPaint( wxPaintEvent &event );
	void ScrollWindow( int dx, int dy, const wxRect *rect );

private:
	wxGraphDialog   *m_owner;
	wxGraphTopLabels       *m_topLabels;
	wxGraphLeftLabels      *m_rightLabels;

	DECLARE_DYNAMIC_CLASS(wxGraphCanvas)
	DECLARE_EVENT_TABLE()
};

wxBitmap AnimationSettingBitmap( size_t index );

#endif