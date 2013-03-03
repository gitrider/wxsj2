/********************************************************************
��������:	2008��1��22��
�ļ�����:	Obj_ItemPool.h
������:		�

�ļ����ܣ� �ػ���Ϸ�е�����Ʒ����
�޸ļ�¼��

*********************************************************************/
#ifndef		_ITEM_POOL_H_
#define		_ITEM_POOL_H_
#include	"Type.h"

#define		ITEM_POOL_INVAILD	-1				//��ЧObj_Item���


class Obj_Item;



/*
*		Obj_Item	����
*/
class Obj_ItemPool
{
	//Obj_Item����
	Obj_Item*		m_pObj_Item;
	//����Obj_Item��Id
	INT*			m_pFreeObj_ItemID;
	//ʹ�õ�Obj_Item��Id����
	INT*			m_pUseObj_ItemID;
	//����λ�ñ��
	INT				m_nFreePosition;
	//������
	MyLock			m_Lock;



public:
	Obj_ItemPool();
	~Obj_ItemPool();
	/*
	*	���Obj_ItemPool ��ʼ��,��Server->Init��ִ��
	*/
	BOOL			Init();

	/*
	*	����һ���µ�Obj_Box
	*
	*	
	*/

	Obj_Item*		NewObj_Item();

	/*	ɾ��һ��Obj_Itembox
	*	
	*	
	*
	*/
	//����λ��ɾ��
	VOID			DelObj_Item(uint iItemPos);
	//����ָ��ɾ��
	VOID			DeleteObj(Obj_Item*	pItem);


};

extern Obj_ItemPool* g_pObj_ItemPool;


#endif
