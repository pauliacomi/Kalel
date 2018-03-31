#ifndef DEFINEDIALOGMESSAGES_H
#define DEFINEDIALOGMESSAGES_H
#pragma once


//*************************************************************************************************************************
//						MESSAGES USED IN DIALOGBOXES
//*************************************************************************************************************************

/***************************************************************
*                           UWM_PP_ADSORPTION_DELETE
* Inputs:
*       WPARAM: ignored, 0
*       LPARAM: INT, static_cast<LPARAM>(position)
* Result: LRESULT
*	Logically void, 0, always
* Effect:
*	Causes the PropertySheet to delete the Adsorption tab 
*	at (position)
***************************************************************/
#define UWM_PP_ADSORPTION_DELETE					(WM_USER + 29)


/***************************************************************
*                           UWM_PP_DESORPTION_DELETE
* Inputs:
*       WPARAM: ignored, 0
*       LPARAM: INT, static_cast<LPARAM>(position)
* Result: LRESULT
*	Logically void, 0, always
* Effect:
*	Causes the PropertySheet to delete the Desorption tab
*	at (position)
***************************************************************/
#define UWM_PP_DESORPTION_DELETE					(WM_USER + 30)

#endif