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
#include <set>
#include "ParticleSystemSelector.h"
#include "WXParticleEditorDialog.h"

// ----------------------------------------------------------------------------
// Ogre headers
// ----------------------------------------------------------------------------

#include <OgreParticleSystemManager.h>
#include <OgreBone.h>
#include <OgreSkeletonInstance.h>
#include <OgreEntity.h>
#include <OgreStringConverter.h>
// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------

#include "SceneManipulator.h"
#include "Action.h"
#include "EffectSystem\FairyEffectManager.h"
#include "Core/FairyStaticEntityObject.h"
#include "Core/FairyParticleSystemObject.h"
#include "Core/FairyEffectObject.h"

#include <wx/notebook.h>
// ----------------------------------------------------------------------------
// ParticleSystemSelector class
// ----------------------------------------------------------------------------

#if wxUSE_STL
#define AS_STRING(s)    (s)
#else
#define AS_STRING(s)    ((s).c_str())
#endif


IMPLEMENT_DYNAMIC_CLASS(SetToSceneNodeDialog, wxDialog)
// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(SetToSceneNodeDialog, wxDialog)
EVT_INIT_DIALOG(SetToSceneNodeDialog::OnInitDialog)
END_EVENT_TABLE()

void
SetToSceneNodeDialog::OnInitDialog(wxInitDialogEvent &e)
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer4->SetFlexibleDirection( wxBOTH );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	mStaticEntity->getMeshName();
	wxStaticText* staticText1 = new wxStaticText( this, wxID_ANY, mStaticEntity->getMeshName().c_str(), wxDefaultPosition, wxSize( 60,-1 ), 0 );

	fgSizer4->Add( staticText1, 0, wxALL, 5 );

	mComboBox = new wxComboBox( this, wxID_ANY,wxEmptyString, wxDefaultPosition, wxSize( 200,-1 ), 0, NULL, 0 ); 
	Ogre::SkeletonInstance* pSkeleton = mStaticEntity->getEntity()->getSkeleton();
	if(pSkeleton)
	{
		for(int i = 0;i<pSkeleton->getNumBones();i++)
		{
			mComboBox->Append(pSkeleton->getBone(i)->getName().c_str());
		}
	}
	fgSizer4->Add( mComboBox, 0, wxALL, 5 );

	bSizer13->Add( fgSizer4, 1, wxEXPAND, 5 );

	wxStdDialogButtonSizer* m_sdbSizer5 = new wxStdDialogButtonSizer();
	wxButton* m_sdbSizer5OK = new wxButton( this, wxID_OK );
	m_sdbSizer5->AddButton( m_sdbSizer5OK );
	wxButton* m_sdbSizer5Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer5->AddButton( m_sdbSizer5Cancel );
	m_sdbSizer5->Realize();
	bSizer13->Add( m_sdbSizer5, 1, wxEXPAND, 5 );

	this->SetSizer( bSizer13 );
	this->Layout();

}


static const int feID_PARTICLESYSTEM_LIST = wxNewId();
static const int feID_RELOAD_PARTICLE_LIST =wxNewId();
static const int feID_SET_SCENE_OJBECT_NODE = wxNewId();
BEGIN_EVENT_TABLE(ParticleSystemSelector, wxPanel)
EVT_UPDATE_UI(feID_PARTICLESYSTEM_LIST, ParticleSystemSelector::OnUpdateControls)

EVT_LIST_ITEM_SELECTED(feID_PARTICLESYSTEM_LIST, ParticleSystemSelector::OnParticleSystemListSelected)
EVT_LIST_ITEM_RIGHT_CLICK(feID_PARTICLESYSTEM_LIST,ParticleSystemSelector::OnParticleSystemRightClicked)
EVT_COMMAND_LEFT_DCLICK(feID_PARTICLESYSTEM_LIST, ParticleSystemSelector::OnParticleSystemListLeftDoubleClicked)
EVT_BUTTON(feID_RELOAD_PARTICLE_LIST, ParticleSystemSelector::OnReloadParticleList)
EVT_MENU(feID_SET_SCENE_OJBECT_NODE, ParticleSystemSelector::OnSetToSceneObjectNode)

END_EVENT_TABLE()

ParticleSystemSelector::~ParticleSystemSelector()
{
    if (mParticleSystemList)
    {
        mParticleSystemList->Freeze();
        mParticleSystemList->DeleteAllItems();
        mParticleSystemList->Thaw();
    }

	if (mParticleEditorDialog)
	{
		mParticleEditorDialog->Destroy();
		delete mParticleEditorDialog;
		mParticleEditorDialog = NULL;
	}
}

void
ParticleSystemSelector::Init(const wxString& resourceGroup)
{
    mSceneManipulator = NULL;
    mParticleSystemList = NULL;

	mParticleEditorDialog = NULL;

    mResourceGroup = resourceGroup;
	mFocused = true;
}

void
ParticleSystemSelector::SetSceneManipulator(Fairy::SceneManipulator* sceneManipulator)
{
    mSceneManipulator = sceneManipulator;
	mSceneManipulator->addSceneListener(this);
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

	wxButton *item2 = new wxButton( this, feID_RELOAD_PARTICLE_LIST, _("更新"), wxDefaultPosition, wxDefaultSize, 0 );
	sizer->Add( item2, 0, wxALIGN_CENTER|wxALL, 5 );

    mParticleSystemList = new wxListCtrl(this, feID_PARTICLESYSTEM_LIST,
        wxDefaultPosition, wxDefaultSize,
        wxLC_LIST | wxLC_SINGLE_SEL,
        wxDefaultValidator);

    sizer->Add(mParticleSystemList, 0, wxGROW|wxALL, 5);

    this->SetSizer(sizer);
    sizer->SetSizeHints(this);

    return true;
}

void
ParticleSystemSelector::Reload(void)
{
    if (!Ogre::ResourceGroupManager::getSingletonPtr())
        return;

    mParticleSystemList->Freeze();

    mParticleSystemList->ClearAll();

	mFileMap[0].clear();
	mFileMap[1].clear();
    
    Ogre::ParticleSystemManager::ParticleSystemTemplateIterator iterator = Ogre::ParticleSystemManager::getSingleton().getTemplateIterator();

    while ( iterator.hasMoreElements() )
    {
        // 获取到粒子系统的名称

		long index = mParticleSystemList->GetItemCount();
		wxString name = AS_STRING( iterator.peekNextKey() );
        long item = mParticleSystemList->InsertItem( index, name );
		mFileMap[0].insert(FileItemMap::value_type(name, item));
		mParticleSystemList->SetItemTextColour( index , *wxRED );
        // 使iterator往后移
        iterator.moveNext();
    }

	// 添加特效
	// 填充现在的effect模板的列表
	Fairy::EffectManager::EffectTemplateIterator it = 
		Fairy::EffectManager::getSingleton().getEffectTemplateIterator();

	while ( it.hasMoreElements() )
	{
		long index = mParticleSystemList->GetItemCount();

		wxString name = AS_STRING( it.peekNextKey() );
		long item = mParticleSystemList->InsertItem( index, name ) ;
		mFileMap[1].insert(FileItemMap::value_type(name, item));

		if (it.peekNextKey() == "EffectEmpty")
			mParticleSystemList->SetItemTextColour(index, *wxGREEN);

		it.moveNext();
	}

    mParticleSystemList->Thaw();
}

//////////////////////////////////////////////////////////////////////////

void
ParticleSystemSelector::OnUpdateControls(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() != 0);
}

void ParticleSystemSelector::OnParticleSystemRightClicked(wxListEvent& event)
{
	long item = mParticleSystemList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	if(item >= 0)
	{
		wxString name = mParticleSystemList->GetItemText(item);
		std::set<Fairy::ObjectPtr> ObjectSet = mSceneManipulator->getSelectedObjects();
		for(std::set<Fairy::ObjectPtr>::const_iterator iter = ObjectSet.begin();iter != ObjectSet.end(); iter++)
		{
			Fairy::ObjectPtr object = *iter;
			Fairy::StaticEntityObject* pStaticEntityObject = dynamic_cast<Fairy::StaticEntityObject*>(object.get());
			if(pStaticEntityObject)
			{
				wxPoint pt = event.GetPoint();
				wxMenu menu;
				wxString menuText = wxT("添加到场景节点");
				menu.Append(feID_SET_SCENE_OJBECT_NODE,menuText);
				wxPoint clientpt = event.GetPoint();
				PopupMenu(&menu, pt);
				break;
			}
		}

	}
}

void
ParticleSystemSelector::OnParticleSystemListSelected(wxListEvent& e)
{
	if (!mFocused)
	{
		mFocused = true;
		return;
	}

    long item = mParticleSystemList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (item >= 0)
    {
		wxColour itemColor = mParticleSystemList->GetItemTextColour(item);

		wxString name = mParticleSystemList->GetItemText(item);
		Fairy::Action* action = GetSceneManipulator()->_getAction("CreateObjectAction");

		// 靠颜色来判断是粒子系统或特效
		if ( itemColor == *wxRED )
		{
			action->setParameter("type", "ParticleSystem");
			action->setParameter("has position", "false");
			action->setParameter("%particle system name", AS_STRING(name));
		}
		else
		{
			action->setParameter("type", "Effect");
			action->setParameter("has position", "false");
			action->setParameter("%effect name", AS_STRING(name));
		}

        GetSceneManipulator()->setActiveAction(action);
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

		// 靠颜色来判断是粒子系统或特效
		if ( itemColor == *wxRED )
		{
			Ogre::ParticleSystem *pTemplate = Ogre::ParticleSystemManager::getSingleton().getTemplate(name.c_str());

			if (pTemplate)
			{
				if ( NULL == mParticleEditorDialog )
				{
					int width = 0;
					int height = 0;
					int x = 0;
					int y = 0;

					GetParent()->GetParent()->GetParent()->GetParent()->GetSize(&width, &height);
					GetParent()->GetParent()->GetParent()->GetParent()->GetPosition(&x, &y);

					mParticleEditorDialog = new ParticleEditorDialog(this, wxID_ANY, _("Fairy Particle Editor"), wxPoint(x, y), wxSize(width / 3, height));
				}
				if ( false == mParticleEditorDialog->IsShown() )
					mParticleEditorDialog->Show();	

				mParticleEditorDialog->InitParticleEditor(pTemplate, Ogre::String(name.c_str()), mSceneManipulator);
			}
		}	

		GetSceneManipulator()->setActiveAction(NULL);
    }
}

void 
ParticleSystemSelector::OnReloadParticleList( wxCommandEvent &event )
{
	Reload();
}

void ParticleSystemSelector::SelectItem(const wxString& itemName, bool bParticle)
{
	if (bParticle)
	{
		FileItemMap::iterator it = mFileMap[0].find(itemName);
		if (it != mFileMap[0].end())
		{
			mParticleSystemList->SetItemState(it->second, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
		}
	}
	else
	{
		FileItemMap::iterator it = mFileMap[1].find(itemName);
		if (it != mFileMap[1].end())
		{
			mParticleSystemList->SetItemState(it->second, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
		}
	}	
}

void ParticleSystemSelector::onSelectObject(const Fairy::ObjectPtr& object)
{
	if ((int)mSceneManipulator->getSelectedObjects().size() == 1)
	{
		if (object->getType() == "ParticleSystem")
		{
			Fairy::ParticleSystemObject *particleSystemObject = static_cast<Fairy::ParticleSystemObject *> (object.get());
			Ogre::String particleName = particleSystemObject->getParticleSystemName();
			wxNotebook* notebook = (wxNotebook*)GetParent();
			notebook->SetSelection(2);
			mFocused = false;
			SelectItem(particleName.c_str());
		}
		else if (object->getType() == "Effect")
		{
			Fairy::EffectObject *effectObject = static_cast<Fairy::EffectObject *> (object.get());
			Ogre::String effectName = effectObject->getEffectName();
			wxNotebook* notebook = (wxNotebook*)GetParent();
			notebook->SetSelection(2);
			mFocused = false;
			SelectItem(effectName.c_str(), false);
		}
	}
}
void 
ParticleSystemSelector::OnSetToSceneObjectNode( wxCommandEvent &event )
{
	long item = mParticleSystemList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	if(item >= 0)
	{
		wxColour itemColor = mParticleSystemList->GetItemTextColour(item);
		wxString name = mParticleSystemList->GetItemText(item);
	
		std::set<Fairy::ObjectPtr> ObjectSet = mSceneManipulator->getSelectedObjects();
		for(std::set<Fairy::ObjectPtr>::const_iterator iter = ObjectSet.begin();iter != ObjectSet.end(); iter++)
		{
			Fairy::ObjectPtr object = *iter;
			Fairy::StaticEntityObject* pStaticEntityObject = dynamic_cast<Fairy::StaticEntityObject*>(object.get());
			if(pStaticEntityObject)
			{
				Ogre::SkeletonInstance* pSkeleton = pStaticEntityObject->getEntity()->getSkeleton();
				SetToSceneNodeDialog *dlg = new SetToSceneNodeDialog(this, wxID_ANY,
					name.c_str(), pStaticEntityObject);

				bool ok = (dlg->ShowModal() == wxID_OK);
				if(ok)
				{
					Ogre::String NodeName = dlg->mComboBox->GetValue().c_str();
					if(NodeName == "" || !pSkeleton->getBone(NodeName))
					{
						dlg->Destroy();
						return;
					}

					Ogre::Quaternion qOrientation = pSkeleton->getBone(NodeName)->getOrientation();
					Ogre::Vector3 vNodePosition = pSkeleton->getBone(NodeName)->getPosition();
					if(vNodePosition.x < 0.001 && vNodePosition.x > 0.001)
						vNodePosition.x = 0.0f;
					if(vNodePosition.y < 0.001 && vNodePosition.y > 0.001)
						vNodePosition.y = 0.0f;
					if(vNodePosition.z < 0.001 && vNodePosition.z > 0.001)
						vNodePosition.z = 0.0f;
					Ogre::Vector3 vPosition = pStaticEntityObject->getEntity()->getParentNode()->getPosition();
					vPosition = vPosition + vNodePosition;

					Fairy::Action* action = GetSceneManipulator()->_getAction("CreateObjectAction");

					// 靠颜色来判断是粒子系统或特效
					if ( itemColor == *wxRED )
					{
						action->setParameter("type", "ParticleSystem");
						action->setParameter("has position", "true");
						action->setParameter("%particle system name", AS_STRING(name));
						action->setParameter("%position",Ogre::StringConverter::toString(vPosition));
						//action->setParameter("%orientation",Ogre::StringConverter::toString(qOrientation));
					}
					else
					{
						action->setParameter("type", "Effect");
						action->setParameter("has position", "true");
						action->setParameter("%effect name", AS_STRING(name));
						action->setParameter("%position",Ogre::StringConverter::toString(vPosition));
						//action->setParameter("%orientation",Ogre::StringConverter::toString(qOrientation));
					}

					GetSceneManipulator()->setActiveAction(action);

					dlg->Destroy();
				}
				else
				{
					dlg->Destroy();
					return;

				}

				break;
			}
		}

	}
}

