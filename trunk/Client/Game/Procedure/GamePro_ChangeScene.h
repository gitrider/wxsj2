
/** ��¼����
 */

#ifndef __GAMEPRO_CHANGESCENE_H__
#define __GAMEPRO_CHANGESCENE_H__

#pragma once


#include "GameProcedure.h"
#include "LoginDataStructDef.h"
#include <string>



class tSoundSource;

class CGamePro_ChangeScene: public CGameProcedure
{
public:

	//��¼״̬
	enum PLAYER_CHANGE_SERVER_STATUS
	{
		CHANGESCENE_DISCONNECT,					// �Ͽ�״̬
		CHANGESCENE_CONNECTING,					// ������
		CHANGESCENE_CONNECT_SUCCESS,			// ���ӳɹ�
		CHANGESCENE_SENDING_CGCONNECT,			// ���ڷ���������Ϣ
		CHANGESCENE_RECEIVE_CGCONNECT_SUCCESS,	// ������������
		CHANGESCENE_RECEIVE_CGCONNECT_FAIL,		// ��������������
	};

	// ����״̬
	PLAYER_CHANGE_SERVER_STATUS m_Status;

	// ����״̬
	void	SetStatus(PLAYER_CHANGE_SERVER_STATUS status);

	// �õ�״̬
	PLAYER_CHANGE_SERVER_STATUS		GetStatus(VOID);

	//
	void ConnectToGameServer();

	// ����cgconnect
	void SendCGConnectMsg();

	// �л���enter game ����
	void ChangeToEnterGameProdure();

public:
	CGamePro_ChangeScene(void);
	~CGamePro_ChangeScene(void);

	
public:
	//��������
	//virtual VOID	ProcessInput(VOID);

protected:
	virtual VOID	Init(VOID);
	virtual VOID	Tick(VOID);
	virtual VOID	Render(VOID);
	virtual VOID	Release(VOID);
};

#endif //__GAMEPRO_CHANGESCENE_H__