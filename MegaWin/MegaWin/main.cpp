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
}