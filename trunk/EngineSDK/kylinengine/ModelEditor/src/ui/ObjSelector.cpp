// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "ObjSelector.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
#endif

#include <wx/treectrl.h>

#include "ObjSelector.h"

#include "DataManipulator.h"
// ----------------------------------------------------------------------------
// Ogre headers
// ----------------------------------------------------------------------------

#include <OgreCodec.h>
#include <OgreResourceGroupManager.h>

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
#include "ModelSystem/FairyLogicModelManager.h"

#include "SceneManipulator.h"
#include "Action.h"

// ----------------------------------------------------------------------------
// ActorSelector class
// ----------------------------------------------------------------------------

#if wxUSE_STL
#define AS_STRING(s)    (s)
#else
#define AS_STRING(s)    ((s).c_str())
#endif

static const int feID_MODEL_LIST = wxNewId();
static const int feID_BTN_UPDATE = wxNewId();

BEGIN_EVENT_TABLE(ObjSelector, wxPanel)
    EVT_UPDATE_UI(feID_MODEL_LIST, ObjSelector::OnUpdateControls)

    EVT_TREE_SEL_CHANGED(feID_MODEL_LIST, ObjSelector::OnActorListSelChanged)
    //EVT_CHILD_FOCUS(ObjSelector::OnChildFocus)
	EVT_BUTTON(feID_BTN_UPDATE, ObjSelector::OnBtnUpdate)
END_EVENT_TABLE()

namespace {

    class ActorItemData : public wxTreeItemData
    {
    public:
        ActorItemData(const Fairy::String& name,const Fairy::String& typeName)
            : name(name)
            , typeName(typeName)
        {
        }

        Fairy::String name;
        Fairy::String typeName;
    };
}

ObjSelector::~ObjSelector()
{
    if (mActorList)
    {
        mActorList->Freeze();
        mActorList->DeleteAllItems();
        mActorList->Thaw();
    }
}

void
ObjSelector::Init(const wxString& resourceGroup)
{
    mSceneManipulator = NULL;
    mActorList = NULL;
	m_pParentFrame = NULL;

    mResourceGroup = resourceGroup;
}

void
ObjSelector::SetSceneManipulator(Fairy::SceneManipulator* sceneManipulator)
{
    mSceneManipulator = sceneManipulator;
}

bool
ObjSelector::Create(wxWindow *parent, wxWindowID winid,
                      const wxPoint& pos, const wxSize& size,
                      long style, const wxString& name)
{
    if (!wxPanel::Create(parent, winid, pos, size, style, name))
        return false;

    if (!CreateControls())
        return false;

    Reload();

    return true;
}

void
ObjSelector::Reload(const wxString& resourceGroup)
{
    mResourceGroup = resourceGroup;
    Reload();
}

bool
ObjSelector::CreateControls(void)
{
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	
	mBtnUpdate = new wxButton( this, feID_BTN_UPDATE, wxT("Ë¢ÐÂ"), wxDefaultPosition, wxDefaultSize, 0 );
	sizer->Add( mBtnUpdate, 0, wxALIGN_CENTER|wxALL, 5 );

    mActorList = new wxTreeCtrl(this, feID_MODEL_LIST,
        wxDefaultPosition, wxDefaultSize,
        wxTR_DEFAULT_STYLE | wxTR_SINGLE | wxTR_HIDE_ROOT);

    mActorList->AddRoot(wxT("Root"));

    sizer->Add(mActorList, 1, wxGROW|wxALL, 5);

    this->SetSizer(sizer);
    sizer->SetSizeHints(this);

    return true;
}

wxTreeItemId
ObjSelector::AddLeaf(const wxTreeItemId& root, const wxString& name)
{
    wxString::size_type pos = name.find_last_of(wxT('/'));
    if (pos != wxString::npos)
    {
        wxTreeItemId parent = AddBranch(root, name.substr(0, pos));
        return mActorList->AppendItem(parent, name.substr(pos+1, wxString::npos));
    }
    else
    {
        return mActorList->AppendItem(root, name);
    }
}

wxTreeItemId
ObjSelector::AddBranch(const wxTreeItemId& root, const wxString& name)
{
    wxString::size_type pos = name.find_last_of(wxT('/'));
    if (pos != wxString::npos)
    {
        wxTreeItemId parent = AddBranch(root, name.substr(0, pos));
        return CreateChildItem(parent, name.substr(pos+1, wxString::npos));
    }
    else
    {
        return CreateChildItem(root, name);
    }
}

wxTreeItemId
ObjSelector::CreateChildItem(const wxTreeItemId& parent, const wxString& name)
{
    wxTreeItemIdValue cookie;
    wxTreeItemId item = mActorList->GetFirstChild(parent, cookie);
    while (item.IsOk())
    {
        if (mActorList->GetItemText(item) == name)
            return item;
        item = mActorList->GetNextChild(parent, cookie);
    }

    return mActorList->AppendItem(parent, name);
}

void ObjSelector::addResources(const wxTreeItemId& root, const wxString& pattern, const wxString& typeName, const wxString& parameterName,const wxString& groupname)
{
    Ogre::FileInfoListPtr fileInfoList =
        Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(
        AS_STRING(groupname),
        AS_STRING(pattern));
    for (Ogre::FileInfoList::const_iterator it = fileInfoList->begin(); it != fileInfoList->end(); ++it)
    {
        const Fairy::String& name = it->filename;
        wxTreeItemId item = AddLeaf(root, AS_STRING(name));
        mActorList->SetItemData(item, new ActorItemData(name, AS_STRING(typeName)));
    }
}

void ObjSelector::selItemByName(wxString& itemName)
{
	if (itemName.empty())
	{
		mActorList->Unselect();
	}
	else
	{
		wxTreeItemIdValue cookie;
		wxTreeItemId item = mActorList->GetFirstChild(mActorList->GetRootItem(), cookie);
		bool bFind = false;
		while (item)
		{
			if (mActorList->GetItemText(item) == itemName)
			{
				bFind = true;
				break;
			}
			item = mActorList->GetNextChild(mActorList->GetRootItem(), cookie);
		}

		if (bFind)
		{
			mActorList->SelectItem(item);
		}
		else
		{
			mActorList->Unselect();
		}		
	}
}

void
ObjSelector::Reload(void)
{
    if (!Ogre::ResourceGroupManager::getSingletonPtr())
        return;

    mActorList->Freeze();

    wxTreeItemId root = mActorList->GetRootItem();

	mActorList->Unselect();

    mActorList->DeleteChildren(root);
	
	if(NULL == GetDataManipulator())
		return;

	int nCount = (int)GetDataManipulator()->m_ObjTable.size();

 	for(int i=0;i<nCount;i++)
 	{
		Ogre::String name = GetDataManipulator()->m_ObjTable[i];
		wxTreeItemId item = AddLeaf(root, AS_STRING(name));
		mActorList->SetItemData(item, new ActorItemData(name,""));
 	}

    mActorList->Thaw();
}

//////////////////////////////////////////////////////////////////////////
void ObjSelector::SetParentFrame(ActorEditorFrame* pParentFrame)
{
	m_pParentFrame = pParentFrame;
}

void
ObjSelector::OnUpdateControls(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() != 0);
}

void
ObjSelector::OnActorListSelChanged(wxTreeEvent& e)
{
	if(NULL == GetDataManipulator())
		return;

	wxTreeCtrl* tree = wxStaticCast(e.GetEventObject(), wxTreeCtrl);
    mActivedItem = e.GetItem();

    wxArrayTreeItemIds selections;
    int nCount = tree->GetSelections(selections);

	for (int i = 0; i < nCount; ++i)
	{
        wxTreeItemId item = selections[i];
        ActorItemData* data = static_cast<ActorItemData*>(tree->GetItemData(item));
		if(data)
		{
			Fairy::String name = data->name;
			if (name.compare(GetDataManipulator()->m_CurObjFile) == 0)
			{
				return;
			}

			if (m_pParentFrame)
			{
				m_pParentFrame->OpenSelFile(name);
			}
		}
	}
}

void
ObjSelector::OnChildFocus(wxChildFocusEvent& e)
{
    e.Skip();

    if (e.GetWindow() != mActorList)
        return;

    wxTreeItemId item = mActorList->GetSelection();
    DoActiveItem(item);
}

const Fairy::String ObjSelector::DoActiveItem(const wxTreeItemId& item)
{
    if (item.IsOk())
    {
        ActorItemData* data = static_cast<ActorItemData*>(mActorList->GetItemData(item));
        if (data)
        {
            const Fairy::String& name = data->name;
			if(false == name.empty())
				return name;
        }
    }
	return "";
}

void ObjSelector::OnBtnUpdate(wxCommandEvent& e)
{
	if(NULL == GetDataManipulator())
		return;

	GetDataManipulator()->UpdateObjResource();
	Reload();

	wxString curFile = GetDataManipulator()->m_CurObjFile.c_str();
	selItemByName(curFile);
}
