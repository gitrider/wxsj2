
#pragma once


#include "Obj_Item.h"
#include "GameDefine2.h"
#include "GameDefine_Item.h"
#include "Obj_Item_Equip.h"
#include "GIDBC_Struct.h"
#include "../../ObjectDef.h"
#include "GameStruct_Item.h"
#include "DBC\GMDataBase.h"
#include "Procedure/GameProcedure.h"
#include "GIException.h"

/*
|
|	����  [7/8/2010 �¾���]
|
*/
class CObject;
struct PET_GUID_t;
class CObject_Item_Mount : public CObject_Item
{
public:
	enum MOUNT_BIND_TYPE
	{
		GETUP_BIND = 0,  		// ��ð�
		EQUIP_BIND,				// װ����  
	};
	//const INT MILLISECOND_IN_ONE_DAY = 86400000; //1000 * 60 * 60 * 24

public:

	// �õ���Ʒ�����
	virtual ITEM_CLASS		GetItemClass(VOID) const ;
	// �õ�type��Ϣ
	virtual INT				GetItemTableType(VOID);

	// ͼ����
	virtual LPCTSTR			GetIconName(VOID) const;

	// ��Ʒ������
	virtual LPCTSTR			GetName(VOID) const			{ return m_theBaseDef->szName; }
	// ��Ʒ����
	virtual LPCTSTR			GetDesc(VOID) const			{ return m_theBaseDef->szDesc; }

	// �õ���Ʒ����ļ۸�
	virtual INT			    GetItemBasePrice();

	// �õ�������
	virtual INT				GetItemPrice();

	// �õ���Чʱ��
	INT		                GetEffectTime();

	// �õ���Ч������
	INT		                GetEffectGridNum();

	// �õ���Ʒ���������� 
	virtual LPCTSTR			GetItemTableTypeDesc();

	// �õ�����ʱ��ģ��id
	virtual LPCTSTR			GetDropVisualID(VOID)	const;
	// ��ʾ������ɫ
	virtual LPCTSTR			GetDropVisColor(VOID) const ;


	// ������ϸ����
	virtual VOID			SetExtraInfo(const _ITEM * pItemInfo);
	virtual VOID			SetExtraInfo( CHAR* pstrIn ){};

	// ��ϸ����(������Ҫ������)
	virtual LPCTSTR			GetExtraDesc(VOID);

	// �õ����ʹ�������Ʒ��Ҫ�ĵȼ�
	virtual INT				GetNeedLevel()  { return  m_theBaseDef ->nNeedLevel; }


    /////////////////////////////////////  CObject_Item_Mount��չ����  /////////////////////////////////////

	// �õ�����mount_att������ ID
	INT	        	        GetMountAttID() { return m_theBaseDef->nID;}

	// �õ�����char_mount���� ID
	INT		                GetMountCharID() { return m_theBaseDef->nCharMountID;}

	// �õ�����ģ�� ID
	INT		                GetMountModelID(){ return m_characterData->m_nModelID;}

    // �õ����ӵ��ٶ�
	INT                     GetMountAddtionSpeed() { return m_theBaseDef->nAddtionSpeed; }

	// �õ�����ʱ��
	INT                     GetMountLife() { return m_theBaseDef->nLife; }

	// �õ�������
	MOUNT_BIND_TYPE         GetMountBindType() { return (MOUNT_BIND_TYPE) m_theBaseDef->nBindType; }

	// �õ��Ƿ��Ѱ�
	BOOL                    GetMountIsBinded() { return m_bIsBinded; }

	// �����Ƿ��Ѱ�
	VOID                    SetMountIsBinded(BOOL bBind) { m_bIsBinded = bBind; }

	// �õ����ʱ��
	UINT                    GetMountObtainDate() { return m_nObtainDate; }

	// ���û��ʱ��
	VOID                    SetMountObtainDate(UINT nDate) { m_nObtainDate = nDate; }

	// �õ��Ƿ��ѵ���
	BOOL                    GetMountIsDue() { return m_bIsDue; }

	// �����Ƿ��ѵ���
	VOID                    SetMountIsDue(BOOL bDue) { m_bIsDue = bDue; }

	// �õ�ʣ��ʹ��ʱ��
	UINT                    GetMountRemainTime() { return m_nRemainTime; }

	// ����ʣ��ʹ��ʱ��
	VOID                    SetMountRemainTime(UINT nTime) { m_nRemainTime = nTime; }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////

public:
	                        CObject_Item_Mount(INT id);
	virtual                ~CObject_Item_Mount();

	// ��������
	VOID					AsMount(const _DBC_ITEM_MOUNT_ATT* pMedicDefine);

protected:

	_DBC_ITEM_MOUNT_ATT*	      m_theBaseDef;			// �������Զ���(���ݱ�)
	const _DBC_CHARACTER_MOUNT*   m_characterData;        // ��ʾ��������(���ݱ�)
	BOOL                          m_bIsBinded;            // ��ǰ�Ƿ��Ѱ�
	UINT                          m_nObtainDate;          // ��ȡ����ʱ��ʱ��
	UINT                          m_nRemainTime;          // ʣ��ʹ��ʱ��
	BOOL                          m_bIsDue;               // �Ƿ��ѵ���
	STRING					      m_strExtraDesc;
};
