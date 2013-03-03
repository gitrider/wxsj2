#include "EditMaterialDialog.h"
#include <wx/sizer.h>
#include <wx/stattext.h>

IMPLEMENT_DYNAMIC_CLASS(EditMaterialDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(EditMaterialDialog, wxDialog)
EVT_INIT_DIALOG(EditMaterialDialog::OnInitDialog)
END_EVENT_TABLE()

void
EditMaterialDialog::OnInitDialog(wxInitDialogEvent &e)
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer4->SetFlexibleDirection( wxBOTH );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );


	wxStaticText* staticText2 = new wxStaticText( this, wxID_ANY, wxT("²ÄÖÊÃû³Æ:"), wxDefaultPosition, wxDefaultSize, 0 );

	staticText2->SetMinSize( wxSize( 60,-1 ) );

	fgSizer4->Add( staticText2, 0, wxALL, 5 );

	mTextCtrl = new wxTextCtrl( this, wxID_ANY,mText, wxDefaultPosition, wxSize( 200,-1 ), 0 );
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