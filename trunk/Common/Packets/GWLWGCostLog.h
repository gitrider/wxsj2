#ifndef _GWLWG_COSTLOG_H_
#define _GWLWG_COSTLOG_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	
	class GWLWGCostLog : public Packet 
	{
	public:
		enum
		{
			GW_COST_INVALID	= -1,

			GW_COST_LOG,
			WL_COST_LOG,
			LW_COST_LOG,
		};

		enum
		{
			RET_COST_LOG_OK		= 0x01,			//�ɹ�
			RET_COST_LOG_EXIST	= 0x02,			//�Ѵ���
		};

		GWLWGCostLog( )
		{
			m_LogPos = -1;
			memset(m_SerialKey,0,MAX_PRIZE_SERIAL_LENGTH+1);
		};

		virtual ~GWLWGCostLog( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GWLWG_COSTLOG; }
		virtual UINT			GetPacketSize() const 
		{
			UINT uSize	= sizeof(BYTE);
			if(GW_COST_LOG == m_nType)
			{
				uSize	+= sizeof(_COST_LOG);
			}
			else if(WL_COST_LOG == m_nType)
			{
				uSize	+= sizeof(ID_t);
				uSize	+= sizeof(_COST_LOG);
			}
			else if(LW_COST_LOG == m_nType)
			{
				uSize	+= sizeof(ID_t);
				uSize	+= sizeof(BYTE);
				uSize	+= sizeof(CHAR)*MAX_PRIZE_SERIAL_LENGTH;
			}

			return uSize;
		}

	public:
		VOID				SetType(BYTE nType){m_nType = nType;}
		BYTE				GetType(){return m_nType;}

		VOID				SetResult(BYTE nRes){m_nResult = nRes;}
		BYTE				GetResult(){return m_nResult;}

		VOID				SetLogPos(INT nPos){m_LogPos = (ID_t)nPos;}
		ID_t				GetLogPos(){return m_LogPos;}

		_COST_LOG*			GetLogInfo(){return &m_LogInfo;}

		VOID				SetBillingRetKey(const CHAR* pSerial)
		{
			Assert(pSerial);
			strncpy(m_SerialKey,pSerial,MAX_PRIZE_SERIAL_LENGTH);
			m_SerialKey[MAX_PRIZE_SERIAL_LENGTH] = 0;
		}
		const CHAR*			GetBillingRetKey(){return m_SerialKey;}
	private:
		BYTE				m_nType;		//��Ϣ������
		BYTE				m_nResult;		//������
		ID_t				m_LogPos;		//������Ϣ��λ��
		_COST_LOG			m_LogInfo;		//Ԫ��������Ϣ
		CHAR				m_SerialKey[MAX_PRIZE_SERIAL_LENGTH+1];		//Billing���ص��������к�
	};

	class GWLWGCostLogFactory : public PacketFactory 
	{
	public:
		Packet*			CreatePacket()		{ return new GWLWGCostLog() ; }
		PacketID_t		GetPacketID() const { return PACKET_GWLWG_COSTLOG; }
		UINT			GetPacketMaxSize() const
		{ 
			return sizeof(BYTE)+sizeof(BYTE)+sizeof(ID_t)+sizeof(_COST_LOG);
		}
	};


	class GWLWGCostLogHandler 
	{
	public:
		static UINT Execute( GWLWGCostLog* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;

#endif