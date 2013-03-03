
#include "BulletEditor.h"
#include "BulletTree.h"
#include "EffectObjectPropertyEditor.h"
#include "Core/FairyObjectFactoryManager.h"
#include "EffectSystem/FairyEffectElement.h"
#include "EffectSystem/FairyParticleElement.h"
#include "EffectSystem/FairyProjectorElement.h"
#include "EffectSystem/FairyBulletFlowSystemManager.h"
#include "EffectSystem/FairyBulletOperatorFactory.h"
#include "EffectSystem/FairyAnimationEffectInfo.h"
#include "EffectSystem/FairyAnimationCameraShake.h"
#include "EffectSystem/FairyAnimationRibbon.h"
#include "EffectSystem/FairyAnimationSound.h"
#include "EffectSystem/FairyAnimationSceneLightInfo.h"
#include "EffectSystem/FairyAnimationLine.h"
#include "res/EffectObjectEditor_wdr.h"
#include "BulletSelector.h"
#include "ActorAnimationSettingEditor.h"
#include "ParticleSystemSelector.h"
#include "OgreParticleSystem.h"
#include "OgreParticleSystemRenderer.h"
#include "OgreParticleSystemManager.h"
#include "OgreMaterialManager.h"
#include "MaterialEditor.h"
#include <wx/image.h>
#include <wx/busyinfo.h>
#include "EffectSystem/FairyAnimationBulletFlow.h"
#include "EffectSystem/FairyBulletFlowSystem.h"
#include "EffectSystem/FairyBulletSystemManager.h"
#include "WXEffectEditorApp.h"
#include "OgreRoot.h"
#include "OgreTimer.h"
#include "SkillObjectEditor.h"
#include <OgreStringVector.h>
///////////////////////////////////////////////////////////////////////////

#if wxUSE_STL
#define AS_STRING(s)    (s)
#else
#define AS_STRING(s)    ((s).c_str())
#endif


IMPLEMENT_DYNAMIC_CLASS(SaveAsBulletDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(SaveAsBulletDialog, wxDialog)
EVT_INIT_DIALOG(SaveAsBulletDialog::OnInitDialog)
END_EVENT_TABLE()

void
SaveAsBulletDialog::OnInitDialog(wxInitDialogEvent &e)
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer4->SetFlexibleDirection( wxBOTH );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxStaticText* staticText1 = new wxStaticText( this, wxID_ANY, wxT("脚本文件:"), wxDefaultPosition, wxSize( 60,-1 ), 0 );

	fgSizer4->Add( staticText1, 0, wxALL, 5 );

	mComboBox = new wxComboBox( this, wxID_ANY,wxEmptyString, wxDefaultPosition, wxSize( 200,-1 ), 0, NULL, 0 ); 
	fgSizer4->Add( mComboBox, 0, wxALL, 5 );

	wxStaticText* staticText2 = new wxStaticText( this, wxID_ANY, wxT("模板名称:"), wxDefaultPosition, wxDefaultSize, 0 );

	staticText2->SetMinSize( wxSize( 60,-1 ) );

	fgSizer4->Add( staticText2, 0, wxALL, 5 );

	mTextCtrl = new wxTextCtrl( this, wxID_ANY,wxT(""), wxDefaultPosition, wxSize( 200,-1 ), 0 );
	fgSizer4->Add( mTextCtrl, 0, wxALL, 5 );

	bSizer13->Add( fgSizer4, 1, wxEXPAND, 5 );

	wxStdDialogButtonSizer* m_sdbSizer5 = new wxStdDialogButtonSizer();
	wxButton* m_sdbSizer5OK = new wxButton( this, wxID_OK );
	m_sdbSizer5->AddButton( m_sdbSizer5OK );
	wxButton* m_sdbSizer5Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer5->AddButton( m_sdbSizer5Cancel );
	m_sdbSizer5->Realize();
	bSizer13->Add( m_sdbSizer5, 1, wxEXPAND, 5 );

	this->SetSizer( bSizer13 );
	this->Layout();

	//std::vector<Ogre::String> loadedScripts;
	//Fairy::BulletFlowSystemManager::getSingleton().getLoadedScripts( loadedScripts );

	//for ( size_t i=0; i<loadedScripts.size(); ++i )
	//{
	//	if (loadedScripts[i] != "EmptyBullet.bullet")
	//		mComboBox->AppendString(loadedScripts[i]);
	//}

	//Ogre::String fileName = Fairy::EffectManager::getSingleton().getFileNameByTemplateName(mBulletFlow->getTemplateName());

	//if ( fileName == "EmptyBullet.bullet" )
	//	mComboBox->SetSelection(0);
	//else
	//	mComboBox->SetValue(fileName);
}

IMPLEMENT_DYNAMIC_CLASS(SaveBulletDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(SaveBulletDialog, wxDialog)
EVT_INIT_DIALOG(SaveBulletDialog::OnInitDialog)
END_EVENT_TABLE()

void
SaveBulletDialog::OnInitDialog(wxInitDialogEvent &e)
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer4->SetFlexibleDirection( wxBOTH );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxStaticText* staticText1 = new wxStaticText( this, wxID_ANY, wxT("脚本文件:"), wxDefaultPosition, wxSize( 60,-1 ), 0 );

	fgSizer4->Add( staticText1, 0, wxALL, 5 );

	mComboBox = new wxComboBox( this, wxID_ANY,wxEmptyString, wxDefaultPosition, wxSize( 200,-1 ), 0, NULL, 0 ); 
	fgSizer4->Add( mComboBox, 0, wxALL, 5 );

	bSizer13->Add( fgSizer4, 1, wxEXPAND, 5 );

	wxStdDialogButtonSizer* m_sdbSizer5 = new wxStdDialogButtonSizer();
	wxButton* m_sdbSizer5OK = new wxButton( this, wxID_OK );
	m_sdbSizer5->AddButton( m_sdbSizer5OK );
	wxButton* m_sdbSizer5Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer5->AddButton( m_sdbSizer5Cancel );
	m_sdbSizer5->Realize();
	bSizer13->Add( m_sdbSizer5, 1, wxEXPAND, 5 );

	this->SetSizer( bSizer13 );
	this->Layout();

}


IMPLEMENT_DYNAMIC_CLASS(NewBulletEventDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(NewBulletEventDialog, wxDialog)
EVT_INIT_DIALOG(NewBulletEventDialog::OnInitDialog)
END_EVENT_TABLE()

void
NewBulletEventDialog::OnInitDialog(wxInitDialogEvent &e)
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer4->SetFlexibleDirection( wxBOTH );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxStaticText* staticText1 = new wxStaticText( this, wxID_ANY, wxT("事件名称:"), wxDefaultPosition, wxSize( 60,-1 ), 0 );

	fgSizer4->Add( staticText1, 0, wxALL, 5 );

	mTextCtrl = new wxTextCtrl( this, wxID_ANY,wxEmptyString, wxDefaultPosition, wxSize( 200,-1 ), 0 ); 
	fgSizer4->Add( mTextCtrl, 0, wxALL, 5 );

	bSizer13->Add( fgSizer4, 1, wxEXPAND, 5 );

	wxStdDialogButtonSizer* m_sdbSizer5 = new wxStdDialogButtonSizer();
	wxButton* m_sdbSizer5OK = new wxButton( this, wxID_OK );
	m_sdbSizer5->AddButton( m_sdbSizer5OK );
	wxButton* m_sdbSizer5Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer5->AddButton( m_sdbSizer5Cancel );
	m_sdbSizer5->Realize();
	bSizer13->Add( m_sdbSizer5, 1, wxEXPAND, 5 );

	this->SetSizer( bSizer13 );
	this->Layout();

}

IMPLEMENT_DYNAMIC_CLASS(NewBulletOperatorDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(NewBulletOperatorDialog, wxDialog)
EVT_INIT_DIALOG(NewBulletOperatorDialog::OnInitDialog)
END_EVENT_TABLE()

void
NewBulletOperatorDialog::OnInitDialog(wxInitDialogEvent &e)
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer4->SetFlexibleDirection( wxBOTH );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxStaticText* staticText1 = new wxStaticText( this, wxID_ANY, wxT("操作器名称:"), wxDefaultPosition, wxSize( 60,-1 ), 0 );

	fgSizer4->Add( staticText1, 0, wxALL, 5 );

	mComboBox = new wxComboBox( this, wxID_ANY,wxEmptyString, wxDefaultPosition, wxSize( 200,-1 ), 0, NULL, 0 );

	std::map<Ogre::String,Fairy::BulletOperatorFactory*> operatorFactoryMap =  Fairy::BulletFlowSystemManager::getSingleton().getOperatorFactorys();
	std::map<Ogre::String,Fairy::BulletOperatorFactory*>::iterator it = operatorFactoryMap.begin();
	int index = 0;
	while(it != operatorFactoryMap.end())
	{

		Ogre::String factoryName = it->first;
		mComboBox->Insert(factoryName.c_str(),index);
		it ++;
		index ++;
	}
	fgSizer4->Add( mComboBox, 0, wxALL, 5 );

	bSizer13->Add( fgSizer4, 1, wxEXPAND, 5 );

	wxStdDialogButtonSizer* m_sdbSizer5 = new wxStdDialogButtonSizer();
	wxButton* m_sdbSizer5OK = new wxButton( this, wxID_OK );
	m_sdbSizer5->AddButton( m_sdbSizer5OK );
	wxButton* m_sdbSizer5Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer5->AddButton( m_sdbSizer5Cancel );
	m_sdbSizer5->Realize();
	bSizer13->Add( m_sdbSizer5, 1, wxEXPAND, 5 );

	this->SetSizer( bSizer13 );
	this->Layout();

}

IMPLEMENT_DYNAMIC_CLASS(NewBulletSystemDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(NewBulletSystemDialog, wxDialog)
EVT_INIT_DIALOG(NewBulletSystemDialog::OnInitDialog)
END_EVENT_TABLE()

void
NewBulletSystemDialog::OnInitDialog(wxInitDialogEvent &e)
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer4->SetFlexibleDirection( wxBOTH );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxStaticText* staticText1 = new wxStaticText( this, wxID_ANY, wxT("BulletSystem名称:"), wxDefaultPosition, wxSize( 60,-1 ), 0 );

	fgSizer4->Add( staticText1, 0, wxALL, 5 );

	mTextCtrl = new wxTextCtrl( this, wxID_ANY,wxEmptyString, wxDefaultPosition, wxSize( 200,-1 ), 0);

	fgSizer4->Add( mTextCtrl, 0, wxALL, 5 );

	bSizer13->Add( fgSizer4, 1, wxEXPAND, 5 );

	wxStdDialogButtonSizer* m_sdbSizer5 = new wxStdDialogButtonSizer();
	wxButton* m_sdbSizer5OK = new wxButton( this, wxID_OK );
	m_sdbSizer5->AddButton( m_sdbSizer5OK );
	wxButton* m_sdbSizer5Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer5->AddButton( m_sdbSizer5Cancel );
	m_sdbSizer5->Realize();
	bSizer13->Add( m_sdbSizer5, 1, wxEXPAND, 5 );

	this->SetSizer( bSizer13 );
	this->Layout();

}

IMPLEMENT_DYNAMIC_CLASS(AddBulletEffectInfoDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(AddBulletEffectInfoDialog, wxDialog)
EVT_INIT_DIALOG(AddBulletEffectInfoDialog::OnInitDialog)
END_EVENT_TABLE()

void
AddBulletEffectInfoDialog::OnInitDialog(wxInitDialogEvent &e)
{
	wxDialog::OnInitDialog(e);

	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer *item1 = new wxFlexGridSizer( 2, 0, 0 );

	wxComboBox *item2 = new wxComboBox( this, feID_COMBOCTRL_BULLET_EFFECT, wxT(""), wxDefaultPosition, wxSize(200,-1), 0, NULL, wxCB_DROPDOWN );
	item1->Add( item2, 0, wxALIGN_CENTER|wxALL, 5 );

	wxComboBox *item3 = new wxComboBox( this, feID_COMBOCTRL_BULLET_LOCATOR, wxT(""), wxDefaultPosition, wxSize(200,-1), 0, NULL, wxCB_DROPDOWN );
	item1->Add( item3, 0, wxALIGN_CENTER|wxALL, 5 );

	sizer->Add( item1, 0, wxALIGN_CENTER|wxALL, 10 );

	wxFlexGridSizer *item5 = new wxFlexGridSizer( 2, 0, 0 );

	wxButton *item6 = new wxButton( this, wxID_OK, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	item6->SetDefault();
	item5->Add( item6, 0, wxALIGN_CENTER|wxALL, 5 );

	wxButton *item7 = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	item5->Add( item7, 0, wxALIGN_CENTER|wxALL, 5 );

	sizer->Add( item5, 0, wxALIGN_CENTER|wxALL, 10 );

	this->SetSizer( sizer );
	sizer->SetSizeHints( this );

	Fairy::EffectManager::EffectTemplateIterator it = 
		Fairy::EffectManager::getSingleton().getEffectTemplateIterator();

	while ( it.hasMoreElements() )
	{
		item2->AppendString(it.peekNextKey().c_str());

		it.moveNext();
	}

	item2->SetSelection(0);

	BulletEditor* pEditor= static_cast<BulletEditor*>(this->GetParent());
	if(!pEditor)
		return;
	Fairy::LogicModel *pMbject  = pEditor->GetParentFrame()->GetActorSettingEditor()->GetActorObject();
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
						item3->Append(locatorNames[i].c_str());
						break;
					}
				}
			}
		}
	}

	item3->SetSelection(0);
}

IMPLEMENT_DYNAMIC_CLASS(AddBulletRibbonDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(AddBulletRibbonDialog, wxDialog)
EVT_INIT_DIALOG(AddBulletRibbonDialog::OnInitDialog)
END_EVENT_TABLE()

void
AddBulletRibbonDialog::OnInitDialog(wxInitDialogEvent &e)
{
	wxDialog::OnInitDialog(e);

	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer *item1 = new wxFlexGridSizer( 2, 0, 0 );

	wxComboBox *item3 = new wxComboBox( this, feID_COMBOCTRL_BULLET_LOCATOR, wxT(""), wxDefaultPosition, wxSize(200,-1), 0, NULL, wxCB_DROPDOWN );
	item1->Add( item3, 0, wxALIGN_CENTER|wxALL, 5 );

	sizer->Add( item1, 0, wxALIGN_CENTER|wxALL, 10 );

	wxFlexGridSizer *item5 = new wxFlexGridSizer( 2, 0, 0 );

	wxButton *item6 = new wxButton( this, wxID_OK, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	item6->SetDefault();
	item5->Add( item6, 0, wxALIGN_CENTER|wxALL, 5 );

	wxButton *item7 = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	item5->Add( item7, 0, wxALIGN_CENTER|wxALL, 5 );

	sizer->Add( item5, 0, wxALIGN_CENTER|wxALL, 10 );

	this->SetSizer( sizer );
	sizer->SetSizeHints( this );



	BulletEditor* pEditor= static_cast<BulletEditor*>(this->GetParent());
	if(!pEditor)
		return;
	Fairy::LogicModel *pMbject  = pEditor->GetParentFrame()->GetActorSettingEditor()->GetActorObject();
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
						item3->Append(locatorNames[i].c_str());
						break;
					}
				}
			}
		}
	}

	item3->SetSelection(0);
}


IMPLEMENT_DYNAMIC_CLASS(NewBulletDialog, wxDialog)
// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(NewBulletDialog, wxDialog)
EVT_INIT_DIALOG(NewBulletDialog::OnInitDialog)
END_EVENT_TABLE()

void
NewBulletDialog::OnInitDialog(wxInitDialogEvent &e)
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer4->SetFlexibleDirection( wxBOTH );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );


	wxStaticText* staticText2 = new wxStaticText( this, wxID_ANY, wxT("模板名称:"), wxDefaultPosition, wxDefaultSize, 0 );

	staticText2->SetMinSize( wxSize( 60,-1 ) );

	fgSizer4->Add( staticText2, 0, wxALL, 5 );

	mTextCtrl = new wxTextCtrl( this, wxID_ANY,wxT(""), wxDefaultPosition, wxSize( 200,-1 ), 0 );
	fgSizer4->Add( mTextCtrl, 0, wxALL, 5 );

	bSizer13->Add( fgSizer4, 1, wxEXPAND, 5 );

	wxStdDialogButtonSizer* m_sdbSizer5 = new wxStdDialogButtonSizer();
	wxButton* m_sdbSizer5OK = new wxButton( this, wxID_OK );
	m_sdbSizer5->AddButton( m_sdbSizer5OK );
	wxButton* m_sdbSizer5Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer5->AddButton( m_sdbSizer5Cancel );
	m_sdbSizer5->Realize();
	bSizer13->Add( m_sdbSizer5, 1, wxEXPAND, 5 );

	this->SetSizer( bSizer13 );
	this->Layout();

}

IMPLEMENT_DYNAMIC_CLASS(RecoverSaveBulletDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(RecoverSaveBulletDialog, wxDialog)
EVT_INIT_DIALOG(RecoverSaveBulletDialog::OnInitDialog)
END_EVENT_TABLE()

void
RecoverSaveBulletDialog::OnInitDialog(wxInitDialogEvent &e)
{
	wxDialog::OnInitDialog(e);

	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer *item1 = new wxFlexGridSizer( 2, 0, 0 );

	wxStaticText* text= new wxStaticText(this, wxID_ANY, mText, wxDefaultPosition,wxDefaultSize);
	item1->Add( text, 0, wxALIGN_CENTER|wxALL, 5 );

	sizer->Add( item1, 0, wxALIGN_CENTER|wxALL, 10 );

	wxFlexGridSizer *item5 = new wxFlexGridSizer( 2, 0, 0 );

	wxButton *item6 = new wxButton( this, wxID_OK, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	item6->SetDefault();
	item5->Add( item6, 0, wxALIGN_CENTER|wxALL, 5 );

	wxButton *item7 = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	item5->Add( item7, 0, wxALIGN_CENTER|wxALL, 5 );

	sizer->Add( item5, 0, wxALIGN_CENTER|wxALL, 10 );

	this->SetSizer( sizer );
	sizer->SetSizeHints( this );

}


static const int feID_DELETE_BULLET_ELEMENT =wxNewId();
static const int feID_SAVE_BULLET_OBJECT =wxNewId();
static const int feID_NEW_BULLETFLOW =wxNewId();
static const int feID_SAVEAS_BULLET_OBJECT = wxNewId();
static const int feID_NEW_BULLET_EVENT = wxNewId();
static const int feID_NEW_BULLET_OPERATOR = wxNewId();
static const int feID_NEW_BULLETSYSTEM_OBJECT = wxNewId();
static const int feID_NEW_BULLETSYSTEM_EFFECTINFO = wxNewId();
static const int feID_NEW_BULLETSYSTEM_RIBBON = wxNewId();
static const int feID_NEW_BULLETSYSTEM_CAMERASHAKE = wxNewId();
static const int feID_NEW_BULLETSYSTEM_SOUND = wxNewId();
static const int feID_NEW_BULLETSYSTEM_LIGHT = wxNewId();
static const int feID_NEW_BULLETSYSTEM_LINE = wxNewId();


IMPLEMENT_DYNAMIC_CLASS(BulletEditor, wxPanel)

BEGIN_EVENT_TABLE(BulletEditor, wxPanel)
EVT_MENU(feID_DELETE_BULLET_ELEMENT,BulletEditor::OnDeleteBulletElement)
EVT_MENU(feID_SAVEAS_BULLET_OBJECT,BulletEditor::OnSaveAsBulletObject)
EVT_MENU(feID_SAVE_BULLET_OBJECT,BulletEditor::OnSaveBulletObject)
EVT_MENU(feID_NEW_BULLETFLOW,BulletEditor::OnNewBulletFlow)
EVT_MENU(feID_NEW_BULLET_EVENT,BulletEditor::OnNewBulletEvent)
EVT_MENU(feID_NEW_BULLET_OPERATOR,BulletEditor::OnNewBulletOperator)
EVT_MENU(feID_NEW_BULLETSYSTEM_OBJECT,BulletEditor::OnNewBulletSystem)
EVT_MENU(feID_NEW_BULLETSYSTEM_EFFECTINFO,BulletEditor::OnNewBulletEffectInfo)
EVT_MENU(feID_NEW_BULLETSYSTEM_RIBBON,BulletEditor::OnNewBulletRibbon)
EVT_MENU(feID_NEW_BULLETSYSTEM_CAMERASHAKE,BulletEditor::OnNewBulletCameraShake)
EVT_MENU(feID_NEW_BULLETSYSTEM_SOUND,BulletEditor::OnNewBulletSound)
EVT_MENU(feID_NEW_BULLETSYSTEM_LIGHT,BulletEditor::OnNewBulletLight)
EVT_MENU(feID_NEW_BULLETSYSTEM_LINE,BulletEditor::OnNewBulletLine)


END_EVENT_TABLE()


BulletFlowCreateCallback			BulletEditor::s_BulletFlowCreateCallback;
CreatedBulletCallback				BulletEditor::s_CreatedBulletCallback;
DeleteBulletCallback				BulletEditor::s_DeleteBulletCallback;
SkillDestroyCallback				BulletEditor::s_SkillDestroyCallback;
BulletHitTargetCallback				BulletEditor::s_BulletHitTargetCallback;

/*------------------------子弹的引擎回调事件---------------------------*/

// 技能子弹的创建回调
void BulletFlowCreateCallback::onCreateBulletFlow(const Fairy::String& bulletFlowTemplateName, 
												  const Fairy::TransformInfo& transInfo, void* pSkillUserData)
{
	if (pSkillUserData)
	{
		BulletFlowInfo* pBulletFlowInfo = (BulletFlowInfo*)pSkillUserData;
		// 创建一个子弹系统
		Fairy::BulletFlowSystem* pFlow = Fairy::BulletFlowSystemManager::getSingleton().createBulletFlowSystem( bulletFlowTemplateName );

		// 起始位置
		pFlow->setTargetModel(pBulletFlowInfo->targetModel);
		pFlow->setTargetPoint(pBulletFlowInfo->targetPoint);
		pFlow->setOffsetPosition(pBulletFlowInfo->offsetPoistion);
		pFlow->setOffsetRotation(pBulletFlowInfo->offsetRotation);
		pFlow->setCasterModel(pBulletFlowInfo->casterModel);
		pFlow->setAttachPoint(pBulletFlowInfo->attachPoint);
		EffectEditorApp* app = (EffectEditorApp*)(wxTheApp);
		app->GetMainFrame()->GetBulletEditor()->m_ActiveBulleFlows.push_back(pFlow);
	}
}


// 子弹创建回调
void CreatedBulletCallback::onAfterCreatedBulletSystem(Fairy::BulletFlowSystem* bulletFlow, Fairy::BulletSystem* bulletSystem)
{
	if (bulletFlow && bulletSystem && bulletFlow->m_userData)
	{		
		//// 设置目标列表ID
		//INT nID = *((INT*)bulletFlow->m_userData);

		//INT* pID = new INT;
		//*pID = nID;

		//bulletSystem->m_userData = (VOID*)pID;
	}
	
}

// 子弹销毁回调
void DeleteBulletCallback::onBeforeDeleteBulletSystem(Fairy::BulletSystem* bulletSystem)
{
	if (bulletSystem)
	{
	//	SAFE_DELETE(bulletSystem->m_userData);
	}
}

// 技能销毁回调
void SkillDestroyCallback::onDestroySkill(Fairy::Skill* pSkill)
{
	if (pSkill)
	{
		BulletFlowInfo* pBulletFlowInfo = (BulletFlowInfo*)pSkill->m_userData;
		delete pBulletFlowInfo;
		pBulletFlowInfo = NULL;
	}
	/*EffectEditorApp* app = (EffectEditorApp*)(wxTheApp);
	BulletEditor* pEditor = app->GetMainFrame()->GetBulletEditor();
	std::vector<Fairy::BulletFlowSystem*>::iterator it = pEditor->m_ActiveBulleFlows.begin();
	while(it != pEditor->m_ActiveBulleFlows.end())
	{
		Fairy::BulletFlowSystem* pBulletFlow = *it;

		it ++;
	}*/
}

void BulletHitTargetCallback::onHitTargetCallback(Fairy::BulletFlowSystem* bulletFlow,Fairy::BulletSystem* bulletSystem)
{

}

BulletEditor::BulletEditor( wxWindow* parent,
				   wxWindowID id,
				   const wxPoint& pos,
				   const wxSize& size,
				   long style) : mBulletFlowTemplateName(""),mSaveFileName(""),mBulletFlow(NULL), mConfig(NULL),
				   EFFECT_PATH("../resource/media/特效/子弹/"),mbRecoverSave(false),mFrameTime(0),
				   m_createBulletFlowCallback(NULL),m_afterCreatedBulletSystemCallback(NULL),
				   m_beforeDeleteBulletSystemCallback(NULL),m_bulletSystemHitTargetCallback(NULL)
{
	Create(parent, id, pos, size, style);
	

}

bool
BulletEditor::Create(wxWindow *parent, wxWindowID winid,
							   const wxPoint& pos, const wxSize& size,
							   long style)
{
	if (!wxPanel::Create(parent, winid, pos, size, style))
		return false;

	wxFlexGridSizer* sizer = new wxFlexGridSizer(1);
	sizer->AddGrowableCol(0);
	sizer->AddGrowableRow(1);


	m_toolBar = new wxToolBar( this, wxID_ANY, wxDefaultPosition, wxSize(-1,28)); 

	m_toolBar->AddTool(feID_NEW_BULLETFLOW,
		wxBITMAP(NEW),wxBITMAP(NEW),false,
		0,wxDefaultCoord,NULL, wxT("创建子弹数据"),wxT("创建子弹数据"));

	m_toolBar->AddTool(feID_SAVEAS_BULLET_OBJECT,
		wxBITMAP(SAVEAS),wxBITMAP(SAVEAS),false,
		1,wxDefaultCoord,NULL, wxT("另存子弹数据"),wxT("另存子弹数据"));

	m_toolBar->AddTool(feID_SAVE_BULLET_OBJECT,
		wxBITMAP(SAVE),wxBITMAP(SAVE),false,
		2,wxDefaultCoord,NULL, wxT("保存子弹数据"),wxT("保存子弹数据"));	

	m_toolBar->AddTool(feID_DELETE_BULLET_ELEMENT,
		wxBITMAP(DELETE),wxBITMAP(DELETE),false,
		3,wxDefaultCoord,NULL, wxT("删除子弹元素"),wxT("删除子弹元素"));
	
	m_toolBar->AddSeparator();

	m_toolBar->AddTool(feID_NEW_BULLET_EVENT,
		wxBITMAP(NEWEVENT),wxBITMAP(NEWEVENT),false,
		4,wxDefaultCoord,NULL, wxT("新建事件"),wxT("新建事件"));

	m_toolBar->AddTool(feID_NEW_BULLET_OPERATOR,
		wxBITMAP(NEWOPERATOR),wxBITMAP(NEWOPERATOR),false,
		5,wxDefaultCoord,NULL, wxT("新建子操作试器"),wxT("新建子操作试器"));

	m_toolBar->AddSeparator();
	
	m_toolBar->AddTool(feID_NEW_BULLETSYSTEM_OBJECT,
	wxBITMAP(NEWBULLETSYSTEM),wxBITMAP(NEWBULLETSYSTEM),false,
	6,wxDefaultCoord,NULL, wxT("新建子弹系统"),wxT("新建子弹系统"));

	m_toolBar->AddTool(feID_NEW_BULLETSYSTEM_EFFECTINFO,
		wxBITMAP(NEWAINIMEFFECT),wxBITMAP(NEWAINIMEFFECT),false,
		7,wxDefaultCoord,NULL, wxT("新建效果元素"),wxT("新建效果元素"));

	m_toolBar->AddTool(feID_NEW_BULLETSYSTEM_CAMERASHAKE,
		wxBITMAP(NEWCAMERASHAKE),wxBITMAP(NEWCAMERASHAKE),false,
		8,wxDefaultCoord,NULL, wxT("新建相机震动元素"),wxT("新建相机震动元素"));
	
	m_toolBar->AddTool(feID_NEW_BULLETSYSTEM_RIBBON,
		wxBITMAP(NEWRIBBON),wxBITMAP(NEWRIBBON),false,
		9,wxDefaultCoord,NULL, wxT("新建刀光元素"),wxT("新建刀光元素"));

	m_toolBar->AddTool(feID_NEW_BULLETSYSTEM_SOUND,
		wxBITMAP(NEWSOUND),wxBITMAP(NEWSOUND),false,
		10,wxDefaultCoord,NULL, wxT("新建声音元素"),wxT("新建声音元素"));

	m_toolBar->AddTool(feID_NEW_BULLETSYSTEM_LIGHT,
		wxBITMAP(NEWSCENELIGHT),wxBITMAP(NEWSCENELIGHT),false,
		11,wxDefaultCoord,NULL, wxT("新建灯光元素"),wxT("新建灯光元素"));

	m_toolBar->AddTool(feID_NEW_BULLETSYSTEM_LINE,
		wxBITMAP(NEWLINE),wxBITMAP(NEWLINE),false,
		12,wxDefaultCoord,NULL, wxT("新建连线元素"),wxT("新建连线元素"));


	m_toolBar->Realize();
	
	
	sizer->Add( m_toolBar, 0, wxEXPAND, 0 );

	m_BulletObjectTree = new BulletTree( this, ID_BULLET_OBJECT_TREE, wxDefaultPosition, wxDefaultSize);
	sizer->Add( m_BulletObjectTree, 0, wxGROW|wxALL, 0 );

	this->SetSizer( sizer );
	sizer->SetSizeHints(this);

	return true;
}

BulletEditor::~BulletEditor()
{
	std::vector<Fairy::BulletFlowSystem*>::iterator iter = m_ActiveBulleFlows.begin();

	while (iter != m_ActiveBulleFlows.end())
	{

		Fairy::BulletFlowSystem* pBulletFlowSystem = *iter;

		if(pBulletFlowSystem)
		{

			delete pBulletFlowSystem;
			pBulletFlowSystem = NULL;
		
		}
		iter ++;
	}
	m_ActiveBulleFlows.clear();
}


void BulletEditor::InitEffectObject()
{

}
void BulletEditor::InitBulletEditor(Fairy::BulletFlowSystem* pBulletFlow,const Ogre::String &templateName,bool isNew)
{
	mBulletFlowTemplateName = templateName;
	mBulletFlow  = pBulletFlow;
	if(isNew)
	{
		mbRecoverSave = true;
	}
	Ogre::String fileName = Fairy::BulletFlowSystemManager::getSingleton().getFileNameByTemplateName(templateName);
	Ogre::StringVector BulletSystemNames;
	Fairy::BulletSystemManager::getSingleton().getTemplatesFromScriptFile(fileName,BulletSystemNames);
	mBulletSystemMap.clear();
	Ogre::StringVector::iterator it= BulletSystemNames.begin();
	while(it != BulletSystemNames.end())
	{
		Ogre::String templateName = *it;
		Fairy::BulletSystem* pBulletSystem = Fairy::BulletSystemManager::getSingleton().getBulletSystemTemplate(templateName);
		if(pBulletSystem)
		{
			mBulletSystemMap.insert(std::map<Ogre::String,Fairy::BulletSystem*>::value_type(templateName,pBulletSystem));
		}
		it ++;
	}

	m_BulletObjectTree->DeleteAllItems();
	m_BulletObjectTree->AddBulletToTree(mBulletFlow,templateName);
	std::map<Ogre::String,Fairy::BulletSystem*>::iterator bs_it = mBulletSystemMap.begin();
	while(bs_it != mBulletSystemMap.end())
	{
		m_BulletObjectTree->AddBulletSystemToTree(bs_it->second,bs_it->first);
		bs_it ++;
	}
	GetParentFrame()->GetEffectObjectProperty()->InitBulletEditor(mBulletFlow,templateName);
	GetParentFrame()->GetGraphDialog()->InitBulletEditor(mBulletFlow,mBulletSystemMap,templateName);

	//GetParentFrame()->GetEffectObjectProperty()->InitParticleSettingProperty();


}
void
BulletEditor::SetSceneManipulator(Fairy::SceneManipulator* sceneManipulator)
{
	mSceneManipulator = sceneManipulator;
	mSceneManipulator->addSceneListener(this);
}

//销毁效果物体
void BulletEditor::DestroyEffectObject()
{
	if(mBulletFlowObject)
		mSceneManipulator->deselectObject(mBulletFlowObject);
	mBulletFlowObject.reset();
}
//重新设置效果物体
void BulletEditor::ResetBulletObject()
{
	/*if(mBulletFlowObject)
		mSceneManipulator->deselectObject(mBulletFlowObject);
	mBulletFlowObject.reset();
	mParameters["effect name"] = AS_STRING(mBulletFlowTemplateName);
	mBulletFlowObject = Fairy::ObjectFactoryManager::getSingleton().createInstance("Effect");
	mBulletFlowObject->setPropertyList(mParameters);
	mBulletFlowObject->createRenderInstance(mSceneManipulator);
	Ogre::Vector3 position = Ogre::Vector3::ZERO;
	mBulletFlowObject->setProperty("position", position);
	mSceneManipulator->selectObject(mBulletFlowObject);*/
	m_Frame->GetGraphDialog()->InitBulletEditor(mBulletFlow,mBulletSystemMap,mBulletFlowTemplateName);
}
//刷新效果结构树
void BulletEditor::RefreshBulletTree()
{
	m_BulletObjectTree->DeleteAllItems();
	m_BulletObjectTree->AddBulletToTree(mBulletFlow,mBulletFlowTemplateName);
	std::map<Ogre::String,Fairy::BulletSystem*>::iterator bs_it = mBulletSystemMap.begin();
	while(bs_it != mBulletSystemMap.end())
	{
		m_BulletObjectTree->AddBulletSystemToTree(bs_it->second,bs_it->first);
		bs_it ++;
	}
	m_Frame->GetGraphDialog()->InitBulletEditor(mBulletFlow,mBulletSystemMap,mBulletFlowTemplateName);
}
//选择当前编辑页
void BulletEditor::OnPageSelect()
{
	if(!mBulletFlow)
		return;
	this->ResetBulletObject();
	m_Frame->GetEffectObjectProperty()->InitBulletEditor(mBulletFlow,mBulletFlowTemplateName);
	m_BulletObjectTree->refrashSelected();
}
Fairy::BulletFlowSystem* BulletEditor::getCurrentObjectBulletFlow()
{
	if(!mBulletFlowObject)
		return NULL;
	//Fairy::BulletFlowSystem* pEffectObject = dynamic_cast<Fairy::EffectObject*>(mBulletFlowObject.get());
	//if(pEffectObject)
	//{
	//	return pEffectObject->getEffect();
	//}
	return NULL;
}
Fairy::EffectElement* BulletEditor::GetSelectedElement()
{
	Ogre::StringInterface* element = m_BulletObjectTree->GetSelectBulletObject();
	if(element)
	{
		Fairy::EffectElement* pEffectElement = dynamic_cast<Fairy::EffectElement*>(element);
		if(pEffectElement)
			return pEffectElement;
	}
	return NULL;
}
void BulletEditor::RefreshBulletTreeItem(const wxString& Desc,const wxString& propertyName,void* data)
{
	m_BulletObjectTree->refreshItem(Desc,propertyName,data);
}
void BulletEditor::SelectElementItem(Fairy::EffectElement* element)
{
	if(element)
	{
		if(element->getType() == "Particle")
		{
			Fairy::ParticleElement* particle = dynamic_cast<Fairy::ParticleElement*>(element);
			if(particle)
			{
				Ogre::String particleName = particle->getParticleSystemName();
				Ogre::ParticleSystem* pParticleSystem = Ogre::ParticleSystemManager::getSingleton().getTemplate(particleName);
				if(pParticleSystem)
				{

				}
			}
			
		}
	}
	m_Frame->GetGraphDialog()->InitBulletEditor(mBulletFlow,mBulletSystemMap,mBulletFlowTemplateName);
	m_Frame->GetEffectObjectProperty()->InitBulletEditor(mBulletFlow,mBulletFlowTemplateName);
	m_BulletObjectTree->SelectElementItem(element);
}
void BulletEditor::SelectBulletEvent(Fairy::BulletEventSystem* pEvent)
{
	if(pEvent)
		m_BulletObjectTree->SelectBulletEvent(pEvent);
}
void BulletEditor::SelectBulletOperator(Fairy::BulletOperator* pOperator)
{
	if(pOperator)
		m_BulletObjectTree->SelectBulletOperator(pOperator);
}
void BulletEditor::SelectBulletSystem(Fairy::BulletSystem* pSystem)
{
	if(pSystem)
		m_BulletObjectTree->SelectBulletSystem(pSystem);
}
void BulletEditor::SelectBulletSystemElement(Ogre::StringInterface* pElement)
{
	if(pElement)
		m_BulletObjectTree->SelectBulletSystemElement(pElement);
}
void BulletEditor::EditMeshMatrial(Fairy::MeshElement* meshElement)
{
	if(!meshElement)
		return;
	EditMaterialDialog dlg(this,wxID_ANY,"编辑材质",meshElement->getMaterialName().c_str());
	if(dlg.ShowModal() == wxID_OK)
	{
		const Ogre::String newMatrialName = dlg.mTextCtrl->GetValue().c_str(); 
		Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(newMatrialName);
		if(material.isNull())
		{
			material = Ogre::MaterialManager::getSingleton().create(newMatrialName,"General");
		}
		meshElement->setMaterialName(newMatrialName);
		GetParentFrame()->GetEffectObjectProperty()->InitBulletEditor(mBulletFlow,mBulletFlowTemplateName);
		m_Frame->GetGraphDialog()->InitBulletEditor(mBulletFlow,mBulletSystemMap,mBulletFlowTemplateName);
		ResetBulletObject();
		RefreshBulletTree();
		m_BulletObjectTree->SelectElementItem(meshElement);
		m_Frame->GetMaterialEditor()->InitMaterialEditor(material,newMatrialName);
		m_Frame->GetEditorNotebook()->SetSelection(3);
	}

}
void BulletEditor::SetBulletCallBack()
{
	Fairy::BulletFlowSystemManager::getSingleton().setCreateBulletFlowCallback( &s_BulletFlowCreateCallback );
	Fairy::EffectManager::getSingleton().setDestroySkillCallback(&s_SkillDestroyCallback);
	Fairy::BulletFlowSystemManager::getSingleton().setAfterCreatedBulletSystemCallback( &s_CreatedBulletCallback );
	Fairy::BulletFlowSystemManager::getSingleton().setBeforeDeleteBulletSystemCallback( &s_DeleteBulletCallback );
	Fairy::BulletFlowSystemManager::getSingleton().setBulletSystemHitTargetCallback(&s_BulletHitTargetCallback);
}
void BulletEditor::EditBillboardMatrial(Fairy::BillboardSetElement* billboardElement)
{
	if(!billboardElement)
		return;
	EditMaterialDialog dlg(this,wxID_ANY,"编辑材质",billboardElement->getBillboardMaterial().c_str());
	if(dlg.ShowModal() == wxID_OK)
	{
		const Ogre::String newMatrialName = dlg.mTextCtrl->GetValue().c_str(); 
		Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(newMatrialName);
		if(material.isNull())
		{
			material = Ogre::MaterialManager::getSingleton().create(newMatrialName,"General");
		}
		billboardElement->setBillboardMaterial(newMatrialName);
		m_Frame->GetEffectObjectProperty()->InitBulletEditor(mBulletFlow,mBulletFlowTemplateName);
		m_Frame->GetGraphDialog()->InitBulletEditor(mBulletFlow,mBulletSystemMap,mBulletFlowTemplateName);
		ResetBulletObject();
		RefreshBulletTree();
		m_BulletObjectTree->SelectElementItem(billboardElement);
		m_Frame->GetMaterialEditor()->InitMaterialEditor(material,newMatrialName);
		m_Frame->GetEditorNotebook()->SetSelection(3);
	}
}
void BulletEditor::EditProjectorMatrial(Fairy::ProjectorElement* projectorElement)
{
	if(!projectorElement)
		return;
	EditMaterialDialog dlg(this,wxID_ANY,"编辑材质",projectorElement->getMaterialName().c_str());
	if(dlg.ShowModal() == wxID_OK)
	{
		const Ogre::String newMatrialName = dlg.mTextCtrl->GetValue().c_str(); 
		Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(newMatrialName);
		if(material.isNull())
		{
			material = Ogre::MaterialManager::getSingleton().create(newMatrialName,"General");
		}
		projectorElement->setMaterialName(newMatrialName);
		m_Frame->GetEffectObjectProperty()->InitBulletEditor(mBulletFlow,mBulletFlowTemplateName);
		ResetBulletObject();
		RefreshBulletTree();
		m_BulletObjectTree->SelectElementItem(projectorElement);
		m_Frame->GetMaterialEditor()->InitMaterialEditor(material,newMatrialName);
		m_Frame->GetEditorNotebook()->SetSelection(3);
	}
}
void BulletEditor::EditBeamMatrial(Fairy::BeamElement* beamElement)
{
	if(!beamElement)
		return;
	EditMaterialDialog dlg(this,wxID_ANY,"编辑材质",beamElement->getMaterialName().c_str());
	if(dlg.ShowModal() == wxID_OK)
	{
		const Ogre::String newMatrialName = dlg.mTextCtrl->GetValue().c_str(); 
		Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(newMatrialName);
		if(material.isNull())
		{
			material = Ogre::MaterialManager::getSingleton().create(newMatrialName,"General");
		}
		beamElement->setMaterialName(newMatrialName);
		m_Frame->GetEffectObjectProperty()->InitBulletEditor(mBulletFlow,mBulletFlowTemplateName);
		ResetBulletObject();
		RefreshBulletTree();
		m_BulletObjectTree->SelectElementItem(beamElement);
		m_Frame->GetMaterialEditor()->InitMaterialEditor(material,newMatrialName);
		m_Frame->GetEditorNotebook()->SetSelection(3);
	}
}
void BulletEditor::SetMeshMatrial(const Ogre::String name)
{
	Fairy::MeshElement* meshElement = dynamic_cast<Fairy::MeshElement*>(m_BulletObjectTree->GetSelectBulletObject());
	if(!meshElement)
		return;
	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(name);
	if(material.isNull())
	{
		material = Ogre::MaterialManager::getSingleton().create(name,"General");
	}
	meshElement->setMaterialName(name);
	GetParentFrame()->GetEffectObjectProperty()->InitBulletEditor(mBulletFlow,mBulletFlowTemplateName);
	ResetBulletObject();
	RefreshBulletTree();
	m_BulletObjectTree->SelectElementItem(meshElement);
	m_Frame->GetMaterialEditor()->InitMaterialEditor(material,name);
	m_Frame->GetEditorNotebook()->SetSelection(3);

}
void BulletEditor::SetBillboardMatrial(const Ogre::String name)
{
	Fairy::BillboardSetElement* billboardElement = dynamic_cast<Fairy::BillboardSetElement*>(m_BulletObjectTree->GetSelectBulletObject());
	if(!billboardElement)
		return;
	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(name);
	if(material.isNull())
	{
		material = Ogre::MaterialManager::getSingleton().create(name,"General");
	}
	billboardElement->setBillboardMaterial(name);
	GetParentFrame()->GetEffectObjectProperty()->InitBulletEditor(mBulletFlow,mBulletFlowTemplateName);
	ResetBulletObject();
	RefreshBulletTree();
	m_BulletObjectTree->SelectElementItem(billboardElement);
	m_Frame->GetMaterialEditor()->InitMaterialEditor(material,name);
	m_Frame->GetEditorNotebook()->SetSelection(3);

}
void BulletEditor::SetProjectorMatrial(const Ogre::String name)
{
	Fairy::ProjectorElement* projectorElement = dynamic_cast<Fairy::ProjectorElement*>(m_BulletObjectTree->GetSelectBulletObject());
	if(!projectorElement)
		return;
	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(name);
	if(material.isNull())
	{
		material = Ogre::MaterialManager::getSingleton().create(name,"General");
	}
	projectorElement->setMaterialName(name);
	GetParentFrame()->GetEffectObjectProperty()->InitBulletEditor(mBulletFlow,mBulletFlowTemplateName);
	ResetBulletObject();
	RefreshBulletTree();
	m_BulletObjectTree->SelectElementItem(projectorElement);
	m_Frame->GetMaterialEditor()->InitMaterialEditor(material,name);
	m_Frame->GetEditorNotebook()->SetSelection(3);

}
void BulletEditor::SetBeamMatrial(const Ogre::String name)
{
	Fairy::BeamElement* beamElement = dynamic_cast<Fairy::BeamElement*>(m_BulletObjectTree->GetSelectBulletObject());
	if(!beamElement)
		return;
	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(name);
	if(material.isNull())
	{
		material = Ogre::MaterialManager::getSingleton().create(name,"General");
	}
	beamElement->setMaterialName(name);
	GetParentFrame()->GetEffectObjectProperty()->InitBulletEditor(mBulletFlow,mBulletFlowTemplateName);
	ResetBulletObject();
	RefreshBulletTree();
	m_BulletObjectTree->SelectElementItem(beamElement);
	m_Frame->GetMaterialEditor()->InitMaterialEditor(material,name);
	m_Frame->GetEditorNotebook()->SetSelection(3);

}

void BulletEditor::OnNotifyToolBarCtrl(ToolbarCtrl index)
{
	wxCommandEvent e;

	if (index == TC_NEW_BULLETFLOW)
	{
		OnNewBulletFlow(e);
	}
	else if (index == TC_SAVEAS_BULLET)
	{
		OnSaveAsBulletObject(e);
	}
	else if (index == TC_SAVE_BULLET)
	{
		OnSaveBulletObject(e);
	}
	else if (index == TC_DELETE_ElEMENT)
	{
		OnDeleteBulletElement(e);
	}
	else if (index == TC_NEW_EVENT)
	{
		OnNewBulletEvent(e);
	}
	else if (index == TC_NEW_OPERATOR)
	{
		OnNewBulletOperator(e);
	}
	else if (index == TC_NEW_BULLETSYSTEM)
	{
		OnNewBulletSystem(e);
	}
	else if (index == TC_NEW_BULLETSYSTEM_EFFECTINFO)
	{
		OnNewBulletEffectInfo(e);
	}
	else if (index == TC_NEW_BULLETSYSTEM_CAMERASHAKE)
	{
		OnNewBulletCameraShake(e);
	}
	else if (index == TC_NEW_BULLETSYSTEM_RIBBON)
	{
		OnNewBulletRibbon(e);
	}
	else if (index == TC_NEW_BULLETSYSTEM_SOUND)
	{
		OnNewBulletSound(e);
	}
	else if (index == TC_NEW_BULLETSYSTEM_LIGHT)
	{
		OnNewBulletLight(e);
	}
	else if (index == TC_NEW_BULLETSYSTEM_LINE)
	{
		OnNewBulletLine(e);
	}
	
}

//刷新效果数据
void BulletEditor::OnSaveAsBulletObject(wxCommandEvent &e)
{
	SaveAsBulletDialog *dlg = new SaveAsBulletDialog(this, wxID_ANY,
		_("保存数据"), mBulletFlow);

	bool ok = (dlg->ShowModal() == wxID_OK);
	if(ok)
	{
		Ogre::StringVector templates;

		Ogre::String fileName = dlg->mComboBox->GetValue().c_str();
		const Ogre::String templateName = dlg->mTextCtrl->GetValue().c_str();

		if(templateName.length() == 0)
		{
			dlg->Destroy();
			return;
		}

		//判断文件名
		Ogre::StringVector strings = Ogre::StringUtil::split( fileName, "." );
		if (strings.size() != 2 || strings[1] != ".bullet")
		{
			fileName.append(".bullet");
		}

		Fairy::BulletFlowSystemManager::getSingleton().getTemplatesFromScriptFile(fileName, templates);

		std::ofstream outFile;

		Ogre::String pathFileName = EFFECT_PATH+fileName;
		outFile.open ( pathFileName.c_str(), std::ios::out | std::ios::trunc ); // append to file

		if (!outFile.is_open())
		{
			return;
		}

		bool newTemplate = true;

		// 把所有的模板都写入该文件中
		for ( size_t i=0; i<templates.size(); ++i )
		{
			//	if (templates[i] != mBulletFlow->getTemplateName())
			///{
			Fairy::BulletFlowSystem *pBulletFlow = 
				Fairy::BulletFlowSystemManager::getSingleton().getBulletFlowSystemTemplate(templates[i]);
			assert (pBulletFlow);

			if (pBulletFlow->getTemplateName() == dlg->mTextCtrl->GetValue().c_str())
			{
				saveBulletFlow(mBulletFlow, dlg->mTextCtrl->GetValue().c_str(), outFile );


				newTemplate = false;
			}
			else
			{
				saveBulletFlow(pBulletFlow, pBulletFlow->getTemplateName(), outFile);
			}

			Ogre::StringVector bulletSystemTemplates;
			Fairy::BulletSystemManager::getSingleton().getTemplatesFromScriptFile(fileName,bulletSystemTemplates);
			for(size_t j = 0;j < bulletSystemTemplates.size();++j)
			{
				Fairy::BulletSystem* pBulletSystem = 
					Fairy::BulletSystemManager::getSingleton().getBulletSystemTemplate(bulletSystemTemplates[j]);
				if(pBulletSystem)
					saveBulletSystem(pBulletSystem,bulletSystemTemplates[j],outFile);
			}

		}

		if (newTemplate)
		{
			saveBulletFlow(mBulletFlow, templateName, outFile );

			if(templateName != mBulletFlowTemplateName)
			{

				Fairy::BulletFlowSystem* pBulletFlow = 
					Fairy::BulletFlowSystemManager::getSingleton().createBulletFlowSystemTemplate(templateName);
				*pBulletFlow = *mBulletFlow;
				Fairy::BulletFlowSystemManager::getSingleton().removeBulletFlowTemplateScriptFileMap(mBulletFlowTemplateName);
				Fairy::BulletFlowSystemManager::getSingleton().destroyBulletFlowSystemTemplate(mBulletFlowTemplateName);
				mBulletFlow = pBulletFlow;
				
			}
			Fairy::BulletFlowSystemManager::getSingleton().removeBulletFlowTemplateScriptFileMap(templateName);
			Fairy::BulletFlowSystemManager::getSingleton().addToBulletFlowTemplateScriptFileMap(templateName, fileName);
			std::map<Ogre::String,Fairy::BulletSystem*>::iterator it =  mBulletSystemMap.begin();
			while(it != mBulletSystemMap.end())
			{
				Fairy::BulletSystem* pBulletSystem = it->second;
				Fairy::BulletSystemManager::getSingleton().removeBulletSystemTemplateScriptFileMap(
					pBulletSystem->getTemplateName());
				Fairy::BulletSystemManager::getSingleton().addToBulletSystemTemplateScriptFileMap(
					pBulletSystem->getTemplateName(),fileName);
				saveBulletSystem(pBulletSystem,pBulletSystem->getTemplateName(),outFile);
				it ++;
			}

		}
		outFile.close();
		InitBulletEditor(mBulletFlow,templateName);
		mbRecoverSave = false;
	}
	
}
//保存文件
void BulletEditor::OnSaveBulletObject(wxCommandEvent &e)
{

	if(!mBulletFlow)
		return;
	Ogre::String fileName = Fairy::BulletFlowSystemManager::getSingleton().getFileNameByTemplateName(mBulletFlowTemplateName);
	if(mbRecoverSave)
	{
		char buf[256];
		sprintf(buf,"是否覆盖文件%s中的%s子弹数据!",fileName.c_str(),mBulletFlowTemplateName.c_str());
		RecoverSaveBulletDialog* dlg = new RecoverSaveBulletDialog(m_Frame,wxID_ANY,wxT("覆盖文件"));
		dlg->SetText(wxString(buf));
		

		if(wxID_OK == dlg->ShowModal())
		{
			mbRecoverSave = false;
			dlg->Destroy();
		}
		else
		{
			dlg->Destroy();
			return;
		}
	}

	Ogre::StringVector bulletFlowTemplates;
	

	Fairy::BulletFlowSystemManager::getSingleton().getTemplatesFromScriptFile(fileName, bulletFlowTemplates);
	

	std::ofstream outFile;

	Ogre::String pathFileName = EFFECT_PATH+fileName;
	outFile.open ( pathFileName.c_str(), std::ios::out | std::ios::trunc ); // append to file

	if (!outFile.is_open())
	{
		return;
	}

	bool newTemplate = true;

	// 把所有的模板都写入该文件中
	for ( size_t i=0; i<bulletFlowTemplates.size(); ++i )
	{
		//	if (templates[i] != mBulletFlow->getTemplateName())
		///{
		Fairy::BulletFlowSystem *pBulletFlow =  
			Fairy::BulletFlowSystemManager::getSingleton().getBulletFlowSystemTemplate(bulletFlowTemplates[i]);
		assert (pBulletFlow);

		if (pBulletFlow->getTemplateName() == mBulletFlowTemplateName)
		{
			saveBulletFlow(mBulletFlow, mBulletFlowTemplateName, outFile );

			//mBulletFlow = effect;

			newTemplate = false;
		}
		else
		{
			saveBulletFlow(mBulletFlow, pBulletFlow->getTemplateName(), outFile);

		}
		Ogre::StringVector bulletSystemTemplates;
		Fairy::BulletSystemManager::getSingleton().getTemplatesFromScriptFile(fileName,bulletSystemTemplates);
		for(size_t j = 0;j < bulletSystemTemplates.size();++j)
		{
			Fairy::BulletSystem* pBulletSystem = 
				Fairy::BulletSystemManager::getSingleton().getBulletSystemTemplate(bulletSystemTemplates[j]);
			if(pBulletSystem)
				saveBulletSystem(pBulletSystem,bulletSystemTemplates[j],outFile);
		}

	}


	/*if (newTemplate)
	{
		saveBulletFlow(mBulletFlow, mBulletFlowTemplateName, outFile );
		mBulletFlow->setTemplateName(mBulletFlowTemplateName);
		Fairy::EffectManager::getSingleton().addToEffectTemplateScriptFileMap(mBulletFlowTemplateName, fileName);
	}*/

	outFile.close();

}

void BulletEditor::OnNewBulletFlow(wxCommandEvent &e)
{
	NewBulletDialog *dlg = new NewBulletDialog(this, wxID_ANY,
		wxT("新建子弹管理器"));

	bool ok = (dlg->ShowModal() == wxID_OK);
	if(ok)
	{
		const Ogre::String templateName = dlg->mTextCtrl->GetValue().c_str();

		if(templateName.length() == 0)
		{
			dlg->Destroy();
			return;
		}

		// 刷新EffectManager中的模板内容
		
		
		Fairy::BulletFlowSystem *pBulletFlowSystem = 
			Fairy::BulletFlowSystemManager::getSingleton().createBulletFlowSystemTemplate(templateName);
		if(!pBulletFlowSystem)
		{
			dlg->Destroy();
			return;
		}
		Fairy::BulletFlowSystemManager::getSingleton().addToBulletFlowTemplateScriptFileMap(templateName,templateName + ".bullet");
		InitBulletEditor(pBulletFlowSystem,templateName);

		//wxBusyInfo* busyInfo = new wxBusyInfo(wxT("更新效果数据 ..."), this);
		//m_Frame->GetBulletSelector()->Reload();
		//delete busyInfo;
	}
	dlg->Destroy();

}
void  BulletEditor::OnDeleteBulletElement(wxCommandEvent &e)
{
	if(!mBulletFlow)
		return;
	Ogre::StringInterface *pSelectObject = m_BulletObjectTree->GetSelectBulletObject();
	if(!pSelectObject)
		return;
	Fairy::BulletEventSystem* pEvent = dynamic_cast<Fairy::BulletEventSystem*>(pSelectObject);
	if(pEvent)
	{
		mBulletFlow->removeBulletEventSystem(pEvent);
		InitBulletEditor(mBulletFlow,mBulletFlowTemplateName);
		return;
	}
	Fairy::BulletOperator* pBulletOperator = dynamic_cast<Fairy::BulletOperator*>(pSelectObject);
	if(pBulletOperator)
	{
		Fairy::BulletFlowSystem::BulletEventMap::const_iterator it = mBulletFlow->getBulletEventSystemsMap().begin();
		while(it != mBulletFlow->getBulletEventSystemsMap().end())
		{
			Fairy::BulletEventSystem* pEvent = it->second;

			for(int index = 0; index < pEvent->getNumBulletOperator(); index++)
			{
				Fairy::BulletOperator* pOperator= pEvent->getBulletOperator(index);
				if(pOperator == pBulletOperator)
				{
					pEvent->removeBulletOperator(index);
					InitBulletEditor(mBulletFlow,mBulletFlowTemplateName);
					return;
				}
			}
			it ++;
		}
	}
	Fairy::BulletSystem* pBulletSystem = dynamic_cast<Fairy::BulletSystem*>(pSelectObject);
	if(pBulletSystem)
	{
		Fairy::BulletSystemManager::getSingleton().destroyBulletSystemTemplate(pBulletSystem);
		InitBulletEditor(mBulletFlow,mBulletFlowTemplateName);
		return;
	}

	Ogre::String fileName = Fairy::BulletFlowSystemManager::getSingleton().getFileNameByTemplateName(mBulletFlowTemplateName);
	Ogre::StringVector BulletSystemNames;
	Fairy::BulletSystemManager::getSingleton().getTemplatesFromScriptFile(fileName,BulletSystemNames);
	mBulletSystemMap.clear();
	Ogre::StringVector::iterator it= BulletSystemNames.begin();
	while(it != BulletSystemNames.end())
	{
		Ogre::String templateName = *it;
		Fairy::BulletSystem* pBulletSystem = Fairy::BulletSystemManager::getSingleton().getBulletSystemTemplate(templateName);
		if(pBulletSystem)
		{
			int i = 0;

			for(i = 0;i<pBulletSystem->getNumAnimationEffectInfos();i++)
			{
				Fairy::AnimationEffectInfo *pEffectInfo = pBulletSystem->getAnimationEffectInfo(i);
				if(pEffectInfo == pSelectObject)
				{
					pBulletSystem->removeAnimationEffectInfo(i);
					InitBulletEditor(mBulletFlow,mBulletFlowTemplateName);
					return;
				}

			}

			for(i = 0;i<pBulletSystem->getNumAnimationRibbons();i++)
			{
				Fairy::AnimationRibbon *pRibbon = pBulletSystem->getAnimationRibbon(i);
				if(pRibbon == pSelectObject)
				{
					pBulletSystem->removeAnimationRibbon(i);
					InitBulletEditor(mBulletFlow,mBulletFlowTemplateName);
					return;
				}

			}

			for(i = 0;i<pBulletSystem->getNumAnimationSounds();i++)
			{
				Fairy::AnimationSound *pSound = pBulletSystem->getAnimationSound(i);
				if(pSound == pSelectObject)
				{
					pBulletSystem->removeAnimationSound(i);
					InitBulletEditor(mBulletFlow,mBulletFlowTemplateName);
					return;
				}

			}

			for(i = 0;i<pBulletSystem->getNumAnimationSceneLightInfos();i++)
			{
				Fairy::AnimationSceneLightInfo *pLightInfo = pBulletSystem->getAnimationSceneLightInfo(i);
				if(pLightInfo == pSelectObject)
				{
					pBulletSystem->removeAnimationSceneLightInfo(i);
					InitBulletEditor(mBulletFlow,mBulletFlowTemplateName);
					return;
				}
			}

			for(i = 0;i<pBulletSystem->getNumAnimationCameraShake();i++)
			{
				Fairy::AnimationCameraShake *pCameraShake = pBulletSystem->getAnimationCameraShake(i);
				if(pCameraShake == pSelectObject)
				{
					pBulletSystem->removeAnimationCameraShake(i);
					InitBulletEditor(mBulletFlow,mBulletFlowTemplateName);
					return;
				}
			}
		}
		it ++;
	}

	


	/*for(int i = 0;i<mBulletFlow->getNumElements();i++)
	{
		if(mBulletFlow->getElement(i) == m_BulletObjectTree->GetSelectEffectObject())
		{
			mBulletFlow->destroyElement(i);
			RefreshBulletTree();
			m_Frame->GetEffectObjectProperty()->InitEffectSettingProperty();
			ResetBulletObject();
			break;
		}

	}*/
}

void BulletEditor::OnNewBulletEvent(wxCommandEvent &e)
{
	if(!mBulletFlow)
		return;
	NewBulletEventDialog *dlg = new NewBulletEventDialog(this, wxID_ANY,
		_("创建事件"));

	bool ok = (dlg->ShowModal() == wxID_OK);
	if(ok)
	{
		const Ogre::String eventName = AS_STRING(dlg->mTextCtrl->GetValue());
		if(!mBulletFlow->getBulletEventSystem(eventName))
		{
			Fairy::BulletEventSystem* pEvent = mBulletFlow->addBulletEventSystem(eventName);
			if(!pEvent)
				return;
			ResetBulletObject();
			RefreshBulletTree();
			SelectBulletEvent(pEvent);
		}
	}
	
}
void BulletEditor::OnNewBulletOperator(wxCommandEvent &e)
{
	Fairy::BulletEventSystem* pEvent = 
		dynamic_cast<Fairy::BulletEventSystem*>(m_BulletObjectTree->GetSelectBulletObject());
	if(pEvent)
	{
		NewBulletOperatorDialog *dlg = new NewBulletOperatorDialog(this, wxID_ANY,
			_("创建操作器"));
		bool ok = (dlg->ShowModal() == wxID_OK);
		if(ok)
		{
			const Ogre::String factoryName = AS_STRING(dlg->mComboBox->GetValue());

			Fairy::BulletOperator* pOperator = pEvent->addBulletOperator(factoryName);
			if(!pOperator)
				return;
			ResetBulletObject();
			RefreshBulletTree();
			SelectBulletOperator(pOperator);
		}
	}
}
void BulletEditor::OnNewBulletSystem(wxCommandEvent &e)
{
	
	if(!mBulletFlow)
		return;
	NewBulletSystemDialog *dlg = new NewBulletSystemDialog(this, wxID_ANY,
		_("创建BulletSystem"));

	bool ok = (dlg->ShowModal() == wxID_OK);
	if(ok)
	{
		const Ogre::String BulletSystemName = AS_STRING(dlg->mTextCtrl->GetValue());

		Fairy::BulletSystem* pSystem =
			Fairy::BulletSystemManager::getSingleton().createBulletSystemTemplate(BulletSystemName);
		if(!pSystem)
			return;
		Ogre::String  fileName = Fairy::BulletFlowSystemManager::getSingleton().getFileNameByTemplateName(mBulletFlowTemplateName);
		Fairy::BulletSystemManager::getSingleton().addToBulletSystemTemplateScriptFileMap(pSystem->getTemplateName(),fileName);
		InitBulletEditor(mBulletFlow,mBulletFlowTemplateName);
		SelectBulletSystem(pSystem);

	}
}
void BulletEditor::OnNewBulletEffectInfo(wxCommandEvent &e)
{
	Fairy::BulletSystem* pSystem = dynamic_cast<Fairy::BulletSystem*>(m_BulletObjectTree->GetSelectBulletObject());
	if(pSystem)
	{

		AddBulletEffectInfoDialog *dlg = new AddBulletEffectInfoDialog(this, wxID_ANY,
			_("Animation Effect Info"));

		bool ok = (dlg->ShowModal() == wxID_OK);
		if(ok)
		{
			Ogre::String effect = wxDynamicCast(dlg->FindWindow(feID_COMBOCTRL_BULLET_EFFECT),wxComboBox)->GetValue().c_str();
			Ogre::String locator1 = wxDynamicCast(dlg->FindWindow(feID_COMBOCTRL_BULLET_LOCATOR),wxComboBox)->GetValue().c_str();

			Fairy::AnimationEffectInfo *effectInfo = pSystem->addAnimationEffectInfo();
			assert (effectInfo);

			effectInfo->setEffectTemplateName(effect);
			effectInfo->setAttachPoint(locator1);

			ResetBulletObject();
			RefreshBulletTree();
			SelectBulletSystemElement(effectInfo);
		}

		dlg->Destroy();
	}

}
void BulletEditor::OnNewBulletRibbon(wxCommandEvent &e)
{
	Fairy::BulletSystem* pSystem = dynamic_cast<Fairy::BulletSystem*>(m_BulletObjectTree->GetSelectBulletObject());
	if(pSystem)
	{
		AddBulletRibbonDialog *dlg = new AddBulletRibbonDialog(this, wxID_ANY,
			_("Animation Ribbon"));

		bool ok = (dlg->ShowModal() == wxID_OK);
		if(ok)
		{
			Ogre::String locator1 = wxDynamicCast(dlg->FindWindow(feID_COMBOCTRL_BULLET_LOCATOR),wxComboBox)->GetValue().c_str();

			Fairy::AnimationRibbon *ribbon = pSystem->addAnimationRibbon();
			assert (ribbon);
			//			ribbon->setEffectTemplateName(effect);
			ribbon->setAttachPoint(locator1);

			ResetBulletObject();
			RefreshBulletTree();
			SelectBulletSystemElement(ribbon);
		}

		dlg->Destroy();
	}
}
void BulletEditor::OnNewBulletSound(wxCommandEvent &e)
{
	Fairy::BulletSystem* pSystem = dynamic_cast<Fairy::BulletSystem*>(m_BulletObjectTree->GetSelectBulletObject());
	if(pSystem)
	{
		Fairy::AnimationSound *sound = pSystem->addAnimationSound();
		assert (sound);

		ResetBulletObject();
		RefreshBulletTree();
		SelectBulletSystemElement(sound);
	}
}
void BulletEditor::OnNewBulletLight(wxCommandEvent &e)
{
	Fairy::BulletSystem* pSystem = dynamic_cast<Fairy::BulletSystem*>(m_BulletObjectTree->GetSelectBulletObject());
	if(pSystem)
	{

		Fairy::AnimationSceneLightInfo *light = pSystem->addAnimationSceneLightInfo();
		assert (light);

		ResetBulletObject();
		RefreshBulletTree();
		SelectBulletSystemElement(light);
	}
	
}
void BulletEditor::OnNewBulletLine(wxCommandEvent &e)
{
	Fairy::BulletSystem* pSystem = dynamic_cast<Fairy::BulletSystem*>(m_BulletObjectTree->GetSelectBulletObject());
	if(pSystem)
	{

		Fairy::AnimationLine *line = pSystem->addAnimationLine();
		assert (line);

		ResetBulletObject();
		RefreshBulletTree();
		SelectBulletSystemElement(line);
	}
}
void BulletEditor::OnNewBulletCameraShake(wxCommandEvent &e)
{
	Fairy::BulletSystem* pSystem = dynamic_cast<Fairy::BulletSystem*>(m_BulletObjectTree->GetSelectBulletObject());
	if(pSystem)
	{

		Fairy::AnimationCameraShake *cameraShake = pSystem->addAnimationCameraShake();
		assert (cameraShake);

		ResetBulletObject();
		RefreshBulletTree();
		SelectBulletSystemElement(cameraShake);
	}
}
bool BulletEditor::tick()
{
	int currentTime  = Ogre::Root::getSingleton().getTimer()->getMilliseconds();
	int timeSpace = currentTime - mFrameTime;
	mFrameTime = currentTime;
	std::vector<Fairy::BulletFlowSystem*>::iterator iter = m_ActiveBulleFlows.begin();
	//std::vector<Fairy::BulletFlowSystem*>::iterator tempIter;

	while (iter != m_ActiveBulleFlows.end())
	{
		//tempIter = iter;
		//++iter;

		Fairy::BulletFlowSystem* pBulletFlowSystem = *iter;

		if (NULL == pBulletFlowSystem)
		{
			m_ActiveBulleFlows.erase(iter);
		}
		else
		{
			// 时间结束,删除
			if (pBulletFlowSystem->getAge() >  pBulletFlowSystem->getMaxTime())
			{
				delete pBulletFlowSystem;
				pBulletFlowSystem = NULL;
				m_ActiveBulleFlows.erase(iter);
			}
			// 子弹刷新
			else
			{
				pBulletFlowSystem->update(float(timeSpace)* 0.001f);
				iter++;
			}			
		}
	}

	return true;
}

void BulletEditor::saveBulletFlow(Fairy::BulletFlowSystem *bulletFlow, const Ogre::String &templateName, std::ofstream &of)
{
	assert (bulletFlow);

	of << "bulletflow " << templateName << "\n";
	of << "{\n";

	const Ogre::ParameterList &paraList = bulletFlow->getParameters();

	for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
	{
		of << "\t" << paraList[paraCount].name << "\t" << bulletFlow->getParameter( paraList[paraCount].name ) << "\n";
	}


	Fairy::BulletFlowSystem::BulletEventMap::const_iterator it = bulletFlow->getBulletEventSystemsMap().begin();
	while(it != bulletFlow->getBulletEventSystemsMap().end())
	{
		Fairy::BulletEventSystem* pEvent = it->second;

		of << "\n";
		of << "\teventsystem " << pEvent->getTemplateName() << "\n";
		of << "\t{\n";

		const Ogre::ParameterList &paraList = pEvent->getParameters();

		for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
		{
			of << "\t\t" << paraList[paraCount].name << "\t" << pEvent->getParameter( paraList[paraCount].name ) << "\n";
		}		

		for(int index = 0; index < pEvent->getNumBulletOperator(); index++)
		{
			Fairy::BulletOperator* pOperator= pEvent->getBulletOperator(index);
			Fairy::BulletTester* pTester = dynamic_cast<Fairy::BulletTester*>(pOperator);
			if(pTester)
			{
				of << "\n";
				of << "\t\ttester " << pTester->getType() << "\n";
				of << "\t\t{\n";

				const Ogre::ParameterList &paraList = pTester->getParameters();

				for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
				{
					of << "\t\t\t" << paraList[paraCount].name << "\t" << pTester->getParameter( paraList[paraCount].name ) << "\n";
				}		

				of << "\t\t}\n";

			}
			else
			{

				of << "\n";
				of << "\t\toperator " << pOperator->getType() << "\n";
				of << "\t\t{\n";

				const Ogre::ParameterList &paraList = pOperator->getParameters();

				for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
				{
					of << "\t\t\t" << paraList[paraCount].name << "\t" << pOperator->getParameter( paraList[paraCount].name ) << "\n";
				}		

				of << "\t\t}\n";
			}
		}
		of << "\t}\n";

		it ++;
	}
	of << "}\n";
}
void BulletEditor::saveBulletSystem(Fairy::BulletSystem *bulletSystem, const Ogre::String &templateName, std::ofstream &of)
{
	assert (bulletSystem);
	of << "\n";
	of << "bulletsystem " << templateName << "\n";
	of << "{\n";

	const Ogre::ParameterList &paraList = bulletSystem->getParameters();

	for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
	{
		of << "\t" << paraList[paraCount].name << " " << bulletSystem->getParameter(paraList[paraCount].name) << "\n";
	}

	for (unsigned short i=0; i<bulletSystem->getNumAnimationEffectInfos(); ++i)
	{
		of << "\n";
		of << "\tAnimEffect\n" << "\t{\n";

		Fairy::AnimationEffectInfo *effectInfo = bulletSystem->getAnimationEffectInfo(i);
		assert (effectInfo);

		const Ogre::ParameterList &effectInfoList = effectInfo->getParameters();

		for ( size_t paraCount = 0; paraCount < effectInfoList.size(); ++paraCount )
		{
			of << "\t\t" << effectInfoList[paraCount].name.c_str() 
				<< " " << effectInfo->getParameter( effectInfoList[paraCount].name.c_str() ) << "\n";
		}

		of << "\t}\n";
	}

	for (unsigned short i=0; i<bulletSystem->getNumAnimationRibbons(); ++i)
	{
		of << "\n";
		of << "\tRibbon\n" << "\t{\n";

		Fairy::AnimationRibbon *ribbon = bulletSystem->getAnimationRibbon(i);
		assert (ribbon);

		const Ogre::ParameterList &ribbonList = ribbon->getParameters();

		for ( size_t paraCount = 0; paraCount < ribbonList.size(); ++paraCount )
		{
			of << "\t\t" << ribbonList[paraCount].name.c_str() 
				<< " " << ribbon->getParameter( ribbonList[paraCount].name.c_str() ) << "\n";
		}

		of << "\t}\n";
	}
	for (unsigned short i=0; i<bulletSystem->getNumAnimationCameraShake(); ++i)
	{
		of << "\n";
		of << "\tCameraShake\n" << "\t{\n";

		Fairy::AnimationCameraShake *cameraShake = bulletSystem->getAnimationCameraShake(i);
		assert (cameraShake);

		const Ogre::ParameterList &cameraShakeList = cameraShake->getParameters();

		for ( size_t paraCount = 0; paraCount < cameraShakeList.size(); ++paraCount )
		{
			of << "\t\t" << cameraShakeList[paraCount].name.c_str() 
				<< " " << cameraShake->getParameter( cameraShakeList[paraCount].name.c_str() ) << "\n";
		}

		of << "\t}\n";
	}

	for (unsigned short i=0; i<bulletSystem->getNumAnimationSounds(); ++i)
	{
		of << "\n";
		of << "\tSound\n" << "\t{\n";

		Fairy::AnimationSound *sound = bulletSystem->getAnimationSound(i);
		assert (sound);

		const Ogre::ParameterList &soundList = sound->getParameters();

		for ( size_t paraCount = 0; paraCount < soundList.size(); ++paraCount )
		{
			of << "\t\t" << soundList[paraCount].name.c_str() 
				<< " " << sound->getParameter( soundList[paraCount].name.c_str() ) << "\n";
		}

		of << "\t}\n";
	}

	for (unsigned short i=0; i<bulletSystem->getNumAnimationSceneLightInfos(); ++i)
	{
		of << "\n";
		of << "\tSceneLight\n" << "\t{\n";

		Fairy::AnimationSceneLightInfo *light = bulletSystem->getAnimationSceneLightInfo(i);
		assert (light);

		const Ogre::ParameterList &lightList = light->getParameters();

		for ( size_t paraCount = 0; paraCount < lightList.size(); ++paraCount )
		{
			of << "\t\t" << lightList[paraCount].name.c_str() 
				<< " " << light->getParameter( lightList[paraCount].name.c_str() ) << "\n";
		}

		of << "\t}\n";
	}

	for (unsigned short i=0; i<bulletSystem->getNumAnimationLines(); ++i)
	{
		of << "\n";
		of << "\tLine\n" << "\t{\n";

		Fairy::AnimationLine *pLine = bulletSystem->getAnimationLine(i);
		assert (pLine);

		const Ogre::ParameterList &LineList = pLine->getParameters();

		for ( size_t paraCount = 0; paraCount < LineList.size(); ++paraCount )
		{
			of << "\t\t" << LineList[paraCount].name.c_str() 
				<< " " << pLine->getParameter( LineList[paraCount].name.c_str() ) << "\n";
		}

		of << "\t}\n";
	}

	of << "}";

}
