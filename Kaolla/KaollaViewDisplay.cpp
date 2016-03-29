#include "stdafx.h"
#include "Kaolla.h"

#include "KaollaDoc.h"
#include "KaollaView.h"

#include "ThreadManager.h"


// --------------- Affichages -----------------------------

// Write on the dialog box reserved for new messages
void CKaollaView::AffichageMessages(CString message)
{
	// On rajoute le nouveau message 
	m_StrEditMessages += message;
		SetDlgItemText(IDC_EDIT_MESSAGES,m_StrEditMessages);
	// pEditMessages : le CEdit lié à m_StrEditMessages
	// On écrit dans l'Edit le contenu de m_StrEditMessages
	pEditMessages.GetWindowText(m_StrEditMessages);

	// On met le curseur dans pEditMessages à la fin du contenu de M_StrEditMessages
	// Ce qui permet d'être toujours sur la dernière ligne et voir le dernier message
	pEditMessages.SetSel(m_StrEditMessages.GetLength(),-1);
}

void CKaollaView::RajoutAffichageMessages(CString rajout)
{
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
}

// Ecriture sur la boite de dialogue réservée aux mesures
// Adequate à AffichagesMessages
void CKaollaView::AffichageMesures(CString mesure)
{
	m_StrEditMesures += mesure;
	SetDlgItemText(IDC_EDIT_MESURES,m_StrEditMesures);
	pEditMesures.GetWindowText(m_StrEditMesures);
	//UpdateData(FALSE);
	pEditMesures.SetSel(m_StrEditMesures.GetLength(),-1);
}


void CKaollaView::AffichageEtape(CString etape)
{
	m_StrEtape = etape;
	SetDlgItemText(IDC_EDIT_ETAPE,m_StrEtape);
}


void CKaollaView::RajoutAffichageEtape(CString rajout)
{
	CString m_StrRajout = m_StrEtape;
	m_StrRajout += rajout;
	SetDlgItemText(IDC_EDIT_ETAPE,m_StrRajout);
}


// ------------ Threads --------------

LRESULT CKaollaView::OnThreadAffichage(WPARAM, LPARAM)
{	
	// On affiche les résultats
	SetDlgItemText(IDC_CALO,m_StrCalo);
	SetDlgItemText(IDC_BASSE_PRESSION,m_StrBassePression);
	SetDlgItemText(IDC_HAUTE_PRESSION,m_StrHautePression);
	SetDlgItemText(IDC_TEMPERATURE_CALO,m_StrTemperatureCalo);
	SetDlgItemText(IDC_TEMPERATURE_CAGE,m_StrTemperatureCage);
	SetDlgItemText(IDC_TEMPERATURE_PIECE,m_StrTemperaturePiece);
	SetDlgItemText(IDC_TEMPS,m_StrTemps);


	GetDocument()->UpdateAllViews(this); 

	// Si arrêt de l'expérience
	m_mainDocument = CKaollaDoc::GetDocument();
	if(!m_mainDocument->experiment_running)
	{
		FinAffichageMesure();
	}

    return 0;
}

// ------------ Dialog Box ----

int CKaollaView::MessageBoxConfirmation(CString message, UINT nType)
{
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