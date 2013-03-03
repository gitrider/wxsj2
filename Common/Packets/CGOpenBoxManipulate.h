#ifndef _CG_OPENBOXMANIPULATE_H_
#define _CG_OPENBOXMANIPULATE_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{

	class CGOpenBoxManipulate : public Packet
	{
	public:
		enum
		{
			MANIPULATE_GET_DIRECTLY = 1,		//直接领取  
			MANIPULATE_ONCE_MORE,				//再来一次
			MANIPULATE_GET,						//领取
		};

	public:
		CGOpenBoxManipulate(void)
			: m_nManipulateType(0)
		{

		}

		virtual ~CGOpenBoxManipulate(void){};

	public:
		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;//虚函数加const限制可能有问题
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_CG_OPENBOXMANIPULATE; }
		virtual UINT			GetPacketSize() const { return sizeof(INT); }

		VOID					SetManipulate( INT nManipulateType ) { m_nManipulateType = nManipulateType; };	
		INT						GetManipulate( VOID ) const { return m_nManipulateType; };

	private:
		INT		m_nManipulateType;		//操作的类型


	};

	class CGOpenBoxManipulateFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGOpenBoxManipulate() ; }
		PacketID_t	GetPacketID() const { return PACKET_CG_OPENBOXMANIPULATE; }
		UINT		GetPacketMaxSize() const { return sizeof(INT); }
	};

	class CGOpenBoxManipulateHandler 
	{
	public:
		static UINT Execute( CGOpenBoxManipulate* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;


#endif