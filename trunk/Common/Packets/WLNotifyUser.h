#ifndef _WLNOTIFYUSER_H_
#define _WLNOTIFYUSER_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{

	enum	WORLD_NOTIFY_STATUS
	{
		WNOTIFY_NONE,			//��
		WNOTIFY_ASK_OK,			//ѯ�ʳɹ�
		WNOTIFY_KICK_REQUEST,	//Ҫ��Login����
		WNOTIFY_KICK_NOUSR,		//Worldû��Ҫ�ߵ���
	};

	class WLNotifyUser : public Packet 
	{
	public:
		WLNotifyUser( )
		{
			memset(m_AccName,0,MAX_ACCOUNT+1);
			m_NotifyStatus = WNOTIFY_NONE;
			m_uKey = -1;
		} ;
		virtual ~WLNotifyUser( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_WL_NOTIFYUSER ; }
		virtual UINT			GetPacketSize() const 
		{
			return sizeof(CHAR)*MAX_ACCOUNT+
				sizeof(WORLD_NOTIFY_STATUS)+
				sizeof(UINT);
		}

	public:
		//ʹ�����ݽӿ�
		CHAR*					GetAccName(){return m_AccName;}
		VOID					SetAccName(const CHAR*	pAccount)
		{
			Assert(pAccount);
			strncpy(m_AccName,pAccount,MAX_ACCOUNT*sizeof(CHAR));
			m_AccName[MAX_ACCOUNT] = 0;
		}
		
		WORLD_NOTIFY_STATUS		GetNotifyStatus(){return m_NotifyStatus;}
		VOID					SetNotifyStatus(WORLD_NOTIFY_STATUS wns)
		{
			m_NotifyStatus = wns;
		}

		UINT					GetUserKey(){return m_uKey;}
		VOID					SetUserKey(UINT uKey)
		{
			m_uKey = uKey;
		}
	private:
		//����
		CHAR					m_AccName[MAX_ACCOUNT+1];
		WORLD_NOTIFY_STATUS		m_NotifyStatus;
		UINT					m_uKey;
	};

	class WLNotifyUserFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new WLNotifyUser() ; }
		PacketID_t	GetPacketID()const { return PACKET_WL_NOTIFYUSER ; }
		UINT		GetPacketMaxSize() const
		{ 
			return sizeof(CHAR)*MAX_ACCOUNT+
				sizeof(WORLD_NOTIFY_STATUS)+
				sizeof(UINT);
		}
	};


	class WLNotifyUserHandler 
	{
	public:
		static UINT Execute( WLNotifyUser* pPacket, Player* pPlayer ) ;
	};
};

using namespace Packets;
#endif
