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
#include <wx/listctrl.h>
#include <wx/busyinfo.h>
#include "MaterialSelector.h"
#include "EffectObjectPropertyEditor.h"
#include "EffectObjectEditor.h"
#include "MaterialEditor.h"
#include "WXEffectEditorFrame.h"
#include "OgreResourceManager.h"
#include "OgreMaterialManager.h"
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
// MaterialSelector class
// ----------------------------------------------------------------------------

#if wxUSE_STL
#define AS_STRING(s)    (s)
#else
#define AS_STRING(s)    ((s).c_str())
#endif

static const int feID_MATERIAL_LIST = wxNewId();
static const int feID_RELOAD_MATERIL_LIST =wxNewId();
static const int feID_SET_PARTICLE_MATERIAL = wxNewId();
static const int feID_SET_EFFECT_MATERIAL = wxNewId();
static const int feID_SET_SKILL_MATERIAL = wxNewId();
BEGIN_EVENT_TABLE(MaterialSelector, wxPanel)
EVT_UPDATE_UI(feID_MATERIAL_LIST, MaterialSelector::OnUpdateControls)

EVT_LIST_ITEM_SELECTED(feID_MATERIAL_LIST, MaterialSelector::OnEffectListSelected)
EVT_COMMAND_LEFT_DCLICK(feID_MATERIAL_LIST, MaterialSelector::OnEffectListLeftDoubleClicked)
EVT_LIST_ITEM_RIGHT_CLICK(feID_MATERIAL_LIST,MaterialSelector::OnMaterialRightClicked)
EVT_MENU(feID_RELOAD_MATERIL_LIST, MaterialSelector::OnReloadEffectList)
EVT_MENU(feID_SET_PARTICLE_MATERIAL,MaterialSelector::OnSetParticleMaterial)
EVT_MENU(feID_SET_EFFECT_MATERIAL,MaterialSelector::OnSetEffectMaterial)
EVT_MENU(feID_SET_SKILL_MATERIAL,MaterialSelector::OnSetSkillMaterial)
END_EVENT_TABLE()

MaterialSelector::~MaterialSelector()
{
    if (mMaterialList)
    {
        mMaterialList->Freeze();
        mMaterialList->DeleteAllItems();
        mMaterialList->Thaw();
    }
}

void
MaterialSelector::Init(const wxString& resourceGroup)
{
    mSceneManipulator = NULL;
    mMaterialList = NULL;
	m_Frame = NULL;
    mResourceGroup = resourceGroup;
}

void
MaterialSelector::SetSceneManipulator(Fairy::SceneManipulator* sceneManipulator)
{
    mSceneManipulator = sceneManipulator;
}

bool
MaterialSelector::Create(wxWindow *parent, wxWindowID winid,
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
MaterialSelector::Reload(const wxString& resourceGroup)
{
    mResourceGroup = resourceGroup;
    Reload();
}

bool
MaterialSelector::CreateControls(void)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(1);
	sizer->AddGrowableCol(0);
	sizer->AddGrowableRow(1);


	m_toolBar = new wxToolBar( this, wxID_ANY, wxDefaultPosition, wxSize(-1,26)); 
	m_toolBar->AddTool(feID_RELOAD_MATERIL_LIST,
		wxBITMAP(RELOAD),wxBITMAP(RELOAD),false,
		0,wxDefaultCoord,NULL, wxT("刷新材质资源"),wxT("刷新材质资源"));
	m_toolBar->Realize();

	sizer->Add( m_toolBar, 0, wxALIGN_LEFT|wxALL, 0 );

	mMaterialList = new wxListCtrl(this, feID_MATERIAL_LIST,
		wxDefaultPosition, wxDefaultSize,
		wxLC_LIST | wxLC_SINGLE_SEL,
		wxDefaultValidator);

	sizer->Add( mMaterialList, 0, wxGROW|wxALL, 0 );

	this->SetSizer(sizer);
	sizer->SetSizeHints(this);

    return true;
}

void
MaterialSelector::Reload(void)
{
    if (!Ogre::ResourceGroupManager::getSingletonPtr())
        return;

	
	wxBusyInfo* busyInfo = new wxBusyInfo(_("正在读取材质数据 ..."), this);

    mMaterialList->Freeze();

    mMaterialList->ClearAll();

	Ogre::ResourceManager::ResourceMapIterator resourceMapIterator = Ogre::MaterialManager::getSingleton().getResourceIterator();
	while ( resourceMapIterator.hasMoreElements() )
	{				
		long index = mMaterialList->GetItemCount();

		mMaterialList->InsertItem( index, AS_STRING( resourceMapIterator.peekNextValue()->getName()));

		resourceMapIterator.moveNext();
	}
	mMaterialList->Thaw();
	delete  busyInfo;
    

}

//////////////////////////////////////////////////////////////////////////

void
MaterialSelector::OnUpdateControls(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() != 0);
}

void
MaterialSelector::OnEffectListSelected(wxListEvent& e)
{

}

void
MaterialSelector::OnEffectListLeftDoubleClicked(wxCommandEvent& e)
{
    long item = mMaterialList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (item >= 0)
    {
		wxString name = mMaterialList->GetItemText(item);
		Ogre::MaterialPtr pMaterial = Ogre::MaterialManager::getSingleton().getByName(name.c_str());

		if (!pMaterial.isNull())
		{
			m_Frame->GetMaterialEditor()->InitMaterialEditor(pMaterial,Ogre::String(name.c_str()));
			m_Frame->GetEditorNotebook()->SetSelection(2);
		}
    }
}

void 
MaterialSelector::OnReloadEffectList( wxCommandEvent &event )
{
	Reload();
}
void MaterialSelector::OnSetParticleMaterial(wxCommandEvent& e)
{
	long item = mMaterialList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	wxString name = mMaterialList->GetItemText(item);
	m_Frame->GetEffectObjectEditor()->SetParticleMaterial(name.c_str());
}
void MaterialSelector::OnSetEffectMaterial(wxCommandEvent& e)
{
	long item = mMaterialList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	wxString name = mMaterialList->GetItemText(item);
	Fairy::EffectElement* pEffectElement = 
		dynamic_cast<Fairy::EffectElement*>(m_Frame->GetEffectObjectEditor()->GetSelectedObject());
	if(pEffectElement)
	{
		if(pEffectElement->getType() == "Mesh")
		{
			m_Frame->GetEffectObjectEditor()->SetMeshMatrial(name.c_str());
		}
		else if(pEffectElement->getType() == "BillboardSet")
		{
			m_Frame->GetEffectObjectEditor()->SetBillboardMatrial(name.c_str());
		}
		else if(pEffectElement->getType() == "Projector")
		{
			m_Frame->GetEffectObjectEditor()->SetProjectorMatrial(name.c_str());
		}
		else if(pEffectElement->getType() == "Beam")
		{
			m_Frame->GetEffectObjectEditor()->SetBeamMatrial(name.c_str());
		}
	}
	Ogre::ParticleSystem* pParticleSystem = 
		dynamic_cast<Ogre::ParticleSystem*>(m_Frame->GetEffectObjectEditor()->GetSelectedObject());
	if(pParticleSystem)
	{
		m_Frame->GetEffectObjectEditor()->SetParticleMaterial(name.c_str());
	}
}
void MaterialSelector::OnSetSkillMaterial(wxCommandEvent& e)
{
	long item = mMaterialList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	wxString name = mMaterialList->GetItemText(item);
	Ogre::StringInterface* pElement = m_Frame->GetSkillObjectEidtor()->GetSelectedSkllElement();
	if(pElement)
	{
		Fairy::AnimationRibbon* pRibbon = dynamic_cast<Fairy::AnimationRibbon*>(pElement);
		if(pRibbon)
			m_Frame->GetSkillObjectEidtor()->SetRibbonMatrial(name.c_str());
	}
}
void MaterialSelector::OnMaterialRightClicked(wxListEvent& event)
{
	long item = mMaterialList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	if(item >= 0)
	{
		wxString name = mMaterialList->GetItemText(item);
		Ogre::MaterialPtr pMaterial = Ogre::MaterialManager::getSingleton().getByName(name.c_str());
		if (!pMaterial.isNull())
		{
			
			int iSelectedPage = m_Frame->GetEditorNotebook()->GetSelection();

			if(iSelectedPage == 0)
			{
				Fairy::EffectElement* pEffectElement = dynamic_cast<Fairy::EffectElement*>(m_Frame->GetEffectObjectEditor()->GetSelectedObject());
				if(pEffectElement)
				{
					Ogre::String elementType = pEffectElement->getType();
					if(elementType == "Mesh" || elementType == "BillboardSet" || 
						elementType == "Projector" || elementType == "Beam")
					{
						wxPoint pt = event.GetPoint();
						wxMenu menu;
						wxString menuText = wxT("效果:");
						menuText.Append(elementType.c_str());
						menu.Append(feID_SET_EFFECT_MATERIAL,menuText);
						wxPoint clientpt = event.GetPoint();
						PopupMenu(&menu, pt);

					}
					
				}
				Ogre::ParticleSystem* pParticleSystem = dynamic_cast<Ogre::ParticleSystem*>(m_Frame->GetEffectObjectEditor()->GetSelectedObject());
				if(pParticleSystem)
				{
					wxPoint pt = event.GetPoint();
					wxMenu menu;
					wxString menuText = wxT("粒子:");
					menuText.Append(pParticleSystem->getName().c_str());
					menu.Append(feID_SET_EFFECT_MATERIAL,menuText);
					wxPoint clientpt = event.GetPoint();
					PopupMenu(&menu, pt);

				}
			}
			else if(iSelectedPage == 1)
			{
				Ogre::StringInterface* pElement = m_Frame->GetSkillObjectEidtor()->GetSelectedSkllElement();
				if(!pElement)
					return;
				Fairy::AnimationRibbon* pRibbon = dynamic_cast<Fairy::AnimationRibbon*>(pElement);
				if(pRibbon)
				{
					wxPoint pt = event.GetPoint();
					wxMenu menu;
					wxString menuText = wxT("技能:Ribbon ");
					menu.Append(feID_SET_SKILL_MATERIAL,menuText);
					wxPoint clientpt = event.GetPoint();
					PopupMenu(&menu, pt);
				}
			}

			//#if wxUSE_MENUS
			//	wxMenu menu(title);
			//	menu.Append(TreeTest_About, wxT("&About..."));
			//	menu.AppendSeparator();
			//	menu.Append(TreeTest_Highlight, wxT("&Highlight item"));
			//	menu.Append(TreeTest_Dump, wxT("&Dump"));
			//
			//	PopupMenu(&menu, pt);
			//#endif // wxUSE_MENUS
		}
	}
}
