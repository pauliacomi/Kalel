#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H
#pragma once

#include "afxwin.h"

// Vue CGrapheView
#include "../Kalel Shared/Com Classes/ExperimentData.h"								// Where data about the experimental parameters, results and current status is stored. REQUIRED FOR CARRAYMEASUREMENTS
#include <map>
#include <forward_list>
#include <vector>

class CKalelDoc;

struct PLIST
{
	std::vector<int> nb_points;
	std::vector<float> time_elapsed;
	std::vector<float> calorimeter;
	std::vector<float> low_pressure;
	std::vector<float> high_pressure;
};

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
	void TraceGraph(CDC *pDC, CRect graphe, const PLIST& all_points, int max_p, int min_p, double max_calo, double min_calo, float max_time, float min_time = 0.0);
	void TraceSeries(CDC* pDC, CRect graphe, const std::vector<float>& points_x, const std::vector<float>& points_y, COLORREF colour, float min_x, float min_y, float scale_x, float scale_y);
	void TraceLegend(CRect rect, COLORREF color, CString texte, CDC* pDC);
	void TraceContour(CRect rect, CDC *pDC);
	int NbrIntervalles(double max);

private:
	ExperimentDataStorageArray * measurementArray;

	double maxPressure = 0;
	double minPressure = 0;
	double maxCalo = 0;
	double minCalo = 0;
	float timeMinimum = 0.0f;
	int measurementMinimum = 0;
	int stageName = 0;


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

#endif