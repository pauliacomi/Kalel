#include "stdafx.h"
#include "Kaolla.h"
#include "Connection_port.h"

/* --------- InitDonnees ----------------*/

void CConnection_port::InitDonneesInstrument1()
{
	InitDonneesInstrument(GetTypeInstrument1(),
						  &m_nIndexTypeInstrument1,
						  &m_nPortInstrument1,
						  &m_bInstrument1KeithleyVoie1,
						  &m_bInstrument1KeithleyVoie2,
						  &m_CBInstrument1KeithleyVoie2,
						  &m_nIndexInstrument1KeithleyVoie2,
						  &m_nInstrument1Mensor,
						  GetCOMInstrument1()-1,
						  GetFonctionInstrument1());
}

void CConnection_port::InitDonneesInstrument2()
{
	InitDonneesInstrument(GetTypeInstrument2(),
						  &m_nIndexTypeInstrument2,
						  &m_nPortInstrument2,
						  &m_bInstrument2KeithleyVoie1,
						  &m_bInstrument2KeithleyVoie2,
						  &m_CBInstrument2KeithleyVoie2,
						  &m_nIndexInstrument2KeithleyVoie2,
						  &m_nInstrument2Mensor,
						  GetCOMInstrument2()-1,
						  GetFonctionInstrument2());
}
void CConnection_port::InitDonneesInstrument3()
{
	InitDonneesInstrument(GetTypeInstrument3(),
						  &m_nIndexTypeInstrument3,
						  &m_nPortInstrument3,
						  &m_bInstrument3KeithleyVoie1,
						  &m_bInstrument3KeithleyVoie2,
						  &m_CBInstrument3KeithleyVoie2,
						  &m_nIndexInstrument3KeithleyVoie2,
						  &m_nInstrument3Mensor,
						  GetCOMInstrument3()-1,
						  GetFonctionInstrument3());
}

void CConnection_port::InitDonneesInstrument(int TypeInstr,int* m_nIndex,int* m_nPortInstrument,BOOL* m_bInstrumentKeithleyVoie1,BOOL* m_bInstrumentKeithleyVoie2,CComboBox* m_CBInstrumentKeithleyVoie2,int* m_nIndexInstrumentKeithleyVoie2,int* m_nInstrumentMensor,int GetPort,int GetFonction)
{
	switch(TypeInstr)
	{
		case AUCUN_APPAREIL:
		case TYPE_INDEF:
		case TYPE_INEXIST:
			*m_nIndex = INDEX_AUCUN;
			break;

		case APPAREIL_KEITHLEY:
			*m_nIndex = INDEX_KEITHLEY;
			*m_nPortInstrument = GetPort;
			switch(GetFonction)
			{
				case CALO_V1_BP_V2_KEITHLEY :
					*m_bInstrumentKeithleyVoie1 = TRUE;
					*m_bInstrumentKeithleyVoie2 = TRUE;
					m_CBInstrumentKeithleyVoie2->EnableWindow (TRUE);
					*m_nIndexInstrumentKeithleyVoie2 = LECTURE_BP;
					break;
				case CALO_V1_HP_V2_KEITHLEY :
					*m_bInstrumentKeithleyVoie1 = TRUE;
					*m_bInstrumentKeithleyVoie2 = TRUE;
					m_CBInstrumentKeithleyVoie2->EnableWindow (TRUE);
					*m_nIndexInstrumentKeithleyVoie2 = LECTURE_HP;
					break;
				case CALO_V1_KEITHLEY :
					*m_bInstrumentKeithleyVoie1 = TRUE;
					*m_bInstrumentKeithleyVoie2 = FALSE;
					m_CBInstrumentKeithleyVoie2->EnableWindow (FALSE);
					break;
				case BP_V2_KEITHLEY :
					*m_bInstrumentKeithleyVoie1 = FALSE;
					*m_bInstrumentKeithleyVoie2 = TRUE;
					m_CBInstrumentKeithleyVoie2->EnableWindow (TRUE);
					*m_nIndexInstrumentKeithleyVoie2 = LECTURE_BP;
					break;
				case HP_V2_KEITHLEY :
					*m_bInstrumentKeithleyVoie1 = FALSE;
					*m_bInstrumentKeithleyVoie2 = TRUE;
					m_CBInstrumentKeithleyVoie2->EnableWindow (TRUE);
					*m_nIndexInstrumentKeithleyVoie2 = LECTURE_HP;
					break;
				default :
					break;
			}
			break;

		case APPAREIL_MENSOR:
			*m_nIndex = INDEX_MENSOR;
			*m_nPortInstrument = GetPort;
			if(GetFonction == BP_MENSOR)
				*m_nInstrumentMensor = LECTURE_BP;
			else // if(GetFonction == HP_MENSOR)
				*m_nInstrumentMensor = LECTURE_HP;
			break;
		default:
			break;
	}
}


/* ----------- EnregistrementParametresInstrument ----------- */


void CConnection_port::EnregistrementParametresInstrument1()
{
	EnregistrementParametresInstrument(1,
									   m_nIndexTypeInstrument1,
									   m_nPortInstrument1 + 1,
									   m_bInstrument1KeithleyVoie1,
									   m_bInstrument1KeithleyVoie2,
									   m_nIndexInstrument1KeithleyVoie2,
									   m_nInstrument1Mensor);
}

void CConnection_port::EnregistrementParametresInstrument2()
{
	EnregistrementParametresInstrument(2,
									   m_nIndexTypeInstrument2,
									   m_nPortInstrument2 + 1,
									   m_bInstrument2KeithleyVoie1,
									   m_bInstrument2KeithleyVoie2,
									   m_nIndexInstrument2KeithleyVoie2,
									   m_nInstrument2Mensor);
}

void CConnection_port::EnregistrementParametresInstrument3()
{
	EnregistrementParametresInstrument(3,
									   m_nIndexTypeInstrument3,
									   m_nPortInstrument3 + 1,
									   m_bInstrument3KeithleyVoie1,
									   m_bInstrument3KeithleyVoie2,
									   m_nIndexInstrument3KeithleyVoie2,
									   m_nInstrument3Mensor);
}

void CConnection_port::EnregistrementParametresInstrument(int num_instr,int m_nIndex,int COMInstrument,bool m_bInstrumentKeithleyVoie1, bool m_bInstrumentKeithleyVoie2,int m_nIndexInstrumentKeithleyVoie2,int m_nInstrumentMensor)
{	
	int fonction;
	switch(m_nIndex)
	{
		case INDEX_AUCUN:
			SetInstrument(num_instr,AUCUN_APPAREIL,-1,AUCUNE_FONCTION);
			break;
		case INDEX_KEITHLEY: 
			if(m_bInstrumentKeithleyVoie1 && m_bInstrumentKeithleyVoie2)
			{
				if(m_nIndexInstrumentKeithleyVoie2 == LECTURE_BP)
					fonction = CALO_V1_BP_V2_KEITHLEY;
				else //if(m_nIndexInstrumentKeithleyVoie2 == LECTURE_HP)
					fonction = CALO_V1_HP_V2_KEITHLEY;
			}
			else if(m_bInstrumentKeithleyVoie1)
				fonction = CALO_V1_KEITHLEY;
			else if(m_bInstrumentKeithleyVoie2)
			{
				if(m_nIndexInstrumentKeithleyVoie2 == LECTURE_BP)
					fonction = BP_V2_KEITHLEY;
				else //if(m_nIndexInstrumentKeithleyVoie2 == LECTURE_HP)
					fonction = HP_V2_KEITHLEY;
			}
			else
			{
				// Problème
				fonction = AUCUNE_FONCTION;
			}
			SetInstrument(num_instr,APPAREIL_KEITHLEY,COMInstrument,fonction);
			break;
		case INDEX_MENSOR:
			if(m_nInstrumentMensor == 0)
				fonction = BP_MENSOR;
			else //(m_nInstrument1Mensor == 1)
				fonction = HP_MENSOR;
			SetInstrument(num_instr,APPAREIL_MENSOR,COMInstrument,fonction);
		default:
			break;
	}
}


/* ------------- ShowItem ------------------ */


void CConnection_port::ShowItemInstrument1()
{
	ShowItem(m_nIndexTypeInstrument1,
			 m_bInstrument1KeithleyVoie2,
			 &m_CBPortInstrument1,
			 &m_CheckInstrument1KeithleyVoie1,
			 &m_CheckInstrument1KeithleyVoie2,
			 &m_CBInstrument1KeithleyVoie2,
			 &m_CBInstrument1Mensor);
}

void CConnection_port::ShowItemInstrument2()
{
	ShowItem(m_nIndexTypeInstrument2,
			 m_bInstrument2KeithleyVoie2,
			 &m_CBPortInstrument2,
			 &m_CheckInstrument2KeithleyVoie1,
			 &m_CheckInstrument2KeithleyVoie2,
			 &m_CBInstrument2KeithleyVoie2,
			 &m_CBInstrument2Mensor);
}

void CConnection_port::ShowItemInstrument3()
{
	ShowItem(m_nIndexTypeInstrument3,
			 m_bInstrument3KeithleyVoie2,
			 &m_CBPortInstrument3,
			 &m_CheckInstrument3KeithleyVoie1,
			 &m_CheckInstrument3KeithleyVoie2,
			 &m_CBInstrument3KeithleyVoie2,
			 &m_CBInstrument3Mensor);
}

void CConnection_port::ShowItem(int m_nIndex,bool m_bInstrumentKeithleyVoie2,CComboBox* m_CBPortInstrument,CButton* m_CheckInstrumentKeithleyVoie1,CButton* m_CheckInstrumentKeithleyVoie2,CComboBox* m_CBInstrumentKeithleyVoie2,CComboBox* m_CBInstrumentMensor)
{
	switch(m_nIndex)
	{
		case INDEX_AUCUN :
		case TYPE_INDEF:
		case TYPE_INEXIST:
			m_CBPortInstrument->ShowWindow(SW_HIDE);
			m_CheckInstrumentKeithleyVoie1->ShowWindow(SW_HIDE);
			m_CheckInstrumentKeithleyVoie2->ShowWindow(SW_HIDE);
			m_CBInstrumentKeithleyVoie2->ShowWindow(SW_HIDE);
			m_CBInstrumentKeithleyVoie2->EnableWindow (m_bInstrumentKeithleyVoie2);
			m_CBInstrumentMensor->ShowWindow(SW_HIDE);
			break;
		case INDEX_KEITHLEY :
			m_CBPortInstrument->ShowWindow(SW_SHOW);
			m_CheckInstrumentKeithleyVoie1->ShowWindow(SW_SHOW);
			m_CheckInstrumentKeithleyVoie2->ShowWindow(SW_SHOW);
			m_CBInstrumentKeithleyVoie2->ShowWindow(SW_SHOW);
			m_CBInstrumentKeithleyVoie2->EnableWindow (m_bInstrumentKeithleyVoie2);
			m_CBInstrumentMensor->ShowWindow(SW_HIDE);
			break;
		case INDEX_MENSOR :
			m_CBPortInstrument->ShowWindow(SW_SHOW);
			m_CheckInstrumentKeithleyVoie1->ShowWindow(SW_HIDE);
			m_CheckInstrumentKeithleyVoie2->ShowWindow(SW_HIDE);
			m_CBInstrumentKeithleyVoie2->ShowWindow(SW_HIDE);
			m_CBInstrumentKeithleyVoie2->EnableWindow (m_bInstrumentKeithleyVoie2);
			m_CBInstrumentMensor->ShowWindow(SW_SHOW);
			break;
		default:
			break;
	}
}
