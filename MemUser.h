// MemUser.h: interface for the CMemUser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEMUSER_H__A2D30C97_9428_4023_80A4_664B2B403E04__INCLUDED_)
#define AFX_MEMUSER_H__A2D30C97_9428_4023_80A4_664B2B403E04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MemDefine.h"


class CMemGuildHouseRank
{
public:
	CMemGuildHouseRank();
	~CMemGuildHouseRank();

	BYTE	tSid;	
	int		lGuild;
	BYTE	iGuildNum;
	short	iLastValue;
	short	iGuildLevel;
	DWORD	dwLastTime;
};

class CMemFortress
{
public:
	CMemFortress();
	~CMemFortress();

	short	m_sFortressID;
	int		m_iGuildSid;

	short	m_sTaxRate;
	DWORD	m_dwStoreDN;
	BYTE	m_tWarType;

	TCHAR	m_AttackList[_GUILDLIST_DB];
	TCHAR	m_RepairList[_GUILDREPAIRLIST_DB];
};

class CMemStore
{
public:
	CMemStore();
	~CMemStore();

	short	m_sStoreID;						// »óÁ¡ ÀÎµ¦½º
	int		m_iGuildSid;					// »óÁ¡À» ¼ÒÀ¯ÇÑ ±æµå ÀÎµ¦½º

	short	m_sTaxRate;						// ¼¼±Ý
	DWORD	m_dwStoreDN;					// ´©Àû °ª
	BYTE	m_tWarType;						// ±æµåÀü ½Ã±â °áÁ¤ ÇÃ·¡±×

	TCHAR	m_AttackList[_GUILDLIST_DB];	// ±æµåÀüÀ» ½ÅÃ»ÇÑ ±æµå ¸®½ºÆ®
};

class CMemBank
{
public:
	CMemBank();
	~CMemBank();

	int		m_uid;

	TCHAR	m_strUserID[CHAR_NAME_LENGTH + 1];	// ¾ÆÀÌµð(Ä³¸¯ÅÍ ÀÌ¸§)
	TCHAR	m_UserBankItem[_BANK_DB];		// ÀºÇà³» ¾ÆÀÌÅÛ
	DWORD	m_dwBankDN;						// User Bank DN
};

class CMemAccountBank
{
public:
	CMemAccountBank();
	~CMemAccountBank();

	int		m_uid;

	TCHAR	m_strAccount[CHAR_NAME_LENGTH + 1];	// ¾ÆÀÌµð(Ä³¸¯ÅÍ ÀÌ¸§)
	TCHAR	m_AccountBankItem[_ACCOUNT_BANK_DB];// ÀºÇà³» ¾ÆÀÌÅÛ
	DWORD	m_dwBankDN;							// User Bank DN
};

typedef struct _ITEMLOG
{
	CTime	t;
	short	m_sLogType;
	char	m_strUserId[CHAR_NAME_LENGTH + 1];		// ÇàÀ§ ÁÖÃ¼
	char	m_strEtc[CHAR_NAME_LENGTH + 1];			// °ü·ÃÀÚ

	short	m_sLevel;
	short	m_sSid;
	short	m_sDuration;
	short	m_sBullNum;
	short	m_sCount;
	BYTE	m_tMagic[11];
	BYTE	m_tIQ;

	UINT64	m_iItemSerial;

	DWORD	m_dwResultDN;
	DWORD	m_dwInOutDN;
	DWORD	m_dwRelateDN;
} ITEMLOG;

class CMemUser  
{
public:
	CMemUser();
	~CMemUser();

	CMemBank	m_UB;
	CMemAccountBank m_AB;

	int			m_uid;					// Server User id

	TCHAR		m_strAccount[CHAR_NAME_LENGTH + 1];
	TCHAR		m_strUserID[CHAR_NAME_LENGTH + 1];	// ¾ÆÀÌµð(Ä³¸¯ÅÍ ÀÌ¸§)
	
	//-- yskang 0.1 ±æµå¿¡°Ô È£ÄªºÎ¿©....
	TCHAR m_strLoveName[LOVE_NAME_LENGTH+1];

	DWORD		m_dwSaveTime;			// ³â 4, ¿ù 4, ÀÏ 5, ½Ã 5, ºÐ 6, ÃÊ 6 BYTE	

	short		m_sSTR;					// Èû
	short		m_sCON;					// °Ç°­
	short		m_sDEX;					// ¹ÎÃ¸¼º
	short		m_sVOL;					// ÀÇÁö
	short		m_sWIS;					// ÁöÇý
	
	DWORD		m_iSkin;				// ÇÇºÎ»ö
	DWORD		m_iHair;				// ¸Ó¸®»ö
	short		m_sGender;				// ¼ºº°
	TCHAR		m_strFace[10];			// ¾ó±¼¸ð¾ç

	int			m_curx;					// ÇöÀç X ÁÂÇ¥
	int			m_cury;					// ÇöÀç Y ÁÂÇ¥
	int			m_curz;					// ÇöÀç Á¸

	DWORD		m_dwBuddy;				// ¹öµð¹øÈ£
	long		m_dwGuild;				// ±æµå¹øÈ£

	DWORD	m_dwExp;				// ·¹º§°æÇèÄ¡

	short		m_sPA;					// PA Point
	short		m_sSkillPoint;			// Skill Point
	
	DWORD		m_dwXP;					// X Point

	short		m_sMaxHP;				// ÃÖ´ë HP
	short		m_sHP;					// ÇöÀç HP
	short		m_sMaxPP;				// ÃÖ´ë PP
	short		m_sPP;					// ÇöÀç PP
	short		m_sMaxSP;				// ÃÖ´ë SP
	short		m_sSP;					// ÇöÀç SP

	DWORD		m_dwDN;					// ¼ÒÁö±Ý

	short		m_sCityRank;			// ½Ã¹Î µî±Þ
	short		m_sKillCount;			// PKÇÑ È½¼ö

	short		m_sLevel;				// °è¿­·¹º§
	BYTE		m_byClass;				// Å¬·¡½º

	TCHAR		m_strSkill[_SKILL_DB];	// User Skill DB
	TCHAR		m_strItem[_ITEM_DB];	// User Item DB
	TCHAR		m_strPsi[_PSI_DB];		// User Psionic DB
	TCHAR		m_strHaveEvent[_EVENT_DB];// User Event DB
	TCHAR		m_strTel[_TEL_DB];		// User Tel DB

	short		m_sChangeClass[_CHANGE_DB];		// ÀüÁ÷½Ã Å¬·¡½º·¹º§ ÀúÀå

	BYTE		m_tAbnormalKind;				// »óÅÂÀÌ»ó Á¾·ù
	DWORD		m_dwAbnormalTime;				// »óÅÂÀÌ»ó °É¸°ÈÄ ³²Àº ½Ã°£
	
	TCHAR		m_strQuickItem[_QUICKITEM_DB];	// Quick Item Index
	
	BYTE		m_tIsOP;					// ¿î¿µÀÚÀÎÁö ÆÇ´Ü
											// 0: Normal User
											// 1: Game Operator
											// 2: Server Operator

	BYTE		m_bLive;				// Á×¾ú´Ï? »ì¾Ò´Ï?
	BYTE		m_bLock;                // ·âºÅ 

	BYTE		m_sTempSTR;				// Èû
	BYTE		m_sTempCON;				// °Ç°­
	BYTE		m_sTempDEX;				// ¹ÎÃ¸¼º
	BYTE		m_sTempVOL;				// ÀÇÁö
	BYTE		m_sTempWIS;				// ÁöÇý

	int			m_iCityValue;			// ÇöÀç ´©Àû ¼ºÇâ°ª

	DWORD		m_dwSaintTime;

	DWORD		m_dwHiExpTime;			//Êý¾Ý¿â
	DWORD		m_dwMagicFindTime;		// ¸ÅÁ÷Âù½º 5¹è ¹°¾àÀ» ¸ÔÀºÈÄ ³²Àº ½Ã°£
	DWORD		m_dwNoChatTime;			// Ã¤±ÝÀ» ¸ÔÀºÈÄ ³²Àº ½Ã°£
	DWORD		m_dwBFindTime;//±äÉí
	DWORD		m_dwVIPTime;//»áÔ±
	DWORD		m_dwCloseTime;//¼àÓü
	DWORD		m_dwShopPingDN;//Ôª±¦
	DWORD		m_dwGuarDianTianShi;//ÊØ»¤ÌìÊ¹
	DWORD		m_dwJiFen; //»ý·Ö
	DWORD		m_dwChouJiang; //³é½±
	DWORD		m_dwRenWu; //ÈÎÎñ
	DWORD		m_dwCKMiMa; //²Ö¿âÃÜÂë
	DWORD		m_dwHuanJing; //»Ã¾§
	DWORD		m_dwHuanLing; //»ÃÁé
	DWORD		m_dwZaiXianTime; //ÔÚÏßÊ±¼ä
	DWORD		m_dwPKAddkill;
	DWORD		m_dwFUExpTime;//ÐÒ¸£Ê±¹â
	DWORD		m_dwJiaGong;//Ð¡ºÚÐÞÁ¶¹¥»÷
	DWORD		m_dwJiaFangYu;//Ð¡ºÚÐÞÁ¶·ÀÓù
	DWORD		m_dwJiaMoFang;//Ð¡ºÚÐÞÁ¶Ä§·À
	DWORD		m_dwJiaGongJi;//¼Ó¹¥»÷ Ð¡ºÚÐÞÁ¶
	DWORD		m_dwJiaXueLiang;//¼ÓÑªÁ¿
	DWORD		m_dwJiaMoney;//¼Ó½ðÇ®


	BYTE		m_tPsiOneKind;			
	BYTE		m_tPsiTwoKind;
	BYTE		m_tPsiThreeKind;
	
	DWORD		m_dwPsiOneTime;
	DWORD		m_dwPsiTwoTime;
	DWORD		m_dwPsiThreeTime;

	char		m_arItemLogData[100][sizeof(ITEMLOG)];	// ¾ÆÀÌÅÛ ·Î±ë
	
	int			m_iMyServer;			// Áö±Ý Á¢¼ÓÇÑ ¼­¹ö ÀÎµ¦½º
};

#endif // !defined(AFX_MEMUSER_H__A2D30C97_9428_4023_80A4_664B2B403E04__INCLUDED_)
