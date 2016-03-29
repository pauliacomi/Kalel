// MainThread.cpp : implementation file
//

#include "stdafx.h"
#include "Kaolla.h"
#include "MainThread.h"


// MainThread

IMPLEMENT_DYNCREATE(MainThread, CWinThread)

MainThread::MainThread()
{
}

MainThread::~MainThread()
{
}

BOOL MainThread::InitInstance()
{
	m_bAutoDelete = FALSE;

	ShutdownEvent = ::CreateEvent(
		NULL,   // security
		TRUE,   // manual-reset
		FALSE,  // not signaled
		NULL);  // anonymous

	PauseEvent = ::CreateEvent(
		NULL,      // security
		TRUE,      // manual-reset
		TRUE,      // signaled
		NULL);     // anonymous

	return TRUE;
}

int MainThread::ExitInstance()
{
	::CloseHandle(ShutdownEvent);
	::CloseHandle(PauseEvent);

	return CWinThread::ExitInstance();
}

/*********************************************************************
*                         CMyThread::Wait
* Result: DWORD
*       WAIT_OBJECT_0 if shutting down
*       WAIT_OBJECT_0+1 if not paused
*********************************************************************/

DWORD MainThread::Wait()
{
	HANDLE objects[2];
	objects[0] = ShutdownEvent;
	objects[1] = PauseEvent;
	DWORD result = ::WaitForMultipleObjects(2, objects, FALSE, INFINITE);
	switch (result)
	{ /* result */
	case WAIT_TIMEOUT:
		return Timeout;
	case WAIT_OBJECT_0:
		return Shutdown;
	case WAIT_OBJECT_0 + 1:
		return Running;
	default:
		ASSERT(FALSE); // unknown error
		return Error;
	} /* result */
}

/********************************************************************
*                        Shutdown
* Effect:
*        Sets the shutdown event, then waits for the thread to shut
*        down
********************************************************************/
void MainThread::ShutdownThread()
{
	SetEvent(ShutdownEvent);
	::WaitForSingleObject(m_hThread, INFINITE);
}


BEGIN_MESSAGE_MAP(MainThread, CWinThread)
END_MESSAGE_MAP()


// MainThread message handlers
