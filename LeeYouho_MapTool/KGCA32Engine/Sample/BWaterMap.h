#pragma once
#include "BCore.h"

#define FOGMODE_NONE    0
#define FOGMODE_LINEAR  1

struct cbFog
{
	D3DXVECTOR4 g_ClipPlane;
	D3DXVECTOR4 g_vCameraPos;
	D3DXVECTOR4 g_FogColor;
	D3DXVECTOR4 g_FogInfo;
};

struct cbTanLight
{
	D3DXMATRIX      matNormal;
	D3DXVECTOR3		vLightPos;
	float			f1;
	D3DXVECTOR3		vEye;
	float			f2;
	D3DXVECTOR3		vEyeDir;
	float			f3;
};

struct cbLightDefault
{
	D3DXVECTOR4				cb_AmbientLightColor;
	D3DXVECTOR4				cb_DiffuseLightColor;
	D3DXVECTOR4				cb_SpecularLightColor;
};

class BWaterMap : public BMapTileRender
{
public:
	BMap* m_pWaterTexMap;
	float m_fWaterHeight;
	D3DXMATRIX m_matNormal;

	cbFog m_Fog;
	cbTanLight m_TanLight;
	cbLightDefault m_LightDefault;

	ComPtr<ID3D11Buffer> m_pFogCB;
	ComPtr<ID3D11Buffer> m_pTanLightCB;
	ComPtr<ID3D11Buffer> m_pLightDefaultCB;

	vector<D3DXVECTOR3>		m_vTangentList;
	ComPtr<ID3D11Buffer>	m_pTangentVB;
	
	ComPtr<ID3D11ShaderResourceView> m_WaterMapUpsideSRV;
	ComPtr<ID3D11ShaderResourceView> m_WaterBottomSRV;
	ComPtr<ID3D11ShaderResourceView> m_WaterHeightSRV;
	ComPtr<ID3D11ShaderResourceView> m_WaterNormalSRV;

public:
	HRESULT TextureLoad(ID3D11Device* pDevice, const TCHAR* strNormalTex = NULL, const TCHAR* strHeightTex = NULL);
	HRESULT SetWaterNormalTexture(ID3D11Device* pDevice, const TCHAR* strFilePath);
	HRESULT SetHeightTexture(ID3D11Device* pDevice, const TCHAR* strFilePath);
	bool Render(ID3D11DeviceContext* pContext);
	bool Build(BMap* pMap, DWORD dwWidth, DWORD dwHeight, ID3D11DeviceContext* pImmediateContext, const TCHAR* strNormalTex = NULL, const TCHAR* strHeightTex = NULL);
	bool SetMapHeight(float Height = 0.0f);
	bool SetMapHeight();
	HRESULT CreateWaterMapInputLayout(const TCHAR* pShaderFile);
	void SetWaterMatrix(D3DXMATRIX* World, D3DXMATRIX* View, D3DXMATRIX* Proj);
	bool ClipRender(ID3D11DeviceContext* pContext);
	HRESULT CreateWaterConstantBuffer();
	bool UpdateBuffer();
public:
	BWaterMap();
	~BWaterMap();
};

