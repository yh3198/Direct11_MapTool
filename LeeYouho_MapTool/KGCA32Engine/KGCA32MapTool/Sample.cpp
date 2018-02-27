#include "Sample.h"



#pragma region �⺻ �����Լ�
bool Sample::Init()
{
	/*�⺻ ���� ���� ���� ����*/
	m_VCtrlState = 0;
	m_fHeightValue = 1.0f;
	bParallel = false;
	m_fRadius = 3.0f;
	m_fSpreatingRadius = 3.0f;
	m_VSpreatState = vSpreatNone;
	m_VObjSpreatState = vSpreatNone;
	m_Exporter.SetExporter(m_pImmediateContext, L"MapExportFile.txt");

	/*�׶���Ʈ�� ���� ����*/
	ColoringCnt = 0;
	m_ModeGradient = false;

	/*���͸� ����Ʈ ���� ����*/
	m_vInitLightPos = D3DXVECTOR3(300.0f, 300.0f, 0.0f);

	/*�� ���� ��� ���� ����*/
	m_MapSettingRenderMode = false;
	m_MapSetting.Init(m_pMainCamera, m_pd3dDevice, m_pImmediateContext);

	/*�� ������Ʈ ���� ���� ����*/
	m_MapObjTotalCnt = 0;
	ObjSettingMode = false;
	ObjSelectingMode = false;
	ObjCreateOne = false;
	ObjMovingMode = false;
	ObjRenderMode = false;
	BoxRenderMode = false;
	bMapDebug = false;

	/*�� ������Ʈ ������ ����ȭ �۾�*/
	m_RenderSettingASE.m_pd3dDevice = m_pd3dDevice;
	m_RenderSettingASE.LoadShaderAndInputLayout(L"../../data/shader/AseShader.hlsl");

	/*�� ������ ����*/
	m_Shadow.Init(m_pd3dDevice);
	return true;
}
bool Sample::Frame()
{
	/*�� ������Ʈ*/
	if (m_vMapQuadTree.size() > 0)
	{
		if (m_vMapQuadTree[0]->m_iTotalMapObj != m_MapObjTotalCnt)
		{
			m_vMapQuadTree[0]->ClearObject(m_vMapQuadTree[0]->m_pRootNode);
			vector<InstancingInfo> TempMapInfo;
			int TempTotalsize = 0;
			for (int iTObj = 0; iTObj < m_MapObjMgr.size(); iTObj++)
			{
				TempTotalsize += m_MapObjMgr[iTObj]->m_InstanceInfo.size();
			}
			TempMapInfo.reserve(TempTotalsize);
			for (int iTObj = 0; iTObj < m_MapObjMgr.size(); iTObj++)
			{
				for (int iInfoObj = 0; iInfoObj < m_MapObjMgr[iTObj]->m_InstanceInfo.size(); iInfoObj++)
				{
					TempMapInfo.push_back(m_MapObjMgr[iTObj]->m_InstanceInfo[iInfoObj]);
				}
			}
			m_vMapQuadTree[0]->AddMapObject(m_vMapQuadTree[0]->m_pRootNode, TempMapInfo);
		}
	}

	/*�� ����*/
	if (m_MapSetting.BMapSettingMode)
	{
		if (m_vMapQuadTree.size())
		{
			m_MapSetting.SettingMap(m_vMapQuadTree[0]);
		}
	}
	if (m_MapSetting.m_SelectedMap && m_MapSetting.BApply)
	{
		m_MapSetting.MapInfoSetting();
	}

	/*�� ������*/
	for (int iMap = 0; iMap < m_vMapQuadTree.size(); iMap++)
	{
		m_vMapQuadTree[iMap]->Frame(m_pMainCamera);
	}	
	for (int iMap = 0; iMap < m_vWaterQuadTree.size(); iMap++)
	{
		m_vWaterQuadTree[iMap]->Frame(m_ViewPort, m_pDepthStencilView, m_pRenderTargetView);
	}

	/*�� ���� ���*/
	if (m_VCtrlState && I_Input.m_MFCDIMouseState[0] == KEY_PUSH)
	{
		MapHeightControl();
		if (m_vWaterQuadTree.size())
		{
			m_vWaterQuadTree[0]->bHeightChange = true;
		}
	}

	/*����Ʈ ���*/
	// ����Ʈ ��� & ��� ������ ����
	float t = g_fDurationTime * D3DX_PI;
	D3DXMATRIX mTranslate, mRotation;
	D3DXMatrixTranslation(&mTranslate, m_vInitLightPos.x, m_vInitLightPos.y, m_vInitLightPos.z);
	D3DXMatrixRotationY(&mRotation, t*0.02f);
	D3DXMatrixMultiply(&m_mLightWorld, &mTranslate, &mRotation);
	m_LightPos.x = m_mLightWorld._41;
	m_LightPos.y = m_mLightWorld._42;
	m_LightPos.z = m_mLightWorld._43;
	D3DXVec3Normalize(&m_vDirection, &m_LightPos);
	m_vDirection *= -1.0f;
	if (m_vWaterQuadTree.size())
	{
		m_vWaterQuadTree[0]->m_TanLight.matNormal = m_vWaterQuadTree[0]->m_matNormal;
		m_vWaterQuadTree[0]->m_TanLight.vLightPos = D3DXVECTOR4(m_vDirection, 0.0f);
		m_vWaterQuadTree[0]->m_TanLight.vEye = D3DXVECTOR4(m_pMainCamera->m_vPos, 0.0f);
		m_vWaterQuadTree[0]->m_TanLight.vEyeDir = D3DXVECTOR4(m_pMainCamera->m_vLook, 0.0f);
		m_pImmediateContext->UpdateSubresource(m_vWaterQuadTree[0]->m_pTanLightCB.Get(), 0, NULL, &m_vWaterQuadTree[0]->m_TanLight, 0, 0);
	}

	/*�� ������Ʈ*/
	// ������Ʈ ����
	if (ObjSettingMode && ObjCreateOne && I_Input.m_MFCDIMouseState[0] == KEY_PUSH && m_CreateObjType.size())
	{
		if (GetMapObjectPosition())
		{
			m_MapObjTotalCnt++;
			ObjCreateOne = false;
		}
	}
	// ������Ʈ ����
	if (ObjSelectingMode && I_Input.m_MFCDIMouseState[0] == KEY_PUSH)
	{
		SelectMapObj();
	}
	// ������Ʈ ����
	if (I_Input.KeyCheck(DIK_DELETE) == KEY_PUSH)
	{
		ClearSelected();
	}
	// ������Ʈ �̵�
	if (ObjMovingMode && I_Input.m_MFCDIMouseState[0] == KEY_PUSH)
	{
		GetMapObjectMovePosition();
	}

	/*��� ���� ����*/
	// ���ø��
	if (I_Input.KeyCheck(DIK_C))
	{
		ObjSelectingMode = true;
		ObjSettingMode = false;
		ObjMovingMode = false;
	}
	// �̵����
	if (I_Input.KeyCheck(DIK_V))
	{
		ObjSelectingMode = false;
		ObjSettingMode = false;
		ObjMovingMode = true;
	}
	// �������
	if (I_Input.KeyCheck(DIK_B))
	{
		ObjSettingMode = true;
		ObjMovingMode = false;
	}

	// ������Ʈ MinMax �ڽ� �������
	if (I_Input.KeyCheck(DIK_F11) == KEY_UP)
	{
		BoxRenderMode = !BoxRenderMode;
	}
	// ������Ʈ �������
	if (I_Input.KeyCheck(DIK_F12) == KEY_UP)
	{
		ObjRenderMode = !ObjRenderMode;
	}
	// ���͸�, ������ �ݻ�, ����, ���� ����� ���
	if (I_Input.KeyCheck(DIK_F10) == KEY_UP)
	{
		bMapDebug = !bMapDebug;
	}

	/*������Ʈ ���� ������ (�̻��)*/
	//if (m_vMapQuadTree.size() > 0)
	//{
	//	for (int iDraw = 0; iDraw < m_DrawObjList.size(); iDraw++)
	//	{
	//		m_DrawObjList[iDraw].clear();
	//	}
	//	for (int iObj = 0; iObj < m_vMapQuadTree[0]->m_DrawMapObjList.size(); iObj++)
	//	{
	//		m_DrawObjList[m_vMapQuadTree[0]->m_DrawMapObjList[iObj].m_iObjType].push_back(m_vMapQuadTree[0]->m_DrawMapObjList[iObj].m_iObjNum);
	//	}
	//}

	return true;
}
bool Sample::Render()
{
	//====================================================================================//

	/*�������� ����*/
	// �� ��������
	if (m_VSpreatState && I_Input.m_MFCDIMouseState[0] == KEY_PUSH && m_ModeGradient == false)
	{
		MapSpreating();
	}
	// �� �׶���Ʈ ��������
	if (m_VSpreatState && I_Input.m_MFCDIMouseState[0] == KEY_PUSH && m_ModeGradient == true && ColoringCnt < 1)
	{
		MapSpreating();
		ColoringCnt++;
	}
	// �� �������� ��������
	if (m_VObjSpreatState && I_Input.m_MFCDIMouseState[0] == KEY_PUSH)
	{
		MapObjInfoSpreating();
	}
	//====================================================================================//
	
	//====================================================================================//
	/*������ ����*/
	ID3D11ShaderResourceView *const pSRVs[16] = { 0, };
	m_pImmediateContext->PSSetShaderResources(0, 16, pSRVs);
	if (m_Shadow.m_ShadowPlane.BeginRender(m_pImmediateContext, m_ViewPort, m_pDepthStencilView, m_pRenderTargetView, D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f)))
	{
		RenderShadow();
		m_Shadow.m_ShadowPlane.AfterRender(m_pImmediateContext);
	}
	//====================================================================================//

	//====================================================================================//
	/*������ ��� ����*/
	// ���� �ؽ��� ������
	if (m_MapSettingRenderMode)
	{
		for (int iMap = 0; iMap < m_vMapQuadTree.size(); iMap++)
		{
			m_vMapQuadTree[iMap]->m_pMap->SetMatrix(&m_vMapQuadTree[iMap]->m_pMap->m_matWorld, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
			m_vMapQuadTree[iMap]->AlphaRender(m_pImmediateContext);
		}
	}
	// �Ϲ� ������ (��)
	else
	{
		m_pImmediateContext->PSSetSamplers(1, 1, &BDxState::g_pSSClampLinear);
		m_pImmediateContext->PSSetSamplers(2, 1, &BDxState::g_pSSShadowMap);
		m_pImmediateContext->RSSetState(m_pRasterizerState);
		m_Shadow.m_matWVPT = m_Shadow.m_matShadowView * m_Shadow.m_matShadowProj * m_Shadow.m_matTexture;
		D3DXMatrixTranspose(&m_Shadow.m_cbShadow.g_matShadow, &m_Shadow.m_matWVPT);
		m_Shadow.m_cbShadow.g_ShadowID = 0; // ID�ʽ����츦 �����ֱ� ���� ���� ( ������ �ʿ� ���� )
		m_Shadow.m_cbShadow.g_iNumKernel = 3;
		m_pImmediateContext->UpdateSubresource(m_Shadow.m_pShadowCB, 0, NULL, &m_Shadow.m_cbShadow, 0, 0);
		m_pImmediateContext->VSSetConstantBuffers(1, 1, &m_Shadow.m_pShadowCB);
		m_pImmediateContext->PSSetConstantBuffers(1, 1, &m_Shadow.m_pShadowCB);
		m_pImmediateContext->PSSetShaderResources(6, 1, m_Shadow.m_ShadowPlane.m_pMiniMapRT->m_pDsvSRV.GetAddressOf());
		for (int iMap = 0; iMap < m_vMapQuadTree.size(); iMap++)
		{
			m_vMapQuadTree[iMap]->m_pMap->SetMatrix(&m_vMapQuadTree[iMap]->m_pMap->m_matWorld, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
			m_vMapQuadTree[iMap]->m_pMap->PreRender(m_pImmediateContext);
			// �����ؽ��Ŀ� ���� �̹��� ���
			if (m_vMapQuadTree[iMap]->m_pSplattingSRV.size())
			{
				for (int iTex = 0; iTex < 4; iTex++)
				{
					m_pImmediateContext->PSSetShaderResources((iTex + 2), 1, m_vMapQuadTree[iMap]->m_pSplattingSRV[iTex].GetAddressOf()); // �ؽ��� ����
				}
			}
			// �����ؽ��� ���
			if (m_vMapQuadTree[iMap]->SpreatingSRV.Get())
			{
				m_pImmediateContext->PSSetShaderResources(1, 1, m_vMapQuadTree[iMap]->SpreatingSRV.GetAddressOf()); // �ؽ��� ����
			}
			m_vMapQuadTree[iMap]->PostRender(m_pImmediateContext);
			//m_vMapQuadTree[iMap]->Render(m_pImmediateContext);
		}
	}

	// ���͸� ������
	for (int iWaterMap = 0; iWaterMap < m_vWaterQuadTree.size(); iWaterMap++)
	{
		// ������ ���� ��ȭ
		float w = -m_vWaterQuadTree[0]->m_fWaterHeight + cosf(g_fDurationTime) * m_vWaterQuadTree[0]->m_fWaterWave * 0.2f;
		D3DXMATRIX matWaterWorld;
		D3DXMatrixIdentity(&matWaterWorld);
		matWaterWorld._42 = -w;

		m_vWaterQuadTree[iWaterMap]->m_Fog.g_ClipPlane = D3DXVECTOR4(0.0f, 1.0f, 0.0f, m_vWaterQuadTree[iWaterMap]->m_fWaterHeight);
		m_vWaterQuadTree[iWaterMap]->m_WaterBottomSRV = m_vWaterQuadTree[iWaterMap]->m_MiniMap[1].m_pMiniMapRT->m_pSRV;
		m_vWaterQuadTree[iWaterMap]->m_WaterMapUpsideSRV = m_vWaterQuadTree[iWaterMap]->m_MiniMap[2].m_pMiniMapRT->m_pSRV;
		m_pImmediateContext->UpdateSubresource(m_vWaterQuadTree[iWaterMap]->m_pFogCB.Get(), 0, NULL, &m_vWaterQuadTree[iWaterMap]->m_Fog, 0, 0);
		m_vWaterQuadTree[iWaterMap]->SetWaterMatrix(&matWaterWorld, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
		m_vWaterQuadTree[iWaterMap]->Render();

		/*Ȯ�ο� ����*/
		if (bMapDebug)
		{
			m_vWaterQuadTree[iWaterMap]->m_MiniMap[0].Render(m_pImmediateContext, m_vWaterQuadTree[iWaterMap]->m_MiniMap[0].m_pMiniMapRT->m_pSRV);
			m_pImmediateContext->RSSetViewports(1, &m_ViewPort);
			m_vWaterQuadTree[iWaterMap]->m_MiniMap[1].Render(m_pImmediateContext, m_vWaterQuadTree[iWaterMap]->m_MiniMap[1].m_pMiniMapRT->m_pSRV);
			m_pImmediateContext->RSSetViewports(1, &m_ViewPort);
			m_vWaterQuadTree[iWaterMap]->m_MiniMap[2].Render(m_pImmediateContext, m_vWaterQuadTree[iWaterMap]->m_MiniMap[2].m_pMiniMapRT->m_pSRV);
			m_pImmediateContext->RSSetViewports(1, &m_ViewPort);
		}
	}

	// �� ������Ʈ ������
	if (m_MapObjMgr.size() && ObjRenderMode)
	{
		m_pImmediateContext->PSSetSamplers(1, 1, &BDxState::g_pSSClampLinear);
		m_pImmediateContext->PSSetSamplers(2, 1, &BDxState::g_pSSShadowMap);
		m_pImmediateContext->RSSetState(m_pRasterizerState); //m_pRasterizerState
		m_Shadow.m_matWVPT = m_Shadow.m_matShadowView * m_Shadow.m_matShadowProj * m_Shadow.m_matTexture;
		D3DXMatrixTranspose(&m_Shadow.m_cbShadow.g_matShadow, &m_Shadow.m_matWVPT);
		m_Shadow.m_cbShadow.g_ShadowID = 0; // ID�ʽ����츦 �����ֱ� ���� ���� ( ������ �ʿ� ���� )
		m_Shadow.m_cbShadow.g_iNumKernel = 3;
		m_pImmediateContext->UpdateSubresource(m_Shadow.m_pShadowCB, 0, NULL, &m_Shadow.m_cbShadow, 0, 0);
		m_pImmediateContext->VSSetConstantBuffers(1, 1, &m_Shadow.m_pShadowCB);
		m_pImmediateContext->PSSetConstantBuffers(1, 1, &m_Shadow.m_pShadowCB);
		m_pImmediateContext->PSSetShaderResources(1, 1, m_Shadow.m_ShadowPlane.m_pMiniMapRT->m_pDsvSRV.GetAddressOf());
		for (int iMapObj = 0; iMapObj < m_MapObjMgr.size(); iMapObj++)
		{
			if (m_MapObjMgr[iMapObj]->m_InstanceInfo.size())
			{
				m_MapObjMgr[iMapObj]->SetCamera(m_pMainCamera->m_matView, m_pMainCamera->m_matProj);
				m_MapObjMgr[iMapObj]->SettingCameraForNormal(m_pMainCamera->m_vPos, m_pMainCamera->m_vLook);
				m_pImmediateContext->VSSetConstantBuffers(2, 1, &m_MapObjMgr[iMapObj]->m_pNormalCB);
				/*����ȭ ����*/
				m_pImmediateContext->VSSetShader(m_RenderSettingASE.m_pVS, NULL, 0);
				m_pImmediateContext->PSSetShader(m_RenderSettingASE.m_pPS, NULL, 0);
				m_pImmediateContext->IASetInputLayout(m_RenderSettingASE.m_pVertexLayout);
				m_MapObjMgr[iMapObj]->Render(m_pImmediateContext, m_MapObjMgr[iMapObj]->m_InstanceInfo.size());
				if (BoxRenderMode)
				{
					m_MapObjMgr[iMapObj]->RenderBoundingBoxLine(m_pImmediateContext, m_pMainCamera->m_matView, m_pMainCamera->m_matProj);
				}
			}
		}
	}

	// ����� ���� ������
	if (bMapDebug)
	{
		m_Shadow.m_ShadowPlane.Render(m_pImmediateContext, m_Shadow.m_ShadowPlane.m_pMiniMapRT->m_pDsvSRV);
		m_pImmediateContext->RSSetViewports(1, &m_ViewPort);
	}
	//====================================================================================//

	return true;
}
bool Sample::Release()
{
	S_DEL(m_MFCMapInfo);
	S_DEL(m_MFCMap);
	S_DEL(m_MFCWaterMapInfo);
	S_DEL(m_MFCWaterMap);
	S_DEL(m_WaterMapQuadTree);
	S_DEL(m_MapQuadTree);
	return true;
}
#pragma endregion

#pragma region �⺻ ���� ���� �Լ�
/*�� ���� �Լ�*/
bool Sample::CreateMap(int iCellCount, int iPatchCount, float fCellDistance, TCHAR** MapTexture, int TextureCount, TCHAR* HeightMapTex)
{
	m_MFCMapInfo = new BMapDesc;
	m_MFCMapInfo->iCellCount = iCellCount;
	m_MFCMapInfo->iPatchCount = iPatchCount;
	m_MFCMapInfo->fCellDistance = fCellDistance;
	m_vMFCMapInfo.push_back(m_MFCMapInfo);

	m_MFCMap = new BMap;
	m_MFCMap->SetMapDesc(*m_MFCMapInfo);
	m_MFCMap->Create(m_pd3dDevice, m_pImmediateContext, MapTexture, TextureCount, HeightMapTex, L"../../data/shader/NormalShader.hlsl");
	m_MFCMap->UpdateNormal();
	m_vMapTexture.push_back(MapTexture[0]);
	m_vMFCMap.push_back(m_MFCMap);

	CreateQuadTree(2, 1, m_MFCMap); // ���⼭ �ִ� ���̼��� & �ּ� ���һ���� ���� ���� == MFC���� �Ѱ��ֵ��� �ؾ��Ѵ�.

									// m_MFCWaterMap�� ���͸� ǥ���� ���Դϴ�.
	m_MFCWaterMapInfo = new BMapDesc;
	m_MFCWaterMapInfo->iCellCount = iCellCount;
	m_MFCWaterMapInfo->iPatchCount = iPatchCount;
	m_MFCWaterMapInfo->fCellDistance = fCellDistance;

	m_MFCWaterMap = new BNMap;
	m_MFCWaterMap->SetMapDesc(*m_MFCWaterMapInfo);
	m_MFCWaterMap->Create(m_pd3dDevice, m_pImmediateContext, MapTexture, TextureCount, HeightMapTex, L"../../data/shader/MapShader.hlsl");
	m_MFCWaterMap->UpdateNormal();
	m_vMFCWaterMap.push_back(m_MFCWaterMap);
	return true;
}
/*���� �� ���� �Լ�*/
bool Sample::CreateQuadTree(int iMaxDepthLimit, float fMinDivideSize, BMap* Map)
{
	m_MapQuadTree = new BMapTileRender;
	m_MapQuadTree->m_iMaxDepthLimit = iMaxDepthLimit;
	m_MapQuadTree->m_fMinDivideSize = fMinDivideSize;
	m_MapQuadTree->Update(m_pd3dDevice, m_pMainCamera);
	m_MapQuadTree->Build(Map, Map->m_iNumCols, Map->m_iNumRows, m_pImmediateContext);
	m_MapQuadTree->CreateSpreatingTexture();
	// ���߿� ���� ������ �����Ǿ��� �� �������־���Ѵ�. ������ �⺻ ������ 0������ �־��� ���̴�.
	m_iSelectedMap = 0; //0����
	m_bSelectedMap = m_MapQuadTree; // ������ �⺻ ������ ���� �ٷ� �־��ְ� �ִ�.
	m_vMapQuadTree.push_back(m_MapQuadTree);
	return true;
}
/*�� ���� ��Ʈ�� �Լ�*/
bool Sample::MapHeightControl()
{
	//pNode->m_VertexList;
	// �̰� ���ؽ�
	//m_vMapQuadTree[iMap]->m_pdwLodIndexArray[pNode->m_dwLodLevel]->IndexList[pNode->m_dwLodType];
	// �̰� �ε���

	// ���ؽ��� m_DrawNodeList�� ��� �ְ�
	// �ε����� m_pdwLodIndexArray�� ��� �ִµ� ���° �ε������¸� ������ �˾ƾ� �Ѵ�.
	// ���° �ε��� ������ ���° Ÿ���� �����ִ��� �˾ƾ� �Ѵ�.
	// �ε������ۿ� ���ؽ����۰� �����Ѵ�.
	m_Select.SetMatrix(NULL, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj); //��ǥ�� ��ġ�� �ʿ��ϴ�.
	D3DXVECTOR2 ControlBox;
	m_viClosedIndex.clear();
	for (int iMap = 0; iMap < m_vMapQuadTree.size(); iMap++)
	{
		for (int iCheckNode = 0; iCheckNode < m_vMapQuadTree[iMap]->m_DrawPatchNodeList.size(); iCheckNode++)
		{
			BNode* pNode = m_vMapQuadTree[iMap]->m_DrawPatchNodeList[iCheckNode];
			if (m_Select.IntersectBox(&pNode->m_tBox))
			{
				int iIndex = iCheckNode;
				m_viClosedIndex.push_back(iIndex);
			}
		}
		if (!m_viClosedIndex.size())
		{
			return false;
		}

		/*�浹�� ó���� ������ �Ÿ��� ���� ������� ����*/
		vector<int> m_iOrgiClosedIndex;
		while (m_viClosedIndex.size())
		{
			float PLength = 99999.9f;
			int iApplyIndex = 0;
			for (int iCheckNode = 0; iCheckNode < m_viClosedIndex.size(); iCheckNode++)
			{
				D3DXVECTOR3 Pvec = m_vMapQuadTree[iMap]->m_DrawPatchNodeList[m_viClosedIndex[iCheckNode]]->m_tBox.vCenter - m_Select.m_Ray.vOrigin;
				float BLength = D3DXVec3Length(&Pvec);
				if (PLength > BLength)
				{
					iApplyIndex = iCheckNode;
				}
			}
			m_iOrgiClosedIndex.push_back(m_viClosedIndex[iApplyIndex]);
			vector<int>::iterator itor = m_viClosedIndex.begin();
			itor += iApplyIndex;
			m_viClosedIndex.erase(itor);
		}

		for (int iApply = 0; iApply < m_iOrgiClosedIndex.size(); iApply++)
		{
			BNode* pNode = m_vMapQuadTree[iMap]->m_DrawPatchNodeList[m_iOrgiClosedIndex[iApply]];
			m_vMapQuadTree[iMap]->GetLodType(pNode);
			D3DXVECTOR3 v0, v1, v2;
			int indexsize = m_vMapQuadTree[iMap]->m_pdwLodIndexArray[pNode->m_dwLodLevel]->IndexList[pNode->m_dwLodType].size() / 3;
			for (int iFace = 0; iFace < indexsize; iFace++)
			{
				v0 = pNode->m_VertexList[m_vMapQuadTree[iMap]->m_pdwLodIndexArray[pNode->m_dwLodLevel]->IndexList[pNode->m_dwLodType][iFace * 3 + 0]].Position;
				v1 = pNode->m_VertexList[m_vMapQuadTree[iMap]->m_pdwLodIndexArray[pNode->m_dwLodLevel]->IndexList[pNode->m_dwLodType][iFace * 3 + 1]].Position;
				v2 = pNode->m_VertexList[m_vMapQuadTree[iMap]->m_pdwLodIndexArray[pNode->m_dwLodLevel]->IndexList[pNode->m_dwLodType][iFace * 3 + 2]].Position;
				if (m_Select.CheckPick(v0, v1, v2))
				{
					if (m_VCtrlState == vUp || m_VCtrlState == vDown) // up Down�� ���
					{
						ControlBox = ChangeYPosition(m_Select.CrashPoint, &pNode->m_VertexList);
						if (pNode->m_tBox.vMax.y < ControlBox.x)
						{
							pNode->m_tBox.vMax.y = ControlBox.x;
							pNode->m_tBox.vCenter.y = (pNode->m_tBox.vMax.y + pNode->m_tBox.vMin.y) / 2;
							pNode->m_tBox.fExtent[1] = pNode->m_tBox.vMax.y - pNode->m_tBox.vCenter.y;
						}
						if (pNode->m_tBox.vMin.y > ControlBox.y)
						{
							pNode->m_tBox.vMin.y = ControlBox.y;
							pNode->m_tBox.vCenter.y = (pNode->m_tBox.vMax.y + pNode->m_tBox.vMin.y) / 2;
							pNode->m_tBox.fExtent[1] = pNode->m_tBox.vMax.y - pNode->m_tBox.vCenter.y;
						}
						ControlBox.x = 0;
						ControlBox.y = 0;
						pNode->m_pVertexBuffer.Attach(m_vMapQuadTree[iMap]->CreateVertexBuffer(m_pd3dDevice, &pNode->m_VertexList.at(0), pNode->m_VertexList.size(), sizeof(PNCT_Vertex)));
						for (int iNeighbor = 0; iNeighbor < pNode->m_NeighborList.size(); iNeighbor++)
						{
							if (pNode->m_NeighborList[iNeighbor] != NULL)
							{
								ControlBox = ChangeYPosition(m_Select.CrashPoint, &pNode->m_NeighborList[iNeighbor]->m_VertexList);
								if (pNode->m_NeighborList[iNeighbor]->m_tBox.vMax.y < ControlBox.x)
								{
									pNode->m_NeighborList[iNeighbor]->m_tBox.vMax.y = ControlBox.x;
									pNode->m_NeighborList[iNeighbor]->m_tBox.vCenter.y = (pNode->m_NeighborList[iNeighbor]->m_tBox.vMax.y + pNode->m_NeighborList[iNeighbor]->m_tBox.vMin.y) / 2;
									pNode->m_NeighborList[iNeighbor]->m_tBox.fExtent[1] = pNode->m_NeighborList[iNeighbor]->m_tBox.vMax.y - pNode->m_NeighborList[iNeighbor]->m_tBox.vCenter.y;
								}
								if (pNode->m_NeighborList[iNeighbor]->m_tBox.vMin.y > ControlBox.y)
								{
									pNode->m_NeighborList[iNeighbor]->m_tBox.vMin.y = ControlBox.y;
									pNode->m_NeighborList[iNeighbor]->m_tBox.vCenter.y = (pNode->m_NeighborList[iNeighbor]->m_tBox.vMax.y + pNode->m_NeighborList[iNeighbor]->m_tBox.vMin.y) / 2;
									pNode->m_NeighborList[iNeighbor]->m_tBox.fExtent[1] = pNode->m_NeighborList[iNeighbor]->m_tBox.vMax.y - pNode->m_NeighborList[iNeighbor]->m_tBox.vCenter.y;
								}
								ControlBox.x = 0;
								ControlBox.y = 0;
								pNode->m_NeighborList[iNeighbor]->m_pVertexBuffer.Attach(m_vMapQuadTree[iMap]->
									CreateVertexBuffer(m_pd3dDevice, &pNode->m_NeighborList[iNeighbor]->
										m_VertexList.at(0), pNode->m_NeighborList[iNeighbor]->m_VertexList.size(), sizeof(PNCT_Vertex)));
							}
						}
						return true;
					}
					else // Parallel�� ���
					{
						ControlBox = ChangeYParallel(m_Select.CrashPoint, &pNode->m_VertexList);
						if (pNode->m_tBox.vMax.y < ControlBox.x)
						{
							pNode->m_tBox.vMax.y = ControlBox.x;
							pNode->m_tBox.vCenter.y = (pNode->m_tBox.vMax.y + pNode->m_tBox.vMin.y) / 2;
							pNode->m_tBox.fExtent[1] = pNode->m_tBox.vMax.y - pNode->m_tBox.vCenter.y;
						}
						if (pNode->m_tBox.vMin.y > ControlBox.y)
						{
							pNode->m_tBox.vMin.y = ControlBox.y;
							pNode->m_tBox.vCenter.y = (pNode->m_tBox.vMax.y + pNode->m_tBox.vMin.y) / 2;
							pNode->m_tBox.fExtent[1] = pNode->m_tBox.vMax.y - pNode->m_tBox.vCenter.y;
						}
						ControlBox.x = 0;
						ControlBox.y = 0;
						pNode->m_pVertexBuffer.Attach(m_vMapQuadTree[iMap]->CreateVertexBuffer(m_pd3dDevice, &pNode->m_VertexList.at(0), pNode->m_VertexList.size(), sizeof(PNCT_Vertex)));
						for (int iNeighbor = 0; iNeighbor < pNode->m_NeighborList.size(); iNeighbor++)
						{
							if (pNode->m_NeighborList[iNeighbor] != NULL)
							{
								ControlBox = ChangeYParallel(m_Select.CrashPoint, &pNode->m_NeighborList[iNeighbor]->m_VertexList);
								if (pNode->m_NeighborList[iNeighbor]->m_tBox.vMax.y < ControlBox.x)
								{
									pNode->m_NeighborList[iNeighbor]->m_tBox.vMax.y = ControlBox.x;
									pNode->m_NeighborList[iNeighbor]->m_tBox.vCenter.y = (pNode->m_NeighborList[iNeighbor]->m_tBox.vMax.y + pNode->m_NeighborList[iNeighbor]->m_tBox.vMin.y) / 2;
									pNode->m_NeighborList[iNeighbor]->m_tBox.fExtent[1] = pNode->m_NeighborList[iNeighbor]->m_tBox.vMax.y - pNode->m_NeighborList[iNeighbor]->m_tBox.vCenter.y;
								}
								if (pNode->m_NeighborList[iNeighbor]->m_tBox.vMin.y > ControlBox.y)
								{
									pNode->m_NeighborList[iNeighbor]->m_tBox.vMin.y = ControlBox.y;
									pNode->m_NeighborList[iNeighbor]->m_tBox.vCenter.y = (pNode->m_NeighborList[iNeighbor]->m_tBox.vMax.y + pNode->m_NeighborList[iNeighbor]->m_tBox.vMin.y) / 2;
									pNode->m_NeighborList[iNeighbor]->m_tBox.fExtent[1] = pNode->m_NeighborList[iNeighbor]->m_tBox.vMax.y - pNode->m_NeighborList[iNeighbor]->m_tBox.vCenter.y;
								}
								ControlBox.x = 0;
								ControlBox.y = 0;
								pNode->m_NeighborList[iNeighbor]->m_pVertexBuffer.Attach(m_vMapQuadTree[iMap]->
									CreateVertexBuffer(m_pd3dDevice, &pNode->m_NeighborList[iNeighbor]->
										m_VertexList.at(0), pNode->m_NeighborList[iNeighbor]->m_VertexList.size(), sizeof(PNCT_Vertex)));
							}
						}
						bParallel = true;
						return true;
					}
				}
			}
		}
	}
	return false;
}
/*�� ���� ���� �Լ�(��, �ٿ� / MapHeightControl ���� ȣ��)*/
D3DXVECTOR2 Sample::ChangeYPosition(D3DXVECTOR3 CPos, vector<PNCT_Vertex>* VertexList)
{
	D3DXVECTOR2 ControlBox;
	ControlBox.x = -99999.9f; // MAX
	ControlBox.y = 99999.9f; // MIN
	float HeightControl = 0.0f;
	if (m_VCtrlState == vUp)
	{
		HeightControl = 1.0f * g_fSecPerFrame * m_fHeightValue;
	}
	else //(m_VCtrlState == vDown)
	{
		HeightControl = -1.0f * g_fSecPerFrame * m_fHeightValue;
	}
	for (int iVer = 0; iVer < VertexList->size(); iVer++)
	{
		D3DXVECTOR3 Va = CPos;
		D3DXVECTOR3 Vb = VertexList[0][iVer].Position;
		Va.y = 0.0f;
		Vb.y = 0.0f;
		D3DXVECTOR3 LengthTemp = Va - Vb;
		float length = D3DXVec3Length(&LengthTemp);
		length = length / m_fRadius;
		if (length < 1.0f)
		{
			length *= 90;
			double pi = 3.141592;
			length = pi * length / 180.0;
			length = cosf(length);
			length *= HeightControl; // ���� ��ȭ�� �߰����

			VertexList[0][iVer].Position.y += length;

			if (ControlBox.x < VertexList[0][iVer].Position.y)
			{
				ControlBox.x = VertexList[0][iVer].Position.y;
			}
			if (ControlBox.y > VertexList[0][iVer].Position.y)
			{
				ControlBox.y = VertexList[0][iVer].Position.y;
			}
		}
	}
	return ControlBox;
}
/*�� ���� ���� �Լ�(���� / MapHeightControl ���� ȣ��)*/
D3DXVECTOR2 Sample::ChangeYParallel(D3DXVECTOR3 CPos, vector<PNCT_Vertex>* VertexList)
{
	D3DXVECTOR2 ControlBox;
	ControlBox.x = -99999.9f; // MAX
	ControlBox.y = 99999.9f; // MIN
	float HeightControl = 0.0f;
	if (m_VCtrlState == vUp)
	{
		HeightControl = 1.0f;
	}
	else //(m_VCtrlState == vDown)
	{
		HeightControl = -1.0f;
	}
	for (int iVer = 0; iVer < VertexList->size(); iVer++)
	{
		D3DXVECTOR3 Va = CPos;
		D3DXVECTOR3 Vb = VertexList[0][iVer].Position;
		Va.y = 0.0f;
		Vb.y = 0.0f;
		D3DXVECTOR3 LengthTemp = Va - Vb;
		float length = D3DXVec3Length(&LengthTemp);
		length = length / m_fRadius;
		if (length < 1.0f)
		{
			if (bParallel) //������ �������� ����
			{
				VertexList[0][iVer].Position.y = fParallel;
				if (ControlBox.x < VertexList[0][iVer].Position.y)
				{
					ControlBox.x = VertexList[0][iVer].Position.y;
				}
				if (ControlBox.y > VertexList[0][iVer].Position.y)
				{
					ControlBox.y = VertexList[0][iVer].Position.y;
				}
			}
			else
			{
				VertexList[0][iVer].Position.y = CPos.y;
				fParallel = CPos.y;
				if (ControlBox.x < VertexList[0][iVer].Position.y)
				{
					ControlBox.x = VertexList[0][iVer].Position.y;
				}
				if (ControlBox.y > VertexList[0][iVer].Position.y)
				{
					ControlBox.y = VertexList[0][iVer].Position.y;
				}
			}
		}
	}
	return ControlBox;
}
/*�� ���� ��ȯ�� �Լ�*/
D3DXVECTOR4 Sample::GetHeight(D3DXVECTOR3 HeroPos)
{
	int iXindex;
	int iZindex;
	float fXDetailPoint;
	float fZDetailPoint;
	float fCellDistance = m_MapSetting.m_SelectedMap->m_pMap->m_fCellDistance;
	int iChangePoint = m_MapSetting.m_SelectedMap->m_pRootNode->m_tBox.fExtent[0];
	int HeroAddress;

	iXindex = (HeroPos.x + iChangePoint) / fCellDistance;
	iZindex = ((HeroPos.z - iChangePoint) * -1.0f) / fCellDistance;
	fXDetailPoint = HeroPos.x - (int)HeroPos.x;
	fZDetailPoint = HeroPos.z - (int)HeroPos.z;

	if (fXDetailPoint > 0 && fZDetailPoint < 0) // 2��и�
	{
		fZDetailPoint = 1 + fZDetailPoint;
	}

	if (fXDetailPoint < 0 && HeroPos.z < 0) // 3��и�
	{
		fXDetailPoint = 1 + fXDetailPoint;
		fZDetailPoint = 1 + fZDetailPoint;
	}

	if (fXDetailPoint < 0 && HeroPos.z > 0) // 4��и�
	{
		fXDetailPoint = 1 + fXDetailPoint;
	}

	HeroAddress = m_MapSetting.m_MapAddress[iZindex][iXindex];

	float fResultHeight = 0.0f;
	float Left = m_MapSetting.m_TileInfo[HeroAddress].TileEdge[0].y - m_MapSetting.m_TileInfo[HeroAddress].TileEdge[2].y;
	float ResultLeft = Left*fZDetailPoint + m_MapSetting.m_TileInfo[HeroAddress].TileEdge[2].y;
	float Right = m_MapSetting.m_TileInfo[HeroAddress].TileEdge[1].y - m_MapSetting.m_TileInfo[HeroAddress].TileEdge[3].y;
	float ResultRight = Right*fZDetailPoint + m_MapSetting.m_TileInfo[HeroAddress].TileEdge[3].y;
	float CenterResult = ResultLeft + (ResultRight - ResultLeft) * fXDetailPoint;

	D3DXVECTOR3 TempNormal;
	if (fXDetailPoint + fZDetailPoint < 1.0f)
		TempNormal = m_MapSetting.m_TileInfo[HeroAddress].TileNormal[0];
	else
		TempNormal = m_MapSetting.m_TileInfo[HeroAddress].TileNormal[1];

	D3DXVECTOR4 Result;
	Result.x = TempNormal.x;
	Result.y = TempNormal.y;
	Result.z = TempNormal.z;
	Result.w = CenterResult;

	return Result;
}

/*�������� �Լ�*/
bool Sample::MapSpreating()
{
	m_Select.SetMatrix(NULL, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj); //��ǥ�� ��ġ + ���콺 ���� �޾ƿ���
	m_viClosedIndex.clear();
	for (int iMap = 0; iMap < m_vMapQuadTree.size(); iMap++)
	{
		for (int iCheckNode = 0; iCheckNode < m_vMapQuadTree[iMap]->m_DrawPatchNodeList.size(); iCheckNode++)
		{
			BNode* pNode = m_vMapQuadTree[iMap]->m_DrawPatchNodeList[iCheckNode];
			if (m_Select.IntersectBox(&pNode->m_tBox))
			{
				int iIndex = iCheckNode;
				m_viClosedIndex.push_back(iIndex);
			}
		}
		if (!m_viClosedIndex.size())
		{
			return false;
		}

		/*�浹�� ó���� ������ �Ÿ��� ���� ������� ����*/
		vector<int> m_iOrgiClosedIndex;
		while (m_viClosedIndex.size())
		{
			float PLength = 99999.9f;
			int iApplyIndex = 0;
			for (int iCheckNode = 0; iCheckNode < m_viClosedIndex.size(); iCheckNode++)
			{
				D3DXVECTOR3 Pvec = m_vMapQuadTree[iMap]->m_DrawPatchNodeList[m_viClosedIndex[iCheckNode]]->m_tBox.vCenter - m_Select.m_Ray.vOrigin;
				float BLength = D3DXVec3Length(&Pvec);
				if (PLength > BLength)
				{
					iApplyIndex = iCheckNode;
				}
			}
			m_iOrgiClosedIndex.push_back(m_viClosedIndex[iApplyIndex]);
			vector<int>::iterator itor = m_viClosedIndex.begin();
			itor += iApplyIndex;
			m_viClosedIndex.erase(itor);
		}

		// m_iOrgiClosedIndex = ����� ������� ���ĵ� �浹�� ���
		for (int iApply = 0; iApply < m_iOrgiClosedIndex.size(); iApply++)
		{
			BNode* pNode = m_vMapQuadTree[iMap]->m_DrawPatchNodeList[m_iOrgiClosedIndex[iApply]];
			m_vMapQuadTree[iMap]->GetLodType(pNode);
			D3DXVECTOR3 v0, v1, v2;
			int indexsize = m_vMapQuadTree[iMap]->m_pdwLodIndexArray[pNode->m_dwLodLevel]->IndexList[pNode->m_dwLodType].size() / 3;
			for (int iFace = 0; iFace < indexsize; iFace++)
			{
				v0 = pNode->m_VertexList[m_vMapQuadTree[iMap]->m_pdwLodIndexArray[pNode->m_dwLodLevel]->IndexList[pNode->m_dwLodType][iFace * 3 + 0]].Position;
				v1 = pNode->m_VertexList[m_vMapQuadTree[iMap]->m_pdwLodIndexArray[pNode->m_dwLodLevel]->IndexList[pNode->m_dwLodType][iFace * 3 + 1]].Position;
				v2 = pNode->m_VertexList[m_vMapQuadTree[iMap]->m_pdwLodIndexArray[pNode->m_dwLodLevel]->IndexList[pNode->m_dwLodType][iFace * 3 + 2]].Position;
				if (m_Select.CheckPick(v0, v1, v2)) // �浹������ �Ǻ� �ƴٸ�
				{
					float RootNodeSize = m_vMapQuadTree[iMap]->m_pRootNode->m_tBox.vMax.x - m_vMapQuadTree[iMap]->m_pRootNode->m_tBox.vMin.x;
					D3DXVECTOR3 Crash = m_Select.CrashPoint;
					Crash.z *= -1.0f;
					Crash.x = Crash.x + (RootNodeSize / 2);
					Crash.z = Crash.z + (RootNodeSize / 2);

					SpreatingBox.vMin.x = Crash.x - m_fSpreatingRadius;
					SpreatingBox.vMin.z = Crash.z - m_fSpreatingRadius;
					SpreatingBox.vMax.x = Crash.x + m_fSpreatingRadius;
					SpreatingBox.vMax.z = Crash.z + m_fSpreatingRadius;

					Crash.x = Crash.x / RootNodeSize;
					Crash.z = Crash.z / RootNodeSize;

					SpreatingBox.vMin.x = SpreatingBox.vMin.x / RootNodeSize;
					SpreatingBox.vMin.z = SpreatingBox.vMin.z / RootNodeSize;
					SpreatingBox.vMax.x = SpreatingBox.vMax.x / RootNodeSize;
					SpreatingBox.vMax.z = SpreatingBox.vMax.z / RootNodeSize;

					// uv������ �����Ϸ�
					if (!m_ModeGradient)
						m_vMapQuadTree[iMap]->Spreating(m_pImmediateContext, SpreatingBox, Crash, m_fSpreatingRadius, m_iSpreatingColor);
					else
						m_vMapQuadTree[iMap]->GradientSpreating(m_pImmediateContext, SpreatingBox, Crash, m_fSpreatingRadius, m_iSpreatingColor);
				}
			}
		}
	}
	return false;
}
/*�������ÿ� �ؽ��� ���� �Լ�*/
bool Sample::GetSplattingTexName(int Color, TCHAR** Name)
{
	m_mMapSplattingTexture[Color] = Name[0];
	Text.push_back(Name[0]);
	return true;
}

/*�� ���� �Լ�*/
bool Sample::MapExport()
{
	// ���� �ڵ�� 1���� �ʸ��� export�ϴ� �ڵ��Դϴ�.
	// �ټ��� ���� export�ϱ� ���ؼ��� ������ �ʿ��մϴ�.
	m_Exporter.SetExporter(m_pImmediateContext, L"../../data/SaveExport.txt");
	m_Exporter.ExpMapObjInfo(&m_MapObjMgr);
	m_Exporter.ExpAlphaMapTex(m_vMapQuadTree[0]->SpreatingTexTure);
	m_Exporter.ExpSpawnMapTex(m_vMapQuadTree[0]->ObjPosTexture);
	if (m_vMapTexture.size())
	{
		m_vMapTexture.resize(1);
	}
	m_Exporter.ExpTexName(m_vMapTexture[0], m_mMapSplattingTexture);
	if (m_vWaterQuadTree.size())
	{
		m_Exporter.ExpWaterMapInfo(m_vWaterQuadTree[0]->m_iSelectedMap, m_vWaterQuadTree[0]->m_strWaterTex,
			m_vWaterQuadTree[0]->m_strWaterNormalTex, m_vWaterQuadTree[0]->m_fWaterHeight, m_vWaterQuadTree[0]->m_fWaterWave);
	}
	m_Exporter.ExpMapInfo(m_vMFCMapInfo[0]);
	m_Exporter.ExpHeightTotal(m_vMapQuadTree[0]->m_pRootNode);
	if (!m_Exporter.m_pStream) return false;
	fclose(m_Exporter.m_pStream);
	return true;
}
/*�� ȣ�� �Լ�*/
bool Sample::MapImport(TCHAR* ImportFile)
{
	if (!m_Importer.OpenStream(ImportFile, m_pd3dDevice, m_pImmediateContext, m_pMainCamera))
	{
		return false;
	}
	m_MapObjTotalCnt += m_Importer.LoadMapObj(&m_MapObjMgr);
	m_DrawObjList.resize(m_MapObjMgr.size());
	m_Importer.LoadMap(&m_vMFCMap, &m_vMFCMapInfo, &m_vMapQuadTree, &m_vMapTexture, &m_mMapSplattingTexture, &m_vWaterQuadTree);
	m_bSelectedMap = m_vMapQuadTree[0];
	m_MapSetting.SettingMap(m_vMapQuadTree[0]);
	m_MapSetting.MapInfoSetting();
	m_MapSetting.SettingSoundInfo();
	m_MapSetting.SettingObjInfo();
	CreateNormal();

	//���͸ʿ� �ӽ��ڵ�
	m_bSelectedMap = m_vMapQuadTree[0];
	m_MFCWaterMapInfo = new BMapDesc;
	m_MFCWaterMapInfo->iCellCount = m_vMFCMapInfo[0]->iCellCount;
	m_MFCWaterMapInfo->iPatchCount = m_vMFCMapInfo[0]->iPatchCount;
	m_MFCWaterMapInfo->fCellDistance = m_vMFCMapInfo[0]->fCellDistance;

	m_MFCWaterMap = new BNMap;
	m_MFCWaterMap->SetMapDesc(*m_MFCWaterMapInfo);
	m_MFCWaterMap->Create(m_pd3dDevice, m_pImmediateContext, &m_mMapSplattingTexture[0], 1, NULL, L"../../data/shader/MapShader.hlsl");
	m_MFCWaterMap->UpdateNormal();
	m_vMFCWaterMap.push_back(m_MFCWaterMap);

	m_vWaterQuadTree[0]->m_iMaxDepthLimit = m_vMapQuadTree[m_vWaterQuadTree[0]->m_iSelectedMap]->m_iMaxDepthLimit;
	m_vWaterQuadTree[0]->m_fMinDivideSize = m_vWaterQuadTree[0]->m_fWaterWave;
	m_vWaterQuadTree[0]->Update(m_pd3dDevice, m_pMainCamera, m_vMapQuadTree[m_vWaterQuadTree[0]->m_iSelectedMap]);
	m_vWaterQuadTree[0]->Build(m_vMFCWaterMap[m_vWaterQuadTree[0]->m_iSelectedMap], m_vMFCWaterMap[m_vWaterQuadTree[0]->m_iSelectedMap]->m_iNumCols,
		m_vMFCWaterMap[m_vWaterQuadTree[0]->m_iSelectedMap]->m_iNumRows, m_pImmediateContext, m_vWaterQuadTree[0]->m_strWaterTex, m_vWaterQuadTree[0]->m_strWaterNormalTex);
	m_vWaterQuadTree[0]->m_pWaterReflectMap = m_vMapQuadTree[m_vMFCWaterMap.size() - 1];
	for (int imap = 0; imap < 3; imap++)
	{
		if (!m_vWaterQuadTree[0]->m_MiniMap[imap].Create(m_pd3dDevice, L"../../data/shader/TestShader.hlsl"))
		{
			return false;
		}
		m_vWaterQuadTree[0]->m_MiniMap[imap].Set(m_pd3dDevice, 0, imap * 200, 200, 200);
	}
	return true;
}
#pragma endregion

#pragma region ���͸� ���� �Լ�
/*���͸� ���� �Լ�*/
bool Sample::CreateWaterMap(BMapTileRender* SelectedMap, int iMaxDepthLimit, float fMinDivideSize, int iSelectedMap, TCHAR* WaterTex, TCHAR* WaterNormalTex, float fHeight, float fWave)
{
	if (m_vMFCWaterMap.size())
	{
		m_WaterMapQuadTree = new BWaterMap;
		m_WaterMapQuadTree->m_fWaterHeight = fHeight;
		m_WaterMapQuadTree->m_fWaterWave = fWave;
		m_WaterMapQuadTree->m_iMaxDepthLimit = iMaxDepthLimit;
		m_WaterMapQuadTree->m_fMinDivideSize = fMinDivideSize;
		m_WaterMapQuadTree->Update(m_pd3dDevice, m_pMainCamera, SelectedMap);
		m_WaterMapQuadTree->Build(m_vMFCWaterMap[iSelectedMap], m_vMFCWaterMap[iSelectedMap]->m_iNumCols,
			m_vMFCWaterMap[iSelectedMap]->m_iNumRows, m_pImmediateContext, WaterTex, WaterNormalTex);
		m_WaterMapQuadTree->m_pWaterReflectMap = m_vMapQuadTree[m_vMFCWaterMap.size() - 1];
		for (int imap = 0; imap < 3; imap++)
		{
			if (!m_WaterMapQuadTree->m_MiniMap[imap].Create(m_pd3dDevice, L"../../data/shader/TestShader.hlsl"))
			{
				return false;
			}
			m_WaterMapQuadTree->m_MiniMap[imap].Set(m_pd3dDevice, 0, imap * 200, 200, 200);
		}
		m_vWaterQuadTree.push_back(m_WaterMapQuadTree);
	}
	return false;
}
#pragma endregion

#pragma region �� ���� ���� �Լ�
/*�� �������� �ؽ���ȭ ���� �Լ�*/
bool Sample::MapObjInfoSpreating()
{
	m_Select.SetMatrix(NULL, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj); //��ǥ�� ��ġ + ���콺 ���� �޾ƿ���
	m_viClosedIndex.clear();
	for (int iMap = 0; iMap < m_vMapQuadTree.size(); iMap++)
	{
		for (int iCheckNode = 0; iCheckNode < m_vMapQuadTree[iMap]->m_DrawPatchNodeList.size(); iCheckNode++)
		{
			BNode* pNode = m_vMapQuadTree[iMap]->m_DrawPatchNodeList[iCheckNode];
			if (m_Select.IntersectBox(&pNode->m_tBox))
			{
				int iIndex = iCheckNode;
				m_viClosedIndex.push_back(iIndex);
			}
		}
		if (!m_viClosedIndex.size())
		{
			return false;
		}

		/*�浹�� ó���� ������ �Ÿ��� ���� ������� ����*/
		vector<int> m_iOrgiClosedIndex;
		while (m_viClosedIndex.size())
		{
			float PLength = 99999.9f;
			int iApplyIndex = 0;
			for (int iCheckNode = 0; iCheckNode < m_viClosedIndex.size(); iCheckNode++)
			{
				D3DXVECTOR3 Pvec = m_vMapQuadTree[iMap]->m_DrawPatchNodeList[m_viClosedIndex[iCheckNode]]->m_tBox.vCenter - m_Select.m_Ray.vOrigin;
				float BLength = D3DXVec3Length(&Pvec);
				if (PLength > BLength)
				{
					iApplyIndex = iCheckNode;
				}
			}
			m_iOrgiClosedIndex.push_back(m_viClosedIndex[iApplyIndex]);
			vector<int>::iterator itor = m_viClosedIndex.begin();
			itor += iApplyIndex;
			m_viClosedIndex.erase(itor);
		}

		// m_iOrgiClosedIndex = ����� ������� ���ĵ� �浹�� ���
		for (int iApply = 0; iApply < m_iOrgiClosedIndex.size(); iApply++)
		{
			BNode* pNode = m_vMapQuadTree[iMap]->m_DrawPatchNodeList[m_iOrgiClosedIndex[iApply]];
			m_vMapQuadTree[iMap]->GetLodType(pNode);
			D3DXVECTOR3 v0, v1, v2;
			int indexsize = m_vMapQuadTree[iMap]->m_pdwLodIndexArray[pNode->m_dwLodLevel]->IndexList[pNode->m_dwLodType].size() / 3;
			for (int iFace = 0; iFace < indexsize; iFace++)
			{
				v0 = pNode->m_VertexList[m_vMapQuadTree[iMap]->m_pdwLodIndexArray[pNode->m_dwLodLevel]->IndexList[pNode->m_dwLodType][iFace * 3 + 0]].Position;
				v1 = pNode->m_VertexList[m_vMapQuadTree[iMap]->m_pdwLodIndexArray[pNode->m_dwLodLevel]->IndexList[pNode->m_dwLodType][iFace * 3 + 1]].Position;
				v2 = pNode->m_VertexList[m_vMapQuadTree[iMap]->m_pdwLodIndexArray[pNode->m_dwLodLevel]->IndexList[pNode->m_dwLodType][iFace * 3 + 2]].Position;
				if (m_Select.CheckPick(v0, v1, v2)) // �浹������ �Ǻ� �ƴٸ�
				{
					float RootNodeSize = m_vMapQuadTree[iMap]->m_pRootNode->m_tBox.vMax.x - m_vMapQuadTree[iMap]->m_pRootNode->m_tBox.vMin.x;
					D3DXVECTOR3 Crash = m_Select.CrashPoint;

					Crash.x = Crash.x + (RootNodeSize / 2);
					Crash.z = Crash.z - (RootNodeSize / 2);
					Crash.z *= -1.0f;

					SpreatingBox.vMin.x = Crash.x - m_iSpreatingRange;
					SpreatingBox.vMin.z = Crash.z - m_iSpreatingRange;
					SpreatingBox.vMax.x = Crash.x + m_iSpreatingRange;
					SpreatingBox.vMax.z = Crash.z + m_iSpreatingRange;

					//Crash.x = Crash.x / RootNodeSize;
					//Crash.z = Crash.z / RootNodeSize;

					//SpreatingBox.vMin.x = SpreatingBox.vMin.x / RootNodeSize;
					//SpreatingBox.vMin.z = SpreatingBox.vMin.z / RootNodeSize;
					//SpreatingBox.vMax.x = SpreatingBox.vMax.x / RootNodeSize;
					//SpreatingBox.vMax.z = SpreatingBox.vMax.z / RootNodeSize;

					// uv������ �����Ϸ�
					m_vMapQuadTree[iMap]->ObjSpreating(m_pImmediateContext, SpreatingBox, Crash, m_iSpreatingRange, m_iObjSpreatingColor);
				}
			}
		}
	}
	return false;
}
/*���Ҹ� �븻 �����Լ�*/
void Sample::CreateNormal()
{
	if (m_vMapQuadTree.size() < 1)
		return;
	else
	{
		CalcNormal(m_vMapQuadTree[0]->m_pRootNode, (m_vMapQuadTree[0]->m_dwWidth - 1) / 2);
	}
}
/*���Ҹ� �븻 ����Լ� (���, CreateNormal ���� ȣ��)*/
void Sample::CalcNormal(BNode* pNode, int iRatio)
{
	if (pNode->m_pChild[0] != NULL)
	{
		for (int iNode = 0; iNode < 4; iNode++)
		{
			CalcNormal(pNode->m_pChild[iNode], iRatio);
		}
	}
	else
	{
		int iAround = 0;
		for (int iNor = 0; iNor < pNode->m_VertexList.size(); iNor++)
		{
			int x = pNode->m_VertexList[iNor].Position.x;
			int z = pNode->m_VertexList[iNor].Position.z;
			x = x + iRatio;
			z = (z - iRatio) * -1;
			if (x == iRatio * 2)
			{
				x--;
			}
			if (z == iRatio * 2)
			{
				z--;
			}
			D3DXVECTOR3 avgNormal;
			avgNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			// 1�� Ÿ��
			if (x > 1 && z > 1)
			{
				avgNormal = avgNormal + m_MapSetting.m_TileInfo[m_MapSetting.m_MapAddress[z - 1][x - 1]].TileNormal[1];
				iAround++;
			}
			// 2�� 3�� Ÿ��
			if (z > 1)
			{
				avgNormal = avgNormal + m_MapSetting.m_TileInfo[m_MapSetting.m_MapAddress[z - 1][x]].TileNormal[0];
				avgNormal = avgNormal + m_MapSetting.m_TileInfo[m_MapSetting.m_MapAddress[z - 1][x]].TileNormal[1];
				iAround++;
				iAround++;
			}
			// 4�� 5�� Ÿ��
			if (x > 1 && z < iRatio)
			{
				avgNormal = avgNormal + m_MapSetting.m_TileInfo[m_MapSetting.m_MapAddress[z][x - 1]].TileNormal[0];
				avgNormal = avgNormal + m_MapSetting.m_TileInfo[m_MapSetting.m_MapAddress[z][x - 1]].TileNormal[1];
				iAround++;
				iAround++;
			}
			if (x < iRatio && z < iRatio)
			{
				avgNormal = avgNormal + m_MapSetting.m_TileInfo[m_MapSetting.m_MapAddress[z][x]].TileNormal[0];
				iAround++;
			}
			avgNormal.x /= (float)iAround;
			avgNormal.y /= (float)iAround;
			avgNormal.z /= (float)iAround;
			D3DXVec3Normalize(&avgNormal, &avgNormal);

			pNode->m_VertexList[iNor].Normal.x = avgNormal.x;
			pNode->m_VertexList[iNor].Normal.y = avgNormal.y;
			pNode->m_VertexList[iNor].Normal.z = avgNormal.z;
		}
		m_pImmediateContext->UpdateSubresource(pNode->m_pVertexBuffer.Get(), 0, NULL, &pNode->m_VertexList[0], 0, 0);
	}
}
#pragma endregion

#pragma region �� ������Ʈ ���� �Լ�
/*������Ʈ �����Լ�*/
void Sample::SelectMapObj()
{
	if (m_vSelectedObjInfo.size())
	{
		if (m_vBeforeSelectedObjInfo.size() <= 0)
		{
			m_vBeforeSelectedObjInfo.resize(1);
		}
		m_vBeforeSelectedObjInfo[0] = m_vSelectedObjInfo[0];
	}
	int ObjTotal = 1;
	m_Select.SetMatrix(NULL, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj); //��ǥ�� ��ġ + ���콺 ���� �޾ƿ���
	for (int ObjCnt = 0; ObjCnt < m_MapObjMgr.size(); ObjCnt++) // ���� ������� �� ������Ʈ�� ��, �ı��� ��, ���� �� 3������ ������ 3������ ����.
	{
		for (int iObj = 0; iObj < m_MapObjMgr[ObjCnt]->m_InstanceInfo.size(); iObj++)
		{
			if (m_Select.IntersectBox(&m_MapObjMgr[ObjCnt]->m_tBoundingBox[iObj]))
			{
				m_vSelectedObjInfo.resize(ObjTotal);
				m_vSelectedObjInfo[ObjTotal - 1].m_iObjType = ObjCnt;
				m_vSelectedObjInfo[ObjTotal - 1].m_iObjNum = iObj;
				m_vSelectedObjInfo[ObjTotal - 1].m_fRange = D3DXVec3Length(&(m_MapObjMgr[ObjCnt]->m_tBoundingBox[iObj].vCenter - m_Select.m_Ray.vOrigin));
				ObjTotal++;
			}
		}
	}

	if (m_vSelectedObjInfo.size() < 2)
	{
		if (m_vSelectedObjInfo.size() > 0)
		{
			m_MapObjMgr[m_vSelectedObjInfo[0].m_iObjType]->m_InstanceInfo[m_vSelectedObjInfo[0].m_iObjNum].Selected = true;
			if (m_vBeforeSelectedObjInfo.size() && (m_vBeforeSelectedObjInfo[0].m_iObjType != m_vSelectedObjInfo[0].m_iObjType || m_vBeforeSelectedObjInfo[0].m_iObjNum != m_vSelectedObjInfo[0].m_iObjNum))
			{
				m_MapObjMgr[m_vBeforeSelectedObjInfo[0].m_iObjType]->m_InstanceInfo[m_vBeforeSelectedObjInfo[0].m_iObjNum].Selected = false;
			}
			return;
		}
	}
	else
	{
		int iSelectedObj = 0;
		for (int iSel = 1; iSel < m_vSelectedObjInfo.size(); iSel++)
		{
			if (m_vSelectedObjInfo[iSelectedObj].m_fRange > m_vSelectedObjInfo[iSel].m_fRange)
			{
				iSelectedObj = iSel;
			}
		}
		m_TempInfo = m_vSelectedObjInfo[iSelectedObj];
		m_vSelectedObjInfo.clear();
		m_vSelectedObjInfo.reserve(1);
		m_vSelectedObjInfo.push_back(m_TempInfo);
		m_MapObjMgr[m_vSelectedObjInfo[0].m_iObjType]->m_InstanceInfo[m_vSelectedObjInfo[0].m_iObjNum].Selected = true;
		if (m_vBeforeSelectedObjInfo.size() && (m_vBeforeSelectedObjInfo[0].m_iObjType != m_vSelectedObjInfo[0].m_iObjType || m_vBeforeSelectedObjInfo[0].m_iObjNum != m_vSelectedObjInfo[0].m_iObjNum))
		{
			m_MapObjMgr[m_vBeforeSelectedObjInfo[0].m_iObjType]->m_InstanceInfo[m_vBeforeSelectedObjInfo[0].m_iObjNum].Selected = false;
		}
		return;
	}
}
/*������Ʈ �����Լ�*/
void Sample::ClearSelected()
{
	if (m_vSelectedObjInfo.size())
	{
		vector<InstancingInfo>::iterator iter;
		iter = m_MapObjMgr[m_vSelectedObjInfo[0].m_iObjType]->m_InstanceInfo.begin();
		iter += m_vSelectedObjInfo[0].m_iObjNum;
		m_MapObjMgr[m_vSelectedObjInfo[0].m_iObjType]->m_InstanceInfo.erase(iter);
		m_MapObjMgr[m_vSelectedObjInfo[0].m_iObjType]->m_iInstanceCount--;
		if (m_MapObjMgr[m_vSelectedObjInfo[0].m_iObjType]->m_iInstanceCount > 0)
		{
			m_MapObjMgr[m_vSelectedObjInfo[0].m_iObjType]->InitInstanceBuffer(m_MapObjMgr[m_vSelectedObjInfo[0].m_iObjType]->m_iInstanceCount, m_vSelectedObjInfo[0].m_iObjType);
		}
		m_vSelectedObjInfo.clear();
		m_vBeforeSelectedObjInfo.clear();
	}
}
/*������Ʈ �̵��Լ�*/
bool Sample::GetMapObjectMovePosition()
{
	if (m_vSelectedObjInfo.size() < 1)
	{
		return false;
	}
	m_Select.SetMatrix(NULL, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj); //��ǥ�� ��ġ + ���콺 ���� �޾ƿ���
	for (int iMap = 0; iMap < m_vMapQuadTree.size(); iMap++)
	{
		for (int iCheckNode = 0; iCheckNode < m_vMapQuadTree[iMap]->m_DrawPatchNodeList.size(); iCheckNode++)
		{
			BNode* pNode = m_vMapQuadTree[iMap]->m_DrawPatchNodeList[iCheckNode];
			if (m_Select.IntersectBox(&pNode->m_tBox))
			{
				int iIndex = iCheckNode;
				m_viClosedIndex.push_back(iIndex);
			}
		}
		if (!m_viClosedIndex.size())
		{
			return false;
		}

		/*�浹�� ó���� ������ �Ÿ��� ���� ������� ����*/
		vector<int> m_iOrgiClosedIndex;
		while (m_viClosedIndex.size())
		{
			float PLength = 99999.9f;
			int iApplyIndex = 0;
			for (int iCheckNode = 0; iCheckNode < m_viClosedIndex.size(); iCheckNode++)
			{
				D3DXVECTOR3 Pvec = m_vMapQuadTree[iMap]->m_DrawPatchNodeList[m_viClosedIndex[iCheckNode]]->m_tBox.vCenter - m_Select.m_Ray.vOrigin;
				float BLength = D3DXVec3Length(&Pvec);
				if (PLength > BLength)
				{
					iApplyIndex = iCheckNode;
				}
			}
			m_iOrgiClosedIndex.push_back(m_viClosedIndex[iApplyIndex]);
			vector<int>::iterator itor = m_viClosedIndex.begin();
			itor += iApplyIndex;
			m_viClosedIndex.erase(itor);
		}

		// m_iOrgiClosedIndex = ����� ������� ���ĵ� �浹�� ���
		for (int iApply = 0; iApply < m_iOrgiClosedIndex.size(); iApply++)
		{
			BNode* pNode = m_vMapQuadTree[iMap]->m_DrawPatchNodeList[m_iOrgiClosedIndex[iApply]];
			m_vMapQuadTree[iMap]->GetLodType(pNode);
			D3DXVECTOR3 v0, v1, v2;
			int indexsize = m_vMapQuadTree[iMap]->m_pdwLodIndexArray[pNode->m_dwLodLevel]->IndexList[pNode->m_dwLodType].size() / 3;
			for (int iFace = 0; iFace < indexsize; iFace++)
			{
				v0 = pNode->m_VertexList[m_vMapQuadTree[iMap]->m_pdwLodIndexArray[pNode->m_dwLodLevel]->IndexList[pNode->m_dwLodType][iFace * 3 + 0]].Position;
				v1 = pNode->m_VertexList[m_vMapQuadTree[iMap]->m_pdwLodIndexArray[pNode->m_dwLodLevel]->IndexList[pNode->m_dwLodType][iFace * 3 + 1]].Position;
				v2 = pNode->m_VertexList[m_vMapQuadTree[iMap]->m_pdwLodIndexArray[pNode->m_dwLodLevel]->IndexList[pNode->m_dwLodType][iFace * 3 + 2]].Position;
				if (m_Select.CheckPick(v0, v1, v2)) // �浹������ �Ǻ� �ƴٸ�
				{
					m_MapObjMgr[m_vSelectedObjInfo[0].m_iObjType]->m_InstanceInfo[m_vSelectedObjInfo[0].m_iObjNum].Translate = m_Select.CrashPoint;
					return true;
				}
			}
		}
	}
	return false;
}
/*������Ʈ �����Լ�*/
bool Sample::GetMapObjectPosition()
{
	m_Select.SetMatrix(NULL, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj); //��ǥ�� ��ġ + ���콺 ���� �޾ƿ���
	for (int iMap = 0; iMap < m_vMapQuadTree.size(); iMap++)
	{
		for (int iCheckNode = 0; iCheckNode < m_vMapQuadTree[iMap]->m_DrawPatchNodeList.size(); iCheckNode++)
		{
			BNode* pNode = m_vMapQuadTree[iMap]->m_DrawPatchNodeList[iCheckNode];
			if (m_Select.IntersectBox(&pNode->m_tBox))
			{
				int iIndex = iCheckNode;
				m_viClosedIndex.push_back(iIndex);
			}
		}
		if (!m_viClosedIndex.size())
		{
			return false;
		}

		/*�浹�� ó���� ������ �Ÿ��� ���� ������� ����*/
		vector<int> m_iOrgiClosedIndex;
		while (m_viClosedIndex.size())
		{
			float PLength = 99999.9f;
			int iApplyIndex = 0;
			for (int iCheckNode = 0; iCheckNode < m_viClosedIndex.size(); iCheckNode++)
			{
				D3DXVECTOR3 Pvec = m_vMapQuadTree[iMap]->m_DrawPatchNodeList[m_viClosedIndex[iCheckNode]]->m_tBox.vCenter - m_Select.m_Ray.vOrigin;
				float BLength = D3DXVec3Length(&Pvec);
				if (PLength > BLength)
				{
					iApplyIndex = iCheckNode;
				}
			}
			m_iOrgiClosedIndex.push_back(m_viClosedIndex[iApplyIndex]);
			vector<int>::iterator itor = m_viClosedIndex.begin();
			itor += iApplyIndex;
			m_viClosedIndex.erase(itor);
		}

		// m_iOrgiClosedIndex = ����� ������� ���ĵ� �浹�� ���
		for (int iApply = 0; iApply < m_iOrgiClosedIndex.size(); iApply++)
		{
			BNode* pNode = m_vMapQuadTree[iMap]->m_DrawPatchNodeList[m_iOrgiClosedIndex[iApply]];
			m_vMapQuadTree[iMap]->GetLodType(pNode);
			D3DXVECTOR3 v0, v1, v2;
			int indexsize = m_vMapQuadTree[iMap]->m_pdwLodIndexArray[pNode->m_dwLodLevel]->IndexList[pNode->m_dwLodType].size() / 3;
			for (int iFace = 0; iFace < indexsize; iFace++)
			{
				v0 = pNode->m_VertexList[m_vMapQuadTree[iMap]->m_pdwLodIndexArray[pNode->m_dwLodLevel]->IndexList[pNode->m_dwLodType][iFace * 3 + 0]].Position;
				v1 = pNode->m_VertexList[m_vMapQuadTree[iMap]->m_pdwLodIndexArray[pNode->m_dwLodLevel]->IndexList[pNode->m_dwLodType][iFace * 3 + 1]].Position;
				v2 = pNode->m_VertexList[m_vMapQuadTree[iMap]->m_pdwLodIndexArray[pNode->m_dwLodLevel]->IndexList[pNode->m_dwLodType][iFace * 3 + 2]].Position;
				if (m_Select.CheckPick(v0, v1, v2)) // �浹������ �Ǻ� �ƴٸ�
				{
					m_MapObjMgr[m_CreateObjType[0]]->m_iInstanceCount++;
					m_MapObjMgr[m_CreateObjType[0]]->InitInstanceBuffer(m_MapObjMgr[m_CreateObjType[0]]->m_iInstanceCount, m_CreateObjType[0]);
					m_MapObjMgr[m_CreateObjType[0]]->m_InstanceInfo[m_MapObjMgr[m_CreateObjType[0]]->m_iInstanceCount - 1].Translate = m_Select.CrashPoint;
					return true;
				}
			}
		}
	}
	return false;
}
#pragma endregion

#pragma region ������ ���� �Լ�

/*������ ���� �Լ�*/
void Sample::RenderShadow()
{
	m_pImmediateContext->OMSetDepthStencilState(BDxState::g_pDSSDepthEnable, 0x01);
	m_pImmediateContext->OMSetBlendState(BDxState::g_pAlphaBlend, 0, 0xffffffff);
	m_pImmediateContext->RSSetState(BDxState::g_pRSBackCullSolid);
	m_pImmediateContext->RSSetState(BDxState::g_pRSSlopeScaledDepthBias);
	for (int iMapObj = 0; iMapObj < m_MapObjMgr.size(); iMapObj++)
	{
		if (m_MapObjMgr[iMapObj]->m_InstanceInfo.size())
		{
			m_MapObjMgr[iMapObj]->SetCamera(m_Shadow.m_matShadowView, m_Shadow.m_matShadowProj);
			m_pImmediateContext->VSSetShader(m_Shadow.m_pShadowVS, NULL, 0);
			m_pImmediateContext->PSSetShader(NULL, NULL, 0); // NULL
			m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_MapObjMgr[iMapObj]->m_pConstantBuffer);
			m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			m_pImmediateContext->IASetInputLayout(m_RenderSettingASE.m_pVertexLayout);		// ����ȭ�� �ڵ�
			//m_pImmediateContext->IASetInputLayout(m_MapObjMgr[iMapObj]->m_pVertexLayout); 
			// ���̽��ٽ� ���۸� ���ҽ��� �����ϸ� �Ǳ� ������ �Ƚ����̴��� ������� �ʾƵ� �ȴ�.
			m_MapObjMgr[iMapObj]->DrawInstance(m_pImmediateContext, m_MapObjMgr[iMapObj], m_MapObjMgr[iMapObj]->m_InstanceInfo.size());
		}
	}
}
#pragma endregion



Sample::Sample()
{
}
Sample::~Sample()
{
}