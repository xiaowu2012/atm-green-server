// MarkGMIni.cpp: implementation of the MarkGMIni class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "server.h"
#include "MarkGMIni.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MarkGMIni::MarkGMIni()
{
	//[GM命令]
	::ZeroMemory(Make, sizeof(Make));//刷装备
	::ZeroMemory(AddGold, sizeof(AddGold));//刷钱
	::ZeroMemory(Make, sizeof(Compound));//合成
	::ZeroMemory(Kick, sizeof(Kick));//踢人
	::ZeroMemory(AllKick, sizeof(AllKick));//全下线
	::ZeroMemory(RecallAround, sizeof(RecallAround));//跟我走
	::ZeroMemory(ObServer, sizeof(ObServer));//隐身
	::ZeroMemory(OUTObServer, sizeof(OUTObServer));//退出隐身
	::ZeroMemory(Tracenpc, sizeof(Tracenpc));//飞到NPC
	::ZeroMemory(Traceuser, sizeof(Traceuser));//飞到玩家
	::ZeroMemory(Link, sizeof(Link));//移动
	::ZeroMemory(HumanCount, sizeof(HumanCount));//查看人数
	::ZeroMemory(Jail, sizeof(Jail));//关进监狱
	::ZeroMemory(UJail, sizeof(UJail));//放人
	::ZeroMemory(PaoEXP, sizeof(PaoEXP));//泡经验开关
	::ZeroMemory(PaoUYB, sizeof(PaoUYB));//泡元宝开关
	::ZeroMemory(PaoSBK, sizeof(PaoSBK));//双倍开关
}

MarkGMIni::~MarkGMIni()
{

}
