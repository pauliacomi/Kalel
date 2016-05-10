#include "stdafx.h"
#include "Kaolla.h"

#include "KaollaDoc.h"
#include "KaollaView.h"

#include "Define_Stages.h"	// Here we find the names of the stages


LRESULT CKaollaView::ExchangeData(WPARAM, LPARAM incomingExperimentData)
{
	// Get the incoming pointer and cast it as a smart pointer
	std::auto_ptr<ExperimentData> tempExpData(reinterpret_cast<ExperimentData*>(incomingExperimentData));

	// Copy the data across
	experimentData = *tempExpData;

	return 0;
}

// --------------- Displays -----------------------------


void CKaollaView::OnTimer(UINT nIDEvent)
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
LRESULT CKaollaView::AffichageMessages(WPARAM wParam, LPARAM lParam)
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
LRESULT CKaollaView::AffichageMesures(ExperimentData expD)
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
LRESULT CKaollaView::AffichageEtape(ExperimentData expD)
{

	CString temp;
	
	temp.Format(expD.experimentStage);

	m_StrEtape = "Status: " + temp;

	if (expD.verificationStep != STEP_VERIFICATIONS_UNDEF && expD.verificationStep != STEP_VERIFICATIONS_COMPLETE)
	{
		temp.Format(expD.verificationStep);
		m_StrEtape += " *** Substage: " + temp;
	}

	if (expD.experimentWaiting == true)
	{
		temp.Format(" *** Waiting: %.0f / %.0f", expD.timeToEquilibrateCurrent ,expD.timeToEquilibrate);
		m_StrEtape += temp;
	}

	SetDlgItemText(IDC_EDIT_ETAPE,m_StrEtape);

	return 0;
}


LRESULT CKaollaView::RajoutAffichageEtape(WPARAM wParam, LPARAM lParam)
{
	CString rajout;

	CString m_StrRajout = m_StrEtape;
	m_StrRajout += rajout;
	SetDlgItemText(IDC_EDIT_ETAPE,m_StrRajout);

	return 0;
}




// ------------ Dialog Boxes ----

LRESULT CKaollaView::MessageBoxAlert(WPARAM wParam, LPARAM lParam)
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

LRESULT CKaollaView::MessageBoxConfirmation(WPARAM wParam, LPARAM lParam)
{
	// Get the incoming pointer and cast it as a smart pointer
	std::auto_ptr<CString> message(reinterpret_cast<CString*>(lParam));

	int result;
	bool continuer = true;
	do {
		result = AfxMessageBox(*message, wParam);
		if (result == IDCANCEL || result == IDNO)
		{
			if (AfxMessageBox(PROMPT_RUNNINGEXP, MB_YESNO | MB_ICONWARNING, 0) == IDYES)
				continuer = false;
		}
		else {
			if (result == IDYES || result == IDOK) {
				threadManager->StartThread();
				continuer = false;
			}
		}
	} while (continuer);
	return result;
}










/// sort this out





// Temporary overload
LRESULT CKaollaView::AffichageMessages(CString message)
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

LRESULT CKaollaView::RajoutAffichageMessages(WPARAM wParam, LPARAM lParam)
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