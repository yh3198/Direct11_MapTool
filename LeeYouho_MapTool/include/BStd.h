#pragma once

/*라이브러리*/
#pragma warning (disable:4005)
#pragma comment (lib, "d3d11.lib")

#if defined(_DEBUG)
#pragma comment (lib, "d3dx10d.lib")
#pragma comment (lib, "d3dx11d.lib")
#else
#pragma comment (lib, "d3dx10.lib")
#pragma comment (lib, "d3dx11.lib")
#endif

#pragma comment (lib, "dxgi.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "d2d1.lib")
#pragma comment (lib, "dwrite.lib")
#pragma comment (lib, "WindowsCodecs.lib")
#pragma comment(lib, "fmod_vc.lib")

/*헤더파일*/
#include <fmod.hpp>
#include <fmod_errors.h>
#include <stdlib.h>
#include <queue>
#include <windows.h>
#include <assert.h>
#include <tchar.h>
#include <crtdbg.h>
#include <math.h>
#include <string>
#include <d3dx11.h>
#include <d3d11.h>
#include <dinput.h>
#include <D3DCompiler.h>
#include <time.h>
#include <D3DX10math.h>
#include <vector>
#include <map>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include <memory>
#include <wrl.h>


extern UINT	 g_iWindowWidth;
extern UINT	 g_iWindowHeight;

using namespace Microsoft::WRL;
using namespace std;
#define _CRT_SECURE_NO_WARNINGS

namespace BBASIS
{
	extern float g_fSecPerFrame;
	extern float g_fDurationTime;
	extern HWND g_hWnd;
	extern HINSTANCE g_hInstance;
	
	struct BBASIS_INPUT_MAP
	{
		bool bUpKey;
		bool bDownKey;
		bool bLeftKey;
		bool bRightKey;

		bool bWKey;
		bool bSKey;
		bool bAKey;
		bool bDKey;
		bool bQKey;
		bool bEKey;
		bool bZKey;
		bool bCKey;

		bool bBackViewKey;
		bool bDebugViewKey;
		bool bTreeViewKey;

		bool bLeftClick;
		bool bRightClick;
		bool bMiddleClick;

		bool bLeftHold;
		bool bRightHold;
		bool bMiddleHold;

		bool bExit;
		bool bSpace; // 카메라의 이동가속도를 증가시킨다.

		int  iMouseValue[3];

		bool bFullScreen;
		bool bChangeFillMode;
		bool bChangePrimitive;
		bool bChangeCullMode;
		bool bChangeCameraType;
		bool bDebugRender;
	};
	extern BBASIS_INPUT_MAP g_InputData;


	typedef basic_string<TCHAR> T_STR;
	typedef basic_string<wchar_t> W_STG;
	typedef basic_string<char>  C_STR;
	typedef vector<T_STR>		T_STR_VECTOR;
	typedef basic_string<TCHAR>::iterator	T_ITOR;
	typedef basic_string<wchar_t>::iterator	W_ITOR;
	typedef basic_string<char>::iterator	C_ITOR;
	typedef vector<T_STR>		T_ARRAY_ITOR;

	static std::wstring mtw(std::string str)
	{
		std::wstring ret = std::wstring(str.begin(), str.end());
		return  ret;
	}
	static std::string wtm(std::wstring str)
	{
		return  std::string(str.begin(), str.end());
	}
	static char* GetWtM(WCHAR* data)
	{
		char retData[4096] = { 0 };
		// 변형되는 문자열의 크기가 반환된다.
		int iLength = WideCharToMultiByte(CP_ACP, 0,
			data, -1, 0, 0, NULL, NULL);
		int iRet = WideCharToMultiByte(CP_ACP, 0,
			data, -1,  //  소스
			retData, iLength, // 대상
			NULL, NULL);
		return retData;
	}
	static bool GetWtM(WCHAR* src, char* pDest)
	{
		// 변형되는 문자열의 크기가 반환된다.
		int iLength = WideCharToMultiByte(CP_ACP, 0,
			src, -1, 0, 0, NULL, NULL);
		int iRet = WideCharToMultiByte(CP_ACP, 0,
			src, -1,  //  소스
			pDest, iLength, // 대상
			NULL, NULL);
		if (iRet == 0) return false;
		return true;
	}
	static WCHAR* GetMtW(char* data)
	{
		WCHAR retData[4096] = { 0 };
		// 변형되는 문자열의 크기가 반환된다.
		int iLength = MultiByteToWideChar(CP_ACP, 0, data, -1, 0, 0);
		int iRet = MultiByteToWideChar(CP_ACP, 0,
			data, -1,  //  소스
			retData, iLength); // 대상
		return retData;
	}
	static bool GetMtW(char* pSrc, WCHAR* pDest)
	{
		// 변형되는 문자열의 크기가 반환된다.
		int iLength = MultiByteToWideChar(CP_ACP, 0,
			pSrc, -1, 0, 0);
		int iRet = MultiByteToWideChar(CP_ACP, 0,
			pSrc, -1,  //  소스
			pDest, iLength); // 대상		
		if (iRet == 0) return false;
		return true;
	}
	static void PRINT(char* fmt, ...) // 나열연산자
	{
		va_list arg;
		va_start(arg, fmt);
		char buf[256] = { 0, };
		vsprintf_s(buf, fmt, arg);
		printf("\n=====> %s", buf);
		va_end(arg);
	}

	static void ErrorQuit(TCHAR *msg)
	{
		LPVOID lpMsgBuf;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(TCHAR*)&lpMsgBuf, 0, NULL);
		MessageBox(NULL, (TCHAR*)lpMsgBuf, msg, MB_ICONERROR);
		LocalFree(lpMsgBuf);
		exit(-1);
	}

	template<typename T>
	void stl_wipe_vector(vector<T>& rVector)
	{
		vector<T> emptyData;
		rVector.swap(emptyData);
	}
}

using namespace BBASIS;

/*싱글톤*/
template<class T> class BSingleton
{
public:
	static T& GetInstance()
	{
		static T theSingleInstance;
		return theSingleInstance;
	}
};

/*객체 및 배열 할당과 삭제 및 소멸 매크로*/
#ifndef S_ZERO
#define S_ZERO(A)				{ A = 0 };
#endif

#ifndef S_NEW
#define S_NEW(A, B)				{ if (!A) A = new B; }
#endif 

#ifndef S_DEL
#define S_DEL(A)					{ if (A) delete A; (A)=NULL; }
#endif 

#ifndef S_NEW_ARRAY
#define S_NEW_ARRAY(A, B, C)		{ if (!A && C) A = new B[C]; }
#endif 

#ifndef S_DELETE_ARRAY
#define S_DELETE_ARRAY(A)		{ if (A) delete [] A; (A)=NULL; }
#endif 

#ifndef S_RELEASE
#define S_RELEASE(A)				{ if(A) { (A)->Release(); (A)=NULL; } }
#endif 

#ifndef S_NEW_CLEAR
#define S_NEW_CLEAR( A, B )			{ if (!A) A = new B; if(A) memset( A, 0, sizeof(B) ); };
#endif 

#ifndef S_NEW_ARRAY_CLEAR
#define S_NEW_ARRAY_CLEAR( A, B, C )	{ if (!A && C) A = new B[C]; if(A) memset( A, 0, sizeof(B)*C ); };
#endif

#ifndef S_RETURN
#define S_RETURN(x){ hr = (x); if( FAILED(hr) ) { return hr; } }
#endif

#define BBASIS_START int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow ){ Sample gSample;
#define BBASIS_WIN(x) if( !gSample.InitWindow( hInstance, nCmdShow,  L#x ) )	{	return 0;	}	ShowCursor ( TRUE );gSample.Run();	return 1; }
#define BBASIS_RUN(x) TBASIS_START; TBASIS_WIN(x);

/*맵툴 내 사용 추가 구조체*/
struct InstancingInfo
{
	int ObjName;
	int iInstanceNumber;
	D3DXVECTOR3 Scale;
	D3DXVECTOR3 RotationAxis[3];
	float Angle[3];
	D3DXVECTOR3 Translate;
	bool Selected;
	InstancingInfo()
	{
		ObjName = 9999;
		Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		RotationAxis[0] = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		RotationAxis[1] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		RotationAxis[2] = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		Angle[0] = 0;
		Angle[1] = 0;
		Angle[2] = 0;
		Translate = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Selected = false;
	}
};

struct SHADOW_CONSTANT_BUFFER
{
	D3DXMATRIX			g_matShadow;
	float				g_ShadowID;
	float				g_iNumKernel; // 커널을 통해 블러작업을 좀 더 주겠다.
	float				fZ, fW;
};

struct NORMAL_CONSTANT_BUFFER
{
	D3DXMATRIX g_matNormal[200];
	D3DXVECTOR3 cb_vLightVector;
	float Dumy1;
	D3DXVECTOR3 cb_vEyePos;
	float Dumy2;
	D3DXVECTOR3 cb_vEyeDir;
	float Dumy3;
};

enum T_POSITION
{
	P_BACK = 0,
	P_FRONT,
	P_ONPLANE,
	P_SPANNING
};

struct BMapDesc
{
	int				iNumCols;
	int				iNumRows;
	int				iCellCount;
	int				iPatchCount;
	float			fCellDistance;
	float			fScaleHeight;
	basic_string<TCHAR>	strTextureFile;
	basic_string<TCHAR>	strShaderFile;
	BMapDesc() { iNumCols = 0; iNumRows = 0; iCellCount = 0; iPatchCount = 0; fCellDistance = 0.0f; fScaleHeight = 1.0f; }
};

enum FootSound
{
	Ground = 0,
	Sand,
	Iron,
	Water,
	Carfet,
	wood,
};

struct TileInfo
{
	int TileSound;				// 타일 위에서 나는 소리
	bool MonsterSpawnIndex[5];  // 해당 타일에서 생성될 수 있는 몬스터의 종류
	bool TileMoveUnable;			// 진입불가능여부
	bool TileiTemRespawn;		// 아이템 생성 가능 여부
	bool TileMonsterRespawn;	// 몬스터 생성 가능 여부
	D3DXVECTOR3 TileCenter;		// 아이템 or 몬스터의 생성지점
	D3DXVECTOR3 TileEdge[4];	// 해당 타일의 4개의 버택스 값
	D3DXVECTOR3 TileNormal[2];     // 충돌처리를 위한 노말값
};

struct MapObjInfo
{
	int m_iObjType; // 오브젝트의 종류
	int m_iObjNum;  // 오브젝트의 순서
};
