#pragma once
#include "BQuadTree.h"
#include "BNMap.h"

//====================================================================================//
/*맵 생성관련*/

/*Build 함수를 통해 맵을 생성합니다.*/
/*생성과 동시에 쿼드트리 & LOD가 진행되며 맵의 크기가 2의 제곱수가 아니면 생성되지 않습니다.*/
/*스프레팅 텍스쳐의 경우 정밀성을 위해 10배 더 크게 적용되고 있습니다.*/
/*가로 세로 1000 이상의 맵 제작시 스프레팅 텍스쳐가 생성이 불가능 하므로 만약 이 경우 10배를 제거하셔야 합니다.*/
/*OMSetState 함수의 경우 디바이스에서 DX내의 DSV, DSS, BS를 전부 선언해 두었기 때문에 RTV만 넘겨주면 됩니다.*/


/*오브젝트 스프레팅 관련*/ 

//1. 레드 - 적 몬스터의 리스폰 가능여부
//2. 그린 - 이동불가지역
//3. 블루 - 아이템 리스폰 가능여부
//====================================================================================//

#pragma region LOD 정보 구조체
typedef vector<DWORD> DWORD_VECTOR;
class BStaticData
{
public:
	DWORD dwLevel;
	vector<DWORD_VECTOR> IndexList;
	vector<ComPtr<ID3D11Buffer>> IndexBufferList;
public:
	BStaticData();
	~BStaticData();
};
#pragma endregion

class BMapTileRender : public BQuadTree
{
public:

	BNMap* m_pMap;								// 원본 맵
	int m_iMinSplitDistance;					// 최소분할크기
	DWORD m_dwWidth;							// 맵 가로크기 (정방형 구성 세로와 동일)
	DWORD m_dwHeight;							// 맵 세로크기 (정방형 구성 가로와 동일)
	ID3D11DeviceContext* m_pImmediateContext;	// 메인 콘텍스트
	vector<BStaticData*> m_pdwLodIndexArray;	// 적용된 LOD 인덱스 배열

	//============================ 통합 랜더링 전용 변수============================//
	PNCT_Vertex* m_pVertexList;					// 버텍스 리스트 (통합 랜더링 전용)
	vector<DWORD> m_IndexList;					// 인덱스 리스트 (통합 랜더링 전용)
	int m_iNumFace;								// 페이스 개수 (통합 랜더링 전용)
	bool m_bSharedVertexBuffer;					// 공유정점 버퍼사용 (통합 랜더링 전용)

	ComPtr<ID3D11Buffer> m_pIndexBuffer;		// 인덱스 버퍼 (통합 랜더링 전용)
	ComPtr<ID3D11Buffer> m_pTileIndexBuffer;	// 공유정점 인덱스 버퍼 (통합 랜더링 전용)

	size_t m_ibOffset;							// 인덱스 버퍼 오프셋값 (전체 인덱스의 크기를 합산)
	size_t m_dstibOffset;						// 공유정점 인덱스버퍼 오프셋값 (개별 인덱스 크기)
	//==============================================================================//

	//============================ 맵 스프레팅 전용 변수 ============================//
	map<int, TCHAR*> m_pSplattingTexName;	// 맵 텍스쳐 이름
	D3D11_TEXTURE2D_DESC desc;				// 스프레팅 텍스쳐 정보 (맵 텍스쳐)
	ID3D11Texture2D* StagingTexTure;		// 수정용 텍스쳐 (맵 텍스쳐)
	ID3D11Texture2D* SpreatingTexTure;		// 랜더용 텍스쳐 (맵 텍스쳐)

	D3D11_TEXTURE2D_DESC Objdesc;			// 스프레팅 텍스쳐 정보 (맵 설정정보)
	ID3D11Texture2D* StagingObjPosTexture;  // 수정용 텍스쳐 (맵 설정정보)
	ID3D11Texture2D* ObjPosTexture;			// 랜더용 텍스쳐 (맵 설정정보)

	ComPtr<ID3D11ShaderResourceView> SpreatingSRV;				// 랜더용 쉐이더 리소스 뷰 ( 맵 텍스쳐 )
	ComPtr<ID3D11ShaderResourceView> SpreatingObjSRV;			// 랜더용 쉐이더 리소스 뷰 ( 맵 설정정보 )
	map<int, ComPtr<ID3D11ShaderResourceView>> m_pSplattingSRV; // 적용중인 리소스 뷰
	//===============================================================================//

public:
	/*기본 구성 함수*/
	virtual bool Render(ID3D11DeviceContext* pContext);
	virtual bool PostRender(ID3D11DeviceContext*    pContext);
	virtual bool PreFrame();
	virtual bool Frame(BCamera* pCamera);

	/*맵 생성 및 트리 분할생성 함수*/
	bool Build(BMap* pMap, DWORD dwWidth, DWORD dwHeight, ID3D11DeviceContext* pImmediateContext);
	/*트리 노드 생성함수 (Build 내부 호출)*/
	BNode* CreateNode(BNode* pParentNode, DWORD dwTL, DWORD dwTR, DWORD dwBL, DWORD dwBR);
	/*재정의 # 트리 분할 함수 (쿼드트리 생성시 호출)*/
	bool SubDivide(BNode* pNode);
	/*노드별 바운딩박스 생성함수 (CreateNode 내부호출)*/
	virtual void ComputeBoundingBox(BNode* pNode);
	/*바운딩박스 높이값 반환함수 (ComputeBoundingBox 내부호출)*/
	D3DXVECTOR2 GetHeightFromNode(DWORD dwTL, DWORD dwTR, DWORD dwBL, DWORD dwBR);
	/*버텍스 리스트 생성 함수(Build - SubDevide 내부 호출)*/
	bool CreateVertexList(BNode* pNode);
	/*버텍스 버퍼 생성 함수(Build - SubDevide 내부 호출)*/
	ID3D11Buffer* CreateVertexBuffer(ID3D11Device* pd3dDevice, void *indices, UINT iNumIndex, UINT iSize);
	/*인덱스 리스트 생성 함수(Build 내부 호출)*/
	int	 CreateIndexList(BNode* pNode);
	/*인덱스 버퍼 생성 함수(Build 내부 호출)*/
	ID3D11Buffer* CreateIndexBuffer(ID3D11Device* pd3dDevice, void *indices, UINT iNumIndex, UINT iSize);


	/*적용된 LOD 값 제한 함수*/
	void SetRenderDepth(DWORD dwDepth);
	/*LOD 인덱스 계산함수*/
	bool ComputeStaticLodIndex(int iMaxCells);
	/*LOD 인덱스 버퍼 생성함수 */
	HRESULT CreateLODIndexBuffer(BStaticData* pData);
	/*LOD 인덱스 반환함수*/
	DWORD GetIndexCounter(DWORD dwData, DWORD dwNumCell);

	
	/*스프레팅용 텍스쳐 생성함수*/
	HRESULT CreateSpreatingTexture();
	/*텍스쳐 변경 함수*/
	void Spreating(ID3D11DeviceContext* pImmediateContext, B_BOX Area, D3DXVECTOR3 CrashPoint, float fRadius = 1.0f, int iColor = 0); // 스프레팅
	/*텍스쳐 저장 함수*/
	void SaveSpreating(ID3D11DeviceContext* pImmediateContext);
	/*그라디언트 방식 텍스쳐 변경 함수*/
	void GradientSpreating(ID3D11DeviceContext* pImmediateContext, B_BOX Area, D3DXVECTOR3 CrashPoint, float fRadius, int iColor);
	/*텍스쳐 로드 함수*/
	HRESULT SplattingTextureLoad(ID3D11Device* pDevice, const TCHAR* strFilePath, int iColor);
	/*오브젝트 정보저장 텍스쳐 변경 함수*/
	void ObjSpreating(ID3D11DeviceContext* pImmediateContext, B_BOX Area, D3DXVECTOR3 CrashPoint, float fRadius, int iColor);
	/*랜더용 텍스쳐 적용 함수*/
	HRESULT CreateSRV(ID3D11Texture2D* SpreatingTex, ID3D11Texture2D* StagingTex, D3D11_TEXTURE2D_DESC SpreatingDescV);
	/*알파 텍스쳐 전용 랜더*/
	bool AlphaRender(ID3D11DeviceContext* pContext);


	/*바운딩 박스 재연산 함수(재귀)*/
	void UpdateBoundingBox(BNode* pNode);

	/*정적 인덱스 리스트 갱신함수*/
	int UpdateStaticIndexList(BNode* pNode, DWORD dwCurrentIndex, DWORD dwLod);

	/*노드 인덱스 반환함수*/
	int GetIndex(DWORD dwIndex, DWORD dwTL);

	/*랜더세팅 전체변경 함수*/
	bool OMSetState(ID3D11DeviceContext* pContext, ID3D11RenderTargetView* m_pRTV, ID3D11DepthStencilView* m_pDSV, ID3D11DepthStencilState* m_pDSS, ID3D11BlendState* m_pBS);

public:
	BMapTileRender();
	~BMapTileRender();
};

