// GCPrivateInfo.cpp
// 
/////////////////////////////////////////////////////

#include "stdafx.h"
#include "CGApplyPrivateInfo.h"

BOOL CGApplyPrivateInfo::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_InfoType		), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_CharGUID		), sizeof(UINT));
	iStream.Read( (CHAR*)(&m_Age			), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_Sex			), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_BloodType		), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_YearAnimal		), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_Consella		), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_Province		), sizeof(BYTE));

	iStream.Read( (CHAR*)(&m_SchoolInfoLength	), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_JobInfoLength		), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_CityLength			), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_EmailInfoLength	), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_LuckWordLength		), sizeof(BYTE));

	iStream.Read( (CHAR*)(m_SchoolInfo	), sizeof(CHAR)*m_SchoolInfoLength	);
	iStream.Read( (CHAR*)(m_JobInfo		), sizeof(CHAR)*m_JobInfoLength		);
	iStream.Read( (CHAR*)(m_City		), sizeof(CHAR)*m_CityLength		);
	iStream.Read( (CHAR*)(m_EmailInfo	), sizeof(CHAR)*m_EmailInfoLength	);
	iStream.Read( (CHAR*)(m_LuckWord	), sizeof(CHAR)*m_LuckWordLength	);
	
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGApplyPrivateInfo::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_InfoType		), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_CharGUID		), sizeof(UINT));
	oStream.Write( (CHAR*)(&m_Age			), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_Sex			), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_BloodType		), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_YearAnimal	), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_Consella		), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_Province		), sizeof(BYTE));

	oStream.Write( (CHAR*)(&m_SchoolInfoLength	), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_JobInfoLength		), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_CityLength		), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_EmailInfoLength	), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_LuckWordLength	), sizeof(BYTE));

	oStream.Write( (CHAR*)(m_SchoolInfo	), sizeof(CHAR)*m_SchoolInfoLength	);
	oStream.Write( (CHAR*)(m_JobInfo	), sizeof(CHAR)*m_JobInfoLength		);
	oStream.Write( (CHAR*)(m_City		), sizeof(CHAR)*m_CityLength		);
	oStream.Write( (CHAR*)(m_EmailInfo	), sizeof(CHAR)*m_EmailInfoLength	);
	oStream.Write( (CHAR*)(m_LuckWord	), sizeof(CHAR)*m_LuckWordLength	);

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGApplyPrivateInfo::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGApplyPrivateInfoHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

