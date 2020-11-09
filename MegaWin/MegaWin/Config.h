#pragma once

#include <string>

#define TURN_DEBUG

namespace AppCnf
{
	const int scale  = 1;		// Scale is 1:scale, f.e. scale = 2 => 1:2 (1280; 720) -> (640; 360)
}

namespace WndCnf
{
	const int WIDTH  = 1920 / AppCnf::scale;
	const int HEIGHT = 1080 / AppCnf::scale;
	const int MAXFPS = 120;
	const bool VSYNC	  = false;
	const bool FULLSCREEN = false;
	const char TITLE[] = "MegaWindow";

	struct WindowDesc
	{
		const char *title;
		unsigned width;
		unsigned height;
		int locateX;
		int locateY;
		bool fullScreen;
		bool vSync;

		WindowDesc (int coordX = 0, int coordY = 0);
	};
}