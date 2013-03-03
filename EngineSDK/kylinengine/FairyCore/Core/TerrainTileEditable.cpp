#include "TerrainTileEditable.h"
#include "Terrain.h"
#include "TerrainTileRenderable.h"

#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreRoot.h>

namespace Fairy {

class TerrainTileEditableRenderable : public TerrainTileRenderable
{
protected:
    size_t layerIndex;

public:
    TerrainTileEditableRenderable(TerrainTile *parent, size_t layerIndex)
        : TerrainTileRenderable(parent)
        , layerIndex(layerIndex)
    {
    }

    ~TerrainTileEditableRenderable()
    {
        // Only index data need to delete
        delete mRenderOp.indexData;
    }

    /** Overridden - see Renderable. */
    const Ogre::MaterialPtr& getMaterial(void) const
    {
        const Ogre::MaterialPtr* material =
            static_cast<TerrainTileEditable*>(mParent)->getOverrideMaterial(layerIndex);
        if (material && !material->isNull())
            return *material;

        return TerrainTileRenderable::getMaterial();
    }
};

TerrainTileEditable::TerrainTileEditable(Ogre::SceneNode* parent, Terrain *owner,
								   int xbase, int zbase, int xsize, int zsize)
    : TerrainTile(parent, owner, xbase, zbase, xsize, zsize)
	, mRenderables()
	, mVertexDatas()
    , mGeometryOutOfDate(true)
{
}

TerrainTileEditable::~TerrainTileEditable()
{
	destoryGeometry();
    freeBuffers();
}

const Ogre::MaterialPtr*
TerrainTileEditable::getOverrideMaterial(size_t layerIndex) const
{
    if (!mOwner->mDisplayGridType || layerIndex >= mOwner->mGridTypeInfos.size())
        return NULL;

    return &mOwner->mGridTypeInfos[layerIndex].material;
}

//-----------------------------------------------------------------------

const Ogre::String&
TerrainTileEditable::getMovableType(void) const
{
	static const Ogre::String type = "TerrainTileEditable";
    return type;
}

void
TerrainTileEditable::_updateRenderQueue(Ogre::RenderQueue* queue)
{
    if (mGeometryOutOfDate)
    {
        if (mVertexDatas.empty())
            initBuffers(mOwner->getData(), mXBase, mZBase, mXSize, mZSize);

        createGeometry(mOwner->getData(), mXBase, mZBase, mXSize, mZSize);
    }

    queueRenderables(queue, mRenderables);
}

//-----------------------------------------------------------------------
//david-<<
void TerrainTileEditable::visitRenderables(Ogre::Renderable::Visitor* visitor, 
							  bool debugRenderables)
{
	for (RenderableList::const_iterator it = mRenderables.begin(); it != mRenderables.end(); ++it)
	{
		Ogre::Renderable* pr = *it;
		visitor->visit(pr, 0, false);
	}

}
//david->>
void
TerrainTileEditable::destoryGeometry(void)
{
    destroyRenderables(mRenderables);

    mGeometryOutOfDate = true;
}

void
TerrainTileEditable::freeBuffers(void)
{
	for (std::vector<Ogre::VertexData*>::const_iterator iv = mVertexDatas.begin();
		 iv != mVertexDatas.end();
		 ++iv)
	{
		delete *iv;
	}
	mVertexDatas.clear();
}

void
TerrainTileEditable::fillLayerBuffer(size_t layer, const Ogre::HardwareVertexBufferSharedPtr& buffer)
{
    float* pFloat = static_cast<float*>(buffer->lock(Ogre::HardwareBuffer::HBL_DISCARD));

    const TerrainData* data = mOwner->getData();
    int xbase = mXBase;
    int zbase = mZBase;
    int xsize = mXSize;
    int zsize = mZSize;

    for (int z = 0; z < zsize; ++z)
    {
        for (int x = 0; x < xsize; ++x)
        {
            size_t grid = (zbase + z) * data->mXSize + (xbase + x);
            const TerrainData::GridInfo& gridInfo = data->mGridInfos[grid];
            const TerrainData::LayerInfo& layerInfo = gridInfo.layers[layer];
            if (layerInfo.pixmapId)
            {
                std::pair<Ogre::Real, Ogre::Real> t;
                t = mOwner->_getPixmapCorner(layerInfo, TerrainData::LEFT_TOP, gridInfo.flags);
                *pFloat++ = t.first; *pFloat++ = t.second;
                t = mOwner->_getPixmapCorner(layerInfo, TerrainData::RIGHT_TOP, gridInfo.flags);
                *pFloat++ = t.first; *pFloat++ = t.second;
                t = mOwner->_getPixmapCorner(layerInfo, TerrainData::LEFT_BOTTOM, gridInfo.flags);
                *pFloat++ = t.first; *pFloat++ = t.second;
                t = mOwner->_getPixmapCorner(layerInfo, TerrainData::RIGHT_BOTTOM, gridInfo.flags);
                *pFloat++ = t.first; *pFloat++ = t.second;
            }
            else
            {
                pFloat += 2 * 4;
            }
        }
    }

    buffer->unlock();
}

void
TerrainTileEditable::initBuffers(TerrainData* data, int xbase, int zbase, int xsize, int zsize)
{
	int x, z;
	float* pFloat;
	uint32* pDiffuse;

	size_t vertexCount = xsize * zsize * 4;

	// Create and fill hardware buffers
	Ogre::HardwareVertexBufferSharedPtr posNormBuffer;
	Ogre::HardwareVertexBufferSharedPtr diffuseBuffer;
	Ogre::HardwareVertexBufferSharedPtr layerBuffer[2];
	Ogre::HardwareVertexBufferSharedPtr lightmapBuffer;
	Ogre::HardwareVertexBufferSharedPtr lightBuffer;

	{
		// Compute position and normals
		float* buffer = new float[(xsize+1)*(zsize+1)*6];
		pFloat = buffer;
		for (z = 0; z <= zsize; ++z)
		{
			for (x = 0; x <= xsize; ++x)
			{
				Ogre::Vector3 v;
				v = data->_getPosition(x+xbase, z+zbase);
				*pFloat++ = v.x; *pFloat++ = v.y; *pFloat++ = v.z;
				v = data->_getNormal(x+xbase, z+zbase);
				*pFloat++ = v.x; *pFloat++ = v.y; *pFloat++ = v.z;
			}
		}

		// Create buffer
		posNormBuffer = 
			Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
				6 * sizeof(float),
				vertexCount,
				Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

		// Fill the buffer
		pFloat = static_cast<float*>(posNormBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD));
		for (z = 0; z < zsize; ++z)
		{
			for (x = 0; x < xsize; ++x)
			{
				// NOTE: This algorithm is dependence with 'Corner', search [Corner Map] for detail
                memcpy(pFloat + 0*2*6, buffer + ((z+0) * (xsize+1) + x) * 6, 2*6 * sizeof(float));
                memcpy(pFloat + 1*2*6, buffer + ((z+1) * (xsize+1) + x) * 6, 2*6 * sizeof(float));
				pFloat += 2*2*6;
			}
		}
		posNormBuffer->unlock();

		delete [] buffer;
	}

	// For diffuse buffer
	// Create buffer
	{
		// Compute position and normals
		Ogre::uint32*  diffuse= new uint32[(xsize+1)*(zsize+1)];
		pDiffuse = diffuse;
		for (z = 0; z <= zsize; ++z)
		{
			for (x = 0; x <= xsize; ++x)
			{
				Ogre::RGBA *pCol = static_cast<Ogre::RGBA*>(static_cast<void*>(pDiffuse));
				Ogre::ColourValue col = data->getDiffuseInfo(x+xbase, z+zbase);
				Ogre::Root::getSingleton().convertColourValue(col, pCol++) ;
				pDiffuse =static_cast<uint32*>(static_cast<void*>(pCol));  
			}
		}

		// Create buffer
		diffuseBuffer = 
			Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
			sizeof(uint32),
			vertexCount,
			Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

		// Fill the buffer
		pDiffuse = static_cast<uint32*>(diffuseBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD));
		for (z = 0; z < zsize; ++z)
		{
			for (x = 0; x < xsize; ++x)
			{
				// NOTE: This algorithm is dependence with 'Corner', search [Corner Map] for detail
				memcpy(pDiffuse + 0, diffuse + ((z+0) * (xsize+1) + x), 2* sizeof(uint32));
				memcpy(pDiffuse + 2, diffuse + ((z+1) * (xsize+1) + x), 2* sizeof(uint32));
				pDiffuse += 4;
			}
		}
		diffuseBuffer->unlock();

		delete [] diffuse;
	}	
	
	//laim ´´½¨LIGHTBUFFER
	lightBuffer =Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
		sizeof(uint32),
		vertexCount,
		Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
	pDiffuse = static_cast<uint32*>(lightBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD));
	static int xxxxxx = 0x00000000;
	for (z = 0; z < zsize; ++z)
	{
		for (x = 0; x < xsize; ++x)
		{	
				memcpy(pDiffuse + 0, &xxxxxx,  sizeof(uint32));
				memcpy(pDiffuse + 1, &xxxxxx,  sizeof(uint32));
				memcpy(pDiffuse + 2, &xxxxxx,  sizeof(uint32));
				memcpy(pDiffuse + 3, &xxxxxx,  sizeof(uint32));
				pDiffuse += 4;
		}
	}
	lightBuffer->unlock();


	for (size_t layerIndex = 0; layerIndex < 2; ++layerIndex)
	{
		layerBuffer[layerIndex] =
			Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
				2 * sizeof(float),
				vertexCount,
				Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
	}

    bool includeLightmap = mOwner->_isLightmapUsed();

	if (includeLightmap)
	{
        Ogre::Real xscale = 1.0 / xsize;
        Ogre::Real zscale = 1.0 / zsize;
		lightmapBuffer =
			Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
				2 * sizeof(float),
				vertexCount,
				Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
		pFloat = static_cast<float*>(lightmapBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD));
		for (z = 0; z < zsize; ++z)
		{
			for (x = 0; x < xsize; ++x)
			{
                *pFloat++ = xscale * (x+0); *pFloat++ = zscale * (z+0);
                *pFloat++ = xscale * (x+1); *pFloat++ = zscale * (z+0);
                *pFloat++ = xscale * (x+0); *pFloat++ = zscale * (z+1);
                *pFloat++ = xscale * (x+1); *pFloat++ = zscale * (z+1);
			}
		}
		lightmapBuffer->unlock();
	}

	// Create vertex data
    for (size_t layerIndex = 0; layerIndex < 2; ++layerIndex)
	{
		Ogre::VertexData* vertexData = new Ogre::VertexData;
		mVertexDatas.push_back(vertexData);
        vertexData->vertexStart = 0;
        vertexData->vertexCount = vertexCount;

        Ogre::VertexDeclaration* decl = vertexData->vertexDeclaration;
        Ogre::VertexBufferBinding* bind = vertexData->vertexBufferBinding;

        size_t offset;
        size_t texCoordSet = 0;
		int source = 0;

        // position and normal
		offset = 0;
        decl->addElement(source, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
        offset += 3 * sizeof(float);
        decl->addElement(source, offset, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
        offset += 3 * sizeof(float);
		bind->setBinding(source++, posNormBuffer);

		offset = 0;
		decl->addElement(source, offset, Ogre::VET_COLOUR, Ogre::VES_DIFFUSE);
		offset += Ogre::VertexElement::getTypeSize(Ogre::VET_COLOUR);
		bind->setBinding(source++, diffuseBuffer);



        // texture layer 0
		offset = 0;
        decl->addElement(source, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, texCoordSet++);
        offset += 2 * sizeof(float);
		bind->setBinding(source++, layerBuffer[0]);

        // texture layer 1
        if (layerIndex == 1)
        {
			offset = 0;
            decl->addElement(source, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, texCoordSet++);
            offset += 2 * sizeof(float);
			bind->setBinding(source++, layerBuffer[1]);
        }

        // light-map layer
		if (includeLightmap)
        {
			offset = 0;
            decl->addElement(source, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, texCoordSet++);
            offset += 2 * sizeof(float);
			bind->setBinding(source++, lightmapBuffer);
        }

		//lightbuffer  laim
		offset = 0;
		decl->addElement(source,offset,Ogre::VET_COLOUR,Ogre::VES_SPECULAR);
		bind->setBinding(source++,lightBuffer);
	}
}

void
TerrainTileEditable::createGeometry(TerrainData* data, int xbase, int zbase, int xsize, int zsize)
{
    destoryGeometry();

    // build the material backet map
    MaterialBucketMap materialBucketMap;
    buildMaterialBucketMap(materialBucketMap);

	// Build renderables
    for (MaterialBucketMap::const_iterator im = materialBucketMap.begin(); im != materialBucketMap.end(); ++im)
	{
        const MaterialBucket* mb = &im->second;

        TerrainTileEditableRenderable* renderable = new TerrainTileEditableRenderable(this, mb->layerIndex);
        mRenderables.push_back(renderable);

        size_t numQuads = mb->grids.size();
        size_t indexCount = numQuads * 6;

		renderable->mMaterial = mb->material;

		Ogre::HardwareIndexBufferSharedPtr indexBuffer =
			Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(
				Ogre::HardwareIndexBuffer::IT_16BIT,
				indexCount,
				Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

		Ogre::IndexData* indexData = new Ogre::IndexData;
		indexData->indexBuffer = indexBuffer;
		indexData->indexStart = 0;
		indexData->indexCount = indexCount;

        renderable->mRenderOp.indexData = indexData;
        renderable->mRenderOp.vertexData = mVertexDatas[mb->layerIndex];
        renderable->mRenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
        renderable->mRenderOp.useIndexes = true;

		ushort* pIndices = static_cast<ushort*>(indexBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD));
        for (GridIdList::const_iterator igrid = mb->grids.begin(); igrid != mb->grids.end(); ++igrid)
		{
            size_t grid = *igrid;
            const TerrainData::GridInfo& gridInfo = data->mGridInfos[grid];
            const TerrainData::Corner* corners = gridInfo.getCorners();
            int x = grid % data->mXSize - xbase;
            int z = grid / data->mXSize - zbase;
            assert(x < xsize && z < zsize);
            size_t index = (z*xsize + x) * 4;

            *pIndices++ = index + corners[1];
            *pIndices++ = index + corners[0];
            *pIndices++ = index + corners[2];

            *pIndices++ = index + corners[1];
            *pIndices++ = index + corners[2];
            *pIndices++ = index + corners[3];
		}
		indexBuffer->unlock();
	}

    for (size_t layerIndex = 0; layerIndex < 2; ++layerIndex)
    {
        fillLayerBuffer(layerIndex, mVertexDatas.back()->vertexBufferBinding->getBuffer(layerIndex+2));
    }

    mGeometryOutOfDate = false;
}

void
TerrainTileEditable::_notifyHeightModified(int xstart, int zstart, int xend, int zend)
{
	assert(0 <= xstart && xstart < xend && xend <= mXSize+1);
	assert(0 <= zstart && zstart < zend && zend <= mZSize+1);

    if (mVertexDatas.empty())
    {
        // Nothing todo when buffer never initialised
        return;
    }

	// TODO: (optimization) some grid may be need to recompute normal only

	TerrainData* data = mOwner->getData();
	Ogre::HardwareVertexBufferSharedPtr posNormBuffer = mVertexDatas.front()->vertexBufferBinding->getBuffer(0);
	float* pPosNormBuffer = static_cast<float*>(posNormBuffer->lock(Ogre::HardwareBuffer::HBL_NORMAL));
    float buffer[6];
	int xsize = mXSize;
    int zsize = mZSize;
	int xbase = mXBase;
	int zbase = mZBase;
	for (int z = zstart; z < zend; ++z)
	{
		for (int x = xstart; x < xend; ++x)
		{
			Ogre::Vector3 v;
            v = data->_getPosition(x+xbase, z+zbase);
            buffer[0] = v.x; buffer[1] = v.y; buffer[2] = v.z;
			v = data->_getNormal(x+xbase, z+zbase);
            buffer[3] = v.x; buffer[4] = v.y; buffer[5] = v.z;
            if (0 < z)
            {
                if (0 < x)
                    memcpy(pPosNormBuffer + (((z-1) * xsize + (x-1)) * 4 + 3) * 6, buffer, 6*sizeof(float));
                if (x < xsize)
                    memcpy(pPosNormBuffer + (((z-1) * xsize + (x-0)) * 4 + 2) * 6, buffer, 6*sizeof(float));
            }
            if (z < zsize)
            {
                if (0 < x)
                    memcpy(pPosNormBuffer + (((z-0) * xsize + (x-1)) * 4 + 1) * 6, buffer, 6*sizeof(float));
                if (x < xsize)
                    memcpy(pPosNormBuffer + (((z-0) * xsize + (x-0)) * 4 + 0) * 6, buffer, 6*sizeof(float));
            }
		}
	}
	posNormBuffer->unlock();

	// re-compue bounding box
	data->_computeTileBoundingBox(mBounds, xbase, zbase, xsize, zsize);

    // trigger update of bounding box
    getParentNode()->needUpdate();
}


void
TerrainTileEditable::_notifyLightColourModified(int xstart, int zstart, int xend, int zend)
{
	assert(0 <= xstart && xstart < xend && xend <= mXSize+1);
	assert(0 <= zstart && zstart < zend && zend <= mZSize+1);

	if (mVertexDatas.empty())
	{
		// Nothing todo when buffer never initialised
		return;
	}


	TerrainData* data = mOwner->getData();
	Ogre::HardwareVertexBufferSharedPtr diffuseBuffer = mVertexDatas.front()->vertexBufferBinding->getBuffer(mOwner->_isLightmapUsed()?4:3);
	uint32* pDiffuseBuffer = static_cast<uint32*>(diffuseBuffer->lock(Ogre::HardwareBuffer::HBL_NORMAL));
	uint32 diffuse[1];
	int xsize = mXSize;
	int zsize = mZSize;
	int xbase = mXBase;
	int zbase = mZBase;
	for (int z = zstart; z < zend; ++z)
	{
		for (int x = xstart; x < xend; ++x)
		{
			//uint32* pTempCol = diffuse;			
			Ogre::RGBA *pCol = static_cast<Ogre::RGBA*>(static_cast<void*>(diffuse));
			Ogre::ColourValue col = data->getLightDiffuseInfo(x+xbase, z+zbase);
			Ogre::Root::getSingleton().convertColourValue(col, pCol) ;

			if (0 < z)
			{
				if (0 < x)
					memcpy(pDiffuseBuffer + (((z-1) * xsize + (x-1)) * 4 + 3), diffuse, sizeof(uint32));
				if (x < xsize)
					memcpy(pDiffuseBuffer + (((z-1) * xsize + (x-0)) * 4 + 2), diffuse, sizeof(uint32));
			}
			if (z < zsize)
			{
				if (0 < x)
					memcpy(pDiffuseBuffer + (((z-0) * xsize + (x-1)) * 4 + 1), diffuse, sizeof(uint32));
				if (x < xsize)
					memcpy(pDiffuseBuffer + (((z-0) * xsize + (x-0)) * 4 + 0), diffuse, sizeof(uint32));
			}
		}
	}
	diffuseBuffer->unlock();


}

void
TerrainTileEditable::_notifyColourModified(int xstart, int zstart, int xend, int zend)
{
	assert(0 <= xstart && xstart < xend && xend <= mXSize+1);
	assert(0 <= zstart && zstart < zend && zend <= mZSize+1);

	if (mVertexDatas.empty())
	{
		// Nothing todo when buffer never initialised
		return;
	}

	TerrainData* data = mOwner->getData();
	Ogre::HardwareVertexBufferSharedPtr diffuseBuffer = mVertexDatas.front()->vertexBufferBinding->getBuffer(1);
	uint32* pDiffuseBuffer = static_cast<uint32*>(diffuseBuffer->lock(Ogre::HardwareBuffer::HBL_NORMAL));
	uint32 diffuse[1];
	int xsize = mXSize;
	int zsize = mZSize;
	int xbase = mXBase;
	int zbase = mZBase;
	for (int z = zstart; z < zend; ++z)
	{
		for (int x = xstart; x < xend; ++x)
		{
			//uint32* pTempCol = diffuse;			
			Ogre::RGBA *pCol = static_cast<Ogre::RGBA*>(static_cast<void*>(diffuse));
			Ogre::ColourValue col = data->getDiffuseInfo(x+xbase, z+zbase);
			Ogre::Root::getSingleton().convertColourValue(col, pCol) ;

			if (0 < z)
			{
				if (0 < x)
					memcpy(pDiffuseBuffer + (((z-1) * xsize + (x-1)) * 4 + 3), diffuse, sizeof(uint32));
				if (x < xsize)
					memcpy(pDiffuseBuffer + (((z-1) * xsize + (x-0)) * 4 + 2), diffuse, sizeof(uint32));
			}
			if (z < zsize)
			{
				if (0 < x)
					memcpy(pDiffuseBuffer + (((z-0) * xsize + (x-1)) * 4 + 1), diffuse, sizeof(uint32));
				if (x < xsize)
					memcpy(pDiffuseBuffer + (((z-0) * xsize + (x-0)) * 4 + 0), diffuse, sizeof(uint32));
			}
		}
	}
	diffuseBuffer->unlock();
}

void
TerrainTileEditable::_notifyGridInfoModified(int xstart, int zstart, int xend, int zend)
{
	assert(0 <= xstart && xstart < xend && xend <= mXSize);
	assert(0 <= zstart && zstart < zend && zend <= mZSize);

	// TODO: Optimize  3

    mGeometryOutOfDate = true;
}

}
