#include "stdafx.h" 
//#include "MiBao.h"
//
////密保表名a1,a2.....g6,g7
//CHAR*	g_MiBaoKeyTable[MIBAO_TABLE_ROW_MAX*MIBAO_TABLE_COLUMN_MAX]=
//{
//	"a1","b1","c1","d1","e1","f1","g1",
//	"a2","b2","c2","d2","e2","f2","g2",
//	"a3","b3","c3","d3","e3","f3","g3",
//	"a4","b4","c4","d4","e4","f4","g4",
//	"a5","b5","c5","d5","e5","f5","g5",
//	"a6","b6","c6","d6","e6","f6","g6",
//	"a7","b7","c7","d7","e7","f7","g7"
//};
//
////密保表名11,12.....76,77
//CHAR*	g_MiBaoKeyTable1[MIBAO_TABLE_ROW_MAX*MIBAO_TABLE_COLUMN_MAX]=
//{
//	"11","21","31","41","51","61","71",
//	"12","22","32","42","52","62","72",
//	"13","23","33","43","53","63","73",
//	"14","24","34","44","54","64","74",
//	"15","25","35","45","55","65","75",
//	"16","26","36","46","56","66","76",
//	"17","27","37","47","57","67","77"
//};
//
////MIBAO
//BOOL		MIBAO::InitMiBaoGroupKey(MiBaoGroup& gp)
//{
//__ENTER_FUNCTION
//	gp.CleanUp();
//	BYTE rMax = sizeof(g_MiBaoKeyTable)/sizeof(CHAR*);
//
//	BYTE nNum = 0;
//	while(nNum < MIBAOUNIT_NUMBER)
//	{
//		BYTE rRow = rand() % MIBAO_TABLE_ROW_MAX;
//		BYTE rColumn = rand() % MIBAO_TABLE_COLUMN_MAX;
//		
//		if(FALSE == gp.IsAlreadyHaveUnit(rRow,rColumn))
//		{
//			MiBaoUint&	mu = gp.unit[nNum];
//			mu.row = rRow;
//			mu.column = rColumn;
//
//			INT kIndex = rRow+rColumn*MIBAO_TABLE_ROW_MAX;
//			//INT kIndex = rRow*MIBAO_TABLE_COLUMN_MAX+rColumn;
//			Assert(kIndex >= 0 && kIndex < MIBAO_TABLE_ROW_MAX*MIBAO_TABLE_COLUMN_MAX);
//
//			strncpy(mu.key,g_MiBaoKeyTable[kIndex],MIBAOUNIT_NAME_LENGTH);
//
//			nNum++;
//		}
//	}
//	return TRUE;
//__LEAVE_FUNCTION
//	return FALSE;
//}
//
//BOOL		MIBAO::InitMiBaoGroupKey1(MiBaoGroup& gp)
//{
//__ENTER_FUNCTION
//	gp.CleanUp();
//	BYTE rMax = sizeof(g_MiBaoKeyTable1)/sizeof(CHAR*);
//
//	BYTE nNum = 0;
//	while(nNum < MIBAOUNIT_NUMBER)
//	{
//		BYTE rRow = rand() % MIBAO_TABLE_ROW_MAX;
//		BYTE rColumn = rand() % MIBAO_TABLE_COLUMN_MAX;
//
//		if(FALSE == gp.IsAlreadyHaveUnit(rRow,rColumn))
//		{
//			MiBaoUint&	mu = gp.unit[nNum];
//			mu.row = rRow;
//			mu.column = rColumn;
//
//			INT kIndex = rRow+rColumn*MIBAO_TABLE_ROW_MAX;
//			//INT kIndex = rRow*MIBAO_TABLE_COLUMN_MAX+rColumn;
//			Assert(kIndex >= 0 && kIndex < MIBAO_TABLE_ROW_MAX*MIBAO_TABLE_COLUMN_MAX);
//
//			strncpy(mu.key,g_MiBaoKeyTable1[kIndex],MIBAOUNIT_NAME_LENGTH);
//
//			nNum++;
//		}
//	}
//	return TRUE;
//__LEAVE_FUNCTION
//	return FALSE;
//}