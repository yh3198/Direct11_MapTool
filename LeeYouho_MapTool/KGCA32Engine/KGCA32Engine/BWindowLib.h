#pragma once
#include "BDevice.h"

class BWindowLib : public BDevice
{
public:
	/*��������*/
	HINSTANCE               m_hInstance;		// �ν��Ͻ� �ڵ�
	HWND                    m_hWnd;				// ������ �ڵ�
	DWORD					m_dwWindowStyle;	// ������ ��Ÿ��
	RECT					m_rcWindowBounds;   // ������ ����
	RECT					m_rcWindowClient;   // Ŭ���̾�Ʈ ����	
	UINT					m_iWindowWidth;		// Ŭ���̾�Ʈ ���� ����ũ��
	UINT					m_iWindowHeight;	// �����̾�Ʈ ���� ����ũ��
public:
	// ������ ����
	bool				InitWindow(HINSTANCE hInstance, int nCmdShow, TCHAR* strWindowTitle);
	// ������ ��ġ ȭ�� �߾����� �̵�
	void				CenterWindow(HWND hwnd);
	LRESULT WindowMsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual int			WndProc(HWND, UINT, WPARAM, LPARAM);

public:
	BWindowLib();
	virtual ~BWindowLib();
};

