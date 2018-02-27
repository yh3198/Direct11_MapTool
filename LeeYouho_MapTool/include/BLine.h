#pragma once
#include "BObject.h"

//====================================================================================//
/*Create �Լ��� �����մϴ�.*/
/*m_iPrimitiveType�� ���� ������ 2�� ������ �ξ�� �մϴ�.*/
/*Frame ���� SetMatrix�� ������ ���¸� �������ֱ� ���� SetLineStartToMove || SetLineStartToEnd �� �̿��մϴ�. */
/*Render�� ���� ����մϴ�.*/
//====================================================================================//


class BLine : public BObject
{
public:
	vector<PNCT_Vertex>	m_LineVertexList;	// ���� ���ؽ� ����Ʈ

public:
	/*������ # ���̴� �� ���̾ƿ� ���� �Լ�*/
	HRESULT LoadShaderAndInputLayout(const TCHAR* pShaderFile);
	/*������ # ���ؽ� ���� ���� �Լ�*/
	HRESULT CreateVertexBuffer();
	/*������ # �ε��� ���� ���� �Լ�*/
	HRESULT CreateIndexBuffer();

	/*���� ��ġ���� �Լ�(������ + �̵�����)*/
	bool SetLineStartToMove(ID3D11DeviceContext* pContext, D3DXVECTOR3 vStart, D3DXVECTOR3 Move, D3DXVECTOR4 vColor);
	/*���� ��ġ���� �Լ�(������, ����)*/
	bool SetLineStartToEnd(ID3D11DeviceContext* pContext, D3DXVECTOR3 vStart, D3DXVECTOR3 vEnd, D3DXVECTOR4 vColor);

public:
	BLine();
	~BLine();
};

