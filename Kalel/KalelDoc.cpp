
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
}

CKalelDoc::~CKalelDoc()
{
	for (size_t i = 0; i < m_TableauMesures.size(); i++)
	{
		delete m_TableauMesures[i];
	}
	m_TableauMesures.clear();
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

// Add a measurement to the graph
bool CKalelDoc::GraphAddMeasurement(ExperimentData * expData)
{
	int lastMeasurement;
	bool result = false;

	if (m_TableauMesures.size() == 0)
	{
		m_TableauMesures.push_back(expData);
		result = true;
	}
	else
	{
		if (expData->experimentGraphPoints > m_TableauMesures.back()->experimentGraphPoints) {

			m_TableauMesures.push_back(expData);
			result = true;
		}
	}
	return result;
}


LRESULT CKalelDoc::GraphReset(WPARAM , LPARAM ) {

	for (size_t i = 0; i < m_TableauMesures.size(); i++)
	{
		delete m_TableauMesures[i];
	}

	m_TableauMesures.clear();
	return 0;
}
