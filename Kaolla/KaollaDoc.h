
// KaollaDoc.h : interface of the CKaollaDoc class
//


#pragma once

#include "ExperimentData.h"

class CKaollaDoc : public CDocument
{
protected: // create from serialization only
	CKaollaDoc();
	DECLARE_DYNCREATE(CKaollaDoc)

// Attributes
public:	
	CArrayMesure m_TableauMesures;
	CString TitreGrapheEtape;
	double maxPressure, minPressure, maxCalo, minCalo;
	float TempsMinimum;
	int MesureMinimum, NumeroEtape;

// Operations
public:
	static CKaollaDoc * GetDocument();
	CArrayMesure * GetTableauMesures();

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CKaollaDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

public:
	LRESULT GraphInitialize(WPARAM wParam, LPARAM lParam);
	bool GraphAddMeasurement(ExperimentData expData);
	

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
