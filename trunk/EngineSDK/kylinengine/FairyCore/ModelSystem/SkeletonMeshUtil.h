#ifndef _SkeletonMeshUtil_h_
#define _SkeletonMeshUtil_h_

#include "ModelTypes.h"
#include <OgrePrerequisites.h>



typedef std::vector<Ogre::String>			StringList;


class DObject_NT;

class  MODEL_API	SkeletonMeshUtil
{

public:
	SkeletonMeshUtil(){};
	~SkeletonMeshUtil(){};

//��Դ������
public: 
		//-----------------------------
		//Save �ӿ�,�ļ�����	*.obj  
		//-----------------------------
		static	bool	Save(DObject_NT* pMesh,Ogre::String& StrFileName,bool isBinary = false);

		//----------------------------//
		//Load �ӿ�
		//-----------------------------

		static  bool    Load(DObject_NT*			pMesh,
							const Ogre::String		strObjGroup,
							const Ogre::String&		strXmlFile,
							const StringList&		attribNames,
							const StringList&		attribValues);

		static  bool	LoadChild(DObject_NT*		pMesh,
							const Ogre::String		strObjGroup,
							const Ogre::String&		strXmlFile,
							const StringList&		attribNames,
							const StringList&		attribValues,
							Ogre::SkeletonInstance*		pParentSkeleton,
							Ogre::Bone*					pParentBone
							);
//�ڲ� ʵ�ַ���
private:
		
		//xml	���ش���
		static bool	LoadFromXml(DObject_NT* pMesh);
		//binary ���ش���
		static bool	LoadFrameBinary(DObject_NT* pMesh);
		//xml   ���̴���
		static bool	SaveToXml(DObject_NT* pMesh,Ogre::String StrFileName);
		//binary ���̴���
		static bool	SaveToBinary(DObject_NT* pMesh);

private:
		static void	_LoadEntity(DObject_NT* pMesh);
		static void	_LoadChildEntity(DObject_NT* pMesh);
};
	

#endif