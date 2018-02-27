#include "BSelect.h"

#pragma region ���� ���ÿ� �Լ�
/*���� ��� ���ÿ� �Լ�*/
void BSelect::SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj)
{
	if (pWorld != NULL)
	{
		m_matWorld = *pWorld;
		m_vCenter.x = pWorld->_41;
		m_vCenter.y = pWorld->_42;
		m_vCenter.z = pWorld->_43;
	}
	else
	{
		D3DXMatrixIdentity(&m_matWorld);
		m_vCenter.x = m_matWorld._41;
		m_vCenter.y = m_matWorld._42;
		m_vCenter.z = m_matWorld._43;
	}
	if (pView != NULL)
	{
		m_matView = *pView;
	}
	if (pProj != NULL)
	{
		m_matProj = *pProj;
	}
	Update();
}
/*���� ��ġ ���ÿ� �Լ�*/
void BSelect::Update()
{
	POINT ptCursor;
	GetCursorPos(&ptCursor);
	ScreenToClient(g_hWnd, &ptCursor);

	D3DXVECTOR3 v;
	v.x = (((2.0f * ptCursor.x) / g_iWindowWidth) - 1) / m_matProj._11;
	v.y = -(((2.0f * ptCursor.y) / g_iWindowHeight) - 1) / m_matProj._22;
	v.z = 1.0f;

	D3DXMATRIX mWorldView = m_matWorld * m_matView;
	D3DXMATRIX m;
	D3DXMatrixInverse(&m, NULL, &mWorldView);

	m_Ray.vOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Ray.vDirection = D3DXVECTOR3(v.x, v.y, v.z);
	D3DXVec3TransformCoord(&m_Ray.vOrigin, &m_Ray.vOrigin, &m);
	D3DXVec3TransformNormal(&m_Ray.vDirection, &m_Ray.vDirection, &m);
	D3DXVec3Normalize(&m_Ray.vDirection, &m_Ray.vDirection);
	int k = 0;
}
#pragma endregion

#pragma region �浹 ó���� �Լ�
/*�� ������ �̷���� ���̽� �浹 Ȯ���Լ�*/
bool BSelect::CheckPick(D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2)
{
	FLOAT fBary1, fBary2;
	FLOAT fDist;
	if (IntersectTriangle(m_Ray.vOrigin, m_Ray.vDirection, v0, v1, v2, &fDist, &fBary1, &fBary2))
	{
		m_vSrcVertex[0] = v0;
		m_vSrcVertex[1] = v1;
		m_vSrcVertex[2] = v2;
		m_matWorldPick = m_matWorld;
		return true;
	}
	return false;
}
/*�簢�� �浹 Ȯ���Լ�*/
bool BSelect::IntersectRect(const D3DXVECTOR3& orig, const D3DXVECTOR3& dir, B_BOX m_tBox, FLOAT* t, FLOAT *u, FLOAT *v)
{
	FLOAT fBary1, fBary2;
	FLOAT fDist;
	if (IntersectTriangle(orig, dir, D3DXVECTOR3(m_tBox.vMin.x, 0, m_tBox.vMin.z), D3DXVECTOR3(m_tBox.vMin.x, 0, m_tBox.vMax.z), D3DXVECTOR3(m_tBox.vMax.x, 0, m_tBox.vMax.z), &fDist, &fBary1, &fBary2))
	{
		return true;
	}
	else if (IntersectTriangle(orig, dir, D3DXVECTOR3(m_tBox.vMin.x, 0, m_tBox.vMin.z), D3DXVECTOR3(m_tBox.vMax.x, 0, m_tBox.vMax.z), D3DXVECTOR3(m_tBox.vMax.x, 0, m_tBox.vMin.z), &fDist, &fBary1, &fBary2))
	{
		return true;
	}
	else
	{
		return false;
	}
}
/*�ﰢ�� �浹 Ȯ���Լ� (IntersectRect ���� ȣ��)*/
bool BSelect::IntersectTriangle(const D3DXVECTOR3& orig, const D3DXVECTOR3& dir, D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2, FLOAT* t, FLOAT *u, FLOAT *v)
{
	D3DXVECTOR3 edge1 = v1 - v0;
	D3DXVECTOR3 edge2 = v2 - v0;

	D3DXVec3Cross(&Pvec, &dir, &edge2);

	FLOAT det = D3DXVec3Dot(&edge1, &Pvec);//��Ľ�

										   /*�������� ����� �ǵ��� det������ �����´�.*/
	if (det > 0)
	{
		Tvec = orig - v0;
	}
	else
	{
		Tvec = v0 - orig;
		det = -det;
	}

	if (det < 0.0001f)
		return false;

	*u = D3DXVec3Dot(&Tvec, &Pvec);
	if (*u <0.0f || *u > det)
		return false;

	D3DXVec3Cross(&Qvec, &Tvec, &edge1);
	*v = D3DXVec3Dot(&dir, &Qvec);
	if (*v < 0.0f || *u + *v >det)
		return false;

	*t = D3DXVec3Dot(&edge2, &Qvec);
	FLOAT fInvDet = 1.0f / det;
	*t *= fInvDet;
	*u *= fInvDet;
	*v *= fInvDet;

	//D3DXVECTOR3 e3 = v2 - v1;
	CrashPoint = orig + (*t) * dir;
	//D3DXVECTOR3 i0 = v0 + edge1 * (*u) + edge2 * (*v);
	//
	//this->vPickRayOrig = orig;
	//this->vPickRayDir = dir;
	return true;
}
/*AABB�ڽ� �浹 Ȯ���Լ�*/
bool BSelect::IntersectBox(B_BOX* pBox, B_RAY* pRay)
{
	if (pRay == NULL) pRay = &m_Ray;
	D3DXVECTOR3 tmin;
	tmin.x = (pBox->vMin.x - pRay->vOrigin.x) / pRay->vDirection.x + 0.001f; // 0.001f �� �ٴ� ������ x�� 0�� ��츦 �����ؼ� (���⺤���� x���� 0�ΰ��)
	tmin.y = (pBox->vMin.y - pRay->vOrigin.y) / pRay->vDirection.y + 0.001f;
	tmin.z = (pBox->vMin.z - pRay->vOrigin.z) / pRay->vDirection.z + 0.001f;

	D3DXVECTOR3 tmax;
	tmax.x = (pBox->vMax.x - pRay->vOrigin.x) / pRay->vDirection.x + 0.001f; // 0.001f �� �ٴ� ������ x�� 0�� ��츦 �����ؼ� (���⺤���� x���� 0�ΰ��)
	tmax.y = (pBox->vMax.y - pRay->vOrigin.y) / pRay->vDirection.y + 0.001f;
	tmax.z = (pBox->vMax.z - pRay->vOrigin.z) / pRay->vDirection.z + 0.001f;

	/*������ �ڽ��� ��� ���⿡�� ������ Ȯ���� �����ϰ� �ϱ� ����*/
	D3DXVECTOR3 real_min;
	real_min.x = min(tmin.x, tmax.x);
	real_min.y = min(tmin.y, tmax.y);
	real_min.z = min(tmin.z, tmax.z);

	D3DXVECTOR3 real_max;
	real_max.x = max(tmin.x, tmax.x);
	real_max.y = max(tmin.y, tmax.y);
	real_max.z = max(tmin.z, tmax.z);

	float minmax = min(min(real_max.x, real_max.y), real_max.z);
	float maxmin = max(max(real_min.x, real_min.y), real_min.z);

	if (minmax >= maxmin)
	{
		m_vIntersection = pRay->vOrigin + pRay->vDirection * maxmin;
		return true;
	}
	return false;
}
/*OBB�ڽ� �浹 Ȯ�� �Լ�*/
bool BSelect::CheckOBBToRay(B_BOX* pBox, B_RAY* pRay)
{
	if (pRay == NULL) pRay = &m_Ray;
	float t_min = -999999.0f;
	float t_max = 999999.0f;
	float f[3], fa[3], s[3], sa[3];
	// f = �� ��� ������ ���⺤���� ������
	// fa = f ���� ���밪
	// s = �� ��� ������ ���������� �ڽ����ͱ����� ������ ������
	// sa = s ���� ���밪


	/*vR�� �ڽ��� ���� -> ������ ������������ ����*/
	D3DXVECTOR3 vR = pRay->vOrigin - pBox->vCenter;

	for (int v = 0; v < 3; v++) // ���⼭ v�� �� 3�� x, y, z ���� �ǹ�
	{
		f[v] = D3DXVec3Dot(&pBox->vAxis[v], &pRay->vDirection); // f[v] = �ڽ��� v��° ��� ������ ���⺤���� ������
		s[v] = D3DXVec3Dot(&pBox->vAxis[v], &vR); // s[v] = �ڽ��� v��° ��� ������ ���������� �ڽ����ͱ����� ������ ������ = v��° ���� �������� ���̿� �ڽ� �߽� ������ �Ÿ�
		fa[v] = fabs(f[v]);
		sa[v] = fabs(s[v]);

		// v ��° ���� �������� ���̿� �ڽ� �߽ɻ����� �Ÿ��� �ڽ��� v��° �� ũ�⺸�� ũ�� && 
		// (�� �ڽ��� v����� �ܺο� ��ġ�ϰ�) 
		// �� ��Į���� ���� ���� + ��°��� ���� �������� ���ϰ� �����Ƿ� ���� ���ɼ��� ����
		if (sa[v] > pBox->fExtent[v] && s[v] * f[v] >= 0.0f)
			return false;

		float t1 = (-s[v] - pBox->fExtent[v]) / f[v];
		float t2 = (-s[v] + pBox->fExtent[v]) / f[v];
		if (t1 > t2)
		{
			swap(t1, t2); // t1�� ������ t2���� �۵���
		}
		t_min = max(t_min, t1);
		t_max = min(t_max, t2);
		if (t_min > t_max)
			return false;
	}

	// D X pBox->vAxis[0] �и��� (x��� ������ ���⺤�Ͱ��� �������� ���� �и���)
	float fCross[3], fRhs;
	D3DXVECTOR3 vDxR;
	D3DXVec3Cross(&vDxR, &pRay->vDirection, &vR);
	fCross[0] = fabs(D3DXVec3Dot(&vDxR, &pBox->vAxis[0]));
	float fAxis2 = pBox->fExtent[1] * fa[2];
	float fAxis1 = pBox->fExtent[2] * fa[1];
	fRhs = fAxis2 + fAxis1;
	if (fCross[0] > fRhs)
	{
		m_vDxR = vDxR;
		return false;
	}

	// D X pBox->vAxis[1] �и��� (y��� ������ ���⺤�Ͱ��� �������� ���� �и���)
	fCross[1] = fabs(D3DXVec3Dot(&vDxR, &pBox->vAxis[1]));
	fAxis2 = pBox->fExtent[0] * fa[2];
	fAxis1 = pBox->fExtent[2] * fa[0];
	fRhs = fAxis2 + fAxis1;
	if (fCross[1] > fRhs)
	{
		m_vDxR = vDxR;
		return false;
	}

	// D X pBox->vAxis[2] �и��� (y��� ������ ���⺤�Ͱ��� �������� ���� �и���)
	fCross[2] = fabs(D3DXVec3Dot(&vDxR, &pBox->vAxis[1]));
	fAxis2 = pBox->fExtent[0] * fa[1];
	fAxis1 = pBox->fExtent[1] * fa[0];
	fRhs = fAxis2 + fAxis1;
	if (fCross[2] > fRhs)
	{
		m_vDxR = vDxR;
		return false;
	}
	return true;
}
/*�� �浹 Ȯ�� �Լ�*/
bool BSelect::IntersectRayToSphere(BSphere* pSphere, B_RAY* pRay)
{
	if (pRay == NULL) pRay = &m_Ray;
	// P0 - C
	D3DXVECTOR3 vDir = pRay->vOrigin - pSphere->m_vCenter;
	// B = 2(u dot (p0 - c ))
	float b = 2.0f * D3DXVec3Dot(&pRay->vDirection, &vDir);
	// C = (p0 - c) dot (p0 - c) - r*r
	float c = D3DXVec3Dot(&vDir, &vDir) - (pSphere->m_fRadius * pSphere->m_fRadius);
	// D = B*B - 4*AC;
	float fDiscriminant = (b*b) - (4.0f*c);
	if (fDiscriminant < 0.0f)
	{
		return false;
	}
	fDiscriminant = sqrtf(fDiscriminant);
	float t0 = (-b + fDiscriminant) / 2.0f;
	float t1 = (-b - fDiscriminant) / 2.0f;

	if (t0 >= 0.0f)
	{
		m_vIntersection = pRay->vOrigin + pRay->vDirection * t0;
		return true;
	}
	if (t1 >= 0.0f)
	{
		m_vIntersection = pRay->vOrigin + pRay->vDirection * t1;
		return true;
	}
	return false;
}
/*OBB OBB �� �浹 Ȯ�� �Լ�*/
bool BSelect::CheckOBBToOBB(B_BOX& box0, B_BOX& box1)
{
	D3DXVECTOR3 D(box1.vCenter.x - box0.vCenter.x, box1.vCenter.y - box0.vCenter.y, box1.vCenter.z - box0.vCenter.z);
	float C[3][3]; // �� OBB�ڽ��� �ణ�� ������
	float absC[3][3]; // C�� ���밪
	float AD[3]; // �� OBB�� ��� OBB�ڽ����Ͱ��� ���⺤���� ������
	float R0, R1, R; // R = AD���� ���밪 / R0, R1 = absC ������ �� (box�� �浹���θ� �Ǵ��ϱ� ���� ��)
	float R01; // box1�� �浹���� Ȯ�ΰ� + box0�� �浹���� Ȯ�ΰ�

			   //A0 ~ 2 �и����� box0�� x, y, z
	for (int vAxisCnt = 0; vAxisCnt < 3; vAxisCnt++)
	{
		C[vAxisCnt][0] = D3DXVec3Dot(&box0.vAxis[vAxisCnt], &box1.vAxis[0]);
		C[vAxisCnt][1] = D3DXVec3Dot(&box0.vAxis[vAxisCnt], &box1.vAxis[1]);
		C[vAxisCnt][2] = D3DXVec3Dot(&box0.vAxis[vAxisCnt], &box1.vAxis[2]);
		AD[vAxisCnt] = D3DXVec3Dot(&box0.vAxis[vAxisCnt], &D);
		absC[vAxisCnt][0] = (float)fabs(C[vAxisCnt][0]);
		absC[vAxisCnt][1] = (float)fabs(C[vAxisCnt][1]);
		absC[vAxisCnt][2] = (float)fabs(C[vAxisCnt][2]);
		R = (float)fabs(AD[vAxisCnt]);
		R1 = box1.fExtent[0] * absC[vAxisCnt][0] + box1.fExtent[1] * absC[vAxisCnt][1] + box1.fExtent[2] * absC[vAxisCnt][2];
		R01 = R1 + box0.fExtent[vAxisCnt];
		if (R > R01) return false;
	}

	//B0 ~ 2 �и����� box1�� x, y, z
	for (int vAxisCnt = 0; vAxisCnt < 3; vAxisCnt++)
	{
		R = (float)fabs(D3DXVec3Dot(&box1.vAxis[vAxisCnt], &D));
		R0 = box0.fExtent[0] * absC[0][vAxisCnt] + box0.fExtent[1] * absC[1][vAxisCnt] + box0.fExtent[2] * absC[2][vAxisCnt];
		R01 = R0 + box1.fExtent[vAxisCnt];
		if (R > R01) return false;
	}

	// �и����� �� OBB�� ��� ���� �������� ���� ���� ���
	// A0 x B0 
	R = (float)fabs(AD[2] * C[1][0] - AD[1] * C[2][0]);
	R0 = box0.fExtent[1] * absC[2][0] + box0.fExtent[2] * absC[1][0];
	R1 = box1.fExtent[1] * absC[0][2] + box1.fExtent[2] * absC[0][1];
	R01 = R0 + R1;
	if (R > R01)return false;

	// A0 x B1 
	R = (float)fabs(AD[2] * C[1][1] - AD[1] * C[2][1]);
	R0 = box0.fExtent[1] * absC[2][1] + box0.fExtent[2] * absC[1][1];
	R1 = box1.fExtent[0] * absC[0][2] + box1.fExtent[2] * absC[0][0];
	R01 = R0 + R1;
	if (R > R01)return false;

	// A0 x B2 
	R = (float)fabs(AD[2] * C[1][2] - AD[1] * C[2][2]);
	R0 = box0.fExtent[1] * absC[2][2] + box0.fExtent[2] * absC[1][2];
	R1 = box1.fExtent[0] * absC[0][1] + box1.fExtent[1] * absC[0][0];
	R01 = R0 + R1;
	if (R > R01)return false;

	// A1 x B0 
	R = (float)fabs(AD[0] * C[2][0] - AD[2] * C[0][0]);
	R0 = box0.fExtent[0] * absC[2][0] + box0.fExtent[2] * absC[0][0];
	R1 = box1.fExtent[1] * absC[1][2] + box1.fExtent[2] * absC[1][1];
	R01 = R0 + R1;
	if (R > R01)return false;

	// A1 x B1
	R = (float)fabs(AD[0] * C[2][1] - AD[2] * C[0][1]);
	R0 = box0.fExtent[0] * absC[2][1] + box0.fExtent[2] * absC[0][1];
	R1 = box1.fExtent[0] * absC[1][2] + box1.fExtent[2] * absC[1][0];
	R01 = R0 + R1;
	if (R > R01)return false;

	// A1 x B2 
	R = (float)fabs(AD[0] * C[2][2] - AD[2] * C[0][2]);
	R0 = box0.fExtent[0] * absC[2][2] + box0.fExtent[2] * absC[0][2];
	R1 = box1.fExtent[0] * absC[1][1] + box1.fExtent[1] * absC[1][0];
	R01 = R0 + R1;
	if (R > R01)return false;

	// A2 x B0 
	R = (float)fabs(AD[1] * C[0][0] - AD[0] * C[1][0]);
	R0 = box0.fExtent[0] * absC[1][0] + box0.fExtent[1] * absC[0][0];
	R1 = box1.fExtent[1] * absC[2][2] + box1.fExtent[2] * absC[2][1];
	R01 = R0 + R1;
	if (R > R01)return false;

	// A2 x B1 
	R = (float)fabs(AD[1] * C[0][1] - AD[0] * C[1][1]);
	R0 = box0.fExtent[0] * absC[1][1] + box0.fExtent[1] * absC[0][1];
	R1 = box1.fExtent[0] * absC[2][2] + box1.fExtent[2] * absC[2][0];
	R01 = R0 + R1;
	if (R > R01)return false;

	// A2 x B2 
	R = (float)fabs(AD[1] * C[0][2] - AD[0] * C[1][2]);
	R0 = box0.fExtent[0] * absC[1][2] + box0.fExtent[1] * absC[0][2];
	R1 = box1.fExtent[0] * absC[2][1] + box1.fExtent[1] * absC[2][0];
	R01 = R0 + R1;
	if (R > R01)return false;

	return true;
}
/*��Ʈ��� �浹 Ȯ�ο� �Լ�*/
BNode* BSelect::CheckRootNode(BNode* RootNode, B_RAY* pRay)
{
	if (pRay == NULL) pRay = &m_Ray;

	/*v0 = m_pBoxShape->m_VertexList[m_pBoxShape->m_IndexList[iFace * 3 + 0]].Position;
	v1 = m_pBoxShape->m_VertexList[m_pBoxShape->m_IndexList[iFace * 3 + 1]].Position;
	v2 = m_pBoxShape->m_VertexList[m_pBoxShape->m_IndexList[iFace * 3 + 2]].Position;

	if (I_Input.m_DIMouseState.rgbButtons[0] && m_Select.CheckPick(v0, v1, v2))
	{
	return;
	}*/

	if (IntersectBox(&RootNode->m_tBox))
	{
		return CheckNode(RootNode);
	}
	else
	{
		return NULL;
	}
}
/*��� �浹 Ȯ�ο� �Լ�(CheckRootNode ���� ȣ�� / ���)*/
BNode* BSelect::CheckNode(BNode* pNode, B_RAY* pRay)
{
	if (pRay == NULL) pRay = &m_Ray;
	if (pNode->m_pChild[0] != NULL)
	{
		for (DWORD dwChild = 0; dwChild < 4; dwChild++)
		{
			if (IntersectBox(&pNode->m_pChild[dwChild]->m_tBox))
			{
				BNode* returnNode = CheckNode(pNode->m_pChild[dwChild]);
				return returnNode;
			}
		}
	}
	else
	{
		return pNode;
	}
}
#pragma endregion



BSelect::BSelect()
{
}
BSelect::~BSelect()
{
}
