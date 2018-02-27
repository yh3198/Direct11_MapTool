#pragma once
#include "BObject.h"

//====================================================================================//
/*SetMapDesc함수를 통해 생성할 맵의 세부 수치 설정 후 Create 함수를 통해 생성해야 합니다.*/
//====================================================================================//

class BMap : public BObject
{
public:
	BMapDesc m_MapDesc;					// 맵 기본 세팅 변수
	vector<D3DXVECTOR3> m_VertexArray;	// 맵 구성 버텍스 배열
	vector<float>		m_fHeightList;  // 맵 구성 높이 배열
	TCHAR* m_MapTexture;				// 맵 텍스쳐 이름

	float m_iNumRows;					// 가로 버텍스 (행)
	float m_iNumCols;					// 세로 버텍스 (열)
	float m_iNumCellRows;				// 가로 셀의 갯수
	float m_iNumCellCols;				// 세로 셀의 갯수
	int m_fCellDistance;				// 셀 하나의 길이 (정방형이므로 가로 세로 동일)

	ID3D11PixelShader* m_pAlphaPS;		// 알파맵 픽셀 쉐이더
	D3DXVECTOR3 m_vLightDir;			// 디렉션 라이트

public:
	/*기본 구성 함수*/
	virtual bool Frame();
	virtual bool Release();

	/*맵 생성함수*/
	HRESULT Create(ID3D11Device* pDevice, ID3D11DeviceContext* m_pImmediateContext, TCHAR** pLoadTextureString, int TextureCnt, TCHAR* HeightMapString = NULL, const TCHAR* ShaderString = NULL);
	/*높이맵 생성 함수*/
	bool CreateHeightMap(TCHAR* strHeightMapText);
	/*맵 정보 세팅 (높이맵 생성시에도 호출)*/
	bool SetMapDesc(BMapDesc MapDesc);
	
	/* 재정의 # 버텍스 버퍼 생성 함수 */
	virtual HRESULT CreateVertexBuffer();	
	/* 재정의 # 인덱스 버퍼 생성 함수 */
	virtual HRESULT CreateIndexBuffer();		
	/* 재정의 # 맵 쉐이더 레이아웃 생성 함수*/
	HRESULT LoadShaderAndInputLayout(const TCHAR* pShaderFile);

	/*맵의 해당위치 높이 반환 함수 ( int 기준 ) */
	virtual float GetHeightMap(int row, int col);
	/*맵의 해당위치 높이 반환 함수 ( float 기준 )*/
	virtual float GetHeightMap(float row, float col);
	/*맵의 해당 인덱스의 노말 반환 함수*/
	virtual D3DXVECTOR3 GetNormalOfVertex(UINT Index);
	
	/* 재정의 # 노말 & 접선벡터 생성함수*/
	bool UpdateNormal();
	/* 재정의 # 맵 노말에 따른 색상 변경함수*/
	void CalcVertexColor();

	/*맵 보간용 함수*/
	float Lerp(float fStart, float fEnd, float fTangent);

	/* 버텍스 버퍼 갱신함수 */
	bool ReLoadVBuffer();
		

	

public:
	BMap();
	~BMap();
};

