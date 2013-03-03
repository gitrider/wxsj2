#include "stdafx.h"
#include "GCSouXiaUpdate.h"

using namespace Packets;

BOOL GCSouXiaUpdate::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read((CHAR*)(&m_nCurPos), sizeof(SHORT));
	iStream.Read((CHAR*)(&m_nType), sizeof(BYTE));
	iStream.Read((CHAR*)(&m_nSkillCount), sizeof(BYTE));
	for (int i=0; i<m_nSkillCount; ++i)
	{
		iStream.Read((CHAR*)(&m_nSkillIndex[i]), sizeof(SkillID_t));
	}	

	iStream.Read((CHAR*)(&m_nProductCount), sizeof(BYTE));
	for (int i=0; i<m_nProductCount; ++i)
	{
		iStream.Read((CHAR*)(&m_nProductIndex[i]), sizeof(SkillID_t));
	}	

	iStream.Read((CHAR*)(&m_nPetZhaoHuanCount), sizeof(BYTE));
	iStream.Read((CHAR*)(&m_nPetZhaoHuan), sizeof(SouXia_PetZhaoHuan));

	iStream.Read((CHAR*)(&m_nZuoJiZhaoHuanCount), sizeof(BYTE));
	iStream.Read((CHAR*)(&m_nZuoJiZhaoHuan), sizeof(SouXia_ZuojiZhaoHuan));

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCSouXiaUpdate::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write((CHAR*)(&m_nCurPos), sizeof(SHORT));
	oStream.Write((CHAR*)(&m_nType), sizeof(BYTE));
	oStream.Write((CHAR*)(&m_nSkillCount), sizeof(BYTE));

	for (int i=0; i<m_nSkillCount; ++i)
	{
		oStream.Write((CHAR*)(&m_nSkillIndex[i]), sizeof(SkillID_t));
	}	

	oStream.Write((CHAR*)(&m_nProductCount), sizeof(BYTE));
	for (int i=0; i<m_nProductCount; ++i)
	{
		oStream.Write((CHAR*)(&m_nProductIndex[i]), sizeof(SkillID_t));
	}	

	oStream.Write((CHAR*)(&m_nPetZhaoHuanCount), sizeof(BYTE));
	
	oStream.Write((CHAR*)(&m_nPetZhaoHuan), sizeof(SouXia_PetZhaoHuan));


	oStream.Write((CHAR*)(&m_nZuoJiZhaoHuanCount), sizeof(BYTE));

	oStream.Write((CHAR*)(&m_nZuoJiZhaoHuan), sizeof(SouXia_ZuojiZhaoHuan));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCSouXiaUpdate::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCSouXiaUpdateHandler::Execute( this, pPlayer );  //¿Í»§¶ËÐÞ¸Ä
	__LEAVE_FUNCTION

		return FALSE ;
}
