
/** �����obj
 *	������������ײ�Ĵ���tick
 */

#pragma once


#include "Object.h"



/// �����¼�
#define	MAX_REGISTER_EVENTS_NUM		(20)


enum PHY_EVENT_ID
{
	PE_NONE	 = -1,									/// ���¼�
	PE_COLLISION_WITH_GROUND,						/// ������ػ�����淢���Ӵ�
};


class CObject_Phy : public CObject
{
public:

	CObject_Phy();
	virtual ~CObject_Phy();

public:

	/** ���ݳ�ʼ���������� */
	virtual	VOID	Initial(VOID*);

	/** ���ݵ�������λ��, fvPosition�зֱ���x��z */
	virtual VOID	SetMapPosition(const fVector2& fvPosition);

	/** ע��һ�������¼�,��ע��������¼�,���ڷ���ʱ֪ͨʵ���� */
	virtual VOID	RegisterPhyEvent(PHY_EVENT_ID eventid);

	/** ע��һ�������¼�,�������¼��ķ��������ᱻ֪ͨ��ʵ���� */
	virtual VOID	UnRegisterPhyEvent(PHY_EVENT_ID eventid);

	/** ֪ͨһ�������¼����� */
	virtual VOID	NotifyPhyEvent(PHY_EVENT_ID eventid, VOID* pParam = NULL);

	/** ��һ����ʼ�ٶ� */
	VOID			AddLinearSpeed(fVector3 vSpeed);

	/** �������Ƿ����������� */
	BOOL			IsPhyEnable() {return m_bIsEnable;};

	/** ʹ���������������� */
	VOID			PhyEnable(BOOL bFlag);

private:

	/** �ַ������¼� */
	VOID			DispatchPhyEvent(PHY_EVENT_ID eventid, VOID* pParam = NULL);


private:

	/// ������������ٶ�
	fVector3		m_fvLinearSpeed;

	/// ���������ת�ٶ�
	fVector3		m_fvRotSpeed;

	/// �������Ƿ�����������
	BOOL			m_bIsEnable;

	/// �ϴμ�¼����ʱ��
	UINT			m_nLastTickTime;

	/// ��������ĵ������¼��б�
	PHY_EVENT_ID	m_aEventList[MAX_REGISTER_EVENTS_NUM];

	/// ��������ĵ������¼�����
	UINT			m_nEventListNum;

	/// �������Ƿ��ڿ���
	BOOL			m_bIsInAir;

protected:

	/** һ������Tick */
	virtual VOID	Tick(VOID);
	

	KL_DECLARE_DYNAMIC(CObject_Phy);
	KL_DECLARE_LOGICAL(TRUE);

};
