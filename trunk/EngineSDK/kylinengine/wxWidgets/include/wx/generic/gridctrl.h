///////////////////////////////////////////////////////////////////////////
// Name:        generic/gridctrl.h
// Purpose:     wxGrid controls
// Author:      Paul Gammans, Roger Gammans
// Modified by:
// Created:     11/04/2001
// RCS-ID:      $Id: gridctrl.h,v 1.15 2005/01/17 21:29:20 ABX Exp $
// Copyright:   (c) The Computer Surgery (paul@compsurg.co.uk)
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_GENERIC_GRIDCTRL_H_
#define _WX_GENERIC_GRIDCTRL_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "gridctrl.h"
#endif

#if wxUSE_GRID

#include "wx/grid.h"
#include "wx/string.h"
#include "wx/arrstr.h"
#include "wx/datetime.h"

#define wxGRID_VALUE_CHOICEINT    _T("choiceint")
#define wxGRID_VALUE_DATETIME     _T("datetime")

#if wxUSE_DATETIME

// the default renderer for the cells containing Time and dates..
class WXDLLIMPEXP_ADV wxGridCellDateTimeRenderer : public wxGridCellStringRenderer
{
public:
    wxGridCellDateTimeRenderer(wxString outformat = wxDefaultDateTimeFormat,
                               wxString informat = wxDefaultDateTimeFormat);

    // draw the string right aligned
    virtual void Draw(wxGrid& grid,
                      wxGridCellAttr& attr,
                      wxDC& dc,
                      const wxRect& rect,
                      int row, int col,
                      bool isSelected);

    virtual wxSize GetBestSize(wxGrid& grid,
                               wxGridCellAttr& attr,
                               wxDC& dc,
                               int row, int col);

    virtual wxGridCellRenderer *Clone() const;

    // parameters string format is "width[,precision]"
    virtual void SetParameters(const wxString& params);

protected:
    wxString GetString(wxGrid& grid, int row, int col);

    wxString m_iformat;
    wxString m_oformat;
    wxDateTime m_dateDef;
    wxDateTime::TimeZone m_tz;
};

#endif // wxUSE_DATETIME

// the default renderer for the cells containing Time and dates..
class WXDLLIMPEXP_ADV wxGridCellEnumRenderer : public wxGridCellStringRenderer
{
public:
    wxGridCellEnumRenderer( const wxString& choices = wxEmptyString );

    // draw the string right aligned
    virtual void Draw(wxGrid& grid,
                      wxGridCellAttr& attr,
                      wxDC& dc,
                      const wxRect& rect,
                      int row, int col,
                      bool isSelected);

    virtual wxSize GetBestSize(wxGrid& grid,
                               wxGridCellAttr& attr,
                               wxDC& dc,
                               int row, int col);

    virtual wxGridCellRenderer *Clone() const;

    // parameters string format is "item1[,item2[...,itemN]]"
    virtual void SetParameters(const wxString& params);

protected:
    wxString GetString(wxGrid& grid, int row, int col);

    wxArrayString m_choices;
};


#if wxUSE_COMBOBOX

class WXDLLIMPEXP_ADV wxGridCellEnumEditor : public wxGridCellChoiceEditor
{
public:
    wxGridCellEnumEditor( const wxString& choices = wxEmptyString );
    virtual ~wxGridCellEnumEditor() {}

    virtual wxGridCellEditor*  Clone() const;

    virtual bool EndEdit(int row, int col, wxGrid* grid);
    virtual void BeginEdit(int row, int col, wxGrid* grid);

private:
    long int   m_startint;

    DECLARE_NO_COPY_CLASS(wxGridCellEnumEditor)
};

#endif // wxUSE_COMBOBOX

class WXDLLIMPEXP_ADV wxGridCellAutoWrapStringEditor : public wxGridCellTextEditor
{
public:
    wxGridCellAutoWrapStringEditor() : wxGridCellTextEditor() { }
    virtual void Create(wxWindow* parent,
                        wxWindowID id,
                        wxEvtHandler* evtHandler);

    virtual wxGridCellEditor *Clone() const
        { return new wxGridCellAutoWrapStringEditor; }

    DECLARE_NO_COPY_CLASS(wxGridCellAutoWrapStringEditor)
};

class WXDLLIMPEXP_ADV wxGridCellAutoWrapStringRenderer : public wxGridCellStringRenderer
{
public:
    wxGridCellAutoWrapStringRenderer() : wxGridCellStringRenderer() { }

    virtual void Draw(wxGrid& grid,
                      wxGridCellAttr& attr,
                      wxDC& dc,
                      const wxRect& rect,
                      int row, int col,
                      bool isSelected);

    virtual wxSize GetBestSize(wxGrid& grid,
                               wxGridCellAttr& attr,
                               wxDC& dc,
                               int row, int col);

    virtual wxGridCellRenderer *Clone() const
        { return new wxGridCellAutoWrapStringRenderer; }

private:
    wxArrayString GetTextLines( wxGrid& grid,
                                wxDC& dc,
                                wxGridCellAttr& attr,
                                const wxRect& rect,
                                int row, int col);

};

#endif  // #if wxUSE_GRID

#endif //_WX_GENERIC_GRIDCTRL_H_

