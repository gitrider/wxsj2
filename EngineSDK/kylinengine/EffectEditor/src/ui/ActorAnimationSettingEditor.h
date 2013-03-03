/********************************************************************
filename:   ActorAnimationSettingEditor.h

purpose:    the dialog that set the animation and skill.
*********************************************************************/

#ifndef __ACTORANIMATIONSETTINGDIALOG_H__
#define __ACTORANIMATIONSETTINGDIALOG_H__

#include <wx/dialog.h>
#include <wx/timer.h>
#include <wx/panel.h>
#include <wx/listbox.h>
#include "Core/FairyObject.h"
#include "Core/FairyActorObject.h"
#include "Core/FairyActor.h"
#include <map>
#include <OgreController.h>
#include <OgreColourValue.h>
#include <OgreCommon.h>
#include "ModelSystem/FairyLogicModel.h"

class wxListBox;
class wxSlider;
class wxButton;

class FairySkillEditDialog;
class WeaponSelectorDialog;
class MountSettingDialog;
class EffectEditorFrame;


namespace Fairy	{
	class SceneManipulator;
}



bool GetEffectName(const char* pAnimationName, Ogre::String& effectName);

//////////////////////////////////////////////////////////////////////////
class ActorAnimationSettingEditor : public wxPanel
{
	DECLARE_DYNAMIC_CLASS(ActorAnimationSettingEditor)

public:
	ActorAnimationSettingEditor() {}

	ActorAnimationSettingEditor(wxWindow *parent, wxWindowID id,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL);

	~ActorAnimationSettingEditor();

	
	Fairy::LogicModel *GetCurrentObject(void)
	{
		return mDObject;
	}
	void SetCurrentObject(Ogre::String& name);
	void SetTargetObject(Ogre::String& name);
	void SetSceneManipulator(Fairy::SceneManipulator* sceneManipulator){mSceneManipulator = sceneManipulator;}

	Fairy::SceneManipulator* GetSceneManipulator(void) const {return mSceneManipulator;}

	void SetParentFrame(EffectEditorFrame* frame){m_Frame = frame;}
	EffectEditorFrame* GetParentFrame(){ return m_Frame;}

	Fairy::LogicModel* GetActorObject();
	Fairy::LogicModel* GetTargetObject();
		
	void init();

public: 

	wxListBox *mListBoxAnimation;
	wxSlider  *mSliderAnimRate;
	wxSlider  *mSliderAnimFadeOutTime;

	wxListBox *mListBoxAllSkill;
	
	wxTimer   mTimer;

protected:

    void ChangeObjectAttribute(const Ogre::String&name, const Ogre::String& value);

	DECLARE_EVENT_TABLE()
	
	
	void OnSliderAnimFadeOutTimeUpdate( wxCommandEvent &event );

	void OnListBoxAnimationDoubleClick( wxCommandEvent &event );
	void OnListBoxAllSkillDoubleClick( wxCommandEvent &event );

	

	void OnUpdateDelSkillButtonDown(wxUpdateUIEvent& e);
	void OnUpdateSetSkillButtonDown(wxUpdateUIEvent& e);
	void OnUpdateAnimRate( wxUpdateUIEvent &e );

	void OnCheckBoxLoop( wxCommandEvent &event );
	void OnUpdateCheckBoxLoop(wxUpdateUIEvent& e);

	void OnLeftWeaponButtonDown(wxCommandEvent &e);
	void OnRightWeaponButtonDown(wxCommandEvent &e);

    void OnMountButtonDown(wxCommandEvent &e);

protected:

	Fairy::SceneManipulator *mSceneManipulator;
	EffectEditorFrame* m_Frame;

	Fairy::LogicModel *mDObject;
	Fairy::ObjectPtr mObject;

	Fairy::LogicModel *mTargetDObject;
	Fairy::ObjectPtr mTargetObject;
	Fairy::PropertyNameStringPairList mParameters;


	float mAnimationFadeOutTime;

	wxToolBar* m_toolBar;
    wxButton* mColourButton;

    WeaponSelectorDialog* mWeaponSelectorDialog;
    MountSettingDialog* mMountSettingDialog;
};

#endif