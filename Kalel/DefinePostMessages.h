#ifndef DEFINEPOSTMESSAGES_H
#define DEFINEPOSTMESSAGES_H
#pragma once


//*************************************************************************************************************************
//						SERVER REQUESTS
//*************************************************************************************************************************

/***************************************************************
*                           UWM_THREADSHUTDOWN
* Inputs:
*       WPARAM: ignored, 0
*       LPARAM: RGB value to use for coloring
* Result: LRESULT
*	Logically void, 0, always
* Effect:
*	Causes the view to repaint itself in the specified color
***************************************************************/
#define UWM_THREAD_STOP								(WM_USER + 31)
#define UWM_THREAD_START							(WM_USER + 32)
#define UWM_THREAD_RESTART							(WM_USER + 33)
#define UWM_FUNC_VACUUM_SAMPLE						(WM_USER + 34)
#define UWM_FUNC_VACUUM_BOTTLE						(WM_USER + 35)
#define UWM_FUNC_CHANGE_BOTTLE						(WM_USER + 36)



//*************************************************************************************************************************
//						SERVER CALLBACKS
//*************************************************************************************************************************

#define UWM_SIGNAL_SERVER_CONNECTED					(WM_USER + 40)
#define UWM_SIGNAL_SERVER_DISCONNECTED				(WM_USER + 45)
#define UWM_EXCHANGE_MACHINESETTINGS				(WM_USER + 41)
#define UWM_EXCHANGE_EXPERIMENTSETTINGS				(WM_USER + 43)
#define UWM_EXCHANGEDATA							(WM_USER + 27)
#define UWM_EXCHANGELOGS							(WM_USER + 28)
#define UWM_EXCHANGESTATE							(WM_USER + 29)
#define UWM_EXCHANGESTATESPECIFIC					(WM_USER + 30)
#define UWM_SET_MACHINESETTINGS						(WM_USER + 44)
#define UWM_SYNCED									(WM_USER + 42)
#define UWM_CANCELEXPERIMENT						(WM_USER + 24)
#define UWM_DISPLAYMESSAGE							(WM_USER + 11)
#define UWM_DISPLAYMESSAGEBOX						(WM_USER + 17)
#define UWM_DISPLAYMESSAGEBOXCONF					(WM_USER + 18)
#define UWM_AUTOEXPFINISHED							(WM_USER + 7)


#endif