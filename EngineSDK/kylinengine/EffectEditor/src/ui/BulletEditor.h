#ifndef __BulletEditor__
#define __BulletEditor__

#include <wx/gdicmn.h>
#include <wx/toolbar.h>
#include <wx/combobox.h>
#include <wx/checkbox.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/treectrl.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/stattext.h>
#include "WXSceneListener.h"
#include "SceneManipulator.h"
#include "WXEffectEditorFrame.h"
#include "WXConfig.h"
#include "Core/FairyEffectObject.h"
#include "EffectSystem/FairyEffectElement.h"
#include "EffectSystem/FairyMeshElement.h"
#include "EffectSystem/FairyBeamElement.h"
#include "EffectSystem/FairyBillboardSetElement.h"
#include "EffectSystem/FairyProjectorElement.h"
#include "EffectSystem/FairyBulletFlowSystem.h"
#include "EffectSystem/FairyBulletCallbacks.h"
#include "EditMaterialDialog.h"
#include "EffectSystem/FairyBulletSystem.h"
#include "ModelSystem/FairyLogicModel.h"
#include <map>


///////////////////////////////////////////////////////////////////////////

class BulletTree;
class EffectObjectPropertyEditor;
static const int ID_BULLET_OBJECT_TREE = 10205;
struct BulletFlowInfo
{
	Fairy::LogicModel* targetModel;
	Ogre::String targetPoint;
	Ogre::Vector3 offsetPoistion;
	Ogre::Quaternion offsetRotation;

	Fairy::LogicModel* casterModel;
	Ogre::String attachPoint;

};
class SaveAsBulletDialog : public wxDialog
{
	DECLARE_DYNAMIC_CLASS(SaveAsBulletDialog)

public:
	SaveAsBulletDialog() {}

	// full ctor
	SaveAsBulletDialog(wxWindow *parent, wxWindowID id,
		const wxString& title,
		Fairy::BulletFlowSystem *bulletFlow,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxSize( 289,139 ),
		long style = wxDEFAULT_DIALOG_STYLE)
		: mBulletFlow(bulletFlow), mComboBox(NULL), mTextCtrl(NULL),
		wxDialog(parent, id, title, pos, size, style)
	{
	}

public:

	wxComboBox *mComboBox;
	wxTextCtrl *mTextCtrl;

protected:
	DECLARE_EVENT_TABLE()
	void OnInitDialog(wxInitDialogEvent &e);

protected:
	Fairy::BulletFlowSystem *mBulletFlow;
};

class SaveBulletDialog : public wxDialog
{
	DECLARE_DYNAMIC_CLASS(SaveBulletDialog)

public:
	SaveBulletDialog() {}

	// full ctor
	SaveBulletDialog(wxWindow *parent, wxWindowID id,
		const wxString& title,
		Fairy::Effect *effect,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxSize( 289,108 ),
		long style = wxDEFAULT_DIALOG_STYLE)
		: mBulletFlow(effect), mComboBox(NULL),
		wxDialog(parent, id, title, pos, size, style)
	{
	}

public:

	wxComboBox *mComboBox;

protected:
	DECLARE_EVENT_TABLE()
	void OnInitDialog(wxInitDialogEvent &e);

protected:
	Fairy::Effect *mBulletFlow;
};

class NewBulletEventDialog : public wxDialog
{
	DECLARE_DYNAMIC_CLASS(SaveBulletDialog)

public:
	NewBulletEventDialog() {}

	// full ctor
	NewBulletEventDialog(wxWindow *parent, wxWindowID id,
		const wxString& title,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxSize( 289,108 ),
		long style = wxDEFAULT_DIALOG_STYLE) : mTextCtrl(NULL),
		wxDialog(parent, id, title, pos, size, style)
	{
	}

public:

	wxTextCtrl *mTextCtrl;

protected:
	DECLARE_EVENT_TABLE()
	void OnInitDialog(wxInitDialogEvent &e);
};

class NewBulletOperatorDialog : public wxDialog
{
	DECLARE_DYNAMIC_CLASS(SaveBulletDialog)

public:
	NewBulletOperatorDialog() {}

	// full ctor
	NewBulletOperatorDialog(wxWindow *parent, wxWindowID id,
		const wxString& title,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxSize( 289,108 ),
		long style = wxDEFAULT_DIALOG_STYLE) : mComboBox(NULL),
		wxDialog(parent, id, title, pos, size, style)
	{
	}

public:
	wxComboBox *mComboBox;
protected:
	DECLARE_EVENT_TABLE()
	void OnInitDialog(wxInitDialogEvent &e);
};

class NewBulletSystemDialog : public wxDialog
{
	DECLARE_DYNAMIC_CLASS(SaveBulletDialog)

public:
	NewBulletSystemDialog() {}

	// full ctor
	NewBulletSystemDialog(wxWindow *parent, wxWindowID id,
		const wxString& title,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxSize( 289,108 ),
		long style = wxDEFAULT_DIALOG_STYLE) : mTextCtrl(NULL),
		wxDialog(parent, id, title, pos, size, style)
	{
	}

public:
	wxTextCtrl *mTextCtrl;
protected:
	DECLARE_EVENT_TABLE()
	void OnInitDialog(wxInitDialogEvent &e);
};

static const wxWindowID feID_COMBOCTRL_BULLET_EFFECT = 10204;
static const wxWindowID feID_COMBOCTRL_BULLET_LOCATOR = 10210;

class AddBulletEffectInfoDialog : public wxDialog
{
	DECLARE_DYNAMIC_CLASS(AddBulletEffectInfoDialog)

public:
	AddBulletEffectInfoDialog() {}

	// full ctor
	AddBulletEffectInfoDialog(wxWindow *parent, wxWindowID id,
		const wxString& title,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxDEFAULT_DIALOG_STYLE,
		const wxString& name = wxDialogNameStr)
		: wxDialog(parent, id, title, pos, size, style, name)
	{
	}

protected:
	DECLARE_EVENT_TABLE()
	void OnInitDialog(wxInitDialogEvent &e);
};


class AddBulletRibbonDialog : public wxDialog
{
	DECLARE_DYNAMIC_CLASS(AddBulletRibbonDialog)

public:
	AddBulletRibbonDialog() {}

	// full ctor
	AddBulletRibbonDialog(wxWindow *parent, wxWindowID id,
		const wxString& title,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxDEFAULT_DIALOG_STYLE,
		const wxString& name = wxDialogNameStr)
		: wxDialog(parent, id, title, pos, size, style, name)
	{
	}

protected:
	DECLARE_EVENT_TABLE()
	void OnInitDialog(wxInitDialogEvent &e);
};



class NewBulletDialog : public wxDialog
{
	DECLARE_DYNAMIC_CLASS(NewBulletDialog)

public:
	NewBulletDialog() {}

	// full ctor
	NewBulletDialog(wxWindow *parent, wxWindowID id,
		const wxString& title,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxSize( 289,108 ),
		long style = wxDEFAULT_DIALOG_STYLE)
		: mTextCtrl(NULL),
		wxDialog(parent, id, title, pos, size, style)
	{
	}

public:

	wxTextCtrl *mTextCtrl;

protected:
	DECLARE_EVENT_TABLE()
	void OnInitDialog(wxInitDialogEvent &e);

};

class RecoverSaveBulletDialog : public wxDialog
{
	DECLARE_DYNAMIC_CLASS(RecoverSaveBulletDialog)

public:
	RecoverSaveBulletDialog() {}

	// full ctor
	RecoverSaveBulletDialog(wxWindow *parent, wxWindowID id,
		const wxString& title,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxDEFAULT_DIALOG_STYLE,
		const wxString& name = wxDialogNameStr)
		: wxDialog(parent, id, title, pos, size, style, name)
	{
	}
	void SetText(wxString& text){mText = text;}
protected:
	DECLARE_EVENT_TABLE()
	void OnInitDialog(wxInitDialogEvent &e);
	wxString mText;

};



/*---------------------------------------------------*/
/*
	子弹的引擎回调事件
*/

// 子弹系统创建回调
struct BulletFlowCreateCallback : public Fairy::CreateBulletFlowCallback
{
	virtual void onCreateBulletFlow(const Fairy::String& bulletFlowTemplateName, 
		const Fairy::TransformInfo& transInfo, void* pSkillUserData);
};


// 子弹创建回调
struct   CreatedBulletCallback : public Fairy::AfterCreatedBulletSystemCallback
{
	virtual void onAfterCreatedBulletSystem(Fairy::BulletFlowSystem* bulletFlow, Fairy::BulletSystem* bulletSystem);
};

// 子弹销毁回调
struct DeleteBulletCallback : public Fairy::BeforeDeleteBulletSystemCallback
{
	virtual void onBeforeDeleteBulletSystem(Fairy::BulletSystem* bulletSystem);
};
struct BulletHitTargetCallback : public Fairy::BulletSystemHitTargetCallback
{
	// 当一个Bullet打中目标就调用此回调一次,用户可以通过m_userData来获取用户信息 [4/17/2008 JiangWei]
	virtual void onHitTargetCallback(Fairy::BulletFlowSystem* bulletFlow,Fairy::BulletSystem* bulletSystem);
};


// 技能销毁回调
struct SkillDestroyCallback : public Fairy::DestroySkillCallback
{
	virtual void onDestroySkill(Fairy::Skill* pSkill);
};

///////////////////////////////////////////////////////////////////////////////
/// Class BulletEditor
///////////////////////////////////////////////////////////////////////////////
class BulletEditor : public wxPanel,Fairy::SceneListener
{
protected:
	wxToolBar* m_toolBar;
	BulletTree* m_BulletObjectTree;

	Ogre::String EFFECT_PATH;

public:
	BulletEditor(){}
	
	BulletEditor( wxWindow* parent,
		wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style =  wxTAB_TRAVERSAL );
	bool Create(wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style =  wxTAB_TRAVERSAL);
	~BulletEditor();

	wxToolBar* GetToolBar(){return m_toolBar;}

	void InitBulletEditor(Fairy::BulletFlowSystem* pBulletFlow,const Ogre::String &name,bool isNew = false);

	//销毁效果物体
	void DestroyEffectObject();
	//重新设置效果物体
	void ResetBulletObject();
	//刷新效果结构树
	void RefreshBulletTree();
	//当前编辑页被选择
	void OnPageSelect();
	//刷新属性节点
	void RefreshBulletTreeItem(const wxString& Desc,const wxString& propertyName,void* data = NULL);

	void SelectElementItem(Fairy::EffectElement* element);

	void SelectBulletEvent(Fairy::BulletEventSystem* pEvent);
	void SelectBulletOperator(Fairy::BulletOperator* pOperator);
	void SelectBulletSystem(Fairy::BulletSystem* pSystem);
	void SelectBulletSystemElement(Ogre::StringInterface* pElement);

	void EditMeshMatrial(Fairy::MeshElement* meshElement);
	void EditBillboardMatrial(Fairy::BillboardSetElement* billboardElement);
	void EditProjectorMatrial(Fairy::ProjectorElement* projectorElement);
	void EditBeamMatrial(Fairy::BeamElement* beamElement);

	void SetMeshMatrial(const Ogre::String name);
	void SetBillboardMatrial(const Ogre::String name);
	void SetProjectorMatrial(const Ogre::String name);
	void SetBeamMatrial(const Ogre::String name);

	void SetSceneManipulator(Fairy::SceneManipulator* sceneManipulator);

	void SetBulletCallBack();

	Fairy::SceneManipulator* GetSceneManipulator(void) const
	{
		return mSceneManipulator;
	}

	Fairy::BulletFlowSystem* getCurrentObjectBulletFlow();

	void SetParentFrame(EffectEditorFrame* frame){m_Frame = frame;}
	EffectEditorFrame* GetParentFrame(){ return m_Frame;}

	Fairy::EffectElement* GetSelectedElement();

	std::map<Ogre::String,Fairy::BulletSystem*>& getBulletSystemMap() {return mBulletSystemMap;}

	Fairy::CreateBulletFlowCallback* getCreateBulletFlowCallback() const {return m_createBulletFlowCallback;}
	void setCreateBulletFlowCallback(Fairy::CreateBulletFlowCallback* val) {m_createBulletFlowCallback = val;}

	Fairy::AfterCreatedBulletSystemCallback* getAfterCreatedBulletSystemCallback() const { return m_afterCreatedBulletSystemCallback;}
	void setAfterCreatedBulletSystemCallback(Fairy::AfterCreatedBulletSystemCallback* val) { m_afterCreatedBulletSystemCallback = val;}

	Fairy::BeforeDeleteBulletSystemCallback* getBeforeDeleteBulletSystemCallback() const { return m_beforeDeleteBulletSystemCallback;}
	void setBeforeDeleteBulletSystemCallback(Fairy::BeforeDeleteBulletSystemCallback* val) {m_beforeDeleteBulletSystemCallback = val;}

	Fairy::BulletSystemHitTargetCallback* getBulletSystemHitTargetCallback() const { return m_bulletSystemHitTargetCallback;}
	void setBulletSystemHitTargetCallback(Fairy::BulletSystemHitTargetCallback* val) {m_bulletSystemHitTargetCallback = val;}

	bool tick();

	static BulletFlowCreateCallback		s_BulletFlowCreateCallback;
	static CreatedBulletCallback		s_CreatedBulletCallback;
	static DeleteBulletCallback			s_DeleteBulletCallback;
	static SkillDestroyCallback			s_SkillDestroyCallback;
	static BulletHitTargetCallback		s_BulletHitTargetCallback;

	std::vector<Fairy::BulletFlowSystem*> m_ActiveBulleFlows;

	enum ToolbarCtrl
	{
		TC_NEW_BULLETFLOW,
		TC_SAVEAS_BULLET,
		TC_SAVE_BULLET,
		TC_DELETE_ElEMENT,
		TC_NEW_EVENT,
		TC_NEW_OPERATOR,
		TC_NEW_BULLETSYSTEM,
		TC_NEW_BULLETSYSTEM_EFFECTINFO,
		TC_NEW_BULLETSYSTEM_CAMERASHAKE,
		TC_NEW_BULLETSYSTEM_RIBBON,
		TC_NEW_BULLETSYSTEM_SOUND,
		TC_NEW_BULLETSYSTEM_LIGHT,
		TC_NEW_BULLETSYSTEM_LINE,
	};

	void OnNotifyToolBarCtrl(ToolbarCtrl index);


protected:

	void InitEffectObject();

	void OnSaveAsBulletObject(wxCommandEvent &e);
	//保存效果数据
	void OnSaveBulletObject(wxCommandEvent &e);
	//创建效果数据
	void OnNewBulletFlow(wxCommandEvent &e);
	//删除元素
	void OnDeleteBulletElement(wxCommandEvent &e);

	void OnNewBulletEvent(wxCommandEvent &e);
	void OnNewBulletOperator(wxCommandEvent &e);
	void OnNewBulletSystem(wxCommandEvent &e);
	void OnNewBulletEffectInfo(wxCommandEvent &e);
	void OnNewBulletRibbon(wxCommandEvent &e);
	void OnNewBulletCameraShake(wxCommandEvent &e);
	void OnNewBulletSound(wxCommandEvent &e);
	void OnNewBulletLight(wxCommandEvent &e);
	void OnNewBulletLine(wxCommandEvent &e);


	void saveBulletFlow(Fairy::BulletFlowSystem *bulletFlow, const Ogre::String &templateName, std::ofstream &of);
	void saveBulletSystem(Fairy::BulletSystem *bulletSystem, const Ogre::String &templateName, std::ofstream &of);

	Fairy::BulletFlowSystem *mBulletFlow;
	Ogre::String mBulletFlowTemplateName;
	std::map<Ogre::String,Fairy::BulletSystem*> mBulletSystemMap;
	Ogre::String mSaveFileName;
	bool mbRecoverSave;

	Fairy::ObjectPtr mBulletFlowObject;
	Ogre::NameValuePairList mParameters;

	EffectEditorFrame *m_Frame;

	FairyConfig *mConfig;

	Fairy::SceneManipulator *mSceneManipulator;

	Fairy::CreateBulletFlowCallback*					m_createBulletFlowCallback;
	Fairy::AfterCreatedBulletSystemCallback*			m_afterCreatedBulletSystemCallback;
	Fairy::BeforeDeleteBulletSystemCallback*			m_beforeDeleteBulletSystemCallback;
	Fairy::BulletSystemHitTargetCallback*				m_bulletSystemHitTargetCallback;
	

	int mFrameTime;

	

	


	DECLARE_DYNAMIC_CLASS(BulletEditor)

	DECLARE_EVENT_TABLE()



};

#endif //__EffectObjectEditor__
