#ifndef DIALOGCONNECTSERVER_H
#define DIALOGCONNECTSERVER_H
#pragma once

#include "afxwin.h"
#include "resource.h"

#include <string>

// DialogConnectServer dialog

class DialogConnectServer : public CDialog
{
	DECLARE_DYNAMIC(DialogConnectServer)

public:
	DialogConnectServer(CWnd* pParent = NULL);   // standard constructor
	virtual ~DialogConnectServer();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVERCONNECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
protected:
	int radioChoice;
	DWORD ipAddress;
	CString webAddress;
public:
	afx_msg void OnIdcancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnClickedConnect();

	std::wstring GetAddress();
};

#endif