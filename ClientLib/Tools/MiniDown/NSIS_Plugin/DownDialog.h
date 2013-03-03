#pragma once


//下载结果
enum DOWN_RESULT
{
	DR_BUSYING = 0,	//正在下载中...

	DR_SUCCESS,		//成功下载
	DR_ERROR,		//下载失败
	DR_CANCEL,		//下载取消
};


DOWN_RESULT ShowDownDialog(void);
