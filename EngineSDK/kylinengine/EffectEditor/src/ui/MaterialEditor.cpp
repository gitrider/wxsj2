
#include "MaterialEditor.h"
#include "MaterialTree.h"
#include "EffectObjectPropertyEditor.h"
#include "OgreMaterialSerializer.h"
#include "OgreMaterialManager.h"
#include <wx/filedlg.h>
#include <wx/image.h>
#include <wx/stattext.h>



IMPLEMENT_DYNAMIC_CLASS(NewMaterialDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(NewMaterialDialog, wxDialog)
EVT_INIT_DIALOG(NewMaterialDialog::OnInitDialog)
END_EVENT_TABLE()

void
NewMaterialDialog::OnInitDialog(wxInitDialogEvent &e)
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer4->SetFlexibleDirection( wxBOTH );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );


	wxStaticText* staticText2 = new wxStaticText( this, wxID_ANY, wxT("材质名称:"), wxDefaultPosition, wxDefaultSize, 0 );

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

///////////////////////////////////////////////////////////////////////////
static const int feID_NEW_MATERIAL =wxNewId();
static const int feID_SAVE_MATERIAL =wxNewId();
static const int feID_SAVEAS_MATERIAL =wxNewId();
static const int feID_DELETE_MATERIAL = wxNewId();
static const int feID_NEW_TECHNIQUE = wxNewId();
static const int feID_NEW_PASS = wxNewId();
static const int feID_NEW_TEXTUREUNIT = wxNewId();
//IMPLEMENT_DYNAMIC_CLASS(MaterialEditor, wxPanel)

BEGIN_EVENT_TABLE(MaterialEditor, wxPanel)
EVT_MENU(feID_NEW_MATERIAL,MaterialEditor::OnNewMaterial)
EVT_MENU(feID_DELETE_MATERIAL,MaterialEditor::OnDeleteMaterialObject)
EVT_MENU(feID_SAVE_MATERIAL,MaterialEditor::OnSaveMaterial)
EVT_MENU(feID_SAVEAS_MATERIAL,MaterialEditor::OnSaveAsMaterial)
EVT_MENU(feID_NEW_TECHNIQUE,MaterialEditor::OnNewTechnique)
EVT_MENU(feID_NEW_PASS,MaterialEditor::OnNewPass)
EVT_MENU(feID_NEW_TEXTUREUNIT,MaterialEditor::OnNewTextureUnitState)
END_EVENT_TABLE()


MaterialEditor::MaterialEditor( wxWindow* parent,
				   wxWindowID id,
				   const wxPoint& pos,
				   const wxSize& size,
				   long style) : mMaterialName(""), mConfig(NULL),
				   EFFECT_PATH("../resource/media/特效/材质/"),
				   mSaveFileName(""),mbRecoverSave(false)
				   

{
	Create(parent, id, pos, size, style);

}

bool
MaterialEditor::Create(wxWindow *parent, wxWindowID winid,
							   const wxPoint& pos, const wxSize& size,
							   long style)
{
	if (!wxPanel::Create(parent, winid, pos, size, style))
		return false;

	wxFlexGridSizer* sizer = new wxFlexGridSizer(1);
	sizer->AddGrowableCol(0);
	sizer->AddGrowableRow(1);


	m_toolBar = new wxToolBar( this, wxID_ANY, wxDefaultPosition, wxSize(-1,28)); 
	m_toolBar->AddTool(feID_NEW_MATERIAL,
		wxBITMAP(NEW),wxBITMAP(NEW),false,
		0,wxDefaultCoord,NULL, wxT("创建材质"),wxT("创建材质"));

	m_toolBar->AddTool(feID_SAVEAS_MATERIAL,
		wxBITMAP(SAVEAS),wxBITMAP(SAVEAS),false,
		1,wxDefaultCoord,NULL, wxT("另存材质"),wxT("另存材质"));

	m_toolBar->AddTool(feID_SAVE_MATERIAL,
		wxBITMAP(SAVE),wxBITMAP(SAVE),false,
		2,wxDefaultCoord,NULL, wxT("保存材质"),wxT("保存材质"));

	m_toolBar->AddTool(feID_DELETE_MATERIAL,
		wxBITMAP(DELETE),wxBITMAP(DELETE),false,
		3,wxDefaultCoord,NULL, wxT("删除材质元素"),wxT("删除材质元素"));

	m_toolBar->AddSeparator();

	m_toolBar->AddTool(feID_NEW_TECHNIQUE,
		wxBITMAP(NEWTECHNIQUE),wxBITMAP(NEWTECHNIQUE),false,
		4,wxDefaultCoord,NULL, wxT("添加Technique"),wxT("添加Technique"));

	m_toolBar->AddTool(feID_NEW_PASS,
		wxBITMAP(NEWPASS),wxBITMAP(NEWPASS),false,
		5,wxDefaultCoord,NULL, wxT("添加Pass"),wxT("添加Pass"));

	m_toolBar->AddTool(feID_NEW_TEXTUREUNIT,
		wxBITMAP(NEWTEXTURE),wxBITMAP(NEWTEXTURE),false,
		6,wxDefaultCoord,NULL, wxT("添加贴图单元"),wxT("添加贴图单元"));

	m_toolBar->Realize();
	
	
	sizer->Add( m_toolBar, 0, wxEXPAND, 0 );

	m_MaterialTree = new MaterialTree( this,ID_MATERIAL_OBJECT_TREE, wxDefaultPosition, wxDefaultSize);
	sizer->Add( m_MaterialTree, 0, wxGROW|wxALL, 0 );

	this->SetSizer( sizer );
	sizer->SetSizeHints(this);

	return true;
}

MaterialEditor::~MaterialEditor()
{
	
}
//选择当前编辑页
void MaterialEditor::OnPageSelect()
{
	if(mMaterial.isNull())
		return;
	m_Frame->GetEffectObjectProperty()->InitMaterialEditor(mMaterial,mMaterialName);
	m_MaterialTree->refrashSelected();
}
void MaterialEditor::RefreshMaterialTreeItem(const wxString& Desc,const wxString& propertyName,
											 const wxString& propertyValue,void* data)
{
	if(mMaterial.isNull())
		return;
	m_MaterialTree->refreshItem(Desc,propertyName,propertyValue,data);
}
void MaterialEditor::InitMaterialEditor(Ogre::MaterialPtr& pMaterial,const Ogre::String &templateName)
{
	mMaterial = pMaterial;
	mMaterialName = templateName;
	mSaveFileName = "";
	m_Frame->GetEffectObjectProperty()->InitMaterialEditor(pMaterial,templateName);
	m_MaterialTree->DeleteAllItems();
	m_MaterialTree->AddMaterialToTree(pMaterial,templateName);
}
void
MaterialEditor::SetSceneManipulator(Fairy::SceneManipulator* sceneManipulator)
{
	mSceneManipulator = sceneManipulator;
	mSceneManipulator->addSceneListener(this);
}

void MaterialEditor::OnNotifyToolBarCtrl(ToolbarCtrl index)
{
	wxCommandEvent e;
	if (index == TC_NEW_MATERIAL)
	{
		OnNewMaterial(e);
	}
	else if (index == TC_SAVEAS_MATERIAL)
	{
		OnSaveAsMaterial(e);
	}
	else if (index == TC_SAVE_MATERIAL)
	{
		OnSaveMaterial(e);
	}
	else if (index == TC_DELETE_ElEMENT)
	{
		OnDeleteMaterialObject(e);
	}
	else if (index == TC_NEW_TECHNIQUE)
	{
		OnNewTechnique(e);
	}
	else if (index == TC_NEW_PASS)
	{
		OnNewPass(e);
	}
	else if (index == TC_NEW_TEXTUREUNIT)
	{
		OnNewTextureUnitState(e);
	}
}
void MaterialEditor::OnNewMaterial(wxCommandEvent &e)
{
	NewMaterialDialog *dlg = new NewMaterialDialog(this, wxID_ANY,
		wxT("新建材质"));

	bool ok = (dlg->ShowModal() == wxID_OK);
	if(ok)
	{
		const Ogre::String templateName = dlg->mTextCtrl->GetValue().c_str();

		if(templateName.length() == 0)
		{
			dlg->Destroy();
			return;
		}
		Ogre::MaterialPtr pMaterial= Ogre::MaterialManager::getSingleton().getByName(templateName);
		//Ogre::MaterialPtr *pMaterial;
		if(pMaterial.isNull())
			pMaterial = Ogre::MaterialManager::getSingleton().create(templateName,"General");
		

		InitMaterialEditor(pMaterial,templateName);
		m_Frame->GetEffectObjectProperty()->InitMaterialEditor(pMaterial,templateName);

	}
	dlg->Destroy();

}
void MaterialEditor::OnSaveMaterial(wxCommandEvent &e)
{
	if(mMaterial.isNull())
		return;
	if(mSaveFileName == "")
	{
		wxFileDialog dialog(this, wxT("保存材质文件"), EFFECT_PATH, wxT(""),
			"All texture files (*.material)|*.material", wxSAVE); 

		if (dialog.ShowModal() == wxID_OK)
		{		
			wxArrayString SavePaths;
			dialog.GetPaths(SavePaths);

			mSaveFileName = SavePaths[0].c_str();
		}
		else
		{
			return;
		}

	}

	Ogre::MaterialSerializer ser;
	ser.queueForExport(mMaterial);
	ser.exportQueued(mSaveFileName);

}
void MaterialEditor::OnSaveAsMaterial(wxCommandEvent &e)
{
	if(mMaterial.isNull())
		return;
	wxFileDialog dialog(this, wxT("保存材质文件"), EFFECT_PATH, wxT(""),
		"All texture files (*.material)|*.material", wxSAVE); 

	if (dialog.ShowModal() == wxID_OK)
	{		
		wxArrayString SavePaths;
		dialog.GetPaths(SavePaths);

		mSaveFileName = SavePaths[0].c_str();
	}
	else
	{
		return;
	}

	Ogre::MaterialSerializer ser;
	ser.queueForExport(mMaterial);
	ser.exportQueued(mSaveFileName);

}
void  MaterialEditor::OnDeleteMaterialObject(wxCommandEvent &e)
{
	if(mMaterial.isNull())
		return;
	if(m_MaterialTree->getSelecteTextureUnit())
	{
		Ogre::TextureUnitState* pTexture = m_MaterialTree->getSelecteTextureUnit();
		Ogre::Pass* pPass = pTexture->getParent();
		if(pPass)
		{
			pPass->removeTextureUnitState(pPass->getTextureUnitStateIndex(pTexture));
		}
	}
	else if(m_MaterialTree->getSelectePass())
	{
		Ogre::Pass* pPass = m_MaterialTree->getSelectePass();
		Ogre::Technique* pTechnique = pPass->getParent();
		if(pTechnique)
		{
			pTechnique->removePass(pPass->getIndex());
		}
	}
	else if(m_MaterialTree->getSelecteTechnique())
	{
		Ogre::Technique* pTechnique = m_MaterialTree->getSelecteTechnique();
		for(int i= 0;i<mMaterial->getNumTechniques();i++)
		{
			if(pTechnique == mMaterial->getTechnique(i))
			{
				mMaterial->removeTechnique(i);
				break;
			}
		}
	}
	else
		return;
	m_Frame->GetEffectObjectProperty()->InitMaterialEditor(mMaterial,mMaterialName);
	m_MaterialTree->DeleteAllItems();
	m_MaterialTree->AddMaterialToTree(mMaterial,mMaterialName);

}
void MaterialEditor::OnNewTechnique(wxCommandEvent &e)
{
	if(mMaterial.isNull())
		return;
	Ogre::Technique* pTechinque = mMaterial->createTechnique();
	m_Frame->GetEffectObjectProperty()->InitMaterialEditor(mMaterial,mMaterialName);
	m_MaterialTree->DeleteAllItems();
	m_MaterialTree->AddMaterialToTree(mMaterial,mMaterialName);
	m_MaterialTree->SelecteTechniqueItem(pTechinque);

}
void MaterialEditor::OnNewPass(wxCommandEvent &e)
{
	if(mMaterial.isNull())
		return;
	Ogre::Technique* pTechinque = m_MaterialTree->getSelecteTechnique();
	if(pTechinque)
	{
		Ogre::Pass* pPass = pTechinque->createPass();
		m_Frame->GetEffectObjectProperty()->InitMaterialEditor(mMaterial,mMaterialName);
		m_MaterialTree->DeleteAllItems();
		m_MaterialTree->AddMaterialToTree(mMaterial,mMaterialName);
		m_MaterialTree->SelectePassItem(pPass);
	}
}
void MaterialEditor::OnNewTextureUnitState(wxCommandEvent &e)
{
	if(mMaterial.isNull())
		return;
	Ogre::Pass* pass = m_MaterialTree->getSelectePass();
	if(pass)
	{
		Ogre::TextureUnitState* pTexture = pass->createTextureUnitState();
		m_Frame->GetEffectObjectProperty()->InitMaterialEditor(mMaterial,mMaterialName);
		m_MaterialTree->DeleteAllItems();
		m_MaterialTree->AddMaterialToTree(mMaterial,mMaterialName);
		m_MaterialTree->SelecteTextureUnitItem(pTexture);
	}

}

