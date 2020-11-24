#include "Input.h"
#include "DebugFuncs.h"

Input::Input () :
	m_directInput (nullptr),
	m_keyboard (nullptr),
	m_mouse (nullptr),
	m_hWnd (nullptr),
	m_screenWidth (0),
	m_screenHeight (0),
	m_mouseX (0),
	m_mouseY (0)
{
	ZeroMemory (m_keys,		sizeof (bool) * s_NumKeys);
	ZeroMemory (m_prevKeys, sizeof (bool) * s_NumKeys);
}

Input::~Input ()
{
	if (m_mouse != nullptr)
	{
		m_mouse->Unacquire ();
		m_mouse->Release ();
		m_mouse = nullptr;
	}

	if (m_keyboard != nullptr)
	{
		m_keyboard->Unacquire ();
		m_keyboard->Release ();
		m_keyboard = nullptr;
	}

	// Release the main interface to direct input
	RELEASE (m_directInput);
}

bool Input::Initialize (HINSTANCE hInstance, HWND hWnd, int screenWidth, int screenHeight)
{
	m_hWnd = hWnd;

	HRESULT result = S_OK;

	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	m_mouseX = 0;
	m_mouseY = 0;

	// Initialize the direct input interface
	result = DirectInput8Create (hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
								 (void **) &m_directInput, nullptr);
	CHECK_FAILED (result);

	if (!InitializeMouse ())
	{
		RETURN_FALSE;
	}

	if (!InitializeKeyboard ())
	{
		RETURN_FALSE;
	}

	return true;
}

bool Input::Update ()
{
	memcpy (m_prevKeys, m_keys, sizeof (bool) * s_NumKeys);

	bool result = false;

	// Read keyboard
	result = ReadKeyboard ();
	if (!result)
	{
		RETURN_FALSE;
	}

	// ReadMouse
	result = ReadMouse ();
	if (!result)
	{
		RETURN_FALSE;
	}

	// Update mouse position
	ProccessInput ();

	return true;
}

bool Input::IsKeyDown (unsigned int key)
{
	return m_keys[key];
}

bool Input::IsKeyHit (unsigned int key)
{
	return m_keys[key] && !m_prevKeys[key];
}

void Input::GetMousePosition (int &x, int &y)
{
	x = m_mouseX;
	y = m_mouseY;
}

bool Input::ReadKeyboard ()
{
	if (m_keyboard == nullptr)
	{
		RETURN_FALSE;
	}

	HRESULT result = S_OK;

	// Read keyboard device
	result = m_keyboard->GetDeviceState (sizeof (m_keys), (LPVOID) &m_keys);
	if (FAILED (result))
	{
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
		{
			m_keyboard->Acquire ();
		}
		else
		{
			RETURN_FALSE;
		}
	}

	return true;
}

bool Input::ReadMouse ()
{
	// Sometimes mouse doesn't get initialized when putting a breakpoint 
	// before it reaches it's update 
	
	if (m_mouse == nullptr)
	{
		m_directInput->CreateDevice (GUID_SysKeyboard, &m_keyboard, nullptr);
		RETURN_FALSE;
	}

	HRESULT result = S_OK;

	// Read mouse device
	result = m_mouse->GetDeviceState (sizeof (DIMOUSESTATE), (LPVOID) &m_mouseState);
	if (FAILED (result))
	{
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
		{
			m_mouse->Acquire ();
		}
		else
		{
			RETURN_FALSE;
		}
	}

	return true;
}

void Input::ProccessInput ()
{
	m_mouseX += m_mouseState.lX;	// TODO: Shit needs to be redone
	m_mouseY += m_mouseState.lY;
	
	if (m_mouseX < 0) m_mouseX = 0;
	if (m_mouseY < 0) m_mouseY = 0;
	if (m_mouseX > m_screenWidth)  m_mouseX = m_screenWidth;
	if (m_mouseY > m_screenHeight) m_mouseY = m_screenHeight;

}

bool Input::InitializeMouse ()
{
	HRESULT result = S_OK;

	// Initialize keyboard interface ----------------------------------------------------
	result = m_directInput->CreateDevice (GUID_SysKeyboard, &m_keyboard, nullptr);
	CHECK_FAILED (result);

	result = m_keyboard->SetDataFormat (&c_dfDIKeyboard);
	CHECK_FAILED (result);

	// Set cooperative level of the keyboard so it doesn't share with other programs
	result = m_keyboard->SetCooperativeLevel (m_hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	CHECK_FAILED (result);

	result = m_keyboard->Acquire ();
	CHECK_FAILED (result);

	return true;
}

bool Input::InitializeKeyboard ()
{
	HRESULT result = S_OK;

	// Initialize mouse interface -------------------------------------------------------
	result = m_directInput->CreateDevice (GUID_SysMouse, &m_mouse, nullptr);
	CHECK_FAILED (result);

	result = m_mouse->SetDataFormat (&c_dfDIMouse);
	CHECK_FAILED (result);

	// Set cooperative level pf the mouse to not share with other programs
	result = m_mouse->SetCooperativeLevel (m_hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	CHECK_FAILED (result);

	result = m_mouse->Acquire ();
	CHECK_FAILED (result);

	return true;
}
