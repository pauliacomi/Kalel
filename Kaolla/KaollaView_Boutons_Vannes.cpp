#include "stdafx.h"
#include "Kaolla.h"

#include "KaollaDoc.h"
#include "KaollaView.h"

#include "threads.h"


using namespace std;


// Lorsqu'on appuie sur un des boutons du type 'Ouvrir'
void CKaollaView::OnBnClickedOuvrir1()
{	Ouverture(1);	}

void CKaollaView::OnBnClickedOuvrir2()
{	Ouverture(2);	}

void CKaollaView::OnBnClickedOuvrir3()
{	Ouverture(3);	}

void CKaollaView::OnBnClickedOuvrir4()
{	Ouverture(4);	}

void CKaollaView::OnBnClickedOuvrir5()
{	Ouverture(5);	}

void CKaollaView::OnBnClickedOuvrir6()
{	Ouverture(6);	}

void CKaollaView::OnBnClickedOuvrir7()
{	Ouverture(7);	}

void CKaollaView::OnBnClickedOuvrir8()
{	Ouverture(8);	}


// Lorsqu'on appuie sur un des boutons du type 'Fermer'
void CKaollaView::OnBnClickedFermer1()
{	Fermeture(1);	}

void CKaollaView::OnBnClickedFermer2()
{	Fermeture(2);	}

void CKaollaView::OnBnClickedFermer3()
{	Fermeture(3);	}

void CKaollaView::OnBnClickedFermer4()
{	Fermeture(4);	}

void CKaollaView::OnBnClickedFermer5()
{	Fermeture(5);	}

void CKaollaView::OnBnClickedFermer6()
{	Fermeture(6);	}

void CKaollaView::OnBnClickedFermer7()
{	Fermeture(7);	}

void CKaollaView::OnBnClickedFermer8()
{	Fermeture(8);	}



void CKaollaView::OnBnClickedActiverEV1()
{	ActivationEV1();	}

void CKaollaView::OnBnClickedActiverEV2()
{	ActivationEV2();	}

void CKaollaView::OnBnClickedActiverPompe()
{	ActivationPompe();	}

void CKaollaView::OnBnClickedDesactiverEV1()
{	DesactivationEV1();	}

void CKaollaView::OnBnClickedDesactiverEV2()
{	DesactivationEV2();	}

void CKaollaView::OnBnClickedDesactiverPompe()
{	DesactivationPompe();	}



// fonction lancée par le clic sur le bouton 'Ouvrir' de la vanne i
void CKaollaView::Ouverture(int i)
{
	// On bloque ce bouton
	GetDlgItem(idc_ouvrir[i-1])->EnableWindow(FALSE);

	if (DemandeOuvertureVanne(i))
	{
		MarquerTemoin(i,_T("Ouverte"));

		// On rend accessible le bouton 'Fermer' de la vanne i
		GetDlgItem(idc_fermer[i-1])->EnableWindow(TRUE);
	}
	else
	{
		// On écrit sur la boite de dialogue pour signaler une erreur
		CString message;
		message.Format(_T("Erreur, Impossible d'ouvrir la vanne %d\r\n"),i);
		AffichageMessages(message);
		GetDlgItem(idc_ouvrir[i-1])->EnableWindow(TRUE);
	}
}

// fonction lancée par le clic sur le bouton 'Fermer' de la vanne i
void CKaollaView::Fermeture(int i)
{
	// On bloque ce bouton
	GetDlgItem(idc_fermer[i-1])->EnableWindow(FALSE);

	if (DemandeFermetureVanne(i))
	{
		MarquerTemoin(i,_T("Fermée"));

		// On rend accessible le bouton 'Ouvrir' de la vanne i
		GetDlgItem(idc_ouvrir[i-1])->EnableWindow(TRUE);
	}
	else
	{
		// On écrit sur la boite de dialogue pour signaler une erreur
		CString message;
		message.Format(_T("Erreur, Impossible de fermer la vanne %d\r\n"),i);
		AffichageMessages(message);
		GetDlgItem(idc_fermer[i-1])->EnableWindow(TRUE);
	}
}

void CKaollaView::ActivationEV1()
{
	// On bloque ce bouton
	GetDlgItem(IDC_ACTIVER_EV1)->EnableWindow(FALSE);

	if (DemandeActivationEV1())
	{
		MarquerEV1(_T("Activée"));

		// On rend accessible le bouton 'Fermer' de la vanne i
		GetDlgItem(IDC_DESACTIVER_EV1)->EnableWindow(TRUE);
	}
	else
	{
		// On écrit sur la boite de dialogue pour signaler une erreur
		AffichageMessages(_T("Erreur, Impossible d'activer la valve 1\r\n"));
		GetDlgItem(IDC_ACTIVER_EV1)->EnableWindow(TRUE);
	}
}

void CKaollaView::DesactivationEV1()
{
	// On bloque ce bouton
	GetDlgItem(IDC_DESACTIVER_EV1)->EnableWindow(FALSE);

	if (DemandeDesactivationEV1())
	{
		MarquerEV1(_T("Désactivée"));

		// On rend accessible le bouton 'Ouvrir' de la vanne i
		GetDlgItem(IDC_ACTIVER_EV1)->EnableWindow(TRUE);
	}
	else
	{
		// On écrit sur la boite de dialogue pour signaler une erreur
		AffichageMessages(_T("Erreur, Impossible de désactiver la valve 1\r\n"));
		GetDlgItem(IDC_DESACTIVER_EV1)->EnableWindow(TRUE);
	}
}


void CKaollaView::ActivationEV2()
{
	// On bloque ce bouton
	GetDlgItem(IDC_ACTIVER_EV2)->EnableWindow(FALSE);

	if (DemandeActivationEV2())
	{
		MarquerEV2(_T("Activée"));

		// On rend accessible le bouton 'Fermer' de la vanne i
		GetDlgItem(IDC_DESACTIVER_EV2)->EnableWindow(TRUE);
	}
	else
	{
		// On écrit sur la boite de dialogue pour signaler une erreur
		AffichageMessages(_T("Erreur, Impossible d'activer la valve 1\r\n"));
		GetDlgItem(IDC_ACTIVER_EV2)->EnableWindow(TRUE);
	}
}

void CKaollaView::DesactivationEV2()
{
	// On bloque ce bouton
	GetDlgItem(IDC_DESACTIVER_EV2)->EnableWindow(FALSE);

	if (DemandeDesactivationEV2())
	{
		MarquerEV2(_T("Désactivée"));

		// On rend accessible le bouton 'Ouvrir' de la vanne i
		GetDlgItem(IDC_ACTIVER_EV2)->EnableWindow(TRUE);
	}
	else
	{
		// On écrit sur la boite de dialogue pour signaler une erreur
		AffichageMessages(_T("Erreur, Impossible de désactiver la valve 2\r\n"));
		GetDlgItem(IDC_DESACTIVER_EV2)->EnableWindow(TRUE);
	}
}


void CKaollaView::ActivationPompe()
{
	// On bloque ce bouton
	GetDlgItem(IDC_ACTIVER_POMPE)->EnableWindow(FALSE);

	if (DemandeActivationPompe())
	{
		MarquerPompe(_T("Activée"));

		// On rend accessible le bouton 'Fermer' de la vanne i
		GetDlgItem(IDC_DESACTIVER_POMPE)->EnableWindow(TRUE);
	}
	else
	{
		// On écrit sur la boite de dialogue pour signaler une erreur
		AffichageMessages(_T("Erreur, Impossible d'activer la pompe\r\n"));
		GetDlgItem(IDC_ACTIVER_POMPE)->EnableWindow(TRUE);
	}
}

void CKaollaView::DesactivationPompe()
{
	// On bloque ce bouton
	GetDlgItem(IDC_DESACTIVER_POMPE)->EnableWindow(FALSE);

	if (DemandeDesactivationPompe())
	{
		MarquerPompe(_T("Désactivée"));

		// On rend accessible le bouton 'Ouvrir' de la vanne i
		GetDlgItem(IDC_ACTIVER_POMPE)->EnableWindow(TRUE);
	}
	else
	{
		// On écrit sur la boite de dialogue pour signaler une erreur
		AffichageMessages(_T("Erreur, Impossible de désactiver la valve 1\r\n"));
		GetDlgItem(IDC_DESACTIVER_POMPE)->EnableWindow(TRUE);
	}
}



void CKaollaView::MarquerTemoin(int num_vanne, CString message)
{
	switch(num_vanne)
	{
		case 1:
			m_StrTemoinVanne1 = message;
			SetDlgItemText(IDC_TEMOIN_VANNE1,m_StrTemoinVanne1);
			break;
		case 2:
			m_StrTemoinVanne2 = message;
			SetDlgItemText(IDC_TEMOIN_VANNE2,m_StrTemoinVanne2);
			break;
		case 3:
			m_StrTemoinVanne3 = message;
			SetDlgItemText(IDC_TEMOIN_VANNE3,m_StrTemoinVanne3);
			break;
		case 4:
			m_StrTemoinVanne4 = message;
			SetDlgItemText(IDC_TEMOIN_VANNE4,m_StrTemoinVanne4);
			break;
		case 5:
			m_StrTemoinVanne5 = message;
			SetDlgItemText(IDC_TEMOIN_VANNE5,m_StrTemoinVanne5);
			break;
		case 6:
			m_StrTemoinVanne6 = message;
			SetDlgItemText(IDC_TEMOIN_VANNE6,m_StrTemoinVanne6);
			break;
		case 7:
			m_StrTemoinVanne7 = message;
			SetDlgItemText(IDC_TEMOIN_VANNE7,m_StrTemoinVanne7);
			break;
		case 8:
			m_StrTemoinVanne8 = message;
			SetDlgItemText(IDC_TEMOIN_VANNE8,m_StrTemoinVanne8);
			break;
		default:
			AffichageMessages(_T("Erreur avec les vannes\r\n"));
			break;
	}

}

void CKaollaView::MarquerTousLesTemoinsFermes()
{
	m_StrTemoinVanne1 = _T("Fermée");
	SetDlgItemText(IDC_TEMOIN_VANNE1,m_StrTemoinVanne1);
	m_StrTemoinVanne2 = _T("Fermée");
	SetDlgItemText(IDC_TEMOIN_VANNE2,m_StrTemoinVanne2);
	m_StrTemoinVanne3 = _T("Fermée");
	SetDlgItemText(IDC_TEMOIN_VANNE3,m_StrTemoinVanne3);
	m_StrTemoinVanne4 = _T("Fermée");
	SetDlgItemText(IDC_TEMOIN_VANNE4,m_StrTemoinVanne4);
	m_StrTemoinVanne5 = _T("Fermée");
	SetDlgItemText(IDC_TEMOIN_VANNE5,m_StrTemoinVanne5);
	m_StrTemoinVanne6 = _T("Fermée");
	SetDlgItemText(IDC_TEMOIN_VANNE6,m_StrTemoinVanne6);
	m_StrTemoinVanne7 = _T("Fermée");
	SetDlgItemText(IDC_TEMOIN_VANNE7,m_StrTemoinVanne7);
	m_StrTemoinVanne8 = _T("Fermée");
	SetDlgItemText(IDC_TEMOIN_VANNE8,m_StrTemoinVanne8);

	for(int i=1;i<=8;i++)
	{
		GetDlgItem(idc_fermer[i-1])->EnableWindow(FALSE);
		GetDlgItem(idc_ouvrir[i-1])->EnableWindow(TRUE);
	}


}

void CKaollaView::MarquerEV1(CString message)
{
	m_StrTemoinEV1 = message;
	SetDlgItemText(IDC_TEMOIN_EV1,m_StrTemoinEV1);
}

void CKaollaView::MarquerEV2(CString message)
{
	m_StrTemoinEV2 = message;
	SetDlgItemText(IDC_TEMOIN_EV2,m_StrTemoinEV2);
}

void CKaollaView::MarquerPompe(CString message)
{
	m_StrTemoinPompe = message;
	SetDlgItemText(IDC_TEMOIN_POMPE,m_StrTemoinPompe);
}

void CKaollaView::MarquerValvesEtPompeDesactivees()
{
	m_StrTemoinEV1 = _T("Désactivée");
	SetDlgItemText(IDC_TEMOIN_EV1,m_StrTemoinEV1);

	m_StrTemoinEV2 = _T("Désactivée");
	SetDlgItemText(IDC_TEMOIN_EV2,m_StrTemoinEV2);

	m_StrTemoinPompe = _T("Désactivée");
	SetDlgItemText(IDC_TEMOIN_POMPE,m_StrTemoinPompe);

	GetDlgItem(IDC_DESACTIVER_EV1)->EnableWindow(FALSE);
	GetDlgItem(IDC_ACTIVER_EV1)->EnableWindow(TRUE);
	GetDlgItem(IDC_DESACTIVER_EV2)->EnableWindow(FALSE);
	GetDlgItem(IDC_ACTIVER_EV2)->EnableWindow(TRUE);
	GetDlgItem(IDC_DESACTIVER_POMPE)->EnableWindow(FALSE);
	GetDlgItem(IDC_ACTIVER_POMPE)->EnableWindow(TRUE);

}

