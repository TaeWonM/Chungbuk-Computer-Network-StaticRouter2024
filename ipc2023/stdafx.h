// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__119ECB1B_6E70_4662_A2A9_A20B5201CA81__INCLUDED_)
#define AFX_STDAFX_H__119ECB1B_6E70_4662_A2A9_A20B5201CA81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


//{{AFX_INSERT_LOCATION}}

#define MAX_LAYER_NUMBER		0xff

#define ETHER_MAX_SIZE			1514
#define ETHER_HEADER_SIZE		14
#define ETHER_MAX_DATA_SIZE		( ETHER_MAX_SIZE - ETHER_HEADER_SIZE )

#define TCP_HEADER_SIZE			20
#define IP_HEADER_SIZE			20

#define APP_HEADER_SIZE			( sizeof(unsigned short) +					\
								  sizeof(unsigned char) * 2	)
#define APP_DATA_SIZE			( ETHER_MAX_DATA_SIZE - ( APP_HEADER_SIZE ))

#define FILE_HEADER_SIZE		12
#define FILE_DATA_SIZE			( ETHER_MAX_DATA_SIZE - ( FILE_HEADER_SIZE ))

#define	ETHER_ADDRESS_SIZE			6
#define IP_ADDRESS_SIZE				4
#define	ARP_HEADER_SIZE				2 + 2 + 1 + 1 + 2 + ETHER_ADDRESS_SIZE * 2 + IP_ADDRESS_SIZE * 2

#define BOARDCAST_ETH_ADRR          0xffffffffffff

#define ADAPTER_MAX_SIZE			7
#define IP_ROUTING_TABLE_MAX_SIZE	20
#define IP_QUEUE_SIZE				10
#define ICMP_HEADER_SIZE			8
#define IP_HEADER_SIZE				20

// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__119ECB1B_6E70_4662_A2A9_A20B5201CA81__INCLUDED_)
