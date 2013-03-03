
/** GamePro_Main.h
 *	
 *	功能：	游戏主流程
 *
 *	修改记录：
 *			080401	m_bShowOtherObj
 *					m_bShowOtherObjBoard
 */

#pragma once


#include "GameProcedure.h"
#include "GIMath.h"
#include "..\GameCommand.h"



namespace Fairy
{
class BulletFlowSystem;
};

class CObject;
class CObject_Character;
class CObject_ProjTex_MouseTarget;
class CObject_ProjTex_AuraDure;
struct _DBC_SKILL_DATA;
class tActionItem;
class tSoundSource;
struct InputEvent;

class CGamePro_Main : public CGameProcedure
{
public:

	VOID			SetSceneID(INT nSceneID) { m_SceneID = nSceneID; }

	//设置目的地指示环
	VOID			SetMouseTargetProjTex( const fVector2&  fvAimPos);

	//鼠标操作技能
	VOID			SetActiveSkill(tActionItem* pSkill) { m_pActiveSkill = pSkill; }
	tActionItem*	GetActiveSkill(VOID) { return m_pActiveSkill; }

	BOOL			IsAutoRun(VOID) { return m_bAutoRun; }
	CObject_ProjTex_MouseTarget* GetMouseTarget(void) { return m_pMouseTarget; }

public:

	//设置鼠标
	VOID			OnSetCursor(VOID);

protected:

	//当前进入的场景id
	INT					m_SceneID;
	//玩家自己
	CObject_Character*		m_pMySelf;
	//鼠标拖拽摄像机中
	BOOL					m_bCameraDraging;
	BOOL					m_bIsLastCameraDraging;
	
	//拖动行走中
	BOOL					m_bAutoRun;

protected:

	virtual VOID	Init(VOID);
	virtual VOID	Tick(VOID);
	virtual VOID	Render(VOID);
	virtual VOID	Release(VOID);
	virtual VOID	CloseRequest(VOID);

	virtual LRESULT	MainWndProc(HWND, UINT, WPARAM, LPARAM);

	// 处理键盘单键输入
	VOID	Handle_AccelerateKey(InputEvent& event);

	// 处理键盘组合键输入
	VOID	Handle_Combination_Key(InputEvent& event);

public:

	//处理输入
	virtual VOID	ProcessInput(VOID);
	//处理鼠标输入
			VOID	ProcessInput_Mouse(VOID);
	//处理键盘输入
			VOID	ProcessInput_Keybord(VOID);


	static int						m_nResetCamera;

private:

	//-------------------------------------------
	//当前激活的技能
	tActionItem*					m_pActiveSkill;

	//-------------------------------------------
	//技能范围指示环
	CObject_ProjTex_AuraDure*		m_pSkillArea;
	//鼠标目的地指示环
	CObject_ProjTex_MouseTarget*	m_pMouseTarget;

public:

	CGamePro_Main(VOID);
	virtual ~CGamePro_Main(VOID);

public:

	/// 记录是否已经初始化了
	BOOL			m_bInit;

	/// 是否显示除玩家外其他的对象
	BOOL			m_bShowOtherObj;
	/// 是否显示除玩家外其他对象的信息
	BOOL			m_bShowOtherObjBoard;


	Fairy::BulletFlowSystem* m_bulletFlow;

};
