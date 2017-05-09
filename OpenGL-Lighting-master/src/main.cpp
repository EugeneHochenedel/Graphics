 
#include "LightingOBJApplication.h"
#include "LightingSphereApplication.h" 

int main() {
	
	// change startup application
	BaseApplication* app = new LightingSphereApplication();
	if (app->startup() == true) 
	{
		app->run();
		
	}
	app->shutdown();
	
	system("pause");
	return 0;
}