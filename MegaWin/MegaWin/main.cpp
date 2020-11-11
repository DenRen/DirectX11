#include <cstdio>
#include "AppMgr.h"
#include "DebugFuncs.h"

int main ()
{
	AppMgr app;

	if (app.Initialize ())
	{
		app.Run ();
	}

	DebugEndMain ();
	

	//Rectangle rect (0, 0, 200, 100, RGBA::YELLOW);
}