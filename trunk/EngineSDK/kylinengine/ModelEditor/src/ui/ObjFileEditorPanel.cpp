///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "ObjFileEditorPanel.h"
#include "res/ObjFileEditorPanelHelper.h"

///////////////////////////////////////////////////////////////////////////
#include "DataManipulator.h"
#include <OgreStringConverter.h>
#include <OgreStringVector.h>

BEGIN_EVENT_TABLE(ObjFileEditorPanel, wxPanel)

EVT_COMBOBOX(ID_CMB_ATTRBUTES_NAME, OnChangeAttributesName)
EVT_COMBOBOX(ID_CMB_ATTRBUTES_HINTS, OnChangeAttributesHints)	
EVT_COMBOBOX(ID_CMB_ENTITYS_NAME, OnChangeEntitiesName)
EVT_COMBOBOX(ID_CMB_ENTITYS_FILE, OnChangeEntitiesFileAndMaterial)
EVT_COMBOBOX(ID_CMB_ENTITYS_MATERIAL, OnChangeEntitiesFileAndMaterial)
EVT_COMBOBOX(ID_CMB_MATERIALS_NAME, OnChangeMaterialsName)
EVT_COMBOBOX(ID_CMB_MATERIALS_ENTITY, OnChangeMaterialsEntityAndMat)
EVT_COMBOBOX(ID_CMB_MATERIALS_MATERIAL, OnChangeMaterialsEntityAndMat)
EVT_COMBOBOX(ID_CMB_SLOTS_LOCATOR, OnChangeSlotsLocator)
EVT_COMBOBOX(ID_CMB_SLOTS_BONE, OnChangeBoneAndTrans)
EVT_COMBOBOX(ID_CMB_SLOTS_SLOT, OnChangeSlotsSlot)
EVT_COMBOBOX(ID_CMB_SLOTS_OBJECT, OnChangeSlotsSlot)
EVT_COMBOBOX(ID_CMB_SLOTS_EFFECT, OnChangeSlotsEffect)

EVT_TEXT_ENTER(ID_TXT_SLOTS_POS, OnChangeBoneAndTrans)
EVT_TEXT_ENTER(ID_TXT_SLOTS_ROT, OnChangeBoneAndTrans)

EVT_TEXT(ID_CMB_ATTRBUTES_NAME, OnEditAttibuteName)
EVT_TEXT(ID_CMB_SLOTS_LOCATOR, OnEditLocatorName)
EVT_TEXT_ENTER(ID_CMB_ATTRBUTES_NAME, OnEditAttibuteName)
EVT_TEXT_ENTER(ID_CMB_SLOTS_LOCATOR, OnEditLocatorName)

EVT_BUTTON(ID_BTN_SKELETONS_ADD, OnBtnSkeletonsAdd)
EVT_BUTTON(ID_BTN_SKELETONS_DEL, OnBtnSkeletonsDel)

EVT_BUTTON(ID_BTN_RESET_POS, OnReset)
EVT_BUTTON(ID_BTN_RESET_ROT, OnReset)

EVT_COMBOBOX(ID_CMB_EFFECTS_EFFECT,OnChangeEffectName)
EVT_COMBOBOX(ID_CMB_EFFECTS_FILE,OnChangeEffectInfo)
EVT_COMBOBOX(ID_CMB_EFFECTS_LOCATOR,OnChangeEffectInfo)

EVT_TEXT_ENTER(ID_CMB_EFFECTS_EFFECT, OnEditEffectName)

EVT_CHECKBOX(ID_CHK_EFFECTS_TRANS,OnChangeEffectInfo)
EVT_TEXT_ENTER(ID_TXT_EFFECTS_COLOUR,OnChangeEffectInfo)

EVT_TEXT_ENTER(ID_TXT_ATTRIBUTES_VALUE,OnEditAttributesValue)

EVT_CHECKBOX(ID_CHK_SAVE_DEFAULT_SKELETON, OnSaveDefaultSkeleton)

END_EVENT_TABLE()


ObjFileEditorPanel::ObjFileEditorPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name) : wxPanel( parent, id, pos, size, style ), m_pSceneManipulator(NULL)
{
	m_mainBoxSizer = new wxBoxSizer( wxVERTICAL );

	m_objFileSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("objFile") ), wxHORIZONTAL );

	fileName = new wxStaticText( this, wxID_ANY, wxT("file"), wxDefaultPosition, wxDefaultSize, 0 );
	m_objFileSizer->Add( fileName, 0, wxALL, 5 );

	m_fileNameTxt = new wxTextCtrl( this, ID_TXT_FILE_NAME, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	m_objFileSizer->Add( m_fileNameTxt, 1, wxALL, 5 );

	m_mainBoxSizer->Add( m_objFileSizer, 1, wxEXPAND, 5 );

	
	m_attributeSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("attributes") ), wxHORIZONTAL );
	
	wxGridSizer* attributeGridSizer;
	attributeGridSizer = new wxGridSizer( 2, 2, 0, 0 );
	
	wxBoxSizer* attributeNameSizer;
	attributeNameSizer = new wxBoxSizer( wxHORIZONTAL );
	
	attibuteName = new wxStaticText( this, wxID_ANY, wxT("name"), wxDefaultPosition, wxDefaultSize, 0 );
	attributeNameSizer->Add( attibuteName, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_attributeNameCombo = new wxComboBox( this, ID_CMB_ATTRBUTES_NAME, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN|wxTE_PROCESS_ENTER ); 
	attributeNameSizer->Add( m_attributeNameCombo, 1, wxALIGN_CENTER|wxALIGN_RIGHT|wxTOP, 5 );
	
	attributeGridSizer->Add( attributeNameSizer, 1, wxEXPAND, 5 );
	
	wxBoxSizer* attributeHintsSizer;
	attributeHintsSizer = new wxBoxSizer( wxHORIZONTAL );
	
	attributeHints = new wxStaticText( this, wxID_ANY, wxT("hints"), wxDefaultPosition, wxDefaultSize, 0 );
	attributeHintsSizer->Add( attributeHints, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_attributeHintsCombo = new wxComboBox( this, ID_CMB_ATTRBUTES_HINTS, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY ); 
	attributeHintsSizer->Add( m_attributeHintsCombo, 1, wxALIGN_CENTER|wxALIGN_RIGHT|wxTOP, 5 );

	m_attributeHintsCombo->Enable(false);
	
	attributeGridSizer->Add( attributeHintsSizer, 1, wxEXPAND, 5 );

	//value
	wxBoxSizer* attributeValueSizer;
	attributeValueSizer = new wxBoxSizer( wxHORIZONTAL );

	attributeValue = new wxStaticText( this, wxID_ANY, wxT("value"), wxDefaultPosition, wxDefaultSize, 0 );
	attributeValueSizer->Add( attributeValue, 0, wxALIGN_CENTER|wxALL, 5 );

	m_attributeValueText = new wxTextCtrl( this, ID_TXT_ATTRIBUTES_VALUE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_PROCESS_TAB );
 	attributeValueSizer->Add( m_attributeValueText, 1, wxALIGN_CENTER|wxALL, 5 ); 
 	m_attributeValueText->Enable(false);

	attributeGridSizer->Add( attributeValueSizer, 1, wxEXPAND, 5 );

	
	m_attributeSizer->Add( attributeGridSizer, 1, wxEXPAND, 5 );
	
	m_mainBoxSizer->Add( m_attributeSizer, 2, wxEXPAND, 5 );
	
	m_entitiesSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("entityList") ), wxHORIZONTAL );
	
	wxGridSizer* entityGridSizer;
	entityGridSizer = new wxGridSizer( 2, 2, 0, 0 );
	
	wxBoxSizer* entityNameSizer;
	entityNameSizer = new wxBoxSizer( wxHORIZONTAL );
	
	entityName = new wxStaticText( this, wxID_ANY, wxT("name"), wxDefaultPosition, wxDefaultSize, 0 );
	entityNameSizer->Add( entityName, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_entityNameCombo = new wxComboBox( this, ID_CMB_ENTITYS_NAME, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY ); 
	entityNameSizer->Add( m_entityNameCombo, 1, wxALIGN_CENTER|wxALIGN_RIGHT|wxTOP, 5 );
	
	entityGridSizer->Add( entityNameSizer, 1, wxEXPAND, 5 );
	
	wxBoxSizer* entityFileSizer;
	entityFileSizer = new wxBoxSizer( wxHORIZONTAL );
	
	entityFile = new wxStaticText( this, wxID_ANY, wxT("file"), wxDefaultPosition, wxDefaultSize, 0 );
	entityFileSizer->Add( entityFile, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_entityFileCombo = new wxComboBox( this, ID_CMB_ENTITYS_FILE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY ); 
	entityFileSizer->Add( m_entityFileCombo, 1, wxALIGN_CENTER|wxALL, 5 );
	
	entityGridSizer->Add( entityFileSizer, 1, wxEXPAND, 5 );
	
	wxBoxSizer* entityMaterialSizer;
	entityMaterialSizer = new wxBoxSizer( wxHORIZONTAL );
	
	entityMaterial = new wxStaticText( this, wxID_ANY, wxT("material"), wxDefaultPosition, wxDefaultSize, 0 );
	entityMaterialSizer->Add( entityMaterial, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_entityMaterialCombo = new wxComboBox( this, ID_CMB_ENTITYS_MATERIAL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY ); 
	entityMaterialSizer->Add( m_entityMaterialCombo, 1, wxALIGN_CENTER|wxALIGN_RIGHT|wxTOP, 5 );
	
	entityGridSizer->Add( entityMaterialSizer, 1, wxEXPAND, 5 );
	
	m_entitiesSizer->Add( entityGridSizer, 1, wxEXPAND, 5 );
	
	m_mainBoxSizer->Add( m_entitiesSizer, 2, wxEXPAND, 5 );
	
	m_materialsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("materialList") ), wxHORIZONTAL );
	
	wxGridSizer* materialGridSizer;
	materialGridSizer = new wxGridSizer( 2, 2, 0, 0 );
	
	wxBoxSizer* materialNameSizer;
	materialNameSizer = new wxBoxSizer( wxHORIZONTAL );
	
	materialName = new wxStaticText( this, wxID_ANY, wxT("name"), wxDefaultPosition, wxDefaultSize, 0 );
	materialNameSizer->Add( materialName, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_materialNameCombo = new wxComboBox( this, ID_CMB_MATERIALS_NAME, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY ); 
	materialNameSizer->Add( m_materialNameCombo, 1, wxALIGN_CENTER|wxALL, 5 );
	
	materialGridSizer->Add( materialNameSizer, 1, wxEXPAND, 5 );
	
	wxBoxSizer* materialEntitySizer;
	materialEntitySizer = new wxBoxSizer( wxHORIZONTAL );
	
	materialEntity = new wxStaticText( this, wxID_ANY, wxT("entity"), wxDefaultPosition, wxDefaultSize, 0 );
	materialEntitySizer->Add( materialEntity, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_materialEntityCombo = new wxComboBox( this, ID_CMB_MATERIALS_ENTITY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY ); 
	materialEntitySizer->Add( m_materialEntityCombo, 1, wxALIGN_CENTER|wxALL, 5 );
	
	materialGridSizer->Add( materialEntitySizer, 1, wxEXPAND, 5 );
	
	wxBoxSizer* materialSizer;
	materialSizer = new wxBoxSizer( wxHORIZONTAL );
	
	material = new wxStaticText( this, wxID_ANY, wxT("material"), wxDefaultPosition, wxDefaultSize, 0 );
	materialSizer->Add( material, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_materialCombo = new wxComboBox( this, ID_CMB_MATERIALS_MATERIAL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY ); 
	materialSizer->Add( m_materialCombo, 1, wxALIGN_CENTER|wxALL, 5 );
	
	materialGridSizer->Add( materialSizer, 1, wxEXPAND, 5 );
	
	m_materialsSizer->Add( materialGridSizer, 1, wxEXPAND, 5 );
	
	m_mainBoxSizer->Add( m_materialsSizer, 2, wxEXPAND, 5 );
	
	m_slotsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("slotList") ), wxHORIZONTAL );
	
	wxGridSizer* slotGridSizer;
	slotGridSizer = new wxGridSizer( 5, 2, 0, 0 );	

	wxBoxSizer* locatorSizer;
	locatorSizer = new wxBoxSizer( wxHORIZONTAL );

	locatorName = new wxStaticText( this, wxID_ANY, wxT("locator"), wxDefaultPosition, wxDefaultSize, 0 );
	locatorSizer->Add( locatorName, 0, wxALL, 5 );

	m_locatorNameCombo = new wxComboBox( this, ID_CMB_SLOTS_LOCATOR, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN|wxTE_PROCESS_ENTER ); 
	locatorSizer->Add( m_locatorNameCombo, 1, wxALL, 5 );

	slotGridSizer->Add( locatorSizer, 1, wxALIGN_CENTER|wxEXPAND, 5 );	
	
	wxBoxSizer* boneSizer;
	boneSizer = new wxBoxSizer( wxHORIZONTAL );

	boneName = new wxStaticText( this, wxID_ANY, wxT("bone"), wxDefaultPosition, wxDefaultSize, 0 );
	boneSizer->Add( boneName, 0, wxALL, 5 );

	m_boneNameCombo = new wxComboBox( this, ID_CMB_SLOTS_BONE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY ); 
	boneSizer->Add( m_boneNameCombo, 1, wxALL, 5 );

	slotGridSizer->Add( boneSizer, 1, wxEXPAND, 5 );
	
	wxBoxSizer* slotSizer;
	slotSizer = new wxBoxSizer( wxHORIZONTAL );
	
	slotName = new wxStaticText( this, wxID_ANY, wxT("slot"), wxDefaultPosition, wxDefaultSize, 0 );
	slotSizer->Add( slotName, 0, wxALL, 5 );
	
	m_slotNameCombo = new wxComboBox( this, ID_CMB_SLOTS_SLOT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY ); 
	slotSizer->Add( m_slotNameCombo, 1, wxALL, 5 );
	
	slotGridSizer->Add( slotSizer, 1, wxALIGN_CENTER|wxEXPAND, 5 );
	
	
	slotGridSizer->Add( 0, 0, 1, wxEXPAND, 5 );
	
	wxBoxSizer* objectSizer;
	objectSizer = new wxBoxSizer( wxHORIZONTAL );
	
	objName = new wxStaticText( this, wxID_ANY, wxT("object"), wxDefaultPosition, wxDefaultSize, 0 );
	objectSizer->Add( objName, 0, wxALL, 5 );
	
	m_objNameCombo = new wxComboBox( this, ID_CMB_SLOTS_OBJECT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY ); 
	objectSizer->Add( m_objNameCombo, 1, wxALL, 5 );
	
	slotGridSizer->Add( objectSizer, 1, wxEXPAND, 5 );
	
	wxBoxSizer* attributeEffectSizer;
	attributeEffectSizer = new wxBoxSizer( wxHORIZONTAL );
	
	attributeEffect = new wxStaticText( this, wxID_ANY, wxT("effect"), wxDefaultPosition, wxDefaultSize, 0 );
	attributeEffectSizer->Add( attributeEffect, 0, wxALL, 5 );
	
	m_attributeEffectCombo = new wxComboBox( this, ID_CMB_SLOTS_EFFECT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY ); 
	attributeEffectSizer->Add( m_attributeEffectCombo, 1, wxALL, 5 );
	
	slotGridSizer->Add( attributeEffectSizer, 1, wxEXPAND, 5 );
	
	wxBoxSizer* posSizer;
	posSizer = new wxBoxSizer( wxHORIZONTAL );
	
	posInfo = new wxStaticText( this, wxID_ANY, wxT("pos"), wxDefaultPosition, wxDefaultSize, 0 );
	posSizer->Add( posInfo, 0, wxALL, 5 );
	
	m_posText = new wxTextCtrl( this, ID_TXT_SLOTS_POS, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_PROCESS_TAB );
	posSizer->Add( m_posText, 1, wxALL, 5 );
	
	slotGridSizer->Add( posSizer, 1, wxALIGN_CENTER|wxEXPAND, 5 );

	wxBoxSizer* rotSizer;
	rotSizer = new wxBoxSizer( wxHORIZONTAL );
	
	rotInfo = new wxStaticText( this, wxID_ANY, wxT("rot"), wxDefaultPosition, wxDefaultSize, 0 );
	rotSizer->Add( rotInfo, 0, wxALL, 5 );
	
	m_rotText = new wxTextCtrl( this, ID_TXT_SLOTS_ROT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_PROCESS_TAB );
	rotSizer->Add( m_rotText, 1, wxALL, 5 );
	
	slotGridSizer->Add( rotSizer, 1, wxEXPAND, 5 );

	wxBoxSizer* resetPosSizer;
	resetPosSizer = new wxBoxSizer( wxVERTICAL );

	m_resetPos = new wxButton( this, ID_BTN_RESET_POS, wxT("resetPos"), wxDefaultPosition, wxDefaultSize, 0 );
	resetPosSizer->Add( m_resetPos, 0, wxALL|wxALIGN_RIGHT, 5 );

	slotGridSizer->Add( resetPosSizer, 1, wxEXPAND, 5 );

	wxBoxSizer* resetRotSizer;
	resetRotSizer = new wxBoxSizer( wxVERTICAL );

	m_resetRot = new wxButton( this, ID_BTN_RESET_ROT, wxT("resetRot"), wxDefaultPosition, wxDefaultSize, 0 );
	resetRotSizer->Add( m_resetRot, 0, wxALL|wxALIGN_RIGHT, 5 );

	slotGridSizer->Add( resetRotSizer, 1, wxEXPAND, 5 );
	
	m_slotsSizer->Add( slotGridSizer, 1, wxEXPAND, 5 );
	
	m_mainBoxSizer->Add( m_slotsSizer, 5, wxEXPAND, 5 );

	/////////////////////////////////////

	m_effectsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("effectList") ), wxVERTICAL );

	wxBoxSizer* effectNameSizer;
	effectNameSizer = new wxBoxSizer( wxHORIZONTAL );

	m_effectNameLabel = new wxStaticText( this, wxID_ANY, wxT("name"), wxDefaultPosition, wxDefaultSize, 0 );
	effectNameSizer->Add( m_effectNameLabel, 0, wxALIGN_CENTER|wxALL, 5 );

	m_effectNameCombo = new wxComboBox( this, ID_CMB_EFFECTS_EFFECT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxTE_PROCESS_ENTER|wxTE_PROCESS_TAB ); 
	effectNameSizer->Add( m_effectNameCombo, 1, wxALIGN_CENTER|wxALL, 5 );

	m_effectsSizer->Add( effectNameSizer, 1, wxEXPAND, 5 );

	wxGridSizer* effectBaseSizer;
	effectBaseSizer = new wxGridSizer( 2, 2, 0, 0 );

	wxBoxSizer* effectFileSizer;
	effectFileSizer = new wxBoxSizer( wxHORIZONTAL );

	m_effectFileLabel = new wxStaticText( this, wxID_ANY, wxT("effect"), wxDefaultPosition, wxDefaultSize, 0 );
	effectFileSizer->Add( m_effectFileLabel, 0, wxALIGN_CENTER|wxALL, 5 );

	m_effectFileCombo = new wxComboBox( this, ID_CMB_EFFECTS_FILE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY ); 
	effectFileSizer->Add( m_effectFileCombo, 1, wxALIGN_CENTER|wxALL, 5 );

	effectBaseSizer->Add( effectFileSizer, 1, wxEXPAND, 5 );

	wxBoxSizer* effectLocatorSizer;
	effectLocatorSizer = new wxBoxSizer( wxHORIZONTAL );

	m_effectLocatorLabel = new wxStaticText( this, wxID_ANY, wxT("locator"), wxDefaultPosition, wxDefaultSize, 0 );
	effectLocatorSizer->Add( m_effectLocatorLabel, 0, wxALIGN_CENTER|wxALL, 5 );

	m_effectLocatorCombo = new wxComboBox( this, ID_CMB_EFFECTS_LOCATOR, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY); 
	effectLocatorSizer->Add( m_effectLocatorCombo, 1, wxALIGN_CENTER|wxALL, 5 );

	effectBaseSizer->Add( effectLocatorSizer, 1, wxEXPAND, 5 );

	wxBoxSizer* effectTranslateSizer;
	effectTranslateSizer = new wxBoxSizer( wxHORIZONTAL );

	m_effectTransCheck = new wxCheckBox( this, ID_CHK_EFFECTS_TRANS, wxT("translateAll"), wxDefaultPosition, wxDefaultSize, 0 );

	effectTranslateSizer->Add( m_effectTransCheck, 0, wxALIGN_CENTER|wxALL, 5 );

	effectBaseSizer->Add( effectTranslateSizer, 1, wxEXPAND, 5 );

	wxBoxSizer* effectColourSizer;
	effectColourSizer = new wxBoxSizer( wxHORIZONTAL );

	m_effectColourLabel = new wxStaticText( this, wxID_ANY, wxT("colour"), wxDefaultPosition, wxDefaultSize, 0 );
	effectColourSizer->Add( m_effectColourLabel, 0, wxALIGN_CENTER|wxALL, 5 );

	m_effectColour = new wxTextCtrl( this, ID_TXT_EFFECTS_COLOUR, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_PROCESS_TAB);
	effectColourSizer->Add( m_effectColour, 1, wxALIGN_CENTER|wxALL, 5 );

	effectBaseSizer->Add( effectColourSizer, 1, wxEXPAND, 5 );

	m_effectsSizer->Add( effectBaseSizer, 2, wxEXPAND, 5 );

	m_mainBoxSizer->Add( m_effectsSizer, 3, wxEXPAND, 5 );
	/////////////////////////////////////
	
	m_skeletonSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("skeletonList") ), wxVERTICAL );

	wxBoxSizer* innerSkelentonSizer;
	innerSkelentonSizer = new wxBoxSizer( wxHORIZONTAL );
	
	skeletonName = new wxStaticText( this, wxID_ANY, wxT("skeleton"), wxDefaultPosition, wxDefaultSize, 0 );
	innerSkelentonSizer->Add( skeletonName, 0, wxALL, 5 );
	
	m_skeletonCombo = new wxComboBox( this, ID_CMB_SKELETONS_SKELETON, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY ); 
	innerSkelentonSizer->Add( m_skeletonCombo, 1, wxALL, 5 );
	
	m_skeletonSizer->Add( innerSkelentonSizer, 1, wxEXPAND, 5 );
	
	wxBoxSizer* btnSizer;
	btnSizer = new wxBoxSizer( wxHORIZONTAL );	

	m_saveDefaultCheck = new wxCheckBox( this, ID_CHK_SAVE_DEFAULT_SKELETON, wxT("saveDefault"), wxDefaultPosition, wxDefaultSize, 0 );
	m_saveDefaultCheck->SetValue(true);
	btnSizer->Add( m_saveDefaultCheck, 0, wxALL, 5 );
	
	m_addBtn = new wxButton( this, ID_BTN_SKELETONS_ADD, wxT("Add"), wxDefaultPosition, wxDefaultSize, 0 );
	btnSizer->Add( m_addBtn, 0, wxALL, 5 );
	
	m_DelBtn = new wxButton( this, ID_BTN_SKELETONS_DEL, wxT("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	btnSizer->Add( m_DelBtn, 0, wxALL, 5 );
	
	m_skeletonSizer->Add( btnSizer, 1, wxEXPAND, 5 );
	
	wxBoxSizer* resSizer;
	resSizer = new wxBoxSizer( wxHORIZONTAL );
	
	resName = new wxStaticText( this, wxID_ANY, wxT("resources"), wxDefaultPosition, wxDefaultSize, 0 );
	resSizer->Add( resName, 0, wxALL, 5 );
	
	m_resCombo = new wxComboBox( this, ID_CMB_SKELETONS_RESOURCE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY ); 
	resSizer->Add( m_resCombo, 1, wxALL, 5 );
	
	m_skeletonSizer->Add( resSizer, 1, wxEXPAND, 5 );
	
	m_mainBoxSizer->Add( m_skeletonSizer, 3, wxEXPAND, 5 );
	
	this->SetSizer( m_mainBoxSizer );
	this->Layout();

	SwitchUI();
}

ObjFileEditorPanel::~ObjFileEditorPanel()
{ 
}

bool ObjFileEditorPanel::frameStarted(const Ogre::FrameEvent& evt)
{
	return true;
}

// TODO:Call this function when u open a obj file
void ObjFileEditorPanel::SwitchUI(int index, bool bShow)
{
	switch(index)
	{
		case IFS_FILE_SIZER:
		{
			m_mainBoxSizer->Show(m_objFileSizer,bShow,true);
			m_mainBoxSizer->Layout();
		}
		break;
		case IFS_ATTRIBUTE_SIZER:
		{
			m_mainBoxSizer->Show(m_attributeSizer,bShow,true);
			m_mainBoxSizer->Layout();
		}
		break;
		case IFS_ENTITY_SIZER:
		{
			m_mainBoxSizer->Show(m_entitiesSizer,bShow,true);
			m_mainBoxSizer->Layout();
		}
		break;
		case IFS_MATERIAL_SIZER:
		{
			m_mainBoxSizer->Show(m_materialsSizer,bShow,true);
			m_mainBoxSizer->Layout();
		}
		break;
		case IFS_SLOTS_SIZER:
		{
			m_mainBoxSizer->Show(m_slotsSizer,bShow,true);
			m_mainBoxSizer->Layout();
		}
		break;
		case IFS_EFFECTS_SIZER:
		{
			m_mainBoxSizer->Show(m_effectsSizer,bShow,true);
			m_mainBoxSizer->Layout();
		}
		break;
		case IFS_SKELETON_SIZER:
		{
			m_mainBoxSizer->Show(m_skeletonSizer,bShow,true);
			m_mainBoxSizer->Layout();
		}
		break;
		default:
		{
			m_mainBoxSizer->Show(m_objFileSizer,bShow,true);
			m_mainBoxSizer->Show(m_attributeSizer,bShow,true);
			m_mainBoxSizer->Show(m_entitiesSizer,bShow,true);
			m_mainBoxSizer->Show(m_materialsSizer,bShow,true);
			m_mainBoxSizer->Show(m_slotsSizer,bShow,true);
			m_mainBoxSizer->Show(m_effectsSizer,bShow,true);
			m_mainBoxSizer->Show(m_skeletonSizer,bShow,true);
		}
		break;
	}
}

// For attributes
void ObjFileEditorPanel::OnChangeAttributesName(wxCommandEvent &e)
{
	if (!m_attributeNameCombo || !GetDataManipulator())
	{
		return;
	}

	Fairy::LogicModel* pObj = GetDataManipulator()->m_pObjTemplate;
	if (!pObj)
	{
		return;
	}

	Ogre::String  attributeName = m_attributeNameCombo->GetStringSelection().c_str();
	if(attributeName.empty())
	{
		attributeName = m_attributeNameCombo->GetValue().c_str();
	}

	m_curAttibuteName = attributeName;
		
	GetDataManipulator()->UpdateAttributeInfo(attributeName);
}

void ObjFileEditorPanel::OnChangeAttributesHints(wxCommandEvent &e)
{
	if (!m_attributeNameCombo|| !m_attributeHintsCombo || !GetDataManipulator())
	{
		return;
	}

	Fairy::LogicModel* pObj = GetDataManipulator()->m_pObjTemplate;
	if (!pObj)
	{
		return;
	}

	Ogre::String  attibuteName = m_attributeNameCombo->GetStringSelection().c_str();
	if(attibuteName.empty())
	{
		attibuteName = m_attributeNameCombo->GetValue().c_str();
	}
	if (attibuteName.empty())
	{
		return;
	}

	Ogre::String  hint = m_attributeHintsCombo->GetStringSelection().c_str();
	if(hint.empty())
	{
		hint = m_attributeHintsCombo->GetValue().c_str();
	}

	if(pObj->changeAttributeHints(attibuteName,hint))
	{
		wxLogMessage(wxT("成功修改属性<%s>提示为<%s>!"),attibuteName.c_str(),hint.c_str());
	}
	else
	{
		wxLogMessage(wxT("操作失败!没有找到属性<%s>!"),attibuteName.c_str());
	}
}

// For entities
void ObjFileEditorPanel::OnChangeEntitiesName(wxCommandEvent &e)
{
	if (!m_entityNameCombo || !GetDataManipulator())
	{
		return;
	}

	Fairy::LogicModel* pObj = GetDataManipulator()->m_pObjTemplate;
	if (!pObj)
	{
		return;
	}

	Ogre::String  entityName = m_entityNameCombo->GetStringSelection().c_str();
	if(entityName.empty())
	{
		entityName = m_entityNameCombo->GetValue().c_str();
	}

	GetDataManipulator()->UpdateEntityInfo(entityName);
}

void ObjFileEditorPanel::OnChangeEntitiesFileAndMaterial(wxCommandEvent &e)
{
	if (!m_entityNameCombo|| !m_entityFileCombo|| !m_entityMaterialCombo || !GetDataManipulator())
	{
		return;
	}

	Fairy::LogicModel* pObj = GetDataManipulator()->m_pObjTemplate;
	if (!pObj)
	{
		return;
	}

	Ogre::String  entityName = m_entityNameCombo->GetStringSelection().c_str();
	if(entityName.empty())
	{
		entityName = m_entityNameCombo->GetValue().c_str();
	}

	if (entityName.empty())
	{
		return;
	}

	Ogre::String materialName = m_entityMaterialCombo->GetStringSelection().c_str();
	if (materialName.empty())
	{
		 materialName = m_entityMaterialCombo->GetValue().c_str();
	}

	if (!materialName.empty())
	{
		Ogre::StringVector vec = Ogre::StringUtil::split(materialName,".");
		materialName = vec[0];
	}

	Ogre::String meshName = m_entityFileCombo->GetStringSelection().c_str();
	if (meshName.empty())
	{
		meshName = m_entityFileCombo->GetValue().c_str();
	}

	if(GetDataManipulator()->changeEntityMeshAndMat(entityName, meshName, materialName))
	{
		wxLogMessage(wxT("成功修改属性<%s>的文件为<%s>,材质为<%s>!"),entityName.c_str(),meshName.c_str(), materialName.c_str());
	}
	else
	{
		wxLogMessage(wxT("操作失败!"));
	}
}

// For materials
void ObjFileEditorPanel::OnChangeMaterialsName(wxCommandEvent &e)
{
	if (!m_materialNameCombo || !GetDataManipulator())
	{
		return;
	}

	Fairy::LogicModel* pObj = GetDataManipulator()->m_pObjTemplate;
	if (!pObj)
	{
		return;
	}

	Ogre::String  materialName = m_materialNameCombo->GetStringSelection().c_str();
	if(materialName.empty())
	{
		materialName = m_materialNameCombo->GetValue().c_str();
	}

	GetDataManipulator()->UpdateMaterialInfo(materialName);
}

void ObjFileEditorPanel::OnChangeMaterialsEntityAndMat(wxCommandEvent &e)
{
	if (!m_materialNameCombo|| !m_materialEntityCombo|| !m_materialCombo || !GetDataManipulator())
	{
		return;
	}

	Fairy::LogicModel* pObj = GetDataManipulator()->m_pObjTemplate;
	if (!pObj)
	{
		return;
	}

	Ogre::String  materialName = m_materialNameCombo->GetStringSelection().c_str();
	if(materialName.empty())
	{
		materialName = m_materialNameCombo->GetValue().c_str();
	}

	if (materialName.empty())
	{
		return;
	}

	Ogre::String materialEntity = m_materialEntityCombo->GetStringSelection().c_str();
	if (materialEntity.empty())
	{
		materialEntity = m_materialEntityCombo->GetValue().c_str();
	}

	Ogre::String material = m_materialCombo->GetStringSelection().c_str();
	if (material.empty())
	{
		material = m_materialCombo->GetValue().c_str();
		
	}
	
	if (!material.empty())
	{
		Ogre::StringVector vec = Ogre::StringUtil::split(material,".");
		material = vec[0];
	}

	if(GetDataManipulator()->changeMaterialEntiryAndMat(materialName, materialEntity, material))
	{
		wxLogMessage(wxT("成功修改属性<%s>的实体为<%s>,材质为<%s>!"),materialName.c_str(),materialEntity.c_str(), material.c_str());
	}
	else
	{
		wxLogMessage(wxT("操作失败!"));
	}
}

// For slots
void ObjFileEditorPanel::OnChangeSlotsLocator(wxCommandEvent &e)
{
	if (!m_locatorNameCombo || !GetDataManipulator())
	{
		return;
	}

	Fairy::LogicModel* pObj = GetDataManipulator()->m_pObjTemplate;
	if (!pObj)
	{
		return;
	}

	Ogre::String  locatorName = m_locatorNameCombo->GetStringSelection().c_str();
	if(locatorName.empty())
	{
		locatorName = m_locatorNameCombo->GetValue().c_str();
	}

	if (locatorName.empty())
	{
		return;
	}

	Ogre::String selLocator = locatorName;
	if (locatorName.compare(Fairy::DELLOCATORTIP) == 0)
	{
		if (m_curLocatorName.empty())
		{
			return;
		}
		//Show the confirm dialog
		wxMessageDialog dialog( NULL,wxString::Format("确定要删除挂点<%s>吗?",m_curLocatorName.c_str()),
			wxT("确认!"),
			wxNO_DEFAULT|wxYES_NO|wxICON_INFORMATION);
		switch ( dialog.ShowModal() )
		{
			case wxID_NO:
				return;
			case wxID_YES:				
			default:
				{				
					break;
				}	
		}

		//Delete current locator
		if (GetDataManipulator()->deleteCurLocator())
		{					
			wxLogMessage(wxT("成功删除挂点<%s>"),m_curLocatorName.c_str());
			int idx = m_locatorNameCombo->FindString(m_curLocatorName.c_str());
			m_locatorNameCombo->Delete(idx);		

			// Delete the locator from effect locators
			idx = m_effectLocatorCombo->FindString(m_curLocatorName.c_str());
			m_effectLocatorCombo->Delete(idx);
			
			m_curLocatorName = "";

			GetDataManipulator()->UpdateEffectInfo(m_curEffectName);
		}
		else
		{
			wxLogMessage(wxT("操作失败!"));
		}

		m_locatorNameCombo->SetValue(m_curLocatorName.c_str());
		GetDataManipulator()->UpdateLocatorInfo(m_curLocatorName);	
		return;
	}

	if (locatorName.compare(Fairy::NEWLOCATORTIP) == 0)
	{
		wxMessageDialog dialog( NULL,wxT("确定要新建挂点吗?"),
			wxT("确认!"),
			wxNO_DEFAULT|wxYES_NO|wxICON_INFORMATION);
		switch ( dialog.ShowModal() )
		{
		case wxID_NO:
			return;
		case wxID_YES:				
		default:
			{				
				break;
			}	
		}

		selLocator = GetDataManipulator()->_formatValidLocatorName();

		std::map<Ogre::String, Ogre::String>::iterator boneIter = GetDataManipulator()->m_curBoneList.begin();
		pObj->addLocator(selLocator, boneIter->first, Ogre::Vector3::ZERO, Ogre::Quaternion::IDENTITY);			

		m_locatorNameCombo->AppendString(selLocator);
		m_locatorNameCombo->SetStringSelection(selLocator);

		// Add the new locator name to the effect locators
		m_effectLocatorCombo->AppendString(selLocator);
	}

	m_curLocatorName = locatorName;

	GetDataManipulator()->UpdateLocatorInfo(locatorName);

}

void ObjFileEditorPanel::OnChangeBoneAndTrans(wxCommandEvent &e)
{
	if (!m_locatorNameCombo ||!m_boneNameCombo || !GetDataManipulator())
	{
		return;
	}

	Fairy::LogicModel* pObj = GetDataManipulator()->m_pObjTemplate;
	if (!pObj)
	{
		return;
	}

	Ogre::String  locatorName = m_locatorNameCombo->GetStringSelection().c_str();
	if(locatorName.empty())
	{
		locatorName = m_locatorNameCombo->GetValue().c_str();
	}

	if (locatorName.empty())
	{
		return;
	}

	Ogre::String  boneName = m_boneNameCombo->GetStringSelection().c_str();
	if(boneName.empty())
	{
		boneName = m_boneNameCombo->GetValue().c_str();
	}

	Ogre::String offsetPos = m_posText->GetValue();
	Ogre::Vector3 vPos = Ogre::StringConverter::parseVector3(offsetPos);

	Ogre::String offsetRot = m_rotText->GetValue();
	Ogre::Quaternion qRot = Ogre::StringConverter::parseQuaternion(offsetRot);

	if(GetDataManipulator()->ChangeLocatorInfo(locatorName, boneName, vPos, qRot))
	{	
		GetDataManipulator()->UpdateLocatorInfo(locatorName);
		wxLogMessage(wxT("成功修改挂点<%s>的骨头为<%s>,位移为<%s>,朝向为<%s>!"),locatorName.c_str(),boneName.c_str(), offsetPos.c_str(), offsetRot.c_str());
	}
	else
	{
		wxLogMessage(wxT("操作失败!"));
	}
}
void ObjFileEditorPanel::OnChangeSlotsSlot(wxCommandEvent &e)
{
	if (!m_locatorNameCombo ||!m_slotNameCombo||!m_objNameCombo|| !GetDataManipulator())
	{
		return;
	}

	Fairy::LogicModel* pObj = GetDataManipulator()->m_pObjTemplate;
	if (!pObj)
	{
		return;
	}

	Ogre::String  locatorName = m_locatorNameCombo->GetStringSelection().c_str();
	if(locatorName.empty())
	{
		locatorName = m_locatorNameCombo->GetValue().c_str();
	}
	if (locatorName.empty() 
		|| locatorName.compare(Fairy::NEWLOCATORTIP)== 0 
		|| locatorName.compare(Fairy::DELLOCATORTIP) == 0)
	{
		return;
	}

	Ogre::String  slotName = m_slotNameCombo->GetStringSelection().c_str();
	if(slotName.empty())
	{
		slotName = m_slotNameCombo->GetValue().c_str();
	}

	Ogre::String  objectName = m_objNameCombo->GetStringSelection().c_str();
	if(objectName.empty())
	{
		objectName = m_objNameCombo->GetValue().c_str();
	}

	Ogre::String locatorUse = GetDataManipulator()->isSelSlotUsed(locatorName,slotName);
	if (!locatorUse.empty())
	{
		GetDataManipulator()->UpdateLocatorInfo(locatorName);
		wxMessageDialog dialog( NULL,wxT("确定要自动更新吗?"),
			wxT(wxString::Format("该插槽<%s>已经被挂点<%s>使用了!",slotName.c_str(),locatorUse.c_str())),
			wxNO_DEFAULT|wxYES_NO|wxICON_INFORMATION);
		switch ( dialog.ShowModal() )
		{
		case wxID_NO:
			return;
		case wxID_YES:				
		default:
			{				
				if (GetDataManipulator()->switchSlot(locatorUse, locatorName, slotName))
				{
					wxLogMessage(wxT("成功修改挂点<%s>的插槽为<>,被挂对象为<>!"),locatorUse.c_str());	
					wxLogMessage(wxT("成功修改挂点<%s>的插槽为<%s>,被挂对象为<>!"),locatorName.c_str(),slotName.c_str());	
					GetDataManipulator()->UpdateSlotInfo(slotName, locatorName);
					m_slotNameCombo->SetValue(slotName);
				}	
				else
				{
					wxLogMessage(wxT("操作失败!"));	
				}	
			}	
		}
		return;
	}

	if (GetDataManipulator()->changeLocatorSlot(locatorName,slotName, objectName, ""))
	{
		wxLogMessage(wxT("成功修改挂点<%s>的插槽为<%s>,被挂对象为<%s>!"),locatorName.c_str(),slotName.c_str(), objectName.c_str());	
		GetDataManipulator()->UpdateSlotInfo(slotName, locatorName);
	}	
	else
	{
		wxLogMessage(wxT("操作失败!"));	
	}	
}

void ObjFileEditorPanel::OnChangeSlotsEffect(wxCommandEvent &e)
{
	if(!m_attributeEffectCombo ||!m_locatorNameCombo||!GetDataManipulator())
		return;

	Fairy::LogicModel* pObj = GetDataManipulator()->m_pObjTemplate;

	if(NULL == pObj)
		return;

	Ogre::String  locatorName = m_locatorNameCombo->GetStringSelection().c_str();
	if(locatorName.empty())
	{
		locatorName = m_locatorNameCombo->GetValue().c_str();
	}
	if (locatorName.empty())
	{
		return;
	}

	Ogre::String  effectName = m_attributeEffectCombo->GetStringSelection().c_str();
	if(effectName.empty())
	{
		effectName = m_attributeEffectCombo->GetValue().c_str();
	}

	GetDataManipulator()->changeSlotsEffect(effectName, locatorName);
	
}

void ObjFileEditorPanel::OnBtnSkeletonsAdd(wxCommandEvent &e)
{
	if(!m_skeletonCombo ||!m_resCombo||!GetDataManipulator())
		return;

	Fairy::LogicModel* pObj = GetDataManipulator()->m_pObjTemplate;

	if(NULL == pObj)
		return;

	Ogre::String  skeletonName = m_resCombo->GetStringSelection().c_str();
	if(skeletonName.empty())
	{
		skeletonName = m_resCombo->GetValue().c_str();
	}
	if (skeletonName.empty())
	{
		return;
	}
	// Add the skeleton
	if(GetDataManipulator()->_addSkeleton(skeletonName))
	{
		Fairy::DStringList& skeFileList = GetDataManipulator()->m_skeltonFileList;
		for (Fairy::DStringList::iterator it = skeFileList.begin(); it != skeFileList.end(); ++it)
		{
			if (skeletonName == *it)
			{
				return;
			}
		}

		GetDataManipulator()->setModified(true);
		m_skeletonCombo->AppendString(skeletonName);
		skeFileList.push_back(skeletonName);
		if (GetDataManipulator()->m_skeltonFileList.size() == 1)
		{
			// Check the bone list to show the locator list
			bool showFlag = GetDataManipulator()->_updateBoneList();
			SwitchUI(IFS_SLOTS_SIZER,showFlag);
		}
	}
}

void ObjFileEditorPanel::OnBtnSkeletonsDel(wxCommandEvent &e)
{
	if(!m_skeletonCombo ||!m_resCombo||!GetDataManipulator())
		return;

	Fairy::LogicModel* pObj = GetDataManipulator()->m_pObjTemplate;

	if(NULL == pObj)
		return;

	Ogre::String  skeletonName = m_skeletonCombo->GetStringSelection().c_str();
	if(skeletonName.empty())
	{
		skeletonName = m_skeletonCombo->GetValue().c_str();
	}
	if (skeletonName.empty())
	{
		return;
	}

	// Del the skeleton
	if(GetDataManipulator()->_delSkeleton(skeletonName))
	{
		GetDataManipulator()->setModified(true);
		Fairy::DStringList& skeFileList = GetDataManipulator()->m_skeltonFileList;
		for (Fairy::DStringList::iterator it = skeFileList.begin(); it != skeFileList.end(); ++it)
		{
			if (skeletonName == *it)
			{
				skeFileList.erase(it);
				break;
			}
		}
		bool showFlag = GetDataManipulator()->_updateSkeletonList();
		if (m_skeletonCombo->GetCount() == 0)
		{
			bool showFlag = GetDataManipulator()->_updateBoneList();
			SwitchUI(IFS_SLOTS_SIZER,showFlag);
		}
	}
}

void ObjFileEditorPanel::OnEditAttibuteName(wxCommandEvent &e)
{
	if ( ! m_attributeNameCombo)
	{
		return;
	}

	if (e.GetEventType() == wxEVT_COMMAND_TEXT_ENTER)
	{
		Ogre::String str = m_attributeNameCombo->GetValue().c_str();

		if (str.empty())
		{
			return;
		}

		if ( m_curAttibuteName.compare(str) == 0)
		{
			return;
		}

		int result = GetDataManipulator()->changeAttributeName(m_curAttibuteName, str);

		if ( result >0 )
		{
			int idx = m_attributeNameCombo->FindString(m_curAttibuteName);
			m_attributeNameCombo->SetString(idx, str);
			m_attributeNameCombo->SetValue(str);			

			GetDataManipulator()->updateUIAfterByAttributeName(m_curAttibuteName, str, result);
			wxLogMessage(wxT("成功修改属性<%s>的名称为<%s>!"),m_curAttibuteName.c_str(),str.c_str());
			m_curAttibuteName = str;
		}
		else
		{
			m_attributeNameCombo->SetValue(m_curAttibuteName);	
		}
	}
}

void ObjFileEditorPanel::OnReset(wxCommandEvent &e)
{
	if (!GetDataManipulator())
	{
		return;
	}

	Fairy::LogicModel* pObj = GetDataManipulator()->m_pObjTemplate;
	if (!pObj)
	{
		return;
	}

	Ogre::String  locatorName = m_locatorNameCombo->GetStringSelection().c_str();
	if(locatorName.empty())
	{
		locatorName = m_locatorNameCombo->GetValue().c_str();
	}

	if (locatorName.empty())
	{
		return;
	}

	Ogre::String  boneName = m_boneNameCombo->GetStringSelection().c_str();
	if(boneName.empty())
	{
		boneName = m_boneNameCombo->GetValue().c_str();
	}

	Ogre::String offsetPos = m_posText->GetValue();
	Ogre::Vector3 vPos = Ogre::StringConverter::parseVector3(offsetPos);
	Ogre::String offsetRot = m_rotText->GetValue();	
	Ogre::Quaternion qRot = Ogre::StringConverter::parseQuaternion(offsetRot);
	if (e.GetId() == ID_BTN_RESET_POS)
	{
		vPos = Ogre::Vector3::ZERO;
		offsetPos = Ogre::StringConverter::toString(vPos);
		m_posText->SetValue(offsetPos);
	}
	else if(e.GetId() == ID_BTN_RESET_ROT)
	{
		qRot = Ogre::Quaternion::IDENTITY;
		offsetRot = Ogre::StringConverter::toString(qRot);
		m_rotText->SetValue(offsetRot);
	}	

	if(GetDataManipulator()->ChangeLocatorInfo(locatorName, boneName, vPos, qRot))
	{	
		GetDataManipulator()->_updateAxexInfo();		
		wxLogMessage(wxT("成功修改挂点<%s>的骨头为<%s>,位移为<%s>,朝向为<%s>!"),locatorName.c_str(),boneName.c_str(), offsetPos.c_str(), offsetRot.c_str());
	}
	else
	{
		wxLogMessage(wxT("操作失败!"));
	}
}

void ObjFileEditorPanel::OnEditLocatorName(wxCommandEvent &e)
{
 	if (!m_locatorNameCombo)
 	{
 		return;
 	}
 
 	if (e.GetEventType() == wxEVT_COMMAND_TEXT_ENTER)
 	{
 		Ogre::String str = m_locatorNameCombo->GetValue().c_str();
 
 		if (str.empty())
 		{
 			return;
 		}
 
 		if( m_curLocatorName.compare(str) == 0)
 		{
 			return;
 		}
 
 		if(GetDataManipulator()->changeLocatorName(m_curLocatorName, str))
 		{
 			int idx = m_locatorNameCombo->FindString(m_curLocatorName);
 			m_locatorNameCombo->SetString(idx, str);
 			m_locatorNameCombo->SetValue(str);

			// For effect locator
			int selectIdx = m_effectLocatorCombo->GetSelection();
			idx = m_effectLocatorCombo->FindString(m_curLocatorName);
			m_effectLocatorCombo->SetString(idx,str);
			m_effectLocatorCombo->SetSelection(selectIdx);
 
 			wxLogMessage(wxT("成功修改挂点<%s>的名称为<%s>!"),m_curLocatorName.c_str(),str.c_str());
 
 			m_curLocatorName = str;
 		}
 	}
}

void ObjFileEditorPanel::OnChangeEffectName(wxCommandEvent &e)
{
	if (!m_effectNameCombo || !GetDataManipulator())
	{
		return;
	}

	Fairy::LogicModel* pObj = GetDataManipulator()->m_pObjTemplate;
	if (!pObj)
	{
		return;
	}

	Ogre::String  effectName = m_effectNameCombo->GetStringSelection().c_str();
	if(effectName.empty())
	{
		effectName = m_effectNameCombo->GetValue().c_str();
	}

	if (effectName.empty())
	{
		return;
	}

	Ogre::String selEffect = effectName;
	if (effectName.compare(Fairy::DELEFFECTTIP) == 0)
	{
		if (m_curEffectName.empty())
		{
			return;
		}
		//Show the confirm dialog
		wxMessageDialog dialog( NULL,wxString::Format("确定要删除特效<%s>吗?",m_curEffectName.c_str()),
			wxT("确认!"),
			wxNO_DEFAULT|wxYES_NO|wxICON_INFORMATION);
		switch ( dialog.ShowModal() )
		{
		case wxID_NO:
			return;
		case wxID_YES:				
		default:
			{				
				break;
			}	
		}

		//Delete current effect
		if (GetDataManipulator()->deleteCurEffect())
		{					
			wxLogMessage(wxT("成功删除特效<%s>"),m_curEffectName.c_str());
			int idx = m_effectNameCombo->FindString(m_curEffectName.c_str());
			m_effectNameCombo->Delete(idx);

			m_curEffectName = "";
		}
		else
		{
			wxLogMessage(wxT("操作失败!"));
		}

		m_effectNameCombo->SetValue(m_curEffectName.c_str());
		GetDataManipulator()->UpdateEffectInfo(m_curEffectName);	
		return;
	}

	if (effectName.compare(Fairy::NEWEFFECTTIP) == 0)
	{
		wxMessageDialog dialog( NULL,wxT("确定要新建特效吗?"),
			wxT("确认!"),
			wxNO_DEFAULT|wxYES_NO|wxICON_INFORMATION);
		switch ( dialog.ShowModal() )
		{
		case wxID_NO:
			return;
		case wxID_YES:				
		default:
			{				
				break;
			}	
		}

		selEffect = GetDataManipulator()->_formatValidEffectName();

		if(GetDataManipulator()->addEffectInfo(selEffect))
		{
			m_effectNameCombo->AppendString(selEffect);
			m_effectNameCombo->SetStringSelection(selEffect);
		}
	}

	m_curEffectName = effectName;

	GetDataManipulator()->UpdateEffectInfo(effectName);

}

void ObjFileEditorPanel::OnChangeEffectInfo(wxCommandEvent &e)
{
	Ogre::String  effectName = m_effectNameCombo->GetStringSelection().c_str();
	if(effectName.empty())
	{
		effectName = m_effectNameCombo->GetValue().c_str();
	}
	if (effectName.empty())
	{
		return;
	}

	Ogre::String fileName = m_effectFileCombo->GetStringSelection().c_str();
	if(fileName.empty())
	{
		fileName = m_effectFileCombo->GetValue().c_str();
	}
	if (fileName.empty())
	{
		return;
	}
	
	Ogre::String locatorName = m_effectLocatorCombo->GetStringSelection().c_str();
	if(locatorName.empty())
	{
		locatorName = m_effectLocatorCombo->GetValue().c_str();
	}
	if (locatorName.empty())
	{
		return;
	}

	bool effectTrans = m_effectTransCheck->GetValue();

	Ogre::ColourValue effectCol = Ogre::StringConverter::parseColourValue(m_effectColour->GetValue().c_str());
	
	GetDataManipulator()->changeEffectInfo(effectName, fileName, locatorName, effectTrans, effectCol);
}

void ObjFileEditorPanel::OnEditAttributesValue(wxCommandEvent &e)
{
	if (! m_attributeValueText)
	{
		return;
	}

	wxString currentValue = m_attributeValueText->GetValue();
	if (GetDataManipulator()->ChangeAttributeValue(currentValue.c_str()))
	{
		wxLogMessage(wxT("成功修改属性<%s>的值为<%s>"),m_curAttibuteName.c_str(),currentValue);
	}
	else
	{
		wxLogMessage(wxT("操作失败"));		
	}
	GetDataManipulator()->UpdateAttributeInfo(m_curAttibuteName);
}

void ObjFileEditorPanel::OnEditEffectName(wxCommandEvent &e)
{
	if ( ! m_effectNameCombo)
	{
		return;
	}

	if (e.GetEventType() == wxEVT_COMMAND_TEXT_ENTER)
	{
		Ogre::String str = m_effectNameCombo->GetValue().c_str();

		if (str.empty())
		{
			m_effectNameCombo->SetValue(m_curEffectName);	
			return;
		}

		if ( m_curEffectName.compare(str) == 0)
		{
			m_effectNameCombo->SetValue(m_curEffectName);	
			return;
		}

		if ( GetDataManipulator()->changeEffectName(m_curEffectName, str) )
		{
			int idx = m_effectNameCombo->FindString(m_curEffectName);
			m_effectNameCombo->SetString(idx, str);
			m_effectNameCombo->SetValue(str);

			wxLogMessage(wxT("成功修改特效<%s>的名称为<%s>!"),m_curEffectName.c_str(),str.c_str());
			m_curEffectName = str;
		}
		else
		{
			m_effectNameCombo->SetValue(m_curEffectName);	
		}
	}
}

void ObjFileEditorPanel::OnSaveDefaultSkeleton(wxCommandEvent & e)
{
	if (m_saveDefaultCheck)
	{
		bool saveDefault = m_saveDefaultCheck->GetValue();
		if (GetDataManipulator())
		{
			GetDataManipulator()->setSaveDefaultSkeleton(saveDefault);
		}
	}

}