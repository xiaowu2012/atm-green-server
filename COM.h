///////////////////////////////////////////////////////////////////
//	COM.h : header file for the COM class
//  Copyright (C) 2000,  Jin-Soo Jeon
//
//	If this code works, it was written by JinSoo Jeon.
//	If not, I don't know who wrote it.
//

#if !defined(AFX_COM_H__5110736A_69A5_48DB_99D2_7E58E24261C2__INCLUDED_)
#define AFX_COM_H__5110736A_69A5_48DB_99D2_7E58E24261C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Map.h"
#include "ThrowItem.h"
#include "Store.h"
#include "GuildFortress.h"
#include "mcommon.h"
#include <list>
#include "BufferEx.h"

class USER;
class Item;

typedef struct _SEND_DATA
{
	BOOL	used;
	BYTE	flag;
	int		len;
	int		uid;
	int		x;
	int		y;
	int		z;
	int		zone_index;
	CRect	rect;
	char	pBuf[SEND_BUF_SIZE];
} SEND_DATA;

typedef CTypedPtrArray <CPtrArray, SEND_DATA*> SendDataArray;

class COM  
{
public:
	COM();
	virtual ~COM();

	BOOL Init(int port);

// IKING 2001.1.
public:
	USER* GetUserUid(int uid);
	CWinThread *m_hTimerThread;
//

// variable
public:
	BOOL				m_bInit;

	HANDLE				m_hSendIOCP;

	//SendDataArray		m_arSendData;
	//SendDataArray		m_arSendFree;

	CRITICAL_SECTION	m_critGuild;
	CRITICAL_SECTION	m_critEvent;							// 烙矫肺 捞亥飘甫 困秦 敬促.

	// IKING 2001.1.
	CRITICAL_SECTION	m_critThrowItem;
	//

	CThrowItem*			m_ThrowItemArray[MAX_THROW_ITEM];		// 酒捞袍 

	LONG				m_ThrowAddIndex;
	DWORD				m_dLastItemDelete;
	CThrowItem*			m_ThrowItemUse;//已被使用物品队列
	//CThrowItem*	        
	CThrowItem*			m_ThrowItemUnuse;//未被使用用的物品队列


	int					m_TotalUserCount;

	CString				m_strIP;

	// New Send Thread - alisia
	CPtrList			SendDataList[AUTOMATA_THREAD+1];
	CRITICAL_SECTION	m_critSendData[AUTOMATA_THREAD+1];
	int					nSendDataCount[AUTOMATA_THREAD+1];

protected:
	DWORD				m_dwConcurrency;

	// Function
public:
	void AnnounceZone(TCHAR* pBuf, BYTE sysType, int iZone);
	void SetNpcToViolenceFortressWarEnd(CGuildFortress *pFort);
	void SetFortressViolenceTime(CGuildFortress *pFort, int iGuild, int iType);
	void CheckViolenceInFortress();
	void UpdateMemFortressWarType(CGuildFortress *pFort);
	void SetFortressWarTime(CGuildFortress *pFort);

	void LoadFortressCityDegree(CGuildFortress *pFort);
	void UpdateFortressMem2DB(CGuildFortress *pFort);
	void UpdateFortress2DB(CGuildFortress *pFort);
	void RemoveUserInGuildHouse();
	void SendFortressWarEnd(CGuildFortress *pFort);
	void SendFortressWarBegin(CGuildFortress *pFort);
	void ReUpdateFortress2DB(CGuildFortress*pFort);
	void CheckMessageTime();//温馨提示公告
	void CheckEmoTime();//恶魔广场在线公告
	void CheckBaoZangTime();//禁止PK洞在线公告
	int	Add_DGB_Massage(TCHAR *pBuf, int iLen);//添一个电光板消息
	void Check_DGB_Massage();//处理电光板队列中消息
	void UpdateLiXianTime();//给与离线状态 MarkJay
	void SetPKDaSai();//PK大赛
	void SubQLPKDASAITime();
	void SubPKJiangLi(char* StrName,int DN);
	BOOL GetUserPaiMing();//头顶排行 MarkJay
	BOOL DeleteAllBanIP();//清空白名单IP
	void GetUserPKDaSai();
	void GetUserPKEnd();
	void GetTSDay();//更新守护天使天数
	void GetVIPDay();//更新VIP天数
	void GetFUExpTime();//更新天使双倍时间
	void CheckFortressWarTime();
	void UpdateUserCount();
	USER *GetUser(TCHAR* id);
	void Announce(TCHAR *pBuf, BYTE sysType);
	void SendAll(TCHAR *pBuf, int nLength);
	void CheckGuildWarSchedule(CStore *pStore);
	void SendGuildWarEnd(CStore *pStore);
	void SendGuildWarBegin(CStore *pStore);
	void CheckGuildWarTime();
	void DelThrowItem();
	BOOL SetThrowItem(ItemList *pItem, int x, int y,  int z);
	void DeleteThrowItemArray();
	void InitThrowItemArray();
	void Send(USER *pUser, TCHAR* pBuf, int nLength);
	void Send(SEND_DATA *pData);
	void GetCPUCount();
	void SendTimeToAll();
	void SetWeatherZone(int iZone, BYTE tType, BYTE tOnOff = 0);
	BOOL CreateTimerThread();
	BOOL CreateEventTimerThread(void);
	bool ThrowItemAdd(ItemList *pItem, int x, int y, int z);
	void ThrowItemDel(CThrowItem *Del);
	CThrowItem * ItemGetID();
	void ItemUseInset(CThrowItem *Add);
	void ThrowItemRefresh();
	void SendItemFieldInfo(BYTE type, ItemList *pItem, int x, int y,int z);
	void SendInsight(TCHAR *pBuf, int nLength,int x,int y,int z);

protected:		
	void UpdateStoreMem2DB(CStore* pStore);
	void DeleteAll();
	CRITICAL_SECTION m_DGB_CS; 
	std::list<CBufferEx *>   m_DGB_MessageQueue;
};

typedef struct	_WEATHER_INFO
{
	int		iZone;
	BOOL	bRaining;
}WEATHER_INFO; 


#endif // !defined(AFX_COM_H__5110736A_69A5_48DB_99D2_7E58E24261C2__INCLUDED_)
