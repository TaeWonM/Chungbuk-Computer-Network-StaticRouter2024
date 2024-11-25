#pragma once
#include "afxdialogex.h"
#include "ipc2023Dlg.h"

class Cipc2023Dlg;

// Cipc2023SubDlg 대화 상자
class Cipc2024Sub2Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(Cipc2024Sub2Dlg)

public:
	Cipc2024Sub2Dlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	Cipc2023Dlg* parent;
	void setter(Cipc2023Dlg* dlg);
	virtual ~Cipc2024Sub2Dlg();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	CComboBox m_Combobox;
	CIPAddressCtrl m_DstIp;
	CIPAddressCtrl m_GateWay;
	CIPAddressCtrl m_NetMask;
	CButton m_Check_Up;
	CButton m_Check_GateWay;
	CButton m_Check_Host;
public:
	CEdit m_unMacAddr;
	afx_msg void OnCbnSelchangeCombo();
	afx_msg void OnClose();
	virtual void PostNcDestroy();
	virtual BOOL OnInitDialog();
	void SetComboboxlist();
	virtual void OnOK();
	afx_msg void OnBnClickedCancel();
};
