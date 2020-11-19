#pragma once

#define TURN_DEBUG

#include <windows.h>

namespace WndCnf
{
	const int WIDTH = 1280;
	const int HEIGHT = 720;
	const int WIDTH_OFFSET  = -20;
	const int HEIGHT_OFFSET = -31;
	const int MAXFPS = 120;
	const bool VSYNC = false;
	const bool FULLSCREEN = false;
	const char TITLE[] = "MegaWindow";

	const float minX = -1.0f;
	const float maxX = +1.0f;
	const float maxY = +((float) HEIGHT) / WIDTH;
	const float minY = -maxY;

	const float lenX = 2;
	const float lenY = 2 * maxY;

	struct WindowDesc
	{
		const char *title;
		unsigned width;
		unsigned height;
		int widthOffset;
		int heightOffset;
		int locateX;
		int locateY;
		bool fullScreen;
		bool vSync;

		WindowDesc (int coordX = 0, int coordY = 0);
	};

	void ConvertMouseCoor (UINT lParam, float &coorX, float &coorY);
}