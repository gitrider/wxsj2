#ifndef _CL_ASKCREATECODE_H_
#define _CL_ASKCREATECODE_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "GameDefine.h"

enum
{
	CREATECODE_ASK,			//����һ����֤��
	CREATECODE_SEND,		//������ҵ�����Ĵ�
	CREATECODE_REFRESH,		//����ˢ����֤��
};

namespace Packets
{


	class CLAskCreateCode : public Packet 
	{
	public:
		CLAskCreateCode( )
		{
			m_AskType = CREATECODE_ASK;
			m_Answer.CleanUp();
		};
		virtual ~CLAskCreateCode( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_CL_ASKCREATECODE ; }
		virtual UINT			GetPacketSize() const 
		{
			UINT uSize = sizeof(BYTE);
			if(CREATECODE_SEND==m_AskType)
			{
				uSize += sizeof(CreateCode);
			}
			return uSize;
		}

	public:
		//ʹ�����ݽӿ�
		VOID					SetAskType(BYTE nAskType){m_AskType = nAskType;}
		BYTE					GetAskType(){return m_AskType;}

		VOID					SetAnswer(CreateCode* pAnswer)
		{
			Assert(pAnswer);
			memcpy(&m_Answer,pAnswer,sizeof(CreateCode));
		}
		CreateCode*				GetAnswer(){return &m_Answer;}
	private:
		//����
		BYTE			m_AskType;
		CreateCode		m_Answer;
	};

	class CLAskCreateCodeFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CLAskCreateCode() ; }
		PacketID_t	GetPacketID()const { return PACKET_CL_ASKCREATECODE ; }
		UINT		GetPacketMaxSize() const
		{ 
			return sizeof(BYTE)+sizeof(CreateCode);
		}
	};


	class CLAskCreateCodeHandler 
	{
	public:
		static UINT Execute( CLAskCreateCode* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;

#endif