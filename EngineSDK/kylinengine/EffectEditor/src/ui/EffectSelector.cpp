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

#include "EffectSelector.h"
#include "EffectObjectPropertyEditor.h"
#include "EffectObjectEditor.h"
#include "WXEffectEditorFrame.h"
#include "Core/FairyEffectObject.h"
#include "Core/FairyObjectFactoryManager.h"
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
// EffectSelector class
// ----------------------------------------------------------------------------

#if wxUSE_STL
#define AS_STRING(s)    (s)
#else
#define AS_STRING(s)    ((s).c_str())
#endif

static const int feID_EFFECT_LIST = wxNewId();
static const int feID_RELOAD_EFFECT_LIST =wxNewId();

BEGIN_EVENT_TABLE(EffectSelector, wxPanel)
EVT_UPDATE_UI(feID_EFFECT_LIST, EffectSelector::OnUpdateControls)
EVT_LIST_ITEM_SELECTED(feID_EFFECT_LIST, EffectSelector::OnEffectListSelected)
EVT_COMMAND_LEFT_DCLICK(feID_EFFECT_LIST, EffectSelector::OnEffectListLeftDoubleClicked)
EVT_MENU(feID_RELOAD_EFFECT_LIST, EffectSelector::OnReloadEffectList)

END_EVENT_TABLE()

EffectSelector::~EffectSelector()
{
    if (mEffectList)
    {
        mEffectList->Freeze();
        mEffectList->DeleteAllItems();
        mEffectList->Thaw();
    }
}

void
EffectSelector::Init(const wxString& resourceGroup)
{
    mSceneManipulator = NULL;
    mEffectList = NULL;
	m_Frame = NULL;
    mResourceGroup = resourceGroup;
}

void
EffectSelector::SetSceneManipulator(Fairy::SceneManipulator* sceneManipulator)
{
    mSceneManipulator = sceneManipulator;
}

bool
EffectSelector::Create(wxWindow *parent, wxWindowID winid,
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
EffectSelector::Reload(const wxString& resourceGroup)
{
    mResourceGroup = resourceGroup;
    Reload();
}

bool
EffectSelector::CreateControls(void)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(1);
	sizer->AddGrowableCol(0);
	sizer->AddGrowableRow(1);


	m_toolBar = new wxToolBar( this, wxID_ANY, wxDefaultPosition, wxSize(-1,26)); 
	m_toolBar->AddCheckTool(feID_RELOAD_EFFECT_LIST, _("刷新效果资源"),
		wxBITMAP(RELOAD),wxBITMAP(RELOAD), _("刷新效果资源"));
	m_toolBar->Realize();

	sizer->Add( m_toolBar, 0, wxALIGN_LEFT|wxALL, 0 );

	mEffectList = new wxListCtrl(this, feID_EFFECT_LIST,
		wxDefaultPosition, wxDefaultSize,
		wxLC_LIST | wxLC_SINGLE_SEL,
		wxDefaultValidator);

	sizer->Add( mEffectList, 0, wxGROW|wxALL, 0 );

	this->SetSizer(sizer);
	sizer->SetSizeHints(this);

    return true;
}

void
EffectSelector::Reload(void)
{
    if (!Ogre::ResourceGroupManager::getSingletonPtr())
        return;

    mEffectList->Freeze();

    mEffectList->ClearAll();
    
	// 添加特效
	// 填充现在的effect模板的列表
	Fairy::EffectManager::EffectTemplateIterator it = 
		Fairy::EffectManager::getSingleton().getEffectTemplateIterator();

	while ( it.hasMoreElements() )
	{
		long index = mEffectList->GetItemCount();

		mEffectList->InsertItem( index, AS_STRING( it.peekNextKey() ) );

		if (it.peekNextKey() == "EffectEmpty")
			mEffectList->SetItemTextColour(index, *wxGREEN);

		it.moveNext();
	}

    mEffectList->Thaw();
}

//////////////////////////////////////////////////////////////////////////

void
EffectSelector::OnUpdateControls(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() != 0);
}

void EffectSelector::DestroySelectObject()
{
	if(mObject)
	{
		mSceneManipulator->deselectObject(mObject);
		mObject.reset();
	}
}
void
EffectSelector::OnEffectListSelected(wxListEvent& e)
{
    long item = mEffectList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (item >= 0)
    {

		wxString name = mEffectList->GetItemText(item);
		if(mObject)
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
		mSceneManipulator->selectObject(mObject);
    }
}

void
EffectSelector::OnEffectListLeftDoubleClicked(wxCommandEvent& e)
{
    long item = mEffectList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (item >= 0)
    {
		wxString name = mEffectList->GetItemText(item);

		Fairy::Effect *pTemplate = Fairy::EffectManager::getSingleton().getTemplate(name.c_str());

		if (pTemplate)
		{
			m_Frame->ClearAllSelectorObject();
			m_Frame->HideAllEditorObject();
			m_Frame->GetEffectObjectEditor()->InitEffectEditor(pTemplate,Ogre::String(name.c_str()));
			m_Frame->GetEditorNotebook()->SetSelection(0);
			m_Frame->GetBottomNotebook()->SetSelection(1);
		}
	
    }
}

void 
EffectSelector::OnReloadEffectList( wxCommandEvent &event )
{
	Reload();
}

