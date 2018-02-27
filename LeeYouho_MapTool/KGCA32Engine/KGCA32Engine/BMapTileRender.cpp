#include "BMapTileRender.h"

#pragma region LOD ���� ����ü
BStaticData::BStaticData()
{
}
BStaticData::~BStaticData()
{
	for (int iList = 0; iList < IndexBufferList.size(); iList++)
	{
		IndexList[iList].clear();
	}
	IndexBufferList.clear();
	IndexList.clear();
}
#pragma endregion

#pragma region �⺻ ���� �Լ�
bool BMapTileRender::PreFrame()
{
	return true;
}
bool BMapTileRender::Frame(BCamera* pCamera)
{
	PreFrame();
	m_DrawPatchNodeList.clear();
	if (!BQuadTree::Frame(pCamera))return false;

	GetDrawPatchNode();

	// �� �ڵ忡�� �����ϴ� �ε��� ����� �ǽð� �ε�������Ʈ�� �����ϴ� ������� �̸� �����ص� �ε������۸� ������� �ʽ��ϴ�.
	// ���� �������� �����ص� ������ �ε�������Ʈ�� ���� �Ѱ��� ū ���۸� ����� �������� �ϰ� �˴ϴ�.
	m_iNumFace = 0;
	/*�̿������� LOD�������̰� 2�̻��϶� stop*/
	for (int iNode = 0; iNode < m_DrawPatchNodeList.size(); iNode++)
	{
		for (int iNeighbor = 0; iNeighbor < m_DrawPatchNodeList[iNode]->m_NeighborList.size(); iNeighbor++)
		{
			if (m_DrawPatchNodeList[iNode]->m_NeighborList[iNeighbor])
			{
				DWORD dwValue = abs((float)(m_DrawPatchNodeList[iNode]->m_NeighborList[iNeighbor]->m_dwLodLevel - m_DrawPatchNodeList[iNode]->m_dwLodLevel));
				if (dwValue > 1)
				{
      					int LevelSecondDifferent = 0;
				}
			}
		}
		GetLodType(m_DrawPatchNodeList[iNode]);
		m_iNumFace += UpdateStaticIndexList(m_DrawPatchNodeList[iNode], m_iNumFace * 3, m_DrawPatchNodeList[iNode]->m_dwLodLevel);
	}
	
	return true;
}
bool BMapTileRender::Render(ID3D11DeviceContext* pContext)
{
	m_pMap->PreRender(pContext);
	// �����ؽ��Ŀ� ���� �̹��� ���
	if (m_pSplattingSRV.size())
	{
		for (int iMap = 0; iMap < 4; iMap++)
		{
			pContext->PSSetShaderResources((iMap + 2), 1, m_pSplattingSRV[iMap].GetAddressOf()); // �ؽ��� ����
		}
	}
	// �����ؽ��� ���
	if (SpreatingSRV.Get())
	{
		pContext->PSSetShaderResources(1, 1, SpreatingSRV.GetAddressOf()); // �ؽ��� ����
	}
	PostRender(pContext);
	return true;
}
bool BMapTileRender::PostRender(ID3D11DeviceContext* pContext)
{
	pContext->UpdateSubresource(m_pMap->m_pConstantBuffer, 0, NULL, &m_pMap->m_cbData, 0, 0);
	UINT stride = sizeof(PNCT_Vertex);		 // ���ؽ� �ϳ� ��ü�� ũ��
	UINT offset = 0;						 // ���� ������ġ
	for (int iNode = 0; iNode < m_DrawPatchNodeList.size(); iNode++)
	{
		BNode* pNode = m_DrawPatchNodeList[iNode];
		GetLodType(pNode);
		pContext->IASetVertexBuffers(0, 1, pNode->m_pVertexBuffer.GetAddressOf(), &stride, &offset);
		pContext->IASetIndexBuffer(m_pdwLodIndexArray[pNode->m_dwLodLevel]->IndexBufferList[pNode->m_dwLodType].Get(), DXGI_FORMAT_R32_UINT, 0);
		pContext->UpdateSubresource(m_pMap->m_pConstantBuffer, 0, NULL, &m_pMap->m_cbData, 0, 0);
		pContext->DrawIndexed(m_pdwLodIndexArray[pNode->m_dwLodLevel]->IndexList[pNode->m_dwLodType].size(), 0, 0);
	}
	return true;
}
#pragma endregion

#pragma region �� ���� �����Լ�
/*�� ���� �� Ʈ�� ���һ��� �Լ�*/
bool BMapTileRender::Build(BMap* pMap, DWORD dwWidth, DWORD dwHeight, ID3D11DeviceContext* pImmediateContext)
{
	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;
	m_pMap = (BNMap*)pMap;
	m_iNumFace = (dwWidth - 1) *(dwHeight - 1) * 2;
	m_bUsedIndexList = true;
	m_pImmediateContext = pImmediateContext;

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

		// ���� �ε��� ���� ����
		//int iNumTileRender = pow(4.0f, m_iMaxDepth);
		//m_iNumCell = (dwWidth - 1) / pow(2.0f, (float)m_iMaxDepth);
		//m_IndexList.resize(m_iNumCell * m_iNumCell * 2 * 3); // 2�� * ���ý� 3�� 

		//int iIndex = 0;
		//for (DWORD dwRow = 0; dwRow < m_iNumCell; dwRow++)
		//{
		//	for (DWORD dwCol = 0; dwCol < m_iNumCell; dwCol++)
		//	{
		//		// 0  1      4
		//		// 2      3  5
		//		DWORD dwNextRow = dwRow + 1;
		//		DWORD dwNextCol = dwCol + 1;
		//		m_IndexList[iIndex++] = (dwRow * (m_iNumCell + 1) + dwCol);
		//		m_IndexList[iIndex++] = (dwRow * (m_iNumCell + 1) + dwNextCol);
		//		m_IndexList[iIndex++] = (dwNextRow * (m_iNumCell + 1) + dwCol);
		//		m_IndexList[iIndex++] = (dwNextRow * (m_iNumCell + 1) + dwCol);
		//		m_IndexList[iIndex++] = (dwRow * (m_iNumCell + 1) + dwNextCol);
		//		m_IndexList[iIndex++] = (dwNextRow * (m_iNumCell + 1) + dwNextCol);
		//	}
		//}
		//// ���� �ε��� ���� ����
		//m_pTileIndexBuffer.Attach(CreateIndexBuffer(m_pd3dDevice, &m_IndexList.at(0), m_IndexList.size(), sizeof(DWORD)));
		return true;
	}
	return false;
}
/*Ʈ�� ��� �����Լ� (Build ���� ȣ��)*/
BNode* BMapTileRender::CreateNode(BNode* pParentNode, DWORD dwTL, DWORD dwTR, DWORD dwBL, DWORD dwBR)
{
	BNode* pNode = 0;
	S_NEW(pNode, BNode);

	pNode->m_isLeaf = FALSE;

	// vCorner, m_Child �� �迭�̶� reserve �ʿ����
	pNode->m_CornerIndex.reserve(4);

	pNode->m_CornerIndex.push_back(dwTL);
	pNode->m_CornerIndex.push_back(dwTR);
	pNode->m_CornerIndex.push_back(dwBL);
	pNode->m_CornerIndex.push_back(dwBR);

	ComputeBoundingBox(pNode);

	ldiv_t divValue = ldiv(dwTL, m_dwWidth); // ?
	pNode->m_dwPositionIndex[0] = divValue.rem / (dwTR - dwTL);  // ������ -> X
	pNode->m_dwPositionIndex[1] = divValue.quot / (dwTR - dwTL); // �� -> Y

	if (pParentNode)
	{
		pNode->m_dwDepth = pParentNode->m_dwDepth + 1;
		if (m_iMaxDepth < pNode->m_dwDepth)
		{
			m_iMaxDepth = pNode->m_dwDepth;
		}
	}
	return pNode;
}
/*������ # Ʈ�� ���� �Լ� (����Ʈ�� ������ ȣ��)*/
bool BMapTileRender::SubDivide(BNode* pNode)
{
	if (pNode == NULL) return false;

	// ���� ����� ���� ũ�⸦ ���
	DWORD dwOffsetX = (pNode->m_CornerIndex[1] - pNode->m_CornerIndex[0]);
	DWORD dwOffsetZ = ((pNode->m_CornerIndex[2] - pNode->m_CornerIndex[0]) / m_dwWidth);

	DWORD dwWidthSplit = CheckSize(dwOffsetX);
	DWORD dwHeightSplit = CheckSize(dwOffsetZ);

	// �ִ� ���� �ѵ��� ����
	if (m_iMaxDepthLimit <= pNode->m_dwDepth)
	{
		pNode->m_isLeaf = TRUE;
		CreateVertexList(pNode);
		ComputeBoundingBox(pNode);
		//CreateIndexList(pNode);
		return false;
	}
	// 4��� �� �� ������ ���̻� �������� �ʴ´�.
	if (dwWidthSplit < m_iMinSplitDistance || dwHeightSplit < m_iMinSplitDistance)
	{
		pNode->m_isLeaf = TRUE;
		CreateVertexList(pNode);
		ComputeBoundingBox(pNode);
		//CreateIndexList(pNode);
		return false;
	}
	float fWidthSplit = (pNode->m_vCorner[1].x - pNode->m_vCorner[0].x) / 2;
	float fHeightSplit = (pNode->m_vCorner[0].z - pNode->m_vCorner[2].z) / 2;
	//�ڽ� ��尡 ������ ����ũ�⺸�� �۴ٸ� ���̻� �������� �ʴ´�.
	if (fWidthSplit < m_fMinDivideSize || fHeightSplit < m_fMinDivideSize)
	{
		pNode->m_isLeaf = TRUE;
		CreateVertexList(pNode);
		ComputeBoundingBox(pNode);
		//CreateIndexList(pNode);
		return false;
	}
	DWORD dwSplitCenter = (pNode->m_CornerIndex[0] + dwWidthSplit) + (dwHeightSplit * m_dwWidth);

	DWORD dwEdgeCenter[4];
	dwEdgeCenter[0] = pNode->m_CornerIndex[0] + dwWidthSplit;
	dwEdgeCenter[1] = pNode->m_CornerIndex[1] + dwHeightSplit * m_dwWidth;
	dwEdgeCenter[2] = pNode->m_CornerIndex[2] + dwWidthSplit;
	dwEdgeCenter[3] = pNode->m_CornerIndex[0] + dwHeightSplit * m_dwWidth;

	pNode->m_pChild[0] = CreateNode(pNode, pNode->m_CornerIndex[0], dwEdgeCenter[0], dwEdgeCenter[3], dwSplitCenter);
	pNode->m_pChild[1] = CreateNode(pNode, dwEdgeCenter[0], pNode->m_CornerIndex[1], dwSplitCenter, dwEdgeCenter[1]);
	pNode->m_pChild[2] = CreateNode(pNode, dwEdgeCenter[3], dwSplitCenter, pNode->m_CornerIndex[2], dwEdgeCenter[2]);
	pNode->m_pChild[3] = CreateNode(pNode, dwSplitCenter, dwEdgeCenter[1], dwEdgeCenter[2], pNode->m_CornerIndex[3]);

	return true;
}
/*��庰 �ٿ���ڽ� �����Լ� (CreateNode ����ȣ��)*/
void BMapTileRender::ComputeBoundingBox(BNode* pNode)
{
	D3DXVECTOR2 vHeight = GetHeightFromNode(pNode->m_CornerIndex[0], pNode->m_CornerIndex[1], pNode->m_CornerIndex[2], pNode->m_CornerIndex[3]);

	pNode->m_vCorner[0] = m_pMap->m_VertexList[pNode->m_CornerIndex[0]].Position;
	pNode->m_vCorner[1] = m_pMap->m_VertexList[pNode->m_CornerIndex[1]].Position;
	pNode->m_vCorner[2] = m_pMap->m_VertexList[pNode->m_CornerIndex[2]].Position;
	pNode->m_vCorner[3] = m_pMap->m_VertexList[pNode->m_CornerIndex[3]].Position;

	pNode->m_tBox.vMax = m_pMap->m_VertexList[pNode->m_CornerIndex[1]].Position;
	pNode->m_tBox.vMin = m_pMap->m_VertexList[pNode->m_CornerIndex[2]].Position;
	pNode->m_tBox.vMax.y = vHeight.x;
	pNode->m_tBox.vMin.y = vHeight.y;

	pNode->m_tBox.vCenter = (pNode->m_tBox.vMax + pNode->m_tBox.vMin) / 2.0f;
	pNode->m_tBox.fExtent[0] = pNode->m_tBox.vMax.x - pNode->m_tBox.vCenter.x;
	pNode->m_tBox.fExtent[1] = pNode->m_tBox.vMax.y - pNode->m_tBox.vCenter.y;
	pNode->m_tBox.fExtent[2] = pNode->m_tBox.vMax.z - pNode->m_tBox.vCenter.z;
}
/*�ٿ���ڽ� ���̰� ��ȯ�Լ� (ComputeBoundingBox ����ȣ��)*/
D3DXVECTOR2 BMapTileRender::GetHeightFromNode(DWORD dwTL, DWORD dwTR, DWORD dwBL, DWORD dwBR)
{
	assert(m_pMap);

	DWORD dwStartRow = dwTL / m_dwWidth; // ������
	DWORD dwEndRow = dwBL / m_dwWidth;  // ����

	DWORD dwStartCol = dwTL % m_dwWidth; // ���ۿ�
	DWORD dwEndCol = dwTR % m_dwWidth; // ����

	D3DXVECTOR2 vHeight;
	vHeight.x = -999999.0f;
	vHeight.y = 999999.0f;

	for (DWORD dwRow = dwStartRow; dwRow < dwEndRow; dwRow++)
	{
		for (DWORD dwCol = dwStartCol; dwCol < dwEndCol; dwCol++)
		{
			if (m_pMap->m_VertexList[dwRow* m_dwWidth + dwCol].Position.y > vHeight.x)
			{
				vHeight.x = m_pMap->m_VertexList[dwRow * m_dwWidth + dwCol].Position.y;
			}
			if (m_pMap->m_VertexList[dwRow* m_dwWidth + dwCol].Position.y < vHeight.y)
			{
				vHeight.y = m_pMap->m_VertexList[dwRow * m_dwWidth + dwCol].Position.y;
			}
		}
	}
	return vHeight;
}
/*���ؽ� ����Ʈ ���� �Լ�(Build - SubDevide ���� ȣ��)*/
bool BMapTileRender::CreateVertexList(BNode* pNode)
{
	DWORD dwTL = pNode->m_CornerIndex[0];
	DWORD dwTR = pNode->m_CornerIndex[1];
	DWORD dwBL = pNode->m_CornerIndex[2];
	DWORD dwBR = pNode->m_CornerIndex[3];

	DWORD dwSize = ((dwTR - dwTL) + 1)*((dwTR - dwTL) + 1);
	pNode->m_VertexList.resize(dwSize);
	pNode->m_TangentList.resize(dwSize);

	DWORD dwStartRow = dwTL / m_dwWidth;
	DWORD dwEndRow = dwBL / m_dwWidth;

	DWORD dwStartCol = dwTL % m_dwWidth;
	DWORD dwEndCol = dwTR % m_dwWidth;

	DWORD iVertexIndex = 0;

	float MinX, MaxX, MinY, MaxY, MinZ, MaxZ;
	MinX = 99999.0f;
	MinY = 99999.0f;
	MinZ = 99999.0f;
	MaxX = -99999.0f;
	MaxY = -99999.0f;
	MaxZ = -99999.0f;

	for (DWORD dwRow = dwStartRow; dwRow <= dwEndRow; dwRow++)
	{
		for (DWORD dwCol = dwStartCol; dwCol <= dwEndCol; dwCol++)
		{
			pNode->m_VertexList[iVertexIndex] = m_pMap->m_VertexList[dwRow*m_dwWidth + dwCol];
			pNode->m_TangentList[iVertexIndex++] = m_pMap->m_TangentList[dwRow*m_dwWidth + dwCol];
		}
	}
	pNode->m_pVertexBuffer.Attach(CreateVertexBuffer(m_pd3dDevice, &pNode->m_VertexList.at(0), pNode->m_VertexList.size(), sizeof(PNCT_Vertex)));
	pNode->m_pTangentBuffer.Attach(CreateVertexBuffer(m_pd3dDevice, &pNode->m_TangentList.at(0), pNode->m_TangentList.size(), sizeof(D3DXVECTOR3)));

	return true;
}
/*���ؽ� ���� ���� �Լ�(Build - SubDevide ���� ȣ��)*/
int	BMapTileRender::CreateIndexList(BNode* pNode)
{
	vector<DWORD> Indexlist;
	DWORD dwTL = pNode->m_CornerIndex[0];
	DWORD dwTR = pNode->m_CornerIndex[1];
	DWORD dwBL = pNode->m_CornerIndex[2];
	DWORD dwBR = pNode->m_CornerIndex[3];

	DWORD dwSize = (dwTR - dwTL) * (dwTR - dwTL) * 2 * 3;
	pNode->m_IndexList.resize(dwSize);

	int dwCurrentIndex = 0;

	DWORD dwStartRow = dwTL / m_dwWidth;
	DWORD dwEndRow = dwBL / m_dwWidth;
	DWORD dwStartCol = dwTL % m_dwWidth;
	DWORD dwEndCol = dwTR % m_dwWidth;

	for (DWORD dwRow = dwStartRow; dwRow < dwEndRow; dwRow++)
	{
		for (DWORD dwCol = dwStartCol; dwCol < dwEndCol; dwCol++)
		{
			// 0 1   4
			// 2   3 5
			DWORD dwNextRow = dwRow + 1;
			DWORD dwNextCol = dwCol + 1;
			pNode->m_IndexList[dwCurrentIndex++] = dwRow * m_dwWidth + dwCol;
			pNode->m_IndexList[dwCurrentIndex++] = dwRow * m_dwWidth + dwNextCol;
			pNode->m_IndexList[dwCurrentIndex++] = dwNextRow * m_dwWidth + dwCol;
			pNode->m_IndexList[dwCurrentIndex++] = dwNextRow * m_dwWidth + dwCol;
			pNode->m_IndexList[dwCurrentIndex++] = dwRow * m_dwWidth + dwNextCol;
			pNode->m_IndexList[dwCurrentIndex++] = dwNextRow * m_dwWidth + dwNextCol;
		}
	}
	pNode->m_pIndexBuffer.Attach(CreateIndexBuffer(m_pd3dDevice, &pNode->m_IndexList.at(0), pNode->m_IndexList.size(), sizeof(DWORD)));

	pNode->m_BoxIB.left = m_dstibOffset;
	pNode->m_BoxIB.right = m_dstibOffset + pNode->m_IndexList.size() * sizeof(DWORD);
	pNode->m_BoxIB.top = 0;
	pNode->m_BoxIB.bottom = 1;
	pNode->m_BoxIB.front = 0;
	pNode->m_BoxIB.back = 1;

	m_pImmediateContext->UpdateSubresource(m_pIndexBuffer.Get(), 0, &pNode->m_BoxIB, (void*)&pNode->m_IndexList.at(0), 0, 0);
	pNode->m_iBeginIB = m_ibOffset;
	m_ibOffset += pNode->m_IndexList.size();
	m_dstibOffset = pNode->m_BoxIB.right;

	return Indexlist.size() / 3;
}
/*�ε��� ����Ʈ ���� �Լ�(Build ���� ȣ��)*/
ID3D11Buffer* BMapTileRender::CreateVertexBuffer(ID3D11Device* pd3dDevice, void *indices, UINT iNumIndex, UINT iSize)
{
	HRESULT hr = S_OK;
	ID3D11Buffer* pBuffer = nullptr;

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;						// [�޸� �Ҵ� ��ġ] GPU �޸𸮿� �Ҵ��ض�
	bd.ByteWidth = iNumIndex * iSize;	// [�޸� �Ҵ� ũ��] �� X���� �Ҵ� �� ���̱� ������ *X ( ���� �þ�� ���� �� ���̴� )
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;			// [�޸� ��� �뵵] ���ؽ� ���ۿ����� ����ϰڴ�.
	bd.CPUAccessFlags = 0;								// CPU�� ���ʹ� �׼��� ���� �ʰڴ�.
	bd.MiscFlags = 0;									// �߰� ���� ����

	D3D11_SUBRESOURCE_DATA InitData;			// �ҽ� ���ҽ� �ʱ�ȭ�� �ʿ��� ������ ����
	InitData.pSysMem = indices;//(void*)&m_VertexList.at(0);				// �ش� ���ؽ��� �ҽ��� ����ϰڴ�.
	hr = m_pd3dDevice->CreateBuffer(&bd, &InitData, &pBuffer);
	if (hr != S_OK)
	{
		return nullptr;
	}
	return pBuffer;
}
/*�ε��� ���� ���� �Լ�(Build ���� ȣ��)*/
ID3D11Buffer* BMapTileRender::CreateIndexBuffer(ID3D11Device* pd3dDevice, void *indices, UINT iNumIndex, UINT iSize)
{
	HRESULT hr = S_OK;
	ID3D11Buffer* pBuffer = nullptr;
	// �ε��� ���� ����
	D3D11_BUFFER_DESC ibDesc;
	ibDesc.ByteWidth = iNumIndex * iSize;
	ibDesc.Usage = D3D11_USAGE_DEFAULT;
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER; // �ε��� ���۷� ����ϰڴٴ� ����
	ibDesc.CPUAccessFlags = 0;
	ibDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA ibInitData;
	ZeroMemory(&ibInitData, sizeof(D3D11_SUBRESOURCE_DATA));
	ibInitData.pSysMem = indices;//(void*)&m_IndexList.at(0);
								 //hr = pd3dDevice->CreateBuffer(&ibDesc, &ibInitData, &pBuffer);
	if (indices != NULL)
	{
		hr = pd3dDevice->CreateBuffer(&ibDesc, &ibInitData, &pBuffer);
	}
	else
	{
		hr = pd3dDevice->CreateBuffer(&ibDesc, NULL, &pBuffer);
	}
	if (hr != S_OK)
	{
		return nullptr;
	}
	return pBuffer;
}
#pragma endregion

#pragma region �� LOD ���� �Լ�
/*����� LOD �� ���� �Լ�*/
void BMapTileRender::SetRenderDepth(DWORD dwDepth)
{
	if (m_iMaxDepth <= dwDepth) m_iRenderDepth = -1;
	else m_iRenderDepth = dwDepth;
}
/*LOD �ε��� ����Լ�*/
bool BMapTileRender::ComputeStaticLodIndex(int iMaxCells)
{
	m_pdwLodIndexArray.reserve(m_iPatchLodCount + 1); // m_iPatchLodCount = ������ �ܰ��̹Ƿ� 0������ ���� ���� +1�� ���ش�.

													  // case : Level 0
	BStaticData* pStaticData = NULL;
	S_NEW(pStaticData, BStaticData);
	pStaticData->dwLevel = 0;
	pStaticData->IndexList.resize(1);

	for (DWORD dwRow = 0; dwRow < iMaxCells; dwRow++)
	{
		for (DWORD dwCol = 0; dwCol < iMaxCells; dwCol++)
		{
			// 0 1   4
			// 2   3 5
			DWORD dwNextRow = dwRow + 1;
			DWORD dwNextCol = dwCol + 1;
			pStaticData->IndexList[0].push_back(dwRow * (iMaxCells + 1) + dwCol);
			pStaticData->IndexList[0].push_back(dwRow * (iMaxCells + 1) + dwNextCol);
			pStaticData->IndexList[0].push_back(dwNextRow * (iMaxCells + 1) + dwCol);
			pStaticData->IndexList[0].push_back(dwNextRow * (iMaxCells + 1) + dwCol);
			pStaticData->IndexList[0].push_back(dwRow * (iMaxCells + 1) + dwNextCol);
			pStaticData->IndexList[0].push_back(dwNextRow * (iMaxCells + 1) + dwNextCol);
		}
	}
	m_pdwLodIndexArray.push_back(pStaticData);
	CreateLODIndexBuffer(pStaticData);

	//���� 1���� ~ ������ ��������
	for (DWORD dwLevel = 0; dwLevel < m_iPatchLodCount; dwLevel++)
	{
		//�ּ� ��ġ�� ũ�Ⱑ (0, 1 ,2) ���� 3���̱� ������ level = 0�� ��� ������ = 2 ���� �Ѵ�.
		DWORD dwOffset = pow(2.0f, float(dwLevel) + 1); // dwOffset = �� ���� �� ����
		DWORD dwNumCell = (iMaxCells / dwOffset);

		BStaticData* pStaticData = NULL;
		S_NEW(pStaticData, BStaticData);
		pStaticData->dwLevel = dwLevel + 1;
		pStaticData->IndexList.resize(16); // 16 = 8 4 2 1 �� �� 

										   /*������*/
		for (int iData = 0; iData < 16; iData++)
		{
			DWORD dwIndexCounter = GetIndexCounter(iData, dwNumCell);
			pStaticData->IndexList[iData].resize(dwIndexCounter);
			int iIndex = 0;

			for (DWORD dwRow = 0; dwRow < iMaxCells; dwRow += dwOffset)
			{
				for (DWORD dwCol = 0; dwCol < iMaxCells; dwCol += dwOffset)
				{
					DWORD dwTL = dwRow * (iMaxCells + 1) + dwCol;
					DWORD dwTR = dwTL + dwOffset;
					DWORD dwBL = dwOffset * (iMaxCells + 1) + dwTL;
					DWORD dwBR = dwBL + dwOffset;
					DWORD dwCP = (dwTL + dwBR) / 2;
					// �����Ͱ� ���ų�, ���� ���̰ų� , ������ ���̰ų�, �� ���ų�, �� �Ʒ����
					// 1 = �� 2 = ������ 4 = �Ʒ� 8 = ����
					if (iData != 0 && (dwRow == 0 || dwRow == ((dwNumCell - 1)*dwOffset) || dwCol == 0 || dwCol == ((dwNumCell - 1)*dwOffset)))
					{
						if ((iData & 1) && dwRow == 0)
						{
							DWORD dwUpperCenter = (dwTL + dwTR) / 2;
							pStaticData->IndexList[iData][iIndex++] = dwCP;
							pStaticData->IndexList[iData][iIndex++] = dwTL;
							pStaticData->IndexList[iData][iIndex++] = dwUpperCenter;
							pStaticData->IndexList[iData][iIndex++] = dwCP;
							pStaticData->IndexList[iData][iIndex++] = dwUpperCenter;
							pStaticData->IndexList[iData][iIndex++] = dwTR;
						}
						else
						{
							pStaticData->IndexList[iData][iIndex++] = dwCP;
							pStaticData->IndexList[iData][iIndex++] = dwTL;
							pStaticData->IndexList[iData][iIndex++] = dwTR;
						}
						if ((iData & 2) && (dwCol == (dwNumCell - 1)*dwOffset))
						{
							DWORD dwRightCenter = (dwTR + dwBR) / 2;
							pStaticData->IndexList[iData][iIndex++] = dwCP;
							pStaticData->IndexList[iData][iIndex++] = dwTR;
							pStaticData->IndexList[iData][iIndex++] = dwRightCenter;
							pStaticData->IndexList[iData][iIndex++] = dwCP;
							pStaticData->IndexList[iData][iIndex++] = dwRightCenter;
							pStaticData->IndexList[iData][iIndex++] = dwBR;
						}
						else
						{
							pStaticData->IndexList[iData][iIndex++] = dwCP;
							pStaticData->IndexList[iData][iIndex++] = dwTR;
							pStaticData->IndexList[iData][iIndex++] = dwBR;
						}
						if ((iData & 4) && (dwRow == (dwNumCell - 1)*dwOffset))
						{
							DWORD dwLowerCenter = (dwBR + dwBL) / 2;
							pStaticData->IndexList[iData][iIndex++] = dwCP;
							pStaticData->IndexList[iData][iIndex++] = dwBR;
							pStaticData->IndexList[iData][iIndex++] = dwLowerCenter;
							pStaticData->IndexList[iData][iIndex++] = dwCP;
							pStaticData->IndexList[iData][iIndex++] = dwLowerCenter;
							pStaticData->IndexList[iData][iIndex++] = dwBL;
						}
						else
						{
							pStaticData->IndexList[iData][iIndex++] = dwCP;
							pStaticData->IndexList[iData][iIndex++] = dwBR;
							pStaticData->IndexList[iData][iIndex++] = dwBL;
						}
						if ((iData & 8) && (dwCol == 0))
						{
							DWORD dwLeftCenter = (dwTL + dwBL) / 2;
							pStaticData->IndexList[iData][iIndex++] = dwCP;
							pStaticData->IndexList[iData][iIndex++] = dwBL;
							pStaticData->IndexList[iData][iIndex++] = dwLeftCenter;
							pStaticData->IndexList[iData][iIndex++] = dwCP;
							pStaticData->IndexList[iData][iIndex++] = dwLeftCenter;
							pStaticData->IndexList[iData][iIndex++] = dwTL;
						}
						else
						{
							pStaticData->IndexList[iData][iIndex++] = dwCP;
							pStaticData->IndexList[iData][iIndex++] = dwBL;
							pStaticData->IndexList[iData][iIndex++] = dwTL;
						}
					}
					else
					{
						pStaticData->IndexList[iData][iIndex++] = dwCP;
						pStaticData->IndexList[iData][iIndex++] = dwTL;
						pStaticData->IndexList[iData][iIndex++] = dwTR;

						pStaticData->IndexList[iData][iIndex++] = dwCP;
						pStaticData->IndexList[iData][iIndex++] = dwTR;
						pStaticData->IndexList[iData][iIndex++] = dwBR;

						pStaticData->IndexList[iData][iIndex++] = dwCP;
						pStaticData->IndexList[iData][iIndex++] = dwBR;
						pStaticData->IndexList[iData][iIndex++] = dwBL;

						pStaticData->IndexList[iData][iIndex++] = dwCP;
						pStaticData->IndexList[iData][iIndex++] = dwBL;
						pStaticData->IndexList[iData][iIndex++] = dwTL;
					}
				}
			}
		}
		CreateLODIndexBuffer(pStaticData);
		m_pdwLodIndexArray.push_back(pStaticData);
	}
	return true;
}
/*LOD �ε��� ���� �����Լ� */
HRESULT BMapTileRender::CreateLODIndexBuffer(BStaticData* pData)
{
	HRESULT hr = S_OK;
	pData->IndexBufferList.resize(pData->IndexList.size());
	for (int iData = 0; iData < pData->IndexList.size(); iData++)
	{
		if (pData->IndexList[iData].size() <= 0)
			break;
		pData->IndexBufferList[iData].Attach(CreateIndexBuffer(m_pd3dDevice, &pData->IndexList[iData].at(0), pData->IndexList[iData].size(), sizeof(DWORD)));
	}
	return hr;
}
/*LOD �ε��� ��ȯ�Լ�*/
DWORD BMapTileRender::GetIndexCounter(DWORD dwData, DWORD dwNumCell)
{
	// �� ���� = (�� ���� ���� * �� ���� ���� * 4) + (�� ���� ���� �� �� ���� ���� * dwLevelCount) * 3;
	// dwLevelCount�� 8 4 2 1 �������� ���� �߰� �� �� �ִ� ���̽�
	DWORD dwNumFaces = (dwNumCell * dwNumCell * 4);

	DWORD dwLevelCount = 0;
	if (dwData & 8) dwLevelCount++;
	if (dwData & 4) dwLevelCount++;
	if (dwData & 2) dwLevelCount++;
	if (dwData & 1) dwLevelCount++;

	dwNumFaces = (dwNumFaces + (dwNumCell * dwLevelCount));
	return dwNumFaces * 3;
}
#pragma endregion

#pragma region �������� ���� �Լ�
/*�������ÿ� �ؽ��� �����Լ�*/
HRESULT BMapTileRender::CreateSpreatingTexture()
{
	HRESULT hr = S_OK;
	desc.Width = m_pMap->m_iNumRows * m_pMap->m_fCellDistance* 10.0f;
	desc.Height = m_pMap->m_iNumCols * m_pMap->m_fCellDistance* 10.0f;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

	hr = m_pd3dDevice->CreateTexture2D(&desc, NULL, &SpreatingTexTure);

	desc.Usage = D3D11_USAGE_STAGING;
	desc.BindFlags = 0;
	desc.MiscFlags = 0;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;

	hr = m_pd3dDevice->CreateTexture2D(&desc, NULL, &StagingTexTure);


	Objdesc.Width = m_pMap->m_iNumRows * m_pMap->m_fCellDistance;
	Objdesc.Height = m_pMap->m_iNumCols * m_pMap->m_fCellDistance;
	Objdesc.MipLevels = 1;
	Objdesc.ArraySize = 1;
	Objdesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	Objdesc.SampleDesc.Count = 1;
	Objdesc.SampleDesc.Quality = 0;
	Objdesc.Usage = D3D11_USAGE_DEFAULT;
	Objdesc.CPUAccessFlags = 0;
	Objdesc.MiscFlags = 0;
	Objdesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

	hr = m_pd3dDevice->CreateTexture2D(&Objdesc, NULL, &ObjPosTexture);

	Objdesc.Usage = D3D11_USAGE_STAGING;
	Objdesc.BindFlags = 0;
	Objdesc.MiscFlags = 0;
	Objdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;

	hr = m_pd3dDevice->CreateTexture2D(&Objdesc, NULL, &StagingObjPosTexture);

	return hr;
}
/*�ؽ��� ���� �Լ�*/
void BMapTileRender::Spreating(ID3D11DeviceContext* pImmediateContext, B_BOX Area, D3DXVECTOR3 CrashPoint, float fRadius, int iColor)
{
	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
	D3DXVECTOR3 CrashDot = CrashPoint;
	CrashDot.x = CrashDot.x * desc.Width;
	CrashDot.y = 0.0f;
	CrashDot.z = CrashDot.z * desc.Height;
	B_BOX FixedArea;
	FixedArea.vMax.x = (int)(Area.vMax.x * desc.Width);
	FixedArea.vMax.z = (int)(Area.vMax.z * desc.Height);
	FixedArea.vMin.x = (int)(Area.vMin.x * desc.Width);
	FixedArea.vMin.z = (int)(Area.vMin.z * desc.Height);

	if (FixedArea.vMin.x < 0.0f)
	{
		FixedArea.vMin.x = 0.0f;
	}
	if (FixedArea.vMin.z < 0.0f)
	{
		FixedArea.vMin.z = 0.0f;
	}
	if (FixedArea.vMax.x > desc.Width)
	{
		FixedArea.vMax.x = desc.Width;
	}
	if (FixedArea.vMax.z > desc.Height)
	{
		FixedArea.vMax.z = desc.Height;
	}

	float CheckRadius = fRadius * 10.0f;
	if (SUCCEEDED(pImmediateContext->Map((ID3D11Resource*)StagingTexTure, 0, D3D11_MAP_READ_WRITE, 0, &MappedFaceDest)))
	{
		BYTE* pDestBytes = (BYTE*)MappedFaceDest.pData;
		pDestBytes = pDestBytes + MappedFaceDest.RowPitch * (UINT)FixedArea.vMin.z;
		for (int y = FixedArea.vMin.z; y < FixedArea.vMax.z; y++)
		{
			BYTE* pDest = pDestBytes;
			pDest = pDest + (UINT)(4 * FixedArea.vMin.x);
			for (int x = FixedArea.vMin.x; x < FixedArea.vMax.x; x++)
			{
				D3DXVECTOR3 Dot = D3DXVECTOR3(x, 0.0f, y);
				float fColorPower = D3DXVec3Length(&(Dot - CrashDot));
				if (CheckRadius >= fColorPower)
				{
					switch (iColor)
					{
					case 0:
						*pDest++ = 255;
						*pDest++ = 0;
						*pDest++ = 0;
						*pDest++ = 0;
						break;
					case 1:
						*pDest++ = 0;
						*pDest++ = 255;
						*pDest++ = 0;
						*pDest++ = 0;
						break;
					case 2:
						*pDest++ = 0;
						*pDest++ = 0;
						*pDest++ = 255;
						*pDest++ = 0;
						break;
					case 3:
						*pDest++ = 0;
						*pDest++ = 0;
						*pDest++ = 0;
						*pDest++ = 255;
						break;
					default:
						break;
					}
				}
				else
				{
					pDest += 4;
				}
			}
			pDestBytes += MappedFaceDest.RowPitch;
		}
		pImmediateContext->Unmap(StagingTexTure, 0);
		pImmediateContext->CopyResource(SpreatingTexTure, StagingTexTure);
		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		ZeroMemory(&SRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		SRVDesc.Format = desc.Format;
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRVDesc.Texture2D.MipLevels = desc.MipLevels;
		FAILED(hr = m_pd3dDevice->CreateShaderResourceView(SpreatingTexTure, &SRVDesc, SpreatingSRV.GetAddressOf()));
	}
}
/*�ؽ��� ���� �Լ�*/
void BMapTileRender::SaveSpreating(ID3D11DeviceContext* pImmediateContext)
{
	HRESULT hr;
	hr = S_OK;
	D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
	if (SUCCEEDED(pImmediateContext->Map((ID3D11Resource*)StagingTexTure, 0, D3D11_MAP_READ_WRITE, 0, &MappedFaceDest)))
	{
		BYTE* pDestBytes = (BYTE*)MappedFaceDest.pData;
		for (int y = 0; y < desc.Height * 10; y++)
		{
			BYTE* pDest = pDestBytes;
			for (int x = 0; x < desc.Width * 10; x++)
			{
				BYTE* pRedDest;
				BYTE* pGreenDest;
				BYTE* pBlueDest;
				BYTE* pAlphaDest;

				pRedDest = pDest;
				*pDest++;
				pGreenDest = pDest;
				*pDest++;
				pBlueDest = pDest;
				*pDest++;
				pAlphaDest = pDest;
				*pDest++;

				if (pAlphaDest > 0 && pRedDest == 0 && pGreenDest == 0 && pBlueDest == 0)
				{
					*pRedDest = *pAlphaDest;
					*pGreenDest = *pAlphaDest;
					*pBlueDest = *pAlphaDest;
				}
			}
			pDestBytes += MappedFaceDest.RowPitch;
		}
		pImmediateContext->Unmap(StagingTexTure, 0);
		pImmediateContext->CopyResource(SpreatingTexTure, StagingTexTure);
		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		ZeroMemory(&SRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		SRVDesc.Format = desc.Format;
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRVDesc.Texture2D.MipLevels = desc.MipLevels;
		FAILED(hr = m_pd3dDevice->CreateShaderResourceView(SpreatingTexTure, &SRVDesc, SpreatingSRV.GetAddressOf()));
	}
}
/*�׶���Ʈ ��� �ؽ��� ���� �Լ�*/
void BMapTileRender::GradientSpreating(ID3D11DeviceContext* pImmediateContext, B_BOX Area, D3DXVECTOR3 CrashPoint, float fRadius, int iColor)
{
	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
	D3DXVECTOR3 CrashDot = CrashPoint;
	CrashDot.x = CrashDot.x * desc.Width;
	CrashDot.y = 0.0f;
	CrashDot.z = CrashDot.z * desc.Height;
	B_BOX FixedArea;
	FixedArea.vMax.x = (int)(Area.vMax.x * desc.Width);
	FixedArea.vMax.z = (int)(Area.vMax.z * desc.Height);
	FixedArea.vMin.x = (int)(Area.vMin.x * desc.Width);
	FixedArea.vMin.z = (int)(Area.vMin.z * desc.Height);

	if (FixedArea.vMin.x < 0.0f)
	{
		FixedArea.vMin.x = 0.0f;
	}
	if (FixedArea.vMin.z < 0.0f)
	{
		FixedArea.vMin.z = 0.0f;
	}
	if (FixedArea.vMax.x > desc.Width)
	{
		FixedArea.vMax.x = desc.Width;
	}
	if (FixedArea.vMax.z > desc.Height)
	{
		FixedArea.vMax.z = desc.Height;
	}

	float CheckRadius = fRadius * 10.0f;
	if (SUCCEEDED(pImmediateContext->Map((ID3D11Resource*)StagingTexTure, 0, D3D11_MAP_READ_WRITE, 0, &MappedFaceDest)))
	{
		BYTE* pDestBytes = (BYTE*)MappedFaceDest.pData;
		pDestBytes = pDestBytes + MappedFaceDest.RowPitch * (UINT)FixedArea.vMin.z;
		for (int y = FixedArea.vMin.z; y < FixedArea.vMax.z; y++)
		{
			BYTE* pDest = pDestBytes;
			pDest = pDest + (UINT)(4 * FixedArea.vMin.x);
			for (int x = FixedArea.vMin.x; x < FixedArea.vMax.x; x++)
			{
				D3DXVECTOR3 Dot = D3DXVECTOR3(x, 0.0f, y);
				float fColorPower = D3DXVec3Length(&(Dot - CrashDot));
				if (CheckRadius >= fColorPower)
				{
					BYTE* RedDest;
					RedDest = pDest;
					*pDest++;
					BYTE* GreenDest;
					GreenDest = pDest;
					*pDest++;
					BYTE* BlueDest;
					BlueDest = pDest;
					*pDest++;
					BYTE* AlphaDest;
					AlphaDest = pDest;
					*pDest++;
					fColorPower = (fabs(fColorPower - CheckRadius) / CheckRadius) * 255.0f;
					int iColorPower = (int)fColorPower;
					float fRatio = (float)(*RedDest + *GreenDest + *BlueDest + iColorPower) / 255.0f; //  *AlphaDest +
					switch (iColor)
					{
					case 0:
						*RedDest /= fRatio;
						*RedDest += (iColorPower / fRatio);
						*GreenDest /= fRatio;
						*BlueDest /= fRatio;
						*AlphaDest /= fRatio;
						break;
					case 1:
						*RedDest /= fRatio;
						*GreenDest /= fRatio;
						*GreenDest += (iColorPower / fRatio);
						*BlueDest /= fRatio;
						*AlphaDest /= fRatio;
						break;
					case 2:
						*RedDest /= fRatio;
						*GreenDest /= fRatio;
						*BlueDest /= fRatio;
						*BlueDest += (iColorPower / fRatio);
						*AlphaDest /= fRatio;
						break;
					case 3:
						*RedDest /= fRatio;
						*GreenDest /= fRatio;
						*BlueDest /= fRatio;
						*AlphaDest /= fRatio;
						*AlphaDest += (iColorPower / fRatio);
						break;
					default:
						break;
					}
				}
				else
				{
					pDest += 4;
				}
			}
			pDestBytes += MappedFaceDest.RowPitch;
		}
		pImmediateContext->Unmap(StagingTexTure, 0);
		pImmediateContext->CopyResource(SpreatingTexTure, StagingTexTure);
		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		ZeroMemory(&SRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		SRVDesc.Format = desc.Format;
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRVDesc.Texture2D.MipLevels = desc.MipLevels;
		FAILED(hr = m_pd3dDevice->CreateShaderResourceView(SpreatingTexTure, &SRVDesc, SpreatingSRV.GetAddressOf()));
	}
}
/*�ؽ��� �ε� �Լ�*/
HRESULT BMapTileRender::SplattingTextureLoad(ID3D11Device* pDevice, const TCHAR* strFilePath, int iColor)
{
	HRESULT hr = S_OK;
	if (strFilePath == NULL) m_pSplattingSRV[iColor].Attach(nullptr);
	ID3D11ShaderResourceView* pSRV = nullptr;
	if (FAILED(hr = D3DX11CreateShaderResourceViewFromFile(pDevice, strFilePath, NULL, NULL, &pSRV, NULL)))
	{
		m_pSplattingSRV[iColor].Attach(nullptr);
	}
	m_pSplattingSRV[iColor].Attach(pSRV);
	return hr;
}
/*������Ʈ �������� �ؽ��� ���� �Լ�*/
void BMapTileRender::ObjSpreating(ID3D11DeviceContext* pImmediateContext, B_BOX Area, D3DXVECTOR3 CrashPoint, float fRadius, int iColor)
{
	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
	D3DXVECTOR3 CrashDot = CrashPoint;

	CrashDot.x = (int)CrashDot.x;
	CrashDot.y = 0.0f;
	CrashDot.z = (int)CrashDot.z;

	B_BOX FixedArea;

	FixedArea.vMax.x = CrashDot.x + fRadius;
	FixedArea.vMax.z = CrashDot.z + fRadius;
	FixedArea.vMin.x = CrashDot.x - fRadius + 1.0f;
	FixedArea.vMin.z = CrashDot.z - fRadius + 1.0f;

	if (FixedArea.vMin.x < 0.0f)
	{
		FixedArea.vMin.x = 0.0f;
	}
	if (FixedArea.vMin.z < 0.0f)
	{
		FixedArea.vMin.z = 0.0f;
	}
	if (FixedArea.vMax.x > Objdesc.Width)
	{
		FixedArea.vMax.x = Objdesc.Width;
	}
	if (FixedArea.vMax.z > Objdesc.Height)
	{
		FixedArea.vMax.z = Objdesc.Height;
	}

	// �������� Ư���ϱ� ���� ����
	float fCheckRadius = fRadius * 0.9f;

	if (SUCCEEDED(pImmediateContext->Map((ID3D11Resource*)StagingObjPosTexture, 0, D3D11_MAP_READ_WRITE, 0, &MappedFaceDest)))
	{
		BYTE* pDestBytes = (BYTE*)MappedFaceDest.pData;
		pDestBytes = pDestBytes + MappedFaceDest.RowPitch * (UINT)FixedArea.vMin.z;
		for (int y = FixedArea.vMin.z; y < FixedArea.vMax.z; y++)
		{
			BYTE* pDest = pDestBytes;
			pDest = pDest + (UINT)(4 * FixedArea.vMin.x);
			for (int x = FixedArea.vMin.x; x < FixedArea.vMax.x; x++)
			{
				switch (iColor)
				{
				case 0:
					*pDest++ = 255;
					*pDest++ = 0;
					*pDest++ = 0;
					*pDest++ = 0;
					break;
				case 1:
					*pDest++ = 0;
					*pDest++ = 255;
					*pDest++ = 0;
					*pDest++ = 0;
					break;
				case 2:
					*pDest++ = 0;
					*pDest++ = 0;
					*pDest++ = 255;
					*pDest++ = 0;
					break;
				case 3:
					*pDest++ = 0;
					*pDest++ = 0;
					*pDest++ = 0;
					*pDest++ = 0;
					break;
				default:
					break;
				}
			}
			pDestBytes += MappedFaceDest.RowPitch;
		}
		pImmediateContext->Unmap(StagingObjPosTexture, 0);
		pImmediateContext->CopyResource(ObjPosTexture, StagingObjPosTexture);
		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		ZeroMemory(&SRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		SRVDesc.Format = Objdesc.Format;
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRVDesc.Texture2D.MipLevels = Objdesc.MipLevels;
		FAILED(hr = m_pd3dDevice->CreateShaderResourceView(ObjPosTexture, &SRVDesc, SpreatingSRV.GetAddressOf()));
	}
}
/*������ �ؽ��� ���� �Լ�*/
HRESULT BMapTileRender::CreateSRV(ID3D11Texture2D* SpreatingTex, ID3D11Texture2D* StagingTex, D3D11_TEXTURE2D_DESC SpreatingDesc)
{
	HRESULT hr = S_OK;
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMemory(&SRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	SRVDesc.Format = SpreatingDesc.Format;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = SpreatingDesc.MipLevels;
	m_pImmediateContext->CopyResource(SpreatingTex, StagingTex);
	FAILED(hr = m_pd3dDevice->CreateShaderResourceView(SpreatingTex, &SRVDesc, SpreatingSRV.GetAddressOf()));
	return hr;
}
/*���� �ؽ��� ���� ����*/
bool BMapTileRender::AlphaRender(ID3D11DeviceContext* pContext)
{
	m_pMap->PreRender(pContext);
	//// �����ؽ��Ŀ� ���� �̹��� ���
	//if (m_pSplattingSRV.size())
	//{
	//	for (int iMap = 0; iMap < 4; iMap++)
	//	{
	//		pContext->PSSetShaderResources((iMap + 2), 1, m_pSplattingSRV[iMap].GetAddressOf()); // �ؽ��� ����
	//	}
	//}
	// �����ؽ��� ���
	if (SpreatingSRV.Get())
	{
		pContext->PSSetShaderResources(1, 1, SpreatingSRV.GetAddressOf()); // �ؽ��� ����
	}
	pContext->PSSetShader(m_pMap->m_pAlphaPS, NULL, 0);
	PostRender(pContext);
	return true;
}
#pragma endregion

/*�ٿ�� �ڽ� �翬�� �Լ�(���)*/
void BMapTileRender::UpdateBoundingBox(BNode* pNode)
{
	ComputeBoundingBox(pNode);
	if (pNode->m_pChild[0] != NULL)
	{
		for (int iNode = 0; iNode < 4; iNode++)
		{
			UpdateBoundingBox(pNode->m_pChild[iNode]);
		}
	}
}

/*���� �ε��� ����Ʈ �����Լ�*/
int BMapTileRender::UpdateStaticIndexList(BNode* pNode, DWORD dwCurrentIndex, DWORD dwLod)
{
	if (!m_IndexList.size()) return false;
	int iNumFaces = 0;
	DWORD dwTL = pNode->m_CornerIndex[0];

	DWORD dwIndexCnt = m_pdwLodIndexArray[dwLod]->IndexList[pNode->m_dwLodType].size();
	for (DWORD dwIndex = 0; dwIndex < dwIndexCnt; dwIndex += 3)
	{
		DWORD dw0 = GetIndex(m_pdwLodIndexArray[dwLod]->IndexList[pNode->m_dwLodType][dwIndex + 0], dwTL);
		DWORD dw1 = GetIndex(m_pdwLodIndexArray[dwLod]->IndexList[pNode->m_dwLodType][dwIndex + 1], dwTL);
		DWORD dw2 = GetIndex(m_pdwLodIndexArray[dwLod]->IndexList[pNode->m_dwLodType][dwIndex + 2], dwTL);

		m_IndexList[dwCurrentIndex + 0] = dw0;
		m_IndexList[dwCurrentIndex + 1] = dw1;
		m_IndexList[dwCurrentIndex + 2] = dw2;
		iNumFaces += 1;
		dwCurrentIndex += 3;
	}
	return iNumFaces;
}

/*��� �ε��� ��ȯ�Լ�*/
int BMapTileRender::GetIndex(DWORD dwIndex, DWORD dwTL)
{
	DWORD dwReturn = 0;
	DWORD dwStartRow = dwTL / m_dwWidth;
	DWORD dwStartCol = dwTL % m_dwWidth;

	DWORD dwRow = dwIndex / (int)(pow(2.0f, (float)m_iPatchLodCount) + 1);
	DWORD dwCol = dwIndex % (int)(pow(2.0f, (float)m_iPatchLodCount) + 1);

	dwReturn = (dwStartRow + dwRow) * m_dwWidth + dwStartCol + dwCol;
	return dwReturn;
}

/*�������� ��ü���� �Լ�*/
bool BMapTileRender::OMSetState(ID3D11DeviceContext* pContext, ID3D11RenderTargetView* m_pRTV, ID3D11DepthStencilView* m_pDSV, ID3D11DepthStencilState* m_pDSS, ID3D11BlendState* m_pBS)
{
	pContext->OMSetRenderTargets(1, &m_pRTV, m_pDSV);		// ����Ÿ�ٺ�, ���̹��� ����
	pContext->OMSetDepthStencilState(m_pDSS, 0x00);			// ���� ���ٽ� �伳��
	pContext->OMSetBlendState(m_pBS, 0, -1);				// ���ĺ����� �� ����
	return true;
}

BMapTileRender::BMapTileRender()
{
	m_dwWidth = 0;
	m_dwHeight = 0;
	m_iNumFace = 0;
	m_iMinSplitDistance = 2;
	m_bSharedVertexBuffer = true;
	m_dstibOffset = 0;
	m_ibOffset = 0;
}
BMapTileRender::~BMapTileRender()
{
}