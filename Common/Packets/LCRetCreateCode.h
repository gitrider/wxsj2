#ifndef _LC_RETCREATECODE_H_
#define _LC_RETCREATECODE_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

enum
{
	RETCREATECODE_FAIL,			//验证码错误
	RETCREATECODE_SUCCESS,		//验证码正确
	RETCREATECODE_IMAGE,		//验证码图片
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

		//公用继承接口
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
		//使用数据接口
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
		//数据
		BYTE			m_RetType;							//结果类型
		UCHAR			m_imageBuf[IMAGE_BUF_LENGTH_1];		//图片数据
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