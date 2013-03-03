#ifndef ENCODER_XOR_H
#define ENCODER_XOR_H

#include "Encoder.h"

namespace Encode_Decode
{
	class EncoderXOR_T : public Encoder_T
	{
		public:
			EncoderXOR_T(VOID) {};
			~EncoderXOR_T(VOID) {};
			VOID Reset(VOID) {};
			virtual BOOL Encode(Encoder_T::Parameters_T& rParams) const;
			virtual BOOL Decode(Encoder_T::Parameters_T& rParams) const;
			virtual BOOL Skip(Parameters_T& rParams, INT nSize) const;
			virtual INT GetID(VOID) const {return ID;};
			enum
			{
				ID = Encoder_T::ENCODER_XOR,
			};
		protected:
		private:
			
			
	};
};
#endif //ENCODER_XOR_H
