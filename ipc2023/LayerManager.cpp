#pragma once
// LayerManager.cpp: implementation of the CLayerManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "pch.h"
#include "LayerManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLayerManager::CLayerManager()
	: m_nLayerCount(0),
	mp_sListHead(NULL),
	mp_sListTail(NULL),
	m_nTop(-1)
{

}

CLayerManager::~CLayerManager()
{

}

void CLayerManager::AddLayer(CBaseLayer* pLayer)
{
	mp_aLayers[m_nLayerCount++] = pLayer;
}
// mp_aLayers��� �������� ���� �迭�� ���ο� ������ �ִ� setter �Լ��Դϴ�.

CBaseLayer* CLayerManager::GetLayer(int nindex)
{
	return mp_aLayers[nindex];
}
// mp_aLayers��� �������� ���� �迭�� �ִ� ������ ��� getter �Լ��Դϴ�.

CBaseLayer* CLayerManager::GetLayer(char* pName)
{
	for (int i = 0; i < m_nLayerCount; i++)
	{
		if (!strcmp(pName, mp_aLayers[i]->GetLayerName()))
			return mp_aLayers[i];
	}

	return NULL;
}
// ������ ������ pName string�� ���ؼ� pName �̸��� ���� ������ ����� �Լ��Դϴ�.

void CLayerManager::ConnectLayers(char* pcList)
{
	MakeList(pcList);
	LinkLayer(mp_sListHead);
	int arr;
	arr = 3;
}
// ������ ������ pcList string�� ���ؼ� ������ ����� �Լ��Դϴ�.

void CLayerManager::MakeList(char* pcList)
{
	while (pcList != (char*)0x01)
	{
		char sBuff[100];
		sscanf_s(pcList, "%s", sBuff, sizeof(sBuff));
		pcList = strchr(pcList, ' ') + 1;

		PNODE pNode = AllocNode(sBuff);
		AddNode(pNode);
	}
}
// ������ ������ pcList string�� string ������ �о ���ο� Pnode�� ������ ����� �Լ��Դϴ�.

CLayerManager::PNODE CLayerManager::AllocNode(char* pcName)
{
	PNODE node = new NODE;
	ASSERT(node);

	strcpy_s(node->token, pcName);
	node->next = NULL;

	return node;
}
// pNode�� ���� �Ҵ��ϴ� �κ��Դϴ�.
// node�� �̸��� next�� �����Ǿ� �ֽ��ϴ�.

void CLayerManager::AddNode(PNODE pNode)
{
	if (!mp_sListHead)
	{
		mp_sListHead = mp_sListTail = pNode;
	}
	else
	{
		mp_sListTail->next = pNode;
		mp_sListTail = pNode;
	}
}
// pNode�� �ִ� �κ��Դϴ�. ���� �� ������� �̾����� �����Դϴ�.


void CLayerManager::Push(CBaseLayer* pLayer)
{
	if (m_nTop >= MAX_LAYER_NUMBER)
	{
#ifdef _DEBUG
		TRACE("The Stack is full.. so cannot run the push operation.. \n");
#endif
		return;
	}

	mp_Stack[++m_nTop] = pLayer;
}
// layer�� mp_Stack�� �ִ� Push �Լ��Դϴ�.

CBaseLayer* CLayerManager::Pop()
{
	if (m_nTop < 0)
	{
#ifdef _DEBUG
		TRACE("The Stack is empty.. so cannot run the pop operation.. \n");
#endif
		return NULL;
	}

	CBaseLayer* pLayer = mp_Stack[m_nTop];
	mp_Stack[m_nTop] = NULL;
	m_nTop--;

	return pLayer;
}
// mp_ Stack�� �ִ� layer�� ���� pop �Լ��Դϴ�.

CBaseLayer* CLayerManager::Top()
{
	if (m_nTop < 0)
	{
#ifdef _DEBUG
		TRACE("The Stack is empty.. so cannot run the top operation.. \n");
#endif
		return NULL;
	}

	return mp_Stack[m_nTop];
}
// mp_Stack�� top�κ��� ȣ���ϴ� �κ��Դϴ�.

void CLayerManager::LinkLayer(PNODE pNode)
{
	CBaseLayer* pLayer = NULL;

	while (pNode)
	{
		if (!pLayer)
			pLayer = GetLayer(pNode->token);
		else
		{
			if (*pNode->token == '(')
				Push(pLayer);
			else if (*pNode->token == ')')
				Pop();
			else
			{
				char cMode = *pNode->token;
				char* pcName = pNode->token + 1;

				pLayer = GetLayer(pcName);

				switch (cMode)
				{
				case '*': Top()->SetUpperUnderLayer(pLayer); break;
				case '+': Top()->SetUpperLayer(pLayer); break;
				case '-': Top()->SetUnderLayer(pLayer); break;
				}
			}
		}

		pNode = pNode->next;
	}
}
// pNode�� �μ��� �޾� token�� Ȯ���� ��� �������� Ȯ���ϴ� �Լ��Դϴ�.
// * �̸� �� �Ʒ��� + �̸� ����, - �̸� �Ʒ��� �����ϴ� �Լ��Դϴ�.

void CLayerManager::DeAllocLayer()
{
	for (int i = 0; i < this->m_nLayerCount; i++)
		delete this->mp_aLayers[i];
}
// ���� �Ҵ�� mp_aLayers�� �����ϴ� �ռ��Դϴ�. 
