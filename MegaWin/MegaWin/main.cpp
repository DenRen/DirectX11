#include <cstdio>
#include "AppMgr.h"
#include "DebugFuncs.h"

int main ()
{
	AppMgr app;

	app.Initialize ();
	
	app.Run ();

	DebugEndMain ();
}