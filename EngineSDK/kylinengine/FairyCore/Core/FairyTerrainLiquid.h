#ifndef __FAIRYTERRAINLIQUID_H__
#define __FAIRYTERRAINLIQUID_H__

#include "FairyPrerequisites.h"

#include <OgreMovableObject.h>

namespace Fairy {
    class Terrain;
    class System;

    class LiquidRenderable;

    class TerrainLiquid : public Ogre::MovableObject
    {
    protected:

        /// Shared class-level name for Movable type
        static const Ogre::String msMovableType;
        /// Static member used to automatically generate names for TerrainLiquid objects.
        static Ogre::uint ms_uGenNameCount;

        Terrain* mTerrain;
        System* mSystem;

		Ogre::Matrix4	mReflectRefractMatrix;

        Ogre::Vector3 mPosition;
        size_t mSubdivision;
        size_t mVertexLimit;
        Ogre::Real mTexcoordScale;
        Ogre::Radian mTexcoordRotate;
        bool mDepthTexLayerEnabled;
        Ogre::Real mDepthTexLayerScale;
        Ogre::Real mDepthTexLayerAdjust;
        Ogre::Real mAnimationIteratorInterval;
       Ogre::String mMaterialName;
        Ogre::MaterialPtr mMaterial;
		Ogre::MaterialPtr mClonedMaterial;
        Ogre::Camera* mProjectionCamera;

		Ogre::ColourValue mDiffuse;
		Ogre::Real mAdjustDepth;

		Ogre::ColourValue mMatSpecular;
		Ogre::Real mShininess;
        enum VertexFlag
        {
            VF_USED     = 1 << 0,
            VF_ANIMABLE = 1 << 1,
        };

        uint8* mVertexFlags;
        float* mAllocatedBuffer;
        float* mLevelBuffers[3];        // we need 3 level buffers
        float* mPositionBuffer;         // storage positions in worldspace, for calculate position and normal fast
		Ogre::uint32* mDiffuseBuffer;	// storage diffuse if water vertex
        Ogre::Vector3* mNormalBuffer;   // space for normal calculation
        size_t mUsedBaseX;
        size_t mUsedBaseZ;
        size_t mUsedWidth;
        size_t mUsedHeight;
        size_t mNumUsedVertices;
        Ogre::AxisAlignedBox mBoundingBox;

        Ogre::Real mLastFrameTime;
        Ogre::Real mRemainTimeStamp;
        bool mVertexBuffersDirty;
        bool mTexcoordsDirty;

        Ogre::VertexData* mVertexData;                      // vertex data shared by all renderable
        Ogre::HardwareIndexBufferSharedPtr mIndexBuffer;    // index buffer shared by all renderable
        typedef std::vector<LiquidRenderable*> LiquidRenderableList;
        LiquidRenderableList mLiquidRenderables;
        Ogre::Camera* mCurrentCamera;

        void updateLevelBuffers(void);
        void updateVertexBuffers(void);
        void updateRenderables(void);

        void initBuffers(void);

        void createHardwareBuffers(size_t vertexCount, size_t faceCount);
        void calculatePositions(void);
        void calculateNormals(void);
        void manageTexcoords(void);
		void calculateDiffuse(void);

		mutable Ogre::FogMode mCurrentFogMode;  //!< Current materials fog mode
		
		void _updateMaterials(void) const;
		void _applyFogMode(const Ogre::MaterialPtr& material, Ogre::FogMode oldFogMode, Ogre::FogMode newFogMode) const;
		bool _checkFogProgramName(const Ogre::GpuProgramPtr& program, Ogre::FogMode oldFogMode, Ogre::FogMode newFogMode,Ogre::String& newProgramName) const;

    public:
		TerrainLiquid(System* system, const Ogre::Vector3& position, size_t subdivision, size_t vertexLimit, Ogre::ColourValue diffuse = Ogre::ColourValue::White, Ogre::Real depth = 0, Ogre::ColourValue matSpecular = Ogre::ColourValue::White, Ogre::Real shininess = 64.f);
        ~TerrainLiquid();

        bool build(void);
        void cleanup(void);

        void setMaterialName(const Ogre::String& materialName);
        const Ogre::String& getMaterialName(void) const;

        void setTexcoordScale(Ogre::Real scale);
        Ogre::Real getTexcoordScale(void) const;
        void setTexcoordRotate(Ogre::Radian rotate);
        Ogre::Radian getTexcoordRotate(void) const;

        void setDepthTexLayerSettings(bool enable, Ogre::Real scale = 1, Ogre::Real adjust = 0);
        bool isDepthTexLayerEnabled(void) const;
        Ogre::Real getDepthTexLayerScale(void) const;
        Ogre::Real getDepthTexLayerAdjust(void) const;

		void visitRenderables(Ogre::Renderable::Visitor* visitor, 
			bool debugRenderables = false);

        void setProjectionCamera(Ogre::Camera* camera);

        /** "pushes" a mesh at position [x,z]. Note, that x,z are float in worldspace, hence 
        *	4 vertices are actually pushed
        *	@note 
        *		This should be replaced by push with 'radius' parameter to simulate
        *  		big objects falling into water
        */
        void push(Ogre::Real x, Ogre::Real z, Ogre::Real depth, bool absolute=false) ;

        /** gets height at given x and z in worldspace, takes average value of the closes nodes */
        Ogre::Real getHeight(Ogre::Real x, Ogre::Real z);

        /** updates mesh */
        void update(Ogre::Real timeSinceLastFrame) ;

        Ogre::Real PARAM_C; // ripple speed 
        Ogre::Real PARAM_D; // distance
        Ogre::Real PARAM_U; // viscosity
        Ogre::Real PARAM_T; // time
        bool useFakeNormals;

    public:
        /** Overridden from MovableObject */
        const Ogre::String& getMovableType(void) const;

        /** Returns the bounding box of this TerrainTile */
        const Ogre::AxisAlignedBox& getBoundingBox(void) const;

        /** Overridden from MovableObject */
	    Ogre::Real getBoundingRadius(void) const;

        /** Updates the level of detail to be used for rendering this TerrainTile based on the passed in Camera */
        void _notifyCurrentCamera(Ogre::Camera* camera);

        /** Overridden from MovableObject */
        void _updateRenderQueue(Ogre::RenderQueue *queue);


		void setReflectRefractMatrix( const Ogre::Matrix4 &m ){mReflectRefractMatrix = m; };

		float	mFarDistanceOld;
    };

}

#endif 
