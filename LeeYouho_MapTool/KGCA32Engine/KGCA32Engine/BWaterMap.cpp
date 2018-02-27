#include "BWaterMap.h"

#pragma region �⺻ ���� �Լ�
bool BWaterMap::Frame(D3D11_VIEWPORT OldViewPort, ID3D11DepthStencilView* OldDepthStencilView, ID3D11RenderTargetView* OldRenderTargetView)
{
	BMapTileRender::Frame(m_pCamera);
	WaterFrame(OldViewPort, OldDepthStencilView, OldRenderTargetView);
	return true;
}
bool BWaterMap::Render()
{
	m_pMap->PreRender(m_pImmediateContext);
	m_pImmediateContext->VSSetShader(m_pWaterVS, NULL, 0);
	m_pImmediateContext->PSSetShader(m_pWaterPS, NULL, 0);
	m_pImmediateContext->PSSetShaderResources(0, 1, m_WaterSRV.GetAddressOf());
	m_pImmediateContext->PSSetShaderResources(1, 1, m_WaterBottomSRV.GetAddressOf());
	m_pImmediateContext->PSSetShaderResources(2, 1, m_WaterMapUpsideSRV.GetAddressOf());
	m_pImmediateContext->PSSetShaderResources(3, 1, m_WaterNormalSRV.GetAddressOf());
	m_pImmediateContext->PSSetShaderResources(4, 1, m_WaterHeightSRV.GetAddressOf());

	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pMap->m_pConstantBuffer);
	m_pImmediateContext->PSSetConstantBuffers(0, 1, &m_pMap->m_pConstantBuffer);
	m_pImmediateContext->VSSetConstantBuffers(1, 1, m_pFogCB.GetAddressOf());
	m_pImmediateContext->PSSetConstantBuffers(1, 1, m_pFogCB.GetAddressOf());
	m_pImmediateContext->VSSetConstantBuffers(2, 1, m_pTanLightCB.GetAddressOf());
	m_pImmediateContext->PSSetConstantBuffers(2, 1, m_pTanLightCB.GetAddressOf());
	m_pImmediateContext->VSSetConstantBuffers(3, 1, m_pLightDefaultCB.GetAddressOf());
	m_pImmediateContext->PSSetConstantBuffers(3, 1, m_pLightDefaultCB.GetAddressOf());
	m_pImmediateContext->OMSetBlendState(BDxState::g_pAlphaBlend, 0, -1);
	PostRender(m_pImmediateContext);
	return true;
}
bool BWaterMap::PostRender(ID3D11DeviceContext* pContext)
{
	ID3D11Buffer* Buffer[2];
	UINT stride[2] = { sizeof(PNCT_Vertex), sizeof(D3DXVECTOR3) };		 // ���ؽ� �ϳ� ��ü�� ũ��
	UINT offset[2] = { 0, 0 };						 // ���� ������ġ
	for (int iNode = 0; iNode < m_DrawPatchNodeList.size(); iNode++)
	{
		BNode* pNode = m_DrawPatchNodeList[iNode];
		GetLodType(pNode);
		Buffer[0] = pNode->m_pVertexBuffer.Get();
		Buffer[1] = pNode->m_pTangentBuffer.Get();
		pContext->IASetVertexBuffers(0, 2, Buffer, stride, offset);
		pContext->IASetIndexBuffer(m_pdwLodIndexArray[pNode->m_dwLodLevel]->IndexBufferList[pNode->m_dwLodType].Get(), DXGI_FORMAT_R32_UINT, 0);
		pContext->UpdateSubresource(m_pMap->m_pConstantBuffer, 0, NULL, &m_pMap->m_cbData, 0, 0);
		pContext->DrawIndexed(m_pdwLodIndexArray[pNode->m_dwLodLevel]->IndexList[pNode->m_dwLodType].size(), 0, 0);
	}
	return true;
}
#pragma endregion

#pragma region ���͸� �����Լ�
/*������ # ���͸� �����Լ�*/
bool BWaterMap::Build(BMap* pMap, DWORD dwWidth, DWORD dwHeight, ID3D11DeviceContext* pContext, TCHAR* strWaterTex, TCHAR* strNormalTex, TCHAR* strHeightTex)
{
	bHeightChange = false;
	bRequestCreate = false;

	m_strWaterTex = strWaterTex;
	m_strWaterNormalTex = strNormalTex;
	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;
	m_pImmediateContext = pContext;
	//m_pMap�� ���� ���� �� ���
	//�޾ƿ� pMap�� �ؽ��ķ� �̿��ϱ� ���� ����
	m_pMap = new BNMap;
	m_pMap->SetMapDesc(pMap->m_MapDesc);
	m_pMap->Create(m_pd3dDevice, m_pImmediateContext, NULL, 0, 0, L"../../data/shader/MapShader.hlsl"); // BN�� ClipPS�� ���� ������ ������ ���̴��� �����Ѵ�.
	m_pMap->UpdateNormal();
	CreateWaterMapInputLayout(L"../../data/shader/WaterMapShader.hlsl");
	m_pWaterTexMap = pMap;

	// Height �÷� ����
	m_HeightMap.Create(m_pd3dDevice, L"../../data/shader/TestShader.hlsl", pMap->m_MapDesc.iNumCols, pMap->m_MapDesc.iNumRows);
	m_HeightMap.Set(m_pd3dDevice, 200, 0, m_pMap->m_iNumRows, m_pMap->m_iNumRows);
	if (pMap->m_tBox.vMax.y != 0)
	{
		m_HeightMap.m_pMiniMapRT->DrawHeightTex(m_pd3dDevice, m_pImmediateContext, pMap);
	}
	else
	{
		m_HeightMap.m_pMiniMapRT->DrawQuadTreeHeightTex(m_pd3dDevice, m_pImmediateContext, m_SelectedMap);
	}
	m_WaterHeightSRV = m_HeightMap.m_pMiniMapRT->m_pSRV;

	// �ؽ��� ����
	TextureLoad(strWaterTex, strNormalTex, strHeightTex);

	// ������� ����
	CreateWaterConstantBuffer();

	m_iNumFace = (dwWidth - 1) *(dwHeight - 1) * 2;
	m_bUsedIndexList = true;

	m_pRootNode = CreateNode(NULL, 0, dwWidth - 1, dwWidth * (dwHeight - 1), dwWidth * dwHeight - 1);

	InitLevelOrder(m_pRootNode, m_iMaxDepthLimit); // LOD�� �Լ� LOD ���� resize
	m_pIndexBuffer.Attach(CreateIndexBuffer(m_pd3dDevice, NULL, m_iNumFace * 3, sizeof(DWORD)));

	if (BuildTree(m_pRootNode))
	{
		for (int iNode = 0; iNode < 4; iNode++)
		{
			SetNeighborNode(m_pRootNode->m_pChild[iNode]);
		}
		SetLOD(m_dwWidth, m_iMaxDepth);

		// LOD�� �ε��� ���� �Լ�
		ComputeStaticLodIndex(m_iNumCell);
		return true;
	}
	return false;
}
/*���͸� ���� ���̾ƿ� �����Լ� (Build ���� ȣ��)*/
HRESULT BWaterMap::CreateWaterMapInputLayout(const TCHAR* pShaderFile) // WaterMapShader�� �̿�
{
	HRESULT hr;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS; // ���̴� ����� ���
	ID3DBlob* pVSBUF = NULL;
	ID3DBlob* pPSBUF = NULL;
	ID3DBlob* pGSBUF = NULL;

	ID3DBlob* pError = NULL; // �̰��� �������ν� ������ �� ������ �޾� �� �� �ִ�.
							 /* vs_5_0 == direct11 �� 5.0�� ��� * pVSBuf ����� ��� */

	if (FAILED(D3DX11CompileFromFile(pShaderFile, NULL, NULL, "VS", "vs_5_0", dwShaderFlags, NULL, NULL, &pVSBUF, &pError, NULL)))
	{
		TCHAR buffer[256] = { 0, };
		mbstowcs(buffer, (char*)pError->GetBufferPointer(), 256);
		MessageBox(NULL, buffer, _T("VS"), MB_OK);
	}
	if (FAILED(D3DX11CompileFromFile(pShaderFile, NULL, NULL, "GS", "gs_5_0", dwShaderFlags, NULL, NULL, &pGSBUF, &pError, NULL)))
	{
		TCHAR buffer[256] = { 0, };
		mbstowcs(buffer, (char*)pError->GetBufferPointer(), 256);
		//MessageBox(NULL, buffer, _T("GS"), MB_OK);
	}

	if (FAILED(D3DX11CompileFromFile(pShaderFile, NULL, NULL, "PS", "ps_5_0", dwShaderFlags, NULL, NULL, &pPSBUF, &pError, NULL)))
	{
		TCHAR buffer[256] = { 0, };
		mbstowcs(buffer, (char*)pError->GetBufferPointer(), 256);
		MessageBox(NULL, buffer, _T("PS"), MB_OK);
	}
	if (pVSBUF != nullptr)
	{
		S_RETURN(m_pd3dDevice->CreateVertexShader((DWORD*)pVSBUF->GetBufferPointer(), pVSBUF->GetBufferSize(), NULL, &m_pWaterVS));
	}
	if (pPSBUF != nullptr)
	{
		S_RETURN(m_pd3dDevice->CreatePixelShader((DWORD*)pPSBUF->GetBufferPointer(), pPSBUF->GetBufferSize(), NULL, &m_pWaterPS));
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

	S_RETURN(m_pd3dDevice->CreateInputLayout(layout, iSize, pVSBUF->GetBufferPointer(), pVSBUF->GetBufferSize(), &m_pMap->m_pVertexLayout)); // m_pVertexLayout ����
	S_RELEASE(pVSBUF);
	S_RELEASE(pPSBUF);
	S_RELEASE(pGSBUF);
	return hr;
}
/*������� �����Լ� (Build ���� ȣ��)*/
HRESULT BWaterMap::CreateWaterConstantBuffer()
{
	HRESULT hr = S_OK;
	// WaterMap ::���� & Ŭ���÷� ���� + �� �������� ��� ������� ����
	m_Fog.g_ClipPlane = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 5.0f);
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

	cbDesc.ByteWidth = sizeof(cbLightDefault);
	m_LightDefault.cb_AmbientLightColor = D3DXVECTOR4(0.1f, 0.1f, 0.1f, 1.0f);
	m_LightDefault.cb_DiffuseLightColor = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	m_LightDefault.cb_SpecularLightColor = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 10.0f);
	// cb_SpecularLightColor.w = SpecularPower�̴�.
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = (void*)&m_LightDefault;
	if (FAILED(m_pd3dDevice->CreateBuffer(&cbDesc, &InitData, m_pLightDefaultCB.GetAddressOf())))
		return hr;

	cbDesc.ByteWidth = sizeof(cbTanLight);
	ZeroMemory(&m_TanLight, sizeof(cbTanLight));
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = (void*)&m_TanLight;
	if (FAILED(m_pd3dDevice->CreateBuffer(&cbDesc, &InitData, m_pTanLightCB.GetAddressOf())))
		return hr;

	return hr;
}
/*������ # ���͸� �ٿ���ڽ� �����Լ�*/
void BWaterMap::ComputeBoundingBox(BNode* pNode)
{
	D3DXVECTOR2 vHeight = GetHeightFromNode(pNode->m_CornerIndex[0], pNode->m_CornerIndex[1], pNode->m_CornerIndex[2], pNode->m_CornerIndex[3]);

	pNode->m_vCorner[0] = m_pMap->m_VertexList[pNode->m_CornerIndex[0]].Position;
	pNode->m_vCorner[1] = m_pMap->m_VertexList[pNode->m_CornerIndex[1]].Position;
	pNode->m_vCorner[2] = m_pMap->m_VertexList[pNode->m_CornerIndex[2]].Position;
	pNode->m_vCorner[3] = m_pMap->m_VertexList[pNode->m_CornerIndex[3]].Position;

	pNode->m_tBox.vMax = m_pMap->m_VertexList[pNode->m_CornerIndex[1]].Position;
	pNode->m_tBox.vMin = m_pMap->m_VertexList[pNode->m_CornerIndex[2]].Position;
	pNode->m_tBox.vMax.y = m_fWaterHeight;
	pNode->m_tBox.vMin.y = 0;

	pNode->m_tBox.vCenter = (pNode->m_tBox.vMax + pNode->m_tBox.vMin) / 2.0f;
	pNode->m_tBox.fExtent[0] = pNode->m_tBox.vMax.x - pNode->m_tBox.vCenter.x;
	pNode->m_tBox.fExtent[1] = pNode->m_tBox.vMax.y - pNode->m_tBox.vCenter.y + 4.0f + m_fWaterWave;
	pNode->m_tBox.fExtent[2] = pNode->m_tBox.vMax.z - pNode->m_tBox.vCenter.z;
}
#pragma endregion

#pragma region ������ ���� ȣ���Լ�
/*���͸� ���� ������ �Լ�(Frame ���� ȣ��)*/
bool BWaterMap::WaterFrame(D3D11_VIEWPORT OldViewPort, ID3D11DepthStencilView* OldDepthStencilView, ID3D11RenderTargetView* OldRenderTargetView)
{
	if (bHeightChange) // ���̰� ����Ǿ��� ������ ��û�Ǿ��ٸ�
	{
		m_HeightMap.m_pMiniMapRT->DrawQuadTreeHeightTex(m_pd3dDevice, m_pImmediateContext, m_SelectedMap);
		bHeightChange = false;
	}
	m_Fog.g_vCameraPos.x = m_pCamera->m_vPos.x;
	m_Fog.g_vCameraPos.y = m_pCamera->m_vPos.y;
	m_Fog.g_vCameraPos.z = m_pCamera->m_vPos.z;
	m_Fog.g_vCameraPos.w = 1.0f;
	m_Fog.g_FogColor = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);

	m_pImmediateContext->OMSetBlendState(BDxState::g_pAlphaBlend, 0, 0xffffffff);
	float w = -m_fWaterHeight + cosf(g_fDurationTime) * m_fWaterWave * 0.2f;
	D3DXMATRIX matReflectView;
	D3DXMatrixIdentity(&matReflectView);
	D3DXMatrixReflect_Fixed(&matReflectView, &D3DXPLANE(0.0f, 1.0f, 0.0f, w));
	D3DXMATRIX matView = m_pCamera->m_matView;
	D3DXMatrixMultiply(&matReflectView, &matReflectView, &matView);

	// ���
	m_pImmediateContext->RSSetState(BDxState::g_pRSBackCullSolid);
	D3DXVECTOR4 m_ClipPlaneFront = D3DXVECTOR4(0.0f, 1.0f, 0.0f, w);
	m_Fog.g_ClipPlane = m_ClipPlaneFront;
	m_pImmediateContext->UpdateSubresource(m_pFogCB.Get(), 0, NULL, &m_Fog, 0, 0);
	DrawWaterRT(0, NULL, &m_pCamera->m_matView, &m_pCamera->m_matProj, OldViewPort, OldDepthStencilView, OldRenderTargetView);
	// �ϴ�
	D3DXVECTOR4 m_ClipPlaneBack = D3DXVECTOR4(0.0f, -1.0f, 0.0f, -w);
	m_Fog.g_ClipPlane = m_ClipPlaneBack;
	m_pImmediateContext->UpdateSubresource(m_pFogCB.Get(), 0, NULL, &m_Fog, 0, 0);
	DrawWaterRT(1, NULL, &m_pCamera->m_matView, &m_pCamera->m_matProj, OldViewPort, OldDepthStencilView, OldRenderTargetView);
	// �ݻ�
	m_pImmediateContext->RSSetState(BDxState::g_pRSFrontCullSolid);
	D3DXVECTOR4 m_ClipPlaneReflect = D3DXVECTOR4(0.0f, 1.0f, 0.0f, w);
	m_Fog.g_ClipPlane = m_ClipPlaneReflect;
	m_pImmediateContext->UpdateSubresource(m_pFogCB.Get(), 0, NULL, &m_Fog, 0, 0);
	DrawWaterRT(2, NULL, &matReflectView, &m_pCamera->m_matProj, OldViewPort, OldDepthStencilView, OldRenderTargetView);

	return false;
}
/*�ݻ� ����� �����Լ� (WaterFrame ���� ȣ��)*/
D3DXMATRIX* BWaterMap::D3DXMatrixReflect_Fixed(__inout D3DXMATRIX *pOut, __in const D3DXPLANE *pPlane)
{
	D3DXPLANE P;
	D3DXPlaneNormalize(&P, pPlane);

	*pOut = D3DXMATRIX
	(
		-2.0f * P.a * P.a + 1.0f, -2.0f * P.b * P.a, -2.0f * P.c * P.a, 0.0f,
		-2.0f * P.a * P.b, -2.0f * P.b * P.b + 1.0f, -2.0f * P.c * P.b, 0.0f,
		-2.0f * P.a * P.c, -2.0f * P.b * P.c, -2.0f * P.c * P.c + 1.0f, 0.0f,
		-2.0f * P.a * P.d, -2.0f * P.b * P.d, -2.0f * P.c * P.d, 1.0f
	);

	return pOut;
}
/*�ݻ�, ���� ���ҽ� ���� �Լ� (WaterFrame ���� ȣ��)*/
void BWaterMap::DrawWaterRT(int iMap, D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj, D3D11_VIEWPORT OldViewPort, ID3D11DepthStencilView* OldDepthStencilView, ID3D11RenderTargetView* OldRenderTargetView)
{
	ID3D11ShaderResourceView *const pSRV[8] = { NULL, };
	m_pImmediateContext->PSSetShaderResources(0, 8, pSRV);
	m_pImmediateContext->OMSetBlendState(BDxState::g_pAlphaBlend, 0, 0xffffffff);
	m_pImmediateContext->OMSetDepthStencilState(BDxState::g_pDSSDepthEnable, 0x01);

	if (m_MiniMap[iMap].BeginRender(m_pImmediateContext, OldViewPort, OldDepthStencilView, OldRenderTargetView, D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f)))
	{
		m_pWaterReflectMap->m_pMap->SetMatrix(pWorld, pView, pProj);
		m_pWaterReflectMap->Render(m_pImmediateContext);
		m_MiniMap[iMap].AfterRender(m_pImmediateContext);
	}
}
#pragma endregion

#pragma region �ؽ��� �����Լ�
/*������ # �ؽ��� �����Լ�*/
HRESULT BWaterMap::TextureLoad(const TCHAR* strWaterTex, const TCHAR* strNormalTex, const TCHAR* strHeightTex)
{
	HRESULT hr = S_OK;
	if (strWaterTex)
	{
		hr = SetWaterTexture(strWaterTex);
	}
	if (strNormalTex)
	{
		hr = SetWaterNormalTexture(strNormalTex);
	}
	if (strHeightTex)
	{
		hr = SetHeightTexture(strHeightTex);
	}
	return hr;
}
/*���� �ؽ��� �����Լ�*/
HRESULT BWaterMap::SetWaterTexture(const TCHAR* strFilePath)
{
	HRESULT hr = S_OK;
	FAILED(hr = D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice, strFilePath, NULL, NULL, &m_WaterSRV, NULL));
	return hr;
}
/*���� �븻�ؽ��� �����Լ�*/
HRESULT BWaterMap::SetWaterNormalTexture(const TCHAR* strFilePath)
{
	HRESULT hr = S_OK;
	FAILED(hr = D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice, strFilePath, NULL, NULL, &m_WaterNormalSRV, NULL));
	return hr;
}
/*���� �����ؽ��� �����Լ�*/
HRESULT BWaterMap::SetHeightTexture(const TCHAR* strFilePath)
{
	HRESULT hr = S_OK;
	FAILED(hr = D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice, strFilePath, NULL, NULL, &m_WaterHeightSRV, NULL));
	return hr;
}
#pragma endregion

#pragma region ���͸� ���ΰ� �����Լ�
/*���� ���̰� �����Լ� (���� ȣ�� �ʼ� / �Ǽ� ������ ����)*/
bool BWaterMap::SetMapHeight(float Height)
{
	m_fWaterHeight = Height;
	m_pMap->m_matWorld._42 = m_fWaterHeight;
	return true;
}
/*���� ���̰� �����Լ� (���� ȣ�� �ʼ� / ������� ������ ����)*/
bool BWaterMap::SetMapHeight()
{
	m_pMap->m_matWorld._42 = m_fWaterHeight;
	return true;
}
/*���͸� �⺻ ��� �����Լ�(���� ȣ�� �ʼ� / ����̽� ī�޶� ��)*/
void BWaterMap::Update(ID3D11Device* pd3dDevice, BCamera* pCamera, BMapTileRender* SelectedMap)
{
	m_pd3dDevice = pd3dDevice;
	m_pCamera = pCamera;
	m_SelectedMap = SelectedMap;
}
/*���͸� ��� �����Լ�*/
void BWaterMap::SetWaterMatrix(D3DXMATRIX* World, D3DXMATRIX* View, D3DXMATRIX* Proj)
{
	if (World != NULL)
	{
		m_pMap->m_matWorld = *World;
		m_pRootNode->m_tBox.vCenter.x = World->_41;
		m_pRootNode->m_tBox.vCenter.y = World->_42;
		m_pRootNode->m_tBox.vCenter.z = World->_43;
	}

	// m_matNormal = �������Ϳ�
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
	m_pMap->m_cbData.f_Y = m_SelectedMap->m_pRootNode->m_tBox.vMax.y - m_SelectedMap->m_pRootNode->m_tBox.vMin.y;
	m_pMap->m_cbData.f_Z = m_SelectedMap->m_pRootNode->m_tBox.vMin.y;

	D3DXMatrixTranspose(&m_pMap->m_matWorld, &m_pMap->m_matWorld);
}
#pragma endregion

BWaterMap::BWaterMap()
{
}

BWaterMap::~BWaterMap()
{
}