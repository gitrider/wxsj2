
/** ���������
 */

#pragma once


#include <Winsock2.h>
#pragma comment(lib, "Ws2_32.lib")

#include "Socket.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"
#include "Packet.h"
#include "PacketFactoryManager.h"
#include "GINode.h"
#include "..\World\WorldManager.h"



/// ���������
class CNetManager : public tNode
{
public:

	/// ���ӵ�������
	VOID			ConnectToServer(LPCTSTR szServerAddr, INT nServerPort);
	///���������ݰ�
	VOID			SendPacket(Packet* pPacket);
	//�Ͽ�����
	VOID			Close(VOID);
	//������Ϣ����
	VOID			ProcessInputStream(SocketInputStream& inputStream);
	//�õ���Ϣ����
	PacketFactoryManager&	GetPacketFactory(void) { return m_PacketFactoryManager; }

public:

	//�ڵ��ʼ��
	virtual VOID	Initial(VOID*);
	//�߼���ѭ����
	virtual VOID	Tick(VOID);
	//�ͷ��Լ�����ӵ�е���Դ
	virtual VOID	Release(VOID);

public:

public:

	enum NETMANAGER_STATUS
	{
		CONNECT_SUCESS  = 0,
		CONNECT_FAILED_CREATE_SOCKET_ERROR,
		CONNECT_FAILED_CONNECT_ERROR,
		CONNECT_FAILED_TIME_OUT,

	};

	//--------------------------------------------------------------------------------------------------------------
	//
	// Tick��ͬ����Ϸ����
	//

	// Tick ��Ϸ��¼����.
	VOID	TickGameLoginProcedure();

	// Tick ����Ϸ����
	VOID	TickGameMainProcedure();

	// Tick ����ѡ������
	VOID	TickGameSelCharcterProcedure(VOID);

	// Tick ���ﴴ������
	VOID	TickGameCreateCharcterProcedure(VOID);

	// Tick ���ӵ�����������
	VOID	TickChangeGameServerProcedure(VOID);

	// Tick ��������
	VOID	TickGameEnterProcedure(VOID);

	//
	// Tick��ͬ����Ϸ����
	//
	//--------------------------------------------------------------------------------------------------------------


	//--------------------------------------------------------------------------------------------------------------
	//
	// ��������״̬
	// 

	// ��������״̬
	VOID SetNetStatus(NETMANAGER_STATUS netStatus);

	// Tick ��Ϸ��¼����.
	VOID	SetGameLoginStatus(NETMANAGER_STATUS netStatus);

	// Tick ����Ϸ����
	VOID	SetGameMainStatus(NETMANAGER_STATUS netStatus);


	//
	//  ��������״̬
	//
	//--------------------------------------------------------------------------------------------------------------
	
public:
	CNetManager();
	virtual ~CNetManager();
	static CNetManager* GetMe(VOID) { return s_pMe; }

protected:
	//�����߳�,ר���������ӷ�������
	static UINT CALLBACK	_ConnectThread(LPVOID pParam);
	INT				ConnectThread(VOID);
	//�����߳̾��
	HANDLE			m_hConnectThread;
	UINT			m_timeConnectBegin;

private:
	friend VOID CWorldManager::ChangeScene(INT idTargetScene, INT idTargetSceneResID, const fVector2& fvPos, INT nDirection);

	VOID				WaitConnecting(VOID);
	VOID				WaitPacket(VOID);
	VOID				SendHeartBeat(VOID);

	//SOCKET������
	BOOL				Select(VOID);
	BOOL				ProcessExcept(VOID);
	BOOL				ProcessInput(VOID);
	BOOL				ProcessOutput(VOID);
	VOID				ProcessCommands(VOID);

	//��Ϣ��ִ��
	UINT				executePacket_Gen_Exception(Packet* pPacket);
	UINT				executePacket_CPP_Exception(Packet* pPacket);


protected:
	static CNetManager*		s_pMe;
	///��������Ϣ
	STRING					m_strServerAddr;
	INT						m_nServerPort;

private:
	//�ɶ����
	fd_set					m_ReadFD;
	//��д���
	fd_set					m_WriteFD;
	//�쳣���
	fd_set					m_ExceptFD;
	///SOCKET
	Socket					m_Socket;
	///���뻺����
	SocketInputStream		m_SocketInputStream;
	///���������
	SocketOutputStream		m_SocketOutputStream;
	//SOCKET��������
	PacketFactoryManager	m_PacketFactoryManager;

protected:
	KL_DECLARE_DYNAMIC(CNetManager);
};
