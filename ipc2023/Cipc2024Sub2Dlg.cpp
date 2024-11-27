// Cipc2023SubDlg.cpp: ���� ����
//

#include "pch.h"
#include "ipc2023.h"
#include "afxdialogex.h"
#include "Cipc2024Sub2Dlg.h"
#include "ipc2023Dlg.h"


#define IP_ROUTING_LIST_CONTROL_DST_IP 1 
#define IP_ROUTING_LIST_CONTROL_NET_MASK 2 
#define IP_ROUTING_LIST_CONTROL_GATEWAY 3
#define IP_ROUTING_LIST_CONTROL_FLAG 4
#define IP_ROUTING_LIST_CONTROL_INTERFACE 5
#define IP_ROUTING_LIST_CONTROL_METIRX 6



// Cipc2023SubDlg ��ȭ ����

IMPLEMENT_DYNAMIC(Cipc2024Sub2Dlg, CDialogEx)

Cipc2024Sub2Dlg::Cipc2024Sub2Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{
}

Cipc2024Sub2Dlg::~Cipc2024Sub2Dlg()
{
}

void Cipc2024Sub2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IP_ROUTER_EDIT, m_unMacAddr);
	DDX_Control(pDX, IDC_IP_ROUTER_NET_MASK, m_NetMask);
	DDX_Control(pDX, IDC_IP_ROUTER_IPADDRESS, m_DstIp);
	DDX_Control(pDX, IDC_IP_ROUTER_GATEWAY, m_GateWay);
	DDX_Control(pDX, IDC_IP_ROUTER_COMBO_INTERFACE, m_Combobox);
	DDX_Control(pDX, IDC_IP_ROUTER_CHECK_UP, m_Check_Up);
	DDX_Control(pDX, IDC_IP_ROUTER_CHECK_HOST, m_Check_Host);
	DDX_Control(pDX, IDC_IP_ROUTER_CHECK_GATEWAY, m_Check_GateWay);
}


BEGIN_MESSAGE_MAP(Cipc2024Sub2Dlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_IP_ROUTER_COMBO_INTERFACE, &Cipc2024Sub2Dlg::OnCbnSelchangeCombo)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDCANCEL, &Cipc2024Sub2Dlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK2, &Cipc2024Sub2Dlg::OnOK)
END_MESSAGE_MAP()


// Cipc2023SubDlg �޽��� ó����



void Cipc2024Sub2Dlg::OnCbnSelchangeCombo()
{
	bpf_u_int32 net, mask;	// ��Ʈ��ũ �ּҿ� ����ũ �ּ� ���� ���� ����
	char errbuf[101];	// ���� �޼��� ���� ���� ����
	PPACKET_OID_DATA OidData;	// OID ������ ���� ������ ����
	LPADAPTER adapter = NULL;	// ��� ������ ����

	// OID ����ü �޸� ����(�߰���)
	OidData = (PPACKET_OID_DATA)malloc(6 + sizeof(PPACKET_OID_DATA));
	OidData->Oid = OID_802_3_CURRENT_ADDRESS;	// OID ����
	OidData->Length = 6;	// OID ũ�� ���� ����


	int i = 0;
	int indexnum = m_Combobox.GetCurSel();	// �޺� �ڽ����� ������ ��Ʈ��ũ ��ġ�� �ε��� ��������
	//���� ����� �κ�///////////////////////////////////////////////////////
	if (pcap_lookupnet(parent->m_NILayer->GetAdapter(indexnum)->name, &net, &mask, errbuf) < 0) {
		return;
		printf("error");
	} // ���� ������ ����� ��Ʈ��ũ �ּҿ� ����ũ �ּ� ��ȸ
	adapter = PacketOpenAdapter(parent->m_NILayer->GetAdapter(indexnum)->name);
	/////////////////// getter�Լ���� ����� ���� ���� ���Ƽ� �׷�////////////
	if (!adapter || adapter->hFile == INVALID_HANDLE_VALUE) {
		return;
	}
	// ��� ���� �����ϸ� �Լ� ����

	PacketRequest(adapter, FALSE, OidData);// MAC �ּ� ��������

	// MAC �ּҸ� ���ڿ��� ����
	CString Address_mssage = "";
	Address_mssage.Format(_T("%02x:%02x:%02x:%02x:%02x:%02x"),
		OidData->Data[0], OidData->Data[1], OidData->Data[2],
		OidData->Data[3], OidData->Data[4], OidData->Data[5]);
	SetDlgItemText(IDC_IP_ROUTER_EDIT, Address_mssage); // ����� MAC�ּҸ� �۽��� �ּҷ� ����
	PacketCloseAdapter(adapter); // ��� �ݱ�
}


void Cipc2024Sub2Dlg::OnClose()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CDialogEx::OnClose();
	DestroyWindow();
}

void Cipc2024Sub2Dlg::setter(Cipc2023Dlg* dlg) {
	parent = dlg;
}

void Cipc2024Sub2Dlg::PostNcDestroy()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	parent->IpRoutingDlg = NULL;
	delete this;
	CDialogEx::PostNcDestroy();
}


BOOL Cipc2024Sub2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetComboboxlist();
	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void Cipc2024Sub2Dlg::SetComboboxlist() {
	int max = parent->m_NILayer->GetMaxAdapterIndex();
	for (int i = 0; i < max; i++) {
		pcap_if_t* d = parent->m_NILayer->GetAdapter(i);
		m_Combobox.AddString(_T(d->description));
	}
}

void Cipc2024Sub2Dlg::OnOK()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	unsigned char Ip1, Ip2, Ip3, Ip4;
	m_DstIp.GetAddress(Ip1, Ip2, Ip3, Ip4);
	unsigned char unDst_Ip_Address[] = { Ip1, Ip2, Ip3, Ip4 };
	m_NetMask.GetAddress(Ip1, Ip2, Ip3, Ip4);
	unsigned char unNetMask[] = { Ip1, Ip2, Ip3, Ip4 };
	m_GateWay.GetAddress(Ip1, Ip2, Ip3, Ip4);
	unsigned char unGetWay[] = { Ip1, Ip2, Ip3, Ip4 };
	CString DstIpAddrStr, NetMaskStr, GateWayStr, FlagStr;
	DstIpAddrStr.Format(_T("%d.%d.%d.%d"), unDst_Ip_Address[0], unDst_Ip_Address[1], unDst_Ip_Address[2], unDst_Ip_Address[3]);
	NetMaskStr.Format(_T("%d.%d.%d.%d"), unNetMask[0], unNetMask[1], unNetMask[2], unNetMask[3]);
	GateWayStr.Format(_T("%d.%d.%d.%d"), unGetWay[0], unGetWay[1], unGetWay[2], unGetWay[3]);
	CString UFlag = _T("U"), HFlag = _T("H"), GFlag = _T("G");
	FlagStr = _T("");
	if (m_Check_Up.GetCheck()) FlagStr += UFlag;
	if (m_Check_GateWay.GetCheck()) FlagStr += GFlag;
	else if (m_Check_Host.GetCheck()) FlagStr += HFlag;
	CString deviceID;
	parent->m_Ip->AddRoutingTable(unDst_Ip_Address, unNetMask, unGetWay, FlagStr, m_Combobox.GetCurSel());
	deviceID.Format(_T("%d"), m_Combobox.GetCurSel());
	int ItemIndex = parent->m_RoutingListControl.GetItemCount();
	parent->m_RoutingListControl.InsertItem(ItemIndex, "");
	parent->m_RoutingListControl.SetItemText(ItemIndex, IP_ROUTING_LIST_CONTROL_INTERFACE, deviceID);
	parent->m_RoutingListControl.SetItemText(ItemIndex, IP_ROUTING_LIST_CONTROL_DST_IP, DstIpAddrStr);
	parent->m_RoutingListControl.SetItemText(ItemIndex, IP_ROUTING_LIST_CONTROL_GATEWAY, GateWayStr);
	parent->m_RoutingListControl.SetItemText(ItemIndex, IP_ROUTING_LIST_CONTROL_NET_MASK, NetMaskStr);
	parent->m_RoutingListControl.SetItemText(ItemIndex, IP_ROUTING_LIST_CONTROL_FLAG, FlagStr);
	parent->m_RoutingListControl.SetItemText(ItemIndex, IP_ROUTING_LIST_CONTROL_METIRX, _T("0"));
	CDialogEx::OnOK();
	PostNcDestroy();
}


void Cipc2024Sub2Dlg::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CDialogEx::OnCancel();
	PostNcDestroy();
}
