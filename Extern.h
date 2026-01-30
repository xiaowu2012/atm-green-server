#ifndef _EXTERN_H_
#define _EXTERN_H_

#include "ErrorLog.h"
#include "MarkIni.h"//ini文件
#include "MarkGMIni.h"//ini文件
#include "SERVERINFO.h"
#include "MAP.h"
#include "CELL.h"
#include "EVENT.h"
#include "Npc.h"
#include "NpcTable.h"
#include "NpcItem.h"
#include "NpcThread.h"
#include "ItemTable.h"
#include "SkillTable.h"
#include "PsiTable.h"
#include "LevelUpTable.h"
#include "PAMAExp.h"
#include "TableItem.h"	
#include "NpcChat.h"
#include "Store.h"
#include "InitItemTableSet.h"
#include "MagicItemTable.h"
#include "MyDB.h"
#include "Recover.h"
#include "RecoverRate.h"
#include "DNTable.h"
#include "Guild.h"
#include "GuildUser.h"
#include "GuildStoreTable.h"
#include "EBodyTable.h"
#include "SharedMemory.h"
#include "GuildMapInfoTable.h"
#include "GuildHouseWar.h"
#include "VirtualRoom.h"
#include "GuildHouse.h"
#include "UserLevelUpTable.h"
#include "EventItemTable.h" 
#include "EventItemNew.h" 
#include "NpcSayTable.h"
#include "GuildFortress.h"
#include "MonsterPsiSet.h"
#include "DressingSet.h"
#include "AccessoriUpTable.h"
#include "EventBlockingTable.h"
#include "RemodelingTable.h"
#include "BoxEventTable.h"
#include "SummonTable.h"
#include "ServerDlg.h"
#include "EBodyUpgradeTable.h"
#include "EBodyIdentifyTable.h"
#include "OnlineShop.h"//在线商店
#include "VIPPay.h"//会员充值
#include "MarkPay.h"//在线充值
#include "MarkGonggao.h"//在线公告
#include "MarkChouJiang.h"//在线抽奖

typedef CTypedPtrArray <CPtrArray, MarkIni*>		Markvvini;//ini文件
typedef CTypedPtrArray <CPtrArray, MarkGMIni*>		MarkvvGMini;//ini文件
typedef CTypedPtrArray <CPtrArray, SERVERINFO*>		ServerArray;
typedef CTypedPtrArray <CPtrArray, ZONEINFO*>		TownPotalArray;
typedef CTypedPtrArray <CPtrArray, TOWNPOTAL*>		RandomTownPotalArray;
typedef CTypedPtrArray <CPtrArray, MAP*>			ZoneArray;
typedef	CTypedPtrArray <CPtrArray, CELL*>			CellArray;
typedef CTypedPtrArray <CPtrArray, EVENT*>			EventArray;
typedef CTypedPtrArray <CPtrArray, CNpc*>			NpcArray;
typedef CTypedPtrArray <CPtrArray, CNpcTable*>		NpcTableArray;
typedef CTypedPtrArray <CPtrArray, CNpcThread*>		NpcThreadArray;
typedef CTypedPtrArray <CPtrArray, TableItem*>		TableItemArray;
typedef CTypedPtrArray <CPtrArray, CItemTable*>		ItemTableArray;
typedef CTypedPtrArray <CPtrArray, CMagicItemTable*>MagicItemTableArray;

typedef CTypedPtrArray <CPtrArray, CAccessoriUpTable*>	AccessoriUpTableArray;
typedef CTypedPtrArray <CPtrArray, CRemodelingTable*>	RemodelingTableArray;

typedef CTypedPtrArray <CPtrArray, CSkillTable*>	SkillTableArray;
typedef CTypedPtrArray <CPtrArray, CPsiTable*>		PsiTableArray;
typedef CTypedPtrArray <CPtrArray, CLevelUpTable*>	LevelUpTableArray;
typedef CTypedPtrArray <CPtrArray, CPAMAExp*>		PAMAExpArray;
typedef CTypedPtrArray <CPtrArray, CNpcChat*>		NpcChatArray;
typedef CTypedPtrArray <CPtrArray, CStore*>			StoreArray;

typedef CTypedPtrArray <CPtrArray, WEATHER_INFO*>		WeatherArray;

typedef CTypedPtrArray <CPtrArray, InitItemTable*>		InitItemTableArray;

typedef CTypedPtrArray <CPtrArray, CRecover*>		RecoverArray;
typedef CTypedPtrArray <CPtrArray, CRecoverRate*>	RecoverRateArray;

typedef CTypedPtrArray <CPtrArray, CDNTable*>		DNArray;
typedef CTypedPtrArray <CPtrArray, CGuild*>			GuildArray;
typedef CTypedPtrArray <CPtrArray, CGuildUser*>		GuildUserArray;
typedef CTypedPtrArray <CPtrArray, CGuildStoreTable*>GuildStoreArray;
typedef CTypedPtrArray <CPtrArray, CGuildHouseWar*>	GuildHouseWarArray;

typedef CTypedPtrArray <CPtrArray, CGuildMapInfoTable*>	GuildMapArray;

typedef CTypedPtrArray <CPtrArray, CSharedMemory*>	SharedMemoryArray;

typedef CTypedPtrArray <CPtrArray, CVirtualRoom*>	VirtualRoomArray;
typedef CTypedPtrArray <CPtrArray, CGuildHouse*>	GuildHouseArray;
typedef CTypedPtrArray <CPtrArray, CUserLevelUpTable*>	UserLevelArray;

typedef CTypedPtrArray <CPtrArray, CEventItemTable*> EventItemArray;
typedef CTypedPtrArray <CPtrArray, CEventItemNew*> EventItemNewArray;

typedef CTypedPtrArray <CPtrArray, CNpcSayTable*> NpcSayArray;

typedef CTypedPtrArray <CPtrArray, CGuildFortress*> GuildFortressArray;

typedef CTypedPtrArray <CPtrArray, DynamicMapList*> DynamicMapArray;

typedef CTypedPtrArray <CPtrArray, CMonsterPsi*> MonsterPsiArray;
typedef CTypedPtrArray <CPtrArray, DRESSING_DATA*> DressingDataArray;

typedef CTypedPtrArray <CPtrArray, int*> MyServerArray;

typedef CTypedPtrArray <CPtrArray, CBoxEventTable*> BoxEventTableArray;
typedef CTypedPtrArray <CPtrArray, CSummonTable*> SummonTableArray;

typedef CTypedPtrArray <CPtrArray, CEBodyTable*>			EBodyArray;
typedef CTypedPtrArray <CPtrArray, CEBodyUpgradeTable*>		EBodyUpgradeTableArray;
typedef CTypedPtrArray <CPtrArray, CEBodyIdentifyTable*>	EBodyIdentifyTableArray;
typedef CTypedPtrArray <CPtrArray, OnlineShop*>				OnlineShopArray;//在线商店
typedef CTypedPtrArray <CPtrArray, VIPPay*>					VIPPayArray;//会员充值
typedef CTypedPtrArray <CPtrArray, MarkPay*>				MarkPayArray;//在线充值
typedef CTypedPtrArray <CPtrArray, MarkGonggao*>			MarkGonggaoArray;//在线公告
typedef CTypedPtrArray <CPtrArray, MarkChouJiang*>			MarkChouJiangArray;//在线抽奖

extern GuildArray			g_arGuildData;
extern GuildUserArray		g_arGuildUser;
extern GuildHouseWarArray	g_arGuildHouseWar;
extern GuildHouseArray		g_arGuildHouse;
extern GuildFortressArray	g_arGuildFortress;
//extern GuildStoreArray		g_arGuildStore;	

extern InitItemTableArray		g_arInitItem;

extern TableItemArray	g_DBItemArray;
		
extern CMyDB				g_DB[];
extern CMyDB				g_DBSession[];
extern CMyDB				g_DBNew[];

extern CErrorLog			g_ErrorLog;
extern HANDLE				g_hIOCP;
extern ZoneArray			g_zone;
// alisia
extern ZoneArray			g_zonesize;
//
extern CellArray			g_cell;
extern EventArray			g_event;
extern Markvvini			f_Markini;
extern MarkvvGMini			GM_Markini;

extern ServerArray			g_server;
extern NpcArray				g_arNpc;
extern NpcTableArray		g_arNpcTable;
extern NpcTableArray		g_arCityNpcTable;
extern NpcThreadArray		g_arNpcThread;
extern CNpcItem				g_NpcItem;
extern CNpcItem				g_ValItem;

extern BOOL					g_bNpcExit;
extern BOOL					g_isBaoZangKG;//禁止PK洞开关

extern BOOL					GuildWarEnd;//要塞结束 MarkJay

extern ItemTableArray		g_arItemTable;
extern MagicItemTableArray	g_arMagicItemTable;

extern AccessoriUpTableArray	g_arAccessoriUpTable;		// Accessori Upgrade Table
extern CRipel				g_Ripel;						// Ripel's crest

extern RemodelingTableArray	g_arRemodelingTable1;			// 1 窜 俺炼 Table
extern RemodelingTableArray	g_arRemodelingTable2;			// 2 窜 俺炼 Table
extern RemodelingTableArray	g_arRemodelingTable3;			// 3 窜 俺炼 Table

extern CWordArray			g_arRemodelingRandomIndex1;		// 1 窜 俺炼 犬啦 牢郸胶
extern int					g_iRemodelingArmorIndex1;		// 1 窜 俺炼 规绢备 矫累 牢郸胶
extern CWordArray			g_arRemodelingRandomIndex2;		// 2 窜 俺炼 犬啦 牢郸胶
extern int					g_iRemodelingArmorIndex2;		// 2 窜 俺炼 规绢备 矫累 牢郸胶
extern CWordArray			g_arRemodelingRandomIndex3;		// 3 窜 俺炼 犬啦 牢郸胶
extern int					g_iRemodelingArmorIndex3;		// 3 窜 俺炼 规绢备 矫累 牢郸胶

extern BoxEventTableArray	g_arBoxEventTable;				// Box Event Table Array
extern SummonTableArray		g_arSummonTable;				// Summon Monster Table
extern CServerDlg*			g_pMainDlg;

extern SkillTableArray		g_arSkillTable;
extern PsiTableArray		g_arPsiTable;

extern LevelUpTableArray	g_arLevelUpTable;
extern PAMAExpArray			g_arPAMAExp;

extern StoreArray			g_arStore;
extern StoreArray			g_arPsiStore;

extern NpcChatArray			g_arNpcChat;

extern RecoverArray			g_arRecoverTable;
extern RecoverRateArray		g_arRecoverRateTable;

extern DNArray				g_arDNTable;

extern StoreArray				g_arEBodyStore;
extern EBodyArray				g_arEBodyTable;
extern EBodyUpgradeTableArray	g_arEBodyUpgradeTable;
extern EBodyIdentifyTableArray	g_arEBodyIdentifyTable;
extern OnlineShopArray			g_arOnlineShopTable;//在线商店
extern VIPPayArray			    g_arVIPPayTable;//会员充值
extern MarkPayArray			    g_arMarkPayTable;//在线充值
extern MarkGonggaoArray			g_arMarkGonggaoTable;//在线公告
extern MarkChouJiangArray		g_arMarkChouJiangTable;//在线抽奖

extern GuildMapArray		g_arMapTable;

extern VirtualRoomArray		g_arVirtualRoom;

extern UserLevelArray		g_arUserLevel;

extern EventItemArray		g_arEventItemTable;
extern EventItemArray		g_arAddEventItemTable;
extern EventItemNewArray	g_arEventItemNew;

extern NpcSayArray			g_arNpcSayEventTable;

extern DynamicMapArray		g_arDynamicMapTable;

extern MonsterPsiArray		g_arMonsterPsi;
extern DressingDataArray	g_arDressingData;

extern MyServerArray		g_arMyServer;

extern BOOL	g_bDebug;

extern int					g_ItemSerialIndex;

extern int g_8x[];
extern int g_8y[];
extern int g_16x[];
extern int g_16y[];

extern POINT g_rPt[8][6];

extern int g_BrawlClass[];					// 秦寸 拌凯俊辑 荤侩且荐乐绰 公扁拌凯 辆幅(酒捞袍 抛捞喉 辆幅)
extern int g_StaffClass[];
extern int g_EdgedClass[];
extern int g_FireArmsClass[];

extern BYTE g_DynamicSkillInfo[];

//extern _int64				g_OnlineStart;
extern _int64				g_OnlineEnd;
//extern _int64				g_Online_Update_ticks;

extern _int64				g_OnlineMinStart;
extern _int64				g_Online_Update_Min_ticks;

extern long					g_GameTime;
extern long					g_GameDay;
extern long					g_GameMinute;

extern long					g_TotalNPC;			// DB俊乐绰 醚 荐
extern long					g_CurrentNPCError;	// 技泼俊辑 角菩茄 荐
extern long					g_CurrentNPC;		// 泅犁 霸烙惑俊辑 角力肺 悸泼等 荐
extern long					g_CurrentGuildCount;// 泅犁 辨靛俊 殿废茄 付瘤阜 sid锅龋 

extern WeatherArray			g_WeatherZone;
extern TownPotalArray		g_TownPotal;
extern RandomTownPotalArray	g_RandomTownPotal;

extern int					g_CityRank[5][3];		// 矫刮殿鞭寸 皑家摹
extern int					g_CityRankValue[];		// 矫刮殿鞭 穿利摹

extern int					g_NpcCallNum;//玩家召唤怪物记录数

extern CFile				g_fpSpeedHack;
extern CFile				g_fpShuaWuPin;//GM刷物品记录
extern CFile				g_fpMarkDeBug;//MarkJay漏洞信息
extern CFile				g_fpMarkShiQuLog;//拾取日志
extern CFile				g_fpBackServer;
extern CFile				g_fpGuildHouseRank;
extern CFile				g_fpEventItem;

extern NpcThreadArray		g_arEventNpcThread;

extern int					g_ClassWeight[];		// 阿 拌凯喊 扁夯 公霸

extern SharedMemoryArray	g_arSharedMemory;
extern SharedMemoryArray	g_arStoreSharedMemory;
extern SharedMemoryArray	g_arFortressSharedMemory;
extern SharedMemoryArray	g_arGuildHouseSharedMemory;

extern int	g_ItemAttUpgrade[];
extern int	g_ItemDefUpgrade[];
extern int	g_ItemDefUpgrade_[];
extern int  g_ItemAccessoriUpgrade[];
extern int  g_ItemBlessingUpgrade[];
extern int  g_HuangJLUpgrade[];//普通黄金石头
extern int  g_HuangJLUpgrade12[];//百级黄金石头
extern int  g_ItemNormalDownUpgrade[];
extern int  g_ItemEbodyUpgrade[];
extern int	g_ItemGuardian[];
extern int  g_ItemHsUpgrade[];
extern int	g_ItemFuShouUpgrade[];//小黑副手

extern volatile long g_bShutDown;

extern volatile long g_dwItemSerial;		// Item Logging
extern DWORD g_dwFortressTime;

extern CEventBlockingTable	*g_pEventBlock;

extern int g_iMoonEvent;

extern int g_iAccessoriSlot[MAX_ACCESSORI];
extern int g_iGuardianiSlot[GUARDIAN_SLOT];
extern const short	SID_RING_OF_LIFE;
extern const short	SID_NECKLACE_OF_SHIELD;
extern const short	SID_EARRING_OF_PROTECT;
extern const short	SID_EARRING_OF_FEIFEI; 
extern const short SID_SHENGREN_JIEZHI;//圣人戒指

#endif