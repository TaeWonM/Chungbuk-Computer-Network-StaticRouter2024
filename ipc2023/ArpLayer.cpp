// ChatAppLayer.cpp: implementation of the CChatAppLayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "pch.h"
#include "ArpLayer.h"

#define ARP_LAYER 1

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ArpLayer::ArpLayer(char* pName)
	: CBaseLayer(pName),
	mp_Dlg(NULL)
{
	ResetHeader();
}
// ���� ������ ������ CChatAppLayer�� ���� Ŭ�����Դϴ�.

ArpLayer::~ArpLayer()
{

}

void ArpLayer::ResetHeader()
{
	for (int i = 0; i < ADAPTER_MAX_SIZE; i++) {
		m_sHeader[i].hard_type = htons(0x0001);
		m_sHeader[i].portocal_type = htons(0x0800);
		m_sHeader[i].hard_size = 6;
		m_sHeader[i].portocal_size = 4;
		m_sHeader[i].op_Code = htons(0x0001);
		memset(m_sHeader[i].sender_ethernet_address, 0, ETHER_ADDRESS_SIZE);
		memset(m_sHeader[i].sender_IP_address, 0, IP_ADDRESS_SIZE);
		memset(m_sHeader[i].target_ethernet_address, 0, ETHER_ADDRESS_SIZE);
		memset(m_sHeader[i].target_IP_address, 0, IP_ADDRESS_SIZE);
		m_replyHeader[i].hard_type = htons(0x0001);
		m_replyHeader[i].portocal_type = htons(0x0800);
		m_replyHeader[i].hard_size = 6;
		m_replyHeader[i].portocal_size = 4;
		m_replyHeader[i].op_Code = htons(0x0002);
		memset(m_replyHeader[i].sender_ethernet_address, 0, ETHER_ADDRESS_SIZE);
		memset(m_replyHeader[i].sender_IP_address, 0, IP_ADDRESS_SIZE);
		memset(m_replyHeader[i].target_ethernet_address, 0, ETHER_ADDRESS_SIZE);
		memset(m_replyHeader[i].target_IP_address, 0, IP_ADDRESS_SIZE);
		memset(BroadingCastAddr, 255, ETHER_ADDRESS_SIZE);
	}
}
//m_sHeader�� ChatApp ��� �κ� �κ��� �ʱ�ȭ�ϱ� ����, Reset�Լ��Դϴ�.


BOOL ArpLayer::Send(unsigned char* DstIpAddress, int nlength, int interface_ID)
{
	memcpy(m_sHeader[interface_ID].sender_ethernet_address, m_macAddr[interface_ID], ETHER_ADDRESS_SIZE);
	memcpy(m_sHeader[interface_ID].sender_IP_address, m_ipAddr[interface_ID], IP_ADDRESS_SIZE);
	memcpy(m_sHeader[interface_ID].target_IP_address, DstIpAddress, nlength);
	memset(m_sHeader[interface_ID].target_ethernet_address, 255, ETHER_ADDRESS_SIZE);
	mp_UnderLayer[0]->SetMacDstAddress(m_sHeader[interface_ID].target_ethernet_address, interface_ID);
	mp_UnderLayer[0]->SetMacSrcAddress(m_sHeader[interface_ID].sender_ethernet_address, interface_ID);
	return mp_UnderLayer[0]->Send((unsigned char*)&m_sHeader[interface_ID], ARP_HEADER_SIZE, ARP_LAYER, interface_ID);
}
// ������ ������ Send �Լ��Դϴ�. ����� ���̸� �����ϰ�, �̸� ppayload�� �ִ� data�� �����Ͽ� �� ����(��, CEthernetLayer)�� Send�� �Լ��� �����մϴ�.


BOOL ArpLayer::SendArp(unsigned char* ppayload, unsigned char *DstMacAddr,int nlength, int interface_ID) {

	Send(DstMacAddr, 4, interface_ID);
	return TRUE;
}

BOOL ArpLayer::Receive(unsigned char* ppayload, int interface_ID) {
	BOOL is_in = FALSE;
	P_ARP_HEADER arp = (P_ARP_HEADER)ppayload;
	// GARP 패킷인지 확인: 송신자의 IP 주소 = IP 주소, 대상 MAC 주소 = 브로드캐스트 주소인지 확인
	if (memcmp(arp->target_IP_address, arp->sender_IP_address, 4) == 0) {
		if (memcmp(arp->target_IP_address, m_ipAddr[interface_ID], 4) == 0) {
			if (ntohs(arp->op_Code) == 1) {
				memcpy(m_replyHeader[interface_ID].target_IP_address, m_ipAddr[interface_ID], IP_ADDRESS_SIZE);// 대상 ip주소 = 송신자의 ip주소
				memcpy(m_replyHeader[interface_ID].target_ethernet_address, BroadingCastAddr, ETHER_ADDRESS_SIZE);// 대상 이더넷 주소 = 송신자의 이더넷 주소
				memcpy(m_replyHeader[interface_ID].sender_IP_address, m_ipAddr[interface_ID], IP_ADDRESS_SIZE);// 송신 ip 주소 = 자신의 ip주소
				memcpy(m_replyHeader[interface_ID].sender_ethernet_address, m_macAddr[interface_ID], ETHER_ADDRESS_SIZE); // 송신 이더넷 주소 = 자신의 이더넷 주소
				mp_UnderLayer[0]->SetMacDstAddress(m_replyHeader[interface_ID].target_ethernet_address, interface_ID);
				mp_UnderLayer[0]->Send((unsigned char*)&m_replyHeader[interface_ID], ARP_HEADER_SIZE, ARP_LAYER, interface_ID);
			}
			else {
				AfxMessageBox(_T("Duplicated GARP!!"));
			}
			return TRUE;
		}
		// ARP 캐시 업데이트
		mp_aUpperLayer[0]->UpdateArpCahe(arp->sender_IP_address, arp->sender_ethernet_address);

		return TRUE; // GARP 패킷 처리 완료
	}

	if (memcmp(m_ipAddr[interface_ID], arp->target_IP_address, 4) == 0) {
		if (ntohs(arp->op_Code) == 1) {
			memcpy(m_replyHeader[interface_ID].target_IP_address, arp->sender_IP_address, IP_ADDRESS_SIZE);// 대상 ip주소 = 송신자의 ip주소
			memcpy(m_replyHeader[interface_ID].target_ethernet_address, arp->sender_ethernet_address, ETHER_ADDRESS_SIZE);// 대상 이더넷 주소 = 송신자의 이더넷 주소
			memcpy(m_replyHeader[interface_ID].sender_IP_address, m_ipAddr[interface_ID], IP_ADDRESS_SIZE);// 송신 ip 주소 = 자신의 ip주소
			memcpy(m_replyHeader[interface_ID].sender_ethernet_address, m_macAddr[interface_ID], ETHER_ADDRESS_SIZE); // 송신 이더넷 주소 = 자신의 이더넷 주소
			mp_UnderLayer[0]->SetMacDstAddress(m_replyHeader[interface_ID].target_ethernet_address, interface_ID);
			mp_UnderLayer[0]->Send((unsigned char*)&m_replyHeader[interface_ID], ARP_HEADER_SIZE, ARP_LAYER, interface_ID);
		}
		is_in = TRUE;
	}
	unsigned char* payload;
	if (is_in) {
		payload = (unsigned char*)malloc(sizeof(unsigned char) * (IP_ADDRESS_SIZE + ETHER_ADDRESS_SIZE));
		memcpy(payload, arp->sender_IP_address, IP_ADDRESS_SIZE);
		memcpy(&payload[IP_ADDRESS_SIZE], arp->sender_ethernet_address, ETHER_ADDRESS_SIZE);
	}
	else {
		payload = (unsigned char*)malloc(sizeof(unsigned char) * (IP_ADDRESS_SIZE + ETHER_ADDRESS_SIZE));
		memcpy(payload, arp->target_IP_address, IP_ADDRESS_SIZE);
		memcpy(&payload[IP_ADDRESS_SIZE], arp->target_ethernet_address, ETHER_ADDRESS_SIZE);
	}
	if (mp_aUpperLayer[0]->Receive(payload, is_in, interface_ID) && !is_in) {
		memcpy(m_replyHeader[interface_ID].target_IP_address, arp->sender_IP_address, IP_ADDRESS_SIZE);// 대상 ip주소 = 송신자의 ip주소
		memcpy(m_replyHeader[interface_ID].target_ethernet_address, arp->sender_ethernet_address, ETHER_ADDRESS_SIZE);// 대상 이더넷 주소 = 송신자의 이더넷 주소
		memcpy(m_replyHeader[interface_ID].sender_IP_address, arp->target_IP_address, IP_ADDRESS_SIZE);// 송신 ip 주소 = 자신의 ip주소
		memcpy(m_replyHeader[interface_ID].sender_ethernet_address, m_macAddr[interface_ID], ETHER_ADDRESS_SIZE); // 송신 이더넷 주소 = 자신의 이더넷 주소
		mp_UnderLayer[0]->SetMacDstAddress(m_replyHeader[interface_ID].target_ethernet_address, interface_ID);
		mp_UnderLayer[0]->Send((unsigned char*)&m_replyHeader[interface_ID], ARP_HEADER_SIZE, ARP_LAYER, interface_ID);
	}
	free(payload);
	return FALSE;
}

void ArpLayer::Set_Sender_Address(unsigned char* MACAddr, unsigned char *IpAddress, int interface_ID) {
	Set_Ip_Address(IpAddress, interface_ID);
	Set_Mac_Address(MACAddr, interface_ID);
}

BOOL ArpLayer::SendGARP(const unsigned char* macAddr, int interface_ID) {
    // Broadcast 주소로 대상 MAC 주소 설정
    memset(m_sHeader[interface_ID].target_ethernet_address, 255, ETHER_ADDRESS_SIZE); // Broadcast MAC 주소 설정
	
	mp_UnderLayer[0]->SetMacDstAddress(m_sHeader[interface_ID].target_ethernet_address, interface_ID);
    // 자신의 IP를 대상 IP로 설정하여 GARP 생성
    memcpy(m_sHeader[interface_ID].target_IP_address, m_ipAddr[interface_ID], IP_ADDRESS_SIZE); // 자신의 IP로 설정
	memcpy(m_sHeader[interface_ID].sender_IP_address, m_ipAddr[interface_ID], IP_ADDRESS_SIZE); // 자신의 IP로 설정

    // 전달받은 MAC 주소를 ARP 헤더의 송신 MAC 주소로 설정
    memcpy(m_sHeader[interface_ID].sender_ethernet_address, macAddr, ETHER_ADDRESS_SIZE); // ARP 헤더의 송신 MAC 주소 설정
    // EthernetLayer를 통해 GARP 패킷 전송
    return mp_UnderLayer[0]->Send((unsigned char*)&m_sHeader[interface_ID], ARP_HEADER_SIZE, ARP_LAYER, interface_ID);
}


void ArpLayer::Set_Mac_Address(unsigned char* MACAddr, int interface_ID) {
	memcpy(m_macAddr[interface_ID], MACAddr, 6);
}
void ArpLayer::Set_Ip_Address(unsigned char* IpAddr, int interface_ID) {
	memcpy(m_ipAddr[interface_ID], IpAddr, 4);
}