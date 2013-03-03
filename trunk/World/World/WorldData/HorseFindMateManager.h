/********************************************************************
	created:	2008/05/30
	created:	30:5:2008   17:19
	filename: 	d:\prj\src\GLServer\World\WorldData\HorseFindMateManager.h
	file path:	d:\prj\src\GLServer\World\WorldData
	file base:	HorseFindMateManager
	file ext:	h
	author:		wuwenbin
	
	purpose:	�������ģ��
*********************************************************************/

#ifndef _HORSEFINDMATEMANAGER_H_
#define _HORSEFINDMATEMANAGER_H_

#include "Type.h"
#include "DB_Struct.h"
#include "GameStruct_Query.h"
#include "GameUtil.h"
#include "Array_Base.h"

class HorseFindMateInfoTable
{
public:
	//////////////////////////////////////////////////////////////////////////
	//typedef std::vector<HorseFindMateInfo> HorseFindMateInfoVector;
	typedef Array_Base<HorseFindMateInfo> HorseFindMateInfoVector;

	enum {MAX_HORSEFINDMATEINFO_COUNT = 10};

	HorseFindMateInfoTable();
	~HorseFindMateInfoTable();

	INT Size() const {return (INT)(m_HorseFindMateInfoVector.GetSize());}
	const HorseFindMateInfo& GetEntry(INT idx) const {return m_HorseFindMateInfoVector[idx];}
	BOOL Add(HorseFindMateInfo& entry);
	BOOL Remove(const PET_GUID_t& HorseGuid);
	BOOL Find(const CHAR* szHorseName, 
		INT HorseLevel, 
		INT HorseSex, 
		INT HorseGrowRate,
		INT iPostion,		 /// �������ʼPos
		BOOL& bContinue,	 /// �Ƿ�����һҳ
		INT& iReturnPos,	 /// ���صĽ���Pos
		HorseFindMateInfoVector& Output,	/// ���صĽ����
		GUID_t exceptGuid = INVALID_GUID	/// ���ų������GUID
		) const;

	BOOL Find(const PET_GUID_t& HorseGuid) const;
	BOOL Find(GUID_t PlayerGuid, HorseFindMateInfo& Output) const;

protected:
	//BOOL FindByHorseName(const CHAR* szHorseName, 
	//	const HorseFindMateInfoVector& Input, 
	//	HorseFindMateInfoVector& Output) const;

	//BOOL FindByHorseLevel(INT HorseLevel, 
	//	const HorseFindMateInfoVector& Input, 
	//	HorseFindMateInfoVector& Output) const;

	//BOOL FindByHorseSex(INT HorseSex, 
	//	const HorseFindMateInfoVector& Input, 
	//	HorseFindMateInfoVector& Output) const;

	//BOOL FindByHorseGrowRate(INT HorseGrowRate, 
	//	const HorseFindMateInfoVector& Input, 
	//	HorseFindMateInfoVector& Output) const;

	

private:
	HorseFindMateInfoVector m_HorseFindMateInfoVector;
};

class HorseFindMateInfoManager
{
public:
	~HorseFindMateInfoManager(){}

	static HorseFindMateInfoManager& GetInstance(){
		static HorseFindMateInfoManager instance;
		return instance;
	}

	VOID HeartBeat(UINT uTime);

	BOOL IssueHorseFindMateInfo(const HorseFindMateInfo& entry);
	BOOL CancelHorseFindMateInfo(const PET_GUID_t& HorseGuid);

	BOOL SearchHorseFindMateInfo(
		const CHAR* szHorseName, INT HorseLevel, INT HorseSex, INT HorseGrowRate, /// ��������
		INT iPostion,		 /// �������ʼPos
		BOOL& bContinue,	 /// �Ƿ�����һҳ
		INT& iReturnPos,	 /// ���صĽ���Pos
		HorseFindMateInfoTable::HorseFindMateInfoVector& Output /// ���صĽ����
		);

	BOOL QueryHorseFindMateInfo(GUID_t PlayerGuid, INT iMaxPos,
		INT& iReturnPos,
		HorseFindMateInfoTable::HorseFindMateInfoVector& Output,
		BOOL& bContinue) const;
		
	BOOL OnHorseChangeState(const PET_GUID_t& HorseGuid, INT iStateChangeType);

	//------------------------------------------------------
	const HorseFindMateInfoTable& GetHorseFindMateInfoTable() const{
		return m_HorseFindMateInfoTable;
	}

private:
	HorseFindMateInfoManager();
	HorseFindMateInfoTable m_HorseFindMateInfoTable;

	CMyTimer m_UpdateTimer;
};

#endif

