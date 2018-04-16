
// KalelView_Boutons.cpp : all the code for the valve buttons in the view
//

#include "stdafx.h"
#include "Kalel.h"

#include "KalelDoc.h"
#include "KalelView.h"

#include "../Kalel Shared/timeHelpers.h"


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
LRESULT CKalelView::DisplayTextboxValues(const ExperimentData &data, const ExperimentStatus & status)
{
	// Convert to strings
	m_StrCalo.Format(_T("%.9e"), data.resultCalorimeter);
	m_StrBassePression.Format(_T("%.6f"), data.pressureLow);
	m_StrHautePression.Format(_T("%.6f"), data.pressureHigh);
	m_StrTemperatureCalo.Format(_T("%.2f"), data.temperatureCalo);
	m_StrTemperatureCage.Format(_T("%.2f"), data.temperatureCage);
	m_StrTemperaturePiece.Format(_T("%.2f"), data.temperatureRoom);
	m_StrTemps.Format(_T("%.1f"), std::chrono::duration_cast<std::chrono::seconds>(timeh::NowTime() - timeh::MsToTimePoint(status.timeWaitStart.load())));
	m_StrPressionInitiale.Format(_T("%.6f"), status.pressureInitial.load());
	m_StrPressionFinale.Format(_T("%.6f"), status.pressureFinal.load());

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
LRESULT CKalelView::DisplayStepProgress(const ExperimentStatus &status)
{
	CString substep;

	switch (experimentSettings->experimentType)
	{
	case EXPERIMENT_TYPE_MANUAL:
		m_StrEtape.Format(STR_STAGE_MANUAL);
		break;

	case EXPERIMENT_TYPE_AUTO:
		m_StrEtape.Format(STR_STAGE_AUTO);
		switch (status.mainStage)
		{
		case STAGE_UNDEF:
			substep.Format(STR_STAGE_UNDEF);
			break;
		case STAGE_AUTO_VERIFICATIONS:
			substep.Format(STR_STAGE_AUTO_VERIFICATIONS);
			break;
		case STAGE_AUTO_EQUILIBRATION:
			substep.Format(STR_STAGE_AUTO_EQUILIBRATION);
			break;
		case STAGE_AUTO_ADSORPTION:
			substep.Format(STR_STAGE_AUTO_ADSORPTION);
			break;
		case STAGE_AUTO_DESORPTION:
			substep.Format(STR_STAGE_AUTO_DESORPTION);
			break;
		case STAGE_AUTO_VACUUM_SAMPLE:
			substep.Format(STR_STAGE_VACUUM_SAMPLE);
			break;
		case STAGE_AUTO_END:
			substep.Format(STR_STAGE_AUTO_END);
			break;
		default:
			break;
		}
		break;

	case EXPERIMENT_TYPE_CONTINUOUS:
		m_StrEtape.Format(STR_STAGE_CONT);
		switch (status.mainStage)
		{
		case STAGE_UNDEF:
			substep.Format(STR_STAGE_UNDEF);
			break;
		case STAGE_CONT_VERIFICATIONS:
			substep.Format(STR_STAGE_AUTO_VERIFICATIONS);
			break;
		case STAGE_CONT_FLOWRATE:
			substep.Format(STR_STAGE_CONT_FLOWRATE);
			break;
		case STAGE_CONT_DEADVOL:
			substep.Format(STR_STAGE_CONT_DEADVOL);
			break;
		case STAGE_CONT_EQUILIBRATION:
			substep.Format(STR_STAGE_AUTO_EQUILIBRATION);
			break;
		case STAGE_CONT_ADSORPTION:
			substep.Format(STR_STAGE_CONT_ADSORPTION);
			break;
		case STAGE_CONT_VACUUM_BEFOREEX:
			substep.Format(STR_STAGE_CONT_VACUUM);
			break;
		case STAGE_CONT_END_EQUILIBRATION:
			substep.Format(STR_STAGE_AUTO_EQUILIBRATION);
			break;
		case STAGE_CONT_END:
			break;

		default:
			break;
		}
		break;
	case EXPERIMENT_TYPE_BOTTLE_CHANGE:
		m_StrEtape.Format(STR_STAGE_CHANGE_BOTTLE);
		break;
	case EXPERIMENT_TYPE_BOTTLE_VACUUM:
		m_StrEtape.Format(STR_STAGE_VACUUM_BOTTLE);
		break;
	case EXPERIMENT_TYPE_SAMPLE_VACUUM:
		m_StrEtape.Format(STR_STAGE_VACUUM_SAMPLE);
		break;

	default:
		m_StrEtape.Format(STR_STAGE_UNDEF);
		break;
	}

	if (substep != "")
	{
		m_StrEtape += _T(",   Substage: ") + substep;
	}

	
	if (status.isWaiting)
	{
		auto timeToEquilibrateCurrent = std::chrono::duration_cast<std::chrono::seconds>(timeh::NowTime() - timeh::MsToTimePoint(status.timeWaitStart.load())).count();

		if (status.timeToWait / 60 > 1)
		{
			substep.Format(_T("\n\tWaiting: %.0f min %.0f s /  %.0f min %.0f s"), 
				floorf(timeToEquilibrateCurrent / 60.0f), fmodf(timeToEquilibrateCurrent, 60.0f), 
				floorf(status.timeToWait.load() / 60.0f), fmodf(status.timeToWait.load(), 60.0f));
		}
		else
		{
			substep.Format(_T("\n\tWaiting: %.0f s /  %.0f s"), fmodf(timeToEquilibrateCurrent, 60.0f), status.timeToWait.load());
		}
		m_StrEtape += substep;
	}

	SetDlgItemText(IDC_EDIT_ETAPE, m_StrEtape);

	return 0;
}