#include "StdAfx.h"
#include "AXPErrors.h"

namespace AXP
{

static AXP_ERRORS	g_LastError = AXP_ERR_SUCCESS;
static CHAR			g_strLastErrorDesc[1024] = {0};

//设置错误
void setLastError(AXP_ERRORS err, const char* desc, ...)
{
	g_LastError = err;

	g_strLastErrorDesc[0] = 0;

	va_list ptr; va_start(ptr, desc);
	_vsnprintf(g_strLastErrorDesc, 1024, desc, ptr);
	va_end(ptr);
}

//得到上一个错误
AXP_ERRORS getLastError(void)
{
	return g_LastError;
}

const char* getLastErrorDesc(void)
{
	return g_strLastErrorDesc;
}

}