#include "MyApplication.h"

int main()
{
	Runner* StarSystem = new MyApplication();

	if (StarSystem->startup() == true)
	{
		while (StarSystem->update() == true)
		{
			StarSystem->draw();
		}
		StarSystem->shutdown();
	}

	delete StarSystem;
	return 0;
}