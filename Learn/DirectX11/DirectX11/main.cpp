#include "DXFW.h"
#include "GameScene.h"

int main () {

	DXFW *framework = new DXFW ();

	if (framework->Initialize (new GameScene ()))
	{
		framework->Run ();
	}

	delete framework;

	return 0;
}