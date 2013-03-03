
/**	GMGameInterface.h
 */

#pragma once


#include "GIGameInterface.h"
#include "type.h"
#include "DataPool/DetailAttrib_Player.h"
#include "Object/Logic/Item/Obj_Item.h"
#include "World\ShortestPath.h"


class CObject_ProjTex_MouseTarget;
class tActionItem;

class CGameInterface : public tGameInterfaceBase
{
public:

	//------------------------------------------------
	//ϵͳ����
	//------------------------------------------------
	// �������ϵ��
	
	//��ĳ������ѡ��Ϊ��ҪĿ��
	virtual VOID	Object_SelectAsMainTarget(INT idObj, int iType = 0);
	//��ʾ�Ҽ��˵�
	virtual VOID	Object_ShowContexMenu(INT idObj,BOOL showMyself = FALSE);

	//------------------------------------------------
	// ���

	//�ƶ���������ĳ��λ��
	virtual VOID	Player_MoveTo(const fVector2&  fvAimPos);
	virtual	VOID	Player_MoveTo( INT nSceneID, const fVector2&  fvAimPos );

	/** ʹ�ü���(˲��) */
	virtual VOID	Player_UseSkill( INT idSkill);
	/** ʹ�ü���(��ĳ��Obj) */
	virtual VOID	Player_UseSkill( INT idSkill, INT idObj);
	/** ʹ�ü���(��ĳ����Χ) */
	virtual VOID	Player_UseSkill( INT idSkill, const fVector2& fvPos);
	/** ʹ�ü���(��ĳ������) */
	virtual VOID	Player_UseSkill( INT idSkill, FLOAT fDir);
	virtual VOID	Player_UseSkill( INT idSkill, const fVector2& fvPos, const GUID_t& fDir );
	/** ʹ�ü���(��ĳ������) */
	virtual VOID	Player_UseSkill( INT idSkill, const GUID_t& guid);

	//�ϳ�ĳ�䷽()
	virtual VOID	Player_UseLifeAbility( INT idPrescription, INT nMakeCount );
	virtual VOID	Player_UseLifeAbility( INT idPrescription );
	//�������ϵ�װ��
	virtual VOID	Player_DestroyEquip(const tObject_Item* pEquip);
	//ж�����ϵ�װ��
	virtual VOID	Player_UnEquip(const tObject_Item* pEquip);
	//��������
	virtual VOID	Player_AskLevelUp(VOID);
	//����ϴһ�����Ե�
	virtual VOID	Player_AskCleanBat1Point(VOID);
	//̸��(��ĳ��NPC)
	virtual VOID	Player_Speak(INT idObj);

	// 20100416 AddCodeBegin
	//����ǳ������Զ�Ѱ·��NPC���򱣴��Զ�Ѱ·���������
	virtual	VOID	SetAutoPathNPC( INT nSceneID, STRING strNPCName );
	// 20100416 AddCodeEnd

	//------------------------------------------------
	//��ͼ��ĳ��TripperObj���в�������������Զ�����߹�ȥ
	virtual VOID	TripperObj_Active( UINT nId );
	//��ͼ����ǰ������ĵڼ���
	virtual VOID	ItemBox_PickItem(tObject_Item* pItem);

	//------------------------------------------------
	//ʹ�ð��������Ʒ_ͨ��
	virtual VOID	PacketItem_UserItem(tActionItem* pActionItem, int nTargetServerID, fVector2& fvPos);
	//ʹ�ð��������Ʒ_����
	virtual VOID	PacketItem_UserItem_ToTarget( INT nIndex, UINT nTargetID);
	//ʹ�ð��������Ʒ_װ��
	virtual VOID	PacketItem_UserEquip( INT nIndex );
	//ʹ�ð��������Ʒ_��ʯ
	virtual VOID	PacketItem_UserGem(INT nGemIndex, INT EquipIndex);
	//���ٰ��������Ʒ
	virtual VOID	PacketItem_Destroy(INT nIndex);

	//һ����װ ����һ��װ��
	virtual VOID	PacketItem_SaveSuit(INT nIndex);

	//���������װ�����
	virtual VOID	PacketItem_AddHole( );

	//-------------------------------------------------
	//װ������  [7/8/2010 �¾���]
	virtual VOID    PacketItem_EquipMount(INT nIndex);	
	//�ٻ�����  [7/8/2010 �¾���]
    virtual VOID    PacketItem_CallMount(INT nIndex);
	//�ٻ�����  [7/8/2010 �¾���]
	virtual VOID    PacketItem_RecallMount(INT nIndex);
	//��������  [7/8/2010 �¾���]
	virtual	VOID    PacketItem_DiscardMount(INT nIndex);
	//��������  [7/8/2010 �¾���]
	virtual	VOID    PacketItem_DestroyMount( INT nIndex );

	//-------------------------------------------------
	////��������ϵ�װ�����

	virtual VOID	HumanEquip_AddHole( );



	//���������װ���� 
	virtual VOID	PacketItem_BangDing();


	//�����ϵ�װ����
	virtual VOID	HumanEquip_BangDing() ;

   //�ϳ�װ��
	virtual VOID     EquipSynthesize( INT ProId, INT *Grad);

	virtual VOID     EquipRework(INT  *Grad)	; 
	//------------------------------------------------



	//------------------------------------------------
	//����GMָ��
	virtual VOID	GM_SendCommand( LPCSTR strCommand );

	virtual VOID    SendChatCommand(INT nChannelID, INT nChatType, INT nTeamID, LPCTSTR szContex, LPCTSTR szTargetName);

	//------------------------------------------------
	//����

	//����ȫ����Ⱦ��
	virtual VOID	Scene_SetPostFilter(LPCTSTR szFilterName);
	//����LightMap
	virtual VOID	Scene_SetLightmapQuality(INT nQuality);

	//------------------------------------------------
	//���õ�ǰ�����
	virtual VOID			Skill_SetActive(tActionItem* pActiveSkill);
	virtual tActionItem*	Skill_GetActive(VOID);

	//�ָ���ȱʡ���ܲ����������˳������������������������״̬
	virtual VOID			Skill_CancelAction(VOID);

	//------------------------------------------------
	//������Ʒ
	virtual VOID			Booth_BuyItem( tObject_Item* pItem );
	//������Ʒ
	virtual VOID			Booth_SellItem(tObject_Item* pItem );
	//��ɽ���
	virtual VOID			Booth_Close(VOID);
	//����
	virtual VOID			Booth_Repair(BOOL bAll, INT bagindex, BOOL isInBag = TRUE);
	//��������״̬
	virtual VOID			Booth_EnterRepair(VOID);

	//-------------------------------------------------
	//���³ƺ�
	virtual VOID			Title_UpdateCurTitle(BYTE titleType, SHORT titleData);

	virtual VOID			Stall_OpenStallSale(INT nObjId);

	//------------------------------------------------
	//��Ӫ�ж�
	virtual ENUM_RELATION	GetCampType( CObject *pObj_A, CObject *pObj_B );

protected:

	static CGameInterface* s_pMe;

	INT		Make_Count;
	INT		Make_Prescription;

public:
	struct SceneFindPathStatus 
	{
		FindResult	vFindResult;
		INT	nCurrentPos;
		fVector2 vDestPos;
		INT	nDestScene;
		BOOL bUseFlag;

		VOID SetValue(INT DestScene, const fVector2& DestPos, const FindResult & findResult)
		{
			nCurrentPos = 0;
			nDestScene = DestScene;
			vDestPos = DestPos;
			vFindResult = findResult;
			bUseFlag = TRUE;
		}

		SceneFindPathStatus(VOID)
		{
			nCurrentPos = 0;
			nDestScene = -1;
			bUseFlag = FALSE;
		};
		fVector2		GetNextPosition(VOID)
		{
			if (nCurrentPos < vFindResult.size())
			{
				fVector2  ret( vFindResult[nCurrentPos].xPos, vFindResult[nCurrentPos].yPos );
				++nCurrentPos;
				return ret;
			}
			if (bUseFlag)
			{
				bUseFlag = FALSE;
				return vDestPos;
			}
			return fVector2(-1,-1);
		}
		BOOL IsUse(VOID) { return bUseFlag; }
	};

	VOID CGameInterface::CheckAutoMove( VOID );

protected:
	SceneFindPathStatus	m_SceneFindPathStatus;

public:

	CGameInterface();
	virtual ~CGameInterface();
	static CGameInterface*	GetMe(VOID) { return s_pMe; }

	//------------------------------------------------
	//���ļ̳�
	//------------------------------------------------
	virtual VOID	Initial(VOID*);
	virtual VOID	Tick(VOID);
	virtual VOID	Release(VOID);


	KL_DECLARE_DYNAMIC(CGameInterface);

};