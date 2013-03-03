/////////////////////////////////////////////////////////////////////////////
// Name:        MonsterEditDlg.cpp
// Author:      XX
// Created:     XX/XX/XX
// Copyright:   XX
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "MonsterEditDlg.cpp"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "MonsterEditDlg.h"


#include <wx/splitter.h>
#include <wx/treectrl.h>
#include <wx/listctrl.h>

#include "MonsterEditAction.h"	// ����༭action
#include "SceneManipulator.h"
#include "CDlgChangeMonsterInfo.h"
#include "WXSJ_DBC.h"				// ��ȡexcel
#include "RegionEditAction.h"

#include "winsock.h"				// �����õ����ص�ip��ַ
#pragma comment( lib, "Ws2_32.lib" )

#define  DIR_STEP		0.3925f		
#define _2PI			6.28
#define DIR_STEP_TRANS	36
// WDR: class implementations
using namespace Fairy;

BEGIN_EVENT_TABLE(CMonsterEditDlg, wxPanel)

 	EVT_LIST_ITEM_SELECTED(ID_LISTCTRL_MOSTER_EDIT, CMonsterEditDlg::OnSelected)
	
	EVT_BUTTON(ID_BUTTON_MODIFY_MONSTER_INFO, CMonsterEditDlg::OnModifyMonsterInfo)
	EVT_BUTTON(ID_BUTTON_DIR_DEC, CMonsterEditDlg::OnDecMonsterDir)
	EVT_BUTTON(ID_BUTTON_DIR_INC, CMonsterEditDlg::OnIncMonsterDir)
	EVT_CHOICE(ID_CHOICE_TYPE, CMonsterEditDlg::OnSelectedType)
	EVT_CHOICE(ID_CHOICE_AI_FILE, CMonsterEditDlg::OnSelectedAIFile)
	EVT_TEXT_ENTER(ID_TEXTCTRL_MONSTER_REPUTATION_ID, CMonsterEditDlg::OnEditReputationID)

END_EVENT_TABLE()


CMonsterEditDlg::CMonsterEditDlg(const wxString& resourceGroup)
: m_pSceneManipulator(NULL)
, m_pMonsterEditAction(NULL)
, m_dwAddedMonsterCount(0)
, m_bIsModify(false)
// �����б�ؼ���
, m_pMonsterListCtrl(NULL)
// �Ѿ���ӵĹ����б�
, m_pAddedMonsterTreeCtrl(NULL)
// �޸İ�ť.
, m_pModifyBn(NULL)
// �޸Ķ�����ﰴť.
, m_pMultiModifyBn(NULL)
//������Χ��ť.
, pArea(NULL)
, pLine(NULL)
// ��뾶�ı���
, m_pAreaText(NULL)
// ��뾶�ı���
, m_pRefreshText(NULL)
//·�߱༭���ء�
, m_pLineCheck(NULL)
, m_pModifyMonsterBn(NULL)
, m_pDirDecBn(NULL)
, m_pDirIncBn(NULL)
// ��뾶�ı���
, m_pMonsterNameText(NULL)		// ��������
, m_pMonsterLevelText(NULL)		// ����ȼ�
, m_pMonsterExpText(NULL)		// ���ﾭ��
, m_pMonsterMoveSpeedText(NULL)	// �����ƶ��ٶ�
, m_pMonsterfreshTimeText(NULL)	// ����ˢ��ʱ��
, m_pMonsterRadiusText(NULL)	// �����뾶
, m_pMonsterCampIdText(NULL)	// ������Ӫid
, m_pMonsterBaseAIText(NULL)	// �������AI ID
, m_pMonsterAdvanceAIText(NULL)	// ����߼�AI�ļ���
, m_pMonsterEventText(NULL)		// �����¼��ļ�
, m_pMonsterGroupIdText(NULL)	// Ⱥid�� �������Ż�ʹ��
, m_pMonsterTeamIdText(NULL)	// �������id
, m_pMonsterDirText(NULL)
, m_pChoiceBaseAiId(NULL)
, m_pChoiceAIFile(NULL)
, m_pChoiceEvenFile(NULL)
, m_pMonsterGUIDText(NULL)
, m_pMonsterInstanceName(NULL)
, m_pMonsterTitleName(NULL)
, m_pMonsterPatrol(NULL)
, m_pMonsterType(NULL)
//, m_pShop1(NULL)					// �̵�1
//, m_pShop2(NULL)					// �̵�2
//, m_pShop3(NULL)					// �̵�3
//, m_pShop4(NULL)					// �̵�4//

//:m_pListboxRegion(NULL)
//,m_pListboxPointInRegion(NULL)//
{
    //Init(resourceGroup);
	//InitData();
	memset(m_pShop, 0, sizeof(m_pShop));
}

CMonsterEditDlg::CMonsterEditDlg(
							wxWindow *parent, 
							const wxString& resourceGroup,
							wxWindowID winid,
							const wxPoint& pos,
							const wxSize& size,
							long style,
							const wxString& name
							)
							: m_pSceneManipulator(NULL)
, m_pMonsterEditAction(NULL)
, m_dwAddedMonsterCount(0)
, m_bIsModify(false)
// �����б�ؼ���
, m_pMonsterListCtrl(NULL)
// �Ѿ���ӵĹ����б�
, m_pAddedMonsterTreeCtrl(NULL)
// �޸İ�ť.
, m_pModifyBn(NULL)
// �޸Ķ�����ﰴť.
, m_pMultiModifyBn(NULL)
//������Χ��ť.
, pArea(NULL)
, pLine(NULL)
// ��뾶�ı���
, m_pAreaText(NULL)
// ��뾶�ı���
, m_pRefreshText(NULL)
//·�߱༭���ء�
, m_pLineCheck(NULL)
, m_pModifyMonsterBn(NULL)
, m_pDirDecBn(NULL)
, m_pDirIncBn(NULL)
// ��뾶�ı���
, m_pMonsterNameText(NULL)		// ��������
, m_pMonsterLevelText(NULL)		// ����ȼ�
, m_pMonsterExpText(NULL)		// ���ﾭ��
, m_pMonsterMoveSpeedText(NULL)	// �����ƶ��ٶ�
, m_pMonsterfreshTimeText(NULL)	// ����ˢ��ʱ��
, m_pMonsterRadiusText(NULL)	// �����뾶
, m_pMonsterCampIdText(NULL)	// ������Ӫid
, m_pMonsterBaseAIText(NULL)	// �������AI ID
, m_pMonsterAdvanceAIText(NULL)	// ����߼�AI�ļ���
, m_pMonsterEventText(NULL)		// �����¼��ļ�
, m_pMonsterGroupIdText(NULL)	// Ⱥid�� �������Ż�ʹ��
, m_pMonsterTeamIdText(NULL)	// �������id
, m_pMonsterDirText(NULL)
, m_pChoiceBaseAiId(NULL)
, m_pChoiceAIFile(NULL)
, m_pChoiceEvenFile(NULL)
, m_pMonsterGUIDText(NULL)
, m_pMonsterInstanceName(NULL)
, m_pMonsterTitleName(NULL)
, m_pMonsterPatrol(NULL)
, m_pMonsterType(NULL)
//, m_pShop1(NULL)					// �̵�1
//, m_pShop2(NULL)					// �̵�2
//, m_pShop3(NULL)					// �̵�3
//, m_pShop4(NULL)					// �̵�4//

										
{
    
	//InitData();
	memset(m_pShop, 0, sizeof(m_pShop));
	Create(parent, winid, pos, size, style, name);

}


bool CMonsterEditDlg::Create(
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
	MonsterEditDlg(this, true, true);
	
	// ��ȡ���ؼ�������
	GetAllControl();

	// ��ʼ�������б�ı�ͷ.
	InitMonsterListCtrlTable();

	// ��ȡ����id������ӳ���ļ�����
	//LoadMonsterIdNameTable();

	// ��ʼ��tree�ṹ
	InitAddedMonsterTree();

	// �õ��������ai
	GetBaseAITable();

	// �õ��߼�ai
	GetAIFile();

	// �õ��¼��ű�
	GetEventFile();

	// �õ��̵���Ϣ
	GetShopInfo(); //commentout by yangfei

	InitMonsterType();

	return true;
}


// ��ʾ����
bool CMonsterEditDlg::Show(bool show)
{
	SetActive(show);
	return wxWindow::Show(show);
}

void CMonsterEditDlg::GetAction()
{

	// �õ�����༭�ĳ�������
	if(m_pSceneManipulator)
	{
		Fairy::Action* action = m_pSceneManipulator->_getAction("MonsterEdit");
		m_pMonsterEditAction = (CMonsterEditaction *)action;
	}

}


void CMonsterEditDlg::SetSceneManipulator(Fairy::SceneManipulator* sceneManipulator)
{
	m_pSceneManipulator = sceneManipulator;
	
	if(m_pSceneManipulator)
	{
		m_pSceneManipulator->addSceneListener(this);
		
		// �õ��ײ��action
		GetAction();
	}
}




void CMonsterEditDlg::SetActive(bool bShow)
{


	if(m_pSceneManipulator)
	{
		Fairy::Action* action = m_pSceneManipulator->_getAction("MonsterEdit");
		m_pMonsterEditAction = (CMonsterEditaction *)action;
	}

	if (bShow)
    {
		if(m_pSceneManipulator)
		{
			Fairy::Action* action = m_pSceneManipulator->_getAction("MonsterEdit");
			m_pSceneManipulator->setActiveAction(action);//
			m_pMonsterEditAction = (CMonsterEditaction *)action;
			
		}
       
    }
	else
	{
		if(m_pMonsterEditAction)
		{
			m_pSceneManipulator->setActiveAction(NULL);
		}
	}

	// ��ʾ���س���
	if(m_pMonsterEditAction)
	{
		m_pMonsterEditAction->HideScene(bShow);
		
		Fairy::Action* action = m_pSceneManipulator->_getAction("RegionEdit");
		Fairy::CRegionEditAction* pRegionEditAction = ( Fairy::CRegionEditAction*)action;
		pRegionEditAction->HideScene(bShow);
	
		if(bShow)
		{
			
		}
	}

}


// �õ����еĿؼ�.
void CMonsterEditDlg::GetAllControl()
{
	
	
	// �õ������б�ؼ�.
	m_pMonsterListCtrl = wxDynamicCast(FindWindow(ID_LISTCTRL_MOSTER_EDIT), wxListCtrl);

	// ����Ѿ���ӵĹ����б�Ŀؼ�.
	m_pAddedMonsterTreeCtrl =  wxDynamicCast(FindWindow(ID_TREECTRL_ADDED_MONSTER), wxTreeCtrl);

	if(m_pAddedMonsterTreeCtrl)
	{
		
		m_pAddedMonsterTreeCtrl->Show(false);

	}

	
	// �޸Ĺ�����Ϣ��ť.
	m_pModifyMonsterBn = wxDynamicCast(FindWindow(ID_BUTTON_MODIFY_MONSTER_INFO), wxButton);


	// ���ù���ķ���
	//m_pDirLTBn = wxDynamicCast(FindWindow(ID_BUTTON_LT), wxButton);
	//m_pDirTBn  = wxDynamicCast(FindWindow(ID_BUTTON_T), wxButton);
	//m_pDirRTBn = wxDynamicCast(FindWindow(ID_BUTTON_RT), wxButton);
	//m_pDirLBn  = wxDynamicCast(FindWindow(ID_BUTTON_L), wxButton);
	//m_pDirRBn  = wxDynamicCast(FindWindow(ID_BUTTON_R), wxButton);
	//m_pDirLBBn = wxDynamicCast(FindWindow(ID_BUTTON_LB), wxButton);
	//m_pDirBBn  = wxDynamicCast(FindWindow(ID_BUTTON_B), wxButton);
	//m_pDirRBBn = wxDynamicCast(FindWindow(ID_BUTTON_RB), wxButton);//

	// �������Ӱ�ť.
	m_pDirDecBn = wxDynamicCast(FindWindow(ID_BUTTON_DIR_DEC), wxButton);
	// ������ٰ�ť.
	m_pDirIncBn = wxDynamicCast(FindWindow(ID_BUTTON_DIR_INC), wxButton);//

	// �������֡�
	m_pMonsterNameText		= wxDynamicCast(FindWindow(ID_TEXTCTRL_MONSTER_NAME), wxTextCtrl);				// ��������
	if(m_pMonsterNameText)
	{
		m_pMonsterNameText->Enable(false);
		//m_pMonsterNameText->Show(false);
	}

	m_pMonsterLevelText		= wxDynamicCast(FindWindow(ID_TEXTCTRL_MONSTER_LEVEL), wxTextCtrl);				// ����ȼ�
	if(m_pMonsterLevelText)
	{
		//m_pMonsterLevelText->Enable(true);
	}

    m_pMonsterExpText		= wxDynamicCast(FindWindow(ID_TEXTCTRL_MONSTER_EXP), wxTextCtrl);				// ���ﾭ��
	if(m_pMonsterExpText)
	{
		m_pMonsterExpText->Enable(false);
	}

	m_pMonsterMoveSpeedText = wxDynamicCast(FindWindow(ID_TEXTCTRL_MONSTER_MOVE_SPEED), wxTextCtrl);		// �����ƶ��ٶ�
	if(m_pMonsterMoveSpeedText)
	{
		m_pMonsterMoveSpeedText->Enable(false);
	}

	m_pMonsterfreshTimeText = wxDynamicCast(FindWindow(ID_TEXTCTRL_MONSTER_REFRESH_TIME), wxTextCtrl);		// ����ˢ��ʱ��
	if(m_pMonsterfreshTimeText)
	{
		//m_pMonsterfreshTimeText->Enable(false);
	}

	m_pMonsterRadiusText	= wxDynamicCast(FindWindow(ID_TEXTCTRL_MONSTER_MVOE_RADIUS), wxTextCtrl);		// �����뾶
	if(m_pMonsterRadiusText)
	{
		m_pMonsterRadiusText->Enable(false);
		//m_pMonsterRadiusText->Show(false);
	}

	m_pMonsterCampIdText	= wxDynamicCast(FindWindow(ID_TEXTCTRL_MONSTER_CAMP_ID), wxTextCtrl);			// ������Ӫid
	//if(m_pMonsterCampIdText)
	//{
	//	m_pMonsterCampIdText->Enable(false);
	//	//m_pMonsterCampIdText->Show(false);
	//}
	// �������û�ֻ����ѡ��

	// ����ai
	m_pChoiceBaseAiId       = wxDynamicCast(FindWindow(ID_CHOICE_BASE_AI), wxChoice);						// �������AI ID
	m_pMonsterBaseAIText	= wxDynamicCast(FindWindow(ID_TEXTCTRL_MONSTER_BASE_AI_ID), wxTextCtrl);		// �������AI ID

	// �߼�ai
	m_pChoiceAIFile			= wxDynamicCast(FindWindow(ID_CHOICE_AI_FILE), wxChoice);						// ����߼�AI�ļ���
	m_pMonsterAdvanceAIText = wxDynamicCast(FindWindow(ID_TEXTCTRL_AI_FILE), wxTextCtrl);

	m_pChoiceEvenFile       = wxDynamicCast(FindWindow(ID_COMBO_EVEN_FILE), wxComboBox);	
	m_pMonsterEventText		= wxDynamicCast(FindWindow(ID_TEXTCTRL_EVEN_FILE), wxTextCtrl);					// �����¼��ļ�

	m_pMonsterGroupIdText	= wxDynamicCast(FindWindow(ID_TEXTCTRL_GROUP_ID), wxTextCtrl);					// Ⱥid�� �������Ż�ʹ��
	m_pMonsterTeamIdText	= wxDynamicCast(FindWindow(ID_TEXTCTRL_TEAM_ID), wxTextCtrl);					// �������id
	m_pMonsterDirText       = wxDynamicCast(FindWindow(ID_TEXTCTRL_DIRECT), wxTextCtrl);					// ����
	m_pMonsterType			= wxDynamicCast(FindWindow(ID_CHOICE_TYPE), wxChoice);							// ��������

	m_pShop[0] = wxDynamicCast(FindWindow(ID_CHOICE_SHOP1), wxChoice);										// �̵�1
	m_pShop[1] = wxDynamicCast(FindWindow(ID_CHOICE_SHOP2), wxChoice);										// �̵�2
	m_pShop[2] = wxDynamicCast(FindWindow(ID_CHOICE_SHOP3), wxChoice);										// �̵�3
	m_pShop[3] = wxDynamicCast(FindWindow(ID_CHOICE_SHOP4), wxChoice);										// �̵�4

	// ����ʵ������
	m_pMonsterInstanceName  = wxDynamicCast(FindWindow(ID_TEXTCTRL_INSTANCE_NAME), wxTextCtrl);	
	
	// ����ƺ�
	m_pMonsterTitleName		= wxDynamicCast(FindWindow(ID_TEXTCTRL_TITLE), wxTextCtrl);	

	// ����guid
	m_pMonsterGUIDText		= wxDynamicCast(FindWindow(ID_TEXTCTRL_GUID), wxTextCtrl);	

	// Ѳ��·��
	m_pMonsterPatrol        = wxDynamicCast(FindWindow(ID_TEXTCTRL_PATROL), wxTextCtrl);	
	if(m_pMonsterGUIDText)
	{
		//m_pMonsterGUIDText->Enable(false);
	}

	//
	m_pMonsterReputationID	= wxDynamicCast(FindWindow(ID_TEXTCTRL_MONSTER_REPUTATION_ID), wxTextCtrl);			// ��������id


	// ����ǰһ���汾�Ŀؼ����Ժ�ʹ��.
	m_pMonsterBaseAIText->Show(false);
	m_pMonsterAdvanceAIText->Show(false);
	m_pMonsterEventText->Show(false);
						
}

// ��ʼ�������б�ؼ���ͷ.
void CMonsterEditDlg::InitMonsterListCtrlTable()
{
	if(NULL == m_pMonsterListCtrl)
	{
		return;
	}
	m_pMonsterListCtrl->ClearAll();

	DBC::DBCFile excelDBFile(1);
	DBC::DBCFile excelDBFileModelName(1);
		                         
	if(!excelDBFile.OpenFromTXT("..\\..\\public\\config\\mon_attr_ex.tab"))//MonsterAttrExTable.txt
	{
		wxLogMessage(_T("���ܴ򿪷�����������Ϣ�� ���ܱ༭������Ϣ\n"));
        Enable(false);
        Hide();
		return;
	}

	if(!excelDBFileModelName.OpenFromTXT("..\\resource\\config\\char_model.tab"))
	{
		wxLogMessage(_T("���ܴ򿪷�����������Ϣ�� ���ܱ༭������Ϣ\n"));
		return;
	}

	std::map<int, std::string> Id_Model_Map;
	int iModelCount = excelDBFileModelName.GetRecordsNum();

	const DBC::DBCFile::FIELD* pInfo = NULL;	
	char bufInfo[512];
	int iId     = -1;
	for(int iIndex = 0; iIndex < iModelCount; iIndex++)
	{
		pInfo = excelDBFileModelName.Search_Posistion(iIndex, 0);
		iId = pInfo->iValue;
		
		// ��������
		pInfo = excelDBFileModelName.Search_Posistion(iIndex, 2);
		std::string strTemp = pInfo->pString;
		Id_Model_Map[iId] = strTemp;
	}//


    m_pMonsterListCtrl->InsertColumn(0, "������");	
    m_pMonsterListCtrl->InsertColumn(1, "��������");	
	m_pMonsterListCtrl->InsertColumn(2, "�ȼ�");		
    m_pMonsterListCtrl->InsertColumn(3, "���þ���");	
	m_pMonsterListCtrl->InsertColumn(4, "�ƶ��ٶ�");	
	m_pMonsterListCtrl->InsertColumn(5, "����ʱ��");		
    m_pMonsterListCtrl->InsertColumn(6, "���Χ");	
	m_pMonsterListCtrl->InsertColumn(7, "��ӪID");	

	int iRecordCount = excelDBFile.GetRecordsNum(); 
	int iLastColum   = excelDBFile.GetFieldsNum();
	iId     = -1;

	for(int i = 0; i < iRecordCount; i++)
	{
		// ��ȡ��������
		pInfo = excelDBFile.Search_Posistion(i, 0);
		memset(bufInfo, 0, sizeof(bufInfo));
		sprintf(bufInfo, "%d", pInfo->iValue);
		m_pMonsterListCtrl->InsertItem(i, bufInfo);
		//�󶨹���id
		m_pMonsterListCtrl->SetItemData(i, pInfo->iValue);
		iId = pInfo->iValue;
		
		// ��������.
		pInfo = excelDBFile.Search_Posistion(i, 1);
		memset(bufInfo, 0, sizeof(bufInfo));
		sprintf(bufInfo, "%s", pInfo->pString);
		m_pMonsterListCtrl->SetItem(i, 1, bufInfo);//

		// �ȼ�
		pInfo = excelDBFile.Search_Posistion(i, 2);
		memset(bufInfo, 0, sizeof(bufInfo));
		sprintf(bufInfo, "%d", pInfo->iValue);
		m_pMonsterListCtrl->SetItem(i, 2, bufInfo);

		// ���þ���
		pInfo = excelDBFile.Search_Posistion(i, 5);
		memset(bufInfo, 0, sizeof(bufInfo));
		sprintf(bufInfo, "%d", pInfo->iValue);
		m_pMonsterListCtrl->SetItem(i, 3, bufInfo);

		// �ƶ��ٶ�
		pInfo = excelDBFile.Search_Posistion(i, 40);
		memset(bufInfo, 0, sizeof(bufInfo));
		sprintf(bufInfo, "%d", pInfo->iValue);
		m_pMonsterListCtrl->SetItem(i, 4, bufInfo);
		
		//����ʱ��
		pInfo = excelDBFile.Search_Posistion(i, 3);
		memset(bufInfo, 0, sizeof(bufInfo));
		sprintf(bufInfo, "%d", pInfo->iValue);
		m_pMonsterListCtrl->SetItem(i, 5, bufInfo);

		// ��Ӫ
		pInfo = excelDBFile.Search_Posistion(i, 47);
		memset(bufInfo, 0, sizeof(bufInfo));
		sprintf(bufInfo, "%d", pInfo->iValue);
		m_pMonsterListCtrl->SetItem(i, 7, bufInfo);

		// ����id
		pInfo = excelDBFile.Search_Posistion(i, 33);
		m_IdNameMap[iId] = Id_Model_Map[pInfo->iValue];// ��������ID��29 �ĳ�33��


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////
		//// ��ȡ�ͻ���

		//// ID
        //pInfoClient = excelDBClient.Search_Posistion(i, 0);
		//iId = pInfoClient->iValue;
	
		//// ����id
		//pInfoClient = excelDBClient.Search_Posistion(i, 1);
		//iLookId     = pInfoClient->iValue;


		//// ��¼id������ӳ���.
		//m_Id_Look_IdMap[iId] = iLookId;
		//

		//// ��������.
		//pInfoClient = excelDBClient.Search_Posistion(i, 2);
		//memset(bufInfo, 0, sizeof(bufInfo));
		//sprintf(bufInfo, "%s", pInfoClient->pString);
		//m_pMonsterListCtrl->SetItem(i, 1, bufInfo);
			
	}
}


// ��ȡ����ai��
void CMonsterEditDlg::GetBaseAITable()
{
	if(NULL == m_pChoiceBaseAiId)
	{
		return;
	}
	m_pChoiceBaseAiId->Clear();

	char bufInfo[512];
	char bufKey[64];
	
	int iAICount = -1;
	iAICount = ::GetPrivateProfileInt("INFO", "AINUMBER", -1, "..\\..\\public\\config\\MonsterAITable.ini");
	for(int i = 0; i < iAICount; i++)
	{
		memset(bufKey,  0, sizeof(bufKey));
		memset(bufInfo, 0, sizeof(bufInfo));
		sprintf(bufKey, "AI%d", i);
		::GetPrivateProfileString(bufKey, "DESC", "none", bufInfo, sizeof(bufInfo), "..\\..\\public\\config\\MonsterAITable.ini");
		m_pChoiceBaseAiId->Insert(bufInfo, i);
	}

	if(m_pChoiceBaseAiId->GetCount())
	{
		m_pChoiceBaseAiId->Select(0);
	}

}

// ��ȡ�߼�ai
void CMonsterEditDlg::GetAIFile()
{
	if(NULL == m_pChoiceAIFile)
	{
		return;
	}
	m_pChoiceAIFile->Clear();


	FILE* pFile = NULL;
	pFile = ::fopen("..\\..\\public\\data\\AIScript.dat", "r");
	if(NULL == pFile)
	{
		return;
	}

	char bufInfo[512];
	int iCount = -1;
	int iId    = -1;
	int iPos   = -1;
	std::string strWork = "";
	std::string strNum  = "";

	while(!feof(pFile))
	{
		memset(bufInfo, 0, sizeof(bufInfo));
		::fgets(bufInfo, sizeof(bufInfo), pFile);
		strWork = bufInfo;
		iPos = strWork.find("=");

		if(iPos <= 0)
		{
			continue;
		}

		strNum = strWork.substr(0, iPos);
		iId    = atoi(strNum.c_str());
		iCount++;
		m_pChoiceAIFile->Insert(bufInfo, iCount, (void*)iId);

	}

	if(m_pChoiceAIFile->GetCount())
	{
		m_pChoiceAIFile->Select(0);
	}

	::fclose(pFile);//
}

void CMonsterEditDlg::InitMonsterType()
{
	if(NULL == m_pMonsterType)
	{
		return;
	}
	m_pMonsterType->Clear();

	INT iId = 0;
	m_pMonsterType->Insert("0 =MONSTER", 0, (void*)iId);

	iId = 1;
	m_pMonsterType->Insert("1 =NPC", 1, (void*)iId);

	m_pMonsterType->Select(1);
}

// ��ȡ�¼��ű�
void CMonsterEditDlg::GetEventFile()
{
	if(NULL == m_pChoiceEvenFile)
	{
		return;
	}

	m_pChoiceEvenFile->Clear();

	FILE* pFile = NULL;
	pFile = ::fopen("..\\..\\public\\data\\Script.dat", "r");
	if(NULL == pFile)
	{
		return;
	}

	char bufInfo[512];
	int iCount = -1;
	int iId    = -1;
	int iPos   = -1;
	std::string strWork = "";
	std::string strNum  = "";

	strNum = "-1=�޽ű�";
	iId    = -1;
	iCount++;
	m_pChoiceEvenFile->Insert(strNum.c_str(), iCount, (void*)iId);

	while(!feof(pFile))
	{
		memset(bufInfo, 0, sizeof(bufInfo));
		::fgets(bufInfo, sizeof(bufInfo), pFile);
		strWork = bufInfo;
		iPos = strWork.find("=");

		if(iPos <= 0)
		{
			continue;
		}

		strNum = strWork.substr(0, iPos);
		iId    = atoi(strNum.c_str());
		iCount++;
		m_pChoiceEvenFile->Insert(bufInfo, iCount, (void*)iId);

	}

	if(m_pChoiceEvenFile->GetCount())
	{
		m_pChoiceEvenFile->Select(0);
	}

	::fclose(pFile);//

}//



// ��ʼ�������б�ؼ���ͷ.
void CMonsterEditDlg::GetShopInfo()
{
	if((NULL == m_pShop[0])
		||(NULL == m_pShop[1])
		||(NULL == m_pShop[2])
		||(NULL == m_pShop[3]))
	{
		wxLogMessage(_T("�̵�ؼ�û�г�ʼ���ɹ�.\n"));
        Enable(false);
        Hide();
		return;
	}

	DBC::DBCFile excelDBFile(1);
	if(!excelDBFile.OpenFromTXT("..\\..\\public\\config\\shop_tab.tab"))//ShopTable.txt
	{
		wxLogMessage(_T("���ܴ��̵���Ϣ��.\n"));
        Enable(false);
        Hide();
		return;
	}

	int iRecordCount = excelDBFile.GetRecordsNum(); 
	int iLastColum   = excelDBFile.GetFieldsNum();
	const DBC::DBCFile::FIELD* pInfo = NULL;	
	char bufInfo[512];
    int i;
	for(i = 0; i < iRecordCount; i++)
	{
		// ��ȡ�̵�����,��id
		pInfo = excelDBFile.Search_Posistion(i, 0);
		memset(bufInfo, 0, sizeof(bufInfo));
		sprintf(bufInfo, "%d", pInfo->iValue);
		m_pShop[0]->Insert(bufInfo, i, (void*)pInfo->iValue);
		m_pShop[1]->Insert(bufInfo, i, (void*)pInfo->iValue);
		m_pShop[2]->Insert(bufInfo, i, (void*)pInfo->iValue);
		m_pShop[3]->Insert(bufInfo, i, (void*)pInfo->iValue);
	
	}

	m_pShop[0]->Insert("��", i, (void*)-1);
	m_pShop[1]->Insert("��", i, (void*)-1);
	m_pShop[2]->Insert("��", i, (void*)-1);
	m_pShop[3]->Insert("��", i, (void*)-1);

	
	m_pShop[0]->Select(i);
	m_pShop[1]->Select(i);
	m_pShop[2]->Select(i);
	m_pShop[3]->Select(i);

  

	
}



//������������Ѿ�����������
// �õ�����id�� �����ֵ�ӳ���.
void CMonsterEditDlg::LoadMonsterIdNameTable()
{


	//DBC::DBCFile excelDBFile(1);
	//if(!excelDBFile.OpenFromTXT("..\\LogicEditData\\monster\\MonsterAttrExTable.txt"))
	//{
	//	wxLogMessage(_T("���ܴ򿪷�����������Ϣ�� ���ܱ༭������Ϣ\n"));
	//	return;
	//}
	FILE* pFile = NULL;
	pFile = ::fopen("..\\..\\Public\\Config\\mon_attr_ex.tab", "r");//MonsterAttrExTable.txt

	char buf[2048];
	::fgets(buf, sizeof(buf), pFile);
	::fgets(buf, sizeof(buf), pFile);

	// ����id
	char bufId[64];

	// ���������ļ�����
	char bufName[128];

	m_IdNameMap.clear();

	int iId = -1;
	while(!feof(pFile))
	{
		memset(buf, 0, sizeof(buf));
		memset(bufId, 0, sizeof(bufId));
		memset(bufName, 0, sizeof(bufName));

		// �õ�һ������
		::fgets(buf, sizeof(buf), pFile);
		sscanf(buf, "%s %s ", bufId, bufName);

		if((0 == strcmp("", bufId)) || (0 == strcmp("#", bufId)))
		{
			continue;
		}
		iId = atoi(bufId);
		m_IdNameMap[iId] = bufName;

	}

	std::string strstr = m_IdNameMap[918];

	::fclose(pFile);//

}

void CMonsterEditDlg::OnSelected(wxListEvent& event)
{
	
	if(m_pMonsterListCtrl)
	{
		//int iIndex = m_pMonsterListCtrl->getitems();
		int iSelIndex = event.m_itemIndex;
		int iId     = -1;
		if(iSelIndex >= 0)
		{

			m_iCurSelListCtrlIndex = iSelIndex;
			//ShowMonsterTypeInfo(iSelIndex);
			ShowMonsterTypeInfoByIndex(iSelIndex);
			iId = m_pMonsterListCtrl->GetItemData(iSelIndex);
			if(m_IdNameMap.count(iId))
			{
				if(m_pMonsterEditAction)
				{
					std::string strObjName = m_IdNameMap[iId];
					m_pMonsterEditAction->SetNewEditObject(strObjName.c_str(), iId);
					
				}
			}

			ReSetDataInfo();

			////
			//// ���Ի�����
			////
			//if(m_pChoiceEvenFile->GetCount())
			//{
			//	m_pChoiceEvenFile->Select(0);
			//}

			//// ����ai
			//if(m_pChoiceBaseAiId)
			//{
			//	m_pChoiceBaseAiId->Select(0);
			//}

			//// �߼�ai
			//if(m_pChoiceAIFile)
			//{
			//	m_pChoiceAIFile->Select(0);
			//}


			//m_pMonsterTeamIdText->SetLabel("-1");		// �������id
			//m_pMonsterDirText->SetLabel("0.00");		// ���﷽��
			//m_pMonsterInstanceName->SetLabel("");		// ����ʵ������
			//m_pMonsterTitleName->SetLabel("");			// ����ƺ�����
			//m_pMonsterPatrol->SetLabel("-1");			// ����Ѳ��·��//

		}
		
	}//
}


// ��ʼ�������б���.
void CMonsterEditDlg::InitAddedMonsterTree()
{
	if((m_pAddedMonsterTreeCtrl)&&(m_pMonsterListCtrl))
	{
		m_pAddedMonsterTreeCtrl->DeleteAllItems();
		m_AddedMonsterRoot = m_pAddedMonsterTreeCtrl->AddRoot("�Ѿ���ӵĹ���");

		wxTreeItemId newTreeItemId;
		std::string  strName = "";

		m_Id_TreeItem_Map.clear();

		int iCount = m_pMonsterListCtrl->GetItemCount();
		for(int i = 0; i < iCount; i++)
		{
			strName = m_pMonsterListCtrl->GetItemText(i);
			newTreeItemId = m_pAddedMonsterTreeCtrl->AppendItem(m_AddedMonsterRoot, strName.c_str());
			m_Id_TreeItem_Map[m_pMonsterListCtrl->GetItemData(i)] = newTreeItemId;
		}
	}//
}

// ֪ͨ����Ļص����� ���һ������
void CMonsterEditDlg::onAddNewMonster(unsigned long ulMonsterId, unsigned long ulInstanceId)
{
	// �Ѿ������޸ġ�
	m_bIsModify = true;
	AddNewMonster(ulMonsterId, ulInstanceId);

}


// ֪ͨ����Ļص����� ɾ��һ������
void CMonsterEditDlg::onDelMonster( unsigned long ulMonsterId, unsigned long ulInstanceId)
{

	// �Ѿ������޸ġ�
	m_bIsModify = true;
	DelMonster(ulMonsterId, ulInstanceId);

}


	// ���һ������
void CMonsterEditDlg::AddNewMonster(unsigned long ulMonsterId, unsigned long ulInstanceId)
{

	if(NULL == m_pAddedMonsterTreeCtrl)
	{
		return;
	}

	if(0 == m_Id_TreeItem_Map.count(ulMonsterId))
	{
		// ���б����Ҳ�����Ӧ�Ĺ������ơ�
		wxString strAddNew;
	
		// ����һ������.
		strAddNew.Format(wxT("%d"), ulInstanceId);
		//m_pAddedMonsterTreeCtrl->AppendItem(m_AddedMonsterRoot, strAddNew, -1, -1, (wxTreeItemData *)ulInstanceId);
		wxTreeItemId NewItemId = m_pAddedMonsterTreeCtrl->AppendItem(m_AddedMonsterRoot, strAddNew);
		m_Id_AddedMonsterMap[ulInstanceId] = NewItemId;

	}
	else
	{
		wxString strAddNew			= "";
		wxString strCount			= "";
		wxString strOldName			= "";
		std::string strMonsterName  = "";
		wxTreeItemId NewItemId;

		// �õ�Ҫ�޸ĵĽڵ�.
		wxTreeItemId treeItemId = m_Id_TreeItem_Map[ulMonsterId];

		// �õ��ڵ�ĸ���.
		size_t iCount = m_pAddedMonsterTreeCtrl->GetChildrenCount(treeItemId);
		iCount++;

		// ����һ������.
		strAddNew = wxString::Format(wxT("%d"), ulInstanceId);
		//m_pAddedMonsterTreeCtrl->AppendItem(treeItemId, strAddNew, -1, -1, (wxTreeItemData *)ulInstanceId);
		NewItemId = m_pAddedMonsterTreeCtrl->AppendItem(treeItemId, strAddNew);
		m_Id_AddedMonsterMap[ulInstanceId] = NewItemId;

		// ��������ڵ�Ŀǰһ���ж��ٸ�����.
		strMonsterName = m_IdNameMap[ulMonsterId];
		strCount = wxString::Format(wxT("%s (%d)"), strMonsterName.c_str(), iCount);
		m_pAddedMonsterTreeCtrl->SetItemText(treeItemId, strCount);
	}

	// ��¼һ����ӵĹ��������
	wxString strCount;

	m_dwAddedMonsterCount++;
	strCount = wxString::Format(wxT("�Ѿ���ӵĹ��� (%d)"), m_dwAddedMonsterCount);
	m_pAddedMonsterTreeCtrl->SetItemText(m_AddedMonsterRoot, strCount);

	// ��ǰ�༭��ʵ��
	m_ulCurEditMonsterInstance = ulInstanceId;

	INSTANCE_MONSTERINFO_MAP::iterator it;
	it = m_MonsterInfoTable.find(-1);

	if(it != m_MonsterInfoTable.end())
	{
		CMonsterInstanceInfo monsterInfo = it->second;
		monsterInfo.m_dwInstanceId		 = ulInstanceId;

		// ���
		m_MonsterInfoTable[ulInstanceId] = monsterInfo;
		if(m_pMonsterEditAction)
		{
			m_pMonsterEditAction->ChangeMonsterDir(ulInstanceId, monsterInfo.m_fMonstDir);
		}

		// ɾ����ʱ����
		m_MonsterInfoTable.erase(it);
	}
	else
	{
		CMonsterInstanceInfo monsterInfo;

		monsterInfo.m_dwInstanceId		= ulInstanceId;
		monsterInfo.m_dwMonsterGUID		= GetGuid();
		monsterInfo.m_dwObjectId		= ulMonsterId;
		monsterInfo.m_strInstanceName	= GetInstanceName();
		monsterInfo.m_strTitleName	    = GetTitleName();
		monsterInfo.m_position          = GetMonsterPos();
		monsterInfo.m_fMonstDir         = GetDir();
		monsterInfo.m_EvenId			= GetEvenId();
		monsterInfo.m_iRefreshTime      = GetRefreshTime();
		monsterInfo.m_iReputationID		= GetReputationID();
		if(monsterInfo.m_iRefreshTime < 0)
		{
			monsterInfo.m_iRefreshTime = 1000;
		}
		monsterInfo.m_iGroupId			= GetGroupId();
		monsterInfo.m_iTeamId			= GetTeamId();
		monsterInfo.m_iBaseAIId			= GetBaseAIId();
		monsterInfo.m_iAdvanceAIId		= GetAdvanceAIId();
		monsterInfo.m_iLineid           = GetPatrolId();
		monsterInfo.m_iLevel			= GetLevel();
		monsterInfo.m_iType				= GetMonsterType();
		monsterInfo.m_iCampId			= GetCamp();
		m_MonsterInfoTable[ulInstanceId] = monsterInfo;
	}

	// ��ʾ����.
	ShowMonsterInfo(ulInstanceId);
}
		
// ɾ��һ������.
void CMonsterEditDlg::DelMonster(unsigned long ulMonsterId, unsigned long ulInstanceId)
{

	//if(NULL == m_pAddedMonsterTreeCtrl)
	//{
	//	return;
	//}

	//if(0 == m_Id_TreeItem_Map.count(ulMonsterId))
	//{
	//	
	//	//// ���б����Ҳ�����Ӧ�Ĺ������ơ�
	//	//wxString strAddNew;
	//
	//	//// ����һ������.
	//	//strAddNew.Format(wxT("%d"), ulInstanceId);
	//	////m_pAddedMonsterTreeCtrl->AppendItem(m_AddedMonsterRoot, strAddNew, -1, -1, (wxTreeItemData *)ulInstanceId);
	//	//m_pAddedMonsterTreeCtrl->AppendItem(m_AddedMonsterRoot, strAddNew);

	//	return;

	//}
	//else
	//{
	//	wxString strAddNew = "";
	//	wxString strCount;
	//	wxString strOldName;
	//	std::string strMonsterName = "";
	//	wxTreeItemId NewItemId;

	//	if(0 == m_Id_AddedMonsterMap.count(ulInstanceId))
	//	{
	//		return;
	//	}

	//	// �õ�Ҫ�޸ĵĽڵ�.
	//	wxTreeItemId treeItemId = m_Id_TreeItem_Map[ulMonsterId];

	//	// �õ��ڵ�ĸ���.
	//	size_t iCount = m_pAddedMonsterTreeCtrl->GetChildrenCount(treeItemId);
	//	iCount--;
	//
	//	// ��������ڵ�Ŀǰһ���ж��ٸ�����.
	//	strMonsterName = m_IdNameMap[ulMonsterId];
	//	strCount = wxString::Format(wxT("%s (%d)"), strMonsterName.c_str(), iCount);
	//	m_pAddedMonsterTreeCtrl->SetItemText(treeItemId, strCount);

	//	// �õ�Ҫɾ���Ľڵ㡣��ɾ��.
	//	NewItemId = m_Id_AddedMonsterMap[ulInstanceId];
	//	ID_TREE_ITEM_MAP::iterator it;
	//	it = m_Id_AddedMonsterMap.find(ulInstanceId);
	//	if(it != m_Id_AddedMonsterMap.end())
	//	{
	//		m_Id_AddedMonsterMap.erase(it);
	//	}
	//	m_pAddedMonsterTreeCtrl->Delete(NewItemId);


	//}

	//// ��¼һ����ӵĹ��������
	//wxString strCount;

	//m_dwAddedMonsterCount--;
	//strCount = wxString::Format(wxT("�Ѿ���ӵĹ��� (%d)"), m_dwAddedMonsterCount);
	//m_pAddedMonsterTreeCtrl->SetItemText(m_AddedMonsterRoot, strCount);//

	INSTANCE_MONSTERINFO_MAP::iterator it;

	it = m_MonsterInfoTable.find(ulInstanceId);
	if(it != m_MonsterInfoTable.end())
	{
		m_MonsterInfoTable.erase(it);
	}

	
}

// ɾ������.
void CMonsterEditDlg::DelMonster()
{
	if(m_pMonsterEditAction)
	{
		m_pMonsterEditAction->DelSelectMonster();
	}

}


// �Ƿ��ֹ����༭
void CMonsterEditDlg::EnableAreaEdit(bool bEnable)
{

	// ��������ťѡ��
	if(pArea)
	{
		pArea->SetValue(bEnable);
	}

	// ����·�߰�ť��ѡ��
	if(pLine)
	{
		pLine->SetValue(!bEnable);
	}

	// ��뾶�ı���
	if(m_pAreaText)
	{
		m_pAreaText->Enable(bEnable);
	}

	if(m_pLineCheck)
	{
		m_pLineCheck->Enable(!bEnable);
	}
	

}

 // �޸Ĺ������
void CMonsterEditDlg::OnModifyData( wxCommandEvent &event )
{
	// �Ѿ������޸ġ�
	m_bIsModify = true;
	ModifySelectData();
}

 // �޸Ķ���������
void CMonsterEditDlg::OnModifyMultiData( wxCommandEvent &event )
{
	// �Ѿ������޸ġ�
	m_bIsModify = true;
	ModifyMultiData();
}


// �޸Ĺ�����Ϣ
void CMonsterEditDlg::OnModifyMonsterInfo( wxCommandEvent &event )
{
	// �Ѿ������޸ġ�
	m_bIsModify = true;
	//ModifyMultiData();

	ModifyMonsterInfo();
}

// ���ٹ���ķ���
void CMonsterEditDlg::OnDecMonsterDir( wxCommandEvent &event )
{
	// �Ѿ������޸ġ�
	m_bIsModify = true;

	// ���ٹ���ķ���
	DecMonsterDir();

		
	
}

// ���ӹ��﷽��
void CMonsterEditDlg::OnIncMonsterDir( wxCommandEvent &event )
{
	// �Ѿ������޸ġ�
	m_bIsModify = true;
	
	// ���ӹ��﷽��
	IncMonsterDir();
	
}
void CMonsterEditDlg::OnSelectedType( wxCommandEvent &event )
{
	m_bIsModify = true;

}
void CMonsterEditDlg::OnSelectedAIFile( wxCommandEvent &event )
{
	m_bIsModify = true;

}

void CMonsterEditDlg::OnEditReputationID( wxCommandEvent &event )
{
	m_bIsModify = true;
}

// �޸�ѡ��Ĺ���Ĳ���.
void CMonsterEditDlg::ModifyMultiData()
{

	if(wxMessageBox(_T("��ȷ���ı����ֵ�������������û��redo undo��\n"), _T("�޸�"), wxICON_QUESTION | wxYES_NO, this) == wxNO)
	{
		return;
	}

	wxTreeItemId Treeitem;
	if(NULL == m_pAddedMonsterTreeCtrl)
	{
		return;
	}

	if(NULL == m_pMonsterEditAction)
	{
		return;
	}

	Treeitem = m_pAddedMonsterTreeCtrl->GetSelection();

	if(!Treeitem.IsOk())
	{
		return;
	}
		
	CDlgChangeMosterInfo dlg(this, wxID_ANY, _("Change_Mosnter_Info"));
	dlg.ShowModal();

	switch(dlg.m_iChangeType)
	{
		case CDlgChangeMosterInfo::TIME:
			{
				if(Treeitem == m_AddedMonsterRoot)
				{
					m_pMonsterEditAction->ChangeAllMonsterRefreshTime(dlg.m_iTime);
					return;
				}

				unsigned long dwMonsterId = GetMonsterIdByTreeItem(Treeitem); 
				m_pMonsterEditAction->ChangeMonsterRefreshTime_Type(dwMonsterId, dlg.m_iTime);



			}
			break;
		case CDlgChangeMosterInfo::RADIUS:
			{
				if(Treeitem == m_AddedMonsterRoot)
				{
					m_pMonsterEditAction->ChangeAllMonsterRadius(dlg.m_fRadius);
					return;
				}

				unsigned long dwMonsterId = GetMonsterIdByTreeItem(Treeitem); 
				m_pMonsterEditAction->ChangeMonsterRefreshRadius_Type(dwMonsterId, dlg.m_fRadius);


			}
			break;
		case CDlgChangeMosterInfo::BOTH:
			{
				if(Treeitem == m_AddedMonsterRoot)
				{
					m_pMonsterEditAction->ChangeAllMonsterRefreshTimeRadius(dlg.m_iTime, dlg.m_fRadius);
					return;
				}

				unsigned long dwMonsterId = GetMonsterIdByTreeItem(Treeitem); 
				m_pMonsterEditAction->ChangeMonsterRefreshTimeRadius_Type(dwMonsterId, dlg.m_iTime, dlg.m_fRadius);

			}
			break;
	}

}

// �޸�ѡ��Ĺ���Ĳ���
void CMonsterEditDlg::ModifySelectData()
{
	
	if(m_pMonsterEditAction)
	{
		CMonsterInfo* pMonsterInfo = m_pMonsterEditAction->GetMonster(m_pMonsterEditAction->m_dwCurSelMonsterInstanceId);
		
		if(NULL == pMonsterInfo)
		{
			return;
		}

		wxString strValue = "";
		char bufInfo[512];

		if(m_pMonsterLevelText)
		{
			memset(bufInfo, 0, sizeof(bufInfo));
			strValue  = m_pMonsterLevelText->GetValue();
			sprintf(bufInfo, "%s", strValue.c_str());
			pMonsterInfo->m_iLevel = atoi(bufInfo);
		}

		if(m_pMonsterBaseAIText)
		{
			memset(bufInfo, 0, sizeof(bufInfo));
			strValue  = m_pMonsterBaseAIText->GetValue();
			sprintf(bufInfo, "%s", strValue.c_str());
			pMonsterInfo->m_iBaseAIId = atoi(bufInfo);
		}

		if(m_pMonsterAdvanceAIText)
		{
			pMonsterInfo->m_strAdvanceAIFile = m_pMonsterAdvanceAIText->GetValue().c_str();
		}

		if(m_pMonsterEventText)
		{
			pMonsterInfo->m_strEvent = m_pMonsterEventText->GetValue().c_str();
		}

		if(m_pMonsterGroupIdText)
		{
			memset(bufInfo, 0, sizeof(bufInfo));
			strValue  = m_pMonsterGroupIdText->GetValue();
			sprintf(bufInfo, "%s", strValue.c_str());
			pMonsterInfo->m_iGroupId = atoi(bufInfo);
		}

		if(m_pMonsterTeamIdText)
		{
			memset(bufInfo, 0, sizeof(bufInfo));
			strValue  = m_pMonsterTeamIdText->GetValue();
			sprintf(bufInfo, "%s", strValue.c_str());
			pMonsterInfo->m_iTeamId = atoi(bufInfo);
		}

		if(m_pMonsterDirText)
		{
			memset(bufInfo, 0, sizeof(bufInfo));
			strValue  = m_pMonsterDirText->GetValue();
			sprintf(bufInfo, "%s", strValue.c_str());
			pMonsterInfo->m_fMonstDir = atof(bufInfo);
		}//

	}
			
		//wxString strValue = "";
		//strValue = wxString::Format(wxT("%.4f"), pMonsterInfo->m_fAreaRadius);
		//m_pAreaText->SetLabel(strValue);//

}


void CMonsterEditDlg::onSelMonster( unsigned long ulMonsterId, unsigned long ulInstanceId)
{
	SelMonster(ulMonsterId, ulInstanceId);
}
void CMonsterEditDlg::onMoveMonster( unsigned long ulMonsterId, Ogre::Vector3& pos)
{
	CMonsterInstanceInfo& MonsterInfo = m_MonsterInfoTable[ulMonsterId];
	MonsterInfo.m_position = pos;

}

void CMonsterEditDlg::SelMonster( unsigned long ulMonsterId, unsigned long ulInstanceId)
{
	if(0 == m_Id_AddedMonsterMap.count(ulInstanceId))
	{
		return;
	}

	if(NULL == m_pAddedMonsterTreeCtrl)
	{
		return;
	}

	//// ������֦�б���Ϣ
	//wxTreeItemId TreeItem= m_Id_AddedMonsterMap[ulInstanceId];
	//m_pAddedMonsterTreeCtrl->SelectItem(TreeItem);
	//m_pAddedMonsterTreeCtrl->EnsureVisible(TreeItem);

	// ���õ�ǰ�༭�Ĺ���ʵ��id
	m_ulCurEditMonsterInstance = ulInstanceId;

	// ��ʾ�������Ϣ.
	ShowMonsterInfo(ulInstanceId);

	// ��ʾ����������Ϣ��
	ShowMonsterTypeInfo(ulMonsterId);

}


// ��ʾѡ�еĹ������Ϣ
void CMonsterEditDlg::ShowMonsterInfo( unsigned long ulInstanceId)
{
	if(m_MonsterInfoTable.count(m_ulCurEditMonsterInstance))
	{
		
		CMonsterInstanceInfo& MonsterInfo = m_MonsterInfoTable[m_ulCurEditMonsterInstance];
				
		char bufInfo[512];

		// ����ai
		//if(m_pMonsterBaseAIText)
		//{
		//	memset(bufInfo, 0, sizeof(bufInfo));
		//	sprintf(bufInfo, "%d", MonsterInfo.m_iBaseAIId);
		//	m_pMonsterBaseAIText->SetLabel(bufInfo);// �������AI ID
		//}//

		if(m_pChoiceBaseAiId)
		{
			m_pChoiceBaseAiId->Select(MonsterInfo.m_iBaseAIId);
		}

		// �߼�ai
		//if(m_pMonsterAdvanceAIText)
		//{
		//	memset(bufInfo, 0, sizeof(bufInfo));
		//	sprintf(bufInfo, "%d", MonsterInfo.m_iAdvanceAIId);
		//	m_pMonsterAdvanceAIText->SetLabel(bufInfo);
		//}//
		if(m_pChoiceAIFile)
		{
			m_pChoiceAIFile->Select(MonsterInfo.m_iAdvanceAIId);
		}

		if(m_pMonsterType)
		{
			m_pMonsterType->Select(MonsterInfo.m_iType);
		}

		// �¼�id
		//if(m_pMonsterEventText)
		//{
		//	memset(bufInfo, 0, sizeof(bufInfo));
		//	sprintf(bufInfo, "%d", MonsterInfo.m_EvenId);
		//	m_pMonsterEventText->SetLabel(bufInfo);
		//}//

		if(m_pChoiceEvenFile)
		{
			int iCount = m_pChoiceEvenFile->GetCount();
			std::string strName = "";
			std::string strNum  = "";
			int iPos = -1;

			for(int i = 0; i < iCount; i++)
			{
				strName = m_pChoiceEvenFile->GetString(i);
				iPos = strName.find("=");

				if(iPos <= 0)
				{
					continue;
				}

				strNum = strName.substr(0, iPos);
				if(MonsterInfo.m_EvenId == atoi(strNum.c_str()))
				{
					m_pChoiceEvenFile->Select(i);
					break;
				}

			}
			
		}

		if(m_pMonsterLevelText)
		{
			memset(bufInfo, 0, sizeof(bufInfo));
			sprintf(bufInfo, "%d", MonsterInfo.m_iLevel);
			m_pMonsterLevelText->SetLabel(bufInfo);
		}

		// ��id
		if(m_pMonsterGroupIdText)
		{
			memset(bufInfo, 0, sizeof(bufInfo));
			sprintf(bufInfo, "%d", MonsterInfo.m_iGroupId);
			m_pMonsterGroupIdText->SetLabel(bufInfo);
		}

		// ��id
		if(m_pMonsterTeamIdText)
		{
			memset(bufInfo, 0, sizeof(bufInfo));
			sprintf(bufInfo, "%d", MonsterInfo.m_iTeamId);
			m_pMonsterTeamIdText->SetLabel(bufInfo);
		}

		// ����
		if(m_pMonsterDirText)
		{
			memset(bufInfo, 0, sizeof(bufInfo));
			sprintf(bufInfo, "%.4f", MonsterInfo.m_fMonstDir);
			m_pMonsterDirText->SetLabel(bufInfo);
		}

		// guid
		if(m_pMonsterGUIDText)
		{
			memset(bufInfo, 0, sizeof(bufInfo));
			sprintf(bufInfo, "%d", MonsterInfo.m_dwMonsterGUID);
			m_pMonsterGUIDText->SetLabel(bufInfo);
		}


		// ʵ������
		if(m_pMonsterInstanceName)
		{
			m_pMonsterInstanceName->SetLabel(MonsterInfo.m_strInstanceName.c_str());
		}

		// �ƺ�
		if(m_pMonsterTitleName)
		{
			m_pMonsterTitleName->SetLabel(MonsterInfo.m_strTitleName.c_str());
		}

		// Ѳ��id
		if(m_pMonsterPatrol)
		{
			memset(bufInfo, 0, sizeof(bufInfo));
			sprintf(bufInfo, "%d", MonsterInfo.m_iLineid);
			m_pMonsterPatrol->SetLabel(bufInfo);
		}

		// ˢ��ʱ��
        if(m_pMonsterfreshTimeText)
		{
			memset(bufInfo, 0, sizeof(bufInfo));
			sprintf(bufInfo, "%d", MonsterInfo.m_iRefreshTime);
			m_pMonsterfreshTimeText->SetLabel(bufInfo);
		}

		// ����
		if( m_pMonsterReputationID && MonsterInfo.m_iReputationID >= 0 )
		{
			memset( bufInfo, 0, sizeof(bufInfo) );
			sprintf( bufInfo, "%d", MonsterInfo.m_iReputationID );
			m_pMonsterReputationID->SetLabel(bufInfo);
		}
		else
			m_pMonsterReputationID->SetLabel("-1");

		// ��ʾ�̵�
		int iShopSel = -1;
		int iValue   = -1;
		for(int i = 0; i < SHOP_COUNT; i++)
		{
			iShopSel = MonsterInfo.m_iShopArray[i];
			if(-1 == iShopSel)
			{
				m_pShop[i]->Select(m_pShop[i]->GetCount()-1);
			}
			else
			{
				for(int iItem = 0; iItem < m_pShop[i]->GetCount(); iItem++)
				{
					iValue = atoi(m_pShop[i]->GetString(iItem));
					if(iValue == iShopSel)
					{
						m_pShop[i]->Select(iItem);
						break;
					}
					
				}
			}
		}


		
	}

}

// ͨ��treeitem�õ���������id
unsigned long CMonsterEditDlg::GetMonsterIdByTreeItem(wxTreeItemId treeItem)
{
	
	ID_TREE_ITEM_MAP::iterator it;
	ID_TREE_ITEM_MAP::iterator itEnd;

	wxTreeItemId TreeItemFind;

	itEnd = m_Id_TreeItem_Map.end();
	for(it = m_Id_TreeItem_Map.begin(); it != itEnd; it++)
	{
		TreeItemFind = it->second;
		if(TreeItemFind == treeItem)
		{
			return it->first;
		}
	}

	return 0xffffffff;
}

void CMonsterEditDlg::ShowMonsterTypeInfo(int iSelIndex)
{
	if(NULL == m_pMonsterListCtrl)
	{
		return;
	}
	
	wxListItem itemInfo;
	int iSize = m_pMonsterListCtrl->GetItemCount();
    int i;
	for(i = 0; i < iSize; i++)
	{
		itemInfo.m_itemId = i;
		itemInfo.m_col = 0;
		itemInfo.m_mask = wxLIST_MASK_TEXT;
		
		m_pMonsterListCtrl->GetItem( itemInfo);
		if(atoi(itemInfo.GetText()) == iSelIndex)
		{
			break;
		}
	}

	
	if(i >= iSize)
	{
		return;
	}

	// Ϊʲô���ܼ���仰�� ������濪�����߰�̫���ˣ�
	//memset(&itemInfo, 0, sizeof(itemInfo));
	itemInfo.m_itemId = i;
    itemInfo.m_col = 1;
    itemInfo.m_mask = wxLIST_MASK_TEXT;

	

	// �õ�����
	m_pMonsterListCtrl->GetItem(itemInfo);
	if(m_pMonsterNameText)
	{
		char bufInfo[128];
		memset(bufInfo, 0, sizeof(bufInfo));
		sprintf(bufInfo, "%s--%d", itemInfo.GetText(), iSelIndex);
		
	
		m_pMonsterNameText->SetLabel(bufInfo);
	}

	// �õ��ȼ�
	itemInfo.SetColumn(2);
	m_pMonsterListCtrl->GetItem(itemInfo);
	if(m_pMonsterLevelText)
	{
		if( m_pMonsterLevelText->GetLabel() == "-1" || m_pMonsterLevelText->GetLabel() == "" )
			m_pMonsterLevelText->SetLabel(itemInfo.GetText());
	}

	// �õ�����
	itemInfo.SetColumn(3);
	m_pMonsterListCtrl->GetItem(itemInfo);
	if(m_pMonsterExpText)
	{
		m_pMonsterExpText->SetLabel(itemInfo.GetText());
	}

	// �õ��ƶ��ٶ�
	itemInfo.SetColumn(4);
	m_pMonsterListCtrl->GetItem(itemInfo);
	if(m_pMonsterMoveSpeedText)
	{
		m_pMonsterMoveSpeedText->SetLabel(itemInfo.GetText());
	}//

	//// �õ�����ʱ��
	//itemInfo.SetColumn(5);
	//m_pMonsterListCtrl->GetItem(itemInfo);
	//if(m_pMonsterfreshTimeText)
	//{
	//	m_pMonsterfreshTimeText->SetLabel(itemInfo.GetText());
	//}//

	// �õ����Χ
	itemInfo.SetColumn(6);
	m_pMonsterListCtrl->GetItem(itemInfo);
	if(m_pMonsterRadiusText)
	{
		m_pMonsterRadiusText->SetLabel(itemInfo.GetText());
	}//

	// �õ���ӪID
	itemInfo.SetColumn(7);
	m_pMonsterListCtrl->GetItem(itemInfo);
	if(m_pMonsterCampIdText)
	{
		m_pMonsterCampIdText->SetLabel(itemInfo.GetText());
	}//
}

	// �޸�ѡ��Ĺ���Ĳ���.
void CMonsterEditDlg::ModifyMonsterInfo()
{
	if(m_MonsterInfoTable.count(m_ulCurEditMonsterInstance))
	{

		CMonsterInstanceInfo& MonsterInfo = m_MonsterInfoTable[m_ulCurEditMonsterInstance];
		
		MonsterInfo.m_strInstanceName	= GetInstanceName();
		MonsterInfo.m_strTitleName	    = GetTitleName();
		MonsterInfo.m_fMonstDir         = GetDir();
		MonsterInfo.m_EvenId			= GetEvenId();
		MonsterInfo.m_iRefreshTime      = GetRefreshTime();
		MonsterInfo.m_iGroupId			= GetGroupId();
		MonsterInfo.m_iTeamId			= GetTeamId();
		MonsterInfo.m_iBaseAIId			= GetBaseAIId();
		MonsterInfo.m_iAdvanceAIId		= GetAdvanceAIId();
		MonsterInfo.m_iLineid           = GetPatrolId();
		MonsterInfo.m_iReputationID		= GetReputationID();
		MonsterInfo.m_iLevel			= GetLevel();
		MonsterInfo.m_iType				= GetMonsterType();
		MonsterInfo.m_iCampId			= GetCamp();

		for(int i = 0; i < SHOP_COUNT; i++)
		{
			MonsterInfo.m_iShopArray[i] = GetShopIndex(i);
		}

	}

}




// ���ٹ���ķ���
void CMonsterEditDlg::DecMonsterDir()
{
	if(m_pMonsterEditAction)
	{
		CMonsterInfo* pMonsterInfo = m_pMonsterEditAction->GetMonster(m_pMonsterEditAction->m_dwCurSelMonsterInstanceId);
		
		if(NULL == pMonsterInfo)
		{
			return;
		}

		pMonsterInfo->m_fMonstDir -= DIR_STEP;
		m_pMonsterEditAction->ChangeMonsterDir(m_pMonsterEditAction->m_dwCurSelMonsterInstanceId, pMonsterInfo->m_fMonstDir);
		
		
		char buf[1024];
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "%.4f", pMonsterInfo->m_fMonstDir);
		if(m_pMonsterDirText)
		{
			m_pMonsterDirText->SetLabel(buf);
		}

		m_pMonsterEditAction->SetModifyed();

	
		if(m_MonsterInfoTable.count(m_ulCurEditMonsterInstance))
		{
			CMonsterInstanceInfo& MonsterInfo = m_MonsterInfoTable[m_ulCurEditMonsterInstance];
			MonsterInfo.m_fMonstDir         = GetDir();
		}

	}

	
}


// ���ӹ��﷽��
void CMonsterEditDlg::IncMonsterDir()
{

	if(m_pMonsterEditAction)
	{
		CMonsterInfo* pMonsterInfo = m_pMonsterEditAction->GetMonster(m_pMonsterEditAction->m_dwCurSelMonsterInstanceId);
		
		if(NULL == pMonsterInfo)
		{
			return;
		}

		pMonsterInfo->m_fMonstDir += DIR_STEP;
		m_pMonsterEditAction->ChangeMonsterDir(m_pMonsterEditAction->m_dwCurSelMonsterInstanceId, pMonsterInfo->m_fMonstDir);


		char buf[1024];
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "%.4f", pMonsterInfo->m_fMonstDir);
		if(m_pMonsterDirText)
		{
			m_pMonsterDirText->SetLabel(buf);
		}

		m_pMonsterEditAction->SetModifyed();


		if(m_MonsterInfoTable.count(m_ulCurEditMonsterInstance))
		{
			CMonsterInstanceInfo& MonsterInfo = m_MonsterInfoTable[m_ulCurEditMonsterInstance];
			MonsterInfo.m_fMonstDir         = GetDir();
		}

	}



}

// ��ս���.
void CMonsterEditDlg::ClearUI()
{
	m_MonsterInfoTable.clear();
	m_iCurSelListCtrlIndex = -1;
	// ���¹����Ѿ������������Ϳؼ�.
	InitAddedMonsterTree();

	// ����ı���.
	
	if(m_pMonsterNameText)
	{
		m_pMonsterNameText->SetLabel("");
	}

	if(m_pMonsterLevelText)
	{
		m_pMonsterLevelText->SetLabel("");
	}

	if(m_pMonsterExpText)
	{
		m_pMonsterExpText->SetLabel("");
	}

	if(m_pMonsterMoveSpeedText )
	{
		m_pMonsterMoveSpeedText ->SetLabel("");
	}

	if(m_pMonsterfreshTimeText )
	{
		m_pMonsterfreshTimeText ->SetLabel("");
	}

	if(m_pMonsterRadiusText)
	{
		m_pMonsterRadiusText->SetLabel("");
	}

	if(m_pMonsterCampIdText)
	{
		m_pMonsterCampIdText->SetLabel("");
	}

	if(m_pMonsterBaseAIText)
	{
		m_pMonsterBaseAIText->SetLabel("");
	}

	if(m_pMonsterAdvanceAIText)
	{
		m_pMonsterAdvanceAIText->SetLabel("");
	}

	if(m_pMonsterEventText)
	{
		m_pMonsterEventText->SetLabel("");
	}

	if(m_pMonsterGroupIdText)
	{
		m_pMonsterGroupIdText->SetLabel("");
	}

	if(m_pMonsterTeamIdText)
	{
		m_pMonsterTeamIdText->SetLabel("");
	}

	if(m_pMonsterDirText)
	{
		m_pMonsterDirText->SetLabel("");
	}

	ClearCounter();
	
}


// ���action����
void CMonsterEditDlg::ClearActionData()
{
	if(m_pMonsterEditAction)
	{
		m_pMonsterEditAction->ReleaseAllMonster();
	}

}

	// ��ռ�����
void CMonsterEditDlg::ClearCounter()
{
	m_dwAddedMonsterCount = 0;
}


// �õ�guid
DWORD CMonsterEditDlg::GetGuid()
{

	u_long dwId = 0;
	time_t  LocalTime;
	
	// �õ�ϵͳʱ��
	time(&LocalTime);
	
	// �õ�������ip��ַ
	u_long ulIp = htonl(INADDR_ANY);

	LocalTime = 0x00ffffff & LocalTime;

	dwId = ulIp<< 24;

	dwId = dwId | LocalTime;

	return dwId; 

}

// �õ�����ai
int CMonsterEditDlg::GetBaseAIId()
{
	if(m_pChoiceBaseAiId)
	{
		return m_pChoiceBaseAiId->GetSelection();
	}
	else
	{
		return -1;
	}
}


// �õ��߼�ai
int CMonsterEditDlg::GetAdvanceAIId()
{
	if(m_pChoiceAIFile)
	{
		return m_pChoiceAIFile->GetSelection();
	}
	else
	{
		return -1;
	}
}

int CMonsterEditDlg::GetMonsterType()
{
	if(m_pMonsterType)
	{
		return m_pMonsterType->GetSelection();
	}
	else
	{
		return 0;
	}
}
int CMonsterEditDlg::GetCamp()
{
	if(m_pMonsterCampIdText)
	{
		return atoi( m_pMonsterCampIdText->GetLabel().c_str() );
	}
	else
	{
		return 20;
	}
}

// �õ��¼�id
int CMonsterEditDlg::GetEvenId()
{
	if(m_pChoiceEvenFile)
	{
		int iIndex = m_pChoiceEvenFile->GetSelection();
		std::string strName = m_pChoiceEvenFile->GetString(iIndex);

		int iPos = strName.find("=");

		if(iPos <= 0)
		{
			return -1;
		}

		std::string strNum = strName.substr(0, iPos);
		return atoi(strNum.c_str());


	}
	else
	{
		return -1;
	}
}

// �õ���id
int CMonsterEditDlg::GetGroupId()
{
	if(m_pMonsterGroupIdText)
	{
		if("" == m_pMonsterGroupIdText->GetValue())
		{
			return -1;
		}

		return atoi(m_pMonsterGroupIdText->GetValue().c_str());
	}
	else
	{
		return -1;
	}
	
}

	
// �õ�����id
int CMonsterEditDlg::GetTeamId()
{
	if(m_pMonsterTeamIdText)
	{
		if("" == m_pMonsterTeamIdText->GetValue())
		{
			return -1;
		}


		return atoi(m_pMonsterTeamIdText->GetValue().c_str());
	}
	else
	{
		return -1;
	}
}

// �õ�Ѳ��·��
int CMonsterEditDlg::GetPatrolId()
{
	if(m_pMonsterPatrol)
	{
		if("" == m_pMonsterPatrol->GetValue())
		{
			return -1;
		}


		return atoi(m_pMonsterPatrol->GetValue().c_str());
	}
	else
	{
		return -1;
	}
}

int CMonsterEditDlg::GetLevel()
{

	if(m_pMonsterLevelText)
	{
		return atoi(m_pMonsterLevelText->GetValue());
	}

	return -1;
}
	
// �õ�ʵ������
std::string CMonsterEditDlg::GetInstanceName()
{
	std::string strName = "";

	if(m_pMonsterInstanceName)
	{
		strName = m_pMonsterInstanceName->GetValue().c_str();
	}

	return strName;
}

// �õ��ƺ�����
std::string CMonsterEditDlg::GetTitleName()
{
	std::string strName = "";

	if(m_pMonsterTitleName)
	{
		strName = m_pMonsterTitleName->GetValue().c_str();
	}

	return strName;
}

// �õ�����
float CMonsterEditDlg::GetDir()
{

	if(m_pMonsterDirText)
	{
		return atof(m_pMonsterDirText->GetValue());
	}

	return 0;
}


// �õ�ˢ��ʱ��
int CMonsterEditDlg::GetRefreshTime()
{
	if(m_pMonsterfreshTimeText)
	{
		if("" == m_pMonsterfreshTimeText->GetValue())
		{
			return -1;
		}

		return atoi(m_pMonsterfreshTimeText->GetValue().c_str());
	}

	return -1;
}

// �õ�λ��
Ogre::Vector3 CMonsterEditDlg::GetMonsterPos()
{
	Ogre::Vector3 pos(0, 0, 0);

	if(m_pMonsterEditAction)
	{
		return m_pMonsterEditAction->GetMonsterPos(m_ulCurEditMonsterInstance);
	}

	return  pos;
}



// �������ȡ�ļ�
unsigned long CMonsterEditDlg::ReadMonsterFromFile(std::string& strFileName)
{
	// �������
	m_MonsterInfoTable.clear();

	char bufMonsterIndex[64];
	int iMonsterCount = ::GetPrivateProfileInt("info", "monstercount", 0, strFileName.c_str());
	
	char bufX[64];
	char bufZ[64];

	String  MonsterTypename;
	Ogre::Vector3 pos;

	char bufInstanceName[128];
	CMonsterInstanceInfo MonsterInfo;
	
	for(int i = 0; i < iMonsterCount; i++)
	{

		memset(&MonsterInfo, 0, sizeof(MonsterInfo));
		memset(bufMonsterIndex, 0, sizeof(bufMonsterIndex));
		memset(bufX, 0, sizeof(bufX));
		memset(bufZ, 0, sizeof(bufZ));
		sprintf(bufMonsterIndex, "monster%d", i);

		// ��ȡ��������
		MonsterInfo.m_dwObjectId = ::GetPrivateProfileInt(bufMonsterIndex, "type", -1, strFileName.c_str());

		if(0 == m_IdNameMap.count(MonsterInfo.m_dwObjectId))
		{
			char bufMessage[1024];
			memset(bufMessage, 0, sizeof(bufMessage));
			sprintf(bufMessage, wxT("�ļ��з����˳����༭���޷�ʶ��Ĺ�������-- %d!"), MonsterInfo.m_dwObjectId);
			wxLogMessage(bufMessage);
			continue;
		}
		
		// ��ȡ����guid
		MonsterInfo.m_dwMonsterGUID = ::GetPrivateProfileInt(bufMonsterIndex, "guid", 0xffffffff, strFileName.c_str());

		// ��ȡ����ʵ������
		memset(bufInstanceName, 0, sizeof(bufInstanceName));
		::GetPrivateProfileString(bufMonsterIndex, "name",  "none", bufInstanceName, sizeof(bufInstanceName), strFileName.c_str());
		MonsterInfo.m_strInstanceName = bufInstanceName;

		// �ƺ�
		memset(bufInstanceName, 0, sizeof(bufInstanceName));
		::GetPrivateProfileString(bufMonsterIndex, "title",  "none", bufInstanceName, sizeof(bufInstanceName), strFileName.c_str());
		MonsterInfo.m_strTitleName = bufInstanceName;

		// ��ȡλ��
		::GetPrivateProfileString(bufMonsterIndex, "pos_x",  "none", bufX, sizeof(bufX), strFileName.c_str());
		::GetPrivateProfileString(bufMonsterIndex, "pos_z",  "none", bufZ, sizeof(bufZ),strFileName.c_str());
		MonsterInfo.m_position.x = atof(bufX);
		MonsterInfo.m_position.z = atof(bufZ);

		// ��ȡ����
		int iMonsterDir = ::GetPrivateProfileInt(bufMonsterIndex, "dir",  -1, strFileName.c_str());
		iMonsterDir += 9;
		iMonsterDir = iMonsterDir % DIR_STEP_TRANS;
		MonsterInfo.m_fMonstDir = (iMonsterDir*1.0 / DIR_STEP_TRANS) * _2PI;

		// ��ȡ�¼�id
		MonsterInfo.m_EvenId  = ::GetPrivateProfileInt(bufMonsterIndex, "script_id", -1, strFileName.c_str());

		// ��ȡˢ��ʱ��
		MonsterInfo.m_iRefreshTime = ::GetPrivateProfileInt(bufMonsterIndex, "respawn_time", -1, strFileName.c_str());

		// ��ȡgroupid
		 MonsterInfo.m_iGroupId = ::GetPrivateProfileInt(bufMonsterIndex, "group_id", -1, strFileName.c_str());

		// ��ȡteamid
		MonsterInfo.m_iTeamId = ::GetPrivateProfileInt(bufMonsterIndex, "team_id", -1, strFileName.c_str());

		// �������basseAiId
		MonsterInfo.m_iBaseAIId = ::GetPrivateProfileInt(bufMonsterIndex, "base_ai",  -1, strFileName.c_str());

		// ������ĸ߼�ai�ļ�����
		MonsterInfo.m_iAdvanceAIId = ::GetPrivateProfileInt(bufMonsterIndex, "ai_file",  -1, strFileName.c_str());

		// ��ȡ�̵���Ϣ
		MonsterInfo.m_iShopArray[0] = ::GetPrivateProfileInt(bufMonsterIndex, "shop0",  -1, strFileName.c_str());
		MonsterInfo.m_iShopArray[1] = ::GetPrivateProfileInt(bufMonsterIndex, "shop1",  -1, strFileName.c_str());
		MonsterInfo.m_iShopArray[2] = ::GetPrivateProfileInt(bufMonsterIndex, "shop2",  -1, strFileName.c_str());
		MonsterInfo.m_iShopArray[3] = ::GetPrivateProfileInt(bufMonsterIndex, "shop3",  -1, strFileName.c_str());
	
		// ��ȡѲ��·��id
		MonsterInfo.m_iLineid = ::GetPrivateProfileInt(bufMonsterIndex, "patrol_id",  -1, strFileName.c_str());

		// ��ȡ��������
		MonsterInfo.m_iReputationID = ::GetPrivateProfileInt(bufMonsterIndex, "ReputationID",  -1, strFileName.c_str());

		// �ȼ�
		MonsterInfo.m_iLevel = ::GetPrivateProfileInt(bufMonsterIndex, "level",  -1, strFileName.c_str());

		MonsterInfo.m_iType = ::GetPrivateProfileInt(bufMonsterIndex, "npc",  1, strFileName.c_str());

		MonsterInfo.m_iCampId = ::GetPrivateProfileInt(bufMonsterIndex, "camp",  20, strFileName.c_str());

		m_MonsterInfoTable[-1] = MonsterInfo;

		// ���һ���µĹ���.
		if(m_pMonsterEditAction)
		{
			m_pMonsterEditAction->AddNewMonster(MonsterInfo.m_position, MonsterInfo.m_dwObjectId, true);
		}
	}

	if(m_pMonsterEditAction)
	{
		m_pMonsterEditAction->HideScene(m_pMonsterEditAction->m_bIsShow);
	}
	return 0;
}


	
// ������洢���ļ�
unsigned long CMonsterEditDlg::SaveMonsterToFile(std::string& strFileName)
{
	char bufWrite[128];
	char bufMonsterIndex[128];
	int iMonsterType  = 0;
	int  iMonsterDir;
	char bufX[64];
	char bufZ[64];

	float fx = 0;
	float fy = 0;
	float fz = 0;

	// ɾ���ɵ��ļ���
	::DeleteFile(strFileName.c_str());

    String  MonsterTypename;
	Ogre::Vector3 pos;

	int iMonsterCount = 0;
	iMonsterCount = m_MonsterInfoTable.size();

	memset(bufWrite, 0, sizeof(bufWrite));
	sprintf(bufWrite, "%d\r\n", iMonsterCount);

	// ��д����ĸ���
	WritePrivateProfileString("info", "monstercount", bufWrite, strFileName.c_str());
	
	INSTANCE_MONSTERINFO_MAP::iterator it;
	INSTANCE_MONSTERINFO_MAP::iterator itEnd;
	
	itEnd = m_MonsterInfoTable.end();
	int i = 0;
	
	for(it = m_MonsterInfoTable.begin(); it != itEnd; it++)
	{
		CMonsterInstanceInfo& MonsterInfo = it->second;

		memset(bufMonsterIndex, 0, sizeof(bufMonsterIndex));
		sprintf(bufMonsterIndex, "monster%d", i);

		// д�����guid
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d", MonsterInfo.m_dwMonsterGUID);
		::WritePrivateProfileString(bufMonsterIndex, "guid",  bufWrite, strFileName.c_str());
		
		// д���������.
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d", MonsterInfo.m_dwObjectId);
		::WritePrivateProfileString(bufMonsterIndex, "type", bufWrite, strFileName.c_str());

			
		// д��������
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%s", MonsterInfo.m_strInstanceName.c_str());
		::WritePrivateProfileString(bufMonsterIndex, "name", bufWrite, strFileName.c_str());

		// �ƺ�
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%s", MonsterInfo.m_strTitleName.c_str());
		::WritePrivateProfileString(bufMonsterIndex, "title", bufWrite, strFileName.c_str());


		// д�����λ��
		memset(bufX, 0, sizeof(bufX));

		// ���г���������߼�����ת��
        fx = MonsterInfo.m_position.x;
		fy = MonsterInfo.m_position.y;
		fz = MonsterInfo.m_position.z;

		sprintf(bufX, "%.4f", fx);
		::WritePrivateProfileString(bufMonsterIndex, "pos_x",  bufX, strFileName.c_str());

		memset(bufZ, 0, sizeof(bufZ));
		sprintf(bufZ, "%.4f", fz);
		::WritePrivateProfileString(bufMonsterIndex, "pos_z",  bufZ, strFileName.c_str());

		// д����ķ���.
		iMonsterDir = (MonsterInfo.m_fMonstDir / _2PI) * DIR_STEP_TRANS;
		iMonsterDir = iMonsterDir + 27;
		iMonsterDir = iMonsterDir % DIR_STEP_TRANS;
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d", iMonsterDir);
		::WritePrivateProfileString(bufMonsterIndex, "dir",  bufWrite, strFileName.c_str());


		// д������¼��ļ�����
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d", MonsterInfo.m_EvenId);
		::WritePrivateProfileString(bufMonsterIndex, "script_id",  bufWrite, strFileName.c_str());


		// д����ˢ��ʱ��
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d", MonsterInfo.m_iRefreshTime);
		::WritePrivateProfileString(bufMonsterIndex, "respawn_time",  bufWrite, strFileName.c_str());


		// д�����groupid
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d", MonsterInfo.m_iGroupId);
		::WritePrivateProfileString(bufMonsterIndex, "group_id",  bufWrite, strFileName.c_str());

		// д�����teamid
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d", MonsterInfo.m_iTeamId);
		::WritePrivateProfileString(bufMonsterIndex, "team_id",  bufWrite, strFileName.c_str());

		// д�����basseAiId
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d", MonsterInfo.m_iBaseAIId);
		::WritePrivateProfileString(bufMonsterIndex, "base_ai",  bufWrite, strFileName.c_str());

		// д����ĸ߼�ai�ļ�����
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d", MonsterInfo.m_iAdvanceAIId);
		::WritePrivateProfileString(bufMonsterIndex, "ai_file",  bufWrite, strFileName.c_str());

		// дѲ��·��id
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d", MonsterInfo.m_iLineid);
		::WritePrivateProfileString(bufMonsterIndex, "patrol_id",  bufWrite, strFileName.c_str());

		// д�̵���Ϣ

		//1
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d", MonsterInfo.m_iShopArray[0]);
		::WritePrivateProfileString(bufMonsterIndex, "shop0",  bufWrite, strFileName.c_str());

		//2
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d", MonsterInfo.m_iShopArray[1]);
		::WritePrivateProfileString(bufMonsterIndex, "shop1",  bufWrite, strFileName.c_str());

		//3
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d", MonsterInfo.m_iShopArray[2]);
		::WritePrivateProfileString(bufMonsterIndex, "shop2",  bufWrite, strFileName.c_str());

		//4
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d", MonsterInfo.m_iShopArray[3]);
		::WritePrivateProfileString(bufMonsterIndex, "shop3",  bufWrite, strFileName.c_str());

		//����
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d", MonsterInfo.m_iReputationID);
		::WritePrivateProfileString(bufMonsterIndex, "ReputationID",  bufWrite, strFileName.c_str());
		

		// д�����level
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d", MonsterInfo.m_iLevel);
		::WritePrivateProfileString(bufMonsterIndex, "level",  bufWrite, strFileName.c_str());

		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d", MonsterInfo.m_iType);
		::WritePrivateProfileString(bufMonsterIndex, "npc",  bufWrite, strFileName.c_str());
       
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d\r\n", MonsterInfo.m_iCampId);
		::WritePrivateProfileString(bufMonsterIndex, "camp",  bufWrite, strFileName.c_str());

		// ������
		i++;
	
	}

	return 0;
}


// �õ��̵�����
int CMonsterEditDlg::GetShopIndex(int iIndex)
{
	
	if(-1 == iIndex)
	{
		return - 1;
	}

	int iSelectIndex = -1;
	wxChoice*  pShop = NULL;
	pShop = m_pShop[iIndex];
		

	if(pShop)
	{
		iSelectIndex   = pShop->GetSelection();
		if(-1 == iSelectIndex)
		{
			return -1;
		}


		// ����ѡ��������һ��Ԫ��.
		if(iSelectIndex == pShop->GetCount() - 1)
		{
			return -1;
		}

		int iShopIndex = (int)pShop->GetClientData(iSelectIndex);
		return iShopIndex;
	}
	else
	{
		return -1;
	}

	return -1;
}

int CMonsterEditDlg::GetReputationID()
{
	if(m_pMonsterReputationID)
	{
		if("" == m_pMonsterReputationID->GetValue())
		{
			return -1;
		}

		return atoi(m_pMonsterReputationID->GetValue().c_str());
	}

	return -1;
}

void CMonsterEditDlg::ShowMonsterTypeInfoByIndex(int iSelIndex)
{
	if(NULL == m_pMonsterListCtrl)
	{
		return;
	}
	
	wxListItem itemInfo;
	int iSize = m_pMonsterListCtrl->GetItemCount();
		
	if(iSelIndex >= iSize)
	{
		return;
	}

	// Ϊʲô���ܼ���仰�� ������濪�����߰�̫���ˣ�
	//memset(&itemInfo, 0, sizeof(itemInfo));
	itemInfo.m_itemId = iSelIndex;
    itemInfo.m_col = 1;
    itemInfo.m_mask = wxLIST_MASK_TEXT;

	

	// �õ�����
	m_pMonsterListCtrl->GetItem(itemInfo);
	if(m_pMonsterNameText)
	{
		m_pMonsterNameText->SetLabel(itemInfo.GetText());
	}

	// �õ��ȼ�
	itemInfo.SetColumn(2);
	m_pMonsterListCtrl->GetItem(itemInfo);
	if(m_pMonsterLevelText)
	{
		//if( m_pMonsterLevelText->GetLabel() == "-1" || m_pMonsterLevelText->GetLabel() == "" )
			m_pMonsterLevelText->SetLabel(itemInfo.GetText());
	}

	// �õ�����
	itemInfo.SetColumn(3);
	m_pMonsterListCtrl->GetItem(itemInfo);
	if(m_pMonsterExpText)
	{
		m_pMonsterExpText->SetLabel(itemInfo.GetText());
	}

	// �õ��ƶ��ٶ�
	itemInfo.SetColumn(4);
	m_pMonsterListCtrl->GetItem(itemInfo);
	if(m_pMonsterMoveSpeedText)
	{
		m_pMonsterMoveSpeedText->SetLabel(itemInfo.GetText());
	}//

	//// �õ�����ʱ��
	//itemInfo.SetColumn(5);
	//m_pMonsterListCtrl->GetItem(itemInfo);
	//if(m_pMonsterfreshTimeText)
	//{
	//	m_pMonsterfreshTimeText->SetLabel(itemInfo.GetText());
	//}//

	// �õ����Χ
	itemInfo.SetColumn(6);
	m_pMonsterListCtrl->GetItem(itemInfo);
	if(m_pMonsterRadiusText)
	{
		m_pMonsterRadiusText->SetLabel(itemInfo.GetText());
	}//

	// �õ���ӪID
	itemInfo.SetColumn(7);
	m_pMonsterListCtrl->GetItem(itemInfo);
	if(m_pMonsterCampIdText)
	{
		m_pMonsterCampIdText->SetLabel(itemInfo.GetText());
	}//
}


// ����������Ϣ2006-4-5
void CMonsterEditDlg::ReSetDataInfo()
{
	//
	// ���Ի�����
	//
	if(m_pChoiceEvenFile->GetCount())
	{
		m_pChoiceEvenFile->Select(0);
	}

	// ����ai
	if(m_pChoiceBaseAiId)
	{
		m_pChoiceBaseAiId->Select(0);
	}

	// �߼�ai
	if(m_pChoiceAIFile)
	{
		m_pChoiceAIFile->Select(0);
	}

	if(m_pMonsterType)
	{
		m_pMonsterType->Select(0);
	}

	m_pMonsterTeamIdText->SetLabel("-1");		// �������id
	m_pMonsterDirText->SetLabel("0.00");		// ���﷽��
	m_pMonsterInstanceName->SetLabel("");		// ����ʵ������
	m_pMonsterTitleName->SetLabel("");			// ����ƺ�����
	m_pMonsterPatrol->SetLabel("-1");			// ����Ѳ��·��
	m_pMonsterGroupIdText->SetLabel("-1");		// ��id

	for(int i = 0; i < SHOP_COUNT; i++)
	{
		m_pShop[i]->Select(m_pShop[i]->GetCount()-1);
	}
}








