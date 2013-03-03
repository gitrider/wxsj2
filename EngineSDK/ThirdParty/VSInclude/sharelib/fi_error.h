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
		return "没有权限";
		break;
	case FI_MACRO_ERROR_UNSUPPORT_REQUEST:
		return "不支持的操作";
		break;
	case FI_MACRO_ERROR_SERVER_NOT_EXIST:
		return "服务器不存在";
		break;
	case FI_MACRO_UNKONW_USER:
		return "用户不存在";
		break;
	default:
		return "未知的错误";
	}
}
#endif
