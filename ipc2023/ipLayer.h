#pragma once
// ipLayer.h: interface for the CChatAppLayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPLAYER_H__E78615DE_0F23_41A9_B814_34E2B3697EF2__INCLUDED_)
#define AFX_IPLAYER_H__E78615DE_0F23_41A9_B814_34E2B3697EF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseLayer.h"
#include "pch.h"
#include <map>
#include <queue>
class ipLayer
	: public CBaseLayer
{
private:
	inline void		ResetHeader();
	CObject* mp_Dlg;
	CString nullString;
public:
	
	std::map<CString,CString> m_IpMap;
	std::map<CString, CString> m_ProxyMap;
	int m_Routing_Table_Max_Index = 0;
	BOOL			Receive(unsigned char* ppayload, BOOL is_in, int interface_ID);
	BOOL			Receive(unsigned char* ppayload, int interface_ID);
	BOOL			Send(unsigned char* ppayload, int nlength, int interface_ID);
	void			RemoveItem(CString IpAddr, CString MacAddr);
	BOOL			UpdateArpCahe(unsigned char* ipAddr, unsigned char* macAddr);
	void			AddProxyItem(CString IpAddr, CString MacAddr);
	void			DeleteProxyItem(CString IpAddr);
	void			DeleteItem(CString IpAddr);
	void			DeleteAllItem();
	void			initRoutingTable();
	unsigned char*	MacAddr2HexInt(CString Mac_address);
	unsigned char*	IpAddr2HexInt(CString Ip_address);
	void			AddRoutingTable(unsigned char * Destination, unsigned char* Netmask, unsigned char* Gateway, CString Flag, int interface_ID);
	void			AddQueue(unsigned char* DstIPAddr, unsigned char* ppayload, int interface_ID);
	ipLayer(char* pName);

	typedef struct _IP_HEADER {
		unsigned char	Version_Length; // total length of the data
		unsigned char	TOS; // type of application data
		unsigned short  TotalLengh;
		unsigned short  Identification;
		unsigned short	Flags_FragmentOffset; // application data
		unsigned char	ToTimeLive;
		unsigned char	Protocol;
		unsigned short	HeaderChecksum;
		unsigned char	source_IP_address[IP_ADDRESS_SIZE];
		unsigned char	target_IP_address[IP_ADDRESS_SIZE];
		unsigned char	data[65515];

	} IP_HEADER, * P_IP_HEADER;
	CString Msg;

	typedef struct _IP_ROUTING_TABLE {
		unsigned char m_Destination[4];
		unsigned char m_Netmask[4];
		unsigned char m_Gateway[4];
		CString m_Flag;
		int m_interfaceID;
	} IP_ROUTING_TABLE, * P_IP_ROUTING_TABLE;
	IP_ROUTING_TABLE m_IP_Routing_Table[IP_ROUTING_TABLE_MAX_SIZE];

	typedef struct _ARP_QUEUE {
		unsigned char   DstAddr[IP_ADDRESS_SIZE];
		unsigned char*	payload;
		int				Interface_ID;

	} IP_QUEUE, * P_IP_QUEUE;
	std::queue<IP_QUEUE> m_Ip_Queue;
	int IP_Queue_Header = 0;
	virtual ~ipLayer();
};

#endif // !defined(AFX_CHATAPPLAYER_H__E78615DE_0F23_41A9_B814_34E2B3697EF2__INCLUDED_)
