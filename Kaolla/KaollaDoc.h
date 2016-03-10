// KaollaDoc.h : interface de la classe CKaollaDoc
//


#pragma once



#include "Mesure.h"

#include "Parametres_appareil.h"
#include "Connection_port.h"

class CKaollaDoc : public CDocument
{
protected: // cr�ation � partir de la s�rialisation uniquement
	CKaollaDoc();
	DECLARE_DYNCREATE(CKaollaDoc)

// Attributs
public:
	CArrayMesure m_TableauMesures;
	double max_pression,min_pression,max_calo,min_calo;
	float TempsMinimum;
	int MesureMinimum,NumeroEtape;

	CParametres_appareil param;

// Op�rations
public:

// Substitutions
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Impl�mentation
public:
	virtual ~CKaollaDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Fonctions g�n�r�es de la table des messages
protected:
	DECLARE_MESSAGE_MAP()

public:
	void RajoutMesure(CMesure NouvellesMesures);
	void RajoutMesure(int num, double tps, double calorimetre, double bpression, double hpression, double temp_calo, double temp_cage, double temp_piece);
	void RajoutMesure(double tps, double calorimetre, double bpression, double hpression, double temp_calo, double temp_cage, double temp_piece);
	CArrayMesure* GetTableauMesures(void);

	CString TitreGrapheEtape;
};


