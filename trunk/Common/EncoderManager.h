#ifndef ENCODERMANAGER_H
#define ENCODERMANAGER_H

#include "Encoder.h"
#include "Encoder/Encoder_XOR.h"
#include "InstanceManager.h"

using namespace Encode_Decode;
typedef InstanceManager_T<Encoder_T, Encoder_T::MAX_ID+1> EncoderManager_T;

template<>
	inline bool InstanceManager_T<Encoder_T, Encoder_T::MAX_ID+1>::Init(VOID)
{
	REGISTER_INSTANCE(EncoderXOR_T);
	return TRUE;
}
Encoder_T const* Encoder_GetInstanceByID(INT nID);
#endif //ENCODERMANAGER_H
