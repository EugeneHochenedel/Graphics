#include "GenerationApplication.h"

Procedural::Procedural()
{
	glfwInit();
	screen = glfwCreateWindow(1280, 720, "Procedural Generation", nullptr, nullptr);

	if (screen == nullptr)
	{
		glfwTerminate();
	}

	glfwMakeContextCurrent(screen);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(screen);
		glfwTerminate();
	}

	view = glm::lookAt(glm::vec3(10, 10, 10), glm::vec3(0), glm::vec3(0, 1, 0));
	projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.0f, 0.1f, 1000.0f);

	glEnable(GL_BLEND);
	glClearColor(0.25f, 0.25f, 0.25f, 1);
	glEnable(GL_DEPTH_TEST);
}

bool Procedural::startup()
{
	const char* vsSource;

	const char* fsSource;

	return true;
}

bool Procedural::update()
{
	while (glfwWindowShouldClose(screen) == false && glfwGetKey(screen, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		return true;
	}
	return false;
}

void Procedural::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_projectionViewMatrix = projection * view;
	
	glfwSwapBuffers(screen);
	glfwPollEvents();
}

void Procedural::shutdown()
{
	glfwDestroyWindow(screen);
	glfwTerminate();
}

std::string Procedural::ReadIn(std::string fileName)
{
	std::string info;
	std::string container;
	std::ifstream file(fileName);

	if (file.is_open())
	{
		while (std::getline(file, info))
		{
			container += info + "\n";
		}
		file.close();
	}
	return container;
}

void Procedural::createPlain()
{

}