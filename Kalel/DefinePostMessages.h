#pragma once
#ifndef DEFINEPOSTMESSAGES_H
#define DEFINEPOSTMESSAGES_H

// Define windows message for threads
/***************************************************************
*                           WM_THREADFINISHEDREG
* Inputs:
*       WPARAM: ignored, 0
*       LPARAM: RGB value to use for coloring
* Result: LRESULT
*	Logically void, 0, always
* Effect:
*	Causes the view to repaint itself in the specified color
***************************************************************/
#define WM_THREADFINISHEDREG		WM_USER + 7

/***************************************************************
*                           WM_THREADSHUTDOWN
* Inputs:
*       WPARAM: ignored, 0
*       LPARAM: RGB value to use for coloring
* Result: LRESULT
*	Logically void, 0, always
* Effect:
*	Causes the view to repaint itself in the specified color
***************************************************************/
#define WM_THREADSHUTDOWN			WM_USER + 8

/***************************************************************
*                           WM_UPDATEBUTTONS
* Inputs:
*       WPARAM: ignored, 0
*       LPARAM: RGB value to use for coloring
* Result: LRESULT
*	Logically void, 0, always
* Effect:
*	Causes the view to repaint itself in the specified color
***************************************************************/
#define WM_UPDATEBUTTONS			WM_USER + 100

// Calls to display parametrs 
/***************************************************************
*                           WM_UPDATEBUTTONS
* Inputs:
*       WPARAM: ignored, 0
*       LPARAM: RGB value to use for coloring
* Result: LRESULT
*	Logically void, 0, always
* Effect:
*	Causes the view to repaint itself in the specified color
***************************************************************/
#define WM_DISPLAY_PORT_DIALOG						WM_USER + 9
/***************************************************************
*                           WM_UPDATEBUTTONS
* Inputs:
*       WPARAM: ignored, 0
*       LPARAM: RGB value to use for coloring
* Result: LRESULT
*	Logically void, 0, always
* Effect:
*	Causes the view to repaint itself in the specified color
***************************************************************/
#define WM_DISPLAY_APPARATUS_SETTINGS_DIALOG		WM_USER + 10

// Calls to Document

#define WM_DISPLAYMESSAGE			WM_USER + 11
#define WM_DISPLAYADDMESSAGE		WM_USER + 12
#define WM_DISPLAYSTEP				WM_USER + 13
#define WM_DISPLAYADDSTEP			WM_USER + 14
#define WM_DISPLAYPREVIOUSSTEP		WM_USER + 15
#define WM_DISPLAYTEMPERATURES		WM_USER + 16
#define WM_DISPLAYMESSAGEBOX		WM_USER + 17
#define WM_DISPLAYMESSAGEBOXCONF	WM_USER + 18
#define WM_DISPLAYMESSAGEBOXSTOPEX	WM_USER + 19
#define WM_UPDATEDISPLAY			WM_USER + 21
#define WM_UNLOCKMENU				WM_USER + 22
#define WM_ENABLESTARTBUTTON		WM_USER + 23
#define WM_CANCELEXPERIMENT			WM_USER + 24
#define WM_EXCHANGEDATA				WM_USER + 27
#define WM_GRAPHRESET				WM_USER + 28

#define WM_PP_ADSORPTION_DELETE		WM_USER + 29
#define WM_PP_DESORPTION_DELETE		WM_USER + 30

#endif // !DEFINEPOSTMESSAGES_H


