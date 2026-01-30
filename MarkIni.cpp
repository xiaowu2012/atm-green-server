// MarkIni.cpp: implementation of the MarkIni class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "server.h"
#include "MarkIni.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MarkIni::MarkIni()
{
	//[服务器]
	int TJ_JGTJ = 0;//最高等级
	int JG_DX = 0;//加点最高值
	int ioExp = 0; //初始经验倍数
	int idwXP = 0; //战斗经验
	::ZeroMemory(GameName, sizeof(GameName));//游戏名字
	int KG130 = 0; //130技能
	//[在线充值模式]
	int CZ_YZB_YB = 0;
	//[商城重叠开关]
	int ShopKG = 0;
	//[小号保护相差等级]
	int XHBH_DJ = 0;
	//[群攻开关]
	int QunGong_KG = 0;//群攻开关
	//[评分开关]
	int PingFen_KG = 0;//评分开关
	//[3倍人数开关]
	int RenShu_KG = 0;//3倍人数开关
	//GM刷物品数量
	int ShuaWuPin_Num = 0;//刷物品数量
	int Shop_Qh_KG = 0;//商店模式切换开关
	int Wg_HP_KG = 0;//疯狂加血开关
	int Wl_EBODYSkill = 0;//物理必杀伤害倍率
	int Fs_EBODYSkill = 0;//法师必杀伤害倍率
	::ZeroMemory(WEBURL, sizeof(WEBURL));
	//[赌博]
	int WQBZSL = 0;//吃金标给标志数量
	int JJWQSL = 0;//吃金标给标志数量
	int WQCG = 0;//吃金标给标志数量
	int WQSB = 0;//吃金标给标志数量
	int ZHBZSL = 0;//吃金标给标志数量
	int JJBZSL = 0;//吃金标给标志数量
	int SSSJBZSL = 0;//吃金标给标志数量
	int SSJJBZSL = 0;//吃金标给标志数量
	int ZBCG = 0;//吃金标给标志数量
	int ZBSB = 0;//吃金标给标志数量
	int SSCG = 0;//吃金标给标志数量
	int SSSB = 0;//吃金标给标志数量
	//[怪物数量限制]
	int GuaiWuNum_150_155 = 0;
	int GuaiWuNum_155_160 = 0;
	//[套装盒子]
	int ITEM_GM_KG = 0;
	//[经验]
	int Exp_Kz_Kg = 0;	//经验控制开关
    int iNpcExp_50 = 0;//50级以下的经验倍数32
    int iNpcExp_60 = 0;//60级以下的经验倍数16
    int iNpcExp_70 = 0;//70级以下的经验倍数8
    int iNpcExp_80 = 0;//80级以下的经验倍数4
    int iNpcExp_90 = 0;//90级以下的经验倍数2
    int iNpcExp_99 = 0;//99级以下的经验倍数2
    int iNpcExp_100 = 0;//100级以下的经验倍数
	int iNpcExp_104 = 0;//104级以下的经验倍数
	int iNpcExp_105 = 0;//105级以下的经验倍数
	int iNpcExp_106 = 0;//106级以下的经验倍数
	int iNpcExp_107 = 0;//107级以下的经验倍数
	int iNpcExp_108 = 0;//108级以下的经验倍数
    int iNpcExp_109 = 0;//109级以下的经验倍数
    int iNpcExp_110 = 0;//110级以下的经验倍数
	int iNpcExp_115 = 0;//115级以下的经验倍数
    int iNpcExp_120 = 0;//120级以后的经验倍数
	int iNpcExp_125 = 0;//125级以后的经验倍数
	int iNpcExp_130 = 0;//130级以后的经验倍数
	int iNpcExp_140 = 0;//140级以后的经验倍数
	int iNpcExp_150 = 0;//150级以后的经验倍数
	int iNpcExp_155 = 0;//155级以后的经验倍数
	int iNpcExp_160 = 0;//160级以后的经验倍数
	//[怪物]
	int JXSX_KG = 0;//锆石垃圾属性开关
	int BOSSHS_KG = 0;//BOSS锆石垃圾属性开关
	int iJydjxz = 0;//经验等级限制，相差多少级内才有经验，默认是40
	int iGwdbxz = 0;//怪物掉宝等级限制，默认是50
	int iZdJyxz = 0;//获得战斗经验的等级限制，默认是40
	int m_zDLPS = 3;//掉宝属性排数
	int m_zSXCD = 1;//掉宝属性重叠
	int XY_LSDB = 1;//掉宝属性重叠
	int XY_HSDB = 1;//掉宝属性重叠
	int FXY_LSDB = 1;//掉宝属性重叠
	int FXY_HSDB = 1;//掉宝属性重叠
	int GW_BaoLv = 2;//掉宝属性重叠
	//[抗转魔]
	int KZM_KG = 0; //抗转魔开关
	int KZM_BFB = 0; //抗转魔百分比
	//[物理攻击速度]
	int AttackDelay = 0;
	//在线泡元宝
	int PYB_KG = 0; //泡元宝开关
	int PYB_DJ = 0; //泡元宝等级
	int BASIC_ADD_YB_TIME = 0;//泡元宝 相隔时间 单位秒
	int PYB_ShuLiang = 0; //泡元宝数量
	//[在线泡经验] 
	int EXP_KG = 0;//泡经验开关
	//[在线泡决战币]
	int PJB_KG = 0;//泡决战币开关
	int PJB_DJ = 0;//泡决战币等级
	int BASIC_ADD_JB_TIME = 0;//泡决战币 相隔时间 单位秒
	int PJB_ShuLiang = 0;//泡决战币数量
	//[交易密码开关]
	int JYMM_KG = 0;//交易密码开关
	//[开关] 
	int SBEXP_KG = 0;//双倍经验开关
	int GJXY_KG = 0;//眩晕开关
	int DLGL_KG = 0;//聊天过滤开关
	int JXRW_KG = 0;//机械任务开关
	int BSJMF_KG = 0;//80级魔法任务开关
	int DQXSDM_KG = 0;//丢钱显示开关
	int PKDLXS_KG = 0;//PK掉落显示地面开关
	int CH_lV_KG = 0;//称号显示等级开关
	//[恶魔广场开关]
	int EM_ZDKG = 0;//恶魔广场自动开关
	int EM_KG = 0;//恶魔广场开关
	//[PK大赛]
	int PK_ZDKG = 0; //PK大赛自动开关
	int PK_DSKG = 0; //PK大赛开关
	int PK_JIANG = 0; //PK大赛杀人奖励
	//[回收]
	int JG_SIPAIJ = 0;//四排价格
	int JG_BAIGAO = 0;//白锆价格
	//[军团]
	int JT_YSTS = 0; //要塞相隔天数
	int JT_Fortressid = 1000;//要塞团图标
	//[召唤]
	int ZH_KQhead = 0;//召唤怪物前ID
	int ZH_KQfoot = 0;//召唤怪物后ID
}

MarkIni::~MarkIni()
{

}
