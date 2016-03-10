// KaollaView.h : interface de la classe CKaollaView
//

#ifndef _KAOLLAVIEW_H_
#define _KAOLLAVIEW_H_

#pragma once
#include "afxwin.h"


#include "Parametres_appareil.h"
#include "Connection_port.h"

#include <afxmt.h> // CEvent

using namespace std;

#define WM_THREADAFFICHAGE		WM_USER + 6

// tableaux des IDC des boutons des vannes 'Ouvrir' et 'fermer'
const int idc_ouvrir[] = {IDC_OUVRIR1,IDC_OUVRIR2,IDC_OUVRIR3,IDC_OUVRIR4,
						  IDC_OUVRIR5,IDC_OUVRIR6,IDC_OUVRIR7,IDC_OUVRIR8};

const int idc_fermer[] = {IDC_FERMER1,IDC_FERMER2,IDC_FERMER3,IDC_FERMER4,
						  IDC_FERMER5,IDC_FERMER6,IDC_FERMER7,IDC_FERMER8};

const int idc_temoin[] = {IDC_TEMOIN_VANNE1,IDC_TEMOIN_VANNE2,IDC_TEMOIN_VANNE3,IDC_TEMOIN_VANNE4,
						  IDC_TEMOIN_VANNE5,IDC_TEMOIN_VANNE6,IDC_TEMOIN_VANNE7,IDC_TEMOIN_VANNE8};


class CKaollaView : public CFormView
{

public:
	CParametres_appareil* m_parametres_appareil;
	CConnection_port* m_connection_port;

	BOOL fin_experience; // TRUE une fois que le bouton "stop" est déclenché
                         // Permet de faire la dernière mesure et de l'enregistrer.

	CEdit pEditMessages;
	CString m_StrEditMessages;
	CString m_StrCalo;
	CString m_StrBassePression;
	CString m_StrHautePression;
	CString m_StrTemperatureCalo;
	CString m_StrTemperatureCage;
	CString m_StrTemperaturePiece;
	CString m_StrTemps;
	CButton m_ButtonLancer;
	CButton m_ButtonArreter;
	CEdit pEditMesures;
	CString m_StrEditMesures;


	float TemperatureCalo;
	float TemperatureCage; 
    float TemperaturePiece;
	string Calorimetre;
	string Basse_Pression;
	double Haute_Pression;

	CString m_StrPressionInitiale;
	CString m_StrPressionFinale;
	CString m_StrEtape;

	CString m_StrTemoinVanne1;
	CString m_StrTemoinVanne2;
	CString m_StrTemoinVanne3;
	CString m_StrTemoinVanne4;
	CString m_StrTemoinVanne5;
	CString m_StrTemoinVanne6;
	CString m_StrTemoinVanne7;
	CString m_StrTemoinVanne8;
	CString m_StrTemoinEV1;
	CString m_StrTemoinEV2;
	CString m_StrTemoinPompe;

protected: // création à partir de la sérialisation uniquement
	CKaollaView();
	DECLARE_DYNCREATE(CKaollaView)

public:
	enum{ IDD = IDD_KAOLLA_FORM };

// Attributs
public:
	CKaollaDoc* GetDocument() const;

// Opérations
public:

// Substitutions
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV
	virtual void OnInitialUpdate(); // premier appel après la construction

// Implémentation
public:
	virtual ~CKaollaView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Fonctions générées de la table des messages
protected:
	DECLARE_MESSAGE_MAP()
public:

	// KaollaView

	void DoEvents(void);
	void ChargerDocument(void);
	void DebloqueMenu(void);
	void MiseAJour();
	void LancementThreadMSVAmpoule();
	void LancementThreadMSVBouteille();
	void LancementThreadChangementBouteille();

	// KaollaView_Affichage

	void AffichageMessages(CString message);
	void RajoutAffichageMessages(CString rajout);
	void AffichageMesures(CString mesure);
	void AffichageEtape(CString etape);
	void RajoutAffichageEtape(CString rajout);

	LRESULT OnThreadAffichage(WPARAM wParam, LPARAM lParam);
	int MessageBoxConfirmation(CString message, UINT nType);

	// KaollaView_Boutons

	afx_msg void OnBnClickedLancer();
	afx_msg void OnBnClickedArreter();
	void ConfirmationLancement();
	void Annuler();
	
	afx_msg void OnBnClickedButtonParametresExperience();
	afx_msg void OnBnClickedArretSousVide();
	afx_msg void OnBnClickedPause();
	afx_msg void OnBnClickedProchaineCommande();
	afx_msg void OnBnClickedProchaineDose();
	afx_msg void OnBnClickedProchaineEtape();
	afx_msg void OnBnClickedReprise();

	// KaollaView_Boutons_Vannes

	afx_msg void OnBnClickedOuvrir1();
	afx_msg void OnBnClickedOuvrir2();
	afx_msg void OnBnClickedOuvrir3();
	afx_msg void OnBnClickedOuvrir4();
	afx_msg void OnBnClickedOuvrir5();
	afx_msg void OnBnClickedOuvrir6();
	afx_msg void OnBnClickedOuvrir7();
	afx_msg void OnBnClickedOuvrir8();

	afx_msg void OnBnClickedFermer1();
	afx_msg void OnBnClickedFermer2();
	afx_msg void OnBnClickedFermer3();
	afx_msg void OnBnClickedFermer4();
	afx_msg void OnBnClickedFermer5();
	afx_msg void OnBnClickedFermer6();
	afx_msg void OnBnClickedFermer7();
	afx_msg void OnBnClickedFermer8();

	afx_msg void OnBnClickedActiverEV1();
	afx_msg void OnBnClickedDesactiverEV1();
	afx_msg void OnBnClickedActiverEV2();
	afx_msg void OnBnClickedDesactiverEV2();
	afx_msg void OnBnClickedActiverPompe();
	afx_msg void OnBnClickedDesactiverPompe();

	void Ouverture(int i);
	void Fermeture(int i);

	void ActivationEV1();
	void DesactivationEV1();
	void ActivationEV2();
	void DesactivationEV2();
	void ActivationPompe();
	void DesactivationPompe();

	void MarquerTemoin(int num_vanne, CString message);
	void MarquerTousLesTemoinsFermes();
	void MarquerEV1(CString message);
	void MarquerEV2(CString message);
	void MarquerPompe(CString message);
	void MarquerValvesEtPompeDesactivees();

	afx_msg void OnMsvAmpoule();
	afx_msg void OnUpdateMsvAmpoule(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // version de débogage dans KaollaView.cpp
inline CKaollaDoc* CKaollaView::GetDocument() const
   { return reinterpret_cast<CKaollaDoc*>(m_pDocument); }
#endif

#endif