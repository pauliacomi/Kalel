#pragma once
#ifndef LISTEETAPE_H
#define LISTEETAPE_H

#include "InfoEtape.h"

#define POINT_ARRET_NON_DEFINI			1000
#define POINT_ARRET_LEA					1001
#define POINT_ARRET_SANS_DOSE_LEA		1002
#define POINT_ARRET_MISE_SOUS_VIDE_LEA	1003


const InfoEtape InfoTemperature = InfoEtape(STAGE_TEMP,_T("STAGE_TEMP"),POINT_ARRET_LEA);
const InfoEtape InfoLigneBaseEquilibre = InfoEtape(STAGE_EQ_LINE,_T("Ligne de base et d'équilibre"),POINT_ARRET_SANS_DOSE_LEA);
const InfoEtape InfoAdsorptionContinue = InfoEtape(STAGE_CONT_ADSORPTON,_T("Adsorption Continue"),POINT_ARRET_SANS_DOSE_LEA);
const InfoEtape InfoPetitesDoses = InfoEtape(STAGE_SMALL_DOSES,_T("Petites Doses"),POINT_ARRET_LEA);
const InfoEtape InfoMoyennesDoses = InfoEtape(STAGE_MEDIUM_DOSES,_T("Moyennes Doses"),POINT_ARRET_LEA);
const InfoEtape InfoGrandesDoses = InfoEtape(STAGE_BIG_DOSES,_T("Grandes Doses"),POINT_ARRET_LEA);
const InfoEtape InfoDesorption = InfoEtape(STAGE_DESORPTION,_T("Désorption"),POINT_ARRET_LEA);
const InfoEtape InfoMiseSousVide = InfoEtape(STAGE_UNDER_VACUUM,_T("Mise sous vide"),POINT_ARRET_MISE_SOUS_VIDE_LEA);

const InfoEtape ListeEtape[] =	{InfoTemperature,InfoLigneBaseEquilibre,InfoAdsorptionContinue,
								InfoPetitesDoses,InfoMoyennesDoses,InfoGrandesDoses,InfoDesorption,InfoMiseSousVide};
/*
const int ListeEtape[]={STAGE_TEMP,STAGE_EQ_LINE,STAGE_CONT_ADSORPTON,
				  STAGE_SMALL_DOSES,STAGE_BIG_DOSES,STAGE_DESORPTION,STAGE_UNDER_VACUUM};	

const CString ListeNomEtape[]={_T("STAGE_TEMP"),_T("Ligne de base et d'équilibre"),_T("Adsorption Continue"),
						 _T("Petites Doses"), _T("Grandes Doses"), _T("Désorption"), _T("Mise sous vide")};
*/
#define nb_etape 7


#endif