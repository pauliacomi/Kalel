
// KalelDoc.cpp : implementation of the CKalelDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Kalel.h"
#endif


#include "KalelDoc.h"

#include <propkey.h>

#include "ExperimentData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CKalelDoc

IMPLEMENT_DYNCREATE(CKalelDoc, CDocument)

BEGIN_MESSAGE_MAP(CKalelDoc, CDocument)
END_MESSAGE_MAP()


// CKalelDoc construction/destruction

CKalelDoc::CKalelDoc()
{
	TitreGrapheEtape = _T("");
	GraphInitialize(NULL, NULL);
}

CKalelDoc::~CKalelDoc()
{
}

BOOL CKalelDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}


// CKalelDoc serialization

void CKalelDoc::Serialize(CArchive& ar)
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
void CKalelDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CKalelDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CKalelDoc::SetSearchContent(const CString& value)
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

// CKalelDoc diagnostics

#ifdef _DEBUG
void CKalelDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CKalelDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Returns a pointer to the document itself
CKalelDoc * CKalelDoc::GetDocument()
{
	CFrameWnd * pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);
	return (CKalelDoc *)pFrame->GetActiveDocument();
}

//-------------------- CKalelDoc custom functions

LRESULT CKalelDoc::GraphInitialize(WPARAM wParam, LPARAM lParam)
{
	TempsMinimum = NULL;
	MesureMinimum = NULL;
	NumeroEtape = NULL;

	return 0;
}

// Add a measurement to the graph -- Regular
bool CKalelDoc::GraphAddMeasurement(ExperimentData expData)
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


LRESULT CKalelDoc::GraphReset(WPARAM , LPARAM ) {
	
	m_TableauMesures.RemoveAll();
	TempsMinimum = NULL;
	MesureMinimum = NULL;
	NumeroEtape = NULL;

	return 0;
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
CArrayMesure* CKalelDoc::GetTableauMesures(void)
{
	return &m_TableauMesures;
}