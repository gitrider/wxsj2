#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "ObjSelector.h"
#endif

#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
#endif

#include <wx/treectrl.h>

#include "ActorSelector.h"

#include "DataManipulator.h"

#if wxUSE_STL
#define AS_STRING(s)    (s)
#else
#define AS_STRING(s)    ((s).c_str())
#endif

static const int feID_MODEL_LIST = wxNewId();
static const int feID_MODEL_BUTTON = wxNewId();

BEGIN_EVENT_TABLE(ActorSelector, wxPanel)
    EVT_UPDATE_UI(feID_MODEL_LIST, ActorSelector::OnUpdateControls)
    EVT_TREE_SEL_CHANGED(feID_MODEL_LIST, ActorSelector::OnTreeSelChanged)
	//EVT_BUTTON(feID_MODEL_BUTTON, ActorSelector::OnCreateButton)
END_EVENT_TABLE()

namespace {

    class ModelItemData : public wxTreeItemData
    {
    public:
        ModelItemData(const Fairy::String& name, const Fairy::String& typeName, const Fairy::String& parameterName)
            : name(name)
            , typeName(typeName)
            , parameterName(parameterName)
        {
        }

        Fairy::String name;
        Fairy::String typeName;
        Fairy::String parameterName;
    };
}

ActorSelector::~ActorSelector()
{
    if (mActorList)
    {
        mActorList->Freeze();
		bInDel = TRUE;
		mActorList->DeleteAllItems();
        mActorList->Thaw();
    }
}


void ActorSelector::Init(const wxString& resourceGroup)
{
    mSceneManipulator = NULL;
    mActorList = NULL;
	bInDel = FALSE;
    mResourceGroup = resourceGroup;
	m_pParentFrame = NULL;
}

void ActorSelector::SetParentFrame(ActorEditorFrame* pParentFrame)
{
	m_pParentFrame = pParentFrame;
}

void ActorSelector::SetSceneManipulator(Fairy::SceneManipulator* sceneManipulator)
{
    mSceneManipulator = sceneManipulator;
}


bool ActorSelector::Create(wxWindow *parent, wxWindowID winid,
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


void ActorSelector::Reload(const wxString& resourceGroup)
{
    mResourceGroup = resourceGroup;
    Reload();
}


bool ActorSelector::CreateControls(void)
{
    wxFlexGridSizer* sizer = new wxFlexGridSizer(1);
    sizer->AddGrowableCol(0);
    sizer->AddGrowableRow(0);

    mActorList = new wxTreeCtrl(this, feID_MODEL_LIST,
        wxDefaultPosition, wxDefaultSize,
        wxTR_DEFAULT_STYLE | wxTR_SINGLE | wxTR_HIDE_ROOT);

    mActorList->AddRoot(wxT("Root"));

    sizer->Add(mActorList, 0, wxGROW|wxALL, 5);


	//mCreateButton = new wxButton( this, feID_MODEL_BUTTON, wxT("&´´½¨"), wxDefaultPosition, wxDefaultSize, 0 );
 //   sizer->Add( mCreateButton, 0, wxALIGN_BOTTOM|wxALL, 5 );

    this->SetSizer(sizer);
    sizer->SetSizeHints(this);

    return true;
}


wxTreeItemId ActorSelector::AddLeaf(const wxTreeItemId& root, const wxString& name)
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


wxTreeItemId ActorSelector::AddBranch(const wxTreeItemId& root, const wxString& name)
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


wxTreeItemId ActorSelector::CreateChildItem(const wxTreeItemId& parent, const wxString& name)
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


void ActorSelector::addResources(const wxTreeItemId& root, const wxString& pattern, const wxString& typeName, const wxString& parameterName,const wxString& groupname)
{
    Ogre::FileInfoListPtr fileInfoList =
        Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(
        AS_STRING(groupname),
        AS_STRING(pattern));
    for (Ogre::FileInfoList::const_iterator it = fileInfoList->begin(); it != fileInfoList->end(); ++it)
    {
        const Fairy::String& name = it->filename;
        wxTreeItemId item = AddLeaf(root, AS_STRING(name));
        mActorList->SetItemData(item, new ModelItemData(name, AS_STRING(typeName), AS_STRING(parameterName)));
    }
}


void ActorSelector::Reload(void)
{
    if (!Ogre::ResourceGroupManager::getSingletonPtr())
        return;

    mActorList->Freeze();

    wxTreeItemId root = mActorList->GetRootItem();

	mActorList->Unselect();

    mActorList->DeleteChildren(root);
	
	if(NULL == GetDataManipulator())
		return;

	int nCount = (int)GetDataManipulator()->m_MeshTable.size();

	for(int i=0;i<nCount;i++)
	{
		Ogre::String name = GetDataManipulator()->m_MeshTable[i];
		wxTreeItemId item = AddLeaf(root, AS_STRING(name));
		mActorList->SetItemData(item, new ModelItemData(name,"",""));
	}

    mActorList->Thaw();
}

//////////////////////////////////////////////////////////////////////////


void ActorSelector::OnUpdateControls(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() != 0);
}


void ActorSelector::OnActorListSelChanged(wxTreeEvent& e)
{
	if(bInDel)
		return;

    wxTreeItemId item = e.GetItem();
    wxASSERT(item.IsOk());
}

void ActorSelector::OnTreeSelChanged(wxTreeEvent& e)
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
		ModelItemData* data = static_cast<ModelItemData*>(tree->GetItemData(item));
		if(data)
		{
			Fairy::String name = data->name;

			if (name.compare(GetDataManipulator()->m_CurMeshFile) == 0)
			{
				return;
			}

			if (m_pParentFrame)
			{
				m_pParentFrame->OpenSelFile(name,true);
			}
		}
	}
}
