// GCCharBaseAttrib.h
// 
/////////////////////////////////////////////////////////

#ifndef __GCCHARBASEATTRIB_H__
#define __GCCHARBASEATTRIB_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCCharBaseAttrib : public Packet
	{
	public:
		enum ECHAR_AI_TYPE
		{
			CHAR_AI_TYPE_INVALID		= -1,	// INVALID
			CHAR_AI_TYPE_SCANNPC		= 0,	// 主动NPC
			CHAR_AI_TYPE_NOTSCANNPC		= 1,	// 非主动NPC
			CHAR_AI_TYPE_CANNOTATTACK	= 2,	// 不可攻击NPC
		};

		GCCharBaseAttrib( )
		{
			Reset();
		}
		virtual ~GCCharBaseAttrib( ){}

		VOID Reset( VOID )
		{
			m_ObjID				= UINT_MAX;

			m_uFlags			= 0;
			m_Job				= INVALID_JOB;
			m_Country			= INVALID_COUNTRY;
			m_wDataID			= 0xFFFF;
			m_HPPercent			= 0;
			m_MPPercent			= 0;
			m_PneumaPercent		= 0;
			m_nRage				= 0;
			m_fMoveSpeed		= 0.f;
			m_fAttackSpeed		= 1.f;
			m_CampData.CleanUp();
			m_nPortraitID		= INVALID_ID;
			m_nMountID			= INVALID_ID;
			m_nModelID			= INVALID_ID;
			m_nAIType			= -1;

			m_byNameSize		= 0;
			memset( m_szName, 0, sizeof( m_szName ) );

			m_byCountryTitleSize		= 0;
			memset( m_szCountryTitle, 0, sizeof( m_szCountryTitle ) );
            m_CountryTitleType			= 0;

            m_byGuildTitleSize  		= 0;
            memset( m_szGuildTitle, 0, sizeof( m_szGuildTitle ) );
            m_GuildTitleType			= 0;

            m_byNormalTitleSize	    	= 0;
            memset( m_szNormalTitle, 0, sizeof( m_szNormalTitle ) );
            m_NormalTitleType			= 0;

			m_uPlayerData		= 0;

			m_bStallIsOpen		= 0;
			m_nStallNameSize	= 0;
			memset( m_szStallName, 0, MAX_STALL_NAME);

			m_bOpenPublic		= 0;	// 玩家摊位开市标记	20100706 BLL

			m_OccupantGUID		= INVALID_ID ;
			m_OwnerID			= INVALID_ID ;
		}

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_CHARBASEATTRIB; }
		virtual UINT			GetPacketSize()const 
		{
			UINT uAttribSize = sizeof(ObjID_t) + sizeof(UINT);

			if ( m_uFlags & (1 << UPDATE_CHAR_ATT_JOB) )
				uAttribSize += sizeof(m_Job);

			if ( m_uFlags & (1 << UPDATE_CHAR_ATT_COUNTRY) )
				uAttribSize += sizeof(m_Country);

			if ( m_uFlags & (1 << UPDATE_CHAR_ATT_DATA_ID) )
				uAttribSize += sizeof(m_wDataID);

			if ( m_uFlags & (1<<UPDATE_CHAR_ATT_NAME) )
			{
				uAttribSize += sizeof(m_byNameSize);
				uAttribSize += sizeof(CHAR) * m_byNameSize;
			}

			if ( m_uFlags & (1<<UPDATE_CHAR_ATT_COUNTRYTITLE) )
			{
				uAttribSize += sizeof(m_CountryTitleType);
				uAttribSize += sizeof(m_byCountryTitleSize);
				uAttribSize += sizeof(CHAR) * m_byCountryTitleSize;
			}

            if ( m_uFlags & (1<<UPDATE_CHAR_ATT_GUILDTITLE) )
            {
                uAttribSize += sizeof(m_GuildTitleType);
                uAttribSize += sizeof(m_byGuildTitleSize);
                uAttribSize += sizeof(CHAR) * m_byGuildTitleSize;
            }

            if ( m_uFlags & (1<<UPDATE_CHAR_ATT_NORMALTITLE) )
            {
                uAttribSize += sizeof(m_NormalTitleType);
                uAttribSize += sizeof(m_byNormalTitleSize);
                uAttribSize += sizeof(CHAR) * m_byNormalTitleSize;
            }

			if ( m_uFlags & (1<<UPDATE_CHAR_ATT_LEVEL) )
				uAttribSize += sizeof(m_Level);

			if ( m_uFlags & (1<<UPDATE_CHAR_ATT_HP_PERCENT) )
				uAttribSize += sizeof(m_HPPercent);

			if ( m_uFlags & (1<<UPDATE_CHAR_ATT_MP_PERCENT) )
				uAttribSize += sizeof(m_MPPercent);

			if ( m_uFlags & (1<<UPDATE_CHAR_ATT_PNEUMA_PERCENT) )
				uAttribSize += sizeof(m_PneumaPercent);

			if ( m_uFlags & (1<<UPDATE_CHAR_ATT_RAGE) )
				uAttribSize += sizeof(m_nRage);

			if ( m_uFlags & (1<<UPDATE_CHAR_ATT_STEALTH_LEVEL) )
				uAttribSize += sizeof(m_nStealthLevel);

			if ( m_uFlags & (1<<UPDATE_CHAR_ATT_SIT) )
				uAttribSize += sizeof(m_cMoodState);

			if ( m_uFlags & (1<<UPDATE_CHAR_ATT_MOVE_SPEED) )
				uAttribSize += sizeof(m_fMoveSpeed);

			if ( m_uFlags & (1<<UPDATE_CHAR_ATT_ATTACK_SPEED) )
				uAttribSize += sizeof(m_fAttackSpeed);

			if ( m_uFlags & (1<<UPDATE_CHAR_ATT_CAMP_ID) )
				uAttribSize += sizeof(m_CampData);

			if ( m_uFlags & (1<<UPDATE_CHAR_ATT_PORTRAIT_ID) )
				uAttribSize += sizeof(m_nPortraitID);

			if ( m_uFlags & (1<<UPDATE_CHAR_ATT_MODEL_ID) )
				uAttribSize += sizeof(m_nModelID);

			if ( m_uFlags & (1<<UPDATE_CHAR_ATT_MOUNT_ID) )
				uAttribSize += sizeof(m_nMountID);

             //  [7/23/2010 陈军龙]
			if ( m_uFlags & (1<<UPDATE_CHAR_ATT_MOUNT_INDEX) )
				uAttribSize += sizeof(m_nMountIndex);

			if ( m_uFlags & (1<<UPDATE_CHAR_ATT_AITYPE) )
				uAttribSize += sizeof(m_nAIType);

			if ( m_uFlags & (1<<UPDATE_CHAR_ATT_PLAYER_DATA) )
			{
				uAttribSize += sizeof(m_uPlayerData);
				uAttribSize += sizeof(m_HairColor);
			}

			if ( m_uFlags & (1<<UPDATE_CHAR_IS_IN_STALL) )
				uAttribSize += sizeof(m_bStallIsOpen);

			if ( m_uFlags & (1<<UPDATE_CHAR_STALL_NAME) )
			{
				uAttribSize += sizeof(m_nStallNameSize) + sizeof(CHAR)*m_nStallNameSize;
			}

			if (m_uFlags & (1<<UPDATE_CHAR_OCCUPANT) )
			{
				uAttribSize += sizeof(GUID_t) ;
			}

			if (m_uFlags & (1<<UPDATE_CHAR_OWNER) )
			{
				uAttribSize += sizeof(ObjID_t) ;
			}

			if (m_uFlags &(1<<UPDATE_CHAR_BUSOBJID))
			{
				uAttribSize += sizeof(ObjID_t) ;
			}

			if (m_uFlags & (1<<UPDATE_CHAR_STALL_PUBLIC) ) 
			{
				uAttribSize += sizeof(BYTE) ;
			}	
			
			return uAttribSize;
		}

	public:
		//使用数据接口
		VOID			setObjID(ObjID_t id) { m_ObjID = id; }
		ObjID_t			getObjID(VOID)const { return m_ObjID; }

		UINT			getFlags( VOID )const{ return m_uFlags; }

		BOOL			IsUpdateAttrib( ENUM_UPDATE_CHAR_ATT eAttrib )const{ return (m_uFlags & (1<<(INT)eAttrib))?(TRUE):(FALSE); }
		VOID			SetUpdateAttrib( ENUM_UPDATE_CHAR_ATT eAttrib, BOOL bUpdate )
		{
			if ( bUpdate )
				m_uFlags |= (1<<(INT)eAttrib);
			else
				m_uFlags &= (~(1<<(INT)eAttrib));
		}
		
		VOID			setJob(BYTE nJob) { m_Job = nJob; SetUpdateAttrib(UPDATE_CHAR_ATT_JOB,TRUE);}
		BYTE			getJob(VOID)const { return m_Job; }
		
		VOID			setCountry(BYTE nCountry) { m_Country = nCountry; SetUpdateAttrib(UPDATE_CHAR_ATT_COUNTRY,TRUE);}
		BYTE			getCountry(VOID)const { return m_Country; }
		
		VOID			setDataID( WORD wID ){ m_wDataID = wID; SetUpdateAttrib(UPDATE_CHAR_ATT_DATA_ID,TRUE); }
		WORD			getDataID( VOID )const{ return m_wDataID; }

		VOID			setLevel( BYTE Level ){ m_Level = Level; SetUpdateAttrib(UPDATE_CHAR_ATT_LEVEL,TRUE); }
		BYTE			getLevel( VOID )const{ return m_Level; }

		VOID			setHPPercent( BYTE HPPercent ){ m_HPPercent = HPPercent; SetUpdateAttrib(UPDATE_CHAR_ATT_HP_PERCENT,TRUE); }
		BYTE			getHPPercent( VOID )const{ return m_HPPercent; }

		VOID			setMPPercent( BYTE MPPercent ){ m_MPPercent = MPPercent; SetUpdateAttrib(UPDATE_CHAR_ATT_MP_PERCENT,TRUE); }
		BYTE			getMPPercent( VOID )const{ return m_MPPercent; }

		VOID			setPneumaPercent( BYTE PneumaPercent ){ m_PneumaPercent = PneumaPercent; SetUpdateAttrib(UPDATE_CHAR_ATT_PNEUMA_PERCENT,TRUE); }
		BYTE			getPneumaPercent( VOID )const{ return m_PneumaPercent; }

		VOID			setRage( INT nRage ){ m_nRage = nRage; SetUpdateAttrib(UPDATE_CHAR_ATT_RAGE,TRUE); }
		INT				getRage( VOID )const{ return m_nRage; }

		VOID			setStealthLevel( INT nStealthLevel ){ m_nStealthLevel = nStealthLevel; SetUpdateAttrib(UPDATE_CHAR_ATT_STEALTH_LEVEL,TRUE); }
		INT				getStealthLevel( VOID )const{ return m_nStealthLevel; }

		VOID			setMoodState(INT nState){ m_cMoodState = (char)nState; SetUpdateAttrib(UPDATE_CHAR_ATT_SIT,TRUE); }
		INT				getMoodState( VOID )const{ return (INT)(m_cMoodState); }

		VOID			setMoveSpeed( FLOAT fMoveSpeed ){ m_fMoveSpeed = fMoveSpeed; SetUpdateAttrib(UPDATE_CHAR_ATT_MOVE_SPEED,TRUE); }
		FLOAT			getMoveSpeed( VOID )const{ return m_fMoveSpeed; }

		VOID			setAttackSpeed( FLOAT fAttackSpeed ){ m_fAttackSpeed = fAttackSpeed; SetUpdateAttrib(UPDATE_CHAR_ATT_ATTACK_SPEED,TRUE); }
		FLOAT			getAttackSpeed( VOID )const{ return m_fAttackSpeed; }

		VOID					setCampData( const _CAMP_DATA *pCampData ){ m_CampData = *pCampData; SetUpdateAttrib(UPDATE_CHAR_ATT_CAMP_ID,TRUE); }
		const _CAMP_DATA		*getCampData( VOID )const{ return &m_CampData; }

		VOID			setPortraitID( INT nPortraitID ){ m_nPortraitID =nPortraitID; SetUpdateAttrib(UPDATE_CHAR_ATT_PORTRAIT_ID,TRUE); }
		UINT			getPortraitID( VOID )const{ return m_nPortraitID;}

		VOID			setModelID( INT nModelID ){ m_nModelID =nModelID; SetUpdateAttrib(UPDATE_CHAR_ATT_MODEL_ID,TRUE); }
		UINT			getModelID( VOID )const{ return m_nModelID;}

		VOID			setMountID(INT nID) {m_nMountID = nID; SetUpdateAttrib(UPDATE_CHAR_ATT_MOUNT_ID, TRUE);}
		INT				getMountID(VOID) {return m_nMountID;}

        //  [7/23/2010 陈军龙]
		VOID			setMountIndex(INT nIndex) {m_nMountIndex = nIndex; SetUpdateAttrib(UPDATE_CHAR_ATT_MOUNT_INDEX, TRUE);}
		INT				getMountIndex(VOID) {return m_nMountIndex;}

		VOID			setAIType(INT nAIType) {m_nAIType = nAIType; SetUpdateAttrib(UPDATE_CHAR_ATT_AITYPE, TRUE);	}
		INT				getAIType(VOID) const { return m_nAIType; } 

		VOID			setName( const CHAR *pszName ){
			if ( !pszName )
				return ;
			m_byNameSize = (BYTE)(strlen( pszName ));
			strncpy( m_szName, pszName, MAX_CHARACTER_NAME-1 );
			SetUpdateAttrib(UPDATE_CHAR_ATT_NAME,TRUE);
		}

		const CHAR		*getName( VOID )const{ return m_szName; }


        //////////////////////////////////////////////////////////////////////////
        //称号相关
        //////////////////////////////////////////////////////////////////////////
		VOID			setCountryTitle( const CHAR *pszTitle ){
			SetUpdateAttrib(UPDATE_CHAR_ATT_COUNTRYTITLE,TRUE);
			if ( !pszTitle )
				return ;
			m_byCountryTitleSize = (BYTE)(strlen( pszTitle ));
			strncpy( m_szCountryTitle, pszTitle, MAX_CHARACTER_TITLE-1 );
		}

		const CHAR		*getCountryTitle( VOID )const{ return m_szCountryTitle; }

		VOID			setTitleType(BYTE nTitleType)
                        {
                            m_CountryTitleType = nTitleType;
                            SetUpdateAttrib(UPDATE_CHAR_ATT_COUNTRYTITLE, TRUE);
                        }
		BYTE			getTitleType(VOID)  { return m_CountryTitleType; } 

        VOID			setCountryTitleType(BYTE nTitleType)
                        {
                            m_CountryTitleType = nTitleType;
                            SetUpdateAttrib(UPDATE_CHAR_ATT_GUILDTITLE, TRUE);
                        }
        BYTE			getCountryTitleType(VOID)  { return m_CountryTitleType; } 
        

        VOID			setGuildTitle( const CHAR *pszTitle ){
            SetUpdateAttrib(UPDATE_CHAR_ATT_GUILDTITLE,TRUE);
            if ( !pszTitle )
                return ;
            m_byGuildTitleSize = (BYTE)(strlen( pszTitle ));
            strncpy( m_szGuildTitle, pszTitle, MAX_CHARACTER_TITLE-1 );
        }

        const CHAR		*getGuildTitle( VOID )const{ return m_szGuildTitle; }

        VOID			setGuildTitleType(BYTE nTitleType)
                        {
                            m_GuildTitleType = nTitleType;
                            SetUpdateAttrib(UPDATE_CHAR_ATT_GUILDTITLE, TRUE);
                        }
        BYTE			getGuildTitleType(VOID)  { return m_GuildTitleType; } 


        VOID			setNormalTitle( const CHAR *pszTitle ){
            SetUpdateAttrib(UPDATE_CHAR_ATT_NORMALTITLE,TRUE);
            if ( !pszTitle )
                return ;
            m_byNormalTitleSize = (BYTE)(strlen( pszTitle ));
            strncpy( m_szNormalTitle, pszTitle, MAX_CHARACTER_TITLE-1 );
        }

        const CHAR		*getNormalTitle( VOID )const{ return m_szNormalTitle; }

        VOID			setNormalTitleType(BYTE nTitleType)
        {
            m_NormalTitleType = nTitleType;
            SetUpdateAttrib(UPDATE_CHAR_ATT_NORMALTITLE, TRUE);
        }
        BYTE			getNormalTitleType(VOID) { return m_NormalTitleType; } 

        //////////////////////////////////////////////////////////////////////////
        //
        //////////////////////////////////////////////////////////////////////////


		VOID			setStallName( const CHAR *pszName ){
			if ( !pszName )
				return ;
			m_nStallNameSize = (BYTE)(strlen( pszName ));
			strncpy( m_szStallName, pszName, MAX_STALL_NAME-1 );
			SetUpdateAttrib(UPDATE_CHAR_STALL_NAME,TRUE);
		}

		const CHAR		*getStallName( VOID )const{ return m_szStallName; }

		VOID			setFaceMeshID( UINT uMeshID ){
			m_uPlayerData |= ((uMeshID & 0XFF));
			SetUpdateAttrib(UPDATE_CHAR_ATT_PLAYER_DATA,TRUE);
		}
		UINT			getFaceMeshID( VOID )const
		{
			UINT uFaceMeshID = (m_uPlayerData & 0x000000FF);
			if(uFaceMeshID == (UINT)0xFF)
			{
				uFaceMeshID = 0xFFFFFFFF;
			}
			return uFaceMeshID;
		}

		VOID			setHairMeshID( UINT uMeshID ){
			m_uPlayerData |= ((uMeshID & 0XFF)<<8);
			SetUpdateAttrib(UPDATE_CHAR_ATT_PLAYER_DATA,TRUE);
		}
		UINT			getHairMeshID( VOID )const
		{
			UINT uHairMeshID = ((m_uPlayerData>>8) & 0x000000FF);
			if(uHairMeshID == (UINT)0xFF)
			{
				uHairMeshID = 0xFFFFFFFF;
			}
			return uHairMeshID;
		}

		VOID			setHairMatrixID( UINT uColor ){
			m_HairColor = uColor;
			SetUpdateAttrib(UPDATE_CHAR_ATT_PLAYER_DATA,TRUE);
		}
		UINT			getHairMatrixID( VOID )const{ return m_HairColor; }

		VOID			setIsOpen( BYTE StallIsOpen ){ m_bStallIsOpen = StallIsOpen; SetUpdateAttrib(UPDATE_CHAR_IS_IN_STALL,TRUE); }
		BYTE			getIsOpen( VOID )const{ return m_bStallIsOpen; }

		VOID			setOccupantGUID( GUID_t guid ){ m_OccupantGUID = guid ; SetUpdateAttrib(UPDATE_CHAR_OCCUPANT,TRUE); }
		GUID_t			getOccupantGUID( )const{ return m_OccupantGUID ; }

		VOID			setOwnerID( ObjID_t id ){ m_OwnerID = id ; SetUpdateAttrib(UPDATE_CHAR_OWNER,TRUE); }
		ObjID_t			getOwnerID( )const{ return m_OwnerID ; }

		ObjID_t			GetBusObjID() const { return m_BusID; }
		VOID			SetBusObjID( ObjID_t busID ) { m_BusID = busID; SetUpdateAttrib(UPDATE_CHAR_BUSOBJID,TRUE);}

		VOID			setIsOpenPublic( BYTE bOpenPublic ){ m_bOpenPublic = bOpenPublic; SetUpdateAttrib(UPDATE_CHAR_STALL_PUBLIC,TRUE); }
		BYTE			getIsOpenPublic( VOID )const{ return m_bOpenPublic; }

	private:
		ObjID_t			m_ObjID;		// ObjID

		// 每个位表示一个属性是否要刷新 ENUM_UPDATE_CHAR_ATT
		UINT			m_uFlags;		
		BYTE			m_Job;
		BYTE			m_Country;
		//数据部分	
		WORD			m_wDataID;			// 怪物的数据资源ID, 如果是玩家，则是性别
		BYTE			m_Level;			// 等级
		BYTE			m_HPPercent;		// 生命值百分比
		BYTE			m_MPPercent;		// 魔法值百分比
		BYTE			m_PneumaPercent;	//元气百分比
		INT				m_nRage;			// 怒气
		INT				m_nStealthLevel;	// 隐身级别
		CHAR			m_cMoodState;		// 表情状态
		FLOAT			m_fMoveSpeed;		// 移动的速度
		FLOAT			m_fAttackSpeed;		// 攻击速度
		_CAMP_DATA		m_CampData;			// 阵营
		INT				m_nPortraitID;		// 头像ID
		INT				m_nModelID;			// 变形
		INT				m_nMountID;			// 座骑
		INT				m_nMountIndex;		// 座骑索引                                      [7/23/2010 陈军龙]
		INT				m_nAIType;			// AI类型
		// Player专有
		BYTE			m_byNameSize;					        // 玩家姓名长度,不包括最后的'\0'
		CHAR			m_szName[MAX_CHARACTER_NAME];	        // 玩家姓名
		BYTE			m_byCountryTitleSize;					// 玩家国家称号长度,不包括最后的'\0'
		CHAR			m_szCountryTitle[MAX_CHARACTER_TITLE];	// 玩家国家称号头衔
        BYTE			m_byGuildTitleSize;					    // 玩家帮会称号长度,不包括最后的'\0'
        CHAR			m_szGuildTitle[MAX_CHARACTER_TITLE];	// 玩家帮会称号头衔
        BYTE			m_byNormalTitleSize;					// 玩家玩法称号头衔长度,不包括最后的'\0'
        CHAR			m_szNormalTitle[MAX_CHARACTER_TITLE];	// 玩家玩法称号头衔

		BYTE			m_CountryTitleType;				
		BYTE			m_GuildTitleType;				
		BYTE			m_NormalTitleType;

		UINT			m_uPlayerData;					//   FFFF|FF|FF
														//        |   |   
														//      头发 脸型
														//      模型 模型

		UINT			m_HairColor;					// 头发颜色

		//摊位状态
		BYTE			m_bStallIsOpen;					// 摊位是否已经打开
		BYTE			m_nStallNameSize;				// 摊位名长度
		CHAR			m_szStallName[MAX_STALL_NAME];	// 摊位名

		BYTE			m_bOpenPublic;					// 摊位是否开市(公开显示摊位牌给其他玩家)	20100706 BLL

		GUID_t			m_OccupantGUID ;
		ObjID_t			m_OwnerID ;
		ObjID_t			m_BusID;
	};


	class GCCharBaseAttribFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCCharBaseAttrib() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_CHARBASEATTRIB; }
		UINT		GetPacketMaxSize()const { return	sizeof(GCCharBaseAttrib) - sizeof(Packet); }
	};

	class GCCharBaseAttribHandler 
	{
	public:
		static UINT Execute( GCCharBaseAttrib* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;



#endif