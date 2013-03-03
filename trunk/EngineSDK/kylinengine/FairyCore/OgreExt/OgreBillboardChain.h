/**
	����ļ�����ogre���ĵ�EffectBillboardChain��Ļ����Ͻ����޸ĵģ����ʺ�Ӧ����Ҫ��
	Ϊ�����ͻ��������һ������
*/

#ifndef _EffectBillboardChain_H__
#define _EffectBillboardChain_H__

#include "OgrePrerequisites.h"

#include "OgreSimpleRenderable.h"

namespace Ogre {


   /** Contains the data of an element of the EffectBillboardChain.
   */
   class EffectBillboardChainElement
   {

   public:

      EffectBillboardChainElement();

      EffectBillboardChainElement(Vector3 position,
                       Ogre::Real width,
                       Ogre::Real uTexCoord,
                       ColourValue colour);

      Vector3 position;
      Ogre::Real width;
      Ogre::Real uTexCoord;
      ColourValue colour;

   };

    /** Allows the rendering of a chain of connected billboards.
    */
   class EffectBillboardChain : public SimpleRenderable
   {

   public:
         
      typedef std::vector<EffectBillboardChainElement> EffectBillboardChainElementList;

      EffectBillboardChain(int maxNbChainElements = 10);
      virtual ~EffectBillboardChain();

      virtual void _notifyCurrentCamera(Camera* cam);
      virtual Ogre::Real getSquaredViewDepth(const Camera* cam) const;
      virtual Ogre::Real getBoundingRadius(void) const;

      void setNbChainElements(unsigned int nbChainElements);
      void setChainElement(unsigned int elementIndex, const EffectBillboardChainElement& billboardChainElement);
      void updateBoundingBox();

      void _updateRenderQueue(RenderQueue* queue);

      void isInitData(bool init)
      {
          mInitData = init;
      }

   protected:

	   void _createBuffer(void);

   protected:

      Ogre::Real mRadius;

      int mCurrentNbChainElements;

      EffectBillboardChainElementList mChainElementList;

      // ���chain�������Ƿ��Ѿ�׼������ȥ��Ⱦ��
      bool mInitData;

      void updateHardwareBuffers();
   };

} // namespace

#endif

