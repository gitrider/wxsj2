/****************************************\
*										*
* 			  ���ò���ϵͳ				*
*										*
*						 by fangyuan	*
\****************************************/
#pragma once
#include "GIVariable.h"

namespace LuaPlus
{
	class LuaState;
	class LuaObject;
}

class CVariableSystem : public tVariableSystem
{
public:
	//�õ�ĳ��������ֵ����������ڣ��򷵻�FALSE
	virtual BOOL	GetVariable(LPCTSTR szName, STRING& strValue)		;	
	//����ĳ��������ֵ����������ڣ������ȴ���
	virtual VOID	SetVariable(LPCTSTR szName, LPCTSTR szValue, BOOL bTemp=TRUE, BOOL bFireEvent=TRUE)	;
	//����ĳ��������ֵ��������������Ч, ��һ������ʱ�Ż���Ч
	virtual VOID	SetVariableDelay(LPCTSTR szName, LPCTSTR szValue)	;

	//-------------------------------------------------------------------
	//��������ͨ��

	//����
	virtual VOID	SetAs_Int(LPCTSTR szName, INT nValue, BOOL bTemp=TRUE)				;
	//�޷�������
	virtual VOID	SetAs_Uint(LPCTSTR szName, UINT uValue, BOOL bTemp=TRUE)				;
	//������
	virtual VOID	SetAs_Float(LPCTSTR szName, FLOAT fValue, BOOL bTemp=TRUE)			;
	//Vector2
	virtual VOID	SetAs_Vector2(LPCTSTR szName, FLOAT fX, FLOAT fY, BOOL bTemp=TRUE)	;

	//-------------------------------------------------------------------
	//���ٻ�ȡͨ��

	//�ַ���, DEF=""
	virtual const STRING&	GetAs_String(LPCTSTR szName, BOOL* bHave = NULL)	;
	//����, DEF=0
	virtual INT				GetAs_Int(LPCTSTR szName, BOOL* bHave = NULL)		;
	//�޷�������, DEF=0
	virtual UINT			GetAs_Uint(LPCTSTR szName, BOOL* bHave = NULL)		;
	//������, DEF=0.0f
	virtual FLOAT			GetAs_Float(LPCTSTR szName, BOOL* bHave = NULL)		;
	//Vector2, DEF=(0.0f, 0.0f)
	virtual fVector2		GetAs_Vector2(LPCTSTR szName, BOOL* bHave = NULL)	;

	//-------------------------------------------------------------------
	//������δ���õı���,����ñ����Ѿ����壬�˺�����Ч
	virtual void			SetVariableDefault(LPCTSTR szName, LPCTSTR szValue);

	virtual void			LoadPrivateConfig();

	virtual void            GetVariableInFile( LPCTSTR szFileName, LPCTSTR szTitle, LPCTSTR szKey, LPTSTR szValue, INT nSize );
	virtual void            SetVariableInFile( LPCTSTR szFileName, LPCTSTR szTitle, LPCTSTR szKey, LPCTSTR szValue );

public:
	//���ڽű����õĺ���
	INT Lua_SetGlobalVariable(LuaPlus::LuaState* state);	//����ȫ�ֱ���
	INT Lua_GetGlobalVariable(LuaPlus::LuaState* state);	//ȡ��ȫ�ֱ���

protected:
	LuaPlus::LuaObject* m_pMetaTable;

protected:
	struct VARIABLE
	{
		STRING	vValue;	//����ֵ
		BOOL	bTemp;	//��ʱ����
	};
#ifdef __SGI_STL_PORT
	typedef std::hash_map< STRING, VARIABLE >	VARIABLE_MAP;
#else
	typedef stdext::hash_map< STRING, VARIABLE >	VARIABLE_MAP;
#endif

	//���ļ��ж�ȡ����
	VOID	LoadVariable(LPCTSTR szFileName, VARIABLE_MAP& mapBuf);
	//���������浽�ļ���
	VOID	SaveVariable(LPCTSTR szFileName, VARIABLE_MAP& mapBuf);

public:
	virtual VOID	Initial(VOID*);
	virtual VOID	Release(VOID);
	virtual VOID	ReSet(VOID);
protected:
	//��ǰ����ı���
	VARIABLE_MAP		m_mapCurrent;
	//��Ϸ�˳�ʱ���Ĳ�����ı���,�Ա���һ����Ϸ������Ч
	VARIABLE_MAP		m_mapNeedSave;

	void StringEncrypt( std::string& strSrc, std::string& strDst );
	void ConvertOldSaving_pfc( const std::string& strAccount, const std::string& strCharacter );
	void ConvertOldSaving_cfg( const std::string& strAccount );

public:
	CVariableSystem();
	virtual ~CVariableSystem();
	static CVariableSystem* GetMe(VOID) { return s_pMe; }

protected:
	static CVariableSystem*		s_pMe;
protected:

	void SaveAcountVariables(void);
	KL_DECLARE_DYNAMIC(CVariableSystem);
};
