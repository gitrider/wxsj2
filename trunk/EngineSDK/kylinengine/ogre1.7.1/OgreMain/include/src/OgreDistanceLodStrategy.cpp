/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2009 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/

#include "OgreStableHeaders.h"
#include "OgreDistanceLodStrategy.h"

#include <limits>

#include "OgreViewport.h"

namespace Ogre {
    //-----------------------------------------------------------------------
    template<> DistanceLodStrategy* Singleton<DistanceLodStrategy>::ms_Singleton = 0;
    DistanceLodStrategy* DistanceLodStrategy::getSingletonPtr(void)
    {
        return ms_Singleton;
    }
    DistanceLodStrategy& DistanceLodStrategy::getSingleton(void)
    {
        assert( ms_Singleton );  return ( *ms_Singleton );
    }
    //-----------------------------------------------------------------------
    DistanceLodStrategy::DistanceLodStrategy()
        : LodStrategy("Distance")
        , mReferenceViewEnabled(false)
        , mReferenceViewValue(-1)
    { }
    //-----------------------------------------------------------------------
    Real DistanceLodStrategy::getValueImpl(const MovableObject *movableObject, const Ogre::Camera *camera) const
    {
        // Get squared depth taking into account bounding radius
        // (d - r) ^ 2 = d^2 - 2dr + r^2, but this requires a lot 
        // more computation (including a sqrt) so we approximate 
        // it with d^2 - r^2, which is good enough for determining 
        // lod.
        Real squaredDepth = movableObject->getParentNode()->getSquaredViewDepth(camera) - Math::Sqr(movableObject->getBoundingRadius());

        // Check if reference view needs to be taken into account
        if (mReferenceViewEnabled)
        {
            // Reference view only applicable to perspective projection
            assert(camera->getProjectionType() == PT_PERSPECTIVE && "Camera projection type must be perspective!");

            // Get camera viewport
            Viewport *viewport = camera->getViewport();

            // Get viewport area
            Real viewportArea = static_cast<Real>(viewport->getActualWidth() * viewport->getActualHeight());

            // Get projection matrix (this is done to avoid computation of tan(fov / 2))
            const Matrix4& projectionMatrix = camera->getProjectionMatrix();

            // Compute bias value (note that this is similar to the method used for PixelCountLodStrategy)
            Real biasValue = viewportArea * projectionMatrix[0][0] * projectionMatrix[1][1];

            // Scale squared depth appropriately
            squaredDepth *= (mReferenceViewValue / biasValue);
        }

        // Squared depth should never be below 0, so clamp
        squaredDepth = std::max(squaredDepth, Real(0));

        // Now adjust it by the camera bias and return the computed value
        return squaredDepth * camera->_getLodBiasInverse();
    }
    //-----------------------------------------------------------------------
    Real DistanceLodStrategy::getBaseValue() const
    {
        return Real(0);
    }
    //---------------------------------------------------------------------
    Real DistanceLodStrategy::transformBias(Real factor) const
    {
        assert(factor > 0.0f && "Bias factor must be > 0!");
        return 1.0f / factor;
    }
    //-----------------------------------------------------------------------
    Real DistanceLodStrategy::transformUserValue(Real userValue) const
    {
        // Square user-supplied distance
        return Math::Sqr(userValue);
    }
    //-----------------------------------------------------------------------
    ushort DistanceLodStrategy::getIndex(Real value, const Mesh::MeshLodUsageList& meshLodUsageList) const
    {
        // Get index assuming ascending values
        return getIndexAscending(value, meshLodUsageList);
    }
    //-----------------------------------------------------------------------
    ushort DistanceLodStrategy::getIndex(Real value, const Material::LodValueList& materialLodValueList) const
    {
        // Get index assuming ascending values
        return getIndexAscending(value, materialLodValueList);
    }
    //---------------------------------------------------------------------
    bool DistanceLodStrategy::isSorted(const Mesh::LodValueList& values) const
    {
        // Determine if sorted ascending
        return isSortedAscending(values);
    }
        //---------------------------------------------------------------------
    void DistanceLodStrategy::sort(Mesh::MeshLodUsageList& meshLodUsageList) const
    {
        // Sort ascending
        return sortAscending(meshLodUsageList);
    }
    //---------------------------------------------------------------------
    void DistanceLodStrategy::setReferenceView(Real viewportWidth, Real viewportHeight, Radian fovY)
    {
        // Determine x FOV based on aspect ratio
        Radian fovX = fovY * (viewportWidth / viewportHeight);

        // Determine viewport area
        Real viewportArea = viewportHeight * viewportWidth;

        // Compute reference view value based on viewport area and FOVs
        mReferenceViewValue = viewportArea * Math::Tan(fovX * 0.5f) * Math::Tan(fovY * 0.5f);

        // Enable use of reference view
        mReferenceViewEnabled = true;
    }
    //---------------------------------------------------------------------
    void DistanceLodStrategy::setReferenceViewEnabled(bool value)
    {
        // Ensure reference value has been set before being enabled
        if (value)
            assert(mReferenceViewValue != -1 && "Reference view must be set before being enabled!");

        mReferenceViewEnabled = value;
    }
    //---------------------------------------------------------------------
    bool DistanceLodStrategy::getReferenceViewEnabled() const
    {
        return mReferenceViewEnabled;
    }

} // namespace
