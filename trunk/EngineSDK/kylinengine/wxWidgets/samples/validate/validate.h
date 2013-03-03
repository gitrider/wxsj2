/////////////////////////////////////////////////////////////////////////////
// Name:        validate.h
// Purpose:     wxWidgets validation sample
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id: validate.h,v 1.9 2004/05/25 11:19:29 JS Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(__APPLE__)
#   pragma interface
#endif

#include "wx/app.h"
#include "wx/combobox.h"
#include "wx/dialog.h"
#include "wx/dynarray.h"
#include "wx/frame.h"
#include "wx/listbox.h"
#include "wx/string.h"

// Define a new application type
class MyApp : public wxApp
{
public:
    bool OnInit();
};

// Define a new frame type
class MyFrame : public wxFrame
{
public:
    MyFrame(wxFrame *frame, const wxString&title, int x, int y, int w, int h);

    void OnQuit(wxCommandEvent& event);
    void OnTestDialog(wxCommandEvent& event);
    void OnToggleBell(wxCommandEvent& event);

private:
    wxListBox *m_listbox;
    bool m_silent;

    DECLARE_EVENT_TABLE()
};

class MyDialog : public wxDialog
{
public:
    MyDialog(wxWindow *parent, const wxString& title,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            const long style = wxDEFAULT_DIALOG_STYLE);
    bool TransferDataToWindow();
    wxTextCtrl *text;
    wxComboBox *combobox;
};

class MyData
{
public:
    MyData();
    // These data members are designed for transfer to and from
    // controls, via validators. For instance, a text control's
    // transferred value is a string:
    wxString m_string;
    // Listboxes may permit multiple selections, so their state
    // is transferred to an integer-array class.
    wxArrayInt m_listbox_choices;
    bool m_checkbox_state;
    // Comboboxes differ from listboxes--validators transfer
    // the string entered in the combobox's text-edit field.
    wxString m_combobox_choice;
    int m_radiobox_choice;
};

#define VALIDATE_DIALOG_ID      200

#define VALIDATE_TEST_DIALOG      2
#define VALIDATE_TOGGLE_BELL      3

#define VALIDATE_TEXT           101
#define VALIDATE_LIST           102
#define VALIDATE_CHECK          103
#define VALIDATE_COMBO          105
#define VALIDATE_RADIO          106

