
#include "EncoderManager.h"

static EncoderManager_T g_EncoderManager;

using namespace Encode_Decode;
Encoder_T const* Encoder_GetInstanceByID(INT nID)
{
	return g_EncoderManager.GetInstanceById(nID);
}

