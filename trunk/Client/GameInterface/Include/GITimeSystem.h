
/**	GITimeSystem.h
 */

#pragma once


#include "GINode.h"

#include <time.h>

/** ʱ��ϵͳ,����ϵͳʱ���ϵļ�������ٿ���
 */
class KYLIN_API tTimeSystem : public tNode
{
public:

	//ȡ�����20ms����ÿ��ƽ�����ѵ�ʱ��
	FLOAT			GetLoopTime(VOID) const { return m_fLoopTime; }
	//ȡ����һ�����ѵ�ʱ��
	FLOAT			GetDeltaTime(VOID) const { return m_fDeltaTime; }
	//ȡ�ø���m_fLoopTime�ۻ���ϵͳ����ʱ��
	FLOAT			GetRealTime(VOID) const { return m_fRealTime; }
	//ȡ�õ�ǰʱ��
	UINT			GetTimeNow(VOID) const { return m_dwTimeNow; }
	//ȡ������
	FLOAT			GetFPS(VOID) const { return m_fFPS; }
	//����ʱ���,����ʱ����(49.71��)
	UINT			CalSubTime(UINT timeStart, UINT timeEnd);
	//�õ�����
	UINT			GetTickCount(VOID) { return m_dwTickCount; }
	
	VOID			SetServerTime( time_t sTime );
	time_t			GetServerTime(VOID);

public:

	//�ڵ��ʼ��
	virtual VOID						Initial(VOID*);
	//�߼���ѭ����
	virtual VOID						Tick(VOID);

private:

	UINT		m_dwTimeNow;		//��ǰʱ��
	FLOAT		m_fLoopTime;		//���20ms����ÿ��ƽ�����ѵ�ʱ��
	FLOAT		m_fDeltaTime;		//��һ�������ѵ�ʱ��
	FLOAT		m_fRealTime;		//����m_fLoopTime�ۻ���ϵͳ����ʱ��
	FLOAT		m_fFPS;				//��ǰ������
	UINT		m_dwTickCount;		//������

	time_t		m_serverTime;		// ������У��ʱ��
	time_t		m_localTime;		// �յ�������У��ʱ��ʱ����ʱ��


	KL_DECLARE_DYNAMIC(tTimeSystem);

};