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
		LNOTIFY_NONE,			//��
		LNOTIFY_ASK_REQUEST,	//ѯ���Ƿ�����
		LNOTIFY_KICK_REQUEST,	//Ҫ���ߴ�������
		LNOTIFY_KICK_OK,		//���˳ɹ�
		LNOTIFY_KICK_NOUSR,		//Loginû�д���
		LNOTIFY_CRC_ERR,		//��ɫ������CRCУ�����
		LNOTIFY_MAGICINT_ERR,	//��ɫ������MagicIntУ�����
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

		//���ü̳нӿ�
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
		//ʹ�����ݽӿ�
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
		//����
		//�û�����
		CHAR					m_AccName[MAX_ACCOUNT+1];
		//��ѯ״̬
		LOGIN_NOTIFY_STATUS		m_NotifyStatus;
		//Ψһֵ
		UINT					m_uKey;
		//CRC
		GUID_t					m_CharGUID;	//��ɫGUID
		UINT					m_SaveCrc;	//ShareMemory����ǰ�����Crcֵ
		UINT					m_LoadCrc;	//Login���غ�����Crcֵ
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