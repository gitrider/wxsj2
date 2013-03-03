//说明：obj文件编辑界面
#ifndef __ObjFileEditDialog_H__
#define __ObjFileEditDialog_H__

#include <wx/panel.h>
#include <wx/textctrl.h>
#include <wx/combobox.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/statbox.h>

#include "WXSceneListener.h"

#include <OgreFrameListener.h>

namespace Fairy
{
    class SceneManipulator;  
}

class wxTextCtrl;
class wxComboBox;

class ObjFileEditDialog : public wxPanel, Fairy::SceneListener, Ogre::FrameListener
{
public:

    ObjFileEditDialog(wxWindow *parent,
        wxWindowID winid = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL | wxNO_BORDER,
        const wxString& name = "ObjFileEditDialog")
        : m_pSceneManipulator(NULL)
		, m_pAttributeNameComboBox(NULL)
		, m_pAttributeValueTextCtrl(NULL)
		, m_pAttributeHintsComboBox(NULL)
    {
        Create(parent, winid, pos, size, style, name);
    }

    ~ObjFileEditDialog(void);

    bool Create(wxWindow *parent,
        wxWindowID winid = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL | wxNO_BORDER,
        const wxString& name = "ObjFileEditDialog");

	virtual bool frameStarted(const Ogre::FrameEvent& evt);

	void SetSceneManipulator(Fairy::SceneManipulator* sceneManipulator){
			m_pSceneManipulator = sceneManipulator;
	}

//消息函数
	DECLARE_EVENT_TABLE()

	void OnAddObjectName(wxCommandEvent &e);								//添加objectname

	void OnAddAttribute(wxCommandEvent &e);									//添加attributename

	void OnAddEntitylist(wxCommandEvent &e);								//添加entitylist

	void OnAddMateriallist(wxCommandEvent &e);								//添加materiallist

	void OnAddLocator(wxCommandEvent &e);									//添加locator

	void OnDelLocator(wxCommandEvent &e);									//删除locator
//================================================================================================
	void OnChangeAttributeHints(wxCommandEvent &e);

	void OnChangeLocator(wxCommandEvent &e);

	void OnChangeSlot(wxCommandEvent &e);									//改变slot

	void OnChangeEffect(wxCommandEvent &e);									//改变effect

	void OnChangeMaterial(wxCommandEvent &e);								//改变材质

	void OnSelObjTemplate(wxCommandEvent &e);								//选择obj模板

	void OnAddSkeleton(wxCommandEvent &e);									//添加骨骼

	void OnDelSkeleton(wxCommandEvent &e);									//删除骨骼
public:
	void DynamicShowUI(bool attributeshow,bool entityshow,bool matshow,
		 bool locatorshow,bool skeletonshow);
public:
    Fairy::SceneManipulator* m_pSceneManipulator;
//object
	Ogre::String m_ObjectName;			

	wxTextCtrl* m_pObjectNameTextCtrl;
//attribute
	wxStaticBox* AttrituteStaticBox;
	wxStaticBox* AttachModelStaticBox;
	wxStaticText* AttributeNameStaticText;
	wxStaticText* AttributeHintsStaticText;
	wxStaticText* AttributeSlotStaticText;
	wxStaticText* AttributeAttachStaticText;

	wxComboBox* m_pAttributeNameComboBox;

	wxTextCtrl* m_pAttributeValueTextCtrl;

	wxComboBox* m_pAttributeHintsComboBox;

	wxComboBox* m_pSlotNameComboBox;

	wxComboBox* m_pAttachObjNameComboBox;
//entitylist
	wxStaticBox*  EntityStaticBox;
	wxStaticText* EntityNameStaticText;
	wxStaticText* EntityFileStaticText;
	wxStaticText* EntityMaterialStaticText;

	wxComboBox* m_pEntityListNameComboBox;

	wxTextCtrl* m_pEntityListFileTextCtrl;

	wxComboBox* m_pEntityListMaterialComboBox;
//materiallist
	wxStaticBox*  MatStaticBox;
	wxStaticText* MatNameStaticText;
	wxStaticText* MatEntityStaticText;
	wxStaticText* MatMaterialStaticText;

	wxComboBox* m_pMaterialListNameComboBox;

	wxTextCtrl* m_pMaterialListEntityTextCtrl;

	wxComboBox* m_pMaterialListMaterialComboBox;
//locator
	wxStaticBox*  LocatorStaticBox;
	wxStaticBox*  PosAndOriStaticBox;

	wxStaticText* LocatorNameStaticText;
	wxStaticText* LocatorBoneStaticText;
	wxStaticText* LocatorEffectStaticText;
	wxStaticText* LocatorPosStaticText;
	wxStaticText* LocatorOriStaticText;

	wxComboBox* m_pLocatorNameComboBox;

	wxComboBox* m_pBoneComboBox;

	wxTextCtrl* m_pLocatorPosTextCtrl;

	wxTextCtrl* m_pLocatorOriTextCtrl;

	wxComboBox* m_pEffectNameComboBox;
//skeleton
	wxStaticBox*  SkeStaticBox;

	wxStaticText* SkeNameStaticText;
	wxStaticText* SkeTableStaticText;

	wxComboBox* m_pSkeletonName;

	wxComboBox* m_pSkeletonTable;

	wxButton* m_pSkeletonAdd;
	wxButton* m_pSkeletonDel;
//new file
	wxComboBox* m_pObjFileTable;
};


#endif // __ObjFileEditDialog_H__
