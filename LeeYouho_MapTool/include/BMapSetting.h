#pragma once
#include "BStd.h"
#include "BMapTileRender.h"
#include "BSelect.h"
#include "BCamera.h"
#include "BInput.h"

//====================================================================================//
/*Init을 통해 카메라와 콘텍스트를 세팅하고 SettingMap 함수를 통해 맵을 설정합니다.*/
/*MapInfoSetting / SettingSoundInfo / SettingObjInfo 함수를 통해 필요한 정보값을 세팅합니다.*/
//====================================================================================//

class BMapSetting
{
public:
	/*MFC 설정변경용 bool*/
	bool BMapSettingMode;			// 맵세팅 모드
	bool BCreatePlane;				// 맵플랜 생성모드
	bool BLastCheck;				// 드래그 여부 체크
	bool BApply;					// 맵세팅 적용
	
	BCamera* m_pMainCamera;						// 메인 카메라
	ID3D11DeviceContext* m_pImmediateContext;	// 메인 콘텍스트
	
	BSelect m_InfoSelect;						// 맵 선택 변수
	vector<vector<int>> m_MapAddress;			// 전체 맵 주소
	vector<TileInfo> m_TileInfo;				// 맵의 정보값
	BMapTileRender* m_SelectedMap;				// 선택을 적용중인 맵

public:
	/*기본 구성 함수*/
	void Init(BCamera* MainCamera, ID3D11Device* m_pd3dDevice, ID3D11DeviceContext* pContext);
	void Frame();
	void Render();
	void Release();

	/*세부정보 세팅 맵 설정함수*/
	void SettingMap(BMapTileRender* vMapQuadTree);

	/*맵 정보 세팅함수*/
	void MapInfoSetting();
	/*높이 정보 세팅함수(SettingPositionInfo 내부 호출)*/
	void SettingPositionInfo(BNode* pNode, int iTotalRow);
	/*맵 세부설정 세팅함수 (소리, 알파텍스쳐 이용)*/
	void SettingSoundInfo();
	/*맵 세부정보 세팅함수 (오브젝트)*/
	void SettingObjInfo();
	
public:
	BMapSetting();
	~BMapSetting();
};

