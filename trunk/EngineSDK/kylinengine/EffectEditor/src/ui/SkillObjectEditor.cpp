
#include "SkillObjectEditor.h"
#include "SkillObjectTree.h"
#include "WXGraphDialog.h"
#include "EffectObjectPropertyEditor.h"
#include "Core/FairyObjectFactoryManager.h"
#include "EffectSystem/FairyAnimationBulletFlow.h"
#include "EffectSystem/FairyAnimationRibbon.h"
#include "EffectSystem/FairyAnimationSound.h"
#include "EffectSystem/FairyAnimationEffectInfo.h"
#include "EffectSystem/FairyAnimationSceneLightInfo.h"
#include "EffectSystem/FairyAnimationCameraShake.h"
#include "EffectSystem/FairyBulletFlowSystem.h"
#include "SkillSelector.h"
#include "EditMaterialDialog.h"
#include "BulletEditor.h"
#include "MaterialEditor.h"
#include "ActorAnimationSettingEditor.h"
#include "OgreMaterialManager.h"
#include "Core/FairySceneInfo.h"
#include "Core/FairyEffectObject.h"
#include <wx/image.h>
#include <wx/msgdlg.h>
#include <wx/busyinfo.h>
#include <OgreStringVector.h>
///////////////////////////////////////////////////////////////////////////

#if wxUSE_STL
#define AS_STRING(s)    (s)
#else
#define AS_STRING(s)    ((s).c_str())
#endif



IMPLEMENT_DYNAMIC_CLASS(AddEffectInfoDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(AddEffectInfoDialog, wxDialog)
EVT_INIT_DIALOG(AddEffectInfoDialog::OnInitDialog)
END_EVENT_TABLE()

void
AddEffectInfoDialog::OnInitDialog(wxInitDialogEvent &e)
{
	wxDialog::OnInitDialog(e);

	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer *item1 = new wxFlexGridSizer( 2, 0, 0 );

	wxComboBox *item2 = new wxComboBox( this, feID_COMBOCTRL_EFFECT, wxT(""), wxDefaultPosition, wxSize(200,-1), 0, NULL, wxCB_DROPDOWN );
	item1->Add( item2, 0, wxALIGN_CENTER|wxALL, 5 );

	wxComboBox *item3 = new wxComboBox( this, feID_COMBOCTRL_LOCATOR, wxT(""), wxDefaultPosition, wxSize(200,-1), 0, NULL, wxCB_DROPDOWN );
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

	SkillObjectEditor* pEditor= static_cast<SkillObjectEditor*>(this->GetParent());
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
						// info of the locator is valid
						item3->Append(locatorNames[i].c_str());
						break;
					}
				}						
			}
		}
	}

	item3->SetSelection(0);
}


IMPLEMENT_DYNAMIC_CLASS(AddRibbonDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(AddRibbonDialog, wxDialog)
EVT_INIT_DIALOG(AddRibbonDialog::OnInitDialog)
END_EVENT_TABLE()

void
AddRibbonDialog::OnInitDialog(wxInitDialogEvent &e)
{
	wxDialog::OnInitDialog(e);

	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer *item1 = new wxFlexGridSizer( 2, 0, 0 );

	wxComboBox *item3 = new wxComboBox( this, feID_COMBOCTRL_LOCATOR, wxT(""), wxDefaultPosition, wxSize(200,-1), 0, NULL, wxCB_DROPDOWN );
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


	
	SkillObjectEditor* pEditor= static_cast<SkillObjectEditor*>(this->GetParent());
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

IMPLEMENT_DYNAMIC_CLASS(AddSkillDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(AddSkillDialog, wxDialog)
EVT_INIT_DIALOG(AddSkillDialog::OnInitDialog)
END_EVENT_TABLE()

void
AddSkillDialog::OnInitDialog(wxInitDialogEvent &e)
{
	wxDialog::OnInitDialog(e);

	wxBoxSizer *item0 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer *item1 = new wxFlexGridSizer( 2, 0, 0 );

	wxStaticText *item2 = new wxStaticText( this, wxID_ANY, /*_("Animation")*/ wxT("动作"), wxDefaultPosition, wxDefaultSize, 0 );
	item1->Add( item2, 0, wxALIGN_CENTER|wxALL, 5 );

	wxString *strs3 = (wxString*) NULL;
	wxComboBox *item3 = new wxComboBox( this, ID_COMBO_ANIMATION, wxT(""), wxDefaultPosition, wxSize(200,-1), 0, strs3, wxCB_DROPDOWN );
	item1->Add( item3, 0, wxALIGN_CENTER|wxALL, 5 );

	wxStaticText *item4 = new wxStaticText( this, wxID_ANY, /*_("Skill Name")*/wxT("技能名称"), wxDefaultPosition, wxDefaultSize, 0 );
	item1->Add( item4, 0, wxALIGN_CENTER|wxALL, 5 );

	wxTextCtrl *item5 = new wxTextCtrl( this, ID_TEXTCTRL_SKILL, wxT(""), wxDefaultPosition, wxSize(200,-1), 0 );
	item1->Add( item5, 0, wxALIGN_CENTER|wxALL, 5 );

	wxButton *item6 = new wxButton( this, wxID_OK, /*_("OK")*/wxT("确认[&O]"), wxDefaultPosition, wxDefaultSize, 0 );
	item1->Add( item6, 0, wxALIGN_CENTER|wxALL, 5 );

	wxButton *item7 = new wxButton( this, wxID_CANCEL, /*_("Cancel")*/wxT("取消[&C]"), wxDefaultPosition, wxDefaultSize, 0 );
	item1->Add( item7, 0, wxALIGN_CENTER|wxALL, 5 );

	item0->Add( item1, 0, wxALIGN_CENTER|wxALL, 5 );

	this->SetSizer( item0 );
	item0->SetSizeHints( this );

	SkillObjectEditor* pSkillEditor = dynamic_cast<SkillObjectEditor*>(m_parent);
	assert(pSkillEditor);

	ActorAnimationSettingEditor *parentDialog = pSkillEditor->GetParentFrame()->GetActorSettingEditor();
	assert (parentDialog);
	Fairy::LogicModel *object = parentDialog->GetCurrentObject();

	for ( unsigned short i = 0; i < object->getSkeletonAnimationCount(); ++i )
	{
		Ogre::Animation *anim = object->getSkeletonAnimation(i);

		if ( anim )
		{
			wxString nameStr(anim->getName().c_str());
			item3->AppendString(nameStr);
		}
	}

	// 加入obj中定义的动作名称
	Fairy::LogicModelManager::AnimationSkeletonLinksIterator asmIt = 
		Fairy::LogicModelManager::getSingleton().getAnimationSkeletonLinksIterator(object->getName());

	while ( asmIt.hasMoreElements() )
	{
		wxString animName(asmIt.peekNextKey().c_str());

		if ( wxNOT_FOUND == item3->FindString(animName) )
		{
			wxString nameStr(animName.c_str());
			item3->AppendString(nameStr);
		}

		asmIt.moveNext();
	}

	item3->SetSelection(0);
	mAnimComboBox = item3;

	mSkillNameTextCtrl = item5;	
}

IMPLEMENT_DYNAMIC_CLASS(SaveAsSkillDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(SaveAsSkillDialog, wxDialog)
EVT_INIT_DIALOG(SaveAsSkillDialog::OnInitDialog)
END_EVENT_TABLE()

void
SaveAsSkillDialog::OnInitDialog(wxInitDialogEvent &e)
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

	Ogre::StringVector loadedScripts;
	Fairy::EffectManager::getSingleton().getLoadedSkillScripts( loadedScripts );

	for ( size_t i=0; i<loadedScripts.size(); ++i )
	{
		if (loadedScripts[i] != "EmptySkill.skill")
			mComboBox->AppendString(loadedScripts[i]);
	}

	// 若曾保存过，则显示上次保存的文件名
	SkillObjectEditor* pSkillObjectEditor = static_cast<SkillObjectEditor*>(mParent);
	if (!pSkillObjectEditor->getSaveFileName().empty())
	{
		Ogre::String fileName = Fairy::EffectManager::getSingleton().getFileNameBySkillTemplateName(mSkill->getSkillName());

		if ( fileName == "EmptySkill.skill" )
			mComboBox->SetSelection(0);
		else
			mComboBox->SetValue(fileName);
	}
}


IMPLEMENT_DYNAMIC_CLASS(SaveSkillDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(SaveSkillDialog, wxDialog)
EVT_INIT_DIALOG(SaveSkillDialog::OnInitDialog)
END_EVENT_TABLE()

void
SaveSkillDialog::OnInitDialog(wxInitDialogEvent &e)
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


static const int feID_DELETE_SKILL_ELEMENT =wxNewId();
static const int feID_SAVEAS_SKILL =wxNewId();
static const int feID_SAVE_SKILL =wxNewId();
static const int feID_NEW_SKILL =wxNewId();
static const int feID_REFRESH_SKILL_OBJECT = wxNewId();
static const int feID_NEW_SKILL_EFFECT = wxNewId();
static const int feID_NEW_SKILL_RIBBON = wxNewId();
static const int feID_NEW_SKILL_SOUND = wxNewId();
static const int feID_NEW_SKILL_SCENELIGHT = wxNewId();
static const int feID_NEW_SKILL_MISSILE = wxNewId();
static const int feID_NEW_SKILL_CAMERASHAKE = wxNewId();

IMPLEMENT_DYNAMIC_CLASS(SkillObjectEditor, wxPanel)

BEGIN_EVENT_TABLE(SkillObjectEditor, wxPanel)
EVT_MENU(feID_DELETE_SKILL_ELEMENT,SkillObjectEditor::OnDeleteSkillElement)
EVT_MENU(feID_REFRESH_SKILL_OBJECT,SkillObjectEditor::OnRefreshSkillObject)
EVT_MENU(feID_SAVEAS_SKILL,SkillObjectEditor::OnSaveAsSkillObject)
EVT_MENU(feID_NEW_SKILL,SkillObjectEditor::OnNewSkill)
EVT_MENU(feID_SAVE_SKILL,SkillObjectEditor::OnSaveSkillObject)
EVT_MENU(feID_NEW_SKILL_EFFECT,SkillObjectEditor::OnNewAnimEffect)
EVT_MENU(feID_NEW_SKILL_RIBBON,SkillObjectEditor::OnNewRibbon)
EVT_MENU(feID_NEW_SKILL_SOUND,SkillObjectEditor::OnNewSound)
EVT_MENU(feID_NEW_SKILL_SCENELIGHT,SkillObjectEditor::OnNewSceneLight)
EVT_MENU(feID_NEW_SKILL_MISSILE,SkillObjectEditor::OnNewMissile)
EVT_MENU(feID_NEW_SKILL_CAMERASHAKE,SkillObjectEditor::OnNewBulletCameraShake)

END_EVENT_TABLE()


SkillObjectEditor::SkillObjectEditor( wxWindow* parent,
				   wxWindowID id,
				   const wxPoint& pos,
				   const wxSize& size,
				   long style) : mSkillTemplateName(""), mSaveFileName(""), mSkill(NULL), mConfig(NULL),
				   EFFECT_PATH("../resource/media/特效/技能/"),
				   mDObject(NULL)
{
	Create(parent, id, pos, size, style);
}

bool
SkillObjectEditor::Create(wxWindow *parent, wxWindowID winid,
							   const wxPoint& pos, const wxSize& size,
							   long style)
{
	if (!wxPanel::Create(parent, winid, pos, size, style))
		return false;

	wxFlexGridSizer* sizer = new wxFlexGridSizer(1);
	sizer->AddGrowableCol(0);
	sizer->AddGrowableRow(1);


	m_toolBar = new wxToolBar( this, wxID_ANY, wxDefaultPosition, wxSize(-1,28)); 
	m_toolBar->AddTool(feID_NEW_SKILL,
		wxBITMAP(NEW),wxBITMAP(NEW),false,
		0,wxDefaultCoord,NULL, wxT("创建技能数据"),wxT("创建技能数据"));

	m_toolBar->AddTool(feID_SAVEAS_SKILL,
		wxBITMAP(SAVEAS),wxBITMAP(SAVEAS),false,
		1,wxDefaultCoord,NULL, wxT("另存技能数据"),wxT("另存技能数据"));

	m_toolBar->AddTool(feID_SAVE_SKILL,
		wxBITMAP(SAVE),wxBITMAP(SAVE),false,
		2,wxDefaultCoord,NULL, wxT("保存技能数据"),wxT("保存技能数据"));	

	m_toolBar->AddTool(feID_DELETE_SKILL_ELEMENT,
		wxBITMAP(DELETE),wxBITMAP(DELETE),false,
		3,wxDefaultCoord,NULL, wxT("删除技能元素"),wxT("删除技能元素"));
	
	m_toolBar->AddSeparator();

	m_toolBar->AddTool(feID_NEW_SKILL_EFFECT,
		wxBITMAP(NEWAINIMEFFECT),wxBITMAP(NEWAINIMEFFECT),false,
		4,wxDefaultCoord,NULL, wxT("添加效果元素"),wxT("添加效果元素"));
	m_toolBar->AddTool(feID_NEW_SKILL_CAMERASHAKE,
		wxBITMAP(NEWCAMERASHAKE),wxBITMAP(NEWCAMERASHAKE),false,
		5,wxDefaultCoord,NULL, wxT("新建相机震动元素"),wxT("新建相机震动元素"));
	m_toolBar->AddTool(feID_NEW_SKILL_RIBBON,
		wxBITMAP(NEWRIBBON),wxBITMAP(NEWRIBBON),false,
		6,wxDefaultCoord,NULL, wxT("添加刀光元素"),wxT("添加刀光元素"));
	m_toolBar->AddTool(feID_NEW_SKILL_SOUND,
		wxBITMAP(NEWSOUND),wxBITMAP(NEWSOUND),false,
		7,wxDefaultCoord,NULL, wxT("添加声音元素"),wxT("添加声音元素"));
	m_toolBar->AddTool(feID_NEW_SKILL_SCENELIGHT,
		wxBITMAP(NEWSCENELIGHT),wxBITMAP(NEWSCENELIGHT),false,
		8,wxDefaultCoord,NULL, wxT("添加场景灯光元素"),wxT("添加场景灯光元素"));
	m_toolBar->AddTool(feID_NEW_SKILL_MISSILE,
		wxBITMAP(NEWANIMBULLETFLOW),wxBITMAP(NEWANIMBULLETFLOW),false,
		9,wxDefaultCoord,NULL, wxT("添加导弹元素"),wxT("添加导弹元素"));


	m_toolBar->Realize();
	
	
	sizer->Add( m_toolBar, 0, wxEXPAND, 0 );

	m_SkillObjectTree = new SkillObjectTree( this, ID_SKILL_OBJECT_TREE, wxDefaultPosition, wxDefaultSize);
	sizer->Add( m_SkillObjectTree, 0, wxGROW|wxALL, 0 );

	this->SetSizer( sizer );
	sizer->SetSizeHints(this);

	return true;
}

SkillObjectEditor::~SkillObjectEditor()
{
}


void SkillObjectEditor::InitSkillObject()
{

}
void SkillObjectEditor::InitSkillEditor(Fairy::Skill* pSkill,Ogre::String& templateName)
{
	mSkillTemplateName = templateName;
	mSkill = pSkill;

	GetParentFrame()->GetEffectObjectProperty()->InitSkillEditor(mSkill,templateName);

	m_SkillObjectTree->DeleteAllItems();
	m_SkillObjectTree->AddSkillToTree(mSkill);
	m_SkillObjectTree->SelectSkillSetting();

	Fairy::LogicModel* pDObject = m_Frame->GetActorSettingEditor()->GetActorObject();

	pDObject->setAnimEditing(false);
	m_Frame->GetGraphDialog()->InitSkillEditor(mSkill,mSkillTemplateName);
	m_Frame->GetGraphDialog()->InitAnimTimePosSlider();
	
	try
	{
		pDObject->createSkill( mSkill->getSkillName(), m_Frame->GetGraphDialog()->GetLoop());
		if(mSkill->getNumAnimationBulletFlows() > 0)
		{
			BulletFlowInfo* pInfo = new BulletFlowInfo;
			Fairy::AnimationBulletFlow* pAnimationFlow = mSkill->getAnimationBulletFlow(0);

			pInfo->attachPoint = pAnimationFlow->getAttachPoint();
			pInfo->offsetPoistion = pAnimationFlow->getOffsetPos();
			pInfo->offsetRotation = pAnimationFlow->getOffsetRotation();
			pInfo->casterModel = m_Frame->GetActorSettingEditor()->GetCurrentObject();
			pInfo->targetModel = m_Frame->GetActorSettingEditor()->GetTargetObject();
			pInfo->targetPoint = "身体中心点";

			pDObject->getCurrentSkill()->m_userData = pInfo;

		}
		
	}
	catch ( Ogre::Exception& e)
	{
		Ogre::String msg = "[Warning] Animation : " + mSkill->getSkillName() + "_getAnimationState Failed!";
		wxLogMessage(msg.c_str());		
	}
	mDObject = pDObject;
}
void
SkillObjectEditor::SetSceneManipulator(Fairy::SceneManipulator* sceneManipulator)
{
	mSceneManipulator = sceneManipulator;
	mSceneManipulator->addSceneListener(this);
}

//销毁效果物体
void SkillObjectEditor::DestroySkillObject()
{
	Fairy::LogicModel* pDObject = m_Frame->GetActorSettingEditor()->GetActorObject();
	if(pDObject)
		pDObject->delCurrentSkill();
}
//重新设置效果物体
void SkillObjectEditor::ResetSkillObject()
{
	Fairy::LogicModel* pDObject = m_Frame->GetActorSettingEditor()->GetActorObject();
	if(!pDObject)
		return;
	//pDObject->execute(0.0f);
	try
	{
		pDObject->createSkill( mSkillTemplateName, m_Frame->GetGraphDialog()->GetLoop());
		if(mSkill->getNumAnimationBulletFlows() > 0)
		{
			BulletFlowInfo* pInfo = new BulletFlowInfo;
			Fairy::AnimationBulletFlow* pAnimationFlow = mSkill->getAnimationBulletFlow(0);

			pInfo->attachPoint = pAnimationFlow->getAttachPoint();
			pInfo->offsetPoistion = pAnimationFlow->getOffsetPos();
			pInfo->offsetRotation = pAnimationFlow->getOffsetRotation();
			pInfo->casterModel = m_Frame->GetActorSettingEditor()->GetCurrentObject();
			pInfo->targetModel = m_Frame->GetActorSettingEditor()->GetTargetObject();
			pInfo->targetPoint = "身体中心点";

			pDObject->getCurrentSkill()->m_userData = pInfo;

		}
	}
	catch ( Ogre::Exception& e)
	{
	}
	//mSkill = Fairy::EffectManager::getSingleton().getSkill(mSkillTemplateName);
	m_Frame->GetGraphDialog()->InitSkillEditor(mSkill,mSkillTemplateName);

}
//刷新效果结构树
void SkillObjectEditor::RefreshSkillTree()
{
	m_SkillObjectTree->DeleteAllItems();
	m_SkillObjectTree->AddSkillToTree(mSkill);
}
//选择当前编辑页
void SkillObjectEditor::OnPageSelect()
{
	if(!mSkill)
		return;
	ResetSkillObject();
	m_Frame->GetEffectObjectProperty()->InitSkillEditor(mSkill,mSkillTemplateName);
	m_SkillObjectTree->refrashSelected();
}
void SkillObjectEditor::RefreshSkillTreeItem(const wxString& Desc,const wxString& propertyName,void* data)
{
	m_SkillObjectTree->refreshItem(Desc,propertyName,data);
}
//刷新效果数据
void SkillObjectEditor::OnRefreshSkillObject(wxCommandEvent &e)
{
	m_Frame->GetEffectObjectProperty()->RefreshEffectObject();
}
void SkillObjectEditor::SelectElementItem(Ogre::StringInterface* element)
{
	m_SkillObjectTree->SelectElementItem(element);
}
void SkillObjectEditor::EditRibbonMatrial(Fairy::AnimationRibbon* ribbon)
{
	if(!ribbon)
		return;
	EditMaterialDialog dlg(this,wxID_ANY,"编辑材质",ribbon->getMaterialName().c_str());
	if(dlg.ShowModal() == wxID_OK)
	{
		const Ogre::String newMatrialName = dlg.mTextCtrl->GetValue().c_str(); 
		Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(newMatrialName);
		if(material.isNull())
		{
			material = Ogre::MaterialManager::getSingleton().create(newMatrialName,"General");
		}
		ribbon->setMaterialName(newMatrialName);
		GetParentFrame()->GetEffectObjectProperty()->InitSkillEditor(mSkill,mSkillTemplateName);
		m_Frame->GetGraphDialog()->InitSkillEditor(mSkill,mSkillTemplateName);
		ResetSkillObject();
		RefreshSkillTree();
		m_SkillObjectTree->SelectElementItem(ribbon);
		m_Frame->GetMaterialEditor()->InitMaterialEditor(material,newMatrialName);
		m_Frame->GetEditorNotebook()->SetSelection(3);
	}

}

void SkillObjectEditor::OnNotifyToolBarCtrl(ToolbarCtrl index)
{
	wxCommandEvent e;
	if (index == TC_NEW_SKILL)
	{
		OnNewSkill(e);
	}
	else if (index == TC_SAVEAS_SKILL)
	{
		OnSaveAsSkillObject(e);
	}
	else if (index == TC_SAVE_SKILL)
	{
		OnSaveSkillObject(e);
	}
	else if (index == TC_DELETE_ElEMENT)
	{
		OnDeleteSkillElement(e);
	}
	else if (index == TC_NEW_EFFECT)
	{
		OnNewAnimEffect(e);
	}
	else if (index == TC_NEW_CAMERASHAKE)
	{
		OnNewBulletCameraShake(e);
	}
	else if (index == TC_NEW_RIBBON)
	{
		OnNewRibbon(e);
	}
	else if (index == TC_NEW_SOUND)
	{
		OnNewSound(e);
	}
	else if (index == TC_NEW_SCENELIGHT)
	{
		OnNewSceneLight(e);
	}
	else if (index == TC_NEW_BULLETFLOW)
	{
		OnNewMissile(e);
	}
}

void SkillObjectEditor::saveSkill(Fairy::Skill *skill, const Ogre::String &templateName, std::ofstream &of)
{
	assert (skill);

	of << "\n";
	of << "skill " << templateName << "\n";
	of << "{\n";

	const Ogre::ParameterList &paraList = skill->getParameters();

	for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
	{
		of << "\t" << paraList[paraCount].name << " " << skill->getParameter(paraList[paraCount].name) << "\n";
	}

	for (unsigned short i=0; i<skill->getNumAnimationEffectInfos(); ++i)
	{
		of << "\tAnimEffect\n" << "\t{\n";

		Fairy::AnimationEffectInfo *effectInfo = skill->getAnimationEffectInfo(i);
		assert (effectInfo);

		const Ogre::ParameterList &effectInfoList = effectInfo->getParameters();

		for ( size_t paraCount = 0; paraCount < effectInfoList.size(); ++paraCount )
		{
			of << "\t\t" << effectInfoList[paraCount].name.c_str() 
				<< " " << effectInfo->getParameter( effectInfoList[paraCount].name.c_str() ) << "\n";
		}

		of << "\t}\n";
	}

	for (unsigned short i=0; i<skill->getNumAnimationRibbons(); ++i)
	{
		of << "\tRibbon\n" << "\t{\n";

		Fairy::AnimationRibbon *ribbon = skill->getAnimationRibbon(i);
		assert (ribbon);

		const Ogre::ParameterList &ribbonList = ribbon->getParameters();

		for ( size_t paraCount = 0; paraCount < ribbonList.size(); ++paraCount )
		{
			of << "\t\t" << ribbonList[paraCount].name.c_str() 
				<< " " << ribbon->getParameter( ribbonList[paraCount].name.c_str() ) << "\n";
		}

		of << "\t}\n";
	}

	for (unsigned short i=0; i<skill->getNumAnimationCameraShake(); ++i)
	{
		of << "\tCameraShake\n" << "\t{\n";

			Fairy::AnimationCameraShake *cameraShake = skill->getAnimationCameraShake(i);
		assert (cameraShake);

		const Ogre::ParameterList &cameraShakeList = cameraShake->getParameters();

		for ( size_t paraCount = 0; paraCount < cameraShakeList.size(); ++paraCount )
		{
			of << "\t\t" << cameraShakeList[paraCount].name.c_str() 
				<< " " << cameraShake->getParameter( cameraShakeList[paraCount].name.c_str() ) << "\n";
		}

		of << "\t}\n";
	}

	for (unsigned short i=0; i<skill->getNumAnimationSounds(); ++i)
	{
		of << "\tSound\n" << "\t{\n";

		Fairy::AnimationSound *sound = skill->getAnimationSound(i);
		assert (sound);

		const Ogre::ParameterList &soundList = sound->getParameters();

		for ( size_t paraCount = 0; paraCount < soundList.size(); ++paraCount )
		{
			of << "\t\t" << soundList[paraCount].name.c_str() 
				<< " " << sound->getParameter( soundList[paraCount].name.c_str() ) << "\n";
		}

		of << "\t}\n";
	}

	for (unsigned short i=0; i<skill->getNumAnimationSceneLightInfos(); ++i)
	{
		of << "\tSceneLight\n" << "\t{\n";

		Fairy::AnimationSceneLightInfo *light = skill->getAnimationSceneLightInfo(i);
		assert (light);

		const Ogre::ParameterList &lightList = light->getParameters();

		for ( size_t paraCount = 0; paraCount < lightList.size(); ++paraCount )
		{
			of << "\t\t" << lightList[paraCount].name.c_str() 
				<< " " << light->getParameter( lightList[paraCount].name.c_str() ) << "\n";
		}

		of << "\t}\n";
	}

	for (unsigned short i=0; i<skill->getNumAnimationBulletFlows(); ++i)
	{
		of << "\tAnimBulletFlow\n" << "\t{\n";

		Fairy::AnimationBulletFlow *bulletFlow = skill->getAnimationBulletFlow(i);
		assert (light);

		const Ogre::ParameterList &bulletFlowList = bulletFlow->getParameters();

		for ( size_t paraCount = 0; paraCount < bulletFlowList.size(); ++paraCount )
		{
			of << "\t\t" << bulletFlowList[paraCount].name.c_str() 
				<< " " << bulletFlow->getParameter( bulletFlowList[paraCount].name.c_str() ) << "\n";
		}

		of << "\t}\n";
	}
	of << "}\n";

}

void SkillObjectEditor::OnSaveAsSkillObject(wxCommandEvent &e)
{
	if (NULL == mSkill)
	{
		wxMessageBox("您还没有创建技能，请创建后重试！","提示");
		return;
	}

	SaveAsSkillDialog *dlg = new SaveAsSkillDialog(this, wxID_ANY,
		_("另存技能"), mSkill);

	bool ok = (dlg->ShowModal() == wxID_OK);
	if(ok)
	{
		Ogre::StringVector templates;

		Ogre::String fileName = dlg->mComboBox->GetValue().c_str();
		Ogre::String templateName = dlg->mTextCtrl->GetValue().c_str();

		if(templateName.length() == 0)
		{
			dlg->Destroy();
			return;
		}

		//判断文件名
		Ogre::StringVector strings = Ogre::StringUtil::split( fileName, "." );
		if (strings.size() != 2 || strings[1] != "skill")
		{
			fileName.append(".skill");
		}

		Fairy::EffectManager::getSingleton().getSkillTemplatesFromScriptFile(fileName, templates);

		std::ofstream outFile;

		Ogre::String pathFileName = EFFECT_PATH+fileName;
		outFile.open ( pathFileName.c_str(), std::ios::out | std::ios::trunc ); // append to file

		if (!outFile.is_open())
		{
			dlg->Destroy();
			return;
		}

		bool newTemplate = true;


		// 把所有的模板都写入该文件中
		for ( size_t i=0; i<templates.size(); ++i )
		{
			//	if (templates[i] != mEffect->getTemplateName())
			///{
			Fairy::Skill *skill = Fairy::EffectManager::getSingleton().getSkill(templates[i]);
			assert (skill);

			if (skill->getSkillName() == dlg->mTextCtrl->GetValue().c_str())
			{
				saveSkill(mSkill, dlg->mTextCtrl->GetValue().c_str(), outFile );


				newTemplate = false;
			}
			else
				saveSkill(skill, skill->getSkillName(), outFile);

		}

		if (newTemplate)
		{
			
			// 刷新EffectManager中的模板内容
			Fairy::Skill *skill = Fairy::EffectManager::getSingleton().getSkill(templateName);
			if (NULL == skill)
			{
				skill = Fairy::EffectManager::getSingleton().createSkillTemplate(templateName);
			}

			*skill = *mSkill;

			saveSkill(skill, templateName, outFile );

			Fairy::EffectManager::getSingleton().addToSkillTemplateScriptFileMap(templateName, fileName);

			InitSkillEditor(skill, templateName);
		}

		outFile.close();
	}

	wxBusyInfo* busyInfo = new wxBusyInfo(wxT("更新技能数据 ..."), this);
	m_Frame->GetSkillSelector()->Reload();
	delete busyInfo;

	dlg->Destroy();

}
void SkillObjectEditor::OnSaveSkillObject(wxCommandEvent &e)
{
	if (NULL == mSkill)
	{
		wxMessageBox("您还没有创建技能，请创建后重试！","提示");
		return;
	}

	if(mSaveFileName == "")
	{
		SaveSkillDialog *dlg = new SaveSkillDialog(this, wxID_ANY,
			_("保存技能"), mSkill);

		bool ok = (dlg->ShowModal() == wxID_OK);
		if(ok)
		{
			mSaveFileName = dlg->mComboBox->GetValue().c_str();

			//判断文件名
			Ogre::StringVector strings = Ogre::StringUtil::split( mSaveFileName, "." );

			if (strings.size() != 2 || strings[1] != "skill")
			{
				mSaveFileName.append(".skill");
			}
			dlg->Destroy();
		}
		else
		{
			dlg->Destroy();
			return;

		}

	}
	//if(mbRecoverSave)
	//{
	//	char buf[256];
	//	sprintf(buf,"是否覆盖文件%s中的%s效果数据!",mSaveFileName.c_str(),mSkillTemplateName.c_str());
	//	RecoverSaveDialog* dlg = new RecoverSaveDialog(m_Frame,wxID_ANY,wxT("覆盖文件"));
	//	dlg->SetText(wxString(buf));


	//	if(wxID_OK == dlg->ShowModal())
	//	{
	//		mbRecoverSave = false;
	//		dlg->Destroy();
	//	}
	//	else
	//	{
	//		dlg->Destroy();
	//		return;
	//	}
	//}

	Ogre::StringVector templates;

	const Ogre::String fileName = mSaveFileName;


	Fairy::EffectManager::getSingleton().getSkillTemplatesFromScriptFile(fileName, templates);

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
		//	if (templates[i] != mEffect->getTemplateName())
		///{
		Fairy::Skill *skill = Fairy::EffectManager::getSingleton().getSkill(templates[i]);
		assert (skill);

		if (skill->getSkillName() == mSkillTemplateName)
		{
			saveSkill(mSkill, mSkillTemplateName, outFile );

			*skill = *mSkill;

			newTemplate = false;
		}
		else
			saveSkill(skill, skill->getSkillName(), outFile);

	}


	if (newTemplate)
	{
		saveSkill(mSkill, mSkillTemplateName, outFile );
		mSkill->setSkillName(mSkillTemplateName);
		Fairy::EffectManager::getSingleton().addToSkillTemplateScriptFileMap(mSkillTemplateName, fileName);
	}

	outFile.close();
}

void SkillObjectEditor::OnNewSkill(wxCommandEvent &e)
{
	AddSkillDialog *dlg = new AddSkillDialog(this, wxID_ANY, /*_("Add Skill")*/wxT("添加新技能") );
	dlg->Centre();
	bool ok = dlg->ShowModal() == wxID_OK;
	if(ok)
	{
		// 获取所选的动作名称
		wxString selectedAnimName = dlg->mAnimComboBox->GetStringSelection();
		wxString skillName = dlg->mSkillNameTextCtrl->GetValue();

		Fairy::Skill *pSkill = Fairy::EffectManager::getSingleton().createSkillTemplate(skillName.c_str());
		pSkill->setAnimationName(selectedAnimName.c_str());
		mSkillTemplateName = pSkill->getSkillName();

		//mDObject->createSkill(skill->getSkillName(),false, false,10.0f);// mAnimationFadeOutTime);

		// 刷新动作进度条
		m_Frame->GetGraphDialog()->InitAnimTimePosSlider();
		InitSkillEditor(pSkill,mSkillTemplateName);
	}

	dlg->Destroy();

}
Ogre::StringInterface* SkillObjectEditor::GetSelectedSkllElement()
{
	return m_SkillObjectTree->GetSelectSkillObject();
}
void SkillObjectEditor::SetRibbonMatrial(const Ogre::String name)
{
	Fairy::AnimationRibbon* pRibbon = dynamic_cast<Fairy::AnimationRibbon*>(m_SkillObjectTree->GetSelectSkillObject());
	if(!pRibbon)
		return;
	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(name);
	if(material.isNull())
	{
		material = Ogre::MaterialManager::getSingleton().create(name,"General");
	}
	pRibbon->setMaterialName(name);
	GetParentFrame()->GetEffectObjectProperty()->InitSkillEditor(mSkill,mSkillTemplateName);
	ResetSkillObject();
	RefreshSkillTree();
	m_SkillObjectTree->SelectElementItem(pRibbon);
	m_Frame->GetMaterialEditor()->InitMaterialEditor(material,name);
	m_Frame->GetEditorNotebook()->SetSelection(3);

}
void  SkillObjectEditor::OnDeleteSkillElement(wxCommandEvent &e)
{
	if(!m_SkillObjectTree->GetSelectSkillObject())
		return;

	int i = 0;

	for(i = 0;i<mSkill->getNumAnimationEffectInfos();i++)
	{
		Fairy::AnimationEffectInfo *pEffectInfo = mSkill->getAnimationEffectInfo(i);
		if(pEffectInfo == m_SkillObjectTree->GetSelectSkillObject())
		{
			mSkill->removeAnimationEffectInfo(i);
			ResetSkillObject();
			RefreshSkillTree();
			m_Frame->GetEffectObjectProperty()->InitSkillSettingProperty();
			m_Frame->GetGraphDialog()->InitSkillEditor(mSkill,mSkillTemplateName);
			return;
		}

	}

	for(i = 0;i<mSkill->getNumAnimationRibbons();i++)
	{
		Fairy::AnimationRibbon *pRibbon = mSkill->getAnimationRibbon(i);
		if(pRibbon == m_SkillObjectTree->GetSelectSkillObject())
		{
			mSkill->removeAnimationRibbon(i);
			ResetSkillObject();
			RefreshSkillTree();
			m_Frame->GetEffectObjectProperty()->InitSkillSettingProperty();
			m_Frame->GetGraphDialog()->InitSkillEditor(mSkill,mSkillTemplateName);
			return;
		}

	}

	for(i = 0;i<mSkill->getNumAnimationSounds();i++)
	{
		Fairy::AnimationSound *pSound = mSkill->getAnimationSound(i);
		if(pSound == m_SkillObjectTree->GetSelectSkillObject())
		{
			mSkill->removeAnimationSound(i);
			ResetSkillObject();
			RefreshSkillTree();
			m_Frame->GetEffectObjectProperty()->InitSkillSettingProperty();
			m_Frame->GetGraphDialog()->InitSkillEditor(mSkill,mSkillTemplateName);
			return;
		}

	}

	for(i = 0;i<mSkill->getNumAnimationSceneLightInfos();i++)
	{
		Fairy::AnimationSceneLightInfo *pLightInfo = mSkill->getAnimationSceneLightInfo(i);
		if(pLightInfo == m_SkillObjectTree->GetSelectSkillObject())
		{
			mSkill->removeAnimationSceneLightInfo(i);
			ResetSkillObject();
			RefreshSkillTree();
			m_Frame->GetEffectObjectProperty()->InitSkillSettingProperty();
			m_Frame->GetGraphDialog()->InitSkillEditor(mSkill,mSkillTemplateName);
			
			return;
		}
	}

	for(i = 0;i<mSkill->getNumAnimationBulletFlows();i++)
	{
		Fairy::AnimationBulletFlow *pBulletFlow = mSkill->getAnimationBulletFlow(i);
		if(pBulletFlow == m_SkillObjectTree->GetSelectSkillObject())
		{
			mSkill->removeAnimationBulletFlow(i);
			ResetSkillObject();
			RefreshSkillTree();
			m_Frame->GetEffectObjectProperty()->InitSkillSettingProperty();
			m_Frame->GetGraphDialog()->InitSkillEditor(mSkill,mSkillTemplateName);

			return;
		}
	}

	for(i = 0;i<mSkill->getNumAnimationCameraShake();i++)
	{
		Fairy::AnimationCameraShake *pCameraShake = mSkill->getAnimationCameraShake(i);
		if(pCameraShake == m_SkillObjectTree->GetSelectSkillObject())
		{
			mSkill->removeAnimationCameraShake(i);
			ResetSkillObject();
			RefreshSkillTree();
			m_Frame->GetEffectObjectProperty()->InitSkillSettingProperty();
			m_Frame->GetGraphDialog()->InitSkillEditor(mSkill,mSkillTemplateName);

			return;
		}
	}

}

void SkillObjectEditor::OnNewAnimEffect(wxCommandEvent &e)
{
	if (mSkill)
	{
		AddEffectInfoDialog *dlg = new AddEffectInfoDialog(this, wxID_ANY,
			_("Animation Effect Info"));

		bool ok = (dlg->ShowModal() == wxID_OK);
		if(ok)
		{
			Ogre::String effect = wxDynamicCast(dlg->FindWindow(feID_COMBOCTRL_EFFECT),wxComboBox)->GetValue().c_str();
			Ogre::String locator1 = wxDynamicCast(dlg->FindWindow(feID_COMBOCTRL_LOCATOR),wxComboBox)->GetValue().c_str();

			Fairy::AnimationEffectInfo *effectInfo = mSkill->addAnimationEffectInfo();
			assert (effectInfo);

			effectInfo->setEffectTemplateName(effect);
			effectInfo->setAttachPoint(locator1);

			ResetSkillObject();
			RefreshSkillTree();
			m_Frame->GetGraphDialog()->InitSkillEditor(mSkill,mSkillTemplateName);
			SelectElementItem(effectInfo);
		}

		dlg->Destroy();
	}
}
void SkillObjectEditor::OnNewRibbon(wxCommandEvent &e)
{
	if (mSkill)
	{
		AddRibbonDialog *dlg = new AddRibbonDialog(this, wxID_ANY,
			_("Animation Ribbon"));

		bool ok = (dlg->ShowModal() == wxID_OK);
		if(ok)
		{
			Ogre::String locator1 = wxDynamicCast(dlg->FindWindow(feID_COMBOCTRL_LOCATOR),wxComboBox)->GetValue().c_str();

			Fairy::AnimationRibbon *ribbon = mSkill->addAnimationRibbon();
			assert (ribbon);
			//			ribbon->setEffectTemplateName(effect);
			ribbon->setAttachPoint(locator1);

			ResetSkillObject();
			RefreshSkillTree();
			m_Frame->GetGraphDialog()->InitSkillEditor(mSkill,mSkillTemplateName);
			SelectElementItem(ribbon);
		}

		dlg->Destroy();
	}

}
void SkillObjectEditor::OnNewSound(wxCommandEvent &e)
{
	if (mSkill)
	{
		Fairy::AnimationSound *sound = mSkill->addAnimationSound();
		assert (sound);

		ResetSkillObject();
		RefreshSkillTree();
		m_Frame->GetGraphDialog()->InitSkillEditor(mSkill,mSkillTemplateName);
		SelectElementItem(sound);

	}

}
void SkillObjectEditor::OnNewSceneLight(wxCommandEvent &e)
{
	if (mSkill)
	{
		Fairy::AnimationSceneLightInfo *light = mSkill->addAnimationSceneLightInfo();
		assert (light);

		ResetSkillObject();
		RefreshSkillTree();
		m_Frame->GetGraphDialog()->InitSkillEditor(mSkill,mSkillTemplateName);
		SelectElementItem(light);
	}

}
void SkillObjectEditor::OnNewBulletCameraShake(wxCommandEvent &e)
{
	if(mSkill)
	{

		Fairy::AnimationCameraShake *cameraShake = mSkill->addAnimationCameraShake();
		assert (cameraShake);

		ResetSkillObject();
		RefreshSkillTree();
		m_Frame->GetGraphDialog()->InitSkillEditor(mSkill,mSkillTemplateName);
		SelectElementItem(cameraShake);
	}
}

void SkillObjectEditor::OnNewMissile(wxCommandEvent &e)
{
	if (mSkill)
	{
		Fairy::AnimationBulletFlow *bulletFlow = mSkill->addAnimationBulletFlow();
		assert (bulletFlow);

		ResetSkillObject();
		RefreshSkillTree();
		m_Frame->GetEffectObjectProperty()->InitSkillSettingProperty();
		m_Frame->GetGraphDialog()->InitSkillEditor(mSkill,mSkillTemplateName);
		SelectElementItem(bulletFlow);
	}
}

Fairy::Skill* SkillObjectEditor::getCurrentObjectSkill()
{
	Fairy::LogicModel* pDObject = m_Frame->GetActorSettingEditor()->GetActorObject();
	if(pDObject)
	{
		return pDObject->getCurrentSkill();
	}
	return NULL;
}