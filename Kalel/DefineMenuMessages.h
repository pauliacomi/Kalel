#ifndef DEFINEMENUMESSAGES_H
#define DEFINEMENUMESSAGES_H
#pragma once


//*************************************************************************************************************************
//						INCOMING REQUEST
//*************************************************************************************************************************

/***************************************************************
*                           UWM_UPDATEBUTTONS
* Inputs:
*       WPARAM: ignored, 0
*       LPARAM: INT, 
* Result: LRESULT
*	Logically void, 0, always
* Effect:
*	Causes the view to repaint itself in the specified color
***************************************************************/
#define UWM_DISP_PORT_DIALOG						(WM_USER + 37)
#define UWM_DISP_DEVSETTINGS_DIALOG					(WM_USER + 38)
#define UWM_DISP_CONNECTS_DIALOG					(WM_USER + 39)

#endif