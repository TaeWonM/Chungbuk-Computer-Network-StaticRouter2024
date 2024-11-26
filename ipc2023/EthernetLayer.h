// EthernetLayer.h: interface for the CEthernetLayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ETHERNETLAYER_H__7857C9C2_B459_4DC8_B9B3_4E6C8B587B29__INCLUDED_)
#define AFX_ETHERNETLAYER_H__7857C9C2_B459_4DC8_B9B3_4E6C8B587B29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseLayer.h"
#include "pch.h"

class CEthernetLayer
	: public CBaseLayer
{
private:
	inline void		ResetHeader();

public:
	BOOL			Receive(unsigned char* ppayload, int Interface_ID);
	BOOL			Send(unsigned char* ppayload, int nlength, int DetLayer, int interface_ID);
	BOOL			SetMacDstAddress(unsigned char* ppayload, int interface_ID);
	BOOL			SetMacSrcAddress(unsigned char* ppayload, int interface_ID);
	void			SetDestinAddress(unsigned char* pAddress, int index);
	void			SetSourceAddress(unsigned char* pAddress, unsigned char* pAddress2);
	unsigned char* GetSourceAddress(int index);
	unsigned char* GetDstAddress(int index);
	unsigned char BROADCASTING_ADDR[6];
	void SetBroadcasting_address();
	bool is_Broadcast = false;
	CEthernetLayer(char* pName);
	virtual ~CEthernetLayer();

	typedef struct _ETHERNET_HEADER {

		unsigned char	enet_dstaddr[6];		// destination address of ethernet layer
		unsigned char	enet_srcaddr[6];		// source address of ethernet layer
		unsigned short	enet_type;		// type of ethernet layer
		unsigned char	enet_data[ETHER_MAX_DATA_SIZE]; // frame data

	} ETHERNET_HEADER, * PETHERNET_HEADER;
protected:
	ETHERNET_HEADER	m_sHeader[2];
};

#endif // !defined(AFX_ETHERNETLAYER_H__7857C9C2_B459_4DC8_B9B3_4E6C8B587B29__INCLUDED_)
