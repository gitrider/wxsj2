#ifndef _WGDetailOtherOnlineAttrib_Human_H_
#define _WGDetailOtherOnlineAttrib_Human_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{

	class WGDetailOtherOnlineAttrib_Human :
		public Packet
	{
	public:
		WGDetailOtherOnlineAttrib_Human( )
		{
			m_Flags.CleanUp();
			m_TargetID = 0;
		}
		virtual ~WGDetailOtherOnlineAttrib_Human( ){}

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_WG_DETAILOTHERONLINEATTRIB_HUMAN ; }
		virtual UINT			GetPacketSize()const 
		{
			UINT uSize = sizeof(ObjID_t) + sizeof(Flag128) ;
			UINT i;
			for ( i = 0; i < DETAIL_ATTRIB_NUMBERS; i++ )
			{
				if(m_Flags.isSetBit(i))
				{
					switch ( i )
					{
					case DETAIL_ATTRIB_S_SKILLPOINT:
						uSize += COMMON_S_NUMBER*sizeof(INT);
						break;
					case DETAIL_ATTRIB_CAMP:
						uSize += sizeof( m_CampData );
						break;
					case DETAIL_ATTRIB_CURRENT_PET_GUID:
						uSize += sizeof( PET_GUID_t );
						break;
					case DETAIL_ATTRIB_FAMILY:
						uSize += sizeof(CHAR);
						break;
					case DETAIL_ATTRIB_GUILD_OFFICIAL:
						uSize += sizeof(CHAR);
						break;
					default:
						uSize += sizeof( UINT );	
						break;
					}
				}
			}
			//每次都发送目标对象的ID
			uSize += sizeof( ObjID_t );
			return uSize;
		}

	public:
		VOID			SetPlayerID(ObjID_t id) { m_ObjID = id; }
		ObjID_t			GetPlayerID(VOID)const { return m_ObjID; }

		Flag128			GetFlags( VOID )const{ return m_Flags; }

		BOOL			IsUpdateAttrib( ENUM_DETAIL_ATTRIB eAttrib )const{ return m_Flags.isSetBit((INT)eAttrib)?(TRUE):(FALSE); }
		VOID			SetUpdateAttrib( ENUM_DETAIL_ATTRIB eAttrib, BOOL bUpdate ){ m_Flags.UpdateBits((INT)eAttrib,bUpdate); }

		UINT			GetLevel( VOID )const{ return m_uLevel; }
		VOID			SetLevel( UINT uLevel ){ m_uLevel =uLevel; SetUpdateAttrib( DETAIL_ATTRIB_LEVEL, TRUE ); }

		INT				GetCountry(VOID)	const	{return m_nCountry;}
		VOID			SetCountry(INT	Country){ m_nCountry	=	Country; SetUpdateAttrib(DETAIL_ATTRIB_COUNTRY,TRUE); }

		INT				GetMenpai(VOID)	const	{return m_nMenPai;}
		VOID			SetMenpai(INT	Menpai){ m_nMenPai	=	Menpai; SetUpdateAttrib(DETAIL_ATTRIB_MEMPAI,TRUE); }

		INT				GetGuild(VOID)	const	{return m_nGuild;}
		VOID			SetGuild(INT	nGuild){ m_nGuild	=	nGuild; SetUpdateAttrib(DETAIL_ATTRIB_GUILD,TRUE); }

		INT				GetGuildPoint(VOID)	const	{return m_nGP;}
		VOID			SetGuildPoint(INT	nGP){ m_nGP	=	nGP; SetUpdateAttrib(DETAIL_ATTRIB_GUILD_POINT,TRUE); }

		INT				GetPKValue(VOID)	const	{return m_nPK;}
		VOID			SetPKValue(INT	nPK){ m_nPK	=	nPK; SetUpdateAttrib(DETAIL_ATTRIB_PK_VALUE,TRUE); }

		INT				GetHonor(VOID)	const	{return m_nHonor;}
		VOID			SetHonor(INT	nHonor){ m_nHonor	=	nHonor; SetUpdateAttrib(DETAIL_ATTRIB_HONOR,TRUE); }

		CHAR    		GetFamilyID(VOID)	const	{return m_cFamilyID;}
		VOID			SetFamilyID(CHAR	cFamilyID){ m_cFamilyID	=	cFamilyID; SetUpdateAttrib(DETAIL_ATTRIB_FAMILY,TRUE); }

		CHAR			GetGuildOfficial(VOID)	const	{return m_cGuildOfficial;}
		VOID			SetGuildOfficial(CHAR	cOfficial){ m_cGuildOfficial	=	cOfficial; SetUpdateAttrib(DETAIL_ATTRIB_GUILD_OFFICIAL,TRUE); }

		const _CAMP_DATA		*GetCampData(VOID)const{ return &m_CampData; }
		VOID					SetCampData( const _CAMP_DATA *pCampData ) { m_CampData = *pCampData; }


		INT				GetDataID(VOID)	const	{return m_nDataID;}
		VOID			SetDataID(INT	nDataID)		{m_nDataID = nDataID; SetUpdateAttrib(DETAIL_ATTRIB_DATAID,TRUE);}

		INT				GetPortraitID(VOID)	const	{return m_nPortraitID;}
		VOID			SetPortraitID(INT	nPortraitID)		{m_nPortraitID = nPortraitID; SetUpdateAttrib(DETAIL_ATTRIB_PORTRAITID,TRUE);}

		INT				GetModelID(VOID)	const	{return m_nModelID;}
		VOID			SetModelID(INT	nModelID)		{m_nModelID = nModelID; SetUpdateAttrib(DETAIL_ATTRIB_MODELID,TRUE);}

		INT				GetFaceModel(VOID)	const	{return m_nFaceModel;}
		VOID			SetFaceModel(INT	nFaceModel)		{m_nFaceModel = nFaceModel; SetUpdateAttrib(DETAIL_ATTRIB_FACEMODEL,TRUE);}

		INT				GetMountID(VOID)	const	{return m_nMountID;}
		VOID			SetMountID(INT	nID)		{m_nMountID = nID; SetUpdateAttrib(DETAIL_ATTRIB_MOUNTID,TRUE);}

		PET_GUID_t		GetCurrentPetGUID( VOID )const{ return m_guidCurrentPet; }
		VOID			SetCurrentPetGUID( PET_GUID_t guid ){ m_guidCurrentPet =guid; SetUpdateAttrib( DETAIL_ATTRIB_CURRENT_PET_GUID, TRUE ); }

		INT				GetExp( VOID )const{ return m_nExp; }
		VOID			SetExp( INT nExp ){ m_nExp = nExp; SetUpdateAttrib( DETAIL_ATTRIB_EXP, TRUE ); }

		INT				GetHP(VOID) const{return m_nHP;}
		VOID			SetHP(INT nHp){m_nHP	= nHp;SetUpdateAttrib(DETAIL_ATTRIB_HP,TRUE);}

		INT				GetMP(VOID) const {return m_nMP;}
		VOID			SetMP(INT nMp){m_nMP = nMp; SetUpdateAttrib(DETAIL_ATTRIB_MP,TRUE);}

		INT				GetPneuma(VOID) const {return m_nPneuma;}
		VOID			SetPneuma(INT nPneuma){m_nPneuma = nPneuma; SetUpdateAttrib(DETAIL_ATTRIB_PNEUMA,TRUE);}

		INT				GetMAXHP(VOID) const {return m_nMAXHP;}
		VOID			SetMAXHP(INT nMaxHp) {m_nMAXHP = nMaxHp;SetUpdateAttrib(DETAIL_ATTRIB_MAXHP,TRUE);}

		INT				GetMAXMP(VOID) const {return m_nMAXMP;}
		VOID			SetMAXMP(INT nMaxMp) {m_nMAXMP = nMaxMp;SetUpdateAttrib(DETAIL_ATTRIB_MAXMP,TRUE);}

		INT				GetMAXPneuma(VOID) const {return m_nMaxPneuma;}
		VOID			SetMAXPneuma(INT nMaxPneuma) {m_nMaxPneuma = nMaxPneuma;SetUpdateAttrib(DETAIL_ATTRIB_MAXPNEUMA,TRUE);}

		FLOAT			GetMoveSpeed(VOID) const {return m_fMoveSpeed;}
		VOID			SetMoveSpeed(FLOAT fMoveSpeed){m_fMoveSpeed = fMoveSpeed;SetUpdateAttrib(DETAIL_ATTRIB_MOVESPEED,TRUE);}

		UINT			GetMoney( VOID )const{ return m_uMoney; }
		VOID			SetMoney( UINT uMoney ){ m_uMoney =uMoney; SetUpdateAttrib( DETAIL_ATTRIB_MONEY, TRUE ); }

		INT				GetStr() const {return m_Str;}			
		VOID			SetStr(INT str){m_Str	=	str;SetUpdateAttrib(DETAIL_ATTRIB_STR,TRUE);}

		/*INT				GetSpr() const{return m_Spr;}			
		VOID			SetSpr(INT spr){m_Spr	=	spr;SetUpdateAttrib(DETAIL_ATTRIB_SPR,TRUE);}*/


		INT				GetCon() const {return m_Con;}			
		VOID			SetCon(INT con){m_Con	=	con;SetUpdateAttrib(DETAIL_ATTRIB_CON,TRUE);}

		INT				GetInt()const {return m_Int;}			
		VOID			SetInt(INT iInt){m_Int	=	iInt;SetUpdateAttrib(DETAIL_ATTRIB_INT,TRUE);}

		INT				GetDex() const {return m_Dex;}			
		VOID			SetDex(INT dex){m_Dex	=	dex;SetUpdateAttrib(DETAIL_ATTRIB_DEX,TRUE);}


		INT				GetPoint_Remain( VOID )const{ return m_nPoint_Remain; }
		VOID			SetPoint_Remain( INT nPoint_Remain ){ m_nPoint_Remain = nPoint_Remain; SetUpdateAttrib( DETAIL_ATTRIB_POINT_REMAIN, TRUE ); }

		INT				GetSkillPoint_Remain( VOID )const{ return m_nSkillPoint_Remain; }
		VOID			SetSkillPoint_Remain( INT nSkillPoint_Remain ){ m_nSkillPoint_Remain = nSkillPoint_Remain; SetUpdateAttrib( DETAIL_ATTRIB_SKILLPOINT_REMAIN, TRUE ); }

		INT				GetS_SkillPoint( INT nS )const{ Assert(nS<COMMON_S_NUMBER); return m_nS_SkillPoints[nS]; }
		VOID			SetS_SkillPoint( INT nSkillPoint, INT nS ){ Assert(nS<COMMON_S_NUMBER); m_nS_SkillPoints[nS] = nSkillPoint; SetUpdateAttrib( DETAIL_ATTRIB_S_SKILLPOINT, TRUE ); }

		INT				GetHP_ReSpeed( VOID )const{ return m_nHP_ReSpeed; }
		VOID			SetHP_ReSpeed( INT nHP_ReSpeed ){ m_nHP_ReSpeed = nHP_ReSpeed; SetUpdateAttrib( DETAIL_ATTRIB_HP_RESPEED, TRUE ); }

		INT				GetMP_ReSpeed( VOID )const{ return m_nMP_ReSpeed; }
		VOID			SetMP_ReSpeed( INT nMP_ReSpeed ){ m_nMP_ReSpeed = nMP_ReSpeed; SetUpdateAttrib( DETAIL_ATTRIB_MP_RESPEED, TRUE ); }

		INT				GetVigor_ReSpeed( VOID )const{ return m_nVigor_ReSpeed; }
		VOID			SetVigor_ReSpeed( INT nVigor_ReSpeed ){ m_nVigor_ReSpeed = nVigor_ReSpeed; SetUpdateAttrib( DETAIL_ATTRIB_VIGOR_RESPEED, TRUE ); }


		//INT				GetAtt_Physics( VOID )const{ return m_nAtt_Physics; }
		//VOID			SetAtt_Physics( INT nAtt_Physics ){ m_nAtt_Physics = nAtt_Physics; SetUpdateAttrib( DETAIL_ATTRIB_ATT_PHYSICS, TRUE ); }
		//
		INT				GetAtt_Near( VOID )const{ return m_nAtt_Near; }
		VOID			SetAtt_Near( INT nAtt_Near ){ m_nAtt_Near = nAtt_Near; SetUpdateAttrib( DETAIL_ATTRIB_ATT_NEAR, TRUE ); }

		INT				GetAtt_Far( VOID )const{ return m_nAtt_Far; }
		VOID			SetAtt_Far( INT nAtt_Far ){ m_nAtt_Far = nAtt_Far; SetUpdateAttrib( DETAIL_ATTRIB_ATT_FAR, TRUE ); }

		INT				GetDef_Near( VOID )const{ return m_nDef_Near; }
		VOID			SetDef_Near( INT nDef_Near ){ m_nDef_Near = nDef_Near; SetUpdateAttrib( DETAIL_ATTRIB_DEF_NEAR, TRUE ); }

		INT				GetDef_Far( VOID )const{ return m_nDef_Far; }
		VOID			SetDef_Far( INT nDef_Far ){ m_nDef_Far = nDef_Far; SetUpdateAttrib( DETAIL_ATTRIB_DEF_FAR, TRUE ); }

		INT				GetAtt_MagicNear( VOID )const{ return m_nAtt_MagicNear; }
		INT				GetAtt_MagicFar( VOID )const{ return m_nAtt_MagicFar; }

		VOID			SetAtt_MagicNear( INT nAtt_MagicNear ){ m_nAtt_MagicNear = nAtt_MagicNear; SetUpdateAttrib( DETAIL_ATTRIB_ATT_MAGIC_NEAR, TRUE ); }
		VOID			SetAtt_MagicFar( INT nAtt_MagicFar ){ m_nAtt_MagicFar = nAtt_MagicFar; SetUpdateAttrib( DETAIL_ATTRIB_ATT_MAGIC_FAR, TRUE ); }

		/*INT				GetDef_Physics( VOID )const{ return m_nDef_Physics; }
		VOID			SetDef_Physics( INT nDef_Physics ){ m_nDef_Physics = nDef_Physics; SetUpdateAttrib( DETAIL_ATTRIB_DEF_PHYSICS, TRUE ); }*/

		INT				GetDef_MagicNear( VOID )const{ return m_nDef_MagicNear; }
		INT				GetDef_MagicFar( VOID )const{ return m_nDef_MagicFar; }

		VOID			SetDef_MagicNear( INT nDef_MagicNear ){ m_nDef_MagicNear = nDef_MagicNear; SetUpdateAttrib( DETAIL_ATTRIB_DEF_MAGIC_NEAR, TRUE ); }
		VOID			SetDef_MagicFar( INT nDef_MagicFar ){ m_nDef_MagicFar = nDef_MagicFar; SetUpdateAttrib( DETAIL_ATTRIB_DEF_MAGIC_FAR, TRUE ); }

		INT				GetToughness( VOID )const{ return m_nToughness; }
		VOID			SetToughness( INT nToughness ){ m_nToughness = nToughness; SetUpdateAttrib( DETAIL_ATTRIB_TOUGHNESS, TRUE ); }

		INT				GetHit( VOID )const{ return m_nHit; }
		VOID			SetHit( INT nHit ){ m_nHit = nHit; SetUpdateAttrib( DETAIL_ATTRIB_HIT, TRUE ); }

		INT				GetMiss(VOID) const {return m_nMiss;}
		VOID			SetMiss(INT nMiss){m_nMiss	=	 nMiss;SetUpdateAttrib(DETAIL_ATTRIB_MISS,TRUE);}

		INT				GetCritic( VOID )const{ return m_nCritic; }
		VOID			SetCritic( INT Critic ){ m_nCritic = Critic; SetUpdateAttrib( DETAIL_ATTRIB_CRITRATE, TRUE ); }

		INT				GetCriticHurt( VOID )const{ return m_nCriticHurt; }
		VOID			SetCriticHurt( INT Critic ){ m_nCriticHurt = Critic; SetUpdateAttrib( DETAIL_ATTRIB_CRITICALHURT, TRUE ); }

		INT				GetMaxRage( VOID )const{ return m_nMaxRage; }
		VOID			SetMaxRage( INT Rage ){ m_nMaxRage = Rage; SetUpdateAttrib( DETAIL_ATTRIB_MAXRAGE, TRUE ); }

		INT				GetRage( VOID )const{ return m_nRage; }
		VOID			SetRage( INT Rage ){ m_nRage = Rage; SetUpdateAttrib( DETAIL_ATTRIB_RAGE, TRUE ); }

		INT				GetRage_ReSpeed( VOID )const{ return m_nRage_ReSpeed; }
		VOID			SetRage_ReSpeed( INT nRage_ReSpeed ){ m_nRage_ReSpeed = nRage_ReSpeed; SetUpdateAttrib( DETAIL_ATTRIB_RAGE_RESPEED, TRUE ); }


		INT				GetStrikePoint( VOID )const{ return m_nStrikePoint; }
		VOID			SetStrikePoint( INT StrikePoint ){ m_nStrikePoint = StrikePoint; SetUpdateAttrib( DETAIL_ATTRIB_STRIKE_POINT, TRUE ); }

		INT				GetAttSpeed(VOID) const {return m_nAttackSpeed;}
		VOID			SetAttSpeed(INT AttSpeed){m_nAttackSpeed = AttSpeed;SetUpdateAttrib(DETAIL_ATTRIB_ATTACKSPEED,TRUE);}

		INT				GetAttGold(VOID)	const {return m_nAttGold;}
		VOID			SetAttGold(INT AttGold){ m_nAttGold = AttGold;SetUpdateAttrib(DETAIL_ATTRIB_ATTACK_GOLD,TRUE);}

		INT				GetAttWood(VOID)	const {return m_nAttWood;}
		VOID			SetAttWood(INT AttWood){m_nAttWood = AttWood; SetUpdateAttrib(DETAIL_ATTRIB_ATTACK_WOOD,TRUE);}

		INT				GetAttWater(VOID)	const {return m_nAttWater;}
		VOID			SetAttWater(INT AttWater){m_nAttWater = AttWater; SetUpdateAttrib(DETAIL_ATTRIB_ATTACK_WATER,TRUE);}

		INT				GetAttFire(VOID) const {return m_nAttFire;}
		VOID			SetAttFire(INT AttFire){m_nAttFire = AttFire;SetUpdateAttrib(DETAIL_ATTRIB_ATTACK_FIRE,TRUE);}

		INT				GetAttSoil(VOID)	const {return m_nAttSoil;}
		VOID			SetAttSoil(INT AttSoil){m_nAttSoil = AttSoil; SetUpdateAttrib(DETAIL_ATTRIB_ATTACK_SOIL,TRUE);}

		INT				GetDefGold(VOID)	const {return m_nDefGold;}
		VOID			SetDefGold(INT DefGold){ m_nDefGold = DefGold;SetUpdateAttrib(DETAIL_ATTRIB_DEFENCE_GOLD,TRUE);}

		INT				GetDefWood(VOID)	const {return m_nDefWood;}
		VOID			SetDefWood(INT DefWood){m_nDefWood = DefWood; SetUpdateAttrib(DETAIL_ATTRIB_DEFENCE_WOOD,TRUE);}

		INT				GetDefWater(VOID)	const {return m_nDefWater;}
		VOID			SetDefWater(INT DefWater){m_nDefWater = DefWater; SetUpdateAttrib(DETAIL_ATTRIB_DEFENCE_WATER,TRUE);}

		INT				GetDefFire(VOID) const {return m_nDefFire;}
		VOID			SetDefFire(INT DefFire){m_nDefFire = DefFire;SetUpdateAttrib(DETAIL_ATTRIB_DEFENCE_FIRE,TRUE);}

		INT				GetDefSoil(VOID)	const {return m_nDefSoil;}
		VOID			SetDefSoil(INT DefSoil){m_nDefSoil = DefSoil; SetUpdateAttrib(DETAIL_ATTRIB_DEFENCE_SOIL,TRUE);}

		INT				GetLimitMove(VOID)	const {return m_bLimitMove;}
		VOID			SetLimitMove(BOOL bLimitMove){m_bLimitMove	= bLimitMove; SetUpdateAttrib(DETAIL_ATTRIB_LIMIT_MOVE,TRUE);}

		INT				GetNearReduce(VOID)	const {return m_nNearReduce;}
		VOID			SetNearReduce(INT nNearReduce){m_nNearReduce	= nNearReduce; SetUpdateAttrib(DETAIL_ATTRIB_NEARATT_REDUCE,TRUE);}

		INT				GetFarReduce(VOID)	const {return m_nFarReduce;}
		VOID			SetFarReduce(INT nFarReduce){m_nFarReduce	= nFarReduce; SetUpdateAttrib(DETAIL_ATTRIB_FARATT_REDUCE,TRUE);}

		INT				GetMagicNearReduce(VOID)	const {return m_nMagicNearReduce;}
		INT				GetMagicFarReduce(VOID)	const {return m_nMagicFarReduce;}

		VOID			SetMagicNearReduce(INT nMagicNearReduce){m_nMagicNearReduce	= nMagicNearReduce; SetUpdateAttrib(DETAIL_ATTRIB_MAGIC_NEAR_ATT_REDUCE,TRUE);}
		VOID			SetMagicFarReduce(INT nMagicFarReduce){m_nMagicFarReduce	= nMagicFarReduce; SetUpdateAttrib(DETAIL_ATTRIB_MAGIC_FAR_ATT_REDUCE,TRUE);}

		INT				GetDreadReduce(VOID)	const {return m_nDreadReduce;}
		VOID			SetDreadReduce(INT nDreadReduce){m_nDreadReduce	= nDreadReduce; SetUpdateAttrib(DETAIL_ATTRIB_DREAD_RESIST,TRUE);}

		INT				GetComaReduce(VOID)	const {return m_nComaReduce;}
		VOID			SetComaReduce(INT nComaReduce){m_nComaReduce	= nComaReduce; SetUpdateAttrib(DETAIL_ATTRIB_COMA_RESIST,TRUE);}

		INT				GetHushReduce(VOID)	const {return m_nHushReduce;}
		VOID			SetHushReduce(INT nHushReduce){m_nHushReduce	= nHushReduce; SetUpdateAttrib(DETAIL_ATTRIB_HUSH_RESIST,TRUE);}

		INT				GetUnarmReduce(VOID)	const {return m_nUnarmReduce;}
		VOID			SetUnarmReduce(INT nUnarmReduce){m_nUnarmReduce	= nUnarmReduce; SetUpdateAttrib(DETAIL_ATTRIB_UNARM_RESIST,TRUE);}

		INT				GetAttSpeedResist(VOID)	const {return m_nAttSpeedResist;}
		VOID			SetAttSpeedResist(INT nAttSpeedResist){m_nAttSpeedResist	= nAttSpeedResist; SetUpdateAttrib(DETAIL_ATTRIB_ATTACKSPEED_RESIST,TRUE);}

		INT				GetSkillSpeedResist(VOID)	const {return m_nSkillSpeedResist;}
		VOID			SetSkillSpeedResist(INT nSkillSpeedResist){m_nSkillSpeedResist	= nSkillSpeedResist; SetUpdateAttrib(DETAIL_ATTRIB_SKILLSPEED_RESIST,TRUE);}

		INT				GetMoveSpeedResist(VOID)	const {return m_nMoveSpeedResist;}
		VOID			SetMoveSpeedResist(INT nMoveSpeedResist){m_nMoveSpeedResist	= nMoveSpeedResist; SetUpdateAttrib(DETAIL_ATTRIB_MOVEREDUCE_RESIST,TRUE);}


		INT				GetCanActionFlag1(VOID)	const {return m_bCanActionFlag1;}
		VOID			SetCanActionFlag1(BOOL bFlag){m_bCanActionFlag1	= bFlag; SetUpdateAttrib(DETAIL_ATTRIB_CAN_ACTION1,TRUE);}

		INT				GetCanActionFlag2(VOID)	const {return m_bCanActionFlag2;}
		VOID			SetCanActionFlag2(BOOL bFlag){m_bCanActionFlag2	= bFlag; SetUpdateAttrib(DETAIL_ATTRIB_CAN_ACTION2,TRUE);}

		INT				GetVigor(VOID)	const {return m_Vigor;}
		VOID			SetVigor(INT nVigor){m_Vigor	= nVigor; SetUpdateAttrib(DETAIL_ATTRIB_VIGOR,TRUE);}

		INT				GetMaxVigor(VOID)	const {return m_MaxVigor;}
		VOID			SetMaxVigor(INT nMaxVigor){m_MaxVigor	= nMaxVigor; SetUpdateAttrib(DETAIL_ATTRIB_MAX_VIGOR,TRUE);}

		INT				GetEnergy(VOID)	const {return m_Energy;}
		VOID			SetEnergy(INT nEnergy){m_Energy	= nEnergy; SetUpdateAttrib(DETAIL_ATTRIB_ENERGY,TRUE);}

		INT				GetMaxEnergy(VOID)	const {return m_MaxEnergy;}
		VOID			SetMaxEnergy(INT nMaxEnergy){m_MaxEnergy	= nMaxEnergy; SetUpdateAttrib(DETAIL_ATTRIB_MAX_ENERGY,TRUE);}

		INT				GetGoodBadValue( ) const { return m_nGoodBadValue; }
		VOID			SetGoodBadValue( INT nValue ) { m_nGoodBadValue = nValue; SetUpdateAttrib(DETAIL_ATTRIB_GOODBADVALUE, TRUE); }

		ObjID_t			GetTargetID() const { return m_TargetID; }
		VOID			SetTargetID(ObjID_t objID);
	private:
		ObjID_t			m_ObjID;	// 所有Obj类型的ObjID

		Flag128			m_Flags;		// 每个位表示一个属性是否要刷新 ENUM_DETAIL_ATTRIB


		UINT			m_uLevel;		// 等级
		INT				m_nExp;			// 经验值
		INT				m_nHP;			//生命点
		INT				m_nMP;			//魔法点
		INT				m_nPneuma;		//元气
		INT				m_nMAXHP;		//最大生命点
		INT				m_nMAXMP;		//最大魔法点
		INT				m_nMaxPneuma;	//最大元气值
		FLOAT			m_fMoveSpeed;	//移动速度


		UINT			m_uMoney;		// 游戏币数

		//一级战斗属性


		INT				m_Str;					//力量 力量
		//INT				m_Spr;					//灵气 灵力
		INT				m_Con;					//体制 体制
		INT 			m_Int;					//定力 智力
		INT 			m_Dex;					//身法 敏捷
		INT				m_nPoint_Remain;		//剩余待分配点数
		INT				m_nSkillPoint_Remain;   //剩余待分配技能点
		INT				m_nS_SkillPoints[COMMON_S_NUMBER]; //已分配的点

		//二级战斗属性
		INT				m_nHP_ReSpeed;		//HP恢复速度  点/秒
		INT				m_nMP_ReSpeed;		//MP恢复速度  点/秒
		INT				m_nVigor_ReSpeed;	//活力恢复速度  点/秒
		//INT				m_nAtt_Physics;		//物理攻击力

		INT				m_nAtt_Near;        //近程攻击
		INT             m_nAtt_Far;         //远程攻击

		INT				m_nAtt_MagicNear;	//近程内功攻击力
		INT				m_nAtt_MagicFar;	//近程外功攻击力
		

		INT				m_nDef_Near;        //近程防御
		INT             m_nDef_Far;         //远程防御

		INT				m_nDef_MagicNear;	//近程内功防御力
		INT				m_nDef_MagicFar;	//近程外功防御力

		INT             m_nToughness;       //韧性
		INT				m_nHit;				//命中率
		INT				m_nMiss;			//闪避率
		INT				m_nCritic;			//致命一击率
		INT				m_nCriticHurt;		//暴击伤害

		INT				m_nMaxRage;			//怒气上限
		INT				m_nRage;			//怒气
		INT				m_nRage_ReSpeed;		//怒气恢复速度  点/秒
		INT				m_nStrikePoint;		//连技点

		INT				m_nAttackSpeed;		//攻击速度

		INT				m_nAttGold;			//金攻击
		INT				m_nDefGold;			//金防御

		INT				m_nAttWood;			//木攻击
		INT				m_nDefWood;			//木防御

		INT				m_nAttWater;		//水攻击
		INT				m_nDefWater;		//水防御

		INT				m_nAttFire;			//火攻击
		INT				m_nDefFire;			//火防御

		INT				m_nAttSoil;			//土攻击
		INT				m_nDefSoil;			//土防御

		INT				m_nNearReduce;		//近程减免
		INT				m_nFarReduce;		//远程减免

		INT				m_nMagicNearReduce;	//近程内功减免
		INT				m_nMagicFarReduce;	//近程外功减免

		INT				m_nDreadReduce;		//抵抗恐惧
		INT				m_nComaReduce;		//抵抗昏迷
		INT				m_nHushReduce;		//抵抗沉默
		INT				m_nUnarmReduce;		//抵抗缴械
		INT				m_nAttSpeedResist;	//抗攻击减速
		INT				m_nSkillSpeedResist;//抗施法
		INT				m_nMoveSpeedResist;	//抗减速

		INT             m_nCountry;         //国家
		INT				m_nMenPai;			//门派
		INT				m_nGuild;			//帮派
		INT				m_nGP;	    		//帮派贡献度
		INT             m_nHonor;           //荣誉
		INT             m_nPK;              //杀气
		CHAR            m_cFamilyID;        //帮会家族ID
		CHAR            m_cGuildOfficial;   //帮会官职

		_CAMP_DATA		m_CampData;			// 阵营	
		INT				m_nDataID;			// DataID
		INT				m_nPortraitID;		// 头像ID
		INT				m_nModelID;			// 外形
		INT				m_nFaceModel;       //脸型
		INT				m_nMountID;			//座骑
		PET_GUID_t		m_guidCurrentPet;	//当前宠物

		BOOL			m_bLimitMove;		//是否限制不能移动
		BOOL			m_bCanActionFlag1;		//技能受限标记1,用于昏迷催眠
		BOOL			m_bCanActionFlag2;		//技能受限标记2,用于沉默

		INT				m_Vigor ;			// 活力
		INT				m_MaxVigor ;		// 活力上限
		INT				m_Energy ;			// 精力
		INT				m_MaxEnergy ;		// 精力上限

		INT				m_nGoodBadValue;	// 善恶值
		ObjID_t			m_TargetID;			//被攻击目标的ID
	};

	class WGDetailOtherOnlineAttrib_HumanFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new WGDetailOtherOnlineAttrib_Human() ; }
		PacketID_t	GetPacketID()const { return PACKET_WG_DETAILOTHERONLINEATTRIB_HUMAN; }
		UINT		GetPacketMaxSize()const { return	sizeof(WGDetailOtherOnlineAttrib_Human) - sizeof(Packet); }

	};

	class WGDetailOtherOnlineAttrib_HumanHandler 
	{
	public:
		static UINT Execute( WGDetailOtherOnlineAttrib_Human* pPacket, Player* pPlayer ) ;
	};


}


#endif

