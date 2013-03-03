/********************************************************************
	created:	2007/12/24
	created:	24:12:2007   17:11
	author:		Richard
	
	purpose:	具体字符串拆分类,指定拆分的分隔符

	注意:
		永远到末级拆分对象那里按取得所有级的有效拆分结果, 因为前面的层级保存的只是中间

		过程的临时结果

		目前的限制是: 

			1 不能动态分配内存,所以链上的子对象不能动态装配,所以不能实现成标准的职责链模式,而

			是经过变通的.

			2 目前的方案是,链上的所有对象都是对上一级进行装饰, 添加新的拆分规则只需要添加新的

			具体拆分类就可以了,不要修改已经完成的类, 这样可以始终保持 装饰 模式的特点以及易

			扩展的好处

			3 由使用者对职责链进行组装(参考测试代码), 所以使用者必须了解所有的具体拆分类
				
	使用说明:
		
		1 由使用者进行职责链的装配, 比如, 使用者有属性: SplitString[3],那么链的长度最大为3

		为每个节点设置子节点,并用分隔符初始化每个节点,每个节点可以有不同的分隔符

		2 具体可参考SplitStringBase.h中的 测试代码


*********************************************************************/

/********************************************************************
	在使用时的限制
		
		因为不能动态分配内存,所以字符串缓存所需的内存必须预分配,因此对字符串的长度有上限的限制

*********************************************************************/

#ifndef SplitStringLevelOne_h__
#define SplitStringLevelOne_h__


#include "SplitStringBase.h"

class SplitStringLevelOne : public SplitStringBase
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
};

#endif // SplitStringLevelOne_h__