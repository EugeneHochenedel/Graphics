#include "TexturesApplication.h"

int main()
{
	Application* Textures = new Texture();

	if (Textures->startup() == true)
	{
		while (Textures->update() == true)
		{
			Textures->draw();
		}
		Textures->shutdown();
	}

	delete Textures;
	return 0;
}