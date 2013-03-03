
/** GamePro_Main.h
 *	
 *	���ܣ�	��Ϸ������
 *
 *	�޸ļ�¼��
 *			080401	m_bShowOtherObj
 *					m_bShowOtherObjBoard
 */

#pragma once


#include "GameProcedure.h"
#include "GIMath.h"
#include "..\GameCommand.h"



namespace Fairy
{
class BulletFlowSystem;
};

class CObject;
class CObject_Character;
class CObject_ProjTex_MouseTarget;
class CObject_ProjTex_AuraDure;
struct _DBC_SKILL_DATA;
class tActionItem;
class tSoundSource;
struct InputEvent;

class CGamePro_Main : public CGameProcedure
{
public:

	VOID			SetSceneID(INT nSceneID) { m_SceneID = nSceneID; }

	//����Ŀ�ĵ�ָʾ��
	VOID			SetMouseTargetProjTex( const fVector2&  fvAimPos);

	//����������
	VOID			SetActiveSkill(tActionItem* pSkill) { m_pActiveSkill = pSkill; }
	tActionItem*	GetActiveSkill(VOID) { return m_pActiveSkill; }

	BOOL			IsAutoRun(VOID) { return m_bAutoRun; }
	CObject_ProjTex_MouseTarget* GetMouseTarget(void) { return m_pMouseTarget; }

public:

	//�������
	VOID			OnSetCursor(VOID);

protected:

	//��ǰ����ĳ���id
	INT					m_SceneID;
	//����Լ�
	CObject_Character*		m_pMySelf;
	//�����ק�������
	BOOL					m_bCameraDraging;
	BOOL					m_bIsLastCameraDraging;
	
	//�϶�������
	BOOL					m_bAutoRun;

protected:

	virtual VOID	Init(VOID);
	virtual VOID	Tick(VOID);
	virtual VOID	Render(VOID);
	virtual VOID	Release(VOID);
	virtual VOID	CloseRequest(VOID);

	virtual LRESULT	MainWndProc(HWND, UINT, WPARAM, LPARAM);

	// ������̵�������
	VOID	Handle_AccelerateKey(InputEvent& event);

	// ���������ϼ�����
	VOID	Handle_Combination_Key(InputEvent& event);

public:

	//��������
	virtual VOID	ProcessInput(VOID);
	//�����������
			VOID	ProcessInput_Mouse(VOID);
	//�����������
			VOID	ProcessInput_Keybord(VOID);


	static int						m_nResetCamera;

private:

	//-------------------------------------------
	//��ǰ����ļ���
	tActionItem*					m_pActiveSkill;

	//-------------------------------------------
	//���ܷ�Χָʾ��
	CObject_ProjTex_AuraDure*		m_pSkillArea;
	//���Ŀ�ĵ�ָʾ��
	CObject_ProjTex_MouseTarget*	m_pMouseTarget;

public:

	CGamePro_Main(VOID);
	virtual ~CGamePro_Main(VOID);

public:

	/// ��¼�Ƿ��Ѿ���ʼ����
	BOOL			m_bInit;

	/// �Ƿ���ʾ������������Ķ���
	BOOL			m_bShowOtherObj;
	/// �Ƿ���ʾ������������������Ϣ
	BOOL			m_bShowOtherObjBoard;


	Fairy::BulletFlowSystem* m_bulletFlow;

};
