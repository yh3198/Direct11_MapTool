#pragma once
#include "BStd.h"

using namespace BBASIS;

enum keyState {
	KEY_FREE = 0,
	KEY_UP = 1,
	KEY_PUSH = 2,
	KEY_HOLD = 3,
};

class BInput : public BSingleton <BInput>
{
private:
	friend class BSingleton<BInput>;

public:
	LPDIRECTINPUT8 m_pDI;					// ���̷�Ʈ ��ǲ ��ü
	LPDIRECTINPUTDEVICE8 m_pKeyDevice;		// Ű����
	LPDIRECTINPUTDEVICE8 m_pMouseDevice;	// ���콺
	
	DIDEVICEOBJECTDATA m_didod[16];


	BYTE m_KeyState[256];					// Ű���忡�� �޾ƿ��� �Է������� ��
	BYTE m_KeyStateOld[256];				// hold, up ���¸� Ȯ���ϱ� ���� ���� ���°�
	DIMOUSESTATE m_DIMouseState;			// ���콺 ���°�
	DIMOUSESTATE m_DIMouseBeforeState;		// ���콺 �������°�
  
	TCHAR m_csBuffer[260];
	BYTE m_MouseState[3];
	BYTE m_BeforeMouseState[3];
	POINT m_MousePos;
	DWORD m_dwImmediate;

	/*MFC��*/
	BYTE m_MFCDIMouseState[3];
	BYTE m_MFCDIMouseBeforeState[3];
	HWND m_hWnd;
public:
	bool Init();
	bool frame();
	bool Render();
	bool Release();

	BYTE KeyCheck(BYTE dwKey);
	HRESULT InitDirectInput();
	void DeviceAcquire();
	void DeviceUnacquire();
	void MsgEvent(MSG msg);

public:
	BInput();
	~BInput();
};

#define I_Input BInput::GetInstance()