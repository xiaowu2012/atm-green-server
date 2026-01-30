// MarkPay.cpp: implementation of the MarkPay class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "server.h"
#include "MarkPay.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MarkPay::MarkPay()
{
	m_iSid = 0;
	::ZeroMemory(m_iSname, sizeof(m_iSname));
	m_Formal = 0;
	m_flag = 0;
}

MarkPay::~MarkPay()
{

}
