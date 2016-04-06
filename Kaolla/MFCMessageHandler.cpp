#include "stdafx.h"
#include "MFCMessageHandler.h"


MFCMessageHandler::MFCMessageHandler(/*HWND h*/)
{
}


MFCMessageHandler::~MFCMessageHandler()
{
}

BOOL MFCMessageHandler::ResetDocumentGraph(LPVOID pParam)
{
	// Get view handler and check for validity
	const HWND hViewFrame = reinterpret_cast<HWND>(pParam);
	ASSERT(hViewFrame != NULL);

	// Post the required message. The view should resolve the message to its parent document
	::PostMessage(hViewFrame, WM_RESETDOCUMENTGRAPH, NULL, NULL);

	return 0;
}

BOOL MFCMessageHandler::DisplayMeasurement(CString pParam)
{
	//m_KaollaView->AffichageMesures(rajout_mesure);
	return 0;
}

BOOL MFCMessageHandler::DisplayMessage(CString pParam)
{
	//m_KaollaView->AffichageMessages(Message);
	return 0;
}

BOOL MFCMessageHandler::DisplayAddMessage(CString pParam)
{
	//m_KaollaView->RajoutAffichageMessages(rajout);
	return 0;
}

BOOL MFCMessageHandler::DisplayStep(CString pParam)
{
	//m_KaollaView->AffichageEtape(etape);
	return 0;
}

BOOL MFCMessageHandler::DisplaAddStep(CString pParam)
{
	//m_KaollaView->RajoutAffichageEtape(rajout);
	return 0;
}

BOOL MFCMessageHandler::DisplayPreviousStep()
{
	//m_KaollaView->SetDlgItemText(IDC_EDIT_ETAPE, m_KaollaView->m_StrEtape);

	return 0;
}

BOOL MFCMessageHandler::DisplayCalorimeter()
{

	/*m_KaollaView->m_StrCalo.Format(_T("%.8E"), resultat_calo);
	m_KaollaView->SetDlgItemText(IDC_CALO, m_KaollaView->m_StrCalo);*/

	return 0;
}

BOOL MFCMessageHandler::DisplayHighPressure()
{
	/*m_KaollaView->m_StrHautePression.Format(_T("%f"), resultat_hp);
	m_KaollaView->SetDlgItemText(IDC_HAUTE_PRESSION, m_KaollaView->m_StrHautePression);*/

	return 0;
}

BOOL MFCMessageHandler::DisplayLowPressure()
{
	
	/*m_KaollaView->m_StrBassePression.Format(_T("%f"), resultat_bp);
	m_KaollaView->SetDlgItemText(IDC_BASSE_PRESSION, m_KaollaView->m_StrBassePression);*/
	return 0;
}

BOOL MFCMessageHandler::DisplayTemperatures()
{

	/*m_KaollaView->m_StrTemperatureCalo.Format(_T("%f"), TemperatureCalo);
	m_KaollaView->m_StrTemperatureCage.Format(_T("%f"), TemperatureCage);
	m_KaollaView->m_StrTemperaturePiece.Format(_T("%f"), TemperaturePiece);
	m_KaollaView->SetDlgItemText(IDC_TEMPERATURE_CALO, m_KaollaView->m_StrTemperatureCalo);
	m_KaollaView->SetDlgItemText(IDC_TEMPERATURE_CAGE, m_KaollaView->m_StrTemperatureCage);
	m_KaollaView->SetDlgItemText(IDC_TEMPERATURE_PIECE, m_KaollaView->m_StrTemperaturePiece);*/
	return 0;
}

BOOL MFCMessageHandler::DisplayMessageBox(CString message, UINT nType)
{
	// Get view handler and check for validity

	//return m_KaollaView->MessageBoxConfirmation(message, nType);
	return 0;
}

