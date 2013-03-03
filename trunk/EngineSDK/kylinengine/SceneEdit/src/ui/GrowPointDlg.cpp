/////////////////////////////////////////////////////////////////////////////
// Name:        GrowPointDlg.
// Author:      XX
// Created:     XX/XX/XX
// Copyright:   XX
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "GrowPointDlg.cpp"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "GrowPointDlg.h"

#include <wx/treectrl.h>
#include <wx/listctrl.h>

#include "GrowPointEditAction.h"
#include "SceneManipulator.h"
#include "WXEditorFrame.h"
#include "WXSceneCanvas.h"
#include "WXSJ_DBC.h"				// ��ȡexcel
#include "SceneInfoMapDialog.h"		// ����ͼ����.
#include "RegionEditAction.h"

// WDR: class implementations


BEGIN_EVENT_TABLE(CGrowPointEditDlg, wxPanel)

 	 EVT_LIST_ITEM_SELECTED(ID_LISTCTRL_GROW_POINT,		  CGrowPointEditDlg::OnGrowPointTypeSelected)
	 EVT_TREE_SEL_CHANGED(ID_TREECTRL_ADDED_GROW_POINT,   CGrowPointEditDlg::OnGrowPointAddedSelected)

END_EVENT_TABLE()

CGrowPointEditDlg::CGrowPointEditDlg(const wxString& resourceGroup)
: m_pGrowPointListCtrl(NULL)
, m_pAddedGrowPointTreeCtrl(NULL)
, m_pSceneManipulator(NULL)
, m_pGrowPointEditAction(NULL)
, m_pNavigationDlg(NULL)
, m_pParent(NULL)

{
    //Init(resourceGroup);
	//InitData();
}

CGrowPointEditDlg::CGrowPointEditDlg(
							wxWindow *parent, 
							const wxString& resourceGroup,
							wxWindowID winid,
							const wxPoint& pos,
							const wxSize& size,
							long style,
							const wxString& name
							)
: m_pGrowPointListCtrl(NULL)
, m_pAddedGrowPointTreeCtrl(NULL)
, m_pSceneManipulator(NULL)
, m_pGrowPointEditAction(NULL)
, m_pNavigationDlg(NULL)
, m_pParent(NULL)
						
										
{
    Create(parent, winid, pos, size, style, name);
	//InitData();

}


bool CGrowPointEditDlg::Create(
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

	// ��������������� �Ի���Ŀؼ�������ʾ
	GrowPointDlg(this, true, true);
	
	//// ��ȡ���ؼ�������
	//GetAllControl();

	//// ��ʼ�������б�ı�ͷ.
	//InitMonsterListCtrlTable();

	//// ��ȡ����id������ӳ���ļ�����
	//LoadMonsterIdNameTable();

	//// ��ʼ��tree�ṹ
	//InitAddedMonsterTree();

	// �õ����еĿؼ�.
	GetAllControl();

	// ��ʼ�������б�ı�ͷ.
	InitGrowPointListCtrlTable();

	// ��ʼ�������б�
	InitAddedGrowPointTree();

	GetAction();
	return true;
}

// ��ʾ����
bool CGrowPointEditDlg::Show(bool show)
{
	SetActive(show);
	return wxWindow::Show(show);
}


// ��ʼ�������б�ı�ͷ.
void CGrowPointEditDlg::InitGrowPointListCtrlTable()
{
	m_pGrowPointListCtrl->InsertColumn(0, "���");	
    m_pGrowPointListCtrl->InsertColumn(1, "����");	
	m_pGrowPointListCtrl->InsertColumn(2, "����");	


	DBC::DBCFile excelDB(1);
	DBC::DBCFile excelDBFileModelName(1);
	if(!excelDB.OpenFromTXT("..\\..\\Public\\Config\\grow_point.tab"))
	{
		wxLogMessage(_T("���ܴ���������Ϣ�� ���ܱ༭��������Ϣ\n"));
        Enable(false);
        Hide();
		return;
	}

	if(!excelDBFileModelName.OpenFromTXT("..\\resource\\config\\grow_point.tab"))
	{
		wxLogMessage(_T("���ܴ򿪷�����������Ϣ�� ���ܱ༭������Ϣ\n"));
		return;
	}


	m_IdNameMap.clear();
	m_IdMeshMap.clear();

	int iRecordCount = excelDBFileModelName.GetRecordsNum(); 
	const DBC::DBCFile::FIELD* pInfo = NULL;	
	char bufInfo[512];
	int  iId  = 0;

	char buf[16];
	sprintf(buf,"recordCount: %d",iRecordCount);
	OutputDebugStr(buf);



	for(int i = 0; i < iRecordCount; i++)
	{
		// ��ȡid
		pInfo = excelDBFileModelName.Search_Posistion(i, 0);
		memset(bufInfo, 0, sizeof(bufInfo));
		sprintf(bufInfo, "%d", pInfo->iValue);
		m_pGrowPointListCtrl->InsertItem(i, bufInfo);
		iId = pInfo->iValue;
		//id
		m_pGrowPointListCtrl->SetItemData(i, pInfo->iValue);
		

		// ��ȡ����
		pInfo = excelDBFileModelName.Search_Posistion(i, 1);
		memset(bufInfo, 0, sizeof(bufInfo));
		sprintf(bufInfo, "%s", pInfo->pString);
		m_pGrowPointListCtrl->SetItem(i, 1, bufInfo);
		m_IdNameMap[iId] = bufInfo;

		// ��ȡ����.
		pInfo = excelDBFileModelName.Search_Posistion(i, 3);
		memset(bufInfo, 0, sizeof(bufInfo));
		sprintf(bufInfo, "%s", pInfo->pString);
		m_pGrowPointListCtrl->SetItem(i, 2, bufInfo);
		m_IdMeshMap[iId] = bufInfo;
		
		
	}
	
}


// �õ����еĿؼ�.
void CGrowPointEditDlg::GetAllControl()
{
	// �õ������б�ؼ�.
	m_pGrowPointListCtrl = wxDynamicCast(FindWindow(ID_LISTCTRL_GROW_POINT), wxListCtrl);

	// ����Ѿ���ӵĹ����б�Ŀؼ�.
	m_pAddedGrowPointTreeCtrl =  wxDynamicCast(FindWindow(ID_TREECTRL_ADDED_GROW_POINT), wxTreeCtrl);


}

// ��ʼ�������б���.
void CGrowPointEditDlg::InitAddedGrowPointTree()
{

	if(m_pAddedGrowPointTreeCtrl)
	{
		m_pAddedGrowPointTreeCtrl->DeleteAllItems();
		m_AddedGrowPointRoot = m_pAddedGrowPointTreeCtrl->AddRoot("�Ѿ���ӵ�������");

		ID_NAME_DLG_MAP::iterator it;
		ID_NAME_DLG_MAP::iterator itEnd;
		wxTreeItemId newTreeItemId;
		std::string  strName = "";

		m_IdTreeItemMap.clear();
		itEnd = m_IdNameMap.end();
		for(it = m_IdNameMap.begin(); it != itEnd; it++)
		{
			strName = it->second;
			newTreeItemId = m_pAddedGrowPointTreeCtrl->AppendItem(m_AddedGrowPointRoot, strName.c_str());
			m_IdTreeItemMap[it->first] = newTreeItemId;
		}

	}//

}


// �õ�action
void CGrowPointEditDlg::GetAction()
{
	// �õ�����༭�ĳ�������
	if(m_pSceneManipulator)
	{
		Fairy::Action* action = m_pSceneManipulator->_getAction("GrowPointEdit");
		m_pGrowPointEditAction = (Fairy::CGrowPointEditAction *)action;
	}
}

// ���ó����������
void CGrowPointEditDlg::SetSceneManipulator(Fairy::SceneManipulator* sceneManipulator)
{
	m_pSceneManipulator = sceneManipulator;
	
	if(m_pSceneManipulator)
	{
		m_pSceneManipulator->addSceneListener(this);
		
		// �õ��ײ��action
		GetAction();
		SetActionMeshType();
		SetActive(false);
	}
}


// �����
void CGrowPointEditDlg::SetActive(bool bShow)
{

	if(bShow)
    {
		if(m_pSceneManipulator)
		{
			Fairy::Action* action = m_pSceneManipulator->_getAction("GrowPointEdit");
			m_pSceneManipulator->setActiveAction(action);//
				
		}
       
    }
	else
	{
		if(m_pGrowPointEditAction)
		{
			m_pSceneManipulator->setActiveAction(NULL);
		}
	}

	// ��ʾ���س���
	if(m_pGrowPointEditAction)
	{
		m_pGrowPointEditAction->HideScene(bShow);

		Fairy::Action* action = m_pSceneManipulator->_getAction("RegionEdit");
		Fairy::CRegionEditAction* pRegionEditAction = ( Fairy::CRegionEditAction*)action;
		pRegionEditAction->HideScene(bShow);

		if(bShow)
		{
			
		}
	}

}


 // ѡ���Ѿ����������
void CGrowPointEditDlg::OnGrowPointAddedSelected(wxTreeEvent& event)
{

	if(m_pAddedGrowPointTreeCtrl)
	{
		wxTreeItemId TreeItem = event.GetItem();
		
		wxString strInfo = m_pAddedGrowPointTreeCtrl->GetItemText(TreeItem);

		unsigned long ulGrowPointInstance = 0;

		char szBuf[512];

		memset(szBuf, 0, sizeof(szBuf));
		sprintf(szBuf, "%s", strInfo.c_str());

		sscanf(szBuf, "%d", &ulGrowPointInstance);

		if(m_pGrowPointEditAction)
		{
			if(m_pGrowPointEditAction->m_GrowPointMap.count(ulGrowPointInstance))
			{
				m_pGrowPointEditAction->ViewGrowPos(ulGrowPointInstance);
				m_pGrowPointEditAction->SetUnSelectObjectLook(m_pGrowPointEditAction->m_dwCurSelGrowPointInstanceId);
				m_pGrowPointEditAction->SetSelectObjectLook(ulGrowPointInstance);
				m_pGrowPointEditAction->m_dwCurSelGrowPointInstanceId = ulGrowPointInstance;

				if(m_pParent)
				{
					m_pParent->SetFocus();
					m_pParent->GetCanvas()->SetFocus();
					m_pParent->GetCanvas()->Enable(true);
					
				}
			}
		}
		
	}//



}



// ѡ��һ�������������
void CGrowPointEditDlg::OnGrowPointTypeSelected(wxListEvent& event)
{
	if(m_pGrowPointListCtrl)
	{
		int iSelIndex = event.m_itemIndex;
		int iId = 0;
		if(iSelIndex >= 0)
		{
			iId = m_pGrowPointListCtrl->GetItemData(iSelIndex);
			if(m_IdMeshMap.count(iId))
			{
				if(m_pGrowPointEditAction)
				{
					std::string strMeshName = m_IdMeshMap[iId];
					m_pGrowPointEditAction->ChangeGrowPointMeshType(iId, strMeshName);
					
				}
			}

		}
		
	}//
}

void CGrowPointEditDlg::AddNewGrowPoint(unsigned long ulMonsterId, unsigned long ulInstanceId)
{

	Ogre::Vector3 pos(0, 0, 0);

	if(NULL == m_pAddedGrowPointTreeCtrl)
	{
		return;
	}

	if(m_pGrowPointEditAction)
	{
		pos = m_pGrowPointEditAction->GetGrowLogicPos(ulInstanceId);
	}

	if(0 == m_IdTreeItemMap.count(ulMonsterId))
	{
		// ���б����Ҳ�����Ӧ�Ĺ������ơ�
		wxString strAddNew;

		// ����һ������.
		strAddNew.Format(wxT("%d, x=%.2f, y=%.2f, z=%.2f"), ulInstanceId, pos.x, pos.y, pos.z);
		wxTreeItemId NewItemId = m_pAddedGrowPointTreeCtrl->AppendItem(m_AddedGrowPointRoot, strAddNew);
		m_IdTreeItemMap[ulInstanceId] = NewItemId;

	}
	else
	{
		wxString strAddNew			= "";
		wxString strCount			= "";
		wxString strOldName			= "";
		std::string strMonsterName  = "";
		wxTreeItemId NewItemId;

		// �õ�Ҫ�޸ĵĽڵ�.
		wxTreeItemId treeItemId = m_IdTreeItemMap[ulMonsterId];

		// �õ��ڵ�ĸ���.
		size_t iCount = m_pAddedGrowPointTreeCtrl->GetChildrenCount(treeItemId);
		iCount++;

		// ����һ������.
		//strAddNew = wxString::Format(wxT("%d"), ulInstanceId);
		strAddNew = wxString::Format(wxT("%d, x=%.2f,y=%.2f,z=%.2f"), ulInstanceId, pos.x, pos.y, pos.z);
		NewItemId = m_pAddedGrowPointTreeCtrl->AppendItem(treeItemId, strAddNew);
		
		// ��¼����ӵ����͵ľ��.
		m_AddedTreeItemMap[ulInstanceId] = NewItemId;

		// ��������ڵ�Ŀǰһ���ж��ٸ�����.
		strMonsterName = m_IdNameMap[ulMonsterId];
		strCount = wxString::Format(wxT("%s (%d)"), strMonsterName.c_str(), iCount);
		m_pAddedGrowPointTreeCtrl->SetItemText(treeItemId, strCount);

        // ȷ����ʾ
		m_pAddedGrowPointTreeCtrl->EnsureVisible(NewItemId);

		if(m_pNavigationDlg)
		{
			Ogre::Vector3 ScenePos(0, 0, 0);
			ScenePos = m_pGrowPointEditAction->GetGrowPos(ulInstanceId);
			m_pNavigationDlg->SetPointRGB(ScenePos.x, ScenePos.z, 2, 2, 0);
		}


	}

	// ��¼һ����ӵĹ��������
	wxString strCount;

	int iGrowPointCount = 0;
	if(m_pGrowPointEditAction)
	{
		iGrowPointCount = m_pGrowPointEditAction->m_GrowPointMap.size();
	}
	strCount = wxString::Format(wxT("�Ѿ���ӵĿ��� (%d)"), iGrowPointCount);
	m_pAddedGrowPointTreeCtrl->SetItemText(m_AddedGrowPointRoot, strCount);

	
}


void CGrowPointEditDlg::onAddNewGrowPoint(unsigned long ulTypeId, unsigned long ulInstanceId)
{
	AddNewGrowPoint(ulTypeId, ulInstanceId);
}

// ɾ��һ��������
void CGrowPointEditDlg::onDelGrowPoint( unsigned long ulTypeId, unsigned long ulInstanceId)
{
	DelGrowPoint(ulTypeId, ulInstanceId);
}
	
// ѡ��һ��������
void CGrowPointEditDlg::onSelGrowPoint( unsigned long ulTypeId, unsigned long ulInstanceId)
{
	

}

// ɾ��һ��������
void CGrowPointEditDlg::DelGrowPoint(unsigned long ulTypeId, unsigned long ulInstanceId)
{

	if(NULL == m_pAddedGrowPointTreeCtrl)
	{
		return;
	}

	if(0 == m_AddedTreeItemMap.count(ulInstanceId))
	{
	
		return;

	}
	else
	{

		
		if(m_pNavigationDlg)
		{
			Ogre::Vector3 ScenePos(0, 0, 0);
			ScenePos = m_pGrowPointEditAction->GetGrowPos(ulInstanceId);
			m_pNavigationDlg->SetPointRGB(ScenePos.x, ScenePos.z, 0, 0, 0);
		}


		wxString strAddNew = "";
		wxString strCount;
		wxString strOldName;
		std::string strGrowName = "";
		wxTreeItemId NewItemId;

		// �õ�Ҫ�޸ĵĽڵ�.
		wxTreeItemId treeItemId = m_IdTreeItemMap[ulTypeId];

		// �õ��ڵ�ĸ���.
		size_t iCount = m_pAddedGrowPointTreeCtrl->GetChildrenCount(treeItemId);
		iCount--;
	
		// ��������ڵ�Ŀǰһ���ж��ٸ�����.
		strGrowName = m_IdNameMap[ulTypeId];
		strCount = wxString::Format(wxT("%s (%d)"), strGrowName.c_str(), iCount);
		m_pAddedGrowPointTreeCtrl->SetItemText(treeItemId, strCount);

		// �õ�Ҫɾ���Ľڵ㡣��ɾ��.
		NewItemId = m_AddedTreeItemMap[ulInstanceId];
		ID_TREE_ITEM_MAP::iterator it;
		it = m_AddedTreeItemMap.find(ulInstanceId);
		if(it != m_AddedTreeItemMap.end())
		{
			m_AddedTreeItemMap.erase(it);
		}
		m_pAddedGrowPointTreeCtrl->Delete(NewItemId);


	}

	
	// ��¼һ����ӵĹ��������
	wxString strCount;

	int iGrowPointCount = 0;
	if(m_pGrowPointEditAction)
	{
		iGrowPointCount = m_pGrowPointEditAction->m_GrowPointMap.size();
	}
	strCount = wxString::Format(wxT("�Ѿ���ӵĹ��� (%d)"), iGrowPointCount);
	m_pAddedGrowPointTreeCtrl->SetItemText(m_AddedGrowPointRoot, strCount);


}

void CGrowPointEditDlg::ClearUI()
{
	m_AddedTreeItemMap.clear();
	m_IdTreeItemMap.clear();

	if(m_pAddedGrowPointTreeCtrl)
	{
		m_pAddedGrowPointTreeCtrl->DeleteAllItems();
	}//

	InitAddedGrowPointTree();
}


void CGrowPointEditDlg::SetActionMeshType()
{
	if(	NULL == m_pGrowPointEditAction)
	{
		return;
	}
	std::string strMeshName;
	m_pGrowPointEditAction->m_typeMeshMap.clear();

	unsigned long ulId;
	ID_NAME_DLG_MAP::iterator it;
	ID_NAME_DLG_MAP::iterator itEnd;

	itEnd = m_IdMeshMap.end();
	for(it = m_IdMeshMap.begin(); it != itEnd; it++)
	{

		if(	m_pGrowPointEditAction)
		{
			ulId = it->first;
			strMeshName = it->second;
			m_pGrowPointEditAction->AddGrowPointMeshType(ulId, strMeshName);
		}
		
	}
}


// ���ø�����
void CGrowPointEditDlg::SetParent(FairyEditorFrame* pParent)
{
	m_pParent = pParent;
}

