/********************************************************************
	��������:	2005��11��1��
	����ʱ��:	9:30
	�ļ�����:	ItemBoxManager.h
	�ļ�·��:	d:\Prj\Server\Server\Item\ItemBoxManager.h
	������:		������
	
	�ļ����ܣ� ʵ�ֶ�Obj_ItemBox�Ĺ���
			   ÿ������һ��������
	�޸ļ�¼��
	
*********************************************************************/
#ifndef			_ITEMBOX_MANAGER_H_
#define			_ITEMBOX_MANAGER_H_
#include		"Type.h"

class Scene;
class Obj_ItemBox;
struct ItemBoxContaner;
/*
 *	ItemBox ��������
 */
class ItemBoxManager
{

public:
	ItemBoxManager();
	~ItemBoxManager();
public:

	VOID				Init();
	//���������Ϣ
	VOID				CleanUp( ) ;
	//�߼��ӿ�
	virtual BOOL		HeartBeat( UINT uTime=0 ) ;
	
	//���ItemBox ��������
	BOOL				Add(Obj_ItemBox*	pItemBox);

	//�Ƴ�ItemBox ��������
	BOOL				Remove(ObjID_t ObjID);

	//����ItemBox
	Obj_ItemBox*		GenItemBox();
	//����ItemBox
	VOID				RecycleItemBox(ObjID_t ObjID);

	//����ItemBoxManager ��������
	VOID				SetScene( Scene* pScene )
	{Assert( pScene ) ;
	m_pScene = pScene ;} ;
	
	//���ItemBox ��������
	Scene*				GetScene(){ return m_pScene ; } ;

	/*
	 *	�ı䵽ָ����С	
	 */
	BOOL				Resize(uint iSize);
	
	Obj_ItemBox*		GetItemBox(ObjID_t ObjID);

	BOOL				RemoveAllItemBox( VOID );
public:



	//���㲿�ַ���
	/*
	 *	�ӹ�������������Ʒ����
	 *
	 *	uint	iMonsterLevel			���Ｖ��
	 *	uint	iKillerLevel			ɱ�ֽ�ɫ����
	 *	INT		iMonsterType			��������
	 *
	 */
	/*ItemBoxContaner			CaculateItemDropFromMonster( uint iMonsterLevel,
														 uint iKillerLevel,
														 INT  iMonsterType);*/

	/*
	 *	
	 */
	Obj_ItemBox*			CreateMonsterDropItembox(WORLD_POS* pPos);


	/*
	 *	������ItemBox �Ĵ���
	 */
	Obj_ItemBox*			CreateGrowPointItemBox(INT	IBType);
	
	
private:
	/*
	 *	�߼�ʱ�����
	 */
	CMyTimer			m_LogicTimer;
private:
	// ItemBox id ����
	ObjID_t*			m_pItemBoxObjID ;
	// ItemBoxManager �������С
	uint				m_TotalSize ;
	// ��ǰ�����ItemBox����
	uint				m_Count ;
	// ����ָ��
	Scene*				m_pScene;


};	

#endif