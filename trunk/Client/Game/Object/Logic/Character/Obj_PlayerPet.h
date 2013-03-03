
/** Obj_PlayerPet.h
 */

#pragma once


#include "Obj_PlayerNPC.h"



class CObject_PlayerPet : public CObject_PlayerNPC
{
public:

	//���ؽ�ɫ����
	virtual CHARACTER_TYPE	GetCharacterType( VOID ) const { return CT_PLAYERPET;	}

	//------------------------------------------
	//�߼����ݷ����ı�
	//------------------------------------------
protected:

	// RenderInterface����/ɾ��
	virtual VOID			CreateRenderInterface(VOID);

public:

	//�����ܲ��ٶ�
			VOID			Debug_ChangeSpeed(FLOAT fStep);
	//�����ܲ�Ƶ��
			VOID			Debug_ChangeRate(FLOAT fStep);
			FLOAT			Debug_GetRate(VOID) { return m_fDebugRate; }
protected:

	//-- for debug
	//��ɫ���������ٶ�
	FLOAT					m_fDebugRate;
	//-- for debug

protected:

	virtual VOID				OnNewSoundHandle(VOID);
	virtual VOID				OnDeleteSoundHandle(VOID);

	//--------------------------------------------------------
	//���ļ̳�
	//--------------------------------------------------------
public:

	CObject_PlayerPet();
	virtual ~CObject_PlayerPet();

	///���ݳ�ʼ�����壬��ͬ������Ⱦ��
	virtual	VOID			Initial(VOID*);
	///�߼�����
	virtual VOID			Tick(VOID);

protected:

	KL_DECLARE_DYNAMIC(CObject_PlayerPet);
	KL_DECLARE_LOGICAL(true);

};	// class
