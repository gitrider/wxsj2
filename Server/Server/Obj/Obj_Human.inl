// Obj_Human.inl

#ifndef _OBJ_HUMAN_INL_
#define _OBJ_HUMAN_INL_

#include "GWGuild.h"
#include "GWCountryAP.h"

AI_Human*				Obj_Human::GetHumanAI( VOID )
{
	return ((AI_Human*)m_pAI_Character);
}

AvoidOverlap*           Obj_Human::GetAvoidOverLap()const
{
	return m_pAvoidOverlap;
}

ObjID_t					Obj_Human::GetAssistantID( VOID ) const
{
	return m_AssistantID;
}

VOID					Obj_Human::SetAssistantID( ObjID_t id )
{
	m_AssistantID = id ;
}

HumanDB*				Obj_Human::GetDB()
{
	return &m_DB;
}

GUID_t					Obj_Human::GetGUID( VOID )const
{
	return m_DB.GetGUID();
}

VOID					Obj_Human::SetGuildID(GuildID_t guildID)
{
	m_DB.SetDBGuildID(guildID);
}

GuildID_t				Obj_Human::GetGuildID( )
{
	return m_DB.GetDBGuildID();
}

VOID					Obj_Human::SetFamilyID(INT nFamilyID)
{
    m_DB.SetDBFamilyID(nFamilyID);
}

INT     				Obj_Human::GetFamilyID( )
{
    return m_DB.GetDBFamilyID();
}

VOID                    Obj_Human::SetGuildOfficial(CHAR cOfficial)
{
    m_DB.SetDBGuildOfficial(cOfficial);
}

CHAR                    Obj_Human::GetGuildOffocial( )
{
    return m_DB.GetDBGuildOfficial();
}

VOID                    Obj_Human::SetGuildPoint(INT nGP)
{
    m_DB.SetDBGuildPoint(nGP);
}

INT                     Obj_Human::GetGuildPoint( )
{
    return m_DB.GetDBGuildPoint();
}

INT                     Obj_Human::GetHonor()
{
    return m_DB.GetDBHonor();
}
VOID                    Obj_Human::SetHonor(INT nHonor)
{
    return m_DB.SetDBHonor(nHonor);
}

VOID 		            Obj_Human::SendMsg_RefeshGP( INT nGP, INT nType )
{
    GUILD_CGW_GP gpInfo;
    gpInfo.m_GP         =   nGP;
    gpInfo.m_GuildGUID  =   GetGuildID();

    GWGuild* pMsg = (GWGuild*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_GUILD);
    pMsg->SetGUID(GetGUID());
    memcpy(&pMsg->GetGuildPacket()->m_PacketLevelInfo, &gpInfo, sizeof(GUILD_CGW_GP));
    pMsg->GetGuildPacket()->m_uPacketType   =   nType;
    g_pServerManager->SendPacket( pMsg, INVALID_ID );
}

VOID 		            Obj_Human::SendMsg_RefeshAP( INT nAP, INT nType )
{
    GWCountryAP* pMsg = (GWCountryAP*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_COUNTRY_AP);

    pMsg->SetGuid(GetGUID());
    pMsg->SetAP(nAP);
    pMsg->SetType(nType);

    g_pServerManager->SendPacket( pMsg, INVALID_ID );
}

BOOL					Obj_Human::GetSex()
{
	return m_DB.GetSex();
}


INT						Obj_Human::GetCountry( )
{
	return m_DB.GetCountry();
}

VOID					Obj_Human::SetCountry( INT nCountry )
{
	m_DB.SetCountry( nCountry );
}

MENPAI_ATTRIBUTE		Obj_Human::GetMenPai( )
{
	return (MENPAI_ATTRIBUTE)m_DB.GetMenPai();
}

VOID					Obj_Human::SetMenPai( INT Menpai )
{
	m_DB.SetMenPai( Menpai );
}

const _SUIT_SETTING& 			Obj_Human::GetEquipSuitSetting( UINT nEquipSuitNum ) const
{
	Assert(nEquipSuitNum<MAX_EQUIP_SUIT_NUM);
	return m_DB.GetSettingDB()->m_SuitSetting[nEquipSuitNum];
}

VOID			Obj_Human::SetEquipSuitSetting( _SUIT_SETTING& nSuitSetting, UINT nEquipSuitNum )
{
	Assert(nEquipSuitNum<MAX_EQUIP_SUIT_NUM);
	memcpy((void*)&GetSettingList()->m_SuitSetting[nEquipSuitNum], (void*)&nSuitSetting, sizeof(_SUIT_SETTING));
}

const _SUBTRAIN_SETTING&		Obj_Human::GetSubTrainSetting( ) const
{
	return m_DB.GetSettingDB()->m_SubTrainSetting;
}

VOID			Obj_Human::SetSubTrainSetting( _SUBTRAIN_SETTING& nSubTrainSetting)
{
	memcpy((void*)&GetSettingList()->m_SubTrainSetting, (void*)&nSubTrainSetting, sizeof(_SUBTRAIN_SETTING));
}

// GM专用
VOID					Obj_Human::SetGod( BYTE bGod )
{
	m_bGod = bGod;
}

BOOL					Obj_Human::IsGod(VOID) const
{
	return (BOOL)m_bGod;
}

VOID					Obj_Human::SetChangeSceneFlag( BYTE bChangeSceneFlag )
{
	m_bChangeSceneFlag = bChangeSceneFlag;
}

BOOL					Obj_Human::IsHaveChangeSceneFlag(VOID) const
{
	return (BOOL)m_bChangeSceneFlag;
}

// 生活技能相关的函数
const _ABILITY_LIST*	Obj_Human::GetAbilityList( ) const
{
	return m_DB.GetAbilityDB();
}

AbilityOpera*			Obj_Human::GetAbilityOpera( )
{
	return &m_LastOpera;
}

VOID					Obj_Human::reset_AbilityOpera( ) 
{
	m_LastOpera.CleanUp();
}

VOID					Obj_Human::Set_RemainPoints(INT Point)
{
	m_DB.SetDBRemainPoints(Point);
}

VOID					Obj_Human::Set_CommonS_SkillPoints(INT nS, INT Point)
{
	Assert((COMMON_S)nS<COMMON_S_NUMBER);
	m_DB.SetDB_S_SkillPoints(nS, Point);
}

VOID				Obj_Human::Set_RemainSkillPoints(INT Point)
{
	m_DB.SetDBRemainSkillPoints(Point);
}

VOID				Obj_Human::Set_S_SkillPoints(INT nS, INT Point)
{
	Assert((COMMON_S)nS<COMMON_S_NUMBER);
	m_DB.SetDB_S_SkillPoints(nS, Point);
}

const CHAR				*Obj_Human::GetName( VOID )const
{
	return m_DB.GetName();
}

const CHAR				*Obj_Human::GetTitle( VOID )const
{
    //ModifyBy AMos
	//return m_DB.GetCurTitle();
    return NULL;
}

const BYTE				Obj_Human::GetTitleType( VOID )const
{
    //ModifyBy AMos
	//return m_DB.GetCurTitleType();
    return 0;
}

INT						Obj_Human::GetLevel( VOID )const
{
	return m_DB.GetHumanDB()->m_Level;
}

INT						Obj_Human::GetPKValue( VOID )const
{
	return m_DB.GetHumanDB()->m_nPKValue;
}

PET_GUID_t				Obj_Human::GetCurrentHorseGUID( VOID )const
{
	return m_DB.GetHumanDB()->m_guidCurrentPet;
}

UINT					Obj_Human::GetExp( )
{
	return m_DB.GetExp();
}

UINT					Obj_Human::GetEquipID(HUMAN_EQUIP equipPoint)
{
	return m_DB.GetEquipID(equipPoint);
}

UINT					Obj_Human::GetEquipLevel(HUMAN_EQUIP equipPoint)
{
	return m_DB.GetEquipLevel(equipPoint);
}

//////////////////////////////////////////////////////////////////////////
//称号
//////////////////////////////////////////////////////////////////////////
VOID                    Obj_Human::AddTitleBuff(INT buffID1, INT buffID2)
{
    if (buffID1 != 0)
    {
        g_ImpactCore.SendImpactToUnit( *this, (ID_t)buffID1, GetID() );
    }

    if (buffID2 != 0)
    {
        g_ImpactCore.SendImpactToUnit( *this, (ID_t)buffID2, GetID() );
    }
}

VOID                    Obj_Human::DelTitleBuff(INT buffID1, INT buffID2)
{
    if (buffID1 != 0)
    {
        Obj_Character::Impact_UnregisterImpactByDataIndex( buffID1 );
    }

    if (buffID2 != 0)
    {
        Obj_Character::Impact_UnregisterImpactByDataIndex( buffID2 );
    }
}

VOID                    Obj_Human::SetCurCountryTitle( INT iTitleID)
{
    INT BuffID1 = 0, BuffID2 = 0;  
    m_AttrBackUp.m_bTitleModified = TRUE;
    m_DB.SetCurCountryTitle(iTitleID, BuffID1, BuffID2);
    AddTitleBuff(BuffID1, BuffID2);
}

VOID                    Obj_Human::SetCurGuildTitle(INT iTitleID)
{
    INT BuffID1 = 0, BuffID2 = 0;  
    m_AttrBackUp.m_bTitleModified = TRUE;
    m_DB.SetCurGuildTitle(iTitleID, BuffID1, BuffID2);
    AddTitleBuff(BuffID1, BuffID2);
}

VOID                    Obj_Human::SetCurNormalTitle(INT iTitleID)
{
    INT BuffID1 = 0, BuffID2 = 0;  
    m_AttrBackUp.m_bTitleModified = TRUE;
    m_DB.SetCurNormalTitle(iTitleID, BuffID1, BuffID2);
    AddTitleBuff(BuffID1, BuffID2);
}

INT                     Obj_Human::GetCurCountryTitle()
{
    return m_DB.GetCurCountryTitle();
}

const CHAR*             Obj_Human::GetTitleNameByID(INT iTitleID, INT iSex )
{
    return m_DB.GetTitleNameByID(iTitleID, iSex);
}

const CHAR*             Obj_Human::GetCurCountryTitleName()
{
    return m_DB.GetCountryTitleName();
}

const CHAR*             Obj_Human::GetCurGuildTitleName()
{
    return m_DB.GetGuildTitleName();
}

const CHAR*             Obj_Human::GetCurNormalTitleName()
{
    return m_DB.GetNormalTitleName();
}

INT                     Obj_Human::GetCurGuildTitle()
{
    return m_DB.GetCurGuildTitle();
}

INT                     Obj_Human::GetCurNormalTitle()
{
    return m_DB.GetCurNormalTitle();
}

VOID                    Obj_Human::RemoveTitle(INT iTitleID)
{
    m_AttrBackUp.m_bTitleModified = TRUE;
    INT nBUffID = m_DB.RemoveTitle(iTitleID);
    if (nBUffID != 0)
    {
        Skill_UnregisterSkill(nBUffID);
    }

    const INT countryTitle   = GetCurCountryTitle();
    const INT guildtitle     = GetCurGuildTitle();
    const INT normalTitle    = GetCurNormalTitle();

    if (GetCurCountryTitle() ==  iTitleID)
    {
        SetCurCountryTitle(INVALID_ID);
    }
    else if (GetCurGuildTitle() == iTitleID)
    {
        SetCurGuildTitle(INVALID_ID);
    }
    else if (GetCurNormalTitle() == iTitleID)
    {
        SetCurNormalTitle(INVALID_ID);
    }
}

VOID                    Obj_Human::SetGuildTitleName(const CHAR *pszTitle, INT iSize)
{
    m_AttrBackUp.m_bTitleModified = TRUE;
    m_DB.SetGuildTitleName(pszTitle, iSize);
}

const CHAR*             Obj_Human::GetGuildTitleName() const
{
    return m_DB.GetGuildTitleName();
}

VOID                    Obj_Human::SetCountryTitleName(const CHAR *pszTitle, INT iSize)
{
    m_AttrBackUp.m_bTitleModified = TRUE;
    m_DB.SetCountryTitleName(pszTitle, iSize);
}

const CHAR*             Obj_Human::GetCountryTitleName() const
{
    return m_DB.GetCountryTitleName();
}

VOID                    Obj_Human::SetNormalTitleName(const CHAR *pszTitle, INT iSize)
{
    m_AttrBackUp.m_bTitleModified = TRUE;
    m_DB.SetNormalTitleName(pszTitle, iSize);
}

const CHAR*             Obj_Human::GetNormalTitleName() const
{
    return m_DB.GetNormalTitleName();
}

WORD					Obj_Human::GetEquipVer( )
{
	return m_EquipVer; 
}

VOID					Obj_Human::SetEquipVer( WORD Ver )
{ 
	m_EquipVer = Ver; 
}

SceneID_t				Obj_Human::GetTransSceneID( )
{
	return m_TransSceneID; 
}

VOID					Obj_Human::SetTransSceneID( SceneID_t id )
{ 
	m_TransSceneID = id; 
}


 TeamInfo*		Obj_Human::GetTeamInfo()
{
	return &m_TeamInfo; 
}

 ItemContainer*	 Obj_Human::GetBaseContain()
{
	return &m_BaseItem;
}

ItemContainer*	 Obj_Human::GetExtraBagContain(INT nIndex)
{
	Assert(nIndex<MAX_EXTRA_BAG_NUM);
	return &m_ExtraBagItem[nIndex];
}

ItemContainer*	 Obj_Human::GetExtraContain()
{
	return &m_ExtraContainerItem;
}

// ItemContainer*			Obj_Human::GetTaskContain()
//{
//	return &m_TaskItem;
//}
//
// ItemContainer*			Obj_Human::GetMatContain()
//{
//	return &m_MatItem;
//}

 ItemContainer*			Obj_Human::GetEquipContain()
{
	return &m_EquipItem;
}

 ItemContainer*			Obj_Human::GetBankContain()
{
	return &m_BankItem;
}

 ItemContainer*			Obj_Human::GetPetContain()
{
	return &m_PetItem;
}

 ItemContainer*			Obj_Human::GetMountContain()  // [7/8/2010 陈军龙]
{
    return &m_MountItem;
}

HumanOpenBox*			Obj_Human::GetHumanOpenBox()
{
	return &m_HumanOpenBox;
}

 HumanRelation*	Obj_Human::GetHumanRelation()
{
	return &m_HumanRelation ;
}

//const UINT *Obj_Human::GetMissionHaveDoneFlags( VOID )const
//{
//	return GetConstMissionList()->m_aMissionHaveDoneFlags;
//}

// 设置是否已经做过了某个任务
VOID Obj_Human::SetMissionHaveDone( MissionID_t idMission, BOOL bHaveDone )
{
__ENTER_FUNCTION
	MissionID_t idIndex = (idMission >> 5);
	Assert( idIndex < MAX_CHAR_MISSION_FLAG_LEN );
	if ( idIndex < MAX_CHAR_MISSION_FLAG_LEN )
	{
		m_DB.SetMissionDone(idMission,idIndex);
		//((GetMissionList()->m_aMissionHaveDoneFlags[idIndex] |= (0x00000001 << ((UINT)idMission & 0x0000001F))) != 0);
		OnMissionHaveDoneFlagsChanged( idMission );

		MISSION_LOG_PARAM sParam ;
		sParam.CharGUID = GetGUID();
		sParam.OPType	= MISSION_OP_DONEFLAG ;
		sParam.SceneID	= getScene()->SceneID() ;
		sParam.XPos		= getWorldPos()->m_fX ;
		sParam.ZPos		= getWorldPos()->m_fZ ;
		sParam.idMission= idMission ;
		sParam.nDataIndex = idIndex ;
		sParam.nDataValue = GetConstMissionList()->m_aMissionHaveDoneFlags[idIndex] ;
		SaveMissionLog(&sParam) ;
	}
__LEAVE_FUNCTION
}

// 是否已经做过了某个任务
BOOL Obj_Human::IsMissionHaveDone( MissionID_t idMission )const
{
__ENTER_FUNCTION
	MissionID_t idIndex = (idMission >> 5);
	Assert( idIndex < MAX_CHAR_MISSION_FLAG_LEN );
	if ( idIndex < MAX_CHAR_MISSION_FLAG_LEN )
		return ((GetConstMissionList()->m_aMissionHaveDoneFlags[idIndex] & (0x00000001 << ((UINT)idMission & 0x0000001F))) != 0);
	else
		return FALSE;
__LEAVE_FUNCTION
	return FALSE ;
}

// 检测是否已经有了这个任务
BOOL Obj_Human::IsHaveMission( MissionID_t idMission )const
{
	return (GetMissionIndexByID( idMission ) != UINT_MAX);
}

// 设置任务参数
VOID Obj_Human::SetMissionParam( UINT uIndexMission, UINT uIndexParam, INT nValue )
{
__ENTER_FUNCTION
	Assert(uIndexMission < MAX_CHAR_MISSION_NUM &&uIndexParam < MAX_MISSION_PARAM_NUM );
	if (uIndexMission < MAX_CHAR_MISSION_NUM &&uIndexParam < MAX_MISSION_PARAM_NUM )
	{
		m_DB.SetMissionParam(uIndexMission,uIndexParam,nValue);
		OnMissionParamChanged(uIndexMission,uIndexParam );
	}
__LEAVE_FUNCTION
}

// 取任务参数
INT Obj_Human::GetMissionParam( UINT uIndexMission, UINT uIndexParam )const
{
__ENTER_FUNCTION
	Assert(uIndexMission < MAX_CHAR_MISSION_NUM &&uIndexParam < MAX_MISSION_PARAM_NUM );
	if (uIndexMission < MAX_CHAR_MISSION_NUM &&uIndexParam < MAX_MISSION_PARAM_NUM )
		return GetConstMissionList()->m_aMission[uIndexMission].m_anParam[uIndexParam];
	else
		return FALSE;
__LEAVE_FUNCTION
	return FALSE ;
}

const _OWN_MISSION *Obj_Human::GetMission( UINT uIndex )const
{
	if (uIndex < MAX_CHAR_MISSION_NUM )
		return &GetConstMissionList()->m_aMission[uIndex];
	else
		return NULL;
}

//UINT Obj_Human::GetCanPickMissionItemCount( VOID )const
//{
//	return m_uCanPickMissionItemIDNum;
//}
//
//const UINT *Obj_Human::GetCanPickMissionItemList( VOID )const
//{
//	return &(m_aCanPickMissionItemList[0]);
//}
//
//VOID Obj_Human::AddCanPickMissionItem( UINT uItemDataID )
//{
//	if ( m_uCanPickMissionItemIDNum < MAX_CHAR_CAN_PICK_MISSION_ITEM_NUM )
//	{
//		m_aCanPickMissionItemList[m_uCanPickMissionItemIDNum++] =uItemDataID;
//		OnAddCanPickMissionItem(uItemDataID );
//	}
//}

INT Obj_Human::GetMissionData( INT nIndex )//读取附加任务数据
{
__ENTER_FUNCTION
	if( nIndex<0 || nIndex>=MAX_CHAR_MISSION_DATA_NUM )
	{
		AssertEx( FALSE, "下标超出范围" ) ;
		return 0 ;
	}
	return GetMissionList()->m_aMissionData[nIndex] ;
__LEAVE_FUNCTION
	return 0 ;
}
VOID Obj_Human::SetMissionData( INT nIndex, INT nData )//设置当前附加任务数据
{
__ENTER_FUNCTION
	if( nIndex<0 || nIndex>=MAX_CHAR_MISSION_DATA_NUM )
	{
		AssertEx( FALSE, "下标超出范围" ) ;
	}
	m_DB.SetMissionData(nIndex,nData);
	OnMissionParamChanged(nIndex,nData,1);
__LEAVE_FUNCTION
}

VOID Obj_Human::SetCurTarget( ObjID_t idTarget )
{
	m_CurTargetID = idTarget;
}

ObjID_t Obj_Human::GetCurTarget( VOID ) const
{
	return m_CurTargetID;
}

#endif
