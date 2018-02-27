#pragma once
#include "BDxRT.h"
#include "BMap.h"
#include "BMapTileRender.h"

//====================================================================================//
/*WaterMap�� �ݻ�, �������� �̹����� ���Ǵ� Ŭ�����Դϴ�.*/
/*���̸��� ���� �ؽ��� ������ ������ �����մϴ�.*/
/*�⺻���� ������ BDxRT Ŭ������ �����մϴ�.*/
//====================================================================================//

class BDxRTHeight : public BDxRT
{
public:
	ComPtr<ID3D11Texture2D> m_pHeightTexture; // ���̸� ���� �ؽ���

public:
	/*����Ʈ �����Լ� # ������*/
	HRESULT Create(ID3D11Device* pd3dDevice, DXGI_FORMAT format, FLOAT Width = 1024, FLOAT Height = 1024);

	/*������ ���̸� �ؽ��ĸ� �����ϴ� �Լ�*/
	HRESULT DrawHeightTex(ID3D11Device* pd3dDevice, ID3D11DeviceContext* m_pContext, BMap* m_pMap);

	//====================================================================================//
	/*DrawQuadTreeHeightTex �Լ�*/
	// ���� ���� ���� ����� ȣ���� �� �ʿ��մϴ�. ( �� ����, ���� �� )
	// WaterMap�� ������ �ش� �Լ��� ���� ����� ���̿� ����Ͽ� ������ ����˴ϴ�.
	//====================================================================================//
	/*���� ���̸� ����Ʈ �ؽ��Ŀ� �����ϴ� �Լ�*/
	HRESULT DrawQuadTreeHeightTex(ID3D11Device* pd3dDevice, ID3D11DeviceContext* m_pContext, BMapTileRender* m_pMap);
	/*����Ʈ���� ���̰��� ���̸ʿ� �����Ű�� �Լ�(DrawQuadTreeHeightTex ���� ȣ��)*/
	void DrawNodeHeightTex(BNode* pNode, BYTE* pDestBytes, int iChangePoint, float fCellDistance, UINT RowPitch, float fRatio, float fMin);
	/*����Ʈ���� MaxY �� �����Լ�(DrawQuadTreeHeightTex ���� ȣ��)*/
	void SetRootNodeYPosition(BNode* pNode, float* MaxY, float* MinY);

public:
	BDxRTHeight();
	~BDxRTHeight();
};

