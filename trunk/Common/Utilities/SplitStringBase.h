/********************************************************************
	created:	2007/12/24
	created:	24:12:2007   17:11
	filename: 	MapServer\Server\Utilities\SplitStringBase.h
	file path:	MapServer\Server\Utilities
	author:		Richard
	
	purpose:	�ַ��������Ľӿ�

				1 �������ַ�����ָ�����ַ����в��ʵ��Ϊ Decorator ģʽ,��֧��һ���ַ����ò�ͬ�ķָ������.

				�ָ����Ǿ��в㼶��ϵ��,���Ե���ָ��ÿһ���Ĳ�ֱ�ʶ��,���� ; Ϊһ����ַ�,�Ѳ�ֵ���
				
				�������ٰ�������ַ��������в��, ��ʱְ�����ᰴ�ձ���֯�Ĳ㼶����ϵ���δ��ݾ����Լ�
				
				��ֵ��ַ������Ӷ���ֱ��ְ��������.

				2 ��������ʹ�ò���ģʽ,������㷨����,��ͬ�Ĳ���㷨��Ѳ�ֺ���ַ������浽������

	ע��:
				1 ��Զ��ĩ����ֶ������ﰴȡ�����м�����Ч��ֽ��, ��Ϊǰ��Ĳ㼶�����ֻ���м�

				���̵���ʱ���

	Ŀǰ��������: 

				1 ���ܶ�̬�����ڴ�,�������ϵ��Ӷ����ܶ�̬װ��,���Բ���ʵ�ֳɱ�׼��ְ����ģʽ,��

				�Ǿ�����ͨ��.

				2 Ŀǰ�ķ�����,���ϵ����ж����Ƕ���һ������װ��, ����µĲ�ֹ���ֻ��Ҫ����µ�

				��������Ϳ�����,��Ҫ�޸��Ѿ���ɵ���, ��������ʼ�ձ��� װ�� ģʽ���ص��Լ���

				��չ�ĺô�

				3 ��ʹ���߶�ְ����������װ(�ο����Դ���), ����ʹ���߱����˽����еľ�������
				
	ʹ��˵��:
		
				1 ��ʹ���߽���ְ������װ��, ����, ʹ����������: SplitString[3],��ô���ĳ������Ϊ3

				Ϊÿ���ڵ������ӽڵ�,���÷ָ�����ʼ��ÿ���ڵ�,ÿ���ڵ�����в�ͬ�ķָ���

				2 ����ɲο������ ���Դ���


*********************************************************************/

/********************************************************************
	��ʹ��ʱ������
		
		��Ϊ���ܶ�̬�����ڴ�,�����ַ�������������ڴ����Ԥ����,��˶��ַ����ĳ��������޵�����

*********************************************************************/

/********************************************************************
	�ɹ����Ե�����
	SplitStringLevelOne g_TestLevel1;
	SplitStringLevelTwo g_TestLevel2;

	SplitStringLevelOne g_TestLevel1;
	SplitStringLevelTwo g_TestLevel2;

	CHAR g_str[640] = {"1-20;2-30;3;4-60;5;6"};
	CHAR g_str1[640] = {"1;3;5;6;9;10;4294967295"};
*********************************************************************/

/********************************************************************
	���Դ���
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
		//��Ϊ����Ŀʹ�õ���ֵ���Ϊ32λ��ֵ������10���ַ����Ա�ʾ���ֵ��10���ƣ�
		//��\0,Ϊ11���ַ�
		MAX_FINAL_STRING_LENGTH = 11,
		//����������ֺ���ַ�����󳤶�Ϊ, 10 + '-' + 10 + '\0', 32Ϊ��ȫֵ
		MAX_SPLIT_STRING_LENGTH = 256,
		MAX_LEVEL_ONE_LENGTH	= 100,
		MAX_LEVEL_TWO_LENGTH	= MAX_LEVEL_ONE_LENGTH,
		//�����ԭʼ�ַ���,�������һ������ֵĶεĸ���Ϊ20
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
	// �������л���
	//************************************
	virtual VOID Reset();

	//************************************
	// Method:    Init
	// FullName:  SplitStringBase::Init
	// Access:    public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: CHAR cSplit	�ָ�����
	// Parameter: SplitStringBase * pChild �����Ҫ����
	//
	//			  �Ĳ�ֲ㼶��ָ���²��ֶ���,���� NULL
	//
	// ָ���ָ�����ְ�������¸��ڵ�
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
	// ȡ�ò�ֺ��ַ���������
	//************************************
	virtual INT GetResultLineCount();
	//************************************
	// Method:    GetResultLine
	// FullName:  SplitStringBase::GetResultLine
	// Access:    virtual public 
	// Returns:   CHAR*
	// Qualifier:
	// Parameter: INT iLine  ��ǰҪ�������
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

	//ְ�������Ӷ���
	SplitStringBase*	m_pChild;
	CHAR				m_SplitChar[2];
	CHAR				m_strSplitLevelOne[MAX_LEVEL_ONE_LENGTH][MAX_SPLIT_STRING_LENGTH];
	INT					m_iLevelOneLineCount;
};
#endif // SplitStringBase_h__
