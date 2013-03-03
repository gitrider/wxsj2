#ifndef GAIBANG_GGB_H
#define GAIBANG_GGB_H

#include "MenPai.h"

namespace MenPai_Module
{
	class GaiBangGGB_T: public MenPai_T
	{
		public:
			enum
			{
				ID= MenPai_T::ID_GAIBANG_GGB,
			};
			
			GaiBangGGB_T(){};
			~GaiBangGGB_T(){};
			MenPai_T::MenPaiID_T GetID(VOID) const {return (MenPai_T::MenPaiID_T)ID;};
			//Add Rage And Strike point part here
			VOID OnDepleteStrikePoints(Obj_Human& rMe, INT nStrikePoint) const;
			virtual VOID OnDamage(Obj_Human& rMe, INT nDamage) const{};
			
		protected:
		private:
			INT TransferValueToRage(INT nDamage) const;
	};
};
#endif //GaiBangGGB_H


