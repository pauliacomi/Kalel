// KaollaDoc.cpp : implémentation de la classe CKaollaDoc
//

#include "stdafx.h"
#include "Kaolla.h"

#include "KaollaDoc.h"

#include "Mesure.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKaollaDoc

IMPLEMENT_DYNCREATE(CKaollaDoc, CDocument)

BEGIN_MESSAGE_MAP(CKaollaDoc, CDocument)
END_MESSAGE_MAP()


// construction ou destruction de CKaollaDoc

CKaollaDoc::CKaollaDoc()
: TitreGrapheEtape(_T(""))
{
	TempsMinimum = -1;
	MesureMinimum = -1;
	NumeroEtape = -1;

}

CKaollaDoc::~CKaollaDoc()
{
}

BOOL CKaollaDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO : ajoutez ici le code de réinitialisation
	// (les documents SDI réutiliseront ce document)

	return TRUE;
}




// sérialisation de CKaollaDoc

void CKaollaDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO : ajoutez ici le code de stockage
	}
	else
	{
		// TODO : ajoutez ici le code de chargement
	}
}


// diagnostics pour CKaollaDoc

#ifdef _DEBUG
void CKaollaDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CKaollaDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// commandes pour CKaollaDoc

void CKaollaDoc::RajoutMesure(CMesure NouvellesMesures)
{
	int dernier=m_TableauMesures.GetUpperBound();
	m_TableauMesures[dernier]=NouvellesMesures;
}

void CKaollaDoc::RajoutMesure(int num, double tps, double calorimetre, double bpression, double hpression, double temp_calo, double temp_cage, double temp_piece)
{
	//Mettre condition
	if(m_TableauMesures.IsEmpty())
	{
		max_pression = max(bpression,hpression);
		min_pression = min(bpression,hpression);

		max_calo=calorimetre;
		min_calo=calorimetre;
	}
	else
	{
		max_pression = max(max_pression,bpression);
		max_pression = max(max_pression,hpression);
		min_pression = min(min_pression,bpression);
		min_pression = min(min_pression,hpression);

		max_calo = max(max_calo,calorimetre);
		min_calo = min(min_calo,calorimetre);
	}
	m_TableauMesures.SetSize(num);
	m_TableauMesures[num-1]=CMesure(num,tps, calorimetre,bpression,hpression,temp_calo,temp_cage,temp_piece);
	int j=num-1;
	TRACE("m_TableauMesures[%d]=CMesure(%d,%d,%f,%f,%f,%f,%f,%f)\n",j,m_TableauMesures[j].numero,m_TableauMesures[j].temps,m_TableauMesures[j].calo,m_TableauMesures[j].basse_pression,
		m_TableauMesures[j].haute_pression,m_TableauMesures[j].temperature_calo,m_TableauMesures[j].temperature_cage,
		m_TableauMesures[j].temperature_piece);
	int a=m_TableauMesures[j].numero;
	double b=m_TableauMesures[j].temps;
	double c=m_TableauMesures[j].calo;
	double d=m_TableauMesures[j].basse_pression;
	int premier=1;
}

void CKaollaDoc::RajoutMesure(double tps, double calorimetre, double bpression, double hpression, double temp_calo, double temp_cage, double temp_piece)
{
	int dernier;
	if(m_TableauMesures.IsEmpty())
	{
		dernier=1;
		m_TableauMesures.SetSize(3);
	}
	else
	    dernier=m_TableauMesures.GetUpperBound();
	m_TableauMesures[dernier-1]=CMesure(dernier,tps, calorimetre,bpression,hpression,temp_calo,temp_cage,temp_piece);
	int j=dernier-1;
	TRACE("m_TableauMesures[%d]=CMesure(%d,%d,%f,%f,%f,%f,%f,%f)\n",j,m_TableauMesures[j].numero,
		m_TableauMesures[j].temps,m_TableauMesures[j].calo,m_TableauMesures[j].basse_pression,
		m_TableauMesures[j].haute_pression,m_TableauMesures[j].temperature_calo,m_TableauMesures[j].temperature_cage,
		m_TableauMesures[j].temperature_piece);
	int premier=1;
}



CArrayMesure* CKaollaDoc::GetTableauMesures(void)
{
	return &m_TableauMesures;
}


