///////////////////////////////////////////////////////////////////////////////
// Name:        generic/wizard.h
// Purpose:     declaration of generic wxWizard class
// Author:      Vadim Zeitlin
// Modified by: Robert Vazan (sizers)
// Created:     28.09.99
// RCS-ID:      $Id: wizard.h,v 1.19 2004/06/17 16:22:27 ABX Exp $
// Copyright:   (c) 1999 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// wxWizard
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "wizardg.h"
#endif

class WXDLLEXPORT wxButton;
class WXDLLEXPORT wxStaticBitmap;
class WXDLLIMPEXP_ADV wxWizardEvent;
class WXDLLEXPORT wxBoxSizer;
class WXDLLIMPEXP_ADV wxWizardSizer;

class WXDLLIMPEXP_ADV wxWizard : public wxWizardBase
{
public:
    // ctor
    wxWizard() { Init(); }
    wxWizard(wxWindow *parent,
             int id = wxID_ANY,
             const wxString& title = wxEmptyString,
             const wxBitmap& bitmap = wxNullBitmap,
             const wxPoint& pos = wxDefaultPosition,
             long style = wxDEFAULT_DIALOG_STYLE)
    {
        Init();
        Create(parent, id, title, bitmap, pos, style);
    }
    bool Create(wxWindow *parent,
             int id = wxID_ANY,
             const wxString& title = wxEmptyString,
             const wxBitmap& bitmap = wxNullBitmap,
             const wxPoint& pos = wxDefaultPosition,
             long style = wxDEFAULT_DIALOG_STYLE);
    void Init();

    // implement base class pure virtuals
    virtual bool RunWizard(wxWizardPage *firstPage);
    virtual wxWizardPage *GetCurrentPage() const;
    virtual void SetPageSize(const wxSize& size);
    virtual wxSize GetPageSize() const;
    virtual void FitToPage(const wxWizardPage *firstPage);
    virtual wxSizer *GetPageAreaSizer() const;
    virtual void SetBorder(int border);

    // implementation only from now on
    // -------------------------------

    // is the wizard running?
    bool IsRunning() const { return m_page != NULL; }

    // show the prev/next page, but call TransferDataFromWindow on the current
    // page first and return false without changing the page if
    // TransferDataFromWindow() returns false - otherwise, returns true
    bool ShowPage(wxWizardPage *page, bool goingForward = true);

    // do fill the dialog with controls
    // this is app-overridable to, for example, set help and tooltip text
    virtual void DoCreateControls();

private:
    // was the dialog really created?
    bool WasCreated() const { return m_btnPrev != NULL; }

    // event handlers
    void OnCancel(wxCommandEvent& event);
    void OnBackOrNext(wxCommandEvent& event);
    void OnHelp(wxCommandEvent& event);

    void OnWizEvent(wxWizardEvent& event);

    void AddBitmapRow(wxBoxSizer *mainColumn);
    void AddStaticLine(wxBoxSizer *mainColumn);
    void AddBackNextPair(wxBoxSizer *buttonRow);
    void AddButtonRow(wxBoxSizer *mainColumn);

    void FinishLayout();

    wxSize GetManualPageSize() const;

    // the page size requested by user
    wxSize m_sizePage;

    // the dialog position from the ctor
    wxPoint m_posWizard;

    // wizard state
    wxWizardPage *m_page;       // the current page or NULL
    wxBitmap      m_bitmap;     // the default bitmap to show

    // wizard controls
    wxButton    *m_btnPrev,     // the "<Back" button
                *m_btnNext;     // the "Next>" or "Finish" button
    wxStaticBitmap *m_statbmp;  // the control for the bitmap

    // Whether user called SetBorder()
    bool m_calledSetBorder;
    // Border around page area sizer requested using SetBorder()
    int m_border;

    // Whether RunWizard() was called
    bool m_started;

    // Page area sizer will be inserted here with padding
    wxBoxSizer *m_sizerBmpAndPage;

    // Actual position and size of pages
    wxWizardSizer *m_sizerPage;

    friend class wxWizardSizer;

    DECLARE_DYNAMIC_CLASS(wxWizard)
    DECLARE_EVENT_TABLE()
    DECLARE_NO_COPY_CLASS(wxWizard)
};

