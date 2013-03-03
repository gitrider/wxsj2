#ifndef TAOHUAGU_YTH_H
#define TAOHUAGU_YTH_H

#include "MenPai.h"

namespace MenPai_Module
{
	class TaoHuaGuYTH_T: public MenPai_T
	{
		public:
			enum
			{
				ID= MenPai_T::ID_TAOHUAGU_YTH,
			};
			
			TaoHuaGuYTH_T(){};
			~TaoHuaGuYTH_T(){};
			MenPai_T::MenPaiID_T GetID(VOID) const {return (MenPai_T::MenPaiID_T)ID;};
			virtual VOID OnDamage(Obj_Human& rMe, INT nDamage) const{};
			
			//Add Rage And Strike point part here

		protected:
		private:
	};
};
#endif //TAOHUAGU_YTH_H


