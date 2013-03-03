/********************************************************************
��������:	2005��11��1��
����ʱ��:	10:28
�ļ�����:	ItemManager.h
�ļ�·��:	d:\Prj\Server\Server\Item\ItemManager.h
������:		������

�ļ����ܣ�	����Ʒ����������,���ݽ��й�����
�޸ļ�¼��

*********************************************************************/

#ifndef _ITEM_MANAGER_H_
#define _ITEM_MANAGER_H_


#include "Type.h"
#include "LuaInterface.h"
#include "LogDefine.h"

class Scene;
class Obj_Human;
struct _CREATEITEM_INIT;

class ItemManager
{

	MyLock			m_Lock;

public:

	//��Ʒ��������ʼ��
	BOOL			Init();

	//����һ��Ψһ����ƷSerial
	INT				GenItemSerial();

	//����һ��Ψһ��ƷGUID
	BOOL			GenItemGuid(_ITEM_GUID& ig);

	/*	
	 *	����_ITEM_TYPE ���� _ITEM
	 *	iQuality ����ɫװ����Ʒ�ʶ� 
	 */
	BOOL			CreateItem(ITEM_LOG_PARAM* pLogParam,_ITEM_TYPE it,_ITEM& item,INT iQuality=1);

	/*	
	 *	���� ItemSerial ���� _ITEM
	 *  iQuality ����ɫװ����Ʒ�ʶ� 
	 */
	BOOL			CreateItem(ITEM_LOG_PARAM* pLogParam,UINT ItemSerial,_ITEM& item,INT iQuality=1);

	
	/*	
	*	���� _CREATEITEM_INIT ���� _ITEM
	*  pInit �ǳ�ʼ������ 
	*/
	BOOL			CreateItem(ITEM_LOG_PARAM* pLogParam, _ITEM& item, _CREATEITEM_INIT* pInit=NULL);
protected:
private:
	/*
	 *	�ڲ�ʹ�÷�����Ʒ���ɷ���
	 */
	//����װ��
	BOOL			_CreateEquipFromTable(ITEM_LOG_PARAM* pLogParam,_ITEM_TYPE  it,INT iQuality,_ITEM& item);
	//������ʯ
	BOOL			_CreateGemFromTable(_ITEM_TYPE it,_ITEM& item);
	//������ʯ
	BOOL			_CreateGemFromTable(UINT	TableIndex,_ITEM& item);
	//������ͨ����
	BOOL			_CreateMedicFromTable(_ITEM_TYPE it,_ITEM& item);

	BOOL			_CreateTaskFromTable(_ITEM_TYPE it,_ITEM& item);
	
	//�����ر�ͼ
	BOOL			_CreateStoreMapFromTable(_ITEM_TYPE	it,_ITEM& item);
	
	//�����������
	BOOL			_CreateExtraBagFromTable(_ITEM_TYPE	it,_ITEM& item);

	//�������������add by gh for Soulbead 2010/06/28
	BOOL			_CreateSoulBeadFromTable(_ITEM_TYPE	it,_ITEM& item);
	//�������� [7/14/2010 �¾���]
	BOOL	        _CreateMountFromTable(_ITEM_TYPE	it,_ITEM& item);
	//����װ��
	BOOL			_CreateEquipFromTable(ITEM_LOG_PARAM* pLogParam, _ITEM& item, _CREATEITEM_INIT* pInit);
	//������ʯ
	BOOL			_CreateGemFromTable(_ITEM& item, _CREATEITEM_INIT* pInit);

	//������ͨ����
	BOOL			_CreateMedicFromTable(_ITEM& item, _CREATEITEM_INIT* pInit);

	BOOL			_CreateTaskFromTable(_ITEM& item, _CREATEITEM_INIT* pInit);

	//�����ر�ͼ
	BOOL			_CreateStoreMapFromTable(_ITEM& item, _CREATEITEM_INIT* pInit);

	//�����������
	BOOL			_CreateExtraBagFromTable(_ITEM& item, _CREATEITEM_INIT* pInit);

	//��������
	BOOL	        _CreateMountFromTable(_ITEM& item, _CREATEITEM_INIT* pInit);
	//��Ʒ�������к�
	INT				m_nLastGenItemSerial;
	
	//������
	ID_t			m_WorldId;
	//���������
	ID_t			m_ServerId;
	//�����ڴ���ʵ�Ԫ
	ItemSerialKeySMU*	m_pSerialSMU;
};

BOOL EquipCanAddHole(EQUIP_TYPE equip_type);

//ȫ��Ψһ��Ʒ������
extern	ItemManager*		g_pItemManager;
#endif