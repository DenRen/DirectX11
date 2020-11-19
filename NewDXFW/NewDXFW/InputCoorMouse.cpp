#include <cmath>

#include "InputCoorMouse.h"

MousePosition InputCoorMouse::m_mousePos = {0, 0};
MousePosition InputCoorMouse::m_prevMousePos = {0, 0};
bool InputCoorMouse::m_changed = false;

void InputCoorMouse::SetPosition (MousePosition mousePosition)
{
	m_mousePos = mousePosition;

	CheckChanged ();
}

void InputCoorMouse::SetPosition (float coorX, float coorY)
{
	m_mousePos.x = coorX;
	m_mousePos.y = coorY;

	CheckChanged ();
}

MousePosition InputCoorMouse::GetPosition ()
{
	return m_mousePos;
}

bool InputCoorMouse::Changed ()
{
	return m_changed;
}

void InputCoorMouse::CheckChanged ()
{
	if (fabs (m_mousePos.x - m_prevMousePos.x) > 0.01 ||
		fabs (m_mousePos.y - m_prevMousePos.y) > 0.01)
	{
		m_prevMousePos = m_mousePos;
		m_changed = true;
	}
	else
	{
		m_changed = false;
	}
}
