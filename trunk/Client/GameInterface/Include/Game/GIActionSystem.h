
/**	GIActionSystem.h
 */

#pragma once


#include "GINode.h"


/*
|
|	���ﶨ����ǡ��������Ľӿڣ��������ܲ������԰�������Ʒ��ʹ�ò�����
|
*/

/// ����
enum ACTION_OPTYPE	
{
	AOT_EMPTY,					//-��-

	AOT_SKILL,					//ս������
	AOT_SKILL_LIFEABILITY,		//�����
	AOT_ITEM,					//��Ʒ
//	AOT_XINFA,					//�ķ�
	AOT_PET_SKILL,				//���＼��
	AOT_EQUIP,					//װ��
	AOT_CHATMOOD,				//���춯��
	AOT_MOUSECMD_REPARE,		//���ָ��_����
	AOT_MOUSECMD_IDENTIFY,		//���ָ��_����
	AOT_MOUSECMD_ADDFRIEND,		//���ָ��_��Ϊ����
	AOT_CHANGE_SUIT,			//һ����װ
	AOT_MOUSECMD_EXCHANGE,		//����
};


class tActionReference;

class KYLIN_API tActionItem
{
public:

	//--------------------------
	//�õ�id
	virtual INT				GetID(VOID) const			= 0;
	//�õ�����
	virtual LPCTSTR			GetName(VOID) const			= 0;
	//�õ�ͼ��
	virtual LPCTSTR			GetIconName(VOID) const		= 0;
	
	virtual VOID			SetIconName(STRING& name)	= 0;
	//����Check״̬
	virtual VOID			SetCheckState(BOOL bCheck)	= 0;
	//�������
	virtual VOID			AddReference(tActionReference* pRef,BOOL bIsInMenuToolbar=FALSE)	= 0;
	//�Ͽ�����
	virtual VOID			RemoveRefrence(tActionReference* pRef)	= 0;

	virtual ACTION_OPTYPE	GetType(VOID) const		= 0;
	//�����ַ���
	virtual	LPCTSTR			GetType_String(VOID)	= 0;
	
	//�õ�����ID
	/*
	|	����ս������, �Ǽ��ܱ��е�ID (DBC_SKILL_DATA)
	|	��������ܣ�������ܱ��е�ID(DBC_LIFEABILITY_DEFINE)
	|	������Ʒ������Ʒ���е�ID(DBC_ITEM_*)
	|	�����ķ������ķ����е�ID(DBC_SKILL_XINFA)
	*/
	virtual INT				GetDefineID(VOID)		= 0;
	//�õ�����
	virtual INT				GetNum(VOID)			= 0;
	//�õ��ڲ�����
	virtual VOID*			GetImpl(VOID)			= 0;
	//�õ�����
	virtual LPCTSTR			GetDesc(VOID)			= 0;
	//�õ���ȴ״��ID
	virtual INT				GetCoolDownID(VOID)		= 0;
	//�õ���������������
	virtual INT				GetPosIndex(VOID)		= 0;
	//�����
	virtual VOID			DoAction(VOID)			= 0;
	//�����(������)
	virtual VOID			DoSubAction(VOID)		= 0;
	//�Ƿ���Ч
	virtual BOOL			IsValidate(VOID)		= 0;
	// Is Enabled
	virtual BOOL			IsEnabled(VOID) const	= 0;
	//Enable
	virtual VOID			Enable(VOID)			= 0;
	//Disable
	virtual VOID			Disable(VOID)			= 0;	
	//�����ȴ�Ƿ����
	virtual BOOL			CoolDownIsOver(VOID)	= 0;
	//�϶�����
	virtual VOID			NotifyDragDropDragged(BOOL bDestory, LPCSTR szTargetName, LPCSTR szSourceName) = 0;
	//��ʾtooltips
	virtual VOID			NotifyTooltipsShow(int btn_left, int btn_top, int btn_right, int btn_bottom) = 0;
	//����tooltips
	virtual VOID			NotifyTooltipsHide(VOID) = 0;
	//��ѯ�߼�����
	virtual STRING			GetAttributeValue(LPCTSTR szAttributeName) = 0;

};


/** UI����
 */
class KYLIN_API tActionReference
{
public:

	//�߼�Action��ʧ
	virtual VOID			BeDestroyed(VOID) = 0;
	//���ݸ���
	virtual VOID			Update(INT id) = 0;
	//��ť����
	virtual VOID			SetCheck(BOOL bCheck) = 0;
	//��ť���ó�default��״̬
	virtual VOID			SetDefault(BOOL bDefault) = 0;
	//������ȴ
	virtual VOID			EnterCoolDown(INT fTime, FLOAT fPercent) = 0;
	
	//��ʾ�ַ�, cChar����ʾ���ַ���0����ʾ
	enum CORNER_NUMBER_POS
	{
		ANP_TOPLEFT		= 0,
		ANP_TOPRIGHT,
		ANP_BOTLEFT,
		ANP_BOTRIGHT,
	};

	virtual VOID			SetCornerChar(CORNER_NUMBER_POS pos, LPCTSTR szChar) = 0;

	virtual	VOID			Disable(VOID) = 0;
	virtual	VOID			Enable(VOID) = 0;

};


class KYLIN_API tActionSystem : public tNode
{
public:

	//����idȡ��Action
	virtual tActionItem*	GetAction(INT id) = 0;
	//��ǰ������ʾTootips�İ�ť
	virtual tActionItem*	GetTooltipsFocus(void) = 0;
	virtual VOID			SaveAction(VOID) = 0;
	//�õ�ȱʡ����
	virtual tActionItem*	GetDefaultAction(VOID) = 0;
	//����ȱʡ����
	virtual VOID			SetDefaultAction(tActionItem* pDefAction) = 0;

	//���ÿ�ݼ�״̬ Nick 2008.1.12
	virtual VOID			SetActionSystemKeyState( INT keyState, BOOL isFKey ) = 0;

	KL_DECLARE_DYNAMIC( tActionSystem );

};
