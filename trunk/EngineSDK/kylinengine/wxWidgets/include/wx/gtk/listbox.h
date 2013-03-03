/////////////////////////////////////////////////////////////////////////////
// Name:        listbox.h
// Purpose:     wxListBox class declaration
// Author:      Robert Roebling
// Id:          $Id: listbox.h,v 1.49 2005/02/13 17:07:47 VZ Exp $
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#ifndef __GTKLISTBOXH__
#define __GTKLISTBOXH__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "listbox.h"
#endif

#include "wx/list.h"

class WXDLLIMPEXP_BASE wxSortedArrayString;

//-----------------------------------------------------------------------------
// wxListBox
//-----------------------------------------------------------------------------

class wxListBox : public wxListBoxBase
{
public:
    // ctors and such
    wxListBox();
    wxListBox( wxWindow *parent, wxWindowID id,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            int n = 0, const wxString choices[] = (const wxString *) NULL,
            long style = 0,
            const wxValidator& validator = wxDefaultValidator,
            const wxString& name = wxListBoxNameStr )
    {
#if wxUSE_CHECKLISTBOX
        m_hasCheckBoxes = FALSE;
#endif // wxUSE_CHECKLISTBOX
        Create(parent, id, pos, size, n, choices, style, validator, name);
    }
    wxListBox( wxWindow *parent, wxWindowID id,
            const wxPoint& pos,
            const wxSize& size,
            const wxArrayString& choices,
            long style = 0,
            const wxValidator& validator = wxDefaultValidator,
            const wxString& name = wxListBoxNameStr )
    {
#if wxUSE_CHECKLISTBOX
        m_hasCheckBoxes = FALSE;
#endif // wxUSE_CHECKLISTBOX
        Create(parent, id, pos, size, choices, style, validator, name);
    }
    virtual ~wxListBox();

    bool Create(wxWindow *parent, wxWindowID id,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                int n = 0, const wxString choices[] = (const wxString *) NULL,
                long style = 0,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = wxListBoxNameStr);
    bool Create(wxWindow *parent, wxWindowID id,
                const wxPoint& pos,
                const wxSize& size,
                const wxArrayString& choices,
                long style = 0,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = wxListBoxNameStr);

    // implement base class pure virtuals
    virtual void Clear();
    virtual void Delete(int n);

    virtual int GetCount() const;
    virtual wxString GetString(int n) const;
    virtual void SetString(int n, const wxString& s);
    virtual int FindString(const wxString& s) const;

    virtual bool IsSelected(int n) const;
    virtual void DoSetSelection(int n, bool select);
    virtual int GetSelection() const;
    virtual int GetSelections(wxArrayInt& aSelections) const;

    virtual int DoAppend(const wxString& item);
    virtual void DoInsertItems(const wxArrayString& items, int pos);
    virtual void DoSetItems(const wxArrayString& items, void **clientData);

    virtual void DoSetFirstItem(int n);

    virtual void DoSetItemClientData(int n, void* clientData);
    virtual void* DoGetItemClientData(int n) const;
    virtual void DoSetItemClientObject(int n, wxClientData* clientData);
    virtual wxClientData* DoGetItemClientObject(int n) const;

    static wxVisualAttributes
    GetClassDefaultAttributes(wxWindowVariant variant = wxWINDOW_VARIANT_NORMAL);
    
    // implementation from now on

    void GtkAddItem( const wxString &item, int pos=-1 );
    int GtkGetIndex( GtkWidget *item ) const;
    GtkWidget *GetConnectWidget();
    bool IsOwnGtkWindow( GdkWindow *window );
    void DoApplyWidgetStyle(GtkRcStyle *style);
    void OnInternalIdle();

#if wxUSE_TOOLTIPS
    void ApplyToolTip( GtkTooltips *tips, const wxChar *tip );
#endif // wxUSE_TOOLTIPS

    GtkList   *m_list;
    wxList     m_clientList;

#if wxUSE_CHECKLISTBOX
    bool       m_hasCheckBoxes;
#endif // wxUSE_CHECKLISTBOX

    int        m_prevSelection;
    bool       m_blockEvent;

    virtual void FixUpMouseEvent(GtkWidget *widget, wxCoord& x, wxCoord& y);

protected:
    virtual wxSize DoGetBestSize() const;

    // return the string label for the given item
    wxString GetRealLabel(struct _GList *item) const;

    // Widgets that use the style->base colour for the BG colour should
    // override this and return true.
    virtual bool UseGTKStyleBase() const { return true; }

private:
    // this array is only used for controls with wxCB_SORT style, so only
    // allocate it if it's needed (hence using pointer)
    wxSortedArrayString *m_strings;

    DECLARE_DYNAMIC_CLASS(wxListBox)
};

#endif // __GTKLISTBOXH__
