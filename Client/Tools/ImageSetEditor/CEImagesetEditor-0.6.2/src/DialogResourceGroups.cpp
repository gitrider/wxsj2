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
#include "DialogResourceGroups.h"
#include "wxPathCellEditor.h"
#include "CEGUIHelper.h"
#include <CEGUISystem.h>
#include <CEGUIDefaultResourceProvider.h>

//--------------------------------------------------------------------------------
BEGIN_EVENT_TABLE( DialogResourceGroups, wxDialog )
EVT_GRID_EDITOR_SHOWN( DialogResourceGroups::OnEditorShown )
EVT_GRID_CELL_CHANGE( DialogResourceGroups::OnCellEdit )
EVT_BUTTON( ID_ADDGROUP_BUTTON, DialogResourceGroups::OnAddgroupButtonClick )
EVT_BUTTON( ID_REMOVEGROUP_BUTTON, DialogResourceGroups::OnRemovegroupButtonClick )
EVT_BUTTON( ID_DEFAULTGROUP_BUTTON, DialogResourceGroups::OnSetDefaultGroupButtonClick )
EVT_BUTTON( wxID_OK, DialogResourceGroups::OnOkClick )
EVT_BUTTON( wxID_CANCEL, DialogResourceGroups::OnCancelClick )
END_EVENT_TABLE()

//--------------------------------------------------------------------------------
DialogResourceGroups::DialogResourceGroups( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style ) :
        m_groupGUID( 0 ),
        m_groupsGrid( 0 ),
        m_addGroupButton( 0 ),
        m_removeGroupButton( 0 ),
        m_okButton( 0 ),
        m_cancelButton( 0 )
{
    Create( parent, id, caption, pos, size, style );
}

//--------------------------------------------------------------------------------
bool DialogResourceGroups::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    SetExtraStyle( GetExtraStyle() | wxWS_EX_BLOCK_EVENTS );
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();

    return true;
}

//--------------------------------------------------------------------------------
void DialogResourceGroups::CreateControls()
{
    DialogResourceGroups * itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer( wxVERTICAL );
    itemDialog1->SetSizer( itemBoxSizer2 );

    wxStaticBox* itemStaticBoxSizer3Static = new wxStaticBox( itemDialog1, wxID_ANY, wxT( "Defined groups" ) );
    wxStaticBoxSizer* itemStaticBoxSizer3 = new wxStaticBoxSizer( itemStaticBoxSizer3Static, wxVERTICAL );
    itemBoxSizer2->Add( itemStaticBoxSizer3, 1, wxGROW | wxALL, 5 );

    m_groupsGrid = new wxGrid( itemDialog1, ID_GROUPS_GRID, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL );
    m_groupsGrid->SetHelpText( wxT( "List of currently defined resource groups." ) );
    if ( ShowToolTips() )
        m_groupsGrid->SetToolTip( wxT( "List of currently defined resource groups." ) );
    m_groupsGrid->SetDefaultRowSize( 20 );
    m_groupsGrid->SetColLabelSize( 20 );
    m_groupsGrid->SetRowLabelSize( 20 );
    m_groupsGrid->CreateGrid( 0, 2, wxGrid::wxGridSelectRows );
    m_groupsGrid->SetColLabelValue( 0, wxT( "Name" ) );
    m_groupsGrid->SetColLabelValue( 1, wxT( "Directory" ) );
    m_groupsGrid->SetColSize( 0, 100 );
    m_groupsGrid->SetColSize( 1, 320 );
    itemStaticBoxSizer3->Add( m_groupsGrid, 1, wxGROW | wxALL, 5 );

    wxStaticText* itemStaticText5 = new wxStaticText( itemDialog1, wxID_STATIC, wxT( "Default resource group:" ), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer3->Add( itemStaticText5, 0, wxALIGN_LEFT | wxLEFT | wxRIGHT | wxTOP | wxADJUST_MINSIZE, 5 );

    m_defaultGroupTextCtrl = new wxTextCtrl( itemDialog1, ID_DEFAULTGROUP_TEXTCTRL, _T( "" ), wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxSIMPLE_BORDER );
    m_defaultGroupTextCtrl->SetHelpText( wxT( "Shows the currently selected default resource group." ) );
    if ( ShowToolTips() )
        m_defaultGroupTextCtrl->SetToolTip( wxT( "Shows the currently selected default resource group." ) );
    //m_defaultGroupTextCtrl->Enable(false);
    itemStaticBoxSizer3->Add( m_defaultGroupTextCtrl, 0, wxGROW | wxLEFT | wxRIGHT | wxBOTTOM, 5 );

    wxBoxSizer* itemBoxSizer5 = new wxBoxSizer( wxHORIZONTAL );
    itemStaticBoxSizer3->Add( itemBoxSizer5, 0, wxGROW | wxALL, 5 );

    m_addGroupButton = new wxButton( itemDialog1, ID_ADDGROUP_BUTTON, wxT( "Add Group" ), wxDefaultPosition, wxDefaultSize, 0 );
    m_addGroupButton->SetHelpText( wxT( "Add a new resource group." ) );
    if ( ShowToolTips() )
        m_addGroupButton->SetToolTip( wxT( "Add a new resource group." ) );
    itemBoxSizer5->Add( m_addGroupButton, 1, wxGROW | wxALL, 5 );

    m_removeGroupButton = new wxButton( itemDialog1, ID_REMOVEGROUP_BUTTON, wxT( "Remove Group" ), wxDefaultPosition, wxDefaultSize, 0 );
    m_removeGroupButton->SetHelpText( wxT( "Delete the selected resource group." ) );
    if ( ShowToolTips() )
        m_removeGroupButton->SetToolTip( wxT( "Delete the selected resource group." ) );
    itemBoxSizer5->Add( m_removeGroupButton, 1, wxGROW | wxALL, 5 );

    m_setDefaultButton = new wxButton( itemDialog1, ID_DEFAULTGROUP_BUTTON, wxT( "Set Default Group" ), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer5->Add( m_setDefaultButton, 1, wxGROW | wxALL, 5 );

    wxStdDialogButtonSizer* itemStdDialogButtonSizer8 = new wxStdDialogButtonSizer;

    itemBoxSizer2->Add( itemStdDialogButtonSizer8, 0, wxALIGN_RIGHT | wxALL, 5 );
    m_okButton = new wxButton( itemDialog1, wxID_OK, wxT( "&OK" ), wxDefaultPosition, wxDefaultSize, 0 );
    m_okButton->SetDefault();
    m_okButton->SetHelpText( wxT( "Accept changes made to resource groups." ) );
    if ( ShowToolTips() )
        m_okButton->SetToolTip( wxT( "Accept changes made to resource groups." ) );
    itemStdDialogButtonSizer8->AddButton( m_okButton );

    m_cancelButton = new wxButton( itemDialog1, wxID_CANCEL, wxT( "&Cancel" ), wxDefaultPosition, wxDefaultSize, 0 );
    m_cancelButton->SetHelpText( wxT( "Cancel all changes made to resource groups." ) );
    if ( ShowToolTips() )
        m_cancelButton->SetToolTip( wxT( "Cancel all changes made to resource groups." ) );
    itemStdDialogButtonSizer8->AddButton( m_cancelButton );

    itemStdDialogButtonSizer8->Realize();
}

//--------------------------------------------------------------------------------
bool DialogResourceGroups::ShowToolTips()
{
    return true;
}

//--------------------------------------------------------------------------------
wxBitmap DialogResourceGroups::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
    wxUnusedVar( name );
    return wxNullBitmap;
}

//--------------------------------------------------------------------------------
wxIcon DialogResourceGroups::GetIconResource( const wxString& name )
{
    // Icon retrieval
    wxUnusedVar( name );
    return wxNullIcon;
}

//--------------------------------------------------------------------------------
void DialogResourceGroups::OnAddgroupButtonClick( wxCommandEvent& event )
{
    addGroupEntry( getUniqueGroupName(), wxT( "." ) );
    event.Skip();
}

//--------------------------------------------------------------------------------
void DialogResourceGroups::OnRemovegroupButtonClick( wxCommandEvent& event )
{
    if ( m_groupsGrid->IsSelection() )
    {
        int delidx;

        // selection could be in the form of cells or rows, so grab both
        wxGridCellCoordsArray cellsSel( m_groupsGrid->GetSelectionBlockTopLeft() );
        wxArrayInt rowsSel = m_groupsGrid->GetSelectedRows();

        if ( cellsSel.Count() > 0 )
            delidx = cellsSel.Item( 0 ).GetRow();
        else if ( rowsSel.Count() > 0 )
            delidx = rowsSel.Item( 0 );
        else
            delidx = -1;

        // if there is something selected,
        if ( delidx != -1 )
            // delete it!
            deleteGroupEntry( m_groupsGrid->GetCellValue( delidx, 0 ) );
    }

    event.Skip();
}

//--------------------------------------------------------------------------------
void DialogResourceGroups::OnSetDefaultGroupButtonClick( wxCommandEvent& event )
{
    if ( m_groupsGrid->IsSelection() )
    {
        int idx;

        // selection could be in the form of cells or rows, so grab both
        wxGridCellCoordsArray cellsSel( m_groupsGrid->GetSelectionBlockTopLeft() );
        wxArrayInt rowsSel = m_groupsGrid->GetSelectedRows();

        if ( cellsSel.Count() > 0 )
            idx = cellsSel.Item( 0 ).GetRow();
        else if ( rowsSel.Count() > 0 )
            idx = rowsSel.Item( 0 );
        else
            idx = -1;

        // if there is something selected,
        if ( idx != -1 )
            // set as default
            setDefaultGroup( m_groupsGrid->GetCellValue( idx, 0 ) );
    }

    event.Skip();
}

//--------------------------------------------------------------------------------
void DialogResourceGroups::OnOkClick( wxCommandEvent& event )
{
    // dump the grid values into a map
    ResourceGroupsMap newGroups;
    int sz = m_groupsGrid->GetNumberRows();

    for ( int i = 0; i < sz; ++i )
        newGroups[ m_groupsGrid->GetCellValue( i, 0 ) ] =
            m_groupsGrid->GetCellValue( i, 1 );

    // build a list of deleted groups
    wxArrayString deletedGroups;
    ResourceGroupsMap::iterator iter = m_resourceGroups.begin();
    while ( iter != m_resourceGroups.end() )
    {
        if ( newGroups.count( iter->first ) == 0 )
            deletedGroups.Add( iter->first );

        ++iter;
    }

    // set our map to equal the new set of defined groups
    m_resourceGroups = newGroups;

    // get the resource provier from CEGUI::System
    CEGUI::DefaultResourceProvider* rp =
        static_cast<CEGUI::DefaultResourceProvider*>(
            CEGUI::System::getSingleton().getResourceProvider() );

    // erase groups that were deleted
    for ( size_t j = 0; j < deletedGroups.size(); ++j )
        rp->clearResourceGroupDirectory( CEGUIHelper::ToCEGUIString( deletedGroups[ j ] ) );

    // update all groups according to our map
    iter = m_resourceGroups.begin();
    while ( iter != m_resourceGroups.end() )
    {
        rp->setResourceGroupDirectory(
            CEGUIHelper::ToCEGUIString( iter->first ),
            CEGUIHelper::ToCEGUIString( iter->second ) );

        ++iter;
    }

    // Set the default resource group
    rp->setDefaultResourceGroup(
        CEGUIHelper::ToCEGUIString( m_defaultGroupName ) );

    // wipe out backup info incase some strangeness happens
    m_backupResourceGroups.clear();
    m_backupDefaultGroupName.clear();

    event.Skip();
}

//--------------------------------------------------------------------------------
void DialogResourceGroups::OnCancelClick( wxCommandEvent& event )
{
    // Restore the internal state to what it was before we started playing
    m_resourceGroups = m_backupResourceGroups;

    // sync the grid ready for next time
    clearGroupsGrid();
    populateGrid();

    setDefaultGroup( m_backupDefaultGroupName );

    // wipe out backup info incase some strangeness happens
    m_backupResourceGroups.clear();
    m_backupDefaultGroupName.clear();

    event.Skip();
}

//--------------------------------------------------------------------------------
void DialogResourceGroups::OnCellEdit( wxGridEvent& event )
{
    wxString groupName = m_groupsGrid->GetCellValue( event.GetRow(), 0 );

    // did we edit the name?
    if ( event.GetCol() == 0 )
    {
        // has the name actually changed?
        if ( m_oldGroupName.Cmp( groupName ) != 0 )
        {
            if ( renameResourceGroup( m_oldGroupName, groupName ) )
                m_oldGroupName = groupName;
            else
                m_groupsGrid->SetCellValue( event.GetRow(), 0, m_oldGroupName );
        }
    }
    // update the directory value instead
    else
    {
        setGroupDirectory(
            groupName,
            m_groupsGrid->GetCellValue( event.GetRow(), 1 ) );
    }

    event.Skip();
}

//--------------------------------------------------------------------------------
void DialogResourceGroups::OnEditorShown( wxGridEvent& event )
{
    int col = event.GetCol();
    int row = event.GetRow();

    if ( col == 0 )
        m_oldGroupName = m_groupsGrid->GetCellValue( row, 0 );

    event.Skip();
}

//--------------------------------------------------------------------------------
bool DialogResourceGroups::addGroupEntry( const wxString& groupName, const wxString& directory )
{
    // see if this exists, and put up an error if it does
    if ( isResourceGroupDefined( groupName ) )
    {
        wxLogError ( wxT( "DialogResourceGroups::addGroupEntry - Error: The resource group already exists." ) );
        return false;
    }

    int idx = m_groupsGrid->GetNumberRows();

    if ( m_groupsGrid->AppendRows() )
    {
        m_groupsGrid->SetCellAlignment( idx, 0, wxALIGN_LEFT, wxALIGN_CENTRE );
        m_groupsGrid->SetCellAlignment( idx, 1, wxALIGN_LEFT, wxALIGN_CENTRE );
        m_groupsGrid->SetCellValue( idx, 0, groupName );
        // this cell uses our custom editor
        m_groupsGrid->SetCellEditor( idx, 1, new wxPathCellEditor() );

        // add an empty entry in the map
        m_resourceGroups[ groupName ];
        // set the directory name
        setGroupDirectory( groupName, directory );

        return true;
    }
    else
    {
        wxLogError ( wxT( "DialogResourceGroups::addGroupEntry - Error: unable to add a new row to the grid." ) );
        return false;
    }
}

//-----------------------------------------------------------------------
bool DialogResourceGroups::deleteGroupEntry( const wxString& groupName )
{
    if ( isResourceGroupDefined( groupName ) )
    {
        m_resourceGroups.erase( groupName );
        // erase entry from the grid
        int idx = getInternalGroupIndex( groupName );
        if ( idx != -1 )
            m_groupsGrid->DeleteRows( idx, 1 );

        // update the default if we just deleted it
        if ( m_defaultGroupName.Cmp( groupName ) == 0 )
            setDefaultGroup( wxT( "" ) );

        return true;
    }
    else
    {
        wxLogError ( wxT( "DialogResourceGroups::setGroupDirectory - Error: Resource group is not defined." ) );
        return false;
    }

}

//-----------------------------------------------------------------------
bool DialogResourceGroups::setGroupDirectory( const wxString& groupName, const wxString& directory )
{
    if ( isResourceGroupDefined( groupName ) )
    {
        wxString finalDirspec( directory );

        // ensure there is a trailing slash of some kind
        size_t lastIdx = finalDirspec.length() - 1;
        if ( !finalDirspec.empty() &&
                ( finalDirspec[ lastIdx ] != '/' ) &&
                ( finalDirspec[ lastIdx ] != '\\' ) )
        {
            finalDirspec += '/';
        }

        m_resourceGroups[ groupName ] = finalDirspec;

        // update the grid
        int idx = getInternalGroupIndex( groupName );
        if ( idx != -1 )
            m_groupsGrid->SetCellValue( idx, 1, finalDirspec );

        return true;
    }
    else
    {
        wxLogError ( wxT( "DialogResourceGroups::setGroupDirectory - Error: Resource group is not defined." ) );
        return false;
    }
}

//-----------------------------------------------------------------------
wxString DialogResourceGroups::getGroupDirectory( const wxString& groupName ) const
{
    ResourceGroupsMap::const_iterator iter = m_resourceGroups.find( groupName );

    if ( iter != m_resourceGroups.end() )
        return iter->second;
    else
    {
        wxLogError ( wxT( "DialogResourceGroups::getGroupDirectory - Error: Resource group is not defined." ) );
        return wxString( wxT( "" ) );
    }
}

//-----------------------------------------------------------------------
bool DialogResourceGroups::isResourceGroupDefined( const wxString& groupName ) const
{
    ResourceGroupsMap::const_iterator iter = m_resourceGroups.find( groupName );
    return ( iter != m_resourceGroups.end() );
}

//-----------------------------------------------------------------------
bool DialogResourceGroups::renameResourceGroup( const wxString& currentName, const wxString& newName )
{
    if ( !isResourceGroupDefined( newName ) )
    {
        if ( isResourceGroupDefined( currentName ) )
        {
            wxString dir = m_resourceGroups[ currentName ];
            m_resourceGroups.erase( currentName );
            m_resourceGroups[ newName ] = dir;

            // update the grid
            int idx = getInternalGroupIndex( currentName );
            if ( idx != -1 )
                m_groupsGrid->SetCellValue( idx, 0, newName );

            // handle case where default group is renamed
            if ( m_defaultGroupName.Cmp( currentName ) == 0 )
                setDefaultGroup( newName );

            return true;
        }
        else
            wxLogError ( wxT( "DialogResourceGroups::renameResourceGroup - Error: Resource group is not defined." ) );
    }
    else
        wxLogError ( wxT( "DialogResourceGroups::renameResourceGroup - Error: Resource group name already exists." ) );

    return false;
}

//-----------------------------------------------------------------------
DialogResourceGroups::ResourceGroupsMap::const_iterator DialogResourceGroups::getGroupIteratorStart() const
{
    return m_resourceGroups.begin();
}

//-----------------------------------------------------------------------
DialogResourceGroups::ResourceGroupsMap::const_iterator DialogResourceGroups::getGroupIteratorEnd() const
{
    return m_resourceGroups.end();
}

//-----------------------------------------------------------------------
bool DialogResourceGroups::setDefaultGroup( const wxString& groupName )
{
    if ( groupName.empty() || isResourceGroupDefined( groupName ) )
    {
        m_defaultGroupName = groupName;

        // update the dialog
        m_defaultGroupTextCtrl->SetValue( m_defaultGroupName );

        return true;
    }
    else
    {
        wxLogError ( wxT( "DialogResourceGroups::setDefaultGroup - Error: Resource group is not defined." ) );
        return false;
    }
}

//-----------------------------------------------------------------------
const wxString& DialogResourceGroups::getDefaultGroup() const
{
    return m_defaultGroupName;
}

//-----------------------------------------------------------------------
wxString DialogResourceGroups::getUniqueGroupName()
{
    wxString name( wxT( "NewGroup" ) );
    wxString basename( name );
    wxString count;

    do
    {
        count.sprintf ( wxT( "%d" ), ++m_groupGUID );
        name = basename + count;
    }
    while ( isResourceGroupDefined( name ) );

    return name;
}

//-----------------------------------------------------------------------
int DialogResourceGroups::getInternalGroupIndex( const wxString& groupName ) const
{
    int rowsize = m_groupsGrid->GetRows();

    for ( int i = 0; i < rowsize; i++ )
    {
        const wxString rowname = m_groupsGrid->GetCellValue( i, 0 );
        if ( rowname.Cmp( groupName ) == 0 )
            return i;
    }

    return -1;
}

//-----------------------------------------------------------------------
void DialogResourceGroups::clearGroupsGrid()
{
    int sz = m_groupsGrid->GetNumberRows();
    if ( sz )
        m_groupsGrid->DeleteRows( 0, sz );
}

//-----------------------------------------------------------------------
void DialogResourceGroups::populateGrid()
{
    ResourceGroupsMap::const_iterator iter =
        getGroupIteratorStart();

    while (iter != getGroupIteratorEnd() )
    {
        m_groupsGrid->InsertRows( 0, 1);
        m_groupsGrid->SetCellValue( 0, 0, iter->first );
        m_groupsGrid->SetCellValue( 0, 1, iter->second );

        ++iter;
    }
}

//-----------------------------------------------------------------------
void DialogResourceGroups::invoke()
{
    // make a backup of the current groups and default incase we are
    // cancelled.
    m_backupDefaultGroupName = m_defaultGroupName;
    m_backupResourceGroups = m_resourceGroups;

    // show ourselves
    ShowModal();
}
