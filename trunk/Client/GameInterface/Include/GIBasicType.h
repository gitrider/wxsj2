
/** GIBasicType.h
 */

#ifndef _GAME_INTERFACE_BASIC_TYPE_H_
#define _GAME_INTERFACE_BASIC_TYPE_H_


#include <windows.h>
#include <shlwapi.h>
#include <Mmsystem.h>

#include <tchar.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <string>
#include <vector>
#include <list>
#include <map>
#include <hash_map>
#include <algorithm>



/** Ԥ����ѡ��
 */

#ifdef GAME_EXPORTS
		#define KYLIN_API __declspec(dllexport)
#else
		#define KYLIN_API __declspec(dllimport)
#endif
#ifdef GAME_LIB
#undef  KYLIN_API
#define KYLIN_API
#endif

// disable: "<type> needs to have dll-interface to be used by clients'
// Happens on STL member variables which are not public therefore is ok
#pragma warning (disable : 4251)

// ��ͼ����İ汾��
#define	KL_ENGINE_VERSION			((UINT)101)




/** �������Ͷ���
 */

#define INVALID_UID					(-1)

/// �ַ������
#ifdef UNICODE
	typedef	std::wstring			STRING;
#else
	typedef	std::string				STRING;
#endif




#endif	// _GAME_INTERFACE_BASIC_TYPE_H_
