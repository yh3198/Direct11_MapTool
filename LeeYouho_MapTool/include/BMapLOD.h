#pragma once
#include "BNode.h"
typedef vector<BNode*> BNODE_VECTOR;

//====================================================================================//
/*Update, SetLOD�� ���� �⺻ LOD���� ���� �� InitLevelOrder�� ���� ������ �����մϴ�.*/
/*GetExpansionRatio�� ���ڰ��� ���� LOD ���� ��� ��ȭ�� �����մϴ�.*/
/*�������� �������κ����� �Ÿ�, �ִ� �ּҰ�, ī�޶�� ������ �Ÿ��� ���� ������ �����˴ϴ�. */
//====================================================================================//


class BMapLOD
{
public:
	ID3D11Device* m_pd3dDevice;				// ���� 3D ����̽�
	BCamera* m_pCamera;						// ���� ī�޶�
	vector<BNODE_VECTOR> m_LevelList;		// ��� ��������Ʈ
	vector<BNode*> m_DrawPatchNodeList;		// ���� ��� ����Ʈ (��ü ��� ����)
	vector<BNode*> m_DrawNodeList;			// ���� ��� ����Ʈ (���� ��� ����)
	float m_fExpansionRatio;				// LOD ���� (ȭ�� ���� ����( 0 ~ 1 ���� ��� ))
	int m_iNumCell;							// Ʈ���� ������ ���̿� ���� �� ����
	int m_iPatchLodCount;					// LOD ���� ���� ( z = pow (x,y) ���� y = log(z) / log(x) )
	bool m_bThresHoldValue;					// ����ȿ�� ��뿩��
	
public:
	/*���� ī�޶�, ����̽� �����Լ�*/
	virtual void	Update(ID3D11Device* pd3dDevice, BCamera* pCamera = NULL);
	/*LOD ���� �����Լ�*/
	virtual void	SetLOD(DWORD dwWidth, int iNumDepth);
	/*��� ��������Ʈ �����Լ�*/
	virtual void	InitLevelOrder(BNode* pRootNode, int iMaxDepth);

	/*�ش� ����� ���� LOD ���� ��ȯ�Լ�*/
	virtual DWORD	GetLodSubIndex(BNode* pNode);
	/*�ش� ����� ���� LOD Ÿ�� ��ȯ�Լ�*/
	virtual DWORD	GetLodType(BNode* pNode);

	/*�������κ����� ������*/
	virtual float	GetExpansionRatio(D3DXVECTOR3 vCenter, float fRadius);
	/*�ִ� �ּҰ�*/
	virtual float	GetExpansionRatio(D3DXVECTOR3 vMax, D3DXVECTOR3 vMin);
	/*ī�޶��� ��ġ�κ��� ���������� �Ÿ�*/
	virtual float	GetExpansionRatio(D3DXVECTOR3 vCenter);

	/*���� ��� ����Ʈ �����Լ�*/
	virtual void	GetDrawPatchNode();
	/*���� ��� ����Ʈ �����Լ�(���, GetDrawPatchNode ���� ȣ��)*/
	virtual void	AddDrawPatchNode(BNode* pNode);
	
#pragma region �ζ��� �Լ�
	/*����ȿ�� �����Լ�*/
	void SetThresHold(bool bFlag) { m_bThresHoldValue = bFlag; };
	/*ȭ�� �������� �����Լ�*/
	virtual void SetExpansionRatio(int iRatio) { m_fExpansionRatio = iRatio; }
#pragma endregion

public:
	BMapLOD();
	~BMapLOD();
};

