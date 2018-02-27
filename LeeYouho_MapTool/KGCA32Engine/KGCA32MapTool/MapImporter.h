#pragma once
#include "BCore.h"
#include "BAseObj.h"

//====================================================================================//
// 툴에서 제작된 내용을 불러오기 위한 클래스입니다.
// 호출 순서는 다음과 같습니다.
// 1. OpenStream
// 2. LoadMapObj
// 3. LoadMap
//====================================================================================//

class MapImporter
{
public:
	ID3D11DeviceContext* m_pImmediateContext;	// 메인 콘텍스트
	ID3D11Device* m_pd3dDevice;					// 메인 디바이스
	BCamera* m_MainCamera;						// 메인 카메라
	ID3D11Texture2D* StagingTexTure;			// 값 변환용 텍스쳐

	FILE* m_pStream;					// 로드할 파일 포인터
	fpos_t m_FilePosition;				// 파일 포인터의 위치
	TCHAR m_pBuffer[256];				// 파일 문자열 버퍼 (전체)
	TCHAR m_pString[256];				// 파일 문자열 버퍼 (부분)
	TCHAR MapTexture[256];				// 파일 문자열 버퍼 (맵 기본텍스쳐)
	TCHAR m_pSplatting[4][256];			// 파일 문자열 버퍼 (맵 알파이용 텍스쳐명)
	TCHAR m_WaterMapTex[256];			// 파일 문자열 버퍼 (워터맵 텍스쳐명)
	TCHAR m_WaterNormalMapTex[256];		// 파일 문자열 버퍼 (워터 노말맵 텍스쳐명)

	int iCellCount;				// 맵 셀값
	int iPatchCount;			// 맵 패치값
	float fCellDistance;		// 맵 셀 거리값
	
	BAseObj* m_MapObj;			// 맵 오브젝트

public:
	/*로드 파일 설정 및 메인 값 세팅함수*/
	bool OpenStream(const TCHAR* pFileName, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext, BCamera* MainCamera);
	
	/*파일 전체로드 함수*/
	bool LoadMap(vector<BMap*>* m_vMFCMap, vector<BMapDesc*>* m_vMFCMapInfo, vector<BMapTileRender*>* m_vMapQuadTree, vector<TCHAR*>* m_vMapTexture, map<int, TCHAR*>* m_mMapSplattingTexture, vector<BWaterMap*>* m_vWaterQuadTree);
	/*알파 텍스쳐 생성 함수 (LoadMap 내부 호출)*/
	ID3D11Texture2D* LoadAlphaTexture(TCHAR* texture, BMap* Map);
	/*오브젝트 정보 텍스쳐 생성 함수 (LoadMap 내부 호출)*/
	ID3D11Texture2D* LoadSpawnTexture(TCHAR* texture, BMap* Map);
	/*높이값 세팅 함수 (LoadMap 내부 호출)*/
	void GetHeightFromTex(BNode* Node);

	/*오브젝트 로드 함수*/
	int LoadMapObj(vector<BAseObj*>* ObjMgr);

	//=========================================//
					/*바이너리식*/
	//=========================================//


public:
	MapImporter();
	~MapImporter();
};

