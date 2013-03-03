#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "ObjFileEditDlg_wdr.h"
#endif

// For compilers that support precompilation
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// Include private header
#include "ObjFileEditDlg_wdr.h"

#include <wx/intl.h>

// Euro sign hack of the year
#if wxUSE_UNICODE
#define __WDR_EURO__ wxT("\u20ac")
#else
#if defined(__WXMAC__)
#define __WDR_EURO__ wxT("\xdb")
#elif defined(__WXMSW__)
#define __WDR_EURO__ wxT("\x80")
#else
#define __WDR_EURO__ wxT("\xa4")
#endif
#endif

// Implement window functions
wxSizer *ObjFileEditDlg( wxWindow *parent, bool call_fit, bool set_sizer )
{
	wxBoxSizer *item0 = new wxBoxSizer( wxVERTICAL );

	wxGridSizer *itemL1 = new wxGridSizer( 4, 0, 0 );

	wxGridSizer *itemL2 = new wxGridSizer( 0, 1, 0 );

	wxGridSizer *itemL3 = new wxGridSizer( 0, 1, 0 );

	wxGridSizer *itemL4 = new wxGridSizer( 4, 0, 0 );

	wxGridSizer *itemL5 = new wxGridSizer( 0, 1, 0 );

	wxGridSizer *itemL6 = new wxGridSizer( 4, 0, 0 );

	wxGridSizer *itemL7 = new wxGridSizer( 0, 1, 0 );

	wxGridSizer *itemL8 = new wxGridSizer( 6, 0, 0 );

	wxGridSizer *itemL9 = new wxGridSizer( 4, 0, 0 );

	wxGridSizer *itemL10 = new wxGridSizer( 0, 1, 0 );

	wxGridSizer *itemL11 = new wxGridSizer( 3, 0, 0 );

	wxGridSizer *itemL12 = new wxGridSizer( 2, 0, 0 );

	wxGridSizer *itemL13 = new wxGridSizer( 2, 0, 0 );

	wxGridSizer *itemL14 = new wxGridSizer(2,0,0);

    wxBoxSizer *statsizer1 = new wxStaticBoxSizer(
        new wxStaticBox(parent, wxID_ANY, _T("选择obj模板")), wxHORIZONTAL );

    wxBoxSizer *statsizer2 = new wxStaticBoxSizer(
        new wxStaticBox(parent, ID_STATIC_ATTRIBUTE, _T("attribute")), wxHORIZONTAL );

    wxBoxSizer *statsizer3 = new wxStaticBoxSizer(
        new wxStaticBox(parent, ID_STATIC_ATTACHMODEL, _T("attachmodel")), wxHORIZONTAL );

    wxBoxSizer *statsizer4 = new wxStaticBoxSizer(
        new wxStaticBox(parent, ID_STATIC_ENTITY, _T("entitylist")), wxHORIZONTAL );

    wxBoxSizer *statsizer5 = new wxStaticBoxSizer(
        new wxStaticBox(parent, ID_STATIC_MATERIAL, _T("materiallist")), wxHORIZONTAL );

    wxBoxSizer *statsizer6 = new wxStaticBoxSizer(
        new wxStaticBox(parent, ID_STATIC_LOCATOR, _T("locatorlist")), wxHORIZONTAL );

    wxBoxSizer *statsizer7 = new wxStaticBoxSizer(
        new wxStaticBox(parent, ID_STATIC_POSANDORI, _T("locator pos*ori")), wxHORIZONTAL );

    wxBoxSizer *statsizer8 = new wxStaticBoxSizer(
        new wxStaticBox(parent, ID_STATIC_SKELETON, _T("skeleton")), wxHORIZONTAL );

	wxString *strsnew = (wxString*) NULL;
	wxComboBox *itemCb = new wxComboBox( parent, ID_COMBO_NEWFILE, wxT(""), wxDefaultPosition, wxDefaultSize, 0, strsnew, wxCB_DROPDOWN );
	statsizer1->Add( itemCb, 0, wxALIGN_LEFT|wxALL, 5 );

	item0->Add( statsizer1, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	////Attribute

	item0->Add( itemL2, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxStaticText *item4 = new wxStaticText( parent, ID_TEXT_ATTRIBUTE_NAME, _("name"), wxDefaultPosition, wxDefaultSize, 0 );
	statsizer2->Add( item4, 0, wxALIGN_CENTER|wxALL, 5 );

	wxString *strs6 = (wxString*) NULL;
	wxComboBox *item5 = new wxComboBox( parent, ID_COMBO_PROPERTYNAME, wxT(""), wxDefaultPosition, wxDefaultSize, 0, strs6, wxCB_DROPDOWN );
	statsizer2->Add( item5, 0, wxALIGN_CENTER|wxALL, 5 );

	wxStaticText *item8 = new wxStaticText( parent, ID_TEXT_ATTRIBUTE_HINTS, _("hints"), wxDefaultPosition, wxDefaultSize, 0 );
	statsizer2->Add( item8, 0, wxALIGN_CENTER|wxALL, 5 );

	wxString *strs0 = (wxString*) NULL;
	wxComboBox *item9 = new wxComboBox( parent, ID_COMBO_PROPERHINTS, wxT(""), wxDefaultPosition, wxDefaultSize, 0, strs0, wxCB_DROPDOWN );
	statsizer2->Add( item9, 0, wxALIGN_CENTER|wxALL, 5 );

	item0->Add( statsizer2, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	////////Entitylist

	wxStaticText *item101 = new wxStaticText( parent, ID_TEXT_ENTITY_NAME, _("name"), wxDefaultPosition, wxDefaultSize, 0 );
	statsizer4->Add( item101, 0, wxALIGN_CENTER|wxALL, 5 );

	wxString *strs7 = (wxString*) NULL;
	wxComboBox *item11 = new wxComboBox( parent, ID_COMBO_ENTITYNAME, wxT(""), wxDefaultPosition, wxDefaultSize, 0, strs7, wxCB_DROPDOWN );
	statsizer4->Add( item11, 0, wxALIGN_CENTER|wxALL, 5 );

	wxStaticText *item12 = new wxStaticText( parent, ID_TEXT_ENTITY_FILE, _("file"), wxDefaultPosition, wxDefaultSize, 0 );
	statsizer4->Add( item12, 0, wxALIGN_CENTER|wxALL, 5 );

	wxTextCtrl *item13 = new wxTextCtrl( parent, ID_TEXTCTRL_ENTITYFILE, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	statsizer4->Add( item13, 0, wxALIGN_CENTER|wxALL, 5 );

	wxStaticText *item14 = new wxStaticText( parent, ID_TEXT_ENTITY_MATERIAL, _("material"), wxDefaultPosition, wxDefaultSize, 0 );
	statsizer4->Add( item14 ,0, wxALIGN_CENTER|wxALL, 5 );

	wxString *strs7x = (wxString*) NULL;
	wxComboBox *item11x = new wxComboBox( parent, ID_COMBO_ENTITYMATERIAL, wxT(""), wxDefaultPosition, wxDefaultSize, 0, strs7x, wxCB_DROPDOWN );
	statsizer4->Add( item11x, 0, wxALIGN_CENTER|wxALL, 5 );

	item0->Add( statsizer4, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	//////Materiallist
	wxStaticText *item16x = new wxStaticText( parent, ID_TEXT_MATERIAL_NAME, _("name"), wxDefaultPosition, wxDefaultSize, 0 );
	statsizer5->Add( item16x, 0, wxALIGN_CENTER|wxALL, 5 );

	wxString *strs8 = (wxString*) NULL;
	wxComboBox *item17 = new wxComboBox( parent, ID_COMBO_MATERIALNAME, wxT(""), wxDefaultPosition, wxDefaultSize, 0, strs8, wxCB_DROPDOWN );
	statsizer5->Add( item17, 0, wxALIGN_CENTER|wxALL, 5 );

	wxStaticText *item18 = new wxStaticText( parent, ID_TEXT_MATERIAL_ENTITY, _("entity"), wxDefaultPosition, wxDefaultSize, 0 );
	statsizer5->Add( item18, 0, wxALIGN_CENTER|wxALL, 5 );

	wxTextCtrl *item19 = new wxTextCtrl( parent, ID_TEXTCTRL_MATERIALENTITY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	statsizer5->Add( item19, 0, wxALIGN_CENTER|wxALL, 5 );

	wxStaticText *item20 = new wxStaticText( parent, ID_TEXT_MATERIAL_MATERIAL, _("material"), wxDefaultPosition, wxDefaultSize, 0 );
	statsizer5->Add( item20 ,0, wxALIGN_CENTER|wxALL, 5 );

	wxString *strs8a = (wxString*) NULL;
	wxComboBox *item21 = new wxComboBox( parent, ID_COMBO_MATERIALMATERIAL, wxT(""), wxDefaultPosition, wxDefaultSize, 0, strs8a, wxCB_DROPDOWN );
	statsizer5->Add( item21, 0, wxALIGN_CENTER|wxALL, 5 );

	item0->Add( statsizer5, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	////locators
	wxStaticText *item22 = new wxStaticText( parent, ID_TEXT_LOCATOR_NAME, _("locatorname"), wxDefaultPosition, wxDefaultSize, 0 );
	statsizer6->Add( item22, 0, wxALIGN_CENTER|wxALL, 5 );

	wxString *strs1 = (wxString*) NULL;
	wxComboBox *item23 = new wxComboBox( parent, ID_COMBO_LOCATOR_NAME, wxT(""), wxDefaultPosition-wxPoint(0,5), wxDefaultSize, 0, strs1, wxCB_DROPDOWN );
	statsizer6->Add( item23, 0, wxALIGN_CENTER|wxALL, 5 );

	wxStaticText *item26b = new wxStaticText( parent, ID_TEXT_LOCATOR_BONE, _("bone"), wxDefaultPosition, wxDefaultSize, 0 );
	statsizer6->Add( item26b, 0, wxALIGN_CENTER|wxALL, 5 );

	wxString *strs10b = (wxString*) NULL;
	wxComboBox *item27b = new wxComboBox( parent, ID_COMOB_BONE, wxT(""), wxDefaultPosition-wxPoint(0,5), wxDefaultSize, 0, strs10b, wxCB_DROPDOWN );
	statsizer6->Add( item27b, 0, wxALIGN_CENTER|wxALL, 5 );

	//effect
	wxStaticText *item30 = new wxStaticText( parent, ID_TEXT_LOCATOR_EFFECT, _("effect"), wxDefaultPosition, wxDefaultSize, 0 );
	statsizer6->Add( item30, 0, wxALIGN_CENTER|wxALL, 5 );

	wxString *strs5 = (wxString*) NULL;
	wxComboBox *item31 = new wxComboBox( parent, ID_COMBO_EFFECT, wxT(""), wxDefaultPosition, wxDefaultSize, 0, strs5, wxCB_DROPDOWN );
	statsizer6->Add( item31, 0, wxALIGN_CENTER|wxALL, 5 );

	item0->Add( statsizer6, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxStaticText *item26 = new wxStaticText( parent, ID_TEXT_ATTRIBUTE_SLOT, _("slotname"), wxDefaultPosition, wxDefaultSize, 0 );
	statsizer3->Add( item26, 0, wxALIGN_CENTER|wxALL, 5 );

	wxString *strs10 = (wxString*) NULL;
	wxComboBox *item27 = new wxComboBox( parent, ID_COMBO_SLOTNAME, wxT(""), wxDefaultPosition-wxPoint(0,5), wxDefaultSize, 0, strs10, wxCB_DROPDOWN );
	statsizer3->Add( item27, 0, wxALIGN_CENTER|wxALL, 5 );

	//attachmodel
	wxStaticText *item24 = new wxStaticText( parent, ID_TEXT_ATTRIBUTE_ATTACH, _("attached model"), wxDefaultPosition, wxDefaultSize, 0 );
	statsizer3->Add( item24, 0, wxALIGN_CENTER|wxALL, 5 );

	wxString *strs4 = (wxString*) NULL;
	wxComboBox *item25 = new wxComboBox( parent, ID_COMBO_ATTACTHMODEL, wxT(""), wxDefaultPosition-wxPoint(0,5), wxDefaultSize, 0, strs4, wxCB_DROPDOWN );
	statsizer3->Add( item25, 0, wxALIGN_CENTER|wxALL, 5 );

	item0->Add( statsizer3, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxStaticText *item28 = new wxStaticText( parent, ID_TEXT_LOCATOR_POS, _("位置"), wxDefaultPosition, wxDefaultSize, 0 );
	statsizer7->Add( item28, 0, wxALIGN_CENTER|wxALL, 5 );

	wxTextCtrl *item29 = new wxTextCtrl( parent, ID_TEXTCTRL_LOCATOR_POSTION, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	statsizer7->Add( item29, 0, wxALIGN_CENTER|wxALL, 5 );

	wxStaticText *item42 = new wxStaticText( parent, ID_TEXT_LOCATOR_ORI, _("方向"), wxDefaultPosition, wxDefaultSize, 0 );
	statsizer7->Add( item42, 0, wxALIGN_CENTER|wxALL, 5 );

	wxTextCtrl *item43 = new wxTextCtrl( parent, ID_TEXTCTRL_LOCATOR_ORIENTATION, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	statsizer7->Add( item43, 0, wxALIGN_CENTER|wxALL, 5 );

	item0->Add( statsizer7, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	//skeleton
	wxStaticText *item32 = new wxStaticText( parent, ID_TEXT_SKELETON_NAME, _("角色骨骼"), wxDefaultPosition, wxDefaultSize, 0 );
	statsizer8->Add( item32, 0, wxALIGN_CENTER|wxALL, 5 );

	wxString *strs2 = (wxString*) NULL;
	wxComboBox *item33 = new wxComboBox( parent, ID_COMBO_SKELETON_NAME, wxT(""), wxDefaultPosition, wxDefaultSize, 0, strs2, wxCB_DROPDOWN );
	statsizer8->Add( item33, 0, wxALIGN_CENTER|wxALL, 5 );

	wxButton* item41 = new wxButton( parent, ID_BUTTON_SKELETON_DEL, wxT("Skeleton删除"), wxDefaultPosition, wxDefaultSize, 0 );
    statsizer8->Add( item41, 0, wxALIGN_LEFT|wxALL, 5 );

	wxButton* item421 = new wxButton( parent, ID_BUTTON_SKELETON_ADD, wxT("Skeleton添加"), wxDefaultPosition, wxDefaultSize, 0 );
    statsizer8->Add( item421, 0, wxALIGN_LEFT|wxALL, 5 );

	wxStaticText *item34 = new wxStaticText( parent, ID_TEXT_SKELETON_TABLE, _("资源表"), wxDefaultPosition, wxDefaultSize, 0 );
	statsizer8->Add( item34, 0, wxALIGN_CENTER|wxALL, 5 );

	wxString *strs3 = (wxString*) NULL;
	wxComboBox *item35 = new wxComboBox( parent, ID_COMBO_SKELETON_TABLE, wxT(""), wxDefaultPosition, wxDefaultSize, 0, strs3, wxCB_DROPDOWN );
	statsizer8->Add( item35, 0, wxALIGN_CENTER|wxALL, 5 );

	item0->Add( statsizer8, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	if (set_sizer)
	{
		parent->SetSizer( item0 );
		if (call_fit)
			item0->SetSizeHints( parent );
	}

	return item0;
}

// Implement menubar functions

// Implement toolbar functions

// Implement bitmap functions


// End of generated file
