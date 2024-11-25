#pragma once
#include "afxdialogex.h"
#include "ipc2023Dlg.h"


// Cipc2023SubDlg_Router 대화 상자

class Cipc2023SubDlg_Router : public CDialogEx
{
	DECLARE_DYNAMIC(Cipc2023SubDlg_Router)

public:
	Cipc2023SubDlg_Router(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	Cipc2023Dlg* parent;
	void setter(Cipc2023Dlg* dlg);
	virtual ~Cipc2023SubDlg_Router();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	CComboBox m_RCombobox;
	CIPAddressCtrl m_RDST_IP;
	CIPAddressCtrl m_RNetMask;
	CIPAddressCtrl m_RGateWay;
	CButton m_CHECK_UP;
	CButton m_CHECK_Gate;
	CButton m_CHECK_Host;
public:
	afx_msg void OnBnClickedCheckUp();
	afx_msg void OnBnClickedCheckGateway();
	afx_msg void OnBnClickedCheckHost();
	afx_msg void OnCbnSelChangeComboInterface();
	virtual void OnOK();
	
};
