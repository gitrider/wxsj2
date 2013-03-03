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

#include "SkillSelector.h"
#include "EffectObjectPropertyEditor.h"
#include "ActorAnimationSettingEditor.h"
#include "WXEffectEditorFrame.h"
#include "WXGraphDialog.h"
#include "Core/FairyObjectFactoryManager.h"
#include "SkillObjectEditor.h"
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
// SkillSelector class
// ----------------------------------------------------------------------------

#if wxUSE_STL
#define AS_STRING(s)    (s)
#else
#define AS_STRING(s)    ((s).c_str())
#endif

static const int feID_SKILL_LIST = wxNewId();
static const int feID_RELOAD_SKILL_LIST =wxNewId();

BEGIN_EVENT_TABLE(SkillSelector, wxPanel)
EVT_UPDATE_UI(feID_SKILL_LIST, SkillSelector::OnUpdateControls)

EVT_LIST_ITEM_SELECTED(feID_SKILL_LIST, SkillSelector::OnEffectListSelected)
EVT_COMMAND_LEFT_DCLICK(feID_SKILL_LIST, SkillSelector::OnSkillListLeftDoubleClicked)
EVT_BUTTON(feID_RELOAD_SKILL_LIST, SkillSelector::OnReloadEffectList)

END_EVENT_TABLE()

SkillSelector::~SkillSelector()
{
    if (mSkillList)
    {
        mSkillList->Freeze();
        mSkillList->DeleteAllItems();
        mSkillList->Thaw();
    }
}

void
SkillSelector::Init(const wxString& resourceGroup)
{
    mSceneManipulator = NULL;
    mSkillList = NULL;
	m_Frame = NULL;
    mResourceGroup = resourceGroup;
}

void
SkillSelector::SetSceneManipulator(Fairy::SceneManipulator* sceneManipulator)
{
    mSceneManipulator = sceneManipulator;
}

bool
SkillSelector::Create(wxWindow *parent, wxWindowID winid,
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
SkillSelector::Reload(const wxString& resourceGroup)
{
    mResourceGroup = resourceGroup;
    Reload();
}

bool
SkillSelector::CreateControls(void)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(1);
	sizer->AddGrowableCol(0);
	sizer->AddGrowableRow(1);


	m_toolBar = new wxToolBar( this, wxID_ANY, wxDefaultPosition, wxSize(-1,28)); 
	m_toolBar->AddCheckTool(feID_RELOAD_SKILL_LIST, _("刷新效果资源"),
		wxBITMAP(RELOAD),wxBITMAP(RELOAD), _("刷新效果资源"));
	m_toolBar->Realize();

	sizer->Add( m_toolBar, 0, wxALIGN_LEFT|wxALL, 0 );

	mSkillList = new wxListCtrl(this, feID_SKILL_LIST,
		wxDefaultPosition, wxDefaultSize,
		wxLC_LIST | wxLC_SINGLE_SEL,
		wxDefaultValidator);

	sizer->Add( mSkillList, 0, wxGROW|wxALL, 0 );

	this->SetSizer(sizer);
	sizer->SetSizeHints(this);

    return true;
}

void
SkillSelector::Reload(void)
{
    if (!Ogre::ResourceGroupManager::getSingletonPtr())
        return;

    mSkillList->Freeze();

    mSkillList->ClearAll();
    
	// 添加特效
	// 填充现在的skill模板的列表
	Fairy::EffectManager::SkillIterator it = 
		Fairy::EffectManager::getSingleton().getSkillIterator();

	while ( it.hasMoreElements() )
	{
		long index = mSkillList->GetItemCount();

		mSkillList->InsertItem( index, AS_STRING( it.peekNextKey() ) );
		mSkillList->SetItemTextColour( index , *wxBLUE );

		it.moveNext();
	}

    mSkillList->Thaw();
}

//////////////////////////////////////////////////////////////////////////

void
SkillSelector::OnUpdateControls(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() != 0);
}

void
SkillSelector::OnEffectListSelected(wxListEvent& e)
{
    long item = mSkillList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (item >= 0)
    {
		/*if(mObject)
		{
			mSceneManipulator->deselectObject(mObject);
			mObject.reset();
		}
		m_Frame->ClearAllSelectorObject();
		//mParameters["particle system name"] = AS_STRING(name);
		mObject = Fairy::ObjectFactoryManager::getSingleton().createInstance("Skill");
		mObject->setPropertyList(mParameters);
		mObject->createRenderInstance(mSceneManipulator);

		Ogre::Vector3 position = Ogre::Vector3::ZERO;
		mObject->setProperty("position", position);
		mSceneManipulator->selectObject(mObject);*/
    }
}

void
SkillSelector::OnSkillListLeftDoubleClicked(wxCommandEvent& e)
{
    long item = mSkillList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (item >= 0)
    {

		wxString name = mSkillList->GetItemText(item);
		Fairy::Skill* pSkill = Fairy::EffectManager::getSingleton().getSkill(name.c_str());
		if(!pSkill)
			return;

		m_Frame->GetGraphDialog()->InitAnimTimePosSlider();
		m_Frame->ClearAllSelectorObject();
		m_Frame->HideAllEditorObject();
		
		m_Frame->GetSkillObjectEidtor()->InitSkillEditor(pSkill,Ogre::String(name.c_str()));
		m_Frame->GetEditorNotebook()->SetSelection(1);
		m_Frame->GetBottomNotebook()->SetSelection(1);
    }
}

void 
SkillSelector::OnReloadEffectList( wxCommandEvent &event )
{
	Reload();
}

