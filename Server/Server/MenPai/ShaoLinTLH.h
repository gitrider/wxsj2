#ifndef SHAOLIN_TLH_H
#define SHAOLIN_TLH_H
#include "MenPai.h"
namespace MenPai_Module
{
	class ShaoLinTLH_T: public MenPai_T
	{
		public:
			enum
			{
				ID= MenPai_T::ID_SHAOLIN_TLH,
			};
			
			ShaoLinTLH_T(){};
			~ShaoLinTLH_T(){};
			MenPai_T::MenPaiID_T GetID(VOID) const {return (MenPai_T::MenPaiID_T)ID;};
			//Add Rage And Strike point part here
			
		protected:
		private:
	};
};
#endif //ShaoLinTLH_H

