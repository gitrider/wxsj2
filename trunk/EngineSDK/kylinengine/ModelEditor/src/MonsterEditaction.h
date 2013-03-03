#pragma once

#include "Action.h"
#include "HitInfo.h"
#include "ScanLine.h"
#include <OgreMaterial.h>
#include <map>
#include <vector>
#include "LogicDataStructDef.h"

class CEditDobject_NT;
namespace Fairy 
{
	class LogicModel;

	#define		FUNCTION_KEY        0X00000000
	#define		FUNC_KEY_SHIFT		0X00000001
	#define		FUNC_KEY_CTRL		0X00000002
	#define		FUNC_KEY_ALT		0X00000004
	

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// ������Ϣ����
	//

	#define		REFRESH_TIME		1000		// ����ˢ��ʱ�䣬 ��λ����
	#define     AREA_RADIUS			2			// ��뾶1.
	#define		SHOP_COUNT			4			// �̵����.

	typedef std::vector<Ogre::Vector3> LINE_VECTOR;
	class CMonsterInfo
	{

	public:

		enum ACTIVE_TYPE
		{
			AREA = 0,
			LINE,
		};


	public:
		CMonsterInfo();
		~CMonsterInfo();

	public:
		unsigned long	m_dwInstanceId;			// ÿһ��ʵ����id
		

		// ������ʾ��object
		CEditDobject_NT*     m_pDobject;
		Fairy::LogicModel*	m_pModel;

		int				m_iRefreshTime;			// ˢ��ʱ�䡣
		int				m_ActiveType;			// ����͡�

		float			m_fAreaRadius;			// �������������򣬼�¼���Χ�İ뾶 
		LINE_VECTOR		m_lineVector;			// ����������·�ߣ���¼���Χ��·��

		// �����ɵ��ļ�����д��������
		unsigned long	m_dwObjectId;			// ����id�� ͨ���������ҵ������ļ�, ���ǹ�������͡�
       	int				m_iBaseAIId;			// ����AI��š�
		std::string		m_strAdvanceAIFile;		// ����ai�ű��ļ�
		std::string		m_strEvent;				// �¼��ű��ļ�.
		unsigned long   m_dwMonsterGUID;		// �����guid
		Ogre::Vector3	m_position;				// �����λ�á�
		int				m_iGroupId;				// ��id
		int				m_iTeamId;				// �ӵ�id
		float			m_fMonstDir;			// ����ķ���
		int				m_iReputationID;		// ���������
		int				m_iLevel;				// ����ĵȼ�

		int				m_iShop[SHOP_COUNT];	// �̵�			

	

	public:

		void DeleteDObject();

	};
	typedef std::map<unsigned long, CMonsterInfo> MONSTERINFO_MAP;
	

	//
	// ������Ϣ����
	//
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class CMonsterEditaction : public Action
	{


	public:

		 CMonsterEditaction(SceneManipulator* sceneManipulator);
    	~CMonsterEditaction(void);


	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// ��Ա���ݶ���.
	//
	public:


		// �������Ŀ���ʱ��;
		unsigned long m_ulAddtime;
		// �Ƿ���Ӳ�����
		bool m_bIsAddOperator;

		// ������ʾ����༭���ܵĽڵ�.
		Ogre::SceneNode*	m_pSceneNode;			

		// ��ʱѡ�е��������.
		CEditDobject_NT* m_tempObject;

		// ��¼���̵İ���״̬.
		unsigned long     m_FuncKeyState;

		// �Ƿ���ʾ����.
		bool  m_bIsShow;

		// �����б�
		MONSTERINFO_MAP	m_MonsterInfoMap;

		// ע���ڵ�ͼ�еĹ�����б�.
		POS_ID_MAP		m_MonsterPosMap;

		// ��ǰ������ӹ���ʵ��id, ����µĹ�ʹ��
		// 
		// �� 0 ��ʼ�� 0xffffffff��ʾ���Ϸ�id 
		unsigned long m_dwMonsterInstanceId;

		// ��ǰ�༭�Ĺ�������ơ�
		String	m_strCurEditMonsterName;

		// ��ǰ�༭�Ĺ������ε�id
		unsigned long   m_dwCurEditMonsterId;

		// �� 0 ��ʼ�� 0xffffffff��ʾ���Ϸ�id 
		unsigned long m_dwCurSelMonsterInstanceId;

		// ��ǰ�༭�Ĺ������ε�id
		unsigned long   m_dwGUID;

		// ����id����������ӳ���
		ID_NAME_MAP m_IdNameMap;

		// id������ӳ���
		ID_ID_MAP   m_Id_LookId_Map;

		// �õ�����id�� �����ֵ�ӳ���.
		void LoadMonsterIdNameTable();

		// �õ��������ͱ�
		void LoadMonsterTypeTable();

		// ��ǰ������
		Ogre::Vector3	m_curPoint;


	//
	// ��Ա���ݶ���.
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////////////



	////////////////////////////////////////////////////////////////////////////////////
	//
	// ���̺Ͷ��̲���
	//

	// �������ȡ�ļ�
	unsigned long ReadEditMonsterFromFile(std::string strFileName);
	
	// ������洢���ļ�
	unsigned long SaveEditMonsterToFile(std::string strFileName);

		
	//
	// ���̺Ͷ��̲���
	//
	////////////////////////////////////////////////////////////////////////////////////




	/////////////////////////////////////////////////////////////////////////////////////////
	//
	// �����Ĳ���
	//

	
	// ��������༭�ĳ����ڵ�
	void CreateSceneNode();

	// ��������༭�����ڵ�
	void HideScene(bool bHide);

	// ���ݵ��εĸ߶ȸı�༭������
	void SetMonsterHeight();

	// �������λ�ò���.
	void SetCamera(float fx, float fy);

	//
	// �����Ĳ���
	//
	/////////////////////////////////////////////////////////////////////////////////////////
	
	

	/////////////////////////////////////////////////////////////////////////////////////////////
	//
	// ���ܼ����ݲ�����
	//
	//

	
	// ���ò�������.
	void setParameter(const String& name, const String& value);

	// ���ð��µĹ��ܼ�
	void SetFuncKey(unsigned long ulFuncKey);

	//  ��չ��ܼ�.
	void ClearFuncKey();


	//
	//
	// ���ܼ����ݲ�����
	//
	///////////////////////////////////////////////////////////////////////////////////////////////

	
	protected:
    

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// �ⲿ���õ��麯����
	//

	// ���������麯��.
    void _onMove(const Point& pt);
    void _onBegin(const Point& pt);
    void _onDrag(const Point& pt);
    void _onEnd(const Point& pt, bool canceled);

	// ��ǰ����active����.
    void _onActive(bool active);

	//
	// �ⲿ���õ��麯����
	//
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	

	public:
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// ����༭������
	//

	//�����µı༭ģ�͡�
	void SetNewEditObject(String& name, unsigned long dwObjectId);

	//�����µı༭ģ�͡�
	void SetNewEditObject(const char* pName, unsigned long dwObjectId);

	// ����ģ�͡�
	void ChangeModel(String& name);

	// ����ģ�͡�
	void ChangeModel(const char* pName);

	// ���һ������
	bool AddNewMonster(const char* pName,  Ogre::Vector3& pos, unsigned long dwObjectId);

	// ���һ������
	bool AddNewMonster(String& name,  Ogre::Vector3& pos, unsigned long dwObjectId);

	// ���һ������
	bool AddNewMonster(Ogre::Vector3& pos, unsigned long dwObjectId, bool bTransPos = false);

	// ��ʼ���༭������
	void InitEditData();

	// �ͷ����еĹ��
	void ReleaseAllMonster();

	// �õ�ѡ�еĹ���
	unsigned long  SelectObject( const Point& pt);

	// �Ƿ�ѡ�й���
	bool IsSelectObject(Ogre::Ray& ray, CMonsterInfo& monsterInfo, float& fdis);

	// ����ѡ������Ĳ���
	void DoSelectObject(const Point& pt);

	// �����������Ĳ���
	void DoAddNewMonster(const Point& pt);

	// ɾ������
	void DelSelectMonster();

	
	// ɾ������
	void DelMonster(unsigned long dwInstanceId);

	// ����ɾ������Ĳ���
	void DoDelMonster(const Point& pt);

	// ���ݹ���ʵ��id�� �õ��������Ϣָ��
	CMonsterInfo* GetMonster(unsigned long dwInstanceId);

	// �ı����еĹ����ˢ��ʱ�䡣
	void ChangeAllMonsterRefreshTime(int iTime = 1000);

	// �ı����й���Ļ�뾶��
	void ChangeAllMonsterRadius(float fRadius = 2.0f);

	// �ı�ˢ��ʱ��ͻ�뾶��
	void ChangeAllMonsterRefreshTimeRadius(int iTime = 1000, float fRadius = 2.0f);

	// �ı�ĳһ��ֵ�ˢ��ʱ�䡣
	void ChangeMonsterRefreshTime_Type(unsigned long dwObjectId, int iTime = 1000);

	// �ı�ĳһ��ֵĻ�뾶��
	void ChangeMonsterRefreshRadius_Type(unsigned long dwObjectId, float fRadius = 2.0f);

	// �ı�ĳһ��ֵ�ˢ��ʱ��ͻ�뾶��
	void ChangeMonsterRefreshTimeRadius_Type(unsigned long dwObjectId, int iTime = 1000, float fRadius = 2.0f);

	// �ı�����ˢ��ʱ�䡣
	void ChangeMonsterRefreshTime(unsigned long dwInstanceId, int iTime = 1000);

	// �ı�����ˢ�°뾶��
	void ChangeMonsterRefreshRadius(unsigned long dwInstanceId, float fRadius = 2.0f);

	// �ı�����ˢ��ʱ��Ͱ뾶��
	void ChangeMonsterRefreshTimeRadius(unsigned long dwInstanceId, int iTime = 1000, float fRadius = 2.0f);

	// �ı����ķ���
	void ChangeMonsterDir(unsigned long dwInstanceId, float radian);

	//��ռ�����
	void ClearCounter();

	// �����Ѿ��޸�
	void SetModifyed(bool bModifyed = true);

	// �õ�id������ӳ���
	void GetMonsterIdLookTable();

	// �õ������λ��
	Ogre::Vector3   GetMonsterPos(unsigned long ulInstance);



	//
	// ����༭������
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

		
	public:

		// �õ�������������.
		const String& getName(void) const;

		
	};
}// namespace Fairy 
