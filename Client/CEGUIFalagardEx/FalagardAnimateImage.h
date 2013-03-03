#pragma once
#include "falmodule.h"
#include "ceguiwindowfactory.h"
#include "falbutton.h"
#include "FalAnimateProperties.h"
#include "elements\ceguistaticimage.h"
namespace CEGUI
{
	class Animate;
	namespace FalagardAnimateImageProperties
	{
		class Animate: public Property
		{
		public:
			Animate() : Property(
				"Animate",
				"Property to get/set empty status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};

		class AnimateID: public Property
		{
		public:
			AnimateID() : Property(
				"AnimateID",
				"Property to get/set empty status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};

		class UseImageSize: public Property
		{
		public:
			UseImageSize() : Property(
				"UseImageSize",
				"Property to get/set empty status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};

	};
	class FALAGARDBASE_API FalagardAnimateImage :	public StaticImage
	{
	public:
		static const utf8   WidgetTypeName[];       //!< type name for this widget.
     
		virtual void	updateSelf(float elapsed);
		virtual void	drawSelf(float z);
	protected:
		int				m_nAnimateStart;
		Animate			*m_pAnimate;
		void			drawNormal(float z);

		float			m_fAlpha;
		bool			m_bPlay;
		bool			m_bUseImageSize;
	public:

		static FalagardAnimateImageProperties::Animate			d_animateProperty;
		static FalagardAnimateImageProperties::AnimateID		d_animateIDProperty;
		static FalagardAnimateImageProperties::UseImageSize		d_useImageSizeProperty;

		virtual void Play( bool bPlay );

		void SetAnimate( const String& strAnimate );
		void SetAnimateThroughId(int id);
		void SetUseImageSize( bool bIsUseImageSize ) { m_bUseImageSize = bIsUseImageSize; };
		FalagardAnimateImage(const String& type, const String& name);
		~FalagardAnimateImage(void);
	};

	class FALAGARDBASE_API FalagardAnimateImageFactory : public WindowFactory
    {
    public:
        FalagardAnimateImageFactory(void) : WindowFactory(FalagardAnimateImage::WidgetTypeName) { }
        ~FalagardAnimateImageFactory(void){}
        Window* createWindow(const String& name);
        void destroyWindow(Window* window);
    };
}