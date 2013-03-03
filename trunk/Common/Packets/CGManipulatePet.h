// CGManipulatePet.h
// 
// 请求操作宠物返回结果
//
//////////////////////////////////////////////////////

#ifndef __CG_MANIPULATEPET_H__
#define __CG_MANIPULATEPET_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	enum ENUM_MANIPULATE_TYPE
	{
		MANIPULATE_INVALID = -1,			// 无效
		MANIPULATE_CREATEPET,				// 召唤宠物
		MANIPULATE_DELETEPET,				// 收回宠物
		MANIPULATE_FREEPET,					// 放生宠物
		MANIPULATE_ASKOTHERPETINFO,			// 察看其他玩家的宠物信息(宠物征友等用...)
		MANIPULATE_CREATEHORSE,				// 召唤坐骑
		MANIPULATE_DELETEHORSE,				// 收回坐骑
		MANIPULATE_FREEHORSE,				// 收回坐骑
		MANIPULATE_HORSE_ITEM,				// 使用物品
		MANIPULATE_HORSE_CHECKUP,			// 鉴定马，鉴定之前不给client传成长率，没有其它影响
		MANIPULATE_HORSE_NICKNAME,			// 修改昵称
		MANIPULATE_HORSE_OPEN_MATING_UI,	// 打开交配界面
		MANIPULATE_HORSE_MATING_LOCK,		// 交配坐骑锁定
		MANIPULATE_HORSE_MATING_UNLOCK,		// 交配坐骑解锁
		MANIPULATE_HORSE_MATING_CONFIRM,	// 坐骑交配确认
		MANIPULATE_HORSE_MATING_CANCEL,		// 坐骑交配取消
		MANIPULATE_HORSE_MATING_RETRIEVE,	// 马领回
		MANIPULATE_NUMBERS
	};

	class CGManipulatePet : public Packet
	{
	public:

		CGManipulatePet( )
		{
		}

		~CGManipulatePet( ){}

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_MANIPULATEPET ; }
		virtual UINT			GetPacketSize()const 
		{
				UINT uSize = sizeof(ObjID_t) + sizeof(PET_GUID_t) + sizeof(INT)*5 + sizeof(m_sNickName);
				return uSize;
		}

		
	public:
		VOID			SetObjID(const ObjID_t objID) { m_ObjID = objID; }
		ObjID_t			GetObjID(VOID) const { return m_ObjID; }

		VOID			SetGUID(PET_GUID_t guid){ m_PetGUID = guid; }
		PET_GUID_t		GetGUID(VOID)const{ return m_PetGUID; }

		// nType	:	ENUM_MANIPULATE_TYPE
		VOID			SetManipulateType(INT nType){ m_nType = nType; }
		INT				GetManipulateType(VOID)const{ return m_nType; }
		ObjID_t			GetCharObjID() const { return m_CharObjID; }
		VOID			SetCharObjID(ObjID_t val) { m_CharObjID = val; }
		INT				GetTypeData() const { return m_iTypeData; }
		VOID			SetTypeData(INT val) { m_iTypeData = val; }
		const CHAR*		GetNickName() const { return &m_sNickName[0]; }
		VOID			SetNickName(const CHAR* val) 
		{
			if( strlen(val) >= MAX_NICK_NAME-1 )
				strncpy( m_sNickName, val, sizeof(m_sNickName) );
			else
				strcpy( m_sNickName, val );
			m_sNickName[MAX_NICK_NAME-1] = 0;
		}
		INT				GetBagIndex() const { return m_iBagIndex; }
		VOID			SetBagIndex(INT val) { m_iBagIndex = val; }
		UINT			GetItemID() const { return m_uItemID; }
		VOID			SetItemID(UINT val) { m_uItemID = val; }
	private:
		ObjID_t			m_ObjID;					// 宠物的ObjID;
		ObjID_t			m_CharObjID;	
		PET_GUID_t		m_PetGUID;
		INT				m_nType;
		INT				m_iTypeData;				//由具体的操作类型解释其数值含义
		CHAR			m_sNickName[MAX_NICK_NAME];	//宠物昵称
		INT				m_iBagIndex;
		UINT			m_uItemID;

	};


	class CGManipulatePetFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGManipulatePet() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_MANIPULATEPET; }
		UINT		GetPacketMaxSize()const 
		{

			UINT uSize = sizeof(CGManipulatePet) - sizeof(Packet);
			return uSize;
		}
	};

	class CGManipulatePetHandler 
	{
	public:
		static UINT Execute( CGManipulatePet* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif // __GCPLAYERDIE_H__
