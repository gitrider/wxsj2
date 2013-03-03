
/**	GMDP_Struct_Country.h
 *
 *	功能：	国家相关的数据
 *
 *	修改记录：	
 *			
 *					
 */

#pragma once


#include "Type.h"


#define COUNTRY_FUNCTION_TYPE_NUM	5



/// 官职类型
enum eFunctionaryType
{
	FT_ZAIXIANG = 0,       //宰相
	FT_YUANSHUAI,          //元帅
	FT_YUSHIDAFU,          //御史大夫
	FT_BUTOU,              //捕头
};


/// 国家官职信息
struct CountryFunctionaryInfo
{
	eFunctionaryType	m_eType;				// 官职类型
	STRING				m_sFunctionaryName;		// 官职名称
};

