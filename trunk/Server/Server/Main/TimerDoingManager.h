
//-----------------------------------------------------------------------------
// �ļ��� : TimerDoingManager.h
// �޸���ʷ:
//����:		�����
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

	BOOL				HeartBeat( UINT uTime=0 );			//�߼�����

	//������ʱ���¼�
	VOID				StartupTimerDoing(INT TblIndex);

	//ֹͣ��ʱ���¼�
	VOID				StopTimerDoing(INT TblIndex);

	//���ϵͳ��ʱ���Ƿ����,return TRUE:δ���ڣ�FALSE������
	BOOL				CheckSystemTimerValidity(INT TblIndex);

	//�ж϶�ʱ���Ƿ�ﵽ��ʼ�����ʱ��
	BOOL				IsStartOrEndTime(INT TimeType,INT TblTime);

	VOID				SetActive(BOOL bRet,INT index) 
	{
		AssertEx(index>=0&&index<MAX_TIMER_DOING_NUM,"��ʱ�������־�±�Խ��");	
		m_IsActive[index] = bRet; 
	}

	BOOL				GetActive(INT index) 
	{
		AssertEx(index>=0&&index<MAX_TIMER_DOING_NUM,"��ʱ�������־�±�Խ��");	
		return m_IsActive[index];
	}

	VOID				SetLifetime(BOOL bRet,INT index) 
	{
		AssertEx(index>=0&&index<MAX_TIMER_DOING_NUM,"���ö�ʱ��������ʱ�±�Խ��");	
		m_IsLifetime[index] = bRet; 
	}

	VOID				SetTimerParam(INT val,INT TimerIndex,INT ParamIndex) 
	{
		AssertEx(TimerIndex>=0&&TimerIndex<MAX_TIMER_DOING_NUM,"���ö�ʱ������ʱ�±�Խ��");	
		AssertEx(ParamIndex>=0&&ParamIndex<MAX_TIEMR_PARAM_NUM,"���ö�ʱ������ʱ�±�Խ��");
		m_TimerParam[TimerIndex][ParamIndex] = val; 
	}

	INT 				GetTimerParam(INT TimerIndex,INT ParamIndex) 
	{
		AssertEx(TimerIndex>=0&&TimerIndex<MAX_TIMER_DOING_NUM,"���ö�ʱ������ʱ�±�Խ��");	
		AssertEx(ParamIndex>=0&&ParamIndex<MAX_TIEMR_PARAM_NUM,"���ö�ʱ������ʱ�±�Խ��");
		return m_TimerParam[TimerIndex][ParamIndex] ; 
	}

	VOID				SetReportType(BYTE type,INT index) 
	{
		AssertEx(index>=0&&index<MAX_TIMER_DOING_NUM,"���ö�ʱ���㲥ʱ�±�Խ��");	
		m_ReportType[index] = type; 
	}

	VOID				SetScene(Scene* pScene){ m_pScene = pScene; }
	Scene*				GetScene()			{ return m_pScene; }
private:
	//����ʱ���¼�ʹ�õĶ�ʱ��
	CMyTimer			m_LogicTimer[MAX_TIMER_DOING_NUM];
	//�������߶ȵĶ�ʱ��
	CMyTimer			m_ManagerTimer;

	//����ʱ������״̬,�ɽű�����
	BOOL				m_IsActive[MAX_TIMER_DOING_NUM]  ;

	//��ʱ���Ƿ���������Ч���ڣ��ɿ�ʼ�ͽ���ʱ�����
	BOOL				m_IsLifetime[MAX_TIMER_DOING_NUM];

	//��ʱ������
	INT					m_TimerParam[MAX_TIMER_DOING_NUM][MAX_TIEMR_PARAM_NUM];

	//��ʱ����㲥����
	BYTE				m_ReportType[MAX_TIMER_DOING_NUM];

	Scene				*m_pScene;

};


#endif

