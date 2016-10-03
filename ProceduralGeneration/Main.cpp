#include "GenerationApplication.h"

int main()
{
	Application* ProceduralGeneration = new Procedural();

	if (ProceduralGeneration->startup() == true)
	{
		while (ProceduralGeneration->update() == true)
		{
			ProceduralGeneration->draw();
		}
		ProceduralGeneration->shutdown();
	}

	delete ProceduralGeneration;
	return 0;
}