#ifndef _PACKET_H_
#define _PACKET_H_

#define ACCOUNT_LENGTH		12  // 帐号字节
#define PASSWORD_LENGTH		12  // 密码字节
#define CHAR_NAME_LENGTH	30  // 人物名字节
#define	CHAR_NAME_LENGTH_CLIENT 16
#define GUILD_MARK_SIZE		384
#define GUILD_MAP_COUNT		40 //  40俺
#define TEL_NAME_LENGTH		20
#define SHOP_NAME_LENGTH	18 // 商店字节

#define CLASS_NUM			4  //职业总数

#define SKILL_NUM			5 //技能框
#define SKILL_LEVEL			41 // 技能等级总数

//#define SKILL_NAME_LENGTH	14

#define EQUIP_ITEM_NUM		10  // 装备栏空间
#define	INVENTORY_NUM		24 // 包裹空间
//#define QUICK_ITEM		5
#define EBODY_NUM			4  // 机械身体空间
#define GUARDIAN_NUM		1  // 守护空间
#define TOTAL_SKILL_NUM			(CLASS_NUM * SKILL_NUM)
//#define TOTAL_CLASS_SKILL_NUM	(SKILL_NUM * SKILL_LEVEL)

#define TOTAL_PSI_NUM		32 // 魔法总数

#define MAX_ITEM_NUM		255			// 一次移动物品总数
#define BANK_MAX_ITEM		1000		// 个人银行物品总数
#define ACCOUNT_BANK_MAX_ITEM	32767	// 综合银行物品总数

#define TOTAL_BANK_ITEM_NUM 48            // 个人银行空间数
#define TOTAL_ACCOUNT_BANK_ITEM_NUM 150   // 综合银行空间数

#define TOTAL_PERSONALSHOP_ITEM_NUM 7    // 个人商店物品数

#define TOTAL_INVEN_MAX		((EQUIP_ITEM_NUM + INVENTORY_NUM))//包裹栏   
#define TOTAL_ITEM_NUM		((EQUIP_ITEM_NUM + INVENTORY_NUM + EBODY_NUM + 1))
#define TAOZHUANG_ITEM_NUM		((EQUIP_ITEM_NUM + EBODY_NUM + 1 + 1))//穿上装备判断 MarkJay
//#define TOTAL_ITEM_SLOT_NUM ((EQUIP_ITEM_NUM + INVENTORY_NUM + QUICK_ITEM))
//#define TOTAL_USER_ITEM_NUM	((TOTAL_ITEM_NUM + EBODY_ITEM))

#define _SKILL_SIZE			((sizeof(short) + sizeof(BYTE)))
#define _ITEM_SIZE			((sizeof(short) * 5) + (sizeof(BYTE) * 11) + (sizeof(BYTE)))
#define _PSI_SIZE			((sizeof(short) + sizeof(BYTE)))
#define _SKILLINFO_SIZE		((sizeof(BYTE) + sizeof(DWORD)))

#define USER_SKILL_LEN		(_SKILL_SIZE * TOTAL_SKILL_NUM)
#define USER_ITEM_LEN		(_ITEM_SIZE * TOTAL_ITEM_NUM)

#define USER_SKILLINFO_LEN	(_SKILLINFO_SIZE * TOTAL_SKILL_NUM)

#define EQUIP_SLOT			0
#define INVENTORY_SLOT		1
//#define QUICK_SLOT			2
#define TRADE_SLOT			3
#define BANK_SLOT			4
#define GUILD_SLOT			5
#define ACCOUNT_BANK_SLOT	6

#define BRAWL				0		// Character Making 矫 荤侩登绰 努贰胶 箭磊
#define STAFF				1
#define	EDGED				2
#define FIREARMS			3
#define NOCLASS				4

#define BRAWL_ITEM			8		// Item Table 俊 沥狼等 努贰胶 箭磊
#define STAFF_ITEM			4
#define	EDGED_ITEM			2
#define FIREARMS_ITEM		1

#define MAX_MOVE_TIME		100

#define ATTACK_UPGRADE_BAND		16	// 物理攻击武器升级加成数
#define ATTACK_UPGRADE_PSI_BAND	4	// 魔法攻击武器升级加成数
#define DEFENSE_UPGRADE_BAND	4	// 防具升级加成数

const BYTE SUCCESS				=	1;
const BYTE FAIL					=	0;
const BYTE SUCCESS_ZONE_CHANGE	=	2;

const BYTE ERR_1				=	1;
const BYTE ERR_2				=	2;
const BYTE ERR_3				=	3;
const BYTE ERR_4				=	4;
const BYTE ERR_5				=	5;
const BYTE ERR_6				=	6;
const BYTE ERR_7				=	7;
const BYTE ERR_8				=	8;
const BYTE ERR_9				=	9;
const BYTE ERR_10				=	10;
const BYTE ERR_11				=	11;
const BYTE ERR_12				=	12;
const BYTE UNKNOWN_ERR			=	255;

const BYTE ERR_SHORT_ATTACK_DELAY	= 0;
const BYTE ERR_TOO_LONG_DISTANCE	= 1;
const BYTE ERR_BROKEN				= 2;
const BYTE ERR_EMPTY_BULLET			= 3;
const BYTE ERR_EMPTY_ENERGE			= 4;
const BYTE ERR_EMPTY_SP				= 5;

const BYTE INFO_MODIFY			=	1;
const BYTE INFO_DELETE			= 	2;
const BYTE INFO_TOWNPOTAL		=   3;
const BYTE INFO_TOWNPOTAL_DELETE=	4;

const BYTE ITEM_INFO_MODIFY		=	1;
const BYTE ITEM_INFO_DELETE		=	2;
const BYTE ITEM_INFO_PICKUP		=	3;

const BYTE NORMAL_CHAT			=	1;//正常聊天
const BYTE WHISPER_CHAT			=	2;//密语聊天
const BYTE SHOUT_CHAT			=	3;//呐喊
const BYTE GUILD_CHAT			=	4;//军团聊天
const BYTE BUDDY_CHAT			=	5;//好友聊天
const BYTE BUDDY_INVITE			=	6;//组队聊天
const BYTE BUDDY_OFF			=	7;
const BYTE MOP_CHAT				=	10;
const BYTE WHISPER_OK			=	11;
const BYTE WHISPER_DENY			=	12;

const BYTE CALL_CHAT			=	15;		// 漂沥蜡历 家券
const BYTE REMOTE_CHAT			=	16;		// 漂沥蜡历 漂沥谅钎肺 碍力 捞悼
const BYTE MOP_CALL_CHAT		=	17;		// 漂沥各 漂沥谅钎肺 碍力 捞悼

const BYTE GUILD_INVITE			=	18;		// 辨靛 啊涝
const BYTE GUILD_DISPERSE		=	19;		// 辨靛 秦魂
const BYTE GUILD_OFF			=	20;		// 辨靛 呕硼
const BYTE GUILD_KICK_OUT		=	21;		// 辨付啊 辨靛盔 眠规

const BYTE ITEM_MODE_CHAT		=	22;		// 酒捞袍 罐扁 铰倡, 芭例甫 钎矫

const BYTE OP_CLOAK_ON			=	23;		// GM模式开启
const BYTE OP_CLOAK_OFF			=	24;		// GM模式关闭
const BYTE ALL_CHAT				=	25;		// 给所有人,地图,发送
const BYTE GUILD_FIELD_WAR		=   26;		// 惑措 辨靛甫 惑措肺 鞘靛傈阑 脚没茄促.
const BYTE GUILD_SURRENDER		=   27;		// 军团副团长
const BYTE PK_COUNT_CHAT		=	28;     // PK值
const BYTE COPY_TELEPORT		=	29;		// 瘤沥茄 惑措规苞 鞍捞 捞悼茄促.
const BYTE GUILD_USER_CHECK		=	30;		// 傈眉 辨靛盔 格废甫 焊咯霖促.
const BYTE GUILD_WAR_APPLY_LIST =	31;		// 辨傈阑 脚没茄 傈眉 辨靛 格废阑 焊咯霖促.
const BYTE GUILD_FORTRESS_DOOR	=	32;		// 器飘府胶 己巩阑 凯绊 摧绰促.
const BYTE GUILD_DEPUTE_POWER	=	33;		// 辨付 鼻茄阑 何辨付俊霸 困烙
const BYTE GUILD_STOP_POWER		=	34;		// 何辨付 鼻茄阑 沥瘤
const BYTE GUILD_USER_INFO_CALL =	35;		//yskang 0.2
const BYTE GUILD_CALL_CHAT		=	36;		//yskang 0.2 器飘府胶甫 啊瘤绊 乐绰 辨靛炉捞 漂沥 蜡历甫 家券茄促.

const BYTE SYSTEM_SPECIAL		=	0;		// 特殊弹出窗口
const BYTE SYSTEM_NORMAL		=	1;		// 普通蓝色信息
const BYTE SYSTEM_ERROR			=	2;		// 错误黄色信息
const BYTE SYSTEM_NPC			=	3;		// NPC的白色信息
const BYTE SYSTEM_ANNOUNCE		=	4;		// 公告红色信息
const BYTE SYSTEM_DISCONNECT	=	5;		// 断开连接弹出窗口
const BYTE SYSTEM_CESHIA        =	6;      // 1
const BYTE SYSTEM_CESHI	        =	8;      // 黑底红色电光信息

// Hit Mark List (Skill List)
const BYTE HIT_NONE				=	0;
const BYTE HIT_DAMAGE_C			=	1;
const BYTE HIT_VITAL_C			=	2;
const BYTE HIT_DELAY_C			=	3;
const BYTE HIT_CONCENTRATE		=	4;
const BYTE HIT_PSY_ACCLIMATE	=	5;
const BYTE HIT_FAST_CAST		=	6;
const BYTE HIT_WEAPON_ACCLIMATE	=	7;
const BYTE HIT_FAST_ATTACK		=	8;
const BYTE HIT_OUT_RANGE		=	9;
const BYTE HIT_STRAFE			=	10;
const BYTE HIT_BROKEN			=	11;
const BYTE HIT_FIXED			=	12;
const BYTE HIT_OVERHIT			=	13;
const BYTE HIT_FIRE				=	14;
const BYTE HIT_ICE				=	15;
const BYTE HIT_LIGHT			=	16;
const BYTE HIT_NUMB				=	17;
const BYTE HIT_POISON			=	18;
const BYTE HIT_C_GUARD			=	19;

const BYTE PSI_TELEPORT			=	7;

const BYTE BBS_NOT_OWNER		=	2;
const BYTE BBS_CONTENTS_DELETE	=	3;

// 惑怕捞惑 沥焊
//#define _ABNORMAL_DB			5

const BYTE ABNORMAL_TIME			=	0;
const BYTE PSI_ONE_TIME				=	1;
const BYTE PSI_TWO_TIME				=	2;
const BYTE PSI_THREE_TIME			=	3;

const BYTE ABNORMAL_BYTE_NONE		=	0;
const BYTE ABNORMAL_BYTE_POISON		=	1;
const BYTE ABNORMAL_BYTE_CONFUSION	=	2;
const BYTE ABNORMAL_BYTE_PARALYSIS	=	3;
const BYTE ABNORMAL_BYTE_BLIND		=	4;
const BYTE ABNORMAL_BYTE_LIGHT		=	5;
const BYTE ABNORMAL_BYTE_FIRE		=	6;
const BYTE ABNORMAL_BYTE_COLD		=	7;

const DWORD ABNORMAL_NONE			=	0;
const DWORD ABNORMAL_POISON			=	1;
const DWORD ABNORMAL_CONFUSION		=	1 << 2;
const DWORD ABNORMAL_PARALYSIS		=	1 << 3;
const DWORD ABNORMAL_BLIND			=	1 << 4;
const DWORD ABNORMAL_LIGHT			=	1 << 5;
const DWORD ABNORMAL_FIRE			=	1 << 6;
const DWORD ABNORMAL_COLD			=	1 << 7;
const DWORD ABNORMAL_HASTE			=	1 << 8; // 加速状态图标
const DWORD ABNORMAL_SHIELD			=	1 << 9; // 防护状态图标
const DWORD ABNORMAL_HIDE			=	1 << 10; // 体力回复?
const DWORD ABNORMAL_DEX_UP			=	1 << 11; // 加敏状态图标
const DWORD ABNORMAL_MAX_HP_UP		=	1 << 12; // 大血状态图标
const DWORD ABNORMAL_ADAMANTINE		=	1 << 13; // 金刚护盾图标
const DWORD ABNORMAL_MIGHTYWEAPON	=	1 << 14; // 武器强化图标
const DWORD ABNORMAL_BERSERKER		=	1 << 15; // 狂暴攻击图标
const DWORD ABNORMAL_HIEXP			=	1 << 16; // 兴奋图标
const DWORD ABNORMAL_MAGICFIND		=	1 << 17; // 幸运图标
const DWORD ABNORMAL_NOCHAT			=	1 << 18; // 禁言图标
const DWORD ABNORMAL_FASTRUN		=	1 << 19; // 神行图标
const DWORD ABNORMAL_MINDSHOCK		=	1 << 20; // 心灵风暴图标
const DWORD ABNORMAL_MINDGUARD		=	1 << 21; // 心灵守护图标
const DWORD ABNORMAL_PSISHIELD		=	1 << 22; // 抗魔护盾图标
const DWORD ABNORMAL_PIERCING_SHIELD=	1 << 23; // 力场穿刺图标
//const DWORD ABNORMAL_FUQIAN			=	1 << 24; // 福签图标
//const DWORD ABNORMAL_TEST2			=	1 << 25; //
const DWORD ABNORMAL_FUDAI			=	1 << 26; // 福袋图标
//const DWORD ABNORMAL_PAOBUTING		=	1 << 27; // 干燥饼干图标
const DWORD ABNORMAL_JIANYU			=	1 << 28;//坐牢
const DWORD ABNORMAL_BIANSHEN		=	1 << 29;//心灵守护
const DWORD ABNORMAL_BIGSHIELD		=	1 << 30;// 超级防护图标
const DWORD ABNORMAL_VIP	        =	1 << 24; //VIP
const DWORD OPERATION_MODE			=	1 << 31;	// GM模范

const DWORD STATE_1 = 1 << 0; //可以使用机械状态
const DWORD STATE_0_1 = 1 << 1;
const DWORD STATE_2 = 1 << 2; 
const DWORD STATE_3 = 1 << 3; 
const DWORD STATE_4 = 1 << 4; // hp up
const DWORD STATE_5 = 1 << 5; //武器加1
const DWORD STATE_6 = 1 << 6; //绿翅膀
const DWORD STATE_7 = 1 << 7; //天使
const DWORD STATE_8 = 1 << 8; 
const DWORD STATE_9 = 1 << 9; // hp down
const DWORD STATE_10 = 1 << 10; //
const DWORD STATE_11 = 1 << 11; //sp
const DWORD STATE_12 = 1 << 12; 
const DWORD STATE_13 = 1 << 13; //投身
const DWORD STATE_14 = 1 << 14; //x
const DWORD STATE_15 = 1 << 15; //白级外表
const DWORD STATE_16 = 1 << 16; 
const DWORD STATE_17 = 1 << 17; 
const DWORD STATE_18 = 1 << 18; //机甲狂暴
const DWORD STATE_19 = 1 << 19; 
const DWORD STATE_20 = 1 << 20; 
const DWORD STATE_21 = 1 << 21; //exp 2
const DWORD STATE_22 = 1 << 22; //magic up
const DWORD STATE_23 = 1 << 23; //xp 3
const DWORD STATE_24 = 1 << 24;//礼服
const DWORD STATE_25 = 1 << 25; 
const DWORD STATE_26 = 1 << 26; 
const DWORD STATE_27 = 1 << 27; 
const DWORD STATE_28 = 1 << 28;//防局+1
const DWORD STATE_29 = 1 << 29;//武器+2
const DWORD STATE_30 = 1 << 30;//图标

const DWORD ABNORMAL_CLEAR_STATUS	= (0xFFFFFFFF ^ (ABNORMAL_POISON | ABNORMAL_CONFUSION | ABNORMAL_PARALYSIS | ABNORMAL_BLIND | ABNORMAL_LIGHT | ABNORMAL_FIRE | ABNORMAL_COLD));
// 第一形态魔法类 包含 (拳 加速 |  法 防护 超级防护 心灵风暴 | 刀 魔法抗盾 | 枪 疾风之雨 生命之光 力场穿刺)
// 此类型魔法无法叠加 任何 不论职业
const DWORD ABNORMAL_CLEAR_STATUS1	= (0xFFFFFFFF ^ (STATE_1 | STATE_5 | STATE_6 | STATE_7 | STATE_13 | STATE_18 | STATE_21 | STATE_22 | STATE_23 | STATE_24 | STATE_28 | STATE_29 | STATE_30));
const DWORD	ABNORMAL_CLEAR_PSI_ONE	= (0xFFFFFFFF ^ (ABNORMAL_HASTE | ABNORMAL_SHIELD | ABNORMAL_HIDE | ABNORMAL_DEX_UP | ABNORMAL_MAX_HP_UP | ABNORMAL_FASTRUN | ABNORMAL_PSISHIELD | ABNORMAL_PIERCING_SHIELD | ABNORMAL_MINDSHOCK));
// 第二形态魔法类 包含 (金刚护盾 武器强化 狂暴攻击)
const DWORD ABNORMAL_CLEAR_PSI_TWO	= (0xFFFFFFFF ^ (ABNORMAL_ADAMANTINE | ABNORMAL_MIGHTYWEAPON | ABNORMAL_BERSERKER));
// 第三形态魔法类 包含 (拳 神行 | 法 心灵守护)  神行与加速 可以叠加为神速并叠加狂暴
const DWORD ABNORMAL_CLEAR_PSI_THREE= (0xFFFFFFFF ^ (ABNORMAL_MINDGUARD));


//const DWORD New_Status_22	= 1<<21;//玩家名字上带NEW 只有自己能看到

const short TYPE_MONEY_SID		=	10000;	// 酒捞袍 苞 捣阑 备盒窍扁困秦 sid甫 农霸 棱疽促.

//效果
const BYTE HP_POTION			= 1;//加血
const BYTE PP_POTION			= 2;//加蓝
const BYTE SP_POTION			= 3;//加体
const BYTE ALL_POTION			= 4;//加常
const BYTE SALUTE				= 5;//鞭炮
const BYTE EXP_POTION			= 6;//使用兴奋
const BYTE MAGIC_POTION			= 7;//使用幸运
const BYTE KILLREMOVE_POTION	= 8;
const BYTE DEAD_POTION			= 9;//使用圣药
const BYTE XMAS_SALUTE			= 10;//圣诞烟花
const BYTE YEAR_SALUTE			= 11;//新年烟花
const BYTE BIAN_SHEN			= 15;//变身效果显示
const BYTE TEST_21				= 21;//无

//#################################################################
//	Zone Server
//#################################################################
const BYTE	SERVER_LOGIN_REQ			= 1;

const BYTE	SERVER_LOGIN_RESULT			= 2;

const BYTE	SERVER_USER_LOGIN_REQ		= 3;

const BYTE	SERVER_USER_LOGIN_RESULT	= 4;

const BYTE	SERVER_DOUBLE_ACCOUNT_CLOSE = 5;

const BYTE	SERVER_DOUBLE_ID_CLOSE		= 6;

const BYTE	SERVER_USER_LOGOUT_REQ		= 7;

const BYTE	SERVER_USER_ZONELOGIN_REQ	= 8;

const BYTE	SERVER_USER_ZONELOGIN_RESULT= 9;

const BYTE	SERVER_USER_WHISPER_ONOFF	= 10;

const BYTE	SERVER_USER_WHISPER_REQ		= 11;

const BYTE	SERVER_USER_WHISPER_RESULT	= 12;

const BYTE	SERVER_USER_GUILD_CHAT		= 13;

const BYTE	SERVER_GUILD_NEW			= 14;

const BYTE	SERVER_GUILD_DISPERSE		= 15;

const BYTE	SERVER_GUILD_INVITE			= 16;

const BYTE	SERVER_GUILD_OFF			= 17;

const BYTE	SERVER_GUILD_MARK			= 18;

const BYTE	SERVER_USER_RESTART			= 19;

const BYTE	SERVER_USER_ZONE_LOGOUT		= 20;

const BYTE	SERVER_ALIVE				= 21;

const BYTE	SERVER_ANNOUNCE				= 22;

const BYTE	SERVER_ARK_WINNER			= 23;

const BYTE  SERVER_GUILD_SUBMASTER_ON	= 24;

const BYTE  SERVER_GUILD_SUBMASTER_OFF	= 25;

const BYTE	ZONE_LOGIN_REQ				= 233;

const BYTE	ZONE_LOGIN_RESULT			= 234;


// CHAR INFO SERVER
const BYTE CIS_DISCONNECT				= 235;

//#################################################################
//	辑滚 抛胶飘 内靛烙 (2001-11-15)
//#################################################################
const BYTE CHECK_ALIVE_REQ		= 255;

const BYTE CHECK_ALIVE_RESULT	= 255;

const BYTE CLIENT_TICK			= 254;


const BYTE DB_USER_COUNT			= 253;


const BYTE DB_GUILD_WHEREHOUSE_EVENT_REQ = 252;


const BYTE DB_GUILD_ITEM_MOVE_REQ	= 251;


const BYTE ANNOUNCE_NOTICE			= 250;


const BYTE ENCRYPTION_PKT			= 249;


const BYTE DB_USER_DEAD				= 248;


//#################################################################
//	Information		(1 ~ 9)
//#################################################################

const BYTE USER_INFO			=	1;

const BYTE CHAR_DATA			=	2;

const BYTE NPC_INFO				= 3;

const BYTE	ITEM_INFO			= 4;

const BYTE	SYSTEM_MSG			= 5;

const BYTE CLIENT_TIMER			= 6;

const BYTE SET_CLIENT_WORLD		= 7;

const BYTE CHANGE_ITEM_INFO		= 8;
const BYTE USERINFO_REQ			= 9;

//#################################################################
//	Login UI	(10 ~ 29)
//#################################################################

const BYTE SESSION_LOGIN_REQ	=	10;

const BYTE SESSION_LOGIN_RESULT	=	11;

const BYTE ACCOUNT_LOGIN_REQ =		12;
const BYTE ACCOUNT_LOGIN_RESULT =	13;

const BYTE GAME_START_REQ =				14;

const BYTE GAME_START_RESULT =			15;

const BYTE NEW_ACCOUNT_REQ =		16;

const BYTE NEW_ACCOUNT_RESULT =		17;

const BYTE	NEW_CHAR_REQ		= 18;

const BYTE	NEW_CHAR_RESULT		= 19;

const BYTE	DELETE_CHAR_REQ		= 20;

const BYTE	DELETE_CHAR_RESULT	= 21;

const BYTE	LOGOUT_REQ			= 22;

const BYTE	REROLL_REQ			= 23;

const BYTE	REROLL_RESULT		= 24;

const BYTE	RESTART_REQ			= 25;

const BYTE	RESTART_RESULT		= 26;

const BYTE	CHECK_ID_REQ		= 27;

const BYTE	CHECK_ID_RESULT		= 27;

//#################################################################
//	CHAT, BBS		(28 ~ 43)
//#################################################################

const BYTE	MOVE_CHAT_REQ		= 28;

const BYTE	MOVE_CHAT_RESULT	= 29;

const BYTE	CHAT_REQ			= 30;

const BYTE	CHAT_RESULT			= 31;

const BYTE	FIND_USER_REQ		= 32;

const BYTE	FIND_USER_RESULT	= 33;

const BYTE	BBS_OPEN			= 34;

const BYTE	BBS_NEXT			= 35;

const BYTE	BBS_NEXT_RESULT		= 36;

const BYTE	BBS_READ			= 37;

const BYTE	BBS_READ_RESULT		= 38;

const BYTE	BBS_WRITE			= 39;

const BYTE	BBS_EDIT			= 40;

const BYTE	BBS_ERROR			= 41;

const BYTE	BBS_DELETE			= 42;

//#################################################################
//	Item		(43 ~ 63)
//#################################################################
const BYTE	ITEM_BULLNUM		= 43;

const BYTE	ITEM_DURATION		= 44;

const BYTE	ITEM_LOAD_REQ		= 45;

const BYTE	ITEM_LOAD_RESULT	= 46;

const BYTE	ITEM_GIVE_REQ		= 47;

const BYTE	ITEM_GIVE_RESULT	= 48;


const BYTE	ITEM_TRADE_REQ		= 49;

const BYTE	ITEM_TRADE_RESULT	= 50;

const BYTE	EXCHANGE_REQ		= 51;

const BYTE	EXCHANGE_RESULT		= 52;

const BYTE	ITEM_MOVE_REQ		 = 53;

const BYTE	ITEM_MOVE_RESULT	 = 54;

const BYTE  ITEM_USE_REQ		= 55;

const BYTE  ITEM_USE_RESULT		= 56;

const BYTE	MONEY_CHANGED		= 57;

const BYTE	FIELD_ITEM_INFO		= 58;

const BYTE	ITEM_WEIGHT_CHANGE	= 59;

const BYTE	SERVER_TIME_REQ		= 59;

const BYTE	SERVER_TIME_RESULT	= 59;

const BYTE PK_BUTTON_REQ			= 62;

const BYTE	QUESTWINDOW_OPEN_REQ	= 63;

const BYTE	QUESTWINDOW_OPEN_RESULT	= 63;	// Packet 犁劝侩

const BYTE	TRADE_MODE_REQ		= 64;

const BYTE	TRADE_MODE_RESULT	= 65;

const BYTE DH_DAMAGE_NUM = 66;

const BYTE M_SET_HP = 67;

const BYTE	OPEN_EQUIP_REQ		= 68;

const BYTE	OPEN_EQUIP_RESULT	= 69;

const BYTE	CHANGE_WEAPON_REQ	= 70;

const BYTE	CHANGE_WEAPON_RESULT= 71;

const BYTE	SKILL_ONOFF_REQ		= 72;

const BYTE	SKILL_ONOFF_RESULT	= 73;

const BYTE	PSI_CHANGE_REQ		= 74;

const BYTE	PSI_CHANGE_RESULT	= 75;

const BYTE	LEVEL_UP			= 76;

const BYTE	LEVEL_DOWN			= 77;

const BYTE	USER_STATUS_SKILL_UP_REQ= 78;

const BYTE	USER_STATUS_SKILL_UP_RESULT= 79;

const BYTE	BUDDY_MODE_REQ			= 80;

const BYTE	BUDDY_MODE_RESULT		= 81;

const BYTE	BUDDY_USER_CHANGE		= 82;

const BYTE	CHANGE_DIR_REQ		= 88;

const BYTE CHANGE_DIR_RESULT	= 89;

const BYTE	MOVE_FIRST_REQ		= 90;

const BYTE	MOVE_FIRST_RESULT	= 91;

const BYTE	MOVE_REQ			= 92;

const BYTE	MOVE_RESULT			= 93;

const BYTE	MOVE_END_REQ		= 94;

const BYTE	MOVE_END_RESULT		= 95;

const BYTE	RUN_FIRST_REQ		= 96;

const BYTE	RUN_FIRST_RESULT	= 97;

const BYTE	RUN_REQ				= 98;

const BYTE	RUN_RESULT			= 99;

const BYTE	RUN_END_REQ			= 100;

const BYTE	RUN_END_RESULT		= 101;

const BYTE	SET_STAEMINA		= 102;

const BYTE SET_XP					= 106;

const BYTE SET_HP					= 107;

const BYTE SET_PP					= 108;

const BYTE PSI_ATTACK_REQ			= 109;

const BYTE PSI_ATTACK_RESULT		= 110;

const BYTE ATTACK_REQ				= 111;

const BYTE ATTACK_RESULT			= 112;

const BYTE	DEAD					= 113;

const BYTE	LIVE_REQ				= 114;

const BYTE	PSI_TOWN_POTAL			= 115;

const BYTE	SET_SPEED_MONSTER		= 116;

const BYTE	SET_USER_STATE			= 117;

const BYTE	SET_USER_PK_STATE			= 118;

const BYTE	USE_POTION					= 119;

const BYTE MAX_HP_EXIT				= 120;

const BYTE	CLIENT_EVENT			= 131;

const BYTE	STORE_OPEN				= 132;

const BYTE	BUY_ITEM				= 133;

const BYTE	CLIENT_EVENT_SAY		= 134;

const BYTE	SELL_ITEM_OPEN_RESULT	= 135;

const BYTE	SELL_ITEM				= 136;

const BYTE	REPAIR_ITEM_OPEN_RESULT	= 138;

const BYTE	REPAIR_ITEM				= 139;		

const BYTE	BUY_PSY_OPEN_RESULT		= 140;

const BYTE	BUY_PSY_REQ				= 141;

const BYTE	BUY_PSY_RESULT			= 142;

const BYTE	BANK_OPEN				= 143;

const BYTE	BANK_ITEM_MOVE_REQ		= 144;

const BYTE	BANK_ITEM_MOVE_RESULT	= 145;

const BYTE	CURE_RESULT				= 146;

const BYTE	HELP_REQ				= 147;

const BYTE	ZONE_CHANGE_RESULT		= 148;

const BYTE	EBODY_IDENTIFY_OPEN	= 149;

const BYTE EBODY_IDENTIFY_REQ		= 150;

const BYTE UPGRADE_ITEM_DIALOG		= 152;

const BYTE UPGRADE_ITEM_REQ			= 153;

const BYTE UPGRADE_ITEM_RESULT		= 154;

const BYTE TELEPORT_REQ				= 155;

const BYTE TELEPORT_RESULT			= 156;

const BYTE TELEPORT_EDIT_REQ		= 157;

const BYTE TELEPORT_EDIT_RESULT		= 158;

const BYTE TELEPORT_COPY_REQ		= 159;

const BYTE TELEPORT_COPY_DIALOG_REQ	= 160;

const BYTE TELEPORT_COPY_RESULT		= 161;

const BYTE GUARD_BOW				= 162;

const BYTE EVENT_UPGRADE_ITEM_REQ   = 163;

//#################################################################
//	Guild		(170 ~ 200)
//#################################################################
const BYTE  GUILD_DIALOG			= 170;

const BYTE GUILD_OPEN_REQ			= 171;

const BYTE GUILD_OPEN_RESULT		= 172;

const BYTE GUILD_ADD_REQ			= 173;

const BYTE GUILD_ADD_RESULT			= 174;

const BYTE GUILD_MARK_ADD_REQ		= 175;

const BYTE GUILD_MARK_ADD_RESULT	= 176;

const BYTE	GUILD_WAREHOUSE_OPEN	= 177;

const BYTE	GUILD_ITEM_MOVE_REQ		= 178;

const BYTE	GUILD_ITEM_MOVE_RESULT	= 179;

const BYTE GUILD_INFO				= 180;

const BYTE GUILD_USER_INFO_REQ		= 181;

const BYTE GUILD_USER_INFO_RESULT	= 182;

const BYTE GUILD_WAREHOUSE_CLOSE	= 183;

const BYTE GUILD_TAX_OPEN			= 184;

const BYTE GUILD_TAX_REQ			= 185;

const BYTE GUILD_WAR_OPEN			= 186;

const BYTE GUILD_WAR_REQ			= 187;

const BYTE GUILD_WAR_APPLY_OPEN		= 188;

const BYTE GUILD_WAR_APPLY_REQ		= 189;

const BYTE GUILD_WAR				= 190;

const BYTE GUILD_FIELD_WAR_REQ		= 191;

const BYTE GUILD_FIELD_WAR_RESULT	= 192;

const BYTE SIGHT_INFO_COMPRESSED	= 193;

const BYTE GUILD_HOUSE_APPLY_REQ	= 194;

const BYTE GUILD_FORTRESS_NCIRCLE	= 195;

const BYTE DEL_GUILD_CHAR_REQ		= 196;

const BYTE GUILD_INVITE_USER_REQ	= 197;

const BYTE CHALLENGE_GUILD			= 198;

const BYTE GUILD_OFF_WITH_THREAD	= 199;

const BYTE	NEW_CHAR_REQ_WITH_THREAD = 200;

const BYTE ADDRESS_WINDOW_OPEN		= 201;

const BYTE AREA_ATTACK_RESULT		= 202;

//const BYTE GUILD_WAR_AREA			= 203;

const BYTE DRESSING					= 204;		

const BYTE DRESSING_WINDOW_OPEN		= 205;

const BYTE ACCOUNT_BANK_OPEN		= 206;

const BYTE ACCOUNT_BANK_ITEM_MOVE_RESULT = 207;

const BYTE SELECT_OVER100_SKILL		= 208;

const BYTE ACCOUNT_BANK_ITEM_MOVE_REQ = 209;

const BYTE PRE_REG_USER_EVENT_REQ	= 210;

const BYTE PRE_REG_GAMEROOM_EVENT_REQ= 211;

const BYTE ADDRESS_WINDOW_OPEN_NEW		= 212;

const BYTE ENCRYPTION_START_REQ			= 213;

const BYTE ENCRYPTION_START_RESULT		= 214;

const BYTE GUILD_INVITE_REQ				= 215;

const BYTE GUILD_INVITE_RESULT			= 216;
//-----------------------------------------------------------------------------------------------------
//--yskang 0.1
//辨靛 扁瓷吝 龋莫阑 何咯窍扁 困窍咯 眠啊登绰 菩哦
//-----------------------------------------------------------------------------------------------------
const BYTE LOVE_NAME_REQ		= 217;//重复

const BYTE LOVE_NAME_RESULT		= 217;

const BYTE LOVE_NAME			= 218; 

const BYTE MAIL_INFO_REQ			= 219;	

const BYTE MAIL_INFO_RESULT			= 219;

const BYTE MAIL_SEND_REQ			= 220;	

const BYTE MAIL_SEND_RESULT			= 220;

const BYTE MAIL_LIST_REQ			= 221;

const BYTE MAIL_LIST_RESULT			= 221;

const BYTE MAIL_CONTENTS_REQ		= 222;

const BYTE MAIL_CONTENTS_RESULT		= 222;

const BYTE MAIL_DELETE_REQ			= 223;

const BYTE MAIL_DELETE_RESULT		= 223;


const BYTE REVIVAL_REQ				= 224;


const BYTE REVIVAL_RESULT			= 225;

const BYTE EXPRESSION_REQ			= 226;

const BYTE EXPRESSION_OPEN			= 230;
const BYTE TRANSFORM_REQ			= 234;
const BYTE TRANSFORM_WINDOW			= 236;
/*
	(C ->S )
	BYTE			00			//40级准备改变	和238的一样的
					01			//机甲升级框 能量套装改造 (制作控制系统，是不是机甲？？？)
					02			//一个能放任何物品，包括药的物品框,不知道是什么。。。返回的包 27 00 00 00 3E 07 (0A 00 ,这个是SLOT) (01 00， 这个是数量) 
								//02是用来销毁物品用的

					03			//一个空白消自框，试试看能不能放文字
								short				//event num
								varchar				//弹出确认框的文字
								short				//zero
					04			//一个ID CHECK的 角色更名的
					05			//装备锁定
					06			装备解锁

					08			//放入哈迪斯物品的？这里能放任何物品，还有，能放数量.
								//返回的包 3E 5B 0B 00(位置) 01 00(数量) 00 
*/
const BYTE PERSONAL_SHOP_REQ		= 239;
const BYTE SCREEN_CAPTURE_REQ		= 245;

#endif
