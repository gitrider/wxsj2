
/**	GMDP_Struct_Country.h
 *
 *	���ܣ�	������ص�����
 *
 *	�޸ļ�¼��	
 *			
 *					
 */

#pragma once


#include "Type.h"


#define COUNTRY_FUNCTION_TYPE_NUM	5



/// ��ְ����
enum eFunctionaryType
{
	FT_ZAIXIANG = 0,       //����
	FT_YUANSHUAI,          //Ԫ˧
	FT_YUSHIDAFU,          //��ʷ���
	FT_BUTOU,              //��ͷ
};


/// ���ҹ�ְ��Ϣ
struct CountryFunctionaryInfo
{
	eFunctionaryType	m_eType;				// ��ְ����
	STRING				m_sFunctionaryName;		// ��ְ����
};

