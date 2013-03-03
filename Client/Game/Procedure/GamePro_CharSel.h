
/** 人物选择流程
 */

#pragma once


#include "GameProcedure.h"
#include "LoginDataStructDef.h"
#include "Object\Logic\UIObj\UIModel.h"		// 界面上显示的模型



#define  MAX_SHOW_IN_UI		3


struct DB_CHAR_BASE_INFO;
class CObject;

class CGamePro_CharSel : public CGameProcedure
{
public:

	/// 登录状态
	enum PLAYER_CHARSEL_STATUS
	{
		CHARSEL_CHARSHOW,
		CHARSEL_SELDONE,
		CHARSEL_CONNNECT_GAME_SERVER_READY,	// 与游戏服务器的连接准备好, 可以连接.
	};


public:

	/// 切换流程是否清空界面。
	BOOL			m_bClearUIModel;

	/// 当前选中的要删除的角色
	int				m_iCurSelRoleDel;

	// 用户名
	std::string		m_strUserName;

	/// 当前得到的角色的个数
	int				m_iCharacterCount;

	/// 进入到游戏的guid
	GUID_t			m_EnterGameGUID;

	/// 在ui上显示的模型
	CModelShowInUI		m_Character[MAX_SHOW_IN_UI];	// 保存玩家已经创建的角色， 在人物选择界面使用
	DB_CHAR_BASE_INFO	m_CharacterInfo[MAX_SHOW_IN_UI];


public:

	CGamePro_CharSel(VOID);
	virtual ~CGamePro_CharSel(VOID);


	/** 清空角色数据 */
	void ClearUIModel();

	// 添加一个人物
	int AddCharacter(const DB_CHAR_BASE_INFO& CharacterInfo);

	// 切换到帐号输入界面
	void ChangeToAccountInput();

	// 切换到创建人物界面
	void ChangeToCreateRole();

	// 切换到服务器连接流程
	void ChangeToServerConnect();

	// 得到当前角色的个数
	int  GetCurRoleCount();

	// 通过索引得到角色的信息
	const DB_CHAR_BASE_INFO* GetRoleInfo(int iIndex);

	// 通过索引得到角色的UI 模型名字
	const TCHAR* GetRoleUIName(int iIndex);

	// 删除一个角色
	int DelRole(int iRoleIndex);

	// 删除选择的角色
	int DelSelRole();

	// 进入游戏
	int SendEnterGameMsg(int iRoleIndex);

	// 位置是否合法
	bool IsValidIndex(int iRoleIndex);

	// 设置当前要删除的角色
	void SetCurSelDel(int iIndex);


	// 设置用户名字
	void SetUserName(const char* pUserName); 

public:

	//处理输入
	virtual VOID	ProcessInput(VOID);

protected:
	PLAYER_CHARSEL_STATUS	m_Status;

	BOOL m_bIsLoadScene;

public:

	VOID SetStatus(PLAYER_CHARSEL_STATUS status);

	//主角在渲染层中的接口
//	CObject*		m_pObject;

protected:

	virtual VOID	Init(VOID);
	virtual VOID	Tick(VOID);
	virtual VOID	Render(VOID);
	virtual VOID	Release(VOID);
	virtual VOID	CloseRequest(VOID);



//616====== 测试
private:
		void InitUiModel(void);
		bool		m_bInitUiModel;

};
