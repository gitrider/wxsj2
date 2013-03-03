
/** 
 */

#include "StdAfx.h"
#include "ObjEntityStatic.h"
#include "../../Engine/EngineInterface.h"
#include "Core/FairySystem.h"
#include "Core/FairyObjectFactoryManager.h"
#include "Core/FairyLogicModelObject.h"
#include <OgreVector3.h>
#include "GIException.h"




#define	FOBJ_POSITION		_T("position")			// λ��
#define	FOBJ_ORIENTATION	_T("orientation")		// ��ת

#define FOBJ_MESH_FILE		_T("mesh name")			// ��̬���������ļ�



CObjEntityStatic::CObjEntityStatic() : CObjectEntity()
{
	Fairy::System* pFairySystem = CEngineInterface::GetMe()->GetFairySystem();
	// ��Fairy�д���ʵ��
	m_pFairyObj = pFairySystem->getObjectFactoryManager()->createInstance(Fairy::String("StaticEntity"));
	// ��ogre�д�����Ⱦ����
	m_pFairyObj->createRenderInstance( pFairySystem );
	// ��thisָ�뱣����FairyData��
	m_pFairyObj->setData((Fairy::ulong)this);	
	// �ɱ����ѡ��
	m_bRayQuery = TRUE;
}

CObjEntityStatic::~CObjEntityStatic()
{
}


VOID CObjEntityStatic::StaObj_SetMeshFile(LPCTSTR szMeshFile)
{
	KLAssert(szMeshFile && m_pFairyObj);

	m_pFairyObj->setProperty(FOBJ_MESH_FILE, Fairy::String(szMeshFile));
}

VOID CObjEntityStatic::SetVisible(BOOL bVisible)
{
	KLThrow("(Hide static object)Not Support yet!");

}


VOID CObjEntityStatic::SetPosition(const fVector3& vPos)
{
	KLAssert(m_pFairyObj);

	// ����ת��
	fVector3 fvGfxPos;
	CEngineInterface::GetMe()->Axis_Trans(CEngineInterface::AX_GAME, vPos, CEngineInterface::AX_GFX, fvGfxPos);

	m_pFairyObj->setProperty(FOBJ_POSITION, Ogre::Vector3(fvGfxPos.x, fvGfxPos.y, fvGfxPos.z));
}

VOID CObjEntityStatic::SetOrientation(const fVector3& vRotate)
{
	KLAssert(m_pFairyObj);

	Ogre::Quaternion qu(Ogre::Radian(vRotate.y), Ogre::Vector3::UNIT_Y);
	m_pFairyObj->setProperty(FOBJ_ORIENTATION, qu);
}

