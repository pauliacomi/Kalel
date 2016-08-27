#pragma once
#include "../Kalel Shared/Com Classes/ExperimentData.h"				// Where data about the experimental parameters, results and current status is stored

// Vue CGrapheView

class CKalelDoc;

class CGrapheView : public CView
{
	DECLARE_DYNCREATE(CGrapheView)

protected:
	CGrapheView();           // constructeur protégé utilisé par la création dynamique
	virtual ~CGrapheView();
	
	void OnInitialUpdate();

public:
	CKalelDoc* GetDocument() const;

	void TraceAxis(CRect place_graphe, CRect axe_graphe, CDC *pDC,CString);
	void TraceScale(CRect graphe,CRect axe_graphe,int max_pression,int min_pression,double max_calo,double min_calo,
					   CDC *pDC,float min_temps = 0.0);
	void TraceGraph(CRect graphe,int max_pression,int min_pression,double max_calo,double min_calo,
					  CDC *pDC,float min_temps=0.0,int PremiereMesure=0);
	void TraceLegend(CRect rect, COLORREF color, CString texte, CDC* pDC);
	void TraceContour(CRect rect, CDC *pDC);
	double MaxPressionEchelle(double max_p);
	int NbrIntervalles(double max);

private:
	CArrayMeasurments * measurementArray;
	double maxPressure, minPressure, maxCalo, minCalo;
	CString titleGrapheEtape;
	float timeMinimum;
	int measurementMinimum, stageName;

public:
	virtual void OnDraw(CDC* pDC);      // substitué pour dessiner cette vue
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
};


