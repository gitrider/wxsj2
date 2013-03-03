
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
	//系统操作
	//------------------------------------------------
	// 物体操作系列
	
	//将某个物体选择为主要目标
	virtual VOID	Object_SelectAsMainTarget(INT idObj, int iType = 0);
	//显示右键菜单
	virtual VOID	Object_ShowContexMenu(INT idObj,BOOL showMyself = FALSE);

	//------------------------------------------------
	// 玩家

	//移动到场景中某个位置
	virtual VOID	Player_MoveTo(const fVector2&  fvAimPos);
	virtual	VOID	Player_MoveTo( INT nSceneID, const fVector2&  fvAimPos );

	/** 使用技能(瞬发) */
	virtual VOID	Player_UseSkill( INT idSkill);
	/** 使用技能(对某个Obj) */
	virtual VOID	Player_UseSkill( INT idSkill, INT idObj);
	/** 使用技能(对某个范围) */
	virtual VOID	Player_UseSkill( INT idSkill, const fVector2& fvPos);
	/** 使用技能(对某个方向) */
	virtual VOID	Player_UseSkill( INT idSkill, FLOAT fDir);
	virtual VOID	Player_UseSkill( INT idSkill, const fVector2& fvPos, const GUID_t& fDir );
	/** 使用技能(对某个队友) */
	virtual VOID	Player_UseSkill( INT idSkill, const GUID_t& guid);

	//合成某配方()
	virtual VOID	Player_UseLifeAbility( INT idPrescription, INT nMakeCount );
	virtual VOID	Player_UseLifeAbility( INT idPrescription );
	//销毁身上的装备
	virtual VOID	Player_DestroyEquip(const tObject_Item* pEquip);
	//卸载身上的装备
	virtual VOID	Player_UnEquip(const tObject_Item* pEquip);
	//请求升级
	virtual VOID	Player_AskLevelUp(VOID);
	//请求洗一级属性点
	virtual VOID	Player_AskCleanBat1Point(VOID);
	//谈话(对某个NPC)
	virtual VOID	Player_Speak(INT idObj);

	// 20100416 AddCodeBegin
	//如果是超链接自动寻路的NPC，则保存自动寻路所需的属性
	virtual	VOID	SetAutoPathNPC( INT nSceneID, STRING strNPCName );
	// 20100416 AddCodeEnd

	//------------------------------------------------
	//试图对某个TripperObj进行操作，如果距离过远，会走过去
	virtual VOID	TripperObj_Active( UINT nId );
	//试图捡起当前盒子里的第几项
	virtual VOID	ItemBox_PickItem(tObject_Item* pItem);

	//------------------------------------------------
	//使用包裹里的物品_通用
	virtual VOID	PacketItem_UserItem(tActionItem* pActionItem, int nTargetServerID, fVector2& fvPos);
	//使用包裹里的物品_对象
	virtual VOID	PacketItem_UserItem_ToTarget( INT nIndex, UINT nTargetID);
	//使用包裹里的物品_装备
	virtual VOID	PacketItem_UserEquip( INT nIndex );
	//使用包裹里的物品_宝石
	virtual VOID	PacketItem_UserGem(INT nGemIndex, INT EquipIndex);
	//销毁包裹里的物品
	virtual VOID	PacketItem_Destroy(INT nIndex);

	//一键换装 保存一套装备
	virtual VOID	PacketItem_SaveSuit(INT nIndex);

	//给包裹里的装备打孔
	virtual VOID	PacketItem_AddHole( );

	//-------------------------------------------------
	//装备坐骑  [7/8/2010 陈军龙]
	virtual VOID    PacketItem_EquipMount(INT nIndex);	
	//召唤坐骑  [7/8/2010 陈军龙]
    virtual VOID    PacketItem_CallMount(INT nIndex);
	//召回坐骑  [7/8/2010 陈军龙]
	virtual VOID    PacketItem_RecallMount(INT nIndex);
	//丢弃坐骑  [7/8/2010 陈军龙]
	virtual	VOID    PacketItem_DiscardMount(INT nIndex);
	//销毁坐骑  [7/8/2010 陈军龙]
	virtual	VOID    PacketItem_DestroyMount( INT nIndex );

	//-------------------------------------------------
	////给玩家身上的装备打孔

	virtual VOID	HumanEquip_AddHole( );



	//给包裹里的装备绑定 
	virtual VOID	PacketItem_BangDing();


	//给身上的装备绑定
	virtual VOID	HumanEquip_BangDing() ;

   //合成装备
	virtual VOID     EquipSynthesize( INT ProId, INT *Grad);

	virtual VOID     EquipRework(INT  *Grad)	; 
	//------------------------------------------------



	//------------------------------------------------
	//发送GM指令
	virtual VOID	GM_SendCommand( LPCSTR strCommand );

	virtual VOID    SendChatCommand(INT nChannelID, INT nChatType, INT nTeamID, LPCTSTR szContex, LPCTSTR szTargetName);

	//------------------------------------------------
	//场景

	//设置全局渲染器
	virtual VOID	Scene_SetPostFilter(LPCTSTR szFilterName);
	//设置LightMap
	virtual VOID	Scene_SetLightmapQuality(INT nQuality);

	//------------------------------------------------
	//设置当前激活技能
	virtual VOID			Skill_SetActive(tActionItem* pActiveSkill);
	virtual tActionItem*	Skill_GetActive(VOID);

	//恢复到缺省技能操作，用于退出修理，鉴定，等特殊鼠标命令状态
	virtual VOID			Skill_CancelAction(VOID);

	//------------------------------------------------
	//购买物品
	virtual VOID			Booth_BuyItem( tObject_Item* pItem );
	//卖出商品
	virtual VOID			Booth_SellItem(tObject_Item* pItem );
	//完成交易
	virtual VOID			Booth_Close(VOID);
	//修理
	virtual VOID			Booth_Repair(BOOL bAll, INT bagindex, BOOL isInBag = TRUE);
	//进入修理状态
	virtual VOID			Booth_EnterRepair(VOID);

	//-------------------------------------------------
	//更新称号
	virtual VOID			Title_UpdateCurTitle(BYTE titleType, SHORT titleData);

	virtual VOID			Stall_OpenStallSale(INT nObjId);

	//------------------------------------------------
	//阵营判断
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
	//核心继承
	//------------------------------------------------
	virtual VOID	Initial(VOID*);
	virtual VOID	Tick(VOID);
	virtual VOID	Release(VOID);


	KL_DECLARE_DYNAMIC(CGameInterface);

};