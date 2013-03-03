// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx/wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
#include <wx/scrolwin.h>
#include <wx/toolbar.h>
#include <wx/stattext.h>
#include <wx/button.h>
#endif

#include "EffectObjectPropertyEditor.h"
#include "WXGraphDialog.h"
#include "MaterialEditor.h"
// ----------------------------------------------------------------------------
// PropertyGrid headers
// ----------------------------------------------------------------------------

#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/propdev.h>
#include <wx/propgrid/advprops.h>
#include <wx/propgrid/manager.h>

#include "WXColourProperty.h"
#include "WXConfig.h"
#include "PropertyManager.h"

#include "SceneManipulator.h"
#include "Core/FairySceneInfo.h"
#include "Core/FairyParticleSystemObject.h"
#include "Core/FairyEffectObject.h"
#include "EffectSystem/FairyEffect.h"
#include "EffectSystem/FairyEffectElement.h"
#include "EffectSystem/FairyParticleElement.h"
#include "EffectSystem/FairySkill.h"
#include "EffectSystem/FairyAnimationEffectInfo.h"
#include "EffectSystem/FairyBulletSystemManager.h"
#include "EffectSystem/FairyBulletFlowSystemManager.h"
#include "EffectSystem/FairyAnimationCameraShake.h"
#include "EffectSystem/FairyAnimationRibbon.h"
#include "EffectSystem/FairyAnimationSceneLightInfo.h"
#include "EffectSystem/FairyAnimationSound.h"
#include "EffectSystem/FairyAnimationBulletFlow.h"
#include "WXEffectEditorFrame.h"
#include "WXConfig.h"
#include "EffectSettingGlobalData.h"
#include "ActorAnimationSettingEditor.h"
#include "MaterialEidtorCommon.h"
#include "BulletEditor.h"
//extern EffectSettingGlobalData gEffectSettingGlobalData;
// ----------------------------------------------------------------------------
// Resources
// ----------------------------------------------------------------------------

#include "res/EffectObjectProperty_wdr.h"

// ----------------------------------------------------------------------------
// Ogre headers
// ----------------------------------------------------------------------------
#include <OgreParticleSystem.h>
#include <OgreParticleEmitter.h>
#include <OgreParticleAffector.h>
#include <OgreParticleSystemManager.h>
#include <OgreStringConverter.h>
#include <OgreResourceManager.h>
#include <OgreMaterialManager.h>
#include <OgreParticleSystemRenderer.h>
#include <OgreParticleAffectorFactory.h>
#include <OgreParticleEmitterFactory.h>
#include "EffectObjectEditor.h"
#include "SkillObjectEditor.h"
// ----------------------------------------------------------------------------
// macros
// ----------------------------------------------------------------------------

#if wxUSE_STL
#define AS_STRING(s)    (s)
#else
#define AS_STRING(s)    ((s).c_str())
#endif


class EffectObjectPropertyClass : public wxCustomPropertyClass
{
public:
	EffectObjectPropertyClass(const wxString& label,
		const wxString& name = wxPG_LABEL,void* data = NULL):m_data(data),
		wxCustomPropertyClass(label,name)
	{

	}

	~EffectObjectPropertyClass(){}
	void SetData(void* data){m_data = data;}
	void* GetData(){return m_data;}
protected:
	void* m_data;


};
// ----------------------------------------------------------------------------
// EffectObjectPropertyEditor class
// ----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(EffectObjectPropertyEditor, wxPanel)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(EffectObjectPropertyEditor, wxPanel)
	EVT_PG_CHANGED(ID_PROPERTY_EDITOR, EffectObjectPropertyEditor::OnPropertyChanged)
	EVT_PG_SELECTED(ID_PROPERTY_EDITOR, EffectObjectPropertyEditor::OnPropertyGridSelect)
	EVT_PG_RIGHT_CLICK(ID_PROPERTY_EDITOR,EffectObjectPropertyEditor::OnPropertyGridRightClick)

END_EVENT_TABLE()

// full ctor
EffectObjectPropertyEditor::EffectObjectPropertyEditor(wxWindow *parent, wxWindowID id,
					 const wxPoint& pos,
					 const wxSize& size,
					 long style,
					 const wxString& name)
					 : wxPanel(parent, id,pos, size, style, name)
					 ,m_Frame(NULL)
					 ,mParticleSystem(NULL)
					 ,mParticleSystemTemplateName("")
					 ,mEffect(NULL)
					 ,mEffectTemplateName("")
					 ,mSkill(NULL)
					 ,mSkillTemplateName("")
					 ,mBulletFlow(NULL)
					 ,mBulletFlowTemplateName("")
					 ,mMaterialTemplateName("")
					 ,m_EditType(eProperty_edit_null)
					 ,mConfig(NULL)
					 ,EFFECT_PATH("../Media/EffectSystem/")

{
	mConstantList.clear();

	wxFlexGridSizer *sizer = new wxFlexGridSizer(1, 0, 0);
	sizer->AddGrowableCol(0);
	sizer->AddGrowableRow(0);

	wxPropertyGridManager *propertyManager = new wxPropertyGridManager(this, ID_PROPERTY_EDITOR,
		wxDefaultPosition, // position 
		wxDefaultSize, // size
		wxCLIP_CHILDREN | wxCLIP_SIBLINGS |
		wxPG_BOLD_MODIFIED | // Modified values are drawn in bold font
		wxPG_SPLITTER_AUTO_CENTER | // Automatically center splitter until user manually adjusts it
		wxPG_TOOLTIPS | // Display tooltips for cell text that cannot be shown completely
		wxPG_TOOLBAR | // Include toolbar
		wxPG_DESCRIPTION | // Include description box
		wxFULL_REPAINT_ON_RESIZE |
		wxPGMAN_DEFAULT_STYLE); // Plus default style
	propertyManager->SetExtraStyle(wxPG_EX_CLASSIC_SPACING);
	wxASSERT( propertyManager );
	sizer->Add( propertyManager, 0, wxALIGN_CENTER|wxALL|wxGROW, 5 );

	if (sizer)
	{
		this->SetSizer( sizer );
		sizer->SetSizeHints( this );
	}

	mPropertyGridManager = wxDynamicCast(this->FindWindow(ID_PROPERTY_EDITOR), wxPropertyGridManager);

	if (!mConfig)
	{
		mConfig = new FairyConfig("../EditorConfigFiles/FairyParticleEditor.cfg");
	}	

}

void EffectObjectPropertyEditor::RefreshEffectObject()
{
	Fairy::Scene::ObjectsByTypeRange particles =
		mSceneManipulator->getSceneInfo()->findObjectsByType(Fairy::ParticleSystemObject::msType);

	for (Fairy::Scene::ObjectsByTypeIterator it = particles.first; it != particles.second; ++it)
	{
		const Fairy::ObjectPtr& object = *it;
		Fairy::ParticleSystemObject* particleObject = static_cast<Fairy::ParticleSystemObject *>(object.get());

		Ogre::ParticleSystem *system = particleObject->getParticleSystem();
		Ogre::String templateName = particleObject->getPropertyAsString("particle system name");

		// 现在做的是直接对模板的修改，所以要修改全部的同个模板的特效
		if ( system && templateName == mParticleSystemTemplateName )
		{
			for ( unsigned short i = 0; i < system->getNumEmitters(); ++i )
			{
				system->getEmitter(i)->setEnabled(false);
			}

			system->clear();

			for ( unsigned short i = 0; i < system->getNumEmitters(); ++i )
			{
				system->getEmitter(i)->setEnabled(true);
			}
		}
	}
}

EffectObjectPropertyEditor::~EffectObjectPropertyEditor()
{
	for (std::list<wxPGConstants*>::const_iterator it = mConstantList.begin(); it != mConstantList.end(); ++it)
	{
		wxPGConstants* constants = *it;
		if (constants->UnRef())
			delete constants;
	}
	mConstantList.clear();

	if (mConfig)
	{
		delete mConfig;
		mConfig = NULL;
	}
}

void EffectObjectPropertyEditor::UpdatePositionProperty(const Ogre::Vector3& pos)
{
	if(m_EditType == eProperty_edit_effect)
	{
		wxPGId id = mPropertyGridManager->GetPropertyByLabel("Position");		
		if (id != NULL)
		{
			wxPGId parentId = mPropertyGridManager->GetPropertyParent(id);
			Ogre::String posStr = Ogre::StringConverter::toString(pos);
			mPropertyGridManager->SetPropertyValue(id,posStr);
			
			EffectObjectPropertyClass* pProperty = dynamic_cast<EffectObjectPropertyClass*>(&(parentId.GetProperty()));
			assert(pProperty);
			Fairy::EffectElement* pElement = (Fairy::EffectElement*)(pProperty->GetData());
			if(pElement)
			{
				pElement->setParameter("Position", posStr);
				m_Frame->GetEffectObjectEditor()->ResetEffectObject();
				m_Frame->GetEffectObjectEditor()->RefreshEffectTreeItem("Effect element item","Position",pElement);				
			}
			
			m_Frame->GetEffectObjectEditor()->UpdateLocatorNode(true);
			GetSceneManipulator()->getLocatorNode()->setPosition(pos);
		}
	}
}

void
EffectObjectPropertyEditor::OnPropertyChanged(wxPropertyGridEvent &e)
{
	// Skip parent properties
	if (e.GetPropertyPtr()->GetParentingType() != 0)
		return;

	wxPGId id = e.GetPropertyPtr()->GetId();
	wxPGId parentId = mPropertyGridManager->GetPropertyParent(id);

	Fairy::String propertyName = e.GetPropertyName().c_str();
	Fairy::String propertyValue = e.GetPropertyValueAsString().c_str();


	//效果属性修改
	if(m_EditType == eProperty_edit_effect)
	{
		bool isTemplateSetting = false;
		if(parentId.GetProperty().GetName() == "EffectSetting")
		{
			EffectObjectPropertyClass* pProperty = dynamic_cast<EffectObjectPropertyClass*>(&(parentId.GetProperty()));
			assert(pProperty);
			Fairy::Effect* pEffect = (Fairy::Effect*)(pProperty->GetData());
			if(pEffect)
			{
				pEffect->setParameter(propertyName, propertyValue);
				if (!isTemplateSetting)
				{
					pEffect->setParameter(propertyName, propertyValue);
					isTemplateSetting = true;
				}
				m_Frame->GetEffectObjectEditor()->ResetEffectObject();
				m_Frame->GetEffectObjectEditor()->RefreshEffectTreeItem("Effect property item",propertyName,pEffect);
			}
		}
		if(parentId.GetProperty().GetName() == "EffectElement")
		{
			if (propertyName == "Orientation")
			{
				Ogre::Quaternion OrientationValue = Ogre::StringConverter::parseQuaternion(propertyValue);
				Ogre::Radian rfAngle  = Ogre::Radian(Ogre::Degree(OrientationValue.z));
				Ogre::Vector3 rkAxis = Ogre::Vector3(OrientationValue.w,OrientationValue.x,OrientationValue.y);
				rkAxis.normalise();
				Ogre::Quaternion Orientation;
				Orientation.FromAngleAxis(rfAngle,rkAxis);
				Ogre::String valueString = Ogre::StringConverter::toString(rkAxis);
				valueString += " ";
				valueString += Ogre::StringConverter::toString(OrientationValue.z);
				id.GetProperty().SetValueFromString(valueString.c_str(),wxPG_FULL_VALUE);
				propertyValue = Ogre::StringConverter::toString(Orientation);
		
			}
			
			EffectObjectPropertyClass* pProperty = dynamic_cast<EffectObjectPropertyClass*>(&(parentId.GetProperty()));
			assert(pProperty);
			Fairy::EffectElement* pElement = (Fairy::EffectElement*)(pProperty->GetData());
			if(pElement)
			{
				pElement->setParameter(propertyName, propertyValue);
				if (!isTemplateSetting)
				{
					pElement->setParameter(propertyName, propertyValue);
					isTemplateSetting = true;
				}
				m_Frame->GetEffectObjectEditor()->ResetEffectObject();
				m_Frame->GetEffectObjectEditor()->RefreshEffectTreeItem("Effect element item",propertyName,pElement);				
			}
			
			if (propertyName == "Position")
			{
				m_Frame->GetEffectObjectEditor()->UpdateLocatorNode(true);
				Ogre::Vector3 newPos = Ogre::StringConverter::parseVector3(propertyValue);
				GetSceneManipulator()->getLocatorNode()->setPosition(pElement->getRelPos());
			}
		}
		else if(parentId.GetProperty().GetName() == "ParticleBasicSetting")
		{
			Ogre::SceneManager* creator = mSceneManipulator->getSceneManager();

			isTemplateSetting = false;

			EffectObjectPropertyClass* pProperty = dynamic_cast<EffectObjectPropertyClass*>(&(parentId.GetProperty()));
			assert(pProperty);
			Ogre::ParticleSystem* pParticleSystem = (Ogre::ParticleSystem*)(pProperty->GetData());

			/*if ( propertyName == "quota" )
			{
				Ogre::String systemName = pParticleSystem->getName();
				Ogre::SceneNode *node = pParticleSystem->getParentSceneNode();
				Ogre::ParticleSystem* tempPSys = Ogre::ParticleSystemManager::getSingleton().createTemplate("tempParticleSystem","General");
				//tempPSys->set

				// make a copy but use quota of tempPSys (request copyQuota = false)
				cloneParticleSystem(tempPSys, pParticleSystem, false);

				// Remove the current system.
				unloadPsystem(pParticleSystem, mParticleSystemTemplateName);

				// Create a new system based on the temp system, with the same name as the currently selected system.
				Ogre::ParticleSystem* newPSys =
					creator->createParticleSystem(systemName, Ogre::StringConverter::parseInt(propertyValue));

				// Copy the parameters from temp to new
				cloneParticleSystem(newPSys, tempPSys);

				// Destroy the temporary system
				tempPSys->destroy();

				// Attach the new version of the system to the main node.
				if (node)
					node->attachObject(newPSys);

				//pParticleObject->setParticleSystem(newPSys);

				newPSys->setRenderQueueGroup(Ogre::RENDER_QUEUE_8);

				if (!isTemplateSetting)
				{
					pParticleSystem->setParameter(propertyName, propertyValue);
					isTemplateSetting = true;
				}
				//ParticleObjectEditor* pParticleEditor = m_Frame->GetParticleObjectEditor();
				//pParticleEditor->RefreshParticleSetting();
				//pParticleEditor->ResetParticleObject();
				//pParticleEditor->RefreshParticleTreeItem("Particle BasicSetting item",propertyName,mParticleSystem);

			}
			else*/ if (propertyName == "renderer")
			{
				pParticleSystem->setParameter(propertyName, propertyValue);

				if (propertyValue == "mesh")
				{
					pParticleSystem->setDefaultDimensions(1.0f, 1.0f);
					// 删除所有的影响器
					pParticleSystem->removeAllAffectors();
				}

				if (!isTemplateSetting)
				{
					pParticleSystem->setParameter(propertyName, propertyValue);

					if (propertyValue == "mesh")
					{
						pParticleSystem->setDefaultDimensions(1.0f, 1.0f);
						// 删除所有的影响器
						pParticleSystem->removeAllAffectors();
					}

					isTemplateSetting = true;
				}
				EffectObjectEditor* pEffectEditor = m_Frame->GetEffectObjectEditor();
				pEffectEditor->InitEffectEditor(mEffect,mEffectTemplateName);
				m_Frame->GetEffectObjectEditor()->RefreshEffectTreeItem("Particle BasicSetting item renderer",propertyName,pParticleSystem);
				

			}
			else if(propertyName == "material")
			{
				
				pParticleSystem->setParameter(propertyName, propertyValue);
				m_Frame->GetEffectObjectEditor()->ResetEffectObject();
				m_Frame->GetEffectObjectEditor()->RefreshEffectTreeItem("Particle BasicSetting item material",propertyName,pParticleSystem);
			}
			else
			{

				pParticleSystem->setParameter(propertyName, propertyValue);
				m_Frame->GetEffectObjectEditor()->ResetEffectObject();
				m_Frame->GetEffectObjectEditor()->RefreshEffectTreeItem("Particle BasicSetting item",propertyName,pParticleSystem);
			}
			
		}
		else if(parentId.GetProperty().GetName() == "ParticleRenderer")
		{
			EffectObjectPropertyClass* pProperty = dynamic_cast<EffectObjectPropertyClass*>(&(parentId.GetProperty()));
			assert(pProperty);
			Ogre::ParticleSystemRenderer* pParticlRender = (Ogre::ParticleSystemRenderer*)(pProperty->GetData());
			pParticlRender->setParameter(propertyName, propertyValue);

			if (!isTemplateSetting)
			{
				pParticlRender->setParameter(propertyName, propertyValue);
				isTemplateSetting = true;
			}
			m_Frame->GetEffectObjectEditor()->ResetEffectObject();
			m_Frame->GetEffectObjectEditor()->RefreshEffectTreeItem("Particle render item",propertyName,pParticlRender);
		}

		else if(parentId.GetProperty().GetName() == "ParticleEmtter")
		{
			EffectObjectPropertyClass* pProperty = dynamic_cast<EffectObjectPropertyClass*>(&(parentId.GetProperty()));
			assert(pProperty);
			Ogre::ParticleEmitter* pEmitter = (Ogre::ParticleEmitter*)(pProperty->GetData());
			if(pEmitter)
			{
				pEmitter->setParameter(propertyName, propertyValue);
				if (!isTemplateSetting)
				{
					pEmitter->setParameter(propertyName, propertyValue);
					isTemplateSetting = true;
				}
				m_Frame->GetEffectObjectEditor()->ResetEffectObject();
				m_Frame->GetEffectObjectEditor()->RefreshEffectTreeItem("Particle emitter item",propertyName,pEmitter);
			}
		}
		else if(parentId.GetProperty().GetName() == "ParticleAffector")
		{
			// affector
			EffectObjectPropertyClass* pProperty = dynamic_cast<EffectObjectPropertyClass*>(&(parentId.GetProperty()));
			assert(pProperty);
			Ogre::ParticleAffector* pAffector = (Ogre::ParticleAffector*)(pProperty->GetData());

			if(pAffector)
			{
				pAffector->setParameter(propertyName, propertyValue);
				if (!isTemplateSetting)
				{
					pAffector->setParameter(propertyName, propertyValue);
					isTemplateSetting = true;
				}
				m_Frame->GetEffectObjectEditor()->ResetEffectObject();
				m_Frame->GetEffectObjectEditor()->RefreshEffectTreeItem("Particle affector item",propertyName,pAffector);
			}

		}

	}
	//技能属性修改
	else if(m_EditType == eProperty_edit_skill)
	{

		unsigned short numOfEffectInfos = mSkill->getNumAnimationEffectInfos();
		unsigned short numOfRibbons = mSkill->getNumAnimationRibbons();
		unsigned short numOfSounds = mSkill->getNumAnimationSounds();
		unsigned short unmOfLights = mSkill->getNumAnimationSceneLightInfos();

		if(parentId.GetProperty().GetName() == "SkillSetting")
		{
			if (propertyName == "AttachTime")
			{
				float inputValue = Ogre::StringConverter::parseReal(propertyValue);


			/*	if ( inputValue >= 1.0f )
				{
					wxMessageBox(_("Wrong parameter!"));
					return;
				}*/
				propertyValue = Ogre::StringConverter::toString(inputValue);
			}
			else if (propertyName == "HitTime" || propertyName == "BreakTime" || propertyName == "ShakeTime")
			{
				Ogre::StringVector valueArray = Ogre::StringUtil::split(propertyValue);

				propertyValue.clear();

				for (size_t i=0; i<valueArray.size(); ++i)
				{
					float inputValue = Ogre::StringConverter::parseReal(valueArray[i]);

					/*if ( inputValue >= 1.0f )
					{
						wxMessageBox(_("Wrong parameter!"));
						return;
					}*/

					propertyValue += Ogre::StringConverter::toString(inputValue);
					propertyValue += " ";
				}
			}

			mSkill->setParameter(propertyName, propertyValue);

			SkillObjectEditor* pSkillEditor = m_Frame->GetSkillObjectEidtor();
			pSkillEditor->ResetSkillObject();
			mSkill = Fairy::EffectManager::getSingleton().getSkill(mSkillTemplateName);
			pSkillEditor->RefreshSkillTreeItem("Skill property item",propertyName,mSkill);
		}
		else if(parentId.GetProperty().GetName() == "SkillElement")
		{
			if (propertyName == "OffsetRotation")
			{
				Ogre::Quaternion OrientationValue = Ogre::StringConverter::parseQuaternion(propertyValue);
				Ogre::Radian rfAngle  = Ogre::Radian(Ogre::Degree(OrientationValue.z));
				Ogre::Vector3 rkAxis = Ogre::Vector3(OrientationValue.w,OrientationValue.x,OrientationValue.y);
				rkAxis.normalise();
				Ogre::Quaternion Orientation;
				Orientation.FromAngleAxis(rfAngle,rkAxis);
				Ogre::String valueString = Ogre::StringConverter::toString(rkAxis);
				valueString += " ";
				valueString += Ogre::StringConverter::toString(OrientationValue.z);
				id.GetProperty().SetValueFromString(valueString.c_str(),wxPG_FULL_VALUE);
				propertyValue = Ogre::StringConverter::toString(Orientation);
		
			}
			EffectObjectPropertyClass* pProperty = dynamic_cast<EffectObjectPropertyClass*>(&(parentId.GetProperty()));
			assert(pProperty);
			Ogre::StringInterface* pSkillElement = (Ogre::StringInterface*)(pProperty->GetData());
			pSkillElement->setParameter(propertyName, propertyValue);
			
			m_Frame->GetSkillObjectEidtor()->ResetSkillObject();
			mSkill = Fairy::EffectManager::getSingleton().getSkill(mSkillTemplateName);
			m_Frame->GetSkillObjectEidtor()->RefreshSkillTreeItem("Skill Element item",propertyName,pSkillElement);

		}

	}
	else if(m_EditType == eProperty_edit_bullet)
	{
		if(parentId.GetProperty().GetName() == "BulletFlowSetting")
		{
			mBulletFlow->setParameter(propertyName, propertyValue);

			BulletEditor* pBulletEditor = m_Frame->GetBulletEditor();
			pBulletEditor->ResetBulletObject();

			pBulletEditor->RefreshBulletTreeItem("BulletFlow property item",propertyName,mBulletFlow);
		}
		else if(parentId.GetProperty().GetName() == "BulletOperator")
		{
			EffectObjectPropertyClass* pProperty = dynamic_cast<EffectObjectPropertyClass*>(&(parentId.GetProperty()));
			assert(pProperty);
			Fairy::BulletOperator* pOperator = (Fairy::BulletOperator*)(pProperty->GetData());
			pOperator->setParameter(propertyName, propertyValue);

			BulletEditor* pBulletEditor = m_Frame->GetBulletEditor();
			pBulletEditor->ResetBulletObject();
			pBulletEditor->RefreshBulletTreeItem("BulletOperator property item",propertyName,pOperator);
		}
		else if(parentId.GetProperty().GetName() == "BulletElement")
		{
			if (propertyName == "OffsetRotation")
			{
				Ogre::Quaternion OrientationValue = Ogre::StringConverter::parseQuaternion(propertyValue);
				Ogre::Radian rfAngle  = Ogre::Radian(Ogre::Degree(OrientationValue.z));
				Ogre::Vector3 rkAxis = Ogre::Vector3(OrientationValue.w,OrientationValue.x,OrientationValue.y);
				rkAxis.normalise();
				Ogre::Quaternion Orientation;
				Orientation.FromAngleAxis(rfAngle,rkAxis);
				Ogre::String valueString = Ogre::StringConverter::toString(rkAxis);
				valueString += " ";
				valueString += Ogre::StringConverter::toString(OrientationValue.z);
				id.GetProperty().SetValueFromString(valueString.c_str(),wxPG_FULL_VALUE);
				propertyValue = Ogre::StringConverter::toString(Orientation);
		
			}

			EffectObjectPropertyClass* pProperty = dynamic_cast<EffectObjectPropertyClass*>(&(parentId.GetProperty()));
			assert(pProperty);
			Ogre::StringInterface* pElement = (Ogre::StringInterface*)(pProperty->GetData());
			pElement->setParameter(propertyName, propertyValue);

			BulletEditor* pBulletEditor = m_Frame->GetBulletEditor();
			pBulletEditor->ResetBulletObject();
			pBulletEditor->RefreshBulletTreeItem("BulletSystem Element item",propertyName,pElement);
		}
	}
	//材质属性修改
	else if(m_EditType == eProperty_edit_material)
	{
		if(parentId.GetProperty().GetName() == "Material")
		{
			EffectObjectPropertyClass* pProperty = dynamic_cast<EffectObjectPropertyClass*>(&(parentId.GetProperty()));
			assert(pProperty);
			Ogre::Material* pMaterial = (Ogre::Material*)(pProperty->GetData());
			if (propertyName == "name")
			{
				if(propertyValue == "")
					return;
			
				Ogre::MaterialPtr newMaterial = pMaterial->clone(propertyValue);
				m_Frame->GetMaterialEditor()->InitMaterialEditor(newMaterial,propertyValue);
				InitMaterialEditor(newMaterial,propertyValue);
				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("material item",propertyName.c_str(),propertyValue.c_str(),pMaterial);

			}
			else if (propertyName == "receive_shadows")
			{
				if(propertyValue == "on")
					pMaterial->setReceiveShadows(true);
				else if(propertyValue == "off")
					pMaterial->setReceiveShadows(false);
				else
					return;

				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("material item",propertyName.c_str(),propertyValue.c_str(),pMaterial);
			}
			else if(propertyName == "transparency_casts_shadows")
			{
				if(propertyValue == "on")
					pMaterial->setTransparencyCastsShadows(true);
				else if(propertyValue == "off")
					pMaterial->setTransparencyCastsShadows(false);
				else 
					return;
				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("material item",propertyName.c_str(),propertyValue.c_str(),pMaterial);
			}
		}
		else if(parentId.GetProperty().GetName() == "technique")
		{
			EffectObjectPropertyClass* pProperty = dynamic_cast<EffectObjectPropertyClass*>(&(parentId.GetProperty()));
			assert(pProperty);
			Ogre::Technique* pTechnique = (Ogre::Technique*)(pProperty->GetData());
			if(propertyName == "name")
			{
				pTechnique->setName(propertyValue);
				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("technique item",propertyName.c_str(),propertyValue.c_str(),pTechnique);
				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("technique","technique",propertyValue.c_str(),pTechnique);
			}
			else if(propertyName == "lod_index")
			{
				pTechnique->setLodIndex(Ogre::StringConverter::parseInt(propertyValue));
				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("technique item",propertyName.c_str(),propertyValue.c_str(),pTechnique);
			}
			else if(propertyName == "scheme")
			{
				pTechnique->setSchemeName(propertyValue);
				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("technique item",propertyName.c_str(),propertyValue.c_str(),pTechnique);
			}
		}
		else if(parentId.GetProperty().GetName() == "pass")
		{
			EffectObjectPropertyClass* pProperty = dynamic_cast<EffectObjectPropertyClass*>(&(parentId.GetProperty()));
			assert(pProperty);
			Ogre::Pass* pPass = (Ogre::Pass*)(pProperty->GetData());
			if(propertyName == "name")
			{
				pPass->setName(propertyValue);
				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("pass item",propertyName.c_str(),propertyValue.c_str(),pPass);
				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("pass","pass",propertyValue.c_str(),pPass);
			}
			else if(propertyName == "ambient")
			{
				pPass->setAmbient(Ogre::StringConverter::parseColourValue(propertyValue));
				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("pass item",propertyName.c_str(),propertyValue.c_str(),pPass);
			}
			else if(propertyName == "diffuse")
			{
				pPass->setDiffuse(Ogre::StringConverter::parseColourValue(propertyValue));
				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("pass item",propertyName.c_str(),propertyValue.c_str(),pPass);
			}
			else if(propertyName == "specular")
			{
				pPass->setSpecular(Ogre::StringConverter::parseColourValue(propertyValue));
				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("pass item",propertyName.c_str(),propertyValue.c_str(),pPass);
			}
			else if(propertyName == "emissive")
			{
				pPass->setSelfIllumination(Ogre::StringConverter::parseColourValue(propertyValue));
				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("pass item",propertyName.c_str(),propertyValue.c_str(),pPass);
			}
			else if(propertyName == "scene_blend")
			{
				if(propertyValue == "transparent_alpha")
					pPass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
				else if(propertyValue == "transparent_color")
					pPass->setSceneBlending(Ogre::SBT_TRANSPARENT_COLOUR);
				else if(propertyValue == "modulate")
					pPass->setSceneBlending(Ogre::SBT_MODULATE);
				else if(propertyValue == "add")
					pPass->setSceneBlending(Ogre::SBT_ADD);
				else if(propertyValue == "replace")
					pPass->setSceneBlending(Ogre::SBT_REPLACE);

				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("pass item",propertyName.c_str(),propertyValue.c_str(),pPass);
			}
			else if(propertyName == "depth_check")
			{
				pPass->setDepthCheckEnabled(Ogre::StringConverter::parseBool(propertyValue));
				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("pass item",propertyName.c_str(),propertyValue.c_str(),pPass);
			}
			else if (propertyName == "depth_write")
			{
				pPass->setDepthWriteEnabled(Ogre::StringConverter::parseBool(propertyValue));
				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("pass item",propertyName.c_str(),propertyValue.c_str(),pPass);
			}
			else if(propertyName == "depth_func")
			{
				if(propertyValue == "always_fail")
					pPass->setDepthFunction(Ogre::CMPF_ALWAYS_FAIL);
				else if(propertyValue == "always_pass")
					pPass->setDepthFunction(Ogre::CMPF_ALWAYS_PASS);
				else if(propertyValue == "less")
					pPass->setDepthFunction(Ogre::CMPF_LESS);
				else if(propertyValue == "less_equal")
					pPass->setDepthFunction(Ogre::CMPF_LESS_EQUAL);
				else if(propertyValue == "equal")
					pPass->setDepthFunction(Ogre::CMPF_EQUAL);
				else if(propertyValue == "not_equal")
					pPass->setDepthFunction(Ogre::CMPF_NOT_EQUAL);
				else if(propertyValue == "greater_equal")
					pPass->setDepthFunction(Ogre::CMPF_GREATER_EQUAL);
				else if(propertyValue == "greater")
					pPass->setDepthFunction(Ogre::CMPF_GREATER);
				else
					return;

				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("pass item",propertyName.c_str(),propertyValue.c_str(),pPass);
			}
			else if(propertyName == "colour_write")
			{
				pPass->setColourWriteEnabled(Ogre::StringConverter::parseBool(propertyValue));
				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("pass item",propertyName.c_str(),propertyValue.c_str(),pPass);
			}
			else if(propertyName == "cull_hardware")
			{
				if(propertyValue == "none")
					pPass->setCullingMode(Ogre::CULL_NONE);
				else if(propertyValue == "clockwise")
					pPass->setCullingMode(Ogre::CULL_CLOCKWISE);
				else if(propertyValue == "anticlockwise")
					pPass->setCullingMode(Ogre::CULL_ANTICLOCKWISE);
				else 
					return;
				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("pass item",propertyName.c_str(),propertyValue.c_str(),pPass);
			}
			else if(propertyName == "cull_software")
			{
				if(propertyValue == "none")
					pPass->setManualCullingMode(Ogre::MANUAL_CULL_NONE);
				else if(propertyValue == "back")
					pPass->setManualCullingMode(Ogre::MANUAL_CULL_BACK);
				else if(propertyValue == "front")
					pPass->setManualCullingMode(Ogre::MANUAL_CULL_FRONT);
				else 
					return;
				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("pass item",propertyName.c_str(),propertyValue.c_str(),pPass);
			}
			else if(propertyName == "lighting")
			{
				pPass->setLightingEnabled(Ogre::StringConverter::parseBool(propertyValue));
				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("pass item",propertyName.c_str(),propertyValue.c_str(),pPass);
			}
			else if(propertyName == "shading")
			{
				if(propertyValue == "flat")
					pPass->setShadingMode(Ogre::SO_FLAT);
				else if(propertyValue == "gouraud")
					pPass->setShadingMode(Ogre::SO_GOURAUD);
				else if(propertyValue == "phong")
					pPass->setShadingMode(Ogre::SO_PHONG);
				else 
					return;
				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("pass item",propertyName.c_str(),propertyValue.c_str(),pPass);
			}
			else if(propertyName == "polygon_mode")
			{
				if(propertyValue == "points")
					pPass->setPolygonMode(Ogre::PM_POINTS);
				else if(propertyValue == "wireframe")
					pPass->setPolygonMode(Ogre::PM_WIREFRAME);
				else if(propertyValue == "solid")
					pPass->setPolygonMode(Ogre::PM_SOLID);
				else 
					return;
				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("pass item",propertyName.c_str(),propertyValue.c_str(),pPass);
			}
			else if(propertyName == "depth_bias")
			{
				pPass->setDepthBias(Ogre::StringConverter::parseInt(propertyValue));
				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("pass item",propertyName.c_str(),propertyValue.c_str(),pPass);
			}
			else if(propertyName == "fog_override")
			{
				Ogre::StringUtil::toLowerCase(propertyValue);
				Ogre::StringVector vecparams = Ogre::StringUtil::split(propertyValue, " \t");
				if (vecparams[0]=="true")
				{
					// if true, we need to see if they supplied all arguments, or just the 1... if just the one,
					// Assume they want to disable the default fog from effecting this material.
					if( vecparams.size() == 8 )
					{
						Ogre::FogMode mFogtype;
						if( vecparams[1] == "none" )
							mFogtype = Ogre::FOG_NONE;
						else if( vecparams[1] == "linear" )
							mFogtype = Ogre::FOG_LINEAR;
						else if( vecparams[1] == "exp" )
							mFogtype = Ogre::FOG_EXP;
						else if( vecparams[1] == "exp2" )
							mFogtype = Ogre::FOG_EXP2;
						else
							return;

						pPass->setFog(
							true,
							mFogtype,
							Ogre::ColourValue(
							Ogre::StringConverter::parseReal(vecparams[2]),
							Ogre::StringConverter::parseReal(vecparams[3]),
							Ogre::StringConverter::parseReal(vecparams[4])),
							Ogre::StringConverter::parseReal(vecparams[5]),
							Ogre::StringConverter::parseReal(vecparams[6]),
							Ogre::StringConverter::parseReal(vecparams[7])
							);
					}
					else
					{
						pPass->setFog(true);
					}
				}
				else if (vecparams[0]=="false")
					pPass->setFog(false);

				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("pass item",propertyName.c_str(),propertyValue.c_str(),pPass);
			}
			else if(propertyName == "max_lights")
			{
				pPass->setMaxSimultaneousLights(Ogre::StringConverter::parseInt(propertyValue));
				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("pass item",propertyName.c_str(),propertyValue.c_str(),pPass);
			}
			/*else if(propertyName == "only_light_type")
			{
				if(propertyValue == "point")
				pPass->setDepthCheckEnabled(Ogre::StringConverter::parseBool(propertyValue));
				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("pass item",propertyName.c_str(),propertyValue.c_str(),pPass);
			}*/
			else if(propertyName == "point_size")
			{
				pPass->setPointSize(Ogre::StringConverter::parseReal(propertyValue));
				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("pass item",propertyName.c_str(),propertyValue.c_str(),pPass);
			}
			else if(propertyName == "point_sprites")
			{
				pPass->setPointSpritesEnabled(Ogre::StringConverter::parseBool(propertyValue));
				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("pass item",propertyName.c_str(),propertyValue.c_str(),pPass);
			}
			else if(propertyName == "point_size_attenuation")
			{
				Ogre::StringVector vecparams = Ogre::StringUtil::split(propertyValue, " \t");
				if (vecparams.size() != 1 && vecparams.size() != 4)
				{
					return;
				}
				if (vecparams[0] == "off")
				{
					pPass->setPointAttenuation(false);
				}
				else if (vecparams[0] == "on")
				{
					if (vecparams.size() == 4)
					{
						pPass->setPointAttenuation(true,
							Ogre::StringConverter::parseReal(vecparams[1]),
							Ogre::StringConverter::parseReal(vecparams[2]),
							Ogre::StringConverter::parseReal(vecparams[3]));
					}
					else
					{
						pPass->setPointAttenuation(true);
					}
				}
				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("pass item",propertyName.c_str(),propertyValue.c_str(),pPass);
			}
			else if(propertyName == "point_size_min")
			{
				pPass->setPointMinSize(Ogre::StringConverter::parseReal(propertyValue));
				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("pass item",propertyName.c_str(),propertyValue.c_str(),pPass);
			}
			else if(propertyName == "point_size_max")
			{
				pPass->setPointMaxSize(Ogre::StringConverter::parseReal(propertyValue));
				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("pass item",propertyName.c_str(),propertyValue.c_str(),pPass);
			}
		}
		else if(parentId.GetProperty().GetName() == "texture_unit")
		{
			EffectObjectPropertyClass* pProperty = dynamic_cast<EffectObjectPropertyClass*>(&(parentId.GetProperty()));
			assert(pProperty);
			Ogre::TextureUnitState* pTextureUnit = (Ogre::TextureUnitState*)(pProperty->GetData());
			if(propertyName == "name")
			{
				pTextureUnit->setName(propertyValue);
				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("texture_unit item",propertyName.c_str(),propertyValue.c_str(),pTextureUnit);
				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("texture_unit","texture_unit",propertyValue.c_str(),pTextureUnit);
			}
			else if(propertyName == "texture")
			{
				pTextureUnit->setTextureName(propertyValue);
				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("texture_unit item",propertyName.c_str(),propertyValue.c_str(),pTextureUnit);

			}
			else if(propertyName == "texture_alias")
			{
				pTextureUnit->setTextureNameAlias(propertyValue);
				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("texture_unit item",propertyName.c_str(),propertyValue.c_str(),pTextureUnit);

			}
			else if(propertyName == "anim_texture")
			{
				Ogre::StringVector vecparams = Ogre::StringUtil::split(propertyValue, " \t");
				size_t numParams = vecparams.size();
				// Determine which form it is
				// Must have at least 3 params though
				if (numParams < 3)
				{
					return ;
				}
				if (numParams == 3 && Ogre::StringConverter::parseInt(vecparams[1]) != 0 )
				{
					// First form using base name & number of frames
					pTextureUnit->setAnimatedTextureName(
						vecparams[0],
						Ogre::StringConverter::parseInt(vecparams[1]),
						Ogre::StringConverter::parseReal(vecparams[2]));
				}
				else
				{
					// Second form using individual names
					pTextureUnit->setAnimatedTextureName(
						(Ogre::String*)&vecparams[0],
						numParams-1,
						Ogre::StringConverter::parseReal(vecparams[numParams-1]));
				}
				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("texture_unit item",propertyName.c_str(),propertyValue.c_str(),pTextureUnit);

			}
			else if(propertyName == "tex_coord_set")
			{
				pTextureUnit->setTextureCoordSet(Ogre::StringConverter::parseInt(propertyValue));
				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("texture_unit item",propertyName.c_str(),propertyValue.c_str(),pTextureUnit);
			}
			else if(propertyName == "tex_border_colour")
			{
				pTextureUnit->setTextureBorderColour(Ogre::StringConverter::parseColourValue(propertyValue));
				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("texture_unit item",propertyName.c_str(),propertyValue.c_str(),pTextureUnit);
			}
			else if(propertyName == "tex_address_mode_u")
			{
				Ogre::TextureUnitState::UVWAddressingMode addressingMode = 
					pTextureUnit->getTextureAddressingMode();
				if(propertyValue == "warp")
					addressingMode.u = Ogre::TextureUnitState::TAM_WRAP;
				else if(propertyValue == "mirror")
					addressingMode.u = Ogre::TextureUnitState::TAM_MIRROR;
				else if(propertyValue == "clamp")
					addressingMode.u = Ogre::TextureUnitState::TAM_CLAMP;
				else if(propertyValue == "border")
					addressingMode.u = Ogre::TextureUnitState::TAM_BORDER;

				pTextureUnit->setTextureAddressingMode(addressingMode);

				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("texture_unit item",propertyName.c_str(),propertyValue.c_str(),pTextureUnit);
			}
			else if(propertyName == "tex_address_mode_v")
			{
				Ogre::TextureUnitState::UVWAddressingMode addressingMode = 
					pTextureUnit->getTextureAddressingMode();
				if(propertyValue == "warp")
					addressingMode.v = Ogre::TextureUnitState::TAM_WRAP;
				else if(propertyValue == "mirror")
					addressingMode.v = Ogre::TextureUnitState::TAM_MIRROR;
				else if(propertyValue == "clamp")
					addressingMode.v = Ogre::TextureUnitState::TAM_CLAMP;
				else if(propertyValue == "border")
					addressingMode.v = Ogre::TextureUnitState::TAM_BORDER;

				pTextureUnit->setTextureAddressingMode(addressingMode);

				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("texture_unit item",propertyName.c_str(),propertyValue.c_str(),pTextureUnit);
			}
			else if(propertyName == "tex_address_mode_w")
			{
				Ogre::TextureUnitState::UVWAddressingMode addressingMode = 
					pTextureUnit->getTextureAddressingMode();
				if(propertyValue == "warp")
					addressingMode.w = Ogre::TextureUnitState::TAM_WRAP;
				else if(propertyValue == "mirror")
					addressingMode.w = Ogre::TextureUnitState::TAM_MIRROR;
				else if(propertyValue == "clamp")
					addressingMode.w = Ogre::TextureUnitState::TAM_CLAMP;
				else if(propertyValue == "border")
					addressingMode.w = Ogre::TextureUnitState::TAM_BORDER;

				pTextureUnit->setTextureAddressingMode(addressingMode);

				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("texture_unit item",propertyName.c_str(),propertyValue.c_str(),pTextureUnit);
			}
			else if(propertyName == "filtering_min")
			{
				if(propertyValue == "none")
					pTextureUnit->setTextureFiltering(Ogre::FT_MIN,Ogre::FO_NONE);
				else if(propertyValue == "point")
					pTextureUnit->setTextureFiltering(Ogre::FT_MIN,Ogre::FO_POINT);
				else if(propertyValue == "linear")
					pTextureUnit->setTextureFiltering(Ogre::FT_MIN,Ogre::FO_LINEAR);
				else if(propertyValue == "anisotropic")
					pTextureUnit->setTextureFiltering(Ogre::FT_MIN,Ogre::FO_ANISOTROPIC);

				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("texture_unit item",propertyName.c_str(),propertyValue.c_str(),pTextureUnit);
			}
			else if(propertyName == "filtering_mag")
			{
				if(propertyValue == "none")
					pTextureUnit->setTextureFiltering(Ogre::FT_MAG,Ogre::FO_NONE);
				else if(propertyValue == "point")
					pTextureUnit->setTextureFiltering(Ogre::FT_MAG,Ogre::FO_POINT);
				else if(propertyValue == "linear")
					pTextureUnit->setTextureFiltering(Ogre::FT_MAG,Ogre::FO_LINEAR);
				else if(propertyValue == "anisotropic")
					pTextureUnit->setTextureFiltering(Ogre::FT_MAG,Ogre::FO_ANISOTROPIC);

				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("texture_unit item",propertyName.c_str(),propertyValue.c_str(),pTextureUnit);
			}
			else if(propertyName == "mipmaps")
			{
				if(propertyValue == "none")
					pTextureUnit->setTextureFiltering(Ogre::FT_MIP,Ogre::FO_NONE);
				else if(propertyValue == "point")
					pTextureUnit->setTextureFiltering(Ogre::FT_MIP,Ogre::FO_POINT);
				else if(propertyValue == "linear")
					pTextureUnit->setTextureFiltering(Ogre::FT_MIP,Ogre::FO_LINEAR);
				else if(propertyValue == "anisotropic")
					pTextureUnit->setTextureFiltering(Ogre::FT_MIP,Ogre::FO_ANISOTROPIC);

				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("texture_unit item",propertyName.c_str(),propertyValue.c_str(),pTextureUnit);
			}
			else if(propertyName == "filtering_mip")
			{
				pTextureUnit->setNumMipmaps(Ogre::StringConverter::parseInt(propertyValue));
				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("texture_unit item",propertyName.c_str(),propertyValue.c_str(),pTextureUnit);
			}
			else if(propertyName == "colour_op")
			{
				if(propertyValue == "replace")
					pTextureUnit->setColourOperation(Ogre::LBO_REPLACE);
				else if(propertyValue == "add")
					pTextureUnit->setColourOperation(Ogre::LBO_ADD);
				else if(propertyValue == "modulate")
					pTextureUnit->setColourOperation(Ogre::LBO_MODULATE);
				else if(propertyValue == "alpha_blend")
					pTextureUnit->setColourOperation(Ogre::LBO_ALPHA_BLEND);
				else
					return;

				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("texture_unit item",propertyName.c_str(),propertyValue.c_str(),pTextureUnit);
			}
			else if(propertyName == "colour_op_ex")
			{
				Ogre::StringUtil::toLowerCase(propertyValue);
				Ogre::StringVector vecparams = Ogre::StringUtil::split(propertyValue, " \t");
				size_t numParams = vecparams.size();

				if (numParams < 3 || numParams > 10)
				{
					return;
				}
				Ogre::LayerBlendOperationEx op;
				Ogre::LayerBlendSource src1, src2;
				Ogre::Real manual = 0.0;
				Ogre::ColourValue colSrc1 = Ogre::ColourValue::White;
				Ogre::ColourValue colSrc2 = Ogre::ColourValue::White;

				
				op = convertBlendOpEx(vecparams[0]);
				src1 = convertBlendSource(vecparams[1]);
				src2 = convertBlendSource(vecparams[2]);

				if (op == Ogre::LBX_BLEND_MANUAL)
				{
					if (numParams < 4)
					{
						return;
					}
					manual = Ogre::StringConverter::parseReal(vecparams[3]);
				}

				if (src1 == Ogre::LBS_MANUAL)
				{
					unsigned int parIndex = 3;
					if (op == Ogre::LBX_BLEND_MANUAL)
						parIndex++;

					if (numParams < parIndex + 3)
					{
						return;
					}

					colSrc1.r = Ogre::StringConverter::parseReal(vecparams[parIndex++]);
					colSrc1.g = Ogre::StringConverter::parseReal(vecparams[parIndex++]);
					colSrc1.b = Ogre::StringConverter::parseReal(vecparams[parIndex++]);
					if (numParams > parIndex)
					{
						colSrc1.a = Ogre::StringConverter::parseReal(vecparams[parIndex]);
					}
					else
					{
						colSrc1.a = 1.0f;
					}
				}

				if (src2 == Ogre::LBS_MANUAL)
				{
					unsigned int parIndex = 3;
					if (op == Ogre::LBX_BLEND_MANUAL)
						parIndex++;
					if (src1 == Ogre::LBS_MANUAL)
						parIndex += 3;

					if (numParams < parIndex + 3)
					{
						return ;
					}

					colSrc2.r = Ogre::StringConverter::parseReal(vecparams[parIndex++]);
					colSrc2.g = Ogre::StringConverter::parseReal(vecparams[parIndex++]);
					colSrc2.b = Ogre::StringConverter::parseReal(vecparams[parIndex++]);
					if (numParams > parIndex)
					{
						colSrc2.a = Ogre::StringConverter::parseReal(vecparams[parIndex]);
					}
					else
					{
						colSrc2.a = 1.0f;
					}
				}
				pTextureUnit->setColourOperationEx(op, src1, src2, colSrc1, colSrc2, manual);
				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("texture_unit item",propertyName.c_str(),propertyValue.c_str(),pTextureUnit);
			}
			else if(propertyName == "scroll_u")
			{
				pTextureUnit->setTextureUScroll(Ogre::StringConverter::parseReal(propertyValue));
				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("texture_unit item",propertyName.c_str(),propertyValue.c_str(),pTextureUnit);
			}
			else if(propertyName == "scroll_v")
			{
				pTextureUnit->setTextureVScroll(Ogre::StringConverter::parseReal(propertyValue));
				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("texture_unit item",propertyName.c_str(),propertyValue.c_str(),pTextureUnit);
			}
			else if(propertyName == "env_map")
			{
				if(propertyValue == "off")
					pTextureUnit->setEnvironmentMap(false);
				else if(propertyValue == "planar")
					pTextureUnit->setEnvironmentMap(true,Ogre::TextureUnitState::ENV_PLANAR);
				else if(propertyValue == "curved")
					pTextureUnit->setEnvironmentMap(true,Ogre::TextureUnitState::ENV_CURVED);
				else if(propertyValue == "reflection")
					pTextureUnit->setEnvironmentMap(true,Ogre::TextureUnitState::ENV_REFLECTION);
				else if(propertyValue == "normal")
					pTextureUnit->setEnvironmentMap(true,Ogre::TextureUnitState::ENV_NORMAL);
				else
					return;

				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("texture_unit item",propertyName.c_str(),propertyValue.c_str(),pTextureUnit);
			}
			else if(propertyName == "rotate")
			{
				Ogre::Radian redian = Ogre::Radian(Ogre::Degree(Ogre::StringConverter::parseReal(propertyValue)));
				pTextureUnit->setTextureRotate(redian);
				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("texture_unit item",propertyName.c_str(),propertyValue.c_str(),pTextureUnit);
			}
			else if(propertyName == "scale_u")
			{
				pTextureUnit->setTextureUScale(Ogre::StringConverter::parseReal(propertyValue));
				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("texture_unit item",propertyName.c_str(),propertyValue.c_str(),pTextureUnit);
			}
			else if(propertyName == "scale_v")
			{
				pTextureUnit->setTextureVScale(Ogre::StringConverter::parseReal(propertyValue));
				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("texture_unit item",propertyName.c_str(),propertyValue.c_str(),pTextureUnit);
			}
			else if(propertyName == "scroll_anim")
			{
				Ogre::Vector3 vSpeed = Ogre::StringConverter::parseVector3(propertyValue);
				pTextureUnit->setScrollAnimation(vSpeed.x,vSpeed.y);
				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("texture_unit item",propertyName.c_str(),propertyValue.c_str(),pTextureUnit);
			}
			else if(propertyName == "rotate_anim")
			{
				pTextureUnit->setRotateAnimation(Ogre::StringConverter::parseReal(propertyValue));
				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("texture_unit item",propertyName.c_str(),propertyValue.c_str(),pTextureUnit);
			}
			else if(propertyName == "wave_xform")
			{
				Ogre::StringUtil::toLowerCase(propertyValue);
				Ogre::StringVector vecparams = Ogre::StringUtil::split(propertyValue, " \t");

				if (vecparams.size() != 6)
				{
					return;
				}
				Ogre::TextureUnitState::TextureTransformType ttype;
				Ogre::WaveformType waveType;
				// Check transform type
				if (vecparams[0]=="scroll_x")
					ttype = Ogre::TextureUnitState::TT_TRANSLATE_U;
				else if (vecparams[0]=="scroll_y")
					ttype = Ogre::TextureUnitState::TT_TRANSLATE_V;
				else if (vecparams[0]=="rotate")
					ttype = Ogre::TextureUnitState::TT_ROTATE;
				else if (vecparams[0]=="scale_x")
					ttype = Ogre::TextureUnitState::TT_SCALE_U;
				else if (vecparams[0]=="scale_y")
					ttype = Ogre::TextureUnitState::TT_SCALE_V;
				else
				{
					return;
				}
				// Check wave type
				if (vecparams[1]=="sine")
					waveType = Ogre::WFT_SINE;
				else if (vecparams[1]=="triangle")
					waveType = Ogre::WFT_TRIANGLE;
				else if (vecparams[1]=="square")
					waveType = Ogre::WFT_SQUARE;
				else if (vecparams[1]=="sawtooth")
					waveType = Ogre::WFT_SAWTOOTH;
				else if (vecparams[1]=="inverse_sawtooth")
					waveType = Ogre::WFT_INVERSE_SAWTOOTH;
				else
				{
					return;
				}

				pTextureUnit->setTransformAnimation(
					ttype,
					waveType,
					Ogre::StringConverter::parseReal(vecparams[2]),
					Ogre::StringConverter::parseReal(vecparams[3]),
					Ogre::StringConverter::parseReal(vecparams[4]),
					Ogre::StringConverter::parseReal(vecparams[5]) );
				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("texture_unit item",propertyName.c_str(),propertyValue.c_str(),pTextureUnit);
			}
			else if(propertyName == "transform")
			{
				pTextureUnit->setTextureTransform(Ogre::StringConverter::parseMatrix4(propertyValue));
				m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("texture_unit item",propertyName.c_str(),propertyValue.c_str(),pTextureUnit);
			}
		}		

	}
}

void EffectObjectPropertyEditor::cloneParticleSystem(Ogre::ParticleSystem* newPSys, const Ogre::ParticleSystem* const clonedPSys, bool copyQuota)
{
	newPSys->removeAllEmitters();
	newPSys->removeAllAffectors();

	if ( copyQuota )
	{
		clonedPSys->copyParametersTo(newPSys);
	}

	// Copy renderer params
	newPSys->setRenderer(clonedPSys->getRendererName());
	if (newPSys->getRenderer() && clonedPSys->getRenderer())
	{
		clonedPSys->getRenderer()->copyParametersTo( newPSys->getRenderer() );
	}

	// Copy emitters
	for (unsigned short i = 0; i < clonedPSys->getNumEmitters(); ++i)
	{
		Ogre::ParticleEmitter* rhsEm = clonedPSys->getEmitter(i);
		Ogre::ParticleEmitter* newEm = newPSys->addEmitter(rhsEm->getType());
		rhsEm->copyParametersTo(newEm);
	}
	// Copy affectors
	for (unsigned short i = 0; i < clonedPSys->getNumAffectors(); ++i)
	{
		Ogre::ParticleAffector* rhsAf = clonedPSys->getAffector(i);
		Ogre::ParticleAffector* newAf = newPSys->addAffector(rhsAf->getType());
		rhsAf->copyParametersTo(newAf);
	}

	newPSys->setMaterialName(clonedPSys->getMaterialName());
	newPSys->setDefaultDimensions(clonedPSys->getDefaultWidth(), clonedPSys->getDefaultHeight());
	newPSys->setCullIndividually(clonedPSys->getCullIndividually());
}

//void EffectObjectPropertyEditor::unloadPsystem(Ogre::ParticleSystem *system, const std::string& templateName)
//{	
//	if(system)
//	{
//		OGRE_DELETE system;
//		system = NULL;
//	}
//}

void EffectObjectPropertyEditor::OnPropertyGridSelect ( wxPropertyGridEvent& event )
{
	//wxPGId id = event.GetProperty();

	//if ( true == id.IsOk() && false == mPropertyGridManager->IsPropertyEnabled(id) )
	//{
	//	//mPropertyGridManager->GetGrid()->CollapseAll();
	//	mPropertyGridManager->Expand(id);
	//}

	mPropertyGridManager->OnPropertyGridSelect(event);
}
void EffectObjectPropertyEditor::OnPropertyGridRightClick(wxPropertyGridEvent& event )
{
	// Skip parent properties
	if (event.GetPropertyPtr()->GetParentingType() != 0)
		return;

	wxPGId id = event.GetPropertyPtr()->GetId();
	wxPGId parentId = mPropertyGridManager->GetPropertyParent(id);

	Fairy::String propertyName = event.GetPropertyName().c_str();
	Fairy::String propertyValue = event.GetPropertyValueAsString().c_str();


	if(m_EditType == eProperty_edit_effect)
	{
		if(parentId.GetProperty().GetName() == "EffectElement" && parentId.GetProperty().GetLabel() == "Mesh")
		{
			
			if ( propertyName == "Material" )
			{
				EffectObjectPropertyClass* pProperty = dynamic_cast<EffectObjectPropertyClass*>(&(parentId.GetProperty()));
				assert(pProperty);
				Fairy::MeshElement* pMeshElement = (Fairy::MeshElement*)(pProperty->GetData());
				m_Frame->GetEffectObjectEditor()->EditMeshMatrial(pMeshElement);
				m_Frame->GetEffectObjectEditor()->RefreshEffectTreeItem("Effect element item",propertyName.c_str(),
					pMeshElement);

			}
		}
		else if(parentId.GetProperty().GetName() == "EffectElement" && parentId.GetProperty().GetLabel() == "BillboardSet")
		{
			if ( propertyName == "Material" )
			{
				EffectObjectPropertyClass* pProperty = dynamic_cast<EffectObjectPropertyClass*>(&(parentId.GetProperty()));
				assert(pProperty);
				Fairy::BillboardSetElement* pBillboardElement = (Fairy::BillboardSetElement*)(pProperty->GetData());
				m_Frame->GetEffectObjectEditor()->EditBillboardMatrial(pBillboardElement);
				m_Frame->GetEffectObjectEditor()->RefreshEffectTreeItem("Effect element item",propertyName.c_str(),
					pBillboardElement);

			}
		}
		else if(parentId.GetProperty().GetName() == "EffectElement" && parentId.GetProperty().GetLabel() == "Projector")
		{
			if ( propertyName == "Material" )
			{
				EffectObjectPropertyClass* pProperty = dynamic_cast<EffectObjectPropertyClass*>(&(parentId.GetProperty()));
				assert(pProperty);
				Fairy::ProjectorElement* pProjectorElement = (Fairy::ProjectorElement*)(pProperty->GetData());
				m_Frame->GetEffectObjectEditor()->EditProjectorMatrial(pProjectorElement);
				m_Frame->GetEffectObjectEditor()->RefreshEffectTreeItem("Effect element item",propertyName.c_str(),
					pProjectorElement);

			}
		}
		else if(parentId.GetProperty().GetName() == "EffectElement" && parentId.GetProperty().GetLabel() == "Beam")
		{
			if ( propertyName == "Material" )
			{
				EffectObjectPropertyClass* pProperty = dynamic_cast<EffectObjectPropertyClass*>(&(parentId.GetProperty()));
				assert(pProperty);
				Fairy::BeamElement* pBeamElement = (Fairy::BeamElement*)(pProperty->GetData());
				m_Frame->GetEffectObjectEditor()->EditBeamMatrial(pBeamElement);
				m_Frame->GetEffectObjectEditor()->RefreshEffectTreeItem("Effect element item",propertyName.c_str(),
					pBeamElement);

			}
		}
		else if(parentId.GetProperty().GetName() == "EffectElement" && parentId.GetProperty().GetLabel() == "Cylinder")
		{
			if ( propertyName == "Material" )
			{
				EffectObjectPropertyClass* pProperty = dynamic_cast<EffectObjectPropertyClass*>(&(parentId.GetProperty()));
				assert(pProperty);
				Fairy::CylinderElement* pCylinderElement = (Fairy::CylinderElement*)(pProperty->GetData());
				m_Frame->GetEffectObjectEditor()->EditCylinderMatrial(pCylinderElement);
				m_Frame->GetEffectObjectEditor()->RefreshEffectTreeItem("Effect element item",propertyName.c_str(),
					pCylinderElement);

			}
		}
		else if(parentId.GetProperty().GetName() == "ParticleBasicSetting")
		{
			EffectObjectPropertyClass* pProperty = dynamic_cast<EffectObjectPropertyClass*>(&(parentId.GetProperty()));
			assert(pProperty);

			Ogre::ParticleSystem* pParticleSystem = (Ogre::ParticleSystem*)(pProperty->GetData());
			if ( propertyName == "material" )
			{
				m_Frame->GetEffectObjectEditor()->EditMatrial();
				m_Frame->GetEffectObjectEditor()->RefreshEffectTreeItem("Particle BasicSetting item material",propertyName.c_str(),
					pParticleSystem);

			}
		}
	}
	else if(m_EditType == eProperty_edit_skill)
	{
		if(parentId.GetProperty().GetName() == "SkillElement" && parentId.GetProperty().GetLabel() == "Ribbon")
		{
			if ( propertyName == "Material" )
			{
				EffectObjectPropertyClass* pProperty = dynamic_cast<EffectObjectPropertyClass*>(&(parentId.GetProperty()));
				assert(pProperty);
				Fairy::AnimationRibbon* pRibbon = (Fairy::AnimationRibbon*)(pProperty->GetData());
				m_Frame->GetSkillObjectEidtor()->EditRibbonMatrial(pRibbon);
				m_Frame->GetSkillObjectEidtor()->RefreshSkillTreeItem("Skill Element item",propertyName.c_str(),pRibbon);

			}
		}
	}
	else if(m_EditType == eProperty_edit_material)
	{
		if(parentId.GetProperty().GetName() == "texture_unit")
		{
			EffectObjectPropertyClass* pProperty = dynamic_cast<EffectObjectPropertyClass*>(&(parentId.GetProperty()));
			assert(pProperty);
			Ogre::TextureUnitState* pTextureUnit = (Ogre::TextureUnitState*)(pProperty->GetData());
			if(propertyName == "texture")
			{
				wxFileDialog dialog(this, _T("贴图文件"), wxT("../resource/media/特效/贴图/"), wxT(""),
					"All texture files (*.dds;*.bmp;*.gif;*.png;*.jpg;*.tga)|*.dds;*.bmp;*.gif;*.png;*.jpg;*.tga", wxMULTIPLE); 

				if (dialog.ShowModal() == wxID_OK)
				{		
					mSelectedFileNames.clear();
					dialog.GetFilenames(mSelectedFileNames);
					
					pTextureUnit->setTextureName(mSelectedFileNames[0].c_str());
					InitMaterialTextureUnit(pTextureUnit);
					m_Frame->GetMaterialEditor()->RefreshMaterialTreeItem("texture_unit item",propertyName.c_str(),mSelectedFileNames[0],pTextureUnit);

				}
			}
			else if(propertyName == "anim_texture")
			{
			
			}

		}
	}

}
void EffectObjectPropertyEditor::clearAllProperty()
{
	mPropertyGridManager->Freeze();
	mPropertyGridManager->GetGrid()->Clear();
	mPropertyGridManager->Thaw();
}

void EffectObjectPropertyEditor::InitEffectEditor(Fairy::Effect *pEffect,const Ogre::String &templateName)
{
	mEffect = pEffect;
	mEffectTemplateName = templateName;
	m_EditType = eProperty_edit_effect;
	InitEffectSettingProperty();
	SetTitle(mEffectTemplateName.c_str());
}
void EffectObjectPropertyEditor::InitSkillEditor(Fairy::Skill* pSkill,Ogre::String& tmplateName)
{
	mSkill = pSkill;
	mSkillTemplateName = tmplateName;
	m_EditType = eProperty_edit_skill;
	InitSkillSettingProperty();
	SetTitle(mSkillTemplateName.c_str());

}
void EffectObjectPropertyEditor::InitMaterialEditor(Ogre::MaterialPtr& pMaterial,const Ogre::String &templateName)
{
	mMaterial = pMaterial;
	mMaterialTemplateName = templateName;
	m_EditType = eProperty_edit_material;
	InitMaterialProperty();
	SetTitle(mMaterialTemplateName.c_str());
	
}
void EffectObjectPropertyEditor::InitBulletEditor(Fairy::BulletFlowSystem* pBulletFlow,const Ogre::String &templateName)
{
	mBulletFlow = pBulletFlow;
	mBulletFlowTemplateName = templateName;
	m_EditType = eProperty_edit_bullet;
	InitBulletFlowProperty();
	SetTitle(mBulletFlowTemplateName.c_str());
}
void EffectObjectPropertyEditor::InitParticleSettingProperty(Ogre::ParticleSystem* pParticleSystem)
{
	mPropertyGridManager->Freeze();
	mPropertyGridManager->GetGrid()->Clear();

	// particle system option

	wxPGId id =	mPropertyGridManager->Append( new EffectObjectPropertyClass(wxT("BasicSetting"),wxT("ParticleBasicSetting"),pParticleSystem) );

	wxPGId itemId;

	if ( id.IsOk() )
	{
		mPropertyGridManager->DisableProperty(id);
		mPropertyGridManager->SetPropertyColour(id, *wxLIGHT_GREY);

		const Ogre::ParameterList &paraList = pParticleSystem->getParameters();

		for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
		{
			Ogre::String value = pParticleSystem->getParameter(paraList[paraCount].name);

			itemId = mPropertyGridManager->AppendIn( id, CreateParticleSettingProperty(paraList[paraCount], value) );
			mPropertyGridManager->SetPropertyColourToDefault(itemId);
		}
	}

	mPropertyGridManager->Thaw();
	mPropertyGridManager->ExpandAll();
	mPropertyGridManager->SelectProperty(id);
	mPropertyGridManager->Refresh();

}
void EffectObjectPropertyEditor::InitParticleEmitterProperty(Ogre::ParticleEmitter *emitter)
{
	mPropertyGridManager->Freeze();
	mPropertyGridManager->GetGrid()->Clear();

	// particle system option

	wxPGId itemId;

	assert (emitter);

	wxPGId id = mPropertyGridManager->Append( new EffectObjectPropertyClass(emitter->getType().c_str(),wxT("ParticleEmtter"),emitter) );

	if ( id.IsOk() )
	{
		mPropertyGridManager->DisableProperty(id);
		mPropertyGridManager->SetPropertyColour(id, *wxLIGHT_GREY);

		const Ogre::ParameterList &paraList = emitter->getParameters();

		for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
		{
			Ogre::String value = emitter->getParameter(paraList[paraCount].name);

			itemId = mPropertyGridManager->AppendIn( id, CreateParticleEmitterProperty(paraList[paraCount], value) );

			mPropertyGridManager->SetPropertyColourToDefault(itemId);
		}
	}		

	mPropertyGridManager->Thaw();
	mPropertyGridManager->ExpandAll();
	mPropertyGridManager->Refresh();

}
void EffectObjectPropertyEditor::InitParticleAffectorProperty(Ogre::ParticleAffector *affector)
{
	mPropertyGridManager->Freeze();
	mPropertyGridManager->GetGrid()->Clear();

	wxPGId itemId;

	assert (affector);

	wxPGId id = mPropertyGridManager->Append( new EffectObjectPropertyClass(affector->getType().c_str(),wxT("ParticleAffector"),affector) );

	if ( id.IsOk() )
	{
		mPropertyGridManager->DisableProperty(id);
		mPropertyGridManager->SetPropertyColour(id, *wxLIGHT_GREY);

		const Ogre::ParameterList &paraList = affector->getParameters();

		for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
		{
			Ogre::String value = affector->getParameter(paraList[paraCount].name);

			itemId = mPropertyGridManager->AppendIn( id, CreateParticleAffectorProperty(paraList[paraCount], value) );

			mPropertyGridManager->SetPropertyColourToDefault(itemId);

		}
	}		

	mPropertyGridManager->Thaw();
	mPropertyGridManager->ExpandAll();
	mPropertyGridManager->Refresh();
}
void EffectObjectPropertyEditor::InitParticleRenderProperty(Ogre::ParticleSystemRenderer *pRenderer)
{

	mPropertyGridManager->Freeze();
	mPropertyGridManager->GetGrid()->Clear();

	assert (pRenderer);
	wxPGId id = mPropertyGridManager->Append( new EffectObjectPropertyClass(pRenderer->getType().c_str(),wxT("ParticleRenderer"),pRenderer) );
	wxPGId itemId;

	if ( id.IsOk() )
	{
		mPropertyGridManager->DisableProperty(id);
		mPropertyGridManager->SetPropertyColour(id, *wxLIGHT_GREY);
		const Ogre::ParameterList &paraList = pRenderer->getParameters();

		for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
		{
			Ogre::String value = pRenderer->getParameter(paraList[paraCount].name);
			itemId = mPropertyGridManager->AppendIn( id, CreateParticleRenderProperty(pRenderer,paraList[paraCount], value) );
			mPropertyGridManager->SetPropertyColourToDefault(itemId);
		}
	}
	mPropertyGridManager->Thaw();
	mPropertyGridManager->ExpandAll();
	mPropertyGridManager->Refresh();

}

void EffectObjectPropertyEditor::InitEffectProperty()
{
	mPropertyGridManager->Freeze();
	mPropertyGridManager->GetGrid()->Clear();

	wxPGId itemId;
	wxPGId id = mPropertyGridManager->Append( new EffectObjectPropertyClass(mEffectTemplateName.c_str(),wxT("EffectSetting"),mEffect) );

	if ( id.IsOk() )
	{
		mPropertyGridManager->DisableProperty(id);
		mPropertyGridManager->SetPropertyColour(id, *wxLIGHT_GREY);

		const Ogre::ParameterList &paraList = mEffect->getParameters();
		for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
		{
			Ogre::String value = mEffect->getParameter(paraList[paraCount].name);
			itemId = mPropertyGridManager->AppendIn( id, CreateEffectSettingProperty(paraList[paraCount], value) );
			mPropertyGridManager->SetPropertyColourToDefault(itemId);
		}
	}	

	for(int i = 0;i<mEffect->getNumElements();i++)
	{
		Fairy::EffectElement* pElement = mEffect->getElement(i);
		wxPGId itemId;
		assert (pElement);
		wxPGId id = mPropertyGridManager->Append( new EffectObjectPropertyClass(pElement->getType().c_str(),wxT("EffectElement"),pElement) );

		if ( id.IsOk() )
		{
			mPropertyGridManager->DisableProperty(id);
			mPropertyGridManager->SetPropertyColour(id, *wxLIGHT_GREY);

			const Ogre::ParameterList &paraList = pElement->getParameters();
			for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
			{
				Ogre::String value = pElement->getParameter(paraList[paraCount].name);
				itemId = mPropertyGridManager->AppendIn( id, CreateEffectElementProperty(paraList[paraCount], value) );
				mPropertyGridManager->SetPropertyColourToDefault(itemId);
			}
		}
	}


	mPropertyGridManager->Thaw();
	mPropertyGridManager->ExpandAll();
	mPropertyGridManager->Refresh();
}
void EffectObjectPropertyEditor::InitEffectSettingProperty()
{
	mPropertyGridManager->Freeze();
	mPropertyGridManager->GetGrid()->Clear();

	wxPGId itemId;
	wxPGId id = mPropertyGridManager->Append( new EffectObjectPropertyClass(mEffectTemplateName.c_str(),wxT("EffectSetting"),mEffect) );

	if ( id.IsOk() )
	{
		mPropertyGridManager->DisableProperty(id);
		mPropertyGridManager->SetPropertyColour(id, *wxLIGHT_GREY);

		const Ogre::ParameterList &paraList = mEffect->getParameters();
		for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
		{
			Ogre::String value = mEffect->getParameter(paraList[paraCount].name);
			itemId = mPropertyGridManager->AppendIn( id, CreateEffectSettingProperty(paraList[paraCount], value) );
			mPropertyGridManager->SetPropertyColourToDefault(itemId);
		}
	}		
	mPropertyGridManager->Thaw();
	mPropertyGridManager->ExpandAll();
	mPropertyGridManager->Refresh();

}
void EffectObjectPropertyEditor::InitEffectElementProperty(Fairy::EffectElement* pElement)
{
	mPropertyGridManager->Freeze();
	mPropertyGridManager->GetGrid()->Clear();

	wxPGId itemId;
	assert (pElement);
	wxPGId id = mPropertyGridManager->Append( new EffectObjectPropertyClass(pElement->getType().c_str(),wxT("EffectElement"),pElement) );

	if ( id.IsOk() )
	{
		mPropertyGridManager->DisableProperty(id);
		mPropertyGridManager->SetPropertyColour(id, *wxLIGHT_GREY);

		const Ogre::ParameterList &paraList = pElement->getParameters();
		for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
		{
			Ogre::String value = pElement->getParameter(paraList[paraCount].name);
			itemId = mPropertyGridManager->AppendIn( id, CreateEffectElementProperty(paraList[paraCount], value) );
			mPropertyGridManager->SetPropertyColourToDefault(itemId);
		}
	}		
	mPropertyGridManager->Thaw();
	mPropertyGridManager->ExpandAll();
	mPropertyGridManager->Refresh();
}


void EffectObjectPropertyEditor::InitSkillProperty()
{

}
void EffectObjectPropertyEditor::InitSkillSettingProperty()
{
	mSkill = Fairy::EffectManager::getSingleton().getSkill(mSkillTemplateName);
	mPropertyGridManager->Freeze();
	mPropertyGridManager->GetGrid()->Clear();

	wxPGId itemId;
	wxPGId id = mPropertyGridManager->Append( new EffectObjectPropertyClass(mSkillTemplateName.c_str(),wxT("SkillSetting"),mSkill) );

	if ( id.IsOk() )
	{
		mPropertyGridManager->DisableProperty(id);
		mPropertyGridManager->SetPropertyColour(id, *wxLIGHT_GREY);

		const Ogre::ParameterList &paraList = mSkill->getParameters();
		for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
		{
			Ogre::String value = mSkill->getParameter(paraList[paraCount].name);
			itemId = mPropertyGridManager->AppendIn( id, CreateSkillSettingProperty(paraList[paraCount], value) );
			mPropertyGridManager->SetPropertyColourToDefault(itemId);
		}
	}		
	mPropertyGridManager->Thaw();
	mPropertyGridManager->ExpandAll();
	mPropertyGridManager->Refresh();

}
void EffectObjectPropertyEditor::InitSkillElementProperty(Ogre::StringInterface* pElement)
{
	mSkill = Fairy::EffectManager::getSingleton().getSkill(mSkillTemplateName);
	mPropertyGridManager->Freeze();
	mPropertyGridManager->GetGrid()->Clear();

	Ogre::String name = "";
	if(dynamic_cast<Fairy::AnimationEffectInfo*>(pElement))
		name = "EffectInfo";
	else if(dynamic_cast<Fairy::AnimationRibbon*>(pElement))
		name = "Ribbon";
	else if(dynamic_cast<Fairy::AnimationCameraShake*>(pElement))
		name = "CameraShake";
	else if(dynamic_cast<Fairy::AnimationSceneLightInfo*>(pElement))
		name = "Light";
	else if(dynamic_cast<Fairy::AnimationSound*>(pElement))
		name = "Sound";
	else if(dynamic_cast<Fairy::AnimationBulletFlow*>(pElement))
		name = "BulletFlow";
	wxPGId itemId;
	wxPGId id = mPropertyGridManager->Append( new EffectObjectPropertyClass(name,wxT("SkillElement"),pElement) );

	if ( id.IsOk() )
	{
		mPropertyGridManager->DisableProperty(id);
		mPropertyGridManager->SetPropertyColour(id, *wxLIGHT_GREY);

		const Ogre::ParameterList &paraList = pElement->getParameters();
		for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
		{
			Ogre::String value = pElement->getParameter(paraList[paraCount].name);
			itemId = mPropertyGridManager->AppendIn( id, CreateSkillElementProperty(paraList[paraCount], value) );
			mPropertyGridManager->SetPropertyColourToDefault(itemId);
		}
	}		
	mPropertyGridManager->Thaw();
	mPropertyGridManager->ExpandAll();
	mPropertyGridManager->Refresh();

}

void EffectObjectPropertyEditor::InitBulletFlowProperty()
{
	mPropertyGridManager->Freeze();
	mPropertyGridManager->GetGrid()->Clear();

	wxPGId itemId;
	wxPGId id = mPropertyGridManager->Append( new EffectObjectPropertyClass(mBulletFlowTemplateName.c_str(),wxT("BulletFlowSetting"),mBulletFlow) );

	if ( id.IsOk() )
	{
		mPropertyGridManager->DisableProperty(id);
		mPropertyGridManager->SetPropertyColour(id, *wxLIGHT_GREY);

		const Ogre::ParameterList &paraList = mBulletFlow->getParameters();
		for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
		{
			Ogre::String value = mBulletFlow->getParameter(paraList[paraCount].name);
			itemId = mPropertyGridManager->AppendIn( id, CreateBulletFlowSettingProperty(paraList[paraCount], value) );
			mPropertyGridManager->SetPropertyColourToDefault(itemId);
		}
	}		
	mPropertyGridManager->Thaw();
	mPropertyGridManager->ExpandAll();
	mPropertyGridManager->Refresh();

}
void EffectObjectPropertyEditor::InitBulletOperator(Fairy::BulletOperator* pOperator)
{
	mPropertyGridManager->Freeze();
	mPropertyGridManager->GetGrid()->Clear();

	wxPGId itemId;
	wxPGId id = mPropertyGridManager->Append( new EffectObjectPropertyClass(pOperator->getType().c_str(),wxT("BulletOperator"),pOperator) );

	if ( id.IsOk() )
	{
		mPropertyGridManager->DisableProperty(id);
		mPropertyGridManager->SetPropertyColour(id, *wxLIGHT_GREY);

		const Ogre::ParameterList &paraList = pOperator->getParameters();
		for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
		{
			Ogre::String value = pOperator->getParameter(paraList[paraCount].name);
			itemId = mPropertyGridManager->AppendIn( id, CreateBulletOperatorProperty(pOperator,paraList[paraCount], value) );
			mPropertyGridManager->SetPropertyColourToDefault(itemId);
		}
	}		
	mPropertyGridManager->Thaw();
	mPropertyGridManager->ExpandAll();
	mPropertyGridManager->Refresh();

}
void EffectObjectPropertyEditor::InitBulletSystemElement(Ogre::StringInterface* pElement)
{
	mPropertyGridManager->Freeze();
	mPropertyGridManager->GetGrid()->Clear();

	Ogre::String name = "";
	if(dynamic_cast<Fairy::AnimationEffectInfo*>(pElement))
		name = "EffectInfo";
	else if(dynamic_cast<Fairy::AnimationRibbon*>(pElement))
		name = "Ribbon";
	else if(dynamic_cast<Fairy::AnimationCameraShake*>(pElement))
		name = "CameraShake";
	else if(dynamic_cast<Fairy::AnimationSceneLightInfo*>(pElement))
		name = "Light";
	else if(dynamic_cast<Fairy::AnimationSound*>(pElement))
		name = "Sound";

	wxPGId itemId;
	wxPGId id = mPropertyGridManager->Append( new EffectObjectPropertyClass(name,wxT("BulletElement"),pElement) );

	if ( id.IsOk() )
	{
		mPropertyGridManager->DisableProperty(id);
		mPropertyGridManager->SetPropertyColour(id, *wxLIGHT_GREY);

		const Ogre::ParameterList &paraList = pElement->getParameters();
		for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
		{
			Ogre::String value = pElement->getParameter(paraList[paraCount].name);
			itemId = mPropertyGridManager->AppendIn( id, CreateBulletSystemElementProperty(paraList[paraCount], value) );
			mPropertyGridManager->SetPropertyColourToDefault(itemId);
		}
	}		
	mPropertyGridManager->Thaw();
	mPropertyGridManager->ExpandAll();
	mPropertyGridManager->Refresh();

}

void EffectObjectPropertyEditor::InitMaterialProperty()
{
	if(mMaterial.isNull())
		return;
	mPropertyGridManager->Freeze();
	mPropertyGridManager->GetGrid()->Clear();

	wxPGId itemId;
	wxPGId id = mPropertyGridManager->Append( new EffectObjectPropertyClass(mMaterial->getName(),wxT("Material"),mMaterial.get()) );

	if ( id.IsOk() )
	{
		wxPGProperty* property = NULL;
		mPropertyGridManager->DisableProperty(id);
		mPropertyGridManager->SetPropertyColour(id, *wxLIGHT_GREY);

		
		itemId = mPropertyGridManager->AppendIn(id,wxStringProperty(wxT("name"), wxT("name"), mMaterial->getName().c_str()));
		mPropertyGridManager->SetPropertyColourToDefault(itemId);

		bool receive = mMaterial->getReceiveShadows();
		itemId = mPropertyGridManager->AppendIn(id,CreateBoolOnOffProperty(Ogre::String("receive_shadows"),receive));
		mPropertyGridManager->SetPropertyColourToDefault(itemId);

		bool casts = mMaterial->getTransparencyCastsShadows();
		itemId = mPropertyGridManager->AppendIn(id,CreateBoolOnOffProperty(Ogre::String("transparency_casts_shadows"),casts));
		mPropertyGridManager->SetPropertyColourToDefault(itemId);

	}		
	mPropertyGridManager->Thaw();
	mPropertyGridManager->ExpandAll();
	mPropertyGridManager->Refresh();

}
void EffectObjectPropertyEditor::InitMaterialTechnique(Ogre::Technique* technique)
{
	if(mMaterial.isNull())
		return;
	mPropertyGridManager->Freeze();
	mPropertyGridManager->GetGrid()->Clear();

	wxPGId itemId;
	wxPGId id = mPropertyGridManager->Append( new EffectObjectPropertyClass(wxT("technique"),wxT("technique"),technique) );

	if ( id.IsOk() )
	{
		wxPGProperty* property = NULL;
		mPropertyGridManager->DisableProperty(id);
		mPropertyGridManager->SetPropertyColour(id, *wxLIGHT_GREY);

		Ogre::String nameValue = technique->getName();
		itemId = mPropertyGridManager->AppendIn( id, wxStringProperty(wxT("name"), wxT("name"), nameValue.c_str()));
		mPropertyGridManager->SetPropertyColourToDefault(itemId);

		Ogre::String lodValue = Ogre::StringConverter::toString(technique->getLodIndex());
		itemId = mPropertyGridManager->AppendIn( id, wxStringProperty(wxT("lod_index"), wxT("lod_index"), lodValue.c_str()));
		mPropertyGridManager->SetPropertyColourToDefault(itemId);

		Ogre::String schemeValue = technique->getSchemeName();
		itemId = mPropertyGridManager->AppendIn( id, wxStringProperty(wxT("scheme"), wxT("scheme"), schemeValue.c_str()));
		mPropertyGridManager->SetPropertyColourToDefault(itemId);

	}		
	mPropertyGridManager->Thaw();
	mPropertyGridManager->ExpandAll();
	mPropertyGridManager->Refresh();

}
void EffectObjectPropertyEditor::InitMaterialPass(Ogre::Pass* pass)
{
	if(mMaterial.isNull())
		return;
	mPropertyGridManager->Freeze();
	mPropertyGridManager->GetGrid()->Clear();

	wxPGId itemId;
	wxPGId id = mPropertyGridManager->Append( new EffectObjectPropertyClass(wxT("pass"),wxT("pass"),pass) );

	if ( id.IsOk() )
	{
		mPropertyGridManager->DisableProperty(id);
		mPropertyGridManager->SetPropertyColour(id, *wxLIGHT_GREY);

		Ogre::String pass_nameName = pass->getName();
		itemId = mPropertyGridManager->AppendIn(id,wxStringProperty(wxT("name"), wxT("name"), pass_nameName.c_str()));
		mPropertyGridManager->SetPropertyColourToDefault(itemId);

		Ogre::ColourValue ambientValue = pass->getAmbient();
		itemId = mPropertyGridManager->AppendIn(id,wxFairyColourProperty(wxT("ambient"), wxT("ambient"), 
			colorConverter(Ogre::StringConverter::toString(ambientValue).c_str())));
		mPropertyGridManager->SetPropertyColourToDefault(itemId);

		Ogre::ColourValue diffuseValue = pass->getDiffuse();
		itemId = mPropertyGridManager->AppendIn(id,wxFairyColourProperty(wxT("diffuse"), wxT("diffuse"),
			colorConverter(Ogre::StringConverter::toString(diffuseValue).c_str())));
		mPropertyGridManager->SetPropertyColourToDefault(itemId);

		Ogre::ColourValue specularValue = pass->getSpecular();
		itemId = mPropertyGridManager->AppendIn(id,wxFairyColourProperty(wxT("specular"), wxT("specular"),
			colorConverter(Ogre::StringConverter::toString(specularValue).c_str())));
		mPropertyGridManager->SetPropertyColourToDefault(itemId);

		Ogre::ColourValue emissiveValue = pass->getSpecular();
		itemId = mPropertyGridManager->AppendIn(id,wxFairyColourProperty(wxT("emissive"), wxT("emissive"),
			colorConverter(Ogre::StringConverter::toString(emissiveValue).c_str())));
		mPropertyGridManager->SetPropertyColourToDefault(itemId);

		//场景混合
		Ogre::SceneBlendFactor SourceBlendFactor = pass->getSourceBlendFactor();
		Ogre::SceneBlendFactor DestBlendFactor = pass->getDestBlendFactor();
		Ogre::String blendName = getSceneBlendName(SourceBlendFactor,DestBlendFactor);
		wxPGConstants SceneBlendConstants;
		SceneBlendConstants.AddString("transparent_alpha");
		SceneBlendConstants.AddString("transparent_color");
		SceneBlendConstants.AddString("modulate");
		SceneBlendConstants.AddString("add");
		SceneBlendConstants.AddString("replace");
		SceneBlendConstants.AddString("null");

		wxPGProperty* scene_blend_property = wxEnumProperty(wxT("scene_blend"), wxT("scene_blend"), SceneBlendConstants);
		scene_blend_property->SetValueFromString(blendName.c_str(),0);
		itemId = mPropertyGridManager->AppendIn(id,scene_blend_property);
		mPropertyGridManager->SetPropertyColourToDefault(itemId);

		bool check = pass->getDepthCheckEnabled();
		itemId = mPropertyGridManager->AppendIn(id,CreateBoolProperty(Ogre::String("depth_check"),check));
		mPropertyGridManager->SetPropertyColourToDefault(itemId);

		bool depth_write = pass->getDepthWriteEnabled();
		itemId = mPropertyGridManager->AppendIn(id,CreateBoolProperty(Ogre::String("depth_write"), depth_write));
		mPropertyGridManager->SetPropertyColourToDefault(itemId);

		Ogre::CompareFunction function = pass->getDepthFunction();
		Ogre::String functionName = getCompareFunctionName(function);
		wxPGConstants functionConstants;
		functionConstants.AddString("always_fail");
		functionConstants.AddString("always_pass");
		functionConstants.AddString("less");
		functionConstants.AddString("less_equal");
		functionConstants.AddString("equal");
		functionConstants.AddString("not_equal");
		functionConstants.AddString("greater_equal");
		functionConstants.AddString("greater");

		wxPGProperty* function_property = wxEnumProperty(wxT("depth_func"), wxT("depth_func"), functionConstants);
		function_property->SetValueFromString(functionName.c_str(),0);
		itemId = mPropertyGridManager->AppendIn(id,function_property);
		mPropertyGridManager->SetPropertyColourToDefault(itemId);


		bool color_write = pass->getColourWriteEnabled();
		itemId = mPropertyGridManager->AppendIn(id,CreateBoolProperty(Ogre::String("colour_write"),color_write));
		mPropertyGridManager->SetPropertyColourToDefault(itemId);

		Ogre::CullingMode cullingMode =  pass->getCullingMode();
		Ogre::String cullingModeName = getCullingModeName(cullingMode);
		wxPGConstants cullingModeConstants;
		cullingModeConstants.AddString("none");
		cullingModeConstants.AddString("clockwise");
		cullingModeConstants.AddString("anticlockwise");
		wxPGProperty* cullingMode_property = wxEnumProperty(wxT("cull_hardware"), wxT("cull_hardware"), cullingModeConstants);
		cullingMode_property->SetValueFromString(cullingModeName.c_str(),0);
		itemId = mPropertyGridManager->AppendIn(id,cullingMode_property);
		mPropertyGridManager->SetPropertyColourToDefault(itemId);


		Ogre::ManualCullingMode maualCullingMode =  pass->getManualCullingMode();
		Ogre::String maualCullingName = getManualCullingModeName(maualCullingMode);
		wxPGConstants maualCullingConstants;
		maualCullingConstants.AddString("none");
		maualCullingConstants.AddString("back");
		maualCullingConstants.AddString("front");
		wxPGProperty* maualCulling_property = wxEnumProperty(wxT("cull_software"), wxT("cull_software"), maualCullingConstants);
		maualCulling_property->SetValueFromString(maualCullingName.c_str(),0);
		itemId = mPropertyGridManager->AppendIn(id,maualCulling_property);
		mPropertyGridManager->SetPropertyColourToDefault(itemId);

		bool light = pass->getLightingEnabled();
		itemId = mPropertyGridManager->AppendIn(id,CreateBoolProperty(Ogre::String("lighting"),light));
		mPropertyGridManager->SetPropertyColourToDefault(itemId);


		Ogre::ShadeOptions shaderOpt = pass->getShadingMode();
		Ogre::String shadingName = getShadingModeName(shaderOpt);
		wxPGConstants shadingConstants;
		shadingConstants.AddString("flat");
		shadingConstants.AddString("gouraud");
		shadingConstants.AddString("phong");
		wxPGProperty* shading_property = wxEnumProperty(wxT("shading"), wxT("shading"), shadingConstants);
		shading_property->SetValueFromString(shadingName.c_str(),0);
		itemId = mPropertyGridManager->AppendIn(id,shading_property);
		mPropertyGridManager->SetPropertyColourToDefault(itemId);


		Ogre::PolygonMode polygonMode = pass->getPolygonMode();
		Ogre::String polygonModeName = getPolygonModeName(polygonMode);
		wxPGConstants polygonModeConstants;
		polygonModeConstants.AddString("points");
		polygonModeConstants.AddString("wireframe");
		polygonModeConstants.AddString("solid");
		wxPGProperty* polygonMode_property = wxEnumProperty(wxT("polygon_mode"), wxT("polygon_mode"), polygonModeConstants);
		polygonMode_property->SetValueFromString(polygonModeName.c_str(),0);
		itemId = mPropertyGridManager->AppendIn(id,polygonMode_property);
		mPropertyGridManager->SetPropertyColourToDefault(itemId);

		Ogre::Vector2 vDepthBias = Ogre::Vector2::ZERO;
		vDepthBias.x = pass->getDepthBiasConstant();
		vDepthBias.y = pass->getDepthBiasSlopeScale();
		Ogre::String depthBiasName = Ogre::StringConverter::toString(vDepthBias);
		itemId = mPropertyGridManager->AppendIn(id,wxStringProperty(wxT("depth_bias"), wxT("depth_bias"), depthBiasName.c_str()));
		mPropertyGridManager->SetPropertyColourToDefault(itemId);

		//bool fogOverride = pass->getFogOverride();
		//itemId = mPropertyGridManager->AppendIn(id,CreateBoolProperty(Ogre::String("fog_override"),fogOverride));
		//mPropertyGridManager->SetPropertyColourToDefault(itemId);

		Ogre::String fogOverrideName;
		bool fogOverride = pass->getFogOverride();
		fogOverrideName = Ogre::StringConverter::toString(fogOverride);
		fogOverrideName += " ";
		Ogre::FogMode fogMode = pass->getFogMode();
		fogOverrideName += getFogModeName(fogMode);
		Ogre::ColourValue fogColor = pass->getFogColour();
		fogOverrideName += " ";
		fogOverrideName += Ogre::StringConverter::toString(fogColor);
		fogOverrideName += " ";
		fogOverrideName += Ogre::StringConverter::toString(pass->getFogDensity());
		fogOverrideName += " ";
		fogOverrideName += Ogre::StringConverter::toString(pass->getFogStart());
		fogOverrideName += " ";
		fogOverrideName += Ogre::StringConverter::toString(pass->getFogEnd());

		itemId = mPropertyGridManager->AppendIn(id,wxStringProperty(wxT("fog_override"), wxT("fog_override"), fogOverrideName.c_str()));
		mPropertyGridManager->SetPropertyColourToDefault(itemId);


		Ogre::String maxLightName = Ogre::StringConverter::toString(pass->getMaxSimultaneousLights());
		itemId = mPropertyGridManager->AppendIn(id,wxStringProperty(wxT("max_lights"), wxT("max_lights"), maxLightName.c_str()));
		mPropertyGridManager->SetPropertyColourToDefault(itemId);

		/*Ogre::Light::LightTypes  lightType = pass->getOnlyLightType();
		Ogre::String onlyLightTypeName = getLightTypeName(lightType);
		wxPGConstants lightTypeConstants;
		lightTypeConstants.AddString("point");
		lightTypeConstants.AddString("directional");
		lightTypeConstants.AddString("spot");
		wxPGProperty* onlyLightType_property = wxEnumProperty(wxT("only_light_type"), wxT("only_light_type"), lightTypeConstants);
		onlyLightType_property->SetValueFromString(onlyLightTypeName.c_str(),0);
		itemId = mPropertyGridManager->AppendIn(id,onlyLightType_property);
		mPropertyGridManager->SetPropertyColourToDefault(itemId);*/

		Ogre::String pointSizeName = Ogre::StringConverter::toString(pass->getPointSize());
		itemId = mPropertyGridManager->AppendIn(id,wxStringProperty(wxT("point_size"), wxT("point_size"), pointSizeName.c_str()));
		mPropertyGridManager->SetPropertyColourToDefault(itemId);

		bool sprites = pass->getPointSpritesEnabled();
		itemId = mPropertyGridManager->AppendIn(id,CreateBoolProperty(Ogre::String("point_sprites"),sprites));
		mPropertyGridManager->SetPropertyColourToDefault(itemId);

		Ogre::String pointSizeAttenName = pass->isPointAttenuationEnabled() ? "on" : "off";
		pointSizeAttenName += " ";
		pointSizeAttenName += Ogre::StringConverter::toString(pass->getPointAttenuationConstant());
		pointSizeAttenName += " ";
		pointSizeAttenName += Ogre::StringConverter::toString(pass->getPointAttenuationLinear());
		pointSizeAttenName += " ";
		pointSizeAttenName += Ogre::StringConverter::toString(pass->getPointAttenuationQuadratic());

		itemId = mPropertyGridManager->AppendIn(id,wxStringProperty(wxT("point_size_attenuation"), wxT("point_size_attenuation"), pointSizeAttenName.c_str()));
		mPropertyGridManager->SetPropertyColourToDefault(itemId);


		Ogre::String pointMinName = Ogre::StringConverter::toString(pass->getPointMinSize());
		itemId = mPropertyGridManager->AppendIn(id,wxStringProperty(wxT("point_size_min"), wxT("point_size_min"), pointMinName.c_str()));
		mPropertyGridManager->SetPropertyColourToDefault(itemId);

		Ogre::String pointMaxName = Ogre::StringConverter::toString(pass->getPointMaxSize());
		itemId = mPropertyGridManager->AppendIn(id,wxStringProperty(wxT("point_size_max"), wxT("point_size_max"), pointMaxName.c_str()));
		mPropertyGridManager->SetPropertyColourToDefault(itemId);

	}		
	mPropertyGridManager->Thaw();
	mPropertyGridManager->ExpandAll();
	mPropertyGridManager->Refresh();

}
void EffectObjectPropertyEditor::InitMaterialTextureUnit(Ogre::TextureUnitState* textureUnit)
{
	if(mMaterial.isNull())
		return;
	mPropertyGridManager->Freeze();
	mPropertyGridManager->GetGrid()->Clear();

	wxPGId itemId;
	wxPGId id = mPropertyGridManager->Append( new EffectObjectPropertyClass(wxT("texture_unit"),wxT("texture_unit"),textureUnit) );

	if ( id.IsOk() )
	{
		mPropertyGridManager->DisableProperty(id);
		mPropertyGridManager->SetPropertyColour(id, *wxLIGHT_GREY);

		itemId = mPropertyGridManager->AppendIn(id,wxStringProperty(wxT("name"), wxT("name"), textureUnit->getName().c_str()));
		mPropertyGridManager->SetPropertyColourToDefault(itemId);

		itemId = mPropertyGridManager->AppendIn(id,wxStringProperty(wxT("texture"), wxT("texture"), textureUnit->getTextureName().c_str()));
		mPropertyGridManager->SetPropertyColourToDefault(itemId);

		itemId = mPropertyGridManager->AppendIn(id,wxStringProperty(wxT("texture_alias"), wxT("texture_alias"), textureUnit->getTextureNameAlias().c_str()));
		mPropertyGridManager->SetPropertyColourToDefault(itemId);

		//贴图
		Ogre::String animTextureName;
		int iFramsNum = textureUnit->getNumFrames();
		if(iFramsNum > 1)
		{
			animTextureName = textureUnit->getFrameTextureName(0);
			animTextureName += " ";
			animTextureName += Ogre::StringConverter::toString(iFramsNum).c_str();
			animTextureName += " ";
			animTextureName += Ogre::StringConverter::toString(textureUnit->getAnimationDuration()).c_str();

		}
		else
		{
			animTextureName = "off";
		}
		itemId = mPropertyGridManager->AppendIn(id,wxStringProperty(wxT("anim_texture"), wxT("anim_texture"), animTextureName.c_str()));
		mPropertyGridManager->SetPropertyColourToDefault(itemId);

		Ogre::String coordSetName = Ogre::StringConverter::toString(textureUnit->getTextureCoordSet());
		itemId = mPropertyGridManager->AppendIn(id,wxStringProperty(wxT("tex_coord_set"), wxT("tex_coord_set"), coordSetName.c_str()));
		mPropertyGridManager->SetPropertyColourToDefault(itemId);

		Ogre::ColourValue borderValue = textureUnit->getTextureBorderColour();
		itemId = mPropertyGridManager->AppendIn(id,wxFairyColourProperty(wxT("tex_border_colour"), wxT("tex_border_colour"), 
			colorConverter(Ogre::StringConverter::toString(borderValue).c_str())));
		mPropertyGridManager->SetPropertyColourToDefault(itemId);

		//贴图地址模式
	
		Ogre::TextureUnitState::UVWAddressingMode addressingMode = 
			textureUnit->getTextureAddressingMode();
		Ogre::String addressMode_uName = getAddressingName(addressingMode.u);
		Ogre::String addressMode_vName = getAddressingName(addressingMode.v);
		Ogre::String addressMode_wName = getAddressingName(addressingMode.w);

		wxPGConstants address_uModeConstants;
		address_uModeConstants.AddString("warp");
		address_uModeConstants.AddString("mirror");
		address_uModeConstants.AddString("clamp");
		address_uModeConstants.AddString("border");

		wxPGProperty* addressMode_u_property = wxEnumProperty(wxT("tex_address_mode_u"), wxT("tex_address_mode_u"), address_uModeConstants);
		addressMode_u_property->SetValueFromString(addressMode_uName.c_str(),0);
		itemId = mPropertyGridManager->AppendIn(id,addressMode_u_property);
		mPropertyGridManager->SetPropertyColourToDefault(itemId);

		wxPGConstants address_vModeConstants;
		address_vModeConstants.AddString("warp");
		address_vModeConstants.AddString("mirror");
		address_vModeConstants.AddString("clamp");
		address_vModeConstants.AddString("border");

		wxPGProperty* addressMode_v_property = wxEnumProperty(wxT("tex_address_mode_v"), wxT("tex_address_mode_v"), address_vModeConstants);
		addressMode_v_property->SetValueFromString(addressMode_vName.c_str(),0);
		itemId = mPropertyGridManager->AppendIn(id,addressMode_v_property);
		mPropertyGridManager->SetPropertyColourToDefault(itemId);

		wxPGConstants address_wModeConstants;
		address_wModeConstants.AddString("warp");
		address_wModeConstants.AddString("mirror");
		address_wModeConstants.AddString("clamp");
		address_wModeConstants.AddString("border");

		wxPGProperty* addressMode_w_property = wxEnumProperty(wxT("tex_address_mode_w"), wxT("tex_address_mode_w"), address_wModeConstants);
		addressMode_w_property->SetValueFromString(addressMode_wName.c_str(),0);
		itemId = mPropertyGridManager->AppendIn(id,addressMode_w_property);
		mPropertyGridManager->SetPropertyColourToDefault(itemId);



		Ogre::FilterOptions minFiltering = textureUnit->getTextureFiltering(Ogre::FT_MIN);
		Ogre::String filtering_minName = getFilteringName(minFiltering);

		Ogre::FilterOptions magFiltering = textureUnit->getTextureFiltering(Ogre::FT_MAG);
		Ogre::String filtering_magName = getFilteringName(magFiltering);

		Ogre::FilterOptions mipFiltering = textureUnit->getTextureFiltering(Ogre::FT_MIP);
		Ogre::String filtering_mipName = getFilteringName(mipFiltering);

		wxPGConstants filteringConstants;
		filteringConstants.AddString("none");
		filteringConstants.AddString("point");
		filteringConstants.AddString("linear");
		filteringConstants.AddString("anisotropic");

		wxPGProperty* filtering_min_property = wxEnumProperty(wxT("filtering_min"), wxT("filtering_min"), filteringConstants);
		filtering_min_property->SetValueFromString(filtering_minName.c_str(),0);
		itemId = mPropertyGridManager->AppendIn(id,filtering_min_property);
		mPropertyGridManager->SetPropertyColourToDefault(itemId);

		wxPGProperty* filtering_mag_property = wxEnumProperty(wxT("filtering_mag"), wxT("filtering_mag"), filteringConstants);
		filtering_mag_property->SetValueFromString(filtering_magName.c_str(),0);
		itemId = mPropertyGridManager->AppendIn(id,filtering_mag_property);
		mPropertyGridManager->SetPropertyColourToDefault(itemId);

		wxPGProperty* filtering_mip_property = wxEnumProperty(wxT("filtering_mip"), wxT("filtering_mip"), filteringConstants);
		filtering_mip_property->SetValueFromString(filtering_mipName.c_str(),0);
		itemId = mPropertyGridManager->AppendIn(id,filtering_mip_property);
		mPropertyGridManager->SetPropertyColourToDefault(itemId);


		Ogre::String numMipmapsName = Ogre::StringConverter::toString(textureUnit->getNumMipmaps());
		itemId = mPropertyGridManager->AppendIn(id,wxStringProperty(wxT("mipmaps"), wxT("mipmaps"), numMipmapsName.c_str()));
		mPropertyGridManager->SetPropertyColourToDefault(itemId);


		//颜色操作模式
		Ogre::LayerBlendModeEx colourBlendMode = textureUnit->getColourBlendMode();

		Ogre::LayerBlendOperationEx operation = colourBlendMode.operation;
		Ogre::LayerBlendSource source1 = colourBlendMode.source1;
		Ogre::LayerBlendSource source2 = colourBlendMode.source2;
		Ogre::ColourValue colourArg1 = colourBlendMode.colourArg1;
		Ogre::ColourValue colourArg2 = colourBlendMode.colourArg2;
		Ogre::Real factor = colourBlendMode.factor;

		Ogre::SceneBlendFactor colourBlendFallbackSrc = textureUnit->getColourBlendFallbackSrc();
		Ogre::SceneBlendFactor colourBlendFallbackDest = textureUnit->getColourBlendFallbackDest();

		Ogre::String colour_opName;

		if(operation == Ogre::LBX_SOURCE1 && source1 ==Ogre::LBS_TEXTURE && source2 == Ogre::LBS_CURRENT
			&& colourBlendFallbackSrc == Ogre::SBF_ONE && colourBlendFallbackDest == Ogre::SBF_ZERO)
			colour_opName = "replace";
		else if(operation == Ogre::LBX_ADD && source1 ==Ogre::LBS_TEXTURE && source2 == Ogre::LBS_CURRENT
			&& colourBlendFallbackSrc == Ogre::SBF_ONE && colourBlendFallbackDest == Ogre::SBF_ONE)
			colour_opName = "add";
		else if(operation == Ogre::LBX_MODULATE && source1 ==Ogre::LBS_TEXTURE && source2 == Ogre::LBS_CURRENT
			&& colourBlendFallbackSrc == Ogre::SBF_DEST_COLOUR && colourBlendFallbackDest == Ogre::SBF_ZERO)
			colour_opName = "modulate";
		else if(operation == Ogre::LBX_BLEND_TEXTURE_ALPHA && source1 ==Ogre::LBS_TEXTURE && source2 == Ogre::LBS_CURRENT
			&& colourBlendFallbackSrc == Ogre::SBF_SOURCE_ALPHA && colourBlendFallbackDest == Ogre::SBF_ONE_MINUS_SOURCE_ALPHA)
			colour_opName = "alpha_blend";
		else
			colour_opName = "none";

		wxPGConstants colorOpConstants;
		colorOpConstants.AddString("replace");
		colorOpConstants.AddString("add");
		colorOpConstants.AddString("modulate");
		colorOpConstants.AddString("alpha_blend");

		wxPGProperty* color_op_property = wxEnumProperty(wxT("colour_op"), wxT("colour_op"), colorOpConstants);
		color_op_property->SetValueFromString(colour_opName.c_str(),0);
		itemId = mPropertyGridManager->AppendIn(id,color_op_property);
		mPropertyGridManager->SetPropertyColourToDefault(itemId);

		Ogre::String colour_op_exName;
		colour_op_exName = getLayerBlendOperationName(operation);
		colour_op_exName += " ";
		colour_op_exName += getLayerBlendName(source1);
		colour_op_exName += " ";
		colour_op_exName += getLayerBlendName(source2);
		colour_op_exName += "  ";
		colour_op_exName += Ogre::StringConverter::toString(colourArg1);
		colour_op_exName += "  ";
		colour_op_exName += Ogre::StringConverter::toString(colourArg2);
		colour_op_exName += " ";
		colour_op_exName += Ogre::StringConverter::toString(factor);

		itemId = mPropertyGridManager->AppendIn(id,wxStringProperty(wxT("colour_op_ex"), wxT("colour_op_ex"), colour_op_exName.c_str()));
		mPropertyGridManager->SetPropertyColourToDefault(itemId);

		//alpha操作
		Ogre::LayerBlendModeEx alphaBlendMode = textureUnit->getAlphaBlendMode();

		Ogre::LayerBlendOperationEx alphaOperation = alphaBlendMode.operation;
		Ogre::LayerBlendSource alphaSource1 = alphaBlendMode.source1;
		Ogre::LayerBlendSource alphaSource2 = alphaBlendMode.source2;
		Ogre::Real alphaArg1 = alphaBlendMode.alphaArg1;
		Ogre::Real alphaArg2 = alphaBlendMode.alphaArg2;
		Ogre::Real alphafactor = alphaBlendMode.factor;


		Ogre::String alpha_op_exName;
		alpha_op_exName =  getLayerBlendOperationName(alphaOperation);
		alpha_op_exName += " ";
		alpha_op_exName += getLayerBlendName(alphaSource1);
		alpha_op_exName += " ";
		alpha_op_exName += getLayerBlendName(alphaSource2);
		alpha_op_exName += " ";
		alpha_op_exName += Ogre::StringConverter::toString(alphaArg1<0.01 || alphaArg1> -0.01 ? 0.0f : alphaArg1);
		alpha_op_exName += " ";
		alpha_op_exName += Ogre::StringConverter::toString(alphaArg2<0.01 || alphaArg1> -0.01 ? 0.0f : alphaArg2);
		alpha_op_exName += " ";
		alpha_op_exName += Ogre::StringConverter::toString(alphafactor<0.01 || alphaArg1> -0.01 ? 0.0f : alphafactor);

		itemId = mPropertyGridManager->AppendIn(id,wxStringProperty(wxT("alpha_op_ex"), wxT("alpha_op_ex"), alpha_op_exName.c_str()));
		mPropertyGridManager->SetPropertyColourToDefault(itemId);


		Ogre::String UScrollName = Ogre::StringConverter::toString(textureUnit->getTextureUScroll());
		itemId = mPropertyGridManager->AppendIn(id,wxStringProperty(wxT("scroll_u"), wxT("scroll_u"), UScrollName.c_str()));
		mPropertyGridManager->SetPropertyColourToDefault(itemId);

		Ogre::String VScrollName = Ogre::StringConverter::toString(textureUnit->getTextureVScroll());
		itemId = mPropertyGridManager->AppendIn(id,wxStringProperty(wxT("scroll_v"), wxT("scroll_v"), VScrollName.c_str()));
		mPropertyGridManager->SetPropertyColourToDefault(itemId);

		Ogre::String RotateName = Ogre::StringConverter::toString(textureUnit->getTextureRotate().valueDegrees());
		itemId = mPropertyGridManager->AppendIn(id,wxStringProperty(wxT("rotate"), wxT("rotate"), RotateName.c_str()));
		mPropertyGridManager->SetPropertyColourToDefault(itemId);

		Ogre::String UScaleName = Ogre::StringConverter::toString(textureUnit->getTextureUScale());
		itemId = mPropertyGridManager->AppendIn(id,wxStringProperty(wxT("scale_u"), wxT("scale_u"), UScaleName.c_str()));
		mPropertyGridManager->SetPropertyColourToDefault(itemId);

		Ogre::String VScaleName = Ogre::StringConverter::toString(textureUnit->getTextureVScale());
		itemId = mPropertyGridManager->AppendIn(id,wxStringProperty(wxT("scale_v"), wxT("scale_v"), VScaleName.c_str()));
		mPropertyGridManager->SetPropertyColourToDefault(itemId);


		//贴图
		Ogre::TextureUnitState::EffectMap effectMap = textureUnit->getEffects();
		Ogre::TextureUnitState::EffectMap::iterator it;

		//环境贴图
		it = effectMap.find(Ogre::TextureUnitState::ET_ENVIRONMENT_MAP);
		int envMapType = -1;
		if (it != effectMap.end())
			envMapType = it->second.subtype;
		Ogre::String env_mapName;
		if(envMapType == -1)
			env_mapName = "off";
		else
			env_mapName = getEnvironmentTypeName((Ogre::TextureUnitState::EnvMapType)envMapType);

		wxPGConstants envConstants;
		envConstants.AddString("off");
		envConstants.AddString("planar");
		envConstants.AddString("curved");
		envConstants.AddString("reflection");
		envConstants.AddString("normal");

		wxPGProperty* env_map_property = wxEnumProperty(wxT("env_map"), wxT("env_map"), envConstants);
		env_map_property->SetValueFromString(env_mapName.c_str(),0);
		itemId = mPropertyGridManager->AppendIn(id,env_map_property);
		mPropertyGridManager->SetPropertyColourToDefault(itemId);



		//滚动动画
		float fUSpeed = 0.0f;
		float fVSpeed = 0.0f;
		it = effectMap.find(Ogre::TextureUnitState::ET_UVSCROLL);
		if (it != effectMap.end())
		{
			fUSpeed = fVSpeed = it->second.arg1;
		}

		it = effectMap.find(Ogre::TextureUnitState::ET_USCROLL);
		if (it != effectMap.end())
		{
			fUSpeed = it->second.arg1;
		}

		it = effectMap.find(Ogre::TextureUnitState::ET_VSCROLL);
		if (it != effectMap.end())
		{
			fVSpeed = it->second.arg1;
		}
		Ogre:: Vector3 vSpeed = Ogre:: Vector3(fUSpeed,fVSpeed,0);
		Ogre::String scrollAnim_Name = Ogre::StringConverter::toString(vSpeed);
		itemId = mPropertyGridManager->AppendIn(id,wxStringProperty(wxT("scroll_anim"), wxT("scroll_anim"), scrollAnim_Name.c_str()));
		mPropertyGridManager->SetPropertyColourToDefault(itemId);
	

		//旋转动画
		float fRotateSpeed = 0.0f;
		it = effectMap.find(Ogre::TextureUnitState::ET_ROTATE);
		if (it != effectMap.end())
		{
			fRotateSpeed = it->second.arg1;
		}

		Ogre::String rotateName = Ogre::StringConverter::toString(fRotateSpeed);
		itemId = mPropertyGridManager->AppendIn(id,wxStringProperty(wxT("rotate_anim"), wxT("rotate_anim"), rotateName.c_str()));
		mPropertyGridManager->SetPropertyColourToDefault(itemId);

		//wave_xform动画
		Ogre::String  wave_xformName;
		it = effectMap.find(Ogre::TextureUnitState::ET_TRANSFORM);
		if (it != effectMap.end())
		{
			int ttype = it->second.subtype;
			wave_xformName = getTransformTypeName((Ogre::TextureUnitState::TextureTransformType)ttype);
			wave_xformName += " ";
			int waveType = it->second.waveType;
			wave_xformName += getWaveformTypeName((Ogre::WaveformType)waveType);
			wave_xformName += " ";
			wave_xformName += Ogre::StringConverter::toString( it->second.base);
			wave_xformName += " ";
			wave_xformName += Ogre::StringConverter::toString( it->second.frequency);
			wave_xformName += " ";
			wave_xformName += Ogre::StringConverter::toString( it->second.phase);
			wave_xformName += " ";
			wave_xformName += Ogre::StringConverter::toString( it->second.amplitude);

		}
		else
		{
			wave_xformName = "off";
		}

		itemId = mPropertyGridManager->AppendIn(id,wxStringProperty(wxT("wave_xform"), wxT("wave_xform"), wave_xformName.c_str()));
		mPropertyGridManager->SetPropertyColourToDefault(itemId);

		//贴图转换矩阵
		Ogre::String transformName = Ogre::StringConverter::toString(textureUnit->getTextureTransform());
		itemId = mPropertyGridManager->AppendIn(id,wxStringProperty(wxT("transform"), wxT("transform"), transformName.c_str()));
		mPropertyGridManager->SetPropertyColourToDefault(itemId);


	}
	mPropertyGridManager->Thaw();
	mPropertyGridManager->ExpandAll();
	mPropertyGridManager->Refresh();


}
wxPGProperty *EffectObjectPropertyEditor::CreateBoolProperty(Ogre::String& name,bool value)
{
	wxPGProperty* property = NULL;
	wxPGConstants* constants = wxPropertyGrid::CreateConstantsArray(2);		

	constants->AddString("true");
	constants->AddString("false");

	registerConstants(constants);
	if (constants)
	{
		// 创建出combobox
		property = wxEnumProperty(name.c_str(), name.c_str(), *constants);
		// 设置成当前这个参数的值
		property->SetValueFromString(value ? wxT("true") : wxT("false"),0);
	}
	return property;

}
wxPGProperty *EffectObjectPropertyEditor::CreateBoolOnOffProperty(Ogre::String& name,bool value)
{
	wxPGProperty* property = NULL;
	wxPGConstants* constants = wxPropertyGrid::CreateConstantsArray(2);		

	constants->AddString("on");
	constants->AddString("off");

	registerConstants(constants);
	if (constants)
	{
		// 创建出combobox
		property = wxEnumProperty(name.c_str(), name.c_str(), *constants);
		// 设置成当前这个参数的值
		property->SetValueFromString(value ? wxT("on") : wxT("off"),0);
	}
	return property;
}

wxPGProperty*
EffectObjectPropertyEditor::CreateProperty(const Ogre::ParameterDef& propertyDef, Ogre::String &value)
{
	wxPGProperty* property = NULL;
	wxString name = propertyDef.name.c_str();
	Ogre::ParameterType paraType = propertyDef.paramType;

	switch ( paraType )
	{
	case Ogre::PT_COLOURVALUE:
		property = wxFairyColourProperty(name, name, colorConverter(value));
		break;

	case Ogre::PT_STRING:
		{

			property = wxStringProperty(name, name, value.c_str());

			break;
		}

	case Ogre::PT_REAL:
		{
			property = wxFloatProperty(name, name, Ogre::StringConverter::parseReal(value));

			break;
		}

	case Ogre::PT_BOOL:
		{
			size_t id = reinterpret_cast<size_t>(&name);
			wxPGConstants* constants = wxPropertyGrid::GetConstantsArray(id);

			if (!constants)
			{
				constants = wxPropertyGrid::CreateConstantsArray(id);		

				constants->Add("true");
				constants->Add("false");

				registerConstants(constants);
			}

			if (constants)
			{
				// 创建出combobox
				property = wxEnumProperty(name, name, *constants);
				// 设置成当前这个参数的值
				property->SetValueFromString(value.c_str(),0);
			}
			break;
		}

	default:
		property = wxStringProperty(name, name, value.c_str());
		break;
	}

	assert (property);

	return property;
}


wxPGProperty * 
EffectObjectPropertyEditor::CreateParticleSettingProperty(const Ogre::ParameterDef& propertyDef, Ogre::String &value)
{
	wxPGProperty* property = NULL;
	wxString name = propertyDef.name.c_str();
	Ogre::ParameterType paraType = propertyDef.paramType;

	switch ( paraType )
	{
	case Ogre::PT_COLOURVALUE:
		property = wxFairyColourProperty(name, name, colorConverter(value));
		break;

	case Ogre::PT_STRING:
		{
			if (propertyDef.name == "renderer")
			{
					
				int iRenderCount = 0;
				wxPGConstants constants;
				Ogre::ParticleSystemManager::ParticleRendererFactoryIterator it = 
					Ogre::ParticleSystemManager::getSingleton().getRendererFactoryIterator();
				
				while(it.hasMoreElements())
				{
					Ogre::ParticleSystemRendererFactory* pRenderFactory = it.getNext();
					constants.AddString(wxT(pRenderFactory->getType().c_str()));
					iRenderCount++;
				}

				// 创建出combobox
				property = wxEnumProperty(name, name,constants,iRenderCount);
				// 设置成当前这个参数的值
				property->SetValueFromString(value.c_str(),0);
					
			}			
			else
				property = wxStringProperty(name, name, value.c_str());

			break;
		}

	case Ogre::PT_REAL:
		{
			property = wxFloatProperty(name, name, Ogre::StringConverter::parseReal(value));

			break;
		}

	case Ogre::PT_BOOL:
		{
			size_t id = reinterpret_cast<size_t>(&name);
			wxPGConstants* constants = wxPropertyGrid::GetConstantsArray(id);

			if (!constants)
			{
				constants = wxPropertyGrid::CreateConstantsArray(id);		

				constants->AddString("true");
				constants->AddString("false");

				registerConstants(constants);
			}

			if (constants)
			{
				// 创建出combobox
				property = wxEnumProperty(name, name, *constants);
				// 设置成当前这个参数的值
				property->SetValueFromString(value.c_str(),0);
			}
			break;
		}

	default:
		property = wxStringProperty(name, name, value.c_str());
		break;
	}

	assert (property);

	return property;
	

}
wxPGProperty * 
EffectObjectPropertyEditor::CreateParticleEmitterProperty(const Ogre::ParameterDef& propertyDef, Ogre::String &value)
{
	wxPGProperty* property = NULL;
	wxString name = propertyDef.name.c_str();
	Ogre::ParameterType paraType = propertyDef.paramType;

	switch ( paraType )
	{
	case Ogre::PT_COLOURVALUE:
		property = wxFairyColourProperty(name, name, colorConverter(value));
		break;

	case Ogre::PT_STRING:
		{
			if (propertyDef.name == "billboard_type")
			{
				wxPGConstants* constants = PopulateConstants(propertyDef.name);
				if (constants)
				{
					// 创建出combobox
					property = wxEnumProperty(name, name, *constants);
					// 设置成当前这个参数的值
					property->SetValueFromString(value.c_str(),0);
				}
			}			
			else
				property = wxStringProperty(name, name, value.c_str());

			break;
		}

	case Ogre::PT_REAL:
		{
			property = wxFloatProperty(name, name, Ogre::StringConverter::parseReal(value));

			break;
		}

	case Ogre::PT_BOOL:
		{
			size_t id = reinterpret_cast<size_t>(&name);
			wxPGConstants* constants = wxPropertyGrid::GetConstantsArray(id);

			if (!constants)
			{
				constants = wxPropertyGrid::CreateConstantsArray(id);		

				constants->AddString("true");
				constants->AddString("false");

				registerConstants(constants);
			}

			if (constants)
			{
				// 创建出combobox
				property = wxEnumProperty(name, name, *constants);
				// 设置成当前这个参数的值
				property->SetValueFromString(value.c_str(),0);
			}
			break;
		}

	default:
		property = wxStringProperty(name, name, value.c_str());
		break;
	}

	assert (property);

	return property;
	

}
wxPGProperty * 
EffectObjectPropertyEditor::CreateParticleAffectorProperty(const Ogre::ParameterDef& propertyDef, Ogre::String &value)
{
	wxPGProperty* property = NULL;
	wxString name = propertyDef.name.c_str();
	Ogre::ParameterType paraType = propertyDef.paramType;

	switch ( paraType )
	{
	case Ogre::PT_COLOURVALUE:
		property = wxFairyColourProperty(name, name, colorConverter(value));
		break;

	case Ogre::PT_STRING:
		{
			if (propertyDef.name != "animation_name")
			{
				wxPGConstants* constants = PopulateConstants(propertyDef.name);
				if (constants)
				{
					// 创建出combobox
					property = wxEnumProperty(name, name, *constants);
					// 设置成当前这个参数的值
					property->SetValueFromString(value.c_str(),0);
				}
			}			
			else
				property = wxStringProperty(name, name, value.c_str());

			break;
		}

	case Ogre::PT_REAL:
		{
			property = wxFloatProperty(name, name, Ogre::StringConverter::parseReal(value));

			break;
		}

	case Ogre::PT_BOOL:
		{
			size_t id = reinterpret_cast<size_t>(&name);
			wxPGConstants* constants = wxPropertyGrid::GetConstantsArray(id);

			if (!constants)
			{
				constants = wxPropertyGrid::CreateConstantsArray(id);		

				constants->AddString("true");
				constants->AddString("false");

				registerConstants(constants);
			}

			if (constants)
			{
				// 创建出combobox
				property = wxEnumProperty(name, name, *constants);
				// 设置成当前这个参数的值
				property->SetValueFromString(value.c_str(),0);
			}
			break;
		}

	default:
		property = wxStringProperty(name, name, value.c_str());
		break;
	}

	assert (property);

	return property;

}
wxPGProperty * 
EffectObjectPropertyEditor::CreateParticleRenderProperty(Ogre::ParticleSystemRenderer *renderer,
														 const Ogre::ParameterDef& propertyDef, Ogre::String &value)
{
	wxPGProperty* property = NULL;
	wxString name = propertyDef.name.c_str();
	Ogre::ParameterType paraType = propertyDef.paramType;

	switch ( paraType )
	{
	case Ogre::PT_COLOURVALUE:
		property = wxFairyColourProperty(name, name, colorConverter(value));
		break;

	case Ogre::PT_STRING:
		{
			Ogre::String RenderType =  renderer->getType();
			if(RenderType == "billboard")
			{
				if (propertyDef.name == "billboard_type")
				{
					wxPGConstants constants;
					constants.AddString("point");
					constants.AddString("oriented_common");
					constants.AddString("oriented_self");
					constants.AddString("perpendicular_common");
					constants.AddString("perpendicular_self");
				
					// 创建出combobox
					property = wxEnumProperty(name, name, constants);
					// 设置成当前这个参数的值
					property->SetValueFromString(value.c_str(),0);
	
				}
				else if (propertyDef.name == "billboard_origin")
				{
					wxPGConstants constants;
					constants.AddString("top_left");
					constants.AddString("top_center");
					constants.AddString("top_right");
					constants.AddString("center_left");
					constants.AddString("center");
					constants.AddString("center_right");
					constants.AddString("bottom_left");
					constants.AddString("bottom_center");
					constants.AddString("bottom_right");
					// 创建出combobox
					property = wxEnumProperty(name, name, constants);
					// 设置成当前这个参数的值
					property->SetValueFromString(value.c_str(),0);

				}
				else if (propertyDef.name == "billboard_rotation_type")
				{
					wxPGConstants constants;
					constants.AddString("vertex");
					constants.AddString("texcoord");
	
					// 创建出combobox
					property = wxEnumProperty(name, name, constants);
					// 设置成当前这个参数的值
					property->SetValueFromString(value.c_str(),0);

				}
				else
					property = wxStringProperty(name, name, value.c_str());

			}
			else if(RenderType == "mesh")
			{
				if (propertyDef.name == "orientation_type")
				{
					wxPGConstants constants;
					constants.AddString("oriented_origin");
					constants.AddString("oriented_self");

					property = wxEnumProperty(name, name, constants);
					property->SetValueFromString(value.c_str(),0);
				}
				else
					property = wxStringProperty(name, name, value.c_str());
			}
			else if(RenderType == "texcoord_billboard")
			{
				if (propertyDef.name == "billboard_type")
				{
					wxPGConstants constants;
					constants.AddString("point");
					constants.AddString("oriented_common");
					constants.AddString("oriented_self");
					constants.AddString("perpendicular_common");
					constants.AddString("perpendicular_self");

					property = wxEnumProperty(name, name, constants);
					property->SetValueFromString(value.c_str(),0);
				}
				else if (propertyDef.name == "billboard_origin")
				{
					wxPGConstants constants;
					constants.AddString("top_left");
					constants.AddString("top_center");
					constants.AddString("top_right");
					constants.AddString("center");
					constants.AddString("bottom_left");
					constants.AddString("bottom_center");
					constants.AddString("bottom_right");

					property = wxEnumProperty(name, name, constants);
					property->SetValueFromString(value.c_str(),0);
				}
				else if (propertyDef.name == "billboard_rotation_type")
				{
					wxPGConstants constants;
					constants.AddString("vertex");
					constants.AddString("texcoord");

					property = wxEnumProperty(name, name, constants);
					property->SetValueFromString(value.c_str(),0);
				}
				else
					property = wxStringProperty(name, name, value.c_str());
			}
			else
			{
				property = wxStringProperty(name, name, value.c_str());

			}
			break;
		}

	case Ogre::PT_REAL:
		{
			property = wxFloatProperty(name, name, Ogre::StringConverter::parseReal(value));

			break;
		}
	case Ogre::PT_UNSIGNED_INT:
		{
			property = wxIntProperty(name, name, Ogre::StringConverter::parseUnsignedInt(value));

			break;
		}

	case Ogre::PT_BOOL:
		{
			size_t id = reinterpret_cast<size_t>(&name);
			wxPGConstants* constants = wxPropertyGrid::GetConstantsArray(id);

			if (!constants)
			{
				constants = wxPropertyGrid::CreateConstantsArray(id);		

				constants->AddString("true");
				constants->AddString("false");

				registerConstants(constants);
			}

			if (constants)
			{
				// 创建出combobox
				property = wxEnumProperty(name, name, *constants);
				// 设置成当前这个参数的值
				property->SetValueFromString(value.c_str(),0);
			}
			break;
		}

	default:
		property = wxStringProperty(name, name, value.c_str());
		break;
	}

	assert (property);

	return property;

}

wxPGProperty* EffectObjectPropertyEditor::CreateEffectSettingProperty(const Ogre::ParameterDef& propertyDef, Ogre::String &value)
{
	wxPGProperty* property = NULL;
	wxString name = propertyDef.name.c_str();
	Ogre::ParameterType paraType = propertyDef.paramType;

	switch ( paraType )
	{
	case Ogre::PT_COLOURVALUE:
		property = wxFairyColourProperty(name, name, colorConverter(value));
		break;

	case Ogre::PT_STRING:
		{

			property = wxStringProperty(name, name, value.c_str());

			break;
		}

	case Ogre::PT_REAL:
		{
			property = wxFloatProperty(name, name, Ogre::StringConverter::parseReal(value));

			break;
		}

	case Ogre::PT_BOOL:
		{
			size_t id = reinterpret_cast<size_t>(&name);
			wxPGConstants* constants = wxPropertyGrid::GetConstantsArray(id);

			if (!constants)
			{
				constants = wxPropertyGrid::CreateConstantsArray(id);		

				constants->AddString("true");
				constants->AddString("false");

				registerConstants(constants);
			}

			if (constants)
			{
				// 创建出combobox
				property = wxEnumProperty(name, name, *constants);
				// 设置成当前这个参数的值
				property->SetValueFromString(value.c_str(),0);
			}
			break;
		}

	default:
		property = wxStringProperty(name, name, value.c_str());
		break;
	}

	assert (property);

	return property;

}
wxPGProperty* EffectObjectPropertyEditor::CreateEffectElementProperty(const Ogre::ParameterDef& propertyDef, Ogre::String &value)
{
	wxPGProperty* property = NULL;
	wxString name = propertyDef.name.c_str();
	Ogre::ParameterType paraType = propertyDef.paramType;

	switch ( paraType )
	{
	case Ogre::PT_COLOURVALUE:
		property = wxFairyColourProperty(name, name, colorConverter(value));
		break;

	case Ogre::PT_STRING:
		{
			if (name == "MeshName")
			{
				wxPGConstants* constants = PopulateConstants(name.c_str());
				if (constants)
				{
					// 创建出combobox
					property = wxEnumProperty(name, name, *constants);
					// 设置成当前这个参数的值
					property->SetValueFromString(value.c_str(),0);
				}				
			}
			else
			{
				property = wxStringProperty(name,name,value);
			}

			break;
		}

	case Ogre::PT_REAL:
		{
			property = wxFloatProperty(name, name, Ogre::StringConverter::parseReal(value));

			break;
		}

	case Ogre::PT_BOOL:
		{
			size_t id = reinterpret_cast<size_t>(&name);
			wxPGConstants* constants = wxPropertyGrid::GetConstantsArray(id);

			if (!constants)
			{
				constants = wxPropertyGrid::CreateConstantsArray(id);		

				constants->AddString("true");
				constants->AddString("false");

				registerConstants(constants);
			}

			if (constants)
			{
				// 创建出combobox
				property = wxEnumProperty(name, name, *constants);
				// 设置成当前这个参数的值
				property->SetValueFromString(value.c_str(),0);
			}
			break;
		}
	case Ogre::PT_QUATERNION:
		{
			if (name == "Orientation")
			{
				Ogre::Quaternion qOrientation = Ogre::StringConverter::parseQuaternion(value.c_str());
				Ogre::Radian rfAngle;
				Ogre::Vector3 rkAxis;
				qOrientation.ToAngleAxis(rfAngle,rkAxis);
				Ogre::String OrientationStr = Ogre::StringConverter::toString(rkAxis);
				OrientationStr += " ";
				OrientationStr += Ogre::StringConverter::toString(rfAngle.valueDegrees());
				
				property = wxStringProperty(name, name, OrientationStr.c_str());
			}
			else
			{
				property = wxStringProperty(name, name, value.c_str());
			}
			break;
		}

	default:
		property = wxStringProperty(name, name, value.c_str());
		break;
	}

	assert (property);

	return property;

}
wxPGProperty* EffectObjectPropertyEditor::CreateSkillSettingProperty(const Ogre::ParameterDef& propertyDef, Ogre::String &value)
{
	wxPGProperty* property = NULL;
	wxString name = propertyDef.name.c_str();
	Ogre::ParameterType paraType = propertyDef.paramType;

	switch ( paraType )
	{
	case Ogre::PT_COLOURVALUE:
		property = wxFairyColourProperty(name, name, colorConverter(value));
		break;

	case Ogre::PT_STRING:
		{
			if (name == "HitTime")
			{
				Ogre::String newValue("");

				for (unsigned short i=0; i<mSkill->getNumHitTimes(); ++i)
				{
					float fTime = mSkill->getHitTime(i);
	
					newValue += Ogre::StringConverter::toString( fTime);
					newValue += " ";
				}
				property = wxStringProperty(name, name, newValue.c_str());
			}
			else if (name == "BreakTime")
			{
				Ogre::String newValue("");

				for (unsigned short i=0; i<mSkill->getNumBreakTimes(); ++i)
				{
					float fTime = mSkill->getBreakTime(i);
	
					newValue += Ogre::StringConverter::toString( fTime);
					newValue += " ";
				}
				property = wxStringProperty(name, name, newValue.c_str());
			}
			else if (name == "ShakeTime")
			{
				Ogre::String newValue("");

				for (unsigned short i=0; i<mSkill->getNumShakeTimes(); ++i)
				{
					float fTime = mSkill->getShakeTime(i);
					newValue += Ogre::StringConverter::toString( fTime);
					newValue += " ";
				}
				property = wxStringProperty(name, name, newValue.c_str());
			}
			else if (name == "Animation")
			{			
				if (m_Frame == NULL)
				{
					break;
				}

				Fairy::LogicModel *object = m_Frame->GetActorSettingEditor()->GetActorObject();
				assert(object);

				wxPGConstants constants;
				Ogre::StringVector baseAnimName;

				for ( unsigned short i = 0; i < object->getSkeletonAnimationCount(); ++i )
				{
					Ogre::Animation *anim = object->getSkeletonAnimation(i);

					if ( anim )
					{
						constants.AddString(anim->getName().c_str());
						baseAnimName.push_back(anim->getName());
					}
				}

				// 加入obj中定义的动作名称
				Fairy::LogicModelManager::AnimationSkeletonLinksIterator asmIt = 
					Fairy::LogicModelManager::getSingleton().getAnimationSkeletonLinksIterator(object->getName());

				while ( asmIt.hasMoreElements() )
				{
					wxString animName(asmIt.peekNextKey().c_str());

					bool bExist = false;
					for (int i = 0; i<(int)baseAnimName.size(); ++i)
					{
						if (baseAnimName[i] == animName)
						{
							bExist = true;
							break;
						}
					}
					if(!bExist)
					{
						constants.AddString(animName.c_str());
					}
					asmIt.moveNext();
				}

				property = wxEnumProperty(name, name, constants);
				property->SetValueFromString(value.c_str(),0);

			}
			else
			{
				property = wxStringProperty(name, name, value.c_str());
			}			

			break;
		}

	case Ogre::PT_REAL:
		{
			if (name == "AttachTime")
			{
				float inputValue = Ogre::StringConverter::parseReal(value);

				if (inputValue >= 0.0f && inputValue <= 1.0f)
				{
					value = Ogre::StringConverter::toString( inputValue);
				}
			}

			property = wxFloatProperty(name, name, Ogre::StringConverter::parseReal(value));

			break;
		}

	case Ogre::PT_BOOL:
		{
			size_t id = reinterpret_cast<size_t>(&name);
			wxPGConstants* constants = wxPropertyGrid::GetConstantsArray(id);

			if (!constants)
			{
				constants = wxPropertyGrid::CreateConstantsArray(id);		
				constants->AddString("true");
				constants->AddString("false");
				registerConstants(constants);
			}

			if (constants)
			{
				// 创建出combobox
				property = wxEnumProperty(name, name, *constants);
				// 设置成当前这个参数的值
				property->SetValueFromString(value.c_str(),0);
			}
			break;
		}

	default:
		property = wxStringProperty(name, name, value.c_str());
		break;
	}

	assert (property);

	return property;

}
wxPGProperty* EffectObjectPropertyEditor::CreateSkillElementProperty(const Ogre::ParameterDef& propertyDef, Ogre::String &value)
{
	wxPGProperty* property = NULL;
	wxString name = propertyDef.name.c_str();
	Ogre::ParameterType paraType = propertyDef.paramType;

	switch ( paraType )
	{
	case Ogre::PT_COLOURVALUE:
		property = wxFairyColourProperty(name, name, colorConverter(value));
		break;

	case Ogre::PT_STRING:
		{
			property = wxStringProperty(name, name, value.c_str());
			if (name == "AttachPoint")
			{
				if (m_Frame == NULL)
				{
					break;
				}

				wxPGConstants constants;
			
				Fairy::LogicModel *pMbject  = m_Frame->GetActorSettingEditor()->GetActorObject();
				assert (pMbject);

				Ogre::StringVector boneNameList;

				for ( unsigned short i=0; i<pMbject->getNumBones(); ++i )
				{
					boneNameList.push_back(pMbject->getBoneName(i));
				}

				Ogre::StringVector locatorNames;
				pMbject->getLocatorNames(locatorNames);

				for ( size_t i=0; i<locatorNames.size(); ++i )
				{
					Fairy::LogicModel::LocatorValue value;
					if(pMbject->getLocatorInfo(locatorNames[i], value))
					{
						if(value.mBoneName != "" )
						{
							for ( int index=0; index<(int)boneNameList.size(); ++index )
							{
								if (boneNameList[index] == value.mBoneName)
								{
									// info of the locator is valid
									constants.AddString(locatorNames[i].c_str());
									break;
								}
							}						
						}
					}
				}

				property = wxEnumProperty(name, name, constants);
				property->SetValueFromString(value.c_str(),0);

			}
			else if (name == "EffectTemplateName")
			{
				if (m_Frame == NULL)
				{
					break;
				}

				wxPGConstants constants;

				Fairy::EffectManager::EffectTemplateIterator it = 
					Fairy::EffectManager::getSingleton().getEffectTemplateIterator();

				while ( it.hasMoreElements() )
				{
					 constants.AddString(it.peekNextKey().c_str());
					it.moveNext();
				}				

				property = wxEnumProperty(name, name, constants);
				property->SetValueFromString(value.c_str(),0);
			}
			else if (name == "BulletFlowTemplateName")
			{
				if (m_Frame == NULL)
				{
					break;
				}

				wxPGConstants constants;

				Fairy::BulletFlowSystemManager::BulletFlowTemplateIterator it = 
					Fairy::BulletFlowSystemManager::getSingleton().getBulletFlowTemplateIterator();

				while ( it.hasMoreElements() )
				{
					constants.AddString(it.peekNextKey().c_str());
					it.moveNext();
				}				

				property = wxEnumProperty(name, name, constants);
				property->SetValueFromString(value.c_str(),0);
			}
			else if (name == "SoundName")
			{
				if (m_Frame == NULL)
				{
					break;
				}

				wxPGConstants* constants = PopulateConstants(name.c_str());
				if (constants)
				{
					property = wxEnumProperty(name, name, *constants);
					property->SetValueFromString(value.c_str(),0);
				}
			}
			break;
		}

	case Ogre::PT_REAL:
		{
			property = wxFloatProperty(name, name, Ogre::StringConverter::parseReal(value));

			break;
		}

	case Ogre::PT_BOOL:
		{
			size_t id = reinterpret_cast<size_t>(&name);
			wxPGConstants* constants = wxPropertyGrid::GetConstantsArray(id);

			if (!constants)
			{
				constants = wxPropertyGrid::CreateConstantsArray(id);		

				constants->AddString("true");
				constants->AddString("false");

				registerConstants(constants);
			}

			if (constants)
			{
				// 创建出combobox
				property = wxEnumProperty(name, name, *constants);
				// 设置成当前这个参数的值
				property->SetValueFromString(value.c_str(),0);
			}
			break;
		}
	case Ogre::PT_QUATERNION:
		{
			if (name == "OffsetRotation")
			{
				Ogre::Quaternion qOrientation = Ogre::StringConverter::parseQuaternion(value.c_str());
				Ogre::Radian rfAngle;
				Ogre::Vector3 rkAxis;
				qOrientation.ToAngleAxis(rfAngle,rkAxis);
				Ogre::String OrientationStr = Ogre::StringConverter::toString(rkAxis);
				OrientationStr += " ";
				OrientationStr += Ogre::StringConverter::toString(rfAngle.valueDegrees());
				
				property = wxStringProperty(name, name, OrientationStr.c_str());
			}
			else
			{
				property = wxStringProperty(name, name, value.c_str());
			}
			break;
		}

	default:
		property = wxStringProperty(name, name, value.c_str());
		break;
	}

	assert (property);

	return property;
}
wxPGProperty* EffectObjectPropertyEditor::CreateBulletFlowSettingProperty(const Ogre::ParameterDef& propertyDef, Ogre::String &value)
{
	wxPGProperty* property = NULL;
	wxString name = propertyDef.name.c_str();
	Ogre::ParameterType paraType = propertyDef.paramType;

	switch ( paraType )
	{
	case Ogre::PT_COLOURVALUE:
		property = wxFairyColourProperty(name, name, colorConverter(value));
		break;

	case Ogre::PT_STRING:
		property = wxStringProperty(name, name, value.c_str());
		break;

	case Ogre::PT_REAL:
		property = wxFloatProperty(name, name, Ogre::StringConverter::parseReal(value));
		break;

	case Ogre::PT_BOOL:
		{
			size_t id = reinterpret_cast<size_t>(&name);
			wxPGConstants* constants = wxPropertyGrid::GetConstantsArray(id);

			if (!constants)
			{
				constants = wxPropertyGrid::CreateConstantsArray(id);		
				constants->AddString("true");
				constants->AddString("false");
				registerConstants(constants);
			}

			if (constants)
			{
				// 创建出combobox
				property = wxEnumProperty(name, name, *constants);
				// 设置成当前这个参数的值
				property->SetValueFromString(value.c_str(),0);
			}
			break;
		}

	default:
		property = wxStringProperty(name, name, value.c_str());
		break;
	}

	assert (property);

	return property;
}
wxPGProperty* EffectObjectPropertyEditor::CreateBulletOperatorProperty(Fairy::BulletOperator* pOperator,const Ogre::ParameterDef& propertyDef, Ogre::String &value)
{
	wxPGProperty* property = NULL;
	wxString name = propertyDef.name.c_str();
	Ogre::ParameterType paraType = propertyDef.paramType;

	switch ( paraType )
	{
	case Ogre::PT_COLOURVALUE:
		property = wxFairyColourProperty(name, name, colorConverter(value));
		break;

	case Ogre::PT_STRING:
		{
			property = wxStringProperty(name, name, value.c_str());
			if (name == "pursuittype")
			{
				wxPGConstants constants;

				constants.AddString("parabola");
				constants.AddString("beeline");
				constants.AddString("roate");
				constants.AddString("curve");
				constants.AddString("none");

				property = wxEnumProperty(name, name, constants);
				property->SetValueFromString(value.c_str(),0);
			}	
			else if (name == "bulletsystemname")
			{
				wxPGConstants constants;
				Fairy::BulletFlowSystem* pBulletFlowSystem = pOperator->getParent()->getParent();
				Ogre::String filename = Fairy::BulletFlowSystemManager::getSingleton().getFileNameByTemplateName(pBulletFlowSystem->getTemplateName());
				Ogre::StringVector bulletSystemNames;
				Fairy::BulletSystemManager::getSingleton().getTemplatesFromScriptFile(filename,bulletSystemNames);

				Ogre::StringVector::iterator it = bulletSystemNames.begin();
				while(it != bulletSystemNames.end())
				{
					Ogre::String bulletSystemName = *it;
					constants.AddString(bulletSystemName.c_str());
					it ++;
				}

				property = wxEnumProperty(name, name, constants);
				property->SetValueFromString(value.c_str(),0);
			}
		}
		break;

	case Ogre::PT_REAL:
		property = wxFloatProperty(name, name, Ogre::StringConverter::parseReal(value));
		break;

	case Ogre::PT_BOOL:
		{
			size_t id = reinterpret_cast<size_t>(&name);
			wxPGConstants* constants = wxPropertyGrid::GetConstantsArray(id);

			if (!constants)
			{
				constants = wxPropertyGrid::CreateConstantsArray(id);		
				constants->AddString("true");
				constants->AddString("false");
				registerConstants(constants);
			}

			if (constants)
			{
				// 创建出combobox
				property = wxEnumProperty(name, name, *constants);
				// 设置成当前这个参数的值
				property->SetValueFromString(value.c_str(),0);
			}
			break;
		}

	default:
		property = wxStringProperty(name, name, value.c_str());
		break;
	}

	assert (property);

	return property;
}
wxPGProperty* EffectObjectPropertyEditor::CreateBulletSystemElementProperty(const Ogre::ParameterDef& propertyDef, Ogre::String &value)
{
	wxPGProperty* property = NULL;
	wxString name = propertyDef.name.c_str();
	Ogre::ParameterType paraType = propertyDef.paramType;

	switch ( paraType )
	{
	case Ogre::PT_COLOURVALUE:
		property = wxFairyColourProperty(name, name, colorConverter(value));
		break;

	case Ogre::PT_STRING:
		{
			property = wxStringProperty(name, name, value.c_str());
			if (name == "AttachPoint")
			{
				if (m_Frame == NULL)
				{
					break;
				}

				wxPGConstants constants;

				Fairy::LogicModel *pMbject  = m_Frame->GetActorSettingEditor()->GetActorObject();
				assert (pMbject);

				Ogre::StringVector boneNameList;

				for ( unsigned short i=0; i<pMbject->getNumBones(); ++i )
				{
					boneNameList.push_back(pMbject->getBoneName(i));
				}

				Ogre::StringVector locatorNames;
				pMbject->getLocatorNames(locatorNames);

				for ( size_t i=0; i<locatorNames.size(); ++i )
				{
					Fairy::LogicModel::LocatorValue value;
					if(pMbject->getLocatorInfo(locatorNames[i], value))
					{
						if(value.mBoneName != "" )
						{
							for ( int index=0; index<(int)boneNameList.size(); ++index )
							{
								if (boneNameList[index] == value.mBoneName)
								{
									// info of the locator is valid
									constants.AddString(locatorNames[i].c_str());
									break;
								}
							}						
						}
					}
				}

				property = wxEnumProperty(name, name, constants);
				property->SetValueFromString(value.c_str(),0);

			}
			else if (name == "EffectTemplateName")
			{
				if (m_Frame == NULL)
				{
					break;
				}

				wxPGConstants constants;

				Fairy::EffectManager::EffectTemplateIterator it = 
					Fairy::EffectManager::getSingleton().getEffectTemplateIterator();

				while ( it.hasMoreElements() )
				{
					constants.AddString(it.peekNextKey().c_str());
					it.moveNext();
				}				

				property = wxEnumProperty(name, name, constants);
				property->SetValueFromString(value.c_str(),0);
			}
			else if (name == "SoundName")
			{
				if (m_Frame == NULL)
				{
					break;
				}

				wxPGConstants* constants = PopulateConstants(name.c_str());
				if (constants)
				{
					property = wxEnumProperty(name, name, *constants);
					property->SetValueFromString(value.c_str(),0);
				}
			}
			break;
		}

	case Ogre::PT_REAL:
		{
			property = wxFloatProperty(name, name, Ogre::StringConverter::parseReal(value));

			break;
		}

	case Ogre::PT_BOOL:
		{
			size_t id = reinterpret_cast<size_t>(&name);
			wxPGConstants* constants = wxPropertyGrid::GetConstantsArray(id);

			if (!constants)
			{
				constants = wxPropertyGrid::CreateConstantsArray(id);		

				constants->AddString("true");
				constants->AddString("false");

				registerConstants(constants);
			}

			if (constants)
			{
				// 创建出combobox
				property = wxEnumProperty(name, name, *constants);
				// 设置成当前这个参数的值
				property->SetValueFromString(value.c_str(),0);
			}
			break;
		}
	case Ogre::PT_QUATERNION:
		{
			if (name == "OffsetRotation")
			{
				Ogre::Quaternion qOrientation = Ogre::StringConverter::parseQuaternion(value.c_str());
				Ogre::Radian rfAngle;
				Ogre::Vector3 rkAxis;
				qOrientation.ToAngleAxis(rfAngle,rkAxis);
				Ogre::String OrientationStr = Ogre::StringConverter::toString(rkAxis);
				OrientationStr += " ";
				OrientationStr += Ogre::StringConverter::toString(rfAngle.valueDegrees());

				property = wxStringProperty(name, name, OrientationStr.c_str());
			}
			else
			{
				property = wxStringProperty(name, name, value.c_str());
			}
			break;
		}

	default:
		property = wxStringProperty(name, name, value.c_str());
		break;
	}

	assert (property);

	return property;
}
void EffectObjectPropertyEditor::InitParticleProperty(void)
{

	mPropertyGridManager->Freeze();
	mPropertyGridManager->GetGrid()->Clear();

	// particle system option

	wxPGId id =	mPropertyGridManager->Append( new EffectObjectPropertyClass(wxT("BasicSetting"),wxT("ParticleBasicSetting"),mParticleSystem));

	wxPGId itemId;

	if ( id.IsOk() )
	{
		mPropertyGridManager->DisableProperty(id);
		mPropertyGridManager->SetPropertyColour(id, *wxRED);

		const Ogre::ParameterList &paraList = mParticleSystem->getParameters();

		for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
		{
			Ogre::String value = mParticleSystem->getParameter(paraList[paraCount].name);

			itemId = mPropertyGridManager->AppendIn( id, CreateProperty(paraList[paraCount], value) );
			mPropertyGridManager->SetPropertyColourToDefault(itemId);
		}
	}

	Ogre::ParticleSystemRenderer *pRenderer = mParticleSystem->getRenderer();

	assert (pRenderer);

	id = mPropertyGridManager->Append(new EffectObjectPropertyClass(pRenderer->getType().c_str(),wxT("ParticleRenderer"),pRenderer) );

	if ( id.IsOk() )
	{
		mPropertyGridManager->DisableProperty(id);
		mPropertyGridManager->SetPropertyColour(id, *wxBLUE);

		const Ogre::ParameterList &paraList = pRenderer->getParameters();

		for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
		{
			Ogre::String value = pRenderer->getParameter(paraList[paraCount].name);

			itemId = mPropertyGridManager->AppendIn( id, CreateProperty(paraList[paraCount], value) );
			mPropertyGridManager->SetPropertyColourToDefault(itemId);

		}
	}

	for ( unsigned short i=0; i<mParticleSystem->getNumEmitters(); ++i )
	{
		Ogre::ParticleEmitter *emitter = mParticleSystem->getEmitter(i);
		assert (emitter);

		id = mPropertyGridManager->Append(new EffectObjectPropertyClass(emitter->getType().c_str(),wxT("ParticleEmtter"),emitter));

		if ( id.IsOk() )
		{
			mPropertyGridManager->DisableProperty(id);
			mPropertyGridManager->SetPropertyColour(id, *wxGREEN);

			const Ogre::ParameterList &paraList = emitter->getParameters();

			for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
			{
				Ogre::String value = emitter->getParameter(paraList[paraCount].name);

				itemId = mPropertyGridManager->AppendIn( id, CreateProperty(paraList[paraCount], value) );
				mPropertyGridManager->SetPropertyColourToDefault(itemId);

			}
		}		
	}

	for ( unsigned short i=0; i<mParticleSystem->getNumAffectors(); ++i )
	{
		Ogre::ParticleAffector *affector = mParticleSystem->getAffector(i);
		assert (affector);

		id = mPropertyGridManager->Append(new EffectObjectPropertyClass(affector->getType().c_str(),wxT("ParticleAffector"),affector));

		if ( id.IsOk() )
		{
			mPropertyGridManager->DisableProperty(id);
			mPropertyGridManager->SetPropertyColour(id, *wxCYAN);

			const Ogre::ParameterList &paraList = affector->getParameters();

			for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
			{
				Ogre::String value = affector->getParameter(paraList[paraCount].name);

				itemId = mPropertyGridManager->AppendIn( id, CreateProperty(paraList[paraCount], value) );
				mPropertyGridManager->SetPropertyColourToDefault(itemId);

			}
		}		
	}

	mPropertyGridManager->Thaw();
	mPropertyGridManager->ExpandAll();
	mPropertyGridManager->Refresh();

}

void
EffectObjectPropertyEditor::SetSceneManipulator(Fairy::SceneManipulator* sceneManipulator)
{
	mSceneManipulator = sceneManipulator;
	mSceneManipulator->addSceneListener(this);
}
void
EffectObjectPropertyEditor::registerConstants(wxPGConstants* constants)
{
	assert(constants);
	constants->Ref();
	mConstantList.push_back(constants);
}

wxPGConstants * EffectObjectPropertyEditor::PopulateConstants(const Ogre::String& name)
{
	size_t id = reinterpret_cast<size_t>(&name);
	wxPGConstants* constants = wxPropertyGrid::GetConstantsArray(id);

	if (constants)
	{
		for (std::list<wxPGConstants*>::iterator it = mConstantList.begin(); it != mConstantList.end(); ++it)
		{
			if (constants == *it)
			{
				if (constants->UnRef())
					delete constants;

				mConstantList.erase(it);

				break;
			}
		}
	}
//	if (!constants)
//	{
		constants = wxPropertyGrid::CreateConstantsArray(id);

		// 先看看配置文件中有没有附加的选项
		if (mConfig && mConfig->exists( name, "combo" ))
		{
			Ogre::StringVector valueList;

			valueList = mConfig->getList( name, "combo", "," );

			for ( size_t i=0; i<valueList.size(); ++i )
			{
				constants->Add(valueList[i].c_str());
			}
		}
		else if ( name == "material" )
		{
			typedef std::list<Ogre::String> MaterialFileNameList;
			MaterialFileNameList materialFileNameList;

			Ogre::FileInfoListPtr fileInfoList =
				Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(
				Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
				"*.material");
			for (Ogre::FileInfoList::const_iterator it = fileInfoList->begin(); it != fileInfoList->end(); ++it)
			{
				if ( it->archive->getName() == EFFECT_PATH)
				{
					materialFileNameList.push_back(it->filename);
				}
			}

			Ogre::ResourceManager::ResourceMapIterator resourceMapIterator = Ogre::MaterialManager::getSingleton().getResourceIterator();

			while ( resourceMapIterator.hasMoreElements() )
			{				
				Ogre::String matName = resourceMapIterator.peekNextValue()->getName();

				for ( MaterialFileNameList::iterator i = materialFileNameList.begin();
					i != materialFileNameList.end(); ++i )
				{
					if ( *i == resourceMapIterator.peekNextValue()->getOrigin() )
					{
						constants->Add(matName.c_str());
						break;
					}
				}

				resourceMapIterator.moveNext();
			}
		}
		else if ( name == "mesh_name" )
		{
			Ogre::FileInfoListPtr fileInfoList =
				Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(
				Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
				"*.mesh");
			for (Ogre::FileInfoList::const_iterator it = fileInfoList->begin(); it != fileInfoList->end(); ++it)
			{
				if ( it->archive->getName() == EFFECT_PATH)
				{
					const Fairy::String& name = it->filename;
					constants->Add(name.c_str());
				}
			}
		}
		else if ( name == "MeshName")
		{
			Ogre::FileInfoListPtr fileInfoList =
				Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(
				Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
				"*.mesh");

			for(Ogre::FileInfoList::const_iterator it = fileInfoList->begin(); it != fileInfoList->end(); ++it)
			{	
				const Ogre::Archive* archive = it->archive;
				const Ogre::String name = archive->getName();
				if (name == "../resource/media/特效/模型")
				{
					Ogre::String fileName = it->filename;
					constants->Add(fileName.c_str());
				}					
			}
		}
		else if ( name == "SoundName")
		{
			Ogre::FileInfoListPtr fileInfoList =
				Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(
				Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
				"*.wav");

			for(Ogre::FileInfoList::const_iterator it = fileInfoList->begin(); it != fileInfoList->end(); ++it)
			{	
				const Ogre::Archive* archive = it->archive;
				const Ogre::String name = archive->getName();
				if (name == "../resource/sound/skill")
				{
					Ogre::String fileName = "skill\\"+it->filename;
					constants->Add(fileName.c_str());
				}					
			}
		}

		registerConstants(constants);
//	}

	return constants;
}
