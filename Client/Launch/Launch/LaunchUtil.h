#pragma once


/*!
\brief
	将一段字符串按照回车换行分解成字符串列表

\param szSectionBuf
	字符串指针

\param nLen
	字符内存长度(防止越界)

\param vSectionBuf
	返回的字符串列表

\return 
	字符串列表长度
*/
INT ConvertSectionInVector(LPCTSTR szSectionBuf, INT nLen, std::vector< std::string >& vSectionBuf);


