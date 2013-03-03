#ifndef __FI_ERROR_DEFINE_H__
#define __FI_ERROR_DEFINE_H__

#define FI_MACRO_UNKONW_USER			-1000
#define FI_MACRO_ERROR_NO_PRIVILITY		-1001

#define FI_MACRO_ERROR_UNSUPPORT_REQUEST	-1002
#define FI_MACRO_ERROR_SERVER_NOT_EXIST		-1010


inline
std::string FI_GetErrorString( int nErrorNo )
{
	switch( nErrorNo )
	{
	case FI_MACRO_ERROR_NO_PRIVILITY:
		return "û��Ȩ��";
		break;
	case FI_MACRO_ERROR_UNSUPPORT_REQUEST:
		return "��֧�ֵĲ���";
		break;
	case FI_MACRO_ERROR_SERVER_NOT_EXIST:
		return "������������";
		break;
	case FI_MACRO_UNKONW_USER:
		return "�û�������";
		break;
	default:
		return "δ֪�Ĵ���";
	}
}
#endif
