#pragma once
#include "BNode.h"
typedef vector<BNode*> BNODE_VECTOR;

//====================================================================================//
/*Update, SetLOD를 통해 기본 LOD변수 세팅 후 InitLevelOrder를 통해 레벨을 생성합니다.*/
/*GetExpansionRatio의 인자값을 통해 LOD 적용 방식 변화가 가능합니다.*/
/*적용방식은 중점으로부터의 거리, 최대 최소값, 카메라와 중점의 거리비를 통한 비율로 결정됩니다. */
//====================================================================================//


class BMapLOD
{
public:
	ID3D11Device* m_pd3dDevice;				// 메인 3D 디바이스
	BCamera* m_pCamera;						// 메인 카메라
	vector<BNODE_VECTOR> m_LevelList;		// 노드 레벨리스트
	vector<BNode*> m_DrawPatchNodeList;		// 랜더 노드 리스트 (전체 노드 기준)
	vector<BNode*> m_DrawNodeList;			// 랜더 노드 리스트 (리프 노드 기준)
	float m_fExpansionRatio;				// LOD 비율 (화면 투영 비율( 0 ~ 1 범위 사용 ))
	int m_iNumCell;							// 트리의 레벨의 깊이에 따른 셀 개수
	int m_iPatchLodCount;					// LOD 레벨 개수 ( z = pow (x,y) 에서 y = log(z) / log(x) )
	bool m_bThresHoldValue;					// 문턱효과 사용여부
	
public:
	/*메인 카메라, 디바이스 세팅함수*/
	virtual void	Update(ID3D11Device* pd3dDevice, BCamera* pCamera = NULL);
	/*LOD 레벨 세팅함수*/
	virtual void	SetLOD(DWORD dwWidth, int iNumDepth);
	/*노드 레벨리스트 생성함수*/
	virtual void	InitLevelOrder(BNode* pRootNode, int iMaxDepth);

	/*해당 노드의 적용 LOD 레벨 반환함수*/
	virtual DWORD	GetLodSubIndex(BNode* pNode);
	/*해당 노드의 적용 LOD 타입 반환함수*/
	virtual DWORD	GetLodType(BNode* pNode);

	/*중점으로부터의 반지름*/
	virtual float	GetExpansionRatio(D3DXVECTOR3 vCenter, float fRadius);
	/*최대 최소값*/
	virtual float	GetExpansionRatio(D3DXVECTOR3 vMax, D3DXVECTOR3 vMin);
	/*카메라의 위치로부터 중점까지의 거리*/
	virtual float	GetExpansionRatio(D3DXVECTOR3 vCenter);

	/*랜더 노드 리스트 생성함수*/
	virtual void	GetDrawPatchNode();
	/*랜더 노드 리스트 생성함수(재귀, GetDrawPatchNode 내부 호출)*/
	virtual void	AddDrawPatchNode(BNode* pNode);
	
#pragma region 인라인 함수
	/*문턱효과 세팅함수*/
	void SetThresHold(bool bFlag) { m_bThresHoldValue = bFlag; };
	/*화면 투영비율 세팅함수*/
	virtual void SetExpansionRatio(int iRatio) { m_fExpansionRatio = iRatio; }
#pragma endregion

public:
	BMapLOD();
	~BMapLOD();
};

