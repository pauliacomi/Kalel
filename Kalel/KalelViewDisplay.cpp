
// KalelView_Boutons.cpp : all the code for the valve buttons in the view
//

#include "stdafx.h"
#include "Kalel.h"

#include "KalelDoc.h"
#include "KalelView.h"


// --------------- Displays -----------------------------

// Write on the dialog box reserved for new messages
LRESULT CKalelView::AffichageMessages(WPARAM, LPARAM lParam)
{
	// Get the incoming pointer and cast it as a smart pointer
	std::auto_ptr<CString> message(reinterpret_cast<CString*>(lParam));

	// Add the new message
	m_StrEditMessages += *message;
	m_StrEditMessages += "\r\n";
	SetDlgItemText(IDC_EDIT_MESSAGES,m_StrEditMessages);

	// pEditMessages : le CEdit lié à m_StrEditMessages
	// On écrit dans l'Edit le contenu de m_StrEditMessages
	pEditMessages.GetWindowText(m_StrEditMessages);

	// On met le curseur dans pEditMessages à la fin du contenu de M_StrEditMessages
	// Ce qui permet d'être toujours sur la dernière ligne et voir le dernier message
	pEditMessages.SetSel(m_StrEditMessages.GetLength(),-1);

	return 0;
}


// Write the 
LRESULT CKalelView::DisplayTextboxValues(ExperimentData * data)
{

	// Convert to strings
	m_StrCalo.Format(_T("%.9e"), data->resultCalorimeter);
	m_StrBassePression.Format(_T("%.6f"), data->pressureLow);
	m_StrHautePression.Format(_T("%.6f"), data->pressureHigh);
	m_StrTemperatureCalo.Format(_T("%.2f"), data->temperatureCalo);
	m_StrTemperatureCage.Format(_T("%.2f"), data->temperatureCage);
	m_StrTemperaturePiece.Format(_T("%.2f"), data->temperatureRoom);
	m_StrTemps.Format(_T("%.1f"), data->timeElapsed);
	m_StrPressionInitiale.Format(_T("%.6f"), data->pressureInitial);
	m_StrPressionFinale.Format(_T("%.6f"), data->pressureFinal);

	// Refresh textboxes
	SetDlgItemText(IDC_CALO, m_StrCalo);
	SetDlgItemText(IDC_BASSE_PRESSION, m_StrBassePression);
	SetDlgItemText(IDC_HAUTE_PRESSION, m_StrHautePression);
	SetDlgItemText(IDC_TEMPERATURE_CALO, m_StrTemperatureCalo);
	SetDlgItemText(IDC_TEMPERATURE_CAGE, m_StrTemperatureCage);
	SetDlgItemText(IDC_TEMPERATURE_PIECE, m_StrTemperaturePiece);
	SetDlgItemText(IDC_TEMPS, m_StrTemps);
	SetDlgItemText(IDC_PRESSION_INITIALE, m_StrPressionInitiale);
	SetDlgItemText(IDC_PRESSION_FINALE, m_StrPressionFinale);

	return 0;
}

// Write on the dialog box reserved for new measurements
LRESULT CKalelView::DiplayMeasurements(ExperimentData * data)
{
	CString mesure;

	mesure.Format(_T("Time=%.2f  Calo=%.2f  LP=%.2f  HP=%.2f"), data->timeElapsed, data->resultCalorimeter, data->pressureLow, data->pressureHigh);

	m_StrEditMesures += mesure;
	m_StrEditMesures += "\r\n";
	SetDlgItemText(IDC_EDIT_MESURES,m_StrEditMesures);
	pEditMesures.GetWindowText(m_StrEditMesures);

	pEditMesures.SetSel(m_StrEditMesures.GetLength(),-1);

	return 0;
}

// Display the step
LRESULT CKalelView::DisplayStepProgress(ExperimentData * data)
{

	CString temp;
	
	temp.Format(data->experimentStage);

	m_StrEtape = temp;

	if (data->verificationStep != STEP_VERIFICATIONS_UNDEF && data->verificationStep != STEP_VERIFICATIONS_COMPLETE)
	{
		temp.Format(data->verificationStep);
		m_StrEtape += _T(",   Substage: ") + temp;
	}

	if (data->experimentWaiting == true)
	{
		if (data->timeToEquilibrate / 60 > 1)
		{
			temp.Format(_T(" *** Waiting: %.0f min %.0f s /  %.0f min %.0f s"), floorf(data->timeToEquilibrateCurrent / 60.0f), fmodf(data->timeToEquilibrateCurrent, 60.0f), floorf(data->timeToEquilibrate / 60.0f), fmodf(data->timeToEquilibrate, 60.0f));
		}
		else
		{
			temp.Format(_T(" *** Waiting: %.0f s /  %.0f s"), data->timeToEquilibrateCurrent, data->timeToEquilibrate);
		}
		m_StrEtape += temp;
	}

	SetDlgItemText(IDC_EDIT_ETAPE,m_StrEtape);

	return 0;


	//#define jour_sec 86400
	//#define heure_sec 3600
	//#define minute_sec 60
	//#define message_jour 	0
	//#define message_heure 	1
	//#define message_minute	2
	//#define message_seconde	3

	/*CString Automation::MessageTemps(int duree)
	{
		int TypeMessage;

		if (duree >= jour_sec)
			TypeMessage = message_jour;
		else
			if (duree >= heure_sec)
				TypeMessage = message_heure;
			else
				if (duree >= minute_sec)
					TypeMessage = message_minute;
				else
					TypeMessage = message_seconde;

		CString temps_attente, tmp;
		temps_attente = _T("");
		int duree_restante = duree;
		switch (TypeMessage)
		{
		case message_jour:
			tmp.Format(_T("%dj"), duree_restante / jour_sec);
			duree_restante = duree_restante%jour_sec;
			temps_attente += tmp;
		case message_heure:
			if (TypeMessage == message_heure)
				tmp.Format(_T("%dh"), duree_restante / heure_sec);
			else
				tmp.Format(_T("%02dh"), duree_restante / heure_sec);
			duree_restante = duree_restante%heure_sec;
			temps_attente += tmp;
		case message_minute:
			if (TypeMessage == message_minute)
				tmp.Format(_T("%dmin"), duree_restante / minute_sec);
			else
				tmp.Format(_T("%02dmin"), duree_restante / minute_sec);
			duree_restante = duree_restante%minute_sec;
			temps_attente += tmp;
		case message_seconde:
			if (TypeMessage == message_seconde)
				tmp.Format(_T("%ds"), duree_restante);
			else
				tmp.Format(_T("%02ds"), duree_restante);
			temps_attente += tmp;
		}
		return temps_attente;
	}*/
}



// ------------ Dialog Boxes ----

LRESULT CKalelView::MessageBoxAlert(WPARAM wParam, LPARAM lParam)
{
	// Get the incoming pointer and cast it as a smart pointer
	std::auto_ptr<CString> message(reinterpret_cast<CString*>(lParam));
	std::auto_ptr<UINT> nType(reinterpret_cast<UINT*>(wParam));
	
	return AfxMessageBox(*message, *nType);
}

LRESULT CKalelView::MessageBoxConfirmation(WPARAM wParam, LPARAM lParam)
{
	// Get the incoming pointer and cast it as a smart pointer
	std::auto_ptr<CString> message(reinterpret_cast<CString*>(lParam));
	std::auto_ptr<UINT> nType(reinterpret_cast<UINT*>(wParam));

	int result;
	bool continuer = true;
	do {
		result = AfxMessageBox(*message, *nType);
		if (result == IDCANCEL)
		{
			if (AfxMessageBox(PROMPT_CANCELEXP, MB_YESNO | MB_ICONWARNING, 0) == IDYES) {
				commHandler.ResetClient();
				continuer = false;
			}
		}
		else {
			if (result == IDYES || result == IDOK) {
				commHandler.ResumeClient();
				continuer = false;
			}
			if (result == IDNO) {
				commHandler.ResumeClient();
				commHandler.SetUserContinue();
				continuer = false;
			}
		}
	} while (continuer);
	return result;
}