#include "Config.h"
#include <windows.h>
#include <math.h>

namespace WndCnf
{
	WindowDesc::WindowDesc (int coordX, int coordY) :
		title (TITLE),
		width (WIDTH),
		height (HEIGHT),
		widthOffset (WIDTH_OFFSET),
		heightOffset (HEIGHT_OFFSET),
		locateX (coordX),
		locateY (coordY),
		fullScreen (FULLSCREEN),
		vSync (VSYNC)
	{}

	// Предполагается, что экран DX от {minX:max, minY:maxY}
	void ConvertMouseCoor (UINT lParam, float &coorX, float &coorY)
	{
		static bool positivX = false, positivY = true;

		coorX = -maxX * (1.0f - 2.0f * LOWORD (lParam) / ((float) WIDTH  + WIDTH_OFFSET));
		coorY =  maxY * (1.0f - 2.0f * HIWORD (lParam) / ((float) HEIGHT + HEIGHT_OFFSET));

		if (LOWORD (lParam) > 16000)
		{
			coorX = (2 * positivX - 1) * maxX;
		}
		else
		{
			positivX = !signbit (coorX);
		}

		if (HIWORD (lParam) > 16000)
		{
			coorY = (2 * positivY - 1) * maxY;
		}
		else
		{
			positivY = !signbit (coorY);
		}
	}
}