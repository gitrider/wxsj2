/********************************************************************
	��������:	2005��11��10��
	����ʱ��:	10:58
	�ļ�����:	HumanItemLogic.h
	�ļ�·��:	d:\Prj\Server\Server\Item\HumanItemLogic.h
	������:		������
	
	�ļ����ܣ� Obj_Human ����Ʒ�����ķ�װ
	�޸ļ�¼��
	
*********************************************************************/

#ifndef _HUMAN_ITEM_LOGIC_H_
#define _HUMAN_ITEM_LOGIC_H_
#include "Type.h"

class Obj_Human;
class Item;
class ItemContainer;
struct _EXCHANGE_ITEM_LIST ;
struct ITEM_LOG_PARAM;
struct _CREATEITEM_INIT;

class HumanItemLogic
{
public:
	static	ItemContainer*	GetBagItemContain(Obj_Human* pHuman, _ITEM_GUID* pItemID, INT& nBagIndex);

	//ͨ����ƷGUID,ȡ��Human����Ʒ������
	static	ItemContainer*	GetBagItemContain(Obj_Human* pHuman, _ITEM_GUID* pItemID);

	//���Human����λ��ΪuIndex����������
	static ItemContainer*	GetBagContainer(Obj_Human*	pHuman,UINT uBagIndex);

	//���Human����װ��������
	static ItemContainer*	GetEquipContainer(Obj_Human* pHuman);

	static ItemContainer*   GetBagByExtraContainPos(Obj_Human*	pHuman,UINT uExtraIndex);
	//-----------------------------------------------------------------------------------------------------------------
	/*
	*	��������
	*/
	/*
	*	��������
	*  ��Human����ɾ��uCount������ΪuItemIndex ����Ʒ
	*/
	static	BOOL			EraseBagItem(ITEM_LOG_PARAM* pLogParam,Obj_Human*	pHuman,UINT uItemIndex,UINT uCount);

	/*
	*	��������
	*  ��Human���ϱ���pContainer, ɾ��uCount������ΪuItemIndex ����Ʒ
	*  ����ʣ��δɾ������
	*/
	static	INT				EraseBagItem(ITEM_LOG_PARAM* pLogParam,Obj_Human*	pHuman,ItemContainer* pContainer, UINT uItemIndex, INT uCount);

	//��Human����ɾ����uBagIndex λ���ϵ���Ʒ
	static	BOOL			EraseBagItem(ITEM_LOG_PARAM* pLogParam,Obj_Human*	pHuman,UINT uBagIndex);

	//����Human����uBagIndex λ���ϵ���Ʒ
	static  BOOL			DecBagItemLayCount(Obj_Human*,UINT uBagIndex,UINT uCount =1);

	//����Human����uBagIndex λ���ϵ���Ʒ,uBagIndex ����һ��Ҫ�ж���
	static  BOOL			IncBagItemLayCount(Obj_Human*,UINT uBagIndex,UINT uCount =1);

	//pItemContainerָ������
	//uItemIndex ��ָ��Ҫ�������ɿռ����Ʒ����
	//Number ��ָ��Ҫ���ɵ� uItemIndex ���͵���Ʒ����
	//NewSpace ���ڱ������� Number �� uItemIndex ���͵���Ʒ��Ҫ���±����ռ�
	static	INT				CalcBagItemSpace(Obj_Human* pHuman,ItemContainer* pItemContainer, UINT uItemIndex, INT Number,INT& NewSpace);
	
	//uItemIndex ��ָ��Ҫ�������ɿռ����Ʒ����
	//Number ��ָ��Ҫ���ɵ� uItemIndex ���͵���Ʒ����
	//NewSpace ���ڱ������� Number �� uItemIndex ���͵���Ʒ��Ҫ���±����ռ�
	static	BOOL			CalcBagItemSpace(Obj_Human* pHuman,UINT uItemIndex,INT Number,INT& NewSpace);

	//���㱳���ո��ܺ�
	static	INT				CalcBagSpace(Obj_Human* pHuman);

	//�����Ƿ��������ItemList
	static	BOOL			CanBagReceiveItemList(Obj_Human* pHuman,_ITEM_LIST& List);

	//����ItemList
	static	BOOL			ReceiveBagItemList(ITEM_LOG_PARAM* pLogParam,Obj_Human* pHuman,_ITEM_LIST& List);

	//���Human����λ��ΪuIndex��Item
	static	Item*			GetBagItem(Obj_Human*	pHuman,UINT uBagIndex);

	//���Human�����ж�ӦGUID����Ʒλ��
	static	INT				GetBagItemPosByGUID(Obj_Human* pHuman,_ITEM_GUID guid);

	//���Human�����ж�ӦItemIndex����Ʒλ��
	static	INT				GetBagItemPosByType(Obj_Human* pHuman,UINT uItemIndex);

	//���Human������Ʒ���ΪuItemIndex��ͳ�Ƹ���
	static	INT				CalcBagItemCount(Obj_Human* pHuman,UINT uItemIndex);

	//����human������Ʒ���;ö�
	static	BOOL			SetBagItemDur(Obj_Human*	pHuman,UINT uBagIndex, INT iDur);

	static	BOOL			SetBagItemCurMaxDur(Obj_Human*	pHuman,UINT uBagIndex, INT iDur);

	//����Human��������Ʒ�ļ�������
	static	BOOL			SetBagItemIdent(Obj_Human*	pHuman,UINT uBagIndex);

	//����Human��������Ʒ�Ĳ���
	static	BOOL			SetBagItemParam(Obj_Human* pHuman,UINT uBagIndex,UINT Start,INT Type,INT value);

	//����human������Ʒ������;ö�
	static	BOOL			SetBagItemMaxDur(Obj_Human*	pHuman,UINT uBagIndex, INT iDur);

	//�����Ϸ���uCount������ΪItemIndex ����Ʒ
	static	BOOL			CreateMultiItemToBag(ITEM_LOG_PARAM* pLogParam, Obj_Human*	pHuman,UINT uItemIndex,UINT uCount);

	//�����Ϸ���һ������ΪuItemIndex ����Ʒ
	static	BOOL			CreateItemToBag(ITEM_LOG_PARAM* pLogParam, Obj_Human*	pHuman,UINT uItemIndex,UINT& uBagIndex,UINT uQuality = 1);

	//�����Ϸ���һ������ΪuItemIndex,����ΪpTemplateItem����Ʒ
	static	BOOL			CreateItemToBag(ITEM_LOG_PARAM* pLogParam,Obj_Human* pHuman, _CREATEITEM_INIT* pInit, UINT& uBagIndex,UINT uQuality =1);

	//���������ڵ���Ʒ
	VOID	LockBagItem(Obj_Human*	pHuman, _ITEM_GUID* pItemID) ;

	//���������ڵ���Ʒ
	VOID	UnlockBagItem(Obj_Human*	pHuman, _ITEM_GUID* pItemID ) ;

	/*
	*	���ñ�����װ�����;ö�	
	*
	*	UINT uBagIndex	������λ��
	*
	*  INT Dur					�;öȵ���
	*/
	static	BOOL			SetBagEquipDur(Obj_Human*	pHuman,UINT uBagIndex, INT iDur);

	/*
	*	���ñ�����װ��������;ö�	
	*
	*	UINT uBagIndex	������λ��
	*
	*  INT Dur					�;öȵ���
	*/
	static	BOOL			SetBagEquipMaxDur(Obj_Human*	pHuman, UINT uBagIndex, INT iDur);

	/*
	*	���ñ�����װ���ĵ�ǰ����;ö�	
	*
	*	UINT uBagIndex	������λ��
	*
	*  INT Dur					�;öȵ���
	*/
	static	BOOL			SetCurBagEquipMaxDur(Obj_Human*	pHuman,UINT uBagIndex, INT iDur);


	static BOOL				ExchangeBagItem(Obj_Human*	pHuman1,UINT uBagIndex1, Obj_Human* pHuman2, UINT uBagIndex2);
	//-----------------------------------------------------------------------------------------------------------------
	//
	/*
	 *	װ������
	 */
	
	/*
	 *	�����Ѿ�װ������Ʒ���;ö�	
	 *
	 *	HUMAN_EQUIP EquipPoint	װ���λ��
	 *
	 *  INT Dur					�;öȵ���
	 */
	static	BOOL			SetEquipDur(Obj_Human*	pHuman,HUMAN_EQUIP EquipPoint,INT iDur);

	/*
	*	�����Ѿ�װ������Ʒ������;ö�	
	*
	*	HUMAN_EQUIP EquipPoint	װ���λ��
	*
	*  INT Dur					�;öȵ���
	*/
	static	BOOL			SetEquipMaxDur(Obj_Human*	pHuman,HUMAN_EQUIP EquipPoint,INT iDur);

	/*
	*	�����Ѿ�װ������Ʒ�ĵ�ǰ����;ö�	
	*
	*	HUMAN_EQUIP EquipPoint	װ���λ��
	*
	*  INT Dur					�;öȵ���
	*/
	static	BOOL			SetCurEquipMaxDur(Obj_Human*	pHuman,HUMAN_EQUIP EquipPoint,INT iDur);

	/*	ɾ����ɫ�Ѿ�װ���ϵ���Ʒ
	 *
	 *	EquipPoint װ������
	 */ 
	static	BOOL			EraseEquip(Obj_Human* pHuman,HUMAN_EQUIP	EquipPoint);

	/*
	 *	���װ���ϵ���Ʒ
	 *	EquipPoint װ������
     */
	static	Item*			GetEquip(Obj_Human* pHuman,HUMAN_EQUIP EquipPoint);
	
	
	//���Human����Equip���ΪuItemIndex��ͳ�Ƹ���
	static	INT				CalcEquipItemCount(Obj_Human* pHuman,UINT uItemIndex);


	//����Human����EquipPointװ�����װ���˺���
	static	BOOL			SetEquipDamagePoint(Obj_Human* pHuman,HUMAN_EQUIP EquipPoint, UINT nDamagePoint);
	//-----------------------------------------------------------------------------------------------------------------
	//
	/*
	 *	���в���
	 */
	static	Item*			GetBankItem(Obj_Human* pHuman,UINT BankIndex);
	
	/*
	*	����Ʒ������
	*/
	static	BOOL			RecieveItemToBag(Obj_Human* pHuman, ItemContainer* pContainer, UINT uIndex );

	/*
	*	��ָ����Ʒ������
	*/
	static	BOOL			RecieveItemToBag(ITEM_LOG_PARAM* pLogParam, Obj_Human* pHuman, _ITEM* pItem);

	/*
	*	�����в�����Ʒ�ܲ��ܱ�����
	*/
	static	BOOL			CanReceiveExchangeItemList(Obj_Human* pHuman,_EXCHANGE_ITEM_LIST& List);

	//-----------------------------------------------------------------------------------------------------------------
	//
	/*
	*	���ﲿ��  [7/8/2010 �¾���]
	*/
	static	ItemContainer*	GetMountContainer(Obj_Human*	pHuman); 
	static	Item*	        GetMount(Obj_Human* pHuman, INT nIndex);

	//-----------------------------------------------------------------------------------------------------------------
	//
	/*
	*	������������
	*/
	//��Human����ɾ������������uBagIndex λ���ϵ���Ʒ
	static	BOOL			EraseExtraContainerItem(ITEM_LOG_PARAM* pLogParam,Obj_Human*	pHuman,UINT uBagIndex);

	//���㱳���ո��ܺ�
	static	UINT			CalcExtraContainerSpace(Obj_Human* pHuman,UINT uItemIndex);

	//���Human����λ��ΪuIndex��Item
	static	Item*			GetExtraContainerItem(Obj_Human*	pHuman,UINT uBagIndex);

	static	Item*			GetExtraBagFromBagIndex(Obj_Human* pHuman, UINT uBagIndex);
	//���Human�����ж�ӦGUID����Ʒλ��
	static	UINT			GetExtraContainerItemPosByGUID(Obj_Human* pHuman,_ITEM_GUID guid);

	//-----------------------------------------------------------------------------------------------------------------
	//
	/*
	*	����֮��
	*/
	//������֮���ƶ���Ʒ�Ĳ���
	static	BOOL			MoveItem(ITEM_LOG_PARAM* pLogParam,Obj_Human*	pHuman,ItemContainer*	pContainer,UINT uIndex, INT& uBagPos);
	
	//������֮���ƶ���Ʒ�Ĳ���
	static	BOOL			MoveItem(ITEM_LOG_PARAM* pLogParam,Obj_Human*	pHuman,ItemContainer*	pContainer,UINT uIndex, Obj_Human* pDestMan, INT& uBagPos);

	//������֮���з���Ʒ

	static BOOL				SplitItem(Obj_Human* pHuman,
		ItemContainer* pSourContainer, 
		UCHAR uSourIndex,  
		UCHAR uCount,  
		ItemContainer* pDestContainer,
		INT nDestIndex /* =INVALID_INDEX  */,
        INT opType = 0);

	//������֮��ϲ���Ʒ
	static	BOOL			SpliceItem(Obj_Human* pHuman,
		ItemContainer* pSourContainer, 
		UCHAR uSourIndex,
		ItemContainer* pDestContainer,
		UCHAR uDestIndex);
};


#endif