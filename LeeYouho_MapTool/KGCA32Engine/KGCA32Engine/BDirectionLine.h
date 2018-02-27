#pragma once
#include "BLine.h"

//====================================================================================//
/*기본 라인 클래스와 동일하게 사용 가능한 방향라인입니다.*/
/*라인 클래스와 달리 라인의 위치설정이 필요 없습니다.*/
/*토폴로지 세팅을 PreRender에서 행하기에 별도의 세팅이 필요하지 않습니다.*/
//====================================================================================//


class BDirectionLine : public BLine
{
public:
	/*재정의 # 버텍스 버퍼 생성함수*/
	HRESULT CreateVertexBuffer();
	/*재정의 # 인덱스 버퍼 생성함수*/
	HRESULT CreateIndexBuffer();
	/*재정의 # 랜더 세팅함수(토폴로지 재설정)*/
	bool PreRender(ID3D11DeviceContext* pContext);

public:
	/*생성자에서 기본 토폴로지를 세팅합니다.*/
	BDirectionLine();
	~BDirectionLine();
};

