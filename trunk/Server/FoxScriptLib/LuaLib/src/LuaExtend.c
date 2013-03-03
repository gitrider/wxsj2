//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
// 
// File:	LuaExtend.cpp
// Date:	2001-9-27 15:46:40
// Code:	Romandou
// Desc:	��չ��LuaAPI����
//---------------------------------------------------------------------------
#include "lua.h"
#include "string.h"
#include <stdio.h>
#include <stdarg.h>

//---------------------------------------------------------------------------
// ����:	Lua_SetTable
// ����:	�޸�Lua table�е�ĳ����Ա��ֵ
// ����:	Lua_State * L
// ����:	int nIndex �����ڶ��е�index
// ����:	const char * szMemberName ���ַ�����ʾ��Ա������
// ����:	double Number ��Ա����ֵ
// ����:	int 
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//                   Lua_SetTableMember
//    �ú����������ɵĺ�����ɣ���ʵ�ָı�Lua Table�е�ĳ��������ֵ
//    Lua��Table�ı�����ʽ�����֣�һ������������ʽ��ʾ�� Table[1],����һ
//	  �������ַ�������ʽ��ʾ����Table.Member��	
//	  ͬʱ������ֵ������Number��String��CFun��
//	 	  nIndexΪ�����ڶ�ջ�е�λ�á�
//---------------------------------------------------------------------------
int Lua_SetTable_DoubleFromName(Lua_State * L, int nIndex, const char * szMemberName, double Number)
{
	
	if (! Lua_IsTable(L, nIndex))
		return 0;
	Lua_PushString(L, szMemberName);
	Lua_PushNumber(L, Number);
	Lua_SetTable(L, nIndex);
	return 1;
	
}


//---------------------------------------------------------------------------
// ����:	Lua_SetTable_IntFromName
// ����:	
// ����:	Lua_State * L
// ����:	int nIndex
// ����:	const char * szMemberName
// ����:	int Number
// ����:	int 
//---------------------------------------------------------------------------
int Lua_SetTable_IntFromName(Lua_State * L, int nIndex, const char * szMemberName, int Number)
{

	if (! Lua_IsTable(L, nIndex))
		return 0;
	Lua_PushString(L, szMemberName);
	Lua_PushNumber(L, (double)Number);
	Lua_SetTable(L, nIndex);
	return 1;
	
}


//---------------------------------------------------------------------------
// ����:	Lua_SetTable_StringFromName
// ����:	
// ����:	Lua_State * L
// ����:	int nIndex
// ����:	const char * szMemberName
// ����:	char * szString
// ����:	int 
//---------------------------------------------------------------------------
int Lua_SetTable_StringFromName(Lua_State * L ,int nIndex, const char * szMemberName, char * szString)
{
	
	if (! Lua_IsTable(L, nIndex))
		return 0;
	Lua_PushString(L, szMemberName);
	Lua_PushString(L, szString);
	Lua_SetTable(L, nIndex);
	return 1;
	
}

//---------------------------------------------------------------------------
// ����:	Lua_SetTable_StringFromId
// ����:	
// ����:	Lua_State * L
// ����:	int nIndex
// ����:	int Id
// ����:	const char * szString
// ����:	int 
//---------------------------------------------------------------------------
int Lua_SetTable_StringFromId(Lua_State * L, int nIndex, int Id, const char * szString)
{
	if (! Lua_IsTable(L, nIndex))
		return 0;
	Lua_PushNumber(L, (double)Id);
	Lua_PushString(L, szString);
	Lua_SetTable(L, nIndex);
	return 1;
}


//---------------------------------------------------------------------------
// ����:	Lua_SetTable_DoubleFromId
// ����:	
// ����:	Lua_State * L
// ����:	int nIndex
// ����:	int Id
// ����:	double nNumber
// ����:	int 
//---------------------------------------------------------------------------
int Lua_SetTable_DoubleFromId(Lua_State * L , int nIndex, int Id, double nNumber)
{
	if (! Lua_IsTable(L, nIndex))
		return 0;
	Lua_PushNumber(L, (double) Id);
	Lua_PushNumber(L, nNumber);
	Lua_SetTable(L, nIndex);
	return 1;
}

//---------------------------------------------------------------------------
// ����:	Lua_SetTable_IntFromId
// ����:	
// ����:	Lua_State * L
// ����:	int nIndex
// ����:	int Id
// ����:	int nNumber
// ����:	int 
//---------------------------------------------------------------------------
int Lua_SetTable_IntFromId(Lua_State * L, int nIndex, int Id, int nNumber)
{
	if (! Lua_IsTable(L, nIndex))
		return 0;
	Lua_PushNumber(L, (double) Id);
	Lua_PushNumber(L, (double) nNumber);
	Lua_SetTable(L, nIndex);
	return 1;
}

//---------------------------------------------------------------------------
// ����:	Lua_SetTable_CFunFromName
// ����:	
// ����:	Lua_State * L
// ����:	int nIndex
// ����:	const char * szMemberName
// ����:	Lua_CFunction CFun
// ����:	int 
//---------------------------------------------------------------------------
int Lua_SetTable_CFunFromName(Lua_State * L, int nIndex, const char * szMemberName, Lua_CFunction CFun)
{
	if (! Lua_IsTable(L, nIndex))
		return 0;
	Lua_PushString(L, szMemberName);
	Lua_PushCFunction(L, CFun);
	Lua_SetTable(L, nIndex);
	return 1;
}

//---------------------------------------------------------------------------
// ����:	Lua_SetTable_CFunFromId
// ����:	
// ����:	Lua_State * L
// ����:	int nIndex
// ����:	int nId
// ����:	Lua_CFunction CFun
// ����:	int 
//---------------------------------------------------------------------------
int Lua_SetTable_CFunFromId(Lua_State * L, int nIndex, int nId, Lua_CFunction CFun)
{
	if (! Lua_IsTable(L, nIndex))
		return 0;
	Lua_PushNumber(L, nId);
	Lua_PushCFunction(L, CFun);
	Lua_SetTable(L, nIndex);
	return 1;
}


//---------------------------------------------------------------------------
// ����:	Lua_GetValuesFromStack
// ����:	
// ����:	Lua_State * L
// ����:	char * cFormat
// ����:	...
// ����:	int 
//---------------------------------------------------------------------------
int Lua_GetValuesFromStack(Lua_State * L, char * cFormat, ...)	
{

	va_list vlist;
	double* pNumber;
	 const char  *pString;
	   const char ** pStrPoint;
	
	int i = 0;
	int nTopIndex = 0;
	int nIndex = 0;
	int nValueNum = 0;//cFormat���ַ����ȣ���ʾ��Ҫȡ�Ĳ�������
	
	
	if (! L)
		return 0;
	
	nTopIndex = Lua_GetTopIndex(L);	
	nValueNum = strlen(cFormat);
	
	if (nTopIndex == 0 || nValueNum == 0)//����ջ�������ݻ�ȡ�����Ƿ���FALSE
		return 0;
	
	if (nTopIndex < nValueNum)
		nIndex = 1 ;
	else
		nIndex = nTopIndex - nValueNum +1;
	
	{
		va_start(vlist, cFormat);     
		
		while (cFormat[i] != '\0')
		{
			
			switch(cFormat[i])
			{
			case 'n'://����ֵΪ��ֵ��,Number,��ʱLuaֻ����double�ε�ֵ
				{
					pNumber = va_arg(vlist, double *);
					
					if (pNumber == NULL)
						return 0;
					
					if (Lua_IsNumber(L, nIndex ))
					{

						* pNumber = Lua_ValueToNumber(L, nIndex ++ );
						
					}
					else
					{
					//	ScriptError(LUA_SCRIPT_NOT_NUMBER_ERROR);
						return 0;
					}
					
					
				}
				break;
			case 's'://�ַ�����
				{
					pStrPoint = va_arg(vlist, const char **);
					
					if (pStrPoint == NULL)
						return 0;
					
					if (Lua_IsString(L, nIndex))
					{
						pString = Lua_ValueToString(L, nIndex++);
					
						*pStrPoint = pString;
						
					}
					else
					{
					//	ScriptError(LUA_SCRIPT_NOT_STRING_ERROR);
						return 0;
					}
				}
				break;
				
			}
			
			

			i ++;	
		}
		va_end(vlist);     		/* Reset variable arguments.      */
		
	}
	return	1;
}



