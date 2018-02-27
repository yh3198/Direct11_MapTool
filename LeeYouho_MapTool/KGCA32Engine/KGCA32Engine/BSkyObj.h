#pragma once
#include "BBox.h"

//====================================================================================//
/*��ī�̹ڽ� ���� Ŭ�����Դϴ�.*/
/*�ڽ��� ������ �����մϴ�.*/
/*Create ������ 6���� �ؽ��İ� �ʿ��մϴ�.*/
/*��ī�̹ڽ� �ؽ��ļ���*/
// ��, ��, ������, �Ʒ�, ��, ����
//====================================================================================//


class BSkyObj : public BBox
{
public:
	ID3D11ShaderResourceView* m_pTextureSkySRV[6]; // ��ī�� �ڽ� �ؽ���

public:
	/*������ # ��� ���� �Լ�*/
	void BSkyObj::SetMatrix(D3DXMATRIX World, D3DXMATRIX View, D3DXMATRIX Proj);

public:
	BSkyObj();
	~BSkyObj();
};