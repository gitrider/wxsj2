// GCManipulatePetRet.h
// 
// 请求操作宠物返回结果
//
//////////////////////////////////////////////////////

#ifndef __GC_MANIPULATEPETRET_H__
#define __GC_MANIPULATEPETRET_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCManipulatePetRet : public Packet
	{
	public:
		enum ENUM_MANIPULATEPET_RET
		{
			MANIPULATEPET_RET_INVALID = -1,
			MANIPULATEPET_RET_CAPTUREFALID,				// 捕捉失败
			MANIPULATEPET_RET_CAPTURESUCC,				// 捕捉成功
			MANIPULATEPET_RET_CALLUPFALID,				// 召唤失败
			MANIPULATEPET_RET_CALLUPSUCC,				// 召唤成功
			MANIPULATEPET_RET_FREEFALID,				// 放生失败
			MANIPULATEPET_RET_FREESUCC,					// 放生成功
			MANIPULATEPET_RET_RECALLFALID,				// 收回失败
			MANIPULATEPET_RET_RECALLSUCC,				// 收回成功
			MANIPULATEPET_RET_MATINGSUCCESS,			// 马交配成功
			MANIPULATEPET_RET_UNKNOWN,					// 未知错误
			MANIPULATEPET_RET_NO_TEAM,					// 没有组队
			MANIPULATEPET_RET_TEAM_MEM_COUNT,			// 队伍人数不是2人, 或两人不在同一场景中
			MANIPULATEPET_RET_TEAM_LEADER,				// 不是队长
			MANIPULATEPET_RET_AREA,						// 与NPC距离不对
			MANIPULATEPET_RET_HORSE_LOCK,				// 参与交配的马处于锁定状态,不能交配
			MANIPULATEPET_RET_HORSE_SEX_SAME,			// 参与交配的马性别相同
			MANIPULATEPET_RET_HORSE_HAPPINESS,			// 参与交配的马快乐度不足100
			MANIPULATEPET_RET_HORSE_GENERATION, 		// 参与交配的马不是一代马
			MANIPULATEPET_RET_HORSE_LEVEL,				// 参与交配的马等级差超过5
			MANIPULATEPET_RET_HORSE_SPOUSED,			// 参与交配的马有配偶且不是对方
			MANIPULATEPET_RET_HORSE_SCENE,				// 参与交配的马主人不在同一场景中
			MANIPULATEPET_RET_HORSE_LEVEL15,			// 参与交配的马等级必须大于15级
			MANIPULATEPET_RET_HORSE_CALLUP,				// 参与交配的马不能是出战状态
			MANIPULATEPET_RET_HORSE_VARIATION,			// 参与交配的马不能是变异
			MANIPULATEPET_RET_HORSE_MATINGLEVEL,		// 参与交配的马等级与上次交配相差不足20级
			MANIPULATEPET_RET_HORSE_MATING,				// 参与交配的马的主人有其它马在交配
			MANIPULATEPET_RET_HORSE_SPOUSE,				// 并没有骑乘在这里繁殖
			MANIPULATEPET_RET_HORSE_MATING_FINISH,		// 骑乘还未繁殖完毕
			MANIPULATEPET_RET_HORSE_NO_SOLT,			// 没有空格子了

			MANIPULATEPET_RET_HORSE_OPEN_MATING_UI_OK,	// 打开交配界面成功
			MANIPULATEPET_RET_HORSE_OPEN_MATING_UI_FAIL,// 打开交配界面失败

			MANIPULATEPET_RET_HORSE_MATING_LOCK_OK,		// 交配坐骑锁定成功
			MANIPULATEPET_RET_HORSE_MATING_LOCK_FAIL,	// 交配坐骑锁定失败
			MANIPULATEPET_RET_HORSE_OTHER_MATING_LOCK,	// 对方坐骑锁定

			MANIPULATEPET_RET_HORSE_MATING_UNLOCK,		// 交配坐骑解锁
			MANIPULATEPET_RET_HORSE_OTHER_MATING_UNLOCK,// 交配坐骑解锁

			MANIPULATEPET_RET_HORSE_MATING_CONFIRM,		// 坐骑交配确认
			MANIPULATEPET_RET_HORSE_MATING_CONFIRM_FAIL,// 坐骑交配确认
			MANIPULATEPET_RET_HORSE_MATING_CANCEL,		// 坐骑交配取消
			MANIPULATEPET_RET_HORSE_MATING_RETRIEVE		// 交配坐骑成功领回
		};

		GCManipulatePetRet( )
		{
			m_bFighting = FALSE;
			m_Ret = -1;
		}

		~GCManipulatePetRet( ){}

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_MANIPULATEPETRET ; }
		virtual UINT			GetPacketSize()const
		{ 
			return sizeof(PET_GUID_t) +
				sizeof(BOOL) +
				sizeof(INT) +
				sizeof(ObjID_t);
		}
		
	public:
		//使用数据接口
		PET_GUID_t			GetGUID(VOID) const { return m_GUID; }
		VOID				SetGUID(PET_GUID_t guid) { m_GUID = guid; }
		BOOL				IsFighting(VOID) const { return m_bFighting; }
		INT					GetManipulateRet(VOID) const { return (ENUM_MANIPULATEPET_RET)m_Ret; }
		VOID				SetManipulateRet(INT Ret) { m_Ret = Ret; }
		VOID				SetFightingFlag(BOOL bFlag) { m_bFighting = bFlag; }
		ObjID_t				GetObjID() const { return m_ObjID; }
		VOID				SetObjID(ObjID_t val) { m_ObjID = val; }

	private:
		PET_GUID_t		m_GUID;
		BOOL			m_bFighting;	// 是否处于参战状态
		INT				m_Ret;			// 返回结果
		ObjID_t			m_ObjID;		
	};


	class GCManipulatePetRetFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCManipulatePetRet() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_MANIPULATEPETRET; }
		UINT		GetPacketMaxSize()const 
		{ 
			return sizeof(PET_GUID_t) +
	     		   sizeof(BOOL) +
				   sizeof(INT) +
				   sizeof(ObjID_t);
		}
	};

	class GCManipulatePetRetHandler 
	{
	public:
		static UINT Execute( GCManipulatePetRet* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif // __GCPLAYERDIE_H__
