#ifndef LISTEETAPE_H
#define LISTEETAPE_H

#include "InfoEtape.h"

#define POINT_ARRET_NON_DEFINI			1000
#define POINT_ARRET_LEA					1001
#define POINT_ARRET_SANS_DOSE_LEA		1002
#define POINT_ARRET_MISE_SOUS_VIDE_LEA	1003


const InfoEtape InfoTemperature = InfoEtape(TEMPERATURES,_T("Temperatures"),POINT_ARRET_LEA);
const InfoEtape InfoLigneBaseEquilibre = InfoEtape(LIGNE_EQUILIBRE,_T("Ligne de base et d'équilibre"),POINT_ARRET_SANS_DOSE_LEA);
const InfoEtape InfoAdsorptionContinue = InfoEtape(ADSORPTION_CONTINUE,_T("Adsorption Continue"),POINT_ARRET_SANS_DOSE_LEA);
const InfoEtape InfoPetitesDoses = InfoEtape(PETITES_DOSES,_T("Petites Doses"),POINT_ARRET_LEA);
const InfoEtape InfoMoyennesDoses = InfoEtape(MOYENNES_DOSES,_T("Moyennes Doses"),POINT_ARRET_LEA);
const InfoEtape InfoGrandesDoses = InfoEtape(GRANDES_DOSES,_T("Grandes Doses"),POINT_ARRET_LEA);
const InfoEtape InfoDesorption = InfoEtape(DESORPTION,_T("Désorption"),POINT_ARRET_LEA);
const InfoEtape InfoMiseSousVide = InfoEtape(MISE_SOUS_VIDE,_T("Mise sous vide"),POINT_ARRET_MISE_SOUS_VIDE_LEA);

const InfoEtape ListeEtape[] =	{InfoTemperature,InfoLigneBaseEquilibre,InfoAdsorptionContinue,
								InfoPetitesDoses,InfoMoyennesDoses,InfoGrandesDoses,InfoDesorption,InfoMiseSousVide};
/*
const int ListeEtape[]={TEMPERATURES,LIGNE_EQUILIBRE,ADSORPTION_CONTINUE,
				  PETITES_DOSES,GRANDES_DOSES,DESORPTION,MISE_SOUS_VIDE};	

const CString ListeNomEtape[]={_T("Temperatures"),_T("Ligne de base et d'équilibre"),_T("Adsorption Continue"),
						 _T("Petites Doses"), _T("Grandes Doses"), _T("Désorption"), _T("Mise sous vide")};
*/
#define nb_etape 7


#endif