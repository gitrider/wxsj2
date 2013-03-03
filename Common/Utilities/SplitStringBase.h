/********************************************************************
	created:	2007/12/24
	created:	24:12:2007   17:11
	filename: 	MapServer\Server\Utilities\SplitStringBase.h
	file path:	MapServer\Server\Utilities
	author:		Richard
	
	purpose:	字符串拆分类的接口

				1 将给定字符串用指定的字符进行拆分实现为 Decorator 模式,以支持一个字符串用不同的分隔符拆分.

				分隔符是具有层级关系的,可以单独指定每一级的拆分标识符,比如 ; 为一级拆分符,已拆分的字
				
				符串会再按二级拆分符继续进行拆分, 此时职责链会按照被组织的层级级关系依次传递经过自己
				
				拆分的字符串给子对象直道职责链结束.

				2 具体拆分类使用策略模式,将拆分算法分离,不同的拆分算法会把拆分后的字符串缓存到数组中

	注意:
				1 永远到末级拆分对象那里按取得所有级的有效拆分结果, 因为前面的层级保存的只是中间

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

				2 具体可参考下面的 测试代码


*********************************************************************/

/********************************************************************
	在使用时的限制
		
		因为不能动态分配内存,所以字符串缓存所需的内存必须预分配,因此对字符串的长度有上限的限制

*********************************************************************/

/********************************************************************
	可供测试的数据
	SplitStringLevelOne g_TestLevel1;
	SplitStringLevelTwo g_TestLevel2;

	SplitStringLevelOne g_TestLevel1;
	SplitStringLevelTwo g_TestLevel2;

	CHAR g_str[640] = {"1-20;2-30;3;4-60;5;6"};
	CHAR g_str1[640] = {"1;3;5;6;9;10;4294967295"};
*********************************************************************/

/********************************************************************
	测试代码
	g_TestLevel2.Init( '-', NULL );
	g_TestLevel1.Init( ';', &g_TestLevel2 );
	g_TestLevel1.DoSplit( g_str );

	g_TestLevel1.Reset();
	g_TestLevel1.Init( ';', NULL );
	g_TestLevel1.DoSplit( g_str1 );

	INT iCount = g_TestLevel1.GetResultLineCount( 1 );
	for( INT i=0; i<iCount; ++i )
	{
	cout << g_TestLevel1.GetResultLine( 1, i );
	if( i < iCount-1 )
	cout << ",  ";
	}
	cout << endl;

	for( INT i=1; i<3; ++i )
	{
	iCount = g_TestLevel2.GetResultLineCount( i );
	for( INT j=0; j<iCount; ++j )
	{
	cout << g_TestLevel2.GetResultLine( i, j );
	if( j < iCount-1 )
	cout << ",  ";
	}
	cout << endl;
	}
*********************************************************************/

#include "Type.h"

#ifndef SplitStringBase_h__
#define SplitStringBase_h__

class SplitStringBase
{
public:
	enum
	{
		//因为本项目使用的数值最大为32位数值，所以10个字符可以表示最大值（10进制）
		//加\0,为11个字符
		MAX_FINAL_STRING_LENGTH = 11,
		//经过初步拆分后的字符串最大长度为, 10 + '-' + 10 + '\0', 32为安全值
		MAX_SPLIT_STRING_LENGTH = 256,
		MAX_LEVEL_ONE_LENGTH	= 100,
		MAX_LEVEL_TWO_LENGTH	= MAX_LEVEL_ONE_LENGTH,
		//输入的原始字符串,允许最大一级待拆分的段的个数为20
		MAX_INPUT_STRING_LENGTH = MAX_LEVEL_ONE_LENGTH * MAX_SPLIT_STRING_LENGTH
	};

	SplitStringBase(VOID);
	virtual ~SplitStringBase(VOID);

	//************************************
	// Method:    GetChild
	// FullName:  SplitStringBase::GetChild
	// Access:    public 
	// Returns:   const SplitStringBase&
	// Qualifier:
	// 
	//************************************
	virtual const SplitStringBase* GetChild();
	
	//************************************
	// Method:    Reset
	// FullName:  SplitStringBase::Reset
	// Access:    virtual public 
	// Returns:   VOID
	// Qualifier:
	// 清零所有缓存
	//************************************
	virtual VOID Reset();

	//************************************
	// Method:    Init
	// FullName:  SplitStringBase::Init
	// Access:    public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: CHAR cSplit	分隔符号
	// Parameter: SplitStringBase * pChild 如果需要更多
	//
	//			  的拆分层级则指定下层拆分对象,否则 NULL
	//
	// 指定分隔符和职责链的下个节点
	//************************************
	virtual VOID Init( CHAR cSplit, SplitStringBase* pChild );

	//************************************
	// Method:    DoSplit
	// FullName:  SplitStringBase::DoSplit
	// Access:    virtual public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: const CHAR * strSplit
	//************************************
	virtual BOOL DoSplit( const CHAR* strSplit ) = 0;

	//************************************
	// Method:    GetResultLineCount
	// FullName:  SplitStringBase::GetResultLineCount
	// Access:    virtual public 
	// Returns:   INT
	// Qualifier:
	// 取得拆分后字符串的数量
	//************************************
	virtual INT GetResultLineCount();
	//************************************
	// Method:    GetResultLine
	// FullName:  SplitStringBase::GetResultLine
	// Access:    virtual public 
	// Returns:   CHAR*
	// Qualifier:
	// Parameter: INT iLine  当前要处理的行
	//************************************
	virtual const CHAR* GetResultLine( INT iLine );

protected:

	//************************************
	// Method:    DoSubSplit
	// FullName:  SplitStringBase::DoSubSplit
	// Access:    protected 
	// Returns:   VOID
	// Qualifier:
	//************************************
	BOOL DoSubSplit();

protected:

	//职责链的子对象
	SplitStringBase*	m_pChild;
	CHAR				m_SplitChar[2];
	CHAR				m_strSplitLevelOne[MAX_LEVEL_ONE_LENGTH][MAX_SPLIT_STRING_LENGTH];
	INT					m_iLevelOneLineCount;
};
#endif // SplitStringBase_h__
