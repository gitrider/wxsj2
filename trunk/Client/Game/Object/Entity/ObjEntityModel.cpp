
#include "StdAfx.h"
#include "ObjEntityModel.h"
#include "../../Engine/EngineInterface.h"
#include "Core/FairySystem.h"
#include "Core/FairyObjectFactoryManager.h"
#include "Core/FairyLogicModelObject.h"
#include <OgreVector3.h>
#include "GIException.h"




#define	FOBJ_POSITION		_T("position")			// λ��
#define	FOBJ_ORIENTATION	_T("orientation")		// ��ת

#define FOBJ_MODEL_FILE		_T("model name")		// ��̬ģ����Դ����



CObjEntityModel::CObjEntityModel() : CObjectEntity()
{
	Fairy::System* pFairySystem = CEngineInterface::GetMe()->GetFairySystem();
	// ��Fairy�д���ʵ��
	m_pFairyObj = pFairySystem->getObjectFactoryManager()->createInstance(Fairy::String("Model"));
	// ��ogre�д�����Ⱦ����
	m_pFairyObj->createRenderInstance( pFairySystem );
	// ��thisָ�뱣����FairyData��
	m_pFairyObj->setData((Fairy::ulong)this);	
	// �ɱ����ѡ��
	m_bRayQuery = TRUE;
}

CObjEntityModel::~CObjEntityModel()
{
}


VOID CObjEntityModel::StaObj_SetMeshFile(LPCTSTR szMeshFile)
{
	KLAssert(szMeshFile && m_pFairyObj);

	m_pFairyObj->setProperty(FOBJ_MODEL_FILE, Fairy::String(szMeshFile));
}

VOID CObjEntityModel::SetVisible(BOOL bVisible)
{
	KLThrow("(Hide static object)Not Support yet!");

}


VOID CObjEntityModel::SetPosition(const fVector3& vPos)
{
	KLAssert(m_pFairyObj);

	// ����ת��
	fVector3 fvGfxPos;
	CEngineInterface::GetMe()->Axis_Trans(CEngineInterface::AX_GAME, vPos, CEngineInterface::AX_GFX, fvGfxPos);

	m_pFairyObj->setProperty(FOBJ_POSITION, Ogre::Vector3(fvGfxPos.x, fvGfxPos.y, fvGfxPos.z));
}

VOID CObjEntityModel::SetOrientation(const fVector3& vRotate)
{
	KLAssert(m_pFairyObj);

	Ogre::Quaternion qu(Ogre::Radian(vRotate.y), Ogre::Vector3::UNIT_Y);
	m_pFairyObj->setProperty(FOBJ_ORIENTATION, qu);
}
