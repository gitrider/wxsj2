#pragma once
#include "GINode.h"

//===================================================
//游戏系统配置变量管理器
#include "GIMath.h"

class KYLIN_API tVariableSystem : public tNode
{
public:
	//得到某个变量的值，如果不存在，则返回FALSE
	virtual BOOL	GetVariable(LPCTSTR szName, STRING& strValue)		= 0;
	//设置某个变量的值，如果不存在，则首先创建
	virtual VOID	SetVariable(LPCTSTR szName, LPCTSTR szValue, BOOL bTemp=TRUE, BOOL bFireEvent=TRUE)	= 0;
	//设置某个变量的值，但并不马上生效, 下一次启动时才会生效
	virtual VOID	SetVariableDelay(LPCTSTR szName, LPCTSTR szValue)	= 0;

	//-------------------------------------------------------------------
	//快速设置通道

	//整数
	virtual VOID	SetAs_Int(LPCTSTR szName, INT nValue, BOOL bTemp = TRUE)				= 0;
	//无符号整数
	virtual VOID	SetAs_Uint(LPCTSTR szName, UINT uValue, BOOL bTemp = TRUE)				= 0;
	//浮点数
	virtual VOID	SetAs_Float(LPCTSTR szName, FLOAT fValue, BOOL bTemp = TRUE)			= 0;
	//Vector2
	virtual VOID	SetAs_Vector2(LPCTSTR szName, FLOAT fX, FLOAT fY, BOOL bTemp = TRUE)	= 0;

	//-------------------------------------------------------------------
	//快速获取通道

	//字符串, DEF=""
	virtual const STRING&	GetAs_String(LPCTSTR szName, BOOL* bHave = NULL)	= 0;
	//整数, DEF=0
	virtual INT				GetAs_Int(LPCTSTR szName, BOOL* bHave = NULL)		= 0;
	//无符号整数, DEF=0
	virtual UINT			GetAs_Uint(LPCTSTR szName, BOOL* bHave = NULL)		= 0;
	//浮点数, DEF=0.0f
	virtual FLOAT			GetAs_Float(LPCTSTR szName, BOOL* bHave = NULL)		= 0;
	//Vector2, DEF=(0.0f, 0.0f)
	virtual fVector2		GetAs_Vector2(LPCTSTR szName, BOOL* bHave = NULL)	= 0;

	virtual void			SetVariableDefault(LPCTSTR szName, LPCTSTR szValue) = 0;

	virtual void			LoadPrivateConfig()									= 0;

	virtual void            GetVariableInFile( LPCTSTR szFileName, LPCTSTR szTitle, LPCTSTR szKey, LPTSTR szValue, INT nSize ) = 0;
	virtual void            SetVariableInFile( LPCTSTR szFileName, LPCTSTR szTitle, LPCTSTR szKey, LPCTSTR szValue ) = 0;
	virtual void			ReSet() = 0;
	KL_DECLARE_DYNAMIC(tVariableSystem);
};

