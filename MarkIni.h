// MarkIni.h: interface for the MarkIni class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MarkINI_H__91E00755_5610_4254_B6F1_F1A31A9E8E1F__INCLUDED_)
#define AFX_MarkINI_H__91E00755_5610_4254_B6F1_F1A31A9E8E1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class MarkIni  
{
public:
	MarkIni();
	virtual ~MarkIni();

	//[服务器]
	int TJ_JGTJ;//最高等级
	int JG_DX;//加点最高值
	int ioExp; //初始经验倍数
	int idwXP; //战斗经验
	TCHAR GameName[100];//游戏名字
	int KG130; //130技能
	//[在线充值模式]
	int CZ_YZB_YB;
	//[商城重叠开关]
	int ShopKG;
	//[小号保护相差等级]
	int XHBH_DJ;
	//[群攻开关]
	int QunGong_KG;//群攻开关
	//[评分开关]
	int PingFen_KG;//评分开关
	//[3倍人数开关]
	int RenShu_KG;//3倍人数开关
	//GM刷物品数量
	int ShuaWuPin_Num;//刷物品数量
	int Shop_Qh_KG;//商店模式切换开关
	int Wg_HP_KG;//疯狂加血开关
	int Wl_EBODYSkill;//物理必杀伤害倍率
	int Fs_EBODYSkill;//法师必杀伤害倍率
	TCHAR WEBURL[100]; //在线web
	//[赌博]
	int WQBZSL;//吃金标给标志数量
	int JJWQSL;//吃金标给标志数量
	int WQCG;//吃金标给标志数量
	int WQSB;//吃金标给标志数量
	int ZHBZSL;//吃金标给标志数量
	int JJBZSL;//吃金标给标志数量
	int SSSJBZSL;//吃金标给标志数量
	int SSJJBZSL;//吃金标给标志数量
	int ZBCG;//吃金标给标志数量
	int ZBSB;//吃金标给标志数量
	int SSCG;//吃金标给标志数量
	int SSSB;//吃金标给标志数量
	//[怪物数量限制]
	int GuaiWuNum_150_155;
	int GuaiWuNum_155_160;
	//[套装盒子]
	int ITEM_GM_KG;
	//[经验]
	int Exp_Kz_Kg;	//经验控制开关
	int iNpcExp_50;//50级以下的经验倍数32
	int iNpcExp_60;//60级以下的经验倍数16
	int iNpcExp_70;//70级以下的经验倍数8
	int iNpcExp_80;//80级以下的经验倍数4
	int iNpcExp_90;//90级以下的经验倍数2
	int iNpcExp_99;//99级以下的经验倍数2
	int iNpcExp_100;//100级以下的经验倍数
	int iNpcExp_104;//104级以下的经验倍数
	int iNpcExp_105;//105级以下的经验倍数
	int iNpcExp_106;//106级以下的经验倍数
	int iNpcExp_107;//107级以下的经验倍数
	int iNpcExp_108;//108级以下的经验倍数
	int iNpcExp_109;//109级以下的经验倍数
	int iNpcExp_110;//110级以下的经验倍数
	int iNpcExp_115;//115级以下的经验倍数
	int iNpcExp_120;//120级以后的经验倍数
	int iNpcExp_125;//125级以后的经验倍数
	int iNpcExp_130;//130级以后的经验倍数
	int iNpcExp_140;//140级以后的经验倍数
	int iNpcExp_150;//150级以后的经验倍数
	int iNpcExp_155;//155级以后的经验倍数
	int iNpcExp_160;//160级以后的经验倍数
	//[怪物]
	int JXSX_KG;//锆石垃圾属性开关
	int BOSSHS_KG;//锆石垃圾属性开关
	int iJydjxz;//经验等级限制，相差多少级内才有经验
	int iGwdbxz;//怪物掉宝等级限制
	int iZdJyxz;//获得战斗经验的等级限制
	int m_zDLPS;//掉宝属性排数
	int m_zSXCD;//掉宝属性重叠
	int XY_LSDB;//掉宝属性重叠
	int XY_HSDB;//掉宝属性重叠
	int FXY_LSDB;//掉宝属性重叠
	int FXY_HSDB;//掉宝属性重叠
	int GW_BaoLv;
	//[抗转魔]
	int KZM_KG; //抗转魔开关
	int KZM_BFB; //抗转魔百分比
	//[物理攻击速度]
	int AttackDelay;
	//在线泡元宝
	int PYB_KG; //泡元宝开关
	int PYB_DJ; //泡元宝等级
	int BASIC_ADD_YB_TIME;//泡元宝 相隔时间 单位秒
	int PYB_ShuLiang; //泡元宝数量
	//[在线泡经验] 
	int EXP_KG;//泡经验开关
	//[在线泡决战币]
	int PJB_KG;//泡决战币开关
	int PJB_DJ;//泡决战币等级
	int BASIC_ADD_JB_TIME;//泡决战币 相隔时间 单位秒
	int PJB_ShuLiang;//泡决战币数量
	//[交易密码开关]
	int JYMM_KG;//交易密码开关
	//[开关] 
	int SBEXP_KG; //双倍经验开关
	int GJXY_KG;//眩晕开关
	int DLGL_KG;//聊天过滤开关
	int JXRW_KG;//机械任务开关
	int BSJMF_KG;//80级魔法任务开关
	int DQXSDM_KG;//丢钱显示开关
	int PKDLXS_KG;//PK掉落显示地面开关
	int CH_lV_KG;//称号显示等级开关
	//[恶魔广场开关]
	int EM_ZDKG;//恶魔广场自动开关
	int EM_KG;//恶魔广场开关
	//[PK大赛]
	int PK_ZDKG; //PK大赛自动开关
	int PK_DSKG; //PK大赛开关
	int PK_JIANG; //PK大赛杀人奖励
	//[回收]
	int JG_SIPAIJ;//四排价格
	int JG_BAIGAO;//白锆价格
	//[军团]
	int JT_YSTS; //要塞相隔天数
	int JT_Fortressid;//要塞团图标
	//[召唤]
	int ZH_KQhead;//召唤怪物前ID
	int ZH_KQfoot;//召唤怪物后ID
};

#endif // !defined(AFX_MarkINI_H__91E00755_5610_4254_B6F1_F1A31A9E8E1F__INCLUDED_)
