#include <wx/wxprec.h>
#include "ObjFileEditDlg.h"
#include "res/ObjFileEditDlg_wdr.h"
#include "DataManipulator.h"

#include <OgreStringConverter.h>
BEGIN_EVENT_TABLE(ObjFileEditDialog, wxPanel)

EVT_BUTTON(ID_BUTTON_OBJECT,OnAddObjectName)
EVT_BUTTON(ID_BUTTON_ATTRIBUTE,OnAddAttribute)
EVT_BUTTON(ID_BUTTON_ENTITYLIST,OnAddEntitylist)
EVT_BUTTON(ID_BUTTON_MATERIALLIST,OnAddMateriallist)
EVT_BUTTON(ID_BUTTON_LOCATOR_ADD,OnAddLocator)
EVT_BUTTON(ID_BUTTON_LOCATOR_DEL,OnDelLocator)
EVT_BUTTON(ID_BUTTON_SKELETON_ADD,OnAddSkeleton)
EVT_BUTTON(ID_BUTTON_SKELETON_DEL,OnDelSkeleton)

EVT_COMBOBOX(ID_COMBO_ATTACTHMODEL,OnChangeSlot)
EVT_COMBOBOX(ID_COMBO_EFFECT,OnChangeEffect)
EVT_COMBOBOX(ID_COMBO_NEWFILE,OnSelObjTemplate)
EVT_COMBOBOX(ID_COMBO_LOCATOR_NAME,OnChangeLocator)
EVT_COMBOBOX(ID_COMBO_ENTITYMATERIAL,OnChangeMaterial)
EVT_COMBOBOX(ID_COMBO_MATERIALMATERIAL,OnChangeMaterial)
EVT_COMBOBOX(ID_COMBO_PROPERHINTS,OnChangeAttributeHints)
END_EVENT_TABLE()

ObjFileEditDialog::~ObjFileEditDialog(void)
{

}

bool ObjFileEditDialog::Create(wxWindow *parent, wxWindowID winid,
							   const wxPoint& pos, const wxSize& size,
							   long style, const wxString& name)
{
	if (!wxPanel::Create(parent, winid, pos, size, style, name))
		return false;

	ObjFileEditDlg(this, true, true);
	//object
	m_pObjectNameTextCtrl	  		= wxDynamicCast(FindWindow(ID_TEXTCTRL_OBJECTNAME), wxTextCtrl);
	//attribute
	m_pAttributeNameComboBox  		= wxDynamicCast(FindWindow(ID_COMBO_PROPERTYNAME), wxComboBox);
	m_pAttributeValueTextCtrl 		= wxDynamicCast(FindWindow(ID_TEXTCTRL_PROPERVALUE), wxTextCtrl);
	m_pAttributeHintsComboBox 		= wxDynamicCast(FindWindow(ID_COMBO_PROPERHINTS), wxComboBox);
	//entitylist
	m_pEntityListNameComboBox 		= wxDynamicCast(FindWindow(ID_COMBO_ENTITYNAME), wxComboBox);
	m_pEntityListFileTextCtrl 		= wxDynamicCast(FindWindow(ID_TEXTCTRL_ENTITYFILE), wxTextCtrl);
	m_pEntityListMaterialComboBox   = wxDynamicCast(FindWindow(ID_COMBO_ENTITYMATERIAL), wxComboBox);
	//materiallist
	m_pMaterialListNameComboBox     = wxDynamicCast(FindWindow(ID_COMBO_MATERIALNAME), wxComboBox);
	m_pMaterialListEntityTextCtrl   = wxDynamicCast(FindWindow(ID_TEXTCTRL_MATERIALENTITY), wxTextCtrl);
	m_pMaterialListMaterialComboBox = wxDynamicCast(FindWindow(ID_COMBO_MATERIALMATERIAL), wxComboBox);
	//locatro
	m_pLocatorNameComboBox			= wxDynamicCast(FindWindow(ID_COMBO_LOCATOR_NAME), wxComboBox);
	m_pAttachObjNameComboBox		= wxDynamicCast(FindWindow(ID_COMBO_ATTACTHMODEL), wxComboBox);
	m_pSlotNameComboBox				= wxDynamicCast(FindWindow(ID_COMBO_SLOTNAME), wxComboBox);
	m_pBoneComboBox					= wxDynamicCast(FindWindow(ID_COMOB_BONE), wxComboBox);
	m_pLocatorPosTextCtrl		    = wxDynamicCast(FindWindow(ID_TEXTCTRL_LOCATOR_POSTION), wxTextCtrl);
	m_pLocatorOriTextCtrl			= wxDynamicCast(FindWindow(ID_TEXTCTRL_LOCATOR_ORIENTATION), wxTextCtrl);
	//effect
	m_pEffectNameComboBox			= wxDynamicCast(FindWindow(ID_COMBO_EFFECT), wxComboBox);
	//skeleton
	m_pSkeletonName					= wxDynamicCast(FindWindow(ID_COMBO_SKELETON_NAME), wxComboBox);
	m_pSkeletonTable				= wxDynamicCast(FindWindow(ID_COMBO_SKELETON_TABLE), wxComboBox);
	m_pSkeletonAdd					= wxDynamicCast(FindWindow(ID_BUTTON_SKELETON_ADD), wxButton);
	m_pSkeletonDel					= wxDynamicCast(FindWindow(ID_BUTTON_SKELETON_DEL), wxButton);
	//static
	AttrituteStaticBox				= wxDynamicCast(FindWindow(ID_STATIC_ATTRIBUTE), wxStaticBox);
	AttachModelStaticBox			= wxDynamicCast(FindWindow(ID_STATIC_ATTACHMODEL), wxStaticBox);
	AttributeNameStaticText			= wxDynamicCast(FindWindow(ID_TEXT_ATTRIBUTE_NAME), wxStaticText);
	AttributeHintsStaticText		= wxDynamicCast(FindWindow(ID_TEXT_ATTRIBUTE_HINTS), wxStaticText);
	AttributeSlotStaticText			= wxDynamicCast(FindWindow(ID_TEXT_ATTRIBUTE_SLOT), wxStaticText);
	AttributeAttachStaticText		= wxDynamicCast(FindWindow(ID_TEXT_ATTRIBUTE_ATTACH), wxStaticText);

	EntityStaticBox					= wxDynamicCast(FindWindow(ID_STATIC_ENTITY), wxStaticBox);
	EntityNameStaticText			= wxDynamicCast(FindWindow(ID_TEXT_ENTITY_NAME), wxStaticText);
	EntityFileStaticText			= wxDynamicCast(FindWindow(ID_TEXT_ENTITY_FILE), wxStaticText);
	EntityMaterialStaticText		= wxDynamicCast(FindWindow(ID_TEXT_ENTITY_MATERIAL), wxStaticText);

	MatStaticBox					= wxDynamicCast(FindWindow(ID_STATIC_MATERIAL), wxStaticBox);
	MatNameStaticText				= wxDynamicCast(FindWindow(ID_TEXT_MATERIAL_NAME), wxStaticText);
	MatEntityStaticText				= wxDynamicCast(FindWindow(ID_TEXT_MATERIAL_ENTITY), wxStaticText);
	MatMaterialStaticText			= wxDynamicCast(FindWindow(ID_TEXT_MATERIAL_MATERIAL), wxStaticText);

	LocatorStaticBox				= wxDynamicCast(FindWindow(ID_STATIC_LOCATOR), wxStaticBox);
	PosAndOriStaticBox				= wxDynamicCast(FindWindow(ID_STATIC_POSANDORI), wxStaticBox);
	LocatorNameStaticText			= wxDynamicCast(FindWindow(ID_TEXT_LOCATOR_NAME), wxStaticText);
	LocatorBoneStaticText			= wxDynamicCast(FindWindow(ID_TEXT_LOCATOR_BONE), wxStaticText);
	LocatorEffectStaticText			= wxDynamicCast(FindWindow(ID_TEXT_LOCATOR_EFFECT), wxStaticText);
	LocatorPosStaticText			= wxDynamicCast(FindWindow(ID_TEXT_LOCATOR_POS), wxStaticText);
	LocatorOriStaticText			= wxDynamicCast(FindWindow(ID_TEXT_LOCATOR_ORI), wxStaticText);

	SkeStaticBox					= wxDynamicCast(FindWindow(ID_STATIC_SKELETON), wxStaticBox);
	SkeNameStaticText				= wxDynamicCast(FindWindow(ID_TEXT_SKELETON_NAME), wxStaticText);
	SkeTableStaticText				= wxDynamicCast(FindWindow(ID_TEXT_SKELETON_TABLE), wxStaticText);

	m_pObjFileTable					= wxDynamicCast(FindWindow(ID_COMBO_NEWFILE), wxComboBox);

	return true;
}

bool ObjFileEditDialog::frameStarted(const Ogre::FrameEvent &evt)
{
	return true;
}

void ObjFileEditDialog::OnAddObjectName(wxCommandEvent &e)
{
	if(NULL == m_pObjectNameTextCtrl)
		return;

	Fairy::CGod* pModel = GetDataManipulatr()->FindModel(GetDataManipulatr()->m_CurModelName);

	if(NULL == pModel)
		return;

	pModel->m_Objectname = m_pObjectNameTextCtrl->GetValue().c_str();

	wxMessageBox(wxString::Format("添加ojectname:<%s>成功!",pModel->m_Objectname.c_str()));
}

void ObjFileEditDialog::OnAddAttribute(wxCommandEvent &e)
{
	//if(NULL == m_pAttributeNameComboBox
	//	&&m_pAttributeValueTextCtrl
	//	&&m_pAttributeHintsComboBox)
	//	return;

	//Fairy::CGod* pModel = GetDataManipulatr()->FindModel(GetDataManipulatr()->m_CurModelName);

	//if(NULL == pModel)
	//	return;

	//DString attributename  = m_pAttributeNameComboBox->GetStringSelection().c_str();

	//if(attributename.empty())
	//	attributename	   = m_pAttributeNameComboBox->GetValue().c_str();

	//DString attributevalue = m_pAttributeValueTextCtrl->GetStringSelection().c_str();

	//DString attributehints = m_pAttributeHintsComboBox->GetStringSelection().c_str();

	//if(attributehints.empty())
	//	attributehints	   = m_pAttributeHintsComboBox->GetValue().c_str();

	//if(!attributename.empty())
	//{
	//	Fairy::LogicModel::AttribMap::iterator it = pModel->m_objAttribute.find(attributename);

	//	if(it != pModel->m_objAttribute.end())
	//	{
	//		wxMessageBox(wxString::Format("已添加属性<%s>!",attributename.c_str()));
	//	}
	//	else
	//	{
	//		pModel->m_objAttribute.insert(Fairy::LogicModel::AttribMap::value_type(attributename,Fairy::LogicModel::AttribValue(attributevalue,attributehints)));

	//		int nIndex = pModel->m_objAttribute.size();

	//		wxMessageBox(wxString::Format("添加第%d个属性<%s>成功!",nIndex,attributename.c_str()));
	//	}
	//}
}

void ObjFileEditDialog::OnAddEntitylist(wxCommandEvent &e)
{
	//if(NULL == m_pAttributeNameComboBox
	//		 &&m_pAttributeValueTextCtrl
	//		 &&m_pAttributeHintsComboBox)
	//	return;

	//Fairy::CGod* pModel = GetDataManipulatr()->FindModel(GetDataManipulatr()->m_CurModelName);

	//if(NULL == pModel)
	//	return;

	//DString entitylistname	   = m_pEntityListNameComboBox->GetStringSelection().c_str();

	//if(entitylistname.empty())
	//	entitylistname		   = m_pEntityListNameComboBox->GetValue().c_str();

	//DString entitylistfile	   = m_pEntityListFileTextCtrl->GetStringSelection().c_str();

	//DString entitylistmaterial = m_pEntityListMaterialTextCtrl->GetStringSelection().c_str();

	//if(!entitylistname.empty())
	//{		
	//	if(pModel->m_objEntitylist.find(entitylistname) != pModel->m_objEntitylist.end())
	//	{
	//		wxMessageBox(wxString::Format("已添加实体<%s>!",entitylistname.c_str()));
	//	}
	//	else
	//	{
	//		pModel->InsertEntity(entitylistname,entitylistfile,entitylistmaterial);

	//		int nIndex = pModel->m_objEntitylist.size();

	//		wxMessageBox(wxString::Format("添加第%d个实体<%s>成功!",nIndex,entitylistname.c_str()));
	//	}
	//}

}

void ObjFileEditDialog::OnAddMateriallist(wxCommandEvent &e)
{
	//if(NULL == m_pMaterialListNameComboBox
	//		 &&m_pMaterialListEntityComboBox
	//		 &&m_pMaterialListMaterialTextCtrl)
	//	return;

	//Fairy::CGod* pModel = GetDataManipulatr()->FindModel(GetDataManipulatr()->m_CurModelName);

	//if(NULL == pModel)
	//	return;

	//DString materiallistname	   = m_pMaterialListNameComboBox->GetStringSelection().c_str();

	//if(materiallistname.empty())
	//	materiallistname		   = m_pMaterialListNameComboBox->GetValue().c_str();

	//DString materiallistentiy	   = m_pMaterialListEntityComboBox->GetStringSelection().c_str();

	//if(materiallistentiy.empty())
	//	materiallistentiy		   = m_pMaterialListEntityComboBox->GetValue().c_str();

	//DString materiallistmaterial   = m_pMaterialListMaterialTextCtrl->GetStringSelection().c_str();

	//if(!materiallistname.empty())
	//{		
	//	if(pModel->m_objMateriallist.find(materiallistname) != pModel->m_objMateriallist.end())
	//	{
	//		wxMessageBox(wxString::Format("已添加材质<%s>!",materiallistname.c_str()));
	//	}
	//	else
	//	{
	//		pModel->InsertMaterial(materiallistname,materiallistentiy,materiallistmaterial);

	//		int nIndex = pModel->m_objMateriallist.size();

	//		wxMessageBox(wxString::Format("添加第%d个材质<%s>成功!",nIndex,materiallistname.c_str()));
	//	}
	//}
}

void ObjFileEditDialog::OnAddLocator(wxCommandEvent &e)
{
	//if(NULL == m_pLocatorNameComboBox
	//	|| NULL == m_pSlotNameComboBox)
	//	return;

	//Fairy::CGod* pModel = GetDataManipulatr()->FindModel(GetDataManipulatr()->m_CurModelName);

	//if(NULL == pModel)
	//	return;

	//DString locatorname			  = m_pLocatorNameComboBox->GetStringSelection().c_str();

	//if(locatorname.empty())
	//	locatorname			      = m_pLocatorNameComboBox->GetValue().c_str();

	//if(locatorname.empty())
	//	return;

	//std::vector<Ogre::String> vec = Ogre::StringUtil::split(locatorname,"\\");

	//const DString bone			  = vec[0];

	//const DString locator		  = vec[1];

	//DString slotname		      = m_pSlotNameComboBox->GetStringSelection().c_str();

	////if(slotname.empty())
	////	slotname			      = m_pSlotNameComboBox->GetValue().c_str();

	//bool bResult = pModel->AddLocator(locator,bone,slotname);

	//if(bResult)
	//{
	//	wxMessageBox(wxString::Format("添加挂点<%s>成功!",locator.c_str()));
	//}
	//else
	//{
	//	wxMessageBox("未选择要添加的挂点或该挂点已存在!");
	//}
}

void ObjFileEditDialog::OnDelLocator(wxCommandEvent &e)
{
	//if(NULL == m_pAttachObjNameComboBox || NULL == m_pSlotNameComboBox)
	//	return;

	//Fairy::CGod* pModel = GetDataManipulatr()->FindModel(GetDataManipulatr()->m_CurModelName);

	//if(NULL == pModel)
	//	return;

	//DString locatorname			  = m_pLocatorNameComboBox->GetStringSelection().c_str();

	////if(locatorname.empty())
	////	locatorname			      = m_pLocatorNameComboBox->GetValue().c_str();

	//if(locatorname.empty())
	//	return;

	//std::vector<Ogre::String> vec = Ogre::StringUtil::split(locatorname,"\\");

	//const DString locator		  = vec[1];

	//bool bResult = pModel->DelLocator(locator);

	//if(bResult)
	//{
	//	wxMessageBox(wxString::Format("删除挂点<%s>成功!",locator.c_str()));
	//}
	//else
	//{
	//	wxMessageBox("请选择要删除挂点!");
	//}
}

void ObjFileEditDialog::OnChangeAttributeHints(wxCommandEvent &e)
{

	if(NULL == m_pAttributeHintsComboBox
	|| NULL == m_pAttributeNameComboBox
	|| NULL == GetDataManipulatr())
		return;

	Fairy::CGod* pModel = GetDataManipulatr()->FindModel(GetDataManipulatr()->m_CurModelName);

	if(NULL == pModel)
		return;
	
	DString Attributename = m_pAttributeNameComboBox->GetStringSelection().c_str();

	if(Attributename.empty())
		Attributename     = m_pAttributeNameComboBox->GetValue().c_str();

	DString AttributeHints = m_pAttributeHintsComboBox->GetStringSelection().c_str();

	bool bResult = pModel->ChangeHints(Attributename,AttributeHints);

	if(bResult)
	{
		wxMessageBox(wxString::Format("成功修改属性<%s>提示为<%s>!",Attributename.c_str(),AttributeHints.c_str()));
	}
	else
	{
		wxMessageBox(wxString::Format("操作失败!没有找到属性<%s>!",Attributename.c_str()));
	}
}

void ObjFileEditDialog::OnChangeSlot(wxCommandEvent &e)
{
	if(NULL == m_pLocatorNameComboBox)
		return;

	Fairy::CGod* pModel = GetDataManipulatr()->FindModel(GetDataManipulatr()->m_CurModelName);

	if(NULL == pModel)
		return;

	DString attachobjname		  = m_pAttachObjNameComboBox->GetStringSelection().c_str();

	DString slotname		      = m_pSlotNameComboBox->GetStringSelection().c_str();

	int nResult = pModel->ChangeSlot(slotname,attachobjname);

	if(0 == nResult)
	{
		wxMessageBox(wxString::Format("不存在slot<%s>对应的骨头，请检查美术资源!",slotname.c_str()));
	}
	else if(2 == nResult)
	{
		wxMessageBox(wxString::Format("属性slot<%s>的提示说明不为挂点!",slotname.c_str()));
	}
	else if(3 == nResult)
	{
		wxMessageBox(wxString::Format("未找到插槽slot<%s>对应的属性,请检查obj模板!",slotname.c_str()));
	}
}

void ObjFileEditDialog::OnChangeEffect(wxCommandEvent &e)
{
	if(NULL == m_pEffectNameComboBox)
		return;

	Fairy::CGod* pModel = GetDataManipulatr()->FindModel(GetDataManipulatr()->m_CurModelName);

	if(NULL == pModel)
		return;

	DString locatorname			  = m_pLocatorNameComboBox->GetStringSelection().c_str();

	if(locatorname.empty())
		locatorname			      = m_pLocatorNameComboBox->GetValue().c_str();

	if(locatorname.empty())
		return;

	std::vector<Ogre::String> locatorvec = Ogre::StringUtil::split(locatorname,"\\");

	if(locatorvec.size() < 2)
		return;

	const DString bone			  = locatorvec[0];

	const DString locator		  = locatorvec[1];

	DString effectname			  = m_pEffectNameComboBox->GetStringSelection().c_str();

	if(effectname.empty())
		return;

	std::vector<Ogre::String> effectvec = Ogre::StringUtil::split(effectname,".");

	const DString effect		  = effectvec[0];

	bool nResult = pModel->ChangeEffect(effect,locator,bone);

	if(!nResult)
	{
		wxMessageBox(wxString::Format("不存在特效对应的挂点<%s>，请检查美术资源!",locator.c_str()));
	}
}

void ObjFileEditDialog::OnChangeMaterial(wxCommandEvent &e)
{
	if(NULL == m_pEntityListMaterialComboBox
	|| NULL == m_pMaterialListMaterialComboBox)
	return;

	DString entmatname			  = m_pEntityListMaterialComboBox->GetStringSelection().c_str();

	DString matmatname			  = m_pMaterialListMaterialComboBox->GetStringSelection().c_str();

	if(entmatname.empty() && matmatname.empty())
		return;

	DString matname;
	if(entmatname.empty())
	{
		matname = matmatname;
	}
	else
	{
		matname = entmatname;
	}

	Fairy::CGod* pModel = GetDataManipulatr()->FindModel(GetDataManipulatr()->m_CurModelName);

	if(NULL == pModel)
		return;

	std::vector<Ogre::String> matvec = Ogre::StringUtil::split(matname,".");


	pModel->ChangeMat(matvec[0],matvec[0]);
}

void ObjFileEditDialog::DynamicShowUI(bool attributeshow,bool entityshow,bool matshow,
									  bool locatorshow,bool skeletonshow)
{
	if(NULL == m_pAttributeNameComboBox
		|| NULL == m_pAttributeHintsComboBox
		|| NULL == m_pEntityListNameComboBox
		|| NULL == m_pEntityListFileTextCtrl
		|| NULL == m_pEntityListMaterialComboBox
		|| NULL == m_pMaterialListNameComboBox
		|| NULL == m_pMaterialListEntityTextCtrl
		|| NULL == m_pMaterialListMaterialComboBox
		|| NULL == m_pLocatorNameComboBox
		|| NULL == m_pAttachObjNameComboBox
		|| NULL == m_pSlotNameComboBox
		|| NULL == m_pEffectNameComboBox
		|| NULL == m_pBoneComboBox
		|| NULL == m_pSkeletonName
		|| NULL == m_pSkeletonTable)
		return;

	bool move = false;
	int x,y;
	//attribute
	m_pAttributeNameComboBox->GetPosition(&x,&y);

	m_pAttributeNameComboBox->Show(attributeshow);
	m_pAttributeHintsComboBox->Show(attributeshow);
	m_pSkeletonName->Show(attributeshow);
	m_pSlotNameComboBox->Show(attributeshow);
	m_pAttachObjNameComboBox->Show(attributeshow);
	AttrituteStaticBox->Show(attributeshow);
	AttachModelStaticBox->Show(attributeshow);
	AttributeNameStaticText->Show(attributeshow);
	AttributeHintsStaticText->Show(attributeshow);
	AttributeSlotStaticText->Show(attributeshow);	
	AttributeAttachStaticText->Show(attributeshow);
	//entitylist
	EntityStaticBox->Show(entityshow);
	m_pEntityListNameComboBox->Show(entityshow);
	m_pEntityListFileTextCtrl->Show(entityshow);
	m_pEntityListMaterialComboBox->Show(entityshow);
	//EntityStaticText->Show(entityshow);		
	EntityNameStaticText->Show(entityshow);	
	EntityFileStaticText->Show(entityshow);

	EntityMaterialStaticText->GetPosition(&x,&y);
	//EntityMaterialStaticText->SetPosition(wxPoint(186,86));
	//m_pEntityListNameComboBox->GetPosition(&x,&y);
	//m_pEntityListNameComboBox->Move(x,y-154);	
	//m_pEntityListFileTextCtrl->GetPosition(&x,&y);
	//m_pEntityListFileTextCtrl->Move(x,y-154);	
	//m_pEntityListMaterialComboBox->GetPosition(&x,&y);
	//m_pEntityListMaterialComboBox->Move(x,y-154);	
	//EntityStaticText->GetPosition(&x,&y);
	//EntityStaticText->Move(x,y-154);	
	//EntityNameStaticText->GetPosition(&x,&y);
	//EntityNameStaticText->Move(x,y-154);	
	//EntityFileStaticText->GetPosition(&x,&y);
	//EntityFileStaticText->Move(x,y-154);	
	//EntityMaterialStaticText->GetPosition(&x,&y);
	//EntityMaterialStaticText->Move(x,y-154);	

	//materiallist
	m_pMaterialListNameComboBox->Show(matshow);
	m_pMaterialListEntityTextCtrl->Show(matshow);
	m_pMaterialListMaterialComboBox->Show(matshow);
	MatStaticBox->Show(matshow);		
	MatNameStaticText->Show(matshow);	
	MatEntityStaticText->Show(matshow);	
	MatMaterialStaticText->Show(matshow);
	//locator
	m_pLocatorNameComboBox->Show(locatorshow);
	m_pBoneComboBox->Show(locatorshow);
	m_pEffectNameComboBox->Show(locatorshow);
	m_pLocatorPosTextCtrl->Show(locatorshow);
	m_pLocatorOriTextCtrl->Show(locatorshow);
	LocatorStaticBox->Show(locatorshow);
	PosAndOriStaticBox->Show(locatorshow);
	LocatorNameStaticText->Show(locatorshow);	
	LocatorBoneStaticText->Show(locatorshow);	
	LocatorEffectStaticText->Show(locatorshow);	
	LocatorPosStaticText->Show(locatorshow);	
	LocatorOriStaticText->Show(locatorshow);	
	//skeleton
	m_pSkeletonName->Show(skeletonshow);
	m_pSkeletonTable->Show(skeletonshow);
	m_pSkeletonAdd->Show(skeletonshow);
	m_pSkeletonDel->Show(skeletonshow);
	SkeStaticBox->Show(skeletonshow);		
	SkeNameStaticText->Show(skeletonshow);
	SkeTableStaticText->Show(skeletonshow);

}

void ObjFileEditDialog::OnSelObjTemplate(wxCommandEvent &e)
{
	if(NULL == GetDataManipulatr())
		return;

	Fairy::CGod* pModel = GetDataManipulatr()->FindModel(GetDataManipulatr()->m_CurModelName);

	if(NULL == pModel)
	{
		wxMessageBox("请选择需要创建obj文件的模型!");
		return;
	}

	if(!pModel->m_IsCanSelectTemplate)
	{
		wxMessageBox("该模型已有对应的obj文件,如要生成obj文件,请使用mesh文件创建!");
		return;
	}

	DString templatename			  = m_pObjFileTable->GetStringSelection().c_str();

	if(templatename.empty())
		templatename			      = m_pObjFileTable->GetValue().c_str();

	pModel->m_IsAvatarTemplate = false;

	pModel->UpdateObjTemplateData(templatename);
}

void ObjFileEditDialog::OnChangeLocator(wxCommandEvent &e)
{
	if(NULL == m_pLocatorNameComboBox
	|| NULL == GetDataManipulatr())

	 return;

	Fairy::CGod* pModel = GetDataManipulatr()->FindModel(GetDataManipulatr()->m_CurModelName);

	if(NULL == pModel)
		return;

	DString locatorname			  = m_pLocatorNameComboBox->GetStringSelection().c_str();

	if(locatorname.empty())
		locatorname			      = m_pLocatorNameComboBox->GetValue().c_str();

	if(locatorname.empty())
		return;

	std::vector<Ogre::String> locatorvec = Ogre::StringUtil::split(locatorname,"\\");

	if(locatorvec.size() < 2)
		return;

	const DString bone			  = locatorvec[0];

	const DString locator		  = locatorvec[1];

	pModel->UpdateLocatorPosAndOriData(locator);
}

void ObjFileEditDialog::OnAddSkeleton(wxCommandEvent &e)
{
	if(NULL == m_pSkeletonAdd
	|| NULL == m_pSkeletonTable
	|| NULL == m_pSkeletonName
	|| NULL == GetDataManipulatr())

	return;

	Fairy::CGod* pModel = GetDataManipulatr()->FindModel(GetDataManipulatr()->m_CurModelName);

	if(NULL == pModel)
		return;

	DString skeletonname			  = m_pSkeletonTable->GetStringSelection().c_str();

	if(skeletonname.empty())
		skeletonname			      = m_pSkeletonTable->GetValue().c_str();

	if(skeletonname.empty())
	{
		wxMessageBox("请选择要添加的骨骼!");
		return;
	}

	pModel->m_AniSkelist.push_back(skeletonname);

	DStringList Anilist;

	pModel->m_AniSkelistLink.insert(Fairy::CGod::AniSkelistLink::value_type(skeletonname,Anilist));

	wxMessageBox(wxString::Format("添加骨骼<%s>成功!",skeletonname.c_str()));

	m_pSkeletonName->Clear();

	for(int i=0;i<(int)pModel->m_AniSkelist.size();i++)
	{
		m_pSkeletonName->AppendString(pModel->m_AniSkelist[i].c_str());
	}
}

void ObjFileEditDialog::OnDelSkeleton(wxCommandEvent &e)
{
	if(NULL == m_pSkeletonAdd
	|| NULL == m_pSkeletonName
	|| NULL == GetDataManipulatr())

	return;

	Fairy::CGod* pModel = GetDataManipulatr()->FindModel(GetDataManipulatr()->m_CurModelName);

	if(NULL == pModel)
		return;

	DString skeletonname			  = m_pSkeletonName->GetStringSelection().c_str();

	if(skeletonname.empty())
	{
		wxMessageBox("请选择要删除的骨骼!");
		return;
	}

	int nSize = pModel->m_AniSkelist.size();

	DStringList::iterator it;

	for(int i=0;i<nSize;i++)
	{
		it = pModel->m_AniSkelist.begin()+i;

		if(skeletonname.compare(pModel->m_AniSkelist[i]) == 0)
		{
			pModel->m_AniSkelist.erase(it);
			break;
		}
	}

	Fairy::CGod::AniSkelistLink::iterator itlink = pModel->m_AniSkelistLink.find(skeletonname);

	if(itlink != pModel->m_AniSkelistLink.end())
	{
		pModel->m_AniSkelistLink.erase(skeletonname);
	}

	wxMessageBox(wxString::Format("删除骨骼<%s>成功!",skeletonname.c_str()));

	m_pSkeletonName->Clear();

	for(int i=0;i<(int)pModel->m_AniSkelist.size();i++)
	{
		m_pSkeletonName->AppendString(pModel->m_AniSkelist[i].c_str());
	}
}