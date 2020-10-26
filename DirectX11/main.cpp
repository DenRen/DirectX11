#include <chrono>
#include "DXFW.h"
#include "DXMAnager.h"
#include "Timer.h"
#include <Windows.h>
#include <iostream>

int main () {

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