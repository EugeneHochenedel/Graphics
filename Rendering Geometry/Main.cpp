#include "RenderingApplication.h"

int main()
{
	Applic* Geometry = new Renderer();

	if (Geometry->startup() == true)
	{
		while (Geometry->update() == true)
		{
			Geometry->draw();
		}
		Geometry->shutdown();
	}

	delete Geometry;
	return 0;
}