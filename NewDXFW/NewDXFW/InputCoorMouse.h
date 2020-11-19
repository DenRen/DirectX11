#pragma once

#include "NewsQueue.h"

class InputCoorMouse
{
public:

	static void SetPosition (MousePosition mousePosition);
	static void SetPosition (float coorX, float coorY);

	static MousePosition GetPosition ();

	static MousePosition m_mousePos;

	static bool Changed ();

private:

	static void CheckChanged ();
	static MousePosition m_prevMousePos;
	static bool m_changed;
};

