#include "stdafx.h"
#include "MFCMessageHandler.h"

// Message definitions
#include "DefinePostMessages.h"


MFCMessageHandler::MFCMessageHandler(/*HWND h*/)
{
}


MFCMessageHandler::~MFCMessageHandler()
{
}

BOOL MFCMessageHandler::SetHandle(HWND h)
{
	// Get view handler and check for validity
	windowHandle = reinterpret_cast<HWND>(h);
	ASSERT(windowHandle != NULL);

	return 0;
}

BOOL MFCMessageHandler::ResetDocumentGraph()
{
	// Post the required message. The view should resolve the message to its parent document
	::PostMessage(windowHandle, WM_GRAPHRESET, NULL, NULL);

	return 0;
}

BOOL MFCMessageHandler::GraphSetTitle(CString title)
{
	/*void CManip::DonneesManuelleGrapheEtape()
	{
		int nb_heures = 2;
		int nb_secondes = nb_heures * 3600;

		int coeff = (int)(temps_manip / nb_secondes);
		float temps_min = coeff * nb_secondes;


		if (true m_Doc->NumeroEtape < coeff + 1)
		{
			ASSERT(0);
			messageHandler.GraphDataAutoStep(temps_min, numero_mesure - 1, coeff + 1);

			CString titre;
			titre.Format(_T("%dh - %dh d'expérience"), nb_heures * coeff, nb_heures*(coeff + 1));
			messageHandler.GraphSetTitle(titre);
		}
	}*/
	return 0;
}

BOOL MFCMessageHandler::GraphAddData(int num, double tps, double calorimeter, double lowPressure, double highPressure, double tempCalo, double tempCage, double tempPiece)
{
	return 0;
}

BOOL MFCMessageHandler::GraphDataAutoStep(float temps, int mesure, int num_etape)
{
	/*m_Doc->TempsMinimum = temps;
	m_Doc->MesureMinimum = mesure - 1;
	m_Doc->NumeroEtape = num_etape;*/
	return 0;
}

BOOL MFCMessageHandler::CancelExperiment()
{
	// Post the required message
	::PostMessage(windowHandle, WM_CANCELEXPERIMENT, NULL, NULL);

	return 0;
}

BOOL MFCMessageHandler::ExchangeData(ExperimentData pParam)
{
	// Create a new instance of the storage class and equate it to the local class
	ExperimentData * newData = new ExperimentData();
	*newData = pParam;
	
	// Post the required message, now the main thread is responsible for deleting the new class
	::PostMessage(windowHandle, WM_EXCHANGEDATA, NULL, (LPARAM)newData);

	return 0;
}

BOOL MFCMessageHandler::UpdateDisplay()
{
	// Post the required message
	::PostMessage(windowHandle, WM_UPDATEDISPLAY, NULL, NULL);

	return 0;
}

BOOL MFCMessageHandler::UnlockMenu()
{
	// Post the required message
	::PostMessage(windowHandle, WM_UNLOCKMENU, NULL, NULL);

	return 0;
}

BOOL MFCMessageHandler::EnableStartButton()
{
	// Post the required message
	::PostMessage(windowHandle, WM_ENABLESTARTBUTTON, NULL, NULL);

	return 0;
}

BOOL MFCMessageHandler::DisplayMeasurement(int pParam)
{
	// Post the required message
	::PostMessage(windowHandle, WM_DISPLAYMEASUREMENT, NULL, NULL);

	return 0;
}




BOOL MFCMessageHandler::DisplayAddMessage(int pParam)
{
	// Post the required message
	::PostMessage(windowHandle, WM_DISPLAYADDMESSAGE, NULL, NULL);

	return 0;
}

BOOL MFCMessageHandler::DisplayStep(int pParam)
{
	// Post the required message
	::PostMessage(windowHandle, WM_DISPLAYSTEP, NULL, NULL);

	return 0;
}

BOOL MFCMessageHandler::DisplaAddStep(CString pParam)
{
	// Post the required message
	::PostMessage(windowHandle, WM_DISPLAYADDSTEP, NULL, NULL);

	return 0;
}



///////////////////////////////////////////////////////////////////////////////////////////////

BOOL MFCMessageHandler::DisplayMessage(int pParam, int pInt1, int pInt2, double pDouble)
{
	// Create a new pointer 
	CString * message = new CString;
	if (pDouble != default_val)
	{
		message->Format(pParam, pDouble);
	}
	else
	{
		if (pInt1 != default_val)
		{
			if (pInt1 != default_val)
			{
				message->Format(pParam, pInt1, pInt2);
			}
			else
			{
				message->Format(pParam, pInt1);
			}
		}
		else
		{
			message->Format(pParam);
		}
	}

	// Other thread is now responsible for deleting this object
	::PostMessage(windowHandle, WM_DISPLAYMESSAGE, NULL, (LPARAM)message);

	return 0;
}

BOOL MFCMessageHandler::DisplayMessageBox(int pParam, UINT nType, float pFloat)
{
	// Create a new pointer 
	CString * message = new CString;
	if (pFloat != default_val)
	{
		message->Format(pParam, pFloat);
	}
	else
	{
		message->Format(pParam);
	}

	// Other thread is now responsible for deleting this object
	::PostMessage(windowHandle, WM_DISPLAYMESSAGEBOX, nType, (LPARAM)message);

	return 0;
}

void MFCMessageHandler::ExperimentStart()
{
	/*messageHandler.DisplayMessage(MESSAGE_FILLLINE);
	messageHandler.DisplayMeasurement(MESSAGE_EXPSTART);*/
}

void MFCMessageHandler::ExperimentEnd()
{
	//// Unlock the menu
	//messageHandler.UnlockMenu();
	//// Enable start button
	//messageHandler.EnableStartButton();
}

