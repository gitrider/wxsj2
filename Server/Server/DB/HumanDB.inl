#ifndef _HUMAN_DB_INL_
#define _HUMAN_DB_INL_
#include "TimeManager.h"



VOID				HumanDB::Lock()
{
	N_SM_LOCK(SM_S_WRITE)
}

VOID				HumanDB::UnLock()
{
	N_SM_UNLOCK(SM_S_WRITE)
}

BOOL				HumanDB::IsFirstEnterScene()
{
	return m_dbHuman->m_bFirstEnterScence;
}

VOID				HumanDB::ClearFirstEnterScene()
{
	m_dbHuman->m_bFirstEnterScence = FALSE;
}

GUID_t				HumanDB::GetGUID() const
{
	return m_dbHuman->m_GUID ; 
}
VOID				HumanDB::SetGUID( GUID_t guid )
{ 
	m_dbHuman->m_GUID = guid ; 
	SetDBEnumAttrDirty(CATTR_DB_GUID);
} 

INT					HumanDB::GetPortraitID() const
{
	return m_dbHuman->m_PortraitID ; 
}

VOID				HumanDB::SetPortraitID( INT nID )
{
	m_dbHuman->m_PortraitID = nID ; 
	SetDBEnumAttrDirty(CATTR_DB_PORTRAITID);
}

const	CHAR*		HumanDB::GetName()const
{
	return m_dbHuman->m_Name ;
} 
BYTE				HumanDB::GetSex()
{ 
	return m_dbHuman->m_Sex ; 
} 

INT					HumanDB::GetHP()
{
	return m_dbHuman->m_HP ; 
} 
VOID				HumanDB::SetHP( INT hp )
{
	m_dbHuman->m_HP = hp ; 
	SetDBEnumAttrDirty(CATTR_DB_HP);
} 

INT					HumanDB::GetMP()
{
	return m_dbHuman->m_MP ; 
} 
VOID				HumanDB::SetMP( INT mp )
{
	m_dbHuman->m_MP = mp ; 
	SetDBEnumAttrDirty(CATTR_DB_MP);
} 

INT					HumanDB::GetPneuma()
{
	return m_dbHuman->m_nPneuma; 
} 
VOID				HumanDB::SetPneuma( INT pneuma )
{
	m_dbHuman->m_nPneuma = pneuma ; 
	SetDBEnumAttrDirty(CATTR_DB_PNEUMA);
} 

INT					HumanDB::GetStrikePoint(VOID) 
{
	return m_dbHuman->m_StrikePoint;
}
VOID				HumanDB::SetStrikePoint(INT nStrikePoint) 
{
	m_dbHuman->m_StrikePoint = nStrikePoint;
	SetDBEnumAttrDirty(CATTR_DB_STRIKEPOINT);
}

INT					HumanDB::GetVigor(VOID)
{
	return m_dbHuman->m_Vigor;
}
VOID				HumanDB::SetVigor(INT nVigor)
{
	m_dbHuman->m_Vigor = nVigor;
	SetDBEnumAttrDirty(CATTR_DB_VIGOR);
}

INT					HumanDB::GetMaxVigor(VOID)
{
	return m_dbHuman->m_MaxVigor;
}
VOID				HumanDB::SetMaxVigor(INT nMaxVigor)
{
	m_dbHuman->m_MaxVigor = nMaxVigor;
	SetDBEnumAttrDirty(CATTR_DB_MAX_VIGOR);
}

INT					HumanDB::GetVigorRegeneRate(VOID)
{
	return m_dbHuman->m_VigorRegeneRate;
}
VOID				HumanDB::SetVigorRegeneRate(INT nRate)
{
	m_dbHuman->m_VigorRegeneRate = nRate;
	SetDBEnumAttrDirty(CATTR_DB_VIGOR_REGENE_RATE);
}

INT					HumanDB::GetEnergy(VOID)
{
	return m_dbHuman->m_Energy;
}
VOID				HumanDB::SetEnergy(INT nEnergy)
{
	m_dbHuman->m_Energy = nEnergy;
	SetDBEnumAttrDirty(CATTR_DB_ENERGY);
}

INT					HumanDB::GetMaxEnergy(VOID)
{
	return m_dbHuman->m_MaxEnergy;
}
VOID				HumanDB::SetMaxEnergy(INT nMaxEnergy)
{
	m_dbHuman->m_MaxEnergy = nMaxEnergy;
	SetDBEnumAttrDirty(CATTR_DB_MAX_ENERGY);
}

INT					HumanDB::GetEnergyRegeneRate(VOID)
{
	return m_dbHuman->m_EnergyRegeneRate;
}
VOID				HumanDB::SetEnergyRegeneRate(INT nRate)
{
	m_dbHuman->m_EnergyRegeneRate = nRate;
	SetDBEnumAttrDirty(CATTR_DB_ENERGY_REGENE_RATE);
}

INT 				HumanDB::GetRage( VOID )
{ 
	return m_dbHuman->m_Rage; 
}
VOID				HumanDB::SetRage( INT nRage )
{
	m_dbHuman->m_Rage = nRage; 
	SetDBEnumAttrDirty(CATTR_DB_RAGE);
}

BYTE				HumanDB::GetLevel()
{
	return m_dbHuman->m_Level ; 
} 

VOID				HumanDB::SetLevel( BYTE level )
{ 
	m_dbHuman->m_Level = level ; 
	SetDBEnumAttrDirty(CATTR_DB_LEVEL);
}

INT					HumanDB::GetPKValue( VOID )const
{
	return m_dbHuman->m_nPKValue;
}

VOID				HumanDB::SetPKValue( INT nValue )
{
	m_dbHuman->m_nPKValue = nValue;
	SetDBEnumAttrDirty(CATTR_DB_PKVALUE);
}

PET_GUID_t			HumanDB::GetCurrentPetGUID( VOID )const
{
	return m_dbHuman->m_guidCurrentPet;
}

VOID				HumanDB::SetCurrentPetGUID( PET_GUID_t guid )
{
	m_dbHuman->m_guidCurrentPet = guid;
	SetDBEnumAttrDirty(CATTR_DB_CURRENTPET);
}

UINT				HumanDB::GetExp()
{
	return m_dbHuman->m_Exp ; 
} 
VOID				HumanDB::SetExp( UINT exp )
{
	m_dbHuman->m_Exp = exp ; 
	SetDBEnumAttrDirty(CATTR_DB_EXP);
} 

INT					HumanDB::GetCountry( )
{
	return m_dbHuman->m_nCountry;
}

VOID				HumanDB::SetCountry( INT nCountry )
{
	m_dbHuman->m_nCountry = nCountry;
	SetDBEnumAttrDirty(CATTR_DB_COUNTRY);
}

INT					HumanDB::GetMenPai( )
{ 
	return m_dbHuman->m_MenPai ; 
} 
VOID				HumanDB::SetMenPai( INT MenPai )
{
	m_dbHuman->m_MenPai = MenPai ; 
	SetDBEnumAttrDirty(CATTR_DB_MENPAI);
} 

UINT				HumanDB::GetMoney()
{	
	return m_dbHuman->m_Money;
}
VOID				HumanDB::SetMoney(UINT money)
{
	m_dbHuman->m_Money	=	money;
	SetDBEnumAttrDirty(CATTR_DB_MONEY);
}

UINT				HumanDB::GetGold()
{
    return m_dbHuman->m_Gold;
}

VOID				HumanDB::SetGold(UINT gold)
{
    m_dbHuman->m_Gold   =   gold;
}
UINT				HumanDB::GetTicket()
{
    return m_dbHuman->m_Ticket;
}

VOID				HumanDB::SetTicket(UINT ticket)
{
    m_dbHuman->m_Ticket =   ticket;
}

INT					HumanDB::GetGoodBadValue()
{
	return m_dbHuman->m_nGoodBadValue;
}

INT					HumanDB::SetGoodBadValue(INT nValue)
{
	m_dbHuman->m_nGoodBadValue = nValue;
	SetDBEnumAttrDirty(CATTR_DB_GOODBAD);
	return m_dbHuman->m_nGoodBadValue;
}

_PLAYERSHOP_GUID	HumanDB::GetShopGuid(INT nIndex)
{
	return m_dbHuman->m_ShopGuid[nIndex];
}

VOID				HumanDB::SetShopGuid( INT nIndex, _PLAYERSHOP_GUID pGuid )
{
	m_dbHuman->m_ShopGuid[nIndex] = pGuid; 
	SetDBEnumAttrDirty((CHAR_ATTR_DB)(CATTR_DB_PLAYER_SHOP_BEGIN + nIndex));
}

_PLAYERSHOP_GUID	HumanDB::GetFavoriteShopGuid(INT nIndex)
{
	return m_dbHuman->m_FavoriteList[nIndex];
}

VOID				HumanDB::SetFavoriteShopGuid( INT nIndex, _PLAYERSHOP_GUID pGuid )
{
	m_dbHuman->m_FavoriteList[nIndex] = pGuid; 
	SetDBEnumAttrDirty((CHAR_ATTR_DB)(CATTR_DB_PLAYER_SHOP_FAVORITE_BEGIN + nIndex));
}

const CHAR*			HumanDB::GetPasswd()
{
	return m_dbHuman->m_Passwd;
}

VOID				HumanDB::SetPasswd( const CHAR* szPasswd )
{
	strncpy( m_dbHuman->m_Passwd, szPasswd, MAX_PWD - 1 );
	SetDBEnumAttrDirty(CATTR_DB_PASSWORD);
}

VOID				HumanDB::DelPasswd()
{
	memset( m_dbHuman->m_Passwd, 0, MAX_PWD );
	SetDBEnumAttrDirty(CATTR_DB_PASSWORD);
}

UINT				HumanDB::GetPasswdDeleteTime()
{
	return m_dbHuman->m_uPwdDelTime;
}

VOID				HumanDB::SetPasswdDeleteTime( UINT uTime )
{
	m_dbHuman->m_uPwdDelTime;
	SetDBEnumAttrDirty(CATTR_DB_PWDDELTIME);
}

BYTE				HumanDB::GetXinfaLevel(INT nXinfaID)
{
	for(INT i=0; i<m_dbXinFa->m_Count; i++){
		if( m_dbXinFa->m_aXinFa[i].m_nXinFaID == nXinfaID)
			return m_dbXinFa->m_aXinFa[i].m_nLevel;
	}
	return -1;
}

VOID				HumanDB::SetXinfaUplevel(INT nXinfaID, BYTE UplevelNum)
{
	for(INT i=0; i<m_dbXinFa->m_Count; i++){
		if( m_dbXinFa->m_aXinFa[i].m_nXinFaID == nXinfaID)
			m_dbXinFa->m_aXinFa[i].m_nLevel = UplevelNum;
	}

	SetDBEnumAttrDirty(CATTR_DB_XINFA);
}

UINT				HumanDB::GetHairColor(VOID)const
{
	return m_dbHuman->m_HairColor;
}

VOID				HumanDB::SetHairColor(UINT uColor)
{
	m_dbHuman->m_HairColor = uColor;
	SetDBEnumAttrDirty(CATTR_DB_HAIR_COLOR);
}

BYTE				HumanDB::GetFaceColor(VOID)const
{
	return m_dbHuman->m_FaceColor;
}

VOID				HumanDB::SetFaceColor(BYTE byColor)
{
	m_dbHuman->m_FaceColor = byColor;
	SetDBEnumAttrDirty(CATTR_DB_FACE_COLOR);
}

BYTE				HumanDB::GetHairModel(VOID)const
{
	return m_dbHuman->m_HairModel;
}

VOID				HumanDB::SetHairModel(BYTE byModel)
{
	m_dbHuman->m_HairModel = byModel;
	SetDBEnumAttrDirty(CATTR_DB_HAIR_MODEL);
}

BYTE				HumanDB::GetFaceModel(VOID)const
{
	return m_dbHuman->m_FaceModel;
}

VOID				HumanDB::SetFaceModel(BYTE byModel)
{
	m_dbHuman->m_FaceModel = byModel;
	SetDBEnumAttrDirty(CATTR_DB_FACE_MODEL);
}

UINT				HumanDB::GetEquipID(HUMAN_EQUIP	equipPoint)
{
	return m_dbEquip->m_pItem[equipPoint].m_ItemIndex;
}

UINT				HumanDB::GetEquipLevel(HUMAN_EQUIP	equipPoint)
{
	return m_dbEquip->m_pItem[equipPoint].GetEquipData()->m_NeedLevel;
}

INT					HumanDB::GetDBAttrLvl1(CHAR_ATTR_LEVEL1 enAttrType)              //一级属性GET
{
	return	 m_dbHuman->m_BaseAttrLevel1.Get(enAttrType) ;
}
VOID				HumanDB::SetDBAttrLvl1(CHAR_ATTR_LEVEL1 enAttrType,INT iValue)	//一级属性SET
{
		m_dbHuman->m_BaseAttrLevel1.Set(enAttrType,iValue) ;
		SetDBEnumAttrDirty((CHAR_ATTR_DB)(CATTR_DB_STR+enAttrType));
}
INT					HumanDB::GetRemainPoints()
{
	return m_dbHuman->m_Level1Points;
}	//剩余一级属性

INT					HumanDB::GetRemainSkillPoints()
{
	return m_dbHuman->m_RemainSkillPoints;
}	

INT					HumanDB::Get_S_SkillPoints(INT nS)
{
	Assert(nS >= 0 && (COMMON_S)nS<COMMON_S_NUMBER);
	return m_dbHuman->m_S_SkillPoints[nS];
}	

VOID				HumanDB::SetDBRemainPoints(INT Point)
{ 
	m_dbHuman->m_Level1Points=Point;
	SetDBEnumAttrDirty(CATTR_DB_REMAINPOINT);
}

VOID               HumanDB::SetDBRemainSkillPoints(INT Point)
{ 
	m_dbHuman->m_RemainSkillPoints=Point;
	SetDBEnumAttrDirty(CATTR_DB_REMAINSKILLPOINT);
}

VOID				HumanDB::SetDB_S_SkillPoints(INT nS, INT Point)
{ 
	Assert((COMMON_S)nS<COMMON_S_NUMBER);
	m_dbHuman->m_S_SkillPoints[nS]=Point;
	SetDBEnumAttrDirty(CATTR_DB_REMAINSKILLPOINT);
}

VOID				HumanDB::SetMonsterPetAttr( INT iIndex, _Monster_Pet_DB_Struct* pMonsterPet )
{
	//不需要写数据库，不置脏标记
	Assert( iIndex>=0 && iIndex<MAX_MONSTER_PET_TAKE );
	m_dbMonsterPet->m_aMonsterPet[iIndex] = *pMonsterPet;
}

VOID				HumanDB::SetHorseMattingFlag( BOOL bFlag )
{
	m_dbHuman->m_bHorseMating = bFlag;
	SetDBEnumAttrDirty(CATTR_DB_HORSE_MATTING_FLAG);
}

const _CAMP_DATA *HumanDB::GetDBCampData(VOID)const
{
	return &(m_dbHuman->m_CampData);
}
VOID				HumanDB::SetDBCampData(const _CAMP_DATA *pCampData)
{
	m_dbHuman->m_CampData	=	*pCampData;
	SetDBEnumAttrDirty(CATTR_DB_CAMP);
}
VOID				HumanDB::SetDBStartScene(SceneID_t	sceneID)
{
	m_dbHuman->m_StartScene	=	sceneID;
	SetDBEnumAttrDirty(CATTR_DB_AT_SCENE);
}
SceneID_t			HumanDB::GetDBBakScene( )
{
	return		m_dbHuman->m_BakScene ;
}
VOID				HumanDB::SetDBBakScene(SceneID_t	sceneID)
{
	m_dbHuman->m_BakScene	=	sceneID;
	SetDBEnumAttrDirty(CATTR_DB_BK_SCENE);
}


//////////////////////////////////////////////////////////////////////////
//称号
//////////////////////////////////////////////////////////////////////////
const _TITLE*       HumanDB::GetTitleInfo() const
{
    return &m_dbHuman->m_Titles;
}

const CHAR*       HumanDB::GetTitleNameByID( INT iTitleID, INT iSex ) 
{
    return NULL;
}

BOOL                HumanDB::UpdateTitleTime()
{
    for (INT i=0; i<MAX_TITLE_SIZE; ++i)
    {
        if (m_dbHuman->m_Titles.m_TitleArray[i].m_iTitleID != INVALID_ID)
        {
            INT iTime = g_TitleInfoTbl.m_TableInfo[i].m_uTime;
            if (iTime > 0)//计时称号
            {
                if ((UINT)g_pTimeManager->GetANSITime() >= m_dbHuman->m_Titles.m_TitleArray[i].m_uTime) 
                {
                    RemoveTitle(i);
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}

VOID                HumanDB::AddTitle(INT iTitleID, INT &BuffID1, INT &BuffID2 )
{
    m_dbHuman->m_Titles.m_TitleArray[iTitleID].m_iTitleID   = g_TitleInfoTbl.m_TableInfo[iTitleID].m_iTitleID;
    m_dbHuman->m_Titles.m_TitleArray[iTitleID].m_iTitleType = g_TitleInfoTbl.m_TableInfo[iTitleID].m_iTitleType;
    m_dbHuman->m_Titles.m_TitleArray[iTitleID].m_iBuffID    = g_TitleInfoTbl.m_TableInfo[iTitleID].m_iBuffID;
    m_dbHuman->m_Titles.m_TitleArray[iTitleID].m_iSuitID    = g_TitleInfoTbl.m_TableInfo[iTitleID].m_iSuitID;
    BuffID1 = m_dbHuman->m_Titles.m_TitleArray[iTitleID].m_iBuffID;


    INT iTime = g_TitleInfoTbl.m_TableInfo[iTitleID].m_uTime;
    if (iTime > 0)//计时称号
    {
        m_dbHuman->m_Titles.m_TitleArray[iTitleID].m_uTime  = (UINT)iTime * 60 + (UINT)g_pTimeManager->GetANSITime();
    }

    INT iSuitID = m_dbHuman->m_Titles.m_TitleArray[iTitleID].m_iSuitID;
    bool bSuitable = TRUE;
    if (iSuitID != INVALID_TITLE_ID)
    {
        INT comTitleID = g_TitleComTbl.m_TableCombination[iSuitID].m_comTitleID;

        INT iGroupID = g_TitleComTbl.m_TableCombination[iSuitID].m_iGroupID;
        for (INT i=0; i<MAX_TITLE_COMBINATION; ++i)
        {
            INT part = g_TitleComTbl.m_TableCombination[iSuitID].m_arPart[i];

            Assert(part < MAX_TITLE_SIZE);
            if (part == INVALID_TITLE_ID)
                continue;

            if (m_dbHuman->m_Titles.m_TitleArray[part].m_iTitleID == INVALID_TITLE_ID)
            {
                bSuitable = false;
                break;
            }
        }

        if (bSuitable)//组合ID都有了，生成新称号
        {
            INT iNewTitle = g_TitleComTbl.m_TableCombination[iSuitID].m_comTitleID;
            m_dbHuman->m_Titles.m_TitleArray[comTitleID].m_iTitleID   = g_TitleInfoTbl.m_TableInfo[iNewTitle].m_iTitleID;
            m_dbHuman->m_Titles.m_TitleArray[comTitleID].m_iTitleType = g_TitleInfoTbl.m_TableInfo[iNewTitle].m_iTitleType;
            m_dbHuman->m_Titles.m_TitleArray[comTitleID].m_uTime      = g_TitleInfoTbl.m_TableInfo[iNewTitle].m_uTime;
            m_dbHuman->m_Titles.m_TitleArray[comTitleID].m_iBuffID    = g_TitleInfoTbl.m_TableInfo[iNewTitle].m_iBuffID;
            m_dbHuman->m_Titles.m_TitleArray[comTitleID].m_iSuitID    = g_TitleInfoTbl.m_TableInfo[iNewTitle].m_iSuitID;

            BuffID2 = m_dbHuman->m_Titles.m_TitleArray[comTitleID].m_iBuffID;
        }
    }
}

VOID			    HumanDB::SetCurCountryTitle( INT iTitleID, INT &BuffID1, INT &BuffID2 )
{
    AssertEx(iTitleID < MAX_TITLE_SIZE ,"国家称号ID越界");
    //新增title
    if (INVALID_TITLE_ID == m_dbHuman->m_Titles.m_TitleArray[iTitleID].m_iTitleID)
    {
        AddTitle(iTitleID, BuffID1, BuffID2);
    }
    m_dbHuman->m_Titles.m_CurCountryTitle = iTitleID;

    SetDBEnumAttrDirty(CATTR_DB_TITLEINFO);
}

VOID                HumanDB::SetCurGuildTitle(INT iTitleID, INT &BuffID1, INT &BuffID2 )
{
    AssertEx(iTitleID < MAX_TITLE_SIZE ,"帮会称号ID越界");
    //新增title
    if (INVALID_TITLE_ID == m_dbHuman->m_Titles.m_TitleArray[iTitleID].m_iTitleID)
    {
        AddTitle(iTitleID, BuffID1, BuffID2);
    }
    m_dbHuman->m_Titles.m_CurGuildTitle = iTitleID;

    SetDBEnumAttrDirty(CATTR_DB_TITLEINFO);
}

VOID                HumanDB::SetCurNormalTitle(INT iTitleID, INT &BuffID1, INT &BuffID2 )
{
    AssertEx(iTitleID < MAX_TITLE_SIZE ,"功能称号ID越界");
    //新增title
    if (INVALID_TITLE_ID == m_dbHuman->m_Titles.m_TitleArray[iTitleID].m_iTitleID)
    {
        AddTitle(iTitleID, BuffID1, BuffID2);
    }
    m_dbHuman->m_Titles.m_CurNormalTitle = iTitleID;

    SetDBEnumAttrDirty(CATTR_DB_TITLEINFO);
}

INT				    HumanDB::GetCurCountryTitle()
{
    return m_dbHuman->m_Titles.m_CurCountryTitle;
}

INT                 HumanDB::GetCurGuildTitle()
{
    return m_dbHuman->m_Titles.m_CurGuildTitle;
}

INT                 HumanDB::GetCurNormalTitle()
{
    return m_dbHuman->m_Titles.m_CurNormalTitle;
}

INT                 HumanDB::RemoveTitle(INT iTitleID)
{
    INT buffId1=0, buffId2=0;
    if (GetCurCountryTitle() == iTitleID)
    {
        SetCurCountryTitle(INVALID_ID, buffId1, buffId2);
    }

    if (GetCurGuildTitle() == iTitleID)
    {
        SetCurGuildTitle(INVALID_ID, buffId1, buffId2);
    }


    if (GetCurNormalTitle() == iTitleID)
    {
        SetCurNormalTitle(INVALID_ID, buffId1, buffId2);
    }

    m_dbHuman->m_Titles.m_TitleArray[iTitleID].m_iTitleID   = INVALID_ID;
    m_dbHuman->m_Titles.m_TitleArray[iTitleID].m_iTitleType = INVALID_ID;
    m_dbHuman->m_Titles.m_TitleArray[iTitleID].m_uTime      = INVALID_ID;
    m_dbHuman->m_Titles.m_TitleArray[iTitleID].m_iBuffID    = INVALID_ID;
    m_dbHuman->m_Titles.m_TitleArray[iTitleID].m_iSuitID    = INVALID_ID;

    return m_dbHuman->m_Titles.m_TitleArray[iTitleID].m_iBuffID;
}

VOID                HumanDB::SetOfficalTitleName(const CHAR *pszTitle, INT iSize)
{
    Assert(iSize<MAX_NICK_NAME);
    memset(m_dbHuman->m_Titles.m_szOfficialTitleName, 0, MAX_NICK_NAME);
    memcpy(m_dbHuman->m_Titles.m_szOfficialTitleName, pszTitle, iSize);

    SetDBEnumAttrDirty(CATTR_DB_TITLEINFO);
}

const CHAR*         HumanDB::GetOfficalTitleName() const
{
    return m_dbHuman->m_Titles.m_szOfficialTitleName;
}

VOID                HumanDB::SetCountryTitleName(const CHAR *pszTitle, INT iSize)
{
    Assert(iSize<MAX_NICK_NAME);
    memset(m_dbHuman->m_Titles.m_szCurCountryTitle, 0, MAX_NICK_NAME);
    memcpy(m_dbHuman->m_Titles.m_szCurCountryTitle, pszTitle, iSize);

    SetDBEnumAttrDirty(CATTR_DB_TITLEINFO);
}

const CHAR*         HumanDB::GetCountryTitleName() const
{
    return m_dbHuman->m_Titles.m_szCurCountryTitle;
}

VOID                HumanDB::SetGuildTitleName(const CHAR *pszTitle, INT iSize)
{
    Assert(iSize<MAX_NICK_NAME);
    memset(m_dbHuman->m_Titles.m_szCurGuildTitle, 0, MAX_NICK_NAME);
    memcpy(m_dbHuman->m_Titles.m_szCurGuildTitle, pszTitle, iSize);

    SetDBEnumAttrDirty(CATTR_DB_TITLEINFO);
}

const CHAR*         HumanDB::GetGuildTitleName() const
{
    return m_dbHuman->m_Titles.m_szCurGuildTitle;
}

VOID                HumanDB::SetNormalTitleName(const CHAR *pszTitle, INT iSize)
{
    Assert(iSize<MAX_NICK_NAME);
    memset(m_dbHuman->m_Titles.m_szCurNormalTitle, 0, MAX_NICK_NAME);
    memcpy(m_dbHuman->m_Titles.m_szCurNormalTitle, pszTitle, iSize);

    SetDBEnumAttrDirty(CATTR_DB_TITLEINFO);
}

const CHAR*         HumanDB::GetNormalTitleName() const
{
    return m_dbHuman->m_Titles.m_szCurNormalTitle;
}

//////////////////////////////////////////////////////////////////////////
//银行
//////////////////////////////////////////////////////////////////////////

const INT			HumanDB::GetBankItemCount( ) const
{
	return m_dbBank->m_Count;
}
VOID				HumanDB::SetBankItemCount( const INT Bankitemcount )
{
	Assert(Bankitemcount<MAX_BANK_SIZE);
	m_dbBank->m_Count = Bankitemcount;
}
const _ITEM*		HumanDB::GetBankItem( const INT bankindex ) const
{
	Assert(bankindex<MAX_BANK_SIZE);
	return &m_dbBank->m_pItem[bankindex];
}
VOID				HumanDB::SetBankItem( const INT bankindex, const _ITEM* item )
{
	Assert(bankindex<MAX_BANK_SIZE);
	memcpy(&m_dbBank->m_pItem[bankindex], item, sizeof(_ITEM));
}
const INT			HumanDB::GetBankEndIndex( ) const
{
	return m_dbBank->m_CurEndIndex;
}
VOID				HumanDB::SetBankEndIndex( const INT bankindex )
{
	m_dbBank->m_CurEndIndex = bankindex;
	SetDBEnumAttrDirty(CATTR_DB_BANK_OFFSIZE);
}
const INT			HumanDB::GetBankMoney( ) const
{
	return m_dbBank->m_Money;
}
VOID				HumanDB::SetBankMoney( const INT money )
{
	m_dbBank->m_Money = money;
	SetDBEnumAttrDirty(CATTR_DB_BANK_MONEY);
}

const INT			HumanDB::GetBankSize( ) const
{
    GetBankEndIndex();
}

VOID				HumanDB::ExpandBankSize(UINT uBanksize)
{
    INT nSize = uBanksize+GetBankEndIndex();
    Assert(nSize <= MAX_BANK_SIZE);
    SetBankEndIndex(nSize);
}

VOID				HumanDB::EraseBankItem(UINT uBankIndex)
{
	memset(_GetBankItem(uBankIndex),0,sizeof(_ITEM));
}
VOID				HumanDB::SwapBankWithBank(UINT iBankIndexFr, UINT iBankIndexTo )
{
	Assert(iBankIndexFr<MAX_BAG_SIZE);
	Assert(iBankIndexTo<MAX_BANK_SIZE );
	SwapItem(&m_dbBank->m_pItem[iBankIndexFr],&m_dbBank->m_pItem[iBankIndexTo]);
}


VOID				HumanDB::SwapBankWithBag(UINT iBagIndex, UINT iBankIndex )
{
	Assert(iBagIndex<MAX_BAG_SIZE);
	Assert(iBankIndex<MAX_BANK_SIZE );
	SwapItem(&m_dbBank->m_pItem[iBankIndex],&m_dbBag->m_pItem[iBagIndex]);
}
VOID				HumanDB::SwapBankWithEquip( HUMAN_EQUIP	equipPoint, UINT iBankIndex )
{
	Assert(HEQUIP_NUMBER>equipPoint);
	Assert(iBankIndex<MAX_BANK_SIZE);
	SwapItem(&m_dbEquip->m_pItem[equipPoint],&m_dbBank->m_pItem[iBankIndex]);
}

VOID				HumanDB::OverWriteBag2Bank(UINT iBagIndex, UINT iBankIndex)
{
	Assert(iBagIndex<MAX_BAG_SIZE);
	Assert(iBankIndex<MAX_BANK_SIZE);
	
	OverWriteItem(&m_dbBag->m_pItem[iBagIndex], &m_dbBank->m_pItem[iBankIndex]);
}

VOID				HumanDB::OverWriteEquip2Bank(HUMAN_EQUIP	equipPoint, UINT iBankIndex)
{
	Assert(HEQUIP_NUMBER>equipPoint);
	Assert(iBankIndex<MAX_BANK_SIZE);
	OverWriteItem(&m_dbEquip->m_pItem[equipPoint] ,&m_dbBank->m_pItem[iBankIndex]);
}

VOID				HumanDB::OverWriteBank2Bag(UINT iBagIndex, UINT iBankIndex)
{
	Assert(iBagIndex<MAX_BAG_SIZE);
	Assert(iBankIndex<MAX_BANK_SIZE);
	OverWriteItem(&m_dbBank->m_pItem[iBankIndex], &m_dbBag->m_pItem[iBagIndex]);
}

VOID				HumanDB::OverWriteBank2Equip(HUMAN_EQUIP	equipPoint, UINT iBankIndex)
{
	Assert(HEQUIP_NUMBER>equipPoint);
	Assert(iBankIndex<MAX_BANK_SIZE);
	OverWriteItem(&m_dbBank->m_pItem[iBankIndex], &m_dbEquip->m_pItem[equipPoint] );
}




//position 不实时写数据库
VOID				HumanDB::SetDBPosition(const WORLD_POS& pos)
{
	m_dbHuman->m_Position	=	pos;
	//SetDBEnumAttrDirty(CATTR_DB_AT_POSITION);
}

VOID				HumanDB::SetDBBakPosition(const WORLD_POS& pos)
{
	m_dbHuman->m_BakPosition = pos;
}

inline  const WORLD_POS*	HumanDB::GetDBBakPosition()
{
	return &m_dbHuman->m_BakPosition;
}
//玩家logout 时调用的函数
VOID				HumanDB::WriteDBPosition(WORLD_POS& pos)
{
	m_dbHuman->m_Position	=	pos;
	SetDBEnumAttrDirty(CATTR_DB_AT_POSITION);
}

VOID				HumanDB::SetOnlineTime(UINT uTime)	//设置总在线时间
{
	m_dbHuman->m_OnlineTime = uTime;
	SetDBEnumAttrDirty(CATTR_DB_ONLINETIME);
}
UINT				HumanDB::GetOnlineTime()			//取得总在线时间	
{
	return m_dbHuman->m_OnlineTime;
}
VOID				HumanDB::SetLoginTime(UINT uTime)	//设置最后登录时间
{
	m_dbHuman->m_LastLoginTime = uTime;
	SetDBEnumAttrDirty(CATTR_DB_LASTLOGINTIME);
}
UINT				HumanDB::GetLoginTime()				//取得最后登录时间
{
	return m_dbHuman->m_LastLoginTime;
}
VOID				HumanDB::SetLogoutTime(UINT uTime)	//设置最后下线时间
{
	m_dbHuman->m_LastLogoutTime = uTime;
	SetDBEnumAttrDirty(CATTR_DB_LASTLOGOUTTIME);
}
UINT				HumanDB::GetLogoutTime()			//取得最后下线时间
{
	return m_dbHuman->m_LastLogoutTime;
}

VOID				HumanDB::SetLeaveGuildTime(time_t uTime)	//设置离开帮会时间
{
    m_dbHuman->m_LeaveGuildTime = uTime;
    SetDBEnumAttrDirty(CATTR_DB_LEAVEGUILDTIME);
}
time_t				HumanDB::GetLeaveGuildTime()			//取得离开帮会时间
{
    return m_dbHuman->m_LeaveGuildTime;
}


inline  VOID		HumanDB::SetDBVersion(UINT Ver)		//设置数据库版本
{
	m_dbHuman->m_DBVersion = Ver;
	SetDBEnumAttrDirty(CATTR_DB_DBVERSION);
}
inline  UINT		HumanDB::GetDBVersion()				//取得数据库版本
{
	return m_dbHuman->m_DBVersion;
}

VOID				HumanDB::SetDBGuid(GUID_t	guid)
{
	m_dbHuman->m_GUID	=	guid;
	SetDBEnumAttrDirty(CATTR_DB_GUID);
}

GuildID_t			HumanDB::GetDBGuildID( )
{
	return m_dbHuman->m_GuildID ;
}

VOID				HumanDB::SetDBGuildID( GuildID_t guildID )
{
	m_dbHuman->m_GuildID	=	guildID;
	SetDBEnumAttrDirty(CATTR_DB_GUILDID);
}

INT                 HumanDB::GetDBFamilyID( )
{
    return m_dbHuman->m_FamilyID;
}

VOID				HumanDB::SetDBFamilyID( INT nFamilyID )
{
    m_dbHuman->m_FamilyID	=	nFamilyID;
    SetDBEnumAttrDirty(CATTR_DB_FAMILYID);
}

INT                 HumanDB::GetDBGuildOfficial( )
{
    return m_dbHuman->m_nGuildOfficial;   
}

VOID				HumanDB::SetDBGuildOfficial( INT nOfficial )
{
    m_dbHuman->m_nGuildOfficial =   nOfficial;
    SetDBEnumAttrDirty(CATTR_DB_GUILDOFFICIAL);
}

INT                 HumanDB::GetDBGuildPoint( )
{
    return m_dbHuman->m_nGP;
}

VOID				HumanDB::SetDBGuildPoint( INT nGP )
{
    m_dbHuman->m_nGP            =   nGP;
    SetDBEnumAttrDirty(CATTR_DB_GUILDPOINT);
}

INT                 HumanDB::GetDBHonor( )
{
    return m_dbHuman->m_nHonor;
}
VOID				HumanDB::SetDBHonor( INT nHonor )
{
    m_dbHuman->m_nHonor =   nHonor;
    SetDBEnumAttrDirty(CATTR_DB_HONOR);
}

TeamID_t			HumanDB::GetDBTeamID( )
{
	return m_dbHuman->m_TeamID ;
}

VOID				HumanDB::SetDBTeamID( TeamID_t teamID )
{
m_dbHuman->m_TeamID	=	teamID;
	SetDBEnumAttrDirty(CATTR_DB_TEAMID);
}

INT                 HumanDB::GetMountID( )               // [7/8/2010 陈军龙]
{
    return m_dbHuman->m_nMountID;
}

VOID				HumanDB::SetMountID( INT nID )       // [7/8/2010 陈军龙]
{
    m_dbHuman->m_nMountID	=	nID;
    //SetDBEnumAttrDirty(CATTR_DB_TEAMID);
}

INT                 HumanDB::GetMountIndex( )            // [7/8/2010 陈军龙]
{
	return m_dbHuman->m_nMountIndex;
}

VOID				HumanDB::SetMountIndex( INT nIndex ) // [7/8/2010 陈军龙]
{
	m_dbHuman->m_nMountIndex	=	nIndex;
	//SetDBEnumAttrDirty(CATTR_DB_TEAMID);
}

//////////////////////////////////////////////////////////////////////////
//背包数据访问方法		
//////////////////////////////////////////////////////////////////////////
VOID				HumanDB::EraseBagItem(uint BagIndex)
{
	Assert(BagIndex<MAX_BAG_SIZE);
	_ITEM* pItem = _GetBagItem(BagIndex);
	Assert(pItem);
	memset(pItem, 0,sizeof(_ITEM));
}



//////////////////////////////////////////////////////////////////////////
VOID				HumanDB::SwapEquipBag(HUMAN_EQUIP	equipPoint,uint iBagIndex)
{
	Assert(HEQUIP_NUMBER>equipPoint);
	Assert(iBagIndex<MAX_BAG_SIZE);
	SwapItem(&m_dbEquip->m_pItem[equipPoint],&m_dbBag->m_pItem[iBagIndex]);
}

VOID				HumanDB::OverWriteBag2Equip(uint iBagIndex,HUMAN_EQUIP	equipPoint)
{
	Assert(HEQUIP_NUMBER>equipPoint);
	Assert(iBagIndex<MAX_BAG_SIZE);
	
	OverWriteItem(&m_dbBag->m_pItem[iBagIndex], &m_dbEquip->m_pItem[equipPoint]);
}

VOID				HumanDB::OverWriteEquip2Bag(HUMAN_EQUIP	equipPoint,uint iBagIndex)
{
	Assert(HEQUIP_NUMBER>equipPoint);
	Assert(iBagIndex<MAX_BAG_SIZE);
	OverWriteItem(&m_dbEquip->m_pItem[equipPoint] ,&m_dbBag->m_pItem[iBagIndex]);
}


VOID				HumanDB::PutBagDB(const _ITEM* pItem,uint iBagIndex)
{
	m_dbBag->PutBagDB(pItem,iBagIndex);
}

VOID				HumanDB::SwapBagDB(uint iAIndex,uint iBIndex)
{
	m_dbBag->SwapBagDB(iAIndex,iBIndex);
}

BOOL				HumanDB::IncBagItemCount(uint iBagIndex)
{
	return m_dbBag->IncCount(iBagIndex);
}

BOOL				HumanDB::DecBagItemCount(uint iBagIndex)
{
	
	return m_dbBag->DecCount(iBagIndex);
}

VOID				HumanDB::SetItemBind(uint iBagIndex,BOOL	bBind)
{
	m_dbBag->SetItemBind(iBagIndex,bBind);
}

const _ITEM*		HumanDB::GetBagItem(uint iBagIndex) const
{
	
	return m_dbBag->GetBagDB(iBagIndex);
}

const	_ITEM*		HumanDB::GetEquipItem(HUMAN_EQUIP equipPoint)
{
	return &m_dbEquip->m_pItem[equipPoint];
}

const	_ITEM*		HumanDB::GetMountItem(UINT iMountIndex)
{
	return &m_dbMount->m_pItem[iMountIndex];
}

VOID				HumanDB::EraseEquipItem(HUMAN_EQUIP equipPoint)
{
	m_dbEquip->m_pItem[equipPoint].CleanUp();
}

VOID	        	HumanDB::EraseMountItem(UINT iMountIndex)
{
	m_dbMount->m_pItem[iMountIndex].CleanUp();
}

VOID				HumanDB::SetEquipMark(HUMAN_EQUIP eEquipPoint, BYTE bMark)
{
	m_dbEquip->Set(eEquipPoint,bMark);
}

VOID				HumanDB::SetEquipDur(HUMAN_EQUIP eEquipPoint, INT Dur)
{
	m_dbEquip->SetEquipDur(eEquipPoint,Dur);
}

VOID				HumanDB::SetItemDur(uint iBagIndex, INT Dur)
{
	
	m_dbBag->SetItemDur(iBagIndex,Dur);
}


inline	UINT		HumanDB::GetBaseBagSize()
{
	return	MAX_SINGLEBAG_SIZE;
}

inline UINT			HumanDB::GetExtraContainerSize()
{
	return MAX_EXTRA_BAG_NUM;
}

inline	UINT		HumanDB::GetExtraBagSize(UINT nIndex)
{
	_ITEM* pExtraBagItem = _GetBagItem(EXTRA_CONTAINER_OFFSET+nIndex);
	Assert(pExtraBagItem);
	if (pExtraBagItem->IsNullType())
	{
		return 0;
	}
	Assert(pExtraBagItem->ItemClass() == ICLASS_EXTRABAG);
	return pExtraBagItem->GetExtraBagData()->m_nSize;
}

//inline	UINT		HumanDB::GetTaskBagSize()
//{
//	return m_dbBag->m_TaskBagSize;
//}
//inline	UINT		HumanDB::GetMatBagSize()
//{
//	return m_dbBag->m_MatBagSize;
//}


inline _ITEM*		HumanDB::_GetBagItem(UINT uBagIndex)
{
	return	m_dbBag->m_pItem+uBagIndex;
}
inline _ITEM*		HumanDB::_GetBankItem(UINT uBankIndex)
{
	Assert(uBankIndex<MAX_BANK_SIZE);
	return	m_dbBank->m_pItem+uBankIndex;
}
inline VOID			HumanDB::SetCooldown(CooldownID_t nID,Time_t nTime)
{
	m_dbCooldown->RegisterCooldown(nID, nTime);
	SetDBEnumAttrDirty(CATTR_DB_COOLDOWN);
}
inline BOOL			HumanDB::IsCooldowned(CooldownID_t nID) const
{
	return m_dbCooldown->IsSpecificSlotCooldownedByID(nID);
}
inline VOID			HumanDB::HeartBeat_Cooldown(Time_t nDeltaTime)
{
	//这里不加脏标记
	m_dbCooldown->HeartBeat(nDeltaTime);
}
inline Time_t		HumanDB::GetCooldownRemain(CooldownID_t nID) const
{
	return m_dbCooldown->GetRemainTimeByID(nID);
}
inline BYTE		HumanDB::GetSkillCount()
{
	return m_dbSkill->m_Count;
}
inline VOID		HumanDB::SetSkillCount(BYTE Count)
{
	Assert(Count<=MAX_CHAR_SKILL_NUM);
	m_dbSkill->m_Count	=	Count;
	SetDBEnumAttrDirty(CATTR_DB_SKILL);
}
inline _OWN_SKILL& HumanDB::GetSkillID(BYTE Index)
{
	Assert(Index<MAX_CHAR_SKILL_NUM);
	return m_dbSkill->m_aSkill[Index];
}
inline VOID		HumanDB::SetSkillID(const _OWN_SKILL& oSkill,BYTE Index)
{
	Assert(Index<MAX_CHAR_SKILL_NUM);
	m_dbSkill->m_aSkill[Index]	=	oSkill;
	SetDBEnumAttrDirty(CATTR_DB_SKILL);
}

inline VOID		HumanDB::SetSkillLevel(const INT nID, BYTE Index, const INT nLevel)
{
	Assert(Index<MAX_CHAR_SKILL_NUM);
	m_dbSkill->m_aSkill[Index].m_nLevel	=	nLevel;
	if(nLevel>0)// temp for test [6/13/2008 wuwenbin]
		m_dbSkill->m_aSkill[Index].m_eState = _OWN_SKILL::OWN_SKILL_CAN_USE;
	SetDBEnumAttrDirty(CATTR_DB_SKILL);
}

inline BYTE		HumanDB::GetXinFaCount()
{
	return m_dbXinFa->m_Count;
}
inline VOID		HumanDB::SetXinFaCount(BYTE Count)
{
	m_dbXinFa->m_Count	=	Count;
	SetDBEnumAttrDirty(CATTR_DB_XINFA);
}

inline SkillID_t HumanDB::GetXinfaID(BYTE Index)
{
	Assert(Index<MAX_CHAR_XINFA_NUM);
	return m_dbXinFa->m_aXinFa[Index].m_nXinFaID; 

}
inline VOID		HumanDB::SetXinfaID(SkillID_t nID,BYTE Index)
{
	Assert(Index<MAX_CHAR_XINFA_NUM);
	m_dbXinFa->m_aXinFa[Index].m_nXinFaID	=	nID;
	SetDBEnumAttrDirty(CATTR_DB_XINFA);
}
inline BYTE	HumanDB::GetXinfaLevel(BYTE Index)
{
	Assert(Index<MAX_CHAR_XINFA_NUM);
	return m_dbXinFa->m_aXinFa[Index].m_nLevel;
}
inline VOID	HumanDB::SetXinfaLevel(BYTE Level,BYTE Index)
{
	Assert(Index<MAX_CHAR_XINFA_NUM);
	m_dbXinFa->m_aXinFa[Index].m_nLevel	=	Level;	
	SetDBEnumAttrDirty(CATTR_DB_XINFA);
}
inline VOID HumanDB::SetMissionDone(MissionID_t idMission,MissionID_t idIndex)
{
	((m_dbMission->m_aMissionHaveDoneFlags[idIndex] |= (0x00000001 << ((UINT)idMission & 0x0000001F))) != 0);
	SetDBEnumAttrDirty(CATTR_DB_MISSION);
}

inline VOID HumanDB::SetMissionParam(UINT uIndexMission, UINT uIndexParam, INT nValue )
{
	m_dbMission->m_aMission[uIndexMission].m_anParam[uIndexParam] = nValue;
	SetDBEnumAttrDirty(CATTR_DB_MISSION);
}
inline VOID	HumanDB::SetMissionData( INT nIndex, INT nData )
{
	m_dbMission->m_aMissionData[nIndex] = nData ;
	SetDBEnumAttrDirty(CATTR_DB_MISSION);
}

inline VOID HumanDB::SetMission(UINT uIndex,MissionID_t idMission,ScriptID_t idScript)
{
	m_dbMission->m_aMission[uIndex].m_idMission = idMission;
	m_dbMission->m_aMission[uIndex].m_idScript  = idScript;
	SetDBEnumAttrDirty(CATTR_DB_MISSION);
}

inline VOID	HumanDB::SetMissionCount(UINT uCount)
{
	m_dbMission->m_Count = uCount;
	SetDBEnumAttrDirty(CATTR_DB_MISSION);
}

inline VOID	HumanDB::SetFlags_KillObject(UINT uIndex, BOOL bSet )
{
	m_dbMission->m_aMission[uIndex].SetFlags_KillObject(bSet);
	SetDBEnumAttrDirty(CATTR_DB_MISSION);
}
inline VOID	HumanDB::SetFlags_EnterArea(UINT uIndex, BOOL bSet )
{
	m_dbMission->m_aMission[uIndex].SetFlags_EnterArea(bSet);
	SetDBEnumAttrDirty(CATTR_DB_MISSION);
}
inline VOID	HumanDB::SetFlags_ItemChanged(UINT uIndex, BOOL bSet )
{
	m_dbMission->m_aMission[uIndex].SetFlags_ItemChanged(bSet);
	SetDBEnumAttrDirty(CATTR_DB_MISSION);
}
inline VOID	HumanDB::SetFlags_PetChanged(UINT uIndex, BOOL bSet )
{
	m_dbMission->m_aMission[uIndex].SetFlags_PetChanged(bSet);
	SetDBEnumAttrDirty(CATTR_DB_MISSION);
}
inline VOID HumanDB::ClearMission(UINT uIndex)
{
	m_dbMission->m_aMission[uIndex].Cleanup();
	m_dbMission->m_Count--;
}

//////////////////////////////////////////////////////////////////////////
//关系数据访问方法
//////////////////////////////////////////////////////////////////////////

VOID				HumanDB::SetRelation(INT Index, const _OWN_RELATION* pRelation)
{
	Assert(pRelation);
	Assert(Index>=0 && Index<MAX_RELATION_SIZE);
	m_dbRelation->m_aRelation[Index].Init(pRelation);
	SetDBEnumAttrDirty(CATTR_DB_RELATION);
}

VOID				HumanDB::ModifyFriendType( INT nIndex, RELATION_TYPE RelationType )
{
	m_dbRelation->m_aRelation[nIndex].m_Type = RelationType;
	SetDBEnumAttrDirty(CATTR_DB_RELATION);
}

VOID				HumanDB::UpdateRelationInfo(INT Index, const RELATION_MEMBER* pMemberInfo )
{
	Assert(pMemberInfo);
	Assert(Index>=0 && Index<MAX_RELATION_SIZE);
	memcpy( (void*)&(m_dbRelation->m_aRelation[Index].m_Member), pMemberInfo, sizeof(RELATION_MEMBER) );
	SetDBEnumAttrDirty(CATTR_DB_RELATION);
}

VOID				HumanDB::SetMood(const CHAR* szMood)
{
	Assert( szMood );
	strncpy( m_dbRelation->m_szMood, szMood, sizeof(m_dbRelation->m_szMood)-1 );
	SetDBEnumAttrDirty(CATTR_DB_RELATION);
}

VOID				HumanDB::SetName(const INT index,const CHAR* szName)
{
	Assert( szName );
	Assert( index>=0&&index<RELATION_GROUP_F5);
	strncpy( m_dbRelation->m_szName[index], szName, MAX_GROUP_NAME-1 );
	SetDBEnumAttrDirty(CATTR_DB_RELATION);
}

VOID				HumanDB::ModifyFriendPoint(INT Index, INT nFriendPoint)
{
	Assert(Index>=0 && Index<MAX_RELATION_SIZE);
	m_dbRelation->m_aRelation[Index].m_FriendPoint += nFriendPoint;
	SetDBEnumAttrDirty(CATTR_DB_RELATION);
}

VOID				HumanDB::AdjustRelationType(INT Index, UCHAR uType)
{
	Assert(Index>=0 && Index<MAX_RELATION_SIZE);
	m_dbRelation->m_aRelation[Index].m_Type = uType;
	SetDBEnumAttrDirty(CATTR_DB_RELATION);
}

VOID				HumanDB::AdjustRelationGroup(INT Index, UCHAR uGroup)
{
	Assert(Index>=0 && Index<MAX_RELATION_SIZE);
	m_dbRelation->m_aRelation[Index].m_Group = uGroup;
	SetDBEnumAttrDirty(CATTR_DB_RELATION);
}

VOID				HumanDB::DeleteRelation(INT Index)
{
	Assert(Index>=0 && Index<MAX_RELATION_SIZE);
	m_dbRelation->m_aRelation[Index].CleanUp();
	SetDBEnumAttrDirty(CATTR_DB_RELATION);
}

VOID				HumanDB::IncRelationCount(INT Index)
{
	Assert(Index>=0 && Index<RELATION_GROUP_NUMBER);
	++(m_dbRelation->m_aRelationCount[Index]);
	SetDBEnumAttrDirty(CATTR_DB_RELATION);
}

VOID				HumanDB::DecRelationCount(INT Index)
{
	Assert(Index>=0 && Index<RELATION_GROUP_NUMBER);
	--(m_dbRelation->m_aRelationCount[Index]);
	SetDBEnumAttrDirty(CATTR_DB_RELATION);
}

VOID				HumanDB::SetSpouse(const MarriageInfo* pMarriageInfo)
{
	Assert(pMarriageInfo);
	m_dbRelation->m_MarriageInfo.m_SpouseGUID = pMarriageInfo->m_SpouseGUID;
	SetDBEnumAttrDirty(CATTR_DB_RELATION);
}

VOID				HumanDB::AddPrentice(GUID_t guid)
{
	Assert(m_dbRelation->m_PrenticeInfo.m_uPrenticeCount<=MAX_PRENTICE_COUNT);

	for( INT i=0; i<MAX_PRENTICE_COUNT; ++i )
	{
		if(m_dbRelation->m_PrenticeInfo.m_PrenticeGUID[i] == INVALID_ID)
		{
			m_dbRelation->m_PrenticeInfo.m_PrenticeGUID[i] = guid;
			++(m_dbRelation->m_PrenticeInfo.m_uPrenticeCount);
			break;
		}
	}

	SetDBEnumAttrDirty(CATTR_DB_RELATION);
}

VOID				HumanDB::DelPrentice(GUID_t guid)
{
	Assert(m_dbRelation->m_PrenticeInfo.m_uPrenticeCount>0);

	for( INT i=0; i<MAX_PRENTICE_COUNT; ++i )
	{
		if(m_dbRelation->m_PrenticeInfo.m_PrenticeGUID[i] == guid)
		{
			m_dbRelation->m_PrenticeInfo.m_PrenticeGUID[i] = INVALID_ID;
			--(m_dbRelation->m_PrenticeInfo.m_uPrenticeCount);
			break;
		}
	}

	SetDBEnumAttrDirty(CATTR_DB_RELATION);
}

VOID				HumanDB::SetMoralPoint(UINT uPoint)
{
	Assert(uPoint <= MAX_MORAL_POINT);
	m_dbRelation->m_PrenticeInfo.m_uMoralPoint = uPoint;
	SetDBEnumAttrDirty(CATTR_DB_RELATION);
}

VOID				HumanDB::SetBetrayTime(time_t BetrayTime)
{
	m_dbRelation->m_PrenticeInfo.m_BetrayingTime = BetrayTime;
	SetDBEnumAttrDirty(CATTR_DB_RELATION);
}

VOID				HumanDB::SetMaster(const MasterInfo* pMasterInfo)
{
	Assert(pMasterInfo);
	m_dbRelation->m_MasterInfo.m_MasterGUID = pMasterInfo->m_MasterGUID;
	SetDBEnumAttrDirty(CATTR_DB_RELATION);
}

VOID				HumanDB::SetAbilityLevel(AbilityID_t abilityid,INT lvl)
{
	if( abilityid<1 || abilityid>MAX_CHAR_ABILITY_NUM )
	{
		return;
	}

	m_dbAbility->Get_Ability(abilityid).m_Level = lvl;

	SetDBEnumAttrDirty(CATTR_DB_ABILITY);
}

VOID				HumanDB::SetAbilityExp(AbilityID_t abilityid,INT exp)
{
	if( abilityid<1 || abilityid>MAX_CHAR_ABILITY_NUM )
	{
		return;
	}

	m_dbAbility->Get_Ability(abilityid).m_Exp = exp;
	SetDBEnumAttrDirty(CATTR_DB_ABILITY);
}

VOID				HumanDB::SetPrescrLearnedFlag(const PrescriptionID_t prescrid, BOOL flag )
{
	__ENTER_FUNCTION

	INT idx;
	idx = (INT)prescrid;
	if( idx<0 || idx>=MAX_ABILITY_PRESCRIPTION_NUM )
	{
		Assert(FALSE);
		return;
	}

	if ( flag != FALSE )
	{ // 学会
		m_dbAbility->m_aPrescr[idx>>3] |= (0x01<<idx%8);
	}
	else
	{ // 放弃
		m_dbAbility->m_aPrescr[idx>>3] &= ~(0x01<<idx%8);
	}

	__LEAVE_FUNCTION
}


inline	const _PET_DB_LOAD*	HumanDB::GetPet(uint	iIndex)	const 
{
		Assert(iIndex<HUMAN_PET_MAX_COUNT);
		return &m_dbPetList->m_aPetDB[iIndex];
}

inline VOID			HumanDB::_RegDBAttr(CHAR_ATTR_DB eOffSet,VOID* pArg,INT iSize)
{
	Assert(eOffSet<CATTR_DB_NUMBER);
	Assert(eOffSet>=0);
	Assert(pArg);
	Assert(iSize);
	Assert(m_AttrReg);
	m_AttrReg->m_AttrPtr[eOffSet] = pArg;
	m_AttrReg->m_AttrSize[eOffSet] = iSize;
}

inline VOID			HumanDB::_RegSMAttr(CHAR_ATTR_DB eOffSet,VOID* pArg,INT iSize)
{
	Assert(eOffSet<CATTR_DB_NUMBER);
	Assert(eOffSet>=0);
	Assert(pArg);
	Assert(iSize);
	Assert(m_AttrRegSM);
	m_AttrRegSM->m_AttrPtr[eOffSet] = pArg;
	m_AttrRegSM->m_AttrSize[eOffSet] = iSize;
}

inline VOID			HumanDB::SetSettingData(INT nType, _OWN_SETTING* Value )
{
	if( nType<0 || nType>=SETTING_TYPE_NUMBER )
		return ;

	m_dbSetting->m_aSetting[nType] = *Value ;
	SetDBEnumAttrDirty(CATTR_DB_SETTING);
}

#define SETTINGFLAGISTRUE(Setting, FlagSet) (Setting&(1<<FlagSet))
inline VOID			HumanDB::SetPrivateInfo(_PRIVATE_INFO_DB_LOAD* pPrivateInfo)
{
	__ENTER_FUNCTION
		
		Assert(pPrivateInfo);
		memcpy(m_dbPrivateInfo,pPrivateInfo,sizeof(_PRIVATE_INFO_DB_LOAD));
		SetDBEnumAttrDirty(CATTR_DB_PRIVATEINFO);

	__LEAVE_FUNCTION

}

inline const _OWN_SETTING* HumanDB::GetSetting( INT nType ) const
{
	if( nType<0 || nType>=SETTING_TYPE_NUMBER )
		return NULL ;

	return &(m_dbSetting->m_aSetting[nType]) ;
}

inline	INT					HumanDB::GetReserve(INT index)
{
	Assert(index>=0);
	Assert(index<MAX_RESERVE);
	return	m_dbHuman->m_Reserve[index];
}
inline VOID						HumanDB::SetReserve(INT index,INT value)
{
	Assert(index>=0);
	Assert(index<MAX_RESERVE);
	m_dbHuman->m_Reserve[index] = value;
	SetDBEnumAttrDirty(CATTR_DB_RESERVE);
}


const	_BAG_DB_LOAD*			HumanDB::GetBag()
{
	return m_dbBag;
}
const	_HUMAN_DB_LOAD*			HumanDB::GetHumanDB()	const
{
	return	m_dbHuman;
}
const	_EQUIP_DB_LOAD*			HumanDB::GetEquipDB()
{
	return	m_dbEquip;
}
const	_BANK_DB_LOAD*			HumanDB::GetBankDB()
{
	return m_dbBank;
}

const _SKILL_DB_LOAD*			HumanDB::GetSkillDB()
{
	return m_dbSkill;
}

const _MONSTER_PET_DB*			HumanDB::GetMonsterPetDB() const
{
	return m_dbMonsterPet;
}

const _COOLDOWN_DB_LOAD_FOR_HUMAN* HumanDB::GetCooldownDB() const
{
	return m_dbCooldown;	
}

const	_RELATION_DB_LOAD*		HumanDB::GetRelationDB() const
{
	return m_dbRelation;
}
const _ABILITY_DB_LOAD*	HumanDB::GetAbilityDB() const
{
	return m_dbAbility;
}
const _XINFA_DB_LOAD*			HumanDB::GetXinFaDB() const
{
	return m_dbXinFa;
}

const _IMPACT_DB_LOAD*		HumanDB::GetImpactDB() const
{
	return m_dbImpact;
}

const _MISSION_DB_LOAD*		HumanDB::GetMissionDB() const
{
	return m_dbMission;
}

const _SETTING_DB_LOAD*		HumanDB::GetSettingDB() const
{
	return m_dbSetting;
}

const	_PRIVATE_INFO_DB_LOAD*	HumanDB::GetPrivateInfoDB() const
{
	return m_dbPrivateInfo;
}

const	_MOUNT_DB_LOAD*			HumanDB::GetMountDB()  const
{
	return	m_dbMount;
}

VOID				HumanDB::WriteHumanDB(const	_HUMAN_DB_LOAD* pHumanDB)
{
	memcpy(m_dbHuman,pHumanDB,sizeof(_HUMAN_DB_LOAD));
}

VOID				HumanDB::WriteBagDB(const	_BAG_DB_LOAD* pBagDB)
{
	memcpy(m_dbBag,pBagDB,sizeof(_BAG_DB_LOAD));
}

VOID				HumanDB::WriteEquipDB(const	_EQUIP_DB_LOAD*	pEquipDB)
{
	memcpy(m_dbEquip,pEquipDB,sizeof(_EQUIP_DB_LOAD));
}


VOID				HumanDB::WriteBankDB(const	_BANK_DB_LOAD*	pBankDB)
{
	memcpy(m_dbBank,pBankDB,sizeof(_BANK_DB_LOAD));					
}

VOID				HumanDB::WriteSkillDB(const _SKILL_DB_LOAD* pSkillDB)
{
	memcpy(m_dbSkill,pSkillDB,sizeof(_SKILL_DB_LOAD));					
}

VOID				HumanDB::WriteCooldownDB(const _COOLDOWN_DB_LOAD_FOR_HUMAN* pCooldownDB)
{
	memcpy(m_dbCooldown,pCooldownDB,sizeof(_COOLDOWN_DB_LOAD_FOR_HUMAN));					
}

VOID				HumanDB::WriteRelationDB(const _RELATION_DB_LOAD* pRelationDB)
{
	memcpy(m_dbRelation, pRelationDB, sizeof(_RELATION_DB_LOAD));					
}

VOID				HumanDB::WriteAbilityDB(const _ABILITY_DB_LOAD* pAbilityDB)
{
	memcpy(m_dbAbility,pAbilityDB,sizeof(_ABILITY_DB_LOAD));
}

VOID				HumanDB::WriteXinFaDB(const _XINFA_DB_LOAD* pXinFaDB)
{
	memcpy(m_dbXinFa,pXinFaDB,sizeof(_XINFA_DB_LOAD));
}


VOID				HumanDB::WriteImpactDB(const _IMPACT_DB_LOAD* pImpactDB)
{
	memcpy(m_dbImpact,pImpactDB,sizeof(_IMPACT_DB_LOAD));
}

VOID				HumanDB::WriteMissionDB(const _MISSION_DB_LOAD* pMissionDB)
{
	memcpy(m_dbMission,pMissionDB,sizeof(_MISSION_DB_LOAD));
}

VOID				HumanDB::WriteSettingDB(const _SETTING_DB_LOAD* pSettingDB)
{
	memcpy(m_dbSetting,pSettingDB,sizeof(_SETTING_DB_LOAD));
}
VOID			    HumanDB::WritePrivateInfo(const _PRIVATE_INFO_DB_LOAD* pPrivateDB)
{
	memcpy(m_dbPrivateInfo,pPrivateDB,sizeof(_PRIVATE_INFO_DB_LOAD));
}

VOID				HumanDB::WriteMountDB(const _MOUNT_DB_LOAD* pMountDB)
{
    memcpy(m_dbMount,pMountDB,sizeof(_MOUNT_DB_LOAD));
}

// add by gh 2010/05/10
SOUXIA_DATA*	HumanDB::GetSouXiaInfo(SHORT index)
{
	Assert(NULL != m_dbSouXia);
	Assert(index<MAX_SOUXIA_CONTAINER && index >= 0);
	return		&(m_dbSouXia->m_SouXiaData[index]);
}

VOID	HumanDB::WriteSouXiaDB(const _SOUXIA_DB_LOAD* pSouXiaDB)
{
	memcpy(m_dbSouXia,pSouXiaDB,sizeof(_SOUXIA_DB_LOAD));
}

const	_SOUXIA_DB_LOAD*		HumanDB::GetSouXiaDB() const
{
	return m_dbSouXia;
}

VOID	HumanDB::SetSouXiaDBInfo(_SOUXIA_DB_LOAD* pSouXiaInfo, SHORT offSet)
{
	__ENTER_FUNCTION
	Assert(pSouXiaInfo);
	Assert(offSet >= 0);
	memcpy((SOUXIA_DATA*)&(m_dbSouXia->m_SouXiaData[offSet]), (SOUXIA_DATA*)&(pSouXiaInfo->m_SouXiaData[offSet]), sizeof(SOUXIA_DATA));
	SetDBEnumAttrDirty((CHAR_ATTR_DB)(CATTR_DB_SOUXIA_BEGIN+offSet)); 
	__LEAVE_FUNCTION
}

// end of add
#endif

