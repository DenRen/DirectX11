#include "Config.h"

namespace WndCnf
{
	WindowDesc::WindowDesc (int coordX, int coordY) :
		title (TITLE),
		width (WIDTH),
		height (HEIGHT),
		locateX (coordX),
		locateY (coordY),
		fullScreen (FULLSCREEN),
		vSync (VSYNC)
	{}
}