#pragma once
#include "BSphere.h"
#include "BQuadTree.h"

/*Select 전용 레이 구조체*/
struct B_RAY
{
	float fExtent;			 // 레이 확장률	
	D3DXVECTOR3 vOrigin;	 // 시작점 = 뷰좌표계의 카메라의 원점
	D3DXVECTOR3 vDirection;  // RAY의 방향백터
};

//====================================================================================//
/*마우스 충돌판정을 위한 클래스입니다.*/
/*SetMatrix를 통한 레이의 행렬세팅과 Update를 통한 레이의 위치 확인이 충돌체크전에 이루어져야 합니다.*/
//====================================================================================//

class BSelect
{
public:
	B_RAY m_Ray;					// 마우스를 통해 생성된 레이
	D3DXVECTOR3 m_vIntersection;	// 충돌지점
	D3DXMATRIX m_matWorld;			// 레이 변환용 월드행렬
	D3DXMATRIX m_matView;			// 레이 변환용 뷰행렬
	D3DXMATRIX m_matProj;			// 레이 변환용 브로젝션행렬
	D3DXVECTOR3 m_vDxR;				// 분리축				
	D3DXVECTOR3 m_vCenter;			// 레이의 센터

	D3DXVECTOR3 m_vSrcVertex[3];	// 충돌 판정용 페이스 구성 버텍스
	D3DXMATRIX m_matWorldPick;		// 충돌 지점에서의 월드행렬
	D3DXVECTOR3 Qvec;				// OBB충돌시의 Q벡터
	D3DXVECTOR3 Tvec;				// OBB충돌시의 T벡터
	D3DXVECTOR3 Pvec;				// OBB충돌시의 P벡터

	D3DXVECTOR3 CrashPoint;			// 변환용 충돌지점

public:
	/*레이 행렬 세팅용 함수*/
	void SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj);
	/*레이 위치 세팅용 함수*/
	void Update();

	/*세 점으로 이루어진 페이스 충돌 확인함수*/
	bool CheckPick(D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2);
	/*사각형 충돌 확인함수*/
	bool IntersectRect(const D3DXVECTOR3& orig, const D3DXVECTOR3& dir,B_BOX m_tBox, FLOAT* t, FLOAT *u, FLOAT *v);
	/*삼각형 충돌 확인함수 (IntersectRect 내부 호출)*/
	bool IntersectTriangle(const D3DXVECTOR3& orig, const D3DXVECTOR3& dir, D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2, FLOAT* t, FLOAT *u, FLOAT *v);
	/*AABB박스 충돌 확인함수*/
	bool IntersectBox(B_BOX* pBox, B_RAY* pRay = NULL);
	/*OBB박스 충돌 확인 함수*/
	bool CheckOBBToRay(B_BOX* pBox, B_RAY* pRay = NULL);
	/*구 충돌 확인 함수*/
	bool IntersectRayToSphere(BSphere* pSphere, B_RAY* pRay);
	/*OBB OBB 간 충돌 확인 함수*/
	bool CheckOBBToOBB(B_BOX& box0, B_BOX& box1);
	/*루트노드 충돌 확인용 함수*/
	BNode* CheckRootNode(BNode* RootNode, B_RAY* pRay = NULL);
	/*노드 충돌 확인용 함수(CheckRootNode 내부 호출 / 재귀)*/
	BNode* CheckNode(BNode* pNode, B_RAY* pRay = NULL);

public:
	BSelect();
	~BSelect();
};

