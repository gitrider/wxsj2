/********************************************************************
	created:	2008/03/31
	created:	31:3:2008   10:26
	filename: 	MapServer\Server\Obj\Horse\DropFromMonster.h
	file path:	MapServer\Server\Obj\Horse
	file base:	DropFromMonster
	file ext:	h
	author:		Richard
	
	purpose:	������������ĵ������
*********************************************************************/

#ifndef DropFromMonster_h__
#define DropFromMonster_h__

#include "Type.h"
#include "DB_Struct.h"
#include "HorseStruct.h"
#include "GameTable.h"

class DropFromMonster
{
public:
	DropFromMonster();
	~DropFromMonster();

	//************************************
	// Method:    DropHorse
	// FullName:  DropFromMonster::DropHorse
	// Access:    public 
	// Returns:   INT ����ʵ�ʵĵ�������
	// Qualifier:
	// Parameter: INT iDropID
	// ���ݵ���ID���������
	//************************************
	static INT  DropHorse( INT iDropID, Obj_Character* pObj );

	//************************************
	// Method:    DropHorseByRate
	// FullName:  DropFromMonster::DropHorseByRate
	// Access:    public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: _HORSE_DROP & oHorseDrop
	// Parameter: INT i
	// Parameter: Obj_Character * pObj
	// Parameter: INT iMaxDropNum
	// ���ݸ��ʼ��������
	//************************************
	static VOID DropHorseByRate( _HORSE_DROP& oHorseDrop, INT i, Obj_Character* pObj, INT iMaxDropNum );
	//************************************
	// Method:    CalcDropRate
	// FullName:  DropFromMonster::CalcDropRate
	// Access:    public 
	// Returns:   INT
	// Qualifier:
	// Parameter: _HORSE_DROP & oHorseDrop
	// Parameter: INT i
	// ����������
	//************************************
	static INT  CalcDropRate( _HORSE_DROP& oHorseDrop, INT i );
	//************************************
	// Method:    DropHorseImp
	// FullName:  DropFromMonster::DropHorseImp
	// Access:    public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: INT iMaxDropNum
	// Parameter: Obj_Character * pObj
	// Parameter: _HORSE_DROP & oHorseDrop
	// Parameter: INT i
	// ����������ID
	//************************************
	static VOID DropHorseImp( INT iMaxDropNum, Obj_Character* pObj, _HORSE_DROP& oHorseDrop, INT i );
	
	//************************************
	// Method:    GetDropID
	// FullName:  DropFromMonster::GetDropID
	// Access:    public static 
	// Returns:   
	// Qualifier:
	// Parameter: INT iIndex
	// ��ȡ�������ID
	//************************************
	static INT GetDropID( INT iIndex );
private:
	static VOID Cleanup();

private:
	static INT m_aDropID[MaxHorseDropGroupNum];
	static INT m_iCurrentDropCount;
};

#endif // DropFromMonster_h__