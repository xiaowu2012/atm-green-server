// MarkChouJiang.h: interface for the MarkChouJiang class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MARKCHOUJIANG_H__8FC7E003_C60F_4477_B80F_9956092FCA11__INCLUDED_)
#define AFX_MARKCHOUJIANG_H__8FC7E003_C60F_4477_B80F_9956092FCA11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class MarkChouJiang  
{
public:
	MarkChouJiang();
	virtual ~MarkChouJiang();
public:
	short m_iSid;
	TCHAR m_iSname[16];
	short m_iNum;
	short m_upgrade;
	short m_sx1;
	short m_sx2;
	short m_sx3;
	short m_sx4;
	short m_sx5;
	short m_sx6;
	short m_sx7;
	short m_sx8;
	short m_sx9;
	short m_sx10;
};

#endif // !defined(AFX_MARKCHOUJIANG_H__8FC7E003_C60F_4477_B80F_9956092FCA11__INCLUDED_)
