
//-----------------------------------------------------------------------------
// 文件名 : TimerDoingManager.h
// 修改历史:
//创建:		王勇鸷
//-----------------------------------------------------------------------------

#ifndef __TIMER_DOING_MANAGER__
#define __TIMER_DOING_MANAGER__

#include "Type.h"
#include "TimeManager.h"
#include "GameTable.h"
//#include "SceneManager.h"

class Scene;

class TimerDoingManager
{
public:
	TimerDoingManager();
	~TimerDoingManager();

	BOOL				Init( );
	VOID				CleanUp( );

	BOOL				HeartBeat( UINT uTime=0 );			//逻辑处理

	//启动定时器事件
	VOID				StartupTimerDoing(INT TblIndex);

	//停止定时器事件
	VOID				StopTimerDoing(INT TblIndex);

	//检查系统定时器是否过期,return TRUE:未过期，FALSE：过期
	BOOL				CheckSystemTimerValidity(INT TblIndex);

	//判断定时器是否达到开始或结束时间
	BOOL				IsStartOrEndTime(INT TimeType,INT TblTime);

	VOID				SetActive(BOOL bRet,INT index) 
	{
		AssertEx(index>=0&&index<MAX_TIMER_DOING_NUM,"定时器激活标志下标越界");	
		m_IsActive[index] = bRet; 
	}

	BOOL				GetActive(INT index) 
	{
		AssertEx(index>=0&&index<MAX_TIMER_DOING_NUM,"定时器激活标志下标越界");	
		return m_IsActive[index];
	}

	VOID				SetLifetime(BOOL bRet,INT index) 
	{
		AssertEx(index>=0&&index<MAX_TIMER_DOING_NUM,"设置定时器生命期时下标越界");	
		m_IsLifetime[index] = bRet; 
	}

	VOID				SetTimerParam(INT val,INT TimerIndex,INT ParamIndex) 
	{
		AssertEx(TimerIndex>=0&&TimerIndex<MAX_TIMER_DOING_NUM,"设置定时器参数时下标越界");	
		AssertEx(ParamIndex>=0&&ParamIndex<MAX_TIEMR_PARAM_NUM,"设置定时器参数时下标越界");
		m_TimerParam[TimerIndex][ParamIndex] = val; 
	}

	INT 				GetTimerParam(INT TimerIndex,INT ParamIndex) 
	{
		AssertEx(TimerIndex>=0&&TimerIndex<MAX_TIMER_DOING_NUM,"设置定时器参数时下标越界");	
		AssertEx(ParamIndex>=0&&ParamIndex<MAX_TIEMR_PARAM_NUM,"设置定时器参数时下标越界");
		return m_TimerParam[TimerIndex][ParamIndex] ; 
	}

	VOID				SetReportType(BYTE type,INT index) 
	{
		AssertEx(index>=0&&index<MAX_TIMER_DOING_NUM,"设置定时器广播时下标越界");	
		m_ReportType[index] = type; 
	}

	VOID				SetScene(Scene* pScene){ m_pScene = pScene; }
	Scene*				GetScene()			{ return m_pScene; }
private:
	//各定时器事件使用的定时器
	CMyTimer			m_LogicTimer[MAX_TIMER_DOING_NUM];
	//管理器高度的定时器
	CMyTimer			m_ManagerTimer;

	//各定时器开启状态,由脚本控制
	BOOL				m_IsActive[MAX_TIMER_DOING_NUM]  ;

	//计时器是否在生命有效期内，由开始和结束时间控制
	BOOL				m_IsLifetime[MAX_TIMER_DOING_NUM];

	//定时器参数
	INT					m_TimerParam[MAX_TIMER_DOING_NUM][MAX_TIEMR_PARAM_NUM];

	//定时器活动广播类型
	BYTE				m_ReportType[MAX_TIMER_DOING_NUM];

	Scene				*m_pScene;

};


#endif

