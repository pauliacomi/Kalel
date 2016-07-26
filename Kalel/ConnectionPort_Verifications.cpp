#include "stdafx.h"
#include "Kalel.h"
#include "ConnectionPort.h"


void ConnectionPort::Verifications()
{
	InitialisationVerifications();

	VerificationPortUSB();

	if(!AucunInstrumentBranche())
	{
		VerifInstrument1();
		VerifInstrument2();
		VerifInstrument3();

		VerificationLectureMesures();
	}

	VerificationPortCOM();

}

void ConnectionPort::InitialisationVerifications()
{
	index_verifInstrument = 0;
	bPbm = FALSE;
	bWarning = FALSE;
	bPbmCalo = FALSE;
	bPbmHP = FALSE;
	bPbmBP = FALSE;
	StrPbmCalo = _T("Plus d'un instrument mesurent la calorimétrie :");
	StrPbmBP = _T("Plus d'un instrument mesurent la basse pression :");
	StrPbmHP = _T("Plus d'un instrument mesurent la haute pression :");
	StrMessageErreur = _T("Erreur(s) : \t\n");
	StrMessageWarning = _T("Warning(s) : \t\n");
	CM_Calo.SetConnectionMesure(-1,-1);
	CM_HP.SetConnectionMesure(-1,-1);
	CM_BP.SetConnectionMesure(-1,-1);
}



void ConnectionPort::VerificationPortUSB()
{
	if(m_nIndexPortVannes == m_nIndexPortTemperatures)
	{
		bPbm = TRUE;
		CString MessageErreurUSB;
		MessageErreurUSB.Format(_T("Les ports Vannes et Températures sont les mêmes : Dev%d\t"),
									m_nIndexPortVannes + 1);
		StrMessageErreur += MessageErreurUSB;
	}
}

void ConnectionPort::VerificationPortCOM()
{
	for(int i=0;i<index_verifInstrument-1;i++)
	{
		for(int j=i+1;j<index_verifInstrument;j++)
		{
			if(verifInstrument[i].COM == verifInstrument[j].COM)
			{
				CString temp;
				temp.Format(_T("Même port entre l'instrument %d et l'instrument %d (COM%d)\t\n"),
								verifInstrument[i].numero,verifInstrument[j].numero,verifInstrument[i].COM);
				StrMessageErreur += temp;
				bPbm = TRUE;
			}
		}
	}
}


void ConnectionPort::VerificationLectureMesures()
{
	if(CM_Calo.index == -1)
	{
		bWarning = TRUE;
		StrMessageWarning += _T("Aucun appareil ne calcule la calorimétrie.\t\n");
	}

	if(CM_BP.index == -1)
	{
		bWarning = TRUE;
		StrMessageWarning += _T("Aucun appareil ne calcule la basse pression.\t\n");
	}

	if(CM_HP.index == -1)
	{
		bWarning = TRUE;
		StrMessageWarning += _T("Aucun appareil ne calcule la haute pression.\t\n");
	}
}

bool ConnectionPort::AucunInstrumentBranche()
{
	// Aucun appareil branché
	if(m_nIndexTypeInstrument1 == INDEX_AUCUN && 
	   m_nIndexTypeInstrument2 == INDEX_AUCUN && 
	   m_nIndexTypeInstrument3 == INDEX_AUCUN)
	{
		bPbm = TRUE;
		StrMessageErreur += _T("Aucun appareil n'est signalé\t\n");
		return TRUE;
	}

	return FALSE;
}



void ConnectionPort::VerifInstrument1()
{
	VerifUnInstrument(1,
					  m_nIndexTypeInstrument1,
					  m_bInstrument1KeithleyVoie1,
					  m_bInstrument1KeithleyVoie2,
					  m_nIndexInstrument1KeithleyVoie2,
					  m_nInstrument1Mensor,
					  m_nPortInstrument1+1);
}

void ConnectionPort::VerifInstrument2()
{
	VerifUnInstrument(2,
					  m_nIndexTypeInstrument2,
					  m_bInstrument2KeithleyVoie1,
					  m_bInstrument2KeithleyVoie2,
					  m_nIndexInstrument2KeithleyVoie2,
					  m_nInstrument2Mensor,
					  m_nPortInstrument2+1);
}

void ConnectionPort::VerifInstrument3()
{
	VerifUnInstrument(3,
					  m_nIndexTypeInstrument3,
					  m_bInstrument3KeithleyVoie1,
					  m_bInstrument3KeithleyVoie2,
					  m_nIndexInstrument3KeithleyVoie2,
					  m_nInstrument3Mensor,
					  m_nPortInstrument3+1);
}


void ConnectionPort::VerifUnInstrument(int num_instr,int m_nIndex,BOOL m_bKeithleyVoie1,BOOL m_bKeithleyVoie2,int m_nInstrumentKeithleyVoie2,int m_nMensor,int PortInstr)
{
	switch(m_nIndex)
	{
		case INDEX_AUCUN:
			break;
		case INDEX_KEITHLEY:
			verifInstrument[index_verifInstrument].numero = num_instr;
			verifInstrument[index_verifInstrument].type = INSTRUMENT_KEITHLEY;
			verifInstrument[index_verifInstrument].COM = PortInstr;
			index_verifInstrument++;
			
			if(!m_bKeithleyVoie1 && !m_bKeithleyVoie2)
			{
				bPbm = TRUE;
				CString temp;
				temp.Format(_T("Aucune lecture déclarée sur le Keithley de l'instrument %d\t\n"),num_instr);
				StrMessageErreur += temp;
			}
			
			if(m_bKeithleyVoie1)
				VerifMesure(&CM_Calo,num_instr, INSTRUMENT_KEYTHLEY_V1,&StrPbmCalo,&bPbmCalo);
				
			if(m_bKeithleyVoie2)
			{
				if(m_nInstrumentKeithleyVoie2 == READ_LP)
					VerifMesure(&CM_BP,num_instr,INSTRUMENT_KEYTHLEY_V2,&StrPbmBP,&bPbmBP);
				else //if(m_nInstrumentKeithleyVoie2 == READ_HP)
					VerifMesure(&CM_HP,num_instr,INSTRUMENT_KEYTHLEY_V2,&StrPbmHP,&bPbmHP);
			}
			break;
		case INDEX_MENSOR:
			verifInstrument[index_verifInstrument].numero = num_instr;
			verifInstrument[index_verifInstrument].type = INSTRUMENT_MENSOR;
			verifInstrument[index_verifInstrument].COM = PortInstr;
			index_verifInstrument++;
			
			if(m_nMensor == READ_LP)
				VerifMesure(&CM_BP,num_instr,MENSOR_VOIE,&StrPbmBP,&bPbmBP);
				
			if(m_nMensor == READ_HP)
				VerifMesure(&CM_HP,num_instr,MENSOR_VOIE,&StrPbmHP,&bPbmHP);
			break;
		default:
			break;
	}
}

void ConnectionPort::VerifMesure(ConnectionMesure* CM,int num_instr,int voie_mesure,CString* StrPbm,BOOL* bPbmMesure)
{
	if(CM->index != -1)
	{
		*bPbmMesure = TRUE;
		bPbm = TRUE;
	}
	else
		CM->SetConnectionMesure(voie_mesure,num_instr);
	CString temp;
	temp.Format(_T(" Instrument %d"),num_instr);
	*StrPbm += temp;
}

void ConnectionPort::EnregistrementVerifications()
{
	if(CM_Calo.index == -1)
		SetMesureCalo(FALSE);
	else
		SetMesureCalo(TRUE);

	if(CM_BP.index == -1)
		SetMesureBassePression(FALSE);
	else
		SetMesureBassePression(TRUE);

	if(CM_HP.index == -1)
		SetMesureHautePression(FALSE);
	else
		SetMesureHautePression(TRUE);
}