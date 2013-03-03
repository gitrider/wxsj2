/********************************************************************
filename:   OgreRibbonRenderer.h

purpose:    ribbon particle renderer
*********************************************************************/
#ifndef __RibbonRenderer_H__
#define __RibbonRenderer_H__

#include <OgreParticleSystemRenderer.h>

namespace Ogre
{
    class RibbonSet;

    class RibbonRenderer : public ParticleSystemRenderer
    {
    public:

        class _OgrePrivate CmdRibbonHeadAlpha : public ParamCommand
        {
        public:
           Ogre::String doGet(const void* target) const;
            void doSet(void* target, const Ogre::String& val);
        };

        class _OgrePrivate CmdRibbonTailAlpha : public ParamCommand
        {
        public:
           Ogre::String doGet(const void* target) const;
            void doSet(void* target, const Ogre::String& val);
        };

        class _OgrePrivate CmdRibbonHeadWidthScale : public ParamCommand
        {
        public:
           Ogre::String doGet(const void* target) const;
            void doSet(void* target, const Ogre::String& val);
        };

        class _OgrePrivate CmdRibbonTailWidthScale : public ParamCommand
        {
        public:
           Ogre::String doGet(const void* target) const;
            void doSet(void* target, const Ogre::String& val);
        };

        class _OgrePrivate CmdRibbonElementCount : public ParamCommand
        {
        public:
           Ogre::String doGet(const void* target) const;
            void doSet(void* target, const Ogre::String& val);
        };

        RibbonRenderer();
        ~RibbonRenderer();

        void setRibbonHeadAlpha(Ogre::Real alpha);
        Ogre::Real getRibbonHeadAlpha(void) const;

        void setRibbonTailAlpha(Ogre::Real alpha);
        Ogre::Real getRibbonTailAlpha(void) const;

        void setRibbonHeadWidthScale(Ogre::Real width);
        Ogre::Real getRibbonHeadWidthScale(void) const;

        void setRibbonTailWidthScale(Ogre::Real width);
        Ogre::Real getRibbonTailWidthScale(void) const;

        void setRibbonElementCount(size_t count);
        size_t getRibbonElementCount(void) const;

        /// @copydoc ParticleSystemRenderer::getType
        virtual const Ogre::String& getType(void) const;
        /// @copydoc ParticleSystemRenderer::_updateRenderQueue
        virtual void _updateRenderQueue(RenderQueue* queue, 
			std::list<Particle*>& currentParticles, bool cullIndividually);

		void visitRenderables(Renderable::Visitor* visitor, 
			bool debugRenderables = false);
        /// @copydoc ParticleSystemRenderer::_setMaterial
        virtual void _setMaterial(MaterialPtr& mat);
        /// @copydoc ParticleSystemRenderer::_notifyCurrentCamera
        virtual void _notifyCurrentCamera(Camera* cam);
        /// @copydoc ParticleSystemRenderer::_notifyParticleRotated
        virtual void _notifyParticleRotated(void);
        /// @copydoc ParticleSystemRenderer::_notifyParticleResized
        virtual void _notifyParticleResized(void);
        /// @copydoc ParticleSystemRenderer::_notifyParticleQuota
        virtual void _notifyParticleQuota(size_t quota);
        /// @copydoc ParticleSystemRenderer::_notifyAttached
        virtual void _notifyAttached(Node* parent, bool isTagPoint = false);
        /// @copydoc ParticleSystemRenderer::_notifyDefaultDimensions
        virtual void _notifyDefaultDimensions(Ogre::Real width, Ogre::Real height);
        /// @copydoc ParticleSystemRenderer::setRenderQueueGroup
        virtual void setRenderQueueGroup(uint8 queueID);
        /// @copydoc ParticleSystemRenderer::setKeepParticlesInLocalSpace
        virtual void setKeepParticlesInLocalSpace(bool keepLocal);
        /// @copydoc ParticleSystemRenderer::_getSortMode
        virtual Ogre::SortMode _getSortMode(void) const;

        /// @copydoc ParticleSystemRenderer::_createVisualData
        virtual ParticleVisualData* _createVisualData(void);
        /// @copydoc ParticleSystemRenderer::_destroyVisualData
        virtual void _destroyVisualData(ParticleVisualData* vis);
        /** Optional callback notified when particle expired */
        virtual void _notifyParticleExpired(Particle* particle);
        /** Optional callback notified when particles moved */
        virtual void _notifyParticleMoved(std::list<Particle*>& currentParticles);

		

    protected:

        RibbonSet* mRibbonSet;

        /// is the ribbon in world space
        bool mWorldSpace;

        /// ���ڱ�ʶ��ǰ�Ŀ���ribbon�±�
        size_t mNextRibbonIndex;

        static CmdRibbonHeadAlpha msRibbonHeadAlphaCmd;
        static CmdRibbonTailAlpha msRibbonTailAlphaCmd;
        static CmdRibbonHeadWidthScale msRibbonHeadWidthScaleCmd;
        static CmdRibbonTailWidthScale msRibbonTailWidthScaleCmd;
        static CmdRibbonElementCount msRibbonElementCountCmd;
    };

    /** Factory class for MeshParticleRenderer */
    class RibbonRendererFactory : public ParticleSystemRendererFactory
    {
    public:
        /// @copydoc FactoryObj::getType
        virtual const Ogre::String& getType() const;
        /// @copydoc FactoryObj::createInstance
        virtual ParticleSystemRenderer* createInstance( const Ogre::String& name );    
        /// @copydoc FactoryObj::destroyInstance
        virtual void destroyInstance( ParticleSystemRenderer* inst);    
    };
}

#endif // __RibbonRenderer_H__