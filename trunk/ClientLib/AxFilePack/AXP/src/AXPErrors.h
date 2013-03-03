/*
		Axia|FilePack
			错误处理
*/

#pragma once

#include "AXP.h"

namespace AXP
{

//设置错误
void setLastError(AXP_ERRORS err, const char* desc="", ...);

//得到上一个错误
AXP_ERRORS AXP_API	getLastError(void);

//得到上一个错误的描述
const char* AXP_API	getLastErrorDesc(void); 

}