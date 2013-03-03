/********************************************************************
	created:	2007/12/24
	created:	24:12:2007   17:11
	file base:	SplitString
	file ext:	h
	author:		Richard
	
	purpose:	�����ַ��������,ָ����ֵķָ���

				1 �������ַ�����ָ�����ַ����в��ʵ��Ϊ Decorator ģʽ,��֧��һ���ַ����ò�ͬ�ķָ������.

				�ָ����Ǿ��в㼶��ϵ��,���Ե���ָ��ÿһ���Ĳ�ֱ�ʶ��,���� ; Ϊһ����ַ�,�Ѳ�ֵ���
				
				�������ٰ�������ַ��������в��, ��ʱְ�����ᰴ�ձ���֯�Ĳ㼶����ϵ���δ��ݾ����Լ�
				
				��ֵ��ַ������Ӷ���ֱ��ְ��������.

				2 ��������ʹ�ò���ģʽ,������㷨����,��ͬ�Ĳ���㷨��Ѳ�ֺ���ַ������浽������

	ע��:

		1 ��Զ��ĩ����ֶ������ﰴȡ�����м�����Ч��ֽ��, ��Ϊǰ��Ĳ㼶�����ֻ���м�

		���̵���ʱ���

	Ŀǰ��������: 
		
		1 �ο�SplitStringBase.h�е�˵��
	ʹ��˵��:
		1 �ο�SplitStringBase.h�е�˵��

*********************************************************************/

/********************************************************************
	��ʹ��ʱ������
		
		��Ϊ���ܶ�̬�����ڴ�,�����ַ�������������ڴ����Ԥ����,��˶��ַ����ĳ��������޵�����

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
	// Returns:   BOOL TRUE��ָ���Ĳ�ַ��Ž�������Ч���,����ΪFALSE
	// Qualifier:
	// Parameter: const CHAR * strSplit
	// ִ�в�ֲ���,����SplitStringBase����,ʵ�ֲ���ģʽ
	//************************************
	virtual BOOL DoSplit( const CHAR* strSplit );

private:
	//************************************
	// Method:    DoSubLevelSplit
	// FullName:  SplitStringLevelTwo::DoSubLevelSplit
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// ������2����ֵ��ַ���չ��
	//************************************
	VOID DoSubLevelSplit( INT iPos, const CHAR* strSplit );
private:
	CHAR m_strSplitLevelTwo[MAX_LEVEL_ONE_LENGTH][MAX_SPLIT_STRING_LENGTH];
};

#endif // SplitStringLevelTwo_h__