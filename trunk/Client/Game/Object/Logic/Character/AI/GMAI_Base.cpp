#include "StdAfx.h"
#include "GMAI_Base.h"
#include "GIException.h"

CAI_Base::CAI_Base(CObject_Character* pCharObj)
{
	KLAssert(pCharObj);

	m_pCharacterObj = pCharObj;
}

CAI_Base::~CAI_Base( VOID )
{
}
