#pragma once
#include "BObject.h"

//====================================================================================//
/*Create 함수로 생성합니다.*/
/*m_iPrimitiveType의 값을 선형인 2로 설정해 두어야 합니다.*/
/*Frame 에서 SetMatrix와 라인의 형태를 지정해주기 위해 SetLineStartToMove || SetLineStartToEnd 를 이용합니다. */
/*Render를 통해 출력합니다.*/
//====================================================================================//


class BLine : public BObject
{
public:
	vector<PNCT_Vertex>	m_LineVertexList;	// 라인 버텍스 리스트

public:
	/*재정의 # 쉐이더 및 레이아웃 생성 함수*/
	HRESULT LoadShaderAndInputLayout(const TCHAR* pShaderFile);
	/*재정의 # 버텍스 버퍼 생성 함수*/
	HRESULT CreateVertexBuffer();
	/*재정의 # 인덱스 버퍼 생성 함수*/
	HRESULT CreateIndexBuffer();

	/*라인 위치설정 함수(시작점 + 이동벡터)*/
	bool SetLineStartToMove(ID3D11DeviceContext* pContext, D3DXVECTOR3 vStart, D3DXVECTOR3 Move, D3DXVECTOR4 vColor);
	/*라인 위치설정 함수(시작점, 끝점)*/
	bool SetLineStartToEnd(ID3D11DeviceContext* pContext, D3DXVECTOR3 vStart, D3DXVECTOR3 vEnd, D3DXVECTOR4 vColor);

public:
	BLine();
	~BLine();
};

