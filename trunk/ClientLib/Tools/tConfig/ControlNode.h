#pragma once

//ʹ��Slider���Ƶ���Ŀ
struct SliderNode
{
	const char*		szVariableName;
	int				nValueNum;
	int				nValueBuf[8];
	const int		szValueDesc[8];

	UINT		nID_Slider;
	UINT		nID_Desc;
};

//ʹ��CheckBox���Ƶ���Ŀ
struct CheckNode
{
	const char*		szVariableName;

	UINT			nID_Slider;
};