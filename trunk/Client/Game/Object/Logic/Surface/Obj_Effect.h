
/** Obj_Effect.h
 */

#ifndef __OBJ_EFFECT_H__
#define __OBJ_EFFECT_H__


#include "Type.h"
#include "Obj_Surface.h"



struct SObject_EffectInit :	public SObjectInit
{
	const CHAR*		m_pszEffectName;
	BOOL			m_bLoopEffect;


	SObject_EffectInit( VOID )
	{
		m_pszEffectName		= NULL;
		m_bLoopEffect		= FALSE;
	}

	virtual VOID Reset( VOID )
	{
		SObjectInit::Reset();
		m_pszEffectName		= NULL;
		m_bLoopEffect		= FALSE;
	}
};



/** 特效对象
 */
class CObject_Effect : public CObject_Surface
{
public:
	
	CObject_Effect();
	virtual ~CObject_Effect();

	
	virtual	VOID Initial( VOID *pInit );
	virtual VOID Release( VOID );

	virtual VOID Tick( VOID );

	virtual BOOL IsEffectStopped( VOID )const;

protected:
	
	VOID ChangEffect( const CHAR *pszEffectName, BOOL bLoop );


protected:
	
	BOOL			m_bLoopEffect;
	FLOAT			m_fEffectTime;

protected:

	KL_DECLARE_DYNAMIC(CObject_Effect);
	KL_DECLARE_LOGICAL(true);

};


#endif // __OBJ_EFFECT_H__
