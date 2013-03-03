#ifndef __CLASKCHARLIST_H__
#define __CLASKCHARLIST_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


	class CLAskCharList : public Packet 
	{
	public:
		CLAskCharList( ){} ;
		virtual ~CLAskCharList( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_CL_ASKCHARLIST;}
		virtual UINT			GetPacketSize() const 
		{
			return sizeof(CHAR)*MAX_ACCOUNT;
		}

	public:
		//ʹ�����ݽӿ�

		const	CHAR*			GetAccount()	const;
		VOID					SetAccount(const CHAR*	pAccount);

		PlayerID_t				GetPlayerID() const
		{
			return PlayerID;
		}

		VOID					SetPlayerID(PlayerID_t	 pID)
		{
			PlayerID = pID;
		}	

	private:
		//����

		//�û�����
		CHAR					szAccount[MAX_ACCOUNT+1];
		//��ҳ�id���ͻ��˲�����д
		PlayerID_t				PlayerID;
	};

	class CLAskCharListFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CLAskCharList() ; }
		PacketID_t	GetPacketID()const { return PACKET_CL_ASKCHARLIST ; }
		UINT		GetPacketMaxSize() const
		{ 
			return	sizeof(CHAR)*MAX_ACCOUNT;
		}
	};


	class CLAskCharListHandler 
	{
	public:
		static UINT Execute( CLAskCharList* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;


#endif