#include "BMap.h"

//====================================================================================//
/*SetMapDesc함수를 통해 생성할 맵의 세부 수치 설정 후 Create 함수를 통해 생성해야 합니다.*/
//====================================================================================//

#pragma region 기본 구성 함수
bool BMap::Frame()
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matRotate);
	D3DXMatrixIdentity(&m_matTrans);
	D3DXMatrixIdentity(&m_matScale);
	return true;
}
bool BMap::Release()
{
	BObject::Release();
	m_VertexArray.clear();
	m_fHeightList.clear();
	return true;
}
#pragma endregion

#pragma region 맵 생성 관련 함수
/*맵 생성함수*/
HRESULT BMap::Create(ID3D11Device* pDevice, ID3D11DeviceContext* m_pImmediateContext, TCHAR** pLoadTextureString, int TextureCnt, TCHAR* HeightMapString, const TCHAR* ShaderString)
{
	m_pd3dDevice = pDevice;
	m_pContext = m_pImmediateContext;

	HRESULT hr = S_OK;

	if (HeightMapString != NULL)
	{
		CreateHeightMap(HeightMapString);
	}
	S_RETURN(CreateVertexBuffer());
	S_RETURN(CreateIndexBuffer());
	S_RETURN(LoadShaderAndInputLayout(ShaderString));
	S_RETURN(CreateConstantBuffer());
	if (pLoadTextureString != NULL)
	{
		if (TextureCnt < 2)
		{
			m_MapTexture = pLoadTextureString[0];
			S_RETURN(TextureLoad(pDevice, pLoadTextureString[0]));
		}
		else
		{
			S_RETURN(TextureLoad(pDevice, pLoadTextureString, TextureCnt));
		}
	}
	UpdateMinMaxBox();
	return hr;
}
/*높이맵 생성 함수*/
bool BMap::CreateHeightMap(TCHAR* strHeightMapText)
{
	HRESULT hr;
	D3DX11_IMAGE_INFO ImageInfo; // 실제 읽어드린 정보
	D3DX11_IMAGE_LOAD_INFO loadInfo;
	ZeroMemory(&loadInfo, sizeof(D3DX11_IMAGE_LOAD_INFO));
	loadInfo.Usage = D3D11_USAGE_STAGING; // CPU인식허가
	loadInfo.CpuAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ; // 텍스쳐 정보를 읽어오기 위한 플래그
	loadInfo.Format = DXGI_FORMAT_FROM_FILE; // 읽어온 파일 포맷을 그대로 사용
	loadInfo.pSrcInfo = &ImageInfo;

	ID3D11Resource *pTexture = NULL;
	if (FAILED(hr = D3DX11CreateTextureFromFile(m_pd3dDevice, strHeightMapText, &loadInfo, NULL, &pTexture, NULL)))
	{
		return false;
	}

	ID3D11Texture2D *pTexture2D = NULL;
	if (FAILED(pTexture->QueryInterface(__uuidof(ID3D11Texture2D), (LPVOID*)&pTexture2D)))
	{
		return false;
	} // 리소스로부터 텍스쳐를 얻어낸다.
	S_RELEASE(pTexture);

	D3D11_TEXTURE2D_DESC desc;
	pTexture2D->GetDesc(&desc);
	m_fHeightList.resize(desc.Height*desc.Width);

	if (pTexture2D)
	{
		D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
		if (SUCCEEDED(m_pContext->Map((ID3D11Resource*)pTexture2D, D3D11CalcSubresource(0, 0, 1)/*최고 해상도의 텍스쳐*/, D3D11_MAP_READ, 0, &MappedFaceDest)))
		{
			UCHAR* pTexels = (UCHAR*)MappedFaceDest.pData;
			PNCT_Vertex Vertex;
			for (UINT row = 0; row < desc.Height; row++)
			{
				UINT RowStart = row * MappedFaceDest.RowPitch;
				for (UINT col = 0; col < desc.Width; col++)
				{
					UINT ColStart = col * 4;
					UINT uRed = pTexels[RowStart + ColStart + 0];
					UINT uGreen = pTexels[RowStart + ColStart + 1];
					UINT uBrue = pTexels[RowStart + ColStart + 2];
					m_fHeightList[row * desc.Width + col] = uRed;	/// DWORD이므로 pitch/4	
				}
			}
			m_pContext->Unmap(pTexture2D, D3D11CalcSubresource(0, 0, 1));
		}
	}

	BMapDesc MainMap;
	MainMap.iNumCols = desc.Width;
	MainMap.iNumRows = desc.Height;
	MainMap.fScaleHeight = 0.1f;
	MainMap.fCellDistance = m_fCellDistance; // 여기서 결정
	SetMapDesc(MainMap);
	S_RELEASE(pTexture2D);
	return true;
}
/*맵 정보 세팅 (높이맵 생성시에도 호출)*/
bool BMap::SetMapDesc(BMapDesc MapDesc)
{
	if (MapDesc.iNumCols == 0 || MapDesc.iNumRows == 0)
	{
		MapDesc.iNumCols = MapDesc.iCellCount * MapDesc.iPatchCount + 1;
		MapDesc.iNumRows = MapDesc.iCellCount * MapDesc.iPatchCount + 1;
	}
	/*생성 가능한 최대 사이즈 설정*/
	if (MapDesc.iNumCols > 1025 || MapDesc.iNumRows > 1025)
	{
		MapDesc.iNumCols = 1025; // 정점개수 105만개,
		MapDesc.iNumRows = 1025; // 페이스 개수 209만개 
	}

	m_MapDesc = MapDesc;
	m_iNumRows = MapDesc.iNumCols;
	m_iNumCols = MapDesc.iNumRows;
	m_iNumCellRows = m_iNumRows - 1;
	m_iNumCellCols = m_iNumCols - 1;
	m_iNumVertices = m_iNumRows*m_iNumCols;
	m_iNumFace = m_iNumCellRows*m_iNumCellCols * 2;
	m_fCellDistance = MapDesc.fCellDistance;
	return true;
}
#pragma endregion

#pragma region 맵 생성 관련 재정의 함수
/* 재정의 # 버텍스 버퍼 생성 함수 */
HRESULT BMap::CreateVertexBuffer()		// 버텍스 버퍼 생성
{
	HRESULT hr = S_OK;
	m_VertexList.resize(m_iNumVertices);
	float iHalfCols = (m_iNumCols - 1) / 2;
	float iHalfRows = (m_iNumRows - 1) / 2;
	float ftxOffsetU = 1.0f / (m_iNumCols - 1);
	float ftxOffsetV = 1.0f / (m_iNumRows - 1);

	for (int iRow = 0; iRow < m_iNumRows; iRow++)
	{
		for (int iCol = 0; iCol < m_iNumCols; iCol++)
		{
			int  iVertexIndex = iRow * m_iNumCols + iCol;
			m_VertexList[iVertexIndex].Position.x = ((float)iCol - iHalfCols) * m_fCellDistance;
			m_VertexList[iVertexIndex].Position.z = -(((float)iRow - iHalfRows) * m_fCellDistance);

			if (m_fHeightList.size() != 0)
			{
				m_VertexList[iVertexIndex].Position.y = m_fHeightList[iVertexIndex] * m_MapDesc.fScaleHeight;
			}
			else
			{
				m_VertexList[iVertexIndex].Position.y = 0.0f;
			}
			m_VertexList[iVertexIndex].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			m_VertexList[iVertexIndex].Color = D3DXVECTOR4(1, 1, 1, 1.0f);
			m_VertexList[iVertexIndex].Texture = D3DXVECTOR2(ftxOffsetU * iCol, ftxOffsetV * iRow); //x = icol * (1/width -1)   y = iRow * (1/height -1)
		}
	}

	m_VertexCnt = m_VertexList.size();

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;						// [메모리 할당 위치] GPU 메모리에 할당해라
	bd.ByteWidth = sizeof(PNCT_Vertex) * m_VertexCnt;	// [메모리 할당 크기] 점 X개를 할당 할 것이기 때문에 *X ( 수가 늘어나면 증가 할 것이다 )
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;			// [메모리 사용 용도] 버텍스 버퍼용으로 사용하겠다.
	bd.CPUAccessFlags = 0;								// CPU로 부터는 액세스 하지 않겠다.
	bd.MiscFlags = 0;									// 추가 설정 없음

	D3D11_SUBRESOURCE_DATA InitData;			// 소스 리소스 초기화에 필요한 데이터 지정
	InitData.pSysMem = (void*)&m_VertexList.at(0);				// 해당 버텍스를 소스로 사용하겠다.
	S_RETURN(m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer));

	return hr;
}
/* 재정의 # 인덱스 버퍼 생성 함수 */
HRESULT BMap::CreateIndexBuffer()		// 인덱스 버퍼 생성
{
	HRESULT hr = S_OK;
	m_IndexList.resize(m_iNumFace * 3);
	int iCurIndex = 0;
	for (int iRow = 0; iRow < m_iNumCellRows; iRow++)
	{
		for (int iCol = 0; iCol < m_iNumCellCols; iCol++)
		{
			//0	1    4   
			//2	   3 5
			int iNextRow = iRow + 1;
			int iNextCol = iCol + 1;
			m_IndexList[iCurIndex + 0] = iRow * m_iNumCols + iCol;
			m_IndexList[iCurIndex + 1] = iRow * m_iNumCols + iNextCol;
			m_IndexList[iCurIndex + 2] = iNextRow * m_iNumCols + iCol;
			m_IndexList[iCurIndex + 3] = m_IndexList[iCurIndex + 2];
			m_IndexList[iCurIndex + 4] = m_IndexList[iCurIndex + 1];
			m_IndexList[iCurIndex + 5] = iNextRow * m_iNumCols + iNextCol;

			iCurIndex += 6;
		}
	}

	UINT iNumIndex = m_IndexList.size();

	// 인덱스 버퍼 생성
	D3D11_BUFFER_DESC ibDesc;
	ibDesc.ByteWidth = iNumIndex * sizeof(DWORD);
	ibDesc.Usage = D3D11_USAGE_DEFAULT;
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER; // 인덱스 버퍼로 사용하겠다는 선언
	ibDesc.CPUAccessFlags = 0;
	ibDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA ibInitData;
	ZeroMemory(&ibInitData, sizeof(D3D11_SUBRESOURCE_DATA));
	ibInitData.pSysMem = (void*)&m_IndexList.at(0);
	S_RETURN(m_pd3dDevice->CreateBuffer(&ibDesc, &ibInitData, &m_pIndexBuffer));

	return hr;
}
/* 재정의 # 맵 쉐이더 레이아웃 생성 함수*/
HRESULT BMap::LoadShaderAndInputLayout(const TCHAR* pShaderFile)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS; // 쉐이더 디버그 허용
	ID3DBlob* pVSBUF = NULL;
	ID3DBlob* pPSBUF = NULL;
	ID3DBlob* pAlphaPSBUF = NULL;
	ID3DBlob* pGSBUF = NULL;

	ID3DBlob* pError = NULL; // 이것을 넣음으로써 오류가 난 이유를 받아 낼 수 있다.
							 /* vs_5_0 == direct11 은 5.0을 사용 * pVSBuf 결과가 담김 */

	if (FAILED(D3DX11CompileFromFile(pShaderFile, NULL, NULL, "VS", "vs_5_0", dwShaderFlags, NULL, NULL, &pVSBUF, &pError, NULL)))
	{
		TCHAR buffer[256] = { 0, };
		mbstowcs(buffer, (char*)pError->GetBufferPointer(), 256);
		MessageBox(NULL, buffer, _T("VS"), MB_OK);
	}
	if (pVSBUF != nullptr)
	{
		S_RETURN(m_pd3dDevice->CreateVertexShader((DWORD*)pVSBUF->GetBufferPointer(), pVSBUF->GetBufferSize(), NULL, &m_pVS));
	}

	if (FAILED(D3DX11CompileFromFile(pShaderFile, NULL, NULL, "GS", "gs_5_0", dwShaderFlags, NULL, NULL, &pGSBUF, &pError, NULL)))
	{
		TCHAR buffer[256] = { 0, };
		mbstowcs(buffer, (char*)pError->GetBufferPointer(), 256);
		//MessageBox(NULL, buffer, _T("GS"), MB_OK);
	}
	if (pGSBUF != nullptr)
	{
		S_RETURN(m_pd3dDevice->CreateGeometryShader((DWORD*)pGSBUF->GetBufferPointer(), pGSBUF->GetBufferSize(), NULL, &m_pGS));
	}

	if (FAILED(D3DX11CompileFromFile(pShaderFile, NULL, NULL, "PS", "ps_5_0", dwShaderFlags, NULL, NULL, &pPSBUF, &pError, NULL)))
	{
		TCHAR buffer[256] = { 0, };
		mbstowcs(buffer, (char*)pError->GetBufferPointer(), 256);
		//MessageBox(NULL, buffer, _T("PS"), MB_OK);
	}
	if (pPSBUF != nullptr)
	{
		S_RETURN(m_pd3dDevice->CreatePixelShader((DWORD*)pPSBUF->GetBufferPointer(), pPSBUF->GetBufferSize(), NULL, &m_pPS));
	}

	if (FAILED(D3DX11CompileFromFile(pShaderFile, NULL, NULL, "AlphaMap_PS", "ps_5_0", dwShaderFlags, NULL, NULL, &pAlphaPSBUF, &pError, NULL)))
	{
		TCHAR buffer[256] = { 0, };
		mbstowcs(buffer, (char*)pError->GetBufferPointer(), 256);
		//MessageBox(NULL, buffer, _T("PS"), MB_OK);
	}
	if (pAlphaPSBUF != nullptr)
	{
		S_RETURN(m_pd3dDevice->CreatePixelShader((DWORD*)pAlphaPSBUF->GetBufferPointer(), pAlphaPSBUF->GetBufferSize(), NULL, &m_pAlphaPS));
	}

	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",	  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTURE",  0, DXGI_FORMAT_R32G32_FLOAT,       0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	UINT iSize = sizeof(layout) / sizeof(layout[0]);

	S_RETURN(m_pd3dDevice->CreateInputLayout(layout, iSize, pVSBUF->GetBufferPointer(), pVSBUF->GetBufferSize(), &m_pVertexLayout)); // m_pVertexLayout 제작

	S_RELEASE(pVSBUF);
	S_RELEASE(pPSBUF);
	S_RELEASE(pAlphaPSBUF);
	S_RELEASE(pGSBUF);
	return hr;
}
#pragma endregion

#pragma region 맵 정보 반환함수 
/*맵의 해당위치 높이 반환 함수 ( int 기준 ) */
float BMap::GetHeightMap(int row, int col)
{
	return m_VertexList[row * m_iNumRows + col].Position.y;
	//m_fHeightList[row * m_iNumRows + col] * m_MapDesc.fScaleHeight;
}
/*맵의 해당위치 높이 반환 함수 ( float 기준 )*/
float BMap::GetHeightMap(float fPosX, float fPosZ)
{
	//fPosX/fPosZ의 위치에 해당하는 높이 맵셀을 찾는다.
	//m_iNumCols와 m_iNumRows은 가로 세로의 실제 크기값
	float fCellX = (float)((float)m_iNumCellCols * (float)m_fCellDistance / 2.0f + fPosX);
	float fCellZ = (float)((float)m_iNumCellRows * (float)m_fCellDistance / 2.0f - fPosZ);

	/*셀의 크기로 나누어 1단위의 값으로 바꿔 높이맵 배열에 접근*/
	fCellX /= (float)m_fCellDistance;
	fCellZ /= (float)m_fCellDistance;

	// fCellX, fCellZ값보다 작거나 같은 최대 정수 (소수부분을 잘라낸다)
	float fVertexCol = ::floorf(fCellX);
	float fVertexRow = ::floorf(fCellZ);

	// 높이맵의 범위를 벗어나면 강제 초기화
	if (fVertexCol < 0.0f) fVertexCol = 0.0f;
	if (fVertexRow < 0.0f) fVertexRow = 0.0f;
	if ((float)(m_iNumCols - 2) < fVertexCol) fVertexCol = (float)(m_iNumCols - 2);
	if ((float)(m_iNumRows - 2) < fVertexRow) fVertexRow = (float)(m_iNumRows - 2);

	// 계산된 셀의 플랜을 구성하는 4개 정점의 높이값을 찾는다. 
	//  A   B
	//  *---*
	//  | / |
	//  *---*  
	//  C   D
	float A = GetHeightMap((int)fVertexRow, (int)fVertexCol);
	float B = GetHeightMap((int)fVertexRow, (int)fVertexCol + 1);
	float C = GetHeightMap((int)fVertexRow + 1, (int)fVertexCol);
	float D = GetHeightMap((int)fVertexRow + 1, (int)fVertexCol + 1);

	// A정점의 위치에서 떨어진 값(변위값)을 계산한다.
	float fDeltaX = fCellX - fVertexCol;
	float fDeltaZ = fCellZ - fVertexRow;

	//보간작업을 위한 기준 페이스를 찾는다
	float fHeight = 0.0f;
	if (fDeltaZ < (1.0f - fDeltaX))
	{
		float uy = B - A; // A->B
		float vy = C - A; // A->C
		fHeight = A + Lerp(0.0f, uy, fDeltaX) + Lerp(0.0f, vy, fDeltaZ);
	}
	else
	{
		float uy = C - D;
		float vy = B - D;
		fHeight = D + Lerp(0.0f, uy, 1.0f - fDeltaX) + Lerp(0.0f, vy, 1.0f - fDeltaZ);
	}
	return fHeight;
}
/*맵의 해당 인덱스의 노말 반환 함수*/
D3DXVECTOR3 BMap::GetNormalOfVertex(UINT Index)
{
	return m_FaceNormals[Index];
}
#pragma endregion

#pragma region 맵 노말맵핑 관련 재정의 함수
/* 재정의 # 노말 & 접선벡터 생성함수*/
bool BMap::UpdateNormal()
{
	InitFaceNormal();
	GenNormalLookupTable();
	CalcPerVertexNormalsFastLookup();
	CalcVertexColor();
	UpDateTangentBuffer();
	return true;
}
/* 재정의 # 맵 노말에 따른 색상 변경함수*/
void BMap::CalcVertexColor()
{
	for (int iRow = 0; iRow < m_iNumRows; iRow++)
	{
		for (int iCol = 0; iCol < m_iNumCols; iCol++)
		{
			int  iVertexIndex = iRow * m_iNumCols + iCol;
			m_VertexList[iVertexIndex].Color.x = m_VertexList[iVertexIndex].Normal.x;
			m_VertexList[iVertexIndex].Color.y = m_VertexList[iVertexIndex].Normal.y;
			m_VertexList[iVertexIndex].Color.z = m_VertexList[iVertexIndex].Normal.z;
			m_VertexList[iVertexIndex].Color.w = 1.0f;
		}
	}
}
#pragma endregion

/*맵 보간용 함수*/
float BMap::Lerp(float fStart, float fEnd, float fTangent)
{
	return fStart - (fStart*fTangent) + (fEnd*fTangent);
}

/* 버텍스 버퍼 갱신함수 */
bool BMap::ReLoadVBuffer()
{
	m_pContext->UpdateSubresource(m_pVertexBuffer, 0, NULL, (void*)&m_VertexList.at(0), 0, 0);
	return true;
}


BMap::BMap()
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matRotate);
	D3DXMatrixIdentity(&m_matTrans);
	D3DXMatrixIdentity(&m_matScale);
	m_vLightDir = D3DXVECTOR3(0, -1, 0);
}
BMap::~BMap()
{
}
