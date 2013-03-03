#ifndef _LC_RETLOGINCODE_H_
#define _LC_RETLOGINCODE_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

enum
{
	RETLOGINCODE_FAIL,		//��֤�����
	RETLOGINCODE_SUCCESS,	//��֤����ȷ
	RETLOGINCODE_IMAGE,		//��֤��ͼƬ
};

namespace Packets
{


	class LCRetLoginCode : public Packet 
	{
	public:
		LCRetLoginCode( )
		{
			m_RetType = RETLOGINCODE_IMAGE;
			memset(m_imageBuf,0,sizeof(UCHAR)*IMAGE_BUF_LENGTH_1);
		};
		virtual ~LCRetLoginCode( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_LC_RETLOGINCODE ; }
		virtual UINT			GetPacketSize() const 
		{
			UINT uSize = sizeof(BYTE);
			if(RETLOGINCODE_IMAGE==m_RetType)
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

	class LCRetLoginCodeFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new LCRetLoginCode() ; }
		PacketID_t	GetPacketID()const { return PACKET_LC_RETLOGINCODE; }
		UINT		GetPacketMaxSize() const
		{ 
			return	sizeof(BYTE)+sizeof(UCHAR)*IMAGE_BUF_LENGTH_1;
		}
	};


	class LCRetLoginCodeHandler 
	{
	public:
		static UINT Execute( LCRetLoginCode* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets;

#endif