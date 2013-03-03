#ifndef __BodySelectorDialog_H__
#define __BodySelectorDialog_H__

#include <wx/dialog.h>

class FairyConfig;
class wxListBox;

#include "ModelSystem/FairyLogicModel.h"

#include <OgreString.h>
#include <OgreColourValue.h>

namespace Fairy {
    class Effect;
}
enum E_MODEL_PART
{
	EMP_HEAD    =  0,
	EMP_FACE		,
	EMP_SHOULDER	,
	EMP_BODY		,
	EMP_HAND		,
	EMP_FOOT		,
	EMP_LWEAPON		,
	EMP_RWEAPON		,
	EMP_SHIELD		,
	EMP_RIDER		,
	EMP_NUM			
};
class BodySelectorDialog : public wxDialog
{
    DECLARE_DYNAMIC_CLASS(BodySelectorDialog)
 
public:
	BodySelectorDialog() {}
 
    // full ctor
    BodySelectorDialog(wxWindow *parent, wxWindowID id,
			 const E_MODEL_PART epart,
			 const Ogre::String title,
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize,
             long style = wxDEFAULT_DIALOG_STYLE,
             const wxString& name = wxDialogNameStr)
        : wxDialog(parent, id, title, pos, size, style, name),
		m_ePart(epart),
        m_pBodyList(NULL),
		m_UpdateName(""),
		m_nModel(0)
    {
    }

    ~BodySelectorDialog();

public:
    DECLARE_EVENT_TABLE()
    
	void OnListItemChanged(wxCommandEvent& e);
public:
	void OnInitDialog(/*wxInitDialogEvent &e*/);

	void ReloadRes(Ogre::String updatename);

	void SetUpdateName(Ogre::String updatename){
		m_UpdateName = updatename;
	}

	Ogre::String GetUpdateName(){
		return m_UpdateName;
	}
private:
    E_MODEL_PART m_ePart;
    wxListBox* m_pBodyList;
	Ogre::String m_UpdateName;
	int m_nModel;								//显示模式(0:创建AVATAR模型，1:角色换装)
};

#endif // __BodySelectorDialog_H__
