#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "ModelPropertyEditDlg_wdr.h"
#endif

// For compilers that support precompilation
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// Include private header
#include "ModelBoneInfoDlg_wdr.h"

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

wxSizer *ModelBoneInfoDlg( wxWindow *parent, bool call_fit, bool set_sizer )
{
    wxBoxSizer *item0 = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer *item1 = new wxBoxSizer( wxVERTICAL );

    wxStaticText *item6 = new wxStaticText( parent, ID_TEXT, _("骨头明细"), wxDefaultPosition, wxDefaultSize, 0 );
    item1->Add( item6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxString *strs2 = (wxString*) NULL;
    wxListBox *item2 = new wxListBox( parent, ID_LISTBOX_ModelBoneInfoDlg, wxDefaultPosition, wxSize(250,500), 0, strs2, wxLB_SINGLE );
    item1->Add( item2, 0, wxALIGN_CENTER|wxALL, 5 );

    item0->Add( item1, 0, wxALIGN_CENTER|wxALL, 5 );

    wxGridSizer *item3 = new wxGridSizer( 2, 0, 0 );

    wxStaticText *item4 = new wxStaticText( parent, ID_TEXT, _("骨头数量"), wxDefaultPosition, wxDefaultSize, 0 );
    item3->Add( item4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxTextCtrl *item5 = new wxTextCtrl( parent, ID_TEXTCTRL_BONENUM, wxT(""), wxDefaultPosition, wxSize(130,-1), wxTE_PROCESS_ENTER );
    item3->Add( item5, 0, wxALIGN_CENTER|wxALL, 5 );

    wxStaticText *item7 = new wxStaticText( parent, ID_TEXT, _("绑定点数量"), wxDefaultPosition, wxDefaultSize, 0 );
    item3->Add( item7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxTextCtrl *item8 = new wxTextCtrl( parent, ID_TEXTCTRL_LOCATORNUM, wxT(""), wxDefaultPosition, wxSize(130,-1), wxTE_PROCESS_ENTER );
    item3->Add( item8, 0, wxALIGN_CENTER|wxALL, 5 );

    item0->Add( item3, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

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
