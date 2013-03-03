#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "AvatarDialog_wdr.h"
#endif

// For compilers that support precompilation
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// Include private header
#include "AvatarDialog_wdr.h"

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

wxSizer *AvatarDlg( wxWindow *parent, bool call_fit, bool set_sizer )
{
    wxBoxSizer *item0 = new wxBoxSizer( wxVERTICAL );

    wxGridSizer *item1 = new wxGridSizer( 2, 0, 0 );

    wxStaticText *item2 = new wxStaticText( parent, ID_TEXT, _("ͷ"), wxDefaultPosition, wxSize(120,20), 0 );
    item1->Add( item2, 0, wxALIGN_CENTER|wxALL, 5 );

	wxButton* item3 = new wxButton( parent, ID_BUTTON_HEAD, wxT("δָ��"), wxDefaultPosition, wxSize(120,20), 0 );
    item1->Add( item3, 0, wxALIGN_BOTTOM|wxALL, 5 );

    wxStaticText *item4 = new wxStaticText( parent, ID_TEXT, _("��"), wxDefaultPosition, wxSize(120,20), 0 );
    item1->Add( item4, 0, wxALIGN_CENTER|wxALL, 5 );

	wxButton* item5 = new wxButton( parent, ID_BUTTON_FACE, wxT("δָ��"), wxDefaultPosition, wxSize(120,20), 0 );
    item1->Add( item5, 0, wxALIGN_BOTTOM|wxALL, 5 );

	wxStaticText *item6 = new wxStaticText( parent, ID_TEXT, _("��"), wxDefaultPosition, wxSize(120,20), 0 );
    item1->Add( item6, 0, wxALIGN_CENTER|wxALL, 5 );

	wxButton* item7 = new wxButton( parent, ID_BUTTON_SHOULDER, wxT("δָ��"), wxDefaultPosition, wxSize(120,20), 0 );
    item1->Add( item7, 0, wxALIGN_BOTTOM|wxALL, 5 );

	wxStaticText *item8 = new wxStaticText( parent, ID_TEXT, _("����"), wxDefaultPosition, wxSize(120,20), 0 );
    item1->Add( item8, 0, wxALIGN_CENTER|wxALL, 5 );

	wxButton* item9 = new wxButton( parent, ID_BUTTON_BODY, wxT("δָ��"), wxDefaultPosition, wxSize(120,20), 0 );
    item1->Add( item9, 0, wxALIGN_BOTTOM|wxALL, 5 );

	wxStaticText *item10 = new wxStaticText( parent, ID_TEXT, _("��"), wxDefaultPosition, wxSize(120,20), 0 );
    item1->Add( item10, 0, wxALIGN_CENTER|wxALL, 5 );

	wxButton* item11 = new wxButton( parent, ID_BUTTON_HAND, wxT("δָ��"), wxDefaultPosition, wxSize(120,20), 0 );
    item1->Add( item11, 0, wxALIGN_BOTTOM|wxALL, 5 );

	wxStaticText *item12 = new wxStaticText( parent, ID_TEXT, _("��"), wxDefaultPosition, wxSize(120,20), 0 );
    item1->Add( item12, 0, wxALIGN_CENTER|wxALL, 5 );

	wxButton* item13 = new wxButton( parent, ID_BUTTON_FOOT, wxT("δָ��"), wxDefaultPosition, wxSize(120,20), 0 );
    item1->Add( item13, 0, wxALIGN_BOTTOM|wxALL, 5 );

	//wxStaticText *item14 = new wxStaticText( parent, ID_TEXT, _("��������"), wxDefaultPosition, wxSize(120,20), 0 );
 //   item1->Add( item14, 0, wxALIGN_CENTER|wxALL, 5 );

	//wxButton* item15 = new wxButton( parent, ID_BUTTON_LWEAPON, wxT("δָ��"), wxDefaultPosition, wxSize(120,20), 0 );
 //   item1->Add( item15, 0, wxALIGN_BOTTOM|wxALL, 5 );

	//wxStaticText *item16 = new wxStaticText( parent, ID_TEXT, _("��������"), wxDefaultPosition, wxSize(120,20), 0 );
 //   item1->Add( item16, 0, wxALIGN_CENTER|wxALL, 5 );

	//wxButton* item17 = new wxButton( parent, ID_BUTTON_RWEAPON, wxT("δָ��"), wxDefaultPosition, wxSize(120,20), 0 );
 //   item1->Add( item17, 0, wxALIGN_BOTTOM|wxALL, 5 );

	//wxStaticText *item18 = new wxStaticText( parent, ID_TEXT, _("��"), wxDefaultPosition, wxSize(120,20), 0 );
 //   item1->Add( item18, 0, wxALIGN_CENTER|wxALL, 5 );

	//wxButton* item19 = new wxButton( parent, ID_BUTTON_SHIELD, wxT("δָ��"), wxDefaultPosition, wxSize(120,20), 0 );
 //   item1->Add( item19, 0, wxALIGN_BOTTOM|wxALL, 5 );

	//wxStaticText *item20 = new wxStaticText( parent, ID_TEXT, _("����"), wxDefaultPosition, wxSize(120,20), 0 );
 //   item1->Add( item20, 0, wxALIGN_CENTER|wxALL, 5 );

	//wxButton* item21 = new wxButton( parent, ID_BUTTON_RIDER, wxT("δָ��"), wxDefaultPosition, wxSize(120,20), 0 );
 //   item1->Add( item21, 0, wxALIGN_BOTTOM|wxALL, 5 );

    item0->Add( item1, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

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