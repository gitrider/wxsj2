
/**
 */

#pragma once


#include "GIBasicType.h"



/** ����������
 */

/// ������뵼���ĺ���DllInit
/// ���ڲ�����ܳ�ʼ��
typedef void (__stdcall* FUNC_DLLINIT)(LPVOID);

/// ������뵼���ĺ���DllRelease
/// ���ڲ�������ͷ�
typedef void (__stdcall* FUNC_DLLRELEASE)(void);

/// ������뵼��һ����ΪDllMagicCode��U32����,ֵ���� DLL_MAGIC_CODE|TD_KERNEL_VERSION
#define DLL_MAGIC_CODE	((UINT)0XC0DE0000)

