#include "BMapLOD.h"

//====================================================================================//
/*Update, SetLOD를 통해 기본 LOD변수 세팅 후 InitLevelOrder를 통해 레벨을 생성합니다.*/
/*GetExpansionRatio의 인자값을 통해 LOD 적용 방식 변화가 가능합니다.*/
/*적용방식은 중점으로부터의 거리, 최대 최소값, 카메라와 중점의 거리비를 통한 비율로 결정됩니다. */
//====================================================================================//


#pragma region LOD 생성함수
/*메인 카메라, 디바이스 세팅함수*/
void BMapLOD::Update(ID3D11Device* pd3dDevice, BCamera* pCamera)
{
	m_pd3dDevice = pd3dDevice;
	m_pCamera = pCamera;
}
/*LOD 레벨 세팅함수*/
void	BMapLOD::SetLOD(DWORD dwWidth, int iNumDepth)
{
	m_iNumCell = (dwWidth - 1) / pow(2.0f, (float)iNumDepth); // 트리의 레벨에 따른 총 셀의 갯수
	m_iPatchLodCount = (log((float)m_iNumCell) / log(2.0f)); // LOD레벨 갯수
	if (m_iPatchLodCount > 2)
	{
		m_iPatchLodCount = 2; // 0 , 1 , 2 단계의 3개의 LOD생성
	}
}
/*노드 레벨리스트 생성함수*/
void BMapLOD::InitLevelOrder(BNode* pRootNode, int iMaxDepth)
{
	m_LevelList.resize(iMaxDepth + 1);
	for (int iLevel = 1; iLevel < iMaxDepth + 1; iLevel++)
	{
		m_LevelList[iLevel].resize(pow(4.0f, iLevel));
	}
	m_LevelList[0].push_back(pRootNode);
}
#pragma endregion

#pragma region 노드 LOD 레벨, 타입 반환함수
/*해당 노드의 적용 LOD 레벨 반환함수*/
DWORD BMapLOD::GetLodSubIndex(BNode* pNode)
{
	int iNumPatchIndex = m_iNumCell;
	float fRatio = GetExpansionRatio(pNode->m_tBox.vCenter);
	DWORD dwCurrentRatio = fRatio * m_iPatchLodCount;
	DWORD dwMaxRatio = fRatio * m_iPatchLodCount + 0.5f;
	DWORD dwMinRatio = fRatio * m_iPatchLodCount - 0.5f;

	// 문턱효과
	if (m_bThresHoldValue)
	{
		if (pNode->m_dwLodLevel < dwCurrentRatio)
		{
			if (pNode->m_dwLodLevel < dwMinRatio)
			{
				pNode->m_dwLodLevel = dwCurrentRatio;
			}
		}
		else if (pNode->m_dwLodLevel > dwCurrentRatio)
		{
			if (pNode->m_dwLodLevel > dwMaxRatio)
			{
				pNode->m_dwLodLevel = dwCurrentRatio;
			}
		}
	}
	else
	{
		pNode->m_dwLodLevel = dwCurrentRatio;
	}
	if (m_iPatchLodCount < pNode->m_dwLodLevel)
	{
		pNode->m_dwLodLevel -= 1;
	}
	return pNode->m_dwLodLevel;
}
/*해당 노드의 적용 LOD 타입 반환함수*/
DWORD	BMapLOD::GetLodType(BNode* pNode)
{
	if (pNode->m_NeighborList.size() <= 0) return 0;
	DWORD dwType = 0;
	if (pNode->m_NeighborList[0] && pNode->m_NeighborList[0]->m_dwLodLevel < pNode->m_dwLodLevel) dwType += 1;
	if (pNode->m_NeighborList[1] && pNode->m_NeighborList[1]->m_dwLodLevel < pNode->m_dwLodLevel) dwType += 4;
	if (pNode->m_NeighborList[2] && pNode->m_NeighborList[2]->m_dwLodLevel < pNode->m_dwLodLevel) dwType += 8;
	if (pNode->m_NeighborList[3] && pNode->m_NeighborList[3]->m_dwLodLevel < pNode->m_dwLodLevel) dwType += 2;

	pNode->m_dwLodType = dwType; // == 사용해야 하는 인덱스 버퍼의 번호
	return dwType;
}
#pragma endregion

#pragma region LOD 비율 반환함수
/*중점으로부터의 반지름*/
float	BMapLOD::GetExpansionRatio(D3DXVECTOR3 vCenter, float fRadius)
{
	float fRatio = 0.0f;

	D3DXVECTOR3 vCenterViewPos, vSideViewPos;
	vSideViewPos = vCenter + (m_pCamera->m_vSide * fRadius);
	D3DXVec3TransformCoord(&vCenterViewPos, &vCenter, &m_pCamera->m_matView);
	D3DXVec3TransformCoord(&vSideViewPos, &vSideViewPos, &m_pCamera->m_matView);

	D3DXVECTOR3 vCenterProjPos, vSideProjPos;
	D3DXVec3TransformCoord(&vCenterProjPos, &vCenterViewPos, &m_pCamera->m_matProj);
	D3DXVec3TransformCoord(&vSideProjPos, &vSideViewPos, &m_pCamera->m_matProj);

	D3DXVec3Normalize(&vCenterProjPos, &vCenterProjPos);
	D3DXVECTOR2 vA = D3DXVECTOR2(vCenterProjPos.x * 0.5f + 0.5f, vCenterProjPos.y * 0.5f + 0.5f);
	D3DXVECTOR2 vB = D3DXVECTOR2(vSideProjPos.x * 0.5f + 0.5f, vSideProjPos.y * 0.5f + 0.5f);

	float fDistance = D3DXVec3Length(&(m_pCamera->m_vPos - vCenter));
	if (fDistance > 2000.0f)
	{
		return 1.0f;
	}
	fRatio = 1.0f - fDistance / 2000.0f;
	return fRatio;
}
/*최대 최소값*/
float	BMapLOD::GetExpansionRatio(D3DXVECTOR3 vMax, D3DXVECTOR3 vMin)
{
	float fRatio = 0.0f;

	D3DXVECTOR3 vCenterViewPos, vSideViewPos;
	D3DXVec3TransformCoord(&vCenterViewPos, &vMax, &m_pCamera->m_matView);
	D3DXVec3TransformCoord(&vSideViewPos, &vMin, &m_pCamera->m_matView);

	D3DXVECTOR3 vCenterProjPos, vSideProjPos;
	D3DXVec3TransformCoord(&vCenterProjPos, &vCenterViewPos, &m_pCamera->m_matProj);
	D3DXVec3TransformCoord(&vSideProjPos, &vSideViewPos, &m_pCamera->m_matProj);

	D3DXVECTOR2 vA = D3DXVECTOR2(vCenterProjPos.x * 0.5f + 0.5f, vCenterProjPos.y * 0.5f + 0.5f);
	D3DXVECTOR2 vB = D3DXVECTOR2(vSideProjPos.x * 0.5f + 0.5f, vSideProjPos.y * 0.5f + 0.5f);

	fRatio = D3DXVec3Length(&(vCenterProjPos - vSideProjPos));
	return fRatio;
}
/*카메라의 위치로부터 중점까지의 거리*/
float	BMapLOD::GetExpansionRatio(D3DXVECTOR3 vCenter)
{
	D3DXVECTOR3 DistanceCamera = m_pCamera->m_vPos;
	D3DXVECTOR3 DistancevCenter = vCenter;
	DistanceCamera.y = 0.0f;
	DistancevCenter.y = 0.0f;

	float fDistance = D3DXVec3Length(&(DistanceCamera - DistancevCenter));
	if (fDistance > 1000.0f)
	{
		return 1.0f;
	}
	float fRatio = fDistance / 1000.0f;
	return fRatio;
}
#pragma endregion

#pragma region 랜더 노드 리스트 생성함수
/*랜더 노드 리스트 생성함수*/
void BMapLOD::GetDrawPatchNode()
{
	for (int iNode = 0; iNode < m_DrawNodeList.size(); iNode++)
	{
		AddDrawPatchNode(m_DrawNodeList[iNode]);
	}
}
/*랜더 노드 리스트 생성함수(재귀, GetDrawPatchNode 내부 호출)*/
void BMapLOD::AddDrawPatchNode(BNode* pNode)
{
	if (pNode->m_isLeaf)
	{
		m_DrawPatchNodeList.push_back(pNode);
		GetLodSubIndex(pNode);
		return;
	}
	for (int iNode = 0; iNode < 4; iNode++)
	{
		AddDrawPatchNode(pNode->m_pChild[iNode]);
	}
}
#pragma endregion
 

BMapLOD::BMapLOD()
{
	m_fExpansionRatio = 0.1f;
	m_iNumCell = 1;
	m_iPatchLodCount = 0;
	m_pCamera = NULL;
	m_bThresHoldValue = true;
}
BMapLOD::~BMapLOD()
{
}
