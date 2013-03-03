#ifndef EMEI_CEM_H
#define EMEI_CEM_H

#include "MenPai.h"

namespace MenPai_Module
{
	class EMeiCEM_T: public MenPai_T
	{
		public:
			enum
			{
				ID= MenPai_T::ID_EMEI_CEM,
			};
			
			EMeiCEM_T(){};
			~EMeiCEM_T(){};
			MenPai_T::MenPaiID_T GetID(VOID) const {return (MenPai_T::MenPaiID_T)ID;};
			//Add Rage And Strike point part here
			virtual VOID OnHealTarget(Obj_Human& rMe, INT nHealedHp) const;
			virtual VOID OnDamage(Obj_Human& rMe, INT nDamage) const{};
			
		protected:
		private:
			virtual INT TransferValueToRage(INT nDamage) const;
	};
};
#endif //EMEI_CEM_H


