

#ifndef __LOGINPLAYER_H__
#define __LOGINPLAYER_H__

#include "Type.h"
#include "Player.h"
#include "MiBao.h"


class LoginPlayer : public Player
{
public :
	LoginPlayer( ) ;
	~LoginPlayer( ) ;

	//��Ϣִ�нӿ�
	virtual BOOL		ProcessCommand( BOOL Option = TRUE ) ;
	//���ݽ��սӿ�
	virtual BOOL		ProcessInput( ) ;
	//���ݷ��ͽӿ�
	virtual BOOL		ProcessOutput( ) ;

//�����ӿڣ����������߼�
	virtual BOOL		HeartBeat( UINT uTime=0 ) ;

	//�������
	virtual VOID		CleanUp( ) ;


public :
	//Ӧ�ýӿ�
	virtual BOOL		IsBillingPlayer( ){ return FALSE; } ;
	virtual BOOL		IsLoginPlayer( ){ return TRUE; } ;
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
	virtual VOID		ResetKick( ) ;
	virtual VOID		Disconnect( ) ;
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
public :
	const CHAR*				GetAccount();
	VOID					SetAccount(const CHAR*  pAccount);
	
	UINT					GetVersion();
	VOID					SetVersion(UINT uVersion);

	UINT					GetQueuePos();
	VOID					SetQueuePos(UINT QueuePos);

	INT						GetCharNumber();
	VOID					SetCharNumber(INT CharNumber);

	VOID					SetCharGUID(GUID_t	guid,UINT uIndex);

	BOOL					IsGUIDOwner(GUID_t guid);

	VOID					SetUserKey(UINT key);
	UINT					GetUserKey();

	VOID					SetDBOperating(BOOL bOp);
	BOOL					GetDBOperating();

	VOID					SetReadyKickCount(INT KickCount);	
	INT						GetReadyKickCount();

	UINT					GetLastSendTurnTime();
	VOID					SetLastSendTurnTime(UINT uTime);

	INT						GetWrongPWCount();
	VOID					SetWrongPWCount(INT Count);
	
	SceneID_t				GetChooseSceneID() const;
	VOID					SetChooseSceneID(SceneID_t sid);

	WORLD_POS				GetWorldPos()	const;
	VOID					SetWorldPos(WORLD_POS& pos);

	UINT					GetAccOnlineTime(){return m_AccountTotalOnlineTime;}
	VOID					SetAccOnlineTime(UINT nTime);

	BOOL					GetAccFatigueSign(){return m_FatigueSign;}
	VOID					SetAccFatigueSign(CHAR cSign);

	const BYTE				GetProxyType() const{return m_ProxyType;}
	VOID					SetProxyType(BYTE nType){m_ProxyType=nType;}	//enum CL_NetProvider
	VOID					SendCreateCode();
	VOID					SendLoginCode();

	//�Ƿ�ǰ��Ҵ��ڶϿ����ӵ���ʱ״̬
	BOOL					IsDisconnectCountDown(VOID) const;
	//�����Ͽ����ӵ���ʱ������Ϊ����ʱ������
	VOID					TrigerDisconnectCountDown(UINT time);
	//��λ�Ͽ����ӵ���ʱ
	VOID					ResetDisconnectCountDown(VOID);
private:
	//��������˶Ͽ����ӵ���ʱ��ִ�е���ʱ������ʱ��������FALSE�����򷵻�TRUE
	BOOL					DisconnectCountDown(UINT now);
public:
	BOOL					GetPhoneBind();
	VOID					SetPhoneBind(CHAR cSign);

	BOOL					GetIPBind();
	VOID					SetIPBind(CHAR cSign);

	BOOL					GetMiBaoBind();
	VOID					SetMiBaoBind(CHAR cSign);

	BOOL					GetMacBind();
	VOID					SetMacBind(CHAR cSign);

	BOOL					GetRealNameBind();
	VOID					SetRealNameBind(CHAR cSign);

	BOOL					GetInputNameBind();
	VOID					SetInputNameBind(CHAR cSign);

	UINT					m_LastDBOpTime;		//���һ�����ݿ����ʱ��
private :
	uint					m_Status ;
	GUID_t					m_AccountGuid;				//�˺�guid ��billingsystem ���أ�ͨ����֤�Ժ�
														//�Ŀͻ����Ժ����������
	UINT					m_Version;					//�ͻ��˰汾
	UINT					m_LastSendProcessTurn;		//������߼��Ŷ���Ϣʱ��
	UINT					m_QueuePos;					//�ڶ����е�λ��
	INT						m_CharNumber;				//��ɫ����
	CHAR					szAccount[MAX_ACCOUNT+1] ;	//�û�����
	GUID_t					m_GUIDList[DB_CHAR_NUMBER]; //�û���ɫGUID�б�
	UINT					m_uKey;						//��ɫ��¼Key
	BOOL					m_bDBOprating;				//�Ƿ�ǰ�������ݿ����
	INT						m_ReadyKickCount;
	INT						m_WrongPWCount;				//������֤�������
	SceneID_t				m_SceneID;					//������
	WORLD_POS				m_WorldPos;					//����λ��
	//MyLock					m_Lock;	
public :
	//��Ϸ����
	uint					m_KickTime ;		//�ж��Ƿ���Ҫ�ߵ���ҵļ�ʱ��
	uint					m_LastSendTime ;	//�ϴη������ݵ�ʱ��
	UINT					m_ConnectTime;
	uint					m_CurrentTime ;		//��ǰ�߼�ʱ��
	INT						m_LeftTimeToQuit ;	//ʣ�౻����˳���ʱ��
	BOOL					m_Dirty ;			//�˱�־��ʾ��ǰ�����Ѿ���Ч��
												//����Ҫ�����κ�״̬��Ϣ���������ݷ�����
	
	BOOL					m_bEnableMiBao;		//����Ƿ�ѡ�����ܱ�����
	MiBaoGroup				m_MiBaoGroup;		//�ܱ��������
	UINT					m_MiBaoInputBeginTime;	//�ܱ�����Ŀ�ʼʱ��

	BOOL					m_FatigueSign;				//�������Ƿ���
	UINT					m_AccountTotalOnlineTime;	//�ʺ�������ʱ��

	BYTE					m_ProxyType;				//���������
	BOOL					m_bAlreadyCheckCreateCode;	//�Ƿ��Ѿ��ȶԹ���֤��
	CreateCode				m_CreateCode;				//��������ʱ����֤��
	BOOL					m_bAlreadyCheckLoginCode;	//�Ƿ��Ѿ��ȶԹ���¼��֤��
	CreateCode				m_LoginCode;				//��¼����ʱ����֤��

	CMyTimer				m_DisconnectCountDown;		//�Ͽ����ӵ���ʱ
	BOOL					m_bNeedDisconnect;			//����Ƿ���Ҫ�Ͽ�����

	AccountSafeSign			m_AccountSafeSign;			//�ʺŰ�ȫ��־
};

CHAR* ProxyTool_GetIPByNetProvider(LoginPlayer const* pPlayer,ID_t ServerID);
UINT  ProxyTool_GetPortByNetProvider(LoginPlayer const* pPlayer,ID_t ServerID);

#endif
