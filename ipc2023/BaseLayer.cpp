// BaseLayer.cpp: implementation of the CBaseLayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ipc2023.h"
#include "BaseLayer.h"
#include "pch.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseLayer::CBaseLayer(char* pName)
	: m_nUpperLayerCount(0), m_nUnderLayerCount(0)
{
	m_pLayerName = pName;
}

CBaseLayer::~CBaseLayer()
{

}

void CBaseLayer::SetUnderUpperLayer(CBaseLayer* pUULayer)
{
	if (!pUULayer) // if the pointer is null, 
	{
#ifdef _DEBUG
		TRACE("[CBaseLayer::SetUnderUpperLayer] The variable , 'pUULayer' is NULL");
#endif
		return;
	}

	//////////////////////// fill the blank ///////////////////////////////
		// ���ڷ� ���� ������ ���� ������ Under�� ����
		// ���� ������ ���ڷ� ���� ������ Upper�� ���´�.
	SetUnderLayer(pUULayer);
	pUULayer->SetUpperLayer(this);
	///////////////////////////////////////////////////////////////////////
}
// ���� layer�� �Ʒ� �������� �����ϰ�, ���� layer�� �� ������ ������ ������ �����մϴ�.

void CBaseLayer::SetUpperUnderLayer(CBaseLayer* pUULayer)
{
	if (!pUULayer) // if the pointer is null, 
	{
#ifdef _DEBUG
		TRACE("[CBaseLayer::SetUpperUnderLayer] The variable , 'pUULayer' is NULL");
#endif
		return;
	}

	//////////////////////// fill the blank ///////////////////////////////
		// ���ڷ� ���� ������ Upper�� ����
		// ���� ������ Upper�� ���� ������ Under�� ���´�.
	SetUpperLayer(pUULayer);
	pUULayer->SetUnderLayer(this);
	///////////////////////////////////////////////////////////////////////
}
// ���� �Լ��� �ݴ�� ���� layer�� �� �������� �����ϰ�, ���� layer�� �Ʒ� ������ ������ ������ �����մϴ�.

void CBaseLayer::SetUpperLayer(CBaseLayer* pUpperLayer)
{
	if (!pUpperLayer) // if the pointer is null, 
	{
#ifdef _DEBUG
		TRACE("[CBaseLayer::SetUpperLayer] The variable , 'pUpperLayer' is NULL");
#endif
		return;
	}

	// UpperLayer is added..
	this->mp_aUpperLayer[m_nUpperLayerCount++] = pUpperLayer;
}
// ���� ������ �����ϴ� �Լ��Դϴ�. mp_aUpperLayer��� �迭�� pUpperLayer�� �ֽ��ϴ�.

void CBaseLayer::SetUnderLayer(CBaseLayer* pUnderLayer)
{
	if (!pUnderLayer) // if the pointer is null, 
	{
#ifdef _DEBUG
		TRACE("[CBaseLayer::SetUnderLayer] The variable , 'pUnderLayer' is NULL\n");
#endif
		return;
	}

	// UnderLayer assignment..
	this->mp_UnderLayer[m_nUnderLayerCount++] = pUnderLayer;
}
// �Ʒ��� ������ �����ϴ� �Լ��Դϴ�. mp_aUpperLayer��� �迭�� pUpperLayer�� �ֽ��ϴ�.


CBaseLayer* CBaseLayer::GetUpperLayer(int nindex)
{
	if (nindex < 0 ||
		nindex > m_nUpperLayerCount ||
		m_nUpperLayerCount < 0)
	{
#ifdef _DEBUG
		TRACE("[CBaseLayer::GetUpperLayer] There is no UpperLayer in Array..\n");
#endif 
		return NULL;
	}

	return mp_aUpperLayer[nindex];
}
// index�� ���ؼ� ������ ��� getter�Լ��Դϴ�.

CBaseLayer* CBaseLayer::GetUnderLayer(int index)
{
	if (index < 0 ||
		index > m_nUnderLayerCount ||
		m_nUnderLayerCount < 0)
	{
#ifdef _DEBUG
		TRACE("[CBaseLayer::GetUnderLayer] There is not a UnerLayer..\n");
#endif 
		return NULL;
	}

	return mp_UnderLayer[index];
}
// ���� �������� �Ʒ� �����̶�� ������ ���� ����ϴ� �κ��Դϴ�.

char* CBaseLayer::GetLayerName()
{
	return m_pLayerName;
}
// ���� ������ �̸��� Ȯ���ϴ� getter�Լ��Դϴ�.

