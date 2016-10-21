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
#define UWM_GOT_MACHINE_SETTINGS					(WM_USER + 41)
#define UWM_EXCHANGEDATA							(WM_USER + 27)
#define UWM_SYNCED									(WM_USER + 42)
#define UWM_UPDATEBUTTONS							(WM_USER + 100)

#define UWM_CANCELEXPERIMENT						(WM_USER + 24)

#define UWM_DISPLAYMESSAGE							(WM_USER + 11)
#define UWM_DISPLAYMESSAGEBOX						(WM_USER + 17)
#define UWM_DISPLAYMESSAGEBOXCONF					(WM_USER + 18)
#define UWM_THREADFINISHEDREG						(WM_USER + 7)


//*************************************************************************************************************************
//						SERVER CALLBACKS
//*************************************************************************************************************************




#define UWM_THREADSHUTDOWN							(WM_USER + 8)
#endif // !DEFINEPOSTMESSAGES_H


