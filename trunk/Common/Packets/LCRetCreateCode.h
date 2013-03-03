#ifndef _LC_RETCREATECODE_H_
#define _LC_RETCREATECODE_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

enum
{
	RETCREATECODE_FAIL,			//��֤�����
	RETCREATECODE_SUCCESS,		//��֤����ȷ
	RETCREATECODE_IMAGE,		//��֤��ͼƬ
};

namespace Packets
{


	class LCRetCreateCode : public Packet 
	{
	public:
		LCRetCreateCode( )
		{
			m_RetType = RETCREATECODE_IMAGE;
			memset(m_imageBuf,0,sizeof(UCHAR)*IMAGE_BUF_LENGTH_1);
		};
		virtual ~LCRetCreateCode( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_LC_RETCREATECODE ; }
		virtual UINT			GetPacketSize() const 
		{
			UINT uSize = sizeof(BYTE);
			if(RETCREATECODE_IMAGE==m_RetType)
			{
				uSize += sizeof(UCHAR)*IMAGE_BUF_LENGTH_1;
			}
			return uSize;
		}

	public:
		//ʹ�����ݽӿ�
		VOID					SetRetType(BYTE nType)
		{
			m_RetType = nType;
		}
		BYTE					GetRetType()
		{
			return m_RetType;
		}
		UCHAR*					GetImageBuf(){return m_imageBuf;}
	private:
		//����
		BYTE			m_RetType;							//�������
		UCHAR			m_imageBuf[IMAGE_BUF_LENGTH_1];		//ͼƬ����
	};

	class LCRetCreateCodeFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new LCRetCreateCode() ; }
		PacketID_t	GetPacketID()const { return PACKET_LC_RETCREATECODE; }
		UINT		GetPacketMaxSize() const
		{ 
			return	sizeof(BYTE)+sizeof(UCHAR)*IMAGE_BUF_LENGTH_1;
		}
	};


	class LCRetCreateCodeHandler 
	{
	public:
		static UINT Execute( LCRetCreateCode* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets;

#endif