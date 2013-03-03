/********************************************************************************
 *	�ļ�����	GameStruct_TopList.h
 *	�����ˣ�	�����
 *	����ʱ�䣺	2008 �� 4 �� 8 ��	20:00
 *
 *	����˵����	���а��һЩ���ݽṹ����Ϣ�ṹ
 *	�޸ļ�¼��
 *	�޸��ˣ�	yyz
 *	�޸�ʱ�䣺	2010 ��	7 �� 12��

*********************************************************************************/

#ifndef __GAMESTRUCT_TOPLIST_H__
#define __GAMESTRUCT_TOPLIST_H__

#include "Type.h"
#include "Packet.h"

#define LEVEL_TOP_MEMBER_NUM  5000

enum TOP_LIST_TYPE
{//�����ID�Ǻ�toplist�����id���Ӧ�ģ��޸�����Ҳ���޸ı������а��id
	LEVEL_TOP_LIST = 0,			//�ȼ����а�
	MONEY_TOP_LIST ,			//��Ϸ�����а�
	REPUTE_TOP_LIST,			//�������а�
	PET_TOP_LIST,				//�������а�
	ECTYPE_TOP_LIST,			//�������а�
	ERRANTRY_TOP_LIST,			//�������а�
	CRIME_TOP_LIST,				//������а�
	BINDMONEY_TOP_LIST,			//����Ϸ�����а�
	RMB_TOP_LIST,				//��������а�
	BINDRMB_TOP_LIST,			//����������а�
	FAVOR_TOP_LIST,				//�������а�
	GUILD_TOP_LIST,				//�������а�
	
	TOP_LIST_NUM,				//���а�����

};

//���а������Ϣ
struct TOP_LIST_ATTR
{
	INT			m_TopListID;				//���а�ID
	CHAR		m_Name[MAX_TOP_LIST_NAME];	
	UINT		m_UpdateInterval;			//ˢ�µ�ʱ����
	INT			m_MemberCount ;				//���а���������
	INT			m_RecordCount;				//��ǰ��¼��
	INT			m_EnterMinKey;				//����������а����С�ȼ�
};


//����Ҫ��ʾ����һ�������
struct _TOP_DATA
{			
	GUID_t	m_Guid;							//���GUID
	INT		m_Level;						//�ȼ�
	INT		m_Country;						//����
	CHAR	m_Name[MAX_CHARACTER_NAME];		//����
	INT		m_NameSize;
	MenPaiID_t	 m_MenPai;					//ְҵ

	UINT	GetSize( ) const
	{
		UINT uSize = sizeof(m_Guid) + sizeof(m_Level) + sizeof(m_Country)
					 + m_NameSize + sizeof(m_NameSize) + sizeof(m_MenPai);
		return uSize;
	}

	VOID	Read( SocketInputStream& iStream )
	{
		iStream.Read( (CHAR*)&m_Guid,sizeof(m_Guid) );
		iStream.Read( (CHAR*)&m_Level,sizeof(m_Level) );
		iStream.Read( (CHAR*)&m_Country,sizeof(m_Country) );
		iStream.Read( (CHAR*)&m_NameSize,sizeof(m_NameSize) );

		
		if( m_NameSize > 0 && m_NameSize < MAX_CHARACTER_NAME )
		{
			iStream.Read(m_Name,m_NameSize );
		}	
		iStream.Read( (CHAR*)&m_MenPai,sizeof(m_MenPai) );
	}

	VOID	Write( SocketOutputStream& oStream ) const
	{
		oStream.Write( (CHAR*)&m_Guid,sizeof(m_Guid) );
		oStream.Write( (CHAR*)&m_Level, sizeof(m_Level) );
		oStream.Write( (CHAR*)&m_Country,sizeof(m_Country) );
		oStream.Write( (CHAR*)&m_NameSize,sizeof(m_NameSize) );
		
		if( m_NameSize > 0 && m_NameSize < MAX_CHARACTER_NAME )
		{
			oStream.Write( m_Name, m_NameSize );
		}

		oStream.Write( (CHAR*)&m_MenPai,sizeof(m_MenPai) );
	}
};


//�ȼ����а���Ҫ��ʾ���������: �������� + ��չ����
struct _LEVEL_TOP_DATA
{
	_LEVEL_TOP_DATA()
	{
		memset(&m_UserData,0,sizeof(_TOP_DATA));
		m_Exp	 = INVALID_ID;
		m_OnlineTime = 0;
		m_GuildNameSize  = 0;
		memset(m_GuildName,0,MAX_GUILD_NAME_SIZE);
		m_OldRank = 0;
	}

	UINT	GetSize( ) const
	{
		UINT uSize = sizeof(m_Exp) + 
					 sizeof(m_OldRank)+
					 sizeof(m_OnlineTime)+
					 sizeof(BYTE) + 
					sizeof(CHAR) * m_GuildNameSize+ 
					 m_UserData.GetSize();
		return uSize;
	}

	VOID	Read( SocketInputStream& iStream )
	{
		iStream.Read( (CHAR*)&m_Exp, sizeof(m_Exp) );
		iStream.Read( (CHAR*)&m_OnlineTime, sizeof(m_OnlineTime) );
		iStream.Read( (CHAR*)&m_OldRank, sizeof(m_OldRank) );
		iStream.Read( (CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize) );
		if( m_GuildNameSize > 0 && m_GuildNameSize < MAX_GUILD_NAME_SIZE )
		{
			iStream.Read(m_GuildName,m_GuildNameSize );
		}	
		m_UserData.Read(iStream);
	}

	VOID	Write( SocketOutputStream& oStream ) const
	{
		oStream.Write( (CHAR*)&m_Exp, sizeof(m_Exp) );
		oStream.Write( (CHAR*)&m_OnlineTime, sizeof(m_OnlineTime) );
		oStream.Write( (CHAR*)&m_OldRank, sizeof(m_OldRank) );
		oStream.Write( (CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize) );
		if( m_GuildNameSize > 0 && m_GuildNameSize < MAX_GUILD_NAME_SIZE )
		{
			oStream.Write(m_GuildName,m_GuildNameSize );
		}	
		m_UserData.Write(oStream);
	}

	_TOP_DATA	m_UserData;				//��һ�������

	//��������չ����
	INT			m_Exp;					//����ֵ���ڴ���Ϊ����
	UINT		m_OnlineTime;			//����ʱ�䣬�ڴ����θ���
	CHAR		m_GuildName[MAX_GUILD_NAME_SIZE];		//�������
	BYTE		m_GuildNameSize;
	INT			m_OldRank;			//֮ǰ������
};

//��Ϸ�����а���Ҫ��ʾ���������: �������� + ��չ����
struct _MONEY_TOP_DATA
{
	_MONEY_TOP_DATA()
	{
		memset(&m_UserData,0,sizeof(_TOP_DATA));
		m_Money  = 0;
		m_OnLineTime = 0;
		m_OldRank = 0;
		m_GuildNameSize  = 0;
		memset(m_GuildName,0,MAX_GUILD_NAME_SIZE);
	
	}

	UINT	GetSize( ) const
	{
		UINT uSize = sizeof(m_Money) +
			sizeof(m_OldRank)+
			sizeof(m_OnLineTime)+
			sizeof(BYTE) + 
			sizeof(CHAR) * m_GuildNameSize+ 
			m_UserData.GetSize();
		return uSize;

	}

	VOID	Read( SocketInputStream& iStream )
	{
		iStream.Read( (CHAR*)&m_Money, sizeof(m_Money) );
		iStream.Read( (CHAR*)&m_OnLineTime, sizeof(m_OnLineTime) );
		iStream.Read( (CHAR*)&m_OldRank, sizeof(m_OldRank) );
		iStream.Read( (CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize) );
		if( m_GuildNameSize > 0 && m_GuildNameSize < MAX_GUILD_NAME_SIZE )
		{
			iStream.Read(m_GuildName,m_GuildNameSize );
		}
		m_UserData.Read(iStream);
	}

	VOID	Write( SocketOutputStream& oStream ) const
	{
		oStream.Write( (CHAR*)&m_Money, sizeof(m_Money) );
		oStream.Write( (CHAR*)&m_OnLineTime, sizeof(m_OnLineTime) );
		oStream.Write( (CHAR*)&m_OldRank, sizeof(m_OldRank) );
		oStream.Write( (CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize) );
		if( m_GuildNameSize > 0 && m_GuildNameSize < MAX_GUILD_NAME_SIZE )
		{
			oStream.Write(m_GuildName,m_GuildNameSize );
		}
		m_UserData.Write(oStream);
	}

	_TOP_DATA	m_UserData;				//��һ�������

	//��������չ����
	UINT	m_Money;	
	UINT	m_OnLineTime;				//���������ʱ��,��Ϊ����
	INT		m_OldRank;					//���֮ǰ������
	CHAR	m_GuildName[MAX_GUILD_NAME_SIZE];		//�������
	BYTE	m_GuildNameSize;

};

//�������а���Ҫ��ʾ����ҵ����ԣ��������� + ��չ����
struct _REPUTE_TOP_DATA
{
	_REPUTE_TOP_DATA()
	{
		memset(&m_UserData,0,sizeof(_TOP_DATA));
		m_Repute = 0;
		m_GuildNameSize  = 0;
		memset(m_GuildName,0,MAX_GUILD_NAME_SIZE);
	}

	UINT	GetSize() const
	{
		UINT uSize = sizeof(m_Repute) +
					 sizeof(BYTE) + 
					 sizeof(CHAR) * m_GuildNameSize+ 
					 m_UserData.GetSize();
		return uSize;
	}

	VOID	Read( SocketInputStream& iStream )
	{
		iStream.Read( (CHAR*)&m_Repute, sizeof(m_Repute) );
		iStream.Read( (CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize) );
		if( m_GuildNameSize > 0 && m_GuildNameSize < MAX_GUILD_NAME_SIZE )
		{
			iStream.Read(m_GuildName,m_GuildNameSize );
		}
		m_UserData.Read(iStream);
	}

	VOID	Write( SocketOutputStream& oStream ) const
	{
		oStream.Write( (CHAR*)&m_Repute, sizeof(m_Repute) );
		oStream.Write( (CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize) );
		if( m_GuildNameSize > 0 && m_GuildNameSize < MAX_GUILD_NAME_SIZE )
		{
			oStream.Write(m_GuildName,m_GuildNameSize );
		}
		m_UserData.Write(oStream);
	}

	_TOP_DATA	m_UserData;				//��һ�������

	//��������չ����
	UINT	m_Repute;					//��ҵ�����

	CHAR	m_GuildName[MAX_GUILD_NAME_SIZE];		//�������
	BYTE	m_GuildNameSize;

};



//�������а���Ҫ��ʾ����ҵ�����: �������� + ��չ����
struct	_PET_TOP_DATA
{
	_PET_TOP_DATA()
	{
		memset(&m_UserData,0,sizeof(_TOP_DATA));
		m_PetLevel  = 0;
		m_GuildNameSize  = 0;
		memset(m_GuildName,0,MAX_GUILD_NAME_SIZE);
	}

	UINT	GetSize( ) const
	{
		UINT uSize = sizeof(m_PetLevel) +
			sizeof(BYTE) + 
			sizeof(CHAR) * m_GuildNameSize+ 
			m_UserData.GetSize();
		return uSize;

	}

	VOID	Read( SocketInputStream& iStream )
	{
		iStream.Read( (CHAR*)&m_PetLevel, sizeof(m_PetLevel) );
		iStream.Read( (CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize) );
		if( m_GuildNameSize > 0 && m_GuildNameSize < MAX_GUILD_NAME_SIZE )
		{
			iStream.Read(m_GuildName,m_GuildNameSize );
		}
		m_UserData.Read(iStream);
	}


	VOID	Write( SocketOutputStream& oStream ) const
	{
		oStream.Write( (CHAR*)&m_PetLevel, sizeof(m_PetLevel) );
		oStream.Write( (CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize) );
		if( m_GuildNameSize > 0 && m_GuildNameSize < MAX_GUILD_NAME_SIZE )
		{
			oStream.Write(m_GuildName,m_GuildNameSize );
		}
		m_UserData.Write(oStream);
	}

	_TOP_DATA	m_UserData;			//��һ�������

	//��������չ����
	UINT	m_PetLevel;				//����ȼ�

	CHAR	m_GuildName[MAX_GUILD_NAME_SIZE];		//�������
	BYTE	m_GuildNameSize;

};

//�������а���Ҫ��ʾ����ҵ�����: �������� + ��չ����
struct  _ECTYPE_TOP_DATA
{
	_ECTYPE_TOP_DATA()
	{
		memset(&m_UserData,0,sizeof(_TOP_DATA));
		EctypeLevel  = 0;
		EctypeNum = 0;
		m_GuildNameSize  = 0;
		memset(m_GuildName,0,MAX_GUILD_NAME_SIZE);

	}

	UINT	GetSize( ) const
	{
		UINT uSize = sizeof(EctypeLevel) +
			sizeof(EctypeNum)+
			sizeof(BYTE) + 
			sizeof(CHAR) * m_GuildNameSize+ 
			m_UserData.GetSize();
		return uSize;

	}

	VOID	Read( SocketInputStream& iStream )
	{
		iStream.Read( (CHAR*)&EctypeLevel, sizeof(EctypeLevel) );
		iStream.Read( (CHAR*)&EctypeNum, sizeof(EctypeNum) );
		iStream.Read( (CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize) );
		if( m_GuildNameSize > 0 && m_GuildNameSize < MAX_GUILD_NAME_SIZE )
		{
			iStream.Read(m_GuildName,m_GuildNameSize );
		}
		m_UserData.Read(iStream);
	}

	VOID	Write( SocketOutputStream& oStream ) const
	{
		oStream.Write( (CHAR*)&EctypeLevel, sizeof(EctypeLevel) );
		oStream.Write( (CHAR*)&EctypeNum, sizeof(EctypeNum) );
		oStream.Write( (CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize) );
		if( m_GuildNameSize > 0 && m_GuildNameSize < MAX_GUILD_NAME_SIZE )
		{
			oStream.Write(m_GuildName,m_GuildNameSize );
		}
		m_UserData.Write(oStream);
	}

	_TOP_DATA	m_UserData;				//��һ�������

	//��������չ����
	UINT	EctypeNum;					//������������	
	UINT	EctypeLevel;				//������������
	
	CHAR	m_GuildName[MAX_GUILD_NAME_SIZE];		//�������
	BYTE	m_GuildNameSize;
};

//�������а���Ҫ��ʾ���������: �������� + ��չ����
struct _ERRANTRY_TOP_DATA 
{
	_ERRANTRY_TOP_DATA()
	{
		memset(&m_UserData,0,sizeof(_TOP_DATA));
		m_Errantry  = 0;
		m_GuildNameSize  = 0;
		memset(m_GuildName,0,MAX_GUILD_NAME_SIZE);

	}

	UINT	GetSize( ) const
	{
		UINT uSize = sizeof(m_Errantry) +
			sizeof(BYTE) + 
			sizeof(CHAR) * m_GuildNameSize+ 
			m_UserData.GetSize();
		return uSize;

	}

	VOID	Read( SocketInputStream& iStream )
	{
		iStream.Read( (CHAR*)&m_Errantry, sizeof(m_Errantry) );
		iStream.Read( (CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize) );
		if( m_GuildNameSize > 0 && m_GuildNameSize < MAX_GUILD_NAME_SIZE )
		{
			iStream.Read(m_GuildName,m_GuildNameSize );
		}
		m_UserData.Read(iStream);
	}

	VOID	Write( SocketOutputStream& oStream ) const
	{
		oStream.Write( (CHAR*)&m_Errantry, sizeof(m_Errantry) );
		oStream.Write( (CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize) );
		if( m_GuildNameSize > 0 && m_GuildNameSize < MAX_GUILD_NAME_SIZE )
		{
			oStream.Write(m_GuildName,m_GuildNameSize );
		}
		m_UserData.Write(oStream);
	}

	_TOP_DATA	m_UserData;				//��һ�������

	//��������չ����
	UINT	 m_Errantry;				//��ҵ�����ֵ

	CHAR	m_GuildName[MAX_GUILD_NAME_SIZE];		//�������
	BYTE	m_GuildNameSize;

};

//������а���Ҫ��ʾ���������: �������� + ��չ����
struct _CRIME_TOP_DATA 
{
	_CRIME_TOP_DATA()
	{
		memset(&m_UserData,0,sizeof(_TOP_DATA));
		m_Crime  = 0;
		m_GuildNameSize  = 0;
		memset(m_GuildName,0,MAX_GUILD_NAME_SIZE);

	}

	UINT	GetSize( ) const
	{
		UINT uSize = sizeof(m_Crime) +
			sizeof(BYTE) + 
			sizeof(CHAR) * m_GuildNameSize+ 
			m_UserData.GetSize();
		return uSize;

	}

	VOID	Read( SocketInputStream& iStream )
	{
		iStream.Read( (CHAR*)&m_Crime, sizeof(m_Crime) );
		iStream.Read( (CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize) );
		if( m_GuildNameSize > 0 && m_GuildNameSize < MAX_GUILD_NAME_SIZE )
		{
			iStream.Read(m_GuildName,m_GuildNameSize );
		}
		m_UserData.Read(iStream);
	}

	VOID	Write( SocketOutputStream& oStream ) const
	{
		oStream.Write( (CHAR*)&m_Crime, sizeof(m_Crime) );
		oStream.Write( (CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize) );
		if( m_GuildNameSize > 0 && m_GuildNameSize < MAX_GUILD_NAME_SIZE )
		{
			oStream.Write(m_GuildName,m_GuildNameSize );
		}
		m_UserData.Write(oStream);
	}

	_TOP_DATA	m_UserData;				//��һ�������

	//��������չ����
	UINT	 m_Crime;					//��ҵ����ֵ

	CHAR	m_GuildName[MAX_GUILD_NAME_SIZE];		//�������
	BYTE	m_GuildNameSize;
};


//�������а�Ҫ��ʾ������FAVOR_TOP_LIST
struct _FAVOR_TOP_DATA
{
	_FAVOR_TOP_DATA()
	{
		memset(&m_UserData,0,sizeof(_TOP_DATA));
		m_Favor  = 0;
		m_OnLineTime = 0;
		m_GuildNameSize  = 0;
		memset(m_GuildName,0,MAX_GUILD_NAME_SIZE);

	}

	UINT	GetSize( ) const
	{
		UINT uSize = sizeof(m_Favor) +
			sizeof(m_OnLineTime)+
			sizeof(BYTE) + 
			sizeof(CHAR) * m_GuildNameSize+ 
			m_UserData.GetSize();
		return uSize;

	}

	VOID	Read( SocketInputStream& iStream )
	{
		iStream.Read( (CHAR*)&m_Favor, sizeof(m_Favor) );
		iStream.Read( (CHAR*)&m_OnLineTime, sizeof(m_OnLineTime) );
		iStream.Read( (CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize) );
		if( m_GuildNameSize > 0 && m_GuildNameSize < MAX_GUILD_NAME_SIZE )
		{
			iStream.Read(m_GuildName,m_GuildNameSize );
		}
		m_UserData.Read(iStream);
	}

	VOID	Write( SocketOutputStream& oStream ) const
	{
		oStream.Write( (CHAR*)&m_Favor, sizeof(m_Favor) );
		oStream.Write( (CHAR*)&m_OnLineTime, sizeof(m_OnLineTime) );
		oStream.Write( (CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize) );
		if( m_GuildNameSize > 0 && m_GuildNameSize < MAX_GUILD_NAME_SIZE )
		{
			oStream.Write(m_GuildName,m_GuildNameSize );
		}
		m_UserData.Write(oStream);
	}

	_TOP_DATA	m_UserData;				//��һ�������

	//��������չ����
	UINT	m_Favor;					//��ҵ�����ֵ	
	UINT	m_OnLineTime;				//���������ʱ��,��Ϊ����

	CHAR	m_GuildName[MAX_GUILD_NAME_SIZE];		//�������
	BYTE	m_GuildNameSize;

};

//�������а�Ҫ��ʾ������GUILD_TOP_LIST
struct _GUILD_TOP_DATA
{
	_GUILD_TOP_DATA()
	{
		m_GuildID = 0;
		m_GuildLevel = 0;
		m_GuildNum = 0;
		m_GuildResource = 0;
		m_GuildMoney = 0;

		m_CDRNameSize = 0;
		memset(m_CDRName,0,MAX_CHARACTER_NAME);
		m_GuildNameSize  = 0;
		memset(m_GuildName,0,MAX_GUILD_NAME_SIZE);
	}

	UINT	GetSize( ) const
	{
		UINT uSize = sizeof(m_GuildID)+
			sizeof(m_GuildLevel) +
			sizeof(m_GuildNum)+
			sizeof(m_GuildResource)+
			sizeof(m_GuildMoney)+
			sizeof(BYTE) *2 + 
			sizeof(CHAR) * m_GuildNameSize+ 
			sizeof(CHAR) * m_CDRNameSize;
		return uSize;
	}

	VOID	Read( SocketInputStream& iStream )
	{
		iStream.Read( (CHAR*)&m_GuildID, sizeof(m_GuildID) );
		iStream.Read( (CHAR*)&m_GuildLevel, sizeof(m_GuildLevel) );
		iStream.Read( (CHAR*)&m_GuildNum, sizeof(m_GuildNum) );
		iStream.Read( (CHAR*)&m_GuildResource, sizeof(m_GuildResource) );
		iStream.Read( (CHAR*)&m_GuildMoney, sizeof(m_GuildMoney) );

		iStream.Read( (CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize) );
		if( m_GuildNameSize > 0 && m_GuildNameSize < MAX_GUILD_NAME_SIZE )
		{
			iStream.Read(m_GuildName,m_GuildNameSize );
		}
		iStream.Read( (CHAR*)&m_CDRNameSize, sizeof(m_CDRNameSize) );
		if( m_CDRNameSize > 0 && m_CDRNameSize < MAX_CHARACTER_NAME )
		{
			iStream.Read(m_CDRName,m_CDRNameSize );
		}
	}

	VOID	Write( SocketOutputStream& oStream ) const
	{
		oStream.Write( (CHAR*)&m_GuildID, sizeof(m_GuildID) );
		oStream.Write( (CHAR*)&m_GuildLevel, sizeof(m_GuildLevel) );
		oStream.Write( (CHAR*)&m_GuildNum, sizeof(m_GuildNum) );
		oStream.Write( (CHAR*)&m_GuildResource, sizeof(m_GuildResource) );
		oStream.Write( (CHAR*)&m_GuildMoney, sizeof(m_GuildMoney) );

		oStream.Write( (CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize) );
		if( m_GuildNameSize > 0 && m_GuildNameSize < MAX_GUILD_NAME_SIZE )
		{
			oStream.Write(m_GuildName,m_GuildNameSize );
		}
		oStream.Write( (CHAR*)&m_CDRNameSize, sizeof(m_CDRNameSize) );
		if( m_CDRNameSize > 0 && m_CDRNameSize < MAX_CHARACTER_NAME )
		{
			oStream.Write(m_CDRName,m_CDRNameSize );
		}
	}

	//��������չ����
	UINT	m_GuildID;					//����ID
	UINT	m_GuildLevel;				//���ɵȼ�
	UINT	m_GuildNum;					//��������
	UINT	m_GuildResource;			//������Դ����
	UINT	m_GuildMoney;				//���ɽ�Ǯ

	CHAR	m_CDRName[MAX_CHARACTER_NAME];		//�᳤����
	BYTE	m_CDRNameSize;

	CHAR	m_GuildName[MAX_GUILD_NAME_SIZE];	//�������
	BYTE	m_GuildNameSize;
};


//���͸���Ϸ�������ĵȼ����а�
struct WORLD_TOP_LIST
{
	UCHAR				m_Type;
	INT					m_DataCount;	

	_LEVEL_TOP_DATA		m_LevelDataEx[TOP_USDR_NUM];	//�ȼ����а�
	_MONEY_TOP_DATA		m_MoneyDataEx[TOP_USDR_NUM];	//�Ƹ����а�
	_REPUTE_TOP_DATA	m_ReputeDataEx[TOP_USDR_NUM];	//�������а�
	_PET_TOP_DATA		m_PetDataEx[TOP_USDR_NUM];		//�������а�
	_ECTYPE_TOP_DATA	m_EctypeDataEx[TOP_USDR_NUM];	//�������а�
	_ERRANTRY_TOP_DATA	m_ErrantryDataEx[TOP_USDR_NUM];	//�������а�
	_CRIME_TOP_DATA		m_CrimeDataEx[TOP_USDR_NUM];	//������а�
	_MONEY_TOP_DATA		m_BindMoneyDataEx[TOP_USDR_NUM];//����Ϸ�����а�
	_MONEY_TOP_DATA		m_RMBDataEx[TOP_USDR_NUM];		//��������а�
	_MONEY_TOP_DATA		m_BindRMBDataEx[TOP_USDR_NUM];	//����������а�
	_FAVOR_TOP_DATA		m_FavorDataEx[TOP_USDR_NUM];	//�������а�
	_GUILD_TOP_DATA		m_GuildDataEx[TOP_USDR_NUM];	//�������а�


	VOID	CleanUp( )
	{
		m_Type = TOP_LIST_NUM;
		m_DataCount = 0;
		memset(m_LevelDataEx,0,sizeof(_LEVEL_TOP_DATA)*TOP_USDR_NUM);
		memset(m_MoneyDataEx,0,sizeof(_MONEY_TOP_DATA)*TOP_USDR_NUM);
		memset(m_ReputeDataEx,0,sizeof(_REPUTE_TOP_DATA)*TOP_USDR_NUM);
		memset(m_PetDataEx,0,sizeof(_PET_TOP_DATA)*TOP_USDR_NUM);
		memset(m_EctypeDataEx,0,sizeof(_ECTYPE_TOP_DATA)*TOP_USDR_NUM);
		memset(m_ErrantryDataEx,0,sizeof(_ERRANTRY_TOP_DATA)*TOP_USDR_NUM);
		memset(m_CrimeDataEx,0,sizeof(_CRIME_TOP_DATA)*TOP_USDR_NUM);

		memset(m_BindMoneyDataEx,0,sizeof(_MONEY_TOP_DATA)*TOP_USDR_NUM);
		memset(m_RMBDataEx,0,sizeof(_MONEY_TOP_DATA)*TOP_USDR_NUM);
		memset(m_BindRMBDataEx,0,sizeof(_MONEY_TOP_DATA)*TOP_USDR_NUM);
		memset(m_FavorDataEx,0,sizeof(_FAVOR_TOP_DATA)*TOP_USDR_NUM);
		memset(m_GuildDataEx,0,sizeof(_GUILD_TOP_DATA)*TOP_USDR_NUM);

	}

	UINT	GetSize( ) const
	{
		UINT uSize = sizeof(m_Type);
		uSize += sizeof(m_DataCount);

		switch(m_Type)
		{
		case LEVEL_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				uSize += m_LevelDataEx[i].GetSize();
			}
			break;

		case MONEY_TOP_LIST:
		case BINDMONEY_TOP_LIST:
		case RMB_TOP_LIST:
		case BINDRMB_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				uSize += m_MoneyDataEx[i].GetSize();
			}
			break;
		case REPUTE_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				uSize += m_ReputeDataEx[i].GetSize();
			}
			break;
		case PET_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				uSize += m_PetDataEx[i].GetSize();
			}
			break;
		case ECTYPE_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				uSize += m_EctypeDataEx[i].GetSize();
			}
			break;
		case ERRANTRY_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				uSize += m_ErrantryDataEx[i].GetSize();
			}
			break;
		case CRIME_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				uSize += m_CrimeDataEx[i].GetSize();
			}
			break;
		case FAVOR_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				uSize += m_FavorDataEx[i].GetSize();
			}
			break;
		case GUILD_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				uSize += m_ErrantryDataEx[i].GetSize();
			}
			break;

		default:
			Assert(FALSE);
			break;
		}

		return uSize;
	}

	VOID	Read( SocketInputStream& iStream )
	{
		iStream.Read( (CHAR*)&m_Type, sizeof(m_Type) );
		iStream.Read( (CHAR*)&m_DataCount, sizeof(m_DataCount) );

		switch(m_Type)
		{
		case LEVEL_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_LevelDataEx[i].Read(iStream);
			}
			break;

		case MONEY_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_MoneyDataEx[i].Read(iStream);
			}
			break;

		case REPUTE_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_ReputeDataEx[i].Read(iStream);
			}
			break;
		case PET_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_PetDataEx[i].Read(iStream);
			}
			break;
		case ECTYPE_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_EctypeDataEx[i].Read(iStream);
			}
			break;
		case ERRANTRY_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_ErrantryDataEx[i].Read(iStream);
			}
			break;
		case CRIME_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_CrimeDataEx[i].Read(iStream);
			}
			break;
		case BINDMONEY_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_BindMoneyDataEx[i].Read(iStream);
			}
			break;
		case RMB_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_RMBDataEx[i].Read(iStream);
			}
			break;
		case BINDRMB_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_BindRMBDataEx[i].Read(iStream);
			}
			break;
		case FAVOR_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_FavorDataEx[i].Read(iStream);
			}
			break;
		case GUILD_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_GuildDataEx[i].Read(iStream);
			}
			break;

		default:
			Assert(FALSE);
			break;
		}

	}

	VOID	Write( SocketOutputStream& oStream ) const
	{
		oStream.Write( (CHAR*)&m_Type, sizeof(m_Type) );
		oStream.Write( (CHAR*)&m_DataCount, sizeof(m_DataCount) );

		switch(m_Type)
		{
		case LEVEL_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_LevelDataEx[i].Write(oStream);
			}
			break;

		case MONEY_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_MoneyDataEx[i].Write(oStream);
			}
			break;
		case REPUTE_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_ReputeDataEx[i].Write(oStream);
			}
			break;
		case PET_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_PetDataEx[i].Write(oStream);
			}
			break;
		case ECTYPE_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_EctypeDataEx[i].Write(oStream);
			}
			break;
		case ERRANTRY_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_ErrantryDataEx[i].Write(oStream);
			}
			break;
		case CRIME_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_CrimeDataEx[i].Write(oStream);
			}
			break;
		case BINDMONEY_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_BindMoneyDataEx[i].Write(oStream);
			}
			break;
		case RMB_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_RMBDataEx[i].Write(oStream);
			}
			break;
		case BINDRMB_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_BindRMBDataEx[i].Write(oStream);
			}
			break;
		case FAVOR_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_FavorDataEx[i].Write(oStream);
			}
			break;
		case GUILD_TOP_LIST:
			for(int i=0;i<m_DataCount;i++)
			{
				m_GuildDataEx[i].Write(oStream);
			}
			break;

		default:
			Assert(FALSE);
			break;
		}
	}

};

#endif 
