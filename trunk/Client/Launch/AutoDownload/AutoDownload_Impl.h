/************************************************\
*												*
*												*
*			AtuoDownload �ڲ�����				*
*												*
*								by jinchao		*
*												*
\************************************************/

#pragma once

struct CAutoDownload::AUTODOWN_IMPL
{
	//�ص�����
	HWND			m_hCallbackWnd;

	//��ǰ״̬
	AUTODOWN_STATE	m_state;

	//�ص�OwnerID
	int				m_nOwnerID;

	//�����ļ�URl
	std::string		m_strURL;

	//�����ļ�����λ��
	std::string		m_strLocalFile;
};