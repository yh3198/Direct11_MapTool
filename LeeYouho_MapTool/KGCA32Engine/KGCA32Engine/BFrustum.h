#pragma once
#include "BStd.h"
#include "BPlane.h"
#include "BSphere.h"
#include "BBox.h"

class BFrustum
{
public:
	D3DXMATRIX m_matWorld;		 // ī�޶��� �������
	D3DXMATRIX m_matView;		 // ī�޶��� �����
	D3DXMATRIX m_matProj;		 // ī�޶��� �����������
	D3DXMATRIX m_matViewProj;    // ���ν��� ������ ������ķ� ������ ���� ���

	D3DXVECTOR3 m_vFrustum[8];	 // ���ν����� �����ϴ� ����
	BPlane m_Plane[6];		     // ���ν����� �����ϴ� ��
	BBox* m_pBoxShape;		     // ���ν��� �ڽ�

public:
	/*���ν��� �����Լ�*/
	bool CreateFrustum(ID3D11Device* m_pd3dDevice, const TCHAR* pShaderFile);

	/*���ν��� �浹 üũ�Լ�*/
	BOOL CheckPoint(D3DXVECTOR3* pv);
	BOOL CheckSphere(BSphere* pSphere);
	BOOL CheckOBB(B_BOX* pBox);
	T_POSITION CheckPositionOBBInPlane(B_BOX* pBox);

	/*���ν��� ���� �����Լ�*/
	bool FrustRender(ID3D11DeviceContext* pContext);  /*m_pBoxShape�� ������ ������ ���� ���ľ� �մϴ�*/

public:
	BFrustum();
	virtual ~BFrustum();
};

