#ifndef __EffectObjectEditor__
#define __EffectObjectEditor__

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
#include "EffectSystem/FairyCylinderElement.h"
#include "EditMaterialDialog.h"
#include "OgreParticleSystem.h"
#include <map>
///////////////////////////////////////////////////////////////////////////

class EffectObjectTree;
class EffectObjectPropertyEditor;
static const int ID_EFFECT_OBJECT_TREE = 10101;
class SaveAsEffectDialog : public wxDialog
{
	DECLARE_DYNAMIC_CLASS(SaveAsEffectDialog)

public:
	SaveAsEffectDialog() {}

	// full ctor
	SaveAsEffectDialog(wxWindow *parent, wxWindowID id,
		const wxString& title,
		Fairy::Effect *effect,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxSize( 289,139 ),
		long style = wxDEFAULT_DIALOG_STYLE)
		: mEffect(effect), mComboBox(NULL), mTextCtrl(NULL), mParent(parent),
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
	Fairy::Effect *mEffect;
	wxWindow *mParent;
};

class SaveEffectDialog : public wxDialog
{
	DECLARE_DYNAMIC_CLASS(SaveEffectDialog)

public:
	SaveEffectDialog() {}

	// full ctor
	SaveEffectDialog(wxWindow *parent, wxWindowID id,
		const wxString& title,
		Fairy::Effect *effect,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxSize( 289,108 ),
		long style = wxDEFAULT_DIALOG_STYLE)
		: mEffect(effect), mComboBox(NULL),
		wxDialog(parent, id, title, pos, size, style)
	{
	}

public:

	wxComboBox *mComboBox;

protected:
	DECLARE_EVENT_TABLE()
	void OnInitDialog(wxInitDialogEvent &e);

protected:
	Fairy::Effect *mEffect;
};


class NewEffectDialog : public wxDialog
{
	DECLARE_DYNAMIC_CLASS(NewEffectDialog)

public:
	NewEffectDialog() {}

	// full ctor
	NewEffectDialog(wxWindow *parent, wxWindowID id,
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

class RecoverSaveDialog : public wxDialog
{
	DECLARE_DYNAMIC_CLASS(RecoverSaveDialog)

public:
	RecoverSaveDialog() {}

	// full ctor
	RecoverSaveDialog(wxWindow *parent, wxWindowID id,
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

class NewParticleElementDialog : public wxDialog
{
	DECLARE_DYNAMIC_CLASS(NewParticleElementDialog)

public:
	NewParticleElementDialog() {}

	// full ctor
	NewParticleElementDialog(wxWindow *parent, wxWindowID id,
		const wxString& title,
		Fairy::Effect *effect,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxSize( 289,108 ),
		long style = wxDEFAULT_DIALOG_STYLE)
		: mEffect(effect), mComboBox(NULL),
		wxDialog(parent, id, title, pos, size, style)
	{
	}

public:

	wxComboBox *mComboBox;

protected:
	DECLARE_EVENT_TABLE()
	void OnInitDialog(wxInitDialogEvent &e);

protected:
	Fairy::Effect *mEffect;
};


//////////////////////////////////////////////////////////////////////////
class NewParticleDialog : public wxDialog
{
	DECLARE_DYNAMIC_CLASS(NewParticleDialog)

public:
	NewParticleDialog() {}

	// full ctor
	NewParticleDialog(wxWindow *parent, wxWindowID id,
		const wxString& title,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxSize( 289,108 ),
		long style = wxDEFAULT_DIALOG_STYLE)
		: wxDialog(parent, id, title, pos, size, style),mTextCtrl(NULL)
	{
	}

public:
	wxTextCtrl *mTextCtrl;

protected:
	DECLARE_EVENT_TABLE()
	void OnInitDialog(wxInitDialogEvent &e);

};

///////////////////////////////////////////////////////////////////////////////
/// Class NewParticleEmitterDlg
///////////////////////////////////////////////////////////////////////////////
class NewParticleEmitterDlg : public wxDialog 
{
	DECLARE_DYNAMIC_CLASS(NewParticleEmitterDlg)

protected:
	wxStaticText* m_staticText;
	wxComboBox* m_EmitterComboBox;
	wxButton* m_CancelBtn;
	wxButton* m_OkBtn;

public:
	NewParticleEmitterDlg(){}
	NewParticleEmitterDlg( wxWindow* parent, wxWindowID id = wxID_ANY, 
		const wxString& title = wxT("添加粒子发射器"), 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxSize( 174,119 ), long style = wxDEFAULT_DIALOG_STYLE );
	~NewParticleEmitterDlg();
	wxString GetEmitterTypeName() const;

protected:
	DECLARE_EVENT_TABLE()
	void OnInitDialog(wxInitDialogEvent &e);
};

///////////////////////////////////////////////////////////////////////////////
/// Class NewParticleAffectorDlg
///////////////////////////////////////////////////////////////////////////////
class NewParticleAffectorDlg : public wxDialog 
{
	DECLARE_DYNAMIC_CLASS(NewParticleAffectorDlg)

protected:
	wxStaticText* m_staticText;
	wxComboBox* m_AffectorComboBox;
	wxButton* m_CancelBtn;
	wxButton* m_OkBtn;

public:
	NewParticleAffectorDlg(){}
	NewParticleAffectorDlg( wxWindow* parent, wxWindowID id = wxID_ANY, 
		const wxString& title = wxT("添加粒子影响器"), 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxSize( 174,119 ), long style = wxDEFAULT_DIALOG_STYLE );
	~NewParticleAffectorDlg();
	wxString GetAffectorTypeName() const;

protected:
	DECLARE_EVENT_TABLE()
	void OnInitDialog(wxInitDialogEvent &e);
};
///////////////////////////////////////////////////////////////////////////////
/// Class DeleteParticleObjectDlg
///////////////////////////////////////////////////////////////////////////////
class DeleteParticleObjectDlg : public wxDialog 
{
	DECLARE_DYNAMIC_CLASS(DeleteParticleObjectDlg)

protected:
	wxStaticText* m_staticText;
	wxButton* m_CancelBtn;
	wxButton* m_OkBtn;

public:
	DeleteParticleObjectDlg(){}
	DeleteParticleObjectDlg( wxWindow* parent, wxWindowID id = wxID_ANY, 
		const wxString& title = wxT("删除粒子元素"), 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxSize( 188,87 ), long style = wxDEFAULT_DIALOG_STYLE );
	~DeleteParticleObjectDlg();
	void SetObjectType(const wxString& type){m_ObjectType = type;}
	void SetObjectName(const wxString& name){m_ObjectName = name;}

protected:
	wxString m_ObjectType;
	wxString m_ObjectName;
	DECLARE_EVENT_TABLE()
	void OnInitDialog(wxInitDialogEvent &e);
};


///////////////////////////////////////////////////////////////////////////////
/// Class EffectObjectEditor
///////////////////////////////////////////////////////////////////////////////
class EffectObjectEditor : public wxPanel,Fairy::SceneListener
{
protected:
	wxToolBar* m_toolBar;
	EffectObjectTree* m_EffectObjectTree;

	Ogre::String EFFECT_PATH;

public:
	enum ToolbarCtrl
	{
		TC_NEW_EFFECT,
		TC_SAVEAS_EFFECT,
		TC_SAVE_EFFECT,
		TC_DELETE_ElEMENT,
		TC_NEW_PARTICLE,
		TC_NEW_MESH,
		TC_NEW_BILLBOARDSET,
		TC_NEW_PROJECTOR,
		TC_NEW_BEAM,
		TC_NEW_POINTLIGHT,
		TC_NEW_CYLINDER,
		TC_NEW_PARTICLE_SYSTEM,
		TC_NEW_EMITTER,
		TC_NEW_AFFECTOR,
	};
	EffectObjectEditor(){}
	
	EffectObjectEditor( wxWindow* parent,
		wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style =  wxTAB_TRAVERSAL );
	bool Create(wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style =  wxTAB_TRAVERSAL);
	~EffectObjectEditor();

	wxToolBar* GetToolBar(){return m_toolBar;}

	void ReInitEffectEditor();
	void InitEffectEditor(Fairy::Effect* pEffect,const Ogre::String &name,bool isNew = false);
	void InitParticleEditor(Ogre::ParticleSystem* pParticleSystem,const Ogre::String &name,bool isNew);

	//销毁效果物体
	void DestroyEffectObject();
	//重新设置效果物体
	void ResetEffectObject();
	//刷新效果结构树
	void RefreshEffectTree();
	//当前编辑页被选择
	void OnPageSelect();
	//刷新属性节点
	void RefreshEffectTreeItem(const wxString& Desc,const wxString& propertyName,void* data = NULL);

	void SelectElementItem(Fairy::EffectElement* element);

	void EditMeshMatrial(Fairy::MeshElement* meshElement);
	void EditBillboardMatrial(Fairy::BillboardSetElement* billboardElement);
	void EditProjectorMatrial(Fairy::ProjectorElement* projectorElement);
	void EditBeamMatrial(Fairy::BeamElement* beamElement);
	void EditCylinderMatrial(Fairy::CylinderElement* cylinderElement);

	void SetMeshMatrial(const Ogre::String name);
	void SetBillboardMatrial(const Ogre::String name);
	void SetProjectorMatrial(const Ogre::String name);
	void SetBeamMatrial(const Ogre::String name);
	void SetParticleMaterial(const Ogre::String name );

	void SetSceneManipulator(Fairy::SceneManipulator* sceneManipulator);

	Fairy::SceneManipulator* GetSceneManipulator(void) const
	{
		return mSceneManipulator;
	}

	Fairy::Effect* getCurrentObjectEffect();
	Fairy::EffectObject* getCurrentEffectObject();

	//编辑材质
	void EditMatrial();


	void SetParentFrame(EffectEditorFrame* frame){m_Frame = frame;}
	EffectEditorFrame* GetParentFrame(){ return m_Frame;}

	Ogre::StringInterface* GetSelectedObject();

	typedef std::map<Ogre::String,Ogre::ParticleSystem*> ParticleSystemMap;

	Ogre::String getSaveFileName(void) const { return mSaveFileName; }

	void OnNotifyToolBarCtrl(ToolbarCtrl index);

	void UpdateLocatorNode(bool bVisible = false);

protected:

	void InitEffectObject();

	void OnSaveAsEffectObject(wxCommandEvent &e);
	//保存效果数据
	void OnSaveEffectObject(wxCommandEvent &e);
	//创建效果数据
	void OnNewEffect(wxCommandEvent &e);
	//删除元素
	void OnDeleteEffectElement(wxCommandEvent &e);
	//添加粒子元素
	void OnNewParticle(wxCommandEvent &e);
	//添加模型元素
	void OnNewMesh(wxCommandEvent &e);
	//添加广告板元素
	void OnNewBillboardSet(wxCommandEvent &e);
	//添加地面投影元素
	void OnNewPorjector(wxCommandEvent &e);
	//添加光柱元素
	void OnNewBeam(wxCommandEvent &e);
	//添加点光照元素
	void OnNewPointLight(wxCommandEvent &e);
	void OnNewCylinder(wxCommandEvent &e);
	void OnNewParticleSystem(wxCommandEvent &e);
	void OnAddParitcleAffector(wxCommandEvent &e);
	void OnAddParitcleEmitter(wxCommandEvent &e);
	void OnShow(wxActivateEvent& e);

	void SelectParticleSystemItem(Ogre::ParticleSystem* pParticleSystem,bool select = false);
	void SelectBasicSettingItem(Ogre::ParticleSystem* pParticleSystem,bool select = false);
	void SelectRendererItem(Ogre::ParticleSystem* pParticleSystem,bool select = false);
	void SelectEmitterItem(Ogre::ParticleEmitter* pEmitter,bool select = false);
	void SelectAffectorItem(Ogre::ParticleAffector* pAffector,bool select = false);

	void saveEffect(Fairy::Effect *effect, const Ogre::String &templateName, std::ofstream &of, Ogre::String filename, bool saveParticle = true);
	//保存粒子系统
	void saveParticleSystem(Ogre::ParticleSystem *particleSystem, const Ogre::String &templateName, std::ofstream &of);

	Fairy::Effect *mEffect;
	Ogre::String mEffectTemplateName;
	Ogre::String mSaveFileName;
	bool mbRecoverSave;

	std::map<Ogre::String,Ogre::ParticleSystem*> mParticleSystemMap;

	Fairy::ObjectPtr mEffectObject;
	Fairy::PropertyNameStringPairList mParameters;

	EffectEditorFrame *m_Frame;

	FairyConfig *mConfig;

	Fairy::SceneManipulator *mSceneManipulator;


	DECLARE_DYNAMIC_CLASS(EffectObjectEditor)

	DECLARE_EVENT_TABLE()



};

#endif //__EffectObjectEditor__
