#include <cstdio>
#include "AppMgr.h"
#include "DebugFuncs.h"
#include "Rectangle.h"

int main ()
{
	AppMgr app;

	if (!app.Initialize ())
	{
		RETURN_FALSE;
	}

	app.Run ();

	DebugEndMain ();
}