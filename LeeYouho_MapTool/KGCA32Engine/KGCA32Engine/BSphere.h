#pragma once
#include "BObject.h"

//====================================================================================//
/*오브젝트와 사용법은 동일합니다.*/
/*UpdateQuaternion을 통해 구의 움직임 컨트롤 합니다.*/
//====================================================================================//


class BSphere : public BObject
{
public:
	vector<D3DXVECTOR3> m_VertexArray;		// 구의 버텍스리스트 (연산용)
	UINT m_Slices;							// 구의 슬라이스 값
	double m_fRadius;						// 구의 반지름
	D3DXVECTOR3 m_vCenter;					// 구의 중점

public:
	/*재정의 # 버텍스 버퍼 생성함수*/
	virtual HRESULT CreateVertexBuffer();
	/*재정의 # 랜더 람수*/
	virtual bool PostRender(ID3D11DeviceContext* pContext);
	/*구의 이동 함수*/
	bool UpdateQuaternion();
	

public:
	BSphere();
	~BSphere();
};

