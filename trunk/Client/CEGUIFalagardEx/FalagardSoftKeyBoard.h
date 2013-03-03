#pragma once
#include "falstaticimage.h"
#include "ifalagardsoftkeyboard.h"
#include <vector>
namespace CEGUI
{
	//const int SOFTKEY_NUMBER = 
	enum SoftKey_Type
	{
		SoftKey_Number = 0, // ����
		SoftKey_Word,		// ��ĸ
		SoftKey_Punctuation,// ���
		SoftKey_Type_Number,
		SoftKey_Back,		// �˸�
		SoftKey_Shift,		// �л�
		SoftKey_Return,		// �س�
		SoftKey_Space,		// �ո�
		SoftKey_TypeNumber,
	};

	enum SoftKey_Define
	{
		SoftKeyDef_Number = 0,			// ����
		SoftKeyDef_Lower_Word,			// ��д��ĸ
		SoftKeyDef_Capital_Word,		// Сд��ĸ
		SoftKeyDef_Punctuation,			// ���
		SoftKeyDef_MaxNumber,
	};

	namespace FalagardSoftKeyProperties
	{
		class KeySize: public Property
		{
		public:
			KeySize() : Property(
				"KeySize",
				"Property to get/set the horizontal formatting mode.  Value is one of the HorzFormatting strings.",
				"")
			{}

			String	get(const PropertyReceiver* receiver) const;
			void	set(PropertyReceiver* receiver, const String& value);
		};

		class HoverImage: public Property
		{
		public:
			HoverImage() : Property(
				"HoverImage",
				"Property to get/set the horizontal formatting mode.  Value is one of the HorzFormatting strings.",
				"")
			{}

			String	get(const PropertyReceiver* receiver) const;
			void	set(PropertyReceiver* receiver, const String& value);
		};
		
		class PushedImage: public Property
		{
		public:
			PushedImage() : Property(
				"PushedImage",
				"Property to get/set the horizontal formatting mode.  Value is one of the HorzFormatting strings.",
				"")
			{}

			String	get(const PropertyReceiver* receiver) const;
			void	set(PropertyReceiver* receiver, const String& value);
		};
	};
	struct SKey_Define
	{
		utf32    uChar; // �ַ�
		int     nType;
	};
	struct SKey_Position
	{
		Point	m_Pos;
		bool	m_bHover; // �Ƿ�ѡ����
		SKey_Define* m_pKeyDefine; // ���������ַ�
		void setHover( bool bHover ){ m_bHover = bHover; };
		SKey_Position()
		{
			m_bHover = false;
		};
	};
	enum BUTTON_TYPE
	{
		BUTTON_TYPE_NORMAL = 0,
		BUTTON_TYPE_BACK,
		BUTTON_TYPE_SHIFT,
		BUTTON_TYPE_RETURN,
		BUTTON_TYPE_NUMBER,

	};
	class FALAGARDBASE_API FalagardSoftKeyBoard : public FalagardStaticImage, public IFalagardSoftKeyBoard
	{
	public:
		static const utf8   WidgetTypeName[];       
		static const String		EventNamespace;
	private:
		std::vector<SKey_Position> m_aKeyPosList[SoftKey_TypeNumber ]; // 7���ַ���λ��
		std::vector<SKey_Define> m_aKeyDefineList[ SoftKeyDef_MaxNumber ];
		SKey_Define   m_KeyDefReturn;
		SKey_Define   m_KeyDefBack;
		SKey_Define   m_KeyDefShift;
		Window*  m_pAimEditBox;
		bool			m_bShiftDown;
		bool			m_bKeyDown;

		SKey_Position* m_pLastKey;
		SKey_Position* m_pCurrentKey;

		float GridWidth;
		float GridHeight;

		const Image*  m_pHoverImage[ BUTTON_TYPE_NUMBER ]; /// ����������
		const Image*  m_pPushedImage[ BUTTON_TYPE_NUMBER ]; /// ���µ�����

	protected:

		void setKeyPosition( int x, int y, int nType );
		void setKeyDefine( utf32 nchar, int nType );
		void ResetAllKey();
		void setKeyHover( SKey_Position* pKey, bool bHover );
		// �������λ���ϵ�״̬
		void updateKeyPosition( const Point& screenPos );
		// �����Ǹ��������λ����
		SKey_Position* getKeyByPosition( const Point& screenPos );

		void resetAllPosition();
	public:
		FalagardSoftKeyBoard(const String& type, const String& name);
		~FalagardSoftKeyBoard(void);
		virtual void initialise();
		virtual void SetActiveEditBox( String& windowName );
		virtual void onMouseMove( MouseEventArgs& e );
		virtual void onMouseButtonDown( MouseEventArgs& e );
		virtual void onMouseButtonUp( MouseEventArgs& e );
		virtual void onMouseLeaves( MouseEventArgs& e );
		virtual void populateRenderCache();
		virtual void onShown(WindowEventArgs& e);

		void setKeySize( const Size& size );

		void setHoverImage( int nType, const Image* pImage );
		void setPushedImage( int nType,const Image* pImage );


		static FalagardSoftKeyProperties::HoverImage   d_hoverImage;
		static FalagardSoftKeyProperties::PushedImage  d_pushedImage;
		static FalagardSoftKeyProperties::KeySize      d_keySize;

		static const String EventReturn;			
	};

	class FALAGARDBASE_API FalagardSoftKeyFactory : public WindowFactory
    {
    public:
        FalagardSoftKeyFactory(void) : WindowFactory(FalagardSoftKeyBoard::WidgetTypeName) { }
        ~FalagardSoftKeyFactory(void){}
        Window* createWindow(const String& name);
        void destroyWindow(Window* window);
    };
};