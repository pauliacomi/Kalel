
// Kalel ServerDlg.h : header file
//
#pragma once

#include "Kalel.h"

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


	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()

	Kalel mainBackend;

	CEdit pEditMessages;
	CString displayText;
	int logSize;
};
