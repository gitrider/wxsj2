
/** GameCommand.h
 */

#pragma once


#include "Type.h"



#define MAX_OBJ_CMD_PARAM_NUM	(16)

/// 
enum ENUM_GAME_COMMAD_TYPE
{
	GAME_COMMAD_TYPE_INVALID	= -1,
	GAME_COMMAD_TYPE_WORLD,
	GAME_COMMAD_TYPE_OBJECT,
	GAME_COMMAD_TYPE_AI,
	GAME_COMMAD_TYPE_DPC,			// Data pool
	GAME_COMMAD_TYPE_UI,
	GAME_COMMAD_TYPE_MOUSE,
};

/// Run Command �ķ���ֵ
enum RC_RESULT
{
	RC_OK = 0,
	RC_ERROR,
	RC_SKIP,
	RC_WAIT, // �ȴ�
};


/** ��Ϸ���߼�ָ�����
 */
struct SCommand
{
public:

	/// ��������
	WORD		m_wID;		// �磺WC_NONE, OC_NONE, UIC_NONE
	/// �����������
	union
	{
		UINT				m_adwParam[MAX_OBJ_CMD_PARAM_NUM];
		UINT				m_auParam[MAX_OBJ_CMD_PARAM_NUM];
		FLOAT				m_afParam[MAX_OBJ_CMD_PARAM_NUM];
		INT					m_anParam[MAX_OBJ_CMD_PARAM_NUM];
		BOOL				m_abParam[MAX_OBJ_CMD_PARAM_NUM];
		unsigned __int64	m_auqParam[MAX_OBJ_CMD_PARAM_NUM/2];
		__int64				m_aqParam[MAX_OBJ_CMD_PARAM_NUM/2];
		VOID*				m_apParam[MAX_OBJ_CMD_PARAM_NUM];
		const VOID*			m_apConstParam[MAX_OBJ_CMD_PARAM_NUM];
	};

//------------------------------------------------
protected:

	ENUM_GAME_COMMAD_TYPE	m_eType;

public:

	SCommand( VOID )
	{
		m_eType = GAME_COMMAD_TYPE_INVALID;
	}

	ENUM_GAME_COMMAD_TYPE GetType( VOID )const
	{
		return m_eType;
	}

	VOID Cleanup( VOID )
	{
		m_wID	= 0xFFFF;
		memset( m_adwParam, 0, sizeof ( m_adwParam ) );
	}
};



/** Ӧ����object������
 */
struct SCommand_Object : public SCommand
{
	SCommand_Object( VOID )
	{
		m_eType = GAME_COMMAD_TYPE_OBJECT;
	}
};

/** Ӧ����AI������
 */
struct SCommand_AI : public SCommand
{
	SCommand_AI( VOID )
	{
		m_eType = GAME_COMMAD_TYPE_AI;
	}
};

/** Ӧ����UI������
 */
struct SCommand_DPC : public SCommand
{
	SCommand_DPC( VOID )
	{
		m_eType = GAME_COMMAD_TYPE_DPC;
	}
};

/** ��������ϵ�����
 */
struct SCommand_Mouse : public SCommand
{
	enum MOUSE_CMD_TYPE
	{
		MCT_NULL,		

		MCT_PLAYER_MOVETO,	/// �ƶ���Ŀ���
							//  m_afParam[0][1] = The position of target area

		MCT_PLAYER_SELECT,	/// ѡ������
							//  m_adwParam[0] = The ID of target object

		MCT_SKILL_OBJ,		/// ��ѡĿ��ļ���
							//  m_adwParam[0] = The ID of skill
							//  m_adwParam[1] = The ID of target object
		MCT_SKILL_AREA,		/// ��Χ����
							//  m_adwParam[0] = The ID of skill
							//  m_afParam[1][2] = The position of target area
		MCT_SKILL_DIR,		/// ������
							//  m_adwParam[0] = The ID of skill
							//  m_afParam[1] = The direction

		MCT_HIT_TRIPPEROBJ,	/// �����ܹ�������TripperObj��(�������...)
							//  m_adwParam[0] = The ID of Tripper obj
							//  ...

		MCT_SPEAK,			/// ��NPC̸��
							//  m_adwParam[0] = The ID of NPC

		MCT_CONTEXMENU,		/// ��ʾ�Ҽ��˵�
							//  m_adwParam[0] = The ID of NPC

		MCT_REPAIR,			/// ����ָ��
		MCT_CANCLE_REPAIR,	/// ����ָ��

		MCT_USE_ITEM,		/// ��Ʒʹ��
							//  m_apParam[0] = Action Item
							//  m_adwParam[1] = The ID of target object
							//  m_afParam[2] = Pos X
							//  m_afParam[3] = Pos Z
							//  m_adwParam[4] = Is area

		MCT_CANCEL_USE_ITEM,/// ȡ����Ʒʹ��


		/// ����ڽ����ϵ�����״̬

		MCT_UI_USE_IDENTIFY,			/// ʹ�ü������� 	
		MCT_UI_USE_CANCEL_IDENTIFY,		/// ȡ��ʹ�ü������� 		

		MCT_ADD_FRIEND,		/// ��Ӻ���
		MCT_EXCHANGE,		/// ����
		//MCT_PLAYER_JUMP,	//��Ծ
	};


	SCommand_Mouse( VOID )
	{
		m_eType = GAME_COMMAD_TYPE_MOUSE;
	}

	/// �����������
	MOUSE_CMD_TYPE	m_typeMouse;
};
