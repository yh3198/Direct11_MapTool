#pragma once
#include "BMap.h"
//====================================================================================//
/*�븻������ ���� Tangent ���� �߰��� ������ ���Դϴ�.*/
/*������ ���� �� Ŭ������ �����մϴ�.*/
//====================================================================================//


class BNMap : public BMap
{
public:
	ID3D11VertexShader*		m_pNoneVS; // Clip ���� ���ؽ� ���̴�

public:
	/*���̴� ���̾ƿ� ���� �Լ�*/
	HRESULT LoadShaderAndInputLayout(const TCHAR* pShaderFile);
	
	/*��� ���ÿ� �Լ�*/
	void SetMatrix(D3DXMATRIX* World, D3DXMATRIX* View, D3DXMATRIX* Proj);
	
	/*������ Clip�� ���� ������ ���� �Լ�*/
	bool RenderNoneClip(ID3D11DeviceContext* pContext);

public:
	BNMap();
	~BNMap();
};

