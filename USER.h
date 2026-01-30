// USER.h: interface for the USER class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USER_H__24E85DFA_7A9E_4707_8A6A_BDEB26C34932__INCLUDED_)
#define AFX_USER_H__24E85DFA_7A9E_4707_8A6A_BDEB26C34932__INCLUDED_

#include "Define.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OV.h"
#include "Npc.h"

#include "EVENT_DATA.h"
#include "LOGIC_ELSE.h"
#include "EXEC.h"

#include "Store.h"
#include "Guild.h"
#include "GuildUSer.h"

#include "SharedMemory.h"
#include "MemUser.h"
#include "Compress.h"
#include "Cbsocket.h"
#include "IOCPSocket.h"

#include "Serverinfo.h"
#include "JvCryption.h"

#define PSI_LEN				((sizeof(short)) + (sizeof(BYTE)))

//#define _SKILL_DB			100
//#define _ITEM_DB			600
//#define _PSI_DB			100
#define _HISTORY_DB			500
#define _BHISTORY_DB		1000

//#define _CHANGE_DB		4
//#define _QUICKITEM_DB		10

#define MAX_LEVEL_DN		2000		// 饭骇寸 倾侩 家瘤陛 01_4_24

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>最高等级
#define ZUIGAO_LEVEL                f_Markini[0]->TJ_JGTJ

#define HEAD				0
#define BODY				1
#define RIGHT_HAND			4
#define LEFT_HAND			5
#define PANTS				8
#define SHOES				9

#define PA_MA_STATUS		1			// PA, MA 荐摹函悼 眉农
#define SKILL_STATUS		2			// 胶懦饭骇 荐摹函悼 眉农

#define POISON_TIME			(60 * 1000)
#define CONFUSION_TIME		(5 * 1000)   //眩晕时间
#define PARALYSIS_TIME		(20 * 1000)
#define BLIND_TIME			(60 * 1000)
#define LIGHT_TIME			(2 * 1000)
#define FIRE_TIME			(60 * 1000)
#define COLD_TIME			(10 * 1000)

#define BANK_ITEM_INPUT		1
#define BANK_ITEM_OUTPUT	2	
#define BANK_DN_INPUT		3
#define BANK_DN_OUTPUT		4

#define GUILD_ITEM_INPUT	1
#define GUILD_ITEM_OUTPUT	2	
#define GUILD_DN_INPUT		3
#define GUILD_DN_OUTPUT		4

#define BUDDY_JOIN			1
#define BUDDY_LEAVE			2
#define BUDDY_CHANGE		3

#define USE_CLASS_TYPE		8			// DB惑俊辑 荤侩且荐 乐绰 辆幅 啊瘤荐(1 努贰胶寸 弥措 m_byClass烙)

#define MAX_BUDDY_USER_NUM	4			//最大组队人数

#define DIR_DOWN			0			// 阿 焊绊乐绰 规氢阑 沥狼茄促.
#define	DIR_DOWNLEFT		1
#define DIR_LEFT			2
#define	DIR_UPLEFT			3
#define DIR_UP				4
#define DIR_UPRIGHT			5
#define DIR_RIGHT			6
#define	DIR_DOWNRIGHT		7

#define TOWN_POTAL_SIZE		6			// 罚待且 伎 裹困 荤捞令 (3伎 * 2)
#define SEARCH_TOWN_POTAL_COUNT	40			// 器呕 困摹甫 弥措 40锅 鳖瘤 茫阑 扁雀甫 霖促.

#define EVENT_HELP_BAND		600
#define COUNTERATTACK_MAX_NUM 8			// 弥措 8规氢俊辑 馆拜捞 老绢朝荐乐栏骨肺

#define CLIENT_WALK_TICK	500			// 跑步速度
#define CLIENT_RUN_TICK		300			// 移动速度

#define ITEM_UPGRADE_COUNT	5			// 酒捞袍 俺炼老锭 醚 俺炼 冉荐
#define ITEM_UPGRADE_VALUE	4			// 俺炼肺 牢秦 函拳登绰 己瓷蔼

#define ADD_USER_LEVEL		70

#define TEL_MAX_COUNT		61			// 传送门
#define TEL_ADD_EDIT		1	
#define TEL_DEL_EDIT		2

#define CORE_BUFF_SIZE		6000

#define RIPEL_TOP			798
#define RIPEL_BOTTOM		799
#define RIPEL_LEFT			800
#define RIPEL_RIGHT			801
#define RIPEL_CREST			802

#define PSI_DAMAGE_DELAY	0			// 限制释放速度。。。。1秒一次反应
#define NO_DAMAGE_TIME		7000		// GameStart, Zone Change 悼救狼 公利矫埃

#define HIEXP_TIME			(3600 * 2 * 1000)	// 兴奋时间2小时
#define MAGICFIND_TIME		(3600 * 2 * 1000)	// 幸运时间2小时
#define HIEXP_TIME_1212			(3600 * 12 * 1000)	// 超级兴奋时间12小时
#define MAGICFIND_TIME_1189		(3600 * 12 * 1000)	// 超级幸运时间12小时
#define BIANSHEN_TIME		(3600 * 10 * 1000)   // 变身时间控制1小时
#define HUANXIANG_TIME		(3600 * 10 * 1000)   // 幻想石头时间
#define VIP_TIME			(3600 * 240 * 1000)   // VIP48小时时间
#define CARD_TIME			(3600 * 60 * 1000)   // 点卡60小时时间
#define VIP_TIME_5			(3600 * 5 * 1000)   // VIP48小时时间
#define CHOUJIANG_TIME				10			// 10次抽奖机会

const short	SID_RING_OF_LIFE		= 702;
const short SID_NECKLACE_OF_SHIELD	= 703;
const short SID_EARRING_OF_PROTECT	= 704;
const short SID_EARRING_OF_FEIFEI	= 627;
const short SID_SHENGREN_JIEZHI		= 1125;//圣人戒指

class EVENT;
class CHyper;  
class COM;
class Item;
class CCircularBuffer;
class CVirtualRoom;

typedef  CTypedPtrArray <CPtrArray, Item*> ItemArray;

struct  SkillList
{
	short	sSid;
	BYTE	tLevel;
//	BYTE	tOnOff;
};

struct	PsionicList
{
	short sSid;
	BYTE  tOnOff;
};

struct ExchangeItem
{
	BYTE  bySlot;
	ItemList changedList;
};

struct ShopItem
{
	short  sSlot;
	DWORD  dwItemPrice;
	ItemList ShopList;
};

struct BuyItemList
{
	short sSid;
	short sNum;
};

struct BuddyList
{
	int		uid;								// 惑措规 uid
	DWORD	dwAddTime;								//加入时间
	TCHAR	m_strUserID[CHAR_NAME_LENGTH + 1];		// ID	
};

struct CounterAttack
{
	DWORD	dwLastAttackTime;					// 各捞 馆拜拌魂阑 夸没茄 矫埃
	int		iDamage;							// 馆拜 措固瘤
	int		iNpcUid;							// 秦寸 各
	long	lSid;								// 静饭靛俊狼茄 曼炼甫 硅力..
};

struct GuildUserList
{
	TCHAR	GuildUser[CHAR_NAME_LENGTH + 1];	// 辨靛盔 ID 
	short	sLevel;								// 立加吝牢 Level
};

struct TeleportList
{
	int		iSid;
	int		x;
	int		y;
	int		z;
	TCHAR	TelName[TEL_NAME_LENGTH + 1];
};

typedef CTypedPtrArray <CPtrArray, int*>			EventNumArray;
typedef CTypedPtrArray <CPtrArray, ExchangeItem*>	ExchangeArray;
typedef CTypedPtrArray <CPtrArray, ShopItem*>		ShopArray;
typedef CTypedPtrArray <CPtrArray, ItemList*>		ItemListArray;
typedef CTypedPtrArray <CPtrArray, ITEMLOG*>		ItemLogArray;

class USER : public CBSocket
{
public:
	USER();
	virtual ~USER();

public:
	TCHAR	m_RData[CORE_BUFF_SIZE+1];
	volatile DWORD m_Recv_value;
	volatile DWORD m_Send_value;

	T_KEY m_public_key;

	CJvCryption m_cryptfn;
	int m_isCryptionFlag;

	void SetPublicKey();

// zi_gi Session Time
public:
	BOOL m_bSessionLogOut;
	int	m_iDisplayType;			// 皋技瘤 鸥涝
	int	m_iValidTime;			// 蜡瓤 矫埃
	int m_iTimeInterval;		// 皋技瘤 埃拜
	SYSTEMTIME m_validDate;		// 蜡瓤 矫埃
	SYSTEMTIME m_LoginTime;		// 
	bool m_bSessionOnline;//离线商店定义 MarkJay
	short m_UserTopNum;//玩家排行名次 MarkJay

// IKING 2001.1.
public:
	CString				m_strZoneIP;
	int					m_nZonePort;
	int					m_iModSid;
	BOOL				m_UserFlag;
//	long				m_Recv_value;
	int					m_UserStatus;
	CRITICAL_SECTION	m_CS_ExchangeItem;
	CRITICAL_SECTION	m_CS_ShopItem;
	long				m_ConnectionSuccessTick;

public:
	void DBGameStartResult( char *pBuf, int s_index, int dlength );
	void DBAccountLoginResult(TCHAR *pBuf, int index, int dlength );
	void DBUpdateUserData(char *temp_buffer, int &temp_index );

	void SoftClose();
	int SocketDisConnect();

	void ChatReqWithLength(TCHAR *pBuf, int iLen);		// 牢磊蔼阑 崔府霖促.
	void RestartReq(TCHAR *pBuf);
	void GameStart(TCHAR* pBuf);
	void GuildDisperse(TCHAR *pBuf);
	void GuildOff(TCHAR *pBuf);
	void GuildKickOut(TCHAR *pBuf);
	void GuildWharehouseOpenReq();
	void GuildItemMoveReq(TCHAR *pBuf);

	void BBSDelete(TCHAR* pBuf);
	void BBSEdit(TCHAR *pBuf);
	void BBSWrite(TCHAR *pBuf);
	void BBSRead(TCHAR *pBuf);
	void BBSNext(TCHAR *pBuf);

	// zinee 02-11-15 
	void MailInfoReq( TCHAR* pBuf );
	void MailSendReq( TCHAR* pBuf );
	void MailListReq( TCHAR* pBuf );
	void MailContentReq( TCHAR* pBuf );
	void MailDeleteReq( TCHAR* pBuf );
	int  MailGetList( char* pBuf, int nStartIndex, int nCount, int& index );
	BYTE MailGetInfo( char* pBuf, int& index );//新邮件与读过邮件数

	void GuildMarkAddReq(TCHAR *pBuf);
	BOOL DeleteGuildUser(TCHAR *strUserID);//删除军团成员
	BOOL DeleteMarkChouJiang(TCHAR *iSname);//删除抽奖奖品数据
	void NewGuild(TCHAR *pBuf);

	// IKING 2002.1.
	void ZoneLogOut(int save_z, int save_x, int save_y);
	//
	void StopAction();
	int AcceptProcess();
	int SockCloseProcess(int nError=0);

	virtual void OnClose(int nErrorCode);
	virtual void OnSend(int nErrorCode);

	int NoEncodeSend(int length, char *pBuf);
	void ProcessData(char *pBuf, int length);
//

// Member Variable
public:
	COM*		m_pCom;
	//SOCKET		m_socket;
	int			m_uid;
	BYTE		m_state;

	//OV			m_ovRecv;
	//OV			m_ovSend;
	//OV			m_ovClose;

	// Memory DB
	CSharedMemory*	m_pSharedMemory;
	CMemUser*		m_pMD;

	//
	//	Zone Changing 
	//
	BOOL		m_NowZoneChanging;			// 促弗 粮栏肺 捞悼且锭 唱俊霸 府胶飘 
											// 昏力甫 舅妨临鞘夸绝澜.
	//
	//	Cell Info
	//
	CPoint		m_ptCell;					// 泅犁 Cell 困摹
	int			m_nCellZone;

	//
	//	Send, Receive Buffer
	//
	TCHAR		m_TempBuf[SEND_BUF_SIZE];

	//
	//	Account Data
	//
	int			m_iMyServer;
	TCHAR		m_strAccount[ACCOUNT_LENGTH + 1];		//账号
	int			m_nCharNum;								//人物数量
	TCHAR		m_strChar[3][CHAR_NAME_LENGTH + 1];		// 人物名字

	//
	//	Game Data
	//
	TCHAR		m_strUserID[CHAR_NAME_LENGTH + 1];			// 酒捞叼(某腐磐 捞抚)
	TCHAR		m_strTradeUser[CHAR_NAME_LENGTH + 1];		// 芭贰吝牢 某腐磐 捞抚
	int			m_iTradeUid;


	//	
	//	NPC俊 措茄 蜡历率俊辑 馆拜胶懦 贸府
	//													// 弥措 8付府 各府胶飘甫 蜡瘤		
	CounterAttack	m_CounterAttackList[COUNTERATTACK_MAX_NUM];


	//
	//	Item Info Data
	//
	int			m_ItemFieldInfoCount;					// 
	int			m_ItemFieldInfoIndex;
	TCHAR		m_ItemFieldInfoBuf[8192];

	ExchangeArray	m_arExchangeItem;	// 背券且 酒捞袍 格废
	ShopArray		m_arShopItem;		// Personal Shop Item Array
	//
	//	UpdateUserData
	//
	DWORD		m_dwLastUpdateTime;		// 付瘤阜栏肺 诀单捞飘茄 矫埃
	DWORD		m_dwLastSaveTime;		// Last User save time
	long		m_lRequireNum;

	DWORD		m_dwLastSpeedTime;		// 胶乔靛琴阑 规瘤窍扁困秦
	DWORD		m_dwLastMoveAndRun;		// SP : 付瘤阜栏肺 框流牢 矫埃
	DWORD		m_dwLastAttackTime;		// 啊厘 弥辟俊 傍拜茄 矫埃
	DWORD		m_dwLastPsiAttack;		// 啊厘 弥辟俊 荤捞坷葱栏肺 傍拜茄 矫埃
	DWORD		m_dwLastMaxAttack;      //上次使用必杀的时间
	DWORD		m_dwLastPsiTime;		// 啊厘 弥辟俊 荤捞坷葱栏肺 傍拜茄 矫埃
	DWORD		m_dwCastDelay;			// 荤捞坷葱 某胶泼 掉饭捞
	DWORD		m_dwLastTimeCount;		// 啊厘 弥辟俊 鸥捞赣甫 捣 矫埃
	DWORD		m_dwLastAbnormalTime;	// 惑怕捞惑捞 吧赴矫埃
	DWORD		m_dwLastHPTime;			// HP啊 雀汗等 付瘤阜 矫埃

	DWORD		m_dwLastPsiDamagedTime;	// 啊厘 弥辟俊 荤捞坷葱 单固瘤甫 涝篮 矫埃

	DWORD		m_dwLastHasteTime;		// 付瘤阜栏肺 庆捞胶飘啊 吧赴矫埃
	DWORD		m_dwLastShieldTime;		// 付瘤阜栏肺 蒋靛甫 敬 矫埃
	DWORD		m_dwLastBigShieldTime;
	DWORD		m_dwLastDexUpTime;		// 付瘤阜栏肺 DEX UP 阑 敬 矫埃
	DWORD		m_dwLastMaxHPUpTime;	// 付瘤阜栏肺 MAX HP UP 阑 敬 矫埃
	DWORD		m_dwLastAdamantineTime;	// 付瘤阜栏肺 Adamantine阑 敬 矫埃
	DWORD		m_dwLastMightyWeaponTime;//付瘤阜栏肺 MightyWeapon阑 敬 矫埃
	DWORD		m_dwLastBerserkerTime;	//付瘤阜栏肺 Berserker甫 敬 矫埃

	DWORD       m_ShouhuTime;          //守护减耐久
	DWORD       m_dwKanZBZT;                 //查看装备状态
	DWORD       m_dwLastItem44Time;          //闪药水时间
	DWORD       m_JiaXueTime;				 //疯狂加血时间
	DWORD       m_FuShouTime;          //守护减耐久 小黑副手

	DWORD       m_UserCardTime;          //用户充值在线时间

	DWORD       m_VIPCardTime;          //VIP用户在线时间

	DWORD		m_dwHasteTime;			// Haste
	DWORD		m_dwShieldTime;			// Shield
	DWORD		m_dwBigShieldTime;	
	DWORD		m_dwDexUpTime;			// Dex Up
	DWORD		m_dwMaxHPUpTime;		// Max HP Up
	DWORD		m_dwAdamantineTime;		// Adamantine
	DWORD		m_dwMightyWeaponTime;	// Mighty Weapon
	DWORD		m_dwBerserkerTime;		// Berserker
	
	DWORD		m_dwLastFastRunTime;		// 付瘤阜栏肺 Fast Run 阑 敬 矫埃
	DWORD		m_dwLastMindShockTime;		// 付瘤阜栏肺 Mind Shock 甫 敬 矫埃
	DWORD		m_dwLastMindGuardTime;		// 付瘤阜栏肺 Mind Guard 甫 敬 矫埃
	DWORD		m_dwLastPsiShieldTime;		// 付瘤阜栏肺 Psionic Shield 甫 敬 矫埃
	DWORD		m_dwLastPiercingShieldTime;	// 付瘤阜栏肺 Piercing Shield 甫 敬 矫埃

	DWORD		m_dwFastRunTime;			// Fast Run 阑 静绊 巢篮 矫埃
	DWORD		m_dwMindShockTime;			// Mind Shock 甫 静绊 巢篮 矫埃
	DWORD		m_dwMindGuardTime;			// Mind Guard 甫 静绊 巢篮 矫埃
	DWORD		m_dwPsiShieldTime;			// Psionic Shield 甫 静绊 巢篮 矫埃
	DWORD		m_dwPiercingShieldTime;		// Piercing Shield 甫 静绊 巢篮 矫埃

	BYTE		m_tPsiOneKind;
	BYTE		m_tPsiTwoKind;
	BYTE		m_tPsiThreeKind;

	DWORD		m_dwPsiOneTime;
	DWORD		m_dwPsiTwoTime;
	DWORD		m_dwPsiThreeTime;

	DWORD		m_dwLastHiExpTime;		// 付瘤阜栏肺 版氰摹 拱距阑 冈篮 矫埃
	DWORD		m_dwLastMagicFindTime;	// 付瘤阜栏肺 概流蛮胶 拱距阑 冈篮 矫埃
	DWORD		m_dwLastNoChatTime;		// 付瘤阜栏肺 盲陛阑 冈篮 矫埃
	DWORD		m_dwLastBFindTime;
	DWORD		m_dwLastVIPTime;
	DWORD		m_dwLastCKMiMa;
	DWORD		m_dwLastCloseTime;
	int			m_dwYuanBaoStart;
	int			m_JiaXue_Fail;		// 计算加血瓶数

	DWORD		m_dwLastHuanJing;
	DWORD		m_dwLastHuanLing;

	DWORD		m_dwLastFUExpTime;//幸福时光

	DWORD		m_dwHiExpTime;			//数据库
	DWORD		m_dwMagicFindTime;		
	DWORD		m_dwNoChatTime;	
	DWORD		m_dwBFindTime;
	DWORD		m_dwVIPTime;
	DWORD		m_dwCloseTime;
	DWORD		m_dwShopPingDN;
	DWORD		m_dwGuarDianTianShi;
	DWORD		m_dwJiFen;
	DWORD		m_dwChouJiang;
	DWORD		m_dwLastChouJiang;
	DWORD		m_dwRenWu;
	DWORD		m_dwCKMiMa;
	DWORD		m_dwHuanJing;
	DWORD		m_dwHuanLing;
	DWORD		m_dwZaiXianTime;
	DWORD		m_dwPKAddkill;
	DWORD		m_dwFUExpTime;//幸福时光
	DWORD		m_dwJiaGong;//小黑修炼攻击
	DWORD		m_dwJiaFangYu;//小黑修炼防御
	DWORD		m_dwJiaMoFang;//小黑修炼魔防
	DWORD		m_dwJiaGongJi;//加攻击 小黑修炼
	DWORD		m_dwJiaXueLiang;//加血量
	DWORD		m_dwJiaMoney;//加金钱

	DWORD		m_dwAbnormalInfo;		// 惑怕捞惑 沥焊
	DWORD		m_dwAbnormalInfo_;		// 第二个状态

	DWORD		m_dwNoDamageTime;		// Game Start, Zone Change 饶 单固瘤甫 涝瘤 臼绰 矫埃
	DWORD		m_dwLastNoDamageTime;	// 付瘤阜栏肺 Game Start, Zone Change 殿阑 茄 矫埃
	
	DWORD		m_iHPIntervalTime;		// HP 雀汗 矫埃 埃拜
	DWORD		m_iSPIntervalTime;		// SP 雀汗 矫埃 埃拜
	DWORD		m_iPPIntervalTime;		// PP 雀汗 矫埃 埃拜
	DWORD       m_ionlineTime;          // 自动涨经验

	int			m_oldx,m_oldy,m_oldz,m_MoveAutoTime,m_CheckCode,m_CodeFailNum;//验证码相关

	CString		UserCodeStr;

	int			m_iHPRate;				// HP 雀汗 厚啦
	int			m_iPPRate;				// PP 雀汗 厚啦
	int			m_iSPRate;				// SP 雀汗 厚啦
	
	BOOL		m_bWhisper;				// 庇富 倾侩/芭何

//	BOOL		m_bTradeMode;			// Trade 倾侩/芭何
	BOOL		m_bTradeWaiting;		// Trade 夸没吝
	BOOL		m_bNowTrading;			// 泅犁 芭贰吝
	BOOL		m_bExchangeOk;			// 背券铰倡
	BOOL		m_bTradeItem;			// 酒捞袍阑 窍唱扼档 棵啡栏搁...
	DWORD		m_TradeMoney;			// 芭贰 吝牢 陛咀

	BOOL		m_bPShopOpen;			// Personal shop open or closed
	BOOL		m_bPShopView;			// Personal shop being viewed
	int			m_iPShopViewuid;		// Personal shop view uid
	BOOL		m_bViewingAShop;		// User viewing a personal shop

	BOOL		m_bNoItemMove;			// 诀弊饭捞靛, 俺炼, 府概流殿 酒捞袍阑 颗辨 荐 绝绰 葛靛老锭 悸泼

	BuyItemList	m_TradeItemNum[TOTAL_ITEM_NUM];

	BOOL		m_bLogOut;				// Logout 吝牢啊?
	BOOL		m_bZoneLogOut;			// ZoneLogOut 吝牢啊?
	int			m_presx;				// 矫具 拌魂侩 x (捞傈 矫具)
	int			m_presy;				// 矫具 拌魂侩 y (捞傈 矫具)

	//
	//	Buddy Data
	BOOL		m_bBuddyMode;			// Buddy 倾侩/芭何
	BOOL		m_bNowBuddy;			// 泅犁 滚叼吝
	BOOL		m_bMakeBuddy;			// 滚叼 惯扁牢
	BuddyList	m_MyBuddy[MAX_BUDDY_USER_NUM]; // 蜡瘤 且荐乐绰 滚叼 备己盔 格废

	//
	//	Guild Data
	short		m_sGuildVersion;		// 辨靛 巩剧 滚怜
	TCHAR		m_strGuildName[CHAR_NAME_LENGTH + 1];	
	BOOL		m_bGuildMaster;			// 辨靛 付胶磐
	BOOL		m_bRepresentationGuild;	// 鼻茄 措青 咯何	 
	ItemList	m_GuildItem[TOTAL_BANK_ITEM_NUM];// Guild Item DB
	DWORD		m_dwGuildDN;					// Guild DN DB
	GuildUserList m_MyGuildUser[MAX_GUILD_USER];

	int         m_dwGuildLevel;

	BYTE		m_tFortressWar;			// 傍己傈老锭...
	BYTE		m_tGuildWar;			// 惑痢 辨靛傈老锭...
	BYTE		m_tGuildHouseWar;		// War at Virtual Room 老锭...
	BYTE		m_tQuestWar;			// 涅胶飘甫 柳青吝老锭

	BOOL		m_bFieldWarApply;		// 鞘靛傈阑 脚没茄率父 悸泼
	int			m_FieldWarGMUid;		// 鞘靛傈老锭 惑措规 辨付 uid
	int			m_dwFieldWar;			// 鞘靛傈 矫累捞搁 惑措祈 辨靛 锅龋啊 甸绢柯促.
	//--yskang 0.1
	TCHAR		m_strLoveName[LOVE_NAME_LENGTH];//辨靛 扁瓷吝 龋莫阑 何咯 窍扁困茄 函荐....

	//
	//	炮饭器飘 困摹甫 MAX父怒 历厘茄促. 
	//
	TeleportList m_strTeleport[TEL_MAX_COUNT];
	long		m_lCopyUsed;				// 1 捞搁 泅犁 墨乔抹捞 抗距登绢 乐促 
	int			m_CopyUid;
	BYTE		m_tIndex;
	TCHAR		m_strCopyUser[CHAR_NAME_LENGTH + 1];	

	//
	//	CityRank Data
	BOOL		m_bPkStatus;			// 泪矫悼救 墨坷惑怕牢瘤甫 汲沥
	BOOL		m_bPkKeyDown;			//PK是否开启
	DWORD		m_dwPkStatusTime;		// 墨坷己氢老锭 瘤加 矫埃甫 魄沥
	int			m_iCityValue;			// 泅犁 穿利 己氢蔼
	short		m_sCityRank;			// 矫刮 殿鞭
	short		m_sKillCount;			// PK茄 冉荐

	//
	//  EVENT Data
	EventNumArray	m_arEventNum;		// 角青茄 捞亥飘 锅龋

	//
	//  Item Weight Data
	int			m_iMaxWeight;			// 蜡历啊 甸荐乐绰 弥措 公霸肺 (con+str)*10 + 扁夯公霸 + m_sLevel * 6;
	int			m_iCurWeight;			// 泅犁 公霸
	BOOL		m_bRefuse;				// 酒捞袍 罐扁 芭例 可记 TRUE = 芭例, FALSE = 罐扁 铰倡  

	// 某腐磐 扁夯 加己
	BYTE		m_bLive;				// 磷菌聪? 混疽聪?
	BYTE		m_bLock;                // 封号

	short		m_sSTR;					// 塞
	short		m_sCON;					// 扒碍
	short		m_sDEX;					// 刮酶己
	short		m_sVOL;					// 狼瘤
	short		m_sWIS;					// 瘤驱
	
//	short		m_sAbilitySum;			// 瓷仿摹 钦拌

	bool		m_sDramageUp;			// 伤害提高
	bool		m_sDramageUpPercent;	// 伤害提高百分比
	BOOL		m_VipLive;


	DWORD		m_dwYBUpTime;			// 自动增长元宝时间
	DWORD		m_dwJZBUpTime;			// 自动增长决战币时间
	DWORD		m_WaiGuaTime;			// 检测外挂时间
	DWORD		m_AutoWaiGuaTime;		// 自动挂机时间
	DWORD		m_dwEXPUpTime;			// 自动涨经验时间

	DWORD		m_dwUPZaiXianTime;			// 在线时间记录	
	DWORD		m_dwLastUPZaiXianTime;		//最后的时间记录

	DWORD		m_dwUPXiuLianTime;			// 小黑修炼时间记录	
	DWORD		m_dwLastUPXiuLianTime;		// 最后的修炼时间记录

	DWORD		m_iSkin;				// 乔何祸
	DWORD		m_iHair;				// 赣府祸
	short		m_sGender;				// 己喊
	TCHAR		m_strFace[10];			// 倔奔葛剧

	int			m_curx;					// 泅犁 X 谅钎
	int			m_cury;					// 泅犁 Y 谅钎
	int			m_curz;					// 泅犁 粮
	int			m_ZoneIndex;			// 泅犁 粮狼 牢郸胶

	DWORD		m_dwBuddy;				// 滚叼锅龋
	long		m_dwGuild;				// 辨靛锅龋
	bool		isGuildRen;             //军团翅膀开关
//	DWORD		m_dwGuild;				// 辨靛锅龋

	DWORD		m_dwExp;				// 饭骇版氰摹
	DWORD		m_dwExpNext;			// 促澜 饭骇版氰摹	(版氰摹抛捞喉俊辑 掘绰促.)

	DWORD		m_dwLastDownExp;		// 付瘤阜栏肺 磷菌阑锭 冻绢哆赴 版氰摹

	short		m_sPA;					// PA Point
	short		m_sSkillPoint;			// Skill Point
	short		m_sSkillPoint_;
	
	DWORD		m_dwXP;					// X Point

	short		m_sMaxHP;				// 弥措 HP
	short		m_sHP;					// 泅犁 HP
	short		m_sMaxPP;				// 弥措 PP
	short		m_sPP;					// 泅犁 PP
	short		m_sMaxSP;				// 弥措 SP
	short		m_sSP;					// 泅犁 SP

	DWORD		m_dwDN;					// 家瘤陛

	short		m_sLevel;				// 拌凯饭骇
	BYTE		m_byClass;				// 努贰胶

	TCHAR		m_strSkill[_SKILL_DB];	// User Skill DB
	TCHAR		m_strItem[_ITEM_DB];	// User Item DB
	TCHAR		m_strPsi[_PSI_DB];		// User Psionic DB

	short		m_sChangeClass[_CHANGE_DB];	// 傈流矫 努贰胶饭骇 历厘

	BYTE		m_tAbnormalKind;
	DWORD		m_dwAbnormalTime;

	BYTE		m_tIsOP;					// 款康磊牢瘤 魄窜
											// 0: Normal User
											// 1: Game Operator
											// 2: Server Operator

	// 某腐磐 扁夯 加己 (Reroll 侩)
	BYTE		m_sTempSTR;					// 塞
	BYTE		m_sTempCON;					// 扒碍
	BYTE		m_sTempDEX;					// 刮酶己
	BYTE		m_sTempINT;					// 瘤侥
	BYTE		m_sTempVOL;					// 狼瘤
	BYTE		m_sTempWIS;					// 瘤驱

	BYTE		m_tTempClass;				// 烙矫 努贰胶

	DWORD		m_dwLastShoutChatTime;		//最后一次呐喊时间

	BOOL		m_isUser; //玩家
	BOOL		m_MItemLock;//物品保护锁
	int			m_GongXiFaCai;//避免误伤

	// 酒捞袍 概流 加己	
	short		m_DynamicUserData[MAGIC_COUNT];			// 概流 加己捞 嘿阑版快 函拳登绰 瓷仿摹
	short		m_DynamicEBodyData[EBODY_MAGIC_COUNT];	// EBody 加己捞 嘿阑版快 函拳登绰 瓷仿摹
	short       m_DynamicMagicItem[10];


//	BYTE		m_DynamicSkillInfo[TOTAL_SKILL_NUM];// 概流 加己 胶懦钎饭骇函悼甫 m_UserSkill[]俊 措莫栏肺 函券 

	DWORD		m_dwSaintTime;

//-------------------------[ End DB ] ---------------------------------------------//

	int			m_nHavePsiNum;			// 荤捞坷葱 焊蜡荐(阿 蜡历付促 促抚)
	BYTE		m_tDir;					// 蜡历啊 焊绊 乐绰 规氢( move(), Attack(), PsiAttack() : 窃荐俊 拱覆)
	
	SkillList	m_UserSkill[TOTAL_SKILL_NUM];
	SkillList	m_UserSkill_[12];
	BYTE		m_UserSkillInfo[TOTAL_SKILL_NUM];
	short		m_UserChangeSkillInfo;// 傈流且版快 促弗 拌凯俊辑 胶懦甫 啊廉坷绰 秦寸 胶懦 牢郸胶

	ItemList	m_UserItem[TOTAL_ITEM_NUM];

	ItemList	m_UserBankItem[TOTAL_BANK_ITEM_NUM];// User Bank Item DB
	DWORD		m_dwBankDN;							// User Bank DN DB

	ItemList	m_AccountBankItem[TOTAL_ACCOUNT_BANK_ITEM_NUM];	//Account Bank Item DB
	DWORD		m_dwAccountBankDN;								//Account Bank DN DB

	char		m_PersonalShopName[SHOP_NAME_LENGTH + 9];//修复魔法卡号 MarkJay

	PsionicList	m_UserPsi[TOTAL_PSI_NUM];

	CCircularBuffer *m_pCB;
	CCircularBuffer *m_pCBCore;

	// Speed Hack Check 侩 函荐甸
	int m_iTickCount;
	BOOL m_bSpeedHackEnble;//封加速检测 MarkJay
	DWORD m_dwServerTickBasic;
	DWORD m_dwServerTick;
	DWORD m_dwClientTick;
    DWORD m_dLastCheckTick;
	DWORD m_dCheckTick;
    DWORD m_dTotCheckTick;

	// alisia
	CCompressManager	m_CompMng;
	int					m_CompCount;
	TCHAR				m_CompBuf[10240];
	int					m_iCompIndex;
	DWORD				m_dwSaintTimeCount;

	// User Dead老锭 促弗 率俊辑 曼炼 给窍档废..
	long				m_lDeadUsed;

	// 家葛己 厩技荤府率 函荐甸
	BOOL				m_bRingOfLife;
	BOOL				m_bNecklaceOfShield;
	BOOL				m_bEarringOfProtect;
	BOOL				m_bCBOff;
	BOOL				m_bShengRenjiezhi;

	// Member Function
public:
	void SetFastRun(int iTime);
	BOOL SetMindShock(int iTime);
	void SetMindGuard(int iTime);
	void SetPsiShield(int iTime);
	void SetPiercingShield(int iTime);
	void QuestWindowOpenReq();
	void BianShen(int x);
	//void GWBianShen(int x);
	void ResultGuardianDuration();//守护能源
	void ResultFuShouDuration();//副手电池 小黑副手
	void SendAccessoriDuration(short sSid);
	void ClearAbnormalTime(DWORD dwType);
	void SendDGDuration(short sSid,int slot);
	BOOL SummonMonster(CString szName, CPoint pt);
	void GetResource();
	int FindItem(int sid);
	BOOL FindEvent(int event_num);
	void UserHaveEventDataToStr(TCHAR *pBuf);
	void StrToHaveEventData(TCHAR *pBuf);
	void InitEventSlot();
	void GetWideRangeAttack(int x, int y, int damage);
	int GetQunGongAttack(int x, int y, int damage);//群攻
	int GetPsiAttackUp();// 取得法师精修伤..
	BOOL UpdateUserStatus();
	void UpdateUser();
	BOOL UpdateUserItemDN();
	BOOL UpdateUserItemShopPingYB();//保存元宝
	BOOL UpdateUserItemHiExpTime();//保存兴奋时间
	BOOL UpdateUserItemMagicFindTime();//保存幸运时间
	BOOL UpdateUserItemFUExpTime();//保存幸福时光时间
	void DropItemFromDeadUser(BOOL bPK, int enemyrank = -1);
	DWORD GetItemCostByCityRank(int sid, int nRate);
	void GetCityRank();
	void IsChangeCityRank(int iMyCityRank, USER *pUser);
	void IsLegalDefence(USER *pUser);
	BOOL IsLegalDefence_isdead(USER *pUser); //yskang 0.7 辨靛傈矫 惑措 辨靛盔 磷捞搁 墨坷 登绰巴 规瘤 困窍咯 
	BOOL CheckRunSpeedHack(DWORD checkTick, BOOL bTwo);
	BOOL CheckMoveSpeedHack(DWORD checkTick);
	void MopCallReq(TCHAR *pBuf);
	void SetCounterAttack(int uid, int damage);
	void SendMyWeaponChangeInfo();
	BOOL UpdateUserExchangeData(USER *pUser);
	void GetRecoverySpeed();
	int GetMaxDamage(BYTE tWeaponClass);
	void SendItemFieldInfoToMe();
	void AddItemFieldInfoToMe(ItemList *pItem, BYTE type, int x, int y);
	CPoint ConvertToServer(int x, int y);
	CPoint ConvertToServerByZone(int z, int x, int y);
	void SendToRange(char* pBuf, int index, int min_x, int min_y, int max_x, int max_y);
	void SendRangeInfoToMe(int min_x, int min_y, int max_x, int max_y, BYTE flag);
	void FillUserInfo(char* pBuf, int& index, BYTE flag);
	void SendUserInfoBySightChange(int dir_x, int dir_y, int prex, int prey);
	CPoint ConvertToClient(int x, int y);
	BOOL IsMovable_C(int x, int y);
	BOOL IsMovable_S(int x, int y);
	BOOL IsMovable_S(int zone, int x, int y);
	CPoint FindNearAvailablePoint_C(int x, int y);
	CPoint FindNearAvailablePoint_S(int x, int y);
	CPoint FindNearAvailablePoint_S(int zone,int x, int y);
	BOOL IsReservedID(char* szName);
	int GetFinalDamage(USER* pUser, int nInitDamage, BYTE tMyWeapon,BOOL &bIsCritical,int max=0);//yskang 0.3
	void IsDeadUser();
	BOOL Teleport(int xpos, int ypos);
	void SetHide(int iTime);
	void SetShield(int iTime);
	void SetBigShield(int iTime);
	void SetFireDamage();
	void SetConFusion();//眩晕
	void UpdateTuBiao();//更新图标 MarkJay
	void DelTuBiao();//删除图标 MarkJay
	void SetHaste(int iTime);
	void EmoGuangChang();//恶魔广场
	short GetSkillLevel(BYTE tWeaponClass, short sSid);
	int SendDamagedItem(int totalDamage);
	int GetCounterAttack();
	double GetIronSkill();
	void SetUserToMagicUser(int iMaxHpUp = 0);
	void CheckMagicItemMove();
	void HelpStatusOpenReq();
	void GetMagicItemSetting();
	void GiveItemToUser(TCHAR *pBuf);
	void GiveMoneyToUser(TCHAR *pBuf);
	BOOL IsThereUser(USER *pUser);
	int GetDirection(CPoint ptStart, CPoint ptEnd, int nDirCount = 8);
	BOOL IsCity();
	BOOL IsMapCheck();
	BOOL IsException(int iTargetID, BYTE tPsi, CPoint ptPsi);
	int IsPKZone(USER *pUser);
	BOOL ISUserInSight();
	BOOL SendRestartLoginResult();
	void BuddyChat(TCHAR *pBuf);
	BOOL CheckBuddyMember(TCHAR *pID);
	void SendBuddyUserLeave(int iCount);
	void BuddyUserChange(TCHAR *pBuf, BYTE tType,int iLen); //修复组队报错 小黑
	void SetBuddyMode(TCHAR *pBuf);
	void SendBuddyUserChange(BYTE type, int iCount = 1);
	void BankOutPutDN(TCHAR *pBuf);
	void BankInPutDN(TCHAR *pBuf);
	void BankOutPut(TCHAR *pBuf);
	void BankInPut(TCHAR *pBuf);
	void BankItemMoveReq(TCHAR *pBuf);
	void InitUserBankItem();
	void MakeInintItem(TCHAR *pBuf, BYTE tClass);
	BOOL IsCanEquipItem(int iSlot);
	void SendUserTownPotal(USER *pUser, BYTE tMode	/*INFO_MODIFY*/);
	void SendMyTownPotal(BYTE towho, BYTE type);
	void InitUserItem();
	BOOL UpdateUserBank();
	void UserBankItemToStr(TCHAR *pBuf);
	void StrToUserBankItem(TCHAR *pBuf);
	BOOL LoadUserBank();
	void BankOpenReq();
	void SendAllCure(int iStoreID, int iCost);
	short GetNeedPP(BOOL* bSuccessSkill, BYTE tWeaponClass, short sSid);
	short GetHP();
	CPoint FindNearRandomPoint(int xpos, int ypos);
	void DeleteCellUidFromTownPotal();
	void AddCellUidFromTownPotal();
	void SendTownPotal(BYTE result, BYTE changeZone);
	void SendUserStatusSkill();
	void SetColdDamage();
	void SetDecBullNum(BYTE tSlot, short sDec);
	void UserTimer();
	BOOL CheckPsiCastingDelay(/*short sSid*/);
	void TownPotal();
	void GetMapMoneyInfo(DWORD dwDN, CPoint pt, int uid = -1);
//	void GetMapItemInfo(ItemListArray &arList, CPoint pt, int uid = -1, BYTE tMode = ITEM_INFO_MODIFY);
	void SendItemFieldInfo(BYTE type, BYTE towho, ItemList *pItem, int x, int y);

	void GiveItemToMap(TCHAR *pBuf, BOOL bChao = FALSE);
	void PKGiveItemToMap(TCHAR *pBuf, BOOL bChao = FALSE);
	void GiveMoneyToMap(TCHAR *pBuf);
	int CheckSkillClass(short sSid);
	void SendBBSError(BYTE tError);
	//yskang 0.3 void SendAttackSuccess(int tuid, CByteArray& arAction1, CByteArray& arAction2, short sHP, short sMaxHP);
	void SendAttackSuccess(int tuid, BOOL bIsCritical, short sHP, short sMaxHP);//yskang 0.3
	void SendAttackMiss(int tuid);
	void SendAttackFail(BYTE tErrorCode);
	void SendXP();
	void SendBullNum(BYTE tSlot);
	BOOL CheckRenWuZhangHao(TCHAR* UserAccount,TCHAR* UserID);//检测人物帐号
	BOOL PsionicProcess(int nTarget, short sSid, CPoint pt, short sNeedPP);
	BOOL IsSuccessPsi(BOOL* bSuccessSkill, BYTE tWeaponClass, short sSid);
	int GetPsiRange(short sSid);
	int DecAttackPP(BOOL *bSuccessSkill, BYTE tWeaponClass, short sSid, BOOL bDec = TRUE);
	void SendPP(BOOL bLevelDown = FALSE);
	short GetPP();
	BOOL IsHavePsionic(BYTE tSid);
	int CheckPsiClass(short sSid);
	void BuyPsi(TCHAR *pBuf);
	CStore* GetPsiStore(int nStore);
	void HyperTextEvent(TCHAR *pBuf);
	void NpcEvent(TCHAR *pBuf);
	BYTE GetNpcClass(CNpc* pNpc);
	void SendHP();
	void SendDuration(BYTE tSlot, int iDamage);
	int GetItemFieldNum(int iNum);
	void ItemUseReq(TCHAR *pBuf);
	void SendPsiOpen(int nStore);
	void SendWeatherInMoveZone();
//	void SendWeatherToZone(BYTE tType, BYTE tOnOff);
	void UserBasicAbility();
	void UserStatusSkillUp(TCHAR *pBuf);
	void SendExp(int tType = 1);
	// TRUE捞搁 蜡历俊狼秦 磷澜
	void GetLevelDownExp(BOOL bPK = FALSE, int enemyrank = -1, BOOL bEventSummon = FALSE,TCHAR *strPKName = NULL);//yskang 0.8 dead_log俊 磊脚阑 磷牢 荤侩磊甫 巢扁扁 困窍咯
	void GetExp(DWORD iNpcExp);
	void GetExpBuddy(int iNpcExp );
	void GetExpCommon( int iNpcExp );
	void SendPsiStoreOpen(int iStoreID);
	void ZoneMoveReq(int zone, int moveX, int moveY);
	BOOL LiveCity(int x, int y, BYTE type = INFO_MODIFY);
	BOOL ZoneChangeProcess(int zone, int moveX, int moveY);
	void SendZoneChange(BOOL bResult);
	short GetDecDuration(short sSid);
	short GetItemDefense(short sSid, int slot);
	int GetCriticalInitDamage(BOOL *bSuccessSkill, BYTE tWeaponClass);
	double GetCriticalHit(BYTE tWeaponClass,int max);
//	BOOL GetCriticalHit(BOOL* bSuccessSkill, BYTE tWeaponClass);
//	int GetWeaponDamage(short sSid);
	int GetWeaponDamage(short sSid, BOOL bMax = FALSE);
	int GetWeaponDamagePSI(short sSid, BOOL bMax = FALSE);//获得武器魔法攻击
//	int GetNormalInitDamage(BOOL* bSuccessSkill, BYTE tWeaponClass);
//yskang 0.3	int GetNormalInitDamage(BYTE tWeaponClass, USER* pTarget, BOOL bMax = FALSE);
	int GetNormalInitDamage(BYTE tWeaponClass, USER* pTarget,BOOL &bIsCritical, BOOL bMax = FALSE);
	double GetCriticalGuard(BOOL *bSuccessSkill, BYTE& tWeaponClass);
//	BOOL GetCriticalGuard(BOOL *bSuccessSkill, BYTE& tWeaponClass);
//	int GetAvoid(BYTE tClass);
	int GetAvoid();
	int GetHitRate(short sSid, BYTE tArmClass);
	int GetAttackRange(short sSid);
	int DecAttackSP(BOOL* bSuccessSkill, BYTE tWeaponClass);
	int GetMaxDuration(short sSid);
	void SendQuickChange();
	void ItemLoadReq(TCHAR *pBuf);
	void ItemGiveReq(TCHAR *pBuf);
	void SendSroreFail(int iType, BYTE err_code);
	int GetBreakDec();
	int GetBreakRate(short sSid);
	BOOL CheckAttackDelay(BOOL *bSuccess, BYTE tWeaponClass);
	void SendSP(BOOL bLevelDown = FALSE);
	short DecSP(short sDec);
	short GetSP(void);
	void RepairItem(TCHAR *pBuf);
	DWORD GetRepairCost(short sSlot);
	void RepairItemOpenReq(int iStoreID);
	void SellItem(TCHAR *pBuf);
	BOOL IsCanUseWeaponSkill(BYTE& tWeaponClass);
	void SwapItem(int nSlot1, int nSlot2);
	int GetEmptyInvenSlot(int *pEmpty);
	void UpdateInvenSlot(CWordArray *arEmptySlot,CWordArray *arSameSlot = NULL, BYTE tType = 3);

	DWORD GetSellCost(short sSlot);
	void SellItemOpenReq(int sStoreID);
	void UserInfoReq(TCHAR* pBuf);
	void SendMoneyChanged(void);
	void ReSetItemSlot(ItemList *pItem);
	void UserRemoteCallReq(TCHAR *pBuf);
	void UserCallReq(TCHAR *pBuf);
	void ChangeDir(TCHAR* pBuf);
	void SetWeather(int tType, int tOnOff);
	void SendExchangeItem(USER* pUser, BYTE bySlot, DWORD dwCount);
	void SendExchangeFail(BYTE result, BYTE error_code);
	void PsiAttack(TCHAR* pBuf);
	void MopCatchReq(TCHAR* pBuf);
	//void SendDamageNum(BYTE type,short nTarget, short dDamage);
	//void SendNpcHP(int nTarget,int npchp);
//	void SendTradeSuccess(int uid);
	void SendTradeSuccess(int uid, TCHAR *pstrID);
	void SendTradeFail(int uid, BYTE error_code);
	void UserSkillInfoToStr(TCHAR* pBuf);
	void UserPsiToStr(TCHAR* pBuf);
	void StrToUserPsi(TCHAR* pBuf);
	void UserSkillToStr(TCHAR* pBuf);
	void StrToUserSkill(TCHAR* pBuf);
	void UserItemToStr(TCHAR* pBuf);
	void StrToUserItem(TCHAR* pBuf);
	void SendSystemMsg(TCHAR *pMsg, BYTE type, int nWho);
	void GetEquipPos(BYTE byWear, CPoint& pt);
	int GetEBodyPos(BYTE byWear);
	int GetSameItem(ItemList Item, int nSlot);
	int GetEmptySlot(int nSlot);
	int GetSlotClass(int nSlot, int nType = 0, BOOL flag = TRUE);
	BYTE GetSkillRate(BYTE byClass, int sid, int nDefusion = 0);
	void GetSkillInfo(int sid, BYTE& tRate);
	DWORD GetNextLevel(int iCurrLevel);
	DWORD GetNextPAMAExp(DWORD dwExp);
	void MakeEmptyPsi(TCHAR *pBuf);
	void MakeEmptySkill(TCHAR* pBuf, BYTE tClass);
	BOOL IsDoubleAccount(char* account);//增加离线商店功能 MarkJay
	BOOL GetDistance(int xpos, int ypos, int dist, int* ret = NULL);

//	void ChangeCellUid(CPoint ptOld, CPoint ptNew);
//	void DeleteCellUid();
//	void AddCellUid();
//	BOOL SetCellIndex(int zone, int xpos, int ypos);

	void LiveReq(TCHAR *pBuf, BYTE tOption = 0);
	void RevivalReq( TCHAR *pBuf );//神之泪..复活
	void SendPsiAttackResult(BYTE result, int tuid, BYTE psi);
	int GetAttack();
	void SetDamage(int nDamage);
	int GetDefense(short* sItemDefense = NULL);
	void SendNpcInfo(CNpc* pNpc, BYTE tMode = INFO_MODIFY);
	CNpc* GetNpc(int nid);
	CNpc* GetNpc(TCHAR* pNpcName);
	void SendRange(TCHAR *pBuf, int nLength, CRect rect);
	void SendScreen(TCHAR *pBuf, int nLength);
	void SendInsight(TCHAR* pBuf, int nLength);
	void SendExactScreen(TCHAR* pBuf, int nLength);
	void SendZone(TCHAR* pBuf, int nLength);
	void SendAll(TCHAR* pBuf, int nLength);
	void SendMyInfo(BYTE towho, BYTE type);
	void SendCharData();
	void SendUserInfo(USER *pUser, BYTE tMode = INFO_MODIFY);
//	void SendUserInfo(int nUid, BYTE tMode = INFO_MODIFY);

	BOOL		InitUser();
	int			Init( int bufCreateFlag = 0 );
	void		Send(TCHAR* pBuf, int nLength);
	void		LogOut();

	//void		Close();
	//void		Receive();

	// Inline Function
	USER*	GetUser(int uid);
	USER*	GetUser(TCHAR* id);
	int     ZHUANGBEIID(int x1);
	int		GetUid(int x, int y);
	BOOL	SetUid(int x, int y, int id);
	void Attack(TCHAR* pBuf);

	void	AddAbnormalInfo(DWORD dwAbnormal);
	void	DeleteAbnormalInfo(DWORD dwAbnormal);
	BOOL	CheckAbnormalInfo(DWORD dwAbnormal);
	void	ClearAbnormalGroup(DWORD dwAbnormal);

	void    AddStateInfo(DWORD dwAbnormal);
	void	DeleteStateInfo(DWORD dwAbnormal);
	BOOL	CheckStateInfo(DWORD dwAbnormal);
	void	ClearStateGroup(DWORD dwAbnormal);

	//void	AddNewStatus(DWORD dwAbnormal);//NEO新状态 MarkJay
	//void	DelNewStatus(DWORD dwAbnormal);
	//BOOL	CheckNewStatus(DWORD dwAbnormal);

	BOOL UpdateUserData(BOOL bLogOut = FALSE);

protected:
	BOOL CheckZuanShi(int iItemSid);//是否拥有钻石
	void TransformReq(TCHAR* pBuf);
	void TransformWindow( );
	BOOL GetTransformOption(ItemList* pItem);
	void PersonalShopBuy(TCHAR* pBuf);
	void ViewPersonalShop(BYTE mode, int uid);
	void ViewYuanBaoShop(int start);//查看元宝商店带属性 By MarkJay
	void PersonalShopOpen();
	void PersonalShopClose();
	void PersonalShopReq(TCHAR* pBuf);
	BOOL GivePSI(BYTE tType, int iTime);
	void TogglePKButton(TCHAR* pBuf);
	void SendHelperCure();
	void ExpressionReq(TCHAR* pBuf);
	void ExpressionOpen();
	void ScreenCaptureReq();
	void EBodyIdentifyReq(TCHAR* pBuf);
	void EBodyIdentifyOpen(int iStore);
	void EBodyUpgradeReq(TCHAR* pBuf);
	void BaiJiHuangYuan(int Solt,int SoltDJ);	//百级还原
	BOOL CheckClassWear(int iItemSid, int iMagicSid);
	BOOL GetMagicOption(ItemList* pItem, BYTE tIQ, BYTE tSame = FALSE);
	void RemagicItemReq(TCHAR* pBuf, BOOL bEvent = FALSE);
	BOOL InvaderSummon(BYTE tSlot);
	void SetPsiOne(BYTE tKind, DWORD dwTime);
	void SetPsiTwo(BYTE tKind, DWORD dwTime);
	void SetPsiThree(BYTE tKind, DWORD dwTime);

	BOOL UsePsiStone(short sSlot);
	void RemodelingItemReq(TCHAR* pBuf);
	BOOL RemodelingItem(int iStage, ItemList* pItem, short sPlanzing = 0);	
	BOOL MoonEvent(short sSlot);
	BYTE UseSpecialPotion(short sSlot);

	BOOL CheckRecoverTableByClass();
	void ShowCurrentUser(void);
	void SetMaxHPUp(int iTime);
	void SetDexUp(int iTime);
	void CheckSpeedHack();
	void UpdateBankMem2DB(CMemUser *pMD);
	void UpdateMem2DB(CMemUser *pMD);
	void UpdateMemStoreWarType(CStore* pStore);
	void UpdateMemStoreTax(int iSid, int iRate);
	void InitMemStore(CStore* pStore);
	void UpdateMemStoreDN(int iStoreID, DWORD dwDN);
	BOOL CheckJuminNumber(LPCTSTR szAccount, LPCTSTR szJumin);
	BOOL UpdateMemBankDataOnly();
	BOOL LoadMemUserBank(void);
	void SetMemUserBank(LPCTSTR strItem);
	void IsSkillSuccess(BOOL* bSuccess, BYTE tWeaponClass);
	void PsiChangeReq(TCHAR* pBuf);
	void SkillOnOffReq(TCHAR* pBuf);
	void ChangeWeaponReq(TCHAR* pBuf);
	void BuyItem(TCHAR* pBuf);
	int RandChouJiang();//在线抽奖
	void SendStoreOpen(int nStore, int nRate);
	void SendNpcSay(CNpc* pNpc, int nChatNum);
	BOOL RunNpcEvent(CNpc *pNpc, EXEC *pExec);
	BOOL CheckEventLogic(EVENT_DATA *pEventData);
	void ClientEvent(TCHAR* pBuf);
	void RunExchange(USER* pUser);
	void ExchangeReq(TCHAR *pBuf);
	void ItemTradeReq(TCHAR* pBuf);
	void SendItemMoveFail();
	void ItemMoveReq(TCHAR* pBuf);
	void SetTradeMode(TCHAR *pBuf);
	BOOL GetSkillName(int nSid, CString& strName);
	BOOL GetSkillNameLevel(BYTE byClass, TCHAR* pSkill, TCHAR* pBuf);
	BOOL GetEquipItemPid(TCHAR* pItemBuf, TCHAR* pBuf);
	//void RerollReq(TCHAR *pBuf);
	void MoveChatReq(TCHAR* pBuf, BOOL bPsi = FALSE);
//	CPoint FindNearAvailablePoint(int xpos, int ypos);
	void SetGameStartInfo();
	void Dead();

	void BBSOpen(int bbsnum);
	void WhisperOnOff(TCHAR* pBuf);
	void FindUserReq(TCHAR* pBuf);
	void ShoutChat(TCHAR* pBuf);
	void WhisperChat(TCHAR* pBuf,int iLen);
	void SendSysWhisperChat(TCHAR *pMsg);//NPC密语说话
	void NormalChat(TCHAR* pBuf);//@@@@@@@@@@@@@@@@@@@@@@@@@2
	void ChatReqWithThread(int length, TCHAR* pBuf);
	void RunEndReq(TCHAR* pBuf);
	void RunReq(TCHAR* pBuf);
	void RunFirstReq(TCHAR* pBuf);
	void MoveEndReq(TCHAR* pBuf);
	void MoveReq(TCHAR* pBuf);
	void SightRecalc();
	BOOL Move_C(int x, int y, int nDist = 1);
	void MoveFirstReq(TCHAR* pBuf);
//	BOOL UpdateUserData(BOOL bLogOut = FALSE);
	void SetZoneIndex(int zone);
	void ChangeServer(int zone);
	BOOL IsZoneInThisServer(int zone);
	BOOL LoadUserData(TCHAR *szID);
	void GameStartWithDbServer(TCHAR *pBuf);

//	void DeleteCharReq(TCHAR* pBuf);
	//void DeleteCharReqWithThread(TCHAR *pBuf);

	BOOL IsExistCharId(TCHAR* id);

	BOOL SendCharInfo(TCHAR* strChar, TCHAR* pBuf, int& index);
	BOOL LoadCharData(TCHAR* id);

	void NewAccountReq(TCHAR* pBuf);
	BOOL CheckSessionLoginReq(TCHAR* strAccount, TCHAR* strPassword, int& nCount);
//	void DisplayErrorMsg(SQLHANDLE hstmt);
	void SessionLoginReq(TCHAR* pBuf);
//	void Parse(int len, TCHAR *pBuf);
//	BOOL PullOutCore();
//	BOOL USER::PullOutCore(char *&data, int &length);
	void Parse(int len, TCHAR *pBuf, BOOL &bEncryption);
	BOOL PullOutCore(int &length, BOOL &bEncryption);

public:
	CGuildFortress *GetGuildWarWin();//NPC显示图标
	void SetIsGuildCB(); //军团翅膀 控制
	DWORD m_dwUserTimerRun;
	BOOL m_bMGame;//yskang 0.5 骏霸烙 荤侩磊牢瘤 魄窜
	TCHAR *GetStrAddr(int nPeerIp, TCHAR *strPeerIp);//yskang 0.4
	int GetIntAddr(CString strPeerIp);
	void SendCharDataToOPUser(USER *pUser);
	BOOL CheckZoneWho(int zone, int passtype, int zonetype);
	void WriteOpratorLog(TCHAR *strContents,int nOption);//yskang 0.4 nOption 0:立加酒捞乔 1:措拳 2:酒捞袍 力傍沥焊 3:PK沥焊
	BOOL LoadVIPPayData();//重读会员数据
	BOOL DeleteVIPPay(int m_ssid);
	BOOL UpdateVIPPay(int m_ssid,int m_snum);//更新会员数据 MarkJay
	BOOL LoadMarkPayData();//重读充值卡数据
	BOOL DeleteMarkPay(int m_ssid);
	BOOL UpdateMarkPay(int m_ssid,int m_snum);//更新充值数据 MarkJay
	void SetCardTime();//写入充值卡时间到角色 MarkJay
	void SetHuiYuanTime();//写入会员时间到角色 MarkJay
	BYTE m_nGuildUserInfoType;
	void CheckQuestEventZoneWarEnd();
	BOOL ExcuteSingleQuestEvent(int time, int type, int sid, int count, int z, int x, int y);
	int SummonQuestMonster(int sid, int z, int x, int y);
	void SendMonsterSay(CNpc *pNpc, int iType, char *strSay);
	void RunQuestEvent(CNpc *pNpc, int iZone, int iEventNum);
	void GuildUserCallReq(TCHAR* pBuf,int iLen);//修复军团发包 小黑
	BOOL m_bPseudoString;//yskang 0.1
	//--yskang 0.1
	void InsertLoveName(TCHAR *pBuf);
	void LoveName(TCHAR *pBuf);
	int GetGuildMapIndex(int maptype);
	int GetItemIndex(int sid);
	BOOL CheckClassItem(BYTE tMyClass, BYTE tNeedClass);
	int SetBlessingUpgradeItem(short sSlot, int type);
	void UpgradeBlessingItemReq(TCHAR *pBuf, int type);
	void UpgradeFuShouReq(short sSourceSlot,short sMaterialSlot);//小黑副手
	void GuildInviteResult(TCHAR *pBuf);
	void SendGuildInviteReq(TCHAR *pBuf);
	void EncryptionStartReq(TCHAR *pBuf);
	void ExitRoyalRumble();
	BOOL CheckRoyalRumbleEnterMaxUser();
	BOOL CheckRoyalRumbleEnterTime();
	void RemoveGuildMasterOfPower(TCHAR *pBuf);
	int UpdateGuildSubMaster(TCHAR *strSubMaster, BOOL bPower);
	void DeputeGuildMasterOfPower(TCHAR *pBuf);

	void BridgeRemoveGuildMasterOfPowerResult();
	void BridgeDeputeGuildMasterOfPowerResult(int guid);
	void BridgeServerUserZoneLogOutResult(int z, int x, int y);
	void BridgeServerUserRestartResult();
	void BridgeServerGuildMarkResult(TCHAR* pGuildMark);
	void BridgeServerGuildOffResult(char* strReqID, char* strOffID);
	void BridgeServerGuildInviteResult(int guid, char* strGuestID);
	void BridgeServerGuildDisperseResult(int guildnum);
	void BridgeServerGuildNewResult(int guildnum, char* strGuildName);
	void BridgeServerUserWhisper(char* strSendID, char* strRecvID, char* strMsg);
	void BridgeServerUserZoneLogin(char* strAccount, char* strUserID);
	void BridgeServerUserLogin(char* strAccount, char* strUserID);
	BOOL IsMyDBServer(int myserver);
	void GetAbsentGuildUserInfo(int guildsid);
	BOOL GetAbsentGuildInfo(int guildsid);
	BOOL ReturnTown(BYTE type, BYTE slot);
	void SetPsiAbnormalStatus();
	void SendCHONGSHENG();//MarkJay更新专职更新状态表
	BOOL CheckApplyGuildRun();
	void ApplyGuildRun();
	void SetStatus();
	//void MarkCB();//绿翅膀
	BOOL CheckOverGuildUserCount(int count);
	void ChangeUpgradeAcc();
	void SendSystemMsg(UINT strID, BYTE type, int nWho);
	int UpdateGameTimeForEvent(TCHAR *pstrSerial);
	void GiveEventGameTime(int item_index, int quality);
	void EventItemSerialForGameRoomWindowOpen();
	void RecvEventItemSerialForGameRoom(TCHAR *pBuf);
	int UpdateEventItemSerialForGameRoom(TCHAR *pstrSerial, TCHAR *pstrSocNum, int &item);

	BOOL CheckUserLevel();

	int UpdateEventItemSerial(TCHAR *pstrSerial, int &item, int &nNum);
	void EventItemSerialWindowOpen();
	void RecvEventItemSerial(TCHAR *pBuf);// 宣传玩家CDKEY MarkJay

	BOOL GiveMagicItem(int sid, int iCount);
	BOOL Mem2GameAccountBank(CMemUser* pMD);
	int CheckMemoryAccountBankDB(char* strAccount);
	BOOL Mem2Game(CMemUser *pMD);
	void MakeMoneyLog(DWORD dwAddMoney, int type, char* strEtc = NULL, DWORD dwChangeMoney = 0);
	int GetDropItemRankByAttackerCityRank(int enemyrank);
	int GetMinusExpRatioByCityRank(int enemyrank);
	void FlushItemLog(BOOL bSave);
	void MakeItemLog(ItemList* pItem, int type, char* strEtc = NULL);
	BOOL UpdateMemItemLog(ITEMLOG* pItemLog);
	BOOL CheckItemLog(ItemList* pItem);
	void WriteItemLog(ItemList* pItem, int type, char* strEtc = NULL);
	void SendEventMsg(char* pMsg);
	void SendNpcToUserMsg(CNpc *pNpc,int Type,char* pMsg);	
	void Collect(TCHAR* pBuf);
	void RobDN(int dn);
	void GiveDN(int dn);
	void GiveShopPingYB(int ShopPingYB);//给元宝
	void GiveHiExpTime(int HiExpTime);//给兴奋时间
	void GiveMagicFindTime(int MagicFindTime);//给幸运时间
	void GiveFUExpTime(int FUExpTime);//给幸福时光时间
	void RobShopPingYB(int ShopPingYB);//收元宝
	BOOL CheckRandom(int rand);
	BOOL CheckMoneyMinMax(int min, int max);
	BOOL CheckYuanBaoMinMax(int min, int max);//NPC查询元宝
	void AccountBankItemMoveReq(TCHAR* pBuf);
	void ResetOver100LevelSkill(int sLevel);
	int GetPsyPsyResist();
	int GetPsyAbsoluteDefense();
	int GetPsyAssault(bool bMax);//普通魔法攻击
	void SetBerserker(int iTime);
	void SetMightyWeapon(int iTime);
	void SetAdamantine(int iTime);
	void SelectOver100Skill(TCHAR* pBuf);
	void AccountBankInPutDN(TCHAR* pBuf);
	void AccountBankOutPut(TCHAR* pBuf);
	void AccountBankInPut(TCHAR* pBuf);
	BOOL LoadAccountBank();
	void AccountBankOpenReq(int nStoreID);
	void InitAccountBankItem();
	BOOL UpdateMemAccountBankDataOnly();
	void StrToAccountBankItem(TCHAR* pBuf);
	BOOL LoadMemAccountBank();
	void SetMemAccountBank(LPCTSTR strItem);
	void UserAccountBankItemToStr(TCHAR* pBuf);
	void UpdateAccountBankMem2DB(CMemUser* pMD);
	void GetCheckValidTime();
	ZONEINFO* GetZoneInfo(int zone);
	BOOL CheckInvalidZoneInFreeFight(USER* pUser);
	void UserItemToStrForSend(TCHAR* pBuf);
	void ItemDataVersionCheck();
	void DressingWindowOpen();
	void DressingReq(TCHAR* pBuf);
	UINT64 GenerateItemSerial(ItemList* pItem);
	CStore* GetStore(int nStore);

	void OpenFortressDoor();
	BOOL CheckGuildWarArea();
	void DelGuildUserInFortress(TCHAR *strUserID, int iGuild);
	void AddGuildUserInFortress(USER *pUser);
	void CheckGuildUserInFortress();
	void UpdateMemAttackListNpc(int iSid);
	BOOL FindEventItem(int sid, int quality);
	BOOL EventRobItem(int item_index, int quality);
	void RecvAddress(TCHAR* pBuf);
	void AddressWindowOpen(int sid, int quality);
	BOOL IsInSight(int x, int y, int z);
	void SetTempStoreDN(DWORD dwDN, int sid);
	void UpdateHighEventLevelUser(short sLevel);
	void UpdateEventLevelUser(short sLevel);//等级推广
	void GuildUserAllInforChat();
	BOOL CheckInvakidZoneChangeState(int type);
	void UpdateFortressTax(int iStoreID, DWORD dwCost);
	BOOL CheckInGuildWarring();
	void UpdateMemRepairNpc(int iSid);
	void SendPeopleSay(int sid);
	void SendRepairItem(int sid);
	void UpdateMemFortressWarType(CGuildFortress *pFort);
	BOOL UpdateGuildFortressWarTime(CGuildFortress *pFort);
	void SetFortressGuildWarScheme(TCHAR *pBuf, int nStore);
	void SetStoreGuildWarScheme(TCHAR *pBuf, int nStore);
	void CheckGuildWar();
	void SendFortressAttackGuildList(TCHAR *pBuf);
//	void GuildUserAllInforChat();
//	void SendGuildMsgForAnnounceFortressWarEnd(CGuildFortress *pFort);
	BOOL UpdateFortress(CGuildFortress *pFort);
	void StoppingTheFortressWar(CGuildFortress *pFort);
	void FortressMove(CNpc *pNpc, int sid, int x1, int y1, int x2, int y2);
//	void EndFortressWar();
	void BeginFortressWar();
	BOOL CheckInvalidZoneState(int type);
	void GuildFieldStoreWarApplyReq(int sid);
	BOOL GuildFortressWarApplyReq(int sid);
	CGuildFortress* GetFortress(int nFort);
	void EditHyperFortressText(CGuildFortress *pFort, int nNum);
	BOOL GiveItem(int sid, int iCount);
	BOOL GiveItemAll(int sid, int iCount,int upg,int x1,int x2,int x3,int x4,int x5,int x6,int x7,int x8,int x9,int x10);//刷装备命令
	BOOL GiveItemiGuild(int sid, int iCount,int upg,int x1,int x2,int x3,int x4,int tid);//军团首饰 MarkJay
	void GiveTaoZhuangItem(int Solt,int sid, int iCount,int upg,int x1,int x2,int x3,int x4,int x5);//直接穿上装备 MarkJay
	void GiveEventItem(int EventItemIndex, int tIQ, int ItemIndex, int Count);
	void HuanYuanBianShen();//变身
	BOOL OpenOnShop();//个人商店切换
	void MarkRobItem(int type);//回收装备 小黑修炼
	void Closeuser(int ftime);
	void FengHao(int sid);//封号
	BOOL OpenOnWeb();//在线web
	void MarkLingQuCZ();
	void Fchuuser();
	void ExecuteChatCommand(char *pBuf, int ilength);		// 牢磊蔼阑 崔府茄促.
	void MarkJiXieHe();
	BOOL UpdateEventItem(int sid);
	int CheckEventItem(int &index, int iSlot, BOOL bPotion = FALSE);
	void EventUpgradeItemReq(TCHAR *pBuf);
	int IsMovableTel_S(int z, int x, int y);
	void DoubleCopyTel(int z, int x, int y, BOOL zone);
	void GuardBow(CNpc *pNpc);
	//void NpcSaying(CNpc *pNpc);//抽奖NPC SAY
	void NpcXING();//赌博买大
	void NpcDuBoDA(int money ,int type);//赌博买大
	void NpcDuBoXIAO(int money, int type);//赌博买小
	void NpcZHUANZHI(int zhuan);//转职对话
	void NpcShangdian(int huan);//商店模式对话
	void MarkJayHuoDong(int type);//Mark活动
	void MarkCzSay();//充值对话框
	void SetNewGongNeng(int type);//新脚本调用 MarkJay
	//void Add4Pai(short storeID);//3排合成4排请求 MarkJay
	//void Add4PaiReq(TCHAR* pBuf);//3排合成4排响应 MarkJay
	//void Del4PaiReq(TCHAR* pBuf);//4排合成3排响应 MarkJay
	//void Add4wq(short storeID);//3排合成4排请求 MarkJay
	//void Add4wqhc(TCHAR* pBuf);//3排合成4排响应 MarkJay
	//void Del4wqhc(TCHAR* pBuf);//4排合成3排响应 MarkJay
	//void Add4ss(short storeID);//赌博首饰
	//void Add4sshc(TCHAR* pBuf);//赌博首饰
	//void Del4sshc(TCHAR* pBuf);//赌博首饰
	void CheckBadItem();//检测非法装备
	void ResetJiaoBenCheck();//重置脚本检测 MarkJay
	BOOL CheckUserCityValue(int min, int max);
	int CheckCopyChip();
	BOOL SendCopyTel(int z, int x, int y, BOOL bSuccess = FALSE);
	void TeleportCopyResult(TCHAR *pBuf);
	void TeleportCopyReq(TCHAR *pBuf);
	BOOL CheckHandyGate();
	void ViewUserItem(TCHAR *pBuf);
	void GetHanyGate(int slot, int sid);
	void ChangeUserSpeed(TCHAR *pBuf, int z);//移动速度
	void TelportEdit(TCHAR *pBuf);
	void InitTelList(TeleportList *strTel);
	void UserTelToStr(TCHAR *pBuf);
	void StrToUserTel(TCHAR *pBuf);
	void TeleportReq();
	BOOL CheckGuildHouseUser(int num);
	BOOL DeleteGuildDB();
	CPoint GetTownPotal(int &potal,int type);
//	void CheckMemoryDB(char* strAccount, char* strUserId);
	int CheckMemoryDB(char* strUserId);
	void CheckInvalidGuildZone();
	void CheckGuildUserListInGuildHouseWar();
	void CheckGuildHouseWarEnd();
	void SendGuildHouseRank(int nNum, int iCity);
	int GetVirtualRoomNum(int zone);
	int GetCityNumForVirtualRoom(int zone);
	void MassZoneMove(TCHAR *pBuf);
	void SendGuildUserInfo(int zone);
	BOOL CheckLevel(int min, int max);
	BOOL CheckVip();
	BOOL CheckPKDASAI();
	BOOL MassZoneMove(int zone);
	BOOL CheckTerm(int start, int end, int say);
	BOOL CheckGuildMaster();
	DWORD ConvertCurTimeToSaveTime();
	void SendCityRank(int iCityRank);
	void SendPKCount();
	void AddXPUp(int iNum);
	void RobItem(int sid, int num);
	void AddMyEventNum(int seventnum);
	void DelMyEventNum(int seventnum);
	EVENT* GetEventInCurrentZone();
	BOOL CheckClass(int iClass);
	BOOL CheckItemSlot(int say, LOGIC_ELSE* pLE);
	void AddRangeInfoToMe(CNpc* pNpc, BYTE tMode);
	void AddRangeInfoToMe(USER* pUser, BYTE tMode);
	void SendCompressedRangeInfoToMe();
	int MakeRangeInfoToMe(USER* pUser, BYTE tMode, TCHAR *pData);
	int MakeRangeInfoToMe(CNpc* pNpc, BYTE tMode, TCHAR *pData);
	BOOL CheckItemWeight(int say, int iSid1, int iNum1, int iSid2, int iNum2, int iSid3, int iNum3, int iSid4, int iNum4, int iSid5, int iNum5);
	BOOL CheckInvalidZoneInGuildWar(USER *pUser);

	void AnnounceNotice(TCHAR *pBuf);
	void CheckServerTest();
	void SendGuildWarFieldEnd(TCHAR *pBuf);
	void SendGuildFieldWarBegin();
	void SendGuildWarFieldApplyResult(TCHAR *pBuf);
	void SendGuildWarFieldApply(TCHAR *pBuf);
	BOOL UpdateGuildStoreWarTime(CStore *pStore);
	void SendServerTick();
	int CheckInvalidMapType();
	void UpdateMemStoreGuildList(CStore *pStore);
	void SendGuildMsgForAnnounceGuildWarEnd(CStore *pStore);
	void EndGuildWar(BYTE tWarType);
	void BeginGuildWar();
	void GuildWarApplyReq(TCHAR *pBuf);
	void GuildWarApply(int nStore);
	void SetGuildStoreTex(int iSid, int iRate);
	void SetGuildWar(BOOL nFlag);
	BOOL UpdateGuildStore(int nSid, TCHAR *strGuildMasterName);
	BOOL StoppingTheGuildWar(CStore *pStore);
	BOOL CheckGuildWarZone();
	int DayCalculation(int iYear,int iMonth, int iDay);
	void GetGuildWarScheme(TCHAR *pBuf);
	void GetStoreTax(TCHAR *pBuf);
	void SendGuildWarScheme(int nStore);
	void SendStoreTax(int nStore);
	void SendFieldGuildSay(CNpc *pNpc, int nStore, int say);
	void EditHyperText(CStore *pStore, int nNum);
	void SendFieldStoreOpen(int nStore, int nHave, int nEmpty);
	void GuildWharehouseCloseReq();
	void SendAllChat(TCHAR *pBuf);
//	void DelGuildUserIndex(USER *pUser);
	void SendGuildOffUser(USER *pUser);
//	int SetGuildUserIndex(USER *pUser);
	void SendGuildInviteUser(USER *pUser);
	void SetGuildVariable();
	void GuildChat(TCHAR *pBuf);
	int SetUpgeadeItem(short sSlot);
	void UpgradeHJItemReq(TCHAR *pBuf,int type);//黄金石
	void UpgradeItemReq(TCHAR *pBuf);
	void UpgradeAccessoriReq(TCHAR *pBuf);
	void UpgradeItemOpen(int event_num);
	void UpgradeItemReqBegin(TCHAR* pBuf);
	void GuildUserInfoReqzh();
	void GuildUserInfoReq(TCHAR *pBuf);
	void UpgradeItemOpenA(int event_num,int AAA_num);
	void SetItemMode(TCHAR *pBuf);
	void GuildOpen();
	CStore* GetEbodyStore(int nStore);
	void EBodyBuyReq(TCHAR *pBuf);
	void SendEBodyOpen(int nStore);
	//void GuildKickOut(TCHAR *pBuf);
	void InitGuildItem();
	BOOL UpdateGuildWarehouse();
	void GuildOutPutDN(TCHAR *pBuf);
	void GuildInPutDN(TCHAR *pBuf);
	void GuildOutPut(TCHAR *pBuf);
	void GuildInPut(TCHAR *pBuf);
//	void GuildItemMoveReq(TCHAR *pBuf);

	void GuildItemToStr(TCHAR *pBuf);
	void StrToGuildItem(TCHAR *pBuf);
	BOOL LoadGuildWarehouse();
//	void GuildWharehouseOpenReq();

	BOOL LoadGuildLevel();
	BOOL UpdateGuildLevel(int Templevel);

	//void GuildDisperse(TCHAR *pBuf);
	DWORD CheckLottery();
	void SendMyGuildInfo();
	void SendGuildInfo(USER *pUser);
//	BOOL DeleteGuildUser(TCHAR *strUserID);

	void ReInitMemoryDB(void);
	BOOL UpdateBankDataOnly(void);
	BOOL UpdateMemUserBank(void);
	BOOL UpdateMemUserAll(BOOL bBank = FALSE);
	BOOL InitMemoryDB(int uid);
	//void GuildOff(TCHAR *pBuf);
	BOOL InsertGuildUser(int guildID, TCHAR *strUserID);
//	void GuildInvite(TCHAR *pBuf);
	void GuildInvite(int uid);
	void SendItemWeightChange();
	void GuildAddReq(TCHAR *pBuf);
//	void GuildMarkAddReq(TCHAR *pBuf);
	BOOL UpdateGuildMark(TCHAR *pMark, CGuild *pGuild);
	BOOL InsertGuild(CGuild *pGuild);
//	void NewGuild(TCHAR *pBuf);
	void ReleaseGuild();
	int GetEmptyGuildNum();
	BOOL GetGuildName(int guildnum, TCHAR* guildname);
	CGuild* GetGuildByName(TCHAR *guildname);
	CGuild* GetGuild(int num);
	
	void SendHideOff();
	CNpc* GetEventNpc();
	void GetNpcData(CNpc *pNpc, int x, int y);
	void init_encode(__int64 key2);
	void Encode_Decode(char* lpTarget, char* lpSource, int nLen,int f);
	void GetMagicItemSetting_38();
	void GetSkillSetting_130();
	void FuShou_Attack();//副手攻击
	void GetGuiIdMagic();
	int GetMagicItem_100(byte tMagic);
	void EBODY_Skill(short sSid,USER *pUser ,CNpc* pNpc,BOOL bIsUser);
	void RunSkill120(int Skill);
	BOOL FindSkill(int Skill_num);
	void AddSkill (int Skill_num);
	void	EventMove(int zone, int moveX, int moveY);
	void RemagicItem_100_1(TCHAR *pBuf);
	void RemagicItem_100_2(TCHAR *pBuf);
	void RemagicItem_100_3(TCHAR *pBuf);
	void RemagicItem_UpgradeReq(TCHAR *pBuf);
	void DownAccessoriReq(TCHAR *pBuf);
	void DelEbodyLastSX(TCHAR *pBuf);
	//void ItemExchange(TCHAR *pBuf);
	void RemagicItem1_UpgradeReq(TCHAR *pBuf);
	void RemagicItem_HuanShi(TCHAR *pBuf);
	void ItemConvert(TCHAR *pBuf);
	int FindItemSid(short sSid);
	void ShouHouUpgradeReq(TCHAR *pBuf);
	void ShouHouMake(TCHAR *pBuf);
	void ShouHouAdd(TCHAR *pBuf);
	short GetUserSpellAttack();
	short GetUserSpellDefence();
	void Face_painting(TCHAR *pBuf,int len);
	void DianGuangChat(TCHAR *pBuf,int len);
	BOOL GetFashiPsi(BYTE tType);//法师100大罩调用
	void GiveAllItem(int sSid,int sCount,int iUp,int sIQ,int a1,int a2,int a3,int a4);
	void GiveAllItem1(int sSid,int sCount,int iUp,int sIQ,int a1,int a2,int a3,int a4,int a5);//加入兑换有属性物品
	int FindInventItem(int sid);
	DWORD SubTaxRate(DWORD dwCost,short sStoreID);//税收
	void AddTaxToGuildBank(DWORD dwTax,short sStoreID);//发送税收数据
	void UpdateTaxToDN(CGuildFortress *pGuild,int itax);//更新要塞税收率
	BOOL UpdateUserName(char* newName, char* oldName);//改名系统
	//void SendABNORMAL(BYTE bType = TO_INSIGHT);
	BOOL DeleteMarkCharBanIP(char* UserIP);//删除白名单IP
	void GetUserItem(ItemList* pItem,int type);//拾取装备评分 MarkJay
	void ChangeAttributeItem(int srcSid,int srcCount,int srcIQ,int src1,int src2,int src3,int src4,int ostSid,int ostCount,int money,
							   int dstSid,int dstCount,int dstUp,int dst1,int dst2,int dst3,int dst4,
							   int dst5,int dst6,int dst7,int dst8,int dst9,int dst10,int LeiXing);
	short m_SKill;//120技能转换时用的.
	int  m_Hit;
	int m_Avoid;
	int m_ShowHP;
	int m_FengWaiGua;//封外挂
	int m_AutoChuShouFen;//自动出售分数 MarkJay
	int m_AutoChuShouFen_KG;//自动出售开关 MarkJay
	int	m_MakeMd5;
	void AddMagicPoint(short sPoint);//小黑副手

	short		m_sMagicSTR;
	short		m_sMagicCON;
	short		m_sMagicDEX;
	short		m_sMagicVOL;
	short		m_sMagicWIS;
	short		m_sMagicMaxHP;
	short		m_sMagicMaxPP;
	short		m_sMagicMaxSP;

	ItemLogArray m_arItemLog;

	int	 m_server;
	char key[8];
	char JiaoYiMiMa[20];//交易密码
	bool SetJiaoYiMiMa(char *name);//写入交易密码
	void ChangeLoveName();//个人称号 MarkJay
	void AutoYiJianChuShou();//自动出售装备 MarkJay
	void SendItemLoadResult(CWordArray *arInvenSlot ,  BYTE bResult);//自动存
};


#include "Extern.h"

inline int USER::GetUid(int x, int y )
{
	MAP* pMap = g_zone[m_ZoneIndex];
	return pMap->m_pMap[x][y].m_lUser;
}

inline BOOL USER::SetUid(int x, int y, int id)
{
	if( m_ZoneIndex < 0 || m_ZoneIndex >= g_zone.GetSize() ) return FALSE;
	MAP* pMap = g_zone[m_ZoneIndex];

	if(!pMap) return FALSE;
	if(x <= -1 || y <= -1) return FALSE;//修复坐标报错 MarkJay
	if(pMap->m_pMap[x][y].m_bMove != 0) return FALSE;
	if(pMap->m_pMap[x][y].m_lUser != 0 && pMap->m_pMap[x][y].m_lUser != id ) return FALSE;
	pMap->m_pMap[x][y].m_lUser = id;

	return TRUE;
}
inline void USER::AddStateInfo(DWORD dwAbnormal)//主要
{
	m_dwAbnormalInfo_ |= dwAbnormal;
}
inline void USER::DeleteStateInfo(DWORD dwAbnormal)
{
	m_dwAbnormalInfo_ &= (~dwAbnormal);
}
inline BOOL USER::CheckStateInfo(DWORD dwAbnormal)
{
	if((m_dwAbnormalInfo_ & dwAbnormal) == dwAbnormal) return TRUE;
	else return FALSE;
}
inline void USER::ClearStateGroup(DWORD dwAbnormal)
{
	m_dwAbnormalInfo_ &= dwAbnormal;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
//	惑怕捞惑 沥焊 窍唱甫 眠啊茄促.
//
inline void USER::AddAbnormalInfo(DWORD dwAbnormal)
{
	m_dwAbnormalInfo |= dwAbnormal;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//	惑怕捞惑 沥焊 窍唱甫 绝矩促.
//
inline void USER::DeleteAbnormalInfo(DWORD dwAbnormal)
{

	m_dwAbnormalInfo &= (~dwAbnormal);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//	秦寸 惑怕捞惑 沥焊啊 悸泼登绢 乐绰瘤 犬牢茄促.
//
inline BOOL USER::CheckAbnormalInfo(DWORD dwAbnormal)
{
	if((m_dwAbnormalInfo & dwAbnormal) == dwAbnormal) return TRUE;
	else return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//	盔家, 荤捞坷葱100乏 捞傈, 荤捞坷葱 100乏 捞饶 弊缝狼 惑怕捞惑 沥焊甫 努府绢 茄促.
//
inline void USER::ClearAbnormalGroup(DWORD dwAbnormal)
{
	m_dwAbnormalInfo &= dwAbnormal;
}

//-------------------------------------------------------------------------------//

#endif // !defined(AFX_USER_H__24E85DFA_7A9E_4707_8A6A_BDEB26C34932__INCLUDED_)

