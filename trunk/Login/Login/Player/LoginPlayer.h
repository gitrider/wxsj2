

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

	//消息执行接口
	virtual BOOL		ProcessCommand( BOOL Option = TRUE ) ;
	//数据接收接口
	virtual BOOL		ProcessInput( ) ;
	//数据发送接口
	virtual BOOL		ProcessOutput( ) ;

//心跳接口，处理所有逻辑
	virtual BOOL		HeartBeat( UINT uTime=0 ) ;

	//清除数据
	virtual VOID		CleanUp( ) ;


public :
	//应用接口
	virtual BOOL		IsBillingPlayer( ){ return FALSE; } ;
	virtual BOOL		IsLoginPlayer( ){ return TRUE; } ;
	virtual BOOL		IsServerPlayer( ){ return FALSE ; } ;

	//连接成功后初始化基本数据
	VOID				Init( ) ;

	//向此Player发送一个消息包
	//此接口只能在本执行线程内处理（无数据同步能力）
	virtual BOOL		SendPacket( Packet* pPacket ) ;
	
	//玩家状态设置、读取接口
	VOID				SetPlayerStatus( uint status ){ m_Status = status ; } ;
	uint				GetPlayerStatus( ) { return m_Status ; } ;
	
	//断开网络连接，并且回收Player数据
	//当数据被回收后可能马上会被PlayerPool分配出去使用
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

	//是否当前玩家处于断开连接倒计时状态
	BOOL					IsDisconnectCountDown(VOID) const;
	//触发断开连接倒计时，参数为倒计时毫秒数
	VOID					TrigerDisconnectCountDown(UINT time);
	//复位断开连接倒计时
	VOID					ResetDisconnectCountDown(VOID);
private:
	//如果启动了断开连接倒计时，执行倒计时，倒计时结束返回FALSE，否则返回TRUE
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

	UINT					m_LastDBOpTime;		//最后一次数据库操作时间
private :
	uint					m_Status ;
	GUID_t					m_AccountGuid;				//账号guid 由billingsystem 返回，通过验证以后
														//的客户端以后都用这个数据
	UINT					m_Version;					//客户端版本
	UINT					m_LastSendProcessTurn;		//最后发送逻辑排队消息时间
	UINT					m_QueuePos;					//在队列中的位置
	INT						m_CharNumber;				//角色个数
	CHAR					szAccount[MAX_ACCOUNT+1] ;	//用户名称
	GUID_t					m_GUIDList[DB_CHAR_NUMBER]; //用户角色GUID列表
	UINT					m_uKey;						//角色登录Key
	BOOL					m_bDBOprating;				//是否当前进行数据库操作
	INT						m_ReadyKickCount;
	INT						m_WrongPWCount;				//密码验证错误计数
	SceneID_t				m_SceneID;					//场景号
	WORLD_POS				m_WorldPos;					//世界位置
	//MyLock					m_Lock;	
public :
	//游戏数据
	uint					m_KickTime ;		//判断是否需要踢掉玩家的计时器
	uint					m_LastSendTime ;	//上次发送数据的时间
	UINT					m_ConnectTime;
	uint					m_CurrentTime ;		//当前逻辑时间
	INT						m_LeftTimeToQuit ;	//剩余被清除退出的时间
	BOOL					m_Dirty ;			//此标志表示当前连接已经无效，
												//不需要发送任何状态消息给世界数据服务器
	
	BOOL					m_bEnableMiBao;		//玩家是否选择了密保数据
	MiBaoGroup				m_MiBaoGroup;		//密保相关数据
	UINT					m_MiBaoInputBeginTime;	//密保输入的开始时间

	BOOL					m_FatigueSign;				//防沉迷是否开启
	UINT					m_AccountTotalOnlineTime;	//帐号总在线时间

	BYTE					m_ProxyType;				//代理的类型
	BOOL					m_bAlreadyCheckCreateCode;	//是否已经比对过验证码
	CreateCode				m_CreateCode;				//创建人物时的验证码
	BOOL					m_bAlreadyCheckLoginCode;	//是否已经比对过登录验证码
	CreateCode				m_LoginCode;				//登录人物时的验证码

	CMyTimer				m_DisconnectCountDown;		//断开连接倒计时
	BOOL					m_bNeedDisconnect;			//标记是否需要断开连接

	AccountSafeSign			m_AccountSafeSign;			//帐号安全标志
};

CHAR* ProxyTool_GetIPByNetProvider(LoginPlayer const* pPlayer,ID_t ServerID);
UINT  ProxyTool_GetPortByNetProvider(LoginPlayer const* pPlayer,ID_t ServerID);

#endif
