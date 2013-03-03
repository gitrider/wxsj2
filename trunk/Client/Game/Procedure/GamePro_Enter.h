
/** ���볡��ǰ�ĵȴ�����
 */

#pragma once


#include "GameProcedure.h"



class CGamePro_Enter : public CGameProcedure
{
public:

	//--------------------------------------------------------------
	//��¼״̬
	enum ENTER_STATUS
	{
		ENTERSCENE_CONNECTING,	//!< ��game server��������
		ENTERSCENE_READY,		//!< ��ʼ��״̬

		ENTERSCENE_REQUESTING,	//!< ���ͽ��볡��������...
		ENTERSCENE_OK,			//!< ���볡���ɹ�����ת����һ��ѭ��
		ENTERSCENE_FAILED,		//!< ���볡��ʧ��
	};
	ENTER_STATUS		GetStatus(VOID) { return m_Status; }
	VOID				SetStatus(ENTER_STATUS status) { m_Status = status; }

	//--------------------------------------------------------------
	//������Ҫ����ĳ�������Ϣ(����ID/����id, ���еȼ�)
	VOID SetSceneID(INT nSceneID, INT nCityLevel, INT nResID) 
	{ 
		m_nSceneID = nSceneID; 
		m_nCityLevel = nCityLevel;
		m_nResID = nResID;
	}
	//616====== ���� ��ȡ����id
	INT GetSceneID()
	{
		return m_nSceneID;
	}

	INT	GetResID()
	{
		return m_nResID;
	}

	VOID				SetEnterType(UINT dwEnterType) { m_dwEnterType = dwEnterType; }
	UINT				GetEnterType(void) const { return m_dwEnterType; }

	//����ָ������
	VOID				EnterScene(void);


protected:
	// ������Ϸ��������ip ��ַ�Ͷ˿ں�
	TCHAR	m_szLoginServerAddr[64];
	INT		m_nLoginServerPort;

	// ������Ϸ�������ĳ����ţ��������ҳ��У�������ҳ���ID
	INT		m_nSceneID;
	INT		m_nResID;
	//�������ҳ��У����еȼ�
	INT		m_nCityLevel;

	UINT			m_dwEnterType;
	ENTER_STATUS	m_Status;

protected:
	virtual VOID	Init(VOID);
	virtual VOID	Tick(VOID);
	virtual VOID	Render(VOID);
	virtual VOID	Release(VOID);
	virtual VOID	MouseInput(VOID);

public:
	CGamePro_Enter(VOID);
	virtual ~CGamePro_Enter(VOID);
};
