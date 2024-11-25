#pragma once
#include "afxdialogex.h"
#include "ipc2023Dlg.h"

class Cipc2023Dlg;

// Cipc2023SubDlg ��ȭ ����
class Cipc2024Sub2Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(Cipc2024Sub2Dlg)

public:
	Cipc2024Sub2Dlg(CWnd* pParent = nullptr);   // ǥ�� �������Դϴ�.
	Cipc2023Dlg* parent;
	void setter(Cipc2023Dlg* dlg);
	virtual ~Cipc2024Sub2Dlg();

	// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
