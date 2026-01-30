// VIPPay.cpp: implementation of the VIPPay class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "server.h"
#include "VIPPay.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

VIPPay::VIPPay()
{
	m_iSid = 0;
	::ZeroMemory(m_iSname, sizeof(m_iSname));
	m_Formal = 0;
	m_flag = 0;
}

VIPPay::~VIPPay()
{

}
