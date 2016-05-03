#include "stdafx.h"
#include "Kaolla.h"

#include "KaollaDoc.h"
#include "KaollaView.h"

#include "ThreadManager.h"


// --------------- Affichages -----------------------------

// Write on the dialog box reserved for new messages
LRESULT CKaollaView::AffichageMessages(WPARAM wParam, LPARAM lParam)
{
	CString message;

	// On rajoute le nouveau message 
	m_StrEditMessages += message;
		SetDlgItemText(IDC_EDIT_MESSAGES,m_StrEditMessages);
	// pEditMessages : le CEdit lié à m_StrEditMessages
	// On écrit dans l'Edit le contenu de m_StrEditMessages
	pEditMessages.GetWindowText(m_StrEditMessages);

	// On met le curseur dans pEditMessages à la fin du contenu de M_StrEditMessages
	// Ce qui permet d'être toujours sur la dernière ligne et voir le dernier message
	pEditMessages.SetSel(m_StrEditMessages.GetLength(),-1);

	return 0;
}

// Temporary overload
LRESULT CKaollaView::AffichageMessages(CString message)
{
	// On rajoute le nouveau message 
	m_StrEditMessages += message;
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
		SetDlgItemText(IDC_EDIT_MESSAGES,StrtmpMessages);
	// pEditMessages : le CEdit lié à m_StrEditMessages
	// On écrit dans l'Edit le contenu de m_StrEditMessages
	pEditMessages.GetWindowText(StrtmpMessages);

	// On met le curseur dans pEditMessages à la fin du contenu de M_StrEditMessages
	// Ce qui permet d'être toujours sur la dernière ligne et voir le dernier message
	pEditMessages.SetSel(StrtmpMessages.GetLength(),-1);

	return 0;
}

// Ecriture sur la boite de dialogue réservée aux mesures
// Adequate à AffichagesMessages
LRESULT CKaollaView::AffichageMesures(WPARAM wParam, LPARAM lParam)
{
	CString mesure;

	m_StrEditMesures += mesure;
	SetDlgItemText(IDC_EDIT_MESURES,m_StrEditMesures);
	pEditMesures.GetWindowText(m_StrEditMesures);
	//UpdateData(FALSE);
	pEditMesures.SetSel(m_StrEditMesures.GetLength(),-1);

	return 0;
}


LRESULT CKaollaView::AffichageEtape(WPARAM wParam, LPARAM lParam)
{

	CString etape;

	m_StrEtape = etape;
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


// ------------ Threads --------------

LRESULT CKaollaView::ExchangeData(WPARAM, LPARAM incomingExperimentData)
{
	// Get the incoming pointer and cast it as a smart pointer
	std::auto_ptr<ExperimentData> tempExpData(reinterpret_cast<ExperimentData*>(incomingExperimentData));
	 
	// Safely copy the data across
	EnterCriticalSection(&experimentData->criticalSection);
	experimentData = tempExpData.get();
	LeaveCriticalSection(&experimentData->criticalSection);

    return 0;
}

void CKaollaView::OnTimer(UINT nIDEvent)
{
	// Safely store
	EnterCriticalSection(&experimentData->criticalSection);
	m_StrCalo.Format(_T("%.2f") ,experimentData->resultCalorimeter);
	m_StrBassePression.Format(_T("%.2f"), experimentData->pressureLow);
	m_StrHautePression.Format(_T("%.2f"), experimentData->pressureHigh);
	m_StrTemperatureCalo.Format(_T("%.2f"), experimentData->temperatureCalo);
	m_StrTemperatureCage.Format(_T("%.2f"), experimentData->temperatureCage);
	m_StrTemperaturePiece.Format(_T("%.2f"), experimentData->temperatureRoom);
	m_StrTemps.Format(_T("%.2f"), experimentData->experimentTime);
	LeaveCriticalSection(&experimentData->criticalSection);

	// Refresh view
	SetDlgItemText(IDC_CALO, m_StrCalo);
	SetDlgItemText(IDC_BASSE_PRESSION, m_StrBassePression);
	SetDlgItemText(IDC_HAUTE_PRESSION, m_StrHautePression);
	SetDlgItemText(IDC_TEMPERATURE_CALO, m_StrTemperatureCalo);
	SetDlgItemText(IDC_TEMPERATURE_CAGE, m_StrTemperatureCage);
	SetDlgItemText(IDC_TEMPERATURE_PIECE, m_StrTemperaturePiece);
	SetDlgItemText(IDC_TEMPS, m_StrTemps);

	GetDocument()->UpdateAllViews(this);

	CFormView::OnTimer(nIDEvent);	// Call base class handler.
}

// ------------ Dialog Box ----

LRESULT CKaollaView::MessageBoxConfirmation(WPARAM wParam, LPARAM lParam)
{
	CString message;
	int nType = 0;

	int result;
	bool continuer = true;
	do{
		result = AfxMessageBox(message, nType);
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