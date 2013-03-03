
/**
 */

#pragma once


#include "GINode.h"
#include "GIMath.h"
#include "../../../Common/GameDefine2.h"


/** GameINTerface��
 *	������Ϸ���ⲿ���ƽӿ��࣬���е��ⲿģ����Ҫͨ������ӿ���������Ϸ�����ݺͿ���
*/

class tActionItem;
class tObject_Item;
class CObject;
enum HUMAN_EQUIP;

#ifndef GUID_t
typedef UINT	GUID_t;
#endif

class KYLIN_API tGameInterfaceBase : public tNode
{
public:

	//------------------------------------------------
	// �������ϵ��
	
	//��ĳ������ѡ��Ϊ��ҪĿ��
	virtual VOID	Object_SelectAsMainTarget(INT idObj, int iType = 0)	= 0;
	//��ʾ�Ҽ��˵�
	virtual VOID	Object_ShowContexMenu(INT idObj, BOOL showMyself = FALSE) = 0;

	///------------------------------------------------
	// ���ǲ���ϵ��

	//�ƶ���������ĳ��λ��
	virtual VOID Player_MoveTo(const fVector2&  fvAimPos) = 0;
	virtual	VOID Player_MoveTo( INT nSceneID, const fVector2&  fvAimPos ) =0;
	//ʹ�ü���(˲��)
	virtual VOID	Player_UseSkill( INT idSkill)	= 0;
	//ʹ�ü���(��ĳ��Obj)
	virtual VOID	Player_UseSkill( INT idSkill, INT idObj) = 0;
	//ʹ�ü���(��ĳ����Χ)
	virtual VOID	Player_UseSkill( INT idSkill, const fVector2& fvPos) = 0;
	//ʹ�ü���(��ĳ������)
	virtual VOID	Player_UseSkill( INT idSkill, FLOAT fDir )	= 0;
	//ʹ�ü���(��ĳ������(GUID))
	virtual VOID	Player_UseSkill( INT idSkill, const GUID_t& fDir )	= 0;
	virtual VOID	Player_UseSkill( INT idSkill, const fVector2& fvPos, const GUID_t& fDir )	= 0;
	//�ϳ�ĳ�䷽()
	virtual VOID	Player_UseLifeAbility( INT idPrescription, INT nMakeCount )	= 0;
	virtual VOID	Player_UseLifeAbility( INT idPrescription )	= 0;
	//�������ϵ�װ��
	virtual VOID	Player_DestroyEquip(const tObject_Item* pEquip)  = 0;
	//ж�����ϵ�װ��
	virtual VOID	Player_UnEquip(const tObject_Item* pEquip)  = 0;
	//��������
	virtual VOID	Player_AskLevelUp(VOID)						= 0;
	//����ϴһ�����Ե�
	virtual VOID	Player_AskCleanBat1Point(VOID)				= 0;
	//̸��(��ĳ��NPC)
	virtual VOID	Player_Speak(INT idObj)					= 0;

	// 20100419 AddCodeBegin
	//������Զ�Ѱ·��NPC���򱣴��Զ�Ѱ·����
	virtual	VOID	SetAutoPathNPC( INT nSceneID , STRING strNPCName ) = 0;
	// 20100419 AddCodeEnd

	//------------------------------------------------
	//��ͼ��ĳ��TripperObj���в�������������Զ�����߹�ȥ
	virtual VOID	TripperObj_Active( UINT nId )				= 0;
	//��ͼ����ǰ������ĵڼ���
	virtual VOID	ItemBox_PickItem(tObject_Item* pItem)		= 0;


	//------------------------------------------------
	//ʹ�ð��������Ʒ_ͨ��
	virtual VOID	PacketItem_UserItem(tActionItem* pActionItem, int nTargetServerID, fVector2& fvPos) = 0;
	//ʹ�ð��������Ʒ_����
	virtual VOID	PacketItem_UserItem_ToTarget( INT nIndex, UINT nTargetID)	= 0;
	//ʹ�ð��������Ʒ_װ��
	virtual VOID	PacketItem_UserEquip( INT nIndex )					= 0;
	//ʹ�ð��������Ʒ_��ʯ
	virtual VOID	PacketItem_UserGem(INT nGemIndex, INT EquipIndex)	= 0;
	//���ٰ��������Ʒ
	virtual VOID	PacketItem_Destroy(INT nIndex)						= 0;


	//���������װ�����
	virtual VOID	PacketItem_AddHole()						= 0;

	//------------------------------------------------
	//װ������  [7/8/2010 �¾���]
	virtual VOID    PacketItem_EquipMount(INT nIndex)           = 0;
	//�ٻ�����  [7/8/2010 �¾���]
	virtual VOID    PacketItem_CallMount(INT nIndex)            = 0;
	//�ٻ�����  [7/8/2010 �¾���]
	virtual VOID    PacketItem_RecallMount(INT nIndex)          = 0;
	//��������  [7/8/2010 �¾���]
	virtual	VOID    PacketItem_DiscardMount(INT nIndex)         = 0;
	//��������  [7/8/2010 �¾���]
	virtual	VOID    PacketItem_DestroyMount( INT nIndex )       = 0;


	//��������ϵ�װ�����

	virtual VOID	HumanEquip_AddHole()					= 0;



	//���������װ���� 
	virtual VOID	PacketItem_BangDing()						= 0 ;


	//�����ϵ�װ���� 
	virtual VOID	HumanEquip_BangDing()						= 0;

    //�ϳ�װ��
	virtual VOID    EquipSynthesize( INT ProId, INT *Grad)               = 0;

    //����װ��
	virtual VOID    EquipRework(INT  *Grad)				= 0;	 
	//------------------------------------------------
	//����GMָ��
	virtual VOID	GM_SendCommand( LPCSTR strCommand )			= 0;
	virtual VOID    SendChatCommand(INT nChannelID, INT nChatType, INT nTeamID, LPCTSTR szContex, LPCTSTR szTargetName)							= 0;	

	//------------------------------------------------
	//����
	virtual VOID	Scene_SetPostFilter(LPCTSTR szFilterName)	= 0;
	virtual VOID	Scene_SetLightmapQuality(INT nQuality)		= 0;

	//------------------------------------------------
	//���õ�ǰ�����
	virtual VOID			Skill_SetActive(tActionItem* pActiveSkill)	= 0;
	virtual tActionItem*	Skill_GetActive(VOID)				= 0;
	//�ָ���ȱʡ���ܲ����������˳������������������������״̬
	virtual VOID			Skill_CancelAction(VOID) = 0;

	//------------------------------------------------
	//������Ʒ
	virtual VOID			Booth_BuyItem( tObject_Item* pItem ) = 0;
	//������Ʒ
	virtual VOID			Booth_SellItem(tObject_Item* pItem ) = 0;
	//��ɽ���
	virtual VOID			Booth_Close(VOID)					 = 0;
	//����
	virtual VOID			Booth_Repair(BOOL bAll, INT bagindex, BOOL isInBag = TRUE) = 0;
	//��������״̬
	virtual VOID			Booth_EnterRepair(VOID) = 0;
	 
	//-------------------------------------------------
	//���³ƺ�
	virtual VOID			Title_UpdateCurTitle(BYTE titleType, SHORT titleData)	 = 0;

	virtual VOID			Stall_OpenStallSale(INT nObjId)		 = 0;
	//------------------------------------------------
	//��Ӫ�ж�
	virtual ENUM_RELATION	GetCampType( CObject *pObj_A, CObject *pObj_B ) = 0;

protected:

	KL_DECLARE_DYNAMIC(tGameInterfaceBase);

};
