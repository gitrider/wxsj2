
// For compilers that support precompilation, includes "wx/wx.h>.
#include <wx/wxprec.h>

#include <wx/treectrl.h>
#include "WXEffectEditorFrame.h"
#include "ActorSelector.h"
#include <wx/busyinfo.h>
// ----------------------------------------------------------------------------
// Ogre headers
// ----------------------------------------------------------------------------

#include <OgreCodec.h>
#include <OgreResourceGroupManager.h>

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
#include "ModelSystem/FairyLogicModelManager.h"
#include "ActorAnimationSettingEditor.h"
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

static const int feID_OBJECT_LIST = wxNewId();
static const int feID_RELOAD_D_OBJECT_LIST = wxNewId();

BEGIN_EVENT_TABLE(ActorSelector, wxPanel)
    EVT_UPDATE_UI(feID_OBJECT_LIST, ActorSelector::OnUpdateControls)
	EVT_MENU(feID_RELOAD_D_OBJECT_LIST,ActorSelector::OnReload)
	EVT_LIST_ITEM_SELECTED(feID_OBJECT_LIST, ActorSelector::OnObjectListSelected)
	EVT_COMMAND_LEFT_DCLICK(feID_OBJECT_LIST, ActorSelector::OnObjectListLeftDoubleClicked)
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
}

void
ActorSelector::SetSceneManipulator(Fairy::SceneManipulator* sceneManipulator)
{
    mSceneManipulator = sceneManipulator;
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
    sizer->AddGrowableRow(1);

	m_toolBar = new wxToolBar( this, wxID_ANY, wxDefaultPosition, wxSize(-1,28)); 

	m_toolBar->AddCheckTool(feID_RELOAD_D_OBJECT_LIST, _("刷新Object资源"),
		wxBITMAP(RELOAD),wxBITMAP(RELOAD), _("刷新Object资源"));
	m_toolBar->Realize();

	m_toolBar->Realize();
	sizer->Add( m_toolBar, 0, wxEXPAND, 0 );

	mActorList = new wxListCtrl(this, feID_OBJECT_LIST,
		wxDefaultPosition, wxDefaultSize,
		wxLC_LIST | wxLC_SINGLE_SEL,
		wxDefaultValidator);

    sizer->Add( mActorList, 0, wxGROW|wxALL, 0 );

    this->SetSizer(sizer);
    sizer->SetSizeHints(this);

    return true;
}

void
ActorSelector::Reload(void)
{
    if (!Ogre::ResourceGroupManager::getSingletonPtr())
        return;

	wxBusyInfo *busyInfo = new wxBusyInfo(_("正在读取角色数据 ..."), this);
	
    mActorList->Freeze();
	mActorList->ClearAll();

	Fairy::LogicModelManager::LogicModelTemplateIterator modelIt = 
		Fairy::LogicModelManager::getSingleton().getLogicModelTemplateIterator();

	while ( modelIt.hasMoreElements() )
	{
		long index = mActorList->GetItemCount();

		mActorList->InsertItem( index, AS_STRING( modelIt.peekNextKey() ) );

		modelIt.moveNext();
	}
    mActorList->Thaw();
	delete  busyInfo;
	busyInfo = NULL;
}

//////////////////////////////////////////////////////////////////////////

void
ActorSelector::OnUpdateControls(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() != 0);
}

void 
ActorSelector::OnReload(wxCommandEvent &e)
{
	Reload();
}

void ActorSelector::OnObjectListSelected(wxListEvent& e)
{

}
void ActorSelector::OnObjectListLeftDoubleClicked(wxCommandEvent& e)
{
	long item = mActorList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	if (item >= 0)
	{
		wxString name = mActorList->GetItemText(item);

		m_Frame->ClearAllSelectorObject();
			//m_Frame->HideAllEditorObject();
		m_Frame->GetActorSettingEditor()->SetCurrentObject(Ogre::String(name.c_str()));
	}
}

