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
#ifndef _PROPERTIES_PANEL_H_
#define _PROPERTIES_PANEL_H_

// Standard wxWidget includes
#include "wx/wx.h" 

#include "EditorDocument.h"

#include <wx/grid.h>
#include <wx/dynarray.h>


class EditorView;

class PropertiesPanel: public wxPanel
{
public:
    //! Constructor
    PropertiesPanel(wxWindow* parent, const wxPoint& pos, const wxSize& size);

    //! Sets the view which we are a part of.
    void setView (EditorView* aView);

    //! Create the widgets that populate the PropertiesPanel.
    void createContent();

    //! Resets the PropertiesPanel to it's initial state.
    void reset();

    //
    // The following are all GUI handler members; they react to changes
    // in the panel GUI and update the editor document accordingly.
    //
    // Note that these members should not directly update anything
    // relating to the view, that will happen via the document's
    // response to being updated.
    //
    //! Handler for when the image file browse button is pressed
    void onBrowseImageFile(wxCommandEvent& WXUNUSED(event));
    //! Handler for when the imageset name is changed by typing
    void onChangeImagesetName(wxCommandEvent& WXUNUSED(event));
    //! Handler for when the image filename is changed by typing.
    void onChangeImageFilename(wxCommandEvent& WXUNUSED(event));
    //! Handler for when the native resolution selection changes
    void onChangeNativeResolution(wxCommandEvent& WXUNUSED(event));
    //! Handler for when the auto scaled setting is changed.
    void onChangeAutoScaled(wxCommandEvent& WXUNUSED(event));
    //! Handler called when a row is added to the image definitions grid.
    void onGridAddRow(wxCommandEvent& WXUNUSED(event));
    //! Handler called when a row is removed from the image definitions grid.
    void onGridDelRow(wxCommandEvent& WXUNUSED(event));
    //! Handler called when the contents of a grid cell is changed.
    void onGridCellChange(wxGridEvent& event);
    //! Handler called when the a grid cell is about to be edited.
    void onGridCellEdit(wxGridEvent& event);
    //! Handler called when a grid cell (row) is selected.
    void onGridCellSelect(wxGridEvent& event);

    //
    // The following are accessor and manipulator members which are used
    // to obtain or update the information in the GUI panel widgets.
    //
    // Generally the manipulators are called by the EditorView in response
    // to something on the EditorDocument changing.  The EditorDocument
    // should never really call these members directly - everything should
    // be done via the EditorView; this keeps a nice document/view abstraction
    // and saves everything from being more tightly coupled.
    //
    //! Return the Imageset name as presented in the panel.
    wxString getImagesetName() const;

    //! Set the Imageset name to be presented in the panel.
    void setImagesetName(const wxString& name);

    //! Return the source image filename as presented in the panel.
    wxString getImageFilename() const;

    //! Set the source image filename as presented in the panel.
    void setImageFilename(const wxString& name);

    //! Return wxPoint describing the native resolution selected in the panel.
    wxPoint getNativeResolution() const;

    //! Set the native resolution to be selected in the panel.
    void setNativeResolution(const wxPoint& NativeResolution);

    //! Return whether the auto scale checkbox is selected.
    bool getAutoScaled() const;

    //! Set whether the auto scale checkbox is selected.
    void setAutoScaled(const bool AutoScaled);

    //! Adds a new image region definition to the grid
    void addRegion(const wxString& name, const wxRect& dim, int rowindex = -1);

    //! Removes an image region definition from the grid.
    int deleteRegion(const wxString& name);

    //! Removes all image region definitions from the grid.
    void deleteAllRegions();

    //! Selects an image region within the grid
    int selectRegion(const wxString& name);

    //! Returns the name of any currently selection image region.
    wxString getSelectedRegion() const;

    //! Sets the area fields for a image region.
    void setRegionArea(const wxString& name, const wxRect& dim);

    //! Returns the currently defined area for an image region.
    wxRect getRegionArea(const wxString& name) const;

    //! renames an image region definition.
    void renameRegion( const wxString& currentname, const wxString& newname );


private:
    //! returns whether we have access to a valid EditorDocument object.
    bool isDocumentValid() const;

    //! returns the current EditorDocument object.
    EditorDocument* getDocument() const;

    //! returns a unique name to be used for a new image region.
    wxString getUniqueImageName();

    static const char s_charParseNativeResolution;

    void createPropertiesGrid(wxWindow* parent);

    //! returns the index of a named region in the grid
    int getInternalRegionIndex( const wxString& name ) const;

    //! Return the given path 'filename' relative to the default resource group directory
    wxString getDefaultResourceGroupRelativePath( const wxString& filename ) const;

    //
    // Data fields
    //
    //! The view to which we are attached.
    EditorView* m_view;
    //! Text control that holds the name of the Imageset
    wxTextCtrl* m_imagesetNameTextCtrl;
    //! Text control that holds the file name of the source image.
    wxTextCtrl* m_imageFilenameTextCtrl;
    //! Button that invokes the file dialog to select an image file.
    wxButton* m_imageFilenameBrowseButton;
    //! Combobox that presents a set of native resolution options
    wxComboBox* m_nativeResolutionComboBox;
    //! Checkbox which controls the auto-scaled option.
    wxCheckBox* m_autoScaleCheckbox;
    //! Grid that holds the image region definitions
    wxGrid* m_propertiesGrid;
    //! number GUID used to generate unique image definition names.
    size_t m_imageGUID;

    wxString m_oldRowName;

    DECLARE_EVENT_TABLE()
};

#endif // _PROPERTIES_PANEL_H_
