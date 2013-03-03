

#include "stdafx.h"
#include "ItemOperator.h"
#include "ItemManager.h"
#include "HumanDB.h"
#include "ItemRuler.h"
#include "Log.h"


ItemOperator	g_ItemOperator;


ItemOperator::ItemOperator( )
{
__ENTER_FUNCTION


__LEAVE_FUNCTION
}

ItemOperator::~ItemOperator( )
{
__ENTER_FUNCTION

	CleanUp( ) ;

__LEAVE_FUNCTION
}

BOOL ItemOperator::Init( _ITEMOPERATOR_INIT* pInit )
{
__ENTER_FUNCTION


__LEAVE_FUNCTION

	return TRUE ;
}

VOID ItemOperator::CleanUp( )
{
__ENTER_FUNCTION


__LEAVE_FUNCTION
}

INT ItemOperator::CreateItem( ITEM_LOG_PARAM*	pLogParam,
							  Item* pTemplateItem ,
							  ItemContainer* pDestContainer,
							  INT nDestIndex )
{
__ENTER_FUNCTION
	if (!pDestContainer->IsValid())
	{
		return FALSE ;
	}
	if(		pTemplateItem==NULL 
		||  pDestContainer==NULL 
		||  (nDestIndex<INVALID_INDEX 
		||  nDestIndex>=pDestContainer->GetContainerSize())
	  )	 
	{
		return ITEMOE_UNKNOW ;
	}
	if (!pDestContainer->IsCanUse())
	{
		return ITEMOE_DESTOPUSE_FAIL;
	}
	Item* pSourItem = pTemplateItem ;
	Item* pDestItem = NULL ;

	if( pSourItem->IsEmpty() )
	{
		return ITEMOE_SOUROPERATOR_EMPTY ;
	}
	else if( pSourItem->IsLock() )
	{
		return ITEMOE_SOUROPERATOR_LOCK ;
	}

	INT nFinalDestIndex = nDestIndex ;
	if( nFinalDestIndex == INVALID_INDEX )
	{//����Ҹ��ط�������Ʒ
		
		if(pSourItem->m_Type != IDT_PET)
		{
			INT nSourLayCount = pSourItem->GetLayedNum();
			INT nRepIndex = pDestContainer->GetIndexByType( pSourItem->GetItemTableIndex(),pSourItem->GetLayedNum() ) ;
			if( nRepIndex!=INVALID_INDEX )
			{//�Ѿ���ͬ����Ʒ��
				pDestItem = pDestContainer->GetItem( nRepIndex ) ;
				if( !pDestItem->IsLock() && pDestItem->IsCanLay() )
				{//��Ʒ�ܹ��ص�����
					INT nDestLayCount = pDestItem->GetLayedNum() ;
					INT nDestMaxLayCount = pDestItem->GetMaxLayedNum() ;

					if( nSourLayCount+nDestLayCount<=nDestMaxLayCount )
					{
						pLogParam->ItemType		=	pDestItem->GetItemTableIndex();
						pLogParam->ContainerPos		=	pDestContainer->ConIndex2BagIndex(nRepIndex);
						pLogParam->SecOpType	=	ITEM_CREATE_TILED;
						pLogParam->ItemGuid		=	pDestContainer->GetItem(nRepIndex)->GetGUID();
						pLogParam->Count		=	nSourLayCount+nDestLayCount;
						SetItemLayCount( pDestContainer, nRepIndex, nSourLayCount+nDestLayCount ) ;
						return nRepIndex ;
					}
				}
			}
		}
		

		nFinalDestIndex = pDestContainer->GetEmptyItemIndex() ;
		if( nFinalDestIndex == INVALID_INDEX )
		{
			return ITEMOE_DESTOPERATOR_FULL ;
		}
	}
	else 
	{
		pDestItem = pDestContainer->GetItem( nDestIndex ) ;
		Assert( pDestItem ) ;

		if( !pDestItem->IsEmpty() )
		{//Ŀ��λ������Ʒ
			return ITEMOE_DESTOPERATOR_HASITEM ;
		}
	}
	
	switch(pSourItem->m_Type )
	{
	case IDT_ITEM:
		{
			//��create������item��Ҫ�µ�guid
			_ITEM_GUID NewGUID;
			if(!GenItemGUID(NewGUID))
			{
				return ITEMOE_CREATEGUID_FAIL;
			}

			pDestContainer->SetItem( nFinalDestIndex, pSourItem ) ;
			SetItemGUID( pDestContainer, nFinalDestIndex, &NewGUID ) ;
			pDestItem = pDestContainer->GetItem(nFinalDestIndex);
			pLogParam->ItemType		=	pDestItem->GetItemTableIndex();
			pLogParam->ContainerPos		=	pDestContainer->ConIndex2BagIndex(nFinalDestIndex);
			pLogParam->ItemGuid		=	pDestItem->GetGUID();
			pLogParam->Count		=	pDestItem->GetLayedNum();
			
		}
		break;
	case IDT_PET:
		{
			PET_GUID_t	NewGUID;
			if(!GenItemGUID(NewGUID))
			{
				return ITEMOE_CREATEGUID_FAIL;
			}
			pDestContainer->SetItem( nFinalDestIndex, pSourItem ) ;
			SetItemGUID( pDestContainer, nFinalDestIndex, &NewGUID ) ;

		}
		break;
	default:
		{
			return ITEMOE_CREATEGUID_FAIL;
			Assert(FALSE);
		}
	}

	return ITEMOE_SUCCESS;


__LEAVE_FUNCTION

	return ITEMOE_UNKNOW ;
}


BOOL	ItemOperator::CreateItem(ITEM_LOG_PARAM* pLogParam,
								UINT uItemIndex,
								UINT uQuality,
								ItemContainer* pDestContainer,
								BOOL&	bNewEmptyPos,
								UINT&	uConIndex,
								INT nDestIndex 
								)
{

	__ENTER_FUNCTION

	Assert(pDestContainer);
	Assert(uItemIndex!=0);
	Assert(uItemIndex!=INVALID_ID);
	Assert(pLogParam);
	if (!pDestContainer->IsValid())
	{
		return FALSE ;
	}
	if(uItemIndex==0||uItemIndex==INVALID_ID)
		return FALSE;
	
	if (!pDestContainer->IsCanUse())
	{
		return FALSE;
	}

	uConIndex		=	INVALID_INDEX;
	bNewEmptyPos	=	FALSE;
	
	if (nDestIndex != INVALID_INDEX)
	{
		Assert(nDestIndex<pDestContainer->GetContainerSize());
		Item* pItem = pDestContainer->GetItem(nDestIndex);
		if (pItem->IsEmpty())
		{
			_ITEM	createItem;

			g_pItemManager->CreateItem(pLogParam,uItemIndex,createItem,uQuality);

			if(createItem.IsNullType())
			{
				return FALSE;
			}

			pLogParam->ItemType = uItemIndex;
			pLogParam->ContainerPos   = pDestContainer->ConIndex2BagIndex(nDestIndex);
			pLogParam->ItemGuid	= createItem.m_ItemGUID;

			Item*	pDestItem =	pDestContainer->GetItem(nDestIndex);
			if(pDestItem->IsEmpty())
			{
				pDestContainer->SetItemValue(nDestIndex,&createItem);
				return TRUE;
			}
			else
			{
				Assert(FALSE);//���ܳ��ֵ����
			}
		}
		if (pItem->GetItemTableIndex() != uItemIndex)
		{
			return FALSE;
		}

		//����ɵ�����Ʒ
		if(ItemRulerCheck::CheckTypeRuler(IRL_TILE,uItemIndex))
		{
			if(pItem->IsLock())
			{
				return FALSE;
			}
			//ֻ��һ���������������
			if (pItem->GetLayedNum()<pItem->GetMaxLayedNum())
			{
				uConIndex				=	nDestIndex;
				bNewEmptyPos			=	FALSE;
				pLogParam->ItemType		=	uItemIndex;
				pLogParam->ContainerPos		=	pDestContainer->ConIndex2BagIndex(nDestIndex);
				pLogParam->SecOpType	=	ITEM_CREATE_TILED;
				pLogParam->ItemGuid		=	pItem->GetGUID();
				pLogParam->Count		=	pItem->GetLayedNum()+1;
				return IncItemLayCount(pDestContainer, nDestIndex);
			}
			return FALSE;
		}
	}

	//����ɵ�����Ʒ
	if(ItemRulerCheck::CheckTypeRuler(IRL_TILE,uItemIndex))
	{
		//ֻ��һ���������������
		INT iNoFullBagIndex =	pDestContainer->GetNoFullItemIndex(uItemIndex);

		if(iNoFullBagIndex != INVALID_INDEX)
		{
			uConIndex				=	iNoFullBagIndex;
			bNewEmptyPos			=	FALSE;
			pLogParam->ItemType		=	uItemIndex;
			pLogParam->ContainerPos		=	pDestContainer->ConIndex2BagIndex(iNoFullBagIndex);
			pLogParam->SecOpType	=	ITEM_CREATE_TILED;
			pLogParam->ItemGuid		=	pDestContainer->GetItem(iNoFullBagIndex)->GetGUID();
			pLogParam->Count		=	pDestContainer->GetItem(iNoFullBagIndex)->GetLayedNum()+1;
			return IncItemLayCount(pDestContainer,iNoFullBagIndex);
		}
	}

	//����һ��Ҫnew��Ʒ
	INT iEmptyBagIndex	= pDestContainer->GetEmptyItemIndex();

	if(iEmptyBagIndex != INVALID_INDEX)
	{

		_ITEM	createItem;
		
		g_pItemManager->CreateItem(pLogParam,uItemIndex,createItem,uQuality);
		
		if(createItem.IsNullType())
		{
			return FALSE;
		}
		
		pLogParam->ItemType = uItemIndex;
		pLogParam->ContainerPos   = pDestContainer->ConIndex2BagIndex(iEmptyBagIndex);
		pLogParam->ItemGuid	= createItem.m_ItemGUID;
		
		Item*	pDestItem =	pDestContainer->GetItem(iEmptyBagIndex);
		if(pDestItem->IsEmpty())
		{
			pDestContainer->SetItemValue(iEmptyBagIndex,&createItem);
			bNewEmptyPos	= TRUE;	
			uConIndex		= iEmptyBagIndex;
			return TRUE;
		}
		else
		{
			Assert(FALSE);//���ܳ��ֵ����
		}
	}
	return FALSE;

	__LEAVE_FUNCTION

	return FALSE;
}

//---------------------------------------------------------------------------------
BOOL	ItemOperator::CreateItem(ITEM_LOG_PARAM* pLogParam,
								 UINT uQuality,
								 ItemContainer* pDestContainer,
								 UINT&	uConIndex,
								  _CREATEITEM_INIT* pInit)
{

	__ENTER_FUNCTION

	Assert(pDestContainer);
	Assert(pLogParam);
	Assert(pInit);

	if (!pDestContainer->IsValid())
	{
		return FALSE ;
	}
	if(pInit->m_nItemIndex==0||pInit->m_nItemIndex==INVALID_ID)
		return FALSE;

	if (!pDestContainer->IsCanUse())
	{
		return FALSE;
	}

	uConIndex		=	INVALID_INDEX;

	//����һ��Ҫnew��Ʒ
	INT iEmptyBagIndex	= pDestContainer->GetEmptyItemIndex();

	if(iEmptyBagIndex != INVALID_INDEX)
	{
		_ITEM	createItem;
		g_pItemManager->CreateItem(pLogParam,createItem,pInit);
		if(createItem.IsNullType())
		{
			return FALSE;
		}

		pLogParam->ItemType = pInit->m_nItemIndex;
		pLogParam->ContainerPos   = pDestContainer->ConIndex2BagIndex(iEmptyBagIndex);
		pLogParam->ItemGuid	= createItem.m_ItemGUID;

		Item*	pDestItem =	pDestContainer->GetItem(iEmptyBagIndex);
		if(pDestItem->IsEmpty())
		{
			pDestContainer->SetItemValue(iEmptyBagIndex,&createItem);
			uConIndex		= iEmptyBagIndex;
			return TRUE;
		}
		else
		{
			Assert(FALSE);//���ܳ��ֵ����
		}
	}
	return FALSE;

	__LEAVE_FUNCTION

		return FALSE;
}




INT ItemOperator::CopyItem( ItemContainer* pSourContainer,
							INT			   nSourIndex ,
							ItemContainer* pDestContainer,
							INT nDestIndex )
{
__ENTER_FUNCTION
	Assert(pSourContainer);
	Assert(pDestContainer);
	if (!pSourContainer->IsValid())
	{
		return ITEMOE_UNKNOW ;
	}
	if (!pDestContainer->IsValid())
	{
		return ITEMOE_UNKNOW ;
	}
	if( pSourContainer==NULL 
		|| (nSourIndex<0 || nSourIndex>=pSourContainer->GetContainerSize())
		|| pDestContainer==NULL 
		|| (nDestIndex<INVALID_INDEX || nDestIndex>=pDestContainer->GetContainerSize())
		)
	{
		return ITEMOE_UNKNOW ;
	}

	Item* pSourItem = NULL ;
	Item* pDestItem = NULL ;

	//��֤
	pSourItem = pSourContainer->GetItem( nSourIndex ) ;
	if(pSourItem==NULL)
	{
		Assert(FALSE) ;
		return ITEMOE_UNKNOW ;
	}

	if( pSourItem->IsEmpty() )
	{
		return ITEMOE_SOUROPERATOR_EMPTY ;
	}
	else if( pSourItem->IsLock() )
	{
		return ITEMOE_SOUROPERATOR_LOCK ;
	}

	if (!pDestContainer->IsCanUse())
	{
		return ITEMOE_DESTOPUSE_FAIL;
	}

	INT nFinalDestIndex = nDestIndex ;
	
	if( nFinalDestIndex == INVALID_INDEX )
	{//�Զ�Ѱ�ҿո񣬲��Զ���䵽�ɵ��ӵĸ������ˣ�
		nFinalDestIndex = pDestContainer->GetEmptyItemIndex() ;
		if( nFinalDestIndex == INVALID_INDEX )
		{
			return ITEMOE_DESTOPERATOR_FULL ;
		}
	}
	else
	{//ָ������
		pDestItem = pDestContainer->GetItem( nDestIndex ) ;
		if( pDestItem==NULL )
		{
			Assert(FALSE) ;
			return ITEMOE_UNKNOW ;
		}
		if( !pDestItem->IsEmpty() )
		{//Ŀ��λ������Ʒ
			return ITEMOE_DESTOPERATOR_HASITEM ;
		}
		if( pDestItem->IsLock() )
		{//Ŀ��λ���ѱ���
			return ITEMOE_DESTOPERATOR_LOCK;
		}
	}

	//������ȥ���Զ�����
	pDestContainer->SetItem( nFinalDestIndex, pSourItem ) ;

	return nFinalDestIndex ;

__LEAVE_FUNCTION

	return ITEMOE_UNKNOW ;
}



INT	ItemOperator::CopyItem(ItemBoxContaner* pSourceContainer,
						   INT				nSourceIndex,
						   BOOL&		    bNewPos,
						   ItemContainer*   pDestContainer,
						   INT nDestIndex/* =INVALID_INDEX */)
{
	__ENTER_FUNCTION

		if (!pDestContainer->IsValid())
		{
			return ITEMOE_UNKNOW ;
		}

		if( pSourceContainer==NULL 
			|| (nSourceIndex<0 
			|| nSourceIndex>=MAX_BOXITEM_NUMBER)
			|| pDestContainer==NULL 
			|| (nDestIndex<INVALID_INDEX 
			|| nDestIndex>=pDestContainer->GetContainerSize())
		  )
		{return ITEMOE_UNKNOW ;	}

	   _ITEM* pSourItem = &(pSourceContainer->m_Item[nSourceIndex]);

		if( pSourItem==NULL )
		{
			Assert(FALSE) ;
			return ITEMOE_UNKNOW ;
		}
		
		Item* pDestItem = NULL ;

		INT nFinalDestIndex = nDestIndex ;
		if( nFinalDestIndex == INVALID_INDEX )
		{
			INT nRepIndex = pDestContainer->GetIndexByType( pSourItem->m_ItemIndex, pSourItem->GetItemCount() ) ;
			
			if( nRepIndex!=INVALID_INDEX ) 
			{//ͬ����Ʒ����
				
				pDestItem = pDestContainer->GetItem( nRepIndex ) ;
				Assert(pDestItem);

				if(!pDestItem)
				{
					return ITEMOE_UNKNOW;
				}
				if( !pDestItem->IsEmpty() && !pDestItem->IsLock() && pDestItem->IsCanLay() )
				{//��Ʒ�ܹ��ص�����
					INT nDestLayCount = pDestItem->GetLayedNum() ;
					INT nDestMaxLayCount = pDestItem->GetMaxLayedNum() ;
					INT nSourLayCount = pSourItem->GetItemCount();
					if( nSourLayCount+nDestLayCount<nDestMaxLayCount )
					{
						SetItemLayCount( pDestContainer, nRepIndex, nSourLayCount+nDestLayCount ) ;
						return nRepIndex ;
					}
				}
			}
			nFinalDestIndex = pDestContainer->GetEmptyItemIndex() ;
			if( nFinalDestIndex == INVALID_INDEX )
			{
				return ITEMOE_DESTOPERATOR_FULL ;
			}
			
			pDestItem	=	pDestContainer->GetItem(nFinalDestIndex);
			
		}
		else 
		{
			pDestItem = pDestContainer->GetItem( nDestIndex ) ;
			if( pDestItem==NULL )
			{
				Assert(FALSE) ;
				return ITEMOE_UNKNOW ;
			}

			if( !pDestItem->IsEmpty() )
			{//Ŀ��λ������Ʒ
				return ITEMOE_DESTOPERATOR_HASITEM ;
			}
			
		}
		
		pDestItem->SetItemValue(pSourItem);
		bNewPos = TRUE;

		return	nFinalDestIndex;

	__LEAVE_FUNCTION

		return ITEMOE_UNKNOW ;

}

INT ItemOperator::MoveItem(ItemContainer* pDestContainer, _ITEM* pSourItem, INT nDestIndex)
{
	__ENTER_FUNCTION

		if (!pDestContainer->IsValid())
		{
			return ITEMOE_UNKNOW ;
		}
		if  ((nDestIndex<INVALID_INDEX) || (nDestIndex>=pDestContainer->GetContainerSize()))
		{
			return ITEMOE_UNKNOW ;
		}
		if( pSourItem==NULL )
		{
			Assert(FALSE) ;
			return ITEMOE_UNKNOW ;
		}
		else if( pSourItem->IsNullType() )
		{
			return ITEMOE_SOUROPERATOR_EMPTY ;
		}

		if (!pDestContainer->IsCanUse())
		{
			return ITEMOE_DESTOPUSE_FAIL;
		}
		/*else if( pSourItem->IsLock() )
		{
			return ITEMOE_SOUROPERATOR_LOCK ;
		}*/

		Item* pDestItem = NULL ;
		INT nFinalDestIndex = nDestIndex ;
		if( nFinalDestIndex == INVALID_INDEX )
		{//����Ҹ��ط�������Ʒ
				INT nSourLayCount = pSourItem->GetItemCount();
				INT nRepIndex = pDestContainer->GetIndexByType( pSourItem->m_ItemIndex,pSourItem->GetItemCount() ) ;
				if( nRepIndex!=INVALID_INDEX )
				{//�Ѿ���ͬ����Ʒ��
					pDestItem = pDestContainer->GetItem( nRepIndex );
					if( !pDestItem->IsLock() && pDestItem->IsCanLay() )
					{//��Ʒ�ܹ��ص�����
						INT nDestLayCount = pDestItem->GetLayedNum() ;
						INT nDestMaxLayCount = pDestItem->GetMaxLayedNum() ;
						INT nSourLayCount = pSourItem->GetItemCount() ;
						if( nSourLayCount+nDestLayCount<=nDestMaxLayCount )
						{
							SetItemLayCount( pDestContainer, nRepIndex, nSourLayCount+nDestLayCount ) ;
							return nRepIndex ;
						}
					}
				}

			nFinalDestIndex = pDestContainer->GetEmptyItemIndex() ;
			if( nFinalDestIndex == INVALID_INDEX )
			{
				return ITEMOE_DESTOPERATOR_FULL ;
			}
		}
		else 
		{
			pDestItem = pDestContainer->GetItem( nDestIndex ) ;
			if( pDestItem==NULL )
			{
				Assert(FALSE) ;
				return ITEMOE_UNKNOW ;
			}

			if( !pDestItem->IsEmpty() )
			{//Ŀ��λ������Ʒ
				return ITEMOE_DESTOPERATOR_HASITEM ;
			}
			Assert( !pDestItem->IsLock() ) ;
		}

		pDestContainer->SetItemValue( nFinalDestIndex, pSourItem ) ;
		return nFinalDestIndex;

		__LEAVE_FUNCTION

			return ITEMOE_UNKNOW ;
}

INT ItemOperator::MoveItem( ItemContainer* pSourContainer,
							INT nSourIndex ,
							ItemContainer* pDestContainer,
							INT nDestIndex )
{
__ENTER_FUNCTION

	if  (	pSourContainer==NULL 
		|| (nSourIndex<0)
		||  pDestContainer==NULL 
		|| (nDestIndex<INVALID_INDEX)
		)
	{
		return ITEMOE_UNKNOW ;
	}
	if (!pSourContainer->IsValid())
	{
		return ITEMOE_UNKNOW ;
	}
	if (!pDestContainer->IsValid())
	{
		return ITEMOE_UNKNOW ;
	}
	if (nDestIndex>=pDestContainer->GetContainerSize())
	{
		return ITEMOE_UNKNOW ; 
	}
	if (nSourIndex>=pSourContainer->GetContainerSize())
	{
	}
	if (!pDestContainer->IsCanUse())
	{
		return ITEMOE_DESTOPUSE_FAIL;
	}

	Item* pSourItem = pSourContainer->GetItem(nSourIndex) ;
	Item* pDestItem = NULL ;

	if( pSourItem==NULL )
	{
		Assert(FALSE) ;
		return ITEMOE_UNKNOW ;
	}
	else if( pSourItem->IsEmpty() )
	{
		return ITEMOE_SOUROPERATOR_EMPTY ;
	}
	else if( pSourItem->IsLock() )
	{
		return ITEMOE_SOUROPERATOR_LOCK ;
	}

	INT nFinalDestIndex = nDestIndex ;
	if( nFinalDestIndex == INVALID_INDEX )
	{//����Ҹ��ط�������Ʒ
		if(pSourItem->m_Type != IDT_PET)
		{
			INT nSourLayCount = pSourItem->GetLayedNum();
			INT nRepIndex = pDestContainer->GetIndexByType( pSourItem->GetItemTableIndex(),pSourItem->GetLayedNum() ) ;
			if( nRepIndex!=INVALID_INDEX )
			{//�Ѿ���ͬ����Ʒ��
				pDestItem = pDestContainer->GetItem( nRepIndex );
				if( !pDestItem->IsLock() && pDestItem->IsCanLay() )
				{//��Ʒ�ܹ��ص�����
					INT nDestLayCount = pDestItem->GetLayedNum() ;
					INT nDestMaxLayCount = pDestItem->GetMaxLayedNum() ;
					INT nSourLayCount = pSourItem->GetLayedNum() ;
					if( nSourLayCount+nDestLayCount<=nDestMaxLayCount )
					{
						SetItemLayCount( pDestContainer, nRepIndex, nSourLayCount+nDestLayCount ) ;
						pSourContainer->EraseItem(nSourIndex);//���ƣ�����
						return nRepIndex ;
					}
				}
			}
		}

		nFinalDestIndex = pDestContainer->GetEmptyItemIndex() ;
		if( nFinalDestIndex == INVALID_INDEX )
		{
			return ITEMOE_DESTOPERATOR_FULL ;
		}
	}
	else 
	{
		pDestItem = pDestContainer->GetItem( nDestIndex ) ;
		if( pDestItem==NULL )
		{
			Assert(FALSE) ;
			return ITEMOE_UNKNOW ;
		}

		if( !pDestItem->IsEmpty() )
		{//Ŀ��λ������Ʒ
			return ITEMOE_DESTOPERATOR_HASITEM ;
		}
		Assert( !pDestItem->IsLock() ) ;
	}

	pDestContainer->SetItem( nFinalDestIndex, pSourItem ) ;
	pSourContainer->EraseItem(nSourIndex);
	
	return nFinalDestIndex;

__LEAVE_FUNCTION

	return ITEMOE_UNKNOW ;
}


//INT	ItemOperator::MoveExtraBagItem( ItemContainer* pSourContainer,
//									 INT nSourIndex ,
//									 INT nDestIndex )
//{
//	__ENTER_FUNCTION
//
//	if  (	pSourContainer==NULL 
//		|| (nSourIndex<0 || nSourIndex>=pSourContainer->GetContainerSize())
//		|| (nDestIndex<INVALID_INDEX || nDestIndex>=pSourContainer->GetContainerSize())
//		)
//	{
//		return ITEMOE_UNKNOW ;
//	}
//	Item* pSourItem = pSourContainer->GetItem(nSourIndex) ;
//	Item* pDestItem = NULL ;
//
//	if( pSourItem==NULL )
//	{
//		return ITEMOE_UNKNOW ;
//	}
//	else if( pSourItem->IsEmpty() )
//	{
//		return ITEMOE_SOUROPERATOR_EMPTY ;
//	}
//	else if( pSourItem->IsLock() )
//	{
//		return ITEMOE_SOUROPERATOR_LOCK ;
//	}
//	/*if (nDestIndex<EXTRA_CONTAINER_OFFSET)
//	{
//		return ITEMOE_UNKNOW;
//	}*/
//	INT nFinalDestIndex = nDestIndex ;
//	if( nFinalDestIndex == INVALID_INDEX )
//	{	//����Ҹ��ط�������Ʒ
//		nFinalDestIndex = pSourContainer->GetEmptyItemIndex() ;
//	}
//	else 
//	{
//		if (!pSourContainer->GetItem(nDestIndex))
//		{
//			return ITEMOE_DESTOPERATOR_FAIL ;
//		}
//
//		pDestItem = pSourContainer->GetItem( nDestIndex ) ;
//		if( pDestItem==NULL )
//		{
//			Assert(FALSE) ;
//			return ITEMOE_UNKNOW ;
//		}
//
//		if( !pDestItem->IsEmpty() )
//		{//Ŀ��λ������Ʒ
//			return ITEMOE_DESTOPERATOR_HASITEM ;
//		}
//		Assert( !pDestItem->IsLock() ) ;
//	}
//	
//	BOOL ret = pSourContainer->SetItem( nFinalDestIndex, pSourItem ) ;
//	Assert(ret);
//	pSourContainer->EraseItem(nSourIndex);
//	Assert(ret);
//
//	return nFinalDestIndex;
//
//	__LEAVE_FUNCTION
//
//		return ITEMOE_UNKNOW ;
//}


INT	ItemOperator::MoveItem( ItemContainer* pSourContainer,
							 INT			 nSourIndex ,
							 BOOL&			 bNewPos,
							 ItemContainer* pDestContainer,
							 INT			 nDestIndex )
{
	__ENTER_FUNCTION

		if  (	pSourContainer==NULL 
			|| (nSourIndex<0 || nSourIndex>=pSourContainer->GetContainerSize())
			||  pDestContainer==NULL 
			|| (nDestIndex<INVALID_INDEX || nDestIndex>=pDestContainer->GetContainerSize())
			)
		{
			return ITEMOE_UNKNOW ;
		}
		if (!pDestContainer->IsCanUse())
		{
			return ITEMOE_DESTOPUSE_FAIL;
		}
		Item* pSourItem = pSourContainer->GetItem(nSourIndex) ;
		Item* pDestItem = NULL ;
		bNewPos = FALSE;

		if( pSourItem==NULL )
		{
			Assert(FALSE) ;
			return ITEMOE_UNKNOW ;
		}
		else if( pSourItem->IsEmpty() )
		{
			return ITEMOE_SOUROPERATOR_EMPTY ;
		}
		else if( pSourItem->IsLock() )
		{
			return ITEMOE_SOUROPERATOR_LOCK ;
		}


		INT nFinalDestIndex = nDestIndex ;
		if( nFinalDestIndex == INVALID_INDEX )
		{//����Ҹ��ط�������Ʒ
			INT nRepIndex = pDestContainer->GetIndexByType( pSourItem->GetItemTableIndex(),pSourItem->GetLayedNum() ) ;
			if( nRepIndex!=INVALID_INDEX )
			{//�Ѿ���ͬ����Ʒ��
				pDestItem = pDestContainer->GetItem( nRepIndex );
				if( !pDestItem->IsLock() && pDestItem->IsCanLay() )
				{//��Ʒ�ܹ��ص�����
					INT nDestLayCount = pDestItem->GetLayedNum() ;
					INT nDestMaxLayCount = pDestItem->GetMaxLayedNum() ;
					INT nSourLayCount = pSourItem->GetLayedNum() ;
					if( nSourLayCount+nDestLayCount<=nDestMaxLayCount )
					{
						SetItemLayCount( pDestContainer, nRepIndex, nSourLayCount+nDestLayCount ) ;
						pSourContainer->EraseItem(nSourIndex);//���ƣ�����
						return nRepIndex ;
					}
				}
			}
			nFinalDestIndex = pDestContainer->GetEmptyItemIndex() ;
			if( nFinalDestIndex == INVALID_INDEX )
			{
				return ITEMOE_DESTOPERATOR_FULL ;
			}
			bNewPos = TRUE;
		}
		else 
		{
			pDestItem = pDestContainer->GetItem( nDestIndex ) ;
			if( pDestItem==NULL )
			{
				Assert(FALSE) ;
				return ITEMOE_UNKNOW ;
			}
			else if( !pDestItem->IsEmpty() )
			{//Ŀ��λ������Ʒ
				return ITEMOE_DESTOPERATOR_HASITEM ;
			}
			Assert( !pDestItem->IsLock() ) ;
		}
		if (nFinalDestIndex>=0)
		{
			pDestContainer->SetItem( nFinalDestIndex, pSourItem ) ;
			pSourContainer->EraseItem( nSourIndex );
		}
		
		return nFinalDestIndex;

		__LEAVE_FUNCTION

		return ITEMOE_UNKNOW ;
}


INT ItemOperator::MoveItem( ItemContainer* pSourContainer,
						   INT nSourIndex,
						   INT nDestIndex)
{
	__ENTER_FUNCTION

		if  (	pSourContainer==NULL 
			|| (nSourIndex<0 || nSourIndex>=pSourContainer->GetContainerSize())
			|| (nDestIndex<INVALID_INDEX || nDestIndex>=pSourContainer->GetContainerSize()))
		{
			return ITEMOE_UNKNOW ;
		}
		
		if (!pSourContainer->IsCanUse())
		{
			return ITEMOE_DESTOPUSE_FAIL;
		}
		Item* pSourItem = pSourContainer->GetItem(nSourIndex) ;
		Item* pDestItem = NULL ;

		if( pSourItem==NULL )
		{
			Assert(FALSE) ;
			return ITEMOE_UNKNOW ;
		}

		INT nFinalDestIndex = nDestIndex ;
	
		pDestItem = pSourContainer->GetItem( nDestIndex ) ;
		if( pDestItem==NULL )
		{
			Assert(FALSE) ;
			return ITEMOE_UNKNOW ;
		}

		Assert( !pDestItem->IsLock() ) ;
		
		if( pSourItem->IsEmpty() )
		{
			return ITEMOE_SOUROPERATOR_EMPTY ;
		}
		else if( pSourItem->IsLock() )
		{
			return ITEMOE_SOUROPERATOR_LOCK ;
		}

		if( !pDestItem->IsEmpty() )
		{//Ŀ��λ������Ʒ
			
			Assert( pSourItem->GetItemTableIndex() == pDestItem->GetItemTableIndex() );
			INT nDestLayCount = pDestItem->GetLayedNum() ;
			INT nDestMaxLayCount = pDestItem->GetMaxLayedNum() ;
			INT nSourLayCount = pSourItem->GetLayedNum() ;
			if( nSourLayCount+nDestLayCount <= nDestMaxLayCount )
			{
				SetItemLayCount( pSourContainer, nFinalDestIndex, nSourLayCount+nDestLayCount ) ;
				BOOL bRet = pSourContainer->EraseItem(nSourIndex);
				Assert(bRet);
				return nFinalDestIndex ;
			}
			else
			{
				SetItemLayCount(pSourContainer,nFinalDestIndex,nDestMaxLayCount);
				SetItemLayCount(pSourContainer,nSourIndex,nSourLayCount+nDestLayCount-nDestMaxLayCount);
				return nFinalDestIndex ;
			}
		}

		return ITEMOE_DESTOPERATOR_EMPTY;

		__LEAVE_FUNCTION

			return ITEMOE_UNKNOW ;
}





INT ItemOperator::ExchangeItem( ItemContainer* pSourContainer,
								INT nSourIndex ,
								ItemContainer* pDestContainer,
								INT nDestIndex )
{
__ENTER_FUNCTION

	if(		pSourContainer==NULL 
		|| (nSourIndex<0 || nSourIndex>=pSourContainer->GetContainerSize())
		||  pDestContainer==NULL 
		|| (nDestIndex<INVALID_INDEX || nDestIndex>=pDestContainer->GetContainerSize())
		)
	{
		return ITEMOE_UNKNOW ;
	}

	if (!pSourContainer->IsCanUse())
	{
		return ITEMOE_SOUROPUSE_FAIL;
	}

	if (!pDestContainer->IsCanUse())
	{
		return ITEMOE_DESTOPUSE_FAIL;
	}
	Item* pSourItem = NULL ;
	Item* pDestItem = NULL ;

	pDestItem = pDestContainer->GetItem( nDestIndex ) ;
	if( pDestItem==NULL )
	{
		Assert(FALSE) ;
		return ITEMOE_DESTOPERATOR_EMPTY ;
	}
	else if( pDestItem->IsEmpty() )
	{
		return ITEMOE_DESTOPERATOR_EMPTY ;
	}
	else if( pDestItem->IsLock() )
	{
		return ITEMOE_DESTOPERATOR_LOCK ;
	}

	pSourItem = pSourContainer->GetItem( nSourIndex ) ;
	if( pSourItem==NULL )
	{
		Assert(FALSE) ;
		return ITEMOE_SOUROPERATOR_EMPTY ;
	}
	else if( pSourItem->IsEmpty() )
	{
		return ITEMOE_SOUROPERATOR_EMPTY ;
	}
	else if( pSourItem->IsLock() )
	{
		return ITEMOE_SOUROPERATOR_LOCK ;
	}
	
	if(pSourItem->m_Type != pDestItem->m_Type)
	{
		return ITEMOE_DIFF_ITEM_DATA;
	}
	
	switch(pSourItem->m_Type) 
	{
	case IDT_ITEM:
		{
			_ITEM		TmpITEM ;
			const _ITEM_INIT	Init(&TmpITEM) ;

			Item		tempItem;
			tempItem.Init(&Init);
			TmpITEM = *pSourItem->m_pItem ;

			pSourContainer->SetItem( nSourIndex, pDestItem) ;
			pDestContainer->SetItem( nDestIndex, &tempItem) ;
		}
		break;
	case IDT_PET:
		{
			_PET_DB_LOAD	TmpPet ;
			const _ITEM_INIT	Init(&TmpPet) ;

			Item		tempItem;
			tempItem.Init(&Init);
			TmpPet = *pSourItem->m_pPet ;
			pSourContainer->SetItem( nSourIndex, pDestItem) ;
			pDestContainer->SetItem( nDestIndex, &tempItem) ;
		}
		break;
	default:
		{
			Assert(FALSE);
			return ITEMOE_UNKNOW ;
		}
	}

	return ITEMOE_SUCCESS;
__LEAVE_FUNCTION

	return ITEMOE_UNKNOW ;
}

BOOL	ItemOperator::EraseItem(ItemContainer* pDestContainer, UCHAR uIndex)
{
	__ENTER_FUNCTION
		
		if(		pDestContainer==NULL 
			||  (uIndex<0 || uIndex>=pDestContainer->GetContainerSize())
		   )
		{
			return FALSE ;
		}

		Item* pDestItem = pDestContainer->GetItem(uIndex);
		
		if( pDestItem==NULL )
		{
			Assert(FALSE) ;
			return FALSE;
		}
		else if( pDestItem->IsEmpty() )
		{
			return FALSE ;
		}
		else if( pDestItem->IsLock() )
		{
			return FALSE;
		}
		
		return pDestContainer->EraseItem(uIndex);

	__LEAVE_FUNCTION

		return	FALSE;
}

BOOL	ItemOperator::SplitItem(ITEM_LOG_PARAM* pLogParam,
								ItemContainer* pSourContainer, 
								UCHAR uSourIndex,  
								UCHAR uCount,  
								ItemContainer* pDestContainer,
								INT nDestIndex /* =INVALID_INDEX  */)
{
	__ENTER_FUNCTION
		
		if  (	pSourContainer==NULL 
			|| (uSourIndex<0 || uSourIndex>=pSourContainer->GetContainerSize())
			|| pDestContainer ==NULL
			|| (nDestIndex<INVALID_INDEX ))
		{
			return ITEMOE_UNKNOW ;
		}
		
		if (!pDestContainer->IsCanUse())
		{
			return ITEMOE_DESTOPUSE_FAIL;
		}

		Assert(pLogParam);
		Item* pSourItem = pSourContainer->GetItem(uSourIndex) ;

		if( pSourItem==NULL )
		{
			Assert(FALSE) ;
			return ITEMOE_UNKNOW ;
		}
		
		if(pSourItem->GetLayedNum()<=uCount 
		  || uCount == 0)
		{
			return ITEMOE_UNKNOW ;	
		}

		if( pSourItem->IsEmpty() )
		{
			return ITEMOE_SOUROPERATOR_EMPTY ;
		}
		else if( pSourItem->IsLock() )
		{
			return ITEMOE_SOUROPERATOR_LOCK ;
		}
		
		INT nFinalDestIndex = nDestIndex ;
		if( nFinalDestIndex == INVALID_INDEX )
		{  //����Ҹ��ط�������Ʒ
			nFinalDestIndex = pDestContainer->GetEmptyItemIndex() ;
			if( nFinalDestIndex == INVALID_INDEX )
			{
				return ITEMOE_DESTOPERATOR_FULL ;
			}
		}
		
		if(CreateItem(pLogParam,pSourItem,pDestContainer,nFinalDestIndex)!=0)
		{
			return ITEMOE_UNKNOW;
		}

		SetItemLayCount(pDestContainer,nFinalDestIndex,uCount);
		SetItemLayCount(pSourContainer,uSourIndex,pSourItem->GetLayedNum()-uCount);

		return ITEMOE_SUCCESS;
		
	__LEAVE_FUNCTION

		return ITEMOE_UNKNOW;
}

INT	ItemOperator::SpliceItem(ItemContainer* pSourContainer,			//ԭ����
									UCHAR uSourIndex,				//ԭ������λ��
									ItemContainer* pDestContainer,	//Ŀ������
									INT nDestIndex)					//Ŀ��λ��
{
	__ENTER_FUNCTION

		if  (	pSourContainer==NULL 
			|| (uSourIndex<0 || uSourIndex>=pSourContainer->GetContainerSize())
			|| pDestContainer ==NULL
			|| (nDestIndex<INVALID_INDEX ))
		{
			return ITEMOE_UNKNOW ;
		}

		if (!pDestContainer->IsCanUse())
		{
			return ITEMOE_DESTOPUSE_FAIL;
		}

		Item* pSourItem = pSourContainer->GetItem(uSourIndex) ;
		if( NULL==pSourItem )
		{
			Assert(FALSE) ;
			return ITEMOE_UNKNOW ;
		}

		Item* pDestItem = pDestContainer->GetItem(nDestIndex);
		if( NULL==pDestItem )
		{
			Assert(FALSE) ;
			return ITEMOE_UNKNOW ;
		}
		if( pSourItem->GetItemIndex() != pDestItem->GetItemIndex() )
		{
			Assert(FALSE) ;
			return ITEMOE_UNKNOW ;
		}
		if( pSourItem->IsEmpty() || pDestItem->IsEmpty() )
		{
			return ITEMOE_SOUROPERATOR_EMPTY ;
		}
		if( pSourItem->IsLock() || pDestItem->IsLock() )
		{
			return ITEMOE_SOUROPERATOR_LOCK ;
		}
		if( !pSourItem->IsCanLay() ||
			!pDestItem->IsCanLay() ||
			pSourItem->GetLayedNum()+pDestItem->GetLayedNum() > pDestItem->GetMaxLayedNum() )
		{
			Assert(FALSE) ;
			return ITEMOE_UNKNOW ;
		}

		//������Ѿ���ȫ��֤����Ʒ�ܹ�����
		
        //�޸�Ŀ����Ʒ������
		INT nSourLayCount = pSourItem->GetLayedNum();
		INT nDestLayCount = pDestItem->GetLayedNum();
		SetItemLayCount( pDestContainer, nDestIndex, nSourLayCount+nDestLayCount );

		//ɾ��ԭ������Ʒ
		BOOL bRet	= pSourContainer->EraseItem(uSourIndex);
		Assert(bRet);

		return ITEMOE_SUCCESS;

	__LEAVE_FUNCTION
		return ITEMOE_UNKNOW;

}

INT ItemOperator::MoveSpliceItem(ItemContainer* pSourContainer,		//ԭ����
									UCHAR uSourIndex,				//ԭ������λ��
									ItemContainer* pDestContainer,	//Ŀ������
									INT nDestIndex)					//Ŀ��λ��
{
	__ENTER_FUNCTION

		if  (	pSourContainer==NULL 
			|| (uSourIndex<0 || uSourIndex>=pSourContainer->GetContainerSize())
			|| pDestContainer ==NULL
			|| (nDestIndex<INVALID_INDEX ))
		{
			return ITEMOE_UNKNOW ;
		}

		if (!pDestContainer->IsCanUse())
		{
			return ITEMOE_DESTOPUSE_FAIL;
		}

		Item* pSourItem = pSourContainer->GetItem(uSourIndex);
		if( NULL==pSourItem )
		{
			Assert(FALSE) ;
			return ITEMOE_UNKNOW ;
		}

		Item* pDestItem = pDestContainer->GetItem(nDestIndex);
		if( NULL==pDestItem )
		{
			Assert(FALSE) ;
			return ITEMOE_UNKNOW ;
		}
		if( pSourItem->GetItemIndex() != pDestItem->GetItemIndex() )
		{
			Assert(FALSE) ;
			return ITEMOE_UNKNOW ;
		}
		if( !pSourItem->IsCanLay() )
		{
			Assert(FALSE) ;
			return ITEMOE_UNKNOW ;
		}
		if( pSourItem->IsEmpty() || pDestItem->IsEmpty() )
		{
			return ITEMOE_SOUROPERATOR_EMPTY ;
		}
		if( pSourItem->IsLock() || pDestItem->IsLock() )
		{
			return ITEMOE_SOUROPERATOR_LOCK ;
		}

		//������Ѿ���ȫ��֤����Ʒ�ܹ�����

        //�޸�Ŀ����Ʒ������
		INT nSourLayCount = pSourItem->GetLayedNum();
		INT nDestLayCount = pDestItem->GetLayedNum();
		INT nMaxLayCount  = pDestItem->GetMaxLayedNum();
		
		if( nMaxLayCount >= nSourLayCount + nDestLayCount )
		{
			SetItemLayCount( pDestContainer, nDestIndex, nSourLayCount+nDestLayCount );

			//ɾ��ԭ������Ʒ
			BOOL bRet = pSourContainer->EraseItem(uSourIndex);
			Assert(bRet);
		}
		else
		{
			SetItemLayCount( pDestContainer, nDestIndex, nMaxLayCount );
			SetItemLayCount( pSourContainer, uSourIndex, nSourLayCount+nDestLayCount - nMaxLayCount );
		}

		return ITEMOE_SUCCESS;

	__LEAVE_FUNCTION
		return ITEMOE_UNKNOW;
}



VOID ItemOperator::LockItem( ItemContainer* pContainer, INT nIndex )
{
__ENTER_FUNCTION

	if(		pContainer==NULL 
		||  nIndex<0 
		||  nIndex>=pContainer->GetContainerSize() )
	{
		AssertEx( FALSE, "ItemOperator::LockItem" ) ;
		return ;
	}

	Item* pItem = pContainer->GetItem( nIndex ) ;

	pItem->Lock( ) ;

__LEAVE_FUNCTION
}

VOID ItemOperator::UnlockItem( ItemContainer* pContainer, INT nIndex )
{
__ENTER_FUNCTION

	if(		pContainer==NULL 
		||  nIndex<0 
		||  nIndex>=pContainer->GetContainerSize() )
	{
		AssertEx( FALSE, "ItemOperator::UnlockItem" ) ;
		return ;
	}

	Item* pItem = pContainer->GetItem( nIndex ) ;

	pItem->Unlock( ) ;
	pItem->SetInExchange(FALSE);

__LEAVE_FUNCTION
}

BOOL  ItemOperator::SetItemLayCount( ItemContainer* pContainer, INT nIndex, INT nCount )
{
__ENTER_FUNCTION

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetItemLayCount get invalid");
		return FALSE;
	}
	if (!pContainer->IsCanUse())
	{
		return FALSE;
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)
		return FALSE;

	if(pDestItem->IsEmpty())
	{
		return FALSE;
	}

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return FALSE;
	}

	return pContainer->SetItemLayCount(nIndex,nCount);
	
 __LEAVE_FUNCTION
 return FALSE;
}

BOOL ItemOperator::DecItemLayCount(ItemContainer* pContainer,INT nIndex, INT nCount)
{
	__ENTER_FUNCTION
		
		if(!pContainer)
		{
			AssertEx(FALSE,"ItemOperator::DecItemLayCount get invalid");
			return	FALSE;
		}
		
		Item* pDestItem = pContainer->GetItem(nIndex);
		Assert(pDestItem);
		if(!pDestItem)return	FALSE;

		if(pDestItem->IsEmpty())
		{
			return FALSE;
		}

		if(pDestItem->IsLock())
		{	
			Assert(FALSE);
			return	FALSE;
		}

		return pContainer->DecItemLayCount(nIndex,nCount);

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL	ItemOperator::IncItemLayCount(ItemContainer* pContainer,INT nIndex, INT nCount)
{
	__ENTER_FUNCTION
		
		if(!pContainer)
		{
			AssertEx(FALSE,"ItemOperator::IncItemLayCount get invalid");
			return	FALSE;
		}
		if (!pContainer->IsCanUse())
		{
			return FALSE;
		}

		Item* pDestItem = pContainer->GetItem(nIndex);
		Assert(pDestItem);
		if(!pDestItem)return	FALSE;

		if(pDestItem->IsLock())
		{	
			Assert(FALSE);
			return	FALSE;
		}

		return pContainer->IncItemLayCount(nIndex,nCount);

	__LEAVE_FUNCTION

		return	FALSE;
}


BOOL	ItemOperator::SetItemDur(ItemContainer* pContainer,INT nIndex, INT nDur)
{
	__ENTER_FUNCTION

		if(!pContainer)
		{
			AssertEx(FALSE,"ItemOperator::SetItemDur get Invalid");
			return	FALSE;
		}

		if (!pContainer->IsCanUse())
		{
			return FALSE;
		}

		Item* pDestItem = pContainer->GetItem(nIndex);
		Assert(pDestItem);
		if(!pDestItem)return	FALSE;

		if(pDestItem->IsLock())
		{	
			Assert(FALSE);
			return	FALSE;
		}

		return pContainer->SetItemDur(nIndex,nDur);

	__LEAVE_FUNCTION

		return	FALSE;
}

BOOL	ItemOperator::SetItemIdent(ItemContainer* pContainer,INT nIndex)
{
	__ENTER_FUNCTION

		if(!pContainer)
		{
			AssertEx(FALSE,"ItemOperator::SetItemIdent get Invalid");
			return	FALSE;
		}

		if (!pContainer->IsCanUse())
		{
			return FALSE;
		}

		Item* pDestItem = pContainer->GetItem(nIndex);
		Assert(pDestItem);
		if(!pDestItem)return	FALSE;

		if(pDestItem->IsLock())
		{	
			Assert(FALSE);
			return	FALSE;
		}

		return pContainer->SetItemIdent(nIndex);

		__LEAVE_FUNCTION

		return	FALSE;
}

BOOL	ItemOperator::SetItemDamagePoint(ItemContainer* pContainer,INT nIndex, INT nPoint)
{
	__ENTER_FUNCTION

		if(!pContainer)
		{
			AssertEx(FALSE,"ItemOperator::SetItemDamagePoint get Invalid");
			return	FALSE;
		}

		if (!pContainer->IsCanUse())
		{
			return FALSE;
		}

		Item* pDestItem = pContainer->GetItem(nIndex);
		Assert(pDestItem);
		if(!pDestItem)return	FALSE;

		if(pDestItem->IsLock())
		{	
			Assert(FALSE);
			return	FALSE;
		}

		return pContainer->SetItemDamagePoint(nIndex,nPoint);

	__LEAVE_FUNCTION

		return	FALSE;
}

BOOL	ItemOperator::SetItemMaxDur(ItemContainer* pContainer,INT nIndex, INT nDur)
{
	__ENTER_FUNCTION

		if(!pContainer)
		{
			AssertEx(FALSE,"ItemOperator::SetItemDur get Invalid");
			return	FALSE;
		}

		if (!pContainer->IsCanUse())
		{
			return FALSE;
		}

		Item* pDestItem = pContainer->GetItem(nIndex);
		Assert(pDestItem);
		if(!pDestItem)return	FALSE;

		if(pDestItem->IsLock())
		{	
			Assert(FALSE);
			return	FALSE;
		}

		return pContainer->SetItemMaxDur(nIndex,nDur);

		__LEAVE_FUNCTION

			return	FALSE;
}

BOOL	ItemOperator::SetItemCurMaxDur(ItemContainer* pContainer,INT nIndex, INT nDur)
{
	__ENTER_FUNCTION

		if(!pContainer)
		{
			AssertEx(FALSE,"ItemOperator::SetItemDur get Invalid");
			return	FALSE;
		}
		if (!pContainer->IsCanUse())
		{
			return FALSE;
		}

		Item* pDestItem = pContainer->GetItem(nIndex);
		Assert(pDestItem);
		if(!pDestItem)return	FALSE;

		if(pDestItem->IsLock())
		{	
			Assert(FALSE);
			return	FALSE;
		}

		return pContainer->SetItemCurMaxDur(nIndex,nDur);

		__LEAVE_FUNCTION

			return	FALSE;
}

BOOL	ItemOperator::SetItemBind(ItemContainer* pContainer,INT nIndex)
{
	__ENTER_FUNCTION
		
		if(!pContainer)
		{
			AssertEx(FALSE,"ItemOperator::SetItemBind get Invalid");
			return	FALSE;
		}
		if (!pContainer->IsCanUse())
		{
			return FALSE;
		}
		Item* pDestItem = pContainer->GetItem(nIndex);
		Assert(pDestItem);
		if(!pDestItem)return	FALSE;

		if(pDestItem->IsLock())
		{	
			Assert(FALSE);
			return	FALSE;
		}

		return pContainer->SetItemBind(nIndex);

	__LEAVE_FUNCTION

		return FALSE;
}


BOOL ItemOperator::SetItemPWLock(ItemContainer*	pContainer,INT nIndex, BOOL bLock)
{
	__ENTER_FUNCTION

		if(!pContainer)
		{
			AssertEx(FALSE,"ItemOperator::SetItemPWLock get Invalid");
			return	FALSE;
		}

		Item* pDestItem = pContainer->GetItem(nIndex);
		Assert(pDestItem);
		if(!pDestItem)return	FALSE;

		if(pDestItem->IsLock())
		{	
			Assert(FALSE);
			return	FALSE;
		}

		return pContainer->SetItemPWLock(nIndex, bLock);

		__LEAVE_FUNCTION

			return FALSE;
}

BOOL ItemOperator::SetItemCreator(ItemContainer* pContainer,INT nIndex,const CHAR* CreatorName)
{
	__ENTER_FUNCTION

		if(!pContainer)
		{
			AssertEx(FALSE,"ItemOperator::SetItemCreator get Invalid");
			return	FALSE;
		}
		if (!pContainer->IsCanUse())
		{
			return FALSE;
		}

		Item* pDestItem = pContainer->GetItem(nIndex);
		Assert(pDestItem);
		if(!pDestItem)return	FALSE;

		if(pDestItem->IsLock())
		{	
			Assert(FALSE);
			return	FALSE;
		}

		return pContainer->SetItemCreator(nIndex, CreatorName);

		__LEAVE_FUNCTION

			return FALSE;
}

BOOL ItemOperator::SetItemParam(ItemContainer* pContainer,INT nIndex,UINT start,INT type,INT value)
{
	__ENTER_FUNCTION

		if(!pContainer)
		{
			AssertEx(FALSE,"ItemOperator::SetItemParam get Invalid");
			return	FALSE;
		}

		Item* pDestItem = pContainer->GetItem(nIndex);
		Assert(pDestItem);
		if(!pDestItem)return	FALSE;

		if(pDestItem->IsLock())
		{	
			Assert(FALSE);
			return	FALSE;
		}

		return pContainer->SetItemParam(nIndex,start,type,value);

		__LEAVE_FUNCTION

			return FALSE;
}

VOID ItemOperator::SetItemGUID( ItemContainer* pContainer, INT nIndex, _ITEM_GUID* pGuid )
{
__ENTER_FUNCTION
	
	Assert(pContainer);
	Assert(pGuid);
	
	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetItemGUID() Get Invalid");
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)
	{
		return;
	}

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return;
	}
	
	pContainer->SetItemGuid(nIndex,pGuid);

	__LEAVE_FUNCTION
}


VOID ItemOperator::SetItemGUID( ItemContainer* pContainer, INT nIndex, PET_GUID_t* pGuid ) 
{
	__ENTER_FUNCTION

	Assert(pContainer);
	Assert(pGuid);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetItemGUID() Get Invalid");
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)
	{
		return;
	}

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return;
	}

	pContainer->SetItemGuid(nIndex,pGuid);

	__LEAVE_FUNCTION
}

BOOL ItemOperator::GenItemGUID(_ITEM_GUID& ig)
{
__ENTER_FUNCTION

	return	g_pItemManager->GenItemGuid(ig);

__LEAVE_FUNCTION

	return FALSE;
}

BOOL ItemOperator::GenItemGUID(PET_GUID_t& ig)
{
	__ENTER_FUNCTION
	
		//todo
		return TRUE;

	__LEAVE_FUNCTION
		
		return FALSE;
}

BOOL	ItemOperator::AddItemAttr(ItemContainer* pContainer,INT nIndex,_ITEM_ATTR iA)
{
	__ENTER_FUNCTION

		Assert(pContainer);

		if(!pContainer)
		{
			AssertEx(FALSE,"ItemOperator::AddItemAttr() Get Invalid");
		}
		if (!pContainer->IsCanUse())
		{
			return FALSE;
		}

		Item* pDestItem = pContainer->GetItem(nIndex);
		Assert(pDestItem);
		if(!pDestItem)return	FALSE;

		if(pDestItem->IsLock())
		{	
			Assert(FALSE);
			return	FALSE;
		}
		return pContainer->AddItemAttr(nIndex,iA);

	__LEAVE_FUNCTION

		return	FALSE;
}

BOOL	ItemOperator::DelItemAttr(ItemContainer* pContainer,INT nIndex,_ITEM_ATTR iA)
{
	__ENTER_FUNCTION

		Assert(pContainer);

		if(!pContainer)
		{
			AssertEx(FALSE,"ItemOperator::DelItemAttr() Get Invalid");
		}

		Item* pDestItem = pContainer->GetItem(nIndex);
		Assert(pDestItem);
		if(!pDestItem)	return	FALSE;

		if(pDestItem->IsLock())
		{	
			Assert(FALSE);
			return	FALSE;
		}
		return pContainer->DelItemAttr(nIndex,iA);

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL	ItemOperator::DelGemInfo(ItemContainer* pContainer,INT nIndex,INT GemIndex)
{
	__ENTER_FUNCTION

		Assert(pContainer);
		
		if(!pContainer)
		{
			AssertEx(FALSE,"ItemOperator::DelItemInfo() Get Invalid");
		}
		if (!pContainer->IsCanUse())
		{
			return FALSE;
		}

		Item* pDestItem = pContainer->GetItem(nIndex);
		Assert(pDestItem);
		if(!pDestItem)	return	FALSE;

		if(pDestItem->IsLock())
		{	
			Assert(FALSE);
			return	FALSE;
		}
		return pContainer->DelGemInfo(nIndex,GemIndex);

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL	ItemOperator::AddGemInfo(ItemContainer* pContainer,INT nIndex,INT GemIndex,UINT GemHolePos)
{
	__ENTER_FUNCTION

		Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::AddItemInfo() Get Invalid");
	}
	if (!pContainer->IsCanUse())
	{
		return FALSE;
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}
	return pContainer->AddGemInfo(nIndex,GemIndex, GemHolePos);

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL  	ItemOperator::SetItemValue(ItemContainer*	pContainer,INT nIndex,const _PET_DB_LOAD*	pPet)
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetItemValue Get Invalid");
	}
	if (!pContainer->IsCanUse())
	{
		return FALSE;
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}
	return pContainer->SetItemValue(nIndex,pPet);

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL  	ItemOperator::SetPetGUID(ItemContainer*	pContainer,INT nIndex,PET_GUID_t GUID)				//���ó���GUID
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetPetGUID() Get Invalid");
	}
	if (!pContainer->IsCanUse())
	{
		return FALSE;
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}
	return pContainer->SetPetGUID(nIndex,GUID);

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL  	ItemOperator::SetSpouseGUID(ItemContainer*	pContainer,INT nIndex,PET_GUID_t GUID)
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetSpouseGUID() Get Invalid");
	}
	if (!pContainer->IsCanUse())
	{
		return FALSE;
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}
	return pContainer->SetSpouseGUID(nIndex,GUID);

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL	ItemOperator::SetDataID(ItemContainer*	pContainer,INT nIndex,INT ID)							//���ó���ģ��		
{
	__ENTER_FUNCTION

		Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetDataID() Get Invalid");
	}
	if (!pContainer->IsCanUse())
	{
		return FALSE;
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}
	return pContainer->SetDataID(nIndex,ID);

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL 	ItemOperator::SetName(ItemContainer*	pContainer,INT nIndex,const CHAR* pName)						//��������
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetName() Get Invalid");
	}
	if (!pContainer->IsCanUse())
	{
		return FALSE;
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	Assert(pName);

	return pContainer->SetName(nIndex,pName);

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL   	ItemOperator::SetNick(ItemContainer*	pContainer,INT nIndex,const CHAR* pNick)						//�����ǳ�
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetNick() Get Invalid");
	}
	if (!pContainer->IsCanUse())
	{
		return FALSE;
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	Assert(pNick);

	return pContainer->SetNick(nIndex,pNick);

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL  	ItemOperator::SetLevel(ItemContainer*	pContainer,INT nIndex,INT level)						//���õȼ�
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetLevel() Get Invalid");
	}
	if (!pContainer->IsCanUse())
	{
		return FALSE;
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	return pContainer->SetLevel(nIndex,level);

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL  	ItemOperator::SetTakeLevel(ItemContainer*	pContainer,INT nIndex,INT takeLevel)				//����Я���ȼ�
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetTakeLevel() Get Invalid");
	}
	if (!pContainer->IsCanUse())
	{
		return FALSE;
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	return pContainer->SetTakeLevel(nIndex,takeLevel);

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL  	ItemOperator::SetAttackType(ItemContainer*	pContainer,INT nIndex,INT attackType)				//���ý������ͣ���/����
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetAttackType() Get Invalid");
	}
	if (!pContainer->IsCanUse())
	{
		return FALSE;
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	return pContainer->SetAttackType(nIndex,attackType);

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL  	ItemOperator::SetAIType(ItemContainer*	pContainer,INT nIndex,INT AIType)						//����AI����
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetAIType() Get Invalid");
	}
	if (!pContainer->IsCanUse())
	{
		return FALSE;
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	return pContainer->SetAIType(nIndex,AIType);

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL  	ItemOperator::SetCampData(ItemContainer*	pContainer,INT nIndex,const _CAMP_DATA* pCamp)		//������Ӫ
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetCampData() Get Invalid");
	}
	if (!pContainer->IsCanUse())
	{
		return FALSE;
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	return pContainer->SetCampData(nIndex,pCamp);

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL  	ItemOperator::SetHP(ItemContainer*	pContainer,INT nIndex,INT hp)								//��������ֵ
{
	__ENTER_FUNCTION

	//Assert(pContainer);

	//if(!pContainer)
	//{
	//	AssertEx(FALSE,"ItemOperator::SetHP() Get Invalid");
	//}

	//Item* pDestItem = pContainer->GetItem(nIndex);
	//Assert(pDestItem);
	//if(!pDestItem)	return	FALSE;

	//if(pDestItem->IsLock())
	//{	
	//	Assert(FALSE);
	//	return	FALSE;
	//}

	//return pContainer->SetHP(nIndex,hp);
	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL  	ItemOperator::SetLife(ItemContainer*	pContainer,INT nIndex,INT Life)						//���õ�ǰ����
{
	__ENTER_FUNCTION

	//Assert(pContainer);

	//if(!pContainer)
	//{
	//	AssertEx(FALSE,"ItemOperator::SetLife() Get Invalid");
	//}

	//Item* pDestItem = pContainer->GetItem(nIndex);
	//Assert(pDestItem);
	//if(!pDestItem)	return	FALSE;

	//if(pDestItem->IsLock())
	//{	
	//	Assert(FALSE);
	//	return	FALSE;
	//}

	//return pContainer->SetLife(nIndex,Life);
	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;

}

BOOL  	ItemOperator::SetPetType(ItemContainer*	pContainer,INT nIndex,BYTE PetType)					//���������죬Ұ��
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetPetType() Get Invalid");
	}
	if (!pContainer->IsCanUse())
	{
		return FALSE;
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	return pContainer->SetPetType(nIndex,PetType);

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL  	ItemOperator::SetGeneration(ItemContainer*	pContainer,INT nIndex,BYTE Gen)					//������
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetGeneration() Get Invalid");
	}
	if (!pContainer->IsCanUse())
	{
		return FALSE;
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	return pContainer->SetGeneration(nIndex,Gen);

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL  	ItemOperator::SetHappiness(ItemContainer*	pContainer,INT nIndex,BYTE byHappiness)						//���ֶ�		
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetHappiness() Get Invalid");
	}
	if (!pContainer->IsCanUse())
	{
		return FALSE;
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	return pContainer->SetHappiness(nIndex,byHappiness);

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL  	ItemOperator::SetStrPer(ItemContainer*	pContainer,INT nIndex,INT strper)						//��������	
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetStrPer() Get Invalid");
	}
	if (!pContainer->IsCanUse())
	{
		return FALSE;
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	return pContainer->SetStrengthPer(nIndex,strper);

	__LEAVE_FUNCTION

	return FALSE;
}

//��������	
BOOL  	ItemOperator::SetSmartnessPer(ItemContainer*	pContainer,INT nIndex,INT conper) 
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetConPer() Get Invalid");
	}
	if (!pContainer->IsCanUse())
	{
		return FALSE;
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	return pContainer->SetSmartnessPer(nIndex,conper);

	__LEAVE_FUNCTION

	return FALSE;
}

//��������
BOOL   	ItemOperator::SetMindPer(ItemContainer*	pContainer,INT nIndex,INT dexper)
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetDexPer() Get Invalid");
	}
	if (!pContainer->IsCanUse())
	{
		return FALSE;
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	return pContainer->SetMindPer(nIndex,dexper);

	__LEAVE_FUNCTION

	return FALSE;
}

//��������
BOOL   	ItemOperator::SetConstitutionPer(ItemContainer*	pContainer,INT nIndex,INT intper)
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetIntPer() Get Invalid");
	}
	if (!pContainer->IsCanUse())
	{
		return FALSE;
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	return pContainer->SetConstitutionPer(nIndex,intper);

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL	ItemOperator::SetGenGu(ItemContainer*	pContainer,INT nIndex,INT gengu)						//����
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetGenGu() Get Invalid");
	}
	if (!pContainer->IsCanUse())
	{
		return FALSE;
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	return pContainer->SetGenGu(nIndex,gengu);

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL ItemOperator::SetSavvy(ItemContainer*	pContainer,INT nIndex,INT iSavvy)
{
	__ENTER_FUNCTION

		Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetGenGu() Get Invalid");
	}
	if (!pContainer->IsCanUse())
	{
		return FALSE;
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	return pContainer->SetSavvy(nIndex,iSavvy);

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL  	ItemOperator::SetGrowRate(ItemContainer*	pContainer,INT nIndex,FLOAT rate)					//�ɳ���
{
	__ENTER_FUNCTION

		Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetGrowRate() Get Invalid");
	}
	if (!pContainer->IsCanUse())
	{
		return FALSE;
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	return pContainer->SetGrowRate(nIndex,rate);

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  	ItemOperator::SetRemainPoint(ItemContainer*	pContainer,INT nIndex,INT rPoint)					//һ������ʣ�����
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetRemainPoint() Get Invalid");
	}
	if (!pContainer->IsCanUse())
	{
		return FALSE;
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	return pContainer->SetRemainPoint(nIndex,rPoint);

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  	ItemOperator::SetExp(ItemContainer*	pContainer,INT nIndex,INT exp)						//����ֵ
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetExp() Get Invalid");
	}
	if (!pContainer->IsCanUse())
	{
		return FALSE;
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	return pContainer->SetExp(nIndex,exp);

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  	ItemOperator::SetLvl1Attr(ItemContainer*	pContainer,INT nIndex,CHAR_ATTR_LEVEL1 type,INT value)//����һ��ս�����ԣ����������ܺ�װ�����ӵĲ��֣�
{
	__ENTER_FUNCTION

		Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetLvl1Attr() Get Invalid");
	}
	if (!pContainer->IsCanUse())
	{
		return FALSE;
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	return pContainer->SetLvl1Attr(nIndex,type,value);

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  	ItemOperator::SetSkill(ItemContainer*	pContainer,INT nIndex,UINT SkillIndex,_PET_SKILL skill)	 //���＼��
{	
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetSkill() Get Invalid");
	}
	if (!pContainer->IsCanUse())
	{
		return FALSE;
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	return pContainer->SetSkill(nIndex,SkillIndex,skill);

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL ItemOperator::SetMatingFlag( ItemContainer* pContainer,INT nIndex, BOOL bFlag )
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetMatingFlag() Get Invalid");
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	return pContainer->SetMatingFlag(nIndex,bFlag);

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL ItemOperator::SetMatingStartTime( ItemContainer* pContainer,INT nIndex, UINT uTime )
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetMatingStartTime() Get Invalid");
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	return pContainer->SetMatingStartTime(nIndex,uTime);

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL ItemOperator::SetMatingFinishFlag( ItemContainer* pContainer,INT nIndex, BOOL bFlag )
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetMatingFinishFlag() Get Invalid");
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	return pContainer->SetMatingFinishFlag(nIndex,bFlag);

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL ItemOperator::SetMatingLevel( ItemContainer* pContainer,INT nIndex, INT iLevel )
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetMatingLevel() Get Invalid");
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	return pContainer->SetMatingLevel(nIndex,iLevel);

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL ItemOperator::SetPetSex( ItemContainer* pContainer,INT nIndex, INT iSex )
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetPetSex() Get Invalid");
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	return pContainer->SetPetSex(nIndex,iSex);

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL ItemOperator::SetCheckUpFlag( ItemContainer* pContainer,INT nIndex, BOOL bCheckup )
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetCheckUpFlag() Get Invalid");
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	return pContainer->SetCheckUpFlag(nIndex,bCheckup);

	__LEAVE_FUNCTION

	return FALSE;
}