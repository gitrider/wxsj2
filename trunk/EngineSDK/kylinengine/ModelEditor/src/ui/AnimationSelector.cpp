// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "AnimationSelector.h"
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

#include "AnimationSelector.h"
#include <OgreStringConverter.h>

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
#include "DataManipulator.h"
#include "Core/FairyStaticEntityObject.h"
#include "OgreEntity.h"
// ----------------------------------------------------------------------------
// ActorSelector class
// ----------------------------------------------------------------------------

#if wxUSE_STL
#define AS_STRING(s)    (s)
#else
#define AS_STRING(s)    ((s).c_str())
#endif

static const int feID_ANIMATION_LIST = wxNewId();
static const int ID_BTN_PLAY = wxNewId();
static const int ID_BTN_PAUSE = wxNewId();
static const int ID_SLIDER = wxNewId();
static const int ID_FUSE = wxNewId();

BEGIN_EVENT_TABLE(AnimationSelector, wxPanel)
    EVT_UPDATE_UI(feID_ANIMATION_LIST, AnimationSelector::OnUpdateControls)

    EVT_TREE_SEL_CHANGED(feID_ANIMATION_LIST, AnimationSelector::OnAnimationListSelChanged)

	EVT_BUTTON(ID_BTN_PLAY, AnimationSelector::OnBtn)
	EVT_BUTTON(ID_BTN_PAUSE,  AnimationSelector::OnBtn)

	EVT_SLIDER(ID_SLIDER,  AnimationSelector::OnSliderUpdate)

	EVT_TEXT_ENTER(ID_FUSE, AnimationSelector::OnFuseChange)
	//EVT_TEXT(ID_FUSE, AnimationSelector::OnFuseChange)

END_EVENT_TABLE()

namespace {

    class AnimationItemData : public wxTreeItemData
    {
    public:
        AnimationItemData(const Fairy::String& name)
            : name(name)
        {
        }

        Fairy::String name;
    };
}

AnimationSelector::~AnimationSelector()
{
    if (mAnimationList)
    {
        mAnimationList->Freeze();
        mAnimationList->DeleteAllItems();
        mAnimationList->Thaw();
    }
}

void
AnimationSelector::Init(const wxString& resourceGroup)
{
    mSceneManipulator = NULL;
    mAnimationList = NULL;

    mResourceGroup = resourceGroup;

}

void
AnimationSelector::SetSceneManipulator(Fairy::SceneManipulator* sceneManipulator)
{
    mSceneManipulator = sceneManipulator;
}

bool
AnimationSelector::Create(wxWindow *parent, wxWindowID winid,
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
AnimationSelector::Reload(const wxString& resourceGroup)
{
    mResourceGroup = resourceGroup;
    Reload();
}

bool
AnimationSelector::CreateControls(void)
{
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    mAnimationList = new wxTreeCtrl(this, feID_ANIMATION_LIST,
        wxDefaultPosition, wxDefaultSize,
        wxTR_DEFAULT_STYLE | wxTR_SINGLE | wxTR_HIDE_ROOT);

    mAnimationList->AddRoot(wxT("Root"));

    sizer->Add(mAnimationList,1, wxGROW|wxALL, 5);

	// Add the control panel
	wxBoxSizer* m_controlSizer;
	m_controlSizer = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* m_sliderSizer;
	m_sliderSizer = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* m_btnSizer;
	m_btnSizer = new wxBoxSizer( wxHORIZONTAL );

	m_btnPlay = new wxButton( this, ID_BTN_PLAY, wxT("Play"), wxDefaultPosition, wxDefaultSize, 0 );
	m_btnSizer->Add( m_btnPlay, 0, wxALIGN_CENTER|wxALL, 5 );

	m_btnPause = new wxButton( this, ID_BTN_PAUSE, wxT("Pause"), wxDefaultPosition, wxDefaultSize, 0 );
	m_btnSizer->Add( m_btnPause, 0, wxALIGN_CENTER|wxALL, 5 );

	wxBoxSizer* m_fuseSizer;
	m_fuseSizer = new wxBoxSizer( wxHORIZONTAL );

	m_staticTextFuse = new wxStaticText( this, wxID_ANY, wxT("Fuse(s)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_fuseSizer->Add( m_staticTextFuse, 0, wxALIGN_CENTER|wxALL, 5 );

	m_fuseValue = new wxTextCtrl( this, ID_FUSE, wxT("0"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	m_fuseSizer->Add( m_fuseValue, 1, wxALL, 5 );

	m_btnSizer->Add( m_fuseSizer, 0, wxALIGN_CENTER|wxALL, 5 );


	m_sliderSizer->Add( m_btnSizer, 1, wxEXPAND, 5 );

	m_slider = new wxSlider( this, ID_SLIDER, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS|wxSL_HORIZONTAL );
	m_sliderSizer->Add( m_slider, 1, wxALL|wxEXPAND, 5 );

	m_controlSizer->Add( m_sliderSizer, 1, wxEXPAND, 5 );

	wxBoxSizer* m_infoSizer;
	m_infoSizer = new wxBoxSizer( wxHORIZONTAL );

	wxGridSizer* m_innerInfoSizer;
	m_innerInfoSizer = new wxGridSizer( 1, 4, 0, 0 );

	wxBoxSizer* m_titleSizer;
	m_titleSizer = new wxBoxSizer( wxVERTICAL );

	m_staticTextBlank = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_titleSizer->Add( m_staticTextBlank, 0, wxALL, 5 );

	m_startTimeLabel = new wxStaticText( this, wxID_ANY, wxT("Time(s)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_titleSizer->Add( m_startTimeLabel, 1, wxALL, 5 );

	m_startFrameLabel = new wxStaticText( this, wxID_ANY, wxT("Frame(f)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_titleSizer->Add( m_startFrameLabel, 1, wxALL, 5 );

	m_innerInfoSizer->Add( m_titleSizer, 1, wxEXPAND, 5 );

	wxBoxSizer* m_startSizer;
	m_startSizer = new wxBoxSizer( wxVERTICAL );

	m_startLabel = new wxStaticText( this, wxID_ANY, wxT("Start"), wxDefaultPosition, wxDefaultSize, 0 );
	m_startSizer->Add( m_startLabel, 0, wxALL, 5 );

	wxBoxSizer* m_startTimeSizer;
	m_startTimeSizer = new wxBoxSizer( wxHORIZONTAL );

	m_startTimeText = new wxTextCtrl( this, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	m_startTimeSizer->Add( m_startTimeText, 1, wxALL, 5 );

	m_startSizer->Add( m_startTimeSizer, 1, wxEXPAND, 5 );

	wxBoxSizer* m_startFrameSizer;
	m_startFrameSizer = new wxBoxSizer( wxHORIZONTAL );

	m_startFrameText = new wxTextCtrl( this, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	m_startFrameSizer->Add( m_startFrameText, 1, wxALL, 5 );

	m_startSizer->Add( m_startFrameSizer, 1, wxEXPAND, 5 );

	m_innerInfoSizer->Add( m_startSizer, 1, wxALL|wxEXPAND, 5 );

	wxBoxSizer* m_curSizer;
	m_curSizer = new wxBoxSizer( wxVERTICAL );

	m_curLabel = new wxStaticText( this, wxID_ANY, wxT("Current"), wxDefaultPosition, wxDefaultSize, 0 );
	m_curSizer->Add( m_curLabel, 0, wxALL, 5 );

	wxBoxSizer* m_curTimeSizer;
	m_curTimeSizer = new wxBoxSizer( wxHORIZONTAL );

	m_curTimeText = new wxTextCtrl( this, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	m_curTimeSizer->Add( m_curTimeText, 1, wxALL, 5 );

	m_curSizer->Add( m_curTimeSizer, 1, wxEXPAND, 5 );

	wxBoxSizer* m_curFrameSizer;
	m_curFrameSizer = new wxBoxSizer( wxHORIZONTAL );

	m_curFrameText = new wxTextCtrl( this, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	m_curFrameSizer->Add( m_curFrameText, 1, wxALL, 5 );

	m_curSizer->Add( m_curFrameSizer, 1, wxEXPAND, 5 );

	m_innerInfoSizer->Add( m_curSizer, 1, wxALL|wxEXPAND, 5 );

	wxBoxSizer* m_endSizer;
	m_endSizer = new wxBoxSizer( wxVERTICAL );

	m_endLabel = new wxStaticText( this, wxID_ANY, wxT("end"), wxDefaultPosition, wxDefaultSize, 0 );
	m_endSizer->Add( m_endLabel, 0, wxALL, 5 );

	wxBoxSizer* m_endTimeSizer;
	m_endTimeSizer = new wxBoxSizer( wxHORIZONTAL );

	m_endTimeText = new wxTextCtrl( this, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	m_endTimeSizer->Add( m_endTimeText, 1, wxALL, 5 );

	m_endSizer->Add( m_endTimeSizer, 1, wxEXPAND, 5 );

	wxBoxSizer* m_endFrameSizer;
	m_endFrameSizer = new wxBoxSizer( wxHORIZONTAL );

	m_endFrameText = new wxTextCtrl( this, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	m_endFrameSizer->Add( m_endFrameText, 1, wxALL, 5 );

	m_endSizer->Add( m_endFrameSizer, 1, wxEXPAND, 5 );

	m_innerInfoSizer->Add( m_endSizer, 1, wxALL|wxEXPAND, 5 );

	m_infoSizer->Add( m_innerInfoSizer, 1, wxEXPAND, 5 );

	m_controlSizer->Add( m_infoSizer, 2, wxEXPAND, 5 );

	 sizer->Add(m_controlSizer, 0, wxGROW|wxALL, 5);

	 m_btnPlay->Enable(false);
	 m_btnPause->Enable(false);

    this->SetSizer(sizer);
    sizer->SetSizeHints(this);

    return true;
}

wxTreeItemId
AnimationSelector::AddLeaf(const wxTreeItemId& root, const wxString& name)
{
    wxString::size_type pos = name.find_last_of(wxT('/'));
    if (pos != wxString::npos)
    {
        wxTreeItemId parent = AddBranch(root, name.substr(0, pos));
        return mAnimationList->AppendItem(parent, name.substr(pos+1, wxString::npos));
    }
    else
    {
        return mAnimationList->AppendItem(root, name);
    }
}

wxTreeItemId
AnimationSelector::AddBranch(const wxTreeItemId& root, const wxString& name)
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
AnimationSelector::CreateChildItem(const wxTreeItemId& parent, const wxString& name)
{
    wxTreeItemIdValue cookie;
    wxTreeItemId item = mAnimationList->GetFirstChild(parent, cookie);
    while (item.IsOk())
    {
        if (mAnimationList->GetItemText(item) == name)
            return item;
        item = mAnimationList->GetNextChild(parent, cookie);
    }

    return mAnimationList->AppendItem(parent, name);
}

void
AnimationSelector::Reload(void)
{
	resetState();

	if (!Ogre::ResourceGroupManager::getSingletonPtr())
		return;
	if(NULL == GetDataManipulator())
		return;

	mAnimationList->Freeze();

	wxTreeItemId root = mAnimationList->GetRootItem();

	mAnimationList->Unselect();

	mAnimationList->DeleteChildren(root);

	if (mIsMeshAnim)
	{
		// Get animation list of current mesh	
		Fairy::ObjectPtr pCurMesh = GetDataManipulator()->m_pMeshObject;
		if (!pCurMesh)
		{
			mAnimationList->Thaw();
			return;
		}

		Fairy::StaticEntityObject* entity = static_cast<Fairy::StaticEntityObject*>((pCurMesh).get());
		Ogre::Entity* pCurEntity = entity->getEntity();
		if(pCurEntity && pCurEntity->hasSkeleton())
		{	
			//更新动画列表
			unsigned short numAnim = pCurEntity->getSkeleton()->getNumAnimations();
			for(int i=0;i<numAnim;++i)
			{
				Ogre::String AniName = pCurEntity->getSkeleton()->getAnimation(i)->getName();
				wxTreeItemId item = AddLeaf(root, AniName);
				mAnimationList->SetItemData(item, new AnimationItemData(AniName));
			}
		}
		mAnimationList->Thaw();
	}
	else
	{
		Fairy::LogicModel* pObjModel = GetDataManipulator()->m_pObjTemplate;
		if (!pObjModel)
		{
			mAnimationList->Thaw();
			return;
		}

		Fairy::LogicModelManager::AnimationSkeletonLinks* pAniLinks = GetDataManipulator()->_getCurAniLinks();
		if (!pAniLinks)
		{
			mAnimationList->Thaw();
			return;
		}

		for ( Fairy::LogicModelManager::AnimationSkeletonLinks::iterator iter = pAniLinks->begin(); iter!= pAniLinks->end(); ++iter)
		{
			Ogre::String AniName(iter->first);
			wxTreeItemId item = AddLeaf(root, AniName);
			mAnimationList->SetItemData(item, new AnimationItemData(AniName));
		}

		mAnimationList->Thaw();
	}
}

bool AnimationSelector::hasSelectedItem()
{
	if (mAnimationList)
	{
		wxTreeItemId id = mAnimationList->GetSelection();
		if (id)
		{
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////

void
AnimationSelector::OnUpdateControls(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() != 0);
}

void AnimationSelector::resetState()
{
	m_bPause = false;

	m_btnPause->Enable(false);
	m_btnPlay->Enable(false);

	m_endTimeText->SetValue(wxT("0"));
	m_endFrameText->SetValue(wxT("0"));

	m_curTimeText->SetValue(wxT("0"));
	m_curFrameText->SetValue(wxT("0"));

	m_slider->SetValue(0);
}

void AnimationSelector::pauseAnimation(bool bPause)
{
	if (!GetDataManipulator())
	{
		return;
	}

	Fairy::LogicModel* pObjModel = GetDataManipulator()->m_pObjTemplate;

	if ( !pObjModel || pObjModel->getCurrentAnimName().empty())
	{
		return;
	}	

	if (!bPause)
	{
		if (!m_bPause)
		{
			return;
		}
	}
	else
	{
		if (m_bPause)
		{
			return;
		}
	}

	m_bPause = !m_bPause;

	m_btnPause->Enable(!m_bPause);
	m_btnPlay->Enable(m_bPause);

	if ( pObjModel && !pObjModel->getCurrentAnimName().empty())
	{
		pObjModel->setAnimEditing(m_bPause);
	}	
}

void
AnimationSelector::OnAnimationListSelChanged(wxTreeEvent& e)
{
	// reset the state
	resetState();

    wxTreeItemId item = e.GetItem();

    wxASSERT(item.IsOk());

	wxTreeCtrl* tree = wxStaticCast(e.GetEventObject(), wxTreeCtrl);
    mActivedItem = e.GetItem();

    wxArrayTreeItemIds selections;
    int nCount = tree->GetSelections(selections);

	if(nCount <= 0)
		return;

	if (mIsMeshAnim)
	{
		GetDataManipulator()->changeMeshAnimation(DoActiveItem(item));
		return;
	}

	GetDataManipulator()->changeAnimation(DoActiveItem(item),0.0);

	Fairy::LogicModel* pObjModel = GetDataManipulator()->m_pObjTemplate;

	if ( pObjModel && !pObjModel->getCurrentAnimName().empty())
	{
		
		m_btnPause->Enable(true);
 
 		pObjModel->setAnimEditing(m_bPause);

		Ogre::String animationName = pObjModel->getCurrentAnimName();

		bool bLoop = pObjModel->getAnimationLoop();
		Ogre::Real timeTotal = pObjModel->getAnimationLength(animationName);

		m_endTimeText->SetValue(Ogre::StringConverter::toString(timeTotal));
		m_endFrameText->SetValue(Ogre::StringConverter::toString(timeTotal*STATIC_FPS));
	}
}

void AnimationSelector::OnBtn(wxCommandEvent & e)
{
	if (e.GetId() == ID_BTN_PLAY)
	{
		if (!m_bPause)
		{
			return;
		}
	}
	else
	{
		if (m_bPause)
		{
			return;
		}
	}

	m_bPause = !m_bPause;

	m_btnPause->Enable(!m_bPause);
	m_btnPlay->Enable(m_bPause);

	Fairy::LogicModel* pObjModel = GetDataManipulator()->m_pObjTemplate;

	if ( pObjModel && !pObjModel->getCurrentAnimName().empty())
	{
		pObjModel->setAnimEditing(m_bPause);
	}	
}

void AnimationSelector::OnSliderUpdate(wxCommandEvent & e)
{
	if (!m_bPause)
	{
		return;
	}
	int pos = m_slider->GetValue();
	
	Fairy::LogicModel* pObjModel = GetDataManipulator()->m_pObjTemplate;

	if ( pObjModel && !pObjModel->getCurrentAnimName().empty())
	{
		Ogre::Real timeTotal = pObjModel->getAnimationLength(pObjModel->getCurrentAnimName());

		Ogre::Real posValue = ((Ogre::Real)pos)/m_slider->GetMax();
		pObjModel->setAnimTimePos(posValue);
		m_curTimeText->SetValue(Ogre::StringConverter::toString(posValue*timeTotal));
		m_curFrameText->SetValue(Ogre::StringConverter::toString(posValue*timeTotal*STATIC_FPS));
	}
}

void AnimationSelector::OnFuseChange(wxCommandEvent& e)
{
	if ( m_fuseValue )
	{
		Ogre::String valueTime = m_fuseValue->GetValue().c_str();
		Ogre::Real totalTime = Ogre::StringConverter::parseReal(valueTime);
		if (totalTime <0 || totalTime>1)
		{
			m_fuseValue->SetValue("0");
			return;
		}
		m_curFuseValue = totalTime;
	}
}

const Fairy::String AnimationSelector::DoActiveItem(const wxTreeItemId& item)
{
    if (item.IsOk())
    {
        AnimationItemData* data = static_cast<AnimationItemData*>(mAnimationList->GetItemData(item));
        if (data)
        {
            const Fairy::String& name = data->name;
			if(false == name.empty())
				return name;
        }
    }
	return "";
}
