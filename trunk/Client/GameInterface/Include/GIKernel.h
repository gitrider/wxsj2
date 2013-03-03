
/** GIKernel.h
 */

#pragma once


#include "GIBasicType.h"
#include "GINode.h"
#include "GIPlugin.h"



/** ���ݺ���
 */
class KYLIN_API tKernel
{
	//--------------------------------------------------
	//		���������
	//--------------------------------------------------
public:

	//��ĳ����ע�ᵽȫ�����������
	VOID		ReisgerClass(tClass* pNewClass);
	//����������Ƿ���м̳й�ϵ
	BOOL		IsKindOf(const tClass* pThisClass, const tClass* pBaseClass) const;
	//�����ַ�����ʽ����ȡ��ĳ������
	tClass*		OpenClass(LPCTSTR szClassName);

protected:

	//ȫ�ֶ����������
	std::map< STRING, tClass* >	m_ClassMap;
	//��������е�һ����
	tClass*						m_pFirstClass;
	//���ڵ�
	tNode						m_theRoot;

	//--------------------------------------------------
	//		�ڵ�������
	//--------------------------------------------------
public:

	/**
		�´���һ���ڵ���뵽�ڵ����ϣ����нڵ�Ĵ�����Ӧ��ͨ������������������
			@param
				szClassName �������Ľڵ���������������Ѿ�ע���
				szPosition  �������Ľڵ����ڵĸ��ڵ��·��
				pParentNode	�������Ľڵ����ڵĸ��ڵ��ָ��
				szNodeName	�������Ľڵ������

		�ɹ������´����Ľڵ㣬���ʧ��,����NULL	
	*/
	///�����ַ�����ʽ·��������
	virtual tNode*	NewNode(LPCTSTR szClassName, LPCTSTR szPosition, LPCTSTR szNodeName);
	///���ݸ��ڵ�ָ����ʽ������
	virtual tNode*	NewNode(LPCTSTR szClassName, tNode* pParentNode, LPCTSTR szNodeName);

	///����·����ȡ�ýڵ㣬���� pNode = GetNode("scene\\object")
	virtual tNode*	GetNode(LPCTSTR szPathName);

	//--------------------------------------------------
	//		������
	//--------------------------------------------------
public:

	// ���ز��
	BOOL			LoadPlugin(LPCTSTR szPluginName, LPVOID pParam);
	// �رպ���
	VOID			ShutDown(VOID);

protected:

	//�����Ϣ
	struct PLUGIN_INFO
	{
		HMODULE			hModule;
		STRING			strPluginFile;
		FUNC_DLLINIT	pfnDllInit;
		FUNC_DLLRELEASE pfnDllRelease;
	};
	// �ͷŲ��
	VOID			FreeAllPlugin(VOID);
	//���в��
	std::vector< PLUGIN_INFO >	m_vAllPlugin;

public:

	tKernel(VOID);
	virtual ~tKernel(VOID);
	//�õ�����
	static tKernel* GetMe(VOID) { return s_pMe; }
	static void*	GetUserdata(){ return s_pUserdata; }
	static void		SetUserdata(void* pUserdata)
	{
		s_pUserdata = pUserdata;
	}
protected:

	static tKernel*		s_pMe;
	static void*		s_pUserdata;
};

