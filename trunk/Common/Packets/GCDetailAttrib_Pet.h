// GCDetailAttrib_Pet.h
// 
// ֻ��֪ͨ��ҳ����б��е���Ϣ, ����ĸ�����Ϣ��Ȼ��GCCharBaseAttrib
// 
//////////////////////////////////////////////////////

#ifndef __GCDETAILATTRIB_PET_H__
#define __GCDETAILATTRIB_PET_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"
#include "PetDetailExtraStructDefine.h"
#define MAX_EXTRAINFO_LENGTH 256
namespace Packets
{
	enum 
	{
		TYPE_NORMAL = 0, 
		TYPE_EXCHANGE,
		TYPE_STALL,
		TYPE_PLAYERSHOP,
		TYPE_CONTEX_MENU_OTHER_PET,
	};

	enum ENUM_PET_DETAIL_ATTR
	{
		PET_DETAIL_ATTR_INVALID		= -1,
		PET_DETAIL_ATTR_OBJ_ID,				// ObjID
		PET_DETAIL_ATTR_DATA_ID,			// DataID
		PET_DETAIL_ATTR_NAME,				// ����
		PET_DETAIL_ATTR_NICK_NAME,			// �ǳ�
		PET_DETAIL_ATTR_AI_TYPE,			// �Ը�
		PET_DETAIL_ATTR_SPOUSE_GUID,		// ��żID
		PET_DETAIL_ATTR_LEVEL,				// �ȼ�
		PET_DETAIL_ATTR_EXP,				// ����
		PET_DETAIL_ATTR_HP,					// Ѫ��ǰֵ
		PET_DETAIL_ATTR_HPMAX,				// Ѫ���ֵ

		PET_DETAIL_ATTR_LIFE,				// ��ǰ����
		PET_DETAIL_ATTR_GENERATION,			// ������
		PET_DETAIL_ATTR_HAPPINESS,			// ���ֶ�

		//PET_DETAIL_ATTR_ATT_PHYSICS,		// ��������
		PET_DETAIL_ATTR_ATT_NEAR,				//�����⹦������
		PET_DETAIL_ATTR_ATT_FAR,					//Զ���⹦������

		PET_DETAIL_ATTR_ATT_MAGIC_NEAR,		// �����ڹ�������
		PET_DETAIL_ATTR_ATT_MAGIC_FAR,		// Զ�̳��ڹ�������
		//PET_DETAIL_ATTR_DEF_PHYSICS,		// ���������
		PET_DETAIL_ATTR_DEF_NEAR,				// �����⹦������
		PET_DETAIL_ATTR_DEF_FAR,					//Զ���⹦������

		PET_DETAIL_ATTR_DEF_MAGIC_NEAR,		// �����ڹ�������
		PET_DETAIL_ATTR_DEF_MAGIC_FAR,		// Զ���ڹ�������

		PET_DETAIL_ATTR_HIT,				// ������
		PET_DETAIL_ATTR_MISS,				// ������
		PET_DETAIL_ATTR_CRITICAL,			// ������

		PET_DETAIL_ATTR_MODELID,			// ����
		PET_DETAIL_ATTR_MOUNTID,			// ����ID
		
		PET_DETAIL_ATTR_STRPERCEPTION,		// ��������
		PET_DETAIL_ATTR_CONPERCEPTION,		// ��������
		PET_DETAIL_ATTR_DEXPERCEPTION,		// ������
		//PET_DETAIL_ATTR_SPRPERCEPTION,		// ��������
		PET_DETAIL_ATTR_INTPERCEPTION,		// ��������

		PET_DETAIL_ATTR_STR,				// ����
		PET_DETAIL_ATTR_CON,				// ����
		PET_DETAIL_ATTR_DEX,				// ��
		PET_DETAIL_ATTR_INT,				// ����
		PET_DETAIL_ATTR_GENGU,				// ����

		PET_DETAIL_ATTR_POINT_REMAIN,		// Ǳ�ܵ�

		PET_DETAIL_ATTR_SKILL_0,			// ����0
		PET_DETAIL_ATTR_SKILL_1,			// ����1
		PET_DETAIL_ATTR_SKILL_2,			// ����2
		PET_DETAIL_ATTR_SKILL_3,			// ����3
		PET_DETAIL_ATTR_SKILL_4,			// ����4
		PET_DETAIL_ATTR_SKILL_5,			// ����5
		PET_DETAIL_ATTR_SKILL_6,			// ����6
		PET_DETAIL_ATTR_SKILL_7,			// ����7
		PET_DETAIL_ATTR_SKILL_8,			// ����8
		PET_DETAIL_ATTR_SKILL_9,			// ����9
		PET_DETAIL_ATTR_SKILL_10,			// ����10
		PET_DETAIL_ATTR_SKILL_11,			// ����11


		PET_DETAIL_ATTR_SAVVY,				// ����
		PET_DETAIL_ATTR_GROWRATE,			// �ɳ���
		PET_DETAIL_ATTR_SEX,				// �Ա�
		PET_DETAIL_ATTR_NUMBERS
	};
	
	enum HorseState
	{
		//����״̬
		HorseState_Normal,
		//�����У����������ܲ���
		HorseState_Mating
	};
	class GCDetailAttrib_Pet :
		public Packet
	{
	public:
		GCDetailAttrib_Pet( ){
			m_Flags.m_uHighFlags	=	0;
			m_Flags.m_uLowFlags		=	0;
			m_ExtraInfoLength		=	0;
			memset(m_ExtraInfoData, 0, MAX_EXTRAINFO_LENGTH);
			memset( m_szNickName, 0, sizeof(m_szNickName) );
			memset( m_szName, 0, sizeof(m_szName) );
			
			m_iHorseState = HorseState_Normal;
		}
		virtual ~GCDetailAttrib_Pet( ){}

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_DETAILATTRIB_PET; }
		virtual UINT			GetPacketSize()const 
		{
			UINT uSize = sizeof(PET_GUID_t) + sizeof(Flag64) + sizeof(INT) + sizeof(INT);
			UINT i;
			for ( i = 0; i < PET_DETAIL_ATTR_NUMBERS; i++ )
			{
				if(m_Flags.isSetBit(i))
				{
					switch ( i )
					{
					case PET_DETAIL_ATTR_SPOUSE_GUID:
						uSize += sizeof(PET_GUID_t);
						break;
					case PET_DETAIL_ATTR_OBJ_ID:
						uSize += sizeof(ObjID_t);
						break;
					case PET_DETAIL_ATTR_NAME:
						uSize += sizeof(BYTE) + m_byNameSize;
						break;
					case PET_DETAIL_ATTR_NICK_NAME:
						uSize += sizeof(BYTE) + m_byNickNameSize;
						break;
					case PET_DETAIL_ATTR_GENERATION:
					case PET_DETAIL_ATTR_HAPPINESS:
						uSize += sizeof(BYTE);
						break;

					case PET_DETAIL_ATTR_SKILL_0:
					case PET_DETAIL_ATTR_SKILL_1:
					case PET_DETAIL_ATTR_SKILL_2:
					case PET_DETAIL_ATTR_SKILL_3:
					case PET_DETAIL_ATTR_SKILL_4:
					case PET_DETAIL_ATTR_SKILL_5:
					case PET_DETAIL_ATTR_SKILL_6:
					case PET_DETAIL_ATTR_SKILL_7:
					case PET_DETAIL_ATTR_SKILL_8:
					case PET_DETAIL_ATTR_SKILL_9:
					case PET_DETAIL_ATTR_SKILL_10:
					case PET_DETAIL_ATTR_SKILL_11:

						uSize += sizeof( _PET_SKILL );
						break;
					default:
						uSize += sizeof( UINT );	
						break;
					}
				}
			}

			uSize += sizeof(BYTE);
			uSize += m_ExtraInfoLength;
			return uSize;
		}

	public:
		PET_GUID_t				GetGUID( VOID )const{ return m_GUID; }
		VOID					SetGUID( PET_GUID_t GUID ){ m_GUID = GUID; }

		INT						GetTradeIndex( VOID )const{ return m_nTradeIndex; }
		VOID					SetTradeIndex( INT nIndex ){ m_nTradeIndex = nIndex; }

		Flag64					GetFlags( VOID )const{ return m_Flags; }

		BOOL					IsUpdateAttrib( ENUM_PET_DETAIL_ATTR eAttrib )const{ return m_Flags.isSetBit((INT)eAttrib)?(TRUE):(FALSE); }
		VOID					SetUpdateAttrib( ENUM_PET_DETAIL_ATTR eAttrib, BOOL bUpdate ){ m_Flags.UpdateBits((INT)eAttrib,bUpdate); }

		VOID					SetObjID(ObjID_t id) { m_ObjID = id; SetUpdateAttrib(PET_DETAIL_ATTR_OBJ_ID,TRUE); }
		ObjID_t					GetObjID(VOID)const { return m_ObjID; }

		VOID					SetDataID(INT id) { m_nDataID = id; SetUpdateAttrib(PET_DETAIL_ATTR_DATA_ID,TRUE); }
		INT						GetDataID(VOID)const { return m_nDataID; }

		VOID					SetName( const CHAR *pszName )
		{
			m_byNameSize = (BYTE)(strlen( pszName ));
			strncpy( m_szName, pszName, MAX_CHARACTER_NAME-1 );
			SetUpdateAttrib(PET_DETAIL_ATTR_NAME,TRUE);
		}

		const CHAR*				GetName( VOID )const;

		const CHAR*				GetNickName() const;
		VOID					SetNickName( const CHAR* pszNickName )
		{
			m_byNickNameSize = (BYTE)(strlen( pszNickName ));
			strncpy( m_szNickName, pszNickName, MAX_CHARACTER_NAME-1 );
			SetUpdateAttrib(PET_DETAIL_ATTR_NICK_NAME,TRUE);
		}

		INT						GetAIType( VOID )const{ return m_nAIType; }
		VOID					SetAIType( INT nAIType ){ m_nAIType = nAIType; SetUpdateAttrib(PET_DETAIL_ATTR_AI_TYPE, TRUE); }

		PET_GUID_t				GetSpouseGUID( VOID )const{ return m_SpouseGUID; }
		VOID					SetSpouseGUID( PET_GUID_t guid ){ m_SpouseGUID = guid; SetUpdateAttrib(PET_DETAIL_ATTR_SPOUSE_GUID, TRUE); }

		INT						GetLevel( VOID )const{ return m_nLevel; }
		VOID					SetLevel( INT nLevel ){ m_nLevel = nLevel; SetUpdateAttrib(PET_DETAIL_ATTR_LEVEL, TRUE); }

		INT						GetExp( VOID )const{ return m_nExp; }
		VOID					SetExp( INT nExp ){ m_nExp = nExp; SetUpdateAttrib(PET_DETAIL_ATTR_EXP, TRUE ); }

		INT						GetHP( VOID )const{ return m_nHP; }
		VOID					SetHP( INT nHP ){ m_nHP = nHP; SetUpdateAttrib(PET_DETAIL_ATTR_HP, TRUE ); }

		INT						GetHPMax( VOID )const{ return m_nHPMax; }
		VOID					SetHPMax( INT nHPMax ){ m_nHPMax = nHPMax; SetUpdateAttrib(PET_DETAIL_ATTR_HPMAX, TRUE ); }

		INT						GetLife( VOID )const{ return m_nLife; }
		VOID					SetLife( INT nLife ){ m_nLife = nLife; SetUpdateAttrib(PET_DETAIL_ATTR_LIFE, TRUE ); }

		BYTE					GetGeneration( VOID )const{ return m_byGeneration; }
		VOID					SetGeneration( BYTE byGeneration ){ m_byGeneration = byGeneration; SetUpdateAttrib(PET_DETAIL_ATTR_GENERATION, TRUE ); }

		BYTE					GetHappiness( VOID )const{ return m_byHappiness; }
		VOID					SetHappiness( BYTE byHappiness ){ m_byHappiness = byHappiness; SetUpdateAttrib(PET_DETAIL_ATTR_HAPPINESS, TRUE ); }

		/*INT						GetAtt_Physics( VOID )const{ return m_nAtt_Physics; }
		VOID					SetAtt_Physics( INT nAtt_Physics ){ m_nAtt_Physics = nAtt_Physics; SetUpdateAttrib(PET_DETAIL_ATTR_ATT_PHYSICS, TRUE ); }*/
		
		INT						GetAtt_Near( VOID )const{ return m_nAtt_Near; }
		VOID					SetAtt_Near( INT nAtt_Near ){ m_nAtt_Near = nAtt_Near; SetUpdateAttrib(PET_DETAIL_ATTR_ATT_NEAR, TRUE ); }
		INT						GetAtt_Far( VOID )const{ return m_nAtt_Far; }
		VOID					SetAtt_Far( INT nAtt_Far ){ m_nAtt_Far = nAtt_Far; SetUpdateAttrib(PET_DETAIL_ATTR_ATT_FAR, TRUE ); }

		INT						GetAtt_MagicNear( VOID )const{ return m_nAtt_MagicNear; }
		INT						GetAtt_MagicFar( VOID )const{ return m_nAtt_MagicFar; }

		VOID					SetAtt_MagicNear( INT nAtt_MagicNear ){ m_nAtt_MagicNear = nAtt_MagicNear; SetUpdateAttrib(PET_DETAIL_ATTR_ATT_MAGIC_NEAR, TRUE ); }
		VOID					SetAtt_MagicFar( INT nAtt_MagicFar ){ m_nAtt_MagicFar = nAtt_MagicFar; SetUpdateAttrib(PET_DETAIL_ATTR_ATT_MAGIC_FAR, TRUE ); }

		/*INT						GetDef_Physics( VOID )const{ return m_nDef_Physics; }
		VOID					SetDef_Physics( INT nDef_Physics ){ m_nDef_Physics = nDef_Physics; SetUpdateAttrib(PET_DETAIL_ATTR_DEF_PHYSICS, TRUE ); }*/
		INT						GetDef_Near( VOID )const{ return m_nDef_Near; }
		VOID					SetDef_Near( INT nDef_Near ){ m_nDef_Near = nDef_Near; SetUpdateAttrib(PET_DETAIL_ATTR_DEF_NEAR, TRUE ); }
		INT						GetDef_Far( VOID )const{ return m_nDef_Far; }
		VOID					SetDef_Far( INT nDef_Far ){ m_nDef_Far = nDef_Far; SetUpdateAttrib(PET_DETAIL_ATTR_DEF_FAR, TRUE ); }

		INT						GetDef_MagicNear( VOID )const{ return m_nDef_MagicNear; }
		INT						GetDef_MagicFar( VOID )const{ return m_nDef_MagicFar; }

		VOID					SetDef_MagicNear( INT nDef_MagicNear ){ m_nDef_MagicNear = nDef_MagicNear; SetUpdateAttrib(PET_DETAIL_ATTR_DEF_MAGIC_NEAR, TRUE ); }
		VOID					SetDef_MagicFar( INT nDef_MagicFar ){ m_nDef_MagicFar = nDef_MagicFar; SetUpdateAttrib(PET_DETAIL_ATTR_DEF_MAGIC_FAR, TRUE ); }

		INT						GetHit( VOID )const{ return m_nHit; }
		VOID					SetHit( INT nHit ){ m_nHit = nHit; SetUpdateAttrib(PET_DETAIL_ATTR_HIT, TRUE ); }

		INT						GetMiss( VOID )const{ return m_nMiss; }
		VOID					SetMiss( INT nMiss ){ m_nMiss = nMiss; SetUpdateAttrib(PET_DETAIL_ATTR_MISS, TRUE ); }

		INT						GetCritical( VOID )const{ return m_nCritical; }
		VOID					SetCritical( INT nCritical ){ m_nCritical = nCritical; SetUpdateAttrib(PET_DETAIL_ATTR_CRITICAL, TRUE ); }

		INT						GetModelID( VOID )const{ return m_nModelID; }
		VOID					SetModelID( INT nModelID ){ m_nModelID = nModelID; SetUpdateAttrib(PET_DETAIL_ATTR_MODELID, TRUE ); }

		INT						GetMountID( VOID )const{ return m_nMountID; }
		VOID					SetMountID( INT nMountID ){ m_nMountID = nMountID; SetUpdateAttrib(PET_DETAIL_ATTR_MOUNTID, TRUE ); }

		INT						GetStrPerception( VOID )const{ return m_StrPerception; }
		VOID					SetStrPerception(INT StrPerception ){ m_StrPerception = StrPerception; SetUpdateAttrib(PET_DETAIL_ATTR_STRPERCEPTION, TRUE ); }

		INT						GetConPerception( VOID )const{ return m_ConPerception; }
		VOID					SetConPerception(INT ConPerception ){ m_ConPerception = ConPerception; SetUpdateAttrib(PET_DETAIL_ATTR_CONPERCEPTION, TRUE ); }

		INT						GetDexPerception( VOID )const{ return m_DexPerception; }
		VOID					SetDexPerception(INT  DexPerception ){ m_DexPerception = DexPerception; SetUpdateAttrib(PET_DETAIL_ATTR_DEXPERCEPTION, TRUE ); }

		/*INT						GetSprPerception( VOID )const{ return m_SprPerception; }
		VOID					SetSprPerception(INT SprPerception ){ m_SprPerception = SprPerception; SetUpdateAttrib(PET_DETAIL_ATTR_SPRPERCEPTION, TRUE ); }*/

		INT						GetIntPerception( VOID )const{ return m_IntPerception; }
		VOID					SetIntPerception(INT  IntPerception ){ m_IntPerception = IntPerception; SetUpdateAttrib(PET_DETAIL_ATTR_INTPERCEPTION, TRUE ); }

		INT						GetStr( VOID )const{ return m_Str; }
		VOID					SetStr( INT Str ){ m_Str = Str; SetUpdateAttrib(PET_DETAIL_ATTR_STR, TRUE ); }

		INT						GetCon( VOID )const{ return m_Con; }
		VOID					SetCon( INT Con ){ m_Con = Con; SetUpdateAttrib(PET_DETAIL_ATTR_CON, TRUE ); }

		INT						GetDex( VOID )const{ return m_Dex; }
		VOID					SetDex( INT  Dex ){ m_Dex = Dex; SetUpdateAttrib(PET_DETAIL_ATTR_DEX, TRUE ); }

		/*INT						GetSpr( VOID )const{ return m_Spr; }
		VOID					SetSpr( INT Spr ){ m_Spr = Spr; SetUpdateAttrib(PET_DETAIL_ATTR_SPR, TRUE ); }*/

		INT						GetInt( VOID )const{ return m_Int; }
		VOID					SetInt( INT  Int ){ m_Int = Int; SetUpdateAttrib(PET_DETAIL_ATTR_INT, TRUE ); }

		INT						GetGenGu( VOID )const{ return m_GenGu; }
		VOID					SetGenGu( INT  nGenGu ){ m_GenGu = nGenGu; SetUpdateAttrib(PET_DETAIL_ATTR_GENGU, TRUE ); }

		INT						GetRemainPoint( VOID )const{ return m_nRemainPoint; }
		VOID					SetRemainPoint( INT nRemainPoint ){ m_nRemainPoint = nRemainPoint; SetUpdateAttrib(PET_DETAIL_ATTR_POINT_REMAIN, TRUE ); }

		const _PET_SKILL		*GetSkill( UINT uIndex )const{ return ((uIndex<PET_MAX_SKILL_COUNT)?(&(m_aSkill[uIndex])):(NULL)); }
		VOID					SetSkill( UINT uIndex, const _PET_SKILL *pSkill )
		{
			if ( pSkill != NULL && uIndex < PET_MAX_SKILL_COUNT )
			{
				m_aSkill[uIndex] = *pSkill;
				SetUpdateAttrib( (ENUM_PET_DETAIL_ATTR)(uIndex + PET_DETAIL_ATTR_SKILL_0), TRUE );
			}
			else
			{
				Assert( (pSkill != NULL && uIndex < PET_MAX_SKILL_COUNT) && "GCDetailAttrib_Pet::SetSkill" );
			}
		}

		BYTE					GetExtraInfoLength(VOID) const {return m_ExtraInfoLength;}
		VOID					SetExtraInfoLength(BYTE ExtraInfoLength) {m_ExtraInfoLength = ExtraInfoLength;}

		BYTE*					GetExtraInfoData(VOID) {return m_ExtraInfoData;}
		VOID					SetExtraInfoData(BYTE* ExtraInfoData)
		{
			memcpy(m_ExtraInfoData, ExtraInfoData, m_ExtraInfoLength*sizeof(BYTE));
		}

		INT  GetGrowRate() const { return m_iGrowRate; }
		VOID SetGrowRate(INT val) { m_iGrowRate = val; SetUpdateAttrib(PET_DETAIL_ATTR_GROWRATE, TRUE );}
		INT	 GetSavvy() const { return m_iSavvy; }
		VOID SetSavvy(INT val) { m_iSavvy = val; SetUpdateAttrib(PET_DETAIL_ATTR_SAVVY, TRUE );}
		INT  GetHorseState() const { return m_iHorseState; }
		VOID SetHorseState(INT val) { m_iHorseState = val; }
		INT	 GetSex() const { return m_iSex; }
		VOID SetSex(INT val) { m_iSex = val; SetUpdateAttrib(PET_DETAIL_ATTR_SEX, TRUE );}

	private:
		PET_GUID_t		m_GUID;										// ID
		INT				m_nTradeIndex;								// �����õ�������ֵ

		Flag64			m_Flags;									// ÿ��λ��ʾһ�������Ƿ�Ҫˢ�� ENUM_DETAIL_ATTRIB

		ObjID_t			m_ObjID;									// ����Obj���͵�ObjID
		INT				m_nDataID;									// ģ��ID,��������
		BYTE			m_byNameSize;								// ���Ƴ���,����������'\0'
		BYTE			m_byNickNameSize;							// �ǳƳ���,����������'\0'
		CHAR			m_szName[MAX_CHARACTER_NAME];				// ����
		CHAR			m_szNickName[MAX_CHARACTER_NAME];			// �ǳ�
		INT				m_nAIType;									// �Ը�
		PET_GUID_t		m_SpouseGUID;								// ��ż��GUID
		INT				m_nLevel;									// �ȼ�
		INT				m_nExp;										// ����
		INT				m_nHP;										// Ѫ��ǰֵ
		INT				m_nHPMax;									// Ѫ���ֵ
		INT				m_iHorseState;								// ��ǰ������״̬

		INT				m_nLife;									// ��ǰ����
		BYTE			m_byGeneration;								// ������
		BYTE			m_byHappiness;								// ���ֶ�

		INT				m_nAtt_Near;									//�����⹦������
		INT				m_nAtt_Far;									//Զ���⹦������
		
		INT				m_nAtt_MagicNear;							// �����ڹ�������
		INT				m_nAtt_MagicFar;								// Զ���ڹ�������

		INT				m_nDef_Near;									//�����⹦������		
		INT				m_nDef_Far;										//Զ���⹦������
		
		INT				m_nDef_MagicNear;							// �����ڹ�������
		INT				m_nDef_MagicFar;								// Զ���ڹ�������

		INT				m_nHit;										// ������
		INT				m_nMiss;									// ������
		INT				m_nCritical;								// ������

		INT				m_nModelID;									// ����
		INT				m_nMountID;									// ����ID

		INT				m_StrPerception;							// ��������
		INT				m_ConPerception;							// ��������
		INT 			m_DexPerception;							// ������
		INT 			m_IntPerception;							// ��������

		INT				m_Str;										// ����
		INT				m_Con;										// ����
		INT 			m_Dex;										// ��
		INT 			m_Int;										// ����
		INT 			m_GenGu;									// ����
		INT				m_iSavvy;									// ����
		INT				m_iGrowRate;								// �ɳ���
		INT				m_nRemainPoint;								// Ǳ�ܵ�
		INT				m_iSex;										// �Ա�

		_PET_SKILL		m_aSkill[PET_MAX_SKILL_COUNT];				// �����б�

		BYTE			m_ExtraInfoLength;							// ������Ϣ����
		BYTE			m_ExtraInfoData[MAX_EXTRAINFO_LENGTH];		// ������Ϣ����


	};

	class GCDetailAttrib_PetFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCDetailAttrib_Pet() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_DETAILATTRIB_PET; }
		UINT		GetPacketMaxSize()const
		{
			UINT uSize = sizeof(GCDetailAttrib_Pet) - sizeof(Packet) ;
			return uSize;
		}
	};

	class GCDetailAttrib_PetHandler 
	{
	public:
		static UINT Execute( GCDetailAttrib_Pet* pPacket, Player* pPlayer ) ;
	};
}

#endif // __GCDETAILATTRIB_PET_H__
