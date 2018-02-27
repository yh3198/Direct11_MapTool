#include "BWindowLib.h"

BWindowLib* g_pWindow = NULL;
UINT g_iWindowWidth = 0;
UINT g_iWindowHeight = 0;

namespace BBASIS
{
	HWND g_hWnd;
	HINSTANCE g_hInstance;
};

/*���� ������ ���ν���*/
LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	assert(g_pWindow);
	return g_pWindow->WindowMsgProc(hWnd, message, wParam, lParam);
}

/*�����Լ� ������ ���ν���*/
int BWindowLib::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return -1;
}

/*������ ���ν���*/
LRESULT BWindowLib::WindowMsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int iReturn = WndProc(hWnd, message, wParam, lParam);
	if (iReturn >= 0)
	{
		return iReturn;
	}

	switch (message)
	{
	case WM_SIZE:
	{
		if (SIZE_MINIMIZED != wParam) // �ּ�ȭ
		{
			UINT width = LOWORD(lParam);
			UINT height = HIWORD(wParam);
			ResizeDevice(width, height);
		}
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_F8:
		{
			BOOL isFullScreen = TRUE;
			m_pSwapChain->GetFullscreenState(&isFullScreen, NULL);
			m_pSwapChain->SetFullscreenState(!isFullScreen, NULL);
		} break;
		case VK_ESCAPE:
		{
			SendMessage(hWnd, WM_CLOSE, 0, 0);
		}break;
		}
	}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}
void BWindowLib::CenterWindow(HWND hwnd)
{
	// ȭ�� ��ũ���� �ػ�(���̿� ����)�� ��´�.
	int iScreenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	int iScreenHeight = GetSystemMetrics(SM_CYFULLSCREEN);

	// ������ Ŭ���̾�Ʈ �߾Ӱ� ȭ�� ��ũ�� �߾��� �����.
	int iDestX = (iScreenWidth - (m_rcWindowBounds.right - m_rcWindowBounds.left)) / 2;
	int iDestY = (iScreenHeight - (m_rcWindowBounds.bottom - m_rcWindowBounds.top)) / 2;

	// �����츦 ȭ���߾����� �̵���Ų��.
	MoveWindow(hwnd, iDestX, iDestY, m_rcWindowBounds.right - m_rcWindowBounds.left, m_rcWindowBounds.bottom - m_rcWindowBounds.top, true);
}

bool BWindowLib::InitWindow(HINSTANCE hInstance, int nCmdShow, TCHAR* strWindowTitle)
{
	// Register class
	g_hInstance = hInstance;
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = &StaticWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"TBasis3D11_Sample";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	if (!RegisterClassEx(&wcex))
		return false;

	// Create window
	m_hInstance = hInstance;
	RECT rc = { 0, 0, 800, 600 };
	// �۾�����(  Ÿ��Ʋ ��/��輱/�޴�/��ũ�� �� ���� ������ ������ ����), ������ ��Ÿ��, �޴�����
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	m_hWnd = CreateWindowEx(WS_EX_TOOLWINDOW, //WS_EX_TOPMOST,// | WS_EX_TOOLWINDOW,
		L"TBasis3D11_Sample", strWindowTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,
		NULL);
	if (!m_hWnd)
		return false;

	g_hWnd = m_hWnd;
	// ������ ��Ÿ���� �����ϰ� ��ũ�� ������ Ŭ���̾�Ʈ ������ ��´�.
	m_dwWindowStyle = GetWindowLong(m_hWnd, GWL_STYLE);
	GetWindowRect(m_hWnd, &m_rcWindowBounds);
	GetClientRect(m_hWnd, &m_rcWindowClient);

	CenterWindow(m_hWnd);
	UpdateWindow(m_hWnd);

	// ������ Ŭ���̾�Ʈ ���̿� ���̸� ��´�.
	m_iWindowWidth = m_rcWindowClient.right - m_rcWindowClient.left;
	m_iWindowHeight = m_rcWindowClient.bottom - m_rcWindowClient.top;

	g_iWindowWidth = m_iWindowWidth;
	g_iWindowHeight = m_iWindowHeight;
	ShowWindow(m_hWnd, nCmdShow);
	return true;
}

BWindowLib::BWindowLib()
{
	m_hInstance = NULL;
	m_hWnd = NULL;
	g_pWindow = this;
}

BWindowLib::~BWindowLib()
{
}
