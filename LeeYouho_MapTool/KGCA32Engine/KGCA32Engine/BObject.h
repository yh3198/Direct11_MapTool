#pragma once
#include "BWindowLib.h"
#include "BObjStd.h"

//====================================================================================//
/*���� ������Ʈ ����� �⺻ Ŭ�����Դϴ�.*/
/*Create �Լ��� �̿��Ͽ� ������Ʈ ������ �����մϴ�.*/
//====================================================================================//

class BObject
{
public:
	vector<PNCT_Vertex> m_VertexList;			 // ���ؽ� ����
	vector<DWORD> m_IndexList;					 // ���ؽ� �ε���
	UINT m_VertexCnt;							 // ���ؽ� ����
	VSConstantB m_cbData;						 // ������� �ɹ�
	B_BOX m_tBox;								 // �ִ� �ּҰ�

	ID3D11Buffer*			m_pVertexBuffer;	 // ���ؽ� ����
	ID3D11Buffer*			m_pIndexBuffer;		 // �ε��� ����
	ID3D11Buffer*			m_pConstantBuffer;   // ��� ����
	ID3D11InputLayout*		m_pVertexLayout;	 // ���ؽ� ���̾ƿ�
	
	vector<D3DXVECTOR3>		m_TangentList;		 // �������� ����Ʈ
	ComPtr<ID3D11Buffer>	m_pTangentBuffer;	 // �������� ����
	UINT m_TangentCnt;

	vector<ComPtr<ID3D11ShaderResourceView>> m_pTextureSRV;			 // �ؽ��� ���ҽ�
	vector<ComPtr<ID3D11ShaderResourceView>> m_pNormalTextureSRV;	 // �븻 �ؽ��� ���ҽ�
	INT m_iCurIndex;												 // �ؽ��� �ε���

	ID3D11VertexShader*		m_pVS;				// ���ؽ� ���̴�
	ID3D11PixelShader*		m_pPS;				// �ȼ� ���̴�
	ID3D11GeometryShader*	m_pGS;				// ���� ���̴�

	ID3D11Device* m_pd3dDevice;					// ����̽� �������̽�
	ID3D11DeviceContext* m_pContext;			// ���� ���ؽ�Ʈ

	D3DXMATRIX m_matWorld;  // ������ǥ (���Ͱ�) * ���� ��� = ������ǥ
	D3DXMATRIX m_matView;	// ������ǥ * ����� = ����ǥ (ī�޶��� �̵��� ���� ��ǥ)
	D3DXMATRIX m_matProj;	// ����ǥ * ������� = ������ǥ (ī�޶��� ������ ���� ��ǥ)

	D3DXMATRIX m_matTrans;  // �̵����
	D3DXMATRIX m_matRotate; // ȸ�����
	D3DXMATRIX m_matScale;  // ũ�����


	/*������Ʈ�� ��ġ ����*/
	D3DXVECTOR3 m_vPos;			// ��ġ����
	D3DXVECTOR3 m_vTarget;		// ���⺤��
	D3DXVECTOR3 m_vUp;			// ���⺤��

	/*��ġ�� ��Ÿ���� ���⺤��*/
	D3DXVECTOR3 m_vLook;		// ���⺤��
	D3DXVECTOR3 m_vSide;		// ���̵庤��

	/*������Ʈ ȸ�� ����*/
	float m_fPitch;				//x�� ����ȸ��
	float m_fYaw;				//y�� ����ȸ��
	float m_fRoll;				//z�� ����ȸ��
	float m_SpeedUp;			//�ӵ���

	// �������� ����
	UINT m_iPrimitiveType;		// ������ ������
	int m_iNumFace;				// �� �ﰢ���� ����
	int m_iNumVertices;			// �� ���ؽ��� ����
	vector<int> m_Normallookuptable;	// �븻 ������̺� �ε���
	vector<D3DXVECTOR3> m_FaceNormals;  // �����븻
	
public:
	/*�⺻ ���� �Լ�*/
	virtual bool Init();
	virtual bool Frame();
	virtual bool PostFrame();
	virtual bool PreRender(ID3D11DeviceContext* pContext);
	virtual bool Render(ID3D11DeviceContext* pContext);
	virtual bool PostRender(ID3D11DeviceContext* pContext);
	virtual bool Release();

	/*������Ʈ ���� �Լ� (���� �ؽ���)*/
	virtual HRESULT Create(ID3D11Device* pDevice, const TCHAR* pLoadShaderFile, TCHAR** pLoadTextureString = NULL, int TextureCnt= 1);
	/*������Ʈ ���� �Լ� (���� �ؽ���)*/
	HRESULT Create(ID3D11Device* pDevice, const TCHAR* pLoadShaderFile, const TCHAR* pLoadTextureString);
	/*���̾ƿ�, ���̴� �����Լ�(Create ���� ����)*/
	virtual HRESULT LoadShaderAndInputLayout(const TCHAR* pShaderFile);
	/*���ؽ� ���� �����Լ�*/
	virtual HRESULT CreateVertexBuffer();
	/*�ε��� ���� �����Լ�*/
	virtual HRESULT CreateIndexBuffer();
	/*��� ���� �����Լ�*/
	virtual HRESULT CreateConstantBuffer();
	/*�ؽ��� �ε��Լ� (���� �ؽ���)*/
	virtual HRESULT TextureLoad(ID3D11Device* pDevice, const TCHAR* strFilePath);
	/*�ؽ��� �ε��Լ� (���� �ؽ���)*/
	virtual HRESULT TextureLoad(ID3D11Device* pDevice, TCHAR* strFilePathes[], int strCount);
	/*min max �ڽ� �����Լ�*/
	void UpdateMinMaxBox();
	
	/*��� �����Լ�*/
	virtual void SetMatrix(D3DXMATRIX World, D3DXMATRIX View, D3DXMATRIX Proj);
	/*������Ʈ �̵��Լ� (Init ����ȣ��)*/
	virtual bool UpdateVector();
	/*������Ʈ ȸ���Լ� (PostFrame ����ȣ��)*/
	virtual bool UpdateQuaternion();
	/*���°� �����Լ�*/
	bool OMSetState(ID3D11DeviceContext* pContext, ID3D11RenderTargetView* m_pRTV, ID3D11DepthStencilView* m_pDSV, ID3D11DepthStencilState* m_pDSS, ID3D11BlendState* m_pBS);

	/*�������� �����Լ�*/
	virtual bool UpdateNormal();
	/*���̽� �븻 �����Լ� (UpdateNormal ���� ȣ��)*/
	virtual bool InitFaceNormal();
	/*������̺�(�ε���) �����Լ� (UpdateNormal ���� ȣ��)*/
	virtual void GenNormalLookupTable();
	/*���̽��븻 ��� ���ؽ� �븻���� �����Լ� (UpdateNormal ���� ȣ��)*/
	virtual void CalcPerVertexNormalsFastLookup();
	/*�������� �����Լ� (UpdateNormal ���� ȣ��)*/
	virtual bool UpDateTangentBuffer();
	/*�������� �����Լ�(UpDateTangentBuffer ���� ȣ��)*/
	void UpDateTangentSpaceVectors(D3DXVECTOR3 *v0, D3DXVECTOR3 *v1, D3DXVECTOR3 *v2, D3DXVECTOR2 uv0, D3DXVECTOR2 uv1, D3DXVECTOR2 uv2, D3DXVECTOR3 *vTangent, D3DXVECTOR3 *vBiNormal, D3DXVECTOR3 *vNormal);
	
#pragma region �����ǿ� �����Լ�
	/*���� �����Լ� (�����ǿ� �����Լ�)*/
	virtual bool UpdateBuffer() { return true; };
	/*min max �ڽ� �����Լ� (�����ǿ� �����Լ�)*/
	virtual void UpdateVertexMinMax() { return; };
#pragma endregion

public:
	BObject();
	virtual ~BObject();
};

