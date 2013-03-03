#ifndef TAOHUAGU_DTH_H
#define TAOHUAGU_DTH_H

#include "MenPai.h"

namespace MenPai_Module
{
	class TaoHuaGuDTH_T: public MenPai_T
	{
		public:
			enum
			{
				ID= MenPai_T::ID_TAOHUAGU_DTH,
			};
			
			TaoHuaGuDTH_T(){};
			~TaoHuaGuDTH_T(){};
			MenPai_T::MenPaiID_T GetID(VOID) const {return (MenPai_T::MenPaiID_T)ID;};
			virtual VOID OnDamage(Obj_Human& rMe, INT nDamage) const{};
			
			//Add Rage And Strike point part here

		protected:
		private:
	};
};
#endif //TAOHUAGU_DTH_H


