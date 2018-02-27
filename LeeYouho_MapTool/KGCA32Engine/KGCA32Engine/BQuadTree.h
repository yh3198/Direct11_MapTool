#pragma once
#include "BNode.h"
#include "BCamera.h"
#include "BMapLOD.h"

//====================================================================================//
/*맵 생성관련*/

/*Build 함수를 통해 맵을 생성합니다.*/
/*생성과 동시에 쿼드트리 & LOD가 진행되며 맵의 크기가 2의 제곱수가 아니면 생성되지 않습니다.*/
//====================================================================================//

class BQuadTree : public BMapLOD
{
public:
	BNode* m_pRootNode;			// 쿼드트리의 루트(최상위)노드
	float m_fWidth;				// 쿼드트리의 가로범위
	float m_fHeight;			// 쿼드트리의 세로범위
	int m_iMaxDepth;			// 리프노드의 최대 레벨
	int m_iMaxDepthLimit;		// 노드 분할의 최대한도
	int m_iRenderDepth;			// 최대 LOD 랜더 깊이
	float m_fMinDivideSize;		// 최소 분할크기

	vector<BObject*> m_DrawObjList;			// 카메라 프로스텀 내 오브젝트 리스트 ( 박스 전용 )
	vector<MapObjInfo> m_DrawMapObjList;	// 카메라 프로스텀 내 오브젝트 리스트 ( 맵 오브젝트 전용 )
	int m_iTotalMapObj;						// 맵 전체 맵오브젝트 수
	queue<BNode*> m_QuadTreeQueue;			// 노드 검색용 큐
	bool m_bUsedIndexList;					// 정점 인덱스를 이용한 공간분할 허용 변수

public:
	/*기본 구성 함수*/
	virtual bool Frame(BCamera* pCamera);
	virtual bool Release();

	/*맵 생성 및 트리 분할생성 함수*/
	bool Build(float fWidth, float fHeight);
	/*트리 분할 함수*/
	virtual bool BuildTree(BNode* pNode);
	/*노드 생성 함수*/
	BNode* CreateNode(BNode* pParentNode, float fLeft, float fRight, float fBottom, float fTop); // 노드 생성함수
	/*노드 분할 판별 함수*/
	virtual bool SubDivide(BNode* pNode);
	/*분할 가능 사이즈 체크 함수*/
	DWORD CheckSize(DWORD dwSize);

	/*노드 검색용 함수*/
	BNode* FindNode(BNode* pNode, BObject* pObj);
	/*프로스텀 & 노드 충돌판별 함수*/
	virtual void DrawFindNode(BNode* pNode, BCamera* pCamera);
	/*랜더 할 노드 분별 함수*/
	void VisibleNode(BNode* pNode, BCamera* pCamera);
	/*이웃 노드 세팅 함수*/
	void SetNeighborNode(BNode* pNode);
	/*이웃 노드 검색 함수*/
	void FindNeighborNode(BNode* pNode);

	/*쿼드트리 내부 오브젝트 추가 함수*/
	int AddObject(BObject* pObj);
	/*쿼드트리 내 맵 오브젝트 유무 판별용 함수*/
	int CheckRect(BNode* pNode, BObject* pObj);
	/*랜더 할 오브젝트 분별함수*/
	void VisibleObject(BNode* pNode, BCamera* pCamera);
	/*맵 오브젝트 추가 함수*/
	void AddMapObject(BNode* pNode, vector<InstancingInfo> MapInfo);
	/*맵 오브젝트의 위치 판별용 함수*/
	int CheckPosition(BNode* pNode, D3DXVECTOR3 pObj);
	/*전체 노드 내 맵 오브젝트 삭제 함수*/
	void ClearObject(BNode* pNode);


	
public:
	BQuadTree();
	virtual ~BQuadTree();
};

