// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "ParticleEditorDialog.h"
#endif

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

#include "WXParticleEditorDialog.h"
#include "WXMaterialEditorDialog.h"

// ----------------------------------------------------------------------------
// PropertyGrid headers
// ----------------------------------------------------------------------------

#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/propdev.h>
#include <wx/propgrid/advprops.h>
#include <wx/propgrid/manager.h>

#include "WXColourProperty.h"
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

#include "WXConfig.h"
#include "EffectSettingGlobalData.h"

//extern EffectSettingGlobalData gEffectSettingGlobalData;
// ----------------------------------------------------------------------------
// Resources
// ----------------------------------------------------------------------------

#include "res/ParticleEditor_wdr.h"

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
// ----------------------------------------------------------------------------
// macros
// ----------------------------------------------------------------------------

#if wxUSE_STL
#define AS_STRING(s)    (s)
#else
#define AS_STRING(s)    ((s).c_str())
#endif

// ----------------------------------------------------------------------------
// ParticleEditorDialog class
// ----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(ParticleEditorDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(ParticleEditorDialog, wxDialog)
	EVT_INIT_DIALOG(ParticleEditorDialog::OnInitDialog)
	EVT_PG_CHANGED(ID_PROPERTY_EDITOR, ParticleEditorDialog::OnPropertyChanged)
	EVT_PG_SELECTED(ID_PROPERTY_EDITOR, ParticleEditorDialog::OnPropertyGridSelect)

	EVT_BUTTON(ID_BUTTON_REFRESH, ParticleEditorDialog::OnRefreshButtonDown)
	EVT_BUTTON(ID_BUTTON_ADD_EMITTER, ParticleEditorDialog::OnAddEmitterButtonDown)
	EVT_BUTTON(ID_BUTTON_ADD_AFFECTOR, ParticleEditorDialog::OnAddAffectorButtonDown)
	EVT_BUTTON(ID_BUTTON_DELETE, ParticleEditorDialog::OnDeleteButtonDown)
	EVT_BUTTON(ID_BUTTON_SAVE, ParticleEditorDialog::OnSaveButtonDown)
	EVT_BUTTON(ID_BUTTON_SHOW_MATERIAL_EDITOR, ParticleEditorDialog::OnShowMaterialEditorButtonDown)

END_EVENT_TABLE()


void
ParticleEditorDialog::OnInitDialog(wxInitDialogEvent &e)
{
	wxDialog::OnInitDialog(e);

	wxSizer* sizer = ParticleEditor(this, true, true);
	mPropertyGridManager = wxDynamicCast(this->FindWindow(ID_PROPERTY_EDITOR), wxPropertyGridManager);

	if (!mConfig)
	{
		mConfig = new FairyConfig("../EditorConfigFiles/FairyParticleEditor.cfg");
	}	
}

void ParticleEditorDialog::OnRefreshButtonDown(wxCommandEvent &e)
{
	Fairy::Scene::ObjectsByTypeRange particles =
		mSceneManipulator->getSceneInfo()->findObjectsByType(Fairy::ParticleSystemObject::msType);

	for (Fairy::Scene::ObjectsByTypeIterator it = particles.first; it != particles.second; ++it)
	{
		const Fairy::ObjectPtr& object = *it;
		Fairy::ParticleSystemObject* particleObject = static_cast<Fairy::ParticleSystemObject *>(object.get());

		Ogre::ParticleSystem *system = particleObject->getParticleSystem();
		Ogre::String templateName = particleObject->getPropertyAsString("particle system name");

		// ����������ֱ�Ӷ�ģ����޸ģ�����Ҫ�޸�ȫ����ͬ��ģ�����Ч
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

void ParticleEditorDialog::OnAddEmitterButtonDown(wxCommandEvent &e)
{
	Fairy::Scene::ObjectsByTypeRange particles =
		mSceneManipulator->getSceneInfo()->findObjectsByType(Fairy::ParticleSystemObject::msType);

	for (Fairy::Scene::ObjectsByTypeIterator it = particles.first; it != particles.second; ++it)
	{
		const Fairy::ObjectPtr& object = *it;
		Fairy::ParticleSystemObject* particleObject = static_cast<Fairy::ParticleSystemObject *>(object.get());

		Ogre::ParticleSystem *system = particleObject->getParticleSystem();
		Ogre::String templateName = particleObject->getPropertyAsString("particle system name");

		// ����������ֱ�Ӷ�ģ����޸ģ�����Ҫ�޸�ȫ����ͬ��ģ�����Ч
		if ( system && templateName == mParticleSystemTemplateName )
		{
			system->addEmitter(mEmitterComboBox->GetStringSelection().c_str());
		}
	}

	mParticleSystem->addEmitter(mEmitterComboBox->GetStringSelection().c_str());

	InitParticleProperty();
}

void ParticleEditorDialog::OnAddAffectorButtonDown(wxCommandEvent &e)
{
	bool isTemplateSetting = false;

	Fairy::Scene::ObjectsByTypeRange particles =
		mSceneManipulator->getSceneInfo()->findObjectsByType(Fairy::ParticleSystemObject::msType);

	for (Fairy::Scene::ObjectsByTypeIterator it = particles.first; it != particles.second; ++it)
	{
		const Fairy::ObjectPtr& object = *it;
		Fairy::ParticleSystemObject* particleObject = static_cast<Fairy::ParticleSystemObject *>(object.get());

		Ogre::ParticleSystem *system = particleObject->getParticleSystem();
		Ogre::String templateName = particleObject->getPropertyAsString("particle system name");

		// ����������ֱ�Ӷ�ģ����޸ģ�����Ҫ�޸�ȫ����ͬ��ģ�����Ч
		if ( system && templateName == mParticleSystemTemplateName )
		{			
			system->addAffector(mAffectorComboBox->GetStringSelection().c_str());

			if (!isTemplateSetting)
			{
				mParticleSystem->addAffector(mAffectorComboBox->GetStringSelection().c_str());
				isTemplateSetting = true;
			}
		}	
	}

	InitParticleProperty();
}


void ParticleEditorDialog::OnDeleteButtonDown(wxCommandEvent &e)
{
	wxPGId id = mPropertyGridManager->GetSelectedProperty();

	if ( true == id.IsOk() && false == mPropertyGridManager->IsPropertyEnabled(id) )
	{
		short index = mPropertyGridManager->GetPropertyIndex(id);

		// �������ǰ��affector������
		index = index - 2;

		if (index < 0)
			return;

		bool deleteEmitter = false;

		if ( ( index - mParticleSystem->getNumEmitters() ) < 0 )
		{
			deleteEmitter = true;
		}
		
		short indexAffector = index - mParticleSystem->getNumEmitters();

		Fairy::Scene::ObjectsByTypeRange particles =
			mSceneManipulator->getSceneInfo()->findObjectsByType(Fairy::ParticleSystemObject::msType);

		for (Fairy::Scene::ObjectsByTypeIterator it = particles.first; it != particles.second; ++it)
		{
			const Fairy::ObjectPtr& object = *it;
			Fairy::ParticleSystemObject* particleObject = static_cast<Fairy::ParticleSystemObject *>(object.get());

			Ogre::ParticleSystem *system = particleObject->getParticleSystem();
			Ogre::String templateName = particleObject->getPropertyAsString("particle system name");

			// ����������ֱ�Ӷ�ģ����޸ģ�����Ҫ�޸�ȫ����ͬ��ģ�����Ч
			if ( system && templateName == mParticleSystemTemplateName )
			{
				if (deleteEmitter)
				{
					system->removeEmitter(index);
				}
				else
				{
					system->removeAffector(indexAffector);
				}
			}
		}

		if (deleteEmitter)
		{
			mParticleSystem->removeEmitter(index);
		}
		else
		{
			mParticleSystem->removeAffector(indexAffector);
		}

		InitParticleProperty();
	}	
}

ParticleEditorDialog::~ParticleEditorDialog()
{
	for (std::list<wxPGConstants*>::const_iterator it = mConstantList.begin(); it != mConstantList.end(); ++it)
	{
		wxPGConstants* constants = *it;
		if (constants->UnRef())
			delete constants;
	}
	mConstantList.clear();

	// ɾ�����ʶԻ���
	if (mMaterialEditorDialog)
	{
		mMaterialEditorDialog->Destroy();
		delete mMaterialEditorDialog;
		mMaterialEditorDialog = NULL;
	}

	if (mConfig)
	{
		delete mConfig;
		mConfig = NULL;
	}
}

void
ParticleEditorDialog::OnPropertyChanged(wxPropertyGridEvent &e)
{
	enum IndexType
	{
		BasicSetting,
		Renderer,
	};

	unsigned short numOfEmitters = mParticleSystem->getNumEmitters();
	unsigned short numOfAffectors = mParticleSystem->getNumAffectors();

	// Skip parent properties
	if (e.GetPropertyPtr()->GetParentingType() != 0)
		return;

	wxPGId id = e.GetPropertyPtr()->GetId();
	wxPGId parentId = mPropertyGridManager->GetPropertyParent(id);

	Fairy::String propertyName = e.GetPropertyName().c_str();
	Fairy::String propertyValue = e.GetPropertyValueAsString().c_str();

	Fairy::Scene::ObjectsByTypeRange particles =
		mSceneManipulator->getSceneInfo()->findObjectsByType(Fairy::ParticleSystemObject::msType);
    Ogre::SceneManager* creator = mSceneManipulator->getSceneManager();

	bool isTemplateSetting = false;

	for (Fairy::Scene::ObjectsByTypeIterator it = particles.first; it != particles.second; ++it)
	{
		unsigned short index = mPropertyGridManager->GetPropertyIndex(parentId);

		const Fairy::ObjectPtr& object = *it;
		Fairy::ParticleSystemObject* particleObject = static_cast<Fairy::ParticleSystemObject *>(object.get());

		Ogre::ParticleSystem *system = particleObject->getParticleSystem();
		Ogre::String templateName = particleObject->getPropertyAsString("particle system name");

		// ����������ֱ�Ӷ�ģ����޸ģ�����Ҫ�޸�ȫ����ͬ��ģ�����Ч
		if ( system && templateName == mParticleSystemTemplateName )
		{
			switch (index)
			{
			case BasicSetting:
				{
					if ( propertyName == "quota" )
					{
						Ogre::String systemName = system->getName();
						Ogre::SceneNode *node = system->getParentSceneNode();
						Ogre::ParticleSystem* tempPSys =
                            creator->createParticleSystem("tempParticleSystem", Ogre::StringConverter::parseInt(propertyValue));

						// make a copy but use quota of tempPSys (request copyQuota = false)
						cloneParticleSystem(tempPSys, system, false);

						// Remove the current system.
						unloadPsystem(system, mParticleSystemTemplateName);

						// Create a new system based on the temp system, with the same name as the currently selected system.
						Ogre::ParticleSystem* newPSys =
                            creator->createParticleSystem(systemName, Ogre::StringConverter::parseInt(propertyValue));

						// Copy the parameters from temp to new
						cloneParticleSystem(newPSys, tempPSys);

						// Destroy the temporary system
						OGRE_DELETE tempPSys;
						tempPSys = NULL;

						// Attach the new version of the system to the main node.
                        if (node)
						    node->attachObject(newPSys);

						particleObject->setParticleSystem(newPSys);

                        newPSys->setRenderQueueGroup(Ogre::RENDER_QUEUE_8);

						if (!isTemplateSetting)
						{
							mParticleSystem->setParameter(propertyName, propertyValue);
							isTemplateSetting = true;
						}

					}
					else if (propertyName == "renderer")
					{
						system->setParameter(propertyName, propertyValue);

						if (propertyValue == "mesh")
						{
							system->setDefaultDimensions(1.0f, 1.0f);
							// ɾ�����е�Ӱ����
							system->removeAllAffectors();
						}

						if (!isTemplateSetting)
						{
							mParticleSystem->setParameter(propertyName, propertyValue);

							if (propertyValue == "mesh")
							{
								mParticleSystem->setDefaultDimensions(1.0f, 1.0f);
								// ɾ�����е�Ӱ����
								mParticleSystem->removeAllAffectors();
							}

							isTemplateSetting = true;
						}
						InitParticleProperty();
					}
					else
					{						
						system->setParameter(propertyName, propertyValue);

						if (!isTemplateSetting)
						{
							mParticleSystem->setParameter(propertyName, propertyValue);
							isTemplateSetting = true;
						}
					}

					break;
				}

			case Renderer:
				{
					system->getRenderer()->setParameter(propertyName, propertyValue);

					if (!isTemplateSetting)
					{
						mParticleSystem->getRenderer()->setParameter(propertyName, propertyValue);
						isTemplateSetting = true;
					}
					break;
				}

			default:
				{
					// affector
					if ( index > (Renderer + numOfEmitters) )
					{
						index = index - Renderer - numOfEmitters - 1;

						system->getAffector(index)->setParameter(propertyName, propertyValue);

						if (!isTemplateSetting)
						{
							mParticleSystem->getAffector(index)->setParameter(propertyName, propertyValue);
							isTemplateSetting = true;
						}
					}
					// emitter
					else
					{
						index = index - Renderer - 1;

						system->getEmitter(index)->setParameter(propertyName, propertyValue);

						if (!isTemplateSetting)
						{
							mParticleSystem->getEmitter(index)->setParameter(propertyName, propertyValue);
							isTemplateSetting = true;
						}
					}

					break;
				}
			}
		}
	}

    // �Ѿ�������effect�е�����ҲҪ��Ӧ�޸�
	Fairy::Scene::ObjectsByTypeRange effects =
		mSceneManipulator->getSceneInfo()->findObjectsByType(Fairy::EffectObject::msType);

	for (Fairy::Scene::ObjectsByTypeIterator it = effects.first; it != effects.second; ++it)
	{
		unsigned short index = mPropertyGridManager->GetPropertyIndex(parentId);

		const Fairy::ObjectPtr& object = *it;
		const Fairy::EffectObject* effectObject = static_cast<Fairy::EffectObject *>(object.get());

		Fairy::Effect *effect = effectObject->getEffect();

		assert (effect);

		for ( size_t i=0; i<effect->getNumElements(); ++i )
		{
			Fairy::EffectElement *element = effect->getElement(i);

			if ( element->getType() == "Particle" )
			{
				Ogre::String name = static_cast<Fairy::ParticleElement *>(element)->getParticleSystemName();
				if (name == mParticleSystemTemplateName)
				{
					Ogre::ParticleSystem *system = static_cast<Fairy::ParticleElement *>(element)->getParticleSystem();

					switch (index)
					{
					case BasicSetting:
						{
							if ( propertyName == "quota" )
							{
								Ogre::String systemName = system->getName();
                                Ogre::SceneNode *node = system->getParentSceneNode();
								Ogre::ParticleSystem* tempPSys =
                                    creator->createParticleSystem("tempParticleSystem", Ogre::StringConverter::parseInt(propertyValue));

								// make a copy but use quota of tempPSys (request copyQuota = false)
								cloneParticleSystem(tempPSys, system, false);

								// Remove the current system.
								unloadPsystem(system, mParticleSystemTemplateName);

								// Create a new system based on the temp system, with the same name as the currently selected system.
								Ogre::ParticleSystem* newPSys =
                                    creator->createParticleSystem(systemName, Ogre::StringConverter::parseInt(propertyValue));

								// Copy the parameters from temp to new
								cloneParticleSystem(newPSys, tempPSys);

								// Destroy the temporary system
								OGRE_DELETE tempPSys;
								tempPSys = NULL;

								// Attach the new version of the system to the main node.
                                if (node)
								    node->attachObject(newPSys);

								static_cast<Fairy::ParticleElement *>(element)->setParticleSystem(newPSys);

                                newPSys->setRenderQueueGroup(Ogre::RENDER_QUEUE_8);

								if (!isTemplateSetting)
								{
									mParticleSystem->setParameter(propertyName, propertyValue);
									isTemplateSetting = true;
								}

							}
							else if (propertyName == "renderer")
							{
								system->setParameter(propertyName, propertyValue);

								if (propertyValue == "mesh")
								{
									system->setDefaultDimensions(1.0f, 1.0f);
									// ɾ�����е�Ӱ����
									system->removeAllAffectors();
								}

								if (!isTemplateSetting)
								{
									mParticleSystem->setParameter(propertyName, propertyValue);

									if (propertyValue == "mesh")
									{
										mParticleSystem->setDefaultDimensions(1.0f, 1.0f);
										// ɾ�����е�Ӱ����
										mParticleSystem->removeAllAffectors();
									}

									isTemplateSetting = true;
								}
								InitParticleProperty();
							}
							else
							{						
								system->setParameter(propertyName, propertyValue);

								if (!isTemplateSetting)
								{
									mParticleSystem->setParameter(propertyName, propertyValue);
									isTemplateSetting = true;
								}
							}

							break;
						}

					case Renderer:
						{
							system->getRenderer()->setParameter(propertyName, propertyValue);

							if (!isTemplateSetting)
							{
								mParticleSystem->getRenderer()->setParameter(propertyName, propertyValue);
								isTemplateSetting = true;
							}
							break;
						}

					default:
						{
							// affector
							if ( index > (Renderer + numOfEmitters) )
							{
								index = index - Renderer - numOfEmitters - 1;

								system->getAffector(index)->setParameter(propertyName, propertyValue);

								if (!isTemplateSetting)
								{
									mParticleSystem->getAffector(index)->setParameter(propertyName, propertyValue);
									isTemplateSetting = true;
								}
							}
							// emitter
							else
							{
								index = index - Renderer - 1;

								system->getEmitter(index)->setParameter(propertyName, propertyValue);

								if (!isTemplateSetting)
								{
									mParticleSystem->getEmitter(index)->setParameter(propertyName, propertyValue);
									isTemplateSetting = true;
								}
							}

							break;
						}
					}
				}
			}
		}		
	}

	if (gEffectSettingGlobalData.mCurrentSkill)
	{
		for ( unsigned short i = 0; i < gEffectSettingGlobalData.mCurrentSkill->getNumAnimationEffectInfos(); ++i )
		{
			unsigned short index = mPropertyGridManager->GetPropertyIndex(parentId);

			Fairy::AnimationEffectInfo *effectInfo = gEffectSettingGlobalData.mCurrentSkill->getAnimationEffectInfo(i);
			assert (effectInfo);
			
			Fairy::Effect *effect = effectInfo->getEffect();

			assert (effect);

			for ( size_t i=0; i<effect->getNumElements(); ++i )
			{
				Fairy::EffectElement *element = effect->getElement(i);

				if ( element->getType() == "Particle" )
				{
					Ogre::String name = static_cast<Fairy::ParticleElement *>(element)->getParticleSystemName();
					if (name == mParticleSystemTemplateName)
					{
						Ogre::ParticleSystem *system = static_cast<Fairy::ParticleElement *>(element)->getParticleSystem();

						switch (index)
						{
						case BasicSetting:
							{
								if ( propertyName == "quota" )
								{
									Ogre::String systemName = system->getName();
                                    Ogre::SceneNode *node = system->getParentSceneNode();

                                    Ogre::ParticleSystem* tempPSys =
                                        creator->createParticleSystem("tempParticleSystem", Ogre::StringConverter::parseInt(propertyValue));

									// make a copy but use quota of tempPSys (request copyQuota = false)
									cloneParticleSystem(tempPSys, system, false);

									// Remove the current system.
									unloadPsystem(system, mParticleSystemTemplateName);

									// Create a new system based on the temp system, with the same name as the currently selected system.
									Ogre::ParticleSystem* newPSys =
                                        creator->createParticleSystem(systemName, Ogre::StringConverter::parseInt(propertyValue));

									// Copy the parameters from temp to new
									cloneParticleSystem(newPSys, tempPSys);

									// Destroy the temporary system
									OGRE_DELETE tempPSys;
									tempPSys = NULL;

									// Attach the new version of the system to the main node.
                                    if (node)
									    node->attachObject(newPSys);

									static_cast<Fairy::ParticleElement *>(element)->setParticleSystem(newPSys);

                                    newPSys->setRenderQueueGroup(Ogre::RENDER_QUEUE_8);

									if (!isTemplateSetting)
									{
										mParticleSystem->setParameter(propertyName, propertyValue);
										isTemplateSetting = true;
									}

								}
								else if (propertyName == "renderer")
								{
									system->setParameter(propertyName, propertyValue);

									if (propertyValue == "mesh")
									{
										system->setDefaultDimensions(1.0f, 1.0f);
										// ɾ�����е�Ӱ����
										system->removeAllAffectors();
									}

									if (!isTemplateSetting)
									{
										mParticleSystem->setParameter(propertyName, propertyValue);

										if (propertyValue == "mesh")
										{
											mParticleSystem->setDefaultDimensions(1.0f, 1.0f);
											// ɾ�����е�Ӱ����
											mParticleSystem->removeAllAffectors();
										}

										isTemplateSetting = true;
									}
									InitParticleProperty();
								}
								else
								{						
									system->setParameter(propertyName, propertyValue);

									if (!isTemplateSetting)
									{
										mParticleSystem->setParameter(propertyName, propertyValue);
										isTemplateSetting = true;
									}
								}

								break;
							}

						case Renderer:
							{
								system->getRenderer()->setParameter(propertyName, propertyValue);

								if (!isTemplateSetting)
								{
									mParticleSystem->getRenderer()->setParameter(propertyName, propertyValue);
									isTemplateSetting = true;
								}
								break;
							}

						default:
							{
								// affector
								if ( index > (Renderer + numOfEmitters) )
								{
									index = index - Renderer - numOfEmitters - 1;

									system->getAffector(index)->setParameter(propertyName, propertyValue);

									if (!isTemplateSetting)
									{
										mParticleSystem->getAffector(index)->setParameter(propertyName, propertyValue);
										isTemplateSetting = true;
									}
								}
								// emitter
								else
								{
									index = index - Renderer - 1;

									system->getEmitter(index)->setParameter(propertyName, propertyValue);

									if (!isTemplateSetting)
									{
										mParticleSystem->getEmitter(index)->setParameter(propertyName, propertyValue);
										isTemplateSetting = true;
									}
								}

								break;
							}
						}
					}
				}
			}
		}
	}

	//InitParticleProperty();
}

void ParticleEditorDialog::cloneParticleSystem(Ogre::ParticleSystem* newPSys, const Ogre::ParticleSystem* const clonedPSys, bool copyQuota)
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

void ParticleEditorDialog::unloadPsystem(Ogre::ParticleSystem *system, const std::string& templateName)
{	
    if(system)
	{
		OGRE_DELETE system;
		system = NULL;
	}
}

void ParticleEditorDialog::OnPropertyGridSelect ( wxPropertyGridEvent& event )
{
	//wxPGId id = event.GetProperty();

	//if ( true == id.IsOk() && false == mPropertyGridManager->IsPropertyEnabled(id) )
	//{
	//	//mPropertyGridManager->GetGrid()->CollapseAll();
	//	mPropertyGridManager->Expand(id);
	//}

	mPropertyGridManager->OnPropertyGridSelect(event);
}

void 
ParticleEditorDialog::InitParticleEditor( Ogre::ParticleSystem *pSystem, Ogre::String &templateName,
										   Fairy::SceneManipulator *manipulator )
{
	assert ( pSystem && manipulator );

	mParticleSystem = pSystem;
	mParticleSystemTemplateName = templateName;
	mSceneManipulator = manipulator;	

	if ( ( NULL == mEmitterComboBox) && ( NULL == mAffectorComboBox ) )
	{
		mEmitterComboBox = wxDynamicCast(this->FindWindow(ID_COMBO_EMITTER), wxComboBox);
		mAffectorComboBox = wxDynamicCast(this->FindWindow(ID_COMBO_AFFECTOR), wxComboBox);

		assert (mAffectorComboBox && mEmitterComboBox);

		Ogre::ParticleSystemManager::ParticleEmitterFactoryIterator eit = Ogre::ParticleSystemManager::getSingleton().getEmitterFactoryIterator();

		while (eit.hasMoreElements())
		{
			Ogre::ParticleEmitterFactory* eFactory = eit.getNext();

			mEmitterComboBox->AppendString( eFactory->getName().c_str() );
		}

		mEmitterComboBox->SetSelection(0);

		// ���������ļ��ų������õ�affector
		Ogre::ParticleSystemManager::ParticleAffectorFactoryIterator ait = Ogre::ParticleSystemManager::getSingleton().getAffectorFactoryIterator();

		std::vector<Ogre::String> valueList;

		valueList.clear();

		if (mConfig && mConfig->exists( "unused_particle_system_affector", "list" ))
		{
			valueList = mConfig->getList( "unused_particle_system_affector", "list", "," );
		}

		while (ait.hasMoreElements())
		{
			Ogre::ParticleAffectorFactory* aFactory = ait.getNext();

			Ogre::String affectorName = aFactory->getName();

			bool unused = false;

			for (size_t i = 0; i < valueList.size(); ++i)
			{
				if (affectorName == valueList[i])
				{
					unused = true;
					break;
				}
			}

			if (unused)
				continue;

			mAffectorComboBox->AppendString( affectorName.c_str() );
		}

		mAffectorComboBox->SetSelection(0);
	}	

	if ( NULL == mInsertCheckBox )
	{
		mInsertCheckBox = wxDynamicCast(this->FindWindow(ID_CHECKBOX_INSERT), wxCheckBox);
		assert (mInsertCheckBox);
	}

	InitParticleProperty();

	SetTitle(mParticleSystemTemplateName.c_str());
}

void ParticleEditorDialog::InitParticleProperty(void)
{
	int selectedPos = mPropertyGridManager->GetScrollPos(wxVERTICAL);
	int selectedPos1 = mPropertyGridManager->GetScrollPos(wxHORIZONTAL);

	mPropertyGridManager->Freeze();
	mPropertyGridManager->GetGrid()->Clear();

	// particle system option

	wxPGId id =	mPropertyGridManager->Append( wxCustomProperty("BasicSetting") );

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

	Ogre::ParticleSystemRenderer *renderer = mParticleSystem->getRenderer();

	assert (renderer);

	id = mPropertyGridManager->Append( wxCustomProperty(renderer->getType().c_str()) );

	if ( id.IsOk() )
	{
		mPropertyGridManager->DisableProperty(id);

		mPropertyGridManager->SetPropertyColour(id, *wxBLUE);

		const Ogre::ParameterList &paraList = renderer->getParameters();

		for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
		{
			Ogre::String value = renderer->getParameter(paraList[paraCount].name);

			itemId = mPropertyGridManager->AppendIn( id, CreateProperty(paraList[paraCount], value) );

			mPropertyGridManager->SetPropertyColourToDefault(itemId);

		}
	}

	for ( unsigned short i=0; i<mParticleSystem->getNumEmitters(); ++i )
	{
		Ogre::ParticleEmitter *emitter = mParticleSystem->getEmitter(i);
		assert (emitter);

		id = mPropertyGridManager->Append( wxCustomProperty(emitter->getType().c_str()) );

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

		id = mPropertyGridManager->Append( wxCustomProperty(affector->getType().c_str()) );

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

	mPropertyGridManager->SetScrollPos(wxVERTICAL, selectedPos);
	mPropertyGridManager->SetScrollPos(wxHORIZONTAL, selectedPos1);

}

wxPGProperty*
ParticleEditorDialog::CreateProperty(const Ogre::ParameterDef& propertyDef, Ogre::String &value)
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
                    // ������combobox
                    property = wxEnumProperty(name, name, *constants);
                    // ���óɵ�ǰ���������ֵ
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

				constants->Add("true");
				constants->Add("false");

				registerConstants(constants);
			}

			if (constants)
			{
				// ������combobox
				property = wxEnumProperty(name, name, *constants);
				// ���óɵ�ǰ���������ֵ
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

void
ParticleEditorDialog::registerConstants(wxPGConstants* constants)
{
	assert(constants);
	constants->Ref();
	mConstantList.push_back(constants);
}

wxPGConstants * ParticleEditorDialog::PopulateConstants(const Ogre::String& name)
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

		// �ȿ��������ļ�����û�и��ӵ�ѡ��
		if (mConfig && mConfig->exists( name, "combo" ))
		{
			std::vector<Ogre::String> valueList;

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

		registerConstants(constants);
//	}

	return constants;
}

void ParticleEditorDialog::OnSaveButtonDown(wxCommandEvent &e)
{
	SaveAsParticleDialog *dlg = new SaveAsParticleDialog(this, wxID_ANY,
		_("Save Particle System"), mParticleSystem, mParticleSystemTemplateName);

	bool ok = (dlg->ShowModal() == wxID_OK);
	if(ok)
	{		
		const Ogre::String templateName = dlg->mTextCtrl->GetValue().c_str();

		std::ofstream outFile;

		Ogre::String pathFileName = EFFECT_PATH+templateName+".particle";
		outFile.open ( pathFileName.c_str(), std::ios::out | std::ios::trunc ); // append to file

		if (!outFile.is_open())
		{
			return;
		}

		Ogre::ParticleSystem *system = Ogre::ParticleSystemManager::getSingleton().getTemplate(templateName);

		if (system == NULL)
		{
			system = Ogre::ParticleSystemManager::getSingleton().createTemplate(templateName,"General");

			*system = *mParticleSystem;
		}

		// �����е�ģ�嶼д����ļ���
		saveParticleSystem(mParticleSystem, templateName, outFile );

		mParticleSystemTemplateName = templateName;

		outFile.close();
	}

	dlg->Destroy();
}

void ParticleEditorDialog::saveParticleSystem(Ogre::ParticleSystem *particleSystem, const Ogre::String &templateName, std::ofstream &of)
{
	// ���������ϵͳ�����ƣ������У����{
	of << templateName.c_str();
	of << "\n{\n";

	// �����������
	const Ogre::ParameterList& list = particleSystem->getParameters();

	for ( size_t i=0; i< list.size(); ++i )
	{
		of << '\t';
		of << list[i].name.c_str();
		of << ' ';

		/*if ( list[i].name == "common_direction" &&
			particleSystem->getParameter( "common_direction" ) == "-1.07374e+008 -1.07374e+008 -1.07374e+008" )
			of << "0.0 -1.0 0.0";
		else*/
			of << particleSystem->getParameter(  list[i].name ).c_str();

		of << '\n';
	}

	of << '\n';

	// ���renderer����
	const Ogre::ParameterList &renderParaList = particleSystem->getRenderer()->getParameters();

	for ( size_t i=0; i< renderParaList.size(); ++i )
	{
		of << '\t';
		of << renderParaList[i].name.c_str();
		of << ' ';

		/*if ( renderParaList[i].name == "common_direction" &&
			particleSystem->getRenderer()->getParameter( "common_direction" ) == "-1.07374e+008 -1.07374e+008 -1.07374e+008" )
			of << "0.0 -1.0 0.0";
		else*/
			of << particleSystem->getRenderer()->getParameter(  renderParaList[i].name ).c_str();

		of << '\n';
	}

	of << '\n';

	// ���emitter
	unsigned short numOfEmitter = particleSystem->getNumEmitters();

	Ogre::ParticleEmitter *pEmitter = NULL;

	// �������е�emitter
	for ( unsigned short j=0; j<numOfEmitter; ++j )
	{
		pEmitter = particleSystem->getEmitter(j);

		const Ogre::ParameterList& list = pEmitter->getParameters();

		of << "\temitter " << pEmitter->getType().c_str() << "\n\t{\n\t";

		// �������emitter�Ĳ���
		for ( size_t z=0; z<list.size(); ++z )
		{
			of << '\t' << list[z].name.c_str() << ' ' << pEmitter->getParameter( list[z].name ).c_str() << "\n\t";
		}

		// ������emitter��}
		of << "}\n\n";
	}

	// ���affector
	unsigned short numOfAffector = particleSystem->getNumAffectors();

	Ogre::ParticleAffector *pAffector = NULL;

	// �������е�affector
	for ( unsigned short ii=0; ii<numOfAffector; ++ii )
	{
		pAffector = particleSystem->getAffector(ii);

		const Ogre::ParameterList& list = pAffector->getParameters();

		of << "\taffector " << pAffector->getType().c_str() << "\n\t{\n\t";

		// ������affector�Ĳ���
		for ( size_t jj=0; jj<list.size(); ++jj )
		{
			of << '\t' << list[jj].name.c_str() << ' ' << pAffector->getParameter( list[jj].name ).c_str() << "\n\t";
		}

		// ������affector��}
		of << "}\n\n";
	}

	// ����������ϵͳ��}
	of << "}\n\n";
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC_CLASS(SaveAsParticleDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(SaveAsParticleDialog, wxDialog)
EVT_INIT_DIALOG(SaveAsParticleDialog::OnInitDialog)
END_EVENT_TABLE()

void
SaveAsParticleDialog::OnInitDialog(wxInitDialogEvent &e)
{
	wxDialog::OnInitDialog(e);

	wxFlexGridSizer *item0 = new wxFlexGridSizer( 2, 0, 0 );

	wxStaticText *item1 = new wxStaticText( this, ID_TEXT_SAVE_AS_PARTICLE, wxT("Script File"), wxDefaultPosition, wxDefaultSize, 0 );
	item0->Add( item1, 0, wxALIGN_CENTER|wxALL, 5 );

	wxString *strs2 = (wxString*) NULL;
	wxComboBox *item2 = new wxComboBox( this, ID_COMBO_SAVE_AS_PARTICLE, wxT(""), wxDefaultPosition, wxSize(200,-1), 0, strs2, wxCB_DROPDOWN );
	item0->Add( item2, 0, wxALIGN_CENTER|wxALL, 5 );

	wxStaticText *item3 = new wxStaticText( this, ID_TEXT_SAVE_AS_PARTICLE, wxT("Template Name"), wxDefaultPosition, wxDefaultSize, 0 );
	item0->Add( item3, 0, wxALIGN_CENTER|wxALL, 5 );

	wxTextCtrl *item4 = new wxTextCtrl( this, ID_TEXTCTRL_SAVE_AS_PARTICLE, wxT(""), wxDefaultPosition, wxSize(200,-1), 0 );
	item0->Add( item4, 0, wxALIGN_CENTER|wxALL, 5 );

	wxButton *item5 = new wxButton( this, wxID_OK, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	item0->Add( item5, 0, wxALIGN_CENTER|wxALL, 5 );

	wxButton *item6 = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	item0->Add( item6, 0, wxALIGN_CENTER|wxALL, 5 );

	this->SetSizer( item0 );
	item0->SetSizeHints( this );

	/*std::vector<Ogre::String> loadedScripts;
	Fairy::EffectManager::getSingleton().getLoadedScripts( loadedScripts );

	for ( size_t i=0; i<loadedScripts.size(); ++i )
	{
		if (loadedScripts[i] != "EmptyEffect.effect")
			item2->AppendString(loadedScripts[i]);
	}*/

	//Ogre::String fileName = m

	//if ( fileName == "EmptyEffect.effect" )
	//	item2->SetSelection(0);
	//else
	//	item2->SetValue(fileName);

	mComboBox = item2;

	//if (mEffect->getTemplateName() != "EffectEmpty")
	//	item4->SetValue(mEffect->getTemplateName());
	item4->SetValue(mParticleSystemName);

	mTextCtrl = item4;
}

void ParticleEditorDialog::OnShowMaterialEditorButtonDown(wxCommandEvent &e)
{
	if ( NULL == mMaterialEditorDialog )
	{
		//int width = 0;
		//int height = 0;
		//int x = 0;
		//int y = 0;

		//GetParent()->GetParent()->GetParent()->GetParent()->GetSize(&width, &height);
		//GetParent()->GetParent()->GetParent()->GetParent()->GetPosition(&x, &y);

		mMaterialEditorDialog = new MaterialEditorDialog(this, wxID_ANY, _("���ʱ༭��"));
	}
	if ( false == mMaterialEditorDialog->IsShown() )
		mMaterialEditorDialog->Show();	

	gEffectSettingGlobalData.mCurrentEditEffectType = CEFT_PARTICLE;
	gEffectSettingGlobalData.mCurrentParticleSystem = mParticleSystem;
	gEffectSettingGlobalData.mCurrentParticleTemplateName = mParticleSystemTemplateName;

	mMaterialEditorDialog->InitDialog(mSceneManipulator);
}

