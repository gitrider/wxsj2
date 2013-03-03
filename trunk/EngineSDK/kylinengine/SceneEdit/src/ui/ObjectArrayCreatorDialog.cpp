// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------
#include "ObjectArrayCreatorDialog.h"
#include <OgreStringConverter.h>

const long int feID_AXIS_COMBOBOX = wxNewId();

IMPLEMENT_DYNAMIC_CLASS(ObjectArrayCreatorDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(ObjectArrayCreatorDialog, wxDialog)
EVT_INIT_DIALOG(ObjectArrayCreatorDialog::OnInitDialog)
EVT_COMBOBOX(feID_AXIS_COMBOBOX, ObjectArrayCreatorDialog::OnComboBox)
END_EVENT_TABLE()

void
ObjectArrayCreatorDialog::OnInitDialog(wxInitDialogEvent &e)
{
	wxDialog::OnInitDialog(e);
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* baseSizer;
	baseSizer = new wxBoxSizer( wxVERTICAL );

	// axis
	wxBoxSizer* axisSizer;
	axisSizer = new wxBoxSizer( wxHORIZONTAL );

	wxStaticText * staticText3 = new wxStaticText( this, wxID_ANY, wxT("朝向"), wxDefaultPosition, wxDefaultSize, 0 );
	axisSizer->Add( staticText3, 0, wxALIGN_CENTER|wxALL, 5 );

	m_axisCombo = new wxComboBox( this, feID_AXIS_COMBOBOX, wxT(""), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN |wxCB_READONLY);
	m_axisCombo->AppendString(wxT("长"));
	m_axisCombo->AppendString(wxT("高"));
	m_axisCombo->AppendString(wxT("宽"));
	m_axisCombo->SetValue(wxT("长"));
	axisSizer->Add( m_axisCombo, 0, wxALIGN_CENTER|wxALL, 5 );

	baseSizer->Add( axisSizer, 1, wxEXPAND, 5 );	

	wxBoxSizer* numSizer;
	numSizer = new wxBoxSizer( wxHORIZONTAL );
	
	// num
	wxBoxSizer* countSizer;
	countSizer = new wxBoxSizer( wxHORIZONTAL );

	wxStaticText * staticText2 = new wxStaticText( this, wxID_ANY, wxT("数量"), wxDefaultPosition, wxDefaultSize, 0 );
	countSizer->Add( staticText2, 0, wxALIGN_CENTER|wxALL, 5 );

	m_countTextCtrl = new wxTextCtrl( this, wxID_ANY, wxT("1"), wxDefaultPosition, wxDefaultSize, 0 );
	countSizer->Add( m_countTextCtrl, 1, wxALIGN_CENTER|wxALL, 5 );

	numSizer->Add( countSizer, 0, wxEXPAND, 5 );

	// size
	wxBoxSizer* sizeSizer;
	sizeSizer = new wxBoxSizer( wxHORIZONTAL );

	wxStaticText * staticText = new wxStaticText( this, wxID_ANY, wxT("物体尺寸"), wxDefaultPosition, wxDefaultSize, 0 );
	sizeSizer->Add( staticText, 0, wxALIGN_CENTER|wxALL, 5 );

	wxString currentSize = Ogre::StringConverter::toString(m_boxSize.x).c_str();

	m_sizeTextCtrl = new wxTextCtrl( this, wxID_ANY, currentSize, wxDefaultPosition, wxDefaultSize, 0 );
	sizeSizer->Add( m_sizeTextCtrl, 0, wxALIGN_CENTER|wxALL, 5 );

	numSizer->Add( sizeSizer, 0, wxEXPAND, 5 );	

	baseSizer->Add( numSizer, 0, wxEXPAND, 5 );

	wxStdDialogButtonSizer* m_sdbSizer = new wxStdDialogButtonSizer();
	wxButton* m_sdbSizer5OK = new wxButton( this, wxID_OK );
	m_sdbSizer->AddButton( m_sdbSizer5OK );
	wxButton* m_sdbSizer5Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer->AddButton( m_sdbSizer5Cancel );
	m_sdbSizer->Realize();
	baseSizer->Add( m_sdbSizer, 1, wxEXPAND||wxALL, 5 );

	this->SetSizer( baseSizer );
	this->Layout();
}

void ObjectArrayCreatorDialog::OnComboBox(wxCommandEvent& e)
{
	int selId = m_axisCombo->GetSelection();
	wxString currentSize = wxEmptyString;
	if (selId == 1)
	{
		currentSize = Ogre::StringConverter::toString(m_boxSize.y).c_str();		
	}
	else if (selId == 2)
	{
		currentSize = Ogre::StringConverter::toString(m_boxSize.z).c_str();
	}
	else
	{
		currentSize = Ogre::StringConverter::toString(m_boxSize.x).c_str();
	}
	
	m_sizeTextCtrl->SetValue(currentSize);
}