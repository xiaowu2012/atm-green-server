// MarkGMIni.h: interface for the MarkGMIni class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MarkGMIni_H__91E00755_5610_4254_B6F1_F1A31A9E8E1F__INCLUDED_)
#define AFX_MarkGMIni_H__91E00755_5610_4254_B6F1_F1A31A9E8E1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class MarkGMIni  
{
public:
	MarkGMIni();
	virtual ~MarkGMIni();

	//[GM命令]
	TCHAR Make[100];//刷装备
	TCHAR AddGold[100];//刷钱
	TCHAR Compound[100];//合成
	TCHAR Kick[100];//踢人
	TCHAR AllKick[100];//全下线
	TCHAR RecallAround[100];//跟我走
	TCHAR ObServer[100];//隐身
	TCHAR OUTObServer[100];//退出隐身
	TCHAR Tracenpc[100];//飞到NPC
	TCHAR Traceuser[100];//飞到玩家
	TCHAR Link[100];//移动
	TCHAR HumanCount[100];//查看人数
	TCHAR Jail[100];//关进监狱
	TCHAR UJail[100];//放人
	TCHAR PaoEXP[100];//泡经验开关
	TCHAR PaoUYB[100];//泡元宝开关
	TCHAR PaoSBK[100];//双倍开关
};

#endif // !defined(AFX_MarkGMIni_H__91E00755_5610_4254_B6F1_F1A31A9E8E1F__INCLUDED_)
