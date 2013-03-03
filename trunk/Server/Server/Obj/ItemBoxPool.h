/********************************************************************
	��������:	2005��11��1��
	����ʱ��:	9:22
	�ļ�����:	ItemBoxPool.h
	�ļ�·��:	d:\Prj\Server\Server\Item\ItemBoxPool.h
	������:		������
	
	�ļ����ܣ� ?
	�޸ļ�¼��
	
*********************************************************************/



#ifndef		_ITEMBOX_POOL_H_
#define		_ITEMBOX_POOL_H_
#include	"Type.h"

#define		ITEM_POOL_INVAILD	-1				//��ЧObj_ItemBox���


class Obj_ItemBox;



 /*
  *		Obj_ItemBox	����
  */
class ItemBoxPool
{
	//Obj_ItemBox����
	Obj_ItemBox*		m_pItemBox;
	//����Itembox��Id
	INT*			m_pFreeItemID;
	//ʹ�õ�Obj_ItemBox��Id����
	INT*			m_pUseItemID;
	//����λ�ñ��
	INT				m_nFreePosition;
	//������
	MyLock			m_Lock;

	

public:
	ItemBoxPool();
	~ItemBoxPool();
	/*
	 *	���ItemBoxPool ��ʼ��,��Server->Init��ִ��
	 */
	BOOL			Init();
	
	/*
	 *	����һ���µ�Obj_ItemBox
	 *
	 *	
	 */
	
	Obj_ItemBox*		NewItemBox();
	
	/*	ɾ��һ��Itembox
	 *	
	 *	
	 *
	 */
	//����λ��ɾ��
	VOID			DelItemBox(uint iItemPos);
	//����ָ��ɾ��
	VOID			DeleteObj(Obj_ItemBox*	pItem);


};

extern ItemBoxPool* g_pItemBoxPool;


#endif