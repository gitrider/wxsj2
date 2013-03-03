/********************************************************************
��������:	2008��1��22��
�ļ�����:	Obj_ItemManager.h
������:		�

�ļ����ܣ� ʵ�ֶ�Obj_Item�Ĺ���
ÿ������һ��������
�޸ļ�¼��

*********************************************************************/
#ifndef			_OBJ_ITEM_MANAGER_H_
#define			_OBJ_ITEM_MANAGER_H_
#include		"Type.h"
#include		"Item.h"

class Scene;
class Obj_Item;
/*
*	Obj_Item ��������
*/
class Obj_ItemManager
{

public:
	Obj_ItemManager();
	~Obj_ItemManager();
public:

	VOID				Init();
	//���������Ϣ
	VOID				CleanUp( ) ;
	//�߼��ӿ�
	virtual BOOL		HeartBeat( UINT uTime=0 ) ;

	//���Obj_Item ��������
	BOOL				Add(Obj_Item*	pItem);

	//�Ƴ�Obj_Item ��������
	BOOL				Remove(ObjID_t ObjID);

	//����Obj_Item
	Obj_Item*			GenObj_Item();
	//����Obj_Item
	VOID				RecycleObj_Item(ObjID_t ObjID);

	//����Obj_ItemManager ��������
	VOID				SetScene( Scene* pScene )
	{Assert( pScene ) ;
	m_pScene = pScene ;} ;

	//���Obj_Item ��������
	Scene*				GetScene(){ return m_pScene ; } ;

	/*
	*	�ı䵽ָ����С	
	*/
	BOOL				Resize(uint iSize);

	Obj_Item*			GetObj_Item(ObjID_t ObjID);

	BOOL				RemoveAllObj_Item( VOID );
public:



	//���㲿�ַ���
	/*
	*	�ӹ�������������Ʒ����
	*
	*	uint	iMonsterLevel			���Ｖ��
	*	uint	iKillerLevel			ɱ�ֽ�ɫ����
		uint	iMonsterID				����ID	
	*	INT		iMonsterType			��������
	*
	*/
	Obj_ItemContaner			CaculateItemDropFromMonster(uint iKillerLevel,
		uint iMonsterID,
		BOOL bTeam);
	/*
	*	
	*/
	Obj_Item*				CreateMonsterDropObj_Item(WORLD_POS* pPos,  _ITEM* item);



	Obj_Item*				CreatePersonDropObj_Item(WORLD_POS* pPos, Item* pItem);
	///*
	//*	������Item �Ĵ���
	//*/
	//Obj_Item*				CreateGrowPointObj_Item(INT	IBType);


private:
	/*
	*	�߼�ʱ�����
	*/
	CMyTimer			m_LogicTimer;
private:
	// Obj_Item id ����
	ObjID_t*			m_pObj_ItemID ;
	// Obj_ItemManager �������С
	uint				m_TotalSize ;
	// ��ǰ�����Obj_Item����
	uint				m_Count ;
	// ����ָ��
	Scene*				m_pScene;
};	

#endif

