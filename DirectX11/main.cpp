#include <chrono>
#include "DXFW.h"
#include "DXMAnager.h"
#include "Timer.h"
#include <Windows.h>

int main () {

	printf ("%g\n", Timer::GetDeltaTime ());
	Sleep (2000);
	printf ("%g\n", Timer::GetDeltaTime ());
	Sleep (2000);
	printf ("%g\n", Timer::GetDeltaTime ());


	DXFW *framework = new DXFW ();
	//DXManager *DXMgr = new DXManager ();

	if (framework->Initialize ())
	{
		framework->Run ();
	}

	//delete DXMgr;
	delete framework;

	return 0;
}