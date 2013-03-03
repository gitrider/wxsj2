#ifndef __LOGIN_TABLE__
#define  __LOGIN_TABLE__

#include "Type.h"
#include "GameDefine.h"
#include "GameDefine2.h"

struct _DEFAULT_CHARDATA_TBL
{
	INT		m_Table[MAX_COUNTRY_NUM][MAX_MENPAI_NUM][CHAR_ININUM] ;

	_DEFAULT_CHARDATA_TBL( )
	{
		memset( m_Table, 0, sizeof(INT)*MAX_MENPAI_NUM*CHAR_ININUM*MAX_COUNTRY_NUM ) ;
	};
	INT		Get( uint Country, uint ProfessionType, uint CharParam )
	{
		Assert( Country<MAX_COUNTRY_NUM && ProfessionType<MAX_MENPAI_NUM && CharParam<CHAR_ININUM ) ;
		return m_Table[Country][ProfessionType][CharParam] ;
	};
	VOID	Set(uint Country,  uint MenPai ,uint Attr, INT iValue){
		Assert( Country<MAX_COUNTRY_NUM && MenPai<MATTRIBUTE_NUMBER && Attr<CHAR_ININUM  ) ;
		m_Table[Country][MenPai][Attr] = iValue;	
	}
	VOID	Init();
};
extern _DEFAULT_CHARDATA_TBL g_DefaultCharDataTbl ;


#endif