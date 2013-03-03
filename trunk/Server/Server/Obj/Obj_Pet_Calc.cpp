// Obj_Pet_Calc.cpp

#include "stdafx.h"
#include "GameTable.h"

#include "Obj_Pet.h"

//ѱ�����ã�����ȼ�����*�������������ָ���/����������������������ָ����ֶ�/100����
INT CalcPetDomesticationMoney( INT nLevel, INT nHP, INT nHPMax, INT nHappiness )
{
	FLOAT fParam = g_PetDomesticationMoneyTbl.GetDomesticationMoney( nLevel );
	return (INT)(fParam *( (((FLOAT)nHPMax - (FLOAT)nHP)/(FLOAT)nHPMax) + ((100.f - (FLOAT)nHappiness)/100.f) ));
}
