// FileLayer.cpp: implementation of the CFileLayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "pch.h"
#include "CNILayer.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNILayer::CNILayer(char* pName)
	: CBaseLayer(pName)
{
	for (int i = 0; i < 2; i++) {
		m_adapterIndex[i] = new _THREADSTRUCT;
		m_adapterIndex[i]->Layer = this;
	}
}

CNILayer::~CNILayer()
{
}

BOOL CNILayer::Send(unsigned char* ppayload, int nlength, int interface_ID)
{
	char errbuf[1001];
	if ((tmp_fp = pcap_open_live(m_AdapterList[m_adapterIndex[interface_ID]->adapter_num]->name, 65536, 0, 1000, errbuf)) == NULL) {
		AfxMessageBox(_T("Unable to open the adapter"));
		return false;
	}
	if (!send_packet(tmp_fp, ppayload, nlength)) {
		AfxMessageBox(_T("ERROR SENDING THE PACKET"));
		return false;
	}

	return true;
}
// 가장 아래 계층인 File 계층의 Send 함수 입니다. IpcBuff.txt라는 파일을 만들어서 적습니다

BOOL CNILayer::Receive(int interface_ID)
{
	m_pThread = AfxBeginThread(ReceiveThread, (LPVOID)m_adapterIndex[interface_ID]);
	return true;
}
// 가장 아래 계층인 File 계층의 Receive 함수 입니다. IpcBuff.txt라는 파일을 읽습니다.
// 그 후, 크기에 맞게 이를 확인하고, 위의 헤더인 Ethernet 계층에 전해줍니다.

BOOL  CNILayer::send_packet(pcap_t* handle, u_char* message, int len) {
	if (pcap_sendpacket(handle, message, len) != 0) {
		AfxMessageBox(_T("Not Send"));
		return false;
	}
	else return true;
}

UINT CNILayer::ReceiveThread(LPVOID pParam) {
	ThreadStruct* Param = (ThreadStruct*)pParam;
	CNILayer* PID = Param->Layer;
	int adapternum = Param->adapter_num;
	int curIndex;
	if (PID->m_adapterIndex[0]->adapter_num == adapternum) curIndex = 0;
	else curIndex = 1;
	pcap_t* fp;
	char errbuf[1001];
	if ((fp = pcap_open_live(PID->m_AdapterList[adapternum]->name, 65536, 0, 1000, errbuf)) == NULL) {
		AfxMessageBox(_T(errbuf));
		PID->is_set = false;
		return false;
	}
	struct pcap_pkthdr* header;
	const u_char* pkt_data;
	int result;
	while (PID->is_set) {
		result = pcap_next_ex(fp, &header, &pkt_data);
		if (result == 0) {

		}
		else if (result == 1) {
			if (PID->mp_aUpperLayer[0]->Receive((u_char*)pkt_data, curIndex)) {
			}
		}
	}
	return 0;
}

void CNILayer::SetAdpterDeivce() {
	pcap_if_t* alldevs, * d;
	char errbuff[1001];
	if (pcap_findalldevs(&alldevs, errbuff) == -1 || !alldevs) {
		AfxMessageBox(_T("Any NIC not detected"));
		exit(1);
	}
	for (d = alldevs; d; d = d->next) {
		m_AdapterList[m_Maxadapter] = d;
		m_Maxadapter++;
	}
}

pcap_if_t* CNILayer::GetAdapter(int index) {
	return m_AdapterList[index];
}

int CNILayer::GetMaxAdapterIndex() {
	return m_Maxadapter;
}

void CNILayer::Set_is_set(bool value) {
	is_set = value;
}
bool CNILayer::Get_is_set() {
	return is_set;
}

int CNILayer::GetAdapterIndex(int index) {
	return m_adapterIndex[index]->adapter_num;
}
void CNILayer::SetAdapterIndex(int value, int index) {
	m_adapterIndex[index]->adapter_num = value;
}