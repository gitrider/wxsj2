#ifndef ENCODER_H
#define ENCODER_H

#include "BaseType.h"

namespace Encode_Decode
{
	class Encoder_T
	{
		public:
			enum
			{
				ENCODER_INVALID = INVALID_ID,
				ENCODER_XOR = 0,
			};
			class Parameters_T
			{
				public:
					Parameters_T(VOID) :
						m_pCripher(NULL),
						m_nCripherSize(0),
						m_pPlainText(NULL),
						m_nPlainTextSize(0),
						m_pKey(NULL),
						m_nKeySize(0),
						m_nParam0(0),
						m_nParam1(0)
					{};
					~Parameters_T(VOID) {};
					VOID Reset(VOID)
					{
						m_pCripher = NULL;
						m_nCripherSize = 0;
						m_pPlainText = NULL;
						m_nPlainTextSize = 0;
						m_pKey = NULL;
						m_nKeySize = 0;
						m_nParam0 = 0;
						m_nParam1 = 0;
					};
					UCHAR const* GetConstPlainText(VOID) const {return m_pPlainText;};
					UCHAR* GetAlterablePlainText(VOID) {return m_pPlainText;};
					VOID SetPlainText(UCHAR *pPlainText) {m_pPlainText = pPlainText;};
					INT  GetPlainTextSize(VOID) const {return m_nPlainTextSize;};
					VOID SetPlainTextSize(INT nSize) {m_nPlainTextSize = nSize;};

					UCHAR const* GetConstCripher(VOID) const {return m_pCripher;};
					UCHAR* GetAlterableCripher(VOID) {return m_pCripher;};
					VOID SetCripher(UCHAR *pCripher) {m_pCripher = pCripher;};
					INT  GetCripherSize(VOID) const {return m_nCripherSize;};
					VOID SetCripherSize(INT nSize) {m_nCripherSize = nSize;};

					UCHAR const* GetKey(VOID) const {return m_pKey;};
					VOID SetKey(UCHAR const* pKey) {m_pKey = pKey;};
					INT  GetKeySize(VOID) const {return m_nKeySize;};
					VOID SetKeySize(INT nSize) {m_nKeySize = nSize;};

					INT  GetParam0(VOID) const {return m_nParam0;}
					VOID SetParam0(INT nParam) {m_nParam0 = nParam;};

					INT  GetParam1(VOID) const {return m_nParam1;}
					VOID SetParam1(INT nParam) {m_nParam1 = nParam;};

				protected:
				private:
					UCHAR * m_pPlainText;
					INT m_nPlainTextSize;
					UCHAR * m_pCripher;
					INT m_nCripherSize;
					UCHAR const* m_pKey;
					INT m_nKeySize;
					INT m_nParam0;
					INT m_nParam1;
			};
			enum
			{
				ID = ENCODER_INVALID,
				MAX_ID = 15,
			};
		public:
			Encoder_T() {};
			~Encoder_T() {};
			virtual VOID Reset(){};
			virtual BOOL Encode(Parameters_T& rParams) const {return FALSE;};
			virtual BOOL Decode(Parameters_T& rParams) const {return FALSE;};
			virtual BOOL Skip(Parameters_T& rParams, INT nSize) const {return FALSE;};
			virtual INT GetID(VOID) const {return ID;};
		protected:
		private:
	};
};
#endif //ENCODER_H
