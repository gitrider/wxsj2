/********************************************************************
	created:	2007/12/24
	created:	24:12:2007   17:11
	file base:	SplitString
	file ext:	h
	author:		Richard
	
	purpose:	具体字符串拆分类,指定拆分的分隔符

				1 将给定字符串用指定的字符进行拆分实现为 Decorator 模式,以支持一个字符串用不同的分隔符拆分.

				分隔符是具有层级关系的,可以单独指定每一级的拆分标识符,比如 ; 为一级拆分符,已拆分的字
				
				符串会再按二级拆分符继续进行拆分, 此时职责链会按照被组织的层级级关系依次传递经过自己
				
				拆分的字符串给子对象直道职责链结束.

				2 具体拆分类使用策略模式,将拆分算法分离,不同的拆分算法会把拆分后的字符串缓存到数组中

	注意:

		1 永远到末级拆分对象那里按取得所有级的有效拆分结果, 因为前面的层级保存的只是中间

		过程的临时结果

	目前的限制是: 
		
		1 参考SplitStringBase.h中的说明
	使用说明:
		1 参考SplitStringBase.h中的说明

*********************************************************************/

/********************************************************************
	在使用时的限制
		
		因为不能动态分配内存,所以字符串缓存所需的内存必须预分配,因此对字符串的长度有上限的限制

*********************************************************************/

#ifndef SplitStringLevelTwo_h__
#define SplitStringLevelTwo_h__

#include "SplitStringBase.h"

class SplitStringLevelTwo : public SplitStringBase
{
public:
	//************************************
	// Method:    DoSplit
	// FullName:  SplitStringBase::DoSplit
	// Access:    virtual public 
	// Returns:   BOOL TRUE用指定的拆分符号进行了有效拆分,否则为FALSE
	// Qualifier:
	// Parameter: const CHAR * strSplit
	// 执行拆分操作,重载SplitStringBase方法,实现策略模式
	//************************************
	virtual BOOL DoSplit( const CHAR* strSplit );

private:
	//************************************
	// Method:    DoSubLevelSplit
	// FullName:  SplitStringLevelTwo::DoSubLevelSplit
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// 将包含2级拆分的字符串展开
	//************************************
	VOID DoSubLevelSplit( INT iPos, const CHAR* strSplit );
private:
	CHAR m_strSplitLevelTwo[MAX_LEVEL_ONE_LENGTH][MAX_SPLIT_STRING_LENGTH];
};

#endif // SplitStringLevelTwo_h__