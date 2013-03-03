#ifndef __ObjectArrayCreatorDialog_H__
#define __ObjectArrayCreatorDialog_H__

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/combobox.h>
#include <wx/dialog.h>
#include "OgreVector3.h"

class ObjectArrayCreatorDialog : public wxDialog
{
	DECLARE_DYNAMIC_CLASS(ObjectArrayCreatorDialog)

public:
	ObjectArrayCreatorDialog(){}
	ObjectArrayCreatorDialog(const Ogre::Vector3& sizeInfo, wxWindow* parent, 
		wxWindowID id = wxID_ANY, 
		const wxString& title = wxT("创建物体队列"), 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxSize( 300,200 ), 
		long style = wxDEFAULT_DIALOG_STYLE)
		: wxDialog(parent, id, title, pos, size, style)
	{
		m_boxSize = sizeInfo;
	}

public: 
	wxTextCtrl* m_sizeTextCtrl;
	wxTextCtrl* m_countTextCtrl;
	wxComboBox* m_axisCombo;
	Ogre::Vector3 m_boxSize;


protected:
	DECLARE_EVENT_TABLE()
	void OnInitDialog(wxInitDialogEvent &e);
	void OnComboBox(wxCommandEvent& e);
};

#endif // __ObjectArrayCreatorDialog_H__
