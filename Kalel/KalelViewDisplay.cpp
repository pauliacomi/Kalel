#include "stdafx.h"
#include "Kalel.h"

#include "KalelDoc.h"
#include "KalelView.h"


LRESULT CKalelView::ExchangeData(WPARAM, LPARAM incomingExperimentData)
{
	// Get the incoming pointer and cast it as a smart pointer
	std::auto_ptr<ExperimentData> tempExpData(reinterpret_cast<ExperimentData*>(incomingExperimentData));

	// Copy the data across
	experimentData = *tempExpData;

	return 0;
}

// --------------- Displays -----------------------------


void CKalelView::OnTimer(UINT nIDEvent)
{
	// Convert to strings
	m_StrCalo.Format(_T("%.2f"), experimentData.resultCalorimeter);
	m_StrBassePression.Format(_T("%.2f"), experimentData.pressureLow);
	m_StrHautePression.Format(_T("%.2f"), experimentData.pressureHigh);
	m_StrTemperatureCalo.Format(_T("%.2f"), experimentData.temperatureCalo);
	m_StrTemperatureCage.Format(_T("%.2f"), experimentData.temperatureCage);
	m_StrTemperaturePiece.Format(_T("%.2f"), experimentData.temperatureRoom);
	m_StrTemps.Format(_T("%.1f"), experimentData.experimentTime);
	m_StrPressionInitiale.Format(_T("%.1f"), experimentData.pressureInitial);
	m_StrPressionFinale.Format(_T("%.1f"), experimentData.pressureFinal);

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

	// Write the current step
	AffichageEtape(experimentData);

	// Write graph
	bool recorded = GetDocument()->GraphAddMeasurement(experimentData);

	// Write in measurement box
	if (recorded) {
		AffichageMesures(experimentData);
	}

	// Update all views
	GetDocument()->UpdateAllViews(this);

	CFormView::OnTimer(nIDEvent);	// Call base class handler.
}


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


// Write on the dialog box reserved for new measurements
LRESULT CKalelView::AffichageMesures(ExperimentData expD)
{
	CString mesure;

	mesure.Format(_T("Time=%.2f  Calo=%.2f  LP=%.2f  HP=%.2f"), experimentData.experimentTime, experimentData.resultCalorimeter, experimentData.pressureLow, experimentData.pressureHigh);

	m_StrEditMesures += mesure;
	m_StrEditMesures += "\r\n";
	SetDlgItemText(IDC_EDIT_MESURES,m_StrEditMesures);
	pEditMesures.GetWindowText(m_StrEditMesures);

	pEditMesures.SetSel(m_StrEditMesures.GetLength(),-1);

	return 0;
}

// Display the step
LRESULT CKalelView::AffichageEtape(ExperimentData expD)
{

	CString temp;
	
	temp.Format(expD.experimentStage);

	m_StrEtape = temp;

	if (expD.verificationStep != STEP_VERIFICATIONS_UNDEF && expD.verificationStep != STEP_VERIFICATIONS_COMPLETE)
	{
		temp.Format(expD.verificationStep);
		m_StrEtape += ",   Substage: " + temp;
	}

	if (expD.experimentWaiting == true)
	{
		if (expD.timeToEquilibrate / 60 > 1)
		{
			temp.Format(" *** Waiting: %.0f min %.0f s /  %.0f min %.0f s", floorf(expD.timeToEquilibrateCurrent / 60.0f), fmodf(expD.timeToEquilibrateCurrent, 60.0f), floorf(expD.timeToEquilibrate / 60.0f), fmodf(expD.timeToEquilibrate, 60.0f));
		}
		else
		{
			temp.Format(" *** Waiting: %.0f s /  %.0f s", expD.timeToEquilibrateCurrent, expD.timeToEquilibrate);
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


LRESULT CKalelView::RajoutAffichageEtape(WPARAM, LPARAM)
{
	CString rajout;

	CString m_StrRajout = m_StrEtape;
	m_StrRajout += rajout;
	SetDlgItemText(IDC_EDIT_ETAPE,m_StrRajout);

	return 0;
}




// ------------ Dialog Boxes ----

LRESULT CKalelView::MessageBoxAlert(WPARAM wParam, LPARAM lParam)
{
	// Get the incoming pointer and cast it as a smart pointer
	std::auto_ptr<CString> message(reinterpret_cast<CString*>(lParam));
	
	int result;
	bool continuer = true;
	do{
		result = AfxMessageBox(*message, wParam);
		if(result==IDCANCEL || result == IDNO)
		{
			if(AfxMessageBox(PROMPT_RUNNINGEXP, MB_YESNO | MB_ICONWARNING,0)==IDYES)
				continuer=false;
		}
		else
			continuer=false;
		
	}while(continuer);
	return result;
}

LRESULT CKalelView::MessageBoxConfirmation(WPARAM wParam, LPARAM lParam)
{
	// Get the incoming pointer and cast it as a smart pointer
	std::auto_ptr<CString> message(reinterpret_cast<CString*>(lParam));

	int result;
	bool continuer = true;
	do {
		result = AfxMessageBox(*message, wParam);
		if (result == IDCANCEL)
		{
			if (AfxMessageBox(PROMPT_RUNNINGEXP, MB_YESNO | MB_ICONWARNING, 0) == IDYES)
				continuer = false;
		}
		else {
			if (result == IDYES || result == IDOK) {
				threadManager->StartThread();
				continuer = false;
			}
			if (result == IDNO) {
				EnterCriticalSection(&experimentSettings->criticalSection);
				experimentSettings->continueAnyway = true;
				experimentSettings->dataModified = true;
				LeaveCriticalSection(&experimentSettings->criticalSection);
				threadManager->StartThread();
				continuer = false;
			}
		}
	} while (continuer);
	return result;
}










/// sort this out





// Temporary overload
LRESULT CKalelView::AffichageMessages(CString message)
{
	// On rajoute le nouveau message 
	m_StrEditMessages += message;
	m_StrEditMessages += "\r\n";
	SetDlgItemText(IDC_EDIT_MESSAGES, m_StrEditMessages);

	// pEditMessages : le CEdit lié à m_StrEditMessages
	// On écrit dans l'Edit le contenu de m_StrEditMessages
	pEditMessages.GetWindowText(m_StrEditMessages);

	// On met le curseur dans pEditMessages à la fin du contenu de M_StrEditMessages
	// Ce qui permet d'être toujours sur la dernière ligne et voir le dernier message
	pEditMessages.SetSel(m_StrEditMessages.GetLength(), -1);

	return 0;
}

LRESULT CKalelView::RajoutAffichageMessages(WPARAM, LPARAM)
{
	CString rajout;

	// On rajoute le nouveau message 
	CString StrtmpMessages;
	StrtmpMessages += m_StrEditMessages;
	StrtmpMessages += rajout;
	SetDlgItemText(IDC_EDIT_MESSAGES, StrtmpMessages);
	// pEditMessages : le CEdit lié à m_StrEditMessages
	// On écrit dans l'Edit le contenu de m_StrEditMessages
	pEditMessages.GetWindowText(StrtmpMessages);

	// On met le curseur dans pEditMessages à la fin du contenu de M_StrEditMessages
	// Ce qui permet d'être toujours sur la dernière ligne et voir le dernier message
	pEditMessages.SetSel(StrtmpMessages.GetLength(), -1);

	return 0;
}