// MarkGonggao.cpp: implementation of the MarkGonggao class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "server.h"
#include "MarkGonggao.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MarkGonggao::MarkGonggao()
{
	::ZeroMemory(m_strGGSay, sizeof(m_strGGSay));
}

MarkGonggao::~MarkGonggao()
{

}
