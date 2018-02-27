#pragma once
#include "BObject.h"

//====================================================================================//
/*�⺻ ��� Ŭ�����Դϴ�.*/
/*������Ʈ�� ������ �����մϴ�.*/
/*CreatePlane�� ���� �ɹ����� fA, fB, fC, fD�� �����Ͽ� ����� ������ ������ �����մϴ�.*/
//====================================================================================//

class BPlane : public BObject
{
public:
	float fA, fB, fC, fD; // ����� �����Ŀ� ����

public:
	/*������ # ���ؽ� ���� ���� �Լ�*/
	virtual HRESULT CreateVertexBuffer();
	/*������ # �ε��� ���� ���� �Լ�*/
	virtual HRESULT CreateIndexBuffer();
	/*������ # MinMax�ڽ� ���� �Լ�*/
	void UpdateVertexMinMax();

	/*����� ������ �����Լ� (3���� ���ؽ� �̿�)*/
	bool CreatePlane(D3DXVECTOR3 v0, D3DXVECTOR3 v1, D3DXVECTOR3 v2);
	/*����� ������ �����Լ� (�븻���Ϳ� 1���� ���ؽ� �̿�)*/
	bool CreatePlane(D3DXVECTOR3 vNormal, D3DXVECTOR3 v0);
	
public:
	BPlane();
	~BPlane();
};

