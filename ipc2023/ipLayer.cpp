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
	initRoutingTable();
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

// ARP에 대한 Receive 입니다.
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
		if (!m_Ip_Queue.empty()) {
			IP_QUEUE temp;
			temp = m_Ip_Queue.front();
			if (memcmp(temp.DstAddr, DstIpAddr, 4) == 0) {
				m_Ip_Queue.pop();
				P_IP_HEADER payload = (P_IP_HEADER)temp.payload;
				mp_UnderLayer[1]->SetMacDstAddress(DstIpAddr,temp.Interface_ID);
				mp_UnderLayer[1]->Send(ppayload, payload->TotalLengh, temp.Interface_ID);
			}
		}
		m_IpMap.insert({ DstIpAddrStr , DstMacAddrStr });
		return mp_aUpperLayer[0]->Receive(DstIpAddrStr, DstMacAddrStr, FALSE, interface_ID);
	}
}

// IP에 대한 Receive 입니다.
BOOL ipLayer::Receive(unsigned char* ppayload, int interface_ID)
{
	P_IP_HEADER payload = (P_IP_HEADER)ppayload;
	CString DstIpAddrStr;
	int index = 0;
	for (index = 0; index < m_Routing_Table_Max_Index; index++) {
		int i;
		for (i = 0; i < IP_ADDRESS_SIZE; i++) {
			if ((m_IP_Routing_Table[index].m_Netmask[i] & payload->target_IP_address[i]) != m_IP_Routing_Table[index].m_Destination[i]) break;
		}
		if (i == IP_ADDRESS_SIZE) {
			break;
		}
	}
	if (index == m_Routing_Table_Max_Index) {
		AfxMessageBox(_T("Nothing Mached"));
		return FALSE;
	}
	// 이때부터 라우팅 테이블에 존재한다고 가정..
	unsigned char* DstIpAddr;
	//Flag에 따라 움직이는 법이 달라짐..
	if (m_IP_Routing_Table[index].m_Flag == "U") {
		//일단 Map의 String 형태로... 변환.
		DstIpAddrStr.Format(_T("%d.%d.%d.%d"), payload->target_IP_address[0], payload->target_IP_address[1],
			payload->target_IP_address[2], payload->target_IP_address[3]);
		//일단 존재하는지 확인
		if (!m_IpMap.empty() && m_IpMap.find(DstIpAddrStr) != m_IpMap.end()) {
			//존재하는데, nullString 값이 들어있으면 어짜피 없다는 뜻이니, 검사
			if (m_IpMap[DstIpAddrStr] != nullString) {
				//존재하니까, 이 값들을 IP에서 바로 Ethernet으로.. 
				DstIpAddr = MacAddr2HexInt(m_IpMap[DstIpAddrStr]);
				mp_UnderLayer[1]->SetMacDstAddress(DstIpAddr, m_IP_Routing_Table[index].m_interfaceID);
				mp_UnderLayer[1]->Send(ppayload, payload->TotalLengh, m_IP_Routing_Table[index].m_interfaceID);
				free(DstIpAddr);
				return TRUE;
			}
			else {
				//존재하는데, nullString 값이 들어있으니 큐에 넣고, Arp 날리기..
				DstIpAddr = IpAddr2HexInt(DstIpAddrStr);
				AddQueue(DstIpAddr, ppayload, m_IP_Routing_Table[index].m_interfaceID);
				mp_UnderLayer[0]->Send(DstIpAddr, 4, m_IP_Routing_Table[index].m_interfaceID);
				return TRUE;
			}
		}
		else {
			//존재하지 않으니까, 큐에 넣고, Arp 날리기..
			DstIpAddr = IpAddr2HexInt(DstIpAddrStr);
			AddQueue(DstIpAddr, ppayload, m_IP_Routing_Table[index].m_interfaceID);
			mp_UnderLayer[0]->Send(DstIpAddr, 4, m_IP_Routing_Table[index].m_interfaceID);
			return TRUE;
		}
	}
	//Flag에 따라 움직이는 법이 달라짐..
	if (m_IP_Routing_Table[index].m_Flag == "UG") {
		//일단 검사..
		DstIpAddrStr.Format(_T("%d.%d.%d.%d"), m_IP_Routing_Table[index].m_Gateway[0], m_IP_Routing_Table[index].m_Gateway[1],
			m_IP_Routing_Table[index].m_Gateway[2], m_IP_Routing_Table[index].m_Gateway[3]);
		if (!m_IpMap.empty() && m_IpMap.find(DstIpAddrStr) != m_IpMap.end()) {
			if (m_IpMap[DstIpAddrStr] != nullString) {
				//중요하게 달라진 부분은 밑 부분.. Gateway의 Ip Address를 넣고 있다.
				DstIpAddr = MacAddr2HexInt(m_IpMap[DstIpAddrStr]);
				mp_UnderLayer[1]->SetMacDstAddress(DstIpAddr, m_IP_Routing_Table[index].m_interfaceID);
				mp_UnderLayer[1]->Send(ppayload, payload->TotalLengh, m_IP_Routing_Table[index].m_interfaceID);
				free(DstIpAddr);
				return TRUE;
			}
			else {
				//중요하게 달라진 부분은 밑 부분.. Gateway의 Ip Address를 넣고 있다.
				DstIpAddr = IpAddr2HexInt(DstIpAddrStr);
				AddQueue(DstIpAddr, ppayload, m_IP_Routing_Table[index].m_interfaceID);
				mp_UnderLayer[0]->Send(DstIpAddr, 4, m_IP_Routing_Table[index].m_interfaceID);
				return TRUE;
			}
		}
		else {
			//중요하게 달라진 부분은 밑 부분.. Gateway의 Ip Address를 넣고 있다.
			DstIpAddr = IpAddr2HexInt(DstIpAddrStr);
			AddQueue(DstIpAddr, ppayload, m_IP_Routing_Table[index].m_interfaceID);
			mp_UnderLayer[0]->Send(DstIpAddr, 4, m_IP_Routing_Table[index].m_interfaceID);
			return TRUE;
		}
	}
	if (m_IP_Routing_Table[index].m_Flag == "UH") {
		return TRUE;
	}
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

void ipLayer::initRoutingTable() {
	for (int i = 0; i < IP_ROUTING_TABLE_MAX_SIZE; i++) {
		memset(m_IP_Routing_Table[i].m_Destination, 0, IP_ADDRESS_SIZE);
		memset(m_IP_Routing_Table[i].m_Netmask, 0, IP_ADDRESS_SIZE);
		memset(m_IP_Routing_Table[i].m_Gateway, 0, IP_ADDRESS_SIZE);
		m_IP_Routing_Table[i].m_Flag = "";
		m_IP_Routing_Table[i].m_interfaceID = 0;
	}
	m_Routing_Table_Max_Index = 0;
}

void ipLayer::AddRoutingTable(unsigned char* Destination, unsigned char* Netmask, unsigned char* Gateway, CString Flag, int interface_ID) {
	memcpy(m_IP_Routing_Table[m_Routing_Table_Max_Index].m_Destination, Destination, IP_ADDRESS_SIZE);
	memcpy(m_IP_Routing_Table[m_Routing_Table_Max_Index].m_Netmask, Netmask, IP_ADDRESS_SIZE);
	memcpy(m_IP_Routing_Table[m_Routing_Table_Max_Index].m_Gateway, Gateway, IP_ADDRESS_SIZE);
	m_IP_Routing_Table[m_Routing_Table_Max_Index].m_Flag = Flag;
	m_IP_Routing_Table[m_Routing_Table_Max_Index].m_interfaceID = interface_ID;
	m_Routing_Table_Max_Index++;
}

unsigned char* ipLayer::MacAddr2HexInt(CString Mac_address)
{
	CString TempToken; // MAC 주소의 각 부분을 저장할 변수 설정
	unsigned char* ether = (unsigned char*)malloc(sizeof(unsigned char) * 6);

	// MAC 주소를 ':' 로 구분하여 16진수 변환
	for (int i = 0; i < 6; i++) {
		if (AfxExtractSubString(TempToken, Mac_address, i, ':')) {
			ether[i] = (unsigned char)strtoul(TempToken.GetString(), NULL, 16);
		}
		else {
			AfxMessageBox(_T("주소를 설정 오류발생",
				"경고"),
				MB_OK | MB_ICONSTOP);
			free(ether);
			return nullptr;
			// 주소 설정 시 오류가 발생하면 오류 메세지 출력하고 할당된 메모리 해제 및 NULL 반환
		}
	}
	ether[6] = '\0';  // 종료 문자 추가 '\0'

	return ether;	// 변환된 MAC 주소 반환
}


void ipLayer::AddQueue(unsigned char* DstIPAddr, unsigned char* ppayload, int interface_ID) {
	IP_QUEUE temp;
	P_IP_HEADER payload = (P_IP_HEADER)ppayload;
	memcpy(temp.DstAddr, DstIPAddr, IP_ADDRESS_SIZE);
	unsigned char* temp_payload;
	temp_payload = new unsigned char[payload->TotalLengh];
	memcpy(temp_payload, ppayload, payload->TotalLengh);
	temp.payload = temp_payload;
	temp.Interface_ID = interface_ID;
	m_Ip_Queue.push(temp);
}

unsigned char* ipLayer::IpAddr2HexInt(CString Ip_address)
{
	CString TempToken; // MAC 주소의 각 부분을 저장할 변수 설정
	unsigned char* ether = (unsigned char*)malloc(sizeof(unsigned char) * 4);

	// MAC 주소를 ':' 로 구분하여 16진수 변환
	for (int i = 0; i < 4; i++) {
		if (AfxExtractSubString(TempToken, Ip_address, i, '.')) {
			ether[i] = (unsigned char)strtoul(TempToken.GetString(), NULL, 10);
		}
		else {
			AfxMessageBox(_T("주소를 설정 오류발생",
				"경고"),
				MB_OK | MB_ICONSTOP);
			free(ether);
			return nullptr;
			// 주소 설정 시 오류가 발생하면 오류 메세지 출력하고 할당된 메모리 해제 및 NULL 반환
		}
	}
	ether[4] = '\0';  // 종료 문자 추가 '\0'

	return ether;	// 변환된 MAC 주소 반환
}