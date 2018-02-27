#pragma once
#include "BRenderPlaneHeight.h"
#include "BMapTileRender.h"
#include "BNMap.h"

#define FOGMODE_NONE    0 // ���� �̻��
#define FOGMODE_LINEAR  1 // ���� ���
#pragma region ����, ������ ���� ����ü
/*���� ����ü*/
struct cbFog
{
	D3DXVECTOR4 g_ClipPlane;
	D3DXVECTOR4 g_vCameraPos;
	D3DXVECTOR4 g_FogColor;
	D3DXVECTOR4 g_FogInfo;
	float		g_fogMode;		//x(������)
	float		g_fogDensity;	//y(��)
	float		g_fogStart;		//z(�������װŸ�)
	float		g_fogEnd;		//w(�������װŸ�)
};

/*�븻���� ����ü*/
struct cbTanLight
{
	D3DXMATRIX      matNormal;
	D3DXVECTOR4		vLightPos;
	D3DXVECTOR4		vEye;
	D3DXVECTOR4		vEyeDir;
};

/*������ ����ü*/
struct cbLightDefault
{
	D3DXVECTOR4				cb_AmbientLightColor;
	D3DXVECTOR4				cb_DiffuseLightColor;
	D3DXVECTOR4				cb_SpecularLightColor;
};
#pragma endregion

//====================================================================================//
/*���͸��� LOD, ����Ʈ���� ����� ���·� ���� �ʰ� �����ϰ� ����Ͻø� �˴ϴ�.*/
/*�ʰ� �����ϰ� ���ν��� ���� �ִ� �κи��� �������Ͽ� ����ȭ�Ǿ� �ֽ��ϴ�.*/
/*���͸� Ŭ������ ������ WaterFrame �Լ��� �����ϰ� �ֽ��ϴ� Frame�� WaterFrame �� �� ���ư��� �մϴ�.*/
/*Buildȣ�� �� �ѱ�� pMap�� ���� ���� �ƴ� ���͸����� ������ ���Դϴ�.*/
//====================================================================================//


class BWaterMap : public BMapTileRender
{
public:
	BMap* m_pWaterTexMap;					// ���͸��� ������ ( ���͸��� ũ�� ��ġ���� ���� )
	BMapTileRender* m_pWaterReflectMap;		// ���͸��� �ݻ�, ���� ������ ���ҽ��� ����� ���� ������
	TCHAR* m_strWaterTex;					// ���͸��� �ؽ��� ��
	TCHAR* m_strWaterNormalTex;				// ���͸��� �븻�ؽ��� ��

	int m_iSelectedMap;						// ���͸����� ���õ� ���� ��ȣ ( ���� �� ����, ����Ʈ�� �̿�� ��� )
	float m_fWaterHeight;					// ���͸��� ���̰�
	float m_fWaterWave;						// ���͸��� ���̺갪
	D3DXMATRIX m_matNormal;					// ���͸��� �븻���

	cbFog m_Fog;							// ���� ����
	cbTanLight m_TanLight;					// �븻 ����Ʈ ����
	cbLightDefault m_LightDefault;			// ����Ʈ ����

	ComPtr<ID3D11Buffer> m_pFogCB;			// ���� �������
	ComPtr<ID3D11Buffer> m_pTanLightCB;		// �븻 ����Ʈ �������
	ComPtr<ID3D11Buffer> m_pLightDefaultCB; // ����Ʈ �������

	vector<D3DXVECTOR3>		m_vTangentList; // �������� ����Ʈ
	ComPtr<ID3D11Buffer>	m_pTangentVB;	// �������� ����
	
	ComPtr<ID3D11ShaderResourceView> m_WaterMapUpsideSRV; // m_pReflectRV // �ݻ� ���ҽ�
	ComPtr<ID3D11ShaderResourceView> m_WaterBottomSRV;	  // m_pRefractRV // ���� ���ҽ�
	ComPtr<ID3D11ShaderResourceView> m_WaterHeightSRV;	  // ���͸ʿ� ����ǰ� �ִ� ���� ���� ���̸� ���ҽ� ( �̸� ���� ���̿� ���� ���׸� ���� )
	ComPtr<ID3D11ShaderResourceView> m_WaterNormalSRV;	  // �븻 ���ҽ�
	ComPtr<ID3D11ShaderResourceView> m_WaterSRV;		  // �ؽ��� ���ҽ�

	ID3D11VertexShader*		m_pWaterVS; // ���ؽ� ���̴�
	ID3D11PixelShader*		m_pWaterPS; // �ȼ� ���̴�
		
	BRenderPlaneHeight m_HeightMap;		// �������� ���̸�

	bool bHeightChange;					// ���� ���濩�� �Ǵܺ���
	bool bRequestCreate;				// ���̸� ���� ��û���� �Ǵܺ���

	BMapTileRender* m_SelectedMap;		// ���͸� �������� ����Ʈ�� ( Import�� ��� )
	BRenderPlane m_MiniMap[3];			// �ݻ� ���� ���� ���ҽ��� ����� ���� �̴ϸ�
	
public:
	/*�⺻ ���� �Լ�*/
	bool Frame(D3D11_VIEWPORT OldViewPort, ID3D11DepthStencilView* OldDepthStencilView, ID3D11RenderTargetView* OldRenderTargetView);
	bool Render();
	bool PostRender(ID3D11DeviceContext* pContext);
	
	/*������ # ���͸� �����Լ�*/
	bool Build(BMap* pMap, DWORD dwWidth, DWORD dwHeight, ID3D11DeviceContext* pContext, TCHAR* strWaterTex = NULL, TCHAR* strNormalTex = NULL, TCHAR* strHeightTex = NULL);
	/*���͸� ���� ���̾ƿ� �����Լ� (Build ���� ȣ��)*/
	HRESULT CreateWaterMapInputLayout(const TCHAR* pShaderFile);
	/*������� �����Լ� (Build ���� ȣ��)*/
	HRESULT CreateWaterConstantBuffer();
	/*������ # ���͸� �ٿ���ڽ� �����Լ�*/
	void ComputeBoundingBox(BNode* pNode);

	/*���͸� ���� ������ �Լ�(Frame ���� ȣ��)*/
	bool WaterFrame(D3D11_VIEWPORT OldViewPort, ID3D11DepthStencilView* OldDepthStencilView, ID3D11RenderTargetView* OldRenderTargetView);
	/*�ݻ� ����� �����Լ� (WaterFrame ���� ȣ��)*/
	D3DXMATRIX* D3DXMatrixReflect_Fixed(__inout D3DXMATRIX *pOut, __in const D3DXPLANE *pPlane);
	/*�ݻ�, ���� ���ҽ� ���� �Լ� (WaterFrame ���� ȣ��)*/
	void DrawWaterRT(int iMap, D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj, D3D11_VIEWPORT OldViewPort, ID3D11DepthStencilView* OldDepthStencilView, ID3D11RenderTargetView* OldRenderTargetView);
	
	/*������ # �ؽ��� ���� �Լ�*/
	HRESULT TextureLoad(const TCHAR* strWaterTex, const TCHAR* strNormalTex, const TCHAR* strHeightTex);
	/*���� �ؽ��� �����Լ�*/
	HRESULT SetWaterTexture(const TCHAR* strFilePath);
	/*���� �븻�ؽ��� �����Լ�*/
	HRESULT SetWaterNormalTexture(const TCHAR* strFilePath);
	/*���� �����ؽ��� �����Լ�*/
	HRESULT SetHeightTexture(const TCHAR* strFilePath);

	/*���� ���̰� �����Լ� (���� ȣ�� �ʼ� / �Ǽ� ������ ����)*/
	bool SetMapHeight(float Height = 0.0f);
	/*���� ���̰� �����Լ� (���� ȣ�� �ʼ� / ������� ������ ����)*/
	bool SetMapHeight();
	/*���͸� �⺻ ��� �����Լ�(���� ȣ�� �ʼ� / ����̽� ī�޶� ��)*/
	void Update(ID3D11Device* pd3dDevice, BCamera* pCamera, BMapTileRender* SelectedMap);
	/*���͸� ��� �����Լ�*/
	void SetWaterMatrix(D3DXMATRIX* World, D3DXMATRIX* View, D3DXMATRIX* Proj);



public:
	BWaterMap();
	~BWaterMap();
};

