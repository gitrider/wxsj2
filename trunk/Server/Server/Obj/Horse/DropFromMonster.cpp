/********************************************************************
	created:	2008/03/31
	created:	31:3:2008   10:29
	filename: 	MapServer\Server\Obj\Horse\DropFromMonster.cpp
	file path:	MapServer\Server\Obj\Horse
	file base:	DropFromMonster
	file ext:	cpp
	author:		Richard
	
	purpose:	
*********************************************************************/


#include "stdafx.h"
#include "DropFromMonster.h"
#include "Scene.h"

INT DropFromMonster::m_aDropID[MaxHorseDropGroupNum];
INT DropFromMonster::m_iCurrentDropCount;

DropFromMonster::DropFromMonster()
{
	Cleanup();
}

DropFromMonster::~DropFromMonster()
{

}

VOID DropFromMonster::Cleanup()
{
	memset( m_aDropID, -1, sizeof(m_aDropID) );
	m_iCurrentDropCount = 0;
}

INT DropFromMonster::DropHorse( INT iDropID, Obj_Character* pObj )
{
	__ENTER_FUNCTION
	Assert( iDropID <= g_HorseDrop.m_iCount && "DropFromMonster::DropHorse" );
	if( iDropID == INVALID_ID )
	{
		return 0;
	}
	Cleanup();
	_HORSE_DROP& oHorseDrop = g_HorseDrop.m_aHorse_drop[iDropID];
	INT iMaxDropNum		= oHorseDrop.m_iMaxDropNum;
	//所有掉落组的循环
	for( INT i=0; i<MaxHorseDropGroupNum; ++i )
	{
		//每组最大掉落次数的循环
		for( INT iDrop=0; iDrop<oHorseDrop.m_aDropGroup[i].m_iGroupMaxDropTimes; ++iDrop )
		{
			DropHorseByRate(oHorseDrop, i, pObj, iMaxDropNum);
			if( m_iCurrentDropCount >= iMaxDropNum )
			{
				return m_iCurrentDropCount;
			}
		}
	}

	return m_iCurrentDropCount;
	__LEAVE_FUNCTION
	return 0;
}

VOID DropFromMonster::DropHorseImp( INT iMaxDropNum, Obj_Character* pObj, _HORSE_DROP& oHorseDrop, INT i )
{
	__ENTER_FUNCTION
	//随机选择掉落马的对应马基础表的ID
	if( oHorseDrop.m_aDropGroup[i].m_iHorseIDCount == 0 )
	{
		return;
	}
	INT iIDIndex = pObj->getScene()->GetRand100() % oHorseDrop.m_aDropGroup[i].m_iHorseIDCount;
	if( iIDIndex >= oHorseDrop.m_aDropGroup[i].m_iHorseIDCount )
	{
		iIDIndex -= 1;
	}
	m_aDropID[m_iCurrentDropCount] = oHorseDrop.m_aDropGroup[i].m_aGroupHorseID[iIDIndex];
	++m_iCurrentDropCount;
	__LEAVE_FUNCTION
}

VOID DropFromMonster::DropHorseByRate( _HORSE_DROP& oHorseDrop, INT i, Obj_Character* pObj, INT iMaxDropNum )
{
	__ENTER_FUNCTION

	//计算掉落概率
	INT Importance = CalcDropRate(oHorseDrop, i);

	//检查掉落概率，如果符合则进行掉落
	if( pObj->getScene()->GetRand100() <= Importance )
	{
		DropHorseImp(iMaxDropNum, pObj, oHorseDrop, i);
	}
	__LEAVE_FUNCTION
}

INT DropFromMonster::CalcDropRate( _HORSE_DROP& oHorseDrop, INT i )
{
	__ENTER_FUNCTION
	return (oHorseDrop.m_iMonsterImportance * 100) / oHorseDrop.m_aDropGroup[i].m_iGroupImportance;
	__LEAVE_FUNCTION
}

INT DropFromMonster::GetDropID( INT iIndex )
{
	__ENTER_FUNCTION
	Assert( iIndex < m_iCurrentDropCount );
	return m_aDropID[iIndex];
	__LEAVE_FUNCTION
	return INVALID_ID;
}