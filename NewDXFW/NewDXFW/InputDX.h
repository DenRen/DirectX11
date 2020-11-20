#pragma once

const int DIRECTINPUT_VERSION = 0x0800; // Removes annoying message defaulting to version 8 

#include <dinput.h>

class InputDX
{
public:
	InputDX ();
	~InputDX ();

	bool Initialize (HINSTANCE hInstance, HWND hWnd);
	bool Update ();

	bool IsKeyDown (unsigned int key);
	bool IsKeyHit (unsigned int key);
	void GetMousePosition (int &x, int &y);

private:
	const static int s_NumKeys = 256;

	bool ReadKeyboard ();
	bool ReadMouse ();
	void ProccessInput ();

	IDirectInput8 *m_directInput;
	IDirectInputDevice8 *m_keyboard;
	IDirectInputDevice8 *m_mouse;
	DIMOUSESTATE m_mouseState;

	bool m_keys[s_NumKeys];
	bool m_prevKeys[s_NumKeys];

	bool InitializeMouse ();
	bool InitializeKeyboard ();

	int m_mouseX;
	int m_mouseY;

	HWND m_hWnd;
};