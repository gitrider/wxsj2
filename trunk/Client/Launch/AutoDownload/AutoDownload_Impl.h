/************************************************\
*												*
*												*
*			AtuoDownload 内部数据				*
*												*
*								by jinchao		*
*												*
\************************************************/

#pragma once

struct CAutoDownload::AUTODOWN_IMPL
{
	//回调窗口
	HWND			m_hCallbackWnd;

	//当前状态
	AUTODOWN_STATE	m_state;

	//回调OwnerID
	int				m_nOwnerID;

	//下载文件URl
	std::string		m_strURL;

	//下载文件本地位置
	std::string		m_strLocalFile;
};