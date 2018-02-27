#include "BWaterMap.h"

// 워터맵의 높이맵 //
// Create 에서 높이맵을 받아온다.
// 워터맵의 텍스쳐는 워터맵에 해당하는 높이 텍스쳐만을 사용한다. 
// 만약 높이텍스쳐를 사용하지 않는다면 ?
// 워터맵은 맵과 같은 크기의 맵이다. 맵을 읽어올때와 같은 방식으로 y값을 받아와서 워터맵의 높이맵 생성이 가능해진다.
// 맵을 읽어올 때 워터맵도 함께 생성하도록 한다.

HRESULT BWaterMap::TextureLoad(ID3D11Device* pDevice, const TCHAR* strNormalTex, const TCHAR* strHeightTex)
{
	HRESULT hr = S_OK;
	if (strNormalTex)
	{
		hr = SetWaterNormalTexture(pDevice, strNormalTex);
	}
	if (strHeightTex)
	{
		hr = SetHeightTexture(pDevice, strHeightTex);
	}
	return hr;
}

// 워터의 텍스쳐 질감을 표현해 줄 노말맵 ( 아주 중요 )
HRESULT BWaterMap::SetWaterNormalTexture(ID3D11Device* pDevice, const TCHAR* strFilePath)
{
	HRESULT hr = S_OK;
	FAILED(hr = D3DX11CreateShaderResourceViewFromFile(pDevice, strFilePath, NULL, NULL, &m_WaterNormalSRV, NULL));
	return hr;
}
HRESULT BWaterMap::SetHeightTexture(ID3D11Device* pDevice, const TCHAR* strFilePath)
{
	HRESULT hr = S_OK;
	FAILED(hr = D3DX11CreateShaderResourceViewFromFile(pDevice, strFilePath, NULL, NULL, &m_WaterHeightSRV, NULL));
	return hr;
}

bool BWaterMap::Render(ID3D11DeviceContext* pContext)
{
	return true;
}

/*여기서 넘기는 pMap은 실제 Map이 아니라 WaterPlane 이다.*/
bool BWaterMap::Build(BMap* pMap, DWORD dwWidth, DWORD dwHeight, ID3D11DeviceContext* pImmediateContext, const TCHAR* strNormalTex, const TCHAR* strHeightTex)
{
	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;
	//m_pMap은 실제 물이 될 평면
	//받아온 pMap은 텍스쳐로 이용하기 위한 지형
	m_pMap = new BMap;
	m_pMap->SetMapDesc(pMap->m_MapDesc);
	m_pMap->Create(m_pd3dDevice, m_pImmediateContext, NULL, 0);
	m_pMap->UpdateNormal();
	CreateWaterMapInputLayout(L"../../data/shader/WaterMapShader");
	m_pWaterTexMap = pMap;

	TextureLoad(m_pd3dDevice, strNormalTex, strHeightTex);

	m_iNumFace = (dwWidth - 1) *(dwHeight - 1) * 2;
	m_bUsedIndexList = true;
	m_pImmediateContext = pImmediateContext;

	m_pRootNode = CreateNode(NULL, 0, dwWidth - 1, dwWidth * (dwHeight - 1), dwWidth * dwHeight - 1);

	InitLevelOrder(m_pRootNode, m_iMaxDepthLimit); // LOD용 함수 LOD 레벨 resize
	m_pIndexBuffer.Attach(CreateIndexBuffer(m_pd3dDevice, NULL, m_iNumFace * 3, sizeof(DWORD)));

	if (BuildTree(m_pRootNode))
	{
		for (int iNode = 0; iNode < 4; iNode++)
		{
			SetNeighborNode(m_pRootNode->m_pChild[iNode]);
		}
		SetLOD(m_dwWidth, m_iMaxDepth);

		// LOD용 인덱스 제작 함수
		ComputeStaticLodIndex(m_iNumCell);
		return true;
	}
	return false;
}

// 높이값의 변화로 물의 높낮이 설정
// Init 와는 별개로 설정해주어야 한다.
bool BWaterMap::SetMapHeight(float Height)
{
	m_fWaterHeight = Height;
	m_pMap->m_matWorld._42 = m_fWaterHeight;
	return true;
}

bool BWaterMap::SetMapHeight()
{
	m_pMap->m_matWorld._42 = m_fWaterHeight;
	return true;
}

HRESULT BWaterMap::CreateWaterMapInputLayout(const TCHAR* pShaderFile) // WaterMapShader를 이용
{
	HRESULT hr;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS; // 쉐이더 디버그 허용
	ID3DBlob* pVSBUF = NULL;
	ID3DBlob* pPSBUF = NULL;
	ID3DBlob* pGSBUF = NULL;

	ID3DBlob* pError = NULL; // 이것을 넣음으로써 오류가 난 이유를 받아 낼 수 있다.
							 /* vs_5_0 == direct11 은 5.0을 사용 * pVSBuf 결과가 담김 */

	if (FAILED(D3DX11CompileFromFile(pShaderFile, NULL, NULL, "Water_VS", "vs_5_0", dwShaderFlags, NULL, NULL, &pVSBUF, &pError, NULL)))
	{
		TCHAR buffer[256] = { 0, };
		mbstowcs(buffer, (char*)pError->GetBufferPointer(), 256);
		MessageBox(NULL, buffer, _T("VS"), MB_OK);
	}
	if (pVSBUF != nullptr)
	{
		S_RETURN(m_pd3dDevice->CreateVertexShader((DWORD*)pVSBUF->GetBufferPointer(), pVSBUF->GetBufferSize(), NULL, &m_pMap->m_pVS));
	}

	if (FAILED(D3DX11CompileFromFile(pShaderFile, NULL, NULL, "Water_GS", "gs_5_0", dwShaderFlags, NULL, NULL, &pGSBUF, &pError, NULL)))
	{
		TCHAR buffer[256] = { 0, };
		mbstowcs(buffer, (char*)pError->GetBufferPointer(), 256);
		MessageBox(NULL, buffer, _T("GS"), MB_OK);
	}
	if (pGSBUF != nullptr)
	{
		S_RETURN(m_pd3dDevice->CreateGeometryShader((DWORD*)pGSBUF->GetBufferPointer(), pGSBUF->GetBufferSize(), NULL, &m_pMap->m_pGS));
	}

	if (FAILED(D3DX11CompileFromFile(pShaderFile, NULL, NULL, "Water_PS", "ps_5_0", dwShaderFlags, NULL, NULL, &pPSBUF, &pError, NULL)))
	{
		TCHAR buffer[256] = { 0, };
		mbstowcs(buffer, (char*)pError->GetBufferPointer(), 256);
		MessageBox(NULL, buffer, _T("PS"), MB_OK);
	}
	if (pPSBUF != nullptr)
	{
		S_RETURN(m_pd3dDevice->CreatePixelShader((DWORD*)pPSBUF->GetBufferPointer(), pPSBUF->GetBufferSize(), NULL, &m_pMap->m_pPS));
	}

	D3D11_INPUT_ELEMENT_DESC layout[] = 
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",	  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTURE",  0, DXGI_FORMAT_R32G32_FLOAT,       0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT",  0, DXGI_FORMAT_R32G32_FLOAT,       1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT iSize = sizeof(layout) / sizeof(layout[0]);

	S_RETURN(m_pd3dDevice->CreateInputLayout(layout, iSize, pVSBUF->GetBufferPointer(), pVSBUF->GetBufferSize(), &m_pMap->m_pVertexLayout)); // m_pVertexLayout 제작
	S_RELEASE(pVSBUF);
	S_RELEASE(pPSBUF);
	S_RELEASE(pGSBUF);
	return hr;
}

void BWaterMap::SetWaterMatrix(D3DXMATRIX* World, D3DXMATRIX* View, D3DXMATRIX* Proj)
{
	if (World != NULL)
	{
		m_pMap->m_matWorld = *World;
		m_pMap->m_tBox.vCenter.x = World->_41;
		m_pMap->m_tBox.vCenter.y = World->_42;
		m_pMap->m_tBox.vCenter.z = World->_43;
	}

	// m_matNormal = 접선벡터용
	D3DXMatrixInverse(&m_matNormal, 0, &m_pMap->m_matWorld);

	if (View)
	{
		m_pMap->m_matView = *View;
	}
	if (Proj)
	{
		m_pMap->m_matProj = *Proj;
	}
	D3DXMatrixTranspose(&m_pMap->m_matWorld, &m_pMap->m_matWorld);
	D3DXMatrixTranspose(&m_pMap->m_matView, &m_pMap->m_matView);
	D3DXMatrixTranspose(&m_pMap->m_matProj, &m_pMap->m_matProj);

	m_pMap->m_cbData.m_matWorld = m_pMap->m_matWorld;
	m_pMap->m_cbData.m_matView = m_pMap->m_matView;
	m_pMap->m_cbData.m_matProj = m_pMap->m_matProj;
	m_pMap->m_cbData.f_Time = cosf(timeGetTime()*0.001f) * 0.5f + 0.5f;

	D3DXMatrixTranspose(&m_pMap->m_matWorld, &m_pMap->m_matWorld);
}

bool BWaterMap::ClipRender(ID3D11DeviceContext* pContext)
{
	m_pMap->PreRender(pContext);
	ID3D11Buffer* Buffer[2] = 
}

HRESULT BWaterMap::CreateWaterConstantBuffer()
{
	HRESULT hr;
	// WaterMap ::포그 & 클립플랜 생성 + 맵 랜더링용 모든 상수버퍼 생성
	float w = -30.0f + cosf(g_fDurationTime) * 30.0f;
	m_Fog.g_ClipPlane = D3DXVECTOR4(0.0f, 1.0f, 0.0f, w);
	m_Fog.g_FogColor = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Fog.g_FogInfo.x = FOGMODE_LINEAR;
	m_Fog.g_FogInfo.y = 0.05f;
	m_Fog.g_FogInfo.z = 10.0f;
	m_Fog.g_FogInfo.w = 1000.0f;

	D3D11_BUFFER_DESC cbDesc;
	cbDesc.Usage = D3D11_USAGE_DEFAULT;
	cbDesc.CPUAccessFlags = 0;
	cbDesc.ByteWidth = sizeof(cbFog);
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = (void*)&m_Fog;
	if (FAILED(m_pd3dDevice->CreateBuffer(&cbDesc, &InitData, m_pFogCB.GetAddressOf())))
		return hr; 

	cbDesc.ByteWidth = sizeof(cbTanLight);
	if (FAILED(m_pd3dDevice->CreateBuffer(&cbDesc, NULL, m_pTanLightCB.GetAddressOf())))
		return hr;

	cbDesc.ByteWidth = sizeof(cbLightDefault);
	m_LightDefault.cb_AmbientLightColor = D3DXVECTOR4(0.1f, 0.1f, 0.1f, 1.0f);
	m_LightDefault.cb_DiffuseLightColor = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	m_LightDefault.cb_SpecularLightColor = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 10.0f);
	// cb_SpecularLightColor.w = SpecularPower이다.
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = (void*)&m_LightDefault;
	if (FAILED(m_pd3dDevice->CreateBuffer(&cbDesc, &InitData, m_pLightDefaultCB.GetAddressOf())))
		return hr;

	return hr;
}

// 접선벡터버퍼 계산
bool BWaterMap::UpdateBuffer() 
{
	D3DXVECTOR3 vTangent, vBiNormal, vNormal;
	int iIndex = 0;
	int i0, i1, i2, i3, i4, i5;

	m_vTangentList.resize()

}


BWaterMap::BWaterMap()
{
}

BWaterMap::~BWaterMap()
{
}
