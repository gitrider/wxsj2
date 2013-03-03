#include "StdAfx.h"
#include "../SMNetManager.h"
#include "KeyExchange.h"
#include "../SMAgentManager.h"
#include "../SMAgent.h"

//class CNetManager;
using namespace SMITH;
namespace Packets
{
	uint	KeyExchangeHandler::Execute(KeyExchange* pPacket, Player* pPlayer )
	{
		__ENTER_FUNCTION
		Agent* pAgent = (SMITH::Agent*)pPlayer;
		pAgent->getNetManager()->KeyAgreement(pPacket->GetKey(), pPacket->GetKeySize());

		if(FALSE==pAgent->getNetManager()->EnableEncrypto())
		{
			return PACKET_EXE_ERROR;
		}
		__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
	}
}
