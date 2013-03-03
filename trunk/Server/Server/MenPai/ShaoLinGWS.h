#ifndef SHAOLIN_GWS_H
#define SHAOLIN_GWS_H
#include "MenPai.h"
namespace MenPai_Module
{
	class ShaoLinGWS_T: public MenPai_T
	{
		public:
			enum
			{
				ID= MenPai_T::ID_SHAOLIN_GWS,
			};
			
			ShaoLinGWS_T(){};
			~ShaoLinGWS_T(){};
			MenPai_T::MenPaiID_T GetID(VOID) const {return (MenPai_T::MenPaiID_T)ID;};
			//Add Rage And Strike point part here
			
		protected:
		private:
	};
};
#endif //ShaoLinGWS_H

