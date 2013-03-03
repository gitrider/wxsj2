/********************************************************************************
//	�ļ�����	GameStruct_Script.h
//	ȫ·����	d:\Prj\Common\GameStruct_Script.h
//	�����ˣ�	jack
//	����ʱ�䣺	2006 �� 1 �� 9 ��
//
//	����˵����	
//	�޸ļ�¼��
*********************************************************************************/

#ifndef __GAMESTRUCT_SCRIPT_H__
#define __GAMESTRUCT_SCRIPT_H__

#include "Type.h"

class SocketInputStream;
class SocketOutputStream;

//m_IntCount=1
//m_aIntValue=[1989]
//m_StrCount=3
//m_aStrOffset=[5][16][23]
//m_aStrValue:
//hello\0come herer\0gogogo\0
//


#define MAX_PARAM_SZIE 16
#define MAX_STR_SIZE 512
struct X_PARAM
{
	BYTE		m_IntCount ;//���Ͳ�������
	BYTE		m_StrCount ;//�ַ��Ͳ�������
	INT			m_aIntValue[MAX_PARAM_SZIE] ;//��������
	SHORT		m_aStrOffset[MAX_PARAM_SZIE] ;//�ַ��Ͳ��������\0�����ַ������е�ƫ��λ��
	CHAR		m_aStrValue[MAX_STR_SIZE] ;//�ַ�����

public :
	X_PARAM( ) ;
	~X_PARAM( ) ;

	VOID		CleanUp( ) ;

	VOID		Read(SocketInputStream& iStream) ;
	VOID		Write(SocketOutputStream& oStream) const ;

	INT			GetSize() const ;//ȡ��X_PARAM����Ч�ڴ��С

	INT			GetIntCount( ) ;//ȡ��X_PARAM�е������Ͳ�������
	INT			GetIntValue( INT nIndex ) ;//ȡ�õ�nIndex��X_PARAM�е������Ͳ���
	
	INT			GetStrCount( ) ;//ȡ��X_PARAM�е��ַ�����������
	CHAR*		GetStrValue( INT nIndex ) ;//ȡ��X_PARAM�еĵ�nIndex���ַ�������

	INT			AddIntValue( INT nValue ) ;//��X_PARAM�����һ�����Ͳ���
										   //����ֵΪ�˲��������
	INT			SetIntValue( INT nIndex, INT nValue ) ;//�޸�X_PARAM�еĵ�nIndex�����β���

	INT			AddStrValue( const CHAR* szIn ) ;//��X_PARAM�����һ���ַ����Ͳ���
										   //����ֵΪ�˲��������
	INT			SetStrValue( INT nIndex, CHAR* szIn ) ;//�޸�X_PARAM�еĵ�nIndex���ַ����Ͳ���
};


#define MAX_FUNC_NAME_SIZE 64
//#define MAX_STRING_PARAM_SIZE 256
#define MAX_INT_PARAM_COUNT 6
struct X_SCRIPT
{
	ScriptID_t	m_ScriptID ;
	BYTE		m_uFunNameSize ;
	CHAR		m_szFunName[MAX_FUNC_NAME_SIZE] ;
//	BYTE		m_uStrParamSize ;
//	CHAR		m_szStrParam[MAX_STRING_PARAM_SIZE] ;
	BYTE		m_uParamCount ;
	INT			m_aParam[MAX_INT_PARAM_COUNT] ;

public :
	X_SCRIPT( ) ;
	~X_SCRIPT( ) ;

	VOID		CleanUp( ) ;

	VOID		Read(SocketInputStream& iStream) ;
	VOID		Write(SocketOutputStream& oStream) const ;

	INT			GetSize() const ;//ȡ��X_SCRIPT����Ч�ڴ��С

	VOID		SetScriptID( ScriptID_t sid ){ m_ScriptID = sid ; }
	ScriptID_t	GetScriptID( ){ return m_ScriptID ; }

	VOID		SetFunName( char* szfunname )
	{
		strncpy( m_szFunName, szfunname, MAX_FUNC_NAME_SIZE-1 ) ;
		m_uFunNameSize = (BYTE)strlen(m_szFunName) ;
	};
	CHAR*		GetFunName( ){ return m_szFunName ; }
	INT			GetFunNameSize( ){ return m_uFunNameSize ; }

	//VOID		SetStrParam( char* szstrparam )
	//{
	//	strncpy( m_szStrParam, szstrparam, MAX_STRING_PARAM_SIZE-1 ) ;
	//	m_uStrParamSize = (BYTE)strlen(m_szStrParam) ;
	//};
	//CHAR*		GetStrParam( ){ return m_szStrParam ; }
	//INT			GetStrParamSize( ){ return m_uStrParamSize ; }

	VOID		SetParamCount( BYTE count ){ m_uParamCount = count ; }
	BYTE		GetParamCount( ){ return m_uParamCount ; }

	VOID		SetParam( BYTE uIndex, INT param )
	{
		if( uIndex>=MAX_INT_PARAM_COUNT ) return ;
		m_aParam[uIndex] = param ;
	}
	INT			GetParam( BYTE uIndex )
	{
		if( uIndex>=MAX_INT_PARAM_COUNT ) return 0 ;
		return m_aParam[uIndex] ;
	}

};


#endif
