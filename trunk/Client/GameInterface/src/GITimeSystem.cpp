
#include "StdAfx.h"
#include "GITimeSystem.h"
#include "GIException.h"
#include "GIProfile.h"



/// ������ʹ��ʱ�亯��ָ��,��ֹ�û�ʹ�ü򵥵ı��ٳ��ָ���ϵͳ�ٶ�
typedef DWORD (__stdcall *PFNTIMEGETTIME)(VOID);
static PFNTIMEGETTIME	g_ptimeGetTime	= ::timeGetTime;


KL_IMPLEMENT_DYNAMIC(tTimeSystem, GETCLASS(tNode));


VOID tTimeSystem::Initial(VOID*)
{
	m_fRealTime = 0.0f;
	m_fLoopTime = 0.0f;
	m_fDeltaTime= 0.0f;
	m_fFPS		= 0.0f;
	m_dwTickCount = 0;

	m_dwTimeNow = (*g_ptimeGetTime)();

	m_serverTime = 0;
	m_localTime  = 0;
}

UINT tTimeSystem::CalSubTime(UINT timeStart, UINT timeEnd)
{
	if(timeEnd < timeStart) 
		return ((UINT)0XFFFFFFFF-timeStart) + timeEnd;
	else 
		return timeEnd-timeStart;
}

VOID tTimeSystem::Tick(VOID)
{
	//-------------------------------------------------------------
	static UINT	s_dwOldTime = 0;		//�ϴ�ִ��Tick��ʱ��

	//-------------------------------------------------------------
	//������һ
	m_dwTickCount++;

	//-------------------------------------------------------------
	//��¼��ǰʱ��
	m_dwTimeNow = (*g_ptimeGetTime)();
	//��¼����һ�������ѵ�ʱ�䣬 ���ǿ�ʱ���ڵ�����(49.71��)
	UINT subTime = CalSubTime(s_dwOldTime, m_dwTimeNow);

	m_fDeltaTime = (FLOAT)subTime;

	//-------------------------------------------------------------
	//����ÿ�������ѵ�ƽ��ʱ��
	const UINT	MAXTIME_CAL_LOOPTIME	= 20;	//ÿ20ms����һ��ÿ�廨�ѵ�ʱ��
	{
		static UINT	s_dwLoopCnt = 0;		//���ۼƼ���
		static UINT	s_dwLoopHop = 0;		//ʱ���ۼƼ���
		s_dwLoopCnt++;
		s_dwLoopHop += subTime;
		if( s_dwLoopHop > MAXTIME_CAL_LOOPTIME )		
		{
			m_fLoopTime = (s_dwLoopHop/(FLOAT)s_dwLoopCnt)/1000.0f;

			s_dwLoopCnt=0;
			s_dwLoopHop=0;
		}
	}

	//-------------------------------------------------------------
	//��������
	const UINT	MAXTIME_CAL_FPS			= 1000;	//ÿ1�����һ��fps
	{
		static UINT	s_dwLoopCnt = 0;		//���ۼƼ���
		static UINT	s_dwLoopHop = 0;		//ʱ���ۼƼ���
		s_dwLoopCnt++;
		s_dwLoopHop += subTime;
		if( s_dwLoopHop > MAXTIME_CAL_FPS )		
		{
			m_fFPS		= (FLOAT)(s_dwLoopCnt*1000)/s_dwLoopHop;

			s_dwLoopCnt=0;
			s_dwLoopHop=0;
		}
	}

	s_dwOldTime = m_dwTimeNow;
	m_fRealTime += m_fLoopTime;

	//--------------------------------------------------------
	//Profile: ����в�ѯ�����Ҷ�ջ��
	KLProfile_Tick();
}

VOID tTimeSystem::SetServerTime( time_t sTime ) 
{
	m_serverTime = sTime;
	time( &m_localTime );
}

time_t tTimeSystem::GetServerTime(VOID) 
{
	if( m_serverTime <= 0 )
		return 0;

	time_t curTime;
	time( &curTime );

	return m_serverTime + curTime - m_localTime;
}