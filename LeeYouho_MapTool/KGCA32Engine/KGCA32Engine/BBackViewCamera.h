#pragma once
#include "BCamera.h"

//====================================================================================//
/*BackView 카메라의 경우 Init에 넘겨준 타겟값을 기준으로 연산을 합니다.*/
/*이외의 사용법은 카메라 클래스와 동일합니다.*/
//====================================================================================//

class BBackViewCamera : public BCamera
{
public:
	D3DXVECTOR3 m_vCameraDestination;  // 카메라 보간이동의 목표지점
	D3DXVECTOR3 m_vObjectVector[3];	   // 대상의 월드변환 행렬의 방향백터
	float m_fPrevRadius;			   // 대상과 카메라의 거리
	D3DXVECTOR3 m_vHeroLook;		   // 대상의 시선벡터
	D3DXVECTOR3 m_vHeroUp;			   // 대상의 상향벡터
	D3DXVECTOR3 m_vHeroSide;		   // 대상의 사이드벡터

	D3DXVECTOR3 m_vDefaultEye;		   // 카메라의 초기 위치
	D3DXVECTOR3 m_vDefaultLookAt;	   // 카메라의 초기 타겟

public:
	/* 재정의 # 카메라 생성 */
	virtual bool Init(D3DXVECTOR3 vTarget);
	/* 재정의 # 카메라 연산 (오브젝트를 기준으로 연산) */
	virtual bool Frame();

	/* 재정의 # 뷰행렬 생성함수(오브젝트의 방향벡터를 기준으로 이동)*/
	virtual bool SetViewMatrix(D3DXVECTOR3 vPos, D3DXVECTOR3 vTarget, D3DXVECTOR3 vUp);

	/* 재정의 # 카메라 회전 연산함수*/
	virtual bool UpdateQuaternion();
	/* 재정의 # 카메라 회전 반영함수*/
	virtual bool UpdateVector();
	

public:
	BBackViewCamera();
	virtual ~BBackViewCamera();
};

