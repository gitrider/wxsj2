// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "ParticleSystemSelector.h"
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

#include <wx/splitter.h>
//#include <wx/treectrl.h>
#include <wx/listctrl.h>

#include "ParticleSystemSelector.h"
#include "EffectObjectPropertyEditor.h"
#include "WXEffectEditorFrame.h"
#include "Core/FairyObjectFactoryManager.h"
#include "Core/FairyPrerequisites.h"
#include "WXGraphDialog.h"
#include "OgreParticleSystemManager.h"
#include "EffectObjectEditor.h"
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
// ParticleSystemSelector class
// ----------------------------------------------------------------------------

#if wxUSE_STL
#define AS_STRING(s)    (s)
#else
#define AS_STRING(s)    ((s).c_str())
#endif

static const int feID_PARTICLESYSTEM_LIST = wxNewId();
static const int feID_RELOAD_PARTICLE_LIST =wxNewId();
static const int feID_STOP_PARTICLE =wxNewId();

BEGIN_EVENT_TABLE(ParticleSystemSelector, wxPanel)
EVT_UPDATE_UI(feID_PARTICLESYSTEM_LIST, ParticleSystemSelector::OnUpdateControls)

EVT_LIST_ITEM_SELECTED(feID_PARTICLESYSTEM_LIST, ParticleSystemSelector::OnParticleSystemListSelected)
EVT_COMMAND_LEFT_DCLICK(feID_PARTICLESYSTEM_LIST, ParticleSystemSelector::OnParticleSystemListLeftDoubleClicked)
EVT_MENU(feID_RELOAD_PARTICLE_LIST, ParticleSystemSelector::OnReloadParticleList)
EVT_MENU(feID_STOP_PARTICLE, ParticleSystemSelector::OnStopParticle)

END_EVENT_TABLE()

ParticleSystemSelector::~ParticleSystemSelector()
{
    if (mParticleSystemList)
    {
        mParticleSystemList->Freeze();
        mParticleSystemList->DeleteAllItems();
        mParticleSystemList->Thaw();
    }
}

void
ParticleSystemSelector::Init(const wxString& resourceGroup)
{
    mSceneManipulator = NULL;
    mParticleSystemList = NULL;
	m_Frame = NULL;
    mResourceGroup = resourceGroup;
}

void
ParticleSystemSelector::SetSceneManipulator(Fairy::SceneManipulator* sceneManipulator)
{
    mSceneManipulator = sceneManipulator;
}

bool
ParticleSystemSelector::Create(wxWindow *parent, wxWindowID winid,
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
ParticleSystemSelector::Reload(const wxString& resourceGroup)
{
    mResourceGroup = resourceGroup;
    Reload();
}

bool
ParticleSystemSelector::CreateControls(void)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(1);
	sizer->AddGrowableCol(0);
	sizer->AddGrowableRow(1);


	m_toolBar = new wxToolBar( this, wxID_ANY, wxDefaultPosition, wxSize(-1,28), wxTB_HORIZONTAL|wxNO_BORDER ); 
	m_toolBar->AddTool(feID_RELOAD_PARTICLE_LIST,
		wxBITMAP(RELOAD),wxBITMAP(RELOAD),false,
		0,wxDefaultCoord,NULL, "刷新粒子资源", "刷新粒子资源");

	m_toolBar->AddTool(feID_STOP_PARTICLE,
		wxBITMAP(EFFECTSTOP),wxBITMAP(EFFECTSTOP),false,
		1,wxDefaultCoord,NULL, "停止粒子播放", "停止粒子播放");


	m_toolBar->Realize();

	sizer->Add( m_toolBar, 0, wxALIGN_LEFT|wxALL, 0 );

	mParticleSystemList = new wxListCtrl(this, feID_PARTICLESYSTEM_LIST,
		wxDefaultPosition, wxDefaultSize,
		wxLC_LIST | wxLC_SINGLE_SEL,
		wxDefaultValidator);

	sizer->Add( mParticleSystemList, 0, wxGROW|wxALL, 0 );

	this->SetSizer(sizer);
	sizer->SetSizeHints(this);

    return true;
}

void
ParticleSystemSelector::Reload(void)
{
	if(mObject)
	{
		mSceneManipulator->deselectObject(mObject);
		mObject.reset();
	}
	
    if (!Ogre::ResourceGroupManager::getSingletonPtr())
        return;

    mParticleSystemList->Freeze();

    mParticleSystemList->ClearAll();
    
    Ogre::ParticleSystemManager::ParticleSystemTemplateIterator iterator = Ogre::ParticleSystemManager::getSingleton().getTemplateIterator();

    while ( iterator.hasMoreElements() )
    {
        // 获取到粒子系统的名称

		long index = mParticleSystemList->GetItemCount();
        mParticleSystemList->InsertItem( index, AS_STRING( iterator.peekNextKey() ) );
		mParticleSystemList->SetItemTextColour( index , *wxRED );
        // 使iterator往后移
        iterator.moveNext();
    }
    mParticleSystemList->Thaw();
}

//////////////////////////////////////////////////////////////////////////

void
ParticleSystemSelector::OnUpdateControls(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() != 0);
}
void ParticleSystemSelector::DestroySelectObject()
{
	if(mObject)
	{
		mSceneManipulator->deselectObject(mObject);
		mObject.reset();
	}
}
void
ParticleSystemSelector::OnParticleSystemListSelected(wxListEvent& e)
{
    long item = mParticleSystemList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (item >= 0)
    {
		wxColour itemColor = mParticleSystemList->GetItemTextColour(item);

		wxString name = mParticleSystemList->GetItemText(item);

		if(mObject)
		{
			mSceneManipulator->deselectObject(mObject);
			mObject.reset();
		}
		m_Frame->ClearAllSelectorObject();
		mParameters["particle system name"] = AS_STRING(name);
		mObject = Fairy::ObjectFactoryManager::getSingleton().createInstance("ParticleSystem");
		mObject->setPropertyList(mParameters);
		mObject->createRenderInstance(mSceneManipulator);

		Ogre::Vector3 position = Ogre::Vector3::ZERO;
		mObject->setProperty("position", position);
		mSceneManipulator->selectObject(mObject);

    }
}

void
ParticleSystemSelector::OnParticleSystemListLeftDoubleClicked(wxCommandEvent& e)
{
    long item = mParticleSystemList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (item >= 0)
    {
		wxColour itemColor = mParticleSystemList->GetItemTextColour(item);

		wxString name = mParticleSystemList->GetItemText(item);

		Ogre::ParticleSystem *pTemplate = Ogre::ParticleSystemManager::getSingleton().getTemplate(name.c_str());


		if(pTemplate)
		{
			m_Frame->ClearAllSelectorObject();
			m_Frame->HideAllEditorObject();
			m_Frame->GetEffectObjectEditor()->InitParticleEditor(pTemplate,name.c_str(),false);
			m_Frame->GetEditorNotebook()->SetSelection(0);
			m_Frame->GetBottomNotebook()->SetSelection(1);
		}
    }
}

void 
ParticleSystemSelector::OnReloadParticleList( wxCommandEvent &event )
{
	Reload();
}
void ParticleSystemSelector::OnStopParticle(wxCommandEvent &event)
{
	if(mObject)
	{
		mSceneManipulator->deselectObject(mObject);
		mObject.reset();
	}
}
