#include "StdAfx.h"
#include "Manip_AutoGaz.h"

using namespace std;

ofstream fichier_mesure_autogaz;


void CManip_AutoGaz::OuvertureFichierMesures()
{
	fichier_mesure_autogaz.open(NomFichier("csv").c_str(), ios_base::out /*| ios_base::trunc*/);
	
	// vider le ofstream fichier, pas le .csv, et on peut réitérer l'écriture en enlevant le caractère "fin de fichier"
	fichier_mesure_autogaz.clear(); 
	// Ecriture des noms des colonnes
	//fichier_mesure_autogaz << "N°mesure;Temps(s);Calorimètre(W);Basse Pression(Bar);Haute Pression(Bar);T°C Calo;T°C Cage;T°C pièce" << endl;
	fichier_mesure_autogaz << "N°mesure;Temps(s);Calorimètre(W);Basse Pression(Bar);Haute Pression(Bar);T°C Calo;T°C Cage;T°C pièce;Vanne 6;Etape;Dose;" << endl;
}

void CManip_AutoGaz::EnregistrementFichierMesures()
{
	char char_resultat_calo[20];
	sprintf(char_resultat_calo,"%.8E",resultat_calo);

	if(fichier_mesure_autogaz)
	{
		fichier_mesure_autogaz << numero_mesure << ";" << temps_manip << ";";
		fichier_mesure_autogaz << char_resultat_calo << ";" << resultat_bp << ";" << resultat_hp << ";";
		//fichier_mesure_autogaz << TemperatureCalo << ";" << TemperatureCage << ";" << TemperaturePiece << ";" << endl;
		fichier_mesure_autogaz << TemperatureCalo << ";" << TemperatureCage << ";" << TemperaturePiece << ";";
		fichier_mesure_autogaz << EstOuvert_Vanne(6) << ";";
		fichier_mesure_autogaz << etape_en_cours << ";";

		switch(etape_en_cours)
		{
			case LIGNE_EQUILIBRE :
			case ADSORPTION_CONTINUE :
			case MISE_SOUS_VIDE :
				fichier_mesure_autogaz << "0;";
				break;
			case PETITES_DOSES :
			case MOYENNES_DOSES :
			case GRANDES_DOSES :
				fichier_mesure_autogaz << dose << ";";
				break;
			case DESORPTION :
				fichier_mesure_autogaz << injection << ";";
				break;
			default :
				break;
		}

		fichier_mesure_autogaz << endl;
	}
}


void CManip_AutoGaz::FermetureFichierMesures()
{
	fichier_mesure_autogaz.close();
}

