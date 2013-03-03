#ifndef XIAOYAO_ZXY_H
#define XIAOYAO_ZXY_H

#include "MenPai.h"

namespace MenPai_Module
{
	class XiaoYaoZXY_T: public MenPai_T
	{
		public:
			enum
			{
				ID= MenPai_T::ID_XIAOYAO_ZXY,
			};
			
			XiaoYaoZXY_T(){};
			~XiaoYaoZXY_T(){};
			MenPai_T::MenPaiID_T GetID(VOID) const {return (MenPai_T::MenPaiID_T)ID;};
			//Add Rage And Strike point part here
			virtual VOID OnMyTrapActivated(Obj_Human& rMe, Obj_Special& rTrap) const;

		protected:
		private:
			virtual INT TransferValueToRage(INT nDamage) const;
	};
};
#endif //XIAOYAO_ZXY_H


