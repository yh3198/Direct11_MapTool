#pragma once
#include "BObject.h"

//====================================================================================//
/*SetMapDesc�Լ��� ���� ������ ���� ���� ��ġ ���� �� Create �Լ��� ���� �����ؾ� �մϴ�.*/
//====================================================================================//

class BMap : public BObject
{
public:
	BMapDesc m_MapDesc;					// �� �⺻ ���� ����
	vector<D3DXVECTOR3> m_VertexArray;	// �� ���� ���ؽ� �迭
	vector<float>		m_fHeightList;  // �� ���� ���� �迭
	TCHAR* m_MapTexture;				// �� �ؽ��� �̸�

	float m_iNumRows;					// ���� ���ؽ� (��)
	float m_iNumCols;					// ���� ���ؽ� (��)
	float m_iNumCellRows;				// ���� ���� ����
	float m_iNumCellCols;				// ���� ���� ����
	int m_fCellDistance;				// �� �ϳ��� ���� (�������̹Ƿ� ���� ���� ����)

	ID3D11PixelShader* m_pAlphaPS;		// ���ĸ� �ȼ� ���̴�
	D3DXVECTOR3 m_vLightDir;			// �𷺼� ����Ʈ

public:
	/*�⺻ ���� �Լ�*/
	virtual bool Frame();
	virtual bool Release();

	/*�� �����Լ�*/
	HRESULT Create(ID3D11Device* pDevice, ID3D11DeviceContext* m_pImmediateContext, TCHAR** pLoadTextureString, int TextureCnt, TCHAR* HeightMapString = NULL, const TCHAR* ShaderString = NULL);
	/*���̸� ���� �Լ�*/
	bool CreateHeightMap(TCHAR* strHeightMapText);
	/*�� ���� ���� (���̸� �����ÿ��� ȣ��)*/
	bool SetMapDesc(BMapDesc MapDesc);
	
	/* ������ # ���ؽ� ���� ���� �Լ� */
	virtual HRESULT CreateVertexBuffer();	
	/* ������ # �ε��� ���� ���� �Լ� */
	virtual HRESULT CreateIndexBuffer();		
	/* ������ # �� ���̴� ���̾ƿ� ���� �Լ�*/
	HRESULT LoadShaderAndInputLayout(const TCHAR* pShaderFile);

	/*���� �ش���ġ ���� ��ȯ �Լ� ( int ���� ) */
	virtual float GetHeightMap(int row, int col);
	/*���� �ش���ġ ���� ��ȯ �Լ� ( float ���� )*/
	virtual float GetHeightMap(float row, float col);
	/*���� �ش� �ε����� �븻 ��ȯ �Լ�*/
	virtual D3DXVECTOR3 GetNormalOfVertex(UINT Index);
	
	/* ������ # �븻 & �������� �����Լ�*/
	bool UpdateNormal();
	/* ������ # �� �븻�� ���� ���� �����Լ�*/
	void CalcVertexColor();

	/*�� ������ �Լ�*/
	float Lerp(float fStart, float fEnd, float fTangent);

	/* ���ؽ� ���� �����Լ� */
	bool ReLoadVBuffer();
		

	

public:
	BMap();
	~BMap();
};

