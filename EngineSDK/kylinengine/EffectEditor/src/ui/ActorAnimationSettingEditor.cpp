 #if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "ActorAnimationSettingEditor.h"
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

#include <wx/listbox.h>
#include <wx/colordlg.h>

#include "UIColourConverter.h"

#include "ActorAnimationSettingEditor.h"
#include "WeaponSelectorDialog.h"
#include "MountSettingDialog.h"
#include "WXGraphDialog.h"
#include "SceneManipulator.h"
#include "WXEffectEditorFrame.h"
#include "EffectSystem/FairyEffectManager.h"
#include "EffectSystem/FairySkill.h"
#include "EffectSystem/FairyEffect.h"

#include "WXEffectEditorApp.h"
#include "EffectSettingGlobalData.h"

#include <OgreAnimation.h>
#include <OgreSceneNode.h>
#include <OgreLight.h>
#include <OgreMath.h>
#include "Core/FairyVariant.h"
#include "Core/FairyFlags.h"
#include "Core/FairySceneInfo.h"
#include "Core/FairyPrerequisites.h"
#include "Core/FairyObjectFactoryManager.h"
#include <OgreControllerManager.h>
#include <OgrePredefinedControllers.h>
#include <OgreLogManager.h>
#include <OgreStringConverter.h>
#include <OgreTimer.h>

const int ID_LISTBOX_ANIMATION = 10001;
const int ID_LISTBOX_ALL_SKILL = 10007;
const int ID_BUTTON_LEFTWEAPON = 10011;
const int ID_BUTTON_RIGTHTWEAPON = 10012;
const int ID_BUTTON_MOUNT = 10013;

const int ID_BUTTON_COLOUR = 10024;
const int ID_BUTTON_CLOTH = 10025;
const int ID_SLIDER_TIME = 10026;
const int ID_SLIDER_GLOBAL_ANIM_RATE = 10027;
const int ID_SLIDER_FADE_OUT_TIME = 10028;

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC_CLASS(ActorAnimationSettingEditor, wxPanel)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(ActorAnimationSettingEditor, wxPanel)
	//EVT_SLIDER(ID_SLIDER_TIME, ActorAnimationSettingEditor::OnSliderTimePosUpdate)
	//EVT_SLIDER(ID_SLIDER_FADE_OUT_TIME, ActorAnimationSettingEditor::OnSliderAnimFadeOutTimeUpdate)
	EVT_LISTBOX(ID_LISTBOX_ANIMATION, ActorAnimationSettingEditor::OnListBoxAnimationDoubleClick)
	//EVT_LISTBOX(ID_LISTBOX_ALL_SKILL, ActorAnimationSettingEditor::OnListBoxAllSkillDoubleClick)
	//EVT_UPDATE_UI(ID_SLIDER_GLOBAL_ANIM_RATE, ActorAnimationSettingEditor::OnUpdateAnimRate)
	

	//EVT_BUTTON(ID_BUTTON_LEFTWEAPON, ActorAnimationSettingEditor::OnLeftWeaponButtonDown)
	//EVT_BUTTON(ID_BUTTON_RIGTHTWEAPON, ActorAnimationSettingEditor::OnRightWeaponButtonDown)

 //   EVT_BUTTON(ID_BUTTON_MOUNT, ActorAnimationSettingEditor::OnMountButtonDown)

END_EVENT_TABLE()

// full ctor
ActorAnimationSettingEditor::ActorAnimationSettingEditor(wxWindow *parent, wxWindowID id,
							const wxPoint& pos,const wxSize& size,long style)
							: wxPanel(parent, id,pos, size, style),
							mListBoxAnimation(NULL),mSceneManipulator(NULL),
							mListBoxAllSkill(NULL), mAnimationFadeOutTime(0.3f),
							mSliderAnimFadeOutTime(NULL),mDObject(NULL),mTargetDObject(NULL),
							mWeaponSelectorDialog(NULL), mMountSettingDialog(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(1);
	sizer->AddGrowableCol(0);
	sizer->AddGrowableRow(1);

	m_toolBar = new wxToolBar( this, wxID_ANY, wxDefaultPosition, wxSize(-1,28)); 
	m_toolBar->Realize();

	sizer->Add( m_toolBar, 0, wxEXPAND, 0 );

	mListBoxAnimation =  new wxListBox( this, ID_LISTBOX_ANIMATION, wxDefaultPosition, wxDefaultSize, 0,NULL, wxLB_SINGLE );

	sizer->Add( mListBoxAnimation, 0, wxGROW|wxALL, 0 );

	this->SetSizer( sizer );
	sizer->SetSizeHints(this);
}
void ActorAnimationSettingEditor::init()
{

	wxGraphDialog* pGraphDialog = m_Frame->GetGraphDialog();
	if(pGraphDialog)
		pGraphDialog->SetLoop(mDObject->getAnimationLoop());

	this->SetTitle(Fairy::VariantCast<Ogre::String>(mObject->getProperty("actor name")));

	//重新设置动画列表框

	mListBoxAnimation->Clear();

	for ( unsigned short i = 0; i < mDObject->getSkeletonAnimationCount(); ++i )
	{
		Ogre::Animation *anim = mDObject->getSkeletonAnimation(i);

		if ( anim )
		{
			wxString nameStr(anim->getName().c_str());
			mListBoxAnimation->InsertItems( 1, &nameStr, 0 );            
		}
	}
	// 加入obj中定义的动作名称
	Fairy::LogicModelManager::AnimationSkeletonLinksIterator asmIt = 
		Fairy::LogicModelManager::getSingleton().getAnimationSkeletonLinksIterator(mDObject->getName());

	while ( asmIt.hasMoreElements() )
	{
		wxString animName(asmIt.peekNextKey().c_str());

		if ( wxNOT_FOUND == mListBoxAnimation->FindString(animName) )
		{
			wxString nameStr(animName.c_str());
			mListBoxAnimation->InsertItems( 1, &nameStr, 0 );
		}

		asmIt.moveNext();
	}

	//重新设置声音
	if ( NULL == Fairy::LogicModel::GetPlaySoundFuncton() )
	{
		Fairy::LogicModel::SetPlaySoundFuncton(wxGetApp().funcPlay);
	}

	if ( NULL == Fairy::LogicModel::GetStopSoundFunction() )
		Fairy::LogicModel::SetStopSoundFunction(wxGetApp().funcStop);


   
}
ActorAnimationSettingEditor::~ActorAnimationSettingEditor()
{
}
void ActorAnimationSettingEditor::SetCurrentObject(Ogre::String& name)
{
	if(mObject)
	{
		GetSceneManipulator()->deselectObject(mObject);
		mObject.reset();
	}
	mParameters["actor name"] = name;
	mObject  = Fairy::ObjectFactoryManager::getSingleton().createInstance("Actor");
	mObject->setPropertyList(mParameters);
	mObject->createRenderInstance(GetSceneManipulator());

	Ogre::Vector3 position = Ogre::Vector3::ZERO;
	mObject->setProperty("position", position);
	GetSceneManipulator()->selectObject(mObject);
	Fairy::ActorObject* pActorObject = dynamic_cast<Fairy::ActorObject*>(mObject.get());
	mDObject  = pActorObject->getActor()->getImpl();
	init();
}
void ActorAnimationSettingEditor::SetTargetObject(Ogre::String& name)
{
	if(mTargetObject)
	{
		GetSceneManipulator()->deselectObject(mTargetObject);
		mTargetObject.reset();
	}
	mParameters["actor name"] = name;
	mTargetObject  = Fairy::ObjectFactoryManager::getSingleton().createInstance("Actor");
	mTargetObject->setPropertyList(mParameters);
	mTargetObject->createRenderInstance(GetSceneManipulator());

	Ogre::Vector3 position = Ogre::Vector3(0,0,500);
	mTargetObject->setProperty("position", position);
	Fairy::ActorObject* pTargetObject = dynamic_cast<Fairy::ActorObject*>(mTargetObject.get());
	mTargetDObject  = pTargetObject->getActor()->getImpl();
}
Fairy::LogicModel* ActorAnimationSettingEditor::GetActorObject()
{
	Fairy::ActorObject* pActorObject = dynamic_cast<Fairy::ActorObject*>(mObject.get());
	return pActorObject->getActor()->getImpl();
}
Fairy::LogicModel* ActorAnimationSettingEditor::GetTargetObject()
{
	Fairy::ActorObject* pTargetObject = dynamic_cast<Fairy::ActorObject*>(mTargetObject.get());
	return pTargetObject->getActor()->getImpl();
}

void ActorAnimationSettingEditor::OnListBoxAnimationDoubleClick( wxCommandEvent &event )
{
	if (mListBoxAnimation->GetSelection() == wxNOT_FOUND)
		return;

	mDObject->delCurrentSkill();

	Ogre::String animName = mListBoxAnimation->GetStringSelection().c_str();

	assert ( !animName.empty() );

	mDObject->createSkill( animName,m_Frame->GetGraphDialog()->GetLoop(), true, mAnimationFadeOutTime);
	m_Frame->GetGraphDialog()->InitAnimTimePosSlider();
}


void ActorAnimationSettingEditor::OnListBoxAllSkillDoubleClick( wxCommandEvent &event )
{
	if (mListBoxAllSkill->GetSelection() == wxNOT_FOUND)
		return;

	//mDObject->createSkill( mListBoxAllSkill->GetStringSelection().c_str(), mCheckBoxLoop->GetValue(), false, mAnimationFadeOutTime);

	//InitAnimTimePosSlider();

	mDObject->setAnimEditing(false);

	gEffectSettingGlobalData.mCurrentSkill = mDObject->getCurrentSkill();
}


void ActorAnimationSettingEditor::OnUpdateDelSkillButtonDown(wxUpdateUIEvent& e)
{
	bool enable = false;

	if ( mDObject && mListBoxAllSkill && mDObject->getCurrentSkill() &&
		( mListBoxAllSkill->GetSelection() != wxNOT_FOUND ) && 
		mListBoxAllSkill->GetStringSelection() == mDObject->getCurrentSkill()->getSkillName() )
	{
		enable = true;
	}
	e.Enable(enable);
}

void ActorAnimationSettingEditor::OnUpdateSetSkillButtonDown(wxUpdateUIEvent& e)
{
	bool enable = false;

	if ( mDObject && mListBoxAllSkill && mDObject->getCurrentSkill() &&
		( mListBoxAllSkill->GetSelection() != wxNOT_FOUND ) && 
		mListBoxAllSkill->GetStringSelection() == mDObject->getCurrentSkill()->getSkillName() )
	{
		enable = true;
	}
	e.Enable(enable);
}


void ActorAnimationSettingEditor::OnLeftWeaponButtonDown(wxCommandEvent &e)
{    
    if (NULL == mWeaponSelectorDialog)
    {
        mWeaponSelectorDialog = new WeaponSelectorDialog(this, wxID_ANY, _("Weapon"), mDObject );
    }

    mWeaponSelectorDialog->setEquipHand(true);

    if ( false == mWeaponSelectorDialog->IsShown() )
        mWeaponSelectorDialog->Show();
}

void ActorAnimationSettingEditor::OnRightWeaponButtonDown(wxCommandEvent &e)
{
    if (NULL == mWeaponSelectorDialog)
    {
        mWeaponSelectorDialog = new WeaponSelectorDialog(this, wxID_ANY, _("Weapon"), mDObject );
    }

    mWeaponSelectorDialog->setEquipHand(false);

    if ( false == mWeaponSelectorDialog->IsShown() )
        mWeaponSelectorDialog->Show();
}

void ActorAnimationSettingEditor::OnSliderAnimFadeOutTimeUpdate( wxCommandEvent &event )
{
	int value = mSliderAnimFadeOutTime->GetValue();
	mAnimationFadeOutTime = (float)value / 10.0f;
}


void ActorAnimationSettingEditor::OnMountButtonDown(wxCommandEvent &e)
{
    if (NULL == mMountSettingDialog)
    {
        mMountSettingDialog = new MountSettingDialog(this, wxID_ANY, _("MountSetting"), mDObject );
    }

    if ( false == mMountSettingDialog->IsShown() )
        mMountSettingDialog->Show();
}

void ActorAnimationSettingEditor::ChangeObjectAttribute(const Ogre::String&name, const Ogre::String& value)
{
    try
    {
        mDObject->changeAttribute(name, value);
    }
    catch (Ogre::Exception& e)
    {
        Ogre::LogManager::getSingleton().logMessage(e.getFullDescription());
    }
}