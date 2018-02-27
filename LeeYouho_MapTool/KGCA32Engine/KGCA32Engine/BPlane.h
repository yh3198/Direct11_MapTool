#pragma once
#include "BObject.h"

//====================================================================================//
/*기본 평면 클래스입니다.*/
/*오브젝트와 사용법은 동일합니다.*/
/*CreatePlane을 통해 맴버변수 fA, fB, fC, fD를 갱신하여 평면의 방정식 생성이 가능합니다.*/
//====================================================================================//

class BPlane : public BObject
{
public:
	float fA, fB, fC, fD; // 평면의 방정식용 변수

public:
	/*재정의 # 버텍스 버퍼 생성 함수*/
	virtual HRESULT CreateVertexBuffer();
	/*재정의 # 인덱스 버퍼 생성 함수*/
	virtual HRESULT CreateIndexBuffer();
	/*재정의 # MinMax박스 갱신 함수*/
	void UpdateVertexMinMax();

	/*평면의 방정식 생성함수 (3개의 버텍스 이용)*/
	bool CreatePlane(D3DXVECTOR3 v0, D3DXVECTOR3 v1, D3DXVECTOR3 v2);
	/*평면의 방정식 생성함수 (노말벡터와 1개의 버텍스 이용)*/
	bool CreatePlane(D3DXVECTOR3 vNormal, D3DXVECTOR3 v0);
	
public:
	BPlane();
	~BPlane();
};

