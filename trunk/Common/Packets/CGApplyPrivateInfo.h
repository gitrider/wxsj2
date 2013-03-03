// CGApplyPrivateInfo.h
// 
//////////////////////////////////////////////////////

#ifndef __CG_APPLY_PRIVATEINFO_H__
#define __CG_APPLY_PRIVATEINFO_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGApplyPrivateInfo : public Packet
	{
	public:
		CGApplyPrivateInfo( )
		{
			m_InfoType		= 0;
			m_CharGUID		= 0;
			m_Age			= 0;
			m_Sex			= 0;
			m_BloodType		= 0;
			m_YearAnimal	= 0;
			m_Consella		= 0;
			m_Province		= 0;

			m_SchoolInfoLength = 0;
			memset(m_SchoolInfo, 0, MAX_SCHOOLINFO_LENGTH);

			m_JobInfoLength = 0;
			memset(m_JobInfo, 0, MAX_JOBINFO_LENGTH);

			m_CityLength = 0;
			memset(m_City, 0, MAX_CITY_LENGTH);			

			m_EmailInfoLength = 0;
			memset(m_EmailInfo, 0, MAX_EMAIL_LENGTH);	

			m_LuckWordLength = 0;
			memset(m_LuckWord, 0, MAX_LUCKWORD_LENGTH);	
		}
		virtual ~CGApplyPrivateInfo( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_APPLYPRIVATEINFO; }
		virtual UINT			GetPacketSize()const { return sizeof(BYTE)*12 + 
													 sizeof(UINT)	 +
													 sizeof(CHAR)*( m_SchoolInfoLength
																	+ m_JobInfoLength
																	+ m_CityLength
																	+ m_EmailInfoLength
																	+ m_LuckWordLength );}

	public:

		BYTE		GetInfoType()  {return m_InfoType;}
		VOID		SetInfoType(BYTE InfoType)  {m_InfoType = InfoType;}

		//UINT m_CharGUID
		UINT		GetCharGUID()  {return m_CharGUID;}
		VOID		SetCharGUID(UINT CharGUID)  {m_CharGUID = CharGUID;}

		//BYTE m_Age
		BYTE		GetAge()  {return m_Age;}
		VOID		SetAge(BYTE Age)  {m_Age = Age;}

		//BYTE m_Sex
		BYTE		GetSex()  {return m_Sex;}
		VOID		SetSex(BYTE Sex)  {m_Sex = Sex;}

		//BYTE m_BloodType
		BYTE		GetBloodType()  {return m_BloodType;}
		VOID		SetBloodType(BYTE BloodType)  {m_BloodType = BloodType;}

		//BYTE m_YearAnimal
		BYTE		GetYearAnimal()  {return m_YearAnimal;}
		VOID		SetYearAnimal(BYTE YearAnimal)  {m_YearAnimal = YearAnimal;}

		//BYTE m_Consella
		BYTE		GetConsella()  {return m_Consella;}
		VOID		SetConsella(BYTE Consella)  {m_Consella = Consella;}

		//BYTE m_Province
		BYTE		GetProvince()  {return m_Province;}
		VOID		SetProvince(BYTE Province)  {m_Province = Province;}

		//BYTE m_SchoolInfoLength;
		BYTE		GetSchoolInfoLength()  {return m_SchoolInfoLength;}
		VOID		SetSchoolInfoLength(BYTE SchoolInfoLength)  {
			m_SchoolInfoLength = (SchoolInfoLength>=MAX_SCHOOLINFO_LENGTH) ? MAX_SCHOOLINFO_LENGTH : SchoolInfoLength ;
		}
		//m_SchoolInfo
		CHAR*		GetSchoolInfo()  {return m_SchoolInfo;}
		VOID		SetSchoolInfo(CHAR* SchoolInfo)  
		{
			memcpy(m_SchoolInfo, SchoolInfo, m_SchoolInfoLength);
		}

		//BYTE m_JobInfoLength
		BYTE		GetJobInfoLength()  {return m_JobInfoLength;}
		VOID		SetJobInfoLength(BYTE JobInfoLength)  {
			m_JobInfoLength = (JobInfoLength>=MAX_JOBINFO_LENGTH) ? MAX_JOBINFO_LENGTH : JobInfoLength;
		}
		//m_JobInfo
		CHAR*		GetJobInfo()  {return m_JobInfo;}
		VOID		SetJobInfo(CHAR* JobInfo)  
		{
			memcpy(m_JobInfo, JobInfo, m_JobInfoLength);
		}

		//BYTE m_CityLength
		BYTE		GetCityLength()  {return m_CityLength;}
		VOID		SetCityLength(BYTE CityLength)  {
			m_CityLength = (CityLength>=MAX_CITY_LENGTH) ? MAX_CITY_LENGTH : CityLength;}
		//m_City
		CHAR*		GetCity()  {return m_City;}
		VOID		SetCity(CHAR* City)  
		{
			memcpy(m_City, City, m_CityLength);
		}

		//BYTE m_EmailInfoLength
		BYTE		GetEmailInfoLength()  {return m_EmailInfoLength;}
		VOID		SetEmailInfoLength(BYTE EmailInfoLength)  {
			m_EmailInfoLength = (EmailInfoLength>=MAX_EMAIL_LENGTH) ? MAX_EMAIL_LENGTH : EmailInfoLength;
		}
		//m_EmailInfo
		CHAR*		GetEmailInfo()  {return m_EmailInfo;}
		VOID		SetEmailInfo(CHAR* EmailInfo)  
		{
			memcpy(m_EmailInfo, EmailInfo, m_EmailInfoLength);
		}

		//BYTE m_LuckWordLength
		BYTE		GetLuckWordLength()  {return m_LuckWordLength;}
		VOID		SetLuckWordLength(BYTE LuckWordLength)  {
			m_LuckWordLength = (LuckWordLength>=MAX_LUCKWORD_LENGTH) ? MAX_LUCKWORD_LENGTH : LuckWordLength;}
		//m_LuckWord
		CHAR*		GetLuckWord()  {return m_LuckWord;}
		VOID		SetLuckWord(CHAR* LuckWord)  
		{
			memcpy(m_LuckWord, LuckWord, m_LuckWordLength);
		}


	private:
		BYTE		m_InfoType;							//保密程度
		UINT		m_CharGUID;							//角色编号
		BYTE		m_Age;								//年龄
		BYTE		m_Sex;								//性别
		BYTE		m_BloodType;						//血型
		BYTE		m_YearAnimal;						//生肖
		BYTE		m_Consella;							//星座
		BYTE		m_Province;							//省份

		BYTE		m_SchoolInfoLength;
		CHAR		m_SchoolInfo[MAX_SCHOOLINFO_LENGTH];//毕业院校
		
		BYTE		m_JobInfoLength;
		CHAR		m_JobInfo[MAX_JOBINFO_LENGTH];		//职业信息
		
		BYTE		m_CityLength;
		CHAR		m_City[MAX_CITY_LENGTH];			//城市
		
		BYTE		m_EmailInfoLength;
		CHAR		m_EmailInfo[MAX_EMAIL_LENGTH];		//邮件
		
		BYTE		m_LuckWordLength;
		CHAR		m_LuckWord[MAX_LUCKWORD_LENGTH];	//心情寄语

	};

	class CGApplyPrivateInfoFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGApplyPrivateInfo() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_APPLYPRIVATEINFO; };
		UINT		GetPacketMaxSize()const { return sizeof(BYTE)*12 + 
													 sizeof(UINT)	 +
													 sizeof(CHAR)*(MAX_SCHOOLINFO_LENGTH
																	+ MAX_JOBINFO_LENGTH
																	+ MAX_CITY_LENGTH
																	+ MAX_EMAIL_LENGTH
																	+ MAX_LUCKWORD_LENGTH);}
	};

	class CGApplyPrivateInfoHandler 
	{
	public:
		static UINT Execute( CGApplyPrivateInfo* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
