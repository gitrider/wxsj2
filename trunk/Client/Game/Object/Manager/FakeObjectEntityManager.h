#pragma once
#include "GIGfxSystem.h"
#include <OgreVector3.h>
#include <OgreTexture.h>

namespace Ogre
{
	class Camera;
	class Viewport;
	class RenderTexture;
	class Rectangle2D;
};
class CObjEntityActor;
//����UI��ʾ��ģ������Ⱦ��Ĺ�����
class CFakeObjectEntityManager
{
public:
	//����UI��ʾ��ģ����Ⱦ����
	struct FakeObject
	{
		FakeObject()
			: strName("")
			, pCamera(NULL)            
			, pViewPort(NULL)
			, pEntityNode(NULL)
			, pAttachEntityNode(0)
			, fCameraHeight(0.0f)
			, fCameraDistance(0.0f)
			, fCameraPitch(0.0f)
			, pRectange(NULL)
		{
		}

		STRING					strName;			//����
		Ogre::Camera*			pCamera;			//Ogre�����
		Ogre::TexturePtr		ptrRenderTexture;	//������Ⱦ��Texture
		Ogre::Viewport*			pViewPort;			//������Ⱦ��View
		CObjEntityActor*		pEntityNode;		//��ʾ�ڵ㣬ֻ������ʾ�������𴴽�/ɾ��
		CObjEntityActor*		pAttachEntityNode;	//���ӽڵ㣬һ����������
		float					fCameraHeight;		//������߶�
		float					fCameraDistance;	//���������
		float					fCameraPitch;		//������Ƕ�
		Ogre::Rectangle2D*		pRectange;			//��Ⱦģ��
	};
	typedef std::map< STRING, FakeObject > FakeObjectMap;

public:
	//����һ������ڵ�
	void	CreateObject(LPCTSTR szObjName, CObjEntityActor* pNode, CObjEntityActor* pAttachNode, LPCTSTR szCameraName, int nTexWidth, int nTexHeight, LPCTSTR szBackgroundName);
	//����
	void	DestroyObject(LPCTSTR szObjName);
	//���ø�ģ���Ƿ�����UI����ʾ�����ڴ򿪺͹ر�ViewPort��ˢ�£�ȱʡ����ʾ
	void	SetVisible(LPCTSTR szObjName, BOOL bVisible, float fAspectRatio);
	//����������Ⱦ��ģ�ͣ�����ģ����Ϊ�ɼ���������Ϊ���ɼ�
	void	SetActiveObject(FakeObject* pObj);
	//������������
	bool	GetCamera(LPCTSTR szObjName, float& fHeight, float& fDistance, float& fPitch);
	//�������������
	bool	SetCamera(LPCTSTR szObjName, float fHeight, float fDistance, float fPitch);
	//
	bool	SetAttachObj(CObjEntityActor* pNode, CObjEntityActor* pAttachNode);

	//����ViewPort����Object
	FakeObject*		GetObjectAsViewPort(Ogre::Viewport* pViewPort);

	VOID Debug_SetFakeCamera(int nType, float fValue);

public:
	CFakeObjectEntityManager(void);
	~CFakeObjectEntityManager(void);

	static CFakeObjectEntityManager*	GetMe(VOID) { return s_pMe; }

	//��ʼ��
	VOID		Initial(VOID);
	//�ͷ���Դ
	VOID		Release(VOID);

protected:
	//�õ�ָ����Ⱦ�����ڵ㣬���û�оʹ������µģ�������Map��
	FakeObjectMap::iterator		_GetFakeNode(LPCTSTR szNodeName, tEntityNode* pNode, LPCTSTR szCameraName, int nTexWidth, int nTexHeight, LPCTSTR szBackgroundName);
	//���������
	void						_UpdateCamera(FakeObject& node);

	void						_DestroyFakeNode(FakeObject& obj);

	// ����obj����������ȡ���fake object���ڵ�λ��
	float                       _GetFakeObjPos(LPCTSTR szName);

protected:
	static CFakeObjectEntityManager*	s_pMe;

	//-------------------------------------------
	//��ʾ����
	FakeObjectMap			m_mapObject;
	//�ڳ����е�λ��
	Ogre::Vector3 			m_fvPosition;			

	//����ViewPort���������ݱ�
	std::map< Ogre::Viewport*, FakeObject* >	m_mapIndexOfViewPort;

	// ��������fake object��λ��
	typedef std::map<STRING, float> FakeObjectPositionMap;
	FakeObjectPositionMap m_FakeObjectPositionMap;
};
