#include "StdAfx.h"
#include "Manip.h"



using namespace std;


CManip::CManip(void)
{
	m_KaollaView = NULL;
	manip_en_cours = FALSE;
	m_proprietes_experience = new CProprietes_experience("");
	for(int i=0;i<nb_instrument;i++)
		instrument[i] = new CInstrument();
	AppareilCalo.voie_mesure = VOIE_INDEF;
	AppareilCalo.index = INDEX_INDEF;
	AppareilHP.voie_mesure = VOIE_INDEF;
	AppareilHP.index = INDEX_INDEF;
	AppareilBP.voie_mesure = VOIE_INDEF;
	AppareilBP.index = INDEX_INDEF;
}

CManip::~CManip(void)
{
}



void CManip::SetKaollaView(CKaollaView* pKV)
{
	m_KaollaView = pKV;
}

void CManip::SetKeithley(Keithley* Keith)
{
	g_pKeithley = Keith;
}

void CManip::SetMensor(Mensor* Mens)
{
	g_pMensor = Mens;
}

void CManip::SetVannes(CVannes* pVannes)
{
	g_pVanne = pVannes;
}

void CManip::SetTemperature(CTemperature* pTemperature)
{
	g_pTemperature = pTemperature;
}



void CManip::InitialisationDocument()
{
	m_Doc = m_KaollaView->GetDocument();
	m_Doc->TempsMinimum = -1;
	m_Doc->MesureMinimum = -1;
	m_Doc->NumeroEtape = -1;
}



void CManip::InitialisationInstruments()
{
	int index_instr=0;

	AppareilCalo.voie_mesure = VOIE_INDEF;
	AppareilCalo.index = INDEX_INDEF;
	AppareilHP.voie_mesure = VOIE_INDEF;
	AppareilHP.index = INDEX_INDEF;
	AppareilBP.voie_mesure = VOIE_INDEF;
	AppareilBP.index = INDEX_INDEF;

	synchCalo = PassageNul;
	synchHP = PassageNul;
	synchBP = PassageNul;

	for(int i=1;i<=nb_instrument;i++)
	{
		switch(GetTypeInstrument(i))
		{
			case APPAREIL_KEITHLEY:
				instrument[index_instr]->SetParametresInstrument(GetCOMInstrument(i),APPAREIL_KEITHLEY);
				switch(GetFonctionInstrument(i))
				{
					case CALO_V1_BP_V2_KEITHLEY :
						AppareilCalo.voie_mesure = KEITHLEY_V1;
						AppareilCalo.index = index_instr;
						AppareilBP.voie_mesure = KEITHLEY_V2;
						AppareilBP.index = index_instr;
						if(instrument[index_instr]->COM == 1)
						{
							synchCalo = PassageCOM1;
							synchBP = PassageCOM1;
						}
						else
						{
							synchCalo = PassageCOMs;
							synchBP = PassageCOMs;
						}
						break;

					case CALO_V1_HP_V2_KEITHLEY :
						AppareilCalo.voie_mesure = KEITHLEY_V1;
						AppareilCalo.index = index_instr;
						AppareilHP.voie_mesure = KEITHLEY_V2;
						AppareilHP.index = index_instr;
						if(instrument[index_instr]->COM == 1)
						{
							synchCalo = PassageCOM1;
							synchHP = PassageCOM1;
						}
						else
						{
							synchCalo = PassageCOMs;
							synchHP = PassageCOMs;
						}
						break;

					case CALO_V1_KEITHLEY :
						AppareilCalo.voie_mesure = KEITHLEY_V1;
						AppareilCalo.index = index_instr;
						if(instrument[index_instr]->COM == 1)
							synchCalo = PassageCOM1;
						else
							synchCalo = PassageCOMs;
						break;

					case BP_V2_KEITHLEY :
						AppareilBP.voie_mesure = KEITHLEY_V2;
						AppareilBP.index = index_instr;
						if(instrument[index_instr]->COM == 1)
							synchBP = PassageCOM1;
						else
							synchBP = PassageCOMs;
						break;

					case HP_V2_KEITHLEY :
						AppareilHP.voie_mesure = KEITHLEY_V2;
						AppareilHP.index = index_instr;
						if(instrument[index_instr]->COM == 1)
							synchHP = PassageCOM1;
						else
							synchHP = PassageCOMs;
						break;

					default : //problème
						break;
				}
				index_instr++;
				break;
			case APPAREIL_MENSOR:
				instrument[index_instr]->SetParametresInstrument(GetCOMInstrument(i),APPAREIL_MENSOR);
				if(GetFonctionInstrument(i) == BP_MENSOR)
				{
					AppareilBP.voie_mesure = MENSOR_VOIE;
					AppareilBP.index = index_instr;
					if(instrument[index_instr]->COM == 1)
						synchBP = PassageCOM1;
					else
						synchBP = PassageCOMs;
				}
				else //if(GetFonctionInstrument(i) == HP_MENSOR)
				{
					AppareilHP.voie_mesure = MENSOR_VOIE;
					AppareilHP.index = index_instr;
					if(instrument[index_instr]->COM == 1)
						synchHP = PassageCOM1;
					else
						synchHP = PassageCOMs;
				}
				index_instr++;
				break;
			case AUCUN_APPAREIL:
			case TYPE_INDEF:
			case TYPE_INEXIST:
			default:
				break;
		}
	}
}

void CManip::InitialisationManip()
{
	InitialisationInstruments();
	OuvrirInstruments();
	InitialisationDocument();
}



void CManip::SetManipManuelle()
{
	TypeExperience = MANUEL;
	GenreExperience = MANUEL;
}

void CManip::SetManipAuto()
{
	TypeExperience = AUTO;
	GenreExperience = AUTO;
}




void CManip::MiseAJour()
{
	m_KaollaView->MiseAJour();
}


void CManip::DebloqueMenu()
{ 
	m_KaollaView->DebloqueMenu();
}

void CManip::RemettreBoutonLancer()
{
	m_KaollaView->GetDlgItem(IDC_LANCER)->EnableWindow(TRUE);
}


void CManip::ReinitialisationManuelle()
{
	m_proprietes_experience->ReinitialisationManuelle();
}


