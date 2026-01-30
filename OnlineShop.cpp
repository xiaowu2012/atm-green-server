// OnlineShop.cpp: implementation of the OnlineShop class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "server.h"
#include "OnlineShop.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

OnlineShop::OnlineShop()
{
	m_oSid = 0;
	m_iSid = 0;
	::ZeroMemory(m_iSname, sizeof(m_iSname));
	m_sLevel = 0;		
	m_sDuration = 0;
	m_sBullNum = 0;
	m_tIQ = 0;
	m_price = 0;
	m_iNum = 0;

	m_sx1 = 0;
	m_sx2 = 0;
	m_sx3 = 0;
	m_sx4 = 0;
	m_sx5 = 0;
	m_upgrade = 0;
	m_sx6 = 0;
	m_sx7 = 0;
	m_sx8 = 0;
	m_sx9 = 0;
	m_sx10 = 0;
	::ZeroMemory(m_iText, sizeof(m_iText));
	m_type = 0;
}

OnlineShop::~OnlineShop()
{

}
