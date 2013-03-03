
#ifndef _GI_PROFILE_INCLUDE_
#define _GI_PROFILE_INCLUDE_


#include "GIBasicType.h"



namespace AxProfile
{

	void KYLIN_API AxProfile_SamplingBegin(void);
	void KYLIN_API AxProfile_SamplingEnd(void);

	void KYLIN_API AxProfile_PushNode(const char* szName, const char* szParent=0);
	void KYLIN_API AxProfile_PopNode(const char* szName);

	void KYLIN_API AxProfile_Report(void);

}


struct AUTO_SAMPLING
{
	AUTO_SAMPLING()		{ AxProfile::AxProfile_SamplingBegin(); }
	~AUTO_SAMPLING()	{ AxProfile::AxProfile_SamplingEnd(); }
};



#endif	// _GI_PROFILE_INCLUDE_