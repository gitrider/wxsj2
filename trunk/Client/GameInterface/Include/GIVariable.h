#pragma once
#include "GINode.h"

//===================================================
//��Ϸϵͳ���ñ���������
#include "GIMath.h"

class KYLIN_API tVariableSystem : public tNode
{
public:
	//�õ�ĳ��������ֵ����������ڣ��򷵻�FALSE
	virtual BOOL	GetVariable(LPCTSTR szName, STRING& strValue)		= 0;
	//����ĳ��������ֵ����������ڣ������ȴ���
	virtual VOID	SetVariable(LPCTSTR szName, LPCTSTR szValue, BOOL bTemp=TRUE, BOOL bFireEvent=TRUE)	= 0;
	//����ĳ��������ֵ��������������Ч, ��һ������ʱ�Ż���Ч
	virtual VOID	SetVariableDelay(LPCTSTR szName, LPCTSTR szValue)	= 0;

	//-------------------------------------------------------------------
	//��������ͨ��

	//����
	virtual VOID	SetAs_Int(LPCTSTR szName, INT nValue, BOOL bTemp = TRUE)				= 0;
	//�޷�������
	virtual VOID	SetAs_Uint(LPCTSTR szName, UINT uValue, BOOL bTemp = TRUE)				= 0;
	//������
	virtual VOID	SetAs_Float(LPCTSTR szName, FLOAT fValue, BOOL bTemp = TRUE)			= 0;
	//Vector2
	virtual VOID	SetAs_Vector2(LPCTSTR szName, FLOAT fX, FLOAT fY, BOOL bTemp = TRUE)	= 0;

	//-------------------------------------------------------------------
	//���ٻ�ȡͨ��

	//�ַ���, DEF=""
	virtual const STRING&	GetAs_String(LPCTSTR szName, BOOL* bHave = NULL)	= 0;
	//����, DEF=0
	virtual INT				GetAs_Int(LPCTSTR szName, BOOL* bHave = NULL)		= 0;
	//�޷�������, DEF=0
	virtual UINT			GetAs_Uint(LPCTSTR szName, BOOL* bHave = NULL)		= 0;
	//������, DEF=0.0f
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

