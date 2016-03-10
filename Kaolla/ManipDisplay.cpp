#include "StdAfx.h"
#include "Manip.h"

// This part of the CManip class deals with displaying everything on the main user interface
// It should be re-written to use commands
 
// ---------- Measurements ----------------

void CManip::AffichageMesures()
{
	char char_resultat_calo[20];
	sprintf_s(char_resultat_calo,"%.8E",resultat_calo);

	ostringstream message;
	message << numero_mesure << ")  temps=" << temps_manip << "  ";
	message << "calo=" << char_resultat_calo << "  bp=" << resultat_bp << "  hp=" << resultat_hp << "  ";
	
	CString rajout_mesure;
	rajout_mesure.Format(_T("%s\r\n"),message.str().c_str());
	
	m_KaollaView->AffichageMesures(rajout_mesure);
}

void CManip::AffichageMesures(CString rajout_mesure)
{
	m_KaollaView->AffichageMesures(rajout_mesure);
}


// --------- Messages -------------------

void CManip::AffichageMessages(CString Message)
{
	m_KaollaView->AffichageMessages(Message);
}

void CManip::RajoutAffichageMessages(CString rajout)
{
	m_KaollaView->RajoutAffichageMessages(rajout);
}

// --------- Etape ------------------

void CManip::AffichageEtape(CString etape)
{
	m_KaollaView->AffichageEtape(etape);
}

void CManip::RajoutAffichageEtape(CString rajout)
{
	m_KaollaView->RajoutAffichageEtape(rajout);
}

void CManip::AncienAffichageEtape()
{
	m_KaollaView->SetDlgItemText(IDC_EDIT_ETAPE,m_KaollaView->m_StrEtape);
}


// -------- Pression, Temperature -------------
void CManip::AfficherCalo()
{
	m_KaollaView->m_StrCalo.Format(_T("%.8E"),resultat_calo);
	m_KaollaView->SetDlgItemText(IDC_CALO,m_KaollaView->m_StrCalo);
}


void CManip::AfficherBassePression()
{
	m_KaollaView->m_StrBassePression.Format(_T("%f"),resultat_bp);
	m_KaollaView->SetDlgItemText(IDC_BASSE_PRESSION,m_KaollaView->m_StrBassePression);
}

void CManip::AfficherHautePression()
{
	m_KaollaView->m_StrHautePression.Format(_T("%f"),resultat_hp);
	m_KaollaView->SetDlgItemText(IDC_HAUTE_PRESSION,m_KaollaView->m_StrHautePression);
}

void CManip::AfficherTemperatures()
{
	m_KaollaView->m_StrTemperatureCalo.Format(_T("%f"),TemperatureCalo);
	m_KaollaView->m_StrTemperatureCage.Format(_T("%f"),TemperatureCage);
	m_KaollaView->m_StrTemperaturePiece.Format(_T("%f"),TemperaturePiece);
	m_KaollaView->SetDlgItemText(IDC_TEMPERATURE_CALO,m_KaollaView->m_StrTemperatureCalo);
	m_KaollaView->SetDlgItemText(IDC_TEMPERATURE_CAGE,m_KaollaView->m_StrTemperatureCage);
	m_KaollaView->SetDlgItemText(IDC_TEMPERATURE_PIECE,m_KaollaView->m_StrTemperaturePiece);
}

// ------- Dialogbox, replace by command ----------

int CManip::MessageBoxConfirmation(CString message, UINT nType)
{
	return m_KaollaView->MessageBoxConfirmation(message,nType);
}