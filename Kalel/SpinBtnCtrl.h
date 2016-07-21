#if !defined(AFX_SPINBTNCTRL_H__98DE1443_58B6_43DA_ABBB_F9A2CA4417C7__INCLUDED_)
#define AFX_SPINBTNCTRL_H__98DE1443_58B6_43DA_ABBB_F9A2CA4417C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpinBtnCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSpinBtnCtrl window

class CSpinBtnCtrl : public CSpinButtonCtrl
{
// Constructor & Destructor
public:
	CSpinBtnCtrl();
	virtual ~CSpinBtnCtrl();

// Attributes
public:
	double m_min;
	double m_max;
	double m_inc;
	double m_pos;
	int m_nbDigits;
	CString m_strFormat;

// Operations
public:
	void SetRange(double lower,double upper);
	void GetRange(double & lower,double & upper) const;

	double SetPos(double pos);
	double GetPos() const;

	double GetInc();
	void SetInc(double inc);

	void SetFormat(int nbDigits);
	void SetFormat(CString strFormat);

	void UpdateBuddy();

// Overrides
// ClassWizard generated virtual function overrides
//{{AFX_VIRTUAL(CSpinBtnCtrl)
//}}AFX_VIRTUAL

// Generated message map functions
protected:
	//{{AFX_MSG(CSpinBtnCtrl)
	afx_msg void OnDeltapos(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPINBTNCTRL_H__98DE1443_58B6_43DA_ABBB_F9A2CA4417C7__INCLUDED_)
