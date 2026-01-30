#ifndef __SINSTANCE_H__
#define __SINSTANCE_H__

#ifndef __AFXMT_H__
#include "afxmt.h"
#endif

class CInstanceChecker : public CObject
{
public:
	//Constructors / Destructors
	CInstanceChecker(const CString& sMutexName);
	virtual ~CInstanceChecker();
	
	//General functions
	void ActivateChecker();
	BOOL TrackFirstInstanceRunning();
	BOOL PreviousInstanceRunning();
	HWND ActivatePreviousInstance(); 
	
protected:
	//Virtual methods
	virtual CString GetMMFFilename();
	virtual HWND GetWindowToTrack();
	
	//Standard non-virtual methods
	void ReleaseLock();
	
	//Data
	CMutex m_instanceDataMutex;
	CMutex m_executeMutex;
	CSingleLock* m_pExecuteLock;
};


#endif //__SINSTANCE_H__