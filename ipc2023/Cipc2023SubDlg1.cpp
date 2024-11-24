// Cipc2023SubDlg1.cpp: 구현 파일
//

#include "pch.h"
#include "ipc2023.h"
#include "afxdialogex.h"
#include "Cipc2023SubDlg1.h"
#include "ipc2023Dlg.h"

// Cipc2023SubDlg1 대화 상자

IMPLEMENT_DYNAMIC(Cipc2023SubDlg1, CDialogEx)

Cipc2023SubDlg1::Cipc2023SubDlg1(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

	EnableAutomation();

}

Cipc2023SubDlg1::~Cipc2023SubDlg1()
{
}

void Cipc2023SubDlg1::OnFinalRelease()
{
	// 자동화 개체에 대한 마지막 참조가 해제되면
	// OnFinalRelease가 호출됩니다.  기본 클래스에서 자동으로 개체를 삭제합니다.
	// 기본 클래스를 호출하기 전에 개체에 필요한 추가 정리 작업을
	// 추가하세요.

	CDialogEx::OnFinalRelease();
}

void Cipc2023SubDlg1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IP_ROUTER_IPADDRESS, m_IpAddressCtrl);
	DDX_Control(pDX, IDC_IP_ROUTER_NET_MASK, m_NetMaskCtrl);
	DDX_Control(pDX, IDC_IP_ROUTER_GATEWAY, m_GatewayCtrl);
	DDX_Control(pDX, IDC_IP_ROUTER_COMBO_INTERFACE, m_InterfaceCombo);
	DDX_Check(pDX, IDC_IP_ROUTER_CHECK_UP, m_CheckUp);
	DDX_Check(pDX, IDC_IP_ROUTER_CHECK_GATEWAY, m_CheckGateway);
	DDX_Check(pDX, IDC_IP_ROUTER_CHECK_HOST, m_CheckHost);
}


BEGIN_MESSAGE_MAP(Cipc2023SubDlg1, CDialogEx)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IP_ROUTER_IPADDRESS, &Cipc2023SubDlg1::OnIpnFieldchangedIpRouterIpaddress)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IP_ROUTER_NET_MASK, &Cipc2023SubDlg1::OnIpnFieldchangedIpRouterNetMask)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IP_ROUTER_GATEWAY, &Cipc2023SubDlg1::OnIpnFieldchangedIpRouterGateway)
	ON_BN_CLICKED(IDC_IP_ROUTER_CHECK_UP, &Cipc2023SubDlg1::OnBnClickedIpRouterCheckUp)
	ON_BN_CLICKED(IDC_IP_ROUTER_CHECK_GATEWAY, &Cipc2023SubDlg1::OnBnClickedIpRouterCheckGateway)
	ON_BN_CLICKED(IDC_IP_ROUTER_CHECK_HOST, &Cipc2023SubDlg1::OnBnClickedIpRouterCheckHost)
	ON_CBN_SELCHANGE(IDC_IP_ROUTER_COMBO_INTERFACE, &Cipc2023SubDlg1::OnCbnSelchangeIpRouterComboInterface)
	ON_BN_CLICKED(IDOK2, &Cipc2023SubDlg1::OnBnClickedOk2)
	ON_BN_CLICKED(IDCANCEL, &Cipc2023SubDlg1::OnBnClickedCancel)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(Cipc2023SubDlg1, CDialogEx)
END_DISPATCH_MAP()

// 참고: IID_Iipc2023SubDlg1에 대한 지원을 추가하여
//  VBA에서 형식 안전 바인딩을 지원합니다.
//  이 IID는 .IDL 파일에 있는 dispinterface의 GUID와 일치해야 합니다.

// {a21fdd5a-3612-4240-b7ac-f7fb540033c3}
static const IID IID_Iipc2023SubDlg1 =
{0xa21fdd5a,0x3612,0x4240,{0xb7,0xac,0xf7,0xfb,0x54,0x00,0x33,0xc3}};

BEGIN_INTERFACE_MAP(Cipc2023SubDlg1, CDialogEx)
	INTERFACE_PART(Cipc2023SubDlg1, IID_Iipc2023SubDlg1, Dispatch)
END_INTERFACE_MAP()


// Cipc2023SubDlg1 메시지 처리기


void Cipc2023SubDlg1::OnIpnFieldchangedIpRouterIpaddress(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


void Cipc2023SubDlg1::OnIpnFieldchangedIpRouterNetMask(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


void Cipc2023SubDlg1::OnIpnFieldchangedIpRouterGateway(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


void Cipc2023SubDlg1::OnBnClickedIpRouterCheckUp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void Cipc2023SubDlg1::OnBnClickedIpRouterCheckGateway()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void Cipc2023SubDlg1::OnBnClickedIpRouterCheckHost()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void Cipc2023SubDlg1::OnCbnSelchangeIpRouterComboInterface()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void Cipc2023SubDlg1::OnBnClickedOk2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK(); // 모달 종료
}


void Cipc2023SubDlg1::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}

void Cipc2023SubDlg1::GetDlgData(CString& destination, CString& netmask, CString& gateway, CString& netInterface, CString& flag)
{
	GetDlgItemText(IDC_IP_ROUTER_IPADDRESS, destination);
	GetDlgItemText(IDC_IP_ROUTER_NET_MASK, netmask);
	GetDlgItemText(IDC_IP_ROUTER_GATEWAY, gateway);

	// 콤보박스 선택 항목 가져오기
	int comboIndex = m_InterfaceCombo.GetCurSel();
	if (comboIndex != CB_ERR)
	{
		m_InterfaceCombo.GetLBText(comboIndex, netInterface);
	}

	// 체크박스 상태 확인
	flag.Empty();
	if (m_CheckUp) flag += _T("UP ");
	if (m_CheckGateway) flag += _T("Gateway ");
	if (m_CheckHost) flag += _T("Host");
}