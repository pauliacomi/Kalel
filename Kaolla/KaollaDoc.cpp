
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

#include "Mesure.h"
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
	InitializeGraph(NULL, NULL);
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

LRESULT CKaollaDoc::InitializeGraph(WPARAM wParam, LPARAM lParam)
{
	TempsMinimum = -1;
	MesureMinimum = -1;
	NumeroEtape = -1;

	return 0;
}

// Add a measurement to the graph -- Regular
void CKaollaDoc::RajoutMesure(CMesure NouvellesMesures)
{
	int lastMeasurement = m_TableauMesures.GetUpperBound();
	m_TableauMesures[lastMeasurement] = NouvellesMesures;
}

// Add a measurement to the graph -- Overload 1
void CKaollaDoc::RajoutMesure(int num, double tps, double calorimeter, double lowPressure, double highPressure, double tempCalo, double tempCage, double tempPiece)
{
	// Set the maximum and minimums
	if (m_TableauMesures.IsEmpty())
	{
		maxPressure = max(lowPressure, highPressure);
		minPressure = min(lowPressure, highPressure);

		maxCalo = calorimeter;
		minCalo = calorimeter;
	}
	else
	{
		maxPressure = max(maxPressure, lowPressure);
		maxPressure = max(maxPressure, highPressure);

		minPressure = min(minPressure, lowPressure);
		minPressure = min(minPressure, highPressure);

		maxCalo = max(maxCalo, calorimeter);
		minCalo = min(minCalo, calorimeter);
	}

	// Add measurement to the array
	m_TableauMesures.SetSize(num);
	m_TableauMesures[num - 1] = CMesure(num, tps, calorimeter, lowPressure, highPressure, tempCalo, tempCage, tempPiece);

	//Debug code
#ifdef _DEBUG
	int j = num - 1;
	TRACE("m_TableauMesures[%d]=CMesure(%d,%d,%f,%f,%f,%f,%f,%f)\n", j, m_TableauMesures[j].numero, m_TableauMesures[j].temps, m_TableauMesures[j].calo, m_TableauMesures[j].basse_pression,
		m_TableauMesures[j].haute_pression, m_TableauMesures[j].temperature_calo, m_TableauMesures[j].temperature_cage,
		m_TableauMesures[j].temperature_piece);
#endif // _DEBUG

}

// Add a measurement to the graph -- Overload 2
void CKaollaDoc::RajoutMesure(double tps, double calorimeter, double lowPressure, double highPressure, double tempCalo, double tempCage, double tempPiece)
{
	int lastMeasurement;
	if (m_TableauMesures.IsEmpty())
	{
		lastMeasurement = 1;
		m_TableauMesures.SetSize(3);
	}
	else
		lastMeasurement = m_TableauMesures.GetUpperBound();
	m_TableauMesures[lastMeasurement - 1] = CMesure(lastMeasurement, tps, calorimeter, lowPressure, highPressure, tempCalo, tempCage, tempPiece);
	

	//Debug code
#ifdef _DEBUG
	int j = lastMeasurement - 1;
	TRACE("m_TableauMesures[%d]=CMesure(%d,%d,%f,%f,%f,%f,%f,%f)\n", j, m_TableauMesures[j].numero,
		m_TableauMesures[j].temps, m_TableauMesures[j].calo, m_TableauMesures[j].basse_pression,
		m_TableauMesures[j].haute_pression, m_TableauMesures[j].temperature_calo, m_TableauMesures[j].temperature_cage,
		m_TableauMesures[j].temperature_piece);
#endif // _DEBUG
}

// ?
CArrayMesure* CKaollaDoc::GetTableauMesures(void)
{
	return &m_TableauMesures;
}