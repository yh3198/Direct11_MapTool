#pragma once
#include "BCamera.h"

//====================================================================================//
/*분할용 노드 클래스입니다.*/
//====================================================================================//

class BNode
{
public:
	DWORD m_dwDepth;				// 노드의 깊이
	DWORD m_isLeaf;					// 노드의 상태 = 리프노드 여부
	D3DXVECTOR3 m_vCorner[4];		// 노드의 코너값
	BNode* m_pChild[4];				// 노드의 하위노드
	BNode* m_pParent;				// 노드의 상위노드
	vector <DWORD> m_CornerIndex;	// 코너 버텍스의 인덱스 값
	B_BOX m_tBox;					// 노드의 바운딩박스
	DWORD m_dwLodLevel;				// 노드의 LOD 레벨
	DWORD m_dwLodType;				// 노드의 LOD 타입

	vector<BObject*> m_ObjectList;	  // 노드에 포함된 오브젝트 정보 (박스 전용)
	vector<MapObjInfo> m_vMapObjInfo; // 노드에 포함된 오브젝트 정보

	DWORD m_dwPositionIndex[2];			// 높이맵 쿼드트리용 인덱스 X, Y
	vector<BNode*>		m_NeighborList; // 이웃노드 정보

#pragma region 충돌 확인용 박스변수 (미사용)
	/* 충돌 확인용 박스 (미사용) */
	D3D11_BOX			m_BoxVB;		
	D3D11_BOX			m_BoxIB;
	UINT				m_iBeginVB;
	UINT				m_iBeginIB;
	vector<DWORD>		m_IndexList;
	vector<PNCT_Vertex>	m_VertexList;
	vector<D3DXVECTOR3> m_TangentList;
#pragma endregion 

	ComPtr<ID3D11Buffer>	m_pVertexBuffer;	// 노드 버텍스 버퍼
	ComPtr<ID3D11Buffer>	m_pIndexBuffer;		// 노드 인덱스 버퍼
	ComPtr<ID3D11Buffer>	m_pTangentBuffer;	// 노드 탄젠트 버퍼


public:
	BNode();
	~BNode();
};

