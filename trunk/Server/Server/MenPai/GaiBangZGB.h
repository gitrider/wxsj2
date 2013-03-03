#ifndef GAIBANG_ZGB_H
#define GAIBANG_ZGB_H

#include "MenPai.h"

namespace MenPai_Module
{
	class GaiBangZGB_T: public MenPai_T
	{
		public:
			enum
			{
				ID= MenPai_T::ID_GAIBANG_ZGB,
			};
			
			GaiBangZGB_T(){};
			~GaiBangZGB_T(){};
			MenPai_T::MenPaiID_T GetID(VOID) const {return (MenPai_T::MenPaiID_T)ID;};
			//Add Rage And Strike point part here
			VOID OnDepleteStrikePoints(Obj_Human& rMe, INT nStrikePoint) const;
			virtual VOID OnDamage(Obj_Human& rMe, INT nDamage) const{};
			
		protected:
		private:
			INT TransferValueToRage(INT nDamage) const;
	};
};
#endif //GaiBangZGB_H


