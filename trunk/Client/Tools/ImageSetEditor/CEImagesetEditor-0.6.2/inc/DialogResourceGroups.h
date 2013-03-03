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
#ifndef _DIALOG_RESOURCE_GROUPS_H_
#define _DIALOG_RESOURCE_GROUPS_H_

#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/hashmap.h>

//--------------------------------------------------------------------------------
#define ID_RESOURCE_GROUPS_DIALOG 10000
#define SYMBOL_DIALOGRESOURCEGROUPS_STYLE wxCAPTION|wxRESIZE_BORDER
#define SYMBOL_DIALOGRESOURCEGROUPS_TITLE wxT("Resource Groups")
#define SYMBOL_DIALOGRESOURCEGROUPS_IDNAME ID_RESOURCE_GROUPS_DIALOG
#define SYMBOL_DIALOGRESOURCEGROUPS_SIZE wxSize(400, 300)
#define SYMBOL_DIALOGRESOURCEGROUPS_POSITION wxDefaultPosition
#define ID_GROUPS_GRID 10001
#define ID_ADDGROUP_BUTTON 10002
#define ID_REMOVEGROUP_BUTTON 10003
#define ID_DEFAULTGROUP_TEXTCTRL 10004
#define ID_DEFAULTGROUP_BUTTON 1005

//--------------------------------------------------------------------------------
class DialogResourceGroups: public wxDialog
{
    DECLARE_EVENT_TABLE()

public:
    WX_DECLARE_STRING_HASH_MAP( wxString, ResourceGroupsMap );

    //! Constructor
    DialogResourceGroups( wxWindow* parent, wxWindowID id = SYMBOL_DIALOGRESOURCEGROUPS_IDNAME, const wxString& caption = SYMBOL_DIALOGRESOURCEGROUPS_TITLE, const wxPoint& pos = SYMBOL_DIALOGRESOURCEGROUPS_POSITION, const wxSize& size = SYMBOL_DIALOGRESOURCEGROUPS_SIZE, long style = SYMBOL_DIALOGRESOURCEGROUPS_STYLE );

    //! Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_DIALOGRESOURCEGROUPS_IDNAME, const wxString& caption = SYMBOL_DIALOGRESOURCEGROUPS_TITLE, const wxPoint& pos = SYMBOL_DIALOGRESOURCEGROUPS_POSITION, const wxSize& size = SYMBOL_DIALOGRESOURCEGROUPS_SIZE, long style = SYMBOL_DIALOGRESOURCEGROUPS_STYLE );

    //! Creates the controls and sizers
    void CreateControls();

    //! wxEVT_GRID_EDITOR_SHOWN event handler for ID_GROUPS_GRID
    void OnEditorShown( wxGridEvent& event );

    //! wxEVT_GRID_CELL_EDIT event handler for ID_GROUPS_GRID
    void OnCellEdit( wxGridEvent& event );

    //! wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_ADDGROUP_BUTTON
    void OnAddgroupButtonClick( wxCommandEvent& event );

    //! wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_REMOVEGROUP_BUTTON
    void OnRemovegroupButtonClick( wxCommandEvent& event );

    //! wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_DEFAULTGROUP_BUTTON
    void OnSetDefaultGroupButtonClick( wxCommandEvent& event );

    //! wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
    void OnOkClick( wxCommandEvent& event );

    //! wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL
    void OnCancelClick( wxCommandEvent& event );

    //! Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    //! Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );

    //! Should we show tooltips?
    static bool ShowToolTips();

    /** Invokes the dialog, this should be called to show the dialog, as
     * opposed to some other methods which may also appear to work.
    */
    void invoke();

    bool isResourceGroupDefined( const wxString& groupName ) const;
    bool renameResourceGroup( const wxString& currentName, const wxString& newName );
    bool addGroupEntry( const wxString& groupName, const wxString& directory );
    bool deleteGroupEntry( const wxString& groupName );
    bool setGroupDirectory( const wxString& groupName, const wxString& directory );
    wxString getGroupDirectory( const wxString& groupName ) const;

    bool setDefaultGroup( const wxString& groupName );
    const wxString& getDefaultGroup() const;

    ResourceGroupsMap::const_iterator getGroupIteratorStart() const;
    ResourceGroupsMap::const_iterator getGroupIteratorEnd() const;


protected:
    //! generate a new group name
    wxString getUniqueGroupName();
    //! get index of a group name in the grid, or -1 for none.
    int getInternalGroupIndex( const wxString& groupName ) const;
    //! clears the grid (but not the internal map of groups)
    void clearGroupsGrid();
    //! populates the grid based on the internal map of groups
    void populateGrid();

    //! Map of groups defined
    ResourceGroupsMap m_resourceGroups;
    //! The current default resource group selected
    wxString m_defaultGroupName;
    //! The pre-editing map of groups defined
    ResourceGroupsMap m_backupResourceGroups;
    //! The pre-editing default resource group selected
    wxString m_backupDefaultGroupName;
    //! old group name (used for tracking edits and renames of groups)
    wxString m_oldGroupName;
    //! value used in generating unique group names (when adding a new group)
    int m_groupGUID;
    //! Read-only text control that shows the selected default group
    wxTextCtrl* m_defaultGroupTextCtrl;
    //! button to select the default group
    wxButton* m_setDefaultButton;
    //! the grid of defined groups and their directories.
    wxGrid* m_groupsGrid;
    //! button to add a new group
    wxButton* m_addGroupButton;
    //! button to delete selected group
    wxButton* m_removeGroupButton;
    //! main dialog ok button
    wxButton* m_okButton;
    //! main dialog cancel button
    wxButton* m_cancelButton;
};

#endif // _DIALOG_RESOURCE_GROUPS_H_
