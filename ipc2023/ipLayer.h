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
#include <tuple>
class ipLayer
	: public CBaseLayer
{
private:
	inline void		ResetHeader();
	CObject* mp_Dlg;
	CString nullString;
public:
	//BOOL			sendAck(unsigned char* ppayload);
	std::map<CString,CString> m_IpMap;
	std::map<CString, CString> m_ProxyMap;
	std::map<CString, CString> m_RouterMap;
	BOOL			Receive(unsigned char* ppayload, BOOL is_in);
	BOOL			Send(unsigned char* ppayload, int nlength);
	void			RemoveItem(CString IpAddr, CString MacAddr);
	BOOL			UpdateArpCahe(unsigned char* ipAddr, unsigned char* macAddr);
	void			AddProxyItem(CString IpAddr, CString MacAddr);
	void			AddRouterItem(CString DesAddr, CString NetMask, CString GateWay, CString Flag);
	void			DeleteProxyItem(CString IpAddr);
	void			DeleteRouterItem(CString IpAddr);	// add
	void			DeleteItem(CString IpAddr);
	void			DeleteAllItem();
	ipLayer(char* pName);
	virtual ~ipLayer();
};

#endif // !defined(AFX_CHATAPPLAYER_H__E78615DE_0F23_41A9_B814_34E2B3697EF2__INCLUDED_)










