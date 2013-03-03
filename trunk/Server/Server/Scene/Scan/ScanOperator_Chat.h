/////////////////////////////////////////////////////////////////////////////////
//�ļ�����ScanOperator_Chat.h
//�����ߣ�jack
//����������������Χ�ķ���Ҫ���obj���û�����������Ϣ
//�޸ļ�¼��2005-12-08����
/////////////////////////////////////////////////////////////////////////////////


#ifndef __SCANOPERATOR_CHAT_H__
#define __SCANOPERATOR_CHAT_H__

#include "Type.h"
#include "ScanOperator.h"

class Packet ;

struct SCANOPERATOR_CHAT_INIT : public SCANOPERATOR_INIT
{
	Packet*			m_pPacket ;
	INT				m_nChatType ;
	GuildID_t		m_GuildID ;
	//����
	JiaZuID_t				m_JiaZuID;
	//����
	CountryID_t			m_CountryID;

	SCANOPERATOR_CHAT_INIT( )
	{
		m_pPacket = NULL ;
		m_nChatType = 0 ;
		m_GuildID = INVALID_ID ;
		m_JiaZuID = INVALID_ID;
		m_CountryID = INVALID_ID;
	};
};



class ScanOperator_Chat : public ScanOperator
{
public :
	ScanOperator_Chat( ) ;
	virtual ~ScanOperator_Chat( ) ;

	//��ʼ��ɨ�������
	virtual BOOL				Init( SCANOPERATOR_CHAT_INIT* pInit ) ;
	//ɨ�������ʼǰ�ص�
	virtual BOOL				On_BeforeScan( ) ;
	//�жϲ����е�Zone�Ƿ���Ҫɨ��
//	virtual BOOL				On_IsNeedScan( ZoneID_t ZoneID ) ;
	//������һ��Obj, ����ֵ�� SCANRETURN
	virtual UINT				On_FindObj( Obj* pObj ) ;
	//ɨ�������ɺ�ص�
//	virtual VOID				On_AfterScan( ) ;


public :
	Packet*			m_pPacket ;
	INT				m_nChatType ;
	GuildID_t		m_GuildID ;
	//����
	JiaZuID_t				m_JiaZuID;
	//����
	CountryID_t			m_CountryID;

};




#endif
