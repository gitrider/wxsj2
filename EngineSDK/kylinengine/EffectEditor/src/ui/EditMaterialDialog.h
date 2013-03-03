#ifndef __EditMaterial_Dialog_h__
#define __EditMaterial_Dialog_h__
#include <wx/dialog.h>
#include <wx/textctrl.h>

class EditMaterialDialog : public wxDialog
{
	DECLARE_DYNAMIC_CLASS(EditMaterialDialog)

public:
	EditMaterialDialog() {}

	// full ctor
	EditMaterialDialog(wxWindow *parent, wxWindowID id,
		const wxString& title,
		wxString text,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxSize( 289,108 ),
		long style = wxDEFAULT_DIALOG_STYLE)
		: mTextCtrl(NULL),mText(text),
		wxDialog(parent, id, title, pos, size, style)
	{
	}

public:

	wxTextCtrl *mTextCtrl;
	wxString  mText;
protected:
	DECLARE_EVENT_TABLE()
	void OnInitDialog(wxInitDialogEvent &e);

};
#endif