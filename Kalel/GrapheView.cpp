// GrapheView.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "GrapheView.h"

#include "KalelDoc.h"															// For the document pointer
#include "../Kalel Shared/Resources/StringTable.h"								// Error message strings
#include <algorithm>

static const int	BORDER_WIDTH		= 15;
static const int	LEGEND_HEIGHT		= 30;
static const int	TAILLE_POINT		= 15000;
static const int	NUMBER_OF_POINTS	= 2000;
static const int	NONEXISTENT			= -255;
static const int	HEIGHT_LIMIT		= 12 * BORDER_WIDTH + LEGEND_HEIGHT + 100;
static const int	WIDTH_LIMIT			= 300;
static const float	RECENT_HOURS		= 0.01f;

// CGrapheView

IMPLEMENT_DYNCREATE(CGrapheView, CView)

BEGIN_MESSAGE_MAP(CGrapheView, CView)
END_MESSAGE_MAP()

CGrapheView::CGrapheView()
{
}

CGrapheView::~CGrapheView()
{
}

void CGrapheView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CKalelDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	measurementArray = pDoc->m_TableauMesures;
}



// Dessin CGrapheView

void CGrapheView::OnDraw(CDC* pDC)
{
	CRect rect_grapheView;
	GetClientRect(rect_grapheView);

	// Code marked with REQ #001 is here to stop flickering
	CPaintDC ScreenDC(this); // device context for painting											// REQ #001
	CBitmap bm;																						// REQ #001
	bm.CreateCompatibleBitmap(&ScreenDC, rect_grapheView.Width(), rect_grapheView.Height());		// REQ #001
	pDC->SelectObject(&bm);																			// REQ #001

	if((rect_grapheView.Height() < HEIGHT_LIMIT) || (rect_grapheView.Width() < WIDTH_LIMIT))
	{
		// Si la place pour les graphes est trop petit on met un message d'avertissement
		// On écrit 'str' dans le lpRect avec le format nFormat
		CString textToWrite;
		textToWrite.Format(ERROR_GRAPH_TOO_SMALL);

		pDC->SetTextColor(RGB(255,0,0));
		pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText(textToWrite, rect_grapheView, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	}

	else{
		if (measurementArray != nullptr && !measurementArray->empty())
		{

			// Acquisition des données 
			PLIST all_points;

			all_points.nb_points = measurementArray->size();

			std::for_each(measurementArray->begin(), measurementArray->end(),
				[&all_points](const std::pair<std::chrono::system_clock::time_point, std::shared_ptr<ExperimentData>>& p){
				
				all_points.time_elapsed.push_back(p.second->GettimeElapsedSec());
				all_points.high_pressure.push_back(p.second->GetpressureHigh());
				all_points.low_pressure.push_back(p.second->GetpressureLow());
				all_points.calorimeter.push_back(p.second->GetresultCalorimeter());

				});
						

			// Valeurs utilisées pour les échelles et les axes d'abscisses et d'ordonnées
			// Set the maximums and minimums
			maxPressure = max(maxPressure, all_points.low_pressure.back());
			maxPressure = max(maxPressure, all_points.high_pressure.back());

			minPressure = min(minPressure, all_points.low_pressure.back());
			minPressure = min(minPressure, all_points.high_pressure.back());

			maxCalo = max(maxCalo, all_points.calorimeter.back());
			minCalo = min(minCalo, all_points.calorimeter.back());

			auto pmax_ceil = ceil(1.1 * maxPressure);
			auto pmin_floor = floor(1.1 * minPressure);

			float timeMaximum = all_points.time_elapsed.back();
			double displayedSeconds = RECENT_HOURS * 3600;
			timeMinimum = timeMaximum - displayedSeconds;

			// Les graphes

			// On divisera l'espace pour les graphes en 3 : 
			//   - La partie du haut pour le graphe complet
			//   - La partie du milieu pour le graphe par étape
			//   - La petite partie du bas pour la légende

			CRect grapheHaut,grapheMilieu,grapheBas;

			grapheHaut = CRect(rect_grapheView.left + BORDER_WIDTH,rect_grapheView.top,
							   rect_grapheView.right - BORDER_WIDTH,(rect_grapheView.bottom-LEGEND_HEIGHT)/2);

			grapheMilieu = CRect(rect_grapheView.left + BORDER_WIDTH,grapheHaut.bottom,
								 rect_grapheView.right - BORDER_WIDTH,rect_grapheView.bottom-LEGEND_HEIGHT);

			grapheBas = CRect(rect_grapheView.left + BORDER_WIDTH,grapheMilieu.bottom,
							  rect_grapheView.right- BORDER_WIDTH,rect_grapheView.bottom);


			// ----------------------------------------------------------------
			// -------------- Graphe complet ----------------------------------
			// ----------------------------------------------------------------

			// place_grapheComplet : La place qui est réservé pour tracer le graphe complet
			CRect place_grapheComplet = grapheHaut;
			// où sera tracé l'axe tout simplement
			CRect axe_grapheComplet;     
			axe_grapheComplet = CRect (place_grapheComplet.left  + BORDER_WIDTH,
									   place_grapheComplet.top   + BORDER_WIDTH,
									   place_grapheComplet.right - BORDER_WIDTH,
									   place_grapheComplet.bottom- 3*BORDER_WIDTH);

			// grapheComplet : Où seront tracés les points
			CRect grapheComplet = CRect (axe_grapheComplet.left,
										 axe_grapheComplet.top + BORDER_WIDTH,
										 axe_grapheComplet.right,
										 axe_grapheComplet.bottom - BORDER_WIDTH);
		
			CString titreComplet;
			titreComplet.Format(GRAPH_COMPLETE);

			TraceAxis(place_grapheComplet, axe_grapheComplet, pDC, titreComplet, pmax_ceil);
			TraceScale(grapheComplet, axe_grapheComplet, pmax_ceil, pmin_floor, maxCalo, minCalo, pDC, timeMaximum);
			TraceGraph(pDC, grapheComplet, all_points, pmax_ceil, pmin_floor, maxCalo, minCalo, timeMaximum);



			// ----------------------------------------------------------------
			// ------------- Graphe Etape -------------------------------------
			// ----------------------------------------------------------------

			CRect place_grapheEtape = grapheMilieu;
			CRect axe_grapheEtape;     // où sera tracé l'axe tout simplement
			axe_grapheEtape = CRect (place_grapheEtape.left  + BORDER_WIDTH,
									 place_grapheEtape.top   + BORDER_WIDTH,
									 place_grapheEtape.right - BORDER_WIDTH,
									 place_grapheEtape.bottom- 3*BORDER_WIDTH);

			CRect grapheEtape = CRect (axe_grapheEtape.left,
									   axe_grapheEtape.top + BORDER_WIDTH,
									   axe_grapheEtape.right,
									   axe_grapheEtape.bottom - BORDER_WIDTH);
			
			CString titleGrapheEtape;
			titleGrapheEtape.Format(GRAPH_PART, RECENT_HOURS);

			TraceAxis(place_grapheEtape, axe_grapheEtape, pDC, titleGrapheEtape, pmax_ceil);
			TraceScale(grapheEtape, axe_grapheEtape, pmax_ceil, pmin_floor, maxCalo, minCalo, pDC, timeMaximum, timeMinimum);
			TraceGraph(pDC, grapheEtape, all_points, pmax_ceil, pmin_floor, maxCalo, minCalo, timeMaximum, timeMinimum);
		

			// ------------------------------------------------------------
			// --------------- Légende ------------------------------------
			// ------------------------------------------------------------

			CRect legende = grapheBas;
		
			CRect legende_calo = CRect(legende.left,
									   legende.top,
									   legende.right/3,
									   legende.bottom);

			CRect legende_basse_pression = CRect(legende_calo.right,
												 legende.top,
												 2*legende.right/3,
												 legende.bottom);

			CRect legende_haute_pression = CRect(legende_basse_pression.right,
												 legende.top,
												 legende.right,
												 legende.bottom);

			COLORREF color1 = RGB(255,0,0);
			COLORREF color2 = RGB(0,185,0);
			COLORREF color3 = RGB(0,0,255);

			CString texte1, texte2, texte3;
			texte1.Format(TEXT_CALORIMETER);
			texte2.Format(TEXT_PRESSURE_HIGHRANGE);
			texte3.Format(TEXT_PRESSURE_LOWRANGE);

			TraceContour(legende, pDC);
			TraceLegend(legende_calo,color1,texte1,pDC);
			TraceLegend(legende_basse_pression,color2,texte2,pDC);
			TraceLegend(legende_haute_pression,color3,texte3,pDC);
		
		}
	}

	ScreenDC.BitBlt(0, 0, rect_grapheView.Width(), rect_grapheView.Height(), pDC, 0, 0, SRCCOPY);		// REQ #001
}


// ------------------------------------------------
// --------------- Axes ---------------------------
// ------------------------------------------------


void CGrapheView::TraceAxis(CRect place_graphe, CRect axe_graphe, CDC *pDC, CString titre, float pressure_max)
{
	// ----------- Traçage des axes -------------------------
	pDC->SetTextColor(RGB(255,255,255));

	// MoveTo : on modifie la position du point en mémoire à l'endroit indiqué 
	// LineTo : on trace une ligne du point en mémoire à l'endroit indiqué
	//          puis le point en mémoire prendra ces coordonnées
	pDC->MoveTo(axe_graphe.left,axe_graphe.top);
	pDC->LineTo(axe_graphe.left,axe_graphe.bottom);
	pDC->LineTo(axe_graphe.right,axe_graphe.bottom);
	pDC->LineTo(axe_graphe.right,axe_graphe.top);

	// ----------- Ecriture des données --------------------
	CRect enteteComplet = CRect(place_graphe.left,
							    place_graphe.top,
								place_graphe.right,
								place_graphe.top + 2*BORDER_WIDTH);

	CRect piedComplet = CRect(place_graphe.left,
							  place_graphe.bottom - 2*BORDER_WIDTH,
							  place_graphe.right,
							  place_graphe.bottom);

	CString texte_max_pression;
	texte_max_pression.Format(_T("Max : %0.2f Bar"), pressure_max);
	pDC->SetTextColor(RGB(0,0,0));
	pDC->DrawText(texte_max_pression,enteteComplet,DT_LEFT);

	CString texte_max_calo;
	texte_max_calo.Format(_T("Max : %0.2E W"), maxCalo);
	pDC->DrawText(texte_max_calo,enteteComplet,DT_RIGHT);

	//affichage du titre
	pDC->DrawText(titre,enteteComplet,DT_CENTER);

	CString texte_min_pression;
	texte_min_pression.Format(_T("Min : %0.2f Bar"), minPressure);
	pDC->DrawText(texte_min_pression,piedComplet,DT_LEFT);

	CString texte_min_calo;
	texte_min_calo.Format(_T("Min : %0.2E W"), minCalo);
	pDC->DrawText(texte_min_calo,piedComplet,DT_RIGHT);
}


// ------------------------------------------------
// -------------- Echelle -------------------------
// ------------------------------------------------



void CGrapheView::TraceScale(CRect graphe,CRect axe_graphe,int max_pression,int min_pression,double max_calo,double min_calo,
								CDC *pDC, float max_time, float min_time)
{
	// intervalle : entre 2 traits
	// il y aura donc (nb_intervalle + 1) traits
		
	// ------- Echelle pression -----------------------------------

	int taille_text=40; // largueur cadre pour le texte des valeurs

	int nb_intervalles = NbrIntervalles(max_pression);
	int ecart_pression = max_pression-min_pression;
		

	pDC->SetTextColor(RGB(0,0,128));
			
		
	for(auto i=0;i<=nb_intervalles;i++)
	{
		CRect rect_text;
		// traçage du trait
		int pt_trait1 = graphe.left - 5; // abs de l'extrémité gauche
		int pt_trait2 = graphe.left + 5; // abs de l'extrémité droit
		int ord = i*graphe.Height() / nb_intervalles + graphe.top;
		pDC->MoveTo(pt_trait1,ord);
		pDC->LineTo(pt_trait2,ord);
			
			
		//rect_text : pour écrire la valeur
		int l=graphe.left + BORDER_WIDTH;
		int t=ord-10;//BORDER_WIDTH;
		int r=graphe.left + taille_text + BORDER_WIDTH;
		int b=ord+10;//BORDER_WIDTH;
			
		rect_text=CRect(l,t,r,b);


		CString texte;
		texte.Format(_T("%.2f"),max_pression - ((double)(i*ecart_pression))/nb_intervalles + min_pression);

		pDC->DrawText(texte,rect_text,DT_LEFT);

	}


	// ------ Echelle Calo -----------------------------------------
	int nb_intervalles_calo = 1;//5;
	//double ecart_calo = max_calo-min_calo;
		
	for(auto i=0;i<=nb_intervalles_calo;i++)
	{
		CRect rect_text;
		int pt_trait1=graphe.right-5;
		int pt_trait2=graphe.right+5;
		int ord= i*graphe.Height()/nb_intervalles_calo + graphe.top;
		pDC->MoveTo(pt_trait1,ord);
		pDC->LineTo(pt_trait2,ord);
	}


	// ----- marquage du temps ----------------------------------------------
	int nb_labels = 4;
	int max_time_int = static_cast<int>(ceil(max_time));

	for (int i=0;i<= nb_labels;i++)
	{
		CRect rect_text;

		int abs = i*axe_graphe.Width() / nb_labels + 2 * BORDER_WIDTH;
			
		int pt_trait1=axe_graphe.bottom-5;
		int pt_trait2=axe_graphe.bottom;

		pDC->MoveTo(abs,pt_trait1);
		pDC->LineTo(abs,pt_trait2);


		//Valeur pour 'exterieur' axe
		int l = abs - 2 * BORDER_WIDTH;
		int t = axe_graphe.bottom;
		int r = abs + 2 * BORDER_WIDTH;
		int b = axe_graphe.bottom + 20;

		rect_text = CRect(l, t, r, b);


		CString texte;
		int texte_int = static_cast<int>(min_time + i*(max_time_int - min_time) / nb_labels);
		texte.Format(_T("%d"), texte_int);
		pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText(texte, rect_text, DT_BOTTOM | DT_CENTER);
	}
}


// -------------------------------------------------
// --------------- Graphe --------------------------
// -------------------------------------------------

void CGrapheView::TraceGraph(CDC *pDC, CRect graphe, const PLIST& all_points,
                             int max_p, int min_p, double max_calo, double min_calo, float max_time, float min_time)
{
	// rapport = hauteur du graphe / (max_calo - min_calo)
	// rapport = valeur (bar ou µV) par pixel

	float rapport_calo, rapport_pression, rapport_temps;
	float ecart_temps = max_time - min_time;
	float ecart_calo = max_calo - min_calo;
	float ecart_pression = max_p - min_p;
		
	if (ecart_calo!=0)
		rapport_calo = graphe.Height() / ecart_calo;
	else
		rapport_calo = 0;

	if (ecart_pression!=0)
		rapport_pression = graphe.Height() / ecart_pression;
	else
		rapport_pression=0;

	if (ecart_temps!=0)
		rapport_temps = graphe.Width() / ecart_temps;
	else
		rapport_temps = 0;


	// ----------------- Version 4 (tous les points)-----------------------------------------

	// traçage des courbes

	// ------------ Calo en rouge -------------------------
	TraceSeries(pDC, graphe, all_points.time_elapsed, all_points.calorimeter, RGB(255, 0, 0), min_time, min_calo, rapport_temps, rapport_calo);

	// ------------ Basse pression en vert ----------------
	TraceSeries(pDC, graphe, all_points.time_elapsed, all_points.low_pressure, RGB(0, 185, 0), min_time, min_p, rapport_temps, rapport_pression);

	// ------------ Haute pression en bleu ----------------
	TraceSeries(pDC, graphe, all_points.time_elapsed, all_points.high_pressure, RGB(0, 0, 255), min_time, min_p, rapport_temps, rapport_pression);
}


// -------------------------------------------------
// --------------- Series --------------------------
// -------------------------------------------------

void CGrapheView::TraceSeries(CDC* pDC, CRect graphe, const std::vector<float>& points_x, const std::vector<float>& points_y, 
	COLORREF colour, float min_x, float min_y, float scale_x, float scale_y)
{
	// Changement de 'pen' pour tracer les courbes avec une couleur différente
	// On sélectionne ce 'Pen' dans ce device context
	// On sauve l'ancien 'Pen' par la même occasion
	CPen newPen(PS_SOLID, 1, colour);
	CPen * pOldPen = pDC->SelectObject(&newPen);

	if (points_x.size() != points_y.size())
	{
		ASSERT(FALSE);
	}

	bool first_point = true;

	for (auto it = 0; it < points_x.size(); ++it)
	{
		POINT P;
		P.x = graphe.left + scale_x * (points_x[it] - min_x);
		if (P.x < graphe.left)
			continue;
		P.y = graphe.bottom - scale_y * (points_y[it] - min_y);

		if (first_point) {
			pDC->MoveTo(P);
			first_point = false;
		}
		else
			pDC->LineTo(P);
	}

	// restaure l'ancien 'pen' dans le device context
	pDC->SelectObject(pOldPen);
}


// -------------------------------------------------
// --------------- Legende --------------------------
// -------------------------------------------------


void CGrapheView::TraceLegend(CRect rect, COLORREF color, CString texte, CDC* pDC)
{

	// On trace une ligne de couleur représentative de la courbe	
	CPen newPen1(PS_SOLID,2,color);
	CPen*pOldPen1=pDC->SelectObject(&newPen1);
	pDC->SelectObject(&newPen1);
	pDC->MoveTo(rect.left + rect.Width()/6,rect.top + rect.Height()/2);
	pDC->LineTo(rect.left + rect.Width()/2,rect.top + rect.Height()/2);
	pDC->SelectObject(pOldPen1);

	// On écrit ici 'texte' pour dire ce que cette ligne représente

	CRect texte_legende = CRect(rect.left + rect.Width()/2 + 5,
								rect.top + rect.Height()/2 - 10,
								rect.right,
								rect.top + rect.Height()/2 + 10);
	pDC->SetTextColor(RGB(0,0,0));
	pDC->DrawText(texte,texte_legende,DT_LEFT);
}


// Fonction test pour voir le contour du rect alloué
void CGrapheView::TraceContour(CRect rect, CDC *pDC)
{
	pDC->SetTextColor(RGB(255,255,255));

	pDC->MoveTo(rect.left,rect.top);
	pDC->LineTo(rect.left,rect.bottom);
	pDC->LineTo(rect.right,rect.bottom);
	pDC->LineTo(rect.right,rect.top);
	pDC->LineTo(rect.left,rect.top);
}

int CGrapheView::NbrIntervalles(double max)
{
	if(max == 3 || max == 15 || max == 30)
		return 3;
	if(max == 5 || max == 10 || max == 50 || max == 100)
		return 5;
	return 4;
}





CKalelDoc* CGrapheView::GetDocument() const // la version non déboguée est en ligne
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKalelDoc)));
	return (CKalelDoc*)m_pDocument;
}

// Diagnostics de CGrapheView

#ifdef _DEBUG
void CGrapheView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CGrapheView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// Gestionnaires de messages de CGrapheView
