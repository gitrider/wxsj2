//------------------------------------------------------------------------------
// Source code generated by wxDesigner from file: ChangeMonsgerInfo.wdr
// Do not modify this file, all changes will be lost!
//------------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "ChangeMonsgerInfo_wdr.h"
#endif

// For compilers that support precompilation
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// Include private header
#include "ChangeMonsgerInfo_wdr.h"


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

wxSizer *ChangeMonsterInfo( wxWindow *parent, bool call_fit, bool set_sizer )
{
    wxBoxSizer *item0 = new wxBoxSizer( wxVERTICAL );

    wxGridSizer *item1 = new wxGridSizer( 2, 0, 0 );

    wxStaticText *item2 = new wxStaticText( parent, ID_TEXT1, wxT("Refresh_Time"), wxDefaultPosition, wxDefaultSize, 0 );
    item1->Add( item2, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxTextCtrl *item3 = new wxTextCtrl( parent, ID_TEXTCTRL_REFRESH_TIME1, wxT(""), wxDefaultPosition, wxSize(80,-1), 0 );
    item1->Add( item3, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxStaticText *item4 = new wxStaticText( parent, ID_TEXT1, wxT("Area_Radius"), wxDefaultPosition, wxDefaultSize, 0 );
    item1->Add( item4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxTextCtrl *item5 = new wxTextCtrl( parent, ID_TEXTCTRL_AREA_RADIUS1, wxT(""), wxDefaultPosition, wxSize(80,-1), 0 );
    item1->Add( item5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    item0->Add( item1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxGridSizer *item6 = new wxGridSizer( 2, 0, 0 );

    wxButton *item7 = new wxButton( parent, ID_BUTTON_CHANGE_TIME, wxT("Change_Time"), wxDefaultPosition, wxDefaultSize, 0 );
    item6->Add( item7, 0, wxADJUST_MINSIZE|wxGROW|wxALL, 5 );

    wxButton *item8 = new wxButton( parent, ID_BUTTON_CHANGE_RADIUS, wxT("Change_Radius"), wxDefaultPosition, wxDefaultSize, 0 );
    item6->Add( item8, 0, wxGROW|wxALL, 5 );

    wxButton *item9 = new wxButton( parent, ID_BUTTON_CHANGE_BOTH, wxT("Change_Both"), wxDefaultPosition, wxDefaultSize, 0 );
    item6->Add( item9, 0, wxGROW|wxALL, 5 );

    wxButton *item10 = new wxButton( parent, ID_BUTTON_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    item6->Add( item10, 0, wxGROW|wxALL, 5 );

    item0->Add( item6, 0, wxALIGN_CENTER|wxALL, 5 );

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
