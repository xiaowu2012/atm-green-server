// LOGIC_ELSE.cpp: implementation of the LOGIC_ELSE class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LOGIC_ELSE.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LOGIC_ELSE::LOGIC_ELSE()
{

}

LOGIC_ELSE::~LOGIC_ELSE()
{

}

void LOGIC_ELSE::Init()
{
	for( int i = 0; i < MAX_LOGIC_ELSE_INT; i++)
	{
		m_LogicElseInt[i] = -1;
	}

	m_LogicElseChar[0] = 0;

	m_bAnd = TRUE;
}

void LOGIC_ELSE::Parse_and(char *pBuf)
{
	int index = 0, i = 0;
	char temp[1024];

	index += ParseSpace( temp, pBuf+index );

	if( !strcmp( temp, "LEVEL" ) )
	{
		m_LogicElse = LOGIC_LEVEL;

		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );
	}
	else if( !strcmp( temp, "VIP" ) )
	{
		m_LogicElse = LOGIC_VIP;
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );
	}
	else if( !strcmp( temp, "PKDASAI" ) )
	{
		m_LogicElse = LOGIC_PKDASAI;
	}
	else if( !strcmp( temp, "EXIST_ITEM" ) )
	{
		m_LogicElse = LOGIC_EXIST_ITEM;

		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );
	}
	else if( !strcmp( temp, "EXIST_EVENT_ITEM" ) )
	{
		m_LogicElse = LOGIC_EXIST_EVENT_ITEM;

		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );
	}
	else if( !strcmp( temp, "ABSENT_ITEM" ) )
	{
		m_LogicElse = LOGIC_ABSENT_ITEM;

		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );
	}
	else if( !strcmp( temp, "EXIST_EVENT" ) )
	{
		m_LogicElse = LOGIC_EXIST_EVENT;

		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );
	}
	else if( !strcmp( temp, "ABSENT_EVENT" ) )
	{
		m_LogicElse = LOGIC_ABSENT_EVENT;

		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );
	}
	else if( !strcmp( temp, "CLASS" ) )
	{
		m_LogicElse = LOGIC_CLASS;

		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );
	}
	else if( !strcmp( temp, "WEIGHT" ) )
	{
		m_LogicElse = LOGIC_WEIGHT;

		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );	// SAY
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );	// Item uid
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );	// No
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );	// Item uid	
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );	// No
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );	// Item uid
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );
	}
	else if( !strcmp( temp, "CHECK_ITEMSLOT" ) )
	{
		m_LogicElse = LOGIC_CHECK_ITEMSLOT;

		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );	// SAY
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );	// Item uid
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );	// No
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );	// Item uid	
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );	// No
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );	// Item uid
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );
	}
	else if( !strcmp( temp, "EXIST_GM" ) )
	{
		m_LogicElse = LOGIC_EXIST_GM;
	}
	else if( !strcmp( temp, "OVER_GUILD_USER_COUNT" ) )
	{
		m_LogicElse = LOGIC_OVER_GUILD_USER_COUNT;

		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );	// guild user count
	}
	else if( !strcmp( temp, "ZONE_WHO" ) )
	{
		m_LogicElse = LOGIC_ZONE_WHO;

		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );	// 해당 존
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );	// 0 : 사람없음면 통과
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );	// 이벤트 존 포인터 접근 타입(이벤트 마다 구조가 달라서 ..)
	}
	else if( !strcmp( temp, "KILL_GH" ) )
	{
		m_LogicElse = LOGIC_KILL_GH;
	}
	else if( !strcmp( temp, "MASS_ZONE_MOVE" ) )
	{
		m_LogicElse = LOGIC_MASS_ZONE_MOVE;

		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );	// 해당 존
	}
	else if( !strcmp( temp, "TERM" ) )
	{
		m_LogicElse = LOGIC_TERM;

		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );	// 시작날짜
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );	// 끝나는 날짜
	}
	else if( !strcmp( temp, "GUILD_HOUSE_NUMBER" ) )
	{
		m_LogicElse = LOGIC_GUILD_HOUSE_NUMBER;

		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );	// 해당 길드하우스 번호
	}
	else if( !strcmp( temp, "CHECK_CV" ) )
	{
		m_LogicElse = LOGIC_CHECK_CV;

		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );	// Min City Value
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );	// Max City Value
	}
	else if( !strcmp( temp, "MONEY" ) )
	{
		m_LogicElse = LOGIC_MONEY;

		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp ); // min
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp ); // max
	}
	else if( !strcmp( temp, "YUANBAO" ) )//NPC꿴璂禱괜
	{
		m_LogicElse = LOGIC_YUANBAO;

		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp ); // min
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp ); // max
	}
	else if( !strcmp( temp, "RAND" ) )
	{
		m_LogicElse = LOGIC_RAND;

		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );
	}
	else if( !strcmp( temp, "RR_TIME" ) )
	{
		m_LogicElse = LOGIC_RR_TIME;
	}
	else if( !strcmp( temp, "RR_MAX_USER" ) )
	{
		m_LogicElse = LOGIC_RR_MAX_USER;
	}else if( !strcmp( temp, "EXIST_SKILL" ) )
	{
		m_LogicElse = LOGIC_EXIST_SKILL;
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );
	}
	m_bAnd = TRUE;
}

void LOGIC_ELSE::Parse_or(char *pBuf)
{
	int index = 0, i = 0;
	char temp[1024];

	index += ParseSpace( temp, pBuf+index );

	if( !strcmp( temp, "LEVEL" ) )
	{
		m_LogicElse = LOGIC_LEVEL;

		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );
	}
	else if( !strcmp( temp, "EXIST_ITEM" ) )
	{
		m_LogicElse = LOGIC_EXIST_ITEM;

		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );
	}
	else if( !strcmp( temp, "EXIST_EVENT_ITEM" ) )
	{
		m_LogicElse = LOGIC_EXIST_EVENT_ITEM;

		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );
	}
	else if( !strcmp( temp, "ABSENT_ITEM" ) )
	{
		m_LogicElse = LOGIC_ABSENT_ITEM;

		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );
	}
	else if( !strcmp( temp, "EXIST_EVENT" ) )
	{
		m_LogicElse = LOGIC_EXIST_EVENT;

		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );
	}
	else if( !strcmp( temp, "ABSENT_EVENT" ) )
	{
		m_LogicElse = LOGIC_ABSENT_EVENT;

		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );
	}
	else if( !strcmp( temp, "WEIGHT" ) )
	{
		m_LogicElse = LOGIC_WEIGHT;

		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );	// SAY
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );	// Item uid
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );	// No
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );	// Item uid	
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );	// No
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );	// Item uid
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );
	}
	else if( !strcmp( temp, "CHECK_ITEMSLOT" ) )
	{
		m_LogicElse = LOGIC_CHECK_ITEMSLOT;

		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );	// SAY
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );	// Item uid
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );	// No
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );	// Item uid	
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );	// No
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );	// Item uid
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );
	}
	else if( !strcmp( temp, "MONEY" ) )
	{
		m_LogicElse = LOGIC_MONEY;

		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp ); // min
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp ); // max
	}
	else if( !strcmp( temp, "YUANBAO" ) )//NPC꿴璂禱괜
	{
		m_LogicElse = LOGIC_YUANBAO;

		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp ); // min
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp ); // max
	}
	else if( !strcmp( temp, "RAND" ) )
	{
		m_LogicElse = LOGIC_RAND;

		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );
	}
	else if( !strcmp( temp, "RR_TIME" ) )
	{
		m_LogicElse = LOGIC_RR_TIME;
	}
	else if( !strcmp( temp, "EXIST_SKILL" ) )
	{
		m_LogicElse = LOGIC_EXIST_SKILL;
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );
	}
	m_bAnd = FALSE;
}
