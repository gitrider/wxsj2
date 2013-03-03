// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "ActorSelector.h"
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

#include "ActorSelector.h"

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
#include "Core/FairyActorObject.h"
#include <wx/notebook.h>

// ----------------------------------------------------------------------------
// ActorSelector class
// ----------------------------------------------------------------------------

#if wxUSE_STL
#define AS_STRING(s)    (s)
#else
#define AS_STRING(s)    ((s).c_str())
#endif

static const int feID_MODEL_LIST = wxNewId();

BEGIN_EVENT_TABLE(ActorSelector, wxPanel)
    EVT_UPDATE_UI(feID_MODEL_LIST, ActorSelector::OnUpdateControls)

    EVT_TREE_SEL_CHANGED(feID_MODEL_LIST, ActorSelector::OnActorListSelChanged)
    EVT_CHILD_FOCUS(ActorSelector::OnChildFocus)
END_EVENT_TABLE()

namespace {

    class ActorItemData : public wxTreeItemData
    {
    public:
        ActorItemData(const Fairy::String& name)
            : name(name)
        {
        }

        Fairy::String name;
    };
}

ActorSelector::~ActorSelector()
{
    if (mActorList)
    {
        mActorList->Freeze();
        mActorList->DeleteAllItems();
        mActorList->Thaw();
    }
}

void
ActorSelector::Init(const wxString& resourceGroup)
{
    mSceneManipulator = NULL;
    mActorList = NULL;

    mResourceGroup = resourceGroup;
	mFocused = false;
}

void
ActorSelector::SetSceneManipulator(Fairy::SceneManipulator* sceneManipulator)
{
    mSceneManipulator = sceneManipulator;
	mSceneManipulator->addSceneListener(this);
}

bool
ActorSelector::Create(wxWindow *parent, wxWindowID winid,
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
ActorSelector::Reload(const wxString& resourceGroup)
{
    mResourceGroup = resourceGroup;
    Reload();
}

bool
ActorSelector::CreateControls(void)
{
    wxFlexGridSizer* sizer = new wxFlexGridSizer(1);
    sizer->AddGrowableCol(0);
    sizer->AddGrowableRow(0);

    mActorList = new wxTreeCtrl(this, feID_MODEL_LIST,
        wxDefaultPosition, wxDefaultSize,
        wxTR_DEFAULT_STYLE | wxTR_SINGLE | wxTR_HIDE_ROOT);

    mActorList->AddRoot(wxT("Root"));

    sizer->Add(mActorList, 0, wxGROW|wxALL, 5);

    this->SetSizer(sizer);
    sizer->SetSizeHints(this);

    return true;
}

wxTreeItemId
ActorSelector::AddLeaf(const wxTreeItemId& root, const wxString& name)
{
    wxString::size_type pos = name.find_last_of(wxT('/'));
    if (pos != wxString::npos)
    {
        wxTreeItemId parent = AddBranch(root, name.substr(0, pos));
        return mActorList->AppendItem(parent, name.substr(pos+1, wxString::npos));
    }
    else
    {
		wxTreeItemId leaf = mActorList->AppendItem(root, name);
		mFileMap.insert(FileIdMap::value_type( name, leaf ));
		return leaf;
    }
}

wxTreeItemId
ActorSelector::AddBranch(const wxTreeItemId& root, const wxString& name)
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
ActorSelector::CreateChildItem(const wxTreeItemId& parent, const wxString& name)
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

void
ActorSelector::Reload(void)
{
    if (!Ogre::ResourceGroupManager::getSingletonPtr())
        return;

    mActorList->Freeze();

    wxTreeItemId root = mActorList->GetRootItem();
    mActorList->DeleteChildren(root);

	mFileMap.clear();

    wxString pattern = "*.obj";
	Ogre::FileInfoListPtr fileInfoList =
		Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(
		AS_STRING(mResourceGroup),
		AS_STRING(pattern));
	for (Ogre::FileInfoList::const_iterator it = fileInfoList->begin(); it != fileInfoList->end(); ++it)
	{
		const Fairy::String& name = it->filename;
		// Ìí¼ÓBranch
		wxString strBranch = it->archive->getName();
		wxString::size_type pos = strBranch.find_last_of("media");
		strBranch = strBranch.substr(pos+2, strBranch.length() - pos);
		wxTreeItemId itemBranch = AddBranch(root, strBranch);
		// Ìí¼ÓLeaf
		wxTreeItemId item = AddLeaf(itemBranch, AS_STRING(name));
		mActorList->SetItemData(item, new ActorItemData(name));
		//mActorList->SelectItem(item, true);
	}

    mActorList->Thaw();
}

//////////////////////////////////////////////////////////////////////////

void
ActorSelector::OnUpdateControls(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() != 0);
}

void
ActorSelector::OnActorListSelChanged(wxTreeEvent& e)
{
    wxTreeItemId item = e.GetItem();
    wxASSERT(item.IsOk());
	if (mFocused)
	{
		DoActiveItem(item);
	}
}

void
ActorSelector::OnChildFocus(wxChildFocusEvent& e)
{
    e.Skip();

    if (e.GetWindow() != mActorList)
        return;
	mFocused = true;

    wxTreeItemId item = mActorList->GetSelection();
    DoActiveItem(item);
}

void
ActorSelector::DoActiveItem(const wxTreeItemId& item)
{
    if (item.IsOk())
    {
        ActorItemData* data = static_cast<ActorItemData*>(mActorList->GetItemData(item));
        if (data)
        {
            const Fairy::String& name = data->name;
            Fairy::Action* action = GetSceneManipulator()->_getAction("CreateObjectAction");
            action->setParameter("type", "Actor");
            action->setParameter("%actor name", name);
			action->setParameter("has position", "false");
			
            GetSceneManipulator()->setActiveAction(action);
        }
    }
}

void ActorSelector::SelectItem(const wxString& itemName)
{
	FileIdMap::iterator it = mFileMap.find(itemName);
	if (it != mFileMap.end())
	{
		mActorList->SelectItem(it->second);
	}
}

void ActorSelector::onSelectObject(const Fairy::ObjectPtr& object)
{
	if ((int)mSceneManipulator->getSelectedObjects().size() == 1)
	{
		if (object->getType() == "Actor")
		{
			Fairy::ActorObject *actorObject = static_cast<Fairy::ActorObject *> (object.get());
			Ogre::String actorName = actorObject->getActorName();
			wxNotebook* notebook = (wxNotebook*)GetParent();
			notebook->SetSelection(1);
			mFocused = false;
			SelectItem(actorName.c_str());
		}
	}
}
