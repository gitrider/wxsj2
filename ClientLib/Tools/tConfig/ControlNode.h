#pragma once

//使用Slider控制的项目
struct SliderNode
{
	const char*		szVariableName;
	int				nValueNum;
	int				nValueBuf[8];
	const int		szValueDesc[8];

	UINT		nID_Slider;
	UINT		nID_Desc;
};

//使用CheckBox控制的项目
struct CheckNode
{
	const char*		szVariableName;

	UINT			nID_Slider;
};