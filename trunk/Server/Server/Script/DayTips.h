#pragma once
#include "GameTable.h"

class DayTips
{	
public:
	DayTips(void);
public:
	~DayTips(void);
public:
	// �Ƿ��ڻʱ����
	BOOL IsMissionInTime( INT nMissionID );

	// �Ƿ��ǽ��ջ
	BOOL IsTodayMission( INT nMissionID ); 

	// ��ȡһ���ĿɽӴ��� 
	INT	 GetMissionCount( INT nMisID );

	// �����Ǳ��µڼ��� 
	INT	GetWeekNO();

	// �ж�ʱ���Ƿ���ʱ�����
	INT	IsInTimeSegment( const VALID_TIME_SLICE pSlice, const tm& pCheck, int* pValue );

	// �����������
	BOOL CommonCheck( INT nMissionID, Obj_Human* pHuman );

	// ����һ����������Ƿ��ǽ���
	INT  LastIsTodayRecord( INT time );

	// ��ȡת����Ľ�������
	INT GetTodayFormatDate();

	// ��ȡ�������ɴ����±�
	INT GetMisDataCountIndex( INT nMissionID );

	// ��ȡ��������һ��������ڵ��±�
	INT GetMisDataDateIndex( INT nMissionID );

	// ��ȡ�����
	INT GetMisCount();

	// �����±��ȡ���Ӧ������ID
	INT GetMisIDByIndex( INT nIndex );

	static DayTips* GetMe(){ return s_dayTip; }

protected:
	static DayTips* s_dayTip;
};
