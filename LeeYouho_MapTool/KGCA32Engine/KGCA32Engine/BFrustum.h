#pragma once
#include "BStd.h"
#include "BPlane.h"
#include "BSphere.h"
#include "BBox.h"

class BFrustum
{
public:
	D3DXMATRIX m_matWorld;		 // 카메라의 월드행렬
	D3DXMATRIX m_matView;		 // 카메라의 뷰행렬
	D3DXMATRIX m_matProj;		 // 카메라의 프로젝션행렬
	D3DXMATRIX m_matViewProj;    // 프로스텀 정점을 월드행렬로 보내기 위한 행렬

	D3DXVECTOR3 m_vFrustum[8];	 // 프로스텀을 구성하는 정점
	BPlane m_Plane[6];		     // 프로스텀을 구성하는 면
	BBox* m_pBoxShape;		     // 프로스텀 박스

public:
	/*프로스텀 생성함수*/
	bool CreateFrustum(ID3D11Device* m_pd3dDevice, const TCHAR* pShaderFile);

	/*프로스텀 충돌 체크함수*/
	BOOL CheckPoint(D3DXVECTOR3* pv);
	BOOL CheckSphere(BSphere* pSphere);
	BOOL CheckOBB(B_BOX* pBox);
	T_POSITION CheckPositionOBBInPlane(B_BOX* pBox);

	/*프로스텀 랜더 세팅함수*/
	bool FrustRender(ID3D11DeviceContext* pContext);  /*m_pBoxShape의 랜더링 과정을 따로 거쳐야 합니다*/

public:
	BFrustum();
	virtual ~BFrustum();
};

