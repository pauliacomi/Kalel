#include "StdAfx.h"
#include "Manip.h"

#include "MFCMessageHandler.h"		// Will handle all communication from base functionality to MFC


// ------------ Constructor and Destructor

CManip::CManip()
{
	manip_en_cours = FALSE;
	// this should most definately not be defined here, it's so wrong it hurts
	// m_proprietes_experience = new CProprietes_experience("");
	for(int i=0;i<NB_OF_INSTRUMENTS;i++)
		instrument[i] = new CInstrument();
	AppareilCalo.voie_mesure = VOIE_INDEF;
	AppareilCalo.index = INDEX_INDEF;
	AppareilHP.voie_mesure = VOIE_INDEF;
	AppareilHP.index = INDEX_INDEF;
	AppareilBP.voie_mesure = VOIE_INDEF;
	AppareilBP.index = INDEX_INDEF;
}

CManip::~CManip()
{
	for (int i = 0; i<NB_OF_INSTRUMENTS; i++){
		//delete instrument[i];    // needed but throws an error currently
	}
}


// -------------- Functions that set the pointers to external objects
// This is most likely not thread safe so must rewrite

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



// -------------- Initialisation functions --------------------

// Complete initialisation (Graph reset + Instrument definition)
void CManip::InitialisationManip()
{
	InitialisationInstruments();
	OuvrirInstruments();
}


// The instruments which the calorimeter uses are defined here
// It looks good, might need some error detection. Canditate for new class.
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

	for(int i=1;i<=NB_OF_INSTRUMENTS;i++)
	{
		switch(GetTypeInstrument(i))
		{
			case INSTRUMENT_KEITHLEY:
				instrument[index_instr]->SetParametresInstrument(GetCOMInstrument(i),INSTRUMENT_KEITHLEY);
				switch(GetFonctionInstrument(i))
				{
					case CALO_V1_BP_V2_KEITHLEY :
						AppareilCalo.voie_mesure = INSTRUMENT_KEYTHLEY_V1;
						AppareilCalo.index = index_instr;
						AppareilBP.voie_mesure = INSTRUMENT_KEYTHLEY_V2;
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
						AppareilCalo.voie_mesure = INSTRUMENT_KEYTHLEY_V1;
						AppareilCalo.index = index_instr;
						AppareilHP.voie_mesure = INSTRUMENT_KEYTHLEY_V2;
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
						AppareilCalo.voie_mesure = INSTRUMENT_KEYTHLEY_V1;
						AppareilCalo.index = index_instr;
						if(instrument[index_instr]->COM == 1)
							synchCalo = PassageCOM1;
						else
							synchCalo = PassageCOMs;
						break;

					case INSTRUMENT_KEYTHLEY_LP_V2 :
						AppareilBP.voie_mesure = INSTRUMENT_KEYTHLEY_V2;
						AppareilBP.index = index_instr;
						if(instrument[index_instr]->COM == 1)
							synchBP = PassageCOM1;
						else
							synchBP = PassageCOMs;
						break;

					case INSTRUMENT_KEYTHLEY_HP_V2 :
						AppareilHP.voie_mesure = INSTRUMENT_KEYTHLEY_V2;
						AppareilHP.index = index_instr;
						if(instrument[index_instr]->COM == 1)
							synchHP = PassageCOM1;
						else
							synchHP = PassageCOMs;
						break;

					default : //probl�me
						break;
				}
				index_instr++;
				break;
			case INSTRUMENT_MENSOR:
				instrument[index_instr]->SetParametresInstrument(GetCOMInstrument(i),INSTRUMENT_MENSOR);
				if(GetFonctionInstrument(i) == INSTRUMENT_MENSOR_LP)
				{
					AppareilBP.voie_mesure = MENSOR_VOIE;
					AppareilBP.index = index_instr;
					if(instrument[index_instr]->COM == 1)
						synchBP = PassageCOM1;
					else
						synchBP = PassageCOMs;
				}
				else //if(GetFonctionInstrument(i) == INSTRUMENT_MENSOR_HP)
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
			case INSTRUMENT_NONE:
			case INSTRUMENT_UNDEF:
			case INSTRUMENT_INEXIST:
			default:
				break;
		}
	}
}


// Set the type of experiment being done (AUTOMATIC || MANUAL)
void CManip::SetManipType(int experimentType)
{
	TypeExperience = experimentType;
	GenreExperience = experimentType;
}


// Tell the main view to update itself - this should be done by using a message command
void CManip::MiseAJour()
{
	messageHandler.UpdateDisplay();
}

// Tell the main view to unlock the menu - this should be done by using a message command
void CManip::DebloqueMenu()
{ 
	messageHandler.UnlockMenu();
}

// Tell the main view to unlock the launch button - this should be done by using a message command
void CManip::RemettreBoutonLancer()
{
	messageHandler.EnableStartButton();
}

// Manually reinitialise the experimental properties
void CManip::ReinitialisationManuelle()
{
	m_proprietes_experience->ReinitialisationManuelle();
}


