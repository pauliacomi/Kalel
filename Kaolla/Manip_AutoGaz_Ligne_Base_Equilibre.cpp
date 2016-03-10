#include "StdAfx.h"
#include "Manip_AutoGaz.h"


using namespace std;



void CManip_AutoGaz::LigneBaseEtEquilibre(LPVOID pParam)
{
	PointArretSansDoseFonction();
	etape_en_cours = LIGNE_EQUILIBRE;
	
	DonneesAutoGrapheEtape();

	CString nom_etape = _T("Ligne de base et équilibre");
	int temps_minutes = divers.temps_ligne_base;

	LireEcrireAfficher(divers.temps_ligne_base,nom_etape,pParam);

	PointArretSansDoseFonction();
}
