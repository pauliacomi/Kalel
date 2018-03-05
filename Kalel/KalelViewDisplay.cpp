
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
LRESULT CKalelView::DisplayTextboxValues(std::shared_ptr<ExperimentData> data, std::shared_ptr<ExperimentStatus> status)
{

	// Convert to strings
	m_StrCalo.Format(_T("%.9e"), data->GetresultCalorimeter());
	m_StrBassePression.Format(_T("%.6f"), data->GetpressureLow());
	m_StrHautePression.Format(_T("%.6f"), data->GetpressureHigh());
	m_StrTemperatureCalo.Format(_T("%.2f"), data->GettemperatureCalo());
	m_StrTemperatureCage.Format(_T("%.2f"), data->GettemperatureCage());
	m_StrTemperaturePiece.Format(_T("%.2f"), data->GettemperatureRoom());
	m_StrTemps.Format(_T("%.1f"), status->GettimeElapsedSec());
	m_StrPressionInitiale.Format(_T("%.6f"), status->GetpressureInitial());
	m_StrPressionFinale.Format(_T("%.6f"), status->GetpressureFinal());

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

// Display the step
LRESULT CKalelView::DisplayStepProgress(std::shared_ptr<ExperimentStatus> status)
{

	CString temp;

	temp.Format(status->experimentStage);

	m_StrEtape = temp;

	if (status->verificationStep != STEP_VERIFICATIONS_UNDEF && status->verificationStep != STEP_VERIFICATIONS_COMPLETE)
	{
		temp.Format(status->verificationStep);
		m_StrEtape += _T(",   Substage: ") + temp;
	}

	if (status->experimentWaiting == true)
	{
		if (status->timeToEquilibrate / 60 > 1)
		{
			temp.Format(_T(" *** Waiting: %.0f min %.0f s /  %.0f min %.0f s"), floorf(status->timeToEquilibrateCurrent / 60.0f), fmodf(status->timeToEquilibrateCurrent, 60.0f), floorf(status->timeToEquilibrate / 60.0f), fmodf(status->timeToEquilibrate, 60.0f));
		}
		else
		{
			temp.Format(_T(" *** Waiting: %.0f s /  %.0f s"), status->GettimeToEquilibrateCurrent(), status->GettimeToEquilibrate());
		}
		m_StrEtape += temp;
	}

	SetDlgItemText(IDC_EDIT_ETAPE, m_StrEtape);

	return 0;
}