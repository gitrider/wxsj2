/////////////////////////////////////////////////////////////////////////////
// Program:     wxWidgets Widgets Sample
// Name:        radiobox.cpp
// Purpose:     Part of the widgets sample showing wxRadioBox
// Author:      Vadim Zeitlin
// Created:     15.04.01
// Id:          $Id: radiobox.cpp,v 1.16 2005/02/23 15:50:48 ABX Exp $
// Copyright:   (c) 2001 Vadim Zeitlin
// License:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// for compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_RADIOBOX

// for all others, include the necessary headers
#ifndef WX_PRECOMP
    #include "wx/log.h"

    #include "wx/bitmap.h"
    #include "wx/button.h"
    #include "wx/checkbox.h"
    #include "wx/radiobox.h"
    #include "wx/statbox.h"
    #include "wx/textctrl.h"
#endif

#include "wx/sizer.h"

#include "widgets.h"

#include "icons/radiobox.xpm"

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// control ids
enum
{
    RadioPage_Reset = 100,
    RadioPage_Update,
    RadioPage_Selection,
    RadioPage_Label,
    RadioPage_LabelBtn,
    RadioPage_Enable2nd,
    RadioPage_Show2nd,
    RadioPage_Radio
};

// layout direction radiobox selections
enum
{
    RadioDir_Default,
    RadioDir_LtoR,
    RadioDir_TtoB
};

// default values for the number of radiobox items
static const unsigned int DEFAULT_NUM_ENTRIES = 12;
static const unsigned int DEFAULT_MAJOR_DIM = 3;

// ----------------------------------------------------------------------------
// RadioWidgetsPage
// ----------------------------------------------------------------------------

class RadioWidgetsPage : public WidgetsPage
{
public:
    RadioWidgetsPage(wxBookCtrl *book, wxImageList *imaglist);
    virtual ~RadioWidgetsPage(){};

    virtual wxControl *GetWidget() const { return m_radio; }

protected:
    // event handlers
    void OnCheckOrRadioBox(wxCommandEvent& event);
    void OnRadioBox(wxCommandEvent& event);

    void OnButtonReset(wxCommandEvent& event);
    void OnButtonRecreate(wxCommandEvent& event);

    void OnButtonSelection(wxCommandEvent& event);
    void OnButtonSetLabel(wxCommandEvent& event);

    void OnUpdateUIReset(wxUpdateUIEvent& event);
    void OnUpdateUIUpdate(wxUpdateUIEvent& event);
    void OnUpdateUISelection(wxUpdateUIEvent& event);

    // reset the wxRadioBox parameters
    void Reset();

    // (re)create the wxRadioBox
    void CreateRadio();

    // the controls
    // ------------

    // the check/radio boxes for styles
    wxCheckBox *m_chkVert;
    wxCheckBox *m_2ndEnabled;
    wxCheckBox *m_2ndShown;
    wxRadioBox *m_radioDir;

    // the gauge itself and the sizer it is in
    wxRadioBox *m_radio;
    wxSizer *m_sizerRadio;

    // the text entries for command parameters
    wxTextCtrl *m_textNumBtns,
               *m_textMajorDim,
               *m_textCurSel,
               *m_textSel,
               *m_textLabel,
               *m_textLabelBtns;

private:
    DECLARE_EVENT_TABLE()
    DECLARE_WIDGETS_PAGE(RadioWidgetsPage)
};

// ----------------------------------------------------------------------------
// event tables
// ----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(RadioWidgetsPage, WidgetsPage)
    EVT_BUTTON(RadioPage_Reset, RadioWidgetsPage::OnButtonReset)

    EVT_BUTTON(RadioPage_Update, RadioWidgetsPage::OnButtonRecreate)
    EVT_BUTTON(RadioPage_LabelBtn, RadioWidgetsPage::OnButtonRecreate)

    EVT_BUTTON(RadioPage_Selection, RadioWidgetsPage::OnButtonSelection)
    EVT_BUTTON(RadioPage_Label, RadioWidgetsPage::OnButtonSetLabel)

    EVT_UPDATE_UI(RadioPage_Update, RadioWidgetsPage::OnUpdateUIUpdate)
    EVT_UPDATE_UI(RadioPage_Selection, RadioWidgetsPage::OnUpdateUISelection)

    EVT_RADIOBOX(RadioPage_Radio, RadioWidgetsPage::OnRadioBox)

    EVT_CHECKBOX(wxID_ANY, RadioWidgetsPage::OnCheckOrRadioBox)
    EVT_RADIOBOX(wxID_ANY, RadioWidgetsPage::OnCheckOrRadioBox)
END_EVENT_TABLE()

// ============================================================================
// implementation
// ============================================================================

IMPLEMENT_WIDGETS_PAGE(RadioWidgetsPage, _T("Radio"));

RadioWidgetsPage::RadioWidgetsPage(wxBookCtrl *book,
                                   wxImageList *imaglist)
                  : WidgetsPage(book)
{
    imaglist->Add(wxBitmap(radio_xpm));

    // init everything
    m_chkVert = (wxCheckBox *)NULL;
    m_2ndEnabled = (wxCheckBox *)NULL;
    m_2ndShown = (wxCheckBox *)NULL;

    m_textNumBtns =
    m_textLabelBtns =
    m_textLabel = (wxTextCtrl *)NULL;

    m_radio =
    m_radioDir = (wxRadioBox *)NULL;
    m_sizerRadio = (wxSizer *)NULL;

    wxSizer *sizerTop = new wxBoxSizer(wxHORIZONTAL);

    // left pane
    wxStaticBox *box = new wxStaticBox(this, wxID_ANY, _T("&Set style"));

    wxSizer *sizerLeft = new wxStaticBoxSizer(box, wxVERTICAL);

    m_chkVert = CreateCheckBoxAndAddToSizer(sizerLeft, _T("&Vertical layout"));

    static const wxString layoutDir[] =
    {
        _T("default"),
        _T("left to right"),
        _T("top to bottom")
    };

    m_radioDir = new wxRadioBox(this, wxID_ANY, _T("Numbering:"),
                                wxDefaultPosition, wxDefaultSize,
                                WXSIZEOF(layoutDir), layoutDir,
                                1, wxRA_SPECIFY_COLS);
    sizerLeft->Add(m_radioDir, 0, wxGROW | wxALL, 5);

    // if it's not defined, we can't change the radiobox direction
#ifndef wxRA_LEFTTORIGHT
    m_radioDir->Disable();
#endif // wxRA_LEFTTORIGHT

    wxSizer *sizerRow;
    sizerRow = CreateSizerWithTextAndLabel(_T("&Major dimension:"),
                                           wxID_ANY,
                                           &m_textMajorDim);
    sizerLeft->Add(sizerRow, 0, wxGROW | wxALL, 5);

    sizerRow = CreateSizerWithTextAndLabel(_T("&Number of buttons:"),
                                           wxID_ANY,
                                           &m_textNumBtns);
    sizerLeft->Add(sizerRow, 0, wxGROW | wxALL, 5);

    wxButton *btn;
    btn = new wxButton(this, RadioPage_Update, _T("&Update"));
    sizerLeft->Add(btn, 0, wxALIGN_CENTRE_HORIZONTAL | wxALL, 5);

    sizerLeft->Add(5, 5, 0, wxGROW | wxALL, 5); // spacer

    btn = new wxButton(this, RadioPage_Reset, _T("&Reset"));
    sizerLeft->Add(btn, 0, wxALIGN_CENTRE_HORIZONTAL | wxALL, 15);

    // middle pane
    wxStaticBox *box2 = new wxStaticBox(this, wxID_ANY, _T("&Change parameters"));
    wxSizer *sizerMiddle = new wxStaticBoxSizer(box2, wxVERTICAL);

    sizerRow = CreateSizerWithTextAndLabel(_T("Current selection:"),
                                           wxID_ANY,
                                           &m_textCurSel);
    sizerMiddle->Add(sizerRow, 0, wxGROW | wxALL, 5);

    sizerRow = CreateSizerWithTextAndButton(RadioPage_Selection,
                                            _T("&Change selection:"),
                                           wxID_ANY,
                                           &m_textSel);
    sizerMiddle->Add(sizerRow, 0, wxGROW | wxALL, 5);

    sizerRow = CreateSizerWithTextAndButton(RadioPage_Label,
                                            _T("&Label for box:"),
                                            wxID_ANY,
                                            &m_textLabel);
    sizerMiddle->Add(sizerRow, 0, wxGROW | wxALL, 5);

    sizerRow = CreateSizerWithTextAndButton(RadioPage_LabelBtn,
                                            _T("&Label for buttons:"),
                                            wxID_ANY,
                                            &m_textLabelBtns);
    sizerMiddle->Add(sizerRow, 0, wxGROW | wxALL, 5);

    m_2ndEnabled = CreateCheckBoxAndAddToSizer(sizerMiddle, _T("2nd item enabled"));
    m_2ndShown = CreateCheckBoxAndAddToSizer(sizerMiddle, _T("2nd item shown"));

    // right pane
    wxSizer *sizerRight = new wxBoxSizer(wxHORIZONTAL);
    sizerRight->SetMinSize(150, 0);
    m_sizerRadio = sizerRight; // save it to modify it later

    Reset();
    CreateRadio();

    // the 3 panes panes compose the window
    sizerTop->Add(sizerLeft, 0, wxGROW | (wxALL & ~wxLEFT), 10);
    sizerTop->Add(sizerMiddle, 1, wxGROW | wxALL, 10);
    sizerTop->Add(sizerRight, 0, wxGROW | (wxALL & ~wxRIGHT), 10);

    // final initializations
    SetSizer(sizerTop);

    sizerTop->Fit(this);
}

// ----------------------------------------------------------------------------
// operations
// ----------------------------------------------------------------------------

void RadioWidgetsPage::Reset()
{
    m_textMajorDim->SetValue(wxString::Format(_T("%u"), DEFAULT_MAJOR_DIM));
    m_textNumBtns->SetValue(wxString::Format(_T("%u"), DEFAULT_NUM_ENTRIES));
    m_textLabel->SetValue(_T("I'm a radiobox"));
    m_textLabelBtns->SetValue(_T("item"));

    m_chkVert->SetValue(false);
    m_2ndEnabled->SetValue(true);
    m_2ndShown->SetValue(true);
    m_radioDir->SetSelection(RadioDir_Default);
}

void RadioWidgetsPage::CreateRadio()
{
    int sel;
    if ( m_radio )
    {
        sel = m_radio->GetSelection();

        m_sizerRadio->Detach( m_radio );

        delete m_radio;
    }
    else // first time creation, no old selection to preserve
    {
        sel = -1;
    }

    unsigned long count;
    if ( !m_textNumBtns->GetValue().ToULong(&count) )
    {
        wxLogWarning(_T("Should have a valid number for number of items."));

        // fall back to default
        count = DEFAULT_NUM_ENTRIES;
    }

    unsigned long majorDim;
    if ( !m_textMajorDim->GetValue().ToULong(&majorDim) )
    {
        wxLogWarning(_T("Should have a valid major dimension number."));

        // fall back to default
        majorDim = DEFAULT_MAJOR_DIM;
    }

    wxString *items = new wxString[count];

    wxString labelBtn = m_textLabelBtns->GetValue();
    for ( size_t n = 0; n < count; n++ )
    {
        items[n] = wxString::Format(_T("%s %lu"),
                                    labelBtn.c_str(), (unsigned long)n + 1);
    }

    int flags = m_chkVert->GetValue() ? wxRA_VERTICAL
                                      : wxRA_HORIZONTAL;

#ifdef wxRA_LEFTTORIGHT
    switch ( m_radioDir->GetSelection() )
    {
        default:
            wxFAIL_MSG( _T("unexpected wxRadioBox layout direction") );
            // fall through

        case RadioDir_Default:
            break;

        case RadioDir_LtoR:
            flags |= wxRA_LEFTTORIGHT;
            break;

        case RadioDir_TtoB:
            flags |= wxRA_TOPTOBOTTOM;
            break;
    }
#endif // wxRA_LEFTTORIGHT

    m_radio = new wxRadioBox(this, RadioPage_Radio,
                             m_textLabel->GetValue(),
                             wxDefaultPosition, wxDefaultSize,
                             count, items,
                             majorDim,
                             flags);

    delete [] items;

    if ( sel >= 0 && (size_t)sel < count )
    {
        m_radio->SetSelection(sel);
    }

    m_sizerRadio->Add(m_radio, 1, wxGROW);
    m_sizerRadio->Layout();

    m_radio->Enable( 1 , m_2ndEnabled->GetValue() );
    m_radio->Show( 1 , m_2ndShown->GetValue() );
}

// ----------------------------------------------------------------------------
// event handlers
// ----------------------------------------------------------------------------

void RadioWidgetsPage::OnButtonReset(wxCommandEvent& WXUNUSED(event))
{
    Reset();

    CreateRadio();
}

void RadioWidgetsPage::OnCheckOrRadioBox(wxCommandEvent& WXUNUSED(event))
{
    CreateRadio();
}

void RadioWidgetsPage::OnRadioBox(wxCommandEvent& event)
{
    int sel = m_radio->GetSelection();
    int event_sel = event.GetSelection();
    wxUnusedVar(event_sel);

    wxLogMessage(_T("Radiobox selection changed, now %d"), sel);

    wxASSERT_MSG( sel == event_sel,
                  _T("selection should be the same in event and radiobox") );

    m_textCurSel->SetValue(wxString::Format(_T("%d"), sel));
}

void RadioWidgetsPage::OnButtonRecreate(wxCommandEvent& WXUNUSED(event))
{
    CreateRadio();
}

void RadioWidgetsPage::OnButtonSetLabel(wxCommandEvent& WXUNUSED(event))
{
    m_radio->wxControl::SetLabel(m_textLabel->GetValue());
}

void RadioWidgetsPage::OnButtonSelection(wxCommandEvent& WXUNUSED(event))
{
    unsigned long sel;
    if ( !m_textSel->GetValue().ToULong(&sel) ||
            (sel >= (size_t)m_radio->GetCount()) )
    {
        wxLogWarning(_T("Invalid number specified as new selection."));
    }
    else
    {
        m_radio->SetSelection(sel);
    }
}

void RadioWidgetsPage::OnUpdateUIUpdate(wxUpdateUIEvent& event)
{
    unsigned long n;
    event.Enable( m_textNumBtns->GetValue().ToULong(&n) &&
                  m_textMajorDim->GetValue().ToULong(&n) );
}

void RadioWidgetsPage::OnUpdateUISelection(wxUpdateUIEvent& event)
{
    unsigned long n;
    event.Enable( m_textSel->GetValue().ToULong(&n) &&
                   (n < (size_t)m_radio->GetCount()) );
}

void RadioWidgetsPage::OnUpdateUIReset(wxUpdateUIEvent& event)
{
    // only enable it if something is not set to default
    bool enable = m_chkVert->GetValue();

    if ( !enable )
    {
        unsigned long numEntries;

        enable = !m_textNumBtns->GetValue().ToULong(&numEntries) ||
                    numEntries != DEFAULT_NUM_ENTRIES;

        if ( !enable )
        {
            unsigned long majorDim;

            enable = !m_textMajorDim->GetValue().ToULong(&majorDim) ||
                        majorDim != DEFAULT_MAJOR_DIM;
        }
    }

    event.Enable(enable);
}

#endif // wxUSE_RADIOBOX
