
// KaollaDoc.cpp : implementation of the CKaollaDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Kaolla.h"
#endif


#include "KaollaDoc.h"

#include <propkey.h>

#include "ExperimentData.h"
#include "DefinePostMessages.h"		// For message definitions

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CKaollaDoc

IMPLEMENT_DYNCREATE(CKaollaDoc, CDocument)

BEGIN_MESSAGE_MAP(CKaollaDoc, CDocument)
END_MESSAGE_MAP()


// CKaollaDoc construction/destruction

CKaollaDoc::CKaollaDoc()
{
	TitreGrapheEtape = _T("");
	experiment_running = FALSE;
	GraphInitialize(NULL, NULL);
}

CKaollaDoc::~CKaollaDoc()
{
}

BOOL CKaollaDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}


// CKaollaDoc serialization

void CKaollaDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CKaollaDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CKaollaDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CKaollaDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CKaollaDoc diagnostics

#ifdef _DEBUG
void CKaollaDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CKaollaDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Returns a pointer to the document itself
CKaollaDoc * CKaollaDoc::GetDocument()
{
	CFrameWnd * pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);
	return (CKaollaDoc *)pFrame->GetActiveDocument();
}

//-------------------- CKaollaDoc custom functions

LRESULT CKaollaDoc::GraphInitialize(WPARAM wParam, LPARAM lParam)
{
	TempsMinimum = NULL;
	MesureMinimum = NULL;
	NumeroEtape = NULL;

	return 0;
}

// Add a measurement to the graph -- Regular
bool CKaollaDoc::GraphAddMeasurement(ExperimentData expData)
{
	int lastMeasurement;
	bool result = false;

	if (m_TableauMesures.IsEmpty())
	{
		m_TableauMesures.SetSize(1);
		m_TableauMesures[0] = expData;

		//Set the maximums and minimums

		maxPressure = max(expData.pressureLow, expData.pressureHigh);
		minPressure = min(expData.pressureLow, expData.pressureHigh);

		maxCalo = expData.resultCalorimeter;
		minCalo = expData.resultCalorimeter;
	}
	else
	{
		lastMeasurement = m_TableauMesures.GetUpperBound();

		if (expData.experimentMeasurements > m_TableauMesures[lastMeasurement].experimentMeasurements) {

		maxPressure = max(maxPressure, expData.pressureLow);
		maxPressure = max(maxPressure, expData.pressureHigh);
		
		minPressure = min(minPressure, expData.pressureLow);
		minPressure = min(minPressure, expData.pressureHigh);
		
		maxCalo = max(maxCalo, expData.resultCalorimeter);
		minCalo = min(minCalo, expData.resultCalorimeter);

		m_TableauMesures.SetSize(lastMeasurement + 2);
		m_TableauMesures[lastMeasurement+1] = expData;

		result = true;
		}
	}
	return result;
}

/*void CManip::DonneesManuelleGrapheEtape()
{
int nb_heures = 2;
int nb_secondes = nb_heures * 3600;

int coeff = (int)(temps_manip / nb_secondes);
float temps_min = coeff * nb_secondes;


if (true m_Doc->NumeroEtape < coeff + 1)
{
ASSERT(0);
messageHandler.GraphDataAutoStep(temps_min, numero_mesure - 1, coeff + 1);

CString titre;
titre.Format(_T("%dh - %dh d'expérience"), nb_heures * coeff, nb_heures*(coeff + 1));
messageHandler.GraphSetTitle(titre);
}
}*/


// ?
CArrayMesure* CKaollaDoc::GetTableauMesures(void)
{
	return &m_TableauMesures;
}