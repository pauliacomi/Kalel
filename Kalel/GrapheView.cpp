// GrapheView.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "GrapheView.h"

//
#include "KalelDoc.h"			// For the document pointer
#include "StringTable.h"		// For the strings 


#define border 15
#define hauteur_legende 30

#define TAILLE_POINT 15000
#define NOMBREPOINTS 2000
#define INEXISTANT -255

#define RECENT_HOURS 0.005

// CGrapheView

IMPLEMENT_DYNCREATE(CGrapheView, CView)

BEGIN_MESSAGE_MAP(CGrapheView, CView)
END_MESSAGE_MAP()

CGrapheView::CGrapheView()
{
	titleGrapheEtape = _T("");
	timeMinimum = 0;
	measurementMinimum = 0;
	stageName = 0;
}

CGrapheView::~CGrapheView()
{
}

void CGrapheView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CKalelDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	measurementArray = &pDoc->m_TableauMesures;
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

	int limite_hauteur = 12 * border + hauteur_legende + 100;
	int limite_largeur = 300;

	if((rect_grapheView.Height() < limite_hauteur) || (rect_grapheView.Width() < limite_largeur))
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
		if (measurementArray->size() != 0)
		{

			// Acquisition des données 
			ExperimentData * experimentData = (*measurementArray).back();

			// Valeurs utilisées pour les échelles et les axes d'abscisses et d'ordonnées
			// Set the maximums and minimums
			maxPressure = max(maxPressure, experimentData->pressureLow);
			maxPressure = max(maxPressure, experimentData->pressureHigh);
			maxPressure = MaxPressionEchelle(maxPressure);

			minPressure = min(minPressure, experimentData->pressureLow);
			minPressure = min(minPressure, experimentData->pressureHigh);
			minPressure = 0;

			maxCalo = max(maxCalo, experimentData->resultCalorimeter);
			minCalo = min(minCalo, experimentData->resultCalorimeter);

			int displayedSeconds = RECENT_HOURS * 3600;
			timeMinimum = experimentData->experimentTime - displayedSeconds;
			float partialCoefficient = (experimentData->experimentTime / experimentData->experimentGraphPoints);
			measurementMinimum  = (int)(experimentData->experimentGraphPoints - displayedSeconds / partialCoefficient);
			if (measurementMinimum < 0)
				measurementMinimum = 0;

			//float timeMinimum = partialCoefficient * displayedSeconds;


			// Les graphes

			// On divisera l'espace pour les graphes en 3 : 
			//   - La partie du haut pour le graphe complet
			//   - La partie du milieu pour le graphe par étape
			//   - La petite partie du bas pour la légende

			CRect grapheHaut,grapheMilieu,grapheBas;

			grapheHaut = CRect(rect_grapheView.left + border,rect_grapheView.top,
							   rect_grapheView.right - border,(rect_grapheView.bottom-hauteur_legende)/2);

			grapheMilieu = CRect(rect_grapheView.left + border,grapheHaut.bottom,
								 rect_grapheView.right - border,rect_grapheView.bottom-hauteur_legende);

			grapheBas = CRect(rect_grapheView.left + border,grapheMilieu.bottom,
							  rect_grapheView.right- border,rect_grapheView.bottom);


			// ----------------------------------------------------------------
			// -------------- Graphe complet ----------------------------------
			// ----------------------------------------------------------------

			// place_grapheComplet : La place qui est réservé pour tracer le graphe complet
			CRect place_grapheComplet = grapheHaut;
			// où sera tracé l'axe tout simplement
			CRect axe_grapheComplet;     
			axe_grapheComplet = CRect (place_grapheComplet.left  + border,
									   place_grapheComplet.top   + border,
									   place_grapheComplet.right - border,
									   place_grapheComplet.bottom- 3*border);

			// grapheComplet : Où seront tracés les points
			CRect grapheComplet = CRect (axe_grapheComplet.left,
										 axe_grapheComplet.top + border,
										 axe_grapheComplet.right,
										 axe_grapheComplet.bottom - border);
		
			CString titreComplet;
			titreComplet.Format(GRAPH_COMPLETE);
			TraceAxis(place_grapheComplet,axe_grapheComplet,pDC,titreComplet);

			TraceScale(grapheComplet, axe_grapheComplet, maxPressure, minPressure, maxCalo, minCalo, pDC);
			TraceGraph(grapheComplet, maxPressure, minPressure, maxCalo, minCalo, pDC);



			// ----------------------------------------------------------------
			// ------------- Graphe Etape -------------------------------------
			// ----------------------------------------------------------------

			CRect place_grapheEtape = grapheMilieu;
			CRect axe_grapheEtape;     // où sera tracé l'axe tout simplement
			axe_grapheEtape = CRect (place_grapheEtape.left  + border,
									 place_grapheEtape.top   + border,
									 place_grapheEtape.right - border,
									 place_grapheEtape.bottom- 3*border);

			CRect grapheEtape = CRect (axe_grapheEtape.left,
									   axe_grapheEtape.top + border,
									   axe_grapheEtape.right,
									   axe_grapheEtape.bottom - border);
			
			titleGrapheEtape.Format(GRAPH_PART, RECENT_HOURS);
			TraceAxis(place_grapheEtape, axe_grapheEtape, pDC, titleGrapheEtape);
			TraceScale(grapheEtape, axe_grapheEtape, maxPressure, minPressure, maxCalo, minCalo, pDC, timeMinimum);
			TraceGraph(grapheEtape, maxPressure, minPressure, maxCalo, minCalo, pDC, timeMinimum, measurementMinimum);
		

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


void CGrapheView::TraceAxis(CRect place_graphe, CRect axe_graphe, CDC *pDC, CString titre)
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
								place_graphe.top + 2*border);

	CRect piedComplet = CRect(place_graphe.left,
							  place_graphe.bottom - 2*border,
							  place_graphe.right,
							  place_graphe.bottom);

	CString texte_max_pression;
	texte_max_pression.Format(_T("Max : %0.2f Bar"), maxPressure);
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
								CDC *pDC, float min_temps)
{
	// intervalle : entre 2 traits
	// il y aura donc (nb_intervalle + 1) traits
		
	// ------- Echelle pression -----------------------------------

	int taille_text=40; // largueur cadre pour le texte des valeurs

	//int nb_intervalles=5; //nb_traits = nb_intervalles + 1
	int nb_intervalles=NbrIntervalles(max_pression);
	int ecart_pression = max_pression-min_pression;
		

	pDC->SetTextColor(RGB(0,0,128));
			
		
	for(int i=0;i<=nb_intervalles;i++)
	{
		CRect rect_text;
		// traçage du trait
		int pt_trait1=graphe.left-5; // abs de l'extrémité gauche
		int pt_trait2=graphe.left+5; // abs de l'extrémité droit
		int ord= i*graphe.Height()/nb_intervalles + graphe.top;
		pDC->MoveTo(pt_trait1,ord);
		pDC->LineTo(pt_trait2,ord);
			
			
		//rect_text : pour écrire la valeur
		int l=graphe.left + border;
		int t=ord-10;//border;
		int r=graphe.left + taille_text + border;
		int b=ord+10;//border;
			
		rect_text=CRect(l,t,r,b);


		CString texte;
		texte.Format(_T("%.2f"),max_pression - ((double)(i*ecart_pression))/nb_intervalles + min_pression);

		pDC->DrawText(texte,rect_text,DT_LEFT);

	}


	// ------ Echelle Calo -----------------------------------------
	int nb_intervalles_calo = 1;//5;
	//int ecart_calo = max_calo-min_calo;
	double ecart_calo = max_calo-min_calo;
		
	for(int i=0;i<=nb_intervalles_calo;i++)
	{
		CRect rect_text;
		int pt_trait1=graphe.right-5;
		int pt_trait2=graphe.right+5;
		int ord= i*graphe.Height()/nb_intervalles_calo + graphe.top;
		pDC->MoveTo(pt_trait1,ord);
		pDC->LineTo(pt_trait2,ord);
			
		/*
		int l=graphe.right - taille_text - border - 30;
		int t=ord-10;//border;
		int r=graphe.right - border;
		int b=ord+10;//border;
			
		rect_text=CRect(l,t,r,b);


		//CString texte;
		//double bidule = max_calo - i*ecart_calo/nb_intervalles_calo + min_calo;
		//texte.Format(_T("%0.2E1f"),bidule);


		CString texte;
		switch(i)
		{
			case 0:
				texte.Format(_T("10E-3"));
				break;
			case 1:
				texte.Format(_T("8E-3"));
				break;
			case 2:
				texte.Format(_T("6E-3"));
				break;
			case 3:
				texte.Format(_T("4E-3"));
				break;
			case 4:
				texte.Format(_T("2E-3"));
				break;
			case 5:
				texte.Format(_T("0"));
				break;
			default:
				texte.Format(_T("Inconnu"));
		}
		pDC->SetTextColor(RGB(0,0,128));
		pDC->DrawText(texte,rect_text,DT_RIGHT);
		*/

	}


	// ----- marquage du temps ----------------------------------------------
	int nb_trait_abs=4;
	int temps = (int)(*measurementArray).back()->experimentTime;

	for (int i=0;i<=nb_trait_abs;i++)
	{
		CRect rect_text;

		int abs= i*axe_graphe.Width()/nb_trait_abs + 2*border;
			
		int pt_trait1=axe_graphe.bottom-5;
		//int pt_trait2=axe_graphe.bottom+5;
		int pt_trait2=axe_graphe.bottom;

		pDC->MoveTo(abs,pt_trait1);
		pDC->LineTo(abs,pt_trait2);


		//Valeur pour 'exterieur' axe
		int l=abs - 2*border;
		int t=axe_graphe.bottom;// + border;
		int r=abs + 2*border;
		int b=axe_graphe.bottom + 20;//+ border;

		rect_text= CRect(l,t,r,b);


		CString texte;
		int texte_int = (int)(min_temps + i*(temps-min_temps)/nb_trait_abs);
		texte.Format(_T("%d"),texte_int);
		pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText(texte,rect_text,DT_BOTTOM|DT_CENTER);
	}
}


// -------------------------------------------------
// --------------- Graphe --------------------------
// -------------------------------------------------



void CGrapheView::TraceGraph(CRect graphe,int max_pression,int min_pression,double max_calo,double min_calo,
							   CDC *pDC,float min_temps,int firstMeasurement)
{
	// rapport = hauteur du graphe / (max_calo - min_calo)
	// rapport = valeur (bar ou µV) par pixel
	float rapport_calo, rapport_pression, rapport_temps;
	float max_temps = (*measurementArray).back()->experimentTime;
	float ecart_temps = max_temps - min_temps;
	float ecart_calo = max_calo - min_calo;
	float ecart_pression = max_pression - min_pression;
		
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

/*

	// --------------------- Version 1 ----------------------------------------

	const int valeur = TAILLE_POINT;

	

	// Liste de POINT afin de créer plusieurs lignes entre ces points
	// POINT : int x et int y
	POINT GBasse_pression[valeur],GHaute_pression[valeur],GCalo[valeur];

	//conversion des points pour le graphe
	int j=0;
	for(int i=PremiereMesure;i<=m_TableauMesures->GetUpperBound() && j<valeur;i++)
	{
		int saut_de_mesure = (int)(((m_TableauMesures->GetUpperBound()-PremiereMesure) / 1000)+1);

		if((i-PremiereMesure) % saut_de_mesure == 0)
		{
			double abs_temps = graphe.left + rapport_temps * (pDoc->m_TableauMesures[i].temps - min_temps);

			GCalo[j].x = abs_temps;
			GCalo[j].y = graphe.bottom - rapport_calo * pDoc->m_TableauMesures[i].calo;

			GBasse_pression[j].x=abs_temps;
			GBasse_pression[j].y=graphe.bottom - rapport_pression * (pDoc->m_TableauMesures[i].basse_pression);

			GHaute_pression[j].x=abs_temps;
			GHaute_pression[j].y=graphe.bottom - rapport_pression * (pDoc->m_TableauMesures[i].haute_pression);
			j++;
		}
	}

	
	//UINT	nb_points=m_TableauMesures->GetSize() - PremiereMesure;
	UINT nb_points = j;

	// traçage des courbes

	// ------------ Calo en rouge -------------------------

	// Changement de 'pen' pour tracer les courbes avec une couleur différente
	// On sélectionne ce 'Pen' dans ce device context
    // On sauve l'ancien 'Pen' par la même occasion

	CPen newPen1(PS_SOLID,1,RGB(255,0,0));
	CPen*pOldPen1=pDC->SelectObject(&newPen1);
	pDC->SelectObject(&newPen1);

	// dessine avec le 'Pen'
	pDC->Polyline(GCalo,nb_points);
	// restaure l'ancien 'pen' dans le device context
	pDC->SelectObject(pOldPen1);

	// ------------ Basse pression en vert ----------------
	CPen newPen2(PS_SOLID,1,RGB(0,185,0));
	CPen*pOldPen2=pDC->SelectObject(&newPen2);
	pDC->SelectObject(&newPen2);
	pDC->Polyline(GBasse_pression,nb_points);
	pDC->SelectObject(pOldPen2);

	// ------------ Haute pression en bleu ----------------
	CPen newPen3(PS_SOLID,1,RGB(0,0,255));
	CPen*pOldPen3=pDC->SelectObject(&newPen3);
	pDC->SelectObject(&newPen3);
	pDC->Polyline(GHaute_pression,nb_points);
	pDC->SelectObject(pOldPen3);
	*/

/*
	// ----------------- Version 2 -----------------------------------------

	// const int valeur = TAILLE_POINT;
	const int valeur = NOMBREPOINTS;

	int dif = m_TableauMesures->GetSize() / valeur;
	

	// Liste de POINT afin de créer plusieurs lignes entre ces points
	// POINT : int x et int y
	POINT GBasse_pression[valeur],GHaute_pression[valeur],GCalo[valeur];

	//conversion des points pour le graphe
	int j=0;
	//for(int i=PremiereMesure;i<=m_TableauMesures->GetUpperBound() && j<valeur;i++)
	for(int i=PremiereMesure;i<=m_TableauMesures->GetUpperBound() && j<valeur;i=i+(dif+1))
	{

		int saut_de_mesure = (int)(((m_TableauMesures->GetUpperBound()-PremiereMesure) / 1000)+1);

		if((i-PremiereMesure) % saut_de_mesure == 0)
		{
			double abs_temps = graphe.left + rapport_temps * (pDoc->m_TableauMesures[i].temps - min_temps);

			GCalo[j].x = abs_temps;
			//GCalo[j].y = graphe.bottom - rapport_calo * pDoc->m_TableauMesures[i].calo;
			GCalo[j].y = graphe.bottom - rapport_calo * (pDoc->m_TableauMesures[i].calo - min_calo);

			GBasse_pression[j].x=abs_temps;
			GBasse_pression[j].y=graphe.bottom - rapport_pression * (pDoc->m_TableauMesures[i].basse_pression - min_pression);

			GHaute_pression[j].x=abs_temps;
			GHaute_pression[j].y=graphe.bottom - rapport_pression * (pDoc->m_TableauMesures[i].haute_pression - min_pression);
			j++;
		}
	}


	//UINT	nb_points=m_TableauMesures->GetSize() - PremiereMesure;
	UINT nb_points = j;

	// traçage des courbes

	// ------------ Calo en rouge -------------------------

	// Changement de 'pen' pour tracer les courbes avec une couleur différente
	// On sélectionne ce 'Pen' dans ce device context
    // On sauve l'ancien 'Pen' par la même occasion

	CPen newPen1(PS_SOLID,1,RGB(255,0,0));
	CPen*pOldPen1=pDC->SelectObject(&newPen1);
	pDC->SelectObject(&newPen1);

	// dessine avec le 'Pen'
	pDC->Polyline(GCalo,nb_points);
	// restaure l'ancien 'pen' dans le device context
	pDC->SelectObject(pOldPen1);

	// ------------ Basse pression en vert ----------------
	CPen newPen2(PS_SOLID,1,RGB(0,185,0));
	CPen*pOldPen2=pDC->SelectObject(&newPen2);
	pDC->SelectObject(&newPen2);
	pDC->Polyline(GBasse_pression,nb_points);
	pDC->SelectObject(pOldPen2);

	// ------------ Haute pression en bleu ----------------
	CPen newPen3(PS_SOLID,1,RGB(0,0,255));
	CPen*pOldPen3=pDC->SelectObject(&newPen3);
	pDC->SelectObject(&newPen3);
	pDC->Polyline(GHaute_pression,nb_points);
	pDC->SelectObject(pOldPen3);
*/

/*
	// ----------------- Version 3 -----------------------------------------

	// const int valeur = TAILLE_POINT;
	const int valeur = NOMBREPOINTS;

	int dif = (m_TableauMesures->GetSize() - PremiereMesure) / (valeur-1);

	POINT temp_Calo, temp_Basse_pression, temp_Haute_pression;

	temp_Calo.x = INEXISTANT;
	temp_Calo.y = INEXISTANT;

	temp_Basse_pression.x = INEXISTANT;
	temp_Basse_pression.y = INEXISTANT;

	temp_Haute_pression.x = INEXISTANT;
	temp_Haute_pression.y = INEXISTANT;

	POINT GCalo[valeur],GBasse_pression[valeur],GHaute_pression[valeur];

	for(int compteur = 0;compteur<=dif;compteur++)
	{

		// Liste de POINT afin de créer plusieurs lignes entre ces points
		// POINT : int x et int y
		


		//conversion des points pour le graphe
		int j=0;
		if (temp_Calo.x != INEXISTANT)
		{
			GCalo[j].x = temp_Calo.x;
			GCalo[j].y = temp_Calo.y;

			GBasse_pression[j].x=temp_Basse_pression.x;
			GBasse_pression[j].y=temp_Basse_pression.y;

			GHaute_pression[j].x=temp_Haute_pression.x;
			GHaute_pression[j].y=temp_Haute_pression.y;
			j++;
		}
		for(int i=PremiereMesure + compteur*(valeur-1);i<=m_TableauMesures->GetUpperBound() && j<=valeur;i++)
		{
			int saut_de_mesure = (int)(((m_TableauMesures->GetUpperBound()-PremiereMesure) / 1000)+1);

			if((i-PremiereMesure) % saut_de_mesure == 0)
			{
				double abs_temps = graphe.left + rapport_temps * (pDoc->m_TableauMesures[i].temps - min_temps);

				GCalo[j].x = abs_temps;
				GCalo[j].y = graphe.bottom - rapport_calo * (pDoc->m_TableauMesures[i].calo - min_calo);

				GBasse_pression[j].x=abs_temps;
				GBasse_pression[j].y=graphe.bottom - rapport_pression * (pDoc->m_TableauMesures[i].basse_pression - min_pression);

				GHaute_pression[j].x=abs_temps;
				GHaute_pression[j].y=graphe.bottom - rapport_pression * (pDoc->m_TableauMesures[i].haute_pression - min_pression);
				j++;
			}
		}


		temp_Calo=GCalo[j-1];
		temp_Basse_pression=GBasse_pression[j-1];
		temp_Haute_pression=GHaute_pression[j-1];

		//UINT	nb_points=m_TableauMesures->GetSize() - PremiereMesure;
		UINT nb_points = j;

		// traçage des courbes

		// ------------ Calo en rouge -------------------------

		// Changement de 'pen' pour tracer les courbes avec une couleur différente
		// On sélectionne ce 'Pen' dans ce device context
		// On sauve l'ancien 'Pen' par la même occasion

		CPen newPen1(PS_SOLID,1,RGB(255,0,0));
		CPen*pOldPen1=pDC->SelectObject(&newPen1);
		pDC->SelectObject(&newPen1);

		// dessine avec le 'Pen'
		pDC->Polyline(GCalo,nb_points);
		// restaure l'ancien 'pen' dans le device context
		pDC->SelectObject(pOldPen1);

		// ------------ Basse pression en vert ----------------
		CPen newPen2(PS_SOLID,1,RGB(0,185,0));
		CPen*pOldPen2=pDC->SelectObject(&newPen2);
		pDC->SelectObject(&newPen2);
		pDC->Polyline(GBasse_pression,nb_points);
		pDC->SelectObject(pOldPen2);

		// ------------ Haute pression en bleu ----------------
		CPen newPen3(PS_SOLID,1,RGB(0,0,255));
		CPen*pOldPen3=pDC->SelectObject(&newPen3);
		pDC->SelectObject(&newPen3);
		pDC->Polyline(GHaute_pression,nb_points);
		pDC->SelectObject(pOldPen3);

	}
*/

/*
	// ----------------- Version 3/test -----------------------------------------

	// const int valeur = TAILLE_POINT;
	const int valeur = NOMBREPOINTS;

	int dif = (m_TableauMesures->GetSize() - PremiereMesure) / valeur;

	POINT temp_Calo, temp_Basse_pression, temp_Haute_pression;

	temp_Calo.x = INEXISTANT;
	temp_Calo.y = INEXISTANT;

	temp_Basse_pression.x = INEXISTANT;
	temp_Basse_pression.y = INEXISTANT;

	temp_Haute_pression.x = INEXISTANT;
	temp_Haute_pression.y = INEXISTANT;

	POINT GCalo[valeur],GBasse_pression[valeur],GHaute_pression[valeur];

	for(int compteur = 0;compteur<=dif;compteur++)
	{

		// Liste de POINT afin de créer plusieurs lignes entre ces points
		// POINT : int x et int y
		


		//conversion des points pour le graphe
		int j=0;
		//if (temp_Calo.x != INEXISTANT)
		//{
		//	GCalo[j].x = temp_Calo.x;
		//	GCalo[j].y = temp_Calo.y;

		//	GBasse_pression[j].x=temp_Basse_pression.x;
		//	GBasse_pression[j].y=temp_Basse_pression.y;

		//	GHaute_pression[j].x=temp_Haute_pression.x;
		//	GHaute_pression[j].y=temp_Haute_pression.y;
		//	j++;
		//}
		for(int i=PremiereMesure + compteur*valeur;i<=m_TableauMesures->GetUpperBound() && j<valeur;i++)
		{
			int saut_de_mesure = (int)(((m_TableauMesures->GetUpperBound()-PremiereMesure) / 1000)+1);

			if((i-PremiereMesure) % saut_de_mesure == 0)
			{
				double abs_temps = graphe.left + rapport_temps * (pDoc->m_TableauMesures[i].temps - min_temps);

				GCalo[j].x = abs_temps;
				GCalo[j].y = graphe.bottom - rapport_calo * (pDoc->m_TableauMesures[i].calo - min_calo);

				GBasse_pression[j].x=abs_temps;
				GBasse_pression[j].y=graphe.bottom - rapport_pression * (pDoc->m_TableauMesures[i].basse_pression - min_pression);

				GHaute_pression[j].x=abs_temps;
				GHaute_pression[j].y=graphe.bottom - rapport_pression * (pDoc->m_TableauMesures[i].haute_pression - min_pression);
				j++;
			}
		}


		//UINT	nb_points=m_TableauMesures->GetSize() - PremiereMesure;
		UINT nb_points = j;

		// traçage des courbes

		// ------------ Calo en rouge -------------------------

		// Changement de 'pen' pour tracer les courbes avec une couleur différente
		// On sélectionne ce 'Pen' dans ce device context
		// On sauve l'ancien 'Pen' par la même occasion

		CPen newPen1(PS_SOLID,1,RGB(255,0,0));
		CPen*pOldPen1=pDC->SelectObject(&newPen1);
		pDC->SelectObject(&newPen1);

		if (temp_Calo.x != INEXISTANT)
		//if (temp_Calo.x != INEXISTANT && compteur!=dif)
		//if (temp_Calo.x != INEXISTANT && j!=valeur)
		{
			pDC->MoveTo(temp_Calo);
			pDC->LineTo(GCalo[0]);
		}

		// dessine avec le 'Pen'
		pDC->Polyline(GCalo,nb_points);


		// restaure l'ancien 'pen' dans le device context
		pDC->SelectObject(pOldPen1);



		// ------------ Basse pression en vert ----------------
		CPen newPen2(PS_SOLID,1,RGB(0,185,0));
		CPen*pOldPen2=pDC->SelectObject(&newPen2);
		pDC->SelectObject(&newPen2);

		pDC->Polyline(GBasse_pression,nb_points);
		if (temp_Basse_pression.x != INEXISTANT)
		//if (temp_Basse_pression.x != INEXISTANT && compteur!=dif)
		//if (temp_Basse_pression.x != INEXISTANT && j!=valeur)
		{
			pDC->MoveTo(temp_Basse_pression);
			pDC->LineTo(GBasse_pression[0]);
		}
		pDC->SelectObject(pOldPen2);



		// ------------ Haute pression en bleu ----------------
		CPen newPen3(PS_SOLID,1,RGB(0,0,255));
		CPen*pOldPen3=pDC->SelectObject(&newPen3);
		pDC->SelectObject(&newPen3);

		pDC->Polyline(GHaute_pression,nb_points);
		if (temp_Haute_pression.x != INEXISTANT)
		//if (temp_Haute_pression.x != INEXISTANT && compteur!=dif)
		//if (temp_Haute_pression.x != INEXISTANT && j!=valeur)
		{
			pDC->MoveTo(temp_Haute_pression);
			pDC->LineTo(GHaute_pression[0]);
		}
		pDC->SelectObject(pOldPen3);


		temp_Calo=GCalo[j-1];
		temp_Basse_pression=GBasse_pression[j-1];
		temp_Haute_pression=GHaute_pression[j-1];
	}
*/


	// ----------------- Version 4 (tous les points)-----------------------------------------

	// traçage des courbes

	// ------------ Calo en rouge -------------------------

	// Changement de 'pen' pour tracer les courbes avec une couleur différente
	// On sélectionne ce 'Pen' dans ce device context
	// On sauve l'ancien 'Pen' par la même occasion
	CPen newPen1(PS_SOLID,1,RGB(255,0,0));
	CPen * pOldPen1 = pDC->SelectObject(&newPen1);

	for(int i = firstMeasurement;i <= measurementArray->size() - 1;i++)
	{
		POINT PCalo;
		PCalo.x = graphe.left + rapport_temps * ((*measurementArray)[i]->experimentTime - min_temps);
		PCalo.y = graphe.bottom - rapport_calo * ((*measurementArray)[i]->resultCalorimeter - min_calo);
		if(i == firstMeasurement)
			pDC->MoveTo(PCalo);
		else
			pDC->LineTo(PCalo);
	}
	// restaure l'ancien 'pen' dans le device context
	pDC->SelectObject(pOldPen1);


	// ------------ Basse pression en vert ----------------
	CPen newPen2(PS_SOLID, 1, RGB(0, 185, 0));
	CPen * pOldPen2 = pDC->SelectObject(&newPen2);

	for(int i = firstMeasurement;i<= measurementArray->size() - 1;i++)
	{
		POINT PBasse_pression;
		PBasse_pression.x = graphe.left + rapport_temps * ((*measurementArray)[i]->experimentTime - min_temps);
		PBasse_pression.y = graphe.bottom - rapport_pression * ((*measurementArray)[i]->pressureLow - min_pression);
		if(i == firstMeasurement)
			pDC->MoveTo(PBasse_pression);
		else
			pDC->LineTo(PBasse_pression);
	}
	pDC->SelectObject(pOldPen2);


	// ------------ Haute pression en bleu ----------------
	CPen newPen3(PS_SOLID,1,RGB(0,0,255));
	CPen * pOldPen3=pDC->SelectObject(&newPen3);
	
	for(int i= firstMeasurement;i<= measurementArray->size() - 1;i++)
	{
		POINT PHaute_pression;
		PHaute_pression.x = graphe.left + rapport_temps * ((*measurementArray)[i]->experimentTime - min_temps);
		PHaute_pression.y = graphe.bottom - rapport_pression * ((*measurementArray)[i]->pressureHigh - min_pression);
		if(i == firstMeasurement)
			pDC->MoveTo(PHaute_pression);
		else
			pDC->LineTo(PHaute_pression);
	}
	pDC->SelectObject(pOldPen3);
	

/*
	// ----------------- Version 5 (une partie des points)-----------------------------------------

	const int valeur = NOMBREPOINTS;
	int dif = m_TableauMesures->GetSize() / valeur;

	int der_index=m_TableauMesures->GetUpperBound();

	// traçage des courbes

	// ------------ Calo en rouge -------------------------

	// Changement de 'pen' pour tracer les courbes avec une couleur différente
	// On sélectionne ce 'Pen' dans ce device context
	// On sauve l'ancien 'Pen' par la même occasion
	CPen newPen1(PS_SOLID,1,RGB(255,0,0));
	CPen*pOldPen1=pDC->SelectObject(&newPen1);
	pDC->SelectObject(&newPen1);

	for(int i=PremiereMesure;i<=m_TableauMesures->GetUpperBound();i=i+(dif+1))
	{
		POINT PCalo;
		double abs_temps = graphe.left + rapport_temps * (pDoc->m_TableauMesures[i].temps - min_temps);
		PCalo.x = abs_temps;
		PCalo.y = graphe.bottom - rapport_calo * (pDoc->m_TableauMesures[i].calo - min_calo);
		if(i==PremiereMesure)
			pDC->MoveTo(PCalo);
		else
			pDC->LineTo(PCalo);
	}
	POINT PCaloDer;
	double abs_temps_der = graphe.left + rapport_temps * (pDoc->m_TableauMesures[der_index].temps - min_temps);
	PCaloDer.x = abs_temps_der;
	PCaloDer.y = graphe.bottom - rapport_calo * (pDoc->m_TableauMesures[der_index].calo - min_calo);
	pDC->LineTo(PCaloDer);

	// restaure l'ancien 'pen' dans le device context
	pDC->SelectObject(pOldPen1);


	// ------------ Basse pression en vert ----------------
	CPen newPen2(PS_SOLID,1,RGB(0,185,0));
	CPen*pOldPen2=pDC->SelectObject(&newPen2);
	pDC->SelectObject(&newPen2);

	for(int i=PremiereMesure;i<=m_TableauMesures->GetUpperBound();i=i+(dif+1))
	{
		POINT PBasse_pression;
		double abs_temps = graphe.left + rapport_temps * (pDoc->m_TableauMesures[i].temps - min_temps);
		PBasse_pression.x=abs_temps;
		PBasse_pression.y=graphe.bottom - rapport_pression * (pDoc->m_TableauMesures[i].basse_pression - min_pression);
		if(i==PremiereMesure)
			pDC->MoveTo(PBasse_pression);
		else
			pDC->LineTo(PBasse_pression);
	}

	POINT PBasse_pressionDer;
	PBasse_pressionDer.x=abs_temps_der;
	PBasse_pressionDer.y=graphe.bottom - rapport_pression * (pDoc->m_TableauMesures[der_index].basse_pression - min_pression);
	pDC->LineTo(PBasse_pressionDer);

	pDC->SelectObject(pOldPen2);


	// ------------ Haute pression en bleu ----------------
	CPen newPen3(PS_SOLID,1,RGB(0,0,255));
	CPen*pOldPen3=pDC->SelectObject(&newPen3);
	pDC->SelectObject(&newPen3);
	
	for(int i=PremiereMesure;i<=m_TableauMesures->GetUpperBound();i=i+(dif+1))
	{
		POINT PHaute_pression;
		double abs_temps = graphe.left + rapport_temps * (pDoc->m_TableauMesures[i].temps - min_temps);
		PHaute_pression.x=abs_temps;	
		PHaute_pression.y=graphe.bottom - rapport_pression * (pDoc->m_TableauMesures[i].haute_pression - min_pression);
		if(i==PremiereMesure)
			pDC->MoveTo(PHaute_pression);
		else
			pDC->LineTo(PHaute_pression);
	}

	POINT PHaute_pressionDer;
	PHaute_pressionDer.x=abs_temps_der;	
	PHaute_pressionDer.y=graphe.bottom - rapport_pression * (pDoc->m_TableauMesures[der_index].haute_pression - min_pression);
	pDC->LineTo(PHaute_pressionDer);

	pDC->SelectObject(pOldPen3);
*/

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


double CGrapheView::MaxPressionEchelle(double max_p)
{
	if(max_p <=1)
		return 1;
	if(1<max_p && max_p <=2)
		return 2;
	if(2<max_p && max_p <=3)
		return 3;
	if(3<max_p && max_p <=4)
		return 4;
	if(4<max_p && max_p <=5)
		return 5;
	if(5<max_p && max_p <=8)
		return 8;
	if(8<max_p && max_p <=10)
		return 10;
	if(10<max_p && max_p <=12)
		return 12;
	if(12<max_p && max_p <=15)
		return 15;
	if(15<max_p && max_p <=20)
		return 20;
	if(20<max_p && max_p <=25)
		return 25;
	if(25<max_p && max_p <=28)
		return 28;
	if(28<max_p && max_p <=30)
		return 30;
	if(30<max_p && max_p <=40)
		return 40;
	if(40<max_p && max_p <=50)
		return 50;
	if(50<max_p && max_p <=60)
		return 60;
	if(60<max_p && max_p <=70)
		return 70;
	if(70<max_p && max_p <=80)
		return 80;
	//if(80<max_p && max_p <=100)
	return 100;
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
