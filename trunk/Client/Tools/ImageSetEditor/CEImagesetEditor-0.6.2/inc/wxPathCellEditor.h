///////////////////////////////////////////////////////////////////////////////
//  For project details and authors, refer to README and AUTHORS files
//
//  This file is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This file is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
//  To view the licence online, go to: http://www.gnu.org/copyleft/gpl.html
////////////////////////////////////////////////////////////////////////////////
#ifndef _WX_PATH_CELL_EDITOR_H_
#define _WX_PATH_CELL_EDITOR_H_

#include <wx/grid.h>

// forward refs
class wxButton;
class wxTextCtrl;
class wxBoxSizer;

/** This is a custom wxGridCellEditor for use with the wxWidgets wxGrid component.
 * it allows us to have a grid cell that displays a small 'browse button' when
 * edited - pressing the browse button will invoke the directory selection
 * common dialog - upon selecting something in that dialog, the path text
 * is copied into the cell.  Other than this, the cell operates the same as a
 * normal text cell.
*/
class wxPathCellEditor : public wxEvtHandler, public wxGridCellEditor
{
public:
    wxPathCellEditor();

    virtual void Create(wxWindow* parent,
                        wxWindowID id,
                        wxEvtHandler* evtHandler);
    virtual void SetSize(const wxRect& rect);

    virtual void PaintBackground(const wxRect& rectCell, wxGridCellAttr *attr);

    virtual bool IsAcceptedKey(wxKeyEvent& event);
    virtual void BeginEdit(int row, int col, wxGrid* grid);
    virtual bool EndEdit(int row, int col, wxGrid* grid);

    virtual void Reset();
    virtual void StartingKey(wxKeyEvent& event);
    virtual void HandleReturn(wxKeyEvent& event);

    // parameters string format is "max_width"
    virtual void SetParameters(const wxString& params);

    virtual wxGridCellEditor* Clone() const
        { return new wxGridCellTextEditor; }

    virtual wxString GetValue() const;

    void OnBrowseButtonClick( wxCommandEvent& event );

protected:
    wxTextCtrl* Text() const { return m_text; }

    void DoBeginEdit(const wxString& startValue);
    void DoReset(const wxString& startValue);

private:
    //! max number of chars allowed
    size_t m_maxChars;
    wxString m_startValue;

    wxButton* m_browse;
    wxTextCtrl* m_text;
    wxBoxSizer* m_sizer;

    DECLARE_EVENT_TABLE()

    DECLARE_NO_COPY_CLASS(wxPathCellEditor)
};

#endif // _WX_PATH_CELL_EDITOR_H_
