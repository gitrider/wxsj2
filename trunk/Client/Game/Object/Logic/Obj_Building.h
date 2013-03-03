
#pragma once

#include "Obj_Static.h"


/**
	�������������ȼ���ʾ��ͬģ�ͣ�֧��Model,Mesh,��Obj
*/

struct _DBC_CITY_BUILDING;
struct _DBC_BUILDING_DEFINE;

class CObject_Building : public CObject_Static
{
	/********************************************
		���нӿ�
	*********************************************/
public:
	//ģ����������
	enum ResType
	{
		RT_Mesh,	//��̬ģ��
		RT_Model,	//��̬ģ��
		RT_Object,	//�߼�ģ��
	};

	/********************************************
		�߼����ݷ����ı�
	*********************************************/
	//��Դ����ID�����ı�
	void	SetLogicData_Define(const _DBC_CITY_BUILDING* pDefine);
	//�ȼ������ı�
	void	SetLogicData_Level(INT nLevel);

	/********************************************
		�ڲ��ӿ�
	*********************************************/
protected:
	//����Ⱦ�㴴����Ⱦָ��
	virtual VOID CreateRenderInterface(VOID);

public:
	CObject_Building();
	virtual ~CObject_Building();
	virtual VOID Release();

protected:
	/********************************************
		�߼�����
	*********************************************/
	const _DBC_CITY_BUILDING*		m_pBuildingDefine;		//!< �����ﶨ��(DBC_CITY_BUILDING)
	const _DBC_BUILDING_DEFINE*		m_pResDefine;			//!< ��Դ����(DBC_BUILDING_DEINFE)

	ResType				m_resType;					//��Դ����
	int					m_nLevel;					//��ǰ�߼��ȼ�

protected:
	KL_DECLARE_DYNAMIC(CObject_Building);
};
