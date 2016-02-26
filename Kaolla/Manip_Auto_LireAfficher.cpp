#include "StdAfx.h"
#include "Manip_Auto.h"


using namespace std;


void CManip_Auto::LireAfficherBassePression()
{
	LectureBassePression();
	AfficherBassePression();
}

void CManip_Auto::LireAfficherHautePression()
{
	LectureHautePression();
	AfficherHautePression();
}

void CManip_Auto::LireAfficherCalo()
{
	LectureCalo();
	AfficherCalo();
}


void CManip_Auto::AfficherPressionInitiale()
{
	m_KaollaView->m_StrPressionInitiale.Format(_T("%f"),PressionInitiale);
	m_KaollaView->SetDlgItemText(IDC_PRESSION_INITIALE,m_KaollaView->m_StrPressionInitiale);
}

void CManip_Auto::InitialiserAfficherPressionInitiale(double init)
{
	PressionInitiale = init;
	AfficherPressionInitiale();
}

void CManip_Auto::AfficherPressionFinale()
{
	m_KaollaView->m_StrPressionFinale.Format(_T("%f"),PressionFinale);
	m_KaollaView->SetDlgItemText(IDC_PRESSION_FINALE,m_KaollaView->m_StrPressionFinale);
}

void CManip_Auto::InitialiserAfficherPressionFinale(double init)
{
	PressionFinale = init;
	AfficherPressionFinale();
}