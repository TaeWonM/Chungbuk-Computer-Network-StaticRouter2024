// EthernetLayer.cpp: implementation of the CEthernetLayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "pch.h"
#include "EthernetLayer.h"

#define ARP_LAYER 1 // �߰���
#define IP_LAYER 2 // �߰���


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEthernetLayer::CEthernetLayer(char* pName)
	: CBaseLayer(pName)
{
	ResetHeader();
}
// ������ CEthernetLayer�� ���� Ŭ�����Դϴ�.

CEthernetLayer::~CEthernetLayer()
{
}

void CEthernetLayer::ResetHeader()
{
	for (int i = 0; i < 2; i++) {
		memset(m_sHeader[i].enet_dstaddr, 0, 6);
		memset(m_sHeader[i].enet_srcaddr, 0, 6);
		memset(m_sHeader[i].enet_data, 0, ETHER_MAX_DATA_SIZE);
	}
}
//m_sHeader�� Ethernet ��� �κ��� �ʱ�ȭ�ϱ� ����, Reset�Լ��Դϴ�.

unsigned char* CEthernetLayer::GetSourceAddress(int index)
{
	return m_sHeader[index].enet_srcaddr;
}
//enet_srcaddr�� �����ϴ� getter�Դϴ�.

unsigned char* CEthernetLayer::GetDstAddress(int index)
{
	//////////////////////// fill the blank ///////////////////////////////
	// Ethernet ������ �ּ� return
	return m_sHeader[index].enet_dstaddr;
	///////////////////////////////////////////////////////////////////////
}

void CEthernetLayer::SetSourceAddress(unsigned char* pAddress, unsigned char* pAddress2)
{
	//////////////////////// fill the blank ///////////////////////////////
		// �Ѱܹ��� source �ּҸ� Ethernet source�ּҷ� ����
	memcpy(m_sHeader[0].enet_srcaddr, pAddress, 6);
	memcpy(m_sHeader[1].enet_srcaddr, pAddress2, 6);
	///////////////////////////////////////////////////////////////////////
}
//enet_srcaddr�� �����ϴ� setter�Դϴ�.

void CEthernetLayer::SetDestinAddress(unsigned char* pAddress, int index)
{
	memcpy(m_sHeader[index].enet_dstaddr, pAddress, 6);
}
//enet_dstaddr�� �����ϴ� setter�Դϴ�.
//--------------------------------------------------- ���� 2024.10.13.-------------------------------------

BOOL CEthernetLayer::Send(unsigned char* ppayload, int nlength, int DetLayer, int interface_ID)
{
	memcpy(m_sHeader[interface_ID].enet_data, ppayload, nlength);
	BOOL bSuccess = FALSE;
	// ACK ó��: ä�� ������ ������ ���
	if (DetLayer == ARP_LAYER) {
		m_sHeader[interface_ID].enet_type = htons(0x0806);
		bSuccess = mp_UnderLayer[0]->Send((unsigned char*)&m_sHeader[interface_ID], nlength + ETHER_HEADER_SIZE, interface_ID);
	}
	else if (DetLayer == IP_LAYER) {
		m_sHeader[interface_ID].enet_type = htons(0x0800);
		bSuccess = mp_UnderLayer[0]->Send((unsigned char*)&m_sHeader[interface_ID], nlength + ETHER_HEADER_SIZE, interface_ID);
	}
	else {
		return FALSE;
	}

	return bSuccess;
}
// Ethernet ������ Send �Լ��Դϴ�. ����� ���̸� �����ϰ�, �̸� ppayload�� �ִ� data�� �����Ͽ� �� ����(��, CFileLayer)�� Send�� �Լ��� �����մϴ�.
//---------------------------------------------------- ������� ---------------------------------------

//--------------------------------------------------- ���� 2024.10.13.-------------------------------------

BOOL CEthernetLayer::Receive(unsigned char* ppayload, int Interface_ID)
{
	PETHERNET_HEADER pFrame = (PETHERNET_HEADER)ppayload;

	BOOL bSuccess = FALSE;
	//////////////////////// fill the blank ///////////////////////////////
		// Chatapp or Fileapp layer�� Ethernet Frame�� data�� �Ѱ��ش�
		// ������ ������ Ÿ�Ե鿡 ���ؼ��� ����
	if ((memcmp(pFrame->enet_dstaddr, m_sHeader[Interface_ID].enet_srcaddr, 6) == 0 ||
		(memcmp(pFrame->enet_dstaddr, BROADCASTING_ADDR, 6) == 0 && memcmp(pFrame->enet_srcaddr, m_sHeader[Interface_ID].enet_srcaddr, 6) != 0)) &&
		(ntohs(pFrame->enet_type) == 0x0806 || ntohs(pFrame->enet_type) == 0x2081))
	{
		if (memcmp(pFrame->enet_dstaddr, BROADCASTING_ADDR, 6) == 0) is_Broadcast = true;

		if (ntohs(pFrame->enet_type) == 0x0806) { // ä�� Ÿ���̸� Arplayer�� �ø�
			bSuccess = mp_aUpperLayer[0]->Receive((unsigned char*)pFrame->enet_data, Interface_ID);
		}
		else if (ntohs(pFrame->enet_type) == 0x0800) { // ���� Ÿ���̸� IPlayer�� �ø�
			bSuccess = mp_aUpperLayer[1]->Receive((unsigned char*)pFrame->enet_data, Interface_ID);
		}
	}
	///////////////////////////////////////////////////////////////////////

	return bSuccess;
}
// Ethernet ������ Receive �Լ��Դϴ�. 
// �� �������� ���޹��� ppayload�� PETHERNET_HEADER�� �־ ����� ����ϴ�.
// �� ��, ���� Ȯ���� ���� �ʰ�, �ٷ� ���� ������ ChatApp ������ �����մϴ�.
//---------------------------------------------------- ������� ---------------------------------------

//--------------------------------------------------- ���� 2024.10.13.-------------------------------------
//---------------------------------------------------- ������� ---------------------------------------

void CEthernetLayer::SetBroadcasting_address() {
	memset(BROADCASTING_ADDR, 255, 6);
}

BOOL CEthernetLayer::SetMacDstAddress(unsigned char* ppayload, int interface_ID) {
	memcpy(m_sHeader[interface_ID].enet_dstaddr, ppayload, ETHER_ADDRESS_SIZE);
	return TRUE;
}
BOOL CEthernetLayer::SetMacSrcAddress(unsigned char* ppayload, int interface_ID) {
	memcpy(m_sHeader[interface_ID].enet_srcaddr, ppayload, ETHER_ADDRESS_SIZE);
	return TRUE;
}