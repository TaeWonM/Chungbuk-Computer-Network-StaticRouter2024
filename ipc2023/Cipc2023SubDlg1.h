#pragma once
#include "afxdialogex.h"


// Cipc2023SubDlg1 대화 상자

class Cipc2023SubDlg1 : public CDialogEx
{
	DECLARE_DYNAMIC(Cipc2023SubDlg1)

public:
	Cipc2023SubDlg1(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~Cipc2023SubDlg1();

	virtual void OnFinalRelease();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif
	CIPAddressCtrl m_IpAddressCtrl;
	CIPAddressCtrl m_NetMaskCtrl;
	CIPAddressCtrl m_GatewayCtrl;
	CComboBox m_InterfaceCombo;
	BOOL m_CheckUp;
	BOOL m_CheckGateway;
	BOOL m_CheckHost;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	afx_msg void OnIpnFieldchangedIpRouterIpaddress(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnIpnFieldchangedIpRouterNetMask(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnIpnFieldchangedIpRouterGateway(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedIpRouterCheckUp();
	afx_msg void OnBnClickedIpRouterCheckGateway();
	afx_msg void OnBnClickedIpRouterCheckHost();
	afx_msg void OnCbnSelchangeIpRouterComboInterface();
	afx_msg void OnBnClickedOk2();
	afx_msg void OnBnClickedCancel();
public:
	void GetDlgData(CString& destination, CString& netmask, CString& gateway, CString& netInterface, CString& flag);
};
