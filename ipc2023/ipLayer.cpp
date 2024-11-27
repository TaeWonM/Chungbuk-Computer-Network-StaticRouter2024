// ipLayer.cpp: implementation of the CFileLayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "pch.h"
#include "ipLayer.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ipLayer::ipLayer(char* pName)
	: CBaseLayer(pName), nullString("NULL")
{
	ResetHeader();
}

ipLayer::~ipLayer()
{
}

void ipLayer::ResetHeader() {
	//m_IpAddrmap.clear();
}
BOOL ipLayer::Send(unsigned char* ppayload, int nlength, int interface_ID)
{	
	CString unIpAddr;
	unIpAddr.Format("%d.%d.%d.%d", ppayload[0], ppayload[1], ppayload[2], ppayload[3]);
	if (!m_IpMap.empty() && m_IpMap.find(unIpAddr) != m_IpMap.end()) return TRUE;
	else {
		m_IpMap.insert({unIpAddr, nullString});
		return mp_UnderLayer[0]->Send(ppayload, 4, interface_ID);
	}
}

// ARP�� ���� Receive �Դϴ�.
BOOL ipLayer::Receive(unsigned char* ppayload, BOOL is_in, int interface_ID)
{
	unsigned char DstIpAddr[4];
	unsigned char DstMacAddr[6];
	memcpy(DstIpAddr, ppayload, 4);
	memcpy(DstMacAddr, &ppayload[4], 6);
	CString DstIpAddrStr, DstMacAddrStr;
	DstIpAddrStr.Format(_T("%d.%d.%d.%d"), DstIpAddr[0], DstIpAddr[1], DstIpAddr[2], DstIpAddr[3]);
	DstMacAddrStr.Format(_T("%02x:%02x:%02x:%02x:%02x:%02x"), DstMacAddr[0], DstMacAddr[1], DstMacAddr[2], DstMacAddr[3], DstMacAddr[4], DstMacAddr[5]);
	if (!is_in) {
		if (!m_ProxyMap.empty() && m_ProxyMap.find(DstIpAddrStr) != m_ProxyMap.end()) {
			//TODO : Proxymessage
			return TRUE;
		}
		else return FALSE;
	}
	else {
		if (!m_IpMap.empty() && m_IpMap.find(DstIpAddrStr) != m_IpMap.end()) {
			m_IpMap[DstIpAddrStr] = DstMacAddrStr;
			return mp_aUpperLayer[0]->Receive(DstIpAddrStr, DstMacAddrStr, TRUE, interface_ID);
		}
		m_IpMap.insert({ DstIpAddrStr , DstMacAddrStr });
		return mp_aUpperLayer[0]->Receive(DstIpAddrStr, DstMacAddrStr, FALSE, interface_ID);
	}
}

BOOL ipLayer::Receive(unsigned char* ppayload, int interface_ID)
{
	return FALSE;
}

void ipLayer::RemoveItem(CString IpAddr, CString MacAddr) {
	auto iter = m_IpMap.find(IpAddr);
	if (iter != m_IpMap.end())
	{
		m_IpMap.erase(m_IpMap.find(IpAddr));
	}
}

BOOL ipLayer::UpdateArpCahe(unsigned char* ipAddr, unsigned char* macAddr) {
	CString DstIpAddrStr, DstMacAddrStr;
	DstIpAddrStr.Format(_T("%d.%d.%d.%d"), ipAddr[0], ipAddr[1], ipAddr[2], ipAddr[3]);
	DstMacAddrStr.Format(_T("%02x:%02x:%02x:%02x:%02x:%02x"), macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
	if (!m_IpMap.empty() && m_IpMap.find(DstIpAddrStr) != m_IpMap.end()) {
		m_IpMap[DstIpAddrStr] = DstMacAddrStr;
		return mp_aUpperLayer[0]->UpdateArpCahe(ipAddr, macAddr);
	}
	return FALSE;
}

void ipLayer::AddProxyItem(CString IpAddr, CString MacAddr) {
	m_ProxyMap.insert({ IpAddr , MacAddr });
}

void ipLayer::DeleteProxyItem(CString IpAddr) {
	m_ProxyMap.erase(IpAddr);
}

void ipLayer::DeleteItem(CString IpAddr) {
	m_IpMap.erase(IpAddr);
}

void ipLayer::DeleteAllItem() {
	m_IpMap.clear();
}