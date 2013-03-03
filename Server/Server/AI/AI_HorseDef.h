/********************************************************************
	created:	2008/03/18
	created:	18:3:2008   10:49
	filename: 	MapServer\Server\Ai\AI_HorseDef.h
	file path:	MapServer\Server\Ai
	file base:	AI_HorseDef
	file ext:	h
	author:		Richard
	
	purpose:	
*********************************************************************/

#ifndef AI_HorseDef_h__
#define AI_HorseDef_h__

#define HORSE_REFOLLOW_DISTSQR_A 9.f
#define HORSE_REFOLLOW_DISTSQR_B 64.f
#define HORSE_REFOLLOW_DISTSQR_C 225.f
#define HORSE_MAX_ATTACK_DIST 8.f
#define HORSE_SCAN_IMPACTOFOWNER_INTERVAL (5000)

enum AI_HORSE_STATE
{
	AI_HORSE_STATE_INVALID	= -1,	// INVALID	
	AI_HORSE_IDLE			= 0,	// ÐÝÏÐ 
};

#endif // AI_HorseDef_h__