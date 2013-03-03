
namespace CEGUI
{

	class IFalagardAnimate
	{
	public:
		/*!
		\brief
			Recaculate all subwindow
		*/
		virtual void	Play(bool bPlay) = 0;

		virtual void	SetAnimate( const String& strAnimate ) = 0;

	};

}