#ifndef __CLASKLOGIN_H__
#define __CLASKLOGIN_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "../Version.h"


#define CURRENT_LOGIN_VERSION CURRENT_VERSION

namespace Packets
{


	class CLAskLogin : public Packet 
	{
	public:
		CLAskLogin( )
		{
			memset(szAccount,0,MAX_ACCOUNT+1);
			memset(szPassWord,0,MAX_MD5_SIZE+1);
			for(INT i = 0; i < MIBAOUNIT_NUMBER; ++i)	memset(mAllMiBaoValue[i],0,MIBAOUNIT_VALUE_LENGTH+1);
			memset(mMacAddr,0,MAX_MD5_SIZE+1);
		} ;
		virtual ~CLAskLogin( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_CL_ASKLOGIN ; }
		virtual UINT			GetPacketSize() const 
		{
			return sizeof(CHAR)*MAX_ACCOUNT+
				   sizeof(CHAR)*(MAX_MD5_SIZE)+
				   sizeof(UINT)+sizeof(CHAR)*MIBAOUNIT_NUMBER*(MIBAOUNIT_VALUE_LENGTH+1)+
				   sizeof(CHAR)*MAX_MD5_SIZE;
				

		}

	public:
		//ʹ�����ݽӿ�
		const	CHAR*			GetAccount()	const;
		VOID					SetAccount(CHAR*	pAccount);
		const	CHAR*			GetPassWord()	const;
		VOID					SetPassWord(CHAR*	pPassWord);
		UINT					GetVersion() const;
		VOID					SetVersion(UINT version);

		INT						GetPassWordLength();
		
		VOID					SetMiBaoValue(INT idx, const CHAR* pValue)
		{
			if(idx < 0 || idx >= MIBAOUNIT_NUMBER)	return;
			strncpy(mAllMiBaoValue[idx],pValue,MIBAOUNIT_VALUE_LENGTH);
		}

		const	CHAR*			GetMiBaoValue(INT idx)	const
		{
			if(idx < 0 || idx >= MIBAOUNIT_NUMBER)	return	NULL;
			return mAllMiBaoValue[idx];
		}
		CHAR*					GetMacAddr();
		VOID					SetMacAddr(CHAR*	pMacMD5);
	private:
		//����
		CHAR					szAccount[MAX_ACCOUNT+1] ;	//�û�����
		CHAR					szPassWord[MAX_MD5_SIZE+1];	//�û�����
		UINT					uVersion;					//�ͻ��˰汾
		CHAR					mAllMiBaoValue[MIBAOUNIT_NUMBER][MIBAOUNIT_VALUE_LENGTH+1];	//�ܱ�ֵ
		CHAR					mMacAddr[MAX_MD5_SIZE+1];	//�û����Ե�MAC��ַMD5
	};

	class CLAskLoginFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CLAskLogin() ; }
		PacketID_t	GetPacketID()const { return PACKET_CL_ASKLOGIN ; }
		UINT		GetPacketMaxSize() const
		{ 
			return	sizeof(CHAR)*MAX_ACCOUNT+
					sizeof(CHAR)*MAX_MD5_SIZE+
					sizeof(UINT)+sizeof(CHAR)*MIBAOUNIT_NUMBER*(MIBAOUNIT_VALUE_LENGTH+1)+
					sizeof(CHAR)*MAX_MD5_SIZE;}
	};


	class CLAskLoginHandler 
	{
	public:
		static UINT Execute( CLAskLogin* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;


#endif