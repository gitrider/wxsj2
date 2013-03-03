#ifndef _HorseFindMateModule_H_
#define _HorseFindMateModule_H_

#include "Type.h"
#include "DB_Struct.h"

class Obj_Human;
struct HorseFindMateInfoPerPage;

class HorseFindMateModule
{
public:
	~HorseFindMateModule() {}
	static HorseFindMateModule& GetInstance() {
		static HorseFindMateModule instance;
		return instance;
	}

	BOOL IssueHorseFindMateInfo(Obj_Human* pOwner, const _PET_DB* pHorseDB, const CHAR* pszLeaveWords) const;
	BOOL CancelHorseFindMateInfo(Obj_Human* pOwner, const _PET_DB* pHorseDB) const;
	BOOL SearchHorseFindMateInfo(Obj_Human* pOwner, const CHAR* szHorseName, INT HorseLevel, INT HorseSex, INT HorseGrowRate, INT iMaxPos) const;
	BOOL QueryHorseFindMateInfo(Obj_Human* pOwner, INT iPosition) const;

	VOID OnHorseChangeState(Obj_Human* pOwner, const PET_GUID_t& HorseGuid, INT StateType);
	VOID OnGetQueryResults(Obj_Human* pOwner, const HorseFindMateInfoPerPage& Result);

protected:
	HorseFindMateModule() {}

	BOOL CanIssueHorseFindMateInfo(Obj_Human* pOwner, const _PET_DB* pHorseDB) const;

private:
};

#endif

