#ifndef WUDANG_JWD_H
#define WUDANG_JWD_H

#include "MenPai.h"

namespace MenPai_Module
{
	class WuDangJWD_T: public MenPai_T
	{
		public:
			enum
			{
				ID= MenPai_T::ID_WUDANG_JWD,
			};
			
			WuDangJWD_T(){};
			~WuDangJWD_T(){};
			MenPai_T::MenPaiID_T GetID(VOID) const {return (MenPai_T::MenPaiID_T)ID;};
			virtual VOID OnDamage(Obj_Human& rMe, INT nDamage) const{};
			

			//Add Rage And Strike point part here

		protected:
		private:
	};
};
#endif //WUDANG_JWD_H


