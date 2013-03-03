#ifndef __TERRAINDATA_H__
#define __TERRAINDATA_H__

#include "FairyPrerequisites.h"

#include <OgreVector3.h>
#include <OgreAxisAlignedBox.h>
#include <OgreColourValue.h>
#include <OgreCommon.h>
#include <OgreVector2.h>

namespace Fairy {

class XMLParser;
class XMLWriter;

class TerrainData
{
public:

	// NOTE: Some code dependence with this enum, search [Corner Map] for detail
    enum OrientationFlags {
	    FlipHorizontal		= 1,	//!< Horizontal flip the pixmap
	    FlipVertical		= 2,	//!< Vertical flip the pixmap
	    RotateRightAngle	= 4,	//!< Rotate the pixmap anticlockwise 90 degree
        MirrorDiagonal      = 8,    //!< Mirror the pixmap by diagonal
    };

	// NOTE: Some code dependence with this enum, search [Corner Map] for detail
    enum Corner {
        LEFT_TOP            = 0,
        RIGHT_TOP           = 1,
        LEFT_BOTTOM         = 2,
        RIGHT_BOTTOM        = 3,
    };

    enum {
        NumLayers = 2,              //!< number texture layers
    };

	// NOTE: Some code dependence with fields order of this structure, search [Pixmap Fields Order] for detail
    struct Pixmap {
        size_t textureId;           //!< the texture this pixmap reference to, access by mTextures[textureId]
        Ogre::Real left;                  //!< defining the left of area on the texture that makes up this pixmap
        Ogre::Real top;                   //!< defining the top of area on the texture that makes up this pixmap
        Ogre::Real right;                 //!< defining the right of area on the texture that makes up this pixmap
        Ogre::Real bottom;                //!< defining the bottom of area on the texture that makes up this pixmap

        Pixmap()
        {
            // null constructor
        }

        explicit Pixmap(size_t textureId)
            : textureId(textureId)
            , left(0)
            , top(0)
            , right(1)
            , bottom(1)
        {
        }

        Pixmap(size_t textureId, Ogre::Real left, Ogre::Real top, Ogre::Real right, Ogre::Real bottom)
            : textureId(textureId)
            , left(left)
            , top(top)
            , right(right)
            , bottom(bottom)
        {
        }

        bool operator== (const Pixmap& rhs) const
        {
            return textureId == rhs.textureId &&
                   left == rhs.left && right == rhs.right &&
                   top == rhs.top && bottom == rhs.bottom;
        }

        bool operator!= (const Pixmap& rhs) const
        {
            return !(*this == rhs);
        }

        bool operator< (const Pixmap& rhs) const
        {
            if (textureId < rhs.textureId) return true;
            if (textureId > rhs.textureId) return false;
            if (left < rhs.left) return true;
            if (left > rhs.left) return false;
            if (top < rhs.top) return true;
            if (top > rhs.top) return false;
            if (right < rhs.right) return true;
            if (right > rhs.right) return false;
            if (bottom < rhs.bottom) return true;
            if (bottom > rhs.bottom) return false;
            return false;
        }
    };

    struct LayerInfo {
        ushort pixmapId;            //!< the pixmap this layer info reference to, 0 means hole, otherwise access by mPixmaps[pixmapId-1]
        ushort orientation;         //!< combine flags of OrientationFlags

        bool operator== (const LayerInfo& rhs) const
        {
            return pixmapId == rhs.pixmapId &&
                   orientation == rhs.orientation;
        }

        bool operator!= (const LayerInfo& rhs) const
        {
            return !(*this == rhs);
        }
    };

    struct GridInfo {
        LayerInfo layers[NumLayers];
        Ogre::uint flags;

        enum Flag
        {
            FlipDiagonal    = 1 << 0,
            DisableDecal    = 1 << 1,
        };

        const Corner* getCorners(void) const
        {
            return msConnerMap[(flags & FlipDiagonal) ? 1 : 0];
        }

        bool operator== (const GridInfo& rhs) const
        {
            return layers[0] == rhs.layers[0] &&
                   layers[1] == rhs.layers[1] &&
                   flags == rhs.flags;
        }

        bool operator!= (const GridInfo& rhs) const
        {
            return !(*this == rhs);
        }
    };

    struct SurfaceParams
    {
        Ogre::ColourValue ambient;
        Ogre::ColourValue diffuse;
        Ogre::ColourValue specular;
        Ogre::ColourValue emissive;
        Ogre::Real shininess;

        bool operator== (const SurfaceParams& rhs) const
        {
            return ambient == rhs.ambient &&
                   diffuse == rhs.diffuse &&
                   specular == rhs.specular &&
                   emissive == rhs.emissive &&
                   shininess == rhs.shininess;
        }

        bool operator!= (const SurfaceParams& rhs) const
        {
            return !(*this == rhs);
        }
    };

    struct FogReplacement
    {
       Ogre::String programNames[4];      // access by fog mode
    };

    typedef std::vector<GridInfo> GridInfoArray;

    struct GridInfoCopy
    {
       Ogre::String        fileName;
        GridInfoArray gridInfos;
        uint32        touchTimes;
        uint32        gridCount;
    };

    typedef std::vector<GridInfoCopy> GridInfoArrayCopys;

    typedef std::vector<Ogre::Real> RealArray;
    typedef std::vector<Pixmap> PixmapArray;
    typedef std::vector<String> TextureArray;
    typedef std::map<String,Ogre::String> MaterialTemplates;
    typedef std::vector<FogReplacement> FogReplacements;

    // Access by flip diagonal or not
    static const Corner msConnerMap[2][4];

    // Default surface params
    static const SurfaceParams msDefaultSurfaceParams;

    // options

    int mXSize;							//!< X dimension of the map (in grids)
    int mZSize;							//!< Z dimension of the map (in grids)
    int mTileSize;						//!< Dimensions of each tile (in grids)
    Ogre::Vector3 mScale;				//!< World space unit of each grid
    Ogre::Vector3 mPosition;			//!< World space position of the first grid	

    // derived fields

    Ogre::Vector3 mInvScale;        //!< 1 / mScale
    Ogre::Vector3 mCentre;          //!< The centre of the terrain
    Ogre::Real mMinX, mMaxX;
    Ogre::Real mMinZ, mMaxZ;
    int mNumTilePerX;               //!< (width - 1) / tileSize + 1
    int mNumTilePerZ;               //!< (depth - 1) / tileSize + 1
    mutable Ogre::AxisAlignedBox mBoundBox; //!< The bound box

    // artist's data

    RealArray mHeightmap;           //!< Height of each end point, ((width+1) * (depth+1)) elements
    GridInfoArray mGridInfos;       //!< Grid info array, (width * depth) elements
    PixmapArray mPixmaps;           //!< The pixmaps the each layer reference to
    TextureArray mTextures;         //!< Textures that pixmap is referenced to
    Ogre::Image* mLightmapImage;	//!< Lightmap image of terrain, NULL if not exists

    //GridInfoArrayCopys mGridInfoArrayCopys; //WANGLING:载入同一个地图时不刷新导致bug，先注掉

   Ogre::String mName;                   //!< The name of the grid scene

   Ogre::String mResourceGroupName;      //!< The resource group name contains terrain resources
   Ogre::String mHeightmapType;          //!< The height-map file type
   Ogre::String mHeightmapFilename;      //!< The height-map file name
	String mLightmapType;
	String mLightmapFilename;		//!< The light-map file name
   Ogre::String mGridInfoFilename;       //!< The grid info file name 

    /// The material templates
    MaterialTemplates mMaterialTemplates;
    /// The fog replacements
    FogReplacements mFogReplacements;
    /// The supported fog replacements
    FogReplacements mSupportedFogReplacements;

	/// The fog replacements for liquid
	FogReplacements mWaterFogReplacements;
	/// The supported fog replacements for liquid
	FogReplacements mWaterSupportedFogReplacements;

    SurfaceParams mSurfaceParams;	

	typedef Ogre::Vector2 Point;
 	struct PointLess
 	{
 		bool operator()(const Point& v1, const Point& v2) const
 		{
			if (v1.x < v2.x) return true;
			if (v1.x == v2.x && v1.y < v2.y) return true;

 			return false;
 		}
 	};
	
	typedef std::map<Point, Ogre::ColourValue, PointLess> DiffuseInfoMap;
	DiffuseInfoMap mDiffuseInfoMap;

	DiffuseInfoMap mLightDiffuseInfoMap;

public:

	void _computeTileBoundingBox(Ogre::AxisAlignedBox& box, int startx, int startz, int width, int depth) const;

	Ogre::Vector3 _convertPosition(Ogre::Real x, Ogre::Real h, Ogre::Real z) const
	{
		return Ogre::Vector3(mPosition.x + mScale.x * x,
					         mPosition.y + mScale.y * h,
                             mPosition.z + mScale.z * z);
	}

    size_t _getJunctionIndex(int x, int z) const
    {
		assert(isValidJunction(x, z));
        return z * (mXSize + 1) + x;
    }

	size_t _getLightmapSamplingIndex(int x, int z) const
	{
		assert(isValidLightmapSampling(x, z));
		return z * (mXSize << 3) + x;
	}

    size_t _getJunctionIndex(const std::pair<int, int>& coord) const
    {
		return _getJunctionIndex(coord.first, coord.second);
    }

    size_t _getGridIndex(int x, int z) const
    {
        assert(isValidGrid(x, z));
        return z * mXSize + x;
    }

    size_t _getGridIndex(const std::pair<int, int>& coord) const
    {
        return _getGridIndex(coord.first, coord.second);
    }

    /** Returns the terrain RAW height at the given junction coordinates */
    Ogre::Real _getRawHeight(int x, int z) const
	{
		return mHeightmap[_getJunctionIndex(x, z)];
	}

    /** Returns the terrain world space position at the given junction coordinates */
    Ogre::Vector3 _getPosition(int x, int z) const
	{
		return _convertPosition(x, _getRawHeight(x, z), z);
	}

    /** Returns the terrain normal at the given junction coordinates */
    Ogre::Vector3 _getNormal(int x, int z) const;

    std::pair<bool, Ogre::Real> _intersectGrid(const Ogre::Ray& ray, int x, int z,
        bool positiveSide, bool negativeSide) const;

    int _registerPixmap(const Ogre::String& textureName, Ogre::Real left, Ogre::Real top, Ogre::Real right, Ogre::Real bottom);

    /** Optimize the terrain data
    @remark
        Remove the useless pixmaps and textures, so we has more slot
        to store the pixmap and texture infomation. Re-sort pixmaps
        and textures for more effective.
    @return
        true if the terrain is compactable (so the terrain data is modified,
        you need to rebuild terrain renderable), false otherwise.
    */
    bool _optimize(void);

    bool _reshape(int left, int top, int right, int bottom, size_t pixmapId = 0);

public:

    void _updateDerivedFields(void);

protected:
    void _loadBaseInfo(XMLParser* parser, const Ogre::String& filename, const Ogre::String& groupName);
    void _saveBaseInfo(XMLWriter* writer, const Ogre::String& filename) const;

    void _fixupMissingInfo(void);
    void _fixupSupportedMaterials(void);

    void _loadHeightmap(const Ogre::String& filename, const Ogre::String& type, const Ogre::String& groupName);
    void _saveHeightmap(const Ogre::String& filename) const;

	void _loadLightmap(const Ogre::String& filename, const Ogre::String& type, const Ogre::String& groupName);
	void _saveLightmap(const Ogre::String& filename) const;

    void _loadGridInfo(const Ogre::String& filename, const Ogre::String& groupName);
    void _saveGridInfo(const Ogre::String& filename) const;

    bool _findGridInfoCopy(const Ogre::String& fileName, size_t& index);
    void _sortGridInfoCopys(void);

public:
    TerrainData();
    ~TerrainData();

    void reset(void);
    void load(XMLParser* parser, const Ogre::String& filename, const Ogre::String& groupName, bool loadLightmap);
    void save(XMLWriter* writer, const Ogre::String& filename, bool preservingNames = false) const;		
    bool reloadLightmap(void);

    const Ogre::AxisAlignedBox& getBoundBox() const;

    int getXSize(void) const
    {
        return mXSize;
    }

    int getZSize(void) const
    {
        return mZSize;
    }

    // Check the terrain is empty
    bool isEmpty(void) const
    {
        return mXSize <= 0 || mZSize <= 0;
    }

    // Check the given values is a valid junction coordinates
    bool isValidJunction(int x, int z) const
    {
        return !isEmpty() && 0 <= x && x <= mXSize && 0 <= z && z <= mZSize;
    }

	// Check the given values is a valid lightmap sampling coordinates
	bool isValidLightmapSampling(int x, int z) const
	{
		return !isEmpty() && 0 <=x && x < (mXSize << 3) && 0 <= z && z < (mZSize << 3);
	}

    // Check the given values is a valid junction coordinates
    bool isValidJunction(const std::pair<int, int>& coord) const
    {
        return isValidJunction(coord.first, coord.second);
    }

    // Check the given values is a valid grid coordinates
    bool isValidGrid(int x, int z) const
    {
        return 0 <= x && x < mXSize && 0 <= z && z < mZSize;
    }

    // Check the given values is a valid grid coordinates
    bool isValidGrid(const std::pair<int, int>& coord) const
    {
        return isValidGrid(coord.first, coord.second);
    }

    // Check the given values is a valid world coordinates
    bool isValidCoord(Ogre::Real x, Ogre::Real z) const
    {
        return mMinX <= x && x <= mMaxX && mMinZ <= z && z <= mMaxZ;
    }

    /** Returns the terrain world space height at the given junction coordinates */
    Ogre::Real getHeight(int x, int z) const
	{
		return mPosition.y + _getRawHeight(x, z) * mScale.y;
	}

    /** Returns the terrain world space height at the given junction coordinates */
    Ogre::Real getHeight(const std::pair<int, int>& coord) const
	{
		return getHeight(coord.first, coord.second);
	}

    /** Set the terrain world space height at the given junction coordinates */
    void setHeight(int x, int z, Ogre::Real height)
    {
        mHeightmap[_getJunctionIndex(x, z)] = (height - mPosition.y) * mInvScale.y;
    }

    /** Set the terrain world space height at the given junction coordinates */
    void setHeight(const std::pair<int, int>& coord, Ogre::Real height)
    {
        setHeight(coord.first, coord.second, height);
    }

    void setGridInfo(int x, int z, const GridInfo& info)
    {
        mGridInfos[_getGridIndex(x, z)] = info;
    }

    void setGridInfo(int index, const GridInfo& info)
    {
        mGridInfos[index] = info;
    }

    void setGridInfo(const std::pair<int, int>& coord, const GridInfo& info)
    {
        setGridInfo(coord.first, coord.second, info);
    }

	void setDiffuseInfo(int x, int z, const Ogre::ColourValue& color);
	Ogre::ColourValue getDiffuseInfo(int x, int z);
	void setLightDiffuseInfo(int x, int z, const Ogre::ColourValue& color);
	Ogre::ColourValue getLightDiffuseInfo(int x, int z);

	void setDiffuseApplyInfo(int x, int z, bool bApply);
	bool getDiffuseApplyInfo(int x, int z);

    /** Returns the nearest terrain junction at the given world space coordinates */
    std::pair<int, int> getJunction(Ogre::Real x, Ogre::Real z) const;

    std::pair<Ogre::Real, Ogre::Real> worldToJunction(Ogre::Real x, Ogre::Real z) const;
    std::pair<Ogre::Real, Ogre::Real> junctionToWorld(Ogre::Real x, Ogre::Real z) const;

    /** Returns the terrain grid at the given world space coordinates */
    std::pair<int, int> getGrid(Ogre::Real x, Ogre::Real z) const;

    std::pair<Ogre::Real, Ogre::Real> worldToGrid(Ogre::Real x, Ogre::Real z) const;
    std::pair<Ogre::Real, Ogre::Real> gridToWorld(Ogre::Real x, Ogre::Real z) const;

    /** Returns the terrain height at the given world space coordinates */
    Ogre::Real getHeightAt(Ogre::Real x, Ogre::Real z) const;

	/** Returns the terrain height at the given grid space coordinates */
    Ogre::Real getHeightAtGrid(Ogre::Real x, Ogre::Real z) const;

    /** Returns the terrain position at the given world space coordinates */
    Ogre::Vector3 getPositionAt(Ogre::Real x, Ogre::Real z) const;

    /** Returns the terrain position at the given grid space coordinates */
    Ogre::Vector3 getPositionAtGrid(Ogre::Real x, Ogre::Real z) const;

    /** Returns the terrain normal at the given world space coordinates */
    Ogre::Vector3 getNormalAt(Ogre::Real x, Ogre::Real z) const;

    /** Returns the terrain position and normal at the given world space coordinates */
    void getPositionAndNormalAt(Ogre::Real x, Ogre::Real z, Ogre::Vector3& position, Ogre::Vector3& normal) const;

    /** Returns the terrain height at the given grid centre */
    Ogre::Real getGridHeight(int x, int z) const;

    /** Returns the terrain position at the given grid centre */
    Ogre::Vector3 getGridPosition(int x, int z) const;

    /** Return the info of the grid at the given grid */
    const GridInfo & getGridInfo(int x, int z) const
    {
        return mGridInfos[ _getGridIndex(x,z) ];
    }

    /** Ray / terrain intersection, returns boolean result and
        the intersect point postion/normal at terrain. */
    bool getIntersects(const Ogre::Ray& ray, Ogre::Vector3& position, Ogre::Vector3* normal = NULL, bool allowOutside = false,
        bool positiveSide = true, bool negativeSide = false) const;

    /** Frustum / terrain intersection, returns an array of positions and normals,
        the positions/normals groupped by triangle of the intersection */
    size_t getIntersectsTriangles(const Ogre::Frustum* frustum, int areaLimit,
        bool receivedDecalOnly,
        std::vector<Ogre::Vector3>& positions, std::vector<Ogre::Vector3>* normals = NULL);
};

}

#endif // 
