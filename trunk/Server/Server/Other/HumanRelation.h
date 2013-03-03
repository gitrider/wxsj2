
/////////////////////////////////////////////////////////////////////////////////
//�ļ�����HumanRelation.h
//�����ߣ�jack
//����������ʵ����Һ����Լ���������ع���
//�޸ļ�¼��2005-11-30 ����
//			2005-12-16 �������ݽṹ�Լ�����ģ�飬fancy
/////////////////////////////////////////////////////////////////////////////////

#ifndef __HUMANRELATION_H__
#define __HUMANRELATION_H__

#include "Type.h"
#include "DB_Struct.h"

class Obj_Human;

class HumanRelation
{
public :
	HumanRelation( );
	~HumanRelation( );

	BOOL							Init( Obj_Human* pHuman );
	VOID							CleanUp( );

	// �ж��Ƿ��Լ��ĺ���
	BOOL							IsFriend( GUID_t guid );

	// �ж��Ƿ��ں�����
	BOOL							IsBlackName( GUID_t guid );

	// �õ��������֮��Ĺ�ϵ����
	RELATION_TYPE					GetRelationType( GUID_t guid );

	// �ж��������֮���Ƿ����ĳ�ֹ�ϵ
	BOOL							HaveRelation( RELATION_TYPE RelationType, GUID_t guid );

	// ���һ����ϵ��
	BOOL							AddRelation( RELATION_TYPE RelationType, _RELATION* pRelationData );

	// ���һ����ϵ��
	BOOL							AddRelation( RELATION_TYPE RelationType, const RELATION_MEMBER* pMember );

	// ɾ��һ����ϵ��
	BOOL							DelRelation( RELATION_TYPE RelationType, GUID_t guid );

	// �����ϵ
	BOOL							RelationTransition( RELATION_TYPE newRelationType, GUID_t guid );

	// �Ƿ�ĳ���Ѿ���Ա
	BOOL							IsGroupFull( RELATION_GROUP RelationGroup );

	// �ж�һ�ֹ�ϵ�Ƿ��ܹ�ֱ��ת������һ�ֹ�ϵ
	BOOL							CanTransition( RELATION_TYPE oldRelationType, RELATION_TYPE newRelationType );

	// �õ�����
	const CHAR*						GetMood( );

	// ��������
	VOID							SetMood( const CHAR* szMood );

	// ��ѯ��ϵֵ
	INT								GetFriendPoint( GUID_t guid );

	// ���ӹ�ϵֵ
	VOID							IncFriendPoint( GUID_t guid, INT nPoint = 1 );

	// ���ٹ�ϵֵ
	VOID							DecFriendPoint( GUID_t guid, INT nPoint = 1 );

	// ���÷��飨�ýӿڴ�����Ϊ�˿ͻ�����ʾ�����ã�
	BOOL							SetFriendGroup( GUID_t guid, RELATION_GROUP RelationGroup );

	// ����ϵ�б���Ϣ�ṹ
	VOID							FillRelationList( GC_RELATIONLIST& RelationList );

	// ���¹�ϵ�����ݣ������ظ��º�Ĺ�ϵ������
	const _OWN_RELATION*			UpdateRelationInfo( _RELATION* pRelationData );

	// ��ù�ϵ������
	const _OWN_RELATION*			GetRelationInfo( GUID_t guid );

	// ͨ�����ֻ�ù�ϵ������
	const _OWN_RELATION*			GetRelationInfo( const CHAR *szName );

	// ����ϵ�������е�ȱ�������
	//BOOL							CompleteRelationInfo( _RELATION& RelationData );

	// �����ǽ����ص�һЩ����

	// �Ƿ��ѻ�
	BOOL							IsMarried();

	// �õ���ż�� GUID
	GUID_t							GetSpouseGUID();

	// ��ĳ�� GUID ��Ӧ����ҽ�飨�����ﲻ���Ա�
	BOOL							Marry( GUID_t guid );

	// ���
	BOOL							Divorce( );

	// ������ʦͽ��һЩ��ز���

	// �ж� guid ��Ӧ������Ƿ����Լ���ʦ��
	BOOL							IsMaster( GUID_t guid );

	// �õ�ʦ���� GUID
	GUID_t							GetMasterGUID();

	// �ж� guid ��Ӧ������Ƿ����Լ���ͽ��
	BOOL							IsPrentice( GUID_t guid );

	// �ж��Ƿ���ʦ��
	BOOL							HaveMaster( );

	// ȡ���м���ͽ��
	INT								GetPrenticeCount();

	// ȡ�õ� nIndex ��ͽ�ܵ� GUID
	GUID_t							GetNthPrentice( INT nIndex );

	// ȡ��ʦ�µ�
	UINT							GetMoralPoint( );

	// ����ʦ�µ�
	BOOL							SetMoralPoint( UINT uPoint );

	// ȡ�����һ��ͽ����ʦʱ��ݵ�ǰʱ��ĳ��ȣ�����������
	UINT							GetPrenticeBetrayTime();

	// �� guid ��Ӧ�����Ϊʦ
	BOOL							Aprentice( GUID_t guid );

	// �� guid ��Ӧ�����Ϊͽ
	BOOL							Recruit( GUID_t guid );

	// ����ʦ��
	BOOL							LeaveMaster( );

	// ���� GUID Ϊ guid ��ͽ��
	BOOL							ExpelPrentice( GUID_t guid );

	// �����ǽ�ݵ�һЩ����

	// �ж��Ƿ��ǽ���ֵ�
	BOOL							IsBrother( GUID_t guid );

	// �����к��ѷ����ʼ�
	VOID							SendMailToAllFriend( const CHAR* szMail );

protected :
	inline const _RELATION_DB_LOAD*	GetRelationDB( );

	inline const _OWN_RELATION*		GetRelationList( );

	// ȡ����ϵ�˵����ݣ�����ָ���Լ� Index ֵ
	const _OWN_RELATION*			GetRelation( RELATION_TYPE RelationType, GUID_t guid, INT& index );

protected :
	Obj_Human*						m_pHuman;
};




#endif
