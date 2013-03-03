
#include "EffectObjectEditor.h"
#include "EffectObjectTree.h"
#include "EffectObjectPropertyEditor.h"
#include "Core/FairyObjectFactoryManager.h"
#include "EffectSystem/FairyEffectElement.h"
#include "EffectSystem/FairyParticleElement.h"
#include "EffectSystem/FairyProjectorElement.h"
#include "res/EffectObjectEditor_wdr.h"
#include "EffectSelector.h"
#include "ParticleSystemSelector.h"
#include "OgreParticleSystem.h"
#include "OgreParticleSystemRenderer.h"
#include "OgreParticleSystemManager.h"
#include "OgreMaterialManager.h"
#include "OgreParticleEmitterFactory.h"
#include "OgreParticleAffectorFactory.h"
#include "MaterialEditor.h"
#include <wx/image.h>
#include <wx/busyinfo.h>
#include <OgreStringVector.h>
///////////////////////////////////////////////////////////////////////////

#if wxUSE_STL
#define AS_STRING(s)    (s)
#else
#define AS_STRING(s)    ((s).c_str())
#endif


IMPLEMENT_DYNAMIC_CLASS(SaveAsEffectDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(SaveAsEffectDialog, wxDialog)
EVT_INIT_DIALOG(SaveAsEffectDialog::OnInitDialog)
END_EVENT_TABLE()

void
SaveAsEffectDialog::OnInitDialog(wxInitDialogEvent &e)
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
	Fairy::EffectManager::getSingleton().getLoadedScripts( loadedScripts );

	for ( size_t i=0; i<loadedScripts.size(); ++i )
	{
		if (loadedScripts[i] != "EmptyEffect.effect")
			mComboBox->AppendString(loadedScripts[i]);
	}

	// 若曾保存过，则显示上次保存的文件名
	EffectObjectEditor* pEffectObjectEditor = static_cast<EffectObjectEditor*>(mParent);
	if (!pEffectObjectEditor->getSaveFileName().empty())
	{
		Ogre::String fileName = Fairy::EffectManager::getSingleton().getFileNameByTemplateName(mEffect->getTemplateName());

		if ( fileName == "EmptyEffect.effect" )
			mComboBox->SetSelection(0);
		else
			mComboBox->SetValue(fileName);
	}
}

IMPLEMENT_DYNAMIC_CLASS(SaveEffectDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(SaveEffectDialog, wxDialog)
EVT_INIT_DIALOG(SaveEffectDialog::OnInitDialog)
END_EVENT_TABLE()

void
SaveEffectDialog::OnInitDialog(wxInitDialogEvent &e)
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


IMPLEMENT_DYNAMIC_CLASS(NewEffectDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(NewEffectDialog, wxDialog)
EVT_INIT_DIALOG(NewEffectDialog::OnInitDialog)
END_EVENT_TABLE()

void
NewEffectDialog::OnInitDialog(wxInitDialogEvent &e)
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

IMPLEMENT_DYNAMIC_CLASS(RecoverSaveDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(RecoverSaveDialog, wxDialog)
EVT_INIT_DIALOG(RecoverSaveDialog::OnInitDialog)
END_EVENT_TABLE()

void
RecoverSaveDialog::OnInitDialog(wxInitDialogEvent &e)
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

IMPLEMENT_DYNAMIC_CLASS(NewParticleElementDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(NewParticleElementDialog, wxDialog)
EVT_INIT_DIALOG(NewParticleElementDialog::OnInitDialog)
END_EVENT_TABLE()

void
NewParticleElementDialog::OnInitDialog(wxInitDialogEvent &e)
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer4->SetFlexibleDirection( wxBOTH );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxStaticText* staticText1 = new wxStaticText( this, wxID_ANY, wxT("粒子名称:"), wxDefaultPosition, wxSize( 60,-1 ), 0 );

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

	mComboBox->Clear();

	Ogre::String fileName = Fairy::EffectManager::getSingleton().getFileNameByTemplateName(mEffect->getTemplateName());
	Ogre::StringVector templateNames;
	Fairy::EffectManager::getSingleton().getParticleTemplatesFromScriptFile(fileName,templateNames);
	
	Ogre::StringVector::iterator it = templateNames.begin();
	while(it != templateNames.end())
	{
		Ogre::String templateName = *it;
		mComboBox->AppendString(AS_STRING(templateName));
		it ++;
		mComboBox->Select(0);
	}
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC_CLASS(NewParticleDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(NewParticleDialog, wxDialog)
EVT_INIT_DIALOG(NewParticleDialog::OnInitDialog)
END_EVENT_TABLE()

void
NewParticleDialog::OnInitDialog(wxInitDialogEvent &e)
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

	mTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 200,-1 ), 0 );
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


//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC_CLASS(NewParticleEmitterDlg, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(NewParticleEmitterDlg, wxDialog)
EVT_INIT_DIALOG(NewParticleEmitterDlg::OnInitDialog)
END_EVENT_TABLE()

NewParticleEmitterDlg::NewParticleEmitterDlg( wxWindow* parent, wxWindowID id, const wxString& title,
											 const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{

}

void NewParticleEmitterDlg::OnInitDialog(wxInitDialogEvent &e)
{
	wxDialog::OnInitDialog(e);

	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );

	m_staticText = new wxStaticText( this, wxID_ANY, wxT("粒子发射器类型:"), wxDefaultPosition, wxDefaultSize, 0 );
	//m_staticText->Wrap( -1 );
	bSizer3->Add( m_staticText, 0, wxALL, 5 );

	m_EmitterComboBox = new wxComboBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 160,-1 ), 0, NULL, 0 ); 
	bSizer3->Add( m_EmitterComboBox, 0, wxALL, 5 );

	Ogre::ParticleSystemManager::ParticleEmitterFactoryIterator it = Ogre::ParticleSystemManager::getSingleton().getEmitterFactoryIterator();
	while(it.hasMoreElements())
	{
		Ogre::ParticleEmitterFactory* pEmitterFactory = it.getNext();
		m_EmitterComboBox->AppendString(pEmitterFactory->getName());
		m_EmitterComboBox->Select(0);
	}
	m_EmitterComboBox->SetEditable(false);
	wxGridSizer* gSizer3;
	gSizer3 = new wxGridSizer( 2, 2, 0, 0 );

	m_OkBtn = new wxButton( this, wxID_OK, wxT("确定"), wxDefaultPosition, wxDefaultSize, 0 );
	m_OkBtn->SetDefault();
	gSizer3->Add( m_OkBtn, 0, wxALL, 5 );

	m_CancelBtn = new wxButton( this, wxID_CANCEL, wxT("取消"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer3->Add( m_CancelBtn, 0, wxALL, 5 );

	bSizer3->Add( gSizer3, 1, wxEXPAND, 5 );

	this->SetSizer( bSizer3 );
	this->Layout();
}
wxString NewParticleEmitterDlg::GetEmitterTypeName() const
{
	wxString typeName = m_EmitterComboBox->GetValue();
	return typeName;

}
NewParticleEmitterDlg::~NewParticleEmitterDlg()
{
}


//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC_CLASS(NewParticleAffectorDlg, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(NewParticleAffectorDlg, wxDialog)
EVT_INIT_DIALOG(NewParticleAffectorDlg::OnInitDialog)
END_EVENT_TABLE()

NewParticleAffectorDlg::NewParticleAffectorDlg( wxWindow* parent, wxWindowID id, const wxString& title,
											   const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{

}

void NewParticleAffectorDlg::OnInitDialog(wxInitDialogEvent &e)
{
	wxDialog::OnInitDialog(e);

	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );

	m_staticText = new wxStaticText( this, wxID_ANY, wxT("粒子影响器类型:"), wxDefaultPosition, wxDefaultSize, 0 );
	//m_staticText->Wrap( -1 );
	bSizer3->Add( m_staticText, 0, wxALL, 5 );

	m_AffectorComboBox = new wxComboBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 160,-1 ), 0, NULL, 0 ); 
	bSizer3->Add( m_AffectorComboBox, 0, wxALL, 5 );


	Ogre::ParticleSystemManager::ParticleAffectorFactoryIterator it = Ogre::ParticleSystemManager::getSingleton().getAffectorFactoryIterator();
	while(it.hasMoreElements())
	{
		Ogre::ParticleAffectorFactory* pAffectorFactory = it.getNext();
		m_AffectorComboBox->AppendString(pAffectorFactory->getName());
		m_AffectorComboBox->Select(0);
	}
	m_AffectorComboBox->SetEditable(false);

	wxGridSizer* gSizer3;
	gSizer3 = new wxGridSizer( 2, 2, 0, 0 );

	m_OkBtn = new wxButton( this, wxID_OK, wxT("确定"), wxDefaultPosition, wxDefaultSize, 0 );
	m_OkBtn->SetDefault();
	gSizer3->Add( m_OkBtn, 0, wxALL, 5 );

	m_CancelBtn = new wxButton( this, wxID_CANCEL, wxT("取消"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer3->Add( m_CancelBtn, 0, wxALL, 5 );

	bSizer3->Add( gSizer3, 1, wxEXPAND, 5 );

	this->SetSizer( bSizer3 );
	this->Layout();
}
wxString NewParticleAffectorDlg::GetAffectorTypeName() const
{
	wxString typeName = m_AffectorComboBox->GetValue();
	return typeName;

}
NewParticleAffectorDlg::~NewParticleAffectorDlg()
{
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC_CLASS(DeleteParticleObjectDlg, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(DeleteParticleObjectDlg, wxDialog)
EVT_INIT_DIALOG(DeleteParticleObjectDlg::OnInitDialog)
END_EVENT_TABLE()

DeleteParticleObjectDlg::DeleteParticleObjectDlg( wxWindow* parent, wxWindowID id, const wxString& title,
												 const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{

}

void DeleteParticleObjectDlg::OnInitDialog(wxInitDialogEvent &e)
{
	wxDialog::OnInitDialog(e);

	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );

	wxString text = wxT("删除") + m_ObjectType  + wxT(":") + m_ObjectName;

	m_staticText = new wxStaticText( this, wxID_ANY,text, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_staticText, 0, wxALL, 5 );

	wxGridSizer* gSizer3;
	gSizer3 = new wxGridSizer( 2, 2, 0, 0 );

	m_OkBtn = new wxButton( this, wxID_OK, wxT("确定"), wxDefaultPosition, wxDefaultSize, 0 );
	m_OkBtn->SetDefault(); 
	gSizer3->Add( m_OkBtn, 0, wxALL, 5 );

	m_CancelBtn = new wxButton( this, wxID_CANCEL, wxT("取消"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer3->Add( m_CancelBtn, 0, wxALL, 5 );

	bSizer3->Add( gSizer3, 1, wxEXPAND, 5 );

	this->SetSizer( bSizer3 );
	this->Layout();
}

DeleteParticleObjectDlg::~DeleteParticleObjectDlg()
{
}



static const int feID_DELETE_EFFECT_ELEMENT =wxNewId();
static const int feID_SAVE_EFFECT_OBJECT =wxNewId();
static const int feID_NEW_EFFECT =wxNewId();
static const int feID_SAVEAS_EFFECT_OBJECT = wxNewId();
static const int feID_NEW_EFFECT_PARTICLE =wxNewId();
static const int feID_NEW_EFFECT_MESH =wxNewId();
static const int feID_NEW_EFFECT_BILLBOARDSET =wxNewId();
static const int feID_NEW_EFFECT_PROJECTOR =wxNewId();
static const int feID_NEW_EFFECT_BEAM =wxNewId();
static const int feID_NEW_EFFECT_POINTLIGHT =wxNewId();
static const int feID_NEW_EFFECT_CYLINDER =wxNewId();
static const int feID_NEW_PARTICLE = wxNewId();
static const int feID_DELETE_PARTICLE_OBJECT = wxNewId();
static const int feID_ADD_PARTICLE_EMITTER =wxNewId();
static const int feID_ADD_PARTICLE_AFFECTOR =wxNewId();
static const int feID_PARTICLE_MATERIAL =wxNewId();


IMPLEMENT_DYNAMIC_CLASS(EffectObjectEditor, wxPanel)

BEGIN_EVENT_TABLE(EffectObjectEditor, wxPanel)
EVT_MENU(feID_DELETE_EFFECT_ELEMENT,EffectObjectEditor::OnDeleteEffectElement)
EVT_MENU(feID_SAVEAS_EFFECT_OBJECT,EffectObjectEditor::OnSaveAsEffectObject)
EVT_MENU(feID_SAVE_EFFECT_OBJECT,EffectObjectEditor::OnSaveEffectObject)
EVT_MENU(feID_NEW_EFFECT,EffectObjectEditor::OnNewEffect)
EVT_MENU(feID_NEW_EFFECT_PARTICLE,EffectObjectEditor::OnNewParticle)
EVT_MENU(feID_NEW_EFFECT_MESH,EffectObjectEditor::OnNewMesh)
EVT_MENU(feID_NEW_EFFECT_BILLBOARDSET,EffectObjectEditor::OnNewBillboardSet)
EVT_MENU(feID_NEW_EFFECT_PROJECTOR,EffectObjectEditor::OnNewPorjector)
EVT_MENU(feID_NEW_EFFECT_BEAM,EffectObjectEditor::OnNewBeam)
EVT_MENU(feID_NEW_EFFECT_POINTLIGHT,EffectObjectEditor::OnNewPointLight)
EVT_MENU(feID_NEW_EFFECT_CYLINDER,EffectObjectEditor::OnNewCylinder)
EVT_MENU(feID_ADD_PARTICLE_EMITTER,EffectObjectEditor::OnAddParitcleEmitter)
EVT_MENU(feID_ADD_PARTICLE_AFFECTOR,EffectObjectEditor::OnAddParitcleAffector)
EVT_MENU(feID_NEW_PARTICLE,EffectObjectEditor::OnNewParticleSystem)
EVT_ACTIVATE(EffectObjectEditor::OnShow)

END_EVENT_TABLE()


EffectObjectEditor::EffectObjectEditor( wxWindow* parent,
				   wxWindowID id,
				   const wxPoint& pos,
				   const wxSize& size,
				   long style) : mEffectTemplateName(""),mSaveFileName(""),mEffect(NULL), mConfig(NULL),
				   EFFECT_PATH("../resource/media/特效/特效/"),mbRecoverSave(false)
{
	Create(parent, id, pos, size, style);

}

bool
EffectObjectEditor::Create(wxWindow *parent, wxWindowID winid,
							   const wxPoint& pos, const wxSize& size,
							   long style)
{
	if (!wxPanel::Create(parent, winid, pos, size, style))
		return false;

	wxFlexGridSizer* sizer = new wxFlexGridSizer(1);
	sizer->AddGrowableCol(0);
	sizer->AddGrowableRow(1);


	m_toolBar = new wxToolBar( this, wxID_ANY, wxDefaultPosition, wxSize(-1,28)); 

	m_toolBar->AddTool(feID_NEW_EFFECT,
		wxBITMAP(NEW),wxBITMAP(NEW),false,
		0,wxDefaultCoord,NULL, wxT("创建效果数据"),wxT("创建效果数据"));

	m_toolBar->AddTool(feID_SAVEAS_EFFECT_OBJECT,
		wxBITMAP(SAVEAS),wxBITMAP(SAVEAS),false,
		1,wxDefaultCoord,NULL, wxT("另存效果数据"),wxT("另存效果数据"));

	m_toolBar->AddTool(feID_SAVE_EFFECT_OBJECT,
		wxBITMAP(SAVE),wxBITMAP(SAVE),false,
		2,wxDefaultCoord,NULL, wxT("保存效果数据"),wxT("保存效果数据"));

	m_toolBar->AddTool(feID_DELETE_EFFECT_ELEMENT,
		wxBITMAP(DELETE),wxBITMAP(DELETE),false,
		3,wxDefaultCoord,NULL, wxT("删除效果元素"),wxT("删除效果元素"));
	
	m_toolBar->AddSeparator();

	m_toolBar->AddTool(feID_NEW_EFFECT_PARTICLE,
		wxBITMAP(NEWPARTICLE),wxBITMAP(NEWPARTICLE),false,
		4,wxDefaultCoord,NULL, wxT("添加粒子效果元素"),wxT("添加粒子效果元素"));

	m_toolBar->AddTool(feID_NEW_EFFECT_MESH,
		wxBITMAP(NEWMESH),wxBITMAP(NEWMESH),false,
		5,wxDefaultCoord,NULL, wxT("添加模型效果元素"),wxT("添加模型效果元素"));

	m_toolBar->AddTool(feID_NEW_EFFECT_BILLBOARDSET,
		wxBITMAP(NEWBILLBOARDSET),wxBITMAP(NEWBILLBOARDSET),false,
		6,wxDefaultCoord,NULL, wxT("添加广告板效果元素"),wxT("添加广告板效果元素"));

	m_toolBar->AddTool(feID_NEW_EFFECT_PROJECTOR,
		wxBITMAP(NEWPROJECTOR),wxBITMAP(NEWPROJECTOR),false,
		7,wxDefaultCoord,NULL, wxT("添加投射效果元素"),wxT("添加投射效果元素"));

	m_toolBar->AddTool(feID_NEW_EFFECT_BEAM,
		wxBITMAP(NEWBEAM),wxBITMAP(NEWBEAM),false,
		8,wxDefaultCoord,NULL, wxT("添加光柱效果元素"),wxT("添加光柱效果元素"));

	m_toolBar->AddTool(feID_NEW_EFFECT_POINTLIGHT,
		wxBITMAP(NEWPOINTLIGHT),wxBITMAP(NEWPOINTLIGHT),false,
		9,wxDefaultCoord,NULL, wxT("添加点光源果元素"),wxT("添加点光源效果元素"));

	m_toolBar->AddTool(feID_NEW_EFFECT_CYLINDER,
		wxBITMAP(NEWCYLINDER),wxBITMAP(NEWCYLINDER),false,
		10,wxDefaultCoord,NULL, wxT("添加圆柱模型元素"),wxT("添加圆柱模型元素"));

	m_toolBar->AddTool(feID_NEW_PARTICLE,
		wxBITMAP(NEWPARTICLESYSTEM),wxBITMAP(NEWPARTICLESYSTEM),false,
		11,wxDefaultCoord,NULL, wxT("添加粒子数据"), wxT("添加粒子数据"));

	m_toolBar->AddSeparator();

	m_toolBar->AddTool(feID_ADD_PARTICLE_EMITTER,
		wxBITMAP(NEWPARTICLEEMITTER),wxBITMAP(NEWPARTICLEEMITTER),false,
		12,wxDefaultCoord,NULL, wxT("添加粒子发射器"), wxT("添加粒子发射器"));

	m_toolBar->AddTool(feID_ADD_PARTICLE_AFFECTOR,
		wxBITMAP(NEWPARTICLEAFFECTOR),wxBITMAP(NEWPARTICLEAFFECTOR),false,
		13,wxDefaultCoord,NULL, wxT("添加粒子影响器"), wxT("添加粒子影响器"));


	m_toolBar->Realize();
	
	
	sizer->Add( m_toolBar, 0, wxEXPAND, 0 );

	m_EffectObjectTree = new EffectObjectTree( this, ID_EFFECT_OBJECT_TREE, wxDefaultPosition, wxDefaultSize);
	sizer->Add( m_EffectObjectTree, 0, wxGROW|wxALL, 0 );

	this->SetSizer( sizer );
	sizer->SetSizeHints(this);

	return true;
}

EffectObjectEditor::~EffectObjectEditor()
{
	
}


void EffectObjectEditor::InitEffectObject()
{

}
void EffectObjectEditor::InitParticleEditor(Ogre::ParticleSystem* pParticleSystem,const Ogre::String &name,bool isNew = false)
{
	if(!pParticleSystem)
		return;
	Ogre::String filename = Fairy::EffectManager::getSingleton().getFileNameByParticleTemplateName(name);
	Ogre::StringVector effectTemplates;
	Fairy::EffectManager::getSingleton().getTemplatesFromScriptFile(filename,effectTemplates);
	if(!effectTemplates.empty())
	{
		Ogre::String templateName = effectTemplates[0];
		Fairy::Effect* pEffectTemplate = Fairy::EffectManager::getSingleton().getTemplate(templateName);
		if(pEffectTemplate)
			InitEffectEditor(pEffectTemplate,templateName,isNew);

	}

}
void EffectObjectEditor::ReInitEffectEditor()
{
	if(!mEffect)
		return;
	InitEffectEditor(mEffect,mEffectTemplateName);
}
void EffectObjectEditor::InitEffectEditor(Fairy::Effect* effect,const Ogre::String &templateName,bool isNew)
{
	mEffectTemplateName = templateName;
	mEffect = effect;

	for(int i =0;i<mEffect->getNumElements();i++)
		mEffect->getElement(i)->setVisible(true);
	mSaveFileName = Fairy::EffectManager::getSingleton().getFileNameByTemplateName(templateName);
	if(!isNew)
	{
		
		mbRecoverSave = true;
	}
	else
	{
		mbRecoverSave = false;
	}
	mEffect->createSceneNode();
	if(mEffectObject)
		mSceneManipulator->deselectObject(mEffectObject);
	mEffectObject.reset();
	mParameters["effect name"] = AS_STRING(mEffectTemplateName);
	mEffectObject = Fairy::ObjectFactoryManager::getSingleton().createInstance("Effect");
	mEffectObject->setPropertyList(mParameters);
	mEffectObject->createRenderInstance(mSceneManipulator);

	Ogre::Vector3 position = Ogre::Vector3::ZERO;
	mEffectObject->setProperty("position", position);
	mSceneManipulator->selectObject(mEffectObject);


	Ogre::String fileName = Fairy::EffectManager::getSingleton().getFileNameByTemplateName(templateName);
	Ogre::StringVector particletemplates;
	Fairy::EffectManager::getSingleton().getParticleTemplatesFromScriptFile(fileName, particletemplates);
	mParticleSystemMap.clear();
	Ogre::StringVector::iterator it= particletemplates.begin();
	while(it != particletemplates.end())
	{
		Ogre::String templateName = *it;
		Ogre::ParticleSystem* pBulletSystem  = Ogre::ParticleSystemManager::getSingleton().getTemplate(templateName);
		if(pBulletSystem)
		{
			mParticleSystemMap.insert(std::map<Ogre::String,Ogre::ParticleSystem*>::value_type(templateName,pBulletSystem));
		}
		it ++;
	}

	GetParentFrame()->GetEffectObjectProperty()->InitEffectEditor(mEffect,templateName);
	GetParentFrame()->GetGraphDialog()->InitEffectEditor(mEffect,templateName);

	m_EffectObjectTree->DeleteAllItems();
	m_EffectObjectTree->AddEffectToTree(mEffect,templateName);
	std::map<Ogre::String,Ogre::ParticleSystem*>::iterator ps_it = mParticleSystemMap.begin();
	while(ps_it != mParticleSystemMap.end())
	{
		m_EffectObjectTree->AddParticleToTree(ps_it->second,ps_it->first);
		ps_it ++;
	}

	//GetParentFrame()->GetEffectObjectProperty()->InitParticleSettingProperty();


}
void
EffectObjectEditor::SetSceneManipulator(Fairy::SceneManipulator* sceneManipulator)
{
	mSceneManipulator = sceneManipulator;
	mSceneManipulator->addSceneListener(this);
}

//销毁效果物体
void EffectObjectEditor::DestroyEffectObject()
{
	if(mEffectObject)
		mSceneManipulator->deselectObject(mEffectObject);
	mEffectObject.reset();
}

void EffectObjectEditor::UpdateLocatorNode(bool bVisible)
{
	if (!bVisible)
	{
		GetSceneManipulator()->setLocatorVisible(false);
	}
	else
	{
		GetSceneManipulator()->setLocatorVisible(true);

		Fairy::EffectObject* pEffectObject = getCurrentEffectObject();
		if(pEffectObject)
		{
			Ogre::SceneNode* pEffectNode = pEffectObject->getSceneNode();
			Ogre::SceneNode* pLocatorNode = GetSceneManipulator()->getLocatorNode();
			Ogre::SceneNode* pParentNode = pLocatorNode->getParentSceneNode();
			if(pParentNode)
				pParentNode->removeChild(pLocatorNode);
			pEffectNode->addChild(pLocatorNode);
			//pLocatorNode->setOrientation(pElement->getRelOrientation());
		}
	}
}

//重新设置效果物体
void EffectObjectEditor::ResetEffectObject()
{
	if(mEffectObject)
		mSceneManipulator->deselectObject(mEffectObject);
	mEffectObject.reset();
	mParameters["effect name"] = AS_STRING(mEffectTemplateName);
	mEffectObject = Fairy::ObjectFactoryManager::getSingleton().createInstance("Effect");
	mEffectObject->setPropertyList(mParameters);
	mEffectObject->createRenderInstance(mSceneManipulator);
	Ogre::Vector3 position = Ogre::Vector3::ZERO;
	mEffectObject->setProperty("position", position);
	mSceneManipulator->selectObject(mEffectObject);
	m_Frame->GetGraphDialog()->InitEffectEditor(mEffect,mEffectTemplateName);
}
//刷新效果结构树
void EffectObjectEditor::RefreshEffectTree()
{
	m_EffectObjectTree->DeleteAllItems();
	m_EffectObjectTree->AddEffectToTree(mEffect,mEffectTemplateName);
	m_Frame->GetGraphDialog()->InitEffectEditor(mEffect,mEffectTemplateName);
}
//选择当前编辑页
void EffectObjectEditor::OnPageSelect()
{
	if(!mEffect)
		return;
	this->ResetEffectObject();
	m_Frame->GetEffectObjectProperty()->InitEffectEditor(mEffect,mEffectTemplateName);
	m_EffectObjectTree->refrashSelected();
}
Fairy::Effect* EffectObjectEditor::getCurrentObjectEffect()
{
	if(!mEffectObject)
		return NULL;
	Fairy::EffectObject* pEffectObject = dynamic_cast<Fairy::EffectObject*>(mEffectObject.get());
	if(pEffectObject)
	{
		return pEffectObject->getEffect();
	}
	return NULL;
}
Fairy::EffectObject* EffectObjectEditor::getCurrentEffectObject()
{
	if(!mEffectObject)
		return NULL;
	Fairy::EffectObject* pEffectObject = dynamic_cast<Fairy::EffectObject*>(mEffectObject.get());
	if(pEffectObject)
	{
		return pEffectObject;
	}
	return NULL;
}
Ogre::StringInterface* EffectObjectEditor::GetSelectedObject()
{
	Ogre::StringInterface* pSelectObject = m_EffectObjectTree->GetSelectEffectObject();
	if(pSelectObject)
	{
		return pSelectObject;
	}
	return NULL;
}
void EffectObjectEditor::RefreshEffectTreeItem(const wxString& Desc,const wxString& propertyName,void* data)
{
	m_EffectObjectTree->refreshItem(Desc,propertyName,data);
}
void EffectObjectEditor::SelectElementItem(Fairy::EffectElement* element)
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
					//m_Frame->GetParticleObjectEditor()->InitParticleEditor(pParticleSystem,particleName);
				}
			}
			
		}
	}
	m_Frame->GetGraphDialog()->InitEffectEditor(mEffect,mEffectTemplateName);
	m_Frame->GetEffectObjectProperty()->InitEffectEditor(mEffect,mEffectTemplateName);
	m_EffectObjectTree->SelectElementItem(element);
}
void EffectObjectEditor::EditMeshMatrial(Fairy::MeshElement* meshElement)
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
		GetParentFrame()->GetEffectObjectProperty()->InitEffectEditor(mEffect,mEffectTemplateName);
		m_Frame->GetGraphDialog()->InitEffectEditor(mEffect,mEffectTemplateName);
		ResetEffectObject();
		RefreshEffectTree();
		m_EffectObjectTree->SelectElementItem(meshElement);
		m_Frame->GetMaterialEditor()->InitMaterialEditor(material,newMatrialName);
		m_Frame->GetEditorNotebook()->SetSelection(2);
	}

}
void EffectObjectEditor::EditBillboardMatrial(Fairy::BillboardSetElement* billboardElement)
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
		m_Frame->GetEffectObjectProperty()->InitEffectEditor(mEffect,mEffectTemplateName);
		m_Frame->GetGraphDialog()->InitEffectEditor(mEffect,mEffectTemplateName);
		m_Frame->GetGraphDialog()->InitEffectEditor(mEffect,mEffectTemplateName);
		ResetEffectObject();
		RefreshEffectTree();
		m_EffectObjectTree->SelectElementItem(billboardElement);
		m_Frame->GetMaterialEditor()->InitMaterialEditor(material,newMatrialName);
		m_Frame->GetEditorNotebook()->SetSelection(2);
	}
}
void EffectObjectEditor::EditProjectorMatrial(Fairy::ProjectorElement* projectorElement)
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
		m_Frame->GetEffectObjectProperty()->InitEffectEditor(mEffect,mEffectTemplateName);
		ResetEffectObject();
		RefreshEffectTree();
		m_EffectObjectTree->SelectElementItem(projectorElement);
		m_Frame->GetMaterialEditor()->InitMaterialEditor(material,newMatrialName);
		m_Frame->GetEditorNotebook()->SetSelection(2);
	}
}
void EffectObjectEditor::EditBeamMatrial(Fairy::BeamElement* beamElement)
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
		m_Frame->GetEffectObjectProperty()->InitEffectEditor(mEffect,mEffectTemplateName);
		ResetEffectObject();
		RefreshEffectTree();
		m_EffectObjectTree->SelectElementItem(beamElement);
		m_Frame->GetMaterialEditor()->InitMaterialEditor(material,newMatrialName);
		m_Frame->GetEditorNotebook()->SetSelection(2);
	}
}
void EffectObjectEditor::EditCylinderMatrial(Fairy::CylinderElement* cylinderElement)
{
	if(!cylinderElement)
		return;
	EditMaterialDialog dlg(this,wxID_ANY,"编辑材质",cylinderElement->getMaterialName().c_str());
	if(dlg.ShowModal() == wxID_OK)
	{
		const Ogre::String newMatrialName = dlg.mTextCtrl->GetValue().c_str(); 
		Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(newMatrialName);
		if(material.isNull())
		{
			material = Ogre::MaterialManager::getSingleton().create(newMatrialName,"General");
		}
		cylinderElement->setMaterialName(newMatrialName);
		m_Frame->GetEffectObjectProperty()->InitEffectEditor(mEffect,mEffectTemplateName);
		ResetEffectObject();
		RefreshEffectTree();
		m_EffectObjectTree->SelectElementItem(cylinderElement);
		m_Frame->GetMaterialEditor()->InitMaterialEditor(material,newMatrialName);
		m_Frame->GetEditorNotebook()->SetSelection(2);
	}

}
void EffectObjectEditor::SetMeshMatrial(const Ogre::String name)
{
	Fairy::MeshElement* meshElement = dynamic_cast<Fairy::MeshElement*>(m_EffectObjectTree->GetSelectEffectObject());
	if(!meshElement)
		return;
	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(name);
	if(material.isNull())
	{
		material = Ogre::MaterialManager::getSingleton().create(name,"General");
	}
	meshElement->setMaterialName(name);
	GetParentFrame()->GetEffectObjectProperty()->InitEffectEditor(mEffect,mEffectTemplateName);
	InitEffectEditor(mEffect,mEffectTemplateName);
	m_EffectObjectTree->SelectElementItem(meshElement);
	m_Frame->GetMaterialEditor()->InitMaterialEditor(material,name);
	m_Frame->GetEditorNotebook()->SetSelection(2);

}
void EffectObjectEditor::SetBillboardMatrial(const Ogre::String name)
{
	Fairy::BillboardSetElement* billboardElement = dynamic_cast<Fairy::BillboardSetElement*>(m_EffectObjectTree->GetSelectEffectObject());
	if(!billboardElement)
		return;
	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(name);
	if(material.isNull())
	{
		material = Ogre::MaterialManager::getSingleton().create(name,"General");
	}
	billboardElement->setBillboardMaterial(name);
	GetParentFrame()->GetEffectObjectProperty()->InitEffectEditor(mEffect,mEffectTemplateName);
	InitEffectEditor(mEffect,mEffectTemplateName);
	m_EffectObjectTree->SelectElementItem(billboardElement);
	m_Frame->GetMaterialEditor()->InitMaterialEditor(material,name);
	m_Frame->GetEditorNotebook()->SetSelection(2);

}
void EffectObjectEditor::SetProjectorMatrial(const Ogre::String name)
{
	Fairy::ProjectorElement* projectorElement = dynamic_cast<Fairy::ProjectorElement*>(m_EffectObjectTree->GetSelectEffectObject());
	if(!projectorElement)
		return;
	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(name);
	if(material.isNull())
	{
		material = Ogre::MaterialManager::getSingleton().create(name,"General");
	}
	projectorElement->setMaterialName(name);
	GetParentFrame()->GetEffectObjectProperty()->InitEffectEditor(mEffect,mEffectTemplateName);
	InitEffectEditor(mEffect,mEffectTemplateName);
	m_EffectObjectTree->SelectElementItem(projectorElement);
	m_Frame->GetMaterialEditor()->InitMaterialEditor(material,name);
	m_Frame->GetEditorNotebook()->SetSelection(2);

}
void EffectObjectEditor::SetBeamMatrial(const Ogre::String name)
{
	Fairy::BeamElement* beamElement = dynamic_cast<Fairy::BeamElement*>(m_EffectObjectTree->GetSelectEffectObject());
	if(!beamElement)
		return;
	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(name);
	if(material.isNull())
	{
		material = Ogre::MaterialManager::getSingleton().create(name,"General");
	}
	beamElement->setMaterialName(name);
	GetParentFrame()->GetEffectObjectProperty()->InitEffectEditor(mEffect,mEffectTemplateName);
	InitEffectEditor(mEffect,mEffectTemplateName);
	m_EffectObjectTree->SelectElementItem(beamElement);
	m_Frame->GetMaterialEditor()->InitMaterialEditor(material,name);
	m_Frame->GetEditorNotebook()->SetSelection(2);

}

void EffectObjectEditor::SetParticleMaterial(const Ogre::String name)
{
	Ogre::ParticleSystem* pParticleSystem = 
		dynamic_cast<Ogre::ParticleSystem*>(m_EffectObjectTree->GetSelectEffectObject());
	if(!pParticleSystem)
		return;
	if(name.length() == 0)
	{
		return;
	}
	Ogre::MaterialPtr pMaterial= Ogre::MaterialManager::getSingleton().getByName(name);
	//Ogre::MaterialPtr *pMaterial;
	if(pMaterial.isNull())
		pMaterial = Ogre::MaterialManager::getSingleton().create(name,"General");
	pParticleSystem->setMaterialName(name);

	GetParentFrame()->GetEffectObjectProperty()->InitEffectEditor(mEffect,mEffectTemplateName);
	InitEffectEditor(mEffect,mEffectTemplateName);
	m_EffectObjectTree->SelectParticleSetting(pParticleSystem,true);
	m_Frame->GetMaterialEditor()->InitMaterialEditor(pMaterial,name);
	m_Frame->GetEditorNotebook()->SetSelection(2);
}

void EffectObjectEditor::OnNotifyToolBarCtrl(ToolbarCtrl index)
{
	wxCommandEvent e;
	if (index == TC_NEW_EFFECT)
	{
		OnNewEffect(e);
	}
	else if (index == TC_SAVEAS_EFFECT)
	{
		OnSaveAsEffectObject(e);
	}
	else if(index == TC_SAVE_EFFECT)
	{
		OnSaveEffectObject(e);
	}
	else if (index == TC_DELETE_ElEMENT)
	{
		OnDeleteEffectElement(e);
	}
	else if (index == TC_NEW_PARTICLE)
	{
		OnNewParticle(e);
	}
	else if(index == TC_NEW_MESH)
	{
		OnNewMesh(e);
	}
	else if (index == TC_NEW_BILLBOARDSET)
	{
		OnNewBillboardSet(e);
	}
	else if (index == TC_NEW_PROJECTOR)
	{
		OnNewPorjector(e);
	}	
	else if (index == TC_NEW_BEAM)
	{
		OnNewBeam(e);
	}
	else if (index == TC_NEW_POINTLIGHT)
	{
		OnNewPointLight(e);
	}	
	else if(index == TC_NEW_CYLINDER)
	{
		OnNewCylinder(e);
	}
	else if(index == TC_NEW_PARTICLE_SYSTEM)
	{
		OnNewParticleSystem(e);
	}
	else if (index == TC_NEW_EMITTER)
	{
		OnAddParitcleEmitter(e);
	}
	else if (index == TC_NEW_AFFECTOR)
	{
		OnAddParitcleAffector(e);
	}
}


//刷新效果数据
void EffectObjectEditor::OnSaveAsEffectObject(wxCommandEvent &e)
{
	if (NULL == mEffect)
	{
		wxMessageBox("您还没有创建效果，请创建后重试！","提示");
		return;
	}

	SaveAsEffectDialog *dlg = new SaveAsEffectDialog(this, wxID_ANY,
		_("另存特效"), mEffect);

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
		if (strings.size() != 2 || strings[1] != "effect")
		{
			fileName.append(".effect");
		}

		Fairy::EffectManager::getSingleton().getTemplatesFromScriptFile(fileName, templates);

		std::ofstream outFile;

		Ogre::String pathFileName = EFFECT_PATH+fileName;
		outFile.open ( pathFileName.c_str(), std::ios::out | std::ios::trunc ); // append to file

		if (!outFile.is_open())
		{
			return;
		}

		int startpos = outFile.tellp();
		bool newTemplate = true;
		std::map<Ogre::String,Ogre::ParticleSystem*> particleSystemMap;

		// 把所有的模板都写入该文件中
		for ( size_t i=0; i<templates.size(); ++i )
		{
			//	if (templates[i] != mEffect->getTemplateName())
			///{
			Fairy::Effect *effect = Fairy::EffectManager::getSingleton().getTemplate(templates[i]);
			assert (effect);

			if (effect->getTemplateName() == dlg->mTextCtrl->GetValue().c_str())
			{
				saveEffect(mEffect, dlg->mTextCtrl->GetValue().c_str(), outFile, fileName);

				//*effect = *mEffect;

				newTemplate = false;
			}
			else
			{
				saveEffect(effect, effect->getTemplateName(), outFile, fileName);
			}
			Ogre::StringVector ParticleTemplates;
			Fairy::EffectManager::getSingleton().getParticleTemplatesFromScriptFile(fileName,ParticleTemplates);
			Ogre::StringVector::iterator it = ParticleTemplates.begin();
			while(it != ParticleTemplates.end())
			{
				Ogre::String name = *it;
				Ogre::ParticleSystem* pParticleSystem = Ogre::ParticleSystemManager::getSingleton().getTemplate(name);
				particleSystemMap.insert(std::map<Ogre::String,Ogre::ParticleSystem*>::value_type(name,pParticleSystem));
				it ++;
			}


		}

		if (newTemplate)
		{

			// 刷新EffectManager中的模板内容
			Fairy::Effect *effect = Fairy::EffectManager::getSingleton().getTemplate(templateName);
			if (NULL == effect)
			{
				effect = Fairy::EffectManager::getSingleton().createEffectTemplate(templateName);
			}

			*effect = *mEffect;

			saveEffect(effect, templateName, outFile, fileName);
			Fairy::EffectManager::getSingleton().addToEffectTemplateScriptFileMap(templateName, fileName);

			std::map<Ogre::String,Ogre::ParticleSystem*>::iterator it = mParticleSystemMap.begin();
			while(it != mParticleSystemMap.end())
			{
				Ogre::String name = it->first;
				Ogre::ParticleSystem* pParticleSystem = it->second;
				Fairy::EffectManager::getSingleton().removeParticleTemplateScriptFileMap(name);
				Fairy::EffectManager::getSingleton().addToParticleTemplateScriptFileMap(name,fileName);

				particleSystemMap.insert(std::map<Ogre::String,Ogre::ParticleSystem*>::value_type(name,pParticleSystem));
				it ++;
			}

			InitEffectEditor(effect,templateName);
		}


		std::map<Ogre::String,Ogre::ParticleSystem*>::iterator it = particleSystemMap.begin();
		while(it != particleSystemMap.end())
		{
			Ogre::String name = it->first;
			Ogre::ParticleSystem* pParticleSystem = it->second;
			if(NULL != pParticleSystem)
			{
				saveParticleSystem(pParticleSystem,name,outFile);
			}
			it ++;
		}

		int endpos = outFile.tellp();

		outFile.close();

		// 保证不保存空文件
		if ((0 == startpos) && (startpos == endpos))
		{
			try
			{
				DeleteFile(pathFileName.c_str());
			}
			catch (...)
			{
				throw;
			}
		}

		mSaveFileName = fileName;
	}

	wxBusyInfo* busyInfo = new wxBusyInfo(wxT("更新效果数据 ..."), this);
	m_Frame->GetEffectSelector()->Reload();
	delete busyInfo;
	
	mbRecoverSave = false;
	
	dlg->Destroy();
	
}
//保存文件
void EffectObjectEditor::OnSaveEffectObject(wxCommandEvent &e)
{
	if (NULL == mEffect)
	{
		wxMessageBox("您还没有创建效果，请创建后重试！","提示");
		return;
	}

	if(mSaveFileName == "")
	{
		SaveEffectDialog *dlg = new SaveEffectDialog(this, wxID_ANY,
			_("保存特效"), mEffect);

		bool ok = (dlg->ShowModal() == wxID_OK);
		if(ok)
		{
			mSaveFileName = dlg->mComboBox->GetValue().c_str();

			//判断文件名
			Ogre::StringVector strings = Ogre::StringUtil::split( mSaveFileName, "." );

			if (strings.size() != 2 || strings[1] != "effect")
			{
				mSaveFileName.append(".effect");
			}
			dlg->Destroy();
		}
		else
		{
			dlg->Destroy();
			return;

		}

	}
	if(mbRecoverSave)
	{
		char buf[256];
		sprintf(buf,"是否覆盖文件%s中的%s效果数据!",mSaveFileName.c_str(),mEffectTemplateName.c_str());
		RecoverSaveDialog* dlg = new RecoverSaveDialog(m_Frame,wxID_ANY,wxT("覆盖文件"));
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

	Ogre::StringVector templates;

	const Ogre::String fileName = mSaveFileName;


	Fairy::EffectManager::getSingleton().getTemplatesFromScriptFile(fileName, templates);

	std::ofstream outFile;

	Ogre::String pathFileName = EFFECT_PATH+fileName;
	outFile.open ( pathFileName.c_str(), std::ios::out | std::ios::trunc ); // append to file

	if (!outFile.is_open())
	{
		return;
	}

	int startpos = outFile.tellp();
	bool newTemplate = true;
	std::map<Ogre::String,Ogre::ParticleSystem*> particleSystemMap;
	// 把所有的模板都写入该文件中
	for ( size_t i=0; i<templates.size(); ++i )
	{
		//	if (templates[i] != mEffect->getTemplateName())
		///{
		Fairy::Effect *effect = Fairy::EffectManager::getSingleton().getTemplate(templates[i]);
		assert (effect);

		if (effect->getTemplateName() == mEffectTemplateName)
		{
			saveEffect(mEffect, mEffectTemplateName, outFile, fileName);

			newTemplate = false;
		}
		else
		{
			saveEffect(effect, effect->getTemplateName(), outFile, fileName);
		}
		Ogre::StringVector ParticleTemplates;
		Fairy::EffectManager::getSingleton().getParticleTemplatesFromScriptFile(fileName,ParticleTemplates);
		Ogre::StringVector::iterator it = ParticleTemplates.begin();
		while(it != ParticleTemplates.end())
		{
			Ogre::String name = *it;
			Ogre::ParticleSystem* pParticleSystem = Ogre::ParticleSystemManager::getSingleton().getTemplate(name);
			particleSystemMap.insert(std::map<Ogre::String,Ogre::ParticleSystem*>::value_type(name,pParticleSystem));
			it ++;
		}

	}


	if (newTemplate)
	{
		saveEffect(mEffect, mEffectTemplateName, outFile, fileName);

		Fairy::EffectManager::getSingleton().addToEffectTemplateScriptFileMap(mEffectTemplateName, fileName);
	}


	std::map<Ogre::String,Ogre::ParticleSystem*>::iterator it = particleSystemMap.begin();
	while(it != particleSystemMap.end())
	{
		Ogre::String name = it->first;
		Ogre::ParticleSystem* pParticleSystem = it->second;
		if(NULL != pParticleSystem)
		{
			saveParticleSystem(pParticleSystem,name,outFile);
		}
		it ++;
	}

	int endpos = outFile.tellp();

	outFile.close();

	// 保证不保存空文件
	if ((0 == startpos) && (startpos == endpos))
	{
		try
		{
			DeleteFile(pathFileName.c_str());
		}
		catch (...)
		{
			throw;
		}
	}
}

void EffectObjectEditor::OnNewEffect(wxCommandEvent &e)
{
	NewEffectDialog *dlg = new NewEffectDialog(this, wxID_ANY,
		wxT("新建特效"));

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
		
		
		Fairy::Effect *effect = Fairy::EffectManager::getSingleton().getTemplate(templateName);
		if(!effect)
			effect = Fairy::EffectManager::getSingleton().createEffectTemplate(templateName);
		Ogre::String fileName = templateName + ".effect";
		Fairy::EffectManager::getSingleton().addToEffectTemplateScriptFileMap(templateName,fileName);

		InitEffectEditor(effect,templateName,true);

		wxBusyInfo* busyInfo = new wxBusyInfo(wxT("更新效果数据 ..."), this);
		m_Frame->GetEffectSelector()->Reload();
		delete busyInfo;
	}
	dlg->Destroy();

}
void  EffectObjectEditor::OnDeleteEffectElement(wxCommandEvent &e)
{
	if(!m_EffectObjectTree->GetSelectEffectObject())
		return;

	for(int i = 0;i<mEffect->getNumElements();i++)
	{
		if(mEffect->getElement(i) == m_EffectObjectTree->GetSelectEffectObject())
		{
			mEffect->destroyElement(i);
			RefreshEffectTree();
			m_Frame->GetEffectObjectProperty()->InitEffectSettingProperty();
			InitEffectEditor(mEffect,mEffectTemplateName);
			return;
		}
	}

	Ogre::StringInterface* pParticleObject = m_EffectObjectTree->GetSelectEffectObject();
	if(!pParticleObject)
		return;
	Ogre::ParticleSystem* pParticleSystem = dynamic_cast<Ogre::ParticleSystem*>(pParticleObject);
	Ogre::ParticleEmitter* pEmitter = dynamic_cast<Ogre::ParticleEmitter*>(pParticleObject);
	Ogre::ParticleAffector* pAffector = dynamic_cast<Ogre::ParticleAffector*>(pParticleObject);
	if(pParticleSystem)
	{
		Fairy::EffectManager::getSingleton().removeParticleTemplateScriptFileMap(pParticleSystem->getName());
		for(int i = 0;i<mEffect->getNumElements();i++)
		{
			Fairy::ParticleElement* pParticleElement = dynamic_cast<Fairy::ParticleElement*>(mEffect->getElement(i));
			if(pParticleElement && pParticleElement->getParticleSystemName() == pParticleSystem->getName())
				pParticleElement->setParticleSystemName("");
					
		}
		InitEffectEditor(mEffect,mEffectTemplateName);
		return;
	}
	Ogre::String fileName = Fairy::EffectManager::getSingleton().getFileNameByTemplateName(mEffectTemplateName);
	Ogre::StringVector particleNames;
	Fairy::EffectManager::getSingleton().getParticleTemplatesFromScriptFile(fileName,particleNames);
	Ogre::StringVector::iterator it= particleNames.begin();
	while(it != particleNames.end())
	{
		Ogre::String templateName = *it;
		Ogre::ParticleSystem* pParticlSystem  = Ogre::ParticleSystemManager::getSingleton().getTemplate(templateName);
		if(pParticlSystem)
		{
			if(pEmitter)
			{
				int iEmitterCount = pParticlSystem->getNumEmitters();
				for(int i=0;i<iEmitterCount;i++)
				{
					if(pParticlSystem->getEmitter(i) == pEmitter)
					{
						DeleteParticleObjectDlg* dlg = new DeleteParticleObjectDlg(this,wxID_ANY);
						dlg->SetObjectType(wxString("粒子发射器"));
						dlg->SetObjectName(wxString(pEmitter->getType().c_str()));
						bool ok = (dlg->ShowModal() == wxID_OK);
						if(ok)
						{	
							pParticlSystem->removeEmitter(i);
							InitEffectEditor(mEffect,mEffectTemplateName);
							dlg->Destroy();
							return;
						}
						dlg->Destroy();
					}
				}
				
			}
			else if(pAffector)
			{
				
				int iAffectorCount = pParticlSystem->getNumAffectors();
				for(int i=0;i<iAffectorCount;i++)
				{
					if(pParticlSystem->getAffector(i) == pAffector)
					{
						DeleteParticleObjectDlg* dlg = new DeleteParticleObjectDlg(this,wxID_ANY);
						dlg->SetObjectType(wxString("粒子影响器"));
						dlg->SetObjectName(wxString(pAffector->getType().c_str()));
						bool ok = (dlg->ShowModal() == wxID_OK);
						if(ok)
						{
							pParticlSystem->removeAffector(i);
							InitEffectEditor(mEffect,mEffectTemplateName);
							dlg->Destroy();
							return;
						}
						dlg->Destroy();
					}
				}
					
			}
			
		}
		it ++;
	}

}
//添加粒子元素
void  EffectObjectEditor::OnNewParticle(wxCommandEvent &e)
{
	if(mEffect)
	{
		NewParticleElementDialog *dlg = new NewParticleElementDialog(this, wxID_ANY,
			_("创建粒子元素"), mEffect);

		bool ok = (dlg->ShowModal() == wxID_OK);
		if(ok)
		{
			const Ogre::String particleName = AS_STRING(dlg->mComboBox->GetValue());
			mEffect->addElement("Particle");
			Fairy::EffectElement *pElement = mEffect->getElement(mEffect->getNumElements()-1);
			pElement->setParameter("ParticleSystem",particleName);

			InitEffectEditor(mEffect,mEffectTemplateName);
			m_Frame->GetGraphDialog()->InitEffectEditor(mEffect,mEffectTemplateName);
		}
	}
}
//添加模型元素
void  EffectObjectEditor::OnNewMesh(wxCommandEvent &e)
{
	if(mEffect)
	{
		mEffect->addElement("Mesh");
		InitEffectEditor(mEffect,mEffectTemplateName);
	}
		
}
//添加广告板元素
void  EffectObjectEditor::OnNewBillboardSet(wxCommandEvent &e)
{
	if(mEffect)
	{
		mEffect->addElement("BillboardSet");
		InitEffectEditor(mEffect,mEffectTemplateName);
	}
}
//添加地面投影元素
void  EffectObjectEditor::OnNewPorjector(wxCommandEvent &e)
{
	if(mEffect)
	{
		mEffect->addElement("Projector");
		InitEffectEditor(mEffect,mEffectTemplateName);
	}

}
//添加光柱元素
void  EffectObjectEditor::OnNewBeam(wxCommandEvent &e)
{
	if(mEffect)
	{
		mEffect->addElement("Beam");
		InitEffectEditor(mEffect,mEffectTemplateName);
	}
}
//添加点光照元素
void  EffectObjectEditor::OnNewPointLight(wxCommandEvent &e)
{
	if(mEffect)
	{
		mEffect->addElement("PointLight");
		InitEffectEditor(mEffect,mEffectTemplateName);
	}
	
}
void EffectObjectEditor::OnNewCylinder(wxCommandEvent &e)
{
	if(mEffect)
	{
		mEffect->addElement("Cylinder");
		InitEffectEditor(mEffect,mEffectTemplateName);
	}
}
void EffectObjectEditor::OnShow(wxActivateEvent& e)
{
	OutputDebugStr("show \n");
}
//新建粒子数据
void EffectObjectEditor::OnNewParticleSystem(wxCommandEvent &e)
{
	if(!mEffect)
		return;

	NewParticleDialog *dlg = new NewParticleDialog(this, wxID_ANY,
		wxT("新建粒子系统"));

	bool ok = (dlg->ShowModal() == wxID_OK);
	if(ok)
	{	
		Ogre::String templateName = dlg->mTextCtrl->GetValue().c_str();

		if(templateName == "")
		{
			dlg->Destroy();
			wxMessageBox(wxT("模板名称不能为空!"),wxT("模板名称错误"));
			return;
		}

		Ogre::String fileName = Fairy::EffectManager::getSingleton().getFileNameByTemplateName(mEffectTemplateName);
		if(Ogre::ParticleSystemManager::getSingleton().getTemplate(templateName))
		{
			dlg->Destroy();
			wxMessageBox(wxT("模板名称名称已经存在!"),wxT("模板名称错误"));
			return;
		}
		Ogre::ParticleSystem *system = Ogre::ParticleSystemManager::getSingleton().createTemplate(templateName,Ogre::String("General"));
		Fairy::EffectManager::getSingleton().addToParticleTemplateScriptFileMap(templateName,fileName);
		InitEffectEditor(mEffect,mEffectTemplateName);
		SelectParticleSystemItem(system,true);
	}
	dlg->Destroy();
}

//添加粒子影响器
void EffectObjectEditor::OnAddParitcleAffector(wxCommandEvent &e)
{
	Ogre::ParticleSystem* pParticleSystem = dynamic_cast<Ogre::ParticleSystem*>(m_EffectObjectTree->GetSelectEffectObject());
	if(!pParticleSystem)
		return;
	NewParticleAffectorDlg *dlg = new NewParticleAffectorDlg(this, wxID_ANY);

	bool ok = (dlg->ShowModal() == wxID_OK);
	if(ok)
	{		
		const Ogre::String affectorTypeName = AS_STRING(dlg->GetAffectorTypeName());
		OutputDebugStr(AS_STRING(affectorTypeName));
		Ogre::ParticleAffector* pAffector = pParticleSystem->addAffector(affectorTypeName);
		InitEffectEditor(mEffect,mEffectTemplateName);

		if (pAffector)
		{
			SelectAffectorItem(pAffector,true);
		}

	}
	dlg->Destroy();

}
//添加粒子发射器
void EffectObjectEditor::OnAddParitcleEmitter(wxCommandEvent &e)
{
	Ogre::ParticleSystem* pParticleSystem = dynamic_cast<Ogre::ParticleSystem*>(m_EffectObjectTree->GetSelectEffectObject());
	if(!pParticleSystem)
		return;
	NewParticleEmitterDlg *dlg = new NewParticleEmitterDlg(this, wxID_ANY);

	bool ok = (dlg->ShowModal() == wxID_OK);
	if(ok)
	{		
		const Ogre::String emtterTypeName = AS_STRING(dlg->GetEmitterTypeName());
		OutputDebugStr(AS_STRING(emtterTypeName));
		Ogre::ParticleEmitter* pEmitter = pParticleSystem->addEmitter(emtterTypeName);
		InitEffectEditor(mEffect,mEffectTemplateName);

		if (pEmitter)
		{
			SelectEmitterItem(pEmitter,true);
		}
	}
	dlg->Destroy();

}
void EffectObjectEditor::SelectParticleSystemItem(Ogre::ParticleSystem* pParticleSystem,bool select)
{
	m_EffectObjectTree->SelectParticleSystem(pParticleSystem,select);
}
void EffectObjectEditor::SelectBasicSettingItem(Ogre::ParticleSystem* pParticleSystem,bool select)
{
	m_EffectObjectTree->SelectParticleSetting(pParticleSystem,select);
}
void EffectObjectEditor::SelectRendererItem(Ogre::ParticleSystem* pParticleSystem,bool select)
{
	m_EffectObjectTree->SelectParticleRenderer(pParticleSystem,select);
}
void EffectObjectEditor::SelectEmitterItem(Ogre::ParticleEmitter* pEmitter,bool select)
{
	m_EffectObjectTree->SelectParticleEmitter(pEmitter,select);
}
void EffectObjectEditor::SelectAffectorItem(Ogre::ParticleAffector* pAffector,bool select)
{
	m_EffectObjectTree->SelectParticleAffector(pAffector,select);
}

void EffectObjectEditor::EditMatrial()
{
	Ogre::ParticleSystem* pParticleSystem = 
		dynamic_cast<Ogre::ParticleSystem*>(m_EffectObjectTree->GetSelectEffectObject());
	

	if(!pParticleSystem)
		return;
	EditMaterialDialog dlg(this,wxID_ANY,"编辑材质",pParticleSystem->getMaterialName().c_str());
	if(dlg.ShowModal() == wxID_OK)
	{
		const Ogre::String newMatrialName = dlg.mTextCtrl->GetValue().c_str(); 
		Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(newMatrialName);
		if(material.isNull())
		{
			material = Ogre::MaterialManager::getSingleton().create(newMatrialName,"General");
		}
		pParticleSystem->setMaterialName(newMatrialName);

		//m_ParticleObjectTree->SelectParticleSetting();
		//GetParentFrame()->GetEffectObjectProperty()->InitParticleSettingProperty();
		m_Frame->GetMaterialEditor()->InitMaterialEditor(material,newMatrialName);
		m_Frame->GetEditorNotebook()->SetSelection(2);
	}

}

void EffectObjectEditor::saveEffect(Fairy::Effect *effect, const Ogre::String &templateName, std::ofstream &of, Ogre::String filename, bool saveParticle)
{
	assert (effect);

	of << "\n";
	of << "effect " << templateName << "\n";
	of << "{";


	for ( size_t i=0; i < effect->getNumElements(); ++i )
	{
		Fairy::EffectElement *element = effect->getElement(i);
		assert (element);

		of << "\n";
		of << "\telement " << element->getType() << "\n";
		of << "\t{\n";

		const Ogre::ParameterList &paraList = element->getParameters();

		for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
		{
			of << "\t\t" << paraList[paraCount].name << "\t" << element->getParameter( paraList[paraCount].name ) << "\n";
		}		

		of << "\t}\n";
	}

	const Ogre::ParameterList &paraList = effect->getParameters();

	for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
	{
		of << "\n\t" << paraList[paraCount].name << "\t" << effect->getParameter( paraList[paraCount].name ) << "\n";
	}	

	of << "}\n";

	// 记录要保存的Particle模板
	/*if(saveParticle)
	{
		for(size_t i= 0;i<effect->getNumElements();i++)
		{
			Fairy::ParticleElement* pParticleElement = dynamic_cast<Fairy::ParticleElement*>(effect->getElement(i));
			if(NULL != pParticleElement)
			{
				Ogre::String ptParticleSystemName = pParticleElement->getParticleSystemName();
				Ogre::ParticleSystem* pSystem  = Ogre::ParticleSystemManager::getSingleton().getTemplate(ptParticleSystemName);
				if((NULL != pSystem) && Fairy::EffectManager::getSingleton().getFileNameByParticleTemplateName(ptParticleSystemName).empty())
				{
					Fairy::EffectManager::getSingleton().addToParticleTemplateScriptFileMap(ptParticleSystemName, filename);
				}
			}
		}
	}*/
}
void EffectObjectEditor::saveParticleSystem(Ogre::ParticleSystem *particleSystem, const Ogre::String &templateName, std::ofstream &of)
{
	// 先输出粒子系统的名称，并换行，输出{
	of << "\n";
	of << "particle ";
	of << templateName.c_str();
	of << "\n{\n";

	// 输出基本属性
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

	// 输出renderer属性
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

	// 输出emitter
	unsigned short numOfEmitter = particleSystem->getNumEmitters();

	Ogre::ParticleEmitter *pEmitter = NULL;

	// 遍历所有的emitter
	for ( unsigned short j=0; j<numOfEmitter; ++j )
	{
		pEmitter = particleSystem->getEmitter(j);

		const Ogre::ParameterList& list = pEmitter->getParameters();

		of << "\temitter " << pEmitter->getType().c_str() << "\n\t{\n\t";

		// 输出各个emitter的参数
		for ( size_t z=0; z<list.size(); ++z )
		{
			of << '\t' << list[z].name.c_str() << ' ' << pEmitter->getParameter( list[z].name ).c_str() << "\n\t";
		}

		// 输出这个emitter的}
		of << "}\n\n";
	}

	// 输出affector
	unsigned short numOfAffector = particleSystem->getNumAffectors();

	Ogre::ParticleAffector *pAffector = NULL;

	// 遍历所有的affector
	for ( unsigned short ii=0; ii<numOfAffector; ++ii )
	{
		pAffector = particleSystem->getAffector(ii);

		const Ogre::ParameterList& list = pAffector->getParameters();

		of << "\taffector " << pAffector->getType().c_str() << "\n\t{\n\t";

		// 输出这个affector的参数
		for ( size_t jj=0; jj<list.size(); ++jj )
		{
			of << '\t' << list[jj].name.c_str() << ' ' << pAffector->getParameter( list[jj].name ).c_str() << "\n\t";
		}

		// 输出这个affector的}
		of << "}\n\n";
	}

	// 输出这个粒子系统的}
	of << "}\n\n";
}