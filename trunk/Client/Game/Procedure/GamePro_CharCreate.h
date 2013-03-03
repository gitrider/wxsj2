
/** 
 *	���ﴴ������
 */
#pragma once


#include "GameProcedure.h"
#include "CharacterFaceMng.h"
#include "FaceModelMng.h"
#include "HairModelMng.h"
#include "..\Object\ObjectDef.h"



class CObject_PlayerOther;

class CGamePro_CharCreate : public CGameProcedure
{
public:
	
	/// ��¼״̬
	enum PLAYER_CHAR_CREATE_STATUS
	{
		CHAR_CREATE_CREATE_OK = 0 ,	// ������ɫ�ɹ�
		CHAR_CREATE_CREATEING,		// ������ɫ
	};

public:

	CGamePro_CharCreate(VOID);
	virtual ~CGamePro_CharCreate(VOID);

	/** ��������ģ�� */
	int CreateModel();

	/** �л���ѡ��������� */
	void ChangeToSelectRole();

	/** ��ʼ��������������ɫ��UI��ʾģ�ͣ� */
	void CallInit()											{ Init(); }


	/** ������ɫ */
	int CreateRole();

	/** �л�����ɫѡ����� */
	void ChangeToRoleSel();


	/** ����ͷ��id */
	void SetFace(int iFaceId);	
	/** ͨ���Ա����������ͷ��id */
	void SetFaceByRaceAndIndex(int iRace, int iIndex);	


	/** ��������id */
	void SetFaceModel(int iFaceModelId);	
	/** ͨ���Ա����������ͷ��id */
	void SetFaceModelByRaceAndIndex(int iRace, int iIndex);	


	/** ��������id */
	void SetHairModel(int iHairModelId);	
	/** ͨ���Ա����������ͷ��id */
	void SetHairModelByRaceAndIndex(int iRace, int iIndex);	


	/**
	 *	�Ա�
	 */
	VOID SetGender( INT nGender );
	INT GetGender( VOID );

	void PlayAnimation(int iGender, int iActionIndex);

	/**
	 *	ְҵ
	 */
	VOID SetProfession( INT nProfession );
	INT GetProfession( VOID );


protected:

	virtual VOID	Init(VOID);
	virtual VOID	Tick(VOID);
	virtual VOID	Render(VOID);
	virtual VOID	Release(VOID);
	virtual VOID	MouseInput(VOID);
	// ��������
	virtual VOID	ProcessInput(VOID);


public:

	PLAYER_CHAR_CREATE_STATUS	m_Status;


	CObject_PlayerOther*	m_pAvatar[2];						 /// ����UI��ʾ���߼�����.

	TCHAR					m_szBufModelName1[128];				 /// Ůģ������.
	TCHAR					m_szBufModelName2[128];				 /// ��ģ������.


	TCHAR					m_szCreateRoleName[128];			 /// Ҫ�����Ľ�ɫ������
	int						m_iFaceColor1;						 /// �����沿��ɫ
	int 					m_iFaceModle1;						 /// �����沿ģ��
	int 					m_iHairColor1;						 /// ����ͷ����ɫ
	int 					m_iHairModle1;						 /// ����ͷ��ģ��
	int 					m_iSex;								 /// �����Ա�
	int                     m_iCountry1;                         /// ����
	int                     m_iProfession1;                      /// ְҵ
	int                     m_iFaceId;                           /// ����ͷ��

	//test ����login֮��Ͳ�����
	int						m_nDefHeadGeo;		// ͷ
	int						m_nDefShoulder;		// ��
	int						m_nDefBody;			// ����
	int						m_nDefArm;			// ��
	int						m_nDefFoot;			// ��
				

	/// ͷ�������.
	CCharacterFaceMng		m_FaceMng;
	/// �Ƿ��ȡͷ����Ϣ.
	int						m_iIsLoadFace;

	/// ������Ϣ������
	CFaceModelMng			m_FaceModelmng;
	/// �Ƿ��ȡ����ģ����Ϣ.
	int						m_iIsLoadFaceModel;

	/// ������Ϣ������
	CHairModelMng			m_HairModelMng;
	/// �Ƿ��ȡ����ģ����Ϣ.
	int						m_iIsLoadHairModel;

private:

	/// 
	ENUM_GENDER				m_eGender;

	/// 
	INT						m_nUIViewProfession;

};	// class
