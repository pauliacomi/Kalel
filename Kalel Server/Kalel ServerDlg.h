
// Kalel ServerDlg.h : header file
//
#pragma once

#include "Automation/ThreadManager.h"
#include <memory>

// CKalelServerDlg dialog
class CKalelServerDlg : public CDHtmlDialog
{
// Construction
public:
	CKalelServerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KALELSERVER_DIALOG, IDH = IDR_HTML_KALELSERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// Implementation
protected:
	HICON m_hIcon;

	std::unique_ptr<ThreadManager> threadManager;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()

};
