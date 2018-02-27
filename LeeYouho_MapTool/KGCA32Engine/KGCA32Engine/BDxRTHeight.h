#pragma once
#include "BDxRT.h"
#include "BMap.h"
#include "BMapTileRender.h"

//====================================================================================//
/*WaterMap의 반사, 굴절등의 이미지에 사용되는 클래스입니다.*/
/*높이맵을 통한 텍스쳐 생성과 적용이 가능합니다.*/
/*기본적인 사용법은 BDxRT 클래스와 동일합니다.*/
//====================================================================================//

class BDxRTHeight : public BDxRT
{
public:
	ComPtr<ID3D11Texture2D> m_pHeightTexture; // 높이맵 저장 텍스쳐

public:
	/*뷰포트 생성함수 # 재정의*/
	HRESULT Create(ID3D11Device* pd3dDevice, DXGI_FORMAT format, FLOAT Width = 1024, FLOAT Height = 1024);

	/*생성된 높이맵 텍스쳐를 이전하는 함수*/
	HRESULT DrawHeightTex(ID3D11Device* pd3dDevice, ID3D11DeviceContext* m_pContext, BMap* m_pMap);

	//====================================================================================//
	/*DrawQuadTreeHeightTex 함수*/
	// 메인 맵의 높이 변경시 호출이 꼭 필요합니다. ( 맵 생성, 변경 등 )
	// WaterMap의 색상이 해당 함수에 의해 적용된 높이에 비례하여 투명도가 적용됩니다.
	//====================================================================================//
	/*맵의 높이를 뷰포트 텍스쳐에 적용하는 함수*/
	HRESULT DrawQuadTreeHeightTex(ID3D11Device* pd3dDevice, ID3D11DeviceContext* m_pContext, BMapTileRender* m_pMap);
	/*쿼드트리의 높이값을 높이맵에 적용시키는 함수(DrawQuadTreeHeightTex 내부 호출)*/
	void DrawNodeHeightTex(BNode* pNode, BYTE* pDestBytes, int iChangePoint, float fCellDistance, UINT RowPitch, float fRatio, float fMin);
	/*쿼드트리의 MaxY 값 갱신함수(DrawQuadTreeHeightTex 내부 호출)*/
	void SetRootNodeYPosition(BNode* pNode, float* MaxY, float* MinY);

public:
	BDxRTHeight();
	~BDxRTHeight();
};

