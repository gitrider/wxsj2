/////////////////////////////////////////////////////////////////////////////
// Name:        CEventAreaEditDlg.cpp
// Author:      XX
// Created:     XX/XX/XX
// Copyright:   XX
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "CEventAreaEditDlg.cpp"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "CEventAreaEditDlg.h"


#include <wx/splitter.h>
#include <wx/treectrl.h>
#include <wx/listctrl.h>
#include <wx/grid.h>

// ----------------------------------------------------------------------------
// Ogre headers
// ----------------------------------------------------------------------------

#include <OgreCodec.h>
#include <OgreResourceGroupManager.h>
#include <OgreStringConverter.h>

// ----------------------------------------------------------------------------

#include "Core/Terrain.h"
#include "Core/TerrainData.h"

#include "SceneManipulator.h"
#include "Action.h"
#include "WXOperatorManager.h"
#include "wx/wxprec.h"
#include "RegionEditDlgSelector.h"
#include "res/NpcPatrolEdit_wdr.h"
#include "EventAreaEditAction.h"
#include "UnreachRegion.h"
#include "SceneManipulator.h"
#include "WXOperatorManager.h"

using namespace Fairy;

BEGIN_EVENT_TABLE(CEventAreaEditDlg, wxPanel)
 
	EVT_BUTTON(ID_BUTTON_NEW_AREA_ID,	CEventAreaEditDlg::OnAddNewId)
	EVT_BUTTON(ID_BUTTON_MODITY,		CEventAreaEditDlg::OnModifyArea)
	EVT_BUTTON(ID_BUTTON_DEL_RECT, CEventAreaEditDlg::OnDeleteRect)
	EVT_RADIOBUTTON(ID_RADIOBUTTON_SELECT, CEventAreaEditDlg::OnSelect)
	EVT_RADIOBUTTON(ID_RADIOBUTTON_DRAW, CEventAreaEditDlg::OnDraw)
	EVT_LIST_ITEM_SELECTED(ID_LISTCTRL, CEventAreaEditDlg::OnSelectEventAreaRect)
	EVT_CHECKBOX(ID_CHECKBOX_LOCAL, CEventAreaEditDlg::OnCheckLocal)
	EVT_GRID_CMD_CELL_CHANGE(ID_GRID_PARAMS, CEventAreaEditDlg::OnGridCellChanged)
END_EVENT_TABLE()


CEventAreaEditDlg::CEventAreaEditDlg(const wxString& resourceGroup)
// �Ѿ���������б�
: m_pRectListCtrl(NULL)
// �����id��ť.
,m_pAddNewEventAreaIdBn(NULL)

// ���Select
,m_pSelectBn(NULL)

// ���Draw
,m_pDrawBn(NULL)

// id�ؼ�
,m_pEventIdText(NULL)

// �ű��ؼ�
,m_pScriptidText(NULL)

// ��������
,m_pSceneManipulator(NULL)

// ��Ӧ��action
,m_pEditAction(NULL)




{
    //Init(resourceGroup);
	
	//InitData();
}

CEventAreaEditDlg::CEventAreaEditDlg(
							wxWindow *parent, 
							const wxString& resourceGroup,
							wxWindowID winid,
							const wxPoint& pos,
							const wxSize& size,
							long style,
							const wxString& name
							)
							: m_pRectListCtrl(NULL)
// �����id��ť.
,m_pAddNewEventAreaIdBn(NULL)

// ���Select
,m_pSelectBn(NULL)

// ���Draw
,m_pDrawBn(NULL)

// id�ؼ�
,m_pEventIdText(NULL)

// �ű��ؼ�
,m_pScriptidText(NULL)

// ��������
,m_pSceneManipulator(NULL)

// ��Ӧ��action
,m_pEditAction(NULL)

						
{
    //Init(resourceGroup);
    Create(parent, winid, pos, size, style, name);
	////////////////////////////////////////////////////////////////////////////////////
	//
	// ��ȡ���ؼ�������
	//

	// �������ؼ���ָ��.
	m_pRectListCtrl = wxDynamicCast(FindWindow(ID_LISTCTRL), wxListCtrl);

	// �����id��ť
	m_pAddNewEventAreaIdBn = wxDynamicCast(FindWindow(ID_BUTTON_NEW_AREA_ID), wxButton);

	// // ���Select
	//m_pSelectBn = wxDynamicCast(FindWindow(ID_RADIOBUTTON_SELECT), wxButton);

	//  // ���Draw
	//m_pDrawBn = wxDynamicCast(FindWindow(ID_RADIOBUTTON_DRAW), wxButton);

	m_pLocalCheck = wxDynamicCast(FindWindow(ID_CHECKBOX_LOCAL), wxCheckBox);

	m_pParamsGrid = wxDynamicCast(FindWindow(ID_GRID_PARAMS), wxGrid);


	 // id�ؼ�
	m_pEventIdText  = wxDynamicCast(FindWindow(ID_TEXTCTRL_AREA_ID), wxTextCtrl);

	// �ű��ؼ�
	m_pScriptidText = wxDynamicCast(FindWindow(ID_TEXTCTRL_SCRIPT_ID), wxTextCtrl);
	

	if(m_pEventIdText)
	{
		m_pEventIdText->SetLabel("0");
	}

	if(m_pScriptidText)
	{
		m_pScriptidText->SetLabel("-1");
	}//

	// �õ��Ѿ���������б��ָ��.
	m_pRectListCtrl = wxDynamicCast(FindWindow(ID_LISTCTRL), wxListCtrl);

	m_pRectListCtrl->SetSingleStyle(wxLC_SINGLE_SEL, true);
	m_pRectListCtrl->InsertColumn(0, "�¼�����");
    //m_pRectListCtrl->InsertColumn(1, "EventId");
	//m_pRectListCtrl->InsertColumn(2, "ScriptId");
    m_pRectListCtrl->InsertColumn(1, "�¼�ID");
	m_pRectListCtrl->InsertColumn(2, "�ű�ID");

  }

void CEventAreaEditDlg::Init(const wxString& resourceGroup)
{
 

}


bool CEventAreaEditDlg::Create(
								  wxWindow *parent, 
								  wxWindowID winid,
								  const wxPoint& pos, 
								  const wxSize& size,
								  long style, 
								  const wxString& name
								  )
{
    if (!wxPanel::Create(parent, winid, pos, size, style, name))
        return false;

	EventArea(this, true, true);

    return true;
}

CEventAreaEditDlg::~CEventAreaEditDlg(void)
{

}



bool CEventAreaEditDlg::Show(bool show)
{
	SetActive(show);
	return wxWindow::Show(show);
}

void CEventAreaEditDlg::SetActive(bool bShow)
{
	if (bShow)
    {
		if(m_pSceneManipulator)
		{
			Fairy::Action* action = m_pSceneManipulator->_getAction("EventAreaEdit");
			m_pSceneManipulator->setActiveAction(action);//
			m_pEditAction = (CEventAreaEditAction *)action;
			
		}
       
    }
	else
	{
		if(m_pEditAction)
		{
			//m_pEditAction->SetOldHitPointMaterial();
			m_pSceneManipulator->setActiveAction(NULL);
		}
	}

	// ��ʾ���س���
	if(m_pEditAction)
	{
		m_pEditAction->HideScene(bShow);

		if(bShow)
		{
			// ���ݵ��εĸ߶���ʾ����.
			//m_pEditAction->SetRegionHeight();
		}
	}//

}

void CEventAreaEditDlg::SetSceneManipulator(Fairy::SceneManipulator* sceneManipulator)
{
	m_pSceneManipulator = sceneManipulator;
	
	if(m_pSceneManipulator)
	{
		m_pSceneManipulator->addSceneListener(this);
		GetAction();
	}
}


/////////////////////////////////////////////////////////////////////////////////////////
//
// �����༭�Ļص�����
//


///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ui���棬 �¼���Ӧ����
//


/*******/
///////////////////////////////////////////////////////////////////////////////////////////
//
// �߼����ݲ���
//

// ��ʼ���߼�����
void CEventAreaEditDlg::InitData()
{
	// ��ǰ������id
	m_CurEventAreaId = 0;

	// ����ǰ�Ľű�id
	m_iCurScriptId   = 0;

	// ��������.
	m_OperatroType   = 1;
}

// �������
void CEventAreaEditDlg::ClearData()
{
 
	// ��ս�������
	this->ClearUI();

	// ���action����.
	if(m_pEditAction)
	{
		m_pEditAction->ReleaseRectInfoMap();
	}//

}



// ��RegionAction��ˢ������, ���̵�ʱ��ʹ��
void CEventAreaEditDlg::RefreshData()
{


}


// ���ui���������
void CEventAreaEditDlg::ClearUiData()
{

}


// ��ȡ����
void CEventAreaEditDlg::LoadData(std::string strFile)
{
	if(m_pEditAction)
	{
		m_pEditAction->ReadEventAreaFromFile(strFile);
		RefreshDataUI();
	}
}

// �洢����
void CEventAreaEditDlg::SaveData(std::string strFile)
{
	if(m_pEditAction)
	{
		m_pEditAction->SaveEventAreaToFile(strFile);
	}
}

// �õ�����༭�ĳ�������
void CEventAreaEditDlg::GetEventAreaAction()
{

}


// ��ť������ ����һ����id
void CEventAreaEditDlg::OnAddNewId( wxCommandEvent &event )
{
	if(m_pEditAction)
	{
		if(m_pEditAction->m_bLocalArea)
		{
			m_pEditAction->m_iMaxLocalEventAreaId++;
			m_pEditAction->m_iCurEditEventAreaId = m_pEditAction->m_iMaxLocalEventAreaId;
		}
		else
		{
			m_pEditAction->m_iMaxEventAreaId++;
			m_pEditAction->m_iCurEditEventAreaId = m_pEditAction->m_iMaxEventAreaId;
		}	

		char buf[64];
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "%d", m_pEditAction->m_iCurEditEventAreaId);

		m_pEventIdText  = wxDynamicCast(FindWindow(ID_TEXTCTRL_AREA_ID), wxTextCtrl);
		if(m_pEventIdText)
		{
			m_pEventIdText->SetLabel(buf);
		}
	}
}


// ��ť������ ����һ����id
void CEventAreaEditDlg::OnModifyArea( wxCommandEvent &event )
{
	if(m_pEditAction && m_pEditAction->m_pCurOperatorRect)
	{
		//m_pEditAction->m_iCurEditEventAreaId);

		std::string strStlValue = "";
		int iEventid  = -1;
		int iScriptid = -1;

		strStlValue = m_pEventIdText->GetValue();
		iEventid    = atoi(strStlValue.c_str());
		strStlValue = m_pScriptidText->GetValue();
		iScriptid   = atoi(strStlValue.c_str());


		int iSelect = -1;
		int iItemCount = m_pRectListCtrl->GetItemCount();
	
		for(int i = 0; i < iItemCount; i++)
		{
			if(m_pEditAction->m_pCurOperatorRect->m_iId == m_pRectListCtrl->GetItemData(i))
			{
				iSelect = i;
				break;
			}
		}

		if(-1 != iSelect)
		{
			if(m_pEventIdText)
			{
				m_pRectListCtrl->SetItem(iSelect, 1, m_pEventIdText->GetValue().c_str());
			}

			// �ű��ؼ�
			if(m_pScriptidText)
			{
				m_pRectListCtrl->SetItem(iSelect, 2, m_pScriptidText->GetValue().c_str());
			}
		}

		m_pEditAction->ModifyEventArea(m_pEditAction->m_pCurOperatorRect->m_iId, iEventid, iScriptid);

	}
}


// ��ť������ ����һ����id
void CEventAreaEditDlg::OnDeleteRect( wxCommandEvent &event )
{
	if((m_pEditAction)&&(m_pRectListCtrl))
	{
		int iSelItemIndex = m_pRectListCtrl->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
		int iIndex = m_pRectListCtrl->GetItemData(iSelItemIndex);
		m_pEditAction->DeleteRect(iIndex);
		
	}
}

void CEventAreaEditDlg::OnCheckLocal( wxCommandEvent & event)
{
	if(m_pLocalCheck)
	{
		bool bLocal = m_pLocalCheck->GetValue();
		if (m_pEditAction)
		{
			m_pEditAction->setParameter("isLocal", Ogre::StringConverter::toString(bLocal));
			RefreshDataUI();

			m_pEditAction->m_iCurEditEventAreaId = m_pEditAction->m_bLocalArea?m_pEditAction->m_iMaxLocalEventAreaId:m_pEditAction->m_iMaxEventAreaId;
		
			char buf[64];
			memset(buf, 0, sizeof(buf));
			sprintf(buf, "%d", m_pEditAction->m_iCurEditEventAreaId);

			m_pEventIdText  = wxDynamicCast(FindWindow(ID_TEXTCTRL_AREA_ID), wxTextCtrl);
			if(m_pEventIdText)
			{
				m_pEventIdText->SetLabel(buf);
			}
		}
	}
}

void CEventAreaEditDlg::OnGridCellChanged(wxGridEvent & event)
{
	int row = event.GetRow();
	int	col = event.GetCol();
	
	Ogre::String newValue = m_pParamsGrid->GetCellValue(row, col).c_str();

	CEventAreaInfo* pCurInfo = m_pEditAction->m_pCurOperatorRect;			
	if (pCurInfo)
	{	
		if (col == 0)
		{
			pCurInfo->m_paramsName[row] = newValue;
		}
		else
		{
			pCurInfo->m_paramsValue[row] = newValue;
		}
	}
}

// ��ť������ ѡ��
void CEventAreaEditDlg::OnSelect( wxCommandEvent &event )
{
	if(m_pEditAction)
	{
		m_pEditAction->m_iOperator = SELECT;
	}
}

// ��ť������ ��
void CEventAreaEditDlg::OnDraw( wxCommandEvent &event )
{
	
	if(m_pEditAction)
	{
		m_pEditAction->m_iOperator = DRAW;
	}

}

//�õ��ű�id
std::string CEventAreaEditDlg::GetScriptIdValue()
{

	wxString strValue = "";
	std::string strStlValue = "";
	if(m_pScriptidText)
	{
		strValue = m_pScriptidText->GetValue();

		strStlValue = strValue.c_str();
		
	}//

	return strStlValue;
}

///////////////////////////////////////////////////////////////////////////////////////////
//
// �߼����ݲ���
//


// ���ñ༭action�Ľű�id
void CEventAreaEditDlg::SetActionScriptValue()
{
	std::string strScriptValue = "-1";

	strScriptValue = GetScriptIdValue();
	char bufInfo[256];
	memset(bufInfo, 0, sizeof(bufInfo));
	sprintf(bufInfo, "%s", strScriptValue.c_str());
	int iScriptValue = atoi(bufInfo);
	int iEventId = 0;


	if(m_pEventIdText)
	{
		iEventId = atoi(m_pEventIdText->GetValue().c_str());
	}

	if(m_pEditAction)
	{
		m_pEditAction->m_iCurEditScriptid = iScriptValue;
		if (m_pEditAction->m_bLocalArea)
		{
			if(m_pEditAction->m_iMaxLocalEventAreaId < iEventId)
			{
				m_pEditAction->m_iMaxLocalEventAreaId = iEventId;
			}
		}
		else
		{
			if(m_pEditAction->m_iMaxEventAreaId < iEventId)
			{
				m_pEditAction->m_iMaxEventAreaId = iEventId;
			}
		}		

		m_pEditAction->m_iCurEditEventAreaId = iEventId;

	}
	
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ������Ӧ�ص�����
//

void CEventAreaEditDlg::onAddNewRect(unsigned long ulTypeId, unsigned long ulInstanceId)
{
	char bufInfo[200];
	memset(bufInfo, 0, sizeof(bufInfo));

	if(m_pRectListCtrl)
	{
		int iItemCount = m_pRectListCtrl->GetItemCount();
	
		memset(bufInfo, 0, sizeof(bufInfo));
		sprintf(bufInfo, "�¼�����_%d", ulInstanceId);
		m_pRectListCtrl->InsertItem(iItemCount, bufInfo);
		//�󶨹���id
		m_pRectListCtrl->SetItemData(iItemCount, ulInstanceId);
		
		if(m_pEventIdText)
		{
			m_pRectListCtrl->SetItem(iItemCount, 1, m_pEventIdText->GetValue().c_str());
		}

		// �ű��ؼ�
		if(m_pScriptidText)
		{
			m_pRectListCtrl->SetItem(iItemCount, 2, m_pScriptidText->GetValue().c_str());
		}
		
		m_pRectListCtrl->SetItemState(iItemCount, wxLIST_STATE_SELECTED , wxLIST_STATE_SELECTED);
		m_pRectListCtrl->EnsureVisible(iItemCount);
	}

	UpdateParamsInfo();
}

	// ɾ��һ������
void CEventAreaEditDlg::onDelRect( unsigned long ulTypeId, unsigned long ulInstanceId)
{
	if(NULL == m_pRectListCtrl)
	{
		return;
	}

	int iItemCount = m_pRectListCtrl->GetItemCount();
	for(int i = 0; i < iItemCount; i++)
	{
		if(ulInstanceId == m_pRectListCtrl->GetItemData(i))
		{
			m_pRectListCtrl->DeleteItem(i);
			break;
		}
	}

	UpdateParamsInfo();
}
	
	// ѡ��һ������
void CEventAreaEditDlg::onSelRect( unsigned long ulTypeId, unsigned long ulInstanceId)
{
	int iItemCount = m_pRectListCtrl->GetItemCount();
	for(int i = 0; i < iItemCount; i++)
	{
		if(ulInstanceId == m_pRectListCtrl->GetItemData(i))
		{
			m_pRectListCtrl->SetItemState(i, wxLIST_STATE_SELECTED , wxLIST_STATE_SELECTED);
			m_pRectListCtrl->EnsureVisible(i);
			break;
		}
	}

	if((m_pEditAction)&&(m_pEditAction->m_pCurOperatorRect))
	{
		char bufInfo[128];
		
		// ��������id
		memset(bufInfo, 0, sizeof(bufInfo));
		sprintf(bufInfo, "%d", m_pEditAction->m_pCurOperatorRect->m_iEventAreaId);
		m_pEventIdText->SetValue(bufInfo);

		// ���ýű�ֵ
		memset(bufInfo, 0, sizeof(bufInfo));
		sprintf(bufInfo, "%d", m_pEditAction->m_pCurOperatorRect->m_iScriptId);
		m_pScriptidText->SetValue(bufInfo);
	}//

	UpdateParamsInfo();
}

// �õ�action
void CEventAreaEditDlg::GetAction()
{
	if(m_pSceneManipulator)
	{
		Fairy::Action* action = m_pSceneManipulator->_getAction("EventAreaEdit");
		m_pSceneManipulator->setActiveAction(action);//
		m_pEditAction = (CEventAreaEditAction *)action;
		
	}
}

// ��ս���
void CEventAreaEditDlg::ClearUI()
{

	if(m_pRectListCtrl)
	{
		m_pRectListCtrl->DeleteAllItems();
	}

}

// ˢ�½���
void CEventAreaEditDlg::RefreshDataUI()
{
	ClearUI();

	int iItemCount = 0;
	if((m_pEditAction)&&(m_pRectListCtrl))
	{

		char bufInfo[256];
		iItemCount = m_pRectListCtrl->GetItemCount();
		CEventAreaInfo* pEventAreaInfo = NULL;
		SHOW_RECT_MAP::iterator itEventAreaEnd;
		SHOW_RECT_MAP::iterator it;
		if (m_pEditAction->m_bLocalArea)
		{
			itEventAreaEnd = m_pEditAction->m_LocalRectShowMap.end();
			it = m_pEditAction->m_LocalRectShowMap.begin();
		}
		else
		{
			itEventAreaEnd = m_pEditAction->m_RectShowMap.end();
			it = m_pEditAction->m_RectShowMap.begin();
		}

		for(; it != itEventAreaEnd; it++)
		{
			pEventAreaInfo = it->second;
			if(pEventAreaInfo)
			{
				memset(bufInfo, 0, sizeof(bufInfo));
				sprintf(bufInfo, "�¼�����_%d", pEventAreaInfo->m_iId);
				m_pRectListCtrl->InsertItem(iItemCount, bufInfo);

				m_pRectListCtrl->SetItemData(iItemCount, iItemCount);
		
				memset(bufInfo, 0, sizeof(bufInfo));
				sprintf(bufInfo, "%d", pEventAreaInfo->m_iEventAreaId);
				m_pRectListCtrl->SetItem(iItemCount, 1, bufInfo);


				memset(bufInfo, 0, sizeof(bufInfo));
				sprintf(bufInfo, "%d", pEventAreaInfo->m_iScriptId);
				m_pRectListCtrl->SetItem(iItemCount, 2, bufInfo);
				
				m_pRectListCtrl->SetItemState(iItemCount, wxLIST_STATE_SELECTED , wxLIST_STATE_SELECTED);
				m_pRectListCtrl->EnsureVisible(iItemCount);
				iItemCount++;
						
			}
		}

	}
}

void CEventAreaEditDlg::UpdateParamsInfo()
{
	m_pParamsGrid->ClearGrid();

	if(m_pEditAction)
	{
		if (!m_pEditAction->m_bLocalArea)
		{
			m_pParamsGrid->Enable(false);
		}
		else
		{
			CEventAreaInfo* pCurInfo = m_pEditAction->m_pCurOperatorRect;			
			if (pCurInfo)
			{		
				for(int i = 0; i <10; ++i)
				{
					Ogre::String paramName = pCurInfo->m_paramsName[i];
					Ogre::String paramValue =  pCurInfo->m_paramsValue[i];
					m_pParamsGrid->SetCellValue(i, 0, paramName);
					m_pParamsGrid->SetCellValue(i, 1, paramValue);
				}
			}

			m_pParamsGrid->Enable(true);
		}
	}

}


// ѡ��һ�������������
void CEventAreaEditDlg::OnSelectEventAreaRect(wxListEvent& event)
{
	
	if(m_pRectListCtrl)
	{
		int iSelIndex = event.m_itemIndex;
		int iId = 0;
		if(iSelIndex >= 0)
		{
			iId = m_pRectListCtrl->GetItemData(iSelIndex);
			if(m_pEditAction)
			{
				m_pEditAction->SelectEventAreaRect(iId);
			}
		}
		
	}

	UpdateParamsInfo();
}

