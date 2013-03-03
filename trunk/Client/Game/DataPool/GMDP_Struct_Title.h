
/** 数据池数据结构 -称号
 */

#pragma once


// 保存的称号数据
struct _TITLE_
{
	// 保存称号的类型
	enum
	{
		INVALID_TITLE = 0,	// 无效
		
		ID_TITLE,			// 保存id
		STRING_TITLE,		// 保存字符串
	};


	BOOL		bFlag;		// 表明此title是否是id
	BYTE		bType;		// 表明此title的类型
	BYTE		bArrId;		// 记录某类称号在整个数组的索引
	INT			nTime;		// title时限到期时间，无限期的用0

//	union
//	{
		INT		ID;									// 如果是id,这个就是表中id
		CHAR	szTitleData[ MAX_CHARACTER_TITLE ];	// 如果不是id，这个就是title内容
//	};
	
	
	_TITLE_()
	{
		bFlag	= INVALID_TITLE;
		bType	= _TITLE::NO_TITLE;
		bArrId	= -1;
		nTime	= 0;
		ID		= -1;

		memset( szTitleData, 0, MAX_CHARACTER_TITLE );
	}

};