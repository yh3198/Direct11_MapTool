#pragma once
#include "BObject.h"

//====================================================================================//
/*기본 박스 클래스입니다.*/
/*오브젝트와 사용법은 동일합니다.*/
//====================================================================================//


class BBox : public BObject
{
public:
	/*재정의 # 버텍스 버퍼 생성함수*/
	virtual HRESULT CreateVertexBuffer();
	/*재정의 # 인덱스 버퍼 생성함수*/
	virtual HRESULT CreateIndexBuffer();
	/*재정의 # MinMax 박스 갱신함수*/
	virtual void UpdateVertexMinMax();
public:
	BBox();
	~BBox();
};

