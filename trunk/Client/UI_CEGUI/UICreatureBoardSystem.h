
/* ��ɫͷ���ϵ���ϢUI������
 */

#pragma once


#include "GIBasicType.h"
#include "ceguicolourrect.h"
#include "ifalagardbehitboard.h"
#include <list>
#include "ceguivector.h"


namespace CEGUI
{
	class Window;
};
class CCreatureBoard;

struct STRUCT_BEHIT_DATA
{
	float	m_fXPos;
	float	m_fYPos;
	float   m_nXSpeed; // X���ؼ��ƶ��ٶ�
	float   m_nYSpeed; // Y���ؼ��ƶ��ٶ�

	float   m_nXAcceleration;
	float   m_nYAcceleration; // ��������ļ��ٶ�

	
	int		m_nMaxLife;

	bool    m_bAlphaMode;	// �Ƿ���alphaģʽ
	int		m_nMoveMode;
	int		m_nUseTempPos;
	
	CEGUI::colour  m_ColorType;
	CEGUI::colour  m_BackColorType;
	
	float   m_fWidth;
	float   m_fHeight;
};
class CBeHitBoard
{
	
public:
	CBeHitBoard();
	~CBeHitBoard();

	void ResetData( LPCTSTR szText  );

	CEGUI::Window *	m_pWindow;
	char		m_szWindowName[ 128 ];
	
	bool	m_bDoubleHit;

	float   m_nStartX;
	float   m_nStartY;

	float   m_fAlpha;
	int		m_nLife;			// ������
	CEGUI::colour  m_ColorType;//int		m_nColorType;
	CEGUI::colour  m_BackColorType;//int		m_nColorType;

	float   m_nXSpeed; // X���ؼ��ƶ��ٶ�
	float   m_nYSpeed; // Y���ؼ��ƶ��ٶ�
	float   m_nXAcceleration;
	float   m_nYAcceleration; // ��������ļ��ٶ�
	int		m_nMaxLife;
	bool    m_bAlphaMode;	// �Ƿ���alphaģʽ

	float   m_fScale;
	STRUCT_BEHIT_DATA  *m_sData;

	float   m_fDoubleTime;
	bool    m_bZoomOn;
	int		m_nMoveMode;
	int		m_nUseTempPos;
	int     m_nTempPosIndex;
	int     m_nMoveType;
public:
	void Update();
	bool IsInUse(){ return m_bInUse; };
	void InUse( bool bUse );

private:
	bool  m_bInUse; // �Ƿ�����ʹ��
	
};
// ��������ͷ��ð���ֵĹ�����
class CBeHitBoardManager
{

public:
	CBeHitBoardManager();
	virtual ~CBeHitBoardManager();
public:
	int				m_nRaisSize; // �Զ������óߴ�
	std::list< CBeHitBoard* >  m_aWindowList;

	CBeHitBoard* GetFreeWindow();
	void FreeWindow( CBeHitBoard* pWindow );


	static CBeHitBoardManager*	GetMe(VOID) { return s_pMe; }

	static CBeHitBoardManager* s_pMe;

	STRUCT_BEHIT_DATA	m_BeHitData[ CEGUI::MAX_MOVE_TYPE_NUMBER ];
	CEGUI::colour		m_ColorType[ CEGUI::MAX_COLOR_TYPE_NUMBER ];
	CEGUI::colour		m_BackColorType[ CEGUI::MAX_COLOR_TYPE_NUMBER ];
	CEGUI::Point		m_TempPoint[ CEGUI::MAX_TEMP_TYPE_NUMBER ]; 
	float				getNormalSize() { return m_fNormalSize;}
	float				getDoubleSize() { return m_fDoubleSize;}
	float				getDoubleEndSize() { return m_fDoubleEndSize;}
	float				getDoubleTime() { return m_fDoubleTime; }
private:
	int					m_nCounter;
	
	CBeHitBoard*		NewWindow();

	float				m_fNormalSize;
	float				m_fDoubleSize;
	float				m_fDoubleEndSize;

	float				m_fDoubleTime;

};
class CUICreatureBoardSystem
{
public:
	static CEGUI::Window*	GetClientScreen(VOID) { return CUICreatureBoardSystem::m_pCreatureBoardGroundSheet; };
	static CEGUI::Window*   m_pCreatureBoardGroundSheet; // �ͻ��˴��ڵװ�

	//����/���� ��Ϣ��
	CCreatureBoard*		CreateCreatureBoard();
	void				DestroyCreatureBoard(CCreatureBoard* pBoard);
	// ����һ������Ч����Ϣ��
	// bDoubleHit �Ƿ�Ϊ�ػ�
	// szValue ��ʾ���ַ���
	// nStartX nStartY ��ʵ��Ļ����
	void				AddNewBeHit( bool bDoubleHit, LPCTSTR szValue, float nStartX, float nStartY, int nType = 0, int nMoveType = 0 );

public:
	//��������ʼ��
	VOID				Initial(VOID);
	//�ͷ�������Դ
	VOID				Release(VOID);

	VOID				Tick();

protected:
	//������Ϣ�������
	std::list< CCreatureBoard* >	m_listAllBoard;

	std::list< CBeHitBoard* >		m_listBeHitBoard;

	CBeHitBoardManager*				m_pBeHitBoardManager;

protected:
	//��Ϣ���
	std::list< CCreatureBoard* >	m_listBoardDataPool;

public:
	CUICreatureBoardSystem();
	virtual ~CUICreatureBoardSystem();
	static CUICreatureBoardSystem*	GetMe(VOID) { return s_pMe; }

	static CUICreatureBoardSystem* s_pMe;

	float   GetMaxDistance() { return m_nMaxDistance; };
	float   GetMaxDispearDistance() { return m_nMaxDispearDistance; };

protected:
	int m_nBeHitCounter;
	float m_nMaxDistance; // ������ʾ��Χ
	float m_nMaxDispearDistance;// ���������Χ����ʧ
};