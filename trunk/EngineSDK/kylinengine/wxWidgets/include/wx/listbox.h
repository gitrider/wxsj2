///////////////////////////////////////////////////////////////////////////////
// Name:        wx/listbox.h
// Purpose:     wxListBox class interface
// Author:      Vadim Zeitlin
// Modified by:
// Created:     22.10.99
// RCS-ID:      $Id: listbox.h,v 1.27 2005/04/08 14:33:56 MW Exp $
// Copyright:   (c) wxWidgets team
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_LISTBOX_H_BASE_
#define _WX_LISTBOX_H_BASE_

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "listboxbase.h"
#endif

#include "wx/defs.h"

#if wxUSE_LISTBOX

#include "wx/ctrlsub.h"         // base class

// forward declarations are enough here
class WXDLLIMPEXP_BASE wxArrayInt;
class WXDLLIMPEXP_BASE wxArrayString;

// ----------------------------------------------------------------------------
// global data
// ----------------------------------------------------------------------------

extern WXDLLEXPORT_DATA(const wxChar*) wxListBoxNameStr;

// ----------------------------------------------------------------------------
// wxListBox interface is defined by the class wxListBoxBase
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxListBoxBase : public wxControlWithItems
{
public:
    wxListBoxBase() { }
    virtual ~wxListBoxBase();

    // all generic methods are in wxControlWithItems, except for the following
    // ones which are not yet implemented by wxChoice/wxComboBox
    void Insert(const wxString& item, int pos)
        { DoInsert(item, pos); }
    void Insert(const wxString& item, int pos, void *clientData)
        { DoInsert(item, pos); SetClientData(pos, clientData); }
    void Insert(const wxString& item, int pos, wxClientData *clientData)
        { DoInsert(item, pos); SetClientObject(pos, clientData); }

    void InsertItems(int nItems, const wxString *items, int pos);
    void InsertItems(const wxArrayString& items, int pos)
        { DoInsertItems(items, pos); }

    void Set(int n, const wxString* items, void **clientData = NULL);
    void Set(const wxArrayString& items, void **clientData = NULL)
        { DoSetItems(items, clientData); }

    // multiple selection logic
    virtual bool IsSelected(int n) const = 0;
    virtual void SetSelection(int n) { DoSetSelection(n, true); }
    void SetSelection(int n, bool select) { DoSetSelection(n, select); }
    void Deselect(int n) { DoSetSelection(n, false); }
    void DeselectAll(int itemToLeaveSelected = -1);

    virtual bool SetStringSelection(const wxString& s, bool select);
    virtual bool SetStringSelection(const wxString& s)
    {
        return SetStringSelection(s, true);
    }

    // works for single as well as multiple selection listboxes (unlike
    // GetSelection which only works for listboxes with single selection)
    virtual int GetSelections(wxArrayInt& aSelections) const = 0;

    // set the specified item at the first visible item or scroll to max
    // range.
    void SetFirstItem(int n) { DoSetFirstItem(n); }
    void SetFirstItem(const wxString& s);

    // ensures that the given item is visible scrolling the listbox if
    // necessary
    virtual void EnsureVisible(int n);

    // a combination of Append() and EnsureVisible(): appends the item to the
    // listbox and ensures that it is visible i.e. not scrolled out of view
    void AppendAndEnsureVisible(const wxString& s);

    // return true if the listbox allows multiple selection
    bool HasMultipleSelection() const
    {
        return (m_windowStyle & wxLB_MULTIPLE) ||
               (m_windowStyle & wxLB_EXTENDED);
    }

    // return true if this listbox is sorted
    bool IsSorted() const { return (m_windowStyle & wxLB_SORT) != 0; }

    // emulate selecting or deselecting the item event.GetInt() (depending on
    // event.GetExtraLong())
    void Command(wxCommandEvent& event);

    // compatibility - these functions are deprecated, use the new ones
    // instead
    bool Selected(int n) const { return IsSelected(n); }

protected:
    // NB: due to wxGTK implementation details, DoInsert() is implemented
    //     using DoInsertItems() and not the other way round
    virtual int DoInsert(const wxString& item, int pos)
        { InsertItems(1, &item, pos); return pos; }

    // to be implemented in derived classes
    virtual void DoInsertItems(const wxArrayString& items, int pos) = 0;
    virtual void DoSetItems(const wxArrayString& items, void **clientData) = 0;

    virtual void DoSetFirstItem(int n) = 0;

    virtual void DoSetSelection(int n, bool select) = 0;


    DECLARE_NO_COPY_CLASS(wxListBoxBase)
};

// ----------------------------------------------------------------------------
// include the platform-specific class declaration
// ----------------------------------------------------------------------------

#if defined(__WXUNIVERSAL__)
    #include "wx/univ/listbox.h"
#elif defined(__WXMSW__)
    #include "wx/msw/listbox.h"
#elif defined(__WXMOTIF__)
    #include "wx/motif/listbox.h"
#elif defined(__WXGTK__)
    #include "wx/gtk/listbox.h"
#elif defined(__WXMAC__)
    #include "wx/mac/listbox.h"
#elif defined(__WXPM__)
    #include "wx/os2/listbox.h"
#elif defined(__WXCOCOA__)
    #include "wx/cocoa/listbox.h"
#endif

#endif // wxUSE_LISTBOX

#endif
    // _WX_LISTBOX_H_BASE_
