// MainFrm.h : interface de la classe CMainFrame
//


#pragma once

class CMainFrame : public CFrameWnd
{
	
protected: // cr�ation � partir de la s�rialisation uniquement
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributs
protected:
	CSplitterWnd m_wndSplitter;
public:

// Op�rations
public:

// Substitutions
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Impl�mentation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // membres incorpor�s de la barre de contr�le
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Fonctions g�n�r�es de la table des messages
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
};


