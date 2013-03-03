

#ifndef __GAMEPLAYER_H__
#define __GAMEPLAYER_H__

#include "Type.h"
#include "Player.h"

struct FULLUSERDATA ;
class Obj_Human ;
class RecyclePlayerManager;

class GamePlayer : public Player
{
public :
	GamePlayer( ) ;
	~GamePlayer( ) ;

	//��Ϣִ�нӿ�
	virtual BOOL		ProcessCommand( BOOL Option = TRUE ) ;
	//���ݽ��սӿ�
	virtual BOOL		ProcessInput( ) ;
	//���ݷ��ͽӿ�
	virtual BOOL		ProcessOutput( ) ;

//�����ӿڣ����������߼�
	virtual BOOL		HeartBeat( UINT uTime=0, INT nFlag=0 ) ;

	//�������
	virtual VOID		CleanUp( ) ;

	BOOL IsCanLogic( );


public :
	//Ӧ�ýӿ�

	virtual BOOL		IsGamePlayer( ){ return TRUE ; } ;
	virtual BOOL		IsServerPlayer( ){ return FALSE ; } ;

	//���ӳɹ����ʼ����������
	VOID				Init( ) ;

	//���Player����һ����Ϣ��
	//�˽ӿ�ֻ���ڱ�ִ���߳��ڴ���������ͬ��������
	virtual BOOL		SendPacket( Packet* pPacket ) ;
	
	//���״̬���á���ȡ�ӿ�
	VOID				SetPlayerStatus( uint status ){ m_Status = status ; } ;
	uint				GetPlayerStatus( ) { return m_Status ; } ;
	
	//�Ͽ��������ӣ����һ���Player����
	//�����ݱ����պ�������ϻᱻPlayerPool�����ȥʹ��
	BOOL				FreeOwn( ) ;
	BOOL				ChooseFreeOwn(RecyclePlayerManager* pRecycler);
	BOOL				ReadyFreeOwn();
	BOOL				IsCanFree();
	INT					GetFreeOwnFlag();


	virtual VOID		ResetKick( ) ;
	virtual VOID		Disconnect( ) ;

//
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
public :
	//ͨ����Ϸ�ӿ�

	//��֤�û�����ĺϷ���
	BOOL		CheckKey( uint key ) ;

	//ȡ���û�����
	BOOL		InitHuman( FULLUSERDATA* pData,INT	Type ) ;

	//�뿪������ʱ����û�����
	BOOL		ExitHuman( ) ;


	VOID		SetHuman( Obj_Human* pHuman ){ m_pHuman = pHuman ; } ;
	Obj_Human*		GetHuman( ){ return m_pHuman ; } ;

	BOOL		IsDirty( ){ return m_Dirty ; } ;
	VOID		SetDirty( BOOL bValue ){ m_Dirty = bValue ; } ;

	VOID		Save( ) ;

private :
	uint					m_Status ;

public :
	//��Ϸ����
	Obj_Human*				m_pHuman ;

	GUID_t					m_HumanGUID ;


	uint					m_KickTime ;		//�ж��Ƿ���Ҫ�ߵ���ҵļ�ʱ��
	uint					m_LastSendTime ;	//�ϴη������ݵ�ʱ��
	uint					m_CurrentTime ;		//��ǰ�߼�ʱ��
	INT						m_LeftTimeToQuit ;	//ʣ�౻����˳���ʱ��

	BOOL					m_Dirty ;			//�˱�־��ʾ��ǰ�����Ѿ���Ч��
												//����Ҫ�����κ�״̬��Ϣ���������ݷ�����

	INT						m_SaveTime ;
};





#endif
