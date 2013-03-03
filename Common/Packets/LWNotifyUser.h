#ifndef _LWNOTIFYUSER_H_
#define _LWNOTIFYUSER_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

#include "DB_Struct.h"

namespace Packets
{
	enum	LOGIN_NOTIFY_STATUS
	{
		LNOTIFY_NONE,			//空
		LNOTIFY_ASK_REQUEST,	//询问是否在线
		LNOTIFY_KICK_REQUEST,	//要求踢此人下线
		LNOTIFY_KICK_OK,		//踢人成功
		LNOTIFY_KICK_NOUSR,		//Login没有此人
		LNOTIFY_CRC_ERR,		//角色档案的CRC校验出错
		LNOTIFY_MAGICINT_ERR,	//角色档案的MagicInt校验出错
	};

	class LWNotifyUser : public Packet 
	{
	public:
		LWNotifyUser( )
		{
			memset(m_AccName,0,MAX_ACCOUNT+1);
			m_NotifyStatus = LNOTIFY_NONE;
			m_uKey = -1;
			m_SaveCrc = m_LoadCrc = 0;
			m_CharGUID = INVALID_GUID;
		} ;
		virtual ~LWNotifyUser( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_LW_NOTIFYUSER ; }
		virtual UINT			GetPacketSize() const 
		{
			return 	sizeof(CHAR)*MAX_ACCOUNT+
				sizeof(LOGIN_NOTIFY_STATUS)+
				sizeof(UINT)*3+
				sizeof(GUID_t);	
		}

	public:
		//使用数据接口
		const	CHAR*			GetAccount()	const;
		VOID					SetAccount(const CHAR*	pAccount);

		LOGIN_NOTIFY_STATUS		GetNotifyStatus()
		{
			return m_NotifyStatus;
		}
		VOID					SetNotifyStatus(LOGIN_NOTIFY_STATUS lns)
		{
			m_NotifyStatus = lns;
		}

		UINT					GetUserKey()	const
		{
			return m_uKey;
		}

		VOID					SetUserKey(UINT uKey)
		{
			m_uKey = uKey;
		}

		UINT					GetSaveCrc() const{return m_SaveCrc;}
		VOID					SetSaveCrc(UINT scrc){m_SaveCrc=scrc;}

		UINT					GetLoadCrc() const{return m_LoadCrc;}
		VOID					SetLoadCrc(UINT lcrc){m_LoadCrc=lcrc;}

		GUID_t					GetCharGUID() const{return m_CharGUID;}
		VOID					SetCharGUID(GUID_t guid){m_CharGUID=guid;}
	private:
		//数据
		//用户名称
		CHAR					m_AccName[MAX_ACCOUNT+1];
		//查询状态
		LOGIN_NOTIFY_STATUS		m_NotifyStatus;
		//唯一值
		UINT					m_uKey;
		//CRC
		GUID_t					m_CharGUID;	//角色GUID
		UINT					m_SaveCrc;	//ShareMemory保存前计算的Crc值
		UINT					m_LoadCrc;	//Login加载后计算的Crc值
	};

	class LWNotifyUserFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new LWNotifyUser() ; }
		PacketID_t	GetPacketID()const { return PACKET_LW_NOTIFYUSER ; }
		UINT		GetPacketMaxSize() const
		{ 
			return 	sizeof(CHAR)*MAX_ACCOUNT+
					sizeof(LOGIN_NOTIFY_STATUS)+
					sizeof(UINT)*3+
					sizeof(GUID_t);
		}
	};


	class LWNotifyUserHandler 
	{
	public:
		static UINT Execute( LWNotifyUser* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;


#endif