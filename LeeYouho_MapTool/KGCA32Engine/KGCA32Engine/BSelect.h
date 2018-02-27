#pragma once
#include "BSphere.h"
#include "BQuadTree.h"

/*Select ���� ���� ����ü*/
struct B_RAY
{
	float fExtent;			 // ���� Ȯ���	
	D3DXVECTOR3 vOrigin;	 // ������ = ����ǥ���� ī�޶��� ����
	D3DXVECTOR3 vDirection;  // RAY�� �������
};

//====================================================================================//
/*���콺 �浹������ ���� Ŭ�����Դϴ�.*/
/*SetMatrix�� ���� ������ ��ļ��ð� Update�� ���� ������ ��ġ Ȯ���� �浹üũ���� �̷������ �մϴ�.*/
//====================================================================================//

class BSelect
{
public:
	B_RAY m_Ray;					// ���콺�� ���� ������ ����
	D3DXVECTOR3 m_vIntersection;	// �浹����
	D3DXMATRIX m_matWorld;			// ���� ��ȯ�� �������
	D3DXMATRIX m_matView;			// ���� ��ȯ�� �����
	D3DXMATRIX m_matProj;			// ���� ��ȯ�� ����������
	D3DXVECTOR3 m_vDxR;				// �и���				
	D3DXVECTOR3 m_vCenter;			// ������ ����

	D3DXVECTOR3 m_vSrcVertex[3];	// �浹 ������ ���̽� ���� ���ؽ�
	D3DXMATRIX m_matWorldPick;		// �浹 ���������� �������
	D3DXVECTOR3 Qvec;				// OBB�浹���� Q����
	D3DXVECTOR3 Tvec;				// OBB�浹���� T����
	D3DXVECTOR3 Pvec;				// OBB�浹���� P����

	D3DXVECTOR3 CrashPoint;			// ��ȯ�� �浹����

public:
	/*���� ��� ���ÿ� �Լ�*/
	void SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj);
	/*���� ��ġ ���ÿ� �Լ�*/
	void Update();

	/*�� ������ �̷���� ���̽� �浹 Ȯ���Լ�*/
	bool CheckPick(D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2);
	/*�簢�� �浹 Ȯ���Լ�*/
	bool IntersectRect(const D3DXVECTOR3& orig, const D3DXVECTOR3& dir,B_BOX m_tBox, FLOAT* t, FLOAT *u, FLOAT *v);
	/*�ﰢ�� �浹 Ȯ���Լ� (IntersectRect ���� ȣ��)*/
	bool IntersectTriangle(const D3DXVECTOR3& orig, const D3DXVECTOR3& dir, D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2, FLOAT* t, FLOAT *u, FLOAT *v);
	/*AABB�ڽ� �浹 Ȯ���Լ�*/
	bool IntersectBox(B_BOX* pBox, B_RAY* pRay = NULL);
	/*OBB�ڽ� �浹 Ȯ�� �Լ�*/
	bool CheckOBBToRay(B_BOX* pBox, B_RAY* pRay = NULL);
	/*�� �浹 Ȯ�� �Լ�*/
	bool IntersectRayToSphere(BSphere* pSphere, B_RAY* pRay);
	/*OBB OBB �� �浹 Ȯ�� �Լ�*/
	bool CheckOBBToOBB(B_BOX& box0, B_BOX& box1);
	/*��Ʈ��� �浹 Ȯ�ο� �Լ�*/
	BNode* CheckRootNode(BNode* RootNode, B_RAY* pRay = NULL);
	/*��� �浹 Ȯ�ο� �Լ�(CheckRootNode ���� ȣ�� / ���)*/
	BNode* CheckNode(BNode* pNode, B_RAY* pRay = NULL);

public:
	BSelect();
	~BSelect();
};

