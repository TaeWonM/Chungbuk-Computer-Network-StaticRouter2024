#pragma once
// BaseLayer.h: interface for the CBaseLayer class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"
#include"stdafx.h"

#if !defined(AFX_BASELAYER_H__041C5A07_23A9_4CBC_970B_8743460A7DA9__INCLUDED_)
#define AFX_BASELAYER_H__041C5A07_23A9_4CBC_970B_8743460A7DA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBaseLayer
{
public:
	char* GetLayerName();

	CBaseLayer* GetUnderLayer(int nindex);
	CBaseLayer* GetUpperLayer(int nindex);
	void			SetUnderUpperLayer(CBaseLayer* pUULayer = NULL);
	void			SetUpperUnderLayer(CBaseLayer* pUULayer = NULL);
	void			SetUnderLayer(CBaseLayer* pUnderLayer = NULL);
	void			SetUpperLayer(CBaseLayer* pUpperLayer = NULL);

	CBaseLayer(char* pName = NULL);
	virtual ~CBaseLayer();

	// param : unsigned char*	- the data of the upperlayer
	//         int				- the length of data
	virtual	BOOL	Send(unsigned char*, int nlength, int interface_ID) { return FALSE; }
	virtual	BOOL	Send(unsigned char*, int nlength, int DetLayer, int interface_ID) { return FALSE; }
	virtual BOOL	SendArp(unsigned char* ppayload, unsigned char* MacAddr, int nlength, int interface_ID) { return FALSE; }
	// param : unsigned char*	- the data of the underlayer
	virtual	BOOL	Receive(int interface_ID) { return FALSE; }
	virtual BOOL	Receive(unsigned char* ppayload, int interface_ID) { return FALSE; };
	virtual BOOL	Receive(unsigned char* ppayload, BOOL is_in, int interface_ID) { return FALSE; };
	virtual	BOOL	Receive(CString IpAddr, CString MacAddr, BOOL is_in, int interface_ID) { return FALSE; };
	virtual BOOL	SetMacDstAddress(unsigned char* ppayload, int interface_ID) { return mp_UnderLayer[0]->SetMacDstAddress(ppayload, interface_ID); }
	virtual BOOL	UpdateArpCahe(unsigned char* ipAddr, unsigned char* macAddr) { return mp_UnderLayer[0]->UpdateArpCahe(ipAddr, macAddr); }
	virtual BOOL	SetMacSrcAddress(unsigned char* ppayload, int interface_ID) { return mp_UnderLayer[0]->SetMacSrcAddress(ppayload, interface_ID); }
protected:
	char* m_pLayerName;
	CBaseLayer* mp_UnderLayer[MAX_LAYER_NUMBER];							// UnderLayer pointer
	CBaseLayer* mp_aUpperLayer[MAX_LAYER_NUMBER];		// UpperLayer pointer
	int				m_nUpperLayerCount;						// UpperLayer Count
	int				m_nUnderLayerCount;
};

#endif // !defined(AFX_BASELAYER_H__041C5A07_23A9_4CBC_970B_8743460A7DA9__INCLUDED_)
