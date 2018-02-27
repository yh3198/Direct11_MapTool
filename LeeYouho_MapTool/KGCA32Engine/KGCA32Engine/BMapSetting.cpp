#include "BMapSetting.h"

#pragma region 기본 구성 함수
void BMapSetting::Init(BCamera* MainCamera, ID3D11Device* m_pd3dDevice, ID3D11DeviceContext* pContext)
{
	m_pMainCamera = MainCamera;
	m_pImmediateContext = pContext;
}
void BMapSetting::Frame()
{
}
void BMapSetting::Render()
{
}
void BMapSetting::Release()
{
	for (int iAddress = 0; iAddress < m_MapAddress.size(); iAddress)
	{
		m_MapAddress[iAddress].clear();
	}
	m_MapAddress.clear();
	m_TileInfo.clear();
}
#pragma endregion

/*세부정보 세팅 맵 설정함수*/
void BMapSetting::SettingMap(BMapTileRender* vMapQuadTree)
{
	m_SelectedMap = vMapQuadTree;
}

#pragma region 맵 세부정보 세팅함수
/*맵 정보 세팅함수*/
void BMapSetting::MapInfoSetting()
{
	int MaxX = m_SelectedMap->m_pRootNode->m_tBox.vMax.x - m_SelectedMap->m_pRootNode->m_tBox.vMin.x;
	int MaxZ = m_SelectedMap->m_pRootNode->m_tBox.vMax.z - m_SelectedMap->m_pRootNode->m_tBox.vMin.z;

	m_MapAddress.resize(MaxZ);
	m_TileInfo.resize(MaxX*MaxZ);
	for (int ZCnt = 0; ZCnt < MaxZ; ZCnt++)
	{
		m_MapAddress[ZCnt].resize(MaxX);
	}
	SettingPositionInfo(m_SelectedMap->m_pRootNode, m_SelectedMap->m_pMap->m_iNumCellCols);
	BApply = false;
}
/*높이 정보 세팅함수(SettingPositionInfo 내부 호출)*/
void BMapSetting::SettingPositionInfo(BNode* pNode, int iTotalRow)
{
	if (pNode->m_pChild[0] != NULL)
	{
		for (int iNode = 0; iNode < 4; iNode++)
		{
			SettingPositionInfo(pNode->m_pChild[iNode], iTotalRow);
		}
	}
	else
	{
		int iChangePoint = m_SelectedMap->m_pRootNode->m_tBox.fExtent[0];
		int iVerCnt = 0;
		int iRowStart = ((pNode->m_tBox.vMax.z - iChangePoint) * -1.0f) / m_SelectedMap->m_pMap->m_fCellDistance;
		int iColStart = (pNode->m_tBox.vMin.x + iChangePoint) / m_SelectedMap->m_pMap->m_fCellDistance;
		int iRowEnd = ((pNode->m_tBox.vMin.z - iChangePoint) * -1.0f) / m_SelectedMap->m_pMap->m_fCellDistance; // +1;
		int iColEnd = (pNode->m_tBox.vMax.x + iChangePoint) / m_SelectedMap->m_pMap->m_fCellDistance; // +1;

		for (int z = iRowStart; z < iRowEnd; z++)
		{
			for (int x = iColStart; x < iColEnd; x++)
			{
				int iIndex = iTotalRow * z + x;
				m_MapAddress[z][x] = iIndex;
				m_TileInfo[iIndex].TileEdge[0] = pNode->m_VertexList[(z - iRowStart) * (iColEnd - iColStart + 1) + (x - iColStart)].Position;
				m_TileInfo[iIndex].TileEdge[1] = pNode->m_VertexList[(z - iRowStart) * (iColEnd - iColStart + 1) + (x - iColStart) + 1].Position;
				m_TileInfo[iIndex].TileEdge[2] = pNode->m_VertexList[((z - iRowStart) + 1) * (iColEnd - iColStart + 1) + (x - iColStart) + 0].Position;
				m_TileInfo[iIndex].TileEdge[3] = pNode->m_VertexList[((z - iRowStart) + 1) * (iColEnd - iColStart + 1) + (x - iColStart) + 1].Position;
				m_TileInfo[iIndex].TileCenter = (m_TileInfo[iIndex].TileEdge[0] + m_TileInfo[iIndex].TileEdge[1] + m_TileInfo[iIndex].TileEdge[2] + m_TileInfo[iIndex].TileEdge[3]) / 4.0f;
				
				D3DXVECTOR3 e0 = m_TileInfo[iIndex].TileEdge[1] - m_TileInfo[iIndex].TileEdge[0];
				D3DXVECTOR3 e1 = m_TileInfo[iIndex].TileEdge[2] - m_TileInfo[iIndex].TileEdge[0];

				D3DXVECTOR3 TempNormal;
				D3DXVec3Cross(&TempNormal, &e0, &e1);
				D3DXVec3Normalize(&TempNormal, &TempNormal);
				m_TileInfo[iIndex].TileNormal[0] = TempNormal;

				e0 = m_TileInfo[iIndex].TileEdge[1] - m_TileInfo[iIndex].TileEdge[2];
				e1 = m_TileInfo[iIndex].TileEdge[3] - m_TileInfo[iIndex].TileEdge[2];

				D3DXVec3Cross(&TempNormal, &e0, &e1);
				D3DXVec3Normalize(&TempNormal, &TempNormal);
				m_TileInfo[iIndex].TileNormal[1] = TempNormal;
			}									
		}										
	}											
	BApply = false;
}
/*맵 세부설정 세팅함수 (소리, 알파텍스쳐 이용)*/
void BMapSetting::SettingSoundInfo()
{
	//m_SelectedMap->ObjPosTexture
	//m_TileInfo
	HRESULT hr = S_OK;
	D3D11_MAPPED_SUBRESOURCE MappedFaceDest;

	if (SUCCEEDED(m_pImmediateContext->Map((ID3D11Resource*)m_SelectedMap->StagingTexTure, 0, D3D11_MAP_READ_WRITE, 0, &MappedFaceDest)))
	{
		BYTE* pDestBytes = (BYTE*)MappedFaceDest.pData;
		int iRange = (m_SelectedMap->desc.Width / 10) - 1;
		// 현재 맵의 알파텍스쳐는 10배 크기로 되어있기 때문에 10배 줄여준다.
		for (int y = 0; y < iRange; y++)
		{
			BYTE* pDest = pDestBytes;
			for (int x = 0; x < iRange; x++)
			{
				//
				m_TileInfo[m_MapAddress[y][x]].TileSound = 3;
				if (*pDest > 127)
				{
					m_TileInfo[m_MapAddress[y][x]].TileSound = 0;
				}
				*pDest++;
				if (*pDest > 127)
				{
					m_TileInfo[m_MapAddress[y][x]].TileSound = 1;
				}
				*pDest++;
				if (*pDest > 127)
				{
					m_TileInfo[m_MapAddress[y][x]].TileSound = 2;
				}
				*pDest++;
				*pDest++;
				pDest += 36; // 9칸을 넘어가야 하는데 각 색상이 4개씩 있으므로 36
			}
			pDestBytes += (MappedFaceDest.RowPitch * 10);
		}
		m_pImmediateContext->Unmap(m_SelectedMap->StagingTexTure, 0);
	}
}
/*맵 세부정보 세팅함수 (오브젝트)*/
void BMapSetting::SettingObjInfo()
{
	//m_SelectedMap->ObjPosTexture
	//m_TileInfo
	HRESULT hr = S_OK;
	D3D11_MAPPED_SUBRESOURCE MappedFaceDest;

	if (SUCCEEDED(m_pImmediateContext->Map((ID3D11Resource*)m_SelectedMap->StagingObjPosTexture, 0, D3D11_MAP_READ_WRITE, 0, &MappedFaceDest)))
	{
		BYTE* pDestBytes = (BYTE*)MappedFaceDest.pData;
		int iRange = m_SelectedMap->Objdesc.Width - 1;
		for (int y = 0; y < iRange; y++)
		{
			BYTE* pDest = pDestBytes;
			for (int x = 0; x < iRange; x++)
			{
				//
				if (*pDest > 0)
					m_TileInfo[m_MapAddress[y][x]].TileMonsterRespawn = true;
				else
					m_TileInfo[m_MapAddress[y][x]].TileMonsterRespawn = false;
				*pDest++;
				if (*pDest > 0)
					m_TileInfo[m_MapAddress[y][x]].TileMoveUnable = true;
				else
					m_TileInfo[m_MapAddress[y][x]].TileMoveUnable = false;
				*pDest++;
				if (*pDest > 0)
					m_TileInfo[m_MapAddress[y][x]].TileiTemRespawn = true;
				else
					m_TileInfo[m_MapAddress[y][x]].TileiTemRespawn = false;
				*pDest++;
				*pDest++;
			}
			pDestBytes += MappedFaceDest.RowPitch;
		}
		m_pImmediateContext->Unmap(m_SelectedMap->StagingObjPosTexture, 0);
	}
}
#pragma endregion


BMapSetting::BMapSetting()
{
	BLastCheck = false;
	BMapSettingMode = true;
	BApply = true;
}
BMapSetting::~BMapSetting()
{
}