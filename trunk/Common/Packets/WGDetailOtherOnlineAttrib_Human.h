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

		//���ü̳нӿ�
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
			//ÿ�ζ�����Ŀ������ID
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
		ObjID_t			m_ObjID;	// ����Obj���͵�ObjID

		Flag128			m_Flags;		// ÿ��λ��ʾһ�������Ƿ�Ҫˢ�� ENUM_DETAIL_ATTRIB


		UINT			m_uLevel;		// �ȼ�
		INT				m_nExp;			// ����ֵ
		INT				m_nHP;			//������
		INT				m_nMP;			//ħ����
		INT				m_nPneuma;		//Ԫ��
		INT				m_nMAXHP;		//���������
		INT				m_nMAXMP;		//���ħ����
		INT				m_nMaxPneuma;	//���Ԫ��ֵ
		FLOAT			m_fMoveSpeed;	//�ƶ��ٶ�


		UINT			m_uMoney;		// ��Ϸ����

		//һ��ս������


		INT				m_Str;					//���� ����
		//INT				m_Spr;					//���� ����
		INT				m_Con;					//���� ����
		INT 			m_Int;					//���� ����
		INT 			m_Dex;					//�� ����
		INT				m_nPoint_Remain;		//ʣ����������
		INT				m_nSkillPoint_Remain;   //ʣ������似�ܵ�
		INT				m_nS_SkillPoints[COMMON_S_NUMBER]; //�ѷ���ĵ�

		//����ս������
		INT				m_nHP_ReSpeed;		//HP�ָ��ٶ�  ��/��
		INT				m_nMP_ReSpeed;		//MP�ָ��ٶ�  ��/��
		INT				m_nVigor_ReSpeed;	//�����ָ��ٶ�  ��/��
		//INT				m_nAtt_Physics;		//��������

		INT				m_nAtt_Near;        //���̹���
		INT             m_nAtt_Far;         //Զ�̹���

		INT				m_nAtt_MagicNear;	//�����ڹ�������
		INT				m_nAtt_MagicFar;	//�����⹦������
		

		INT				m_nDef_Near;        //���̷���
		INT             m_nDef_Far;         //Զ�̷���

		INT				m_nDef_MagicNear;	//�����ڹ�������
		INT				m_nDef_MagicFar;	//�����⹦������

		INT             m_nToughness;       //����
		INT				m_nHit;				//������
		INT				m_nMiss;			//������
		INT				m_nCritic;			//����һ����
		INT				m_nCriticHurt;		//�����˺�

		INT				m_nMaxRage;			//ŭ������
		INT				m_nRage;			//ŭ��
		INT				m_nRage_ReSpeed;		//ŭ���ָ��ٶ�  ��/��
		INT				m_nStrikePoint;		//������

		INT				m_nAttackSpeed;		//�����ٶ�

		INT				m_nAttGold;			//�𹥻�
		INT				m_nDefGold;			//�����

		INT				m_nAttWood;			//ľ����
		INT				m_nDefWood;			//ľ����

		INT				m_nAttWater;		//ˮ����
		INT				m_nDefWater;		//ˮ����

		INT				m_nAttFire;			//�𹥻�
		INT				m_nDefFire;			//�����

		INT				m_nAttSoil;			//������
		INT				m_nDefSoil;			//������

		INT				m_nNearReduce;		//���̼���
		INT				m_nFarReduce;		//Զ�̼���

		INT				m_nMagicNearReduce;	//�����ڹ�����
		INT				m_nMagicFarReduce;	//�����⹦����

		INT				m_nDreadReduce;		//�ֿ��־�
		INT				m_nComaReduce;		//�ֿ�����
		INT				m_nHushReduce;		//�ֿ���Ĭ
		INT				m_nUnarmReduce;		//�ֿ���е
		INT				m_nAttSpeedResist;	//����������
		INT				m_nSkillSpeedResist;//��ʩ��
		INT				m_nMoveSpeedResist;	//������

		INT             m_nCountry;         //����
		INT				m_nMenPai;			//����
		INT				m_nGuild;			//����
		INT				m_nGP;	    		//���ɹ��׶�
		INT             m_nHonor;           //����
		INT             m_nPK;              //ɱ��
		CHAR            m_cFamilyID;        //������ID
		CHAR            m_cGuildOfficial;   //����ְ

		_CAMP_DATA		m_CampData;			// ��Ӫ	
		INT				m_nDataID;			// DataID
		INT				m_nPortraitID;		// ͷ��ID
		INT				m_nModelID;			// ����
		INT				m_nFaceModel;       //����
		INT				m_nMountID;			//����
		PET_GUID_t		m_guidCurrentPet;	//��ǰ����

		BOOL			m_bLimitMove;		//�Ƿ����Ʋ����ƶ�
		BOOL			m_bCanActionFlag1;		//�������ޱ��1,���ڻ��Դ���
		BOOL			m_bCanActionFlag2;		//�������ޱ��2,���ڳ�Ĭ

		INT				m_Vigor ;			// ����
		INT				m_MaxVigor ;		// ��������
		INT				m_Energy ;			// ����
		INT				m_MaxEnergy ;		// ��������

		INT				m_nGoodBadValue;	// �ƶ�ֵ
		ObjID_t			m_TargetID;			//������Ŀ���ID
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

