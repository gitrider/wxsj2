#ifndef WUDANG_QWD_H
#define WUDANG_QWD_H

#include "MenPai.h"

namespace MenPai_Module
{
	class WuDangQWD_T: public MenPai_T
	{
		public:
			enum
			{
				ID= MenPai_T::ID_WUDANG_QWD,
			};
			
			WuDangQWD_T(){};
			~WuDangQWD_T(){};
			MenPai_T::MenPaiID_T GetID(VOID) const {return (MenPai_T::MenPaiID_T)ID;};
			virtual VOID OnDamage(Obj_Human& rMe, INT nDamage) const{};
			
			//Add Rage And Strike point part here

		protected:
		private:
	};
};
#endif //WUDANG_QWD_H


