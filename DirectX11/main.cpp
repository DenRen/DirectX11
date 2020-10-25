#include "DXFW.h"
#include "DXMAnager.h"

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