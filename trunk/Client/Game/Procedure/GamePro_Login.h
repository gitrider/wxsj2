
/** ��¼����
 */
#pragma once


#include "GameProcedure.h"
#include "LoginDataStructDef.h"
#include <string>
#include <vector>



// ���繩Ӧ�̵ĸ���
#define PROVIDE_COUNT	3

// ���Ϸ��ַ�������
typedef std::vector<std::string>  UNUSE_STRING_VECTOR;

class tSoundSource;
class CGamePro_Login : public CGameProcedure
{
public:

	//��¼״̬
	enum PLAYER_LOGIN_STATUS
	{
		LOGIN_DEBUG_SETTING,			//!< -- FOR DEBUG �û�����

		LOGIN_SELECT_SERVER,			// ѡ�����������.
		LOGIN_DISCONNECT,				//!< ��δ��¼
		
		LOGIN_CONNECTING,				//!< ���ӷ�������...
		LOGIN_CONNECTED_OK,				//!< �ɹ����ӵ�������
		LOGIN_CONNECT_FAILED,			//!< ���ӵ�������ʧ��

		LOGIN_ACCOUNT_BEGIN_REQUESTING,	// ��������֮ǰ״̬
		LOGIN_ACCOUNT_REQUESTING,		//!< �����ʺ���Ϣ���ݰ���������...
		LOGIN_ACCOUNT_OK,				//!< �ʺ���֤�ɹ�
		LOGIN_ACCOUNT_FAILED,			//!< �ʺ���֤ʧ��

		LOGIN_WAIT_FOR_LOGIN,			// �Ŷӽ�����Ϸ״̬.

		LOGIN_FIRST_LOGIN,				// �״ε�¼
		LOGIN_CHANGE_SCENE,				// �л��������ص�¼
	};

	PLAYER_LOGIN_STATUS		GetStatus(VOID) { return m_Status; }
	VOID					SetStatus(PLAYER_LOGIN_STATUS status) { m_Status = status; }
	VOID					SetRelogin(BOOL bReLogin) { m_bReLogin = bReLogin; }

	// ���õ�¼״̬Ϊ�״ε�¼���������л������ĵ�¼״̬��
	VOID					FirstLogin(VOID);

	// ���õ�¼״̬Ϊ�л������ĵ�¼״̬
	VOID					LoginForChangeScene(VOID);

	//-----------------------------------------------------------------------------------------------------------
	//
	// �������ļ��õ�, login server ��ip��ַ�Ͷ˿ں�
	//
	//

	// login server ��ip��ַ�Ͷ˿ں�.
	TCHAR	m_szLoginServerAddr[64];
	INT		m_nLoginServerPort;

	// ���ö˿ں�.
	void SetPort(INT iPort);

	// ����ip��ַ
	void SetIpAddr(LPCTSTR szServerAddr);


	// ������Ϣ
	PAREA_INFO  m_pAreaInfo;

	// �������
	int			m_iAreaCount;


	// ��ȡ�����������ļ��Ĵ�����Ϣ״̬
	enum LOAD_LOGIN_SERVER_ERR
	{
		LOAD_LOGIN_SERVER_SUCCESS = 0,		// ��ȡ�ɹ�.
		LOAD_LOGIN_SERVER_FILE_NOT_FINDED,	// �Ҳ����ļ�
		LOAD_LOGIN_SERVER_AREA_NOT_FINDED,	// û������
		LOAD_LOGIN_SERVER_OTHER_ERR,		// ��������

	};


	// ��ȡlogin server��Ϣ
	//
	// ����ֵ: 
	//			0 -- ��ȡ�ɹ�
	//			1 -- ʧ���Ҳ��������ļ�
	//          2 -- ��������    
	int  LoadLoginServerInfo(std::string strLoginServerInfoFile = "..\\Patch\\serverlist.tab");

	// ��ȡ������Ϣ.
	int ReadAreaInfo(FILE* pFile, int iAreaIndex);

	// ��ȡlogin server��Ϣ.
	int ReadLoginInfo(FILE* pFile, int iAreaIndex, int iLoginServerIndex);

	// ��ʼ��������Ϣ
	void InitLoginAreaInfo();

	//
	// 
	// �������ļ��õ�, login server ��ip��ַ�Ͷ˿ں�
	//
	//-----------------------------------------------------------------------------------------------------------------------------
	



	//-----------------------------------------------------------------------------------------------------------------------------
	//
	// �߼����ݲ���
	//
	//

	// �õ�����ĸ���
	int GetAreaCount();

	// �õ���������
	const TCHAR* GetAreaName(int iAreaIndex);

	// �õ�������Ϣ
	PAREA_INFO GetAreaInfo(int iAreaIndex);

	// �õ�������login server �ĸ���
	int GetAreaLoginServerCount(int iAreaIndex);

	// �õ�������login server ����Ϣ.
	PLOGINSERVER_INOF GetAreaLoginServerInfo(int iAreaIndex, int iLoginServerIndex);

	// ���ӵ�login server
	int ConnectToLoginServer(int iAreaIndex, int iLoginServerIndex);

	// ���ӵ�login server
	int ConnectToLoginServer();

	// �õ���Ϸ�汾
	int GetGameVersion();

	// ��ǰѡ�������id
	int m_iCurSelAreaId;

	// ��ǰѡ���login server id
	int m_iCurSelLoginServerId;

	// ��¼ѡ��ķ�����
	int SelectLoginServer(int iAreaIndex, int iLoginServerIndex);
	VOID GetLastLoginAreaAndServerIndex(INT& area, INT &serverId, STRING&name);
	VOID GetLastLoginServerID();

	// �ڽ�����ѡ��ɵķ�����
	VOID SelectOldServer();



	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// �Զ�ѡ����������
	//

	// ��ǰѡ�������
	int m_iAutoAreaIndex[PROVIDE_COUNT];

	// ��ǰѡ���login server ����
	int m_iAutoLoginServerIndex[PROVIDE_COUNT];


	// ǰһ���ҵ�����״̬���е�����.
	int m_iPreFindIdle;

	// ǰһ���ҵ�����״̬����������.
	int m_iPreFindNor;

	// ǰһ���ҵ�����״̬ӵ��������.
	int m_iPreFindBusy;

	// ǰһ���ҵ�����״̬����������.
	int m_iPreFindFull;

	// ���Ź�Ӧ��
	PROVIDE_INFO m_Provide0;

	// ��ͨ��Ӧ��
	PROVIDE_INFO m_Provide1;

	// ������Ӧ��
	PROVIDE_INFO m_Provide2;

	// ͨ�����繩Ӧ��ѡ��һ��login server
	int AutoSelLoginServer(int iProvide);

	// �����Զ�ѡ��login server��Ҫ�����ݱ�
	int BuildAutoSelTable(); 


	// 
	//  �߼����ݲ���
	//
	//-------------------------------------------------------------------------------------------------------------------------------




	//-------------------------------------------------------------------------------------------------------------------------------
	//
	// �������
	//

	// ���ʺ��������
	int OpenCountInputDlg();

	// �򿪷�����ѡ�����
	int OpenSelectServerDlg();

	// ������ѡ�����
	int OpenSelectCharacter();

	// �л�������ѡ�����
	int ChangeToCharacterSel();

	//
	// �������
	//
	//-------------------------------------------------------------------------------------------------------------------------------


	//-------------------------------------------------------------------------------------------------------------------------------
	//
	// ����������Ϣ����
	//

	// ����ͬ����Ϣ
	int SendClConnectMsg();

	// �û���
	std::string m_strUserName;

	// ����
	std::string m_strPassword;

	// ����û���������.
	int CheckAccount(CHAR* szUserName, CHAR* szPassword);

	// ����check msg ��Ϣ
	int SendCheckAccountMsg();

	//
	// ����������Ϣ����
	//
	//-------------------------------------------------------------------------------------------------------------------------------


public:
	//��������
	virtual VOID	ProcessInput(VOID);

protected:
	virtual VOID	Init(VOID);
	virtual VOID	Tick(VOID);
	virtual VOID	Render(VOID);
	virtual VOID	Release(VOID);

protected:

	// �����������������״̬.
	PLAYER_LOGIN_STATUS	m_Status;
	//�Ƿ��ǴӴ��������µķ�����
	BOOL				m_bReLogin;
	//��������
	tSoundSource*		m_pSoundSource;

	INT	m_nPrevArea;
	INT m_nPrevServer;

public:
	CGamePro_Login(VOID);
	virtual ~CGamePro_Login(VOID);
};
