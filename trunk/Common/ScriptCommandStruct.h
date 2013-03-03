// ScriptCommandStruct.h
//
/////////////////////////////////////////////////////////

#ifndef __SCRIPTCOMMANDSTRUCT_H__
#define __SCRIPTCOMMANDSTRUCT_H__

#include "Type.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"

// script command
enum ENUM_SCRIPT_COMMAND
{
	SCRIPT_COMMAND_INVALID	= -1,
	SCRIPT_COMMAND_EVENT_LIST_RESPONSE,			// �¼��б���
	SCRIPT_COMMAND_MISSION_RESPONSE,			// �����¼��Ĳ�ѯ����
	SCRIPT_COMMAND_MISSION_REGIE,				// ���������ѯ����
	SCRIPT_COMMAND_MISSION_DEMAND_RESPONSE,		// ��������Ĳ�ѯ����
	SCRIPT_COMMAND_MISSION_CONTINUE_RESPONSE,	// ����ļ�����ť�¼�����
	SCRIPT_COMMAND_MISSION_TIPS,				// ������ʾ
	SCRIPT_COMMAND_TRADE,						// ����
};

#define	DEF_SCRIPT_STRING_LEN	(512+1)
struct ScriptString
{
	SHORT	m_Size ;
	CHAR	m_szString[DEF_SCRIPT_STRING_LEN];

	VOID Reset( VOID ){
		m_Size = 0 ;
		memset( m_szString, 0, sizeof( m_szString ) );
	}

	ScriptString &operator = ( UINT uValue ){
		return *this;
	}

	ScriptString &operator = ( const CHAR *pszString ){
		if ( pszString != NULL )
		{
			m_Size = (SHORT)strlen( pszString );
			if ( m_Size > DEF_SCRIPT_STRING_LEN - 1 )
				m_Size = DEF_SCRIPT_STRING_LEN - 1;

			if ( m_Size > 0 )
				memcpy( m_szString, pszString, m_Size );

			m_szString[m_Size] = 0;
		}
		else
		{
			m_Size = 0 ;
			m_szString[0] = 0;
		}
		return *this;
	}

	UINT GetBufSize( VOID )const;

	BOOL Read( SocketInputStream& iStream );
	BOOL Write( SocketOutputStream& oStream )const;
};

/////////////////////////////////////////////////////////////////
// �¼��б� 
enum ENUM_EVENT_ITEM_TYPE
{
	EVENT_ITEM_TYPE_INVALID	= -1,	// ��Ч
	EVENT_ITEM_TYPE_SECTION,		// �ڵ�
	EVENT_ITEM_TYPE_SCRIPT_ID,		// ѡ��
	EVENT_ITEM_TYPE_TEXT,			// �ı�
};

struct ScriptEventItem
{
	INT					m_nType;	// ENUM_EVENT_ITEM_TYPE
	INT					m_state;
	INT					m_index;
	ScriptString		m_strString;
	ScriptID_t			m_idScript;

	VOID Reset( VOID ){
		m_nType			= EVENT_ITEM_TYPE_INVALID;
		m_strString.Reset();
	}

	UINT GetBufSize( VOID )const;

	VOID SetSection( const ScriptString *pstrSection ){
		m_nType	= EVENT_ITEM_TYPE_SECTION;
		m_strString = *pstrSection;
	}

	VOID SetScriptID( INT index,INT state,ScriptID_t idScript, const ScriptString *pstrSection ){
		m_nType	= EVENT_ITEM_TYPE_SCRIPT_ID;
		m_state = state;
		m_index = index;
		m_idScript = idScript;
		m_strString = *pstrSection;
	}

	VOID SetText( const ScriptString *pstrSection ){
		m_nType	= EVENT_ITEM_TYPE_TEXT;
		m_strString = *pstrSection;
	}

	BOOL Read( SocketInputStream& iStream );
	BOOL Write( SocketOutputStream& oStream )const;
};

#define MAX_EVENT_LIST_ITEM_COUNT	(16)
struct ScriptParam_EventList
{
	ObjID_t				m_idNPC;
	BYTE				m_yItemCount;
	ScriptEventItem		m_seiItem[MAX_EVENT_LIST_ITEM_COUNT];

	VOID Reset( VOID ){
		m_idNPC			= INVALID_ID;
		m_yItemCount	= 0;
		UINT i;
		for ( i = 0; i < MAX_EVENT_LIST_ITEM_COUNT; i++ )
		{
			m_seiItem[i].Reset();
		}
	}

	UINT GetBufSize( VOID )const;

	VOID AddItem( const ScriptEventItem *pItem ){
		if ( m_yItemCount < MAX_EVENT_LIST_ITEM_COUNT )
		{
			m_seiItem[m_yItemCount] = *pItem;
			m_yItemCount++;
		}
	}

	const ScriptEventItem *GetItem( BYTE yIndex )const{
		return (yIndex < m_yItemCount )?(&m_seiItem[yIndex]):(NULL);
	}

	BOOL Read( SocketInputStream& iStream );
	BOOL Write( SocketOutputStream& oStream )const;
};

/////////////////////////////////////////////////////////////////
// ������Ϣ
struct SMissionBonusItem
{
	BYTE		m_yCount;
	UINT		m_uItemID;

	VOID Reset( VOID ){
		m_yCount	= 0;
		m_uItemID	= UINT_MAX;
	}

	UINT GetBufSize( VOID )const;

	BOOL Read( SocketInputStream& iStream );
	BOOL Write( SocketOutputStream& oStream )const;
};
//������Ҫɱ����NPC
struct SMissionDemandKill
{
	BYTE		m_yCount;
	UINT		m_uNPCID;

	VOID Reset( VOID ){
		m_yCount	= 0;
		m_uNPCID	= UINT_MAX;
	}

	UINT GetBufSize( VOID )const;

	BOOL Read( SocketInputStream& iStream );
	BOOL Write( SocketOutputStream& oStream )const;
};
enum ENUM_MISSION_BONUS_TYPE
{
	MISSION_BONUS_TYPE_INVALID	= -1,
	MISSION_BONUS_TYPE_MONEY,			// ��Ǯ
	MISSION_BONUS_TYPE_ITEM,			// ��Ʒ
	MISSION_BONUS_TYPE_ITEM_RAND,		// �����Ʒ
	MISSION_BONUS_TYPE_ITEM_RADIO,		// ��ѡ1��Ʒ
	MISSION_BONUS_TYPE_EXP,				// ��������
};

// �����Ľṹ
struct SMissionBonus
{
	INT							m_nType;		// ENUM_MISSION_BONUS_TYPE
	union{
		UINT					m_uMoney;
		UINT					m_uExp;
		SMissionBonusItem		m_ItemBonus;
	};

	VOID Reset( VOID ){
		m_nType		= MISSION_BONUS_TYPE_INVALID;
		m_ItemBonus.Reset();
	}

	VOID SetMoney( UINT uMoney ){
		m_nType		= MISSION_BONUS_TYPE_MONEY;
		m_uMoney	=uMoney;
	}

	VOID SetItem( UINT uItemID, BYTE yItemCount ){
		m_nType		= MISSION_BONUS_TYPE_ITEM;
		m_ItemBonus.m_yCount	= yItemCount;
		m_ItemBonus.m_uItemID	=uItemID;
	}

	VOID SetItemRand( VOID ){
		m_nType		= MISSION_BONUS_TYPE_ITEM_RAND;
	}

	VOID SetItemRadio( UINT uItemID, BYTE yItemCount ){
		m_nType		= MISSION_BONUS_TYPE_ITEM_RADIO;
		m_ItemBonus.m_yCount	= yItemCount;
		m_ItemBonus.m_uItemID	=uItemID;
	}

	UINT GetBufSize( VOID )const;

	BOOL Read( SocketInputStream& iStream );
	BOOL Write( SocketOutputStream& oStream )const;
};

#define MAX_MISSION_TEXT_COUNT		(8)
#define MAX_MISSION_BONUS_COUNT		(16)
struct ScriptParam_MissionInfo
{
	ObjID_t			m_idNPC;			// �����������ʱ�Ĳ���
	ScriptID_t		m_idScript;			// ��������������ز������磺�������񣨴����������Ĳ�������ID�����ǽű�ID��
	MissionID_t		m_idMission;		// ���ڿͻ�����ʾ��Ϣ�Ĳ�ѯ
	BYTE			m_yTextCount;
	ScriptString	m_aText[MAX_MISSION_TEXT_COUNT];
	BYTE			m_yBonusCount;
	SMissionBonus	m_aBonus[MAX_MISSION_BONUS_COUNT];

	VOID Reset( VOID ){
		m_idNPC			= INVALID_ID;
		m_idScript		= INVALID_ID;
		m_idMission		= INVALID_ID;
		m_yTextCount	= 0;
		UINT i;
		for ( i = 0; i < MAX_MISSION_TEXT_COUNT; i++ )
		{
			m_aText[i].Reset();
		}
		m_yBonusCount	= 0;
		for ( i = 0; i < MAX_MISSION_BONUS_COUNT; i++ )
		{
			m_aBonus[i].Reset();
		}
	}

	VOID AddText( const ScriptString *pstrText ){
		if ( m_yTextCount < MAX_MISSION_TEXT_COUNT )
		{
			m_aText[m_yTextCount] = *pstrText;
			m_yTextCount++;
		}
	}

	VOID AddBonus( const SMissionBonus *pBonus ){
		if ( m_yBonusCount < MAX_MISSION_BONUS_COUNT )
		{
			m_aBonus[m_yBonusCount] = *pBonus;
			m_yBonusCount++;
		}
	}

	UINT GetBufSize( VOID )const;

	BOOL Read( SocketInputStream& iStream );
	BOOL Write( SocketOutputStream& oStream )const;
};

/////////////////////////////////////////////////////////////////
// ���������ѯ����
typedef ScriptParam_EventList	ScriptParam_MissionRegie;

/////////////////////////////////////////////////////////////////
// ����������Ϣ

// ����ṹ����
typedef SMissionBonusItem SMissionDemandItem;

#define MAX_MISSION_DEMAND_COUNT		(8)
struct ScriptParam_MissionDemandInfo
{
	enum
	{
		MISSION_NODONE	=0,
		MISSION_DONE,
		MISSION_CHECK,
	};
	ObjID_t				m_idNPC;			// �����������ʱ�Ĳ���
	ScriptID_t			m_idScript;			// �����������ʱ�Ĳ���
	MissionID_t			m_idMission;		// ���ڿͻ�����ʾ��Ϣ�Ĳ�ѯ
	INT					m_bDone;			// ��ɱ�־ 0:δ��ɣ�1:��ɣ�2:��Ҫ�����ж�
	BYTE				m_yTextCount;
	ScriptString		m_aText[MAX_MISSION_TEXT_COUNT];
	BYTE				m_yDemandCount;
	SMissionDemandItem	m_aDemandItem[MAX_MISSION_DEMAND_COUNT];

	VOID Reset( VOID ){
		m_idNPC			= INVALID_ID;
		m_idScript		= INVALID_ID;
		m_idMission		= INVALID_ID;
		m_bDone			= FALSE;
		m_yDemandCount	= 0;
		m_yTextCount	= 0;
		UINT i;
		for ( i = 0; i < MAX_MISSION_TEXT_COUNT; i++ )
		{
			m_aText[i].Reset();
		}
		for ( i = 0; i < MAX_MISSION_DEMAND_COUNT; i++ )
		{
			m_aDemandItem[i].Reset();
		}
	}

	VOID AddText( const ScriptString *pstrText ){
		if ( m_yTextCount < MAX_MISSION_TEXT_COUNT )
		{
			m_aText[m_yTextCount] = *pstrText;
			m_yTextCount++;
		}
	}

	VOID AddDemandItem( const SMissionDemandItem *pItem ){
		if ( m_yDemandCount < MAX_MISSION_DEMAND_COUNT )
		{
			m_aDemandItem[m_yDemandCount] = *pItem;
			m_yDemandCount++;
		}
	}

	UINT GetBufSize( VOID )const;

	BOOL Read( SocketInputStream& iStream );
	BOOL Write( SocketOutputStream& oStream )const;
};

/////////////////////////////////////////////////////////////////
// ����ļ�����ť�¼�����(��������Ľ����ȵ�)
struct ScriptParam_MissionContinueInfo
{
	ObjID_t				m_idNPC;			// �����������ʱ�Ĳ���
	ScriptID_t			m_idScript;			// �����������ʱ�Ĳ���
	MissionID_t			m_idMission;		// ���ڿͻ�����ʾ��Ϣ�Ĳ�ѯ
	BYTE				m_yTextCount;						// ˵�����ı�����Ŀ
	ScriptString		m_aText[MAX_MISSION_TEXT_COUNT];	// ˵�����ı�
	BYTE				m_yBonusCount;						// ������߽�������Ŀ
	SMissionBonus		m_aBonus[MAX_MISSION_BONUS_COUNT];	// ��������б�

	VOID Reset( VOID ){
		m_idNPC			= INVALID_ID;
		m_idScript		= INVALID_ID;
		m_idMission		= INVALID_ID;
		m_yBonusCount	= 0;
		m_yTextCount	= 0;
		UINT i;
		for ( i = 0; i < MAX_MISSION_TEXT_COUNT; i++ )
		{
			m_aText[i].Reset();
		}
		for ( i = 0; i < MAX_MISSION_BONUS_COUNT; i++ )
		{
			m_aBonus[i].Reset();
		}
	}

	VOID AddText( const ScriptString *pstrText ){
		if ( m_yTextCount < MAX_MISSION_TEXT_COUNT )
		{
			m_aText[m_yTextCount] = *pstrText;
			m_yTextCount++;
		}
	}

	VOID AddBonus( const SMissionBonus *pBonus ){
		if ( m_yBonusCount < MAX_MISSION_DEMAND_COUNT )
		{
			m_aBonus[m_yBonusCount] = *pBonus;
			m_yBonusCount++;
		}
	}

	UINT GetSelectItemID(INT nIndex)
	{
		INT i;
		for(i=0;i<m_yBonusCount;i++)
		{
			if( MISSION_BONUS_TYPE_ITEM_RADIO == m_aBonus[i].m_nType )
				break;
		}
		return m_aBonus[i + nIndex].m_ItemBonus.m_uItemID;
	}

	UINT GetBufSize( VOID )const;

	BOOL Read( SocketInputStream& iStream );
	BOOL Write( SocketOutputStream& oStream )const;
};

/////////////////////////////////////////////////////////////////
// ������ʾ
struct ScriptParam_MissionTips
{
	ScriptString		m_strText;

	VOID Reset( VOID ){
		m_strText.Reset();
	}

	UINT GetBufSize( VOID )const;

	BOOL Read( SocketInputStream& iStream );
	BOOL Write( SocketOutputStream& oStream )const;
};

/////////////////////////////////////////////////////////////////
// ����
#define MAX_TRADE_ITEM_COUNT	(128)
struct STradeItem
{
	UINT		m_uDataID;				// Excel�е�Index
	_ITEM_TYPE	m_typeItem;				// ��������
	BYTE		m_yCount;				// ��������

	VOID Reset( VOID ){
		m_uDataID	= UINT_MAX;
		m_typeItem.CleanUp();
		m_yCount	= 0;
	}

	UINT GetBufSize( VOID )const;

	BOOL Read( SocketInputStream& iStream );
	BOOL Write( SocketOutputStream& oStream )const;
};

// ������Ϣ���õ��Ľṹ
struct ScriptParam_Trade
{
	BYTE			m_yItemCount;							// ������Ŀ
	STradeItem		m_aTradeItem[MAX_TRADE_ITEM_COUNT];		// �����б�
	BOOL			m_bRepair;								// �Ƿ���������

	VOID Reset( VOID ){
		m_yItemCount	= 0;
		UINT i;
		for( i = 0; i < MAX_TRADE_ITEM_COUNT; i++ )
		{
			m_aTradeItem[i].Reset( );
		}
	}

	UINT GetBufSize( VOID )const;

	BOOL Read( SocketInputStream& iStream );
	BOOL Write( SocketOutputStream& oStream )const;
};

#endif
