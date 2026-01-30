// OnlineShop.h: interface for the OnlineShop class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ONLINESHOP_H__8FC7E003_C60F_4477_B80F_9956092FCA11__INCLUDED_)
#define AFX_ONLINESHOP_H__8FC7E003_C60F_4477_B80F_9956092FCA11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class OnlineShop  
{
public:
	OnlineShop();
	virtual ~OnlineShop();
public:
	short m_oSid;
	short m_iSid;
	TCHAR m_iSname[20];
	short	m_sLevel;		
	short	m_sDuration;
	short	m_sBullNum;
	short	m_tIQ;
	DWORD m_price;
	short m_iNum;
	short m_sx1;
	short m_sx2;
	short m_sx3;
	short m_sx4;
	short m_sx5;
	short m_upgrade;
	short m_sx6;
	short m_sx7;
	short m_sx8;
	short m_sx9;
	short m_sx10;
	TCHAR m_iText[50];
	short m_type;
};

#endif // !defined(AFX_ONLINESHOP_H__8FC7E003_C60F_4477_B80F_9956092FCA11__INCLUDED_)
