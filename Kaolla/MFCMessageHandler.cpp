#include "stdafx.h"
#include "MFCMessageHandler.h"

// Message definitions
#include "DefinePostMessages.h"

MFCMessageHandler::MFCMessageHandler(/*HWND h*/)
{
}


MFCMessageHandler::~MFCMessageHandler()
{
}

BOOL MFCMessageHandler::SetHandle(HWND h)
{
	// Get view handler and check for validity
	windowHandle = reinterpret_cast<HWND>(h);
	ASSERT(windowHandle != NULL);

	return 0;
}

BOOL MFCMessageHandler::ResetDocumentGraph()
{
	// Post the required message. The view should resolve the message to its parent document
	::PostMessage(windowHandle, WM_RESETDOCUMENTGRAPH, NULL, NULL);

	return 0;
}

BOOL MFCMessageHandler::CancelExperiment()
{
	// Post the required message
	::PostMessage(windowHandle, WM_CANCELEXPERIMENT, NULL, NULL);

	return 0;
}

BOOL MFCMessageHandler::DisplayInitialPressure()
{
	// Post the required message
	::PostMessage(windowHandle, WM_DISPLAYINITIALPRESSURE, NULL, NULL);
/*
	m_KaollaView->m_StrPressionInitiale.Format(_T("%f"), PressionInitiale);
	m_KaollaView->SetDlgItemText(IDC_PRESSION_INITIALE, m_KaollaView->m_StrPressionInitiale);*/
	return 0;
}

BOOL MFCMessageHandler::DisplayFinalPressure()
{
	// Post the required message
	::PostMessage(windowHandle, WM_DISPLAYFINALPRESSURE, NULL, NULL);
/*
	m_KaollaView->m_StrPressionFinale.Format(_T("%f"), PressionFinale);
	m_KaollaView->SetDlgItemText(IDC_PRESSION_FINALE, m_KaollaView->m_StrPressionFinale);*/

	return 0;
}

BOOL MFCMessageHandler::ExchangeData()
{
	// Post the required message
	::PostMessage(windowHandle, WM_EXCHANGEDATA, NULL, NULL);
	/*
	m_KaollaView->m_StrCalo.Format(_T("%.8E"), resultat_calo);
	m_KaollaView->m_StrBassePression.Format(_T("%f"), resultat_bp);
	m_KaollaView->m_StrHautePression.Format(_T("%f"), resultat_hp);
	m_KaollaView->m_StrTemperatureCalo.Format(_T("%f"), TemperatureCalo);
	m_KaollaView->m_StrTemperatureCage.Format(_T("%f"), TemperatureCage);
	m_KaollaView->m_StrTemperaturePiece.Format(_T("%f"), TemperaturePiece);
	m_KaollaView->m_StrTemps.Format(_T("%.2f"), temps_manip);*/

	return 0;
}

BOOL MFCMessageHandler::UpdateDisplay()
{
	// Post the required message
	::PostMessage(windowHandle, WM_UPDATEDISPLAY, NULL, NULL);

	return 0;
}

BOOL MFCMessageHandler::UnlockMenu()
{
	// Post the required message
	::PostMessage(windowHandle, WM_UNLOCKMENU, NULL, NULL);

	return 0;
}

BOOL MFCMessageHandler::EnableStartButton()
{
	// Post the required message
	::PostMessage(windowHandle, WM_ENABLESTARTBUTTON, NULL, NULL);

	return 0;
}

BOOL MFCMessageHandler::DisplayMeasurement(CString pParam)
{
	// Post the required message
	::PostMessage(windowHandle, WM_DISPLAYMEASUREMENT, NULL, NULL);

	return 0;
}

BOOL MFCMessageHandler::DisplayMessage(CString pParam)
{
	// Post the required message
	::PostMessage(windowHandle, WM_DISPLAYMESSAGE, NULL, NULL);

	return 0;
}

BOOL MFCMessageHandler::DisplayAddMessage(CString pParam)
{
	// Post the required message
	::PostMessage(windowHandle, WM_DISPLAYADDMESSAGE, NULL, NULL);

	return 0;
}

BOOL MFCMessageHandler::DisplayStep(CString pParam)
{
	// Post the required message
	::PostMessage(windowHandle, WM_DISPLAYSTEP, NULL, NULL);

	return 0;
}

BOOL MFCMessageHandler::DisplaAddStep(CString pParam)
{
	// Post the required message
	::PostMessage(windowHandle, WM_DISPLAYADDSTEP, NULL, NULL);

	return 0;
}

BOOL MFCMessageHandler::DisplayPreviousStep()
{
	// Post the required message
	::PostMessage(windowHandle, WM_DISPLAYPREVIOUSSTEP, NULL, NULL);

	//m_KaollaView->SetDlgItemText(IDC_EDIT_ETAPE, m_KaollaView->m_StrEtape);

	return 0;
}

BOOL MFCMessageHandler::DisplayCalorimeter()
{
	// Post the required message
	::PostMessage(windowHandle, WM_DISPLAYCALORIMETER, NULL, NULL);

	/*m_KaollaView->m_StrCalo.Format(_T("%.8E"), resultat_calo);
	m_KaollaView->SetDlgItemText(IDC_CALO, m_KaollaView->m_StrCalo);*/

	return 0;
}

BOOL MFCMessageHandler::DisplayHighPressure()
{
	// Post the required message
	::PostMessage(windowHandle, WM_DISPLAYHIGHPRESSURE, NULL, NULL);

	/*m_KaollaView->m_StrHautePression.Format(_T("%f"), resultat_hp);
	m_KaollaView->SetDlgItemText(IDC_HAUTE_PRESSION, m_KaollaView->m_StrHautePression);*/

	return 0;
}

BOOL MFCMessageHandler::DisplayLowPressure()
{
	// Post the required message
	::PostMessage(windowHandle, WM_DISPLAYLOWPRESSURE, NULL, NULL);

	/*m_KaollaView->m_StrBassePression.Format(_T("%f"), resultat_bp);
	m_KaollaView->SetDlgItemText(IDC_BASSE_PRESSION, m_KaollaView->m_StrBassePression);*/
	return 0;
}

BOOL MFCMessageHandler::DisplayTemperatures()
{
	// Post the required message
	::PostMessage(windowHandle, WM_DISPLAYTEMPERATURES, NULL, NULL);

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
	// Post the required message
	::PostMessage(windowHandle, WM_DISPLAYMESSAGEBOX, NULL, NULL);

	return 0;
}

