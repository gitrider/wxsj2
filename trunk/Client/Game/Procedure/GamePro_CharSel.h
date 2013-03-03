
/** ����ѡ������
 */

#pragma once


#include "GameProcedure.h"
#include "LoginDataStructDef.h"
#include "Object\Logic\UIObj\UIModel.h"		// ��������ʾ��ģ��



#define  MAX_SHOW_IN_UI		3


struct DB_CHAR_BASE_INFO;
class CObject;

class CGamePro_CharSel : public CGameProcedure
{
public:

	/// ��¼״̬
	enum PLAYER_CHARSEL_STATUS
	{
		CHARSEL_CHARSHOW,
		CHARSEL_SELDONE,
		CHARSEL_CONNNECT_GAME_SERVER_READY,	// ����Ϸ������������׼����, ��������.
	};


public:

	/// �л������Ƿ���ս��档
	BOOL			m_bClearUIModel;

	/// ��ǰѡ�е�Ҫɾ���Ľ�ɫ
	int				m_iCurSelRoleDel;

	// �û���
	std::string		m_strUserName;

	/// ��ǰ�õ��Ľ�ɫ�ĸ���
	int				m_iCharacterCount;

	/// ���뵽��Ϸ��guid
	GUID_t			m_EnterGameGUID;

	/// ��ui����ʾ��ģ��
	CModelShowInUI		m_Character[MAX_SHOW_IN_UI];	// ��������Ѿ������Ľ�ɫ�� ������ѡ�����ʹ��
	DB_CHAR_BASE_INFO	m_CharacterInfo[MAX_SHOW_IN_UI];


public:

	CGamePro_CharSel(VOID);
	virtual ~CGamePro_CharSel(VOID);


	/** ��ս�ɫ���� */
	void ClearUIModel();

	// ���һ������
	int AddCharacter(const DB_CHAR_BASE_INFO& CharacterInfo);

	// �л����ʺ��������
	void ChangeToAccountInput();

	// �л��������������
	void ChangeToCreateRole();

	// �л�����������������
	void ChangeToServerConnect();

	// �õ���ǰ��ɫ�ĸ���
	int  GetCurRoleCount();

	// ͨ�������õ���ɫ����Ϣ
	const DB_CHAR_BASE_INFO* GetRoleInfo(int iIndex);

	// ͨ�������õ���ɫ��UI ģ������
	const TCHAR* GetRoleUIName(int iIndex);

	// ɾ��һ����ɫ
	int DelRole(int iRoleIndex);

	// ɾ��ѡ��Ľ�ɫ
	int DelSelRole();

	// ������Ϸ
	int SendEnterGameMsg(int iRoleIndex);

	// λ���Ƿ�Ϸ�
	bool IsValidIndex(int iRoleIndex);

	// ���õ�ǰҪɾ���Ľ�ɫ
	void SetCurSelDel(int iIndex);


	// �����û�����
	void SetUserName(const char* pUserName); 

public:

	//��������
	virtual VOID	ProcessInput(VOID);

protected:
	PLAYER_CHARSEL_STATUS	m_Status;

	BOOL m_bIsLoadScene;

public:

	VOID SetStatus(PLAYER_CHARSEL_STATUS status);

	//��������Ⱦ���еĽӿ�
//	CObject*		m_pObject;

protected:

	virtual VOID	Init(VOID);
	virtual VOID	Tick(VOID);
	virtual VOID	Render(VOID);
	virtual VOID	Release(VOID);
	virtual VOID	CloseRequest(VOID);



//616====== ����
private:
		void InitUiModel(void);
		bool		m_bInitUiModel;

};
