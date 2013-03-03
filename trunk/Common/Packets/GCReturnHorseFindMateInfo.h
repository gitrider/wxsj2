/********************************************************************************
 *	�����ˣ�	���ı�
 *	����ʱ�䣺	2008 �� 5 �� 20 ��	
 *
 *	����˵����	
 *	�޸ļ�¼��
*********************************************************************************/

#ifndef _GCReturnHorseFindMateInfo_h_
#define _GCReturnHorseFindMateInfo_h_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "GameStruct_Query.h"

namespace Packets
{
	class GCReturnHorseFindMateInfo : public Packet
	{
	public:
		GCReturnHorseFindMateInfo(){}
		virtual					~GCReturnHorseFindMateInfo(){}

		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_RETURNHORSEFINDMATEINFO; }

		virtual UINT			GetPacketSize() const
		{
			return m_HorseFindMateResult.GetSize() + sizeof(BOOL) + sizeof(BOOL) + sizeof(INT);
		}

		VOID SetHorseFindMateResult(const HorseFindMateInfoPerPage& value){
			m_HorseFindMateResult = value;
		}
		const HorseFindMateInfoPerPage& GetHorseFindMateResult() const{
			return m_HorseFindMateResult;
		}

		VOID SetSearch(BOOL value) { m_bSearch = value; }
		VOID SetContinue(BOOL value) { m_bContinue = value; }
		BOOL GetSearch() const { return m_bSearch; }
		BOOL GetContinue() const { return m_bContinue; }
		VOID SetReturnPostion(INT value) {m_iReturnPostion = value;}
		INT GetReturnPostion() const {return m_iReturnPostion;}

	private:
		BOOL m_bSearch;		// �Ƿ���������Ϊ�ķ��ؽ��
		BOOL m_bContinue;   // �Ƿ�����Ϣ��ֻ�������鿴��Ϊ�ķ��ؽ������
		HorseFindMateInfoPerPage m_HorseFindMateResult;
		INT					m_iReturnPostion; //��ѯ����Users�е����Index
	};

	class GCReturnHorseFindMateInfoFactory : public PacketFactory
	{
	public:
		Packet*					CreatePacket() { return new GCReturnHorseFindMateInfo(); }
		PacketID_t				GetPacketID() const { return PACKET_GC_RETURNHORSEFINDMATEINFO; }
		UINT					GetPacketMaxSize() const { 
			return sizeof(HorseFindMateInfoPerPage) + sizeof(BOOL) + sizeof(BOOL) + sizeof(INT); 
		}
	};

	class GCReturnHorseFindMateInfoHandler
	{
	public:
		static UINT Execute( GCReturnHorseFindMateInfo* pPacket, Player* pPlayer ) ;
	};

}

#endif // __CG_GUILD_H__
