// CGPlayerDieResult.h
// 
// ���������Ĳ������
//
//////////////////////////////////////////////////////

#ifndef __CGPLAYERDIERESULT_H__
#define __CGPLAYERDIERESULT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGPlayerDieResult : public Packet
	{
	public:
		CGPlayerDieResult( )
		{
			m_nResultCode		= DIE_RESULT_CODE_INVALID;
		}
		virtual ~CGPlayerDieResult( ){}

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_PLAYER_DIE_RESULT ; }
		virtual UINT			GetPacketSize()const { return	sizeof(INT); }

	public:
		//ʹ�����ݽӿ�
		VOID				SetResultCode(INT nResult) { m_nResultCode = nResult; }
		INT					GetResultCode(VOID)const { return m_nResultCode; }

	private:
		INT					m_nResultCode;			// ENUM_DIE_RESULT_CODE
	};


	class CGPlayerDieResultFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGPlayerDieResult() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_PLAYER_DIE_RESULT; }
		UINT		GetPacketMaxSize()const { return	sizeof(INT); }
	};

	class CGPlayerDieResultHandler 
	{
	public:
		static UINT Execute( CGPlayerDieResult* pPacket, Player* pPlayer ) ;
	};
}

#endif // __CGPLAYERDIERESULT_H__
