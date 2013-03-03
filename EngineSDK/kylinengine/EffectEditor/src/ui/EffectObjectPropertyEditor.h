#ifndef __ParticleEditorDialog_H__
#define __ParticleEditorDialog_H__

#include <wx/dialog.h>
#include <wx/panel.h>
#include <list>
#include <OgreStringInterface.h>
#include "WXSceneListener.h"
#include <wx/checkbox.h>
#include "Core/FairyEffectObject.h"
#include "EffectSystem/FairySkill.h"
#include "Core/FairyParticleSystemObject.h"
#include "ModelSystem/FairyLogicModel.h"
#include "EffectSystem/FairyBulletFlowSystem.h"
#include "EffectSystem/FairyBulletEventSystem.h"
class wxPropertyGridManager;
class wxPropertyGridEvent;
class wxPGProperty;
class EffectObjectPropertyClass;
class wxPGId;
class wxPGConstants;
class wxComboBox;
class EffectEditorFrame;
class FairyConfig;

namespace Ogre	{
	class ParticleSystem;
}

namespace Fairy	{
	class SceneManipulator;
}

static const wxWindowID ID_TEXT_SAVE_AS_PARTICLE = 10106;
static const wxWindowID ID_COMBO_SAVE_AS_PARTICLE = 10107;
static const wxWindowID ID_TEXTCTRL_SAVE_AS_PARTICLE = 10108;


class EffectObjectPropertyEditor : public wxPanel,Fairy::SceneListener
{
	Ogre::String EFFECT_PATH;

	DECLARE_DYNAMIC_CLASS(EffectObjectPropertyEditor)

public:

	enum eEditType
	{
		eProperty_edit_null,
		eProperty_edit_particle,
		eProperty_edit_effect,
		eProperty_edit_skill,
		eProperty_edit_material,
		eProperty_edit_bullet
	};

	EffectObjectPropertyEditor() {}

	// full ctor
	EffectObjectPropertyEditor(wxWindow *parent, wxWindowID id,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxDEFAULT_FRAME_STYLE & (~wxCLOSE_BOX),
		const wxString& name = wxDialogNameStr);

	~EffectObjectPropertyEditor();

	void InitEffectEditor(Fairy::Effect *pEffect,const Ogre::String &templateName);
	void InitSkillEditor(Fairy::Skill* pSkill,Ogre::String& tmplateName);
	void InitMaterialEditor(Ogre::MaterialPtr& pMaterial,const Ogre::String &templateName);
	void InitBulletEditor(Fairy::BulletFlowSystem* pBulletFlow,const Ogre::String &templateName);

	//粒子属性组
	void InitParticleProperty(void);
	void InitParticleSettingProperty(Ogre::ParticleSystem* pParticleSystem);
	void InitParticleEmitterProperty(Ogre::ParticleEmitter *emitter);
	void InitParticleAffectorProperty(Ogre::ParticleAffector *affector);
	void InitParticleRenderProperty(Ogre::ParticleSystemRenderer *pRenderer);

	//效果属性
	void InitEffectProperty();
	void InitEffectSettingProperty();
	void InitEffectElementProperty(Fairy::EffectElement* pElement);

	//技能属性
	void InitSkillProperty();
	void InitSkillSettingProperty();
	void InitSkillElementProperty(Ogre::StringInterface* pElement);

	//材质属性
	void InitMaterialProperty();
	void InitMaterialTechnique(Ogre::Technique* technique);
	void InitMaterialPass(Ogre::Pass* pass);
	void InitMaterialTextureUnit(Ogre::TextureUnitState* textureUnit);

	void InitBulletFlowProperty();
	void InitBulletOperator(Fairy::BulletOperator* pOperator);
	void InitBulletSystemElement(Ogre::StringInterface* pElement);

	void clearAllProperty();

	

	void SetSceneManipulator(Fairy::SceneManipulator* sceneManipulator);

	Fairy::SceneManipulator* GetSceneManipulator(void) const
	{
		return mSceneManipulator;
	}
	void RefreshEffectObject();

	void SetParentFrame(EffectEditorFrame* frame){m_Frame = frame;}
	EffectEditorFrame* GetParentFrame(){ return m_Frame;}

	void UpdatePositionProperty(const Ogre::Vector3& pos);

protected:

	DECLARE_EVENT_TABLE()

	void OnPropertyChanged(wxPropertyGridEvent &e);
	void OnPropertyGridSelect ( wxPropertyGridEvent& event );
	void OnPropertyGridRightClick(wxPropertyGridEvent& event );
	void registerConstants(wxPGConstants* constants);
	wxPGConstants * PopulateConstants(const Ogre::String& name);

	
	void cloneParticleSystem(Ogre::ParticleSystem* newPSys, const Ogre::ParticleSystem* const clonedPSys, bool copyQuota = true);
	//void unloadPsystem(Ogre::ParticleSystem *system, const std::string& templateName);

	wxPGProperty *CreateBoolProperty(Ogre::String& name,bool value);
	wxPGProperty *CreateBoolOnOffProperty(Ogre::String& name,bool value);

	wxPGProperty *CreateProperty(const Ogre::ParameterDef& propertyDef, Ogre::String &value);

	wxPGProperty *CreateParticleSettingProperty(const Ogre::ParameterDef& propertyDef, Ogre::String &value);
	wxPGProperty *CreateParticleEmitterProperty(const Ogre::ParameterDef& propertyDef, Ogre::String &value);
	wxPGProperty *CreateParticleAffectorProperty(const Ogre::ParameterDef& propertyDef, Ogre::String &value);
	wxPGProperty *CreateParticleRenderProperty(Ogre::ParticleSystemRenderer *renderer,const Ogre::ParameterDef& propertyDef, Ogre::String &value);

	wxPGProperty *CreateEffectSettingProperty(const Ogre::ParameterDef& propertyDef, Ogre::String &value);
	wxPGProperty *CreateEffectElementProperty(const Ogre::ParameterDef& propertyDef, Ogre::String &value);

	wxPGProperty *CreateSkillSettingProperty(const Ogre::ParameterDef& propertyDef, Ogre::String &value);
	wxPGProperty *CreateSkillElementProperty(const Ogre::ParameterDef& propertyDef, Ogre::String &value);

	wxPGProperty *CreateBulletFlowSettingProperty(const Ogre::ParameterDef& propertyDef, Ogre::String &value);
	wxPGProperty *CreateBulletOperatorProperty(Fairy::BulletOperator* pOperator,const Ogre::ParameterDef& propertyDef, Ogre::String &value);
	wxPGProperty *CreateBulletSystemElementProperty(const Ogre::ParameterDef& propertyDef, Ogre::String &value);


protected:

	EffectEditorFrame* m_Frame;

	wxPropertyGridManager* mPropertyGridManager;

	std::list<wxPGConstants*> mConstantList;

	Ogre::ParticleSystem *mParticleSystem;
	Ogre::String mParticleSystemTemplateName;
	
	Fairy::Effect* mEffect;
	Ogre::String mEffectTemplateName;

	Fairy::Skill* mSkill;
	Ogre::String mSkillTemplateName;

	Fairy::BulletFlowSystem* mBulletFlow;
	Ogre::String mBulletFlowTemplateName;

	Ogre::MaterialPtr mMaterial;
	Ogre::String mMaterialTemplateName;

	FairyConfig *mConfig;

	eEditType m_EditType;

	Fairy::SceneManipulator *mSceneManipulator;

	wxArrayString mSelectedFileNames;

	wxComboBox *mEmitterComboBox;
	wxComboBox *mAffectorComboBox;

	wxCheckBox *mInsertCheckBox;

};



#endif // __ParticleEditorDialog_H__
