// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "BodySelectorDialog.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
#include <wx/scrolwin.h>
#include <wx/toolbar.h>
#include <wx/stattext.h>
#include <wx/button.h>
#endif

#include "BodySelectorDialog.h"
#include "DataManipulator.h"

#include <wx/treectrl.h>
#include <wx/colordlg.h>

#include "ModelSystem/FairyLogicModelManager.h"

#include "EffectSystem/FairyEffectManager.h"

#include "Core/FairyVariant.h"
// ----------------------------------------------------------------------------
// Resources
// ----------------------------------------------------------------------------

#include "res/BodySelector_wdr.h"

// ----------------------------------------------------------------------------
// Ogre headers
// ----------------------------------------------------------------------------
#include <OgreStringConverter.h>
// ----------------------------------------------------------------------------
// macros
// ----------------------------------------------------------------------------


IMPLEMENT_DYNAMIC_CLASS(BodySelectorDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(BodySelectorDialog, wxDialog)
//EVT_INIT_DIALOG(BodySelectorDialog::OnInitDialog)
EVT_LISTBOX(ID_LISTBOX_BODY, BodySelectorDialog::OnListItemChanged)
END_EVENT_TABLE()

void BodySelectorDialog::OnInitDialog(/*wxInitDialogEvent &e*/)
{
	//wxDialog::OnInitDialog(e);
	wxSizer* sizer = BodySelector(this, true, true);

	m_pBodyList = wxDynamicCast(this->FindWindow(ID_LISTBOX_BODY), wxListBox);

	if(!m_UpdateName.empty())
		m_nModel = 1;

	ReloadRes(m_UpdateName);

	this->Show(true);

	SetFocus();
}

BodySelectorDialog::~BodySelectorDialog()
{
}

void BodySelectorDialog::ReloadRes(Ogre::String updatename)
{

	Ogre::String group = "Hero";
	Ogre::FileInfoListPtr fileInfoList =
		Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		"*.mesh");

	switch(m_ePart)
	{
	case EMP_HEAD:
		{
			m_UpdateName += "帽子";
			this->SetPosition(wxPoint(260,75));
		}
		break;
	case EMP_FACE:
		{
			m_UpdateName += "脸";
			this->SetPosition(wxPoint(430,75));
		}
		break;
	case EMP_SHOULDER:
		{
			m_UpdateName += "肩甲";
			this->SetPosition(wxPoint(260,370));
		}
		break;
	case EMP_BODY:
		{
			m_UpdateName += "身体";
			this->SetPosition(wxPoint(430,370));
		}
		break;
	case EMP_HAND:
		{
			m_UpdateName += "双手";
			this->SetPosition(wxPoint(260,670));
		}
		break;
	case EMP_FOOT:
		{
			m_UpdateName += "双脚";
			this->SetPosition(wxPoint(430,670));
		}
		break;
	case EMP_LWEAPON:
		{
			//m_UpdateName += "帽子";
		}
		break;
	case EMP_RWEAPON:
		{
			//m_UpdateName += "帽子";
		}
		break;
	case EMP_SHIELD:
		{
			//m_UpdateName += "帽子";
		}
		break;
	case EMP_RIDER:
		{
			//m_UpdateName += "帽子";
		}
		break;
	default:		
		break;

	}
	m_pBodyList->Clear();
	int nIndex = 0;
	for (Ogre::FileInfoList::const_iterator it = fileInfoList->begin(); it != fileInfoList->end(); ++it)
	{	
		const Ogre::String& name = it->filename;

		if(GetDataManipulatr())
		{
			Fairy::CGod* pModel = GetDataManipulatr()->FindModel(GetDataManipulatr()->m_CurModelName);

			if(pModel != NULL)
			{
				if(m_nModel)
				{
					std::vector<Ogre::String> vec = Ogre::StringUtil::split(name,"_");
					if(m_UpdateName.compare(vec[0]) == 0)
					{
						m_pBodyList->Insert(name.c_str(),nIndex);
						nIndex++;
					}
				}
			}
			else
			{
				std::basic_string <char>::size_type indexCh;
				indexCh = name.find(m_UpdateName);
				if(indexCh != std::string::npos)
				{
					indexCh = name.find("男");
					if(indexCh != std::string::npos)
					{
						m_pBodyList->Insert(name.c_str(),nIndex);
						nIndex++;
						continue;
					}
					indexCh = name.find("女");
					if(indexCh != std::string::npos)
					{
						m_pBodyList->Insert(name.c_str(),nIndex);
						nIndex++;
					}
				}
			}
		}
	}
}

void BodySelectorDialog::OnListItemChanged(wxCommandEvent& e)
{
	Ogre::String item = e.GetString();

	int nId = e.GetId();

	if(GetDataManipulatr())
	{
		try{
			Fairy::CGod* pModel = GetDataManipulatr()->FindModel(GetDataManipulatr()->m_CurModelName);

			if(pModel)
			{	

				if(NULL == pModel->m_pObjModel)
					return;

				if(m_nModel)
				{
					pModel->SaveTempEntity();
					pModel->SetTempEntity(pModel->m_pObjTemplate);
					Ogre::String UpdateName = pModel->m_pObjTemplate->GetEntityName(m_UpdateName);
					pModel->m_pObjModel->changeEntity(UpdateName,item);
					pModel->GetTempntity();

					if(GetDataManipulatr()->GetActiveButton(static_cast<int>(m_ePart)))
						GetDataManipulatr()->GetActiveButton(static_cast<int>(m_ePart))->SetLabel(item);
				}
				else
				{
					GetDataManipulatr()->MakeUpModel(item);
					if(GetDataManipulatr()->GetActiveButton(static_cast<int>(m_ePart)))
						GetDataManipulatr()->GetActiveButton(static_cast<int>(m_ePart))->SetLabel(item);
				}
			}
			else
			{
				GetDataManipulatr()->MakeUpModel(item);
				if(GetDataManipulatr()->GetActiveButton(static_cast<int>(m_ePart)))
					GetDataManipulatr()->GetActiveButton(static_cast<int>(m_ePart))->SetLabel(item);
			}
		}
		catch(...)
		{
			Ogre::LogManager::getSingleton().logMessage(GetDataManipulatr()->m_CurModelName + "类型转换失败!");
			return;
		}

	}
}