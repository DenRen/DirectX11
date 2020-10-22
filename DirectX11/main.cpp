#include "DXFW.h"

int main () {
	DXFW *framework = new DXFW ();

	if (framework->Initialize ())
	{
		framework->Run ();
	}

	delete framework;
	return 0;
}