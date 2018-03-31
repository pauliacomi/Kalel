// SpinBtnCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "SpinBtnCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpinBtnCtrl

CSpinBtnCtrl::CSpinBtnCtrl()
{
	m_min = 0.0;
	m_max = 100.0;
	m_inc = 1.0;
	m_pos = 0.0;
	m_nbDigits = 1;
	m_strFormat = "%.1f";
}

CSpinBtnCtrl::~CSpinBtnCtrl()
{
}


BEGIN_MESSAGE_MAP(CSpinBtnCtrl, CSpinButtonCtrl)
	//{{AFX_MSG_MAP(CSpinBtnCtrl)
	ON_NOTIFY_REFLECT(UDN_DELTAPOS, &CSpinBtnCtrl::OnDeltapos)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CSpinBtnCtrl message handlers

void CSpinBtnCtrl::SetRange(double lower,double upper)
{
	m_min = lower;
	m_max = upper;
}
void CSpinBtnCtrl::GetRange(double & lower,double & upper) const
{
	lower = m_min;
	upper = m_max;
}

double CSpinBtnCtrl::SetPos(double pos)
{
	if ((pos >= m_min)&&(pos <= m_max))
	{
		double oldPos = m_pos;
		m_pos = pos;

		return oldPos;
	}
	return m_pos;
}

double CSpinBtnCtrl::GetPos() const
{
	return m_pos;
}

double CSpinBtnCtrl::GetInc()
{
	return m_inc;
}
void CSpinBtnCtrl::SetInc(double inc)
{
	m_inc = inc;
}

void CSpinBtnCtrl::SetFormat(int nbDigits)
{
	m_nbDigits = nbDigits > 0 ? nbDigits : 1;
}

void CSpinBtnCtrl::SetFormat(CString strFormat)
{
	m_strFormat = strFormat;
}

void CSpinBtnCtrl::UpdateBuddy()
{
	CString strPos;
	//strPos.Format("%0.*f",m_nbDigits,m_pos);
	strPos.Format(m_strFormat,m_pos);

	CWnd* pWnd = GetBuddy();

	if (pWnd)
		pWnd->SetWindowText(strPos);
}

void CSpinBtnCtrl::OnDeltapos(NMHDR* pNMHDR, LRESULT* pResult) 
{
    NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;     
    HWND pWnd = ::GetWindow(pNMUpDown->hdr.hwndFrom, GW_HWNDPREV);

    if (pWnd != NULL)
	{   
		wchar_t cValue[20];
        ::GetWindowText(pWnd, cValue, 20);
       
		m_pos = _wtof(cValue);
   
        if (pNMUpDown->iDelta > 0)
			m_pos += m_inc;
        else
			m_pos -= m_inc;

        if (m_pos > m_max) m_pos = m_max;
        if (m_pos < m_min) m_pos = m_min;
       
		CString strValue;
		//strValue.Format("%.*f",m_nbDigits,m_pos);
		strValue.Format(m_strFormat,m_pos);
		::SetWindowText(pWnd, strValue);
    }   
    *pResult = 0; 
}
