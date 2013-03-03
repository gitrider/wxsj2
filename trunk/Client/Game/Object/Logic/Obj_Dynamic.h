
#pragma once


#include "Obj_Map.h"



#define MINI_MOVE_DISTANCE		(0.05f)

typedef std::list< fVector3 > CPathNodeList;


/** ע��������,���ƶ������� 
 */
class CObject_Dynamic : public CObject_Map
{
public:

	CObject_Dynamic();
	virtual ~CObject_Dynamic();

private:

	BOOL					m_bServerPath;		// �Ƿ���SERVER��������·��
	UINT					m_dwPathID;

	CPathNodeList			m_listPathNode;
	fVector3				m_posPathNodePrev;

protected:

	BOOL IsServerPath( VOID ) const											{ return m_bServerPath; }

	const fVector3 *GetPrevPathNode( VOID ) const							{ return &m_posPathNodePrev; }
	const fVector3 *GetBeginPathNode( VOID ) const;
	VOID AddPathNode( BOOL bServerPath, UINT dwPathID, const fVector3 *pfvNode );
	VOID CleanupPathNode( VOID );

	BOOL BeginPathNode( VOID );
	BOOL NextPathNode( VOID );
	BOOL CalcMoveDir( VOID );
	BOOL DoMove( FLOAT fElapseTime, FLOAT fSpeed );

	virtual VOID OnNextPathNode( VOID );
	virtual VOID OnMoveStop( VOID );

public:

	UINT GetPathID( VOID ) const											{ return m_dwPathID; }

	const fVector3* GetCurMoveTargetPos( VOID ) const						{ return GetBeginPathNode(); }

	const CPathNodeList* GetPathNodeList( VOID ) const						{ return &m_listPathNode; }

protected:

	KL_DECLARE_DYNAMIC(CObject_Dynamic);
	KL_DECLARE_LOGICAL(true);

};
