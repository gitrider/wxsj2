#pragma once


//���ؽ��
enum DOWN_RESULT
{
	DR_BUSYING = 0,	//����������...

	DR_SUCCESS,		//�ɹ�����
	DR_ERROR,		//����ʧ��
	DR_CANCEL,		//����ȡ��
};


DOWN_RESULT ShowDownDialog(void);
