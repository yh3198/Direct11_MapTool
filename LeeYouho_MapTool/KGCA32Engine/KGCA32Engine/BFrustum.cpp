#include "BFrustum.h"

/*프로스텀 생성함수*/
bool BFrustum::CreateFrustum(ID3D11Device* m_pd3dDevice, const TCHAR* pShaderFile)
{
	S_NEW(m_pBoxShape, BBox);
	m_pBoxShape->m_VertexList.resize(24);
	m_pBoxShape->m_VertexList[0].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	m_pBoxShape->m_VertexList[1].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	m_pBoxShape->m_VertexList[2].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	m_pBoxShape->m_VertexList[3].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	m_pBoxShape->m_VertexList[4].Normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_pBoxShape->m_VertexList[5].Normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_pBoxShape->m_VertexList[6].Normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_pBoxShape->m_VertexList[7].Normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	m_pBoxShape->m_VertexList[8].Normal = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_pBoxShape->m_VertexList[9].Normal = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_pBoxShape->m_VertexList[10].Normal = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_pBoxShape->m_VertexList[11].Normal = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

	m_pBoxShape->m_VertexList[12].Normal = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	m_pBoxShape->m_VertexList[13].Normal = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	m_pBoxShape->m_VertexList[14].Normal = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	m_pBoxShape->m_VertexList[15].Normal = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);

	m_pBoxShape->m_VertexList[16].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_pBoxShape->m_VertexList[17].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_pBoxShape->m_VertexList[18].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_pBoxShape->m_VertexList[19].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	m_pBoxShape->m_VertexList[20].Normal = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	m_pBoxShape->m_VertexList[21].Normal = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	m_pBoxShape->m_VertexList[22].Normal = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	m_pBoxShape->m_VertexList[23].Normal = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	m_pBoxShape->m_VertexList[0].Color = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.3f);
	m_pBoxShape->m_VertexList[1].Color = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.3f);
	m_pBoxShape->m_VertexList[2].Color = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.3f);
	m_pBoxShape->m_VertexList[3].Color = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.3f);

	m_pBoxShape->m_VertexList[4].Color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 0.3f);
	m_pBoxShape->m_VertexList[5].Color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 0.3f);
	m_pBoxShape->m_VertexList[6].Color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 0.3f);
	m_pBoxShape->m_VertexList[7].Color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 0.3f);

	m_pBoxShape->m_VertexList[8].Color = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 0.3f);
	m_pBoxShape->m_VertexList[9].Color = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 0.3f);
	m_pBoxShape->m_VertexList[10].Color = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 0.3f);
	m_pBoxShape->m_VertexList[11].Color = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 0.3f);

	m_pBoxShape->m_VertexList[12].Color = D3DXVECTOR4(1.0f, 0.0f, 1.0f, 0.3f);
	m_pBoxShape->m_VertexList[13].Color = D3DXVECTOR4(1.0f, 0.0f, 1.0f, 0.3f);
	m_pBoxShape->m_VertexList[14].Color = D3DXVECTOR4(1.0f, 0.0f, 1.0f, 0.3f);
	m_pBoxShape->m_VertexList[15].Color = D3DXVECTOR4(1.0f, 0.0f, 1.0f, 0.3f);

	m_pBoxShape->m_VertexList[16].Color = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 0.3f);
	m_pBoxShape->m_VertexList[17].Color = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 0.3f);
	m_pBoxShape->m_VertexList[18].Color = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 0.3f);
	m_pBoxShape->m_VertexList[19].Color = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 0.3f);

	m_pBoxShape->m_VertexList[20].Color = D3DXVECTOR4(0.0f, 1.0f, 1.0f, 0.3f);
	m_pBoxShape->m_VertexList[21].Color = D3DXVECTOR4(0.0f, 1.0f, 1.0f, 0.3f);
	m_pBoxShape->m_VertexList[22].Color = D3DXVECTOR4(0.0f, 1.0f, 1.0f, 0.3f);
	m_pBoxShape->m_VertexList[23].Color = D3DXVECTOR4(0.0f, 1.0f, 1.0f, 0.3f);

	m_pBoxShape->m_VertexList[0].Texture = D3DXVECTOR2(0.0f, 0.0f);
	m_pBoxShape->m_VertexList[1].Texture = D3DXVECTOR2(1.0f, 0.0f);
	m_pBoxShape->m_VertexList[2].Texture = D3DXVECTOR2(1.0f, 1.0f);
	m_pBoxShape->m_VertexList[3].Texture = D3DXVECTOR2(0.0f, 1.0f);

	m_pBoxShape->m_VertexList[4].Texture = D3DXVECTOR2(0.0f, 0.0f);
	m_pBoxShape->m_VertexList[5].Texture = D3DXVECTOR2(1.0f, 0.0f);
	m_pBoxShape->m_VertexList[6].Texture = D3DXVECTOR2(1.0f, 1.0f);
	m_pBoxShape->m_VertexList[7].Texture = D3DXVECTOR2(0.0f, 1.0f);

	m_pBoxShape->m_VertexList[8].Texture = D3DXVECTOR2(0.0f, 0.0f);
	m_pBoxShape->m_VertexList[9].Texture = D3DXVECTOR2(1.0f, 0.0f);
	m_pBoxShape->m_VertexList[10].Texture = D3DXVECTOR2(1.0f, 1.0f);
	m_pBoxShape->m_VertexList[11].Texture = D3DXVECTOR2(0.0f, 1.0f);

	m_pBoxShape->m_VertexList[12].Texture = D3DXVECTOR2(0.0f, 0.0f);
	m_pBoxShape->m_VertexList[13].Texture = D3DXVECTOR2(1.0f, 0.0f);
	m_pBoxShape->m_VertexList[14].Texture = D3DXVECTOR2(1.0f, 1.0f);
	m_pBoxShape->m_VertexList[15].Texture = D3DXVECTOR2(0.0f, 1.0f);

	m_pBoxShape->m_VertexList[16].Texture = D3DXVECTOR2(0.0f, 0.0f);
	m_pBoxShape->m_VertexList[17].Texture = D3DXVECTOR2(1.0f, 0.0f);
	m_pBoxShape->m_VertexList[18].Texture = D3DXVECTOR2(1.0f, 1.0f);
	m_pBoxShape->m_VertexList[19].Texture = D3DXVECTOR2(0.0f, 1.0f);

	m_pBoxShape->m_VertexList[20].Texture = D3DXVECTOR2(0.0f, 0.0f);
	m_pBoxShape->m_VertexList[21].Texture = D3DXVECTOR2(1.0f, 0.0f);
	m_pBoxShape->m_VertexList[22].Texture = D3DXVECTOR2(1.0f, 1.0f);
	m_pBoxShape->m_VertexList[23].Texture = D3DXVECTOR2(0.0f, 1.0f);


	D3DXMatrixMultiply(&m_matViewProj, &m_matView, &m_matProj);
	D3DXMatrixInverse(&m_matViewProj, NULL, &m_matViewProj);

	// 0123 = 전면 4567 = 후면
	//     5    6
	// 1    2
	//     4    7
	// 0    3  

	m_vFrustum[0] = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	m_vFrustum[1] = D3DXVECTOR3(-1.0f,  1.0f, 0.0f);
	m_vFrustum[2] = D3DXVECTOR3( 1.0f,  1.0f, 0.0f);
	m_vFrustum[3] = D3DXVECTOR3( 1.0f, -1.0f, 0.0f);

	m_vFrustum[4] = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	m_vFrustum[5] = D3DXVECTOR3(-1.0f,  1.0f, 1.0f);
	m_vFrustum[6] = D3DXVECTOR3( 1.0f,  1.0f, 1.0f);
	m_vFrustum[7] = D3DXVECTOR3( 1.0f, -1.0f, 1.0f);

	for (int iVertex = 0; iVertex < 8; iVertex++)
	{
		D3DXVec3TransformCoord(&m_vFrustum[iVertex], &m_vFrustum[iVertex], &m_matViewProj);
	}

	/*내적했을때 - 가 나오면 밖, + 가 나오면 안쪽이 나오도록 반시계방향으로 판 생성*/
	m_Plane[0].CreatePlane(m_vFrustum[5], m_vFrustum[0], m_vFrustum[1]); // 좌
	m_Plane[1].CreatePlane(m_vFrustum[3], m_vFrustum[6], m_vFrustum[2]); // 우
	m_Plane[2].CreatePlane(m_vFrustum[5], m_vFrustum[2], m_vFrustum[6]); // 상
	m_Plane[3].CreatePlane(m_vFrustum[0], m_vFrustum[7], m_vFrustum[3]); // 하
	m_Plane[4].CreatePlane(m_vFrustum[0], m_vFrustum[2], m_vFrustum[1]); // 앞쪽
	m_Plane[5].CreatePlane(m_vFrustum[6], m_vFrustum[4], m_vFrustum[5]); // 뒤쪽

	//앞
	m_pBoxShape->m_VertexList[0].Position = m_vFrustum[1];
	m_pBoxShape->m_VertexList[1].Position = m_vFrustum[2];
	m_pBoxShape->m_VertexList[2].Position = m_vFrustum[3];
	m_pBoxShape->m_VertexList[3].Position = m_vFrustum[0];
	//뒤
	m_pBoxShape->m_VertexList[4].Position = m_vFrustum[6];
	m_pBoxShape->m_VertexList[5].Position = m_vFrustum[5];
	m_pBoxShape->m_VertexList[6].Position = m_vFrustum[4];
	m_pBoxShape->m_VertexList[7].Position = m_vFrustum[7];
	//왼
	m_pBoxShape->m_VertexList[8].Position = m_vFrustum[5];
	m_pBoxShape->m_VertexList[9].Position = m_vFrustum[1];
	m_pBoxShape->m_VertexList[10].Position = m_vFrustum[0];
	m_pBoxShape->m_VertexList[11].Position = m_vFrustum[4];
	//오른
	m_pBoxShape->m_VertexList[12].Position = m_vFrustum[2];
	m_pBoxShape->m_VertexList[13].Position = m_vFrustum[6];
	m_pBoxShape->m_VertexList[14].Position = m_vFrustum[7];
	m_pBoxShape->m_VertexList[15].Position = m_vFrustum[3];
	//위
	m_pBoxShape->m_VertexList[16].Position = m_vFrustum[5];
	m_pBoxShape->m_VertexList[17].Position = m_vFrustum[6];
	m_pBoxShape->m_VertexList[18].Position = m_vFrustum[2];
	m_pBoxShape->m_VertexList[19].Position = m_vFrustum[1];
	//아래
	m_pBoxShape->m_VertexList[20].Position = m_vFrustum[0];
	m_pBoxShape->m_VertexList[21].Position = m_vFrustum[3];
	m_pBoxShape->m_VertexList[22].Position = m_vFrustum[7];
	m_pBoxShape->m_VertexList[23].Position = m_vFrustum[4];

	m_pBoxShape->m_VertexCnt = m_pBoxShape->m_VertexList.size();

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;						
	bd.ByteWidth = sizeof(PNCT_Vertex) * m_pBoxShape->m_VertexCnt;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;							

	D3D11_SUBRESOURCE_DATA InitData;			
	InitData.pSysMem = (void*)&m_pBoxShape->m_VertexList.at(0);
	m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pBoxShape->m_pVertexBuffer);

	m_pBoxShape->m_pd3dDevice = m_pd3dDevice;
	m_pBoxShape->CreateIndexBuffer();
	m_pBoxShape->LoadShaderAndInputLayout(pShaderFile);
	m_pBoxShape->CreateConstantBuffer();

	return true;
}

/*프로스텀 충돌 체크함수*/
#pragma region 프로스텀 충돌함수
/*교점 충돌*/
BOOL BFrustum::CheckPoint(D3DXVECTOR3* pv)
{
	float fPlaneToPoint;
	for (int iPlane = 0; iPlane < 6; iPlane++)
	{
		fPlaneToPoint = m_Plane[iPlane].fA * pv->x + m_Plane[iPlane].fB * pv->y + m_Plane[iPlane].fC * pv->z + m_Plane[iPlane].fD;
		if (fPlaneToPoint < 0.0f)
			return FALSE;
	}
	return TRUE;
}
/*구 충돌*/
BOOL BFrustum::CheckSphere(BSphere* pSphere)
{
	float fPlaneToCenter;
	for (int iPlane = 0; iPlane < 6; iPlane++)
	{
		fPlaneToCenter = m_Plane[iPlane].fA * pSphere->m_vCenter.x + m_Plane[iPlane].fB * pSphere->m_vCenter.y + m_Plane[iPlane].fC * pSphere->m_vCenter.z + m_Plane[iPlane].fD;
		if (fPlaneToCenter <= -pSphere->m_fRadius)
			return FALSE;
	}
	return TRUE;
}
/*OBB 박스충돌*/
BOOL BFrustum::CheckOBB(B_BOX* pBox)
{
	float fPlaneToCenter = 0.0f;
	float fDistance = 0.0f;
	D3DXVECTOR3 vDir;

	for (int iPlane = 0; iPlane < 6; iPlane++)
	{
		vDir = pBox->vAxis[0] * pBox->fExtent[0];
		fDistance = fabs(m_Plane[iPlane].fA * vDir.x + m_Plane[iPlane].fB * vDir.y + m_Plane[iPlane].fC * vDir.z);
		vDir = pBox->vAxis[1] * pBox->fExtent[1];
		fDistance += fabs(m_Plane[iPlane].fA * vDir.x + m_Plane[iPlane].fB * vDir.y + m_Plane[iPlane].fC * vDir.z);
		vDir = pBox->vAxis[2] * pBox->fExtent[2];
		fDistance += fabs(m_Plane[iPlane].fA * vDir.x + m_Plane[iPlane].fB * vDir.y + m_Plane[iPlane].fC * vDir.z);

		fPlaneToCenter = m_Plane[iPlane].fA * pBox->vCenter.x + m_Plane[iPlane].fB * pBox->vCenter.y + m_Plane[iPlane].fC * pBox->vCenter.z + m_Plane[iPlane].fD;

		if (fPlaneToCenter <= -fDistance)
		{
			return FALSE;
		}
	}
	return TRUE;
}
/*OBB 박스충돌 (충돌상태 반환)*/
T_POSITION BFrustum::CheckPositionOBBInPlane(B_BOX* pBox)
{
	float fPlaneToCenter = 0.0f;
	float fDistance = 0.0f;
	D3DXVECTOR3 vDir;
	T_POSITION t_Position;

	t_Position = P_FRONT;
	for (int iPlane = 0; iPlane < 6; iPlane++)
	{
		vDir = pBox->vAxis[0] * pBox->fExtent[0];
		fDistance = fabs(m_Plane[iPlane].fA * vDir.x + m_Plane[iPlane].fB*vDir.y + m_Plane[iPlane].fC * vDir.z);
		vDir = pBox->vAxis[1] * pBox->fExtent[1];
		fDistance += fabs(m_Plane[iPlane].fA * vDir.x + m_Plane[iPlane].fB*vDir.y + m_Plane[iPlane].fC * vDir.z);
		vDir = pBox->vAxis[2] * pBox->fExtent[2];
		fDistance += fabs(m_Plane[iPlane].fA * vDir.x + m_Plane[iPlane].fB*vDir.y + m_Plane[iPlane].fC * vDir.z);

		fPlaneToCenter = m_Plane[iPlane].fA * pBox->vCenter.x + m_Plane[iPlane].fB*pBox->vCenter.y +
			m_Plane[iPlane].fC * pBox->vCenter.z + m_Plane[iPlane].fD;

		if (fPlaneToCenter <= fDistance)
		{
			t_Position = P_SPANNING;
		}
		if (fPlaneToCenter + 2.0f < -fDistance)
		{
			return P_BACK;
		}
	}
	return t_Position;
}
#pragma endregion

/*프로스텀 랜더 세팅함수*/
bool BFrustum::FrustRender(ID3D11DeviceContext* pContext)
{
	D3DXMatrixMultiply(&m_matViewProj, &m_matView, &m_matProj);
	D3DXMatrixInverse(&m_matViewProj, NULL, &m_matViewProj);

	// 0123 = 전면 4567 = 후면
	//     5    6
	// 1    2
	//     4    7
	// 0    3  

	m_vFrustum[0] = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	m_vFrustum[1] = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	m_vFrustum[2] = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	m_vFrustum[3] = D3DXVECTOR3(1.0f, -1.0f, 0.0f);

	m_vFrustum[4] = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	m_vFrustum[5] = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
	m_vFrustum[6] = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_vFrustum[7] = D3DXVECTOR3(1.0f, -1.0f, 1.0f);

	for (int iVertex = 0; iVertex < 8; iVertex++)
	{
		D3DXVec3TransformCoord(&m_vFrustum[iVertex], &m_vFrustum[iVertex], &m_matViewProj);
	}

	m_Plane[0].CreatePlane(m_vFrustum[5], m_vFrustum[0], m_vFrustum[1]); // 좌
	m_Plane[1].CreatePlane(m_vFrustum[3], m_vFrustum[6], m_vFrustum[2]); // 우
	m_Plane[2].CreatePlane(m_vFrustum[5], m_vFrustum[2], m_vFrustum[6]); // 상
	m_Plane[3].CreatePlane(m_vFrustum[0], m_vFrustum[7], m_vFrustum[3]); // 하
	m_Plane[4].CreatePlane(m_vFrustum[0], m_vFrustum[2], m_vFrustum[1]); // 앞쪽
	m_Plane[5].CreatePlane(m_vFrustum[6], m_vFrustum[4], m_vFrustum[5]); // 뒤쪽

	//앞
	m_pBoxShape->m_VertexList[0].Position = m_vFrustum[1];
	m_pBoxShape->m_VertexList[1].Position = m_vFrustum[2];
	m_pBoxShape->m_VertexList[2].Position = m_vFrustum[3];
	m_pBoxShape->m_VertexList[3].Position = m_vFrustum[0];
	//뒤
	m_pBoxShape->m_VertexList[4].Position = m_vFrustum[6];
	m_pBoxShape->m_VertexList[5].Position = m_vFrustum[5];
	m_pBoxShape->m_VertexList[6].Position = m_vFrustum[4];
	m_pBoxShape->m_VertexList[7].Position = m_vFrustum[7];
	//왼
	m_pBoxShape->m_VertexList[8].Position = m_vFrustum[5];
	m_pBoxShape->m_VertexList[10].Position = m_vFrustum[0];
	m_pBoxShape->m_VertexList[11].Position = m_vFrustum[4];
	//오른
	m_pBoxShape->m_VertexList[12].Position = m_vFrustum[2];
	m_pBoxShape->m_VertexList[13].Position = m_vFrustum[6];
	m_pBoxShape->m_VertexList[14].Position = m_vFrustum[7];
	m_pBoxShape->m_VertexList[15].Position = m_vFrustum[3];
	//위
	m_pBoxShape->m_VertexList[16].Position = m_vFrustum[5];
	m_pBoxShape->m_VertexList[17].Position = m_vFrustum[6];
	m_pBoxShape->m_VertexList[18].Position = m_vFrustum[2];
	m_pBoxShape->m_VertexList[19].Position = m_vFrustum[1];
	//아래
	m_pBoxShape->m_VertexList[20].Position = m_vFrustum[0];
	m_pBoxShape->m_VertexList[21].Position = m_vFrustum[3];
	m_pBoxShape->m_VertexList[22].Position = m_vFrustum[7];
	m_pBoxShape->m_VertexList[23].Position = m_vFrustum[4];
	
	pContext->UpdateSubresource(m_pBoxShape->m_pVertexBuffer, 0, NULL, &m_pBoxShape->m_VertexList[0], 0, 0);
	
	return true;
}



BFrustum::BFrustum()
{
}
BFrustum::~BFrustum()
{
}
