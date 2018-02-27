#pragma once
#include "BCamera.h"

//====================================================================================//
/*���ҿ� ��� Ŭ�����Դϴ�.*/
//====================================================================================//

class BNode
{
public:
	DWORD m_dwDepth;				// ����� ����
	DWORD m_isLeaf;					// ����� ���� = ������� ����
	D3DXVECTOR3 m_vCorner[4];		// ����� �ڳʰ�
	BNode* m_pChild[4];				// ����� �������
	BNode* m_pParent;				// ����� �������
	vector <DWORD> m_CornerIndex;	// �ڳ� ���ؽ��� �ε��� ��
	B_BOX m_tBox;					// ����� �ٿ���ڽ�
	DWORD m_dwLodLevel;				// ����� LOD ����
	DWORD m_dwLodType;				// ����� LOD Ÿ��

	vector<BObject*> m_ObjectList;	  // ��忡 ���Ե� ������Ʈ ���� (�ڽ� ����)
	vector<MapObjInfo> m_vMapObjInfo; // ��忡 ���Ե� ������Ʈ ����

	DWORD m_dwPositionIndex[2];			// ���̸� ����Ʈ���� �ε��� X, Y
	vector<BNode*>		m_NeighborList; // �̿���� ����

#pragma region �浹 Ȯ�ο� �ڽ����� (�̻��)
	/* �浹 Ȯ�ο� �ڽ� (�̻��) */
	D3D11_BOX			m_BoxVB;		
	D3D11_BOX			m_BoxIB;
	UINT				m_iBeginVB;
	UINT				m_iBeginIB;
	vector<DWORD>		m_IndexList;
	vector<PNCT_Vertex>	m_VertexList;
	vector<D3DXVECTOR3> m_TangentList;
#pragma endregion 

	ComPtr<ID3D11Buffer>	m_pVertexBuffer;	// ��� ���ؽ� ����
	ComPtr<ID3D11Buffer>	m_pIndexBuffer;		// ��� �ε��� ����
	ComPtr<ID3D11Buffer>	m_pTangentBuffer;	// ��� ź��Ʈ ����


public:
	BNode();
	~BNode();
};

