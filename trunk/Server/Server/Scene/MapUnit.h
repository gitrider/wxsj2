

#ifndef __MAPUNIT_H__
#define __MAPUNIT_H__

/*

	.map �ļ�

�ļ�ͷ

	struct MAP_UNIT_FILEHEAD
	{
		UINT		uMagic;		// 'MAPU'   0X5550414D
		INT			nXSize;			// X�����С
		INT			nZSize;			// Z�����С
	};
	
�ļ��ṹ

     [MAP_UNIT_FILEHEAD] [_MAP_UNIT] [_MAP_UNIT] [_MAP_UNIT] ...
                         \   ___________________________ ...
                                   nXSize*nZSize

*/

//�����༭�����ɵ�ÿ����ͼ���е���Ϣ
struct _MAP_UNIT
{
	WORD	m_Flag ;	//��־��Ϣ
						// ���ISSET0(m_Flag)��ֵλ�棬���ͼ��λ������ 
						//		    
                        //     00000000|00000000
                        //                    ||_  WalkDisable  -�Ƿ��ֹ���������ߵ�OBJ��Խ  [0 �ɴ�Խ  1���ɴ�Խ]
	                    //                    |__  FlyDisable   -�Ƿ��ֹ���з��е�OBJ��Խ    [0 �ɴ�Խ  1���ɴ�Խ]
                        //

	BYTE	m_Height ;	//�߶���Ϣ
	BYTE	m_Reserved ;


	_MAP_UNIT( )
	{
		m_Flag = 0 ;
		m_Height = 0 ;
		m_Reserved = 0 ;
	};
};

#endif
