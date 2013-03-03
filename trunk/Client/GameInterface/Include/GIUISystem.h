
/**	GIUISystem.h
 */

#pragma once


#include "GIBasicType.h"
#include "GINode.h"
#include "GIMath.h"
#include "GITransferItemSystem.h"



/// 聊天模板的最大索引量
enum
{
	TALK_TEMPLATE_NOBODY = 0,
	TALK_TEMPLATE_OTHER,
	TALK_TEMPLATE_MYSELF,

	TALK_TEMPLATE_MAX
};

// 过滤类型
enum FILTER_TYPE
{
	FT_NONE = 0,	// 无类型
	FT_CHAT,		// 聊天
	FT_ROLE,		// 创建人物
	FT_NAME,		// 命名
	FT_STALL,		// 摆摊

	FT_MAX,
};

// 过滤等级
enum FILTER_RANK
{
	FR_NO_MATCH = 0,	// 忽略
	FR_PART_MATCH,		// 部分匹配
	FR_FULL_MATCH,		// 完全匹配

	FR_MAX,
};

#define TALK_HELPSTRING_MAX		200
#define TALK_HELPSTRING_KEY		"help"


/** 角色头顶上的信息板
 */
class KYLIN_API tCreatureBoard
{
public:

	/** 删除自身 */
	virtual VOID			Destroy(VOID)	= 0;
	/** 设置在屏幕上的坐标(像素) */
	virtual VOID			SetPosition(FLOAT nX, FLOAT nY) = 0;
	/** 显示/隐藏 */
	virtual VOID			Show(BOOL bShow) = 0;
	/** 显示/隐藏玩家属性（血条和等级） */
	virtual VOID			ShowCharAttr(BOOL bShow) = 0;
	/** 显示/隐藏玩家称号 */
	virtual VOID			ShowCharTitle(BOOL bShow, INT nType) = 0;

	/** 设置名字 */
	virtual VOID			SetElement_Name(LPCTSTR szName)				= 0;
	/** 设置生命值 */
	virtual VOID			SetElement_HPProgress( INT hp, INT maxHp )	= 0;
	virtual	VOID			SetElement_HPProgress( FLOAT hpPercent )	= 0;
	/** 设置等级 */
	virtual VOID			SetElement_Level( INT nLevel, INT nType )			= 0;
	/** 设置称号 */
	virtual VOID			SetElement_Title(LPCTSTR szName, INT nType = 0)		= 0;
	/** 设置国家标记 */
	virtual VOID			SetElement_CountryFlag(INT nCountry)	= 0;
	/** 设置队长标记 */
	virtual VOID			SetElement_LeaderFlag( BOOL bShowj, INT nType )		= 0;
	/** 设置 pk标记 | 任务提示 */
	virtual VOID			SetElement_PKFlag( INT nType, INT nState, BOOL bShow ) = 0;
	/** 设置摆摊上的文字 */
	virtual VOID			SetElement_SaleText(LPCTSTR szSaleText) = 0;
	/** 设置是否显示摆摊信息 */
	virtual VOID			SetElement_SaleSign(BOOL bShow)			= 0;
	/** 设置逻辑对象ID */
	virtual VOID			SetElement_ObjId(INT nObjId)			= 0;
	// 设置玩家的状态（正常、死亡）
	virtual VOID			SetInfoState( int nState )				= 0;
	// 设置信息离主角的位置
	virtual void			SetInfoDistance( float nDistance )		= 0;
	// 设置玩家头上聊天框里的内容
	virtual VOID			SetElement_PaoPaoText( LPCTSTR szPaoPao ) = 0;
	// 得到屏幕位置
	virtual fVector2		GetPosition(void) const = 0;
	// 是否显示
	virtual BOOL			isShow() = 0;

	// 显示摆摊状态的信息板		20100527 BLL
	virtual VOID				ShowIsInStall( BOOL bIsMyself ) = 0;

};


/** UI系统虚接口
 */
class KYLIN_API tUISystem : public tNode
{
public:

	// 窗口大小发生改变
	virtual VOID			OnSizeChange(UINT message, WPARAM wParam, LPARAM lParam)	= 0;
	// 处理输入,如果返回TRUE，则表示ui截获了输入
	virtual VOID			InjectInput(VOID) = 0;
	// 文字输入消息
	virtual BOOL			MessageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;
	//鼠标是否在ui上空
	virtual BOOL			IsMouseHover(VOID) = 0;
	//鼠标是否在聊天信息上空
	virtual BOOL			IsMouseHover_ChatBoard(LONG x, LONG y) = 0;
	//获得聊天信息里的超链中的信息
	virtual STRING			getChatBoard_HyplinkContex(LONG x, LONG y) = 0;
	//创建一个新的角色信息板
	virtual tCreatureBoard*	CreateCreatureBoard(VOID) = 0;
	virtual VOID			AddNewBeHitBoard( bool bDouble, LPCTSTR szInfo, FLOAT nStartX, FLOAT nStartY, int nType = 0 ,int nMoveType = 0) = 0;
	//某一组UI是否处于打开中
	virtual BOOL			IsWindowShow(LPCTSTR szUIName, LPCTSTR szChildName = NULL) = 0;
	virtual BOOL			CloseAllWindow() = 0;
	//通知UI创建一组UI显示模型
	virtual VOID			FakeObj_Show(LPCTSTR szWinName, LPCTSTR szName) = 0;
	//通知UI销毁一组UI显示模型
	virtual VOID			FakeObj_Close(LPCTSTR szWinName, LPCTSTR szName) = 0;

	//--- FOR DEBUG
	virtual VOID		Debug_PushChatString(LPCTSTR szName, LPCTSTR szMsg)	= 0;
	virtual VOID		Debug_SaveFontTexture(VOID) = 0;
	//--- FOR DEBUG

	virtual VOID			ChangeDragFlag(bool Flag) = 0;
	virtual BOOL			IsDragFlag(VOID) = 0;

	//逻辑层添加系统通知信息
	virtual STRING			ParserString_VarParam(LPCTSTR strID, ... ) = 0;
	virtual VOID			ParserString_NoColor(const STRING& strSrc, STRING& strOut, BOOL bControl = FALSE) = 0;
	virtual STRING			ParserString_NoColor_VarParam(LPCTSTR strID, ... ) = 0;
	virtual STRING			ParserString_NoVarParam(LPCTSTR strID) = 0;
	virtual STRING			ParserString_NoColor_NoVarParam(LPCTSTR strID) = 0;

	//聊天模板字串的取得
	virtual STRING			getTalkTemplate(const STRING& strKey, UINT index) = 0;
	virtual STRING			ModifyChatTemplate(const STRING& strTemp, const STRING& strTalker, const STRING& strTarget) = 0;

	virtual STRING			getTalkRandHelpMsg() = 0;
	//查看当前获取输入的EditBox
	virtual BOOL			IsIMEEditboxActive(LPCTSTR szWinName) = 0;
	//输入法状态
	virtual INT				GetCurrentIMEState() = 0;
	//获取EditBox的文字
	virtual STRING			getIMEEditboxString(LPCTSTR szWinName) =0;


	//将一个物品信息传入
	virtual bool			InjectItemInfo(tTransferItemSystem::Element* pElement) = 0;

	virtual void			OnPushEcsape() = 0;
	virtual void			SetPaoPaoActive( bool bActive ) = 0;
	virtual bool			IsPaoPaoActive() = 0;

//	virtual UINT			OgreColor_CEGUIColor_Limit_Lumination( UINT OgreColor,FLOAT fLumination) = 0;

	virtual UINT			Lumination_OgreColor( UINT OgreColor, INT Lumination ) = 0;
	virtual INT				OgreColor_Lumination( UINT OgreColor ) = 0;

	//过滤字符串中的非法敏感字符
	virtual BOOL	CheckStringFilter(const STRING& strIn, const FILTER_TYPE filterType = FT_NONE) = 0;
	virtual BOOL	CheckStringCode(const STRING& strIn, STRING& strOut) = 0;
	//完全匹配过滤
	virtual BOOL	CheckStringFullCompare(const STRING& strIn, const STRING& strFilterType = "all", BOOL bUseAllTable = TRUE) = 0;

	//获取当前的鼠标下面的窗口
	virtual STRING	GetDebugString(VOID) = 0;
	

	//获取当前的鼠标下面的窗口
	virtual void	OpenWindow(const STRING& winName) = 0;
	virtual void	CloseWindow(const STRING& winName) = 0;
	virtual void	ToggleWindow(const STRING& winName) = 0;

	virtual bool	HasInputFocus() = 0;

	//添加聊天信息（提高显示聊天信息的效率）
	virtual bool			addChatHistoryMessage(INT msgID,LPCTSTR szWinName, LPCTSTR szMessage, INT nOp = -1, UINT nDisappearTime = 0) =0;
	virtual VOID		ReplaceStringUseFilter(const STRING& strIn, STRING& strOut, FILTER_TYPE filterType = FT_CHAT) =0;
	virtual std::string		string_check_valid(const char* szString) = 0;

	virtual bool	ReloadWindowScript( const STRING& windowName ) =0;

	virtual VOID	UIRenderToggleShow(VOID) =0;
	
	KL_DECLARE_DYNAMIC(tUISystem);

};