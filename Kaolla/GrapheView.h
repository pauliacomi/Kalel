#pragma once


// Vue CGrapheView

class CKaollaDoc;

class CGrapheView : public CView
{
	DECLARE_DYNCREATE(CGrapheView)

protected:
	CGrapheView();           // constructeur protégé utilisé par la création dynamique
	virtual ~CGrapheView();

public:
	CKaollaDoc* GetDocument() const;
	void trace_axe(CRect place_graphe, CRect axe_graphe, CDC *pDC, CKaollaDoc* pDoc,CString);
	/*
	void trace_echelle(CRect graphe,CRect axe_graphe,int max_pression,int min_pression,int max_calo,int min_calo,
					   CDC *pDC, CKaollaDoc* pDoc,float min_temps = 0.0);
	void trace_graphe(CRect graphe,int max_pression,int min_pression,int max_calo,int min_calo,
					  CDC *pDC,CKaollaDoc *pDoc,float min_temps=0.0,int PremiereMesure=0);
  */
	void trace_echelle(CRect graphe,CRect axe_graphe,int max_pression,int min_pression,double max_calo,double min_calo,
					   CDC *pDC, CKaollaDoc* pDoc,float min_temps = 0.0);
	void trace_graphe(CRect graphe,int max_pression,int min_pression,double max_calo,double min_calo,
					  CDC *pDC,CKaollaDoc *pDoc,float min_temps=0.0,int PremiereMesure=0);
	void trace_legende(CRect rect, COLORREF color, CString texte, CDC* pDC);

	void traceContour(CRect rect, CDC *pDC);

	double MaxPressionEchelle(double max_p);

	int NbrIntervalles(double max);

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


