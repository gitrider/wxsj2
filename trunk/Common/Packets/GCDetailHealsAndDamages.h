#ifndef __GCDETAILHEALSANDDAMAGES_H__
#define __GCDETAILHEALSANDDAMAGES_H__


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	class GCDetailHealsAndDamages : public Packet
	{
	public:
		enum
		{
			IDX_HP_MODIFICATION = 0,
			IDX_MP_MODIFICATION,
			IDX_PNEUMA_MODIFICATION,
			IDX_RAGE_MODIFICATION,
			IDX_STRIKE_POINT_MODIFICATION,
			NUM_OF_FLAGS,
		};
		
		typedef BitFlagSet_T<NUM_OF_FLAGS> DirtyFlagSet_T;
		
		GCDetailHealsAndDamages(){
			Reset();
		}
		virtual ~GCDetailHealsAndDamages(){}

		VOID Reset(VOID){
			m_nReceiverID					= INVALID_ID;
			m_nSenderID						= INVALID_ID;
			m_nSenderLogicCount 			= 0;
			m_bIsCriticalHit				= FALSE;
			m_bBeatBack						= FALSE;
			m_bBeatFly						= FALSE;
			m_BeatBackPos.m_fX				= 0;
			m_BeatBackPos.m_fZ				= 0;
			m_CloseToPos					= m_BeatBackPos;
			m_fBeatFlyDistance				= 5.0f;
			m_fAttackerAngle				= 0.0f;
			m_bCloseTo						= FALSE;
			m_nHpModification				= 0;			
			m_nMpModification				= 0;
			m_nPneumaModification			= 0;
			m_nRageModification				= 0;		
			m_nStrikePointModification		= 0;
			m_DirtyFlags.ClearAllFlags();

		}

		//公用继承接口
		virtual BOOL			Read(SocketInputStream& iStream);
		virtual BOOL			Write(SocketOutputStream& oStream)const;
		virtual UINT			Execute(Player* pPlayer);

		virtual PacketID_t		GetPacketID()const {return PACKET_GC_DETAIL_HEALS_AND_DAMAGES;}
		virtual UINT			GetPacketSize()const 
								{
									INT nSize =  m_DirtyFlags.GetByteSize() + sizeof(m_nReceiverID) + sizeof(m_nSenderID) + sizeof(m_nSenderLogicCount);
									for(INT nIdx=IDX_HP_MODIFICATION; NUM_OF_FLAGS>nIdx; ++nIdx)
									{
										if(TRUE==m_DirtyFlags.GetFlagByIndex(nIdx))
										{
											nSize += sizeof(INT);
										}
									}
									nSize += sizeof(m_bIsCriticalHit);

									nSize += sizeof(m_bBeatBack);
									nSize += sizeof(m_BeatBackPos);
									nSize += sizeof(m_bBeatFly);
									nSize += sizeof(m_CloseToPos);
									nSize += sizeof(m_fBeatFlyDistance);
									nSize += sizeof(m_fAttackerAngle);
									nSize += sizeof(m_bCloseTo);
									return nSize;
								}

	public:
		//使用数据接口
		VOID			SetReceiverID(ObjID_t id) {m_nReceiverID = id;}
		ObjID_t			GetReceiverID(VOID)const {return m_nReceiverID;}

		VOID			SetSenderID(ObjID_t id) {m_nSenderID = id;}
		ObjID_t			GetSenderID(VOID)const {return m_nSenderID;}

		VOID			SetSenderLogicCount(INT nCount) {m_nSenderLogicCount=nCount;}
		INT				GetSenderLogicCount(VOID) const {return m_nSenderLogicCount;}
		
		VOID			SetHpModification(INT nValue) {m_nHpModification = nValue;m_DirtyFlags.MarkFlagByIndex(IDX_HP_MODIFICATION);}
		INT				GetHpModification(VOID) const {return m_nHpModification;}

		VOID			SetMpModification(INT nValue) {m_nMpModification = nValue;m_DirtyFlags.MarkFlagByIndex(IDX_MP_MODIFICATION);}
		INT 			GetMpModification(VOID) const {return m_nMpModification;}

		VOID			SetPneumaModification(INT nValue) {m_nPneumaModification = nValue;m_DirtyFlags.MarkFlagByIndex(IDX_PNEUMA_MODIFICATION);}
		INT 			GetPneumaModification(VOID) const {return m_nPneumaModification;}

		VOID			SetRageModification(INT nValue) {m_nRageModification = nValue;m_DirtyFlags.MarkFlagByIndex(IDX_RAGE_MODIFICATION);}
		INT 			GetRageModification(VOID) const {return m_nRageModification;}
		
		VOID			SetStrikePointModification(INT nValue) {m_nStrikePointModification = nValue;m_DirtyFlags.MarkFlagByIndex(IDX_STRIKE_POINT_MODIFICATION);}
		INT 			GetStrikePointModification(VOID) const {return m_nStrikePointModification;}

		VOID			SetCriticalHitFlag(BOOL bFlag) {m_bIsCriticalHit = bFlag;}
		BOOL 			IsCriticalHit(VOID) const {return m_bIsCriticalHit;}

		BOOL			IsHpModificationDirty(VOID) const {return m_DirtyFlags.GetFlagByIndex(IDX_HP_MODIFICATION);};
		BOOL			IsMpModificationDirty(VOID) const {return m_DirtyFlags.GetFlagByIndex(IDX_MP_MODIFICATION);};
		BOOL			IsPneumaModificationDirty(VOID) const {return m_DirtyFlags.GetFlagByIndex(IDX_PNEUMA_MODIFICATION);};
		BOOL			IsRageModificationDirty(VOID) const {return m_DirtyFlags.GetFlagByIndex(IDX_RAGE_MODIFICATION);};
		BOOL			IsStrikePointModificationDirty(VOID) const {return m_DirtyFlags.GetFlagByIndex(IDX_STRIKE_POINT_MODIFICATION);};
		VOID			SetBeatBack( BOOL bBeatBack ) { m_bBeatBack = bBeatBack; }
		BOOL			IsBeatBack() const { return m_bBeatBack; }
		VOID			SetBeatFly( BOOL bBeatFly ) { m_bBeatFly = bBeatFly; }
		BOOL			IsBeatFly() const { return m_bBeatFly; }
		const WORLD_POS& GetBeatBackPos() { return m_BeatBackPos; }
		VOID			 SetBeatBackPos( const WORLD_POS& sPos ) { m_BeatBackPos = sPos; }
		const WORLD_POS& GetCloseToPos() { return m_CloseToPos; }
		VOID			 SetCloseToPos( const WORLD_POS& sPos ) { m_CloseToPos = sPos; }
		FLOAT			 GetBeatFlyDistance() { return m_fBeatFlyDistance; }
		VOID			 SetBeatFlyDistance( FLOAT fDistance ) { m_fBeatFlyDistance = fDistance; }
		FLOAT			 GetAttackerAngle() { return m_fAttackerAngle; }
		VOID			 SetAttackerAngle( FLOAT fAngle ) { m_fAttackerAngle = fAngle; }
		VOID			 SetCloseToEnable( BOOL bEnable ) { m_bCloseTo = bEnable; }
		BOOL			 IsCloseTo() const { return m_bCloseTo; }

	private:
		ObjID_t			m_nReceiverID;				// 效果接受者的ID
		ObjID_t			m_nSenderID;				// 效果发送者的ID
		INT				m_nSenderLogicCount;		//效果创建者的逻辑计数
		INT				m_nHpModification;			//生命变化量
		INT				m_nMpModification;			//内力变化量
		INT				m_nPneumaModification;		//活力变化量
		INT 			m_nRageModification;		//怒气变化量
		INT				m_nStrikePointModification; //连击点变化量
		BOOL			m_bIsCriticalHit;			//是否是会心一击
		DirtyFlagSet_T 	m_DirtyFlags;				//有哪些域有数据的脏标记
		BOOL			m_bBeatBack;				//是否被击退
		BOOL			m_bBeatFly;					//是否被击飞
		WORLD_POS		m_BeatBackPos;				//被击退到的位置
		WORLD_POS		m_CloseToPos;				//击打者跟进被击退者到达的位置
		FLOAT			m_fBeatFlyDistance;			//击飞距离
		FLOAT			m_fAttackerAngle;			//跟进时击打者的朝向
		BOOL			m_bCloseTo;
	};

	class GCDetailHealsAndDamagesFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() {return new GCDetailHealsAndDamages();}
		PacketID_t	GetPacketID()const {return PACKET_GC_DETAIL_HEALS_AND_DAMAGES;}
		UINT		GetPacketMaxSize()const 
					{
						return 	sizeof(ObjID_t)
							+	sizeof(ObjID_t)
							+	sizeof(INT)*5
							+	sizeof(BOOL)
							+	GCDetailHealsAndDamages::DirtyFlagSet_T::BYTE_SIZE
							+	sizeof(BOOL)
							+	sizeof(BOOL)
							+	sizeof(WORLD_POS)
							+	sizeof(WORLD_POS)
							+	sizeof(FLOAT)
							+	sizeof(FLOAT)
							+	sizeof(BOOL);
					}
	};

	class GCDetailHealsAndDamagesHandler 
	{
	public:
		static UINT Execute(GCDetailHealsAndDamages* pPacket, Player* pPlayer);
	};
}

using namespace Packets;



#endif //__GCDETAILHEALSANDDAMAGES_H__
