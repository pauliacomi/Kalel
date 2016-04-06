#include "StdAfx.h"
#include "Manip.h"

// This part of the CManip class deals with displaying everything on the main user interface
// It should be re-written to use commands
 
// ---------- Measurements ----------------

void CManip::AffichageMesures()
{
	char char_resultat_calo[20];
	sprintf_s(char_resultat_calo,"%.8E",resultat_calo);

	ostringstream message;
	message << numero_mesure << ")  temps=" << temps_manip << "  ";
	message << "calo=" << char_resultat_calo << "  bp=" << resultat_bp << "  hp=" << resultat_hp << "  ";
	
	CString rajout_mesure;
	rajout_mesure.Format(_T("%s\r\n"),message.str().c_str());
	
	messageHandler.DisplayMeasurement(rajout_mesure);
}

