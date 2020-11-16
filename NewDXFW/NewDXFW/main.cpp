#include <cstdio>
#include "AppMgr.h"
#include "DebugFunc.h"

int main ()
{
	auto App = new AppMgr ();
	if (!App->Initialize ())
	{
		RETURN_FALSE;
	}

	App->Run ();

	DebugEndMain ();
}