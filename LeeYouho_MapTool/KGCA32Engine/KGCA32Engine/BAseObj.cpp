#include "BAseObj.h"


#pragma region 기본 구성 함수
bool BAseObj::Init() { return true; }
bool BAseObj::Frame()
{
	// m_fElapseTime = 진행되고 있는 애니메이션의 시간
	// 따라서 m_fElapseTime의 계산은 프레임당 경과된 시간 * 1초당 진행해야 하는 프레임속도 * 프레임당 틱수
	m_fElapseTime += g_fSecPerFrame * m_Scene.iFrameSpeed * m_Scene.iTickPerFrame * 0.1F;
	if (m_fElapseTime >= m_Scene.iLastFrame * m_Scene.iTickPerFrame)
	{
		m_fElapseTime = m_Scene.iFirstFrame * m_Scene.iTickPerFrame;
	}

	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	for (DWORD dwObject = 0; dwObject < m_pData.size(); dwObject++)
	{
		BMesh2* pMesh = m_pData[dwObject].get();
		if (pMesh->m_bUsed == false)continue;
		if (pMesh->m_pParent)
			Interpolate(pMesh, &pMesh->m_pParent->m_matCalculation, m_fElapseTime);
		else
			Interpolate(pMesh, &mat, m_fElapseTime);
	}
	return true;
}
bool BAseObj::Render(ID3D11DeviceContext* pContext, int DrawCount)
{
	/*쉐이더는 한번 세팅한 값은 영원히 간다 (Init에 만들어도 문제가 없음)*/
	/*쉐이더 적용*/
	pContext->HSSetShader(NULL, NULL, 0);
	pContext->DSSetShader(NULL, NULL, 0);
	pContext->GSSetShader(NULL, NULL, 0);
	/*최적화 세팅으로 인해 사용을 하지 않는다.*/
	//pContext->VSSetShader(m_pVS, NULL, 0);
	//pContext->PSSetShader(m_pPS, NULL, 0);
	//pContext->IASetInputLayout(m_pVertexLayout);
	pContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 어떤 형상으로 뿌릴것인가.
	
	/*레이아웃 세팅*/

	//pContext->OMSetRenderTargets(1, &m_pRTV, m_pDSV);		// 깊이 스텐실 뷰를 사용하겠다.
	//pContext->OMSetDepthStencilState(m_pDSS, 0x00);			// 깊이 스텐실 뷰를 사용하겠다.

	//Draw(pContext, this);
	DrawInstance(pContext, this, DrawCount);
	return true;
}
bool BAseObj::Release()
{
	return true;
}
#pragma endregion

#pragma region ASE 파일 파싱 관련함수
/*파일 로드 함수*/
bool BAseObj::Load(ID3D11Device* pd3dDevice, const TCHAR* strFileName, const TCHAR* strShaderName)
{
	m_pd3dDevice = pd3dDevice;
	//CStopwatch stopwatch;
	if (!Load(strFileName, pd3dDevice))
	{
		return false;
	}
	if (!Convert(pd3dDevice))
	{
		return false;
	}
	if (!CreateBox())
	{
		return false;
	}
	if (Create(pd3dDevice) != S_OK)
	{
		return false;
	}
	if (UpdateNormal() != true)
	{
		return false;
	}
	if (CreateNormalCB() != S_OK)
	{
		return false;
	}
	return true;
}
bool BAseObj::Load(const TCHAR* strFileName, ID3D11Device* pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;

	TCHAR Drive[260];
	TCHAR Dir[260];
	TCHAR FName[260];
	TCHAR Ext[260];

	/*받아온 파일명 분해*/
	_tsplitpath(strFileName, Drive, Dir, FName, Ext);

	/*경로저장*/
	m_szDirName = Drive;
	m_szDirName += Dir;

	/*파일명, 형식저장*/
	m_szName = FName;
	m_szName += Ext;

	if (m_Parser.OpenStream(strFileName))
	{
		INT iObjectType = -1;
		UINT iNumTypes = sizeof(AseSectionHeaderTokens) / sizeof(AseSectionHeaderTokens[0]);
		bool bRet = true;
		//enum AseSectionType { ASCIIEXPORT = 0, COMMENT, SCENE, MATERIAL_LIST, GEOM_OBJECT, HELPER_OBJECT, };

		/* 씬 & 매터리얼 로드 */
		while ((iObjectType = SearchTokenArray(iNumTypes, AseSectionHeaderTokens)) >= 0)  // 헤더 부분을 받아온다.
		{
			switch (iObjectType)
			{
			case SCENE:
			{
				if (!LoadScene()) return m_Parser.CloseStream(_T("파일 오류"));
			}break;
			case MATERIAL_LIST:
			{
				if (!LoadMaterial()) return m_Parser.CloseStream(_T("파일 오류"));
			}break;
			case GEOM_OBJECT:
			{
				if (!LoadObject()) return m_Parser.CloseStream(_T("파일 오류"));
			}break;
			case HELPER_OBJECT:
			{
				if (!LoadHelperObject()) return m_Parser.CloseStream(_T("파일 오류"));
			}break;
			default:
				return m_Parser.CloseStream(_T("파일 오류"));
			}
		}
		return m_Parser.CloseStream();
	}
	return false;
}
/*파일 로드 보조함수(Load 함수 내부 호출)*/
bool BAseObj::LoadScene()
{
	if (!m_Parser.GetData(_T("*SCENE_FIRSTFRAME"), &m_Scene.iFirstFrame, INT_DATA)) return false;
	if (!m_Parser.GetData(_T("*SCENE_LASTFRAME"), &m_Scene.iLastFrame, INT_DATA)) return false;
	if (!m_Parser.GetData(_T("*SCENE_FRAMESPEED"), &m_Scene.iFrameSpeed, INT_DATA)) return false;
	if (!m_Parser.GetData(_T("*SCENE_TICKSPERFRAME"), &m_Scene.iTickPerFrame, INT_DATA)) return false;
	return true;
}
bool BAseObj::LoadMaterial()
{
	//enum AseMaterialType { MATERIAL_COUNT = 0, MATERIAL, NUMSUBMTLS, SUBMATERIAL, MAP_SUBNO, BITMAP_FILE, };
	int iNumTypes = sizeof(AseMaterialTokens) / sizeof(AseMaterialTokens[0]); // ARRAYSIZE

	TCHAR szTexName[256] = _T("");
	BMtrl* pCurrentMtrl = NULL;

	int iObjectType = -1;
	while ((iObjectType = SearchTokenArray(iNumTypes, AseMaterialTokens)) >= 0)
	{
		switch (iObjectType)
		{
		case MATERIAL:
		{
			BMtrl Material;
			m_Material.push_back(Material);
			pCurrentMtrl = &m_Material[m_Material.size() - 1];
		}break;

		case SUBMATERIAL:
		{
			BMtrl SubMaterial;
			m_Material[m_Material.size() - 1].m_SubMaterial.push_back(SubMaterial);
			/*현재 메테리얼 = 지정된 메테리얼의 서브 메테리얼 {서브 메테리얼의 순서는 지정된 메테리얼이 지니고 있는 서브 메테리얼의 사이즈 -1 (현재 서브 메테리얼의 위치)}*/
			pCurrentMtrl = &m_Material[m_Material.size() - 1].m_SubMaterial[m_Material[m_Material.size() - 1].m_SubMaterial.size() - 1];
		}break;

		case MAP_SUBNO:
		{
			BTextexMap TexMap;
			if (!m_Parser.GetData(&TexMap.m_dwType, INT_DATA)) return false;
			pCurrentMtrl->m_TexMaps.push_back(TexMap);
		}break;

		case BITMAP_FILE:
		{
			m_Parser.GetData(&szTexName, STRING_DATA);
			BTextexMap* pMapType = &pCurrentMtrl->m_TexMaps[pCurrentMtrl->m_TexMaps.size() - 1];

			TCHAR Drive[260];
			TCHAR Dir[260];
			TCHAR FName[260];
			TCHAR Ext[260];
			TCHAR *LoadName;					 // 텍스쳐명
			TCHAR *LoadNormalName;				 // 노말텍스쳐명

			/*받아온 파일명 분해*/
			_tsplitpath(szTexName, Drive, Dir, FName, Ext);

			T_STR temp = _T("../../data/"); // 임시 파일위치 지정
											/*파일명, 형식저장*/
			temp += FName;
			temp += Ext;

			T_STR Normaltemp = _T("../../data/");
			T_STR Nmp = _T("_nmp");
			Normaltemp += FName;
			Normaltemp += Nmp;
			Normaltemp += Ext;

			pMapType->m_strTextureName = temp;
			pMapType->m_strNormalTextureName = Normaltemp;
			pMapType->m_dwIndex = m_iCurIndex;

			LoadName = (TCHAR*)temp.c_str();
			TextureLoad(m_pd3dDevice, LoadName);

			LoadNormalName = (TCHAR*)Normaltemp.c_str();
			NormalTextureLoad(m_pd3dDevice, LoadNormalName);
			//텍스쳐를 따로 저장하는 클래스가 없으므로 szTexName 을 이용해 create 과정에서 직접 제작해야 한다.
			//pMapType->m_dwIndex = I_Texture.Add(m_pd3dDevice, pMapType->m_strTextureName.c_str(), m_szDirName.c_str());
		}break;

		case RETURN_GEOM_OBJECT:
		case RETURN_HELPER_OBJECT:
		{
			m_Parser.RestoreFilePosition();
			return true;
		}
		}
	}
	return true;
}
bool BAseObj::LoadObject()
{
	INT ObjectType = -1;
	BGeomMesh Object;
	m_GeomObject.push_back(Object);
	BGeomMesh* pGeomObj = &m_GeomObject[m_GeomObject.size() - 1]; //Object를 등록시키고 포인트로 받아옴
	pGeomObj->m_ClassType = CLASS_GEOM;

	TCHAR szNodeName[256] = _T("");
	TCHAR szParentName[256] = _T("");

	UINT iNumTypes = sizeof(AseObjectTokens) / sizeof(AseObjectTokens[0]);
	while ((ObjectType = SearchTokenArray(iNumTypes, AseObjectTokens)) >= 0)
	{
		switch (ObjectType)
		{

		case NEW_GEOM_OBJECT:
		{
			m_Parser.RestoreFilePosition();
			return true;
		}break;
		case NEW_HELPER_OBJECT:
		{
			m_Parser.RestoreFilePosition();
			return true;
		}break;

		case NODE_NAME:
		{
			m_Parser.GetData(&szNodeName, STRING_DATA);
			pGeomObj->m_strNodeName = szNodeName;
		}break;

		case PARENT_NAME:
		{
			m_Parser.GetData(&szParentName, STRING_DATA);
			pGeomObj->m_strParentName = szParentName;
		}break;

		case NODE_TM:
		{
			/*D3DXMatrixIdentity(&pGeomObj->m_matWorld);
			if (!m_Parser.GetData(_T("*TM_ROW0"), &pGeomObj->m_matWorld.m[0], VECTOR_DATA)) return false;
			if (!m_Parser.GetData(_T("*TM_ROW1"), &pGeomObj->m_matWorld.m[2], VECTOR_DATA)) return false;
			if (!m_Parser.GetData(_T("*TM_ROW2"), &pGeomObj->m_matWorld.m[1], VECTOR_DATA)) return false;
			if (!m_Parser.GetData(_T("*TM_ROW3"), &pGeomObj->m_matWorld.m[3], VECTOR_DATA)) return false;
			pGeomObj->m_matWorld._44 = 1.0f;*/
			LoadTM(pGeomObj);
		}break;

		case MESH:
		{
			// Position Data
			m_Parser.GetData(_T("*MESH_NUMVERTEX"), &pGeomObj->m_PosVertexList.dwNumVertex, INT_DATA);
			pGeomObj->m_iNumFace = 0;
			if (pGeomObj->m_PosVertexList.dwNumVertex > 0)
			{
				m_Parser.GetData(_T("*MESH_NUMFACES"), &pGeomObj->m_PosVertexList.dwNumFace, INT_DATA);
				pGeomObj->m_iNumFace = pGeomObj->m_PosVertexList.dwNumFace;

				GetVertexListFromString(_T("*MESH_VERTEX_LIST"), pGeomObj->m_PosVertexList.dwNumVertex, pGeomObj->m_PosVertexList.pVertexList);
				GetPositionFaceListFromString(_T("*MESH_FACE_LIST"), pGeomObj->m_PosVertexList.dwNumFace, pGeomObj->m_PosVertexList);
			}
			// Texture Coord data
			m_Parser.GetData(_T("*MESH_NUMTVERTEX"), &pGeomObj->m_TexVertexList.dwNumVertex, INT_DATA);
			if (pGeomObj->m_TexVertexList.dwNumVertex > 0)
			{
				GetVertexListFromString(_T("*MESH_TVERTLIST"), pGeomObj->m_TexVertexList.dwNumVertex, pGeomObj->m_TexVertexList.pVertexList);
				m_Parser.GetData(_T("*MESH_NUMTVFACES"), &pGeomObj->m_TexVertexList.dwNumFace, INT_DATA);
				GetFaceListFromString(_T("*MESH_TFACELIST"), pGeomObj->m_TexVertexList.dwNumFace, pGeomObj->m_TexVertexList);
			}
			// Color Data
			m_Parser.GetData(_T("*MESH_NUMCVERTEX"), &pGeomObj->m_ColVertexList.dwNumVertex, INT_DATA);
			if (pGeomObj->m_ColVertexList.dwNumVertex > 0)
			{
				GetVertexListFromString(_T("*MESH_CVERTLIST"), pGeomObj->m_ColVertexList.dwNumVertex, pGeomObj->m_ColVertexList.pVertexList);
				m_Parser.GetData(_T("*MESH_NUMCVFACES"), &pGeomObj->m_ColVertexList.dwNumFace, INT_DATA);
				GetFaceListFromString(_T("*MESH_CFACELIST"), pGeomObj->m_ColVertexList.dwNumFace, pGeomObj->m_ColVertexList);
			}
			// Normal Data
			if (pGeomObj->m_iNumFace > 0)
			{
				GetNormalListFromString(_T("*MESH_NORMALS"), pGeomObj->m_iNumFace, pGeomObj);
			}
		}break;

		case TM_ANIMATION:
		{
			LoadAnimation(pGeomObj);
		}break;

		case TM_VISIBILITY:
		{
			// 애니메이션 구현 함수 필요
		}break;

		case MATERIAL_REF:
		{
			m_Parser.GetData(&pGeomObj->m_iMtrlRef, INT_DATA);
		}break;
		}
	}
	return true;
}
bool BAseObj::LoadHelperObject()
{
	INT ObjectType = -1;
	BGeomMesh Object;
	m_GeomObject.push_back(Object);
	BGeomMesh* pGeomObj = &m_GeomObject[m_GeomObject.size() - 1]; //Object를 등록시키고 포인트로 받아옴

	TCHAR szNodeName[256] = _T("");
	TCHAR szParentName[256] = _T("");

	//m_Parser.GetData(_T("NODE_NAME"), &szNodeName, STRING_DATA);
	//pGeomObj->m_strNodeName = szNodeName;

	UINT iNumTypes = sizeof(AseHelperObjectTokens) / sizeof(AseHelperObjectTokens[0]);
	while ((ObjectType = SearchTokenArray(iNumTypes, AseHelperObjectTokens)) >= 0)
	{
		switch (ObjectType)
		{
		case HELPER_NODE_NAME:
		{
			m_Parser.GetData(&szNodeName, STRING_DATA);
			pGeomObj->m_strNodeName = szNodeName;
		}break;

		case HELPER_NODE_PARENT:
		{
			m_Parser.GetData(&szParentName, STRING_DATA);
			pGeomObj->m_strParentName = szParentName;
		}break;

		case HELPER_CLASS:
		{
			TCHAR szHelperClassName[256] = _T("");
			m_Parser.GetData(&szHelperClassName, STRING_DATA);
			if (_tcsicmp(szHelperClassName, _T("Bone")) == 0)
			{
				pGeomObj->m_ClassType = CLASS_BONE;
			}
			else if (_tcsicmp(szHelperClassName, _T("Dummy")) == 0)
			{
				pGeomObj->m_ClassType = CLASS_DUMMY;
			}
		}break;
		case HELPER_MODE_TM:
		{
			LoadTM(pGeomObj);
		}break;
		case BOUNDINGBOX_MIN:
		{
			m_Parser.GetData(&pGeomObj->m_BBox.vMin, VECTOR_DATA);
			m_Parser.GetData(&pGeomObj->m_BBox.vMax, VECTOR_DATA);
		}break;
		case HELPER_TM_ANIMATION:
		{
			LoadAnimation(pGeomObj);
		}break;
		case HELPER_GEOM_OBJECT:
		{
			m_Parser.RestoreFilePosition();
			return true;
		}break;
		case HELPER_HELPER_OBJECT:
		{
			m_Parser.RestoreFilePosition();
			return true;
		}break;
		default:
		{
			return true;
		}break;
		}
	}
	return true;
}
bool BAseObj::LoadTM(BGeomMesh* pObject)
{
	D3DXMatrixIdentity(&pObject->m_matWorld);
	if (!m_Parser.GetData(_T("*TM_ROW0"), &pObject->m_matWorld.m[0], VECTOR_DATA)) return false;
	if (!m_Parser.GetData(_T("*TM_ROW1"), &pObject->m_matWorld.m[2], VECTOR_DATA)) return false;
	if (!m_Parser.GetData(_T("*TM_ROW2"), &pObject->m_matWorld.m[1], VECTOR_DATA)) return false;
	if (!m_Parser.GetData(_T("*TM_ROW3"), &pObject->m_matWorld.m[3], VECTOR_DATA)) return false;
	pObject->m_matWorld._44 = 1.0f;

	//인버스 매트릭스 확인 코드
	D3DXVECTOR3 v0, v1, v2, v3;
	v0 = pObject->m_matWorld.m[0];
	v1 = pObject->m_matWorld.m[1];
	v2 = pObject->m_matWorld.m[2];
	D3DXVec3Cross(&v3, &v1, &v2);
	if (D3DXVec3Dot(&v3, &v0) < 0.0f)
	{
		D3DXMATRIX matW;
		D3DXMatrixScaling(&matW, -1.0f, -1.0f, -1.0f);
		D3DXMatrixMultiply(&pObject->m_matWorld, &pObject->m_matWorld, &matW);
	}

	D3DXVECTOR3 vVector;
	float fAngle;
	D3DXQUATERNION qRotation;
	m_Parser.GetData(_T("*TM_POS"), &vVector, VECTOR_DATA);
	D3DXMatrixTranslation(&pObject->m_matWorldTrans, vVector.x, vVector.y, vVector.z);

	m_Parser.GetData(_T("*TM_ROTAXIS"), &vVector, VECTOR_DATA);
	m_Parser.GetData(_T("*TM_ROTANGLE"), &fAngle, FLOAT_DATA);
	//임의의 축과 각을 쿼터니언으로 변환
	D3DXQuaternionRotationAxis(&qRotation, &vVector, fAngle);
	D3DXMatrixRotationQuaternion(&pObject->m_matWorldRotate, &qRotation);

	//스케일 축을 중심으로 회전하는 값이 스케일 값이므로
	// 스케일 축의 회전만큼을 먼저 반대로 회전한 이후
	// 스케일 값을 적용시키고 그다음에 다시 스케일축만큼 회전시켜 원상복귀
	D3DXMATRIX matScl;
	m_Parser.GetData(_T("*TM_SCALE"), &vVector, VECTOR_DATA);
	D3DXMatrixScaling(&pObject->m_matWorldScale, vVector.x, vVector.y, vVector.z);
	D3DXVECTOR3 vAxis;
	m_Parser.GetData(_T("*TM_SCALEAXIS"), &vAxis, VECTOR_DATA);
	m_Parser.GetData(_T("*TM_SCALEAXISANG"), &fAngle, FLOAT_DATA);

	// 스케일축의 행렬과 그 역행렬을 구한다.
	D3DXMATRIX matRotation, matRotationInv;
	D3DXMatrixRotationAxis(&matRotation, &vAxis, fAngle);
	D3DXMatrixInverse(&matRotationInv, NULL, &matRotation);
	pObject->m_matWorldScale = matRotationInv * pObject->m_matWorldScale * matRotation;
}
bool BAseObj::LoadAnimation(BGeomMesh* pGeomObj)
{
	pGeomObj->m_PosTrack.reserve(10000);
	INT AniType = -1;
	UINT iNumTypes = sizeof(AseAniTokens) / sizeof(AseAniTokens[0]);
	while ((AniType = SearchTokenArray(iNumTypes, AseAniTokens)) >= 0)
	{
		switch (AniType + 70)
		{
		case CONTROL_POS_SAMPLE:
		{
			m_Parser.GetData(&pGeomObj->m_PosTrack, ANIPOS_DATA);
		}break;
		case CONTROL_ROT_SAMPLE:
		{
			m_Parser.GetData(&pGeomObj->m_RotTrack, ANIROT_DATA);
		}break;
		case CONTROL_SCL_SAMPLE:
		{
			m_Parser.GetData(&pGeomObj->m_SclTrack, ANISCL_DATA);
		}break;
		case TM_MATERIAL_REF:
		{
			m_Parser.RestoreFilePosition();
			return true;
		}break;
		case NEW_GEOM:
		{
			m_Parser.RestoreFilePosition();
			return true;
		}
		case NEW_HELPER:
		{
			m_Parser.RestoreFilePosition();
			return true;
		}
		}
	}
	return true;
}
/*버텍스 정보 로드함수*/
bool BAseObj::GetVertexListFromString(TCHAR* strVertexList, DWORD dwNumFace, vector<D3DXVECTOR3>& VertexList, TCHAR* strToken)
{
	if (dwNumFace > 0 && m_Parser.GetData(strVertexList))
	{
		VertexList.resize(dwNumFace);
		for (DWORD dwVertexCount = 0; dwVertexCount < dwNumFace; dwVertexCount++)
		{
			D3DXVECTOR3 vVertex;
			_stscanf(m_Parser.GetNextLine(), _T("%s%d%f%f%f"), m_Parser.m_pString, &m_Parser.m_iData, &vVertex.x, &vVertex.z, &vVertex.y);
			VertexList[dwVertexCount] = vVertex;
		}
	}
	return true;
}
/*페이스 정보 로드함수(칼라, 노말)*/
bool BAseObj::GetFaceListFromString(TCHAR* strFaceList, DWORD dwNumFace, BVertexList& VertexList, TCHAR* strToken)
{
	if (dwNumFace > 0 && m_Parser.GetData(strFaceList))
	{
		VertexList.pFaceList.resize(dwNumFace);
		for (DWORD dwFaceCount = 0; dwFaceCount < dwNumFace; dwFaceCount++)
		{
			BFaceList vFaceList;
			_stscanf(m_Parser.GetNextLine(), _T("%s%d %d%d%d"), m_Parser.m_pString, &m_Parser.m_iData, &vFaceList._0, &vFaceList._2, &vFaceList._1);
			VertexList.pFaceList[dwFaceCount] = vFaceList;
		}
	}
	return true;
}
/*페이스 정보 로드함수(포지션)*/
bool BAseObj::GetPositionFaceListFromString(TCHAR* strFaceList, DWORD dwNumFace, BVertexList& VertexList, TCHAR* strToken)
{
	if (dwNumFace > 0 && m_Parser.GetData(strFaceList))
	{
		VertexList.pFaceList.resize(dwNumFace);
		VertexList.pSubListMtl.resize(dwNumFace);

		for (DWORD dwFace = 0; dwFace < dwNumFace; dwFace++)
		{
			// *MESH_FACE
			_stscanf(m_Parser.GetNextLine(), _T("%s%s %s%d %s%d %s%d %s%d%s%d%s%d"),
				m_Parser.m_pString, m_Parser.m_pString,
				m_Parser.m_pString, &VertexList.pFaceList[dwFace]._0,
				m_Parser.m_pString, &VertexList.pFaceList[dwFace]._2,
				m_Parser.m_pString, &VertexList.pFaceList[dwFace]._1,

				m_Parser.m_pString, &m_Parser.m_iData,	// AB:
				m_Parser.m_pString, &m_Parser.m_iData,	// BC:
				m_Parser.m_pString, &m_Parser.m_iData);	// CA:
														// *MESH_SMOOTHING 이 빠져있는 상태이다 나중에 사용한다고 하니 수정이 필요
			TCHAR* pBuffer = _tcsstr(m_Parser.m_pBuffer, L"*MESH_MTLID");
			_stscanf(pBuffer, _T("%s %d"), m_Parser.m_pString, &VertexList.pSubListMtl[dwFace]);
			//		m_Parser.m_pString, &m_Parser.m_iData,  // *MESH_SMOOTHING
			//		m_Parser.m_pString, &VertexList.pSubListMtl[dwFace]);	// *MESH_MTLID			
		}

		//for (DWORD dwFace = 0; dwFace < dwNumFace; dwFace++)
		//{
		//	// *MESH_FACE
		//	_stscanf(m_Parser.GetNextLine(), _T("%s%s %s%d %s%d %s%d %s%d%s%d%s%d %s%d %s%d"),
		//		m_Parser.m_pString, m_Parser.m_pString,
		//		m_Parser.m_pString, &VertexList.pFaceList[dwFace]._0,
		//		m_Parser.m_pString, &VertexList.pFaceList[dwFace]._2,
		//		m_Parser.m_pString, &VertexList.pFaceList[dwFace]._1,

		//		m_Parser.m_pString, &m_Parser.m_iData,	// AB:
		//		m_Parser.m_pString, &m_Parser.m_iData,	// BC:
		//		m_Parser.m_pString, &m_Parser.m_iData,	// CA:

		//		m_Parser.m_pString, &m_Parser.m_iData,  // *MESH_SMOOTHING
		//		m_Parser.m_pString, &VertexList.pSubListMtl[dwFace]);	// *MESH_MTLID			
		//}
	}
	return true;
}
/*노말 정보 로드 함수*/
bool BAseObj::GetNormalListFromString(TCHAR* strFaceList, DWORD dwNumFace, BGeomMesh* pGeomObj, TCHAR* strToken)
{
	if (m_Parser.GetData(strFaceList))
	{
		pGeomObj->m_pNorVertexList.resize(dwNumFace * 4);
		for (DWORD dwFaceCount = 0; dwFaceCount < dwNumFace; dwFaceCount++)
		{
			D3DXVECTOR3 vVertex;
			_stscanf(m_Parser.GetNextLine(), _T("%s%d%f%f%f"), m_Parser.m_pString, &m_Parser.m_iData, &vVertex.x, &vVertex.z, &vVertex.y);
			pGeomObj->m_pNorVertexList[dwFaceCount * 4 + 0] = vVertex;
			_stscanf(m_Parser.GetNextLine(), _T("%s%d%f%f%f"), m_Parser.m_pString, &m_Parser.m_iData, &vVertex.x, &vVertex.z, &vVertex.y);
			pGeomObj->m_pNorVertexList[dwFaceCount * 4 + 1] = vVertex;
			_stscanf(m_Parser.GetNextLine(), _T("%s%d%f%f%f"), m_Parser.m_pString, &m_Parser.m_iData, &vVertex.x, &vVertex.z, &vVertex.y);
			pGeomObj->m_pNorVertexList[dwFaceCount * 4 + 3] = vVertex;
			_stscanf(m_Parser.GetNextLine(), _T("%s%d%f%f%f"), m_Parser.m_pString, &m_Parser.m_iData, &vVertex.x, &vVertex.z, &vVertex.y);
			pGeomObj->m_pNorVertexList[dwFaceCount * 4 + 2] = vVertex;
		}
	}
	return true;
}
/*토큰 검색 함수 (Load 내부 호출)*/
int BAseObj::SearchTokenArray(INT NumObjType, TCHAR** pObjectType)
{
	return m_Parser.GetSearchStringArray(pObjectType, NumObjType, true);
}
#pragma endregion 

#pragma region 파싱정보변환 관련 함수
/*정보 변환 함수*/
bool BAseObj::Convert(ID3D11Device* pd3dDevice)
{
	//shared_ptr<BMesh2> pAseMesh = make_shared<BMesh2>();
	for (DWORD dwObject = 0; dwObject < m_GeomObject.size(); dwObject++)
	{
		//auto
		shared_ptr<BMesh2> pAseMesh = make_shared<BMesh2>();
		BMesh2* pMesh = pAseMesh.get();
		int iSubMtrl = -1;
		if (m_GeomObject[dwObject].m_iMtrlRef >= 0)
		{
			iSubMtrl = m_Material[m_GeomObject[dwObject].m_iMtrlRef].m_SubMaterial.size();
		}
		pMesh->m_strNodeName = m_GeomObject[dwObject].m_strNodeName;
		pMesh->m_strParentName = m_GeomObject[dwObject].m_strParentName;
		pMesh->m_iNumFace = m_GeomObject[dwObject].m_iNumFace;
		pMesh->m_matWorld = m_GeomObject[dwObject].m_matWorld;
		pMesh->m_ClassType = m_GeomObject[dwObject].m_ClassType;
		pMesh->m_matCalculation = m_GeomObject[dwObject].m_matWorld;
		pMesh->m_matWorldTrans = m_GeomObject[dwObject].m_matWorldTrans;
		pMesh->m_matWorldRotate = m_GeomObject[dwObject].m_matWorldRotate;
		pMesh->m_matWorldScale = m_GeomObject[dwObject].m_matWorldScale;

		D3DXMatrixInverse(&pMesh->m_matInverse, NULL, &pMesh->m_matWorld);

		//pMesh->m_iDiffuseTex = -1;

		if (pMesh->m_ClassType != 0)
		{
			/*포지션 트랙*/
			BAnimTrack* pPrevTrack = NULL;
			//첫번째 트랙은 이전 트랙이 없으므로 prev링크드리스트 구성을 NULL로 해주기 위함
			for (int iTrack = 0; iTrack < m_GeomObject[dwObject].m_PosTrack.size(); iTrack++)
			{
				auto pTrack = make_shared<BAnimTrack>();
				pTrack->iTick = m_GeomObject[dwObject].m_PosTrack[iTrack].iTick;
				pTrack->vVector = m_GeomObject[dwObject].m_PosTrack[iTrack].vVector;
				// 각 트랙을 돌면서 틱값과 백터의 값을 저장한다.
				pPrevTrack = SetDoublyLinkedList(pTrack.get(), pPrevTrack);
				// 각 트랙간의 링크드리스트 구성, pPrevTrack을 반환받는 이유는 다음 for문을 돌때 prevtrack으로 현재 트랙을 이용하기 때문에 (반환되는 값은 현재트랙)
				pMesh->m_pPosTrack.push_back(pTrack);
			}

			/*로테이션 트랙*/
			pPrevTrack = NULL;
			for (int iTrack = 0; iTrack < m_GeomObject[dwObject].m_RotTrack.size(); iTrack++)
			{
				auto pTrack = make_shared<BAnimTrack>();
				pTrack->iTick = m_GeomObject[dwObject].m_RotTrack[iTrack].iTick;

				//임의의 축과 각을 쿼터니언으로 변환
				//세타 / 2 가 아닌 이유는 D3DXQuaternionRotationAxis에서 받아온 w 값을 2로 나눠서 계산해주기 때문에 (편리하다)
				D3DXQuaternionRotationAxis(&pTrack->qRotate, &D3DXVECTOR3(m_GeomObject[dwObject].m_RotTrack[iTrack].qRotate.x,
					m_GeomObject[dwObject].m_RotTrack[iTrack].qRotate.y,
					m_GeomObject[dwObject].m_RotTrack[iTrack].qRotate.z),
					m_GeomObject[dwObject].m_RotTrack[iTrack].qRotate.w); // w는 세타값
																		  // 이전트랙의 쿼터니온과 누적
				if (pPrevTrack != NULL)
				{
					D3DXQuaternionMultiply(&pTrack->qRotate, &pPrevTrack->qRotate, &pTrack->qRotate);
				}
				pPrevTrack = SetDoublyLinkedList(pTrack.get(), pPrevTrack);
				pMesh->m_pRotTrack.push_back(pTrack);
			}


			/*스케일 트랙*/
			pPrevTrack = NULL;
			for (int iTrack = 0; iTrack < m_GeomObject[dwObject].m_SclTrack.size(); iTrack++)
			{
				auto pTrack = make_shared<BAnimTrack>();
				pTrack->iTick = m_GeomObject[dwObject].m_SclTrack[iTrack].iTick;
				pTrack->vVector = m_GeomObject[dwObject].m_SclTrack[iTrack].vVector;

				// 임의의 축과 각을 쿼터니언으로 변환
				D3DXQuaternionRotationAxis(&pTrack->qRotate, &D3DXVECTOR3(m_GeomObject[dwObject].m_SclTrack[iTrack].qRotate.x,
					m_GeomObject[dwObject].m_SclTrack[iTrack].qRotate.y,
					m_GeomObject[dwObject].m_SclTrack[iTrack].qRotate.z),
					m_GeomObject[dwObject].m_SclTrack[iTrack].qRotate.w);
				pPrevTrack = SetDoublyLinkedList(pTrack.get(), pPrevTrack);
				pMesh->m_pSclTrack.push_back(pTrack);
			}
			m_pData.push_back(pAseMesh);
			continue;
		}

		if (iSubMtrl <= 0) // 서브메터리얼이 없는 경우
		{
			if (!SetTriangleBuffer(pMesh, &m_GeomObject[dwObject]))
			{
				//S_DEL(pMesh);
				continue;
			}

			pMesh->SetUniqueBuffer();
			if (m_GeomObject[dwObject].m_iMtrlRef >= 0) // 메테리얼이 있다면, 없으면 -1
			{
				BMtrl* pMtrl = &m_Material[m_GeomObject[dwObject].m_iMtrlRef];
				if (pMtrl->m_TexMaps.size())
				{
					pMesh->m_iDiffuseTex[0] = pMtrl->m_TexMaps[0].m_dwIndex;// 임시코드
					pMesh->m_TextureName = pMtrl->m_TexMaps[0].m_strTextureName;
					pMesh->m_NormalTextureName = pMtrl->m_TexMaps[0].m_strNormalTextureName;
				}
			}

			/*포지션 트랙*/
			BAnimTrack* pPrevTrack = NULL;
			//첫번째 트랙은 이전 트랙이 없으므로 prev링크드리스트 구성을 NULL로 해주기 위함
			for (int iTrack = 0; iTrack < m_GeomObject[dwObject].m_PosTrack.size(); iTrack++)
			{
				auto pTrack = make_shared<BAnimTrack>();
				pTrack->iTick = m_GeomObject[dwObject].m_PosTrack[iTrack].iTick;
				pTrack->vVector = m_GeomObject[dwObject].m_PosTrack[iTrack].vVector;
				// 각 트랙을 돌면서 틱값과 백터의 값을 저장한다.
				pPrevTrack = SetDoublyLinkedList(pTrack.get(), pPrevTrack);
				// 각 트랙간의 링크드리스트 구성, pPrevTrack을 반환받는 이유는 다음 for문을 돌때 prevtrack으로 현재 트랙을 이용하기 때문에 (반환되는 값은 현재트랙)
				pMesh->m_pPosTrack.push_back(pTrack);
			}


			/*로테이션 트랙*/
			pPrevTrack = NULL;
			for (int iTrack = 0; iTrack < m_GeomObject[dwObject].m_RotTrack.size(); iTrack++)
			{
				auto pTrack = make_shared<BAnimTrack>();
				pTrack->iTick = m_GeomObject[dwObject].m_RotTrack[iTrack].iTick;

				//임의의 축과 각을 쿼터니언으로 변환
				//세타 / 2 가 아닌 이유는 D3DXQuaternionRotationAxis에서 받아온 w 값을 2로 나눠서 계산해주기 때문에 (편리하다)
				D3DXQuaternionRotationAxis(&pTrack->qRotate, &D3DXVECTOR3(m_GeomObject[dwObject].m_RotTrack[iTrack].qRotate.x,
					m_GeomObject[dwObject].m_RotTrack[iTrack].qRotate.y,
					m_GeomObject[dwObject].m_RotTrack[iTrack].qRotate.z),
					m_GeomObject[dwObject].m_RotTrack[iTrack].qRotate.w); // w는 세타값
																		  // 이전트랙의 쿼터니온과 누적
				if (pPrevTrack != NULL)
				{
					D3DXQuaternionMultiply(&pTrack->qRotate, &pPrevTrack->qRotate, &pTrack->qRotate);
				}
				pPrevTrack = SetDoublyLinkedList(pTrack.get(), pPrevTrack);
				pMesh->m_pRotTrack.push_back(pTrack);
			}


			/*스케일 트랙*/
			pPrevTrack = NULL;
			for (int iTrack = 0; iTrack < m_GeomObject[dwObject].m_SclTrack.size(); iTrack++)
			{
				auto pTrack = make_shared<BAnimTrack>();
				pTrack->iTick = m_GeomObject[dwObject].m_SclTrack[iTrack].iTick;
				pTrack->vVector = m_GeomObject[dwObject].m_SclTrack[iTrack].vVector;

				// 임의의 축과 각을 쿼터니언으로 변환
				D3DXQuaternionRotationAxis(&pTrack->qRotate, &D3DXVECTOR3(m_GeomObject[dwObject].m_SclTrack[iTrack].qRotate.x,
					m_GeomObject[dwObject].m_SclTrack[iTrack].qRotate.y,
					m_GeomObject[dwObject].m_SclTrack[iTrack].qRotate.z),
					m_GeomObject[dwObject].m_SclTrack[iTrack].qRotate.w);
				pPrevTrack = SetDoublyLinkedList(pTrack.get(), pPrevTrack);
				pMesh->m_pSclTrack.push_back(pTrack);
			}

			/*vistrack 미구현*/

			m_pData.push_back(pAseMesh);
			continue;
		}
		else // 서브 메터리얼이 있는 경우
		{
			// CStopwatch watch; 아마 이벤트 관련변수
			for (int iSub = 0; iSub < iSubMtrl; iSub++)
			{
				shared_ptr<BMesh2> pSubAseMesh = make_shared<BMesh2>();
				BMesh2* pSubMesh = pSubAseMesh.get();
				pSubMesh->m_matWorld = pMesh->m_matWorld;
				pMesh->m_pSubMesh.push_back(pSubAseMesh);
			}

			/*버텍스 생성과정에서 월드행렬의 역행렬을 곱해줘서 로컬행렬로 만들어 줌*/
			if (!SetTriangleSubBuffer(pMesh->m_pSubMesh, &m_GeomObject[dwObject]))
			{
				continue;
			}


			for (int iSub = 0; iSub < iSubMtrl; iSub++)
			{
				BMesh2* pSubMesh = pMesh->m_pSubMesh[iSub].get();
				pSubMesh->m_iNumFace = pSubMesh->m_FaceList.size();
				pSubMesh->SetUniqueBuffer();
				//pSubMesh->m_iDiffuseTex = -1;
				if (m_GeomObject[dwObject].m_iMtrlRef >= 0)
				{
					BMtrl* pMtrl = &m_Material[m_GeomObject[dwObject].m_iMtrlRef];
					int TexCnt = pMtrl->m_SubMaterial[iSub].m_TexMaps.size();
					pSubMesh->m_TextureName = pMtrl->m_SubMaterial[iSub].m_TexMaps[0].m_strTextureName;
					pSubMesh->m_NormalTextureName = pMtrl->m_SubMaterial[iSub].m_TexMaps[0].m_strNormalTextureName;
					if (TexCnt)
					{
						BMtrl* pSubMtrl = &pMtrl->m_SubMaterial[iSub];
						for (int iCnt = 0; iCnt < TexCnt; iCnt++)
						{
							pSubMesh->m_iDiffuseTex[iCnt] = pSubMtrl->m_TexMaps[iCnt].m_dwIndex;
						}
					}

					/*if (pMtrl->m_SubMaterial[iSub].m_TexMaps.size())
					{
					BMtrl* pSubMtrl = &pMtrl->m_SubMaterial[iSub];
					pSubMesh->m_iDiffuseTex = pSubMtrl->m_TexMaps[0].m_dwIndex;
					}*/
				}
			}
			//watch.output
			//애니메이션 트랙 복사
			/*포지션 트랙*/
			BAnimTrack* pPrevTrack = NULL;
			//첫번째 트랙은 이전 트랙이 없으므로 prev링크드리스트 구성을 NULL로 해주기 위함
			for (int iTrack = 0; iTrack < m_GeomObject[dwObject].m_PosTrack.size(); iTrack++)
			{
				auto pTrack = make_shared<BAnimTrack>();
				pTrack->iTick = m_GeomObject[dwObject].m_PosTrack[iTrack].iTick;
				pTrack->vVector = m_GeomObject[dwObject].m_PosTrack[iTrack].vVector;
				// 각 트랙을 돌면서 틱값과 백터의 값을 저장한다.
				pPrevTrack = SetDoublyLinkedList(pTrack.get(), pPrevTrack);
				// 각 트랙간의 링크드리스트 구성, pPrevTrack을 반환받는 이유는 다음 for문을 돌때 prevtrack으로 현재 트랙을 이용하기 때문에 (반환되는 값은 현재트랙)
				pMesh->m_pPosTrack.push_back(pTrack);
			}

			/*로테이션 트랙*/
			pPrevTrack = NULL;
			for (int iTrack = 0; iTrack < m_GeomObject[dwObject].m_RotTrack.size(); iTrack++)
			{
				auto pTrack = make_shared<BAnimTrack>();
				pTrack->iTick = m_GeomObject[dwObject].m_RotTrack[iTrack].iTick;

				//임의의 축과 각을 쿼터니언으로 변환
				//세타 / 2 가 아닌 이유는 D3DXQuaternionRotationAxis에서 받아온 w 값을 2로 나눠서 계산해주기 때문에 (편리하다)
				D3DXQuaternionRotationAxis(&pTrack->qRotate, &D3DXVECTOR3(m_GeomObject[dwObject].m_RotTrack[iTrack].qRotate.x,
					m_GeomObject[dwObject].m_RotTrack[iTrack].qRotate.y,
					m_GeomObject[dwObject].m_RotTrack[iTrack].qRotate.z),
					m_GeomObject[dwObject].m_RotTrack[iTrack].qRotate.w); // w는 세타값
																		  // 이전트랙의 쿼터니온과 누적
				if (pPrevTrack != NULL)
				{
					D3DXQuaternionMultiply(&pTrack->qRotate, &pPrevTrack->qRotate, &pTrack->qRotate);
				}
				pPrevTrack = SetDoublyLinkedList(pTrack.get(), pPrevTrack);
				pMesh->m_pRotTrack.push_back(pTrack);
			}


			/*스케일 트랙*/
			pPrevTrack = NULL;
			for (int iTrack = 0; iTrack < m_GeomObject[dwObject].m_SclTrack.size(); iTrack++)
			{
				auto pTrack = make_shared<BAnimTrack>();
				pTrack->iTick = m_GeomObject[dwObject].m_SclTrack[iTrack].iTick;
				pTrack->vVector = m_GeomObject[dwObject].m_SclTrack[iTrack].vVector;

				// 임의의 축과 각을 쿼터니언으로 변환
				D3DXQuaternionRotationAxis(&pTrack->qRotate, &D3DXVECTOR3(m_GeomObject[dwObject].m_SclTrack[iTrack].qRotate.x,
					m_GeomObject[dwObject].m_SclTrack[iTrack].qRotate.y,
					m_GeomObject[dwObject].m_SclTrack[iTrack].qRotate.z),
					m_GeomObject[dwObject].m_SclTrack[iTrack].qRotate.w);
				pPrevTrack = SetDoublyLinkedList(pTrack.get(), pPrevTrack);
				pMesh->m_pSclTrack.push_back(pTrack);
			}
		}
		m_pData.push_back(pAseMesh);
	}
	m_GeomObject.clear();
	m_Material.clear();

	/*애니메이션 첫 트랙이 없을경우 곱할 행렬을 만들어주기 위한 과정*/
	return InheriteCollect();
}
/*0프레임 애니메이션 생성 함수(Convert 내부 호출)*/
bool BAseObj::InheriteCollect()
{
	D3DXMATRIX m_matInverse;
	D3DXQUATERNION qR;
	D3DXVECTOR3 vTrans, vScale;
	for (DWORD dwObject = 0; dwObject < m_pData.size(); dwObject++)
	{
		if (!m_pData[dwObject]->m_strParentName.empty())
		{
			BMesh2* pParentNode = SearchToCollects(m_pData[dwObject]->m_strParentName);
			if (pParentNode)
			{
				m_pData[dwObject]->m_pParent = pParentNode;

				/*자식의 행렬을 저장*/
				m_matInverse = m_pData[dwObject]->m_matWorld * pParentNode->m_matInverse;
				D3DXMatrixDecompose(&vScale, &qR, &vTrans, &m_matInverse);
				D3DXMatrixScaling(&m_pData[dwObject]->m_matWorldScale, vScale.x, vScale.y, vScale.z);
				D3DXMatrixTranslation(&m_pData[dwObject]->m_matWorldTrans, vTrans.x, vTrans.y, vTrans.z);
				D3DXMatrixRotationQuaternion(&m_pData[dwObject]->m_matWorldRotate, &qR);

				pParentNode->m_pChildMesh.push_back(m_pData[dwObject].get());
			}
		}
	}
	// 미사용 오브젝트 삭제
	for (DWORD dwObject = 0; dwObject < m_pData.size(); dwObject++)
	{
		if (m_pData[dwObject]->m_pChildMesh.size() <= 0 && (m_pData[dwObject]->m_ClassType == CLASS_BONE || m_pData[dwObject]->m_ClassType == CLASS_DUMMY))
		{
			m_pData[dwObject]->m_bUsed = false;
		}
	}
	return true;
}
/*부모노드 검색함수 (InheriteCollect 내부 호출)*/
BMesh2* BAseObj::SearchToCollects(T_STR m_strParentName)
{
	for (DWORD dwGroup = 0; dwGroup < m_pData.size(); dwGroup++)
	{
		if (m_pData[dwGroup]->m_strNodeName == m_strParentName)
		{
			return m_pData[dwGroup].get();
		}
	}
	return NULL;
}
/*인덱스 최적화 함수 (서브메터리얼 미포함)*/
bool BAseObj::SetTriangleBuffer(BMesh2* pMesh, BGeomMesh* pAseMesh, DWORD dwSubMtrl, DWORD dwMask)
{
	D3DXMATRIX matInverse;
	D3DXMatrixInverse(&matInverse, NULL, &pAseMesh->m_matWorld);

	pMesh->m_FaceList.reserve(pAseMesh->m_iNumFace); // resize와 같으나 공간이 있을뿐 할당은 하지 않은상태 (push_back으로만 초기화가 가능하다)

	int index = 0;
	for (DWORD dwFace = 0; dwFace < pAseMesh->m_iNumFace; dwFace++)
	{
		PNCT_Vertex vVertex;
		DWORD dwMtrl = dwMask & pAseMesh->m_PosVertexList.pSubListMtl[dwFace];

		if (dwMtrl != dwSubMtrl) continue;

		for (DWORD dwVertex = 0; dwVertex < 3; dwVertex++)
		{
			if (pAseMesh->m_PosVertexList.dwNumVertex > 0 && pAseMesh->m_PosVertexList.dwNumFace > 0)
			{
				DWORD dwIndex = pAseMesh->m_PosVertexList.pFaceList[dwFace].v[dwVertex];
				vVertex.Position = pAseMesh->m_PosVertexList.pVertexList[dwIndex];
			}

			if (pAseMesh->m_pNorVertexList.size() > 0)
			{
				vVertex.Normal = pAseMesh->m_pNorVertexList[dwFace * 4 + dwVertex + 1];
			}

			/*로드부분 순서 조정해보고 변화 가능한지 확인할 것*/
			/* UV, Color 둘 다 */
			if (pAseMesh->m_ColVertexList.dwNumVertex && pAseMesh->m_ColVertexList.dwNumFace)
			{
				DWORD dwIndex = pAseMesh->m_ColVertexList.pFaceList[dwFace].v[dwVertex];
				vVertex.Color.x = pAseMesh->m_ColVertexList.pVertexList[dwIndex].x;
				vVertex.Color.z = pAseMesh->m_ColVertexList.pVertexList[dwIndex].y;
				vVertex.Color.y = pAseMesh->m_ColVertexList.pVertexList[dwIndex].z;
				vVertex.Color.w = 1.0f;
			}
			else
			{
				vVertex.Color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			}

			if (pAseMesh->m_TexVertexList.dwNumVertex && pAseMesh->m_TexVertexList.dwNumFace)
			{
				DWORD dwIndex = pAseMesh->m_TexVertexList.pFaceList[dwFace].v[dwVertex];
				vVertex.Texture.x = pAseMesh->m_TexVertexList.pVertexList[dwIndex].x;
				vVertex.Texture.y = 1.0f - pAseMesh->m_TexVertexList.pVertexList[dwIndex].z;
			}

			D3DXVec3TransformCoord(&vVertex.Position, &vVertex.Position, &matInverse);
			//D3DXVec3TransformCoord(&vVertex.Normal, &vVertex.Normal, &matInverse);
			pMesh->m_FaceList.push_back(vVertex);
		}
	}

	if (pMesh->m_FaceList.size() <= 0) return false;
	return true;
}
/*인덱스 최적화 함수 (서브메터리얼 포함)*/
bool BAseObj::SetTriangleSubBuffer(vector<shared_ptr<BMesh2>>& pSubMesh, BGeomMesh* pAseMesh)
{
	int Index = 0;
	D3DXMATRIX matInverse;
	D3DXMatrixInverse(&matInverse, NULL, &pAseMesh->m_matWorld);

	for (DWORD dwFace = 0; dwFace < pAseMesh->m_iNumFace; dwFace++)
	{
		PNCT_Vertex vVertex;
		DWORD dwMtrl = pAseMesh->m_PosVertexList.pSubListMtl[dwFace];
		// 255제거용
		if (dwMtrl > 30)
		{
			dwMtrl = 4;
		}

		for (DWORD dwVertex = 0; dwVertex < 3; dwVertex++)
		{
			if (pAseMesh->m_PosVertexList.dwNumVertex > 0 &&
				pAseMesh->m_PosVertexList.dwNumFace > 0)
			{
				DWORD dwIndex = pAseMesh->m_PosVertexList.pFaceList[dwFace].v[dwVertex];
				vVertex.Position = pAseMesh->m_PosVertexList.pVertexList[dwIndex];
			}

			if (pAseMesh->m_pNorVertexList.size() > 0)
			{
				// 4 = FaceNormal[0], V0-Normal[1], V0-Normal[1], V0-Normal[2]
				vVertex.Normal = pAseMesh->m_pNorVertexList[dwFace * 4 + dwVertex + 1];
			}

			if (pAseMesh->m_ColVertexList.dwNumVertex && pAseMesh->m_ColVertexList.dwNumFace)
			{
				// AseFile의 R G B => Load된 정보는 R B G로 저장되어 있다.
				DWORD dwIndex = pAseMesh->m_ColVertexList.pFaceList[dwFace].v[dwVertex];
				vVertex.Color.x = pAseMesh->m_ColVertexList.pVertexList[dwIndex].x;
				vVertex.Color.z = pAseMesh->m_ColVertexList.pVertexList[dwIndex].y;
				vVertex.Color.y = pAseMesh->m_ColVertexList.pVertexList[dwIndex].z;
				vVertex.Color.w = 1.0f;
			}
			else
			{
				vVertex.Color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			}

			if (pAseMesh->m_TexVertexList.dwNumVertex && pAseMesh->m_TexVertexList.dwNumFace)
			{
				// AseFile의 U V W => Load된 정보는 U W V로 저장되어 있다.
				DWORD dwIndex = pAseMesh->m_TexVertexList.pFaceList[dwFace].v[dwVertex];
				vVertex.Texture.x = pAseMesh->m_TexVertexList.pVertexList[dwIndex].x;
				vVertex.Texture.y = 1.0f - pAseMesh->m_TexVertexList.pVertexList[dwIndex].z;
			}

			// 로컬 정점 = 월드 정점 * 월드행렬의 역행렬
			D3DXVec3TransformCoord(&vVertex.Position, &vVertex.Position, &matInverse);
			//D3DXVec3TransformCoord(&vVertex.Normal, &vVertex.Normal, &matInverse);
			pSubMesh[dwMtrl]->m_FaceList.push_back(vVertex);
		}
	}
	return true;
}
#pragma endregion

#pragma region 매쉬 오브젝트 생성 관련 함수
/*재정의 # 매쉬 오브젝트 생성함수*/
HRESULT BAseObj::Create(ID3D11Device* pDevice)
{
	m_pd3dDevice = pDevice;

	HRESULT hr = S_OK;

	S_RETURN(CreateVertexBuffer());
	S_RETURN(CreateIndexBuffer());
	S_RETURN(TextureLoad(m_pd3dDevice));
	//S_RETURN(LoadShaderAndInputLayout(L"../../data/shader/AseShader.hlsl"));
	S_RETURN(CreateConstantBuffer());

	return hr;
}
/*재정의 # 매쉬 버텍스 생성함수*/
HRESULT BAseObj::CreateVertexBuffer()		// 버텍스 버퍼 생성
{
	HRESULT hr = S_OK;

	for (DWORD dwObject = 0; dwObject < m_pData.size(); dwObject++) // 매쉬의 개수만큼 돈다
	{
		BMesh2* pMesh = m_pData[dwObject].get();
		if (pMesh->m_pSubMesh.size() > 0)	// 서브매쉬가 존재한다면
		{
			for (DWORD dwSub = 0; dwSub < pMesh->m_pSubMesh.size(); dwSub++) // 서브매쉬 수만큼 생성
			{
				BMesh2* pSubMesh = pMesh->m_pSubMesh[dwSub].get();
				if (pSubMesh->m_VertexArray.size() < 3) continue;

				m_VertexCnt = pSubMesh->m_VertexArray.size();
				ID3D11Buffer* pBuffer = nullptr;
				D3D11_BUFFER_DESC bd;
				bd.Usage = D3D11_USAGE_DEFAULT;						// [메모리 할당 위치] GPU 메모리에 할당해라
				bd.ByteWidth = sizeof(PNCT_Vertex) * m_VertexCnt;	// [메모리 할당 크기] 점 X개를 할당 할 것이기 때문에 *X ( 수가 늘어나면 증가 할 것이다 )
				bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;			// [메모리 사용 용도] 버텍스 버퍼용으로 사용하겠다.
				bd.CPUAccessFlags = 0;								// CPU로 부터는 액세스 하지 않겠다.
				bd.MiscFlags = 0;									// 추가 설정 없음

				D3D11_SUBRESOURCE_DATA InitData;			// 소스 리소스 초기화에 필요한 데이터 지정
				InitData.pSysMem = (void*)&pSubMesh->m_VertexArray.at(0);				// 해당 버텍스를 소스로 사용하겠다.
				S_RETURN(m_pd3dDevice->CreateBuffer(&bd, &InitData, &pBuffer));

				pSubMesh->m_pVertexBuffer.Attach(pBuffer);
			}
		}
		else
		{
			if (pMesh->m_VertexArray.size() < 3)continue;

			m_VertexCnt = pMesh->m_VertexArray.size();
			ID3D11Buffer* pBuffer = nullptr;
			D3D11_BUFFER_DESC bd;
			bd.Usage = D3D11_USAGE_DEFAULT;						// [메모리 할당 위치] GPU 메모리에 할당해라
			bd.ByteWidth = sizeof(PNCT_Vertex) * m_VertexCnt;	// [메모리 할당 크기] 점 X개를 할당 할 것이기 때문에 *X ( 수가 늘어나면 증가 할 것이다 )
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;			// [메모리 사용 용도] 버텍스 버퍼용으로 사용하겠다.
			bd.CPUAccessFlags = 0;								// CPU로 부터는 액세스 하지 않겠다.
			bd.MiscFlags = 0;									// 추가 설정 없음

			D3D11_SUBRESOURCE_DATA InitData;			// 소스 리소스 초기화에 필요한 데이터 지정
			InitData.pSysMem = (void*)&pMesh->m_VertexArray.at(0);				// 해당 버텍스를 소스로 사용하겠다.
			m_pd3dDevice->CreateBuffer(&bd, &InitData, &pBuffer);

			pMesh->m_pVertexBuffer.Attach(pBuffer);
		}
	}
	return hr;
}
/*재정의 # 매쉬 인덱스 생성함수*/
HRESULT BAseObj::CreateIndexBuffer()		// 인덱스 버퍼 생성
{
	HRESULT hr = S_OK;

	for (DWORD dwObject = 0; dwObject < m_pData.size(); dwObject++)
	{
		BMesh2* pMesh = m_pData[dwObject].get();
		if (pMesh->m_pSubMesh.size() > 0)
		{
			for (DWORD dwSub = 0; dwSub < pMesh->m_pSubMesh.size(); dwSub++)
			{
				BMesh2* pSubMesh = pMesh->m_pSubMesh[dwSub].get();
				if (pSubMesh->m_VertexArray.size() < 3)continue;

				ID3D11Buffer* pBuffer = nullptr;
				UINT iNumIndex = pSubMesh->m_IndexArray.size();
				// 인덱스 버퍼 생성
				D3D11_BUFFER_DESC ibDesc;
				ibDesc.ByteWidth = iNumIndex * sizeof(DWORD);
				ibDesc.Usage = D3D11_USAGE_DEFAULT;
				ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER; // 인덱스 버퍼로 사용하겠다는 선언
				ibDesc.CPUAccessFlags = 0;
				ibDesc.MiscFlags = 0;

				D3D11_SUBRESOURCE_DATA ibInitData;
				ZeroMemory(&ibInitData, sizeof(D3D11_SUBRESOURCE_DATA));
				ibInitData.pSysMem = (void*)&pSubMesh->m_IndexArray.at(0);
				S_RETURN(m_pd3dDevice->CreateBuffer(&ibDesc, &ibInitData, &pBuffer));

				pSubMesh->m_pIndexBuffer.Attach(pBuffer);
			}
		}
		else
		{
			if (pMesh->m_VertexArray.size() < 3)continue;

			ID3D11Buffer* pBuffer = nullptr;
			UINT iNumIndex = pMesh->m_IndexArray.size();
			// 인덱스 버퍼 생성
			D3D11_BUFFER_DESC ibDesc;
			ibDesc.ByteWidth = iNumIndex * sizeof(DWORD);
			ibDesc.Usage = D3D11_USAGE_DEFAULT;
			ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER; // 인덱스 버퍼로 사용하겠다는 선언
			ibDesc.CPUAccessFlags = 0;
			ibDesc.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA ibInitData;
			ZeroMemory(&ibInitData, sizeof(D3D11_SUBRESOURCE_DATA));
			ibInitData.pSysMem = (void*)&pMesh->m_IndexArray.at(0);
			S_RETURN(m_pd3dDevice->CreateBuffer(&ibDesc, &ibInitData, &pBuffer));

			pMesh->m_pIndexBuffer.Attach(pBuffer);
		}
	}
	return hr;
}
/*재정의 # 매쉬 텍스쳐(쉐이더리소스) 생성함수*/
HRESULT BAseObj::TextureLoad(ID3D11Device* pDevice)
{
	HRESULT hr = S_OK;
	for (DWORD dwObject = 0; dwObject < m_pData.size(); dwObject++)
	{
		BMesh2* pMesh = m_pData[dwObject].get();
		if (pMesh->m_pSubMesh.size() > 0)
		{
			for (DWORD dwSub = 0; dwSub < pMesh->m_pSubMesh.size(); dwSub++)
			{
				BMesh2* pSubMesh = pMesh->m_pSubMesh[dwSub].get();
				if (pSubMesh->m_iDiffuseTex[0] < 0) continue;
				int iCnt = 0;
				pSubMesh->m_pTextureSRV.resize(10);
				while (pSubMesh->m_iDiffuseTex[iCnt] > -1) // 원래 0이었음
				{
					pSubMesh->m_pTextureSRV[iCnt] = m_pTextureSRV[pSubMesh->m_iDiffuseTex[iCnt]].Get();
					iCnt++;
				}
				if (m_pNormalTextureSRV[0].Get() != NULL)
				{
					pSubMesh->m_pNormalTextureSRV.resize(1);
					pSubMesh->m_pNormalTextureSRV[0] = m_pNormalTextureSRV[0].Get();
				}
			}
		}
		else
		{
			if (pMesh->m_iDiffuseTex[0] < 0) continue;
			int iCnt = 0;
			while (pMesh->m_iDiffuseTex[iCnt] > -1)
			{
				pMesh->m_pTextureSRV.resize(10);
				pMesh->m_pTextureSRV[iCnt] = m_pTextureSRV[pMesh->m_iDiffuseTex[iCnt]].Get();
				iCnt++;
			}
			if (m_pNormalTextureSRV[0].Get() != NULL)
			{
				pMesh->m_pNormalTextureSRV.resize(1);
				pMesh->m_pNormalTextureSRV[0] = m_pNormalTextureSRV[0].Get();
			}
		}
	}
	return hr;
}
#pragma endregion

#pragma region 랜더링 관련 함수
/*랜더링 카메라 세팅함수*/
bool BAseObj::SetCamera(D3DXMATRIX Camera_matView, D3DXMATRIX Camera_matProj)
{
	m_matView = Camera_matView;
	m_matProj = Camera_matProj;
	return true;
}
/*랜더링 행렬 세팅함수*/
void BAseObj::SetMatrix(D3DXMATRIX World, D3DXMATRIX View, D3DXMATRIX Proj)
{
	m_matWorld = World;
	m_matView = View;
	m_matProj = Proj;

	D3DXMATRIX InvWorldMat;
	D3DXMatrixInverse(&InvWorldMat, NULL, &World);

	D3DXMatrixTranspose(&m_matWorld, &m_matWorld);
	D3DXMatrixTranspose(&m_matView, &m_matView);
	D3DXMatrixTranspose(&m_matProj, &m_matProj);

	m_cbData.m_matWorld = m_matWorld;
	m_cbData.m_matView = m_matView;
	m_cbData.m_matProj = m_matProj;
	m_cbData.m_matNormal = InvWorldMat;
	m_cbData.f_Time = cosf(timeGetTime()*0.001f) * 0.5f + 0.5f;

	D3DXMatrixTranspose(&m_matWorld, &m_matWorld);
	D3DXMatrixTranspose(&m_matView, &m_matView);
	D3DXMatrixTranspose(&m_matProj, &m_matProj);
}
/*랜더링 인스턴싱 리스트 갱신함수*/
bool BAseObj::SetDrawObj(vector<int> DrawList)
{
	if (DrawList.size() >0)
	{
		m_DrawInstanceList = DrawList;
		return true;
	}
	return false; // 인스턴싱이 0인경우
}
/*전체 매쉬 랜더함수*/
bool BAseObj::Draw(ID3D11DeviceContext* pContext, BAseObj* pParent)
{
	for (DWORD dwObject = 0; dwObject < m_pData.size(); dwObject++)
	{
		auto pMesh = m_pData[dwObject].get();

		pParent->SetMatrix(pMesh->m_matCalculation, pParent->m_matView, pParent->m_matProj);

		if (pParent != NULL)
		{
			pContext->UpdateSubresource(pParent->m_pConstantBuffer, 0, NULL, &m_cbData, 0, 0);
		}
		else
		{
			pContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &m_cbData, 0, 0);
		}

		if (pMesh->m_pSubMesh.size() > 0)
		{
			for (DWORD dwSub = 0; dwSub < pMesh->m_pSubMesh.size(); dwSub++)
			{
				auto pSubMesh = pMesh->m_pSubMesh[dwSub].get();
				if (pSubMesh->m_iNumFace < 1)
				{
					continue;
				}
				UINT stride = sizeof(PNCT_Vertex);
				UINT offset = 0;
				pContext->IASetVertexBuffers(0, 1, pSubMesh->m_pVertexBuffer.GetAddressOf(), &stride, &offset);
				pContext->IASetIndexBuffer(pSubMesh->m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
				pContext->PSSetShaderResources(0, 10, pSubMesh->m_pTextureSRV[0].GetAddressOf());
				pContext->DrawIndexed(pSubMesh->m_IndexArray.size(), 0, 0);
			}
		}
		else
		{
			if (pMesh->m_iNumFace < 1)
			{
				continue;
			}
			UINT stride = sizeof(PNCT_Vertex);
			UINT offset = 0;
			pContext->IASetVertexBuffers(0, 1, pMesh->m_pVertexBuffer.GetAddressOf(), &stride, &offset);
			pContext->IASetIndexBuffer(pMesh->m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
			pContext->PSSetShaderResources(0, 10, pMesh->m_pTextureSRV[0].GetAddressOf());
			pContext->DrawIndexed(pMesh->m_IndexArray.size(), 0, 0);
		}
	}
	return true;
}
/*부분 매쉬 랜더함수*/
void BAseObj::DrawMesh(ID3D11DeviceContext* pContext, BMesh2* pMesh)
{

	UINT stride = sizeof(PNCT_Vertex);
	UINT offset = 0;
	SetMatrix(pMesh->m_matWorld, m_matView, m_matProj); // 카메라 넘겨줘야 함 
	pContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &m_cbData, 0, 0);

	pContext->IASetInputLayout(m_pVertexLayout);
	pContext->IASetVertexBuffers(0, 1, pMesh->m_pVertexBuffer.GetAddressOf(), &stride, &offset);
	pContext->IASetIndexBuffer(pMesh->m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	int iCnt = 0;
	while (pMesh->m_iDiffuseTex[iCnt] > 0)
	{
		pContext->PSSetShaderResources(0, 1, pMesh->m_pTextureSRV[iCnt].GetAddressOf());
		iCnt++;
	}
	//pContext->PSSetShaderResources(0, 1, pMesh->m_pTextureSRV.GetAddressOf());
	pContext->DrawIndexed(pMesh->m_IndexArray.size(), 0, 0);
}
/*인스턴싱 매쉬 랜더함수*/
bool BAseObj::DrawInstance(ID3D11DeviceContext* pContext, BAseObj* pParent, int DrawCount)
{
	// 매쉬 & 서브매쉬 랜더링 작업
	for (DWORD dwObject = 0; dwObject < m_pData.size(); dwObject++)
	{
		auto pMesh = m_pData[dwObject].get();

		ID3D11ShaderResourceView* pClearSRV = 0;
		pContext->PSSetShaderResources(2, 1, &pClearSRV);

		pParent->SetMatrix(pMesh->m_matCalculation, pParent->m_matView, pParent->m_matProj);
		if (pParent != NULL)
		{
			pContext->UpdateSubresource(pParent->m_pConstantBuffer, 0, NULL, &m_cbData, 0, 0);
		}
		else
		{
			pContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &m_cbData, 0, 0);
		}

		if (pMesh->m_pSubMesh.size() > 0)
		{
			for (DWORD dwSub = 0; dwSub < pMesh->m_pSubMesh.size(); dwSub++)
			{
				auto pSubMesh = pMesh->m_pSubMesh[dwSub].get();
				if (pSubMesh->m_iNumFace < 1)
				{
					continue;
				}
				UpdateInstanceInfo(pContext, m_InstanceInfo, pMesh);

				// 노말맵핑용 상수버퍼 세팅
				for (int iInsCnt = 0; iInsCnt < m_pInvInstaneList.size(); iInsCnt++)
				{
					m_cbNormal.g_matNormal[iInsCnt] = m_pInvInstaneList[iInsCnt];
				}
				m_cbNormal.cb_vLightVector = D3DXVECTOR3(128, 80, -128);
				pContext->UpdateSubresource(m_pNormalCB, 0, NULL, &m_cbNormal, 0, 0);

				ID3D11Buffer* Buffer[3] = { pSubMesh->m_pVertexBuffer.Get(), m_BufferInstance.Get(), pSubMesh->m_pTangentBuffer.Get() };
				UINT stride[3] = { sizeof(PNCT_Vertex), sizeof(D3DXMATRIX), sizeof(D3DXVECTOR3) };
				UINT offset[3] = { 0, 0, 0 };
				pContext->IASetVertexBuffers(0, 3, Buffer, stride, offset);
				pContext->IASetIndexBuffer(pSubMesh->m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
				pContext->PSSetShaderResources(0, 1, pSubMesh->m_pTextureSRV[0].GetAddressOf());
				if (pSubMesh->m_pNormalTextureSRV.size() > 0)
				{
					pContext->PSSetShaderResources(2, 1, pSubMesh->m_pNormalTextureSRV[0].GetAddressOf());
				}
				UpdateBoundingBox();
				pContext->DrawIndexedInstanced(pSubMesh->m_IndexArray.size(), DrawCount, 0, 0, 0);
			}
		}
		else
		{
			if (pMesh->m_iNumFace < 1)
			{
				continue;
			}
			ID3D11Buffer* Buffer[3] = { pMesh->m_pVertexBuffer.Get(), m_BufferInstance.Get(), pMesh->m_pTangentBuffer.Get() };
			UINT stride[3] = { sizeof(PNCT_Vertex), sizeof(D3DXMATRIX), sizeof(D3DXVECTOR3) };
			UINT offset[3] = { 0, 0, 0 };
			pContext->IASetVertexBuffers(0, 3, Buffer, stride, offset);
			pContext->IASetIndexBuffer(pMesh->m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
			pContext->PSSetShaderResources(0, 1, pMesh->m_pTextureSRV[0].GetAddressOf());
			if (pMesh->m_pNormalTextureSRV.size() > 0)
			{
				pContext->PSSetShaderResources(2, 1, pMesh->m_pNormalTextureSRV[0].GetAddressOf());
			}
			UpdateInstanceInfo(pContext, m_InstanceInfo, pMesh);

			// 노말맵핑용 상수버퍼 세팅
			for (int iInsCnt = 0; iInsCnt < m_pInvInstaneList.size(); iInsCnt++)
			{
				m_cbNormal.g_matNormal[iInsCnt] = m_pInvInstaneList[iInsCnt];
			}
			m_cbNormal.cb_vLightVector = D3DXVECTOR3(128, 80, -128);
			pContext->UpdateSubresource(m_pNormalCB, 0, NULL, &m_cbNormal, 0, 0);

			UpdateBoundingBox();
			pContext->DrawIndexedInstanced(pMesh->m_IndexArray.size(), DrawCount, 0, 0, 0);
		}
	}
	return true;
}
#pragma endregion

#pragma region 텍스쳐 로드 관련 함수
/*텍스쳐 로드 함수(단일)*/
HRESULT BAseObj::TextureLoad(ID3D11Device* pDevice, TCHAR* strFilePath)
{
	HRESULT hr = S_OK;
	m_pTextureSRV.resize(m_iCurIndex + 1);
	if (FAILED(hr = D3DX11CreateShaderResourceViewFromFile(pDevice, strFilePath, NULL, NULL, m_pTextureSRV[m_iCurIndex].GetAddressOf(), NULL)))
	{
		return hr;
	}
	m_iCurIndex++;
	return hr;
}
/*텍스쳐 로드 함수(다중)*/
HRESULT BAseObj::TextureLoad(ID3D11Device* pDevice, TCHAR* strFilePathes[], int strCount)
{
	HRESULT hr = S_OK;

	for (int iTex = 0; iTex < strCount; iTex++)
	{
		m_pTextureSRV.resize(strCount);
		if (FAILED(hr = D3DX11CreateShaderResourceViewFromFile(pDevice, strFilePathes[iTex], NULL, NULL, m_pTextureSRV[iTex].GetAddressOf(), NULL)))
		{
			return hr;
		}
		m_iCurIndex++;
	}
	return hr;
}
/*노말맵핑 텍스쳐 로드 함수*/
void BAseObj::NormalTextureLoad(ID3D11Device* pDevice, TCHAR* strFilePath)
{
	m_pNormalTextureSRV.resize(1);
	D3DX11CreateShaderResourceViewFromFile(pDevice, strFilePath, NULL, NULL, m_pNormalTextureSRV[0].GetAddressOf(), NULL);
}
#pragma endregion

#pragma region 애니메이션 관련 함수
/*애니메이션 프레임 링크생성 함수(더블 링크드리스트)*/
BAnimTrack* BAseObj::SetDoublyLinkedList(BAnimTrack* pCurrentTrack, BAnimTrack* pPrev)
{
	if (pPrev)
	{
		pPrev->pNext = pCurrentTrack;
		pCurrentTrack->pPrev = pPrev;
	}
	return pCurrentTrack;
}
/*애니메이션 프레임 기반 최종행렬 생성(인터폴레이션)함수*/
void BAseObj::Interpolate(BMesh2* pMesh, D3DXMATRIX* matParent, float fFrameTick)
{
	//TM = AnimMat * ParentTM;
	//AaniMat = TM * Inverse(ParentTM)
	D3DXQUATERNION qR, qS;
	D3DXMatrixIdentity(&pMesh->m_matCalculation);
	D3DXMATRIX matAnim, matPos, matRotate, matScale;

	matRotate = pMesh->m_matWorldRotate;
	matPos = pMesh->m_matWorldTrans;
	matScale = pMesh->m_matWorldScale;

	D3DXQuaternionRotationMatrix(&qR, &matRotate); //사원수 생성 (0번째가 없을 경우를 대비)
	D3DXQuaternionRotationMatrix(&qS, &matScale);

	float fStartTick = m_Scene.iFirstFrame * m_Scene.iTickPerFrame;
	float fEndTick = 0.0f;

	/*회전행렬 생성*/
	BAnimTrack* pStartTrack = NULL;
	BAnimTrack* pEndTrack = NULL;
	if (pMesh->m_pRotTrack.size())
	{
		// pStartTrack을 찾을 수 있으면
		if (GetAnimationTrack(fFrameTick, pMesh->m_pRotTrack, &pStartTrack, &pEndTrack))
		{
			qR = pStartTrack->qRotate;
			fStartTick = pStartTrack->iTick;
		}
		if (pEndTrack)
		{
			fEndTick = pEndTrack->iTick;
			D3DXQuaternionSlerp(&qR, &qR, &pEndTrack->qRotate, (fFrameTick - fStartTick) / (fEndTick - fStartTick));
		}
		D3DXMatrixRotationQuaternion(&matRotate, &qR);
	}

	/*이동행렬 생성*/
	pStartTrack = NULL;
	pEndTrack = NULL;

	D3DXVECTOR3 Trans(matPos._41, matPos._42, matPos._43);
	if (pMesh->m_pPosTrack.size())
	{
		if (GetAnimationTrack(fFrameTick, pMesh->m_pPosTrack, &pStartTrack, &pEndTrack))
		{
			Trans = pStartTrack->vVector; // Trans로 시작 움직임값을 받아와서
			fStartTick = pStartTrack->iTick;
		}
		if (pEndTrack)
		{
			fEndTick = pEndTrack->iTick;
			D3DXVec3Lerp(&Trans, &Trans, &pEndTrack->vVector, (fFrameTick - fStartTick) / (fEndTick - fStartTick)); // trans 값과 EndTrack의 움직임값을 보간한다.
		}
		D3DXMatrixTranslation(&matPos, Trans.x, Trans.y, Trans.z);	// 보간으로 나온 trans의 이동값을 이동행렬로 변환한다.
	}

	/*크기행렬 생성*/
	pStartTrack = NULL;
	pEndTrack = NULL;

	D3DXMATRIX matScaleRot, matInvScaleRot;
	D3DXVECTOR3 vScale(matScale._11, matScale._22, matScale._33);
	if (pMesh->m_pSclTrack.size())
	{
		// pStartTrack을 찾을 수 있으면
		if (GetAnimationTrack(fFrameTick, pMesh->m_pSclTrack, &pStartTrack, &pEndTrack))
		{
			vScale = pStartTrack->vVector;
			qS = pStartTrack->qRotate;
			fStartTick = pStartTrack->iTick;
		}
		if (pEndTrack)
		{
			fEndTick = pEndTrack->iTick;
			D3DXVec3Lerp(&vScale, &vScale, &pEndTrack->vVector, (fFrameTick - fStartTick) / (fEndTick - fStartTick));
			D3DXQuaternionSlerp(&qS, &qS, &pEndTrack->qRotate, (fFrameTick - fStartTick) / (fEndTick - fStartTick));
		}
		D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z);
		D3DXMatrixRotationQuaternion(&matScaleRot, &qS);
		D3DXMatrixInverse(&matInvScaleRot, NULL, &matScaleRot);
		matScale = matInvScaleRot * matScale * matScaleRot;
	}

	/*알파값 미구현 P087 참조*/

	D3DXMatrixIdentity(&matAnim);
	D3DXMatrixMultiply(&matAnim, &matScale, &matRotate);
	matAnim._41 = matPos._41;
	matAnim._42 = matPos._42;
	matAnim._43 = matPos._43;

	// 최종 애니메이션 행렬 생성
	D3DXMatrixMultiply(&pMesh->m_matCalculation, &matAnim, matParent);

	// 최종 행렬의 반전확인
	D3DXVECTOR3 v0, v1, v2, v3;
	v0 = pMesh->m_matCalculation.m[0];
	v1 = pMesh->m_matCalculation.m[1];
	v2 = pMesh->m_matCalculation.m[2];
	D3DXVec3Cross(&v3, &v1, &v2);
	if (D3DXVec3Dot(&v3, &v0) < 0.0f)
	{
		D3DXMATRIX matW;
		D3DXMatrixScaling(&matW, -1.0f, -1.0f, -1.0f);
		D3DXMatrixMultiply(&pMesh->m_matCalculation,
			&pMesh->m_matCalculation, &matW);
	}
}
/*애니메이션 트랙 반환 함수*/
//pEndTrack이 없으면 flase를 리턴 (보간할 대상이 없을 경우)
bool BAseObj::GetAnimationTrack(float fFrame, vector<shared_ptr<BAnimTrack>> pTrackList, BAnimTrack** pStartTrack, BAnimTrack** pEndTrack)
{
	for (DWORD dwTrack = 0; dwTrack < pTrackList.size(); dwTrack++)
	{
		BAnimTrack *pTrack = pTrackList[dwTrack].get();
		// fFrame보다 큰 Tick트랙이 있으면 이전 트랙을 넘겨주어야 하기 때문에 break 한다.
		if (pTrack->iTick > fFrame)
		{
			*pEndTrack = pTrack;
			break;
		}
		// 현재 프레임보다 큰 값이 없다면, 가장 시간경과가 오래된 것을 사용한다.
		*pStartTrack = pTrack;
	}
	return (*pStartTrack != NULL) ? true : false;
}
#pragma endregion

#pragma region 인스턴싱 관련 함수
/*재정의 # 인스턴싱 전용 쉐이더 레이아웃 생성함수*/
HRESULT BAseObj::LoadShaderAndInputLayout(const TCHAR* pShaderFile)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS; // 쉐이더 디버그 허용
	ID3DBlob* pVSBUF = NULL;
	ID3DBlob* pPSBUF = NULL;
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

	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",	  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTURE",  0, DXGI_FORMAT_R32G32_FLOAT,       0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "mTransform", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA,     1 },
		{ "mTransform", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA,    1 },
		{ "mTransform", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA,    1 },
		{ "mTransform", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA,    1 },

		{ "TANGENT",  0, DXGI_FORMAT_R32G32_FLOAT,      2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	UINT iSize = sizeof(layout) / sizeof(layout[0]);

	S_RETURN(m_pd3dDevice->CreateInputLayout(layout, iSize, pVSBUF->GetBufferPointer(), pVSBUF->GetBufferSize(), &m_pVertexLayout)); // m_pVertexLayout 제작

	S_RELEASE(pVSBUF);
	S_RELEASE(pPSBUF);
	S_RELEASE(pGSBUF);
	return hr;
}
/*인스턴스 버퍼, 정보 생성함수*/
void BAseObj::InitInstanceBuffer(int iInstanceCount, int ObjNum)
{
	if (iInstanceCount < 1)
	{
		return;
	}
	m_iInstanceCount = iInstanceCount;
	m_pInstanceList.resize(iInstanceCount);
	m_pInvInstaneList.resize(iInstanceCount);
	m_tBoundingBox.resize(iInstanceCount);
	m_InstanceInfo.resize(iInstanceCount);
	for (int iCount = 0; iCount < iInstanceCount; iCount++)
	{
		m_InstanceInfo[iCount].iInstanceNumber = iCount;
		m_InstanceInfo[iCount].ObjName = ObjNum;
	}
	CreateInstanceBuffer(iInstanceCount, sizeof(D3DXMATRIX), m_pInstanceList);
}
/*인스턴스 버퍼 생성 함수 (InitInstanceBuffer 내부 호출)*/
HRESULT BAseObj::CreateInstanceBuffer(UINT ListSize, UINT InstnaceSize, vector<D3DXMATRIX> m_pInstance)
{
	HRESULT hr = S_OK;
	ID3D11Buffer* pBuffer = nullptr;
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = ListSize * InstnaceSize;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = (void*)&m_pInstance.at(0);
	hr = m_pd3dDevice->CreateBuffer(&bd, &InitData, &pBuffer);

	m_BufferInstance.Attach(pBuffer);
	return hr;
}
/*인스턴스 정보 갱신함수*/
void BAseObj::UpdateInstanceInfo(ID3D11DeviceContext* pContext, vector<InstancingInfo> InstanceInfo, BMesh2* MeshData)
{
	m_pDrawInstanceList.clear();
	if (InstanceInfo.size() == 0)
	{
		m_pInstanceList.clear();
		return;
	}
	for (int iNum = 0; iNum < m_pInstanceList.size(); iNum++)
	{
		m_pInstanceList[iNum] = MeshData->m_matCalculation;
		D3DXMATRIX Scaletemp;
		D3DXMATRIX Rotationtemp;
		D3DXMATRIX TranslateTemp;
		D3DXMatrixIdentity(&Scaletemp);
		D3DXMatrixIdentity(&Rotationtemp);
		D3DXMatrixIdentity(&TranslateTemp);
		if (InstanceInfo[iNum].Scale.x == 0)
			InstanceInfo[iNum].Scale.x = 1;
		if (InstanceInfo[iNum].Scale.y == 0)
			InstanceInfo[iNum].Scale.y = 1;
		if (InstanceInfo[iNum].Scale.z == 0)
			InstanceInfo[iNum].Scale.z = 1;
		D3DXMatrixScaling(&Scaletemp, InstanceInfo[iNum].Scale.x, InstanceInfo[iNum].Scale.y, InstanceInfo[iNum].Scale.z);
		for (int iRot = 0; iRot < 3; iRot++)
		{
			if (InstanceInfo[iNum].Angle[iRot] != 0)
			{
				D3DXMatrixRotationAxis(&Rotationtemp, &InstanceInfo[iNum].RotationAxis[iRot], InstanceInfo[iNum].Angle[iRot] * 0.017453f);
			}
		}
		TranslateTemp._41 = InstanceInfo[iNum].Translate.x;
		TranslateTemp._42 = InstanceInfo[iNum].Translate.y;
		TranslateTemp._43 = InstanceInfo[iNum].Translate.z;

		D3DXMatrixMultiply(&m_pInstanceList[iNum], &m_pInstanceList[iNum], &Scaletemp);
		D3DXMatrixMultiply(&m_pInstanceList[iNum], &m_pInstanceList[iNum], &Rotationtemp);
		D3DXMatrixMultiply(&m_pInstanceList[iNum], &m_pInstanceList[iNum], &TranslateTemp);

		D3DXMatrixInverse(&m_pInvInstaneList[iNum], 0, &m_pInstanceList[iNum]);
		//D3DXMatrixTranspose(&m_pInvInstaneList[iNum], &m_pInvInstaneList[iNum]);
		D3DXMatrixTranspose(&m_pInstanceList[iNum], &m_pInstanceList[iNum]);
	}

	//for (int iObj = 0; iObj < m_DrawInstanceList.size(); iObj++)
	//{
	//	m_pDrawInstanceList.push_back(m_pInstanceList[m_DrawInstanceList[iObj]]);
	//}
	//if (m_DrawInstanceList.size() > 0)
	//{
	//	CreateInstanceBuffer(m_pDrawInstanceList.size(), sizeof(D3DXMATRIX), m_pDrawInstanceList);
	//}
	pContext->UpdateSubresource(m_BufferInstance.Get(), 0, NULL, &m_pInstanceList.at(0), 0, 0);
}
#pragma endregion

#pragma region 충돌 관련 함수
/*충돌박스 생성 함수*/
bool BAseObj::CreateBox()
{
	m_tBox.vMax = D3DXVECTOR3(-99999.0f, -99999.0f, -99999.0f);
	m_tBox.vMin = D3DXVECTOR3(99999.0f, 99999.0f, 99999.0f);

	for (int iMesh = 0; iMesh < m_pData.size(); iMesh++)
	{
		if (m_pData[iMesh]->m_pSubMesh.size() > 0) // 서브메터리얼이 존재할 경우
		{
			for (int iSub = 0; iSub < m_pData[iMesh]->m_pSubMesh.size(); iSub++)
			{
				//m_pData[iMesh]->m_pSubMesh[]
				if (m_pData[iMesh]->m_pSubMesh[iSub]->m_iNumFace < 1)
				{
					continue;
				}
				if (m_pData[iMesh]->m_pSubMesh[iSub]->m_tBox.vMax.x > m_tBox.vMax.x)
				{
					m_tBox.vMax.x = m_pData[iMesh]->m_pSubMesh[iSub]->m_tBox.vMax.x;
				}
				if (m_pData[iMesh]->m_pSubMesh[iSub]->m_tBox.vMax.y > m_tBox.vMax.y)
				{
					m_tBox.vMax.y = m_pData[iMesh]->m_pSubMesh[iSub]->m_tBox.vMax.y;
				}
				if (m_pData[iMesh]->m_pSubMesh[iSub]->m_tBox.vMax.z > m_tBox.vMax.z)
				{
					m_tBox.vMax.z = m_pData[iMesh]->m_pSubMesh[iSub]->m_tBox.vMax.z;
				}

				if (m_pData[iMesh]->m_pSubMesh[iSub]->m_tBox.vMin.x < m_tBox.vMin.x)
				{
					m_tBox.vMin.x = m_pData[iMesh]->m_pSubMesh[iSub]->m_tBox.vMin.x;
				}
				if (m_pData[iMesh]->m_pSubMesh[iSub]->m_tBox.vMin.y < m_tBox.vMin.y)
				{
					m_tBox.vMin.y = m_pData[iMesh]->m_pSubMesh[iSub]->m_tBox.vMin.y;
				}
				if (m_pData[iMesh]->m_pSubMesh[iSub]->m_tBox.vMin.z < m_tBox.vMin.z)
				{
					m_tBox.vMin.z = m_pData[iMesh]->m_pSubMesh[iSub]->m_tBox.vMin.z;
				}
				m_tBox.vCenter = (m_tBox.vMax + m_tBox.vMin) / 2;
				m_tBox.fExtent[0] = m_tBox.vMax.x - m_tBox.vCenter.x;
				m_tBox.fExtent[1] = m_tBox.vMax.y - m_tBox.vCenter.y;
				m_tBox.fExtent[2] = m_tBox.vMax.z - m_tBox.vCenter.z;
				m_tBox.vAxis[0] = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
				m_tBox.vAxis[1] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				m_tBox.vAxis[2] = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

				//  1   2
				//0   3
				//  5   6
				//4   7
				m_tBox.vPos[0] = D3DXVECTOR3(m_tBox.vMin.x, m_tBox.vMax.y, m_tBox.vMin.z);
				m_tBox.vPos[1] = D3DXVECTOR3(m_tBox.vMin.x, m_tBox.vMax.y, m_tBox.vMax.z);
				m_tBox.vPos[2] = D3DXVECTOR3(m_tBox.vMax.x, m_tBox.vMax.y, m_tBox.vMax.z);
				m_tBox.vPos[3] = D3DXVECTOR3(m_tBox.vMax.x, m_tBox.vMax.y, m_tBox.vMin.z);

				m_tBox.vPos[4] = D3DXVECTOR3(m_tBox.vMin.x, m_tBox.vMin.y, m_tBox.vMin.z);
				m_tBox.vPos[5] = D3DXVECTOR3(m_tBox.vMin.x, m_tBox.vMin.y, m_tBox.vMax.z);
				m_tBox.vPos[6] = D3DXVECTOR3(m_tBox.vMax.x, m_tBox.vMin.y, m_tBox.vMax.z);
				m_tBox.vPos[7] = D3DXVECTOR3(m_tBox.vMax.x, m_tBox.vMin.y, m_tBox.vMin.z);
			}
		}

		else // 없을경우
		{
			if (m_pData[iMesh]->m_iNumFace < 1)
			{
				continue;
			}
			if (m_pData[iMesh]->m_tBox.vMax.x > m_tBox.vMax.x)
			{
				m_tBox.vMax.x = m_pData[iMesh]->m_tBox.vMax.x;
			}
			if (m_pData[iMesh]->m_tBox.vMax.y > m_tBox.vMax.y)
			{
				m_tBox.vMax.y = m_pData[iMesh]->m_tBox.vMax.y;
			}
			if (m_pData[iMesh]->m_tBox.vMax.z > m_tBox.vMax.z)
			{
				m_tBox.vMax.z = m_pData[iMesh]->m_tBox.vMax.z;
			}

			if (m_pData[iMesh]->m_tBox.vMin.x < m_tBox.vMin.x)
			{
				m_tBox.vMin.x = m_pData[iMesh]->m_tBox.vMin.x;
			}
			if (m_pData[iMesh]->m_tBox.vMin.y < m_tBox.vMin.y)
			{
				m_tBox.vMin.y = m_pData[iMesh]->m_tBox.vMin.y;
			}
			if (m_pData[iMesh]->m_tBox.vMin.z < m_tBox.vMin.z)
			{
				m_tBox.vMin.z = m_pData[iMesh]->m_tBox.vMin.z;
			}
			m_tBox.vCenter = (m_tBox.vMax + m_tBox.vMin) / 2;
			m_tBox.fExtent[0] = m_tBox.vMax.x - m_tBox.vCenter.x;
			m_tBox.fExtent[1] = m_tBox.vMax.y - m_tBox.vCenter.y;
			m_tBox.fExtent[2] = m_tBox.vMax.z - m_tBox.vCenter.z;
			m_tBox.vAxis[0] = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
			m_tBox.vAxis[1] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			m_tBox.vAxis[2] = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

			//  1   2
			//0   3
			//  5   6
			//4   7
			m_tBox.vPos[0] = D3DXVECTOR3(m_tBox.vMin.x, m_tBox.vMax.y, m_tBox.vMin.z);
			m_tBox.vPos[1] = D3DXVECTOR3(m_tBox.vMin.x, m_tBox.vMax.y, m_tBox.vMax.z);
			m_tBox.vPos[2] = D3DXVECTOR3(m_tBox.vMax.x, m_tBox.vMax.y, m_tBox.vMax.z);
			m_tBox.vPos[3] = D3DXVECTOR3(m_tBox.vMax.x, m_tBox.vMax.y, m_tBox.vMin.z);

			m_tBox.vPos[4] = D3DXVECTOR3(m_tBox.vMin.x, m_tBox.vMin.y, m_tBox.vMin.z);
			m_tBox.vPos[5] = D3DXVECTOR3(m_tBox.vMin.x, m_tBox.vMin.y, m_tBox.vMax.z);
			m_tBox.vPos[6] = D3DXVECTOR3(m_tBox.vMax.x, m_tBox.vMin.y, m_tBox.vMax.z);
			m_tBox.vPos[7] = D3DXVECTOR3(m_tBox.vMax.x, m_tBox.vMin.y, m_tBox.vMin.z);
		}
	}

	m_BoxLine.resize(12);
	for (int iLine = 0; iLine < m_BoxLine.size(); iLine++)
	{
		m_BoxLine[iLine].Create(m_pd3dDevice, L"../../data/shader/LineShader.hlsl");
	}

	return true;
}
/*충돌박스 정보 갱신함수*/
void BAseObj::UpdateBoundingBox()
{
	for (int iNum = 0; iNum < m_pInstanceList.size(); iNum++)
	{
		m_tBoundingBox[iNum] = m_tBox;
		D3DXMATRIX Temp;
		Temp = m_pInstanceList[iNum];
		D3DXMatrixTranspose(&Temp, &Temp);
		for (int iPos = 0; iPos < 8; iPos++)
		{
			D3DXVec3TransformCoord(&m_tBoundingBox[iNum].vPos[iPos], &m_tBoundingBox[iNum].vPos[iPos], &Temp);// &m_pInstanceList[iNum]);
		}
		D3DXVec3TransformCoord(&m_tBoundingBox[iNum].vCenter, &m_tBoundingBox[iNum].vCenter, &Temp);
		D3DXVec3TransformCoord(&m_tBoundingBox[iNum].vMax, &m_tBoundingBox[iNum].vMax, &Temp);
		D3DXVec3TransformCoord(&m_tBoundingBox[iNum].vMin, &m_tBoundingBox[iNum].vMin, &Temp);
		m_tBoundingBox[iNum].fExtent[0] = m_tBoundingBox[iNum].vMax.x - m_tBoundingBox[iNum].vCenter.x;
		m_tBoundingBox[iNum].fExtent[1] = m_tBoundingBox[iNum].vMax.y - m_tBoundingBox[iNum].vCenter.y;
		m_tBoundingBox[iNum].fExtent[2] = m_tBoundingBox[iNum].vMax.z - m_tBoundingBox[iNum].vCenter.z;
		D3DXVec3TransformNormal(&m_tBoundingBox[iNum].vAxis[0], &m_tBoundingBox[iNum].vAxis[0], &Temp);
		D3DXVec3TransformNormal(&m_tBoundingBox[iNum].vAxis[1], &m_tBoundingBox[iNum].vAxis[1], &Temp);
		D3DXVec3TransformNormal(&m_tBoundingBox[iNum].vAxis[2], &m_tBoundingBox[iNum].vAxis[2], &Temp);
		D3DXVec3Normalize(&m_tBoundingBox[iNum].vAxis[0], &m_tBoundingBox[iNum].vAxis[0]);
		D3DXVec3Normalize(&m_tBoundingBox[iNum].vAxis[1], &m_tBoundingBox[iNum].vAxis[1]);
		D3DXVec3Normalize(&m_tBoundingBox[iNum].vAxis[2], &m_tBoundingBox[iNum].vAxis[2]);
	}
}
/*충돌박스 랜더링 함수*/
bool BAseObj::RenderBoundingBoxLine(ID3D11DeviceContext* m_pImmediateContext, D3DXMATRIX Camera_matView, D3DXMATRIX Camera_matProj)
{
	for (int iInstance = 0; iInstance < m_InstanceInfo.size(); iInstance++)
	{
		D3DXVECTOR4 LineColor = D3DXVECTOR4(1, 0, 0, 1);
		D3DXMATRIX Temp;
		Temp = m_pInstanceList[iInstance];
		D3DXMatrixTranspose(&Temp, &Temp);
		for (int iLine = 0; iLine < m_BoxLine.size(); iLine++)
		{
			m_BoxLine[iLine].SetMatrix(Temp, Camera_matView, Camera_matProj);
		}
		if (m_InstanceInfo[iInstance].Selected)
		{
			LineColor = D3DXVECTOR4(0, 0, 1, 1);
		}
		m_BoxLine[0].SetLineStartToEnd(m_pImmediateContext, m_tBox.vPos[0], m_tBox.vPos[1], LineColor);
		m_BoxLine[1].SetLineStartToEnd(m_pImmediateContext, m_tBox.vPos[1], m_tBox.vPos[2], LineColor);
		m_BoxLine[2].SetLineStartToEnd(m_pImmediateContext, m_tBox.vPos[2], m_tBox.vPos[3], LineColor);
		m_BoxLine[3].SetLineStartToEnd(m_pImmediateContext, m_tBox.vPos[0], m_tBox.vPos[3], LineColor);
		m_BoxLine[4].SetLineStartToEnd(m_pImmediateContext, m_tBox.vPos[0], m_tBox.vPos[4], LineColor);
		m_BoxLine[5].SetLineStartToEnd(m_pImmediateContext, m_tBox.vPos[1], m_tBox.vPos[5], LineColor);
		m_BoxLine[6].SetLineStartToEnd(m_pImmediateContext, m_tBox.vPos[2], m_tBox.vPos[6], LineColor);
		m_BoxLine[7].SetLineStartToEnd(m_pImmediateContext, m_tBox.vPos[3], m_tBox.vPos[7], LineColor);
		m_BoxLine[8].SetLineStartToEnd(m_pImmediateContext, m_tBox.vPos[4], m_tBox.vPos[5], LineColor);
		m_BoxLine[9].SetLineStartToEnd(m_pImmediateContext, m_tBox.vPos[5], m_tBox.vPos[6], LineColor);
		m_BoxLine[10].SetLineStartToEnd(m_pImmediateContext, m_tBox.vPos[6], m_tBox.vPos[7], LineColor);
		m_BoxLine[11].SetLineStartToEnd(m_pImmediateContext, m_tBox.vPos[7], m_tBox.vPos[4], LineColor);
		for (int iLine = 0; iLine < m_BoxLine.size(); iLine++)
		{
			m_BoxLine[iLine].m_iPrimitiveType = 2;
			m_BoxLine[iLine].Render(m_pImmediateContext);
		}
	}
	return true;
}
#pragma endregion

#pragma region 노말맵핑 관련 함수
/*노말맵핑 정보 상수버퍼 생성함수*/
HRESULT BAseObj::CreateNormalCB()
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC cbDesc;
	cbDesc.Usage = D3D11_USAGE_DEFAULT;					//CPU에서 GPU로 Access 가능하게 해주는 플래그
	cbDesc.CPUAccessFlags = 0;							//CPU에서 GPU로 Access 가능하게 해주는 플래그
	cbDesc.ByteWidth = sizeof(NORMAL_CONSTANT_BUFFER);
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.MiscFlags = 0;

	S_RETURN(m_pd3dDevice->CreateBuffer(&cbDesc, NULL, &m_pNormalCB));
	return hr;
}
/*노말맵핑용 카메라 위치, 시선벡터 설정함수*/
void BAseObj::SettingCameraForNormal(D3DXVECTOR3 Pos, D3DXVECTOR3 Look)
{
	m_cbNormal.cb_vEyePos = Pos;
	m_cbNormal.cb_vEyeDir = Look;
}
/*재정의 # 접선벡터 버퍼 갱신함수 (UpdateNormal 내부 호출)*/
bool BAseObj::UpDateTangentBuffer()
{
	D3DXVECTOR3 vTangent, vBiNormal, vNormal;
	int iIndex = 0;
	int i0, i1, i2, i3, i4, i5;
	for (DWORD dwObject = 0; dwObject < m_pData.size(); dwObject++)
	{
		BMesh2* pMesh = m_pData[dwObject].get();
		int iSubCount = pMesh->m_pSubMesh.size();
		if (iSubCount > 0)
		{
			for (DWORD dwSub = 0; dwSub < iSubCount; dwSub++)
			{
				BMesh2* pSubMesh = pMesh->m_pSubMesh[dwSub].get();
				if (pSubMesh->m_VertexArray.size() < 3) continue;

				m_VertexCnt = pSubMesh->m_VertexArray.size();
				pSubMesh->m_TangentList.resize(m_VertexCnt);

				for (int iIndex = 0; iIndex < pSubMesh->m_IndexArray.size(); iIndex += 3)
				{
					i0 = pSubMesh->m_IndexArray[iIndex + 0];
					i1 = pSubMesh->m_IndexArray[iIndex + 1];
					i2 = pSubMesh->m_IndexArray[iIndex + 2];

					pSubMesh->m_TangentList[i0] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					UpDateTangentSpaceVectors(&pSubMesh->m_VertexArray[i0].Position, &pSubMesh->m_VertexArray[i1].Position, &pSubMesh->m_VertexArray[i2].Position,
						pSubMesh->m_VertexArray[i0].Texture, pSubMesh->m_VertexArray[i1].Texture, pSubMesh->m_VertexArray[i2].Texture,
						&vTangent, &vBiNormal, &pSubMesh->m_VertexArray[i0].Normal);

					pSubMesh->m_TangentList[i0] = vTangent;

					i0 = pSubMesh->m_IndexArray[iIndex + 1];
					i1 = pSubMesh->m_IndexArray[iIndex + 2];
					i2 = pSubMesh->m_IndexArray[iIndex + 0];

					UpDateTangentSpaceVectors(&pSubMesh->m_VertexArray[i0].Position, &pSubMesh->m_VertexArray[i1].Position, &pSubMesh->m_VertexArray[i2].Position,
						pSubMesh->m_VertexArray[i0].Texture, pSubMesh->m_VertexArray[i1].Texture, pSubMesh->m_VertexArray[i2].Texture,
						&vTangent, &vBiNormal, &pSubMesh->m_VertexArray[i0].Normal);

					pSubMesh->m_TangentList[i0] = vTangent;

					i0 = pSubMesh->m_IndexArray[iIndex + 2];
					i1 = pSubMesh->m_IndexArray[iIndex + 0];
					i2 = pSubMesh->m_IndexArray[iIndex + 1];

					UpDateTangentSpaceVectors(&pSubMesh->m_VertexArray[i0].Position, &pSubMesh->m_VertexArray[i1].Position, &pSubMesh->m_VertexArray[i2].Position,
						pSubMesh->m_VertexArray[i0].Texture, pSubMesh->m_VertexArray[i1].Texture, pSubMesh->m_VertexArray[i2].Texture,
						&vTangent, &vBiNormal, &pSubMesh->m_VertexArray[i0].Normal);

					pSubMesh->m_TangentList[i0] = vTangent;
				}
				for (int iIndex = 0; iIndex < pSubMesh->m_TangentList.size(); iIndex++)
				{
					D3DXVec3Normalize(&pSubMesh->m_TangentList[iIndex], &pSubMesh->m_TangentList[iIndex]);
				}

				m_TangentCnt = pSubMesh->m_TangentList.size();

				D3D11_BUFFER_DESC bd;
				bd.Usage = D3D11_USAGE_DEFAULT;
				bd.ByteWidth = sizeof(D3DXVECTOR3) * m_TangentCnt;
				bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				bd.CPUAccessFlags = 0;
				bd.MiscFlags = 0;

				D3D11_SUBRESOURCE_DATA InitData;
				InitData.pSysMem = (void*)&pSubMesh->m_TangentList.at(0);
				m_pd3dDevice->CreateBuffer(&bd, &InitData, &pSubMesh->m_pTangentBuffer); // 접선벡터버퍼 생성
			}
		}
		else
		{
			if (pMesh->m_VertexArray.size() < 3) continue;
			m_VertexCnt = pMesh->m_VertexArray.size();
			pMesh->m_TangentList.resize(m_VertexCnt);

			for (int iIndex = 0; iIndex < pMesh->m_IndexArray.size(); iIndex += 3)
			{
				i0 = pMesh->m_IndexArray[iIndex + 0];
				i1 = pMesh->m_IndexArray[iIndex + 1];
				i2 = pMesh->m_IndexArray[iIndex + 2];

				UpDateTangentSpaceVectors(&pMesh->m_VertexArray[i0].Position, &pMesh->m_VertexArray[i1].Position, &pMesh->m_VertexArray[i2].Position,
					pMesh->m_VertexArray[i0].Texture, pMesh->m_VertexArray[i1].Texture, pMesh->m_VertexArray[i2].Texture,
					&vTangent, &vBiNormal, &pMesh->m_VertexArray[i0].Normal);

				pMesh->m_TangentList[i0] = vTangent;

				i0 = pMesh->m_IndexArray[iIndex + 1];
				i1 = pMesh->m_IndexArray[iIndex + 2];
				i2 = pMesh->m_IndexArray[iIndex + 0];

				UpDateTangentSpaceVectors(&pMesh->m_VertexArray[i0].Position, &pMesh->m_VertexArray[i1].Position, &pMesh->m_VertexArray[i2].Position,
					pMesh->m_VertexArray[i0].Texture, pMesh->m_VertexArray[i1].Texture, pMesh->m_VertexArray[i2].Texture,
					&vTangent, &vBiNormal, &pMesh->m_VertexArray[i0].Normal);

				pMesh->m_TangentList[i0] = vTangent;

				i0 = pMesh->m_IndexArray[iIndex + 2];
				i1 = pMesh->m_IndexArray[iIndex + 0];
				i2 = pMesh->m_IndexArray[iIndex + 1];

				UpDateTangentSpaceVectors(&pMesh->m_VertexArray[i0].Position, &pMesh->m_VertexArray[i1].Position, &pMesh->m_VertexArray[i2].Position,
					pMesh->m_VertexArray[i0].Texture, pMesh->m_VertexArray[i1].Texture, pMesh->m_VertexArray[i2].Texture,
					&vTangent, &vBiNormal, &pMesh->m_VertexArray[i0].Normal);

				pMesh->m_TangentList[i0] = vTangent;
			}
			for (int iIndex = 0; iIndex < pMesh->m_TangentList.size(); iIndex++)
			{
				D3DXVec3Normalize(&pMesh->m_TangentList[iIndex], &pMesh->m_TangentList[iIndex]);
			}

			m_TangentCnt = pMesh->m_TangentList.size();

			D3D11_BUFFER_DESC bd;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(D3DXVECTOR3) * m_TangentCnt;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA InitData;
			InitData.pSysMem = (void*)&pMesh->m_TangentList.at(0);
			m_pd3dDevice->CreateBuffer(&bd, &InitData, &pMesh->m_pTangentBuffer); // 접선벡터버퍼 생성
		}
	}
	return true;
}
/*재정의 # 페이스 노말 생성함수 (UpdateNormal 내부 호출)*/
bool BAseObj::InitFaceNormal()
{
	for (DWORD dwObject = 0; dwObject < m_pData.size(); dwObject++) // 매쉬의 개수만큼 돈다
	{
		BMesh2* pMesh = m_pData[dwObject].get();
		if (pMesh->m_pSubMesh.size() > 0)	// 서브매쉬가 존재한다면
		{
			for (DWORD dwSub = 0; dwSub < pMesh->m_pSubMesh.size(); dwSub++) // 서브매쉬 수만큼 생성
			{
				m_iNumFace = pMesh->m_pSubMesh[dwSub]->m_IndexArray.size() / 3;
				pMesh->m_pSubMesh[dwSub]->m_FaceNormals.resize(m_iNumFace);
				int iCnt = 0;
				for (int iFace = 0; iFace < m_iNumFace * 3; iFace += 3)
				{
					BMesh2* pSubMesh = pMesh->m_pSubMesh[dwSub].get();
					DWORD i0 = pSubMesh->m_IndexArray[iFace];
					DWORD i1 = pSubMesh->m_IndexArray[iFace + 1];
					DWORD i2 = pSubMesh->m_IndexArray[iFace + 2];

					D3DXVECTOR3 v0 = pSubMesh->m_VertexArray[i0].Position;
					D3DXVECTOR3 v1 = pSubMesh->m_VertexArray[i1].Position;
					D3DXVECTOR3 v2 = pSubMesh->m_VertexArray[i2].Position;

					D3DXVECTOR3 e0 = v1 - v0;
					D3DXVECTOR3 e1 = v2 - v0;

					D3DXVec3Cross(&pMesh->m_pSubMesh[dwSub]->m_FaceNormals[iCnt], &e0, &e1);
					D3DXVec3Normalize(&pMesh->m_pSubMesh[dwSub]->m_FaceNormals[iCnt], &pMesh->m_pSubMesh[dwSub]->m_FaceNormals[iCnt]);
					iCnt++;
				}
			}
		}
		else
		{
			m_iNumFace = pMesh->m_IndexArray.size() / 3;
			pMesh->m_FaceNormals.resize(m_iNumFace);
			int iCnt = 0;
			for (int iFace = 0; iFace < m_iNumFace * 3; iFace += 3)
			{
				DWORD i0 = pMesh->m_IndexArray[iFace];
				DWORD i1 = pMesh->m_IndexArray[iFace + 1];
				DWORD i2 = pMesh->m_IndexArray[iFace + 2];

				D3DXVECTOR3 v0 = pMesh->m_VertexArray[i0].Position;
				D3DXVECTOR3 v1 = pMesh->m_VertexArray[i1].Position;
				D3DXVECTOR3 v2 = pMesh->m_VertexArray[i2].Position;

				D3DXVECTOR3 e0 = v1 - v0;
				D3DXVECTOR3 e1 = v2 - v0;

				D3DXVec3Cross(&pMesh->m_FaceNormals[iCnt], &e0, &e1);
				D3DXVec3Normalize(&pMesh->m_FaceNormals[iCnt], &pMesh->m_FaceNormals[iCnt]);
				iCnt++;

			}
		}
	}
	return true;
}
/*룩업테이블(인덱스) 생성함수 (UpdateNormal 내부 호출)*/
void BAseObj::GenNormalLookupTable()
{
	for (DWORD dwObject = 0; dwObject < m_pData.size(); dwObject++) // 매쉬의 개수만큼 돈다
	{
		BMesh2* pMesh = m_pData[dwObject].get();
		if (pMesh->m_pSubMesh.size() > 0)	// 서브매쉬가 존재한다면
		{
			for (DWORD dwSub = 0; dwSub < pMesh->m_pSubMesh.size(); dwSub++) // 서브매쉬 수만큼 생성
			{
				BMesh2* pSubMesh = pMesh->m_pSubMesh[dwSub].get();
				pSubMesh->m_Normallookuptable.resize(pSubMesh->m_VertexArray.size() * 6);
				for (int iCnt = 0; iCnt < pSubMesh->m_Normallookuptable.size(); iCnt++)
				{
					pSubMesh->m_Normallookuptable[iCnt] = -1;
				}
				for (int iFaceCnt = 0; iFaceCnt < pSubMesh->m_IndexArray.size() / 3; iFaceCnt++)
				{
					for (int iListCnt = 0; iListCnt < 3; iListCnt++)
					{
						for (int iVertexCnt = 0; iVertexCnt < 6; iVertexCnt++)
						{
							int vertex = pSubMesh->m_IndexArray[iFaceCnt * 3 + iListCnt];
							if (pSubMesh->m_Normallookuptable[vertex * 6 + iVertexCnt] == -1)
							{
								pSubMesh->m_Normallookuptable[vertex * 6 + iVertexCnt] = iFaceCnt;
								break;
							}
						}
					}
				}
			}
		}
		else
		{
			pMesh->m_Normallookuptable.resize(pMesh->m_VertexArray.size() * 6);
			for (int LookupCnt = 0; LookupCnt < pMesh->m_Normallookuptable.size(); LookupCnt++)
			{
				pMesh->m_Normallookuptable[LookupCnt] = -1;
			}
			for (int iFaceCnt = 0; iFaceCnt < pMesh->m_iNumFace; iFaceCnt++)
			{
				for (int iListCnt = 0; iListCnt < 3; iListCnt++)
				{
					for (int iVertexCnt = 0; iVertexCnt < 6; iVertexCnt++)
					{
						int vertex = pMesh->m_IndexArray[iFaceCnt * 3 + iListCnt];
						if (pMesh->m_Normallookuptable[vertex * 6 + iVertexCnt] == -1)
						{
							pMesh->m_Normallookuptable[vertex * 6 + iVertexCnt] = iFaceCnt;
							break;
						}
					}
				}
			}
		}
	}
}
/*페이스노말 기반 버텍스 노말벡터 연산함수 (UpdateNormal 내부 호출)*/
void BAseObj::CalcPerVertexNormalsFastLookup()
{
	for (DWORD dwObject = 0; dwObject < m_pData.size(); dwObject++) // 매쉬의 개수만큼 돈다
	{
		BMesh2* pMesh = m_pData[dwObject].get();
		if (pMesh->m_pSubMesh.size() > 0)	// 서브매쉬가 존재한다면
		{
			for (DWORD dwSub = 0; dwSub < pMesh->m_pSubMesh.size(); dwSub++) // 서브매쉬 수만큼 생성
			{
				int iVertex = 0;
				BMesh2* pSubMesh = pMesh->m_pSubMesh[dwSub].get();
				for (int VertexCnt = 0; VertexCnt < pSubMesh->m_VertexArray.size(); VertexCnt++)
				{
					D3DXVECTOR3 avgNormal;
					avgNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					for (iVertex = 0; iVertex < 6; iVertex++) // 각 정점당 최대 6개의 페이스
					{
						int triIndex;
						triIndex = pSubMesh->m_Normallookuptable[VertexCnt * 6 + iVertex];

						if (triIndex != -1)
						{
							avgNormal += pSubMesh->m_FaceNormals[triIndex];
						}
						else
							break;
					}
					_ASSERT(iVertex > 0);
					avgNormal.x /= (float)iVertex;
					avgNormal.y /= (float)iVertex;
					avgNormal.z /= (float)iVertex;
					D3DXVec3Normalize(&avgNormal, &avgNormal);

					pSubMesh->m_VertexArray[VertexCnt].Normal.x = avgNormal.x;
					pSubMesh->m_VertexArray[VertexCnt].Normal.y = avgNormal.y;
					pSubMesh->m_VertexArray[VertexCnt].Normal.z = avgNormal.z;
				}
			}
		}
		else
		{
			int iVertex = 0;
			for (int VertexCnt = 0; VertexCnt < pMesh->m_VertexArray.size(); VertexCnt++)
			{
				D3DXVECTOR3 avgNormal;
				avgNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				for (iVertex = 0; iVertex < 6; iVertex++) // 각 정점당 최대 6개의 페이스
				{
					int triIndex;
					triIndex = pMesh->m_Normallookuptable[VertexCnt * 6 + iVertex];

					if (triIndex != -1)
					{
						avgNormal += pMesh->m_FaceNormals[triIndex];
					}
					else
						break;
				}
				_ASSERT(iVertex > 0);
				avgNormal.x /= (float)iVertex;
				avgNormal.y /= (float)iVertex;
				avgNormal.z /= (float)iVertex;
				D3DXVec3Normalize(&avgNormal, &avgNormal);

				pMesh->m_VertexArray[VertexCnt].Normal.x = avgNormal.x;
				pMesh->m_VertexArray[VertexCnt].Normal.y = avgNormal.y;
				pMesh->m_VertexArray[VertexCnt].Normal.z = avgNormal.z;
			}
		}
	}
}
#pragma endregion

#pragma region BMesh 관련 함수
void BMesh2::SetUniqueBuffer()
{
	int iNumFaces = m_FaceList.size() / 3;
	m_VertexArray.reserve(iNumFaces * 3);
	m_IndexArray.reserve(iNumFaces * 3);
	m_tBox.vMax = D3DXVECTOR3(-99999.0f, -99999.0f, -99999.0f);
	m_tBox.vMin = D3DXVECTOR3(99999.0f, 99999.0f, 99999.0f);

	for (int iCnt = 0; iCnt < m_FaceList.size(); iCnt++)
	{
		int iPosReturn = IsEqulVertexList(m_VertexArray, m_FaceList[iCnt]);
		if (iPosReturn < 0)
		{
			if (m_FaceList[iCnt].Position.x > m_tBox.vMax.x)
			{
				m_tBox.vMax.x = m_FaceList[iCnt].Position.x;
			}
			if (m_FaceList[iCnt].Position.y > m_tBox.vMax.y)
			{
				m_tBox.vMax.y = m_FaceList[iCnt].Position.y;
			}
			if (m_FaceList[iCnt].Position.z > m_tBox.vMax.z)
			{
				m_tBox.vMax.z = m_FaceList[iCnt].Position.z;
			}

			if (m_FaceList[iCnt].Position.x < m_tBox.vMin.x)
			{
				m_tBox.vMin.x = m_FaceList[iCnt].Position.x;
			}
			if (m_FaceList[iCnt].Position.y < m_tBox.vMin.y)
			{
				m_tBox.vMin.y = m_FaceList[iCnt].Position.y;
			}
			if (m_FaceList[iCnt].Position.z < m_tBox.vMin.z)
			{
				m_tBox.vMin.z = m_FaceList[iCnt].Position.z;
			}
			m_VertexArray.push_back(m_FaceList[iCnt]);
			iPosReturn = m_VertexArray.size() - 1;
		}
		m_IndexArray.push_back(iPosReturn);
	}
	m_tBox.vCenter = (m_tBox.vMax + m_tBox.vMin) / 2;
	m_tBox.fExtent[0] = m_tBox.vMax.x - m_tBox.vCenter.x;
	m_tBox.fExtent[1] = m_tBox.vMax.y - m_tBox.vCenter.y;
	m_tBox.fExtent[2] = m_tBox.vMax.z - m_tBox.vCenter.z;
	m_tBox.vAxis[0] = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_tBox.vAxis[1] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_tBox.vAxis[2] = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	//  1   2
	//0   3
	//  5   6
	//4   7
	m_tBox.vPos[0] = D3DXVECTOR3(m_tBox.vMin.x, m_tBox.vMax.y, m_tBox.vMin.z);
	m_tBox.vPos[1] = D3DXVECTOR3(m_tBox.vMin.x, m_tBox.vMax.y, m_tBox.vMax.z);
	m_tBox.vPos[2] = D3DXVECTOR3(m_tBox.vMax.x, m_tBox.vMax.y, m_tBox.vMax.z);
	m_tBox.vPos[3] = D3DXVECTOR3(m_tBox.vMax.x, m_tBox.vMax.y, m_tBox.vMin.z);

	m_tBox.vPos[4] = D3DXVECTOR3(m_tBox.vMin.x, m_tBox.vMin.y, m_tBox.vMin.z);
	m_tBox.vPos[5] = D3DXVECTOR3(m_tBox.vMin.x, m_tBox.vMin.y, m_tBox.vMax.z);
	m_tBox.vPos[6] = D3DXVECTOR3(m_tBox.vMax.x, m_tBox.vMin.y, m_tBox.vMax.z);
	m_tBox.vPos[7] = D3DXVECTOR3(m_tBox.vMax.x, m_tBox.vMin.y, m_tBox.vMin.z);

}
int BMesh2::IsEqulVertexList(vector<PNCT_Vertex>&  VertexArray, PNCT_Vertex& Vertex)
{
	for (int iVertex = 0; iVertex < VertexArray.size(); iVertex++)
	{
		if (VertexArray[iVertex] == Vertex)
		{
			return iVertex;
		}
	}
	return -1;
}
#pragma endregion

BAseObj::BAseObj()
{
}
BAseObj::~BAseObj()
{
}