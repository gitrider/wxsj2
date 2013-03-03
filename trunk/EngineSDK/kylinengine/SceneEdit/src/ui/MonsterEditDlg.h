/////////////////////////////////////////////////////////////////////////////
// Name:        MonsterEditDlg.h
// Author:      XX
// Created:     XX/XX/XX
// Copyright:   XX
/////////////////////////////////////////////////////////////////////////////

#ifndef __MonsterEditDlg_H__
#define __MonsterEditDlg_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "MonsterEditDlg.cpp"
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "./res/MonsterEdit_wdr.h"

// WDR: class declarations


#include <wx/panel.h>
#include <wx/image.h>

#include <OgreString.h>
#include <OgreImage.h>
#include "PaintAction.h"
#include "WXSceneListener.h"

#include <map>

namespace Fairy 
{
    class SceneManipulator;
	class CMonsterEditaction;		// ���������������򳡾��༭����
}

typedef std::map<int, std::string>	ID_NAME_DLG_MAP;//
typedef std::map<int, wxTreeItemId> ID_TREE_ITEM_MAP;
typedef std::map<int, int> ID_LOOK_ID_MAP;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ������Ϣ
//
#define SHOP_COUNT	4
class CMonsterInstanceInfo
{


public:
	CMonsterInstanceInfo()
	{
		for(int i = 0; i < SHOP_COUNT; i++)
		{
			m_iShopArray[i] = -1;
		}
	}

	~CMonsterInstanceInfo()
	{

	}

public:
	unsigned long	m_dwInstanceId;			// ÿһ��ʵ����id
	
	int				m_iRefreshTime;			// ˢ��ʱ�䡣
	int				m_ActiveType;			// ����͡�

	float			m_fAreaRadius;			// �������������򣬼�¼���Χ�İ뾶 
	int				m_iLineid;
	
	// �����ɵ��ļ�����д��������
	unsigned long	m_dwObjectId;			// ����id�� ͨ���������ҵ������ļ�, ���ǹ�������͡�
    int				m_iBaseAIId;			// ����AI��š�
	int             m_iAdvanceAIId;			// �߼�aiId
	int             m_EvenId;				// �¼�id
	std::string     m_strInstanceName;		// ����ʵ������.
	std::string     m_strTitleName;			// �ƺ�����

	std::string		m_strAdvanceAIFile;		// ����ai�ű��ļ�
	std::string		m_strEvent;				// �¼��ű��ļ�.
	unsigned long   m_dwMonsterGUID;		// �����guid
	Ogre::Vector3	m_position;				// �����λ�á�
	int				m_iGroupId;				// ��id
	int				m_iTeamId;				// �ӵ�id
	float			m_fMonstDir;			// ����ķ���
	int				m_iReputationID;		// ���������

	int				m_iLevel;				//����ĵȼ�
	int				m_iType;				//NPC���ǹ���
	int				m_iCampId;				//��Ӫ

	int             m_iShopArray[SHOP_COUNT];



public:

	void DeleteDObject()
	{

	}

};

typedef std::map<unsigned long, CMonsterInstanceInfo> INSTANCE_MONSTERINFO_MAP;






class CMonsterEditDlg : public wxPanel, Fairy::SceneListener
{


public:

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// Ҫ�༭���߼�����
	//

		// ����ʵ����
		INSTANCE_MONSTERINFO_MAP		m_MonsterInfoTable;

		// ��ǰ�༭�Ĺ���ʵ��id
		unsigned long		m_ulCurEditMonsterInstance;

		// id ������id��Ӧ��ӳ���.
		ID_NAME_DLG_MAP  m_IdNameMap;
		
		// �õ�����id�� �����ֵ�ӳ���.
		void LoadMonsterIdNameTable();

		// �Ѿ���ӵ����й���ĸ���.
		unsigned long m_dwAddedMonsterCount;

		// �Ƿ������޸�.
		bool m_bIsModify;


		// ���һ������
		void AddNewMonster(unsigned long ulMonsterId, unsigned long ulInstanceId);
		
		// ɾ��һ������.
		void DelMonster(unsigned long ulMonsterId, unsigned long ulInstanceId);

		// ɾ������.
		void DelMonster();

		// �޸�ѡ��Ĺ���Ĳ���.
		void ModifySelectData();

		// �޸�ѡ��Ĺ���Ĳ���.
		void ModifyMultiData();

		// ѡ��һ������.
		void SelMonster(unsigned long ulMonsterId, unsigned long ulInstanceId);

		// �޸�ѡ��Ĺ���Ĳ���.
		void ModifyMonsterInfo();

		// ���ٹ���ķ���
		void DecMonsterDir();

		// ���ӹ��﷽��
		void IncMonsterDir();


	//
	// Ҫ�༭���߼�����
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////

		



	//////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// ��Ҫʹ�õĿؼ�����
	//

	// �����б�ؼ���
	wxListCtrl*	m_pMonsterListCtrl;

	// �Ѿ���ӵĹ����б�
	wxTreeCtrl* m_pAddedMonsterTreeCtrl;


	//////////////////////////
	//
	
	// �޸İ�ť.
	wxButton*  m_pModifyBn;

	// �޸Ķ�����ﰴť.
	wxButton*  m_pMultiModifyBn;

	//������Χ��ť.
	wxRadioButton* pArea;
	wxRadioButton* pLine;

	// ��뾶�ı���
	wxTextCtrl* m_pAreaText;

	// ��뾶�ı���
	wxTextCtrl* m_pRefreshText;

	//·�߱༭���ء�
	wxCheckBox *m_pLineCheck;

	// ����ai
	wxChoice*   m_pChoiceBaseAiId;

	// �߼�ai
	wxChoice*   m_pChoiceAIFile;

	// �¼��ű�
	wxComboBox*   m_pChoiceEvenFile;


	// �̵�1
	wxChoice*   m_pShop[SHOP_COUNT];

	// ��������
	wxChoice*   m_pMonsterType;
	// 
	//////////////////////////


	// �޸Ĺ�����Ϣ��ť.
	wxButton*  m_pModifyMonsterBn;
	wxButton*  m_pDirDecBn;
	wxButton*  m_pDirIncBn;


	// ��뾶�ı���
	wxTextCtrl* m_pMonsterNameText;				// ��������
	wxTextCtrl* m_pMonsterLevelText;			// ����ȼ�
    wxTextCtrl* m_pMonsterExpText;				// ���ﾭ��
	wxTextCtrl* m_pMonsterMoveSpeedText;		// �����ƶ��ٶ�
	wxTextCtrl* m_pMonsterfreshTimeText;		// ����ˢ��ʱ��
	wxTextCtrl* m_pMonsterRadiusText;			// �����뾶
	wxTextCtrl* m_pMonsterCampIdText;			// ������Ӫid
	wxTextCtrl* m_pMonsterGUIDText;			    // ����guid
	// �������û�ֻ����ѡ��

	wxTextCtrl* m_pMonsterBaseAIText;			// �������AI ID
	wxTextCtrl* m_pMonsterAdvanceAIText;		// ����߼�AI�ļ���
	wxTextCtrl* m_pMonsterEventText;			// �����¼��ļ�
	wxTextCtrl* m_pMonsterGroupIdText;			// Ⱥid�� �������Ż�ʹ��
	wxTextCtrl* m_pMonsterTeamIdText;			// �������id
	wxTextCtrl* m_pMonsterDirText;				// ���﷽��
	wxTextCtrl* m_pMonsterInstanceName;			// ����ʵ������
	wxTextCtrl* m_pMonsterTitleName;			// ����ƺ�����
	wxTextCtrl* m_pMonsterPatrol;				// ����Ѳ��·��
	wxTextCtrl* m_pMonsterReputationID;			// ��������id





	// �Ѿ���ӹ������ĸ��ڵ�.
	wxTreeItemId	m_AddedMonsterRoot;

	// ����id����ӵ����е�item��Ӧ���б�
	ID_TREE_ITEM_MAP	m_Id_TreeItem_Map;

	// �Ѿ���ӵĹ����
	ID_TREE_ITEM_MAP	m_Id_AddedMonsterMap;


	// ��ǰѡ��Ĺ����б�����
	int m_iCurSelListCtrlIndex;
	
	////////////////////////////////////////////////////////////////////////////////////////
	//
	// ��ȡ�ļ������
	//

	// �õ����еĿؼ�.
	void GetAllControl();

	// ��ʼ�������б�ؼ���ͷ.
	void InitMonsterListCtrlTable();

	// ��ʼ�������б���.
	void InitAddedMonsterTree();

	// ��ȡ����ai��
	void GetBaseAITable();

	// ��ȡ�߼�ai
	void GetAIFile();

	// �õ��¼��ű�
	void GetEventFile();

	// ��ȡ�̵���Ϣ
	void GetShopInfo();


	// ����������Ϣ2006-4-5
	void ReSetDataInfo();
	//
	// ��ȡ�ļ������
	//
	////////////////////////////////////////////////////////////////////////////////////////

	void InitMonsterType();



	// �Ƿ��ֹ����༭
	void EnableAreaEdit(bool bEnable = true);

	// ��ʾѡ�еĹ������Ϣ.
	void ShowMonsterInfo( unsigned long ulInstanceId);

	// ��ʾѡ�й������͵���Ϣ.
	void ShowMonsterTypeInfo(int iSelIndex);

	// ͨ��������ʾ�������Ϣ
	void ShowMonsterTypeInfoByIndex(int iSelIndex);

	// ͨ��treeitem�õ���������id
	unsigned long GetMonsterIdByTreeItem(wxTreeItemId treeItem);

	// ��ս���.
	void ClearUI();

	// ���action����
	void ClearActionData();

	// ��ռ�����
	void ClearCounter();

	//
	// ��Ҫʹ�õĿؼ�����
	//
	//////////////////////////////////////////////////////////////////////////////////////////////////




public:

	////////////////////////////////////////////////////////////////////////////////////////////
	//
	// ��������
	//
	Fairy::SceneManipulator*	m_pSceneManipulator;
	Fairy::CMonsterEditaction*	m_pMonsterEditAction;

	// �õ�action
	void GetAction();

	// ���ó����������
	void SetSceneManipulator(Fairy::SceneManipulator* sceneManipulator);


	// �����
	void SetActive(bool bShow);

public:

	CMonsterEditDlg(const wxString& resourceGroup = wxT("MonsterEdit"));

	CMonsterEditDlg(
					wxWindow *parent, 
					const wxString& resourceGroup = wxT("MonsterEdit"),
					wxWindowID winid = wxID_ANY,
					const wxPoint& pos = wxDefaultPosition,
					const wxSize& size = wxDefaultSize,
					long style = wxTAB_TRAVERSAL | wxNO_BORDER,
					const wxString& name = "MonsterEdit"
					);


	
	//****************************************************************************************************/
	//
	// ��ʼ������ ���������
	//

	// ����
    bool Create(
				wxWindow *parent,
                wxWindowID winid = wxID_ANY,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxTAB_TRAVERSAL | wxNO_BORDER,
                const wxString& name = "MonsterEdit");


	// ��ʾ����
	bool Show(bool show);

	
	//
	// ��ʼ������ ���������
	//
	//****************************************************************************************************/
										

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// �ؼ����¼���Ӧ������
	//

		// ѡ��һ�����͵Ĺ��
		 void OnSelected(wxListEvent& event);

		 // �޸Ĺ��������ť.
		void OnModifyData( wxCommandEvent &event );

		// �޸Ķ����������İ�ť.
		void OnModifyMultiData( wxCommandEvent &event );

		// �޸Ĺ�����Ϣ
		void OnModifyMonsterInfo( wxCommandEvent &event );

		// ���ٹ���ķ���
		void OnDecMonsterDir( wxCommandEvent &event );

		// ���ӹ��﷽��
		void OnIncMonsterDir( wxCommandEvent &event );

		void OnEditReputationID( wxCommandEvent &event );

		void OnSelectedType( wxCommandEvent &event );

		void OnSelectedAIFile( wxCommandEvent &event );
	//
	//
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// ����֪ͨ�ص�����
	//

	// ���һ������
	virtual void onAddNewMonster(unsigned long ulMonsterId, unsigned long ulInstanceId);

	// ɾ��һ������
	virtual void onDelMonster( unsigned long ulMonsterId, unsigned long ulInstanceId);

	// ѡ��һ������
	virtual void onSelMonster( unsigned long ulMonsterId, unsigned long ulInstanceId);

	// �ƶ�һ������
	virtual void onMoveMonster( unsigned long ulMonsterId, Ogre::Vector3& pos);


	//
	// ����֪ͨ�ص�����
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// �õ�������Ϣ.
	//

	// �õ�guid
	unsigned long GetGuid();

	// �õ�����ai
	int GetBaseAIId();

	// �õ��߼�ai
	int GetAdvanceAIId();

	// �õ�type
	int GetMonsterType();

	// �õ���Ӫ
	int GetCamp();

	// �õ��¼�id
	int GetEvenId();

	// �õ���id
	int GetGroupId();

	// �õ�����id
	int GetTeamId();

	// �õ�ʵ������
	std::string GetInstanceName();

	// �õ��ƺ�����
	std::string GetTitleName();

	int GetLevel();

    // �õ�����
	float GetDir();

	// �õ�ˢ��ʱ��
	int GetRefreshTime();

	// �õ�λ��
	Ogre::Vector3 GetMonsterPos();

	// �õ�Ѳ��·��
	int GetPatrolId();

	// �õ��̵�����
	int GetShopIndex(int iIndex);

	int GetReputationID();

	// ��ȡ����
	unsigned long ReadMonsterFromFile(std::string& strFileName);

	// �洢����
	unsigned long SaveMonsterToFile(std::string& strFileName);

	//
	// �õ�������Ϣ.
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	

	

	DECLARE_EVENT_TABLE()
};


#endif
