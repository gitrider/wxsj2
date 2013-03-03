#pragma once

#include "TDDebuger.h"
class CDlgDebuger;
class TDDebuger : public tDebuger
{
public:
	//�ڵ��ʼ��
	virtual void	Initial(void*);
	//�߼���ѭ����
	virtual void	Tick(void);
	//�ͷ��Լ�����ӵ�е���Դ
	virtual void	Release(void);

	virtual void	InfoTrack_Reset(INT nWidth, INT nHeight);
	virtual void	InfoTrack_AddPoint(INT nX, INT nY);

protected:
	CDlgDebuger*		m_pDlgDebuger;

public:
	TDDebuger(void);
	virtual ~TDDebuger(void);

	TD_DECLARE_DYNAMIC(TDDebuger);
};
