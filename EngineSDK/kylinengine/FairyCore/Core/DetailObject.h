#pragma once
#include "FairyPlacementObject.h"
#include "fairyobjectproxy.h"
#include "ogremesh.h"
#include <OgreImage.h>

namespace Fairy
{
	class DetailObject;
	class ObjectFactory;

	struct DetailObjectLoadder : public Ogre::ManualResourceLoader
	{
		virtual void loadResource( Ogre::Resource* resource) ;
		Ogre::Mesh*		mMesh;
		System*			mSystem;
		Ogre::Image*	mImage;
		DetailObject*	mObj;
		int nSplitNumber;
		float nDensity;
		float nRandPos;
		float nDetailWidth;
		float nDetailHeight;
		float nDetailRandSize;
		float nRegionWidth;
		float nRegionHeight;
		float nAngleRand;
	};

	struct DetailData
	{
		Ogre::MeshPtr	mMesh;
		Ogre::Entity*	mEntity;
		DetailObject*	mObj;
		void build( System *pSystem,  int nSplitNumber, float nDensity, float nRandPos, float nDetailWidth, float nDetailHeight, float nDetailRandSize, float nRegionWidth, float nRegionHeight );
		void build( System *pSystem,  int nSplitNumber, Ogre::Image *pImage, float nDensity, float nRandPos, float nDetailWidth, float nDetailHeight, float nDetailRandSize, float nRegionWidth, float nRegionHeight );
		void destroy();

		DetailObjectLoadder	mDetailObjLoadder;
		DetailData() : mEntity(NULL)	{}
	};

	class DetailObject :
		public PlacementObject
	{
	protected:
		 class Factory;
	public:
		DetailObject(void);
		~DetailObject(void);

		static ObjectFactory* getFactory(void);
		static const String msType;
        static const String msCategory;

	public:
		void	addBaseProperty();
		const String& getType(void) const;
		 const String& getCategory(void) const;
		 void createRenderInstance(System* system);
		 void destroyRenderInstance(void);
		 void updateRenderInstance();

		 void	createDetailObject();
		 void	destroyDetailObject();
		 void	updateDetailObject();

		 void	waveGrass(Ogre::Real nTimePass);

		 const String&	getDetailFileName()	const {return mDetailFileName;}
		 void	setDetailFileName( const String &sFilename );

		 void	onPosChange();

	protected:
		System*	mSystem;
		 ObjectProxy* mProxy;
		DetailData	mDetailData;
		Ogre::Controller<Real>*	mController;
		Ogre::Image	mImage;
		
		
	
	public:
		String	mMaterialName;		
		String  mDetailFileName;
		int			mSplitNumber;	///one split number
		float		mRandPos;
		float		mDensity;
		float		mDetailWidth;
		float		mDetailHeight;
		float		mDetailRandSize;
		float		mRegionWidth;
		float		mRegionHeight;


		Ogre::String	mImageName;
		Ogre::String	mOldImageName;

		float		mLightThrehold1;
		float		mLightThrehold2;
		float		mLightThrehold3;
		float		mLightThrehold4;

	protected:
			Ogre::Real xinc;
			Ogre::Real zinc;
			Ogre::Real xpos;
			Ogre::Real zpos;
			Ogre::Vector4 offset;
		
	};
}

