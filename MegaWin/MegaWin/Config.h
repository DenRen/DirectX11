#pragma once

//#define TURN_DEBUG

namespace AppCnf
{
	const int scale  = 1;		// Scale is 1:scale, f.e. scale = 2 => 1:2 (1280; 720) -> (640; 360) 
	const int WIDTH	 = 1920;
	const int HEIGHT = 1080;
	const int MAXFPS = 120;
	const bool VSYNC = false;
}

namespace WndCnf
{
	const int WIDTH  = AppCnf::WIDTH  / AppCnf::scale;
	const int HEIGHT = AppCnf::HEIGHT / AppCnf::scale;
	const int MAXFPS = AppCnf::MAXFPS;
	const bool VSYNC = AppCnf::VSYNC;
}