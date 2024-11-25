// Cipc2023SubDlg_Router.cpp: 구현 파일
//

#include "pch.h"
#include "ipc2023.h"
#include "afxdialogex.h"
#include "Cipc2023SubDlg_Router.h"

#define ROUTER_LIST_CONTROLL_DES_ADDRESS 1
#define ROUTER_LIST_CONTROLL_NET_MASK 2
#define ROUTER_LIST_CONTROLL_GATE_WAY 3 
#define ROUTER_LIST_CONTROLL_FLAG 4
#define ROUTER_LIST_CONTROLL_DEVICE_ID 5
#define ROUTER_LIST_CONTROLL_METRIC 6

// Cipc2023SubDlg_Router 대화 상자

IMPLEMENT_DYNAMIC(Cipc2023SubDlg_Router, CDialogEx)

Cipc2023SubDlg_Router::Cipc2023SubDlg_Router(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{

}

Cipc2023SubDlg_Router::~Cipc2023SubDlg_Router()
{
}

void Cipc2023SubDlg_Router::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IP_ROUTER_IPADDRESS, m_RDST_IP);
	DDX_Control(pDX, IDC_IP_ROUTER_NET_MASK, m_RNetMask);
	DDX_Control(pDX, IDC_IP_ROUTER_GATEWAY, m_RGateWay);
	DDX_Control(pDX, IDC_IP_ROUTER_COMBO_INTERFACE, m_RCombobox);
	DDX_Control(pDX, IDC_IP_ROUTER_CHECK_UP, m_CHECK_UP);
	DDX_Control(pDX, IDC_IP_ROUTER_CHECK_GATEWAY, m_CHECK_Gate);
	DDX_Control(pDX, IDC_IP_ROUTER_CHECK_HOST, m_CHECK_Host);
}


BEGIN_MESSAGE_MAP(Cipc2023SubDlg_Router, CDialogEx)
	ON_BN_CLICKED(IDC_IP_ROUTER_CHECK_UP, &Cipc2023SubDlg_Router::OnBnClickedCheckUp)
	ON_BN_CLICKED(IDC_IP_ROUTER_CHECK_GATEWAY, &Cipc2023SubDlg_Router::OnBnClickedCheckGateway)
	ON_BN_CLICKED(IDC_IP_ROUTER_CHECK_HOST, &Cipc2023SubDlg_Router::OnBnClickedCheckHost)
	ON_CBN_SELCHANGE(IDC_IP_ROUTER_COMBO_INTERFACE, &Cipc2023SubDlg_Router::OnCbnSelChangeComboInterface)
END_MESSAGE_MAP()


// Cipc2023SubDlg_Router 메시지 처리기

void Cipc2023SubDlg_Router::OnBnClickedCheckUp()
{
	// TODO: 체크박스 "UP" 클릭 시 동작 구현
}

void Cipc2023SubDlg_Router::OnBnClickedCheckGateway()
{
	// TODO: 체크박스 "Gateway" 클릭 시 동작 구현
}

void Cipc2023SubDlg_Router::OnBnClickedCheckHost()
{
	// TODO: 체크박스 "Host" 클릭 시 동작 구현
}
void Cipc2023SubDlg_Router::OnCbnSelChangeComboInterface()
{
	// TODO: 콤보박스 선택 변경 시 동작 구현
	m_RCombobox.ResetContent(); // 기존 내용을 모두 제거

	// 고정된 값 추가
	m_RCombobox.AddString(_T("default"));
	m_RCombobox.AddString(_T("R1"));
	m_RCombobox.AddString(_T("R2"));
	m_RCombobox.AddString(_T("R3"));
}

void Cipc2023SubDlg_Router::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	unsigned char Ip1, Ip2, Ip3, Ip4;
	m_RDST_IP.GetAddress(Ip1, Ip2, Ip3, Ip4);
	unsigned char unDst_Ip_Address[] = { Ip1, Ip2, Ip3, Ip4 };
	CString unDstIpAddrStr;
	unDstIpAddrStr.Format(_T("%d.%d.%d.%d"), unDst_Ip_Address[0], unDst_Ip_Address[1], unDst_Ip_Address[2], unDst_Ip_Address[3]);

	// Netmask 가져오기
	m_RNetMask.GetAddress(Ip1, Ip2, Ip3, Ip4);
	unsigned char NetMask_Ip_Address[] = { Ip1, Ip2, Ip3, Ip4 };
	CString NetMaskIpAddrStr;
	NetMaskIpAddrStr.Format(_T("%d.%d.%d.%d"), NetMask_Ip_Address[0], NetMask_Ip_Address[1], NetMask_Ip_Address[2], NetMask_Ip_Address[3]);

	// Gateway 가져오기
	m_RGateWay.GetAddress(Ip1, Ip2, Ip3, Ip4);
	unsigned char GateWay_Ip_Address[] = { Ip1, Ip2, Ip3, Ip4 };
	CString GateWayIpAddrStr;
	GateWayIpAddrStr.Format(_T("%d.%d.%d.%d"), GateWay_Ip_Address[0], GateWay_Ip_Address[1], GateWay_Ip_Address[2], GateWay_Ip_Address[3]);

	if (unDstIpAddrStr.IsEmpty() || NetMaskIpAddrStr.IsEmpty() || GateWayIpAddrStr.IsEmpty()) {
		AfxMessageBox(_T("주소를 설정 오류발생",
			"경고"),
			MB_OK | MB_ICONSTOP); // 수신자, 목적지 주소가 비어있는 경우 설정한 오류 메세지 출력
		return;
	}
	
	unsigned char AndResult[4];
	for (int i = 0; i < 4; i++) {
		AndResult[i] = unDst_Ip_Address[i] & NetMask_Ip_Address[i];
	}
	CString AndResultStr;
	AndResultStr.Format(_T("%d.%d.%d.%d"), AndResult[0], AndResult[1], AndResult[2], AndResult[3]);

	CString FlagStr;
	if (m_CHECK_UP.GetCheck() == BST_CHECKED) {
		FlagStr += _T("U");
	}
	if (m_CHECK_Gate.GetCheck() == BST_CHECKED) {
		FlagStr += _T("G");
	}
	if (m_CHECK_Host.GetCheck() == BST_CHECKED) {
		FlagStr += _T("H");
	}

	CString deviceID;
	deviceID.Format(_T("%d"), m_RCombobox.GetCurSel());
	int ItemIndex = parent->m_ProxyListControl.GetItemCount();
	parent->m_RouterListControl.InsertItem(ItemIndex, "");
	parent->m_RouterListControl.SetItemText(ItemIndex, ROUTER_LIST_CONTROLL_DES_ADDRESS, AndResultStr); // des & mask 값
	parent->m_RouterListControl.SetItemText(ItemIndex, ROUTER_LIST_CONTROLL_NET_MASK, NetMaskIpAddrStr); // netmask 값
	parent->m_RouterListControl.SetItemText(ItemIndex, ROUTER_LIST_CONTROLL_GATE_WAY, GateWayIpAddrStr); // gateway 값
	parent->m_RouterListControl.SetItemText(ItemIndex, ROUTER_LIST_CONTROLL_FLAG, FlagStr);	// flag 값
	parent->m_RouterListControl.SetItemText(ItemIndex, ROUTER_LIST_CONTROLL_DEVICE_ID, deviceID); // device 값
	parent->m_RouterListControl.SetItemText(ItemIndex, ROUTER_LIST_CONTROLL_METRIC, _T("1"));	// metric 값

	parent->m_Ip->AddRouterItem(AndResultStr, NetMaskIpAddrStr, GateWayIpAddrStr, FlagStr);
	
	// parent->m_Ip->AddRouterItem(unDstIpAddrStr, MacAddr);
	CDialogEx::OnOK();
	PostNcDestroy();
}

void Cipc2023SubDlg_Router::setter(Cipc2023Dlg* dlg) {
	parent = dlg;
}
