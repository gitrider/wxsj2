#ifndef __MaterialEditor__
#define __MaterialEditor__

#include <wx/gdicmn.h>
#include <wx/toolbar.h>
#include <wx/combobox.h>
#include <wx/checkbox.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/treectrl.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include "WXSceneListener.h"
#include "SceneManipulator.h"
#include "WXEffectEditorFrame.h"
#include "WXConfig.h"

///////////////////////////////////////////////////////////////////////////

class MaterialTree;
class EffectObjectPropertyEditor;
///////////////////////////////////////////////////////////////////////////////
/// Class MaterialEditor
///////////////////////////////////////////////////////////////////////////////
class NewMaterialDialog : public wxDialog
{
	DECLARE_DYNAMIC_CLASS(NewMaterialDialog)

public:
	NewMaterialDialog() {}

	// full ctor
	NewMaterialDialog(wxWindow *parent, wxWindowID id,
		const wxString& title,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxSize( 289,108 ),
		long style = wxDEFAULT_DIALOG_STYLE)
		: mTextCtrl(NULL),
		wxDialog(parent, id, title, pos, size, style)
	{
	}

public:

	wxTextCtrl *mTextCtrl;

protected:
	DECLARE_EVENT_TABLE()
	void OnInitDialog(wxInitDialogEvent &e);

};

static const int ID_MATERIAL_OBJECT_TREE = 10111;
class MaterialEditor : public wxPanel,Fairy::SceneListener
{
private:

protected:
	wxToolBar* m_toolBar;
	MaterialTree* m_MaterialTree;
	Ogre::String EFFECT_PATH;

public:
	MaterialEditor(){}
	
	MaterialEditor( wxWindow* parent,
		wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style =  wxTAB_TRAVERSAL );
	bool Create(wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style =  wxTAB_TRAVERSAL);
	~MaterialEditor();

	wxToolBar* GetToolBar(){return m_toolBar;}
	//Ñ¡Ôñµ±Ç°±à¼­Ò³
	void OnPageSelect();

	void InitMaterialEditor(Ogre::MaterialPtr& pMaterial,const Ogre::String &templateName);
	void RefreshMaterialTreeItem(const wxString& Desc,const wxString& propertyName,const wxString& propertyValue,void* data);

	void SetSceneManipulator(Fairy::SceneManipulator* sceneManipulator);

	Fairy::SceneManipulator* GetSceneManipulator(void) const
	{
		return mSceneManipulator;
	}

	void SetParentFrame(EffectEditorFrame* frame){m_Frame = frame;}
	EffectEditorFrame* GetParentFrame(){ return m_Frame;}

	enum ToolbarCtrl
	{
		TC_NEW_MATERIAL,
		TC_SAVEAS_MATERIAL,
		TC_SAVE_MATERIAL,
		TC_DELETE_ElEMENT,
		TC_NEW_TECHNIQUE,
		TC_NEW_PASS,
		TC_NEW_TEXTUREUNIT,
	};
	void OnNotifyToolBarCtrl(ToolbarCtrl index);

protected:

	void OnSaveMaterial(wxCommandEvent &e);
	void OnSaveAsMaterial(wxCommandEvent &e);
	void OnDeleteMaterialObject(wxCommandEvent &e);
	void OnNewMaterial(wxCommandEvent &e);
	void OnNewTechnique(wxCommandEvent &e);
	void OnNewPass(wxCommandEvent &e);
	void OnNewTextureUnitState(wxCommandEvent &e);

	Ogre::MaterialPtr mMaterial;

	EffectEditorFrame *m_Frame;

	FairyConfig *mConfig;

	Ogre::String mMaterialName;
	Ogre::String mSaveFileName;
	bool mbRecoverSave;

	Fairy::SceneManipulator *mSceneManipulator;

	//DECLARE_DYNAMIC_CLASS(MaterialEditor)

	DECLARE_EVENT_TABLE()



};

#endif //__EffectObjectEditor__
