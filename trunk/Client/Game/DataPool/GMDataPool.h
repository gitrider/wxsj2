
/** ���ݳ�
 *
 *	���ܣ�	
 *
 *	�޸ļ�¼��
 *			080307	��������Ϣ
 *			080314	Relation_GetFriendByName
 *					Relation_GetBlackByName
 *			080318	Relation_ClearEnemy
 *					Relation_AddEnemy
 *					Relation_DelEnemy
 *					Relation_GetEnemyNum
 *					Relation_SetEnemyNum
 *					Relation_GetEnemy
 *					Relation_GetEnemyByName
 *					m_uEnemyCount
 *					m_EnemyInfo
 *
 *					m_FingerPlayerNum
 *					m_FingerPlayerList
 *			080331	��ӳƺ���صĺ���
 *			080409  ��ҵ���װ���� ����
 *			080429	SystemTip_GetIndexByType
 *			
 */

#pragma once


#include "GIDataPool.h"
#include "GMDP_Struct_Pet.h"
#include "GameStruct_Script.h"
#include "GameStruct_Pet.h"
#include "GMDP_Struct_Guild.h"
#include "GMDP_Struct_RecordList.h"
#include "GMDP_Struct_PlayerShop.h"
#include "GMDP_Struct_Relation.h"
#include "GIDBC_Struct.h"
#include "GMDP_Struct_Title.h"
#include "GMDP_Struct_SystemTip.h"
#include "GMDP_Struct_Country.h"



class CObject;
class CObject_Character;
class CCharacterData;
class CObject_PlayerNPC;
enum HUMAN_EQUIP;
class tObject_Item;
struct _ITEM;
struct _BUFF_IMPACT_INFO;
struct X_PARAM;
struct X_SCRIPT;
struct EVENT;
class Relation;
class CMailPool;
struct _TITLE_;
struct SOUXIA_DATA;


class CDataPool : public tDataPool
{
public:

	static void WINAPI _Skill_OnUpdatePlayerAttrib(const EVENT* pEvent, UINT dwOwnerData);
	
	//---------------------------------------------
	//�����ӿ�
	//---------------------------------------------
public:

	/************************************************************************/
	/*		CopyTextToClipboardd
	/*		��һ���ı����ݿ�����������
	/************************************************************************/
	BOOL CopyTextToClipboard(HWND hWnd,LPCSTR content);

	BOOL CopyTextFromClipboard(HWND hWnd, LPCSTR pBuf );

	/// �̵�����
	enum TYPE_PLAYER_SHOP
	{
		TYPE_PLAYER_SHOP_INVALID = 0,		/// ��Ч��𣬻�û�з����ȥ
		TYPE_PLAYER_SHOP_ITEM,				/// ��Ʒ��
		TYPE_PLAYER_SHOP_GEM,				/// ��ʯ��
		TYPE_PLAYER_SHOP_WEAPON,			/// ������
		TYPE_PLAYER_SHOP_ARMOR,				/// ���׵�
		TYPE_PLAYER_SHOP_MATERIAL,			/// ���ϵ�
		TYPE_PLAYER_SHOP_PET,				/// �����
	};

	/// ��ȴ��
	struct COOLDOWN_GROUP
	{
		INT		nTime;
		INT		nTotalTime;
		INT 	nTimeElapsed;
	};

public:

/** ��ɫ���Դ���/����
 */
	CCharacterData* CharacterData_Create(CObject_Character* pCharacterData);
	void			CharacterData_Destroy(CObject_Character* pCharacterData);
	CCharacterData*	CharacterData_Get(INT nID);


/** ��ȴ��
 */

	/** ȡ����ȴ�� */
	virtual const COOLDOWN_GROUP*	CoolDownGroup_Get(INT nCoolDownID) const;
	/** ������ȴ�� */
	virtual VOID					CoolDownGroup_UpdateList(const Cooldown_T *pCoolDownList, INT nUpdateNum);
	/** ȡ�ó�����ȴ�� */
	virtual const COOLDOWN_GROUP*	PetSkillCoolDownGroup_Get(INT nCoolDownID) const;
	/** ���³�����ȴ�� */
	virtual VOID					PetSkillCoolDownGroup_UpdateList(const Cooldown_T *pCoolDownList, INT nUpdateNum,const PET_GUID_t nPet_GUID);


/** ������ȴ�� 
 */
	virtual const INT				CommonCoolDown_Get(VOID) const { return m_nCommonCoolDown; }
	virtual VOID					CommonCoolDown_Update(VOID) { m_nCommonCoolDown = COMMONCOOLDOWN_TIME;}
	

/** ����ʱ��
 */

	/** ȡ������ʱ�� */
	virtual const INT				QuestTimeGroup_Get(INT nQuestTimeID) const;
	/** ��������ʱ�� */
	virtual VOID					QuestTimeGroup_UpdateList(const INT nQuestTime, INT nUpdateNum);
	

/** ���װ��ϵ��
 */
	
	/** ��� */
	virtual VOID			UserEquip_Clear(VOID);
	/** ����װ�� */
	virtual VOID			UserEquip_SetItem(HUMAN_EQUIP ptEquip, tObject_Item* pItem, BOOL bClearOld=TRUE);
	/** ȡ��װ�� */
	virtual tObject_Item*	UserEquip_GetItem(HUMAN_EQUIP ptEquip);
	/** ��װ�Ƿ���ϳɹ�,������װ����  */
	virtual INT				UserEquip_IsUnion(VOID);

	/**�����Ƿ���ϳɹ�,	�ֲ��ļ��� */
	virtual BOOL			UserEquipSprite_IsActive( EM_EQUIP_CARD_GROUP index); 

	/**�����Ƿ���ϳɹ�,	�󼤻� */
	virtual BOOL			UserEquipSprite_IsActiveAll( ); 
	/**�����Ƿ���ϳɹ�,	�󼤻�  �жϳ��˽������4���*/
	virtual BOOL			UserEquipSprite_ActiveFour(INT index ); 


/** �������ϵ�� [7/8/2010 �¾���] 
*/
	/** ��� */
	virtual VOID            UserMount_Clear(VOID);
	/** �������� */
	virtual VOID            UserMount_SetItem(INT nIndex, tObject_Item* pMountItem, BOOL bClearOld);
	/** ȡ������ */
	virtual	tObject_Item*   UserMount_GetItem(INT nIndex);
	/** ȡ������λ�� */
	virtual INT             UserMount_GetItemIndex(tObject_Item* itemMount);


/** ����װ��ϵ��
 */
	
	/** ��� */
	virtual VOID			OtherPlayerEquip_Clear(VOID);
	/** ����װ�� */
	virtual VOID			OtherPlayerEquip_SetItem(HUMAN_EQUIP ptEquip, tObject_Item* pItem, BOOL bClearOld=TRUE);
	/** ȡ��װ�� */
	virtual tObject_Item*	OtherPlayerEquip_GetItem(HUMAN_EQUIP ptEquip);
	/** ��װ�Ƿ���ϳɹ�,������װ����  */
	virtual INT				OtherPlayerEquip_IsUnion(VOID);


	/**�����Ƿ���ϳɹ�,	�ֲ��ļ��� */
	virtual BOOL			OtherUserEquipSprite_IsActive( EM_EQUIP_CARD_GROUP index); 

	/**�����Ƿ���ϳɹ�,	�󼤻� */
	virtual BOOL			OtherUserEquipSprite_IsActiveAll( ); 

	/**�����Ƿ���ϳɹ�,	�󼤻�  �жϳ��˽������4���*/
	virtual BOOL			OtherUserEquipSprite_ActiveFour(INT index ); 



/** ����¼����
/
	/** ��� */
	virtual VOID			UserSouXia_Clear(VOID);
	
	/** ��������¼������ϸ���� */
	virtual VOID			UserSouXia_SetInfo( INT nSouXiaIndex, SOUXIA_DATA* pInfo );

	/** ���ָ��λ�õ�����¼����������Ϣ */
	virtual VOID			UserSouXia_ClearInfo( INT nSouXiaIndex );

	/* ȡ����ϸ��Ϣ */
	virtual SOUXIA_DATA*		UserSouXia_GetInfo( INT nSouXiaIndex );

	/* ѧϰ��������¼���� */
	virtual VOID			UserSouXia_SetCount( BYTE nCount ){ m_nSouXiaCount = nCount; };
	virtual BYTE			UserSouXia_GetCount() { return m_nSouXiaCount; };

	/* ������¼���е��������� */
	virtual VOID			UserSouXia_SetTypeCount( BYTE nCount ) { m_nSouXiaTypeCount = nCount; };
	virtual BYTE			UserSouXia_GetTypeCount() { return m_nSouXiaTypeCount; };



/** ������Ʒ
 */

	/** ��� */
	virtual VOID			UserBag_Clear(VOID);
	/** ������Ʒ */
	virtual VOID			UserBag_SetItem(INT nBagIndex, tObject_Item* pItem, BOOL bClearOld=TRUE, BOOL reCalulate=FALSE);
	/** ����������Ʒ���� */
	virtual VOID			UserBag_SetItemInfo(INT nBagIndex, BOOL bEmpty, const _ITEM* pItem);
	/** ȡ��װ�� */
	virtual tObject_Item*	UserBag_GetItem(INT nBagIndex);
	/** ͨ��GUIDȡ��װ��λ�� */
	virtual INT				UserBag_GetItemIndexByGUID(__int64	 ItemGUID);
	/** ͨ��IDȡ����Ʒλ�� */
	virtual INT				UserBag_GetItemIndexByID( INT nID );
	/** ����װ����ϸ��Ϣ */
	virtual VOID			UserBag_AskExtraInfo(INT nBagIndex);
	/** ͨ��GUIDȡ��װ�������� */
	virtual INT				UserBag_CountItemByIDTable(INT	 ItemGUID);

	// �����Ŀ�¡��Ŀǰֻ����ʵ���������Ĺ���
	struct USER_BAG_CLONE
	{
		tObject_Item*					m_pItem;
		INT								m_nItemNum;
	};
	std::vector< USER_BAG_CLONE >		m_vtUserBagClone;
	//------------
	// Clone������Ʒ
	//------------
	// ������Ʒ
	VOID					UserBagClone_SetItem(INT nBagIndex, CDataPool::USER_BAG_CLONE* pCloneObj);
	// �����Ʒ
	const USER_BAG_CLONE*	UserBagClone_GetItem(INT nBagIndex) const;
	// �ѱ����ڵ���ƷClone������
	VOID					UserBagClone_Clone(VOID);
	// ���
	VOID					UserBagClone_Clear(VOID);



/** �򿪵�����
 */

	/** ��� */
	virtual VOID			ItemBox_Clear(VOID);
	/** ��������ID */
	virtual VOID			ItemBox_SetID(INT id)		{ m_idItemBox = id; }
	/** ȡ������ID */
	virtual INT				ItemBox_GetID(VOID) const	{ return m_idItemBox; }
	/** ����������Ʒ */
	virtual VOID			ItemBox_SetItem(INT nBoxIndex, tObject_Item* pItem, BOOL bClearOld=TRUE);
	/** ȡ��������Ʒ */
	virtual tObject_Item*	ItemBox_GetItem(INT nBoxIndex);
	/** ����IDȡ����Ʒ */
	virtual tObject_Item*	ItemBox_GetItem(WORD idWorld, WORD idServer, INT idSerial, INT& nIndex);
	/** ����Ŀǰ��������Ʒ���� */
	virtual INT				ItemBox_GetNumber(VOID);

//�������Ʒ


	/** ��� */
	virtual VOID			ItemDrop_Clear(VOID);
   // ����ض���
	virtual VOID			ItemDrop_Destroy(INT id);

	/** ���õ�����ID */
	virtual VOID			ItemDrop_SetID(INT id)		{ m_idItemDrop = id; }
	/** ȡ�õ�����ID */
	virtual INT				ItemDrop_GetID(VOID) const	{ return m_idItemDrop; }

	/** ������Ʒ */
	virtual VOID			ItemDrop_SetItem(INT nDropObjId, tObject_Item* pItem, BOOL bClearOld=TRUE);

	/** ȡ��������Ʒ */
	virtual tObject_Item*	ItemDrop_GetItem(INT nDropObjId);

	/** ����Ŀǰ��Ʒ���� */
	virtual INT				ItemDrop_GetNumber(VOID);
    //����ʱ���tick
	VOID					ItemDrop_Tick(VOID);

/** �̵�
 */

	virtual VOID				Booth_Clear( VOID );
	virtual VOID				Booth_Sold_Clear( VOID );
	virtual VOID				Booth_SetItem(INT nBoothIndex, tObject_Item* pItem);
	virtual VOID				Booth_SetItemPrice(INT nBoothIndex, UINT nPrice);
	virtual VOID				Booth_SetSoldItem(INT nBoothIndex, tObject_Item* pItem);
	virtual VOID				Booth_SetNumber(INT nBoothNumber)			{ m_nBoothNumber = nBoothNumber; }
	virtual VOID				Booth_SetSoldNumber(INT nBoothNumber)		{ m_nBoothSoldNumber = nBoothNumber; }
	virtual tObject_Item*		Booth_GetItem(INT nBoothIndex);
	virtual UINT				Booth_GetItemPrice(INT nBoothIndex);

	virtual tObject_Item*		Booth_GetSoldItem(INT nBoothIndex);
	virtual	tObject_Item*		Booth_GetItemByID(INT IDtable);
	virtual INT					Booth_GetNumber(VOID) const					{ return m_nBoothNumber; }
	virtual INT					Booth_GetSoldNumber(VOID) const				{ return m_nBoothSoldNumber ; }
//	virtual	BOOL				Booth_IsCursorRepairing(VOID)	const		{return	m_bIsRepairing;}
//	virtual	VOID				Booth_SetCursorRepairing(BOOL flag)			{m_bIsRepairing = flag;}
	virtual	BOOL				Booth_IsClose(VOID)	const					{return	m_bBoothWindowIsDirty;}
	virtual VOID				Booth_Open(VOID)							{m_bBoothWindowIsDirty = FALSE;}
	virtual VOID				Booth_Close(VOID)							{m_bBoothWindowIsDirty = TRUE;}
	virtual VOID				Booth_SetBuyType(INT nBuyType)				{m_nBuyType = nBuyType;}
	virtual INT					Booth_GetBuyType(VOID)						{return m_nBuyType;}
	virtual VOID				Booth_SetRepairType(INT nRepairType)		{m_nRepairType = nRepairType;}
	virtual INT					Booth_GetRepairType(VOID)					{return m_nRepairType;}

	virtual VOID				Booth_SetRepairLevel(INT nRepairLevel)		{ m_nRepairLevel = nRepairLevel; }
	virtual INT					Booth_GetRepairLevel(VOID)					{ return m_nRepairLevel; }
	virtual VOID				Booth_SetBuyLevel(INT nBuyLevel)			{ m_nBuyLevel = nBuyLevel; }
	virtual INT					Booth_GetBuyLevel(VOID)						{ return m_nBuyLevel; }
	virtual VOID				Booth_SetRepairSpend(FLOAT nRepairSpend)	{ m_nRepairSpend = nRepairSpend; }
	virtual FLOAT				Booth_GetRepairSpend(VOID)					{ return m_nRepairSpend; }
	virtual VOID				Booth_SetRepairOkProb(FLOAT nRepairOkProb)	{ m_nRepairOkProb = nRepairOkProb; }
	virtual FLOAT				Booth_GetRepairOkProb(VOID)					{ return m_nRepairOkProb; }

	virtual VOID				Booth_SetShopNpcId(INT nShopNpcId)			{ m_nShopNpcId = nShopNpcId; }
	virtual INT					Booth_GetShopNpcId(VOID)					{ return m_nShopNpcId; }

	virtual BOOL				Booth_IsCanRepair(tObject_Item* pItem);
	virtual BOOL				Booth_IsCanBuy(tObject_Item* pItem);
	
	//  IB SHOP 
	virtual VOID				IBShop_Clear( VOID );
	virtual VOID				IBShop_SetItem(INT nIBShopIndex, tObject_Item* pItem);
	virtual VOID				IBShop_SetItemPrice(INT nIBShopIndex, UINT nPrice);
	virtual VOID				IBShop_SetNumber(INT nIBShopNumber)			{ m_nIBShopNumber = nIBShopNumber; }
	virtual tObject_Item*		IBShop_GetItem(INT nIBShopIndex);
	virtual UINT				IBShop_GetItemPrice(INT nIBShopIndex);

	virtual	tObject_Item*		IBShop_GetItemByID(INT IDtable);
	virtual INT					IBShop_GetNumber(VOID) const				{ return m_nIBShopNumber; }
	virtual VOID				IBShop_SetItemIsNewSell(INT nIBShopIndex, INT isNew);

/** �������ݵķ���
 */

	virtual VOID				UserBank_Clear(VOID);
	virtual VOID				UserBank_SetItem(INT nBankIndex, tObject_Item* pItem, BOOL bClearOld=TRUE);
	virtual tObject_Item*		UserBank_GetItem(INT nBankIndex);
	virtual	VOID				UserBank_SetBankEndIndex(INT endindex)		{ m_CurBankEndIndex = endindex; }
	virtual	INT					UserBank_GetBankEndIndex()					{ return m_CurBankEndIndex; }
	virtual	VOID				UserBank_SetBankMoney(INT Money)			{ m_CurBankMoney = Money; }
	virtual	INT					UserBank_GetBankMoney()						{ return m_CurBankMoney; }
	virtual	VOID				UserBank_SetItemExtraInfo(INT nBankIndex, BOOL bEmpty, const _ITEM* pItem);
	/** ��ѯ����nIndex��ŵ����������ǲ����пո� */
	virtual BOOL				UserBank_IsEmpty(INT nIndex);
	virtual VOID				UserBank_SetNpcId(INT nNpcId)				{ m_nBankNpcID = nNpcId; }
	virtual INT					UserBank_GetNpcId(VOID)						{ return m_nBankNpcID; }


/** ��ϵϵͳ����
 */

	virtual Relation*			GetRelation()	{ return m_pRelation; }
	virtual CMailPool*			GetMailPool()	{ return m_pMailPool; }


/** ���׺����ݷ���
 */
	
	virtual VOID				MyExBox_Clear(VOID);
	virtual VOID				MyExBox_SetItem(INT nExBoxIndex, tObject_Item* pItem, BOOL bClearOld=TRUE);
	virtual tObject_Item*		MyExBox_GetItem(INT nExBoxIndex);
	virtual	VOID				MyExBox_SetItemExtraInfo(INT nExBoxIndex, BOOL bEmpty, const _ITEM* pItem);
	virtual	VOID				MyExBox_SetMoney(INT Money)					{ m_MyExchangeBox.m_Money	= Money; }
	virtual	INT					MyExBox_GetMoney()							{ return m_MyExchangeBox.m_Money;}
	virtual	VOID				MyExBox_SetLock(BOOL Setlock)				{ m_MyExchangeBox.m_IsLocked	=	Setlock; }
	virtual	BOOL				MyExBox_GetLock()							{ return m_MyExchangeBox.m_IsLocked; }
	virtual	VOID				MyExBox_SetConfirm(BOOL SetConfirm)			{ m_MyExchangeBox.m_CanConform	=	SetConfirm; }
	virtual	BOOL				MyExBox_GetConfirm()						{ return m_MyExchangeBox.m_CanConform; }
	virtual	INT					MyExBox_GetItemNum()						{ return (INT)m_MyExchangeBox.m_ItemList.size(); }


	INT							MyExBox_GetPetByGuid(PET_GUID_t PetGuid);
	VOID						MyExBox_SetPet(INT nIndex, SDATA_PET* pPetData, BOOL bClearOld = TRUE);
	SDATA_PET*					MyExBox_GetPet(INT nIndex);
	INT							MyExBox_GetCount(VOID);
	VOID						MyExBox_SetSkill(INT nIndex,INT nSkillIndex,const _OWN_SKILL *Skill);
	PET_SKILL*					MyExBox_GetSkill(INT nIndex,INT nSkillIndex);


	// ��ѯ�Լ��Ľ��׿���ͷ�Ƿ��пո�
	virtual	BOOL				MyExBox_IsEmpty(VOID);


	virtual VOID				OtExBox_Clear(BYTE bFlagClearPet = TRUE);
	virtual VOID				OtExBox_SetItem(INT nExBoxIndex, tObject_Item* pItem, BOOL bClearOld=TRUE);
	virtual tObject_Item*		OtExBox_GetItem(INT nExBoxIndex);
	virtual	VOID				OtExBox_SetItemExtraInfo(INT nExBoxIndex, BOOL bEmpty, const _ITEM* pItem);
	virtual	VOID				OtExBox_SetMoney(INT Money)					{ m_OtherExchangeBox.m_Money	= Money; }
	virtual	INT					OtExBox_GetMoney()							{ return m_OtherExchangeBox.m_Money;}
	virtual	VOID				OtExBox_SetLock(BOOL Setlock)				{ m_OtherExchangeBox.m_IsLocked	=	Setlock; }
	virtual	BOOL				OtExBox_GetLock()							{ return m_OtherExchangeBox.m_IsLocked; }
	virtual	VOID				OtExBox_SetConfirm(BOOL SetConfirm)			{ m_OtherExchangeBox.m_CanConform	=	SetConfirm; }
	virtual	BOOL				OtExBox_GetConfirm()						{ return m_OtherExchangeBox.m_CanConform; }
	virtual	INT					OtExBox_GetItemNum()						{ return (INT)m_OtherExchangeBox.m_ItemList.size(); }

	INT							OtExBox_GetPetByGuid(PET_GUID_t PetGuid);
	VOID						OtExBox_SetPet(INT nIndex, SDATA_PET* pPetData, BOOL bClearOld = TRUE);
	SDATA_PET*					OtExBox_GetPet(INT nIndex);
	INT							OtExBox_GetCount(VOID);
	VOID						OtExBox_SetSkill(INT nIndex,INT nSkillIndex,const _OWN_SKILL *Skill);
	VOID						OtExBox_SetSkill(INT nIndex, INT nSkillIndex, PET_SKILL* pPetSkillData, BOOL bClearOld);
	PET_SKILL*					OtExBox_GetSkill(INT nIndex,INT nSkillIndex);

	/** ��ǰ���׶Է�ID */
	virtual	VOID				SetExchangObjID(INT ObjID)					{ m_CurExID = ObjID;}
	virtual	INT					GetExchangObjID()							{ return m_CurExID;}
	
	/** ���뵽�������б��� */
	virtual	VOID				AddToAppList(INT appid);				
	/** ���������б���ȡ��һ�� */
	virtual	INT					GetFromAppList();						
	/** �������б��Ƿ�� */
	virtual	BOOL				IsStillAnyAppInList();			


/*
	//------------------------------------------------------
	// ��ʯ�ϳ�/��Ƕ���������
	//------------------------------------------------------
	virtual VOID				EnchasePage_Clear(VOID);
	virtual VOID				EnchasePage_SetItem(INT nEnchaseIndex, tObject_Item* pItem, BOOL bClearOld=TRUE);
	virtual tObject_Item*		EnchasePage_GetItem(INT nEnchaseIndex);
*/

/*�Ժ��ټ�
	//��ո�����Ʒ���б�
	virtual VOID				AppendItem_Clear(VOID);
	//���ø�����Ʒ���б�
	virtual VOID				AppendItem_SetItem(INT nIndex, tObject_Item* pItem, BOOL bClearOld=TRUE);
	//ȡ�ø�����Ʒ���б�ĵ�i����Ʒ��
	virtual tObject_Item*		AppendItem_GetItem(INT nIndex);			
*/

	/** ��յݽ�������Ʒ���б� */
	virtual VOID				MissionBox_Clear(VOID);
	/** ���õݽ�������Ʒ���б� */
	virtual VOID				MissionBox_SetItem(INT nMissionBoxIndex, tObject_Item* pItem, BOOL bClearOld=TRUE);
	/** ȡ�õݽ�������Ʒ���б� */
	virtual tObject_Item*		MissionBox_GetItem(INT nMissionBoxIndex);

	/** ���������Ʒ���б� */
	virtual VOID				QuestLogBox_Clear(VOID);							
	/** ����������Ʒ���б� */
	virtual VOID				QuestLogBox_SetItem(INT nQuestIndex, INT nItemIndex, tObject_Item* pItem, BOOL bClearOld=TRUE);
	/** ȡ��������Ʒ���б� */
	virtual tObject_Item*		QuestLogBox_GetItem(INT nQuestIndex, INT nItemIndex);	

	VOID						Copy_To_TargetEquip(CObject* pObj)			{ m_pTargetEquip = pObj; }
	CObject*					GetTargetEquip(VOID)						{ return m_pTargetEquip;}



/** ̯λ�����ݷ���
 */

	virtual VOID				MyStallBox_SetDefaultPage(INT nPage)		{ m_MyStallBox.m_nDefaultPage = nPage;}
	virtual INT					MyStallBox_GetDefaultPage()					{ return m_MyStallBox.m_nDefaultPage;}
	/** ����̯λ�� */
	virtual VOID				MyStallBox_SetPosTax(UINT unPosTax)			{ m_MyStallBox.m_PosTax = unPosTax;}
	/** ���̯λ�� */
	virtual UINT				MyStallBox_GetPosTax(VOID)					{ return m_MyStallBox.m_PosTax;}
	/** ���ý���˰ */
	virtual VOID				MyStallBox_SetTradeTax(UINT unTradeTax)		{ m_MyStallBox.m_TradeTax = unTradeTax;}
	/** ��ý���˰ */
	virtual UINT				MyStallBox_GetTradeTax(VOID)				{ return m_MyStallBox.m_TradeTax;}

	/** ����Լ�̯λ�� */
	virtual VOID				MyStallBox_Clear(VOID);	
	/** ����̯��ID */
	virtual VOID				MyStallBox_SetIsOpen(BOOL IsOpen)			{ m_MyStallBox.m_bIsOpen = IsOpen; }
	/** ���̯��ID */
	virtual BOOL				MyStallBox_GetIsOpen()						{ return m_MyStallBox.m_bIsOpen; }
	/** ����̯��ID */
	virtual VOID				MyStallBox_SetObjID(UINT nObjID)			{ m_MyStallBox.m_ObjID = nObjID; }
	/** ���̯��ID */
	virtual UINT				MyStallBox_GetObjID()						{ return m_MyStallBox.m_ObjID; }
	/** ����̯λ�� */
	virtual VOID				MyStallBox_SetStallName(LPCTSTR szName)		{ m_MyStallBox.m_szStallName = szName;}
	/** ���̯λ�� */
	virtual LPCTSTR				MyStallBox_GetStallName()					{ return m_MyStallBox.m_szStallName.c_str(); }
	/** ����̯���� */
	virtual VOID				MyStallBox_SetOwnerName(LPCTSTR szName)		{ m_MyStallBox.m_szOwnerName = szName;}
	/** ���̯���� */
	virtual LPCTSTR				MyStallBox_GetOwnerName()					{ return m_MyStallBox.m_szOwnerName.c_str(); }
	/** ������ҳ */
	virtual VOID				MyStallBox_SetFirstPage(UINT nFirstPage)	{ m_MyStallBox.m_nFirstPage = nFirstPage;}
	/** �����ҳ */
	virtual UINT				MyStallBox_GetFirstPage()					{ return m_MyStallBox.m_nFirstPage; }
	/** ����̯λ�е���Ʒ */
	virtual VOID				MyStallBox_SetItem(INT nStallBoxIndex, tObject_Item* pItem, BOOL bClearOld=TRUE);
	/** ����̯λ��Ʒ�۸� */
	virtual VOID				MyStallBox_SetItemPrice(INT nStallBoxIndex, UINT nPrice);
	/** ����̯λ��Ʒ���к� */
	virtual VOID				MyStallBox_SetItemSerial(INT nStallBoxIndex, UINT nSerial);
	/** ���̯λ��ָ��λ�õ���Ʒ */
	virtual tObject_Item*		MyStallBox_GetItem(INT nStallBoxIndex);
	/** �����Ʒ�۸� */
	virtual UINT				MyStallBox_GetItemPrice(INT nStallBoxIndex);
	/** �����Ʒ���к� */
	virtual UINT				MyStallBox_GetItemSerial(INT nStallBoxIndex);
	/** ������Ʒ��ϸ��Ϣ */
	virtual	VOID				MyStallBox_SetItemExtraInfo(INT nStallBoxIndex, BOOL bEmpty, const _ITEM* pItem);
	/** �����Ʒ���� */
	virtual	INT					MyStallBox_GetItemNum()						{ return (INT)m_MyStallBox.m_ItemList.size(); }
	/** ��ѯ�Լ��Ƿ��ڰ�̯״̬ */
	virtual	BOOL				MyStallBox_IsOpen()							{ return (INT)m_MyStallBox.m_bIsOpen; }
	/** ��ǰѡ�е���Ʒ */
	virtual VOID				MyStallBox_SetCurSelectItem(tObject_Item* pItem)	{ m_CurSelectItem.pItem = pItem;}
	/** ��ǰѡ�е���Ʒ */
	virtual tObject_Item*		MyStallBox_GetCurSelectItem()				{ return m_CurSelectItem.pItem;}
	/** ��ǰѡ�е���Ʒ���ڵ����� */
	virtual VOID				MyStallBox_SetSelectItemInPos(INT nSelectItemInPos)	{ m_nSelectItemInPos = nSelectItemInPos;}
	/** ��ǰѡ�е���Ʒ���ڵ����� */
	virtual INT					MyStallBox_GetSelectItemInPos()				{ return m_nSelectItemInPos; }
	/** ����GUID������Ʒλ�� */
	virtual	INT					MyStallBox_GetItemIndexByGUID(__int64 ItemGUID);
	virtual	UINT				MyStallBox_GetOwnerGUID()					{ return m_MyStallBox.m_OtGUID; }
	virtual	VOID				MyStallBox_SetOwnerGUID(UINT GUID)			{ m_MyStallBox.m_OtGUID = GUID; }

	/** ��ǰѡ�е���Ʒ���ڵ����� */
	virtual INT					MyStallBox_GetnSelectPetOnStall(){return m_nSelectPetOnStall;}
	/** ����GUID������Ʒλ�� */
	virtual	VOID				MyStallBox_SetnSelectPetOnStall(INT nSelectPetOnStall){m_nSelectPetOnStall = nSelectPetOnStall;};
	virtual	BOOL				MyStallBox_IsHaveEmpty(INT nType);


/** ��̯����
 */

	INT							MyStallBox_GetPetByGuid(PET_GUID_t PetGuid);
	VOID						MyStallBox_SetPet(INT nIndex, SDATA_PET* pPetData, BOOL bClearOld = TRUE);
	VOID						MyStallBox_SetPetPrice(INT nIndex, INT nPetPrice);
	VOID						MyStallBox_SetPetSerial(INT nIndex, INT nSerial);
	INT							MyStallBox_GetPetPrice(INT nIndex);
	INT							MyStallBox_GetPetSerial(INT nIndex);
	SDATA_PET*					MyStallBox_GetPet(INT nIndex);
	INT							MyStallBox_GetCount(VOID);
	VOID						MyStallBox_SetSkill(INT nIndex,INT nSkillIndex,const _OWN_SKILL *Skill);
	PET_SKILL*					MyStallBox_GetSkill(INT nIndex,INT nSkillIndex);
	VOID						MyStallBox_GetSelectpetGuid(PET_GUID_t &CurSelectpetGuid)
	{
		CurSelectpetGuid.m_uHighSection = m_CurSelectpetGuid.m_uHighSection;
		CurSelectpetGuid.m_uLowSection  = m_CurSelectpetGuid.m_uLowSection;
	};           
	VOID						MyStallBox_SetSelectpetGuid(PET_GUID_t CurSelectpetGuid)
	{
		m_CurSelectpetGuid.m_uHighSection = CurSelectpetGuid.m_uHighSection;
		m_CurSelectpetGuid.m_uLowSection  = CurSelectpetGuid.m_uLowSection;
	}
	virtual	BOOL				MyStallBox_IsPetOnStall(PET_GUID_t PetGuid);

	virtual	VOID				MyStallMsg_ClearUp(VOID);
	virtual	VOID				MyStallMsg_SetSerial( UINT nSerial)			{ m_MyStallBox.m_BBS.m_nSerial = nSerial;}
	virtual	INT					MyStallMsg_SetIDByIndex(UINT nIndex, UINT nID);
	virtual	INT					MyStallMsg_SetTimeByIndex(UINT	nIndex, BYTE nHour, BYTE nMin);
	virtual	INT					MyStallMsg_SetHasReplyByIndex(UINT	nIndex, BYTE bhasReply);
	virtual	INT					MyStallMsg_SetReTimeByIndex(UINT nIndex, BYTE nHour, BYTE nMin);
	virtual	VOID				MyStallMsg_SetTitle(LPCTSTR szMsg);
	virtual	INT					MyStallMsg_SetAuthorNameByIndex(UINT	nIndex, LPCTSTR szMsg);
	virtual	INT					MyStallMsg_SetProposedMessageByIndex(UINT	nIndex, LPCTSTR szMsg);
	virtual	INT					MyStallMsg_SetReplyMessageByIndex(UINT nIndex, LPCTSTR szMsg);
	virtual	UINT				MyStallMsg_GetSerial()						{ return m_MyStallBox.m_BBS.m_nSerial;}
	virtual	INT					MyStallMsg_GetIDByIndex(UINT nIndex);
	virtual	BYTE				MyStallMsg_GetHasReplyByIndex(UINT nIndex);
	virtual	LPCTSTR				MyStallMsg_GetTitle();
	virtual	LPCTSTR				MyStallMsg_GetAuthorNameByIndex(UINT nIndex);
	virtual	LPCTSTR				MyStallMsg_GetProposedMessageByIndex(UINT nIndex );
	virtual	LPCTSTR				MyStallMsg_GetReplyMessageByIndex(UINT nIndex);
	virtual	INT					MyStallMsg_GetMessageNum();
	virtual	VOID				MyStallMsg_SetMessageNum(INT nNum);
	virtual	INT					MyStallMsg_GetHourByIndex(UINT	nIndex);
	virtual	INT					MyStallMsg_GetMinByIndex(UINT	nIndex);
	virtual	INT					MyStallMsg_GetReHourByIndex(UINT	nIndex);
	virtual	INT					MyStallMsg_GetReMinByIndex(UINT	nIndex);


	virtual VOID				OtStallBox_SetDefaultPage(INT nPage)		{ m_OtStallBox.m_nDefaultPage = nPage;}
	virtual INT					OtStallBox_GetDefaultPage()					{ return m_OtStallBox.m_nDefaultPage;}
	virtual VOID				OtStallBox_SetTradeTax(UINT unTradeTax)		{ m_OtStallBox.m_TradeTax = unTradeTax;}	//���ý���˰
	virtual UINT				OtStallBox_GetTradeTax(VOID)				{ return m_OtStallBox.m_TradeTax;}			//��ý���˰

	virtual VOID				OtStallBox_Clear(BYTE bFlagClearPet = TRUE);											//��նԷ�̯λ��
	virtual VOID				OtStallBox_SetIsOpen(BOOL IsOpen)			{ m_OtStallBox.m_bIsOpen = IsOpen; }		//����̯��ID
	virtual BOOL				OtStallBox_GetIsOpen()						{ return m_OtStallBox.m_bIsOpen; }			//���̯��ID
	virtual VOID				OtStallBox_SetObjID(UINT nObjID)			{ m_OtStallBox.m_ObjID = nObjID; }			//����̯��ID
	virtual UINT				OtStallBox_GetObjID()						{ return m_OtStallBox.m_ObjID; }			//���̯��ID
	virtual VOID				OtStallBox_SetStallName(LPCTSTR szName)		{ m_OtStallBox.m_szStallName = szName;}		//����̯λ��
	virtual LPCTSTR				OtStallBox_GetStallName()					{ return m_OtStallBox.m_szStallName.c_str(); }//���̯λ��
	virtual VOID				OtStallBox_SetOwnerName(LPCTSTR szName)		{ m_OtStallBox.m_szOwnerName = szName;}		//����̯����
	virtual LPCTSTR				OtStallBox_GetOwnerName()					{ return m_OtStallBox.m_szOwnerName.c_str(); }//���̯����
	virtual VOID				OtStallBox_SetFirstPage(UINT nFirstPage)	{m_OtStallBox.m_nFirstPage = nFirstPage;}	//����̯λ��ҳ
	virtual UINT				OtStallBox_GetFirstPage()					{ return m_OtStallBox.m_nFirstPage; }		//���̯λ��ҳ
	virtual VOID				OtStallBox_SetItem(INT nExBoxIndex, tObject_Item* pItem, BOOL bClearOld=TRUE);			//����̯λ��Ʒ
	virtual VOID				OtStallBox_SetItemPrice(INT nStallBoxIndex, UINT nPrice);								//������Ʒ�۸�
	virtual VOID				OtStallBox_SetItemSerial(INT nStallBoxIndex, UINT nSerial);								//������Ʒ���к�
	virtual tObject_Item*		OtStallBox_GetItem(INT nExBoxIndex);													//����ƶ�λ�õ���Ʒ
	virtual UINT				OtStallBox_GetItemPrice(INT nStallBoxIndex);											//�����Ʒ�۸�
	virtual UINT				OtStallBox_GetItemSerial(INT nStallBoxIndex);											//�����Ʒ���к�
	virtual	VOID				OtStallBox_SetItemExtraInfo(INT nExBoxIndex, BOOL bEmpty, const _ITEM* pItem);			//������Ʒ��ϸ��Ϣ
	virtual	INT					OtStallBox_GetItemNum()						{ return (INT)m_MyStallBox.m_ItemList.size(); }//���̯λ�е���Ʒ����
	virtual	INT					OtStallBox_GetItemIndexByGUID(__int64 ItemGUID);										//����GUID������Ʒλ��

	virtual	UINT				OtStallBox_GetOwnerGUID()					{ return m_OtStallBox.m_OtGUID;}
	virtual	VOID				OtStallBox_SetOwnerGUID(UINT GUID)			{ m_OtStallBox.m_OtGUID = GUID;}

	//��̯����
	INT							OtStallBox_GetPetByGuid(PET_GUID_t PetGuid);
	VOID						OtStallBox_SetPet(INT nIndex, SDATA_PET* pPetData, BOOL bClearOld = TRUE);
	VOID						OtStallBox_SetPetPrice(INT nIndex, INT nPetPrice);
	VOID						OtStallBox_SetPetSerial(INT nIndex, INT nSerial);
	INT							OtStallBox_GetPetPrice(INT nIndex);
	INT							OtStallBox_GetPetSerial(INT nIndex);
	SDATA_PET*					OtStallBox_GetPet(INT nIndex);
	INT							OtStallBox_GetCount(VOID);
	VOID						OtStallBox_SetSkill(INT nIndex,INT nSkillIndex,const _OWN_SKILL *Skill);
	PET_SKILL*					OtStallBox_GetSkill(INT nIndex,INT nSkillIndex);

	virtual	VOID				OtStallMsg_ClearUp(VOID);
	virtual	VOID				OtStallMsg_SetSerial( UINT nSerial)			{ m_OtStallBox.m_BBS.m_nSerial = nSerial;}
	virtual	INT					OtStallMsg_SetIDByIndex(UINT nIndex, UINT nID);
	virtual	INT					OtStallMsg_SetTimeByIndex(UINT	nIndex, BYTE nHour, BYTE nMin);
	virtual	INT					OtStallMsg_SetHasReplyByIndex(UINT	nIndex, BYTE bhasReply);
	virtual	INT					OtStallMsg_SetReTimeByIndex(UINT nIndex, BYTE nHour, BYTE nMin);
	virtual	VOID				OtStallMsg_SetTitle(LPCTSTR szMsg);
	virtual	INT					OtStallMsg_SetAuthorNameByIndex(UINT nIndex, LPCTSTR szMsg);
	virtual	INT					OtStallMsg_SetProposedMessageByIndex(UINT nIndex, LPCTSTR szMsg);
	virtual	INT					OtStallMsg_SetReplyMessageByIndex(UINT nIndex, LPCTSTR szMsg);
	virtual	UINT				OtStallMsg_GetSerial()						{ return m_OtStallBox.m_BBS.m_nSerial;}
	virtual	INT					OtStallMsg_GetIDByIndex(UINT nIndex);
	virtual	BYTE				OtStallMsg_GetHasReplyByIndex(UINT nIndex);
	virtual	LPCTSTR				OtStallMsg_GetTitle();
	virtual	LPCTSTR				OtStallMsg_GetAuthorNameByIndex(UINT nIndex);
	virtual	LPCTSTR				OtStallMsg_GetProposedMessageByIndex(UINT nIndex );
	virtual	LPCTSTR				OtStallMsg_GetReplyMessageByIndex(UINT nIndex);
	virtual	INT					OtStallMsg_GetHourByIndex(UINT	nIndex);
	virtual	INT					OtStallMsg_GetMinByIndex(UINT	nIndex);
	virtual	INT					OtStallMsg_GetReHourByIndex(UINT	nIndex);
	virtual	INT					OtStallMsg_GetReMinByIndex(UINT	nIndex);
	virtual	INT					OtStallMsg_GetMessageNum();
	virtual	VOID				OtStallMsg_SetMessageNum(INT nNum);									   
	

//---------------------------------------------------------------------------------------------------
/** ѧϰ�������������ķ����
 */

	/** �����ķ���ʦ��ID */
//	VOID						StudyXinfa_SetMenpaiInfo(INT idXinfaTeacher)		{ m_idXinfaTeacher = idXinfaTeacher;}
//	INT							StudyXinfa_GetMenpaiInfo(VOID)						{ return m_idXinfaTeacher;}
//	VOID						StudyXinfa_SetXinfaTeacherId(INT nXinfaTeacherId)	{ m_nXinfaTeacherId = nXinfaTeacherId;}
//	INT							StudyXinfa_GetXinfaTeacherId(VOID)					{ return m_nXinfaTeacherId;}
	/** ����������ͼ������ɵ��¼� */
//$$	VOID						SendStudyXinfaEvent(INT nXinfaID);

	/** ����ܽ�ʦ */
	INT							StudyAbility_GetID(VOID)					{ return m_nStudyAbility_ID; }
	VOID						StudyAbility_SetID(INT nAbilityID)			{ m_nStudyAbility_ID = nAbilityID; }

	INT							StudyAbility_GetNeedMoney(VOID)				{ return m_uStudyAbility_NeedMoney; }
	VOID						StudyAbility_SetNeedMoney(INT uNeedMoney)	{ m_uStudyAbility_NeedMoney = uNeedMoney; }

	INT							StudyAbility_GetNeedExp(VOID)				{ return m_uStudyAbility_NeedExp; }
	VOID						StudyAbility_SetNeedExp(INT uNeedExp)		{ m_uStudyAbility_NeedExp = uNeedExp; }

	INT							StudyAbility_GetSkillExp(VOID)				{ return m_uStudyAbility_SkillExp; }
	VOID						StudyAbility_SetSkillExp(INT uSkillExp)		{ m_uStudyAbility_SkillExp = uSkillExp; }

	INT							StudyAbility_GetLevelLimit(VOID)							{ return m_nStudyAbility_LevelLimit;}
	VOID						StudyAbility_SetLevelLimit(INT nStudyAbility_LevelLimit)	{ m_nStudyAbility_LevelLimit = nStudyAbility_LevelLimit;}

	INT							StudyAbility_GetScriptId(VOID)								{ return m_uStudyAbility_ScriptId;}
	VOID						StudyAbility_SetScriptId(INT uStudyAbility_ScriptId)		{ m_uStudyAbility_ScriptId = uStudyAbility_ScriptId;}

	INT							StudyAbility_GetNpcId(VOID)									{ return m_StudyAbility_NpcId;}
	VOID						StudyAbility_SetNpcId(INT StudyAbility_NpcId)				{ m_StudyAbility_NpcId = StudyAbility_NpcId;}



/**  ���ܵ�����������
*/

	INT							Pet_GetPetByGuid(const PET_GUID_t& PetGuid);
	SDATA_PET*					Pet_GetPet(INT nIndex);
	SDATA_PET*					Pet_GetPet(const PET_GUID_t& PetGuid);
	VOID						Pet_ClearPet(INT nIndex);
	INT							Pet_GetCount(VOID);
	VOID						Pet_SetPetFakeModel(INT nIndex, INT nMountID);
	const CHAR*					Pet_GetPetFakeName(INT nIndex);

	// �����������
	VOID						Pet_SetPet( SDATA_PET Pet );
	// �������
	INT							Pet_AddPet( SDATA_PET Pet );


	// ��ó����ʳ������
	INT							Pet_GetPetFoodType(PET_GUID_t guidPet);											
	// ��ó�����Ӧʳ�����͵�idTable����
	VOID						Pet_GetPetFoodTypeMinMax(INT nFoodType, INT* idMin = NULL, INT* idMax = NULL);	
	// �ӱ������ι��������ͼ���ʳ��
	tObject_Item*				Pet_GetLowestLevel_Food_From_Package(PET_GUID_t pg, INT *idxPackage = NULL);	
	// �ӱ������ѱ��������ͼ���ʳ��
	tObject_Item*				Pet_GetLowestLevel_Dome_From_Package(PET_GUID_t pg, INT *idxPackage = NULL);	

	VOID						Pet_CopyToTarget(SDATA_PET* pSourcePet);

	SDATA_PET*					TargetPet_GetPet(VOID)						{ return &m_TargetPet; }
	VOID						TargetPet_Clear(VOID)						{  m_TargetPet.CleanUp();}
	VOID						TargetPet_SetPetModel(INT nMountID);
	PET_SKILL*					TargetPet_GetSkill(INT nSkillIndex);
	VOID						TargetPet_SetSkill(INT nSkillIndex, INT nSkillID, BOOL bCanUse);
	VOID						TargetPet_SetSkill(INT nSkillIndex, PET_SKILL* pPetSkillData, BOOL bClearOld = TRUE);
	const CHAR*					TargetPet_GetPetFakeModel();
	VOID						TargetPet_SetPetFakeModel(INT nMountID);

	VOID						PetStudySkill_SetPetModel(INT nRaceID);

	VOID						Pet_SetSkill(INT nIndex, INT nSkillIndex, INT nSkillID, BOOL bCanUse);
	VOID						Pet_SetSkill(INT nIndex, INT nSkillIndex, PET_SKILL* pPetSkillData, BOOL bClearOld = TRUE);
	PET_SKILL*					Pet_GetSkill(INT nIndex,INT nSkillIndex);
	BOOL						Pet_SendUseSkillMessage( SkillID_t idSkill, ObjID_t idTargetObj, FLOAT fTargetX, FLOAT fTargetZ );


//---------------------------------------------------------------------------------------------------
/**  �����������  [7/8/2010 �¾���] 
*/
    const CHAR*                 Mount_GetMountFakeName(INT nIndex);
	VOID                        Mount_SetMountFakeModel(INT nIndex, INT nMountID);
	INT                         Mount_GetMountCount(VOID);


//---------------------------------------------------------------------------------------------------
/**  ���ܵ�BUFF����
 */

	INT							BuffImpact_GetCount( VOID )const			{ return (INT)(m_mapBuffImpact.size()); }
	const _BUFF_IMPACT_INFO*	BuffImpact_Get( INT nSN )const;
	const _BUFF_IMPACT_INFO*	BuffImpact_GetByIndex( INT nIndex )const;
	const _BUFF_IMPACT_INFO*	BuffImpact_GetByID( INT nID )const;
	BOOL						BuffImpact_Add( const _BUFF_IMPACT_INFO *pBuffImpact );
	VOID						BuffImpact_Remove( INT nSN );
	VOID						BuffImpact_RemoveByIndex( INT nIndex );
	VOID						BuffImpact_RemoveAll( VOID );
	const CHAR*					BuffImpact_GetToolTips( INT nSN )const;
	const CHAR*					BuffImpact_GetToolTipsByIndex( INT nIndex )const;
	BOOL						BuffImpact_Dispel( INT nSN );
	BOOL						BuffImpact_DispelByIndex( INT nIndex );
	INT							BuffImpact_GetTime( INT nIndex );
	INT							BuffImpact_GetTimeByIndex( INT nIndex );


	/** ��ǰ��Ҫ������Ʒ */
	INT							DisCard_GetItemConta()						{ return m_CurDisCardItem.m_nContainer;}
	INT							DisCard_GetItemPos()						{ return m_CurDisCardItem.m_nPosition;}
	VOID						DisCard_SetItemConta(INT nContainer)		{ m_CurDisCardItem.m_nContainer = nContainer;}
	VOID						DisCard_SetItemPos(INT nPosition)			{ m_CurDisCardItem.m_nPosition = nPosition;}


	X_PARAM						m_X_PARAM;
	VOID						X_PARAM_Set(X_PARAM *pBuf,INT nUIIndex);
	X_PARAM*					X_PARAM_Get( VOID )							{ return &m_X_PARAM;};

	X_SCRIPT					m_X_SCRIPT;
	X_SCRIPT*					X_XCRIPT_Get( VOID )						{ return &m_X_SCRIPT;};


	/** ��ǰ���ڲ�ֵ���Ʒ */
	INT							Split_GetItemConta()						{ return m_CurSplitItem.m_nContainer;}
	INT							Split_GetItemPos()							{ return m_CurSplitItem.m_nPosition;}
	INT							Split_GetSum()								{ return m_CurSplitItem.m_nSum;}
	INT							Split_GetNumber()							{ return m_CurSplitItem.m_nNumber;}
	VOID						Split_SetItemConta(INT nContainer)			{ m_CurSplitItem.m_nContainer = nContainer;}
	VOID						Split_SetItemPos(INT nPosition)				{ m_CurSplitItem.m_nPosition = nPosition;}
	VOID						Split_SetSum(INT nSum)						{ m_CurSplitItem.m_nSum = nSum;}
	VOID						Split_SetNumber(INT nNumber)				{ m_CurSplitItem.m_nNumber = nNumber;}

 
  


	/** ��ǰ���ڴ��ǵ���Ʒ */
	VOID						AddStar_CleanUP()								{  m_CurAddStarEquip.CleanUp();}
	INT							AddStar_GetItemConta()							{ return m_CurAddStarEquip.m_nContainer;}
	INT							AddStar_GetItemPos()							{ return m_CurAddStarEquip.m_nPosition;}
	INT							AddStar_GetGemId()								{ return m_CurAddStarEquip.m_GemId;}
	INT							AddStar_GetGemNum()								{ return m_CurAddStarEquip.m_GemNum;}
	INT							AddStar_GetMoney()								{ return m_CurAddStarEquip.m_nMoney;}
	INT							AddStar_GetIntensifyInfo()						{ return m_CurAddStarEquip.m_nIntensifyInfo;}

	VOID						AddStar_SetItemConta(INT nContainer)			{ m_CurAddStarEquip.m_nContainer = nContainer;}
	VOID						AddStar_SetItemPos(INT nPosition)				{ m_CurAddStarEquip.m_nPosition = nPosition;}
	VOID						AddStar_SetGemId(INT nGemID)					{ m_CurAddStarEquip.m_GemId = nGemID;}
	VOID						AddStar_SetGemNum(INT nGemNum)					{ m_CurAddStarEquip.m_GemNum = nGemNum;}
	VOID						AddStar_SetMoney( INT nMoney)					{ m_CurAddStarEquip.m_nMoney = nMoney;}
	VOID						AddStar_SetIntensifyInfo( INT nIntensifyInfo)	{ m_CurAddStarEquip.m_nIntensifyInfo = nIntensifyInfo;}
	


	/** ��ǰ���ڴ�׵���Ʒ */
	VOID						AddHole_CleanUP()								{  m_CurAddHoleEquip.CleanUp();}
	INT							AddHole_GetItemConta()							{ return m_CurAddHoleEquip.m_nContainer;}
	INT							AddHole_GetItemPos()							{ return m_CurAddHoleEquip.m_nPosition;}
	INT							AddHole_GetNeedItemId()							{ return m_CurAddHoleEquip.m_ItemId;}
	INT							AddHole_GetNeedItemNum()						{ return m_CurAddHoleEquip.m_ItemNum;}
	INT							AddHole_GetNeedMoney()							{ return m_CurAddHoleEquip.m_Money;}
	INT							AddHole_GetNpcId()								{ return m_CurAddHoleEquip.m_NpcId;}
	

	VOID						AddHole_SetItemConta(INT nContainer)			{ m_CurAddHoleEquip.m_nContainer = nContainer;}
	VOID						AddHole_SetItemPos(INT nPosition)				{ m_CurAddHoleEquip.m_nPosition = nPosition;}
	VOID						AddHole_SetNeedItemId(INT nNeedID)				{ m_CurAddHoleEquip.m_ItemId = nNeedID;}
	VOID						AddHole_SetNeedItemNum(INT nNeedNum)			{ m_CurAddHoleEquip.m_ItemNum = nNeedNum;}
	VOID						AddHole_SetNeedMoney(INT nNeedMoney)			{ m_CurAddHoleEquip.m_Money = nNeedMoney;}
	VOID						AddHole_SetNpcId(INT npc)						{ m_CurAddHoleEquip.m_NpcId = npc;}


	/** ��ǰ���ڰ󶨵���Ʒ */
	VOID						Bind_CleanUP()								{  m_CurBangDingEquip.CleanUp();}
	INT							Bind_GetItemConta()							{ return m_CurBangDingEquip.m_nContainer;}
	INT							Bind_GetItemPos()							{ return m_CurBangDingEquip.m_nPosition;}
	INT							Bind_GetNeedItemId()						{ return m_CurBangDingEquip.m_ItemId;}
	INT							Bind_GetNeedItemPos()						{ return m_CurBangDingEquip.m_NeedItemPos;}

	
	 
	VOID						Bind_SetItemConta(INT nContainer)			{ m_CurBangDingEquip.m_nContainer = nContainer;}
	VOID						Bind_SetItemPos(INT nPosition)				{ m_CurBangDingEquip.m_nPosition = nPosition;}
	VOID						Bind_SetNeedItemId(INT nNeedItemID)			{ m_CurBangDingEquip.m_ItemId = nNeedItemID;}
	VOID						Bind_SetNeedItemPos(INT nNeedItemPos)		{ m_CurBangDingEquip.m_NeedItemPos = nNeedItemPos;}


	/** ��ǰ���ڸ������͵���Ʒ */
	VOID						ChangeCard_CleanUP()							{  m_CurChangeCardEquip.CleanUp();}
	INT							ChangeCard_GetItemConta()						{ return m_CurChangeCardEquip.m_nContainer;}
	INT							ChangeCard_GetItemPos()							{ return m_CurChangeCardEquip.m_nPosition;}
	INT							ChangeCard_GetNeedItemId()						{ return m_CurChangeCardEquip.m_ItemId;}
	INT							ChangeCard_GetNeedItemPos()						{ return m_CurChangeCardEquip.m_NeedItemPos;}



	VOID						ChangeCard_SetItemConta(INT nContainer)			{ m_CurChangeCardEquip.m_nContainer = nContainer;}
	VOID						ChangeCard_SetItemPos(INT nPosition)			{ m_CurChangeCardEquip.m_nPosition = nPosition;}
	VOID						ChangeCard_SetNeedItemId(INT nNeedItemID)		{ m_CurChangeCardEquip.m_ItemId = nNeedItemID;}
	VOID						ChangeCard_SetNeedItemPos(INT nNeedItemPos)		{ m_CurChangeCardEquip.m_NeedItemPos = nNeedItemPos;}





	/** ��ǰ���ڽ������͵���Ʒ */
	VOID						SwapCard_CleanUP()								{  m_CurSwapCardEquip.CleanUp();}
	INT							SwapCard_GetItemConta1()						{ return m_CurSwapCardEquip.m_nContainer1;}
	INT							SwapCard_GetItemPos1()							{ return m_CurSwapCardEquip.m_nPosition1;}
	INT							SwapCard_GetItemConta2()						{ return m_CurSwapCardEquip.m_nContainer2;}
	INT							SwapCard_GetItemPos2()							{ return m_CurSwapCardEquip.m_nPosition2;}
	INT							SwapCard_GetNeedItemId()						{ return m_CurSwapCardEquip.m_ItemId;}
	INT							SwapCard_GetNeedItemPos()						{ return m_CurSwapCardEquip.m_NeedItemPos;}

	VOID						SwapCard_SetItemConta1(INT nContainer)			{ m_CurSwapCardEquip.m_nContainer1 = nContainer;}
	VOID						SwapCard_SetItemPos1(INT nPosition)				{ m_CurSwapCardEquip.m_nPosition1 = nPosition;}
	VOID						SwapCard_SetItemConta2(INT nContainer)			{ m_CurSwapCardEquip.m_nContainer2 = nContainer;}
	VOID						SwapCard_SetItemPos2(INT nPosition)				{ m_CurSwapCardEquip.m_nPosition2 = nPosition;}
	VOID						SwapCard_SetNeedItemId(INT nNeedItemID)			{ m_CurSwapCardEquip.m_ItemId = nNeedItemID;}
	VOID						SwapCard_SetNeedItemPos(INT nNeedItemPos)		{ m_CurSwapCardEquip.m_NeedItemPos = nNeedItemPos;}





	/** ��ǰ���ڸ����װ�� */
	VOID						Rework_CleanUP()							{  m_CurEquip_ReWork.CleanUp();}
	INT							Rework_GetItemConta()						{ return m_CurEquip_ReWork.m_nContainer;}
	INT							Rework_GetItemPos()							{ return m_CurEquip_ReWork.m_nPosition;}
	INT							Rework_GetIdTable()							{ return m_CurEquip_ReWork.m_IdTable;}



	VOID						Rework_SetItemConta(INT nContainer)			{ m_CurEquip_ReWork.m_nContainer = nContainer;}
	VOID						Rework_SetItemPos(INT nPosition)			{ m_CurEquip_ReWork.m_nPosition = nPosition;}
	VOID					    Rework_SetIdTable(INT id)					{ m_CurEquip_ReWork.m_IdTable = id;}	

	INT							Rework_GetMatId(int index)							
	{ 

		if( 0 >index  && index >= MAX_EQUIP_RWMATL_NUM)   
			return  -1;
		return m_CurEquip_ReWork.m_Mat[index].m_materialId;

	}

	INT							Rework_GetMatNum(int index)					
	{ 

		if( 0 >index  && index >= MAX_EQUIP_RWMATL_NUM)   
			return  -1;
		return m_CurEquip_ReWork.m_Mat[index].m_materialNum;

	}

	INT							Rework_GetMatGrade(int index)					
	{ 

		if( 0 >index  && index >= MAX_EQUIP_RWMATL_NUM)   
			return  -1;
		return m_CurEquip_ReWork.m_Mat[index].m_materialGrade;

	}

	VOID						Rework_RecordMatNum()					    { 	m_CurSynEquip.m_MatNum ++;} 
	VOID						Rework_SetMatId(INT Id,int index)			

	{

		if( 0 >index  && index >= MAX_EQUIP_RWMATL_NUM)   
			return  ;
		m_CurEquip_ReWork.m_Mat[index].m_materialId = Id;	

	}	
	VOID						Rework_SetMatNum(INT Num,int index)			
	{
		if( 0 >index  && index >= MAX_EQUIP_RWMATL_NUM)   
			return  ;
		m_CurEquip_ReWork.m_Mat[index].m_materialNum = Num;

	}	
	VOID					   Rework_SetMatGrade(INT Grade,int index)		

	{
		if( 0 >index  && index >= MAX_EQUIP_RWMATL_NUM)   
			return  ;
		m_CurEquip_ReWork.m_Mat[index].m_materialGrade = Grade;
	}	



	////װ��������

	/** ��ǰ�����������װ�� */
	VOID						SiritRework_CleanUP()							{  m_CurEquip_SpiritReWork.CleanUp();}
	INT							SiritRework_GetItemConta()						{ return m_CurEquip_SpiritReWork.m_nContainer;}
	INT							SiritRework_GetItemPos()						{ return m_CurEquip_SpiritReWork.m_nPosition;}
	INT							SiritRework_GetIdTable()						{ return m_CurEquip_SpiritReWork.m_IdTable;}


	VOID						SiritRework_SetItemConta(INT nContainer)		{ m_CurEquip_SpiritReWork.m_nContainer = nContainer;}
	VOID						SiritRework_SetItemPos(INT nPosition)			{ m_CurEquip_SpiritReWork.m_nPosition = nPosition;}
	VOID					    SiritRework_SetIdTable(INT id)					{ m_CurEquip_SpiritReWork.m_IdTable = id;}	

	INT							SiritRework_GetMatId(int index)							
	{ 

		if( 0 >index  && index >= MAX_EQUIP_SPIRIT_RWMATL_NUM)   
			return  -1;
		return m_CurEquip_SpiritReWork.m_Mat[index].m_materialId;

	}

	INT							SiritRework_GetMatNum(int index)					
	{ 

		if( 0 >index  && index >= MAX_EQUIP_SPIRIT_RWMATL_NUM)   
			return  -1;
		return m_CurEquip_SpiritReWork.m_Mat[index].m_materialNum;

	}

	INT							SiritRework_GetMatGrade(int index)					
	{ 

		if( 0 >index  && index >= MAX_EQUIP_SPIRIT_RWMATL_NUM)   
			return  -1;
		return m_CurEquip_SpiritReWork.m_Mat[index].m_materialGrade;

	}

	VOID						SiritRework_RecordMatNum()					    { 	m_CurSynEquip.m_MatNum ++;} 
	VOID						SiritRework_SetMatId(INT Id,int index)			

	{

		if( 0 >index  && index >= MAX_EQUIP_SPIRIT_RWMATL_NUM)   
			return  ;
		m_CurEquip_SpiritReWork.m_Mat[index].m_materialId = Id;	

	}	
	VOID						SiritRework_SetMatNum(INT Num,int index)			
	{
		if( 0 >index  && index >= MAX_EQUIP_SPIRIT_RWMATL_NUM)   
			return  ;
		m_CurEquip_SpiritReWork.m_Mat[index].m_materialNum = Num;

	}	
	VOID					   SiritRework_SetMatGrade(INT Grade,int index)		

	{
		if( 0 >index  && index >= MAX_EQUIP_SPIRIT_RWMATL_NUM)   
			return  ;
		m_CurEquip_SpiritReWork.m_Mat[index].m_materialGrade = Grade;
	}	





	/** ��ǰ���ںϳɵ�װ�� */
	VOID						SynEquip_CleanUP()								{  m_CurSynEquip.CleanUp();} 

	INT							SynEquip_GetIdTable()							{ return m_CurSynEquip.m_IdTable;}
	INT							SynEquip_GetRecordMatNum()						{ return m_CurSynEquip.m_MatNum;}
	INT							SynEquip_GetMatId(int index)							
	{ 

		if( 0 >index  && index >= MAX_EQUIP_PROMATL_NUM)   

			return  -1;

		return m_CurSynEquip.m_Mat[index].m_materialId;

	}

	INT							SynEquip_GetMatNum(int index)					
	{ 

		if( 0 >index  && index >= MAX_EQUIP_PROMATL_NUM)   

			return  -1;

		return m_CurSynEquip.m_Mat[index].m_materialNum;

	}

	INT							SynEquip_GetMatGrade(int index)					
	{ 

		if( 0 >index  && index >= MAX_EQUIP_PROMATL_NUM)   

			return  -1;

		return m_CurSynEquip.m_Mat[index].m_materialGrade;

	}
 
	VOID						SynEquip_RecordMatNum()					    { 	m_CurSynEquip.m_MatNum ++;} 
	VOID					    SynEquip_SetIdTable(INT id)					{  m_CurSynEquip.m_IdTable = id;}	
	VOID						SynEquip_SetMatId(INT Id,int index)			

	{

		if( 0 >index  && index >= MAX_EQUIP_PROMATL_NUM)   

			return  ;
		m_CurSynEquip.m_Mat[index].m_materialId = Id;	

	}	
	VOID						SynEquip_SetMatNum(INT Num,int index)			
	{
		if( 0 >index  && index >= MAX_EQUIP_PROMATL_NUM)   

       		return  ;
		m_CurSynEquip.m_Mat[index].m_materialNum = Num;

	}	
	VOID					   SynEquip_SetMatGrade(INT Grade,int index)		

	{
		if( 0 >index  && index >= MAX_EQUIP_PROMATL_NUM)   

			return  ;
		m_CurSynEquip.m_Mat[index].m_materialGrade = Grade;
	}	


	/** ��ǰ���ڲ���װ�� */
	VOID						Dismantle_CleanUP()								{  m_CurEquip_Dis.CleanUp();}
	INT							Dismantle_GetItemConta()						{ return m_CurEquip_Dis.m_nContainer;}
	INT							Dismantle_GetItemPos()							{ return m_CurEquip_Dis.m_nPosition;}
	INT							Dismantle_GetIdTable()							{ return m_CurEquip_Dis.m_IdTable;}

	INT							Dismantle_GetRecordMatNum()						{ return m_CurEquip_Dis.m_MatNum;}
	
	INT							Dismantle_GetMatId(int index)							
		{ 
			
			if( 0 >index  && index >= MAX_EQUIP_PROMATL_NUM)   
				return  -1;
			return m_CurEquip_Dis.m_Mat[index].m_materialId;		
		 
		}

	INT							Dismantle_GetMatNum(int index)					
	{ 

		if( 0 >index  && index >= MAX_EQUIP_PROMATL_NUM)   
			return  -1;
		return m_CurEquip_Dis.m_Mat[index].m_materialNum;
		
	}

	INT							Dismantle_GetMatGrade(int index)					
	
	{ 

		if( 0 >index  && index >= MAX_EQUIP_PROMATL_NUM)   
			return  -1;
		return m_CurEquip_Dis.m_Mat[index].m_materialGrade;
	
	}
	INT							Dismantle_GetMatLoss(int index)				  
	
	{ 
		if( 0 >index  && index >= MAX_EQUIP_PROMATL_NUM)   
			return  -1;
		return m_CurEquip_Dis.m_Mat[index].m_nMaterialLoss;

	}

	VOID						Dismantle_RecordMatNum()					    { 	m_CurEquip_Dis.m_MatNum ++;}
	VOID						Dismantle_SetItemConta(INT nContainer)			{ m_CurEquip_Dis.m_nContainer = nContainer;}
	VOID						Dismantle_SetItemPos(INT nPosition)				{ m_CurEquip_Dis.m_nPosition = nPosition;}
	VOID					    Dismantle_SetIdTable(INT id)					{  m_CurEquip_Dis.m_IdTable = id;}	

	VOID						Dismantle_SetMatId(INT Id,int index)			
		
	{
		
		if( 0 >index  && index >= MAX_EQUIP_PROMATL_NUM)   
			return  ;
		m_CurEquip_Dis.m_Mat[index].m_materialId = Id;	
	
	}	
	VOID						Dismantle_SetMatNum(INT Num,int index)			
	
	{
		if( 0 >index  && index >= MAX_EQUIP_PROMATL_NUM)   
			return  ;

		if( Num <=0)
			Num = 0;

		m_CurEquip_Dis.m_Mat[index].m_materialNum = Num;
	
	}	

	VOID						Dismantle_SetMatGrade(INT Grade,int index)		

	{
		if( 0 >index  && index >= MAX_EQUIP_PROMATL_NUM)   
			return  ;
		m_CurEquip_Dis.m_Mat[index].m_materialGrade = Grade;
	}	

	VOID						Dismantle_SetMatLoss(INT Loss,int index)	

	{
		if( 0 >index  && index >= MAX_EQUIP_PROMATL_NUM)   
			return  ;
			m_CurEquip_Dis.m_Mat[index].m_nMaterialLoss = Loss;
	
	}	
 

	/** ��ǰ�������޵���Ʒ */
	VOID						SpRepair_CleanUP()								{  m_CurSpRepairEquip.CleanUp();}
	INT							SpRepair_GetItemConta()							{ return m_CurSpRepairEquip.m_nContainer;}
	INT							SpRepair_GetItemPos()							{ return m_CurSpRepairEquip.m_nPosition;}
	INT							SpRepair_GetNeedItemId()						{ return m_CurSpRepairEquip.m_NeedItemId;}
	INT							SpRepair_GetNeedItemNum()						{ return m_CurSpRepairEquip.m_NeedItemNum;}
	INT							SpRepair_GetIntensifyInfo()						{ return m_CurSpRepairEquip.m_nIntensifyInfo;}

	VOID						SpRepair_SetItemConta(INT nContainer)			{ m_CurSpRepairEquip.m_nContainer = nContainer;}
	VOID						SpRepair_SetItemPos(INT nPosition)				{ m_CurSpRepairEquip.m_nPosition = nPosition;}
	VOID						SpRepair_SetNeedItemId(INT nNeedItemId)			{ m_CurSpRepairEquip.m_NeedItemId = nNeedItemId;}
	VOID						SpRepair_SetNeedItemNum(INT nNeedItemNum)		{ m_CurSpRepairEquip.m_NeedItemNum = nNeedItemNum;}
	VOID						SpRepair_SetIntensifyInfo( INT nIntensifyInfo)	{ m_CurSpRepairEquip.m_nIntensifyInfo = nIntensifyInfo;}


	/** ��ǰ�����������Ʒ */
	VOID						ComRepair_CleanUP()								{  m_CurComRepairEquip.CleanUp();}
	INT							ComRepair_GetItemConta()						{ return m_CurComRepairEquip.m_nContainer;}
	INT							ComRepair_GetItemPos()							{ return m_CurComRepairEquip.m_nPosition;}

	VOID						ComRepair_SetItemConta(INT nContainer)			{ m_CurComRepairEquip.m_nContainer = nContainer;}
	VOID						ComRepair_SetItemPos(INT nPosition)				{ m_CurComRepairEquip.m_nPosition = nPosition;}


//---------------------------------------------------------------------------------------------------
/** ���������ͺ����� 
 */

	/** ��պ����б� */
	VOID						Relation_ClearFriend();	
	/** ��պ����� */
	VOID						Relation_ClearBlack();
	/** ��ճ��� */
	VOID						Relation_ClearEnemy();

	/** ���� */
	UINT						Relation_GetFriendNum()						{ return m_uFriendCount; }
	UINT						Relation_GetBlackNum()						{ return m_uBlackCount; }
	UINT						Relation_GetEnemyNum()						{ return m_uEnemyCount; }

	VOID						Relation_SetFriendNum(UINT uFriendCount)	{ m_uFriendCount = uFriendCount; }
	VOID						Relation_SetBlackNum(UINT uBlackCount)		{ m_uBlackCount = uBlackCount; }
	VOID						Relation_SetEnemyNum(UINT uEnemyCount)		{ m_uEnemyCount = uEnemyCount; }

	/** ���һ������ */
	VOID						Relation_AddFriend(INT index, _OWN_RELATION* pRelation);	
	/** ���һ�������� */
	VOID						Relation_AddBlack(INT index, _OWN_RELATION* pRelation);	
	//
	VOID						Relation_AddEnemy(INT index, _OWN_RELATION* pRelation);

	/** ɾ��һ������ */
	VOID						Relation_DelFriend();	
	/** ɾ��һ�������� */
	VOID						Relation_DelBlack();
	//
	VOID						Relation_DelEnemy();


	/** ����������ȡ�������� */
	BOOL						Relation_GetFriend(INT index, _OWN_RELATION* pRelation);
	/** ����������ȡ���������� */
	BOOL						Relation_GetBlack(INT index, _OWN_RELATION* pRelation);
	//
	BOOL						Relation_GetEnemy(INT index, _OWN_RELATION* pRelation);

	/** ������������ȡ���� */
	BOOL						Relation_GetFriendByName( LPCTSTR name, _OWN_RELATION* pRelation );
	BOOL						Relation_GetBlackByName( LPCTSTR name, _OWN_RELATION* pRelation );
	BOOL						Relation_GetEnemyByName( LPCTSTR name, _OWN_RELATION* pRelation );


	/** ������� */
	VOID						Relation_SetNewGroup( INT index, LPCTSTR name );
	/** ��ȡ���� */
	LPCTSTR						Relation_GetNewGroup( INT index );

	//���������Ϊ���ѵ�GUID 
	VOID						Relation_SetInviteName( const CHAR* nInviteName )
	{
		strncpy( m_FriendInviteName, nInviteName, MAX_CHARACTER_NAME-1);
	}
	//��������Ϊ���ѵ�����
	const CHAR*						Relation_GetInviteName()				{ return m_FriendInviteName; }

//---------------------------------------------------------------------------------------------------
/** �����������
 */

	/** ������������Ϣ */
	VOID						Finger_ClearInfo();

	/** ���������Ϣ */
	FingerPlayerInfo_t*			Finger_GetPlayerInfoByIndex(INT nIndex);
	VOID						Finger_SetPlayerInfoByIndex(INT nIndex, FingerPlayerInfo_t* pPalyerInfo);

	/** ������� */
	INT							Finger_GetPlayerInfoNum()						{ return m_FingerPlayerNum;}
	VOID						Finger_SetPlayerInfoNum(INT iFingerPlayerNum)	{ m_FingerPlayerNum = iFingerPlayerNum;}



//---------------------------------------------------------------------------------------------------
/** ����̵����ݷ���
 */
	
	BOOL						PlayerShop_GetMeIsOwner()					{return m_bMeIsOwner;}
	VOID						PlayerShop_SetMeIsOwner(BOOL bOwner)		{m_bMeIsOwner = bOwner;}

	INT							PlayerShop_GetNpcID()						{return m_nPSNpcID;}
	VOID						PlayerShop_SetNpcID(INT nPSNpcID)			{m_nPSNpcID = nPSNpcID;}

	INT							PlayerShop_GetShopNum()						{return m_nShopNum;}
	VOID						PlayerShop_SetShopNum(INT nShopNum)			{m_nShopNum = nShopNum;}

	FLOAT						PlayerShop_GetCommercialFactor()			{return m_CommerFactor;}
	VOID						PlayerShop_SetCommercialFactor(FLOAT fComFactor)	{ m_CommerFactor = fComFactor;}

	UINT						PlayerShop_GetCost()						{return m_PlayerShopApplyInfo.m_Cost;}
	VOID						PlayerShop_SetCost(UINT uCost)				{m_PlayerShopApplyInfo.m_Cost = uCost;}

	BYTE						PlayerShop_GetType()						{return m_PlayerShopApplyInfo.m_Type;}
	VOID						PlayerShop_SetType(BYTE nType)				{m_PlayerShopApplyInfo.m_Type = nType;}

	LPCTSTR						PlayerShop_GetShopNameByIndex(UINT uIndex);
	VOID						PlayerShop_SetShopNameByIndex(UINT uIndex, LPCTSTR pszShopName);

	LPCTSTR						PlayerShop_GetShopDescByIndex(UINT uIndex);
	VOID						PlayerShop_SetShopDescByIndex(UINT uIndex, LPCTSTR pszShopDesc);

	LPCTSTR						PlayerShop_GetShopFoundedTimeByIndex(UINT uIndex);
	VOID						PlayerShop_SetShopFoundedTimeByIndex(UINT uIndex, LPCTSTR pszTime);

	BYTE						PlayerShop_GetStallNumOpened(UINT uIndex);
	VOID						PlayerShop_SetStallNumOpened(UINT uIndex, BYTE nStallNumOpened);

	BYTE						PlayerShop_GetStallNumOnSale(UINT uIndex);
	VOID						PlayerShop_SetStallNumOnSale(UINT uIndex, BYTE nStallNumOnSale);

	_PLAYERSHOP_GUID			PlayerShop_GetIDByIndex(UINT uIndex);
	VOID						PlayerShop_SetIDByIndex(UINT uIndex, _PLAYERSHOP_GUID nId);

	BYTE						PlayerShop_GetIsFavorByIndex(UINT uIndex);
	VOID						PlayerShop_SetIsFavorByIndex(UINT uIndex, BYTE bIsFavor);

	INT							PlayerShop_GetStallEnableByIndex(BOOL bIsMine, UINT nStallIndex);
	VOID						PlayerShop_SetStallEnableByIndex(BOOL bIsMine, UINT nStallIndex, BYTE bIsEnable);

	BYTE						PlayerShop_GetTypeByIndex(UINT uIndex);
	VOID						PlayerShop_SetTypeByIndex(UINT uIndex, BYTE nType);

	LPCTSTR						PlayerShop_GetOwnerNameByIndex(UINT uIndex);
	VOID						PlayerShop_SetOwnerNameByIndex(UINT uIndex, LPCTSTR pszOwnerName);

	UINT						PlayerShop_GetOwnerGuidByIndex(UINT uIndex);
	VOID						PlayerShop_SetOwnerGuidByIndex(UINT uIndex, UINT uOwnerGuid);

	/** �������̵� */
	VOID						PlayerShop_CleanUp(BOOL bIsMine);

	// �����̵�ID
	VOID						PlayerShop_SetShopID(BOOL bIsMine, _PLAYERSHOP_GUID nShopID);
	/** ����̵�ID */
	_PLAYERSHOP_GUID			PlayerShop_GetShopID(BOOL bIsMine);

	// �����̵����к�
	VOID						PlayerShop_SetShopSerial(BOOL bIsMine, BYTE nShopSerial);
	// ����̵����к�
	BYTE						PlayerShop_GetShopSerial(BOOL bIsMine);

	//����̯λ��Ʒ
	VOID						PlayerShop_SetItem(BOOL bIsMine, INT nStallIndex, INT nBoxIndex, tObject_Item* pItem, BOOL bClearOld=TRUE);
	//����ƶ�λ�õ���Ʒ
	tObject_Item*				PlayerShop_GetItem(BOOL bIsMine, INT nStallIndex, INT nBoxIndex);

	//������Ʒ�۸�
	VOID						PlayerShop_SetItemPrice(BOOL bIsMine, INT nStallIndex, INT nBoxIndex, UINT nPrice);
	//�����Ʒ�۸�
	UINT						PlayerShop_GetItemPrice(BOOL bIsMine, INT nStallIndex, INT nBoxIndex);

	//������Ʒ���к�
	VOID						PlayerShop_SetItemSerial(BOOL bIsMine, INT nStallIndex, INT nBoxIndex, UINT nSerial);
	//�����Ʒ���к�
	UINT						PlayerShop_GetItemSerial(BOOL bIsMine, INT nStallIndex, INT nBoxIndex);

	//�������Ƿ��ϼ�
	VOID						PlayerShop_SetItemOnSale(BOOL bIsMine, INT nStallIndex, INT nBoxIndex, BYTE bIsEnable);
	//�������Ƿ��ϼ�
	BYTE						PlayerShop_GetItemOnSale(BOOL bIsMine, INT nStallIndex, INT nBoxIndex);

	//�����̵걾��
	VOID						PlayerShop_SetBaseMoney(BOOL bIsMine, UINT uBaseMoney);
	//�����̵걾��
	UINT						PlayerShop_GetBaseMoney(BOOL bIsMine);

	//�����̵�ӯ����
	VOID						PlayerShop_SetProfitMoney(BOOL bIsMine, UINT uProfitPrice);
	//�����̵�ӯ����
	UINT						PlayerShop_GetProfitMoney(BOOL bIsMine);

	//ֻ���Լ��ĵ���Կ���
	INT							PlayerShop_GetPartnerNum()					{ return m_nPartnerNum;}
	VOID						PlayerShop_SetPartnerNum(INT nPartnerNum)	{ m_nPartnerNum = nPartnerNum;}

	//����ָ��λ�õĺ������
	VOID						PlayerShop_SetPartnerByIndex(UINT uIndex, GUID_t Guid, LPCTSTR szName);
	//���ָ��λ�õĺ������
	PlayerShopPartners_t* 		PlayerShop_GetPartnerByIndex(UINT uIndex);

	//�����̵�����
	VOID						PlayerShop_SetShopType(BOOL bIsMine, INT nShopType);
	//�����̵�����
	UINT						PlayerShop_GetShopType(BOOL bIsMine);

	//�����̵���̵��Ƿ��ǳ���״̬
	VOID						PlayerShop_SetIsSaleOut(BOOL bIsMine, BOOL bIsSaleOut);
	//�����̵���̵��Ƿ��ǳ���״̬
	BOOL						PlayerShop_GetIsSaleOut(BOOL bIsMine);

	//�����̵���̵��Ƿ��ǳ���״̬
	VOID						PlayerShop_SetSaleOutPrice(BOOL bIsMine, UINT uSaleOutPrice);
	//����̵���̳���
	UINT						PlayerShop_GetSaleOutPrice(BOOL bIsMine);

	//���õ���guid
	VOID						PlayerShop_SetOwnerGuid(BOOL bIsMine, UINT uOwnerGuid);
	//��õ���guid
	UINT						PlayerShop_GetOwnerGuid(BOOL bIsMine);

	//���ý��׼�¼����
	VOID						PlayerShop_SetExRecNum(BOOL bIsMine, UINT uExRecNum);
	//��ý��׼�¼����
	UINT						PlayerShop_GetExRecNum(BOOL bIsMine);

	//���ù����¼����
	VOID						PlayerShop_SetMaRecNum(BOOL bIsMine, UINT uMaRecNum);
	//��ù����¼����
	UINT						PlayerShop_GetMaRecNum(BOOL bIsMine);

	//�����Ѿ�ӵ�еĹ�̨��
	VOID						PlayerShop_SetOpenStallNum(BOOL bIsMine, UINT uOpenStallNum);
	//����Ѿ�ӵ�еĹ�̨��
	UINT						PlayerShop_GetOpenStalldNum(BOOL bIsMine);

	//�����̵������
	VOID						PlayerShop_SetShopSubType(BOOL bIsMine, UINT uShopSubType);
	//����̵������
	UINT						PlayerShop_GetShopSubType(BOOL bIsMine);

	//���õ�����
	VOID						PlayerShop_SetOwnerName(BOOL bIsMine, LPCTSTR pszOwnerName);
	//��õ�����
	LPCTSTR						PlayerShop_GetOwnerName(BOOL bIsMine);

	//�����̵���
	VOID						PlayerShop_SetShopName(BOOL bIsMine, LPCTSTR pszShopName);
	//����̵���
	LPCTSTR						PlayerShop_GetShopName(BOOL bIsMine);

	//�����̵�����
	VOID						PlayerShop_SetShopDesc(BOOL bIsMine, LPCTSTR pszShopDesc);
	//����̵���
	LPCTSTR						PlayerShop_GetShopDesc(BOOL bIsMine);

	//������Ʒ��ϸ��Ϣ
	VOID						PlayerShop_SetItemExtraInfo(BOOL bIsMine, INT nStallIndex, INT nBoxIndex, BOOL bEmpty, const _ITEM* pItem);
	//���̯λ�е���Ʒ����
	INT							PlayerShop_GetItemNum(BOOL bIsMine, INT nStallIndex);
	//����GUID������Ʒλ��
	INT							PlayerShop_GetItemIndexByGUID(BOOL bIsMine, INT nStallIndex, __int64 ItemGUID);

	SDATA_PET*					PlayerShop_GetPet(BOOL bIsMine, INT nStallIndex, INT nIndex);

	//����GUID������̵�ָ����̨�в�����Ʒ
	INT							PlayerShop_GetPetByGuid(BOOL bIsMine, INT nStallIndex, PET_GUID_t PetGuid);
	//��ָ����̨�����ó���
	VOID						PlayerShop_SetPet(BOOL bIsMine, INT nStallIndex, INT nIndex, SDATA_PET* pPetData, BOOL bClearOld = TRUE);
	//����ָ����̨�ĸ������к�
	VOID						PlayerShop_SetPetSerial(BOOL bIsMine, INT nStallIndex, INT nIndex, INT nSerial);
	//���ָ����̨�ĸ������к�
	INT							PlayerShop_GetPetSerial(BOOL bIsMine, INT nStallIndex, INT nIndex);
	VOID						PlayerShop_SetPetPrice(BOOL bIsMine, INT nStallIndex, INT nIndex, INT nPetPrice);
	INT							PlayerShop_GetPetPrice(BOOL bIsMine, INT nStallIndex, INT nIndex);
	VOID						PlayerShop_SetSkill(BOOL bIsMine, INT nStallIndex, INT nIndex, INT nSkillIndex, PET_SKILL* pPetSkillData, BOOL bClearOld = TRUE);	
	VOID						PlayerShop_SetSkill(BOOL bIsMine, INT nStallIndex, INT nIndex,INT nSkillIndex, const _OWN_SKILL *Skill);
	PET_SKILL*					PlayerShop_GetSkill(BOOL bIsMine, INT nStallIndex, INT nIndex,INT nSkillIndex);

	//���
	VOID						PlayerShop_CleanObjItem(BOOL bIsMine);

	//����̵���ѡ�е���Ʒ
	VOID						PlayerShop_MyClearSelect()					{ m_MyPsSelectItem.m_nConIndex = -1;m_MyPsSelectItem.m_nPosition = -1;}
	VOID						PlayerShop_OtClearSelect()					{ m_OtPsSelectItem.m_nConIndex = -1;m_OtPsSelectItem.m_nPosition = -1;}

	INT							PlayerShop_GetMySelectConTa()				{ return m_MyPsSelectItem.m_nConIndex;}
	INT							PlayerShop_GetMySelectPos()					{ return m_MyPsSelectItem.m_nPosition;}
	INT							PlayerShop_GetOtSelectConTa()				{ return m_OtPsSelectItem.m_nConIndex;}
	INT							PlayerShop_GetOtSelectPos()					{ return m_OtPsSelectItem.m_nPosition;}

	VOID						PlayerShop_SetMySelectConTa(INT nConIndex)	{ m_MyPsSelectItem.m_nConIndex = nConIndex;}
	VOID						PlayerShop_SetMySelectPos(INT nPosition)	{ m_MyPsSelectItem.m_nPosition = nPosition;}
	VOID						PlayerShop_SetOtSelectConTa(INT nConIndex)	{ m_OtPsSelectItem.m_nConIndex = nConIndex;}
	VOID						PlayerShop_SetOtSelectPos(INT nPosition)	{ m_OtPsSelectItem.m_nPosition = nPosition;}


//---------------------------------------------------------------------------------------------------
/** �����б�
 */
	
	//������й�����Ϣ
	VOID						Guild_ClearAllInfo();
	//ͨ��������ù�����Ϣ
	GuildInfo_t*				Guild_GetInfoByIndex(INT nIndex);
	//ͨ���������ù�����Ϣ
	VOID						Guild_SetInfoByIndex(INT nIndex, GuildInfo_t* pGuildInfo);

	//��ù�������
	INT							Guild_GetInfoNum()							{ return m_GuildNum;}
	//���ù�������
	VOID						Guild_SetInfoNum(INT iGuildNum)				{ m_GuildNum = iGuildNum;}


/** ���ɳ�Ա�б�
 */
	
	/** ������а�����Ϣ */
	VOID						Guild_ClearMemInfo();
	//ͨ��������ð�����Ϣ
	GuildMemberInfo_t*			Guild_GetMemInfoByIndex(INT nIndex);
	//ͨ���������ð�����Ϣ
	VOID						Guild_SetMemInfoByIndex(INT nIndex, GuildMemberInfo_t* pMemberInfo);

	//��ð�����Ϣ
	INT							Guild_GetMemInfoNum()						{ return m_GuildMemNum;}
	//���ð�����Ϣ
	VOID						Guild_SetMemInfoNum(INT iMemNum)			{ m_GuildMemNum = iMemNum;}

	//��ð��ڵ��������
	INT							Guild_GetMaxMemNum()						{ return m_GuildMaxMemNum;}
	//���ð��ڵ��������
	VOID						Guild_SetMaxMemNum(INT iMaxMemNum)			{ m_GuildMaxMemNum = iMaxMemNum;}

	//��ð�����ּ
	LPCTSTR						Guild_GetDesc()								{return m_GuildDesc.c_str();}
	//���ð�����ּ
	VOID						Guild_SetDesc(LPCTSTR pGuildDesc)			{m_GuildDesc = pGuildDesc;}

	//��ð�����ּ
	LPCTSTR						Guild_GetName()								{return m_GuildName.c_str();}
	//���ð�����ּ
	VOID						Guild_SetName(LPCTSTR pGuildName)			{m_GuildName = pGuildName;}

	//��õ�ǰ����Ȩ��
	BYTE						Guild_GetCurAccess()						{return m_uAccess;}
	//���õ�ǰ����Ȩ��
	VOID						Guild_SetCurAccess(BYTE uAccess)			{m_uAccess = uAccess;}

	//��õ�ǰ����ְλ
	BYTE						Guild_GetCurPosition()						{return m_uPosition;}
	//���õ�ǰ����ְλ
	VOID						Guild_SetCurPosition(BYTE uPosition)		{m_uPosition = uPosition;}


/** ���ɵ���ϸ��Ϣ
 */

	//��հ��ɵ���ϸ��Ϣ
	VOID						Guild_ClearDetailInfo();
	//��ð��ɵ���ϸ��Ϣ
	GuildDetailInfo_t*			Guild_GetDetailInfo();
	//���ð��ɵ���ϸ��Ϣ
	VOID						Guild_SetDetailInfo(GuildDetailInfo_t* pDetailInfo);


/** ���ɿ�����ְλ
 */
	
	//��հ��ɿ�����ְλ
	VOID						Guild_ClearAppointInfo();
	//��ð��ɿ�����ְλ
	GuildAppointPos_t*			Guild_GetAppointInfoByIndex(INT nIndex);
	//���ð��ɿ�����ְλ
	VOID						Guild_SetAppointInfoByIndex( INT nIndex, GuildAppointPos_t* pDetailInfo);


/** ���������Ϣ
*/

	// ���������ߺ������ID
	VOID						Guild_SetInviteGuildId( INT nGuildId );
	VOID						Guild_SetInviteGuildName( LPCTSTR pGuildName );
	VOID						Guild_SetInviteGuildObjName( LPCTSTR pGuildObjName );
	// 
	INT							Guild_GetInviteGuildId( VOID );
	LPCTSTR						Guild_GetInviteGuildName( VOID );
	LPCTSTR						Guild_GetInviteGuildObjName( VOID );


	// ���ͻ�ȡ��Ա��Ϣ����Ϣ
	VOID						Guild_AskInfo( INT index );



/**	��������Ϣ
*/

	//��ü�������
	INT							Guild_GetFamilyNum()								{ return m_iGuildFamilyNum;}
	//���ü�������
	VOID						Guild_SetFamilyNum(INT iGuildFamilyNum)				{ m_iGuildFamilyNum = iGuildFamilyNum;}

	//��հ��ɼ�����Ϣ
	VOID						Guild_ClearFamilyInfo();
	//��ð��ɼ�����Ϣ
	GuildFamilyInfo*			Guild_GetFamilyInfoByIndex(INT nIndex);
	//���ð��ɵ���ϸ��Ϣ
//	VOID						Guild_SetDetailFamilyInfo(GuildFamilyInfo* pDetailInfo);


	//ͨ���������ü�����Ϣ
	VOID						Guild_SetFamilyMemInfoByIndex(INT nIndex, GuildFamilyInfo* pFamilyInfo);

	// 
	VOID						Guild_SetSelectFamilyId( INT familyId )				{ m_iSelectFamilyId = familyId; };
	INT							Guild_GetSelectFamilyId()							{ return m_iSelectFamilyId; };



	// ����ļ�������
	VOID						Guild_AddFamilyWindowData( INT index, STRING winName );
	VOID						Guild_DelFamilyWindowData( INT index );
	STRING						Guild_GetFamilyWindowData( INT index );
	


//---------------------------------------------------------------------------------------------------

/** �ƺ�����
 */


	//�ƺ�����
	VOID						Title_SetTitleNum( INT nType, INT num );
	INT							Title_GetTitleNum( INT nType );
	//��ǰ�ƺ�
	VOID						Title_SetCurTitle( INT nType, INT id );
	INT							Title_GetCurTitle( INT nType );
	//���óƺ�
//	VOID						Title_SetCountry( INT index, _TITLE_* title );
	VOID						Title_SetTitle( INT nType, INT nIndex, INT nTitleIndex );
	INT							Title_GetTitle( INT nType, INT index );		// ȡ�����������������б�������������
	
	//temp
/*
	//����ƺ�����
	VOID						Title_SetGuildNum( INT num )						{ m_iGuildTitleNum = num; };
	INT							Title_GetGuildNum( VOID )							{ return m_iGuildTitleNum;}
	// 
	VOID						Title_SetCurGuild( INT id )							{ m_iCurrGuildTitleId = id; };
	INT							Title_GetCurGuild( VOID )							{ return m_iCurrGuildTitleId; };
	//���ü���ƺ�
	VOID						Title_SetGuild( INT index, _TITLE_* title );
	_TITLE_*					Title_GetGuild( INT index );


	//����淨�ƺ�����
	VOID						Title_SetWanfaNum( INT num )						{ m_iWanfaTitleNum = num; };
	INT							Title_GetWanfaNum( VOID )							{ return m_iWanfaTitleNum; }
	// 
	VOID						Title_SetCurWanfa( INT id )							{ m_iCurrWanfaTitleId = id; };
	INT							Title_GetCurWanfa( VOID )							{ return m_iCurrWanfaTitleId; };
	//�����淨�ƺ�
	VOID						Title_SetWanfa( INT index, _TITLE_* title );
	_TITLE_*					Title_GetWanfa( INT index );
*/

/*
	//�ƺ�����
	VOID						Title_SetNum( INT num, INT nType );
	INT							Title_GetNum( INT nType );
	//��ǰ�ƺ�id
	VOID						Title_SetCur( INT id, INT nType );
	INT							Title_GetCur( INT nType );
	//�ƺ�
	VOID						Title_Set( INT index, INT nType, _TITLE_* title );
	_TITLE_*					Title_Get( INT index, INT nType );
*/


	
//---------------------------------------------------------------------------------------------------

/** ��������
 */

	VOID						Country_Set( INT nType, STRING strName );
	CountryFunctionaryInfo*		Country_Get( INT nType );



//---------------------------------------------------------------------------------------------------

/** �������ɼ���ѧϰ���ݷ���
 */

	VOID						PetSkillStudy_MenPaiList_AddSkill(INT skillId);
	VOID						PetSkillStudy_MenPaiList_Clear();
	PET_SKILL*					PetSkillStudy_MenPaiList_Get(INT idx);	//idx �� m_petSkillStudyList ������
	PET_SKILL*					PetSkillStudy_MenPaiList_Get_BySkillId(INT sdx); //sdx �� PET_SKILL�е� m_pDefine->m_nID


/**	���﹫���
 */
	
	INT							PetPlacard_GetItemCount(VOID)const;
	const _PET_PLACARD_ITEM*	PetPlacard_GetItem(INT nIndex)const;
	VOID						PetPlacard_AddItem(const _PET_PLACARD_ITEM *pItem);
	VOID						PetPlacard_CleanUp();



//---------------------------------------------------------------------------------------------------

/** �������
 */

	virtual INT					PI_GetMyInfoType()								{return m_MyPrivateInfo.m_nInfoType;}
	virtual INT					PI_GetMyAge()									{return m_MyPrivateInfo.m_nAge;}
	virtual INT					PI_GetMySex()									{return m_MyPrivateInfo.m_nSex;}
	virtual INT					PI_GetMyBloodType()								{return m_MyPrivateInfo.m_nBloodType;}
	virtual INT					PI_GetMyYearAnimal()							{return m_MyPrivateInfo.m_nYearAnimal;}
	virtual INT					PI_GetMyConsella()								{return m_MyPrivateInfo.m_nConsella;}
	virtual INT					PI_GetMyProvince()								{return m_MyPrivateInfo.m_nProvince;}

	virtual LPCTSTR				PI_GetMyJobInfo()								{return m_MyPrivateInfo.m_szJobInfo.c_str();}
	virtual LPCTSTR				PI_GetMySchoolInfo()							{return m_MyPrivateInfo.m_szSchoolInfo.c_str();}
	virtual LPCTSTR				PI_GetMyCity()									{return m_MyPrivateInfo.m_szCity.c_str();}
	virtual LPCTSTR				PI_GetMyEmailInfo()								{return m_MyPrivateInfo.m_szEmailInfo.c_str();}
	virtual LPCTSTR				PI_GetMyLuckWord()								{return m_MyPrivateInfo.m_szLuckWord.c_str();}

	virtual INT					PI_GetOtInfoType()								{return m_OtPrivateInfo.m_nInfoType;}
	virtual INT					PI_GetOtAge()									{return m_OtPrivateInfo.m_nAge;}
	virtual INT					PI_GetOtSex()									{return m_OtPrivateInfo.m_nSex;}
	virtual INT					PI_GetOtBloodType()								{return m_OtPrivateInfo.m_nBloodType;}
	virtual INT					PI_GetOtYearAnimal()							{return m_OtPrivateInfo.m_nYearAnimal;}
	virtual INT					PI_GetOtConsella()								{return m_OtPrivateInfo.m_nConsella;}
	virtual INT					PI_GetOtProvince()								{return m_OtPrivateInfo.m_nProvince;}

	virtual LPCTSTR				PI_GetOtName()									{return m_OtPrivateInfo.m_szName.c_str();}
	virtual LPCTSTR				PI_GetOtJobInfo()								{return m_OtPrivateInfo.m_szJobInfo.c_str();}
	virtual LPCTSTR				PI_GetOtSchoolInfo()							{return m_OtPrivateInfo.m_szSchoolInfo.c_str();}
	virtual LPCTSTR				PI_GetOtCity()									{return m_OtPrivateInfo.m_szCity.c_str();}
	virtual LPCTSTR				PI_GetOtEmailInfo()								{return m_OtPrivateInfo.m_szEmailInfo.c_str();}
	virtual LPCTSTR				PI_GetOtLuckWord()								{return m_OtPrivateInfo.m_szLuckWord.c_str();}

	virtual VOID				PI_SetMyInfoType(int nInfoType)					{m_MyPrivateInfo.m_nInfoType = nInfoType;}
	virtual VOID				PI_SetMyAge(int nAge)							{m_MyPrivateInfo.m_nAge = nAge;}
	virtual VOID				PI_SetMySex(int nSex)							{m_MyPrivateInfo.m_nSex = nSex;}
	virtual VOID				PI_SetMyBloodType(int nBloodType)				{m_MyPrivateInfo.m_nBloodType = nBloodType;}
	virtual VOID				PI_SetMyYearAnimal(int nYearAnimal)				{m_MyPrivateInfo.m_nYearAnimal = nYearAnimal;}
	virtual VOID				PI_SetMyConsella(int nConsella)					{m_MyPrivateInfo.m_nConsella = nConsella;}
	virtual VOID				PI_SetMyProvince(int nProvince)					{m_MyPrivateInfo.m_nProvince = nProvince;}

	virtual VOID				PI_SetMyJobInfo(LPCTSTR szJobInfo)				{m_MyPrivateInfo.m_szJobInfo = szJobInfo;}
	virtual VOID				PI_SetMySchoolInfo(LPCTSTR szSchoolInfo)		{m_MyPrivateInfo.m_szSchoolInfo = szSchoolInfo;}
	virtual VOID				PI_SetMyCity(LPCTSTR szCity)					{m_MyPrivateInfo.m_szCity = szCity;}
	virtual VOID				PI_SetMyEmailInfo(LPCTSTR szEmailInfo)			{m_MyPrivateInfo.m_szEmailInfo = szEmailInfo;}
	virtual VOID				PI_SetMyLuckWord(LPCTSTR szLuckWord)			{m_MyPrivateInfo.m_szLuckWord = szLuckWord;}

	virtual VOID				PI_SetOtInfoType(int nInfoType)					{m_OtPrivateInfo.m_nInfoType = nInfoType;}
	virtual VOID				PI_SetOtAge(int nAge)							{m_OtPrivateInfo.m_nAge = nAge;}
	virtual VOID				PI_SetOtSex(int nSex)							{m_OtPrivateInfo.m_nSex = nSex;}
	virtual VOID				PI_SetOtBloodType(int nBloodType)				{m_OtPrivateInfo.m_nBloodType = nBloodType;}
	virtual VOID				PI_SetOtYearAnimal(int nYearAnimal)				{m_OtPrivateInfo.m_nYearAnimal = nYearAnimal;}
	virtual VOID				PI_SetOtConsella(int nConsella)					{m_OtPrivateInfo.m_nConsella = nConsella;}
	virtual VOID				PI_SetOtProvince(int nProvince)					{m_OtPrivateInfo.m_nProvince = nProvince;}

	virtual VOID				PI_SetOtName(LPCTSTR szName)					{m_OtPrivateInfo.m_szName = szName;}
	virtual VOID				PI_SetOtJobInfo(LPCTSTR szJobInfo)				{m_OtPrivateInfo.m_szJobInfo = szJobInfo;}
	virtual VOID				PI_SetOtSchoolInfo(LPCTSTR szSchoolInfo)		{m_OtPrivateInfo.m_szSchoolInfo = szSchoolInfo;}
	virtual VOID				PI_SetOtCity(LPCTSTR szCity)					{m_OtPrivateInfo.m_szCity = szCity;}
	virtual VOID				PI_SetOtEmailInfo(LPCTSTR szEmailInfo)			{m_OtPrivateInfo.m_szEmailInfo = szEmailInfo;}
	virtual VOID				PI_SetOtLuckWord(LPCTSTR szLuckWord)			{m_OtPrivateInfo.m_szLuckWord = szLuckWord;}

	virtual UINT				PI_GetMyCharGUID()								{return m_MyPrivateInfo.m_unCharGUID;}
	virtual UINT				PI_GetOtCharGUID()								{return m_OtPrivateInfo.m_unCharGUID;}

	virtual VOID				PI_SetMyCharGUID(UINT unCharGUID) 				{m_MyPrivateInfo.m_unCharGUID = unCharGUID;}
	virtual VOID				PI_SetOtCharGUID(UINT unCharGUID) 				{m_OtPrivateInfo.m_unCharGUID = unCharGUID;}

	virtual VOID				PI_SetObjId(INT nObjId)							{m_nObjId = nObjId;}
	virtual INT					PI_GetObjId()									{return m_nObjId;}
	
	RecordList_t*				GetRecordList()									{return &m_RecordList;}



//---------------------------------------------------------------------------------------------------

/** ϵͳ��ʾ��Ϣ
 */

	// ����¼�( Param ����ֻ����ӵ�ʹ�� )
	BOOL						SystemTip_AddEventInfo( INT type, const STRING& strDesc, INT Param = -1 );
	// ɾ���¼�
	BOOL						SystemTip_DelEventInfo( INT nIndex );

	// ��ȡĳһ�¼�����Ϣ
	SystemTipEvent*				SystemTip_GetEventInfo( INT nIndex );
	// ����ĳһ�¼���״̬
	BOOL						SystemTip_SetEventState(  );

	// ��ȡ��ǰ��������
	INT							SystemTip_GetUIIndex();

	// �������ͺͲ�������ȡ��Ӧ���¼���UI������
	INT							SystemTip_GetIndexByType( INT type, INT Param );


//---------------------------------------------------------------------------------------------------
// �Ʋ���������

	VOID						Rich_SetPasswordData( INT data )				{ m_PasswordData = data; }
	INT							Rich_GetPasswordData( VOID )					{ return m_PasswordData; }

	BOOL						Rich_IsSet( MINORPASSWD_INFO::PROTECT_TYPE type );

	// ��������룬 �������뱣��ʱ����������
	VOID						Rich_SetInputPassword( const STRING& passwd )	{ m_InputPassword = passwd; };
	STRING						Rich_GetInputPassword( VOID )					{ return m_InputPassword; };

	// �������������״̬�����ࣩ
	VOID						Rich_SetInputPasswdState( INT nType, BOOL bState );
	BOOL						Rich_GetInputPasswdState( INT nType );




//---------------------------------------------------------------------------------------------------
/** �������
 */

	//temp ֻ�Ժ���һ�����

	// �������
	void SetMouseCmd();
	// ȡ���������
	void CancelMouseCmd();



//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
//���ݴ洢			

protected:


/** ��ɫ���Գ�
 */

	/// ���н�ɫ���߼�����
	std::map< INT, CCharacterData* >	m_mapAllCharacterData;


/** ��ȴʱ����
 */

	std::vector< COOLDOWN_GROUP >		m_vCoolDownGroup;
	std::vector< COOLDOWN_GROUP >		m_vPetSkillCoolDownGroup;
	
/** ������ȴ
 */

	INT									m_nCommonCoolDown;
	
/** ����ʣ��ʱ����
 */

	std::vector< INT >					m_vQuestTimeGroup;
	
/** ����Լ�װ��
 */

	/// ���װ��ʵ������
	std::vector< tObject_Item* >		m_vUserEquip;

/** ����װ��
 */

	/// ����װ��ʵ������
	std::vector< tObject_Item* >		m_vOtherPlayerEquip;

/** �����б����
 */

	/// ��������������Ʒ
	struct QUESTLOG_BOX
	{
		std::vector<tObject_Item*>		m_ItemList;					// ��ʾ���������Լ�����Ʒ
		VOID CleanUp();
	};

	std::vector< QUESTLOG_BOX >			m_vQuestLogItem;

/** �鿴�Է���ɫ���Խ���
 */

	/// �Է���Obj
	CObject*							m_pTargetEquip;


/** ���Ǳ����б�
 */

	/// ��ұ���װ��ʵ������
	std::vector< tObject_Item* >		m_vUserBag;

	/// ��ϸ�������
	std::set< INT >						m_setAskBagExtra;


/** ���������б�  [7/8/2010 �¾���] 
*/

	/// �������ʵ������
	std::vector< tObject_Item* >		m_vUserMount;


/** ��������¼�����б�
*/
	
	///�������¼������ϸ��Ϣʵ������
	SOUXIA_DATA*	m_vUserSouXia[MAX_SOUXIA_CONTAINER];

	BYTE		m_nSouXiaCount;		//ѧϰ��������¼����
	BYTE		m_nSouXiaTypeCount;		//������¼���е���������



/** ��ǰ�򿪵�����
 */

	/// ����ID
	INT									m_idItemBox;
	/// ������Ʒʵ������
	std::vector< tObject_Item* >		m_vItemBox;


	/** ��ǰ�������Ʒ
	*/

	INT									m_idItemDrop;

	///���� ��Ʒʵ��
	std::map< INT,tObject_Item*>  		m_vItemDrop;


/** ��ǰ�򿪵�����
 */

	INT									m_nBoothNumber;
	INT									m_nBoothSoldNumber;
	std::vector< tObject_Item* >		m_listBooth;
	std::vector< tObject_Item* >		m_listBoothSold;
	BOOL								m_bBoothWindowIsDirty;
//	BOOL								m_bIsRepairing;
	std::vector< UINT >					m_nlistPrice;
	INT									m_nRepairLevel;		/// ����ȼ�
	INT									m_nBuyLevel;		/// �չ��ȼ�
	INT									m_nRepairType;		/// ��������
	INT									m_nBuyType;			/// �̵���չ�����
	FLOAT								m_nRepairSpend;		/// ������
	FLOAT								m_nRepairOkProb;	/// ����ɹ�����

	/// ��ǰShop��NpcId
	INT									m_nShopNpcId;		// 
	
	// add for IB Shop
	INT									m_nIBShopNumber;
	std::vector< tObject_Item* >		m_listIBShop;
	std::vector< UINT >					m_nlistIBPrice;
	std::vector< UINT >					m_nlistNewSell;		//�²�Ʒ

	// end of  add

/** ��ǰ�򿪵�����
 */

	/// ����������Ʒ�б�
	std::vector< tObject_Item* >		m_listUserBank;
	/// ��ǰ�������Ѿ��򿪵�λ��
	INT									m_CurBankEndIndex;
	/// ��ǰ������Ǯ��
	INT									m_CurBankMoney;
	/// ��ǰ����NPC��ID
	INT									m_nBankNpcID;


/** ��ϵϵͳ����
 */
	Relation*							m_pRelation;
	CMailPool*							m_pMailPool;

	
/** ���׺�
 */

	struct  CLIENT_EXCHANGE_BOX
	{
		BOOL							m_IsLocked;						/// ��ʾ������lockѡ���Ƿ���
		BOOL							m_CanConform;					/// ��ʾ�Ƿ���ʾȷ����ť
		UINT							m_Money;						/// ��ʾ���׵Ľ�Ǯ

		std::vector<tObject_Item*>		m_ItemList;						/// ��ʾ���������Լ�����Ʒ
		std::vector< SDATA_PET* >		m_Petlist;						/// ��ʾ���������Լ��ĳ���

		VOID CleanUp();
	};

	CLIENT_EXCHANGE_BOX					m_MyExchangeBox;				/// �Լ��Ľ��׺�
	CLIENT_EXCHANGE_BOX					m_OtherExchangeBox;				/// �Է��Ľ��׺�
	INT									m_CurExID;
	std::vector< INT >					m_listApplicantsID;				/// ���뽻������
	SHORT								m_pOut;							/// �Ӷ�����ȡ����λ��ָ��
	SHORT								m_pIn;							/// ������е�λ��ָ��


	//��ʯ�ϳ�/��Ƕ��������
//	std::vector< tObject_Item* >		m_vPageEnchase;

	// װ���ϸ�����Ʒ�����飬�Ժ��ټ�
//	std::vector< tObject_Item* >		m_vAppendItem;					/// ���ӵ���Ʒ��


/** �����
 */

	std::vector<tObject_Item*>			m_MissionItemList;				/// �ݽ�������Ʒ����


/** BBS�ϵ�����
 */

	struct MessageEntry_t
	{
		STRING	AuthorName; 
		UINT	nID;
		BYTE	nHour;
		BYTE	nMin;
		BOOL	bHasReply;
		STRING	szMessage;
		BYTE	nReHour;
		BYTE	nReMin;
		STRING	szReplyMessage;

		MessageEntry_t()
		{
			AuthorName		=	"";		
			nID				=	0;
			nHour			=	0;
			nMin			=	0;
			bHasReply		=	0;
			szMessage		=	"";
			nReHour			=	0;
			nReMin			=	0;
			szReplyMessage	=	"";
		}
	};

	struct BBS_t
	{
		BBS_t()
		{
			CleanUp();
		}
		VOID	CleanUp();

		STRING							m_szTitle;
		UINT							m_nSerial;
		INT								m_nMessageNum;
		std::vector< MessageEntry_t >	m_MessageList;
		
	};


/** ̯λ��
 */

	enum STALL_DEFAULT_PAGE
	{
		ITEM_PAGE = 0,     //��Ʒҳ
		PET_PAGE,
	};

	struct STALL_ITEM
	{
		STALL_ITEM()
		{
			pItem	= NULL;
			nPrice	= 0;
			nSerial = 0;
			nbIsEnable	=	0;
		}
		tObject_Item*	pItem;			/// ��Ʒָ��
		UINT			nPrice;			/// �۸�
        UINT			nSerial;		/// ���к�
		BYTE			nbIsEnable;		/// �����Ʒ�Ƿ��ϼ�
	};

	struct STALL_PET
	{
		STALL_PET()
		{
			pPet	= NULL;
			nPrice	= 0;
			nSerial = 0;
			nbIsEnable	=	TRUE;
		}
		SDATA_PET*	pPet;			/// ����ָ��
		UINT		nPrice;			/// �۸�
		UINT		nSerial;		/// ���к�
		BYTE		nbIsEnable;		/// �����Ʒ�Ƿ��ϼ�
	};

	struct CLIENT_STALL_BOX
	{
		INT								m_nDefaultPage;			/// ȱʡ��ҳ��
		UINT							m_ObjID;
		BOOL							m_bIsOpen;				/// �Լ�̯λ�Ƿ��Ѿ��򿪣��Ƿ��ڽ���״̬��
		UINT							m_PosTax;				/// ̯λ��
		UINT							m_TradeTax;				/// ÿ�ν���˰��
		STRING							m_szStallName;			/// ��ʾ̯λ��
		STRING							m_szOwnerName;			/// ��ʾ̯����
		UINT							m_nFirstPage;			/// ��ʼҳ
		std::vector< STALL_ITEM >	 	m_ItemList;				/// ��ʾ̯λ�е�������Ʒ
		std::vector< STALL_PET >	 	m_PetList;				/// ��ʾ̯λ�е����г���
		BBS_t							m_BBS;					/// ̯λ���԰�
		UINT							m_OtGUID;
		VOID							CleanUp();		
	};

	CLIENT_STALL_BOX					m_MyStallBox;			/// �Լ���̯λ��
	CLIENT_STALL_BOX					m_OtStallBox;			/// �Է���̯λ��
	STALL_ITEM							m_CurSelectItem;		/// ���ѡ�е���Ʒ
	INT									m_nSelectItemInPos;		/// ѡ�е������Ʒ���ڵ�����
	PET_GUID_t							m_CurSelectpetGuid;		/// ѡ�еȴ��ϼܵĳ����GUID
	INT									m_nSelectPetOnStall;	/// ��̯λ�ϱ�ѡ�еĳ�����


/** ����̵����
 */

	PLAYERSHOP_APPLY_INFO				m_PlayerShopApplyInfo;		/// �����̵���Ϣ

	std::vector< PLAYERSHOP_INFO >	 	m_PlayerShopList;			/// �̵��б�
	INT									m_nShopNum;

	/// ��ʾ̯λ�е�������Ʒ
	struct PLAYERSHOP_STALL_BOX										
	{
		INT								m_bIsEnable;
		std::vector< STALL_ITEM >	 	m_ItemList;
		std::vector< STALL_PET >	 	m_PetList;

		PLAYERSHOP_STALL_BOX()
		{
			m_bIsEnable = FALSE;
			STALL_ITEM	ItemInit;
			m_ItemList.resize(STALL_BOX_SIZE, ItemInit);
			STALL_PET	PetInit;
			m_PetList.resize(STALL_BOX_SIZE, PetInit);
		}

		VOID CleanUp()
		{
			m_bIsEnable = FALSE;
			for(INT i =0; i<STALL_BOX_SIZE; i++)
			{
				STALL_ITEM	ItemInit;
				m_ItemList[i] = ItemInit;

				STALL_PET	PetInit;
				m_PetList[i] = PetInit;
			}
		}
	};

	enum SHOP_TYPE
	{
		NO_SHOP = 0,
		ITEM_SHOP,
		PET_SHOP,
	};

	/// ��ʾ�̵��е����й�̨
	struct PLAYERSHOP_BOX												
	{
		std::vector< PLAYERSHOP_STALL_BOX >	m_StallList;
		UINT								m_BaseMoney;
		UINT								m_ProfitMoney;
		_PLAYERSHOP_GUID					m_ShopID;
		STRING								m_szShopName;		/// ��ʾ�̵���
		STRING								m_szShopDesc;		/// ��ʾ�̵�����
		STRING								m_szOwnerName;		/// ��ʾ������
		UINT								m_OwnerGuid;		/// ����GUID
		BYTE								m_bIsSaleOut;		/// ���̵��ѱ��̳�
		UINT								m_uSaleOutPrice;	/// �̵���̳���
		BYTE								m_Serial;			/// �̵�����к�
		INT									m_nShopType;		/// �̵������
		BYTE								m_ExRecListNum;		/// ���׼�¼����
		BYTE								m_MaRecListNum;		/// �����¼����
		BYTE								m_StallOpenNum;		/// �Ѿ�ӵ�еĹ�̨��
		BYTE								m_ShopSubType;		/// �̵�������
		std::vector< PlayerShopPartners_t > m_PartnerList;		/// �ϻ����б�
		
		PLAYERSHOP_BOX()
		{
			m_BaseMoney		= 0;
			m_ProfitMoney	= 0;
			m_szShopName	=	"";
			m_szOwnerName	=	"";
			m_szShopDesc	=	"";
			m_OwnerGuid		=	0;
			m_bIsSaleOut	=	FALSE;
			m_uSaleOutPrice	=	0;
			m_Serial		=	0;
			m_ExRecListNum	=	0;
			m_MaRecListNum	=	0;
			m_StallOpenNum	=	0;
			PLAYERSHOP_STALL_BOX StallInit;
			m_StallList.resize(MAX_STALL_NUM_PER_SHOP, StallInit);
			PlayerShopPartners_t PartnerInit;
			m_PartnerList.resize(MAX_PARTNER_PER_SHOP, PartnerInit);
			m_nShopType		= NO_SHOP;
			m_ShopSubType	=	0;
		}

		VOID CleanUp()
		{
			m_BaseMoney		= 0;
			m_ProfitMoney	= 0;
			m_szShopName	=	"";
			m_szOwnerName	=	"";
			m_OwnerGuid		=	0;
			m_bIsSaleOut	=	FALSE;
			m_uSaleOutPrice	=	0;
			m_Serial		=	0;
			m_ExRecListNum	=	0;
			m_MaRecListNum	=	0;
			m_StallOpenNum	=	0;
			m_ShopID.Reset();
			m_nShopType		=  NO_SHOP;
			m_ShopSubType	=	0;
			for(INT i =0; i<MAX_STALL_NUM_PER_SHOP; i++)
			{
				m_StallList[i].CleanUp();
			}
			for(INT i =0; i<MAX_PARTNER_PER_SHOP; i++)
			{
				m_PartnerList[i].CleanUp();
			}
		}
	};

	PLAYERSHOP_BOX		m_MyPlayerShop;			/// �Լ�������̵�
	PLAYERSHOP_BOX		m_OtPlayerShop;			/// �����˵��̵�
	FLOAT				m_CommerFactor;			/// ��ҵָ��
	BOOL				m_bMeIsOwner;			/// ��־�Լ��ǲ��ǵ���
	INT					m_nPartnerNum;			/// �����������

	struct SELECT_ITEM
	{
		INT		m_nConIndex;
		INT		m_nPosition;
		
		VOID CleanUp()
		{
			m_nConIndex = -1;
			m_nPosition = -1;
		}
	};

	SELECT_ITEM			m_MyPsSelectItem;		/// �Լ����̵������ѡ�е���Ʒ
	SELECT_ITEM			m_OtPsSelectItem;		/// ���˵��̵������ѡ�е���Ʒ

	/// ��ǰ����̵�NPC��ID
	INT						m_nPSNpcID;


	/// �ͻ�����ʾ�İ����б�
	GuildInfo_t				m_GuildList[MAX_GUILD_SIZE];
	INT						m_GuildNum;

	/// �����б�
	GuildMemberInfo_t		m_GuildMemList[USER_ARRAY_SIZE];
	INT						m_GuildMaxMemNum;
	INT						m_GuildMemNum;
	STRING					m_GuildDesc;
	STRING					m_GuildName;
	BYTE					m_uPosition;
	BYTE					m_uAccess;

	/// ���ɵ���ϸ��Ϣ
	GuildDetailInfo_t		m_GuildDetailInfo;

	/// ���������ɵ�ְλ��Ϣ
	GuildAppointPos_t		m_GuildPosAvail[GUILD_POSITION_SIZE];

	/// ����Ӱ���Ϣ
	INT						m_GuildInviteId;
	STRING					m_GuildInviteName;		// ��������İ��
	STRING					m_GuildInviteObjName;	// ��������Ķ���


	/// ������
	INT						m_iGuildFamilyNum;		// ����м��������
	INT						m_GuildFamilyMemNum;	// ����м����г�Ա������
	GuildFamilyInfo			m_FamilyList[GUILD_FAMILY_SIZE];
	INT						m_iSelectFamilyId;		// ��ǰѡ��ļ���id

	// ����ļ�������
	map< INT, STRING >			m_mFamilyWinData;


	/// �ƺ�

//	_TITLE_
	INT						m_vTitles[3][MAX_TITLE_SIZE];		// �ƺ�
	INT						m_iTitleNum[3];						// �ƺ�����
	INT						m_iCurrTitleId[3];					// ��ǰʹ�õĳƺ�id


/*
	INT						m_iTitleNum[3];			// ����
	INT						m_iCurrTitleId[3];		// ��ǰʹ�õ�

	_TITLE_					m_vTitles[MAX_TITLE_SIZE];	// �ƺ�
*/

	/// ����
	CountryFunctionaryInfo	m_CountryFunctionInfo[COUNTRY_FUNCTION_TYPE_NUM];


	/// ��Ҫ���ٵ���Ʒ��һ����ʱ������
	struct DISCARD_ITEM
	{
		INT			m_nContainer;
		INT			m_nPosition;

		VOID							CleanUp()
		{
			m_nContainer = -1;
			m_nPosition = -1;
		}
	};

	DISCARD_ITEM						m_CurDisCardItem;

	/// �����Ϣ
	struct SPLIT_ITEM
	{
		INT								m_nContainer;
		INT								m_nPosition;
		INT								m_nSum;			/// �ܸ���
		INT								m_nNumber;		/// �ֳ����ĸ���
		VOID							CleanUp()
		{
			m_nSum		 = 0;
			m_nNumber	 = 0;
			m_nContainer = -1;
			m_nPosition	 = -1;
		}

		SPLIT_ITEM()
		{
			CleanUp();
		}
	};
	SPLIT_ITEM							m_CurSplitItem;


	//������Ϣ

	struct ADD_START_EQUIP
	{
		INT								m_nContainer;    //װ�����߱���
		INT								m_nPosition;	 //װ����	
		INT                             m_GemId;         //��ʯid
        INT                             m_GemNum;		 //��ʯ����	
		INT								m_nIntensifyInfo;//ǿ������
		INT								m_nMoney;		 //�����Ǯ	
		VOID							CleanUp()
		{
			m_nContainer = -1;
			m_nPosition	 = -1;
			m_GemId      = -1;
			m_GemNum	 = -1;
			m_nIntensifyInfo = -1;  						 


		}

		ADD_START_EQUIP()
		{
			CleanUp();
		}

	};
	ADD_START_EQUIP							m_CurAddStarEquip;

	//װ���ϳ� �Ĳ���      

	struct   EQUIP_SYNTHESIZE_MATERIAL
	{

		INT                             m_materialId;     //���� id   
		INT                             m_materialNum;    //��������
		INT                             m_materialGrade;  //���ϵȼ�
	 
		VOID							CleanUp()
		{
			m_materialId = -1;
			m_materialNum	 = -1;
			m_materialGrade    = -1;	 						 

		}

		EQUIP_SYNTHESIZE_MATERIAL()
		{
			CleanUp();
		}

	};


	//װ���ϳ���Ϣ

	struct EQUIP_SYNTHESIZE
	{
		 
		 
		EQUIP_SYNTHESIZE_MATERIAL          m_Mat[MAX_EQUIP_PROMATL_NUM];
		UINT								m_MatNum;	

		INT								m_IdTable; //������һ��

		VOID							CleanUp()
		{
			m_IdTable = 0;
			m_MatNum = 0;
			for ( INT i =0 ;i<MAX_EQUIP_PROMATL_NUM;++i)
			{
				m_Mat[i].CleanUp();
			}

		} 

		EQUIP_SYNTHESIZE()
		{
			CleanUp();
		}
	 
	};
	EQUIP_SYNTHESIZE							m_CurSynEquip;



	//װ������


	//װ������Ĳ���      

	struct   EQUIP_REWORK_MATERIAL
	{

		INT                             m_materialId;     //���� id  
		INT                             m_materialNum;    //����λ��
		INT                             m_materialGrade;  //���ϵȼ�

		VOID			CleanUp()
	  {
			m_materialId = -1;   
			m_materialNum = 0;  
			m_materialGrade = -1;


		}

	};


	struct  EQUIP_REWORK

	{
		INT								m_nContainer;    //װ�����߱���
		INT								m_nPosition;	 //װ����	

		EQUIP_REWORK_MATERIAL		   m_Mat[MAX_EQUIP_RWMATL_NUM];

		INT								m_IdTable; //������һ��
		UINT							m_MatNum;	

		VOID							CleanUp()
		{
			m_nContainer	 = -1;
			m_nPosition		 = -1;
			m_IdTable  = -1;
			m_MatNum = 0;
			for ( INT i =0 ;i<MAX_EQUIP_RWMATL_NUM;++i)
			{
				m_Mat[i].CleanUp();
			}

		} 
		EQUIP_REWORK()
		{
			CleanUp();
		}


	};

	EQUIP_REWORK        m_CurEquip_ReWork;




	//װ��������


	//װ��������Ĳ���    spirit  

	struct   EQUIP_SPIRIT_REWORK_MATERIAL
	{

		INT                             m_materialId;     //���� id  
		INT                             m_materialNum;    //����λ��
		INT                             m_materialGrade;  //���ϵȼ�


		VOID							CleanUp()
		{
			m_materialId = -1;   
			m_materialNum = 0;  
			m_materialGrade = -1;


		}

	};

	struct  EQUIP_SPIRIT_REWORK
	{
		INT								m_nContainer;    //װ�����߱���
		INT								m_nPosition;	 //װ����	

		EQUIP_SPIRIT_REWORK_MATERIAL	m_Mat[MAX_EQUIP_SPIRIT_RWMATL_NUM];

		INT								m_IdTable; //������һ��
		UINT							m_MatNum;	

		VOID							CleanUp()
		{
			m_nContainer	 = -1;
			m_nPosition		 = -1;
			m_IdTable  = -1;
			m_MatNum = 0;
			for ( INT i =0 ;i<MAX_EQUIP_SPIRIT_RWMATL_NUM;++i)
			{
				m_Mat[i].CleanUp();
			}

		} 

		EQUIP_SPIRIT_REWORK()
		{
			CleanUp();
		}

	};

	EQUIP_SPIRIT_REWORK        m_CurEquip_SpiritReWork;




	// �����Ϣ
	struct ADD_HOLE_EQUIP
	{
		INT								m_nContainer;    //װ�����߱���
		INT								m_nPosition;	 //װ����	
		INT                             m_ItemId;        //����id
		INT                             m_ItemNum;		 //��������	
		INT			                    m_Money ;        //���ѽ�Ǯ��Ŀ
		INT								m_NpcId;		//���npc

		VOID							CleanUp()
		{
			m_nContainer = -1;
			m_nPosition	 = -1;
			m_ItemId     = -1;
			m_ItemNum	 = -1;
			m_Money      = -1;
			m_NpcId      = -1;


		}

		ADD_HOLE_EQUIP()
		{
			CleanUp();
		}
	};
	ADD_HOLE_EQUIP							m_CurAddHoleEquip;



	// ����Ϣ
	struct BANG_DING_EQUIP
	{
		INT								m_nContainer;    //װ�����߱���
		INT								m_nPosition;	 //װ����	
		INT                             m_ItemId;        //����id
		INT                             m_NeedItemPos;    //����λ��
	 
		 
		VOID							CleanUp()
		{
			m_nContainer = -1;
			m_nPosition	 = -1;
			m_ItemId     = -1;
			m_NeedItemPos = -1;
	


		}

		BANG_DING_EQUIP()
		{
			CleanUp();
		}
	};
	BANG_DING_EQUIP							m_CurBangDingEquip;




   // װ����������


	struct   SWAP_CARD_EQUIP
	{
		INT								m_nContainer1;    //װ�����߱���
		INT								m_nPosition1;	 //װ����	
		INT								m_nContainer2;    //װ�����߱���
		INT								m_nPosition2;	 //װ����	
		INT                             m_ItemId;        //����id
		INT                             m_NeedItemPos;    //����λ��

		VOID							CleanUp()
		{
			m_nContainer1 = -1;
			m_nPosition1 = -1;
			m_nContainer2 = -1;
			m_nPosition2 = -1;
			m_ItemId     = -1;
			m_NeedItemPos = -1;

		}


		SWAP_CARD_EQUIP()
		{
			CleanUp();
		}



	};

	SWAP_CARD_EQUIP        m_CurSwapCardEquip;


	// װ�� ���͸���


	struct   CJANGE_CARD_EQUIP
	{
		INT								m_nContainer;    //װ�����߱���
 
		INT								m_nPosition;	 //װ����	
		INT                             m_ItemId;        //����id
		INT                             m_NeedItemPos;    //����λ��

		VOID							CleanUp()
		{
			m_nContainer = -1;
			m_nPosition = -1;
			m_ItemId     = -1;
			m_NeedItemPos = -1;

		}

		CJANGE_CARD_EQUIP()
		{
			CleanUp();
		}


	};

	CJANGE_CARD_EQUIP        m_CurChangeCardEquip;




   //װ������Ĳ���      

	struct   EQUIP_DISMANTLE_MATERIAL
	{
		
		INT                             m_materialId;     //���� id   
		INT                             m_materialNum;    //��������
		INT                             m_materialGrade;  //���ϵȼ�
		INT								m_nMaterialLoss;//�����ı���4

		VOID	CleanUp()
		{
			m_materialId =-1;   
			m_materialNum = 0;  
			m_materialGrade = -1;
			m_nMaterialLoss = -1;


		}

	};

    
   //װ�����        

   struct  EQUIP_DISMANTLE

   {

	   INT								m_nContainer;    //װ�����߱���
	   INT								m_nPosition;	 //װ����	

	  EQUIP_DISMANTLE_MATERIAL          m_Mat[MAX_EQUIP_PROMATL_NUM];
	   UINT								m_MatNum;	

	  
	   INT								m_IdTable; //������һ��

	   VOID							CleanUp()
	   {
		   m_nContainer	 = -1;
		   m_nPosition	= -1;
		   m_IdTable = 0;
		   m_MatNum = 0;
		   for ( INT i =0 ;i<MAX_EQUIP_PROMATL_NUM;++i)
		   {
			   m_Mat[i].CleanUp();
		   }

	   } 

	   EQUIP_DISMANTLE()
	   {
		   CleanUp();
	   }

   };

   EQUIP_DISMANTLE        m_CurEquip_Dis;






	//������Ϣ

	//���޺�װ�����;�����=��ǰ�;�����+���޵�������*N��
	struct ADD_SPECIAL_RAPAIR
	{
		INT								m_nContainer;    //װ�����߱���
		INT								m_nPosition;	 //װ����	
		INT                             m_NeedItemId;     //����id
		INT                             m_NeedItemNum;	 //��������	
		INT								m_nIntensifyInfo;//N
		VOID							CleanUp()
		{
			m_nContainer = -1;
			m_nPosition	 = -1;
			m_NeedItemId      = -1;
			m_NeedItemNum	 = -1;
			m_nIntensifyInfo = -1;  						 

		}

		ADD_SPECIAL_RAPAIR()
		{
			CleanUp();
		}
	};
	ADD_SPECIAL_RAPAIR							m_CurSpRepairEquip;






	//������Ϣ  ���޵���Ʒ
	struct ADD_COM_RAPAIR
	{
		INT								m_nContainer;    //װ�����߱���
		INT								m_nPosition;	 //װ����	
		VOID							CleanUp()
		{
			m_nContainer = -1;
			m_nPosition	 = -1;					 
		}
		ADD_COM_RAPAIR()
		{
			CleanUp();
		}
	};
	ADD_COM_RAPAIR							m_CurComRepairEquip;



	/// �������
	struct PRIVATE_INFO
	{
		STRING		m_szName;				/// ��CleanUp�в�Ҫ���
		INT			m_nInfoType;			/// ���̶ܳ�
		UINT		m_unCharGUID;			/// ��ɫ���
		INT			m_nAge;					/// ����
		INT			m_nSex;					/// �Ա�
		STRING		m_szJobInfo;			/// ְҵ��Ϣ		[MAX_JOBINFO_LENGTH]
		STRING		m_szSchoolInfo;			/// ��ҵԺУ		[MAX_SCHOOLINFO_LENGTH]
		INT			m_nBloodType;			/// Ѫ��
		INT			m_nYearAnimal;			/// ��Ф
		INT			m_nConsella;			/// ����
		INT			m_nProvince;			/// ʡ��
		STRING		m_szCity;				/// ����			[MAX_CITY_LENGTH]
		STRING		m_szEmailInfo;			/// �ʼ�			[MAX_EMAIL_LENGTH]
		STRING		m_szLuckWord;			/// �������		[MAX_LUCKWORD_LENGTH]

		VOID		CleanUp()
		{
			m_nInfoType		= 0;
			m_unCharGUID	= 0;
			m_nAge			= 0;
			m_nSex			= 0;
			m_szJobInfo		= "";
			m_szSchoolInfo	= "";
			m_nBloodType	= 0;
			m_nYearAnimal	= 0;
			m_nConsella		= 0;
			m_nProvince		= 0;
			m_szCity		= "";
			m_szEmailInfo	= "";
			m_szLuckWord	= "";
		}
	};
	PRIVATE_INFO	m_MyPrivateInfo;
	PRIVATE_INFO	m_OtPrivateInfo;
	INT				m_nObjId;


/** ���׼�¼�����߲�����¼
 */
	
	RecordList_t						m_RecordList;

	
/** �����ķ���ʦ
 */

	/// �ķ���ʦ���ķ�ID
//	INT									m_idXinfaTeacher;
//	INT									m_nXinfaTeacherId;

	
/** �������ʦ���������
 */

	/// ���̵�����ܵ�ID
	INT									m_nStudyAbility_ID;
	/// ��Ҫ���ѵĽ�Ǯ
	UINT								m_uStudyAbility_NeedMoney;	
	/// ��Ҫ���ĵľ���ֵ
	UINT								m_uStudyAbility_NeedExp;	
	/// ������Ҫ�ﵽ�ĵ�����ѧϰ�в������ģ�
	UINT								m_uStudyAbility_SkillExp;
	/// ѧϰ����������Ҫ�ĵȼ�
	INT									m_nStudyAbility_LevelLimit;
	/// ѧϰ������ܵķ������ű���
	UINT								m_uStudyAbility_ScriptId;
	/// ѧϰ�����NpcServer ID
	UINT								m_StudyAbility_NpcId;

	SDATA_PET							m_listPet[HUMAN_PET_MAX_COUNT];

	SDATA_PET							m_TargetPet;

	struct MountAvatar
	{
		INT					nMountID;
		STRING				strAvatarName;
		CObject_PlayerNPC * pAvatar;

		MountAvatar()
		{
			nMountID		= INVALID_ID;
			pAvatar			= NULL;
			strAvatarName	= "";

		}
		VOID CleanUp()
		{
			nMountID		= INVALID_ID;
			pAvatar			= NULL;
			strAvatarName	= "";
		}
	};

	/// ����Pet UI��ʾ�ĳ������
	MountAvatar							m_listPetAvatar[HUMAN_PET_MAX_COUNT];

	/// ����TargetPet UI��ʾ�ĳ������
	MountAvatar							m_TargetPetAvatar;

	/// ����Mount UI��ʾ���������  [7/14/2010 �¾���]
	MountAvatar							m_listMountAvatar[MAX_MOUNT_SIZE];


/** ���ܵ�BUFF����
 */
	
	typedef std::map< INT, _BUFF_IMPACT_INFO* >	CBuffImpactMap;
	CBuffImpactMap						m_mapBuffImpact;
	INT									m_nBuffImpactTime;


	/// ������ 
	UINT								m_uFriendCount;		
	/// �������� 
	UINT								m_uBlackCount;
	/// ������
	UINT								m_uEnemyCount;

	_FRIEND_INFO						m_FriendInfo[ RELATION_BLACKNAME_OFFSET - RELATION_FRIEND_OFFSET ];	// 200 - 0 = 200
	_BLACKNAME_INFO						m_BlackNameInfo[ MAX_RELATION_SIZE - RELATION_BLACKNAME_OFFSET - 40 ] ;	// 270 - 200-40 = 30
	_ENEMY_INFO							m_EnemyInfo[ 40 ];

	/// �Զ����������
	STRING								m_NewGroup[5];

	//�������Ϊ���ѵ���ҵ�GUID
	CHAR								m_FriendInviteName[MAX_CHARACTER_NAME];

/** �������ɼ���ѧϰ
 */

	/// �������������ļ����б�ֻ��ѧϰ�������ɼ��ܵ�ʱ��ʹ��
	std::vector<PET_SKILL>				m_petSkillStudyList;				


/** ���﹫���
 */
	
	/// ��ǰ����
	INT									m_nPetPlacardItemCount;								
	/// ���﹫�������
	_PET_PLACARD_ITEM					m_aPetPlacardItems[MAX_PETPLACARD_LIST_ITEM_NUM];	



/** ��������
 */
	UINT								m_FingerPlayerNum;
	FingerPlayerInfo_t					m_FingerPlayerList[USER_ARRAY_SIZE];



//================
/** ����ϵͳ��ʾ
 */


	typedef std::map< INT, std::list< SystemTipEvent > >	MAP_SYSTIP;
	typedef MAP_SYSTIP::iterator MAP_SYSTIP_ITER;

	
	// �����������ͱ��������һһ��Ӧ��
	INT				m_UISysTipIndex;

	// ���ݱ���
	MAP_SYSTIP		m_mapSysTip;
	// ��Ӧ��map����
	INT				m_nMapIndex[11];
	INT				m_nMapIndexNum;


//================
/** �Ʋ�����ѡ��
 */

	// 
	INT				m_PasswordData;
	// ���浱ǰ���������
	STRING			m_InputPassword;
	// �����뱣���Ƿ��Ѿ������ˣ�������ʱ�Ƿ񵯳��������룩
	BOOL			m_InputPasswdState[5];


public:

/** �������
 */

	/** ���������� */
	void Identify_Lock(INT iBagPos);

	/** ȡ������������ */
	void Identify_UnLock();

	/// ��ǰ���������λ�� 
	INT			m_nIdentifyPosition;
	/// ʹ�þ���ı�������λ��
	int			m_iIdentifyInBagIndex;
	/// ��������ȼ�
	int			m_iIdentifyLevel;
public:

	// ��Ϸ�����е���װ
	std::map<INT , INT>	 AllSetInfo; //������װid��Ӧ��װ������
	INT  m_nSetRow  ;  //��װ�������

	INT  m_SelfActiveSetId[14];  // �������װid
	INT  m_OtherActiveSetId[14]; // �������װid
	INT  m_SelfActiveSetEquipNum[14];  // �Լ�ÿ���Ѿ��е�װ��������
	INT  m_OtherActiveSetEquipNUm[14]; // ����ÿ���Ѿ��е�װ��������
	INT  m_SelfActiveSetNum ;	// �Լ��������װ��
	INT  m_OtherActiveSetNum ;  // ���˼������װ��



 struct Euuip_Sprite_Chain
 {
	SHORT m_pai1;
	SHORT m_pai2;
	SHORT m_pai3;

	Euuip_Sprite_Chain()
		{
			m_pai1  = -1;
			m_pai2  = -1;
			m_pai3  = -1;
		}
	VOID CleanUp()
	{
		m_pai1  = -1;
		m_pai2  = -1;
		m_pai3  = -1;
	}
 };

 struct Euuip_Sprite_Soul
 {
	 SHORT m_Chain1;
	 SHORT m_Chain2;
	 SHORT m_Chain3;
	 SHORT m_Chain4;
	 SHORT m_Chain5;

	 Euuip_Sprite_Soul ()
	 {
		m_Chain1 = -1; 
		m_Chain2 = -1;
		m_Chain3 = -1;
		m_Chain4 = -1;
		m_Chain5 = -1;    //�����
	
	 }
VOID CleanUp()
	 
	 {
		 m_Chain1 = -1;
		 m_Chain2 = -1;
		 m_Chain3 = -1;
		 m_Chain4 = -1;
		 m_Chain5 = -1;


	 }

 };



	// ��Ϸ�����е����С����
	std::map<INT , Euuip_Sprite_Chain>	 AllSpritActiveChainEqual; //����С������Ϣ  ��ȵ�

	// ��Ϸ�����е����С����
	std::map<INT , Euuip_Sprite_Chain>	 AllSpritActiveChainSeries; //����С������Ϣ ���ŵ�

	// ��Ϸ�����е����С����
	std::map<INT , Euuip_Sprite_Chain>	 AllSpritActiveChainJiang; //����С������Ϣ ��

	// ��Ϸ�����е����󼤻�
	std::map<INT , Euuip_Sprite_Soul>	 AllSpritActiveInfo ;//����󼤻���Ϣ
	


	INT  m_SelfSpritActive[EM_EQUIP_CARD_GROUP_MAXNUM]; //���������С���������  ��������С�����id 
	INT  m_SelfSpritActiveAny[EM_EQUIP_CARD_GROUP_MAXNUM]; //���������С���������  ��������С�����id  ֻ������˴ �� ��
	INT  m_OterSpritActive[EM_EQUIP_CARD_GROUP_MAXNUM]; //���������С���������
	INT  m_OterSpritActiveAny[EM_EQUIP_CARD_GROUP_MAXNUM]; //���������С���������

	INT m_SelfAllSprite_Active;//���������󼤻��id��
	INT m_OtherAllSprite_Active;//���������󼤻��id��
	struct  Equip_Id
	{
		INT  nIdList[50];   //��СΪÿ����װ������ �����ѡһ��ϳ���װ

		INT  nValue;// ��Ч��С

		Equip_Id()
		{
			memset(nIdList,0,sizeof(nIdList));
			nValue = 0;
		}

	  VOID CleanUp()
		{
			memset(nIdList,0,sizeof(nIdList));
			nValue = 0;
		}
	};


	// ��Ϸ�����е���װ    ��װ�� ��Ӧ��Ӧ������װ��id
	std::map<INT , Equip_Id>	 AllSetEquipList; //������װid��Ӧ��װ������

	struct  EquipSet_Attr_Name
	{
		string  m_Name;
		EQUIP_SET_ATTR  m_stAttr[EQUIP_SET_ATTR_NUM]; //��װ����
		INT	m_nAttrActiveNum[EQUIP_SET_ATTR_NUM];
		EquipSet_Attr_Name()
		{
			CleanUp();
		}
		VOID CleanUp()
		{
			m_Name = "";
			memset(m_stAttr,-1,sizeof(m_stAttr));
			memset(m_nAttrActiveNum, 0, sizeof(m_nAttrActiveNum));
		}

	};
	// ��Ϸ�����е���װ  name
	std::map<INT , EquipSet_Attr_Name>	 AllSetEquipName; //������װid��Ӧ��װ��name

public:
	INT		GetAttriAutoPoint(INT nPos);
	VOID	SetAttriAutoPoint(INT val){m_AtriAutoPoint = val;}

protected:
	INT	m_AtriAutoPoint;
public:

	//---------------------------------------------
	//���ļ̳�
	//---------------------------------------------

	CDataPool();
	virtual ~CDataPool();

	static CDataPool*	GetMe(VOID) { return s_pMe; }

public:

	virtual VOID		Initial(VOID*);
	virtual VOID		Release(VOID);
	virtual VOID		Tick(VOID);

protected:

	static CDataPool*	s_pMe;

	KL_DECLARE_DYNAMIC( CDataPool );
};