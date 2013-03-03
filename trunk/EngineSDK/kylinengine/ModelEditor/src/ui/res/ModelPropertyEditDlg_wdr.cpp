#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "ModelPropertyEditDlg_wdr.h"
#endif

// For compilers that support precompilation
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// Include private header
#include "ModelPropertyEditDlg_wdr.h"

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

wxSizer *ModelPropertyInfoEditDlg( wxWindow *parent, bool call_fit, bool set_sizer )
{
    wxBoxSizer *item0 = new wxBoxSizer( wxVERTICAL );

    wxGridSizer *item2 = new wxGridSizer( 2, 0, 0 );

    wxStaticText *item3 = new wxStaticText( parent, ID_TEXT, _("name"), wxDefaultPosition, wxDefaultSize, 0 );
    item2->Add( item3, 0, wxALIGN_CENTER|wxALL, 5 );

    wxTextCtrl *item4 = new wxTextCtrl( parent, ID_TEXTCTRL_PROPERTYNAME, wxT(""), wxDefaultPosition, wxSize(130,-1), wxTE_PROCESS_ENTER );
    item2->Add( item4, 0, wxALIGN_CENTER|wxALL, 5 );

    wxStaticText *item5 = new wxStaticText( parent, ID_TEXT, _("value"), wxDefaultPosition, wxDefaultSize, 0 );
    item2->Add( item5, 0, wxALIGN_CENTER|wxALL, 5 );

    wxTextCtrl *item6 = new wxTextCtrl( parent, ID_TEXTCTRL_PROPERVALUE, wxT(""), wxDefaultPosition, wxSize(130,-1), wxTE_PROCESS_ENTER );
    item2->Add( item6, 0, wxALIGN_CENTER|wxALL, 5 );

    wxStaticText *item7 = new wxStaticText( parent, ID_TEXT, _("hints"), wxDefaultPosition, wxDefaultSize, 0 );
    item2->Add( item7, 0, wxALIGN_CENTER|wxALL, 5 );

    wxTextCtrl *item8 = new wxTextCtrl( parent, ID_TEXTCTRL_PROPERHINTS, wxT(""), wxDefaultPosition, wxSize(130,-1), wxTE_PROCESS_ENTER );
    item2->Add( item8, 0, wxALIGN_CENTER|wxALL, 5 );

	wxStaticText *item9 = new wxStaticText( parent, ID_TEXT, _("hints输入规则:\n与蒙皮相关输入M,\n与动画,特效相关输入A,\n与绑点物件相关输入L,\n与材质相关输入T"),\
		wxDefaultPosition, wxDefaultSize, 0 );
    item2->Add( item9, 0, wxALIGN_LEFT|wxALL, 5 );


    item0->Add( item2, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

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
