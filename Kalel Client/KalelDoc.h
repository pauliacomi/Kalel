
// KalelDoc.h : interface of the CKalelDoc class
//
#pragma once

#include <map>
#include <chrono>

#include "../Kalel Shared/Com Classes/ExperimentData.h"				// Where data about the experimental parameters, results and current status is stored. REQUIRED FOR CARRAYMEASUREMENTS

class CKalelDoc : public CDocument
{
protected: // create from serialization only
	CKalelDoc();
	DECLARE_DYNCREATE(CKalelDoc)

// Attributes
public:	
	std::map<std::chrono::system_clock::time_point, ExperimentData>* pMeasurementArray = nullptr;

// Operations
public:
	static CKalelDoc * GetDocument();

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
	virtual ~CKalelDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

public:
	void GraphSetArray(std::map<std::chrono::system_clock::time_point, ExperimentData> & expData);
	

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
