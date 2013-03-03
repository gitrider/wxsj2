///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __ObjFileEditorPanel__
#define __ObjFileEditorPanel__

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/combobox.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/textctrl.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/button.h>
#include <wx/panel.h>
#include <wx/checkbox.h>

#include "WXSceneListener.h"
#include <OgreFrameListener.h>
#include "Modelsystem\FairyLogicModel.h"

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class ObjFileEditorPanel
///////////////////////////////////////////////////////////////////////////////

namespace Fairy
{
	class SceneManipulator;  
}

class ObjFileEditorPanel : public wxPanel, Fairy::SceneListener, Ogre::FrameListener
{
	public:
		ObjFileEditorPanel( wxWindow* parent, 
		wxWindowID id = wxID_ANY, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxTAB_TRAVERSAL,
		const wxString& name = "ObjFileEditPanel");

		~ObjFileEditorPanel();

		virtual bool frameStarted(const Ogre::FrameEvent& evt);

		void SetSceneManipulator(Fairy::SceneManipulator* sceneManipulator)
		{
			m_pSceneManipulator = sceneManipulator;
		}

	public:
		void SwitchUI( int index = 0, bool bShow = false);		

		// message table
		DECLARE_EVENT_TABLE()

		// For attributes
		void OnChangeAttributesName(wxCommandEvent &e);	
		void OnChangeAttributesHints(wxCommandEvent &e);

		void OnEditAttibuteName(wxCommandEvent &e);

		// For entities
		void OnChangeEntitiesName(wxCommandEvent &e);
		void OnChangeEntitiesFileAndMaterial(wxCommandEvent &e);

		// For materials
		void OnChangeMaterialsName(wxCommandEvent &e);
		void OnChangeMaterialsEntityAndMat(wxCommandEvent &e);
		

		// For slots
		void OnChangeSlotsLocator(wxCommandEvent &e);								
		void OnChangeBoneAndTrans(wxCommandEvent &e);
		void OnChangeSlotsSlot(wxCommandEvent &e);
		void OnChangeSlotsEffect(wxCommandEvent &e);	

		void OnEditLocatorName(wxCommandEvent &e);

		void OnBtnSkeletonsAdd(wxCommandEvent &e);	
		void OnBtnSkeletonsDel(wxCommandEvent &e);	

		void OnChangeEffectName(wxCommandEvent &e);
		void OnChangeEffectInfo(wxCommandEvent &e);
		void OnEditEffectName(wxCommandEvent &e);

		void OnEditAttributesValue(wxCommandEvent & e);

		void OnReset(wxCommandEvent &e);

		void OnSaveDefaultSkeleton(wxCommandEvent & e);

	public:
		
		// Sizer pointers for switchUI
		wxBoxSizer* m_mainBoxSizer;
		wxStaticBoxSizer* m_objFileSizer;
		wxStaticBoxSizer* m_attributeSizer;
		wxStaticBoxSizer* m_entitiesSizer;
		wxStaticBoxSizer* m_materialsSizer;
		wxStaticBoxSizer* m_slotsSizer;
		wxStaticBoxSizer* m_effectsSizer;
		wxStaticBoxSizer* m_skeletonSizer;
		
		// Commons list
		wxStaticText* fileName;
		wxTextCtrl* m_fileNameTxt;

		wxStaticText* attibuteName;
		wxComboBox* m_attributeNameCombo;

		wxStaticText* attributeHints;
		wxComboBox* m_attributeHintsCombo;

		wxStaticText* attributeValue;
		wxTextCtrl* m_attributeValueText;

		wxStaticText* entityName;
		wxComboBox* m_entityNameCombo;

		wxStaticText* entityFile;
		wxComboBox* m_entityFileCombo;

		wxStaticText* entityMaterial;
		wxComboBox* m_entityMaterialCombo;

		wxStaticText* materialName;
		wxComboBox* m_materialNameCombo;

		wxStaticText* materialEntity;
		wxComboBox* m_materialEntityCombo;

		wxStaticText* material;
		wxComboBox* m_materialCombo;

		wxStaticText* locatorName;
		wxComboBox* m_locatorNameCombo;

		wxStaticText* boneName;
		wxComboBox* m_boneNameCombo;

		wxStaticText* slotName;
		wxComboBox* m_slotNameCombo;
		
		wxStaticText* objName;
		wxComboBox* m_objNameCombo;

		wxStaticText* attributeEffect;
		wxComboBox* m_attributeEffectCombo;

		wxStaticText* posInfo;
		wxTextCtrl* m_posText;

		wxStaticText* rotInfo;
		wxTextCtrl* m_rotText;

		wxButton* m_resetPos;
		wxButton* m_resetRot;

		wxStaticText* skeletonName;
		wxComboBox* m_skeletonCombo;
		
		wxButton* m_addBtn;
		wxButton* m_DelBtn;

		wxStaticText* resName;
		wxComboBox* m_resCombo;	

		//For effects
		wxStaticText* m_effectNameLabel;
		wxComboBox* m_effectNameCombo;

		wxStaticText* m_effectFileLabel;
		wxComboBox* m_effectFileCombo;

		wxStaticText* m_effectLocatorLabel;
		wxComboBox* m_effectLocatorCombo;

		wxCheckBox* m_effectTransCheck;

		wxStaticText* m_effectColourLabel;
		wxTextCtrl* m_effectColour;

		wxCheckBox* m_saveDefaultCheck;

		Fairy::SceneManipulator* m_pSceneManipulator;
		Ogre::String m_curAttibuteName;
		Ogre::String m_curLocatorName;
		Ogre::String m_curEffectName;
};

#endif //__ObjFileEditorPanel__
