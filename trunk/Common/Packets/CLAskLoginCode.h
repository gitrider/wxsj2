#ifndef _CL_ASKLOGINCODE_H_
#define _CL_ASKLOGINCODE_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "GameDefine.h"

enum
{
	LOGINCODE_ASK,		//����һ����֤��
	LOGINCODE_SEND,		//������ҵ�����Ĵ�
	LOGINCODE_REFRESH,	//����ˢ����֤��
};

namespace Packets
{


	class CLAskLoginCode : public Packet 
	{
	public:
		CLAskLoginCode( )
		{
			m_AskType = LOGINCODE_ASK;
			m_Answer.CleanUp();
		};
		virtual ~CLAskLoginCode( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_CL_ASKLOGINCODE ; }
		virtual UINT			GetPacketSize() const 
		{
			UINT uSize = sizeof(BYTE);
			if(LOGINCODE_SEND==m_AskType)
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

	class CLAskLoginCodeFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CLAskLoginCode() ; }
		PacketID_t	GetPacketID()const { return PACKET_CL_ASKLOGINCODE ; }
		UINT		GetPacketMaxSize() const
		{ 
			return sizeof(BYTE)+sizeof(CreateCode);
		}
	};


	class CLAskLoginCodeHandler 
	{
	public:
		static UINT Execute( CLAskLoginCode* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;

#endif