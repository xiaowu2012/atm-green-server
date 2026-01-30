#ifndef _DEFINE_H_
#define _DEFINE_H_
#define _ACTIVE_USER
#define CHN_VER

#define _MAX_DWORD				0xFFFFFFFF
#define _MAX_INT				0x7FFFFFFF
#define _MAX_SHORT				0x7FFF

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>接受 发送 结束
#define OVL_RECEIVE				0X01
#define OVL_SEND				0X02
#define OVL_CLOSE				0X03
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>数据包发送标志和结束
#define PACKET_START1			0XAA
#define PACKET_START2			0X55
#define PACKET_END1				0X55
#define PACKET_END2				0XAA
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>接受和发送用户数据包大小
#define RECV_BUF_SIZE			10000
#define SEND_BUF_SIZE			10000


//
//	Define CriticalSection Spin Count
//
#define SPIN_COUNT				4000

//
//	About City
//
#define SANAD					0

//守护天使
#define MAX_TianShi_DAY             30
//
//
//	About USER
//
#define USER_DEAD				0X00
#define USER_LIVE				0X01

//
//	About GUILD
//
#define MAX_GUILD				30000    //最大军团数量
#define MAX_GUILD_USER			50       //军团最大人数
#define GUILD_WAR_PREPARE		0        //要塞准备
#define GUILD_WAR_DECISION		1        //要塞决定
#define GUILD_MAX_TAX			200
#define MAX_GUILD_HOUSE_USER	20

#define GUILD_WAR_READY		0		// 辨靛傈 官肺 5盒傈俊 惯积窍绰 霖厚 窜拌
#define GUILD_WARRING		1		// 瘤陛 辨靛傈吝烙
#define GUILD_WAR_AFFTER	2		// 辨靛傈捞 场抄饶

#define FORTRESS_VIOLENCE_WARRING	1	// 刮缴捞 馆鄂吝
#define FORTRESS_VIOLENCE_AFFTER	0	// End

#define GUILD_WAR_MAX_MSG_TIMER	6		// 傍己傈吝 巢篮 矫埃阑 舅妨霖促.

#define GUILD_GUARD_MAX_NUM		6	// 夸货甫 瘤虐绰 版厚捍栏肺 弥措 6疙鳖瘤 绊侩且荐乐促.
#define _GUILDNPCLIST_DB		24	// 4 * 6

#define GUILD_REPAIR_MAX_NUM	5	// 弥措 5俺 鳖瘤 荐府且 NPC啊 乐促.
#define _GUILDREPAIRLIST_DB		30 // 5 * 6

#define FORTRESS_TARGET_MAX_NUM	4	// 弥措 4俺 鳖瘤 坷胆肺啊 乐促.
#define GUILD_ATTACK_MAX_NUM	20	// 弥措 20俺鳖瘤 辨靛傈 脚没阑 罐阑荐 乐促
#define _GUILDLIST_DB			80	// 4 * 20

#define GUILDFORTRESS_ATTACK_MAX_NUM	20	// 弥措 50俺鳖瘤 辨靛傈 脚没阑 罐阑荐 乐促
#define _GUILDFORTRESSLIST_DB			200	// 4 * 50

#define GUILD_VIRTUAL_WAR_START	16
#define GUILD_VIRTUAL_WAR_END	17
#define GUILD_HOUSE_DAY			18

#define GUILD_FIELD_WARRING		1
#define GUILD_STORE_WARRING		2
#define GUILD_FOTRESS_WARRING	3		
//
//	About NPC
//
#define NPC_NUM					200
#define NPC_DEAD				0X00    //死亡
#define NPC_LIVE				0X01    //活着
#define NPC_ATTACKING			0X02    //攻击中
#define NPC_ATTACKED			0X03
#define NPC_ESCAPE				0X04    //逃跑
#define NPC_STANDING			0X05    //站立
#define NPC_MOVING				0X06
#define NPC_TRACING				0X07
#define NPC_FIGHTING			0X08
#define NPC_STRATEGY			0x09    //策略
#define NPC_BACK				0x10
#define NPC_WAIT				0x11	// 啊惑 傍埃俊辑 积己窍绰 各篮 老雀己 捞骨肺

#define NPC_PASSIVE				150
#define NPC_ACTIVE				100

//
//	About Map Object
//
#define USER_BAND				10000		// Map 困俊 蜡历啊 乐促.
#define NPC_BAND				20000		// Map 困俊 NPC(各器窃)啊 乐促.
#define INVALID_BAND			30000		// 肋给等 ID BAND

//
//	About Big Mop Size In CELL
//
#define NPC_MAP_SIZE			20

//
//	Defines About Max Value
//
#define MAX_BASIC_ITEM			1000
#define MAX_EVENT				10000      //最大脚本事件号

//
//	To Who ???
//
#define TO_ALL					0X01 //发送给所有人
#define TO_ZONE					0X02 //发送给当前地图
#define TO_ME					0X03 //发送给自己
#define TO_INSIGHT				0X04 //发送给周围

#define SEND_USER				0 //发送状态给用户
#define SEND_INSIGHT			1 //发送状态给周围
#define SEND_ZONE				2 //发送状态给当前地图
#define SEND_ALL				3 //发送状态所有人
#define SEND_RANGE				4 //发送状态给看到
#define SEND_SCREEN				5 //发送状态给屏幕内

//
//	矫具贸府
//
#define	SCREEN_X				15
#define	SCREEN_Y				19

//#define SIGHT_SIZE_X			10
//#define SIGHT_SIZE_Y			10
#define SIGHT_SIZE_X			12 //喊话范围
#define SIGHT_SIZE_Y			12

#define DIR_H					1
#define DIR_L					-1
#define DIR_OUTSIDE				3

//
//	State Value
//
#define STATE_ACCEPTED			0X01
#define STATE_CONNECTED			0X02
#define STATE_DISCONNECTED		0X03
#define STATE_GAMESTARTED		0X04
#define STATE_INITED			0X05
#define STATE_LOGOUT			0X06
#define STATE_GAMERESTART		0X07

//
//	Event
//
#define MAX_LOGIC_INT			5
#define MAX_EXEC_INT			3
#define MAX_LOGIC_ELSE_INT		20
#define MAX_LOGIC_CHAR_LEN		128
#define MAX_EXEC_CHAR_LEN		128
#define MAX_LOGIC_ELSE_CHAR_LEN 128

#define MONSTER_ATTACK_EVENT_BAND		10000		// 10000锅 捞惑狼 捞亥飘 锅龋绰 阁胶磐啊 傍拜寸沁阑锭 惯积茄促.

//
//	一般的魔法数量
//
#define BRAWL_PSI_NUM			2 //拳的魔法数量   
#define STAFF_PSI_NUM			7 //法的魔法数量
#define	EDGED_PSI_NUM			2 //刀的魔法数量
#define FIREARMS_PSI_NUM		2 //枪的魔法数量
#define NOCLASS_PSI_NUM			3 //全职业魔法数量

#define	DEFAULT_PSI_DELAY		30000 //初始魔法释放时间(无任何加载情况 法师职业)

//
//  Item
//
#define TYPE_MONEY				0
#define TYPE_ITEM				1

#define MAX_ITEM_UPGRADE_COUNT	10		// 装备改数
#define MAX_EBODY_UPGRADE_COUNT 10		// 机械身体最大改数
#define ITEM_NAME_LENGTH		20      // 物品最大名称
#define MAX_THROW_ITEM			80000		// 地面物品消失数量
#define ITEM_MAX_USE_WEAR		150		// 物品类型最大值(比如 116为重叠消耗品)
#define ITEM_USER_RIGHT_NUM		3		// 判断攻击类并技能有效武器位置代码.

#define EVENT_ITEM_LENGTH		30		// 任务物品的长度

#define EVENT_UNIQUE_ITEM_BAND	30000	// 唯一物品的.	
#define EVENT_RR_ITEM_BAND		30001   

// 升级用物品
#define BLESSING_WEAPONLESS_WEAR	23	// 超级灵石
#define BLESSING_ARMORLESS_WEAR		24	// 超级晶石		

#define NORMAL_WEAPONLESS_WEAR		25	// 灵石	
#define NORMAL_ARMORLESS_WEAR		26	// 晶石	
	
//
//	Quick Item Use
//
#define QUICK_ITEM_SP			1 //体力恢复剂
#define QUICK_ITEM_PP			2 //精神恢复剂
#define QUICK_ITEM_HP			4 //生命恢复剂
#define QUICK_ITEM_ALLREPAIR	7 //常用恢复剂
#define QUICK_ITEM_CONFUSE		8 //镇定剂
#define QUICK_ITEM_PALSY		16 // 麻痹解除
#define QUICK_ITEM_POISON		32 // 解毒剂
#define QUICK_ITEM_VISION		64 // 视力恢复
#define QUICK_ITEM_ALLCURE		127 //所有异常状态恢复
#define QUICK_ITEM_BULL			100 //能量包系列
#define QUICK_ITEM_POTAL		101 //回城系列药水
#define EVENT_ITEM_LOTTERY		102 //彩票系列
#define QUICK_ITEM_TELEPORT		103 //时空门系列
#define	EVENT_ITEM_DEADPOISON	104 //重生系列
#define QUICK_ITEM_SALUTE		113	//烟花系列

#define NORMAL_SALUTE_SID		850 //普通全局模式下
#define XMAS_SALUTE_SID			874 //圣诞全局模式下(包含 圣诞棒 巧克力等物品使用)
#define YEAR_SALUTE_SID			875 //新年全局模式下(包含 新年字体 福袋等效果)


//
//	Moon Event Item
//
#define EVENTITEM_WEAR_MOON		115  //月饼系列物品

//
//	专用的任务类物品
//
#define EVENTITEM_WEAR_INVASION	120  // 召唤盒子系列物品


//
//	Special Potion
//
#define SPECIAL_ITEM_BIANLANG	    	1028  //狼变身
#define SPECIAL_ITEM_BIANXIONG		    1029  //熊变身
#define SPECIAL_ITEM_FEIXINGQI		    1160  //机甲
#define SPECIAL_ITEM_FEIXINGQI3		    1413  //动物机甲
#define SPECIAL_ITEM_FEIXINGQI2		    1166  //战神
#define SPECIAL_ITEM_HUANLING		    1035  //幻灵
#define SPECIAL_ITEM_HUANJING		    1289  //幻晶1289
#define SPECIAL_ITEM_FEIXINGQI1		    1172  //超级机甲
#define SPECIAL_ITEM_RX 				1112  //取消变身
#define SPECIAL_ITEM_VIP_YUE	    	1017  //48小时VIP会员卡
#define SPECIAL_ITEM_VIP_YUE_939	    939  //5小时VIP会员卡
#define SPECIAL_ITEM_NENGYUAN			965   //守护能源
#define SPECIAL_ITEM_ZHAOHUAN	        1155  //军团召唤
#define SPECIAL_ITEM_TS				    1044  //守护天使
#define SPECIAL_ITEM_JIXIE				1182  //机械卷
#define SPECIAL_ITEM_1134				1134  //1000标志 
#define SPECIAL_ITEM_1135				1135  //100元宝
#define SPECIAL_ITEM_1136				1136  //1元宝
#define SPECIAL_ITEM_1142				1142  //1元宝
#define SPECIAL_ITEM_1370				1370  //1000元宝
#define SPECIAL_ITEM_1137				1137  //抽奖卷
#define SPECIAL_ITEM_1141				1141  //多功能状态卡
#define SPECIAL_ITEM_1149				1149  //改名卡
#define SPECIAL_ITEM_999				999   //各职业锆石票
#define SPECIAL_ITEM_1454				1454   //标志盒子
#define SPECIAL_ITEM_1455				1455   //武器盒子
#define SPECIAL_ITEM_1456				1456   //首饰盒子+
#define SPECIAL_ITEM_1113				1113   //首饰盒子
#define SPECIAL_ITEM_1001				1001  //9点给的票 MarkJay
#define SPECIAL_ITEM_1002				1002  //上午GM套装票
#define SPECIAL_ITEM_1196				1196  //GM抗魔套装
#define SPECIAL_ITEM_1197				1197  //GM回避套装
#define SPECIAL_BIAOZHI					724	  //标志


#define SPECIAL_POTION_WEAR		116    //双击使用的物品设置

#define SPECIAL_ITEM_HIEXP		699     //兴奋剂
#define SPECIAL_ITEM_MAGICFIND	700     //幸运药
#define SPECIAL_ITEM_KILLREMOVE	701     //忏悔药

#define SPECIAL_ITEM_HIEXP_1212	         1212     //超级兴奋剂
#define SPECIAL_ITEM_MAGICFIND_1189	     1189     //超级幸运药

#define NORMAL_ITEM_STAMP		714					// zinee 02-11-18

// Revival Item
#define NORMAL_ITEM_REVERSE		807

//
//	Psionic Stone
//
#define PSI_STONE_WEAR			117

#define PSI_STONE_FAST_RUN			706			  // 神行		
#define PSI_STONE_MIND_SHOCK		707           // 心灵风暴
#define PSI_STONE_MIND_GUARD		708            // 心灵守护
#define PSI_STONE_PSI_SHIELD		709            // 魔法盾牌
#define PSI_STONE_PIERCING_SHIELD	710            // 力场穿刺

//
//	辅助性魔法 （带有 魔法状态图标）
//
#define PSIONIC_HASTE				1 // 加速
#define PSIONIC_SHIELD				6 // 防护
#define PSIONIC_DEXUP				11 // 疾风之雨
#define PSIONIC_HPUP				12 // 生命之光

#define PSIONIC_ADAMANTINE			13 // 金刚护盾
#define PSIONIC_MIGHTYWEAPON		14 // 武器强化
#define PSIONIC_BERSERKER			15 // 狂暴攻击

//
//	任务完成系 魔法
//

#define PSIONIC_FAST_RUN			16 // 神行
#define PSIONIC_MIND_SHOCK			17 // 心灵风暴
#define PSIONIC_MIND_GUARD			18 // 心灵守护
#define PSIONIC_PSI_SHIELD			19 // 抗魔护盾
#define PSIONIC_PIERCING_SHIELD		20 // 力场穿刺

//
//	魔法去除命中率 100 = 100%
//
#define SUCCESS_RATE_MIND_SHOCK			25 // 心灵风暴的消除命中率
#define SUCCESS_RATE_PIERCING_SHIELD	10 // 力场穿刺的消蛋命中率

//
//	机械的属性列表
//
#define EBODY_MAGIC_COUNT				31 //属性总列，对应EBODY表 sPid

#define EBODY_NONE						0			// 无
#define EBODY_MOVE_UP					1			// 移动速度1阶段提高
#define EBODY_ATTACK_DELAY_DOWN			2			// 攻击速度1阶段提高
#define EBODY_WEIGHT_UP					3			// 负重 10%增加
#define EBODY_DEFENSE_UP				4			// 防御  2%增加
#define EBODY_PSI_RESIST_UP				5			// 魔法防御 5%增加

#define EBODY_VITAL_RESIST				6			// 穿刺伤害 5%减少			
#define EBODY_BACK_RESIST				7			// 反击伤害 5%减少
#define EBODY_WEAPON_BASE_UP			8			// 武器基本伤害 2%增加
#define EBODY_ATTACK_RATING_UP			9			// 命中率 2%增加
#define EBODY_PSI_ATTACK_UP				10			// 魔法伤害 2%增加

#define EBODY_HP_RECOVERY_UP			11			// 生命恢复增加 10%
#define EBODY_AVOID_RATING_UP			12			// 回避率 2%增加
#define EBODY_RECOVERY_TIME_UP			13			// 恢复周期减少2阶段
#define EBODY_CASTING_DELAY_DOWN		14			// 释放时间减少 2%
#define EBODY_PP_RECOVERY_UP			15			// 精神恢复增加 10%

#define EBODY_STR_TO_DAMAGE				16			// 力量 10%转伤害
#define EBODY_STR_TO_WEIGHT				17			// 力量 10%转负重
#define EBODY_CON_TO_HP					18			// 体质 10%转生命
#define EBODY_CON_TO_DEFENSE			19			// 体质 10%转防御
#define EBODY_CON_TO_BACK_RESIST		20			// 体质 10%转反击伤害减少

#define EBODY_CON_TO_VITAL_RESIST		21			// 体质 10%转穿刺伤害减少
#define EBODY_DEX_TO_ATTACK_RATING		22			// 敏捷 10%转命中
#define EBODY_DEX_TO_AVOID_RATING		23			// 敏捷 10%转回避
#define EBODY_DEX_TO_DAMAGE				24			// 敏捷 10%转远程伤害
#define EBODY_DEX_TO_BACK_RESIST		25			// 敏捷 10%转反击伤害减少

#define EBODY_DEX_TO_VITAL_RESIST		26			// 敏捷 10%转穿刺伤害减少
#define EBODY_VOL_TO_PSI_DAMAGE			27			// 智慧 10%转魔法伤害
#define EBODY_VOL_TO_PSI_RESIST			28			// 智慧 15%转魔法防御
#define EBODY_WIS_TO_PP					29			// 智力 10%转精神
#define EBODY_WIS_TO_CAST_DOWN			30			// 智力 10%转法术大师


//
//	Magic Item
//
#define MAGIC_COUNT						38			// 魔法属性类型
#define STATE_MAGIC_COUNT				8

#define MAGIC_NONE						0//无
#define MAGIC_STR_UP					1 //力量增加
#define MAGIC_CON_UP					2 //体质增加.
#define MAGIC_DEX_UP					3 //敏捷增加
#define MAGIC_VOL_UP					4 //智慧增加
#define MAGIC_WIS_UP					5 //智力增加.

#define MAGIC_BACK_ATTACK				6//反击技术
#define MAGIC_EXCITE					7//兴奋技术
#define MAGIC_IRON_SKIN					8//钢铁皮肤
#define MAGIC_HP_RECOVER_UP				9//自我医疗

#define MAGIC_PSI_CONCENTRATION			10//魔法精修
#define MAGIC_CASTING_DELAY_DOWN		11//法术精修
#define MAGIC_PP_CONSUME_DOWN			12//精神强化
#define MAGIC_PP_RECOVER_UP				13//冥想技术

#define MAGIC_VITAL_CRITICAL			14//穿刺技术
#define MAGIC_CRITICAL_GUARD			15//闪避技术
#define MAGIC_EDGE_CONCENTRATION		16//精神集中
#define MAGIC_SP_RECOVER_UP				17//回复技术

#define MAGIC_ATTACK_ACCURACY_SKILL_UP	18//狙击技术
#define MAGIC_FIRE_DOUBLE_ATTACK		19//连射技术
#define MAGIC_AVOID_SKILL_UP			20//灵动技术
#define MAGIC_MIN_DAMAGE_UP				21//枪械大师

#define MAGIC_DAMAGE_UP					22//损伤增加
#define MAGIC_STATE_ERROR_UP			23//诱发各种状态
#define MAGIC_STATE_ERROR_DOWN			24//防止各种异常状态
#define MAGIC_ALL_SKILL_UP				25//所有技术
#define MAGIC_RANGE_UP					26//射程增加 
#define MAGIC_ATTACK_DELAY_DOWN			27//攻击速度
#define MAGIC_ATTACK_RATING_UP			28//命中率
#define MAGIC_AVOID_RATING_UP			29//回避率
#define MAGIC_BROKEN_DOWN				30//故障率
#define MAGIC_DEFENSE_UP				31//防御增加
#define MAGIC_DURATION_UP				32//耐久性增加
#define MAGIC_MAX_HP_UP					33//生命值增加
#define MAGIC_MAX_PP_UP					34//精神值增加
#define MAGIC_MAX_SP_UP					35//体力值增加

#define MAGIC_PSI_RESIST_UP				36// 魔法抗斥增加
#define MAGIC_PSI_ATTACK_UP				37// 魔法损伤增加

//
//	各种异常状态
//
#define MAGIC_STATE_NORMAL				0 
#define MAGIC_STATE_ELECTRIC			1 
#define MAGIC_STATE_COLD				2 //冰冻
#define MAGIC_STATE_FIRE				4 //灼伤
#define MAGIC_STATE_CONFUSION			8 //眩晕
#define MAGIC_STATE_PARALISYS			16 //麻痹
#define MAGIC_STATE_POINSON				32 //中毒
#define MAGIC_STATE_BLIND				64 //视觉丧失
#define MAGIC_STATE_ALL					127

//
//	普通技能
//
#define SKILL_IRON						0//钢铁皮肤
#define SKILL_EXCITE					1//兴奋
#define SKILL_BACK_ATTACK				2//反击
#define SKILL_HP_RECOVER_UP				3//自我医疗

#define SKILL_PP_RECOVER_UP				5//冥想
#define SKILL_PP_DOWN					6//精神强化
#define SKILL_PSI_SUCCESS_UP			7//魔法精修
#define SKILL_CASTING_DOWN				8//法术大师

#define SKILL_CONCENTRATION				10//精神集中
#define SKILL_CRITICAL_GUARD			11//闪避
#define SKILL_VITAL_CRITICAL			12//穿刺
#define SKILL_SP_RECOVER_UP				13//回复

#define SKILL_MIN_DAMAGE_UP				15//枪械大师
#define SKILL_RE_FIRE_UP				16//连射
#define SKILL_ATTACK_RATING_UP			17//狙击
#define SKILL_AVOID_RATING_UP			18//灵动

#define SKILL_ASSAULT					20//怒邀
#define SKILL_ABSOLUTE_DEFENSE			21//绝对防御
#define SKILL_PSYCHIC_RESIST			22//魔法抗拆
#define SKILL_DEFENSE_UP				23//超级防御

////////////////////////////////////////////////////(官操扁 傈 单捞磐)
#define MAGIC_BROKEN_DOWN_5				5
#define MAGIC_BROKEN_DOWN_11_TO_15		67

#define		DEFAULT_AT_DELAY	150  //物理职业攻击速度基础数 MarkJay
#define		DEFAULT_AT_DELAY_UP	 3  //物理职业最大攻击速度 MarkJay

#define		DEFAULT_SP_DEC		1     // 默认体力消耗值(比如跑动时消耗体力单位)

////////////////////
//攻击判定
const BYTE	ATTACK_FAIL				=	0;//攻击失败
const BYTE	ATTACK_SUCCESS			=	1;//攻击成功
const BYTE	ATTACK_MISS				=	2;//攻击被回避
const BYTE	CRITICAL_ATTACK_SUCCESS =	3;//穿刺成功
const BYTE	CRITICAL_ATTACK_MISS	=	4;//穿刺被回避(对应无我)

const BYTE	ATTACK_MIN		=	15;//20;(最小攻击值)
const BYTE	ATTACK_MAX		=	90;//80;(最大攻击值) 物理判断无加持状态

#define		DAMAGE_DELAY_C_TIME	2000

//
//	升级附加值
//
#define SKILL_INIT_VALUE		300 //升级技能附加值(1/2 等级设置 对应数据库 LEVEL_UP)
#define PSY_INIT_VALUE			300 //魔法升级附加值(PSI_STORE数据库对应 )
#define LEVEL_INIT_VALUE		600 //级别判断附加值(对应数据库USER_LEVEL_UP 包含 HP PP 伤害 防御 负重)

//	槽子
#define LOGIC_POSITION			0X01       //物品槽子
#define LOGIC_MEET_NPC			0X02      //机械，守护槽子
#define EXEC_JIJIA_LAN          0X101    //机甲槽子

//	NPC脚本使用语法
#define EXEC_LINK				0X01
#define EXEC_SAY				0X02
#define EXEC_SAY_IN				0x03
#define EXEC_GIVE_ITEM			0X04 //给予物品
#define EXEC_ROB_ITEM			0X05 // 收取物品
#define EXEC_GIVE_MAGIC			0X06 
#define EXEC_ROB_MAGIC			0X07
#define EXEC_BBS_OPEN			0X08 // 公告栏打开
#define EXEC_DISAPPEAR			0X09
#define EXEC_RUN_MAGIC			0X0A 
#define EXEC_DEAD				0X0B 
#define EXEC_CHANGE_LEVEL		0X0C // 改变级别
#define EXEC_CHANGE_GENDER		0X0D // 改变性别
#define EXEC_CHANGE_HP			0X0E // 改变生命
#define EXEC_CHANGE_MP			0X0F // 改变精神
#define EXEC_CHANGE_STR			0X10 // 改变力量
#define EXEC_CHANGE_DEX			0X11 // 改变敏捷
#define EXEC_CHANGE_INT			0X12 // 改变智慧
#define EXEC_CHANGE_WIS			0X13 // 改变智力
#define EXEC_CHANGE_CHR			0X14
#define EXEC_CHANGE_CON			0X15
#define EXEC_CHANGE_DMAGIC		0X16
#define EXEC_CHANGE_WMAGIC		0X17
#define EXEC_CHANGE_BMAGIC		0X18
#define EXEC_CHANGE_SWORD		0X19
#define EXEC_CHANGE_SPEAR		0X1A
#define EXEC_CHANGE_AXE			0X1B
#define EXEC_CHANGE_BOW			0X1C
#define EXEC_CHANGE_KNUCKLE		0X1D
#define EXEC_CHANGE_WEAPONMAKE	0X1E
#define EXEC_CHANGE_ARMORMAKE	0X1F
#define EXEC_CHANGE_ACCMAKE		0X20
#define EXEC_CHANGE_POTIONMAKE	0X21
#define EXEC_CHANGE_COOKING		0X22
#define EXEC_SHOW_MAGIC			0X23
#define EXEC_MAKER_OPEN			0X24
#define EXEC_STORAGE_OPEN		0X25
#define EXEC_AUCTION_SELL_OPEN	0X26
#define EXEC_AUCTION_BUY_OPEN	0X27
#define EXEC_SELECT_MSG			0X28
#define EXEC_MESSAGE_OK			0X29
#define EXEC_MESSAGE_NONE		0X2A
#define EXEC_GIVE_GOLD			0X2B
#define EXEC_ROB_DN				0X2C
#define EXEC_RUN_EVENT			0X2D
#define EXEC_ADD_EVENT			0X2E
#define EXEC_DEL_EVENT			0X2F
#define EXEC_RAND_SAY			0X30
#define EXEC_RETURN				0X31
#define EXEC_CHANGE_MAGETYPE	0X32
#define EXEC_GUILD_OPEN			0X33 // 军团名称
#define EXEC_ADD_SKILL			0X80
#define EXEC_GIVE_ITEM_ALL		0X81
/********************************************************/
// 2001.5.7 HyperText... 
#define EXEC_STORE_OPEN			0X03 // 酒捞袍 魄概 NPC
#define EXEC_REPAIR_OPEN		0X34 // 酒捞袍 荐府 NPC 		
#define EXEC_SELL_OPEN			0X35 // 酒捞袍 混 NPC 
#define EXEC_ZONE_MOVE			0X36 // 秦寸 粮栏肺 角青
#define EXEC_MOVE				0X41 // 鞍篮 粮俊辑 捞悼 角青
#define EXEC_FT_MOVE			0X3A // 夸货救栏肺 捞悼	
#define EXEC_FT_PEOPLE			0x3B // 刮缴 荐摹甫 钎泅
#define EXEC_FT_REPAIR			0x3C // 刮缴 荐摹甫 钎泅
#define EXEC_PSI_STORE_OPEN		0X37 // 秦寸 荤捞坷葱 魄概
#define EXEC_ALL_CURE			0X38 // 葛电 HP, PP, SP甫 MAX肺 父惦
#define EXEC_CARE				0X39 // 葛电 惑怕捞惑甫 摹丰
#define EXEC_BANK_OPEN			0x40 // 篮青 芒备甫 楷促. 
#define EXEC_GUILD_MARK			0x42 // 辨靛 巩剧阑 殿废
#define EXEC_GUILD_WAREHOUSE_OPEN 0x43 // 辨靛 芒绊甫 楷促.
#define EXEC_EBODY_OPEN			0x44 // E_Body 惑痢阑 坷锹茄促.	
#define EXEC_UPGRADE_OPEN		0x45
#define EXEC_FIELD_STORE_OPEN	0x46 // 鞘靛惑牢老锭绰 盒扁秦具 窍骨肺..蝶肺 沥狼茄促.
#define EXEC_GUILD_SAY			0x47 // 辨靛俊 加茄 惑牢捞 惑措祈 脚盒俊 蝶扼 盒扁茄促...
#define EXEC_GUILD_TAX			0x48 // 辨付啊 秦寸 惑痢俊 技陛阑 炼沥茄促.
#define EXEC_GUILD_SCHEME		0x49 // 辨付啊 秦寸 惑痢俊 措茄 辨靛傈阑 汲沥茄促.
#define EXEC_GUILD_APPLY		0x4A // 促弗 辨付啊 辨靛傈阑 脚没茄促.
#define EXEC_XP_UP				0x4B // XP甫 棵妨淋
#define EXEC_CHECK_GUILD_USER	0x4C // 立加吝牢 辨靛蜡历甫 焊辰促.	
#define EXEC_CHECK_GUILD_RANK	0x4D // 辨靛 窍快胶盒剧甫 困秦 鉴困甫 舅赴促.
#define EXEC_GUARD_BOW			0x4E // 版厚捍捞 蜡历俊霸 牢荤窍搁辑 捞亥飘 措荤甫 茄促.
#define EXEC_GIVE_EVENT_ITEM	0x4F // 捞亥飘 惑前鼻苞 酒捞袍阑 背券茄促. 
#define EXEC_ADDRESS_WINDOW		0x50 // 惑前鼻 林家 涝仿芒 剁快扁.
#define EXEC_EVENT_ROB_ITEM		0x51 // 惑前鼻 雀荐 捞亥飘
#define EXEC_DRESSING			0x52 // 急堡 芒 剁快扁
#define EXEC_ACCOUNT_BANK_OPEN	0x53 // 拌沥 篮青 凯扁
#define	EXEC_GIVE_DN			0x54 // 蜡历俊霸 捣 林扁
#define EXEC_SEND_MSG			0x55 // 矫胶袍 皋技瘤 焊郴扁
#define EXEC_GIVE_MAGIC_ITEM	0x56 // 蜡历俊霸 概流 酒捞袍阑 霖促. item 抛捞喉俊辑 bySpecial亲格阑 概流可记俊 悸泼窍绊, iq甫 概流栏肺 官层辑 霖促.
#define EXEC_INPUT_WINDOW		0x57 // 抗距啊涝磊俊霸 矫府倔 锅龋 涝仿芒阑... 
#define EXEC_INPUTGAME_WINDOW	0x58 // 抗距啊涝 霸烙规俊霸 矫府倔 锅龋 涝仿芒阑... 	
#define EXEC_ADD_GAMETIME_ITEM	0x59 // 惑前鼻阑 爱绊乐绰 沥咀鼻甫 雀荐茄促.
#define EXEC_CHANGE_UPGRADE_ACC 0x5A // 扁粮狼 咀技辑府甫 诀弊饭捞靛 啊瓷茄 咀技辑府肺 背券秦霖促.
#define EXEC_APPLY_GUILD_RUN	0x5B // 辨靛付扼沛 脚没
#define EXEC_RR_ENTER			0x5C // 肺锯烦喉 涝厘
#define EXEC_MON_SAY			0x5D // 涅胶飘甫困秦 各 say甫... 	
#define EXEC_SUMMON				0x5E // 涅胶飘 各阑 家券茄促.
#define EXEC_EBODY_DEFINE		0x5F // EBODY 犬牢 
#define EXEC_HELPERCURE			0x60 // Helper npc
#define EXEC_GIVE_PSI			0x61 // Gives user PSI
#define EXEC_TRANSFORM			0x62 // lvl 20 quest transform window
#define EXEC_100_1		        0x63 // 100级防具转换
#define EXEC_100_2		        0x64 // 解除属性限制
#define EXEC_100_3		        0x65 // 添加及变更属性
#define EXEC_100_4		        0x66 // 属性升级
#define EXEC_100_5		        0x67 // 防具材料化
#define EXEC_100_6		        0x68 // 解除材料化
#define RUN_SKILL		        0x69 // 解除材料化
#define	EXEC_GIVE_YUANBAO		0x6A // Npc给元宝
#define EXEC_ROB_YUANBAO		0X6B // Npc收元宝
#define EXEC_CHOUJIANG			0X6D // Npc抽奖
#define EXEC_MONEYSHOP			0X6E // 查看带属性的元宝
#define EXEC_EMO				0x6F // 恶魔广场
#define EXEC_ON_WEB				0X70 // 游戏中的网页
#define EXEC_MARK_CZ			0X71 // 在线充值
#define EXEC_NPC_DA				0X72 // 赌博大
#define EXEC_NPC_XIAO			0X73 // 赌博小
#define EXEC_NPC_SHUXING		0X74 // NPC快捷键
#define EXEC_NPC_ZHUANZHI		0X75 // 转换职业对话NPC
#define EXEC_SEND_TOU			0x76 // NPC头顶说话
#define EXEC_NPC_SHANGDIAN		0X77 // 转换职业对话NPC
#define EXEC_MARKHUODONG		0X78 // Mark活动
#define EXEC_MARK_CZ_SAY		0X79 // 在线充值对话框
#define EXEC_NEWGONGNENG		0X7A // 新脚本调用 MarkJay
#define EXEC_DUIHUANITEM		0X7B // 兑换有属性物品
#define EXEC_ADD4PAI			0xC9	//赌博装备
#define EXEC_MARK_ROBITEM		0xC6	//回收装备 小黑修炼
#define EXEC_ADD4PWQ			0xC7	//赌博武器
#define EXEC_ADD4SS				0xC8	//赌博首饰
#define EXEC_ZHANCHANG			0x82 // 电1战场


//	Logic Else Define...
#define LOGIC_LEVEL					0X01
#define LOGIC_EXIST_EVENT			0X02
#define LOGIC_ABSENT_EVENT			0X03
#define LOGIC_EXIST_ITEM			0X04
#define LOGIC_ABSENT_ITEM			0X05
#define LOGIC_CLASS					0x06
#define LOGIC_WEIGHT				0x07
#define LOGIC_CHECK_ITEMSLOT		0x08		
#define LOGIC_EXIST_GM				0x09 // 辨靛付胶磐老 版快 烹苞
#define LOGIC_ZONE_WHO				0x10 // 捞悼且 粮俊 蜡历啊 荤侩吝俊 乐绰瘤 眉农
#define LOGIC_KILL_GH				0x11 // 辨靛窍快胶 鼻府甫 器扁
#define LOGIC_MASS_ZONE_MOVE		0x12 // 
#define LOGIC_TERM					0x13 // 老沥 朝楼 裹困救俊辑 角青茄促.	
#define LOGIC_GUILD_HOUSE_NUMBER	0x14 // 辨靛窍快胶 家蜡辨靛啊 乐绰瘤 眉农茄促.
#define LOGIC_CHECK_CV				0x15 // 矫刮 殿鞭阑 眉农茄促.
#define LOGIC_EXIST_EVENT_ITEM		0X16 // 惑前鼻阑 眉农茄促.
#define LOGIC_MONEY					0X17 // 捣阑 眉农茄促
#define LOGIC_RAND					0X18 // 犬伏阑 倒妨辑 犬伏俊 吧府搁 烹苞
#define LOGIC_OVER_GUILD_USER_COUNT 0X19
#define LOGIC_RR_TIME				0X1A // 肺锯烦喉 涝厘 啊瓷茄 矫埃牢瘤 眉农
#define LOGIC_RR_MAX_USER			0X1B // 肺锯烦喉 弥措 涝厘荐啊 酒流 救灯绰瘤 眉农
#define LOGIC_EXIST_SKILL           0X1C // 百级学习什么技能
#define LOGIC_VIP					0X1D // NPC检测会员
#define LOGIC_YUANBAO				0X1E // NPC查元宝
#define LOGIC_PKDASAI				0X1F // PK大赛系统
//
//	User Define Struct
//
typedef union{
	short int	i;
	BYTE		b[2];
} MYSHORT;

typedef union{
	__int64		i;
	BYTE		b[8];
} MYINT64;


//
//	人物属性
//
#define USER_ABILITY_NUM		5		// 人物属性总值(0~4)

#define USER_STR				0   //力量
#define USER_CON				1   //体质
#define USER_DEX				2   //敏捷
#define USER_VOL				3   //智慧
#define USER_WIS				4   //智力

//
//	User Guild Require
//
#define GUILD_MAKE_LEVEL		50    //建立军团要的等级70
#define GUILD_MAKE_DN			500000    //军团建立钱100000	

//
//	默认出生点
//
#define DEFAULT_START_X			200   //出生初始横向坐标
#define DEFAULT_START_Y			550   //出生初始纵向坐标

//
//	物品颜色属性排数
//
#define MAGIC_NUM				11		// 物品属性总值(0~12)

//
//	物品颜色详细
//
const BYTE	NORMAL_ITEM			= 0;   // 白色普通物品
const BYTE	REMODEL_ITEM		= 1;   // 粉色制作物品
const BYTE	MAGIC_ITEM			= 2;   // 蓝色魔法物品
const BYTE	RARE_ITEM			= 3;   // 金色稀有物品
const BYTE	UNIQUE_ITEM			= 4;   // 淡金唯一物品
const BYTE	SET_ITEM			= 5;   // 绿色唯一物品
const BYTE	UPGRADE_ITEM		= 6;   // 升级用物品  
const BYTE	REMODEL_MAGIC_ITEM	= 8;   // 魔法属性的粉色物品
const BYTE	GUARDIAN_ITEM		= 9;   // 守护者唯一物品
const BYTE	RARE2_ITEM			= 12;  // 百级防具金色物品

const BYTE	EVENT_SP1_ITEM	= 201;		// 捞亥飘甫 困秦 冻绢瘤绰 归拳痢 惑前鼻 酒捞袍	
const BYTE	EVENT_SP2_ITEM	= 202;		// 捞亥飘甫 困秦 冻绢瘤绰 巩拳 惑前鼻 酒捞袍	
const BYTE	EVENT_DEF_ITEM	= 243;		// 捞亥飘甫 困秦 冻绢瘤绰 规绢备 背券 酒捞袍	
const BYTE	EVENT_ATT_ITEM	= 242;		// 捞亥飘甫 困秦 冻绢瘤绰 公扁 背券 酒捞袍	
const BYTE	EVENT_POT_ITEM	= 205;		// 捞亥飘甫 困秦 冻绢瘤绰 拱距 背券 酒捞袍	

const BYTE	EVENT_ITEM_MOON		= 220;	// 焊抚崔
const BYTE	EVENT_ITEM_SONGPEON	= 221;	// 价祈
const BYTE	EVENT_ITEM_BOX		= 222;	// 急拱惑磊

const BYTE	EVENT_ATT7_ITEM	= 240;		// 汲朝 捞亥飘甫困秦 
const BYTE	EVENT_DEF7_ITEM	= 241;		// 
const BYTE	EVENT_ATT6_ITEM	= 244;		// 
const BYTE	EVENT_DEF6_ITEM	= 245;		// 
const BYTE	EVENT_ATT4_ITEM	= 246;		// 
const BYTE	EVENT_DEF4_ITEM	= 247;		// 
const BYTE	EVENT_ATT3_ITEM	= 248;		// 
const BYTE	EVENT_DEF3_ITEM	= 249;		// 
const BYTE  EVENT_INIT_STAT_ITEM = 250;	// 必距
const BYTE  EVENT_USER_GAME_TIME = 252; // 俺牢 沥咀力

#define EVENT_ITEM_BAND		  100		

const BYTE	BREAK_NORMAL	= 100;
const BYTE	BREAK_MAGIC		= 101;
const BYTE	BREAK_REMODEL	= 102;
const BYTE	BREAK_RARE		= 103;
const BYTE	BREAK_UNIQUE	= 104;
const BYTE	BREAK_SET		= 105;

//
//	物品的类型值
//
const BYTE	ATTACK_ITEM				= 1;
const BYTE	MATERIAL_ITEM			= 16; //俺炼 酒捞袍阑 困茄 
const UINT	MATERIAL_MAGIC_UP_ITEM	= 625; //魔石
const UINT	MATERIAL_RARE_UP_ITEM	= 626; //神石
//const UINT	MATERIAL_MAGIC_UP_ITEM	= 985; //超级魔石
//const UINT	MATERIAL_RARE_UP_ITEM	= 986; //超级神石
const UINT	MATERIAL_REMODEL_UP_ITEM	= 861;//圣水晶

//
//	Item Upgrade Cost
//
#define ATTACK_UPGRADE_COST		200000	//	武器升级所需金钱
#define DEFENSE_UPGRADE_COST	100000	//  装备升级所需金钱
#define ACCESSORI_UPGRADE_COST	100000	//	首饰升级所需金钱
#define ITEM_REMODELING_COST	50000	//	Item Remodeling Cost
#define ITEM_REMAGIC_COST		300000	//	Item Remagic Cost魔法圣装钱
#define EBODY_UPGRADE_COST		200000	//	机器升级所需金钱 默认1000000
#define EBODY_IDENTIFY_COST		100000	//	机器鉴定所需金钱
#define	BAIJI_ZHUAN_DN			10000000 //  装备转化所需金钱


//
//	物品升级要求战斗经验
//
#define EBODY_UPGRADE_XP	2000    // 机械身体升级要求战斗经验
#define EBODY_IDENTIFY_XP	100     // 机械身体鉴定要求战斗经验
#define FANGJUZHUANHUABAIJI_XP 200  // 百级装备合成要求战斗经验

#define UPDATE_TICKS		500000

//
//	City Level
//
#define USER_PK				1

const short CHAONISE_RANK	= 0;	//霸王
const short DEATH_RANK		= 1;	//恶人
const short EVIL_RANK		= 2;	//土匪
const short VILLAIN_RANK	= 3;	//强盗
const short CAIN_RANK		= 4;    //坏人

const short COMMOM_RANK		= 5;    //平民

const short CITIZEN_RANK	= 6;	//好人
const short VIUAL_RANK		= 7;	//游侠
const short WARRIOR_RANK	= 8;	//英雄
const short HERO_RANK		= 9;	//领袖
const short SAINT_RANK		= 10;   //圣人

#define CITY_RANK_INTERVAL	5;

#define USER_PK_TICKS		25000	//PK红名时间 MarkJay

#define GUILD_HOUSE_MAX_NUM	7		// 霉锅掳 0篮 力寇, 6俺 辨靛 窍快胶

#define UPDATE_GUILD_INVEN_TIME		12
#define UPDATE_GUILD_CITY_TIME		1

#define SYSTEM_GUILD_BAND	900000

//
// 人物任务值的总长度
//
#define MAX_EVENT_NUM		350
//
//	普通技能最高值 
//
const BYTE	MAX_SKILL_MAGIC_NUM	= 23;
//
//	Item Data Version
//
#define CURRENT_ITEM_VERSION	1

//
//	系统文字的对应
//
#define ITEMLOG_PICKUP					1
#define ITEMLOG_THROW					2
#define ITEMLOG_GIVE					3 //给予物品
#define ITEMLOG_RECEIVE					4 //拒绝给予物品
#define ITEMLOG_EXCHANGE_GIVE			5 //开启给予物品
#define ITEMLOG_EXCHANGE_RECEIVE		6 //拒绝给予
#define ITEMLOG_SELL					7 //卖物品
#define ITEMLOG_BUY						8 //买物品
#define ITEMLOG_ACCOUNT_BANKIN			9 //综合银行放入
#define ITEMLOG_ACCOUNT_BANKOUT			10 //综合银行取出
#define ITEMLOG_GUILD_BANKIN			11 //军团仓库放入
#define ITEMLOG_GUILD_BANKOUT			12 //军团仓库取出
#define ITEMLOG_UPGRADE_SUCCESS			13 //物品升级成功
#define ITEMLOG_UPGRADE_FAIL			14 //物品升级失败

#define ITEMLOG_MONEY_LOTTERY			15
#define ITEMLOG_MONEY_GIVE				16
#define ITEMLOG_MONEY_THROW				17
#define ITEMLOG_MONEY_PICKUP			18
#define ITEMLOG_EXCHANGE_GIVE_MONEY		19
#define ITEMLOG_EXCHANGE_RECEIVE_MONEY	20
#define ITEMLOG_SELL_MONEY				21
#define ITEMLOG_BUY_MONEY				22
#define ITEMLOG_ACCOUNT_BANKIN_MONEY	23
#define ITEMLOG_ACCOUNT_BANKOUT_MONEY	24
#define ITEMLOG_GUILD_BANKIN_MONEY		25
#define ITEMLOG_GUILD_BANKOUT_MONEY		26

#define ITEMLOG_EVENT_GIVE				27

#define ITEMLOG_DEAD_THROW				28

#define ITEMLOG_BLESS_UPGRADE_SUCCESS	29
#define ITEMLOG_BLESS_UPGRADE_FAIL		30

#define ITEMLOG_REMODEL_SUCCESS			31 //个性化成功
#define ITEMLOG_REMODEL_FAIL			32 //个性化失败

#define ITEMLOG_BLESS_USE				33		// 绵 酒捞袍 厚胶公府茄 酒捞袍阑 荤侩且措
#define ITEMLOG_SPECIAL_USE_POTION		34		// 特殊药水使用

#define ITEMLOG_PSI_STONE				35		// 特殊魔法球使用

#define ITEMLOG_ACC_USE					36		// 家葛己 厩技荤府啊 荤扼龙锭
#define ITEMLOG_REMAGIC_DELETE			37		// 府概流 矫胶袍栏肺 酒捞袍 昏力

#define ITEMLOG_DEAD_DELETE				38		// PK 肺 牢秦 酒捞袍捞 荤扼龙锭

#define ITEMLOG_EBODY_ADDER_DELETE		39		// EBody 诀弊饭捞靛肺 俊捞歹甫 家厚茄促.
#define ITEMLOG_EBODY_BASE_DELETE		40		// EBody 诀弊饭捞靛 角菩肺 海捞胶甫 昏力茄促.
#define ITEMLOG_EBODY_UPGRADE_SUCCESS	41		// 机械升级成功

//
//	Royal Rumble Define
//

#define RR_EMPTY						-1
#define RR_IDLE							0
#define RR_APPLY						1
#define RR_ENTERING						2
#define RR_ENTERING_END					3
#define RR_START						4
#define RR_END							5

//
//	to detect Event Hacking
//
#define BUY_BLOCK						1
#define SELL_BLOCK						2
#define REPAIR_BLOCK					3
#define CURE_BLOCK						4
#define TEL_BLOCK						5

#define ITEM_MAX_RANGE					16

// Special Accessori
#define MAX_ACCESSORI		4			// 首饰框4个
#define GUARDIAN_SLOT       1           // 守护框1个
#define	RANDOM_PROTECT		30			// 辑器飘 蓖吧捞狼 农府萍拿 公矫 犬啦


//yskang 0.4 款康磊 肺弊扁废 可记
//0:立加酒捞乔 1:措拳 2:酒捞袍 力傍沥焊 3:PK沥焊
#define CONNECT_IP		0
#define CHAT_LOG		1
#define ITEM_LOG		2
#define	PKINFO_LOG		3
#define CALL_USER_LOG	4

#endif