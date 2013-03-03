
#ifndef __TIMEMANAGER_H__
#define __TIMEMANAGER_H__

#include "Type.h"
#ifdef __LINUX__
#include <sys/utsname.h>
#include <sys/time.h>
#endif

class TimeManager
{
public :
	TimeManager( ) ;
	~TimeManager( ) ;

	BOOL			Init( ) ;

	//��ǰʱ�����ֵ����ʼֵ����ϵͳ��ͬ������
	//���ص�ֵΪ��΢�λ��ʱ��ֵ
	uint			CurrentTime( ) ;
	//ֱ�ӷ��أ�������ϵͳ�ӿ�
	uint			CurrentSavedTime( ){ return m_CurrentTime ; } ;
	//ȡ�÷������˳�������ʱ��ʱ�����ֵ
	uint			StartTime( ){ return m_StartTime ; } ;

	//����ǰ��ϵͳʱ���ʽ����ʱ���������
	VOID			SetTime( ) ;

	// �õ���׼ʱ��
	time_t			GetANSITime( );

	// �õ�tm�ṹ��ʱ�� 20100723 BLL
	tm  TimeManager::GetFormatTime( );

public :
	//***ע�⣺
	//���½ӿڵ���û��ϵͳ���ã�ֻ���ʱ��������ڵ�����
	//

	//ȡ������ʱ��ʱ��ġ��ꡢ�¡��ա�Сʱ���֡��롢���ڵ�ֵ��
	INT				GetYear( ){ return m_TM.tm_year+1900 ; } ;	//[1900,????]
	INT				GetMonth( ){ return m_TM.tm_mon ; } ;		//[0,11]
	INT				GetDay( ){ return m_TM.tm_mday ; } ;		//[1,31]
	INT				GetHour( ){ return m_TM.tm_hour ; } ;		//[0,23]
	INT				GetMinute( ){ return m_TM.tm_min ; } ;		//[0,59]
	INT				GetSecond( ){ return m_TM.tm_sec ; } ;		//[0,59]
	//ȡ�õ�ǰ�����ڼ���0��ʾ�������죬1��6��ʾ������һ��������
	uint			GetWeek( ){ return m_TM.tm_wday ; } ;
	//����ǰ��ʱ�䣨�ꡢ�¡��ա�Сʱ���֣�ת����һ��UINT����ʾ
	//���磺0,507,211,233 ��ʾ "2005.07.21 12:33"
	UINT			Time2DWORD( ) ;
	VOID			DWORD2Time( UINT Date, tm* TM ) ;
	//ȡ������DWORDTime��ʱ����λ�����ӣ�, Ret = Date2-Data1
	UINT			DiffDWORDTime( UINT Date1, UINT Date2 ) ;
	// �Ƚ������Ĳ��
	INT				DiffDayCount(time_t uPrevANSITime, time_t uNextANSITime);
	//�Ƚ��ǲ���ͬһ��
	INT				IsDiffDay(time_t uPrevANSITime, time_t uNextANSITime);

	//���磺20070403��ʾ2007��4��3��
	UINT			Time2Day( ) ;
	//ȡ�õ�ǰ������[4bit 0-9][4bit 0-11][5bit 0-30][5bit 0-23][6bit 0-59][6bit 0-59]
	UINT			CurrentDate( ) ;
	//ȡ�÷����������������ʱ�䣨���룩
	UINT			RunTime( ){ 
		CurrentTime( ) ;
		return (m_CurrentTime-m_StartTime);  
	} ;
	//ȡ����������ʱ���ʱ����λ�����룩, Ret = Date2-Data1
	UINT			DiffTime( UINT Date1, UINT Date2 ) ;
	//��һ��UINT������ת����һ��tm�ṹ
	VOID			ConvertUT( UINT Date, tm* TM ) ;
	//��һ��tm�ṹת����һ��UINT������
	VOID			ConvertTU( tm* TM, UINT& Date ) ;
	//ȡ������Ϊ��λ��ʱ��ֵ, ǧλ��������ݣ�������λ����ʱ�䣨������
	UINT			GetDayTime( ) ;
	//ȡ����СʱΪ��λ��ʱ��ֵ��
	//"12723��ʾ����ȵ�127���5(23/4)��ĵ�3(23%4)����ʱ��"
	UINT			GetHourTime( ) ;

	//ȡ������Ϊ��λ��ʱ��ֵ, ǧλ��������ݣ�������λ����ʱ�䣨������
	UINT			GetWeekTime( ) ;


	//ȡ��World Time
	WORLD_TIME		GetWorldTime() { return m_WorldTime;};
	VOID			SetWorldTime( WORLD_TIME tWorldTime ) { m_WorldTime = tWorldTime;};

public :
	uint			m_StartTime ;
	uint			m_CurrentTime ;
	time_t			m_SetTime ;
	tm				m_TM ;
	WORLD_TIME		m_WorldTime;
#ifdef __LINUX__
	struct timeval _tstart, _tend;
	struct timezone tz;
#endif



};

extern TimeManager* g_pTimeManager ;

extern UINT g_FileNameFix_Last ;


#endif
