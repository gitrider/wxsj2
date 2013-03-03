#ifndef _CL_CONNECT_H_
#define _CL_CONNECT_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{

	class CLConnect : public Packet 
	{

	public:
		CLConnect( ){} ;
		virtual ~CLConnect( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_CL_CONNECT ; }
		virtual UINT			GetPacketSize() const 
		{
			return sizeof(BYTE) +
				  sizeof(CL_NetProvider);
		}

	public:
		//ʹ�����ݽӿ�
		VOID					SetUseMiBao(BYTE nUse){mMiBao = nUse;}
		BYTE					GetUseMiBao(){return mMiBao;}

		VOID					SetNetProvider(CL_NetProvider netProvider){ mNetProvider	= netProvider; }
		CL_NetProvider			GetNetProvider() {return mNetProvider;}
	private:
		//����
		BYTE			mMiBao;
		CL_NetProvider	mNetProvider;
		
	};

	class CLConnectFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CLConnect() ; }
		PacketID_t	GetPacketID()const { return PACKET_CL_CONNECT ; }
		UINT		GetPacketMaxSize() const
		{ 
			return sizeof(BYTE)+
				   sizeof(CL_NetProvider);
		}
	};


	class CLConnectHandler 
	{
	public:
		static UINT Execute( CLConnect* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;

#endif