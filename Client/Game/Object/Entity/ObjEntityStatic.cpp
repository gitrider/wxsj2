
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




#define	FOBJ_POSITION		_T("position")			// 位置
#define	FOBJ_ORIENTATION	_T("orientation")		// 旋转

#define FOBJ_MESH_FILE		_T("mesh name")			// 静态物体网格文件



CObjEntityStatic::CObjEntityStatic() : CObjectEntity()
{
	Fairy::System* pFairySystem = CEngineInterface::GetMe()->GetFairySystem();
	// 在Fairy中创建实体
	m_pFairyObj = pFairySystem->getObjectFactoryManager()->createInstance(Fairy::String("StaticEntity"));
	// 在ogre中创建渲染对象
	m_pFairyObj->createRenderInstance( pFairySystem );
	// 将this指针保留在FairyData中
	m_pFairyObj->setData((Fairy::ulong)this);	
	// 可被鼠标选中
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

	// 坐标转化
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

