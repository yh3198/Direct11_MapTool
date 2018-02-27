#pragma once
#include "BStd.h"
#include "BMapTileRender.h"
#include "BSelect.h"
#include "BCamera.h"
#include "BInput.h"

//====================================================================================//
/*Init�� ���� ī�޶�� ���ؽ�Ʈ�� �����ϰ� SettingMap �Լ��� ���� ���� �����մϴ�.*/
/*MapInfoSetting / SettingSoundInfo / SettingObjInfo �Լ��� ���� �ʿ��� �������� �����մϴ�.*/
//====================================================================================//

class BMapSetting
{
public:
	/*MFC ��������� bool*/
	bool BMapSettingMode;			// �ʼ��� ���
	bool BCreatePlane;				// ���÷� �������
	bool BLastCheck;				// �巡�� ���� üũ
	bool BApply;					// �ʼ��� ����
	
	BCamera* m_pMainCamera;						// ���� ī�޶�
	ID3D11DeviceContext* m_pImmediateContext;	// ���� ���ؽ�Ʈ
	
	BSelect m_InfoSelect;						// �� ���� ����
	vector<vector<int>> m_MapAddress;			// ��ü �� �ּ�
	vector<TileInfo> m_TileInfo;				// ���� ������
	BMapTileRender* m_SelectedMap;				// ������ �������� ��

public:
	/*�⺻ ���� �Լ�*/
	void Init(BCamera* MainCamera, ID3D11Device* m_pd3dDevice, ID3D11DeviceContext* pContext);
	void Frame();
	void Render();
	void Release();

	/*�������� ���� �� �����Լ�*/
	void SettingMap(BMapTileRender* vMapQuadTree);

	/*�� ���� �����Լ�*/
	void MapInfoSetting();
	/*���� ���� �����Լ�(SettingPositionInfo ���� ȣ��)*/
	void SettingPositionInfo(BNode* pNode, int iTotalRow);
	/*�� ���μ��� �����Լ� (�Ҹ�, �����ؽ��� �̿�)*/
	void SettingSoundInfo();
	/*�� �������� �����Լ� (������Ʈ)*/
	void SettingObjInfo();
	
public:
	BMapSetting();
	~BMapSetting();
};

