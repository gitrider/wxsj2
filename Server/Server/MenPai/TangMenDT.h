#ifndef TANGMEN_DT_H
#define TANGMEN_DT_H

#include "MenPai.h"

namespace MenPai_Module
{
	class TangMenDT_T: public MenPai_T
	{
		public:
			enum
			{
				ID= MenPai_T::ID_TANGMEN_DT,
			};
			
			TangMenDT_T(){};
			~TangMenDT_T(){};
			MenPai_T::MenPaiID_T GetID(VOID) const {return (MenPai_T::MenPaiID_T)ID;};
			virtual VOID OnDamage(Obj_Human& rMe, INT nDamage) const{};
			//Add Rage And Strike point part here

		protected:
		private:
	};
};
#endif //TANGMEN_DT_H


