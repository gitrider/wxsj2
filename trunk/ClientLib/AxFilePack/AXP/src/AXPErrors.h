/*
		Axia|FilePack
			������
*/

#pragma once

#include "AXP.h"

namespace AXP
{

//���ô���
void setLastError(AXP_ERRORS err, const char* desc="", ...);

//�õ���һ������
AXP_ERRORS AXP_API	getLastError(void);

//�õ���һ�����������
const char* AXP_API	getLastErrorDesc(void); 

}