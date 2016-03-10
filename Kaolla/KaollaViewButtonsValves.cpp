
// KaollaView_Boutons.cpp : all the code for the valve buttons in the view
//

#include "stdafx.h"
#include "Kaolla.h"

#include "KaollaDoc.h"
#include "KaollaView.h"

#include "threads.h"




// Clicking on "open" type buttons
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


// Clicking on "close" type buttons
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


// The other buttons
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



// Function to open a valve from an "open" type button, takes the valve number as argument
void CKaollaView::Ouverture(int i)
{
	// Block the button
	GetDlgItem(idc_ouvrir[i-1])->EnableWindow(FALSE);

	// Create the message string
	CString message;

	// Signal the threading that the valve is to be opened
	if (DemandeOuvertureVanne(i))
	{
		message.Format(TEXT_OPENED);
		MarquerTemoin(i, message);

		// Make the corresponding "close" button of valve i available
		GetDlgItem(idc_fermer[i-1])->EnableWindow(TRUE);
	}
	else
	{
		// Give an error in the text box
		message.Format(ERROR_OPENVALVE,i);
		AffichageMessages(message);
		GetDlgItem(idc_ouvrir[i-1])->EnableWindow(TRUE);
	}
}

// Function to open a valve from an "close" type button, takes the valve number as argument
void CKaollaView::Fermeture(int i)
{
	// Block the button
	GetDlgItem(idc_fermer[i-1])->EnableWindow(FALSE);

	// Create the message string
	CString message;

	// Signal the threading that the valve is to be closed
	if (DemandeFermetureVanne(i))
	{
		message.Format(TEXT_CLOSED);
		MarquerTemoin(i, message);

		// Make the corresponding "open" button of valve i available
		GetDlgItem(idc_ouvrir[i-1])->EnableWindow(TRUE);
	}
	else
	{
		// Give an error in the text box
		message.Format(ERROR_CLOSEVALVE,i);
		AffichageMessages(message);
		GetDlgItem(idc_fermer[i-1])->EnableWindow(TRUE);
	}
}

void CKaollaView::ActivationEV1()
{
	// Block the button
	GetDlgItem(IDC_ACTIVER_EV1)->EnableWindow(FALSE);

	// Create the message string
	CString message;

	// Signal the threading that the valve is to be opened
	if (DemandeActivationEV1())
	{
		message.Format(TEXT_ACTIVATED);
		MarquerEV1(message);

		// Make the corresponding "close" button of valve i available
		GetDlgItem(IDC_DESACTIVER_EV1)->EnableWindow(TRUE);
	}
	else
	{
		// Give an error in the text box
		message.Format(ERROR_OPENEV1);
		AffichageMessages(message);
		GetDlgItem(IDC_ACTIVER_EV1)->EnableWindow(TRUE);
	}
}

void CKaollaView::DesactivationEV1()
{
	// Block the button
	GetDlgItem(IDC_DESACTIVER_EV1)->EnableWindow(FALSE);

	// Create the message string
	CString message;

	// Signal the threading that the valve is to be opened
	if (DemandeDesactivationEV1())
	{
		message.Format(TEXT_DEACTIVATED);
		MarquerEV1(message);

		// Make the corresponding "open" button of Evalve 1 available
		GetDlgItem(IDC_ACTIVER_EV1)->EnableWindow(TRUE);
	}
	else
	{
		// Give an error in the text box
		message.Format(ERROR_CLOSEEV1);
		AffichageMessages(message);
		GetDlgItem(IDC_DESACTIVER_EV1)->EnableWindow(TRUE);
	}
}


void CKaollaView::ActivationEV2()
{
	// Block the button
	GetDlgItem(IDC_ACTIVER_EV2)->EnableWindow(FALSE);

	// Create the message string
	CString message;

	if (DemandeActivationEV2())
	{
		message.Format(TEXT_ACTIVATED);
		MarquerEV2(message);

		// Make the corresponding "closed" button of Evalve 2 available
		GetDlgItem(IDC_DESACTIVER_EV2)->EnableWindow(TRUE);
	}
	else
	{
		// Give an error in the text box
		message.Format(ERROR_OPENEV2);
		AffichageMessages(message);
		GetDlgItem(IDC_ACTIVER_EV2)->EnableWindow(TRUE);
	}
}

void CKaollaView::DesactivationEV2()
{
	// Block the button
	GetDlgItem(IDC_DESACTIVER_EV2)->EnableWindow(FALSE);
	
	// Create the message string
	CString message;

	if (DemandeDesactivationEV2())
	{
		message.Format(TEXT_DEACTIVATED);
		MarquerEV2(message);

		// Make the corresponding "open" button of valve 2 available
		GetDlgItem(IDC_ACTIVER_EV2)->EnableWindow(TRUE);
	}
	else
	{
		// Give an error in the text box
		message.Format(ERROR_CLOSEEV2);
		AffichageMessages(message);
		GetDlgItem(IDC_DESACTIVER_EV2)->EnableWindow(TRUE);
	}
}


void CKaollaView::ActivationPompe()
{
	// Block the button
	GetDlgItem(IDC_ACTIVER_POMPE)->EnableWindow(FALSE);

	// Create the message string
	CString message;

	if (DemandeActivationPompe())
	{
		message.Format(TEXT_ACTIVATED);
		MarquerPompe(message);

		// Make the corresponding "deactivate" button of the pump available
		GetDlgItem(IDC_DESACTIVER_POMPE)->EnableWindow(TRUE);
	}
	else
	{
		// Give an error in the text box
		message.Format(ERROR_ACTIVATEPUMP);
		AffichageMessages(message);
		GetDlgItem(IDC_ACTIVER_POMPE)->EnableWindow(TRUE);
	}
}

void CKaollaView::DesactivationPompe()
{
	// Block the button
	GetDlgItem(IDC_DESACTIVER_POMPE)->EnableWindow(FALSE);

	// Create the message string
	CString message;

	if (DemandeDesactivationPompe())
	{
		message.Format(TEXT_DEACTIVATED);
		MarquerPompe(message);

		// Make the corresponding "activate" button of the pump available
		GetDlgItem(IDC_ACTIVER_POMPE)->EnableWindow(TRUE);
	}
	else
	{
		// Give an error in the text box
		message.Format(ERROR_DEACTIVATEPUMP);
		AffichageMessages(message);
		GetDlgItem(IDC_DESACTIVER_POMPE)->EnableWindow(TRUE);
	}
}


// Mark all the valves as open or closed
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
			CString messageDialog;
			messageDialog.Format(ERROR_PROBLEMVALVES);
			AffichageMessages(messageDialog);
			break;
	}
}

void CKaollaView::MarquerTousLesTemoinsFermes()
{
	// Message for the text boxes
	CString message;
	message.Format(TEXT_CLOSED);

	m_StrTemoinVanne1 = message;
	SetDlgItemText(IDC_TEMOIN_VANNE1,m_StrTemoinVanne1);
	m_StrTemoinVanne2 = message;
	SetDlgItemText(IDC_TEMOIN_VANNE2,m_StrTemoinVanne2);
	m_StrTemoinVanne3 = message;
	SetDlgItemText(IDC_TEMOIN_VANNE3,m_StrTemoinVanne3);
	m_StrTemoinVanne4 = message;
	SetDlgItemText(IDC_TEMOIN_VANNE4,m_StrTemoinVanne4);
	m_StrTemoinVanne5 = message;
	SetDlgItemText(IDC_TEMOIN_VANNE5,m_StrTemoinVanne5);
	m_StrTemoinVanne6 = message;
	SetDlgItemText(IDC_TEMOIN_VANNE6,m_StrTemoinVanne6);
	m_StrTemoinVanne7 = message;
	SetDlgItemText(IDC_TEMOIN_VANNE7,m_StrTemoinVanne7);
	m_StrTemoinVanne8 = message;
	SetDlgItemText(IDC_TEMOIN_VANNE8,m_StrTemoinVanne8);

	for(int i=1;i<=8;i++)
	{
		GetDlgItem(idc_fermer[i-1])->EnableWindow(FALSE);
		GetDlgItem(idc_ouvrir[i-1])->EnableWindow(TRUE);
	}

	// Message for the main dialog
	CString messageDialog;
	messageDialog.Format(TEXT_ALLCLOSED);
	AffichageMessages(messageDialog);
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
	CString message;
	message.Format(TEXT_DEACTIVATED);

	m_StrTemoinEV1 = message;
	SetDlgItemText(IDC_TEMOIN_EV1,m_StrTemoinEV1);

	m_StrTemoinEV2 = message;
	SetDlgItemText(IDC_TEMOIN_EV2,m_StrTemoinEV2);

	m_StrTemoinPompe = message;
	SetDlgItemText(IDC_TEMOIN_POMPE,m_StrTemoinPompe);

	GetDlgItem(IDC_DESACTIVER_EV1)->EnableWindow(FALSE);
	GetDlgItem(IDC_ACTIVER_EV1)->EnableWindow(TRUE);
	GetDlgItem(IDC_DESACTIVER_EV2)->EnableWindow(FALSE);
	GetDlgItem(IDC_ACTIVER_EV2)->EnableWindow(TRUE);
	GetDlgItem(IDC_DESACTIVER_POMPE)->EnableWindow(FALSE);
	GetDlgItem(IDC_ACTIVER_POMPE)->EnableWindow(TRUE);
}

