#ifndef _Model_Types_h_
#define _Model_Types_h_



// ���ֿռ䶨��

#define Begin_Model namespace ModelSystem  \
{

#define End_Model	};


// API ����

#ifdef MODELSYSTEM_STATIC_LINK
#define MODEL_API
#else
#ifndef MODELSYSTEM_EXPORTS
#define MODEL_API		__declspec(dllimport)
#else
#define MODEL_API		__declspec(dllexport)
#endif 
#endif

//Ogreͷ�ļ�����



#endif