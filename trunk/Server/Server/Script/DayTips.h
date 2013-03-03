#pragma once
#include "GameTable.h"

class DayTips
{	
public:
	DayTips(void);
public:
	~DayTips(void);
public:
	// 是否在活动时间内
	BOOL IsMissionInTime( INT nMissionID );

	// 是否是今日活动
	BOOL IsTodayMission( INT nMissionID ); 

	// 获取一项活动的可接次数 
	INT	 GetMissionCount( INT nMisID );

	// 本周是本月第几周 
	INT	GetWeekNO();

	// 判断时间是否在时间段内
	INT	IsInTimeSegment( const VALID_TIME_SLICE pSlice, const tm& pCheck, int* pValue );

	// 公共条件检测
	BOOL CommonCheck( INT nMissionID, Obj_Human* pHuman );

	// 活动最后一次完成日期是否是今天
	INT  LastIsTodayRecord( INT time );

	// 获取转换后的今日日期
	INT GetTodayFormatDate();

	// 获取活动参数完成次数下标
	INT GetMisDataCountIndex( INT nMissionID );

	// 获取活动参数最后一次完成日期的下标
	INT GetMisDataDateIndex( INT nMissionID );

	// 获取活动个数
	INT GetMisCount();

	// 根据下标获取活动对应的任务ID
	INT GetMisIDByIndex( INT nIndex );

	static DayTips* GetMe(){ return s_dayTip; }

protected:
	static DayTips* s_dayTip;
};
