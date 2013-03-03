#ifndef YAOWANGGU_YYW_H
#define YAOWANGGU_YYW_H

#include "MenPai.h"

namespace MenPai_Module
{
	class YaoWangGuYYW_T: public MenPai_T
	{
		public:
			enum
			{
				ID= MenPai_T::ID_YAOWANGGU_YYW,
			};
			
			YaoWangGuYYW_T(){};
			~YaoWangGuYYW_T(){};
			MenPai_T::MenPaiID_T GetID(VOID) const {return (MenPai_T::MenPaiID_T)ID;};
			virtual VOID OnDamage(Obj_Human& rMe, INT nDamage) const{};
			
			//Add Rage And Strike point part here

		protected:
		private:
	};
};
#endif //YAOWANGGU_YYW_H


