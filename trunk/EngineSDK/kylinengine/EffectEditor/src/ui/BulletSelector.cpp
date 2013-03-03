// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------
// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)

// For compilers that support precompilation, includes "wx/wx.h>.
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#endif

#include <wx/splitter.h>
//#include <wx/treectrl.h>
#include <wx/listctrl.h>

#include "BulletSelector.h"
#include "EffectObjectPropertyEditor.h"
#include "BulletEditor.h"
#include "WXEffectEditorFrame.h"
#include "Core/FairyEffectObject.h"
#include "Core/FairyObjectFactoryManager.h"
#include "EffectSystem/FairyBulletFlowSystemManager.h"
#include "EffectSystem/FairyBulletFlowSystem.h"
// ----------------------------------------------------------------------------
// Ogre headers
// ----------------------------------------------------------------------------

#include <OgreParticleSystemManager.h>

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------

#include "SceneManipulator.h"
#include "Action.h"
#include "EffectSystem\FairyEffectManager.h"

// ----------------------------------------------------------------------------
// BulletSelector class
// ----------------------------------------------------------------------------

#if wxUSE_STL
#define AS_STRING(s)    (s)
#else
#define AS_STRING(s)    ((s).c_str())
#endif

static const int feID_BULLET_LIST = wxNewId();
static const int feID_RELOAD_BULLET_LIST =wxNewId();

BEGIN_EVENT_TABLE(BulletSelector, wxPanel)
EVT_UPDATE_UI(feID_BULLET_LIST, BulletSelector::OnUpdateControls)
EVT_LIST_ITEM_SELECTED(feID_BULLET_LIST, BulletSelector::OnBulletListSelected)
EVT_COMMAND_LEFT_DCLICK(feID_BULLET_LIST, BulletSelector::OnBulletListLeftDoubleClicked)
EVT_MENU(feID_RELOAD_BULLET_LIST, BulletSelector::OnReloadBulletList)

END_EVENT_TABLE()

BulletSelector::~BulletSelector()
{
    if (mBulletFlowList)
    {
        mBulletFlowList->Freeze();
        mBulletFlowList->DeleteAllItems();
        mBulletFlowList->Thaw();
    }
}

void
BulletSelector::Init(const wxString& resourceGroup)
{
    mSceneManipulator = NULL;
    mBulletFlowList = NULL;
	m_Frame = NULL;
    mResourceGroup = resourceGroup;
}

void
BulletSelector::SetSceneManipulator(Fairy::SceneManipulator* sceneManipulator)
{
    mSceneManipulator = sceneManipulator;
}

bool
BulletSelector::Create(wxWindow *parent, wxWindowID winid,
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
BulletSelector::Reload(const wxString& resourceGroup)
{
    mResourceGroup = resourceGroup;
    Reload();
}

bool
BulletSelector::CreateControls(void)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(1);
	sizer->AddGrowableCol(0);
	sizer->AddGrowableRow(1);


	m_toolBar = new wxToolBar( this, wxID_ANY, wxDefaultPosition, wxSize(-1,26)); 
	m_toolBar->AddCheckTool(feID_RELOAD_BULLET_LIST, _("刷新子弹资源"),
		wxBITMAP(RELOAD),wxBITMAP(RELOAD), _("刷新子弹资源"));
	m_toolBar->Realize();

	sizer->Add( m_toolBar, 0, wxALIGN_LEFT|wxALL, 0 );

	mBulletFlowList = new wxListCtrl(this, feID_BULLET_LIST,
		wxDefaultPosition, wxDefaultSize,
		wxLC_LIST | wxLC_SINGLE_SEL,
		wxDefaultValidator);

	sizer->Add( mBulletFlowList, 0, wxGROW|wxALL, 0 );

	this->SetSizer(sizer);
	sizer->SetSizeHints(this);

    return true;
}

void
BulletSelector::Reload(void)
{
    if (!Ogre::ResourceGroupManager::getSingletonPtr())
        return;

    mBulletFlowList->Freeze();

    mBulletFlowList->ClearAll();
    
	// 添加特效
	// 填充现在的bullet模板的列表
	Fairy::BulletFlowSystemManager::BulletFlowTemplateIterator it =
		Fairy::BulletFlowSystemManager::getSingleton().getBulletFlowTemplateIterator();
	while ( it.hasMoreElements() )
	{
		long index = mBulletFlowList->GetItemCount();

		mBulletFlowList->InsertItem( index, AS_STRING( it.peekNextKey() ) );

		if (it.peekNextKey() == "BulletEmpty")
			mBulletFlowList->SetItemTextColour(index, *wxGREEN);

		it.moveNext();
	}

    mBulletFlowList->Thaw();
}

//////////////////////////////////////////////////////////////////////////

void
BulletSelector::OnUpdateControls(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() != 0);
}

void BulletSelector::DestroySelectObject()
{
	if(mObject)
	{
		mSceneManipulator->deselectObject(mObject);
		mObject.reset();
	}
}
void
BulletSelector::OnBulletListSelected(wxListEvent& e)
{
    long item = mBulletFlowList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (item >= 0)
    {

		wxString name = mBulletFlowList->GetItemText(item);
		/*if(mObject)
		{
			mSceneManipulator->deselectObject(mObject);
			mObject.reset();
		}
		m_Frame->ClearAllSelectorObject();
		mParameters["effect name"] = AS_STRING(name);
		mObject = Fairy::ObjectFactoryManager::getSingleton().createInstance("Effect");
		mObject->setPropertyList(mParameters);
		mObject->createRenderInstance(mSceneManipulator);

		Ogre::Vector3 position = Ogre::Vector3::ZERO;
		mObject->setProperty("position", position);
		mSceneManipulator->selectObject(mObject);*/
    }
}

void
BulletSelector::OnBulletListLeftDoubleClicked(wxCommandEvent& e)
{
    long item = mBulletFlowList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (item >= 0)
    {
		Ogre::String name = mBulletFlowList->GetItemText(item).c_str();

		Fairy::BulletFlowSystem *pTemplate = Fairy::BulletFlowSystemManager::getSingleton().getBulletFlowSystemTemplate(name);

		if (pTemplate)
		{
			m_Frame->ClearAllSelectorObject();
			m_Frame->HideAllEditorObject();
			BulletEditor* pBulletEditor = m_Frame->GetBulletEditor();
			if(pBulletEditor)
				pBulletEditor->InitBulletEditor(pTemplate,name,true);
			m_Frame->GetEditorNotebook()->SetSelection(3);
			m_Frame->GetBottomNotebook()->SetSelection(1);
		}
    }
}

void 
BulletSelector::OnReloadBulletList( wxCommandEvent &event )
{
	Reload();
}

