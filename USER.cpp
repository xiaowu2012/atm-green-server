// USER.cpp: implementation of the USER class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Server.h"
#include "USER.h"
#include "COM.h"
#include "CircularBuffer.h"
#include "BufferEx.h"
#include "Item.h"
#include "Hyper.h"
#include "Mcommon.h"
#include "Search.h"
#include "ServerDlg.h"
#include "Mcommon.h"
#include "UserManager.h"
#include "UNI_CHAR.h"
#include "RoyalRumble.h"

#include "EventZone.h"

#include "SharedQueue.h"
//#include "MarkIni.h"
#define _CHINA
#define D_WORD_XOR1MUL                          ((WORD)0x009d)
#define D_WORD_XOR3BASE                         ((WORD)0x086d)
#define D_WORD_XOR3MUL                          ((WORD)0x087b)
#define D_XORTABLE_LENGTH                       8

char G_BYTE_DOII_XorTable[D_XORTABLE_LENGTH] = { 0x32, 0x63, 0x51, 0x26, 0x51, 0x22, 0x12, 0x53 };

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "Extern.h"		//	¿ÜºÎº¯¼ö ¼±¾ð

extern CUserManager *g_pUQM;
extern CSearch *g_pUserList;
extern CServerDlg *g_pMainDlg;

// zinee 02/11/26 for Mail Agent
extern CSharedQueue		g_MailAgentQueue;

//extern CRITICAL_SECTION m_CS_ReceiveData[];
extern CRITICAL_SECTION m_CS_FileWrite;
extern CRITICAL_SECTION m_CS_EventItemLogFileWrite;
extern CPtrList RecvPtrList[];
extern long nRecvDataLength[];

// IKING 2002.1.
extern CRITICAL_SECTION m_CS_LoginData;
extern CPtrList RecvLoginData;
extern long nLoginDataCount;
extern CRITICAL_SECTION m_CS_LogoutData;
extern CPtrList RecvLogoutData;
extern long nLogoutDataCount;

extern CPtrList RecvSqlData;
extern CRITICAL_SECTION m_CS_SqlData;
extern long nSqlDataCount;	
extern int g_ChatEnable[];

extern CHATDATAPACKET *g_WaitRecvDataChat[DATA_BUF_SIZE_FOR_THREAD+1][AUTOMATA_THREAD+1];
extern int g_nChatDataHead[];
extern int g_nChatDataTail[];

extern BYTE g_ServerId;

extern TCHAR g_arServerIPAddr[16];
extern CString g_strARKRRWinner;
extern DWORD g_ThreadErr[];

//CString JunTuanName;

extern CEventZone	g_QuestEventZone;

extern long	g_PotionViewOnOff;
//
/// ¾ÏÈ£È­ °ü·Ã
// °ÔÀÓ ¸¶´Ù ´Ù¸£°Ô...
//T_KEY private_key = 0x1234567890123456;
//T_KEY private_key = 0x4584263589548694;
T_KEY private_key = 0xd9b4f1a524657845;
//¹ýÂË·Ç·¨×Ö·û
const char* theSpack[] = 
{
	"gm","GM","Gm","gM",
	"»Æ","·þ",
	"huang","Huang","hUang","huAng","huaNg","huanG",
	"fu","fU","Fu","FU","Fu","Uf",
};
////////////////////////////////////////////////////////////////////// 
//	Àü¿ªº¯¼ö
//
int g_8x[]	= {-2, -1, 0, 1, 2, 1, 0, -1};
int g_8y[]	= {0, -1, -2, -1, 0, 1, 2, 1};
int g_16x[]	= {-4, -3, -2, -1, 0, 1, 2, 3, 4, 3, 2, 1, 0, -1, -2, -3};
int g_16y[]	= {0, -1, -2, -3, -4, -3, -2, -1, 0, 1, 2, 3, 4, 3, 2, 1};

POINT g_rPt[8][6] = 
{
	{{-1, -1}, {0, -1}, {-1, 0}, {0, 0}, {100, 100}, {100, 100}},
	{{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {0, 0}, {1, 0}},
	{{0, -1}, {1, -1}, {0, 0}, {1, 0}, {100, 100}, {100, 100}},
	{{-1, -1}, {0, -1}, {-1, 0}, {0, 0}, {-1, 1}, {0, 1}},
	{{0, -1}, {1, -1}, {0, 0}, {1, 0}, {0, 1}, {1, 1}},
	{{-1, 0}, {0, 0}, {-1, 1}, {0, 1}, {100, 100}, {100, 100}},
	{{-1, 0}, {0, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}},
	{{0, 0}, {1, 0,}, {0, 1}, {1, 1}, {100, 100}, {100, 100}},
};

short		g_sHPConst[CLASS_NUM] = {6, 3, 4, 4}; // ¸÷Ö°ÒµÌåÖÊ×ªÎª¼ÓÉúÃü
short		g_sPPConst[CLASS_NUM] = {1, 5, 3, 5}; // ¸÷Ö°ÒµÖÇ»Û×ªÎª¼Ó¾«Éñ
short		g_sSPConst[CLASS_NUM] = {2, 2, 2, 2}; // ¸÷Ö°ÒµÌåÖÊ×ªÎª¼ÓÌåÁ¦

short		g_sHPLV[CLASS_NUM]	= {4, 3, 4, 3};   // ¸÷Ö°Òµ¼¶±ð×ªÎª¼ÓÉúÃü
short		g_sPPLV[CLASS_NUM]	= {1, 4, 2, 3};   // ¸÷Ö°Òµ¼¶±ð×ªÎª¼Ó¾«Éñ
short		g_sSPLV[CLASS_NUM]	= {1, 1, 1, 1};	  // ¸÷Ö°Òµ¼¶±ð×ªÎª¼ÓÌåÁ¦

short		g_sHPAdd[CLASS_NUM]	= {10, 30, 40, 35};	//¸÷Ö°Òµ¸½¼ÓÉúÃüÖµ
short		g_sPPAdd[CLASS_NUM]	= {20, 10, 20, 20};	//¸÷Ö°Òµ¸½¼Ó¾«ÉñÖµ
short		g_sSPAdd[CLASS_NUM]	= {20, 25, 35, 25};	//¸÷Ö°Òµ¸½¼ÓÌåÁ¦Öµ

int			g_iAccessoriSlot[MAX_ACCESSORI] = {2, 3, 6, 7};
int			g_iGuardianiSlot[GUARDIAN_SLOT] = {38};

extern short		g_sHPConst[CLASS_NUM];
extern short		g_sPPConst[CLASS_NUM];
extern short		g_sSPConst[CLASS_NUM];

extern short		g_sHPLV[CLASS_NUM];
extern short		g_sPPLV[CLASS_NUM];
extern short		g_sSPLV[CLASS_NUM];

extern short		g_sHPAdd[CLASS_NUM];
extern short		g_sPPAdd[CLASS_NUM];
extern short		g_sSPAdd[CLASS_NUM];

extern CRoyalRumble	g_RR;
//////////////////////////////////////////////////////////////////////
// ÇØ´ç °è¿­¿¡¼­ »ç¿ëÇÒ¼öÀÖ´Â ¹«±â°è¿­ Á¾·ù(¾ÆÀÌÅÛ Å×ÀÌºí Á¾·ù)
//
int g_BrawlClass[] = {8,9,10,11,12,13,14,15};
int g_StaffClass[] = {4,5,6,7,12,13,14,15};
int g_EdgedClass[] = {2,3,6,7,10,11,14,15};
int g_FireArmsClass[] = {1,3,5,7,9,11,13,15};

///////////////////////////////////////////////////////////////////////
//	¸ÅÁ÷ÇÏ°í ½ºÅ³ÀÌ ¼­·Î ¸ÅÄªÀÌ ¾ÈµÇ¾î¼­ ¸ÅÁ÷¼Ó¼ºÀ» ½ºÅ³±âÁØÀ¸·Î ÄÁ¹öÆ®ÇÑ´Ù.
//								°ÝÅõ		ÁöÆÎÀÌ				µµ°Ë			ÃÑ±â			  ¹«°è¿­
BYTE g_DynamicSkillInfo[MAX_SKILL_MAGIC_NUM] = { 8,7,6,9,  0,  13,12,10,11,  0,  16,15,14,17,  0,  21,19,18,20,  0,  0,0,0 };

BOOL g_bDebug = FALSE;

int  g_NpcCallNum = 0;//¼ÇÂ¼Íæ¼ÒÕÙ»½¹ÖÎïÊýÁ¿

int	g_CityRank[5][3] =		// µî±ÞÇÏ¶ô½Ã °¨¼Ò¼öÄ¡
{
	{{3200}, {2950}, {3800}},
	{{1800}, {2300}, {3800}},
	{{1100}, {1870}, {2700}},
	{{1333}, {1870}, {2600}},
	{{1333}, {1870}, {2600}},
};
							// ÊÐÃñµÈ¼¶¾­Ñé
int	g_CityRankValue[] = {-39413, -26242, -11899, -2112, -332, 0, 5000, 20000, 83000, 139000, 540000};
					//È­      ·¨      µ¶     Ç¹
int	g_ClassWeight[] = {16500,  16300,   16500,  16300};		// ¸ºÖØ

//ÎäÆ÷10¸Ä
int g_ItemAttUpgrade[] = {10000, 9500, 9000, 6000, 5000, 4000, 4000, 3000, 2000, 1000};
//×°±¸10¸Ä
int g_ItemDefUpgrade[] = {10000, 9500, 9000, 6000, 5000, 4000, 4000, 3000, 2000, 1000};
//°Ù¼¶10¸Ä
int g_ItemDefUpgrade_[] = {10000, 7000, 6000, 5000, 1060, 500, 500, 3, 2, 1};
//¸±ÊÖ10¸Ä Ð¡ºÚ¸±ÊÖ
int g_ItemFuShouUpgrade[] = {10000, 5000, 4000, 3000, 2000, 1000, 800, 10, 1};
//Ê×ÊÎ6 7 8 ¸Ä³É¹¦ÂÊ
//int g_ItemAccessoriUpgrade[] = {9600, 9500, 9400};//Ô­°æ
int g_ItemAccessoriUpgrade[] = {9600, 9500, 9400};
//Áé¾§Éý¼¶³É¹¦ÂÊ
int g_ItemBlessingUpgrade[] = {6100, 8000, 10000};
//Áé¾§Ê§°Ü¼¸ÂÊ
int g_ItemNormalDownUpgrade[] = {6000, 7000};
//»úÐµ³É¹¦ÂÊ
int g_ItemEbodyUpgrade[] = {9000, 5000, 800, 500};//{9000,5000,3000,1};//{6500,3000,400,100}40¶à¿é//{9500, 5000, 3000, 1000}ÄÑ
//ÊØ»¤³É¹¦ÂÊ
int g_ItemGuardian[] = {6000, 100};
//»ÃÊ¯Éý¼¶³É¹¦ÂÊ
int g_ItemHsUpgrade[] = {10000,10000,10000,9000,6000,3000,1000,900,100,10};
//ÆÕÍ¨»Æ½ðÊ¯Í·³É¹¦ÂÊ
int g_HuangJLUpgrade[] = {5000, 6000, 6800, 8200};//Ô½´óÔ½ÄÑ{8000, 9880, 9995, 10000};Ô­
//int g_HuangJLUpgrade[] = {7000, 8200, 9600, 9950};//Ô½´óÔ½ÄÑ{8000, 9880, 9995, 10000};Ô­
//°Ù¼¶»Æ½ðÊ¯Í·³É¹¦ÂÊ
int g_HuangJLUpgrade12[] = {9900,7600,6500,5000,4000};//Ô½Ð¡Ô½ÄÑ
/***************************************
¹¦ÄÜ:×ª´æÊý¾Ý°ü
·µ»ØÖµ: ÎÞ·µÖµ
ÈÕÆÚ: 2006Äê12ÔÂ19ÈÕ
°æ±¾:1.00
****************************************/
void SaveDump(char* lpTarget, int len)
{
	
	int i;
	
	FILE *fp = fopen("c:\\send_rev_dump.log", "a+");
	if(fp != NULL) 
	{
		for(i=0;i<len;i++){
			fprintf(fp, _T("%2.2x "), (unsigned char)lpTarget[i]);       //   ÓÃÊ®Áù½øÖÆµÄÐÎÊ½´æ´¢.   
		}
		fprintf(fp, "\n"); 
		fclose(fp);
	}
	
}
/***************************************
¹¦ÄÜ:³õÊ¼»¯KEY
·µ»ØÖµ: ÎÞ·µÖµ
ÈÕÆÚ: 2006Äê12ÔÂ19ÈÕ
°æ±¾:1.00
*****************************************/
void USER::init_encode(__int64 key2)
{
	int i;
	char *p;
	p=(char*)&key2;
	for(i=0; i<D_XORTABLE_LENGTH; i++){
    	key[i] = p[i] ^ G_BYTE_DOII_XorTable[i];
   }
}

/***************************************
¹¦ÄÜ:·â°ü¼ÓÃÜAND½âÃÜ
·µ»ØÖµ: ÎÞ·µÖµ
ÈÕÆÚ: 2006Äê12ÔÂ19ÈÕ
°æ±¾:1.00
*****************************************/
void USER::Encode_Decode(char* lpTarget, char* lpSource, int nLen,int f)
{
	BYTE	l_BYTE_Xor1 = (WORD)nLen * D_WORD_XOR1MUL, l_BYTE_Xor3;

	WORD	l_WORD_Xor3 = D_WORD_XOR3BASE, nCir;

	for (nCir=0; nCir<(WORD)nLen; nCir++) 
	{
		l_BYTE_Xor3    = HIBYTE(l_WORD_Xor3);
		lpTarget[nCir] = lpSource[nCir] ^ l_BYTE_Xor1 ^ key[nCir % D_XORTABLE_LENGTH] ^ l_BYTE_Xor3;
		l_WORD_Xor3   *= D_WORD_XOR3MUL;
	}
}

  //////////////////////////////////////////////////////////////////////////////////////////////////////
//	ÇØ´ç Å¸ÀÔ¿¡ ´ëÇÑ »óÅÂÀÌ»ó ½Ã°£ Á¤º¸¸¦ Å¬¸®¾î ÇÑ´Ù.
//
void USER::ClearAbnormalTime(DWORD dwType)
{
	DWORD dwCurrTime = GetTickCount();

	switch(dwType)
	{
	case ABNORMAL_TIME:
		m_tAbnormalKind = ABNORMAL_BYTE_NONE;
		m_dwAbnormalTime = 0;
		m_dwLastAbnormalTime = dwCurrTime;

		ClearAbnormalGroup(ABNORMAL_CLEAR_STATUS);
		break;

	case PSI_ONE_TIME:
		m_dwLastHasteTime	= dwCurrTime;			// ¸¶Áö¸·À¸·Î ÇìÀÌ½ºÆ®°¡ °É¸°½Ã°£
		m_dwLastShieldTime	= dwCurrTime;			// ¸¶Áö¸·À¸·Î ½¯µå¸¦ ¾´ ½Ã°£
		m_dwLastDexUpTime	= dwCurrTime;			// ¸¶Áö¸·À¸·Î DEX UP À» ¾´ ½Ã°£
		m_dwLastMaxHPUpTime	= dwCurrTime;			// ¸¶Áö¸·À¸·Î MAX HP UP À» ¾´ ½Ã°£

		m_dwLastFastRunTime			= dwCurrTime;	// ¸¶Áö¸·À¸·Î ÆÐ½ºÆ®·±À» ¾´ ½Ã°£
		m_dwLastMindShockTime		= dwCurrTime;	// ¸¶Áö¸·À¸·Î ¸¶ÀÎµå ¼îÅ©¸¦ ¾´ ½Ã°£
		m_dwLastPsiShieldTime		= dwCurrTime;	// ¸¶Áö¸·À¸·Î »çÀÌ¿À´Ð½¯µå¸¦ ¾´½Ã°£
		m_dwLastPiercingShieldTime = dwCurrTime;	// ¸¶Áö¸·À¸·Î ÇÇ¾î½Ì½¯µå¸¦ ¾´ ½Ã°£
		m_dwLastBigShieldTime       = dwCurrTime;

		m_dwHasteTime		= 0;					// Haste
		m_dwShieldTime		= 0;					// Shield
		m_dwDexUpTime		= 0;					// Dex Up
		m_dwMaxHPUpTime		= 0;					// Max HP Up

		m_dwFastRunTime			= 0;				// Fast Run
		m_dwMindShockTime		= 0;				// Mind Shock
		m_dwPsiShieldTime		= 0;				// Psionic Shield
		m_dwPiercingShieldTime	= 0;				// Piercing Shield
		m_dwBigShieldTime   = 0;


		
		ClearAbnormalGroup(ABNORMAL_CLEAR_PSI_ONE);
		if(m_bNecklaceOfShield) AddAbnormalInfo(ABNORMAL_SHIELD);	// ¼öÈ£ÀÇ ¸ñ°ÉÀÌ

		break;

	case PSI_TWO_TIME:
		m_dwLastAdamantineTime	= dwCurrTime;	// ¸¶Áö¸·À¸·Î AdamantineÀ» ¾´ ½Ã°£
		m_dwLastMightyWeaponTime= dwCurrTime;	// ¸¶Áö¸·À¸·Î MightyWeaponÀ» ¾´ ½Ã°£
		m_dwLastBerserkerTime	= dwCurrTime;	// ¸¶Áö¸·À¸·Î Berserker¸¦ ¾´ ½Ã°£
		
		m_dwAdamantineTime		= 0;			// Adamantine
		m_dwMightyWeaponTime	= 0;			// Mighty Weapon
		m_dwBerserkerTime		= 0;			// Berserker

		ClearAbnormalGroup(ABNORMAL_CLEAR_PSI_TWO);
		break;

	case PSI_THREE_TIME:
		m_dwLastMindGuardTime	= dwCurrTime;	// ¸¶Áö¸·À¸·Î Mind Guard ¸¦ ¾´½Ã°£
		m_dwMindGuardTime		= 0;			// Mind Guard

		ClearAbnormalGroup(ABNORMAL_CLEAR_PSI_THREE);
		break;
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction ...
//
USER::USER()
{	
	m_pCB		= new CCircularBuffer(20000);		// Socket¿¡¼­ Data¸¦ ¹Þ¾Æ¼­ ½×¾ÆµÎ´Â ¹öÆÛ
	m_pCBCore	= new CCircularBuffer(20000);		// Socket¿¡¼­ ¹ÞÀº DataÁß¿¡ À¯È¿ÇÑ ÆÐÅ¶À» °ñ¶ó³»¾î ÀúÀåÇÏ´Â ¹öÆÛ

	m_pSharedMemory = NULL;
	m_pMD			= NULL;

	// IKING 2002.1.
	m_server		= 0;
	m_dwLastDownExp	= 0;
	m_bPseudoString = FALSE;
	m_nGuildUserInfoType = 0x00;//yskang 0.2
	ZeroMemory(m_strLoveName,sizeof(m_strLoveName));

	InitializeCriticalSection(&m_CS_ExchangeItem);
	InitializeCriticalSection(&m_CS_ShopItem);
	m_presx = -1;//ÓÅ»¯´úÂë±ê¼Ç MarkJay
	m_presy = -1;
	JiaoYiMiMa[20] = 0;//½»Ò×ÃÜÂë
}

USER::~USER()
{
	int i = 0;
	EnterCriticalSection( &m_CS_ExchangeItem );
	for(i = 0; i < m_arExchangeItem.GetSize(); i++)
	{
		if( m_arExchangeItem[i] )
		{
			delete m_arExchangeItem[i];
			m_arExchangeItem[i] = NULL;
		}
	}
	m_arExchangeItem.RemoveAll();
	LeaveCriticalSection( &m_CS_ExchangeItem );

	EnterCriticalSection( &m_CS_ShopItem );
	for(i = 0; i < m_arShopItem.GetSize(); i++)
	{
		if( m_arShopItem[i] )
		{
			delete m_arShopItem[i];
			m_arShopItem[i] = NULL;
		}
	}
	m_arShopItem.RemoveAll();
	LeaveCriticalSection( &m_CS_ShopItem );

	if(m_pCB) delete m_pCB;
	if(m_pCBCore) delete m_pCBCore;

	DeleteCriticalSection(&m_CS_ExchangeItem);
	DeleteCriticalSection(&m_CS_ShopItem);
}

int USER::Init( int bufCreateFlag )
{
	CBSocket::Init(bufCreateFlag);

	m_ConnectionSuccessTick = 0;

	if ( m_Sid == -1 )
	{
		m_Sid = 0;
		m_iModSid = AUTOMATA_THREAD;
		m_uid = m_Sid;
	}
	else
	{
		m_iModSid = m_Sid % AUTOMATA_THREAD;
		m_uid = m_Sid;
	}
	
	m_isCryptionFlag = 0;
	m_Recv_value = 0;
	m_Send_value = 0;
	m_public_key = 0;
//-----------µß·åÍ¨Ñ¶---------
	SetPublicKey();
	init_encode(m_public_key);
//----------------------------
	if ( m_uid >= 0 )
		g_pUserList->SetUserUid( m_uid,this );

	m_Recv_value = 0;
	m_UserStatus = USER_JUST_CONNECTED;
	m_pCom = g_pMainDlg->GetCOM();
	m_UserFlag = FALSE;
	InitUser();
	m_ConnectionSuccessTick = GetTickCount();
	m_strZoneIP = "";
	m_nZonePort	= -1;

	return 1;
}

BOOL USER::InitUser()
{
	// Compress Init
	memset( m_CompBuf, NULL, 10240 );		// ¾ÐÃàÇÒ µ¥ÀÌÅÍ¸¦ ¸ðÀ¸´Â ¹öÆÛ
	m_iCompIndex = 0;						// ¾ÐÃàÇÒ µ¥ÀÌÅÍÀÇ ±æÀÌ
	m_CompCount = 0;						// ¾ÐÃàÇÒ µ¥ÀÌÅÍÀÇ °³¼ö
	// Memory DB Data Init
	m_pSharedMemory = NULL;
	m_pMD = NULL;
	// Speed Hack Check º¯¼ö ÃÊ±âÈ­
	m_iTickCount = 0;
	m_dwServerTickBasic = 0;
	m_dwServerTick = 0;
	m_dwClientTick = 0;
	m_ZoneIndex = 0;
	m_bWhisper	= TRUE;						// ±Ó¸» Çã¿ë
	m_bLive		= USER_DEAD;				// Live Or Dead
	m_NowZoneChanging = FALSE;				// Á¸ Ã¼ÀÎÁöÇÒ¶§ TRUE;
	m_bBuddyMode = FALSE;					// Buddy Off
//	m_bTradeMode	= FALSE;				// Trade Off
	m_bTradeWaiting = FALSE;				// Trade ¿äÃ»ÈÄ °á°ú¸¦ ±â´Ù¸®´Â Áß
	m_bNowTrading	= FALSE;				// ÇöÀç °Å·¡Áß
	m_bExchangeOk	= FALSE;				// Exchange ½Â³«
	m_bTradeItem	= FALSE;				// ItemÀ» ¿Ã·Á³õÀ½(X)
	m_iTradeUid		= -1;
	m_bPShopOpen	= FALSE;				// Personal shop open or closed
	m_bPShopView	= FALSE;				// Personal shop being viewed
	m_iPShopViewuid = -1;					// Personal shop view uid
	m_bViewingAShop	= FALSE;				// User viewing a personal shop
	m_bNoItemMove	= FALSE;
	m_bRefuse		= FALSE;				// ±âº» ¿É¼ÇÀº ¾ÆÀÌÅÛ ¹Þ±â 
	m_iMaxWeight	= 0;					// µé¼öÀÖ´Â ¹«°Ô
	m_iCurWeight	= 0;
	isGuildRen = false;
	m_dwGuild = -1;							// ±æµå ÀÎµ¦½º	
	m_sGuildVersion = -1;					// ±æµå ¹®¾ç ¹öÁ¯
	m_dwGuildLevel = -1;
	m_bGuildMaster = FALSE;					// ±æµå ¸¶½ºÅÍ
	m_bRepresentationGuild = FALSE;			// ±ÇÇÑ ´ëÇà ¿©ºÎ	 
	::ZeroMemory(m_strGuildName, sizeof(m_strGuildName)); // ±æµå ÀÌ¸§
	m_bFieldWarApply = FALSE;
	m_FieldWarGMUid = 0;					// ÇÊµåÀüÀÏ¶§ »ó´ë¹æ ±æ¸¶ uid
	m_dwFieldWar = 0;						// ÇÊµåÀü ½ÃÀÛÀÌ¸é »ó´ëÆí ±æµå ¹øÈ£°¡ µé¾î¿Â´Ù.
	::ZeroMemory(m_strChar, sizeof(m_strChar));
	::ZeroMemory(m_strTradeUser, sizeof(m_strTradeUser));
	::ZeroMemory(m_PersonalShopName, sizeof(m_PersonalShopName));
	m_sTempSTR = 0;					
	m_sTempCON = 0;					
	m_sTempDEX = 0;					
	m_sTempINT = 0;					
	m_sTempVOL = 0;					
	m_sTempWIS = 0;	
	
	m_tTempClass = 255;

	m_nHavePsiNum = 0;						// º¸À¯ »çÀÌ¿À´Ð ¼ö
	m_tDir = 0;								// À¯Àú°¡ º¸°í ÀÖ´Â ¹æÇâ
	m_tIsOP = 0;							// ¿î¿µÀÚÀÎÁö ÆÇ´Ü
	m_bLogOut = FALSE;							// Logout ÁßÀÎ°¡ ÆÇ´Ü
	m_bZoneLogOut = FALSE;

	int i = 0;										// ¹Ý°Ý½ºÅ³ Ã³¸®¸¦ À§ÇØ ÃÊ±âÈ­		
	for(i = 0; i < COUNTERATTACK_MAX_NUM; i++)
	{
		m_CounterAttackList[i].iDamage = 0;
		m_CounterAttackList[i].iNpcUid = -1;
		m_CounterAttackList[i].lSid = 0;
		m_CounterAttackList[i].dwLastAttackTime = GetTickCount();
	}

	for(i = 0; i < TOTAL_ITEM_NUM; i++) // °Å·¡Áß ÀÎº¥¿¡ ¹Ù²î´Â ¾ÆÀÌÅÛ ¼ö¸¦ Ã¼Å©À§ÇØ
	{ 
		m_TradeItemNum[i].sSid = -1; 
		m_TradeItemNum[i].sNum = 0; 
	}

	for(i = 0; i < TOTAL_ITEM_NUM; i++) // ¾ÆÀÌÅÛ ½½·Ô¸¦ ÃÊ±âÈ­
	{ 
		m_UserItem[i].sLevel = 0;			 // ÀÌ ¾ÆÀÌÅÛÀ» »ç¿ëÇÒ¼ö ÀÖ´Â ·¹º§
		m_UserItem[i].sSid = -1; 
		m_UserItem[i].sCount = 0;
		m_UserItem[i].sBullNum = 0;
		m_UserItem[i].sDuration = -1;
		m_UserItem[i].tIQ = 0;
		for(int j = 0; j < MAGIC_NUM; j++) m_UserItem[i].tMagic[j] = 0;
		m_UserItem[i].iItemSerial = 0;
	}

	m_bMakeBuddy = FALSE;			// Â¯ÀÎÁö Ã¼Å© 
	m_bNowBuddy = FALSE;			// ÇöÀç ¹öµðÁßÀÎÁö
	for(i = 0; i < MAX_BUDDY_USER_NUM; i++)
	{
		m_MyBuddy[i].uid = -1;
		::ZeroMemory(m_MyBuddy[i].m_strUserID, sizeof(m_MyBuddy[i].m_strUserID));		
	}

	for(i = 0; i < MAX_GUILD_USER; i++)
	{
		m_MyGuildUser[i].sLevel = 0;	// Á¢¼ÓÁßÀÎ ±æµå¿ø
		::ZeroMemory(m_MyGuildUser[i].GuildUser, sizeof(m_MyGuildUser[i].GuildUser));		
	}

	for(i = 0; i < MAGIC_COUNT; i++)		// ¸ÅÁ÷ ½½·ÔÀ» ÃÊ±âÈ­
	{
		m_DynamicUserData[i] = 0;
	}

	for(i = 0; i < EBODY_MAGIC_COUNT; i++)	// EBody ¸ÅÁ÷ ½½·Ô ÃÊ±âÈ­
	{	
		m_DynamicEBodyData[i] = 0;
	}

	InitTelList(m_strTeleport);
	InitEventSlot();

//	m_bLive = USER_LIVE;
	m_nCharNum = 0;

	DWORD dwCurrTime = GetTickCount();

	m_dwLastSpeedTime = dwCurrTime;
	m_dwLastMoveAndRun = dwCurrTime;
	m_dwLastAttackTime = dwCurrTime;
	m_dwLastPsiAttack = dwCurrTime;
	m_dwLastMaxAttack = dwCurrTime;
	m_dwCastDelay = 0;
	m_dwLastTimeCount = dwCurrTime;
	m_dwLastAbnormalTime = dwCurrTime;
	m_dwLastHPTime = dwCurrTime;
	m_dwLastPsiDamagedTime = 0;

	m_dwLastHasteTime = dwCurrTime;
	m_dwLastShieldTime = dwCurrTime;
	m_dwLastDexUpTime = dwCurrTime;
	m_dwLastMaxHPUpTime = dwCurrTime;
	m_dwLastBigShieldTime = dwCurrTime;

	m_dwLastFastRunTime			= dwCurrTime;
	m_dwLastMindShockTime		= dwCurrTime;
	m_dwLastMindGuardTime		= dwCurrTime;
	m_dwLastPsiShieldTime		= dwCurrTime;
	m_dwLastPiercingShieldTime	= dwCurrTime;

	m_dwLastUpdateTime = dwCurrTime;	//
	m_dwLastSaveTime   = dwCurrTime;

	m_ShouhuTime = dwCurrTime; //ÊØ»¤¼õÄÍ¾Ã
	m_dwLastItem44Time = dwCurrTime; //ÉÁÒ©Ë®Ê±¼ä
	m_JiaXueTime = dwCurrTime; //·è¿ñ¼ÓÑªÊ±¼ä
	m_FuShouTime = dwCurrTime; //ÊØ»¤¼õÄÍ¾ÃÐ¡ºÚ¸±ÊÖ
	m_UserCardTime = dwCurrTime;//ÓÃ»§³äÖµÔÚÏßÊ±¼ä
	m_VIPCardTime = dwCurrTime;//VIPÓÃ»§ÔÚÏßÊ±¼ä
	m_ShowHP = 0;//ÏÔÊ¾ÑªÁ¿
	m_GongXiFaCai = 0;//±ÜÃâÎóÉË
	m_FengWaiGua = 0;//·âÍâ¹Ò
	m_AutoChuShouFen = 0;
	m_AutoChuShouFen_KG = 0;
	m_MakeMd5 = 0;
	m_lRequireNum = 0;
	m_ionlineTime = dwCurrTime; // ×Ô¶¯ÕÇ¾­Ñé
	m_dwYBUpTime = dwCurrTime; // ÕÇÔª±¦Ê±¼ä
	m_dwJZBUpTime = dwCurrTime; // ÕÇ¾öÕ½±ÒÊ±¼ä
	m_dwEXPUpTime = dwCurrTime; // ÕÇ¾­ÑéÊ±¼ä
	m_WaiGuaTime = dwCurrTime;//¼ì²âÍâ¹ÒÊ±¼ä
	m_tAbnormalKind		= 0;
	m_dwAbnormalTime	= 0;

	m_dwHasteTime		= 0;
	m_dwShieldTime		= 0;
	m_dwDexUpTime		= 0;
	m_dwMaxHPUpTime		= 0;
	m_dwAdamantineTime	= 0;
	m_dwMightyWeaponTime= 0;
	m_dwBerserkerTime	= 0;

	m_dwFastRunTime			= 0;
	m_dwMindShockTime		= 0;
	m_dwMindGuardTime		= 0;
	m_dwPsiShieldTime		= 0;
	m_dwPiercingShieldTime	= 0;
	m_dwBigShieldTime   = 0;

	m_dwLastHiExpTime		= dwCurrTime;
	m_dwLastMagicFindTime	= dwCurrTime;
	m_dwLastNoChatTime		= dwCurrTime;
	m_dwLastBFindTime		= dwCurrTime;
	m_dwLastVIPTime			= dwCurrTime;
	m_dwLastCKMiMa			= dwCurrTime;
	m_dwLastCloseTime		= dwCurrTime;
	m_dwLastUPZaiXianTime   = dwCurrTime;
	m_dwLastUPXiuLianTime   = dwCurrTime;//Ð¡ºÚÐÞÁ¶

	m_dwLastHuanJing        = dwCurrTime;
	m_dwLastHuanLing		= dwCurrTime;

	m_dwLastFUExpTime		= dwCurrTime;//ÐÒ¸£Ê±¹â

	m_dwHiExpTime		= 0;
	m_dwMagicFindTime	= 0;	
	m_dwNoChatTime		= 0;
	m_dwBFindTime	    = 0;
	m_dwVIPTime	        = 0;
	m_dwCloseTime	    = 0;
	m_dwShopPingDN	    = 0;
	m_dwGuarDianTianShi = 0;
	m_dwJiFen	        = 0;
	m_dwChouJiang	    = 0;
	m_dwRenWu	        = 0;
	m_dwCKMiMa	        = 0;
	m_dwHuanJing	    = 0;
	m_dwHuanLing	    = 0;
	m_dwZaiXianTime	    = 0;
	m_dwPKAddkill	    = 0;
	m_dwFUExpTime		= 0;
	m_dwJiaGong	        = 0;//Ð¡ºÚÐÞÁ¶¹¥»÷
	m_dwJiaFangYu	    = 0;//Ð¡ºÚÐÞÁ¶·ÀÓù
	m_dwJiaMoFang	    = 0;//Ð¡ºÚÐÞÁ¶Ä§·À
	m_dwJiaGongJi	    = 0;  //Ð¡ºÚÐÞÁ¶
	m_dwJiaXueLiang	    = 0;  //Ð¡ºÚÐÞÁ¶
	m_dwJiaMoney	    = 0;  //Ð¡ºÚÐÞÁ¶
	m_dwUPZaiXianTime	= 60*1000;
	m_dwUPXiuLianTime	= 60 * 1000; //ÐÞÁ¶
	m_MItemLock			= FALSE;//ÎïÆ·±£»¤Ëø
	m_dwLastShoutChatTime	= dwCurrTime + 5 * 1000;//×îºóÒ»´ÎÄÅº°Ê±¼ä
	m_dwNoDamageTime		= 0;
	m_dwLastNoDamageTime	= dwCurrTime;
	m_JiaXue_Fail			= 0;
	UserCodeStr				= _T("");

	ResetJiaoBenCheck();//ÖØÖÃ½Å±¾¼ì²â MarkJay

	m_dwAbnormalInfo	= ABNORMAL_NONE;
	m_dwAbnormalInfo_	= ABNORMAL_NONE;

	m_state = STATE_DISCONNECTED;

	m_ItemFieldInfoCount = 0;
	memset( m_ItemFieldInfoBuf, NULL, 8192 );
	m_ItemFieldInfoIndex = 0;

	InitUserItem();
	InitGuildItem();			// º¯¼ö¸¦ ±ú²ýÀÌ ¼¼Å¹ÇÑ´Ù.
	InitAccountBankItem();		// º¯¼ö¸¦ ±ú²ýÀÌ ¼¼Å¹ÇÑ´Ù.

	m_bPkStatus = FALSE;		// Àá½Ãµ¿¾È Ä«¿À»óÅÂÀÎÁö¸¦ ¼³Á¤
	m_bPkKeyDown = FALSE;
	m_dwPkStatusTime = 0;		// Ä«¿À¼ºÇâÀÏ¶§ Áö¼Ó ½Ã°£¸¦ ÆÇÁ¤
	m_iCityValue = 0;			// ÇöÀç ´©Àû ¼ºÇâ°ª
	m_sCityRank = 0;			// ½Ã¹Î µî±Þ
	m_sKillCount = 0;			// PK´©Àû È½¼ö
	m_sMagicSTR = 0;				// ¸ÅÁ÷¼Ó¼º°ú E_Boady°ª¿¡ÀÇÇÑ º¯È­µÈ°ª
	m_sMagicCON = 0;
	m_sMagicDEX = 0;
	m_sMagicVOL = 0;
	m_sMagicWIS = 0;
	m_sMagicMaxHP = 0;
	m_sMagicMaxPP = 0;
	m_sMagicMaxSP = 0;
	m_Hit = 0;
	m_Avoid = 0;
	m_SKill = 0;
//	m_tPsiAbnormal = 0;

	EnterCriticalSection( &m_CS_ExchangeItem );
	for(i = 0; i < m_arExchangeItem.GetSize(); i++)
	{
		if(m_arExchangeItem[i] != NULL)
		{
			delete m_arExchangeItem[i];
			m_arExchangeItem[i] = NULL;
		}
	}
	m_arExchangeItem.RemoveAll();
	LeaveCriticalSection( &m_CS_ExchangeItem );
	
	EnterCriticalSection( &m_CS_ShopItem );
	for(i = 0; i < m_arShopItem.GetSize(); i++)
	{
		if ( m_arShopItem[i] != NULL )
		{
			delete m_arShopItem[i];
			m_arShopItem[i] = NULL;
		}
	}
	m_arShopItem.RemoveAll();

	LeaveCriticalSection( &m_CS_ShopItem );

	for( i = 0; i < m_arItemLog.GetSize(); i++ )
	{
		if( m_arItemLog[i] )
		{
			delete m_arItemLog[i];
		}
	}
	m_arItemLog.RemoveAll();

	m_dwSaintTime = 0;
	m_dwSaintTimeCount = GetTickCount();

	m_lDeadUsed = 0;

	//¼Ò¸ð¼º ¾×¼­¼­¸®¿ë º¯¼ö ÃÊ±âÈ­
	m_bRingOfLife		= 0;
	m_bNecklaceOfShield	= 0;
	m_bEarringOfProtect	= 0;
  	m_bCBOff     	    = 0;
	m_bShengRenjiezhi	= 0;
	m_bSessionOnline = false;//ÀëÏßÉÌµê¶¨Òå MarkJay
	m_UserTopNum = 0;//Íæ¼ÒÅÅÐÐÃû´Î MarkJay
	return TRUE;
}

void USER::InitEventSlot()
{
	for( int i = 0; i < m_arEventNum.GetSize(); i++)
	{
		if( m_arEventNum[i] )
		{
			delete m_arEventNum[i];
			m_arEventNum[i] = NULL;
		}
	}
	m_arEventNum.RemoveAll();
}

void USER::InitUserItem()
{
	int i, j;

	for(i = 0; i < TOTAL_ITEM_NUM; i++) // ¾ÆÀÌÅÛ ½½·Ô¸¦ ÃÊ±âÈ­
	{ 
		m_UserItem[i].sLevel = 0;	
		m_UserItem[i].sSid = -1; 
		m_UserItem[i].sCount = 0;
		m_UserItem[i].sBullNum = 0;
		m_UserItem[i].sDuration = 0;
		m_UserItem[i].tIQ = 0;
		for(j = 0; j < MAGIC_NUM; j++) m_UserItem[i].tMagic[j] = 0;
		m_UserItem[i].iItemSerial = 0;
	}

	for(i = 0; i < MAGIC_COUNT; i++)
	{
		m_DynamicUserData[i] = 0;
	}

	for(i = 0; i < EBODY_MAGIC_COUNT; i++)	// EBody ¸ÅÁ÷ ½½·Ô ÃÊ±âÈ­
	{	
		m_DynamicEBodyData[i] = 0;
	}
}

void USER::InitUserBankItem()
{
	int i, j;
	
	m_dwBankDN = 0;

	for(i = 0; i < TOTAL_BANK_ITEM_NUM; i++) // ÀºÇà ¾ÆÀÌÅÛ ½½·Ô¸¦ ÃÊ±âÈ­
	{ 
		m_UserBankItem[i].sLevel = 0; 
		m_UserBankItem[i].sSid = -1; 
		m_UserBankItem[i].sCount = 0;
		m_UserBankItem[i].sBullNum = 0;
		m_UserBankItem[i].sDuration = 0;
		m_UserBankItem[i].tIQ = 0;
		for(j = 0; j < MAGIC_NUM; j++) m_UserBankItem[i].tMagic[j] = 0;

		m_UserBankItem[i].iItemSerial = 0;
	}
}

void USER::InitAccountBankItem()
{
	int i, j;
	
	m_dwAccountBankDN = 0;

	for(i = 0; i < TOTAL_ACCOUNT_BANK_ITEM_NUM; i++) // °èÁ¤ ÀºÇà ¾ÆÀÌÅÛ ½½·Ô¸¦ ÃÊ±âÈ­
	{ 
		m_AccountBankItem[i].sLevel = 0; 
		m_AccountBankItem[i].sSid = -1; 
		m_AccountBankItem[i].sCount = 0;
		m_AccountBankItem[i].sBullNum = 0;
		m_AccountBankItem[i].sDuration = 0;
		m_AccountBankItem[i].tIQ = 0;
		for(j = 0; j < MAGIC_NUM; j++) m_AccountBankItem[i].tMagic[j] = 0;

		m_AccountBankItem[i].iItemSerial = 0;
	}
}

void USER::InitGuildItem()
{
	int i, j;
	
	m_dwGuildDN = 0;

	for(i = 0; i < TOTAL_BANK_ITEM_NUM; i++) // ÀºÇà ¾ÆÀÌÅÛ ½½·Ô¸¦ ÃÊ±âÈ­
	{ 
		m_GuildItem[i].sLevel = 0; 
		m_GuildItem[i].sSid = -1; 
		m_GuildItem[i].sCount = 0;
		m_GuildItem[i].sBullNum = 0;
		m_GuildItem[i].sDuration = 0;
		m_GuildItem[i].tIQ = 0;
		for(j = 0; j < MAGIC_NUM; j++) m_GuildItem[i].tMagic[j] = 0;

		m_GuildItem[i].iItemSerial = 0;
	}
}

//////////////////////////////////////////////////////////////////////
//	Inline Fn...
//

USER* USER::GetUser(int uid)
{
	if( uid < 0 || uid + USER_BAND >= NPC_BAND ) return NULL;

	//if(uid >= MAX_USER)
	//{
	//	TRACE("######### USER::GetUser() : Invalid UID = %d\n" , uid);
	//	return NULL;
	//}

	return m_pCom->GetUserUid(uid);
}

USER* USER::GetUser(TCHAR* id)
{
	if(!strlen(id)) return NULL;
	CString szSource = id;
	USER *pUser = NULL;

	for( int i = 0; i < MAX_USER; i++)
	{
		pUser = m_pCom->GetUserUid(i);
		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) continue;
		if(pUser->m_state == STATE_DISCONNECTED || pUser->m_state == STATE_LOGOUT ) continue;
		if ( !szSource.CompareNoCase(pUser->m_strUserID) ) return pUser;
	}
	return NULL;
}
/*
inline int USER::GetUid(int x, int y )
{
	MAP* pMap = g_zone[m_ZoneIndex];
	return pMap->m_pMap[x][y].m_lUser;
}

inline BOOL USER::SetUid(int x, int y, int id)
{
	MAP* pMap = g_zone[m_ZoneIndex];

	if(!pMap) return FALSE;
	if(pMap->m_pMap[x][y].m_bMove != 0) return FALSE;
	if(pMap->m_pMap[x][y].m_lUser != 0 && pMap->m_pMap[x][y].m_lUser != id ) return FALSE;
/*
	long lUid = m_uid + USER_BAND;
	long temp = pMap->m_pMap[x][y].m_lUser;

	if(InterlockedCompareExchange(&pMap->m_pMap[x][y].m_lUser, (LONG)0, lUid) == lUid)
	{
		long tt = pMap->m_pMap[x][y].m_lUser;
		return TRUE;
	}
*/
/*	pMap->m_pMap[x][y].m_lUser = id;

	return TRUE;
}
*/
//-------------------------------------------------------------------------------//

CNpc* USER::GetNpc(int nid)
{
	if( nid < 0 || nid >= g_arNpc.GetSize() ) return NULL;

	return g_arNpc[nid];
}

CNpc* USER::GetNpc(TCHAR* pNpcName)
{
	CNpc* pNpc = NULL;

	int nSize = g_arNpc.GetSize();

	for( int i = 0; i < g_arNpc.GetSize(); i++)
	{
		pNpc = g_arNpc[i];
		if( !pNpc ) continue;

		if( _tcscmp(pNpc->m_strName, pNpcName) == 0)
		{
			return pNpc;
		}
	}

	return NULL;
}

/////////////////////////////////////////////////////////////////////
//	Communication Part
int USER::NoEncodeSend(int length, char *pBuf)
{
	if ( m_SockFlag != 1 ) return 0;

	int rv = CBSocket::B_Send( length, pBuf );

	if ( m_SockFlag == 0 || rv == -2 )
	{
		//SockCloseProcess();
		return rv;
	}

	return length;
}

void USER::Send(TCHAR *pBuf, int nLength)
{
	char pTBuf[RECEIVE_BUF_SIZE+1];

	if ( m_SockFlag != 1 ) return;

	if ( nLength > PACKET_DATA_SIZE ) return;

	if(nLength >= SEND_BUF_SIZE - 6)
	{
		TRACE("#### Too Big Send Data!!! (%s, %d, size = %d)\n", __FILE__, __LINE__, nLength);
		return;
	}

//	::ZeroMemory(pTBuf, SEND_BUF_SIZE);
	TCHAR tibuf[SEND_BUF_SIZE+1];
	::ZeroMemory(pTBuf, SEND_BUF_SIZE);
	::ZeroMemory(tibuf, SEND_BUF_SIZE);

	int index = 0;
	MYSHORT slen;

	pTBuf[index++] = (char) PACKET_START1;		// ÆÐÅ¶ ½ÃÀÛÀ» ¾Ë¸®´Â 2 Byte
	pTBuf[index++] = (char) PACKET_START2;

//	slen.i = nLength;
	if ( m_isCryptionFlag == 1 )
	{
		m_Send_value++;							// ÆÐÅ¶¿¡ ÀÏ·Ã ¹øÈ£¸¦ ºÎ¿©ÇÑ´Ù.
		DWORD nSendOrder = m_Send_value;
		m_Send_value &= 0x00ffffff;
		nSendOrder &= 0x00ffffff;

		memcpy( tibuf, &nSendOrder, sizeof(DWORD) );
		memcpy( &tibuf[4], pBuf, nLength );
		nLength += sizeof(DWORD);

		slen.i = nLength + 1;						//¾ÏÈ£È­¿©ºÎ¸¦ ³ªÅ¸³»´Â Ä¿¸Çµå Æ÷ÇÔ
	}
	else slen.i = nLength;

	pTBuf[index++] = (char) (slen.b[0]);		// ÆÐÅ¶ ±æÀÌ¸¦ Short(2Byte)·Î ½Ç¾î º¸³½´Ù
	pTBuf[index++] = (char) (slen.b[1]);		// 2Byte¶ó°í ÇÏ¿© 1Byte * 256 + 2Byte°¡ ¾Æ´Ô

//	memcpy(pTBuf + index, pBuf, nLength);
	// ¾ÏÈ£È­...
	if ( m_isCryptionFlag == 1 )
	{
		pTBuf[index++] = ENCRYPTION_PKT;
		memcpy(pTBuf + index, tibuf, nLength);
#ifdef _CHINA
		Encode_Decode((char *)pTBuf + index, (char *)tibuf, nLength,1);
#else
		m_cryptfn.JvEncryptionFast( nLength, (BYTE*)tibuf, (BYTE*)pTBuf + index );
#endif
	}
	else
	{
		memcpy(pTBuf + index, pBuf, nLength);
	}
	
	index += nLength;

	pTBuf[index++] = (char) PACKET_END1;		// ÆÐÅ¶ ³¡À» ¾Ë¸®´Â 2 Byte
	pTBuf[index++] = (char) PACKET_END2;


	NoEncodeSend(index, pTBuf);

}

void USER::ProcessData(char *pIBuf, int length)//ÖØµã
{
	if ( m_UserStatus == 0 && m_UserFlag == FALSE ) return;

	if(!length) return;

	int len = 0;
	BOOL bEncryption = FALSE;
	
	if(length > 5000)//ÐÞ¸´±¨´í MarkJay
	{
		SYSTEMTIME st;
		CString strDate;
		GetLocalTime(&st);
		strDate.Format("%d-%d-%d %d:%d",st.wYear,st.wMonth,st.wDay ,st.wHour,st.wMinute);
		CString str = _T("");
		str.Format("%s [%s]len = %d Òì³£´¦Àí\r\n",strDate,m_strUserID,length);
		EnterCriticalSection( &m_CS_FileWrite );
		g_fpMarkDeBug.Write( str, str.GetLength() );
		LeaveCriticalSection( &m_CS_FileWrite);
		SoftClose();
		return;
	}

	m_pCB->PutData(pIBuf, length);		// ¹ÞÀº Data¸¦ ¹öÆÛ¿¡ ³Ö´Â´Ù

	if(m_pCB->GetValidCount() > 5000)//ÐÞ¸´±¨´í MarkJay
	{
		SYSTEMTIME st;
		CString strDate;
		GetLocalTime(&st);
		strDate.Format("%d-%d-%d %d:%d",st.wYear,st.wMonth,st.wDay ,st.wHour,st.wMinute);
		CString str = _T("");
		str.Format("%s [%s]Count = %d Òì³£´¦Àí\r\n",strDate,m_strUserID,m_pCB->GetValidCount());
		EnterCriticalSection( &m_CS_FileWrite );
		g_fpMarkDeBug.Write( str, str.GetLength() );
		LeaveCriticalSection( &m_CS_FileWrite);
		SoftClose();
		return;
	}

	if(m_pCB->GetValidCount() > 7)
	{
		BYTE Buff[7];
		m_pCB->GetData((char *)Buff,7);
		if(Buff[0] != PACKET_START1 || Buff[1] != PACKET_START2)
		{
			SoftClose();
			return;
		}
		int iPLen = Buff[3]*256+Buff[2]; 
		if(iPLen > 1024)
		{
			SoftClose();
			return;
		}
	}

	while(PullOutCore(len, bEncryption))
	{
		if( len <= 0 ) break;

		Parse( len, m_RData, bEncryption );

		bEncryption = FALSE;
	}
}

BOOL USER::PullOutCore(int &length, BOOL &bEncryption)
{
	BYTE		*pTmp;
	int			len = 0, sPos = 0, ePos = 0;
	BOOL		foundCore = FALSE;
	MYSHORT		slen;

	len = m_pCB->GetValidCount();

	if(len <= 0 || len >= RECEIVE_BUF_SIZE) return FALSE;

	pTmp = new BYTE[len];
	m_pCB->GetData((char*)pTmp, len);

	for (int i = 0; i < len && !foundCore; i++)
	{
		if (i + 2 >= len) break;

		if (pTmp[i] == PACKET_START1 && pTmp[i + 1] == PACKET_START2)
		{
			sPos = i + 2;

			slen.b[0] = pTmp[sPos];
			slen.b[1] = pTmp[sPos + 1];

			length = (int)slen.i;

			if (length <= 0) goto cancelRoutine;
			if (length > len || length > CORE_BUFF_SIZE) goto cancelRoutine;

			if((BYTE)pTmp[sPos + 2] == ENCRYPTION_PKT)
			{
				bEncryption = TRUE;
			}

			ePos = sPos + length + 2;
			if((ePos + 2) > len ) goto cancelRoutine;

			if (pTmp[ePos] == PACKET_END1 && pTmp[ePos+1] == PACKET_END2)
			{
				::ZeroMemory(m_RData, CORE_BUFF_SIZE+1);

				if(bEncryption) 
				{
#ifdef _CHINA
					Encode_Decode((char *)m_RData, (char *)(pTmp+sPos+3), length -1,0);
#else
					m_cryptfn.JvDecryptionFast( length -1, (BYTE *)(pTmp+sPos+3), (BYTE *)m_RData );
#endif
				}
				else				::CopyMemory((void *)m_RData, (const void *)(pTmp+sPos+2), length);

				m_RData[length] = 0;

				foundCore = TRUE;
				break;
			}
			else
			{
				m_pCB->HeadIncrease(3);
				break;
			}
		}
	}
	if (foundCore) m_pCB->HeadIncrease(6 + length); //6: header 2+ end 2+ length 2

	delete[] pTmp;

	return foundCore;

cancelRoutine:
	if(pTmp) delete[] pTmp;
	return foundCore;
}
//////////////////////////////////////////////////////////////////////////////
// ¿Í»§¶Ë·¢ËÍ¸ø·þÎñ¶ËµÄ´¦Àíº¯Êý ·Ç³£ÖØÒª
void USER::Parse(int len, TCHAR *pBuf, BOOL &bEncryption)
{
	int index = 0;
	DWORD tempVer = 0;
	DWORD dwValue = 0;
	CString str = _T("");

	BYTE command = 0;

	if(bEncryption)
	{
		dwValue = GetDWORD(m_RData + index, index);

		tempVer = dwValue - m_Recv_value;

		if(tempVer > 100)
		{
			return;		// ÆÐÅ¶ ¾ÏÈ£È­ ÀÓ½Ã Å×½ºÆ®¿ëÀÓ
		}

		m_Recv_value = dwValue;
	}

	command = GetByte(m_RData, index);
	if(command == ENCRYPTION_START_REQ) EncryptionStartReq(pBuf + index);//²âÊÔÐÞ¸´µô¶Ë

	UserTimer();

	if( !bEncryption ) return;		// ¾ÏÈ£È­°¡ ¾ÈµÇ¾î ÀÖÀ¸¸é ¹«½Ã..	

	g_ThreadErr[m_iModSid] = command;

	if(command != CLIENT_TICK) 
	{
		m_dwNoDamageTime = 0;
		m_dwLastNoDamageTime = GetTickCount();
	}

	// Before Game Start...
	switch( command )
	{
	case NEW_CHAR_REQ:
		//NewCharReq(pBuf + index);
		break;

	case CHECK_ID_REQ:
		//CheckIDReq(pBuf + index);
		break;

	case REROLL_REQ:
		//RerollReq(pBuf + index);
		break;

	case DELETE_CHAR_REQ:
		//DeleteCharReq(pBuf + index);
//		DeleteCharReqWithThread(pBuf + index);
		break;

	case GAME_START_REQ:
		GameStart(pBuf + index);
		break;

	case ZONE_LOGIN_REQ:
		//ZoneLoginReq(pBuf + index);
		break;

	//ÍË³öÓÎÏ·ÇëÇó
    case LOGOUT_REQ:
		//if(IsCity() && m_bNowTrading == FALSE && m_bNoItemMove == FALSE && m_bViewingAShop == FALSE)
		//{
		//	if(m_bNowBuddy == TRUE)//×Ô¶¯ÍË³ö×é¶Ó
		//	{
		//		for(int i = 0; i < MAX_BUDDY_USER_NUM; i++)
		//		{
		//			if(m_MyBuddy[i].uid == m_uid + USER_BAND) SendBuddyUserLeave(i);
		//		}
		//	}
  //          m_bSessionOnline = true;
		//	UpdateUserData();
		//}
		//else
		//{
			SoftClose();
		//}
		break;

	case CLIENT_TICK:
		if ( g_ChatEnable[m_iModSid] == TRUE ) CheckSpeedHack();
		break;

//	case CHECK_ALIVE_REQ:			//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ Å×½ºÆ® ÄÚµåÀÓ
//		CheckServerTest();
//		break;
//	case ANNOUNCE_NOTICE:
//		AnnounceNotice(pBuf + index);
//		break;
	default:
		break;
	}

	if( g_bShutDown )
	{
		SoftClose();
		return;
	}

	// Game Start...
	switch( command )
	{
	case CHAT_REQ:
//		if ( g_ChatEnable[m_iModSid] == TRUE )	ChatReq(pBuf + index);
		if ( g_ChatEnable[m_iModSid] == TRUE ) ChatReqWithThread(len-1, pBuf + index);
		break;

	case LIVE_REQ:
		if(m_bPShopOpen == TRUE) return;	
		LiveReq(pBuf + index);
		break;

	case REVIVAL_REQ://ÉñÖ®Àá..¸´»î
		if(m_bPShopOpen == TRUE) return;
		RevivalReq( pBuf + index );
		break;

	case USERINFO_REQ:
		if(m_bPShopOpen == TRUE) return;
		UserInfoReq(pBuf + index);
		break;

	//case SYSTEM_MSG://ÓÎÏ·ÐÅÏ¢ MarkJay
	//	break;

	case CLIENT_TIMER:					// ÀÏ´Ü SP¸¦ È¸º¹ÇÑ´Ù.
//		UserTimer();
		break;

	case ITEM_TRADE_REQ:
		if(m_bPShopOpen == TRUE) return;
		ItemTradeReq(pBuf + index);
		break;

	case EXCHANGE_REQ:
		if(m_bPShopOpen == TRUE) return;
		ExchangeReq(pBuf + index);
		break;

	case RESTART_REQ:
		RestartReq(pBuf + index);
		break;

	case HELP_REQ:
		HelpStatusOpenReq();
		break;

	case SCREEN_CAPTURE_REQ:
		ScreenCaptureReq( );
		break;

	case EXPRESSION_REQ:
		if(m_bPShopOpen == TRUE) return;
		ExpressionReq( pBuf + index );
		break;

	case EXPRESSION_OPEN:
		if(m_bPShopOpen == TRUE) return;
		ExpressionOpen( );
		break;

	case PERSONAL_SHOP_REQ:
		if(m_bNowTrading == TRUE) return;
		if(m_bNoItemMove == TRUE) return;
		PersonalShopReq( pBuf + index );
		break;


//	case SERVER_TICK_REQ:
//		SendServerTick();
//		break;

	default:
		break;
	}

	if(m_bLive == USER_DEAD) return;		// ¾Æ·¡ÀÇ ÆÐÅ¶Àº À¯Àú°¡ »ì¾ÆÀÖÀ» °æ¿ì¸¸ Ã³¸®
	if(m_bNowTrading == TRUE) return;		// Trade Áß¿¡´Â µýÁþÀ» ¸øÇÑ´Ù.
	if(m_bNoItemMove == TRUE) return;		// ¾ÆÀÌÅÛÀ» ¿òÁ÷À» ¼ö ¾ø´Â »óÅÂÀÌ¸é ¸®ÅÏ
	if(m_bPShopOpen == TRUE)//ÀëÏßÉÌµêÌßÈË MarkJay
	{
		return;
	}
	if(m_bViewingAShop == TRUE) return;		// A User is viewing another users shop
	if(m_state != STATE_GAMESTARTED) return;
	if( m_bZoneLogOut ) return;

	switch( command )
	{
	case CLIENT_EVENT:
		ClientEvent(pBuf+index);
		break;

	case BANK_ITEM_MOVE_REQ:
		BankItemMoveReq(pBuf + index);
		break;

	case BUY_ITEM:
		BuyItem(pBuf + index);
		break;

	case BUY_PSY_REQ:
		BuyPsi(pBuf + index);
		break;

	case SELL_ITEM:
		SellItem(pBuf + index);
		break;

	case REPAIR_ITEM:
		RepairItem(pBuf + index);
		break;

	case MOVE_CHAT_REQ:
		MoveChatReq(pBuf + index);
		break;

	case CHANGE_DIR_REQ:
		ChangeDir(pBuf + index);
		break;

	case MOVE_FIRST_REQ:
		MoveFirstReq(pBuf + index);
		break;
	case MOVE_REQ:
		MoveReq(pBuf + index);
		break;
	case MOVE_END_REQ:
		MoveEndReq(pBuf + index);
		break;

	case RUN_FIRST_REQ:
		RunFirstReq(pBuf + index);
		break;
	case RUN_REQ:
		RunReq(pBuf + index);
		break;
	case RUN_END_REQ:
		RunEndReq(pBuf + index);
		break;
	
	case FIND_USER_REQ:
		FindUserReq(pBuf + index);
		break;

	case BBS_NEXT:
		BBSNext(pBuf + index);
		break;

	case BBS_READ:
		BBSRead(pBuf + index);
		break;

	case BBS_WRITE:
		//BBSWrite(pBuf + index);
		break;

	case BBS_EDIT:
		//BBSEdit(pBuf + index);
		break;

	case BBS_DELETE:
		//BBSDelete(pBuf + index);
		break;

	case PSI_ATTACK_REQ:
		if(m_dwCloseTime > 0) return; //×øÀÎ²»ÄÜÊ¹ÓÃÄ§·¨
		else if(m_tAbnormalKind == ABNORMAL_BYTE_CONFUSION) return;//Ñ£ÔÎ×´Ì¬²»ÄÜÄ§·¨¹¥»÷
		PsiAttack(pBuf + index); 
		break;

	case ATTACK_REQ:
		if(m_dwCloseTime > 0) return; //×øÀÎ²»ÄÜ¹¥»÷
		if(m_tAbnormalKind == ABNORMAL_BYTE_CONFUSION) return;//Ñ£ÔÎ×´Ì¬²»ÄÜ¹¥»÷
        Attack(pBuf + index);
		break;

	case TRADE_MODE_REQ:
		SetTradeMode(pBuf + index);
		break;

	case ITEM_MOVE_REQ: //ÒÆ¶¯×°±¸»òÕßÎïÆ· ´©µÃ×°±¸µÈ.
		ItemMoveReq(pBuf + index);
		break;

	case ITEM_GIVE_REQ:
		ItemGiveReq(pBuf + index);
		break;

	case ITEM_LOAD_REQ:
		ItemLoadReq(pBuf + index);
		break;

	case ITEM_USE_REQ:
		ItemUseReq(pBuf + index);
		break;

	case CHANGE_WEAPON_REQ:
		ChangeWeaponReq(pBuf + index);
		break;

	case SKILL_ONOFF_REQ:
		SkillOnOffReq(pBuf + index);
		break;

	case PSI_CHANGE_REQ:
		PsiChangeReq(pBuf + index);
		break;

	case USER_STATUS_SKILL_UP_REQ:
		UserStatusSkillUp(pBuf + index);
		break;

	case BUDDY_MODE_REQ: 
		SetBuddyMode(pBuf + index);
		break;

	case UPGRADE_ITEM_REQ://ÎïÆ·Éý¼¶
		if(m_dwKanZBZT == 1)
		{
			SendSystemMsg("²é¿´×°±¸×´Ì¬ÏÂÎïÆ·²»ÄÜÉý¼¶.", SYSTEM_ERROR, TO_ME);
			return;
		}
		if(!m_MItemLock && f_Markini[0]->JYMM_KG == 1)
		{
			SendSystemMsg("¶Ô²»Æð!½»Ò×ÃÜÂëÉÐÎ´½â³ý,ÎïÆ·²»ÄÜÉý¼¶.", SYSTEM_ERROR, TO_ME);
			return;
		}
		UpgradeItemReqBegin(pBuf + index);
		break;

	case GUILD_OPEN_REQ:
		NewGuild(pBuf + index);
//		NewGuildWithThread(pBuf + index);
		break;

	case GUILD_WAREHOUSE_CLOSE:
		GuildWharehouseCloseReq();
		break;

	case GUILD_ADD_REQ:
		GuildAddReq(pBuf + index);
		break;

	case GUILD_MARK_ADD_REQ:
		GuildMarkAddReq(pBuf + index);
//		GuildMarkAddReqWithThread(pBuf + index);
		break;

	case GUILD_ITEM_MOVE_REQ:
		GuildItemMoveReq(pBuf + index);
//		GuildItemMoveReqWithThread(pBuf + index);
		break;

	case GUILD_USER_INFO_REQ:
		GuildUserInfoReq(pBuf + index);
		break;

	case GUILD_TAX_REQ:
		GetStoreTax(pBuf + index);
		break;

	case GUILD_WAR_REQ:
		GetGuildWarScheme(pBuf + index);
		break;

	case GUILD_WAR_APPLY_REQ:
		GuildWarApplyReq(pBuf + index);
		break;

	case GUILD_FIELD_WAR_RESULT:
		//SendGuildWarFieldApplyResult(pBuf + index);
		break;

	case GUILD_HOUSE_APPLY_REQ:
		MassZoneMove(pBuf + index);
		break;

	case TELEPORT_REQ:
		TeleportReq();
		break;

	case TELEPORT_EDIT_REQ: 
		TelportEdit(pBuf + index);
		break;

	//case TELEPORT_COPY_REQ:
	//	TeleportCopyReq(pBuf + index);
	//	break;

	//case TELEPORT_COPY_RESULT:
	//	TeleportCopyResult(pBuf + index);
	//	break;

	case EVENT_UPGRADE_ITEM_REQ:
		//EventUpgradeItemReq(pBuf + index);
		break;

	case ADDRESS_WINDOW_OPEN:
	case ADDRESS_WINDOW_OPEN_NEW:
		//RecvAddress( pBuf + index );
		break;

	// Ðû´«Íæ¼ÒCDKEY MarkJay
	case PRE_REG_USER_EVENT_REQ:
		RecvEventItemSerial( pBuf + index );
		break;

	case PRE_REG_GAMEROOM_EVENT_REQ:
		RecvEventItemSerialForGameRoom( pBuf + index );
		break;

	case ACCOUNT_BANK_ITEM_MOVE_REQ:
		AccountBankItemMoveReq( pBuf + index );
		break;

	case DRESSING:
		//m_bNoItemMove = TRUE;
		//DressingReq( pBuf + index );
		//m_bNoItemMove = FALSE;
		break;

	case SELECT_OVER100_SKILL:
		SelectOver100Skill( pBuf+index );
		break;

	case GUILD_INVITE_REQ:
		SendGuildInviteReq( pBuf+index );
		break;

	case GUILD_INVITE_RESULT:
		GuildInviteResult( pBuf+index );
		break;

	case QUESTWINDOW_OPEN_REQ:
		//QuestWindowOpenReq();
		if(m_dwGuild == -1)
		{
			QuestWindowOpenReq();
		}
		else
		{
			m_nGuildUserInfoType = 0x03;
			GuildUserInfoReqzh();
		}
		break;

	case LOVE_NAME_REQ://--yskang 0.1
		LoveName(pBuf+index);
		break;

	// Mail system by zinee 02-11-15
	case MAIL_INFO_REQ:
		MailInfoReq( pBuf+index );
		break;

	case MAIL_SEND_REQ:
		MailSendReq( pBuf+index );
		break;

	case MAIL_LIST_REQ:
		MailListReq( pBuf+index );
		break;

	case MAIL_CONTENTS_REQ:
		MailContentReq( pBuf+index );
		break;

	case MAIL_DELETE_REQ:
		MailDeleteReq( pBuf+index );
		break;

	case EBODY_IDENTIFY_REQ:
		EBodyIdentifyReq( pBuf + index );
		break;

	case PK_BUTTON_REQ:
		TogglePKButton( pBuf + index );
		break;

	case TRANSFORM_REQ:
		TransformReq( pBuf + index  );
		break;
	case 68://»¯×±±äÉí
		Face_painting(pBuf + index,len);
		break;
	case 30://µç¹â°å
		DianGuangChat(pBuf + index,len);
		break;
	case 251: //²é¿´×°±¸
		if(!m_MItemLock && f_Markini[0]->JYMM_KG == 1)
		{
			SendSystemMsg("¶Ô²»Æð!½»Ò×ÃÜÂëÉÐÎ´½â³ý,ÎÞ·¨²é¿´.", SYSTEM_ERROR, TO_ME);
			return;
		}
		ViewUserItem(pBuf + index);
		break;
	default:
		break;
	}
}
//////////////////áÛ·å²é¿´×°±¸¹¦ÄÜ Ð¡ºÚ2016-11-1//////////////////
void USER::ViewUserItem(TCHAR *pBuf)
{
	int index = 0;
	USER *pUser = NULL;
    char szUserName[255];
	index += 5;
	int nLength = GetVarString(sizeof(szUserName), szUserName, pBuf, index);
	if(nLength <= 0 || nLength > CHAR_NAME_LENGTH) 
	{
		return;
	}
	pUser = GetUser(szUserName);
	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;
	if(strcmp(pUser->m_strUserID, m_strUserID) == 0) return;
	if(pUser->m_tIsOP == 1 || pUser->m_tIsOP == 88 || pUser->m_tIsOP == 89) return;

	if(m_dwKanZBZT != 1)
	{
		CBufferEx TempBuf;
		TempBuf.Add(USE_POTION);
		TempBuf.Add(m_uid + USER_BAND);
		TempBuf.Add((BYTE)16);
		Send( TempBuf, TempBuf.GetLength() );
		SendCharDataToOPUser(pUser);
		m_dwKanZBZT = 1;
		CString strMsg1 = _T("");
		strMsg1.Format( "ÕýÔÚ²é¿´ %s µÄ×°±¸,ÍË³öÇë°´TAB¼ü.", pUser->m_strUserID);
		SendSystemMsg( (LPTSTR)(LPCTSTR)strMsg1, SYSTEM_ERROR, TO_ME);
	}
}
//////////////////////////////////////////////////////////////////////
// »¯×±±äÉí
void USER::Face_painting(TCHAR *pBuf,int len)
{
	int index = 0,i;
	byte sGender;
	int slot = GetShort(pBuf, index);//Ê¹ÓÃÎïÆ·Î»ÖÃ
	if(m_UserItem[slot].sSid != 1000 && m_UserItem[slot].sSid != 1004 && m_UserItem[slot].sSid != 1018) return;
	for(i = 0;i < 10;i++)
	{
		m_strFace[i] = GetByte(pBuf, index);
	}
	sGender = GetByte(pBuf, index);
	if(sGender == 1)
	{
		m_sGender = m_sGender + 1;
		if(m_sGender > 1) m_sGender = 0;
	}
	
	SendMyInfo(TO_INSIGHT, INFO_MODIFY);

	CBufferEx	TempBuf;
	TempBuf.Add((byte)68);
	Send(TempBuf, TempBuf.GetLength());
	CBufferEx	TempBuf1;
	ReSetItemSlot(&m_UserItem[slot]);
	TempBuf1.Add(ITEM_GIVE_RESULT);
	TempBuf1.Add((BYTE)0x1);
	TempBuf1.Add((BYTE)slot);
	TempBuf1.Add(m_UserItem[slot].sLevel);
	TempBuf1.Add(m_UserItem[slot].sSid);
	TempBuf1.Add(m_UserItem[slot].sDuration);
	TempBuf1.Add(m_UserItem[slot].sBullNum);
	TempBuf1.Add(m_UserItem[slot].sCount);
	for(int j = 0; j < MAGIC_NUM; j++) TempBuf1.Add(m_UserItem[slot].tMagic[j]);
	TempBuf1.Add(m_UserItem[slot].tIQ);
	Send(TempBuf1, TempBuf1.GetLength());
}
//////////////////////////////////////////////////////////////////////
// µç¹â°åËµ»°
void USER::DianGuangChat(TCHAR *pBuf,int len)
{
	CBufferEx TempBuf,TempBuf1;
	int index = 1;
	int slot = GetShort(pBuf, index); //Ê¹ÓÃÎïÆ·Î»ÖÃ
	if(slot < 10 || slot >= 34) return;
	if(m_UserItem[slot].sSid != 1021 || m_UserItem[slot].sDuration <= 0) return;
	if(m_dwCloseTime > 0)
	{
		SendSystemMsg( "¶Ô²»Æð,¼àÓüÖÐ²»ÔÊÐí·¢ÑÔ.", SYSTEM_ERROR, TO_ME);
		return;
	}
	SendDGDuration(1021,slot);
	if(m_UserItem[slot].sDuration <= 0)
	{
		ReSetItemSlot(&m_UserItem[slot]);
		TempBuf1.Add(ITEM_GIVE_RESULT);
		TempBuf1.Add((BYTE)0x1);
		TempBuf1.Add((BYTE)slot);
		TempBuf1.Add(m_UserItem[slot].sLevel);
		TempBuf1.Add(m_UserItem[slot].sSid);
		TempBuf1.Add(m_UserItem[slot].sDuration);
		TempBuf1.Add(m_UserItem[slot].sBullNum);
		TempBuf1.Add(m_UserItem[slot].sCount);
		for(int j = 0; j < MAGIC_NUM; j++) TempBuf1.Add(m_UserItem[slot].tMagic[j]);
		TempBuf1.Add(m_UserItem[slot].tIQ);
		Send(TempBuf1, TempBuf1.GetLength());
	}
	TempBuf.Add((byte)0x1f);
	TempBuf.Add((byte)1);
	TempBuf.Add((byte)0x25);
	TempBuf.Add((DWORD)0);
	TempBuf.AddString(m_strUserID);
	TempBuf.AddString(pBuf+4);

	int iNum = m_pCom->Add_DGB_Massage(TempBuf,TempBuf.GetLength());
	CString strMsg;
	if(iNum > 1)
	{
		strMsg.Format("Ç°Ãæ»¹ÓÐ %d Ìõµç¹â°åÐÅÏ¢µÈ´ýÏÔÊ¾.",iNum - 1);
		SendSystemMsg( strMsg.GetBuffer(), SYSTEM_NORMAL, TO_ME);
	}

	//SendAll(TempBuf, TempBuf.GetLength());
}

///////////////////////////////////////////////////////////////////////////////////
//	Session DB¿¡ Account °¡ ÀÖ´ÂÁö È®ÀÎÇÏ´Â Stored Proc
//
BOOL USER::CheckSessionLoginReq(TCHAR *strAccount, TCHAR *strPassword, int& nCount)
{
	nCount = 0;
	
	SQLHSTMT		hstmt;
	SQLRETURN		retcode;
	TCHAR			szSQL[1024];

	BOOL bQuerySuccess = TRUE;
	
	memset(szSQL, 0x00, 1024);
	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call LoginReq (\'%s\', \'%s\',?)}"), strAccount, strPassword);
	
	SQLSMALLINT	sCount = 0;
	SQLINTEGER sCountInd;

	hstmt = NULL;

	int db_index = -1;
	CDatabase* pDB = g_DBSession[m_iModSid].GetDB( db_index );
	if( !pDB ) return FALSE;
	
	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if (retcode!=SQL_SUCCESS)
	{
//		g_DBSession[m_iModSid].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLBindParameter(hstmt,1 ,SQL_PARAM_OUTPUT,SQL_C_SSHORT, SQL_SMALLINT, 0, 0, &sCount, 0, &sCountInd);
	if (retcode!=SQL_SUCCESS) 
	{
//		g_DBSession[m_iModSid].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLExecDirect (hstmt, (unsigned char *)szSQL, SQL_NTS);
	if (retcode ==SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
	{
		while (TRUE)
		{
			retcode = SQLFetch(hstmt);
			if (retcode !=SQL_SUCCESS && retcode !=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
		}
	}
	else if (retcode==SQL_ERROR)
	{
		bQuerySuccess = FALSE;
		DisplayErrorMsg(hstmt);
	}
	else if (retcode==SQL_NO_DATA)
	{
		bQuerySuccess = FALSE;
	}
	
	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

	if( !bQuerySuccess ) 
	{
//		BREAKPOINT();
		g_DBSession[m_iModSid].ReleaseDB(db_index);
		return FALSE;
	}

	nCount = sCount;

	g_DBSession[m_iModSid].ReleaseDB(db_index);

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////
// Ô­°æ²»´øUSER **pTempUser
BOOL USER::IsDoubleAccount(char *account)
{
	if( !strlen( account ) ) return TRUE;

	CString strSource = account;

	USER *pUser = NULL;
	for( int i = 0; i < MAX_USER; i++)
	{
		if(i == m_uid) continue;

		pUser = m_pCom->GetUserUid(i);
		if( pUser == NULL ) continue;

		if( !strSource.CompareNoCase(pUser->m_strAccount))
		{
			if( pUser->m_state != STATE_DISCONNECTED && pUser->m_state != STATE_LOGOUT)
			{
				if(pUser->m_bSessionOnline == true)
				{
					pUser->m_bSessionOnline = false;
					pUser->UpdateUserData(TRUE);
					pUser->SoftClose();
					return TRUE;
				}
				else
				{
					pUser->SendSystemMsg( IDS_USER_DOUBLE_ACCOUNT, SYSTEM_SPECIAL, TO_ME);//µ¯³ö½ÇÉ«
					pUser->SoftClose();
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////////
//	Session ¿¡ Login ÇßÀ» ¶§ÀÇ Ã³¸®
//
void USER::SessionLoginReq(TCHAR *pBuf)
{
	int			index = 0, nCount;
	TCHAR		strAccount[ACCOUNT_LENGTH+1], strPassword[PASSWORD_LENGTH+1];
	BYTE		result = FAIL, error_code = 0;
	CBufferEx	TempBuf;

	::ZeroMemory(strAccount, sizeof(strAccount));
	::ZeroMemory(strPassword, sizeof(strPassword));

	if(!GetVarString(sizeof(strAccount), strAccount, pBuf, index)) return;
	if(!GetVarString(sizeof(strPassword), strPassword, pBuf, index)) return;
	
	if( IsDoubleAccount( strAccount ) )
	{
		TRACE( "DOUBLE ACCOUNT - %s\n", strAccount );
		error_code = ERR_3;
		goto result_send;
	}

	if(CheckSessionLoginReq(strAccount, strPassword, nCount) == FALSE)
	{
		error_code = UNKNOWN_ERR;
		goto result_send;
	}
	
	if(nCount == 0) result = SUCCESS;
	else error_code = (BYTE)nCount;

result_send:

	TempBuf.Add(SESSION_LOGIN_RESULT);
	TempBuf.Add(result);

	if(result != SUCCESS)
	{
		TempBuf.Add(error_code);
		Send(TempBuf, TempBuf.GetLength());
		SoftClose();
	}
	else Send(TempBuf, TempBuf.GetLength());
}

////////////////////////////////////////////////////////////////////////////////////
//	Session ¿¡ New Account ¸¦ ¸¸µç´Ù.
//
void USER::NewAccountReq(TCHAR *pBuf)
{
	int		index = 0, nRet = -1;
	BYTE	result = FAIL;

	TCHAR	szUserID	[12 + 1];
	TCHAR	szPassword	[12 + 1];
	TCHAR	szRealName	[40 + 1];
	TCHAR	szSocNo		[15 + 1];
	TCHAR	szAddress	[80 + 1];
	TCHAR	szTel		[20 + 1];
	TCHAR	szEmail		[30 + 1];
	TCHAR	szQuiz01	[50 + 1];
	TCHAR	szAnswer01	[50 + 1];

	SQLHSTMT		hstmt;
	SQLRETURN		retcode;
	TCHAR			szSQL[1024];
	BOOL			bQuerySuccess = TRUE;
	CDatabase*		pDB = NULL;
	int				db_index = 0;
	SQLSMALLINT		sRet = 0;
	SQLINTEGER		sRetInd;

	if(!GetVarString(12, szUserID,			pBuf, index)) goto result_send;
	if(!GetVarString(12, szPassword,		pBuf, index)) goto result_send;
	if(!GetVarString(40, szRealName,		pBuf, index)) goto result_send;
	if(!GetVarString(15, szSocNo,			pBuf, index)) goto result_send;
	if(!GetVarString(80, szAddress,			pBuf, index)) goto result_send;
	if(!GetVarString(20, szTel,				pBuf, index)) goto result_send;
	if(!GetVarString(30, szEmail,			pBuf, index)) goto result_send;
	if(!GetVarString(50, szQuiz01,			pBuf, index)) goto result_send;
	if(!GetVarString(50, szAnswer01,		pBuf, index)) goto result_send;	
	
	memset(szSQL, 0x00, 1024);
	_sntprintf(szSQL, sizeof(szSQL), TEXT("{? = call NewAccount (\'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\')}"), 
		szUserID, szPassword, szRealName, szSocNo, szAddress, szTel, szEmail, szQuiz01, szAnswer01);
	
	hstmt = NULL;
	
	pDB = g_DBSession[m_iModSid].GetDB( db_index );
	if( !pDB ) return;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if (retcode!=SQL_SUCCESS)
	{
		//g_DBSession[m_iModSid].ReleaseDB(db_index);
		goto result_send;
	}

	retcode = SQLBindParameter(hstmt, 1 ,SQL_PARAM_OUTPUT,SQL_C_SSHORT, SQL_SMALLINT, 0, 0, &sRet, 0, &sRetInd);
	if (retcode!=SQL_SUCCESS) 
	{
		g_DBSession[m_iModSid].ReleaseDB(db_index);
		goto result_send;
	}

	retcode = SQLExecDirect (hstmt, (unsigned char *)szSQL, SQL_NTS);
	if (retcode ==SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
	{
		while (TRUE)
		{
			retcode = SQLFetch(hstmt);
			if (retcode !=SQL_SUCCESS && retcode !=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
		}
	}
	else if (retcode==SQL_ERROR)
	{
		bQuerySuccess = FALSE;
		DisplayErrorMsg(hstmt);
		SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

		g_DBSession[m_iModSid].ReleaseDB(db_index);
		goto result_send;
	}
	else if (retcode==SQL_NO_DATA)
	{
		bQuerySuccess = FALSE;
	}

	if (hstmt!=NULL) SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

	if(bQuerySuccess) nRet = sRet;

	g_DBSession[m_iModSid].ReleaseDB(db_index);

result_send:
		
	if(bQuerySuccess == TRUE && nRet == 0) result = SUCCESS;

	index = 0;
	SetByte(m_TempBuf, NEW_ACCOUNT_RESULT, index);
	SetByte(m_TempBuf, result, index);
	Send(m_TempBuf, index);
}
///////////////////////////////////////////////////////////////////////////////////
//	Character Á¤º¸¸¦ °¡Á®¿Â´Ù.
//
BOOL USER::LoadCharData(TCHAR *id)
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode;
	TCHAR			szSQL[1024];	::ZeroMemory(szSQL, sizeof(szSQL));

	BOOL bFind = FALSE;
	
	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call LOAD_CHAR_DATA (\'%s\')}")/*, m_iMyServer*/, id );
	
	SQLCHAR		strChar[3][CHAR_NAME_LENGTH+1];
	SQLINTEGER	strCharInd[3];

	int db_index = 0;
	CDatabase* pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB ) return FALSE;

	for( int i = 0; i < 3; i++)
	{
		::ZeroMemory(strChar[i], CHAR_NAME_LENGTH+1);
		strCharInd[i] = SQL_NTS;
	}

	retcode = SQLAllocHandle((SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt);
	if( retcode != SQL_SUCCESS )
	{
//		g_DB[m_iModSid].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, SQL_NTS );
	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		retcode = SQLFetch( hstmt );

		if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
		{
			SQLGetData( hstmt, 1, SQL_C_CHAR, strChar[0], CHAR_NAME_LENGTH, &strCharInd[0] );
			SQLGetData( hstmt, 2, SQL_C_CHAR, strChar[1], CHAR_NAME_LENGTH, &strCharInd[1] );
			SQLGetData( hstmt, 3, SQL_C_CHAR, strChar[2], CHAR_NAME_LENGTH, &strCharInd[2] );
		}
		else 
		{
			DisplayErrorMsg(hstmt);
			retcode = SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
			g_DB[m_iModSid].ReleaseDB(db_index);
			return FALSE;
		}
	}
	else
	{
		DisplayErrorMsg(hstmt);
		retcode = SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		//BREAKPOINT();

		g_DB[m_iModSid].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(db_index);

	m_nCharNum = 0;
	int nSize = sizeof(m_strChar);
	::ZeroMemory(m_strChar, sizeof(m_strChar));

	_tcscpy( m_strChar[0], (LPCTSTR)strChar[0]);		if( strlen( m_strChar[0] ) ) m_nCharNum++;
	_tcscpy( m_strChar[1], (LPCTSTR)strChar[1]);		if( strlen( m_strChar[1] ) ) m_nCharNum++;
	_tcscpy( m_strChar[2], (LPCTSTR)strChar[2]);		if( strlen( m_strChar[2] ) ) m_nCharNum++;
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//	Client¿¡ Character ±âº» Á¤º¸¸¦ º¸³½´Ù.
//
BOOL USER::SendCharInfo(TCHAR *strChar, TCHAR* pBuf, int& index)
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode;
	BOOL			bFind = FALSE;
	TCHAR			szSQL[1024];	::ZeroMemory(szSQL, sizeof(szSQL));

	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call SEND_CHAR_DATA (\'%s\')}"), strChar );
	
	SQLCHAR		strCharID[CHAR_NAME_LENGTH + 1], strFace[10];
	SQLINTEGER	strCharInd = SQL_NTS;

	SQLSMALLINT	sSTR, sCON, sDEX, sVOL, sWIS, sGender;
	SQLINTEGER	sInd;

	SDWORD		dSkin, dHair;

	SQLSMALLINT	sLevel;
	SQLCHAR		byClass;

	SQLSMALLINT	sHP, sPP, sSP;

	SQLCHAR		strSkill[_SKILL_DB], strItem[_ITEM_DB];

	::ZeroMemory(strCharID, sizeof(strCharID));
	::ZeroMemory(strFace, sizeof(strFace));
	::ZeroMemory(strSkill, sizeof(strSkill));
	::ZeroMemory(strItem, sizeof(strItem));

	sSTR = sCON = sDEX = sVOL = sWIS = sGender = 0;
	dSkin = dHair = 0;

	byClass = 0;
	sLevel = 1;

	int db_index = 0;
	CDatabase* pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
//		g_DB[m_iModSid].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, SQL_NTS );
	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		retcode = SQLFetch( hstmt );

		if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
		{
			int i = 1;

			SQLGetData( hstmt, i++,	SQL_C_CHAR,		strCharID,	sizeof(strCharID),	&strCharInd);
			SQLGetData( hstmt, i++,	SQL_C_SSHORT,	&sSTR,		sizeof(sSTR),		&sInd);
			SQLGetData( hstmt, i++,	SQL_C_SSHORT,	&sCON,		sizeof(sCON),		&sInd);
			SQLGetData( hstmt, i++,	SQL_C_SSHORT,	&sDEX,		sizeof(sDEX),		&sInd);
			SQLGetData( hstmt, i++,	SQL_C_SSHORT,	&sVOL,		sizeof(sVOL),		&sInd);
			SQLGetData( hstmt, i++,	SQL_C_SSHORT,	&sWIS,		sizeof(sWIS),		&sInd);
			SQLGetData( hstmt, i++,	SQL_C_ULONG,	&dSkin,		sizeof(dSkin),		&sInd);
			SQLGetData( hstmt, i++,	SQL_C_ULONG,	&dHair,		sizeof(dHair),		&sInd);
			SQLGetData( hstmt, i++,	SQL_C_SSHORT,	&sGender,	sizeof(sGender),	&sInd);

			SQLGetData( hstmt, i++, SQL_C_BINARY,	strFace,	sizeof(strFace),	&sInd);

			SQLGetData( hstmt, i++,	SQL_C_TINYINT,	&byClass,	sizeof(byClass),	&sInd);

			SQLGetData( hstmt, i++,	SQL_C_SSHORT,	&sLevel,	sizeof(sLevel),		&sInd);

			SQLGetData( hstmt, i++,	SQL_C_SSHORT,	&sHP,		sizeof(sHP),		&sInd);
			SQLGetData( hstmt, i++,	SQL_C_SSHORT,	&sPP,		sizeof(sPP),		&sInd);
			SQLGetData( hstmt, i++,	SQL_C_SSHORT,	&sSP,		sizeof(sSP),		&sInd);

			SQLGetData( hstmt, i++,	SQL_C_BINARY,	strSkill,	sizeof(strSkill),	&sInd);
			SQLGetData( hstmt, i++,	SQL_C_BINARY,	strItem,	sizeof(strItem),	&sInd);
		}
		else
		{
			DisplayErrorMsg(hstmt);
			retcode = SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);			
			g_DB[m_iModSid].ReleaseDB(db_index);
			return FALSE;
		}
	}
	else
	{
		DisplayErrorMsg(hstmt);
		retcode = SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

		g_DB[m_iModSid].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(db_index);

	TCHAR strID[CHAR_NAME_LENGTH+1];
	_tcscpy(strID, (TCHAR*)strCharID);
	short nSTR = sSTR; 
	short nCON = sCON;
	short nDEX = sDEX;
	short nVOL = sVOL;
	short nWIS = sWIS;

	short	sMaxHP = 0;
	short	sMaxPP = 0;
	short	sMaxSP = 0;

	DWORD dwSkin = dSkin;
	DWORD dwHair = dHair;

	BYTE  cGender = (BYTE)sGender;

	CBufferEx TempBuf;
	TempBuf.AddString(strID);
	TempBuf.Add(nSTR);
	TempBuf.Add(nCON);
	TempBuf.Add(nDEX);
	TempBuf.Add(nVOL);
	TempBuf.Add(nWIS);
	TempBuf.Add(dwSkin);
	TempBuf.Add(dwHair);
	TempBuf.Add(cGender);

	TempBuf.AddData((LPTSTR)strFace, 10);

	TempBuf.Add(byClass);
	TempBuf.Add(sLevel);
	TempBuf.Add((short)sHP);
	TempBuf.Add((short)sPP);
	TempBuf.Add((short)sSP);

	sMaxHP	= g_sHPConst[byClass] * nCON + g_sHPLV[byClass] * (sLevel - 1) + g_sHPAdd[byClass];
	sMaxPP	= g_sPPConst[byClass] * nWIS + g_sPPLV[byClass] * (sLevel - 1) + g_sPPAdd[byClass];
	sMaxSP	= g_sSPConst[byClass] * nCON + g_sSPLV[byClass] * (sLevel - 1) + g_sSPAdd[byClass];

	TempBuf.Add((short)sMaxHP);
	TempBuf.Add((short)sMaxPP);
	TempBuf.Add((short)sMaxSP);

	// Parse Skill
	CHAR szSkillBuf[3 * TOTAL_SKILL_NUM];
	::ZeroMemory(szSkillBuf, sizeof(szSkillBuf));
	if(!GetSkillNameLevel(byClass, (LPTSTR)strSkill, szSkillBuf)) return FALSE;

	// Parse Item
	TCHAR szItemBuf[2 * EQUIP_ITEM_NUM];		// 2 = Item Pid, 10 = Totoal Equip Slot Count
	::ZeroMemory(szItemBuf, sizeof(szItemBuf));

	if(!GetEquipItemPid((LPTSTR)strItem, szItemBuf))
	{
		return FALSE;
	}

	TempBuf.AddData(szSkillBuf, 3 * TOTAL_SKILL_NUM);
	TempBuf.AddData(szItemBuf, 2* EQUIP_ITEM_NUM);

	::CopyMemory(pBuf + index, TempBuf, TempBuf.GetLength());
	index += TempBuf.GetLength();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////
//	ÀÌ¹Ì Á¸ÀçÇÏ´Â Ä³¸¯ÅÍÀÎÁö °Ë»çÇÑ´Ù.
//
BOOL USER::IsExistCharId(TCHAR *id)
{
	/*
	SQLHSTMT		hstmt;
	SQLRETURN		retcode;
	TCHAR			szSQL[1024];

	BOOL bFind = FALSE;
	
	memset( szSQL, 0x00, 1024 );
	_sntprintf(szSQL, sizeof(szSQL), TEXT("SELECT strUserId FROM GAMEUSER WHERE strUserId = \'%s\'"), id );
	
	hstmt = NULL;

	int db_index = 0;
	CDatabase* pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		g_DB[m_iModSid].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, SQL_NTS );
	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		retcode = SQLFetch( hstmt );

		if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
		{
			bFind = TRUE;
		}
	}
	else
	{
		DisplayErrorMsg(hstmt);
		retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		BREAKPOINT();

		g_DB[m_iModSid].ReleaseDB(db_index);
		return TRUE;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(db_index);

	if(!bFind ) return FALSE;

	return TRUE;
	*/

	SQLHSTMT		hstmt;
	SQLRETURN		retcode;
	TCHAR			szSQL[1024];

	SQLSMALLINT	sRet;
	SQLINTEGER sRetInd = SQL_NTS;

	sRet = 0;

	memset( szSQL, 0x00, 1024 );
	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call IS_EXIST_CHAR (\'%s\', ?)}"), id);
	
	hstmt = NULL;

	int db_index = 0;
	CDatabase* pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		return FALSE;
	}

	retcode = SQLBindParameter(hstmt, 1 ,SQL_PARAM_OUTPUT,SQL_C_SSHORT, SQL_SMALLINT, 0, 0, &sRet, 0, &sRetInd);
	if( retcode != SQL_SUCCESS )
	{
		SQLFreeHandle((SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		return FALSE;
	}

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, SQL_NTS );
	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
//		retcode = SQLFetch( hstmt );
//
//		if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
//		{
//		}
	}
	else
	{
		DisplayErrorMsg(hstmt);
		retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
//		BREAKPOINT();

		g_DB[m_iModSid].ReleaseDB(db_index);
		return TRUE;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(db_index);

	if( sRet == 0 ) return FALSE;

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//	°ÔÀÓ ½ÃÀÛ
//
void USER::GameStart(TCHAR *pBuf)
{
	if( g_bShutDown ) return;

	int			index = 0;
	BYTE		result = FAIL, error_code = 0;
	TCHAR		szID[CHAR_NAME_LENGTH+1];
	CPoint		pt(-1, -1);
	CBufferEx	TempBuf;
	int			iMemory = 0;
	int			iMemoryAccountBank = 0;

	//TCHAR		bankstr[_BANK_DB];
	//TCHAR		accountbankstr[_ACCOUNT_BANK_DB];
	
	USER *pDoubleUser = NULL;

	int	nIDLength = GetVarString(sizeof(szID), szID, pBuf, index);

	if(nIDLength == 0 || nIDLength > CHAR_NAME_LENGTH)	
	{
		error_code = ERR_1;
		goto result_send;
	}

	int iMyServer, nAccountID;
	TCHAR		szAccountID[ACCOUNT_LENGTH+1];
#ifdef _CHINA
	iMyServer=1;
	index=index+2;
#else
	iMyServer = GetShort(pBuf, index);
#endif

	m_iMyServer = -1;

	if( !IsMyDBServer( iMyServer ) )
	{
		error_code = 255;
		goto result_send;
	}

	m_iMyServer = iMyServer;

	nAccountID = GetVarString(sizeof(szAccountID), szAccountID, pBuf, index);
	
	if(nAccountID == 0 || nAccountID > ACCOUNT_LENGTH)
	{
		error_code = 1;
		goto result_send;
	}

	if(!LoadCharData(szAccountID))
	{
		error_code = UNKNOWN_ERR;
		goto result_send;
	}
	
	DeleteMarkCharBanIP(inet_ntoa(m_useraddr.sin_addr));//É¾³ý°×Ãûµ¥IP

	if(CheckRenWuZhangHao(szAccountID,szID))//¼ì²âÈËÎïÕÊºÅ
	{
		strcpy(m_strAccount,szAccountID);
	}
	else
	{
		error_code = ERR_2;
		goto result_send;
	}

	if(IsDoubleAccount(m_strAccount))//ÀëÏßÉÌµêÌßÈË MarkJay
	{
		error_code = ERR_1;
		goto result_send;
	}

	//°°Àº ¾ÆÀÌµð°¡ ÀÖÀ¸¸é ½ÇÆÐ ÇÑ´Ù...
	pDoubleUser = GetUser( szID );
	if( pDoubleUser != NULL ) 
	{
		if( pDoubleUser->m_state != STATE_DISCONNECTED || pDoubleUser->m_state == STATE_GAMESTARTED || pDoubleUser->m_state != STATE_LOGOUT )
		{
			error_code = ERR_5;
			TempBuf.Add(GAME_START_RESULT);
			TempBuf.Add(FAIL);
			TempBuf.Add(error_code);
			Send(TempBuf, TempBuf.GetLength());
			pDoubleUser->SendSystemMsg( IDS_USER_DOUBLE_CHAR, SYSTEM_SPECIAL, TO_ME);
			SendSystemMsg( IDS_USER_DOUBLE_CHAR, SYSTEM_SPECIAL, TO_ME);
			pDoubleUser->SoftClose();
			SoftClose();
			return;
		}		
	}
	SetJiaoYiMiMa(szAccountID);//Ð´Èë½»Ò×ÃÜÂë
	g_pMainDlg->BridgeServerUserLogIn( m_uid, szAccountID, szID );//½øÈëÓÎÏ·
	return;



	InitUser();
//	InitMemoryDB(m_uid);
	iMemory = CheckMemoryDB( szID );
//	iMemory = 0;		// ¾ÆµÚ °°Àº °Íµµ ¾ø¾ú°í, °èÁ¤ °°Àº °Íµµ ¾ø¾ú´Ù.
//						// ¾ÆµÚ,ÀºÇà,ÅëÃ¢ ¸ðµÎ DB¿¡¼­ °¡Á®¿Í¾ß ÇÑ´Ù.

//	iMemory = 1;		// ¾ÆµÚ¶û °èÁ¤ÀÌ °°Àº °ÍÀº ÀÖ¾ú°í, °èÁ¤¸¸ °°°í ¾ÆµÚ°¡ ´Ù¸¥°ÍÀº ¾ø¾ú´Ù.
//						// ¾ÆµÚ,ÀºÇà,ÅëÃ¢ ¸ðµÎ MemoryDB¿¡¼­ °¡Á®¿Ô´Ù. DB¿¡¼­ °¡Á®¿ÀÁö ¾Ê´Â´Ù.

//	iMemory = 1;		// ¾ÆµÚ¶û °èÁ¤ÀÌ °°Àº °ÍÀÌ ÀÖ¾ú°í, °èÁ¤¸¸ °°°í ¾ÆµÚ°¡ ´Ù¸¥°Íµµ ÀÖ¾ú´Ù.
//						// ¾ÆµÚ,ÀºÇà,ÅëÃ¢ ¸ðµÎ MemoryDB¿¡¼­ °¡Á®¿Ô´Ù. DB¿¡¼­ °¡Á®¿ÀÁö ¾Ê´Â´Ù.

//	iMemory = 2;		// ¾ÆµÚ¶û °èÁ¤ÀÌ °°Àº °ÍÀº ¾ø¾ú°í, °èÁ¤¸¸ °°Àº °ÍÀÌ ÀÖ¾ú´Ù.
//						// ¾ÆµÚ,ÀºÇà,ÅëÃ¢ ¸ðµÎ DB¿¡¼­ °¡Á®¿Í¾ß ÇÑ´Ù. (³ªÁß¿¡ ÅëÃ¢¸¸ MemoryDB¿¡¼­ °¡Á®¿À´Â ·çÆ¾À» ³ÖÀ»°ÍÀÌ´Ù.)

	if( iMemory == 0 || iMemory == 2 )
	{
		if( !LoadUserData( szID ) )
		{
			error_code = ERR_2;
			goto result_send;
		}
	}
	

	if( !IsZoneInThisServer(m_curz) )
	{
		ChangeServer(m_curz);
		
		ReInitMemoryDB();

		SoftClose();
		return;
	}
//	SetZoneIndex(m_curz);

	m_state = STATE_CONNECTED;

	pt = FindNearAvailablePoint_S(m_curx, m_cury);	// DB¿¡ ÀúÀåµÈ ÁÂÇ¥°¡ ¿òÁ÷ÀÏ ¼ö ÀÖ´Â ÁÂÇ¥ÀÎÁö ÆÇ´Ü
	if(pt.x == -1 || pt.y == -1) 
	{
		error_code = ERR_4;
		goto result_send;
	}

	//CheckUserLevel();								// ÓÅ»¯´úÂë×¢Ïú MarkJay 

	m_curx = pt.x; m_cury = pt.y;
	SetUid(m_curx, m_cury, m_uid + USER_BAND );		// À¯Àú À§Ä¡Á¤º¸ ¼ÂÆÃ
	m_presx = -1;
	m_presy = -1;

	pt = ConvertToClient(m_curx, m_cury);

//	SetUserToMagicUser();
	GetMagicItemSetting();

	m_UserFlag = TRUE;

	result = SUCCESS;

	m_ConnectionSuccessTick = 0;


	InitMemoryDB(m_uid);

	if( iMemory == 0 || iMemory == 2 )
	{
		if( !LoadUserBank() )
		{
			error_code = ERR_2;
			result = FAIL;
			goto result_send;
		}
		if( !LoadAccountBank() )
		{
			error_code = ERR_2;
			result = FAIL;
			goto result_send;
		}
	}

/*
	if( iMemory == 0 )
	{
		if( !LoadUserBank() )
		{
			error_code = ERR_2;
			result = FAIL;
			goto result_send;
		}

//		iMemoryAccountBank = CheckMemoryAccountBankDB( m_strAccount );

//		if( iMemoryAccountBank == 0 )
//		{
//			if( !LoadAccountBank() )
//			{
//				error_code = ERR_2;
//				result = FAIL;
//				goto result_send;
//			}
//		}
//		else
//		{
//			UserAccountBankItemToStr( accountbankstr );
//			SetMemAccountBank( accountbankstr );
//		}
	}
*/
result_send:
	TempBuf.Add(GAME_START_RESULT);
	TempBuf.Add(result);

	if(result != SUCCESS)
	{
		ReInitMemoryDB();

		TempBuf.Add(error_code);
		Send(TempBuf, TempBuf.GetLength());
//		SockCloseProcess();
		SoftClose();
		return;
	}
	
	SendCharData();						// À¯ÀúÀÇ »ó¼¼ Á¤º¸¸¦ º¸³½´Ù.

	//TRACE( "Initial Point : %d %d\n", m_curx, m_cury);
	
	TempBuf.Add((short)m_curz);
	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.Add((short)pt.x);
	TempBuf.Add((short)pt.y);

	TempBuf.Add(m_tDir);

	TempBuf.Add((BYTE)g_GameTime);
	TempBuf.Add((BYTE)g_GameMinute);

	Send(TempBuf, TempBuf.GetLength());

	SetGameStartInfo();

	SendSystemMsg( IDS_USER_OPERATOR_MAIL1, SYSTEM_NORMAL, TO_ME);//"Mark¾öÕ½£¡"
//	SendSystemMsg( IDS_USER_OPERATOR_MAIL2, SYSTEM_NORMAL, TO_ME);//"Mark¾öÕ½£¬ÓÎÏ·ÖÐÓÐÈÎºÎÒÉÎÊÇëÁªÏµÖ÷Ò³¿Í·þ"
}
///////////////////////////////////////////////////////////////////////////
//	ÓÎÏ·¿ªÊ¼³õÊ¼»¯
void USER::SetGameStartInfo()
{
	// GameÀÌ Start µÉ¶§ ÇÊ¿äÇÑ Á¤º¸ Àü¼Û...
//	SendQuickChange();					// È¤½Ã ÀÌÀü Á¢¼Ó¿¡¼­ ¾ÆÀÌÅÛ µî·ÏÀÌ Ãë¼ÒµÈ°ÍÀÌ ÀÖ´ÂÁö ¾Ë¾Æº»´Ù.
	SendUserStatusSkill();

	SendWeatherInMoveZone();			// Á¸¿¡¼­ ³¯¾¾º¯È­¸¦ ¾Ë¾Æº»´Ù. 
	SetWeather(1, 0);					// Á¸¿¡¼­ ½Ã°£

	m_state = STATE_GAMESTARTED;

//	SendMyInfo(TO_INSIGHT, INFO_MODIFY);
	SendMyInfo(TO_INSIGHT, INFO_MODIFY);
	SightRecalc();						// ³» ½Ã¾ß¾ÈÀÇ Á¤º¸¸¦ ³ª¿¡°Ô º¸³½´Ù.

//	LoadUserBank();
//	LoadAccountBank();
	int rank = m_sCityRank + CITY_RANK_INTERVAL;
	if( rank != SAINT_RANK )		// ¾îÂî º¯Çßµç ¼¼ÀÎÆ®°¡ ¾Æ´Ï¶ó¸é
	{
		m_dwSaintTime = 0;
	}

	UpdateMemUserAll(TRUE);

	m_dwServerTick = GetTickCount();

	m_dwLastNoDamageTime = GetTickCount();
	m_dwNoDamageTime = NO_DAMAGE_TIME;
	//TRACE("######### Game Start ##################\n");
}

///////////////////////////////////////////////////////////////////////////
//	ÀåÂøÇÑ ¾ÆÀÌÅÛÁß ¸ÅÁ÷ÀÌ ÀÖÀ¸¸é ¸ÅÁ÷ ¼Ó¼ºÀ» ¹Ý¿µÇÑ´Ù.(º¯¼ö°ªÀ» º¯µ¿)
//	
void USER::GetMagicItemSetting()
{
	int i, j;

	int nCount = 4;										// ¸ÅÁ÷, ·¹¾îÀÏ¶§ ÂüÁ¶ È½¼ö
	int magic_slot = 0;
	int dynamic_slot = 0;
	BOOL bOldShield = CheckAbnormalInfo(ABNORMAL_SHIELD);
	CBufferEx TempBuf;

	for(i = 0; i < MAGIC_COUNT; i++) m_DynamicUserData[i] = 0;
	for(i = 0; i < EBODY_MAGIC_COUNT; i++) m_DynamicEBodyData[i] = 0;
	for(i = 0; i < 10; i++) m_DynamicMagicItem[i]= 0;
	

	for(i = 0; i < EQUIP_ITEM_NUM; i++)					// ÀüÃ¼ ¾ÆÀÌÅÛ ½½·Ô¸¦ ´Ù °Ë»çÇÑ´Ù.
	{
		if(m_UserItem[i].sSid >= 0)			
		{												// ¿©±â¿¡¼­ ¸ÅÁ÷¿¡ ¹Ý¿µµÈ ¾ÆÀÌÅÛ ·¹º§°ªÀ» ¼ÂÆÃÇÑ´Ù.				
			if((m_UserItem[i].tIQ > NORMAL_ITEM && m_UserItem[i].tIQ <= REMODEL_MAGIC_ITEM)||m_UserItem[i].tIQ==12)	// ´Ù ¸ÅÁ÷ ¼Ó¼ºÀÌ ºÙ¾î ÀÖÀ¸¸é...
			{
				if(m_UserItem[i].tIQ == UNIQUE_ITEM) nCount = MAGIC_NUM;
				if(m_UserItem[i].tIQ == 12)// 5ÅÅÊôÐÔÉúÐ§ MarkJay
				{
					nCount = 5;
					GetMagicItem_100(m_UserItem[i].tMagic[6]);
					GetMagicItem_100(m_UserItem[i].tMagic[7]);
				}
														// ¸ÅÁ÷, ·¹¾î´Â 4°³ ½½·Ô¸¸ ³ª¸ÓÁö´Â ¾÷±ÛÇÏ´Âµ¥ ¾¸
				if(i != LEFT_HAND)
				{
					for(j = 0; j < nCount; j++)			// ½½·Ô ¼ö¸¸Å­ µ¹¸°´Ù. 
					{				
						magic_slot = m_UserItem[i].tMagic[j];
						if(magic_slot < 0 || magic_slot >= g_arMagicItemTable.GetSize()) continue;

						dynamic_slot = g_arMagicItemTable[magic_slot]->m_sSubType;
						if(dynamic_slot < 0 || dynamic_slot >= MAGIC_COUNT) continue;

						m_DynamicUserData[dynamic_slot] += g_arMagicItemTable[magic_slot]->GetMagicItemValue();
					}
				}
				else
				{
					if(m_UserItem[i].sSid == 1121 && m_UserItem[i].sDuration > 0)//¸±ÊÖ	
					{
						for(j = 0; j < nCount; j++)			
						{				
							magic_slot = m_UserItem[i].tMagic[j];
							if(magic_slot < 0 || magic_slot >= g_arMagicItemTable.GetSize()) continue;

							dynamic_slot = g_arMagicItemTable[magic_slot]->m_sSubType;
							if(dynamic_slot < 0 || dynamic_slot >= MAGIC_COUNT) continue;

							m_DynamicUserData[dynamic_slot] += g_arMagicItemTable[magic_slot]->GetMagicItemValue();
						}
					}
				}
			}

			if(i == BODY || i == PANTS)	// 80 ·¦ ÀÌ»óÀÇ °©¿Ê¿¡´Â Æ¯¼ö ´É·ÂÄ¡°¡ ÀÖ´Ù.
			{
				if(m_byClass == 3)
				{
					if(m_UserItem[i].sSid >= 0 && m_UserItem[i].sSid < g_arItemTable.GetSize())
					{
						if(g_arItemTable[m_UserItem[i].sSid]->m_byRLevel == 80 && g_arItemTable[m_UserItem[i].sSid]->m_byClass == 1)
						{
							m_DynamicUserData[MAGIC_MAX_HP_UP] += g_arItemTable[m_UserItem[i].sSid]->m_byMPP;
						}
					}
				}
			}
		}
	}

	nCount = 5;
	for(i = TOTAL_INVEN_MAX; i < TOTAL_ITEM_NUM-1; i++)	// E-Body Magic »úÐµ
	{
		if(m_UserItem[i].sSid >= 0)			
		{
			if(m_UserItem[i].tMagic[5] > MAX_EBODY_UPGRADE_COUNT)//»úÐµ¸ÄÊý²»ºÏ·¨É¾³ý MarkJay
			{
				ReSetItemSlot(&m_UserItem[i]);
				TempBuf.Add(ITEM_GIVE_RESULT);
				TempBuf.Add((BYTE)0x01); 
					
				TempBuf.Add((BYTE)i);
				TempBuf.Add(m_UserItem[i].sLevel);
				TempBuf.Add(m_UserItem[i].sSid);
				TempBuf.Add(m_UserItem[i].sDuration);
				TempBuf.Add(m_UserItem[i].sBullNum);
				TempBuf.Add(m_UserItem[i].sCount);
					
				for(int j = 0; j < MAGIC_NUM; j++)
				{
					TempBuf.Add(m_UserItem[i].tMagic[j]);
					TempBuf.Add(m_UserItem[i].tIQ); 
					Send(TempBuf, TempBuf.GetLength());
				}
				continue;
			}
			if(m_UserItem[i].tIQ == MAGIC_ITEM )		// À¶É«»úÐµ
			{
				for(j = 0; j < nCount; j++)			// ½½·Ô ¼ö¸¸Å­ µ¹¸°´Ù. 
				{				
					magic_slot = m_UserItem[i].tMagic[j];
					if(magic_slot < 0 || magic_slot >= g_arEBodyTable.GetSize()) continue;
					
					dynamic_slot = g_arEBodyTable[magic_slot]->m_sSubType;
					if(dynamic_slot < 0 || dynamic_slot >= EBODY_MAGIC_COUNT) continue;
					
					m_DynamicEBodyData[dynamic_slot] += g_arEBodyTable[magic_slot]->m_sChangeValue;
				}
			}
			if(m_UserItem[i].sSid == 987)
			{
				//×îÖÕÉËÔö¼Ó
				m_DynamicMagicItem[7]=m_DynamicMagicItem[7] + m_UserItem[i].tMagic[5]*6;
			}
			else
			{ //×îÖÕÉË¼õÉÙ
				m_DynamicMagicItem[5]=m_DynamicMagicItem[5] + m_UserItem[i].tMagic[5]*2;
			}
		}
	}
	//Í³¼ÆÊØ»¤ÊôÐÔ
    GetMagicItemSetting_38();
	GetGuiIdMagic();
	//GetSkillSetting_130();//¼ÆËã130¼¼ÄÜ
	//°Ù¼¶ÉÏµÄ·ÀÄ§·À Ñª»ÃÊ¯¡£
	m_DynamicUserData[MAGIC_DEFENSE_UP]=m_DynamicUserData[MAGIC_DEFENSE_UP]+m_DynamicMagicItem[0];
	m_DynamicUserData[MAGIC_PSI_RESIST_UP]=m_DynamicUserData[MAGIC_PSI_RESIST_UP]+m_DynamicMagicItem[1];
	m_DynamicUserData[MAGIC_MAX_HP_UP]=m_DynamicUserData[MAGIC_MAX_HP_UP]+m_DynamicMagicItem[2];
	m_bRingOfLife		= FALSE;
	m_bNecklaceOfShield	= FALSE;
	m_bEarringOfProtect = FALSE;
	m_bCBOff	    	= FALSE;
	m_bShengRenjiezhi	= FALSE;
	for(i = 0; i < MAX_ACCESSORI; i++)
	{
		if(m_UserItem[g_iAccessoriSlot[i]].sSid == SID_RING_OF_LIFE)		m_bRingOfLife = TRUE;
		if(m_UserItem[g_iAccessoriSlot[i]].sSid == SID_NECKLACE_OF_SHIELD && m_UserItem[g_iAccessoriSlot[i]].sDuration > 0)	m_bNecklaceOfShield = TRUE;
		if(m_UserItem[g_iAccessoriSlot[i]].sSid == SID_EARRING_OF_PROTECT)	m_bEarringOfProtect = TRUE;
		if(m_UserItem[g_iAccessoriSlot[i]].sSid == SID_EARRING_OF_FEIFEI)	m_bCBOff = TRUE;
		if(m_UserItem[g_iAccessoriSlot[i]].sSid == SID_SHENGREN_JIEZHI)	    m_bShengRenjiezhi = TRUE;
	}
	//if(m_UserItem[0].tMagic[5] >= 10 && m_UserItem[1].tMagic[5] >= 10 && m_UserItem[4].tMagic[5] >= 10 && m_UserItem[8].tMagic[5] >= 10 && m_UserItem[9].tMagic[5] >= 10)//10¸ÄÌ×´øÊØ»¤ÏîÁ´ºÍÎÞÎÒÐ§¹û MarkJay
	//{
	//	m_bNecklaceOfShield = TRUE;
	//	m_bEarringOfProtect = TRUE;
	//}
	if(m_bNecklaceOfShield == TRUE) 
	{
		AddAbnormalInfo(ABNORMAL_SHIELD);
		if(bOldShield == FALSE)
		{
			TempBuf.Add(SET_USER_STATE);
			TempBuf.Add(m_uid + USER_BAND);
			TempBuf.Add(m_dwAbnormalInfo);
			TempBuf.Add(m_dwAbnormalInfo_);

			SendInsight(TempBuf, TempBuf.GetLength());
		}
	}
	else
	{
		if(bOldShield == TRUE && m_dwShieldTime == 0)
		{
			DeleteAbnormalInfo(ABNORMAL_SHIELD);
			TempBuf.Add(SET_USER_STATE);
			TempBuf.Add(m_uid + USER_BAND);
			TempBuf.Add(m_dwAbnormalInfo);
			TempBuf.Add(m_dwAbnormalInfo_);

			SendInsight(TempBuf, TempBuf.GetLength());
		}
	}

	/*if(m_dwBFindTime > 0)//²âÊÔÍ·¶¥´øNEW MarkJay
	{
		AddNewStatus(New_Status_22);
		SendABNORMAL();
	}*/
	if(m_dwMaxHPUpTime)	SetUserToMagicUser(1);	// À¯ÀúÀÇ ±âº»´É·ÂÄ¡ º¯µ¿°ªÀ» ÀúÀåÇÑ´Ù.
	else SetUserToMagicUser();
	SendUserStatusSkill();
//¿ØÖÆ±äÉíºóÉËº¦ ¿ª¹Ø
	if(m_dwBFindTime != 0) m_sDramageUp = true;
	if(m_iSkin == 4 || m_iSkin == 7 || m_iSkin == 8) m_sDramageUpPercent = true;
}
//Í³¼ÆÊØ»¤ÊôÐÔ.
void USER::GetMagicItemSetting_38()
{
	if(m_UserItem[TOTAL_ITEM_NUM-1].sSid==-1)
		return ;
	if(m_UserItem[TOTAL_ITEM_NUM-1].sDuration<=0)
		return ;
	if(m_UserItem[TOTAL_ITEM_NUM-1].tIQ!=9) //¿´¿´ÊÇ²»ÊÇÊØ»¤ÊôÐÔ ²»ÊÇ·µ»Ø
		return ;
	switch (m_UserItem[TOTAL_ITEM_NUM-1].tMagic[0]){//¼ÆËãÊØ»¤±¾ÉíÊôÐÔ
			case 1://1 ËðÉËÔö¼Ó2
				m_DynamicUserData[MAGIC_DAMAGE_UP]=m_DynamicUserData[MAGIC_DAMAGE_UP]+2;
				break;
			case 2:// 2 ËðÉËÔö¼Ó3
				m_DynamicUserData[MAGIC_DAMAGE_UP]=m_DynamicUserData[MAGIC_DAMAGE_UP]+3;
				break;
			case 3:// 3 ËðÉËÔö¼Ó5
				m_DynamicUserData[MAGIC_DAMAGE_UP]=m_DynamicUserData[MAGIC_DAMAGE_UP]+5;
				break;
			case 4://4  ÎäÆ÷µÈ¼¶Ôö¼Ó1
				m_DynamicUserData[MAGIC_DAMAGE_UP]=m_DynamicUserData[MAGIC_DAMAGE_UP]+16;
				break;
			case 5://5  ÎäÆ÷µÈ¼¶Ôö¼Ó2
				m_DynamicUserData[MAGIC_DAMAGE_UP]=m_DynamicUserData[MAGIC_DAMAGE_UP]+32;
				break;
			case 6:// 6 ÎäÆ÷µÈ¼¶Ôö¼Ó3
				m_DynamicUserData[MAGIC_DAMAGE_UP]=m_DynamicUserData[MAGIC_DAMAGE_UP]+48;
				break;
			case 7:// 7 ½«¶Ô·½ÉúÃü2×ª»¯³É×Ô¼ºÉúÃü
				break;
			case 8://8½«¶Ô·½ÉúÃü5×ª»¯³É×Ô¼ºÉúÃü
				break;
			case 9://9 ½«¶Ô·½ÉúÃü10×ª»¯³É×Ô¼ºÉúÃü
				break;
			case 10:// 10¶ÔÖÜÎ§Ôì³É2%ÉËº¦
				break;
			case 11://11 ¶ÔÖÜÎ§Ôì³É5%ÉËº¦
				break;
			case 12://12 ¶ÔÖÜÎ§Ôì³É10%ÉËº¦
				break;
			case 13://13 ·ÀÓùÔö¼Ó3
					m_DynamicUserData[MAGIC_DEFENSE_UP]=m_DynamicUserData[MAGIC_DEFENSE_UP]+3;
				break;
			case 14://14 ·ÀÓùÔö¼Ó6
					m_DynamicUserData[MAGIC_DEFENSE_UP]=m_DynamicUserData[MAGIC_DEFENSE_UP]+6;
				break;
			case 15://15 ·ÀÓùÔö¼Ó10
					m_DynamicUserData[MAGIC_DEFENSE_UP]=m_DynamicUserData[MAGIC_DEFENSE_UP]+10;
				break;
			case 16://16Ä§·¨·ÀÓùÔö¼Ó3
				m_DynamicUserData[MAGIC_PSI_RESIST_UP]=m_DynamicUserData[MAGIC_PSI_RESIST_UP]+3;
				break;
			case 17:// 17Ä§·¨·ÀÓùÔö¼Ó6
				m_DynamicUserData[MAGIC_PSI_RESIST_UP]=m_DynamicUserData[MAGIC_PSI_RESIST_UP]+6;
				break;
			case 18:// 18Ä§·¨·ÀÓùÔö¼Ó10
				m_DynamicUserData[MAGIC_PSI_RESIST_UP]=m_DynamicUserData[MAGIC_PSI_RESIST_UP]+10;
				break;
			case 19://19½«µÐÈË¶Ô×Ô¼º¹¥»÷ÉËº¦2%·´Éäµ½µÐÈËÉíÉÏ
				break;
			case 20://20½«µÐÈË¶Ô×Ô¼º¹¥»÷ÉËº¦3%·´Éäµ½µÐÈËÉíÉÏ
				break;
			case 21://21½«µÐÈË¶Ô×Ô¼º¹¥»÷ÉËº¦5%·´Éäµ½µÐÈËÉíÉÏ
				break;
			case 22://22 ´ò¹Ö¾­ÑéÖµÌá¸ß2%
				break;
			case 23:// 23´ò¹Ö¾­ÑéÖµÌá¸ß3%
				break;
			case 24://24 ´ò¹Ö¾­ÑéÖµÌá¸ß5%
				break;
			case 25:// 25 ½ð±ÒÌá¸ß10%
				break;
			case 26:// 26 ½ð±ÒÌá¸ß20%
				break;
			case 27:// 27 ½ð±ÒÌá¸ß50%
				break;
			case 28:// 28 ËùÓÐ¼¼ÄÜÔö¼Ó1
				m_DynamicUserData[MAGIC_ALL_SKILL_UP]=m_DynamicUserData[MAGIC_ALL_SKILL_UP]+1;
				break;
			case 29://29ËùÓÐ¼¼ÄÜÔö¼Ó2
				m_DynamicUserData[MAGIC_ALL_SKILL_UP]=m_DynamicUserData[MAGIC_ALL_SKILL_UP]+2;
				break;
			case 30://30ËùÓÐ¼¼ÄÜÔö¼Ó3
				m_DynamicUserData[MAGIC_ALL_SKILL_UP]=m_DynamicUserData[MAGIC_ALL_SKILL_UP]+3;
				break;
			case 31://31µôÂäÎïÆ·Ìá¸ß2%
				break;
			case 32:// 32µôÂäÎïÆ·Ìá¸ß5%
				break;
			case 33://33µôÂäÎïÆ·Ìá¸ß10%
				break;
			case 34://34¹¥»÷Á¦Ôö¼Ó10
				m_DynamicUserData[MAGIC_DAMAGE_UP]=m_DynamicUserData[MAGIC_DAMAGE_UP]+10;
				break;
			case 35:// 35¹¥»÷Á¦Ôö¼Ó20
				m_DynamicUserData[MAGIC_DAMAGE_UP]=m_DynamicUserData[MAGIC_DAMAGE_UP]+20;
				break;
			case 36://36¹¥»÷Á¦Ôö¼Ó30
				m_DynamicUserData[MAGIC_DAMAGE_UP]=m_DynamicUserData[MAGIC_DAMAGE_UP]+30;
				break;
			case 37://37ÎüÈ¡¶Ô·½ÉúÃü10
				break;
			case 38:// 38ÎüÈ¡¶Ô·½ÉúÃü20
				break;
			case 39://39ÎüÈ¡¶Ô·½ÉúÃü30
				break;
			case 40://40ÉúÃüÔö¼Ó10
				m_DynamicUserData[MAGIC_MAX_HP_UP]=m_DynamicUserData[MAGIC_MAX_HP_UP]+10;
				break;
			case 41://41ÉúÃüÔö¼Ó20
				m_DynamicUserData[MAGIC_MAX_HP_UP]=m_DynamicUserData[MAGIC_MAX_HP_UP]+20;
				break;
			case 42://  42ÉúÃüÔö¼Ó50
				m_DynamicUserData[MAGIC_MAX_HP_UP]=m_DynamicUserData[MAGIC_MAX_HP_UP]+50;
				break;
				
	}
	for(int i=0;i<3;i++){
		switch (m_UserItem[TOTAL_ITEM_NUM-1].tMagic[1+i]){//¼ÆËãÊØ»¤666ÊôÐÔ
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6://Ã¿´Î¼Óµã3·À
				m_DynamicUserData[MAGIC_DEFENSE_UP]=m_DynamicUserData[MAGIC_DEFENSE_UP]+m_UserItem[TOTAL_ITEM_NUM-1].tMagic[1+i]*3;
				break;
			case 7:
			case 8:
			case 9:
			case 10:
			case 11:
			case 12://Ã¿¼¶¼Ó15Ñª
				m_DynamicUserData[MAGIC_MAX_HP_UP]=m_DynamicUserData[MAGIC_MAX_HP_UP]+(m_UserItem[TOTAL_ITEM_NUM-1].tMagic[1+i]-6)*15;
				break;
			case 13:
			case 14:
			case 15:
			case 16:
			case 17:
			case 18://Ã¿¼¶Ôö¼Ó5¿¹AGIC_DEFENSE_UP
				m_DynamicUserData[MAGIC_PSI_RESIST_UP]=m_DynamicUserData[MAGIC_PSI_RESIST_UP]+(m_UserItem[TOTAL_ITEM_NUM-1].tMagic[1+i]-12)*5;
				break;
		 default:
		 	break;
		}
	}	
}
//¼ÆËã130¼¼ÄÜ
void USER::GetSkillSetting_130()
{
	m_Hit = 0;
	m_Avoid = 0;
	int iCount,sLevel;

	for(int i = 3 * m_byClass;i < (3 * m_byClass + 3);i++)
	{
		switch(i)
		{
			case 0: //ÖÂÃüÖ®´Ï //Ôö¼ÓÃüÖÐ
				sLevel = m_UserSkill_[i].tLevel;
				if(sLevel > 0 && sLevel <= 20)
				{
					m_Hit = (sLevel - 1) * 10 + 30;
				}
				break;
			case 1://ÉúÃüÖ®¹â Ôö¼ÓÉúÃü
				sLevel = m_UserSkill_[i].tLevel;
				if(sLevel > 0 && sLevel <= 20)
				{
					iCount = (sLevel - 1) * 20 + 150;
					m_DynamicUserData[MAGIC_MAX_HP_UP] = m_DynamicUserData[MAGIC_MAX_HP_UP] + iCount;
				}
				break;
			case 2:// ³¬¼¶î¸Æø   ¸ÄÎª µÚÒ»¼¶¡£Ôö¼Ó 30¿¹ 30·À ÒÔºóÃ¿¼¶ Ôö¼Ó 3µã¿¹ 3µã·À
				sLevel = m_UserSkill_[i].tLevel;
				if(sLevel > 0 && sLevel <= 20)
				{
					iCount = (sLevel - 1) * 6 + 30;
					m_DynamicUserData[MAGIC_DEFENSE_UP] = m_DynamicUserData[MAGIC_DEFENSE_UP] + iCount;
					m_DynamicUserData[MAGIC_PSI_RESIST_UP] = m_DynamicUserData[MAGIC_PSI_RESIST_UP] + iCount;
				}
				break;
			case 3://¸¨Öú Ä§·¨  ÎÞÐ§
				break;
			case 4://ÉúÃüÖ®¹â Ôö¼ÓÉúÃü
				sLevel = m_UserSkill_[i].tLevel;
				if(sLevel > 0 && sLevel <= 20)
				{
					iCount = (sLevel - 1) * 12 + 90;
					m_DynamicUserData[MAGIC_MAX_HP_UP] = m_DynamicUserData[MAGIC_MAX_HP_UP] + iCount;
				}
				break;
			case 5://ÁéÃôÖ®»Û
				sLevel = m_UserSkill_[i].tLevel;
				if(sLevel > 0 && sLevel <= 20)
				{
					m_Avoid = (sLevel - 1) * 4 + 24;
				}
				break;
			case 6://ÉúÃüÖ®¹â Ôö¼ÓÉúÃü
				sLevel = m_UserSkill_[i].tLevel;
				if(sLevel > 0 && sLevel <= 20)
				{
					iCount = (sLevel - 1) * 16 + 120;
					m_DynamicUserData[MAGIC_MAX_HP_UP] = m_DynamicUserData[MAGIC_MAX_HP_UP] + iCount;
				}
				break;
			case 7://»¤Ìåî¸Æø  µÚÒ»¼¶ Ôö¼Ó·À 40  Ã¿¼¶Ôö¼Ó1·À
				sLevel = m_UserSkill_[i].tLevel;
				if(sLevel > 0 && sLevel <= 20)
				{
					iCount = (sLevel - 1) * 1 + 40;
					m_DynamicUserData[MAGIC_DEFENSE_UP] = m_DynamicUserData[MAGIC_DEFENSE_UP] + iCount;
				}
				break;
			case 8://Ä§·¨¿¹³â  µÚÒ»¼¶ Ôö¼Ó¿¹ 42  Ã¿¼¶Ôö¼Ó3¿¹
				sLevel = m_UserSkill_[i].tLevel;
				if(sLevel > 0 && sLevel <= 20)
				{
					iCount = (sLevel - 1) * 3 + 42;
					m_DynamicUserData[MAGIC_PSI_RESIST_UP] = m_DynamicUserData[MAGIC_PSI_RESIST_UP] + iCount;
				}
				break;
			case 9://ÉúÃüÖ®¹â   µÚÒ»¼¶ ÉúÃü+105 ÒÔÃ¿¼¶Ôö¼Ó14µãÉúÃü
				sLevel = m_UserSkill_[i].tLevel;
				if(sLevel > 0 && sLevel <= 20)
				{
					iCount = (sLevel - 1) * 14 + 105;
					m_DynamicUserData[MAGIC_MAX_HP_UP] = m_DynamicUserData[MAGIC_MAX_HP_UP] + iCount;
				}
				break;
			case 10://Ä§·¨¿¹³â  µÚÒ»¼¶ Ôö¼Ó¿¹ 45  Ã¿¼¶Ôö¼Ó5¿¹
				sLevel = m_UserSkill_[i].tLevel;
				if(sLevel > 0 && sLevel <= 20)
				{
					iCount = (sLevel - 1) * 8 + 65;
					m_DynamicUserData[MAGIC_PSI_RESIST_UP] = m_DynamicUserData[MAGIC_PSI_RESIST_UP] + iCount;
				}
				break;
			case 11://ÁéÃôÖ®»Û    µÚÒ»¼¶ Ôö¼Ó 50   Ã¿¼¶Ôö¼Ó7µã
				sLevel = m_UserSkill_[i].tLevel;
				if(sLevel > 0 && sLevel <= 20)
				{
					m_Avoid = (sLevel - 1) * 7 + 50;
				}
				break;
			default:
				break;
		}
	}
//±äÉíÊôÐÔ
	//if(m_iSkin == 1) m_Hit = 15;m_DynamicUserData[MAGIC_MAX_HP_UP] = m_DynamicUserData[MAGIC_MAX_HP_UP] + 100;
	if(m_dwBFindTime != 0)
	{
		m_DynamicUserData[MAGIC_MAX_HP_UP] = m_DynamicUserData[MAGIC_MAX_HP_UP] + 250;//100Ñª
		m_DynamicUserData[MAGIC_DEFENSE_UP] = m_DynamicUserData[MAGIC_DEFENSE_UP] + 50;//50·ÀÓù
	}
	//if(m_byClass == 0) m_Hit += 50;//È­¼ÓÃüÖÐ
	if(m_dwVIPTime != 0)
	{
		if(m_byClass == 0) 
		{	
			//m_Hit += 1;//È­ÃüÖÐ//m_DynamicUserData[MAGIC_DEFENSE_UP] = m_DynamicUserData[MAGIC_DEFENSE_UP] + 30;//È­¼Ó50·ÀÓù
			m_DynamicUserData[MAGIC_MAX_HP_UP] = m_DynamicUserData[MAGIC_MAX_HP_UP] + 100;
		}
		else if(m_byClass == 1) 
		{
			//m_Avoid += 1;//·¨Ê¦»Ø±Ü
			m_DynamicUserData[MAGIC_MAX_HP_UP] = m_DynamicUserData[MAGIC_MAX_HP_UP] + 100;
		}	
		else if(m_byClass == 2)
		{
			//m_Hit += 1;//µ¶ÃüÖÐ
			m_DynamicUserData[MAGIC_MAX_HP_UP] = m_DynamicUserData[MAGIC_MAX_HP_UP] + 100;
		}
		else if(m_byClass == 3) 
		{
			//m_Hit += 1;//Ç¹¼ÓÃüÖÐ
			m_DynamicUserData[MAGIC_MAX_HP_UP] = m_DynamicUserData[MAGIC_MAX_HP_UP] + 100;
		}
	}
	if(m_dwGuild != -1)
	{
		m_DynamicUserData[MAGIC_MAX_HP_UP] = m_DynamicUserData[MAGIC_MAX_HP_UP] + 100;//¾üÍÅ³ÉÔ±¶à¸ø100Ñª MarkJay
	}
	if(m_UserItem[0].tMagic[5] >= 7 && m_UserItem[1].tMagic[5] >= 7 && m_UserItem[4].tMagic[5] >= 7 && m_UserItem[8].tMagic[5] >= 7 && m_UserItem[9].tMagic[5] >= 7)
	{
		m_Avoid += 10;
	}
	if(m_UserItem[0].tMagic[5] >= 8 && m_UserItem[1].tMagic[5] >= 8 && m_UserItem[4].tMagic[5] >= 8 && m_UserItem[8].tMagic[5] >= 8 && m_UserItem[9].tMagic[5] >= 8)
	{
		m_DynamicUserData[MAGIC_PSI_RESIST_UP] = m_DynamicUserData[MAGIC_PSI_RESIST_UP] + 25;
		//m_Avoid += 20;
	}
	if(m_UserItem[0].tMagic[5] >= 9 && m_UserItem[1].tMagic[5] >= 9 && m_UserItem[4].tMagic[5] >= 9 && m_UserItem[8].tMagic[5] >= 9 && m_UserItem[9].tMagic[5] >= 9)
	{
		m_DynamicUserData[MAGIC_MAX_HP_UP] = m_DynamicUserData[MAGIC_MAX_HP_UP] + 100;
		m_DynamicUserData[MAGIC_PSI_RESIST_UP] = m_DynamicUserData[MAGIC_PSI_RESIST_UP] + 25;
		//m_Avoid += 20;
	}
	if(m_UserItem[0].tMagic[5] >= 10 && m_UserItem[1].tMagic[5] >= 10 && m_UserItem[4].tMagic[5] >= 10 && m_UserItem[8].tMagic[5] >= 10 && m_UserItem[9].tMagic[5] >= 10)
	{
		m_DynamicUserData[MAGIC_STR_UP] = m_DynamicUserData[MAGIC_STR_UP] + 20;//Á¦
		m_DynamicUserData[MAGIC_CON_UP] = m_DynamicUserData[MAGIC_CON_UP] + 20;//Ìå
		m_DynamicUserData[MAGIC_DEX_UP] = m_DynamicUserData[MAGIC_DEX_UP] + 20;//Ãô
		m_DynamicUserData[MAGIC_VOL_UP] = m_DynamicUserData[MAGIC_VOL_UP] + 20;//»Û
		m_DynamicUserData[MAGIC_WIS_UP] = m_DynamicUserData[MAGIC_WIS_UP] + 20;//ÖÇ
		m_Hit += 15;
		//m_Avoid += 35;
	}
    if(m_UserItem[2].sSid == 820 && m_UserItem[3].sSid == 814)
	{
		if( m_UserItem[6].sSid == 639 || m_UserItem[7].sSid == 639)
		{
			m_DynamicUserData[MAGIC_DAMAGE_UP]+=10;		
			m_DynamicUserData[MAGIC_PSI_ATTACK_UP]+=10;
			AddMagicPoint(10);
		}
	}
	switch(m_dwJiaMoFang)//Ä§·Â
	{
		case 1: m_DynamicUserData[MAGIC_PSI_RESIST_UP] = m_DynamicUserData[MAGIC_PSI_RESIST_UP] + 20; break;
		case 2: m_DynamicUserData[MAGIC_PSI_RESIST_UP] = m_DynamicUserData[MAGIC_PSI_RESIST_UP] + 40; break;
		case 3: m_DynamicUserData[MAGIC_PSI_RESIST_UP] = m_DynamicUserData[MAGIC_PSI_RESIST_UP] + 60; break;
		case 4: m_DynamicUserData[MAGIC_PSI_RESIST_UP] = m_DynamicUserData[MAGIC_PSI_RESIST_UP] + 80; break;
		case 5: m_DynamicUserData[MAGIC_PSI_RESIST_UP] = m_DynamicUserData[MAGIC_PSI_RESIST_UP] + 100; break;
		case 6: m_DynamicUserData[MAGIC_PSI_RESIST_UP] = m_DynamicUserData[MAGIC_PSI_RESIST_UP] + 120; break;
		case 7: m_DynamicUserData[MAGIC_PSI_RESIST_UP] = m_DynamicUserData[MAGIC_PSI_RESIST_UP] + 140; break;
		case 8: m_DynamicUserData[MAGIC_PSI_RESIST_UP] = m_DynamicUserData[MAGIC_PSI_RESIST_UP] + 160; break;
		case 9: m_DynamicUserData[MAGIC_PSI_RESIST_UP] = m_DynamicUserData[MAGIC_PSI_RESIST_UP] + 180; break;
		case 10: m_DynamicUserData[MAGIC_PSI_RESIST_UP] = m_DynamicUserData[MAGIC_PSI_RESIST_UP] + 200; break;
	}
	switch(m_dwJiaXueLiang)//»Ø±Ü
	{
		case 1: m_Avoid += 5; break;
		case 2: m_Avoid += 10; break;
		case 3: m_Avoid += 15; break;
		case 4: m_Avoid += 20; break;
		case 5: m_Avoid += 25; break;
		case 6: m_Avoid += 30; break;
		case 7: m_Avoid += 35; break;
		case 8: m_Avoid += 40; break;
		case 9: m_Avoid += 45; break;
		case 10: m_Avoid += 50; break;
	}
	switch(m_dwJiaXueLiang)//ÃüÖÐ
	{
		case 1: m_Hit += 5; break;
		case 2: m_Hit += 10; break;
		case 3: m_Hit += 15; break;
		case 4: m_Hit += 20; break;
		case 5: m_Hit += 25; break;
		case 6: m_Hit += 30; break;
		case 7: m_Hit += 35; break;
		case 8: m_Hit += 40; break;
		case 9: m_Hit += 45; break;
		case 10: m_Hit += 50; break;
	}
	FuShou_Attack();//¸±ÊÖ¼Ó¹¥»÷ MarkJay
	if(m_iSkin == 4)
	{
		if(m_iHair == 10000)//»Æ»ú¼×¼ÓÑª
		{
			m_DynamicUserData[MAGIC_MAX_HP_UP]=m_DynamicUserData[MAGIC_MAX_HP_UP]+50;
		}
		else if(m_iHair == 10001)//ºì»ú¼×¼ÓÑª
		{
			m_DynamicUserData[MAGIC_MAX_HP_UP]=m_DynamicUserData[MAGIC_MAX_HP_UP]+80;
		}
		else if(m_iHair == 10002)//À¶»ú¼×¼ÓÑª
		{
			m_DynamicUserData[MAGIC_MAX_HP_UP]=m_DynamicUserData[MAGIC_MAX_HP_UP]+100;
		}
		else if(m_iHair == 10004)//ÂÌ»ú¼×¼ÓÑª
		{
			m_DynamicUserData[MAGIC_MAX_HP_UP]=m_DynamicUserData[MAGIC_MAX_HP_UP]+120;
		}
		else if(m_iHair == 10005)//×Ï»ú¼×¼ÓÑª
		{
			m_DynamicUserData[MAGIC_MAX_HP_UP]=m_DynamicUserData[MAGIC_MAX_HP_UP]+150;
		}
		else if(m_iHair == 10006)//ºÚ»ú¼×¼ÓÑª
		{
			m_DynamicUserData[MAGIC_MAX_HP_UP]=m_DynamicUserData[MAGIC_MAX_HP_UP]+200;
		}
	}
	if(m_iSkin == 7) m_DynamicUserData[MAGIC_MAX_HP_UP] = m_DynamicUserData[MAGIC_MAX_HP_UP] + 300;
	if(m_iSkin == 8) m_DynamicUserData[MAGIC_MAX_HP_UP] = m_DynamicUserData[MAGIC_MAX_HP_UP] + 100;
}
//¸±ÊÖ¹¥»÷¼ÆËã MarkJay
void USER::FuShou_Attack()
{
////////////////////////////////////////////////////////////////////////////////////////////////////////
	if(m_UserItem[5].sSid == 1121 && m_UserItem[5].sDuration > 0)//¸±ÊÖÎäÆ÷¼Ó°Ù·Ö±È¹¥»÷ 
	{
		int Damage = 0;
		if(m_byClass == 1)
		{
			Damage = GetUserSpellAttack();
			if(m_UserItem[5].tMagic[0] == 190) m_DynamicUserData[MAGIC_PSI_ATTACK_UP] += (int)((double)Damage * 0.01);
			else if(m_UserItem[5].tMagic[0] == 191) m_DynamicUserData[MAGIC_PSI_ATTACK_UP] += (int)((double)Damage * 0.02);
			else if(m_UserItem[5].tMagic[0] == 192) m_DynamicUserData[MAGIC_PSI_ATTACK_UP] += (int)((double)Damage * 0.03);
			else if(m_UserItem[5].tMagic[0] == 193) m_DynamicUserData[MAGIC_PSI_ATTACK_UP] += (int)((double)Damage * 0.04);
			else if(m_UserItem[5].tMagic[0] == 194) m_DynamicUserData[MAGIC_PSI_ATTACK_UP] += (int)((double)Damage * 0.05);
			else if(m_UserItem[5].tMagic[0] == 195) m_DynamicUserData[MAGIC_PSI_ATTACK_UP] += (int)((double)Damage * 0.06);
			else if(m_UserItem[5].tMagic[0] == 196) m_DynamicUserData[MAGIC_PSI_ATTACK_UP] += (int)((double)Damage * 0.07);
			else if(m_UserItem[5].tMagic[0] == 197) m_DynamicUserData[MAGIC_PSI_ATTACK_UP] += (int)((double)Damage * 0.08);
			else if(m_UserItem[5].tMagic[0] == 198) m_DynamicUserData[MAGIC_PSI_ATTACK_UP] += (int)((double)Damage * 0.09);
			else if(m_UserItem[5].tMagic[0] == 199) m_DynamicUserData[MAGIC_PSI_ATTACK_UP] += (int)((double)Damage * 0.10);
		}
		else
		{
			Damage = GetMaxDamage(m_byClass);
			if(m_UserItem[5].tMagic[0] == 190) m_DynamicUserData[MAGIC_DAMAGE_UP] += (int)((double)Damage * 0.01);
			else if(m_UserItem[5].tMagic[0] == 191) m_DynamicUserData[MAGIC_DAMAGE_UP] += (int)((double)Damage * 0.02);
			else if(m_UserItem[5].tMagic[0] == 192) m_DynamicUserData[MAGIC_DAMAGE_UP] += (int)((double)Damage * 0.03);
			else if(m_UserItem[5].tMagic[0] == 193) m_DynamicUserData[MAGIC_DAMAGE_UP] += (int)((double)Damage * 0.04);
			else if(m_UserItem[5].tMagic[0] == 194) m_DynamicUserData[MAGIC_DAMAGE_UP] += (int)((double)Damage * 0.05);
			else if(m_UserItem[5].tMagic[0] == 195) m_DynamicUserData[MAGIC_DAMAGE_UP] += (int)((double)Damage * 0.06);
			else if(m_UserItem[5].tMagic[0] == 196) m_DynamicUserData[MAGIC_DAMAGE_UP] += (int)((double)Damage * 0.07);
			else if(m_UserItem[5].tMagic[0] == 197) m_DynamicUserData[MAGIC_DAMAGE_UP] += (int)((double)Damage * 0.08);
			else if(m_UserItem[5].tMagic[0] == 198) m_DynamicUserData[MAGIC_DAMAGE_UP] += (int)((double)Damage * 0.09);
			else if(m_UserItem[5].tMagic[0] == 199) m_DynamicUserData[MAGIC_DAMAGE_UP] += (int)((double)Damage * 0.10);
		}
	}
////////////////////////////////////////////////////////////////////////////////////////////////////////
	if(m_UserItem[5].sSid == 1121 && m_UserItem[5].sDuration > 0)//¸±ÊÖÎäÆ÷¼Ó·ÀÓù 
	{
		int Damage1 = 0;//ÉúÃü
		short Damage2 = 0;//Ä§¿¹
		short sTemp = 0;
		short  Damage3 = 0;//·ÀÓù
	
		sTemp = (short)GetIronSkill();
		Damage1 = GetHP();
		Damage2 = GetUserSpellDefence();
		Damage3 = sTemp + GetDefense();

		if(m_UserItem[5].tMagic[1] == 201)
		{
			m_DynamicUserData[MAGIC_MAX_HP_UP] += (int)((double)Damage1 * 0.01);
			m_DynamicUserData[MAGIC_PSI_RESIST_UP] += (int)((double)Damage2 * 0.01);
			m_DynamicUserData[MAGIC_DEFENSE_UP] += (int)((double)Damage3 * 0.01);
		}
		if(m_UserItem[5].tMagic[1] == 202) 
		{
			m_DynamicUserData[MAGIC_MAX_HP_UP] += (int)((double)Damage1 * 0.02);
			m_DynamicUserData[MAGIC_PSI_RESIST_UP] += (int)((double)Damage2 * 0.02);
			m_DynamicUserData[MAGIC_DEFENSE_UP] += (int)((double)Damage3 * 0.02);
		}
		if(m_UserItem[5].tMagic[1] == 203) 
		{
			m_DynamicUserData[MAGIC_MAX_HP_UP] += (int)((double)Damage1 * 0.03);
			m_DynamicUserData[MAGIC_PSI_RESIST_UP] += (int)((double)Damage2 * 0.03);
			m_DynamicUserData[MAGIC_DEFENSE_UP] += (int)((double)Damage3 * 0.03);
		}
		if(m_UserItem[5].tMagic[1] == 204) 
		{
			m_DynamicUserData[MAGIC_MAX_HP_UP] += (int)((double)Damage1 * 0.04);
			m_DynamicUserData[MAGIC_PSI_RESIST_UP] += (int)((double)Damage2 * 0.04);
			m_DynamicUserData[MAGIC_DEFENSE_UP] += (int)((double)Damage3 * 0.04);
		}
		if(m_UserItem[5].tMagic[1] == 205) 
		{
			m_DynamicUserData[MAGIC_MAX_HP_UP] += (int)((double)Damage1 * 0.05);
			m_DynamicUserData[MAGIC_DEFENSE_UP] += (int)((double)Damage3 * 0.05);
			m_DynamicUserData[MAGIC_PSI_RESIST_UP] += (int)((double)Damage2 * 0.05);
		}
		if(m_UserItem[5].tMagic[1] == 206) 
		{
			m_DynamicUserData[MAGIC_MAX_HP_UP] += (int)((double)Damage1 * 0.06);
			m_DynamicUserData[MAGIC_PSI_RESIST_UP] += (int)((double)Damage2 * 0.06);
			m_DynamicUserData[MAGIC_DEFENSE_UP] += (int)((double)Damage3 * 0.06);
		}
		if(m_UserItem[5].tMagic[1] == 207) 
		{
			m_DynamicUserData[MAGIC_MAX_HP_UP] += (int)((double)Damage1 * 0.07);
			m_DynamicUserData[MAGIC_PSI_RESIST_UP] += (int)((double)Damage2 * 0.07);
			m_DynamicUserData[MAGIC_DEFENSE_UP] += (int)((double)Damage3 * 0.07);
		}
		if(m_UserItem[5].tMagic[1] == 208) 
		{
			m_DynamicUserData[MAGIC_MAX_HP_UP] += (int)((double)Damage1 * 0.08);
			m_DynamicUserData[MAGIC_PSI_RESIST_UP] += (int)((double)Damage2 * 0.08);
			m_DynamicUserData[MAGIC_DEFENSE_UP] += (int)((double)Damage3 * 0.08);
		}
		if(m_UserItem[5].tMagic[1] == 209) 
		{
			m_DynamicUserData[MAGIC_MAX_HP_UP] += (int)((double)Damage1 * 0.09);
			m_DynamicUserData[MAGIC_PSI_RESIST_UP] += (int)((double)Damage2 * 0.09);
			m_DynamicUserData[MAGIC_DEFENSE_UP] += (int)((double)Damage3 * 0.09);
		}
		if(m_UserItem[5].tMagic[1] == 210) 
		{
			m_DynamicUserData[MAGIC_MAX_HP_UP] += (int)((double)Damage1 * 0.10);
			m_DynamicUserData[MAGIC_PSI_RESIST_UP] += (int)((double)Damage2 * 0.10);
			m_DynamicUserData[MAGIC_DEFENSE_UP] += (int)((double)Damage3 * 0.10);
		}
   }
}
void USER::GetGuiIdMagic()
{
	LoadGuildLevel();
	if(m_dwGuild != -1)
	{   
		if(m_dwGuildLevel >= 300 && m_dwGuildLevel < 2000)
		{
			SendSystemMsg("Ò»½×¾üÍÅÊôÐÔ¼¤»î ÉúÃüÖµ+50 ·ÀÓù+8 Ä§¿¹+8",SYSTEM_NORMAL,TO_ME);
			m_DynamicUserData[MAGIC_DEFENSE_UP]=m_DynamicUserData[MAGIC_DEFENSE_UP]+8;//·ÀÓù
			m_DynamicUserData[MAGIC_PSI_RESIST_UP]+=8;//Ä§·¨¿¹³â
			m_DynamicUserData[MAGIC_MAX_HP_UP]=m_DynamicUserData[MAGIC_MAX_HP_UP]+50;//ÉúÃüÔö¼Ó10
		}
		else if(m_dwGuildLevel >= 2000 && m_dwGuildLevel < 4000)
		{
			SendSystemMsg("¶þ½×¾üÍÅÊôÐÔ¼¤»î ÉúÃüÖµ+100 ·ÀÓù+12 Ä§¿¹+12",SYSTEM_NORMAL,TO_ME);
			m_DynamicUserData[MAGIC_DEFENSE_UP]=m_DynamicUserData[MAGIC_DEFENSE_UP]+12;//·ÀÓù
			m_DynamicUserData[MAGIC_PSI_RESIST_UP]+=12;//Ä§·¨¿¹³â
			m_DynamicUserData[MAGIC_MAX_HP_UP]=m_DynamicUserData[MAGIC_MAX_HP_UP]+100;//ÉúÃüÔö¼Ó20		 
		}
		else if(m_dwGuildLevel >= 4000 && m_dwGuildLevel < 6000)
		{
			SendSystemMsg("Èý½×¾üÍÅÊôÐÔ¼¤»î ÉúÃüÖµ+150 ·ÀÓù+24 Ä§¿¹+24",SYSTEM_NORMAL,TO_ME);
			m_DynamicUserData[MAGIC_DEFENSE_UP]=m_DynamicUserData[MAGIC_DEFENSE_UP]+24;//·ÀÓù
			m_DynamicUserData[MAGIC_PSI_RESIST_UP]+=24;//Ä§·¨¿¹³â
			m_DynamicUserData[MAGIC_MAX_HP_UP]=m_DynamicUserData[MAGIC_MAX_HP_UP]+150;//ÉúÃüÔö¼Ó30	 
		}
		else if(m_dwGuildLevel >= 6000 && m_dwGuildLevel < 999999)
		{
			SendSystemMsg("ËÄ½×¾üÍÅÊôÐÔ¼¤»î ÉúÃüÖµ+200 ·ÀÓù+36 Ä§¿¹+36",SYSTEM_NORMAL,TO_ME);
			m_DynamicUserData[MAGIC_DEFENSE_UP]=m_DynamicUserData[MAGIC_DEFENSE_UP]+36;//·ÀÓù
			m_DynamicUserData[MAGIC_PSI_RESIST_UP]+=36;//Ä§·¨¿¹³â
			m_DynamicUserData[MAGIC_MAX_HP_UP]=m_DynamicUserData[MAGIC_MAX_HP_UP]+200;//ÉúÃüÔö¼Ó40
		}
	}

	if(m_dwJiaGong >= 50 && m_dwJiaGong < 300)
	{
		SendSystemMsg(" ÐÂ±øÊôÐÔ¼¤»î Îï¹¥+8 Ä§¹¥+4",SYSTEM_NORMAL,TO_ME);
		m_DynamicUserData[MAGIC_DAMAGE_UP]+=8;//ÎïÀíËðÉË+4
		m_DynamicUserData[MAGIC_PSI_ATTACK_UP]+=4;//Ä§·¨ËðÉË+2
	}
	else if(m_dwJiaGong >= 300 && m_dwJiaGong < 800)
	{
		SendSystemMsg(" ÉÙÐ£ÊôÐÔ¼¤»î Îï¹¥+16 Ä§¹¥+8",SYSTEM_NORMAL,TO_ME);
		m_DynamicUserData[MAGIC_DAMAGE_UP]+=16;//ÎïÀíËðÉË+16
		m_DynamicUserData[MAGIC_PSI_ATTACK_UP]+=8;//Ä§·¨ËðÉË+8
	}
	else if(m_dwJiaGong >= 800 && m_dwJiaGong < 2000)
	{
		SendSystemMsg(" ÖÐÐ£ÊôÐÔ¼¤»î Îï¹¥+32 Ä§¹¥+16",SYSTEM_NORMAL,TO_ME);
		m_DynamicUserData[MAGIC_DAMAGE_UP]+=32;//ÎïÀíËðÉË+16
		m_DynamicUserData[MAGIC_PSI_ATTACK_UP]+=16;//Ä§·¨ËðÉË+8
	}
	else if( m_dwJiaGong >= 2000 && m_dwJiaGong < 999999)
	{
		SendSystemMsg(" ÉÏÐ£ÊôÐÔ¼¤»î Îï¹¥+40 Ä§¹¥+25 ÉúÃü+100 ",SYSTEM_NORMAL,TO_ME);
		m_DynamicUserData[MAGIC_DAMAGE_UP]+=40;//ÎïÀíËðÉË+32
		m_DynamicUserData[MAGIC_PSI_ATTACK_UP]+=25;//Ä§·¨ËðÉË+16
		m_DynamicUserData[MAGIC_MAX_HP_UP]+=100;	
	}
}
int USER::GetMagicItem_100(byte tMagic)
{
	if(tMagic < 3 || tMagic > 52) return 0;
	int iDynamic=(tMagic-3)/10;
	int isLevel= (tMagic-3)%10+1;
	int iValue;
	switch(iDynamic)
	{
		case 0://·À+5
			iValue=5*isLevel; break;
		case 1://Ä§·¨·À+5
			iValue=5*isLevel; break;
		case 2://23 ÉúÃüÔö¼Ó10
			iValue=10*isLevel; break;
		case 3://»Ø+4 ²»¿ª·Å
			iValue=0; break;
		case 4://Ä§·¨ÉË¼õÉÙ 10
			iValue=10*isLevel; break;
		case 5://×îÖÕÉË¼õÉÙ 5
			iValue=5*isLevel; break;
		case 6://ÎïÀíÉË¼õÉÙ 10
			iValue=10*isLevel; break;							 
	}
	m_DynamicMagicItem[iDynamic] = m_DynamicMagicItem[iDynamic] + iValue;
	return iValue;
}

///////////////////////////////////////////////////////////////////////////
//	Log Out
//
void USER::LogOut()
{
	if(m_bLogOut == TRUE) return;
	if(m_state != STATE_GAMESTARTED) return;	// ÀÌ¿Ü STATE_CONNECTEDµî ÀÏ¶§´Â ÇØ´ç ÃÊ±âÈ­°¡ ¸Þ¸ð¸®¿¡¾øÀ¸¹Ç·Î Á¦¿ÜÇØ¾ßÇÑ´Ù.
	m_bLogOut = TRUE;
		
	if(m_tGuildWar == GUILD_WARRING && m_dwFieldWar > 0)		
	{
		if(m_bGuildMaster) 
		{
			CString strMsg = _T("");
			strMsg.Format( IDS_USER_GUILD_DEFEAT, m_strGuildName);//"%s ¾üÍÅ±»´ò°Ü."
			SendGuildWarFieldEnd((LPTSTR)(LPCTSTR)strMsg);// Ç×º¹
		}
	}

	if(m_tGuildHouseWar == GUILD_WARRING) CheckGuildUserListInGuildHouseWar(); // ´Ù¸¥ À¯ÀúµéÀº ¹»ÇÏ³ª Ã¼Å©..

	if(m_tQuestWar == GUILD_WARRING) g_QuestEventZone.CheckUserStateInEventZone(m_curz);//CheckQuestEventZoneWarEnd();

	if(m_bNowBuddy == TRUE)				// ¹öµðÁßÀÌ¸é Åëº¸ÇÑ´Ù.
	{
		for(int i = 0; i < MAX_BUDDY_USER_NUM; i++)
		{
			if(m_MyBuddy[i].uid == m_uid + USER_BAND) SendBuddyUserLeave(i);
		}
	}

	// °Å·¡ÁßÀÌ¸é °Å·¡Ãë¼Ò Ã³¸®
	if(m_bNowTrading == TRUE) 
	{
		BYTE result = 0x00;
		USER *pTradeUser = NULL;
		if(m_iTradeUid != -1)	pTradeUser = GetUser(m_iTradeUid - USER_BAND);

		if(pTradeUser != NULL)	pTradeUser->SendExchangeFail(result, (BYTE)0x05);
	}

	// ·Î¾â·³ºí Ã³¸®
	ExitRoyalRumble();

	// º¸È£ÄÚµå Ãß°¡
	int nRet1 = 0, nRet2 = 0, nRet3 = 0;
	nRet1 = UpdateMemBankDataOnly();
	nRet3 = UpdateMemAccountBankDataOnly();
	nRet2 = UpdateUserData(TRUE);

	if(nRet1 == 1 && nRet2 == 1 && nRet3 == 1) ReInitMemoryDB();
	else
	{
		if(nRet1 == 0)
		{
			UpdateMemBankDataOnly();
		}
		if(nRet2 == 0)
		{
			UpdateUserData(TRUE);
		}
		if(nRet3 == 0)
		{
			UpdateMemAccountBankDataOnly();
		}
		ReInitMemoryDB();
	}

	// º¸È£ÄÚµå ( x, y, z°ªÀÌ ¹üÀ§¸¦ ³Ñ´Â°ªÀ¸·Î º¯ÇÒ¼öÀÖ´Ù )
	MAP* pMap = NULL;

	if( m_ZoneIndex < 0 || m_ZoneIndex >= g_zone.GetSize() ) goto go_result;

	pMap = g_zone[m_ZoneIndex];		if( !pMap ) goto go_result;

	if( m_curx < 0 || m_curx >= pMap->m_sizeMap.cx ) goto go_result;
	if( m_cury < 0 || m_cury >= pMap->m_sizeMap.cy ) goto go_result;

	if( g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser == USER_BAND + m_uid ) 
		::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser, 0);

go_result:
	SendMyInfo(TO_INSIGHT, INFO_DELETE);

	m_state = STATE_LOGOUT;

	g_pMainDlg->BridgeServerUserLogOut( m_uid, m_strUserID );
}

void USER::ZoneLogOut(int save_z, int save_x, int save_y)
{
	if(m_bLogOut == TRUE) return;
	if(m_state != STATE_GAMESTARTED) return;	// ÀÌ¿Ü STATE_CONNECTEDµî ÀÏ¶§´Â ÇØ´ç ÃÊ±âÈ­°¡ ¸Þ¸ð¸®¿¡¾øÀ¸¹Ç·Î Á¦¿ÜÇØ¾ßÇÑ´Ù.

	m_bLogOut = TRUE;
//	USER *pUser = NULL;
												// ÀÏ¹Ý ÇÊµåÀü ÁßÀÌ¶ó¸é..		
	if(m_tGuildWar == GUILD_WARRING && m_dwFieldWar > 0)		
	{
		if(m_bGuildMaster) 
		{
			CString strMsg = _T("");
			strMsg.Format( IDS_USER_GUILD_DEFEAT, m_strGuildName);//"%s ¾üÍÅ±»´ò°Ü."
			SendGuildWarFieldEnd((LPTSTR)(LPCTSTR)strMsg);// Ç×º¹
		}
	}

	if(m_tGuildHouseWar == GUILD_WARRING) CheckGuildUserListInGuildHouseWar(); // ´Ù¸¥ À¯ÀúµéÀº ¹»ÇÏ³ª Ã¼Å©..

	if(m_bNowBuddy == TRUE)				// ¹öµðÁßÀÌ¸é Åëº¸ÇÑ´Ù.
	{
		for(int i = 0; i < MAX_BUDDY_USER_NUM; i++)
		{
			if(m_MyBuddy[i].uid == m_uid + USER_BAND) SendBuddyUserLeave(i);
		}
	}

	// °Å·¡ÁßÀÌ¸é °Å·¡Ãë¼Ò Ã³¸®
	if(m_bNowTrading == TRUE) 
	{
		BYTE result = 0x00;
		USER *pTradeUser = NULL;
		if(m_iTradeUid != -1)	pTradeUser = GetUser(m_iTradeUid - USER_BAND);

		if(pTradeUser != NULL)	pTradeUser->SendExchangeFail(result, (BYTE)0x05);
	}

	// ·Î¾â·³ºí Ã³¸®
	ExitRoyalRumble();

	int backup_x, backup_y, backup_z;

	backup_z = m_curz;
	backup_x = m_curx;
	backup_y = m_cury;

	m_curz = save_z;
	m_curx = save_x;
	m_cury = save_y;

	// º¸È£ÄÚµå Ãß°¡
	int nRet1 = 0, nRet2 = 0, nRet3 = 0;
	nRet1 = UpdateMemBankDataOnly();
	nRet3 = UpdateMemAccountBankDataOnly();
	nRet2 = UpdateUserData(TRUE);

	if(nRet1 == 1 && nRet2 == 1 && nRet3 == 1) ReInitMemoryDB();
	else
	{
		if(nRet1 == 0)
		{
			UpdateMemBankDataOnly();
		}
		if(nRet2 == 0)
		{
			UpdateUserData(TRUE);
		}
		if(nRet3 == 0)
		{
			UpdateMemAccountBankDataOnly();
		}
		ReInitMemoryDB();
	}

	m_curz = backup_z;
	m_curx = backup_x;
	m_cury = backup_y;

	// º¸È£ÄÚµå ( x, y, z°ªÀÌ ¹üÀ§¸¦ ³Ñ´Â°ªÀ¸·Î º¯ÇÒ¼öÀÖ´Ù )
	MAP* pMap = NULL;

	if( m_ZoneIndex < 0 || m_ZoneIndex >= g_zone.GetSize() ) goto go_result;

	pMap = g_zone[m_ZoneIndex];		if( !pMap ) goto go_result;

	if( m_curx < 0 || m_curx >= pMap->m_sizeMap.cx ) goto go_result;
	if( m_cury < 0 || m_cury >= pMap->m_sizeMap.cy ) goto go_result;

	if( g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser == USER_BAND + m_uid ) 
		::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser, 0);

go_result:
	SendMyInfo(TO_INSIGHT, INFO_DELETE);

	m_state = STATE_LOGOUT;
}

///////////////////////////////////////////////////////////////////////////
BOOL USER::UpdateUserData(BOOL bLogOut)
{
	if(!bLogOut) if(UpdateMemUserAll()) return TRUE;//MarkJay ²»ÍË³öÖ»¸üÐÂÄÚ´æ

	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	BOOL			bQuerySuccess = TRUE;
	TCHAR			szSQL[8000];		
	TCHAR			strFace[10], strSkill[_SKILL_DB], strItem[_ITEM_DB], strPsi[_PSI_DB], strTel[_TEL_DB];
	TCHAR			strQuickItem[_QUICKITEM_DB];
	TCHAR			strHaveEvent[_EVENT_DB];
	int				i;

	::ZeroMemory(szSQL, sizeof(szSQL));

	::ZeroMemory(strFace, sizeof(strFace));
	::ZeroMemory(strSkill, sizeof(strSkill));
	::ZeroMemory(strItem, sizeof(strItem));
	::ZeroMemory(strPsi, sizeof(strPsi));	
	::ZeroMemory(strTel, sizeof(strTel));
	::ZeroMemory(strHaveEvent, sizeof(strHaveEvent));
	
	::ZeroMemory(strQuickItem, sizeof(strQuickItem));

	::CopyMemory(strFace, m_strFace, sizeof(m_strFace));

	UserSkillToStr(strSkill);
	UserItemToStr(strItem);
	UserPsiToStr(strPsi);
	UserTelToStr(strTel);
	UserHaveEventDataToStr(strHaveEvent);

	DWORD dwCurTime = ConvertCurTimeToSaveTime();		

	SDWORD sFaceLen		= sizeof(strFace);
	SDWORD sSkillLen	= sizeof(strSkill);
	SDWORD sItemLen		= sizeof(strItem);
	SDWORD sPsiLen		= sizeof(strPsi);
	SDWORD sQuickLen	= sizeof(strQuickItem);
	SDWORD sEventLen	= sizeof(strHaveEvent);
	SDWORD sTelLen		= sizeof(strTel);	

	m_tPsiOneKind = m_tPsiTwoKind = m_tPsiThreeKind = 0;
	m_dwPsiOneTime = m_dwPsiTwoTime = m_dwPsiThreeTime = 0;

	if(m_dwHasteTime != 0) 
	{
		m_tPsiOneKind = PSIONIC_HASTE;
		m_dwPsiOneTime = m_dwHasteTime;
	}
	if(m_dwShieldTime != 0) 
	{
		m_tPsiOneKind = PSIONIC_SHIELD;
		m_dwPsiOneTime = m_dwShieldTime;
	}
	if(m_dwDexUpTime != 0) 
	{
		m_tPsiOneKind = PSIONIC_DEXUP;
		m_dwPsiOneTime = m_dwDexUpTime;
	}
	if(m_dwMaxHPUpTime != 0) 
	{
		m_tPsiOneKind = PSIONIC_HPUP;
		m_dwPsiOneTime = m_dwMaxHPUpTime;
	}
	if(m_dwFastRunTime != 0) 
	{
		m_tPsiOneKind = PSIONIC_FAST_RUN;
		m_dwPsiOneTime = m_dwFastRunTime;
	}
	if(m_dwMindShockTime != 0) 
	{
		m_tPsiOneKind = PSIONIC_MIND_SHOCK;
		m_dwPsiOneTime = m_dwMindShockTime;
	}
	if(m_dwPsiShieldTime != 0) 
	{
		m_tPsiOneKind = PSIONIC_PSI_SHIELD;
		m_dwPsiOneTime = m_dwPsiShieldTime;
	}
	if(m_dwBigShieldTime != 0)
	{
		m_tPsiOneKind = 30;
		m_dwPsiOneTime = m_dwBigShieldTime;
	}
	if(m_dwPiercingShieldTime != 0) 
	{
		m_tPsiOneKind = PSIONIC_PIERCING_SHIELD;
		m_dwPsiOneTime = m_dwPiercingShieldTime;
	}

	if(m_dwAdamantineTime != 0) 
	{
		m_tPsiTwoKind = PSIONIC_ADAMANTINE;
		m_dwPsiTwoTime = m_dwAdamantineTime;
	}
	if(m_dwMightyWeaponTime != 0) 
	{
		m_tPsiTwoKind = PSIONIC_MIGHTYWEAPON;
		m_dwPsiTwoTime = m_dwMightyWeaponTime;
	}
	if(m_dwBerserkerTime != 0) 
	{
		m_tPsiTwoKind = PSIONIC_BERSERKER;
		m_dwPsiTwoTime = m_dwBerserkerTime;
	}

	if(m_dwMindGuardTime != 0) 
	{
		m_tPsiThreeKind = PSIONIC_MIND_GUARD;
		m_dwPsiThreeTime = m_dwMindGuardTime;
	}

	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call UPDATE_USER_DATA (\'%s\',%d,%d,%d,%d,%d,%d,%d,%d,\
		?, %d,%d,%d, %d, %d,%d,  %d,%d,  %d, \
		%d,%d,%d,%d,%d,%d, %d,%d,%d,%d, \
		?,?,?,?, %d,%d,\
		%d, %d, %d, ?, %d, %d,\
		?, %d,\
		%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d,\
		\'%s\', \
		%d,%d,		%d,%d,		%d,%d)}"), 
		m_strUserID,	m_sSTR,	m_sCON,	m_sDEX,	m_sVOL,	m_sWIS,	m_iSkin, m_iHair, m_sGender,	
		m_curz,	m_curx,	m_cury,		m_dwBuddy,		m_dwGuild, m_dwExp,		m_sPA, m_sSkillPoint,	m_dwXP,
		m_sMaxHP, m_sHP, m_sMaxPP, m_sPP, m_sMaxSP,	m_sSP,		m_dwDN,	m_sCityRank, m_sLevel,	m_byClass,

		m_tAbnormalKind, m_dwAbnormalTime,         //%D

		m_bLive, m_bLock, m_iCityValue, m_sKillCount, dwCurTime, 
		m_dwSaintTime, 
		m_dwHiExpTime, m_dwMagicFindTime, m_dwNoChatTime, m_dwBFindTime, m_dwVIPTime, m_dwCloseTime, m_dwShopPingDN, m_dwGuarDianTianShi, m_dwJiFen, m_dwChouJiang, m_dwRenWu, m_dwCKMiMa, m_dwHuanJing, m_dwHuanLing, m_dwZaiXianTime, m_dwPKAddkill, m_dwFUExpTime, m_dwJiaGong, m_dwJiaFangYu, m_dwJiaMoFang, m_dwJiaGongJi, m_dwJiaXueLiang, m_dwJiaMoney,
		m_strLoveName, //Êý¾Ý¿â 
		m_tPsiOneKind, m_dwPsiOneTime,		m_tPsiTwoKind, m_dwPsiTwoTime,		m_tPsiThreeKind, m_dwPsiThreeTime); 

	int db_index = 0;
	CDatabase* pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if(retcode != SQL_SUCCESS)
		return FALSE;

	if (retcode == SQL_SUCCESS)
	{
		i = 1;
		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strFace),	0, (TCHAR*)strFace,		0, &sFaceLen );

		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strSkill),	0, (TCHAR*)strSkill,	0, &sSkillLen );
		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strItem),	0, (TCHAR*)strItem,		0, &sItemLen );
		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strPsi),		0, (TCHAR*)strPsi,		0, &sPsiLen );
		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strQuickItem),	0, (TCHAR*)strQuickItem,	0, &sQuickLen );

		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strHaveEvent),	0, (TCHAR*)strHaveEvent,	0, &sEventLen );

		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strTel),			0, (TCHAR*)strTel,			0, &sTelLen );

		retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, SQL_NTS);
		if (retcode ==SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{
		}
		else if (retcode==SQL_ERROR)
		{
			DisplayErrorMsg( hstmt );
			bQuerySuccess = FALSE;
		}
	}

	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(db_index);

	if( !bQuerySuccess ) return FALSE;

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////
//	¸üÐÂ½»Ò×Êý¾Ý
BOOL USER::UpdateUserExchangeData(USER *pUser)
{ 
	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return FALSE;
	if(UpdateMemUserAll() && pUser->UpdateMemUserAll()) return TRUE;

	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	BOOL			bQuerySuccess = TRUE;
	TCHAR			szSQL[8000];		
	TCHAR			strItem[_ITEM_DB], strUserItem[_ITEM_DB];
	int				i;

	::ZeroMemory(szSQL, sizeof(szSQL));
	::ZeroMemory(strItem, sizeof(strItem));
	::ZeroMemory(strUserItem, sizeof(strUserItem));

	UserItemToStr(strItem);
	pUser->UserItemToStr(strUserItem);

	SDWORD sItemLen			= sizeof(strItem);
	SDWORD sUserItemLen		= sizeof(strUserItem);

	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call UPDATE_USER_EXCHANGE_DATA (\'%s\',%d,?,\'%s\',%d,?)}"), 
		m_strUserID, m_dwDN, pUser->m_strUserID, pUser->m_dwDN); 

	int db_index = 0;
	CDatabase* pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if(retcode != SQL_SUCCESS)
		return FALSE;

	if (retcode == SQL_SUCCESS)
	{
		i = 1;
		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strItem),	0, (TCHAR*)strItem,		0, &sItemLen );
		SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, sizeof(strUserItem),	0, (TCHAR*)strUserItem,		0, &sUserItemLen );

		retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, SQL_NTS);
		
		if (retcode ==SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{
		}
		else if (retcode==SQL_ERROR)
		{
			DisplayErrorMsg( hstmt );
			bQuerySuccess = FALSE;
		}
	}
	
	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(db_index);

	if( !bQuerySuccess ) return FALSE;

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////
//	½¨Á¢Êý¾Ý¿â±í½á¹¹Á¬½Ó
BOOL USER::LoadUserData(TCHAR *szID)
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode;
	TCHAR			szSQL[1024];
	DWORD			dwMaxDN = 0;
	BYTE			tPoint = 0;
	int				i;

	::ZeroMemory(szSQL, sizeof(szSQL));
	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call LOAD_USER_DATA (\'%s\')}"), szID);
	
	SQLSMALLINT sSTR, sCON, sDEX, sVOL, sWIS;
	SQLSMALLINT sBasicSTR, sBasicCON, sBasicVOL, sBasicDEX, sBasicWIS;
	SQLUINTEGER	iSkin, iHair, iExp, iXP, iDN;
	SQLINTEGER iCityValue, iGuild;
	SQLSMALLINT	sGender;
	SQLCHAR		strFace[10];
	SQLSMALLINT	sLZ, sLX, sLY, sPA;
	SQLUINTEGER iBuddy;
	SQLSMALLINT	sMaxHP, sHP, sMaxPP, sPP, sMaxSP, sSP, sCityRank, sKillCount;
	SQLSMALLINT sLevel;
	SQLCHAR		byClass, bLive, bLock;
	
	SQLSMALLINT	sSkillPoint;

	SQLCHAR		strSkill[_SKILL_DB], strItem[_ITEM_DB], strPsi[_PSI_DB], strHaveEvent[_EVENT_DB], strTel[_TEL_DB];
	
	SQLCHAR		strQuickItem[_QUICKITEM_DB];	// Äü ¾ÆÀÌÅÛÀº ´õÀÌ»ó ÀúÀåÇÏÁö ¾Ê´Â´Ù.

	SQLCHAR		tAbnormalKind;
	SQLUINTEGER iAbnormalTime;

	//--yskang 0.1
	SQLCHAR		strDbLoveName[LOVE_NAME_LENGTH];

	SQLSMALLINT	sChangeClass[CLASS_NUM];

	SQLCHAR		tIsOP;

	SQLUINTEGER iSaintTime;		// ¼¼ÀÎÆ® ÇÇÄÉÀÌ Ä«¿îÆ® ÁÙÀÌ±â À§ÇØ
	SQLINTEGER	sInd;

	SQLUINTEGER iHiExpTime, iMagicFindTime, iNoChatTime, iBFindTime, iVIPTime, iCloseTime, iShopPingDN, iGunTianShi, iJiFen, iChouJiang, iRenWu,iCKMiMa, iHuanJing, iHuanLing, iZaiXianTime, iPKAddkill, iFUExpTime, iJiaGong, iJiaFangYu, iJiaMoFang, iJiaGongJi,iJiaXueLiang,iJiaMoney;			//Êý¾Ý¿âÀïµÄ

	SQLCHAR		tPsiOneKind, tPsiTwoKind, tPsiThreeKind;			// Haste, Shield, Dex Up, Max HP Up, Fast Run, Mind Shock, Psi Shield, Piercing Shield
	SQLUINTEGER	iPsiOneTime, iPsiTwoTime, iPsiThreeTime;			// Adamantine, MightyWeapon, Berserker
																	// Mind Guard

	sSTR = sCON = sDEX = sVOL = sWIS = 0;
	sBasicSTR = sBasicCON = sBasicVOL = sBasicWIS = sBasicDEX = 0;
	sLZ = sLX = sLY = 1;
	sPA = 0;
	iBuddy = iGuild = 0;
	iExp = iXP = iDN = iCityValue = sKillCount = 0;
	sMaxHP = sHP = sMaxPP = sPP = sMaxSP = sSP = 0;
	sCityRank = 0;
	sLevel = 1;
	byClass = tIsOP = bLive = bLock = 0;

	tAbnormalKind = 0;
	iAbnormalTime = 0;

	iSaintTime = 0;

	iHiExpTime = iMagicFindTime = iNoChatTime = iBFindTime = iVIPTime = iCloseTime = iShopPingDN = iGunTianShi = iJiFen = iChouJiang = iRenWu = iCKMiMa = iHuanJing = iHuanLing = iZaiXianTime = iPKAddkill = iFUExpTime =iJiaGong = iJiaFangYu = iJiaMoFang = iJiaGongJi = iJiaXueLiang = iJiaMoney = 0;//Êý¾Ý¿âÀïµÄ

	tPsiOneKind = tPsiTwoKind = tPsiThreeKind = 0;
	iPsiOneTime = iPsiTwoTime = iPsiThreeTime = 0;

	sSkillPoint = 0;

	::ZeroMemory(strFace, sizeof(strFace));
	::ZeroMemory(strSkill, sizeof(strSkill));
//	::ZeroMemory(strItem, sizeof(strItem));
	::ZeroMemory(strPsi, sizeof(strPsi));
	::ZeroMemory(strTel, sizeof(strTel));
	::ZeroMemory(strHaveEvent, sizeof(strHaveEvent));
	
	::ZeroMemory(strDbLoveName,sizeof(strDbLoveName));

	memset(strItem, -1, sizeof(strItem));
	memset(strQuickItem, -1, sizeof(strQuickItem));		// ÇöÀç Äü¾ÆÀÌÅÛÀº »ç¿ëÇÏÁö ¾Ê´Â´Ù. ³ªÁß¿¡ ´Ù¸¥¿ëµµ·Î »ç¿ëÇÏ·Á°í µðºñ¿¡¼­ Áö¿ìÁø ¾ÊÀ½

	for(i = 0; i < TOTAL_PSI_NUM; i++)					// ÃÊ±âÈ­¸¦ ²À ÇØ¾ß ÇÑ´Ù.
	{
		m_UserPsi[i].sSid = -1;
		m_UserPsi[i].tOnOff = FALSE;
	}

	int db_index = 0;
	CDatabase* pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		TRACE("Fail To Load User Data !!\n");
//		g_DB[m_iModSid].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, SQL_NTS);
	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		retcode = SQLFetch( hstmt );

		if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
		{
			int i = 1;
			SQLGetData( hstmt, i++, SQL_C_SSHORT, &sSTR, sizeof(SQLSMALLINT), &sInd );
			SQLGetData( hstmt, i++, SQL_C_SSHORT, &sCON, sizeof(SQLSMALLINT), &sInd );
			SQLGetData( hstmt, i++, SQL_C_SSHORT, &sDEX, sizeof(SQLSMALLINT), &sInd );
			SQLGetData( hstmt, i++, SQL_C_SSHORT, &sVOL, sizeof(SQLSMALLINT), &sInd );
			SQLGetData( hstmt, i++, SQL_C_SSHORT, &sWIS, sizeof(SQLSMALLINT), &sInd );

			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iSkin,		sizeof(iSkin),		&sInd );
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iHair,		sizeof(iHair),		&sInd );
			SQLGetData( hstmt, i++, SQL_C_SSHORT,	&sGender,	sizeof(sGender),	&sInd );

			SQLGetData( hstmt, i++, SQL_C_BINARY,	strFace,	sizeof(strFace),	&sInd );

			SQLGetData( hstmt, i++, SQL_C_SSHORT,	&sLZ,		sizeof(sLZ),	&sInd );
			SQLGetData( hstmt, i++, SQL_C_SSHORT,	&sLX,		sizeof(sLX),	&sInd );
			SQLGetData( hstmt, i++, SQL_C_SSHORT,	&sLY,		sizeof(sLY),	&sInd );

			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iBuddy,	sizeof(iBuddy),	&sInd );
			SQLGetData( hstmt, i++, SQL_C_SLONG,	&iGuild,	sizeof(iGuild),	&sInd );

			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iExp,		sizeof(iExp),	&sInd );

			SQLGetData( hstmt, i++, SQL_C_SSHORT,	&sPA,		sizeof(sPA),	&sInd );

			SQLGetData( hstmt, i++, SQL_C_SSHORT,	&sSkillPoint,	sizeof(sSkillPoint),	&sInd );

			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iXP,		sizeof(iXP),	&sInd );

			SQLGetData( hstmt, i++, SQL_C_SSHORT,	&sMaxHP,	sizeof(sMaxHP),	&sInd );
			SQLGetData( hstmt, i++, SQL_C_SSHORT,	&sHP,		sizeof(sHP),	&sInd );
			SQLGetData( hstmt, i++, SQL_C_SSHORT,	&sMaxPP,	sizeof(sMaxPP),	&sInd );
			SQLGetData( hstmt, i++, SQL_C_SSHORT,	&sPP,		sizeof(sPP),	&sInd );
			SQLGetData( hstmt, i++, SQL_C_SSHORT,	&sMaxSP,	sizeof(sMaxSP),	&sInd );
			SQLGetData( hstmt, i++, SQL_C_SSHORT,	&sSP,		sizeof(sSP),	&sInd );

			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iDN,		sizeof(iDN),		&sInd );
			SQLGetData( hstmt, i++, SQL_C_SSHORT,	&sCityRank,	sizeof(sCityRank),	&sInd );

			SQLGetData( hstmt, i++, SQL_C_SSHORT,	&sLevel,	sizeof(sLevel),		&sInd );
			SQLGetData( hstmt, i++, SQL_C_TINYINT,	&byClass,	sizeof(byClass),	&sInd );

			SQLGetData( hstmt, i++, SQL_C_BINARY,	strSkill,	sizeof(strSkill),	&sInd );
			SQLGetData( hstmt, i++, SQL_C_BINARY,	strPsi,		sizeof(strPsi),		&sInd );
			SQLGetData( hstmt, i++, SQL_C_BINARY,	strItem,	sizeof(strItem),	&sInd );

			SQLGetData( hstmt, i++, SQL_C_SSHORT,	&sChangeClass[0],	sizeof(sChangeClass[0]),	&sInd );
			SQLGetData( hstmt, i++, SQL_C_SSHORT,	&sChangeClass[1],	sizeof(sChangeClass[1]),	&sInd );
			SQLGetData( hstmt, i++, SQL_C_SSHORT,	&sChangeClass[2],	sizeof(sChangeClass[2]),	&sInd );
			SQLGetData( hstmt, i++, SQL_C_SSHORT,	&sChangeClass[3],	sizeof(sChangeClass[3]),	&sInd );

			SQLGetData( hstmt, i++, SQL_C_TINYINT,	&tAbnormalKind,	sizeof(tAbnormalKind),	&sInd );
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iAbnormalTime,	sizeof(iAbnormalTime),	&sInd );

			SQLGetData( hstmt, i++, SQL_C_BINARY,	strQuickItem,	sizeof(strQuickItem),	&sInd );

			SQLGetData( hstmt, i++, SQL_C_TINYINT,	&tIsOP,			sizeof(tIsOP),	&sInd );

			SQLGetData( hstmt, i++, SQL_C_TINYINT,	&bLive,		sizeof(bLive),		&sInd );
			SQLGetData( hstmt, i++, SQL_C_TINYINT,	&bLock,		sizeof(bLock),		&sInd );

			SQLGetData( hstmt, i++, SQL_C_SSHORT, &sBasicSTR, sizeof(SQLSMALLINT), &sInd );
			SQLGetData( hstmt, i++, SQL_C_SSHORT, &sBasicCON, sizeof(SQLSMALLINT), &sInd );
			SQLGetData( hstmt, i++, SQL_C_SSHORT, &sBasicDEX, sizeof(SQLSMALLINT), &sInd );
			SQLGetData( hstmt, i++, SQL_C_SSHORT, &sBasicVOL, sizeof(SQLSMALLINT), &sInd );
			SQLGetData( hstmt, i++, SQL_C_SSHORT, &sBasicWIS, sizeof(SQLSMALLINT), &sInd );
			SQLGetData( hstmt, i++, SQL_C_SLONG,  &iCityValue,sizeof(iCityValue),&sInd );

			SQLGetData( hstmt, i++, SQL_C_BINARY, strHaveEvent,	sizeof(strHaveEvent), &sInd );
			SQLGetData( hstmt, i++, SQL_C_SSHORT, &sKillCount,	sizeof(sKillCount),	&sInd );
			SQLGetData( hstmt, i++, SQL_C_BINARY, strTel,		sizeof(strTel), &sInd );
			
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iSaintTime,sizeof(iSaintTime),	&sInd );

			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iHiExpTime,		sizeof(iHiExpTime),		&sInd );
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iMagicFindTime,	sizeof(iMagicFindTime),	&sInd );
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iNoChatTime,		sizeof(iNoChatTime),	&sInd );
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iBFindTime,		sizeof(iBFindTime),		&sInd );
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iVIPTime,		    sizeof(iVIPTime),		&sInd );
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iCloseTime,		sizeof(iCloseTime),		&sInd );
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iShopPingDN,		sizeof(iShopPingDN),	&sInd );
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iGunTianShi,		sizeof(iGunTianShi),	&sInd );
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iJiFen,			sizeof(iJiFen),			&sInd );
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iChouJiang,		sizeof(iChouJiang),		&sInd );
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iRenWu,			sizeof(iRenWu),			&sInd );
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iCKMiMa,			sizeof(iCKMiMa),		&sInd );
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iHuanJing,			sizeof(iHuanJing),		&sInd );
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iHuanLing,			sizeof(iHuanLing),		&sInd );
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iZaiXianTime,		sizeof(iZaiXianTime),	&sInd );
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iPKAddkill,		sizeof(iPKAddkill),		&sInd );
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iFUExpTime,		sizeof(iFUExpTime),		&sInd );
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iJiaGong,		    sizeof(iJiaGong),	&sInd );//Ð¡ºÚÐÞÁ¶
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iJiaFangYu,		sizeof(iJiaFangYu),	&sInd );
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iJiaMoFang,		sizeof(iJiaMoFang),	&sInd );
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iJiaGongJi,		sizeof(iJiaGongJi),	&sInd );//Ð¡ºÚÐÞÁ¶
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iJiaXueLiang,		sizeof(iJiaXueLiang),	&sInd );
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iJiaMoney,		sizeof(iJiaMoney),	&sInd );

			SQLGetData( hstmt, i++, SQL_C_CHAR,	strDbLoveName,sizeof(strDbLoveName),	&sInd );	//Êý¾Ý¿âÀïµÄ

			SQLGetData( hstmt, i++, SQL_C_TINYINT,	&tPsiOneKind,	sizeof(tPsiOneKind),	&sInd );
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iPsiOneTime,	sizeof(iPsiOneTime),	&sInd );			

			SQLGetData( hstmt, i++, SQL_C_TINYINT,	&tPsiTwoKind,	sizeof(tPsiTwoKind),	&sInd );
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iPsiTwoTime,	sizeof(iPsiTwoTime),	&sInd );			

			SQLGetData( hstmt, i++, SQL_C_TINYINT,	&tPsiThreeKind,	sizeof(tPsiThreeKind),	&sInd );
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iPsiThreeTime,	sizeof(iPsiThreeTime),	&sInd );	
	
		}
		else
		{
			DisplayErrorMsg(hstmt);
			retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		//	BREAKPOINT();

			g_DB[m_iModSid].ReleaseDB(db_index);
			return FALSE;
		}
	}
	else
	{
		DisplayErrorMsg(hstmt);
		retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		BREAKPOINT();

		g_DB[m_iModSid].ReleaseDB(db_index);
		return FALSE;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(db_index);

	// Copy User Data
	strcpy(m_strUserID, szID);	// User ID

	ZeroMemory(m_strLoveName,sizeof(m_strLoveName));

	if(strlen((char *)strDbLoveName) < 1)
	{
		m_bPseudoString = TRUE;
		strcpy(m_strLoveName,"");
	}
	else
	{
		if(m_iDisplayType != 5 && m_iDisplayType != 6)
		{
			strcpy(m_strLoveName,(char *)strDbLoveName);
		}
		else strcpy(m_strLoveName,"");
	}

	m_sSTR = sSTR;				// Èû
	m_sCON = sCON;				// °Ç°­
	m_sDEX = sDEX;				// ¹ÎÃ¸
	m_sVOL = sVOL;				// ÀÇÁö
	m_sWIS = sWIS;				// ÁöÇý

	m_sTempSTR = (BYTE)sBasicSTR;		// ÃÊ±â ·Ñ°ª¸¦ ¹Þ¾Æ¿Â´Ù.(·¹º§´Ù¿îÀÏ¶§ ¾¸)
	m_sTempCON = (BYTE)sBasicCON;
	m_sTempDEX = (BYTE)sBasicDEX;
	m_sTempVOL = (BYTE)sBasicVOL;
	m_sTempWIS = (BYTE)sBasicWIS;

	m_iSkin = iSkin;			// ÇÇºÎ»ö
	m_iHair = iHair;			// ¸Ó¸®»ö

	m_curz = sLZ;				// Zone
	m_curx = sLX;				// À§Ä¡ x
	m_cury = sLY;				// À§Ä¡ y

	m_sGender = sGender;		// ¼ºº°

	::CopyMemory(m_strFace, strFace, 10);	// ¸Ó¸®¸ð¾ç

	m_dwBuddy = iBuddy;			// Buddy
	m_dwGuild = iGuild;			// Guild
	m_tFortressWar = GUILD_WAR_AFFTER;
	m_tGuildWar = GUILD_WAR_AFFTER;
	m_tGuildHouseWar = GUILD_WAR_AFFTER;
	m_tQuestWar = GUILD_WAR_AFFTER;

	if( m_dwGuild >= 1 )
	{
		if( !GetAbsentGuildInfo( (int)m_dwGuild ) )
		{
			m_dwGuild = -1;
		}
	}

	if(m_dwGuild >= 1)			// ±æµå¿¡ ´ëÇÑ µ¥ÀÌÅÍ¸¦ ÃÊ±âÈ­ÇÑ´Ù.
	{
		SetGuildVariable();
	}

	CheckGuildWar();			// ¸¸¾à °ø¼ºÀü ÁßÀÌ¶ó¸é ¸ðµç À¯Àú¿¡°Ô ¾Ë¸°´Ù.(Áö±ÝÀº °ø¼ºÀü¸¸)

	m_bFieldWarApply = FALSE;
	m_FieldWarGMUid = 0;		// ÇÊµåÀüÀÏ¶§ »ó´ë¹æ ±æ¸¶ uid
	m_dwFieldWar = 0;			// ÇÊµåÀü ½ÃÀÛÀÌ¸é »ó´ëÆí ±æµå ¹øÈ£°¡ µé¾î¿Â´Ù.

	m_dwExp	  = iExp;			// Experience

	m_sPA = sPA;				// PA Point

	m_sSkillPoint = sSkillPoint;// Skill Point

	m_dwXP = iXP;				// X Point
	
	m_sLevel	= sLevel;		// ·¹º§

	if(iExp > GetNextLevel(sLevel)) iExp = GetNextLevel(sLevel);	// ¾ÈÀü ÄÚµå.
	m_byClass	= byClass;		// Å¬·¡½º

	m_sMaxHP	= g_sHPConst[m_byClass] * m_sCON + g_sHPLV[m_byClass] * (m_sLevel - 1) + g_sHPAdd[m_byClass];
	m_sMaxPP	= g_sPPConst[m_byClass] * m_sWIS + g_sPPLV[m_byClass] * (m_sLevel - 1) + g_sPPAdd[m_byClass];
	m_sMaxSP	= g_sSPConst[m_byClass] * m_sCON + g_sSPLV[m_byClass] * (m_sLevel - 1) + g_sSPAdd[m_byClass];

	m_bLive		= bLive;		// Á×¾ú´ÂÁö, »ì¾Ò´ÂÁö?
	m_bLock		= bLock;

	m_sHP		= sHP;			// ÇöÀç HP
	m_sPP		= sPP;			// ÇöÀç PP
	m_sSP		= sSP;			// ÇöÀç SP

	if(m_sHP <= 0 || m_bLive == USER_DEAD) 
	{
		m_bLive = USER_DEAD;
		SetZoneIndex(m_curz);
		IsDeadUser();		// Á×Àº À¯Àú¸é ¸¶À»·Î...

		m_bLive = USER_LIVE;
		m_sHP = m_sMaxHP;
	}
	else SetZoneIndex(m_curz);

	if(iDN < 0) iDN = 0;

	m_dwDN		= iDN;			// ¼ÒÁö±Ý

	m_iCityValue = iCityValue;	// ½Ã¹Î ´©Àû°ª
	m_sKillCount = sKillCount;	// PK´©Àû È½¼ö

	for(i = 0; i < 11; i++)
	{
		if(m_iCityValue < g_CityRankValue[i])// ´©ÀûÄ¡°ªÀÌ ±âÁØ ¼³Á¤°ªº¸´Ù ÀÛÀºÁö ÆÇ´ÜÇÑ´Ù.
		{
			if(i < 6) { m_sCityRank = i - CITY_RANK_INTERVAL; }
			else { m_sCityRank = i -1 - CITY_RANK_INTERVAL; }
			break;
		}
	}

	if(m_iCityValue >= g_CityRankValue[10]) m_sCityRank = CITY_RANK_INTERVAL;

	::CopyMemory(m_strSkill, strSkill, sizeof(m_strSkill));		// ½ºÅ³
	::CopyMemory(m_strItem, strItem, sizeof(m_strItem));		// ¾ÆÀÌÅÛ
	::CopyMemory(m_strPsi, strPsi, sizeof(m_strPsi));			// »çÀÌ¿À´Ð
//SaveDump((char*)strItem,sizeof(strItem));
	StrToUserSkill((LPTSTR)strSkill);
	StrToUserItem((LPTSTR)strItem);
	StrToUserPsi((LPTSTR)strPsi);
	StrToHaveEventData((LPTSTR)strHaveEvent);
	StrToUserTel((LPSTR)strTel);

	m_dwExpNext	= GetNextLevel(m_sLevel);						// ´ÙÀ½·¹º§ÀÌ µÇ±âÀ§ÇÑ °æÇèÄ¡

	for(i = 0; i < TOTAL_SKILL_NUM; i++)						// ½ºÅ³ ¼º°øÀ² 
	{
		GetSkillInfo(i, m_UserSkillInfo[i]);
	}
//	m_UserChangeSkillInfo;										// ÀüÁ÷ÀÌ ÀÖÀ»°æ¿ì ?(5)½ºÅ³¸¦ ÀúÀå

	for(i = 0; i < CLASS_NUM; i++)
	{
		m_sChangeClass[i] = sChangeClass[i];
	}

	m_tAbnormalKind = tAbnormalKind;							// »óÅÂÀÌ»ó Á¤º¸
	m_dwAbnormalTime = iAbnormalTime;
	m_dwLastAbnormalTime = GetTickCount();

	// Quick Item Á¤º¸´Â Áö±Ý ¾²Áö ¾Ê´Â´Ù.
//	int index = 0;		
//	for(i = TOTAL_ITEM_NUM; i < TOTAL_ITEM_SLOT_NUM; i++)
//	{
//		m_UserItem[i].sSid = GetShort((char*)strQuickItem, index);
//	}

//	dwMaxDN = m_sLevel * MAX_LEVEL_DN;									// ¼ÒÁö±ÝÀÌ ÀÌ»óÇÑÁö Ã¼Å©
//	if( m_dwDN > dwMaxDN ) m_dwDN = dwMaxDN; 
	m_tDir = myrand(0, 7);												// ÇöÀç º¸°íÀÖ´Â ¹æÇâÀ» ·¥´ýÀ¸·Î ÃÊ±âÈ­ÇÑ´Ù.
		
	m_tIsOP = tIsOP;
	if(m_tIsOP == 255)//yskang debug µðºñ¿¡ ¿øÀÎ ¸ð¸¦ Çö»óÀ¸·Î 255°¡ µé¾î°¡°í ÀÖÀ½ ÀâÀ»¶§ ±îÁö ÄÚµå À¯Áö
		m_tIsOP = 0;
	if(tIsOP == 1) AddAbnormalInfo(OPERATION_MODE);						// ±âº»ÀÌ Åõ¸í¸ðµåÀÓ...

	CheckInvalidGuildZone();

	DWORD dwCurrTime = GetTickCount();
	m_dwLastSpeedTime = dwCurrTime;
	m_dwLastMoveAndRun = dwCurrTime;
	m_dwLastAttackTime = dwCurrTime;
	m_dwLastPsiAttack = dwCurrTime;
	m_dwLastPsiDamagedTime = 0;
	m_dwCastDelay = 0;
	m_dwLastTimeCount = dwCurrTime;
	m_dwLastAbnormalTime = dwCurrTime;
	m_dwLastHPTime = dwCurrTime;

	m_dwLastHasteTime = dwCurrTime;
	m_dwLastShieldTime = dwCurrTime;
	m_dwLastDexUpTime = dwCurrTime;
	m_dwLastMaxHPUpTime = dwCurrTime;

	m_dwNoDamageTime = 0;
	m_dwLastNoDamageTime = dwCurrTime;

	m_dwSaintTime = iSaintTime;		// ¼¼ÀÎÆ® ÇÇÄÉÀÌ Ä«¿îÆ® ÁÙ¿©ÁÖ±â

	m_dwLastHiExpTime		= dwCurrTime;	
	m_dwLastMagicFindTime	= dwCurrTime;
	m_dwLastNoChatTime		= dwCurrTime;
	m_dwLastBFindTime		= dwCurrTime;
	m_dwLastVIPTime			= dwCurrTime;
	m_dwLastCKMiMa			= dwCurrTime;
	m_dwLastCloseTime		= dwCurrTime;
	m_dwLastHuanJing        = dwCurrTime;
	m_dwLastHuanLing		= dwCurrTime;

	m_dwHiExpTime		= iHiExpTime;
	m_dwMagicFindTime	= iMagicFindTime;
	m_dwNoChatTime		= iNoChatTime;
	m_dwBFindTime		= iBFindTime;
	m_dwVIPTime			= iVIPTime;			
	m_dwCloseTime		= iCloseTime;
	m_dwShopPingDN		= iShopPingDN;
	m_dwGuarDianTianShi	= iGunTianShi;
	m_dwJiFen			= iJiFen;
	m_dwChouJiang		= iChouJiang;
	m_dwRenWu			= iRenWu;
	m_dwCKMiMa			= iCKMiMa;
	m_dwHuanJing		= iHuanJing;
	m_dwHuanLing		= iHuanLing;
	m_dwZaiXianTime		= iZaiXianTime;
	m_dwPKAddkill       = iPKAddkill;
	m_dwFUExpTime		= iFUExpTime;
	m_dwJiaGong         = iJiaGong;//Ð¡ºÚÐÞÁ¶¹¥»÷
	m_dwJiaFangYu       = iJiaFangYu;//Ð¡ºÚÐÞÁ¶·ÀÓù
	m_dwJiaMoFang       = iJiaMoFang;//Ð¡ºÚÐÞÁ¶Ä§·À
	m_dwJiaGongJi		= iJiaGongJi; //Ð¡ºÚÐÞÁ¶
	m_dwJiaXueLiang     = iJiaXueLiang;//Ð¡ºÚÐÞÁ¶
	m_dwJiaMoney        = iJiaMoney;//Ð¡ºÚÐÞÁ¶
	m_dwLastAdamantineTime		= dwCurrTime;	
	m_dwLastMightyWeaponTime	= dwCurrTime;
	m_dwLastBerserkerTime		= dwCurrTime;

	m_dwLastFastRunTime			= dwCurrTime;
	m_dwLastMindShockTime		= dwCurrTime;
	m_dwLastPsiShieldTime		= dwCurrTime;
	m_dwLastPiercingShieldTime	= dwCurrTime;

	m_dwLastHuanJing		    = dwCurrTime;
	m_dwLastHuanLing		    = dwCurrTime;

	m_dwLastFUExpTime			= dwCurrTime;

	m_dwLastMindGuardTime		= dwCurrTime;

	SetPsiOne(tPsiOneKind, iPsiOneTime);
	SetPsiTwo(tPsiTwoKind, iPsiTwoTime);
	SetPsiThree(tPsiThreeKind, iPsiThreeTime);
	
	SetPsiAbnormalStatus();
	SetStatus();//Ë¢ÐÂ

	GetRecoverySpeed();													// À¯Àú Å¬·¡½ºÀÇ È¸º¹¼Óµµ¸¦ °áÁ¤ÇÑ´Ù.

	return TRUE;
}
///////////////////////////////////////////////////////////////////////
// ÔÚÏß³äÖµ
BOOL USER::LoadVIPPayData()
{
	int i;
	for(i = 0; i < g_arVIPPayTable.GetSize(); i++) g_arVIPPayTable.RemoveAll();
	SQLHSTMT		hstmt;
	SQLRETURN		retcode;
	TCHAR			szSQL[2048];
	::ZeroMemory(szSQL, sizeof(szSQL));
	wsprintf(szSQL,TEXT("SELECT * FROM VIPPay"));
	SQLUINTEGER		iSid;
	SQLCHAR			iSname[20];
	SQLUINTEGER		Formal;
	SQLSMALLINT		flag;
	SQLINTEGER		sInd;
	::ZeroMemory(iSname, sizeof(iSname));
	iSid = 0; Formal = 0;
	flag = 0;
	int db_index = 0;
	CDatabase* pDB = g_DB[AUTOMATA_THREAD].GetDB( db_index );
	if( !pDB ) return FALSE;
	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		TRACE("Fail To Load VIPPay Data !!\n");
		AfxMessageBox(_T("VIPPay Table Open Fail!"));
		//g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
		return FALSE;
	}
	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, SQL_NTS);
	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		while (TRUE)
		{
			retcode = SQLFetch(hstmt);

			if (retcode ==SQL_SUCCESS || retcode ==SQL_SUCCESS_WITH_INFO)
			{
				i = 1;
				SQLGetData( hstmt, i++, SQL_C_ULONG, &iSid, sizeof(iSid), &sInd );
				SQLGetData( hstmt, i++, SQL_C_CHAR,	 &iSname, sizeof(iSname), &sInd );
				SQLGetData( hstmt, i++, SQL_C_ULONG, &Formal, sizeof(Formal), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &flag, sizeof(flag), &sInd );
				VIPPay *OSP = new VIPPay;
				OSP->m_iSid = iSid;
				strcpy(OSP->m_iSname, (CHAR*)iSname);
				OSP->m_Formal = Formal;
				OSP->m_flag = flag;
				g_arVIPPayTable.Add(OSP);
			}
			else break;
		}		
	}
	else if (retcode==SQL_NO_DATA)
	{
		g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
		return FALSE;
	}
	else
	{
		retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
		return FALSE;
	}
	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
	return TRUE;
}
///////////////////////////////////////////////////////////////////////
// ÔÚÏß³äÖµ
BOOL USER::LoadMarkPayData()
{
	int i;
	for(i = 0; i < g_arMarkPayTable.GetSize(); i++) g_arMarkPayTable.RemoveAll();
	SQLHSTMT		hstmt;
	SQLRETURN		retcode;
	TCHAR			szSQL[2048];
	::ZeroMemory(szSQL, sizeof(szSQL));
	wsprintf(szSQL,TEXT("SELECT * FROM MarkPay"));
	SQLUINTEGER		iSid;
	SQLCHAR			iSname[20];
	SQLUINTEGER		Formal;
	SQLSMALLINT		flag;
	SQLINTEGER		sInd;
	::ZeroMemory(iSname, sizeof(iSname));
	iSid = 0; Formal = 0;
	flag = 0;
	int db_index = 0;
	CDatabase* pDB = g_DB[AUTOMATA_THREAD].GetDB( db_index );
	if( !pDB ) return FALSE;
	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		TRACE("Fail To Load UserSort Data !!\n");
		AfxMessageBox(_T("MarkPay Table Open Fail!"));
		//g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
		return FALSE;
	}
	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, SQL_NTS);
	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		while (TRUE)
		{
			retcode = SQLFetch(hstmt);

			if (retcode ==SQL_SUCCESS || retcode ==SQL_SUCCESS_WITH_INFO)
			{
				i = 1;
				SQLGetData( hstmt, i++, SQL_C_ULONG, &iSid, sizeof(iSid), &sInd );
				SQLGetData( hstmt, i++, SQL_C_CHAR,	 &iSname, sizeof(iSname), &sInd );
				SQLGetData( hstmt, i++, SQL_C_ULONG, &Formal, sizeof(Formal), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &flag, sizeof(flag), &sInd );
				MarkPay *OSP = new MarkPay;
				OSP->m_iSid = iSid;
				strcpy(OSP->m_iSname, (CHAR*)iSname);
				OSP->m_Formal = Formal;
				OSP->m_flag = flag;
				g_arMarkPayTable.Add(OSP);
			}
			else break;
		}		
	}
	else if (retcode==SQL_NO_DATA)
	{
		g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
		return FALSE;
	}
	else
	{
		retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
		return FALSE;
	}
	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[AUTOMATA_THREAD].ReleaseDB(db_index);
	return TRUE;
}
///////////////////////////////////////////////////////////////////////
//	ÇØ´ç À¯Àú°¡ ½ÇÇàÇÑ ÀÌº¥Æ®¸¦ ÀúÀåÇÑ´Ù.
//
void USER::StrToHaveEventData(TCHAR *pBuf)
{
	int index = 0;
	int eventnum;

	InitEventSlot();

	if( !pBuf[0] ) return;

	short HaveEventNum = GetShort( pBuf, index );

	if(HaveEventNum > MAX_EVENT_NUM) HaveEventNum = MAX_EVENT_NUM;

	int* pEventNum;

	for(int i = 0; i < HaveEventNum; i++)
	{
		eventnum = GetShort( pBuf, index );

		pEventNum = new int;

		*(pEventNum) = eventnum;

		m_arEventNum.Add( pEventNum );
	}
}

///////////////////////////////////////////////////////////////////////
//	ÇØ´ç À¯Àú°¡ ½ÇÇàÇÑ ÀÌº¥Æ®¸¦ DB¿¡ ÀúÀåÇÑ´Ù.
//
void USER::UserHaveEventDataToStr(TCHAR *pBuf)
{
	int index = 0;
	int num = m_arEventNum.GetSize();

	if(num >= MAX_EVENT_NUM) num = MAX_EVENT_NUM;

	SetShort( pBuf, num, index );

	int temp_int;

	for( int i = 0; i < num; i++)
	{
		temp_int = *(m_arEventNum[i]);

		SetShort( pBuf, temp_int, index );
	}
}

///////////////////////////////////////////////////////////////////////
//	ÇØ´ç À¯Àú¿¡ ±æµå°¡ ÀÖ´Ù¸é ±æµå¿¡ ÇÊ¿äÇÑ º¯¼ö¸¦ ÃÊ±âÈ­ÇÑ´Ù.
//
void USER::SetGuildVariable()
{	
	int nLen = 0;
	int iUserIndex = -1;
	CGuild *pGuild = NULL;
//	CGuildUser *pGuildUser = NULL;

	pGuild = GetGuild(m_dwGuild);

	m_tGuildWar = GUILD_WAR_AFFTER;
	m_tFortressWar = GUILD_WAR_AFFTER;

	if(!pGuild)	m_dwGuild = -1;									// ±æµå¿øÀÌ¶ó¸é
	else
	{
		iUserIndex = pGuild->GetUser(m_strUserID);

		if(iUserIndex < 0) m_dwGuild = -1;
		else													// ½ÇÁ¦ ±æµå¿¡ ¼ÓÇÏ¸é
		{
			m_bRepresentationGuild = FALSE;						// À§ÀÓ NO! 

			if(strcmp(pGuild->m_strMasterName, m_strUserID) == 0) m_bGuildMaster = TRUE; //±æ¸¶ÀÎÁö Ã¼Å©
			else if(pGuild->m_lSubMaster == 1)
			{
				if(strcmp(pGuild->m_strSubMasterName, m_strUserID) == 0) m_bRepresentationGuild = TRUE; //±æ¸¶ÀÎÁö Ã¼Å©
			}
			
			if(strcmp(g_arGuildData[m_dwGuild]->m_strUsedUser, m_strUserID) == 0)		// ÇØ´ç ±æµåÃ¢°í¸¦ ÀÌ¿ëÇÏ´ÂÁö Ã¼Å©
			{
				g_arGuildData[m_dwGuild]->m_iUsedUser = -1;
				::ZeroMemory(g_arGuildData[m_dwGuild]->m_strUsedUser, sizeof(g_arGuildData[m_dwGuild]->m_strUsedUser));
				InterlockedExchange(&g_arGuildData[m_dwGuild]->m_lUsed, (LONG)0); 
			}
			
			m_sGuildVersion = pGuild->m_sVersion;				// ±æµå¹öÁ¯ 
			strcpy(m_strGuildName, pGuild->m_strGuildName);		// ±æµåÀÌ¸§
		}
	}

	ReleaseGuild();			// ÇØÁ¦...	
}

///////////////////////////////////////////////////////////////////////
//	Ã³À½ °ÔÀÓ¿¡ Á¢¼ÓÇÒ¶§ ÇØ´ç ±æµå°¡ ±æÀüÀ» ÇÏ¸é ÀÚµ¿À¸·Î Âü°¡ÇÏµµ·ÏÇÑ´Ù.
//
//  Áö±ÝÀº °ø¼ºÀü¸¸ ÇÏµµ·Ï... 
void USER::CheckGuildWar()
{
	int i, j;
	short nCount = 1;

	BOOL bWar = FALSE;

	CGuildFortress *pFort = NULL;

	for(i = 0; i < g_arGuildFortress.GetSize(); i++)
	{
		pFort = g_arGuildFortress[i];

		if(!pFort) continue;

		bWar = FALSE;
		nCount = 1;
		CBufferEx TempBuf, TempBufData;

		if(pFort->m_lUsed == 1)									// ÇØ´ç Áö¿ªÀÌ ±æÀü ÁßÀÌ¸é...
		{
			if(m_dwGuild >= 1)
			{
				if(pFort->m_iGuildSid == m_dwGuild) 
				{
					bWar = TRUE;
					BeginFortressWar();	
				}
				else
				{
					for(j =0; j < GUILDFORTRESS_ATTACK_MAX_NUM; j++)
					{
						if(pFort->m_arAttackGuild[j].lGuild <= 0) continue;

						if(pFort->m_arAttackGuild[j].lGuild == m_dwGuild)
						{
							bWar = TRUE;
							BeginFortressWar();			// ±æµå ÀüÀ» ½ÃÀÛÇÑ´Ù.
							break;
						}
					}
				}

				TempBufData.Add(pFort->m_iGuildSid);			// ¹æ¾î ±æµå			

				for(j =0; j < GUILDFORTRESS_ATTACK_MAX_NUM; j++)		// °ø°Ý ±æµå...
				{
					if(pFort->m_arAttackGuild[j].lGuild > 0)
					{
						TempBufData.Add(pFort->m_arAttackGuild[j].lGuild);
						nCount++;
					}
				}
			}

			TempBuf.Add(GUILD_WAR);
			TempBuf.Add((BYTE)0x01);						// ±æµåÀü ½ÃÀÛ...
			TempBuf.Add((BYTE)GUILD_FOTRESS_WARRING);		// °ø¼ºÀü...
			TempBuf.Add((short)pFort->m_sFortressID);

			if(bWar)
			{
				TempBuf.Add((BYTE)0x01);
				TempBuf.Add(nCount);							// ÃÑ ±æµå¼ö
				TempBuf.AddData(TempBufData, TempBufData.GetLength());
			}
			else TempBuf.Add((BYTE)0x00);

			Send(TempBuf, TempBuf.GetLength());
		}
	}
}

///////////////////////////////////////////////////////////////////////
//	À¯Àú Å¬·¡½º¿¡ ¾Ë¸Â´Â È¸º¹ ¼Óµµ¸¦ ±¸ÇÑ´Ù.
//
void USER::GetRecoverySpeed()
{
	int weight = 0;

	m_iHPIntervalTime = 0;		// HP È¸º¹ ½Ã°£ °£°Ý
	m_iSPIntervalTime = 0;		// SP È¸º¹ ½Ã°£ °£°Ý
	m_iPPIntervalTime = 0;		// PP È¸º¹ ½Ã°£ °£°Ý

	m_iHPRate = 1;
	m_iPPRate = 1;
	m_iSPRate = 1;

	m_iHPIntervalTime = 15000;	// Init default	
	m_iSPIntervalTime = 15000;
	m_iPPIntervalTime = 15000;

	if(CheckRecoverTableByClass())
	{
		m_iHPRate = g_arRecoverTable[m_byClass]->m_byHPDivide;
		m_iPPRate = g_arRecoverTable[m_byClass]->m_byPPDivide;
		m_iSPRate = g_arRecoverTable[m_byClass]->m_bySPDivide;

		m_iHPIntervalTime = (int)(g_arRecoverRateTable[m_byClass]->m_sHpSpeedupTime - (double)(g_arRecoverRateTable[m_byClass]->m_byHpSpeedupLevel/30));
		m_iSPIntervalTime = (int)(g_arRecoverRateTable[m_byClass]->m_sSpSpeedupTime - (double)(g_arRecoverRateTable[m_byClass]->m_bySpSpeedupLevel/30));
		m_iPPIntervalTime = (int)(g_arRecoverRateTable[m_byClass]->m_sPpSpeedupTime - (double)(g_arRecoverRateTable[m_byClass]->m_byPpSpeedupLevel/30));
	}
	
	weight = (int)((double)m_iMaxWeight * 0.7 + 0.5);		// ¹«°Ô Á¦ÇÑÀ¸·Î ÀÎÇÑ È¸º¹ ¼Óµµ¸¦ ´Ê...
	if(m_iCurWeight >= weight) 
	{
		m_iHPIntervalTime = m_iHPIntervalTime * 2;
		m_iSPIntervalTime = m_iSPIntervalTime * 2;
		m_iPPIntervalTime = m_iPPIntervalTime * 2;
	}
}

///////////////////////////////////////////////////////////////////////
//	À¯Àú Á¤º¸ÀÇ Zone ÀÌ ¼­¹ö°¡ ´ç´çÇÏ°í ÀÖ´Â Á¸ÀÎÁö °Ë»çÇÑ´Ù.
//
BOOL USER::IsZoneInThisServer(int zone)
{
	int temp_zone = -1;

	for(int i = 0; i < g_zone.GetSize(); i++)
	{
		if( g_zone[i]->m_Zone == zone )	return TRUE;
		//TRACE("µØÍ¼´úÂë %d \n",g_zone[i]->m_Zone);
	}

	return FALSE;
}

////////////////////////////////////////////////////////////////////////
//	¸Å°³º¯¼öÀÇ Zone ÀÌ ¾î´À ¼­¹ö¿¡¼­ °üÀåÇÏ´ÂÁö¸¦ Ã£¾Æ¼­ 
//	¼¼¼ÇÀÇ ZONEINFO Table À» ¾÷µ¥ÀÌÆ® ÇÑ´Ù.
//
//	¸øÃ£´Â °æ¿ì´Â ³ªÁß¿¡ ÄÚµù
void USER::ChangeServer(int zone)
{
	int server_index = -1;
	int i, j;
	BOOL found = FALSE;
	BOOL bFoundDBIndex = FALSE;

	for( i = 0; i < g_server.GetSize(); i++)
	{
		if(found) break;

		if(g_server[i])
		{
			if( g_server[i]->m_sDBIndex != -1 )
			{
				bFoundDBIndex = FALSE;

				for( j = 0; j < g_arMyServer.GetSize(); j++ )
				{
					if( *(g_arMyServer[j]) == g_server[i]->m_sDBIndex )
					{
						bFoundDBIndex = TRUE;
						break;
					}
				}

				if( !bFoundDBIndex ) continue;
			}

			for( j = 0; j < g_server[i]->m_zone.GetSize(); j++)
			{
				if(found) break;

				if(g_server[i]->m_zone[j])
				{
					if( *(g_server[i]->m_zone[j]) == zone)
					{
						server_index = i;
						found = TRUE;
					}
				}
			}
		}
	}

	if(server_index == -1) 
	{
		TRACE("Can't find valid point...\n");
		return;// FALSE;
	}
/*
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode;
	TCHAR			szSQL[1024];

	memset(szSQL, 0x00, 1024);
	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call CHANGE_SERVER (\'%s\',%d)}"), m_strUserID, server_index);

	int db_index = 0;
	CDatabase* pDB = g_DBSession[m_iModSid].GetDB( db_index );
	if( !pDB ) return;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if (retcode!=SQL_SUCCESS)
	{
//		g_DB[m_iModSid].ReleaseDB(db_index);
		return;
//		bQuerySuccess = FALSE;
	}

	retcode = SQLExecDirect (hstmt, (unsigned char *)szSQL, SQL_NTS);
	if (retcode ==SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
	{
	}
	else if (retcode==SQL_ERROR)
	{
		DisplayErrorMsg(hstmt);
		if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
//		BREAKPOINT();

		g_DB[m_iModSid].ReleaseDB(db_index);
		return;
	}

	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DBSession[m_iModSid].ReleaseDB(db_index);
*/
}

///////////////////////////////////////////////////////////////////////////
//	Zone Index ¸¦ ¼ÂÆÃÇÑ´Ù.
//
void USER::SetZoneIndex(int zone)
{
	for(int i = 0; i < g_zone.GetSize(); i++)
	{
		if( g_zone[i]->m_Zone == zone )
		{
			m_ZoneIndex = i;
			break;
		}
	}
}

//######################################################################################
//	Send Functions

////////////////////////////////////////////////////////////////////////////////////////
//	½Ã¾ß°¡ º¯°æµÆ´ÂÁö ÆÇ´ÜÇÏ°í º¯°æµÆ´Ù¸é º¯°æ³»¿ëÀ» Å¬¶óÀÌ¾ðÆ®·Î Àü¼ÛÇÑ´Ù.
//
void USER::SightRecalc()
{
	int sx, sy;
	sx = m_curx / SIGHT_SIZE_X;
	sy = m_cury / SIGHT_SIZE_Y;

	int dir_x = 0;
	int dir_y = 0;

	if( sx == m_presx && sy == m_presy ) return;
	
	if( m_presx == -1 || m_presy == -1 )
	{
		dir_x = 0;
		dir_y = 0;
	}
	else if( m_presx == -2 || m_presy == -2 )
	{
		dir_x = DIR_OUTSIDE;
		dir_y = DIR_OUTSIDE;
	}
	else
	{
		if( sx > m_presx && abs(sx-m_presx) == 1 )		dir_x = DIR_H;
		if( sx < m_presx && abs(sx-m_presx) == 1 )		dir_x = DIR_L;
		if( sy > m_presy && abs(sy-m_presy) == 1 )		dir_y = DIR_H;
		if( sy < m_presy && abs(sy-m_presy) == 1 )		dir_y = DIR_L;
		if( abs(sx-m_presx) > 1 )						dir_x = DIR_OUTSIDE;
		if( abs(sy-m_presy) > 1 )						dir_y = DIR_OUTSIDE;
	}

//	TRACE("Sight changed....[%d] [%d] -> [%d] [%d] (%d %d)\n", m_presx, m_presy, sx, sy, dir_x, dir_y);

	int prex = m_presx;
	int prey = m_presy;
	m_presx = sx;
	m_presy = sy;

	SendUserInfoBySightChange(dir_x, dir_y, prex, prey);
}

/////////////////////////////////////////////////////////////////////////////////////
//	·¢ËÍ¸Ä±äÈËÎï×´Ì¬·â°ü
void USER::SendUserInfoBySightChange(int dir_x, int dir_y, int prex, int prey)
{
	int min_x = 0, min_y = 0;
	int max_x = 0, max_y = 0;

	int sx = m_presx;
	int sy = m_presy;

	int modify_index = 0;
	TCHAR modify_send[10000];	::ZeroMemory(modify_send, sizeof(modify_send));
	FillUserInfo(modify_send, modify_index, INFO_MODIFY);

	int delete_index = 0;
	TCHAR delete_send[4096];	::ZeroMemory(delete_send, sizeof(delete_send));
	FillUserInfo(delete_send, delete_index, INFO_DELETE);

	if( prex == -1 || prey == -1 )
	{
		min_x = (sx-1)*SIGHT_SIZE_X;
		max_x = (sx+2)*SIGHT_SIZE_X;
		min_y = (sy-1)*SIGHT_SIZE_Y;
		max_y = (sy+2)*SIGHT_SIZE_Y;
		SendRangeInfoToMe(min_x, min_y, max_x, max_y, INFO_MODIFY);
//		SendItemFieldInfoToMe();
		SendCompressedRangeInfoToMe();
		return;
	}
	if( dir_x == DIR_OUTSIDE || dir_y == DIR_OUTSIDE )
	{
		min_x = (prex-1)*SIGHT_SIZE_X;
		max_x = (prex+2)*SIGHT_SIZE_X;
		min_y = (prey-1)*SIGHT_SIZE_Y;
		max_y = (prey+2)*SIGHT_SIZE_Y;
		SendRangeInfoToMe(min_x, min_y, max_x, max_y, INFO_DELETE);
		SendToRange(delete_send, delete_index, min_x, min_y, max_x, max_y);
		min_x = (sx-1)*SIGHT_SIZE_X;
		max_x = (sx+2)*SIGHT_SIZE_X;
		min_y = (sy-1)*SIGHT_SIZE_Y;
		max_y = (sy+2)*SIGHT_SIZE_Y;
		SendRangeInfoToMe(min_x, min_y, max_x, max_y, INFO_MODIFY);
		SendToRange(modify_send, modify_index, min_x, min_y, max_x, max_y);	// ¿î¿µÀÚ µ¥ÀÌÅÍ´Â ¿øÃµºÀ¼¼.
//		SendItemFieldInfoToMe();
		SendCompressedRangeInfoToMe();
		return;
	}
	if( dir_x > 0 )
	{
		min_x = (prex-1)*SIGHT_SIZE_X;
		max_x = (prex)*SIGHT_SIZE_X;
		min_y = (prey-1)*SIGHT_SIZE_Y;
		max_y = (prey+2)*SIGHT_SIZE_Y;
		SendRangeInfoToMe(min_x, min_y, max_x, max_y, INFO_DELETE);
		SendToRange(delete_send, delete_index, min_x, min_y, max_x, max_y);
		min_x = (sx+1)*SIGHT_SIZE_X;
		max_x = (sx+2)*SIGHT_SIZE_X;
		min_y = (sy-1)*SIGHT_SIZE_Y;
		max_y = (sy+2)*SIGHT_SIZE_Y;
		SendRangeInfoToMe(min_x, min_y, max_x, max_y, INFO_MODIFY);
		SendToRange(modify_send, modify_index, min_x, min_y, max_x, max_y);
//		SendItemFieldInfoToMe();
	}
	if( dir_y > 0 )
	{
		min_x = (prex-1)*SIGHT_SIZE_X;
		max_x = (prex+2)*SIGHT_SIZE_X;
		min_y = (prey-1)*SIGHT_SIZE_Y;
		max_y = (prey)*SIGHT_SIZE_Y;
		SendRangeInfoToMe(min_x, min_y, max_x, max_y, INFO_DELETE);
		SendToRange(delete_send, delete_index, min_x, min_y, max_x, max_y);
		min_x = (sx-1)*SIGHT_SIZE_X;
		max_x = (sx+2)*SIGHT_SIZE_X;
		min_y = (sy+1)*SIGHT_SIZE_Y;
		max_y = (sy+2)*SIGHT_SIZE_Y;
		SendRangeInfoToMe(min_x, min_y, max_x, max_y, INFO_MODIFY);
		SendToRange(modify_send, modify_index, min_x, min_y, max_x, max_y);
//		SendItemFieldInfoToMe();
	}
	if( dir_x < 0 )
	{
		min_x = (prex+1)*SIGHT_SIZE_X;
		max_x = (prex+2)*SIGHT_SIZE_X;
		min_y = (prey-1)*SIGHT_SIZE_Y;
		max_y = (prey+2)*SIGHT_SIZE_Y;
		SendRangeInfoToMe(min_x, min_y, max_x, max_y, INFO_DELETE);
		SendToRange(delete_send, delete_index, min_x, min_y, max_x, max_y);
		min_x = (sx-1)*SIGHT_SIZE_X;
		max_x = (sx)*SIGHT_SIZE_X;
		min_y = (sy-1)*SIGHT_SIZE_Y;
		max_y = (sy+2)*SIGHT_SIZE_Y;
		SendRangeInfoToMe(min_x, min_y, max_x, max_y, INFO_MODIFY);
		SendToRange(modify_send, modify_index, min_x, min_y, max_x, max_y);
//		SendItemFieldInfoToMe();
	}
	if( dir_y < 0 )
	{
		min_x = (prex-1)*SIGHT_SIZE_X;
		max_x = (prex+2)*SIGHT_SIZE_X;
		min_y = (prey+1)*SIGHT_SIZE_Y;
		max_y = (prey+2)*SIGHT_SIZE_Y;
		SendRangeInfoToMe(min_x, min_y, max_x, max_y, INFO_DELETE);
		SendToRange(delete_send, delete_index, min_x, min_y, max_x, max_y);
		min_x = (sx-1)*SIGHT_SIZE_X;
		max_x = (sx+2)*SIGHT_SIZE_X;
		min_y = (sy-1)*SIGHT_SIZE_Y;
		max_y = (sy)*SIGHT_SIZE_Y;
		SendRangeInfoToMe(min_x, min_y, max_x, max_y, INFO_MODIFY);
		SendToRange(modify_send, modify_index, min_x, min_y, max_x, max_y);
//		SendItemFieldInfoToMe();
	}

	SendCompressedRangeInfoToMe();
}

////////////////////////////////////////////////////////////////////////////////////
//	º¸³¾ ½Ã¾ß Á¤º¸¸¦ ¾ÐÃàÇÏ¿© º¸³½´Ù
//
void USER::SendCompressedRangeInfoToMe()
{
	if( !m_CompCount || m_CompCount < 0 || !m_iCompIndex || m_iCompIndex < 0 )
	{
		m_CompCount = 0;
		m_iCompIndex = 0;
		return;
	}

	m_CompMng.FlushAddData();

	m_CompMng.AddData( m_CompBuf, m_iCompIndex );

	m_CompMng.PreCompressWork();
	m_CompMng.Compress();

	int comp_data_len = m_CompMng.GetCompressedDataCount();
	int org_data_len = m_CompMng.GetUnCompressDataLength();
	DWORD crc_value = m_CompMng.GetCrcValue();

	CBufferEx	TempBuf;

	TempBuf.Add( SIGHT_INFO_COMPRESSED );
	TempBuf.Add( (short)comp_data_len );
	TempBuf.Add( (short)org_data_len );
	TempBuf.Add( crc_value );
	TempBuf.Add( (short)m_CompCount );

	char* packet = m_CompMng.GetExtractedBufferPtr();

	TempBuf.AddData( packet, comp_data_len );

	SEND_DATA* pNewData = NULL;
	pNewData = new SEND_DATA;
	if(pNewData == NULL)
	{
		m_CompCount = 0;
		m_iCompIndex = 0;
		m_CompMng.FlushAddData();
		return;
	}

	pNewData->flag = SEND_USER;
	pNewData->len = TempBuf.GetLength();

	::CopyMemory(pNewData->pBuf, TempBuf, TempBuf.GetLength());

	pNewData->uid = m_uid;

	// IKING 2001.1.
	//EnterCriticalSection( &(m_pCom->m_critSendData) );
	//m_pCom->m_arSendData.Add( pNewData );
	//LeaveCriticalSection( &(m_pCom->m_critSendData) );
	//PostQueuedCompletionStatus( m_pCom->m_hSendIOCP, 0, 0, NULL );
	m_pCom->Send(pNewData);

	if(pNewData) delete pNewData;
	//

	m_CompCount = 0;
	m_iCompIndex = 0;
	m_CompMng.FlushAddData();
}

////////////////////////////////////////////////////////////////////////////////////
//	Æ¯Á¤ À¯Àú ¶Ç´Â NPCÀÇ Á¤º¸¸¦ Å¬¶óÀÌ¾ðÆ®¿¡¼­ ¿äÃ»ÇÒ °æ¿ì Ã³¸®
//
void USER::UserInfoReq(TCHAR *pBuf)
{
	int index = 0;
	int nUid = GetInt(pBuf, index);
	
	if(nUid < 0 || nUid >= INVALID_BAND) return;
	
	if(nUid >= USER_BAND && nUid < NPC_BAND)
	{
		USER* pSendUser = GetUser(nUid - USER_BAND);	
//		if(pSendUser == NULL || pSendUser->m_curz != m_curz || pSendUser->m_state != STATE_GAMESTARTED) return;
		if( pSendUser == NULL || pSendUser->m_state != STATE_GAMESTARTED ) return;

		if( !IsInSight( pSendUser->m_curx, pSendUser->m_cury, pSendUser->m_curz ) ) return;
		
		SendUserInfo(pSendUser, INFO_MODIFY);
	}
	if(nUid >= NPC_BAND && nUid < INVALID_BAND)
	{
		CNpc* pNpc = GetNpc(nUid - NPC_BAND);
		if(pNpc == NULL) return;

		if( !IsInSight( pNpc->m_sCurX, pNpc->m_sCurY, pNpc->m_sCurZ ) ) return;

		SendNpcInfo(pNpc, INFO_MODIFY);
	}
}

BOOL USER::IsInSight(int x, int y, int z)
{
	if( m_curz != z ) return FALSE;

	int min_x, max_x;
	int min_y, max_y;

	int sx = m_curx / SIGHT_SIZE_X;
	int sy = m_cury / SIGHT_SIZE_Y;
	
	min_x = (sx-1)*SIGHT_SIZE_X; if( min_x < 0 ) min_x = 0;
	max_x = (sx+2)*SIGHT_SIZE_X;
	min_y = (sy-1)*SIGHT_SIZE_Y; if( min_y < 0 ) min_y = 0;
	max_y = (sy+2)*SIGHT_SIZE_Y;
	
	if( m_ZoneIndex < 0 || m_ZoneIndex >= g_zone.GetSize() ) return FALSE;

	MAP* pMap = g_zone[m_ZoneIndex];
	if( !pMap ) return FALSE;
	
	if( max_x >= pMap->m_sizeMap.cx ) max_x = pMap->m_sizeMap.cx - 1;
	if( max_y >= pMap->m_sizeMap.cy ) max_y = pMap->m_sizeMap.cy - 1;

	if( min_x > x || max_x < x ) return FALSE;
	if( min_y > y || max_y < y ) return FALSE;

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////
//	·¢ËÍNPCÐÅÏ¢
void USER::SendNpcInfo(CNpc *pNpc, BYTE tMode /* INFO_MODIFY */)
{
	if( !pNpc ) return;
	CBufferEx TempBuf;
	TempBuf.Add((BYTE)NPC_INFO);
	/////////////¹ÖÎïÌØÐ§
	BYTE mod = 0;	
	short sPid = pNpc->m_sPid;
	if(sPid == 204 || sPid == 211) mod = 3;	
	if(sPid == 192 || sPid == 205 || sPid == 208 || sPid == 268) mod = 5;	
	if(tMode == INFO_DELETE)
	{
		if(sPid == 204 || sPid == 211) mod = 4;	
		if(sPid == 192 || sPid == 205 || sPid == 208 || sPid == 268) mod = 6;	
	}
	if(mod > 0)
	{
		TempBuf.Add((BYTE)mod);
	}
	else
	{
		TempBuf.Add((BYTE)tMode);
	}
	//////////////////////////////
	TempBuf.Add((short)(pNpc->m_sNid + NPC_BAND));
	if(tMode != INFO_MODIFY)
	{
		Send(TempBuf, TempBuf.GetLength());
		return;
	}
	TempBuf.Add((short)pNpc->m_sPid);
	if(pNpc->m_tNpcType == 0)
	{
		TCHAR tempName[50];
		sprintf(tempName,"(Lv.%d)%s",pNpc->m_byClassLevel,pNpc->m_strName);
		TempBuf.AddString(tempName);
	}
	else
	{
		TempBuf.AddString(pNpc->m_strName);
	}
	CPoint pt = ConvertToClient(pNpc->m_sCurX, pNpc->m_sCurY);
	if(pt.x == -1 || pt.y == -1) { pt.x = 1; pt.y = 1; }
	TempBuf.Add((short)pt.x);
	TempBuf.Add((short)pt.y);
//	if(pNpc->m_NpcState == NPC_DEAD) TempBuf.Add((BYTE)0x00);
	if(pNpc->m_sHP <= 0) TempBuf.Add((BYTE)0x00);
	else TempBuf.Add((BYTE)0x01);
	TempBuf.Add(pNpc->m_tNpcType);	// NPC Type
	TempBuf.Add((short)pNpc->m_sMaxHP);	
	TempBuf.Add((short)pNpc->m_sHP);
	for(int zero = 0;zero < 8;zero++)//MarkJayÐÞ¸Ä
	{
		TempBuf.Add((BYTE)0x00);
	}
	TempBuf.Add(pNpc->m_sClientSpeed);
	TempBuf.Add(pNpc->m_byColor);
	if(pNpc->m_tNpcType == NPCTYPE_GUILD_DOOR)
	{
		TempBuf.Add(pNpc->m_sDimension);
	}
	TempBuf.Add(pNpc->m_sQuestSay);
	
	Send(TempBuf, TempBuf.GetLength());
}
///////////////////////////////////////////////////////////////////////////////////
//NPCÐÅÏ¢¸øÎÒ
int USER::MakeRangeInfoToMe(CNpc* pNpc, BYTE tMode, TCHAR *pData)
{
	if( !pNpc ) return 0;
	CPoint pt = ConvertToClient( pNpc->m_sCurX, pNpc->m_sCurY );
	if( pt.x == -1 || pt.y == -1 ) return 0;
	int index = 0;
	SetByte( pData, (BYTE)NPC_INFO, index );
	/////////¹ÖÎïÌØÐ§
	BYTE mod = 0;	
	short sPid = pNpc->m_sPid;
	if(sPid == 204 || sPid == 211) mod = 3;	
	if(sPid == 192 || sPid == 205 || sPid == 208 || sPid == 268) mod = 5;	
	if(tMode == INFO_DELETE)
	{
		if(sPid == 204 || sPid == 211) mod = 4;	
		if(sPid == 192 || sPid == 205 || sPid == 208 || sPid == 268) mod = 6;		
	}
	if(mod > 0)
	{
		SetByte( pData, mod, index );
	}
	else
	{
		SetByte( pData, tMode, index );
	}
	///////////////////////////////
	SetShort( pData, (short)(pNpc->m_sNid + NPC_BAND), index );
	if( tMode != INFO_MODIFY )
	{
		return index;
	}
	if(pNpc->m_tNpcType == 0)
	{
		TCHAR tempName[50];
		sprintf(tempName,"(Lv.%d)%s",pNpc->m_byClassLevel,pNpc->m_strName);
		SetShort( pData, (short)pNpc->m_sPid, index );
		SetByte( pData, (BYTE)strlen( tempName ), index);
		SetString( pData, tempName, _tcslen( tempName ), index );
	}
	else
	{
		SetShort( pData, (short)pNpc->m_sPid, index );
		SetByte( pData, (BYTE)_tcslen( pNpc->m_strName ), index);
		SetString( pData, pNpc->m_strName, _tcslen( pNpc->m_strName ), index );
	}
	SetShort( pData, (short)pt.x, index );
	SetShort( pData, (short)pt.y, index );
	if( pNpc->m_sHP == 0 ) SetByte( pData, 0X00, index );
	else SetByte( pData, 0X01, index );
	SetByte( pData, pNpc->m_tNpcType, index );
	SetShort( pData, (short)pNpc->m_sMaxHP, index );
	SetShort( pData, (short)pNpc->m_sHP, index );
	for(int zero = 0;zero < 8;zero++)//MarkJayÐÞ¸Ä
	{
		SetByte(pData , 0x00, index);
	}
	SetShort( pData, pNpc->m_sClientSpeed, index );	
	//int iRandom = myrand(0,6);//¹ÖÎïÑÕÉ« MarkJay
	/*if(pNpc->m_tNpcType == 0)
	{
		SetByte( pData, (BYTE)iRandom, index );	
	}
	else*/
	//{		
		SetByte( pData, pNpc->m_byColor, index );	
	//}	

	if(pNpc->m_tNpcType == NPCTYPE_GUILD_DOOR) 
	{
		SetShort( pData, pNpc->m_sDimension, index);
	}
	SetShort(pData, pNpc->m_sQuestSay, index);
//==========================NPCÏÔÊ¾ÒªÈûÍÅÍ¼±ê============================
	if((pNpc->m_tNpcType != 1) && (pNpc->m_tNpcType != 3) && (pNpc->m_tNpcType != 6)) return index;
	CGuildFortress *pGuild = GetGuildWarWin();
	if(pGuild == NULL)
	{
		SetInt(pData,-1,index);
		SetByte(pData,0,index);
		SetInt(pData,0,index);
	}
	else
	{
		SetInt(pData,pGuild->m_iGuildSid,index);
		SetVarString(pData,pGuild->m_strGuildName,strlen(pGuild->m_strGuildName),index);
		SetInt(pData,0,index);
	}
//======================================================================
	return index;
}
CGuildFortress *USER::GetGuildWarWin()//ÅÐ¶ÏÒªÈû¾üÍÅ
{
	CGuildFortress* pFort = NULL;
    int Fortress[] = {f_Markini[0]->JT_Fortressid};
	for(int i = 0; i < 3;i++)
	{
		pFort = GetFortress(Fortress[i]);
		if(pFort == NULL) continue;
		if(pFort->m_iGuildSid <= 0 || pFort->m_iGuildSid != m_dwGuild) continue;	
		return pFort;
	}
	return NULL;
}
void USER::AddRangeInfoToMe(CNpc *pNpc, BYTE tMode)
{
	if(pNpc == NULL) return;//MarkJay ÐÞ¸Ä
	int index = 0;
	TCHAR pData[1024];

	index = MakeRangeInfoToMe( pNpc, tMode, pData );

	if( index )
	{
		m_CompCount++;

		SetShort( m_CompBuf, index, m_iCompIndex );				// ¸¸µé¾îÁø À¯Àú Á¤º¸ÀÇ ±æÀÌ
		::CopyMemory( m_CompBuf+m_iCompIndex, pData, index );	// ¸¸µé¾îÁø Á¤º¸¸¦ ¾ÐÃàÇÒ ¹öÆÛ¿¡ º¹»ç
		m_iCompIndex += index;

		if( m_iCompIndex >= 8000 )
		{
			SendCompressedRangeInfoToMe();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////
//	Æ¯Á¤ À¯ÀúÀÇ À¯Àú Á¤º¸¸¦ º¸³½´Ù.
//
void USER::SendUserInfo(USER *pUser, BYTE tMode	/*INFO_MODIFY*/)
{
	if( !pUser ) return;

	CBufferEx	TempBuf;
	int i;

	CPoint pos = ConvertToClient( pUser->m_curx, pUser->m_cury );
	if( pos.x == -1 || pos.y == -1 ) { pos.x = 1; pos.y = 1; }

	TempBuf.Add(USER_INFO);
	TempBuf.Add(tMode);
	TempBuf.Add(pUser->m_uid + USER_BAND);

	if(tMode != INFO_MODIFY)
	{
		Send(TempBuf, TempBuf.GetLength());
		return;
	}

	TempBuf.AddString(pUser->m_strUserID);

	TempBuf.Add((short)pos.x);
	TempBuf.Add((short)pos.y);

	TempBuf.Add(pUser->m_iSkin);
	TempBuf.Add(pUser->m_iHair);
	TempBuf.Add((BYTE)pUser->m_sGender);
	TempBuf.AddData(pUser->m_strFace, 10);

	for( i = 0; i < EQUIP_ITEM_NUM; i++) TempBuf.Add(pUser->m_UserItem[i].sSid);

	TempBuf.Add(pUser->m_sHP);
	TempBuf.Add(pUser->m_sMagicMaxHP);

	TempBuf.Add(pUser->m_tDir);
	TempBuf.Add(pUser->m_dwAbnormalInfo);				// »óÅÂÀÌ»ó Á¤º¸´Â ¾ÕÀ¸·Î m_dwAbnormalInfo ÇÏ³ª¸¸ ÂüÁ¶ÇÑ´Ù. JJS07 2002.10.14
	TempBuf.Add(pUser->m_dwAbnormalInfo_);	
	TempBuf.Add(pUser->m_sCityRank);

	TempBuf.Add((int)pUser->m_dwGuild);					//&&&&&&&&&&&& Test Code
	TempBuf.AddString(pUser->m_strGuildName);			// ±æµå ÀÌ¸§À» Ãß°¡
	TempBuf.Add(pUser->m_sGuildVersion);

	TempBuf.Add(pUser->m_byClass);
	TempBuf.Add((BYTE)pUser->m_bPkStatus);
	TempBuf.AddString(pUser->m_strLoveName); //ÊØ»¤ÏÔÊ¾

	for(i = TOTAL_INVEN_MAX; i < TOTAL_ITEM_NUM-1; i++) TempBuf.Add(pUser->m_UserItem[i].sSid);	// EBody
	if(pUser->m_UserItem[TOTAL_ITEM_NUM-1].sSid!=-1&&pUser->m_UserItem[TOTAL_ITEM_NUM-1].sDuration!=0){
		TempBuf.Add((BYTE)(pUser->m_UserItem[TOTAL_ITEM_NUM-1].tMagic[0]));
		TempBuf.Add((BYTE)0x00);
	}else{
		TempBuf.Add(pUser->m_UserItem[TOTAL_ITEM_NUM-1].tMagic[0]);
		TempBuf.Add((BYTE)0xff);
	}
	TempBuf.AddString(pUser->m_PersonalShopName);
	Send(TempBuf, TempBuf.GetLength());
}

int USER::MakeRangeInfoToMe(USER* pUser, BYTE tMode, TCHAR *pData)
{
	int nLen = 0;
	int i = 0;

	if( !pUser ) return 0;

	CPoint pos = ConvertToClient( pUser->m_curx, pUser->m_cury );
	if( pos.x == -1 || pos.y == -1 ) return 0;

	int index = 0;

	SetByte( pData, USER_INFO, index );
	SetByte( pData, tMode, index );
	SetInt(  pData, pUser->m_uid + USER_BAND, index );

	if( tMode != INFO_MODIFY )
	{
		return index;
	}

	SetByte( pData, (BYTE)strlen( pUser->m_strUserID ), index );
	SetString( pData, pUser->m_strUserID, strlen( pUser->m_strUserID ), index );

	SetShort( pData, pos.x, index );
	SetShort( pData, pos.y, index );

	SetDWORD( pData, pUser->m_iSkin, index );
	SetDWORD( pData, pUser->m_iHair, index );
	SetByte( pData, (BYTE)pUser->m_sGender, index );

	::CopyMemory( pData+index, pUser->m_strFace, 10 );
	index += 10;

	for( i = 0; i < EQUIP_ITEM_NUM; i++ ) SetShort( pData, pUser->m_UserItem[i].sSid, index );

	SetShort( pData, pUser->m_sHP, index );
	SetShort( pData, pUser->m_sMagicMaxHP, index );

	SetByte( pData, pUser->m_tDir, index );

	SetDWORD( pData, pUser->m_dwAbnormalInfo, index);			// »óÅÂÀÌ»ó Á¤º¸
	SetDWORD( pData, pUser->m_dwAbnormalInfo_, index);

	SetShort( pData, pUser->m_sCityRank, index );
	SetInt( pData, pUser->m_dwGuild, index );

	nLen = strlen(pUser->m_strGuildName);
	if(nLen <= 0) nLen = 1;

	SetByte( pData, (BYTE)nLen, index );
	SetString( pData, pUser->m_strGuildName, nLen, index );		// Add Guild Name
	SetShort( pData, pUser->m_sGuildVersion, index );

	SetByte( pData, pUser->m_byClass, index );
	SetByte( pData, pUser->m_bPkStatus, index );
	//-- yskang 0.1 Ãß°¡µÇ´Â ÆÐÅ¶.... È£Äª[¾ÖÄª]
	nLen = strlen(pUser->m_strLoveName);
	if(nLen < 1) nLen =1;
	SetByte( pData, (BYTE)nLen, index );
	SetString(pData,pUser->m_strLoveName,nLen,index);

	for( i = TOTAL_INVEN_MAX; i < TOTAL_ITEM_NUM-1; i++) 
		SetShort( pData, pUser->m_UserItem[i].sSid, index );
	//SetByte( pData, 0x00, index );
	//SetByte( pData, 0x00, index );
	if(pUser->m_UserItem[TOTAL_ITEM_NUM-1].sSid!=-1 && pUser->m_UserItem[TOTAL_ITEM_NUM-1].sDuration != 0)
	{
		SetByte(pData,(BYTE)(pUser->m_UserItem[TOTAL_ITEM_NUM-1].tMagic[0]),index);
		SetByte( pData, 0x00, index );
	}
	else
	{
		SetByte(pData,(BYTE)(pUser->m_UserItem[TOTAL_ITEM_NUM-1].tMagic[0]),index);
		SetByte( pData, 0xff, index );
	}
		
	SetByte(pData, strlen(pUser->m_PersonalShopName), index);
	SetString(pData, pUser->m_PersonalShopName, strlen(pUser->m_PersonalShopName), index);
//SaveDump(pData,index);
	return index;
}

void USER::AddRangeInfoToMe(USER *pUser, BYTE tMode)
{
	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;//MarkJay ÐÞ¸Ä
	int index = 0;
	TCHAR pData[1024];

	index = MakeRangeInfoToMe( pUser, tMode, pData );

	if( index )
	{
		m_CompCount++;

		SetShort( m_CompBuf, index, m_iCompIndex );				// ¸¸µé¾îÁø À¯Àú Á¤º¸ÀÇ ±æÀÌ
		::CopyMemory( m_CompBuf+m_iCompIndex, pData, index );	// ¸¸µé¾îÁø Á¤º¸¸¦ ¾ÐÃàÇÒ ¹öÆÛ¿¡ º¹»ç
		m_iCompIndex += index;

		if( m_iCompIndex >= 8000 )
		{
			SendCompressedRangeInfoToMe();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//¹¦ÄÜ:·¢ËÍÈËÎïÊý¾Ý
//·µ»ØÖµ:ÎÞ
// °æ±¾ 1.00
//ÈÕÆÚ 2006.12.21
void USER::SendCharData()
{
	int i = 0;
	CBufferEx TempBuf;
	TempBuf.Add(CHAR_DATA);
	TempBuf.AddString(m_strUserID);
	TempBuf.Add(m_sMagicSTR);
	TempBuf.Add(m_sMagicCON);
	TempBuf.Add(m_sMagicDEX);
	TempBuf.Add(m_sMagicVOL);
	TempBuf.Add(m_sMagicWIS);
	TempBuf.Add((short)1);  // ÀÓ½Ã ÄÚµå
	TempBuf.Add(m_iSkin);
	TempBuf.Add(m_iHair);
	TempBuf.Add((BYTE)m_sGender);
	TempBuf.AddData(m_strFace, 10);
	TempBuf.Add(m_dwExp);
	TempBuf.Add(m_dwXP);
	TempBuf.Add(m_sSkillPoint);
	TempBuf.Add(m_sSkillPoint_);

	TempBuf.Add(m_sPA);
	TempBuf.Add(m_sMagicMaxHP);
	TempBuf.Add(m_sHP);
	TempBuf.Add(m_sMagicMaxPP);
	TempBuf.Add(m_sPP);
	TempBuf.Add(m_sMagicMaxSP);
	TempBuf.Add(m_sSP);
	TempBuf.Add(m_dwDN);
	TempBuf.Add(m_sCityRank);
	TempBuf.Add(m_sLevel);
	TempBuf.Add(m_byClass);
	TCHAR strSkill[_SKILL_DB], strItem[_ITEM_DB], strPsi[_PSI_DB],strSkill_[12*3];
	::ZeroMemory(strSkill, sizeof(strSkill));
	::ZeroMemory(strItem, sizeof(strItem));
	::ZeroMemory(strPsi, sizeof(strPsi));

	UserSkillToStr((LPTSTR)strSkill);

	UserItemToStrForSend( (LPTSTR)strItem );
	UserPsiToStr((LPTSTR)strPsi);
	int index=0;
	for(i = 0; i < 12; i++){
		SetShort(strSkill_, m_UserSkill_[i].sSid,		index);
		SetByte (strSkill_, m_UserSkill_[i].tLevel,	index);
	}
	TempBuf.AddData(strSkill, USER_SKILL_LEN);		// Skill
	TempBuf.AddData(strSkill_,12*3);
	TempBuf.Add((BYTE)m_nHavePsiNum);				// Psionic
	if(m_nHavePsiNum > 0) TempBuf.AddData(strPsi, m_nHavePsiNum * _PSI_SIZE);

	TempBuf.AddData(strItem, USER_ITEM_LEN);		// Item

	TempBuf.Add(m_dwExpNext);// Next Exp
	
	TempBuf.Add(m_dwAbnormalInfo);
	TempBuf.Add(m_dwAbnormalInfo_);

	for(int zero = 0;zero < 8;zero++)//MarkJayÐÞ¸Ä
	{
		TempBuf.Add((BYTE)0x00);
	}

	TempBuf.Add((int)m_dwGuild);					//&&&&&&&&&&&&&& Test Code
	TempBuf.AddString(m_strGuildName);			// ±æµå ÀÌ¸§À» Ãß°¡
	TempBuf.Add(m_sGuildVersion);

	if(m_dwGuild >= 1)
	{
		//TempBuf.Add((BYTE)0xF4);
		//TempBuf.Add((BYTE)0x02);
		//TempBuf.Add((BYTE)0x41);
		TempBuf.Add((BYTE)0x00);
		TempBuf.Add((BYTE)0x00);
		TempBuf.Add((BYTE)0x00);
	}
	else
	{
		TempBuf.Add((BYTE)0x00);
		TempBuf.Add((BYTE)0x00);
		TempBuf.Add((BYTE)0x00);
	}
	
	TempBuf.Add((BYTE)0xFF);
	TempBuf.Add((short)-1);//¿ª»úÀ¶ÁúÐÄ
	TempBuf.Add((BYTE)0x84); // Finito Added
	TempBuf.Add((BYTE)0x99); // Finito Added
	TempBuf.Add((BYTE)0x2c); // Finito Added
	TempBuf.Add((BYTE)0x02); // Finito Added

	CBufferEx TempBuf1;

	TempBuf1.Add( (short)(TempBuf.GetLength()) );
	TempBuf1.AddData( TempBuf, TempBuf.GetLength() );

	// ¾ÐÃà ÁØºñ 
	m_CompMng.FlushAddData();

	m_CompMng.AddData( TempBuf1, TempBuf1.GetLength() );

	m_CompMng.PreCompressWork();
	m_CompMng.Compress();

	int comp_data_len = m_CompMng.GetCompressedDataCount();
	int org_data_len = m_CompMng.GetUnCompressDataLength();
	DWORD crc_value = m_CompMng.GetCrcValue();

	CBufferEx SendBuf;

	SendBuf.Add( SIGHT_INFO_COMPRESSED );
	SendBuf.Add( (short)comp_data_len );
	SendBuf.Add( (short)org_data_len );
	SendBuf.Add( crc_value );
	SendBuf.Add( (short)1 );

	char* packet = m_CompMng.GetExtractedBufferPtr();

	SendBuf.AddData( packet, comp_data_len );
	
	TRACE( "%d -> %d\n", org_data_len, comp_data_len );

	m_CompMng.FlushExtractedData();//MarkJay Ôö¼Ó ½âÑ¹Ëõ

	SEND_DATA* pNewData = NULL;
	pNewData = new SEND_DATA;
	if(pNewData == NULL)
	{
		m_CompMng.FlushAddData();
		return;
	}

	pNewData->flag = SEND_USER;
	pNewData->len = SendBuf.GetLength();

	::CopyMemory(pNewData->pBuf, SendBuf, SendBuf.GetLength());

	pNewData->uid = m_uid;

	m_pCom->Send(pNewData);
	if(pNewData) delete pNewData;
	//

	m_CompMng.FlushAddData();

}

//////////////////////////////////////////////////////////////////////////////////
//	¹¦ÄÜ: ·¢ËÍ×Ô¼ºÈËÎïÊý¾Ý
//  ·µ»ØÖµ: ÎÞ
//  °æ±¾: 1.00 (Å£Å£¸üÐÂ)
///////////////////////////////////////////////////////////////////////////////////
void USER::SendMyInfo(BYTE towho, BYTE type)
{
	CBufferEx	TempBuf;
	int i;

	TempBuf.Add(USER_INFO);
	TempBuf.Add(type);
	TempBuf.Add(m_uid + USER_BAND);

	CPoint pos = ConvertToClient( m_curx, m_cury );
	if( pos.x == -1 || pos.y == -1 ) { pos.x = 1; pos.y = 1; }

/*	if(type == INFO_MODIFY)
	{
	*/
		TempBuf.AddString(m_strUserID);
		
		TempBuf.Add((short)pos.x);
		TempBuf.Add((short)pos.y);
		
		TempBuf.Add(m_iSkin);
		TempBuf.Add(m_iHair);
		TempBuf.Add((BYTE)m_sGender);
		TempBuf.AddData(m_strFace, 10);
		
		for(i = 0; i < EQUIP_ITEM_NUM; i++) TempBuf.Add(m_UserItem[i].sSid);

		TempBuf.Add(m_sHP);
		TempBuf.Add(m_sMagicMaxHP);

		TempBuf.Add(m_tDir);

		TempBuf.Add(m_dwAbnormalInfo);
		TempBuf.Add(m_dwAbnormalInfo_);
		TempBuf.Add(m_sCityRank);

		TempBuf.Add((int)m_dwGuild);
		TempBuf.AddString(m_strGuildName);
		TempBuf.Add(m_sGuildVersion);
		TempBuf.Add((BYTE)m_byClass);
		TempBuf.Add((BYTE)m_bPkStatus);
		TempBuf.AddString(m_strLoveName);

		for( i = TOTAL_INVEN_MAX; i < TOTAL_ITEM_NUM-1; i++) 
			TempBuf.Add(m_UserItem[i].sSid);	// EBody
		if(m_UserItem[TOTAL_ITEM_NUM-1].sSid != -1 && m_UserItem[TOTAL_ITEM_NUM-1].sDuration != 0)
		{
			//BYTE code=;
			//static BYTE code=0x00;
			TempBuf.Add((BYTE)(m_UserItem[TOTAL_ITEM_NUM-1].tMagic[0]));
			//code++;
			TempBuf.Add((BYTE)0x00);
		}
		else
		{
			TempBuf.Add(m_UserItem[TOTAL_ITEM_NUM-1].tMagic[0]);
			TempBuf.Add((BYTE)0xff);
		}
		TempBuf.AddString(m_PersonalShopName);
//SaveDump(TempBuf,TempBuf.GetLength());
	switch(towho)
	{
	case TO_ALL:
		SendAll(TempBuf, TempBuf.GetLength());
		break;

	case TO_ME:
		Send(TempBuf, TempBuf.GetLength());
		break;

	case TO_ZONE:
		SendZone(TempBuf, TempBuf.GetLength());
		break;

	case TO_INSIGHT:
	default:
		SendInsight(TempBuf, TempBuf.GetLength());
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////
//	¸ðµç À¯Àú¿¡°Ô µ¥ÀÌÅÍ Àü¼Û
///////////////////////////////////////////////////////////////////////////////
void USER::SendAll(TCHAR *pBuf, int nLength)
{
	if( nLength <= 0 || nLength >= SEND_BUF_SIZE ) return;

	SEND_DATA* pNewData = NULL;
	pNewData = new SEND_DATA;
	if( !pNewData ) return;

	pNewData->flag = SEND_ALL;
	pNewData->len = nLength;

	::CopyMemory( pNewData->pBuf, pBuf, nLength );

	// IKING 2001.1.
	//EnterCriticalSection( &(m_pCom->m_critSendData) );
	//m_pCom->m_arSendData.Add( pNewData );
	//LeaveCriticalSection( &(m_pCom->m_critSendData) );
	//PostQueuedCompletionStatus( m_pCom->m_hSendIOCP, 0, 0, NULL );
	m_pCom->Send(pNewData);
	if(pNewData) delete pNewData;
	
}

///////////////////////////////////////////////////////////////////////////////
//Zone------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
void USER::SendZone(TCHAR *pBuf, int nLength)
{
	if( nLength <= 0 || nLength >= SEND_BUF_SIZE ) return;

	SEND_DATA* pNewData = NULL;
	pNewData = new SEND_DATA;
	if( !pNewData ) return;

	pNewData->flag = SEND_ZONE;
	pNewData->len = nLength;

	::CopyMemory( pNewData->pBuf, pBuf, nLength );

	pNewData->z = m_curz;

	// IKING 2001.1.
	//EnterCriticalSection( &(m_pCom->m_critSendData) );
	//m_pCom->m_arSendData.Add( pNewData );
	//LeaveCriticalSection( &(m_pCom->m_critSendData) );
	//PostQueuedCompletionStatus( m_pCom->m_hSendIOCP, 0, 0, NULL );
	m_pCom->Send(pNewData);
	if(pNewData) delete pNewData;
	//

	/*
	USER* pUser = NULL;

	for(int i = 0; i < MAX_USER; i++)
	{
		pUser = m_pCom->user_array[i];

		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED || pUser->m_curz != m_curz) continue;

		pUser->Send(pBuf, nLength);
	}
	*/
}

///////////////////////////////////////////////////////////////////////////////
//	Æ¯Á¤¿µ¿ª³»¿¡ ÀÖ´Â À¯Àú¿¡°Ô µ¥ÀÌÅÍ Àü¼Û
///////////////////////////////////////////////////////////////////////////////
void USER::SendRange(TCHAR *pBuf, int nLength, CRect rect)
{
	if( nLength <= 0 || nLength >= SEND_BUF_SIZE) return;

	SEND_DATA* pNewData = NULL;
	pNewData = new SEND_DATA;
	if( !pNewData ) return;

	pNewData->flag = SEND_RANGE;
	pNewData->len = nLength;

	::CopyMemory( pNewData->pBuf, pBuf, nLength );

	pNewData->uid = 0;
	pNewData->z = m_curz;
	pNewData->rect = rect;
	pNewData->zone_index = m_ZoneIndex;

	// IKING 2001.1.
	//EnterCriticalSection( &(m_pCom->m_critSendData) );
	//m_pCom->m_arSendData.Add( pNewData );
	//LeaveCriticalSection( &(m_pCom->m_critSendData) );
	//PostQueuedCompletionStatus( m_pCom->m_hSendIOCP, 0, 0, NULL );
	m_pCom->Send(pNewData);
	if(pNewData) delete pNewData;
	//
}

///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
void USER::SendInsight(TCHAR *pBuf, int nLength)
{
	if(nLength <= 0 || nLength >= SEND_BUF_SIZE) return;
	
	SEND_DATA* pNewData = NULL;
	pNewData = new SEND_DATA;
	if(pNewData == NULL) return;

	pNewData->flag = SEND_INSIGHT;
	pNewData->len = nLength;

	::CopyMemory(pNewData->pBuf, pBuf, nLength);

	pNewData->uid = 0;
	pNewData->x = m_curx;
	pNewData->y = m_cury;
	pNewData->z = m_curz;
	pNewData->zone_index = m_ZoneIndex;

	// IKING 2001.1.
	//EnterCriticalSection( &(m_pCom->m_critSendData) );
	//m_pCom->m_arSendData.Add( pNewData );
	//LeaveCriticalSection( &(m_pCom->m_critSendData) );
	//PostQueuedCompletionStatus( m_pCom->m_hSendIOCP, 0, 0, NULL );
	m_pCom->Send(pNewData);
	if(pNewData) delete pNewData;
}

void USER::SendExactScreen(TCHAR *pBuf, int nLength)
{
	if(nLength <= 0 || nLength >= SEND_BUF_SIZE) return;
	
	SEND_DATA* pNewData = NULL;
	pNewData = new SEND_DATA;
	if(pNewData == NULL) return;

	pNewData->flag = SEND_SCREEN;
	pNewData->len = nLength;

	::CopyMemory(pNewData->pBuf, pBuf, nLength);

	pNewData->uid = 0;
	pNewData->x = m_curx;
	pNewData->y = m_cury;
	pNewData->z = m_curz;
	pNewData->zone_index = m_ZoneIndex;

	// IKING 2001.1.
	//EnterCriticalSection( &(m_pCom->m_critSendData) );
	//m_pCom->m_arSendData.Add( pNewData );
	//LeaveCriticalSection( &(m_pCom->m_critSendData) );
	//PostQueuedCompletionStatus( m_pCom->m_hSendIOCP, 0, 0, NULL );
	m_pCom->Send(pNewData);
	if(pNewData) delete pNewData;
	//
}

//////////////////////////////////////////////////////////////////////////////
//	ÇÑ È­¸é³»ÀÇ À¯Àú¿¡°Ô µ¥ÀÌÅÍ Àü¼Û
//
void USER::SendScreen(TCHAR *pBuf, int nLength)
{
	USER* pUser = NULL;
	int dx, dy;

	for(int i = 0; i< MAX_USER; i++)
	{
		pUser = m_pCom->GetUserUid(i);

		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED || pUser->m_curz != m_curz) continue;

		dx = abs(m_curx - pUser->m_curx);
		dy = abs(m_cury - pUser->m_cury);

		if((dx + dy) <= (SCREEN_Y * 2))
		{
			pUser->Send(pBuf, nLength);
		}
	}
}

//##############################################################################
//
//	PC ¿òÁ÷ÀÓ °ü·Ã ·çÆ¾
//
//##############################################################################

//////////////////////////////////////////////////////////////////////////////
//	¹æÇâÀüÈ¯ ¿ä±¸¸¦ Ã³¸®ÇÑ´Ù.
//
void USER::ChangeDir(TCHAR *pBuf)
{
	int		index = 0;
	BYTE	dir;

	dir = GetByte(pBuf, index);
	if(dir < 0 || dir >= 8) return;

	m_tDir = dir;

	CBufferEx TempBuf;
	TempBuf.Add(CHANGE_DIR_RESULT);
	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.Add(m_tDir);

//	SendInsight(TempBuf, TempBuf.GetLength());
	SendExactScreen(TempBuf, TempBuf.GetLength());
}

////////////////////////////////////////////////////////////////////////////////
//	ÇöÀç º¸°íÀÖ´Â ¹æÇâ¸¦ Ç¥½ÃÇÑ´Ù. 
//
int USER::GetDirection(CPoint ptStart, CPoint ptEnd, int nDirCount)
{
	//MarkJayÐÞ¸Ä±ê¼Ç Ä¬ÈÏnDir;
	int nDir;					//	3 4 5
								//	2 8 6
								//	1 0 7
	int deltax = ptEnd.x - ptStart.x;
	int deltay = ptEnd.y - ptStart.y;

	if (deltax==0) {
		if (ptEnd.y>ptStart.y) nDir = DIR_DOWNRIGHT;		// ->		
		else nDir = DIR_UPLEFT;							// ->
	}
	else {
		double lean = (double)abs(deltay)/abs(deltax);

		if (ptEnd.y>ptStart.y) {
			if (ptEnd.x>ptStart.x) {
				if (nDirCount == 8) {
					if (lean<.3) nDir = DIR_RIGHT;
					else if (lean>2.) nDir = DIR_DOWN;
					else nDir = DIR_RIGHT;		// -> 
				}
				else {
					if (lean<.2) nDir = DIR_RIGHT;
					else if (lean<.4) nDir = DIR_RIGHT+8;
					else if (lean<1) nDir = DIR_DOWNRIGHT;
					else if (lean<5) nDir = DIR_DOWNRIGHT+8;
					else			 nDir = DIR_DOWN;
				}

			}
			else if (ptEnd.x < ptStart.x) {
				if (nDirCount == 8) {
					if (lean<.3) nDir = DIR_LEFT;
					else if (lean>2.) nDir = DIR_DOWN;
					else nDir = DIR_DOWN;		// -> 
				}
				else {
					if (lean<.2) nDir = DIR_LEFT;
					else if (lean<.4) nDir = DIR_LEFT+7;
					else if (lean<1) nDir = DIR_DOWNLEFT;
					else if (lean<5) nDir = DIR_DOWNLEFT+7;
					else			 nDir = DIR_DOWN;
				}
			}
		}
		else {
			if (ptEnd.x>ptStart.x) {
				if (nDirCount == 8) {
					if (lean<.3) nDir = DIR_UPRIGHT;	// ->
					else if (lean>2.) nDir = DIR_UP;
					else nDir = DIR_UP;		// ->
				}
				else {
					if (lean<.2) nDir = DIR_RIGHT;
					else if (lean<.4) nDir = DIR_RIGHT+7;
					else if (lean<1) nDir = DIR_UPRIGHT;
					else if (lean<5) nDir = DIR_UPRIGHT+7;
					else			 nDir = DIR_UP;
				}
			}
			else if (ptEnd.x < ptStart.x) {
				if (nDirCount == 8) {
					if (lean<.3) nDir = DIR_DOWNLEFT;		// -> 
					else if (lean>2.) nDir = DIR_UP;
					else nDir = DIR_LEFT;				// -> 2()
				} else {
					if (lean<.2) nDir = DIR_LEFT;
					else if (lean<.4) nDir = DIR_LEFT+8;
					else if (lean<1) nDir = DIR_UPLEFT;
					else if (lean<5) nDir = DIR_UPLEFT+8;
					else			 nDir = DIR_UP;
				}
			}
		}
	}

	return nDir;
/*if (deltax==0){
		if (ptEnd.y>ptStart.y) nDir = DIR_DOWN;		// ->DIR_DOWNRIGHT		
		else nDir = DIR_UP;							// ->DIR_UPLEFT
	}
	else {
		double lean = (double)abs(deltay)/abs(deltax);

		if (ptEnd.y>ptStart.y) {
			if (ptEnd.x>ptStart.x) {
				if (nDirCount == 8) {
					if (lean<.3) nDir = DIR_RIGHT;
					else if (lean>2.) nDir = DIR_DOWN;
					else nDir = DIR_DOWNRIGHT;		// -> DIR_RIGHT
				}
				else {
					if (lean<.2) nDir = DIR_RIGHT;
					else if (lean<.4) nDir = DIR_RIGHT+8;
					else if (lean<1) nDir = DIR_DOWNRIGHT;
					else if (lean<5) nDir = DIR_DOWNRIGHT+8;
					else			 nDir = DIR_DOWN;
				}

			}
			else if (ptEnd.x < ptStart.x) {
				if (nDirCount == 8) {
					if (lean<.3) nDir = DIR_LEFT;
					else if (lean>2.) nDir = DIR_DOWN;
					else nDir = DIR_DOWNLEFT;		// -> DIR_DOWN
				}
				else {
					if (lean<.2) nDir = DIR_LEFT;
					else if (lean<.4) nDir = DIR_LEFT+7;
					else if (lean<1) nDir = DIR_DOWNLEFT;
					else if (lean<5) nDir = DIR_DOWNLEFT+7;
					else			 nDir = DIR_DOWN;
				}
			}
		} else {
			if (ptEnd.x>ptStart.x) {
				if (nDirCount == 8) {
					if (lean<.3) nDir = DIR_RIGHT;	// ->DIR_UPRIGHT
					else if (lean>2.) nDir = DIR_UP;
					else nDir = DIR_UPRIGHT;		// ->DIR_UP
				} else {
					if (lean<.2) nDir = DIR_RIGHT;
					else if (lean<.4) nDir = DIR_RIGHT+7;
					else if (lean<1) nDir = DIR_UPRIGHT;
					else if (lean<5) nDir = DIR_UPRIGHT+7;
					else			 nDir = DIR_UP;
				}
			}
			else if (ptEnd.x < ptStart.x) {
				if (nDirCount == 8) {
					if (lean<.3) nDir = DIR_LEFT;		// -> DIR_DOWNLEFT
					else if (lean>2.) nDir = DIR_UP;
					else nDir = DIR_UPLEFT;				// -> 2(DIR_LEFT)
				}
				else {
					if (lean<.2) nDir = DIR_LEFT;
					else if (lean<.4) nDir = DIR_LEFT+8;
					else if (lean<1) nDir = DIR_UPLEFT;
					else if (lean<5) nDir = DIR_UPLEFT+8;
					else			 nDir = DIR_UP;
				}
			}
		}
	}
*/
//m_tDir =  nDir;
}
////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
void USER::MoveFirstReq(TCHAR *pBuf)
{
	BYTE result = FAIL;
	int will_x, will_y, x, y;
	int index = 0;
    DWORD checkTick;

	x = will_x = GetShort( pBuf, index );
	y = will_y = GetShort( pBuf, index );
	m_tDir = GetByte( pBuf, index );
    checkTick = GetDWORD(pBuf, index);
	//if(CheckMoveSpeedHack(checkTick)) return;//¾É°å·â¼ÓËÙ MarkJay
	//if(Move_C(will_x, will_y)) result = SUCCESS;
	if(CheckMoveSpeedHack(610) == FALSE)//·â¼ÓËÙ MarkJay
	{
		if(Move_C(will_x, will_y)) result = SUCCESS;
	}
	
	index = 0;
	SetByte(m_TempBuf, MOVE_FIRST_RESULT, index);
	SetByte(m_TempBuf, result, index);

	if(result == FAIL) 
	{
		SetInt(m_TempBuf, m_uid + USER_BAND, index);
		CPoint t = ConvertToClient(m_curx, m_cury);
		SetShort(m_TempBuf, t.x, index);
		SetShort(m_TempBuf, t.y, index);

		SendInsight(m_TempBuf, index);
		
		//TRACE("Move First Fail : uid = %d, UserName = %s, curx = %d, cury = %d\n", m_uid, m_strUserID, t.x, t.y);
		return;
	}
	

	SetInt(m_TempBuf, m_uid + USER_BAND, index);
	SetShort(m_TempBuf, will_x, index);
	SetShort(m_TempBuf, will_y, index);

	SendInsight(m_TempBuf, index);

	SightRecalc(); 
}

//////////////////////////////////////////////////////////////////////////////////
//	PC °¡ ¿òÁ÷ÀÌ´Â ÁßÀÇ Ã³¸®
//
void USER::MoveReq(TCHAR *pBuf)
{
	BYTE result = FAIL;
	int will_x, will_y, x, y;
	int index = 0;
    DWORD checkTick;

	x = will_x = GetShort( pBuf, index );	// ¿òÁ÷ÀÌ·Á´Â Á¡
	y = will_y = GetShort( pBuf, index );
	m_tDir = GetByte( pBuf, index );
	checkTick = GetDWORD(pBuf, index);
	//if(CheckMoveSpeedHack(checkTick)) return;//¾É°æ·âËÙ MarkJay
	//if(Move_C(x, y)) result = SUCCESS;
	if(CheckMoveSpeedHack(610) == FALSE)//·â¼ÓËÙ MarkJay
	{
		if(Move_C(x, y)) result = SUCCESS;
	}
	
	index = 0;
	SetByte(m_TempBuf, MOVE_RESULT, index);
	SetByte(m_TempBuf, result, index);

	if(result == FAIL) 
	{
		SetInt(m_TempBuf, m_uid + USER_BAND, index);
		CPoint t = ConvertToClient(m_curx, m_cury);
		SetShort(m_TempBuf, t.x, index);
		SetShort(m_TempBuf, t.y, index);

		SendInsight(m_TempBuf, index);
		
		//TRACE("Move Fail : uid = %d, UserName = %s, curx = %d, cury = %d\n", m_uid, m_strUserID, t.x, t.y);
		return;
	}

	SetInt(m_TempBuf, m_uid + USER_BAND, index);
	SetShort(m_TempBuf, will_x, index);
	SetShort(m_TempBuf, will_y, index);
	
	SendInsight(m_TempBuf, index);

	SightRecalc(); 
}
/////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////
void USER::MoveEndReq(TCHAR *pBuf)
{
	BYTE result = FAIL;
	int end_x, end_y, x, y;
	int index = 0;
    DWORD checkTick;

	x = end_x = GetShort( pBuf, index );	// ¿òÁ÷ÀÌ·Á´Â ¸¶Áö¸·Á¡
	y = end_y = GetShort( pBuf, index );
	m_tDir = GetByte( pBuf, index );
	checkTick = GetDWORD(pBuf, index);
	//if(CheckMoveSpeedHack(checkTick)) return;//¾É°æ·âËÙ MarkJay
	//if(Move_C(x, y)) result = SUCCESS;
	if(CheckMoveSpeedHack(610) == FALSE)//·â¼ÓËÙ MarkJay
	{
		if(Move_C(x, y)) result = SUCCESS;
	}
	
	index = 0;
	SetByte(m_TempBuf, MOVE_END_RESULT, index);
	SetByte(m_TempBuf, result, index);

	if(result == FAIL) 
	{
		SetInt(m_TempBuf, m_uid + USER_BAND, index);
		CPoint t = ConvertToClient(m_curx, m_cury);
		SetShort(m_TempBuf, t.x, index);
		SetShort(m_TempBuf, t.y, index);

		SendInsight(m_TempBuf, index);
		
		//TRACE("Move End Fail : uid = %d, UserName = %s, curx = %d, cury = %d\n", m_uid, m_strUserID, t.x, t.y);
		return;
	}

	SetInt(m_TempBuf, m_uid + USER_BAND, index);
	SetShort(m_TempBuf, end_x, index);
	SetShort(m_TempBuf, end_y, index);
	
	SendInsight(m_TempBuf, index);

	SightRecalc();
}

////////////////////////////////////////////////////////////////////////////////
//	Æ¯Á¤ ÁÂÇ¥·Î ÀÌµ¿ (¿î¿µÀÚ¸ðµå)
//
void USER::MoveChatReq(TCHAR *pBuf, BOOL bPsi)
{
	if(bPsi == FALSE && m_tIsOP != 1) return;

	BYTE result = FAIL;
	int will_x, will_y;
	int index = 0;

	will_x = GetShort( pBuf, index );	// ¿òÁ÷ÀÌ·Á´Â Á¡
	will_y = GetShort( pBuf, index );

	CPoint pt = FindNearAvailablePoint_C(will_x, will_y);

	CPoint pc;
	if(pt.x != -1 && pt.y != -1) 
	{
		pc = ConvertToServer(pt.x, pt.y);
		if(pc.x >= g_zone[m_ZoneIndex]->m_sizeMap.cx || pc.y >= g_zone[m_ZoneIndex]->m_sizeMap.cy) return;

		if(pc.x < 0 || pc.y < 0) 
		{
			result = FAIL;
		}
		else
		{
			if(m_curx < 0 || m_cury < 0) return;//ÐÞÕý×ø±ê±¨´í
			::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser, 0);
			::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[pc.x][pc.y].m_lUser, USER_BAND + m_uid);
			m_curx = pc.x;
			m_cury = pc.y;

			result = SUCCESS;
		}
	}

	index = 0;
	SetByte(m_TempBuf, MOVE_CHAT_RESULT, index);
	SetByte(m_TempBuf, result, index);

	if(result == FAIL) 
	{
		// Move Chat ¿¡µµ ³Ö¾îÁà¾ß ÇÏ³ª?
//CPoint t = ConvertToClient(m_curx, m_cur_y);
//SetShort(m_TempBuf, t.x, index);
//SetShort(m_TempBuf, t.y, index);

		Send(m_TempBuf, index);
		return;
	}

	SetInt(m_TempBuf, m_uid + USER_BAND, index);
	SetShort(m_TempBuf, pt.x, index);
	SetShort(m_TempBuf, pt.y, index);
	Send(m_TempBuf, index);

	SendInsight(m_TempBuf, index);

	SightRecalc();
	//yskang 0.4
	//TCHAR strOP[1024]; ZeroMemory(strOP,sizeof(strOP));
	//sprintf(strOP,"move_chat:x=%d,y=%d", pt.x,pt.y);
	//WriteOpratorLog(strOP,CHAT_LOG);
}
//////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////
void USER::MopCatchReq(TCHAR *pBuf)
{
	int index = 0;
	TCHAR szNpcName[80];

	::ZeroMemory(szNpcName, sizeof(szNpcName));

	if(!GetVarString(sizeof(szNpcName), szNpcName, pBuf, index)) return;

	CNpc* pNpc = GetNpc(szNpcName);
	if(pNpc == NULL) return;

	BYTE result = FAIL;
	int will_x, will_y;

	will_x = pNpc->m_sCurX;	// ¿òÁ÷ÀÌ·Á´Â Á¡
	will_y = pNpc->m_sCurY;

	CPoint pt = FindNearAvailablePoint_S(will_x, will_y);
	CPoint ptNew;
	if(pt.x != -1 && pt.y != -1) 
	{
		ptNew = ConvertToClient(pt.x, pt.y);
		if(ptNew.x != -1 && ptNew.y != -1)
		{
			if(m_curx < 0 || m_cury < 0) return;//ÐÞÕý×ø±ê±¨´í
			::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser, 0);
			::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[pt.x][pt.y].m_lUser, USER_BAND + m_uid);
			m_curx = pt.x;
			m_cury = pt.y;

			result = SUCCESS;
		}
	}

	index = 0;
	SetByte(m_TempBuf, MOVE_CHAT_RESULT, index);
	SetByte(m_TempBuf, result, index);

	if(result == FAIL) 
	{
		// Move Chat ¿¡µµ ³Ö¾îÁà¾ß ÇÏ³ª?
//^^	CPoint t = ConvertToClient(m_curx, m_cur_y);
//		SetShort(m_TempBuf, t.x, index);
//		SetShort(m_TempBuf, t.y, index);
//		TRACE("Move Chat Req Fail - %d : %d %d\n", m_uid, will_x, will_y);
		Send(m_TempBuf, index);
		return;
	}

	SetInt(m_TempBuf, m_uid + USER_BAND, index);
	SetShort(m_TempBuf, ptNew.x, index);
	SetShort(m_TempBuf, ptNew.y, index);
	Send(m_TempBuf, index);

	SendInsight(m_TempBuf, index);

	SightRecalc();

	//yskang 0.4
	//TCHAR strOP[1024]; ZeroMemory(strOP,sizeof(strOP));
	//sprintf(strOP,"trace_npc:%s,x=%d,y=%d",szNpcName,ptNew.x,ptNew.y);
	//WriteOpratorLog(strOP,CHAT_LOG);
}

///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void USER::UserCallReq(TCHAR *pBuf)
{
	BYTE result = FAIL;
	int index = 0;
	char szUserName[255];
	USER* pUser = NULL;
	CPoint pt, ptNew;
    CString moonname =_T("") ;

	moonname=m_strUserID;	
	if( m_tIsOP != 1 ) return;									// ¿î¿µÀÚ°¡ ¾Æ´Ô

	int nLength = GetVarString(sizeof(szUserName), szUserName, pBuf, index);
	if(nLength == 0 || nLength > CHAR_NAME_LENGTH) return;		// Àß¸øµÈ À¯Àú¾ÆÀÌµð 
	
	pUser = GetUser(szUserName);
	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;
	
	if(strcmp(pUser->m_strUserID, m_strUserID) == 0) return;	// ÀÚ±âÀÚ½ÅÀº ¾ÈµÊ
	
//	pt = FindNearAvailablePoint_S(m_curx, m_cury);
//	if(pt.x <= -1 && pt.y <= -1) return;						// ¿î¿µÀÚ ÁÖÀ§·Î ¿Ã ÁÂÇ¥°¡ ¾øÀ½

	if(pUser->m_curz != m_curz)									// À¯Àú¿Í ¿î¿µÀÚ°¡ °°ÀºÁ¸¿¡ ÀÖÁö ¾ÊÀ» °æ¿ì
	{
		if( !IsZoneInThisServer(pUser->m_curz) ) return;		// ³» ¼­¹ö¾È¿¡ ÀÖ´ÂÁ¸ÀÌ ¾Æ´Ô ¸®ÅÏ

		ptNew = ConvertToClient(m_curx, m_cury);
		BOOL bSuccess = pUser->ZoneChangeProcess(m_curz, ptNew.x, ptNew.y);	//^^ Check ¿ä¸Á
		
		if(bSuccess)
		{
//			pUser->SendZoneChange(bSuccess);
			pUser->SendWeatherInMoveZone();				// ÀÌµ¿ Á¸ÀÇ ³¯¾¾º¯È­¸¦ ¾Ë¸°´Ù.
		}
		return;
	}
	else
	{													// °°Àº Á¸ÀÌ¸é ÀÌµ¿À¸·Î...		
		pt = pUser->FindNearAvailablePoint_S( m_curx, m_cury );
		ptNew = ConvertToClient( pt.x, pt.y );

		if( ptNew.x == -1 || ptNew.y == -1 ) return;
		if(pUser->m_curx < 0 || pUser->m_cury < 0) return;//ÐÞÕý×ø±ê±¨´í

		::InterlockedExchange(&g_zone[pUser->m_ZoneIndex]->m_pMap[pUser->m_curx][pUser->m_cury].m_lUser, 0);
		::InterlockedExchange(&g_zone[pUser->m_ZoneIndex]->m_pMap[pt.x][pt.y].m_lUser, USER_BAND + pUser->m_uid);
		pUser->m_curx = pt.x;
		pUser->m_cury = pt.y;
		
		result = SUCCESS;
	}
	
	index = 0;
	SetByte(m_TempBuf, MOVE_CHAT_RESULT, index);
	SetByte(m_TempBuf, result, index);

	if(result == FAIL) 
	{
		Send(m_TempBuf, index);
		return;
	}

	SetInt(m_TempBuf, pUser->m_uid + USER_BAND, index);
	SetShort(m_TempBuf, ptNew.x, index);
	SetShort(m_TempBuf, ptNew.y, index);
	pUser->Send(m_TempBuf, index);	// À¯Àú¿¡°Ô´Â »õ·Î¿î ÁÂÇ¥°ªÀ»...
	
	pUser->SendInsight(m_TempBuf, index);
	
	pUser->SightRecalc();

	//yskang 0.4
	//TCHAR strOP[1024]; ZeroMemory(strOP,sizeof(strOP));
	//sprintf(strOP,"ÕÙ»ØÍæ¼Ò:%s,z=%d,x=%d,y=%d",pUser->m_strUserID, pUser->m_curz,pUser->m_curx,pUser->m_cury);
	//WriteOpratorLog(strOP,CALL_USER_LOG);
}

//////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////
void USER::UserRemoteCallReq(TCHAR *pBuf)
{
	CBufferEx TempBuf;
	int will_x = 0, will_y = 0;
	BYTE result = FAIL, error_code = 0;
	int index = 0;
	char szUserName[255];
	USER* pUser = NULL;
	CPoint pt, ptNew;

	int nLength = GetVarString(sizeof(szUserName), szUserName, pBuf, index);
	if(nLength <= 0 || nLength > CHAR_NAME_LENGTH) return;
	
	pUser = GetUser(szUserName);
	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;
	
	if( pUser->m_ZoneIndex < 0 || pUser->m_ZoneIndex >= g_zone.GetSize() ) 
	{
		m_curz = 1;
		m_curx = 192;
		m_cury = 803;
		SetZoneIndex(m_curz);
		return;
	}
	if(pUser->m_curx >= g_zone[m_ZoneIndex]->m_sizeMap.cx || pUser->m_curx < 0)
	{
		m_curz = 1;
		m_curx = 192;
		m_cury = 803;
		SetZoneIndex(m_curz);
		return;
	}
	if(pUser->m_cury >= g_zone[m_ZoneIndex]->m_sizeMap.cy || pUser->m_cury < 0) 
	{
		m_curz = 1;
		m_curx = 192;
		m_cury = 803;
		SetZoneIndex(m_curz);
		return;
	}

	if(strcmp(pUser->m_strUserID, m_strUserID) == 0) return;		// ÀÚ±â ÀÚ½ÅÀº ¾ÈµÊ

	will_x = GetShort( pBuf, index );
	will_y = GetShort( pBuf, index );

	pt = FindNearAvailablePoint_C(will_x, will_y);	// ¸®ÅÏµÇ´Â ÁÂÇ¥µµ Å¬¶óÀÌ¾ðÆ® ÁÂÇ¥´Ù
	if(pt.x == -1 || pt.y == -1) 
	{ 
		// ¿î¿µÀÚ°¡ ¿î¿µÀ» ½±°Ô ÇÏ±â À§ÇØ¼­´Â ¿©±â¼­ ¿î¿µÀÚ¿¡°Ô ½ÇÆÐ¸Þ¼¼Áö¸¦ º¸³»´Â°Ô ÁÁÀ»°Í °°´Ù.
		return;
	}
	
	ptNew = pt;
	pt = ConvertToServer( pt.x, pt.y );
	if(pt.x == -1 || pt.y == -1) return;
	if(pUser->m_curx < 0 || pUser->m_cury < 0) return;//ÐÞÕý×ø±ê±¨´í
	
	::InterlockedExchange(&g_zone[pUser->m_ZoneIndex]->m_pMap[pUser->m_curx][pUser->m_cury].m_lUser, 0);
	::InterlockedExchange(&g_zone[pUser->m_ZoneIndex]->m_pMap[pt.x][pt.y].m_lUser, USER_BAND + pUser->m_uid);
	pUser->m_curx = pt.x;
	pUser->m_cury = pt.y;
	
	result = SUCCESS;

	index = 0;
	SetByte(m_TempBuf, MOVE_CHAT_RESULT, index);
	SetByte(m_TempBuf, result, index);

	if(result == FAIL) 
	{
		Send(m_TempBuf, index);
		return;
	}

	SetInt(m_TempBuf, pUser->m_uid + USER_BAND, index);
	SetShort(m_TempBuf, ptNew.x, index);
	SetShort(m_TempBuf, ptNew.y, index);
	pUser->Send(m_TempBuf, index);	// À¯Àú¿¡°Ô´Â »õ·Î¿î ÁÂÇ¥°ªÀ»...
	
	SendInsight(m_TempBuf, index);
	
	SightRecalc();
	//yskang 0.4
	//TCHAR strOP[1024]; ZeroMemory(strOP,sizeof(strOP));
	//sprintf(strOP,"Remote_User:%s,x=%d,y=%d", pUser->m_strUserID,ptNew.x,ptNew.y);
	//WriteOpratorLog(strOP,CALL_USER_LOG);
}

/*
//////////////////////////////////////////////////////////////////////////////////
//	ÇöÀç À§Ä¡¸¦ Áß½ÉÀ¸·Î 25 ¼¿Áß ¿òÁ÷ÀÏ ¼ö ÀÖ´Â ÁÂÇ¥°¡ ÀÖ´ÂÁö ÆÇ´Ü
//
CPoint USER::FindNearAvailablePoint(int xpos, int ypos)
{
	int i;

	if(IsMovable(xpos, ypos)) return CPoint(xpos, ypos);	// ÇöÀç À§Ä¡

	for(i = 0; i < sizeof(g_8x) / sizeof(g_8x[0]); i++)		// ÁÖº¯ 8¹æÇâ
	{
		if(IsMovable(xpos + g_8x[i], ypos + g_8y[i])) return CPoint(xpos + g_8x[i], ypos + g_8y[i]);
	}

	for(i = 0; i < sizeof(g_16x) / sizeof(g_16x[0]); i++)	// ÁÖº¯ 16¹æÇâ
	{
		if(IsMovable(xpos + g_16x[i], ypos + g_16y[i])) return CPoint(xpos + g_16x[i], ypos + g_16y[i]);
	}

	return CPoint(-1, -1);
}
*/

//////////////////////////////////////////////////////////////////////////////////
//	ÇöÀç À§Ä¡¸¦ Áß½ÉÀ¸·Î 25 ¼¿Áß ¾ÆÀÌÅÛ¸¦ ¶³¾îÁú¼öÀÖ´Â ÁÂÇ¥¸¦ ·¥´ýÀ¸·Î »ý¼º
//
CPoint USER::FindNearRandomPoint(int x, int y)
{
	if( m_ZoneIndex < 0 || m_ZoneIndex >= g_zone.GetSize() ) return CPoint(-1, -1);

	CPoint t;
	int i;
	int iX, iY;
	int rand_x = 1, rand_y = 1;

	MAP *pMap = g_zone[m_ZoneIndex];
	if( !pMap ) return CPoint(-1, -1);
	if( !pMap->m_pMap ) return CPoint(-1, -1);
	if(x >= pMap->m_sizeMap.cx || x < 0 || y >= pMap->m_sizeMap.cy || y < 0) return CPoint(-1, -1);

	int dir[25][2];

	if(g_zone[m_ZoneIndex]->m_pMap[x][y].m_lUser == 0)
	{
		if( IsMovable_S( x, y ) )
		{
			if(x < 0 || y < 0) return CPoint(-1, -1);//ÐÞÕý×ø±ê±¨´í
			if(g_zone[m_ZoneIndex]->m_pMap[x][y].iIndex == -1) return CPoint( x, y );
		}
	}

//	X					Y
	dir[0][0]  =  0;		dir[0][1] =  0;		// 
	dir[1][0]  = -1;		dir[1][1] =  0;		// 
	dir[2][0]  = -1;		dir[2][1] =  1;		// 
	dir[3][0]  =  0;		dir[3][1] =  1;		// 
	dir[4][0]  =  1;		dir[4][1] =  1;		// 

	dir[5][0]  =  1;		dir[5][1] =  0;		// 
	dir[6][0]  =  1;		dir[6][1] = -1;		// 
	dir[7][0]  =  0;		dir[7][1] = -1;		// 
	dir[8][0]  = -1;		dir[8][1] = -1;		// 
	dir[9][0]  = -2;		dir[9][1] = -1;		// 

	dir[10][0] = -2;		dir[10][1] =  0;	// 
	dir[11][0] = -2;		dir[11][1] =  1;	// 
	dir[12][0] = -2;		dir[12][1] =  2;	// 
	dir[13][0] = -1;		dir[13][1] =  2;	// 
	dir[14][0] =  0;		dir[14][1] =  2;	// 

	dir[15][0] =  1;		dir[15][1] =  2;	// 
	dir[16][0] =  2;		dir[16][1] =  2;	// 
	dir[17][0] =  2;		dir[17][1] =  1;	// 
	dir[18][0] =  2;		dir[18][1] =  0;	// 
	dir[19][0] =  2;		dir[19][1] = -1;	// 

	dir[20][0] =  2;		dir[20][1] = -2;	// 
	dir[21][0] =  1;		dir[21][1] = -2;	// 
	dir[22][0] =  0;		dir[22][1] = -2;	// 
	dir[23][0] = -1;		dir[23][1] = -2;	// 
	dir[24][0] = -2;		dir[24][1] = -2;	// 

	rand_x = myrand(1, 24, TRUE);
	rand_y = myrand(0, 1, TRUE);

	iX = dir[rand_x][rand_y] + x;
	iY = dir[rand_x][rand_y] + y;

	rand_x = iX; rand_y = iY;

	if( rand_x >= pMap->m_sizeMap.cx || rand_x < 0 || rand_y >= pMap->m_sizeMap.cy || rand_y < 0) return CPoint(-1, -1);

	if(g_zone[m_ZoneIndex]->m_pMap[rand_x][rand_y].m_lUser == 0)
	{
		if( IsMovable_S( rand_x, rand_y ) )
		{
			if(g_zone[m_ZoneIndex]->m_pMap[rand_x][rand_y].iIndex == -1) return CPoint( rand_x, rand_y );
		}
	}

	rand_x = x, rand_y = y;

	for( i = 1; i < 25; i++)
	{
		iX = rand_x + dir[i][0];
		iY = rand_y + dir[i][1];

		if( iX >= pMap->m_sizeMap.cx || iX < 0 || iY >= pMap->m_sizeMap.cy || iY < 0) continue;
		if(g_zone[m_ZoneIndex]->m_pMap[iX][iY].m_lUser != 0) continue;	// ºó ¸ÊÀÎÁö È®ÀÎÇÑ´Ù.

		if( IsMovable_S( iX, iY ) )
		{
			if(g_zone[m_ZoneIndex]->m_pMap[iX][iY].iIndex == -1) return CPoint( iX, iY );
		}
	}

	return CPoint(-1, -1);
}


//////////////////////////////////////////////////////////////////////////////////
//	x, y ·Î ÀÌµ¿ (x, y) ´Â Å¬¶óÀÌ¾ðÆ® ÁÂÇ¥°è
//
BOOL USER::Move_C(int x, int y, int nDist /* = 1 */)
{
	CPoint t = ConvertToServer(x, y);
	if( t.x == -1 || t.y == -1 ) return FALSE;
	
	if( abs( m_curx - t.x ) > nDist || abs( m_cury - t.y ) > nDist ) return FALSE;

	if(!IsMovable_S(t.x, t.y)) return FALSE;
	if(t.x == m_curx && t.y == m_cury) return FALSE;
	if(m_curx < 0 || m_cury < 0) return FALSE;//ÐÞÕý×ø±ê±¨´í
		// jjs
	if(g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser == 0 || g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser == m_uid + USER_BAND)
	{
		g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser = 0;
	}
	g_zone[m_ZoneIndex]->m_pMap[t.x][t.y].m_lUser = m_uid + USER_BAND;

	m_curx = t.x;
	m_cury = t.y;

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////
//	¼­¹öÁÂÇ¥¸¦ ±âÁØÀ¸·Î x, y °¡ ÀÌµ¿ÇÒ ¼ö ÀÖ´Â Á¡ÀÎÁö ÆÇ´Ü
//
BOOL USER::IsMovable_S(int x, int y)
{
	if( m_ZoneIndex < 0 || m_ZoneIndex >= g_zone.GetSize() ) return FALSE;

	if( !g_zone[m_ZoneIndex] ) return FALSE;
	if( !g_zone[m_ZoneIndex]->m_pMap ) return FALSE;
	if( x >= g_zone[m_ZoneIndex]->m_sizeMap.cx || x < 0 || y >= g_zone[m_ZoneIndex]->m_sizeMap.cy || y < 0) return FALSE;

	if(g_zone[m_ZoneIndex]->m_pMap[x][y].m_bMove) return FALSE;

	int temp_uid = g_zone[m_ZoneIndex]->m_pMap[x][y].m_lUser;

	if( temp_uid != 0 && temp_uid != m_uid + USER_BAND)
	{
		if( temp_uid >= NPC_BAND && temp_uid < INVALID_BAND)
		{
			CNpc *pNpc = NULL;
			pNpc = GetNpc( temp_uid - NPC_BAND );

			if( pNpc == NULL )
			{
				g_zone[m_ZoneIndex]->m_pMap[x][y].m_lUser = 0;
				return TRUE;
			}
			if( pNpc->m_sCurX != x || pNpc->m_sCurY != y )
			{
				if(pNpc->m_sDimension > 0) return FALSE;

				g_zone[m_ZoneIndex]->m_pMap[x][y].m_lUser = 0;
				return TRUE;
			}
		}
		else if(temp_uid >= USER_BAND && temp_uid < NPC_BAND)
		{
			if(temp_uid - USER_BAND < 0 || temp_uid - USER_BAND >= MAX_USER)
			{
				g_zone[m_ZoneIndex]->m_pMap[x][y].m_lUser = 0;
				return TRUE;
			}

			USER *pUser = NULL;
			pUser = GetUser( temp_uid - USER_BAND );

			if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED)
			{
				g_zone[m_ZoneIndex]->m_pMap[x][y].m_lUser = 0;
				return TRUE;
			}

			if( pUser->m_curx != x || pUser->m_cury != y || pUser->m_state != STATE_GAMESTARTED )
			{
				g_zone[m_ZoneIndex]->m_pMap[x][y].m_lUser = 0;
				return TRUE;
			}
		}

		return FALSE;
	}

	return TRUE;
}
BOOL USER::IsMovable_S(int zone, int x, int y)
{
	int ZoneIndex = -1;
	for(int j = 0; j < g_zone.GetSize(); j++)
	{
		if( g_zone[j]->m_Zone == zone )
		{
			ZoneIndex = j;
			break;
		}
	}
	if( ZoneIndex < 0 || ZoneIndex >= g_zone.GetSize() ) return FALSE;

	if( !g_zone[ZoneIndex] ) return FALSE;
	if( !g_zone[ZoneIndex]->m_pMap ) return FALSE;
	if( x >= g_zone[ZoneIndex]->m_sizeMap.cx || x < 0 || y >= g_zone[ZoneIndex]->m_sizeMap.cy || y < 0) return FALSE;

//	CPoint t = ConvertToClient( x, y );		// ¿òÁ÷ÀÌ·Á´Â ¼­¹öÁÂÇ¥°¡ Å¬¶óÀÌ¾ðÆ®¿¡¼­ ¸ø¿òÁ÷ÀÌ´Â ÁÂÇ¥¸é ¸®ÅÏ
//	if( t.x == -1 || t.y == -1 ) return FALSE;

	if(g_zone[ZoneIndex]->m_pMap[x][y].m_bMove) return FALSE;

	int temp_uid = g_zone[ZoneIndex]->m_pMap[x][y].m_lUser;

	if( temp_uid != 0 && temp_uid != m_uid + USER_BAND)
	{
		if( temp_uid >= NPC_BAND && temp_uid < INVALID_BAND)
		{
			CNpc *pNpc = NULL;
			pNpc = GetNpc( temp_uid - NPC_BAND );

			if( pNpc == NULL )
			{
				g_zone[ZoneIndex]->m_pMap[x][y].m_lUser = 0;
				return TRUE;
			}
			if( pNpc->m_sCurX != x || pNpc->m_sCurY != y )
			{
				if(pNpc->m_sDimension > 0) return FALSE;

				g_zone[ZoneIndex]->m_pMap[x][y].m_lUser = 0;
				return TRUE;
			}
		}
		else if(temp_uid >= USER_BAND && temp_uid < NPC_BAND)
		{
			if(temp_uid - USER_BAND < 0 || temp_uid - USER_BAND >= MAX_USER)
			{
				g_zone[ZoneIndex]->m_pMap[x][y].m_lUser = 0;
				return TRUE;
			}

			USER *pUser = NULL;
			pUser = GetUser( temp_uid - USER_BAND );

			if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED)
			{
				g_zone[ZoneIndex]->m_pMap[x][y].m_lUser = 0;
				return TRUE;
			}

			if( pUser->m_curx != x || pUser->m_cury != y || pUser->m_state != STATE_GAMESTARTED )
			{
				g_zone[ZoneIndex]->m_pMap[x][y].m_lUser = 0;
				return TRUE;
			}
		}

		return FALSE;
	}

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////
//	ÇöÀç À¯ÀúÀÇ À§Ä¡¿Í (xpos, ypos) »çÀÌÀÇ °Å¸®¸¦ °è»ê
//
BOOL USER::GetDistance(int xpos, int ypos, int dist, int* ret)
{
	if( m_ZoneIndex < 0 || m_ZoneIndex >= g_zone.GetSize() ) return FALSE;
	if(xpos >= g_zone[m_ZoneIndex]->m_sizeMap.cx || xpos < 0 || ypos >= g_zone[m_ZoneIndex]->m_sizeMap.cy || ypos < 0) return FALSE;

	int dx = abs(xpos - m_curx);
	int dy = abs(ypos - m_cury);

	if(ret != NULL) *ret = (dx + dy) / 2;

	if((dx + dy) > (dist * 2)) return FALSE;
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////
//	Ã³À½ ¶Ù·Á°í ÇÒ ¶§ÀÇ Ã³¸®
//
void USER::RunFirstReq(TCHAR *pBuf)
{
	BYTE result = FAIL;
	int index = 0;
	int wx_1, wy_1, x1, x2;
	int wx_2, wy_2, y1, y2;
//	int ex, ey;
	DWORD checkTick;

	CPoint ptOld(-1, -1);

	if(GetSP() <= 0) 
	{
		SendSystemMsg( IDS_USER_NOT_ENOUGH_STAMINA, SYSTEM_NORMAL, TO_ME);//"ÄãÃ»ÓÐ×ã¹»µÄ¾«Á¦"
		goto fail_result;
	}

	x1 = wx_1 = GetShort( pBuf, index );
	y1 = wy_1 = GetShort( pBuf, index );
	x2 = wx_2 = GetShort( pBuf, index );
	y2 = wy_2 = GetShort( pBuf, index );
	m_tDir = GetByte( pBuf, index );


	checkTick = GetDWORD(pBuf, index);
	if(CheckRunSpeedHack(checkTick, x1 == x2)) return;

	if(IsMovable_C(x1, y1) == FALSE) goto fail_result;
	if(Move_C(x2, y2, 2) == FALSE)
	{
		if(Move_C(x1, y1, 2) == FALSE) goto fail_result;
		wx_2 = wy_2 = -1;	
	}
	
	DecSP(1);
	index = 0;
	result = SUCCESS;
	SetByte(m_TempBuf, RUN_FIRST_RESULT, index );
	SetByte(m_TempBuf, result, index);
	SetInt	(m_TempBuf, m_uid + USER_BAND, index);
	SetShort(m_TempBuf, wx_1, index);
	SetShort(m_TempBuf, wy_1, index);
	SetShort(m_TempBuf, wx_2, index);
	SetShort(m_TempBuf, wy_2, index);

	SendInsight(m_TempBuf, index);
	SightRecalc();
	m_dwLastMoveAndRun = GetTickCount();

	SendSP();
	return;

fail_result:
	index = 0;

	ptOld = ConvertToClient(m_curx, m_cury);
	if(ptOld.x < 0 || ptOld.y < 0) return;

	SetByte(m_TempBuf, RUN_FIRST_RESULT, index );
	SetByte(m_TempBuf, FAIL, index);
	SetInt(m_TempBuf, m_uid + USER_BAND, index );
	SetShort(m_TempBuf, ptOld.x, index );
	SetShort(m_TempBuf, ptOld.y, index );

	SendInsight(m_TempBuf, index);
}

/////////////////////////////////////////////////////////////////////////////////////
//	¶Ù°í ÀÖ´Â ÁßÀÇ Ã³¸®
//
void USER::RunReq(TCHAR *pBuf)
{
	BYTE result = FAIL;
	int index = 0;
	int wx_1, wy_1, x1, y1;
	int wx_2, wy_2, x2, y2;
	DWORD checkTick;

	CPoint ptOld;

	if(GetSP() <= 0) 
	{
		SendSystemMsg( IDS_USER_NOT_ENOUGH_STAMINA, SYSTEM_NORMAL, TO_ME);//"ÄãÃ»ÓÐ×ã¹»µÄ¾«Á¦"
		goto fail_result;
	}

	x1 = wx_1 = GetShort( pBuf, index );
	y1 = wy_1 = GetShort( pBuf, index );
	x2 = wx_2 = GetShort( pBuf, index );
	y2 = wy_2 = GetShort( pBuf, index );
	m_tDir = GetByte( pBuf, index );

	checkTick = GetDWORD(pBuf, index);
	if(CheckRunSpeedHack(checkTick, x1 == x2)) return;

	if(IsMovable_C(x1, y1) == FALSE) goto fail_result;
	if(Move_C(x2, y2, 2) == FALSE)
	{
		if(Move_C(x1, y1, 2) == FALSE) goto fail_result;
		wx_2 = wy_2 = -1;
	}

	DecSP(1);

	index = 0;
	result = SUCCESS;
	SetByte(m_TempBuf, RUN_RESULT, index );
	SetByte(m_TempBuf, result, index);

	SetInt	(m_TempBuf, m_uid + USER_BAND, index);
	SetShort(m_TempBuf, wx_1, index);
	SetShort(m_TempBuf, wy_1, index);
	SetShort(m_TempBuf, wx_2, index);
	SetShort(m_TempBuf, wy_2, index);
	
	SendInsight(m_TempBuf, index);
	SightRecalc();

	m_dwLastMoveAndRun = GetTickCount();
	SendSP();	//^^ ½ºÅ×¹Ì³Ê Á¤º¸¸¦ °°ÀÌ ³ÖÅä·Ï

	return;

fail_result:
	index = 0;
	ptOld = ConvertToClient(m_curx, m_cury);
	if(ptOld.x < 0 || ptOld.y < 0) return;

	SetByte(m_TempBuf, RUN_RESULT, index );
	SetByte(m_TempBuf, FAIL, index);
	SetInt(m_TempBuf, m_uid + USER_BAND, index );
	SetShort(m_TempBuf, ptOld.x, index );
	SetShort(m_TempBuf, ptOld.y, index );

	SendInsight(m_TempBuf, index);
}

//////////////////////////////////////////////////////////////////////////////////////
//	¶Ù´Ù°¡ µµÂøÁ¡¿¡ µµ´ÞÇßÀ» ¶§ÀÇ Ã³¸®
//
void USER::RunEndReq(TCHAR *pBuf)
{
	BYTE result = FAIL;
	int index = 0;
	int wx_1, wy_1, x1, y1;
	int wx_2, wy_2, x2, y2;
	CPoint ptOld;
	DWORD checkTick;

	if(GetSP() <= 0) goto fail_result;

	x1 = wx_1 = GetShort( pBuf, index );
	y1 = wy_1 = GetShort( pBuf, index );
	x2 = wx_2 = GetShort( pBuf, index );
	y2 = wy_2 = GetShort( pBuf, index );
	m_tDir = GetByte( pBuf, index );

	checkTick = GetDWORD(pBuf, index);
	if(CheckRunSpeedHack(checkTick, x1 == x2)) return;

	if(x1 == x2 && y1 == y2)
	{
		if(Move_C(x1, y1, 2) == FALSE) goto fail_result;
		wx_2 = wy_2 = -1;
	}
	else
	{
		if(IsMovable_C(x1, y1) == FALSE) goto fail_result;
		if(Move_C(x2, y2, 2) == FALSE)
		{
			if(Move_C(x1, y1, 2) == FALSE) goto fail_result;
			wx_2 = wy_2 = -1;
		}
	}

	DecSP(1);

	index = 0;
	result = SUCCESS;
	SetByte(m_TempBuf, RUN_RESULT, index );
	SetByte(m_TempBuf, result, index);

	SetInt	(m_TempBuf, m_uid + USER_BAND, index);
	SetShort(m_TempBuf, wx_1, index);
	SetShort(m_TempBuf, wy_1, index);
	SetShort(m_TempBuf, wx_2, index);
	SetShort(m_TempBuf, wy_2, index);
	
	SendInsight(m_TempBuf, index);
	SightRecalc();

	m_dwLastMoveAndRun = GetTickCount();
	SendSP();	//^^ ½ºÅ×¹Ì³Ê Á¤º¸¸¦ °°ÀÌ ³ÖÅä·Ï

	return;

fail_result:
	index = 0;
	ptOld = ConvertToClient(m_curx, m_cury);
	if(ptOld.x < 0 || ptOld.y < 0) return;

	SetByte(m_TempBuf, RUN_END_RESULT, index );
	SetByte(m_TempBuf, FAIL, index);
	SetInt(m_TempBuf, m_uid + USER_BAND, index );
	SetShort(m_TempBuf, ptOld.x, index );
	SetShort(m_TempBuf, ptOld.y, index );

	SendInsight(m_TempBuf, index);
}
///////////////////////////////////////////////////////////////////////////////////
//	ÇöÀç SP ¸¦ ¾ò´Â´Ù.
//
short USER::GetSP()
{
	BYTE	tWeaponClass = 255;

	int iSkillSid = 0;
	int iLevel = 0, iRandom = 0, iSP = 0;

	double dPlusSP = 1.0;
	int iAddSP = 0;

	DWORD dwDiffTime = GetTickCount() - m_dwLastMoveAndRun;

	if(dwDiffTime >= m_iSPIntervalTime)
	{
		if(IsCanUseWeaponSkill(tWeaponClass))	// ÇöÀç ÀÚ½ÅÀÌ ¿À¸¥¼Õ¿¡ µç ¹«±âÀÇ ½ºÅ³ÀÌ ÀÖÀ¸¸é
		{
			int tClass = tWeaponClass * SKILL_NUM; 

			for(int i = tClass; i < tClass + SKILL_NUM; i++)			// ½ºÅ³Áß SPÈ¸º¹ ½ºÅ³ÀÌ ÀÖÀ¸¸é ¹Ý¿µÇÑ´Ù.
			{
				iSkillSid = m_UserSkill[i].sSid;

				if(iSkillSid == SKILL_SP_RECOVER_UP)					// 13 index
				{
					iLevel = m_UserSkill[i].tLevel;							

					// ¾ÆÀÌÅÛ¿¡ ÀÇÇÑ ½ºÅ³ º¯µ¿ ·¹º§
					if(g_DynamicSkillInfo[iSkillSid] >= MAGIC_COUNT) continue;
					if(iLevel >= 1) iLevel += m_DynamicUserData[g_DynamicSkillInfo[iSkillSid]] + m_DynamicUserData[MAGIC_ALL_SKILL_UP];
				
					//if(iLevel > SKILL_LEVEL) iLevel = SKILL_LEVEL;
					if(iSkillSid >= g_arSkillTable.GetSize()) continue;
					if(iLevel < 0 || iLevel >= SKILL_LEVEL) iLevel = SKILL_LEVEL - 1;

					iRandom = (int)((double)XdY(1, 1000) / 10 + 0.5);
					if(iRandom < g_arSkillTable[iSkillSid]->m_arSuccess.GetAt(iLevel)) iSP = 1;

					dPlusSP = 1.0 + (double)iSP * g_arSkillTable[iSkillSid]->m_arRepair.GetAt(iLevel) / 100.0;
				}
			}
		}
							// º¸Á¤°ª
		if(m_iSPRate <= 0) 
		{
			m_iSPRate = 1;
			TRACE("È¸º¹ºñÀ² ¿¡·¯\n");
		}

		if(IsCity() && CheckRecoverTableByClass()) iAddSP = g_arRecoverTable[m_byClass]->m_byTown;
		m_sSP += (int)(dPlusSP * (double)m_sMagicCON/m_iSPRate ) + iAddSP;
		if(m_sSP > m_sMagicMaxSP) m_sSP = m_sMagicMaxSP;

		m_dwLastMoveAndRun = GetTickCount();
	}

	return m_sSP;
}

BOOL USER::CheckRecoverTableByClass()
{
	if(m_byClass < 0 || m_byClass >= g_arRecoverTable.GetSize()) return FALSE;

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////
//	SP ¸¦ °¨¼Ò½ÃÅ²´Ù.
//
short USER::DecSP(short sDec)
{
	m_sSP -= sDec;
	if(m_sSP < 0) m_sSP = 0;

	return m_sSP;
}

////////////////////////////////////////////////////////////////////////////////////
//	SP ¸¦ Å¬¶óÀÌ¾ðÆ®·Î º¸³½´Ù.
//
void USER::SendSP(BOOL bLevelDown)
{
	CBufferEx TempBuf;

	if(m_bLive == USER_DEAD && !bLevelDown) return;

	if(m_sSP > m_sMagicMaxSP) m_sSP = m_sMagicMaxSP;

	TempBuf.Add(SET_STAEMINA);
	TempBuf.Add(m_sSP);

	Send(TempBuf, TempBuf.GetLength());
}

//######################################################################
//
//	Character Chatting
//
//######################################################################


////////////////////////////////////////////////////////////////////////
//	Ã¤ÆÃ Ã³¸®(¹öÆÛ ±æÀÌ¸¦ ¼³Á¤)
//
void USER::ChatReqWithLength(TCHAR *pBuf, int iLen)
{
	int index = 0;
	BYTE type = NORMAL_CHAT;
    CString moonname =_T("");

	type = GetByte(pBuf, index);

	/*CString str = _T("");
	str.Format("%s",pBuf);
	int nSize = strlen(str);
	if(nSize <= 0 || nSize > 45)//×Ö´®ÐÞ¸´ MarkJay
	{
		SendSystemMsg("³¤¶ÈÔÚ45¸ö×ÖÄ¸,21¸öºº×ÖÖ®¼ä!", SYSTEM_ERROR,TO_ME);
		return;
	}
	strncpy(pBuf,(LPCTSTR)str,sizeof(pBuf));//MarkJay ÐÞ¸´*/

//////////////////////¹ýÂË·Ç·¨×Ö·û////////////////////////////
	if(f_Markini[0]->DLGL_KG == 1) //¹ýÂË¿ª¹Ø
	{
		CString thestring = _T("");
		thestring.Format("%s",pBuf);
		int nSize = sizeof(theSpack)/sizeof(char*);
		for(int i = 0; i < nSize;i++)
		{		
			thestring.Replace(theSpack[i],"*");	  
		}
		strncpy(pBuf,(LPCTSTR)thestring,sizeof(pBuf));//MarkJay ÐÞ¸´
	}
//////////////////////////////////////////////////////////////
	if( pBuf[index+1] == '/' && type != WHISPER_CHAT )
	{
		ExecuteChatCommand( pBuf+index+2, iLen );
		return;
	}

    if(m_bLive == USER_DEAD ||  m_state != STATE_GAMESTARTED || m_bZoneLogOut)
	{
         return;	
	}

	switch( type )
	{
	case NORMAL_CHAT:		// ÀÏ¹ÝÃ¤ÆÃ
		NormalChat(pBuf + index);
		break;

	case WHISPER_CHAT:		// Ë½ÁÄ
		WhisperChat(pBuf + index, iLen - index);//ÐÞ¸´ÃÜÓï±¨´í MarkJay
		break;

	case SHOUT_CHAT:		// 8 È­¸é ¿ÜÄ§
		ShoutChat(pBuf + index);
		break;

	case BUDDY_INVITE: case BUDDY_OFF:	// Buddy ÃÊÃ», Å»Åð
		BuddyUserChange(pBuf + index, type, iLen - index);//ÐÞ¸´×é¶Ó±¨´í Ð¡ºÚ
		break;
	case BUDDY_CHAT:		// Buddy Ã¤ÆÃ
		BuddyChat(pBuf + index);
		break;

	case ALL_CHAT:						// ÀüÃ¼ Ã¤ÆÃ
		if(m_tIsOP != 1) break;			// Áö±ÝÀº ¿î¿µÀÚ¸¸ ÇÑ´Ù.
		SendAllChat(pBuf + index);
		break;

	case WHISPER_OK:
	case WHISPER_DENY:
		WhisperOnOff(pBuf);
		break;

	case PK_COUNT_CHAT:
		SendPKCount();
		break;

	case MOP_CHAT:
		if(m_tIsOP != 1) break;
		MopCatchReq(pBuf + index);
		break;
	case MOP_CALL_CHAT:		// 17 ¸÷À» ¼ÒÈ¯	¿î¿µÀÚ ±ÙÃ³·Î... 
		if(m_tIsOP != 1) break;
		MopCallReq(pBuf + index);
		break;

	case CALL_CHAT:			// 15 À¯Àú¸¦ ¼ÒÈ¯
		moonname=m_strUserID;	
		if( m_tIsOP != 1 ) break;
		UserCallReq(pBuf + index);
		break;

	case REMOTE_CHAT:		// 16 Æ¯Á¤À§Ä¡·Î Æ¯Á¤ À¯Àú¸¦ ÀÌµ¿
		if(m_tIsOP != 1) break;
		UserRemoteCallReq(pBuf + index);
		break;

	case GUILD_CHAT:
		GuildChat(pBuf + index);
		break;
//	case GUILD_INVITE:		// 18 ±æµå °¡ÀÔÀ» ÀÇ·ÚÇÑ´Ù.	
//		GuildInvite(pBuf + index);
//		GuildInviteWithThread(pBuf + index);
//		break;
	case GUILD_OFF:			// 20 ±æµå Å»Åð
		GuildOff(pBuf + index);
//		GuildOffWithThread(pBuf + index);
		break;
	case GUILD_KICK_OUT:	// 21 ±æµå¿ø °­Á¦ Ãß¹æ
    	GuildKickOut(pBuf + index);
	//	GuildKickOutWithThread(pBuf + index);
		break;
	case GUILD_DISPERSE:
		GuildDisperse(pBuf + index);
//		GuildDisperseWithThread(pBuf + index);
		break;

	case ITEM_MODE_CHAT:	// 22 ³²ÀÌÁÖ´Â ¾ÆÀÌÅÛÀ» ½Â³«, °ÅºÎ¸¦ ¼³Á¤.
		SetItemMode(pBuf + index);
		break;

	case GUILD_FIELD_WAR:
		SendGuildWarFieldApply(pBuf + index);
		break;

	case GUILD_SURRENDER:	// 26 ÇÊµåÀüÀ» Ç×º¹ÇÑ´Ù.
		{
			CString strMsg = _T("");
			strMsg.Format( IDS_USER_GUILD_DEFEAT, m_strGuildName);//"%s ¾üÍÅ±»´ò°Ü."
			SendGuildWarFieldEnd((LPTSTR)(LPCTSTR)strMsg);// Ç×º¹
		}
		break;

	case COPY_TELEPORT:
		break;

	case GUILD_USER_CHECK:
		GuildUserAllInforChat();
		break;

	case GUILD_WAR_APPLY_LIST:
		SendFortressAttackGuildList(pBuf + index);
		break;

	case GUILD_FORTRESS_DOOR:
		OpenFortressDoor();
		break;

	case GUILD_DEPUTE_POWER:  //¸±ÍÅÊÚÈ¨
		SendSystemMsg("ÊÚÈ¨¶Ô·½", SYSTEM_NORMAL, TO_ME );
		DeputeGuildMasterOfPower(pBuf + index);
		break;

	case GUILD_STOP_POWER:  //ÖÕÖ¹ÊÚÈ¨
		SendSystemMsg("ÖÕÖ¹ÊÚÈ¨", SYSTEM_NORMAL, TO_ME );
		RemoveGuildMasterOfPower(pBuf + index);
		break;

	case GUILD_CALL_CHAT: //yskang 0.2 (35) Æ÷Æ®¸®½º ±æ¸¶°¡ À¯Àú¼ÒÈ¯
		GuildUserCallReq(pBuf + index, iLen - index); //ÐÞ¸´¾üÍÅ·¢°ü Ð¡ºÚ
		break;
	case GUILD_USER_INFO_CALL://yskang 0.2
		m_nGuildUserInfoType = 0x03;
		GuildUserInfoReq(pBuf + index);
		break;
	}	
}

void USER::ChatReqWithThread(int length, TCHAR *pBuf)
{
	int head;
	BYTE *pData;
	CHATDATAPACKET *pCDP;

	pCDP = new CHATDATAPACKET;
	if ( pCDP == NULL )	return;

	pData = new BYTE[length+1];
	if ( pData == NULL )
	{
		delete pCDP;
		return;
	}

	memcpy(pData, pBuf, length);
	pCDP->pData = pData;
	pCDP->pData[length] = '\0';
	pCDP->dcount = length;
	pCDP->code = CHAT_REQ;
	pCDP->UID = m_uid;

	if ( m_iModSid < 0 || m_iModSid > AUTOMATA_THREAD )
	{
		delete pCDP;
		return;
	}

	head = g_nChatDataHead[m_iModSid];

	g_WaitRecvDataChat[head][m_iModSid] = pCDP;
	
	head++;
	head %= DATA_BUF_SIZE_FOR_THREAD;
	g_nChatDataHead[m_iModSid] = head;
}

/////////////////////////////////////////////////////////////////////////
//	¿äÃ»ÇÑ PK Count¸¦ º¸³½´Ù. 
//
void USER::SendPKCount()
{
	CString strMsg = _T("");
	strMsg.Format( IDS_USER_PK_COUNT, m_sKillCount);
	SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);	//"ÄãµÄPKÖµÎª %d ."
}

/////////////////////////////////////////////////////////////////////////
//	ÏµÍ³¹«¸æ
void USER::SendAllChat(TCHAR *pBuf)
{
	CBufferEx TempBuf;

	TempBuf.Add(SYSTEM_MSG);
	TempBuf.Add(SYSTEM_ANNOUNCE);
	TempBuf.AddString(pBuf + 1);

	USER* pUser = NULL;

	for(int i = 0; i < MAX_USER; i++)
	{
		pUser = GetUser(i);

		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) continue;

		pUser->Send(TempBuf, TempBuf.GetLength());
	}
	//yskang 0.4
	//TCHAR strOP[1024]; ZeroMemory(strOP,sizeof(strOP));
	//sprintf(strOP,"GM¹«¸æ:%s", pBuf);
	//WriteOpratorLog(strOP,CHAT_LOG);
}

/////////////////////////////////////////////////////////////////////////
//	¸÷À» Æ¯Á¤À§Ä¡·Î ÀÌµ¿½ÃÅ²´Ù.
//
void USER::MopCallReq(TCHAR *pBuf)
{
	int index = 0;
	char szNpcName[255];

	if(!GetVarString(sizeof(szNpcName), szNpcName, pBuf, index)) return;

	CNpc* pNpc = GetNpc(szNpcName);
	if(pNpc == NULL)
	{
		SendSystemMsg( IDS_USER_INVALID_MONSTER_NAME, SYSTEM_NORMAL, TO_ME);//"ÒªÕÙ»ØµÄÃû³Æ²»¶Ô."
		return;
	}

	short x = GetShort(pBuf, index);
	short y = GetShort(pBuf, index);

	CPoint ptCell = ConvertToServer( x, y );
	ptCell = FindNearAvailablePoint_S(ptCell.x, ptCell.y);

	if(ptCell.x != -1 && ptCell.y != -1) 
	{
		GetNpcData(pNpc, ptCell.x-1, ptCell.y-1);//ÐÞ¸´ÕÙ»½¹ÖÎïµ²×ÅÈË MarkJay
		//yskang 0.4
		//TCHAR strOP[1024]; ZeroMemory(strOP,sizeof(strOP));
		//sprintf(strOP,"ÕÙ»Ø¹ÖÎï:%s,x=%d,y=%d", szNpcName,ptCell.x, ptCell.y);
		//WriteOpratorLog(strOP,CHAT_LOG);
	}
	else SendSystemMsg( IDS_USER_INVALID_POINT_CALL_MONSTER, SYSTEM_NORMAL, TO_ME);//"ÎÞ·¨ÕÒµ½ÄÜÕÙ»ØµÄ³¡Ëù¡£"
}

/////////////////////////////////////////////////////////////////////////
//	ÆÕÍ¨Ëµ»°
void USER::NormalChat(TCHAR *pBuf)
{
	if(m_dwCloseTime > 0)
	{
		SendSystemMsg( "¶Ô²»Æð,¼àÓüÖÐ²»ÔÊÐí·¢ÑÔ.", SYSTEM_ERROR, TO_ME);
		return;      
	}

	CBufferEx TempBuf;
	TempBuf.Add(CHAT_RESULT);
	TempBuf.Add(SUCCESS);
	TempBuf.Add(NORMAL_CHAT);
	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.AddString(m_strUserID);
	if(m_CheckCode != 0)
	{
		CString str = _T("");
		CString Codestr = _T("");
		Codestr.Format("%d",m_CheckCode);
		if(m_CodeFailNum >= 3)
		{
			Closeuser(2);
			SendSystemMsg(_T("ÑéÖ¤Âë·¢ËÍ¶à´Î´íÎó\r\n\r\n±»¹Ø½ø¼àÓü."),SYSTEM_SPECIAL,TO_ME);
			return;
		}
		if(strcmp((LPTSTR)(LPCTSTR)Codestr, pBuf + 1) == 0)
		{
			ResetJiaoBenCheck();
			SendSysWhisperChat(_T("ÑéÖ¤Í¨¹ý,×£ÄúÓÎÏ·Óä¿ì!"));
			return;
		}
		else
		{
			m_CodeFailNum++;
			str.Format("[¶Ô²»Æð,ÑéÖ¤´íÎó!\r\n\r\nÑéÖ¤ÂëÎª%s\r\n\r\n\r\n\r\n\r\n", (LPTSTR)(LPCTSTR)UserCodeStr);
			SendSystemMsg((LPTSTR)(LPCTSTR)str, SYSTEM_CESHIA, TO_ME);
			return;
		}
	}
	else
	{
		TempBuf.AddString(pBuf + 1);
	}

	SendScreen(TempBuf, TempBuf.GetLength());
}
//////////////////////////////////////////////////////////////////////////
//	ÃÜÓïËµ»°
void USER::WhisperChat(TCHAR *pBuf,int iLen)
{
	CBufferEx TempMyBuf;
	CBufferEx TempYouBuf;
	BYTE result = FAIL, error_code = 0;
	int index = 0;
	char szUserName[255];
	USER* pUser = NULL;
	if(m_dwCloseTime > 0)
	{
		SendSystemMsg( "¶Ô²»Æð,¼àÓüÖÐ²»ÔÊÐí·¢ÑÔ.", SYSTEM_ERROR, TO_ME);
		return;      
	}

	if(m_sKillCount >= 100) 
	{
		SendSystemMsg( IDS_USER_TOO_LOW_CITY_VALUE, SYSTEM_ERROR, TO_ME);// "°ÔÍõÎÞ·¨Ê¹ÓÃ."
		return;
	}

	//int nLength = GetVarString(sizeof(szUserName), szUserName, pBuf, index);
	int nLength = GetVarString(iLen, szUserName, pBuf, index);//ÐÞ¸´ÃÜÓï±¨´í MarkJay
	if(nLength <= 0 || nLength > CHAR_NAME_LENGTH) // Àß¸øµÈ À¯Àú¾ÆÀÌµð
	{
		error_code = ERR_6;
		goto result_send;
	}

	pUser = GetUser(szUserName);
	if(!pUser) // ±Ó¸» »ó´ë ¾øÀ½
	{
		g_pMainDlg->BridgeServerWhisperChatReq( m_uid, m_strUserID, szUserName, pBuf + 1 + nLength + 1 );
		return;
	}

	if(pUser->m_state != STATE_GAMESTARTED)  // ÇöÀç °ÔÀÓ ÁøÇàÁßÀÌ ¾Æ´Ô...
	{
		error_code = ERR_3;
		goto result_send;
	}

	if(pUser->m_bWhisper == FALSE) // »ó´ëÆíÀÌ ±Ó¸» °ÅºÎÁß
	{
		error_code = ERR_2;
		goto result_send;
	}

	if(pUser->m_tIsOP == 1)					// ¿î¿µÀÚ¿¡°Ô ±Ó¸»Àº ¾ÈµÊ
	{
		error_code = ERR_3;
		goto result_send;
	}

	if(m_strUserID == pUser->m_strUserID)	// ÀÚ±âÀÚ½Å¿¡°Ô ±Ó¸» -> ¸®ÅÏ
	{
		return;
	}
	result = SUCCESS;

result_send:
	TempMyBuf.Add(CHAT_RESULT);
	TempMyBuf.Add(result);

	if(result != SUCCESS)
	{
		TempMyBuf.Add(error_code);
		Send(TempMyBuf, TempMyBuf.GetLength());
		return;
	}

	TempMyBuf.Add(WHISPER_CHAT);
	TempMyBuf.Add((int)0x00);								// »ó´ë¹æ ¾ÆÀÌµð¸¦ ³ª¿¡°Ô º¸³½´Ù.(ÀÚµ¿ ÀúÀåÀ§ÇØ)
	TempMyBuf.AddString(pUser->m_strUserID);
	TempMyBuf.AddString(pBuf + 1 + nLength + 1);			// pBuf == IDLen(1) + ID + MsgLen(1) + Msg
	Send(TempMyBuf, TempMyBuf.GetLength());
	
	TempYouBuf.Add(CHAT_RESULT);
	TempYouBuf.Add(result);
	TempYouBuf.Add(WHISPER_CHAT);
	TempYouBuf.Add((int)0x01);
	TempYouBuf.AddString(m_strUserID);
	TempYouBuf.AddString(pBuf + 1 + nLength + 1);
	pUser->Send(TempYouBuf, TempYouBuf.GetLength());

	//TCHAR strOP[1024]; ZeroMemory(strOP,sizeof(strOP));
	//sprintf(strOP,"Whisper_Chat:%s,%s", szUserName, pBuf + 1 + nLength + 1);
	//WriteOpratorLog(strOP,CHAT_LOG);
}
///////////////////////////////////////////////////
// ÄÅº°Ëµ»°
void USER::ShoutChat(TCHAR *pBuf)
{
	USER* pUser = NULL;

	if(m_sLevel < 30)
	{
		SendSystemMsg( IDS_USER_TOO_LOW_LEVEL_FOR_SHOUT, SYSTEM_NORMAL, TO_ME);//"¼¶±ðÌ«µÍ²»ÄÜÄÅº°."
		return;
	}

	short sp = GetSP();

	if(sp < 10)
	{
		SendSystemMsg( IDS_USER_NOT_ENOUGH_STAMINA_FOR_SHOUT, SYSTEM_NORMAL, TO_ME);//"ÄãÃ»ÓÐ×ã¹»µÄÌåÁ¦ÄÅº°."
		return;
	}
	if(GetTickCount() - m_dwLastShoutChatTime < 5 * 1000)//Ð¡ÓÚ5Ãë²»ÄÜÄÅº°
	{
		SendSystemMsg("¶Ô²»Æð,Ã¿´ÎÄÅº°¼ä¾àÊ±¼äÎª5Ãë.", SYSTEM_ERROR, TO_ME);
		return;
	}
	if(m_sKillCount >= 100)
	{
		SendSystemMsg( IDS_USER_TOO_LOW_CITY_VALUE, SYSTEM_ERROR, TO_ME);//"°ÔÍõÎÞ·¨Ê¹ÓÃ."
		return;
	}
	if(m_dwCloseTime > 0)
	{
		SendSystemMsg( "¶Ô²»Æð,¼àÓüÖÐ²»ÔÊÐí·¢ÑÔ.", SYSTEM_ERROR, TO_ME);
		return;      
	}
	if(m_dwDN < 5000)
	{
		SendSystemMsg( IDS_USER_NOT_ENOUGH_PAY, SYSTEM_ERROR, TO_ME);//"Ç®²»¹»."
		return; 
	}
//----------Ê¹ÓÃ½ð±ÒÄÅº°------------
	if( m_dwDN <= 5000 ) m_dwDN = 0;
	else m_dwDN = m_dwDN - 5000;
	UpdateUserItemDN();						
	SendMoneyChanged();
//----------------------------------
	DecSP(10);			// ½ºÅ×¹Ì³ª °¨¼Ò
	CBufferEx TempBuf;
	TempBuf.Add(CHAT_RESULT);
	TempBuf.Add(SUCCESS);
	TempBuf.Add(SHOUT_CHAT);
	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.AddString(m_strUserID);
	TempBuf.AddString(pBuf + 1);	

	SendSP();
//-----------------------------È«·þÍæ¼Ò¿É¼û------------------------------
	for(int i = 0; i < MAX_USER; i++)
	{
		pUser = GetUser(i);

		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) continue;

		pUser->Send(TempBuf, TempBuf.GetLength());
	}
//-----------------------------------------------------------------------
	m_dwLastShoutChatTime = GetTickCount();
}
//////////////////////////////////////////////////////////////////////////
//	±Ó¸» Çã¿ë/°ÅºÎ
//
void USER::WhisperOnOff(TCHAR *pBuf)
{
	int index = 0;
	BYTE type = GetByte(pBuf, index);

	if(type == WHISPER_OK) m_bWhisper = TRUE;
	else m_bWhisper = FALSE;

	CBufferEx TempBuf;

	TempBuf.Add(CHAT_RESULT);
	TempBuf.Add(SUCCESS);
	TempBuf.Add(type);
	
	Send(TempBuf, TempBuf.GetLength());

	g_pMainDlg->BridgeServerUserWhisperOnOff( m_uid, (BYTE)m_bWhisper );
}

///////////////////////////////////////////////////////////////////////////
//	Æ¯Á¤ À¯Àú°¡ °ÔÀÓ¿¡ Á¢¼ÓÇÏ°í ÀÖ´ÂÁö Ã£¾Æº»´Ù.
//
void USER::FindUserReq(TCHAR *pBuf)
{
	CBufferEx TempBuf;
	BYTE result = 1;
	int index = 0;
	char szUserName[255];
	USER* pUser = NULL;

	int nLength = GetVarString(sizeof(szUserName), szUserName, pBuf, index);
	if(nLength <= 0 || nLength > CHAR_NAME_LENGTH) // Àß¸øµÈ À¯Àú¾ÆÀÌµð
	{
		result = ERR_1;
		goto result_send;
	}

	pUser = GetUser(szUserName);
	if(!pUser) // »ó´ë ¾øÀ½
	{
		result = ERR_2;
		goto result_send;
	}

	result = 0;

result_send:
	TempBuf.Add(FIND_USER_RESULT);
	TempBuf.Add(result);
	TempBuf.AddString(szUserName);

	Send(TempBuf, TempBuf.GetLength());
}
////////////////////////////////////////////////////////////////////////////
// ×é¶ÓËµ»°
void USER::BuddyChat(TCHAR *pBuf)
{
	int i;
	if(!m_bNowBuddy) 
	{
		SendSystemMsg( IDS_USER_MAKE_BUDDY_FIRST, SYSTEM_NORMAL, TO_ME);//"×é¶ÓÆµµÀ±ØÐëÔÚ¶ÓÎéÖÐ²Å¿ÉÊ¹ÓÃ."
		return;
	}
	if(m_dwCloseTime > 0)
	{
		SendSystemMsg( "¶Ô²»Æð,¼àÓüÖÐ²»ÔÊÐí·¢ÑÔ.", SYSTEM_ERROR, TO_ME);
		return;
	}

	USER *pUser = NULL;

	CBufferEx TempBuf;
	TempBuf.Add(CHAT_RESULT);
	TempBuf.Add(SUCCESS);
	TempBuf.Add(BUDDY_CHAT);
	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.AddString(m_strUserID);
	TempBuf.AddString(pBuf + 1);

	for(i = 0; i < MAX_BUDDY_USER_NUM; i++)
	{
		if(m_MyBuddy[i].uid	>= USER_BAND && m_MyBuddy[i].uid <= NPC_BAND) 
		{
			pUser = GetUser(m_MyBuddy[i].uid - USER_BAND);
			if(!pUser) continue;
			if(pUser->m_state != STATE_GAMESTARTED) continue;
			if(strcmp(pUser->m_strUserID, m_MyBuddy[i].m_strUserID) != 0) continue;

			pUser->Send(TempBuf, TempBuf.GetLength());
		}
	}
}
////////////////////////////////////////////////////////////////////////////
//	Buddy Mode ¸¦ On/Off ÇÑ´Ù
//
void USER::SetBuddyMode(TCHAR *pBuf)
{
/*	int index = 0;
	BOOL bMode = (BOOL)GetByte(pBuf, index);

	m_bBuddyMode = bMode;

	if(m_bBuddyMode == FALSE)	// ÃÊ±âÈ­ ÇÑ´Ù.
	{
		int iCount = m_MyBuddy.GetSize();
		for(int i = 0; i < m_MyBuddy.GetSize(); i++)
		{
			if(m_MyBuddy[i].Compare(m_strUserID) == 0) { SendBuddyUserLeave(i);	break; }
		}
	}

	CBufferEx TempBuf;

	TempBuf.Add(BUDDY_MODE_RESULT);
	TempBuf.Add((BYTE)m_bBuddyMode);

	Send(TempBuf, TempBuf.GetLength());
*/
}

////////////////////////////////////////////////////////////////////////////////
//	³²ÀÌ ÁÖ´Â ¾ÆÀÌÅÛ¸¦ ½Â³«, °ÅÀý¸¦ °áÁ¤ÇÑ´Ù. 
//
void USER::SetItemMode(TCHAR *pBuf)
{
	int index = 0;	

	BYTE tMode = GetByte(pBuf, index);

	if(tMode == 1)			// ¾ÆÀÌÅÛ ¹Þ±â °ÅºÎ
	{
		m_bRefuse = TRUE;
		SendSystemMsg( IDS_USER_ITEM_RECEIVE_DENIED, SYSTEM_NORMAL, TO_ME);// "ÒÑ¾­¹Ø±ÕÎïÆ·½ÓÊÕ."
		return;
	}
	else if(tMode == 0)		// ¾ÆÀÌÅÛ ¹Þ±â ½Â³«
	{
		m_bRefuse = FALSE;
		SendSystemMsg( IDS_USER_ITEM_RECEIVE_PERMITTED, SYSTEM_NORMAL, TO_ME);//"ÒÑ¾­´ò¿ªÎïÆ·½ÓÊÕ."
		return;
	}
	
}

///////////////////////////////////////////////////////////////////////////
//	¼ì²éÊÇ·ñÃæ¶ÔÃæ
BOOL USER::IsThereUser(USER *pUser)
{
	if( !pUser ) return FALSE;

	int nLen = 0;
	nLen = strlen(pUser->m_strUserID);
	if(nLen <= 0 || nLen > CHAR_NAME_LENGTH) return FALSE;

	if( abs( m_tDir - pUser->m_tDir ) != 4 ) return FALSE;

	if( abs( m_curx - pUser->m_curx ) > 2 || abs( m_cury - pUser->m_cury ) > 2 ) return FALSE;

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////
//	×é¶Ó
void USER::BuddyUserChange(TCHAR *pBuf, BYTE tType,int iLen) //ÐÞ¸´×é¶Ó±¨´í Ð¡ºÚ
{
	int index = 0;
	int iCount = 0;
	int i, diffLevel = 0;
	char szUserName[255];

	USER* pUser = NULL;

//	if(m_bBuddyMode == FALSE) return;

	int nLength = GetVarString(iLen, szUserName, pBuf, index);//ÐÞ¸´ÃÜÓï±¨´í Ð¡ºÚ
	//int nLength = GetVarString(sizeof(szUserName), szUserName, pBuf, index);
	if(nLength <= 0 || nLength > CHAR_NAME_LENGTH)
	{
		return;
	}

	if(m_curz == 91 || m_curz == 317 || m_curz == 311 || m_curz == 316/* || CheckInvalidMapType() == 12*/)//PK´óÈü µ¥ÈË¸ñ¶·ÈüµØÍ¼²»ÔÊÐí×é¶Ó MarkJay
	{
		SendSystemMsg("¸ÃµØÍ¼²»ÔÊÐí×é¶Ó!", SYSTEM_ERROR, TO_ME);
		return;
	}

	pUser = GetUser(szUserName);
	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED)//ÐÞ¸´Îª¿ÕÃû×Ö MarkJay
	{
		CString strTemp = _T("");
		strTemp.Format( IDS_USER_NOT_CONNECTED_NOW, szUserName);
		SendSystemMsg((LPTSTR)(LPCTSTR)strTemp, SYSTEM_NORMAL, TO_ME);//"%s Î´Á¬½Ó."
		/*SYSTEMTIME st;
		CString strDate,str3;
		GetLocalTime(&st);
		strDate.Format("%d-%d-%d %d:%d",st.wYear,st.wMonth,st.wDay ,st.wHour,st.wMinute);
		str3.Format("[%s] 1[%s]  2[%s]Ãû×ÖÒì³£\r\n",strDate,szUserName,pUser->m_strUserID);
		EnterCriticalSection( &m_CS_FileWrite );
		g_fpMarkDeBug.Write( str3, str3.GetLength() );
		LeaveCriticalSection( &m_CS_FileWrite);*/
		SoftClose();
		return;
	}

	
	if(tType == BUDDY_OFF)
	{
//		if(!pUser->m_bBuddyMode || !pUser->m_bNowBuddy) return;
		if(!pUser->m_bNowBuddy) return;
		if(!m_bMakeBuddy && strcmp(m_strUserID, pUser->m_strUserID) != 0)
		{
			SendSystemMsg( IDS_USER_HAVE_NOT_PERMISSION, SYSTEM_NORMAL, TO_ME);//"ÄãÃ»È¨ÏÞ."
			return;
		}

		for(i = 0; i < MAX_BUDDY_USER_NUM; i++)
		{
			if(m_MyBuddy[i].uid == pUser->m_uid + USER_BAND) { SendBuddyUserLeave(i);	break; }
		}
		return;
	}
	
	if(tType == BUDDY_INVITE)
	{
		if(/*IsThereUser(pUser) == FALSE || */strcmp(m_strUserID, pUser->m_strUserID) == 0)
		{
			SendSystemMsg( IDS_USER_INVALID_BUDDY_CONDITION, SYSTEM_NORMAL, TO_ME);//"²»·ûºÏÍ¬ÃËÌõ¼þ¡£"
			return;
		}
		
		diffLevel = abs(m_sLevel - pUser->m_sLevel);
		if(diffLevel > 100)					// ·¹º§Â÷ÀÌ°¡ 20À» ³Ñ¾î°¡¸é ¾ÈµÊ×é¶ÓµÈ¼¶
		{
			CString strMsg = _T("");
			strMsg.Format( IDS_USER_BUDDY_LEVEL, pUser->m_strUserID);
			SendSystemMsg(strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_NORMAL, TO_ME);//"ÎÞ·¨×é¶Ó. %s µÄ¼¶±ðµÍÓÚÄãµÄ¼¶±ð10¼¶."
			return;
		}

		if(pUser->m_bNowBuddy == TRUE) 
		{
			CString strTemp = _T("");
			strTemp.Format( IDS_USER_BUDDY_ALREADY, pUser->m_strUserID);
			SendSystemMsg((LPTSTR)(LPCTSTR)strTemp, SYSTEM_NORMAL, TO_ME);//"%s ÒÑ¾­×é¶Ó,²»ÄÜ¼ÓÈëÄãµÄ¶ÓÎé."
			return;
		}
									
		if(!m_bMakeBuddy && !m_bNowBuddy && m_strUserID != pUser->m_strUserID) 
		{
			CBufferEx TempBuf;

			for(i = 0; i < MAX_BUDDY_USER_NUM; i++)		
			{
				m_MyBuddy[i].uid = -1;
				::ZeroMemory(m_MyBuddy[i].m_strUserID, sizeof(m_MyBuddy[i].m_strUserID));		
			}

			m_MyBuddy[0].uid = m_uid + USER_BAND;
			strcpy(m_MyBuddy[0].m_strUserID, m_strUserID);
			m_bMakeBuddy = TRUE;
			m_bNowBuddy = TRUE;

			TempBuf.Add(BUDDY_USER_CHANGE);
			TempBuf.Add((BYTE)0x01);
			TempBuf.Add((int)(m_uid + USER_BAND));
			TempBuf.AddString(m_strUserID);
			TempBuf.Add(m_iSkin);
			TempBuf.Add(m_iHair);
			TempBuf.Add((BYTE)m_sGender);
			TempBuf.AddData(m_strFace, 10);
			TempBuf.Add(m_sMagicMaxHP);
			TempBuf.Add(m_sHP);

			Send(TempBuf, TempBuf.GetLength());
		}

		if(m_bMakeBuddy == FALSE)	return;				// Â¯ÀÌ ¾Æ´Ï¸é ³ª°¡¶ó~~

		for(i = 0; i < MAX_BUDDY_USER_NUM; i++)
		{
			if(m_MyBuddy[i].uid == -1)
			{
				m_MyBuddy[i].uid = pUser->m_uid + USER_BAND;
				strcpy(m_MyBuddy[i].m_strUserID, szUserName);
				SendBuddyUserChange(BUDDY_JOIN, i);
				return;
			}
		}

		SendSystemMsg( IDS_USER_BUDDY_MAX_USER, SYSTEM_NORMAL, TO_ME);//"Ã¿¸ö¶ÓÎéµÄ×î´óÈËÊýÎª4ÈË."
	}
}

///////////////////////////////////////////////////////////////////////////
//	¿Í»§¶ËÓÒÉÏ½ÇµÄÍ¼  ×é¶ÓÍ·Ïñ
void USER::SendBuddyUserChange(BYTE type, int iCount)
{	
	CBufferEx TempBuf;

	USER *pInviteUser = NULL;
	USER *pUser = NULL;
	int i, j;
	
	if(type == BUDDY_JOIN)									// Â¯ÀÌ ÃßÃµÇÏ¿© °¡ÀÔ	
	{
		pInviteUser = GetUser(m_MyBuddy[iCount].uid - USER_BAND);//.GetBuffer(20));

		if(pInviteUser == NULL) return;
		if(pInviteUser->m_state != STATE_GAMESTARTED) return;

		TempBuf.Add(BUDDY_USER_CHANGE);
		TempBuf.Add((BYTE)0x01);							// °¡ÀÔ
		TempBuf.Add((int)(pInviteUser->m_uid + USER_BAND));
		TempBuf.AddString(pInviteUser->m_strUserID);
		TempBuf.Add(pInviteUser->m_iSkin);					// ¾ó±¼
		TempBuf.Add(pInviteUser->m_iHair);
		TempBuf.Add((BYTE)pInviteUser->m_sGender);
		TempBuf.AddData(pInviteUser->m_strFace, 10);
		TempBuf.Add(pInviteUser->m_sMagicMaxHP);					// »óÅÂ 
		TempBuf.Add(pInviteUser->m_sHP);

		for(i = 0; i < MAX_BUDDY_USER_NUM; i++)				// ÃÊÃ»µÈ À¯ÀúÀÇ Á¤º¸¸¦ ³ª¸ÓÁö À¯Àú¿¡°Ô
		{
			if(m_MyBuddy[i].uid == -1) continue;

			pUser = GetUser(m_MyBuddy[i].uid - USER_BAND);	// ¸®½ºÆ®»óÀÇ À¯Àú¸¦ Ã£´Â´Ù.	
			if(pUser == NULL) continue;
			if(pUser->m_state != STATE_GAMESTARTED) continue;
															// Â¯Àº ´õ Ãß°¡ÇÒ ÇÊ¿ä°¡ ¾ø´Ù.
			if(i != 0 && i != iCount)					
			{
				for(j = 0; j < MAX_BUDDY_USER_NUM; j++)
				{
					if(pUser->m_MyBuddy[j].uid == -1)
					{
						pUser->m_MyBuddy[j].uid = pInviteUser->m_uid + USER_BAND;
						strcpy(pUser->m_MyBuddy[j].m_strUserID, pInviteUser->m_strUserID);
						break;
					}
				}
			}
			if(i != iCount) pUser->Send(TempBuf, TempBuf.GetLength());
		}

		pInviteUser->m_bNowBuddy = TRUE;					// °¡ÀÔ Çß´Ù´Â Ç¥½Ã¸¦ ¼ÂÆÃ

		for(i = 0; i < MAX_BUDDY_USER_NUM; i++)				// ³ª¿¡°Ô ±âÁ¸ ¹öµð À¯Àú Á¤º¸ ¸ðµÎ¸¦
		{
			CBufferEx TempBuf2;
			pUser = GetUser(m_MyBuddy[i].uid - USER_BAND);
			if(pUser == NULL) continue;
			if(pUser->m_state != STATE_GAMESTARTED) continue;

			pInviteUser->m_MyBuddy[i].uid = m_MyBuddy[i].uid;
			strcpy(pInviteUser->m_MyBuddy[i].m_strUserID, m_MyBuddy[i].m_strUserID);

			TempBuf2.Add(BUDDY_USER_CHANGE);
			TempBuf2.Add((BYTE)0x01);
			TempBuf2.Add((int)(pUser->m_uid + USER_BAND));
			TempBuf2.AddString(pUser->m_strUserID);
			TempBuf2.Add(pUser->m_iSkin);
			TempBuf2.Add(pUser->m_iHair);
			TempBuf2.Add((BYTE)pUser->m_sGender);
			TempBuf2.AddData(pUser->m_strFace, 10);
			TempBuf2.Add((short)pUser->m_sMagicMaxHP);
			TempBuf2.Add((short)pUser->m_sHP);

			pInviteUser->Send(TempBuf2, TempBuf2.GetLength());
		}
	}
	else if(type == BUDDY_CHANGE)							// »óÅÂ ¼öÄ¡°¡ º¯µ¿ÀÌ ÀÖÀ»¶§
	{
		TempBuf.Add(BUDDY_USER_CHANGE);
		TempBuf.Add((BYTE)0x03);
		TempBuf.Add((int)(m_uid + USER_BAND));
		TempBuf.Add((short)m_sMagicMaxHP);
		TempBuf.Add((short)m_sHP);

		for(i = 0; i < MAX_BUDDY_USER_NUM; i++)
		{
			pUser = GetUser(m_MyBuddy[i].uid - USER_BAND);
			if(pUser == NULL) continue;
			if(pUser->m_state != STATE_GAMESTARTED) continue;
			pUser->Send(TempBuf, TempBuf.GetLength());
		}
	}
}

///////////////////////////////////////////////////////////////////////////
//	×é¶Ó±ä¸ü¶Ó³¤ MarkJay
void USER::SendBuddyUserLeave(int iCount)
{
	int i, j;
	int iNum = 0;
	int olNum = 0;
	USER *pUser = NULL;
	USER *pUser1 = NULL;
	USER *pLeaveUser = NULL;

	CBufferEx TempBuf;										// ±¸¼º¿øÁß¿¡ ³ª°¥¶§

	pLeaveUser = GetUser(m_MyBuddy[iCount].uid - USER_BAND);
	if(pLeaveUser == NULL) goto go_result;
	if(m_bMakeBuddy && strcmp(m_strUserID, pLeaveUser->m_strUserID) == 0)//¶Ó³¤Àë¿ª
	{
		for(i = 0; i < MAX_BUDDY_USER_NUM; i++)				// ¸®½ºÆ®¿¡ µî·ÏµÈ À¯Àú´Â ³ª°£´Ù. 
		{
			CBufferEx TempBuf;
			if(m_MyBuddy[i].uid == -1) continue;
			pUser = GetUser(m_MyBuddy[i].uid - USER_BAND);

			if(pUser == NULL) continue;
			if(pUser->m_state != STATE_GAMESTARTED) continue;

			TempBuf.Add(BUDDY_USER_CHANGE);
			TempBuf.Add((BYTE)0x02);
			TempBuf.Add((int)(pUser->m_uid + USER_BAND));
			pUser->Send(TempBuf, TempBuf.GetLength());

			if(m_uid != pUser->m_uid)						// ÀÚ±â ÀÚ½Å¿¡°Ô´Â º¸³¾ ÇÊ¿ä°¡ ¾ø´Ù.
			{
				pUser->m_bNowBuddy = FALSE;
				for(j = 0; j < MAX_BUDDY_USER_NUM; j++)
				{
					pUser->m_MyBuddy[j].uid = -1;			// Ä£ÀýÈ÷ »ó´ë¹æÀ» ÃÊ±âÈ­ ÇÑ´Ù.
					::ZeroMemory(pUser->m_MyBuddy[j].m_strUserID, sizeof(pUser->m_MyBuddy[j].m_strUserID));
				}
			}
		}
	}
	else if(m_bMakeBuddy)
	{		
		TempBuf.Add(BUDDY_USER_CHANGE);
		TempBuf.Add((BYTE)0x02);
		TempBuf.Add((int)(pLeaveUser->m_uid + USER_BAND));

		for(i = 0; i < MAX_BUDDY_USER_NUM; i++)		
		{
			if(m_MyBuddy[i].uid == -1) continue;
			pUser = GetUser(m_MyBuddy[i].uid - USER_BAND);
			if(pUser == NULL) continue;
			if(pUser->m_state != STATE_GAMESTARTED) continue;
			pUser->Send(TempBuf, TempBuf.GetLength());

			for(j = 0; j < MAX_BUDDY_USER_NUM; j++)
			{
				if(pUser->m_MyBuddy[j].uid == pLeaveUser->m_uid + USER_BAND) 
				{
					pUser->m_MyBuddy[j].uid = -1;	
					::ZeroMemory(pUser->m_MyBuddy[j].m_strUserID, sizeof(pUser->m_MyBuddy[j].m_strUserID));
				}
			}
		}

		iNum = 0;
		pLeaveUser->m_bNowBuddy = FALSE;
		pLeaveUser->m_bMakeBuddy = FALSE;
		for(j = 0; j < MAX_BUDDY_USER_NUM; j++)
		{
			if(m_MyBuddy[j].uid != -1) iNum++;
			pLeaveUser->m_MyBuddy[j].uid = -1;		// Â¯Àº ³ªÁß¿¡ »èÁ¦...	
			::ZeroMemory(pLeaveUser->m_MyBuddy[j].m_strUserID, sizeof(pLeaveUser->m_MyBuddy[j].m_strUserID));
		}

		if(iNum <= 1)						// Â¯ È¥ÀÚÀÏ¶§´Â ÇØ»ê 
		{
			CBufferEx TempBuf2;
			TempBuf2.Add(BUDDY_USER_CHANGE);
			TempBuf2.Add((BYTE)0x02);
			TempBuf2.Add((int)(m_uid + USER_BAND));
			Send(TempBuf2, TempBuf2.GetLength());
			m_bNowBuddy = FALSE;
			m_bMakeBuddy = FALSE;
			for(j = 0; j < MAX_BUDDY_USER_NUM; j++)
			{
				m_MyBuddy[j].uid = -1;		// Â¯Àº ³ªÁß¿¡ »èÁ¦...	
				::ZeroMemory(m_MyBuddy[j].m_strUserID, sizeof(m_MyBuddy[j].m_strUserID));
			}
		}
		return;	
	}
	else
	{
		TempBuf.Add(BUDDY_USER_CHANGE);
		TempBuf.Add((BYTE)0x02);
		TempBuf.Add((int)(pLeaveUser->m_uid + USER_BAND));

		for(i = 0; i < MAX_BUDDY_USER_NUM; i++)				
		{
			if(m_MyBuddy[i].uid == -1) continue;
			pUser = GetUser(m_MyBuddy[i].uid - USER_BAND);
			if(pUser == NULL) continue;
			if(pUser->m_state != STATE_GAMESTARTED) continue;

			pUser->Send(TempBuf, TempBuf.GetLength());

			if(pUser->m_uid != pLeaveUser->m_uid)			// ³ª°¡´Âµ¥ ÀÚ±â¿¡°Ô º¸³¾ ÇÊ¿ä ¾ø´Ù.
			{				
				for(j = 0; j < MAX_BUDDY_USER_NUM; j++)
				{
					if(pUser->m_MyBuddy[j].uid == pLeaveUser->m_uid + USER_BAND) 
					{
						pUser->m_MyBuddy[j].uid = -1;	
						::ZeroMemory(pUser->m_MyBuddy[j].m_strUserID, sizeof(pUser->m_MyBuddy[j].m_strUserID));
						break;
					}
				}
			}
		}

		pUser = GetUser(m_MyBuddy[0].uid - USER_BAND);
		if(pUser != NULL)
		{
			iNum = 0;
			for(j = 0; j < MAX_BUDDY_USER_NUM; j++)
			{
				if(pUser->m_MyBuddy[j].uid != -1) iNum++;
			}

			if(iNum <= 1)						// Â¯ È¥ÀÚÀÏ¶§´Â ÇØ»ê 
			{
				CBufferEx TempBuf2;
				TempBuf2.Add(BUDDY_USER_CHANGE);
				TempBuf2.Add((BYTE)0x02);
				TempBuf2.Add((int)(pUser->m_uid + USER_BAND));
				pUser->Send(TempBuf2, TempBuf2.GetLength());
				pUser->m_bNowBuddy = FALSE;
				pUser->m_bMakeBuddy = FALSE;
				for(j = 0; j < MAX_BUDDY_USER_NUM; j++)
				{
					m_MyBuddy[j].uid = -1;		// Â¯Àº ³ªÁß¿¡ »èÁ¦...	
					::ZeroMemory(m_MyBuddy[j].m_strUserID, sizeof(m_MyBuddy[j].m_strUserID));
				}
			}
		}
	}

go_result:
	m_bMakeBuddy = FALSE;	
	m_bNowBuddy = FALSE;	
	for(j = 0; j < MAX_BUDDY_USER_NUM; j++)
	{
		m_MyBuddy[j].uid = -1;		// Â¯Àº ³ªÁß¿¡ »èÁ¦...	
		::ZeroMemory(m_MyBuddy[j].m_strUserID, sizeof(m_MyBuddy[j].m_strUserID));
	}
}

///////////////////////////////////////////////////////////////////////////
//	BBS Ã³¸®·çÆ¾¿¡¼­ ¿¡·¯°¡ ³¯ °æ¿ì ¿¡·¯Àü¼Û
//
void USER::SendBBSError(BYTE tError)
{
	CBufferEx TempBuf;

	TempBuf.Add(BBS_ERROR);
	TempBuf.Add(tError);

	Send(TempBuf, TempBuf.GetLength());
}

///////////////////////////////////////////////////////////////////////////
//	ÇØ´ç BBS¸¦ Open ÇÑ´Ù.
//
void USER::BBSOpen(int bbsnum)
{
	CBufferEx TempBuf;
	int index = 0;
	int rowcount = 0;
	int i = 0;

	BOOL bQuerySuccess = FALSE;

	SQLHSTMT	hstmt = NULL;
	SQLRETURN	retcode;
	TCHAR		szSQL[1024];	::ZeroMemory(szSQL, sizeof(szSQL));

	_sntprintf(szSQL, sizeof(szSQL), TEXT( "{call BBS_READ_INDEX ( %d, -1 )}" ), bbsnum);

	SQLCHAR		strWriter[CHAR_NAME_LENGTH+1];		::ZeroMemory(strWriter, sizeof(strWriter));
	SQLCHAR		strTitle[50];		::ZeroMemory(strTitle, sizeof(strTitle));
	SQLCHAR		strTime[30];		::ZeroMemory(strTime, sizeof(strTime));

	SQLINTEGER	iIndex, iView;
	SQLINTEGER	iInd;

	int db_index = 0;
	CDatabase* pDB = g_DBNew[m_iModSid].GetDB( db_index );
	if( !pDB ) return;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
//		TRACE("Fail To BBS Open(BBS:%d) !!\n", bbsnum);
//		g_DBNew[m_iModSid].ReleaseDB(db_index);
		return;
	}

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, sizeof(szSQL));
	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		while(1)
		{
			retcode = SQLFetch( hstmt );

			if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
			{
				bQuerySuccess = TRUE;

				i = 1;
				SQLGetData( hstmt, i++, SQL_C_ULONG,	&iIndex,	sizeof(SQLINTEGER), &iInd );
				SQLGetData( hstmt, i++, SQL_C_BINARY,	strWriter,	sizeof(strWriter),	&iInd );
				SQLGetData( hstmt, i++, SQL_C_CHAR,		strTime,	sizeof(strTime),	&iInd );
				SQLGetData( hstmt, i++, SQL_C_ULONG,	&iView,		sizeof(SQLINTEGER),	&iInd );
				SQLGetData( hstmt, i++, SQL_C_BINARY,	strTitle,	sizeof(strTitle),	&iInd );

				TempBuf.Add((int)(iIndex));
				TempBuf.AddString((LPTSTR)strWriter);
				TempBuf.AddString((LPTSTR)strTime);
				TempBuf.Add((int)iView);
				TempBuf.AddString((LPTSTR)strTitle);
				
				::ZeroMemory(strWriter, sizeof(strWriter));
				::ZeroMemory(strTitle, sizeof(strTitle));
				::ZeroMemory(strTime, sizeof(strTime));

				rowcount++;
			}
			else if(retcode == SQL_NO_DATA)
			{
				bQuerySuccess = TRUE;
				break;
			}
			else break;
		}
	}
	else if (retcode == SQL_ERROR)
	{
		bQuerySuccess = FALSE;
		DisplayErrorMsg(hstmt);
		SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
//		BREAKPOINT();

		g_DBNew[m_iModSid].ReleaseDB(db_index);
		return;
	}
	
	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DBNew[m_iModSid].ReleaseDB(db_index);

	if( !bQuerySuccess) return;

	index = 0;
	SetByte(m_TempBuf, BBS_OPEN, index);
	SetShort(m_TempBuf, bbsnum, index );
	SetShort(m_TempBuf, rowcount, index );
	SetString(m_TempBuf, TempBuf, TempBuf.GetLength(), index);
	Send(m_TempBuf, index);
}

//////////////////////////////////////////////////////////////////////////////
//	ÇØ´ç BBS¿¡¼­ ´ÙÀ½ °Ô½Ã¹° Á¤º¸¸¦ ¾ò´Â´Ù.
//
void USER::BBSNext(TCHAR *pBuf)
{
	if ( pBuf == NULL ) return;

	CBufferEx TempBuf;
	int index = 0;
	int i = 1;

	int	nBBS = GetShort(pBuf, index);
	int	nStart = GetInt(pBuf, index);
	
	int rowcount = 0;
	BOOL bQuerySuccess = FALSE;

	SQLHSTMT	hstmt = NULL;
	SQLRETURN	retcode;
	TCHAR		szSQL[1024];	::ZeroMemory(szSQL, sizeof(szSQL));

	_sntprintf(szSQL, sizeof(szSQL), TEXT( "{call BBS_READ_INDEX ( %d, %d )}" ), nBBS, nStart);

	SQLCHAR		strWriter[CHAR_NAME_LENGTH+1];		::ZeroMemory(strWriter, sizeof(strWriter));
	SQLCHAR		strTitle[50];		::ZeroMemory(strTitle, sizeof(strTitle));
	SQLCHAR		strTime[30];		::ZeroMemory(strTime, sizeof(strTime));

	SQLINTEGER	iIndex, iView;
	SQLINTEGER	iInd;

	int db_index = 0;
	CDatabase* pDB = g_DBNew[m_iModSid].GetDB( db_index );
	if( !pDB ) return;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
//		TRACE("Fail To BBS Next(BBS = %d, Start = %d) !!\n", nBBS, nStart);

//		g_DBNew[m_iModSid].ReleaseDB(db_index);
		return;
	}

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, sizeof(szSQL));
	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		while(1)
		{
			retcode = SQLFetch( hstmt );

			if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
			{
				bQuerySuccess = TRUE;

				i = 1;
				SQLGetData( hstmt, i++, SQL_C_ULONG,	&iIndex,	sizeof(SQLINTEGER), &iInd );
				SQLGetData( hstmt, i++, SQL_C_BINARY,	strWriter,	sizeof(strWriter),	&iInd );
				SQLGetData( hstmt, i++, SQL_C_CHAR,		strTime,	sizeof(strTime),	&iInd );
				SQLGetData( hstmt, i++, SQL_C_ULONG,	&iView,		sizeof(SQLINTEGER),	&iInd );
				SQLGetData( hstmt, i++, SQL_C_BINARY,	strTitle,	sizeof(strTitle),	&iInd );

				TempBuf.Add((int)(iIndex));
				TempBuf.AddString((LPTSTR)strWriter);
				TempBuf.AddString((LPTSTR)strTime);
				TempBuf.Add((int)iView);
				TempBuf.AddString((LPTSTR)strTitle);
				
				::ZeroMemory(strWriter, sizeof(strWriter));
				::ZeroMemory(strTitle, sizeof(strTitle));
				::ZeroMemory(strTime, sizeof(strTime));

				rowcount++;
			}
			else if(retcode == SQL_NO_DATA)
			{
				bQuerySuccess = TRUE;
				break;
			}
			else
				break;
		}
	}
	else if (retcode == SQL_ERROR)
	{
		bQuerySuccess = FALSE;
		DisplayErrorMsg(hstmt);
		SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
//		BREAKPOINT();

		g_DBNew[m_iModSid].ReleaseDB(db_index);
		return;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DBNew[m_iModSid].ReleaseDB(db_index);	

	if( !bQuerySuccess) return;

	index = 0;
	SetByte(m_TempBuf, BBS_NEXT_RESULT, index);
	SetShort(m_TempBuf, nBBS, index );
	SetShort(m_TempBuf, rowcount, index );
	SetString(m_TempBuf, TempBuf, TempBuf.GetLength(), index);
	Send(m_TempBuf, index);
}

//////////////////////////////////////////////////////////////////////////////
//	°Ô½Ã¹°ÀÇ ³»¿ëÀ» ÀÐ´Â´Ù.
//
void USER::BBSRead(TCHAR *pBuf)
{
	if ( pBuf == NULL ) return;

	CBufferEx TempBuf;
	int index = 0;
	int i = 1;

	int bbsnum = GetShort( pBuf, index );
	int readindex = GetInt( pBuf, index );

	BOOL bQuerySuccess = FALSE;

	SQLHSTMT	hstmt = NULL;
	SQLRETURN	retcode;
	TCHAR		szSQL[1024];		memset(szSQL, 0x00, sizeof(szSQL));
	
	_sntprintf(szSQL, sizeof(szSQL), TEXT( "{call BBS_READ_CONTENT ( %d, %d )}" ), bbsnum, readindex );

	SQLCHAR		strWriter[CHAR_NAME_LENGTH+1];		memset( strWriter,	NULL, sizeof(strWriter));
	SQLCHAR		strTitle[51];		memset( strTitle,	NULL, sizeof(strTitle));
	SQLCHAR		strTime[31];		memset( strTime,	NULL, sizeof(strTime));
	SQLCHAR		strContent[5001];	memset( strContent, NULL, sizeof(strContent));

	SQLINTEGER	iIndex;
	SQLINTEGER	iInd;

//	SQLSMALLINT	sRet = -1;
//	SQLINTEGER	iRetInd;

	int db_index = 0;
	CDatabase* pDB = g_DBNew[m_iModSid].GetDB( db_index );
	if( !pDB ) return;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		TRACE("Fail To Read BBS (BBS:%d,Index:%d) !!\n", bbsnum, readindex);
		return;
	}

	/*
	retcode = SQLBindParameter(hstmt, 1 ,SQL_PARAM_OUTPUT,SQL_C_SSHORT, SQL_SMALLINT, 0, 0, &sRet, 0, &iRetInd);
	if (retcode!=SQL_SUCCESS) 
	{
		TRACE("USER::BBSRead(), Line = %d, File = %s\n", __LINE__, __FILE__);
		BREAKPOINT();

		g_DB[m_iModSid].ReleaseDB(db_index);
		return;
	}
	*/

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, sizeof(szSQL));
	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		while(1)
		{
			retcode = SQLFetch( hstmt );

			if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
			{
				bQuerySuccess = TRUE;

				i = 1;
				SQLGetData( hstmt, i++, SQL_C_ULONG,	&iIndex,	sizeof(SQLINTEGER), &iInd );
				SQLGetData( hstmt, i++, SQL_C_BINARY,	strWriter,	sizeof(strWriter),	&iInd );
				SQLGetData( hstmt, i++, SQL_C_BINARY,	strTitle,	sizeof(strTitle),	&iInd );
				SQLGetData( hstmt, i++, SQL_C_CHAR,		strTime,	sizeof(strTime),	&iInd );
				SQLGetData( hstmt, i++, SQL_C_BINARY,	strContent,	sizeof(strContent),	&iInd );

				TempBuf.Add((int)(iIndex));
				TempBuf.AddString((LPTSTR)strWriter);
				TempBuf.AddString((LPTSTR)strTitle);
				TempBuf.AddString((LPTSTR)strTime);
				TempBuf.AddLongString((LPTSTR)strContent, _tcslen((LPCTSTR)strContent));

				memset( strWriter,	NULL, sizeof(strWriter));
				memset( strTitle,	NULL, sizeof(strTitle));
				memset( strTime,	NULL, sizeof(strTime));
				memset( strContent, NULL, sizeof(strContent));

				break;
			}
			else if(retcode == SQL_NO_DATA)
			{
				bQuerySuccess = TRUE;
				break;
			}
			else break;
		}
	}
	else
	{				
		DisplayErrorMsg(hstmt);
		retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		BREAKPOINT();

		g_DBNew[m_iModSid].ReleaseDB(db_index);
		return;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DBNew[m_iModSid].ReleaseDB(db_index);
	
	if( !bQuerySuccess )
	{
		if(g_bDebug) SendSystemMsg( IDS_USER_BBS_DELETED_ALREADY, SYSTEM_NORMAL, TO_ME);//"ÒÑ±»É¾³ý."
		SendBBSError(BBS_CONTENTS_DELETE);
		return;
	}
	
	if( !bQuerySuccess ) return;

	index = 0;
	SetByte(m_TempBuf, BBS_READ_RESULT, index);
	SetShort(m_TempBuf, bbsnum, index );
	SetString(m_TempBuf, TempBuf, TempBuf.GetLength(), index);
	Send(m_TempBuf, index);
}
//////////////////////////////////////////////////////////////////////////////
//	»õ·Î¿î °Ô½Ã¹°À» ÀÛ¼ºÇÑ´Ù.
//
void USER::BBSWrite(TCHAR *pBuf)
{
	if ( pBuf == NULL ) return;

	int index = 0;
	int i = 1;
//	CString strText = _T("");
	char strTitle[51];		::ZeroMemory(strTitle,		sizeof(strTitle));
	char strContent[5001];	::ZeroMemory(strContent,	sizeof(strContent));

	int bbsnum = GetShort(pBuf, index);

	if(m_tIsOP != 1 && ( bbsnum == 1 || bbsnum == 3 ) )		// °øÁö°Ô½ÃÆÇ or ·Î¿­·³ºí °Ô½ÃÆÇ
	{
		SendSystemMsg(IDS_USER_NEED_OPERATOR, SYSTEM_NORMAL, TO_ME);// "Ö»ÄÜÓÉ¹ÜÀíÔ±Ê¹ÓÃ."
		return;
	}

	if(!GetVarString(sizeof(strTitle), strTitle, pBuf, index)) return;			// Á¦¸ñÀº 50¹ÙÀÌÆ® ÀÌ³»·Î...	
	if(!GetVarLongString(sizeof(strContent), strContent, pBuf, index)) return;	// ³»¿ëÀº 5000¹ÙÀÌÆ® ÀÌ³»·Î...
	
	SDWORD sTitleLen	= _tcslen(strTitle);
	SDWORD sContentLen	= _tcslen(strContent);
	SDWORD sIDLen		= _tcslen(m_strUserID);

	SQLHSTMT	hstmt = NULL;
	SQLRETURN	retcode;
	TCHAR		szSQL[8000];	::ZeroMemory(szSQL, sizeof(szSQL));

//	_sntprintf(szSQL, sizeof(szSQL), TEXT( "{call BBS_WRITE ( %d, %s, ?, ? )}" ), bbsnum, m_strUserID);
	_sntprintf(szSQL, sizeof(szSQL), TEXT( "{call BBS_WRITE ( %d, ?, ?, ? )}" ), bbsnum );

	int db_index = 0;
	CDatabase* pDB = g_DBNew[m_iModSid].GetDB( db_index );
	if( !pDB ) return;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
//		TRACE("Fail To Write BBS (BBS:%d,Writer:%s,Title:%d) !!\n", bbsnum, m_strUserID, strTitle);

//		g_DBNew[m_iModSid].ReleaseDB(db_index);
		return;
	}

	i = 1;
	SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, 20,		0, (TCHAR*)m_strUserID,	0, &sIDLen );
	SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, 50,		0, (TCHAR*)strTitle,	0, &sTitleLen );
	SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, 5000,	0, (TCHAR*)strContent,	0, &sContentLen );

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, sizeof(szSQL));
	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
	}
	else if (retcode == SQL_ERROR)
	{
		DisplayErrorMsg(hstmt);
		SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		BREAKPOINT();

		g_DBNew[m_iModSid].ReleaseDB(db_index);
		return;
	}
	
	SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DBNew[m_iModSid].ReleaseDB(db_index);
	
	BBSOpen( bbsnum );
}
//////////////////////////////////////////////////////////////////////////////
//	°Ô½Ã¹°À» ¼öÁ¤ÇÑ´Ù.
//
void USER::BBSEdit(TCHAR *pBuf)
{
	if ( pBuf == NULL ) return;

	int index = 0;
	int i = 1;

	char strTitle[51];		::ZeroMemory(strTitle,		sizeof(strTitle));
	char strContent[5001];	::ZeroMemory(strContent,	sizeof(strContent));

	int bbsnum = GetShort(pBuf, index);
	int editindex = GetInt( pBuf, index );

	if(m_tIsOP != 1 && ( bbsnum == 1 || bbsnum == 3 ) )		// °øÁö°Ô½ÃÆÇ or ·Î¿­·³ºí °Ô½ÃÆÇ
	{
		SendSystemMsg(IDS_USER_NEED_OPERATOR, SYSTEM_NORMAL, TO_ME);// "Ö»ÄÜÓÉ¹ÜÀíÔ±Ê¹ÓÃ."
		return;
	}

	if(!GetVarString(sizeof(strTitle), strTitle, pBuf, index)) return;
	if(!GetVarLongString(sizeof(strContent), strContent, pBuf, index)) return;

	SDWORD sTitleLen	= _tcslen(strTitle);
	SDWORD sContentLen	= _tcslen(strContent);
	SDWORD sIDLen		= _tcslen(m_strUserID);

	SQLSMALLINT	sRet = -1;
	SQLINTEGER	iRetInd = SQL_NTS;

	SQLHSTMT	hstmt = NULL;
	SQLRETURN	retcode;
	TCHAR		szSQL[8000];	::ZeroMemory(szSQL, sizeof(szSQL));

	_sntprintf(szSQL, sizeof(szSQL), TEXT( "{call BBS_EDIT ( %d, %d, ?, ?, ?, ?)}" ), bbsnum, editindex );

	int db_index = 0;
	CDatabase* pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB ) return;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		TRACE("Fail To Edit BBS (BBS:%d,Writer:%s,Index:%d) !!\n", bbsnum, m_strUserID, editindex);

		g_DB[m_iModSid].ReleaseDB(db_index);
		return;
	}

	i = 1;
	SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, 20,		0, (TCHAR*)m_strUserID,	0, &sIDLen );
	SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, 50,		0, (TCHAR*)strTitle,	0, &sTitleLen );
	SQLBindParameter( hstmt, i++, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, 5000,	0, (TCHAR*)strContent,	0, &sContentLen );
	SQLBindParameter( hstmt, i++ ,SQL_PARAM_OUTPUT,SQL_C_SSHORT, SQL_SMALLINT,	0,		0, &sRet,				0, &iRetInd);

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, sizeof(szSQL));
	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
	}
	else
	{
		DisplayErrorMsg(hstmt);
		retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		BREAKPOINT();

		g_DBNew[m_iModSid].ReleaseDB(db_index);
		return;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DBNew[m_iModSid].ReleaseDB(db_index);

	if(sRet == 2)
	{
		if(g_bDebug) SendSystemMsg( IDS_USER_BBS_NO_PERMISSION, SYSTEM_NORMAL, TO_ME);// "ÄãÃ»ÓÐÊ¹ÓÃµÄÈ¨ÏÞ."
		SendBBSError(BBS_CONTENTS_DELETE);
		return;
	}
	if(sRet == 3)
	{
		if(g_bDebug) SendSystemMsg( IDS_USER_BBS_DELETED_ALREADY, SYSTEM_NORMAL, TO_ME);//"ÒÑ±»É¾³ý."
		SendBBSError(BBS_CONTENTS_DELETE);
		return;
	}

	BBSOpen( bbsnum );
}
/////////////////////////////////////////////////////////
// MAIL request process fucntion
// zinee 02-11-18

void USER::MailInfoReq( TCHAR* pBuf )
{
	if ( pBuf == NULL ) return;

	TCHAR	TempBuf[10000];
	int		nTempBufPos;
	int		nBufferPos;
	BYTE	error;

	nTempBufPos = 0;
	error = MailGetInfo( TempBuf, nTempBufPos );
//	if( error == 0 ) error = MailGetList( TempBuf, -1, 20, nTempBufPos );

	nBufferPos = 0;
	SetByte(m_TempBuf, MAIL_INFO_RESULT, nBufferPos);
//	SetByte(m_TempBuf, error, nBufferPos );
	SetString(m_TempBuf, TempBuf, nTempBufPos, nBufferPos );
	Send(m_TempBuf, nBufferPos );
}


void USER::MailSendReq( TCHAR* pBuf )
{
	if ( pBuf == NULL ) return;

//	CDatabase*	pDB;
	SQLHSTMT	hstmt = NULL;
	TCHAR		szSQL[1024];	
//	SQLRETURN	retcode;

	USER*	pRecvUser;

	short tSlot;

	int nBufferPos;
//	int	nDBIndex;

	BYTE error;

	SYSTEMTIME	st;
	ItemList	StampItem;

	QUEUE		Queue;
	CBufferEx	TempBuf, QTempBuf;

	SQLCHAR		strUserID[CHAR_NAME_LENGTH+1];		
	SQLCHAR		strSendID[CHAR_NAME_LENGTH+1];
	SQLCHAR		strTime[15];
	BOOL		bRead;
	char		strMessage[1001];

	SQLINTEGER		nUserID = CHAR_NAME_LENGTH+1;		
	SQLINTEGER		nSendID = CHAR_NAME_LENGTH+1;
	SQLINTEGER		nTime = 15;
	SQLINTEGER		nRead = sizeof(bRead);
	SQLINTEGER		nMessage = 1001;

//	SQLSMALLINT		SQLType;
//	SQLSMALLINT		SQL_C_Type;
//	SQLUINTEGER		SQLColLength;
//	SQLINTEGER		SQLDataLength;

	::ZeroMemory(strUserID, sizeof(strUserID));
	::ZeroMemory(strSendID, sizeof(strSendID));
	::ZeroMemory(strTime, sizeof(strTime));
	::ZeroMemory(szSQL, sizeof(szSQL));
	::ZeroMemory(strMessage, sizeof(strMessage));
	::ZeroMemory(&Queue, sizeof(QUEUE) );

	nBufferPos	= 1;
	// Get (strUserID, strMessage) from buffer.
	if(!GetVarString(sizeof(strUserID), (char*)strUserID, pBuf, nBufferPos)) return;
	if(!GetVarLongString(sizeof(strMessage), strMessage, pBuf, nBufferPos)) return;

	// ¹Þ´Â »ç¶÷ ¾ÆÀÌµð°¡ Á¸ÀçÇÏÁö ¾ÊÀ»°æ¿ì
	if( IsExistCharId( (char*)strUserID ) == FALSE )
	{
		error = 2;
		nBufferPos = 0;
		SetByte(m_TempBuf, MAIL_SEND_RESULT, nBufferPos);
		SetByte(m_TempBuf, error, nBufferPos );
		Send(m_TempBuf, nBufferPos);
		return;
	}

	::ZeroMemory( &StampItem, sizeof(ItemList) );
	StampItem.sSid = NORMAL_ITEM_STAMP;
	tSlot = (short)GetSameItem(StampItem, INVENTORY_SLOT);

	//À¯Àú ÀÎº¥¿¡ ¾ÆÀÌÅÛÀÌ ¾ø°Å³ª ³»±¸·ÂÀÌ 0ÀÏ °æ¿ì
	if( tSlot == -1 )

	{
		error = 1;
		nBufferPos = 0;
		SetByte(m_TempBuf, MAIL_SEND_RESULT, nBufferPos);
		SetByte(m_TempBuf, error, nBufferPos );
		Send(m_TempBuf, nBufferPos);
		return;
	}
	else if( m_UserItem[tSlot].sCount <= 0 )
	{
		error = 1;
		nBufferPos = 0;
		SetByte(m_TempBuf, MAIL_SEND_RESULT, nBufferPos);
		SetByte(m_TempBuf, error, nBufferPos );
		Send(m_TempBuf, nBufferPos);
		return;
	}

	///////////////////////////////////////////////////////////////////////////////

	// strSendID, strTime, bRead
	strcpy( (char*)strSendID, m_strUserID );
	GetLocalTime(&st);
	_sntprintf((char*)strTime, sizeof(strTime), TEXT( "%02d/%02d/%02d %02d:%02d" ), 
				st.wYear%100, st.wMonth, st.wDay, st.wHour, st.wMinute );
	bRead = FALSE;

	QTempBuf.Add( (int)0 );
	_sntprintf(szSQL, sizeof(szSQL), TEXT( "{call MAIL_INSERT ( \'%s\',\'%s\',\'%s\',%d,\'%s\' )}" ),strUserID, strSendID, strTime, bRead, strMessage);
	QTempBuf.AddData( szSQL, strlen( szSQL )+1 );
/*	SQLType = SQL_C_CHAR;
	QTempBuf.Add( SQLType );
	SQL_C_Type = SQL_VARCHAR;
	QTempBuf.Add( SQL_C_Type );
	SQLColLength = 31;
	QTempBuf.Add( SQLColLength );
	SQLDataLength = strlen((TCHAR*)strUserID)+1;
	QTempBuf.Add( SQLDataLength );
	QTempBuf.AddData( (TCHAR*)strUserID, SQLDataLength );

	SQLType = SQL_C_CHAR;
	QTempBuf.Add( SQLType );
	SQL_C_Type = SQL_VARCHAR;
	QTempBuf.Add( SQL_C_Type );
	SQLColLength = 31;
	QTempBuf.Add( SQLColLength );
	SQLDataLength = strlen((TCHAR*)strSendID)+1;
	QTempBuf.Add( SQLDataLength );
	QTempBuf.AddData( (TCHAR*)strSendID, SQLDataLength );

	SQLType = SQL_C_CHAR;
	QTempBuf.Add( SQLType );
	SQL_C_Type = SQL_CHAR;
	QTempBuf.Add( SQL_C_Type );
	SQLColLength = 15;
	QTempBuf.Add( SQLColLength );
	SQLDataLength = strlen((TCHAR*)strTime)+1;
	QTempBuf.Add( SQLDataLength );
	QTempBuf.AddData( (TCHAR*)strTime, SQLDataLength );

	SQLType = SQL_C_BINARY;
	QTempBuf.Add( SQLType );
	SQL_C_Type = SQL_VARBINARY;
	QTempBuf.Add( SQL_C_Type );
	SQLColLength = 1001;
	QTempBuf.Add( SQLColLength );
	SQLDataLength = strlen((TCHAR*)strMessage)+1;
	QTempBuf.Add( SQLDataLength );
	QTempBuf.AddData( (TCHAR*)strMessage, SQLDataLength );*/

	if( g_MailAgentQueue.PutQueue( (QUEUE*)((LPTSTR)QTempBuf) ) == TRUE )
		error = 0;
	else error = 3;  // ¸ÞÀÏ½Ã½ºÅÛ °úºÎÇÏÀÏ °æ¿ì
	//////////////////////////////////////////////////////////////////////////////


	/*
	nDBIndex	= 0;
	pDB = g_DB[m_iModSid].GetDB( nDBIndex );
	if( !pDB ) return;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		g_DB[m_iModSid].ReleaseDB(nDBIndex);
		return;
	}


	// strSendID, strTime, bRead
	strcpy( (char*)strSendID, m_strUserID );
	GetLocalTime(&st);
	_sntprintf((char*)strTime, sizeof(strTime), TEXT( "%02d/%02d/%02d %02d:%02d" ), 
				st.wYear%100, st.wMonth, st.wDay, st.wHour, st.wMinute );
	bRead = FALSE;

	_sntprintf(szSQL, sizeof(szSQL), TEXT( "{call MAIL_INSERT ( ?,?,?,%d,? )}" ), bRead);

	retcode = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,	CHAR_NAME_LENGTH+1,	0, (TCHAR*)strUserID,	0, &nUserID );
	retcode = SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,	CHAR_NAME_LENGTH+1,	0, (TCHAR*)strSendID,	0, &nSendID );
	retcode = SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR,	15,					0, (TCHAR*)strTime,		0, &nTime );
	retcode = SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, 1001,			0, (TCHAR*)strMessage,	0, &nMessage );

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, sizeof(szSQL));

	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO ) error = 0;
	else error = 2; //Á¸ÀçÇÏÁö ¾Ê´Â ¾ÆÀÌµð

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(nDBIndex);	
	*/

	nBufferPos = 0;
	SetByte(m_TempBuf, MAIL_SEND_RESULT, nBufferPos);
	SetByte(m_TempBuf, error, nBufferPos );
	if( error == 0 )
	{
		// ¿ìÇ¥ÀÇ ¹«°Ô¸¦ ÇÏ³ª °¨¼Ò½ÃÅ²´Ù.
		m_UserItem[tSlot].sCount -= 1;
		if(m_UserItem[tSlot].sCount <= 0) m_UserItem[tSlot].sSid = -1;

		// ÇöÀç ¹«°Ô¿¡¼­ ¿ìÇ¥ ÇÑ°³ ¹«°Ô¸¦ °¨¼Ò½ÃÅ²´Ù.
		m_iCurWeight -= g_arItemTable[NORMAL_ITEM_STAMP]->m_byWeight;
		if( m_iCurWeight < 0 ) m_iCurWeight = 0;

		SetShort( m_TempBuf, tSlot, nBufferPos );
		SetShort( m_TempBuf, m_UserItem[tSlot].sCount, nBufferPos );;
	}

	Send(m_TempBuf, nBufferPos);

	// »ó´ë¹æ À¯Àú°¡ °ÔÀÓÁßÀÌ¸é »õ¸ÞÀÏÀÌ µµÂøÇß´Ù°í ¾Ë·ÁÁÜ
	if( error == 0 )
	{
		pRecvUser = GetUser((TCHAR*)strUserID);
		if( pRecvUser != NULL )
		{
			if(pRecvUser->m_state == STATE_GAMESTARTED 
				&& pRecvUser->m_tIsOP == 0)
			{
				nBufferPos = 0;
				SetByte(m_TempBuf, MAIL_INFO_RESULT, nBufferPos);
				pRecvUser->MailGetInfo( m_TempBuf, nBufferPos );
				pRecvUser->Send(m_TempBuf, nBufferPos );
			}
		}
	}
}

void USER::MailListReq( TCHAR* pBuf )
{
	if ( pBuf == NULL ) return;
	int nReadPos;
	int nStartIndex;//, nListCount;
	nReadPos	= 0;
	nStartIndex = GetInt(pBuf, nReadPos);

	nReadPos = 0;
	SetByte(m_TempBuf, MAIL_LIST_RESULT, nReadPos);
	MailGetList( m_TempBuf, nStartIndex, 8, nReadPos );
	Send(m_TempBuf, nReadPos);
}

int USER::MailGetList( char* pBuf, int nStartIndex, int nCount, int& index )
{
	if ( pBuf == NULL ) return -1;

	CDatabase*	pDB;
	SQLHSTMT	hstmt = NULL;
	TCHAR		szSQL[1024];	
	SQLRETURN	retcode;

	int i;
	int nListCount;
	int	nDBIndex;

	SQLINTEGER	nSQLIndex;

	SQLINTEGER  nUserIDSize;

	BOOL	bQuerySuccess;

	CBufferEx	TempBuf;

	SQLINTEGER	iIndex;
	SQLCHAR		strUserID[CHAR_NAME_LENGTH+1];		
	SQLCHAR		strSendID[CHAR_NAME_LENGTH+1];
	SQLCHAR		strTime[15];
	BOOL		bRead;


	::ZeroMemory(strUserID, sizeof(strUserID));
	::ZeroMemory(strSendID, sizeof(strSendID));
	::ZeroMemory(strTime, sizeof(strTime));
	::ZeroMemory(szSQL, sizeof(szSQL));

	nDBIndex	= 0;
	pDB = g_DB[m_iModSid].GetDB( nDBIndex );
	if( !pDB ) return -1;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		g_DB[m_iModSid].ReleaseDB(nDBIndex);
		return -1;
	}

	if( nStartIndex > 1 ) nStartIndex--;

	_sntprintf(szSQL, sizeof(szSQL), TEXT( "{call MAIL_READ_LIST ( %d, ? )}" ), nStartIndex );

	nUserIDSize = _tcslen(m_strUserID);

	i=1;
	retcode = SQLBindParameter(hstmt, i++, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 30,	0, (TCHAR*)m_strUserID,	0, &nUserIDSize );

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, sizeof(szSQL));

	nListCount	= 0;

	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		while(nListCount < nCount)
		{
			retcode = SQLFetch( hstmt );

			if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
			{
				bQuerySuccess = TRUE;

				i = 1;
				SQLGetData( hstmt, i++, SQL_C_ULONG,	&iIndex,	sizeof(SQLINTEGER), &nSQLIndex );
				SQLGetData( hstmt, i++, SQL_C_CHAR,		strUserID,	sizeof(strUserID),	&nSQLIndex );
				SQLGetData( hstmt, i++, SQL_C_CHAR,		strSendID,	sizeof(strSendID),	&nSQLIndex );
				SQLGetData( hstmt, i++, SQL_C_CHAR,		strTime,	sizeof(strTime),	&nSQLIndex );
				SQLGetData( hstmt, i++, SQL_C_SHORT,	&bRead,		sizeof(SQL_C_TINYINT),	&nSQLIndex );

				TempBuf.Add((SQLINTEGER)(iIndex));
//				TempBuf.AddString((LPTSTR)strUserID);
				TempBuf.AddString((LPTSTR)strSendID);
				TempBuf.AddString((LPTSTR)(strTime));
				TempBuf.Add((BYTE)bRead);//½â¾öÓÊ¼þÎÞ·¨É¾³ýÎÞ·¨²é¿´ÎÊÌâ MarkJay Ô­°æÃ»BYTE
				
				::ZeroMemory(strUserID, sizeof(strUserID));
				::ZeroMemory(strSendID, sizeof(strSendID));
				::ZeroMemory(strTime, sizeof(strTime));

				nListCount++;
			}
			else if(retcode == SQL_NO_DATA)
			{
				bQuerySuccess = TRUE;
				break;
			}
			else 
			{
				bQuerySuccess = FALSE;
				break;
			}
		}
	}
	else if (retcode == SQL_ERROR)
	{
		bQuerySuccess = FALSE;
		DisplayErrorMsg(hstmt);
		SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		g_DB[m_iModSid].ReleaseDB(nDBIndex);
		return -1;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(nDBIndex);	

	if( !bQuerySuccess) return -1;

	SetShort( pBuf, (short)nListCount, index );
	if( nListCount > 0 ) SetString(pBuf, TempBuf, TempBuf.GetLength(), index);

	return nListCount;
}

BYTE USER::MailGetInfo( char* pBuf, int& index )
{
	if ( pBuf == NULL ) return -1;

	CDatabase*	pDB;
	SQLHSTMT	hstmt = NULL;
	TCHAR		szSQL[1024];	
	SQLRETURN	retcode;

	BYTE	error;

	int		i;
	int		nDBIndex;	

	SQLINTEGER	nNewMail = -1;
	SQLINTEGER	nCurMail = -1;

	SQLINTEGER	nUserIDSize;
	SQLINTEGER	nNewMailSize;
	SQLINTEGER	nCurMailSize;

	::ZeroMemory(szSQL, sizeof(szSQL));

	nDBIndex	= 0;
	pDB = g_DB[m_iModSid].GetDB( nDBIndex );
	if( !pDB ) 
	{
		error = 1;
		goto exit;
	}

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		g_DB[m_iModSid].ReleaseDB(nDBIndex);
		error = 1;
		goto exit;
	}

	_sntprintf(szSQL, sizeof(szSQL), TEXT( "{call MAIL_READ_INFO ( ?, ?, ? )}" ) );

	nUserIDSize = _tcslen(m_strUserID);
	nNewMailSize = SQL_NTS;
	nCurMailSize = SQL_NTS;

	i=1;
	retcode = SQLBindParameter(hstmt, i++, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 30,	0, (TCHAR*)m_strUserID,	0, &nUserIDSize );
	retcode = SQLBindParameter(hstmt, i++, SQL_PARAM_OUTPUT, SQL_C_LONG, SQL_INTEGER, 0,	0, &nNewMail,	0, &nNewMailSize );
	retcode = SQLBindParameter(hstmt, i++, SQL_PARAM_OUTPUT, SQL_C_LONG, SQL_INTEGER, 0,	0, &nCurMail,	0, &nCurMailSize );

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, sizeof(szSQL));

	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		error = 0;
	}
	else if (retcode == SQL_ERROR)
	{
		SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		g_DB[m_iModSid].ReleaseDB(nDBIndex);
		error = 1;
		goto exit;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(nDBIndex);	

exit:

	SetInt(pBuf, nNewMail, index);
	SetInt(pBuf, nCurMail, index);

	return error;
}


void USER::MailContentReq( TCHAR* pBuf )
{
	if ( pBuf == NULL ) return;

	CDatabase*	pDB;
	SQLHSTMT	hstmt = NULL;
	TCHAR		szSQL[1024];	
	SQLRETURN	retcode;
	SQLINTEGER	nSQLIndex;

	int i;
	int nReadPos;
	int	nDBIndex;

	BYTE error;

	int nMailReqIndex;

	CBufferEx TempBuf;

	BOOL bQuerySuccess;

	SQLINTEGER	iIndex;
	SQLCHAR		strUserID[CHAR_NAME_LENGTH+1];		
	SQLCHAR		strSendID[CHAR_NAME_LENGTH+1];
	SQLCHAR		strTime[15];
	SQLCHAR		bRead[20];
	SQLCHAR		strMessage[1001];

	::ZeroMemory(strUserID, sizeof(strUserID));
	::ZeroMemory(strSendID, sizeof(strSendID));
	::ZeroMemory(strTime, sizeof(strTime));
	::ZeroMemory(szSQL, sizeof(szSQL));
	::ZeroMemory(strMessage, sizeof(strMessage));

	nReadPos = 0;
	nMailReqIndex = GetInt( pBuf, nReadPos );

	nDBIndex = 0;
	pDB = g_DB[m_iModSid].GetDB( nDBIndex );
	if( !pDB ) return;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		g_DB[m_iModSid].ReleaseDB(nDBIndex);
		return;
	}

	_sntprintf(szSQL, sizeof(szSQL), TEXT( "{call MAIL_READ_MESSAGE (%d,\'%s\')}" ), nMailReqIndex ,m_strUserID);
	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, sizeof(szSQL));
	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		while(1)
		{
			retcode = SQLFetch( hstmt );

			if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
			{
				bQuerySuccess = TRUE;

				i = 1;
				SQLGetData( hstmt, i++, SQL_C_ULONG,	&iIndex,	sizeof(SQLINTEGER), &nSQLIndex );
				SQLGetData( hstmt, i++, SQL_C_CHAR,		strUserID,	sizeof(strUserID),	&nSQLIndex );
				SQLGetData( hstmt, i++, SQL_C_CHAR,		strSendID,	sizeof(strSendID),	&nSQLIndex );
				SQLGetData( hstmt, i++, SQL_C_CHAR,		strTime,	sizeof(strTime),	&nSQLIndex );
				SQLGetData( hstmt, i++, SQL_C_CHAR,		&bRead,		sizeof(SQL_C_CHAR),	&nSQLIndex );
				SQLGetData( hstmt, i++, SQL_C_BINARY,	strMessage,	sizeof(strMessage),	&nSQLIndex );

				TempBuf.Add((int)(iIndex));
				TempBuf.AddLongString((LPTSTR)strMessage, _tcslen((LPCTSTR)strMessage));

				::ZeroMemory(strUserID, sizeof(strUserID));
				::ZeroMemory(strSendID, sizeof(strSendID));
				::ZeroMemory(strTime, sizeof(strTime));
				::ZeroMemory(strMessage, sizeof(strMessage));

				break;
			}
			else if(retcode == SQL_NO_DATA)
			{
				bQuerySuccess = TRUE;
				break;
			}
			else 
			{
				bQuerySuccess = FALSE;
				break;
			}
		}
	}
	else
	{				
		DisplayErrorMsg(hstmt);
		retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		g_DB[m_iModSid].ReleaseDB(nDBIndex);
		return;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(nDBIndex);
	
	if( bQuerySuccess ) error = 0;
	else error = 1;
	
	nReadPos = 0;
	SetByte(m_TempBuf, MAIL_CONTENTS_RESULT, nReadPos);
	SetByte(m_TempBuf, error, nReadPos);
	if( error == 0 ) SetString(m_TempBuf, TempBuf, TempBuf.GetLength(), nReadPos);

	Send(m_TempBuf, nReadPos);
}

void USER::MailDeleteReq( TCHAR* pBuf )
{
	if ( pBuf == NULL ) return;

	CDatabase*	pDB;
	SQLHSTMT	hstmt = NULL;
	TCHAR		szSQL[1024];	
	SQLRETURN	retcode;

	SQLINTEGER	nUserIDSize;
	SQLINTEGER		sReturn;
	SQLINTEGER	iReturnIndex;

	int nReadPos;
	int	nDBIndex;

	char pSuccessBuf[10000];
	int	nBufferCount;
	int nMailReqIndex;
	int	nMailNextIndex;
	int nMailListCount;

	int	i;
	BYTE error;

	CBufferEx TempBuf;

	::ZeroMemory( szSQL, sizeof(szSQL) );

	nReadPos = 0;
	nMailReqIndex = GetInt( pBuf, nReadPos );
	nMailNextIndex = GetInt( pBuf, nReadPos );

	nDBIndex = 0;
	pDB = g_DB[m_iModSid].GetDB( nDBIndex );
	if( !pDB ) return;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS ) return;

	_sntprintf(szSQL, sizeof(szSQL), TEXT( "{call MAIL_DELETE ( %d, ?, ?)}" ), nMailReqIndex);

	nUserIDSize = _tcslen(m_strUserID);
	sReturn			= -1;
	iReturnIndex	= SQL_NTS;

	i=1;
	retcode = SQLBindParameter(hstmt, i++, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 30,	0, (TCHAR*)m_strUserID,	0, &nUserIDSize );
	retcode = SQLBindParameter(hstmt, i++, SQL_PARAM_OUTPUT,SQL_C_LONG, SQL_INTEGER, 0,		0, &sReturn,			0, &iReturnIndex);
	if (retcode!=SQL_SUCCESS) return;

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, sizeof(szSQL));

	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
	}
	else
	{
		DisplayErrorMsg(hstmt);
		retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		g_DB[m_iModSid].ReleaseDB(nDBIndex);
		return;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(nDBIndex);

	error = (BYTE)sReturn;

	nReadPos = 0;
	SetByte(m_TempBuf, MAIL_DELETE_RESULT, nReadPos);
	SetByte(m_TempBuf, error, nReadPos);
	SetInt(m_TempBuf, nMailReqIndex, nReadPos);

	if( error == 0 )
	{
		nBufferCount = 0;
		MailGetInfo( pSuccessBuf, nBufferCount );
		SetString(m_TempBuf, pSuccessBuf, nBufferCount, nReadPos );

		nBufferCount = 0;
		nMailListCount = MailGetList( pSuccessBuf, nMailNextIndex, 1, nBufferCount );
		//SetShort(m_TempBuf, nMailListCount, nReadPos );
		SetString(m_TempBuf, pSuccessBuf, nBufferCount, nReadPos );
	}

	Send(m_TempBuf, nReadPos);
}



//------------------------------------------------------------
// µ¥ÀÌÅÍ º£ÀÌ½º¸¦ ¾÷µ¥ÀÌÆ® ÇÑ´Ù.
//--yskang 0.1
//ÀÌÇÔ¼ö´Â ¾²·¹µå¿¡¼­¸¸ È£ÃâÇÕ´Ï´Ù.
//------------------------------------------------------------
void USER::InsertLoveName(TCHAR *pBuf)
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	BOOL			bQuerySuccess = TRUE;
	BOOL			bSuccess = FALSE;
	TCHAR			szSQL[160];	
	BYTE error_code=0;
	USER *pUser = NULL;
	CDatabase* pDB = NULL;
	CBufferEx TempBuf;
	CBufferEx ResultTempBuf;
	int index = 0;
	int db_index = 0;
	int uid=0;
	uid = GetInt(pBuf, index);
	if(uid < 0 || uid >= INVALID_BAND) { error_code = ERR_2; goto go_result; }
	pUser = GetUser(uid - USER_BAND);								
	if(!pUser) { error_code = ERR_2; goto go_result; }// À¯Àú°¡ ¾ø´Ù.


	::ZeroMemory(szSQL, sizeof(szSQL));
	index = 0;
	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call INSERT_LOVE_NAME (\'%s\', \'%s\',%d)}"), pUser->m_strUserID, pUser->m_strLoveName, pUser->m_dwGuild); 

	
	//pDB = g_DBNew[m_iModSid].GetDB( db_index );
	pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB ) { error_code = ERR_5; goto go_result; }

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if (retcode != SQL_SUCCESS)
		{ error_code = ERR_5; goto go_result; }

	if (retcode == SQL_SUCCESS)
	{
		retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, SQL_NTS);
		if (retcode ==SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{
		}
		else if (retcode==SQL_ERROR)
		{
			DisplayErrorMsg( hstmt );
			bQuerySuccess = FALSE;
		}
	}
	
	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	//g_DBNew[m_iModSid].ReleaseDB(db_index);
	g_DB[m_iModSid].ReleaseDB(db_index);

	if( !bQuerySuccess ) { error_code = ERR_5; goto go_result; }

	//È£ÄªÀ» ¸Þ¸ð¸® µðºñ¿¡ ÀúÀåÇÑ´Ù(?)
	lstrcpy(pUser->m_pMD->m_strLoveName , pUser->m_strLoveName);
	//È£ÄªÀ» ´Ù¸¥ »ç¶÷µé¿¡°Ô ¾Ë¸°´Ù(°°Àº ½ºÅ©¸°¿¡ ÀÖ´Â»ç¶÷µé¿¡ ÇÑÇØ¼­?)
	if(f_Markini[0]->CH_lV_KG == 1)//³ÆºÅÏÔÊ¾ÅÅÐÐ MarkJay
	{
		pUser->ChangeLoveName();
	}
	else
	{
		TempBuf.Add(LOVE_NAME);
		TempBuf.Add(uid);
		TempBuf.AddString(pUser->m_strLoveName);
		pUser->SendExactScreen(TempBuf, TempBuf.GetLength());
	}
	bSuccess = TRUE;
	m_bPseudoString=FALSE;//ÀÔ·Â ¼º°øÇÏ¿´À¸¹Ç·Î °¡»ó È£ÄªÀÌ ¾Æ´Ï´Ù.
go_result:
	ResultTempBuf.Add(LOVE_NAME_RESULT);
	if(bSuccess == FALSE)
	{
		ResultTempBuf.Add(FAIL);				// ½ÇÆÐ
		ResultTempBuf.Add(error_code);
		Send(ResultTempBuf,ResultTempBuf.GetLength());
		return;
	}
	ResultTempBuf.Add(SUCCESS);					// ¼º°ø	
	Send(ResultTempBuf, ResultTempBuf.GetLength());
}



//////////////////////////////////////////////////////////////////////////////
//	°Ô½Ã¹°À» »èÁ¦ÇÑ´Ù.
//
void USER::BBSDelete(TCHAR *pBuf)
{
	if ( pBuf == NULL ) return;

	int index = 0;
	int bbsnum = GetShort( pBuf, index );
	int delindex = GetInt( pBuf, index );

	if(m_tIsOP != 1 && ( bbsnum == 1 || bbsnum == 3 ) )		//°øÁö°Ô½ÃÆÇ or ·Î¿­·³ºí °Ô½ÃÆÇ
	{
		SendSystemMsg( IDS_USER_NEED_OPERATOR, SYSTEM_NORMAL, TO_ME);// "Ö»ÄÜÓÉ¹ÜÀíÔ±Ê¹ÓÃ."
		return;
	}

	SQLHSTMT	hstmt = NULL;
	SQLRETURN	retcode;
	TCHAR		szSQL[1024];	::ZeroMemory(szSQL, sizeof(szSQL));

	SQLSMALLINT	sRet = -1;
	SQLINTEGER	iRetInd = SQL_NTS;

	SDWORD sIDLen		= _tcslen(m_strUserID);

	_sntprintf(szSQL, sizeof(szSQL), TEXT( "{call BBS_DELETE (%d, %d, ?, ?)}" ), bbsnum, delindex );

	int db_index = 0;
	CDatabase* pDB = g_DBNew[m_iModSid].GetDB( db_index );
	if( !pDB ) return;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		TRACE("Fail To Delete BBS (BBS:%d, Index:%d) !!\n", bbsnum, delindex);
		return;
	}

	retcode = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, 20,		0, (TCHAR*)m_strUserID,	0, &sIDLen );
	retcode = SQLBindParameter(hstmt, 2 ,SQL_PARAM_OUTPUT,SQL_C_SSHORT, SQL_SMALLINT, 0, 0, &sRet, 0, &iRetInd);
	if (retcode!=SQL_SUCCESS) 
	{
		TRACE("USER::BBSDelete(), Line = %d, File = %s\n", __LINE__, __FILE__);
		return;
	}

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, sizeof(szSQL));
	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
	}
	else
	{
		DisplayErrorMsg(hstmt);
		retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);

		g_DBNew[m_iModSid].ReleaseDB(db_index);
		return;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DBNew[m_iModSid].ReleaseDB(db_index);

	if( sRet != 1 )
	{
		if(g_bDebug) SendSystemMsg( IDS_USER_BBS_CANT_DELETE, SYSTEM_NORMAL, TO_ME);//"ÎÞ·¨É¾³ý."
		SendBBSError(BBS_CONTENTS_DELETE);
		return;
	}
	BBSOpen( bbsnum );
}
//##################################################################################
//
//	Battle
//
//##################################################################################

//////////////////////////////////////////////////////////////////////////////////
//	User ÀÇ °ø°Ý·ÂÀ» ¾ò¾î¿Â´Ù.
//
int USER::GetAttack()
{
	// ³ªÁß¿¡´Â ÀüÅõ°ø½Ä´ë·Î Ã³¸®ÇÏµµ·Ï ÇÑ´Ù.
	// Áö±ÝÀº ±×³É ·£´ýÀ¸·Î ¾ò¾î¿Â´Ù.

	int nAttack = XdY(2, 80);
	return nAttack;
}

//////////////////////////////////////////////////////////////////////////////////
//	À¯ÀúÀÇ ¹æ¾î·ÂÀ» ¾ò¾î¿Â´Ù.
//
int USER::GetDefense(short* sItemDefense)
{
	int nDefense = 0;

	short sHeadSid = m_UserItem[HEAD].sSid;
	short sBodySid = m_UserItem[BODY].sSid;
	short sPantsSid = m_UserItem[PANTS].sSid;
	short sShoesSid = m_UserItem[SHOES].sSid;

	short sHeadDefense = GetItemDefense(sHeadSid, HEAD);
	short sBodyDefense = GetItemDefense(sBodySid, BODY);
	short sPantsDefense = GetItemDefense(sPantsSid, PANTS);
	short sShoesDefense = GetItemDefense(sShoesSid, SHOES);

	if(m_UserItem[HEAD].sDuration <= 0) sHeadDefense = 0;
	if(m_UserItem[BODY].sDuration <= 0) sBodyDefense = 0;
	if(m_UserItem[PANTS].sDuration <= 0) sPantsDefense = 0;
	if(m_UserItem[SHOES].sDuration <= 0) sShoesDefense = 0;


	nDefense = sHeadDefense + sBodyDefense + sPantsDefense + sShoesDefense + m_DynamicUserData[MAGIC_DEFENSE_UP] \
			                  + (int)((double)m_sMagicCON * (double)m_DynamicEBodyData[EBODY_CON_TO_DEFENSE] / 100) ; 

	short sMaxLevel = g_arUserLevel.GetSize() + ADD_USER_LEVEL;//yskang temp m_sLevelÀº g_arUserLevelÀÇ ¹è¿­Å©±â¸¦ ³Ñ±æ¼ö ¾ø´Ù.
	if(m_sLevel >= ADD_USER_LEVEL && m_sLevel< sMaxLevel ) 
	{
		nDefense += g_arUserLevel[m_sLevel - ADD_USER_LEVEL]->m_sDefense;
	}

	nDefense += (int)((double) nDefense * (double)m_DynamicEBodyData[EBODY_DEFENSE_UP] / 100); 
	if(nDefense <= 0) 
	{
		nDefense = 0;
	}
	if(m_dwHuanJing > 0)
	{
		nDefense += (int)(nDefense * 0.08);//»Ã¾§
	}
	return nDefense;
}

///////////////////////////////////////////////////////////////////////////////////
//	¾ÆÀÌ¾ð ½ºÅ³
//
double USER::GetIronSkill()
{
	BYTE tWeaponClass = 0;
	
	int IronSkin = 0;
	int iLevel = 0;
	int nDefense = 0;
	int iSkillSid = 0;

	double nISkill = 0;

	if(IsCanUseWeaponSkill(tWeaponClass) == FALSE)		// ÇöÀç ÀÚ½ÅÀÌ ¿À¸¥¼Õ¿¡ µç ¹«±âÀÇ ½ºÅ³À» »ç¿ëÇÒ ¼ö ¾øÀ¸¸é Áï ¸Ç¼ÕÀÌ¸é
	{
		nDefense = (int)((double)m_sMagicCON/3 + 0.5);
		return nDefense; //¾ÆÀÌ¾ð ½ºÅ²ÀÌ ¾øÀ¸¸é 
	}

	int tClass = tWeaponClass * SKILL_NUM;
	// ÀÚ±â ÇöÀç ½ºÅ³Áß... 
	for(int i = tClass; i < tClass + SKILL_NUM; i++)
	{
		iSkillSid = m_UserSkill[i].sSid;

		if(iSkillSid == SKILL_IRON) // ¾ÆÀÌ¾ð ½ºÅ²(9)					// 0 index
		{			
			iLevel = m_UserSkill[i].tLevel;		// ¼ø¼ö ½ºÅ³ ·¹º§ 
			if(iLevel <= 0) continue;
												// ¾ÆÀÌÅÛ¿¡ ÀÇÇÑ ½ºÅ³ º¯µ¿ ·¹º§
			iLevel += m_DynamicUserData[g_DynamicSkillInfo[iSkillSid]] + m_DynamicUserData[MAGIC_ALL_SKILL_UP];

			if(iSkillSid >= g_arSkillTable.GetSize()) return 0;
			if(iLevel >= g_arSkillTable[iSkillSid]->m_arInc.GetSize()) return 0;
			if(iLevel >= SKILL_LEVEL) iLevel = SKILL_LEVEL - 1;

			IronSkin += g_arSkillTable[iSkillSid]->m_arInc.GetAt(iLevel);
		}
	}

	nISkill = ((double)m_sMagicCON/3)*(1 + (double)IronSkin/100.0) + 0.5; // Áõ°¡À²

	if(nISkill <= 0) nISkill = 0;
	return nISkill;
}

///////////////////////////////////////////////////////////////////////////////////
//	Damage °è»ê, ¸¸¾à m_sHP °¡ 0 ÀÌÇÏÀÌ¸é »ç¸ÁÃ³¸®
//
void USER::SetDamage(int nDamage)
{	
	if(nDamage <= 0)	return;	
	if(m_bLive == USER_DEAD)	return;	
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ Test Code
	if( m_sHP <= (short)nDamage )
	{
		if(m_bShengRenjiezhi == TRUE)
		{
			m_sHP = m_sMagicMaxHP;
			SendAccessoriDuration(SID_SHENGREN_JIEZHI);
			m_bShengRenjiezhi = FALSE;
			SendSystemMsg("Ê¥ÈË½äÖ¸Ê¹Äã¸´»î", SYSTEM_ERROR, TO_ME);		
		}		
		else if(InterlockedCompareExchange((LONG*)&m_lDeadUsed, (long)1, (long)0) == (long)0)
		{
			m_sHP = 0;
			Dead();				
		}
		else m_sHP = 1;
	}
	else
	{
		m_sHP -= (short)nDamage;
	}	

	SendHP();
	if(m_bNowBuddy == TRUE) SendBuddyUserChange(BUDDY_CHANGE);		
}

////////////////////////////////////////////////////////////////////////////////////
//  ÓÃ»§ËÀÍö
void USER::Dead()
{
	DWORD dwCurrTime = GetTickCount();

	if(m_bLive == USER_DEAD) return;

	if(UpdateUserData() == FALSE)	
	{
		m_bLive = USER_LIVE;
		m_sHP = 1;
		SendHP();
		TRACE("DEAD HP =1, UpdateUserData() Fail!!!!\n");

		return;
	}

	m_bLive = USER_DEAD;

	ExitRoyalRumble();

	CPoint pos = ConvertToClient( m_curx, m_cury );
	if( pos.x == -1 || pos.y == -1 ) { pos.x = 162; pos.y = 1452; }	

	CBufferEx TempBuf;
	TempBuf.Add(DEAD);
	TempBuf.Add((short)(m_uid + USER_BAND));
	TempBuf.Add((short)pos.x);
	TempBuf.Add((short)pos.y);

	SendInsight(TempBuf, TempBuf.GetLength());
	
	ClearAbnormalTime(ABNORMAL_TIME);
	ClearAbnormalTime(PSI_ONE_TIME);
	ClearAbnormalTime(PSI_TWO_TIME);
	ClearAbnormalTime(PSI_THREE_TIME);
	
	m_dwAbnormalInfo = ABNORMAL_NONE;
	m_dwAbnormalInfo_ = ABNORMAL_NONE;
	//m_dwAbnormalInfo_ |= 1;
		
	if(m_tIsOP == 1) AddAbnormalInfo(OPERATION_MODE);
	
	m_dwNoDamageTime	= 0;
	m_dwLastNoDamageTime= GetTickCount();

	SetPsiAbnormalStatus();
	SetStatus();//Ë¢ÐÂ

	SetUserToMagicUser(0);
	//¿ØÖÆ±äÉíºó¿ª¹Ø
	if(m_dwBFindTime != 0) m_sDramageUp = true;
	if(m_iSkin == 4 || m_iSkin == 7 || m_iSkin == 8) m_sDramageUpPercent = true;

	if(m_tGuildWar == GUILD_WARRING && m_dwFieldWar > 0)   //¾üÍÅÕ½ÖÐ ÍÅ³¤ËÀÁË ¾ÍÊ§°Ü
	{
		if(m_bGuildMaster)		
		{
			CString strMsg = _T("");
			strMsg.Format( IDS_USER_GUILD_DEFEAT, m_strGuildName);//"%s ¾üÍÅ±»´ò°Ü."
			SendGuildWarFieldEnd((LPTSTR)(LPCTSTR)strMsg);
		}
		else EndGuildWar(GUILD_FIELD_WARRING);		
	}

	if(m_bNowTrading == TRUE) 
	{
		BYTE result = 0x00;
		SendExchangeFail(result, (BYTE)0x04);

		USER *pTradeUser = NULL;
		if(m_iTradeUid != -1)
		{
			pTradeUser = GetUser(m_iTradeUid - USER_BAND);
			if(pTradeUser != NULL)	
			{
				if(pTradeUser->m_state == STATE_GAMESTARTED) pTradeUser->SendExchangeFail(result, (BYTE)0x05);
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////
// ËÀÍö¼õ¾­Ñé.ºÍµô¶«Î÷...
void USER::GetLevelDownExp(BOOL bPK, int enemyrank, BOOL bEventSummon,TCHAR *strPKName)
{
	if(bPK == FALSE) return;//¹ÖÎïÉ±ËÀ²»µô¾­Ñé
	BYTE tSlot = 0;
	CBufferEx TempBuf;

	int minusratio = GetMinusExpRatioByCityRank( enemyrank );
    if (!bPK && m_sLevel >= 105) minusratio=minusratio / 2;
	double minusresult = 0.01 * (double)minusratio;

	int skill[] = {0,0,0,0,0};
	int status[] = {0,0,0,0,0};
	int type = 0;
	DWORD minusExp = 0;
	DWORD preExp = 0;
	long lDiffExp = 0;
	int LevelDownType = 10;
	m_dwLastDownExp = 0;

	if(m_bPkStatus) 
	{
		m_dwPkStatusTime = 0;
		m_bPkStatus = FALSE;
	}

	if(m_tGuildHouseWar == GUILD_WARRING) return;// Virtual Room¿¡¼­´Â ¿µÇâÀÌ ¾ø´Ù.
	if(m_sLevel <= 5) return;
	if(CheckInvalidMapType() == 12) return;		// ´ë·ÃÀå
	if(m_curz == 91) return;//PK´óÈü²»µô¾­Ñé
	if(m_curz == 503) 
	{
		SendSystemMsg("ÎÂÜ°ÌáÊ¾Äú:VIPµØÍ¼,ËÀÍö²»µô¾­Ñé.", SYSTEM_ANNOUNCE, TO_ME);
		return;
	}

	if(m_tFortressWar == GUILD_WARRING || m_tGuildWar == GUILD_WARRING)
	{
		minusExp = (DWORD)m_dwExpNext/20;		// ¾üÍÅÕ½Õù¾­ÑéºÍÊÐÃñµÈ¼¶
		LevelDownType = 10;
	}
	if(bEventSummon)							// 
	{
		minusExp = (DWORD)m_dwExpNext/100;		// ÀÌº¥Æ® Ä§°ø ¸ó½ºÅÍ ÀÏ¶§ 1% °¨¼Ò
		LevelDownType = 1;
	}
	else
	{
//		minusExp = (DWORD)m_dwExpNext/10;		// ´ÙÀ½ ·¹º§ °æÇèÄ¡ÀÇ 10%À» ±âÁØÀ¸·Î °¨¼Ò½ÃÅ²´Ù.
		minusExp = (DWORD)((double) m_dwExpNext * minusresult );

		//--------------------------------------------------------------------------------------
		//yskang fortress buger ¸¸¾à °ø¼º Áö¿ªÀÌ¶ó¸é °æÇèÄ¡ 5%
		for(int i = 0; i < g_arGuildFortress.GetSize(); i++)
		{
			//break;
			if(!g_arGuildFortress[i]) continue;

			if(m_curz == g_arGuildFortress[i]->m_iZone)
			{
				minusExp = (DWORD)m_dwExpNext/20;		// °ø¼ºÀüÀÏ¶§...»óÁ¡ ±æµåÀüÀÏ¶§...
				LevelDownType = 5;
				break;
			}
			int type = 0;
			type = CheckInvalidMapType();
			if(type == 8 || type == 10 || type == 15 || type == 9 || type == 11 || type == 16)
			{
				minusExp = (DWORD)m_dwExpNext/20;		// °ø¼ºÀüÀÏ¶§...»óÁ¡ ±æµåÀüÀÏ¶§...
				LevelDownType = 5;
				break;
			}
			if(m_sLevel < 130)//130¼¶Ö®Ç°µô3µã¾­Ñé
			{
				minusExp = (DWORD)m_dwExpNext/40;
				LevelDownType = 1;
				break;
			}
			if(m_sLevel >= 130)//130¼¶Ö®Ç°µô2µã¾­Ñé
			{
				minusExp = (DWORD)m_dwExpNext/50;
				LevelDownType = 1;
				break;
			}
			if(g_arGuildFortress[i]->m_lUsed == 1)
			{
				if(g_arGuildFortress[i]->m_sFortressID == 1000 && type==9)
				{
					minusExp = (DWORD)m_dwExpNext/20;		// °ø¼ºÀüÀÏ¶§...»óÁ¡ ±æµåÀüÀÏ¶§...
					LevelDownType =5;
					break;
				}
				else if(g_arGuildFortress[i]->m_sFortressID == 1001 && type==11)
				{
					minusExp = (DWORD)m_dwExpNext/20;		// °ø¼ºÀüÀÏ¶§...»óÁ¡ ±æµåÀüÀÏ¶§...
					LevelDownType = 5;
					break;
				}
				else if(g_arGuildFortress[i]->m_sFortressID == 1002 && type==16)
				{
					minusExp = (DWORD)m_dwExpNext/20;		// °ø¼ºÀüÀÏ¶§...»óÁ¡ ±æµåÀüÀÏ¶§...
					LevelDownType=5;
					break;
				}
			}
		}
		//-------------------------------------------------------------------------------------------------
	}
//	if(m_bRingOfLife == TRUE && minusExp > 0)	
	
		// ¾ÆÀÌÅÛÀÌ º¯µ¿‰ç´Ù´Â Á¤º¸¸¦ ¾Ë·ÁÁà¾ßÁö
	m_bRingOfLife=false;
	m_bNecklaceOfShield=false;
	m_bEarringOfProtect=false;
	for(int i = 0; i < MAX_ACCESSORI; i++)
	{
		if(m_UserItem[g_iAccessoriSlot[i]].sSid == SID_RING_OF_LIFE)		m_bRingOfLife = TRUE;
		if(m_UserItem[g_iAccessoriSlot[i]].sSid == SID_NECKLACE_OF_SHIELD && m_UserItem[g_iAccessoriSlot[i]].sDuration > 0)	m_bNecklaceOfShield = TRUE;
		if(m_UserItem[g_iAccessoriSlot[i]].sSid == SID_EARRING_OF_PROTECT)	m_bEarringOfProtect = TRUE;
	}
	//if(m_UserItem[0].tMagic[5] >= 10 && m_UserItem[1].tMagic[5] >= 10 && m_UserItem[4].tMagic[5] >= 10 && m_UserItem[8].tMagic[5] >= 10 && m_UserItem[9].tMagic[5] >= 10)//10¸ÄÌ×´øÊØ»¤ÏîÁ´ºÍÎÞÎÒÐ§¹û MarkJay
	//{
	//	m_bNecklaceOfShield = TRUE;
	//	m_bEarringOfProtect = TRUE;
	//}
	
	if(m_bRingOfLife == TRUE)	
	{
		EnterCriticalSection( &m_CS_ExchangeItem );
		SendAccessoriDuration(SID_RING_OF_LIFE);
		LiveReq(NULL, 1);
		SendSystemMsg(IDS_RING_OF_LIFE, SYSTEM_ANNOUNCE, TO_ME);
		m_bRingOfLife=false;//"ÉúÃü½äÖ¸±£»¤ÁËÄã"
		LeaveCriticalSection( &m_CS_ExchangeItem );

		USER* pUser = NULL;
		pUser = GetUser(strPKName);
		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return; 

		if(strcmp(pUser->m_strUserID, m_strUserID) == 0) return;
		if(pUser->m_bNowTrading == TRUE) return;
		if(pUser->m_bPShopOpen == TRUE) return;

		if(pUser->m_curz == 40 || pUser->m_curz == 43 || pUser->m_curz == 44 || pUser->m_curz == 45 || pUser->m_curz == 58 || pUser->m_curz == 61 || pUser->m_curz == 62 || pUser->m_curz == 63 || pUser->m_tFortressWar == GUILD_WARRING)
		{
			return;
		}
		pUser->m_dwShopPingDN += 1; 
		pUser->UpdateUserItemShopPingYB();
		pUser->SendUserStatusSkill();

		CString strMsg = _T("");
		strMsg.Format("PKÖÐ: %s Õ½Ê¤ÁË %s ,»ñµÃÁË 1 Ôª±¦±Ò.",strPKName,m_strUserID);
		SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE, TO_ALL);
		return;
	}
	//if(m_dwVIPTime > 0 && m_VipLive && m_bRingOfLife != TRUE)
	//if(m_tIsOP == 255)
	//SendSystemMsg("<³¬¼¶ÓÃ»§¹¦ÄÜ> -- µãÈ·¶¨ ---- Ô­µØ¸´»î", SYSTEM_ERROR, TO_ME);
	//--------------------------------------------------------------------------------------------------
	//--yskang 0.6 À¯·á »ç¿ëÀÚ  ÇÁ¸®¹Ì¾ö - °æÇèÄ¡ °¨¼Ò¸¦ ´õ ÁÙ¿© ÁØ´Ù. º¸Åë»ç¿ëÀÚÀÇ 1/2¼öÁØÀ¸·Î 
	int tempExp = minusExp;
	if(m_iDisplayType != 5 && m_iDisplayType !=6 && LevelDownType !=1 )
	{
		if(LevelDownType == 5)
			m_dwExp += (DWORD)m_dwExpNext/40; //°æÇèÄ¡¸¦ 2.5% ¿Ã¸° »óÅÂ¿¡¼­ Ã³¸®ÇØÁØ´Ù.
		else if(LevelDownType == 10)
			m_dwExp += (DWORD)((double)m_dwExpNext*minusresult/2.0);
		else
			m_dwExp += (DWORD)m_dwExpNext/20;

		tempExp = minusExp/2;
	}
	//--------------------------------------------------------------------------------------------------

	lDiffExp = m_dwExp - minusExp;
	preExp = m_dwExp;

	m_dwLastDownExp	= tempExp;				// ºÎÈ°ÇÒ °æ¿ì ÀÌ°æÇèÄ¡¸¦ ±âÁØÀ¸·Î °æÇèÄ¡¸¦ º¹±¸ÇÑ´Ù.

    if(m_sLevel > 1)	DropItemFromDeadUser(bPK, enemyrank); //Å£Å£

	if(lDiffExp >= 0)							// °æÇèÄ¡¸¸ °¨¼ÒÇÑ´Ù.
	{
		type = 0;
		m_dwExp = lDiffExp;
		SendExp();
		return;	
	}
	else										// ·¹º§ ´Ù¿îÀÌ ÀÏ¾î³­´Ù.
	{	
		type = 2;
		int sCount = 20;						// ÃÖ´ë ½ÇÇà °¡´ÉÇÑ ·çÇÁ È½¼ö

		lDiffExp = (long)(m_dwExp * (double)100/minusExp);		// 10%°¨¼ÒÁß ¾ó¸¶¸¸Å­ÀÇ %°¡ °¨¼ÒµÇ¾î ÀÖ´ÂÁö...

		m_sLevel = m_sLevel - 1;				// ·¹º§°¨¼ÒÇÏ°í °¨¼ÒµÈ ·¹º§¿¡¼­ ³²Àº%¸¦ ´õ»«´Ù.. 

//		long minus = g_arLevelUpTable[m_sLevel]->m_dwExp/10;
		
		long minus=0;
		if(LevelDownType ==5 )
			minus = (long)(g_arLevelUpTable[m_sLevel]->m_dwExp /20);//5;
		else if(LevelDownType == 1)
			minus = (long)(g_arLevelUpTable[m_sLevel]->m_dwExp /100);//1;
		else
			minus = (long)(g_arLevelUpTable[m_sLevel]->m_dwExp * minusresult);///10;

		minus = (long)(minus * (double)( 1.0 - (double)lDiffExp/100));	// ÇØ´ç ·¹º§¿¡ 10%Áß »©¾ßÇÒ ÃÖÁ¾·®	
		m_dwExp = (DWORD)(g_arLevelUpTable[m_sLevel]->m_dwExp - minus);
												// Skill Point¸¦ °¨ÇÑ´Ù.
		if( m_sLevel < 99 )
		{
			if(g_arLevelUpTable[m_sLevel]->m_tBasicUp)
			{
				if(m_sSkillPoint > 0) m_sSkillPoint -= 1; 
				else 
				{ 
					int iSid = 0;
					int tClass = m_byClass * SKILL_NUM;
					sCount = 0;

					if(m_UserSkill[tClass].tLevel > 0)     { skill[sCount] = tClass; sCount++; }
					if(m_UserSkill[tClass + 1].tLevel > 0) { skill[sCount] = tClass + 1; sCount++; }
					if(m_UserSkill[tClass + 2].tLevel > 0) { skill[sCount] = tClass + 2; sCount++; }
					if(m_UserSkill[tClass + 3].tLevel > 0) { skill[sCount] = tClass + 3; sCount++; }
					if(m_UserSkill[tClass + 4].tLevel > 0) { skill[sCount] = tClass + 4; sCount++; }

	//				while(sCount > 0)					//	0~4, 5~9, 10~14, 15~19±îÁö ½ºÅ³Áß
					if(sCount > 0)
					{
						sCount = sCount - 1;
						type = myrand(0, sCount, TRUE);

						m_UserSkill[skill[type]].tLevel -= 1;
					}			
				}										
			}
		}
		else
		{
			if(m_sLevel <119){//100 - 119 ¼¶Ö®¼ä ¼õ1µã°Ù¼¶¼¼ÄÜ
				int emptyskillslot;
				switch( m_byClass )
				{
					case	BRAWL:
						emptyskillslot = 4;
						break;
					case	STAFF:
						emptyskillslot = 9;
						break;
					case	EDGED:
						emptyskillslot = 14;
						break;
					case	FIREARMS:
						emptyskillslot = 19;
						break;
					default:
						return;
				}

				if(emptyskillslot == -1) return;
				int skillsid = m_UserSkill[emptyskillslot].sSid;

				if(skillsid < 0 || skillsid >= g_arSkillTable.GetSize())
				{
					m_UserSkill[emptyskillslot].sSid = 0;
					return;
				}
				if( g_arSkillTable[skillsid]->m_sSid == -1 ) return;
				m_UserSkill[emptyskillslot].tLevel = (BYTE)((m_sLevel % 100) + 1);
				if(m_sLevel < 100) //·â100µô99Ï´µã,Å­»÷Âú
				{
				    m_UserSkill[emptyskillslot].tLevel = (BYTE)(0);
				}
				if(m_UserSkill[emptyskillslot].tLevel > 20) m_UserSkill[emptyskillslot].tLevel = 20;
			}
			else if(m_sLevel <129)
			{// 120 -129 ¼õ1µã120¼¼ÄÜ×ª»»ºóµÄ¼¼ÄÜ
				int iIndex=m_byClass *SKILL_NUM;
				int i;
				for(i = iIndex; i < iIndex + SKILL_NUM-1; i++)
				{
					if(m_UserSkill[i].sSid>=20)
					{
						m_UserSkill[i].tLevel--;
						if(m_UserSkill[i].tLevel >= 20) m_UserSkill[i].tLevel = 20;
					}
				}
			}
			else if ( f_Markini[0]->KG130 == 1 )//130 ÒÔÉÏ¼õ1µã 130¼¼ÄÜ
			{
				int count = m_UserSkill_[m_byClass*3].tLevel + m_UserSkill_[m_byClass*3+1].tLevel + m_UserSkill_[m_byClass*3+2].tLevel + m_sSkillPoint_;
				if(count == (m_sLevel-130+1)) return;
				if(count > (m_sLevel-130+1))
				{
					if(m_sSkillPoint_ > 0)
					{
						m_sSkillPoint_ -= 1;
					}
					else
					{
						int iSid = 0;
						int tClass = m_byClass * 3;
						sCount = 0;
						if(m_UserSkill_[tClass].tLevel > 0)     { skill[sCount] = tClass; sCount++; }
						if(m_UserSkill_[tClass + 1].tLevel > 0) { skill[sCount] = tClass + 1; sCount++; }
						if(m_UserSkill_[tClass + 2].tLevel > 0) { skill[sCount] = tClass + 2; sCount++; }
						if(sCount > 0)
						{
							sCount = sCount - 1;
							type = myrand(0, sCount, TRUE);
							m_UserSkill_[skill[type]].tLevel -= 1;
						}
					}
				}
			//ResetOver100LevelSkill( m_sLevel );
			}
		}
		
		if(m_sPA > 0)	m_sPA -= 1;
		else									// ´õ ÀÌ»ó ¾ø´Ù¸é ½×¿©ÀÖ´Â Æ÷ÀÎÅÍ¸¦ °¨ÇÑ´Ù.
		{
			sCount = 0;
			if(m_sSTR > m_sTempSTR) { status[sCount] = 0; sCount++; } 
			if(m_sCON > m_sTempCON) { status[sCount] = 1; sCount++; }
			if(m_sDEX > m_sTempDEX) { status[sCount] = 2; sCount++; }
			if(m_sVOL > m_sTempVOL) { status[sCount] = 3; sCount++; }
			if(m_sWIS > m_sTempWIS) { status[sCount] = 4; sCount++; }

			if(sCount > 0)					// 0~4Áß ·¥´ýÀ¸·Î »«´Ù.
			{
				sCount = sCount - 1;
				type = myrand(0, sCount, TRUE);

				type = status[type];

				switch(type)
				{
				case USER_STR:
					if(m_sSTR > m_sTempSTR) m_sSTR -= 1;					
					break;
				case USER_CON:
					if(m_sCON > m_sTempCON) m_sCON -= 1;
					break;
				case USER_DEX:
					if(m_sDEX > m_sTempDEX) m_sDEX -= 1;
					break;
				case USER_VOL:
					if(m_sVOL > m_sTempVOL) m_sVOL -= 1; 
					break;
				case USER_WIS:
					if(m_sWIS > m_sTempWIS) m_sWIS -= 1; 
					break;
				default:
					break;
				}
			}

			SetUserToMagicUser();		// ´É·ÂÄ¡ °¨¼Ò·Î º¯µ¿ÀÌ ÀÏ¾î³ª¸é
		}

		if(m_sHP > m_sMagicMaxHP) m_sHP = m_sMagicMaxHP;
		if(m_sPP > m_sMagicMaxPP) m_sPP = m_sMagicMaxPP;
		if(m_sSP > m_sMagicMaxSP) m_sSP = m_sMagicMaxSP;
	}

	UpdateUserData();

	m_dwExpNext = GetNextLevel(m_sLevel);	// ´ÙÀ½ ·¹º§¾÷À» À§ÇÑ ÃÖ´ëÄ¡

	SendExp((BYTE)0x02);					// ·¹º§¾÷ ÆÐÅ¶À¸·Î Level, Exp, PA, MA, SkillPoint¸¦ °»½Å
	SendUserStatusSkill(); 
	SendSystemMsg( IDS_USER_LEVEL_DOWN, SYSTEM_NORMAL, TO_ME);// "ÄãµÄ¼¶±ðÏÂ½µ"
}

///////////////////////////////////////////////////////////////////////////////////////
// µôÂä×°±¸
void USER::DropItemFromDeadUser(BOOL bPK, int enemyrank)
{
	if(m_state != STATE_GAMESTARTED) return;	// ´Ù¸¥ À¯Àú¿¡¼­ È£ÃâÇÒ¼ö ÀÖÀ¸¹Ç·Î...
	if(!m_MItemLock && f_Markini[0]->JYMM_KG == 1)
	{
		SendSysWhisperChat("½»Ò×ÃÜÂëÉÐÎ´½â³ý,ÉíÉÏ×°±¸²»µôÂä.");
		return;
	}
	int i, sid = -1, invensid = -1, equipsid = -1;
	int rank = m_sCityRank + CITY_RANK_INTERVAL;
	int iRandom = myrand(1, 100);
	int iWhere = myrand(1, 100);

	BOOL bDrop = FALSE;
	BOOL bInven = FALSE;

	CWordArray	arEquip, arInven;

	CBufferEx	TempBuf;

	rank = GetDropItemRankByAttackerCityRank( enemyrank );

	if(CheckGuildWarArea())
	{
		rank -= 1;
		if( rank < 0 ) rank = 0;
	}

	switch(rank)
	{
	case CHAONISE_RANK:
		if(iRandom < 99) { bDrop = TRUE; if(iWhere < 1) bInven = TRUE; }//°ÔÍõ
		break;
	case DEATH_RANK:
		if(iRandom < 95) { bDrop = TRUE; if(iWhere < 5) bInven = TRUE; }//¶ñÈË
		break;
	case EVIL_RANK:
		if(iRandom < 90) { bDrop = TRUE; if(iWhere < 10) bInven = TRUE; }//ÍÁ·Ë
		break;
	case VILLAIN_RANK:
		if(iRandom < 80) { bDrop = TRUE; if(iWhere < 20) bInven = TRUE; }//Ç¿µÁ
		break;		
	case CAIN_RANK:
		if(iRandom < 70) { bDrop = TRUE; if(iWhere < 30) bInven = TRUE; }//»µÈË
		break;
	case COMMOM_RANK:
		if(iRandom < 60) { bDrop = TRUE; if(iWhere < 40) bInven = TRUE; }//Æ½Ãñ
		break;
	case CITIZEN_RANK:
		if(iRandom < 50) { bDrop = TRUE; if(iWhere < 50) bInven = TRUE; }//ºÃÈË
		break;
	case VIUAL_RANK:
		if(iRandom < 40) { bDrop = TRUE; if(iWhere < 60) bInven = TRUE; }//Ó¢ÐÛ
		break;
	case WARRIOR_RANK:
		if(iRandom < 30) { bDrop = TRUE; if(iWhere < 70) bInven = TRUE; }//ÓÎÏÀ
		break;
	case HERO_RANK:		
		if(iRandom < 20) { bDrop = TRUE; if(iWhere < 80) bInven = TRUE; }//ÁìÐä
		break;
	case SAINT_RANK:		
		if(iRandom < 10) { bDrop = TRUE; if(iWhere < 90) bInven = TRUE; }//Ê¥ÈË
		break;

	default:
		bDrop = FALSE;
		break;
	}

	//if(m_curz == 34)//µç1Ê¥ÈËÓÐ¼¸ÂÊµôÂä×°±¸ MarkJay
	//{
 //       if(iRandom < 10) { bDrop = TRUE; if(iWhere < 10) bInven = TRUE; }	
	//}

	if(bDrop)
	{
		arEquip.RemoveAll();
		arInven.RemoveAll();

		for(i = 0; i < EQUIP_ITEM_NUM; i++)
		{
			if(m_UserItem[i].sSid >= 0 && m_UserItem[i].sSid < g_arItemTable.GetSize())
			{
				if(g_arItemTable[m_UserItem[i].sSid]->m_sEvent >= EVENT_UNIQUE_ITEM_BAND) continue;
				if(g_arItemTable[m_UserItem[i].sSid]->m_sSid == 1121) continue;	//ËÀÍöºó¸±ÊÖ²»ÄÜµôÂä Ð¡ºÚ¸±ÊÖ

				arEquip.Add(i);
			}
		}

		for(i = EQUIP_ITEM_NUM; i < TOTAL_INVEN_MAX; i++)
		{
			if(m_UserItem[i].sSid >= 0 && m_UserItem[i].sSid < g_arItemTable.GetSize())
			{
				if(g_arItemTable[m_UserItem[i].sSid]->m_sEvent >= EVENT_UNIQUE_ITEM_BAND) continue;
                if(g_arItemTable[m_UserItem[i].sSid]->m_sSid == 1121) continue;	//ËÀÍöºó¸±ÊÖ²»ÄÜµôÂä Ð¡ºÚ¸±ÊÖ

				arInven.Add(i);
			}
		}

		iWhere = arInven.GetSize();
		if(iWhere > 0)
		{		 
			iRandom = myrand(1, iWhere);
			invensid = arInven[iRandom - 1];
		}

		iWhere = arEquip.GetSize();
		if(iWhere > 0)
		{
			iRandom = myrand(1, iWhere);
			equipsid = arEquip[iRandom - 1];
		}

		if(bInven) sid = invensid;
		else
		{
//			if(rank == SAINT_RANK) return;
			sid = equipsid;
		}
			
		if(rank <= CHAONISE_RANK)
		{
			if(equipsid >= 0) sid = equipsid;
			else			  sid = invensid;
		}

		if(sid != -1 && m_UserItem[sid].sSid >= 0)
		{
			if(m_UserItem[sid].sSid >= 0 && m_UserItem[sid].sSid < g_arItemTable.GetSize())
			{
				/*if(bPK)	// PKµô×°±¸Ö±½ÓÉ¾³ý
				{					
					MakeItemLog(&m_UserItem[sid], ITEMLOG_DEAD_DELETE);
					ReSetItemSlot(&m_UserItem[sid]);
					
					TempBuf.Add(ITEM_GIVE_RESULT);
					TempBuf.Add((BYTE)0x01); 
					
					TempBuf.Add((BYTE)sid);
					TempBuf.Add(m_UserItem[sid].sLevel);
					TempBuf.Add(m_UserItem[sid].sSid);
					TempBuf.Add(m_UserItem[sid].sDuration);
					TempBuf.Add(m_UserItem[sid].sBullNum);
					TempBuf.Add(m_UserItem[sid].sCount);
					
					for(int j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[sid].tMagic[j]);
					
					TempBuf.Add(m_UserItem[sid].tIQ); 
					Send(TempBuf, TempBuf.GetLength());
				}
				else
				{
				*/
					int index = 0;
					
					TCHAR tBuf[10];
					::ZeroMemory(tBuf, sizeof(tBuf));
					
					CPoint pos = ConvertToClient( m_curx, m_cury );
					if( pos.x == -1 || pos.y == -1 ) { pos.x = 1; pos.y = 1; }	// ³ªÁß¿¡ ²À Ã£¾Æ¼­ ¹Ù²ÙÀÚ.
					
					SetShort(tBuf, pos.x, index);
					SetShort(tBuf, pos.y, index);
					
					SetByte(tBuf, (BYTE)sid, index);
					SetShort(tBuf, m_UserItem[sid].sCount, index);
					CString strMsg = _T(" ");
					CString my_strTemp;
					strMsg.Format("PK¿ì±¨: %s µôÂä %d ¸Ä %s :", m_strUserID,m_UserItem[sid].tMagic[5],g_arItemTable[m_UserItem[sid].sSid]->m_strName);
					char szFileName[MAX_PATH + 1];
					GetCurrentDirectory(MAX_PATH,szFileName);//»ñÈ¡³ÌÐòµÄµ±Ç°Ä¿Â¼
					strcat(szFileName,"\\pf.txt");
					FILE *fp;
					char MyTemp[191][50];
					if(fp = fopen(szFileName,"r"))
					{
						for(int j2 = 0;j2 < 191;j2++)
						{
							if(feof(fp))break;
							fgets(MyTemp[j2],50,fp);
							MyTemp[j2][strlen(MyTemp[j2])-1] = 0;   
						}
						fclose(fp);
						for(int j1 = 0;j1 < 4;j1++)
						{
							if(!m_UserItem[sid].tMagic[j1]) continue;	
							my_strTemp.Format("%s ",MyTemp[m_UserItem[sid].tMagic[j1]]);
							strMsg += my_strTemp;
						}
					}
					SendSystemMsg(strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_ANNOUNCE, TO_ALL);					
					//GiveItemToMap(tBuf, TRUE);
					PKGiveItemToMap(tBuf, TRUE);//PKµôÂäÎïÆ·
				//}

				if(!bInven) CheckMagicItemMove();		// ¾ÆÀÌÅÛ º¯µ¿»çÇ×ÀÌÀÖÀ¸¹Ç·Î Ã¼Å©...

			}
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////
//	PP ¸¦ Å¬¶óÀÌ¾ðÆ®·Î º¸³½´Ù.
//
void USER::SendPP(BOOL bLevelDown)
{
	CBufferEx TempBuf;

	if(m_bLive == USER_DEAD && !bLevelDown) return;

	TempBuf.Add(SET_PP);
	TempBuf.Add(m_sPP);
	TempBuf.Add((short)-1);//¿ª»úÀ¶ÁúÐÄ

	Send(TempBuf, TempBuf.GetLength());
}

////////////////////////////////////////////////////////////////////////////////////
//	ÇöÀç PP¸¦ ¾ò´Â´Ù.
//
short USER::GetPP()
{
	BYTE	tWeaponClass = 255;

	int iSkillSid = 0;
	int iLevel = 0, iRandom = 0, iPP = 0;

	double dPlusPP = 1.0; 
	int iAddSP = 0;

	DWORD dwDiffTime = GetTickCount() - m_dwLastPsiTime;

	if(dwDiffTime >= m_iPPIntervalTime)
	{
		if(IsCanUseWeaponSkill(tWeaponClass))	// ÇöÀç ÀÚ½ÅÀÌ ¿À¸¥¼Õ¿¡ µç ¹«±âÀÇ ½ºÅ³ÀÌ ÀÖÀ¸¸é
		{
			int tClass = tWeaponClass * SKILL_NUM; 

			for(int i = tClass; i < tClass + SKILL_NUM; i++)			// ½ºÅ³Áß HPÈ¸º¹ ½ºÅ³ÀÌ ÀÖÀ¸¸é ¹Ý¿µÇÑ´Ù.
			{
				iSkillSid = m_UserSkill[i].sSid;

				if(iSkillSid == SKILL_PP_RECOVER_UP)					// 5 index
				{
					iLevel = m_UserSkill[i].tLevel;		
					if(iLevel < 0) iLevel = 0;

					// ¾ÆÀÌÅÛ¿¡ ÀÇÇÑ ½ºÅ³ º¯µ¿ ·¹º§
					if(iLevel >= 1) iLevel += m_DynamicUserData[g_DynamicSkillInfo[iSkillSid]] + m_DynamicUserData[MAGIC_ALL_SKILL_UP];
				
					if(iLevel >= SKILL_LEVEL) iLevel = SKILL_LEVEL - 1;
					if(iSkillSid >= g_arSkillTable.GetSize()) continue;

					iRandom = (int)((double)XdY(1, 1000) / 10 + 0.5);
					if(iRandom < g_arSkillTable[iSkillSid]->m_arSuccess.GetAt(iLevel)) iPP = 1;

					dPlusPP = 1.0 + iPP * (double)g_arSkillTable[iSkillSid]->m_arRepair.GetAt(iLevel) / 100.0;
				}
			}
		}
								// º¸Á¤°ª
		if(m_iPPRate <= 0) 
		{
			m_iPPRate = 1;
			TRACE("È¸º¹ºñÀ² ¿¡·¯\n");
		}

		if(IsCity() && CheckRecoverTableByClass()) iAddSP = g_arRecoverTable[m_byClass]->m_byTown;
		m_sPP += (int)(dPlusPP * (double)m_sMagicWIS/m_iPPRate ) + iAddSP;
		if(m_sPP > m_sMagicMaxPP) m_sPP = m_sMagicMaxPP;

		m_dwLastPsiTime = GetTickCount();
	}

	return m_sPP;
}

////////////////////////////////////////////////////////////////////////////////////
//	Ä§·¨Ê¹ÓÃÇëÇó
void USER::PsiAttack(TCHAR *pBuf)
{

#ifdef _ACTIVE_USER
		//if(m_iDisplayType == 6 && m_sLevel > 25) return; //yskang 0.5
		if(m_iDisplayType == 6) return; //yskang 0.5
#endif

	DWORD	dwExp = 0;
	int		nDamage = 0;
	int		nTempHP = 0;

	USER*	pUser = NULL;
	CNpc*	pNpc = NULL;

	BYTE	tWeaponClass = 0;
	BOOL	bCanUseSkill = FALSE;
	int		bSuccessSkill[SKILL_NUM] = {FALSE, FALSE, FALSE, FALSE, FALSE};

	int		nPsiRange = 0;
	int		nTPosX	= 0;
	int		nTPosY	= 0;
	int		nDist	= 100;
	short	sNeedPP	= 25000;
	BYTE	tPsiRegi = 0;
	DWORD	dwPsiCast = 0;

	DWORD	dwCurrTick = GetTickCount();

	BOOL	bPsiSuccess = FALSE;

	int index = 0;
	int nTargetID = GetInt(pBuf, index );	// Target ID ¸¦ ¾ò´Â´Ù.
	BYTE byPsi = GetByte(pBuf, index);		// Psionic sid ¸¦ ¾ò´Â´Ù.
	int nPsiX = -1;							// TeleportÇÒ À§Ä¡
	int nPsiY = -1;
	CPoint ptPsi(-1, -1);

	if(byPsi < 0 || byPsi >= TOTAL_PSI_NUM) return;

    int type = 0;
	type = CheckInvalidMapType();	    
    if(type == 8 || type == 10 || type == 15 || type == 9 || type == 11 || type == 16 || m_curz == 92)   
	{
		if(byPsi == 7 || byPsi == 9) return;//ÒªÈû½ûÖ¹Ê¹ÓÃË²ÒÆºÍÉÁ±ÜÄ§·¨
	}

	if(byPsi == PSI_TELEPORT)
	{
		// Teleport ÇÒ À§Ä¡¸¦ ¾ò´Â´Ù.
		nPsiX = GetInt(pBuf, index);
		nPsiY = GetInt(pBuf, index);

		// ÅÚ·¹Æ÷Æ®·Î ÀÌµ¿ÇÒ ¼ö ¾ø´Â ÀÚ¸®ÀÌ¸é Å¬¶óÀÌ¾ðÆ®¿¡¼­ º¸³»ÁØ ÁÂÇ¥ ±×´ë·Î Àü¼Û
		CPoint pt = FindNearAvailablePoint_C(nPsiX, nPsiY); 
		if(pt.x < 0 || pt.y < 0) 
		{
			pt.x = nPsiX;
			pt.y = nPsiY;
		}
		int nRet = -1;
		ptPsi = ConvertToServer(pt.x, pt.y);
		nDist = abs(m_curx - ptPsi.x) + abs(m_cury - ptPsi.y);
		if(ptPsi.x < 0 || ptPsi.y < 0)
		{
			return;//ÐÞ¸´Ä§·¨¿¨×ø±ê MarkJay
		}
		nRet = ((g_zone[m_ZoneIndex]->m_pMap[ptPsi.x][ptPsi.y].m_dwType & 0xFF00 ) >> 8);//1111 1111 0000 0000   0000 0000 1111 1111
		if(nRet >= 0 && nRet < 20)
		{	
	      if(nRet == 8 || nRet == 10 || nRet == 15 || nRet == 9 || nRet == 11 || nRet == 16) return;//ÒªÈû½ûÖ¹Ê¹ÓÃË²ÒÆºÍÉÁ±ÜÄ§·¨
		}
		if(nDist > SCREEN_X + SCREEN_Y) return;
	}
	else	// ´ë»ó¿¡ ÀÇÇÑ Ã¼Å© (º¸°­ÇÒ °Í)
	{
		if(nTargetID < USER_BAND || nTargetID >= INVALID_BAND) return;	// Àß¸øµÈ Target ÀÌ¸é return
	}

	if(m_UserItem[RIGHT_HAND].sSid == -1) return;//Ã»ÓÐÓÒÊÖÎäÆ÷Ôò·µ»Ø MarkJay

	if(IsHavePsionic(byPsi) == FALSE) return;//ÊÇ·ñ´æÔÚºÏ·¨Ä§·¨	
	
	// »çÁ¤°Å¸® °è»ê ------------------------------------------------------------------------//
	nPsiRange = GetPsiRange(byPsi);
	if(nPsiRange > 0)
	{
		if(nTargetID >= USER_BAND && nTargetID < NPC_BAND)	// USER
		{
#ifdef _ACTIVE_USER
	//if(m_iDisplayType == 6 && m_sLevel > 25) return; //yskang 0.5
	if( m_iDisplayType == 6) return; //yskang 0.5
#endif
			pUser = GetUser(nTargetID - USER_BAND);	// User Pointer ¸¦ ¾ò´Â´Ù.
			if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;// Àß¸øµÈ USER ÀÌ¸é ¸®ÅÏ
			if(pUser->m_lDeadUsed == 1) return;
			if(pUser->m_bPShopOpen == TRUE) return;							// User has personal shop
			if((byPsi == 23 || byPsi == 24 || byPsi == 25 || byPsi == 26) && nTargetID == m_uid + USER_BAND) return;//ÐÞ¸´¶Ô×Ô¼ºÊÍ·Å±ØÉ±BUG
			if(pUser->m_tAbnormalKind == ABNORMAL_CONFUSION) return;//Èç¹ûÊÇÑ£ÔÎÔò¿ØÖÆ×ªÒÆ
			if(m_GongXiFaCai == 1 && m_dwGuild != -1)//±ÜÃâÎóÉË MarkJay
			{
				if(m_curz != 40 && m_curz != 43 && m_curz != 44 && m_curz != 45)
				{
					if(byPsi == 2 || byPsi == 4 || byPsi == 5 || byPsi == 8 || byPsi == 10 || byPsi == 23 || byPsi == 24 || byPsi == 25 || byPsi == 26)
					{
						if(pUser->m_dwGuild == m_dwGuild && !IsCity()) return;
					}
				}
			}
			if(byPsi == 23 || byPsi == 25)//È­ºÍ½£µÄ±ØÉ±¾àÀëÖ»ÄÜÊÇ2
			{
				if( m_curz != pUser->m_curz || !GetDistance( pUser->m_curx, pUser->m_cury, (2 + 2) ) ) return;
			}
			// ÇöÀç ÇÑ È­¸é¿¡ ÀÖ´ÂÁö ÆÇ´Ü. ´Ü ¹öµðÀ¯ÀúÀÏ °æ¿ì µÎÈ­¸é±îÁö °¡´É
			if( !CheckBuddyMember(pUser->m_strUserID) )
			{
				if(!IsInSight(pUser->m_curx, pUser->m_cury, pUser->m_curz)) return;
			}
			else
			{
				if( m_curz != pUser->m_curz || !GetDistance( pUser->m_curx, pUser->m_cury, (SCREEN_X + SCREEN_Y) ) ) return;
				// Å¸°ÙÀÌ ¹öµðÀ¯ÀúÀÏ °æ¿ì »çÁ¤°Å¸®´Â µðºñÅ×ÀÌºí ¹«½ÃÇÏ°í 2È­¸é±îÁö °¡´É
				nPsiRange = (SCREEN_X + SCREEN_Y);
			}

			if(pUser->m_dwNoDamageTime != 0) // ¹«Àû½Ã°£Áß¿¡´Â ÁÁÀº »çÀÌ¿À´ÐÀÌ°Ç ³ª»Û »çÀÌ¿À´ÐÀÌ°Ç ½ÃÀüÇÒ ¼ö ¾ø´Ù.
			{
				if((dwCurrTick - pUser->m_dwLastNoDamageTime) > pUser->m_dwNoDamageTime)		// ¹«Àû½Ã°£ÀÌ Ç®¸± ½Ã°£ÀÌ¸é
				{
					pUser->m_dwNoDamageTime = 0;
					pUser->m_dwLastNoDamageTime = GetTickCount();
				}
				else
				{
					pUser->m_dwNoDamageTime = pUser->m_dwNoDamageTime - (dwCurrTick - pUser->m_dwLastNoDamageTime);
					pUser->m_dwLastNoDamageTime = dwCurrTick;
					return;	
				}
			}

			if(CheckBuddyMember(pUser->m_strUserID) || !IsPKZone(pUser))
			{
				if(g_arPsiTable[byPsi]->m_tTarget != 1 && g_arPsiTable[byPsi]->m_tTarget != 2) return;

#ifndef _EVENT_RR 
				// ·Î¿­ ·³ºí ¸Ê¿¡¼­ IsPKZoneÀÌ FALSE¸¦ ¸®ÅÏÇß°í
				// ´Ù¸¥ ¼Ó¼º¿¡ ¼­·Î ¼­ÀÖ´Â °ÍÀÌ¶ó¸é ¸ðµç »çÀÌ¿À´ÐÀÌ ½ÇÆÐ´Ù.
				if(pUser->m_curz == g_RR.m_iZoneNum || m_curz == g_RR.m_iZoneNum ) 
				{
					if( CheckInvalidMapType() != pUser->CheckInvalidMapType() ) return;
				}
#else		// ÀÓ½Ã·Î ÃÖ°­ ÀÌº¥Æ®¶§¹®¿¡
				if( m_curz == g_RR.m_iZoneNum || m_curz == 61 || m_curz == 62 || m_curz ==  63)
				{
					if( CheckInvalidMapType() != pUser->CheckInvalidMapType() ) return;				
				}
				if( pUser->m_curz == g_RR.m_iZoneNum || pUser->m_curz == 61 || pUser->m_curz == 62 || pUser->m_curz ==  63)
				{
					if( CheckInvalidMapType() != pUser->CheckInvalidMapType() ) return;				
				}
#endif
			}
			nTPosX = pUser->m_curx;
			nTPosY = pUser->m_cury;
		}
		else if(nTargetID >= NPC_BAND)				// NPC
		{
			pNpc = GetNpc(nTargetID - NPC_BAND);	// NPC Point ¸¦ ¾ò´Â´Ù.
			if(pNpc == NULL) return;				// Àß¸øµÈ NPC ÀÌ¸é ¸®ÅÏ
			if(pNpc->m_NpcState == NPC_DEAD) return;
			if(pNpc->m_sHP == 0) return;// NPC °¡ ÀÌ¹Ì Á×¾î ÀÖÀ¸¸é ¸®ÅÏ
			if(pNpc->m_tNpcType == NPCTYPE_NPC || pNpc->m_tNpcType == NPCTYPE_DOOR)	return;//ÐÞ¸´É±npcµÄbug
			if(pNpc->m_byAX == 0 && pNpc->m_byAZ == 0 && pNpc->m_tNpcType == 0) return;
			if(pNpc->m_tNpcType == NPCTYPE_GUARD)//Ä§·¨´òÃÅÎÀ¾Í·´»÷
			{
				pNpc->m_Target.id = m_uid + USER_BAND;
				pNpc->m_Target.x = m_curx;
				pNpc->m_Target.y = m_cury;
				pNpc->m_Target.failCount = 0;
				pNpc->Attack(m_pCom);
			}
			if(byPsi == 23 || byPsi == 25)//È­ºÍ½£µÄ±ØÉ±¾àÀëÖ»ÄÜÊÇ2
			{
				if( m_curz != pNpc->m_sCurZ || !GetDistance( pNpc->m_sCurX, pNpc->m_sCurY, (2 + 2) ) ) return;
			}

			if(!IsInSight(pNpc->m_sCurX, pNpc->m_sCurY, pNpc->m_sCurZ)) return;

			nTPosX = pNpc->m_sCurX;
			nTPosY = pNpc->m_sCurY;
		}
		else return;

		if(GetDistance(nTPosX, nTPosY, nPsiRange, &nDist) == FALSE)		// »çÀÌ¿À´Ð ½ÃÀü °Å¸® ¹ÛÀÌ¸é ¸®ÅÏ
		{
			//if(g_bDebug)SendSystemMsg(_T("### Too Long Distance!!!"), SYSTEM_NORMAL, TO_ME);
			return;
		}
	}

	// ¹«±âÀÇ Å¬·¡½º¿Í ÇØ´ç Å¬·¡½ºÀÇ ½ºÅ³À» ¾µ ¼ö ÀÖ´ÂÁö °Ë»ç ---------------//
	bCanUseSkill = IsCanUseWeaponSkill(tWeaponClass);	

	// ¿©À¯ PP Ã¼Å© ---------------------------------------------------------//
	sNeedPP = GetNeedPP(bSuccessSkill, tWeaponClass, byPsi);
	if(sNeedPP > m_sPP) 
	{
		SendPsiAttackResult(FAIL, nTargetID, byPsi);
		SendSystemMsg( IDS_USER_NOT_ENOUGH_PP, SYSTEM_NORMAL, TO_ME);// "ÄãµÄ¾«ÉñÖµ²»¹»."
		return;
	}

	// Casting Delay Ã³¸®
	dwPsiCast = g_arPsiTable[byPsi]->m_sDelayTime;// Ä§·¨ÑÓÊ±

	int iLevel = 0;
	int iDecCast = 0;
	int iSkillSid = 0;
	double dEBodyDec = 0;

	//dwPsiCast -= (int)((double)m_DynamicEBodyData[EBODY_WIS_TO_CAST_DOWN] / 100 * m_sMagicWIS);	// WIS ÀÇ % ¸¦ Ä³½ºÆÃ °¨¼Ò·Î
	//if(dwPsiCast < 0) dwPsiCast = 0;

	dEBodyDec = (double)m_DynamicEBodyData[EBODY_CASTING_DELAY_DOWN] / 100;				// EBODY ¿¡ ÀÇÇÑ Ä³½ºÆÃ ½Ã°£ °¨¼Ò
	if(dEBodyDec >= 1) dEBodyDec = 0.0;

	if(tWeaponClass == STAFF)	
	{													// Ä³½ºÆÃ Å¸ÀÓ °¨¼ÒÃ³¸®		
		if(m_UserItem[RIGHT_HAND].sSid >= 0 && m_UserItem[RIGHT_HAND].sSid < g_arItemTable.GetSize())
		{
			if(g_arItemTable[m_UserItem[RIGHT_HAND].sSid]->m_sCTime > 0)
			{
				dwPsiCast -= g_arItemTable[m_UserItem[RIGHT_HAND].sSid]->m_sCTime;
				if(dwPsiCast < 0) dwPsiCast = 0;
			}
		}

		for(int i = tWeaponClass * SKILL_NUM; i < tWeaponClass * SKILL_NUM + SKILL_NUM; i++)
		{
			iSkillSid = m_UserSkill[i].sSid;

			if(iSkillSid == SKILL_CASTING_DOWN) // Ä³½ºÆÃÅ¸ÀÓ°¨¼Ò			8 index
			{
				// ¼ø¼ö ½ºÅ³ ·¹º§ 				
				iLevel = m_UserSkill[i].tLevel;		
				if(iLevel < 0) iLevel = 0;
				
				// ¾ÆÀÌÅÛ¿¡ ÀÇÇÑ ½ºÅ³ º¯µ¿ ·¹º§
				if(iSkillSid >= g_arSkillTable.GetSize()) break;
				if(iLevel >= 1) iLevel += m_DynamicUserData[g_DynamicSkillInfo[iSkillSid]] + m_DynamicUserData[MAGIC_ALL_SKILL_UP];
				if(iLevel >= g_arSkillTable[iSkillSid]->m_arInc.GetSize()) return;
				if(iLevel >= SKILL_LEVEL) iLevel = SKILL_LEVEL - 1;
								
				iDecCast = g_arSkillTable[iSkillSid]->m_arInc.GetAt(iLevel);
			}
		}
	}
	
	if(iDecCast > 0 || dEBodyDec > 0)
	{
		dwPsiCast = (DWORD)( dwPsiCast * (1 - ((double)iDecCast / 100)- dEBodyDec ));
	}
	//ÊÍ·ÅËÙ¶È
	if(dwPsiCast < 90) return;//ÊÍ·Å93Âú MarkJay
	if(dwPsiCast < 90) dwPsiCast = 90;//ÊÍ·Å¼ä¸ô MarkJay135
	//double TempPsiCast;
	//TempPsiCast = (double)iDecCast + dEBodyDec * 100;
	//CString sysMsg;
	//sysMsg.Format("ÊÍ·ÅÎª %.2f dwPsiCast:%d",(float)TempPsiCast,dwPsiCast);
	//SendSystemMsg((LPTSTR)(LPCTSTR)sysMsg, SYSTEM_NORMAL, TO_ME);
	if(!(byPsi == 23 || byPsi == 24 || byPsi == 25 || byPsi == 26))
	{
		m_dwCastDelay = dwPsiCast;
		if(CheckPsiCastingDelay() == FALSE) return;
	}
	else
	{
		m_dwCastDelay = g_arPsiTable[byPsi]->m_sDelayTime;
		if(dwCurrTick - m_dwLastMaxAttack < m_dwCastDelay) return;
		m_dwLastMaxAttack = dwCurrTick;
		if(m_dwXP <= 0)
		{
			SendSystemMsg( "Õ½¶·¾­Ñé²»×ãÎÞ·¨Ê¹ÓÃ±ØÉ±.", SYSTEM_ERROR, TO_ME);
			return;
		}
		m_dwXP--;
		SendXP();
	}

	if(IsException(nTargetID, byPsi, ptPsi) == FALSE) return;//Ä§·¨²Ù×÷ºÏ·¨ÐÔÅÐ¶Ï
	
	if(byPsi == 23 || byPsi == 25)
	{
		SendPsiAttackResult(SUCCESS, nTargetID, (BYTE)40);//±ØÉ±´øµØÁÑÐ§¹û
		SendPsiAttackResult(SUCCESS, nTargetID, byPsi);
	}
	else if(byPsi == 24 || byPsi == 26)
	{
		SendPsiAttackResult(SUCCESS, nTargetID, (BYTE)190);//±ØÉ±´øµ¼µ¯Ð§¹û
		SendPsiAttackResult(SUCCESS, nTargetID, byPsi);
	}
	else
	{
		SendPsiAttackResult(SUCCESS, nTargetID, byPsi);	 
	}
	BOOL bPsiOK = PsionicProcess(nTargetID, byPsi, ptPsi, sNeedPP);
	
	// PP °¨¼Ò --------------------------------------------------------//
	m_sPP -= sNeedPP;
//·¢ËÍÊ¨ºðµÄ..
	SendPP();
	m_dwLastPsiAttack = GetTickCount();
}

///////////////////////////////////////////////////////////////////////////////////
//	Ä§·¨²Ù×÷ºÏ·¨ÐÔÅÐ¶Ï
BOOL USER::IsException(int iTargetID, BYTE tPsi, CPoint ptPsi)
{	
	USER *pUser = NULL;
	CNpc* pNpc = NULL;

	BOOL bUser = FALSE;
	BOOL bMon = FALSE;
	int type = 0, index = 0;

	if(iTargetID >= USER_BAND && iTargetID < NPC_BAND)	// USER
	{
		pUser = GetUser(iTargetID - USER_BAND);	// User Pointer ¸¦ ¾ò´Â´Ù.
		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return FALSE;// Àß¸øµÈ USER ÀÌ¸é ¸®ÅÏ
		if(pUser->m_bLive == USER_DEAD)	return FALSE;		// Target User °¡ ÀÌ¹Ì Á×¾îÀÖÀ¸¸é ¸®ÅÏ
		bUser = TRUE;
	}
	else if(iTargetID >= NPC_BAND)						// NPC
	{
		pNpc = GetNpc(iTargetID - NPC_BAND);		// NPC Point ¸¦ ¾ò´Â´Ù.
		if(pNpc == NULL) return FALSE;				// Àß¸øµÈ NPC ÀÌ¸é ¸®ÅÏ
		if(pNpc->m_NpcState == NPC_DEAD) return FALSE;// NPC °¡ ÀÌ¹Ì Á×¾î ÀÖÀ¸¸é ¸®ÅÏ
		bMon = TRUE;
	}
	else return FALSE;

	switch((int)tPsi)
	{
	// °ÝÅõ°è¿­ -------------------------------------------------------------------------------//
	case 0:	//Recovery	: SP È¸º¹
		if(bUser) 
		{
			if(pUser->m_dwMindShockTime != 0) return FALSE;	// Mind Shock »óÅÂ¿¡¼­´Â ¾Æ¹« »çÀÌ¿À´Ðµµ ¾È°É¸°´Ù.
		}
		if(bMon) return FALSE;
		break;

	case 1:	// HASTE	: °ø°Ý¼Óµµ Çâ»ó (°ø°Ý µô·¹ÀÌ °¨¼Ò)
		if(bUser) 
		{
			if( pUser->m_dwDexUpTime != 0 || pUser->m_dwShieldTime != 0 || pUser->m_dwMaxHPUpTime != 0 || 
				pUser->m_dwPiercingShieldTime != 0 || pUser->m_dwBigShieldTime !=0 || pUser->m_dwPsiShieldTime != 0 || pUser->m_dwMindShockTime != 0) return FALSE;	// ÀÌ¹Ì ´Ù¸¥ È¿°ú°¡ ÀÖÀ¸¸é Àû¿ë¾ÈµÊ
		}
		if(bMon) return FALSE;
		break;

	case PSIONIC_FAST_RUN:	// ÀÌµ¿¼Óµµ Çâ»ó
		if(bUser) 
		{
			if( pUser->m_dwDexUpTime != 0 || pUser->m_dwShieldTime != 0 || pUser->m_dwMaxHPUpTime != 0 || 
				pUser->m_dwPiercingShieldTime != 0 || pUser->m_dwBigShieldTime !=0 || pUser->m_dwPsiShieldTime != 0 || pUser->m_dwMindShockTime != 0) return FALSE;	// ÀÌ¹Ì ´Ù¸¥ È¿°ú°¡ ÀÖÀ¸¸é Àû¿ë¾ÈµÊ
		}
		if(bMon) return FALSE;

	// ÁöÆÎÀÌ °è¿­ ----------------------------------------------------------------------------//
	case 3:	// Èú¸µ
		if(bUser) 
		{
			if(pUser->m_dwMindShockTime != 0) return FALSE;	// Mind Shock »óÅÂ¿¡¼­´Â ¾Æ¹« »çÀÌ¿À´Ðµµ ¾È°É¸°´Ù.
		}
		if(bMon) return FALSE;
		
		break;

	case 6:	// ½¯µå
		if(bUser) 
		{
			if( pUser->m_dwDexUpTime != 0 || pUser->m_dwBigShieldTime !=0 || pUser->m_dwHasteTime != 0 || pUser->m_dwMaxHPUpTime != 0 || pUser->m_dwMindShockTime != 0 ||
				pUser->m_dwPiercingShieldTime != 0 || pUser->m_dwFastRunTime != 0 || pUser->m_dwPsiShieldTime != 0) return FALSE;	// ÀÌ¹Ì ´Ù¸¥ È¿°ú°¡ ÀÖÀ¸¸é Àû¿ë¾ÈµÊ
		}
		if(bMon) return FALSE;
		break;

	case 7: // ÅÚ·¹Æ÷Æ® 
		if( m_ZoneIndex < 0 || m_ZoneIndex >= g_zone.GetSize() ) return FALSE;
		if(ptPsi.x >= g_zone[m_ZoneIndex]->m_sizeMap.cx || ptPsi.x < 0) return FALSE;
		if(ptPsi.y >= g_zone[m_ZoneIndex]->m_sizeMap.cy || ptPsi.y < 0) return FALSE;

		type = ((g_zone[m_ZoneIndex]->m_pMap[ptPsi.x][ptPsi.y].m_dwType & 0xFF00 ) >> 8);

		if(!CheckInvalidZoneState(type)) return FALSE;

		if(bUser) 
		{
			if(pUser->m_dwMindShockTime != 0) return FALSE;	// Mind Shock »óÅÂ¿¡¼­´Â ¾Æ¹« »çÀÌ¿À´Ðµµ ¾È°É¸°´Ù.
		}
		if(bMon) return FALSE;
		break;

	case PSIONIC_MIND_SHOCK:
		if(bUser) 
		{
			if(pUser->m_dwMindGuardTime != 0) return FALSE;	// Mind Guard »óÅÂ¿¡¼­´Â Mind Shock ¸¦ °É¼ö ¾ø´Ù.
		}
		else return FALSE;

		break;

	// µµ°Ë°è¿­ ------------------------------------------------------------------------------//
	case 9:	// Ecasion	: ¼ø°£È¸ÇÇ
/*		type = CheckInvalidMapType();
//		type = ((g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_dwType & 0xFF00 ) >> 8);
//		if(type > 1 && type < g_arMapTable.GetSize())		// µµ½Ã¸¦ ¹þ¾î³ª¸é...
		if(type > 1 && type < 8)
		{
			index = g_arMapTable[type]->m_sStoreIndex;
			if(index < 0 || index >= g_arStore.GetSize()) return TRUE;

			if(g_arStore[index]->m_lUsed == 1)	return FALSE;  // ÇØ´ç ¼Ó¼º ¸ÊÀÌ ±æµåÀü »óÅÂ	
		}
*/
		type = CheckInvalidMapType();
		if(!CheckInvalidZoneState(type)) return FALSE;

		if(bUser) 
		{
			if(pUser->m_dwMindShockTime != 0) return FALSE;	// Mind Shock »óÅÂ¿¡¼­´Â ¾Æ¹« »çÀÌ¿À´Ðµµ ¾È°É¸°´Ù.
		}
		if(bMon) return FALSE;
		break;

	case PSIONIC_PSI_SHIELD: // »çÀÌ¿À´Ð ½¯µå
		if(bUser) 
		{
			if( pUser->m_dwShieldTime != 0 || pUser->m_dwHasteTime != 0	|| pUser->m_dwMindShockTime != 0 || pUser->m_dwDexUpTime != 0 || 
				pUser->m_dwMaxHPUpTime != 0 || pUser->m_dwBigShieldTime !=0 || pUser->m_dwPiercingShieldTime != 0 || pUser->m_dwFastRunTime != 0) return FALSE; // ÀÌ¹Ì ´Ù¸¥ È¿°ú°¡ ÀÖÀ¸¸é Àû¿ë¾ÈµÊ
		}
		else return FALSE;

		break;

	case 30: // »çÀÌ¿À´Ð ½¯µå
		if(bUser) 
		{
			if( pUser->m_dwShieldTime != 0 || pUser->m_dwHasteTime != 0	|| pUser->m_dwMindShockTime != 0 || pUser->m_dwDexUpTime != 0 || 
				pUser->m_dwMaxHPUpTime != 0 || pUser->m_dwPsiShieldTime != 0|| pUser->m_dwPiercingShieldTime != 0 || pUser->m_dwFastRunTime != 0) return FALSE; // ÀÌ¹Ì ´Ù¸¥ È¿°ú°¡ ÀÖÀ¸¸é Àû¿ë¾ÈµÊ
		}
		if(bMon) return FALSE;

		break;

	// ÃÑ±â -------------------------------------------------------------------------------------------//
	case 11: // Dex Up
		if(bUser) 
		{
			if( pUser->m_dwShieldTime != 0 || pUser->m_dwHasteTime != 0	|| pUser->m_dwMindShockTime != 0 || 
				pUser->m_dwFastRunTime != 0 || pUser->m_dwBigShieldTime !=0 || pUser->m_dwPsiShieldTime != 0) return FALSE; // ÀÌ¹Ì ´Ù¸¥ È¿°ú°¡ ÀÖÀ¸¸é Àû¿ë¾ÈµÊ
		}
		if(bMon) return FALSE;
		break;
	case 12: // Max HP Up
		if(bUser) 
		{
			if( pUser->m_dwShieldTime != 0 || pUser->m_dwHasteTime != 0 || pUser->m_dwMindShockTime != 0 ||
				pUser->m_dwFastRunTime != 0 || pUser->m_dwBigShieldTime !=0 || pUser->m_dwPsiShieldTime != 0) return FALSE; // ÀÌ¹Ì ´Ù¸¥ È¿°ú°¡ ÀÖÀ¸¸é Àû¿ë¾ÈµÊ
		}
		if(bMon) return FALSE;
		break;

	case PSIONIC_PIERCING_SHIELD:	// ÇÇ¾î½Ì½Çµå
		if(bUser) 
		{
			if( pUser->m_dwShieldTime != 0 || pUser->m_dwHasteTime != 0	|| pUser->m_dwMindShockTime != 0 || 
				pUser->m_dwFastRunTime != 0 || pUser->m_dwBigShieldTime !=0 || pUser->m_dwPsiShieldTime != 0) return FALSE; // ÀÌ¹Ì ´Ù¸¥ È¿°ú°¡ ÀÖÀ¸¸é Àû¿ë¾ÈµÊ
		}
		else return FALSE;

		break;

	// ¹«°è¿­ -----------------------------------------------------------------------------------------//
	case 13: // Adamantine
		if(bUser) 
		{
			if( pUser->m_dwMightyWeaponTime != 0 || pUser->m_dwBerserkerTime != 0 || pUser->m_dwMindShockTime != 0) return FALSE; // ÀÌ¹Ì ´Ù¸¥ È¿°ú°¡ ÀÖÀ¸¸é Àû¿ë¾ÈµÊ
		}
		if(bMon) return FALSE;
		break;
	case 14: // Mighty Weapon
		if(bUser) 
		{
			if( pUser->m_dwAdamantineTime != 0 || pUser->m_dwBerserkerTime != 0 || pUser->m_dwMindShockTime != 0) return FALSE; // ÀÌ¹Ì ´Ù¸¥ È¿°ú°¡ ÀÖÀ¸¸é Àû¿ë¾ÈµÊ
		}
		if(bMon) return FALSE;
		break;
	case 15: // Berserker
		if(bUser) 
		{
			if( pUser->m_dwAdamantineTime != 0 || pUser->m_dwMightyWeaponTime != 0 || pUser->m_dwMindShockTime != 0) return FALSE; // ÀÌ¹Ì ´Ù¸¥ È¿°ú°¡ ÀÖÀ¸¸é Àû¿ë¾ÈµÊ
		}
		if(bMon) return FALSE;
		break;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////
//	»çÀÌ¿À´Ð °ø°Ý °á°ú¸¦ Àü¼ÛÇÑ´Ù.
//
void USER::SendPsiAttackResult(BYTE result, int tuid, BYTE psi)
{
	USER* pUser = NULL;
	CBufferEx TempBuf;

	if(m_tIsOP == 1) return;					// ¿î¿µÀÚ ÀÏ¶§´Â »çÀÌ¿À´Ð»óÅÂ°¡ º¯ÇÏ¸é ¾ÈµÈ´Ù. (Åõ¸íÀÌ Ç®·Á¼­...)

	// E-Body Skills
	if(psi == 23 || psi == 25 || psi == 26)
	{
		TempBuf.Add(ATTACK_RESULT);
		if(result != FAIL)
			TempBuf.Add((BYTE)0x06); // Ebody Attack
		else
			TempBuf.Add(result); // Ebody Attack
	}
	else
	{
		TempBuf.Add(PSI_ATTACK_RESULT);
		TempBuf.Add(result);
	}
	if(result == FAIL) 
	{
		Send(TempBuf, TempBuf.GetLength());
		return;
	}
	if(psi == 23  || psi == 25 || psi == 26)
	{
		//TempBuf.Add(psi);
	}
	else
	{
		TempBuf.Add(psi);				// Psionic sid
	}

	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.Add(tuid);

	if(psi == 23 || psi == 24 || psi == 25 || psi == 26)
	{
		if(tuid >= NPC_BAND)
		{
			CNpc *pNpc;
			pNpc = GetNpc(tuid - NPC_BAND);
			if(pNpc == NULL) return;//¹ÖÎïÎª¿ÕÔò·µ»Ø MarkJay
			TempBuf.Add((short)pNpc->m_sHP);
			TempBuf.Add((short)pNpc->m_sMaxHP);
		}
		else if(tuid >= USER_BAND && tuid < NPC_BAND)
		{
			USER *pUser = NULL;
			pUser = GetUser(tuid - USER_BAND);
			if(pUser == NULL) return;//Íæ¼ÒÎª¿ÕÔò·µ»Ø MarkJay
			TempBuf.Add((short)pUser->m_sHP);
			TempBuf.Add((short)pUser->m_sMagicMaxHP);
		}

	}

	// ÃÖ°­ ÀÌº¥Æ®¶§¹®¿¡ ¿î¿µÆÀÀÌ ¿äÃ» 02-11-1 
	if(m_curz == 61 || m_curz == 62 || m_curz == 63)
	{
		SendInsight(TempBuf, TempBuf.GetLength());
	}
	else
	{
		if(tuid >= USER_BAND && tuid < NPC_BAND)	// USER
			pUser = GetUser(tuid - USER_BAND);

		if( pUser != NULL )
		{
			if( ((pUser->m_curx < m_curx-16 && pUser->m_curx >= m_curx+16
				&& pUser->m_cury < m_cury-16 && pUser->m_cury >= m_cury+16)
				|| (( abs( m_curx - pUser->m_curx ) + abs( m_cury - pUser->m_cury ) ) > 16))
				&& pUser->m_curz == m_curz )
				pUser->Send(TempBuf, TempBuf.GetLength());
		}

		SendExactScreen(TempBuf, TempBuf.GetLength());	// 02-10-28 by Youn Gyu
	}
}

////////////////////////////////////////////////////////////////////////////////////
//	ÆÕÍ¨ÎïÀí¹¥»÷¹ý³Ì
void USER::Attack(TCHAR* pBuf)
{
#ifdef _ACTIVE_USER
	if(m_iDisplayType == 6) return; //yskang 0.5
#endif

	if( m_UserItem[RIGHT_HAND].sSid == 669 || m_UserItem[RIGHT_HAND].sSid == 670 )		// ¿À¸¥¼Õ¿¡ Ã¤Áý¿ë ¾ÆÀÌÅÛÀ» ÀåÂøÇß´Ù¸é
	{
		Collect( pBuf );
		return;
	}

	USER*	pUser	= NULL;
	CNpc*	pNpc	= NULL;
	int		nTPosX	= 0;
	int		nTPosY	= 0;
	int		nAttackRange = 0;
	int		nDist	= 100;

	int		nDamage = 0;
	int		nDefense = 0;
//	double	nIronSkin = 0;
	int		nFinalDamage = 0;

	BYTE	tWeaponClass = 0;
	BYTE	tTargetClass = 0;

	BOOL	bCanUseSkill = FALSE;
	int		iRandom = 100;

	int		nHit	= 0;
	int		nAvoid	= 0;
//	int		nWeaponHit	= 0;

	BOOL	bIsHit	= FALSE;
	double	nVCHit	= 0;
	double	nCGuard	= 0;
	BOOL	bIsCritical = FALSE;

	int		nInitDamage = 0;
	
	short	sItemDefense[4] = {0, 0, 0, 0};

	BOOL	bSuccessSkill[SKILL_NUM] = {FALSE, FALSE, FALSE, FALSE, FALSE};
	BOOL	bTargetSkill[SKILL_NUM] = {FALSE, FALSE, FALSE, FALSE, FALSE};
	
	BYTE	tActionIndex = HIT_NONE;
	BYTE	tNpcClass = 0;

	short	sOldDuration = 0, sNewDuration = 0;
	short	sOldBullNum = 0;

	DWORD	dwCurrTick = GetTickCount();

	int		iFireErrDec = 0;
//	double	determine = 0;
//	int iAttackDex = 0, iDefenseDex = 0;
	int	determine = 0;
	int iDexHitRate = 0, iLevelHitRate = 0;
	int iMyDex = 0, iYourDex = 0;

	CByteArray	arSkillAction1, arSkillAction2;							// Skill Action Array

	DWORD dwExp = 0;
	int index = 0;
	int nTargetID = GetShort(pBuf, index);								// È¡µÃServerID

	if(nTargetID < USER_BAND || nTargetID >= INVALID_BAND) return;		// Èç¹ûServerIDÊÇ´íÎó ·µ»Ø

	//ÅÐ¶ÏÍæ¼ÒÊ¹ÓÃ57ÀàÐÍ//
	bCanUseSkill = IsCanUseWeaponSkill(tWeaponClass);	

	if(bCanUseSkill)//Èç¹ûÓÐ57Ôò¼¼ÄÜÉúÐ§
	{
		IsSkillSuccess(bSuccessSkill, tWeaponClass);					//²é¿´¼¼ÄÜÊÇ·ñ³É¹¦
	}

	if(CheckAttackDelay(bSuccessSkill, tWeaponClass) == FALSE)			// Attack Delay Check
	{
//		SendAttackFail(ERR_SHORT_ATTACK_DELAY);
		return;
	}

	// 57ÄÍ¾Ã -------------------------------------------------------------//
	if(tWeaponClass != 255)	// ¸Ç¼ÕÀÌ ¾Æ´Ï¸é
	{
		if(m_UserItem[RIGHT_HAND].sDuration <= 0)
		{
//			SendAttackFail(ERR_BROKEN);
			SendSystemMsg( IDS_USER_ITEM_DAMAGED, SYSTEM_NORMAL, TO_ME);// "ÎïÆ·³Ö¾Ã²»¹»ÐèÒªÐÞÀí."
			return;
		}
	}

	// Target °úÀÇ °Å¸®°è»ê ----------------------------------------------------//
	if(nTargetID >= USER_BAND && nTargetID < NPC_BAND)	//Èç¹û¹¥»÷ÊÇÍæ¼Ò
	{
		pUser = GetUser(nTargetID - USER_BAND);				// User Pointer ¸¦ ¾ò´Â´Ù.
		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;
		if(pUser->m_bLive == USER_DEAD || pUser->m_sHP == 0) return;
		if(m_GongXiFaCai == 1 && m_dwGuild != -1)//±ÜÃâÎóÉË MarkJay
		{
			if(m_curz != 40 && m_curz != 43 && m_curz != 44 && m_curz != 45)
			{
				if(pUser->m_dwGuild == m_dwGuild && !IsCity()) return;
			}
		}
		if(pUser->m_bPShopOpen == TRUE)
		{
			return;
		}

		if(CheckBuddyMember(pUser->m_strUserID)) return;	// °°Àº ¹öµð¿øÀÌ¸é ½ÇÆÐ
		if(!IsPKZone(pUser))
		{
			SendAttackMiss(nTargetID); 
			return;
		}	// PKÁ¸ÀÌ ¾Æ´Ï¹Ç·Î ½Î¿ï¼ö¾ø´Ù.

		if(!m_MItemLock && f_Markini[0]->JYMM_KG == 1 && (!IsCity()))
		{
			SendSysWhisperChat("¶Ô²»Æð!½»Ò×ÃÜÂëÉÐÎ´½â³ý,ÎÞ·¨¹¥»÷¶Ô·½.");
			return;
		}

		if(m_dwCKMiMa == 0 && m_dwGuarDianTianShi == 0)
		{
			SendSystemMsg("Çë³äÖµ,Ã»ÓÐÓÎÏ·Ê±¼äÎÞ·¨¿ªPK.", SYSTEM_ERROR, TO_ME );
			return;
		}

		if(pUser->m_dwNoDamageTime != 0)		
		{
			if((dwCurrTick - pUser->m_dwLastNoDamageTime) > pUser->m_dwNoDamageTime)		// ¹«Àû½Ã°£ÀÌ Ç®¸± ½Ã°£ÀÌ¸é
			{
				pUser->m_dwNoDamageTime = 0;
				pUser->m_dwLastNoDamageTime = GetTickCount();
			}
			else
			{
				pUser->m_dwNoDamageTime = pUser->m_dwNoDamageTime - (dwCurrTick - pUser->m_dwLastNoDamageTime);
				pUser->m_dwLastNoDamageTime = dwCurrTick;
				return;	
			}
		}

		nTPosX = pUser->m_curx;
		nTPosY = pUser->m_cury;
	}
	else if(nTargetID >= NPC_BAND)				//Èç¹û¹¥»÷ÊÇ¹ÖÎï
	{
		pNpc = GetNpc(nTargetID - NPC_BAND);	// NPC Point ¸¦ ¾ò´Â´Ù.
		if(pNpc == NULL) return;				// Àß¸øµÈ NPC ÀÌ¸é ¸®ÅÏ
		if(pNpc->m_NpcState == NPC_DEAD || pNpc->m_sHP <= 0)
		{
//			pNpc->Dead(); 
			//pNpc->m_Delay = pNpc->SendDead(m_pCom, 0);
			return;// NPC °¡ ÀÌ¹Ì Á×¾î ÀÖÀ¸¸é ¸®ÅÏ
		}
		if(pNpc->m_tNpcType == NPCTYPE_NPC || pNpc->m_tNpcType == NPCTYPE_DOOR)	return;			//ÐÞ¸´É±npcµÄbug
		if(pNpc->m_byAX == 0 && pNpc->m_byAZ == 0 && pNpc->m_tNpcType == 0) return;			// °ø°Ý´É·ÂÀÌ ¾ø´Â ¸ó½ºÅÍ(ÇöÀç ¿ø¼®)Àº ÀÏ¹Ý °ø°ÝÀÌ µÇÁö ¾Ê´Â´Ù.

		nTPosX = pNpc->m_sCurX;
		nTPosY = pNpc->m_sCurY;
	}

	nAttackRange = GetAttackRange(m_UserItem[RIGHT_HAND].sSid);//µÃµ½Éä³Ì
	nAttackRange += 1;	// ¹°¸®ÀûÀÎ Å¸°ÝÀ»°¡ÇÒ¶§¸¸ +1¸¦ ÇØÁØ´Ù.(ÀÌµ¿ ¼Óµµ¶§¹®¿¡ ¸÷À» ÀâÁö ¸øÇØ¼­ º¸Á¤À¸·Î...)
	if(tWeaponClass == FIREARMS)
	{
		nAttackRange += m_DynamicUserData[MAGIC_RANGE_UP];//Éä³ÌÔö¼Ó 
	}

	if(GetDistance(nTPosX, nTPosY, nAttackRange, &nDist) == FALSE)		// °ø°Ý°¡´É°Å¸® ¹ÛÀÌ¸é ¸®ÅÏ
	{
		if(g_bDebug) SendSystemMsg(_T("¾àÀëÌ«Ô¶ÁË"), SYSTEM_NORMAL, TO_ME);
		return;
	}

	//ÊÇ·ñ»¹ÓÐÄÜÁ¿°üÊý-------------------------------------//
	if(tWeaponClass == EDGED || tWeaponClass == FIREARMS)
	{
		if(m_UserItem[RIGHT_HAND].sBullNum <= 0)
		{
			if(tWeaponClass == EDGED) SendSystemMsg( IDS_USER_SWORD_PACK_EMPTY, SYSTEM_NORMAL, TO_ME);//"ÄãµÄÎäÆ÷ÒÑ¾­Ã»ÓÐÄÜÁ¿."
			if(tWeaponClass == FIREARMS) SendSystemMsg( IDS_USER_BULLET_EMPTY, SYSTEM_NORMAL, TO_ME);//"ÄãµÄÎäÆ÷ÒÑ¾­Ã»ÓÐÄÜÁ¿."
			return;
		}
		else SetDecBullNum(RIGHT_HAND, 1);//¹¥»÷³É¹¦ ÄÜÁ¿°üÊý¼õ1
	}

	// È¡µÃÍæ¼ÒÃüÖÐ-------------------------------------------------------//
	nHit = GetHitRate(m_UserItem[RIGHT_HAND].sSid, tWeaponClass);
	if(nHit < 0) nHit = 0;


	// È¸ÇÇ°ª/¸íÁßÆÇÁ¤/µ¥¹ÌÁö °è»ê/³»±¸µµ °è»ê/ÃÖÁ¾ °ø°Ý½Ã°£ ¼ÂÆÃ----------//
	if(nTargetID >= USER_BAND && nTargetID < NPC_BAND)	// USER
	{
		pUser = GetUser(nTargetID - USER_BAND);	// User Pointer ¸¦ ¾ò´Â´Ù.
		
		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;// Àß¸øµÈ À¯ÀúÀÌ¸é ¸®ÅÏ
		if(pUser->m_bLive == USER_DEAD || pUser->m_sHP == 0) return;	// Target User °¡ ÀÌ¹Ì Á×¾îÀÖÀ¸¸é ¸®ÅÏ

		// Î»ÖÃÊÇ²»ÊÇÔÚÒ»Æð? ÄÜ²»ÄÜ´òµ½?
		if(!IsInSight(pUser->m_curx, pUser->m_cury, pUser->m_curz)) return;
		
		//È¡µÃ±»´òÍæ¼Ò»Ø±Ü
		nAvoid = pUser->GetAvoid();
		
		// ¸íÁß¿©ºÎ ÆÇ´Ü
		iRandom = (int)((double)XdY(1, 1000) / 10 + 0.5); //´Ù¾ç¼ºÀ» º¸ÀåÇÏ±â À§ÇØ(1, 100)
		
//		iDefenseDex = pUser->m_sMagicDEX;
//		iAttackDex = m_sMagicDEX;	
//		determine = 200 * ((double)iAttackDex / (iAttackDex + iDefenseDex)) * ((double)m_sLevel / (m_sLevel + pUser->m_sLevel));

		if(m_dwDexUpTime) iMyDex = m_sMagicDEX + 10;
		else iMyDex = m_sMagicDEX;

		if(pUser->m_dwDexUpTime) iYourDex = pUser->m_sMagicDEX + 10;
		else iYourDex = pUser->m_sMagicDEX;

		iDexHitRate = (int)( 30.0 * ( (double)iMyDex/(iMyDex + iYourDex) ) + 15.0 );
		iLevelHitRate = (int)( 70.0 * ( (double)m_sLevel/(pUser->m_sLevel + m_sLevel) ) + 15.0 );

		determine = iDexHitRate + iLevelHitRate + nHit + m_Hit - (nAvoid + pUser->m_Avoid);

//		index = (int)(determine + nHit - nAvoid);

		if(determine < ATTACK_MIN) determine = ATTACK_MIN;			// ÃÖ¼Ò
		else if(determine > ATTACK_MAX) determine = ATTACK_MAX;		// ÃÖ´ë

		if(iRandom < determine)	bIsHit = TRUE;		// ¸íÁß
		else bIsHit = FALSE;

		//Ã»ÓÐÃüÖÐ...·¢ËÍ¹¥»÷ÎÞÐ§...
		if(bIsHit == FALSE)					
		{
			SendAttackMiss(nTargetID);
			return;
		}

		ResetJiaoBenCheck();//¹¥»÷Íæ¼ÒµÄÊ±ºò²»³öÑéÖ¤ MarkJay
		pUser->ResetJiaoBenCheck();		

		//¼ÆËã·À ----------------------------------------------------------------//
		nDamage = GetNormalInitDamage(tWeaponClass, pUser, bIsCritical);//È¡µÃÍæ¼ÒµÄ·À.
		if(nDamage < 0) nDamage = 0;
		if(pUser->m_tAbnormalKind == ABNORMAL_BYTE_COLD) nDamage += 10;		// »ó´ëÆíÀÌ ³Ã±â ÀÌ»óÀÌ¸é µ¥¹ÌÁö Ãß°¡

		// ÃÖÁ¾µ¥¹ÌÁö
		nFinalDamage = GetFinalDamage(pUser, nDamage, tWeaponClass,bIsCritical);
		nFinalDamage = nFinalDamage + m_DynamicMagicItem[7];
		nFinalDamage = nFinalDamage - (pUser->m_DynamicMagicItem[5] + pUser->m_DynamicMagicItem[6]);
		if(nFinalDamage < 15) nFinalDamage = 15;		// ¹æ¾î ÄÚµå

		sNewDuration = pUser->SendDamagedItem(nFinalDamage); // »ó´ë¹æ ³»±¸µµ °¨¼Ò¸¦ º¸³½´Ù.

		if(tWeaponClass != 255)//Íæ¼Ò´ò¼Ü¼õÎäÆ÷³Ö¾Ã MarkJay
		{
			sNewDuration = (int)myrand(1,8);//Ä¬ÈÏ²»×¢Ïú MarkJay
			if(tWeaponClass != FIREARMS )
			{
				SendDuration(RIGHT_HAND,sNewDuration);//´ò¼Ü²»¼õÎäÆ÷ÄÍ¾Ã MarkJay
			}
			else
			{
				iRandom = (int)((double)XdY(1, 1000) / 10 + 0.5);
				if(m_UserItem[RIGHT_HAND].sSid >= 0 && m_UserItem[RIGHT_HAND].sSid < g_arItemTable.GetSize())
				{
					if(iRandom < g_arItemTable[m_UserItem[RIGHT_HAND].sSid]->m_byErrorRate)
					{
						iFireErrDec = 4;//Ä¬ÈÏ4´ò¼Ü¿ÛÄÍ¾Ã MarkJay
						if(iFireErrDec < 0) iFireErrDec = 0;
						SendDuration(RIGHT_HAND, iFireErrDec);
					}
				}
			}
		}

		BOOL bIsLegal = IsLegalDefence_isdead(pUser);		// ÀÏ´Ü °ø°ÝÇÏ¸é Á¤´ç¹æÀ§ÀÎÁö ÆÇ´Ü//yskang 0.7
		//PK´óÈü
		if(f_Markini[0]->PK_DSKG == 1)
		{
			if(m_curz == 91 && pUser->m_sHP <= (short)nFinalDamage)
			{
				if(m_dwPKAddkill <= 100)
				{
					m_dwDN += f_Markini[0]->PK_JIANG;
					m_dwPKAddkill += 1;
					UpdateUserItemDN();
					SendMoneyChanged();
					CString strMsg = _T("");
					strMsg.Format( "PK´óÈüÖÐ: %s Õ½Ê¤ÁË %s ,»ñµÃÁË %d ÓÎÏ·±Ò É±ÈËÊý: %d ." ,m_strUserID,pUser->m_strUserID,f_Markini[0]->PK_JIANG,m_dwPKAddkill);
					SendSystemMsg(strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_ANNOUNCE, TO_ALL);
				}
				else 
				{
					m_dwPKAddkill += 1;
					CString strMsg = _T("");
					strMsg.Format( "PK´óÈüÖÐ: %s Õ½Ê¤ÁË %s ,Íê³É´óÈüÉ±ÈËÊý100ÈÎÎñ,É±ÈËÊý: %d ." ,m_strUserID,pUser->m_strUserID,m_dwPKAddkill);
					SendSystemMsg(strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_ANNOUNCE, TO_ALL);
				}
			}
		}
		pUser->SetDamage(nFinalDamage);
//======================Íæ¼ÒPKÎïÀí¹¥»÷ÉËº¦ÖµÏÔÊ¾===============================
			CString strMsg;
			int iMaxHP = pUser->m_sMagicMaxHP;
			if(m_sMagicMaxHP > m_sHP) iMaxHP = pUser->m_sMagicMaxHP;
			strMsg.Format("%s : %d / %d",pUser->m_strUserID,pUser->m_sHP,iMaxHP);
			SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
//==============================================================================
//		if(pUser->m_bLive == FALSE)
		if(pUser->m_lDeadUsed == 1)
		{
			CString strMsg = _T("");
			strMsg.Format( IDS_USER_ATTACK_FROM, m_strUserID,pUser->m_strUserID);
			int type = 0;
        	type = CheckInvalidMapType();
			if(m_tFortressWar == GUILD_WARRING || m_tGuildWar == GUILD_WARRING) //Ö»ÓÐÉóÇë¾üÍÅ²ÅµôÊÐÃñµÈ¼¶	
        	if(type == 8 || type == 10 || type == 15 || type == 9 || type == 11 || type == 16)  bIsLegal = TRUE; 
			int tempRank = m_sCityRank + CITY_RANK_INTERVAL;
			if(bIsLegal == FALSE)//yskang 0.7
				IsChangeCityRank(m_sCityRank, pUser);
			pUser->GetLevelDownExp(USER_PK, tempRank, FALSE, m_strUserID);					// °æÇèÄ¡¿Í ±×¿Ü º¯È­·®¸¦ ¹Ý¿µÇÑ´Ù.
			pUser->SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE, TO_ALL);
			CString msg =_T("");
			msg.Format("Äã±» %s É±º¦ÁË.",m_strUserID);
			pUser->SendSysWhisperChat(msg.GetBuffer(0));
			//PKÉ±ËÀÍ·¶¥ÏÔÊ¾ MarkJay
			CBufferEx PotionBuf;
			PotionBuf.Add(USE_POTION);
			PotionBuf.Add(m_uid + USER_BAND);
			PotionBuf.Add((BYTE)14);
			SendInsight(PotionBuf, PotionBuf.GetLength());
		}
		if(abs(m_sLevel - pUser->m_sLevel) >= f_Markini[0]->XHBH_DJ)//¼ÓÈëÐÂÊÖ±£»¤ÅÐ¶Ï	
		{
			if(pUser->m_bPkStatus == FALSE || m_sCityRank < 0)
			{
				if(m_curz != 40 && m_curz != 43 && m_curz != 44 && m_curz != 45 && m_curz != 58 && m_curz != 61 && m_curz != 62 && m_curz != 63 && m_iHair != 179 && m_curz != 91)
				{
					Closeuser(2);
					TownPotal();
					CString strMsg = _T("");
					strMsg.Format( "%s É±Ð¡ºÅ±»¹Ø½ø¼àÓü2·ÖÖÓ!", m_strUserID);
					SendSystemMsg(strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_ANNOUNCE, TO_ALL);
				}
			}
		}
		// °ø°Ý °á°ú Àü¼Û ------------------------------------------------------------//
		//yskang 0.3 SendAttackSuccess(nTargetID, arSkillAction1, arSkillAction2, pUser->m_sHP, pUser->m_sMagicMaxHP);
		SendAttackSuccess(nTargetID,bIsCritical, pUser->m_sHP, pUser->m_sMagicMaxHP);//yskang 0.3
		m_dwLastAttackTime = GetTickCount();

/*		//yskang 0.4 ¿î¿µÀÚ ·Î±× ±â·ÏÇÏ±â
		TCHAR strOP[1024]; ZeroMemory(strOP,sizeof(strOP));
		sprintf(strOP,"PKINFO:ACCOUNT%s : CharID%s",pUser->m_strAccount, pUser->m_strUserID);
		WriteOpratorLog(strOP,PKINFO_LOG);
*/
		return;
	}

	if(nTargetID >= NPC_BAND)				// NPC
	{
		pNpc = GetNpc(nTargetID - NPC_BAND);		// NPC Point ¸¦ ¾ò´Â´Ù.
		
		if(pNpc == NULL) return;					// Àß¸øµÈ NPC ÀÌ¸é ¸®ÅÏ
		if(pNpc->m_NpcState == NPC_DEAD) return;
		if(pNpc->m_sHP == 0) return;

		if(m_dwCKMiMa == 0 && m_dwGuarDianTianShi == 0)
		{
			SendSystemMsg("Çë³äÖµ,Ã»ÓÐÓÎÏ·Ê±¼äÎÞ·¨´ò¹Ö.", SYSTEM_ERROR, TO_ME );
			return;
		}

		if(pNpc->m_tNpcType == NPCTYPE_GUARD)					// °æºñº´ÀÌ¸é Å¸°ÙÀ» ÇØ´ç À¯Àú·Î
		{
			pNpc->m_Target.id = m_uid + USER_BAND;
			pNpc->m_Target.x = m_curx;
			pNpc->m_Target.y = m_cury;
			pNpc->m_Target.failCount = 0;
			pNpc->Attack(m_pCom);

			return;
		}

		// ÇöÀç ÇÑ È­¸é¿¡ ÀÖ´ÂÁö ÆÇ´Ü
		if(!IsInSight(pNpc->m_sCurX, pNpc->m_sCurY, pNpc->m_sCurZ)) return;

		if(m_dwDexUpTime) iMyDex = m_sMagicDEX + 10;
		else iMyDex = m_sMagicDEX;

		iDexHitRate = (int)( 30.0 * ( (double)iMyDex/(iMyDex + pNpc->m_sDEX) ) + 15.0 );
//		iDexHitRate = (int)( 30.0 * ( (double)m_sMagicDEX/(m_sMagicDEX + pNpc->m_sDEX) ) + 15.0 );
		iLevelHitRate = (int)( 70.0 * ( (double)m_sLevel/(pNpc->m_byClassLevel + m_sLevel) ) + 15.0 );

		determine = iDexHitRate + iLevelHitRate + nHit + m_Hit;

		if(determine < ATTACK_MIN) determine = ATTACK_MIN;			// ÃÖ¼Ò
		else if(determine > ATTACK_MAX) determine = ATTACK_MAX;		// ÃÖ´ë

		iRandom = (int)((double)XdY(1, 1000) / 10 + 0.5); //´Ù¾ç¼ºÀ» º¸ÀåÇÏ±â À§ÇØ(1, 100)

		if(iRandom < determine)	bIsHit = TRUE;		// ¸íÁß
		else bIsHit = FALSE;

		pUser = GetUser(m_uid);

		if(pNpc->m_tNpcType == NPCTYPE_GUILD_GUARD)	pNpc->ChangeTarget(pUser, m_pCom);

		// °ø°Ý ¹Ì½º
		if(bIsHit == FALSE)					
		{
			SendAttackMiss(nTargetID);
			return;
		}
		// NPC ¹æ¾î°ª 
		nDefense = pNpc->GetDefense();

		// ¸íÁßÀÌ¸é //Damage Ã³¸® ----------------------------------------------------------------//
		nDamage = GetNormalInitDamage(tWeaponClass, NULL,bIsCritical,FALSE);
		if(nDamage < 0) nDamage = 0;

		// ¸íÁßÀÌ¸é //Damage Ã³¸® ----------------------------------------------------------------//
		if(bIsHit == TRUE)
		{
			nVCHit = 0.0;
			nVCHit = GetCriticalHit(tWeaponClass,0);
			if(nVCHit > 0.0) bIsCritical = TRUE; //yskang 0.3

			nFinalDamage = (int)((double)nDamage - ((double)nDefense * nVCHit));
			
			if(nFinalDamage < 0) nFinalDamage = 0;
		}
		
		if(tWeaponClass != 255)//´ò¹Ö¼õÎäÆ÷³Ö¾Ã MarkJay
		{
			sNewDuration = (int)myrand(1,8);//Ä¬ÈÏ²»×¢Ïú MarkJay
			if(tWeaponClass != STAFF )
			{
				SendDuration(RIGHT_HAND,sNewDuration);//´ò¹Ö²»¼õÎäÆ÷ÄÍ¾Ã MarkJay
			}
			else
			{
				iRandom = (int)((double)XdY(1, 1000) / 10 + 0.5);
				if(m_UserItem[RIGHT_HAND].sSid >= 0 && m_UserItem[RIGHT_HAND].sSid < g_arItemTable.GetSize())
				{
					if(iRandom < g_arItemTable[m_UserItem[RIGHT_HAND].sSid]->m_byErrorRate)
					{
						iFireErrDec = 4;//Ä¬ÈÏ4´ò¹Ö¿ÛÄÍ¾Ã MarkJay
						if(iFireErrDec < 0) iFireErrDec = 0;
						SendDuration(RIGHT_HAND, iFireErrDec);
					}
				}
			}
		}

		// Calculate Target HP -------------------------------------------------------//
		short sOldNpcHP = pNpc->m_sHP;
//·¢ËÍ¶Ô¹ÖÎïÉËº¦ÊýÖµ
		
	//	CString strMsg;
	//	strMsg.Format("Äú²úÉú¹¥»÷ÊÇ %d,Äú¶Ô %s ×îÖÕÉËº¦ÊÇ %d",nDamage,pNpc->m_strName,nFinalDamage);
	//	SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
		nFinalDamage = nFinalDamage + m_DynamicMagicItem[7];

		//¿ªÆô¹ÖÎï·ÀÓùÉúÐ§ MarkJay
		if(m_byClass == 1)	nFinalDamage -= pNpc->m_sVOL;
		else	nFinalDamage -= pNpc->GetDefense();

//		if(pNpc->SetDamage(nFinalDamage, m_strUserID, m_uid + USER_BAND, m_pCom) == FALSE)
		if(pNpc->SetDamage(nFinalDamage, m_uid + USER_BAND, m_pCom) == FALSE)
		{
			pNpc->SendExpToUserList(m_pCom); // °æÇèÄ¡ ºÐ¹è!!
			pNpc->SendDead(m_pCom);

			if(m_tGuildHouseWar == GUILD_WARRING && pNpc->m_NpcVirtualState == NPC_WAIT)
			{
				CheckGuildHouseWarEnd();
			}
			else
			{
				if(m_tQuestWar == GUILD_WARRING) CheckQuestEventZoneWarEnd();
				int diffLevel = abs(m_sLevel - pNpc->m_byClassLevel);
				if(diffLevel < f_Markini[0]->iZdJyxz)
				{
					m_iCityValue=m_iCityValue+250;
					GetCityRank();
					CheckMaxValue(m_dwXP, f_Markini[0]->idwXP);		// ¸÷ÀÌ Á×À»¶§¸¸ 1 Áõ°¡!	
					SendXP();
				}
			}
		}
//=============Íæ¼ÒÎïÀíÈº¹¥=================
	if(f_Markini[0]->QunGong_KG == 1)
	{
        int Damage = 0;//ÓÃÓÚÈº¹¥ÉËº¦Öµ±äÁ¿
		//if((m_UserItem[4].sSid == 849 && m_UserItem[4].tMagic[5] >= 10) || (m_UserItem[4].sSid == 635 && m_UserItem[4].tMagic[5] >= 10) || (m_UserItem[4].sSid == 803 && m_UserItem[4].tMagic[5] >= 10)) 
		if(m_curz != 92 && m_UserItem[4].sSid != -1 && m_UserItem[4].tMagic[5] >= 10)
		{
			int iRandom = myrand(0,100);			
			if(iRandom > 45 && iRandom < 54) 
            {
				if(m_byClass != 1)
				{
     				Damage = GetMaxDamage(m_byClass);
					if(GetQunGongAttack(m_curx,m_cury,Damage) >= 1)
					{
						SendSystemMsg("ÌìÉñÖ®Å­´ÍÓèÈ«ÆÁÄ»¹¥»÷!", SYSTEM_NPC, TO_ME);
					}
				}
		    }
		}
	}
		//======================¹ÖÎï°Ù·Ö±ÈÉËº¦ÖµÏÔÊ¾===============================
		CString strMsg1 =_T("");
		double r = (pNpc->m_sHP * 100.0 / pNpc->m_sMaxHP);	
		CBufferEx TempBuf;
		TempBuf.Add(CHAT_RESULT);
		TempBuf.Add(SUCCESS);
		TempBuf.Add(NORMAL_CHAT);
		TempBuf.Add(pNpc->m_sNid + NPC_BAND);
		TCHAR tempName[50];
		sprintf(tempName,"(Lv.%d)%s",pNpc->m_byClassLevel,pNpc->m_strName);
		TempBuf.AddString(tempName);
		strMsg1.Format("%.2f%%", r);
		TempBuf.AddString((LPTSTR)(LPCTSTR)strMsg1);
		SendInsight(TempBuf, TempBuf.GetLength());
		//==========================================
		SendAttackSuccess(nTargetID, bIsCritical, pNpc->m_sHP, pNpc->m_sMaxHP);//yskang 0.3

		m_dwLastAttackTime = GetTickCount();
	}
}


///////////////////////////////////////////////////////////////////////////////////
//	ÊÜµ½¹¥»÷×°±¸¼õÄÍ¾Ã	
int USER::SendDamagedItem(int totalDamage)
{
	int iRandom = XdY(1, 1000);

	int iDur = (int)((double)totalDamage * 0.1/10 + 0.5);

	if(iRandom < 200) 
	{ 
//		m_UserItem[HEAD].sDuration -= iDur; 
//		if(m_UserItem[HEAD].sDuration < 0) m_UserItem[HEAD].sDuration = 0; 
		SendDuration(HEAD, iDur);
	}
	else if(iRandom < 450)
	{ 
//		m_UserItem[PANTS].sDuration -= iDur; 
//		if(m_UserItem[PANTS].sDuration < 0) m_UserItem[PANTS].sDuration = 0; 
		SendDuration(PANTS, iDur);
	}
	else if(iRandom < 850)
	{ 
//		m_UserItem[BODY].sDuration -= iDur; 
//		if(m_UserItem[BODY].sDuration < 0) m_UserItem[BODY].sDuration = 0; 
		SendDuration(BODY, iDur);
	}
	else
	{ 
//		m_UserItem[SHOES].sDuration -= iDur; 
//		if(m_UserItem[SHOES].sDuration < 0) m_UserItem[SHOES].sDuration = 0; 
		SendDuration(SHOES, iDur);
	}

	return iDur;
}


///////////////////////////////////////////////////////////////////////////////////
//	¹Ý°ÝÀÌ ÀÖÀ¸¸é ¹Ý°ÝÀ¸·Î
//	
int USER::GetCounterAttack()
{
	BYTE	tWeaponClass = 255;
	int		iLevel = 0;
	int		CounterAttack = 0;

/*	if(IsCanUseWeaponSkill(tWeaponClass) == FALSE)	// ÇöÀç ÀÚ½ÅÀÌ ¿À¸¥¼Õ¿¡ µç ¹«±âÀÇ ½ºÅ³À» »ç¿ëÇÒ ¼ö ¾øÀ¸¸é Áï ¸Ç¼ÕÀÌ¸é
	{
		return CounterAttack;
	}

	int tClass = tWeaponClass * SKILL_NUM; 

	// ÀÚ±â ÇöÀç ½ºÅ³Áß... 
	for(int i = tClass; i < tClass + SKILL_NUM; i++)
	{
		if(i == MAGIC_BACK_ATTACK) // ¹Ý°Ý
		{
			iLevel = m_UserSkill[i].tLevel;		// ¼ø¼ö ½ºÅ³ ·¹º§ 
			if(iLevel <= 0) return 0;
												// ¾ÆÀÌÅÛ¿¡ ÀÇÇÑ ½ºÅ³ º¯µ¿ ·¹º§
			iLevel += m_DynamicUserData[m_DynamicSkillInfo[i]] + m_DynamicUserData[MAGIC_ALL_SKILL_UP];

			if(i >= g_arSkillTable.GetSize()) return 0;
			if(iLevel >= g_arSkillTable[i]->m_arSuccess.GetSize()) return 0;

			CounterAttack += g_arSkillTable[i]->m_arSuccess.GetAt(iLevel);
		}
	}

	CounterAttack += m_DynamicUserData[MAGIC_BACK_ATTACK];// ¾ÆÀÌÅÛ¿¡ÀÇÇÑ ÀÚÃ¼ È¸ÇÇÀ² Áõ°¡
	
	CounterAttack = CounterAttack/100;
*/
	return (int)CounterAttack;	
}

///////////////////////////////////////////////////////////////////////////////////
//	°ø°Ý PK ´ë»óÀÌ °°Àº ¹öµð ±¸¼º¿øÀÌ¸é ½ÇÆÐ
//	
BOOL USER::CheckBuddyMember(TCHAR *pID)
{
	int nLen = 0;
	nLen = strlen(pID);
	if(nLen <= 0 || nLen > CHAR_NAME_LENGTH) return FALSE;

	for(int i = 0; i < MAX_BUDDY_USER_NUM; i++)
	{
		if(m_MyBuddy[i].uid < USER_BAND) continue;

		nLen = 0;
		nLen = strlen(m_MyBuddy[i].m_strUserID);
		if(nLen <= 0 || nLen > CHAR_NAME_LENGTH) continue;

		if(strcmp(m_MyBuddy[i].m_strUserID, pID) == 0) return TRUE;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////////
//	»ñµÃ¾­Ñé
void USER::GetExp(DWORD iNpcExp)					
{
#ifdef _ACTIVE_USER
	if( m_iDisplayType == 6) return; 
#endif
	CString strMsg = _T("");
	int iRate = 0;
	short sTempLevel = 0;
	short sLevel = m_sLevel;
	DWORD dwExp = m_dwExp;
	DWORD dwExpNext = m_dwExpNext;
	int count = 0;
	if(iNpcExp <= 0) return;

	GetCityRank();							

	if(m_sLevel >= ZUIGAO_LEVEL) { m_sLevel = ZUIGAO_LEVEL; return; }
															
	CheckMaxValue((DWORD &)dwExp, (DWORD)iNpcExp);

	while(dwExp >= dwExpNext)				
	{
		if(sLevel == ZUIGAO_LEVEL) { dwExp = dwExpNext; break; }
		if(sLevel < ZUIGAO_LEVEL)
		{
			count += 1;
			sLevel += 1;//Éý¼¶ÁË
			if(sLevel == ZUIGAO_LEVEL)
			{
				strMsg.Format( IDS_USER_HIGHEST_LEVEL, m_strUserID);//"¹§Ï² %s ,ÒÑ´ïµ½×î¸ß¼¶."
				SendSystemMsg(strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_ANNOUNCE, TO_ALL);
			}
		}
		/*if(sLevel >= 120 && !FindEvent(1210))//µÈ¼¶ÍÆ¹ã½±Àø
		{
			UpdateEventLevelUser(sLevel);
			AddMyEventNum(1210);
			//break;
		}
		if(sLevel >= 130 && !FindEvent(1211))
		{
			UpdateEventLevelUser(sLevel);
			AddMyEventNum(1211);
			//break;
		}
		if(sLevel >= 140 && !FindEvent(1212))
		{
			UpdateEventLevelUser(sLevel);
			AddMyEventNum(1212);
			//break;
		}
		if(sLevel >= 150 && !FindEvent(1213))
		{
			UpdateEventLevelUser(sLevel);
			AddMyEventNum(1213);
			//break;
		}*/
		//if(sLevel == 150) m_dwRenWu = 0;//150¼¶´ò¹ÖÇåÁã MarkJay
		UpdateHighEventLevelUser(sLevel);
				
		dwExp = dwExp - dwExpNext;
		dwExpNext = GetNextLevel(sLevel);
	} 
	if(count != 0)
	{
		strMsg.Format( IDS_USER_NEXT_LEVEL, m_strUserID);//"%s ¹§Ï²ÄãÉý¼¶ÁË."
		SendSystemMsg(strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_NORMAL, TO_ME);
	}
	if((sLevel - m_sLevel) > 0)				
	{
		BYTE tType = 2;
		sTempLevel = m_sLevel;

		m_dwExp = dwExp;				
		m_dwExpNext = dwExpNext;		
		m_sLevel = sLevel;					

		while(sTempLevel < sLevel)		
		{
			sTempLevel++;
			CheckMaxValue((short &)m_sPA, (short)1);	

			if( sTempLevel < 100 )
			{
				if((sTempLevel - 1) >= g_arLevelUpTable.GetSize()) continue;

				if(g_arLevelUpTable[sTempLevel - 1]->m_tBasicUp)
				{
					CheckMaxValue((short &)m_sSkillPoint, (short)1);
				}
			}
			else	
			{
				ResetOver100LevelSkill( sTempLevel );
			}
		}

	    if(CheckAbnormalInfo(ABNORMAL_MAX_HP_UP)) SetUserToMagicUser(1);
	    else SetUserToMagicUser();

		m_sHP = m_sMagicMaxHP;				
		m_sPP = m_sMagicMaxPP;
		m_sSP = m_sMagicMaxSP;

		SendExp(tType);						
		SendUserStatusSkill();
	}

#ifdef _ACTIVE_USER

		if(m_iDisplayType == 6) 
		{			
			CString strMsg = _T("");
			strMsg.Format("À¯Àú´ÔÀÌ Á¢¼ÓÇÏ½Å Ã¼ÇèÆÇ ¼­¹ö½º´Â ÀÏºÎ ±â´É»ó Á¦¾àÀÌ ÀÖ½À´Ï´Ù.");
			SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE, TO_ME);
		}
#endif
	
	m_dwExp = dwExp;					

	SendExp();

	UpdateUserData();

	if(f_Markini[0]->CH_lV_KG == 1)//³ÆºÅÏÔÊ¾ÅÅÐÐ MarkJay
	{
		ChangeLoveName();
	}
}

//////////////////////////////////////////////////////////////////////////////////
// ×é¶Ó¾­ÑéÉèÖÃ
void USER::GetExpBuddy(int iNpcExp )
{
	int		i;
	int		nBuddyUsers;
	int		nBuddyLevelSum;
	DWORD	dwBonusExp;
	double	fLevelAdvantage;

	USER*		pUser;
	BuddyList	NearBuddyUser[MAX_BUDDY_USER_NUM];


	if( !m_bNowBuddy ) GetExp( iNpcExp );

	nBuddyUsers = 0;
	nBuddyLevelSum = 0;
	memset( NearBuddyUser, -1, sizeof(BuddyList)*MAX_BUDDY_USER_NUM );
	int diffLevel = 0;

	for(i = 0; i < MAX_BUDDY_USER_NUM; i++)
	{
		if(m_MyBuddy[i].uid != -1 )
		{
			if( m_MyBuddy[i].uid != (m_uid + USER_BAND) )  // ³» ÀÚ½ÅÀÌ ¾Æ´Ñ ÆÄÆ¼¿øÀÌ¶ó¸é..
			{
				pUser = GetUser( m_MyBuddy[i].m_strUserID );
				if( pUser != NULL ) 
				{
					diffLevel = abs(m_sLevel - pUser->m_sLevel);
					if( m_curz == pUser->m_curz && GetDistance( pUser->m_curx, pUser->m_cury, (SCREEN_X + SCREEN_Y) ) &&  diffLevel <= 100)	//ÏÞÖÆ×é¶ÓµÈ¼¶²»ÄÜ³¬¹ý5¼¶²ÅÓÐ¾­Ñé·Ö³É)
					{
						NearBuddyUser[i].uid = m_MyBuddy[i].uid;
						strcpy( NearBuddyUser[i].m_strUserID, m_MyBuddy[i].m_strUserID );

						nBuddyLevelSum += pUser->m_sLevel;
					}
				}

			}

			nBuddyUsers++;
		}
	}
	// ¹öµð¿ø ¼ö¿¡ µû¶ó º¸³Ê½º °æÇèÄ¡. 2¸í:10%, 3¸í:15%, 4¸í:20%
	dwBonusExp = (DWORD)((double)( iNpcExp * (nBuddyUsers*5) )/100 + 0.5);

	if( dwBonusExp != 0 && nBuddyLevelSum != 0 )	
	for( i=0; i< MAX_BUDDY_USER_NUM; i++ )
	{
		if( NearBuddyUser[i].uid != -1 ) 
		{
			pUser = GetUser( NearBuddyUser[i].m_strUserID );

			if( pUser != NULL )
			{
				// ºÐ¹èµÇ´Â °æÇèÄ¡ = (Ãß°¡ °æÇèÄ¡ * °¢ ¹öµð¿øÀÇ ·¹º§ /»ç³ÉÇÑ À¯ÀúÀÇ ·¹º§) 
				//					* °¢ ¹öµð¿øÀÇ ·¹º§ / °æÇèÄ¡¸¦ ¾òÀº ¹öµð¿øÀÇ ·¹º§ ÃÑÇÕ
				if(pUser->m_bLive == USER_LIVE)
				{
					fLevelAdvantage = (double)pUser->m_sLevel/(double)m_sLevel;
					if( fLevelAdvantage > 1 ) fLevelAdvantage = 1;
					pUser->GetExp( (int)(dwBonusExp*fLevelAdvantage*pUser->m_sLevel/nBuddyLevelSum+0.5) );
				}
			}
		}
	}
	GetExp( iNpcExp+dwBonusExp );
}

////////////////////////////////////////////////////////////////////////////////////
// ¾­Ñé
void USER::GetExpCommon( int iNpcExp )
{
	if(m_sLevel >= 100 && m_dwGuild == -1) 
	{
		SendSystemMsg( "100¼¶ÒÔÉÏ¼ÓÈë¾üÍÅ²ÅÄÜ»ñµÃ¾­Ñé.", SYSTEM_ANNOUNCE, TO_ME );
		return;
	}
	CString strMsg = _T("");
	if(m_sLevel >= 110 && m_sLevel < 120)
	{
		if(m_dwRenWu >= (DWORD)f_Markini[0]->GuaiWuNum_150_155)
		{
			strMsg.Format( "×¢Òâ:ÄúÒÑ³¬¹ý½ñÌì´ò¹ÖÏÞÖÆÊý: %d ,ÎÞ·¨»ñµÃ¾­Ñé.",m_dwRenWu);
			SendSystemMsg( (LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME );
			return;
		}
		else
		{
			m_dwRenWu += 1;
		}
	}
	else if(m_sLevel >= 120 && m_sLevel <= 135)
	{
		if(m_dwRenWu >= (DWORD)f_Markini[0]->GuaiWuNum_155_160)
		{
			strMsg.Format( "×¢Òâ:ÄúÒÑ³¬¹ý½ñÌì´ò¹ÖÏÞÖÆÊý: %d ,ÎÞ·¨»ñµÃ¾­Ñé.",m_dwRenWu);
			SendSystemMsg( (LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME );
			return;
		}
		else
		{
			m_dwRenWu += 1;
		}
	}
    int exp;
	int olNum = 0;
	if(f_Markini[0]->SBEXP_KG == 1) iNpcExp = iNpcExp * f_Markini[0]->ioExp * 3;
    else iNpcExp = iNpcExp * f_Markini[0]->ioExp;//³õÊ¼»¯¾­Ñé
	if(m_sLevel <= 50)
	{
		iNpcExp = iNpcExp * f_Markini[0]->iNpcExp_50;//50¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý32
	}
	else if(m_sLevel < 60)
	{
		iNpcExp = iNpcExp * f_Markini[0]->iNpcExp_60;//60¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý16
	}
	else if(m_sLevel < 70)
	{
		iNpcExp = iNpcExp * f_Markini[0]->iNpcExp_70;//70¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý8
	}
	else if(m_sLevel < 80)
	{
	 	iNpcExp = iNpcExp * f_Markini[0]->iNpcExp_80;//80¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý4  
	}
	else if(m_sLevel < 90)
	{
	 	iNpcExp = iNpcExp * f_Markini[0]->iNpcExp_90;//90¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý2 
	}
	else if(m_sLevel < 99)
	{
	 	iNpcExp = iNpcExp * f_Markini[0]->iNpcExp_99;//99¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý2 
	}
	else if(m_sLevel < 100)
	{
	 	iNpcExp = iNpcExp * f_Markini[0]->iNpcExp_100;//100¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý
	}
	else if(m_sLevel < 104)
	{
	 	iNpcExp = iNpcExp * f_Markini[0]->iNpcExp_104;//104¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý
	}
	else if(m_sLevel < 105)
	{
	 	iNpcExp = iNpcExp * f_Markini[0]->iNpcExp_105;//105¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý
	}
	else if(m_sLevel < 106)
	{
	 	iNpcExp = iNpcExp / f_Markini[0]->iNpcExp_106;//106¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý
	}
	else if(m_sLevel < 107)
	{
	 	iNpcExp = iNpcExp / f_Markini[0]->iNpcExp_107;//107¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý
	}
	else if(m_sLevel < 108)
	{
	 	iNpcExp = iNpcExp / f_Markini[0]->iNpcExp_108;//108¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý
	}
	else if(m_sLevel < 109)
	{
	 	iNpcExp = iNpcExp / f_Markini[0]->iNpcExp_109;//109¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý
	}
	else if(m_sLevel < 110)
	{
	 	iNpcExp = iNpcExp / f_Markini[0]->iNpcExp_110;//110¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý
	}
	else if(m_sLevel < 115)
	{
	 	iNpcExp = iNpcExp / f_Markini[0]->iNpcExp_115;//115¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý
	}
	else if(m_sLevel < 120)
	{
		iNpcExp = iNpcExp / f_Markini[0]->iNpcExp_120;//120¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý
 	}
	else if(m_sLevel < 125)
	{
		iNpcExp = iNpcExp / f_Markini[0]->iNpcExp_125;//125¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý
 	}
	else if(m_sLevel < 130)
	{
		iNpcExp = iNpcExp / f_Markini[0]->iNpcExp_130;//130¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý
 	}
	else if(m_sLevel < 140)
	{
		iNpcExp = iNpcExp / f_Markini[0]->iNpcExp_140;//140¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý
 	}
	else if(m_sLevel < 150)
	{
		iNpcExp = iNpcExp / f_Markini[0]->iNpcExp_150;//150¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý
 	}
	else if(m_sLevel < 155)
	{
		iNpcExp = iNpcExp / f_Markini[0]->iNpcExp_155;//155¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý
 	}
	else
	{
		iNpcExp = iNpcExp / f_Markini[0]->iNpcExp_160;//160¼¶ÒÔÏÂµÄ¾­Ñé±¶Êý
 	}
	exp = iNpcExp;

	if(m_dwFUExpTime > 0)//ÐÒ¸£Ê±¹âÔö¼Ó¾­Ñé±¶Êý
	{
		//exp += exp * 2;
		exp += (int)((double)exp * 0.1);
	}
	if(isGuildRen == TRUE)//³á°ò¾­Ñé±¶Êý
	{
		exp += (int)((double)exp * 0.2);
	}
	if( m_dwHiExpTime > 0 )//ÐË·ÜÊ±¾­Ñé±¶Êý
	{
		//exp += exp * 2;
		exp += (int)((double)exp * 0.2);
	}
	if( m_dwVIPTime > 0)//VIPÊ±¾­Ñé±¶Êý
	{
		//exp += exp * 4;
		exp += (int)((double)exp * 0.1);
	}

	//if(m_bNowBuddy)	GetExpBuddy( exp );//ÅÐ¶ÏÊÇ·ñÓÐ×é¶Ó  ×é¶Ó·Ö³É¾­Ñé
	if(m_bNowBuddy == TRUE)//×é¶Ó¸öÈË¾­Ñé¼Ó³É
	{
		for(int t = 0;t < MAX_BUDDY_USER_NUM;t++)
		{
			if(m_MyBuddy[t].uid != -1)
			{
				olNum++;
			}
		}
		if(olNum == 2)
		{
			exp += (int)((double)exp * 0.05);
		}
		if(olNum == 3)
		{
			exp += (int)((double)exp * 0.10);
		}
		if(olNum == 4)
		{
			exp += (int)((double)exp * 0.15);
		}
	}
	GetExp( exp );
	strMsg.Format("»ñµÃ¾­Ñé£º%d",exp);
	SendSystemMsg((LPTSTR)(LPCTSTR)strMsg,SYSTEM_NORMAL,TO_ME);
}

///////////////////////////////////////////////////////////////////////////////////
//  °æÇèÄ¡¸¦ º¸³½´Ù. (·¹º§¾÷ÀÏ¶§ °ü·Ã ¼öÄ¡¸¦ ÁØ´Ù)
//
void USER::SendExp(int tType)
{
	CBufferEx TempBuf;

	TempBuf.Add(LEVEL_UP);
	TempBuf.Add((BYTE)tType);
	TempBuf.Add(m_dwExp);

	if(tType == 2)								// ·¹º§¾÷
	{		
		TempBuf.Add(m_dwExpNext);
		TempBuf.Add(m_sLevel);
		TempBuf.Add(m_sSkillPoint);
		TempBuf.Add(m_sPA);
	}

	Send(TempBuf, TempBuf.GetLength());
}

///////////////////////////////////////////////////////////////////////////////////
//  ¼Óµã  
void USER::UserStatusSkillUp(TCHAR *pBuf)
{
	int index = 0;
	int iPlusMinus = 1;
	BYTE tType = GetByte(pBuf, index);			// PA, MA ¶Ç´Â Skill LevelÀÎÁö ÆÇ´Ü
	BYTE tKinds = GetByte(pBuf, index);			// °¢ Á¾·ù¸¦ ÆÇ´Ü

	if(tType >= 4) return;
	if(tKinds < 0 || tKinds >= TOTAL_SKILL_NUM) return;
	
	int iNum = 0;
	int MaxPA = 0;//¼Óµã×î¸ßÖµ
	MaxPA = f_Markini[0]->JG_DX;
	CBufferEx TempBuf;

	if(tType == PA_MA_STATUS) 
	{
		if(m_sPA <= 0) return;

		switch(tKinds)
		{
		case USER_STR:  //Á¦Á¿
			iNum = m_sSTR;
			iNum += iPlusMinus;							
			if(iNum <= 0 || iNum > MaxPA) return;			
			m_sSTR = iNum; m_sPA -= 1;
			break;

		case USER_CON:  //ÌåÖÊ
			iNum = m_sCON;
			iNum += iPlusMinus;
			if(iNum <= 0 || iNum > MaxPA) return;
			m_sCON = iNum; m_sPA -= 1;
			break;

		case USER_DEX:   //Ãô½Ý
			iNum = m_sDEX;
			iNum += iPlusMinus;
			if(iNum <= 0 || iNum > MaxPA) return;
			m_sDEX = iNum; m_sPA -= 1;
			break;

		case USER_VOL:    //ÖÇ»Û
			iNum = m_sVOL;
			iNum += iPlusMinus;
			if(iNum <= 0 || iNum > MaxPA) return;
			m_sVOL = iNum; m_sPA -= 1;
			break;

		case USER_WIS:    //ÖÇÁ¦
			iNum = m_sWIS;
			iNum += iPlusMinus;
			if(iNum <= 0 || iNum > MaxPA) return;
			m_sWIS = iNum; m_sPA -= 1;
			break;
		}

		SetUserToMagicUser();		// ±âº»´É·Â°ªÀÌ º¯ÇßÀ¸¹Ç·Î ´Ù½Ã ¼ÂÆÃ
									// ±âº»´É·ÂÄ¡ ºÐ¹è¸¦ ±â¾ï
	}
	else if(tType == SKILL_STATUS)
	{
		if(CheckSkillClass(tKinds) != m_byClass) return;
		if(m_sSkillPoint <= 0 || m_sSkillPoint > 500) return;// Æ÷ÀÎÆ®°¡ 1ÀÌ»ó
		if(m_UserSkill[tKinds].tLevel >= 20) return;

		m_UserSkill[tKinds].tLevel += 1;
		m_sSkillPoint -= 1;
	}
	else if(tType == 0x3)
	{
		if(m_sLevel < 130) return;
		if(tKinds/3 != m_byClass) return;
		if(m_sSkillPoint_ <= 0 || m_sSkillPoint_ > 500) return;
		if(m_UserSkill_[tKinds].tLevel >= 20) return;
		m_UserSkill_[tKinds].tLevel += 1;
		m_sSkillPoint_ -= 1;
		CheckMagicItemMove();
		UpdateUserData();
		return;
	}
	else return;
	SendUserStatusSkill();
}


///////////////////////////////////////////////////////////////////////////////////
//	¸üÐÂÈËÎïÊôÐÔµã½çÃæ
void USER::SendUserStatusSkill()
{
	CBufferEx TempBuf;
	BYTE tWeaponClass = 0;

	short	sTemp = 0;
	short	sMaxDamage = 0, sMaxDefense = 0;

	TempBuf.Add(USER_STATUS_SKILL_UP_RESULT);

	IsCanUseWeaponSkill(tWeaponClass);
	sMaxDamage = GetMaxDamage(tWeaponClass);
	
	sTemp = (short)GetIronSkill();
	sMaxDefense = sTemp + GetDefense(); 

	TempBuf.Add(m_sPA);
	TempBuf.Add(m_sMagicSTR);
	TempBuf.Add(m_sMagicCON);
	TempBuf.Add(m_sMagicDEX);
	TempBuf.Add(m_sMagicVOL);
	TempBuf.Add(m_sMagicWIS);
	if(m_sHP > m_sMagicMaxHP) m_sHP = m_sMagicMaxHP;
	if(m_sPP > m_sMagicMaxPP) m_sPP = m_sMagicMaxPP;
	if(m_sSP > m_sMagicMaxSP) m_sSP = m_sMagicMaxSP;

	TempBuf.Add(m_sHP);
	TempBuf.Add(m_sPP);
	TempBuf.Add(m_sSP);

	TempBuf.Add(m_sMagicMaxHP);
	TempBuf.Add(m_sMagicMaxPP);
	TempBuf.Add(m_sMagicMaxSP);

	TempBuf.Add(sMaxDamage);	// ÃÖ´ë °ø°Ý·Â
	TempBuf.Add(sMaxDefense);	// ÃÖ´ë ¹æ¾î·Â

	TempBuf.Add(m_sSkillPoint);

	TempBuf.Add(m_byClass);
	
	for(int i = m_byClass * SKILL_NUM; i < m_byClass * SKILL_NUM + SKILL_NUM; i++)
	{
		TempBuf.Add(m_UserSkill[i].sSid);
		TempBuf.Add(m_UserSkill[i].tLevel);
	}
	TempBuf.Add(m_sSkillPoint_);
	for(int i = m_byClass * 3;i < m_byClass* 3 + 3;i++)
	{
		TempBuf.Add(m_UserSkill_[i].sSid);
		TempBuf.Add(m_UserSkill_[i].tLevel);
	}
	short spellattack = GetUserSpellAttack();
	short spelldefence = GetUserSpellDefence();
	TempBuf.Add(spellattack);
	TempBuf.Add(spelldefence);
	TempBuf.Add(m_sKillCount);
	TempBuf.Add((short)m_dwZaiXianTime);
	TempBuf.Add(m_dwShopPingDN);
	
	GetRecoverySpeed();						// ¾ÆÀÌÅÛÀÌµ¿, ·¹º§¾÷¿¡ÀÇÇÑ ´É·ÂÄ¡º¯µ¿À» È¸º¹¼Óµµ¿¡ ¹°¸°´Ù.
	Send(TempBuf, TempBuf.GetLength());
}
///////////////////////////////////////////////////////////////////
//¼ÆËãÍæ¼Ò×Ô¼ºµÄÄ§·¨¹¥»÷
short USER::GetUserSpellAttack()
{
	int iPsiAttackUp = 0;
	short sBasic = 0;

	if(m_byClass == 1)  //·¨
	{
		if(m_dwBFindTime != 0) sBasic += 20;
		//if(m_sLevel >= 1 && m_sLevel < 140) sBasic += 50;
		//if(m_sLevel >= 140) sBasic += 150;
		//if(m_UserItem[36].sSid == 987) sBasic += 50;//±ØÉ±¼Ó50µã¹¥»÷ MarkJay
		switch(m_dwJiaGongJi)  //Ð¡ºÚÐÞÁ¶
		{
			case 1: sBasic += 16; break;
			case 2: sBasic += 32; break;
			case 3: sBasic += 48; break;
			case 4: sBasic += 64; break;
			case 5: sBasic += 80; break;
			case 6: sBasic += 96; break;
			case 7: sBasic += 112; break;
			case 8: sBasic += 128; break;
			case 9: sBasic += 144; break;
			case 10: sBasic += 160; break;
		}
		if(m_iSkin == 4)
		{
			if(m_iHair == 10000)//»Æ»ú¼×¼ÓÄ§·¨¹¥»÷
			{
				sBasic += 10;
			}
			else if(m_iHair == 10001)//ºì»ú¼×¼ÓÄ§·¨¹¥»÷
			{
				sBasic += 12;
			}
			else if(m_iHair == 10002)//À¶»ú¼×¼ÓÄ§·¨¹¥»÷
			{
				sBasic += 15;
			}
			else if(m_iHair == 10004)//ÂÌ»ú¼×¼ÓÄ§·¨¹¥»÷
			{
				sBasic += 18;
			}
			else if(m_iHair == 10005)//×Ï»ú¼×¼ÓÄ§·¨¹¥»÷
			{
				sBasic += 20;
			}
			else if(m_iHair == 10006)//ºÚ»ú¼×¼ÓÄ§·¨¹¥»÷
			{
				sBasic += 25;
			}
		}
		if(m_iSkin == 7) sBasic += 50;
		if(m_iSkin == 8) sBasic += 20;
		if(sBasic < 0) sBasic = 0;
	}
	
	if(m_UserItem[RIGHT_HAND].tIQ != UNIQUE_ITEM)	
	{
		if(m_UserItem[RIGHT_HAND].sSid >= 0 && m_UserItem[RIGHT_HAND].sSid < g_arItemTable.GetSize())
		{
			if(g_arItemTable[m_UserItem[RIGHT_HAND].sSid]->m_byClass == STAFF_ITEM)
			{
				int iUpCount = m_UserItem[RIGHT_HAND].tMagic[ITEM_UPGRADE_COUNT];
				if(iUpCount > 0 && iUpCount <= MAX_ITEM_UPGRADE_COUNT+1)
				{
					if(m_dwHuanLing > 0)
					{
						if(m_byClass == 1) iUpCount += 1;//»ÃÏëÁéÊ¯
					}
					iPsiAttackUp = (iUpCount * ATTACK_UPGRADE_PSI_BAND);
				}
			}
		}
	}
	iPsiAttackUp += sBasic + m_DynamicUserData[MAGIC_PSI_ATTACK_UP] + (int)((double)m_DynamicEBodyData[EBODY_VOL_TO_PSI_DAMAGE] / 100 * (double)m_sMagicVOL) + m_sMagicVOL/2;
	iPsiAttackUp += (int)((double)iPsiAttackUp * (double)m_DynamicEBodyData[EBODY_PSI_ATTACK_UP] / 100);
	if(m_sLevel >= 70)
	{
		if(m_sLevel - ADD_USER_LEVEL < g_arUserLevel.GetSize())
		{
			iPsiAttackUp += g_arUserLevel[m_sLevel - ADD_USER_LEVEL]->m_sDamage;
		}
	}
	return iPsiAttackUp;
}

///////////////////////////////////////////////////////////////////
//¼ÆËãÍæ¼Ò×Ô¼ºµÄÄ§·¨·ÀÓù
short USER::GetUserSpellDefence()
{
	double iBasic = 0;		//ÖÇ»Û
	iBasic = (int)((double)m_sMagicVOL/2 + 0.5);
	if(iBasic <= 0) iBasic = 0;	
	int sBasic = m_sMagicVOL;//ÖÇ»Û	
	if(m_UserItem[0].tIQ == 12)//Ã±×Ó
	{
		sBasic += m_UserItem[0].tMagic[ITEM_UPGRADE_COUNT]*3;
		if(m_dwHuanJing > 0) sBasic += 3;//»Ã¾§
	}
	if(m_UserItem[1].tIQ == 12)//ÒÂ·þ
	{
		sBasic += m_UserItem[1].tMagic[ITEM_UPGRADE_COUNT]*3;
		if(m_dwHuanJing > 0) sBasic += 3;
	}
	if(m_UserItem[8].tIQ == 12)//¿ã×Ó
	{
		sBasic += m_UserItem[8].tMagic[ITEM_UPGRADE_COUNT]*3;
		if(m_dwHuanJing > 0) sBasic += 3;
	}
	if(m_UserItem[9].tIQ == 12)//Ð¬×Ó
	{
		sBasic += m_UserItem[9].tMagic[ITEM_UPGRADE_COUNT]*3;
		if(m_dwHuanJing > 0) sBasic += 3;
	}
	int dItemResist = sBasic + (int)((double)m_DynamicUserData[MAGIC_PSI_ATTACK_UP] / 2) + m_DynamicUserData[MAGIC_PSI_RESIST_UP] + (int)((double)m_DynamicEBodyData[EBODY_VOL_TO_PSI_RESIST] / 100 * (double)m_sMagicVOL);
    dItemResist += (int)(dItemResist * (double)m_DynamicEBodyData[EBODY_PSI_RESIST_UP] / 100);//»úÐµÔö¼ÓµÄÄ§¿¹
	return dItemResist;
}
///////////////////////////////////////////////////////////////////////////////////
//	ÇØ´ç °è¿­ÀÇ ½ºÅ³ÀÎÁö ÆÇ´Ü.
//
int USER::CheckSkillClass(short sSid)
{
	if(sSid >= 0 && sSid < SKILL_NUM) return BRAWL;
	else if(sSid < (2 * SKILL_NUM)) return STAFF;
	else if(sSid < (3 * SKILL_NUM)) return EDGED;
	else if(sSid < TOTAL_SKILL_NUM) return FIREARMS;
	else return -1;
}
//////////////////////////////////////////////////////////////////////////////////
//	°ø°Ý½ÇÆÐ ÆÐÅ¶À» º¸³½´Ù.
//
void USER::SendAttackFail(BYTE tErrorCode)
{
	CBufferEx TempBuf;
	
	TempBuf.Add(ATTACK_RESULT);
	TempBuf.Add(ATTACK_FAIL);
	TempBuf.Add(tErrorCode);
	
	Send(TempBuf, TempBuf.GetLength());
}

//////////////////////////////////////////////////////////////////////////////////
//	°ø°Ý¹Ì½º ÆÐÅ¶À» º¸³½´Ù.
//
void USER::SendAttackMiss(int tuid)
{
	CBufferEx TempBuf;
	
	TempBuf.Add(ATTACK_RESULT);
	TempBuf.Add(ATTACK_MISS);
	TempBuf.Add((int)(m_uid + USER_BAND));
	TempBuf.Add(tuid);

//	SendInsight(TempBuf, TempBuf.GetLength());
	SendExactScreen(TempBuf, TempBuf.GetLength());
}

///////////////////////////////////////////////////////////////////////////////////
//	°ø°Ý¼º°ø ÆÐÅ¶À» º¸³½´Ù.
//
void USER::SendAttackSuccess(int tuid, BOOL bIsCritical, short sHP, short sMaxHP)//yskang 0.3 //void USER::SendAttackSuccess(int tuid, CByteArray& arAction1, CByteArray& arAction2, short sHP, short sMaxHP)
{
	CBufferEx TempBuf;
//	CByteArray arAction1;
//	CByteArray arAction2;
	
	TempBuf.Add(ATTACK_RESULT);
	//---------------------------------------------------------------
	//yskang 0.3 ÆÐÅ¶À» ÁÙÀÌ±â À§ÇÏ¿© ½ºÅ³¾×¼ÇÀ» ´Ü¼øÇÏ°Ô Àü¼Û
	//ÇÔ¼ö ÀÎÀÚ ÀÚÃ¼¸¦ ¼öÁ¤ÇØ¾ß ÇÒ±î?
	//---------------------------------------------------------------
	if(bIsCritical == FALSE) //Å©¸®Æ¼ÄÃ °ø°ÝÀÌ ¾Æ´Ï³Ä
	{
		TempBuf.Add(ATTACK_SUCCESS);//º¸Åë °ø°Ý
	}
	else
	{
		TempBuf.Add(CRITICAL_ATTACK_SUCCESS); //Å©¸®Æ¼ÄÃ °ø°Ý
	}
	TempBuf.Add((int)(m_uid + USER_BAND));
	TempBuf.Add(tuid);
	//------------------------------------------------------------------

/*	BYTE tAction1 = (BYTE)arAction1.GetSize();
	BYTE tAction2 = (BYTE)arAction2.GetSize();
	int i = 0;

	TempBuf.Add(tAction1);
	if(tAction1 > 0)
	{
		for(i = 0; i < arAction1.GetSize(); i++)
		{
			TempBuf.Add(arAction1[i]);
		}
	}
	TempBuf.Add(tAction2);
	if(tAction2 > 0)
	{
		for(i = 0; i < arAction2.GetSize(); i++)
		{
			TempBuf.Add(arAction2[i]);
		}
	}
*/	
	TempBuf.Add((short)sHP);
	TempBuf.Add((short)sMaxHP);

	// ÃÖ°­ ÀÌº¥Æ®¶§¹®¿¡ ¿î¿µÆÀÀÌ ¿äÃ» 02-11-1 
	if(m_curz == 61 || m_curz == 62 || m_curz == 63) SendInsight(TempBuf, TempBuf.GetLength());
	else SendExactScreen(TempBuf, TempBuf.GetLength());	// ÆÐÅ¶ ºÎÇÏ·®ÀÌ ¸¹¾Æ¼­ °íÄ£°ÍÀÓ 02-10-28 by Youn Gyu
//	SendInsight(TempBuf, TempBuf.GetLength());
}


///////////////////////////////////////////////////////////////////////////////////
//	°°Àº Á¸¾ÈÀÇ À¯Àú¸¦ µµ½Ã¿¡¼­ »ì¸°´Ù
//
BOOL USER::LiveCity(int x, int y, BYTE type)
{
	if( m_ZoneIndex < 0 || m_ZoneIndex >= g_zone.GetSize() ) return FALSE;
	if(m_curx < 0 || m_cury < 0 || m_curx >= g_zone[m_ZoneIndex]->m_sizeMap.cx || m_cury >= g_zone[m_ZoneIndex]->m_sizeMap.cy) return FALSE;
	long lUid = m_uid + USER_BAND;
	if(m_curx < 0 || m_cury < 0) return FALSE;//ÐÞÕý×ø±ê±¨´í
	if(InterlockedCompareExchange((LONG*)&g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser, (long)0, (long)lUid) == (long)lUid)
	{
		m_curx = x;
		m_cury = y;

		::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser, lUid);
		SightRecalc();
		return TRUE;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////////
//	Á×¾ú´ø À¯Àú¸¦ »ì¸°´Ù.
//
void USER::LiveReq(TCHAR *pBuf, BYTE tOption)
{
	// tOption == 1 ÀÌ¸é »ý¸íÀÇ ¹ÝÁö·Î »ì¾Æ³ª´Â °æ¿ìÀÓ
	// ÀÌ°æ¿ì¿¡´Â pBuf °ªÀ» Ã¼Å©ÇÏÁö ¾ÊÅä·Ï ÇÑ´Ù.
	// Áö±Ýµµ pBuf °ªÀº Ã¼Å©ÇÏÁö ¾ÊÁö¸¸ ³ªÁß¿¡ ÀÌ ÇÔ¼ö¸¦ ¼öÁ¤ÇÒ ¶§µµ 
	// ÁÖÀÇÇØ¼­ ¼öÁ¤ÇØ ÁÖµµ·Ï ÇÑ´Ù.

	if(m_bLive == USER_LIVE) return;

	int i;
	int result = 0;
	int iPotalZone = -1;

	int index = 0, rand = 0;

	int x = 0;	
	int y = 0;

	BOOL nResult = FALSE;
	CPoint	ptPotal, pt;	

	if(m_tGuildHouseWar == GUILD_WARRING) CheckGuildUserListInGuildHouseWar(); // ´Ù¸¥ À¯ÀúµéÀº ¹»ÇÏ³ª Ã¼Å©..
/*
	for(i = 0; i < g_TownPotal.GetSize(); i++)
	{
		if(g_TownPotal[i]->iZone == m_curz) { iPotalIndex = i; break; }
	}

	if(iPotalIndex < 0) { nResult = TRUE; goto go_result; }
*/
	pt = GetTownPotal(iPotalZone,tOption);
	x = pt.x;
	y = pt.y;

	if(iPotalZone < 0) 
	{ 
		if( m_curz == 56 || m_curz == 57 || m_curz == 58 || m_curz == 59 ) { nResult = TRUE; goto go_result; }

		x = g_TownPotal[0]->iPotalX;
		y = g_TownPotal[0]->iPotalY;
		iPotalZone = g_TownPotal[0]->iPotalZone;
	}
													// Á¸ Ã¼ÀÎÁö¿¡¼­ ÀÚÃ¼ ÁÂÇ¥¸¦ º¯È¯ÇÑ´Ù.			
//	if(g_TownPotal[iPotalIndex]->iPotalZone != m_curz) ZoneMoveReq(g_TownPotal[iPotalIndex]->iPotalZone, x, y);
	if(iPotalZone != m_curz) ZoneMoveReq(iPotalZone, x, y);
	else if(m_curz == 91 && f_Markini[0]->PK_DSKG == 1) //PK´óÈü
	{
		int randomX = myrand(8,50);
		int randomY = myrand(8,50);
		if((randomX + randomY) %2 == 0 )
		{
			ZoneMoveReq(m_curz, randomX, randomY);
		}
		else
		{
			randomX -= 1;
			ZoneMoveReq(m_curz, randomX, randomY);
		}
	}
	else 
	{	
		ptPotal = ConvertToServer(x, y);			

		i = SEARCH_TOWN_POTAL_COUNT;				// ÁÂÇ¥¸¦ Ã£±âÀ§ÇÑ È½¼ö
		pt = FindNearAvailablePoint_S(ptPotal.x, ptPotal.y);
		if(pt.x == -1 || pt.y == -1)
		{
			while(TRUE)								// Å¸¿îÆ÷Å» ·¥´ý ÁÂÇ¥¸¦ ¾ò´Â´Ù.
			{
				rand = myrand(-TOWN_POTAL_SIZE, TOWN_POTAL_SIZE);
				ptPotal.x += rand; ptPotal.y += rand;

				pt = FindNearAvailablePoint_S(ptPotal.x, ptPotal.y);
				if(pt.x != -1 && pt.y != -1) break;

				i--;
				if(i <= 0) // ³ªÁß¿¡
				{
					pt = ConvertToServer(x, y);
					m_curx = pt.x; m_cury = pt.y;
					nResult = TRUE;
					goto go_result;
				}
			}
		}
		x = pt.x; y = pt.y;
		if(LiveCity(x, y) == FALSE)  { nResult = TRUE; goto go_result; }
	}

go_result:
	m_bLive = USER_LIVE;

	if(m_sMagicMaxHP > m_sMaxHP) m_sHP = m_sMagicMaxHP;
	else m_sHP = m_sMaxHP;
	
//	if(m_tGuildWar == GUILD_WARRING && m_dwFieldWar > 0) EndGuildWar();	// ÇÊµåÀü ¿¡¼­ ÇØ´ç ±æ¸¶ÀÇ ÃÊ±âÈ­°¡ ºüÁö¹Ç·Î ´Ù½Ã ÇÑ´Ù.

	if(nResult)
	{
		SoftClose();
		return;
	}

	InterlockedExchange(&m_lDeadUsed, (LONG)0);
	CPoint pos = ConvertToClient( m_curx, m_cury );

	CBufferEx TempBuf;
	TempBuf.Add(USER_INFO);
	TempBuf.Add(INFO_MODIFY);
	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.AddString(m_strUserID);

	TempBuf.Add((short)pos.x);
	TempBuf.Add((short)pos.y);

	TempBuf.Add(m_iSkin);
	TempBuf.Add(m_iHair);
	TempBuf.Add((BYTE)m_sGender);
	TempBuf.AddData(m_strFace, 10);

	for(i = 0; i < EQUIP_ITEM_NUM; i++) TempBuf.Add(m_UserItem[i].sSid);

	TempBuf.Add(m_sHP);
	TempBuf.Add(m_sMagicMaxHP);

	TempBuf.Add(m_tDir);

//	TempBuf.Add((BYTE)m_strAbnormal[0]);			// Abnormal Info
//	TempBuf.Add(m_tPsiAbnormal);						//&&&&&&&&&&&& Test Code	

	TempBuf.Add(m_dwAbnormalInfo);					// »óÅÂÀÌ»ó Á¤º¸
	TempBuf.Add(m_dwAbnormalInfo_);

	TempBuf.Add(m_sCityRank);
	TempBuf.Add((int)m_dwGuild);					//&&&&&&&&&&&& Test Code
	TempBuf.AddString(m_strGuildName);			// ±æµå ÀÌ¸§À» Ãß°¡
	TempBuf.Add(m_sGuildVersion);

	TempBuf.Add(m_byClass);
	TempBuf.Add((BYTE)m_bPkStatus);

	TempBuf.AddString(m_strLoveName);//yskang 0.1
	
	for( i = TOTAL_INVEN_MAX; i < TOTAL_ITEM_NUM-1; i++) TempBuf.Add(m_UserItem[i].sSid);	// EBody
	if(m_UserItem[TOTAL_ITEM_NUM-1].sSid!=-1&&m_UserItem[TOTAL_ITEM_NUM-1].sDuration!=0){
		TempBuf.Add((BYTE)(m_UserItem[TOTAL_ITEM_NUM-1].tMagic[0]));
		TempBuf.Add((BYTE)0x00);
	}else{
		TempBuf.Add(m_UserItem[TOTAL_ITEM_NUM-1].tMagic[0]);
		TempBuf.Add((BYTE)0xff);
	}
	TempBuf.AddString(m_PersonalShopName);

	SendInsight(TempBuf, TempBuf.GetLength());

	if(m_bNowBuddy == TRUE) SendBuddyUserChange(BUDDY_CHANGE);		// ¹öµðÁßÀÌ¸é ´Ù¸¥ ¹öµð¿ø¿¡°Ô ³¯¸°´Ù.

	//TRACE("%d¹ø À¯Àú »ì¾Æ³²\n", m_uid );
}

/////////////////////////////////////////////////////////////////////////////////////////
// Á×Àº ¹öµð¿øÀ» ºÎÈ°½ÃÅ²´Ù.
//
void USER::RevivalReq( TCHAR *pBuf )
{
	int			i;
	int			nBufferPos;
	short		tSlot;
	BYTE		error;
	USER*		pUser;
	TCHAR		strUserID[31];
	ItemList	ReverseItem;
	CBufferEx	TempBuf, TempYourBuf;
	CPoint		ptRevival, ptClientRevival;


	nBufferPos = 0;
	if(!GetVarString(sizeof(strUserID), (char*)strUserID, pBuf, nBufferPos)) return;

	if(m_bLive == USER_LIVE) return;

	pUser = GetUser(strUserID);	// User Pointer ¸¦ ¾ò´Â´Ù.

	// ºÎÈ°½ÃÄÑÁÖ´Â À¯Àú°¡ Á¸ÀçÇÏÁö ¾ÊÀ½
	if( pUser == NULL )
	{
		error = 1;
		TempBuf.Add( REVIVAL_RESULT );
		TempBuf.Add( error );
		Send( TempBuf, TempBuf.GetLength() );
		return;
	}

	if( !CheckBuddyMember(pUser->m_strUserID) )
	{
		error = 1;
		TempBuf.Add( REVIVAL_RESULT );
		TempBuf.Add( error );
		Send( TempBuf, TempBuf.GetLength() );
		return;
	}

	::ZeroMemory( &ReverseItem, sizeof(ItemList) );
	ReverseItem.sSid = NORMAL_ITEM_REVERSE;
	tSlot = (short)pUser->GetSameItem(ReverseItem, INVENTORY_SLOT);

	//À¯Àú ÀÎº¥¿¡ ¾ÆÀÌÅÛÀÌ ¾ø°Å³ª ³»±¸·ÂÀÌ 0ÀÏ °æ¿ì
	if( tSlot == -1 )
	{
		error = 2;
		TempBuf.Add( REVIVAL_RESULT );
		TempBuf.Add( error );
		Send( TempBuf, TempBuf.GetLength() );
		return;
	}
	else if( pUser->m_UserItem[tSlot].sCount <= 0 )
	{
		error = 2;
		TempBuf.Add( REVIVAL_RESULT );
		TempBuf.Add( error );
		Send( TempBuf, TempBuf.GetLength() );
		return;
	}

	
	////////////////////////////////////////////////////////////////
	// ºÎÈ°À§Ä¡¸¦ Ã£¾Æ¼­ ºÎÈ°½ÃÅ²´Ù.

	if(m_tGuildHouseWar == GUILD_WARRING) CheckGuildUserListInGuildHouseWar(); // ´Ù¸¥ À¯ÀúµéÀº ¹»ÇÏ³ª Ã¼Å©..

	ptRevival = FindNearAvailablePoint_S( m_curx, m_cury );

	// ºÎÈ°ÇÒ À§Ä¡¸¦ ¸øÃ£À¸¸é ¸¶À»¿¡¼­ ºÎÈ°ÇÏµµ·Ï ÇÑ´Ù.
	if( ptRevival.x == -1 || ptRevival.y == -1 )
	{
//		LiveReq( pBuf );
		TownPotal();
		//Close();	// &&& È®ÀÎÇÏÀÚ.(NULL·Î ÇÏ´ÂÁö...)
		return;		
	}

	// ¿©±â¼­ LiveCity ·çÆ¾Àº ÇöÀçÀ§Ä¡¿¡¼­ ºÎÈ°ÇÔ. 
	// ¸¶À»¿¡¼­ ºÎÈ°ÇÏ´Â°ÍÀÌ ¾Æ´Ô.( ÄÚµåÀçÈ°¿ë ^^ )
	if( LiveCity( ptRevival.x, ptRevival.y ) == FALSE )
	{
		error = 3;								// ºÎÈ°ÇÒ Àå¼Ò¸¦ ¸øÃ£À½: ¾Ë¼ö¾ø´Â ¿¡·¯
		TempBuf.Add( REVIVAL_RESULT );
		TempBuf.Add( error );
		Send( TempBuf, TempBuf.GetLength() );
		return;
	}

	m_bLive = USER_LIVE;

	InterlockedExchange(&m_lDeadUsed, (LONG)0);
	ptClientRevival = ConvertToClient( m_curx, m_cury );

	if(m_sMagicMaxHP > m_sMaxHP) m_sHP = m_sMagicMaxHP;
	else m_sHP = m_sMaxHP;
//Å£Å£
	if( m_dwLastDownExp != 0 )
		GetExp( m_dwLastDownExp*50/100 );
		

	error = 0;

	TempBuf.Add( REVIVAL_RESULT );
	TempBuf.Add( error );

	// USER_INFO ¿¡¼­ Mode ¸¦ »« ³ª¸ÓÁö µ¥ÀÌÅÍ ±×´ë·Î Àü¼Û
	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.AddString(m_strUserID);

	TempBuf.Add((short)ptClientRevival.x);
	TempBuf.Add((short)ptClientRevival.y);

	TempBuf.Add(m_iSkin);
	TempBuf.Add(m_iHair);
	TempBuf.Add((BYTE)m_sGender);
	TempBuf.AddData(m_strFace, 10);

	for( i = 0; i < EQUIP_ITEM_NUM; i++ ) TempBuf.Add(m_UserItem[i].sSid);

	TempBuf.Add(m_sHP);
	TempBuf.Add(m_sMagicMaxHP);

	TempBuf.Add(m_tDir);

	TempBuf.Add(m_dwAbnormalInfo);				// »óÅÂÀÌ»ó Á¤º¸
	TempBuf.Add(m_dwAbnormalInfo_);

	TempBuf.Add(m_sCityRank);

	TempBuf.Add((int)m_dwGuild);				//&&&&&&&&&&&& Test Code
	TempBuf.AddString(m_strGuildName);			// ±æµå ÀÌ¸§À» Ãß°¡
	TempBuf.Add(m_sGuildVersion);
	TempBuf.Add(m_byClass);
	TempBuf.Add((BYTE)m_bPkStatus);

	TempBuf.AddString(m_strLoveName);			//yskang 0.1

	for( i = TOTAL_INVEN_MAX; i < TOTAL_ITEM_NUM-1; i++) TempBuf.Add(m_UserItem[i].sSid);	// EBody
	if(m_UserItem[TOTAL_ITEM_NUM-1].sSid != -1 && m_UserItem[TOTAL_ITEM_NUM-1].sDuration != 0)
	{
		TempBuf.Add((BYTE)(m_UserItem[TOTAL_ITEM_NUM-1].tMagic[0]));
		TempBuf.Add((BYTE)0x00);
	}
	else
	{
		TempBuf.Add(m_UserItem[TOTAL_ITEM_NUM-1].tMagic[0]);
		TempBuf.Add((BYTE)0xff);
	}
	TempBuf.AddString(m_PersonalShopName);

	SendInsight(TempBuf, TempBuf.GetLength());

	if(m_bNowBuddy == TRUE) SendBuddyUserChange(BUDDY_CHANGE);		// ¹öµðÁßÀÌ¸é ´Ù¸¥ ¹öµð¿ø¿¡°Ô ³¯¸°´Ù.


	//////////////////////////////////////////////////////////////////////////
	// »ì·ÁÁØ À¯ÀúÀÇ ¸®¹ö½º¸®¿Â ³»±¸µµ¸¦ ±ï°í Á¤º¸¸¦ º¸³½´Ù.

	// ¸®¹ö½º¸®¿ÂÀÇ °¹¼ö¸¦ °¨¼Ò½ÃÅ²´Ù.
	pUser->m_UserItem[tSlot].sCount -= 1;
	if(pUser->m_UserItem[tSlot].sCount <= 0) pUser->m_UserItem[tSlot].sSid = -1;

	// ÇöÀç ¹«°Ô¿¡¼­ ¿ìÇ¥ ÇÑ°³ ¹«°Ô¸¦ °¨¼Ò½ÃÅ²´Ù.
	pUser->m_iCurWeight -= g_arItemTable[NORMAL_ITEM_STAMP]->m_byWeight;
	if( pUser->m_iCurWeight < 0 ) pUser->m_iCurWeight = 0;

	pUser->UpdateUserItemDN();

	error = 1;
	TempYourBuf.Add( ITEM_GIVE_RESULT );
	TempYourBuf.Add( error );
	TempYourBuf.Add( (BYTE)tSlot );
	TempYourBuf.Add( pUser->m_UserItem[tSlot].sLevel );
	TempYourBuf.Add( pUser->m_UserItem[tSlot].sSid );
	TempYourBuf.Add( pUser->m_UserItem[tSlot].sDuration );
	TempYourBuf.Add( pUser->m_UserItem[tSlot].sBullNum );
	TempYourBuf.Add( pUser->m_UserItem[tSlot].sCount );
	for(i = 0; i < MAGIC_NUM; i++) TempYourBuf.Add( pUser->m_UserItem[tSlot].tMagic[i]);
	TempYourBuf.Add( pUser->m_UserItem[tSlot].tIQ );

	pUser->Send(TempYourBuf, TempYourBuf.GetLength());

		//ÔÚBOSS¶´ÖÐÊ¹ÓÃÉñÖ®Àá½«×Ô¶¯»Ø³Ç
	if(m_curz == 409)
	{
		ReturnTown(1, 33);
		return;
	}
}


//######################################################################################
//	Cell Functions
//######################################################################################

////////////////////////////////////////////////////////////////////////////
//	CELL ÀÇ À§Ä¡ index setting
//
/*
BOOL USER::SetCellIndex(int zone, int xpos, int ypos)
{
	CPoint pt;

	for(int i = 0; i < g_cell.GetSize(); i++)
	{
		if( g_cell[i]->m_Zone == zone )
		{
			pt = g_cell[i]->GetCellIndex(xpos, ypos);
			if(pt.x < 0 || pt.y < 0) return FALSE;
			
			m_ptCell = pt;
			m_nCellZone = i;

			break;
		}
	}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////
//	Cell ¿¡ À¯Àú¸¦ µî·ÏÇÑ´Ù.
//
void USER::AddCellUid()
{
	g_cell[m_nCellZone]->AddCell(m_ptCell, m_uid + USER_BAND);
}

////////////////////////////////////////////////////////////////////////////
//	Cell ¿¡¼­ À¯Àú¸¦ »èÁ¦ÇÑ´Ù.
//
void USER::DeleteCellUid()
{
	g_cell[m_nCellZone]->DeleteCell(m_ptCell, m_uid + USER_BAND);
}

////////////////////////////////////////////////////////////////////////////
//	Cell ÀÌ º¯°æµÆÀ» °æ¿ìÀÇ Ã³¸®
//
void USER::ChangeCellUid(CPoint ptOld, CPoint ptNew)
{
	g_cell[m_nCellZone]->ChangeCell(ptOld, ptNew, m_uid + USER_BAND);
}
*/
//-------------------------------------------------------------------------------//

/////////////////////////////////////////////////////////////////////////
//	½ºÅ³Å×ÀÌºí¿¡¼­ ÇØ´ç ½ºÅ³¾ÆÀÌµð¿¡´ëÇÑ ½ºÅ³¸íÀ» ¾ò¾î¿Â´Ù.
//
BOOL USER::GetSkillName(int nSid, CString &strName)
{
/*	int nSize = g_arSkillTable.GetSize();

	if(nSid > nSize || nSid < 0) return FALSE;

	strName = g_arSkillTable[nSid]->m_strName;
*/
	return TRUE;
}

////////////////////////////////////////////////////////////////////////
//	½ºÅ³¹öÆÛ¿¡¼­ ¼Ò¼Ó Å¬·¡½ºÀÇ ½ºÅ³¸í°ú ·¹º§À» ¾ò¾î¿Â´Ù.
//
BOOL USER::GetSkillNameLevel(BYTE byClass, TCHAR* pSkill, TCHAR* pBuf)
{
	if(pSkill == NULL) return FALSE;

	SkillList UserSkill[TOTAL_SKILL_NUM];

	int i, index = 0;
	for(i = 0; i < TOTAL_SKILL_NUM; i++)
	{
		UserSkill[i].sSid		= GetShort(pSkill, index);
		UserSkill[i].tLevel		= GetByte(pSkill, index);
	}

	index = 0;
	for(i = 0; i < TOTAL_SKILL_NUM; i++)
	{
		SetShort(pBuf,UserSkill[i].sSid, index); 
		SetByte(pBuf, UserSkill[i].tLevel, index);
	}

//int nOffset = byClass * 5;
//	short sSid;
//	CString strSkillName;
/*	TCHAR	szTemp[SKILL_NAME_LENGTH];

	index = 0;
	for(i = 0; i < SKILL_NUM; i++)
	{
		sSid = UserSkill[nOffset + i].sSid;
		
//		if(!GetSkillName(sSid, strSkillName))return FALSE;

		SpaceCopy(szTemp, strSkillName, SKILL_NAME_LENGTH);

		SetString(pBuf, szTemp, SKILL_NAME_LENGTH, index);
		SetByte(pBuf, UserSkill[nOffset + i].tLevel, index);
	}
*/
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////
//	ItemTable ¿¡¼­ ÇØ´ç ¾ÆÀÌÅÛ¾ÆÀÌµð¿¡ ´ëÇÑ ±×¸²¹øÈ£¸¦ °¡Á®¿Â´Ù.
//
BOOL USER::GetEquipItemPid(TCHAR *pItemBuf, TCHAR *pBuf)
{
	if(pItemBuf == NULL) return FALSE;

	int nSize = g_arItemTable.GetSize();
	if(EQUIP_ITEM_NUM < 0 || EQUIP_ITEM_NUM >= nSize) return FALSE;

	ItemList UserItem[EQUIP_ITEM_NUM];

	int index = 0;
	int i, j;
	for(i = 0; i < EQUIP_ITEM_NUM; i++)
	{
		UserItem[i].sLevel		= GetShort(pItemBuf, index);
		UserItem[i].sSid		= GetShort(pItemBuf, index);
		UserItem[i].sDuration	= GetShort(pItemBuf, index);
		UserItem[i].sBullNum	= GetShort(pItemBuf, index);
		UserItem[i].sCount		= GetShort(pItemBuf, index);

		for(j = 0; j < MAGIC_NUM; j++) UserItem[i].tMagic[j] = GetByte(pItemBuf, index);

		UserItem[i].tIQ			= GetByte(pItemBuf, index);

		// Å¬¶óÀÌ¾ðÆ®¿¡ º¸³»´Â ÀåÂø Á¤º¸¸¦ ¸¸µé¶§ ½Ã¸®¾ó¹øÈ£´Â º¸³»Áö ¾ÊÀ¸¹Ç·Î ÀÐ¾î¼­ ¹ö¸°´Ù.
		for( j = 0; j < 8; j++ )
		{
			GetByte( pItemBuf, index );
		}
	}	
	
	short sSid = 0;
	short sPid = 0;

	index = 0;
	for(i = 0; i < EQUIP_ITEM_NUM; i++)
	{
		sSid = UserItem[i].sSid;
		if(sSid >= nSize) return FALSE;

		if(sSid != -1)
		{
			sPid = sSid;//g_arItemTable[sSid]->m_sPid;
		}
		else sPid = -1;
		
		SetShort(pBuf, sPid, index);
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////
//	ºñ¾îÀÖ´Â ½ºÅ³Á¤º¸¸¦ ¸¸µç´Ù.
//
void USER::MakeEmptySkill(TCHAR *pBuf, BYTE tClass)
{
	int index = 0;
	int i;

	for(i = 0; i < CLASS_NUM * SKILL_NUM; i++)
	{	
			SetShort(pBuf, i, index);				// sid
			SetByte(pBuf, 0, index);				// skill level
/*
		if(i == (tClass * 5))						// ÁÖ°è¿­ÀÇ ¸íÁß/È¸ÇÇ½ºÅ³ ÇÏ³ª´Â ±âº»ÀûÀ¸·Î ÁØ´Ù.
			SetByte(pBuf, 1, index);				// skill level
		else 
			SetByte(pBuf, 0, index);		

		SetByte(pBuf, 0 , index);					// OnOff
*/	}
}

//////////////////////////////////////////////////////////////////////////
//	ºñ¾îÀÖ´Â ¾ÆÀÌÅÛ Á¤º¸¸¦ ¸¸µç´Ù.(»õ·Î¿î Ä³¸¯À» ¸¸µé¶§ ±âº» ¹«±â¸¦ ÁÖ±âÀ§ÇØ)
//
void USER::MakeInintItem(TCHAR *pBuf, BYTE tClass)
{
	int index = 0;
	int i, j;
	int sSid = 0, sSid2 = 0, sSid3 = 0, sSid4 = 0, sSubSid = 0;

	InitUserItem();

	for(i = 0; i < g_arInitItem.GetSize(); i++)
	{
		if(g_arInitItem[i] == NULL) continue;//Îª¿ÕÔòµ¯Æð MarkJay
		if(tClass == g_arInitItem[i]->tClass) 
		{ 
			sSid = g_arInitItem[i]->iSid; 
			sSubSid = g_arInitItem[i]->iSubSid;
			break; 
		}
	}

	if(g_arItemTable[sSid] == NULL) return;//Îª¿ÕÔò·µ»Ø MarkJay

	m_UserItem[EQUIP_ITEM_NUM].sLevel = g_arItemTable[sSid]->m_byRLevel;
	m_UserItem[EQUIP_ITEM_NUM].sSid = g_arItemTable[sSid]->m_sSid;
	m_UserItem[EQUIP_ITEM_NUM].sDuration = g_arItemTable[sSid]->m_sDuration;
	m_UserItem[EQUIP_ITEM_NUM].sBullNum = g_arItemTable[sSid]->m_sBullNum;
	m_UserItem[EQUIP_ITEM_NUM].sCount = 1;
	for(j = 0; j < MAGIC_NUM; j++) m_UserItem[EQUIP_ITEM_NUM].tMagic[j] = 0;
	m_UserItem[EQUIP_ITEM_NUM].tIQ = 0;
	m_UserItem[EQUIP_ITEM_NUM].iItemSerial = 0;

	switch(tClass)
	{
	case EDGED:
		m_UserItem[EQUIP_ITEM_NUM].sBullNum = g_arItemTable[sSubSid]->m_sBullNum;
	break;

	case FIREARMS:
		m_UserItem[EQUIP_ITEM_NUM].sBullNum = g_arItemTable[sSubSid]->m_sBullNum;
/*		m_UserItem[EQUIP_ITEM_NUM + 1].sLevel = g_arItemTable[sSid]->m_byRLevel;
		m_UserItem[EQUIP_ITEM_NUM + 1].sSid = g_arItemTable[sSubSid]->m_sSid;
		m_UserItem[EQUIP_ITEM_NUM + 1].sDuration = g_arItemTable[sSubSid]->m_sDuration;
		m_UserItem[EQUIP_ITEM_NUM + 1].sBullNum = g_arItemTable[sSubSid]->m_sBullNum;
		m_UserItem[EQUIP_ITEM_NUM + 1].sCount = 1;
		for(j = 0; j < MAGIC_NUM; j++) m_UserItem[EQUIP_ITEM_NUM  +1].tMagic[j] = 0;
		m_UserItem[EQUIP_ITEM_NUM + 1].tIQ = 0;
*/		break;
	}

	for(i = 0; i < TOTAL_ITEM_NUM; i++)
	{	
		SetShort(pBuf, 0, index);		// Level
		SetShort(pBuf, -1, index);		// sid
		SetShort(pBuf, 0, index);		// BullNum
		SetShort(pBuf, 0, index);		// Count
		SetShort(pBuf, 0, index);		// Duration

		for(j = 0; j < MAGIC_NUM; j++) 
			SetByte(pBuf, 0, index);	// Magic Attribute

		SetByte(pBuf, 0, index);		// Item Quality

		for(j = 0; j < 8; j++ )			// Item Serial
		{
			SetByte(pBuf, 0, index );
		}
	}

	UserItemToStr(pBuf);
}

//////////////////////////////////////////////////////////////////////////
//	ºñ¾îÀÖ´Â »çÀÌ¿À´Ð Á¤º¸¸¦ ¸¸µç´Ù.
//
void USER::MakeEmptyPsi(TCHAR *pBuf)
{
	int index = 0;

	SetShort(pBuf, -1, index);		// sid
									// »çÀÌ¿À´Ð ¹öÆÛ´Â -1¸¦ ¸¸³¯¶§±îÁö ÀÐ´Â´Ù. ³ª¸ÓÁö´Â ¸ðµÎ 0·Î ÃÊ±âÈ­
	SetByte(pBuf, 0 , index);		// OnOff
}

//////////////////////////////////////////////////////////////////////////
//	PA or MA Point ¸¦ ¾ò±â À§ÇÑ ´ÙÀ½ °æÇèÄ¡¸¦ ±¸ÇÑ´Ù.
//
DWORD USER::GetNextPAMAExp(DWORD dwExp)
{
	int i;
	int nSize = g_arPAMAExp.GetSize();

	for(i = 0; i < nSize; i++)
	{
		if(g_arPAMAExp[i]->m_dwPAMAExp > dwExp) return g_arPAMAExp[i]->m_dwPAMAExp;
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
//	´ÙÀ½ ·¹º§ÀÌ µÇ±â À§ÇÑ °æÇèÄ¡¸¦ ±¸ÇÑ´Ù.
//
DWORD USER::GetNextLevel(int iCurrLevel)
{
	int nSize = g_arLevelUpTable.GetSize();

	if(iCurrLevel >= nSize) return 0;

	return (DWORD)(g_arLevelUpTable[iCurrLevel]->m_dwExp);
}

////////////////////////////////////////////////////////////////////////////
//	ÇØ´ç ½ºÅ³ÀÇ Á¤º¸¸¦ ±¸ÇÑ´Ù. (ÇöÀç´Â ¼º°øÀ²¸¸ ±¸ÇÑ´Ù.)
//
void USER::GetSkillInfo(int sid, BYTE& tRate)
{
/*	int i = 0;
	int nOnSkill = 0;

	if(m_UserSkill[sid].tOnOff == 0)
	{
		tRate = 0;
	}
	else
	{
		for(i = 0; i < SKILL_NUM; i++)
		{
			if(m_UserSkill[m_byClass * SKILL_NUM + i].tOnOff == 1) nOnSkill++;
		}

		tRate = (BYTE)((float)m_sWIS * 0.5 + m_UserSkill[sid].tLevel * 2 - nOnSkill * 25 + 50);
	}
*/
}

////////////////////////////////////////////////////////////////////////////
//	ÇØ´ç ½ºÅ³ÀÇ ¼º°øÀ²À» ±¸ÇÑ´Ù. (º¯°æµÊ. ÇöÀç ¾È¾¸)
//
BYTE USER::GetSkillRate(BYTE byClass, int sid, int nDefusion)
{
	int nSelect = 0;
/*
	for(int i = 0; i < SKILL_NUM; i++)
	{
		if(m_UserSkill[byClass * SKILL_NUM + i].tOnOff == 1) nSelect++;
	}
*/
//!	BYTE byRate = (BYTE)((double)m_sWIS * 0.3 + (double)m_UserSkill[sid].sLevel * 3.5 - nSelect * 25 + 40 - nDefusion);
	BYTE byRate = 1;

	return byRate;
}

////////////////////////////////////////////////////////////////////////////
//	Trade Mode ¸¦ On/Off ÇÑ´Ù
//
void USER::SetTradeMode(TCHAR *pBuf)
{
/*	int index = 0;
	BOOL bMode = (BOOL)GetByte(pBuf, index);

	m_bTradeMode = bMode;

	CBufferEx TempBuf;

	TempBuf.Add(TRADE_MODE_RESULT);
	TempBuf.Add((BYTE)m_bTradeMode);

	Send(TempBuf, TempBuf.GetLength());
*/
}

////////////////////////////////////////////////////////////////////////////
//	ÎïÆ·ÒÆ¶¯
void USER::ItemMoveReq(TCHAR *pBuf)
{
#ifdef _ACTIVE_USER
//	if(m_iDisplayType == 6 && m_sLevel > 25) return;//yskang 0.5
	if(m_iDisplayType == 6) return; //yskang 0.5
#endif
	int	index = 0;
	int i;
	short sOldSid = 0, sNewSid = 0;
	int nOldPos = 0, nNewPos = 0;
	BYTE byOldWear = 255, byNewWear = 255;

	BYTE tType = GetByte(pBuf, index);			// ÀÏ¹Ý ¾ÆÀÌÅÛ°ú Äü ¾ÆÀÌÅÛ°úÀÇ ±¸ºÐ

	if(tType == 1 || tType == 2) return;		// Äü¾ÆÀÌÅÛ ÂÊÀº Ã³¸®ÇÏÁö ¾Ê´Â´Ù
	
	BYTE byOldSlot = GetByte(pBuf, index);		// ¿ø·¡¾ÆÀÌÅÛ À§Ä¡
	BYTE byNewSlot = GetByte(pBuf, index);		// ¿Å±â·Á´Â ¾ÆÀÌÅÛ À§Ä¡

	if(byOldSlot < 0 || byOldSlot >= TOTAL_ITEM_NUM || byNewSlot < 0 || byNewSlot >= TOTAL_ITEM_NUM) 
	{
		SendItemMoveFail();
		return;
	}
	
	if(m_bNowTrading == TRUE || m_bPShopOpen == TRUE)//ÐÞ¸´ÎïÆ·ÒÆ¶¯¸´ÖÆBUG
	{
		SendItemMoveFail();
		return;
	}

	if(byOldSlot < 10 || byOldSlot > 24)
	{
		if(!m_MItemLock && f_Markini[0]->JYMM_KG == 1)
		{
			SendSysWhisperChat("¶Ô²»Æð!½»Ò×ÃÜÂëÉÐÎ´½â³ý,ÎÞ·¨ÒÆ¶¯ÎïÆ·.");
			SendItemMoveFail();
			return;
		}
	}

	if(byNewSlot < 10 || byNewSlot > 24)
	{
		if(!m_MItemLock && f_Markini[0]->JYMM_KG == 1)
		{
			SendSysWhisperChat("¶Ô²»Æð!½»Ò×ÃÜÂëÉÐÎ´½â³ý,ÎÞ·¨ÒÆ¶¯ÎïÆ·.");
			SendItemMoveFail();
			return;
		}
	}

	sOldSid = m_UserItem[byOldSlot].sSid;
	sNewSid = m_UserItem[byNewSlot].sSid;
	
	if(sOldSid <= -1)							// ¼Ò½º°¡ ºñ¾úÀ¸¸é ½ÇÆÐ
	{
		SendItemMoveFail();
		return;
	}
	
	if(sOldSid >= 0 && sOldSid < g_arItemTable.GetSize()) 
	{
		byOldWear = g_arItemTable[sOldSid]->m_byWear;
	}
	
	if(sNewSid >= 0 && sNewSid < g_arItemTable.GetSize()) 
	{
		byNewWear = g_arItemTable[sNewSid]->m_byWear;
	}
	
	nOldPos = GetSlotClass(byOldSlot);
	nNewPos = GetSlotClass(byNewSlot);
	
	if(nOldPos == -1 || nNewPos == -1)			// ÇØ´çÇÏ´Â ½½·ÔÀÌ ¾ø´Â °æ¿ì
	{
		SendItemMoveFail();
		return;
	}

	short	sChangeSlot[3];
	for(i = 0; i < 3; i++)
	{
		sChangeSlot[i] = 255;
	}

	ItemList	TempItem;
	BOOL		bTZItembool = FALSE;//Ì××°×°±¸ÌáÊ¾
	BOOL		bChangeEquip = FALSE;
	int			iSearchSlot = -1;
	int			iSameSlot = -1;
	BOOL		bEmpty01 = FALSE, bEmpty02 = FALSE;
	CPoint		ptEquipPos(-1, -1);

	int			iItemIndex = 0, iMagicIndex = 0;

	int			iEBodyPos = -1;
	
	switch (nOldPos)
	{
	case 0:						// EQUIP
		switch(nNewPos)
		{
		case 0:					// E -> E
			if(byOldWear != byNewWear)	// °°Àº Âø¿ëºÎÀ§ÀÇ ¾ÆÀÌÅÛÀÌ ¾Æ´Ï¸é
			{
				if(byOldWear != 1 && byOldWear != 8)
				{
					SendItemMoveFail();
					return;
				}
				if(
					(byOldSlot == 4 && byNewSlot != 5) ||
				    (byOldSlot == 5 && byNewSlot != 4) ||
				    (byOldSlot == 6 && byNewSlot != 7) ||
				    (byOldSlot == 7 && byNewSlot != 6)	)
				{
					SendItemMoveFail();
					return;
				}
			}
		
			iItemIndex = GetItemIndex(m_UserItem[byOldSlot].sSid);
			if(iItemIndex < 0 || iItemIndex >= g_arItemTable.GetSize()) { SendItemMoveFail(); return; }
			m_UserItem[byOldSlot].sLevel = g_arItemTable[iItemIndex]->m_byRLevel;

			if(m_UserItem[byOldSlot].tIQ > NORMAL_ITEM)
			{
				if(m_UserItem[byOldSlot].tIQ == UNIQUE_ITEM) iItemIndex = MAGIC_NUM;	// ÃÖ´ë °Ë»ö È½¼ö
				else iItemIndex = 4;								// À¯´ÏÅ©¿Í ¼ÂÆ® ¾ÆÀÌÅÛÀ» »« ¾ÆÀÌÅÛÀº ÃÖ´ë 4°³±îÁö¸¸ ½½·Ô »ç¿ë

				for(i = 0; i < iItemIndex; i++)
				{
					iMagicIndex = m_UserItem[byOldSlot].tMagic[i];
					if(iMagicIndex >= 0 && iMagicIndex < g_arMagicItemTable.GetSize())
					{
						m_UserItem[byOldSlot].sLevel += g_arMagicItemTable[iMagicIndex]->m_tLevel;
					}
				}
			}

			if(m_UserItem[byOldSlot].sLevel > m_sLevel) { SendItemMoveFail(); return; }

			if(!IsCanEquipItem((int)byOldSlot)) { SendItemMoveFail(); return; }

			TempItem = m_UserItem[byOldSlot];
			m_UserItem[byOldSlot] = m_UserItem[byNewSlot];
			m_UserItem[byNewSlot] = TempItem;

			sChangeSlot[0] = byOldSlot;			
			sChangeSlot[1] = byNewSlot;

			if(byOldWear == 1 || byNewWear == 1) bChangeEquip = TRUE;
			break;
		case 1:		// E -> I
			if(byOldWear >= 9 || m_UserItem[byOldSlot].sCount > 1)		// if( count > 1 ) FAIL;
			{
				m_UserItem[byOldSlot].sCount = 1;
				SendItemMoveFail();
				return;
			}

			if(byOldWear >= 6)							// ³»±¸µµ°¡ ¾ø´Â °æ¿ì ÀÎº¥¿¡ °ãÄ¥¼ö ÀÖ´Ù.	
			{
				iSameSlot = GetSameItem(m_UserItem[byOldSlot], INVENTORY_SLOT);	 
				if(iSameSlot != -1)						// ÀÎº¥Åä¸®¿¡ °°Àº ¾ÆÀÌÅÛÀÌ Á¸ÀçÇÏ¸é
				{
					if(m_UserItem[iSameSlot].sCount <= 0 || m_UserItem[iSameSlot].sCount >= _MAX_SHORT) { SendItemMoveFail(); return; }

					ReSetItemSlot(&m_UserItem[byOldSlot]);
					CheckMaxValue((short &)m_UserItem[iSameSlot].sCount, (short)1);

					sChangeSlot[0] = byOldSlot;			
					sChangeSlot[1] = iSameSlot;
					if(byOldWear >=1 && byOldWear <= 8) bChangeEquip = TRUE;
					break;
				}
			}
														// À§ Á¶°ÇÀ» Á¦¿ÜÇÏ¸é ¹«Á¶°Ç ºó ½½·Ô¿¡ ÇÏ³ª¾¿ ´ëÀÔ.
			if(m_UserItem[byNewSlot].sSid == -1) iSearchSlot = byNewSlot;	
			else	iSearchSlot = GetEmptySlot(INVENTORY_SLOT);

			if(iSearchSlot == -1)						// À¯Àú°¡ ¿äÃ»ÇÑ ÁöÁ¡ÀÌ ºñ¾úÀ¸¸é..
			{
				SendItemMoveFail();
				return;
			}

			m_UserItem[iSearchSlot] = m_UserItem[byOldSlot];
//			ReSetItemSlot(byOldSlot);
			ReSetItemSlot(&m_UserItem[byOldSlot]);

			sChangeSlot[0] = byOldSlot;			
			sChangeSlot[1] = iSearchSlot;
			if(byOldWear >=1 && byOldWear <= 8) bChangeEquip = TRUE;
			break;
		case 2:		// E -> EBody
			SendItemMoveFail();
			return;
		default:
			SendItemMoveFail();
			return;
		};
		break;
	case 1:			//INVENTORY
		switch(nNewPos)
		{
		case 0:		// I -> E
			if(byOldWear >= 1 && byOldWear <= 9) bTZItembool = TRUE;//´øÌ××°×°±¸Ê± MarkJay
			if(byOldWear >= 1 && byOldWear <= 8)	// ÀåºñÃ¢À¸·Î ¿Å±æ ¼ö ÀÖ´Â ¾ÆÀÌÅÛÀÌ¸é
			{
				if(!IsCanEquipItem((int)byOldSlot)) 
				{ 
					SendItemMoveFail(); 
					return; 
				}
				if(m_UserItem[byOldSlot].sSid == 1121)//´©´÷¸±ÊÖ Ð¡ºÚ¸±ÊÖ
	            {
	                if(m_UserItem[RIGHT_HAND].sSid == -1)
		            {
		               SendSystemMsg("ÇëÏÈ×°ÖÃÎäÆ÷ºó²Å¿É×°ÖÃ¶ñÄ§Ö®ÊÖ!", SYSTEM_ERROR, TO_ME);
                       SendItemMoveFail(); 
					   return; 
		            } 
					if(m_UserItem[LEFT_HAND].sSid != -1)
					{
		               SendSystemMsg("ÇëÐ¶ÏÂ¸±ÎäÆ÷ºó²Å¿É×°ÖÃ¶ñÄ§Ö®ÊÖ!", SYSTEM_ERROR, TO_ME);
                       SendItemMoveFail(); 
					   return; 
		            } 
	            } 	            

				if(m_UserItem[byOldSlot].sCount > 1)	// ¼Ò½º°¡ 1°³¸¦ ÃÊ°úÇÏ¸é
				{
					bEmpty01 = bEmpty02 = FALSE;

					GetEquipPos(byOldWear, ptEquipPos);

					if(ptEquipPos.x != -1)
					{
						if(m_UserItem[ptEquipPos.x].sSid == -1) bEmpty01 = TRUE;
					}
					if(ptEquipPos.y != -1)
					{
						if(m_UserItem[ptEquipPos.y].sSid == -1) bEmpty02 = TRUE;
					}

					iItemIndex = GetItemIndex(m_UserItem[byOldSlot].sSid);
					if(iItemIndex < 0) { SendItemMoveFail(); return; }
					m_UserItem[byOldSlot].sLevel = g_arItemTable[iItemIndex]->m_byRLevel;

					if(m_UserItem[byOldSlot].tIQ > NORMAL_ITEM)
					{
						if(m_UserItem[byOldSlot].tIQ == UNIQUE_ITEM) iItemIndex = MAGIC_NUM;	// ÃÖ´ë °Ë»ö È½¼ö
						else iItemIndex = 4;								// À¯´ÏÅ©¿Í ¼ÂÆ® ¾ÆÀÌÅÛÀ» »« ¾ÆÀÌÅÛÀº ÃÖ´ë 4°³±îÁö¸¸ ½½·Ô »ç¿ë

						for(i = 0; i < iItemIndex; i++)
						{
							iMagicIndex = m_UserItem[byOldSlot].tMagic[i];
							if(iMagicIndex >= 0 && iMagicIndex < g_arMagicItemTable.GetSize())
							{
								m_UserItem[byOldSlot].sLevel += g_arMagicItemTable[iMagicIndex]->m_tLevel;
							}
						}
					}

					if(m_UserItem[byOldSlot].sLevel > m_sLevel) { SendItemMoveFail(); return; }

					if(bEmpty01)
					{
						m_UserItem[ptEquipPos.x] = m_UserItem[byOldSlot];
						m_UserItem[ptEquipPos.x].sCount = 1;

						m_UserItem[byOldSlot].sCount--;
						
						sChangeSlot[0] = byOldSlot;			
						sChangeSlot[1] = (BYTE)ptEquipPos.x;
						
						if(byOldWear >=1 && byOldWear <= 8) bChangeEquip = TRUE;
						break;
					}

					if(bEmpty02)
					{
						m_UserItem[ptEquipPos.y] = m_UserItem[byOldSlot];
						m_UserItem[ptEquipPos.y].sCount = 1;

						m_UserItem[byOldSlot].sCount--;
						
						sChangeSlot[0] = byOldSlot;			
						sChangeSlot[1] = (BYTE)ptEquipPos.y;
						
						if(byOldWear >=1 && byOldWear <= 8) bChangeEquip = TRUE;
						break;
					}

					if(byNewSlot == (BYTE)ptEquipPos.y) ptEquipPos.x = ptEquipPos.y;
					iSearchSlot = GetEmptySlot(INVENTORY_SLOT);
					iSameSlot = GetSameItem(m_UserItem[ptEquipPos.x], INVENTORY_SLOT);

					if(iSearchSlot == -1 && iSameSlot == -1) 
					{
						SendItemMoveFail();
						return;
					}
					
					if(iSameSlot != -1)
					{
						m_UserItem[iSameSlot].sCount++;

						m_UserItem[ptEquipPos.x] = m_UserItem[byOldSlot];
						m_UserItem[ptEquipPos.x].sCount = 1;

						m_UserItem[byOldSlot].sCount--;

						sChangeSlot[0] = byOldSlot;			
						sChangeSlot[1] = (BYTE)iSameSlot;
						sChangeSlot[2] = (BYTE)ptEquipPos.x;
						
						if(byOldWear >=1 && byOldWear <= 8) bChangeEquip = TRUE;
						break;
					}
					
					if(iSearchSlot != -1)
					{
						m_UserItem[iSearchSlot] = m_UserItem[ptEquipPos.x];
						
						m_UserItem[ptEquipPos.x] = m_UserItem[byOldSlot];
						m_UserItem[ptEquipPos.x].sCount = 1;
						
						m_UserItem[byOldSlot].sCount--;
						
						sChangeSlot[0] = byOldSlot;			
						sChangeSlot[1] = (BYTE)iSearchSlot;
						sChangeSlot[2] = (BYTE)ptEquipPos.x;
						
						if(byOldWear >=1 && byOldWear <= 8) bChangeEquip = TRUE;
						break;
					}
				}
				else									// ¼Ò½º°¡ 1°³¸é
				{
					bEmpty01 = bEmpty02 = FALSE;

					GetEquipPos(byOldWear, ptEquipPos);

					if(ptEquipPos.x != -1)
					{
						if(m_UserItem[ptEquipPos.x].sSid == -1) bEmpty01 = TRUE;
					}
					if(ptEquipPos.y != -1)
					{
						if(m_UserItem[ptEquipPos.y].sSid == -1) bEmpty02 = TRUE;
					}

					iItemIndex = GetItemIndex(m_UserItem[byOldSlot].sSid);
					if(iItemIndex < 0) { SendItemMoveFail(); return; }
					m_UserItem[byOldSlot].sLevel = g_arItemTable[iItemIndex]->m_byRLevel;

					if(m_UserItem[byOldSlot].tIQ > NORMAL_ITEM)
					{
						if(m_UserItem[byOldSlot].tIQ == UNIQUE_ITEM) iItemIndex = MAGIC_NUM;	// ÃÖ´ë °Ë»ö È½¼ö
						else iItemIndex = 4;								// À¯´ÏÅ©¿Í ¼ÂÆ® ¾ÆÀÌÅÛÀ» »« ¾ÆÀÌÅÛÀº ÃÖ´ë 4°³±îÁö¸¸ ½½·Ô »ç¿ë

						for(i = 0; i < iItemIndex; i++)
						{
							iMagicIndex = m_UserItem[byOldSlot].tMagic[i];
							if(iMagicIndex >= 0 && iMagicIndex < g_arMagicItemTable.GetSize())
							{
								m_UserItem[byOldSlot].sLevel += g_arMagicItemTable[iMagicIndex]->m_tLevel;
							}
						}
					}

					if(m_UserItem[byOldSlot].sLevel > m_sLevel) { SendItemMoveFail(); return; }

					if(bEmpty01)
					{
						TempItem = m_UserItem[byOldSlot];
						m_UserItem[byOldSlot] = m_UserItem[ptEquipPos.x];
						m_UserItem[ptEquipPos.x] = TempItem;
						
						sChangeSlot[0] = byOldSlot;			
						sChangeSlot[1] = (BYTE)ptEquipPos.x;
						
						if(byOldWear >=1 && byOldWear <= 8) bChangeEquip = TRUE;
						break;
					}
					if(bEmpty02)
					{
						TempItem = m_UserItem[byOldSlot];
						m_UserItem[byOldSlot] = m_UserItem[ptEquipPos.y];
						m_UserItem[ptEquipPos.y] = TempItem;
						
						sChangeSlot[0] = byOldSlot;			
						sChangeSlot[1] = (BYTE)ptEquipPos.y;
						
						if(byOldWear >=1 && byOldWear <= 8) bChangeEquip = TRUE;
						break;
					}

					TempItem = m_UserItem[byOldSlot];
					m_UserItem[byOldSlot] = m_UserItem[ptEquipPos.x];
					m_UserItem[ptEquipPos.x] = TempItem;
					
					sChangeSlot[0] = byOldSlot;			
					sChangeSlot[1] = (BYTE)ptEquipPos.x;
					
					if(byOldWear >=1 && byOldWear <= 8) bChangeEquip = TRUE;
				}
			}
			else									// ÀåºñÃ¢À¸·Î ¿Å±æ ¼ö ¾ø´Â ¾ÆÀÌÅÛÀÌ¸é
			{
				SendItemMoveFail();
				return;
			}
			break;
		case 1:		// I -> I
			TempItem = m_UserItem[byOldSlot];
			m_UserItem[byOldSlot] = m_UserItem[byNewSlot];
			m_UserItem[byNewSlot] = TempItem;
			
			sChangeSlot[0] = byOldSlot;			
			sChangeSlot[1] = byNewSlot;		
			break;
		case 2:		// I -> EBody (ÀÎº¥¿¡¼­ EBody ½½·ÔÀ¸·Î ·Î ¿Å±â±â)
			if(m_iDisplayType == 5)
			{
				SendSystemMsg("¹«·á »ç¿ëÀÚ´Â EBody¸¦ »ç¿ëÇÒ ¼ö ¾ø½À´Ï´Ù.", SYSTEM_NORMAL, TO_ME);
				SendItemMoveFail();
				return;
			}
			if(f_Markini[0]->JXRW_KG == 1)//»úÐµÈÎÎñ¿ª¹Ø
			{
				if((byOldWear!=130) && FindEvent(16) == FALSE) //×ö»úÐµÈÎÎñ
				{
					SendSystemMsg(IDS_NEED_QUEST, SYSTEM_ERROR, TO_ME);// "ÄãÐèÒªÏÈÍê³ÉÈÎÎñ."
					SendItemMoveFail();
					return;
				}
			}/*
			else
			{
				if((byOldWear!=130) && FindEvent(16) == TRUE) //²»×ö»úÐµÈÎÎñ
				{
					SendSystemMsg(IDS_NEED_QUEST, SYSTEM_ERROR, TO_ME);// "ÄãÐèÒªÏÈÍê³ÉÈÎÎñ."
					SendItemMoveFail();
					return;
				}
			}*/
			//»úÐµºÍÊØ»¤
			if((byOldWear >= 122 && byOldWear <= 125) || (byOldWear == 130))	// EBody ·Î ¿Å±æ ¼ö ÀÖ´Â ¾ÆÀÌÅÛÀÌ¸é
			{
				if(byOldWear == 130)
				{
					m_UserItem[byOldSlot].sLevel = 40;
				}
				if(m_UserItem[byOldSlot].sCount != 1)	// ¼Ò½º°¡ 1°³°¡ ¾Æ´Ï¸é
				{
					SendItemMoveFail();
					return;
				}

				bEmpty01 = bEmpty02 = FALSE;
				iEBodyPos = GetEBodyPos(byOldWear);
				if(iEBodyPos == -1)
				{
					SendItemMoveFail();
					return;
				}
				
				iItemIndex = m_UserItem[byOldSlot].sSid;
				if(iItemIndex < 0 || iItemIndex >= g_arItemTable.GetSize()) 
				{
					SendItemMoveFail(); 
					return;
				}
				m_UserItem[byOldSlot].sLevel = g_arItemTable[iItemIndex]->m_byRLevel;
				
				if(m_UserItem[byOldSlot].tIQ == MAGIC_ITEM)	// EBody ´Â ÇöÀç ¸ðµÎ ¸ÅÁ÷
				{
					iItemIndex = 5; // EBody ´Â ¿É¼ÇÀÌ 5°³
					
					for(i = 1; i < iItemIndex; i++)		// º£ÀÌ½º ¿É¼ÇÀº Ã¼Å©ÇÏÁö ¾Ê´Â´Ù.
					{
						iMagicIndex = m_UserItem[byOldSlot].tMagic[i];
						if(iMagicIndex >= 0 && iMagicIndex < g_arEBodyTable.GetSize())
						{
							m_UserItem[byOldSlot].sLevel += g_arEBodyTable[iMagicIndex]->m_tLevel;
						}
					}
				}
				
				if(m_UserItem[byOldSlot].sLevel > m_sLevel) { SendItemMoveFail(); return; }
				
				TempItem = m_UserItem[byOldSlot];
				m_UserItem[byOldSlot] = m_UserItem[iEBodyPos];
				m_UserItem[iEBodyPos] = TempItem;
					
				sChangeSlot[0] = byOldSlot;			
				sChangeSlot[1] = iEBodyPos;
					
				bChangeEquip = TRUE;
				break;
			}
			else SendItemMoveFail();
			break;
		default:
			SendItemMoveFail();
			return;
		};
		break;

	case 2:			// EBody
		switch(nNewPos)
		{
		case 0:		// EBody -> Equip
			SendItemMoveFail();
			return;
			break;
		case 1:		// EBody -> Inven
			if(m_UserItem[byNewSlot].sSid == -1) iSearchSlot = byNewSlot;	
			else	iSearchSlot = GetEmptySlot(INVENTORY_SLOT);

			if(iSearchSlot == -1)	// ºñ¾îÀÖ´Â ½½·ÔÀ» ¹ß°ßÇÏÁö ¸øÇßÀ¸¸é
			{
				SendItemMoveFail();
				return;
			}

			m_UserItem[iSearchSlot] = m_UserItem[byOldSlot];
			ReSetItemSlot(&m_UserItem[byOldSlot]);

			sChangeSlot[0] = byOldSlot;			
			sChangeSlot[1] = iSearchSlot;
			bChangeEquip = TRUE;

			break;
		case 2:		// EBody -> EBody
			SendItemMoveFail();
			return;
		default:
			break;
		};
		break;
	default:
		SendItemMoveFail();
		return;
	};	

	BYTE nCount = 0;
	for(i = 0; i < 3; i++)
	{
		if(sChangeSlot[i] != 255) nCount++;
	}

	BYTE bySlot;
	CBufferEx TempBuf;

	TempBuf.Add(ITEM_MOVE_RESULT);
	TempBuf.Add(SUCCESS);
	TempBuf.Add(tType);

	if(bChangeEquip)
	{
		
		SendMyWeaponChangeInfo();
		//ÖØÐÂ¼ÆËã×°±¸ÊôÐÔ..
		CheckMagicItemMove();
		SendMyInfo(TO_INSIGHT, INFO_MODIFY);
	}

	if(tType == 0)
	{
		TempBuf.Add(nCount);

		for(i = 0; i < 3; i++)
		{
			bySlot = (BYTE)sChangeSlot[i];
			
			if(bySlot == 255) break;
			TempBuf.Add(bySlot);
			TempBuf.Add(m_UserItem[bySlot].sLevel);
			TempBuf.Add(m_UserItem[bySlot].sSid);
			TempBuf.Add(m_UserItem[bySlot].sDuration);
			TempBuf.Add(m_UserItem[bySlot].sBullNum);
			TempBuf.Add(m_UserItem[bySlot].sCount);

			for(int j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[bySlot].tMagic[j]);

			TempBuf.Add(m_UserItem[bySlot].tIQ); 
		}
	}

	UpdateUserItemDN();

	Send(TempBuf, TempBuf.GetLength());
	//if(bTZItembool == TRUE)
	//{
	//	if(m_UserItem[0].tMagic[5] >= 7 && m_UserItem[1].tMagic[5] >= 7 && m_UserItem[4].tMagic[5] >= 7 && m_UserItem[8].tMagic[5] >= 7 && m_UserItem[9].tMagic[5] >= 7)
	//	{
	//		SendSystemMsg("ÌáÊ¾:7¸ÄÌ×¼Ó10»Ø±Ü.", SYSTEM_NORMAL, TO_ME);
	//	}
	//	if(m_UserItem[0].tMagic[5] >= 8 && m_UserItem[1].tMagic[5] >= 8 && m_UserItem[4].tMagic[5] >= 8 && m_UserItem[8].tMagic[5] >= 8 && m_UserItem[9].tMagic[5] >= 8)
	//	{
	//		SendSystemMsg("ÌáÊ¾:8¸ÄÌ×¼Ó25¿¹.", SYSTEM_NORMAL, TO_ME);
	//	}
	//	if(m_UserItem[0].tMagic[5] >= 9 && m_UserItem[1].tMagic[5] >= 9 && m_UserItem[4].tMagic[5] >= 9 && m_UserItem[8].tMagic[5] >= 9 && m_UserItem[9].tMagic[5] >= 9)
	//	{
	//		SendSystemMsg("ÌáÊ¾:9¸ÄÌ×¼Ó100Ñª50¿¹.", SYSTEM_NORMAL, TO_ME);
	//	}
	//	if(m_UserItem[0].tMagic[5] >= 10 && m_UserItem[1].tMagic[5] >= 10 && m_UserItem[4].tMagic[5] >= 10 && m_UserItem[8].tMagic[5] >= 10 && m_UserItem[9].tMagic[5] >= 10)
	//	{
	//		SendSystemMsg("ÌáÊ¾:10¸ÄÌ×¼¤»îÎÞÎÒºÍÊØ»¤ÏîÁ´Ð§¹û.Òþ²ØÈ«20ÊôÐÔ", SYSTEM_NORMAL, TO_ME);
	//	}
	//}
}

/////////////////////////////////////////////////////////////////////////////
//	ÇØ´ç ¾ÆÀÌÅÛÀÌ ÀåÂøµÉ¶§ ¸ÅÁ÷¼Ó¼º¿¡ ÀÇÇÑ °ªÀÌ ¹Ù²î´ÂÁö Ã¼Å©
//
void USER::CheckMagicItemMove()			//^^ ³ªÁß¿¡ ÃÖÀûÈ­ÇÏÀÚ...
{
	int i, j,t=0;
	int nCount = 4;						// ¸ÅÁ÷, ·¹¾îÀÏ¶§ ÂüÁ¶ È½¼ö´Â 4¹ø±îÁö
	int iMagic = 0;
	int iMagicCount = 0;
	int ebody[5]={0,0,0,0,0};
	BOOL ebody_skill=0;
	BOOL bOldShield = CheckAbnormalInfo(ABNORMAL_SHIELD) || CheckAbnormalInfo(ABNORMAL_BIGSHIELD);
	CBufferEx TempBuf;

	for(i = 0; i < MAGIC_COUNT; i++) m_DynamicUserData[i] = 0;
	for(i = 0; i < EBODY_MAGIC_COUNT; i++) m_DynamicEBodyData[i] = 0;
	for(i = 0; i < 10; i++) m_DynamicMagicItem[i]= 0;

	for(i = 0; i < EQUIP_ITEM_NUM; i++)
	{
		if(m_UserItem[i].sSid < 0 || m_UserItem[i].sSid >= g_arItemTable.GetSize()) continue;	// ¹æ¾îÄÚµå

		if(i == BODY || i == PANTS)
		{
			if(m_byClass == 3)
			{
				if(g_arItemTable[m_UserItem[i].sSid]->m_byRLevel == 80 && g_arItemTable[m_UserItem[i].sSid]->m_byClass == 1)
				{
					m_DynamicUserData[MAGIC_MAX_HP_UP] += g_arItemTable[m_UserItem[i].sSid]->m_byMPP;
				}
			}
		}

		if(i == LEFT_HAND)//¸±ÊÖ
		{
			if(m_UserItem[i].sSid == 1121) 
			{
				if(m_UserItem[i].sDuration <= 0) 
				{
					continue;					
				}			
			} 
			else 
			{
				continue;
			}
		}

		if((m_UserItem[i].tIQ >= REMODEL_ITEM && m_UserItem[i].tIQ <= REMODEL_MAGIC_ITEM)||m_UserItem[i].tIQ <= 12)  // ÀÏ´Ü ³ë¸ÖÀÌ ¾Æ´Ï¸é...
		{
			switch(m_UserItem[i].tIQ)
			{
			case MAGIC_ITEM:
				nCount = 2;
				break;
			case RARE_ITEM:
				nCount = 4;
				break;
			case UNIQUE_ITEM:
				nCount = MAGIC_NUM;
				break;
			case SET_ITEM:
				nCount = 4;
				break;
			case REMODEL_ITEM:
			case REMODEL_MAGIC_ITEM:
				nCount = 3;
				break;
			case 12://MarkJay¿ª·Åºó5ÌõÊôÐÔÉúÐ§
				nCount= 5;
				GetMagicItem_100(m_UserItem[i].tMagic[6]);
				GetMagicItem_100(m_UserItem[i].tMagic[7]);
				break;
			default:
				nCount = 4;
				break;
			}

			for(j = 0; j < nCount; j++)
			{
				iMagic = m_UserItem[i].tMagic[j];
				if(iMagic < 0 || iMagic >= g_arMagicItemTable.GetSize()) continue;

				iMagicCount = g_arMagicItemTable[iMagic]->m_sSubType;
				if(iMagicCount < 0 || iMagicCount >= MAGIC_COUNT) continue;
				
				m_DynamicUserData[iMagicCount] += g_arMagicItemTable[iMagic]->GetMagicItemValue();
			}
		}
	}	
//¼ÆËã»úÐµÊôÐÔ
	nCount = 5;
	BYTE tValue = 0;
	for(i = TOTAL_INVEN_MAX; i < TOTAL_ITEM_NUM-1; i++)	// E-Body Magic ¼Ó¼º Ãß°¡
	{
		if(m_UserItem[i].sSid >= 0)			
		{				
			if(m_UserItem[i].tIQ == MAGIC_ITEM )		// ´Ù ¸ÅÁ÷ ¼Ó¼ºÀÌ ºÙ¾î ÀÖÀ¸¸é...
			{
				for(j = 0; j < nCount; j++)				// ½½·Ô ¼ö¸¸Å­ µ¹¸°´Ù. 
				{				
					iMagic = m_UserItem[i].tMagic[j];
					if(iMagic < 0 || iMagic >= g_arEBodyTable.GetSize()) continue;
					
					iMagicCount = g_arEBodyTable[iMagic]->m_sSubType;
					if(iMagicCount < 0 || iMagicCount >= EBODY_MAGIC_COUNT) continue;
					
					tValue = (BYTE)g_arEBodyTable[iMagic]->m_sChangeValue;
					m_DynamicEBodyData[iMagicCount] += g_arEBodyTable[iMagic]->m_sChangeValue;
				}
				if(m_UserItem[i].sSid==987){
					//×îÖÕÉËÔö¼Ó
					m_DynamicMagicItem[7]=m_DynamicMagicItem[7] + m_UserItem[i].tMagic[5]*6;
				}else{ //×îÖÕÉË¼õÉÙ
					m_DynamicMagicItem[5]=m_DynamicMagicItem[5] + m_UserItem[i].tMagic[5]*2;
				}
			}
		}
	}
	//¼ÆËãÊØ»¤ÊôÐÔ
	GetMagicItemSetting_38();
	GetGuiIdMagic();
	//GetSkillSetting_130();//¼ÆËã130¼¼ÄÜ
	m_DynamicUserData[MAGIC_DEFENSE_UP]=m_DynamicUserData[MAGIC_DEFENSE_UP]+m_DynamicMagicItem[0];
	m_DynamicUserData[MAGIC_PSI_RESIST_UP]=m_DynamicUserData[MAGIC_PSI_RESIST_UP]+m_DynamicMagicItem[1];
	m_DynamicUserData[MAGIC_MAX_HP_UP]=m_DynamicUserData[MAGIC_MAX_HP_UP]+m_DynamicMagicItem[2];
	m_bRingOfLife		= FALSE;
	m_bNecklaceOfShield	= FALSE;
	m_bEarringOfProtect = FALSE;
	m_bCBOff	    	= FALSE;
	m_bShengRenjiezhi	= FALSE;
	for(i = 0; i < MAX_ACCESSORI; i++)
	{
		if(m_UserItem[g_iAccessoriSlot[i]].sSid == SID_RING_OF_LIFE)		m_bRingOfLife = TRUE;
		if(m_UserItem[g_iAccessoriSlot[i]].sSid == SID_NECKLACE_OF_SHIELD && m_UserItem[g_iAccessoriSlot[i]].sDuration > 0)	m_bNecklaceOfShield = TRUE;
		if(m_UserItem[g_iAccessoriSlot[i]].sSid == SID_EARRING_OF_PROTECT)	m_bEarringOfProtect = TRUE;
		if(m_UserItem[g_iAccessoriSlot[i]].sSid == SID_EARRING_OF_FEIFEI)	m_bCBOff = TRUE;
		if(m_UserItem[g_iAccessoriSlot[i]].sSid == SID_SHENGREN_JIEZHI)	    m_bShengRenjiezhi = TRUE;
	}
	//if(m_UserItem[0].tMagic[5] >= 10 && m_UserItem[1].tMagic[5] >= 10 && m_UserItem[4].tMagic[5] >= 10 && m_UserItem[8].tMagic[5] >= 10 && m_UserItem[9].tMagic[5] >= 10)//10¸ÄÌ×´øÊØ»¤ÏîÁ´ºÍÎÞÎÒÐ§¹û MarkJay
	//{
	//	m_bNecklaceOfShield = TRUE;
	//	m_bEarringOfProtect = TRUE;
	//}
	if(m_bNecklaceOfShield == TRUE) 
	{
		AddAbnormalInfo(ABNORMAL_SHIELD);
		if(bOldShield == FALSE)
		{
			TempBuf.Add(SET_USER_STATE);
			TempBuf.Add(m_uid + USER_BAND);
			TempBuf.Add(m_dwAbnormalInfo);
			TempBuf.Add(m_dwAbnormalInfo_);
			SendInsight(TempBuf, TempBuf.GetLength());
		}
	}
	else
	{
		if(bOldShield == TRUE && m_dwShieldTime == 0)
		{
			DeleteAbnormalInfo(ABNORMAL_SHIELD);
			TempBuf.Add(SET_USER_STATE);
			TempBuf.Add(m_uid + USER_BAND);
			TempBuf.Add(m_dwAbnormalInfo);
			TempBuf.Add(m_dwAbnormalInfo_);
			SendExactScreen(TempBuf, TempBuf.GetLength());
		}
	}	

	/*if(m_dwBFindTime > 0)//²âÊÔÍ·¶¥´øNEW MarkJay
	{
		AddNewStatus(New_Status_22);
		SendABNORMAL();
	}*/
	if(CheckAbnormalInfo(ABNORMAL_MAX_HP_UP)) SetUserToMagicUser(1);	// MAX_HP_UPÀÌ¸é °è¼Ó À¯Áö½ÃÄÑÁØ´Ù.
	else									  SetUserToMagicUser();

	SendUserStatusSkill();
}

/////////////////////////////////////////////////////////////////////////////
//	ÇØ´ç ¾ÆÀÌÅÛ ½½·ÔÀÇ ¾ÆÀÌÅÛÀÌ À¯Àú Å¬·¡½º¿¡¼­ »ç¿ëÇÒ¼ö ÀÖ´Â ¹«±âÀÎÁö ÆÇ´ÜÇÑ´Ù.
//
BOOL USER::IsCanEquipItem(int iSlot)
{
	int i;
	BOOL bflag = FALSE;
	CString strMsg;

	int sid = m_UserItem[iSlot].sSid;
	int armClass = g_arItemTable[sid]->m_byClass;

	if(sid < 0 || sid >= g_arItemTable.GetSize()) return FALSE;

	short sNeedStr = g_arItemTable[sid]->m_byRstr;
	short sNeedDex = g_arItemTable[sid]->m_byRdex;
	short sNeedVol = g_arItemTable[sid]->m_byRvol;

	switch(m_byClass)
	{
	case BRAWL:
		for(i = 0; i < USE_CLASS_TYPE; i++) 
		{ 
			if(g_BrawlClass[i] == armClass)
			{
				if(m_sMagicSTR < sNeedStr || m_sMagicDEX < sNeedDex || m_sMagicVOL < sNeedVol) { bflag = TRUE; goto go_result;}
				else return TRUE; 
			}
		}		
		break;
	case STAFF:
		for(i = 0; i < USE_CLASS_TYPE; i++) 
		{ 
			if(g_StaffClass[i] == armClass) 
			{
				if(m_sMagicSTR < sNeedStr || m_sMagicDEX < sNeedDex || m_sMagicVOL < sNeedVol) { bflag = TRUE; goto go_result;}
				else return TRUE; 
			}
		}
		break;
	case EDGED:
		for(i = 0; i < USE_CLASS_TYPE; i++) 
		{ 
			if(g_EdgedClass[i] == armClass)
			{
				if(m_sMagicSTR < sNeedStr || m_sMagicDEX < sNeedDex || m_sMagicVOL < sNeedVol) { bflag = TRUE; goto go_result;}
				else return TRUE; 
			}
		}
		break;
	case FIREARMS:
		for(i = 0; i < USE_CLASS_TYPE; i++) 
		{ 
			if(g_FireArmsClass[i] == armClass)
			{
				if(m_sMagicSTR < sNeedStr || m_sMagicDEX < sNeedDex || m_sMagicVOL < sNeedVol) { bflag = TRUE; goto go_result;}
				else return TRUE; 
			}
		}
		break;
	}

go_result:
	if(bflag)
	{
		SendSystemMsg( IDS_USER_ITEM_TOO_LOW_ABILITY, SYSTEM_NORMAL, TO_ME);//"ÄãµÄÊôÐÔ²»¹»×°±¸´ËÎïÆ·."
		return FALSE;
	}
	strMsg.Format( IDS_USER_ITEM_OTHER_CLASS );//"Õâ¸öÎïÆ·ÊÇÆäËûÖ°ÒµËù×°±¸."
	/*else //MarkJay ÐÞ¸´
	for(i = 0; i < USE_CLASS_TYPE; i++)
	{
		if(g_BrawlClass[i] == armClass) strMsg.Insert(0, _ID(IDS_USER_BRAWL));//"¸ñ¶·¼ÒÏµÁÐ"
		if(g_StaffClass[i] == armClass) strMsg.Insert(0, _ID(IDS_USER_STAFF));//"ÒìÄÜÕßÏµÁÐ"
		if(g_EdgedClass[i] == armClass) strMsg.Insert(0, _ID(IDS_USER_EDGED));//"½£ÎäÊ¿ÏµÁÐ"
		if(g_FireArmsClass[i] == armClass) strMsg.Insert(0, _ID(IDS_USER_FIREARMS)); // "Ç¹ÐµÊ¦ÏµÁÐ"
	}*/
	SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//	ÇØ´ç ¾ÆÀÌÅÛ ½½·ÔÀ» ÃÊ±âÈ­ÇÑ´Ù.
//
void USER::ReSetItemSlot(ItemList *pItem)
{
	if(!pItem) return;

	pItem->sLevel = 0;
	pItem->sSid		= -1;
	pItem->sDuration  = 0;
	pItem->sBullNum	= 0;
	pItem->sCount		= 0;
	::ZeroMemory(pItem->tMagic, MAGIC_NUM);
	pItem->tIQ = 0;
	pItem->iItemSerial = 0;

	pItem->dwTime = 0;
	pItem->uid[0] = pItem->uid[1] = pItem->uid[2] = -1;
	pItem->SuccessRate[0] = pItem->SuccessRate[1] = pItem->SuccessRate[2] = 0;
}

/////////////////////////////////////////////////////////////////////////////
//	¾ÆÀÌÅÛ ¿Å±â±â ½ÇÆÐ ÆÐÅ¶ Àü¼Û
//
void USER::SendItemMoveFail()
{
	CBufferEx TempBuf;

	TempBuf.Add(ITEM_MOVE_RESULT);
	TempBuf.Add((BYTE)FAIL);

	Send(TempBuf, TempBuf.GetLength());
}

/////////////////////////////////////////////////////////////////////////////
//	¾ÆÀÌÅÛÀÌ EQUIP, INVENTORY, EBody Slot Áß ¾îµð¿¡ ¼ÓÇÏ´ÂÁö ÆÇ´ÜÇÑ´Ù.
//	UpDate : 2001. 4. 18
int USER::GetSlotClass(int nSlot, int nType, BOOL flag)
{
	if(nSlot >= 0 && nSlot < EQUIP_ITEM_NUM) return 0;						// Equip
	else if(nSlot >= EQUIP_ITEM_NUM && nSlot < TOTAL_INVEN_MAX) return 1;	// Inven
	else if(nSlot >= TOTAL_INVEN_MAX && nSlot < TOTAL_ITEM_NUM) return 2;	// EBody
	else return -1;
}

//////////////////////////////////////////////////////////////////////////////
//	ºñ¾îÀÖ´Â ¾ÆÀÌÅÛ ½½·ÔÀ» Ã£´Â´Ù.
//
int USER::GetEmptySlot(int nSlot)
{
	int i = 0;

	switch(nSlot)
	{
	case EQUIP_SLOT:
		for(i = 0; i < EQUIP_ITEM_NUM; i++)
		{
			if(m_UserItem[i].sSid == -1) return i;
		}
		break;
	case INVENTORY_SLOT:
		for(i = EQUIP_ITEM_NUM; i < EQUIP_ITEM_NUM + INVENTORY_NUM; i++)
		{
			if(m_UserItem[i].sSid == -1) return i;
		}
		break;
	case BANK_SLOT:
		for(i = 0; i < TOTAL_BANK_ITEM_NUM; i++)
		{
			if(m_UserBankItem[i].sSid == -1) return i;
		}
		break;
	case ACCOUNT_BANK_SLOT:
		for(i = 0; i < TOTAL_ACCOUNT_BANK_ITEM_NUM; i++)
		{
			if(m_AccountBankItem[i].sSid == -1) return i;
		}
		break;
	case GUILD_SLOT:
		for(i = 0; i < TOTAL_BANK_ITEM_NUM; i++)
		{
			if(m_GuildItem[i].sSid == -1) return i;
		}
		break;
	default:
		break;
	}

	return -1;
}

///////////////////////////////////////////////////////////////////////////////
//	»ñµÃÏàÍ¬ÎïÆ·Î»ÖÃ ÖØµþÎïÆ·
int USER::GetSameItem(ItemList Item, int nSlot)
{
	int i = 0;
	int j = 0;
	BOOL nFlag = FALSE;

	if(Item.sSid < 0 || Item.sSid >= g_arItemTable.GetSize()) return -1;
	if(g_arItemTable[Item.sSid]->m_sDuration > 0) return -1;		// ³»±¸µµ°¡ ÀÖÀ¸¸é Àý´ë °ãÄ¡Áö ¾Ê´Â´Ù.
	
	switch(nSlot)
	{
	case INVENTORY_SLOT:
		for(i = EQUIP_ITEM_NUM; i < EQUIP_ITEM_NUM + INVENTORY_NUM; i++)
		{
			if(m_UserItem[i].sSid == Item.sSid &&
			   m_UserItem[i].sDuration == Item.sDuration)  
			{
				for(j = 0; j < MAGIC_NUM; j++)
				{
					if(m_UserItem[i].tMagic[j] != Item.tMagic[j]) { nFlag = TRUE; break; }
				}
					
				if(nFlag) break;

				if(m_UserItem[i].tIQ == Item.tIQ) return i;
			}
			if(m_UserItem[i].sSid == Item.sSid && m_UserItem[i].sSid == SPECIAL_BIAOZHI) return i;//±êÖ¾ÓÀÔ¶ÖØµþ
		}
		break;
	case TRADE_SLOT:
		EnterCriticalSection( &m_CS_ExchangeItem );
		for(i = 0; i < m_arExchangeItem.GetSize(); i++)
		{
			if ( m_arExchangeItem[i] == NULL ) continue;

			if(m_arExchangeItem[i]->changedList.sSid == Item.sSid &&
			   m_arExchangeItem[i]->changedList.sDuration == Item.sDuration &&
			   m_arExchangeItem[i]->changedList.sBullNum == Item.sBullNum)
			{
				for(j = 0; j < MAGIC_NUM; j++)
				{
					if(m_arExchangeItem[i]->changedList.tMagic[j] != Item.tMagic[j]) { nFlag = TRUE; break; }
				}
					
				if(nFlag) break;

				if ( m_arExchangeItem[i]->changedList.tIQ == Item.tIQ)
				{
					LeaveCriticalSection( &m_CS_ExchangeItem );
					return i;
				}
			}
			if(m_arExchangeItem[i]->changedList.sSid == Item.sSid && m_arExchangeItem[i]->changedList.sSid == SPECIAL_BIAOZHI)//±êÖ¾ÓÀÔ¶ÖØµþ
			{
				LeaveCriticalSection( &m_CS_ExchangeItem );
				return i;
			}
		}
		LeaveCriticalSection( &m_CS_ExchangeItem );
		break;
	case BANK_SLOT:
		for(i = 0; i < TOTAL_BANK_ITEM_NUM; i++)				// 
		{
			if(m_UserBankItem[i].sSid == Item.sSid &&
			   m_UserBankItem[i].sDuration == Item.sDuration &&
			   m_UserBankItem[i].sBullNum == Item.sBullNum)  
			{
				for(j = 0; j < MAGIC_NUM; j++)
				{
					if(m_UserBankItem[i].tMagic[j] != Item.tMagic[j]) { nFlag = TRUE; break; }
				}
					
				if(nFlag) break;

				if(m_UserBankItem[i].tIQ == Item.tIQ) return i;
			}
			if(m_UserBankItem[i].sSid == Item.sSid && m_UserBankItem[i].sSid == SPECIAL_BIAOZHI) return i;//±êÖ¾ÓÀÔ¶ÖØµþ
		}
		break;
	case ACCOUNT_BANK_SLOT:
		for(i = 0; i < TOTAL_ACCOUNT_BANK_ITEM_NUM; i++)				// 
		{
			if(m_AccountBankItem[i].sSid == Item.sSid &&
			   m_AccountBankItem[i].sDuration == Item.sDuration &&
			   m_AccountBankItem[i].sBullNum == Item.sBullNum)  
			{
				for(j = 0; j < MAGIC_NUM; j++)
				{
					if(m_AccountBankItem[i].tMagic[j] != Item.tMagic[j]) { nFlag = TRUE; break; }
				}
					
				if(nFlag) break;

				if(m_AccountBankItem[i].tIQ == Item.tIQ) return i;
			}
			if(m_AccountBankItem[i].sSid == Item.sSid && m_AccountBankItem[i].sSid == SPECIAL_BIAOZHI) return i;//±êÖ¾ÓÀÔ¶ÖØµþ
		}
		break;
	case GUILD_SLOT:
		for(i = 0; i < TOTAL_BANK_ITEM_NUM; i++)				// 
		{
			if(m_GuildItem[i].sSid == Item.sSid &&
			   m_GuildItem[i].sDuration == Item.sDuration &&
			   m_GuildItem[i].sBullNum == Item.sBullNum)  
			{
				for(j = 0; j < MAGIC_NUM; j++)
				{
					if(m_GuildItem[i].tMagic[j] != Item.tMagic[j]) { nFlag = TRUE; break; }
				}
					
				if(nFlag) break;

				if(m_GuildItem[i].tIQ == Item.tIQ) return i;
			}
			if(m_GuildItem[i].sSid == Item.sSid && m_GuildItem[i].sSid == SPECIAL_BIAOZHI) return i;//±êÖ¾ÓÀÔ¶ÖØµþ
		}
		break;

	default:
		break;
	}

	return -1;
}

////////////////////////////////////////////////////////////////////////////////////////
//	Äü¾ÆÀÌÅÛ µî·Ï Ãë¼Ò¸¦ Ã£¾Æ¼­ º¸³½´Ù. 
//
void USER::SendQuickChange()
{
/*	int i, j;
	int iChangeNum = 0;
	BOOL bChange = TRUE;
	int tChangeSlot[QUICK_ITEM];
	memset(tChangeSlot, -1, sizeof(tChangeSlot));

	int iQuick = TOTAL_ITEM_NUM;

	CBufferEx TempBuf;

	for(i = 0; i < QUICK_ITEM; i++)
	{
		if(m_UserItem[iQuick].sSid != -1)
		{
			for(j = EQUIP_ITEM_NUM; j < TOTAL_ITEM_NUM; j++)
			{
				if(m_UserItem[iQuick].sSid == m_UserItem[j].sSid) { bChange = FALSE; break; }
			}

			if(bChange == TRUE) 
			{ 
				m_UserItem[i+TOTAL_ITEM_NUM].sSid = -1; 
				tChangeSlot[i] = i; iChangeNum++; 
			}
		}
		iQuick++;
		bChange = TRUE;
	}

	if(iChangeNum == 0) return;

	short sCount = -1;
	TempBuf.Add(ITEM_MOVE_RESULT);
	TempBuf.Add((BYTE)0x01);			// ¼º°ø
	TempBuf.Add((BYTE)0x02);			// Äü¾ÆÀÌÅÛ µî·Ï Ãë¼Ò

	TempBuf.Add((BYTE)iChangeNum);

	for(i = 0; i < QUICK_ITEM; i++)
	{
		if(tChangeSlot[i] != -1)
		{
			TempBuf.Add((BYTE)i);
			TempBuf.Add(sCount);
		}
	}

	Send(TempBuf, TempBuf.GetLength());
*/
}
/////////////////////////////////////////////////////////////////////////////////
//	ÏûºÄÎïÆ·ÐÅÏ¢
void USER::ItemUseReq(TCHAR *pBuf)
{
#ifdef _ACTIVE_USER
	if( m_iDisplayType == 6) return; //yskang 0.5
#endif

	int iMode = 0;
	int index = 0;
	short sSid = 0;
	short sValue = 0;
	int iRandom = 0;

	short sSP = -1;	
	short sHP = -1;	
	short sPP = -1;	

	CString strMsg = _T("");

	BYTE tResult = SUCCESS;
	BOOL bRes = FALSE, bWeight = FALSE;
	int iInvenSlot = 0;

	DWORD dwDN = 0;

	int type = 0;

	BYTE tType = GetByte(pBuf, index);
	BYTE tSlot = GetByte(pBuf, index);
//	sSid = GetShort(pBuf, index);

	if(tType == 3)//ËæÉíÐÞÀí¹¤¾ß MarkJay
	{
		BYTE tSlot1 = GetByte(pBuf, index);		
		if(tSlot < EQUIP_ITEM_NUM || tSlot >= TOTAL_INVEN_MAX) return;		
		if(tSlot1 < 0 || tSlot >= 34) return;
		sSid = m_UserItem[tSlot1].sSid;
		if(sSid < 0 || sSid >= g_arItemTable.GetSize()) return;
		if(m_dwVIPTime == 0)
		{
			SendSystemMsg("²»ÊÇVIP»áÔ±,ÎÞ·¨Ê¹ÓÃËæÉíÐÞÀí.", SYSTEM_ERROR, TO_ME);
			return;
		}
		if(sSid == 1121)
		{
			SendSystemMsg("Ö»ÄÜÐÞÀí×°±¸»òÎäÆ÷.", SYSTEM_ERROR, TO_ME);
			return;
		}
        if(g_arItemTable[sSid]->m_byWear < 1 || g_arItemTable[sSid]->m_byWear> 5)
		{
			SendSystemMsg("Ö»ÄÜÐÞÀí×°±¸»òÎäÆ÷.", SYSTEM_ERROR, TO_ME);
			return;
		}
		if(m_UserItem[tSlot1].sDuration == g_arItemTable[sSid]->m_sDuration)
		{
			SendSystemMsg("²»ÐèÒªÔÙÐÞÀí.", SYSTEM_ERROR, TO_ME);
			return;
		}
		if(g_arItemTable[m_UserItem[tSlot].sSid]->m_byWear == 129)
		{
			m_UserItem[tSlot1].sDuration = g_arItemTable[sSid]->m_sDuration;
			CBufferEx TempBuf1;
			TempBuf1.Add(ITEM_DURATION);
			TempBuf1.Add((byte)tSlot1);
			TempBuf1.Add(m_UserItem[tSlot1].sDuration);
			Send(TempBuf1, TempBuf1.GetLength());
			SendDGDuration(m_UserItem[tSlot].sSid,tSlot);
			if(g_arItemTable[sSid]->m_byWear != 1)
			{
				SendSystemMsg("×°±¸ÐÞÀíÍê³É.", SYSTEM_NORMAL, TO_ME);
			}
			else
			{
				SendSystemMsg("ÎäÆ÷ÐÞÀíÍê³É.", SYSTEM_NORMAL, TO_ME);
			}
			return;
		}		
	}

	BYTE tUsePotion = 0;

	CBufferEx TempBuf,TempBuf1,TempBuf2, PotionBuf;

	TempBuf.Add(ITEM_USE_RESULT);
	
	/************************¿¹¿ÜÃ³¸®****************************************/
	if(tType != 1 && tType != 2) { bRes = TRUE; goto go_result; } 
	if(tSlot < EQUIP_ITEM_NUM || tSlot >= TOTAL_INVEN_MAX) { bRes = TRUE; goto go_result; } 

	sSid = m_UserItem[tSlot].sSid;
	if(sSid < 0 || sSid >= g_arItemTable.GetSize()) { bRes = TRUE; goto go_result; } 
																		// °°Àº ¼Ó¼º ³¢¸® ºÙ¿©³õ±âÀ§ÇØ ITEM_MAX_USE_WEAR¸¦ »ç¿ë
	if(g_arItemTable[sSid]->m_byWear < 100 || g_arItemTable[sSid]->m_byWear > ITEM_MAX_USE_WEAR) 
	{
		if(g_arItemTable[sSid]->m_byWear != 15) { bRes = TRUE; goto go_result; } // ÀÌº¥Æ® º¹±Ç...
	} 

	if(m_UserItem[tSlot].sCount <= 0) { bRes = TRUE; goto go_result; }

	if(g_arItemTable[sSid]->m_sBullNum != 0)
	{
		if(m_UserItem[RIGHT_HAND].sSid < 0) 
		{
			strMsg.Format( IDS_USER_CANT_USE_THIS, g_arItemTable[sSid]->m_strName);// "%s ÎïÆ·ÎÞ·¨Ê¹ÓÃ."
			SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
			bRes = TRUE; 
			goto go_result; 
		} 
		if(g_arItemTable[sSid]->m_byBullType != g_arItemTable[m_UserItem[RIGHT_HAND].sSid]->m_byBullType)
		{
			strMsg.Format( IDS_USER_CANT_USE_THIS, g_arItemTable[sSid]->m_strName);// "%s ÎïÆ·ÎÞ·¨Ê¹ÓÃ."
			SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
			bRes = TRUE; 
			goto go_result;
		}
	}

	/************************½ÇÇà ·çÆ¾***************************************/
	iMode = GetItemFieldNum((int)sSid);
	TempBuf.Add(tResult);
	TempBuf.Add((BYTE)iMode);

	switch(iMode)								
	{
	case QUICK_ITEM_SP:											//¼ÓÌå			
		sValue = g_arItemTable[sSid]->m_sRepairSP;
		m_sSP =  m_sSP + sValue;
		if(m_sMagicMaxSP <= m_sSP) m_sSP = m_sMagicMaxSP;

		sSP = m_sSP;
		TempBuf.Add(sSP);										
		TempBuf.Add(sHP);
		TempBuf.Add(sPP);
		TempBuf.Add((short)-1);  //¿ª»úÀ¶ÁúÐÄ
		bWeight = TRUE;
		tUsePotion = SP_POTION;
		break;

	case QUICK_ITEM_PP:											//¼ÓÀ¶
		sValue = g_arItemTable[sSid]->m_sRepairPP;
		sValue += (short)((double)sValue * m_DynamicEBodyData[EBODY_PP_RECOVERY_UP] / 100);
		m_sPP =  m_sPP + sValue;
		if(m_sMagicMaxPP <= m_sPP) m_sPP = m_sMagicMaxPP;

		sPP = m_sPP;
		TempBuf.Add(sSP);									
		TempBuf.Add(sHP);
		TempBuf.Add(sPP);
		TempBuf.Add((short)-1);//¿ª»úÀ¶ÁúÐÄ
		bWeight = TRUE;
		tUsePotion = PP_POTION;
		break;

	case QUICK_ITEM_HP:											//¼ÓÑª
		if(f_Markini[0]->Wg_HP_KG == 1)
		{
			if(g_arItemTable[sSid]->m_sPid == 30 || g_arItemTable[sSid]->m_sPid == 29 || g_arItemTable[sSid]->m_sPid == 269 || g_arItemTable[sSid]->m_sPid == 28 || g_arItemTable[sSid]->m_sPid == 57)			//¼ø°£ È¸ÇÇ ¾ÆÀÌÅÛ
			{
				DWORD dwCurrTick = GetTickCount();
				if(dwCurrTick - m_JiaXueTime < 100)//HPºìÒ©Ê±¼ä  Ô½´óÑÓ³ÙÔ½´óÏÞÖÆ¼ÓÑªËÙ¶È
				{
					return;
				}
				else
				{
					m_JiaXueTime = dwCurrTick;
				}
			}
		}
		sValue = g_arItemTable[sSid]->m_sRepairHP;
		sValue += (short)((double)sValue * m_DynamicEBodyData[EBODY_HP_RECOVERY_UP] / 100);
		m_sHP =  m_sHP + sValue;
		if(m_sMagicMaxHP <= m_sHP) m_sHP = m_sMagicMaxHP;

		sHP = m_sHP;
		TempBuf.Add(sSP);								
		TempBuf.Add(sHP);
		TempBuf.Add(sPP);
		TempBuf.Add((short)-1);//¿ª»úÀ¶ÁúÐÄ
		bWeight = TRUE;
		tUsePotion = HP_POTION;

		if(m_bNowBuddy == TRUE) SendBuddyUserChange(BUDDY_CHANGE);	
		break;

	case QUICK_ITEM_ALLREPAIR:									//¼Ó³£
		m_sSP += g_arItemTable[sSid]->m_sRepairSP;
		m_sPP += g_arItemTable[sSid]->m_sRepairPP;
		m_sHP += g_arItemTable[sSid]->m_sRepairHP;

		if(m_sMagicMaxSP <= m_sSP) m_sSP = m_sMagicMaxSP;
		if(m_sMagicMaxPP <= m_sPP) m_sPP = m_sMagicMaxPP;
		if(m_sMagicMaxHP <= m_sHP) m_sHP = m_sMagicMaxHP;
		if(m_bNowBuddy == TRUE) SendBuddyUserChange(BUDDY_CHANGE);	
		sSP = m_sSP; sHP = m_sHP; sPP = m_sPP;
		TempBuf.Add(sSP);								
		TempBuf.Add(sHP);
		TempBuf.Add(sPP);
		TempBuf.Add((short)-1);//¿ª»úÀ¶ÁúÐÄ

		bWeight = TRUE;
		tUsePotion = ALL_POTION;
		break;

	case QUICK_ITEM_BULL:	//ÄÜÁ¿°ü ÎïÆ·								
		m_UserItem[RIGHT_HAND].sBullNum = g_arItemTable[sSid]->m_sBullNum;
		SendBullNum(RIGHT_HAND);
		bWeight = TRUE;
		break;
	case QUICK_ITEM_TELEPORT://´«ËÍÃÅ
		sSid = GetByte(pBuf, index);
		GetHanyGate(tSlot, sSid);
		bWeight = TRUE;
		return;
		
	case QUICK_ITEM_POTAL:
		if(g_arItemTable[sSid]->m_sPid == 44)			//ÉÁ±ÜÒ©Ë®
		{
			DWORD dwCurrTick = GetTickCount();
			if(dwCurrTick - m_dwLastItem44Time < 3 * 1000) return;//ÉÁ±ÜÒ©Ë®Ê±¼ä
			else m_dwLastItem44Time = dwCurrTick;
			type = 0, index = 0;
			int iRepeat = 0;
			int rank = m_sCityRank + CITY_RANK_INTERVAL;

			if(rank <= DEATH_RANK || m_sKillCount >= 100) 
			{
				SendSystemMsg( IDS_USER_TOO_LOW_CITY_VALUE, SYSTEM_ERROR, TO_ME);// "°ÔÍõÎÞ·¨Ê¹ÓÃ."
				return;
			}
		    if(m_dwCloseTime > 0 || m_tAbnormalKind == ABNORMAL_BYTE_CONFUSION || m_curz == 62 || m_curz == 409 || m_curz == 92)    //Ñ£ÔÎ/¼àÓü/62/409µØÇøÎÞ·¨Ê¹ÓÃÉÁ±ÜÒ©Ë®
			{
				SendSystemMsg( "ÎÞ·¨Ê¹ÓÃÉÁ±ÜÒ©Ë®.", SYSTEM_ERROR, TO_ME);
				return;
			}

			int type1 = 0;
	        type1 = CheckInvalidMapType();	

			if(type1 == 8 || type1 == 10 || type1 == 15 || type1 == 9 || type1 == 11 || type1 == 16)   
			{
				SendSystemMsg( "ÒªÈûÕ½ÇøÄÚ,ÎÞ·¨Ê¹ÓÃ.", SYSTEM_ERROR, TO_ME);
				return;
			}

			if( m_ZoneIndex < 0 || m_ZoneIndex >= g_zone.GetSize() ) { bRes = TRUE; goto go_result; }

			MAP *pMap = g_zone[m_ZoneIndex];
			if(!pMap) { bRes = TRUE; goto go_result; } 

			type = CheckInvalidMapType();
			if(!CheckInvalidZoneState(type)) { bRes = TRUE; goto go_result; } 

			do
			{
				int iEvadeRange = 14;
				int iEvadeRandom = 1;
				iEvadeRandom = myrand(1, 5);
				int min_x = m_curx - iEvadeRange * iEvadeRandom; if(min_x < 0) min_x = 0;
				int max_x = m_curx + iEvadeRange * iEvadeRandom; if(max_x >= pMap->m_sizeMap.cx) max_x = pMap->m_sizeMap.cx - 1;
				int min_y = m_cury - iEvadeRange * iEvadeRandom; if(min_y < 0) min_y = 0;
				int max_y = m_cury + iEvadeRange * iEvadeRandom; if(max_y >= pMap->m_sizeMap.cy) max_y = pMap->m_sizeMap.cy - 1;
				
				int move_x = 0, move_y = 0;
				
				move_x = myrand(min_x, max_x);
				move_y = myrand(min_y, max_y);

				type = ((g_zone[m_ZoneIndex]->m_pMap[move_x][move_y].m_dwType & 0xFF00 ) >> 8);
				
				if(!CheckInvakidZoneChangeState(type)) { bRes = TRUE; goto go_result; } 

				if(Teleport(move_x, move_y) == FALSE) iRepeat++;
				else break;				
			}
			while(iRepeat < 6);

			if( iRepeat >= 6 )
			{
				bRes = TRUE; goto go_result;
			}
		}
		else if(m_curz == 1001 && g_arItemTable[sSid]->m_sPid == 45)//ËÀÍöÁìµØÎÞ·¨Ê¹ÓÃÊ±¿ÕÒ©Ë®
		{
			SendSystemMsg( "½øÈëËÀÍöÁìµØºó¾ÍÎÞ·¨Ê¹ÓÃÊ±¿ÕÒ©Ë®!", SYSTEM_ERROR, TO_ME);
			bRes = TRUE; goto go_result;
		}
		else			
		{
			if(m_tGuildHouseWar == GUILD_WARRING) { bRes = TRUE; goto go_result; } 
			if(IsCity()) { bRes = TRUE; goto go_result; } 

			if(m_tQuestWar == GUILD_WARRING) g_QuestEventZone.CheckUserStateInEventZone(m_curz);

			if( !ReturnTown( tType, tSlot ) ) return;
		}

		bWeight = TRUE;
		break;

	case QUICK_ITEM_SALUTE:		
		if(sSid == NORMAL_SALUTE_SID)			tUsePotion = SALUTE;//±ÞÅÚ
		else if(sSid == XMAS_SALUTE_SID)		tUsePotion = XMAS_SALUTE;//Ê¥µ®ÑÌ»¨
		else if(sSid == YEAR_SALUTE_SID)		tUsePotion = YEAR_SALUTE;//ÐÂÄêÑÌ»¨
		else tUsePotion = SALUTE;
		bWeight = TRUE;
		break;

	case EVENT_ITEM_LOTTERY://²ÊÆ±±àºÅ374
		dwDN = CheckLottery();		
		ReSetItemSlot(&m_UserItem[tSlot]);
		CheckMaxValue(m_dwDN, dwDN);
		MakeMoneyLog( dwDN, ITEMLOG_MONEY_LOTTERY );
		FlushItemLog( TRUE );
		bWeight = TRUE;
		break;

	case EVENT_ITEM_DEADPOISON:
		tUsePotion = DEAD_POTION;//Ê¥Ò©
		  {			
			int skill_point = 0;
			if( m_sLevel <= 60 )
				skill_point = (int)( (double)m_sLevel / 2 + 0.5 );
			else if( m_sLevel > 60 && m_sLevel <= 99 )
				skill_point = (int)( 30 + (double)m_sLevel - 60 + 0.5 );
			else if( m_sLevel > 99 )
				skill_point = (int)( 30 + 99 - 60 + 0.5 );
			else
			{
				return;
			}
			if(sSid == 1022)//ÖØÉú
			{
				switch (m_byClass)
				{
					case 0://È­
						m_sSTR = 14;
						m_sCON = 15;
						m_sDEX = 10;
						m_sVOL = 9;	
						m_sWIS = 9;
						break;
					case 1:
						m_sSTR = 9;
						m_sCON = 10;
						m_sDEX = 9;
						m_sVOL = 17;
						m_sWIS = 12;
						break;
					case 2:
						m_sSTR = 15;
						m_sCON = 14;
						m_sDEX = 10;
						m_sVOL = 9;
						m_sWIS = 9;
						break;
					case 3:
						m_sSTR = 10;
						m_sCON = 12;
						m_sDEX = 17;
						m_sVOL = 9;
						m_sWIS = 9;
						break;
				}
				m_sPA = (short)(m_sLevel-1)+8;
			}
			else
			{
				m_sSTR = m_sTempSTR;					
				m_sCON = m_sTempCON;					
				m_sDEX = m_sTempDEX;					
				m_sVOL = m_sTempVOL;					
				m_sWIS = m_sTempWIS;
				m_sPA = (short)(m_sLevel-1);
			}
			DelMyEventNum(251);
			for(int i = 0; i < TOTAL_SKILL_NUM; i++)
			{
				m_UserSkill[i].sSid=i;
				m_UserSkill[i].tLevel = 0;
			}
	
			m_UserSkill[4].sSid = 4;
			m_UserSkill[9].sSid = 9;
			m_UserSkill[14].sSid = 14;
			m_UserSkill[19].sSid = 19;

			m_sSkillPoint = (short)skill_point;
			if(m_sLevel >= 130 && f_Markini[0]->KG130 == 1)
			{
				m_sSkillPoint_= m_sLevel - 129;
				if(m_sSkillPoint_ > 60) m_sSkillPoint_ = 60;
				if(m_sSkillPoint_ < 0) m_sSkillPoint_ = 0;//MarkJay ÐÞ¸Ä
				for(int i = 0; i < 12; i++)
				{
					m_UserSkill_[i].sSid=i;
					m_UserSkill_[i].tLevel = 0;
				}
			}
			SetUserToMagicUser();

			UpdateUserData();

			SendUserStatusSkill();

			bWeight = TRUE;
		}
		break;

	case EVENTITEM_WEAR_MOON:
		if(MoonEvent(tSlot)) bWeight = TRUE;
		else bRes = TRUE;
		break;

	case EVENTITEM_WEAR_INVASION:
		if(InvaderSummon(tSlot)) bWeight = TRUE;
		else bRes = TRUE;
		break;

	case SPECIAL_POTION_WEAR:
		tUsePotion = UseSpecialPotion(tSlot);
		if(tUsePotion)
		{
			MakeItemLog( &m_UserItem[tSlot], ITEMLOG_SPECIAL_USE_POTION );
			FlushItemLog( TRUE );

			bWeight = TRUE;
		}
		else bRes = TRUE;

		break;

	case PSI_STONE_WEAR:
		if(UsePsiStone(tSlot) == TRUE)
		{
			MakeItemLog( &m_UserItem[tSlot], ITEMLOG_SPECIAL_USE_POTION );
			FlushItemLog( TRUE );

			bWeight = TRUE;
		}
		else bRes = TRUE;

		break;
	default:
		bRes = TRUE;
		break;
	}

go_result:

	if(bWeight) m_iCurWeight -= g_arItemTable[sSid]->m_byWeight;		// ¾´¸¸Å­ ¹«°Ô¸¦ ÁÙÀÎ´Ù.

	if(bRes)
	{
		tResult = FAIL;
		TempBuf2.Add(ITEM_USE_RESULT);
		TempBuf2.Add(tResult);
		Send(TempBuf2, TempBuf2.GetLength());
		return;
	}

	UpdateUserItemDN();							// ¾ÆÀÌÅÛ º¯µ¿ Ã¼Å©...

	GetRecoverySpeed();							// È¸º¹¼Óµµ Ã¼Å©...

	TempBuf.Add((BYTE)tSlot);

	if(iMode == EVENT_ITEM_LOTTERY) { TempBuf.Add(dwDN); Send(TempBuf, TempBuf.GetLength()); return; }

	m_UserItem[tSlot].sCount -= 1;		//
	if(m_UserItem[tSlot].sCount <= 0) { m_UserItem[tSlot].sSid = -1; sSid = -1; }//SendQuickChange(); }

	TempBuf.Add(m_UserItem[tSlot].sLevel);
	TempBuf.Add(sSid);
	TempBuf.Add(m_UserItem[tSlot].sDuration);
	TempBuf.Add(m_UserItem[tSlot].sBullNum);
	TempBuf.Add(m_UserItem[tSlot].sCount);
	for(int i = 0; i < MAGIC_NUM; i++) TempBuf.Add(m_UserItem[tSlot].tMagic[i]);
	TempBuf.Add(m_UserItem[tSlot].tIQ);

	Send(TempBuf, TempBuf.GetLength());

	type = CheckInvalidMapType();
									
	if(CheckInvalidZoneState(type))		// ÀÏ¹Ý ÇÊµåÀÌ¸é
	{
		if(tUsePotion)
		{
			if(g_PotionViewOnOff == 1) return;	// Ä§·¨Ð§¹û¶¯»­ ¿ª¹Ø

			PotionBuf.Add(USE_POTION);
			PotionBuf.Add(m_uid + USER_BAND);
			if(tUsePotion == 101 || tUsePotion == 102)
			{
				PotionBuf.Add((BYTE)0xf);
				PotionBuf.Add((int)(tUsePotion-100));
			}
			else
			{
				PotionBuf.Add((BYTE)tUsePotion);
			}
			if(m_curz == 61 || m_curz == 62 || m_curz == 63) SendInsight( PotionBuf, PotionBuf.GetLength() );
			else SendExactScreen( PotionBuf, PotionBuf.GetLength() );
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////
//	º¹±Ç ´çÃ·¿©ºÎ¸¦ Ã¼Å©ÇÑ´Ù. 
//
DWORD USER::CheckLottery()
{
	int type = myrand(1, 10);
	DWORD dwDN = 0;

	int iMax = 0;

	switch(type)
	{
	case 1:
		iMax = myrand(1, _MAX_INT);
		if(iMax == 1) dwDN = 10000000;
		break;
	case 2:
		iMax = myrand(1, 1000000000);
		if(iMax == 1) dwDN = 8000000;
		break;
	case 3:
		iMax = myrand(1, 100000000);
		if(iMax == 1) dwDN = 6000000;
		break;
	case 4:
		iMax = myrand(1, 10000000);
		if(iMax == 1) dwDN = 4000000;
		break;
	case 5:
		iMax = myrand(1, 1000000);
		if(iMax == 1) dwDN = 2000000;
		break;
	case 6:
		iMax = myrand(1, 100000);
		if(iMax == 1) dwDN = 1000000;
		break;
	case 7:
		iMax = myrand(1, 10000);
		if(iMax == 1) dwDN = 500000;
		break;
	case 8:
		iMax = myrand(1, 1000);
		if(iMax == 1) dwDN = 250000;
		break;
	case 9:
		iMax = myrand(1, 100);
		if(iMax == 1) dwDN = 100000;
		break;
	case 10:
		iMax = myrand(1, 10);
		if(iMax == 1) dwDN = 10000;
		break;
	}

	return dwDN;
}

BOOL USER::LoadGuildLevel()
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode;
	TCHAR			szSQL[1024];

	::ZeroMemory(szSQL, sizeof(szSQL));
	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call LOAD_GUILD_LEVEL (\'%s\')}"), m_strGuildName);

	SQLUINTEGER iLevel;
	SQLINTEGER	sInd;

	int db_index = 0;
	CDatabase* pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );

	if( retcode != SQL_SUCCESS )
	{
		return FALSE;
	}

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, SQL_NTS);

	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		retcode = SQLFetch( hstmt );

		if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
		{
			int i = 1;
			SQLGetData( hstmt, i++, SQL_C_ULONG,	&iLevel,		sizeof(iLevel),		&sInd );
		}
		else 
		{
			retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
			g_DB[m_iModSid].ReleaseDB(db_index);
			return FALSE;
		}
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(db_index);

	m_dwGuildLevel = iLevel;

	return TRUE;
}
BOOL USER::UpdateGuildLevel(int Templevel)
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode = 0;
	BOOL			bQuerySuccess = TRUE;
	TCHAR			szSQL[8000];		
	int				i;

	int Val = Templevel;

	::ZeroMemory(szSQL, sizeof(szSQL));

	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call UPDATE_GUILD_LEVEL (\'%s\', %d)}"), m_strGuildName, Val);

	int db_index = 0;
	CDatabase* pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		TRACE("Fail To Update Guild Warehouse Data !!\n");

		//g_DB[m_iModSid].ReleaseDB(db_index);
		return FALSE;
	}

	if (retcode == SQL_SUCCESS)
	{
		i = 1;
		retcode = SQLExecDirect(hstmt, (unsigned char *)szSQL, SQL_NTS);		
		if (retcode ==SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{
		}
		else if (retcode==SQL_ERROR)
		{
			DisplayErrorMsg( hstmt );
			SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
			BREAKPOINT();

			g_DB[m_iModSid].ReleaseDB(db_index);
			return FALSE;
		}
	}	
	else
	{
		DisplayErrorMsg( hstmt );
		SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		BREAKPOINT();

		g_DB[m_iModSid].ReleaseDB(db_index);
		return FALSE;
	}

	if (hstmt!=NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(db_index);


	if( !bQuerySuccess ) return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////
//	¾ÆÀÌÅÛ ÇÊµå¿¡¼­ ÇØ´ç Wear ¹øÈ£¸¦ ¾Ë·ÁÁØ´Ù.
//
int USER::GetItemFieldNum(int iNum)
{
	BYTE tCure = 0;
	BYTE tHP = 0;
	BYTE tPP = 0;
	BYTE tSP = 0;

	BYTE tResult = 0;

	if(iNum < 0 || iNum >= g_arItemTable.GetSize()) return 0;//MarkJay ÐÞ¸Ä

	switch(g_arItemTable[iNum]->m_byWear)
	{
	case 100:			// Ä¡·áÁ¦ Á¾·ù..
//		return g_arItemTable[iNum]->m_bySubCure;
		break;
	case 101:			// È¸º¹Á¦ Á¾·ù
		if(g_arItemTable[iNum]->m_bySubCure) { tCure = 1; tCure <<= 3; }
		if(g_arItemTable[iNum]->m_sRepairHP) { tHP = 1; tHP <<= 2; }
		if(g_arItemTable[iNum]->m_sRepairPP) { tPP = 1; tPP <<= 1; }
		if(g_arItemTable[iNum]->m_sRepairSP) tSP = 1; 

		tResult = tCure + tHP + tPP + tSP;
		break;

	case 102: case 103:	// ÃÑ±â µµ°Ë ÆÑ
		tResult = QUICK_ITEM_BULL;
		break;

	case 104:			// ÇÁ¸®¿¡½º ¸®¿Â, ¶óÀÌµå
		tResult = QUICK_ITEM_POTAL;
		break;

	case 14:			// 
		break;

	case 15:			// º¹±Ç
		tResult = EVENT_ITEM_LOTTERY;
		break;

	case 105:			// ÇÚµð°ÔÀÌÆ®
		tResult = QUICK_ITEM_TELEPORT;
		break;

	case 111:
		tResult = EVENT_ITEM_DEADPOISON;
		break;

	case 113:
		tResult = QUICK_ITEM_SALUTE;
		break;
		
	case 115:
		tResult = EVENTITEM_WEAR_MOON;
		break;

	case 116:
		tResult = SPECIAL_POTION_WEAR;
		break;

	case 117:
		tResult = PSI_STONE_WEAR;
		break;
	case 120:
		tResult = EVENTITEM_WEAR_INVASION;
		break;

	default:		
		break;
	}

	return tResult;
/*
	if(g_arItemTable[iNum]->m_byWear == 13) return QUICK_ITEM_POTAL;
	if(g_arItemTable[iNum]->m_byWear == 15) return EVENT_ITEM_LOTTERY;
	if(g_arItemTable[iNum]->m_sBullNum)	return QUICK_ITEM_BULL;

	if(g_arItemTable[iNum]->m_bySubCure) { tCure = 1; tCure <<= 3; }
	if(g_arItemTable[iNum]->m_sRepairHP) { tHP = 1; tHP <<= 2; }
	if(g_arItemTable[iNum]->m_sRepairPP) { tPP = 1; tPP <<= 1; }
	if(g_arItemTable[iNum]->m_sRepairSP) tSP = 1; 

	tResult = tCure + tHP + tPP + tSP;
		
	if(tResult == 8) return g_arItemTable[iNum]->m_bySubCure;

	return tResult;
*/
/*
	SubCure : RepairHP : RepairPP : RepairSP : RETURN
		0		1			0			0		4
		0		0			1			0		2
		0		0			0			1		1
		1		0			0			0		m_bySubCure;
		0		1			1			1		7
*/
}

/////////////////////////////////////////////////////////////////////////////////
//	¸Ê ¶Ç´Â »ó´ë À¯Àú¿¡°Ô ¾ÆÀÌÅÛÀ»...
//
void USER::ItemGiveReq(TCHAR *pBuf)
{
#ifdef _ACTIVE_USER
//	if(m_iDisplayType == 6 && m_sLevel > 25) return; //yskang 0.5
	if(m_iDisplayType == 6) return; //yskang 0.5
#endif

	int		index = 0;

	BYTE tType = GetByte(pBuf, index);
	BYTE tObject = GetByte(pBuf, index);

	if(m_bNowTrading == TRUE || m_bPShopOpen == TRUE) return;//ÐÞ¸´ÎïÆ·ÒÆ¶¯¸´ÖÆBUG

	if(tType == 1)		// À¯Àú
	{
		if(tObject == 0) GiveMoneyToUser(pBuf + index);//È¡ÏûµôÇ®

		if(tObject == 1) GiveItemToUser(pBuf + index);//È¡Ïûµô¸øÎïÆ·
	}
	else if(tType == 0) // ¸Ê
	{
		if(tObject == 0) GiveMoneyToMap(pBuf + index);

		if(tObject == 1) GiveItemToMap(pBuf + index);
	}

}

/////////////////////////////////////////////////////////////////////////////////
// ½»Ò×Ç®
void USER::GiveMoneyToUser(TCHAR *pBuf)
{
	int		index = 0;

	BOOL nFlag = TRUE;
	BYTE error_code = 0;

	BOOL bBuddyMember;
	int  nMeMapType, nYouMapType;

	CString strMsg;
	USER *pUser = NULL;

	int uid = GetInt(pBuf, index);
	if(!m_MItemLock && f_Markini[0]->JYMM_KG == 1)
	{
		SendSysWhisperChat("¶Ô²»Æð!½»Ò×ÃÜÂëÉÐÎ´½â³ý,ÎÞ·¨¸ø¶Ô·½½ðÇ®");
		return;
	}
	if(m_sLevel < 10)
	{
		SendSysWhisperChat("µÈ¼¶µÍÓÚ10¼¶,ÎÞ·¨¸ø¶Ô·½½ðÇ®");
		return;
	}
	if( (uid - USER_BAND) == m_uid ) return;
	pUser = GetUser(uid - USER_BAND);
	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;
	if(pUser->m_bPShopOpen == TRUE || m_bPShopOpen == TRUE) return;

	// º¸ÅëÀ¯ÀúÀÏ °æ¿ì ÇÑ È­¸é, ¹öµðÀ¯ÀúÀÏ °æ¿ì µÎÈ­¸éÀ» ¹þ¾î³ª¸é ¾ÆÀÌÅÛÀÌ³ª µ·À» ÁÙ¼ö ¾ø´Ù. zinee 02/12/10
	// ´Ü ¹öµð¿øÀÌ °ø¼ºÁö¿ª¿¡ ÀÖÀ» °æ¿ì´Â º¸Åë¶§Ã³·³ ÇÑÈ­¸éÀ¸·Î Á¦ÇÑµÈ´Ù.
	bBuddyMember = CheckBuddyMember(pUser->m_strUserID);

	nMeMapType = CheckInvalidMapType();
	nYouMapType = pUser->CheckInvalidMapType();

	if( !bBuddyMember )
	{
		if(!IsInSight(pUser->m_curx, pUser->m_cury, pUser->m_curz)) return;			
	}
	else 
	if( (nMeMapType == 8 || nMeMapType == 10 || nMeMapType == 15)
		||(nMeMapType == 9 || nMeMapType == 11 || nMeMapType == 16)
		||(nYouMapType == 8 || nYouMapType == 10 || nYouMapType == 15)
		||(nYouMapType == 9 || nYouMapType == 11 || nYouMapType == 16) )
	{
			if( !CheckInvalidZoneState(nMeMapType) || !pUser->CheckInvalidZoneState(nYouMapType) )
			{
				if( m_curz != pUser->m_curz || !GetDistance( pUser->m_curx, pUser->m_cury, 2 ) )
					return;
			}

	}
	else if( m_curz != pUser->m_curz || !GetDistance( pUser->m_curx, pUser->m_cury, (SCREEN_X + SCREEN_Y) ) ) return;

	// ·Î¿­·³ºí °æ±âÀåÀÌ¶ó¸é
#ifndef _EVENT_RR 
	if( m_curz == g_RR.m_iZoneNum )
	{
		int metype = CheckInvalidMapType();
		int type = pUser->CheckInvalidMapType();

		if( metype != type ) return;
	}
#else		// ÀÓ½Ã·Î ÃÖ°­ ÀÌº¥Æ®¶§¹®¿¡
	if( m_curz == g_RR.m_iZoneNum || m_curz == 61 || m_curz == 62 || m_curz ==  63)
	{
		int metype = CheckInvalidMapType();
		int type = pUser->CheckInvalidMapType();

		if( metype != type ) return;
	}
#endif

	DWORD	dwDN = GetDWORD(pBuf, index);

	DWORD dwMyDN = 0, dwYourDN = 0, dwTempDN = 0;

	if(pUser->m_bRefuse) { error_code = 0x03; goto go_result; }
	if(dwDN > m_dwDN) { error_code = 0x04; goto go_result; }
	if(dwDN <= 0 || dwDN >= _MAX_DWORD) { error_code = 255; goto go_result; }

	dwYourDN = dwTempDN = pUser->m_dwDN;	// ¹é¾÷¿ë	
	dwMyDN = m_dwDN;

	if(!CheckMaxValueReturn(dwTempDN, dwDN))// ÃÖ´ë DWORD°ªÀ» ³Ñ¾ú´Ù.	
	{
		CheckMaxValue(dwTempDN, dwDN);		// ³ª¸ÓÁö´Â µÇµ¹·Á ÁØ´Ù.
		if( ((double)dwTempDN - (double)dwDN) < 0 ) return;

		dwDN = dwTempDN - dwDN;				// ½ÇÁ¦·Î ºüÁ® ³ª°£µ·
	}
	else dwTempDN += dwDN;					// ÁØ¸¸Å­ ´õÇÑ´Ù.

	//----------------[ ±Ý¾×À» º¯µ¿½ÃÅ²´Ù.]----------------------------------//
	pUser->m_dwDN = dwTempDN;				// ´õÇÏ°í

	if(m_dwDN <= dwDN) m_dwDN = 0;
	else		       m_dwDN -= dwDN;							// »©°í

	MakeMoneyLog( dwDN, ITEMLOG_MONEY_GIVE, pUser->m_strUserID, pUser->m_dwDN );

	// ----------------[ Update Databace ] ----------------------------------//
	if(UpdateUserExchangeData(pUser) == FALSE)
	{
		m_dwDN = dwMyDN;
		pUser->m_dwDN = dwYourDN;
		FlushItemLog( FALSE );
		return;
	}

	nFlag = FALSE;							// Á¶°Ç¸¦ ´Ù ¸¸Á·½ÃÄ×´Ù.
	FlushItemLog( TRUE );

go_result:
	if(nFlag)
	{
		CBufferEx TempBuf;

		BYTE result = FAIL;
		TempBuf.Add(ITEM_GIVE_RESULT);
		TempBuf.Add(result);
		TempBuf.Add(error_code);
		Send(TempBuf, TempBuf.GetLength());
		return ;
	}

	UpdateUserItemDN();
	SendMoneyChanged();
	pUser->SendMoneyChanged();	

	strMsg = _T("");
	strMsg.Format( IDS_USER_DINA_RECEIVE_FROM, m_strUserID, dwDN);//"%s ¸øÁËÄã %d ½ð±Ò."
	pUser->SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
}

/////////////////////////////////////////////////////////////////////////////////
//	¸øÎïÆ·¸øÍæ¼Ò
void USER::GiveItemToUser(TCHAR *pBuf)
{
	CBufferEx TempMyBuf, TempYourBuf;
	ItemList MyItem[TOTAL_ITEM_NUM], YourItem[TOTAL_ITEM_NUM];

	BOOL nFlag = TRUE;

	BOOL bBuddyMember;

	int  nMeMapType, nYouMapType;

	int iWeight = 0;
	int iSlot = -1;
	int i = 0, index = 0; 
	BYTE error_code = 0;
	int sSid = -1;
	BYTE tSlot = 0;
	short sCount = 0;

	CString strMsg, strMsg1;

	USER *pUser = NULL;

	int uid = GetInt(pBuf, index);

	if( (uid - USER_BAND) == m_uid ) return;

	pUser = GetUser(uid - USER_BAND);
	if(!m_MItemLock && f_Markini[0]->JYMM_KG == 1)
	{
		SendSysWhisperChat("¶Ô²»Æð!½»Ò×ÃÜÂëÉÐÎ´½â³ý,ÎÞ·¨¸ø¶Ô·½ÎïÆ·.");
		return;
	}
	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;
	if(pUser->m_bRefuse) { error_code = 0x03; goto go_result; }		

#ifdef _ACTIVE_USER
	if(m_iDisplayType == 6) return;			
	if(pUser->m_iDisplayType == 6) return; 
#endif

	tSlot = GetByte(pBuf, index);
	if(tSlot < EQUIP_ITEM_NUM || tSlot >= EQUIP_ITEM_NUM + INVENTORY_NUM) { error_code = 0x05; goto go_result; }	
	
	sCount = GetShort(pBuf, index);
	if(m_UserItem[tSlot].sCount < sCount || sCount <= 0) { error_code = 0x05; goto go_result; }	
	if(sCount > MAX_ITEM_NUM) { error_code = 0x02; goto go_result; }	

	sSid = m_UserItem[tSlot].sSid;
	if(sSid < 0 || sSid >= g_arItemTable.GetSize()) { error_code = 0x05; goto go_result; }

//-------------------------²é¿´×°±¸--------------------------------
	if(sSid != -1)
	{
		SendSystemMsg("ÇëÊ¹ÓÃ½»Ò×¸ø¶Ô·½ÎïÆ·.", SYSTEM_ERROR, TO_ME);
		error_code = 0x05; 
		goto go_result;
	}
//-----------------------------------------------------------------

	if(pUser->m_bNowTrading == TRUE || pUser->m_bNoItemMove == TRUE) return;

	bBuddyMember = CheckBuddyMember(pUser->m_strUserID);

	nMeMapType = CheckInvalidMapType();
	nYouMapType = pUser->CheckInvalidMapType();

	if( !bBuddyMember )
	{
		if(!IsInSight(pUser->m_curx, pUser->m_cury, pUser->m_curz)) return;			
	}
	else if(  (nMeMapType == 8 || nMeMapType == 10 || nMeMapType == 15)	
			||(nMeMapType == 9 || nMeMapType == 11 || nMeMapType == 16)
			||(nYouMapType == 8 || nYouMapType == 10 || nYouMapType == 15)
			||(nYouMapType == 9 || nYouMapType == 11 || nYouMapType == 16) )
	{
			if( !CheckInvalidZoneState(nMeMapType) || !pUser->CheckInvalidZoneState(nYouMapType) )
			{
				if( m_curz != pUser->m_curz || !GetDistance( pUser->m_curx, pUser->m_cury, 2 ) ) return;
			}

	}
	else if( m_curz != pUser->m_curz || !GetDistance( pUser->m_curx, pUser->m_cury, (SCREEN_X + SCREEN_Y) ) ) return;

#ifndef _EVENT_RR 
	if( m_curz == g_RR.m_iZoneNum )
	{
		int metype = CheckInvalidMapType();
		int type = pUser->CheckInvalidMapType();

		if( metype != type ) return;
	}
#else	
	if( m_curz == g_RR.m_iZoneNum || m_curz == 61 || m_curz == 62 || m_curz ==  63)
	{
		int metype = CheckInvalidMapType();
		int type = pUser->CheckInvalidMapType();

		if( metype != type ) return;
	}
#endif
	
	if( sSid == NORMAL_ITEM_REVERSE && pUser->m_bLive == USER_DEAD && bBuddyMember )
	{
		if( CheckInvalidZoneState(nMeMapType) && pUser->CheckInvalidZoneState(nYouMapType) )
		{
			TempMyBuf.Add(REVIVAL_REQ);
			TempMyBuf.AddString( m_strUserID );
			pUser->Send(TempMyBuf, TempMyBuf.GetLength());
			return;
		}
		else 
			SendSystemMsg(IDS_CANNOT_REVERSE, SYSTEM_NORMAL, TO_ME);
	}

	if( pUser->m_bLive == USER_DEAD ) return;

	if(g_arItemTable[sSid]->m_sEvent >= EVENT_UNIQUE_ITEM_BAND)
	{
		SendSystemMsg( IDS_USER_CANT_GIVE_EVENT_ITEM, SYSTEM_ERROR, TO_ME);
		error_code = 0x05;           //ÎÞ·¨¸øÓè¶Ô·½´ËÎïÆ·
		goto go_result;
	}
	if(g_arItemTable[sSid]->m_sDuration > 0 && m_UserItem[tSlot].sCount >= 2) { error_code = 255; goto go_result; }

	iWeight = g_arItemTable[sSid]->m_byWeight * sCount;
	if(pUser->m_iMaxWeight < pUser->m_iCurWeight + iWeight)
	{
		SendSystemMsg( IDS_USER_OVER_WEIGHT, SYSTEM_ERROR, TO_ME);
		error_code = 0x05; 
		goto go_result;
	}

	for(i = 0; i < TOTAL_ITEM_NUM; i++)
	{
		MyItem[i] = m_UserItem[i];	
		YourItem[i] = pUser->m_UserItem[i];
	}

	if(g_arItemTable[sSid]->m_sDuration > 0)
	{
		iSlot = pUser->GetEmptySlot(INVENTORY_SLOT);
		if(iSlot < 0) { error_code = 0x02; goto go_result; }	

		pUser->m_UserItem[iSlot].sLevel = m_UserItem[tSlot].sLevel;	
		pUser->m_UserItem[iSlot].sSid = m_UserItem[tSlot].sSid;		
		pUser->m_UserItem[iSlot].sCount = 1;
		pUser->m_UserItem[iSlot].sDuration = m_UserItem[tSlot].sDuration;
		pUser->m_UserItem[iSlot].sBullNum = m_UserItem[tSlot].sBullNum;
		pUser->m_UserItem[iSlot].tIQ = m_UserItem[tSlot].tIQ;

		for(i = 0; i < MAGIC_NUM; i++) pUser->m_UserItem[iSlot].tMagic[i] = m_UserItem[tSlot].tMagic[i];

		pUser->m_UserItem[iSlot].iItemSerial = m_UserItem[tSlot].iItemSerial;

		MakeItemLog( &(pUser->m_UserItem[iSlot]), ITEMLOG_GIVE, pUser->m_strUserID );
		ReSetItemSlot(&m_UserItem[tSlot]);								
	}
	else																
	{
		iSlot = pUser->GetSameItem(m_UserItem[tSlot], INVENTORY_SLOT);
		if(iSlot < 0)
		{
			iSlot = pUser->GetEmptySlot(INVENTORY_SLOT);		
			if(iSlot < 0) { error_code = 0x02; goto go_result; }	

			pUser->m_UserItem[iSlot].sLevel = m_UserItem[tSlot].sLevel;	
			pUser->m_UserItem[iSlot].sSid = m_UserItem[tSlot].sSid;	
			pUser->m_UserItem[iSlot].sCount = sCount;			
			pUser->m_UserItem[iSlot].sDuration = m_UserItem[tSlot].sDuration;
			pUser->m_UserItem[iSlot].sBullNum = m_UserItem[tSlot].sBullNum;
			pUser->m_UserItem[iSlot].tIQ = m_UserItem[tSlot].tIQ;

			for(i = 0; i < MAGIC_NUM; i++) pUser->m_UserItem[iSlot].tMagic[i] = m_UserItem[tSlot].tMagic[i];

			pUser->m_UserItem[iSlot].iItemSerial = m_UserItem[tSlot].iItemSerial;
		}
		else
		{
			pUser->m_UserItem[iSlot].sCount += sCount;			
		}

		if((m_UserItem[tSlot].sCount - sCount) <= 0)					
		{																
			ReSetItemSlot(&m_UserItem[tSlot]);
		}
		else m_UserItem[tSlot].sCount -= sCount;

	}
	
	if(UpdateUserExchangeData(pUser) == FALSE)//¸øÓè¶Ô·½ÎïÆ·
	{
		for(i = 0; i < TOTAL_ITEM_NUM; i++)
		{
			m_UserItem[i] = MyItem[i];
			pUser->m_UserItem[i] = YourItem[i];
		}
		FlushItemLog( FALSE );
		return;
	}

	nFlag = FALSE;

go_result:
	BYTE result = SUCCESS;
	TempMyBuf.Add(ITEM_GIVE_RESULT);

	if(nFlag)
	{
		result = FAIL;
		TempMyBuf.Add(result);
		TempMyBuf.Add(error_code);
		Send(TempMyBuf, TempMyBuf.GetLength());
		FlushItemLog( FALSE );
		return ;
	}

	FlushItemLog( TRUE );

	m_iCurWeight -= iWeight;
	if(m_iCurWeight < 0) m_iCurWeight = 0;

	pUser->m_iCurWeight += iWeight;

	UpdateUserItemDN();								
	GetRecoverySpeed();							
	pUser->GetRecoverySpeed();

	TempMyBuf.Add(result);							
	TempMyBuf.Add(tSlot);
	TempMyBuf.Add(m_UserItem[tSlot].sLevel);
	TempMyBuf.Add(m_UserItem[tSlot].sSid);
	TempMyBuf.Add(m_UserItem[tSlot].sDuration);
	TempMyBuf.Add(m_UserItem[tSlot].sBullNum);
	TempMyBuf.Add(m_UserItem[tSlot].sCount);
	for(i = 0; i < MAGIC_NUM; i++) TempMyBuf.Add(m_UserItem[tSlot].tMagic[i]);
	TempMyBuf.Add(m_UserItem[tSlot].tIQ);

	Send(TempMyBuf, TempMyBuf.GetLength());

													
	TempYourBuf.Add(ITEM_GIVE_RESULT);
	TempYourBuf.Add(result);
	TempYourBuf.Add((BYTE)iSlot);
	TempYourBuf.Add(pUser->m_UserItem[iSlot].sLevel);
	TempYourBuf.Add(pUser->m_UserItem[iSlot].sSid);
	TempYourBuf.Add(pUser->m_UserItem[iSlot].sDuration);
	TempYourBuf.Add(pUser->m_UserItem[iSlot].sBullNum);
	TempYourBuf.Add(pUser->m_UserItem[iSlot].sCount);
	for(i = 0; i < MAGIC_NUM; i++) TempYourBuf.Add(pUser->m_UserItem[iSlot].tMagic[i]);
	TempYourBuf.Add(pUser->m_UserItem[iSlot].tIQ);

	strMsg = _T("");
	strMsg.Format( IDS_USER_ITEM_RECEIVE_FROM, m_strUserID,g_arItemTable[sSid]->m_strName); 
	pUser->SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);

	pUser->Send(TempYourBuf, TempYourBuf.GetLength());

	strMsg1 = _T("");
	strMsg1.Format( IDS_USER_ITEM_GIVE_TO, g_arItemTable[sSid]->m_strName,pUser->m_strUserID); 
	SendSystemMsg((LPTSTR)(LPCTSTR)strMsg1, SYSTEM_NORMAL, TO_ME);
}

/////////////////////////////////////////////////////////////////////////////////
//	ÈÔÆú½ðÇ®ÇëÇó
void USER::GiveMoneyToMap(TCHAR *pBuf)
{
	CString strMsg;
	int		index = 0;
	long	lUsed = 0;
	CPoint	ptCell;
	int		iIsItem = 11;	// ¼¿¿¡ °°Àº ÁÂÇ¥ µî·Ï¹æÁö

	BOOL nFlag = TRUE;

	ItemList *pMapItem = NULL;

	DWORD dwMyDN = 0;
	BYTE error_code = 0;

	short sWill_X = GetShort(pBuf, index);
	short sWill_Y = GetShort(pBuf, index);

	DWORD	dwDN = GetDWORD(pBuf, index);

	if(!m_MItemLock && f_Markini[0]->JYMM_KG == 1)
	{
		SendSysWhisperChat("¶Ô²»Æð!½»Ò×ÃÜÂëÉÐÎ´½â³ý,ÎÞ·¨¶ªÆú½ðÇ®.");
		return;
	}

	if(m_tGuildHouseWar == GUILD_WARRING || m_tQuestWar == GUILD_WARRING) 
	{
		SendSystemMsg( IDS_USER_CANT_THROW_IN_QUEST, SYSTEM_ERROR, TO_ME);//"ÎÞ·¨¸øÓèÈÎÎñÎïÆ·."
		return;
	}

	ptCell = ConvertToServer( sWill_X, sWill_Y );
	if(ptCell.x < 0 || ptCell.y < 0) { error_code = 0x04; goto go_result; }

	ptCell = FindNearRandomPoint(ptCell.x, ptCell.y);							// ÇöÀç ÀÚ±âÁÂÇ¥¸¦ Á¦¿ÜÇÑ 24Ä­
	if(ptCell.x < 0 || ptCell.y < 0) { error_code = 0x04; goto go_result; }
	if(ptCell.x >= g_zone[m_ZoneIndex]->m_sizeMap.cx || ptCell.y >= g_zone[m_ZoneIndex]->m_sizeMap.cy) { error_code = 0x04; goto go_result; }

	if(dwDN > m_dwDN) { error_code = 0x04; goto go_result; }
	if(dwDN <= 0 || dwDN >= _MAX_DWORD) { error_code = 255; goto go_result; }

	// ÇöÀç ÀÖ´Â Á¸ÀÌ ·Î¿­·³ºí ¸ÊÀÌ¶ó¸é
#ifndef _EVENT_RR 
	if( m_curz == g_RR.m_iZoneNum )
	{
		int nRet = ((g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_dwType & 0xFF00 ) >> 8);
		int mytype = CheckInvalidMapType();

		if( nRet != mytype ) { error_code = 0x04; goto go_result; }
	}
#else		// ÀÓ½Ã·Î ÃÖ°­ ÀÌº¥Æ®¶§¹®¿¡
	if( m_curz == g_RR.m_iZoneNum || m_curz == 61 || m_curz == 62 || m_curz ==  63)
	{
		int nRet = ((g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_dwType & 0xFF00 ) >> 8);
		int mytype = CheckInvalidMapType();

		if( nRet != mytype ) { error_code = 0x04; goto go_result; }
	}
#endif

	if(InterlockedCompareExchange((LONG*)&g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_FieldUse, (long)1, (long)0) == (long)0)
	{
		pMapItem = new ItemList;

		if(pMapItem != NULL)											// ¸Þ¸ð¸®°¡ Á¦´ë·Î ÇÒ´ç µÇ¾ú´Ù¸é
		{																		
			pMapItem->tType = TYPE_MONEY;
			pMapItem->dwMoney = 0;

			dwMyDN = m_dwDN; 

			if(m_dwDN <= dwDN) m_dwDN = 0;
			else m_dwDN -= dwDN; 

			MakeMoneyLog( dwDN, ITEMLOG_MONEY_THROW );

			CheckMaxValue((DWORD &)pMapItem->dwMoney, (DWORD)dwDN);
			// ----------------[ Update Databace ] ----------------------------------//
//			if(UpdateUserData() == FALSE)
			if(UpdateUserItemDN() == FALSE)
			{
				m_dwDN = dwMyDN;
				::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_FieldUse, 0);

				error_code = 255; goto go_result;
			}

			nFlag = FALSE;

			FlushItemLog( TRUE );

			//m_pCom->DelThrowItem();
			if(f_Markini[0]->DQXSDM_KG == 1 || m_tIsOP == 1)
			{
				m_pCom->SetThrowItem( pMapItem, ptCell.x, ptCell.y, m_ZoneIndex );//¶ªµØÉÏµÄÓÎÏ·±Ò/½ð±ÒÏÔÊ¾ÔÚµØÉÏ
			}
				
		}
		else error_code = 255;

		::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_FieldUse, 0);
	}

go_result:
	if(nFlag)
	{
		CBufferEx TempBuf;

		BYTE result = FAIL;
		TempBuf.Add(ITEM_GIVE_RESULT);
		TempBuf.Add(result);
		TempBuf.Add(error_code);
		Send(TempBuf, TempBuf.GetLength());

		if(pMapItem) delete pMapItem;

		FlushItemLog( FALSE );

		return ;
	}

	UpdateUserItemDN();								// ¾ÆÀÌÅÛ º¯µ¿ Ã¼Å©...
	SendMoneyChanged();
}

/////////////////////////////////////////////////////////////////////////////////
//	ÈÔÆúÎïÆ·ÇëÇó
void USER::GiveItemToMap(TCHAR *pBuf, BOOL bChao)
{
	CString strMsg;
	CBufferEx TempBuf;
	ItemList MyItem[TOTAL_ITEM_NUM];	// ÀÎº¥ ¹é¾÷¿ë 

	BOOL nFlag = TRUE;
//	BOOL bQuickChange = FALSE;

	int i, index = 0; 
	BYTE error_code = 0;

	CPoint	ptCell;
	int sSid = -1;

	short sWill_X = GetShort(pBuf, index);
	short sWill_Y = GetShort(pBuf, index);

	BYTE tSlot = GetByte(pBuf, index);
	short sCount = GetShort(pBuf, index);

	ItemList *pMapItem = NULL;
	if(!m_MItemLock && f_Markini[0]->JYMM_KG == 1)
	{
		SendSysWhisperChat("¶Ô²»Æð!½»Ò×ÃÜÂëÉÐÎ´½â³ý,ÎÞ·¨¶ªÆúÎïÆ·.");
		return;
	}
	if(m_tGuildHouseWar == GUILD_WARRING || m_tQuestWar == GUILD_WARRING) 
	{
		SendSystemMsg( IDS_USER_CANT_THROW_IN_QUEST, SYSTEM_ERROR, TO_ME);//"ÎÞ·¨¸øÓèÈÎÎñÎïÆ·."
		return;
	}
	// ³» ÀÎº¥¿¡ ¾ø´Ù.
	if(tSlot < 0 || tSlot >= TOTAL_ITEM_NUM) { error_code = 0x05; goto go_result; }
	if( !bChao && (tSlot < EQUIP_ITEM_NUM || tSlot >= TOTAL_INVEN_MAX) ) { error_code = 0x05; goto go_result; }

	sSid = m_UserItem[tSlot].sSid;
	if(sSid < 0 || sSid >= g_arItemTable.GetSize()) { error_code = 0x05; goto go_result; }

	if(g_arItemTable[sSid]->m_sEvent >= EVENT_UNIQUE_ITEM_BAND)
	{
		SendSystemMsg( IDS_USER_CANT_THROW_EVENT_ITEM, SYSTEM_ERROR, TO_ME);//"ÎÞ·¨¶ªÆúÈÎÎñÎïÆ·."
		error_code = 0x05; goto go_result;
	}
	if(g_arItemTable[sSid]->m_sSid == 1121)//Ð¡ºÚ¸±ÊÖÎÞ·¨¶ªÆú
	{
		SendSystemMsg( "¡¾¶ñÄ§Ö®Òí¡¿ÎÞ·¨¶ªÆú!", SYSTEM_ERROR, TO_ME);
		error_code = 0x05; goto go_result;
	}
	if(m_UserItem[tSlot].sCount < sCount || sCount <= 0) { error_code = 0x05; goto go_result; }
	ptCell = ConvertToServer( sWill_X, sWill_Y );
	ptCell = FindNearRandomPoint(ptCell.x, ptCell.y);							// ÇöÀç ÀÚ±âÁÂÇ¥¸¦ Á¦¿ÜÇÑ 24Ä­
	if(ptCell.x < 0 || ptCell.y < 0) { error_code = 0x04; goto go_result; }
	if(ptCell.x >= g_zone[m_ZoneIndex]->m_sizeMap.cx || ptCell.y >= g_zone[m_ZoneIndex]->m_sizeMap.cy) { error_code = 0x04; goto go_result; }
	// ÇöÀç ÀÖ´Â Á¸ÀÌ ·Î¿­·³ºí ¸ÊÀÌ¶ó¸é
#ifndef _EVENT_RR 
	if( m_curz == g_RR.m_iZoneNum )
	{
		int nRet = ((g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_dwType & 0xFF00 ) >> 8);
		int mytype = CheckInvalidMapType();

		if( nRet != mytype ) { error_code = 0x04; goto go_result; }
	}
#else		// ÀÓ½Ã·Î ÃÖ°­ ÀÌº¥Æ®¶§¹®¿¡
	if( m_curz == g_RR.m_iZoneNum || m_curz == 61 || m_curz == 62 || m_curz ==  63)
	{
		int nRet = ((g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_dwType & 0xFF00 ) >> 8);
		int mytype = CheckInvalidMapType();

		if( nRet != mytype ) { error_code = 0x04; goto go_result; }
	}
#endif

	for(i = 0; i < TOTAL_ITEM_NUM; i++)// ¾ÆÀÌÅÛ Á¤º¸ ¹é¾÷
	{
		MyItem[i] = m_UserItem[i];		
	}

	if(InterlockedCompareExchange((LONG*)&g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_FieldUse, (long)1, (long)0) == (long)0)
	{
		pMapItem = new ItemList;

		if(pMapItem != NULL)											// ¸Þ¸ð¸®°¡ Á¦´ë·Î ÇÒ´ç µÇ¾ú´Ù¸é
		{
			pMapItem->tType = TYPE_ITEM;
			pMapItem->sLevel = m_UserItem[tSlot].sLevel;
			pMapItem->sSid = m_UserItem[tSlot].sSid;
			pMapItem->sDuration = m_UserItem[tSlot].sDuration;
			pMapItem->sCount = sCount;
			pMapItem->sBullNum = m_UserItem[tSlot].sBullNum;
			pMapItem->tIQ = m_UserItem[tSlot].tIQ;
			for(i = 0; i < MAGIC_NUM; i++) pMapItem->tMagic[i] = m_UserItem[tSlot].tMagic[i];
			pMapItem->iItemSerial = m_UserItem[tSlot].iItemSerial;

			if(g_arItemTable[sSid]->m_sDuration <= 0)
			{
				if((m_UserItem[tSlot].sCount - sCount) <= 0) 
				{ 
					// ¾ÆÀÌÅÛ ·Î±×¸¦ ³²°Ü¾ß ÇÒ°Í °°±ä ÇÑµ¥... °ãÄ¡´Â ¾ÆÀÌÅÛÀÌ¶ó, ÁÖ¼®Ã³¸®ÇÔ
					//if( !bChao )	MakeItemLog( pMapItem, ITEMLOG_THROW );
					//else			MakeItemLog( pMapItem, ITEMLOG_DEAD_THROW );
					ReSetItemSlot(&m_UserItem[tSlot]);
				}
				else m_UserItem[tSlot].sCount -= sCount;
			}
			else
			{
				if( !bChao )	MakeItemLog( pMapItem, ITEMLOG_THROW );
				else			MakeItemLog( pMapItem, ITEMLOG_DEAD_THROW );
				ReSetItemSlot(&m_UserItem[tSlot]);
			}

			// ----------------[ Update Databace ] ----------------------------------//		
//			if(UpdateUserData() == FALSE)
			if(UpdateUserItemDN() == FALSE)
			{
				for(i = 0; i < TOTAL_ITEM_NUM; i++)//¾÷µ¥ÀÌÆ®°¡ ½ÇÆÐÇÏ¸é ºüÁ®³ª¿Â´Ù.
				{
					m_UserItem[i] = MyItem[i];
				}
				::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_FieldUse, 0);

				error_code = 255; 
				goto go_result; 
			}

			//m_pCom->DelThrowItem();
			if(f_Markini[0]->DQXSDM_KG == 1 || m_tIsOP == 1)
			{
				m_pCom->SetThrowItem( pMapItem, ptCell.x, ptCell.y, m_ZoneIndex );//¶ªµØÉÏµÄ¶«Î÷/ÎïÆ·ÏÔÊ¾ÔÚµØÉÏ
			}

			nFlag = FALSE;
		}
		else error_code = 255;

		::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_FieldUse, 0);
	}
go_result:
	BYTE result = SUCCESS;
	TempBuf.Add(ITEM_GIVE_RESULT);
	if(!pMapItem) nFlag = TRUE;
	if(nFlag)
	{
		result = FAIL;
		TempBuf.Add(result);
		TempBuf.Add(error_code);
		Send(TempBuf, TempBuf.GetLength());
		if(pMapItem) delete pMapItem;
		FlushItemLog( FALSE );
		return;
	}
	FlushItemLog( TRUE );
	if(pMapItem->sSid < 0 || pMapItem->sSid >= g_arItemTable.GetSize()) return;//ÐÞ¸Ä MarkJay
	m_iCurWeight -= g_arItemTable[pMapItem->sSid]->m_byWeight * sCount;
	if(m_iCurWeight < 0) m_iCurWeight = 0;
	GetRecoverySpeed();			// ´Ù½Ã È¸º¹¼Óµµ¸¦ °è»ê
	TempBuf.Add(result);
	TempBuf.Add(tSlot);
	TempBuf.Add(m_UserItem[tSlot].sLevel);
	TempBuf.Add(m_UserItem[tSlot].sSid);
	TempBuf.Add(m_UserItem[tSlot].sDuration);
	TempBuf.Add(m_UserItem[tSlot].sBullNum);
	TempBuf.Add(m_UserItem[tSlot].sCount);
	for(int j = 0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[tSlot].tMagic[j]);
	TempBuf.Add(m_UserItem[tSlot].tIQ);
	Send(TempBuf, TempBuf.GetLength());
}
///////////////////////////////////////////////////////////////////////////////
//PKµôÂäÏÔÊ¾
void USER::PKGiveItemToMap(TCHAR *pBuf, BOOL bChao)
{
	CBufferEx TempBuf;
	ItemList MyItem[TOTAL_ITEM_NUM];	// ÀÎº¥ ¹é¾÷¿ë 

	BOOL nFlag = TRUE;
//	BOOL bQuickChange = FALSE;

	int i, index = 0; 
	BYTE error_code = 0;

	CPoint	ptCell;
	int sSid = -1;

	short sWill_X = GetShort(pBuf, index);
	short sWill_Y = GetShort(pBuf, index);

	BYTE tSlot = GetByte(pBuf, index);
	short sCount = GetShort(pBuf, index);

	ItemList *pMapItem = NULL;

	if(m_tGuildHouseWar == GUILD_WARRING || m_tQuestWar == GUILD_WARRING) 
	{
		SendSystemMsg( IDS_USER_CANT_THROW_IN_QUEST, SYSTEM_ERROR, TO_ME);//"ÎÞ·¨¸øÓèÈÎÎñÎïÆ·."
		return;
	}

	// ³» ÀÎº¥¿¡ ¾ø´Ù.
	if(tSlot < 0 || tSlot >= TOTAL_ITEM_NUM) { error_code = 0x05; goto go_result; }
	if( !bChao && (tSlot < EQUIP_ITEM_NUM || tSlot >= TOTAL_INVEN_MAX) ) { error_code = 0x05; goto go_result; }

	sSid = m_UserItem[tSlot].sSid;
	if(sSid < 0 || sSid >= g_arItemTable.GetSize()) { error_code = 0x05; goto go_result; }

	if(g_arItemTable[sSid]->m_sEvent >= EVENT_UNIQUE_ITEM_BAND)
	{
		SendSystemMsg( IDS_USER_CANT_THROW_EVENT_ITEM, SYSTEM_ERROR, TO_ME);//"ÎÞ·¨¶ªÆúÈÎÎñÎïÆ·."
		error_code = 0x05; goto go_result;
	}
	if(g_arItemTable[sSid]->m_sSid == 1121)//Ð¡ºÚ¸±ÊÖÎÞ·¨¶ªÆú
	{
		SendSystemMsg( "¡¾¶ñÄ§Ö®Òí¡¿ÎÞ·¨¶ªÆú!", SYSTEM_ERROR, TO_ME);
		error_code = 0x05; goto go_result;
	}
	if(m_UserItem[tSlot].sCount < sCount || sCount <= 0) { error_code = 0x05; goto go_result; }

	ptCell = ConvertToServer( sWill_X, sWill_Y );
	ptCell = FindNearRandomPoint(ptCell.x, ptCell.y);							// ÇöÀç ÀÚ±âÁÂÇ¥¸¦ Á¦¿ÜÇÑ 24Ä­
	if(ptCell.x < 0 || ptCell.y < 0) { error_code = 0x04; goto go_result; }
	if(ptCell.x >= g_zone[m_ZoneIndex]->m_sizeMap.cx || ptCell.y >= g_zone[m_ZoneIndex]->m_sizeMap.cy) { error_code = 0x04; goto go_result; }

	// ÇöÀç ÀÖ´Â Á¸ÀÌ ·Î¿­·³ºí ¸ÊÀÌ¶ó¸é
#ifndef _EVENT_RR 
	if( m_curz == g_RR.m_iZoneNum )
	{
		int nRet = ((g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_dwType & 0xFF00 ) >> 8);
		int mytype = CheckInvalidMapType();

		if( nRet != mytype ) { error_code = 0x04; goto go_result; }
	}
#else		// ÀÓ½Ã·Î ÃÖ°­ ÀÌº¥Æ®¶§¹®¿¡
	if( m_curz == g_RR.m_iZoneNum || m_curz == 61 || m_curz == 62 || m_curz ==  63)
	{
		int nRet = ((g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_dwType & 0xFF00 ) >> 8);
		int mytype = CheckInvalidMapType();

		if( nRet != mytype ) { error_code = 0x04; goto go_result; }
	}
#endif

	for(i = 0; i < TOTAL_ITEM_NUM; i++)// ¾ÆÀÌÅÛ Á¤º¸ ¹é¾÷
	{
		MyItem[i] = m_UserItem[i];		
	}

	if(InterlockedCompareExchange((LONG*)&g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_FieldUse, (long)1, (long)0) == (long)0)
	{
		pMapItem = new ItemList;

		if(pMapItem != NULL)											// ¸Þ¸ð¸®°¡ Á¦´ë·Î ÇÒ´ç µÇ¾ú´Ù¸é
		{
			pMapItem->tType = TYPE_ITEM;
			pMapItem->sLevel = m_UserItem[tSlot].sLevel;
			pMapItem->sSid = m_UserItem[tSlot].sSid;
			pMapItem->sDuration = m_UserItem[tSlot].sDuration;
			pMapItem->sCount = sCount;
			pMapItem->sBullNum = m_UserItem[tSlot].sBullNum;
			pMapItem->tIQ = m_UserItem[tSlot].tIQ;
			for(i = 0; i < MAGIC_NUM; i++) pMapItem->tMagic[i] = m_UserItem[tSlot].tMagic[i];
			pMapItem->iItemSerial = m_UserItem[tSlot].iItemSerial;

			if(g_arItemTable[sSid]->m_sDuration <= 0)
			{
				if((m_UserItem[tSlot].sCount - sCount) <= 0) 
				{ 
					// ¾ÆÀÌÅÛ ·Î±×¸¦ ³²°Ü¾ß ÇÒ°Í °°±ä ÇÑµ¥... °ãÄ¡´Â ¾ÆÀÌÅÛÀÌ¶ó, ÁÖ¼®Ã³¸®ÇÔ
					//if( !bChao )	MakeItemLog( pMapItem, ITEMLOG_THROW );
					//else			MakeItemLog( pMapItem, ITEMLOG_DEAD_THROW );
					ReSetItemSlot(&m_UserItem[tSlot]);
				}
				else m_UserItem[tSlot].sCount -= sCount;
			}
			else
			{
				if( !bChao )	MakeItemLog( pMapItem, ITEMLOG_THROW );
				else			MakeItemLog( pMapItem, ITEMLOG_DEAD_THROW );
				ReSetItemSlot(&m_UserItem[tSlot]);
			}

			// ----------------[ Update Databace ] ----------------------------------//		
//			if(UpdateUserData() == FALSE)
			if(UpdateUserItemDN() == FALSE)
			{
				for(i = 0; i < TOTAL_ITEM_NUM; i++)//¾÷µ¥ÀÌÆ®°¡ ½ÇÆÐÇÏ¸é ºüÁ®³ª¿Â´Ù.
				{
					m_UserItem[i] = MyItem[i];
				}
				::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_FieldUse, 0);

				error_code = 255; 
				goto go_result; 
			}

			//m_pCom->DelThrowItem();
			if(f_Markini[0]->PKDLXS_KG == 1)//PKµôÂäÎïÆ·ÏÔÊ¾
			{
				m_pCom->SetThrowItem( pMapItem, ptCell.x, ptCell.y, m_ZoneIndex );//PKµôÂäÏÔÊ¾
			}

			nFlag = FALSE;
		}
		else error_code = 255;

		::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_FieldUse, 0);
	}

go_result:
	BYTE result = SUCCESS;
	TempBuf.Add(ITEM_GIVE_RESULT);

	if(!pMapItem) nFlag = TRUE;

	if(nFlag)
	{
		result = FAIL;
		TempBuf.Add(result);
		TempBuf.Add(error_code);
		Send(TempBuf, TempBuf.GetLength());

		if(pMapItem) delete pMapItem;

		FlushItemLog( FALSE );
		return;
	}

	FlushItemLog( TRUE );

	if(pMapItem->sSid < 0 || pMapItem->sSid >= g_arItemTable.GetSize()) return;//ÐÞ¸Ä MarkJay

	m_iCurWeight -= g_arItemTable[pMapItem->sSid]->m_byWeight * sCount;
	if(m_iCurWeight < 0) m_iCurWeight = 0;

	GetRecoverySpeed();			// ´Ù½Ã È¸º¹¼Óµµ¸¦ °è»ê

	TempBuf.Add(result);
	TempBuf.Add(tSlot);
	TempBuf.Add(m_UserItem[tSlot].sLevel);
	TempBuf.Add(m_UserItem[tSlot].sSid);
	TempBuf.Add(m_UserItem[tSlot].sDuration);
	TempBuf.Add(m_UserItem[tSlot].sBullNum);
	TempBuf.Add(m_UserItem[tSlot].sCount);
	for(int j = 0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[tSlot].tMagic[j]);

	TempBuf.Add(m_UserItem[tSlot].tIQ);

	Send(TempBuf, TempBuf.GetLength());
}
/////////////////////////////////////////////////////////////////////////////////
//	×°ÔØÎïÆ·ÇëÇó
void USER::ItemLoadReq(TCHAR *pBuf)
{
#ifdef _ACTIVE_USER
	if(m_iDisplayType == 6) return; //yskang 0.5
#endif

	if( m_ZoneIndex < 0 || m_ZoneIndex >= g_zone.GetSize() ) return;

	BYTE tType;				// 1 : ¸÷
							// 2 : ¸Ê
	int i, j;
	short sX = 0, sY = 0;
	short sSid = -1;

	MAP* pMap = g_zone[m_ZoneIndex];
	if(!pMap) return;

	ItemList* pItem = NULL;

	int iWeight = 0;
	int mapIndex = -1;
	short Diff = 0;
	int index = 0;

	BYTE	error_code = UNKNOWN_ERR;

	DWORD dwDN = 0, dwMyDN = 0;

	BYTE result = SUCCESS;
	BYTE tRight = FAIL;

	CBufferEx TempBuf, TempBuf2;

	ItemList MyItem[TOTAL_ITEM_NUM];	// ÀÎº¥ ¹é¾÷¿ë 

	dwMyDN = m_dwDN;					// µ· Á¤º¸ ¹é¾÷
	for(i = 0; i < TOTAL_ITEM_NUM; i++)
	{
		MyItem[i] = m_UserItem[i];		
	}

	tType = GetByte(pBuf, index);

	TempBuf.Add(ITEM_LOAD_RESULT);			// ÆÐÅ¶ÀÇ ½ÃÀÛ

	if(tType == 2)						//@@@@@@@@@@@ ºÒÇÊ¿äÇÑ ºÎºÐ...
	{
		sX = GetShort(pBuf, index);
		sY = GetShort(pBuf, index);

		CPoint ptCell = ConvertToServer( sX, sY );

		if(ptCell.x < 0 || ptCell.y < 0) { error_code = ERR_2; goto go_result; }
		if(ptCell.x >= pMap->m_sizeMap.cx || ptCell.y >= pMap->m_sizeMap.cy) { error_code = ERR_2; goto go_result; }

		if(GetDistance(ptCell.x, ptCell.y, 2) == FALSE) { error_code = ERR_1; goto go_result; }
				
		mapIndex = pMap->m_pMap[ptCell.x][ptCell.y].iIndex;
		if(mapIndex < 0) { error_code = ERR_6; goto go_result; }
		
		// IKING 2001.1.
		EnterCriticalSection( &m_pCom->m_critThrowItem );
		if ( m_pCom->m_ThrowItemArray[mapIndex] == NULL )
		{
			LeaveCriticalSection( &m_pCom->m_critThrowItem );
			error_code = ERR_6;
			goto go_result;
		}
		pItem = m_pCom->m_ThrowItemArray[mapIndex]->m_pItem;
		LeaveCriticalSection( &m_pCom->m_critThrowItem );
		//

		if( !pItem )
		{
			pMap->m_pMap[ptCell.x][ptCell.y].iIndex = -1;
			error_code = ERR_6;
			goto go_result;
		}

		//------------------------------------------------------------------------
		//	¾ÆÀÌÅÛ¿¡´ëÇÑ À¯ÀúÀÇ ±Ç¸®¸¦ º¸ÀåÇÑ´Ù.
		if( pItem->dwTime > 0 )
		{
			BYTE tNum = 0;
			DWORD curr = ConvertCurTimeToSaveTime();

			if( (curr - pItem->dwTime) <= 62 )
			{
				for(i = 0; i < ITEM_USER_RIGHT_NUM; i++)
				{
					if( pItem->uid[i] >= 0 && pItem->uid[i] + USER_BAND < NPC_BAND )
					{
						if(m_uid == pItem->uid[i]) { tRight = SUCCESS; break; }
					}
					else if( pItem->uid[i] <= -1 ) tNum += 1;
				}

				if(tRight)
				{
					int iRandom = myrand(0, 100);
					if(iRandom > pItem->SuccessRate[i])
					{
						error_code = ERR_6;
						goto go_result;
					}
				}
				else if( tNum != 3 ) 
				{
					error_code = ERR_3;
					goto go_result;
				}
			}
		}
/*		COleDateTime checkTime = COleDateTime(pItem->ThrowTime);
		
		if(checkTime.m_status == COleDateTime::valid)
		{
			CTime cur = CTime::GetCurrentTime();
			CTime last(pItem->ThrowTime);
			CTimeSpan DiffTime;
			DiffTime = cur - last;
			
			if( DiffTime.GetMinutes() <= 1)
			{
				for(i = 0; i < ITEM_USER_RIGHT_NUM; i++)
				{
					if( pItem->uid[i] >= 0 && pItem->uid[i] + USER_BAND < NPC_BAND )
					{
						if(m_uid == pItem->uid[i]) { tRight = SUCCESS; break; }
					}
				}

				if(tRight)
				{
					int iRandom = myrand(0, 100);
					if(iRandom > pItem->SuccessRate[i])
					{
						error_code = ERR_6;
						goto go_result;
					}
				}
				else
				{
					error_code = ERR_3;
					goto go_result;
				}
			}
		}
*/
		tType = pItem->tType;
		if(pItem->tType == TYPE_MONEY)
		{
			if(pItem->dwMoney <= 0 || pItem->dwMoney == _MAX_DWORD) goto go_result;
			if(m_dwDN == _MAX_DWORD) goto go_result;
		}
		else if(pItem->tType == TYPE_ITEM)
		{
			sSid = pItem->sSid;		// µî·ÏµÇÁö ¾ÊÀº ¾ÆÀÌÅÛ
			if(sSid < 0 || sSid >= g_arItemTable.GetSize()) { error_code = ERR_6; goto go_result; }
			if(pItem->sCount < 0 || pItem->sCount > _MAX_SHORT) { error_code = ERR_6; goto go_result; }
//			if(g_arItemTable[sSid]->m_byWear < 6 && pItem->sCount >= 2) goto go_result;	// ³»±¸µµÀÖ´Â°ÍÀº ¿ÀÁ÷ ÇÏ³ª
			if(g_arItemTable[sSid]->m_sDuration > 0 && pItem->sCount >= 2) { error_code = ERR_6; goto go_result; }	// ³»±¸µµÀÖ´Â°ÍÀº ¿ÀÁ÷ ÇÏ³ª
			
/*			iWeight = g_arItemTable[sSid]->m_byWeight * pItem->sCount;
			if(m_iMaxWeight <= m_iCurWeight + iWeight) 
			{
				SendSystemMsg( IDS_USER_OVER_WEIGHT, SYSTEM_ERROR, TO_ME);//"ÄãµÄ°ü¹üÒÑ³¬¸ºÖØ."
				goto go_result;
			}
*/		}
		else goto go_result;

		if(InterlockedCompareExchange((LONG*)&g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_FieldUse,(long)1, (long)0) == (long)0)
		{
			if(pItem->tType == TYPE_MONEY)
			{
				if(!CheckMaxValueReturn(m_dwDN, pItem->dwMoney))//²âÊÔÐÞ¸´µô¶Ë
				{
					DWORD iDN = 0;
					CheckMaxValue(m_dwDN, pItem->dwMoney);

					iDN = m_dwDN;
					dwDN = (DWORD)abs((int)(iDN - pItem->dwMoney));
				}

				CheckMaxValue(m_dwDN, pItem->dwMoney);

				MakeMoneyLog( pItem->dwMoney, ITEMLOG_MONEY_PICKUP );
			}
			else
			{
				index = GetSameItem(*pItem, INVENTORY_SLOT);

				if(index != -1)
				{					
					for(i = 1; i < pItem->sCount + 1; i++)
					{
						iWeight = g_arItemTable[pItem->sSid]->m_byWeight * i;
						if(m_iMaxWeight < m_iCurWeight + iWeight) 
						{
							iWeight = g_arItemTable[pItem->sSid]->m_byWeight * (i - 1);
							if(iWeight == 0)
							{
//								SendSystemMsg( IDS_USER_ITEM_OVERFLOW, SYSTEM_ERROR, TO_ME);
								error_code = ERR_4;
								::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_FieldUse, 0); 			
								goto go_result;
							}
							Diff = i - 1;
							break;
/*							if(pItem->sCount >= i && iWeight > 0)
							{
								Diff = pItem->sCount - i; // ¸Ê¿¡ ³²°ÜÁú °¹¼ö¸¦ ±¸ÇÔ
								pItem->sCount -= i;
								break;
							}
*/						}
						Diff = i;
					}

					if(m_iMaxWeight >= m_iCurWeight + iWeight) 
					{
						if(pItem->sCount > Diff && Diff >= 0)
						{
							Diff = pItem->sCount - Diff; // ¸Ê¿¡ ³²°ÜÁú °¹¼ö¸¦ ±¸ÇÔ
							pItem->sCount -= Diff;
						}
						else Diff = 0;

						CheckMaxValue((short &)m_UserItem[index].sCount, pItem->sCount);
					}
					else
					{
//						SendSystemMsg(IDS_USER_ITEM_OVERFLOW, SYSTEM_ERROR, TO_ME);
						error_code = ERR_4;
						::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_FieldUse, 0); 			
						goto go_result;
					}
/*					Diff = MAX_ITEM_NUM - m_UserItem[index].sCount;
					if(Diff == 0) 
					{
						SendSystemMsg( IDS_USER_ITEM_OVERFLOW, SYSTEM_ERROR, TO_ME);
						::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_FieldUse, 0); 			
						goto go_result;
					}
					//	¸Ê¿¡ ÀÖ´Â ¼ö·®ÀÌ ÀÎº¥¿¡ µé¾î°¥ ÃÖ´ëº¸´Ù ¸¹À¸¸é				
					if(pItem->sCount > Diff && Diff >= 0)
					{
						Diff = pItem->sCount - Diff; // ¸Ê¿¡ ³²°ÜÁú °¹¼ö¸¦ ±¸ÇÔ
						pItem->sCount -= Diff;
					}
					else Diff = 0;
*/
//					CheckMaxValue((short &)m_UserItem[index].sCount, pItem->sCount);
				}
				else
				{
					index = GetEmptySlot(INVENTORY_SLOT);
					if(index == -1) 
					{
//						SendSystemMsg( IDS_USER_ITEM_OVERFLOW, SYSTEM_ERROR, TO_ME);
						error_code = ERR_5;
						::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_FieldUse, 0); 			
						goto go_result;
					} 

					for(i = 1; i < pItem->sCount + 1; i++)
					{
						iWeight = g_arItemTable[pItem->sSid]->m_byWeight * i;
						if(m_iMaxWeight < m_iCurWeight + iWeight) 
						{
							iWeight = g_arItemTable[pItem->sSid]->m_byWeight * (i - 1);
							if(iWeight == 0)
							{
//								SendSystemMsg( IDS_USER_ITEM_OVERFLOW, SYSTEM_ERROR, TO_ME);
								error_code = ERR_4;
								::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_FieldUse, 0); 			
								goto go_result;
							}
							Diff = i;
							break;
						}
						Diff = i;
					}

					if(m_iMaxWeight >= m_iCurWeight + iWeight) 
					{
						if(pItem->sCount > Diff && Diff >= 0)
						{
							Diff = pItem->sCount - Diff; // ¸Ê¿¡ ³²°ÜÁú °¹¼ö¸¦ ±¸ÇÔ
							pItem->sCount -= Diff;
						}
						else Diff = 0;

						CheckMaxValue((short &)m_UserItem[index].sCount, pItem->sCount);
					}
					else
					{
//						SendSystemMsg( IDS_USER_ITEM_OVERFLOW, SYSTEM_ERROR, TO_ME);
						error_code = ERR_4;
						::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_FieldUse, 0); 			
						goto go_result;
					}				

					m_UserItem[index].sLevel = pItem->sLevel;
					m_UserItem[index].sSid = pItem->sSid;
					m_UserItem[index].sCount = pItem->sCount;
					m_UserItem[index].sDuration = pItem->sDuration;
					m_UserItem[index].sBullNum = pItem->sBullNum;
					m_UserItem[index].tIQ = pItem->tIQ;
					m_UserItem[index].sScore = pItem->sScore;								
					m_UserItem[index].sWeapScore = pItem->sWeapScore;
					for(j = 0; j < MAGIC_NUM; j++) m_UserItem[index].tMagic[j] = pItem->tMagic[j];
					m_UserItem[index].iItemSerial = pItem->iItemSerial;
					for(j = 0; j < ITEM_USER_RIGHT_NUM; j++) m_UserItem[index].uid[j] = -1;
					m_UserItem[index].dwTime = 0;

					if(f_Markini[0]->PingFen_KG == 1)//Ê°È¡ÆÀ·ÖÏÔÊ¾ MarkJay
					{
						GetUserItem( &(m_UserItem[index]), ITEMLOG_PICKUP );
					}

					MakeItemLog( &(m_UserItem[index]), ITEMLOG_PICKUP );
				}
			}
			
//			if(UpdateUserData() == FALSE)
			if(UpdateUserItemDN() == FALSE)
			{
				m_dwDN = dwMyDN;
				for(i = 0; i < TOTAL_ITEM_NUM; i++)//¾÷µ¥ÀÌÆ®°¡ ½ÇÆÐÇÏ¸é ºüÁ®³ª¿Â´Ù.
				{
					m_UserItem[i] = MyItem[i];
				}

				::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_FieldUse, 0); 
				error_code = ERR_7;
				goto go_result;
			}

			if(pItem->tType == TYPE_MONEY && dwDN > 0)
			{
				pItem->dwMoney = dwDN;
				SendItemFieldInfo(ITEM_INFO_PICKUP, TO_INSIGHT, pItem, sX, sY);

				// IKING 2001.1.
				EnterCriticalSection( &m_pCom->m_critThrowItem );
				if ( m_pCom->m_ThrowItemArray[mapIndex] != NULL )
				{
					m_pCom->m_ThrowItemArray[mapIndex]->m_pItem->dwMoney = dwDN;
				}
				LeaveCriticalSection( &m_pCom->m_critThrowItem );
			}
			else if(pItem->tType == TYPE_ITEM && Diff > 0)
			{
				pItem->sCount = Diff;
				SendItemFieldInfo(ITEM_INFO_PICKUP, TO_INSIGHT, pItem, sX, sY);

				// IKING 2001.1.
				EnterCriticalSection( &m_pCom->m_critThrowItem );
				if ( m_pCom->m_ThrowItemArray[mapIndex] != NULL )
				{
					m_pCom->m_ThrowItemArray[mapIndex]->m_pItem->sCount = Diff;
				}
				LeaveCriticalSection( &m_pCom->m_critThrowItem );
			}
			else
			{
				pItem->sSid = -1;
				pItem->dwMoney = 0;
				pItem->sCount = 0;
				SendItemFieldInfo(ITEM_INFO_PICKUP, TO_INSIGHT, pItem, sX, sY);
				pMap->m_pMap[ptCell.x][ptCell.y].iIndex = -1;

				// IKING 2001.1.
				EnterCriticalSection( &m_pCom->m_critThrowItem );
				if ( m_pCom->m_ThrowItemArray[mapIndex] != NULL )
				{
					if(m_pCom->m_ThrowItemArray[mapIndex]->m_pItem)
					{
						delete m_pCom->m_ThrowItemArray[mapIndex]->m_pItem;
						m_pCom->m_ThrowItemArray[mapIndex]->m_pItem = NULL;
						m_pCom->ThrowItemDel(m_pCom->m_ThrowItemArray[mapIndex]);
					}
				}
				LeaveCriticalSection( &m_pCom->m_critThrowItem );
				//
			}

			::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[ptCell.x][ptCell.y].m_FieldUse, 0); 			
		}
	}
	else goto go_result;

	// ----------------[ Send Packet ] ----------------------------------//
	
	if(tType == TYPE_ITEM)
	{
		TempBuf.Add(result);
		TempBuf.Add((BYTE)0x01);

		TempBuf.Add((BYTE)index);
		TempBuf.Add(m_UserItem[index].sLevel);
		TempBuf.Add(m_UserItem[index].sSid);
		TempBuf.Add(m_UserItem[index].sDuration);
		TempBuf.Add(m_UserItem[index].sBullNum);
		TempBuf.Add(m_UserItem[index].sCount);
		for(j = 0; j < MAGIC_NUM; j++) TempBuf.Add((BYTE)m_UserItem[index].tMagic[j]);

		TempBuf.Add((BYTE)m_UserItem[index].tIQ);

		Send(TempBuf, TempBuf.GetLength());
		m_iCurWeight += iWeight;
		GetRecoverySpeed();
	}
	else SendMoneyChanged();

	FlushItemLog( TRUE );
	return;

go_result:

	result = FAIL;
	TempBuf.Add(result);
	TempBuf.Add(error_code);
	Send(TempBuf, TempBuf.GetLength());
	//TRACE("ÁÝ±â ½ÇÆÐ\n");

	FlushItemLog( FALSE );
}

/////////////////////////////////////////////////////////////////////////////////
//	¸Ê¿¡ ÀÖ´Â ¾ÆÀÌÅÛ Á¤º¸¸¦ º¸³½´Ù.
//
void USER::SendItemFieldInfo(BYTE type, BYTE towho, ItemList *pItem, int x, int y)
{
	if(!pItem) return;

	CBufferEx TempBuf;

	TempBuf.Add(FIELD_ITEM_INFO);
	TempBuf.Add((short)0x01);
	TempBuf.Add(type);
	TempBuf.Add((short)x);
	TempBuf.Add((short)y);

	if(pItem->tType == TYPE_ITEM)	
	{
		TempBuf.Add(pItem->sSid);
		TempBuf.Add((DWORD)pItem->sCount);
		TempBuf.Add((BYTE)pItem->tIQ);
	}
	else
	{
		TempBuf.Add(TYPE_MONEY_SID);
		TempBuf.Add(pItem->dwMoney);
		TempBuf.Add((BYTE)0);
	}

	if(type == ITEM_INFO_PICKUP) TempBuf.Add((int)(m_uid + USER_BAND));

	switch( towho )
	{

	case TO_ALL:
		SendAll(TempBuf, TempBuf.GetLength());
		break;

	case TO_ME:
		Send(TempBuf, TempBuf.GetLength());
		break;

	case TO_ZONE:
		SendZone(TempBuf, TempBuf.GetLength());
		break;

	case TO_INSIGHT:
		SendInsight(TempBuf, TempBuf.GetLength());
		break;

	default:
		SendInsight(TempBuf, TempBuf.GetLength());
		break;

	}
}

/////////////////////////////////////////////////////////////////////////////////
//	ÇÊµå»ó¿¡ µ· Á¤º¸(ÁÝ´Â »ç¶÷ÀÌ ´©±¸ÀÎÁö ¹àÈù´Ù.)
//
void USER::GetMapMoneyInfo(DWORD dwDN, CPoint pt, int uid)
{
/*	CBufferEx TempBuf;

	short nPosX = 0;
	short nPosY = 0;

	nPosX = (short)pt.x;
	nPosY = (short)pt.y;

	TempBuf.Add(FIELD_ITEM_INFO);

	if(uid == -1)
	{
		TempBuf.Add(MONEY_INFO_MODIFY);
		TempBuf.Add(nPosX);
		TempBuf.Add(nPosY);
		TempBuf.Add(dwDN);
	}

	if(uid >= 0)
	{
		TempBuf.Add(MONEY_PICK_UP);
		TempBuf.Add(nPosX);
		TempBuf.Add(nPosY);
		TempBuf.Add(uid);
		TempBuf.Add(dwDN);
	}

	Send(TempBuf, TempBuf.GetLength());
	*/
}

/////////////////////////////////////////////////////////////////////////////////
//	ÀåºñÃ¢¿¡ µé¾î °¥ ¼ö ÀÖ´Â À§Ä¡¸¦ ±¸ÇÑ´Ù.
//
void USER::GetEquipPos(BYTE byWear, CPoint &pt)
{
	pt.x = -1;
	pt.y = -1;

	if(byWear < 1 || byWear > 8) return;

	switch(byWear)
	{
	case 1:
		pt.x = 4; pt.y = 5;
		break;
	case 2:
		pt.x = 0; pt.y = -1;
		break;
	case 3:
		pt.x = 1; pt.y = -1;
		break;
	case 4:
		pt.x = 8; pt.y = -1;
		break;
	case 5:
		pt.x = 9; pt.y = -1;
		break;
	case 6:
		pt.x = 2; pt.y = -1;
		break;
	case 7:
		pt.x = 3; pt.y = -1;
		break;
	case 8:
		pt.x = 6; pt.y = 7;
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////
//	E-Body Ã¢¿¡ µé¾î°¥ À§Ä¡¸¦  ±¸ÇÑ´Ù.
//
int USER::GetEBodyPos(BYTE byWear)
{
//	if((byWear < 122 || byWear > 125)||byWear!=130)return -1;

	switch(byWear)
	{
	case 122:
		return TOTAL_INVEN_MAX;
	case 123:
		return TOTAL_INVEN_MAX + 1;
	case 124:
		return TOTAL_INVEN_MAX + 2;
	case 125:
		return TOTAL_INVEN_MAX + 3;
	case 130:
		return TOTAL_INVEN_MAX + 4;
	}

	return -1;
}

///////////////////////////////////////////////////////////////////////////////
//	UserItem ¿¡ BufferÀÇ ³»¿ëÀ» Copy ÇÑ´Ù. --> ÇöÀç ¼ÒÁöÇÑ ¾ÆÀÌÅÛ ¹«°Ô¸¦ ´©ÀûÇÑ´Ù.
//
void USER::StrToUserItem(TCHAR *pBuf)
{
	int index = 0;
	int i, j;
	int temp = 0, value = 0;
	int iWear = -1;
	MYINT64 itemserial;

	for(i = 0; i < TOTAL_ITEM_NUM; i++)
	{
		m_UserItem[i].sLevel	= GetShort(pBuf, index);
		m_UserItem[i].sSid		= GetShort(pBuf, index);
		m_UserItem[i].sDuration = GetShort(pBuf, index);
		m_UserItem[i].sBullNum	= GetShort(pBuf, index);
		m_UserItem[i].sCount	= GetShort(pBuf, index);

		if(m_UserItem[i].sCount <= 0) m_UserItem[i].sSid = -1;

		for(j = 0; j < MAGIC_NUM; j++) 
		{
			m_UserItem[i].tMagic[j] = GetByte(pBuf, index);
		}
		m_UserItem[i].tIQ = GetByte(pBuf, index);

		if( m_UserItem[i].sSid == 625 || m_UserItem[i].sSid == 626 )	// ÀÚºê·Ñ, Ä®·Î¸¸
		{
			m_UserItem[i].sDuration = 0;
		}

		for( j = 0; j < 8; j++ )
		{
			itemserial.b[j] = GetByte( pBuf, index );
		}
		m_UserItem[i].iItemSerial = itemserial.i;

		for( j = 0; j < ITEM_USER_RIGHT_NUM; j++ )
		{
			m_UserItem[i].uid[j] = -1;
			m_UserItem[i].SuccessRate[j] = 0;
		}

		m_UserItem[i].dwTime = 0;

		// ¾ÆÀÌÅÛ ±³È¯ ¹ö±× ¶§¹®¿¡ ´Ã¾î³­ ÄÚµå (Áö¿ìÁø ¸»°Í) --------------------------------------------------->>
		if(m_UserItem[i].sSid >= 0 && m_UserItem[i].sSid < g_arItemTable.GetSize())
		{
			if(m_UserItem[i].sDuration > g_arItemTable[m_UserItem[i].sSid]->m_sDuration)
			{
				m_UserItem[i].sDuration = g_arItemTable[m_UserItem[i].sSid]->m_sDuration;
			}

			if(m_UserItem[i].sDuration > 0 && m_UserItem[i].tIQ != UNIQUE_ITEM)
			{
				if(m_UserItem[i].tMagic[ITEM_UPGRADE_COUNT] > 0)		// 6¹øÂ° °³Á¶ È½¼ö
				{
					iWear = g_arItemTable[m_UserItem[i].sSid]->m_byWear;

					if(iWear == ATTACK_ITEM)	// °ø°Ý¿ë ¹«±â
					{
						 if(m_UserItem[i].tMagic[ITEM_UPGRADE_COUNT] > MAX_ITEM_UPGRADE_COUNT + 1)		// ¹«Á¶°Ç 10¾÷ÀÌ»óÀÌ¸é ºñÁ¤»ó ¾ÆÀÌÅÛÀÓ
						 {
							 ReSetItemSlot(&m_UserItem[i]);
							 continue;
						 }

					}
					else if(iWear >= 2 && iWear <= 5)
					{
						 if(m_UserItem[i].tMagic[ITEM_UPGRADE_COUNT] > MAX_ITEM_UPGRADE_COUNT + 1)		// ¹«Á¶°Ç 10¾÷ÀÌ»óÀÌ¸é ºñÁ¤»ó ¾ÆÀÌÅÛÀÓ
						 {
							 ReSetItemSlot(&m_UserItem[i]);
							 continue;
						 }
					}
				}
			}

			if( m_UserItem[i].sSid == g_RR.m_iItemSid )				// ÀÌ ¾ÆÀÌÅÛÀÌ ·Î¾â·³ºí »óÇ°ÀÌ°í
			{
				if( g_strARKRRWinner.CompareNoCase( m_strUserID ) )	// ÇöÀç ½ÂÀÚ°¡ ¾Æ´Ò °æ¿ì
				{
					ReSetItemSlot( &(m_UserItem[i]) );					// »°´Â´Ù.
				}
			}
		}
		else	// ¾ÆÀÌÅÛ Å×ÀÌºí¿¡ ¾ø´Â ¾ÆÀÌÅÛÀº Áö¿î´Ù.
		{
			ReSetItemSlot(&m_UserItem[i]);
		}
		//-----------------------------------------------------------------------------------------------------<<

		if(m_UserItem[i].sSid >= 0 && m_UserItem[i].sSid < g_arItemTable.GetSize())
		{
			if(m_UserItem[i].sCount > 0) m_iCurWeight += (g_arItemTable[m_UserItem[i].sSid]->m_byWeight * m_UserItem[i].sCount);
			else m_iCurWeight += g_arItemTable[m_UserItem[i].sSid]->m_byWeight;
			GetRecoverySpeed();
		}
	}	
}

///////////////////////////////////////////////////////////////////////////////
//	Buffer ¿¡ UserItem Á¤º¸¸¦ Copy ÇÑ´Ù.
//
void USER::UserItemToStr(TCHAR *pBuf)
{
	int index = 0;
	int i, j;
	MYINT64 itemserial;

	for(i = 0; i < TOTAL_ITEM_NUM; i++)
	{
		SetShort(pBuf, m_UserItem[i].sLevel,	index);
		SetShort(pBuf, m_UserItem[i].sSid,		index);
		SetShort(pBuf, m_UserItem[i].sDuration,	index);
		SetShort(pBuf, m_UserItem[i].sBullNum,	index);
		SetShort(pBuf, m_UserItem[i].sCount,	index);

		for(j = 0; j < MAGIC_NUM; j++) SetByte(pBuf, m_UserItem[i].tMagic[j], index);

		SetByte(pBuf, m_UserItem[i].tIQ, index);

		itemserial.i = m_UserItem[i].iItemSerial;

		for( j = 0; j < 8; j++ )
		{
			SetByte( pBuf, itemserial.b[j], index );
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//	Buffer ¿¡ À¯Àú¿¡°Ô º¸³¾ UserItem Á¤º¸¸¦ Copy ÇÑ´Ù.
//
void USER::UserItemToStrForSend(TCHAR *pBuf)
{
	int index = 0;
	int i, j;

	for(i = 0; i < TOTAL_ITEM_NUM; i++)
	{
		SetShort(pBuf, m_UserItem[i].sLevel,	index);
		SetShort(pBuf, m_UserItem[i].sSid,		index);
		SetShort(pBuf, m_UserItem[i].sDuration,	index);
		SetShort(pBuf, m_UserItem[i].sBullNum,	index);
		SetShort(pBuf, m_UserItem[i].sCount,	index);

		for(j = 0; j < MAGIC_NUM; j++) SetByte(pBuf, m_UserItem[i].tMagic[j], index);

		SetByte(pBuf, m_UserItem[i].tIQ, index);
	}
}

///////////////////////////////////////////////////////////////////////////////
//	UserSkill ¿¡ Buffer ÀÇ ³»¿ëÀ» Ä«ÇÇÇÑ´Ù.
//
void USER::StrToUserSkill(TCHAR *pBuf)
{
	int index = 0,i;
	for(i = 0; i < TOTAL_SKILL_NUM; i++)
	{
		m_UserSkill[i].sSid		= GetShort(pBuf, index);
		m_UserSkill[i].tLevel	= GetByte (pBuf, index);
	}
	m_sSkillPoint_ = GetByte (pBuf, index);
	//¶ÁÈ¡130¼¶¼¼ÄÜ
	for(i = 0; i < 12; i++)
	{
		m_UserSkill_[i].sSid = i;
		m_UserSkill_[i].tLevel = GetByte (pBuf, index);
	}
}	

///////////////////////////////////////////////////////////////////////////////
//	Buffer ¿¡ UserSkill Á¤º¸¸¦ Copy ÇÑ´Ù.
//
void USER::UserSkillToStr(TCHAR *pBuf)
{
	int	index = 0;
	for(int i = 0; i < TOTAL_SKILL_NUM; i++)
	{
		SetShort(pBuf, m_UserSkill[i].sSid,		index);
		SetByte (pBuf, m_UserSkill[i].tLevel,	index);
	}
	SetByte (pBuf,(byte)m_sSkillPoint_,index);
	for(int i = 0; i < 12; i++)
	{
		SetByte(pBuf, m_UserSkill_[i].tLevel,	index);
	}
}

///////////////////////////////////////////////////////////////////////////////
//	UserPsi ¿¡ Buffer ÀÇ ³»¿ëÀ» Ä«ÇÇÇÑ´Ù.
//
void USER::StrToUserPsi(TCHAR *pBuf)
{
	int index = 0;

	m_nHavePsiNum = 0;
	for(int i = 0; i < TOTAL_PSI_NUM; i++)
	{

		m_UserPsi[i].sSid	= GetShort(pBuf, index);
		m_UserPsi[i].tOnOff	= GetByte (pBuf, index);
		
		if(m_UserPsi[i].sSid == -1)	break; 
		m_nHavePsiNum++;
	}
}

///////////////////////////////////////////////////////////////////////////////
//	Buffer ¿¡ UserPsi Á¤º¸¸¦ Copy ÇÑ´Ù.
//
void USER::UserPsiToStr(TCHAR *pBuf)
{
	int	index = 0;

	for(int i = 0; i < TOTAL_PSI_NUM; i++)
	{
		SetShort(pBuf, m_UserPsi[i].sSid,	index);
		SetByte (pBuf, m_UserPsi[i].tOnOff,	index);
		if(m_UserPsi[i].sSid == -1) break;		
		// DB¿¡ UpdateÇÒ °æ¿ì
	}
}

///////////////////////////////////////////////////////////////////////////////
//	Buffer ¿¡ UserSkillInfo Á¤º¸¸¦ Copy ÇÑ´Ù.
//
void USER::UserSkillInfoToStr(TCHAR *pBuf)
{
	int	index = 0;
	for(int i = 0; i < TOTAL_SKILL_NUM; i++)
	{
		SetByte (pBuf, m_UserSkillInfo[i],	index);
	}
}

///////////////////////////////////////////////////////////////////////////////
//	½Ã½ºÅÛ ¸Þ¼¼Áö¸¦ Àü¼ÛÇÑ´Ù.
//
void USER::SendSystemMsg(TCHAR *pMsg, BYTE type, int nWho)
{
	CBufferEx TempBuf;

	TempBuf.Add(SYSTEM_MSG);
	TempBuf.Add(type);
	TempBuf.Add(pMsg, _tcslen(pMsg));

	switch(nWho)
	{
	case TO_ALL:
		SendAll(TempBuf, TempBuf.GetLength());
		break;

	case TO_ME:
		Send(TempBuf, TempBuf.GetLength());
		break;

	case TO_ZONE:
		SendZone(TempBuf, TempBuf.GetLength());
		break;

	case TO_INSIGHT:
	default:
		SendInsight(TempBuf, TempBuf.GetLength());
		break;

	}
}
///////////////////////////////////////////////////////////////////////////////
//	Trade Request ¼º°ø Ã³¸®
//
void USER::SendTradeSuccess(int uid, TCHAR *pstrID)//(int uid)
{
	CBufferEx TempBuf;

	TempBuf.Add(ITEM_TRADE_RESULT);
	TempBuf.Add(uid);
	TempBuf.AddString(pstrID);
	TempBuf.Add(SUCCESS);

	Send(TempBuf, TempBuf.GetLength());
}

////////////////////////////////////////////////////////////////////////////////
//	Trade Request ½ÇÆÐÃ³¸®
//
void USER::SendTradeFail(int uid, BYTE error_code)
{
	CBufferEx TempBuf;

	TempBuf.Add(ITEM_TRADE_RESULT);
	TempBuf.Add(uid);
	TempBuf.AddString(m_strUserID);
	TempBuf.Add(FAIL);
	TempBuf.Add(error_code);

	Send(TempBuf, TempBuf.GetLength());
/*
	m_bTradeWaiting = FALSE;		// Trade ¿äÃ»Áß
	m_bNowTrading = FALSE;			// ÇöÀç °Å·¡Áß
	m_bExchangeOk = FALSE;			// ±³È¯½Â³«
	m_bTradeItem = FALSE;			// ¾ÆÀÌÅÛÀ» ÇÏ³ª¶óµµ ¿Ã·ÈÀ¸¸é...
	m_TradeMoney = 0;   			// °Å·¡ ÁßÀÎ ±Ý¾×

	for(int i = 0; i < TOTAL_ITEM_NUM; i++) // °Å·¡Áß ÀÎº¥¿¡ ¹Ù²î´Â ¾ÆÀÌÅÛ ¼ö¸¦ Ã¼Å©À§ÇØ
	{ 
		m_TradeItemNum[i].sSid = -1; 
		m_TradeItemNum[i].sNum = 0; 
	}
*/
}

/////////////////////////////////////////////////////////////////////////////////
//	ÎïÆ·½»Ò×
void USER::ItemTradeReq(TCHAR *pBuf)
{
#ifdef _ACTIVE_USER
//	if(m_iDisplayType == 6 && m_sLevel > 25) return; //yskang 0.5
	if(m_iDisplayType == 6) return; //yskang 0.5
#endif

	//  02-10-12 by Youn Gyu
	int type = CheckInvalidMapType();
	if(!CheckInvalidZoneState(type)) return;
	if(m_bPShopOpen == TRUE || m_bSessionOnline == true || m_bLive == USER_DEAD || m_state != STATE_GAMESTARTED) return; // ÉÌµê×´Ì¬ÔòÎÞ·¨½»Ò×
	if(!m_MItemLock && f_Markini[0]->JYMM_KG == 1)
	{
		SendSysWhisperChat("¶Ô²»Æð!½»Ò×ÃÜÂëÉÐÎ´½â³ý,ÎÞ·¨½øÐÐ½»Ò×.");
		return;
	}
	BYTE result = FAIL;
	BYTE error_code = UNKNOWN_ERR;

	TCHAR szUserName[CHAR_NAME_LENGTH + 1];
	::ZeroMemory(szUserName, sizeof(szUserName));

	USER* pSendUser = NULL;

	int i, index = 0;
	BYTE tMode = 0;

	int uid = GetInt(pBuf, index);

	int nLength = GetVarString(sizeof(szUserName), szUserName, pBuf, index);
	if(nLength <= 0 || nLength > CHAR_NAME_LENGTH) // Àß¸øµÈ À¯Àú¾ÆÀÌµð
	{
		SendTradeFail(uid, ERR_4);
		return;
	}
/*
	if(!CheckGetVarString(CHAR_NAME_LENGTH, szUserName, pBuf, sizeof(BYTE), index))		// Àß¸øµÈ À¯ÀúÀÌ¸§ÀÌ¸é ¿¡·¯
	{
		SendTradeFail(uid, ERR_4);
		return;
	}
*/
	tMode = GetByte(pBuf, index);

	pSendUser = GetUser(uid - USER_BAND);

	if(pSendUser == NULL || pSendUser->m_state != STATE_GAMESTARTED)	// Àß¸øµÈ À¯Àú¾ÆÀÌµðÀÌ¸é ¿¡·¯
	{
		SendTradeFail(uid, ERR_4);
		return;
	}
	
	//////////////////////////////////ÎïÆ·½»Ò×BUGÐÞ¸´////////////////////////////////////////
	_tcscpy(m_strTradeUser, _T(""));


	for(i = 0; i < TOTAL_ITEM_NUM; i++) 
	{ 
		m_TradeItemNum[i].sSid = -1; 
		m_TradeItemNum[i].sNum = 0; 
	}

	EnterCriticalSection( &m_CS_ExchangeItem );
	for(i = 0; i < m_arExchangeItem.GetSize(); i++)
	{
		if ( m_arExchangeItem[i] )
		{
			delete m_arExchangeItem[i];
			m_arExchangeItem[i] = NULL;
		}
	}
	m_arExchangeItem.RemoveAll();
	LeaveCriticalSection( &m_CS_ExchangeItem );

	if(pSendUser->m_bSessionOnline == true)
	{
		SendTradeFail(uid, ERR_3);
		pSendUser->m_bNowTrading = FALSE;
		m_bNowTrading = FALSE;
		return;
	}

	if(pSendUser->m_bNowTrading == TRUE || pSendUser->m_iTradeUid != -1)
	{
		SendTradeFail(uid, ERR_3);
		pSendUser->m_bNowTrading = FALSE;
		m_bNowTrading = FALSE;
		return;
	}
	if(m_bNowTrading == TRUE || m_iTradeUid != -1)
	{
		SendTradeFail(uid, ERR_3);
		m_bNowTrading = FALSE;
		return;
	}
	///////////////////////////////////end if///////////////////////////////////////

	if(IsThereUser(pSendUser) == FALSE)
	{
		SendTradeFail(uid, ERR_2);
		return;
	}

	switch(tMode)
	{
	case 0:			// Trade ¿äÃ»À» Ãë¼ÒÇÏ´Â °æ¿ì
		if(m_bTradeWaiting == TRUE && pSendUser->m_bNowTrading == FALSE)					
		{											// »ó´ë¹æ ½Â³«Àü, ¿äÃ»º¸³»°í ´Ù½Ã Ãë¼Ò ÇÒ¶§		
			pSendUser->SendTradeFail(m_uid + USER_BAND, (BYTE)0x00);
			m_bTradeWaiting = FALSE;
			return;
		}
		if(pSendUser->m_bNowTrading == TRUE)		// ½Â³«ÇÑµÚ °ÅÀýÀÏ¶§
		{
			SendTradeFail(uid, (BYTE)0x00);
			pSendUser->SendTradeFail(m_uid + USER_BAND, (BYTE)0x00);
			return;
		}
		return;
	case 1:			// Trade ¿äÃ»
		m_bTradeWaiting = TRUE;
		break;
	case 2:			// Trade ½Â³«
		if(pSendUser->m_bTradeWaiting != TRUE)
		{
			SendTradeFail(uid, UNKNOWN_ERR);
			return;
		}
		_tcscpy(m_strTradeUser, pSendUser->m_strUserID);
		_tcscpy(pSendUser->m_strTradeUser, m_strUserID);

					// °Å·¡ ½Â³«¶§ ÃÊ±âÈ­ ÇÊ¿ä
		m_TradeMoney = 0;
		pSendUser->m_TradeMoney = 0;

		m_iTradeUid = uid;
		pSendUser->m_iTradeUid = m_uid + USER_BAND;
		
		m_bNowTrading = TRUE;
		pSendUser->m_bNowTrading = TRUE;

		SendTradeSuccess(uid, pSendUser->m_strUserID);
		pSendUser->SendTradeSuccess(m_uid + USER_BAND, m_strUserID);
//		SendTradeSuccess(uid);
//		pSendUser->SendTradeSuccess(m_uid + USER_BAND);

		for(i = 0; i < TOTAL_ITEM_NUM; i++) 
		{
			m_TradeItemNum[i].sSid = m_UserItem[i].sSid;
			m_TradeItemNum[i].sNum = m_UserItem[i].sCount;
			pSendUser->m_TradeItemNum[i].sSid = pSendUser->m_UserItem[i].sSid;
			pSendUser->m_TradeItemNum[i].sNum = pSendUser->m_UserItem[i].sCount;
		}
		return;
	case 3:			// Trade °ÅÀý
		SendTradeFail(uid, ERR_6);
		pSendUser->SendTradeFail(uid, ERR_1);
		return;
	default:
		SendTradeFail(uid, UNKNOWN_ERR);
		return;
	}

	CBufferEx TempBuf;

	TempBuf.Add(ITEM_TRADE_REQ);
	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.AddString(m_strUserID);
	TempBuf.Add(tMode);	
	
	pSendUser->Send(TempBuf, TempBuf.GetLength());

	//MarkJayÔö¼ÓGM½»Ò×¼ÇÂ¼
	//TCHAR strOP[1024]; ZeroMemory(strOP,sizeof(strOP));
	//sprintf(strOP,"GMÓë:ÕÊºÅÎª:[%s]½øÐÐ½»Ò× : ÈËÎïÃû:%s",pSendUser->m_strAccount, pSendUser->m_strUserID);
	//WriteOpratorLog(strOP,ITEM_LOG);
}

/////////////////////////////////////////////////////////////////////////////
//	½»»»ÇëÇó
void USER::ExchangeReq(TCHAR *pBuf)
{
#ifdef _ACTIVE_USER
	if(m_iDisplayType == 6) return; //yskang 0.5
#endif

	if(m_bPShopOpen == TRUE || m_bSessionOnline == true || m_bLive == USER_DEAD || m_state != STATE_GAMESTARTED) return;

	BOOL flag = FALSE;
	int j, iNum = 0;
	int index = 0;
	int uid = 0;
	int Solt = 0;
	BYTE byType = 0, bySlot = 0;
	DWORD dwCount = 0;
	BYTE result = 0;
	BYTE error_code = UNKNOWN_ERR;

	short sCount = 0;

	USER* pSendUser = NULL;
	USER* pTempUser = NULL;

	if(m_bNowTrading != TRUE)
	{
		if(m_iTradeUid != -1) 	pSendUser = GetUser(m_iTradeUid - USER_BAND);
		if(pSendUser != NULL) 
		{
			SendExchangeFail((BYTE)0x00, (BYTE)0x02);
			pSendUser->SendExchangeFail((BYTE)0x00, (BYTE)0x02);
		}
		return;
	}

	if(m_iTradeUid != -1)//ÐÞ¸´½»Ò×BUG
	{
		if (m_bNowTrading != TRUE)
		{
			pSendUser = GetUser(m_iTradeUid - USER_BAND);
			if(pSendUser != NULL) 
			{
				SendExchangeFail((BYTE)0x00, (BYTE)0x02);
				pSendUser->SendExchangeFail((BYTE)0x00, (BYTE)0x02);
			}
			return;
		}
	}

	ItemList	TempItem;
	ExchangeItem* pNewItem = NULL;

	uid		= GetInt(pBuf, index);
	byType	= GetByte(pBuf, index);

	CBufferEx TempBuf;
	CBufferEx TempMyBuf;

	if(uid < 0 || uid >= NPC_BAND)				// Àß¸øµÈ »ó´ëÆí uid¸é °Å·¡ Ãë¼Ò
	{
		result = 0x00;
		error_code = ERR_1;
		SendExchangeFail(result, error_code);

		pTempUser = GetUser(m_iTradeUid - USER_BAND);

		if(pTempUser) 
		{
			pTempUser->SendExchangeFail(result, UNKNOWN_ERR);
		}

		return;
	}

	pSendUser = GetUser(uid - USER_BAND);		// Àß¸øµÈ À¯Àú 
	if(m_bPShopOpen == TRUE || pSendUser == NULL || pSendUser->m_state != STATE_GAMESTARTED || pSendUser->m_bPShopOpen == TRUE || pSendUser->m_bSessionOnline == true || pSendUser->m_bLive == USER_DEAD)
	{
		result = 0x00;
		error_code = ERR_1;
		SendExchangeFail(result, error_code);

		pTempUser = GetUser(m_iTradeUid - USER_BAND);

		if(pTempUser) 
		{
			pTempUser->SendExchangeFail(result, UNKNOWN_ERR);
		}

		return;
	}

	if(_tcscmp(m_strTradeUser, pSendUser->m_strUserID) != 0)		// Ã³À½¿¡ °Å·¡ÇÏ´ø À¯Àú°¡ ¾Æ´Ô
	{
		result = 0x00;
		error_code = ERR_2;
		SendExchangeFail(result, error_code);

		pTempUser = GetUser(m_iTradeUid - USER_BAND);

		if(pTempUser) 
		{
			pTempUser->SendExchangeFail(result, UNKNOWN_ERR);
		}

		return;
	}

	switch(byType)
	{
	case 0:				// ±³È¯Ãë¼Ò
		result = 0x00;
		SendExchangeFail(result, (BYTE)0x04);
		pSendUser->SendExchangeFail(result, (BYTE)0x05);
		return;
	case 1:				// ±³È¯½Â³«
		if(m_bExchangeOk == FALSE) 
		{
			m_bExchangeOk = TRUE;

			index = 0;
			SetByte(m_TempBuf, EXCHANGE_REQ, index);
			SetInt(m_TempBuf, uid, index);
			SetByte(m_TempBuf, 0x01, index);
			Send(m_TempBuf, index);

			index = 0;
			SetByte(TempBuf, EXCHANGE_REQ, index);
			SetInt(TempBuf, m_uid + USER_BAND, index);
			SetByte(TempBuf, 0x02, index);
			pSendUser->Send(TempBuf, index);

		}
		
		if(m_bExchangeOk == TRUE && pSendUser->m_bExchangeOk == TRUE)
		{
			EnterCriticalSection( &m_CS_ExchangeItem );
			for(j = 0; j < m_arExchangeItem.GetSize(); j++)		// ¹«°Ô Á¦ÇÑ¿¡ °É¸®³ª Ã¼Å©...
			{
				if ( m_arExchangeItem[j] )
				{
					short sExSid = m_arExchangeItem[j]->changedList.sSid;
					if(sExSid < 0 || sExSid >= g_arItemTable.GetSize()) continue;

					if(g_arItemTable[sExSid]->m_sEvent >= EVENT_UNIQUE_ITEM_BAND) 
					{
						LeaveCriticalSection( &m_CS_ExchangeItem );

						result = 0x00;
						SendExchangeFail(result, 0x07);
						pSendUser->SendExchangeFail(result, 0x06);
						SendSystemMsg( IDS_USER_CANT_BUYSELL_EVENT_ITEM, SYSTEM_ERROR, TO_ME);//"ÎÞ·¨½»Ò×ÈÎÎñÎïÆ·."
						pSendUser->SendSystemMsg( IDS_USER_CANT_BUYSELL_EVENT_ITEM, SYSTEM_ERROR, TO_ME);//"ÎÞ·¨½»Ò×ÈÎÎñÎïÆ·."
						return;
					}
					//if(g_arItemTable[sExSid]->m_sSid == 1121) //¸±ÊÖ²»¿É½»Ò× Ð¡ºÚ¸±ÊÖ
					//{
					//	LeaveCriticalSection( &m_CS_ExchangeItem );

					//	result = 0x00;
					//	SendExchangeFail(result, 0x07);
					//	pSendUser->SendExchangeFail(result, 0x06);
					//	SendSystemMsg( "¡¾¶ñÄ§Ö®Òí¡¿ÊÇ°ó¶¨µÀ¾ß,½»Ò×Ê§°Ü!", SYSTEM_ERROR, TO_ME);
					//	pSendUser->SendSystemMsg( "¡¾¶ñÄ§Ö®Òí¡¿ÊÇ°ó¶¨µÀ¾ß,½»Ò×Ê§°Ü!", SYSTEM_ERROR, TO_ME);
					//	return;
					//}
					if(!m_MItemLock && f_Markini[0]->JYMM_KG == 1)
					{
						LeaveCriticalSection( &m_CS_ExchangeItem );

						result = 0x00;
						SendExchangeFail(result, 0x07);
						pSendUser->SendExchangeFail(result, 0x06);
						SendSystemMsg( "½»Ò×ÃÜÂëÉÐÎ´½â³ý»ò¶Ô·½ÉÐÎ´½â³ý²»ÄÜ½»Ò×.", SYSTEM_ERROR, TO_ME);//"ÎÞ·¨½»Ò×ÈÎÎñÎïÆ·."
						pSendUser->SendSystemMsg( "½»Ò×ÃÜÂëÉÐÎ´½â³ý»ò¶Ô·½ÉÐÎ´½â³ý²»ÄÜ½»Ò×.", SYSTEM_ERROR, TO_ME);//"ÎÞ·¨½»Ò×ÈÎÎñÎïÆ·."
						return;
					}
					if(!m_MItemLock && m_dwCKMiMa == 0 && m_dwGuarDianTianShi == 0)
					{
						LeaveCriticalSection( &m_CS_ExchangeItem );

						result = 0x00;
						SendExchangeFail(result, 0x07);
						pSendUser->SendExchangeFail(result, 0x06);
						SendSystemMsg( "Ã»ÓÐÓÎÏ·Ê±¼ä»ò¶Ô·½Ã»ÓÐÓÎÏ·Ê±¼ä²»ÄÜ½»Ò×.", SYSTEM_ERROR, TO_ME);//"ÎÞ·¨½»Ò×ÈÎÎñÎïÆ·."
						pSendUser->SendSystemMsg( "Ã»ÓÐÓÎÏ·Ê±¼ä»ò¶Ô·½Ã»ÓÐÓÎÏ·Ê±¼ä²»ÄÜ½»Ò×.", SYSTEM_ERROR, TO_ME);//"ÎÞ·¨½»Ò×ÈÎÎñÎïÆ·."
						return;
					}
					if(m_sLevel < 10)
					{
						LeaveCriticalSection( &m_CS_ExchangeItem );

						result = 0x00;
						SendExchangeFail(result, 0x07);
						pSendUser->SendExchangeFail(result, 0x06);
						SendSystemMsg( "µÈ¼¶µÍÓÚ10¼¶²»ÄÜ½»Ò×.", SYSTEM_ERROR, TO_ME);//"ÎÞ·¨½»Ò×ÈÎÎñÎïÆ·."
						pSendUser->SendSystemMsg( "µÈ¼¶µÍÓÚ10¼¶²»ÄÜ½»Ò×.", SYSTEM_ERROR, TO_ME);//"ÎÞ·¨½»Ò×ÈÎÎñÎïÆ·."
						return;
					}
					/*if(CheckInvalidMapType() == 12)//µ¥ÈË¸ñ¶·Èü MarkJay
					{
						LeaveCriticalSection( &m_CS_ExchangeItem );

						result = 0x00;
						SendExchangeFail(result, 0x07);
						pSendUser->SendExchangeFail(result, 0x06);
						SendSystemMsg( "»î¶¯µØÍ¼,²»ÄÜ½»Ò×.", SYSTEM_ERROR, TO_ME);//"ÎÞ·¨½»Ò×ÈÎÎñÎïÆ·."
						pSendUser->SendSystemMsg( "»î¶¯µØÍ¼,²»ÄÜ½»Ò×.", SYSTEM_ERROR, TO_ME);//"ÎÞ·¨½»Ò×ÈÎÎñÎïÆ·."
					}*/

					bySlot = m_arExchangeItem[j]->bySlot;//´Ë´¦Ôì³É±¨´í±ê¼Ç MarkJay			
					sCount = m_arExchangeItem[j]->changedList.sCount;
					if(m_UserItem[bySlot].sSid != sExSid || sCount > m_UserItem[bySlot].sCount || m_UserItem[bySlot].sCount <= 0)
					{
						LeaveCriticalSection( &m_CS_ExchangeItem );

						result = 0x00;
						SendExchangeFail(result, 0x07);
						pSendUser->SendExchangeFail(result, 0x06);

						SYSTEMTIME st;
						CString strDate;
						GetLocalTime(&st);
						strDate.Format("%d-%d-%d %d:%d",st.wYear,st.wMonth,st.wDay ,st.wHour,st.wMinute);
						CString str = _T("");
						str.Format("%s ½»Ò×BUGÐÞ¸´[%s] ½»Ò×ÎïÆ· %s[%d] ÊýÁ¿ %d Êµ¼ÊÎïÆ·ID %d Êµ¼ÊÊýÁ¿ %d\r\n",strDate,m_strUserID, g_arItemTable[sExSid]->m_strName,sExSid,sCount,m_UserItem[bySlot].sSid,m_UserItem[bySlot].sCount);
						EnterCriticalSection( &m_CS_FileWrite );
						g_fpMarkDeBug.Write( str, str.GetLength() );
						LeaveCriticalSection( &m_CS_FileWrite);
						return;
					}
					iNum = g_arItemTable[sExSid]->m_byWeight * sCount;
					if(pSendUser->m_iMaxWeight < pSendUser->m_iCurWeight + iNum)
					{
						LeaveCriticalSection( &m_CS_ExchangeItem );

						result = 0x00;
						SendExchangeFail(result, 0x07);
						pSendUser->SendExchangeFail(result, 0x06);
						SendSystemMsg( IDS_USER_OVER_WEIGHT_OTHER, SYSTEM_ERROR, TO_ME);//"¸ÃÍæ¼ÒÒÑ¾­³¬ÖØ."
						pSendUser->SendSystemMsg( IDS_USER_OVER_WEIGHT_SAVE, SYSTEM_ERROR, TO_ME);//"±£´æµØµãÊ§°Ü."
						return;
					}
				}
			}
			LeaveCriticalSection( &m_CS_ExchangeItem );


			EnterCriticalSection( &pSendUser->m_CS_ExchangeItem );
			for(j = 0; j < pSendUser->m_arExchangeItem.GetSize(); j++)		// ¹«°Ô Á¦ÇÑ¿¡ °É¸®³ª Ã¼Å©...
			{			
				if ( pSendUser->m_arExchangeItem[j] )
				{
					short sExSid = pSendUser->m_arExchangeItem[j]->changedList.sSid;
					if(sExSid < 0 || sExSid >= g_arItemTable.GetSize()) continue;

					if(g_arItemTable[sExSid]->m_sEvent >= EVENT_UNIQUE_ITEM_BAND) 
					{
						LeaveCriticalSection( &pSendUser->m_CS_ExchangeItem );

						result = 0x00;
						SendExchangeFail(result, 0x07);
						pSendUser->SendExchangeFail(result, 0x06);
						SendSystemMsg( IDS_USER_CANT_BUYSELL_EVENT_ITEM, SYSTEM_ERROR, TO_ME);//"ÎÞ·¨½»Ò×ÈÎÎñÎïÆ·."
						pSendUser->SendSystemMsg( IDS_USER_CANT_BUYSELL_EVENT_ITEM, SYSTEM_ERROR, TO_ME);//"ÎÞ·¨½»Ò×ÈÎÎñÎïÆ·."
						return;
					}
					//if(g_arItemTable[sExSid]->m_sSid == 1121) //¸±ÊÖ²»¿É½»Ò× Ð¡ºÚ¸±ÊÖ
					//{
					//	LeaveCriticalSection( &m_CS_ExchangeItem );

					//	result = 0x00;
					//	SendExchangeFail(result, 0x07);
					//	pSendUser->SendExchangeFail(result, 0x06);
					//	SendSystemMsg( "¡¾¶ñÄ§Ö®Òí¡¿ÊÇ°ó¶¨µÀ¾ß,½»Ò×Ê§°Ü!", SYSTEM_ERROR, TO_ME);
					//	pSendUser->SendSystemMsg( "¡¾¶ñÄ§Ö®Òí¡¿ÊÇ°ó¶¨µÀ¾ß,½»Ò×Ê§°Ü!", SYSTEM_ERROR, TO_ME);
					//	return;
					//}
					if(!m_MItemLock && f_Markini[0]->JYMM_KG == 1)
					{
						LeaveCriticalSection( &pSendUser->m_CS_ExchangeItem );

						result = 0x00;
						SendExchangeFail(result, 0x07);
						pSendUser->SendExchangeFail(result, 0x06);						
						SendSystemMsg( "½»Ò×ÃÜÂëÉÐÎ´½â³ý»ò¶Ô·½ÉÐÎ´½â³ý²»ÄÜ½»Ò×.", SYSTEM_ERROR, TO_ME);//"ÎÞ·¨½»Ò×ÈÎÎñÎïÆ·."
						pSendUser->SendSystemMsg( "½»Ò×ÃÜÂëÉÐÎ´½â³ý»ò¶Ô·½ÉÐÎ´½â³ý²»ÄÜ½»Ò×.", SYSTEM_ERROR, TO_ME);//"ÎÞ·¨½»Ò×ÈÎÎñÎïÆ·."
						return;
					}
					if(!m_MItemLock && m_dwCKMiMa == 0 && m_dwGuarDianTianShi == 0)
					{
						LeaveCriticalSection( &pSendUser->m_CS_ExchangeItem );

						result = 0x00;
						SendExchangeFail(result, 0x07);
						pSendUser->SendExchangeFail(result, 0x06);						
						SendSystemMsg( "Ã»ÓÐÓÎÏ·Ê±¼ä»ò¶Ô·½Ã»ÓÐÓÎÏ·Ê±¼ä²»ÄÜ½»Ò×.", SYSTEM_ERROR, TO_ME);//"ÎÞ·¨½»Ò×ÈÎÎñÎïÆ·."
						pSendUser->SendSystemMsg( "Ã»ÓÐÓÎÏ·Ê±¼ä»ò¶Ô·½Ã»ÓÐÓÎÏ·Ê±¼ä²»ÄÜ½»Ò×.", SYSTEM_ERROR, TO_ME);//"ÎÞ·¨½»Ò×ÈÎÎñÎïÆ·."
						return;
					}
					if(m_sLevel < 10)
					{
						LeaveCriticalSection( &pSendUser->m_CS_ExchangeItem );

						result = 0x00;
						SendExchangeFail(result, 0x07);
						pSendUser->SendExchangeFail(result, 0x06);						
						SendSystemMsg( "µÈ¼¶µÍÓÚ10¼¶²»ÄÜ½»Ò×.", SYSTEM_ERROR, TO_ME);//"ÎÞ·¨½»Ò×ÈÎÎñÎïÆ·."
						pSendUser->SendSystemMsg( "µÈ¼¶µÍÓÚ10¼¶²»ÄÜ½»Ò×.", SYSTEM_ERROR, TO_ME);//"ÎÞ·¨½»Ò×ÈÎÎñÎïÆ·."
						return;
					}
					/*if(CheckInvalidMapType() == 12)//µ¥ÈË¸ñ¶·Èü MarkJay
					{
						LeaveCriticalSection( &pSendUser->m_CS_ExchangeItem );

						result = 0x00;
						SendExchangeFail(result, 0x07);
						pSendUser->SendExchangeFail(result, 0x06);						
						SendSystemMsg( "»î¶¯µØÍ¼,²»ÄÜ½»Ò×.", SYSTEM_ERROR, TO_ME);//"ÎÞ·¨½»Ò×ÈÎÎñÎïÆ·."
						pSendUser->SendSystemMsg( "»î¶¯µØÍ¼,²»ÄÜ½»Ò×.", SYSTEM_ERROR, TO_ME);//"ÎÞ·¨½»Ò×ÈÎÎñÎïÆ·."
						return;
					}*/

					bySlot = pSendUser->m_arExchangeItem[j]->bySlot;
					sCount = pSendUser->m_arExchangeItem[j]->changedList.sCount;
					if(pSendUser->m_UserItem[bySlot].sSid != sExSid || sCount > pSendUser->m_UserItem[bySlot].sCount || pSendUser->m_UserItem[bySlot].sCount <= 0)
					{
						LeaveCriticalSection( &m_CS_ExchangeItem );

						result = 0x00;
						SendExchangeFail(result, 0x07);
						pSendUser->SendExchangeFail(result, 0x06);

						SYSTEMTIME st;
						CString strDate;
						GetLocalTime(&st);
						strDate.Format("%d-%d-%d %d:%d",st.wYear,st.wMonth,st.wDay ,st.wHour,st.wMinute);
						CString str = _T("");
						str.Format("%s ½»Ò×BUGÐÞ¸´[%s] ½»Ò×ÎïÆ· %s[%d] ÊýÁ¿ %d Êµ¼ÊÎïÆ·ID %d Êµ¼ÊÊýÁ¿ %d\r\n",strDate,pSendUser->m_strUserID, g_arItemTable[sExSid]->m_strName,sExSid,sCount,pSendUser->m_UserItem[bySlot].sSid,pSendUser->m_UserItem[bySlot].sCount);
						EnterCriticalSection( &m_CS_FileWrite );
						g_fpMarkDeBug.Write( str, str.GetLength() );
						LeaveCriticalSection( &m_CS_FileWrite);
						return;
					}
					iNum = g_arItemTable[sExSid]->m_byWeight * sCount;
					if(m_iMaxWeight < m_iCurWeight + iNum)
					{
						LeaveCriticalSection( &pSendUser->m_CS_ExchangeItem );

						result = 0x00;
						SendExchangeFail(result, 0x06);
						pSendUser->SendExchangeFail(result, 0x07);
						SendSystemMsg( IDS_USER_OVER_WEIGHT_SAVE, SYSTEM_ERROR, TO_ME);//"±£´æµØµãÊ§°Ü."
						pSendUser->SendSystemMsg( IDS_USER_OVER_WEIGHT_SAVE_OTHER, SYSTEM_ERROR, TO_ME);// "±£´æµØµãÊ§°Ü."
						return;
					}
				}
			}
			LeaveCriticalSection( &pSendUser->m_CS_ExchangeItem );

			RunExchange(pSendUser);
		}

		break;

	case 2:			// Å¸ÀÎÀÌ ±³È¯ ½Â³«(¼­¹ö¸¸ÀÌ ÇÒ ¼ö ÀÖ´Ù)
		result = 0x00;
		error_code = UNKNOWN_ERR;
		SendExchangeFail(result, error_code);
		pSendUser->SendExchangeFail(result, UNKNOWN_ERR);
		return;

	case 3:			// ÀÚ½ÅÀÌ ¾ÆÀÌÅÛ ¿Ã¸²
		if(m_bExchangeOk == TRUE) return;		// µ¿ÀÇÇÏ¸é ´õÀÌ»óÀÇ ¾ÆÀÌÅÛ Ãß°¡¸¦ °ÅºÎÇÑ´Ù.

		bySlot	= GetByte(pBuf, index);
		sCount	= GetShort(pBuf, index);

		if(bySlot < EQUIP_ITEM_NUM || bySlot >= TOTAL_INVEN_MAX) //|| sCount > MAX_ITEM_NUM)
		{
			result = 0x00;
			error_code = ERR_3;

			SendExchangeFail(result, error_code);
			pSendUser->SendExchangeFail(result, error_code);

			return;
		}

		if(sCount <= 0 || sCount >= _MAX_SHORT)
		{
			result = 0x00;
			error_code = ERR_3;

			SendExchangeFail(result, error_code);
			pSendUser->SendExchangeFail(result, error_code);

			return;
		}

		if(m_TradeItemNum[bySlot].sSid < 0 || m_TradeItemNum[bySlot].sSid >= g_arItemTable.GetSize() || m_TradeItemNum[bySlot].sNum < sCount)
		{
			result = 0x00;
			error_code = ERR_3;

			SendExchangeFail(result, error_code);
			pSendUser->SendExchangeFail(result, error_code);
			return;
		}
		m_bTradeItem = TRUE;
		ReSetItemSlot(&TempItem);	
		iNum = m_TradeItemNum[bySlot].sNum;
		if(iNum <= 0)//ÐÞ¸´½»Ò×BUG 
		{
			result = 0x00;
			error_code = ERR_3;
			SendExchangeFail(result, error_code);
			pSendUser->SendExchangeFail(result, error_code);
			return;
		}
		m_TradeItemNum[bySlot].sNum = iNum - sCount;
		if(m_TradeItemNum[bySlot].sNum < 0)//ÆÁ±ÎÏûºÄÆ·Ë¢¶«Î÷
		{
			m_TradeItemNum[bySlot].sNum = 0;
			result = 0x00;
			error_code = ERR_3;
			SendExchangeFail(result, error_code);
			pSendUser->SendExchangeFail(result, error_code);
			return;
		}

		TempItem = m_UserItem[bySlot];
		TempItem.sCount = sCount;

		if(TempItem.sSid < 0 || TempItem.sSid >= g_arItemTable.GetSize())		// ´Ù½ÃÇÑ¹Ý È®ÀÎ
		{
			result = 0x00;
			error_code = ERR_3;
			SendExchangeFail(result, error_code);
			pSendUser->SendExchangeFail(result, error_code);
			return;
		}
/////////////////////////////ÐÞ¸´½»Ò×±êÖ¾³¬¹ý32767ÎÊÌâ//////////////////////////////////
		if(g_arItemTable[TempItem.sSid]->m_sDuration <= 0)
		{
 			Solt = GetSameItem(m_UserItem[bySlot], TRADE_SLOT);
 			if(Solt != -1)
 			{
 				result = 0x00;
 				error_code = ERR_3;
 				SendExchangeFail(result, error_code);
 				pSendUser->SendExchangeFail(result, error_code);
 				return;
 			}
			iNum = pSendUser->GetSameItem(TempItem, INVENTORY_SLOT);
			if(iNum != -1)
			{
				if(pSendUser->m_UserItem[iNum].sCount + TempItem.sCount >= 32767)
				{
					result = 0x00;
					error_code = ERR_3;
					SendExchangeFail(result, error_code);
					pSendUser->SendExchangeFail(result, error_code);
					SendSystemMsg( "¶Ô·½±£¹ÜµÄ×î¶àÊýÁ¿ÊÇ32767¸ö.", SYSTEM_ERROR, TO_ME);
					return;
				}
			}
		}

		EnterCriticalSection( &m_CS_ExchangeItem );
		if(m_arExchangeItem.GetSize() >= INVENTORY_NUM)
		{
			LeaveCriticalSection( &m_CS_ExchangeItem );

			result = 0x00;
			SendExchangeFail(result, 0x06);
			pSendUser->SendExchangeFail(result, 0x07);
			return;
		}
		LeaveCriticalSection( &m_CS_ExchangeItem );

		pNewItem = new ExchangeItem;
		pNewItem->bySlot = bySlot;
		pNewItem->changedList.sLevel = m_UserItem[bySlot].sLevel;
		pNewItem->changedList.sSid = m_UserItem[bySlot].sSid;
		pNewItem->changedList.sDuration = m_UserItem[bySlot].sDuration;
		pNewItem->changedList.sBullNum = m_UserItem[bySlot].sBullNum;
		pNewItem->changedList.sCount = sCount;
		for(j =0; j < MAGIC_NUM; j++) pNewItem->changedList.tMagic[j] = m_UserItem[bySlot].tMagic[j];
		pNewItem->changedList.tIQ = m_UserItem[bySlot].tIQ;
		pNewItem->changedList.iItemSerial = m_UserItem[bySlot].iItemSerial;

		EnterCriticalSection( &m_CS_ExchangeItem );
		m_arExchangeItem.Add(pNewItem);
		LeaveCriticalSection( &m_CS_ExchangeItem );

		SendExchangeItem(pSendUser, bySlot, (DWORD)sCount);
		break;

	case 4:			// Å¸ÀÎÀÌ ¾ÆÀÌÅÛ ¿Ã¸²(¼­¹ö¸¸ÀÌ ÁÙ ¼ö ÀÖ´Ù)
		result = 0x00;
		error_code = UNKNOWN_ERR;
		SendExchangeFail(result, error_code);
		pSendUser->SendExchangeFail(result, UNKNOWN_ERR);
		return;

	case 5:			// ÀÚ½ÅÀÌ µ· ¿Ã¸²
		dwCount	= GetDWORD(pBuf, index);
		if(dwCount <= 0 || dwCount > m_dwDN)
		{
			result = 0x00;
			error_code = ERR_3;
			SendExchangeFail(result, error_code);
			pSendUser->SendExchangeFail(result, error_code);
			return;
		}

//		m_TradeMoney = m_TradeMoney + dwCount;
		CheckMaxValue((DWORD &)m_TradeMoney, (DWORD)dwCount);

		if(m_sLevel < 10)
		{
			result = 0x00;
			error_code = ERR_3;
			SendExchangeFail(result, error_code);
			pSendUser->SendExchangeFail(result, error_code);
			SendSystemMsg( "µÈ¼¶µÍÓÚ10¼¶²»ÄÜ½»Ò×.", SYSTEM_ERROR, TO_ME);//"ÎÞ·¨½»Ò×ÈÎÎñÎïÆ·."
			pSendUser->SendSystemMsg( "µÈ¼¶µÍÓÚ10¼¶²»ÄÜ½»Ò×.", SYSTEM_ERROR, TO_ME);//"ÎÞ·¨½»Ò×ÈÎÎñÎïÆ·."
			return;
		}

		if(m_TradeMoney > m_dwDN)
		{
			result = 0x00;
			error_code = ERR_3;
			SendExchangeFail(result, error_code);
			pSendUser->SendExchangeFail(result, error_code);
			return;
		}

		bySlot = 255;
		SendExchangeItem(pSendUser, bySlot, m_TradeMoney);
//		SendExchangeItem(pSendUser, bySlot, dwCount);
		break;

	case 6:			// Å¸ÀÎÀÌ µ· ¿Ã¸²(¼­¹ö¸¸ÀÌ ÁÙ ¼ö ÀÖ´Ù)
	default:
		result = 0x00;
		error_code = UNKNOWN_ERR;
		SendExchangeFail(result, error_code);
		pSendUser->SendExchangeFail(result, UNKNOWN_ERR);
		return;
	}
}

//////////////////////////////////////////////////////////////////////////////
//	¾ÆÀÌÅÛ ±³È¯ ½ÇÆÐ ÆÐÅ¶À» º¸³½´Ù.
//
void USER::SendExchangeFail(BYTE result, BYTE error_code)
{
	if(m_bSessionOnline == true) return;//ÀëÏßÉÌµê MarkJay
	if ( m_UserStatus == 0 && m_UserFlag == FALSE ) return;

	USER* pTempUser = NULL;
	CBufferEx TempBuf, TempMyBuf;
	int i, iNum = 0;

	BYTE bySlot = 0;
	
	TempBuf.Add(EXCHANGE_RESULT);
	TempBuf.Add(result);
	TempBuf.Add(error_code);

	Send(TempBuf, TempBuf.GetLength());

	if((m_bTradeItem == TRUE || m_TradeMoney > 0) && result == 0)			// ¸¸¾à ±³È¯ÁßÀÌ°í, ¾ÆÀÌÅÛÀ» ¿Ã·Á³ùÀ»°æ¿ì 
	{
		EnterCriticalSection( &m_CS_ExchangeItem );
		iNum = m_arExchangeItem.GetSize();				// ¿Ã·Á³õÀº ¾ÆÀÌÅÛ ¼ö¸¸Å­ »ç¿ëÀÚ¿¡°Ô ´Ù½Ã ¾Ë·ÁÁÜ
		if(iNum > 0)
		{
			TempMyBuf.Add(ITEM_MOVE_RESULT);
			TempMyBuf.Add(SUCCESS);
			TempMyBuf.Add((BYTE)0x00);
			TempMyBuf.Add((BYTE)iNum);

			for(i = 0; i < iNum; i++)
			{
				if ( m_arExchangeItem[i] == NULL ) continue;

				bySlot = m_arExchangeItem[i]->bySlot;

				TempMyBuf.Add(bySlot);	
				TempMyBuf.Add(m_UserItem[bySlot].sLevel);
				TempMyBuf.Add(m_UserItem[bySlot].sSid);
				TempMyBuf.Add(m_UserItem[bySlot].sDuration);
				TempMyBuf.Add(m_UserItem[bySlot].sBullNum);
				TempMyBuf.Add(m_UserItem[bySlot].sCount);

				for(int j =0; j < MAGIC_NUM; j++) TempMyBuf.Add(m_UserItem[bySlot].tMagic[j]);

				TempMyBuf.Add(m_UserItem[bySlot].tIQ); 
			}
			Send(TempMyBuf, TempMyBuf.GetLength());
		}
		LeaveCriticalSection( &m_CS_ExchangeItem );

		if(m_TradeMoney > 0) SendMoneyChanged();
	}

	if(result == 0 || result == 2)
	{
		_tcscpy(m_strTradeUser, _T(""));// »ó´ë¹æÀ» È®ÀÎÇÏ±â À§ÇÑ À¯ÀúÀÌ¸§À» Áö¿î´Ù.
		m_bNowTrading = m_bTradeWaiting = FALSE;
		m_bExchangeOk = m_bTradeItem	= FALSE;// ±³È¯Áß »óÅÂ¸¦ ÃÊ±âÈ­
		m_iTradeUid = -1;
		m_TradeMoney = 0;				// °Å·¡ ±Ý¾×À» ÃÊ±âÈ­

		for(i = 0; i < TOTAL_ITEM_NUM; i++) // °Å·¡Áß ÀÎº¥¿¡ ¹Ù²î´Â ¾ÆÀÌÅÛ ¼ö¸¦ Ã¼Å©À§ÇØ
		{ 
			m_TradeItemNum[i].sSid = -1; 
			m_TradeItemNum[i].sNum = 0; 
		}

		EnterCriticalSection( &m_CS_ExchangeItem );
		for(i = 0; i < m_arExchangeItem.GetSize(); i++)
		{
			if ( m_arExchangeItem[i] )
			{
				delete m_arExchangeItem[i];
				m_arExchangeItem[i] = NULL;
			}
		}
		m_arExchangeItem.RemoveAll();
		LeaveCriticalSection( &m_CS_ExchangeItem );
	}
}

///////////////////////////////////////////////////////////////////////////////
//	±³È¯Ã¢¿¡ ¾ÆÀÌÅÛ ¿Ã¸²
//
void USER::SendExchangeItem(USER *pUser, BYTE bySlot, DWORD dwCount)
{
	int j;
	short sCount;
	short sSid = -1;

	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return;

	CBufferEx TempBuf, TempMyBuf;

	//----------------------------[±Ý¾× ]------------------------------//
	if(bySlot == 255)
	{
		DWORD tempDN = 0;
		if(m_dwDN < dwCount) { tempDN = 0; m_dwDN = 0; }
		else  tempDN = m_dwDN - dwCount;

		TempBuf.Add(EXCHANGE_REQ);				
		TempBuf.Add(m_uid + USER_BAND);
		TempBuf.Add((BYTE)0x06);				// »ó´ëÆí¿¡°Ô µ·À» º¸³½´Ù.
		TempBuf.Add(dwCount);

		TempMyBuf.Add(EXCHANGE_REQ);			// ÀÚ±â¿¡°Ô º¸³½´Ù
		TempMyBuf.Add(pUser->m_uid + USER_BAND);
		TempMyBuf.Add((BYTE)0x05);
		TempMyBuf.Add(dwCount);
		TempMyBuf.Add(tempDN);
		goto go_result;
	}

	//----------------------------[»ó´ë¹æ]------------------------------//
	TempBuf.Add(EXCHANGE_REQ);					// »ó´ëÆí¿¡°Ô º¸³½´Ù.
	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.Add((BYTE)0x04);

	TempBuf.Add(m_UserItem[bySlot].sLevel);
	TempBuf.Add(m_UserItem[bySlot].sSid);
	TempBuf.Add(m_UserItem[bySlot].sDuration);
	TempBuf.Add(m_UserItem[bySlot].sBullNum);
	TempBuf.Add((short)dwCount);
	for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[bySlot].tMagic[j]);
	TempBuf.Add(m_UserItem[bySlot].tIQ); 

	//----------------------------[ÀÚ±â ]------------------------------//
	TempMyBuf.Add(EXCHANGE_REQ);				// Æ®·¹ÀÌµå¿¡ ¿Ã¶ó°¥ ¾ÆÀÌÅÛ
	TempMyBuf.Add(pUser->m_uid + USER_BAND);	
	TempMyBuf.Add((BYTE)0x03);				
	
	TempMyBuf.Add(m_UserItem[bySlot].sLevel);
	TempMyBuf.Add(m_UserItem[bySlot].sSid);
	TempMyBuf.Add(m_UserItem[bySlot].sDuration);
	TempMyBuf.Add(m_UserItem[bySlot].sBullNum);
	TempMyBuf.Add((short)dwCount);
	for(j =0; j < MAGIC_NUM; j++) TempMyBuf.Add(m_UserItem[bySlot].tMagic[j]);
	TempMyBuf.Add(m_UserItem[bySlot].tIQ); 
	
	sCount = m_TradeItemNum[bySlot].sNum;// - (short)dwCount;//m_UserItem[bySlot].sCount - (short)dwCount;
	TempMyBuf.Add(bySlot);						// ÀÎº¥¿¡¼­ º¯È­µÈ ¾ÆÀÌÅÛ
	
	TempMyBuf.Add(m_UserItem[bySlot].sLevel);

	if(sCount <= 0) TempMyBuf.Add(sSid);
	else			TempMyBuf.Add(m_UserItem[bySlot].sSid);

	TempMyBuf.Add(m_UserItem[bySlot].sDuration);
	TempMyBuf.Add(m_UserItem[bySlot].sBullNum);
	TempMyBuf.Add(sCount);
	for(j =0; j < MAGIC_NUM; j++) TempMyBuf.Add(m_UserItem[bySlot].tMagic[j]);
	TempMyBuf.Add(m_UserItem[bySlot].tIQ); 

go_result:
	pUser->Send(TempBuf, TempBuf.GetLength());
	Send(TempMyBuf, TempMyBuf.GetLength());
}

///////////////////////////////////////////////////////////////////////////////
//	½»Ò×½á¹û
void USER::RunExchange(USER* pUser)
{
	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) 
	{
		SendExchangeFail((BYTE)0x00, ERR_1);
		return;
	}
	if( pUser->m_bSessionOnline == true)//ÀëÏßÉÌµê MarkJay
	{
		SendExchangeFail((BYTE)0x00, ERR_1);
		return;
	}
	if(pUser->m_bPShopOpen == TRUE || m_bPShopOpen == TRUE) 
	{
		SendExchangeFail((BYTE)0x00, ERR_1);
		return;
	}
	if ( pUser->m_UserStatus == 0 && pUser->m_UserFlag == FALSE )
	{
		SendExchangeFail((BYTE)0x00, ERR_1);
		return;
	}

	BYTE	result		= 0x01;				// ±³È¯µ¿ÀÇ
	BYTE	tNumberMy	= m_arExchangeItem.GetSize();
	BYTE	tNumberYour	= pUser->m_arExchangeItem.GetSize();

	ItemList		MyItem[TOTAL_ITEM_NUM], YourItem[TOTAL_ITEM_NUM];
	ItemList		TempItem;

	ItemList*		pNewItemList = NULL;
	ItemListArray	arItemListMy, arItemListYour;
	CByteArray		arSlotMy, arSlotYour;
	CString			myItem_str ,youItem_str;

	int i = 0, j = 0;
	int iMyWeight = 0, iYourWeight = 0;

	for(i = 0; i < TOTAL_ITEM_NUM; i++)			// ¾ÆÀÌÅÛ Á¤º¸ ¹é¾÷
	{
		MyItem[i] = m_UserItem[i];
		YourItem[i] = pUser->m_UserItem[i];
	}

	iMyWeight = m_iCurWeight;							// ÇöÀç ¾ÆÀÌÅÛ ¹«°Ô¸¦ ¹é¾÷
	iYourWeight = pUser->m_iCurWeight;

	//----------------------------[init values]------------------------------//
	CBufferEx	TempBuf;
	CBufferEx	TempBuf2;
	CBufferEx	TempBuf3;
	CBufferEx	TempBuf4;

	int			iWear = -1;
	int			nSizeYour = 0;
	int			nSizeMy = 0;
	BOOL		bIsOk = TRUE;
	int			iQuickChange = 0, yQuickChange = 0;
	int			iSameSlot = -1, iEmptySlot = -1;
	short		sSid = -1;
	BYTE		tSlot = 0;
	short		sCount = 0;
	DWORD		dwMyTempDN = 0, dwYourTempDN = 0;
	BOOL		bChangeMyEquip = TRUE, bChangeYourEquip = TRUE;
	//----------------------------[ User ]-----------------------------------//

	if((tNumberYour == 0 && tNumberMy == 0) && (m_TradeMoney <= 0 && pUser->m_TradeMoney <= 0))
	{
		SendExchangeFail(0x00, ERR_3);			// µÑ´Ù ¾ÆÀÌÅÛ, µ· À» ¾È¿Ã¸±°æ¿ì
		pUser->SendExchangeFail(0x00, ERR_3);
		goto go_result;
	}
	
	EnterCriticalSection( &m_CS_ExchangeItem );
	//tNumberMy = m_arExchangeItem.GetSize();
	for(i = 0; i < m_arExchangeItem.GetSize(); i++)				// ÀÚ½ÅÀÌ ÁÖ´Â ¾ÆÀÌÅÛ Ã³¸®
	{
		if ( m_arExchangeItem[i] == NULL ) continue;

		tSlot = (BYTE)m_arExchangeItem[i]->bySlot;

		if(tSlot < EQUIP_ITEM_NUM || tSlot >= TOTAL_INVEN_MAX) { bIsOk = FALSE; break;}
		sCount = m_arExchangeItem[i]->changedList.sCount;
		if(sCount <= 0)  { bIsOk = FALSE; break;}							//×Ô¼ºµÄÊý¾ÝÎª0,ÎÞ·¨½øÐÐ½»Ò×
		if(sCount > m_UserItem[tSlot].sCount) {bIsOk = FALSE ; break;}		//½»Ò×ÊýÁ¿³¬³ö×Ô¼ºµÄÎïÆ·ÊýÁ¿

		if(m_UserItem[tSlot].sSid < 0 || m_UserItem[tSlot].sSid >= g_arItemTable.GetSize())	{ bIsOk = FALSE; break;}
		
		m_iCurWeight -= (g_arItemTable[m_UserItem[tSlot].sSid]->m_byWeight * sCount);
		if(m_iCurWeight < 0) m_iCurWeight = 0;

		// ÁÖ´Â ¾ÆÀÌÅÛÀ» ·Î±ëÇÑ´Ù
		// ¾ÆÀÌÅÛÀÌ º¯°æµÇ±â Àü¿¡ ·Î±ëÀ» ³²±ä´Ù
		MakeItemLog( &m_UserItem[tSlot], ITEMLOG_EXCHANGE_GIVE, pUser->m_strUserID );

		m_UserItem[tSlot].sCount -= sCount;
		if(m_UserItem[tSlot].sCount <= 0) 
		{
			ReSetItemSlot(&m_UserItem[tSlot]);
			//m_UserItem[tSlot].sCount = 0;
			//m_UserItem[tSlot].sSid = -1;
		}

		pNewItemList = new ItemList;
		pNewItemList->sSid = m_UserItem[tSlot].sSid;
		pNewItemList->sLevel = m_UserItem[tSlot].sLevel;
		pNewItemList->sBullNum = m_UserItem[tSlot].sBullNum;
		pNewItemList->sDuration = m_UserItem[tSlot].sDuration;
		pNewItemList->sCount = m_UserItem[tSlot].sCount;

		for(j =0; j < MAGIC_NUM; j++) pNewItemList->tMagic[j] = m_UserItem[tSlot].tMagic[j];
		pNewItemList->tIQ = m_UserItem[tSlot].tIQ; 
		pNewItemList->iItemSerial = m_UserItem[tSlot].iItemSerial;

		arItemListMy.Add(pNewItemList);
		arSlotMy.Add(tSlot);
	}
	LeaveCriticalSection( &m_CS_ExchangeItem );

	EnterCriticalSection( &pUser->m_CS_ExchangeItem );
	//tNumberYour	= pUser->m_arExchangeItem.GetSize();
	for(i = 0; i < pUser->m_arExchangeItem.GetSize(); i++)		// ³²ÀÌ ÁÖ´Â ¾ÆÀÌÅÛ Ã³¸®
	{
		if ( pUser->m_arExchangeItem[i] == NULL ) continue;

		tSlot = (BYTE)pUser->m_arExchangeItem[i]->bySlot;
		if(tSlot < EQUIP_ITEM_NUM || tSlot >= TOTAL_INVEN_MAX) { bIsOk = FALSE; break;}
		sCount = pUser->m_arExchangeItem[i]->changedList.sCount;
		if(sCount <= 0 ) { bIsOk = FALSE; break;}								//½»Ò×ÊýÆ·²»ÄÜÎª0
		if(sCount > pUser->m_UserItem[tSlot].sCount) {bIsOk = FALSE; break;}	//½»Ò×ÊýÆ·ÊýÁ¿³¬³ö×Ô¼ºÓÐµÄÊýÁ¿

		ReSetItemSlot(&TempItem);
		TempItem			= pUser->m_UserItem[tSlot];
		TempItem.sSid		= pUser->m_arExchangeItem[i]->changedList.sSid;
		TempItem.sLevel		= pUser->m_arExchangeItem[i]->changedList.sLevel;
		TempItem.sBullNum	= pUser->m_arExchangeItem[i]->changedList.sBullNum;
		TempItem.sDuration	= pUser->m_arExchangeItem[i]->changedList.sDuration;
		TempItem.sCount		= sCount;

		for(j =0; j < MAGIC_NUM; j++) TempItem.tMagic[j] = pUser->m_arExchangeItem[i]->changedList.tMagic[j];
		TempItem.tIQ = pUser->m_arExchangeItem[i]->changedList.tIQ;
		TempItem.iItemSerial = pUser->m_arExchangeItem[i]->changedList.iItemSerial;

		if(TempItem.sSid < 0 || TempItem.sSid >= g_arItemTable.GetSize()) { bIsOk = FALSE; break;}
		iSameSlot = GetSameItem(TempItem, INVENTORY_SLOT);

		if(iSameSlot == -1)//±³°üÃ»·¢ÏÖÓÐ
		{
			iEmptySlot = GetEmptySlot(INVENTORY_SLOT);
			if(iEmptySlot == -1)			// °°Àº Á¾·ùÀÇ ¾ÆÀÌÅÛµµ ¾ø°í ºó½½·Ôµµ ¾øÀ» °æ¿ì
			{
				bIsOk = FALSE;
				break;
			}

			// ºó½½·ÔÀÌ ÀÖÀ» °æ¿ì
			m_UserItem[iEmptySlot] = TempItem;

			MakeItemLog( &m_UserItem[iEmptySlot], ITEMLOG_EXCHANGE_RECEIVE, pUser->m_strUserID );

			pNewItemList = new ItemList;
			pNewItemList->sSid = m_UserItem[iEmptySlot].sSid;
			pNewItemList->sLevel = m_UserItem[iEmptySlot].sLevel;
			pNewItemList->sBullNum = m_UserItem[iEmptySlot].sBullNum;
			pNewItemList->sDuration = m_UserItem[iEmptySlot].sDuration;
			pNewItemList->sCount = m_UserItem[iEmptySlot].sCount;
	
			for(j =0; j < MAGIC_NUM; j++) pNewItemList->tMagic[j] = m_UserItem[iEmptySlot].tMagic[j];
			pNewItemList->tIQ = m_UserItem[iEmptySlot].tIQ; 
			pNewItemList->iItemSerial = m_UserItem[iEmptySlot].iItemSerial;
			
			arItemListMy.Add(pNewItemList);
			arSlotMy.Add((BYTE)iEmptySlot);
			if(m_UserItem[iEmptySlot].sSid > 0 && m_UserItem[iEmptySlot].sSid < g_arItemTable.GetSize())
				youItem_str.Format(youItem_str+"[%s]%d¸öa<%s>ÊôÐÔ±àºÅ %d %d %d %d %d %d %d %d %d %d,",g_arItemTable[m_UserItem[iEmptySlot].sSid]->m_strName,TempItem.sCount,pUser->m_strUserID,m_UserItem[iEmptySlot].sSid,TempItem.sCount,m_UserItem[iEmptySlot].tMagic[5],m_UserItem[iEmptySlot].tIQ,m_UserItem[iEmptySlot].tMagic[0],m_UserItem[iEmptySlot].tMagic[1],m_UserItem[iEmptySlot].tMagic[2],m_UserItem[iEmptySlot].tMagic[3],m_UserItem[iEmptySlot].tMagic[4],m_UserItem[iEmptySlot].tMagic[6]); //¼ÇÂ¼½»Ò×ÎïÆ·
		}
		else//±³°ü·¢ÏÖÓÐ
		{
			if(CheckMaxValueReturn((DWORD &)m_UserItem[iSameSlot].sCount ,(DWORD)TempItem.sCount) == FALSE)
			{
				//³¬³ö×î´óÊýÁ¿.
				bIsOk = FALSE;
				break;
			}
			m_UserItem[iSameSlot].sCount += TempItem.sCount;

			pNewItemList = new ItemList;
			pNewItemList->sSid = m_UserItem[iSameSlot].sSid;
			pNewItemList->sLevel = m_UserItem[iSameSlot].sLevel;
			pNewItemList->sBullNum = m_UserItem[iSameSlot].sBullNum;
			pNewItemList->sDuration = m_UserItem[iSameSlot].sDuration;
			pNewItemList->sCount = m_UserItem[iSameSlot].sCount;
		
			for(j =0; j < MAGIC_NUM; j++) pNewItemList->tMagic[j] = m_UserItem[iSameSlot].tMagic[j];
			pNewItemList->tIQ = m_UserItem[iSameSlot].tIQ; 
			pNewItemList->iItemSerial = m_UserItem[iSameSlot].iItemSerial;

			arItemListMy.Add(pNewItemList);
			arSlotMy.Add((BYTE)iSameSlot);
			if(m_UserItem[iSameSlot].sSid > 0 && m_UserItem[iSameSlot].sSid < g_arItemTable.GetSize())
				youItem_str.Format(youItem_str+"[%s]%d¸öb<%s>ÊôÐÔ±àºÅ %d %d %d %d %d %d %d %d %d %d,",g_arItemTable[m_UserItem[iSameSlot].sSid]->m_strName,TempItem.sCount,pUser->m_strUserID,m_UserItem[iSameSlot].sSid,TempItem.sCount,m_UserItem[iSameSlot].tMagic[5],m_UserItem[iSameSlot].tIQ,m_UserItem[iSameSlot].tMagic[0],m_UserItem[iSameSlot].tMagic[1],m_UserItem[iSameSlot].tMagic[2],m_UserItem[iSameSlot].tMagic[3],m_UserItem[iSameSlot].tMagic[4],m_UserItem[iSameSlot].tMagic[6]); //¼ÇÂ¼½»Ò×ÎïÆ·
		}

		if(TempItem.sSid >= 0) m_iCurWeight += g_arItemTable[TempItem.sSid]->m_byWeight * TempItem.sCount;
		if(m_iCurWeight < 0) m_iCurWeight = 0;
	}
	LeaveCriticalSection( &pUser->m_CS_ExchangeItem );

	if(bIsOk == FALSE)
	{
		for(i = 0; i < TOTAL_ITEM_NUM; i++)		// Item Information Backup
		{
			m_UserItem[i] = MyItem[i];
			pUser->m_UserItem[i] = YourItem[i];
		}
		SendExchangeFail(0x00, ERR_6);
		pUser->SendExchangeFail(0x00, ERR_7);
		goto go_result;
	}

	//-----------------------------[Send User]----------------------------------//		
	EnterCriticalSection( &pUser->m_CS_ExchangeItem );
	//tNumberYour	= pUser->m_arExchangeItem.GetSize();
	for(i = 0; i < pUser->m_arExchangeItem.GetSize(); i++)			// ÀÚ½ÅÀÌ ÁÖ´Â ¾ÆÀÌÅÛ Ã³¸®
	{
		if ( pUser->m_arExchangeItem[i] == NULL ) continue;

		tSlot = (BYTE)pUser->m_arExchangeItem[i]->bySlot;
		if(tSlot < EQUIP_ITEM_NUM || tSlot >= TOTAL_INVEN_MAX) { bIsOk = FALSE; break;}

		sCount = pUser->m_arExchangeItem[i]->changedList.sCount;
		if(sCount <= 0) { bIsOk = FALSE; break;}										//½»Ò×ÊýÆ·²»ÄÜÎª0
		if(sCount > pUser->m_UserItem[tSlot].sCount) { bIsOk = FALSE; break;}		//½»Ò×ÎïÆ·³¬³öÁË¶Ô·½ÓÐµÄÊýÁ¿

		if(pUser->m_UserItem[tSlot].sSid < 0 || pUser->m_UserItem[tSlot].sSid >= g_arItemTable.GetSize()) { bIsOk = FALSE; break;}

		pUser->m_iCurWeight -= g_arItemTable[pUser->m_UserItem[tSlot].sSid]->m_byWeight * sCount;
		if(pUser->m_iCurWeight < 0) pUser->m_iCurWeight = 0;

		pUser->m_UserItem[tSlot].sCount -= sCount;
		if(pUser->m_UserItem[tSlot].sCount <= 0)
		{
			ReSetItemSlot(&pUser->m_UserItem[tSlot]);
			//pUser->m_UserItem[tSlot].sCount = 0;
			//pUser->m_UserItem[tSlot].sSid = -1;
		}

		pNewItemList = new ItemList;
		pNewItemList->sSid = pUser->m_UserItem[tSlot].sSid;
		pNewItemList->sLevel = pUser->m_UserItem[tSlot].sLevel;
		pNewItemList->sBullNum = pUser->m_UserItem[tSlot].sBullNum;
		pNewItemList->sDuration = pUser->m_UserItem[tSlot].sDuration;
		pNewItemList->sCount = pUser->m_UserItem[tSlot].sCount;

		for(j =0; j < MAGIC_NUM; j++) pNewItemList->tMagic[j] = pUser->m_UserItem[tSlot].tMagic[j];
		pNewItemList->tIQ = pUser->m_UserItem[tSlot].tIQ;
		pNewItemList->iItemSerial = pUser->m_UserItem[tSlot].iItemSerial;

		arItemListYour.Add(pNewItemList);
		arSlotYour.Add(tSlot);
	}
	LeaveCriticalSection( &pUser->m_CS_ExchangeItem );

	EnterCriticalSection( &m_CS_ExchangeItem );
	//tNumberMy = m_arExchangeItem.GetSize();
	for(i = 0; i < m_arExchangeItem.GetSize(); i++)		// ³²ÀÌ ÁÖ´Â ¾ÆÀÌÅÛ Ã³¸®--> arItemListMy¿¡¼­ ¹Þ¾Æ¾ßÇÑ´Ù.
	{
		if ( m_arExchangeItem[i] == NULL ) continue;

		tSlot = (BYTE)m_arExchangeItem[i]->bySlot;

		if(tSlot < EQUIP_ITEM_NUM || tSlot >= TOTAL_INVEN_MAX) { bIsOk = FALSE; break;}

		// À§¿¡¼­ ÁÖ´Â ¾ÆÀÌÅÛÀº º¯µ¿ÀÌ ¾øÀ¸¹Ç·Î ¹Ý¿µÇÑ´Ù.
		TempItem.sLevel		= m_arExchangeItem[i]->changedList.sLevel;
		TempItem.sSid		= m_arExchangeItem[i]->changedList.sSid;
		TempItem.sCount		= m_arExchangeItem[i]->changedList.sCount;
		TempItem.sBullNum	= m_arExchangeItem[i]->changedList.sBullNum;
		TempItem.sDuration	= m_arExchangeItem[i]->changedList.sDuration;

		for(j =0; j < MAGIC_NUM; j++) TempItem.tMagic[j] = m_arExchangeItem[i]->changedList.tMagic[j];
		TempItem.tIQ = m_arExchangeItem[i]->changedList.tIQ;
		TempItem.iItemSerial = m_arExchangeItem[i]->changedList.iItemSerial;

		if(TempItem.sSid < 0 || TempItem.sSid >= g_arItemTable.GetSize()) { bIsOk = FALSE; break;}
		iSameSlot = pUser->GetSameItem(TempItem, INVENTORY_SLOT);

		if(iSameSlot == -1)					// °°Àº Á¾·ùÀÇ ¾ÆÀÌÅÛÀÌ ¾øÀ» °æ¿ì
		{
			iEmptySlot = pUser->GetEmptySlot(INVENTORY_SLOT);
			if(iEmptySlot == -1)			// °°Àº Á¾·ùÀÇ ¾ÆÀÌÅÛµµ ¾ø°í ºó½½·Ôµµ ¾øÀ» °æ¿ì
			{
				bIsOk = FALSE;
				break;
			}

			// ºó½½·ÔÀÌ ÀÖÀ» °æ¿ì
			pUser->m_UserItem[iEmptySlot] = TempItem;

			pNewItemList = new ItemList;
			pNewItemList->sLevel = pUser->m_UserItem[iEmptySlot].sLevel;
			pNewItemList->sSid = pUser->m_UserItem[iEmptySlot].sSid;
			pNewItemList->sBullNum = pUser->m_UserItem[iEmptySlot].sBullNum;
			pNewItemList->sDuration = pUser->m_UserItem[iEmptySlot].sDuration;
			pNewItemList->sCount = pUser->m_UserItem[iEmptySlot].sCount;

			for(j =0; j < MAGIC_NUM; j++) pNewItemList->tMagic[j] = pUser->m_UserItem[iEmptySlot].tMagic[j];
			pNewItemList->tIQ = pUser->m_UserItem[iEmptySlot].tIQ; 
			pNewItemList->iItemSerial = pUser->m_UserItem[iEmptySlot].iItemSerial;
			
			arItemListYour.Add(pNewItemList);
			arSlotYour.Add((BYTE)iEmptySlot);
			if(pUser->m_UserItem[iEmptySlot].sSid > 0 && pUser->m_UserItem[iEmptySlot].sSid < g_arItemTable.GetSize())
				myItem_str.Format(myItem_str+"[%s]%d¸öcÊôÐÔ±àºÅ %d %d %d %d %d %d %d %d %d %d,",g_arItemTable[pUser->m_UserItem[iEmptySlot].sSid]->m_strName,TempItem.sCount,pUser->m_UserItem[iEmptySlot].sSid,TempItem.sCount,pUser->m_UserItem[iEmptySlot].tMagic[5],pUser->m_UserItem[iEmptySlot].tIQ,pUser->m_UserItem[iEmptySlot].tMagic[0],pUser->m_UserItem[iEmptySlot].tMagic[1],pUser->m_UserItem[iEmptySlot].tMagic[2],pUser->m_UserItem[iEmptySlot].tMagic[3],pUser->m_UserItem[iEmptySlot].tMagic[4],pUser->m_UserItem[iEmptySlot].tMagic[6]); //¼ÇÂ¼½»Ò×ÎïÆ·
		}
		else								// °°Àº Á¾·ùÀÇ ¾ÆÀÌÅÛÀÌ ÀÖÀ» °æ¿ì
		{
			if(CheckMaxValueReturn((DWORD &)pUser->m_UserItem[iSameSlot].sCount,(DWORD)TempItem.sCount) == FALSE)
			{
				//³¬³öÁË¶Ô·½½ÓÊÕÎïÆ·µÄ×î´óÊý
				bIsOk = FALSE;
				break;
			}
			pUser->m_UserItem[iSameSlot].sCount += TempItem.sCount;
			
			pNewItemList = new ItemList;
			pNewItemList->sLevel = pUser->m_UserItem[iSameSlot].sLevel;
			pNewItemList->sSid = pUser->m_UserItem[iSameSlot].sSid;
			pNewItemList->sBullNum = pUser->m_UserItem[iSameSlot].sBullNum;
			pNewItemList->sDuration = pUser->m_UserItem[iSameSlot].sDuration;
			pNewItemList->sCount = pUser->m_UserItem[iSameSlot].sCount;

			for(j =0; j < MAGIC_NUM; j++) pNewItemList->tMagic[j] = pUser->m_UserItem[iSameSlot].tMagic[j];
			pNewItemList->tIQ = pUser->m_UserItem[iSameSlot].tIQ; 
			pNewItemList->iItemSerial = pUser->m_UserItem[iSameSlot].iItemSerial;
			
			arItemListYour.Add(pNewItemList);
			arSlotYour.Add((BYTE)iSameSlot);
			if(pUser->m_UserItem[iSameSlot].sSid > 0 && pUser->m_UserItem[iSameSlot].sSid < g_arItemTable.GetSize())
				myItem_str.Format(myItem_str+"[%s]%d¸ödÊôÐÔ±àºÅ %d %d %d %d %d %d %d %d %d %d,",g_arItemTable[pUser->m_UserItem[iSameSlot].sSid]->m_strName,TempItem.sCount,pUser->m_UserItem[iSameSlot].sSid,TempItem.sCount,pUser->m_UserItem[iSameSlot].tMagic[5],pUser->m_UserItem[iSameSlot].tIQ,pUser->m_UserItem[iSameSlot].tMagic[0],pUser->m_UserItem[iSameSlot].tMagic[1],pUser->m_UserItem[iSameSlot].tMagic[2],pUser->m_UserItem[iSameSlot].tMagic[3],pUser->m_UserItem[iSameSlot].tMagic[4],pUser->m_UserItem[iSameSlot].tMagic[6]); //¼ÇÂ¼½»Ò×ÎïÆ·
		}

		if(TempItem.sSid >= 0) pUser->m_iCurWeight += g_arItemTable[TempItem.sSid]->m_byWeight * TempItem.sCount;
	}
	LeaveCriticalSection( &m_CS_ExchangeItem );

	if(bIsOk == FALSE)
	{
		for(i = 0; i < TOTAL_ITEM_NUM; i++)		// Item Information Backup
		{
			m_UserItem[i] = MyItem[i];
			pUser->m_UserItem[i] = YourItem[i];
		}
		SendExchangeFail(0x00, ERR_7);
		pUser->SendExchangeFail(0x00, ERR_6);
		goto go_result;
	}

	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED)// ±âÀûÀûÀ¸·Î »ó´ëÆíÀÌ À¯Àú ¸®½ºÆ®¿¡¼­ »èÁ¦µÇ¾úÀ¸¸é...
	{
		for(i = 0; i < TOTAL_ITEM_NUM; i++)		// Item Information Backup
		{
			m_UserItem[i] = MyItem[i];
		}
		SendExchangeFail(0x00, ERR_5);
		goto go_result;
	}


	//----------------[ ±Ý¾×À» º¯µ¿½ÃÅ²´Ù.]----------------------------------//
	dwMyTempDN = m_dwDN;
	dwYourTempDN = pUser->m_dwDN;
	CheckMaxValue((DWORD &)m_dwDN, (DWORD)pUser->m_TradeMoney);

	MakeMoneyLog( pUser->m_TradeMoney, ITEMLOG_EXCHANGE_RECEIVE_MONEY, pUser->m_strUserID );

	if(m_dwDN <= m_TradeMoney) m_dwDN = 0;
	else m_dwDN = m_dwDN - m_TradeMoney;

	CheckMaxValue((DWORD &)pUser->m_dwDN, (DWORD)m_TradeMoney);

	if(pUser->m_dwDN <= pUser->m_TradeMoney) pUser->m_dwDN = 0;
	else  pUser->m_dwDN = pUser->m_dwDN - pUser->m_TradeMoney;

	MakeMoneyLog( m_TradeMoney, ITEMLOG_EXCHANGE_GIVE_MONEY, pUser->m_strUserID, pUser->m_dwDN );

	// ----------------[ Update Databace ] ----------------------------------//
	if(UpdateUserExchangeData(pUser) == FALSE)
	{
		m_iCurWeight = iMyWeight;
		pUser->m_iCurWeight = iYourWeight;

		m_dwDN = dwMyTempDN;
		pUser->m_dwDN = dwYourTempDN;
		for(i = 0; i < TOTAL_ITEM_NUM; i++)		// »ó´ëÆí ¾÷µ¥ÀÌÆ®°¡ ½ÇÆÐÇÏ¸é ºüÁ®³ª¿Â´Ù.
		{
			m_UserItem[i] = MyItem[i];
			pUser->m_UserItem[i] = YourItem[i];
		}
		SendExchangeFail(0x00, UNKNOWN_ERR);
		pUser->SendExchangeFail(0x00, UNKNOWN_ERR);
		goto go_result;
	}

	//-----------------[ Send Packet ]---------------------------------------//
	nSizeMy = arSlotMy.GetSize();

	TempBuf.Add(EXCHANGE_RESULT);
	TempBuf.Add((BYTE)0x01);
	TempBuf.Add(m_dwDN);
	TempBuf.Add((BYTE)nSizeMy);

	for(i = 0; i < nSizeMy; i++)
	{
		TempBuf.Add((BYTE)arSlotMy[i]);
		TempBuf.Add(arItemListMy[i]->sLevel);
		TempBuf.Add(arItemListMy[i]->sSid);
		TempBuf.Add(arItemListMy[i]->sDuration);
		TempBuf.Add(arItemListMy[i]->sBullNum);
		TempBuf.Add(arItemListMy[i]->sCount);

		for(j =0; j < MAGIC_NUM; j++) TempBuf.Add((BYTE)arItemListMy[i]->tMagic[j]);
		TempBuf.Add((BYTE)arItemListMy[i]->tIQ); 
	}

	Send(TempBuf, TempBuf.GetLength());

	nSizeYour = arSlotYour.GetSize();

	TempBuf2.Add(EXCHANGE_RESULT);
	TempBuf2.Add((BYTE)0x01);
	TempBuf2.Add(pUser->m_dwDN);
	TempBuf2.Add((BYTE)nSizeYour);

	for(i = 0; i < nSizeYour; i++)
	{
		TempBuf2.Add((BYTE)arSlotYour[i]);
		TempBuf2.Add(arItemListYour[i]->sLevel);
		TempBuf2.Add(arItemListYour[i]->sSid);
		TempBuf2.Add(arItemListYour[i]->sDuration);
		TempBuf2.Add(arItemListYour[i]->sBullNum);
		TempBuf2.Add(arItemListYour[i]->sCount);

		for(j =0; j < MAGIC_NUM; j++) TempBuf2.Add((BYTE)arItemListYour[i]->tMagic[j]);
		TempBuf2.Add((BYTE)arItemListYour[i]->tIQ); 
	}

	pUser->Send(TempBuf2, TempBuf2.GetLength());

/*
	if(iQuickChange >= 1)
	{
		sCount = -1;
		TempBuf3.Add(ITEM_MOVE_RESULT);
		TempBuf3.Add((BYTE)0x01);
		TempBuf3.Add((BYTE)0x02);

		TempBuf3.Add((BYTE)iQuickChange);
		for(i = 0; i < QUICK_ITEM; i++)
		{
			if(MyTempQuickItem[i] != -1)
			{
				TempBuf3.Add((BYTE)i);
				TempBuf3.Add(sCount);
			}
		}
		Send(TempBuf3, TempBuf3.GetLength());
	}

	if(yQuickChange >= 1)
	{
		sCount = -1;
		TempBuf4.Add(ITEM_MOVE_RESULT);
		TempBuf4.Add((BYTE)0x01);
		TempBuf4.Add((BYTE)0x02);

		TempBuf4.Add((BYTE)yQuickChange);
		for(i = 0; i < QUICK_ITEM; i++)
		{
			if(YourTempQuickItem[i] != -1)
			{
				TempBuf4.Add((BYTE)i);
				TempBuf4.Add(sCount);
			}
		}
		pUser->Send(TempBuf4, TempBuf4.GetLength());
	}
*/
/*	//	µ· º¯È­°¡ ÀÖÀ¸¸é...^^
	if(pUser->m_TradeMoney > 0 || m_TradeMoney > 0)
	{
		SendMoneyChanged();
		pUser->SendMoneyChanged();
	}
*/
	GetRecoverySpeed();
	pUser->GetRecoverySpeed();

	SendSystemMsg( IDS_USER_OVER_WEIGHT_SAVE_OTHER, SYSTEM_NORMAL, TO_ME);//"±£´æµØµã³É¹¦."
	if(bChangeMyEquip) SendMyInfo(TO_INSIGHT, INFO_MODIFY);
	if(bChangeYourEquip) pUser->SendMyInfo(TO_INSIGHT, INFO_MODIFY);

	FlushItemLog( TRUE );

	if(m_tIsOP != 1)
	{
		SYSTEMTIME st;
		CString strDate;
		GetLocalTime(&st);
		strDate.Format("%d-%d-%d %d:%d",st.wYear,st.wMonth,st.wDay ,st.wHour,st.wMinute);
		CString str = _T("");
		str.Format("%s [%s] Óë [%s] ½»Ò×ÎïÆ· %s ½»Ò×½ð±Ò %d ½ÓÊÕÎïÆ· %s ½ÓÊÕ½ð±Ò %d\r\n",strDate,pUser->m_strUserID, m_strUserID,youItem_str,pUser->m_TradeMoney,myItem_str,m_TradeMoney);
		EnterCriticalSection( &m_CS_FileWrite );
		g_fpMarkDeBug.Write( str, str.GetLength() );
		LeaveCriticalSection( &m_CS_FileWrite);
	}

	//---------------[ Delete Memory ]------------------------------------//
go_result:
	for(i = 0; i < arItemListMy.GetSize(); i++)
	{
		if(arItemListMy[i] != NULL) delete arItemListMy[i];
	}
	arItemListMy.RemoveAll();

	for(i = 0; i < arItemListYour.GetSize(); i++)
	{
		if(arItemListYour[i] != NULL) delete arItemListYour[i];
	}
	arItemListYour.RemoveAll();

	EnterCriticalSection( &m_CS_ExchangeItem );
	for(i = 0; i < m_arExchangeItem.GetSize(); i++)
	{
		if ( m_arExchangeItem[i] != NULL )
		{
			delete m_arExchangeItem[i];
			m_arExchangeItem[i] = NULL;
		}
	}
	m_arExchangeItem.RemoveAll();
	LeaveCriticalSection( &m_CS_ExchangeItem );

	EnterCriticalSection( &pUser->m_CS_ExchangeItem );
	for(i = 0; i < pUser->m_arExchangeItem.GetSize(); i++)
	{
		if(pUser->m_arExchangeItem[i] != NULL)
		{
			delete pUser->m_arExchangeItem[i];
			pUser->m_arExchangeItem[i] = NULL;
		}
	}
	pUser->m_arExchangeItem.RemoveAll();
	LeaveCriticalSection( &pUser->m_CS_ExchangeItem );

	//---------------[ °Å·¡ÈçÀû ÃÊ±âÈ­ ]-------------------------------//
	m_TradeMoney = 0;
	pUser->m_TradeMoney = 0;
	m_bNowTrading = m_bExchangeOk = m_bTradeItem	= FALSE;
	pUser->m_bNowTrading = pUser->m_bExchangeOk = pUser->m_bTradeItem = FALSE;

	_tcscpy(m_strTradeUser, _T(""));		// »ó´ë¹æÀ» È®ÀÎÇÏ±â À§ÇÑ À¯ÀúÀÌ¸§À» Áö¿î´Ù.
	_tcscpy(pUser->m_strTradeUser, _T(""));	// »ó´ë¹æÀ» È®ÀÎÇÏ±â À§ÇÑ À¯ÀúÀÌ¸§À» Áö¿î´Ù.
	m_iTradeUid = -1;						// »ó´ë¹æ uid»èÁ¦
	pUser->m_iTradeUid = -1;				// »ó´ë¹æ uid»èÁ¦

	for(i = 0; i < TOTAL_ITEM_NUM; i++) // °Å·¡Áß ÀÎº¥¿¡ ¹Ù²î´Â ¾ÆÀÌÅÛ ¼ö¸¦ Ã¼Å©À§ÇØ
	{ 
		m_TradeItemNum[i].sSid = -1; 
		m_TradeItemNum[i].sNum = 0; 
	}

	FlushItemLog( FALSE );
}

//////////////////////////////////////////////////////////////////////////////
//	Á¸ Ã¼ÀÎÁö ÇÒ¶§ ³¯¾¾ º¯È­¸¦ ¾Ë¸°´Ù.
//
void USER::SendWeatherInMoveZone()
{
	int tType = 2;
	int iWeather = 1;

	// ÀÌµ¿ÇÏ·Á´Â Á¸ÀÌ ºñ°¡¿Ã¼ö ÀÖ´Â Áö¿ªÀÌ¸é	
	for(int i = 0; i < g_WeatherZone.GetSize(); i++)	
	{
		if(g_WeatherZone[i]->iZone == m_curz)
		{
			if(g_WeatherZone[i]->bRaining == TRUE) SetWeather(tType, iWeather); 
			return ;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//	ÇØ´ç Á¸¿¡ ÀÖ´Â À¯Àú¿¡°Ô ³¯¾¾ º¯È­¸¦ ¾Ë¸°´Ù.
//
//void USER::SetWeather(TCHAR *pBuf)
void USER::SetWeather(int tType, int tOnOff)
{
	CBufferEx TempBuf;
	TempBuf.Add(SET_CLIENT_WORLD);
	TempBuf.Add((BYTE)tType);

	if(tType == 2)	TempBuf.Add((BYTE)tOnOff);
	else			TempBuf.Add((BYTE)g_GameTime);

	Send(TempBuf, TempBuf.GetLength());
}

/*
//////////////////////////////////////////////////////////////////////////////
//	
//
void USER::SendWeatherToZone(BYTE tType, BYTE tOnOff)
{
	CBufferEx TempBuf;
	
	TempBuf.Add(SET_CLIENT_WORLD);

	TempBuf.Add(tType);
	TempBuf.Add(tOnOff);

	Send(TempBuf, TempBuf.GetLength());
}
*/
//##############################################################################
//	Character Event
//##############################################################################

////////////////////////////////////////////////////////////////////////////
//	Client NPC ¿¡¼­ ÀÌº¥Æ® ¼öÇàÀ» ¿äÃ»ÇÑ´Ù.
//
void USER::NpcEvent(TCHAR *pBuf)
{
	int event_zone_index = -1;
	int i, j;
	CNpc*	pNpc = NULL;
	EVENT*	pEvent = NULL;

	int index = 0;
	int nUid = GetInt(pBuf, index);

	if(nUid < 0 || nUid >= INVALID_BAND) return;	// Àß¸øµÈ uid¸é ¸®ÅÏ
	
	if(nUid >= USER_BAND && nUid < NPC_BAND)		// À¯Àú¿¡ ´ëÇÑ ÀÌº¥Æ®
	{
		return;
	}
	
	if(nUid >= NPC_BAND && nUid < INVALID_BAND)		// NPC ¿¡ ´ëÇÑ ÀÌº¥Æ®
	{
		pNpc = GetNpc(nUid - NPC_BAND);
		if(!pNpc ) return;

		if(pNpc->m_sEvent <= 0 ) return;

		for(i = 0; i < g_event.GetSize(); i++)
		{
			if(g_event[i]->m_Zone == m_curz)
			{				
				event_zone_index = i;
				break;
			}
		}

		if(event_zone_index == -1) return;			// ÇöÀç Zone ¿¡´Â define µÈ ÀÌº¥Æ®°¡ ¾ø´Ù.

		pEvent = g_event[event_zone_index];

		if(pEvent == NULL) return;
		if(pNpc->m_sEvent >= pEvent->m_arEvent.GetSize()) return;
		if(pEvent->m_arEvent[pNpc->m_sEvent] == NULL) return;

		if(!CheckEventLogic(pEvent->m_arEvent[pNpc->m_sEvent])) return;	// Á¶°Ç°Ë»ç

		for(j = 0; j < pEvent->m_arEvent[pNpc->m_sEvent]->m_arExec.GetSize(); j++)
		{
			if(RunNpcEvent(pNpc, pEvent->m_arEvent[pNpc->m_sEvent]->m_arExec[j]) == NULL) break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////
//	Client HyperText¿¡¼­ ÀÌº¥Æ® ¼öÇàÀ» ¿äÃ»ÇÑ´Ù.
//
void USER::HyperTextEvent(TCHAR *pBuf)
{
	int event_zone_index = -1;
	int index = 0, i, j;
	EVENT*	pEvent = NULL;

	short sZone = GetShort(pBuf, index);
	short sEventNum = GetShort(pBuf, index);
	TRACE( "Å£Å£DEGBUG NPC ¹¦ÄÜÊÂ¼þºÅ %d\n", sEventNum);
	if(sZone < 0 || sEventNum < 0) return;

	for(i = 0; i < g_event.GetSize(); i++)
	{
		if(g_event[i]->m_Zone == sZone)
		{			
			if(sZone != m_curz)
			{
				return;
//				if(sZone == 2 && (sEventNum >= 600 && sEventNum <= 700)) { event_zone_index = i; break; }
//				else return;
			}
			event_zone_index = i;
			break;
		}
	}

	if(event_zone_index == -1) return;			// ÇöÀç Zone ¿¡´Â define µÈ ÀÌº¥Æ®°¡ ¾ø´Ù.
	
	pEvent = g_event[event_zone_index];

	if(pEvent == NULL) return;
	if(sEventNum >= pEvent->m_arEvent.GetSize()) return;
	if(pEvent->m_arEvent[sEventNum] == NULL) return;

	if(!CheckEventLogic(pEvent->m_arEvent[sEventNum])) return;	// Á¶°Ç°Ë»ç

	for(j = 0; j < pEvent->m_arEvent[sEventNum]->m_arExec.GetSize(); j++)
	{
		if(RunNpcEvent(NULL, pEvent->m_arEvent[sEventNum]->m_arExec[j]) == NULL) break;
	}
}

////////////////////////////////////////////////////////////////////////////
//	ÀÌº¥Æ® Äù½ºÆ®·Î ¸÷ÀÌ ´ã´çÇÏ°í ÀÖ´Â ÀÌº¥Æ®¸¦ ½ÇÇàÇÑ´Ù.
//
void USER::RunQuestEvent(CNpc *pNpc, int iZone, int iEventNum)
{
	if(!pNpc) return;

	int event_zone_index = -1;
	int index = 0, i, j;
	EVENT*	pEvent = NULL;

	if(iZone < 0) return;		// ÀÌº¥Æ® ¹øÈ£´Â 1ºÎÅÍ 10000¹ø±îÁö 
	if(iEventNum <= 0 || iEventNum > NPC_QUEST_MOP) return;

	for(i = 0; i < g_event.GetSize(); i++)
	{
		if(g_event[i]->m_Zone == iZone)
		{			
			if(iZone != m_curz)	return;

			event_zone_index = i;
			break;
		}
	}

	if(event_zone_index == -1) return;			// ÇöÀç Zone ¿¡´Â define µÈ ÀÌº¥Æ®°¡ ¾ø´Ù.
	
	pEvent = g_event[event_zone_index];

	if(pEvent == NULL) return;
	if(iEventNum >= pEvent->m_arEvent.GetSize()) return;
	if(pEvent->m_arEvent[iEventNum] == NULL) return;

	if(!CheckEventLogic(pEvent->m_arEvent[iEventNum])) return;	// Á¶°Ç°Ë»ç

	for(j = 0; j < pEvent->m_arEvent[iEventNum]->m_arExec.GetSize(); j++)
	{
		if(RunNpcEvent(pNpc, pEvent->m_arEvent[iEventNum]->m_arExec[j]) == NULL) break;
	}
}

////////////////////////////////////////////////////////////////////////////
//	Client ¿¡¼­ ÀÌº¥Æ® ¼öÇàÀ» ¿äÃ»ÇÑ´Ù.
//
void USER::ClientEvent(TCHAR *pBuf)
{
	int index = 0;

	BYTE tType = GetByte(pBuf, index);

	if(tType == 1) NpcEvent(pBuf + index);

	if(tType == 2) HyperTextEvent(pBuf + index);

}

/////////////////////////////////////////////////////////////////////////
//	ÀÌº¥Æ®¿¡ Á¶°ÇÀÌ ÀÖ´ÂÁö °Ë»çÇÑ´Ù.
//
BOOL USER::CheckEventLogic(EVENT_DATA *pEventData)
{
	if(pEventData == NULL) return FALSE;

	BOOL bExact = FALSE;

	for( int i = 0; i < pEventData->m_arLogicElse.GetSize(); i++)
	{
		bExact = FALSE;

		LOGIC_ELSE* pLE = pEventData->m_arLogicElse[i];		if( !pLE ) return FALSE;

		switch( pLE->m_LogicElse )
		{
		case LOGIC_LEVEL:
			if( CheckLevel(pLE->m_LogicElseInt[0], pLE->m_LogicElseInt[1]) ) bExact = TRUE; 
			break;

		case LOGIC_VIP://NPC¼ì²â»áÔ±
			if( CheckVip()) bExact = TRUE; 
			break;

		case LOGIC_PKDASAI://PK´óÈü
			if( CheckPKDASAI()) bExact = TRUE; 
			break;

		case LOGIC_EXIST_EVENT:
			if( FindEvent( pLE->m_LogicElseInt[0] ) ) bExact = TRUE;
			break;

		case LOGIC_ABSENT_EVENT:
			if( !FindEvent( pLE->m_LogicElseInt[0] ) ) bExact = TRUE;
			break;

		case LOGIC_CLASS:
			if( CheckClass( pLE->m_LogicElseInt[0] ) ) bExact = TRUE;
			break;

		case LOGIC_EXIST_ITEM:
			if( FindItem( pLE->m_LogicElseInt[0]) >= pLE->m_LogicElseInt[1] ) bExact = TRUE;
			break;

		case LOGIC_EXIST_EVENT_ITEM:
			if( FindEventItem( pLE->m_LogicElseInt[0], pLE->m_LogicElseInt[1] ) ) bExact = TRUE;
			break;
		case LOGIC_EXIST_SKILL:
			if( FindSkill( pLE->m_LogicElseInt[0] ) ) bExact = TRUE;
			break;
		case LOGIC_ABSENT_ITEM:
			if( !FindItem( pLE->m_LogicElseInt[0]) ) bExact = TRUE;
			break;

		case LOGIC_WEIGHT:
			if( CheckItemWeight( pLE->m_LogicElseInt[0], pLE->m_LogicElseInt[1], pLE->m_LogicElseInt[2],
													pLE->m_LogicElseInt[3], pLE->m_LogicElseInt[4],
													pLE->m_LogicElseInt[5], pLE->m_LogicElseInt[6],
													pLE->m_LogicElseInt[7], pLE->m_LogicElseInt[8],
													pLE->m_LogicElseInt[9], pLE->m_LogicElseInt[10]	)) bExact = TRUE;
			break;

		case LOGIC_CHECK_ITEMSLOT:
			if( CheckItemSlot( pLE->m_LogicElseInt[0], pLE) ) bExact = TRUE;
			break;

		case LOGIC_EXIST_GM:
			if( CheckGuildMaster() ) bExact = TRUE;
			break;

		case LOGIC_OVER_GUILD_USER_COUNT:
			if( CheckOverGuildUserCount( pLE->m_LogicElseInt[0] ) ) bExact = TRUE;
			break;

		case LOGIC_ZONE_WHO:
			if( CheckZoneWho(pLE->m_LogicElseInt[0], pLE->m_LogicElseInt[1], pLE->m_LogicElseInt[2]) ) bExact = TRUE;
			break;

		case LOGIC_KILL_GH:
			break;

		case LOGIC_MASS_ZONE_MOVE:
			break;

		case LOGIC_TERM:
			if( CheckTerm( pLE->m_LogicElseInt[0], pLE->m_LogicElseInt[1], pLE->m_LogicElseInt[2]) ) bExact = TRUE; 
			break;
/*		case LOGIC_DN:		// Á¶°Ç DNº¸´Ù ¸¹À»°æ¿ì
			if(m_dwDN >= (DWORD)pLE->m_LogicElseInt[0] ) bExact = TRUE;
			break;
*/
		case LOGIC_MONEY:
			if( CheckMoneyMinMax( pLE->m_LogicElseInt[0], pLE->m_LogicElseInt[1] ) ) bExact = TRUE;
			break;

		case LOGIC_YUANBAO://NPC²éÑ¯Ôª±¦
			if( CheckYuanBaoMinMax( pLE->m_LogicElseInt[0], pLE->m_LogicElseInt[1] ) ) bExact = TRUE;
			break;

		case LOGIC_CHECK_CV:
			if( CheckUserCityValue( pLE->m_LogicElseInt[0], pLE->m_LogicElseInt[1]) ) bExact = TRUE;
			break;

		case LOGIC_GUILD_HOUSE_NUMBER:
			if(CheckGuildHouseUser(pLE->m_LogicElseInt[0])) bExact = TRUE;
			break;

		case LOGIC_RAND:
			if( CheckRandom( pLE->m_LogicElseInt[0] ) ) bExact = TRUE;
			break;

		case LOGIC_RR_TIME:
			if( CheckRoyalRumbleEnterTime() ) bExact = TRUE;
			break;

		case LOGIC_RR_MAX_USER:
			if( CheckRoyalRumbleEnterMaxUser() ) bExact = TRUE;
			break;

		default:
			return FALSE;
		}

		if( !pLE->m_bAnd )		// OR Á¶°ÇÀÏ °æ¿ì bExact°¡ TRUEÀÌ¸é ÀüÃ¼°¡ TRUEÀÌ´Ù
		{
			if( bExact )
			{
				return TRUE;
			}
		}
		else					// AND Á¶°ÇÀÏ °æ¿ì bExact°¡ FALSEÀÌ¸é ÀüÃ¼°¡ FALSEÀÌ´Ù
		{
			if( !bExact )
			{
				return FALSE;
			}
		}
	}

	return TRUE;		// ¾Æ¹«·± Á¶°ÇÀÌ ¾øÀ½¸é TRUE ¸®ÅÏ
}

////////////////////////////////////////////////////////////////////////////
//	ÇØ´ç À¯Àú°¡ Á¦ÇÑ ½Ã¹Î µî±ÞÀÎÁö ÆÇ´Ü
//
BOOL USER::CheckUserCityValue(int min, int max)
{
	if(m_sCityRank >= min && m_sCityRank < max ) return TRUE;

	return FALSE;
}

////////////////////////////////////////////////////////////////////////////
//	ÇØ´ç À¯Àú°¡ Á¦ÇÑ ·¹º§¿¡ µÇ´ÂÁö
//
BOOL USER::CheckLevel(int min, int max)
{
	if(m_sLevel >= min && m_sLevel < max ) return TRUE;
	return FALSE;
}

BOOL USER::CheckVip()//NPC¼ì²â»áÔ±
{
	if(m_dwVIPTime != 0)
	{
		SendSystemMsg( "ÄúÊÇVIPÓÃ»§,¿ÉÒÔÃâ·Ñ½øÈë.", SYSTEM_ERROR, TO_ME);
		ZoneMoveReq(317,100,100);
		return TRUE;
	}
	else if(m_dwDN >= 10000000)
	{
		m_dwDN = m_dwDN - 10000000;
		//SendUserStatusSkill();
		//UpdateUserItemShopPingYB();
		UpdateUserItemDN();// ¸üÐÂ½ðÇ®±ä»¯
		SendMoneyChanged();
		ZoneMoveReq(317,100,100);
		return TRUE;
	}
	SendSystemMsg("Äú²»ÊÇVIP»ò¾öÕ½±Ò²»×ã10000000 ,ÎÞ·¨½øÈë.", SYSTEM_ERROR, TO_ME);
	return FALSE;
}
BOOL USER::CheckPKDASAI()
{
	if(f_Markini[0]->PK_DSKG == 1)
	{
		ZoneMoveReq(91, 22, 22);
		return TRUE;
	}
	else if(m_bNowBuddy == TRUE)
	{
		SendSystemMsg("PK´óÈü²»ÄÜ×é¶Ó½øÈë!", SYSTEM_ERROR, TO_ME);
		return FALSE;
	}
	else
	{
		SendSystemMsg("PK´óÈü»¹Ã»ÓÐ¿ª·Å½øÈë!", SYSTEM_ERROR, TO_ME);
		return FALSE;
	}
}

////////////////////////////////////////////////////////////////////////////
//	ÇØ´ç À¯Àú°¡ ±æ¸¶ÀÎÁö Ã¼Å©
//
BOOL USER::CheckGuildMaster()
{
	if( m_bGuildMaster ) return TRUE;

	SendSystemMsg( IDS_USER_ONLY_GUILD_MASTER_USE, SYSTEM_ERROR, TO_ME);//"Ö»ÄÜÓÉ¾üÍÅ³¤Ê¹ÓÃ."

	return FALSE;
}

////////////////////////////////////////////////////////////////////////////
//	ÀÏÁ¤ ³¯Â¥¿¡¼­ ½ÇÇàµÇ´ÂÁö Ã¼Å©
//
BOOL USER::CheckTerm(int start, int end, int say)
{
	SYSTEMTIME guildTime;
	GetLocalTime(&guildTime);

	if( guildTime.wDay >= start	&& guildTime.wDay <= end ) return TRUE;

	return FALSE;
}

////////////////////////////////////////////////////////////////////////////
//	ÃÖ´ë 20¸íÀ» ÀÌµ¿ ½ÃÅ²´Ù.
//
BOOL USER::MassZoneMove(int cityNum)
{
/*	InterlockedExchange(&g_arGuildHouseWar[cityNum]->m_CurrentGuild.lUsed, 0);

	g_arGuildHouseWar[cityNum]->m_CurrentGuild.arUserList.RemoveAll();
	g_arGuildHouseWar[cityNum]->m_CurrentGuild.lGuild = 0;
	g_arGuildHouseWar[cityNum]->m_CurrentGuild.iCurValue = 0;
	g_arGuildHouseWar[cityNum]->m_CurrentGuild.dwTimer = 0;

*/
	return FALSE;
}

////////////////////////////////////////////////////////////////////////////
//	À¯Àú°¡ ¼ÒÀ¯ÇÑ EventÁß ÇØ´ç Event°¡ ÀÖ´ÂÁö Ã£´Â´Ù.
//
BOOL USER::FindEvent(int event_num)
{
	for(int i = 0; i < m_arEventNum.GetSize(); i++)
	{
		TRACE("ÈÎÎñºÅ:%d \n", *(m_arEventNum[i]));
		if( *(m_arEventNum[i]) == event_num )
		{
			return TRUE;
		}
	}

	return FALSE;
}
BOOL USER::FindSkill(int Skill_num)
{
	int i;
	int iIndex = m_byClass * SKILL_NUM;
	
	if(m_sSkillPoint != 0)
	{
		SendSystemMsg("ÄúµÄ¼¼ÄÜµã»¹Ã»¼ÓÍê,Çë¼ÓÍê¼¼ÄÜµãÔÙ×ª»»!", SYSTEM_ERROR, TO_ME);
		return FALSE;
	}
	for(i = iIndex; i < iIndex + SKILL_NUM; i++)
	{
		if(m_UserSkill[i].sSid == Skill_num) return TRUE;
	}
	return FALSE;
}
void USER::AddSkill (int Skill_num)
{
	m_SKill = Skill_num;
}
// 120¼¼ÄÜ×ª»»
void USER::RunSkill120(int Skill)
{
	int i,count = 0,sLevel = m_sLevel;
	int iIndex = m_byClass *SKILL_NUM;
	
	if(Skill != 20 && Skill != 21 && Skill != 22 && Skill != 23 && Skill != 24) return;
	for(i = iIndex; i < iIndex + SKILL_NUM; i++)
	{
		if(m_UserSkill[i].sSid >= 20 && m_UserSkill[i].sSid <= 24) count++;
	}
	if(count >= 2) return;

	for(i = iIndex; i < iIndex + SKILL_NUM; i++)
	{
		if(m_UserSkill[i].sSid == m_SKill) break;
	}
	if(i >= iIndex + SKILL_NUM) return;

	if(sLevel > 130) sLevel = 129;
	m_UserSkill[i].sSid = Skill;
	m_UserSkill[i].tLevel = m_UserSkill[i].tLevel/2;
	if(sLevel >= 120) m_UserSkill[i].tLevel = m_UserSkill[i].tLevel+(sLevel-119);
	if(m_UserSkill[i].tLevel > 20) m_UserSkill[i].tLevel = 20;//ÐÞ¸´¿¨Ò»µãÅ­»÷ MarkJay
	SendUserStatusSkill();
	return;

}
////////////////////////////////////////////////////////////////////////////
//	À¯Àú°¡ ¼ÒÀ¯ÇÑ ItemÁß ÇØ´ç ItemÀÌ ÀÖ´ÂÁö Ã£´Â´Ù.
//
int USER::FindItem(int sid)
{
	if(sid < 0 || sid >= g_arItemTable.GetSize()) return FALSE;

	int i, j, iNum = -1;
	ItemList TempItem;

	ReSetItemSlot(&TempItem);

	TempItem.sSid		= g_arItemTable[sid]->m_sSid;
	TempItem.sBullNum	= g_arItemTable[sid]->m_sBullNum;
	TempItem.sDuration	= g_arItemTable[sid]->m_sDuration;

	for(i =0; i < MAGIC_NUM; i++) TempItem.tMagic[i] = 0;
	TempItem.tIQ = NORMAL_ITEM;
	if(sid == 755)
	{
		TempItem.tIQ = 2;
		TempItem.tMagic[0] = 137;
	}
	
//	iNum = GetSameItem(TempItem, INVENTORY_SLOT);
	for(i = EQUIP_ITEM_NUM; i < EQUIP_ITEM_NUM + INVENTORY_NUM; i++)
	{
		if(m_UserItem[i].sSid == TempItem.sSid)
		{
			if( m_UserItem[i].sSid != 796 )		// ÀÌº¥Æ® º¹±ÇÀÏ °æ¿ì ¸ÅÁ÷ ¹øÈ£³ª ¿É¼ÇÀ» Ã¼Å©ÇÏÁö ¾Ê´Â´Ù.
			{
				for(j = 0; j < MAGIC_NUM; j++)
				{
					if(m_UserItem[i].tMagic[j] != TempItem.tMagic[j]) break;
				}
					
				if(m_UserItem[i].tIQ == TempItem.tIQ) { iNum = i; break; } 
			}
			else
			{
				iNum = i; break;
			}
		}
	}

	if(iNum != -1)	return m_UserItem[iNum].sCount;
	
	return 0;
}

BOOL USER::FindEventItem(int sid, int quality)
{
	if(sid < 0 || sid >= g_arItemTable.GetSize()) return FALSE;

	int i, j, iNum = -1;
	ItemList TempItem;

	ReSetItemSlot(&TempItem);

	TempItem.sSid		= g_arItemTable[sid]->m_sSid;
	TempItem.sBullNum	= g_arItemTable[sid]->m_sBullNum;
	TempItem.sDuration	= g_arItemTable[sid]->m_sDuration;

	for(i =0; i < MAGIC_NUM; i++) TempItem.tMagic[i] = 0;
	TempItem.tIQ = (BYTE)quality;
	
//	iNum = GetSameItem(TempItem, INVENTORY_SLOT);
	for(i = EQUIP_ITEM_NUM; i < EQUIP_ITEM_NUM + INVENTORY_NUM; i++)
	{
		if(m_UserItem[i].sSid == TempItem.sSid)
		{
			for(j = 0; j < MAGIC_NUM; j++)
			{
				if(m_UserItem[i].tMagic[j] != TempItem.tMagic[j]) break;
			}
				
			if(m_UserItem[i].tIQ == TempItem.tIQ) { iNum = i; break; } 
		}
	}

	if(iNum != -1)	return TRUE;
	
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
//
//	ÀÌº¥Æ®¿¡¼­ ¾ÆÀÌÅÛÀ» ÁÙ¶§ ¹«°Ô Ã¼Å©
//
BOOL USER::CheckItemWeight(int say, int iSid1, int iNum1, int iSid2, int iNum2, int iSid3, int iNum3, int iSid4, int iNum4, int iSid5, int iNum5)
{
	int iWeight = 0;
//	int iMax = g_arItemTable.GetSize();

	if( iSid1 >= 0 && iSid1 < g_arItemTable.GetSize() ) iWeight += g_arItemTable[iSid1]->m_byWeight * iNum1;
	if( iSid2 >= 0 && iSid2 < g_arItemTable.GetSize() ) iWeight += g_arItemTable[iSid2]->m_byWeight * iNum2;
	if( iSid3 >= 0 && iSid3 < g_arItemTable.GetSize() ) iWeight += g_arItemTable[iSid3]->m_byWeight * iNum3;
	if( iSid4 >= 0 && iSid4 < g_arItemTable.GetSize() ) iWeight += g_arItemTable[iSid4]->m_byWeight * iNum4;
	if( iSid5 >= 0 && iSid5 < g_arItemTable.GetSize() ) iWeight += g_arItemTable[iSid5]->m_byWeight * iNum5;

	if(m_iMaxWeight < m_iCurWeight + iWeight) 
	{
//		SendNpcSay(NULL, say);
		SendSystemMsg( IDS_USER_OVER_WEIGHT_RECEIVE_ITEM, SYSTEM_NORMAL, TO_ME);//"ÄãÃ»ÓÐ×ã¹»µÄ¸ºÖØ."
		return FALSE;
	}
	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//
//	ÀÌº¥Æ®¿¡¼­ ¾ÆÀÌÅÛÀ» ÁÙ¶§ ºñ¾îÀÖ´Â ½½·ÔÀÌ ÀÖ´ÂÁö Ã¼Å©
//
BOOL USER::CheckItemSlot(int say, LOGIC_ELSE* pLE)
{
	int sid = 0;
	int iNum = -1;
	ItemList TempItem;

	for(int i = 1; i < 10; i += 2)
	{
		sid = pLE->m_LogicElseInt[i];

		if( sid < 0 || sid >= g_arItemTable.GetSize() ) continue;

		ReSetItemSlot(&TempItem);

		TempItem.sSid		= g_arItemTable[sid]->m_sSid;
		TempItem.sBullNum	= g_arItemTable[sid]->m_sBullNum;
		TempItem.sDuration	= g_arItemTable[sid]->m_sDuration;

		for(int j =0; j < MAGIC_NUM; j++) TempItem.tMagic[j] = 0;
		TempItem.tIQ = NORMAL_ITEM;
	
		iNum = GetSameItem(TempItem, INVENTORY_SLOT);
		if(iNum == -1)							// °°Àº ¾ÆÀÌÅÛÀÌ ¾ø°í
		{
			iNum = GetEmptySlot(INVENTORY_SLOT);
			if(iNum == -1)						// ºó ½½·Ôµµ ¾øÀ¸¸é..
			{
				SendSystemMsg( IDS_USER_INVEN_FULL_RECEIVE_ITEM, SYSTEM_NORMAL, TO_ME);//"ÄãµÄ°ü¹üÒÑÂú."
				return FALSE;
			}
		}
	}

	return TRUE;	
}

//////////////////////////////////////////////////////////////////////
//
//	°°Àº °è¿­ÀÎÁö Ã¼Å©
//
BOOL USER::CheckClass(int iClass)
{
	if(m_byClass == iClass) return TRUE;

	return FALSE;
}

//////////////////////////////////////////////////////////////////////
//
//	ÀÌº¥Æ®¿¡¼­ ÇØ´ç ¾ÆÀÌÅÛÀ» »¯´Â´Ù.
//
void USER::RobItem(int sid, int num)
{
	if(sid < 0 || sid >= g_arItemTable.GetSize()) return;

	int i, j;
	int iSlot = -1;
	CBufferEx TempBuf;
//	BOOL bQuickChange = FALSE;
	BYTE result = SUCCESS;

	ItemList	TempItem;
	ReSetItemSlot(&TempItem);

	TempItem.sSid		= g_arItemTable[sid]->m_sSid;
	TempItem.sBullNum	= g_arItemTable[sid]->m_sBullNum;
	TempItem.sDuration	= g_arItemTable[sid]->m_sDuration;

	for(i =0; i < MAGIC_NUM; i++) TempItem.tMagic[i] = 0;
	TempItem.tIQ = NORMAL_ITEM;
	if(sid==755){
		TempItem.tIQ=2;
		TempItem.tMagic[0]=137;
	}
	
	for(i = EQUIP_ITEM_NUM; i < EQUIP_ITEM_NUM + INVENTORY_NUM; i++)
	{
		if(m_UserItem[i].sSid == TempItem.sSid)
		{
			if( m_UserItem[i].sSid != 796 )
			{
				for(j = 0; j < MAGIC_NUM; j++)
				{
					if(m_UserItem[i].tMagic[j] != TempItem.tMagic[j]) break;
				}
					
				if(m_UserItem[i].tIQ == TempItem.tIQ) { iSlot = i; break; } 
			}
			else
			{
				iSlot = i; break;
			}
		}
	}

	if(iSlot == -1) return;

	if(m_UserItem[iSlot].sSid < 0) return;
	if(m_UserItem[iSlot].sCount < num) return;

	if((m_UserItem[iSlot].sCount - num) <= 0)				// ´ÙÀ½ ³» ÀÎº¥¸¦ »© ÁØ´Ù.		
	{														// Äü¾ÆÀÌÅÛ º¯È­°¡ ÀÖÀ¸¸é		
//		if(g_arItemTable[iSlot]->m_byWear >= 6) bQuickChange = TRUE;

		ReSetItemSlot(&m_UserItem[iSlot]);
	}
	else m_UserItem[iSlot].sCount -= num;

	if(m_UserItem[iSlot].sCount < 0 ) m_UserItem[iSlot].sCount = 0;

	int iWeight = g_arItemTable[iSlot]->m_byWeight * num;

	m_iCurWeight -= iWeight;
	if(m_iCurWeight < 0) m_iCurWeight = 0;

	GetRecoverySpeed();			// ´Ù½Ã È¸º¹¼Óµµ¸¦ °è»ê

	TempBuf.Add(ITEM_GIVE_RESULT);
	TempBuf.Add(result);
	TempBuf.Add((BYTE)iSlot);
	TempBuf.Add(m_UserItem[iSlot].sLevel);
	TempBuf.Add(m_UserItem[iSlot].sSid);
	TempBuf.Add(m_UserItem[iSlot].sDuration);
	TempBuf.Add(m_UserItem[iSlot].sBullNum);
	TempBuf.Add(m_UserItem[iSlot].sCount);
	for(j = 0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[iSlot].tMagic[j]);

	TempBuf.Add(m_UserItem[iSlot].tIQ);

	Send(TempBuf, TempBuf.GetLength());
//	if(bQuickChange) SendQuickChange();				// Äü¾ÆÀÌÅÛ º¯µ¿ÀÌÀÖÀ¸¸é...
}


//	¹®È­ »óÇ°±Ç, ¹éÈ­Á¡ »óÇ°±ÇÀ» ÇÏ³ª¾¿ »©ÁÖ±â À§ÇØ ¸¸µç ÇÔ¼öÀÌ´Ù.
//	ÇöÀç ½ºÅ©¸³Æ®·Î ºÒ¸®´Â °ÍÀÌ ¾Æ´Ï¶ó RecvAddress¿¡¼­¸¸ ºÎ¸¥´Ù.
//	½ºÅ©¸³Æ®·Î ºÎ¸£¸ç, ¹ü¿ëÀ¸·Î »ç¿ëÇÏ°í ½ÍÀ»¶§¿¡´Â ¼öÁ¤ÀÌ ÇÊ¿äÇÒ²¬?
BOOL USER::EventRobItem(int item_index, int quality)
{
	int sid = item_index;
	int num = 1;
	if(sid < 0 || sid >= g_arItemTable.GetSize()) return FALSE;

	int i, j;
	int iSlot = -1;

	CBufferEx TempBuf;
	BYTE result = SUCCESS;

	ItemList	TempItem;
	ReSetItemSlot(&TempItem);

	TempItem.sSid		= g_arItemTable[sid]->m_sSid;
	TempItem.sBullNum	= g_arItemTable[sid]->m_sBullNum;
	TempItem.sDuration	= g_arItemTable[sid]->m_sDuration;

	for(i =0; i < MAGIC_NUM; i++) TempItem.tMagic[i] = 0;
	TempItem.tIQ = quality;
	
	for(i = EQUIP_ITEM_NUM; i < EQUIP_ITEM_NUM + INVENTORY_NUM; i++)
	{
		if(m_UserItem[i].sSid == TempItem.sSid)
		{
			for(j = 0; j < MAGIC_NUM; j++)
			{
				if(m_UserItem[i].tMagic[j] != TempItem.tMagic[j]) break;
			}
				
			if(m_UserItem[i].tIQ == TempItem.tIQ) { iSlot = i; break; } 
		}
	}

	if(iSlot == -1) return FALSE;

	if(m_UserItem[iSlot].sSid < 0) return FALSE;
	if(m_UserItem[iSlot].sCount < num) return FALSE;

	if((m_UserItem[iSlot].sCount - num) <= 0)				// ´ÙÀ½ ³» ÀÎº¥¸¦ »© ÁØ´Ù.		
	{														// Äü¾ÆÀÌÅÛ º¯È­°¡ ÀÖÀ¸¸é		
//		if(g_arItemTable[iSlot]->m_byWear >= 6) bQuickChange = TRUE;

		ReSetItemSlot(&m_UserItem[iSlot]);
	}
	else m_UserItem[iSlot].sCount -= num;

	int iWeight = num * g_arItemTable[iSlot]->m_byWeight;

	m_iCurWeight -= iWeight;
	if(m_iCurWeight < 0) m_iCurWeight = 0;

	GetRecoverySpeed();			// ´Ù½Ã È¸º¹¼Óµµ¸¦ °è»ê

	TempBuf.Add(ITEM_GIVE_RESULT);
	TempBuf.Add(result);
	TempBuf.Add((BYTE)iSlot);
	TempBuf.Add(m_UserItem[iSlot].sLevel);
	TempBuf.Add(m_UserItem[iSlot].sSid);
	TempBuf.Add(m_UserItem[iSlot].sDuration);
	TempBuf.Add(m_UserItem[iSlot].sBullNum);
	TempBuf.Add(m_UserItem[iSlot].sCount);
	for(j = 0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[iSlot].tMagic[j]);

	TempBuf.Add(m_UserItem[iSlot].tIQ);

	Send(TempBuf, TempBuf.GetLength());
//	if(bQuickChange) SendQuickChange();				// Äü¾ÆÀÌÅÛ º¯µ¿ÀÌÀÖÀ¸¸é...

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//
//	ÀÌº¥Æ®¿¡¼­ XP¸¦ ¿Ã·ÁÁØ´Ù.
//
void USER::AddXPUp(int iNum)
{
	if(iNum <= 0) return;

	CheckMaxValue(m_dwXP, iNum);		// ¸÷ÀÌ Á×À»¶§¸¸ 1 Áõ°¡!	
	SendXP();
}

////////////////////////////////////////////////////////////////////////////
//	NPC Event ÀÇ EXEC ¸í·ÉÀ» ¼öÇàÇÑ´Ù.
//
BOOL USER::RunNpcEvent(CNpc *pNpc, EXEC *pExec)
{
	int nDist = 0;
	int result = 0;
	short sID = 0;
	CBufferEx test;
	test.Add(sID);
	
	if(pNpc)
	{
		if(pNpc->m_sCurZ != m_curz) return TRUE;
	}

	switch( pExec->m_Exec )
	{
	case EXEC_RETURN:
		return FALSE;

	case EXEC_BBS_OPEN:
		BBSOpen( pExec->m_ExecInt[0] );
		break;

	case EXEC_ZONE_MOVE:			
		if(pNpc) nDist = abs(m_curx - pNpc->m_sCurX) + abs(m_cury - pNpc->m_sCurY);
		if(nDist > SCREEN_X + SCREEN_Y) return TRUE;
		ZoneMoveReq( pExec->m_ExecInt[0], pExec->m_ExecInt[1], pExec->m_ExecInt[2] );
		break;

	case EXEC_RR_ENTER:
		if( !g_RR.Enter( this ) ) return TRUE;
		ZoneMoveReq( pExec->m_ExecInt[0], pExec->m_ExecInt[1], pExec->m_ExecInt[2] );
		break;

	case EXEC_FT_MOVE:
		FortressMove(pNpc,  pExec->m_ExecInt[0], pExec->m_ExecInt[1], pExec->m_ExecInt[2], pExec->m_ExecInt[3], pExec->m_ExecInt[4] );
		break;

	case EXEC_GUARD_BOW:			// °æºñº´ ÀÎ»çÇÏ±â
		GuardBow(pNpc);
		break;

	case EXEC_MOVE:					// °°ÀºÁ¸¾ÈÀÇ Æ¯º° Àå¼Ò·Î ÀÌµ¿½ÃÅ²´Ù.(³ªÁß¿¡ ÇÊ¿äÇÑÁö °í¹Î!)
		EventMove( pExec->m_ExecInt[0], pExec->m_ExecInt[1], pExec->m_ExecInt[2] );
		break;

	case EXEC_ROB_DN:
		RobDN( pExec->m_ExecInt[0] );
		break;

	case EXEC_SELL_OPEN:// NPC³öÊÛ
		SellItemOpenReq( pExec->m_ExecInt[0] );
		break;

	case EXEC_BANK_OPEN:
	    BankOpenReq();
		break;

	case EXEC_ON_WEB:
		if(!OpenOnWeb()) return FALSE;
		break;

	case EXEC_CHOUJIANG://NPC³é½±
	    //NpcSaying(pNpc);
		break;

	case EXEC_NPC_SHUXING://NPC¿ì½Ý¼ü
		NpcXING();
		break;

	case EXEC_NPC_DA://¶Ä²©´ó
		NpcDuBoDA(pExec->m_ExecInt[0],pExec->m_ExecInt[1]);
		break;

	case EXEC_NPC_XIAO://¶Ä²©Ð¡
		NpcDuBoXIAO(pExec->m_ExecInt[0],pExec->m_ExecInt[1]);
		break;

	case EXEC_NPC_ZHUANZHI://×ª»»Ö°Òµ
		NpcZHUANZHI(pExec->m_ExecInt[0]);
		break;

	case EXEC_NPC_SHANGDIAN://×ª»»ÉÌµê
		NpcShangdian(pExec->m_ExecInt[0]);
		break;

	case EXEC_MARKHUODONG://Mark»î¶¯
		MarkJayHuoDong(pExec->m_ExecInt[0]);
		break;

	case EXEC_MARK_CZ_SAY://³äÖµ¶Ô»°¿ò
		MarkCzSay();
		break;

	case EXEC_NEWGONGNENG://ÐÂ½Å±¾µ÷ÓÃ MarkJay
		SetNewGongNeng(pExec->m_ExecInt[0]);
		break;

	case EXEC_DUIHUANITEM://¶Ò»»ÓÐÊôÐÔÎïÆ·
		ChangeAttributeItem(pExec->m_ExecInt[0],pExec->m_ExecInt[1],pExec->m_ExecInt[2],pExec->m_ExecInt[3],
		                    pExec->m_ExecInt[4],pExec->m_ExecInt[5],pExec->m_ExecInt[6],pExec->m_ExecInt[7],
							pExec->m_ExecInt[8],pExec->m_ExecInt[9],pExec->m_ExecInt[10],pExec->m_ExecInt[11],
							pExec->m_ExecInt[12],pExec->m_ExecInt[13],pExec->m_ExecInt[14],pExec->m_ExecInt[15],
							pExec->m_ExecInt[16],pExec->m_ExecInt[17],pExec->m_ExecInt[18],pExec->m_ExecInt[19]
							,pExec->m_ExecInt[20],pExec->m_ExecInt[21],pExec->m_ExecInt[22],pExec->m_ExecInt[23]);
		break;

	case EXEC_ADD4PAI:
		//Add4Pai(pExec->m_ExecInt[0]);//3ÅÅºÏ³É4ÅÅMarkJay
		break;

	case EXEC_ADD4PWQ:
		//Add4wq(pExec->m_ExecInt[0]);//3ÅÅºÏ³É4ÅÅMarkJay
		break;

	case EXEC_ADD4SS:
		//Add4ss(pExec->m_ExecInt[0]);//3ÅÅºÏ³É4ÅÅMarkJay
		break;

	case EXEC_ACCOUNT_BANK_OPEN:
	    AccountBankOpenReq( pExec->m_ExecInt[0] );
		break;

	case EXEC_GUILD_WAREHOUSE_OPEN:
		GuildWharehouseOpenReq();
		break;

	case EXEC_REPAIR_OPEN:			// ¾ÆÀÌÅÛ ¼ö¸®ÇÏ±â
		RepairItemOpenReq( pExec->m_ExecInt[0] );
		break;

	case EXEC_STORE_OPEN:			// ¾ÆÀÌÅÛ »ç±â
		SendStoreOpen( pExec->m_ExecInt[0], pExec->m_ExecInt[1] );
		break;

	case EXEC_FIELD_STORE_OPEN:		// ÇÊµå¿¡¼­ ±æµå¿Í »óÁ¡ÀÌ ¸¸³µÀ»¶§...
		SendFieldStoreOpen( pExec->m_ExecInt[0], pExec->m_ExecInt[1], pExec->m_ExecInt[2] );
		break;

	case EXEC_GUILD_SAY:
		SendFieldGuildSay( pNpc, pExec->m_ExecInt[0], pExec->m_ExecInt[1] );
		break;

	case EXEC_GUILD_TAX:
		SendStoreTax( pExec->m_ExecInt[0] );
		break;

	case EXEC_GUILD_SCHEME:
		SendGuildWarScheme( pExec->m_ExecInt[0] );
		break;

	case EXEC_GUILD_APPLY:
		GuildWarApply( pExec->m_ExecInt[0] );
		break;

	case EXEC_PSI_STORE_OPEN:
		SendPsiStoreOpen( pExec->m_ExecInt[0] );
		break;

	case EXEC_SAY:
		SendNpcSay( pNpc, pExec->m_ExecInt[0] );
		break;

	case EXEC_ALL_CURE:				// ÖÎÁÆ
		SendAllCure( pExec->m_ExecInt[0],  pExec->m_ExecInt[1] );
		break;

	case EXEC_CARE:					// ¸ðµç »óÅÂÀÌ»ó¸¦ Ä¡·áÇÑ´Ù.
		SendSystemMsg(IDS_USER_UNDER_CONSTRUCTION, SYSTEM_NORMAL, TO_ME);//"ÎÞ·¨ÊµÊ©."
		break;

	case EXEC_GUILD_OPEN:
		GuildOpen();
		break;

	case EXEC_EBODY_OPEN:
//		SendEBodyOpen( pExec->m_ExecInt[0] );
		break;

	case EXEC_EBODY_DEFINE:
		EBodyIdentifyOpen( pExec->m_ExecInt[0] );
		break;

	case EXEC_UPGRADE_OPEN:
		UpgradeItemOpen(pExec->m_ExecInt[1]);
		break;

	case EXEC_ADD_EVENT:
		AddMyEventNum( pExec->m_ExecInt[0] );
		break;
	case EXEC_ADD_SKILL://120¼¼ÄÜ×ª»»Ê±..ÓÃµÄÊý¾Ý
		AddSkill(pExec->m_ExecInt[0]);
		break;

	case EXEC_DEL_EVENT:
		DelMyEventNum( pExec->m_ExecInt[0] );
		break;

	case EXEC_ROB_ITEM:
		RobItem( pExec->m_ExecInt[0], pExec->m_ExecInt[1] );
		break;

	case EXEC_MONEYSHOP://²é¿´Ôª±¦ÉÌµê´øÊôÐÔ
		ViewYuanBaoShop(pExec->m_ExecInt[0]);
		break;

	case EXEC_EMO://¶ñÄ§¹ã³¡
		EmoGuangChang();
		break;

    case EXEC_ZHANCHANG://µç1Õ½³¡
		//ZhanChang();
		break;
	case EXEC_EVENT_ROB_ITEM:
//		EventRobItem( pExec->m_ExecInt[0], pExec->m_ExecInt[1] );
		break;

	case EXEC_XP_UP:
		AddXPUp( pExec->m_ExecInt[0] );
		break;

	case EXEC_CHECK_GUILD_USER:
		SendGuildUserInfo( pExec->m_ExecInt[0] );
		break;

	case EXEC_CHECK_GUILD_RANK:
		SendGuildHouseRank( pExec->m_ExecInt[0], pExec->m_ExecInt[1] );
		break;

	case EXEC_FT_REPAIR:
		SendRepairItem( pExec->m_ExecInt[0] );
		break;

	case EXEC_FT_PEOPLE:
		SendPeopleSay( pExec->m_ExecInt[0] );
		break;

	case EXEC_GIVE_ITEM:
		GiveItem( pExec->m_ExecInt[0], pExec->m_ExecInt[1] );
		break;

	case EXEC_GIVE_MAGIC_ITEM:
		GiveMagicItem( pExec->m_ExecInt[0], pExec->m_ExecInt[1] );
		break;

	case EXEC_GIVE_EVENT_ITEM:
		GiveEventItem( pExec->m_ExecInt[0], pExec->m_ExecInt[1], pExec->m_ExecInt[2], pExec->m_ExecInt[3] );
		break;

	case EXEC_ADDRESS_WINDOW:
		AddressWindowOpen(pExec->m_ExecInt[0], pExec->m_ExecInt[1]);
		break;

		// CDKEY¶Ô»°¿ò MarkJay
	case EXEC_INPUT_WINDOW:				
		EventItemSerialWindowOpen();
		break;

	case EXEC_INPUTGAME_WINDOW:
		EventItemSerialForGameRoomWindowOpen();
		break;

	case EXEC_ADD_GAMETIME_ITEM:
		GiveEventGameTime(pExec->m_ExecInt[0], pExec->m_ExecInt[1]);
		break;

	case EXEC_DRESSING:
		DressingWindowOpen();
		break;

	case EXEC_GIVE_DN:
		GiveDN( pExec->m_ExecInt[0] );
		break;

	case EXEC_GIVE_YUANBAO://NPC¸øÔª±¦
		GiveShopPingYB( pExec->m_ExecInt[0] );
		break;

	case EXEC_ROB_YUANBAO://NPCÊÕÔª±¦
		RobShopPingYB( pExec->m_ExecInt[0] );
		break;

	case EXEC_SEND_MSG:
		SendEventMsg( pExec->m_ExecChar );
		break;

	case EXEC_SEND_TOU:
		SendNpcToUserMsg( pNpc, pExec->m_ExecInt[0], pExec->m_ExecChar );
		break;

	case EXEC_CHANGE_UPGRADE_ACC:
		ChangeUpgradeAcc();
		break;

	case EXEC_APPLY_GUILD_RUN:
		ApplyGuildRun();
		break;

	case EXEC_MON_SAY:
		SendMonsterSay( pNpc, pExec->m_ExecInt[0], pExec->m_ExecChar );
		break;

	case EXEC_SUMMON:
		if(!ExcuteSingleQuestEvent( pExec->m_ExecInt[0], pExec->m_ExecInt[1], pExec->m_ExecInt[2], pExec->m_ExecInt[3], 
								pExec->m_ExecInt[4], pExec->m_ExecInt[5], pExec->m_ExecInt[6])) return FALSE;
		break;

	case EXEC_RUN_EVENT:
		{
			EVENT* pEvent = GetEventInCurrentZone();	if( !pEvent ) break;

			if( !pEvent->m_arEvent[pExec->m_ExecInt[0]] ) break;

			if( !CheckEventLogic( pEvent->m_arEvent[pExec->m_ExecInt[0]] ) ) break;

			for( int i = 0; i < pEvent->m_arEvent[pExec->m_ExecInt[0]]->m_arExec.GetSize(); i++)
			{
				if( !RunNpcEvent( pNpc, pEvent->m_arEvent[pExec->m_ExecInt[0]]->m_arExec[i] ) )
				{
					return FALSE;
				}
			}
		}
		break;
	case EXEC_HELPERCURE:
		SendHelperCure( );
		break;
	case EXEC_GIVE_PSI:
		if(!GivePSI( pExec->m_ExecInt[0], pExec->m_ExecInt[1])) return FALSE;
		break;
	case EXEC_100_1://100¼¶·À¾ß×ª»»
		break;
	case EXEC_100_2://½â³ýÊôÐÔÏÞÖÆ
		break;
	case EXEC_100_3:// Ìí¼Ó¼°±ä¸üÊôÐÔ
		break;
	case EXEC_100_4://ÊôÐÔÉý¼¶
		break;
	case EXEC_100_5://·À¾ß²ÄÁÏ»¯
		break;
	case EXEC_100_6://½â³ý²ÄÁÏ»¯
		break;
	case RUN_SKILL:
		RunSkill120( pExec->m_ExecInt[0]);
		break;
	case EXEC_MARK_ROBITEM:// »ØÊÕ×°±¸ Ð¡ºÚÐÞÁ¶
		MarkRobItem(pExec->m_ExecInt[0]);
		break;
	case EXEC_GIVE_MAGIC://¼ì²éÊÇ·ñ¿ÉÒÔÑ§Ï°¡¡Ä§·¨´ó£°Ä§·¨ ¿ÉÒÔµÄ»°¡£¾Í¸øÒ»¸ö
		if(!GetFashiPsi(pExec->m_ExecInt[0])) return FALSE;
		break;
	case EXEC_GIVE_ITEM_ALL:
		GiveItemAll( pExec->m_ExecInt[0], pExec->m_ExecInt[1],pExec->m_ExecInt[2],pExec->m_ExecInt[3],
		pExec->m_ExecInt[4],pExec->m_ExecInt[5],pExec->m_ExecInt[6],pExec->m_ExecInt[7],pExec->m_ExecInt[8],
		pExec->m_ExecInt[9],pExec->m_ExecInt[10],pExec->m_ExecInt[11],pExec->m_ExecInt[12]);
		break;
	case EXEC_TRANSFORM:
		TransformWindow( );
		break;
	case EXEC_MARK_CZ:
		//MarkLingQuCZ();
		break;
	default:
		break;
	}

	return TRUE;
}
//code ÊýÁ¿¡¡¸ÄÊý¡¡ÑÕÉ«¡¡ÊôÐÔ£±¡¡ÊôÐÔ£²¡¡ÊôÐÔ£³¡¡ÊôÐÔ£´
void USER::GiveAllItem(int sSid,int sCount,int iUp,int sIQ,int a1,int a2,int a3,int a4)
{
	if(sSid < 0 || sSid >= g_arItemTable.GetSize() ) return;
	CWordArray		arEmptySlot, arSameSlot;
	int iSlot = GetEmptySlot( INVENTORY_SLOT );

	if( iSlot < 0 ) return;

	ReSetItemSlot( &m_UserItem[iSlot]);

	m_UserItem[iSlot].sLevel = g_arItemTable[a1]->m_byRLevel;
	m_UserItem[iSlot].sSid = sSid;
	m_UserItem[iSlot].sCount = sCount;
	m_UserItem[iSlot].sDuration = g_arItemTable[a1]->m_sDuration;
	m_UserItem[iSlot].sBullNum = 0;
	m_UserItem[iSlot].tMagic[0] = a1;
	m_UserItem[iSlot].tMagic[1] = a2;
	m_UserItem[iSlot].tMagic[2] = a3;
	m_UserItem[iSlot].tMagic[3] = a4;
	m_UserItem[iSlot].tMagic[4] = 0;
	m_UserItem[iSlot].tMagic[5] = iUp;
	m_UserItem[iSlot].tIQ = sIQ;
	m_UserItem[iSlot].iItemSerial = 0;

//	MakeItemLog( &m_UserItem[iSlot], ITEMLOG_EVENT_GIVE );

	arEmptySlot.Add(iSlot); 
	UpdateInvenSlot(&arEmptySlot, &arSameSlot);

}

void USER::GiveAllItem1(int sSid,int sCount,int iUp,int sIQ,int a1,int a2,int a3,int a4,int a5)
{
	if(sSid < 0 || sSid >= g_arItemTable.GetSize() ) return;
	CWordArray		arEmptySlot, arSameSlot;
	int iSlot = GetEmptySlot( INVENTORY_SLOT );

	if( iSlot < 0 ) return;

	ReSetItemSlot( &m_UserItem[iSlot]);

	m_UserItem[iSlot].sLevel = g_arItemTable[a1]->m_byRLevel;
	m_UserItem[iSlot].sSid = sSid;
	m_UserItem[iSlot].sCount = sCount;
	m_UserItem[iSlot].sDuration = g_arItemTable[a1]->m_sDuration;
	m_UserItem[iSlot].sBullNum = 0;
	m_UserItem[iSlot].tMagic[0] = a1;
	m_UserItem[iSlot].tMagic[1] = a2;
	m_UserItem[iSlot].tMagic[2] = a3;
	m_UserItem[iSlot].tMagic[3] = a4;
	m_UserItem[iSlot].tMagic[4] = a5;
	m_UserItem[iSlot].tMagic[5] = iUp;
	m_UserItem[iSlot].tIQ = sIQ;
	m_UserItem[iSlot].iItemSerial = 0;

//	MakeItemLog( &m_UserItem[iSlot], ITEMLOG_EVENT_GIVE );

	arEmptySlot.Add(iSlot); 
	UpdateInvenSlot(&arEmptySlot, &arSameSlot);
}

BOOL USER::GetFashiPsi(BYTE tType)//·¨Ê¦³¬¼¶·À»¤ÕÖ
{
	CBufferEx TempBuf;
	switch( tType )
	{
		case 30:
		{
			if(IsHavePsionic(6) == false)
			{
				SendNpcSay( NULL, 665);
				return FALSE;
			}
			if(m_dwDN < 1000000)
			{
				SendSystemMsg("Ñ§Ï°·¨Ê¦³¬¼¶·À»¤ÕÖÐèÒª100Íò!", SYSTEM_ERROR, TO_ME);
				return FALSE;
			}
			if(m_dwXP < 8000)
			{
				SendNpcSay( NULL, 666);
				return FALSE;
			}
			for(int j = 0; j < m_nHavePsiNum; j++)
			{
				if(m_UserPsi[j].sSid == 6) m_UserPsi[j].sSid = 30;										
			}
			
			AddMyEventNum(202);
			SendNpcSay( NULL, 664);

			TempBuf.Add(BUY_PSY_RESULT);
			TempBuf.Add((byte)1);
			short sPsiCount = 1;
			TempBuf.Add(sPsiCount);
			for(int i = 0; i < sPsiCount; i++)
			{
				short sSid = (short)30;
				TempBuf.Add(sSid);
			}
			m_dwXP -= 8000;
			m_dwDN -= 1000000;
			SendXP();
			UpdateUserItemDN();
			SendMoneyChanged();
			Send(TempBuf, TempBuf.GetLength());
			return TRUE;
		}
		break;
	}
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////
//	°ø¼ºÀü½Ã ÀÌµ¿À» Á¦ÇÑÇÏ°í ¹®ÀÚÃ¼°¡ ¾ø¾îÁö´Â È¿°ú¶§¹®...
//
void USER::FortressMove(CNpc *pNpc, int sid, int x1, int y1, int x2, int y2) // ¹Û-->³»ºÎ, ³»ºÎ-->¹Û 
{
//	BOOL bSuc = TRUE;
	int nDist = 0;

	if(!pNpc) return;

	nDist = abs(m_curx - pNpc->m_sCurX) + abs(m_cury - pNpc->m_sCurY);
	if(nDist > SCREEN_X + SCREEN_Y) return;

	CGuildFortress* pFort = NULL;
	pFort = GetFortress(sid);

	if(!pFort || pFort->m_iGuildSid <= 0) return;

	int type = CheckInvalidMapType();

	if(pNpc->m_tGuildWar == GUILD_WARRING && pNpc->m_NpcState == NPC_DEAD) return;

	if(pFort->m_iGuildSid != m_dwGuild)
	{
		SendSystemMsg( IDS_USER_NOT_THIS_GUILD_MEMBER, SYSTEM_ERROR, TO_ME);//"Äã²»ÊÇÓµÓÐÕâ¸öÒªÈûµÄ¾üÍÅ³ÉÔ±."
		return;
	}
//	if(!pFort->m_bHaveGuild && pFort->m_lUsed == 0) bSuc = TRUE;

//	if(bSuc)
//	{
		switch(type)
		{
		case 8: case 10: case 15:
			EventMove( pNpc->m_sCurZ, x2, y2 );
			break;
		case 9: case 11: case 16:						
			EventMove( pNpc->m_sCurZ, x1, y1 );
			break;
		}		
		return;
//	}
//	else 
//	{
//		SendSystemMsg( IDS_USER_NOT_THOS_GUILD_MEMBER, SYSTEM_ERROR, TO_ME);
//	}
}

/////////////////////////////////////////////////////////////////////////
//	ÇöÀç ¼­ÀÖ´Â Á¸¿¡¼­ ¹ß»ýÇÏ´Â ÀÌº¥Æ®¸¦ °¡Á®¿Â´Ù.
//
EVENT* USER::GetEventInCurrentZone()
{
	int i;//, event_index = -1;
	EVENT *pEvent;

	for( i = 0; i < g_event.GetSize(); i++)
	{
		pEvent = g_event[i];
		if( !pEvent ) continue;
		if( pEvent->m_Zone == m_curz )
		{
//			event_index = i;
//			break;
			return g_event[i];
		}
	}

//	if(event_index == -1) return NULL;

//	return g_event[event_index];
	return NULL;
}

/////////////////////////////////////////////////////////////////////////
//	ÇöÀç ¼öÇàÇÑ ÀÌº¥Æ®¸¦ ±â·ÏÇÑ´Ù.
//
void USER::AddMyEventNum(int seventnum)
{
	int i;
	int* senum;

	if(MAX_EVENT_NUM <= m_arEventNum.GetSize()) return;

	for( i = 0; i < m_arEventNum.GetSize(); i++)
	{
		if( !m_arEventNum[i] ) continue;

		if( *(m_arEventNum[i]) == seventnum )
		{
			return;
		}
	}

	senum = new int;

	*senum = seventnum;

	m_arEventNum.Add( senum );
}

/////////////////////////////////////////////////////////////////////////
//	¼öÇàÇÑ ÀÌº¥Æ®¸¦ »èÁ¦ÇÑ´Ù.
//
void USER::DelMyEventNum(int seventnum)
{
	int i;

	for( i = 0; i < m_arEventNum.GetSize(); i++)
	{
		if( !m_arEventNum[i] ) continue;

		if( *(m_arEventNum[i]) == seventnum )
		{
			delete m_arEventNum[i];
			m_arEventNum[i] = NULL;
			m_arEventNum.RemoveAt( i, 1 );
			DelMyEventNum( seventnum );
			return;
		}
	}
}

/////////////////////////////////////////////////////////////////////////
//	°æºñº´ ÀÎ»ç ÀÌº¥Æ®¸¦ ½ÃÀÛÇÑ´Ù.
//
void USER::GuardBow(CNpc *pNpc)
{
	if(!pNpc) return;
	if(m_sCityRank < 0) return;

	CString strMsg =_T("");
	int nMax = 0, index = 0;

	CBufferEx TempBuf, TempBuf2;

	CPoint startPt, pt;

	startPt.x = pNpc->m_sCurX;
	startPt.y = pNpc->m_sCurY;
	pt.x = m_curx;
	pt.y = m_cury;

	index = GetDirection(startPt, pt);		// ÇöÀç ¹æÇâÀ» Ç¥½ÃÇÑ´Ù.;

	TempBuf2.Add(GUARD_BOW);
	TempBuf2.Add((int)pNpc->m_sNid + NPC_BAND);
	TempBuf2.Add((BYTE)index);

	SendScreen(TempBuf2, TempBuf2.GetLength());

	TempBuf.Add(CHAT_RESULT);
	TempBuf.Add(SUCCESS);
	TempBuf.Add(NORMAL_CHAT);
	TempBuf.Add(pNpc->m_sNid + NPC_BAND);
	TempBuf.AddString(pNpc->m_strName);

	nMax = g_arNpcSayEventTable.GetSize();

	index = myrand(0, nMax - 1);

	strMsg = g_arNpcSayEventTable[index]->Say;
	strMsg.Replace("%s", m_strUserID);

	TempBuf.AddString((LPTSTR)(LPCTSTR)strMsg);

	SendScreen(TempBuf, TempBuf.GetLength());

}
//³é½±NPC  SAY
//void USER::NpcSaying(CNpc *pNpc)
//{
//	if(!pNpc) return;
//	CString strMsg =_T("");
//	CBufferEx TempBuf;
//	TempBuf.Add(CHAT_RESULT);
//	TempBuf.Add(SUCCESS);
//	TempBuf.Add(NORMAL_CHAT);
//	TempBuf.Add(pNpc->m_sNid + NPC_BAND);
//	TempBuf.AddString(pNpc->m_strName);
//	int error = RandChouJiang();
//	if(error == 1) strMsg.Format("%s ³é½±»î¶¯ÉÐÎ´¿ªÊ¼!", m_strUserID);
//	else if(error == 2)strMsg.Format("±¾´Î³é½±»î¶¯½±Æ·³ØËùÓÐ½±Æ·ÒÑÖÐÍê,Çë¹Ø×¢ÏÂÆÚ»î¶¯!");
//	else if(error == 3)strMsg.Format("¹§Ï²ÐÒÔËÕß¡º%s¡»ÔÚ³é½±»î¶¯ÖÐÖÐ½±ÁË!", m_strUserID);
//	else if(error == 4)strMsg.Format("¡º%s¡»µÄ±¾´Î³é½±Î´»ñµÃÈÎºÎ½±Æ·!", m_strUserID);
//	else return;
//	TempBuf.AddString((LPTSTR)(LPCTSTR)strMsg);
//	SendScreen(TempBuf, TempBuf.GetLength());
//}
void USER::NpcXING()
{
	CString sayStr;
	CBufferEx TempBuf, TempSayBuf , TempBuf3;
	int temptime = (3600 * 1000);
	int hour = 0,min = 0,day = 0;
	sayStr.Format("%s",m_strUserID);//ÓÎÏ·Ãû×Ö
	TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	if(m_dwZaiXianTime > 0)
	{
		sayStr.Format("%d·Ö",m_dwZaiXianTime);//ÏÔÊ¾ÔÚÏßÊ±¼ä
		TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}
	else 
	{
		sayStr.Format("µÈÓÚ0");
		TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}
	if(m_dwHiExpTime > 0)
	{
		hour = m_dwHiExpTime/temptime;	min = m_dwHiExpTime%temptime/(60000);
		sayStr.Format("%dÐ¡Ê±%d·Ö",hour,min);//ÐË·ÜÊ±¼ä
		TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}
	else
	{
		sayStr.Format("µÈÓÚ0");
		TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}
	if(m_dwMagicFindTime > 0)
	{
		hour = m_dwMagicFindTime/temptime;   min = m_dwMagicFindTime%temptime/(60000);
		sayStr.Format("%dÐ¡Ê±%d·Ö",hour,min);//ÐÒÔË
		TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}
	else
	{
		sayStr.Format("µÈÓÚ0");
		TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}
	if(m_dwFUExpTime > 0)
	{
		hour = m_dwFUExpTime/temptime;	min = m_dwFUExpTime%temptime/(60000);
		sayStr.Format("%dÐ¡Ê±%d·Ö",hour,min);//ÐÒ¸£Ê±¹â
		TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}
	else 
	{
		sayStr.Format("µÈÓÚ0");
		TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}
	if(m_dwBFindTime > 0)
	{
		hour = m_dwBFindTime/temptime;   min = m_dwBFindTime%temptime/(60000);
		sayStr.Format("%dÐ¡Ê±%d·Ö",hour,min);//ÏÔÊ¾±äÉíÊ±¼ä
		TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}
	else 
	{
		sayStr.Format("µÈÓÚ0");
		TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}

	if(m_dwHuanJing > 0)
	{
		hour = m_dwHuanJing/temptime;	min = m_dwHuanJing%temptime/(60000);
		sayStr.Format("%dÐ¡Ê±%d·Ö",hour,min);//»ÃÏë¾§Ê¯
		TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}
	else 
	{
		sayStr.Format("µÈÓÚ0");
		TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}
	if(m_dwHuanLing > 0)
	{
		hour = m_dwHuanLing/temptime;	min = m_dwHuanLing%temptime/(60000);
		sayStr.Format("%dÐ¡Ê±%d·Ö",hour,min);//»ÃÏëÁéÊ¯
		TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}
	else 
	{
		sayStr.Format("µÈÓÚ0");
		TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}
	if(m_dwVIPTime > 0)
	{
		hour = m_dwVIPTime/temptime;	min = m_dwVIPTime%temptime/(60000);
		sayStr.Format("VIPÓÃ»§Ê£Óà%dÐ¡Ê±%d·Ö",hour,min);//vipÊ±¹â
		TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}
	else 
	{
		sayStr.Format("µÈÓÚ0");
		TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}
	if(m_dwCKMiMa > 0)
	{
		hour = m_dwCKMiMa/temptime;	min = m_dwCKMiMa%temptime/(60000);
		sayStr.Format("µã¿¨ÓÃ»§Ê£Óà%dÐ¡Ê±%d·Ö",hour,min);
		TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}
	else 
	{
		sayStr.Format("µÈÓÚ0");
		TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}
	if(m_dwRenWu > 0 && m_sLevel >= 110)
	{
		int GuaiWuNum = 0;
		if(m_sLevel >= 110 && m_sLevel < 120)
		{
			GuaiWuNum = f_Markini[0]->GuaiWuNum_150_155 - m_dwRenWu;
		}
		else if(m_sLevel >= 120 && m_sLevel <= 135)
		{
			GuaiWuNum = f_Markini[0]->GuaiWuNum_155_160 - m_dwRenWu;
		}
		sayStr.Format("%d",GuaiWuNum);
		TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}
	else 
	{
		sayStr.Format("Î´¼ÆËã");
		TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
	}
	TempBuf.Add(CLIENT_EVENT_SAY);
	TempBuf.Add((BYTE)SUCCESS);
	TempBuf.Add((short)5009);
	TempBuf.Add((BYTE)0x09);
	TempBuf.AddData(TempSayBuf, TempSayBuf.GetLength());
	Send(TempBuf, TempBuf.GetLength());
}
void USER::NpcDuBoDA(int money,int type)
{
	CString sayStr;
	CBufferEx TempBuf, TempSayBuf , TempBuf3;
	int result = myrand(0,10000);
	int BiaozhiNum = 0;
	BiaozhiNum = FindItem(SPECIAL_BIAOZHI);
	if(type == 1)				//  ¶Ä²©ÓÎÏ·±Ò
	{
		if(m_dwDN >= (DWORD)money)
		{
			if(result <= 2000)
			{
				sayStr.Format("%d ÓÎÏ·±Ò",money);
				TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				//sayStr.Format("%d",result);
				//TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				sayStr.Format("´ó");
				TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				TempBuf.Add(CLIENT_EVENT_SAY);
				TempBuf.Add((BYTE)SUCCESS);
				TempBuf.Add((short)5002);
				TempBuf.Add((BYTE)0x03);
				TempBuf.AddData(TempSayBuf, TempSayBuf.GetLength());
				Send(TempBuf, TempBuf.GetLength());
				m_dwDN += money;
				SendMoneyChanged();
				UpdateUserItemDN();
				for(int i = 0; i < 5; i++)
				{
					TempBuf3.Add(USE_POTION);		
					TempBuf3.Add(m_uid + USER_BAND);
					TempBuf3.Add((BYTE)5);	//ÖÐ½±Ê±´øÑÌ»¨Ð§¹û
					Send( TempBuf3, TempBuf3.GetLength() );
				}
			}
			else
			{
				sayStr.Format("%d ÓÎÏ·±Ò",money);
				TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				//sayStr.Format("%d",result);
				//TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				sayStr.Format("Ð¡");
				TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				TempBuf.Add(CLIENT_EVENT_SAY);
				TempBuf.Add((BYTE)SUCCESS);
				TempBuf.Add((short)5003);
				TempBuf.Add((BYTE)0x03);
				TempBuf.AddData(TempSayBuf, TempSayBuf.GetLength());
				Send(TempBuf, TempBuf.GetLength());
				m_dwDN -= money;
				SendMoneyChanged();
				UpdateUserItemDN();
			}
		}
		else
		{
			sayStr.Format("%d",money);
			TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
			TempBuf.Add(CLIENT_EVENT_SAY);
			TempBuf.Add((BYTE)SUCCESS);
			TempBuf.Add((short)5001);
			TempBuf.Add((BYTE)0x01);
			TempBuf.AddData(TempSayBuf, TempSayBuf.GetLength());
			Send(TempBuf, TempBuf.GetLength());
		}
	}
	else				//  ¶Ä²©µã¾í
	{
		if((DWORD)BiaozhiNum >= (DWORD)money)
		{
			if(result <= 2000)
			{
				sayStr.Format("%d ¸ö±êÖ¾",money);
				TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				//sayStr.Format("%d",result);
				//TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				sayStr.Format("´ó");
				TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				TempBuf.Add(CLIENT_EVENT_SAY);
				TempBuf.Add((BYTE)SUCCESS);
				TempBuf.Add((short)5002);
				TempBuf.Add((BYTE)0x03);
				TempBuf.AddData(TempSayBuf, TempSayBuf.GetLength());
				Send(TempBuf, TempBuf.GetLength());
				GiveItem(SPECIAL_BIAOZHI,money);
				SendUserStatusSkill();//¸üÐÂµã¾íÊýÁ¿
				for(int i = 0; i < 5; i++)
				{
					TempBuf3.Add(USE_POTION);		
					TempBuf3.Add(m_uid + USER_BAND);
					TempBuf3.Add((BYTE)5);	//ÖÐ½±Ê±´øÑÌ»¨Ð§¹û
					Send( TempBuf3, TempBuf3.GetLength() );
				}
			}
			else
			{
				sayStr.Format("%d ¸ö±êÖ¾",money);
				TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				//sayStr.Format("%d",result);
				//TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				sayStr.Format("Ð¡");
				TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				TempBuf.Add(CLIENT_EVENT_SAY);
				TempBuf.Add((BYTE)SUCCESS);
				TempBuf.Add((short)5003);
				TempBuf.Add((BYTE)0x03);
				TempBuf.AddData(TempSayBuf, TempSayBuf.GetLength());
				Send(TempBuf, TempBuf.GetLength());
				RobItem(SPECIAL_BIAOZHI,money);
				SendUserStatusSkill();//¸üÐÂµã¾íÊýÁ¿
			}
		}
		else
		{
			sayStr.Format("%d",money);
			TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
			TempBuf.Add(CLIENT_EVENT_SAY);
			TempBuf.Add((BYTE)SUCCESS);
			TempBuf.Add((short)5001);
			TempBuf.Add((BYTE)0x01);
			TempBuf.AddData(TempSayBuf, TempSayBuf.GetLength());
			Send(TempBuf, TempBuf.GetLength());
		}
	}
}
void USER::NpcDuBoXIAO(int money,int type)
{
	CString sayStr;
	CBufferEx TempBuf, TempSayBuf ,TempBuf3;
	int result = myrand(0,10000);
	int BiaozhiNum = 0;
	BiaozhiNum = FindItem(SPECIAL_BIAOZHI);
	if(type == 1) //¶Ä²©ÓÎÏ·±Ò
	{
		if(m_dwDN >= (DWORD)money)
		{
			if(result <= 2000)
			{
				sayStr.Format("%d ¸öÓÎÏ·±Ò",money);
				TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				//sayStr.Format("%d",result);
				//TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				sayStr.Format("Ð¡");
				TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				TempBuf.Add(CLIENT_EVENT_SAY);
				TempBuf.Add((BYTE)SUCCESS);
				TempBuf.Add((short)5002);
				TempBuf.Add((BYTE)0x03);
				TempBuf.AddData(TempSayBuf, TempSayBuf.GetLength());
				Send(TempBuf, TempBuf.GetLength());
				m_dwDN += money;
				SendMoneyChanged();
				UpdateUserItemDN();
				for(int i = 0; i < 5; i++)
				{
					TempBuf3.Add(USE_POTION);		
					TempBuf3.Add(m_uid + USER_BAND);
					TempBuf3.Add((BYTE)5);	//ÖÐ½±Ê±´øÑÌ»¨Ð§¹û
					Send( TempBuf3, TempBuf3.GetLength() );
				}
			}
			else
			{
				sayStr.Format("%d ¸öÓÎÏ·±Ò",money);
				TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				//sayStr.Format("%d",result);
				//TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				sayStr.Format("´ó");
				TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				TempBuf.Add(CLIENT_EVENT_SAY);
				TempBuf.Add((BYTE)SUCCESS);
				TempBuf.Add((short)5003);
				TempBuf.Add((BYTE)0x03);
				TempBuf.AddData(TempSayBuf, TempSayBuf.GetLength());
				Send(TempBuf, TempBuf.GetLength());
				m_dwDN -= money;
				SendMoneyChanged();
				UpdateUserItemDN();
			}
		}
		else
		{
			sayStr.Format("%d",money);
			TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
			TempBuf.Add(CLIENT_EVENT_SAY);
			TempBuf.Add((BYTE)SUCCESS);
			TempBuf.Add((short)5001);
			TempBuf.Add((BYTE)0x01);
			TempBuf.AddData(TempSayBuf, TempSayBuf.GetLength());
			Send(TempBuf, TempBuf.GetLength());
		}

	}
	else		//¶Ä²©µã¾í
	{
		if((DWORD)BiaozhiNum >= (DWORD)money)
		{
			if(result <= 2000)
			{
				sayStr.Format("%d ¸ö±êÖ¾",money);
				TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				//sayStr.Format("%d",result);
				//TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				sayStr.Format("Ð¡");
				TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				TempBuf.Add(CLIENT_EVENT_SAY);
				TempBuf.Add((BYTE)SUCCESS);
				TempBuf.Add((short)5002);
				TempBuf.Add((BYTE)0x03);
				TempBuf.AddData(TempSayBuf, TempSayBuf.GetLength());
				Send(TempBuf, TempBuf.GetLength());
				GiveItem(SPECIAL_BIAOZHI,money);
				SendUserStatusSkill();
				for(int i = 0; i < 5; i++)
				{
					TempBuf3.Add(USE_POTION);		
					TempBuf3.Add(m_uid + USER_BAND);
					TempBuf3.Add((BYTE)5);	//ÖÐ½±Ê±´øÑÌ»¨Ð§¹û
					Send( TempBuf3, TempBuf3.GetLength() );
				}
			}
			else
			{
				sayStr.Format("%d ¸ö±êÖ¾",money);
				TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				//sayStr.Format("%d",result);
				//TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				sayStr.Format("´ó");
				TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
				TempBuf.Add(CLIENT_EVENT_SAY);
				TempBuf.Add((BYTE)SUCCESS);
				TempBuf.Add((short)5003);
				TempBuf.Add((BYTE)0x03);
				TempBuf.AddData(TempSayBuf, TempSayBuf.GetLength());
				Send(TempBuf, TempBuf.GetLength());
				RobItem(SPECIAL_BIAOZHI,money);
				SendUserStatusSkill();
			}
		}
		else
		{
			sayStr.Format("%d",money);
			TempSayBuf.AddString( (LPTSTR)(LPCTSTR)sayStr );
			TempBuf.Add(CLIENT_EVENT_SAY);
			TempBuf.Add((BYTE)SUCCESS);
			TempBuf.Add((short)5001);
			TempBuf.Add((BYTE)0x01);
			TempBuf.AddData(TempSayBuf, TempSayBuf.GetLength());
			Send(TempBuf, TempBuf.GetLength());
		}
	}
}
void USER::NpcShangdian(int huan)//ÉÌµêÄ£Ê½¶Ô»°
{
	CString sayStr;
	CBufferEx TempBuf, TempSayBuf , TempBuf3;
	if(huan == 0)
	{
		if(m_dwJiFen != 0)
		{
			m_dwJiFen = 0;
			SendSystemMsg( "ÎÂÜ°ÌáÊ¾:¿ªÉÌµêÒÑ¾­×ª»»³É[¾öÕ½±ÒÄ£Ê½]", SYSTEM_ERROR, TO_ME);//"Ç®²»¹»."
			return;
		}
		else
		{
			SendSystemMsg( "×ª»»Ê§°Ü!ÄãÏÖÔÚÒÑ¾­ÊÇ[¾öÕ½±ÒÄ£Ê½]", SYSTEM_ERROR, TO_ME);//"Ç®²»¹»."
			return;
		}
	}
	if(huan == 1)
	{
		if(m_dwJiFen != 1)
		{
			m_dwJiFen = 1;
			SendSystemMsg( "ÎÂÜ°ÌáÊ¾:¿ªÉÌµêÒÑ¾­×ª»»³É[±êÖ¾Ä£Ê½]", SYSTEM_ERROR, TO_ME);//"Ç®²»¹»."
			return;	
		}
		else
		{
			SendSystemMsg( "×ª»»Ê§°Ü!ÄãÏÖÔÚÒÑ¾­ÊÇ[±êÖ¾Ä£Ê½]", SYSTEM_ERROR, TO_ME);//"Ç®²»¹»."
			return;
		}
	}
	if(huan == 2)
	{
		if(m_dwJiFen != 2)
		{
			m_dwJiFen = 2;
			SendSystemMsg( "ÎÂÜ°ÌáÊ¾:¿ªÉÌµêÒÑ¾­×ª»»³É[½ð±êÄ£Ê½]", SYSTEM_ERROR, TO_ME);//"Ç®²»¹»."
			return;	
		}
		else
		{
			SendSystemMsg( "×ª»»Ê§°Ü!ÄãÏÖÔÚÒÑ¾­ÊÇ[½ð±êÄ£Ê½]", SYSTEM_ERROR, TO_ME);//"Ç®²»¹»."
			return;
		}
	}
}
void USER::NpcZHUANZHI(int zhuan)
{
	CString sayStr;
	CBufferEx TempBuf, TempSayBuf , TempBuf3;
	if(zhuan == 0)
	{
		if(m_byClass != 0 && m_sLevel > 119)
		{
			if(m_UserItem[1].sSid == -1 && m_UserItem[4].sSid == -1 && m_UserItem[5].sSid == -1 && m_UserItem[8].sSid == -1 )
			{
				if(FindItem(SPECIAL_BIAOZHI) <= 20000)
				{
					SendSystemMsg("¶Ô²»Æð,×ª»»Ö°ÒµÐèÒª20000±êÖ¾.", SYSTEM_ERROR, TO_ME);
					return;
				}
				RobItem(SPECIAL_BIAOZHI,20000);
				m_dwXP += 100000;
				m_byClass = 0;
				SendCHONGSHENG();
				CString strMsg = _T("");
				strMsg.Format( "%s ³É¹¦×ª»»Îª¡º¸ñ¶·¼Ò¡»." ,m_strUserID);
				SendSystemMsg(strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_ANNOUNCE, TO_ALL);
				return;
			}
			else
			{
				SendSystemMsg( "ÇëÐ¶ÏÂÄãÉíÉÏµÄ×°±¸ÎäÆ÷.", SYSTEM_ERROR, TO_ME);//"Ç®²»¹»."
				return;
			}		
		}
		SendSystemMsg( "ÄãÒÑÊÇ¡º¸ñ¶·¼Ò¡»Ö°Òµ,ÎÞÐè×ª»»,»òµÈ¼¶²»µ½120.", SYSTEM_ERROR, TO_ME);
		return;
	}
	if(zhuan == 1)
	{
		if(m_byClass != 1 && m_sLevel > 119)
		{
			if(m_UserItem[1].sSid == -1 && m_UserItem[4].sSid == -1 && m_UserItem[5].sSid == -1 && m_UserItem[8].sSid == -1 )
			{
				if(FindItem(SPECIAL_BIAOZHI) <= 20000)
				{
					SendSystemMsg("¶Ô²»Æð,×ª»»Ö°ÒµÐèÒª20000±êÖ¾.", SYSTEM_ERROR, TO_ME);
					return;
				}
				RobItem(SPECIAL_BIAOZHI,20000);
				m_dwXP += 100000;
				m_byClass = 1;
				SendCHONGSHENG();
				CString strMsg = _T("");
				strMsg.Format( "%s ³É¹¦×ª»»Îª¡ºÄ§·¨Ê¦¡»." ,m_strUserID);
				SendSystemMsg(strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_ANNOUNCE, TO_ALL);
				return;
			}
			else
			{
				SendSystemMsg( "ÇëÐ¶ÏÂÄãÉíÉÏµÄ×°±¸ÎäÆ÷.", SYSTEM_ERROR, TO_ME);//"Ç®²»¹»."
				return;
			}
		}
		SendSystemMsg( "ÄãÒÑÊÇ¡ºÄ§·¨Ê¦¡»Ö°Òµ,ÎÞÐè×ª»»,»òµÈ¼¶²»µ½120.", SYSTEM_ERROR, TO_ME);
		return;
	}
	if(zhuan == 2)
	{
		if(m_byClass != 2 && m_sLevel > 119)
		{
			if(m_UserItem[1].sSid == -1 && m_UserItem[4].sSid == -1 && m_UserItem[5].sSid == -1 && m_UserItem[8].sSid == -1 )
			{
				if(FindItem(SPECIAL_BIAOZHI) <= 20000)
				{
					SendSystemMsg("¶Ô²»Æð,×ª»»Ö°ÒµÐèÒª20000±êÖ¾.", SYSTEM_ERROR, TO_ME);
					return;
				}
				RobItem(SPECIAL_BIAOZHI,20000);
				m_dwXP += 100000;
				m_byClass = 2;
				SendCHONGSHENG();
				CString strMsg = _T("");
				strMsg.Format( "%s ³É¹¦×ª»»Îª¡º½£ÎäÊ¿¡»." ,m_strUserID);
				SendSystemMsg(strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_ANNOUNCE, TO_ALL);
				return;
			}
			else
			{
				SendSystemMsg( "ÇëÐ¶ÏÂÄãÉíÉÏµÄ×°±¸ÎäÆ÷.", SYSTEM_ERROR, TO_ME);//"Ç®²»¹»."
				return;
			}
		}
		SendSystemMsg( "ÄãÒÑÊÇ¡º½£ÎäÊ¿¡»Ö°Òµ,ÎÞÐè×ª»»,»òµÈ¼¶²»µ½120.", SYSTEM_ERROR, TO_ME);
		return;
	}
	if(zhuan == 3)
	{
		if(m_byClass != 3 && m_sLevel > 119)
		{
			if(m_UserItem[1].sSid == -1 && m_UserItem[4].sSid == -1 && m_UserItem[5].sSid == -1 && m_UserItem[8].sSid == -1 )
			{
				if(FindItem(SPECIAL_BIAOZHI) <= 20000)
				{
					SendSystemMsg("¶Ô²»Æð,×ª»»Ö°ÒµÐèÒª20000±êÖ¾.", SYSTEM_ERROR, TO_ME);
					return;
				}
				RobItem(SPECIAL_BIAOZHI,20000);
				m_dwXP += 100000;
				m_byClass = 3;
				SendCHONGSHENG();
				CString strMsg = _T("");
				strMsg.Format( " %s ³É¹¦×ª»»Îª¡ºÇ¹ÐµÊ¦¡»." ,m_strUserID);
				SendSystemMsg(strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_ANNOUNCE, TO_ALL);
				return;
			}
			else
			{
				SendSystemMsg( "ÇëÐ¶ÏÂÄãÉíÉÏµÄ×°±¸ÎäÆ÷.", SYSTEM_ERROR, TO_ME);//"Ç®²»¹»."
				return;
			}
		}
		SendSystemMsg( "ÄãÒÑÊÇ¡ºÇ¹ÐµÊ¦¡»Ö°Òµ,ÎÞÐè×ª»»,»òµÈ¼¶²»µ½120.", SYSTEM_ERROR, TO_ME);
		return;
	}
}
/////////////////////////////////////////////////////////////////////////
//	ÇØ´ç ÇÊµå »óÀÎ¿¡ °ü·ÃµÈ Á¤º¸¸¦ ¼±º°ÇØ¼­ ¾Ë·ÁÁØ´Ù.
//
void USER::SendFieldGuildSay(CNpc *pNpc, int nStore, int say)
{
	int nChatNum = -1;
	CStore* pStore = NULL;
	CGuildFortress* pFort = NULL;

	if(nStore >= FORTRESS_BAND)
	{
		pFort = GetFortress(nStore);
		if(pFort == NULL) return;

		if(pNpc)
		{
			if(pNpc->m_tNpcType == NPCTYPE_GUILD_GUARD)
			{
				if(pFort->m_tWarType != GUILD_WAR_DECISION)
				{
					if(pFort->m_sFortressID == 1000) nChatNum = 145;
					else if(pFort->m_sFortressID == 1001) nChatNum = 165;
					else if(pFort->m_sFortressID == 1002) nChatNum = 318;
				}
				else
				{
					if(pFort->m_iGuildSid != m_dwGuild && m_bGuildMaster)
					{
						for(int i = 0; i < GUILDFORTRESS_ATTACK_MAX_NUM; i++)
						{
							if(pFort->m_arAttackGuild[i].lGuild == m_dwGuild)
							{
								if(pFort->m_sFortressID == 1000) nChatNum = 144;
								else if(pFort->m_sFortressID == 1001) nChatNum = 164;
								else if(pFort->m_sFortressID == 1002) nChatNum = 317;
							}
						}

						if(nChatNum == -1)
						{
							if(pFort->m_iGuildSid == 900000)
							{
								if(pFort->m_sFortressID == 1000) nChatNum = 146;
								else if(pFort->m_sFortressID == 1001) nChatNum = 166;
								else if(pFort->m_sFortressID == 1002) nChatNum = 319;
							}
							else 
							{
								if(pFort->m_sFortressID == 1000) nChatNum = 143;
								else if(pFort->m_sFortressID == 1001) nChatNum = 163;
								else if(pFort->m_sFortressID == 1002) nChatNum = 316;
							}
						}
					}
					else 
					{
						if(pFort->m_sFortressID == 1000) nChatNum = 144;
						else if(pFort->m_sFortressID == 1001) nChatNum = 164;
						else if(pFort->m_sFortressID == 1002) nChatNum = 317;
					}
				}
			}
			else
			{
				nChatNum = say;
			}
		}
		else
		{
			nChatNum = say;
		}
		EditHyperFortressText(pFort, nChatNum);
	}
}

/////////////////////////////////////////////////////////////////////////
//	°æºñ¿ø¿¡°Ô ±æµå ÀüÀ» ½ÅÃ»ÇÑ´Ù.
//
void USER::GuildWarApply(int nStore)
{
	int nChatNum = -1;
	int nCount = 0;
	CStore* pStore = NULL;
	CGuildFortress* pFort = NULL;

	if(nStore >= FORTRESS_BAND)						// ¿ä»õ¿¡ ¼ÓÇÑ »óÁ¡ÀÌ¸é...
	{
		pFort = GetFortress(nStore);
		if(pFort == NULL) return;

		if(pFort->m_tWarType == GUILD_WAR_DECISION) 
		{	
			if(m_bGuildMaster) 
			{
				CBufferEx TempBuf;

				TempBuf.Add(GUILD_WAR_APPLY_OPEN);
				TempBuf.Add((short)pFort->m_sFortressID);
				TempBuf.AddString(pFort->m_strGuildName);

				Send(TempBuf, TempBuf.GetLength());
				return;
			}		
		}

		nChatNum = 145;

		EditHyperFortressText(pFort, nChatNum);
	}
}

/////////////////////////////////////////////////////////////////////////
//	°æºñ¿ø¿¡°Ô ±æµå ÀüÀ» ½ÅÃ»ÇÑ´Ù.
//
void USER::GuildWarApplyReq(TCHAR *pBuf)
{
	int index = 0;

	if(!m_bGuildMaster || m_dwGuild <= 0)
	{
		SendSystemMsg( IDS_USER_ONLY_GUILD_MASTER_WAR, SYSTEM_NORMAL, TO_ME);//"Ö»ÓÐ¾üÍÅ³¤²ÅÄÜÉêÇëÕ½Õù."
		return;
	}

	CString strMsg;
	short sStore = GetShort(pBuf, index);

	if(sStore < 0) return;

	if(sStore >= FORTRESS_BAND)	GuildFortressWarApplyReq(sStore);	
}
BOOL USER::GuildFortressWarApplyReq(int sid)
{
	int i, j, nLen = 0;

	CString strMsg;

	CGuild *pGuild = NULL;
	CGuildFortress *pFort = NULL;
	CGuildFortress *pTempFort = NULL;
	//JunTuanName = _T("");

	pFort = GetFortress(sid);

	if(!pFort) return FALSE;

	if(m_sLevel < 70)
	{
		SendSystemMsg( IDS_USER_APPLY_CONDITION_LEVEL, SYSTEM_NORMAL, TO_ME);//"Ö»ÓÐ¼¶±ðÔÚ70¼¶ÒÔÉÏÎÞ¾üÍÅÈËÎï²Å¿É½¨Á¢¾üÍÅ."
		return FALSE;
	}

	if(pFort->m_lUsed == 1)
	{
		SendSystemMsg( IDS_USER_CANT_APPLY_IN_WAR, SYSTEM_NORMAL, TO_ME);//"¾üÍÅÎÞ·¨ÉêÇëÕ½Õù"
		return FALSE;
	}

	if(pFort->m_iGuildSid == m_dwGuild)
	{
		SendSystemMsg( IDS_USER_CANT_APPLY_TO_MY_GUILD, SYSTEM_NORMAL, TO_ME);//"ÎÞ·¨ÓëÓµÓÐÒªÈûµÄ¾üÍÅ½øÐÐÕ½Õù."
		return FALSE;
	}

	pGuild = GetGuild(m_dwGuild);

	if(!pGuild) 
	{
		ReleaseGuild();				// ÇØÁ¦...
		return FALSE;
	}

	j = 0;
	CGuildUser pGUser;

	for(i = 0; i < MAX_GUILD_USER; i++)
	{
		pGUser = pGuild->m_arUser[i];
		if(pGUser.m_lUsed != 0) j++;
	}

	ReleaseGuild();

	if(j < 5)//20
	{
		SendSystemMsg( IDS_USER_APPLY_CONDITION_GMEMBER, SYSTEM_NORMAL, TO_ME);
		return FALSE;//ÄãµÄ¾üÍÅÈËÊý²»×ã ÎÞ·¨ÉêÇëÕ½Õù
	}
/*
	for(i = 0; i < g_arGuildHouse.GetSize(); i++)
	{
		if(g_arGuildHouse[i]->iGuild == m_dwGuild)
		{
			SendSystemMsg( IDS_USER_CANT_APPLY_HAVE_HOUSE, SYSTEM_NORMAL, TO_ME);//"ÒÑÓµÓÐ¾üÍÅ»ùµØÎÞ·¨ÉêÇë»ùµØÕ½Õù."
			return;
		}
	}
*/
	for(i = 0; i < g_arGuildFortress.GetSize(); i++)
	{
		if(g_arGuildFortress[i] == NULL) continue;
		pTempFort = g_arGuildFortress[i];
		if(pTempFort == NULL) continue;//MarkJay ÐÞ¸Ä
		if(pTempFort->m_iGuildSid == m_dwGuild)
		{
			SendSystemMsg( IDS_USER_CANT_APPLY_HAVE_FORTRESS, SYSTEM_NORMAL, TO_ME);//"ÒÑÓµÓÐÒªÈûÎÞ·¨ÉêÇëÒªÈûÕ½Õù."
			return FALSE;
		}
	}

	for(i = 0; i < g_arGuildFortress.GetSize(); i++)
	{
		if(g_arGuildFortress[i] == NULL) continue;
		if(pTempFort == NULL) continue;//MarkJay ÐÞ¸Ä
		pTempFort = g_arGuildFortress[i];

		if(InterlockedCompareExchange((LONG*)&g_arGuildFortress[i]->m_lListUsed, (long)1, (long)0) == (long)0)
		{
			for(j = 0; j < GUILDFORTRESS_ATTACK_MAX_NUM; j++)
			{
				if(pTempFort->m_arAttackGuild[j].lGuild == m_dwGuild)
				{
					if(pTempFort->m_sFortressID == 1000) strMsg.Format( IDS_USER_ALREADY_LUINET_FORTRESS );// "ÒÑÓµÓÐÂÞÄáÌØÒªÈûÎÞ·¨ÉêÇë."
					else if(pTempFort->m_sFortressID == 1001) strMsg.Format( IDS_USER_ALREADY_SANAD_FORTRESS );//"ÒÑÓµÓÐÉ³ÄÉµÂÒªÈûÎÞ·¨ÉêÇë."
					else if(pTempFort->m_sFortressID == 1002) strMsg.Format( IDS_USER_ALREADY_ZAMELLYA_FORTRESS );//"ÒÑÓµÓÐÕ§ÄªÀïÒªÈûÎÞ·¨ÉêÇëÒªÈûÕ½Õù."

					SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);					
					InterlockedExchange(&g_arGuildFortress[i]->m_lListUsed, 0);
					return FALSE;
				}
			}
			InterlockedExchange(&g_arGuildFortress[i]->m_lListUsed, 0);
		}
	}

	if(pFort->m_tWarType == GUILD_WAR_DECISION)
	{
		for(i = 0; i < g_arGuildFortress.GetSize(); i++)
		{
			if(g_arGuildFortress[i] == NULL) continue;

			if(g_arGuildFortress[i]->m_sFortressID == sid)
			{
				if(g_arGuildFortress[i]->m_lListCount >= GUILDFORTRESS_ATTACK_MAX_NUM) 
				{
					strMsg.Format( IDS_USER_OVERFLOW_GUILD_WAR_ENEMY, pFort->m_strGuildName);//"%s ±ØÐëÓÐ×ã¹»µÄ³ÉÔ±."
					SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);					
					return FALSE;
				}

				if(InterlockedCompareExchange((LONG*)&g_arGuildFortress[i]->m_lListUsed, (long)1, (long)0) == (long)0)
				{
/*					nLen = strlen(m_strGuildName);
					g_arGuildFortress[i]->m_arAttackGuild[g_arGuildFortress[i]->m_lListCount].lGuild = m_dwGuild;
					strncpy(g_arGuildFortress[i]->m_arAttackGuild[g_arGuildFortress[i]->m_lListCount].strGuildName, m_strGuildName, nLen);
					InterlockedIncrement(&g_arGuildFortress[i]->m_lListCount);
*/					
					if(UpdateFortress(g_arGuildFortress[i]) == FALSE)
					{
						SendSystemMsg( IDS_USER_APPLY_FAIL, SYSTEM_NORMAL, TO_ME);//"¾üÍÅÕ½ÕùÉêÇëÊ§°Ü."
						InterlockedExchange(&g_arGuildFortress[i]->m_lListUsed, 0);
						return FALSE;
					}

//					UpdateMemAttackListNpc(g_arGuildFortress[i]->m_sFortressID);// Agent¿¡´Â ´õÀÌ»ó ÀúÀå¾ÈÇÑ´Ù.
					pTempFort = g_arGuildFortress[i];
					if(pTempFort == NULL) continue;//MarkJay ÐÞ¸Ä
					for(j = 0; j < GUILDFORTRESS_ATTACK_MAX_NUM; j++)
					{
						if(InterlockedCompareExchange((LONG*)&g_arGuildFortress[i]->m_arAttackGuild[j].lUsed, (long)1, (long)0) == (long)0)
						{
							nLen = strlen(m_strGuildName);
							g_arGuildFortress[i]->m_arAttackGuild[j].lGuild = m_dwGuild;
							strncpy(g_arGuildFortress[i]->m_arAttackGuild[j].strGuildName, m_strGuildName, nLen);

							SendSystemMsg( IDS_USER_APPLY_COMPLETED, SYSTEM_NORMAL, TO_ME);//"¾üÍÅÕ½ÕùÉêÇë³É¹¦."
							//JunTuanName.AppendFormat("%s\n\n",m_strGuildName);
							CString strMsg;
             				strMsg.Format("%s ¾üÍÅÉêÇëÁËÒªÈûÕ½Õù.",m_strGuildName);
            				SendSystemMsg( strMsg.GetBuffer(), SYSTEM_ANNOUNCE, TO_ALL);
							break;
						}
					}										

					InterlockedExchange(&g_arGuildFortress[i]->m_lListUsed, 0);
				}
			}
		}
	}
	else SendSystemMsg( IDS_USER_NOT_FIX_SCHEDULE, SYSTEM_NORMAL, TO_ME);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////
//	°°ÀºÁ¸¾ÈÀÇ Æ¯Á¤Àå¼Ò·Î ÀÌµ¿½ÃÅ²´Ù.
//
void USER::EventMove(int zone, int moveX, int moveY)
{
	BYTE result = FAIL;
	int index = 0;

	if(zone != m_curz) return;

	CPoint pt = FindNearAvailablePoint_C(moveX, moveY);
	CPoint pc;
	if(pt.x != -1 && pt.y != -1) 
	{
		pc = ConvertToServer(pt.x, pt.y);
		if(pc.x < 0 || pc.y < 0 || pc.x >= g_zone[m_ZoneIndex]->m_sizeMap.cx || pc.y >= g_zone[m_ZoneIndex]->m_sizeMap.cy) 
		{
			result = FAIL;
		}
		else
		{
			if(m_curx < 0 || m_cury < 0) return;//ÐÞÕý×ø±ê±¨´í
			::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser, 0);
			::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[pc.x][pc.y].m_lUser, USER_BAND + m_uid);
			m_curx = pc.x;
			m_cury = pc.y;

			result = SUCCESS;
		}
	}

	index = 0;
	SetByte(m_TempBuf, MOVE_CHAT_RESULT, index);
	SetByte(m_TempBuf, result, index);

	if(result == FAIL) 
	{
		Send(m_TempBuf, index);
		return;
	}

	SetInt(m_TempBuf, m_uid + USER_BAND, index);
	SetShort(m_TempBuf, pt.x, index);
	SetShort(m_TempBuf, pt.y, index);
	Send(m_TempBuf, index);

	m_dwNoDamageTime = NO_DAMAGE_TIME;			// ¹«Àû½Ã°£ ¼ÂÆÃ
	m_dwLastNoDamageTime = GetTickCount();

	SendInsight(m_TempBuf, index);

	SightRecalc();
}

/////////////////////////////////////////////////////////////////////////
//	¸ðµç HP, PP, SP¸¦ ²ËÃ¤¿î´Ù.
//
void USER::SendAllCure(int iStoreID, int iCost)
{
	BYTE result = SUCCESS;

	CBufferEx TempBuf;

	DWORD dwCost = (DWORD)iCost;

	CStore *pStore = NULL;
	pStore = GetStore(iStoreID);

	if(!pStore) return;

	// ÀÌº¥Æ® À§Ä¡¸¦ °Ë»ç ----------------------------------------//
	CPoint pt = ConvertToClient(m_curx, m_cury);
	if(!g_pEventBlock->CheckUserEvent(m_curz, pt.x, pt.y, CURE_BLOCK)) return;

	DWORD dwTax = (DWORD)( dwCost * ((double)pStore->m_sRate/100) );
	dwCost = (DWORD)(dwCost + dwTax + 0.999);			// ¿Ã¸²(DWORD ·Î ¸¸µé±â À§ÇØ¼­)

	UpdateFortressTax(iStoreID, dwTax);					// Æ÷Æ®¸®½º ¼¼±ÝÀÌ¸é ÀúÀå...

	if(m_sHP >= m_sMagicMaxHP && m_sPP >= m_sMagicMaxPP && m_sSP >= m_sMagicMaxSP)
	{
		SendSystemMsg( IDS_USER_NEED_NOT_CURE, SYSTEM_NORMAL, TO_ME);// "ÄãÒÑ¾­²»ÐèÒªÖÎÁÆ."
		result = FAIL;
		goto go_result;
	}

	if(dwCost > m_dwDN) 
	{ 
		SendSystemMsg( IDS_USER_NOT_ENOUGH_DINA_CURE, SYSTEM_NORMAL, TO_ME); //"ÖÎÁÆÊ§°Ü,Ã»ÓÐ×ã¹»µÄ½ðÇ®."
		result = FAIL;
		goto go_result;
	}
	else m_dwDN = m_dwDN - dwCost;

	m_sHP = m_sMagicMaxHP;
	m_sPP = m_sMagicMaxPP;
	m_sSP = m_sMagicMaxSP;

go_result:
	TempBuf.Add(CURE_RESULT);

	if(result == FAIL) 
	{
		TempBuf.Add(result);			// ½ÇÆÐ
		Send(TempBuf, TempBuf.GetLength());
		return;
	}

	TempBuf.Add((BYTE)0x01);			// È¸º¹

	TempBuf.Add(m_sHP);
	TempBuf.Add(m_sPP);
	TempBuf.Add(m_sSP);

	SendMoneyChanged();
	Send(TempBuf, TempBuf.GetLength());
	if(m_bNowBuddy == TRUE) SendBuddyUserChange(BUDDY_CHANGE);		// ¹öµðÁßÀÌ¸é ´Ù¸¥ ¹öµð¿ø¿¡°Ô ³¯¸°´Ù.

	SendSystemMsg( IDS_USER_CURE_COMPLETED, SYSTEM_NORMAL, TO_ME);//"ÖÎÁÆ³É¹¦."
}

/////////////////////////////////////////////////////////////////////////
//	Zone Change¸¦ ½ÃÀÛÇÑ´Ù.
//	//^^ ÀÌ ÇÔ¼ö ÀÚÃ¼µµ BOOL Çü½ÄÀÌ¾î¾ß ÇÒ °Í °°´Ù.
void USER::ZoneMoveReq(int zone, int moveX, int moveY)
{
	BOOL bSuccess = FALSE;							// ½ÇÆÐ, ¼º°øÀÇ °á°ú°ªÀ» ÀúÀå
	BOOL bFoundDBIndex = FALSE;

	int i, j;

	if( !IsZoneInThisServer(zone) )
	{
		SERVERINFO* pNewData;
		int iNum, sZone, find_flag = FALSE;
		CString strIP;
		int nPort;

		// Á¸¿¡ ÇØ´çÇÏ´Â IP, PORT Ã£±â...
		for( i = 0; i < g_server.GetSize() && find_flag == FALSE; i++)
		{
			BOOL bTrue =FALSE;
			pNewData = g_server[i];

			if ( pNewData == NULL ) continue;
																		// -1ÀÌ¸é ¸ðµÎ°¡ °¥ ¼ö ÀÖ´Â ¼­¹öÀÌ´Ï±î Ã¼Å©ÇÏ°Ô ÇÏ°í
			if( pNewData->m_sDBIndex != -1 )							// ¾Æ´Ï¸é ³»°¡ °¥ ¼ö ÀÖ´Â ¼­¹öÀÎÁö Ã¼Å©ÇÏ°Ô ÇÑ´Ù
			{
				if( pNewData->m_sDBIndex != m_iMyServer ) 
				{
					if(pNewData->m_sDBIndex == 11 &&(m_iMyServer == 1 || m_iMyServer == 2 ||m_iMyServer == 3 ||m_iMyServer == 4 ||m_iMyServer == 5))
						bTrue = TRUE;
					else if(pNewData->m_sDBIndex == 12 &&(m_iMyServer == 6 || m_iMyServer == 7 ||m_iMyServer == 8 ||m_iMyServer == 9 ||m_iMyServer == 10))
						bTrue = TRUE;
					else 
						continue;		// ÀÌ ¼­¹ö Á¤º¸°¡ ³ªÀÇ µ¥ÀÌÅÍ¸¦ °¡Áö°í ÀÖ´Â ¼­¹ö¿©¾ß ÇÑ´Ù.
				}
			}
			
			// -1ÀÌ°Å³ª °¥ ¼ö ÀÖ´Â ¼­¹ö¶ó°í ÆÇ´ÜµÉ¶§
			iNum = pNewData->m_zone.GetSize();
			for ( j = 0; j < iNum; j++ )
			{
				sZone = *(pNewData->m_zone[j]);
				if ( sZone == zone )				// ³»°¡ °¡°íÀÚ ÇÏ´Â Á¸ÀÌ ÀÖ´ÂÁö Ã¼Å©
				{
					find_flag = TRUE;				// ÀÖÀ¸¸é ±× ¼­¹öÀÇ ÁÖ¼Ò¸¦ ³»·ÁÁØ´Ù.
					m_strZoneIP = strIP = pNewData->m_szAddr;
					m_nZonePort = nPort = pNewData->m_nPort;

					break;
				}
			}
		}

		if ( find_flag == TRUE )
		{
			TRACE("INTER COMPUTER ZONE CHANGE STARTED[]...\n");

			if( m_bZoneLogOut ) return;

			m_bZoneLogOut = TRUE;

			g_pMainDlg->BridgeServerUserZoneLogOut( m_uid, m_strUserID, zone, moveX, moveY );
			return;

			CPoint pt = ConvertToServerByZone( zone, moveX, moveY );

			if( pt.x < 0 || pt.y < 0 ) return;

			int save_z = zone;
			int save_x = pt.x;
			int save_y = pt.y;

			m_strZoneIP = strIP;
			m_nZonePort	= nPort;

			// µ¥ÀÌÅ¸ ÀúÀå...
			TRACE("Zone Change Data Saved...\n");
			ZoneLogOut(save_z, save_x, save_y);
			//

			m_curz = zone;
			m_curx = save_x;
			m_cury = save_y;

			CBufferEx TempBuf;
			BYTE result = FAIL;
			
			TempBuf.Add(ZONE_CHANGE_RESULT);

			result = SUCCESS_ZONE_CHANGE;
			TempBuf.Add(result);
			TempBuf.AddString((char *)LPCTSTR(strIP));
			TempBuf.Add(nPort);

			Send(TempBuf, TempBuf.GetLength());
			return;
		}
		//

		SendZoneChange(FALSE);
		return;
	}

	// °°Àº ¼­¹ö°£ÀÇ Á¸ º¯È¯...
	bSuccess = ZoneChangeProcess(zone, moveX, moveY);
	
	if(bSuccess)
	{
		SendWeatherInMoveZone();			// ÀÌµ¿ Á¸ÀÇ ³¯¾¾º¯È­¸¦ ¾Ë¸°´Ù.
		SightRecalc();						// ³» ½Ã¾ß¾ÈÀÇ Á¤º¸¸¦ ³ª¿¡°Ô º¸³½´Ù.
	}

	TRACE("SAVE-COMPUTER ZONE CHANGE...\n");
}


/////////////////////////////////////////////////////////////////////////
//	Á¸Ã¼ÀÎÁöÀÇ ½ÇÁ¦ Ã³¸®¸¦ ´ã´çÇÑ´Ù.
//
BOOL USER::ZoneChangeProcess(int new_z, int moveX, int moveY)
{
	int i, rand;
	BYTE result = SUCCESS;

	MAP* pMap = NULL;

	if( m_ZoneIndex < 0 || m_ZoneIndex >= g_zone.GetSize() ) return FALSE;

	pMap = g_zone[m_ZoneIndex];		if( !pMap ) return FALSE;

	if( m_curx < 0 || m_curx >= pMap->m_sizeMap.cx ) return FALSE;
	if( m_cury < 0 || m_cury >= pMap->m_sizeMap.cy ) return FALSE;

//	if( pMap->m_pMap[m_curx][m_cury].m_lUser != USER_BAND + m_uid ) return FALSE;

//	if( g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser != USER_BAND + m_uid ) return FALSE;		
					
	CPoint ptPotal, pt;
	m_NowZoneChanging = TRUE;					// Zone Change ÇÏ°í ÀÖ´Ù.

	/*************************[ Back Up ]***************************************/	
	int org_x = m_curx;
	int org_y = m_cury;
	int org_z = m_curz;

	/*************************[ Zone Change Init]*******************************/
	SetZoneIndex(new_z);

	ptPotal = ConvertToServer(moveX, moveY);		// ÀÌº¥Æ® ÁÂÇ¥´Â Å¬¶óÀÌ¾ðÆ® ±âÁØÀÌ¹Ç·Î...

	if( ptPotal.x == -1 || ptPotal.y == -1 )
	{
		SetZoneIndex(org_z);
		return FALSE;
	}

	i = SEARCH_TOWN_POTAL_COUNT;				// ÁÂÇ¥¸¦ Ã£±âÀ§ÇÑ È½¼ö
	
	pt = FindNearAvailablePoint_S(ptPotal.x, ptPotal.y);

	if(pt.x == -1 || pt.y == -1) 
	{
		while(TRUE)								// Å¸¿îÆ÷Å» ·¥´ý ÁÂÇ¥¸¦ ¾ò´Â´Ù.
		{
			rand = myrand(-3, 3);
			ptPotal.x += rand; ptPotal.y += rand;
			pt = FindNearAvailablePoint_S(ptPotal.x, ptPotal.y);
			if(pt.x != -1 && pt.y != -1) break;
			i--;
			if(i <= 0) // ³ªÁß¿¡
			{ 
				pt.x = -1; pt.y = -1;
				break;
			}
		}
	}

	if(pt.x == -1 || pt.y == -1) 
	{
		SetZoneIndex(org_z);
		m_NowZoneChanging = FALSE;
		return FALSE;
	}
	else									// µé¾î°¥¼ö¾ø´ÂÁö¿ªÀÌ¸é...
	{
		int type = ((g_zone[m_ZoneIndex]->m_pMap[pt.x][pt.y].m_dwType & 0xFF00 ) >> 8);
		if(!CheckInvakidZoneChangeState(type)) 
		{
			SetZoneIndex(org_z);
			m_NowZoneChanging = FALSE;
			return FALSE;
		}
	}

	SetZoneIndex(org_z);					// ÀÌÀü ¼¿ À¯Àú¿¡°Ô  ¾Ë¸°´Ù.

	if(m_curx < 0 || m_cury < 0) return FALSE;//ÐÞÕý×ø±ê±¨´í
	if(InterlockedCompareExchange((LONG*)&g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser, 
			(long)0, (long)(m_uid + USER_BAND)) == (long)m_uid + USER_BAND)
	{
//		SetUid(m_curx, m_cury, 0);				
		SendMyInfo(TO_INSIGHT, INFO_DELETE);
		
		SetZoneIndex(new_z);						
		m_curx = pt.x;
		m_cury = pt.y;
		m_curz = new_z;
		SetUid(m_curx, m_cury, m_uid + USER_BAND );// ¾ÕÀ¸·Î ÀÌµ¿ÇÒ ÀÚ¸®¸¦ È®º¸, À§Ä¡Á¤º¸ ¼ÂÆÃ

		SendZoneChange(TRUE);	// Á¸Ã¼ÀÎÁö¸¦  ¸ÕÀúº¸³»¾ß ³ªÁß NPCÁ¤º¸°¡ client¿¡¼­ »ì¾ÆÀÖ´Ù.

		m_presx = -1;
		m_presy = -1;
		SightRecalc();
		SendMyInfo( TO_INSIGHT, INFO_MODIFY );
		
		m_NowZoneChanging = FALSE;

		return TRUE;
	}
	else
	{
		SendMyInfo(TO_INSIGHT, INFO_DELETE);

		SetZoneIndex(new_z);						
		m_curx = pt.x;
		m_cury = pt.y;
		m_curz = new_z;
		SetUid(m_curx, m_cury, m_uid + USER_BAND );// ¾ÕÀ¸·Î ÀÌµ¿ÇÒ ÀÚ¸®¸¦ È®º¸, À§Ä¡Á¤º¸ ¼ÂÆÃ

		SendZoneChange(TRUE);	// Á¸Ã¼ÀÎÁö¸¦  ¸ÕÀúº¸³»¾ß ³ªÁß NPCÁ¤º¸°¡ client¿¡¼­ »ì¾ÆÀÖ´Ù.

		m_presx = -1;
		m_presy = -1;
		SightRecalc();
		SendMyInfo( TO_INSIGHT, INFO_MODIFY );
		
		m_NowZoneChanging = FALSE;

		return TRUE;

//		m_NowZoneChanging = FALSE;
//		return FALSE;
	}
}

/////////////////////////////////////////////////////////////////////////
//	Á¸Ã¼ÀÎÁö¸¦ À¯Àú¿¡°Ô º¸³½´Ù.
//
void USER::SendZoneChange(BOOL bResult)
{
	CBufferEx TempBuf;
	BYTE result = FAIL;
	
	TempBuf.Add(ZONE_CHANGE_RESULT);
	
	if(bResult == FALSE)
	{
		TempBuf.Add(result);
		Send(TempBuf, TempBuf.GetLength());			// ½ÇÆÐ¸¦ ¸®ÅÏ
		return;
	}

	CPoint pos = ConvertToClient( m_curx, m_cury );
	if( pos.x == -1 || pos.y == -1 ) { pos.x = 1; pos.y = 1; }	// ÀÌºÎºÐÀº ³ªÁß¿¡ ¹Ù²ÙÀÚ

	result = SUCCESS;
	TempBuf.Add(result);
	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.Add((short)pos.x);
	TempBuf.Add((short)pos.y);
	TempBuf.Add((short)m_curz);

	Send(TempBuf, TempBuf.GetLength());

	m_dwNoDamageTime = NO_DAMAGE_TIME;
	m_dwLastNoDamageTime = GetTickCount();
}

/////////////////////////////////////////////////////////////////////////
//	NPC ´ëÈ­Á¤º¸¸¦ º¸³½´Ù.
//
void USER::SendNpcSay(CNpc *pNpc, int nChatNum)
{
TRACE( "MarkJay DEGBUG NPC SAY %d\n", nChatNum);
	CBufferEx TempBuf;

	TempBuf.Add(CLIENT_EVENT_SAY);
	TempBuf.Add((BYTE)SUCCESS);
	TempBuf.Add((short)nChatNum);
	TempBuf.Add((BYTE)0x00);		

	Send(TempBuf, TempBuf.GetLength());
}
//////////////////////////////////////////////////////////////////////////////////
//	µµ¿ò¸» ÀÌº¥Æ®¸¦ ½ÇÇàÇÑ´Ù.
//
void USER::HelpStatusOpenReq()
{
	SendNpcSay(NULL, 7);
}

//////////////////////////////////////////////////////////////////////////////////
//	»óÁ¡Å¬·¡½ºÀÇ Æ÷ÀÎÅÍ¸¦ ¾ò´Â´Ù.
//
CStore* USER::GetStore(int nStore)
{
	for(int i = 0; i < g_arStore.GetSize(); i++)
	{
		if(g_arStore[i] == NULL) continue;

		if(g_arStore[i]->m_sStoreID == (short)nStore)
		{
//			int temp = g_arStore[i]->m_sRate;
			return g_arStore[i];
		}
	}

	return NULL;	
}

//////////////////////////////////////////////////////////////////////////////////
//	¿ä»õÅ¬·¡½ºÀÇ Æ÷ÀÎÅÍ¸¦ ¾ò´Â´Ù.
//
CGuildFortress* USER::GetFortress(int nFort)
{
	for(int i = 0; i < g_arGuildFortress.GetSize(); i++)
	{
		if(g_arGuildFortress[i] == NULL) continue;

		if(g_arGuildFortress[i]->m_sFortressID == (short)nFort)
		{
			return g_arGuildFortress[i];
		}
	}

	return NULL;	
}

//////////////////////////////////////////////////////////////////////////////////
//	»óÁ¡Å¬·¡½ºÀÇ Æ÷ÀÎÅÍ¸¦ ¾ò´Â´Ù.
//
CStore* USER::GetPsiStore(int nStore)
{
	for(int i = 0; i < g_arPsiStore.GetSize(); i++)
	{
		if(g_arPsiStore[i] == NULL) continue;

		if(g_arPsiStore[i]->m_sStoreID == (short)nStore)
		{
			return g_arPsiStore[i];
		}
	}

	return NULL;	
}

//////////////////////////////////////////////////////////////////////////////////
//	»óÁ¡Å¬·¡½ºÀÇ Æ÷ÀÎÅÍ¸¦ ¾ò´Â´Ù.
//
CStore* USER::GetEbodyStore(int nStore)
{
	for(int i = 0; i < g_arEBodyStore.GetSize(); i++)
	{
		if(g_arEBodyStore[i] == NULL) continue;

		if(g_arEBodyStore[i]->m_sStoreID == (short)nStore)
		{
			return g_arEBodyStore[i];
		}
	}

	return NULL;	
}
//////////////////////////////////////////////////////////////////////////////////
//´ò¿ªÉÌÆ·´°¿Ú
void USER::SendStoreOpen(int nStore, int nRate)
{
	int rank = m_sCityRank + CITY_RANK_INTERVAL;
	if(rank == CHAONISE_RANK && IsCity()) 
	{
		SendSystemMsg( IDS_USER_CANT_USE_YOUR_CITY_RANK, SYSTEM_NORMAL, TO_ME);//"ÊÐÃñµÈ¼¶Ì«µÍ."
		return;
	}
	CStore* pStore = NULL;
	pStore = GetStore(nStore);
	if(pStore == NULL) return;	
	CBufferEx TempBuf;
	int i = 0;
	short sid = 0;
	DWORD dwCost = 0;
	TempBuf.Add(STORE_OPEN);
	TempBuf.Add((short)nStore);
	TempBuf.Add((short)pStore->m_sRate);
	TempBuf.Add((short)(pStore->m_arItems.GetSize()));
	for(i = 0; i < pStore->m_arItems.GetSize(); i++)
	{
		sid = (short)pStore->m_arItems[i];
		if(sid < 0 || sid >= g_arItemTable.GetSize()) continue;//MarkJay ÐÞ¸Ä
		if(pStore->m_arNpcList.GetSize() > 0)
		{
			int iCost = (int)(g_arItemTable[sid]->m_iDN * 0.9);//ÊÐÃñµÈ¼¶ÓÅ»Ý
			dwCost = iCost + (int)( iCost * ((double)(pStore->m_sRate)/100) );
		}
		else if(pStore->m_sStoreID == 50 || pStore->m_sStoreID == 51 || pStore->m_sStoreID == 52)
		{
			int iCost = (int)(g_arItemTable[sid]->m_iDN);//¹Ø±ÕÊÐÃñµÈ¼¶ÓÅ»Ý
			dwCost = iCost + (int)( iCost * ((double)(pStore->m_sRate)/100) );
		}
		else dwCost = GetItemCostByCityRank(sid, pStore->m_sRate);
		TempBuf.Add(sid);
		TempBuf.Add(dwCost);
	}
	Send(TempBuf, TempBuf.GetLength());
}

//////////////////////////////////////////////////////////////////////////////////
//	±æµå°¡ ¼ÓÇÑ »óÁ¡ÀÎÁö ÆÇ´ÜÇØ¼­ »óÁ¡À» ¿ÀÇÂÇÑ´Ù.
//
void USER::SendFieldStoreOpen(int nStore, int nHave, int nEmpty)
{
	int nChatNum = -1;
	CStore* pStore = NULL;
	CGuildFortress* pFort = NULL;
	if(nStore >= FORTRESS_BAND)						// ¿ä»õ¿¡ ¼ÓÇÑ »óÁ¡ÀÌ¸é...
	{
		pFort = GetFortress(nStore);
		if(pFort == NULL) return;

		if(pFort->m_lUsed == 1) 
		{
			SendSystemMsg(IDS_USER_CANT_USE_IN_GUILD_WAR, SYSTEM_NORMAL, TO_ME);//"¾üÍÅÕ½ÕùÖÐÎÞ·¨Ê¹ÓÃ."
			return;
		}
		nChatNum = nEmpty;
		if(pFort->m_bHaveGuild)
		{
			if(pFort->m_iGuildSid > 0) nChatNum = nHave;
		}
		EditHyperFortressText(pFort, nChatNum);
	}
	else											// ¾Æ´Ï¸é ÇÊµå¿¡ ¼ÓÇÑ »óÁ¡
	{
		pStore = GetStore(nStore);
		if(pStore == NULL) return;

		if(pStore->m_lUsed == 1) 
		{
			SendSystemMsg( IDS_USER_CANT_USE_IN_GUILD_WAR, SYSTEM_NORMAL, TO_ME);//"¾üÍÅÕ½ÕùÖÐÎÞ·¨Ê¹ÓÃ."
			return;
		}
		nChatNum = nEmpty;
		if(pStore->m_bHaveGuild)
		{
			if(pStore->m_iGuildSid > 0) nChatNum = nHave;
		}
		EditHyperText(pStore, nChatNum);
	}
}

/////////////////////////////////////////////////////////////////////////
//	ÇØ´ç SAY°¡ »óÁ¡¿¡ ¼ÓÇÑ °¡º¯ÀûÀÎ °ÍÀÎÁö ÆÇ´ÜÇÏ¿© º¯È­½ÃÅ²´Ù.
//
void USER::EditHyperText(CStore *pStore, int nNum)
{
	CString temp;
	int i, j, talknum = -1;
	BYTE tCount = 0;
	CHyper hyperText;
	if(!pStore) return;
	if(nNum < 0) return;
	CBufferEx TempBuf, TempSayBuf; 
	for(i = 0; i < g_arNpcChat.GetSize(); i++)
	{
		if(g_arNpcChat[i]->m_sCid == nNum)
		{
			talknum = i;
			break;
		}
	}
	if(talknum < 0) return;
	hyperText.GetHyperText(g_arNpcChat[talknum]->m_strTalk, g_arNpcChat[talknum]->m_sSize);
	hyperText.load();
	for(i = 0; i < hyperText.m_nCountLine; i++)
	{
		for(j = 0; j < hyperText.m_HyperText[i].m_strText.GetLength()-7; j++){

			temp = hyperText.m_HyperText[i].m_strText.Mid(j, 2);
			if(temp == "%S")
			{
				if("GM" == hyperText.m_HyperText[i].m_strText.Mid(j+2, 2))		// ±æµå ¸¶½ºÅÍÀÌ¸§À» ³Ö´Â´Ù.
				{	
					tCount++;
					TempSayBuf.AddString(pStore->m_strMasterName);
//					hyperText.m_HyperText[i].m_strText.Replace("%SGM%", pStore->m_strMasterName);
				}
				else if("GN" == hyperText.m_HyperText[i].m_strText.Mid(j+2, 2))	// ±æµå ÀÌ¸§À» ³Ö´Â´Ù.
				{
//					hyperText.m_HyperText[i].m_strText.Replace("%SGN%", pStore->m_strGuildName);
					tCount++;
					TempSayBuf.AddString(pStore->m_strGuildName);
				}
				else if("GL" == hyperText.m_HyperText[i].m_strText.Mid(j+2, 2))	// 
				{
					tCount++;
					temp.Format("%d", pStore->m_wPlanWarTime.wMonth);
//					hyperText.m_HyperText[i].m_strText.Replace("%SGL%", temp);
					TempSayBuf.AddString((LPTSTR)(LPCTSTR)temp);
				}
				else if("GD" == hyperText.m_HyperText[i].m_strText.Mid(j+2, 2))	// 
				{
					tCount++;
					temp.Format("%d", pStore->m_wPlanWarTime.wDay);
//					hyperText.m_HyperText[i].m_strText.Replace("%SGD%", temp);
					TempSayBuf.AddString((LPTSTR)(LPCTSTR)temp);
				}
				else if("GT" == hyperText.m_HyperText[i].m_strText.Mid(j+2, 2))	// 
				{
					tCount++;
					temp.Format("%d", pStore->m_wPlanWarTime.wHour);
//					hyperText.m_HyperText[i].m_strText.Replace("%SGT%", temp);
					TempSayBuf.AddString((LPTSTR)(LPCTSTR)temp);
				}
/*				else if("STM" == hyperText.m_HyperText[i].m_strText.Mid(j+2, 2))	// 
				{
					if(iGHWarNum < 0 || iGHWarNum >= g_arGuildHouseWar.GetSize()) return;
					
					nTime = (int)g_arGuildHouseWar[iGHWarNum]->m_CurrentGuild.dwTimer/1000;
					temp.Format("%d", nTime);
					hyperText.m_HyperText[i].m_strText.Replace("%STM%", temp);
				}
*/			}		
		}
		
	}

	TempBuf.Add(CLIENT_EVENT_SAY);
	TempBuf.Add((BYTE)SUCCESS);
	TempBuf.Add((short)nNum);
	TempBuf.Add(tCount);
	TempBuf.AddData(TempSayBuf, TempSayBuf.GetLength());
	Send(TempBuf, TempBuf.GetLength());
}

/////////////////////////////////////////////////////////////////////////
//	ÇØ´ç SAY°¡ °ø¼ºÀü¿¡ ¼ÓÇÑ °¡º¯ÀûÀÎ °ÍÀÎÁö ÆÇ´ÜÇÏ¿© º¯È­½ÃÅ²´Ù.
//
void USER::EditHyperFortressText(CGuildFortress *pFort, int nNum)
{
	CString temp;
	if(pFort == NULL) return;//MarkJay ÐÞ¸Ä
	int i, j, talknum = -1;
	BYTE tCount = 0;
	CHyper hyperText;
	if(!pFort) return;
	if(nNum < 0) return;
	CBufferEx TempBuf, TempSayBuf; 
	for(i = 0; i < g_arNpcChat.GetSize(); i++)
	{
		if(g_arNpcChat[i]->m_sCid == nNum)
		{
			talknum = i;
			break;
		}
	}
	if(talknum < 0) return;
	hyperText.GetHyperText(g_arNpcChat[talknum]->m_strTalk, g_arNpcChat[talknum]->m_sSize);
	hyperText.load();
	for(i = 0; i < hyperText.m_nCountLine; i++)
	{
		for(j = 0; j < hyperText.m_HyperText[i].m_strText.GetLength()-7; j++){

			temp = hyperText.m_HyperText[i].m_strText.Mid(j, 2);
			if(temp == "%S")//ÒªÈûTXT
			{
				//¾üÍÅÃû
				if("GN" == hyperText.m_HyperText[i].m_strText.Mid(j+2, 2))		// ±æµå ¸¶½ºÅÍÀÌ¸§À» ³Ö´Â´Ù.
				{	
					tCount++;
					TempSayBuf.AddString(pFort->m_strGuildName);
				}
				//¾üÍÅ³¤
				else if("GM" == hyperText.m_HyperText[i].m_strText.Mid(j+2, 2))	// ±æµå ÀÌ¸§À» ³Ö´Â´Ù.
				{
					tCount++;
					TempSayBuf.AddString(pFort->m_strMasterName);
				}
				//ÔÂ
				else if("GL" == hyperText.m_HyperText[i].m_strText.Mid(j+2, 2))	// 
				{
					tCount++;
					temp.Format("%d", pFort->m_wPlanWarTime.wMonth);
					TempSayBuf.AddString((LPTSTR)(LPCTSTR)temp);
				}
				//ÈÕ
				else if("GD" == hyperText.m_HyperText[i].m_strText.Mid(j+2, 2))	// 
				{
					tCount++;
					temp.Format("%d", pFort->m_wPlanWarTime.wDay);
					TempSayBuf.AddString((LPTSTR)(LPCTSTR)temp);
				}
				//Ð¡Ê±
				else if("GT" == hyperText.m_HyperText[i].m_strText.Mid(j+2, 2))	// 
				{
					tCount++;
					temp.Format("%d", pFort->m_wPlanWarTime.wHour);
					TempSayBuf.AddString((LPTSTR)(LPCTSTR)temp);
				}
				else if("GG" == hyperText.m_HyperText[i].m_strText.Mid(j+2, 2))	// 
				{
					tCount++;
					temp.Format("%d", pFort->m_dwRepairCost);
					TempSayBuf.AddString((LPTSTR)(LPCTSTR)temp);
				}
/*				else if("STM" == hyperText.m_HyperText[i].m_strText.Mid(j+2, 2))	// 
				{
					if(iGHWarNum < 0 || iGHWarNum >= g_arGuildHouseWar.GetSize()) return;
					
					nTime = (int)g_arGuildHouseWar[iGHWarNum]->m_CurrentGuild.dwTimer/1000;
					temp.Format("%d", nTime);
					hyperText.m_HyperText[i].m_strText.Replace("%STM%", temp);
				}
*/			}		
		}
	}
	TempBuf.Add(CLIENT_EVENT_SAY);
	TempBuf.Add((BYTE)SUCCESS);
	TempBuf.Add((short)nNum);
	TempBuf.Add(tCount);
	TempBuf.AddData(TempSayBuf, TempSayBuf.GetLength());
	Send(TempBuf, TempBuf.GetLength());
}

/////////////////////////////////////////////////////////////////////////
//	»çÀÌ¿À´Ð »óÁ¡ÀÇ ¸ñ·ÏÀ» º¸³½´Ù.
//
void USER::SendPsiStoreOpen(int iStoreID)
{
	CStore* pStore = NULL;
	pStore = GetPsiStore(iStoreID);
	if(pStore == NULL) return;
	CBufferEx TempBuf;
	short	sSid[TOTAL_PSI_NUM];// ÀÓ½Ã·Î ÇØ´ç »çÀÌ¿À´ÐÀ» ÀúÀåÇÑ´Ù.
	memset(sSid, -1, sizeof(sSid));
	int i, j;
	int iStart, iEnd;
	int iClass = -1;
	BOOL bflag;	
	short sCount = 0;
	CWordArray arPsi;			
	CByteArray arChangeClass;
	arPsi.RemoveAll();
	arChangeClass.RemoveAll();
	arChangeClass.Add(m_byClass);
	arChangeClass.Add( NOCLASS ); // ¹«°è¿­ »çÀÌ¿À´ÐÀ» ¹è¿ï ¼ö ÀÖ°Ô ÇÑ´Ù
	for(i = 0; i < _CHANGE_DB; i++)// ÇöÀç, ÀüÁ÷ Å¬·¡½º¸¦ ÀúÀåÇÑ´Ù.
	{
		if(m_sChangeClass[i] > 0 && i != m_byClass) arChangeClass.Add(i);
	}
	iStart = arChangeClass.GetSize();
	for(i = 0; i < arChangeClass.GetSize(); i++)
	{
		iStart = pStore->IsCanBuyPsi(pStore, (int)arChangeClass[i]);

		if(iStart >= 0)			// »óÁ¡¿¡¼­ Ãë±ÞÇÏ´Â ¸ñ·ÏÁß ÀÍÈú¼ö ÀÖ´Â°Í¸¸ ¼±º°
		{
			switch(arChangeClass[i])			
			{
			case BRAWL:			// ÇØ´ç Å¬·¡½º ¶Ç´Â ÀüÁ÷ Å¬·¡½º¿¡ ÇØ´çÇÏ´Â »çÀÌ¿À´Ð ¸ñ·Ï¸¦ ÀúÀå
				iEnd = iStart + BRAWL_PSI_NUM;
				break;
			case STAFF:			// ¸¸¾à¿¡ ÀÌ¹Ì ½ÀµæÇÑ »çÀÌ¿À´ÐÀÌ ÀÖ´ÂÁö °Ë»ç
				iEnd = iStart + STAFF_PSI_NUM;	
				break;
			case EDGED:			// ÀÖ´Ù¸é ÀÌ¹Ì ½ÀµæÇÑ »çÀÌ¿À´ÐÀº Á¦¿Ü
				iEnd = iStart + EDGED_PSI_NUM;	
				break;
			case FIREARMS:
				iEnd = iStart + FIREARMS_PSI_NUM;
				break;
			case NOCLASS:
				iEnd = iStart + NOCLASS_PSI_NUM;
				break;
			}

			for(j = iStart;  j < iEnd; j++) 
			{ 
				sSid[sCount] = (short)j; sCount++;
			}
			
		}
	}
	for(i = 0; i < sCount; i++)	// ÀÍÈú¼ö ÀÖ´Â ¸ñ·Ï¸¸ ¼±º° ÀúÀåÇÑ´Ù.
	{
		bflag = TRUE;
		for(j = 0; j < m_nHavePsiNum; j++) if(m_UserPsi[j].sSid == sSid[i]) { bflag = FALSE; break; }
		if(bflag) arPsi.Add(sSid[i]);
	}
/*
	for(i = 0; i < sCount; i++)	// ÀÍÈú¼ö ÀÖ´Â ¸ñ·Ï¸¸ ¼±º° ÀúÀåÇÑ´Ù.
	{
		bflag = TRUE;
		for(j = 0; j < m_nHavePsiNum; j++) if(m_UserPsi[j].sSid == sSid[i]) { bflag = FALSE; break; }
		if(bflag) arPsi.Add(sSid[i]);
	}
*/
	sCount = arPsi.GetSize();
	if(sCount == 0) 
	{
		arChangeClass.RemoveAll();
		arPsi.RemoveAll();
		SendNpcSay( NULL, 4 );		// say¹øÈ£ 4´Â "´õÀÌ»ó ¹è¿ï¼ö ¾ø´Ù"´Â ¸Þ¼¼ÁöÀÓ
		return;
	}
	TempBuf.Add(BUY_PSY_OPEN_RESULT);
	TempBuf.Add((short)iStoreID);
	TempBuf.Add(sCount);
	for(i = 0; i < sCount; i++) TempBuf.Add(arPsi[i]);
	Send(TempBuf, TempBuf.GetLength());
	arChangeClass.RemoveAll();
	arPsi.RemoveAll();
}

////////////////////////////////////////////////////////////////////////////////////
//	»çÀÌ¿À´Ð¸¦ ±¸ÀÔÇÑ´Ù.
//
void USER::BuyPsi(TCHAR *pBuf)
{
	int i, j;
	int index = 0;
	int iClass = 0;

	BOOL bSame;
	BYTE tError = 1;	
	BYTE tResult = SUCCESS;

	short sStoreID;			// »óÁ¡ ¾ÆÀÌµð
	short sPsiCount;		// ÃÑ Psi ¼ö
	short sSid;

	DWORD dwDN = m_dwDN;
	DWORD dwXP = m_dwXP;

	CBufferEx TempBuf;
	CWordArray	arPsi;
	CByteArray arChangeClass;

	arChangeClass.RemoveAll();
	arPsi.RemoveAll();

	CStore* pStore = NULL;

	sStoreID = GetShort(pBuf, index);

	pStore = GetPsiStore(sStoreID);
	if(pStore == NULL) { tResult = FAIL; goto go_result; }

	sPsiCount = GetShort(pBuf, index);

	if(sPsiCount <= 0 || sPsiCount >= g_arPsiTable.GetSize()) return;

	arChangeClass.Add(m_byClass);
	arChangeClass.Add( NOCLASS );
	for(i = 0; i < _CHANGE_DB; i++)			//	ÀüÁ÷ÇÑ °æ¿ì¿Í Ã³À½ÀÎ °æ¿ì »óÁ¡¿¡¼­ Ãë±ÞÇÑ Å¬·¡½º¿Í °°ÀºÁö 		
	{										//	¾Ë¾Æ¾ßÇÑ´Ù.
		if(m_sChangeClass[i] > 0 && i != m_byClass) arChangeClass.Add(i);
	}
											//	Ã³À½ÀÏ °æ¿ì ÇØ´ç Å¬·¡½º¸¦ Áý¾î³Ö´Â´Ù.
	for(i = 0; i < sPsiCount; i++)
	{
		bSame = FALSE;
		sSid = GetShort(pBuf, index);
		/****************************** ¿¹¿Ü Ã³¸® *****************************/
		if(sSid < 0 || sSid >= g_arPsiTable.GetSize()) { tResult = FAIL; tError = 05; goto go_result; }

											//	ÀÌ¹Ì ½ÀµæÇÑ »çÀÌ¿À´ÐÀº ½ÇÆÐ
		for(j = 0; j < m_nHavePsiNum; j++)
		{
			if(m_UserPsi[j].sSid == sSid) { tResult = FAIL; tError = 06; goto go_result; }
		}

		iClass = CheckPsiClass(sSid);
		if(iClass < 0) { tResult = FAIL; goto go_result; }
											//	½Àµæ ºÒ°¡´ÉÇÑ °è¿­ÀÇ »çÀÌ¿À´Ðµµ ½ÇÆÐ
		for(j = 0;  j < arChangeClass.GetSize(); j++)
		{
			if(arChangeClass[j] == iClass) bSame = TRUE;
		}									//	»óÁ¡ »çÀÌ¿À´ÐÀÌ ÀÚ±â Å¬·¡½º¿Í ¸Â´Â°Ô ¾ø´Ù.
		if(bSame == FALSE) { tResult = FAIL; tError = 01; goto go_result; }
											//	±¸ÀÔ·¹º§ÀÌ ³ô¾Æµµ ½ÇÆÐ
		if(g_arPsiTable[sSid]->m_tNeedLevel > m_sLevel) { tResult = FAIL; tError = 02; goto go_result; }
											//	XP°¡ ¸ðÀÚ¶óµµ ½ÇÆÐ
		if(g_arPsiTable[sSid]->m_iNeedXP > dwXP) { tResult = FAIL; tError = 03; goto go_result; }
											//	DNÀÌ ¸ðÀÚ¶óµµ ½ÇÆÐ
		if(g_arPsiTable[sSid]->m_iNeedDN > dwDN) { tResult = FAIL; tError = 04; goto go_result; }
											//	ÃÖ´ë°ªÀ» ³Ñ¾îµµ ½ÇÆÐ
		if(m_nHavePsiNum >= TOTAL_PSI_NUM) { tResult = FAIL; tError = 05; goto go_result; }
		
		dwDN = dwDN - g_arPsiTable[sSid]->m_iNeedDN;
		dwXP = dwXP - g_arPsiTable[sSid]->m_iNeedXP;
		arPsi.Add(sSid);
	}

	/*************************** User Psi µî·Ï****************************/
	for(i = 0; i < arPsi.GetSize(); i++)
	{
		sSid = arPsi[i];
		m_UserPsi[m_nHavePsiNum].sSid = sSid;
		m_UserPsi[m_nHavePsiNum].tOnOff = FALSE;
		m_nHavePsiNum++;
	}

	m_dwDN = dwDN; m_dwXP = dwXP; 
go_result:
	TempBuf.Add(BUY_PSY_RESULT);
	TempBuf.Add(tResult);

	if(tResult == FAIL) 
	{ 
		TempBuf.Add(tError);
		Send(TempBuf, TempBuf.GetLength()); 
		return; 
	}

	sPsiCount = arPsi.GetSize();

	TempBuf.Add(sPsiCount);

	for(i = 0; i < sPsiCount; i++)
	{
		sSid = (short)arPsi[i];
		TempBuf.Add(sSid);
	}

	SendXP();
	UpdateUserItemDN();
	SendMoneyChanged();
	Send(TempBuf, TempBuf.GetLength());

	arPsi.RemoveAll();
	arChangeClass.RemoveAll();
}

int USER::CheckPsiClass(short sSid)
{
	if(sSid < BRAWL_PSI_NUM) return BRAWL;
	else if(sSid < BRAWL_PSI_NUM + STAFF_PSI_NUM) return STAFF;
	else if(sSid < BRAWL_PSI_NUM + STAFF_PSI_NUM + EDGED_PSI_NUM) return EDGED;
	else if(sSid < BRAWL_PSI_NUM + STAFF_PSI_NUM + EDGED_PSI_NUM + FIREARMS_PSI_NUM) return FIREARMS;
	else if(sSid < TOTAL_PSI_NUM) return NOCLASS;
	else return -1;
}

////////////////////////////////////////////////////////////////////////////////////
//	ÀÎº¥Áß ºó ½½·ÔÀ» Ã£¾Æ¼­ ´ëÀÔ, ¾øÀ¸¸é -1 
//
int USER::GetEmptyInvenSlot(int *pEmpty)
{
	int i, j = 0;

	for(i = EQUIP_ITEM_NUM; i < TOTAL_INVEN_MAX; i++)
	{
		if(m_UserItem[i].sSid == -1) 
		{
			pEmpty[j] = i;
			j++;
		}
	}

	return j;
}

////////////////////////////////////////////////////////////////////////////////////
//	»õ·Î¿î ¾ÆÀÌÅÛµéÀ» ¾òÀ»¶§...(¿¹: ¸Ê¿¡¼­ ¾ÆÀÌÅÛÀ» ÁÖ¿ï¶§, ¸÷¿¡°Ô¼­ ¾òÀ»¶§, »óÁ¡¿¡¼­ »ì¶§)   
//
void USER::UpdateInvenSlot(CWordArray *arEmptySlot,CWordArray *arSameSlot, BYTE tType)
{
	int i, j;
	int iEmpty = 0;
	int iSame = 0;
	CBufferEx TempBuf;

	if(arEmptySlot != NULL) iEmpty = arEmptySlot->GetSize();
	if(arSameSlot != NULL) iSame = arSameSlot->GetSize();

	BYTE bySlot = 0;
	BYTE iCount = (BYTE)(iEmpty + iSame); 

	TempBuf.Add(ITEM_MOVE_RESULT);
	TempBuf.Add(SUCCESS);
	TempBuf.Add(tType);

	TempBuf.Add(iCount);

	for(i = 0; i < iEmpty; i++)
	{
		bySlot = (BYTE)arEmptySlot->GetAt(i);
		TempBuf.Add(bySlot);
		TempBuf.Add(m_UserItem[bySlot].sLevel);
		TempBuf.Add(m_UserItem[bySlot].sSid);
		TempBuf.Add(m_UserItem[bySlot].sDuration);
		TempBuf.Add(m_UserItem[bySlot].sBullNum);
		TempBuf.Add(m_UserItem[bySlot].sCount);

		for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[bySlot].tMagic[j]);

		TempBuf.Add(m_UserItem[bySlot].tIQ); 
	}

	for(i = 0; i < iSame; i++)
	{
		bySlot = (BYTE)arSameSlot->GetAt(i);
		TempBuf.Add(bySlot);
		TempBuf.Add(m_UserItem[bySlot].sLevel);
		TempBuf.Add(m_UserItem[bySlot].sSid);
		TempBuf.Add(m_UserItem[bySlot].sDuration);
		TempBuf.Add(m_UserItem[bySlot].sBullNum);
		TempBuf.Add(m_UserItem[bySlot].sCount);

		for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[bySlot].tMagic[j]);

		TempBuf.Add(m_UserItem[bySlot].tIQ); 
	}
	
	Send(TempBuf, TempBuf.GetLength());
}

////////////////////////////////////////////////////////////////////////////////////
//	¼ÒÁö±ÝÀÌ º¯°æ‰çÀ» °æ¿ì Åëº¸
//
void USER::SendMoneyChanged()
{
	CBufferEx TempBuf;

	TempBuf.Add(MONEY_CHANGED);
	TempBuf.Add((BYTE)0x01);
	TempBuf.Add(m_dwDN);

	Send(TempBuf, TempBuf.GetLength());
}

//////////////////////////////////////////////////////////////////////////////////
//	»óÁ¡¿¡¼­ ¾ÆÀÌÅÛÀ» ±¸ÀÔÇÑ´Ù.
//
void USER::BuyItem(TCHAR *pBuf)
{
#ifdef _ACTIVE_USER
//	if(m_iDisplayType == 6 && m_sLevel > 25) return; //yskang 0.5
	if( m_iDisplayType == 6) return; //yskang 0.5
#endif

	int rank = m_sCityRank + CITY_RANK_INTERVAL;

	if(rank == CHAONISE_RANK && IsCity()) 
	{
		SendSystemMsg(IDS_USER_CANT_USE_YOUR_CITY_RANK, SYSTEM_NORMAL, TO_ME);//"ÊÐÃñµÈ¼¶Ì«µÍ."
		return;
	}

	CStore* pStore = NULL;
	ItemListArray	arEmptyBuy, arSameBuy;
	CWordArray		arEmptySlot, arSameSlot;
	ItemList		MyItem[TOTAL_ITEM_NUM], SameItem[TOTAL_ITEM_NUM];
	int				pTotalEmpty[INVENTORY_NUM];

	int index = 0;
	int i, j, k; 
	short sid = -1, num = -1; 
	int iSlot, iEmptyNum = 0;
	int iWeight = 0;
	DWORD TotalCost = 0;
	DWORD CheckMoney = 0;

	short sStoreID = GetShort(pBuf, index);
	short sCount = GetShort(pBuf, index);

	if(sStoreID < 0 || sCount <= 0 || sCount >= _MAX_SHORT) return;

	pStore = GetStore(sStoreID);
	if(pStore == NULL) return;

	CPoint pt = ConvertToClient(m_curx, m_cury);
	if(!g_pEventBlock->CheckUserEvent(m_curz, pt.x, pt.y, BUY_BLOCK)) return;
	if(sCount > 255) return;				// ÇÑ¹ø¿¡ »ì¼öÀÖ´Â ÃÖ´ë·®¸¦ °­Á¦·Î Á¤ÇÑ´Ù.(MAX_BYTE)
	for(i = 0; i < TOTAL_ITEM_NUM; i++)		// ¾ÆÀÌÅÛ Á¤º¸ ¹é¾÷
	{
		MyItem[i] = m_UserItem[i];
		SameItem[i] = m_UserItem[i];
	}
	// º¯¼ö ÃÊ±âÈ­ ------------------------------------------------//
	BOOL bFlag = FALSE;
	DWORD dwCost = 0;
	CString strMsg = "";
	DWORD dwHuiLv = 0;
	short sRate = pStore->m_sRate;
	DWORD dwTemp = 0;
	if(sStoreID == 50 || sStoreID == 51 || sStoreID == 52)
	dwTemp = FindItem(SPECIAL_BIAOZHI);
	else dwTemp = m_dwDN;
	DWORD dwTax = 0, dwBasicTax = 0;
	arSameSlot.RemoveAll();
	arSameBuy.RemoveAll();
	arEmptyBuy.RemoveAll();
	arEmptySlot.RemoveAll();
	ItemList	*pBuyItem = NULL;
	::ZeroMemory(pTotalEmpty, sizeof(pTotalEmpty));
	GetEmptyInvenSlot(pTotalEmpty);
	int iItemSize = g_arItemTable.GetSize();
	// ±¸ÀÔ¸ñ·Ï ---------------------------------------------------//
	for(i = 0; i < sCount; i++)
	{
		iSlot = -1;
		sid = GetShort(pBuf, index);
		num = GetShort(pBuf, index);

		if(num > 100 && (sStoreID == 50 || sStoreID == 51 || sStoreID == 52))
		{
			SendSystemMsg( "¶Ô²»Æð,Ò»´Î×î¶àÖ»ÄÜ¹ºÂò100¸öÎïÆ·.", SYSTEM_ERROR, TO_ME);
			goto go_result;
		}

		if(num > 10 && (sid == 702 || sid == 699 || sid == 700))
		{
			SendSystemMsg( "ÌØÊâÎïÆ·,Ò»´Î×î¶àÖ»ÄÜ¹ºÂò10¸ö.", SYSTEM_ERROR, TO_ME);
			goto go_result;
		}

		if(iEmptyNum > INVENTORY_NUM) { bFlag = TRUE; goto go_result; }
		if(sid < 0 || num <= 0 || sid >= iItemSize || num >= _MAX_SHORT) { bFlag = TRUE; goto go_result; }

		if(!pStore->IsCanBuy(sid)) { bFlag = TRUE; goto go_result; }
//		if(g_arItemTable[sid]->m_byWear <= 5 || g_arItemTable[sid]->m_byWear == 15)	// º¹±Ç ¾ÆÀÌÅÛ
		if(g_arItemTable[sid]->m_sDuration > 0)	// º¹±Ç ¾ÆÀÌÅÛ
		{
			for(j = 0; j < num; j++)
			{
				iSlot = pTotalEmpty[iEmptyNum];
				if(iSlot == 0) { bFlag = TRUE; goto go_result; }

				pBuyItem = NULL;
				pBuyItem = new ItemList;
				if(pBuyItem == NULL) { bFlag = TRUE; goto go_result; }

				ReSetItemSlot(pBuyItem);

				pBuyItem->sLevel = g_arItemTable[sid]->m_byRLevel;
				pBuyItem->sSid = sid;
				pBuyItem->sCount = 1;
				pBuyItem->sDuration = g_arItemTable[sid]->m_sDuration;
				pBuyItem->sBullNum = g_arItemTable[sid]->m_sBullNum;
				for(k =0; k< MAGIC_NUM; k++) pBuyItem->tMagic[k] = 0;
				pBuyItem->tIQ = 0;
				pBuyItem->iItemSerial = 0;

				arEmptyBuy.Add(pBuyItem);
				arEmptySlot.Add(iSlot);
				iEmptyNum++; pBuyItem = NULL;

				if(iEmptyNum > INVENTORY_NUM) { bFlag = TRUE; goto go_result; }
			}
		}
		else 
		{
//			if(num > MAX_ITEM_NUM) num = MAX_ITEM_NUM;
			pBuyItem = NULL;
			pBuyItem = new ItemList;
			if(pBuyItem == NULL) { bFlag = TRUE; goto go_result; }

			ReSetItemSlot(pBuyItem);

			pBuyItem->sLevel = g_arItemTable[sid]->m_byRLevel;
			pBuyItem->sSid = sid;
			pBuyItem->sCount = num;
			pBuyItem->sDuration = g_arItemTable[sid]->m_sDuration;
			pBuyItem->sBullNum = g_arItemTable[sid]->m_sBullNum;
			for(k =0; k< MAGIC_NUM; k++) pBuyItem->tMagic[k] = 0;
			pBuyItem->tIQ = 0;
			pBuyItem->iItemSerial = 0;

			iSlot = GetSameItem(*pBuyItem, INVENTORY_SLOT);
			if(iSlot != -1)	
			{ 
/*				if(SameItem[iSlot].sCount + num > MAX_ITEM_NUM)
				{
					num = MAX_ITEM_NUM - SameItem[iSlot].sCount;
					SameItem[iSlot].sCount = MAX_ITEM_NUM;
					if(num < 0) num = 0;
				}
*/				if(num != 0)
				{
					pBuyItem->sCount = num;
					arSameBuy.Add(pBuyItem); 
					arSameSlot.Add(iSlot); 
				}
			}
			else			
			{
				iSlot = pTotalEmpty[iEmptyNum];
				if(iSlot == 0) { bFlag = TRUE; goto go_result; }
				arEmptyBuy.Add(pBuyItem); 
				arEmptySlot.Add(iSlot); 
				iEmptyNum++;
			}			
		}

		iWeight += g_arItemTable[sid]->m_byWeight * num;

		if(pStore->m_arNpcList.GetSize() > 0)
		{
			int iCost = (int)(g_arItemTable[sid]->m_iDN * 0.9);//ÊÐÃñµÈ¼¶ÓÅ»Ý

			dwCost += num * iCost;
		}
		else if(pStore->m_sStoreID == 50 || pStore->m_sStoreID == 51 || pStore->m_sStoreID == 52)
		{
			int iCost = (int)(g_arItemTable[sid]->m_iDN); //ÊÐÃñµÈ¼¶ÓÅ»Ý
			dwCost += iCost * num;
		}
		else dwCost += num * GetItemCostByCityRank(sid, 0);//sRate);
	}

	// ±¸ÀÔºñ¿ë °è»ê ¹× ¼ÒÁö±Ý °Ë»ç -------------------------------//
	dwTax = (DWORD)( dwCost * ((double)sRate/100) );
	dwCost = (DWORD)(dwCost + dwTax + 0.999);

	if(m_dwVIPTime > 0)//VIP»áÔ±Âò¶«Î÷ÓÅ»Ý10%
	{
		dwHuiLv = (DWORD)(dwCost * 0.1);
		dwCost = dwCost - dwHuiLv;
		strMsg.Format("ÄúÊÇVIP»áÔ±,¹ºÂòÓÅ»Ý %d",dwHuiLv);
		SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ERROR, TO_ME);		
	}

	//------------------------ÐÞ¸´ÓÎÏ·±ÒÉÌµêÔì³É½ðÇ®ÀÄ³öÎÊÌâ------------------------
	int price;
	price = (DWORD)dwCost;
	CheckMoney += ((price/100000000) * num);
	if(CheckMoney > 20)
	{
		SYSTEMTIME st;
		CString strDate;
		GetLocalTime(&st);
		strDate.Format("%d-%d-%d %d:%d",st.wYear,st.wMonth,st.wDay ,st.wHour,st.wMinute);
		CString str = _T("");
		str.Format("[%s] %s ³¢ÊÔÓÎÏ·±ÒÀÄ³ö Price = %d\r\n",strDate,m_strUserID,price);
		EnterCriticalSection( &m_CS_FileWrite );
		g_fpMarkDeBug.Write( str, str.GetLength() );
		LeaveCriticalSection( &m_CS_FileWrite);
		goto go_result;
	}
	if(dwCost < 0)
	{
		goto go_result;
	}
	//------------------------------------------------------------------------------
	if(sStoreID == 50 || sStoreID == 51 || sStoreID == 52)
	{
		int shuliang = dwCost;
		if(dwCost > dwTemp || dwTemp <= 0 || FindItem(724) < shuliang)//ÐÞ¸´ÏûºÄÆ·ÉÌµêÖØ¸´¹ºÂò MarkJay
		{
			SendSystemMsg( "¶Ô²»Æð,ÄúµÄ±êÖ¾²»×ã.", SYSTEM_ERROR, TO_ME);
			goto go_result;
		}
	}
	else
	{
		if(dwCost > dwTemp)		
		{
			SendSystemMsg( IDS_USER_NOT_ENOUGH_DINA1, SYSTEM_ERROR, TO_ME);// "ÄãÃ»ÓÐ×ã¹»µÄ½ðÇ®(s)."
			goto go_result;
		}
	}

	if(m_iMaxWeight < m_iCurWeight + iWeight)
	{
		SendSystemMsg( IDS_USER_OVER_WEIGHT1, SYSTEM_ERROR, TO_ME);// "ÄãÒÑ¾­´ïµ½×Ô¼º×î´ó¸ºÖØÁ¿."
		goto go_result;
	}

	// ±¸ÀÔ¸ñ·ÏÀ» ÀÎº¥¿¡ -------------------- --------------------//
	for(i = 0; i < arSameBuy.GetSize(); i++)
	{
		CheckMaxValue((short &)m_UserItem[arSameSlot[i]].sCount, (short)arSameBuy[i]->sCount); 
	}

	for(i = 0; i < arEmptyBuy.GetSize(); i++)
	{
		m_UserItem[arEmptySlot[i]].sLevel = arEmptyBuy[i]->sLevel;
		m_UserItem[arEmptySlot[i]].sSid = arEmptyBuy[i]->sSid;
		m_UserItem[arEmptySlot[i]].sCount = arEmptyBuy[i]->sCount;
		m_UserItem[arEmptySlot[i]].sBullNum = arEmptyBuy[i]->sBullNum;
		m_UserItem[arEmptySlot[i]].sDuration = arEmptyBuy[i]->sDuration;
		m_UserItem[arEmptySlot[i]].tIQ = arEmptyBuy[i]->tIQ;
		for(j =0; j < MAGIC_NUM; j++) m_UserItem[arEmptySlot[i]].tMagic[j] = arEmptyBuy[i]->tMagic[j];
		m_UserItem[arEmptySlot[i]].iItemSerial = arEmptyBuy[i]->iItemSerial;

		MakeItemLog( &(m_UserItem[arEmptySlot[i]]), ITEMLOG_BUY );
	}

	if(sStoreID == 50 || sStoreID == 51 || sStoreID == 52)
	{
		RobItem(SPECIAL_BIAOZHI,dwCost);
	}
	else
	{
		if(dwTemp <= dwCost) m_dwDN = 0;
		else m_dwDN = dwTemp - dwCost;	
	}

	MakeMoneyLog( dwCost, ITEMLOG_BUY_MONEY );

	if(UpdateUserItemDN() == FALSE)
	{
		m_dwDN = dwTemp;						// ¼ÒÁö±Ý È¯¿ø
		for(i = 0; i < TOTAL_ITEM_NUM; i++)		// ¾ÆÀÌÅÛ Á¤º¸ È¯¿ø
		{
			m_UserItem[i] = MyItem[i];
		}
			SendSystemMsg( IDS_USER_BUY_FAIL, SYSTEM_ERROR, TO_ME);//"Ã»ÓÐ¹ºÂòÎïÆ·(s)."
		FlushItemLog( FALSE );
		goto go_result;
	}

	if(dwTax < dwCost && pStore->m_bHaveGuild) 
	{
		pStore->m_iGuildDN += dwTax;	// ÇÊµå »óÁ¡ÀÌ¸é ±æµåÃ¢°í Àû¸³
		UpdateMemStoreDN(sStoreID, pStore->m_iGuildDN);
	}
	else
	{
		UpdateFortressTax(sStoreID, dwTax); // Æ÷Æ®¸®½º ¼¼±ÝÀÌ¸é ÀúÀå...
	}

	m_iCurWeight += iWeight;
	GetRecoverySpeed();

	UpdateInvenSlot(&arEmptySlot, &arSameSlot);
	SendMoneyChanged();
//	SendItemWeightChange();				// ÇöÀç ¾ÆÀÌÅÛ ¹«°Ô¸¦ º¸³½´Ù.
	FlushItemLog( TRUE );

go_result:
	if(bFlag) SendSystemMsg( IDS_USER_FULL_INVEN, SYSTEM_ERROR, TO_ME);// "ÄãµÄ°ü¹ü¿Õ¼äÒÑÂú."

	// ¸Þ¸ð¸® ÇØÁ¦ -------------------- --------------------//
	for(i = 0; i < arSameBuy.GetSize(); i++)
	{
		if(arSameBuy[i] != NULL) delete arSameBuy[i];
	}
	arSameBuy.RemoveAll();
	arSameSlot.RemoveAll();
	for(i = 0; i < arEmptyBuy.GetSize(); i++)
	{
		if(arEmptyBuy[i] != NULL) delete arEmptyBuy[i];
	}
	arEmptyBuy.RemoveAll();
	arEmptySlot.RemoveAll();
}

////////////////////////////////////////////////////////////////////////////////////
//	Æ÷Æ®¸®½º¿¡ ¼Ò¼ÓµÈ »óÁ¡ÀÌ¸é ¼¼±ÝÀ» °è»êÇØ¼­ Àû¸³ 
//
void USER::UpdateFortressTax(int iStoreID, DWORD dwCost)
{
	int i, j;

	for(i = 0; i < g_arGuildFortress.GetSize(); i++)
	{
		if(!g_arGuildFortress[i]) continue;

		for(j = 0; j < g_arGuildFortress[i]->arStoreTax.GetSize(); j++)
		{
			if(g_arGuildFortress[i]->arStoreTax[j] == iStoreID)
			{
				g_arGuildFortress[i]->m_iGuildDN += dwCost; 
//				InterlockedExchange(&g_arGuildFortress[i]->m_iGuildDN, (long)dwCost);
				UpdateMemStoreDN(g_arGuildFortress[i]->m_sFortressID, g_arGuildFortress[i]->m_iGuildDN);
				break;
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////
// ´ò¿ªÂôÎïÆ·´°¿Ú
void USER::SellItemOpenReq(int sStoreID)
{
#ifdef _ACTIVE_USER
//	if(m_iDisplayType == 6 && m_sLevel > 25) return; //yskang 0.5
	if(m_iDisplayType == 6) return; //yskang 0.5
#endif

	int rank = m_sCityRank + CITY_RANK_INTERVAL;

	if(rank == CHAONISE_RANK && IsCity()) 
	{
		SendSystemMsg(IDS_USER_CANT_USE_YOUR_CITY_RANK, SYSTEM_NORMAL, TO_ME);//"ÊÐÃñµÈ¼¶Ì«µÍ."
		return;
	}

	int i;
	int index = 0;
	int sipai = 0;
	int baigao = 0;
	int magic1 = 0;
	int dynamic1 = 0;
	int magic2 = 0;
	int dynamic2 = 0;
	int magic3 = 0;
	int dynamic3 = 0;
	int magic4 = 0;
	int dynamic4 = 0;
	int iStart = EQUIP_ITEM_NUM;
	int iEnd = iStart + INVENTORY_NUM;

	CStore* pStore = GetStore(sStoreID);
	if(pStore == NULL) return;

	CBufferEx CostBuf;
	short sSlot = 0;
	DWORD dwSellCost = 0;
	short sCount = 0;
	if(sStoreID == 30)//ËÄÅÅºÍ°×ï¯×Ô¶¯»ØÊÕ
	{
		for(sSlot = 10; sSlot < 34; sSlot++)
		{
			magic1 = m_UserItem[sSlot].tMagic[0];
			if(magic1 < 0 || magic1 >= g_arMagicItemTable.GetSize()) continue;
			dynamic1 = g_arMagicItemTable[magic1]->m_sSubType;
			magic2 = m_UserItem[sSlot].tMagic[1];
			if(magic2 < 0 || magic2 >= g_arMagicItemTable.GetSize()) continue;
			dynamic2 = g_arMagicItemTable[magic2]->m_sSubType;
			magic3 = m_UserItem[sSlot].tMagic[2];
			if(magic3 < 0 || magic3 >= g_arMagicItemTable.GetSize()) continue;
			dynamic3 = g_arMagicItemTable[magic3]->m_sSubType;
			magic4 = m_UserItem[sSlot].tMagic[3];
			if(magic4 < 0 || magic4 >= g_arMagicItemTable.GetSize()) continue;
			dynamic4 = g_arMagicItemTable[magic4]->m_sSubType;

			if(m_UserItem[sSlot].sDuration == 1 && m_UserItem[sSlot].sSid != 907) continue;//ÐÞ¸´»ØÊÕ»úÐµ MarkJay
			if(m_UserItem[sSlot].tMagic[0] != NULL && m_UserItem[sSlot].tMagic[1] != NULL && m_UserItem[sSlot].tMagic[2] != NULL && m_UserItem[sSlot].tMagic[3] != NULL && m_UserItem[sSlot].tMagic[4] == NULL && dynamic1 == dynamic2 && dynamic2 == dynamic3 && dynamic3 == dynamic4)
			{
				dwSellCost = f_Markini[0]->JG_SIPAIJ;
			}
			else if(m_UserItem[sSlot].tMagic[5] == 0 && m_UserItem[sSlot].tIQ == 2 && m_UserItem[sSlot].tMagic[0] == NULL && m_UserItem[sSlot].tMagic[1] == NULL && m_UserItem[sSlot].tMagic[2] == NULL && m_UserItem[sSlot].tMagic[3] == NULL && m_UserItem[sSlot].tMagic[4] == NULL && m_UserItem[sSlot].sSid == 907)
			{
				dwSellCost = f_Markini[0]->JG_BAIGAO;
			}
			else 
			{
				continue;
			}
			if(dwSellCost > 0)
			{
				CostBuf.Add(sSlot);
				CostBuf.Add((short)m_UserItem[sSlot].sDuration);
				CostBuf.Add(dwSellCost);
				sCount++;
			}
		}
	}
	else
	{
		for(i = iStart; i < iEnd; i++)
		{
			if(pStore->IsCanSell(m_UserItem[i].sSid))
			{
				sSlot = (short)i;
				if((m_UserItem[sSlot].tMagic[0] != NULL && m_UserItem[sSlot].tMagic[1] != NULL && m_UserItem[sSlot].tMagic[2] != NULL && m_UserItem[sSlot].tMagic[3] != NULL && m_UserItem[sSlot].tMagic[0] == m_UserItem[sSlot].tMagic[1] && m_UserItem[sSlot].tMagic[1] == m_UserItem[sSlot].tMagic[2] && m_UserItem[sSlot].tMagic[2] == m_UserItem[sSlot].tMagic[3] && m_UserItem[sSlot].tMagic[4] == NULL) || (m_UserItem[sSlot].tMagic[5] == 0 && m_UserItem[sSlot].tIQ == 2 && m_UserItem[sSlot].tMagic[0] == NULL && m_UserItem[sSlot].tMagic[1] == NULL && m_UserItem[sSlot].tMagic[2] == NULL && m_UserItem[sSlot].tMagic[3] == NULL && m_UserItem[sSlot].tMagic[4] == NULL && m_UserItem[sSlot].sSid == 907)) continue;
				dwSellCost = GetSellCost(sSlot);
				if(dwSellCost > 0)
				{
					CostBuf.Add(sSlot);
					CostBuf.Add((short)m_UserItem[sSlot].sDuration);
					CostBuf.Add(dwSellCost);
					sCount++;
				}
			}
		}
	}
	CBufferEx TempBuf;
	TempBuf.Add(SELL_ITEM_OPEN_RESULT);
	TempBuf.Add((short)sStoreID);
	TempBuf.Add(sCount);
	if(sCount > 0) TempBuf.AddData(CostBuf, CostBuf.GetLength());
	Send(TempBuf, TempBuf.GetLength());
}

//////////////////////////////////////////////////////////////////////////////////
//	»óÁ¡¿¡ ÆÈ °¡°ÝÀ» ±¸ÇÑ´Ù.
//
DWORD USER::GetSellCost(short sSlot)
{
	short sSid = m_UserItem[sSlot].sSid;

	if(sSid < 0 || sSid >= g_arItemTable.GetSize()) return (DWORD)0;

	DWORD	dwDN = g_arItemTable[sSid]->m_iDN;
	BYTE    tClass = g_arItemTable[sSid]->m_byClass;
	BYTE	tIQ = m_UserItem[sSlot].tIQ;
	int		iMagic = 0;
	int		i = 0;

	// ¸ÅÁ÷ ¶Ç´Â ·¹¾î ¾ÆÀÌÅÛÀÇ °æ¿ì
	if(tIQ == MAGIC_ITEM)
	{
		for(i = 0; i < MAGIC_NUM; i++)
		{
			if(m_UserItem[sSlot].tMagic[i] != 0) iMagic++;
		}
		//if(tWear != 2) dwDN = (DWORD)(dwDN * (1.2 + (double)(iMagic * 0.1)));//³ýÁËÐ¬Ã± ±ðµÄ¼Û¸ñ¶¼Ê£ÓÚ3 MarkJay
		dwDN = (DWORD)(dwDN * (1.2 + (double)(iMagic * 0.07)));
	}
	if(tIQ == RARE_ITEM)
	{
		for(i = 0; i < MAGIC_NUM; i++)
		{
			if(m_UserItem[sSlot].tMagic[i] != 0) iMagic++;
		}
		//if(tWear != 2) dwDN = (DWORD)(dwDN * (1.6 + (double)(iMagic * 0.1)));//³ýÁËÐ¬Ã± ±ðµÄ¼Û¸ñ¶¼Ê£ÓÚ3 MarkJay
		dwDN = (DWORD)(dwDN * (1.6 + (double)(iMagic * 0.07)));
	}
	if(m_UserItem[sSlot].tMagic[5] != 0 || m_UserItem[sSlot].sSid == 632 || m_UserItem[sSlot].sSid == 1008) return (DWORD)0;//¸Ä¹ýµÄÎïÆ·Óë´«ËÍÃÅ²»ÏÔÊ¾
	if(m_UserItem[sSlot].sSid < 0 || m_UserItem[sSlot].sSid >= g_arItemTable.GetSize()) return (DWORD)0;//MarkJay ÐÞ¸Ä

	short sMaxDuration = g_arItemTable[sSid]->m_sDuration;
	short sCurDuration = m_UserItem[sSlot].sDuration;

	if(sMaxDuration <= 0) { sMaxDuration = sCurDuration = 1; }

	//----------------------------------------------------------------------------------------------------------------
	double duTempCost =0.0;
	if(m_iDisplayType == 5 || m_iDisplayType == 6)
		duTempCost = dwDN * (0.1 - 0.01 * (( 1 - ((double)sCurDuration / (double)sMaxDuration) ) / 0.1)) + 1;
	else
		duTempCost = dwDN * (0.15 - 0.01 * (( 1 - ((double)sCurDuration / (double)sMaxDuration) ) / 0.1)) + 1;
	//----------------------------------------------------------------------------------------------------------------

	if(tClass == FIREARMS && (tIQ - 100) >= 0) duTempCost = duTempCost/2;
	return (DWORD)(duTempCost);
}

//##################################################################################
//	Main UI
//##################################################################################

//////////////////////////////////////////////////////////////////////////////
//	Slot1 °ú Slot2ÀÇ ¾ÆÀÌÅÛÀ» ¹Ù²Û´Ù.
//
void USER::SwapItem(int nSlot1, int nSlot2)
{
	ItemList temp;

	temp = m_UserItem[nSlot1];
	m_UserItem[nSlot1] = m_UserItem[nSlot2];
	m_UserItem[nSlot2] = temp;
}

///////////////////////////////////////////////////////////////////////////////
// ¸Ä±äÎäÆ÷ÇëÇó
void USER::ChangeWeaponReq(TCHAR *pBuf)
{
	int j;
	BYTE result = FAIL;
	BYTE tEnable = FALSE;
	BYTE tWeaponClass = 0;

	if(m_UserItem[LEFT_HAND].sSid == -1 && m_UserItem[RIGHT_HAND].sSid == -1) goto send_result;

	if(m_UserItem[LEFT_HAND].sSid == 1121) goto send_result;//¸±ÊÖ²»ÄÜTABÇÐ»»ÎäÆ÷ Ð¡ºÚ¸±ÊÖ

	SwapItem(RIGHT_HAND, LEFT_HAND);

	if(IsCanUseWeaponSkill(tWeaponClass) == TRUE)
	{
		tEnable = TRUE;
	}
	
	result = SUCCESS;

send_result:

	CBufferEx TempBuf, TempBuf2;
	TempBuf.Add(CHANGE_WEAPON_RESULT);
	TempBuf.Add(result);

	if(result == SUCCESS)
	{
		TempBuf.Add(tEnable);
		
		if(tWeaponClass == 255) tWeaponClass = m_byClass;

		TempBuf.Add(tWeaponClass);

		for(int i = tWeaponClass * SKILL_NUM; i < tWeaponClass * SKILL_NUM + SKILL_NUM; i++)
		{
			TempBuf.Add(m_UserSkill[i].sSid);
			TempBuf.Add(m_UserSkill[i].tLevel);
		}
		TempBuf2.Add(ITEM_MOVE_RESULT);
		TempBuf2.Add((BYTE)0x01);				// ¼º°ø
		TempBuf2.Add((BYTE)0x00);				// ÀÏ¹Ý ¾ÆÀÌÅÛ
		TempBuf2.Add((BYTE)0x02);				// º¯È­µÈ ½½·ÔÀÇ ¼ö = 2

		TempBuf2.Add((BYTE)LEFT_HAND);
		TempBuf2.Add(m_UserItem[LEFT_HAND].sLevel);
		TempBuf2.Add(m_UserItem[LEFT_HAND].sSid);
		TempBuf2.Add(m_UserItem[LEFT_HAND].sDuration);
		TempBuf2.Add(m_UserItem[LEFT_HAND].sBullNum);
		TempBuf2.Add(m_UserItem[LEFT_HAND].sCount);
		for(j =0; j < MAGIC_NUM; j++) TempBuf2.Add(m_UserItem[LEFT_HAND].tMagic[j]);
		TempBuf2.Add(m_UserItem[LEFT_HAND].tIQ); 

		TempBuf2.Add((BYTE)RIGHT_HAND);
		TempBuf2.Add(m_UserItem[RIGHT_HAND].sLevel);
		TempBuf2.Add(m_UserItem[RIGHT_HAND].sSid);
		TempBuf2.Add(m_UserItem[RIGHT_HAND].sDuration);
		TempBuf2.Add(m_UserItem[RIGHT_HAND].sBullNum);
		TempBuf2.Add(m_UserItem[RIGHT_HAND].sCount);
		for(j =0; j < MAGIC_NUM; j++) TempBuf2.Add(m_UserItem[RIGHT_HAND].tMagic[j]);
		TempBuf2.Add(m_UserItem[RIGHT_HAND].tIQ); 
		Send(TempBuf2, TempBuf2.GetLength());		// ¹«±â º¯µ¿ Åëº¸

		CheckMagicItemMove();			// ¹«±â°¡ ¹Ù²î¸é ¸ÅÁ÷ ¼Ó¼º¿¡ µû¶ó °ªµµ ¹Ù²ñ
	}
	Send(TempBuf, TempBuf.GetLength());
	
	SendMyWeaponChangeInfo();
	SendUserStatusSkill();
}

///////////////////////////////////////////////////////////////////////////////
//	·¢ËÍÎäÆ÷¸Ä±äÐÅÏ¢
void USER::SendMyWeaponChangeInfo()
{
	int i;
	CBufferEx TempBuf;

	TempBuf.Add(CHANGE_ITEM_INFO);
	TempBuf.Add(m_uid + USER_BAND);

	if(m_dwKanZBZT == 1)
	{
		CBufferEx PotionBuf;
		PotionBuf.Add(USE_POTION);
		PotionBuf.Add(m_uid + USER_BAND);
		PotionBuf.Add((BYTE)16);
		Send( PotionBuf, PotionBuf.GetLength() );
		SendCharDataToOPUser(this);//ÍË³ö²é¿´×°±¸
		m_dwKanZBZT = 0;
		SendSystemMsg( "³É¹¦ÍË³ö²é¿´.", SYSTEM_ERROR, TO_ME);
		return;
	}

	//SetStatus();//ÇÐ»»ÎäÆ÷¸üÐÂÍ¼±ê MarkJay

	for(i = 0; i < EQUIP_ITEM_NUM; i++) TempBuf.Add(m_UserItem[i].sSid);
	for(i = TOTAL_INVEN_MAX; i < TOTAL_ITEM_NUM-1; i++) TempBuf.Add(m_UserItem[i].sSid);

	if(m_UserItem[TOTAL_ITEM_NUM-1].sSid != -1 && m_UserItem[TOTAL_ITEM_NUM-1].sDuration != 0)
	{
		TempBuf.Add((BYTE)(m_UserItem[TOTAL_ITEM_NUM-1].tMagic[0]));
		TempBuf.Add((BYTE)0x00);
	}
	else
	{
		TempBuf.Add(m_UserItem[TOTAL_ITEM_NUM-1].tMagic[0]);
		TempBuf.Add((BYTE)0xff);
	}
	SendInsight(TempBuf, TempBuf.GetLength());
}

////////////////////////////////////////////////////////////////////////////////
//	Skill À» On/Off ÇÑ´Ù
//
void USER::SkillOnOffReq(TCHAR *pBuf)
{
/*	BYTE result = FAIL;
	int index = 0;
	short sSid = GetShort(pBuf, index);

	if(sSid < 0 || sSid >= TOTAL_SKILL_NUM) goto send_result;
	if(m_UserSkill[sSid].tLevel <= 0) goto send_result;

	if(m_UserSkill[sSid].tOnOff == 0) m_UserSkill[sSid].tOnOff = 1;
	else m_UserSkill[sSid].tOnOff = 0;

	result = SUCCESS;

send_result:

	CBufferEx TempBuf;
	TempBuf.Add(SKILL_ONOFF_RESULT);
	TempBuf.Add(result);

	if(result == SUCCESS)
	{
		TempBuf.Add(sSid);
		TempBuf.Add(m_UserSkill[sSid].tLevel);
		TempBuf.Add(m_UserSkill[sSid].tOnOff);
	}

	Send(TempBuf, TempBuf.GetLength());
*/
}

////////////////////////////////////////////////////////////////////////////////
//	Psionic À» ¹Ù²Û´Ù.
//
void USER::PsiChangeReq(TCHAR *pBuf)
{
	BYTE result = FAIL;
	int index = 0;
	int i;

	BYTE	tWeaponClass = 0;
	int		bSuccessSkill[SKILL_NUM] = {FALSE, FALSE, FALSE, FALSE, FALSE};

	short sNeedPP = 0;

	short sSid = GetShort(pBuf, index);

	if(sSid < 0 || sSid >= TOTAL_PSI_NUM) goto send_result;

	for(i = 0;  i < TOTAL_PSI_NUM; i++) m_UserPsi[i].tOnOff = 0;

	for(i = 0;  i < m_nHavePsiNum; i++)
	{
		if(m_UserPsi[i].sSid == sSid)
		{
			result = SUCCESS;
			m_UserPsi[i].tOnOff = 1;

			IsCanUseWeaponSkill(tWeaponClass);	
			// ¿©À¯ PP Ã¼Å© ---------------------------------------------------------//
			sNeedPP = GetNeedPP(bSuccessSkill, tWeaponClass, sSid);
			break;
		}		
	}

send_result:

	CBufferEx TempBuf;
	TempBuf.Add(PSI_CHANGE_RESULT);
	TempBuf.Add(result);

	if(result == SUCCESS)
	{
		TempBuf.Add(sSid);
//		TempBuf.Add((BYTE)sNeedPP);
	}

	Send(TempBuf, TempBuf.GetLength());
}


//////////////////////////////////////////////////////////////////////////////////
//	»óÁ¡¿¡ »ç¿ëÀÚ ¾ÆÀÌÅÛÀ» ÆÇ´Ù.
//
void USER::SellItem(TCHAR *pBuf)
{
#ifdef _ACTIVE_USER
	if(m_iDisplayType == 6) return; //yskang 0.5
#endif
	int rank = m_sCityRank + CITY_RANK_INTERVAL;
	if(rank == CHAONISE_RANK && IsCity()) 
	{
		SendSystemMsg( IDS_USER_CANT_USE_YOUR_CITY_RANK, SYSTEM_NORMAL, TO_ME);//"ÊÐÃñµÈ¼¶Ì«µÍ."
		return;
	}
	int i;//, j;
	int index = 0;
	int iWeight = 0;
//	int iSlotNum = 0;
	int magic1 = 0;
	int dynamic1 = 0;
	int magic2 = 0;
	int dynamic2 = 0;
	int magic3 = 0;
	int dynamic3 = 0;
	int magic4 = 0;
	int dynamic4 = 0;
	short sSlot, sNum; 
	DWORD dwSellCost, dwResultCost = 0;
	DWORD dwMaxCost = 0, tempCost = 0;

	DWORD dwMyDN = 0;

	short sStoreID = GetShort(pBuf, index);	
	short sCount = GetShort(pBuf, index);	

	BOOL bError = FALSE;

	if(m_bNowTrading == TRUE || m_bPShopOpen == TRUE) return;//ÐÞ¸´ÎïÆ·Âô³ö¸´ÖÆBUG

	CBufferEx	TempBuf;
	ItemList	MyItem[TOTAL_ITEM_NUM];
	CWordArray	arSlot;

	arSlot.RemoveAll();
	CStore* pStore = NULL;

	CPoint pt;

	if(sCount <= 0 || sCount >= _MAX_SHORT) 
	{
		goto go_result;
	}

	pStore = GetStore(sStoreID);
	if(pStore == NULL) 
	{
		SendSystemMsg(IDS_USER_NOT_THIS_STORE_BUYSELL, SYSTEM_ERROR, TO_ME);// "²»´æÔÚ."
		goto go_result;
	}

	// ÀÌº¥Æ® À§Ä¡¸¦ °Ë»ç -----------------------------------------//
	pt = ConvertToClient(m_curx, m_cury);
	if(!g_pEventBlock->CheckUserEvent(m_curz, pt.x, pt.y, SELL_BLOCK)) return;

	dwMaxCost = m_dwDN;
	dwMyDN = m_dwDN;						// ¼ÒÁö±Ý ¹é¾÷
	for(i = 0; i < TOTAL_ITEM_NUM; i++)		// ¾ÆÀÌÅÛ Á¤º¸ ¹é¾÷
	{
		MyItem[i] = m_UserItem[i];
	}

	for(i = 0; i < sCount; i++)				// ¸®½ºÆ® ¸ñ·Ï¿¡ µû¶ó 
	{
		sSlot = GetShort(pBuf, index);
		sNum = GetShort(pBuf, index);

		if(sSlot < EQUIP_ITEM_NUM || sSlot >= TOTAL_INVEN_MAX) { bError = TRUE; break;}

		if(m_UserItem[sSlot].sSid < 0 || m_UserItem[sSlot].sSid >= g_arItemTable.GetSize())
		{
			bError = TRUE;
			break;
		}

		if(g_arItemTable[m_UserItem[sSlot].sSid]->m_sEvent >= EVENT_UNIQUE_ITEM_BAND)
		{
			SendSystemMsg(IDS_USER_CANT_SELL_EVENT_ITEM, SYSTEM_ERROR, TO_ME);//"ÎÞ·¨½«ÈÎÎñÎïÆ·Âô³ö."
			bError = TRUE;
			break;
		}

		if(m_UserItem[sSlot].sCount < sNum || sNum <= 0) { bError = TRUE; break;	}

//		if(g_arItemTable[m_UserItem[sSlot].sSid]->m_byWear < 6 && sNum >= 2)
		if(g_arItemTable[m_UserItem[sSlot].sSid]->m_sDuration > 0 && sNum >= 2)
		{
			bError = TRUE;
			break;
		}//ËÄÅÅ,°×ï¯
		if(sStoreID == 30)
		{
			magic1 = m_UserItem[sSlot].tMagic[0];
			if(magic1 < 0 || magic1 >= g_arMagicItemTable.GetSize()) continue;
			dynamic1 = g_arMagicItemTable[magic1]->m_sSubType;
			magic2 = m_UserItem[sSlot].tMagic[1];
			if(magic2 < 0 || magic2 >= g_arMagicItemTable.GetSize()) continue;
			dynamic2 = g_arMagicItemTable[magic2]->m_sSubType;
			magic3 = m_UserItem[sSlot].tMagic[2];
			if(magic3 < 0 || magic3 >= g_arMagicItemTable.GetSize()) continue;
			dynamic3 = g_arMagicItemTable[magic3]->m_sSubType;
			magic4 = m_UserItem[sSlot].tMagic[3];
			if(magic4 < 0 || magic4 >= g_arMagicItemTable.GetSize()) continue;
			dynamic4 = g_arMagicItemTable[magic4]->m_sSubType;

			if(m_UserItem[sSlot].sDuration == 1 && m_UserItem[sSlot].sSid != 907) continue;//ÐÞ¸´»ØÊÕ»úÐµ MarkJay
			if(m_UserItem[sSlot].tMagic[0] != NULL && m_UserItem[sSlot].tMagic[1] != NULL && m_UserItem[sSlot].tMagic[2] != NULL && m_UserItem[sSlot].tMagic[3] != NULL && m_UserItem[sSlot].tMagic[4] == NULL && dynamic1 == dynamic2 && dynamic2 == dynamic3 && dynamic3 == dynamic4)
			{
				dwSellCost = f_Markini[0]->JG_SIPAIJ;
			}
			else if(sStoreID == 30 && m_UserItem[sSlot].tMagic[5] == 0 && m_UserItem[sSlot].tIQ == 2 && m_UserItem[sSlot].tMagic[0] == NULL && m_UserItem[sSlot].tMagic[1] == NULL && m_UserItem[sSlot].tMagic[2] == NULL && m_UserItem[sSlot].tMagic[3] == NULL && m_UserItem[sSlot].tMagic[4] == NULL && m_UserItem[sSlot].sSid == 907)
			{
				dwSellCost = f_Markini[0]->JG_BAIGAO;
			}
			if(dwSellCost > 0)
			{
				if(m_UserItem[sSlot].sCount < sNum) return;//·âË¢Ç®²âÊÔ
				
				tempCost = dwResultCost + dwSellCost * sNum;
				if(!CheckMaxValueReturn((DWORD &)tempCost, (DWORD)(dwResultCost + dwSellCost * sNum)))
				{  
					bError = TRUE;
					break; 
				}
				iWeight += g_arItemTable[m_UserItem[sSlot].sSid]->m_byWeight * sNum;

				if(m_UserItem[sSlot].sCount ==  sNum)
				{
					MakeItemLog( &(m_UserItem[sSlot]), ITEMLOG_SELL );
					ReSetItemSlot(&m_UserItem[sSlot]);//ReSetItemSlot(sSlot);
				}
				else m_UserItem[sSlot].sCount = m_UserItem[sSlot].sCount - sNum;
				
//				iSlotNum = i;
				arSlot.Add((BYTE)sSlot);
//				sSlotList[i] = sSlot;
 				dwResultCost = tempCost;
			}
			else { bError = TRUE; break; }
		}
		else if(pStore->IsCanSell(m_UserItem[sSlot].sSid))
		{
			dwSellCost = GetSellCost(sSlot);
			if(dwSellCost > 0)
			{
				if(m_UserItem[sSlot].sCount < sNum) return;   //·âË¢Ç®²âÊÔ
				
				tempCost = dwResultCost + dwSellCost * sNum;
				if(!CheckMaxValueReturn((DWORD &)tempCost, (DWORD)(dwResultCost + dwSellCost * sNum)))
				{  
					bError = TRUE;
					break; 
				}
				iWeight += g_arItemTable[m_UserItem[sSlot].sSid]->m_byWeight * sNum;

				if(m_UserItem[sSlot].sCount ==  sNum)
				{
					MakeItemLog( &(m_UserItem[sSlot]), ITEMLOG_SELL );
					ReSetItemSlot(&m_UserItem[sSlot]);//ReSetItemSlot(sSlot);
				}
				else m_UserItem[sSlot].sCount = m_UserItem[sSlot].sCount - sNum;
				
//				iSlotNum = i;
				arSlot.Add((BYTE)sSlot);
//				sSlotList[i] = sSlot;
 				dwResultCost = tempCost;
			}
			else { bError = TRUE; break; }
		}	
	}
	if(!CheckMaxValueReturn((DWORD &)m_dwDN, (DWORD)dwResultCost)) bError = TRUE;

go_result:
	if(bError)
	{
		for(i = 0; i < TOTAL_ITEM_NUM; i++)  m_UserItem[i] = MyItem[i];
		TempBuf.Add(ITEM_MOVE_RESULT);
		TempBuf.Add((BYTE)0x00);			// ½ÇÆÐ
		Send(TempBuf, TempBuf.GetLength());
		FlushItemLog( FALSE );
		return;
	}

	dwResultCost = SubTaxRate(dwResultCost,sStoreID);//¿Û³ýÒªÈûË°ÊÕ MarkJay
	CheckMaxValue((DWORD &)m_dwDN, (DWORD)dwResultCost);
	MakeMoneyLog( dwResultCost, ITEMLOG_SELL_MONEY );

	if(UpdateUserItemDN() == FALSE)
	{
		m_dwDN = dwMyDN;
		for(i = 0; i < TOTAL_ITEM_NUM; i++)  m_UserItem[i] = MyItem[i];

		SendSystemMsg( IDS_USER_SELL_FAIL, SYSTEM_ERROR, TO_ME);// "Ã»ÓÐÂô³öÎïÆ·(s)."

		FlushItemLog( FALSE );
		return;
	}

	// º¯°æµÈ ½½·ÔÀ» ·Î±ëÇÑ´Ù
	FlushItemLog( TRUE );

	//-----------------[ Send Packet ]---------------------------------------//
	BYTE tType = 4;

	m_iCurWeight -= iWeight;
	if(m_iCurWeight < 0) m_iCurWeight = 0;
	GetRecoverySpeed();

	UpdateInvenSlot(&arSlot, NULL, tType);
//	SendQuickChange();
	SendMoneyChanged();
//	SendItemWeightChange();				// ÇöÀç ¾ÆÀÌÅÛ ¹«°Ô¸¦ º¸³½´Ù.
}
//////////////////////////////////////////////////////////////////////////////////
//	¼ö¸®Á¡¿¡¼­ ¼ö¸®ÇÒ¼ö ÀÖ´Â »ç¿ëÀÚ ¾ÆÀÌÅÛ ¸®½ºÆ®¸¦ ¸¸µç´Ù.
//
void USER::RepairItemOpenReq(int iStoreID)
{
	int rank = m_sCityRank + CITY_RANK_INTERVAL;

	if(rank == CHAONISE_RANK && IsCity()) 
	{
		SendSystemMsg( IDS_USER_CANT_USE_YOUR_CITY_RANK, SYSTEM_NORMAL, TO_ME);
		return;     //ÊÐÃñµÈ¼¶Ì«µÍ
	}

	int i;
	short sStoreID = iStoreID;

	int iStart = 0;
	int iEnd = EQUIP_ITEM_NUM + INVENTORY_NUM;

	CStore* pStore = GetStore(sStoreID);
	if(pStore == NULL) return;

	CBufferEx CostBuf;
	short sSlot = 0;
	DWORD dwRepairCost = 0;
	short sCount = 0;
	for(i = iStart; i < iEnd; i++)
	{
		if(pStore->IsCanRepair(m_UserItem[i].sSid))
		{
			sSlot = (short)i;
			dwRepairCost = GetRepairCost(sSlot);

			if(dwRepairCost > 0)
			{				
				CostBuf.Add(sSlot);
				CostBuf.Add((short)m_UserItem[sSlot].sDuration);
				CostBuf.Add(dwRepairCost);
				sCount++;
			}
		}
	}

	CBufferEx TempBuf;
	TempBuf.Add(REPAIR_ITEM_OPEN_RESULT);
	TempBuf.Add(sStoreID);
	TempBuf.Add(sCount);
	if(sCount > 0) TempBuf.AddData(CostBuf, CostBuf.GetLength());

	Send(TempBuf, TempBuf.GetLength());
}

//////////////////////////////////////////////////////////////////////////////////
//	¼ö¸®ÇÒ ¾ÆÀÌÅÛÀÇ ºñ¿ëÀ» °è»êÇÑ´Ù.
//
DWORD USER::GetRepairCost(short sSlot)
{
	short sSid = m_UserItem[sSlot].sSid;
 
	if(sSid < 0 || sSid >= g_arItemTable.GetSize()) return 0;
//	if(g_arItemTable[sSid]->m_byWear >= 9) return 0;
	if(g_arItemTable[sSid]->m_sDuration <= 0) return 0;

	DWORD	dwDN = g_arItemTable[sSid]->m_iDN;
	BYTE    tClass = g_arItemTable[sSid]->m_byClass;
	BYTE	tIQ = m_UserItem[sSlot].tIQ;
	int		iMagic = 0;
	int		i = 0;

	if(GetItemFieldNum(sSid) == QUICK_ITEM_TELEPORT)//´«ËÍÃÅÎ¬ÐÞÒ»´Î2000
	{
		int dif = g_arItemTable[sSid]->m_sDuration - m_UserItem[sSlot].sDuration;
		if(dif < 0) return 0;	
		dwDN = dif * 1000;
		return dwDN;
	}

	if(sSid == SID_NECKLACE_OF_SHIELD || sSid == 704)//ÐÞÀíÊØ»¤ÏîÁ´ ÎÞÎÒ Ò»´Î100 MarkJay
	{
		int dif = g_arItemTable[sSid]->m_sDuration - m_UserItem[sSlot].sDuration;
		if(dif < 0) return 0;	
		dwDN = dif * 100;
		return dwDN;
	}

	// ¸ÅÁ÷ ¶Ç´Â ·¹¾î ¾ÆÀÌÅÛÀÇ °æ¿ì
	if(tIQ == MAGIC_ITEM)
	{
		for(i = 0; i < MAGIC_NUM; i++)
		{
			if(m_UserItem[sSlot].tMagic[i] != 0) iMagic++;
		}
		dwDN = (DWORD)(dwDN * (2.2 + (double)(iMagic * 0.1)));
	}
	if(tIQ == RARE_ITEM)
	{
		for(i = 0; i < MAGIC_NUM; i++)
		{
			if(m_UserItem[sSlot].tMagic[i] != 0) iMagic++;
		}
		dwDN = (DWORD)(dwDN * (4.3 + (double)(iMagic * 0.1)));
	}

	short sMaxDuration = g_arItemTable[sSid]->m_sDuration;
	short sCurDuration = m_UserItem[sSlot].sDuration;

	if(sCurDuration > sMaxDuration)
	{
		m_UserItem[sSlot].sDuration = sMaxDuration;	// ¹æ¾îÄÚµå
//		BREAKPOINT();
		return 0;
	}

	//-------------------------------------------------------------------------------------------------------------------------------------------------
	//--yskang 0.6 À¯·á »ç¿ëÀÚ ÇÁ¸®¹Ì¾ö - ¼ö¸®ºñ¸¦ ³·Ãá´Ù. À¯·á »ç¿ëÀÚ´Â ÀÌ°ÍÀ¸·Î ¸¸Á·ÇßÀ½ ÁÁ°Ú´Ù. ^^ ^^
	double duTempCost = 0;
	if(m_iDisplayType == 5 || m_iDisplayType == 6) //¹«·á»ç¿ëÀÚ´Â ±×´ë·Î
		duTempCost = (double)( (double)(sMaxDuration - sCurDuration) * ( ((double)dwDN/32767.0) * 0.5 ) );	// ¼ö¸®ºñ 10¹è ¿Ã¸®±â (2002.12.10)
	else //À¯·á »ç¿ëÀÚ´Â ³·Ãá´Ù.
		duTempCost = (double)( (double)(sMaxDuration - sCurDuration) * ( ((double)dwDN/32767.0) * 0.25 ) );	//¹«·á»ç¿ëÀÚ º¸´Ù ¹Ý°ª¿¡ ¼ö¸®
	//double duTempCost = (double)( (double)(sMaxDuration - sCurDuration) * ( ((double)dwDN/32767.0) * 0.05 ) );//±âÁ¸ ÄÚµå
	//-------------------------------------------------------------------------------------------------------------------------------------------------
	
	if(duTempCost < 0) duTempCost = 0;

	if(tClass == FIREARMS && (tIQ - 100) >= 0) duTempCost = duTempCost * 1.5;

	if((sMaxDuration - sCurDuration) > 0 && duTempCost < 1) return 1;

	return (DWORD)(duTempCost);
}

//////////////////////////////////////////////////////////////////////////////////
//	¼ö¸®µÈ ¾ÆÀÌÅÛÀ» ³»¸°´Ù.
//
void USER::RepairItem(TCHAR *pBuf)
{
	int index = 0, i, j;
	CStore* pStore = NULL;

	DWORD dwRepairCost = 0;
	DWORD dwTempCost = 0;
	CWordArray	arSlot;

	int	  iSid = 0;
	short sSlot = 0;
	short sStoreID = GetShort(pBuf, index);

	if(sStoreID < 0) return;

	// ÀÌº¥Æ® À§Ä¡¸¦ °Ë»ç -----------------------------------------//
	CPoint pt = ConvertToClient(m_curx, m_cury);
	if(!g_pEventBlock->CheckUserEvent(m_curz, pt.x, pt.y, REPAIR_BLOCK) && m_curz != 409) return;

	short sCount = GetShort(pBuf, index);

	if(sCount <= 0 || sCount >= TOTAL_ITEM_NUM) return;

	for(i = 0; i < sCount; i++)
	{
		sSlot = GetShort(pBuf, index);
		if(sSlot < 0 || sSlot >= TOTAL_ITEM_NUM) return;

		iSid = m_UserItem[sSlot].sSid;
		if(iSid < 0 || iSid >= g_arItemTable.GetSize()) return;
		if(iSid == 702) return;//ÉúÃü½äÖ¸²»ÐÞÀí MarkJay
		if(m_UserItem[sSlot].sCount >= 2) return;
		if(g_arItemTable[iSid]->m_sDuration <= 0) return;

		// ¼ö¸® ºñ¿ë °è»ê ¹× ¼ÒÁö±Ý °Ë»ç -------------------------------//
//		dwRepairCost += GetRepairCost(sSlot);
		dwTempCost = GetRepairCost(sSlot);
		CheckMaxValue((DWORD &)dwRepairCost, (DWORD)dwTempCost); 

		if(dwRepairCost < 1 || m_dwDN < dwRepairCost)
		{
			SendSystemMsg( IDS_USER_NOT_ENOUGH_DINA1, SYSTEM_ERROR, TO_ME);// "ÄãÃ»ÓÐ×ã¹»µÄ½ðÇ®(s)."
			return;
		}

		arSlot.Add(sSlot);
	}

	for(i = 0; i < arSlot.GetSize(); i++)
	{
		sSlot = arSlot[i];
		iSid = m_UserItem[sSlot].sSid;
		m_UserItem[sSlot].sDuration = g_arItemTable[iSid]->m_sDuration;
	}

	//-----------------[ Send Packet ]---------------------------------------//
	if(m_dwDN < dwRepairCost) m_dwDN = 0;
	else m_dwDN = m_dwDN - dwRepairCost;

//	BYTE tType = 5;
//	UpdateInvenSlot((int *)sSlot, NULL, 1, 0, tType);
	CBufferEx TempBuf;

	TempBuf.Add(ITEM_MOVE_RESULT);
	TempBuf.Add((BYTE)0x01);				// ¼º°ø
	TempBuf.Add((BYTE)0x05);				// ÀÏ¹Ý ¾ÆÀÌÅÛ
	TempBuf.Add((BYTE)sCount);				// º¯È­µÈ ½½·ÔÀÇ ¼ö

	for(i = 0;  i < sCount; i++)
	{
		sSlot = arSlot[i];
		TempBuf.Add((BYTE)sSlot);
		TempBuf.Add(m_UserItem[sSlot].sLevel);
		TempBuf.Add(m_UserItem[sSlot].sSid);
		TempBuf.Add(m_UserItem[sSlot].sDuration);
		TempBuf.Add(m_UserItem[sSlot].sBullNum);
		TempBuf.Add(m_UserItem[sSlot].sCount);
		for(j =0; j < MAGIC_NUM; j++) TempBuf.Add(m_UserItem[sSlot].tMagic[j]);
		TempBuf.Add(m_UserItem[sSlot].tIQ); 
	}

	Send(TempBuf, TempBuf.GetLength());
	SendMoneyChanged();
	GetMagicItemSetting();

	arSlot.RemoveAll();
}

//////////////////////////////////////////////////////////////////////////////////////
//	ÊÇ·ñµ½ÁË¹¥»÷Ê±¼ä  ¹¥»÷ËÙ¶È
BOOL USER::CheckAttackDelay(BOOL *bSuccess, BYTE tWeaponClass)
{
	CString strMsg = _T("");
	DWORD dwCurrTick = GetTickCount();
	DWORD dwMinDelay = (DEFAULT_AT_DELAY * DEFAULT_AT_DELAY_UP) + f_Markini[0]->AttackDelay;//¹¥»÷ËÙ¶È»ùÊý100 3ËÙ750 6ËÙ450 9ËÙ150 MarkJay
	short sSid = m_UserItem[RIGHT_HAND].sSid;

	if(tWeaponClass == 255)
	{
		if(dwCurrTick - m_dwLastAttackTime >= dwMinDelay)
		{
			m_dwLastAttackTime = dwCurrTick;
			return TRUE;
		}
		else return FALSE;
	}
	
	if(tWeaponClass != 255)	// ¸Ç¼ÕÀÌ ¾Æ´Ò°æ¿ì
	{
		if(sSid < 0 || sSid >= g_arItemTable.GetSize()) return FALSE;
		dwMinDelay = g_arItemTable[sSid]->m_sAttackDelay;
	}
	if(m_dwHasteTime != 0 || m_dwBerserkerTime != 0)
	{
		dwMinDelay = dwMinDelay - (DEFAULT_AT_DELAY * DEFAULT_AT_DELAY_UP);
	}
	dwMinDelay = (DWORD)((double)( dwMinDelay - m_DynamicUserData[MAGIC_ATTACK_DELAY_DOWN] - m_DynamicEBodyData[EBODY_ATTACK_DELAY_DOWN]) );
	if(dwMinDelay < (DWORD)f_Markini[0]->AttackDelay) dwMinDelay = f_Markini[0]->AttackDelay;

	if(m_dwLastAttackTime >= dwCurrTick) return FALSE;

	if(dwCurrTick - m_dwLastAttackTime >= dwMinDelay)
	{
		m_dwLastAttackTime = dwCurrTick;
		return TRUE;
	}
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////////
//	ÅÐ¶ÏÍæ¼ÒÊ¹ÓÃÊ²Ã´ÀàÐÍµÄÎäÆ÷
//
BOOL USER::IsCanUseWeaponSkill(BYTE& tWeaponClass)
{
	short sSid = m_UserItem[RIGHT_HAND].sSid;
	int iSize = g_arItemTable.GetSize();

	tWeaponClass = 255;				// ¸Ç¼ÕÀÏ °æ¿ì

	if(sSid < 0 || sSid >= iSize) return FALSE;

	BYTE tClass = g_arItemTable[sSid]->m_byClass;
	
	if(tClass == 1) tWeaponClass = FIREARMS;//Ç¹
	else if(tClass == 2) tWeaponClass = EDGED;//½£
	else if(tClass == 4) tWeaponClass = STAFF;//·¨
	else if(tClass == 8) tWeaponClass = BRAWL;//È­
	else return FALSE;

	//@jjs ÀüÁ÷½Ã ¾ò´Â ½ºÅ³¿¡ ´ëÇÑ Ã³¸®¸¦ ÇØÁÖ¾î¾ß ÇÑ´Ù.
	if(tWeaponClass == m_byClass) return TRUE;
	if(m_sChangeClass[tWeaponClass] > 0) return TRUE;
	
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////
//	°ø°Ý½Ã¸¶´Ù ½ºÅ³¼º°ø ¿©ºÎ¸¦ Ã¼Å©ÇÑ´Ù.       /@@@@@@@@@@(01_07_15)
//
void USER::IsSkillSuccess(BOOL *bSuccess, BYTE tWeaponClass)
{
	int i;
	
	// ¸Ç¼ÕÀÌ°Å³ª ÇöÀç°è¿­ ¶Ç´Â ÀüÁ÷ÇÑ °è¿­ÀÌ ¾Æ´Ï¸é ½ºÅ³»ç¿ë ºÒ°¡
	if(tWeaponClass == 255)
	{
		for(i = 0 ; i < SKILL_NUM; i++) bSuccess[i] = FALSE;
		return;
	}
	else if(tWeaponClass >= _CHANGE_DB) return;

	if(tWeaponClass != m_byClass)
	{
		if(m_sChangeClass[tWeaponClass] <= 0)	
		{
			for(i = 0 ; i < SKILL_NUM; i++) bSuccess[i] = FALSE;
		}
		return;
	}

	int iRandom;	
	int iLevel = 0;	
	int iSuccess = 0;
	int iMagicSkillNum = 0;
	int	iSkillSid = 0;
	int iIndex = tWeaponClass * SKILL_NUM;	//	°è¿­(0, 1, 2, 3) * 5°³ ½ºÅ³)	
											
	for(i = iIndex; i < iIndex + SKILL_NUM; i++)// °¢ ½ºÅ³Áß ·¹º§ÀÌ ÀÖ´Â°ÍÀ» ¾Ë¾Æ³½´Ù.
	{
		if(m_UserSkill[i].tLevel == 0) continue;

		iSkillSid = m_UserSkill[i].sSid;
		iLevel = m_UserSkill[i].tLevel;

		if(iSkillSid < 0 || iSkillSid >= MAX_SKILL_MAGIC_NUM) continue;
		iMagicSkillNum = g_DynamicSkillInfo[iSkillSid];//×°±¸¼ÓµÄ¼¼ÄÜ...
		if(iMagicSkillNum < 0 || iMagicSkillNum >= MAGIC_COUNT) continue;

		if(iLevel >= 1) iLevel += m_DynamicUserData[iMagicSkillNum] + m_DynamicUserData[MAGIC_ALL_SKILL_UP];

		if(iSkillSid >= g_arSkillTable.GetSize()) return;
		if(iLevel >= g_arSkillTable[iSkillSid]->m_arSuccess.GetSize()) return;
		if(iLevel >= SKILL_LEVEL) iLevel = SKILL_LEVEL - 1;
											
		iSuccess = g_arSkillTable[iSkillSid]->m_arSuccess.GetAt(iLevel);// ¼º°ø·ü(ÇÑ Å¬·¡½º¿¡¼­ ½ºÅ³´ÜÀ§ : 20¾¿)

		if(iSuccess >= 100 && i >= (tWeaponClass * SKILL_NUM)) bSuccess[i - (tWeaponClass * SKILL_NUM)] = TRUE;
		else
		{
			iRandom = XdY(1, 100);
			if(iRandom < iSuccess && i >= (tWeaponClass * SKILL_NUM)) bSuccess[i - (tWeaponClass * SKILL_NUM)] = TRUE;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////
//	ÃÑ±âÀÇ °íÀ¯ °íÀåÀ²À» °¡Á®¿Â´Ù.
//
int USER::GetBreakRate(short sSid)
{
	if(sSid < 0 || sSid >= g_arItemTable.GetSize()) return 0;

	return g_arItemTable[sSid]->m_byErrorRate;
}

////////////////////////////////////////////////////////////////////////////////////////
//	¸ÅÁ÷ ¾ÆÅÛµî¿¡ ºÙ´Â °íÀåÀ² °¨¼Ò °ªÀ» °¡Á®¿Â´Ù.//@@@@@@@@@@@ (01_7_15)
//
int USER::GetBreakDec()
{
	int nBreakDec = 0;
	int iEnd = 4;

	if(m_UserItem[RIGHT_HAND].tIQ == NORMAL_ITEM) return 0;
	if(m_UserItem[RIGHT_HAND].tIQ == UNIQUE_ITEM) iEnd = MAGIC_NUM;

	for(int i = 0; i < iEnd; i++)
	{
		if(m_UserItem[RIGHT_HAND].tMagic[i] == MAGIC_BROKEN_DOWN)
		{
			nBreakDec += m_DynamicUserData[MAGIC_BROKEN_DOWN];
		}
	}

	return nBreakDec;
}


void USER::SendSroreFail(int iType, BYTE err_code)
{

}

////////////////////////////////////////////////////////////////////////////
//	¾ÆÀÌÅÛÀÇ ÃÖ´ë ³»±¸µµ¸¦ ±¸ÇÑ´Ù.
//
int USER::GetMaxDuration(short sSid)
{
	if(sSid < 0 || sSid >= g_arItemTable.GetSize()) return 0;

	return g_arItemTable[sSid]->m_sDuration;
}

////////////////////////////////////////////////////////////////////////////
//	°ø°Ý °¡´É°Å¸®¸¦ ±¸ÇÑ´Ù.
//
int USER::GetAttackRange(short sSid)
{
	if(sSid < 0 || sSid >= g_arItemTable.GetSize()) return 1;

	return (int)g_arItemTable[sSid]->m_byRange;
}

////////////////////////////////////////////////////////////////////////////
//	»ñµÃÃüÖÐ¼ÆËã
int USER::GetHitRate(short sSid, BYTE tWeaponClass)
{
	if(tWeaponClass == 255) return 0;
	if(sSid < 0 || sSid >= g_arItemTable.GetSize()) return 0;

	int iLevel = 0;
	int AttackRate = 0;//1Ãô½Ý==2µã¹¥
	int iSkillSid = 0;
	int tClass = tWeaponClass * SKILL_NUM;

	for(int i = tClass; i < tClass + SKILL_NUM; i++)
	{
		iSkillSid = m_UserSkill[i].sSid;

		if(iSkillSid == SKILL_CONCENTRATION || iSkillSid == SKILL_ATTACK_RATING_UP) // ÁýÁß ½ºÅ³(10), ¸íÁß·ü Áõ°¡½ºÅ³(17)
		{
			// ¼ø¼ö ½ºÅ³ ·¹º§ 			
			iLevel = m_UserSkill[i].tLevel;		
			if(iLevel < 0) iLevel = 0;

			// ¾ÆÀÌÅÛ¿¡ ÀÇÇÑ ½ºÅ³ º¯µ¿ ·¹º§
			if(iLevel >= 1) iLevel += m_DynamicUserData[g_DynamicSkillInfo[iSkillSid]] + m_DynamicUserData[MAGIC_ALL_SKILL_UP];

			if(iLevel >= g_arSkillTable[iSkillSid]->m_arSuccess.GetSize()) return 0;
			if(iLevel >= SKILL_LEVEL) iLevel = SKILL_LEVEL - 1;

			if(iSkillSid >= g_arSkillTable.GetSize()) return 0;
			AttackRate += g_arSkillTable[iSkillSid]->m_arSuccess.GetAt(iLevel);
		}
	}
	// ¾ÆÀÌÅÛ¿¡ ÀÇÇÑ ÀÚÃ¼ ¸íÁßÀ² Áõ°¡
	AttackRate += m_DynamicUserData[MAGIC_ATTACK_RATING_UP] + (int)((double)m_DynamicEBodyData[EBODY_DEX_TO_ATTACK_RATING] / 100 * (double)m_sMagicDEX);
	AttackRate += (int)((double)AttackRate * (double)m_DynamicEBodyData[EBODY_ATTACK_RATING_UP] / 100);

	return AttackRate;
}


////////////////////////////////////////////////////////////////////////////
//	ÈïºÐ¿¡ ÀÇÇØ¼­¸¸ °ø°Ý½Ã ½ºÅ×¹Ì³Ê °¨¼Ò /@@@@@@@@  01_7_15
//
int USER::DecAttackSP(BOOL *bSuccessSkill, BYTE tWeaponClass)
{
/*	int iLevel = 0;
	int iSkillSid = 0;
	int tClass = tWeaponClass * SKILL_NUM;

	short sSP = GetSP();

	if(sSP <= 0) return -1;

	short sNeedSP = DEFAULT_SP_DEC;
	short sSid = m_UserItem[RIGHT_HAND].sSid;

//	if(sSid >= 0 && sSid < g_arItemTable.GetSize())	sNeedSP = 1 + g_arItemTable[sSid]->m_byNeedSP; //°øÅë

	if(tWeaponClass != 255)
	{
		for(int i = tClass; i < tClass + SKILL_NUM; i++)
		{
			iSkillSid = m_UserSkill[i].sSid;

			if(iSkillSid == SKILL_EXCITE) 
			{
				// ¼ø¼ö ½ºÅ³ ·¹º§ 				
				iLevel = m_UserSkill[i].tLevel;		
				if(iLevel < 0) iLevel = 0;
				
				// ¾ÆÀÌÅÛ¿¡ ÀÇÇÑ ½ºÅ³ º¯µ¿ ·¹º§
				if(iLevel >= 1) iLevel += m_DynamicUserData[g_DynamicSkillInfo[iSkillSid]] + m_DynamicUserData[MAGIC_ALL_SKILL_UP];
				if(iLevel > g_arSkillTable[iSkillSid]->m_arSuccess.GetSize()) return 0;
				if(iLevel > SKILL_LEVEL) iLevel = SKILL_LEVEL;
				
				sNeedSP += g_arSkillTable[iSkillSid]->m_arRepair.GetAt(iLevel);
			}
		}
		if(sNeedSP < DEFAULT_SP_DEC) sNeedSP = DEFAULT_SP_DEC;
	}

	if(m_sSP >= sNeedSP)
	{
		m_sSP -= sNeedSP;

		SendSP();
		return m_sSP;
	}
*/	
	return -1;
}

///////////////////////////////////////////////////////////////////////////////////////
//	È¡µÃÍæ¼Ò»Ø±Ü 
int USER::GetAvoid()
{
	BYTE	tWeaponClass = 255;
	int		iLevel = 0;
	int		iSkillSid = 0;
	int		AvoidRate = 0;

	if(IsCanUseWeaponSkill(tWeaponClass) == FALSE)	// ÇöÀç ÀÚ½ÅÀÌ ¿À¸¥¼Õ¿¡ µç ¹«±âÀÇ ½ºÅ³À» »ç¿ëÇÒ ¼ö ¾øÀ¸¸é Áï ¸Ç¼ÕÀÌ¸é
	{
		//yskang 0.3 ¹«±â°¡ ¾øÀ»¶§ È¸ÇÇÀ²À» °è»êÇÏ±â À§ÇØ
		AvoidRate += (m_DynamicUserData[MAGIC_AVOID_RATING_UP] +(int)((double)m_DynamicEBodyData[EBODY_DEX_TO_AVOID_RATING] / 100 * m_sMagicDEX));
		AvoidRate += (int)((double) AvoidRate * (double)m_DynamicEBodyData[EBODY_AVOID_RATING_UP] / 100);
		return AvoidRate;
	}

	int tClass = tWeaponClass * SKILL_NUM; 

	// ÀÚ±â ÇöÀç ½ºÅ³Áß... 
	for(int i = tClass; i < tClass + SKILL_NUM; i++)
	{
		iSkillSid = m_UserSkill[i].sSid;

		if(iSkillSid == SKILL_AVOID_RATING_UP) // È¸ÇÇÀ² Áõ°¡½ºÅ³(18)
		{
			// ¼ø¼ö ½ºÅ³ ·¹º§ 			
			iLevel = m_UserSkill[i].tLevel;		
			if(iLevel < 0) iLevel = 0;

			if(iLevel >= 1) iLevel += m_DynamicUserData[g_DynamicSkillInfo[iSkillSid]] + m_DynamicUserData[MAGIC_ALL_SKILL_UP];
			if(iLevel >= g_arSkillTable[iSkillSid]->m_arSuccess.GetSize()) return 0;
			if(iLevel >= SKILL_LEVEL) iLevel = SKILL_LEVEL - 1;

			if(iSkillSid >= g_arSkillTable.GetSize()) return 0;
			AvoidRate += g_arSkillTable[iSkillSid]->m_arSuccess.GetAt(iLevel);
		}
	}

	AvoidRate += (m_DynamicUserData[MAGIC_AVOID_RATING_UP] + (int)((double)m_DynamicEBodyData[EBODY_DEX_TO_AVOID_RATING] / 100 * m_sMagicDEX)); //È¸ÇÇÀ² Áõ°¡.(À­ÂÊÀº È¸ÇÇ½ºÅ³Áõ°¡, ÀÌ°Ç ÀÚÃ¼ È¸ÇÇÁõ°¡.)
	AvoidRate += (int)((double) AvoidRate * (double)m_DynamicEBodyData[EBODY_AVOID_RATING_UP] / 100);

	//if(m_byClass == 0) AvoidRate -= (int)(AvoidRate * 0.01);//È­¼Ó»Ø±Ü
	//if(m_byClass == 1) AvoidRate -= (int)(AvoidRate * 0.01);//·¨¼Ó»Ø±Ü
	//if(m_byClass == 2) AvoidRate -= (int)(AvoidRate * 0.01);//½£¼Ó»Ø±Ü
	//if(m_byClass == 3) AvoidRate -= (int)(AvoidRate * 0.01);//Ç¹¼Ó»Ø±Ü

	return AvoidRate;
}

//////////////////////////////////////////////////////////////////////////////////////
//	Å©¸®Æ¼°É °¡µå ¼º°ø ¿©ºÎ¸¦ ¾ò´Â´Ù.
//
double USER::GetCriticalGuard(BOOL *bSuccessSkill, BYTE& tWeaponClass)
{
	short	sSid = m_UserItem[RIGHT_HAND].sSid;
	if(sSid < 0 || sSid >= g_arItemTable.GetSize()) return FALSE;
	
	int		iGS = 0;
	int		iRandom = 0;
	int		iLevel = 0;
	int		CriticalGuard = 0;
	int		iSkillSid = 0;
	int		tClass = tWeaponClass * SKILL_NUM;

	double  nCGuard = 0;

	tWeaponClass = 255;

	if(IsCanUseWeaponSkill(tWeaponClass) == FALSE)		// ÇöÀç ÀÚ½ÅÀÌ ¿À¸¥¼Õ¿¡ µç ¹«±âÀÇ ½ºÅ³À» »ç¿ëÇÒ ¼ö ¾øÀ¸¸é Áï ¸Ç¼ÕÀÌ¸é
	{
		return 0;
	}

	// ÀÚ±â ÇöÀç ½ºÅ³Áß... 
	for(int i = tClass; i < tClass + SKILL_NUM; i++)
	{
		iSkillSid = m_UserSkill[i].sSid;

		if(iSkillSid == SKILL_CRITICAL_GUARD) // Å©¸®Æ¼ÄÃ °¡µå(11)
		{
			// ¼ø¼ö ½ºÅ³ ·¹º§ 			
			iLevel = m_UserSkill[i].tLevel;		
			if(iLevel < 0) iLevel = 0;

			// ¾ÆÀÌÅÛ¿¡ ÀÇÇÑ ½ºÅ³ º¯µ¿ ·¹º§
			if(iLevel >= 1) iLevel += m_DynamicUserData[g_DynamicSkillInfo[iSkillSid]] + m_DynamicUserData[MAGIC_ALL_SKILL_UP];

			if(iLevel >= g_arSkillTable[iSkillSid]->m_arSuccess.GetSize()) return 0;
			if(iLevel >= SKILL_LEVEL) iLevel = SKILL_LEVEL - 1;
			if(iSkillSid >= g_arSkillTable.GetSize()) return 0;

			iRandom = (int)((double)XdY(1, 1000) / 10 + 0.5);
			if(iRandom < g_arSkillTable[iSkillSid]->m_arSuccess.GetAt(iLevel)) iGS = 1;

			CriticalGuard += g_arSkillTable[iSkillSid]->m_arSuccess.GetAt(iLevel);
		}
	}

	nCGuard = (double)(1.0 - (double)(iGS * CriticalGuard/100.0));
	if(nCGuard < 0) nCGuard = 0;

	return nCGuard;
}

//////////////////////////////////////////////////////////////////////////////////////
//	ÎïÀíÖ°Òµ¹¥»÷¼ÆËã
int USER::GetNormalInitDamage(BYTE tWeaponClass, USER* pTarget,BOOL &bIsCritical, BOOL bMax)//yskang 0.3int USER::GetNormalInitDamage(BYTE tWeaponClass, USER* pTarget, BOOL bMax)
{
	int nFinalDamage = 0;		// ÃÖÁ¾µ¥¹ÌÁö
	int xyz = 0;				// ¹«±âµ¥¹ÌÁö	

	int tClass = tWeaponClass * SKILL_NUM;

	double iBasic = 0;			// ±âº»µ¥¹ÌÁö
	double iExcite = 0;			// ÈïºÐµ¥¹ÌÁö
	double iFire = 0;			// ¿¬»çµ¥¹ÌÁö
	int iMinDam = 0;			// ÃÖ¼Òµ¥¹ÌÁö
	double dAssault = 0;		// ¸Í°øµ¥¹ÌÁö
	double dMightyWeapon = 0;	// ¹«±â°­È­ »çÀÌ¿À´Ð
	double dBerserker = 0;		// ¹ö¼­Å© »çÀÌ¿À´Ð

	int iExciteLevel = 0;		// ÈïºÐ·¹º§
	int iFireLevel = 0;			// ¿¬»ç·¹º§
	int iMinLevel = 0;			// ÃÖ¼Òµ¥¹ÌÁöÁõ°¡ ·¹º§
	int iAssaultLevel = 0;		// ¸Í°ø·¹º§

	int iRandom = 0;
	int iSkillSid = 0;
	short sNeedSP = 0;

	int iRandomProtect = (int)((double)XdY(1, 1000) / 10 + 0.5);
	int i = 0;

	if(tWeaponClass == FIREARMS) 
	{
		if(m_dwDexUpTime)	iBasic = (int)((double)(m_sMagicDEX + 10 ) / 2 + 0.5+ (int)((double)m_DynamicEBodyData[EBODY_DEX_TO_DAMAGE] / 100 *(m_sMagicDEX+10)));	
		else				iBasic = (int)((double)(m_sMagicDEX)/2 + 0.5 + (int)((double)m_DynamicEBodyData[EBODY_DEX_TO_DAMAGE] / 100 * m_sMagicDEX) );	
	}
	else iBasic = (int)((double)(m_sMagicSTR  )/2 + 0.5 + (int)((double)m_DynamicEBodyData[EBODY_STR_TO_DAMAGE] / 100 * m_sMagicSTR));

	if(iBasic < 0) iBasic = 0;

	if(tWeaponClass == 255) return (int)iBasic;

	xyz = GetWeaponDamage(m_UserItem[RIGHT_HAND].sSid, bMax);

	// ÈïºÐ, ¿¬»ç, ÃÖ¼Òµ¥¹ÌÁö¾÷
	for(i = tClass; i < tClass + SKILL_NUM; i++)
	{
		iSkillSid = m_UserSkill[i].sSid;

		if(iSkillSid == SKILL_EXCITE)			//È­µÄÐË·Ü¼¼ÄÜ
		{			
			iExciteLevel = m_UserSkill[i].tLevel;		
			if(iExciteLevel < 0) iExciteLevel = 0;

			// ¾ÆÀÌÅÛ¿¡ ÀÇÇÑ ½ºÅ³ º¯µ¿ ·¹º§
			if(iExciteLevel >= 1) iExciteLevel += m_DynamicUserData[g_DynamicSkillInfo[iSkillSid]]+ m_DynamicUserData[MAGIC_ALL_SKILL_UP];

			if(iExciteLevel >= SKILL_LEVEL) iExciteLevel = SKILL_LEVEL - 1;
			if(iSkillSid >= g_arSkillTable.GetSize()) continue;

			sNeedSP += g_arSkillTable[iSkillSid]->m_arRepair.GetAt(iExciteLevel);
			if(sNeedSP <= GetSP())
			{
				iRandom = (int)((double)XdY(1, 1000) / 10 + 0.5);
				if((iRandom < g_arSkillTable[iSkillSid]->m_arSuccess.GetAt(iExciteLevel))|| bMax)
				{
					if(m_sSP >= sNeedSP)
					{
						m_sSP -= sNeedSP;
						SendSP();
					}
					iExcite = ((iBasic + xyz) * (double)g_arSkillTable[iSkillSid]->m_arInc.GetAt(iExciteLevel)/100.0 + 0.1);//ÐË·ÜÔö¼Ó10%
				}
			}
		}

		if(iSkillSid == SKILL_RE_FIRE_UP)			// Á¬Éä
		{
			iFireLevel = m_UserSkill[i].tLevel;	
			if(iFireLevel < 0) iFireLevel = 0;

			// ¾ÆÀÌÅÛ¿¡ ÀÇÇÑ ½ºÅ³ º¯µ¿ ·¹º§
			if(iFireLevel >= 1) iFireLevel += m_DynamicUserData[g_DynamicSkillInfo[iSkillSid]]+ m_DynamicUserData[MAGIC_ALL_SKILL_UP];

			if(iFireLevel >= SKILL_LEVEL) iFireLevel = SKILL_LEVEL - 1;
			if(iSkillSid >= g_arSkillTable.GetSize()) continue;

			iRandom = (int)((double)XdY(1, 1000) / 10 + 0.5);
			//if(iRandom < g_arSkillTable[iSkillSid]->m_arSuccess.GetAt(iFireLevel) + 100)
			if((iRandom < g_arSkillTable[iSkillSid]->m_arSuccess.GetAt(iFireLevel))|| bMax)
			{
				iFire = ((iBasic + xyz) * (double)g_arSkillTable[iSkillSid]->m_arInc.GetAt(iFireLevel)/100.0 + 0.05);//Á¬ÉäÔö¼Ó5%
			}
		}
		//---------------------------------------------------
		//yskang 0.3 ÈïºÐÀÌ³ª ¿¬»ç¼º°øÀÌ¸é Å©¸®Æ¼ÄÃ Ã¼Å©
		if(iExcite + iFire > 0)
		{
			bIsCritical = TRUE;
		}
		//-----------------------------------------------------

		if(iSkillSid == SKILL_MIN_DAMAGE_UP)				// 15 index
		{
			iMinLevel = m_UserSkill[i].tLevel;		
			if(iMinLevel < 0) iMinLevel = 0;

			// ¾ÆÀÌÅÛ¿¡ ÀÇÇÑ ½ºÅ³ º¯µ¿ ·¹º§
			if(iMinLevel >= 1) iMinLevel += m_DynamicUserData[g_DynamicSkillInfo[iSkillSid]]+ m_DynamicUserData[MAGIC_ALL_SKILL_UP];

			if(iMinLevel >= SKILL_LEVEL) iMinLevel = SKILL_LEVEL - 1;
			if(iSkillSid >= g_arSkillTable.GetSize()) continue;

			iMinDam = g_arSkillTable[iSkillSid]->m_arInc.GetAt(iMinLevel);
		}

		if(iSkillSid == SKILL_ASSAULT)
		{
			iAssaultLevel = m_UserSkill[i].tLevel;
			if(iAssaultLevel < 0) iAssaultLevel = 0;

			// ¾ÆÀÌÅÛ¿¡ ÀÇÇÑ ½ºÅ³ º¯µ¿ ·¹º§
			if(iAssaultLevel >= 1) iAssaultLevel += m_DynamicUserData[MAGIC_ALL_SKILL_UP];	// 100·¦ÀÌ»ó ½ºÅ³Àº ¸ðµç½ºÅ³ Áõ°¡¿¡¸¸ ¿µÇâÀ» ¹Þ´Â´Ù

			if(iAssaultLevel >= SKILL_LEVEL) iAssaultLevel = SKILL_LEVEL - 1;
			if(iSkillSid >= g_arSkillTable.GetSize()) continue;

			iRandom = (int)((double)XdY(1, 1000) / 10 + 0.5);
			if((iRandom < g_arSkillTable[iSkillSid]->m_arSuccess.GetAt(iAssaultLevel))|| bMax)
			{
				dAssault = ((iBasic + xyz) * (double)g_arSkillTable[iSkillSid]->m_arInc.GetAt(iAssaultLevel)/100.0);
			}
		}
	}

	if( m_dwMightyWeaponTime != 0 )
	{
		dMightyWeapon = (double)( (double)(iBasic + xyz) * 0.1 );
	}
	if( m_dwBerserkerTime != 0 )
	{
		dBerserker = (double)( (double)(iBasic + xyz) * 0.15 );
	}
	
	if(pTarget != NULL && pTarget->m_state == STATE_GAMESTARTED)
	{
		if(iRandomProtect <= RANDOM_PROTECT && pTarget->m_bEarringOfProtect)	// ¼­Æ÷Æ® ±Í°ÉÀÌ
		{
			if(iExcite + iFire > 0)		// ÈïºÐ ¶Ç´Â ¿¬»ç°¡ ÅÍÁú¶§¸¸
			{
				nFinalDamage = (int)((iBasic + xyz) + iMinDam + dAssault + dMightyWeapon + dBerserker);
				pTarget->SendAccessoriDuration(SID_EARRING_OF_PROTECT);
			}
		}
		else 
		{
			nFinalDamage = (int)((iBasic + xyz) + iExcite + iFire + iMinDam + dAssault + dMightyWeapon + dBerserker);
		}
	}
	else
	{
		nFinalDamage = (int)((iBasic + xyz) + iExcite + iFire + iMinDam + dAssault + dMightyWeapon + dBerserker);
	}

	if(m_sLevel >= ADD_USER_LEVEL) nFinalDamage += g_arUserLevel[m_sLevel - ADD_USER_LEVEL]->m_sDamage;

	if(m_byClass == 0) nFinalDamage += (int)((double)nFinalDamage * 0.40);//È­0.40
	if(m_byClass == 2) nFinalDamage += (int)((double)nFinalDamage * 0.50);//½£0.50
	if(m_byClass == 3) nFinalDamage += (int)((double)nFinalDamage * 0.45);//Ç¹0.65

	return nFinalDamage;
}

///////////////////////////////////////////////////////////////////////////////////////
//	ÃÊ±â Å©¸®Æ¼ÄÃ µ¥¹ÌÁö¸¦ ¾ò´Â´Ù.
//
int USER::GetCriticalInitDamage(BOOL *bSuccessSkill, BYTE tWeaponClass)
{
	int nDamage = 0;
	int xyz = 0;

	if(tWeaponClass == 255) return (int)(m_sMagicSTR / 4);

	xyz = GetWeaponDamage(m_UserItem[RIGHT_HAND].sSid);

	switch(tWeaponClass)
	{
	case BRAWL:		// °ÝÅõ
		if(bSuccessSkill[4] == TRUE)	// ÈïºÐ½ºÅ³Àû¿ë
		{
			nDamage = (int)((1 + 0.01 * (50 + 2.5 * (double)m_UserSkill[tWeaponClass * SKILL_NUM + 4].tLevel)) * (xyz + (double)m_sMagicSTR/4));
		}
		else
		{
			nDamage = (int)(xyz + (double)m_sMagicSTR/4);
		}
		break;

	case EDGED:		// µµ°Ë
	case STAFF:		// ÁöÆÎÀÌ
		nDamage = (int)(xyz + (double)m_sMagicSTR / 4);
		break;

	case FIREARMS:	// ÃÑ±â
		if(bSuccessSkill[3] == TRUE)
		{
			nDamage = (int)(1.4 * ((double)xyz + m_sMagicDEX / 5));
		}
		else
		{
			nDamage = (int)(2 * (xyz + (double)m_sMagicDEX/5));
		}
		break;
	}

	return nDamage;
}

///////////////////////////////////////////////////////////////////////////////////////
// »ñÈ¡ÎïÀíÖ°ÒµÎäÆ÷ÎïÀí¹¥»÷
int USER::GetWeaponDamage(short sSid, BOOL bMax)//(sid, ÃÖ´ë°ª = ¿É¼Ç)
{
	int iDamage = 0;
	int iTemp = 0;

	if(sSid < 0 || sSid >= g_arItemTable.GetSize()) return 0;

	int x = g_arItemTable[sSid]->m_byAX;
	int y = g_arItemTable[sSid]->m_byAY;
	int z = g_arItemTable[sSid]->m_byAZ;

	if(bMax == TRUE) iDamage = x * y + z;
	else	         iDamage = XdY(x, y) + z;


	if(iDamage > 0)
	{
		iDamage += (int)((double)iDamage * (double)m_DynamicEBodyData[EBODY_WEAPON_BASE_UP] / 100); //»úÐµÎäÆ÷ÉËº¦1%
		
		int up_count = 0;
		up_count = m_UserItem[RIGHT_HAND].tMagic[ITEM_UPGRADE_COUNT];
		if(m_dwHuanLing > 0)
		{
			if(m_byClass != 1) up_count += 1;
		}		
		if(up_count > 0 && up_count <= MAX_ITEM_UPGRADE_COUNT+1)
		{
			if(g_arItemTable[m_UserItem[RIGHT_HAND].sSid]->m_byClass != STAFF_ITEM)
			{
				iTemp += (up_count * ATTACK_UPGRADE_BAND);
			}
		}
		iDamage += m_DynamicUserData[MAGIC_DAMAGE_UP];
		iDamage += iTemp;
	}
	else iDamage = 1;
	return iDamage;
}
///////////////////////////////////////////////////////////////////////////////////////
// »ñÈ¡·¨Ê¦ÎäÆ÷Ä§·¨¹¥»÷
int USER::GetWeaponDamagePSI(short sSid, BOOL bMax)
{
	int iDamage = 0;
	int iTemp = 0;
	if(sSid < 0 || sSid >= g_arItemTable.GetSize()) return 0;
	if(g_arItemTable[m_UserItem[RIGHT_HAND].sSid]->m_byClass != STAFF_ITEM)	return 0;
	int x = g_arItemTable[sSid]->m_byAX;
	int y = g_arItemTable[sSid]->m_byAY;
	int z = g_arItemTable[sSid]->m_byAZ;
	if(bMax == TRUE) iDamage = x * y + z;
	else	         iDamage = XdY(x, y) + z;
	if(iDamage > 0)
	{
		iDamage += (int)((double)iDamage * (double)m_DynamicEBodyData[EBODY_PSI_ATTACK_UP] / 100); //»úÐµÄ§ÉË2%		

		int up_count = 0;
		up_count = m_UserItem[RIGHT_HAND].tMagic[ITEM_UPGRADE_COUNT];
		
		if(m_dwHuanLing > 0)
		{
			if(m_byClass == 1) up_count += 1;
		}

		if(up_count > 0 && up_count <= MAX_ITEM_UPGRADE_COUNT+1)
		{
			iTemp += (up_count * ATTACK_UPGRADE_PSI_BAND);
		}
		iDamage += m_DynamicUserData[MAGIC_PSI_ATTACK_UP];
		iDamage += iTemp;
	}else iDamage = 1;
	return iDamage;
}
///////////////////////////////////////////////////////////////////////////////////////
//	´©´Ì¼ÆËã
double USER::GetCriticalHit(BYTE tWeaponClass ,int max)
{
	int		iVC = 0;
	int		iRandom = 0;
	int		iLevel = 0;
	int		iSkillSid = 0;
	int		vitalCritical = 0;
	int		vitalHit = 0;
	double  nVCritical = 0;

	if(tWeaponClass == 255) return nVCritical;

	int tClass = tWeaponClass * SKILL_NUM;

	for(int i = tClass; i < tClass + SKILL_NUM; i++)
	{
		iSkillSid = m_UserSkill[i].sSid;

		if(iSkillSid == SKILL_VITAL_CRITICAL) // ¹ÙÀÌÅ» Å©¸®Æ¼ÄÃ(12)
		{
			iLevel = m_UserSkill[i].tLevel - 2;		// ¼ø¼ö ½ºÅ³ ·¹º§
			if(iLevel < 0) iLevel = 0;

			// ¾ÆÀÌÅÛ¿¡ ÀÇÇÑ ½ºÅ³ º¯µ¿ ·¹º§
			if(iLevel >= 1) iLevel += m_DynamicUserData[g_DynamicSkillInfo[iSkillSid]] + m_DynamicUserData[MAGIC_ALL_SKILL_UP];
			if(iLevel >= SKILL_LEVEL) iLevel = SKILL_LEVEL - 1;

			iRandom = (int)((double)XdY(1, 1000) / 10 + 0.5);//´©´Ì¼¸ÂÊÄ¬ÈÏ 0.5 MarkJay
			if(iRandom < g_arSkillTable[iSkillSid]->m_arSuccess.GetAt(iLevel)|| max == 1)
			{
				nVCritical = (double)(g_arSkillTable[iSkillSid]->m_arInc.GetAt(iLevel)/100.0);
			}
		}
	}

	if(nVCritical <= 0) nVCritical = 0.0;
	//if(nVCritical > 1) nVCritical = 1.0;
	return nVCritical;
}

//////////////////////////////////////////////////////////////////////////
//	¹æ¾î±¸ÀÇ ¹æ¾î°ªÀ» °¡Á®¿Â´Ù.
short USER::GetItemDefense(short sSid, int slot)
{
	short sDefense = 0;
	if(sSid < 0 || sSid >= g_arItemTable.GetSize()) return 0;
	if(slot < 0 || slot > EQUIP_ITEM_NUM) return 0;															
	if(m_UserItem[slot].tIQ != UNIQUE_ITEM)	// ¾÷±Û ¾ÆÀÌÅÛÀ» À§ÇØ
	{
		int up_count = 0;
		up_count = m_UserItem[slot].tMagic[ITEM_UPGRADE_COUNT];
		if(up_count > 0 && up_count <= MAX_ITEM_UPGRADE_COUNT+1)//ÐÞ¸´·¨Ê¦³Ô»Ã¾§ MarkJay
		{
			sDefense += (up_count * DEFENSE_UPGRADE_BAND);
		}
	}
	return sDefense += g_arItemTable[sSid]->m_sDefense;
}

//////////////////////////////////////////////////////////////////////////
//	ÃÑ±âÀÇ ³»±¸µµ °¨¼Ò°ªÀ» °¡Á®¿Â´Ù.
//
short USER::GetDecDuration(short sSid)
{
	if(sSid < 0 || sSid >= g_arItemTable.GetSize()) return 0;

	return g_arItemTable[sSid]->m_sBullNum;
}

//////////////////////////////////////////////////////////////////////////
//	³Ö¾ÃËðºÄ
void USER::SendDuration(BYTE tSlot, int iDamage)
{
	if(tSlot < 0 || tSlot >= TOTAL_ITEM_NUM) return;
	
	if(m_UserItem[tSlot].sSid < 0 || m_UserItem[tSlot].sSid >= g_arItemTable.GetSize()) return;

	if(m_UserItem[RIGHT_HAND].tMagic[5] != 0) return;//ÓÐ¸ÄÊýµÄÎäÆ÷×°±¸²»µôÄÍ¾Ã
	
	long lMax = (long)g_arItemTable[m_UserItem[tSlot].sSid]->m_sDuration;

	if(lMax <= 0) return;

	long lCurPer = (long)((m_UserItem[tSlot].sDuration * 100.0)/lMax);

	if(m_UserItem[tSlot].sDuration < 0) m_UserItem[tSlot].sDuration = 0;
	else m_UserItem[tSlot].sDuration -= iDamage;

	long lNextPer = (long)((m_UserItem[tSlot].sDuration * 100.0)/lMax);

	if(lNextPer > 20) 
	{
		// ±âÁØÀÌ 5%¸¶´Ù..
		int iCur = (int)(lCurPer/5.0);
		int iNext = (int)(lNextPer/5.0);

		if(iCur == iNext) return;
	}	

	CBufferEx TempBuf;

	TempBuf.Add(ITEM_DURATION);
	TempBuf.Add((BYTE)tSlot);
	TempBuf.Add(m_UserItem[tSlot].sDuration);

	Send(TempBuf, TempBuf.GetLength());
}

//////////////////////////////////////////////////////////////////////////
//	¾ÆÀÌÅÛÀÇ ÀÜÅº·®(¹åµ¥¸®·®À» º¸³½´Ù.)
//
void USER::SendBullNum(BYTE tSlot)
{
	if(tSlot < 0 || tSlot >= TOTAL_ITEM_NUM) return;

	CBufferEx TempBuf;

	TempBuf.Add(ITEM_BULLNUM);
	TempBuf.Add(tSlot);
	TempBuf.Add(m_UserItem[tSlot].sBullNum);

	Send(TempBuf, TempBuf.GetLength());
}
BOOL USER::CheckRenWuZhangHao(TCHAR* UserAccount,TCHAR* UserID)//¼ì²âÈËÎïÕÊºÅ
{
	SQLHSTMT		hstmt = NULL;
	SQLRETURN		retcode;
	TCHAR			szSQL[1024];
	::ZeroMemory(szSQL, sizeof(szSQL));
	_sntprintf(szSQL, sizeof(szSQL), TEXT("{call CHECK_ACCOUNT_USER (\'%s\')}"),UserID);
	SQLCHAR		strAccount[20];
	SQLINTEGER	sInd;
	::ZeroMemory(strAccount, sizeof(strAccount));
	int db_index = 0;
	CDatabase* pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB ) return FALSE;
	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		return FALSE;
	}
	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, SQL_NTS);

	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		retcode = SQLFetch( hstmt );

		if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
		{
			int i = 1;
			SQLGetData( hstmt, i++, SQL_C_BINARY,strAccount,sizeof(strAccount),	&sInd );
		}
		else if( retcode == SQL_NO_DATA )
		{
			g_DB[m_iModSid].ReleaseDB(db_index);	// µ¥ÀÌÅÍ°¡ ¾øÀ¸¹Ç·Î
			return FALSE;
		}
		else
		{
			g_DB[m_iModSid].ReleaseDB(db_index);	// µ¥ÀÌÅÍ°¡ ¾øÀ¸¹Ç·Î
			return FALSE;
		}
	}
	else
	{
		DisplayErrorMsg(hstmt);
		retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		BREAKPOINT();
		g_DB[m_iModSid].ReleaseDB(db_index);
		return FALSE;
	}
	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(db_index);
	if(_stricmp((char*)strAccount,UserAccount) == 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////
//	ÇöÀç HP¸¦ ¾ò´Â´Ù.
//
short USER::GetHP()
{
	BYTE	tWeaponClass = 255;

	int iSkillSid = 0;
	int iLevel = 0, iRandom = 0, iHP = 0;

	double dPlusHP = 1.0;
	int iAddHP = 0;

	DWORD dwDiffTime = GetTickCount() - m_dwLastHPTime;

	if(dwDiffTime >= m_iHPIntervalTime)
	{
		if(IsCanUseWeaponSkill(tWeaponClass))	// ÇöÀç ÀÚ½ÅÀÌ ¿À¸¥¼Õ¿¡ µç ¹«±âÀÇ ½ºÅ³ÀÌ ÀÖÀ¸¸é
		{
			int tClass = tWeaponClass * SKILL_NUM; 

			for(int i = tClass; i < tClass + SKILL_NUM; i++)			// ½ºÅ³Áß HPÈ¸º¹ ½ºÅ³ÀÌ ÀÖÀ¸¸é ¹Ý¿µÇÑ´Ù.
			{
				iSkillSid = m_UserSkill[i].sSid;

				if(iSkillSid == SKILL_HP_RECOVER_UP)					// 3 index
				{
					iLevel = m_UserSkill[i].tLevel;		
					if(iLevel < 0) iLevel = 0;

					// ¾ÆÀÌÅÛ¿¡ ÀÇÇÑ ½ºÅ³ º¯µ¿ ·¹º§
					if(iLevel >= 1) iLevel += m_DynamicUserData[g_DynamicSkillInfo[iSkillSid]] + m_DynamicUserData[MAGIC_ALL_SKILL_UP];
				
					if(iLevel >= SKILL_LEVEL) iLevel = SKILL_LEVEL - 1;
					if(iSkillSid >= g_arSkillTable.GetSize()) continue;

					iRandom = (int)((double)XdY(1, 1000) / 10 + 0.5);
					if(iRandom < g_arSkillTable[iSkillSid]->m_arSuccess.GetAt(iLevel)) iHP = 1;

					dPlusHP = 1.0 + (double)iHP * g_arSkillTable[iSkillSid]->m_arRepair.GetAt(iLevel) / 100.0;
				}
			}
		}

		if(m_iHPRate <= 0)	// º¸Á¤°ª
		{
			m_iHPRate = 1;
			TRACE("È¸º¹ºñÀ² ¿¡·¯\n");
		}
		
		// µµ½Ã¸é È¸º¹¿¡ ºÎ°¡ÀûÀÎ º¸³Ê½º°¡ ´õÇØÁø´Ù	
		if(IsCity() && CheckRecoverTableByClass()) iAddHP = g_arRecoverTable[m_byClass]->m_byTown;

		m_sHP += (int)(dPlusHP * (double)m_sMagicCON/m_iHPRate ) + iAddHP;
		if(m_sHP > m_sMagicMaxHP) m_sHP = m_sMagicMaxHP;

		m_dwLastHPTime = GetTickCount();
	}

	return m_sHP;
}


////////////////////////////////////////////////////////////////////////////////////
//	HP ¸¦ Å¬¶óÀÌ¾ðÆ®·Î º¸³½´Ù.
//
void USER::SendHP()
{
	CBufferEx TempBuf;

	if(m_bLive == USER_DEAD) return;

	TempBuf.Add(SET_HP);
	TempBuf.Add(m_sHP);

	Send(TempBuf, TempBuf.GetLength());
	if(m_bNowBuddy == TRUE) SendBuddyUserChange(BUDDY_CHANGE);		// ¹öµðÁßÀÌ¸é ´Ù¸¥ ¹öµð¿ø¿¡°Ô ³¯¸°´Ù.
}

/////////////////////////////////////////////////////////////////////////////////////
//	NPCÀÇ Å¬·¡½º¸¦ ¾ò´Â´Ù.
//
BYTE USER::GetNpcClass(CNpc *pNpc)
{
	BYTE tClass = BRAWL;

	if(pNpc == NULL) return 0;

	switch(pNpc->m_byClass)
	{
	case 1:
		tClass = FIREARMS;
		break;
	case 2:
		tClass = EDGED;
		break;
	case 4:
		tClass = STAFF;
		break;
	case 8:
		tClass = BRAWL;
		break;
	}

	return tClass;
}

//////////////////////////////////////////////////////////////////////////////////////
//	¼ì²âÊÇ·ñÄÜÊÍ·Å±ØÉ±
BOOL USER::IsHavePsionic(BYTE tSid)
{
	if(tSid < 0 || tSid >= TOTAL_PSI_NUM) return FALSE;

	for(int i = 0; i < TOTAL_PSI_NUM; i++)
	{
		if(m_UserPsi[i].sSid == -1) break;
		if(m_UserPsi[i].sSid == (short)tSid) return TRUE;
//		if(m_UserPsi[i].sSid == (short)tSid && m_UserPsi[i].tOnOff == 1) return TRUE;
	}
	if((tSid == 0x17 || tSid == 0x18 || tSid == 0x19 || tSid == 0x1a) && m_UserItem[34].sSid != -1 && m_UserItem[35].sSid != -1 && m_UserItem[36].sSid == 987 && m_UserItem[37].sSid != -1)
		return true;
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////
//	»çÀÌ¿À´Ð °ø°Ý½Ã PP Ã¼Å© ¹× °¨¼ÒÃ³¸®
//
int USER::DecAttackPP(BOOL *bSuccessSkill, BYTE tWeaponClass, short sSid, BOOL bDec)
{
	short sOldPP = GetPP();
	short sNeedPP = 0;
	short b3 = bSuccessSkill[3];	// »çÀÌ¿À´Ð ÀûÀÀ½ºÅ³ ¼º°ø¿©ºÎ

	if(sNeedPP < 0) return -1;

	if(tWeaponClass == STAFF)
	{
		sNeedPP = (short)((double)sNeedPP * ((double)((100 - (2 * b3 * m_UserSkill[tWeaponClass * SKILL_NUM + 3].tLevel + 20))) / 100));
	}
	
	if(sNeedPP < 0) return -1;
	return sNeedPP;

	if(m_sPP >= sNeedPP)
	{
		if(bDec == FALSE) return 1;

		m_sPP -= sNeedPP;
		if(m_sPP < 0) m_sPP = 0;

		if(sOldPP != m_sPP) SendPP();

		return m_sPP;
	}

	if(sOldPP != m_sPP) SendPP();
	
	return -1;
}

////////////////////////////////////////////////////////////////////////////////
//	»çÀÌ¿À´Ð ½ÃÀü½Ã¿¡ ÇÊ¿äÇÑ PP·®À» ±¸ÇÑ´Ù.
//
short USER::GetNeedPP(BOOL *bSuccessSkill, BYTE tWeaponClass, short sSid)
{
	if(sSid < 0 || sSid >= TOTAL_PSI_NUM) return -1;
	if(sSid >= g_arPsiTable.GetSize()) return -1;

	short sNeedPP = g_arPsiTable[sSid]->m_tNeedPP;
//	if(sSid==13 || sSid==14 || sSid==15)  sNeedPP=2000;
	int tClass = tWeaponClass * SKILL_NUM;
	int iLevel = 0;
	int iSkillSid = 0;
	// PP ¼Ò¸ð·® °¨¼Ò ½ºÅ³¿¡ ÀÇÇØ¼­ °¨¼ÒµÉ °ª ±¸ÇÏ±â
	int iStaff = 0;
	if(tWeaponClass == STAFF) iStaff = 1;	// ÁöÆÎÀÌ¸¦ ¾²°í ÀÖ³ª?

	short sDecPP = 0;
	
	if(iStaff == 1)
	{
		for(int i = tClass; i < tClass + SKILL_NUM; i++)
		{
			iSkillSid  = m_UserSkill[i].sSid;	// ? ½ºÅ³¸¦ À§ÇØ
			if(iSkillSid == SKILL_PP_DOWN) // PP ¼Ò¸ð·® °¨¼Ò		6 index
			{
				// ¼ø¼ö ½ºÅ³ ·¹º§ 
				iLevel = m_UserSkill[i].tLevel;		
				if(iLevel < 0) iLevel = 0;
				
				// ¾ÆÀÌÅÛ¿¡ ÀÇÇÑ ½ºÅ³ º¯µ¿ ·¹º§
				if(iLevel >= 1) iLevel += m_DynamicUserData[g_DynamicSkillInfo[iSkillSid]]+ m_DynamicUserData[MAGIC_ALL_SKILL_UP];
				
				if(iLevel >= SKILL_LEVEL) iLevel = SKILL_LEVEL - 1;
				if(iSkillSid >= g_arSkillTable.GetSize()) return 0;
				sDecPP += g_arSkillTable[iSkillSid]->m_arRepair.GetAt(iLevel);
			}
		}
	}

	sNeedPP = (short)((double)sNeedPP * (double)(1 - ((double)(sDecPP) / 100)) + 0.5);

	sSid = -1;
	sSid = m_UserItem[RIGHT_HAND].sSid;
	if(sSid >= 0 && sSid < g_arItemTable.GetSize())
	{
		sDecPP = 0;
		sDecPP = g_arItemTable[sSid]->m_byMPP;		// ¾ÆÀÌÅÛ¿¡ ÀÇÇÑ PP¼Ò¸ð·®À» °¨¼Ò½ÃÅ²´Ù.
		sNeedPP -= sDecPP;
	}

	if(sNeedPP <= 0) sNeedPP = 2;


	return sNeedPP;
}

////////////////////////////////////////////////////////////////////////////////
//	»çÀÌ¿À´Ð »çÁ¤ °Å¸®¸¦ ±¸ÇÑ´Ù.
//
int USER::GetPsiRange(short sSid)
{
	if(sSid < 0 || sSid >= TOTAL_PSI_NUM) return -1;
	if(sSid >= g_arPsiTable.GetSize()) return -1;

	int nRange = g_arPsiTable[sSid]->m_tRange;

	return nRange;
}

/////////////////////////////////////////////////////////////////////////////
//	»çÀÌ¿À´Ð Ä³½ºÆÃ µô·¹ÀÌ¸¦ Ã¼Å©ÇÑ´Ù.
//
BOOL USER::CheckPsiCastingDelay()
{
	DWORD dwCurr = GetTickCount();
	DWORD dwDiff = dwCurr - m_dwLastPsiAttack;

	if(dwDiff < m_dwCastDelay) return FALSE;
	else 
	{
		m_dwCastDelay = 0;
		return TRUE;
	}
}

////////////////////////////////////////////////////////////////////////////////
//	»çÀÌ¿À´Ð ¼º°ø ¿©ºÎ¸¦ °áÁ¤ÇÑ´Ù.
//
//BOOL USER::IsSuccessPsi(BOOL *bSuccessSkill, BYTE tWeaponClass, short sSid)
//{
//	if(sSid < 0 || sSid >= TOTAL_PSI_NUM) return FALSE;
//
//	int iRandom = XdY(1, 100);
//
//	int iStaff = 0;
//	if(tWeaponClass == STAFF) iStaff = 1;
//
//	int iRegi		= g_arPsiTable[sSid]->m_tRegi;			// ÀúÇ× °¡´É ¿©ºÎ
//	DWORD dwCast	= g_arPsiTable[sSid]->m_sDelayTime;		// Ä³½ºÆÃ µô·¹ÀÌ
//
//	int iLevel = 0;
//	if(iStaff == 1 && bSuccessSkill[2] == TRUE) iLevel = m_UserSkill[tWeaponClass * SKILL_NUM + 2].tLevel; // ÁýÁß½ºÅ³ ·¹º§
//
//	int iComp = (int)((double)m_sVOL * 0.7 + bSuccessSkill[2] * (2 * iLevel + 20) + 8 - iRegi * ((double)m_sMagicVOL / 5));
//
//	if(iRandom <= iComp) 
//	{
//		if(bSuccessSkill[4] == TRUE && iStaff == 1)			// ½Å¼Ó ½ÃÀü ½ºÅ³
//			m_dwCastDelay = dwCast / 2;
//		else
//			m_dwCastDelay = dwCast;
//
//		m_dwLastPsiAttack = GetTickCount();
//		return TRUE;
//	}
//
//	return FALSE;
//}

////////////////////////////////////////////////////////////////////////////////////
//Ä§·¨¹¥»÷	Ä§·¨ÉËº¦¼ÆËã....
BOOL USER::PsionicProcess(int nTarget, short sSid, CPoint pt, short sNeedPP)
{
	if( m_ZoneIndex < 0 || m_ZoneIndex >= g_zone.GetSize() ) return FALSE;

	USER* pUser = NULL;
	CNpc* pNpc = NULL;

	BOOL bIsUser = FALSE;

	// skill
	double dAttack = 0;			// »çÀÌ¿À´Ð ¼º°ø·ü Áõ°¡
	double dAssault = 0;		// ¸Í°ø
	double dABDefense = 0;		// Àý´ë ¹æ¾î
	double dPsyResist = 0;		// »çÀÌ¿À´Ð ÀúÇ×
	double dnewAssault = 0;

	// »çÀÌ¿À´Ð
	double dMightyWeapon = 1;	// Mighty Weapon

	int    iAssaultLevel = 0;
    int    iRandom = 0;

	// Item
	double dItemResist = 0;	// ¾ÆÀÌÅÛÀÇ ¸¶¹æ + »çÀÌ¿À´Ð ½¯µå ÀúÇ×·Â
	int	iPsiAttackUp = 0;	// ÁöÆÎÀÌ¿¡ ´Þ·ÁÀÖ´Â »çµ©
	int iUpCount = 0;		// ¾÷±×·¹ÀÌµå Ä«¿îÆ®

	double dDamage = 0;			// ±âº» µ¥¹ÌÁö
	double dDefense = 0;		// ¹æ¾î·Â
	double dWideRangeAttackDamage = 0;
	int iExp = 0;

	DWORD dwExp = 0;

	BYTE tWeaponClass = 0;
	CPoint ptTel;

	CString strMsg = _T("");

	TCHAR szTempBuf[1024];	::ZeroMemory(szTempBuf, sizeof(szTempBuf));
	int index =0;
	int iRepeat = 0;
	int iEvadeRange = 14;
	int iEvadeRandom = 1;

	int iTime = 1;

	DWORD dwCurrTime = GetTickCount();

	MAP* pMap = g_zone[m_ZoneIndex];
	if(!pMap) return FALSE;

	if(nTarget >= USER_BAND && nTarget < NPC_BAND)	// USER
	{
		pUser = GetUser(nTarget - USER_BAND);									// User Pointer ¸¦ ¾ò´Â´Ù.
		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED)return FALSE;	// Àß¸øµÈ USER ÀÌ¸é ¸®ÅÏ
		if(pUser->m_bLive == USER_DEAD)	return FALSE;							// Target User °¡ ÀÌ¹Ì Á×¾îÀÖÀ¸¸é ¸®ÅÏ

		bIsUser = TRUE;
	}
	else if(nTarget >= NPC_BAND)				// NPC
	{
		pNpc = GetNpc(nTarget - NPC_BAND);				// NPC Point ¸¦ ¾ò´Â´Ù.
		if(pNpc == NULL) return FALSE;					// Àß¸øµÈ NPC ÀÌ¸é ¸®ÅÏ
		if(pNpc->m_NpcState == NPC_DEAD) return FALSE;	// NPC °¡ ÀÌ¹Ì Á×¾î ÀÖÀ¸¸é ¸®ÅÏ
		if(pNpc->m_sHP <= 0) return FALSE;
		if(pNpc->m_tGuildWar == GUILD_WAR_AFFTER)
		{
			if(pNpc->m_tNpcType == NPCTYPE_GUILD_NPC || pNpc->m_tNpcType == NPCTYPE_GUILD_DOOR) return FALSE;
		}
		if(pNpc->m_tNpcType != NPCTYPE_MONSTER)
		{
			if(pNpc->m_tGuildWar != GUILD_WARRING) return FALSE;	
		}
	}

	if(sSid < 0 || sSid >= TOTAL_PSI_NUM) return FALSE;

	short sHoldTime	= g_arPsiTable[sSid]->m_sHoldTime;
	short sBasic	= g_arPsiTable[sSid]->m_sBasic;
	short sLevelUp	= g_arPsiTable[sSid]->m_sLevelUp;
	iPsiAttackUp = iUpCount = 0;
	//----------------------------------MarkJay ÐÞ¸´·¨Ê¦ÎäÆ÷Ä§ÉËÎÊÌâ----------------------------------
	iPsiAttackUp = GetUserSpellAttack();
	iPsiAttackUp = iPsiAttackUp + sBasic;
	if(f_Markini[0]->KZM_KG == 1)
	{
		iPsiAttackUp += (int)(m_DynamicUserData[MAGIC_PSI_RESIST_UP] * f_Markini[0]->KZM_BFB / 100) + (int)((double)m_DynamicEBodyData[EBODY_VOL_TO_PSI_DAMAGE] / 100 * (double)m_sMagicVOL);
	}
	iPsiAttackUp = (int)(iPsiAttackUp * 0.8);//ÔöÇ¿·¨Ê¦¹¥»÷Ä¬ÈÏ0.6 MarkJay
	switch(sSid)	// °ø°Ý »çÀÌ¿À´Ð Ã³¸®
	{
	case 23://ÎïÀíÖ°Òµ±ØÉ±.
	case 25:
	case 26: //±ØÉ±ÉËº¦
		EBODY_Skill(sSid,pUser,pNpc,bIsUser);
		break;
	case 2: // ¹í»ð
	case 4:	// ÖÎÁÆ
	case 8: // ¿ñÀ× 8¸ö¸ñËðÉË
	case 5:	// ×ÆÑæ
	case 10:// ¹âÖ®½£
	case 24://·¨Ê¦±ØÉ±.
#ifdef _ACTIVE_USER
//	if(m_iDisplayType == 6 && m_sLevel > 25) return FALSE; //yskang 0.5
	if(m_iDisplayType == 6) return FALSE; //yskang 0.5
#endif
	double EBODYSkill=1;
	
		dAttack		= GetPsiAttackUp();		// È¡µÃ·¨Ê¦¾«ÐÞÉË..
		dDamage  = (double)( 0.5 + m_sLevel * ((double)sLevelUp / 100)+m_sMagicVOL +iPsiAttackUp);	// ±âº» µ¥¹ÌÁö
		if(sSid != 24) dAssault	= GetPsyAssault(FALSE);//ÆÕÍ¨Ä§·¨¹¥»÷
		else
		{
			dAssault = GetPsyAssault(true);//±ØÉ±Ä§·¨ ¹¥»÷.Ôö¼Ó10%ÉËº¦
			EBODYSkill= f_Markini[0]->Fs_EBODYSkill*0.1;//0.7;//·¨Ê¦±ØÉ±Ä§·¨¹¥»÷
		}
		dDamage = (dDamage + (double)( dDamage * (double)( dAttack * 0.01 ) )* (double)( dAssault * 0.01 ))*EBODYSkill;
		dDamage = dDamage + dDamage*(myrand(1,100)%10)/100;
		//×îÖÕÉËº¦Ôö¼Ó¼ÆËã
		dDamage = dDamage + m_DynamicMagicItem[7];
		if(m_dwMightyWeaponTime != 0) dMightyWeapon = (double)( (double)(dDamage) * 0.1);
		dWideRangeAttackDamage = dDamage;
		TRACE( "Ä§·¨¹¥»÷ %lf\n", dDamage);//??
		if(bIsUser)
		{
			if(!m_MItemLock && f_Markini[0]->JYMM_KG == 1 && (!IsCity()))
			{
				SendSysWhisperChat("¶Ô²»Æð!½»Ò×ÃÜÂëÉÐÎ´½â³ý,ÎÞ·¨¹¥»÷¶Ô·½.");
				return FALSE;
			}
			if(m_GongXiFaCai == 1 && m_dwGuild != -1)//±ÜÃâÎóÉË MarkJay
			{	
				if(m_curz != 40 && m_curz != 43 && m_curz != 44 && m_curz != 45)
				{
					if(pUser->m_dwGuild == m_dwGuild && !IsCity()) return FALSE;
				}
			}
			if(m_dwCKMiMa == 0 && m_dwGuarDianTianShi == 0)
			{
				SendSystemMsg("Çë³äÖµ,Ã»ÓÐÓÎÏ·Ê±¼äÎÞ·¨¿ªPK.", SYSTEM_ERROR, TO_ME );
				return FALSE;
			}
			if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return FALSE;//MarkJay ÐÞ¸Ä
			if( pUser->m_strUserID == m_strUserID ) return FALSE;
			if( dwCurrTime - pUser->m_dwLastPsiDamagedTime < PSI_DAMAGE_DELAY) return FALSE;

			dABDefense	= pUser->GetPsyAbsoluteDefense();	 //¾ø¶Ô·ÀÓù		//Ä§·¨¿¹²ð
			dItemResist = 50 + pUser->GetUserSpellDefence();//¼ÆËãÍæ¼Ò×Ô¼ºµÄÄ§·¨·ÀÓù
			// »çÀÌ¿À´Ð ÀúÇ×·Â
			if(pUser->m_dwPsiShieldTime != 0 || pUser->m_dwBigShieldTime != 0)				// ¸¶¹æ ½¯µå ÀÖÀ» ¶§
			{
				dItemResist = dItemResist * 1.5;
			}

			ResetJiaoBenCheck();//¹¥»÷Íæ¼ÒµÄÊ±ºò²»³öÑéÖ¤ MarkJay
			pUser->ResetJiaoBenCheck();

			dItemResist += dItemResist * (double)( dABDefense * 0.01 ) ;//¼¼ÄÜµÄÄ§¿¹
			dDefense = (double)( (double)(m_sMagicVOL)/(double)( m_sMagicVOL + pUser->m_sMagicVOL + dItemResist/2) );
			if(dDefense > 1) dDefense = 1.00;
			if( m_dwMightyWeaponTime != 0 )	dMightyWeapon = (double)( (double)(dDamage) * 0.1 );
			dDamage = dDamage * dDefense + iPsiAttackUp - dItemResist/2;
			dDamage=dDamage-(pUser->m_DynamicMagicItem[5]+pUser->m_DynamicMagicItem[4]);
			if(dDamage < 15) dDamage = 15;
			BOOL bIsLegal = IsLegalDefence_isdead(pUser);
			//PK´óÈü
			if(f_Markini[0]->PK_DSKG == 1)
			{
				if(m_curz == 91 && pUser->m_sHP <= (short)dDamage)
				{
					if(m_dwPKAddkill <= 100)
					{
						m_dwDN += f_Markini[0]->PK_JIANG;
						m_dwPKAddkill += 1;
						UpdateUserItemDN();
						SendMoneyChanged();
						CString strMsg = _T("");
						strMsg.Format( "PK´óÈüÖÐ: %s Õ½Ê¤ÁË %s ,»ñµÃÁË %d ÓÎÏ·±Ò É±ÈËÊý: %d ." ,m_strUserID,pUser->m_strUserID,f_Markini[0]->PK_JIANG,m_dwPKAddkill);
						SendSystemMsg(strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_ANNOUNCE, TO_ALL);
					}
					else 
					{
						m_dwPKAddkill += 1;
						CString strMsg = _T("");
						strMsg.Format( "PK´óÈüÖÐ: %s Õ½Ê¤ÁË %s ,Íê³É´óÈüÉ±ÈËÊý100ÈÎÎñ,É±ÈËÊý: %d ." ,m_strUserID,pUser->m_strUserID,m_dwPKAddkill);
						SendSystemMsg(strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_ANNOUNCE, TO_ALL);
					}
				}
			}
			pUser->SetDamage((int)dDamage);	
//=====================Íæ¼ÒPKÄ§·¨¹¥»÷ÉËº¦ÖµÏÔÊ¾==============================
			int iMaxHP = pUser->m_sMagicMaxHP;
			if(m_sMagicMaxHP > m_sHP) iMaxHP = pUser->m_sMagicMaxHP;
			strMsg.Format("%s : %d / %d",pUser->m_strUserID,pUser->m_sHP,iMaxHP);
			SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
//===========================================================================
			if(pUser->m_sHP > 0)			// Á×Áö¾ÊÀº °æ¿ì Ãß°¡µ¥¹ÌÁö
			{
				if(sSid == 2) pUser->SetColdDamage();	// Cold
				if(sSid == 4) pUser->SetFireDamage();	// Fire
			}
			//if(m_dwVIPTime > 0 && f_Markini[0]->GJXY_KG == 1)//VIP²Å»áÑ£ÔÎ
			// {
			//	if(sSid == 24) 
			//	{
   //               pUser->SetConFusion();//Ñ£ÔÎ
			//	}
			//}
			else if(pUser->m_lDeadUsed == 1)
			{
				int type = 0;
        		type = CheckInvalidMapType();
				if(m_tFortressWar == GUILD_WARRING || m_tGuildWar == GUILD_WARRING) //Ö»ÓÐÉóÇë¾üÍÅ²ÅµôÊÐÃñµÈ¼¶	
        		if(type == 8 || type == 10 || type == 15 || type == 9 || type == 11 || type == 16)  bIsLegal = TRUE; 
				int tempRank = m_sCityRank + CITY_RANK_INTERVAL;
				if(bIsLegal == FALSE)//yskang 0.7 Á¤´ç ¹æÀ§¶ó¸é cityrank´Â Ã³¸® ÇÏÁö ¾Ê´Â´Ù.
					IsChangeCityRank(m_sCityRank, pUser);
				pUser->GetLevelDownExp(USER_PK, tempRank, FALSE,m_strUserID);
				strMsg.Format( IDS_USER_ATTACK_FROM, m_strUserID,pUser->m_strUserID);//"Äã±» %s É±º¦ÁË."
				pUser->SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE, TO_ALL);
				CString msg =_T("");
				msg.Format("Äã±» %s É±º¦ÁË.",m_strUserID);
				pUser->SendSysWhisperChat(msg.GetBuffer(0));
				//PKÉ±ËÀÍ·¶¥ÏÔÊ¾ MarkJay
				CBufferEx PotionBuf;
				PotionBuf.Add(USE_POTION);
				PotionBuf.Add(m_uid + USER_BAND);
				PotionBuf.Add((BYTE)14);
				SendInsight(PotionBuf, PotionBuf.GetLength());
			}
			if(abs(m_sLevel - pUser->m_sLevel) >= f_Markini[0]->XHBH_DJ)//¼ÓÈëÐÂÊÖ±£»¤ÅÐ¶Ï	
			{
				if(pUser->m_bPkStatus == FALSE || m_sCityRank < 0)
				{
					if(m_curz != 40 && m_curz != 43 && m_curz != 44 && m_curz != 45 && m_curz != 58 && m_curz != 61 && m_curz != 62 && m_curz != 63 && m_iHair != 179 && m_curz != 91)
					{
						Closeuser(2);
						TownPotal();
						CString strMsg = _T("");
						strMsg.Format( "%s É±Ð¡ºÅ±»¹Ø½ø¼àÓü2·ÖÖÓ!", m_strUserID);
						SendSystemMsg(strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_ANNOUNCE, TO_ALL);
					}
				}
			}
			if(sSid == 8) 
			{
				GetWideRangeAttack(pUser->m_curx, pUser->m_cury, (int)dWideRangeAttackDamage);	//¿ñÀ×ÖÜÎ§ÉËº¦
			}

			::InterlockedExchange((LPLONG)&pUser->m_dwLastPsiDamagedTime, (LONG)dwCurrTime);
		}
		else	// NPC
		{
			if(pNpc->m_tNpcType != NPCTYPE_MONSTER)
			{
				if(pNpc->m_tGuildWar != GUILD_WARRING) return FALSE;	
			}
			if(m_dwCKMiMa == 0 && m_dwGuarDianTianShi == 0)
			{
				SendSystemMsg("Çë³äÖµ,Ã»ÓÐÓÎÏ·Ê±¼äÎÞ·¨´ò¹Ö.", SYSTEM_ERROR, TO_ME );
				return FALSE;
			}
			dDamage = dDamage * ((double)myrand(80, 110) / 100);

			//¿ªÆô¹ÖÎï·ÀÓùÉúÐ§ MarkJay
			if(m_byClass == 1)
			{
				dDamage -= pNpc->m_sVOL;
			}
			else
			{
				dDamage -= pNpc->GetDefense();
			}

			if(dDamage < 15) dDamage = 15;
			if(pNpc->SetDamage((int)dDamage, m_uid + USER_BAND, m_pCom) == FALSE)
			{
				pNpc->SendExpToUserList(m_pCom); // °æÇèÄ¡ ºÐ¹è!!
				pNpc->SendDead(m_pCom);

				if(m_tGuildHouseWar == GUILD_WARRING && pNpc->m_NpcVirtualState == NPC_WAIT)
				{
					CheckGuildHouseWarEnd();
				}
				else
				{
					if(m_tQuestWar == GUILD_WARRING) CheckQuestEventZoneWarEnd();

					int diffLevel = abs(m_sLevel - pNpc->m_byClassLevel);
					if(diffLevel < f_Markini[0]->iZdJyxz)
					{
						m_iCityValue=m_iCityValue+250;
						GetCityRank();
						CheckMaxValue(m_dwXP, f_Markini[0]->idwXP);		// ¸÷ÀÌ Á×À»¶§¸¸ 1 Áõ°¡!	
						SendXP();
					}
				}
			}
			else								// Á×Áö¾ÊÀº °æ¿ì Ãß°¡µ¥¹ÌÁö
			{
				if(sSid == 2) pNpc->SetColdDamage();	// Cold
			}
//=============Íæ¼ÒÄ§·¨Èº¹¥=================
	if(f_Markini[0]->QunGong_KG == 1)
	{
        int Damage = 0;//ÓÃÓÚÈº¹¥ÉËº¦Öµ±äÁ¿
		if(sSid == 2 || sSid == 4 || sSid == 5 || sSid == 8)
	    {
			//if((m_UserItem[4].sSid == 698 && m_UserItem[4].tMagic[5] >= 10))
			if(m_curz != 92 && m_UserItem[4].sSid != -1 && m_UserItem[4].tMagic[5] >= 10)
			{
				int iRandom = myrand(0,100);			
				if(iRandom > 45 && iRandom < 54) 
				{
					if(m_byClass == 1)
					{
						Damage = GetUserSpellAttack();
						if(GetQunGongAttack(m_curx,m_cury,Damage) >= 1)
						{
							SendSystemMsg("ÌìÉñÖ®Å­´ÍÓèÈ«ÆÁÄ»¹¥»÷!", SYSTEM_NPC, TO_ME);
						}
					}
				}
			}
		}
	}
//==========================================
			//======================¹ÖÎï°Ù·Ö±ÈÉËº¦ÖµÏÔÊ¾===============================
			CString strMsg1 =_T("");
			double r = (pNpc->m_sHP * 100.0 / pNpc->m_sMaxHP);	
			CBufferEx TempBuf;
			TempBuf.Add(CHAT_RESULT);
			TempBuf.Add(SUCCESS);
			TempBuf.Add(NORMAL_CHAT);
			TempBuf.Add(pNpc->m_sNid + NPC_BAND);
			TCHAR tempName[50];
			sprintf(tempName,"(Lv.%d)%s",pNpc->m_byClassLevel,pNpc->m_strName);
			TempBuf.AddString(tempName);
			strMsg1.Format("%.2f%%", r);
			TempBuf.AddString((LPTSTR)(LPCTSTR)strMsg1);
			SendInsight(TempBuf, TempBuf.GetLength());
			//======================
			if(sSid == 8) 
			{
				GetWideRangeAttack(pNpc->m_sCurX, pNpc->m_sCurY, (int)dWideRangeAttackDamage);	//¹üÀ§ »çÀÌ¿À´Ð¿¡ÀÇÇÑ °ø°Ý Ã³¸®
			}
		}
		break;

	}

	switch(sSid)
	{
	// °ÝÅõ°è¿­ -------------------------------------------------------------------------------//
	case 0:	//Recovery	: SP È¸º¹
		if(bIsUser) 
		{
			pUser->m_sSP += (int)(0.5 + sBasic + m_sLevel * ((double)sLevelUp / 100));	// jjs07 2001.11.23 
			//pUser->m_sSP += 20 + m_sMagicVOL / 4;		// old
			if(pUser->m_sSP > pUser->m_sMagicMaxSP) pUser->m_sSP = pUser->m_sMagicMaxSP;
			pUser->SendSP();
		}
		// NPC ÀÎ °æ¿ì´Â ÇöÀç±îÁö´Â ¹«½Ã
		else return FALSE;
		break;

	case 1:	// ÇìÀÌ½ºÆ®	: °ø°Ý¼Óµµ Çâ»ó (°ø°Ý µô·¹ÀÌ °¨¼Ò)
		if(bIsUser) 
		{
			iTime = (int)(0.5 + sHoldTime + m_sLevel * ((double)sLevelUp / 100));
			pUser->SetHaste(iTime);
		}
		// NPC ÀÎ °æ¿ì´Â ÇöÀç±îÁö´Â ¹«½Ã
		else return FALSE;
		break;

	case PSIONIC_FAST_RUN:	// Fast Run	: ÀÌµ¿¼Óµµ Çâ»ó
		if(bIsUser) 
		{
			iTime = (int)(0.5 + sHoldTime + m_sLevel * ((double)sLevelUp / 100));
			pUser->SetFastRun(iTime);
		}
		// NPC ÀÎ °æ¿ì´Â ÇöÀç±îÁö´Â ¹«½Ã
		else return FALSE;
		break;

	// ÁöÆÎÀÌ °è¿­ ----------------------------------------------------------------------------//
	case 3:	// Èú¸µ
		if(bIsUser) 
		{
			
			pUser->m_sHP += (int)(0.5 + sBasic + m_sLevel * ((double)sLevelUp / 100));	// jjs07 2001.11.23 
			if(pUser->m_sHP > pUser->m_sMagicMaxHP) pUser->m_sHP = pUser->m_sMagicMaxHP;
			pUser->SendHP();
		}
		// NPC ÀÎ °æ¿ì´Â ÇöÀç±îÁö´Â ¹«½Ã
		else return FALSE;

		break;

	case 6:	// ½¯µå
		if(bIsUser) 
		{
			iTime = (int)(0.5 + sHoldTime + m_sLevel * ((double)sLevelUp / 100)); // jjs07 2001.11.24
			pUser->SetShield(iTime);	// old
			//pUser->SetShield(m_sMagicVOL);
		}
		// NPC ÀÎ °æ¿ì´Â ÇöÀç±îÁö´Â ¹«½Ã
		else return FALSE;
		break;


	case 30:	// ½¯µå
		if(bIsUser) 
		{
			iTime = (int)(0.5 + sHoldTime + m_sLevel * ((double)sLevelUp / 100)); // jjs07 2001.11.24
			pUser->SetBigShield(iTime);	// old
			//pUser->SetShield(m_sMagicVOL);
		}
		else return FALSE;
		break;

	case 7: // ÅÚ·¹Æ÷Æ® 
		iRepeat = 0;
//		ptTel = ConvertToServer(pt.x, pt.y);
//		if(ptTel.x < 0 || ptTel.y < 0) return FALSE;

		do
		if(m_curz == 1001)
		{
			 SendSystemMsg("ËÀÍöÁìµØÇøÓò,²»¿ÉÒÔÊ¹ÓÃË²ÒÆ.",SYSTEM_NPC,TO_ME);
			 break;
		}
		else
		{
//			type = ((g_zone[m_ZoneIndex]->m_pMap[ptTel.x][ptTel.y].m_dwType & 0xFF00 ) >> 8);
//			if(!CheckInvalidZoneState(type)) { iRepeat++; continue; }

//			if(Teleport(ptTel.x, ptTel.y) == FALSE) iRepeat++;
			if(Teleport(pt.x, pt.y) == FALSE) iRepeat++;
			else break;

		}while(iRepeat < 2);

		break;

	case PSIONIC_MIND_SHOCK:	// Mind Shock
		if(bIsUser) 
		{
			iTime = (int)(0.5 + sHoldTime + m_sLevel * ((double)sLevelUp / 100));
//			if(pUser->SetMindShock(iTime) == TRUE) SendSystemMsg(IDS_MIND_SHOCK_SUCCESS, SYSTEM_NORMAL, TO_ME );
			if(pUser->SetMindShock(iTime) == TRUE) SendSystemMsg("ÐÄÁé·ç±©Ê¹ÓÃ³É¹¦.", SYSTEM_NORMAL, TO_ME );
		}

		break;

	case PSIONIC_MIND_GUARD:	// Mind Guard
		if(bIsUser) 
		{
			iTime = (int)(0.5 + sHoldTime + m_sLevel * ((double)sLevelUp / 100));
			pUser->SetMindGuard(iTime);
		}

		break;

	// µµ°Ë°è¿­ ------------------------------------------------------------------------------//
	case 9:	// Ecasion	: ¼ø°£È¸ÇÇ
		/*if(CheckInvalidMapType() == 12 && m_byClass == 2)//µ¥ÈË¸ñ¶·Èü MarkJay
		{
			SendSystemMsg("»î¶¯ÇøÓò,²»¿ÉÒÔÊ¹ÓÃÉÁ±Ü.",SYSTEM_NPC,TO_ME);
			break;
		}*/
		if(m_curz == 1001)
		{
			 SendSystemMsg("ËÀÍöÁìµØÇøÓò,²»¿ÉÒÔÊ¹ÓÃÉÁ±Ü.",SYSTEM_NPC,TO_ME);
			 break;
		}
		if(bIsUser)	// USER
		{
			int type = 0;
			if(pUser->m_strUserID != m_strUserID) return FALSE;	// ÀÚ±âÀÚ½ÅÀÏ °æ¿ì¿¡¸¸
			
			iRepeat = 0;
			iEvadeRandom = myrand(1, 5);
			do
			{
				int min_x = m_curx - iEvadeRange * iEvadeRandom; if(min_x < 0) min_x = 0;
				int max_x = m_curx + iEvadeRange * iEvadeRandom; if(max_x >= pMap->m_sizeMap.cx) max_x = pMap->m_sizeMap.cx - 1;
				int min_y = m_cury - iEvadeRange * iEvadeRandom; if(min_y < 0) min_y = 0;
				int max_y = m_cury + iEvadeRange * iEvadeRandom; if(max_y >= pMap->m_sizeMap.cy) max_y = pMap->m_sizeMap.cy - 1;
				
				int move_x = 0, move_y =0;
				
				move_x = myrand(min_x, max_x);
				move_y = myrand(min_y, max_y);
	
				type = ((g_zone[m_ZoneIndex]->m_pMap[move_x][move_y].m_dwType & 0xFF00 ) >> 8);
					
				if(!CheckInvakidZoneChangeState(type)) { iRepeat++; continue; }

				if(Teleport(move_x, move_y) == FALSE) iRepeat++;
				else break;
				
			}while(iRepeat < 6);
		}
		else return FALSE;
		break;

	case PSIONIC_PSI_SHIELD:		// »çÀÌ¿À´Ð ½¯µå
		if(bIsUser) 
		{
			iTime = (int)(0.5 + sHoldTime + m_sLevel * ((double)sLevelUp / 100)); // jjs07 2001.11.24
			pUser->SetPsiShield(iTime);
		}
		else return FALSE;
		break;

	// ÃÑ±â -------------------------------------------------------------------------------------------//
	case 11:	// Dex Up
		if(bIsUser) 
		{
			iTime = (int)(0.5 + sHoldTime + m_sLevel * ((double)sLevelUp / 100)); // jjs07 2001.11.24
			pUser->SetDexUp(iTime);
		}
		else return FALSE;
		break;

	case 12:	// Max HP Up
		if(bIsUser) 
		{
			iTime = (int)(0.5 + sHoldTime + m_sLevel * ((double)sLevelUp / 100));
			pUser->SetMaxHPUp(iTime);
		}
		else return FALSE;
		break;

	case PSIONIC_PIERCING_SHIELD:	// ½¯µå ¶Õ±â
		if(bIsUser) 
		{
			iTime = (int)(0.5 + sHoldTime + m_sLevel * ((double)sLevelUp / 100)); // jjs07 2001.11.24
			pUser->SetPiercingShield(iTime);
		}
		else return FALSE;

		break;

	// ¹«°è¿­ -----------------------------------------------------------------------------------------//
	case 13:	// Adamantine
		if(bIsUser)
		{
#ifdef _ACTIVE_USER
//	if(m_iDisplayType == 6 && m_sLevel > 25) return FALSE; //yskang 0.5
	if(m_iDisplayType == 6) return FALSE; //yskang 0.5
#endif
			iTime = (int)(0.5 + sHoldTime + m_sLevel * ((double)sLevelUp / 100 ));	// alisia 2002.03.21
			pUser->SetAdamantine( iTime );
		}
		else return FALSE;
		break;

	case 14:	// MightyWeapon
		if(bIsUser)
		{
#ifdef _ACTIVE_USER
//	if(m_iDisplayType == 6 && m_sLevel > 25) return FALSE; //yskang 0.5
	if(m_iDisplayType == 6) return FALSE; //yskang 0.5
#endif
			iTime = (int)(0.5 + sHoldTime + m_sLevel * ((double)sLevelUp / 100 ));	// alisia 2002.03.21
			pUser->SetMightyWeapon( iTime );
		}
		else return FALSE;
		break;

	case 15:	// Berserker
		if(bIsUser)
		{
#ifdef _ACTIVE_USER
//	if(m_iDisplayType == 6 && m_sLevel > 25) return FALSE; //yskang 0.5
	if(m_iDisplayType == 6) return FALSE; //yskang 0.5
#endif
			iTime = (int)(0.5 + sHoldTime + m_sLevel * ((double)sLevelUp / 100 ));	// alisia 2002.03.21
			pUser->SetBerserker( iTime );///ymc
		}
		else return FALSE;
		break;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////
//ÎïÀí±ØÉ±ÉËº¦¼ÆËã
void USER::EBODY_Skill(short sSid,USER *pUser ,CNpc* pNpc,BOOL bIsUser)
{
	BOOL	bCanUseSkill = FALSE;
	BOOL	bIsCritical = FALSE;
	byte tWeaponClass;
	int nDamage,nFinalDamage,nDefense;
	double EBODYSkill= f_Markini[0]->Wl_EBODYSkill*0.1;//0.9;
	CString strMsg;
	
	 bCanUseSkill = IsCanUseWeaponSkill(tWeaponClass);	
	 if(!bCanUseSkill) return;//Èç¹ûÃ»ÓÐ57 Ôò·µ»Ø

	 //========================ÎïÀíÖ°Òµ±ØÉ±¹¥»÷
	if(m_byClass == 0) EBODYSkill = 0.7;//È­
	if(m_byClass == 2)  EBODYSkill = 0.7;  //µ¶
	if(m_byClass == 3)  EBODYSkill = 0.7;  //Ç¹
	int rand = myrand(1,100);
	if(myrand(1,100) >= 90) EBODYSkill = 0;	//±ÈÉ±µÄ»úÂÊ
 //   if(m_dwVIPTime > 0  && f_Markini[0]->GJXY_KG == 1)//VIP²Å»áÑ£ÔÎ
	//{
	//	if(sSid == 23 || sSid == 25 || sSid == 26)
	//	{
	//		pUser->SetConFusion();//Ñ£ÔÎ
	//	}
	//}
	if(bIsUser)	// USER
	{
		if(!m_MItemLock && f_Markini[0]->JYMM_KG == 1 && (!IsCity()))
		{
			SendSysWhisperChat("¶Ô²»Æð!½»Ò×ÃÜÂëÉÐÎ´½â³ý,ÎÞ·¨ÊÍ·Å±ØÉ±.");
			return;
		}
		if(pUser == NULL) return;//MarkJay ÐÞ¸Ä
		//¼ÆËã·À ----------------------------------------------------------------//
		nDamage =(int)( GetNormalInitDamage(tWeaponClass, pUser, bIsCritical,true)*EBODYSkill);//¼ÆËã×Ü¹¥»÷.
		if(nDamage < 0) nDamage = 0;
		if(pUser->m_tAbnormalKind == ABNORMAL_BYTE_COLD) nDamage += 10;
		nFinalDamage = GetFinalDamage(pUser, nDamage, tWeaponClass,bIsCritical,1);//¼ÆËã×îÖÕÉËº¦
		//×îÖÕÉËº¦Ôö¼Ó¼ÆËã  ºÍ×îÖÕÉË¼õÉÙ
		nFinalDamage = nFinalDamage + m_DynamicMagicItem[7];
		nFinalDamage = nFinalDamage - (pUser->m_DynamicMagicItem[5] + pUser->m_DynamicMagicItem[6]);
		//________________________________________________
		if(nFinalDamage < 0) nFinalDamage = 0;
		BOOL bIsLegal = IsLegalDefence_isdead(pUser);	
		pUser->SetDamage(nFinalDamage);
//=====================Íæ¼ÒÊÍ·Å±ØÉ±ÉËº¦ÖµÏÔÊ¾==============================
		int iMaxHP = pUser->m_sMagicMaxHP;
		if(m_sMagicMaxHP > m_sHP) iMaxHP = pUser->m_sMagicMaxHP;
		strMsg.Format("%s : %d / %d",pUser->m_strUserID,pUser->m_sHP,iMaxHP);
		SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
//=========================================================================
		if(pUser->m_lDeadUsed == 1)
		{
			int type = 0;
        	type = CheckInvalidMapType();
			if(m_tFortressWar == GUILD_WARRING || m_tGuildWar == GUILD_WARRING) //Ö»ÓÐÉóÇë¾üÍÅ²ÅµôÊÐÃñµÈ¼¶	
        	if(type == 8 || type == 10 || type == 15 || type == 9 || type == 11 || type == 16)  bIsLegal = TRUE; 
			int tempRank = m_sCityRank + CITY_RANK_INTERVAL;
			if(bIsLegal == FALSE)//yskang 0.7 Á¤´ç ¹æÀ§¶ó¸é cityrank´Â Ã³¸® ÇÏÁö ¾Ê´Â´Ù.
				IsChangeCityRank(m_sCityRank, pUser);
			pUser->GetLevelDownExp(USER_PK, tempRank, FALSE,m_strUserID);			// °æÇèÄ¡¿Í ±×¿Ü º¯È­·®¸¦ ¹Ý¿µÇÑ´Ù.
			
			strMsg.Format( IDS_USER_ATTACK_FROM, m_strUserID,pUser->m_strUserID);//"Äã±» %s É±º¦ÁË."
			pUser->SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_ANNOUNCE, TO_ALL);
			CString msg =_T("");
			msg.Format("Äã±» %s É±º¦ÁË.",m_strUserID);
			pUser->SendSysWhisperChat(msg.GetBuffer(0));
			//PKÉ±ËÀÍ·¶¥ÏÔÊ¾ MarkJay
			CBufferEx PotionBuf;
			PotionBuf.Add(USE_POTION);
			PotionBuf.Add(m_uid + USER_BAND);
			PotionBuf.Add((BYTE)14);
			SendInsight(PotionBuf, PotionBuf.GetLength());
		}
		if(abs(m_sLevel - pUser->m_sLevel) >= f_Markini[0]->XHBH_DJ)//¼ÓÈëÐÂÊÖ±£»¤ÅÐ¶Ï	
		{
			if(pUser->m_bPkStatus == FALSE || m_sCityRank < 0)
			{
				if(m_curz != 40 && m_curz != 43 && m_curz != 44 && m_curz != 45 && m_curz != 58 && m_curz != 61 && m_curz != 62 && m_curz != 63 && m_iHair != 179 && m_curz != 91)
				{
					Closeuser(2);
					TownPotal();
					CString strMsg = _T("");
					strMsg.Format( "%s É±Ð¡ºÅ±»¹Ø½ø¼àÓü2·ÖÖÓ!", m_strUserID);
					SendSystemMsg(strMsg.GetBuffer(strMsg.GetLength()), SYSTEM_ANNOUNCE, TO_ALL);
				}
			}
		}
	}
	else
	{
		if(pNpc == NULL) return;//MarkJay ÐÞ¸Ä
		nDefense = pNpc->GetDefense();
		nDamage = (int)(GetNormalInitDamage(tWeaponClass, NULL,bIsCritical,true)*1.05);
		if(nDamage < 0) nDamage = 0;
		double nVCHit = 0.0;
		nVCHit = GetCriticalHit(tWeaponClass,1);
		if(nVCHit > 0.0) bIsCritical = TRUE;
		nFinalDamage = (int)((double)nDamage - ((double)nDefense * nVCHit));
		//×îÖÕÉËº¦Ôö¼Ó¼ÆËã  ºÍ×îÖÕÉË¼õÉÙ
		nFinalDamage = nFinalDamage+m_DynamicMagicItem[7];

		if(nFinalDamage < 0) nFinalDamage = 0;
		short sOldNpcHP = pNpc->m_sHP;

		if(pNpc->SetDamage(nFinalDamage, m_uid + USER_BAND, m_pCom) == FALSE)
		{
			pNpc->SendExpToUserList(m_pCom); // °æÇèÄ¡ ºÐ¹è!!
			pNpc->SendDead(m_pCom);

			if(m_tGuildHouseWar == GUILD_WARRING && pNpc->m_NpcVirtualState == NPC_WAIT)
			{
				CheckGuildHouseWarEnd();
			}
			else
			{
				if(m_tQuestWar == GUILD_WARRING) CheckQuestEventZoneWarEnd();
				int diffLevel = abs(m_sLevel - pNpc->m_byClassLevel);
				if(diffLevel < f_Markini[0]->iZdJyxz)
				{
					m_iCityValue=m_iCityValue+250;
					GetCityRank();
					CheckMaxValue(m_dwXP, f_Markini[0]->idwXP);		// ¸÷ÀÌ Á×À»¶§¸¸ 1 Áõ°¡!	
					SendXP();
				}
			}
		}
		//======================¹ÖÎï°Ù·Ö±ÈÉËº¦ÖµÏÔÊ¾===============================
		CString strMsg1 =_T("");
		double r = (pNpc->m_sHP * 100.0 / pNpc->m_sMaxHP);	
		CBufferEx TempBuf;
		TempBuf.Add(CHAT_RESULT);
		TempBuf.Add(SUCCESS);
		TempBuf.Add(NORMAL_CHAT);
		TempBuf.Add(pNpc->m_sNid + NPC_BAND);
		TCHAR tempName[50];
		sprintf(tempName,"(Lv.%d)%s",pNpc->m_byClassLevel,pNpc->m_strName);
		TempBuf.AddString(tempName);
		strMsg1.Format("%.2f%%", r);
		TempBuf.AddString((LPTSTR)(LPCTSTR)strMsg1);
		SendInsight(TempBuf, TempBuf.GetLength());
		//======================
	}
}
//
///////////////////////////////////////////////////////////////////////////
//	XP °ªÀ» º¸³½´Ù.
//
void USER::SendXP()
{
	CBufferEx TempBuf;

	TempBuf.Add(SET_XP);
	TempBuf.Add(m_dwXP);

	Send(TempBuf, TempBuf.GetLength());
}

////////////////////////////////////////////////////////////////////////////
//	¸¶À»·Î ÀÌµ¿ÇÑ´Ù.
//
void USER::TownPotal()
{
	int isZone = -1;
	int iPotalIndex = -1;
	BYTE result = SUCCESS;
	BYTE zoneChange = 0;
	CPoint ptTown, pt;
	CPoint ptPotal;
	
	CBufferEx TempBuf;

	pt = GetTownPotal(isZone,0);

	int i, rand;
	
	MAP* pMap = NULL;

	/*************************[ Back Up ]***************************************/	
	int org_z = m_curz;

	if(isZone <0) { result = FAIL; goto go_result; };	// ¾Æ´Ï¸é °°Àº Á¸¾ÈÀ¸·Î ÀÌµ¿

	if(	!IsZoneInThisServer(isZone) ) { result = FAIL; goto go_result; };

//	ZoneMoveReq( isZone, x, y );
	if( m_ZoneIndex < 0 || m_ZoneIndex >= g_zone.GetSize() ) { result = FAIL; goto go_result; };

	pMap = g_zone[m_ZoneIndex];		if( !pMap ) { result = FAIL; goto go_result; };

	if( m_curx < 0 || m_curx >= pMap->m_sizeMap.cx ) { result = FAIL; goto go_result; };
	if( m_cury < 0 || m_cury >= pMap->m_sizeMap.cy ) { result = FAIL; goto go_result; };

	m_NowZoneChanging = TRUE;					// Zone Change ÇÏ°í ÀÖ´Ù.
	/*************************[ Zone Change Init]*******************************/
	SetZoneIndex(isZone);

	ptPotal = ConvertToServer(pt.x, pt.y);		// ÀÌº¥Æ® ÁÂÇ¥´Â Å¬¶óÀÌ¾ðÆ® ±âÁØÀÌ¹Ç·Î...

	if( ptPotal.x == -1 || ptPotal.y == -1 )
	{
		SetZoneIndex(org_z);
		result = FAIL; goto go_result;
	}

	i = SEARCH_TOWN_POTAL_COUNT;				// ÁÂÇ¥¸¦ Ã£±âÀ§ÇÑ È½¼ö
	
	ptTown = FindNearAvailablePoint_S(ptPotal.x, ptPotal.y);

	if(ptTown.x == -1 || ptTown.y == -1) 
	{
		while(TRUE)								// Å¸¿îÆ÷Å» ·¥´ý ÁÂÇ¥¸¦ ¾ò´Â´Ù.
		{
			rand = myrand(-20, 20);
			ptPotal.x += rand; ptPotal.y += rand;

			ptTown = FindNearAvailablePoint_S(ptPotal.x, ptPotal.y);
			if(ptTown.x != -1 && ptTown.y != -1) break;

			i--;
			if(i <= 0) // ³ªÁß¿¡
			{ 
				ptTown.x = -1; ptTown.y = -1;
				break;
			}
		}
	}

	if(ptTown.x == -1 || ptTown.y == -1) 
	{
		SetZoneIndex(org_z);
		m_NowZoneChanging = FALSE;
		result = FAIL; goto go_result;
	}
	else									// µé¾î°¥¼ö¾ø´ÂÁö¿ªÀÌ¸é...
	{
		int type = ((g_zone[m_ZoneIndex]->m_pMap[ptTown.x][ptTown.y].m_dwType & 0xFF00 ) >> 8);
		if(!CheckInvakidZoneChangeState(type)) 
		{
			SetZoneIndex(org_z);
			m_NowZoneChanging = FALSE;
			result = FAIL; goto go_result;
		}
	}

	SetZoneIndex(org_z);					// ÀÌÀü ¼¿ À¯Àú¿¡°Ô  ¾Ë¸°´Ù.

//	if(m_tQuestWar == GUILD_WARRING) CheckQuestEventZoneWarEnd();

	if(InterlockedCompareExchange((LONG*)&g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser, 
			(long)0, (long)(m_uid + USER_BAND)) == (long)m_uid + USER_BAND)
	{
//		SetUid(m_curx, m_cury, 0);				
		SendMyInfo(TO_INSIGHT, INFO_DELETE);
		
		SetZoneIndex(isZone);						
		m_curx = ptTown.x;
		m_cury = ptTown.y;
		m_curz = isZone;
		SetUid(m_curx, m_cury, m_uid + USER_BAND );// ¾ÕÀ¸·Î ÀÌµ¿ÇÒ ÀÚ¸®¸¦ È®º¸, À§Ä¡Á¤º¸ ¼ÂÆÃ

		SendZoneChange(TRUE);	// Á¸Ã¼ÀÎÁö¸¦  ¸ÕÀúº¸³»¾ß ³ªÁß NPCÁ¤º¸°¡ client¿¡¼­ »ì¾ÆÀÖ´Ù.

		m_presx = -1;
		m_presy = -1;
		SightRecalc();
		SendMyInfo( TO_INSIGHT, INFO_MODIFY );
		
		m_NowZoneChanging = FALSE;
	}
	else
	{
		SendMyInfo(TO_INSIGHT, INFO_DELETE);

		SetZoneIndex(isZone);						
		m_curx = ptTown.x;
		m_cury = ptTown.y;
		m_curz = isZone;
		SetUid(m_curx, m_cury, m_uid + USER_BAND );// ¾ÕÀ¸·Î ÀÌµ¿ÇÒ ÀÚ¸®¸¦ È®º¸, À§Ä¡Á¤º¸ ¼ÂÆÃ

		SendZoneChange(TRUE);	// Á¸Ã¼ÀÎÁö¸¦  ¸ÕÀúº¸³»¾ß ³ªÁß NPCÁ¤º¸°¡ client¿¡¼­ »ì¾ÆÀÖ´Ù.

		m_presx = -1;
		m_presy = -1;
		SightRecalc();
		SendMyInfo( TO_INSIGHT, INFO_MODIFY );
		
		m_NowZoneChanging = FALSE;
	}

	if(result)
	{
		SendWeatherInMoveZone();					// ÀÌµ¿ Á¸ÀÇ ³¯¾¾º¯È­¸¦ ¾Ë¸°´Ù.
		SightRecalc();						// ³» ½Ã¾ß¾ÈÀÇ Á¤º¸¸¦ ³ª¿¡°Ô º¸³½´Ù.
		return;
	}

go_result:

	SendTownPotal(result, zoneChange);
}

////////////////////////////////////////////////////////////////////////////
//	Å¸¿î Æ÷Å»ÆÐÅ¶À» º¸³½´Ù.
//
void USER::SendTownPotal(BYTE result, BYTE changeZone)
{
	CBufferEx TempBuf;

	TempBuf.Add(PSI_TOWN_POTAL);
	TempBuf.Add(result);

	if(result == FAIL) Send(TempBuf, TempBuf.GetLength());

	CPoint pos = ConvertToClient( m_curx, m_cury );
	if( pos.x == -1 || pos.y == -1 ) { pos.x = 1; pos.y = 1; }	// ÀÌºÎºÐÀº ³ªÁß¿¡ ¹Ù²ÙÀÚ

	TempBuf.Add(changeZone);

	TempBuf.Add(m_uid + USER_BAND);

	TempBuf.Add((short)pos.x);
	TempBuf.Add((short)pos.y);
	TempBuf.Add((short)m_curz);

	Send(TempBuf, TempBuf.GetLength());
}

////////////////////////////////////////////////////////////////////////////
//	¼¿¿¡ Å¸¿îÆ÷Å»¸¦ ¾Ë¸°´Ù.
//
void USER::AddCellUidFromTownPotal()
{
//	g_cell[m_nCellZone]->AddCellFromTownPotal(m_ptCell, m_uid + USER_BAND);
}

////////////////////////////////////////////////////////////////////////////
//	¼¿¿¡ Å¸¿îÆ÷Å»·Î ÀÎÇÑ ¼¿ »èÁ¦¸¦ ¾Ë¸°´Ù.
//
void USER::DeleteCellUidFromTownPotal()
{
//	g_cell[m_nCellZone]->DeleteCellFromTownPotal(m_ptCell, m_uid + USER_BAND);
}
////////////////////////////////////////////////////////////////////////////
//	Ã¿ÃëÖ´ÐÐÒ»´Î
void USER::UserTimer()
{
	CString strMsg1 = _T("");
	if(m_state != STATE_GAMESTARTED) return;
	if(m_bLive == USER_DEAD) return;
	int index = 0;
	DWORD dwCurrTick = GetTickCount();
	if(dwCurrTick - m_dwUserTimerRun < 1000) return;//ÉÙÓÚ1ÃëÔò·µ»Ø
	else m_dwUserTimerRun = dwCurrTick;

	CBufferEx TempBuf;
	if(m_curz == 92 && f_Markini[0]->EM_KG == 1)//¶ñÄ§¹ã³¡ËÍ»ØÍæ¼Ò
	{
		if(f_Markini[0]->EM_ZDKG == 0)
		{
			ZoneMoveReq( 26,37,23);
		}
	}
	if(m_curz == 91)//PK´óÈüËÍ»ØÍæ¼Ò
	{
		if(f_Markini[0]->PK_DSKG == 0)
		{
			ZoneMoveReq( 26,48,46);
		}		
	}
	if(m_dwVIPTime > 0 && m_AutoChuShouFen > 0 && m_AutoChuShouFen_KG == 1 && !IsCity())
	{
		AutoYiJianChuShou();
	}
	if(GuildWarEnd == FALSE)
	{
		CGuildFortress *pFort = NULL;
		pFort = GetFortress(f_Markini[0]->JT_Fortressid);
		if(pFort->m_iGuildSid != m_dwGuild)
		{
			for(int i = 0;i < TOTAL_ITEM_NUM;i++)
			{
				if(m_UserItem[i].iItemSerial == f_Markini[0]->JT_Fortressid)
				{
					ReSetItemSlot(&m_UserItem[i]);
					TempBuf.Add(ITEM_GIVE_RESULT);
					TempBuf.Add((BYTE)0x01); 
							
					TempBuf.Add((BYTE)i);
					TempBuf.Add(m_UserItem[i].sLevel);
					TempBuf.Add(m_UserItem[i].sSid);
					TempBuf.Add(m_UserItem[i].sDuration);
					TempBuf.Add(m_UserItem[i].sBullNum);
					TempBuf.Add(m_UserItem[i].sCount);
							
					for(int j = 0; j < MAGIC_NUM; j++)
					{
						TempBuf.Add(m_UserItem[i].tMagic[j]);
						TempBuf.Add(m_UserItem[i].tIQ); 
						Send(TempBuf, TempBuf.GetLength());
					}
					SendSystemMsg( "·ÇÒªÈû¾üÍÅÎïÆ·ÒÑÇå¿Õ.", SYSTEM_ERROR, TO_ME);
				}
			}
		}
	}
	//if(m_curz == 315 && g_isBaoZangKG == FALSE)//½ûÖ¹PK¶´»Ø³Ç
	//{
	//	SendSystemMsg( "¿ª·Å½øÈëÊ±¼äÊÇ24:00-12:00£¡", SYSTEM_ERROR, TO_ME);
	//	//ZoneMoveReq( 1,158,1392);
	//	ZoneMoveReq( 26,36,40);
	//}
	//if(m_curz == 311 && m_byClass == 1)//ÎïÀí¶´ËÍ·¨Ê¦»Ø³Ç
	//{
	//	SendSystemMsg( "·ÇÎïÀíÖ°ÒµËÍ»Ø³ÇÀï.", SYSTEM_ERROR, TO_ME);
	//	ZoneMoveReq( 26,36,40);
	//}
	//if(m_curz == 317 && m_byClass != 1)//ÎïÀí¶´ËÍ·¨Ê¦»Ø³Ç
	//{
	//	SendSystemMsg( "·Ç·¨Ê¦Ö°ÒµËÍ»Ø³ÇÀï.", SYSTEM_ERROR, TO_ME);
	//	ZoneMoveReq( 26,36,40);
	//}

	if(dwCurrTick - m_ShouhuTime > 120000 && !IsCity())//ÊØ»¤¼õÄÍ¾ÃÊ±¼äÄ¬ÈÏ12000
	{
		if(m_UserItem[38].sSid != 0 && m_UserItem[38].sDuration > 1)
		{
			m_UserItem[38].sDuration -= 1;
            if(m_UserItem[38].sDuration < 0) m_UserItem[38].sDuration = 0;
			CBufferEx TempBuf1;

            TempBuf1.Add(ITEM_DURATION);
            TempBuf1.Add((BYTE)38);
            TempBuf1.Add(m_UserItem[38].sDuration);
            Send(TempBuf1, TempBuf1.GetLength());
		}
		else
		{
			m_UserItem[38].sDuration = 0;
			GetMagicItemSetting();
		}
		m_ShouhuTime = dwCurrTick;
	}
	if(f_Markini[0]->EXP_KG == 1 && IsCity() && m_sLevel >= 50)//ÅÝÐË·Ü£¬ÐÒÔË
	{
		if(dwCurrTick - m_ionlineTime >  180 * 1000 )//ÅÝÏà¸ôÊ±¼ä//Ã¿Ãë
		{
			int iNum = 60 * 1000;//ÅÝÐË·Ü
			if(m_dwHiExpTime >= 0 && m_dwHiExpTime <= 3600 * 3 * 1000) GiveHiExpTime(iNum);//ÐË·Ü
			if(m_dwMagicFindTime >= 0 && m_dwMagicFindTime <= 3600 * 3 * 1000) GiveMagicFindTime(iNum);//ÐÒÔË
			//if(m_dwFUExpTime >= 0 && m_dwFUExpTime <= 3600 * 3 * 1000) GiveFUExpTime(iNum);//ÐÒ¸£Ê±¹â
			m_ionlineTime = dwCurrTick;
		}
	}
	else
	{
		m_ionlineTime = dwCurrTick;
	}

	if(!IsMapCheck() && !IsCity() && m_bPShopOpen == FALSE && m_bNowTrading == FALSE && m_dwCloseTime == 0 && m_bSessionOnline == false)//ÑéÖ¤Âë MarkJay
	{
		if(dwCurrTick - m_AutoWaiGuaTime >  60000 * 5)//5·ÖÖÓ
		{
			m_CheckCode = myrand(1000,9999);
			m_AutoWaiGuaTime = dwCurrTick;
			UserCodeStr = _T("");
			int iRandom = myrand(0,4);
			int iRandom2 = myrand(0,2);
			if(iRandom == 0)
			{
				UserCodeStr.Format("[%d]",m_CheckCode);
			}
			if(iRandom == 1)
			{
				UserCodeStr.Format("{%d}",m_CheckCode);
			}
			if(iRandom == 2)
			{
				UserCodeStr.Format("<%d>",m_CheckCode);
			}
			if(iRandom == 3)
			{
				UserCodeStr.Format("-%d-",m_CheckCode);
			}
			if(iRandom == 4)
			{
				UserCodeStr.Format("¡¾%d¡¿",m_CheckCode);
			}
			if(m_oldx == m_curx && m_oldy == m_cury && m_oldz == m_curz)
			{
				if(m_MoveAutoTime++ >= 3)
				{
					ResetJiaoBenCheck();
					m_MoveAutoTime = 0;
				}
			}
			else
			{
				if(m_CodeFailNum < 3)
				{
					m_CodeFailNum++;
					if(iRandom2 == 0)
					{
						strMsg1.Format(" %s\r\n\r\nÇëÔÚÆÕÍ¨ÁÄÌìÖÐ·¢ËÍ\r\n\r\n½øÐÐÑéÖ¤.\r\n\r\n\r\n\r\n\r\n",(LPTSTR)(LPCTSTR)UserCodeStr);
					}
					if(iRandom2 == 1)
					{
						strMsg1.Format("ÇëÔÚÆÕÍ¨ÁÄÌìÖÐ·¢ËÍ\r\n\r\n %s\r\n\r\n½øÐÐÑéÖ¤.\r\n\r\n\r\n\r\n\r\n",(LPTSTR)(LPCTSTR)UserCodeStr);
					}
					if(iRandom2 == 2)
					{
						strMsg1.Format("ÇëÔÚÆÕÍ¨ÁÄÌìÖÐ·¢ËÍ\r\n\r\n½øÐÐÑéÖ¤.\r\n\r\n %s\r\n\r\n\r\n\r\n\r\n",(LPTSTR)(LPCTSTR)UserCodeStr);
					}
					SendSystemMsg((LPTSTR)(LPCTSTR)strMsg1, SYSTEM_CESHIA, TO_ME);
					m_oldx = m_curx;
					m_oldy = m_cury;
					m_oldz = m_curz;
					m_MoveAutoTime = 0;
				}
				else
				{
					Closeuser(2);
					SendSystemMsg(_T("¶à´ÎÃ»ÓÐ·¢ËÍÑéÖ¤Âë!\r\n\r\n±»¹Ø½ø¼àÓü!!!"),SYSTEM_SPECIAL,TO_ME);
					return;
				}
			}
		}
	}
	else
	{
		m_AutoWaiGuaTime = dwCurrTick;
	}

	//if(m_dwCloseTime == 0 && m_bSessionOnline == false)
	//{
	//	if(dwCurrTick - m_UserCardTime >  60 * 1000)//ÔÚÏßÒ»·ÖÖÓ¿ÛÒ»·ÖÖÓµã¿¨Ê±¼ä MarkJay
	//	{
	//		int Formal,Num = 0;
	//		short flag = 0;
	//		int Mark_ssid = 0;
	//		CString straccount = m_strAccount;
	//		LoadMarkPayData();
	//		for(int i = 0;i < g_arMarkPayTable.GetSize();i++)
	//		{
	//			Mark_ssid = g_arMarkPayTable[i]->m_iSid;
	//			CString m_name = g_arMarkPayTable[i]->m_iSname;
	//			if(straccount == m_name)
	//			{
	//				flag = g_arMarkPayTable[i]->m_flag;
	//				Formal = g_arMarkPayTable[i]->m_Formal;
	//				Num += Formal;
	//			}
	//		}
	//		if(flag == 1)
	//		{
	//			if(Num > 60000)
	//			{
	//				UpdateMarkPay(Mark_ssid,60000);
	//				LoadMarkPayData();
	//				UpdateUserData();
	//			}
	//			else
	//			{
	//				DeleteMarkPay(Mark_ssid);
	//				LoadMarkPayData();
	//				UpdateUserData();
	//			}
	//		}
	//		m_UserCardTime = dwCurrTick;
	//	}
	//}
	//else
	//{
	//	m_UserCardTime = dwCurrTick;
	//}

	if(m_dwCloseTime == 0 && m_bSessionOnline == false)
	{
		if(dwCurrTick - m_VIPCardTime >  60 * 1000)//ÔÚÏßÒ»·ÖÖÓ¿ÛÒ»·ÖÖÓ»áÔ±Ê±¼ä MarkJay
		{
			int Formal,Num = 0;
			short flag = 0;
			int Mark_ssid = 0;
			CString straccount = m_strAccount;
			LoadVIPPayData();
			for(int i = 0;i < g_arVIPPayTable.GetSize();i++)
			{
				Mark_ssid = g_arVIPPayTable[i]->m_iSid;
				CString m_name = g_arVIPPayTable[i]->m_iSname;
				if(straccount == m_name)
				{
					flag = g_arVIPPayTable[i]->m_flag;
					Formal = g_arVIPPayTable[i]->m_Formal;
					Num += Formal;
				}
			}
			if(flag == 1)
			{
				if(Num > 60000)
				{
					UpdateVIPPay(Mark_ssid,60000);
					LoadVIPPayData();
					UpdateUserData();
				}
				else
				{
					DeleteVIPPay(Mark_ssid);
					LoadVIPPayData();
					UpdateUserData();
				}
			}
			m_VIPCardTime = dwCurrTick;
		}
	}
	else
	{
		m_VIPCardTime = dwCurrTick;
	}

	//---------------------------------------------------
	/*if(dwCurrTick - m_dwServerTick >= 82000)
	{	
		SendSystemMsg( IDS_USER_SPEED_HACK, SYSTEM_SPECIAL, TO_ME);// "»³ÒÉÄúÊ¹ÓÃ¼ÓËÙ»òÕßÓë·þÎñÆ÷Ö®¼äÍøÂç¹ý¿¨."
		SoftClose();
		return;
	}*/
	if(m_bPkStatus)
	{
		if(m_dwPkStatusTime == 0) m_bPkStatus = FALSE;
		else if(dwCurrTick - m_dwPkStatusTime > USER_PK_TICKS)
		{
			index = 0;
			m_bPkStatus = FALSE;				// Ä«¿À»óÅÂ¿¡¼­ ¹þ¾î³².
			m_dwPkStatusTime = 0;
	
			SetByte(m_TempBuf, SET_USER_PK_STATE, index);
			SetInt(m_TempBuf, m_uid + USER_BAND, index);
			SetByte(m_TempBuf, 0x02, index);
			SetByte(m_TempBuf, m_bPkStatus, index);
			
			Send(m_TempBuf, index);
			SendInsight(m_TempBuf, index);
//			SendExactScreen(m_TempBuf, index);
		}
	}

	if(dwCurrTick - m_dwLastUpdateTime > 300000)//MarkJay ÐÞ¸Ä Ä¬ÈÏ300000
	{
		if ( m_pIocpBase )
			m_pIocpBase->SetAliveTimeUpdate( m_uid, dwCurrTick );
  
		UpdateUserData();

		m_dwLastUpdateTime = dwCurrTick;
	}
	if(dwCurrTick - m_dwLastSaveTime > 90000)//MarkJay ÐÞ¸Ä Ä¬ÈÏ90000
	{
		UpdateUserData(TRUE);
		m_dwLastSaveTime = dwCurrTick;
	}

	if(dwCurrTick - m_dwLastTimeCount < 900)
	{
		return;
	}
	
	m_dwLastTimeCount = dwCurrTick;

	// SP È¸º¹
	short sOldSP = m_sSP;
	GetSP();
	if(m_sSP != sOldSP) SendSP();

	// PP È¸º¹
	short sOldPP = m_sPP;
	GetPP();
	if(m_sPP != sOldPP) SendPP();

	// HP È¸º¹
	short sOldHP = m_sHP;
	GetHP();
	if(m_sHP != sOldHP) SendHP();

	DWORD dwAbnormalTime = m_dwAbnormalTime;
	DWORD dwRemainTime = 0;
	CString szMsg = _T("");
	DWORD dwDiffTime = 0;

	dwCurrTick = GetTickCount();

	//
	// Haste, Shield, Dex Up, Max HP Up Ã³¸® -----------------------------------------------//
	//
	if(m_dwHasteTime != 0)
	{

		if((dwCurrTick - m_dwLastHasteTime) > m_dwHasteTime)		// Haste °¡ Ç®¸± ½Ã°£ÀÌ¸é
		{
			DeleteAbnormalInfo(ABNORMAL_HASTE);
			m_dwHasteTime = 0;
			m_dwLastHasteTime = dwCurrTick;
			
			index = 0;
			SetByte(m_TempBuf, SET_USER_STATE, index);
			SetInt(m_TempBuf, m_uid + USER_BAND, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);
			
			Send(m_TempBuf, index);
		}
		else
		{
			dwRemainTime = m_dwHasteTime - (dwCurrTick - m_dwLastHasteTime);
			m_dwHasteTime = dwRemainTime;
			m_dwLastHasteTime = dwCurrTick;
		}
	}
	if(dwCurrTick - m_FuShouTime > 120000 && !IsCity())//¸±ÊÖ¼õÄÍ¾ÃÐ¡ºÚ¸±ÊÖ
	{ 
		if(m_UserItem[LEFT_HAND].sSid == 1121)
		{
			if(m_UserItem[LEFT_HAND].sDuration > 1)
			{	
				//SendDuration(LEFT_HAND,1);
				m_UserItem[LEFT_HAND].sDuration -= 1;
	            if(m_UserItem[LEFT_HAND].sDuration < 0) m_UserItem[LEFT_HAND].sDuration = 0;
				CBufferEx TempBuf1;

	            TempBuf1.Add(ITEM_DURATION);
	            TempBuf1.Add((BYTE)LEFT_HAND);
	            TempBuf1.Add(m_UserItem[LEFT_HAND].sDuration);
                Send(TempBuf1, TempBuf1.GetLength());
			}
			else
			{
				m_UserItem[LEFT_HAND].sDuration = 0;
				GetMagicItemSetting();
			}
			m_FuShouTime = dwCurrTick;
		}
	}
	if(m_dwShieldTime != 0)
	{
		if((dwCurrTick - m_dwLastShieldTime) > m_dwShieldTime)		// Shield °¡ Ç®¸± ½Ã°£ÀÌ¸é
		{
			m_dwShieldTime = 0;
			m_dwLastShieldTime = dwCurrTick;

			if(m_bNecklaceOfShield == FALSE)	// ¼öÈ£ÀÇ ¸ñ°ÉÀÌ°¡ ¾øÀ¸¸é
			{
				DeleteAbnormalInfo(ABNORMAL_SHIELD);
				
				index = 0;
				SetByte(m_TempBuf, SET_USER_STATE, index);
				SetInt(m_TempBuf, m_uid + USER_BAND, index);
				SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
				SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);
				
				Send(m_TempBuf, index);//MarkJay ÐÞ¸Ä Ä¬ÈÏSendInsight
//				SendExactScreen(m_TempBuf, index);
			}
		}
		else
		{
			dwRemainTime = m_dwShieldTime - (dwCurrTick - m_dwLastShieldTime);
			m_dwShieldTime = dwRemainTime;
			m_dwLastShieldTime = dwCurrTick;
		}
	}

	if(m_dwDexUpTime != 0)
	{
		if((dwCurrTick - m_dwLastDexUpTime) > m_dwDexUpTime)		// Dex Up ÀÌ Ç®¸± ½Ã°£ÀÌ¸é
		{
			DeleteAbnormalInfo(ABNORMAL_DEX_UP);
			m_dwDexUpTime = 0;
			m_dwLastDexUpTime = dwCurrTick;
			
			index = 0;
			SetByte(m_TempBuf, SET_USER_STATE, index);
			SetInt(m_TempBuf, m_uid + USER_BAND, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);		
			Send(m_TempBuf, index);
		}
		else
		{
			dwRemainTime = m_dwDexUpTime - (dwCurrTick - m_dwLastDexUpTime);
			m_dwDexUpTime = dwRemainTime;
			m_dwLastDexUpTime = dwCurrTick;
		}
	}

	if(m_dwMaxHPUpTime != 0)
	{
		if((dwCurrTick - m_dwLastMaxHPUpTime) > m_dwMaxHPUpTime)	// Max HP Up ÀÌ Ç®¸± ½Ã°£ÀÌ¸é
		{
			DeleteAbnormalInfo(ABNORMAL_MAX_HP_UP);
			m_dwMaxHPUpTime = 0;
			m_dwLastMaxHPUpTime = dwCurrTick;
			
			index = 0;
			SetByte(m_TempBuf, SET_USER_STATE, index);
			SetInt(m_TempBuf, m_uid + USER_BAND, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);		
			Send(m_TempBuf, index);
			
			SetUserToMagicUser(-1);
			//-----------------------------------------
			//yskang 0.7 MAX HP UP ¶Ù´Â µµÁß¿¡ Ç®¸®¸é ¸Õ°¡ °É¸° °Í Ã³·³ Àá±ñ ¸ØÃã Çö»ó ¹ö°Å ¼öÁ¤
			CBufferEx	TempBuf;
			TempBuf.Add(MAX_HP_EXIT);
			TempBuf.Add(m_sHP);
			TempBuf.Add(m_sMagicMaxHP);
			Send(TempBuf,TempBuf.GetLength());
			//SendMyInfo(TO_ME, INFO_MODIFY); //ÀüÃ¼ µ¥ÀÌÅ¸´Â º¸³»Áö ¾Ê´Â´Ù.
			//--------------------------------------
			if(m_bNowBuddy == TRUE) SendBuddyUserChange(BUDDY_CHANGE);		// ¹öµðÁßÀÌ¸é ´Ù¸¥ ¹öµð¿ø¿¡°Ô ³¯¸°´Ù.
		}
		else
		{
			dwRemainTime = m_dwMaxHPUpTime - (dwCurrTick - m_dwLastMaxHPUpTime);
			m_dwMaxHPUpTime = dwRemainTime;
			m_dwLastMaxHPUpTime = dwCurrTick;
		}
	}

	if( m_dwMightyWeaponTime != 0)
	{
		if((dwCurrTick - m_dwLastMightyWeaponTime) > m_dwMightyWeaponTime)		// MightyWeapon ÀÌ Ç®¸± ½Ã°£ÀÌ¸é
		{
			DeleteAbnormalInfo(ABNORMAL_MIGHTYWEAPON);
			m_dwMightyWeaponTime = 0;
			m_dwLastMightyWeaponTime = dwCurrTick;
			
			index = 0;
			SetByte(m_TempBuf, SET_USER_STATE, index);
			SetInt(m_TempBuf, m_uid + USER_BAND, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);			
			Send(m_TempBuf, index);
		}
		else
		{
			dwRemainTime = m_dwMightyWeaponTime - (dwCurrTick - m_dwLastMightyWeaponTime);
			m_dwMightyWeaponTime = dwRemainTime;
			m_dwLastMightyWeaponTime = dwCurrTick;
		}
	}

	if( m_dwAdamantineTime != 0)
	{
		if((dwCurrTick - m_dwLastAdamantineTime) > m_dwAdamantineTime)		// Adamantine ÀÌ Ç®¸± ½Ã°£ÀÌ¸é
		{
			DeleteAbnormalInfo(ABNORMAL_ADAMANTINE);
			m_dwAdamantineTime = 0;
			m_dwLastAdamantineTime = dwCurrTick;
			
			index = 0;
			SetByte(m_TempBuf, SET_USER_STATE, index);
			SetInt(m_TempBuf, m_uid + USER_BAND, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);		
			Send(m_TempBuf, index);
		}
		else
		{
			dwRemainTime = m_dwAdamantineTime - (dwCurrTick - m_dwLastAdamantineTime);
			m_dwAdamantineTime = dwRemainTime;
			m_dwLastAdamantineTime = dwCurrTick;
		}
	}

	if( m_dwBerserkerTime != 0)
	{
		if((dwCurrTick - m_dwLastBerserkerTime) > m_dwBerserkerTime)		// Berserker °¡ Ç®¸± ½Ã°£ÀÌ¸é
		{
			DeleteAbnormalInfo(ABNORMAL_BERSERKER);
			m_dwBerserkerTime = 0;
			m_dwLastBerserkerTime = dwCurrTick;
			
			index = 0;
			SetByte(m_TempBuf, SET_USER_STATE, index);
			SetInt(m_TempBuf, m_uid + USER_BAND, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);	
			Send(m_TempBuf, index);//MarkJay ÐÞ¸Ä Ä¬ÈÏSendInsight
//			SendExactScreen(m_TempBuf, index);
		}
		else
		{
			dwRemainTime = m_dwBerserkerTime - (dwCurrTick - m_dwLastBerserkerTime);
			m_dwBerserkerTime = dwRemainTime;
			m_dwLastBerserkerTime = dwCurrTick;
		}
	}

	if( m_dwFastRunTime != 0)
	{
		if((dwCurrTick - m_dwLastFastRunTime) > m_dwFastRunTime)		// Fast Run ÀÌ Ç®¸± ½Ã°£ÀÌ¸é
		{
			DeleteAbnormalInfo(ABNORMAL_FASTRUN);
			m_dwFastRunTime = 0;
			m_dwLastFastRunTime = dwCurrTick;
			
			index = 0;
			SetByte(m_TempBuf, SET_USER_STATE, index);
			SetInt(m_TempBuf, m_uid + USER_BAND, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);
			Send(m_TempBuf, index);//MarkJay ÐÞ¸Ä
		}
		else
		{
			dwRemainTime = m_dwFastRunTime - (dwCurrTick - m_dwLastFastRunTime);
			m_dwFastRunTime = dwRemainTime;
			m_dwLastFastRunTime = dwCurrTick;
		}
	}

	if( m_dwMindShockTime != 0)
	{
		if((dwCurrTick - m_dwLastMindShockTime) > m_dwMindShockTime)		// Mind Shock °¡ Ç®¸± ½Ã°£ÀÌ¸é
		{
			DeleteAbnormalInfo(ABNORMAL_MINDSHOCK);
			m_dwMindShockTime = 0;
			m_dwLastMindShockTime = dwCurrTick;
			
			index = 0;
			SetByte(m_TempBuf, SET_USER_STATE, index);
			SetInt(m_TempBuf, m_uid + USER_BAND, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);	
//			SendInsight(m_TempBuf, index);
			SendExactScreen(m_TempBuf, index);
		}
		else
		{
			dwRemainTime = m_dwMindShockTime - (dwCurrTick - m_dwLastMindShockTime);
			m_dwMindShockTime = dwRemainTime;
			m_dwLastMindShockTime = dwCurrTick;
		}
	}

	if( m_dwMindGuardTime != 0)
	{
		if((dwCurrTick - m_dwLastMindGuardTime) > m_dwMindGuardTime)		// Mind Guard °¡ Ç®¸± ½Ã°£ÀÌ¸é
		{
			DeleteAbnormalInfo(ABNORMAL_MINDGUARD);
			m_dwMindGuardTime = 0;
			m_dwLastMindGuardTime = dwCurrTick;
			
			index = 0;
			SetByte(m_TempBuf, SET_USER_STATE, index);
			SetInt(m_TempBuf, m_uid + USER_BAND, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);		
//			SendInsight(m_TempBuf, index);
			SendExactScreen(m_TempBuf, index);
		}
		else
		{
			dwRemainTime = m_dwMindGuardTime - (dwCurrTick - m_dwLastMindGuardTime);
			m_dwMindGuardTime = dwRemainTime;
			m_dwLastMindGuardTime = dwCurrTick;
		}
	}

	if( m_dwPsiShieldTime != 0)
	{
		if((dwCurrTick - m_dwLastPsiShieldTime) > m_dwPsiShieldTime)		// Psionic Shield °¡ Ç®¸± ½Ã°£ÀÌ¸é
		{
			DeleteAbnormalInfo(ABNORMAL_PSISHIELD);
			m_dwPsiShieldTime = 0;
			m_dwLastPsiShieldTime = dwCurrTick;
			
			index = 0;
			SetByte(m_TempBuf, SET_USER_STATE, index);
			SetInt(m_TempBuf, m_uid + USER_BAND, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);		
			SendInsight(m_TempBuf, index);
//			SendExactScreen(m_TempBuf, index);
		}
		else
		{
			dwRemainTime = m_dwPsiShieldTime - (dwCurrTick - m_dwLastPsiShieldTime);
			m_dwPsiShieldTime = dwRemainTime;
			m_dwLastPsiShieldTime = dwCurrTick;
		}
	}
	if( m_dwBigShieldTime != 0)
	{
		if((dwCurrTick - m_dwLastBigShieldTime) > m_dwBigShieldTime)		// Psionic Shield °¡ Ç®¸± ½Ã°£ÀÌ¸é
		{
				
			DeleteAbnormalInfo(ABNORMAL_BIGSHIELD);
			m_dwBigShieldTime = 0;
			m_dwLastBigShieldTime = dwCurrTick;
			
			index = 0;
			SetByte(m_TempBuf, SET_USER_STATE, index);
			SetInt(m_TempBuf, m_uid + USER_BAND, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);			
			SendInsight(m_TempBuf, index);

//			SendExactScreen(m_TempBuf, index);
		}
		else
		{
			dwRemainTime = m_dwBigShieldTime - (dwCurrTick - m_dwLastBigShieldTime);
			m_dwBigShieldTime = dwRemainTime;
			m_dwLastBigShieldTime = dwCurrTick;
		}
	}

	if( m_dwPiercingShieldTime != 0)
	{
		if((dwCurrTick - m_dwLastPiercingShieldTime) > m_dwPiercingShieldTime)		// Piercing Shield °¡ Ç®¸± ½Ã°£ÀÌ¸é
		{
			DeleteAbnormalInfo(ABNORMAL_PIERCING_SHIELD);
			m_dwPiercingShieldTime = 0;
			m_dwLastPiercingShieldTime = dwCurrTick;
			
			index = 0;
			SetByte(m_TempBuf, SET_USER_STATE, index);
			SetInt(m_TempBuf, m_uid + USER_BAND, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);			
//			SendInsight(m_TempBuf, index);
			SendExactScreen(m_TempBuf, index);
		}
		else
		{
			dwRemainTime = m_dwPiercingShieldTime - (dwCurrTick - m_dwLastPiercingShieldTime);
			m_dwPiercingShieldTime = dwRemainTime;
			m_dwLastPiercingShieldTime = dwCurrTick;
		}
	}
	if(m_dwPKAddkill > 0) //ÇåÀíPK´óÈûÉ±ÈËÊý
	{
		SYSTEMTIME messageTime;
		GetLocalTime(&messageTime);
		if(messageTime.wHour == 3 && messageTime.wMinute == 59 && messageTime.wSecond == 59)
		{
			m_dwPKAddkill = 0;
			UpdateUserData();
		}
	}

	if(m_dwPKAddkill > 0) //¶þ´ÎÇåÀíPK´óÈûÉ±ÈËÊý
	{
		SYSTEMTIME messageTime;
		GetLocalTime(&messageTime);
		if(messageTime.wHour == 15 && messageTime.wMinute == 59 && messageTime.wSecond == 59)
		{
			m_dwPKAddkill = 0;
			UpdateUserData();
		}
	}

	if( m_dwNoDamageTime != 0)	// Game Start, Zone Change ½ÃÀÇ ¹«Àû½Ã°£
	{
		if((dwCurrTick - m_dwLastNoDamageTime) > m_dwNoDamageTime)		// ¹«Àû½Ã°£ÀÌ Ç®¸± ½Ã°£ÀÌ¸é
		{
			m_dwNoDamageTime = 0;
			m_dwLastNoDamageTime = dwCurrTick;
		}
		else
		{
			dwRemainTime = m_dwNoDamageTime - (dwCurrTick - m_dwLastNoDamageTime);
			m_dwNoDamageTime = dwRemainTime;
			m_dwLastNoDamageTime = dwCurrTick;
		}
	}
	if(!IsCity())
	{
		if( m_dwHiExpTime != 0)	// °æÇèÄ¡ 2¹è ¹°¾à ³²Àº ½Ã°£ °è»ê
		{
			if((dwCurrTick - m_dwLastHiExpTime) > m_dwHiExpTime)
			{
				DeleteAbnormalInfo(ABNORMAL_HIEXP);
				m_dwHiExpTime = 0;
				m_dwLastHiExpTime = dwCurrTick;
			
				index = 0;
				SetByte(m_TempBuf, SET_USER_STATE, index);
				SetInt(m_TempBuf, m_uid + USER_BAND, index);
				SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
				SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);		
				Send(m_TempBuf, index);
			}
			else
			{
				dwRemainTime = m_dwHiExpTime - (dwCurrTick - m_dwLastHiExpTime);
				m_dwHiExpTime = dwRemainTime;
				m_dwLastHiExpTime = dwCurrTick;
			}
		}

		if( m_dwMagicFindTime != 0)	// ¸ÅÁ÷Âù½º 5¹è ¹°¾à ³²Àº ½Ã°£ °è»ê
		{
			if((dwCurrTick - m_dwLastMagicFindTime) > m_dwMagicFindTime)
			{
				DeleteAbnormalInfo(ABNORMAL_MAGICFIND);
				m_dwMagicFindTime = 0;
				m_dwLastMagicFindTime = dwCurrTick;

				index = 0;
				SetByte(m_TempBuf, SET_USER_STATE, index);
				SetInt(m_TempBuf, m_uid + USER_BAND, index);
				SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
				SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);
				Send(m_TempBuf, index);
			}
			else
			{
				dwRemainTime = m_dwMagicFindTime - (dwCurrTick - m_dwLastMagicFindTime);
				m_dwMagicFindTime = dwRemainTime;
				m_dwLastMagicFindTime = dwCurrTick;
			}
		}

		if( m_dwFUExpTime != 0)//ÐÒ¸£Ê±¹â
		{
			if((dwCurrTick - m_dwLastFUExpTime) > m_dwFUExpTime)
			{
				DeleteAbnormalInfo(ABNORMAL_FUDAI);
				m_dwFUExpTime = 0;
				m_dwLastFUExpTime = dwCurrTick;

				index = 0;
				SetByte(m_TempBuf, SET_USER_STATE, index);
				SetInt(m_TempBuf, m_uid + USER_BAND, index);
				SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
				SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);
				Send(m_TempBuf, index);
			}
			else
			{
				dwRemainTime = m_dwFUExpTime - (dwCurrTick - m_dwLastFUExpTime);
				m_dwFUExpTime = dwRemainTime;
				m_dwLastFUExpTime = dwCurrTick;
			}
		}
		if( m_dwBFindTime != 0)
		{
			if((dwCurrTick - m_dwLastBFindTime) > m_dwBFindTime)
			{
				m_sDramageUp = false;
				m_sDramageUpPercent = false;
				HuanYuanBianShen();

				m_dwBFindTime = 0;
				m_dwLastBFindTime = dwCurrTick;
			}
			else
			{
				dwRemainTime = m_dwBFindTime - (dwCurrTick - m_dwLastBFindTime);
				m_dwBFindTime = dwRemainTime;
				m_dwLastBFindTime = dwCurrTick;
			}
		}
		if( m_dwHuanLing != 0)
		{
			if((dwCurrTick - m_dwLastHuanLing) > m_dwHuanLing)
			{
				DeleteStateInfo(STATE_5);
				m_dwHuanLing = 0;
				m_dwLastHuanLing = dwCurrTick;
				index = 0;
				SetByte(m_TempBuf, SET_USER_STATE, index);
				SetInt(m_TempBuf, m_uid + USER_BAND, index);
				SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
				SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);					
				Send(m_TempBuf, index);			
			}
			else
			{
				dwRemainTime = m_dwHuanLing - (dwCurrTick - m_dwLastHuanLing);
				m_dwHuanLing = dwRemainTime;
				m_dwLastHuanLing = dwCurrTick;
			}
		}
		if( m_dwHuanJing != 0)
		{
			if((dwCurrTick - m_dwLastHuanJing) > m_dwHuanJing)
			{
				DeleteStateInfo(STATE_28);
				m_dwHuanJing = 0;
				m_dwLastHuanJing = dwCurrTick;
			
				index = 0;
				SetByte(m_TempBuf, SET_USER_STATE, index);
				SetInt(m_TempBuf, m_uid + USER_BAND, index);
				SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
				SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);					
				Send(m_TempBuf, index);			
			}
			else
			{
				dwRemainTime = m_dwHuanJing - (dwCurrTick - m_dwLastHuanJing);
				m_dwHuanJing = dwRemainTime;
				m_dwLastHuanJing = dwCurrTick;
			}
		}
		if(m_dwVIPTime != 0)//VIPÓÃ»§Ê±¼äÅÐ¶Ï
		{
			if((dwCurrTick - m_dwLastVIPTime) > m_dwVIPTime)
			{
				DeleteAbnormalInfo(ABNORMAL_VIP);
				m_dwVIPTime = 0;
				ChangeLoveName();
				m_dwLastVIPTime = dwCurrTick;
				
				index = 0;
				SetByte(m_TempBuf, SET_USER_STATE, index);
				SetInt(m_TempBuf, m_uid + USER_BAND, index);
				SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
				SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);					
				Send(m_TempBuf, index);			
			}
			else
			{
				dwRemainTime = m_dwVIPTime - (dwCurrTick - m_dwLastVIPTime);
				m_dwVIPTime = dwRemainTime;
				m_dwLastVIPTime = dwCurrTick;
			}
		}
	}
	else
	{
		m_dwLastHiExpTime = dwCurrTick;
		m_dwLastMagicFindTime = dwCurrTick;
		m_dwLastFUExpTime = dwCurrTick;
		m_dwLastBFindTime = dwCurrTick;
		m_dwLastHuanLing = dwCurrTick;
		m_dwLastHuanJing = dwCurrTick;
		m_dwLastVIPTime = dwCurrTick;//³ÇÄÚ²»µô»áÔ±Ê±¼ä
	}
	//if( m_dwNoChatTime != 0)	
	//{
	//	if((dwCurrTick - m_dwLastNoChatTime) > m_dwNoChatTime)		
	//	{
	//		m_dwNoChatTime = 0;
	//		m_dwLastNoChatTime = dwCurrTick;
	//	}
	//	else
	//	{
	//		dwRemainTime = m_dwNoChatTime - (dwCurrTick - m_dwLastNoChatTime);
	//		m_dwNoChatTime = dwRemainTime;
	//		m_dwLastNoChatTime = dwCurrTick;
	//	}
	//}
	if( m_dwCKMiMa != 0)// µã¿¨Ê±¼ä¿Û³ý
	{
		if((dwCurrTick - m_dwLastCKMiMa) > m_dwCKMiMa)	
		{
			m_dwCKMiMa = 0;
			m_dwLastCKMiMa = dwCurrTick;
			
			index = 0;
			SetByte(m_TempBuf, SET_USER_STATE, index);
			SetInt(m_TempBuf, m_uid + USER_BAND, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
			SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);					
			Send(m_TempBuf, index);
			SendSystemMsg("ÄúµÄµã¿¨Ê±¼äÒÑµ½!ÐèÒª³äÖµ²ÅÄÜÕý³£ÓÎÏ·!",SYSTEM_ERROR,TO_ME);
		}
		else
		{
			dwRemainTime = m_dwCKMiMa - (dwCurrTick - m_dwLastCKMiMa);
			m_dwCKMiMa = dwRemainTime;
			m_dwLastCKMiMa = dwCurrTick;
		}
	}
	if( m_dwGuarDianTianShi != 0)// ÔÂ¿¨Ê±¼ä¿Û³ý
	{
		SYSTEMTIME messageTime;
		GetLocalTime(&messageTime);
		if(messageTime.wHour == 23 && messageTime.wMinute == 59 && messageTime.wSecond == 59)
		{
			if(m_dwGuarDianTianShi != 0) m_dwGuarDianTianShi -= 1;
			else
			{
				m_dwGuarDianTianShi = 0;			
				index = 0;
				SetByte(m_TempBuf, SET_USER_STATE, index);
				SetInt(m_TempBuf, m_uid + USER_BAND, index);
				SetDWORD(m_TempBuf, m_dwAbnormalInfo, index);
				SetDWORD(m_TempBuf, m_dwAbnormalInfo_, index);					
				Send(m_TempBuf, index);
				SendSystemMsg("ÄúµÄÔÂ¿¨Ê±¼äÒÑµ½!ÐèÒª³äÖµ²ÅÄÜÕý³£ÓÎÏ·!",SYSTEM_ERROR,TO_ME);
			}
		}
		if(m_dwGuarDianTianShi > 2000000000) m_dwGuarDianTianShi = 2000000000;
		if(m_dwGuarDianTianShi < 0) m_dwGuarDianTianShi = 0;
	}
	if( m_dwCloseTime != 0)	// ¼àÓü¼ÇÊ±
	{
		if((dwCurrTick - m_dwLastCloseTime) > m_dwCloseTime)	
		{
			Fchuuser();
			m_dwCloseTime = 0;			
			m_dwLastCloseTime = dwCurrTick;
			SendSystemMsg("·âºÅÊ±¼äµ½,Äã±»ÏµÍ³×Ô¶¯ÊÍ·Å",SYSTEM_ERROR,TO_ME);
		}
		else
		{
			dwRemainTime = m_dwCloseTime - (dwCurrTick - m_dwLastCloseTime);
			m_dwCloseTime = dwRemainTime;
			m_dwLastCloseTime = dwCurrTick;
		}
	}
	if(GetTickCount() - m_WaiGuaTime > 3 * 1000)	
	{
		if(m_FengWaiGua != 1)
		{
			SendSystemMsg( "ÇëÎðÊ¹ÓÃÆäËüÍâ¹Ò!", SYSTEM_ERROR, TO_ME);
			SoftClose();
			return;
		}
	}
	 //×Ô¶¯¼ÓÔª±¦
    if(f_Markini[0]->PYB_KG == 1 && m_sLevel >= f_Markini[0]->PYB_DJ)//ÅÝÔª±¦¿ª¹Ø
    {
		int iTime = 0;
		int temp_money = f_Markini[0]->PYB_ShuLiang; //ÅÝÔª±¦ (¸÷¸ö½Ç¶È¶¼ËÍ)//Ã¿¸ö
		iTime = f_Markini[0]->BASIC_ADD_YB_TIME * 1000; //ÅÝÔª±¦ Ïà¸ôÊ±¼ä µ¥Î»Ãë
		if((int)(dwCurrTick - m_dwYBUpTime) > iTime)	
		{
				if(m_dwShopPingDN >= 9999) return;
				CString strMeony = _T("");	
				strMeony.Format("ÔÚÏß %d Ãë,»ñµÃ %d Ôª±¦.",iTime/1000,temp_money);// "Ìì½µÔª±¦[%d]¸ö"
				SendSystemMsg(strMeony.GetBuffer(strMeony.GetLength()), SYSTEM_NORMAL, TO_ME);
				m_dwShopPingDN += temp_money;
				m_dwYBUpTime = dwCurrTick;
				UpdateUserItemShopPingYB();
				SendUserStatusSkill();
		}
	}
	 //×Ô¶¯¼Ó¾öÕ½±Ò
	if(f_Markini[0]->PJB_KG == 1 && m_sLevel >= f_Markini[0]->PJB_DJ)
    {
		int iTime = 0;
		int temp_money = f_Markini[0]->PJB_ShuLiang;
		iTime = f_Markini[0]->BASIC_ADD_JB_TIME * 1000;
		if((int)(dwCurrTick - m_dwJZBUpTime) > iTime)	
		{
			if(m_dwDN >= 2000000000) return;
			CString strMeony = _T("");	
			strMeony.Format("ÔÚÏß %d Ãë,»ñµÃ %d ¾öÕ½±Ò.",iTime/1000,temp_money);// "Ìì½µÔª±¦[%d]¸ö"
			SendSystemMsg(strMeony.GetBuffer(strMeony.GetLength()), SYSTEM_NORMAL, TO_ME);
			GiveDN(temp_money);
			m_dwJZBUpTime = dwCurrTick;
		}
	}
	if(f_Markini[0]->EXP_KG == 1 && m_sLevel >= 50 && IsCity())//ÔÚÏßÅÝ¾­Ñé MarkJay
	{
		int iTime = 0;
		int iNum =  15000;
		iTime = 80 * 1000;
		if((int)(dwCurrTick - m_dwEXPUpTime) > iTime)	
		{
			GetExp(iNum);
			CString strMsg;
			strMsg.Format("Äã»ñµÃ¾­Ñé %d µã.",iNum);
			SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
			m_dwEXPUpTime = dwCurrTick;
		}
	}
	//ÔÚÏßÊ±¼ä
	if(m_sLevel >= 100 && m_bSessionOnline == false)
	{
		if((dwCurrTick - m_dwLastUPZaiXianTime) > m_dwUPZaiXianTime)//ÔÚÏßÊ±¼ä
		{
			m_dwUPZaiXianTime = 60 * 1000;
			m_dwLastUPZaiXianTime = dwCurrTick;
			m_dwZaiXianTime += 1;					
		}
		else
		{
			dwRemainTime = m_dwUPZaiXianTime - (dwCurrTick - m_dwLastUPZaiXianTime);
			m_dwUPZaiXianTime = dwRemainTime;
			m_dwLastUPZaiXianTime = dwCurrTick;
		}
	}
	// pk count Ç®¾îÁÖ±â
	int rank = m_sCityRank + CITY_RANK_INTERVAL;
	if( rank == SAINT_RANK )		// ÇöÀç ¼¼ÀÎÆ®¶ó¸é
	{
		if( dwCurrTick - m_dwSaintTimeCount >= 60*1000 )
		{
			CheckMaxValue(m_dwSaintTime, 1);

			if( m_dwSaintTime >= 300 )
			{
				// killcount¸¦ ÇÏ³ª ÁÙ¿©ÁØ´Ù.
				m_sKillCount -= 1;
				if( m_sKillCount < 0 ) m_sKillCount = 0;
				m_dwSaintTime = 0;
			}

			m_dwSaintTimeCount = dwCurrTick;
		}
	}
	else
	{
		m_dwSaintTime = 0;
		m_dwSaintTimeCount = dwCurrTick;
	}

	//»óÅÂÀÌ»ó Ã³¸®·çÆ¾ ¼öÇà --------------------------------------//

	switch(m_tAbnormalKind)
	{
	case ABNORMAL_BYTE_NONE:
		return;
	case ABNORMAL_BYTE_POISON:
		break;
	case ABNORMAL_BYTE_CONFUSION://Ñ£ÔÎ
		if((dwCurrTick - m_dwLastAbnormalTime) > dwAbnormalTime)
		{
			CBufferEx TempBuf;
			m_tAbnormalKind = ABNORMAL_BYTE_NONE;
			m_dwAbnormalTime = 0;
			m_dwLastAbnormalTime = GetTickCount();

			SendSystemMsg( "±äÇåÐÑ£¡£¡£¡", SYSTEM_NORMAL, TO_ME);

			TempBuf.Add(SET_USER_STATE);
			TempBuf.Add(m_uid + USER_BAND);
			DeleteAbnormalInfo(ABNORMAL_CONFUSION);
			TempBuf.Add(m_dwAbnormalInfo);
		    TempBuf.Add(m_dwAbnormalInfo_);
			SendInsight(TempBuf, TempBuf.GetLength());	
		}
		else
		{
			dwRemainTime = dwAbnormalTime - (dwCurrTick - m_dwLastAbnormalTime);
			m_dwAbnormalTime = dwRemainTime;
			m_dwLastAbnormalTime = dwCurrTick;
		}
		break;
	case ABNORMAL_BYTE_PARALYSIS:
		break;
	case ABNORMAL_BYTE_BLIND:
		break;
	case ABNORMAL_BYTE_LIGHT:
		break;
	case ABNORMAL_BYTE_FIRE:
		dwDiffTime = dwCurrTick - m_dwLastAbnormalTime;

		if(dwDiffTime >= 5000)	
		{
			SetDamage((int)((double)m_sMaxHP * 0.02 + 0.5));
			//if(m_bLive == USER_DEAD) 
			if(m_lDeadUsed == 1)
			{
				GetLevelDownExp(USER_PK, -1, FALSE,m_strUserID);			// °æÇèÄ¡¿Í ±×¿Ü º¯È­·®¸¦ ¹Ý¿µÇÑ´Ù.
				return;
			}
		}

		if((dwCurrTick - m_dwLastAbnormalTime) > dwAbnormalTime)	// Fire »óÅÂÀÌ»óÀÌ Ç®¸± ½Ã°£ÀÌ ‰çÀ¸¸é
		{
			m_tAbnormalKind = ABNORMAL_BYTE_NONE;
			m_dwAbnormalTime = 0;
			m_dwLastAbnormalTime = GetTickCount();

			if(g_bDebug) SendSystemMsg( IDS_USER_FIRE_DAMAGE_RESET, SYSTEM_NORMAL, TO_ME);//"×ÆÉËÒÑ¸´Ô­."
			CBufferEx TempBuf;
			TempBuf.Add(SET_USER_STATE);
			TempBuf.Add(m_uid + USER_BAND);
			DeleteAbnormalInfo(ABNORMAL_FIRE);
			TempBuf.Add(m_dwAbnormalInfo);
			TempBuf.Add(m_dwAbnormalInfo_);
			Send(TempBuf, TempBuf.GetLength());
		}
		else
		{
			dwRemainTime = dwAbnormalTime - (dwCurrTick - m_dwLastAbnormalTime);
			m_dwAbnormalTime = dwRemainTime;
			m_dwLastAbnormalTime = dwCurrTick;
		}

		break;

	case ABNORMAL_BYTE_COLD:
		if((dwCurrTick - m_dwLastAbnormalTime) > dwAbnormalTime)	// Cold »óÅÂÀÌ»óÀÌ Ç®¸± ½Ã°£ÀÌ ‰çÀ¸¸é
		{
			CBufferEx TempBuf;
			m_tAbnormalKind = ABNORMAL_BYTE_NONE;
			m_dwAbnormalTime = 0;
			m_dwLastAbnormalTime = GetTickCount();

			if(g_bDebug) SendSystemMsg( IDS_USER_COLD_DAMAGE_RESET, SYSTEM_NORMAL, TO_ME);//"±ù¶³ÒÑ¸´Ô­."

			TempBuf.Add(SET_USER_STATE);
			TempBuf.Add(m_uid + USER_BAND);
			DeleteAbnormalInfo(ABNORMAL_COLD);
			TempBuf.Add(m_dwAbnormalInfo);
			TempBuf.Add(m_dwAbnormalInfo_);
			
			Send(TempBuf, TempBuf.GetLength());
		}
		else
		{
			dwRemainTime = dwAbnormalTime - (dwCurrTick - m_dwLastAbnormalTime);
			m_dwAbnormalTime = dwRemainTime;
			m_dwLastAbnormalTime = dwCurrTick;
		}
		break;
	}
}

/*
void USER::MarkCB() //ÂÌ³á°ò
{
	bool MarkTS = false;
	
	CBufferEx TempBuf;
	TempBuf.Add(SET_USER_STATE);
	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.Add(m_dwAbnormalInfo);
	TempBuf.Add(m_dwAbnormalInfo_);
	
	if(m_dwBFindTime > 0)//²âÊÔÍ·¶¥´øNEW MarkJay
	{
		AddNewStatus(New_Status_22);
		SendABNORMAL();
	}
	TempBuf.Add((BYTE)0);
	Send(TempBuf, TempBuf.GetLength()); //¸øme·¢ËÍ
	SendInsight(TempBuf, TempBuf.GetLength()); //¸øËùÓÐÈË·¢ËÍ	
}

*/
void USER::SetPsiAbnormalStatus()
{
	if(m_tAbnormalKind != ABNORMAL_BYTE_NONE)
	{
		switch(m_tAbnormalKind)
		{
		case ABNORMAL_BYTE_CONFUSION:
			AddAbnormalInfo(ABNORMAL_CONFUSION);
			break;
		case ABNORMAL_BYTE_FIRE:
			AddAbnormalInfo(ABNORMAL_FIRE);
			break;
		case ABNORMAL_BYTE_COLD:
			AddAbnormalInfo(ABNORMAL_COLD);
			break;
		}
	}

	if( m_dwHasteTime != 0 )							AddAbnormalInfo(ABNORMAL_HASTE);
	if( m_dwShieldTime != 0 || m_bNecklaceOfShield)		AddAbnormalInfo(ABNORMAL_SHIELD);
	if( m_dwDexUpTime != 0 )							AddAbnormalInfo(ABNORMAL_DEX_UP);
	if( m_dwMaxHPUpTime != 0 )							AddAbnormalInfo(ABNORMAL_MAX_HP_UP);

	if( m_dwFastRunTime != 0 )			AddAbnormalInfo(ABNORMAL_FASTRUN);
	if( m_dwMindShockTime != 0 )		AddAbnormalInfo(ABNORMAL_MINDSHOCK);
	if( m_dwMindGuardTime != 0 )		AddAbnormalInfo(ABNORMAL_MINDGUARD);
	if( m_dwPsiShieldTime != 0 )		AddAbnormalInfo(ABNORMAL_PSISHIELD);
	if( m_dwBigShieldTime != 0 )		AddAbnormalInfo(ABNORMAL_BIGSHIELD);
	if( m_dwPiercingShieldTime != 0 )	AddAbnormalInfo(ABNORMAL_PIERCING_SHIELD);

	if( m_dwAdamantineTime != 0 )	AddAbnormalInfo(ABNORMAL_ADAMANTINE);
	if( m_dwMightyWeaponTime != 0 )	AddAbnormalInfo(ABNORMAL_MIGHTYWEAPON);
	if( m_dwBerserkerTime != 0 )	AddAbnormalInfo(ABNORMAL_BERSERKER);

	if( m_dwHiExpTime != 0)			AddAbnormalInfo(ABNORMAL_HIEXP);
	if( m_dwMagicFindTime != 0)		AddAbnormalInfo(ABNORMAL_MAGICFIND);
	//if( m_dwNoChatTime != 0)		AddAbnormalInfo(ABNORMAL_NOCHAT);

	if( m_dwCloseTime != 0 )		AddAbnormalInfo(ABNORMAL_JIANYU);//¼àÓü
	if( m_dwVIPTime != 0)			AddAbnormalInfo(ABNORMAL_VIP); // VIPÓÃ»§ÏÔÊ¾Í¼
	if( m_dwHuanJing != 0 )			AddAbnormalInfo(ABNORMAL_BIANSHEN);
	if( m_dwFUExpTime != 0)			AddAbnormalInfo(ABNORMAL_FUDAI);//ÐÒ¸£Ê±¹â
	//if( m_sLevel > 0)				AddAbnormalInfo(ABNORMAL_VIP);
}
void USER::SendCHONGSHENG()
{
	BOOL bRes = FALSE, bWeight = FALSE;
	int skill_point = 0;
	int i = 0;
	if(m_sLevel <= 60)
		skill_point = (int)( (double)m_sLevel / 2 + 0.5 );
	else if(m_sLevel > 60 && m_sLevel <= 99)
		skill_point = (int)( 30 + (double)m_sLevel - 60 + 0.5 );
	else if(m_sLevel > 99)
		skill_point = (int)( 30 + 99 - 60 + 0.5 );
	else
	{
		return;
	}
	switch (m_byClass)
	{
		case 0://È­
		m_sSTR = 14;
		m_sCON = 15;
		m_sDEX = 10;
		m_sVOL = 9;	
		m_sWIS = 9;
		break;
		case 1:
		m_sSTR = 9;
		m_sCON = 10;
		m_sDEX = 9;
		m_sVOL = 17;
		m_sWIS = 12;
		break;
		case 2:
		m_sSTR = 15;
		m_sCON = 14;
		m_sDEX = 10;
		m_sVOL = 9;
		m_sWIS = 9;
		break;
		case 3:
		m_sSTR = 10;
		m_sCON = 12;
		m_sDEX = 17;
		m_sVOL = 9;
		m_sWIS = 9;
		break;
	}
	m_sPA = (short)(m_sLevel-1)+8;
	DelMyEventNum(251);
	for(i = 0; i < TOTAL_PSI_NUM; i++)					// ÃÊ±âÈ­¸¦ ²À ÇØ¾ß ÇÑ´Ù.
	{
		m_UserPsi[i].sSid = -1;
		m_UserPsi[i].tOnOff = FALSE;
	}
	for(i = 0; i < TOTAL_SKILL_NUM; i++)
	{
		m_UserSkill[i].sSid = i;
		m_UserSkill[i].tLevel = 0;
	}

	m_UserSkill[4].sSid = 4;
	m_UserSkill[9].sSid = 9;
	m_UserSkill[14].sSid = 14;
	m_UserSkill[19].sSid = 19;

	m_sSkillPoint = (short)skill_point;
	if(m_sLevel >= 130 && f_Markini[0]->KG130 == 1)
	{
		m_sSkillPoint_= m_sLevel - 129;
		if(m_sSkillPoint_ < 0)		m_sSkillPoint_ = 0;
		if(m_sSkillPoint_ > 60)		m_sSkillPoint_ = 60;
		for(i = 0; i < 12; i++)
		{
			m_UserSkill_[i].sSid = i;
			m_UserSkill_[i].tLevel = 0;
		}
	}
	SendCharData();
	SetUserToMagicUser();
	CheckMagicItemMove();
	UpdateUserData();
	SendMyInfo( TO_INSIGHT, INFO_MODIFY );
	SendUserStatusSkill();

	bWeight = TRUE;
}
void USER::SetStatus()//¸üÐÂ×´Ì¬
{
	AddStateInfo(STATE_1);

	CBufferEx TempBuf;
	TempBuf.Add(SET_USER_STATE);
	TempBuf.Add(m_uid + USER_BAND);
	if(m_dwBFindTime > 0) AddStateInfo(STATE_2);//±äÉí
	if(m_dwHuanLing > 0) AddStateInfo(STATE_5);//»ÃÁé
	//if(m_dwGuarDianTianShi > 0) AddStateInfo(STATE_7);//ÌìÊ¹
	if(m_dwHuanJing > 0) AddStateInfo(STATE_28);//»Ã¾§
	if(isGuildRen) AddStateInfo(STATE_6);//ÒªÈû³á°ò
	if(f_Markini[0]->SBEXP_KG == 1)
	{
		AddAbnormalInfo(ABNORMAL_HIEXP);
		AddStateInfo(STATE_21);//Èý±¶ÐË·Ü½ðÉ«Í¼±ê
		AddAbnormalInfo(ABNORMAL_MAGICFIND);
		AddStateInfo(STATE_22);//Èý±¶ÐÒÔË½ðÉ«Í¼±ê
	}
	
	/*if(m_dwBFindTime > 0)//²âÊÔÍ·¶¥´øNEW MarkJay
	{
		AddNewStatus(New_Status_22);
		SendABNORMAL();
	}*/
	TempBuf.Add(m_dwAbnormalInfo);
	TempBuf.Add(m_dwAbnormalInfo_);
	Send(TempBuf, TempBuf.GetLength());
	SendInsight(TempBuf, TempBuf.GetLength()); //¸øËùÓÐÈË·¢ËÍ
}

BOOL USER::ReturnTown(BYTE type, BYTE slot)
{
	int x, y;
	int isZone = -1;
	int iPotalIndex = -1;
	BYTE result = SUCCESS;
	BYTE zoneChange = 0;
	CPoint ptTown, pt;
	
	CBufferEx TempBuf;

	pt = GetTownPotal( isZone,0 );
	x = pt.x;
	y = pt.y;

	if( isZone < 0 ) // ¾Æ´Ï¸é °°Àº Á¸¾ÈÀ¸·Î ÀÌµ¿
	{
		SendTownPotal( FAIL, zoneChange );
		return TRUE;
	}

	if( IsZoneInThisServer( isZone ) )	// ÀÌµ¿ÇÏ·Á´Â Á¸ÀÌ °°Àº ¼­¹ö¾È¿¡ ÀÖÀ»¶§
	{
		TownPotal();
		return TRUE;
	}

	if( slot < EQUIP_ITEM_NUM || slot >= TOTAL_INVEN_MAX )
	{
		SendTownPotal( FAIL, zoneChange );
		return TRUE;
	} 

	int sSid = m_UserItem[slot].sSid;

	if(sSid < 0 || sSid >= g_arItemTable.GetSize()) 
	{
		SendTownPotal( FAIL, zoneChange );
		return TRUE;
	}

	if( m_iCurWeight <= g_arItemTable[sSid]->m_byWeight ) m_iCurWeight = 0;
	else m_iCurWeight -= g_arItemTable[sSid]->m_byWeight;		// ¾´¸¸Å­ ¹«°Ô¸¦ ÁÙÀÎ´Ù.

	m_UserItem[slot].sCount -= 1;
	if(m_UserItem[slot].sCount <= 0) { ReSetItemSlot(&m_UserItem[slot]); sSid = -1; }

	UpdateUserItemDN();							// ¾ÆÀÌÅÛ º¯µ¿ Ã¼Å©...

	GetRecoverySpeed();							// È¸º¹¼Óµµ Ã¼Å©...

	ZoneMoveReq( isZone, x, y );
	return FALSE;
}

BOOL USER::GetAbsentGuildInfo(int guildsid)
{
	int i;

	CGuild* pNewGuild = NULL;

	pNewGuild = GetGuild( guildsid );

	if( pNewGuild )
	{
		if( (int)pNewGuild->m_lSid == guildsid )
		{
			ReleaseGuild();
			return TRUE;
		}
	}

	SQLHSTMT		hstmt;
	SQLRETURN		retcode;
	TCHAR			szSQL[2048];

	::ZeroMemory(szSQL, sizeof(szSQL));

	wsprintf(szSQL,TEXT("SELECT * FROM GUILD where iSid = %d"), guildsid);
	
	SQLUINTEGER		iDN;//, iEXP;
	SQLINTEGER		iSID;
	SQLSMALLINT		sVERSION;
	SQLCHAR			strGuildName[CHAR_NAME_LENGTH + 1], strMasterName[CHAR_NAME_LENGTH + 1];
	SQLCHAR			strItem[_BANK_DB], strMark[GUILD_MARK_SIZE], strHaveMap[GUILD_MAP_COUNT];

	SQLINTEGER		sInd;

	iSID = 0;
	iDN = 0;//iEXP = 0;
	sVERSION = 0;

	g_CurrentGuildCount = 0;			// ÇöÀç µî·ÏÇÑ ±æµåÀÇ ¸¶Áö¸· ¹øÈ£ ¼ÂÆÃ

	::ZeroMemory(strGuildName, sizeof(strGuildName));
	::ZeroMemory(strMasterName, sizeof(strMasterName));
	::ZeroMemory(strMark, sizeof(strMark));
	::ZeroMemory(strHaveMap, sizeof(strHaveMap));
	::ZeroMemory(strItem, sizeof(strItem));

	int db_index = 0;
	CDatabase* pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB )
	{
		ReleaseGuild();
		return FALSE;
	}

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		TRACE("Fail To Load Guild Data !!\n");
		ReleaseGuild();
		return FALSE;
	}

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, SQL_NTS);

	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		while (TRUE)
		{
			retcode = SQLFetch(hstmt);

			if (retcode ==SQL_SUCCESS || retcode ==SQL_SUCCESS_WITH_INFO)
			{
				i = 1;

				SQLGetData( hstmt, i++, SQL_C_SLONG, &iSID, sizeof(iSID), &sInd );
				SQLGetData( hstmt, i++, SQL_C_CHAR, &strGuildName, sizeof(strGuildName), &sInd );
				SQLGetData( hstmt, i++, SQL_C_CHAR, &strMasterName, sizeof(strMasterName), &sInd );
				SQLGetData( hstmt, i++, SQL_C_BINARY, &strMark, sizeof(strMark), &sInd );
				SQLGetData( hstmt, i++, SQL_C_SSHORT, &sVERSION, sizeof(sVERSION), &sInd );
				SQLGetData( hstmt, i++, SQL_C_ULONG, &iDN, sizeof(iDN), &sInd );

				SQLGetData( hstmt, i++, SQL_C_BINARY, &strItem,	sizeof(strItem), &sInd );
				SQLGetData( hstmt, i++, SQL_C_BINARY, &strHaveMap, sizeof(strHaveMap), &sInd );

				if(iSID <= 0 || iSID >= MAX_GUILD ) 
				{
					TRACE(_T("±æµå ÀüÃ¼ Å©±â°¡ MAX_GUILD¸¦ ³Ñ¾ú´Ù."));
					continue;
				}

				pNewGuild = NULL;
				pNewGuild = g_arGuildData[iSID];

				pNewGuild->m_lSid = iSID;
				strcpy(pNewGuild->m_strGuildName, (CHAR*)strGuildName);
				strcpy(pNewGuild->m_strMasterName, (CHAR*)strMasterName);
				pNewGuild->m_sVersion = sVERSION;
				pNewGuild->m_dwGuildDN = iDN;

				pNewGuild->StrToGuildItem((LPTSTR)strItem);
				pNewGuild->strToGuildMap((LPTSTR)strHaveMap);
				::CopyMemory(pNewGuild->m_strMark, strMark, sizeof(pNewGuild->m_strMark));

				g_CurrentGuildCount = iSID;

				::ZeroMemory(strGuildName, sizeof(strGuildName));
				::ZeroMemory(strMasterName, sizeof(strMasterName));
				::ZeroMemory(strMark, sizeof(strMark));
				::ZeroMemory(strHaveMap, sizeof(strHaveMap));
				::ZeroMemory(strItem, sizeof(strItem));
			}
			else break;
		}		
	}
	else if (retcode==SQL_NO_DATA)
	{
		g_DB[m_iModSid].ReleaseDB(db_index);
		ReleaseGuild();
		return FALSE;
	}
	else
	{
		retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		g_DB[m_iModSid].ReleaseDB(db_index);
		ReleaseGuild();
		return FALSE;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(db_index);

	GetAbsentGuildUserInfo( guildsid );

	ReleaseGuild();

	return TRUE;	
}

void USER::GetAbsentGuildUserInfo(int guildsid)
{
	int i;
	SQLHSTMT		hstmt;
	SQLRETURN		retcode;
	TCHAR			szSQL[2048];

	::ZeroMemory(szSQL, sizeof(szSQL));
	wsprintf(szSQL,TEXT("SELECT * FROM GUILD_USER where iSid = %d"), guildsid);

	SQLINTEGER		iSID;
	SQLCHAR			strUserId[CHAR_NAME_LENGTH + 1];

	SQLINTEGER		sInd;

	iSID = 0;
	::ZeroMemory(strUserId, sizeof(strUserId));

	int db_index = 0;
	CDatabase* pDB = g_DB[m_iModSid].GetDB( db_index );
	if( !pDB ) return;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if( retcode != SQL_SUCCESS )
	{
		TRACE("Fail To Load Guild User Data !!\n");
		return;
	}

	retcode = SQLExecDirect( hstmt, (unsigned char*)szSQL, SQL_NTS);
	if( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO )
	{
		while (TRUE)
		{
			retcode = SQLFetch(hstmt);

			if (retcode ==SQL_SUCCESS || retcode ==SQL_SUCCESS_WITH_INFO)
			{
				i = 1;

				SQLGetData( hstmt, i++, SQL_C_SLONG, &iSID, sizeof(iSID), &sInd );
				SQLGetData( hstmt, i++, SQL_C_CHAR, &strUserId, sizeof(strUserId), &sInd );

				if(iSID < 0 || iSID >= MAX_GUILD ) continue;

				if(!g_arGuildData[iSID]) continue;

				g_arGuildData[iSID]->AddUser((TCHAR *)strUserId, iSID);
			}
			else break;
		}		
	}
	else if (retcode==SQL_NO_DATA)
	{
		g_DB[m_iModSid].ReleaseDB(db_index);
		return;
	}
	else
	{
		retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
		g_DB[m_iModSid].ReleaseDB(db_index);
		return;
	}

	retcode = SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	g_DB[m_iModSid].ReleaseDB(db_index);

	return;
}

BOOL USER::IsMyDBServer(int myserver)
{
	for( int i = 0; i < g_arMyServer.GetSize(); i++ )
	{
		
		if( g_arMyServer[i] )
		{
			TRACE( "g_arMyServer: %d\n", *(g_arMyServer[i]));
			if( *(g_arMyServer[i]) == myserver )
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}
//ÓÎÏ·ÈËÎïÉÏÏß
void USER::BridgeServerUserLogin(char *strAccount, char *strUserID)
{
	if(strAccount == NULL || strUserID == NULL) return;//ÕËºÅ¸ú½ÇÉ«Îª¿ÕÔò·µ»Ø MarkJay
	if( g_bShutDown ) return;
	CString strMsg1 = _T("");
	int			index = 0;
	BYTE		result = FAIL, error_code = 0;
	TCHAR		szID[CHAR_NAME_LENGTH+1];
	CPoint		pt;
	CBufferEx	TempBuf;
	CBufferEx	TempBuf3;

	int			iMemory = 0;
	int			iMemoryAccountBank = 0;

	strcpy( szID, strUserID );

	InitUser();
	iMemory = CheckMemoryDB( szID );
	if( iMemory == 0 || iMemory == 2 )
	{
		if( !LoadUserData( szID ) )
		{
			//AfxMessageBox("User Data Load Fail");
			error_code = ERR_2;
			goto result_send;
		}
	}

	m_state = STATE_CONNECTED;

#ifndef _EVENT_RR 
	if( m_curz == g_RR.m_iZoneNum && CheckInvalidMapType() == 12 )
	{
		m_curz = 57;
		SetZoneIndex( m_curz );

		pt = ConvertToServer( 20, 34 );
		m_curx = pt.x;
		m_cury = pt.y;
	}
#else		// ÀÓ½Ã·Î ÃÖ°­ ÀÌº¥Æ®¶§¹®¿¡
	if( m_curz == g_RR.m_iZoneNum || m_curz == 61 || m_curz == 62 || m_curz ==  63)
	{
		if( CheckInvalidMapType() == 12 )
		{
			m_curz = 57;
			SetZoneIndex( m_curz );

			pt = ConvertToServer( 20, 34 );
			m_curx = pt.x;
			m_cury = pt.y;
		}
	}
#endif

	pt = FindNearAvailablePoint_S(m_curx, m_cury);
	if(pt.x == -1 || pt.y == -1) 
	{
		////////////////ÈËÎïÉÏÏßµØÀí×ø±ê MarkJay±ê¼Ç////////////////////
		m_curz = 1;
		m_curx = 192;
		m_cury = 803;

		if( !IsZoneInThisServer( m_curz ) )
		{
			UpdateUserData( TRUE );
			error_code = ERR_4;
			goto result_send;
		}

		SetZoneIndex(m_curz);
		pt = FindNearAvailablePoint_S(m_curx, m_cury);	// DB¿¡ ÀúÀåµÈ ÁÂÇ¥°¡ ¿òÁ÷ÀÏ ¼ö ÀÖ´Â ÁÂÇ¥ÀÎÁö ÆÇ´Ü
	}

	m_curx = pt.x; m_cury = pt.y;
	SetUid(m_curx, m_cury, m_uid + USER_BAND );		// À¯Àú À§Ä¡Á¤º¸ ¼ÂÆÃ
	m_presx = -1;
	m_presy = -1;

	pt = ConvertToClient(m_curx, m_cury);

//	SetUserToMagicUser();
	GetMagicItemSetting();

	m_UserFlag = TRUE;

	result = SUCCESS;

	m_ConnectionSuccessTick = 0;

	InitMemoryDB(m_uid);

	if( iMemory == 0 || iMemory == 2 )
	{
		if( !LoadUserBank() )
		{
			error_code = ERR_2;
			result = FAIL;
			goto result_send;
		}
		if( !LoadAccountBank() )
		{
			error_code = ERR_2;
			result = FAIL;
			goto result_send;
		}
	}

result_send:
	TempBuf.Add(GAME_START_RESULT);
	TempBuf.Add(result);

	if(result != SUCCESS)
	{
		ReInitMemoryDB();

		TempBuf.Add(error_code);
		Send(TempBuf, TempBuf.GetLength());
//		SockCloseProcess();
		return;
	}

	SendCharData();
	TempBuf.Add((short)m_curz);
	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.Add((short)pt.x);
	TempBuf.Add((short)pt.y);
	TempBuf.Add(m_tDir);
	TempBuf.Add((BYTE)g_GameTime);
	TempBuf.Add((BYTE)g_GameMinute);
	Send(TempBuf, TempBuf.GetLength());		
	SetGameStartInfo();
	SetCardTime();//Ð´Èë³äÖµ¿¨Ê±¼äµ½½ÇÉ« MarkJay
	SetHuiYuanTime();//Ð´Èë»áÔ±Ê±¼äµ½½ÇÉ« MarkJay
	GetCheckValidTime();
	strMsg1.Format( IDS_USER_OPERATOR_MAIL1, f_Markini[0]->GameName);
	SendSystemMsg((LPTSTR)(LPCTSTR)strMsg1, SYSTEM_NORMAL, TO_ME);
	SendSystemMsg("±¾·þÎªÂÌÉ«°æ±¾,³¤ÆÚÎÈ¶¨!", SYSTEM_NORMAL, TO_ME);
	if(m_bLock == 1)
	{
		SendSystemMsg("ÄúµÄ½ÇÉ«ÒÑ±»·â,ÇëÁªÏµ¹ÜÀíÔ±!", SYSTEM_SPECIAL, TO_ME);//·âºÅÌáÊ¾ÌáÊ¾
		SoftClose();
	}
	if(f_Markini[0]->SBEXP_KG == 1)
	{
		SendSystemMsg("ÏÖÔÚÈ«·þ¿ªÆô´ò¹Ö3±¶¾­Ñé!ÆÕÍ¨¹ÖÎïµôÂäÎïÆ·ÓÉ3¼þÌáÉýÎªµô4¼þ.×£´ó¼ÒÓÎÏ·Óä¿ì!",SYSTEM_ANNOUNCE,TO_ME);
	}
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>ÒªÈû³á°ò
	SetIsGuildCB();
	SetStatus();
	if(f_Markini[0]->CH_lV_KG == 1)//³ÆºÅÏÔÊ¾ÅÅÐÐ MarkJay
	{
		ChangeLoveName();
	}
//vip
	/*if(m_dwBFindTime > 0)//²âÊÔÍ·¶¥´øNEW MarkJay
	{
		AddNewStatus(New_Status_22);
		SendABNORMAL();
	}*/
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>³á°ò½ä×Ó
	if(f_Markini[0]->RenShu_KG == 1)
	{
		if(m_tIsOP != 1)
		{
			int nCount2 = 0;
			USER *pUser = NULL;
			for(int i = 0; i < MAX_USER; i++)
			{
				pUser = m_pCom->GetUserUid(i);
				if(pUser && pUser->m_state == STATE_GAMESTARTED )nCount2++;
			}
			nCount2 = nCount2 * 3;
			CString msg =_T("");
			CString strMsg2 =_T("");
			strMsg2.Format( "Lv.%d[%s]µÇÂ½ÓÎÏ·!",m_sLevel, strUserID);
			SendSystemMsg((LPTSTR)(LPCTSTR)strMsg2, SYSTEM_NORMAL, TO_ALL);
			msg.Format("µ±Ç°ÔÚÏßÈËÊý:[ %d ]",nCount2);
			SendSysWhisperChat(msg.GetBuffer(0));
		}
	}
	GetLocalTime(&m_LoginTime);		// ·Î±×ÀÎÇÑ ½Ã°£À» ¼³Á¤
}

void USER::SetIsGuildCB()//ÒªÈû³á°ò
{
	CGuildFortress* pFort = NULL;
	int Fortress[] = {f_Markini[0]->JT_Fortressid};
	if(m_dwGuild <= 0) return;
	for(int i = 0; i < 3;i++)
	{
		pFort = GetFortress(Fortress[i]);
		if(pFort == NULL) continue;
		if(pFort->m_iGuildSid <= 0)	return;		
		if(pFort->m_iGuildSid != m_dwGuild)	continue;
		if(pFort->m_iGuildSid == m_dwGuild) isGuildRen = true;
	}
}
void USER::BridgeServerUserZoneLogin(char *strAccount, char *strUserID)
{
	if( g_bShutDown ) return;

	int			index = 0;
	BYTE		result = FAIL, error_code = 0;
	TCHAR		szID[CHAR_NAME_LENGTH+1];
	CPoint		pt;
	CBufferEx	TempBuf;
	int			iMemory = 0;
	int			iMemoryAccountBank = 0;

	strcpy( szID, strUserID );

	InitUser();
//	InitMemoryDB(m_uid);
	iMemory = CheckMemoryDB( szID );
//	iMemory = 0;		// ¾ÆµÚ °°Àº °Íµµ ¾ø¾ú°í, °èÁ¤ °°Àº °Íµµ ¾ø¾ú´Ù.
//						// ¾ÆµÚ,ÀºÇà,ÅëÃ¢ ¸ðµÎ DB¿¡¼­ °¡Á®¿Í¾ß ÇÑ´Ù.

//	iMemory = 1;		// ¾ÆµÚ¶û °èÁ¤ÀÌ °°Àº °ÍÀº ÀÖ¾ú°í, °èÁ¤¸¸ °°°í ¾ÆµÚ°¡ ´Ù¸¥°ÍÀº ¾ø¾ú´Ù.
//						// ¾ÆµÚ,ÀºÇà,ÅëÃ¢ ¸ðµÎ MemoryDB¿¡¼­ °¡Á®¿Ô´Ù. DB¿¡¼­ °¡Á®¿ÀÁö ¾Ê´Â´Ù.

//	iMemory = 1;		// ¾ÆµÚ¶û °èÁ¤ÀÌ °°Àº °ÍÀÌ ÀÖ¾ú°í, °èÁ¤¸¸ °°°í ¾ÆµÚ°¡ ´Ù¸¥°Íµµ ÀÖ¾ú´Ù.
//						// ¾ÆµÚ,ÀºÇà,ÅëÃ¢ ¸ðµÎ MemoryDB¿¡¼­ °¡Á®¿Ô´Ù. DB¿¡¼­ °¡Á®¿ÀÁö ¾Ê´Â´Ù.

//	iMemory = 2;		// ¾ÆµÚ¶û °èÁ¤ÀÌ °°Àº °ÍÀº ¾ø¾ú°í, °èÁ¤¸¸ °°Àº °ÍÀÌ ÀÖ¾ú´Ù.
//						// ¾ÆµÚ,ÀºÇà,ÅëÃ¢ ¸ðµÎ DB¿¡¼­ °¡Á®¿Í¾ß ÇÑ´Ù. (³ªÁß¿¡ ÅëÃ¢¸¸ MemoryDB¿¡¼­ °¡Á®¿À´Â ·çÆ¾À» ³ÖÀ»°ÍÀÌ´Ù.)

	if( iMemory == 0 || iMemory == 2 )
	{
		if( !LoadUserData( szID ) )
		{
			error_code = ERR_2;
			goto result_send;
		}
	}

	m_state = STATE_CONNECTED;

	pt = FindNearAvailablePoint_S(m_curx, m_cury);	// DB¿¡ ÀúÀåµÈ ÁÂÇ¥°¡ ¿òÁ÷ÀÏ ¼ö ÀÖ´Â ÁÂÇ¥ÀÎÁö ÆÇ´Ü
	if(pt.x == -1 || pt.y == -1) 
	{
		error_code = ERR_4;
		goto result_send;
	}

//	CheckUserLevel();					// ¹«·áÃ¼Çè·¹º§ÀÎ 25·¹º§À» ³Ñ¾úÀ» °æ¿ì 

	m_curx = pt.x; m_cury = pt.y;
	SetUid(m_curx, m_cury, m_uid + USER_BAND );		// À¯Àú À§Ä¡Á¤º¸ ¼ÂÆÃ
	m_presx = -1;
	m_presy = -1;

	pt = ConvertToClient(m_curx, m_cury);

	GetMagicItemSetting();							// ÇöÀç ¾ÆÀÌÅÛÁß ¸ÅÁ÷ ¼Ó¼º, ·¹º§º¯µ¿À» µ¿Àû º¯¼ö¿¡ ¹Ý¿µÇÑ´Ù.

	m_UserFlag = TRUE;

	result = SUCCESS;

	m_ConnectionSuccessTick = 0;


	InitMemoryDB(m_uid);

	if( iMemory == 0 || iMemory == 2 )
	{
		if( !LoadUserBank() )
		{
			error_code = ERR_2;
			result = FAIL;
			goto result_send;
		}
		if( !LoadAccountBank() )
		{
			error_code = ERR_2;
			result = FAIL;
			goto result_send;
		}
	}

	// Á¸ Ã¼ÀÎÁö È®ÀÎ¹× º¯¼ö ÃÊ±âÈ­...
	m_strZoneIP = "";
	m_nZonePort	= -1;

	SetPsiAbnormalStatus();
	SetStatus();//Ë¢ÐÂ

result_send:
	TempBuf.Add(GAME_START_RESULT);
	TempBuf.Add(result);

	if(result != SUCCESS)
	{
		ReInitMemoryDB();

		TempBuf.Add(error_code);
		Send(TempBuf, TempBuf.GetLength());
		return;
	}
	
	SendCharData();						// À¯ÀúÀÇ »ó¼¼ Á¤º¸¸¦ º¸³½´Ù.

	TempBuf.Add((short)m_curz);
	TempBuf.Add(m_uid + USER_BAND);
	TempBuf.Add((short)pt.x);
	TempBuf.Add((short)pt.y);

	TempBuf.Add(m_tDir);

	TempBuf.Add((BYTE)g_GameTime);
	TempBuf.Add((BYTE)g_GameMinute);

	Send(TempBuf, TempBuf.GetLength());

	SetGameStartInfo();	


	GetLocalTime(&m_LoginTime);		// ·Î±×ÀÎÇÑ ½Ã°£À» ¼³Á¤
	GetCheckValidTime();			// ÑéÖ¤Ê±¼ä
}

void USER::BridgeServerUserWhisper(char *strSendID, char *strRecvID, char *strMsg)
{
	if(m_tIsOP == 1)						// ¿î¿µÀÚ¿¡°Ô ±Ó¸»Àº ¾ÈµÊ
	{
		return;
	}

	if( !strcmp( strSendID, strRecvID ) )	// ÀÚ±âÀÚ½Å¿¡°Ô ±Ó¸» -> ¸®ÅÏ
	{
		return;
	}

	CBufferEx TempBuf;

	TempBuf.Add(CHAT_RESULT);
	TempBuf.Add(SUCCESS);
	TempBuf.Add(WHISPER_CHAT);
	TempBuf.Add((int)0x01);								// Client¿¡¼­ ¼­·Î ±¸ºÐÇÏ±âÀ§ÇØ(»ó´ë¹æ ±Ó¸» ¾ÆÀÌµð)
	TempBuf.AddString(strSendID);
	TempBuf.AddString(strMsg);			// pBuf == IDLen(1) + ID + MsgLen(1) + Msg
	Send(TempBuf, TempBuf.GetLength());
}

void USER::BridgeServerGuildNewResult(int guildnum, char *strGuildName)
{
	BYTE error_code = 0;
	BOOL bRes = TRUE;

	int i;
	int index = 0;
	int nLength = 0;
	int iGuildId = 0;

	CBufferEx	TempBuf;

	WORD *pwMark = NULL;
	CGuild *pGuild = NULL;

	nLength = strlen( strGuildName );
	if(nLength <= 0 || nLength > CHAR_NAME_LENGTH) { error_code = ERR_1; goto go_result; } // ±æµå ¸íÄª Á¦ÇÑ¿¡ °É¸²
	if(!g_arGuildData[guildnum]) { error_code = ERR_1; goto go_result; }

	EnterCriticalSection( &(m_pCom->m_critGuild) );

	pGuild = g_arGuildData[guildnum];

	pGuild->m_lSid = guildnum;

	if(pGuild == NULL) { error_code = ERR_1; goto go_result; }//MarkJay ÐÞ¸Ä

	strcpy(pGuild->m_strGuildName, strGuildName);
	strcpy(pGuild->m_strMasterName, m_strUserID);

	pGuild->m_dwGuildDN = 0;
	pGuild->m_sVersion = -1;

	pwMark = (WORD*)(&pGuild->m_strMark[0]);
	for (i=0; i<GUILD_MARK_SIZE/sizeof(WORD); i++) {
		pwMark[i] = (0x1f<<11 | 0x1f);	// Client¿¡¼­ »ç¿ëÇÏ´Â Åõ¸í»öÀ¸·Î ÃÊ±âÈ­ÇÑ´Ù.
	}

	if(!InsertGuild(pGuild))
	{
		LeaveCriticalSection( &(m_pCom->m_critGuild) );
		ReleaseGuild();//MarkJay ÐÞ¸Ä
		return;
	}

	pGuild->AddUser( m_strUserID, guildnum );

	LeaveCriticalSection( &(m_pCom->m_critGuild) );

	if( m_dwDN <= GUILD_MAKE_DN ) m_dwDN = 0;
	else m_dwDN -= GUILD_MAKE_DN;		

	m_bGuildMaster = TRUE;
	m_dwGuild = guildnum;

	nLength = 0;
	nLength = strlen( strGuildName );
	if(nLength > 0 && nLength < CHAR_NAME_LENGTH)
	{
		strncpy(m_strGuildName, strGuildName, nLength);
	}

	ReleaseGuild();//MarkJay ÐÞ¸Ä
	bRes = FALSE;

go_result:
	//////////////////////////////PACKET///////////////////////////////////////
	TempBuf.Add(GUILD_OPEN_RESULT);

	if(bRes)
	{
		TempBuf.Add((BYTE)0x00);		// ½ÇÆÐ
		TempBuf.Add(error_code);
		Send(TempBuf, TempBuf.GetLength());
		return;
	}

	TempBuf.Add((BYTE)0x01);		// ¼º°ø
	TempBuf.Add((int)iGuildId);
	TempBuf.AddString(m_strGuildName);

	Send(TempBuf, TempBuf.GetLength());

	SendMoneyChanged();
	SendMyGuildInfo();//(TO_INSIGHT, INFO_MODIFY);
}

void USER::BridgeServerGuildDisperseResult(int guildnum)
{
	int i;//, j;
	int index = 0, nLen = 0;
	BYTE error_code = 0;

	USER *pUser = NULL;
	CGuild *pGuild = NULL;
	CGuildUser *pGuildUser = NULL;

	CStore *pStore = NULL;
	CGuildFortress *pFort = NULL;

	if( guildnum <= 0 || guildnum >= g_arGuildData.GetSize() ) return;

	if( guildnum != (int)m_dwGuild ) return;

	pGuild = GetGuild( m_dwGuild );

	if( !DeleteGuildDB() ) 				// DB »èÁ¦
	{
		ReleaseGuild();
		::InterlockedExchange(&g_arGuildData[m_dwGuild]->m_lUsed, 0);
		return;
	}
	index = m_dwGuild;

	for(i = 0; i < g_arGuildHouse.GetSize(); i++)
	{
		if(m_dwGuild == g_arGuildHouse[i]->iGuild)
		{
			CNpc *pNpc = NULL;				// ¹®ÆÐµµ...
			pNpc = GetNpc(g_arGuildHouse[i]->iMarkNpc);
			if(pNpc)
			{
				pNpc->m_sPid = 0;
				pNpc->m_sMaxHP = 1;
				::ZeroMemory(pNpc->m_strName, sizeof(pNpc->m_strName));
			}

			g_arGuildHouse[i]->iGuild = 0;
			break;
		}
	}

	// ¿ä»õ°¡ ÀÖ´Ù¸é ÀÌ¿ëºÒ°¡·Î ¸¸µé¾î ÁØ´Ù.
	for(i = 0; i < g_arGuildFortress.GetSize(); i++)
	{
		if(!g_arGuildFortress[i]) continue;

		pFort = g_arGuildFortress[i];

		if(pFort == NULL) continue;//MarkJay ÐÞ¸Ä

		if(pFort->m_iGuildSid == m_dwGuild)
		{
			if(pFort->m_lViolenceUsed == 1)
			{
				pFort->m_wMopPartyTime.wYear = 2030;		// ±æµå°¡ ÇØ»êµÇ¸é¼­ ³ª¸ÓÁö´Â ¸ðµÎ ÃÊ±âÈ­¸¦
				pFort->SetNpcToFortressViolenceEnd(m_pCom);
				pFort->GetOutof(m_pCom);
			}

			pFort->SetInitFortress();
			pFort->InitMemFortress(GUILD_WAR_DECISION);
			break;
		}
	}

	g_arGuildData[m_dwGuild]->InitGuild();

	ReleaseGuild();

	for(i = 0; i < MAX_USER; i++)		// ±æµå¿øµé ¼ÂÆÃ
	{
		pUser = m_pCom->GetUserUid(i);

		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) continue;
		if(pUser->m_dwGuild <= 0) continue;

		if(pUser->m_dwGuild == index)
		{
			pUser->SendSystemMsg( IDS_USER_DISMISS_COMPLETED, SYSTEM_NORMAL, TO_ME);//"¾üÍÅ±»½âÉ¢ÁË."

			pUser->m_dwGuild = -1;							// ÃÊ±âÈ­ÇÑ´Ù.
			pUser->m_sGuildVersion = -1;					// ±æµå ¹®¾ç ¹öÁ¯
			pUser->m_dwGuildLevel = -1;
			::ZeroMemory(pUser->m_strGuildName, sizeof(pUser->m_strGuildName));
			pUser->m_bGuildMaster = FALSE;					// ±æµå ¸¶½ºÅÍ
			pUser->m_bRepresentationGuild = FALSE;			// ±ÇÇÑ ´ëÇà ¿©ºÎ

			pUser->m_bFieldWarApply = FALSE;
			pUser->m_tGuildWar = GUILD_WAR_AFFTER;
			pUser->m_tGuildHouseWar = GUILD_WAR_AFFTER;
			pUser->m_tFortressWar = GUILD_WAR_AFFTER;

			pUser->SendMyGuildInfo();
		}
	}
	// DB¿¡¼­ »èÁ¦..
	// Á¢¼ÓÇÑ À¯Àú¿¡¼­ »èÁ¦...
	// ¿ì¼± ¸Þ¸ð¸®¿¡¼­ »èÁ¦... (Guild, Guild_House_Rank,

	UpdateUserData();
}

void USER::BridgeServerGuildInviteResult(int guid, char *strGuestID)
{
	int index = 0;
	BYTE error_code = 0;

	int nLen = 0;

	int uid = 0;
	int iCount = 0;
	USER* pUser = NULL;
	CGuild *pGuild = NULL;

	BOOL bRes = TRUE;

	uid = guid;

	pUser = GetUser( uid );

	if(!pUser) return;

	nLen = strlen(pUser->m_strUserID);
	if(nLen <= 0) return;

	if(pUser->m_dwGuild > 0) return;

	if(CheckInGuildWarring()) return;								// ±æÀüÁß¿¡´Â Çã¶ôÁA¼ö¾ø´Ù.

	pGuild = GetGuild( m_dwGuild );

	if(!pGuild)
	{
		ReleaseGuild();				// ÇØÁ¦...
		return;
	}
									// ¿À·ù...
//	if(strcmp(pGuild->m_strMasterName, m_strUserID) != 0)
	if(!pGuild->IsMasterPower(m_strUserID))
	{
		ReleaseGuild();				// ÇØÁ¦...
		return;
	}

	index = -1;
	index = pGuild->GetUser( pUser->m_strUserID );
	if(index >= 0)
	{
		ReleaseGuild();				// ÇØÁ¦...

		pUser->m_dwGuild = m_dwGuild;			// ±æµå ¹øÈ£¸¦ ´Ù½Ã ¼ÂÆÃÇÑ´Ù.
		SendGuildInfo(pUser);

		error_code = ERR_9;
		goto go_result;
	}// ÀÌ¹Ì ´Ù¸¥ ±æµå¿¡ °¡ÀÔÇÑ À¯Àú

	if(!pGuild->AddUser(pUser->m_strUserID, pGuild->m_lSid))
	{
		ReleaseGuild();				// ÇØÁ¦...
		error_code = ERR_11;			//
		goto go_result;
	}

	if(!InsertGuildUser(pGuild->m_lSid, pUser->m_strUserID)) // ÇØ´ç ±æµå¿¡ °¡ÀÔÀ» ¼ÂÆÃ
	{
		pGuild->RemoveUser(pUser->m_strUserID);

		ReleaseGuild();				// ÇØÁ¦...
		error_code = 255;			//
		goto go_result;
	}

	pUser->m_dwGuild = pGuild->m_lSid;
	pUser->m_bGuildMaster = FALSE;
	pUser->m_sGuildVersion = pGuild->m_sVersion;
	pUser->m_bRepresentationGuild = FALSE;
	strcpy(pUser->m_strGuildName, pGuild->m_strGuildName);
	bRes = FALSE;

go_result:
	if(bRes)
	{
		CBufferEx TempBuf;
		TempBuf.Add(CHAT_RESULT);
		TempBuf.Add((BYTE)0x00);		//½ÇÆÐ
		TempBuf.Add(error_code);
		Send(TempBuf, TempBuf.GetLength());
		return;
	}

	ReleaseGuild();

	UpdateUserData();

	AddGuildUserInFortress(pUser);

	SendGuildInviteUser(pUser);			// ±æµå ¸®½ºÆ®¸¦ ÁÖ°í ¹Þ±â
	SendGuildInfo(pUser);				// ½Ã¾ß ¹üÀ§¿¡ ±æµå¿¡ °¡ÀÔÇÑ »ç¶÷ÀÌ ÀÖÀ¸´Ï±ñ ¹®¾çÀ» º¸ÀÌ¶ó°í ÇÑ´Ù...
}

void USER::BridgeServerGuildOffResult(char *strReqID, char *strOffID)
{
	int index = 0;
	BYTE error_code = 0;
	CString strTemp;

	USER *pUser = NULL;
	CGuild *pGuild = NULL;
	CGuildUser *pGuildUser = NULL;

	BOOL bRes = TRUE;

	if(m_dwGuild <= 0) return;

	if(m_bGuildMaster)	// ¿äÃ»ÇÑ »ç¶÷ÀÌ ±æ¸¶ÀÏ¶§
	{
		if( !strcmp( strReqID, strOffID ) )	// ¿äÃ»ÇÑ ¾ÆÀÌµð¿Í Å»ÅðÇÒ ¾ÆÀÌµð°¡ °°´Ù¸é... ¿¡·¯´Ù
		{
			return;
		}
	}

	if(m_tGuildHouseWar == GUILD_WARRING || m_tGuildWar == GUILD_WARRING || m_tFortressWar == GUILD_WARRING)
	{
		return;
	}

	if(CheckInGuildWarring()) return;								// ±æÀüÁß¿¡´Â Çã¶ôÁA¼ö¾ø´Ù.

	pGuild = GetGuild(m_dwGuild);

	if(!pGuild)
	{
		ReleaseGuild();				// ÇØÁ¦...
		return;
	}

	index = -1;
	index = pGuild->GetUser( strOffID );

	if(index < 0)					// ÇØ´ç ±æµå¿¡¼­ À¯Àú°¡ ¾øÀ¸¸é...
	{
		ReleaseGuild();
		return;
	}

	if(!pGuild->RemoveUser( strOffID ))
	{
		ReleaseGuild();
		error_code = ERR_8;
		goto go_result;
	}

	if(!DeleteGuildUser(strOffID))		// Å×ÀÌºí¿¡¼­ Áö¿î´Ù.
	{
		pGuild->AddUser(strOffID, m_dwGuild);

		ReleaseGuild();
		error_code = 255;
		goto go_result;
	}

	if(pGuild->m_lSubMaster == 1)
	{
		if( !strcmp(pGuild->m_strSubMasterName, strOffID) )
		{
			CString strMsg = _T("");
			strMsg.Format(IDS_USER_GUILD_SUBMASTER_OFF, pGuild->m_strSubMasterName);// "%s ±»È¡Ïûµ£ÈÎ¾üÍÅ¸±ÍÅ³¤."
			SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);

			pGuild->RemoveSubGuildMaster();
		}
	}

	ReleaseGuild();
	bRes = FALSE;

go_result:
	if(bRes)
	{
		CBufferEx TempBuf;
		TempBuf.Add(CHAT_RESULT);
		TempBuf.Add((BYTE)0x00);
		TempBuf.Add(error_code);
		Send(TempBuf, TempBuf.GetLength());
		return;
	}

	if( !strcmp( strReqID, strOffID ) )		// ÀÚ°¡ Å»Åð
	{
		SendGuildOffUser( this );				// ´Ù¸¥ ±æµå¿ø¿¡°Ô ³»°¡ Å»ÅðÇÑ »ç½ÇÀ» ¾Ë¸°´Ù.

		DelGuildUserInFortress( strOffID, m_dwGuild );

		m_dwGuild = -1;							// ÃÊ±âÈ­ÇÑ´Ù.
		m_sGuildVersion = -1;					// ±æµå ¹®¾ç ¹öÁ¯
		m_dwGuildLevel = -1;
		::ZeroMemory(m_strGuildName, sizeof(m_strGuildName));
		//----------------------------------------------------
		//yskang 0.1 ±æµå Å»Åð½Ã È£Äª »èÁ¦
		ZeroMemory(m_strLoveName, sizeof(m_strLoveName));
		ZeroMemory(m_pMD->m_strLoveName,sizeof(m_pMD->m_strLoveName));
		//----------------------------------------------------
		//È£ÄªÀ» ´Ù¸¥ »ç¶÷µé¿¡°Ô ¾Ë¸°´Ù(°°Àº ½ºÅ©¸°¿¡ ÀÖ´Â»ç¶÷µé¿¡ ÇÑÇØ¼­?)
		if(f_Markini[0]->CH_lV_KG == 1)//³ÆºÅÏÔÊ¾ÅÅÐÐ MarkJay
		{
			ChangeLoveName();
		}
		else
		{
			CBufferEx TempBuf;
			TempBuf.Add(LOVE_NAME);
			TempBuf.Add(m_uid+USER_BAND);
			TempBuf.AddString(m_strLoveName);
			SendExactScreen(TempBuf, TempBuf.GetLength());
		}
		//--------------------------------------------------------------
		m_bGuildMaster = FALSE;					// ±æµå ¸¶½ºÅÍ
		m_bRepresentationGuild = FALSE;			// ±ÇÇÑ ´ëÇà ¿©ºÎ

		SendMyGuildInfo();						// ±æµå Å»Åð¸¦ ¾Ë¸°´Ù.

		UpdateUserData();
	}
	else									// °­Á¦ Ãß¹æ
	{
		pUser = GetUser( strOffID );

		if( pUser )
		{
			if( pUser->m_state == STATE_GAMESTARTED )
			{
				pUser->m_dwGuild = -1;
				pUser->m_sGuildVersion = -1;		// ±æµå ¹®¾ç ¹öÁ¯
				pUser->m_dwGuildLevel = -1;
				::ZeroMemory(pUser->m_strGuildName, sizeof(pUser->m_strGuildName));
				//----------------------------------------------------
				//yskang 0.1 ±æµå Å»Åð½Ã È£Äª »èÁ¦
				ZeroMemory(pUser->m_strLoveName, sizeof(pUser->m_strLoveName));
				ZeroMemory(pUser->m_pMD->m_strLoveName,sizeof(pUser->m_pMD->m_strLoveName));
				//È£ÄªÀ» ´Ù¸¥ »ç¶÷µé¿¡°Ô ¾Ë¸°´Ù(°°Àº ½ºÅ©¸°¿¡ ÀÖ´Â»ç¶÷µé¿¡ ÇÑÇØ¼­?)
				if(f_Markini[0]->CH_lV_KG == 1)//³ÆºÅÏÔÊ¾ÅÅÐÐ MarkJay
				{
					pUser->ChangeLoveName();
				}
				else
				{
					CBufferEx TempBuf;
					TempBuf.Add(LOVE_NAME);
					TempBuf.Add(pUser->m_uid + USER_BAND);
					TempBuf.AddString(pUser->m_strLoveName);
					pUser->SendExactScreen(TempBuf, TempBuf.GetLength());
				}
				//----------------------------------------------------
				pUser->m_bGuildMaster = FALSE;			// ±æµå ¸¶½ºÅÍ
				pUser->m_bRepresentationGuild = FALSE;	// ±ÇÇÑ ´ëÇà ¿©ºÎ
				pUser->SendMyGuildInfo();
				pUser->SendSystemMsg( IDS_USER_KICKOUT_GUILD, SYSTEM_NORMAL, TO_ME);// "Äã±»Ìß³öÁË¾üÍÅ."

				SendGuildOffUser( pUser );

				pUser->UpdateUserData();
			}
		}

		strTemp.Format( IDS_USER_KICKOUT_MEMBER, strOffID);//"%s ±»Ìß³öÁË¾üÍÅ."
		SendSystemMsg((LPTSTR)(LPCTSTR)strTemp, SYSTEM_NORMAL, TO_ME);
	}
}
//////////////////////////////////////////////////////////////////////////
// ¸±ÍÅÊÚÈ¨
void USER::BridgeDeputeGuildMasterOfPowerResult(int guid)
{
	int index = 0;
	BYTE error_code = 0;

	int nLen = 0;

	int uid = 0;
	int iCount = 0;
	USER* pUser = NULL;
	CGuild *pGuild = NULL;

	BOOL bRes = TRUE;

	if(!m_bGuildMaster) return; 	// ±æµå ±ÇÇÑÀÌ ¾ø´Ù.

	uid = guid;

	pUser = GetUser( uid );

	if(!pUser || pUser->m_state != STATE_GAMESTARTED) return;	// À¯Àú°¡ ¾ø´Ù.

	nLen = strlen(pUser->m_strUserID);
	if(nLen <= 0 || nLen > CHAR_NAME_LENGTH) return;

	if(pUser->m_dwGuild != m_dwGuild) return;

	if(CheckInGuildWarring()) return;							// ±æÀüÁß¿¡´Â Çã¶ôÁA¼ö¾ø´Ù.

	CString strMsg = _T("");

	pGuild = GetGuild( m_dwGuild );

	if(!pGuild)
	{
		ReleaseGuild();				// ÇØÁ¦...
		return;
	}
									// ¿À·ù...
	if(strcmp(pGuild->m_strMasterName, m_strUserID) != 0)
	{
		ReleaseGuild();				// ÇØÁ¦...
		return;
	}

	index = -1;
	index = pGuild->GetUser(pUser->m_strUserID);
	
	if(index < 0)
	{	
		ReleaseGuild();					// ÇØÁ¦...
		error_code = ERR_8;			// 
		goto go_result;		
	}

	if(pGuild->CheckGuildSubMaster())
	{
		if(UpdateGuildSubMaster(pUser->m_strUserID, TRUE) <= 0)
		{
			pGuild->RemoveSubGuildMaster();
			ReleaseGuild();					// ÇØÁ¦...
			return;
		}

		pGuild->SetSubGuildMaster(pUser->m_strUserID);
		ReleaseGuild();					// ÇØÁ¦...

		pUser->m_bRepresentationGuild = TRUE;
		bRes = FALSE;
	}
	else
	{
		strMsg = _T("");
		strMsg.Format(IDS_USER_GUILD_SUBMASTER, pGuild->m_strSubMasterName);//"ÄãÈÎÃüÁË %s ×÷Îª¾üÍÅ¸±ÍÅ³¤."
		SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
		ReleaseGuild();				// ÇØÁ¦...
		return;
	}

go_result:
	if(bRes)
	{
		CBufferEx TempBuf;
		TempBuf.Add(CHAT_RESULT);
		TempBuf.Add((BYTE)0x00);		//½ÇÆÐ
		TempBuf.Add(error_code);
		Send(TempBuf, TempBuf.GetLength());
		return;
	}

	USER *pGUser = NULL;			// ¸Þ¼¼Áö¸¦ ¾Ë¸°´Ù.

	CBufferEx TempBuf;

	strMsg = _T("");
	strMsg.Format( IDS_USER_GUILD_SUBMASTER_ON, pUser->m_strUserID);//"%s ±»ÈÎÃüµ£ÈÎ¾üÍÅ¸±ÍÅ³¤."

	for(int i = 0; i < MAX_USER; i++)
	{
		pGUser = m_pCom->GetUserUid(i);

		if(pGUser == NULL || pGUser->m_state != STATE_GAMESTARTED) continue;		

		if(m_dwGuild == pGUser->m_dwGuild)
		{
			pGUser->SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
		}
	}
}
//////////////////////////////////////////////////////////////////////////
// È¡Ïû¸±ÍÅÈ¨ÏÞ
void USER::BridgeRemoveGuildMasterOfPowerResult()
{
	int index = 0;
	int nLen = 0;

	int uid = 0;
	USER* pUser = NULL;
	CGuild *pGuild = NULL;
	CString sMsg = _T("");

	if(!m_bGuildMaster && !m_bRepresentationGuild) return; 	// ±æµå ±ÇÇÑÀÌ ¾ø´Ù.

	TCHAR strName[CHAR_NAME_LENGTH + 1];
	::ZeroMemory(strName, sizeof(strName));

	BYTE error_code = 0;
	BOOL bRes = TRUE;

	if(CheckInGuildWarring()) return;// ±æÀüÁß¿¡´Â Çã¶ôÇÒ¼ö¾ø´Ù.

	pGuild = GetGuild(m_dwGuild);

	if(!pGuild) 
	{
		ReleaseGuild();				// ÇØÁ¦...
		error_code = ERR_7;			// ÇØ´ç ±æµå°¡ ¾ø´Ù.
		goto go_result;				
	}

	if( !pGuild->IsMasterPower(m_strUserID) )
	{
		ReleaseGuild();				// ÇØÁ¦...
		error_code = ERR_10;		// 
		goto go_result;
	}

	nLen = 0;
	nLen = strlen(pGuild->m_strSubMasterName);
	if(pGuild->m_lSubMaster == 0 || (nLen <= 0 || nLen > CHAR_NAME_LENGTH) )		// ÇöÀç ºÎ±æ¸¶¾ø´Ù°í ¾Ë·ÁÁØ´Ù.
	{
		SendSystemMsg( IDS_USER_GUILD_SUBMASTER_NOT, SYSTEM_ERROR, TO_ME);
		ReleaseGuild();				// ÇØÁ¦...
		return;
	}

	if(UpdateGuildSubMaster(NULL, FALSE) <= 0)
	{
		ReleaseGuild();				// ÇØÁ¦...
		return;
	}
	
	pUser = GetUser(pGuild->m_strSubMasterName);
	if(pUser && pUser->m_state == STATE_GAMESTARTED)
	{
		pUser->m_bRepresentationGuild = FALSE;
	}

	::CopyMemory(strName, pGuild->m_strSubMasterName, nLen);
	pGuild->RemoveSubGuildMaster();

	ReleaseGuild();				// ÇØÁ¦...

	bRes = FALSE;

go_result:
	if(bRes)
	{
		CBufferEx TempBuf;
		TempBuf.Add(CHAT_RESULT);
		TempBuf.Add((BYTE)0x00);		//½ÇÆÐ
		TempBuf.Add(error_code);
		Send(TempBuf, TempBuf.GetLength());
		return;
	}

	USER *pGUser = NULL;

	CBufferEx TempBuf;

	CString strMsg = _T("");
	strMsg.Format( IDS_USER_GUILD_SUBMASTER_OFF, strName);// "%s ±»È¡Ïûµ£ÈÎ¾üÍÅ¸±ÍÅ³¤."

	for(int i = 0; i < MAX_USER; i++)
	{
		pGUser = m_pCom->GetUserUid(i);

		if(pGUser == NULL || pGUser->m_state != STATE_GAMESTARTED) continue;		

		if(m_dwGuild == pGUser->m_dwGuild)
		{
			pGUser->SendSystemMsg((LPTSTR)(LPCTSTR)strMsg, SYSTEM_NORMAL, TO_ME);
		}
	}		
}

void USER::BridgeServerGuildMarkResult(TCHAR *pGuildMark)
{
	CBufferEx TempBuf;

	BOOL bRes = TRUE;
	BYTE error_code = 0;

	CGuild *pGuild = NULL;

	if(!m_bGuildMaster) return;

	pGuild = GetGuild(m_dwGuild);

	if(!pGuild)
	{
		ReleaseGuild();
		return;
	}

	if(strcmp(pGuild->m_strMasterName, m_strUserID) != 0)
	{
		ReleaseGuild();
		return;
	}

	if(pGuild->m_sVersion >= 0) CheckMaxValue((short &)pGuild->m_sVersion, (short)1);
	else pGuild->m_sVersion += 1;

	if(!UpdateGuildMark(pGuildMark, pGuild))
	{
		ReleaseGuild();
		return;
	}

	m_sGuildVersion = pGuild->m_sVersion;	// ±æµå ¹®¾ç ¹öÁ¯

	::CopyMemory(pGuild->m_strMark, pGuildMark, GUILD_MARK_SIZE);

	bRes = FALSE;

	ReleaseGuild();				// ÇØÁ¦...

	TempBuf.Add(GUILD_MARK_ADD_RESULT);

	TempBuf.Add((BYTE)0x01);				// »õ·Î¿î ¹®¾çÀÌ µî·ÏµÇ¾úÀ¸¸é Áö±Ý µî·ÏÇÑ ±æµå À¯Àú ¸ðµÎ¿¡°Ô º¸³»¾ß ÇÏ³ª
	TempBuf.Add(pGuild->m_sVersion);		// ¾Æ´Ô ¹öÁ¯¸¸ °¡¸£ÃÄÁà¼­ Æ²¸®¸é °è¼Ó º¸³»¾ß ÇÏ³ª...
											// Áö±ÝÀº ¹öÁ¯¸¸ º¸³»¼­...
	Send(TempBuf, TempBuf.GetLength());

	SendMyGuildInfo();
											// ÇØ´ç ±æµåÇÏ¿ì½º ¹®¾çÀ» º¯°æÇÑ´Ù.
	for(int i = 0; i < g_arGuildHouse.GetSize(); i++)
	{
		if(pGuild->m_lSid == g_arGuildHouse[i]->iGuild)
		{
			int modify_index = 0;
			char modify_send[2048];

			CNpc *pNpc = NULL;
			pNpc = GetNpc(g_arGuildHouse[i]->iMarkNpc);
			if(!pNpc) return;

			modify_index = 0;
			pNpc->m_sMaxHP = m_sGuildVersion;
			::ZeroMemory(modify_send, sizeof(modify_send));
			pNpc->FillNpcInfo(m_pCom, modify_send, modify_index, INFO_MODIFY);
			pNpc->SendInsight(m_pCom, modify_send, modify_index);

			break;
		}
	}
}

void USER::BridgeServerUserRestartResult()
{
	if(m_bLogOut == TRUE) return;
	if(m_state != STATE_GAMESTARTED) return;	// ÀÌ¿Ü STATE_CONNECTEDµî ÀÏ¶§´Â ÇØ´ç ÃÊ±âÈ­°¡ ¸Þ¸ð¸®¿¡¾øÀ¸¹Ç·Î Á¦¿ÜÇØ¾ßÇÑ´Ù.

	m_bLogOut = TRUE;

	if(m_tGuildWar == GUILD_WARRING && m_dwFieldWar > 0)
	{									// ÇÊµåÀü ÁßÀÌ¸é ¾Ë·ÁÁØ´Ù.
		if(m_bGuildMaster)
		{
			CString strMsg = _T("");
			strMsg.Format( IDS_USER_GUILD_DEFEAT, m_strGuildName);//"%s ¾üÍÅ±»´ò°Ü."
			SendGuildWarFieldEnd((LPTSTR)(LPCTSTR)strMsg);// Ç×º¹
		}
	}

	if(m_bNowBuddy == TRUE)				// ¹öµðÁßÀÌ¸é Åëº¸ÇÑ´Ù.
	{
		for(int i = 0; i < MAX_BUDDY_USER_NUM; i++)
		{
			if(strcmp(m_MyBuddy[i].m_strUserID, m_strUserID) == 0) SendBuddyUserLeave(i);
		}
	}

	if(m_bNowTrading == TRUE)
	{
		BYTE result = 0x00;
		USER *pTradeUser = NULL;
		if(m_iTradeUid != -1)	pTradeUser = GetUser(m_iTradeUid - USER_BAND);

		if(pTradeUser != NULL)	pTradeUser->SendExchangeFail(result, (BYTE)0x05);
	}

	if(m_tGuildHouseWar == GUILD_WARRING) CheckGuildUserListInGuildHouseWar(); // ´Ù¸¥ À¯ÀúµéÀº ¹»ÇÏ³ª Ã¼Å©..

	// ·Î¾â·³ºí Ã³¸®
	ExitRoyalRumble();

	if(m_bLive == USER_DEAD) m_sHP = m_sMaxHP;

	if(!UpdateMemBankDataOnly()) return;
	if(!UpdateMemAccountBankDataOnly()) return;
	if(!UpdateUserData(TRUE)) return;
	ReInitMemoryDB();

	// alisia
//	if(SendRestartLoginResult() == FALSE) return;
	int		index = 0;
	m_state = STATE_LOGOUT;

	index = 0;
	SetByte(m_TempBuf, RESTART_RESULT, index );
	SetByte(m_TempBuf, SUCCESS, index );
	Send(m_TempBuf, index);
	// alisia


	m_nHavePsiNum = 0;
	m_tIsOP = 0;
	MAP *pMap = NULL;

	if( m_ZoneIndex < 0 || m_ZoneIndex >= g_zone.GetSize() ) goto go_result;

	pMap = g_zone[m_ZoneIndex];
	if( m_curx < 0 || m_curx >= pMap->m_sizeMap.cx ) goto go_result;
	if( m_cury < 0 || m_cury >= pMap->m_sizeMap.cy ) goto go_result;

	if( g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser == USER_BAND + m_uid ) // ¸Ê¿¡¼­ »ç¶óÁø´Ù.
		::InterlockedExchange(&g_zone[m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser, 0);

go_result:
	::ZeroMemory(m_strUserID, sizeof(m_strUserID));

	SendMyInfo(TO_INSIGHT, INFO_DELETE);

//	m_state = STATE_GAMERESTART;		// Á¢¼Ó »óÅÂ¸¦ ¹Ù²Û´Ù.
}

void USER::BridgeServerUserZoneLogOutResult(int z, int x, int y)
{
	CPoint pt = ConvertToServerByZone( z, x, y );

	if( pt.x < 0 || pt.y < 0 ) return;

	int save_z = z;
	int save_x = pt.x;
	int save_y = pt.y;

	// µ¥ÀÌÅ¸ ÀúÀå...
	TRACE("Zone Change Data Saved...\n");
	ZoneLogOut(save_z, save_x, save_y);
	//

	m_curz = z;
	m_curx = save_x;
	m_cury = save_y;

	CBufferEx TempBuf;
	BYTE result = FAIL;
	
	TempBuf.Add(ZONE_CHANGE_RESULT);

	result = SUCCESS_ZONE_CHANGE;
	TempBuf.Add(result);
	TempBuf.AddString((char *)LPCTSTR(m_strZoneIP));
	TempBuf.Add(m_nZonePort);

	Send(TempBuf, TempBuf.GetLength());
	return;
}

// ÇöÀç ·Î¾â ·³ºí¿¡ ÀÔÀå °¡´ÉÇÑ ½Ã°£ÀÎÁö °Ë»çÇÑ´Ù.
BOOL USER::CheckRoyalRumbleEnterTime()
{
	return g_RR.CheckEnteringByTime();
}

// ÇöÀç ·Î¾â ·³ºí¿¡ Ãß°¡ ÀÔÀå °¡´ÉÇÑÁö °Ë»çÇÑ´Ù.
BOOL USER::CheckRoyalRumbleEnterMaxUser()
{
	return g_RR.CheckEnteringByMaxUser();
}

void USER::ExitRoyalRumble()
{
	if( m_curz != g_RR.m_iZoneNum ) return;

	if(CheckInvalidMapType() != 12) return;		// ·Î¿­·³ºí¿¡¼­ ´ë·ÃÀå ¼Ó¼º¿¡ ¼­ÀÖÁö ¾ÊÀ¸¸é »ó°ü¾ø´Ù.

	g_RR.Exit( this );
}


int USER::GetGuildMapIndex(int maptype)
{
	if(maptype < 0) return -1;

	for(int i = 0; i < g_arMapTable.GetSize(); i++)
	{
		if(g_arMapTable[i])
		{
			if( maptype == g_arMapTable[i]->m_sMapIndex ) return i;
		}
	}

	return -1;
}

void USER::GuildUserCallReq(TCHAR *pBuf,int iLen)//¾üÍÅÕÙ»½ 
{
	BOOL bOwnerFortress = FALSE;//Æ÷Æ®¸®½º¸¦ ¼ÒÀ¯ÇÏ°í ÀÖ´Â ±æµåÀÎ°¡ È®ÀÎÇÏ´Â º¯¼ö
	BYTE result = FAIL;
	int index = 0;
	char szUserName[255];
	USER* pUser = NULL;
	CPoint pt, ptNew;

	//if(!m_bGuildMaster && !m_bRepresentationGuild) return;//×¢ÏúÍÅ³¤ ¸±ÍÅ²Å¿ÉÒÔÊ¹ÓÃ MarkJay
	
	bOwnerFortress = TRUE;
	if(!bOwnerFortress) return;

//	int nLength = GetVarString(sizeof(szUserName), szUserName, pBuf, index);
    int nLength = GetVarString(iLen, szUserName, pBuf, index);//ÐÞ¸´¾üÍÅ·¢°ü Ð¡ºÚ
	if(nLength == 0 || nLength > CHAR_NAME_LENGTH) return;		// Àß¸øµÈ À¯Àú¾ÆÀÌµð 
	
	pUser = GetUser(szUserName);
	if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED) return; //°ÔÀÓÁßÀÌÁö ¾ÊÀ¸¸é ¸®ÅÏ
	
	if(strcmp(pUser->m_strUserID, m_strUserID) == 0) return;	// ÀÚ±âÀÚ½ÅÀº ¾ÈµÊ
	if(pUser->m_bNowTrading == TRUE) return;//MarkJay ÐÞ¸Ä
	if(pUser->m_bPShopOpen == TRUE) return;//MarkJay ÐÞ¸Ä
	int type = pUser->CheckInvalidMapType();
	if((type == 8 || type == 10 || type == 15 || type == 9 || type == 11 || type == 16) &&( m_tFortressWar == GUILD_WARRING || m_tGuildWar == GUILD_WARRING))
	{
		SendSystemMsg("ÒªÈûÇøÓò,²»ÔÊÐíÊ¹ÓÃ´«ËÍ!",SYSTEM_ERROR,TO_ME);
		return;
	}
	if(pUser->m_curz == 1005 || pUser->m_curz == 61 || pUser->m_curz == 92 || pUser->m_curz == 311 || pUser->m_curz == 409 || pUser->m_curz == 315 || pUser->m_curz == 317 || pUser->m_curz == 316)
	{
		SendSystemMsg("¸ÃµØÍ¼,²»ÔÊÐíÊ¹ÓÃ´«ËÍ!",SYSTEM_ERROR,TO_ME);
		return;
	}
	if(m_dwDN < 100000)
	{
		SendSystemMsg( "ÓÎÏ·±Ò²»¹»10Íò.", SYSTEM_ERROR, TO_ME);//"Ç®²»¹»."
		return; 
	}
	if( m_dwDN <= 100000 ) m_dwDN = 0;
	else m_dwDN = m_dwDN - 100000;
	UpdateUserItemDN();						
	SendMoneyChanged();
	//if(( FindItem(724)) < 500)
	//{
	//	SendSystemMsg("ÔÝ²»¿ª·Å¾üÍÅ´«ËÍ!",SYSTEM_ERROR,TO_ME);
	//	return;
	//}
	//RobItem(724,500);
	
	if(pUser->m_curz != m_curz)									// À¯Àú¿Í ±æ¸¶°¡ °°ÀºÁ¸¿¡ ÀÖÁö ¾ÊÀ» °æ¿ì
	{
		if( !IsZoneInThisServer(pUser->m_curz) ) return;		// ³» ¼­¹ö¾È¿¡ ÀÖ´ÂÁ¸ÀÌ ¾Æ´Ô ¸®ÅÏ

		ptNew = pUser->ConvertToClient(pUser->m_curx,pUser->m_cury);
		BOOL bSuccess = ZoneChangeProcess(pUser->m_curz, ptNew.x, ptNew.y);	//^^ Check ¿ä¸Á
		
		if(bSuccess)
		{
			pUser->SendWeatherInMoveZone();				// ÀÌµ¿ Á¸ÀÇ ³¯¾¾º¯È­¸¦ ¾Ë¸°´Ù.
		}
		return;
	}
	else
	{	
		pt = FindNearAvailablePoint_S(pUser->m_curx, pUser->m_cury );
		ptNew = ConvertToClient( pt.x, pt.y );

		if( ptNew.x == -1 || ptNew.y == -1 ) return;
		if(m_curx < 0 || m_cury < 0) return;//ÐÞÕý×ø±ê±¨´í
		::InterlockedExchange(&g_zone[pUser->m_ZoneIndex]->m_pMap[m_curx][m_cury].m_lUser, 0);
		::InterlockedExchange(&g_zone[pUser->m_ZoneIndex]->m_pMap[pt.x][pt.y].m_lUser, USER_BAND + pUser->m_uid);
		m_curx = pt.x;
		m_cury = pt.y;
		
		result = SUCCESS;
	}
	
	index = 0;
	SetByte(m_TempBuf, MOVE_CHAT_RESULT, index);
	SetByte(m_TempBuf, result, index);

	if(result == FAIL) 
	{
		Send(m_TempBuf, index);
		return;
	}

	SetInt(m_TempBuf, m_uid + USER_BAND, index);
	SetShort(m_TempBuf, ptNew.x, index);
	SetShort(m_TempBuf, ptNew.y, index);
	Send(m_TempBuf, index);	// À¯Àú¿¡°Ô´Â »õ·Î¿î ÁÂÇ¥°ªÀ»...
	
	SendInsight(m_TempBuf, index);
	
	SightRecalc();
}

int USER::GetIntAddr(CString strPeerIp)//yskang 0.04 stringÇü ¾ÆÀÌÇÇ¸¦ intÇü ¾ÆÀÌÇÇ·Î...
{
	int nRetAddr = 0;
	CString strTemp;
	BYTE *pByte = (BYTE *)&nRetAddr;
	int i = 0;
	while (i < strPeerIp.GetLength ())
	{
		if (strPeerIp[i] == '.')
		{
			strTemp = strPeerIp.Left (i);
			strPeerIp = strPeerIp.Right (strPeerIp.GetLength () - (i + 1));
			*pByte = (BYTE)atoi (strTemp);
			pByte++;
			i = 0;
			if (strPeerIp.Find ('.') == -1)
			{
				*pByte = (BYTE)atoi (strPeerIp);
				break;
			}
			continue;
		}
		i++;
	}	
	return nRetAddr;
}


TCHAR *USER::GetStrAddr(int nPeerIp, TCHAR *strPeerIp)
{
	//TCHAR strPeerIp[1024];
	ZeroMemory(strPeerIp,sizeof(strPeerIp));
	char buffer[20];
	for(int i=0; i<4; i++)
	{
		BYTE a = nPeerIp&0x000000ff;
		_itoa(a,buffer,10);
		strcat(strPeerIp,(const char*)buffer);
		if(i!=3)
			strcat(strPeerIp,".");
		nPeerIp = nPeerIp>>8;
	}
	return (TCHAR *)strPeerIp;
}
///////////////////////////////////////////////////////////////
// Ð´Èë½»Ò×ÃÜÂë
bool USER::SetJiaoYiMiMa(char *strAccount)
{
	SQLHSTMT		hstmt;
	SQLRETURN		retcode;
	TCHAR			szSQL[1024];

	BOOL bQuerySuccess = TRUE;

	memset(szSQL, 0x00, 1024);
	_sntprintf(szSQL, sizeof(szSQL), TEXT("SELECT strUserID, strSocNo FROM NGSCUSER	WHERE struserid='%s'"), strAccount);
	hstmt = NULL;
	int db_index = -1;
	CDatabase* pDB = g_DBSession[m_iModSid].GetDB( db_index );
	if( !pDB ) return FALSE;

	retcode = SQLAllocHandle( (SQLSMALLINT)SQL_HANDLE_STMT, pDB->m_hdbc, &hstmt );
	if(retcode != SQL_SUCCESS) return FALSE;
	int i;
	SQLINTEGER ivd;
	char szChar01[40]="",szChar02[40]="";
	retcode = SQLExecDirect (hstmt, (unsigned char *)szSQL, SQL_NTS);
	if(retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
	{
		while(TRUE)
		{
			retcode = SQLFetch(hstmt);
			if(retcode !=SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) break;
			i = 1;
			JiaoYiMiMa[20] = 0;
			SQLGetData( hstmt, i++, SQL_C_CHAR, szChar01, sizeof(szChar01), &ivd );
			SQLGetData( hstmt, i++, SQL_C_CHAR, szChar02, sizeof(szChar02), &ivd );
			if(strlen(szChar02) > 0)
			{
				strcpy(JiaoYiMiMa,szChar02);
			}
		}
	}
	else if(retcode == SQL_ERROR)
	{
		bQuerySuccess = FALSE;
		DisplayErrorMsg(hstmt);
	}
	else if(retcode == SQL_NO_DATA)
	{
		bQuerySuccess = FALSE;
	}
	if(hstmt != NULL) SQLFreeHandle( (SQLSMALLINT)SQL_HANDLE_STMT, hstmt);
	if(!bQuerySuccess) 
	{
		g_DBSession[m_iModSid].ReleaseDB(db_index);
		return FALSE;
	}
	g_DBSession[m_iModSid].ReleaseDB(db_index);
	return TRUE;
}
///////////////////////////////////////////////////////////////
// Ê°È¡×°±¸ÆÀ·Ö MarkJay
void USER::GetUserItem(ItemList* pItem, int type)
{
	if(!pItem) return;
	if(pItem->sSid < 0 || pItem->sSid >= g_arItemTable.GetSize()) return;
	pItem->sScore = 0;
	pItem->sWeapScore = 0;
	int CheckItem_Str = 0; //¼ì²éÁ¦Á¿
	int CheckItem_Dex = 0; //¼ì²éÃô½Ý
	int CheckItem_Vol = 0; //¼ì²éÖÇ»Û
	for(int i = 0; i < MAGIC_NUM; i++)
	{
		if(pItem->tMagic[i] <= 0 || pItem->tMagic[i] >= g_arMagicItemTable.GetSize())
		{
			continue;
		}
		else
		{
			pItem->sScore += g_arMagicItemTable[pItem->tMagic[i]]->m_sScore;
			pItem->sWeapScore += g_arMagicItemTable[pItem->tMagic[i]]->m_sWeapScore;
		}
		if( pItem->tMagic[i] == 43 || pItem->tMagic[i] == 86 || pItem->tMagic[i] == 107) CheckItem_Str = 1;//¼ì²éÁ¦
		if( pItem->tMagic[i] == 45 || pItem->tMagic[i] == 88 || pItem->tMagic[i] == 109) CheckItem_Dex = 1;//¼ì²éÃô
		if( pItem->tMagic[i] == 46 || pItem->tMagic[i] == 89 || pItem->tMagic[i] == 110) CheckItem_Vol = 1;//¼ì²éÖÇ»Û
	}
	if( CheckItem_Str == 1 &&  CheckItem_Dex == 1)  pItem->sScore = 0; //Á¦Á¿ºÍÃô½ÝÍ¬Ê±³öÏÖ ·ÖÊý0
	if( CheckItem_Dex == 1 &&  CheckItem_Vol == 1)  pItem->sScore = 0; //Ãô½ÝºÍÖÇ»ÛÍ¬Ê±³öÏÖ ·ÖÊý0
	if( CheckItem_Str == 1 &&  CheckItem_Vol == 1)  pItem->sScore = 0; //Á¦Á¿ºÍÖÇ»ÛÍ¬Ê±³öÏÖ ·ÖÊý0

	if(type == ITEMLOG_PICKUP)
	{
		CString sysMsg;
		if(pItem->sSid == 987 || pItem->sSid == 1093 || pItem->sSid == 1144 || pItem->sSid == 1468 || pItem->sSid == 1188
		|| pItem->sSid == 1145 || pItem->sSid == 1185 || pItem->sSid == 1439 || pItem->sSid == 964 || pItem->sSid == 1456
		|| pItem->sSid == 847 || pItem->sSid == 848 || pItem->sSid == 755 || pItem->sSid == 702 || pItem->sSid == 756 || pItem->sSid == 1006)
		{
			sysMsg.Format("%s Ê°È¡ %s",this->m_strUserID,g_arItemTable[pItem->sSid]->m_strName );
		}
		else if(pItem->sSid == 907)
		{
			int tEBodySid = 0;
			tEBodySid = pItem->tMagic[0];
			if(tEBodySid == 14 || tEBodySid == 16 || tEBodySid == 24 || tEBodySid == 27 || tEBodySid == 42 || tEBodySid == 44 || tEBodySid == 52 || tEBodySid == 55)
			{
				TCHAR strTxt_My[59][50];
				_tcscpy(strTxt_My[14],_T("ÊÍ·ÅÊ±¼ä2%¼õÉÙ"));
				_tcscpy(strTxt_My[16],_T("Á¦Á¿µÄ10%±ä³ÉÉËº¦Á¦"));
				_tcscpy(strTxt_My[24],_T("Ãô½Ý10%±äÎªÔ¶¾àÀëÉËº¦Á¦"));
				_tcscpy(strTxt_My[27],_T("ÖÇ»Û10%±äÎªÄ§·¨ÉËº¦Á¦"));
				_tcscpy(strTxt_My[42],_T("ÊÍ·ÅÊ±¼ä1%¼õÉÙ"));
				_tcscpy(strTxt_My[44],_T("Á¦Á¿µÄ5%±ä³ÉÉËº¦Á¦"));
				_tcscpy(strTxt_My[52],_T("Ãô½Ý5%±äÎªÔ¶¾àÀëÉËº¦Á¦"));
				_tcscpy(strTxt_My[55],_T("ÖÇ»Û5%±äÎªÄ§·¨ÉËº¦Á¦"));
				sysMsg.Format("%s Ê°È¡ %s ï¯Ê¯",this->m_strUserID,strTxt_My[tEBodySid]);
			}
		}
		else if(pItem->sSid == 956 || pItem->sSid == 963)
		{
			int ShuXing = 0;
			ShuXing = pItem->tMagic[0];
			if(ShuXing == 4 || ShuXing == 28)
			{
				TCHAR strTxt_My[166][50];
				_tcscpy(strTxt_My[4],_T("ÎäÆ÷µÈ¼¶1Ôö¼Ó"));
				_tcscpy(strTxt_My[28],_T("ËùÓÐ¼¼ÄÜ1Ôö¼Ó"));
				sysMsg.Format("%s Ê°È¡ %s ÊØ»¤",this->m_strUserID,strTxt_My[ShuXing]);
			}
		}
		else if(pItem->sWeapScore >= 150 && g_arItemTable[pItem->sSid]->m_byWear == 1)
		{
			CString GoodsMagic;
			sysMsg.Format("%s Ê°È¡ %s,ÆÀ·Ö%d",this->m_strUserID,g_arItemTable[pItem->sSid]->m_strName,pItem->sWeapScore);
			char szFileName[MAX_PATH + 1];
			GetCurrentDirectory(MAX_PATH,szFileName);//»ñÈ¡³ÌÐòµÄµ±Ç°Ä¿Â¼
			strcat(szFileName,"\\pf.txt");
			FILE *fp;
			char MyTemp[191][50];
			if(fp = fopen(szFileName,"r"))
			{
				for(int j2 = 0;j2 < 191;j2++)
				{
					if(feof(fp))break;
					fgets(MyTemp[j2],50,fp);
					MyTemp[j2][strlen(MyTemp[j2])-1] = 0;   
				}
				fclose(fp);
				for(int j1 = 0;j1 < 5;j1++)
				{
					if(!pItem->tMagic[j1]) continue;	
					GoodsMagic.Format(",%s",MyTemp[pItem->tMagic[j1]]);
					sysMsg += GoodsMagic;
				}
			}
		}
		else if(pItem->sScore >= 150 && g_arItemTable[pItem->sSid]->m_byWear > 1 && g_arItemTable[pItem->sSid]->m_byWear < 6)
		{
			CString GoodsMagic;
			sysMsg.Format("%s Ê°È¡ %s,ÆÀ·Ö%d",this->m_strUserID,g_arItemTable[pItem->sSid]->m_strName,pItem->sScore);
			char szFileName[MAX_PATH + 1];
			GetCurrentDirectory(MAX_PATH,szFileName);//»ñÈ¡³ÌÐòµÄµ±Ç°Ä¿Â¼
			strcat(szFileName,"\\pf.txt");
			FILE *fp;
			char MyTemp[191][50];
			if(fp = fopen(szFileName,"r"))
			{
				for(int j2 = 0;j2 < 191;j2++)
				{
					if(feof(fp))break;
					fgets(MyTemp[j2],50,fp);
					MyTemp[j2][strlen(MyTemp[j2])-1] = 0;   
				}
				fclose(fp);
				for(int j1 = 0;j1 < 5;j1++)
				{
					if(!pItem->tMagic[j1]) continue;	
					GoodsMagic.Format(",%s",MyTemp[pItem->tMagic[j1]]);
					sysMsg += GoodsMagic;
				}
			}
		}
		if(sysMsg.GetLength() > 0 && pItem->sSid >= 0 && pItem->sSid < g_arItemTable.GetSize())
		{
			SYSTEMTIME st;
			CString strDate;
			GetLocalTime(&st);
			strDate.Format("%d-%d-%d %d:%d",st.wYear,st.wMonth,st.wDay ,st.wHour,st.wMinute);
			CString str = _T("");
			str.Format("[%s]ÔÚ[%s] %s\r\n",m_strUserID,strDate,sysMsg);
			EnterCriticalSection( &m_CS_FileWrite );
			g_fpMarkShiQuLog.Write( str, str.GetLength() ); //Ð´ÈëÎïÆ·ÈÕÖ¾
			LeaveCriticalSection( &m_CS_FileWrite);
			m_pCom->Announce((LPTSTR)(LPCTSTR)sysMsg, SYSTEM_ANNOUNCE);
		}
	}	
}