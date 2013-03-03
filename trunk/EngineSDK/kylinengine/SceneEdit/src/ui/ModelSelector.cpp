// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "ModelSelector.h"
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

#include "ModelSelector.h"

// ----------------------------------------------------------------------------
// Ogre headers
// ----------------------------------------------------------------------------

#include <OgreCodec.h>
#include <OgreResourceGroupManager.h>

// ----------------------------------------------------------------------------

#include "SceneManipulator.h"
#include "Action.h"
#include "Core/FairyStaticEntityObject.h"

#include <wx/notebook.h>

// ----------------------------------------------------------------------------
// ModelSelector class
// ----------------------------------------------------------------------------

#if wxUSE_STL
#define AS_STRING(s)    (s)
#else
#define AS_STRING(s)    ((s).c_str())
#endif

static const int feID_MODEL_LIST = wxNewId();

BEGIN_EVENT_TABLE(ModelSelector, wxPanel)
    EVT_UPDATE_UI(feID_MODEL_LIST, ModelSelector::OnUpdateControls)

    EVT_TREE_SEL_CHANGED(feID_MODEL_LIST, ModelSelector::OnModelListSelChanged)
    EVT_CHILD_FOCUS(ModelSelector::OnChildFocus)
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


ModelSelector::~ModelSelector()
{
    if (mModelList)
    {
        mModelList->Freeze();
        mModelList->DeleteAllItems();
        mModelList->Thaw();
    }
}

void
ModelSelector::Init(const wxString& resourceGroup)
{
    mSceneManipulator = NULL;
    mModelList = NULL;

    mResourceGroup = resourceGroup;
	mFocused = false;
}

void
ModelSelector::SetSceneManipulator(Fairy::SceneManipulator* sceneManipulator)
{
    mSceneManipulator = sceneManipulator;
	mSceneManipulator->addSceneListener(this);
}

bool
ModelSelector::Create(wxWindow *parent, wxWindowID winid,
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
ModelSelector::Reload(const wxString& resourceGroup)
{
    mResourceGroup = resourceGroup;
    Reload();
}

bool
ModelSelector::CreateControls(void)
{
    wxFlexGridSizer* sizer = new wxFlexGridSizer(1);
    sizer->AddGrowableCol(0);
    sizer->AddGrowableRow(0);

    mModelList = new wxTreeCtrl(this, feID_MODEL_LIST,
        wxDefaultPosition, wxDefaultSize,
        wxTR_DEFAULT_STYLE | wxTR_SINGLE | wxTR_HIDE_ROOT);

    mModelList->AddRoot(wxT("Root"));

    sizer->Add(mModelList, 0, wxGROW|wxALL, 5);

    this->SetSizer(sizer);
    sizer->SetSizeHints(this);

    return true;
}

wxTreeItemId
ModelSelector::AddLeaf(const wxTreeItemId& root, const wxString& name)
{
    wxString::size_type pos = name.find_last_of(wxT('/'));
    if (pos != wxString::npos)
    {
        wxTreeItemId parent = AddBranch(root, name.substr(0, pos));
        return mModelList->AppendItem(parent, name.substr(pos+1, wxString::npos));
    }
    else
    {
        wxTreeItemId leaf = mModelList->AppendItem(root, name);
		mFileMap.insert(FileIdMap::value_type( name, leaf ));
		return leaf;
    }
}

wxTreeItemId
ModelSelector::AddBranch(const wxTreeItemId& root, const wxString& name)
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
ModelSelector::CreateChildItem(const wxTreeItemId& parent, const wxString& name)
{
    wxTreeItemIdValue cookie;
    wxTreeItemId item = mModelList->GetFirstChild(parent, cookie);
    while (item.IsOk())
    {
        if (mModelList->GetItemText(item) == name)
            return item;
        item = mModelList->GetNextChild(parent, cookie);
    }

    return mModelList->AppendItem(parent, name);
}

void
ModelSelector::Reload(void)
{
    if (!Ogre::ResourceGroupManager::getSingletonPtr())
        return;

    mModelList->Freeze();

    wxTreeItemId root = mModelList->GetRootItem();
    mModelList->DeleteChildren(root);

	mFileMap.clear();
    addResources(root, "*.model", "Model", "%model name");
    addResources(root, "*.mesh", "StaticEntity", "%mesh name");

    mModelList->Thaw();
}

void
ModelSelector::addResources(const wxTreeItemId& root, const wxString& pattern, const wxString& typeName, const wxString& parameterName)
{
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
        mModelList->SetItemData(item, new ModelItemData(name, AS_STRING(typeName), AS_STRING(parameterName)));
		//mModelList->SelectItem(item, true);
    }
}

//////////////////////////////////////////////////////////////////////////

void
ModelSelector::OnUpdateControls(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() != 0);
}

void
ModelSelector::OnModelListSelChanged(wxTreeEvent& e)
{
    wxTreeItemId item = e.GetItem();
    wxASSERT(item.IsOk());
	if (mFocused)
	{
		DoActiveItem(item);
	}
}

void
ModelSelector::OnChildFocus(wxChildFocusEvent& e)
{
    e.Skip();

    if (e.GetWindow() != mModelList)
        return;

	mFocused = true;

    wxTreeItemId item = mModelList->GetSelection();
    DoActiveItem(item);
}

void
ModelSelector::DoActiveItem(const wxTreeItemId& item)
{
    if (item.IsOk())
    {
        ModelItemData* data = static_cast<ModelItemData*>(mModelList->GetItemData(item));
        if (data)
        {
            const Fairy::String& name = data->name;
            Fairy::Action* action = GetSceneManipulator()->_getAction("CreateObjectAction");
            action->setParameter("type", data->typeName);
			action->setParameter("has position", "false");
            action->setParameter(data->parameterName, name);
            GetSceneManipulator()->setActiveAction(action);
        }
    }
}

void ModelSelector::SelectItem(const wxString& itemName)
{
	FileIdMap::iterator it = mFileMap.find(itemName);
	if (it != mFileMap.end())
	{
		mModelList->SelectItem(it->second);
	}
}

void ModelSelector::onSelectObject(const Fairy::ObjectPtr& object)
{
	if ((int)mSceneManipulator->getSelectedObjects().size() == 1)
	{
		if(object->getType() == "StaticEntity")
		{
			Fairy::StaticEntityObject *staticEntityObject = static_cast<Fairy::StaticEntityObject *> (object.get());
			Ogre::String meshName = staticEntityObject->getMeshName();	
			wxNotebook* notebook = (wxNotebook*)GetParent();
			notebook->SetSelection(0);
			mFocused = false;
			SelectItem(meshName.c_str());
		}
	}
}
