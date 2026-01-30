// VIPPay.h: interface for the MarkPay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIPPAY_H__8FC7E003_C60F_4477_B80F_9956092FCA11__INCLUDED_)
#define AFX_VIPPAY_H__8FC7E003_C60F_4477_B80F_9956092FCA11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class VIPPay  
{
public:
	VIPPay();
	virtual ~VIPPay();
public:
	int m_iSid;
	TCHAR m_iSname[20];
	int m_Formal;
	short m_flag;
};

#endif // !defined(AFX_VIPPAY_H__8FC7E003_C60F_4477_B80F_9956092FCA11__INCLUDED_)
