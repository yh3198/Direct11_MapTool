#pragma once
#include "BCore.h"
#include "MapExporter.h"
#include "MapImporter.h"
#include "BMapSetting.h"
#include "BParser.h"
#include "BAseObj.h"

#pragma region MFC 모드 관련 변수 구조체
enum VertexControlerState
{
	vNone = 0,
	vUp,
	vDown,
	vParallel,
};

enum SpreatingState
{
	vSpreatNone = 0,
	vSpreatApply,
};

enum SpreatingColor
{
	vRed = 0,
	vGreen,
	vBlue,
	vAlpha,
};

struct SelectedMapObj
{
	int m_iObjType; // 오브젝트 종류
	int m_iObjNum;  // 해당 오브젝트의 vecter순서
	float m_fRange; // 레이의 원점으로 부터의 거리
};
#pragma endregion

class Sample : public BCore
{
public:
#pragma region 기본 맵툴 관련 변수
	BMapDesc*			m_MFCMapInfo;			// 맵 정보
	BMap*				m_MFCMap;				// 맵
	BMapTileRender*		m_MapQuadTree;			// 분할 맵
	
	vector<BMapDesc*>		m_vMFCMapInfo;		// 맵 정보 벡터
	vector<BMap*>			m_vMFCMap;			// 맵 벡터
	vector<BMapTileRender*> m_vMapQuadTree;		// 분할 맵 벡터

	MapExporter m_Exporter;						// 맵 출력 클래스
	MapImporter m_Importer;						// 맵 입력 클래스

	BSelect m_Select;							// 마우스 충돌 선택 클래스
	vector<int>			m_viClosedIndex;		// 다중 충돌시 확인용 인덱스 (선택시)

	vector<TCHAR*>		m_vMapTexture;			// 맵 텍스쳐명
	map<int, TCHAR*>	m_mMapSplattingTexture;	// 맵 스플레팅 텍스쳐용

	int m_VCtrlState;			// 맵 높이조절 상태확인용 변수 
	int m_VSpreatState;			// 맵 그리기 상태확인용 변수
	int m_VObjSpreatState;		// 오브젝트 정보텍스쳐 그리기용 변수

	/*맵 높이 설정용 변수*/
	float	m_fRadius;			// 마우스 피킹범위
	float	m_fHeightValue;		// 선택된 지점의 높이 값
	bool	bParallel;			// 맵 평탄화 모드
	float	fParallel;			// 선택된 지점의 높이 값(평탄화 모드시 활성화)

	/*맵 스프레팅용 변수*/
	B_BOX	SpreatingBox;		// 스프레팅 가능여부 최대 최소범위uv값
	float	m_fSpreatingRadius;	// 스프레팅 범위
	int		m_iSpreatingColor;	// 스프레팅 색상
	bool	m_ModeGradient;		// 그라디언트 스프레팅 여부
	int		ColoringCnt;		// 연속그리기 방지용 변수

	/*다중 맵 사용시 필요 변수(현재 미사용)*/
	BMapTileRender* m_bSelectedMap; // 나중에 맵이 여러개 생성되었을 때 변경해주어야한다. 지금은 기본 생성된 0번맵을 넣어줄 것이다.
	int m_iSelectedMap;				// 나중에 맵이 여러개 생성되었을 때 변경해주어야한다. 지금은 기본 생성된 0번맵을 넣어줄 것이다.
#pragma endregion

#pragma region 워터맵 관련 변수
	/*워터맵 라이트 변수*/
	D3DXVECTOR3	m_vInitLightPos;	// 고정 라이트 위치
	D3DXMATRIX	m_mLightWorld;		// 변환된 라이트 위치의 행렬
	D3DXVECTOR3 m_LightPos;			// 변환된 라이트 위치
	D3DXVECTOR3 m_vDirection;		// 변환된 라이트 방향
	
	/*워터맵 엔진용 변수*/
	BMapDesc* m_MFCWaterMapInfo;			// 워터맵 정보
	BNMap* m_MFCWaterMap;					// 워터맵 사용 맵
	BWaterMap* m_WaterMapQuadTree;			// 워터맵
	vector<BNMap*> m_vMFCWaterMap;			// 워터맵 사용 맵 벡터 
	vector<BWaterMap*> m_vWaterQuadTree;	// 워터맵 벡터
#pragma endregion

#pragma region 스프래팅 관련 변수
	BMapSetting m_MapSetting;				// 맵 세부 관련정보 세팅 클래스
	vector<TCHAR*> Text;					// 스프레팅용 텍스쳐명 (맵 텍스쳐)
	int m_iSpreatingRange;					// 세부내용 설정 범위
	int m_iObjSpreatingColor;				// 세부내용 설정 색상 
											// (현재는 레드 : 몬스터 스폰 / 블루 : 아이템 스폰 / 그린 : 이동불가 지역)
	bool m_MapSettingRenderMode;			// 화면 랜더모드
#pragma endregion

#pragma region 오브젝트 관련 변수
	vector<BAseObj*> m_MapObjMgr;						// 맵 오브젝트 벡터
	int m_MapObjTotalCnt;								// 맵 오브젝트 총 수량
	vector<SelectedMapObj> m_vSelectedObjInfo;			// 선택된 오브젝트 정보값
	vector<SelectedMapObj> m_vBeforeSelectedObjInfo;	// 이전 선택 오브젝트 정보값
	vector<int> m_CreateObjType;						// 오브젝트 생성용 인덱스 벡터
	
	bool ObjSettingMode;		// 오브젝트 생성모드
	bool ObjCreateOne;			// 오브젝트 생성모드 (단일생성용 변수)
	bool ObjSelectingMode;		// 오브젝트 선택모드
	bool ObjMovingMode;			// 오브젝트 이동모드
	bool ObjRenderMode;			// 오브젝트 랜더모드
	bool BoxRenderMode;			// 오브젝트 MinMax박스 랜더모드
	
	SelectedMapObj m_TempInfo;				// 선택 오브젝트 임시저장소
	vector<vector<int>> m_DrawObjList;		// 랜더링 오브젝트 벡터

	BAseObj m_RenderSettingASE;				// ASE 파일의 VS, PS, Layout 참조를 위한 변수
	BShadow m_Shadow;						// 쉐도우 변수
#pragma endregion

#pragma region 쉐도우 관련 변수
	// 맵 디버그용
	bool bMapDebug;							// 워터맵 & 쉐도우 반사, 굴절, 투영 확인모드
#pragma endregion

public:
#pragma region 기본 구성함수
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
#pragma endregion

#pragma region 기본 맵툴 관련 함수
	/*맵 생성 함수*/
	bool CreateMap(int iCellCount, int iPatchCount, float fCellDistance, TCHAR** MapTexture, int TextureCount, TCHAR* HeightMapTex = NULL);
	/*분할 맵 생성 함수*/
	bool CreateQuadTree(int iMaxDepthLimit, float fMinDivideSize, BMap* Map);
	/*맵 높이 컨트롤 함수*/
	bool MapHeightControl();
	/*맵 높이 변경 함수(업, 다운 / MapHeightControl 내부 호출)*/
	D3DXVECTOR2 ChangeYPosition(D3DXVECTOR3 CPos, vector<PNCT_Vertex>* VertexList);
	/*맵 높이 변경 함수(평행 / MapHeightControl 내부 호출)*/
	D3DXVECTOR2 ChangeYParallel(D3DXVECTOR3 CPos, vector<PNCT_Vertex>* VertexList);
	/*맵 높이 반환용 함수*/
	D3DXVECTOR4 GetHeight(D3DXVECTOR3 HeroPos);
	
	/*스프레팅 함수*/
	bool MapSpreating();
	/*스프레팅용 텍스쳐 설정 함수*/
	bool GetSplattingTexName(int Color, TCHAR** Name);
	
	/*맵 저장 함수*/
	bool MapExport();
	/*맵 호출 함수*/
	bool MapImport(TCHAR* ImportFile);
#pragma endregion

#pragma region 워터맵 관련 함수
	/*워터맵 생성 함수*/
	bool CreateWaterMap(BMapTileRender* SelectedMap, int iMaxDepthLimit, float fMinDivideSize, int iSelectedMap, TCHAR* WaterTex, TCHAR* WaterNormalTex, float fHeight, float fWave);
#pragma endregion

#pragma region 맵 세팅 관련 함수
	/*맵 세부정보 텍스쳐화 저장 함수*/
	bool MapObjInfoSpreating();
	/*분할맵 노말 생성함수*/
	void CreateNormal();
	/*분할맵 노말 계산함수 (재귀, CreateNormal 내부 호출)*/
	void CalcNormal(BNode* pNode, int iRatio);
#pragma endregion

#pragma region 맵 오브젝트 관련 함수
	/*오브젝트 선택함수*/
	void SelectMapObj();
	/*오브젝트 삭제함수*/
	void ClearSelected();
	/*오브젝트 이동함수*/
	bool GetMapObjectMovePosition();
	/*오브젝트 생성함수*/
	bool GetMapObjectPosition();
#pragma endregion

#pragma region 쉐도우 관련 함수
	/*쉐도우 랜더 함수*/
	void RenderShadow();
#pragma endregion

public:
	Sample();
	~Sample();
};

