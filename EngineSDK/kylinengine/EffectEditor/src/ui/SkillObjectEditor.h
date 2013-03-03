#ifndef __SkillObjectEditor__
#define __SkillObjectEditor__

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
#include <wx/utils.h>
#include "WXSceneListener.h"
#include "SceneManipulator.h"
#include "WXEffectEditorFrame.h"
#include "WXConfig.h"
#include "EffectSystem/FairySkill.h"
#include "ModelSystem/FairyLogicModel.h"

///////////////////////////////////////////////////////////////////////////

class SkillObjectTree;
class EffectObjectPropertyEditor;
class EffectEditorFrame;
const int ID_COMBO_ANIMATION = 10001;
const int ID_TEXTCTRL_SKILL = 10002;


static const wxWindowID feID_COMBOCTRL_EFFECT = 10104;
static const wxWindowID feID_COMBOCTRL_LOCATOR = 10110;

class AddEffectInfoDialog : public wxDialog
{
	DECLARE_DYNAMIC_CLASS(AddEffectInfoDialog)

public:
	AddEffectInfoDialog() {}

	// full ctor
	AddEffectInfoDialog(wxWindow *parent, wxWindowID id,
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

class AddRibbonDialog : public wxDialog
{
	DECLARE_DYNAMIC_CLASS(AddRibbonDialog)

public:
	AddRibbonDialog() {}

	// full ctor
	AddRibbonDialog(wxWindow *parent, wxWindowID id,
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

// 创建新技能时所弹出的对话框
class AddSkillDialog : public wxDialog
{
	DECLARE_DYNAMIC_CLASS(AddSkillDialog)

public:
	AddSkillDialog() {}

	// full ctor
	AddSkillDialog(wxWindow *parent, wxWindowID id,
		const wxString& title,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxDEFAULT_DIALOG_STYLE,
		const wxString& name = wxDialogNameStr)
		: wxDialog(parent, id, title, pos, size, style, name),
		mAnimComboBox(NULL), mSkillNameTextCtrl(NULL)
	{
	}

public:

	wxComboBox *mAnimComboBox;
	wxTextCtrl *mSkillNameTextCtrl;

protected:
	DECLARE_EVENT_TABLE()
	void OnInitDialog(wxInitDialogEvent &e);
};


static const int ID_SKILL_OBJECT_TREE = 10101;

class SaveAsSkillDialog : public wxDialog
{
	DECLARE_DYNAMIC_CLASS(SaveAsSkillDialog)

public:
	SaveAsSkillDialog() {}

	// full ctor
	SaveAsSkillDialog(wxWindow *parent, wxWindowID id,
		const wxString& title,
		Fairy::Skill *effect,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxSize( 289,139 ),
		long style = wxDEFAULT_DIALOG_STYLE)
		: mSkill(effect), mComboBox(NULL), mParent(parent),
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
	Fairy::Skill *mSkill;
	wxWindow *mParent;
};

class SaveSkillDialog : public wxDialog
{
	DECLARE_DYNAMIC_CLASS(SaveSkillDialog)

public:
	SaveSkillDialog() {}

	// full ctor
	SaveSkillDialog(wxWindow *parent, wxWindowID id,
		const wxString& title,
		Fairy::Skill *skill,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxSize( 289,108 ),
		long style = wxDEFAULT_DIALOG_STYLE)
		: mSkill(skill), mComboBox(NULL),
		wxDialog(parent, id, title, pos, size, style)
	{
	}

public:

	wxComboBox *mComboBox;

protected:
	DECLARE_EVENT_TABLE()
	void OnInitDialog(wxInitDialogEvent &e);

protected:
	Fairy::Skill *mSkill;
};

///////////////////////////////////////////////////////////////////////////////
/// Class SkillObjectEditor
///////////////////////////////////////////////////////////////////////////////
class SkillObjectEditor : public wxPanel,Fairy::SceneListener
{
protected:
	wxToolBar* m_toolBar;
	SkillObjectTree* m_SkillObjectTree;

	Ogre::String EFFECT_PATH;

public:
	SkillObjectEditor(){}
	
	SkillObjectEditor( wxWindow* parent,
		wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style =  wxTAB_TRAVERSAL );
	bool Create(wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style =  wxTAB_TRAVERSAL);
	~SkillObjectEditor();

	wxToolBar* GetToolBar(){return m_toolBar;}

	void InitSkillEditor(Fairy::Skill* pSkill,Ogre::String & templateName);

	//销毁效果物体
	void DestroySkillObject();
	//重新设置效果物体
	void ResetSkillObject();
	//刷新效果结构树
	void RefreshSkillTree();
	//选择当前编辑页
	void OnPageSelect();
	//刷新属性节点

	void EditRibbonMatrial(Fairy::AnimationRibbon* ribbon);

	void SelectElementItem(Ogre::StringInterface* element);

	void RefreshSkillTreeItem(const wxString& Desc,const wxString& propertyName,void* data = NULL);

	void SetSceneManipulator(Fairy::SceneManipulator* sceneManipulator);

	Fairy::Skill* getCurrentObjectSkill();

	Ogre::StringInterface* GetSelectedSkllElement();

	void SetRibbonMatrial(const Ogre::String name);

	Fairy::SceneManipulator* GetSceneManipulator(void) const
	{
		return mSceneManipulator;
	}

	void SetParentFrame(EffectEditorFrame* frame){m_Frame = frame;}
	EffectEditorFrame* GetParentFrame(){ return m_Frame;}

	Ogre::String getSaveFileName(void) const { return mSaveFileName; }

	Fairy::LogicModel* mDObject;

	Fairy::Skill *mSkill;
	Ogre::String mSkillTemplateName;

	enum ToolbarCtrl
	{
		TC_NEW_SKILL,
		TC_SAVEAS_SKILL,
		TC_SAVE_SKILL,
		TC_DELETE_ElEMENT,
		TC_NEW_EFFECT,
		TC_NEW_CAMERASHAKE,
		TC_NEW_RIBBON,
		TC_NEW_SOUND,
		TC_NEW_SCENELIGHT,
		TC_NEW_BULLETFLOW,
	};
	void OnNotifyToolBarCtrl(ToolbarCtrl index);

protected:

	void InitSkillObject();

    void saveSkill(Fairy::Skill *skill, const Ogre::String &templateName, std::ofstream &of);

	void OnRefreshSkillObject(wxCommandEvent &e);
	//新建技能数据
	void OnNewSkill(wxCommandEvent &e);
	//保存效果数据
	void OnSaveSkillObject(wxCommandEvent &e);
	//另存效果数据
	void OnSaveAsSkillObject(wxCommandEvent &e);
	//删除元素
	void OnDeleteSkillElement(wxCommandEvent &e);

	void OnNewAnimEffect(wxCommandEvent &e);
	void OnNewRibbon(wxCommandEvent &e);
	void OnNewSound(wxCommandEvent &e);
	void OnNewSceneLight(wxCommandEvent &e);
	void OnNewBulletCameraShake(wxCommandEvent &e);
	void OnNewMissile(wxCommandEvent &e);

	EffectEditorFrame *m_Frame;

	FairyConfig *mConfig;

	Fairy::SceneManipulator *mSceneManipulator;

	Ogre::String mSaveFileName;

	DECLARE_DYNAMIC_CLASS(SkillObjectEditor)

	DECLARE_EVENT_TABLE()



};

#endif //__EffectObjectEditor__
