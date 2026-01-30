// Npc.h: interface for the CNpc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPC_H__600048EF_818F_40E9_AC07_0681F5D27D32__INCLUDED_)
#define AFX_NPC_H__600048EF_818F_40E9_AC07_0681F5D27D32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "COM.h"
#include "Map.h"

#include "Packet.h"
#include "PathFind.h"
#include "Store.h"
#include "GuildFortress.h"
#include "EventItemNew.h"

#define CHANGE_PATH_STEP	2
#define CHANGE_PATH_RANGE	2
#define RANDOM_MOVE_LENGTH	10
#define	NPC_ITEM_BAND		2000

#define NPCTYPE_MONSTER		0			// ¹ÖÎï
#define NPCTYPE_NPC			1			// ½Å±¾NPC
#define NPCTYPE_DOOR		2			// ´«ËÍÃÅ
#define NPCTYPE_GUARD		3			// ÃÅÎÀ
#define MPCTYPE_GUARD_MOVE	4			// µ¹¾Æ´Ù´Ï´Â °æºñº´
#define NPCTYPE_GUILD_NPC	5			// ÒªÈûNPC 
#define NPCTYPE_GUILD_GUARD	6			// ÀÏ¹İ ÇÊµå¿¡¼­ »óÁ¡ÁÖº¯À» º¸È£ÇÏ´Â °æºñº´
#define NPCTYPE_GUILD_MARK	7			// °¢ ±æµåÇÏ¿ì½º¿¡ ÀÖ´Â ±æµå¸¶Å© NPC
#define NPCTYPE_GUILD_DOOR	8			// °ø¼ºÀü¿¡¼­ ¾²ÀÌ´Â ¹®

#define GUILD_REPAIR_NPC_BAND	20000	// ¼Õ»óµÇ¾î ¼ö¸®°¡ ÇÊ¿äÇÑ NPC
#define GUILD_FORTRESS_NPC_BAND	25000	// ¼ºÀ» Á¡·ÉÇÏ´Âµ¥ ÇÊ¿äÇÑ NPC

#define MAX_MAP_SIZE		10000		//@@@@@@@@@@@@@@@@@@@@@@@@@@@22
#define MAX_PATH_SIZE		100

#define NPC_ITEM_NUM		5

#define NPC_ATTACK_SHOUT	0
#define NPC_SUBTYPE_LONG_MON 1

#define NPC_TRACING_STEP	100

#define NPC_HAVE_USER_LIST	10

#define NPC_QUEST_MOP		 10000		// Äù½ºÆ®¿ë ¸÷( 1 ~ 10000 )À¸·Î »ç¿ë 
#define NPC_EVENT_MOP		 31000		// ÀÌº¥Æ® ¸÷ ¹øÈ£
#define NPC_EVENT_GREATE_MOP 30000		// Æ¯ ÀÌº¥Æ® ¸ó½ºÅÍ ¹øÈ£
#define NPC_MAGIC_ITEM		 1500		// µôÀ¶É«×°±¸¼¸ÂÊ
#define NPC_RARE_ITEM		 2500		// µô½ğÉ«×°±¸¼¸ÂÊ
#define NPC_EVENT_CHANCE	40			// ÀÌº¥Æ® ¸÷ÀÏ°æ¿ì ¸ÅÁ÷È®·ü°ú ·¹¾î È®·ü¿ï ¿Ã·ÁÁØ´Ù. X 40 


// Ãß¼® ÀÌº¥Æ®¸¦ Å©¸®½º¸¶½º ÀÌº¥Æ®·Î º¯ÇüÇÏ¿© ÁøÇàÇÏ±â À§ÇØ ÀçÁ¤ÀÇ ÇÔ
#define NPC_EVENT_MOON		620			// »êÅ¸ÀÎÇüµå·ÓÈ®À² 5%
#define NPC_EVENT_SONGPEON	1120		// ¸·´ë»çÅÁ µå·ÎÈ®À² 5%
#define NPC_EVENT_BOX		1420		// Å©¸®½º¸¶½º ¾ç¸» µå·ÓÈ®À² 3%

#define EVENTITEM_SID_MOON			873	// »êÅ¸ÀÎÇü	Sid
#define EVENTITEM_SID_SONGPEON_01	872	// ¸·´ë»çÅÁ	Sid
#define EVENTITEM_SID_SONGPEON_11	864	//
#define EVENTITEM_SID_SONGPEON_31	865	//
#define EVENTITEM_SID_SONGPEON_51	866	//
#define EVENTITEM_SID_SONGPEON_71	867	//
#define EVENTITEM_SID_BOX			871 // Å©¸®½º¸¶½º ¾ç¸» Sid

#define NPC_GUILDHOUSE_BAND	10000		// ±æµåÇÏ¿ì½º¿¡ °ü·ÃµÈ NPC
//#define NPC_STORE_BAND			
#define FORTRESS_BAND	1000			// ¿ä»õ¿¡ ¼ÓÇÏ´Â NPC

#define NPC_EVENT_B_ITEM	57			// ¹ü¿ëÈ¸º¹Á¦B ÀÌº¥Æ®¸¦ À§ÇÑ ÀÓ½Ã Á¤ÀÇ
#define NPC_EVENT_ITEM		634			// ÀÌº¥Æ®¸¦ À§ÇÑ ÀÓ½Ã Á¤ÀÇ
#define NPC_EVENT_INIT_STAT	756			// ±Ø¾à ¾ÆÀÌÅÛ
#define NPC_EVENT_FLOWER	773			// ¸÷ÀÌ ¶³¾î¶ß¸®´Â ²É¹Ù±¸´Ï ±³È¯±Ç
#define NPC_EVENT_LOTTO		796			// Åõ½ºÄ«´Ï ÀÌº¥Æ®

// °ø¼ºÀü¿¡ ¾²ÀÓ
										// ±æµåÀü½Ã ¼ö¸®ÇÒ ÇÊ¿ä¼º ÀÌ ÀÖÀ¸¸é
#define NPC_NON_REPAIR_STATE	0		// 0 : ÃÖ°í »óÅÂ		
#define NPC_NEED_REPAIR_STATE	1		// 1 : ¼Õ»óµÇ¾î ¼ö¸® ÇÒ ÇÊ¿ä°¡ ÀÖ´Â »óÅÂ
#define NPC_DEAD_REPAIR_STATE	2		// 2 : ¿ÏÀüÈ÷ ¸Á°¡Áø »óÅÂ

#define NPC_NCIRCLE_DEF_STATE	1
#define NPC_NCIRCLE_ATT_STATE	2

struct  DynamicMapList
{
	POINT map[NPC_MAP_SIZE];
};

struct  NpcSkillList
{
	short	sSid;
	BYTE	tLevel;
	BYTE	tOnOff;
};

struct	ExpUserList
{
	TCHAR	strUserID[CHAR_NAME_LENGTH + 1];			// ¾ÆÀÌµğ(Ä³¸¯ÅÍ ÀÌ¸§)
	int		iUid;		
	int		nDamage;								// Å¸°İÄ¡ ÇÕ
	BOOL	bIs;
};

struct	TargetUser
{
	int		iUid;
	int		sHP;
	int		sMaxHP;
};

struct ItemUserRightlist
{
	short uid;
	int nDamage;
};

//typedef CTypedPtrArray <CPtrArray, ExpUserList*>		arUserList;
typedef CArray <CPoint, CPoint> RandMoveArray;		// 8¹æÇâ RandomMoveÇÏ±â À§ÇÑ Array

class CNpc  
{
public:
	CNpc();
	virtual ~CNpc();

	BOOL SetLive(COM* pCom);
	BOOL FindEnemy(COM* pCom);
	BOOL GetTargetPath(COM* pCom);
	BOOL IsCloseTarget(COM* pCom, int nRange = 1);
	BOOL IsCloseTarget(USER *pUser, int nRange = 1);
	BOOL IsMovingEnd();
	BOOL IsChangePath(COM* pCom, int nStep = CHANGE_PATH_STEP);
	USER* GetUser(COM* pCom, int uid);
	BOOL ResetPath(COM* pCom);
	BOOL GetTargetPos(COM* pCom, CPoint& pt);
	BOOL StepMove(COM* pCom, int nStep);
	BOOL GetLastPoint(int sx, int sy, int& ex, int& ey);

protected:
	void ClearPathFindData(void);
	void InitSkill();
	
public:	
	void SendNpcInfoBySummon(COM* pCom);
	static BOOL CheckClassGaiZaoItem(int artable, int armagic);
	BOOL CheckUserForNpc_Live(int x, int y);
	DWORD GetItemThrowTime();
	void UserListSort();
	CPoint FindNearRandomPointForItem(int x, int y);
	BOOL UpdateEventItemNewRemain(CEventItemNew* pEventItem);
	void GiveItemToMap(COM* pCom, ItemList* pItem);
	void GiveEventItemNewToUser(USER* pUser);
	void SetISerialToItem(ItemList *pItem, int iEventItemSid);
	int AreaAttack(COM* pCom);
	void GetWideRangeAttack(COM* pCom, int x, int y, int damage, int except_uid);
	int PsiAttack(COM* pCom);
	void TestCode(COM *pCom, USER *pUser);
	void SendFortressInsight(COM *pCom, TCHAR *pBuf, int nLength);
	void SetMapAfterGuildWar();
	void SetMapTypeBeforeGuildWar(COM *pCom);
	void SendFortressNCircleColor(COM *pCom);
	//void SetDoorDamagedInFortressWar(int nDamage, TCHAR *id, int uuid, COM *pCom);
	void SetDoorDamagedInFortressWar(int nDamage, USER *pUser);
	//void SetDamagedInFortressWar(int nDamage, TCHAR *id, int uuid, COM *pCom);
	void SetDamagedInFortressWar(int nDamage, USER *pUser);
	void SetFortressState();
	BOOL UpdateEventItem(int sid);
	int GetEventItemNum(COM *pCom);
	void GiveEventItemToUser(USER *pUser);
	void Send(USER* pUser, TCHAR* pBuf, int nLength);
	int GetCityNumForVirtualRoom(int zone);
	//void SetDamagedInGuildWar(int nDamage, TCHAR *id, int uuid, COM *pCom);
	void SetDamagedInGuildWar(int nDamage, USER *pUser);
	void SetGuildType(COM *pCom);
	void EventNpcInit(int x, int y);
	int IsMagicItem(COM* pCom, ItemList *pItem, int iTable);
	int IsTransformedItem(int sid);
	void ToTargetMove(COM* pCom, USER* pUser);
	void NpcTrace(TCHAR* pMsg);
	CPoint ConvertToServer(int x, int y);
	void Init();
	BOOL IsStepEnd();
	BOOL PathFind(CPoint start, CPoint end);
	void InitTarget(void);
	void SendToRange(COM* pCom, char* temp_send, int index, int min_x, int min_y, int max_x, int max_y);
	CPoint ConvertToClient(int x, int y);
	void FillNpcInfo(COM* pCom,char* temp_send, int& index, BYTE flag);
	void SendUserInfoBySightChange(int dir_x, int dir_y, int prex, int prey, COM* pCom);
	int GetFinalDamage(USER* pUser, int type = 1);
	void DeleteNPC();
	void SetFireDamage();
	void SetConFusion();//Ñ£ÔÎ
	BOOL CheckClassItem(int artable, int armagic);
	void ChangeSpeed(COM *pCom, int delayTime);
	CNpc* GetNpc(int nid);
	void InitUserList();
	BOOL GetBackPoint(int &x, int &y);
	void GetBackDirection(int sx, int sy, int ex, int ey);
	void NpcBack(COM *pCom);
	BOOL IsDamagedUserList(USER *pUser);
	void FindFriend();
	void NpcStrategy(BYTE type);
	void NpcTypeParser();
	void NpcFighting(COM *pCom);
	void NpcTracing(COM *pCom);
	void NpcAttacking(COM *pCom);
	void NpcMoving(COM *pCom);
	void NpcSendTeleport(COM *pCom);//ËÀÉñË²ÒÆÌØĞ§
	void NpcStanding(COM *pCom);
	void NpcLive(COM *pCom);
	void ChangeTarget(USER *pUser, COM* pCom);
	BOOL IsSurround(int targetx, int targety);
	BOOL CheckNpcRegenCount();
	void IsUserInSight(COM *pCom);
	void SendExpToUserList(COM *pCom);
	BOOL SetDamage(int nDamage, int uid, COM *pCom);
	void SetColdDamage(void);
	CPoint FindNearRandomPoint(int xpos, int ypos);
	BOOL IsMovable(int x, int y);
	void GiveNpcHaveItem(COM *pCom);
	int GetCriticalInitDamage(BOOL* bSuccessSkill);
	int GetNormalInitDamage();
	void SendAttackMiss(COM* pCom, int tuid);
	void IsSkillSuccess(BOOL *bSuccess);
	BYTE GetWeaponClass();
	//yskang 0.3 void SendAttackSuccess(COM *pCom, int tuid, CByteArray &arAction1, CByteArray &arAction2, short sHP, short sMaxHP);
	void SendAttackSuccess(COM *pCom, int tuid,BOOL bIsCritical, short sHP, short sMaxHP);//yskang 0.3
	void GiveItemToMap(COM *pCom, int iItemNum, BOOL bItem, int iEventNum = 0);
	BOOL GetDistance(int xpos, int ypos, int dist);
	void SendInsight(COM* pCom, TCHAR *pBuf, int nLength);
	void SendExactScreen(COM* pCom, TCHAR* pBuf, int nLength);

	void SightRecalc(COM* pCom);

	BOOL IsInRange();
	BOOL RandomMove(COM* pCom);
	int SendDead(COM* pCom, int type = 1);
	void Dead(void);
	BOOL SetDamage(int nDamage);
	int GetDefense();
	int GetAttack();
	int Attack(COM* pCom);

	// Inline Function
	int GetUid(int x, int y );
	BOOL SetUid(int x, int y, int id);

	BOOL SetUidNPC(int x, int y, int id);
	void shouhu_rand(	ItemList *pMapItem);

	struct Target
	{
		int	id;
		int x;
		int y;
		int failCount;
//		int nLen;
//		TCHAR szName[CHAR_NAME_LENGTH + 1];
	};
	Target	m_Target;
	int		m_ItemUserLevel;		// Á×À»¶§ ¸ÅÁ÷ ÀÌ»ó ¾ÆÀÌÅÛ¸¦ ¶³±¸±âÀ§ÇØ ÂüÁ¶ÇØ¾ßÇÏ´Â À¯ÀúÀÇ·¹º§

	int		m_TotalDamage;	// ÃÑ ´©ÀûµÈ ´ë¹ÌÁö¾ç
	ExpUserList m_DamagedUserList[NPC_HAVE_USER_LIST]; // ³ª¿¡°Ô Å¸°İÄ¡¸¦ ÁØ À¯ÀúÁ¤º¸¸¦ ¸®½ºÆ®·Î °ü¸®ÇÑ´Ù.(°æÇèÄ¡ ºĞ¹è)
//	arUserList m_arDamagedUserList;

	BOOL	m_bFirstLive;		// NPC °¡ Ã³À½ »ı¼ºµÇ´ÂÁö Á×¾ú´Ù »ì¾Æ³ª´ÂÁö ÆÇ´Ü.
	BYTE	m_NpcState;			// NPCÀÇ »óÅÂ - »ì¾Ò´Ù, Á×¾ú´Ù, ¼­ÀÖ´Ù µîµî...
	BYTE	m_NpcVirtualState;	// NPC»óÅÂ -- À¯Àú°¡ µé¾î¿Ã¶§¸¸ È°¼ºÈ­ (ÀÏÈ¸¼ºÀ¸·Î »ì¾Æ³²)
	int		m_ZoneIndex;		// NPC °¡ Á¸ÀçÇÏ°í ÀÖ´Â Á¸ÀÇ ÀÎµ¦½º

	short	m_sNid;				// NPC (¼­¹ö»óÀÇ)ÀÏ·Ã¹øÈ£

	MapInfo	**m_pOrgMap;		// ¿øº» MapInfo ¿¡ ´ëÇÑ Æ÷ÀÎÅÍ

	int		m_nInitX;			// Ã³À½ »ı¼ºµÈ À§Ä¡ X
	int		m_nInitY;			// Ã³À½ »ı¼ºµÈ À§Ä¡ Y

	int		m_sCurZ;			// Current Zone;
	int		m_sCurX;			// Current X Pos;
	int		m_sCurY;			// Current Y Pos;

	int		m_sOrgZ;			// ¿ø·¡ DBÀÇ Zone
	int		m_sOrgX;			// ¿ø·¡ DBÀÇ X Pos
	int		m_sOrgY;			// ¿ø·¡ DBÀÇ Y Pos

	int		m_presx;			// ÀÌÀü ½Ã¾ß X
	int		m_presy;			// ÀÌÀü ½Ã¾ß Y

	//
	//	PathFind Info
	//
	CPoint	m_ptDest;
	int		m_min_x;
	int		m_min_y;
	int		m_max_x;
	int		m_max_y;

	long	m_lMapUsed;			// ¸Ê ¸Ş¸ğ¸®¸¦ º¸È£ÇÏ±âÀ§ÇØ(»ç¿ëÁß : 1, ¾Æ´Ï¸é : 0)
//	int		**m_pMap;

	int		m_pMap[MAX_MAP_SIZE];// 2Â÷¿ø -> 1Â÷¿ø ¹è¿­·Î x * sizey + y

	CSize	m_vMapSize;
	CPoint	m_vStartPoint, m_vEndPoint;

//	CPoint m_curStartPoint;
	CPathFind m_vPathFind;

	NODE	*m_pPath;

	int		m_nInitMinX;
	int		m_nInitMinY;
	int		m_nInitMaxX;
	int		m_nInitMaxY;

	CPoint		m_ptCell;					// ÇöÀç Cell À§Ä¡
	int			m_nCellZone;

	// NPC Class, Skill
	NpcSkillList	m_NpcSkill[SKILL_NUM];	// NPC °¡ °¡Áö°í ÀÖ´Â ½ºÅ³
	BYTE			m_tWeaponClass;			// NPC ÀÇ ¹«±â Å¬·¡½º 

	DWORD			m_dwDelayCriticalDamage;	// Delay Critical Damage
	DWORD			m_dwLastAbnormalTime;		// »óÅÂÀÌ»óÀÌ °É¸°½Ã°£

	BOOL			m_bRandMove;				// 8¹æÇâ ¼±ÅÃ Random Move ÀÎ°¡?
	RandMoveArray	m_arRandMove;				// 8¹æÇâ ¼±ÅÃ Random Move¿¡¼­ »ç¿ëÇÏ´Â ÁÂÇ¥ Array

	DWORD			m_dLastFind;				// FindEnemy ÇÔ¼ö¿¡ ÀÎÀ§Àû TermÀ» ÁÖ±â À§ÇØ »ç¿ëÇÏ´Â TickCount

	BOOL			m_bCanNormalAT;				// ÀÏ¹İ °ø°İÀ» ÇÏ´Â°¡?
	BOOL			m_bCanMagicAT;				// »çÀÌ¿À´Ğ °ø°İÀ» ÇÏ´Â°¡?
	BOOL			m_bCanSPAT;					// Æ¯¼ö °ø°İÀ» ÇÏ´Â°¡?

	BYTE			m_tSPATRange;				// Æ¯¼ö °ø°İ ¹üÀ§
	BYTE			m_tSPATAI;					// Æ¯¼ö °ø°İÀ» ÇÒ ¼ö ÀÖ´Â ¸÷ÀÏ¶§ ¾î´À °æ¿ì¿¡ Æ¯¼ö °ø°İÀ» ÇÒ°ÇÁö

	int				m_iNormalATRatio;			// ÀÏ¹İ °ø°İ È®·ü
	int				m_iSpecialATRatio;			// Æ¯¼ö °ø°İ È®·ü
	int				m_iMagicATRatio;			// ¸ÅÁ÷ °ø°İ È®·ü


	//----------------------------------------------------------------
	//	¾ÆÀÌÅÛÀ» Ã³¸®ÇÏ´Â º¯¼ö
	//----------------------------------------------------------------
	ItemList	m_NpcHaveItem[NPC_ITEM_NUM];	

	//----------------------------------------------------------------
	//	MONSTER DB ÂÊ¿¡ ÀÖ´Â º¯¼öµé
	//----------------------------------------------------------------
	int		m_sSid;				// MONSTER(NPC) Serial ID
	int		m_sPid;				// MONSTER(NPC) Picture ID
	TCHAR	m_strName[20];		// MONSTER(NPC) Name

	int		m_sSTR;				// Èû
	int		m_sDEX;				// ¹ÎÃ¸
	int		m_sVOL;				// ÀÇÁö
	int		m_sWIS;				// ÁöÇı

	int		m_sMaxHP;			// ÃÖ´ë HP
	int		m_sHP;				// ÇöÀç HP
	int		m_sMaxPP;			// ÃÖ´ë PP
	int		m_sPP;				// ÇöÀç PP

	BYTE	m_byClass;			// ¹«±â°è¿­
	BYTE	m_byClassLevel;		// ¹«±â°è¿­ ·¹º§
	
	DWORD	m_sExp;				// °æÇèÄ¡

	int 	m_byAX;				// °ø°İ°ª X
	int 	m_byAY;				// °ø°İ°ª Y
	int	    m_byAZ;				// °ø°İ°ª Z

	int		m_iDefense;		// ¹æ¾î°ª
	BYTE	m_byRange;			// »çÁ¤°Å¸®

	int		m_sAI;				// ÀÎ°øÁö´É ÀÎµ¦½º
	int		m_sAttackDelay;		// °ø°İµô·¹ÀÌ

	BYTE	m_byVitalC;			// ½ÅÃ¼µ¥¹ÌÁö Å©¸®Æ¼ÄÃ
	BYTE	m_byWildShot;		// ³­»ç ·¹º§
	BYTE	m_byExcitedRate;	// ÈïºĞ ·¹º§
	BYTE	m_byIronSkin;
	BYTE	m_byReAttack;
	BYTE	m_bySubAttack;		// »óÅÂÀÌ»ó ¹ß»ı(ºÎ°¡°ø°İ)
	BYTE	m_byState;			// ¸ó½ºÅÍ (NPC) »óÅÂÀÌ»ó

	BYTE	m_byPsi;			// »çÀÌ¿À´Ğ Àû¿ë
	BYTE	m_byPsiLevel;		// »çÀÌ¿À´Ğ·¹º§

	BYTE	m_bySearchRange;	// Àû Å½Áö ¹üÀ§
	int		m_sSpeed;			// ÀÌµ¿¼Óµµ	

	int		m_sInclination;
	BYTE	m_byColor;
	int		m_sStandTime;
	BYTE	m_tNpcType;			// NPC Type
								// 0 : Normal Monster
								// 1 : NPC
								// 2 : °¢ ÀÔ±¸,Ãâ±¸ NPC
								// 3 : °æºñº´

	int		m_sFamilyType;		// °¢ ¸÷µé°£ÀÇ °¡Á·°ü°è Á¤ÀÇ
	BYTE	m_tItemPer;			// ¾ÆÀÌÅÛ ¶³¾îÁú È®·ü
	BYTE	m_tDnPer;			// µ· ¶³¾îÁú È®·ü
	//----------------------------------------------------------------
	//	MONSTER AI¿¡ °ü·ÃµÈ º¯¼öµé
	//----------------------------------------------------------------
	BYTE	m_tNpcLongType;		// °ø°İ °Å¸® : ¿ø°Å¸®(1), ±Ù°Å¸®(0)
	BYTE	m_tNpcAttType;		// °ø°İ ¼ºÇâ : ¼±°ø(1), ÈÄ°ø(0)
	BYTE	m_tNpcGroupType;	// ±ºÁıÀ» Çü¼ºÇÏ³Ä(1), ¾ÈÇÏ³Ä?(0)
//	BYTE	m_tNpcTraceType;	// ³¡±îÁö µû¶ó°£´Ù(1), ½Ã¾ß¿¡¼­ ¾ø¾îÁö¸é ±×¸¸(0)

	//----------------------------------------------------------------
	//	MONSTER_POS DB ÂÊ¿¡ ÀÖ´Â º¯¼öµé
	//----------------------------------------------------------------
	int		m_sMinX;			// NPC ¿òÁ÷ÀÓ ¿µ¿ª
	int		m_sMinY;
	int		m_sMaxX;
	int		m_sMaxY;

	int		m_Delay;			// ´ÙÀ½ »óÅÂ·Î ÀüÀÌµÇ±â ±îÁöÀÇ ½Ã°£
	DWORD	m_dwLastThreadTime;	// NPC Thread ¿¡¼­ ¸¶Áö¸·À¸·Î ¼öÇàÇÑ ½Ã°£

	DWORD	m_dwStepDelay;
	short	m_sClientSpeed;		// ¾Ö´Ï ¼Óµµ ºñÀ²¸¦ Á¤ÀÇ(500 : 100 = 600 : x)

	BYTE	m_byType;
	int		m_sRegenTime;		// NPC Àç»ı½Ã°£
	int		m_sEvent;			// ÀÌº¥Æ® ¹øÈ£
	int		m_sEZone;			// ¸ó½ºÅÍ ÀÚÃ¼°¡ °¡Áö´Â ÀÌº¥Æ®Á¸

	int		m_sGuild;			// NPCTYPE_GUILD_GUARD¶ó¸é ÇØ´ç ÇÊµå»óÁ¡ ÀÎµ¦½º
								// NPCTYPE_GUILD_NPC¶ó¸é...

	short	m_sDimension;		// ÀÌº¥Æ®¸÷ ¶Ç´Â ±×¿Ü ¸Ê¿¡¼­ 2¼¿ ÀÌ»ó ÀÚ¸®¸¦ Â÷ÁöÇÏ´Â NPC¸¦ À§ÇØ

	int		m_sHaveItem;		// ¸ó½ºÅÍ°¡ Á×À» ´ç½Ã ÁÙ ¾ÆÀÌÅÛ ÀÎµ¦½º

	long	m_lEventNpc;

	//----------------------------------------------------------------
	//	»óÅÂÀÌ»ó°ü·Ã
	//----------------------------------------------------------------
	BYTE	m_tAbnormalKind;
	DWORD	m_dwAbnormalTime;
	
	//----------------------------------------------------------------
	//	±æµåÀü°ú °ü·Ã
	//----------------------------------------------------------------
	BYTE	m_tNCircle;			// 0 : Ã³À½»óÅÂ
								// 1 : ¹æ¾îÃø »óÅÂ
								// 2 : °ø°İÃø »óÅÂ

	BYTE	m_tRepairDamaged;	// ±æµåÀü½Ã ¼ö¸®ÇÒ ÇÊ¿ä¼º ÀÌ ÀÖÀ¸¸é
								// 0 : ÃÖ°í »óÅÂ
								// 1 : ¼Õ»óµÇ¾î ¼ö¸® ÇÒ ÇÊ¿ä°¡ ÀÖ´Â »óÅÂ
								// 2 : ¿ÏÀüÈ÷ ¸Á°¡Áø »óÅÂ

	CGuildFortress *m_pGuardFortress;
	CStore* m_pGuardStore;		// °æºñº´ÀÌ¶ó¸é ÇØ´ç »óÁ¡À» °¡Áö°í ÀÖ´Ù.
	BYTE	m_tGuildWar;		// 0 : ±æµåÀü ÁØºñ´Ü°è
								// 1 : ±æµåÀüÁß
								// 2 : ÀÏ¹İ ´Ü°è

	ItemUserRightlist		m_iHaveItemUid[NPC_HAVE_USER_LIST];
	long					m_lFortressState;
	long					m_lDamage;	// °¢ µ¥¹ÌÁö°¡ ÇÑ¹ø¿¡ ÇÏ³ª¾¿ µé¾î¿Àµµ·Ï µ¿±âÈ­ ÇÑ´Ù. by Youn Gyu 02-10-08

	BOOL	m_bSummon;
	int		m_sSummonOrgZ;
	int		m_sSummonOrgX;
	int		m_sSummonOrgY;
	int		m_SummonZoneIndex;

	BOOL	m_bSummonDead;
	LONG	m_lNowSummoning;

	int		m_TableZoneIndex;
	int		m_sTableOrgZ;
	int		m_sTableOrgX;
	int		m_sTableOrgY;

	LONG	m_lKillUid;					// ¸÷À» ¸¶Áö¸·À¸·Î Á×ÀÎ À¯ÀúÀÇ uid
	short	m_sQuestSay;				// Äù½ºÆ®¸¦ °¡Áö°í ÀÖ´Â ÀÌº¥Æ® ¸÷ÀÎ°æ¿ì != 0 (1 ~~ 10000)
	int Dead_User_level;
	int NpcDrop; //¹ÖÎï±¬³öÎïÆ·´ÎÊı
};

#endif // !defined(AFX_NPC_H__600048EF_818F_40E9_AC07_0681F5D27D32__INCLUDED_)
