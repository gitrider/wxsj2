/********************************************************************
filename:   OgreRibbonSet.cpp

purpose:    ��������ڲ���һ�������ribbon chain
*********************************************************************/
#include "OgreRibbonSet.h"

#include <OgreSceneNode.h>

namespace Ogre
{
    RibbonSet::RibbonSet(const Ogre::String& name, size_t maxElements, 
        size_t numberOfChains, bool useTextureCoords, bool useColours)
        : BillboardChain(name, maxElements, 0, useTextureCoords, useColours, true)
        , mSquaredElemLength(0.0f)
        , mDefaultElemWidth(0.0f)
        , mDefaultElemLength(0.0f)
        , mAllDefaultSize(true)
        , mRibbonHeadAlpha(1.0f)
        , mRibbonTailAlpha(1.0f)
        , mNeedChangeColour(false)
        , mRibbonHeadWidthScale(1.0f)
        , mRibbonTailWidthScale(1.0f)
        , mNeedChangeWidth(false)
    {
        // use U as varying texture coord, so we can use 1D textures to 'smear'
        setTextureCoordDirection(TCD_U);
    }
    //-----------------------------------------------------------------------
    void RibbonSet::_notifyResized(void)
    {
        mAllDefaultSize = false;
    }
    //-----------------------------------------------------------------------
    void RibbonSet::setNumberOfChains(size_t numChains)
    {
        BillboardChain::setNumberOfChains(numChains);
    }
    //-----------------------------------------------------------------------
    void RibbonSet::updateRibbon(size_t index, const Vector3& pos, bool worldSpace, const ColourValue& colour, Ogre::Real elemWidth)
    {
        Ogre::Real realWidth;

        // �����ǰ�õ�������ϵͳ�����������Ӵ�С
        if (mAllDefaultSize)
            realWidth = mDefaultElemWidth;
        // �����ǰ�õ�������ϵͳӰ���������������Ӵ�С
        else
            realWidth = elemWidth;

        Vector3 newPos = pos;
        if (mParentNode && worldSpace)
        {
            // Transform position to ourself space
            newPos = mParentNode->_getDerivedOrientation().UnitInverse() *
                (newPos - mParentNode->_getDerivedPosition()) / mParentNode->_getDerivedScale();
        }

        // Node has changed somehow, we're only interested in the derived position
        ChainSegment& seg = mChainSegmentList[index];

        if (seg.head == SEGMENT_EMPTY)
        {
            Element e(newPos, realWidth, 0, colour);
            addChainElement(index, e);
            addChainElement(index, e);
        }

        Element& headElem = mChainElementList[seg.start + seg.head];
        size_t nextElemIdx = seg.head + 1;
        // wrap
        if (nextElemIdx == mMaxElementsPerChain)
            nextElemIdx = 0;
        Element& nextElem = mChainElementList[seg.start + nextElemIdx];

        // Vary the head elem, but bake new version if that exceeds element len
        Vector3 diff = newPos - nextElem.position;
        Ogre::Real sqlen = diff.squaredLength();
        if (sqlen >= mSquaredElemLength)
        {
            // Move existing head to mDefaultElemLength
            Vector3 scaledDiff = diff * (mDefaultElemLength / Math::Sqrt(sqlen));
            headElem.position = nextElem.position + scaledDiff;
            // Add a new element to be the new head
            Element newElem(newPos, realWidth, 0.0f, colour);
            addChainElement(index, newElem);
            // alter diff to represent new head size
            diff = newPos - newElem.position;

        }
        else
        {
            // Extend existing head
            headElem.position = newPos;
        }

        // Is this segment full?
        if ((seg.tail + 1) % mMaxElementsPerChain == seg.head)
        {
            // If so, shrink tail gradually to match head extension
            Element& tailElem = mChainElementList[seg.start + seg.tail];
            size_t preTailIdx;
            if (seg.tail == 0)
                preTailIdx = mMaxElementsPerChain - 1;
            else
                preTailIdx = seg.tail - 1;
            Element& preTailElem = mChainElementList[seg.start + preTailIdx];

            // Measure tail diff from pretail to tail
            Vector3 taildiff = tailElem.position - preTailElem.position;
            Ogre::Real taillen = taildiff.length();
            if (taillen > 1e-06)
            {
                Ogre::Real tailsize = mDefaultElemLength - diff.length();
                taildiff *= tailsize / taillen;
                tailElem.position = preTailElem.position + taildiff;
            }

        }

        // �������ǰchain�е�ʵ��element����
        size_t elementCount;

        // ����addElement�е�ʵ�֣����head����tail��˵��chain�Ѿ��������ˣ����headС��tail��˵��chainδ��
        if (seg.head > seg.tail)
            elementCount = getMaxChainElements();
        else if (seg.head < seg.tail)
            elementCount = seg.tail - seg.head + 1;

        assert (seg.head != seg.tail);

        // ȡ�����chain�е�һ�������һ��Ԫ��
        const Element& firstElem = mChainElementList[seg.start + seg.tail];
        const Element& lastElem = mChainElementList[seg.start + seg.head];

        // ��������֮���ƽ������
        Ogre::Real totalLength = (lastElem.position - firstElem.position).squaredLength();

        // ����������0���Ž��м��㣨��ÿ��particle��һ�ν����������ʱ�����ǵľ�����0��
        if (false == Math::RealEqual(0.0f, totalLength))
        {
            // ������ǰchain�е�����ʵ��Ԫ��
            for (size_t i=0; i<elementCount; ++i)
            {
                Element e = getChainElement(index, i);

                // ���㵱ǰԪ�����һ��Ԫ��֮���ƽ�����룬������Ԫ����chain�е�ʵ��λ�ã��������������ƽ��������̣��ٿ���
                // ��ֱ���������ʵ��length�������ܽ�ʡһ�ο�������
                Ogre::Real ratio = 1.0f - Math::Sqrt( (e.position - firstElem.position).squaredLength() / totalLength );
                e.texCoord = ratio;

                if (mNeedChangeColour)
                    e.colour = colour * (mRibbonHeadAlpha + (mRibbonTailAlpha - mRibbonHeadAlpha) * ratio);

                if (mNeedChangeWidth)
                    e.width = realWidth * (mRibbonHeadWidthScale + (mRibbonTailWidthScale - mRibbonHeadWidthScale) * ratio);

                updateChainElement(index,i,e);
            }
        }

        mBoundsDirty = true;

        if (mParentNode)
        {
            mParentNode->needUpdate();
        }
    }
    //-----------------------------------------------------------------------
    void RibbonSet::resetRibbon(size_t index)
    {
        clearChain(index);

        // we removed an entry so indexes need updating
        mIndexContentDirty = true;
        mBoundsDirty = true;
        // tell parent node to update bounds
        if (mParentNode)
            mParentNode->needUpdate();
    }
    //-----------------------------------------------------------------------
    void RibbonSet::setHeadAlpha(Ogre::Real alpha)
    {
        mRibbonHeadAlpha = alpha;

        // �����һ����ֵ��Ϊ1������Ҫ�ı���ɫ
        if ( false == Math::RealEqual(1.0f, mRibbonHeadAlpha) || 
            false == Math::RealEqual(1.0f, mRibbonTailAlpha) )
            mNeedChangeColour = true;            
    }
    //-----------------------------------------------------------------------
    void RibbonSet::setTailAlpha(Ogre::Real alpha)
    {
        mRibbonTailAlpha = alpha;

        // �����һ����ֵ��Ϊ1������Ҫ�ı���ɫ
        if ( false == Math::RealEqual(1.0f, mRibbonHeadAlpha) || 
            false == Math::RealEqual(1.0f, mRibbonTailAlpha) )
            mNeedChangeColour = true;
    }
    //-----------------------------------------------------------------------
    void RibbonSet::setHeadWidthScale(Ogre::Real scale)
    {
        mRibbonHeadWidthScale = scale;

        // �����һ����ֵ��Ϊ1������Ҫ�ı���ɫ
        if ( false == Math::RealEqual(1.0f, mRibbonHeadWidthScale) || 
            false == Math::RealEqual(1.0f, mRibbonTailWidthScale) )
            mNeedChangeWidth = true;
    }
    //-----------------------------------------------------------------------
    void RibbonSet::setTailWidthScale(Ogre::Real scale)
    {
        mRibbonTailWidthScale = scale;

        // �����һ����ֵ��Ϊ1������Ҫ�ı���ɫ
        if ( false == Math::RealEqual(1.0f, mRibbonHeadWidthScale) || 
            false == Math::RealEqual(1.0f, mRibbonTailWidthScale) )
            mNeedChangeWidth = true;
    }
    //-----------------------------------------------------------------------
    void RibbonSet::setDefaultSize(Ogre::Real width, Ogre::Real length)
    {
        mDefaultElemWidth = width;
        mDefaultElemLength = length;

        mSquaredElemLength = mDefaultElemLength * mDefaultElemLength;
    }
    //-----------------------------------------------------------------------
    void RibbonSet::setMaxChainElements(size_t maxElements)
    {
        BillboardChain::setMaxChainElements(maxElements);

        mBuffersNeedRecreating = mIndexContentDirty = true;
    }
}