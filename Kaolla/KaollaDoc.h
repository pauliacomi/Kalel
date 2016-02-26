
// KaollaDoc.h : interface of the CKaollaDoc class
//


#pragma once

#include "Mesure.h"
#include "Parametres_appareil.h"
#include "Connection_port.h"

class CKaollaDoc : public CDocument
{
protected: // create from serialization only
	CKaollaDoc();
	DECLARE_DYNCREATE(CKaollaDoc)

// Attributes
public:	
	bool experiment_running;	// TRUE when the experiment is running
								// Allows to alert the user of possible loss of data

	CArrayMesure m_TableauMesures;
	double max_pression, min_pression, max_calo, min_calo;
	float TempsMinimum;
	int MesureMinimum, NumeroEtape;

	CParametres_appareil param;

// Operations
public:
	static CKaollaDoc * GetDocument();

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CKaollaDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

public:
	void RajoutMesure(CMesure NouvellesMesures);
	void RajoutMesure(int num, double tps, double calorimetre, double bpression, double hpression, double temp_calo, double temp_cage, double temp_piece);
	void RajoutMesure(double tps, double calorimetre, double bpression, double hpression, double temp_calo, double temp_cage, double temp_piece);
	CArrayMesure* GetTableauMesures(void);

	CString TitreGrapheEtape;

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
