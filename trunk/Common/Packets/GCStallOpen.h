// GCStallOpen.h
// 
// ���߿ͻ��������Ƿ�ɹ�
// 
//////////////////////////////////////////////////////

#ifndef __GCSTALLOPEN_H__
#define __GCSTALLOPEN_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"
#include "GCStallError.h"

namespace Packets
{
	class GCStallOpen : public Packet
	{
	public:
		struct _STALL_ITEM
		{
			BYTE		bIsPet;		//����Ʒ�Ƿ��ǳ���
			BYTE		nIndex;		//̯λ�е�λ��
			UINT		nSerial;	//���к�
			UINT		nPrice;		//�۸�
			_ITEM		item;		//��Ʒ��ϸ��Ϣ
			PET_GUID_t	PetGuid;	//����GUID

			_STALL_ITEM()
			{
				bIsPet	= 0;
				nIndex	= 0;
				nSerial = 0;
				nPrice	= 0;
			}

			bool Read( SocketInputStream& iStream )
			{
				iStream.Read( (CHAR*)(&bIsPet), sizeof(BYTE));
				iStream.Read( (CHAR*)(&nIndex), sizeof(BYTE));
				iStream.Read( (CHAR*)(&nSerial), sizeof(UINT));
				iStream.Read( (CHAR*)(&nPrice), sizeof(UINT));
				item.ReadItemVarAttr(iStream);
				iStream.Read( (CHAR*)(&PetGuid), sizeof(PET_GUID_t));
				return true;
			}
			bool Write( SocketOutputStream& oStream ) const
			{
				oStream.Write( (CHAR*)(&bIsPet), sizeof(BYTE));
				oStream.Write( (CHAR*)(&nIndex), sizeof(BYTE));
				oStream.Write( (CHAR*)(&nSerial), sizeof(UINT));
				oStream.Write( (CHAR*)(&nPrice), sizeof(UINT));
				item.WriteItemVarAttr(oStream);
				oStream.Write( (CHAR*)(&PetGuid), sizeof(PET_GUID_t));
				return true;
			}

			INT GetSize() const
			{
				return GetItemVarSize(item)+2*sizeof(BYTE)+2*sizeof(UINT)+sizeof(PET_GUID_t);
			}
		};

	public:
		GCStallOpen( )
		{
			m_nStallItemNum =	0;
			memset(m_ShopName, 0, MAX_STALL_NAME*sizeof(CHAR));
			memset(m_StallItemList, 0, STALL_BOX_SIZE*sizeof(_STALL_ITEM));
			m_nFirstPage	=	0;
			m_OwnerGuid = 0;
		}
		virtual ~GCStallOpen( ){};

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_STALLOPEN; }
		virtual UINT			GetPacketSize()const
		{
			UINT nSize = 0;
			for (INT i=0; i<m_nStallItemNum; ++i)
			{
				nSize = nSize + m_StallItemList[i].GetSize();
			}
			
			return	sizeof(BYTE) 
			+ nSize
			+ sizeof(CHAR)*MAX_STALL_NAME
			+ sizeof(BYTE) 
			+ sizeof(ObjID_t)
			+ sizeof(GUID_t);
		}

	public:
		BYTE					GetMerchadiseNum(VOID) const {return m_nStallItemNum;};
		VOID					SetMerchadiseNum(BYTE nNumber) {m_nStallItemNum = nNumber;};

		_STALL_ITEM*			GetMerchadiseList(VOID) { return m_StallItemList; }
		VOID					SetMerchadiseList(_STALL_ITEM* pSour)
		{
			memcpy(m_StallItemList, pSour, m_nStallItemNum*sizeof(_STALL_ITEM));
		}

		CHAR*					GetShopName(VOID){ return m_ShopName;}
		VOID					SetShopName(CHAR* ShopName, UINT ilength)
		{
			memset(m_ShopName, 0, MAX_STALL_NAME*sizeof(CHAR));
			memcpy(m_ShopName, ShopName, ilength);
		}

		BYTE					GetFirstPage(VOID) const {return m_nFirstPage;}
		VOID					SetFirstPage(BYTE FirstPage) {m_nFirstPage = FirstPage;}
		
		ObjID_t					GetObjID(VOID) const {return m_OwnerObjId;};
		VOID					SetObjID(ObjID_t ObjID) {m_OwnerObjId = ObjID;};

		GUID_t					GetGUID(VOID) const {return m_OwnerGuid;};
		VOID					SetGUID(GUID_t OwnerGuid) {m_OwnerGuid = OwnerGuid;};

	private:
		//��Ʒ����
		BYTE					m_nStallItemNum;
		//������Ʒ�б�
		_STALL_ITEM				m_StallItemList[STALL_BOX_SIZE];
		//�̵���
		CHAR					m_ShopName[MAX_STALL_NAME];
		//��ҳ
		BYTE					m_nFirstPage;
		//̯��id
		ObjID_t					m_OwnerObjId;
		//̯��GUID
		GUID_t					m_OwnerGuid;

	};

	class GCStallOpenFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCStallOpen() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_STALLOPEN; };
		UINT		GetPacketMaxSize()const { return sizeof(BYTE) 
													+ sizeof(GCStallOpen::_STALL_ITEM)*MAX_STALL_NAME
													+ sizeof(CHAR)*MAX_STALL_NAME
													+ sizeof(BYTE) 
													+ sizeof(ObjID_t)
													+ sizeof(GUID_t);}
	};

	class GCStallOpenHandler 
	{
	public:
		static UINT Execute( GCStallOpen* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
