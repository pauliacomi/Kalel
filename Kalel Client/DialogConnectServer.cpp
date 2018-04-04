// DialogConnectServer.cpp : implementation file
//

#include "stdafx.h"
#include "DialogConnectServer.h"

#include "afxdialogex.h"
#include "StringTable.h"						// Error message strings

#include <sstream>
#include <algorithm>

// DialogConnectServer dialog

IMPLEMENT_DYNAMIC(DialogConnectServer, CDialog)

DialogConnectServer::DialogConnectServer(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_SERVERCONNECT, pParent)
	, radioChoice(0)
	, ipAddress(0)
	, webAddress(_T(""))
	, port(_T("http"))
{

}

DialogConnectServer::~DialogConnectServer()
{
}

void DialogConnectServer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_LOCAL, radioChoice);
	DDX_IPAddress(pDX, IDC_IPADDRESS1, ipAddress);
	DDX_Text(pDX, IDC_EDIT1, webAddress);
	DDX_Text(pDX, IDC_EDIT2, username);
	DDX_Text(pDX, IDC_EDIT3, password);
	DDX_Text(pDX, IDC_EDIT4, port);
}


BEGIN_MESSAGE_MAP(DialogConnectServer, CDialog)
	ON_BN_CLICKED(IDCANCEL, &DialogConnectServer::OnIdcancel)
	ON_BN_CLICKED(IDC_BUTTON1, &DialogConnectServer::OnClickedConnect)
END_MESSAGE_MAP()


// DialogConnectServer message handlers


BOOL DialogConnectServer::OnInitDialog()
{
	CDialog::OnInitDialog();

	UpdateData(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void DialogConnectServer::OnIdcancel()
{
	OnCancel();
}

void DialogConnectServer::OnClickedConnect()
{
	UpdateData(TRUE);

	switch (radioChoice)
	{
	case -1:
		AfxMessageBox(ERROR_CONNECTION_INPUT, MB_OK);
		break;
	case 0:
		OnOK();
		break;
	case 1:
		if (webAddress == _T("")){
			AfxMessageBox(ERROR_CONNECTION_INPUT, MB_OK);
			break;
		}
		OnOK();
		break;
	case 2:
		if (ipAddress == NULL) {
			AfxMessageBox(ERROR_CONNECTION_INPUT, MB_OK);
			break;
		}
		OnOK();
		break;
	default:
		break;
	}
}

std::wstring DialogConnectServer::GetAddress()
{
	std::wstring address;
	CString url;

	switch (radioChoice)
	{
	case 0:
		address = L"localhost";
		break;
	case 1:
		address = webAddress.GetBuffer();
		break;
	case 2:

		unsigned char a, b, c, d;
		d = ipAddress & 0xFF;
		c = (ipAddress >> 8) & 0xFF;
		b = (ipAddress >> 16) & 0xFF;
		a = (ipAddress >> 24) & 0xFF;

		url.Format(_T("%u.%u.%u.%u"), a, b, c, d);
		address = url.GetBuffer();

		break;
	default:
		break;
	}

	return address;
}

std::wstring DialogConnectServer::GetPort()
{
	return std::wstring(port);
}

std::wstring DialogConnectServer::GetUsername()
{
	return std::wstring(username);
}


std::wstring DialogConnectServer::GetPassword()
{
	return std::wstring(password);
}

void DialogConnectServer::SetConnection(const std::wstring &address, const std::wstring &port)
{
	if (address == L"localhost")
	{
		radioChoice = 0;
	}
	else if (address.substr(0, 3) == L"www")
	{
		radioChoice = 1;
		webAddress = address.c_str();
	}
	else if (std::count(address.begin(), address.end(), '.') == 3)
	{
		radioChoice = 2;
		std::wstringstream str(address);
		int a, b, c, d; //to store the 4 ints
		wchar_t ch; //to temporarily store the '.'
		str >> a >> ch >> b >> ch >> c >> ch >> d;
		ipAddress = (a << 24) + (b << 16) + (c << 8) + d;
	}

	this->port = port.c_str();
}

void DialogConnectServer::SetCredentials(const std::wstring &username, const std::wstring &password)
{
	this->username = username.c_str();
	this->password = password.c_str();
}