#include <iostream>
#include "src\gl_core_4_4.h"
#include <GLFW\glfw3.h>
#include "src\Gizmos.h"
#include <glm.hpp>
#include <ext.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat4;

int main()
{
	if (glfwInit() == false)
	{
		return -1;
	}

	GLFWwindow* window = glfwCreateWindow(1280, 720, "Computer Graphics", nullptr, nullptr);

	if (window == nullptr)
	{
		glfwTerminate();
		return -2;
	}

	glfwMakeContextCurrent(window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(window);
		glfwTerminate();
		return -3;
	}

	Gizmos::create();

	mat4 view = glm::lookAt(vec3(0, 20, 20), vec3(0), vec3(0, 2, 0));
	mat4 projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);

	mat4 Star = mat4(1);
	mat4 ExoPlanet = mat4(1);
	mat4 Satellite = mat4(1);

	vec3 ExoPlanetPos = vec3(4, 4, 3);
	vec3 SatellitePos = vec3(2, 0, 3);

	
	

	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();
	printf("GL: %i.%i\n", major, minor);

	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);

	while (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Gizmos::clear();
		
		

		vec4 white(1);
		vec4 red(1, 0, 0, 1);
		vec4 blue(0, 0, 1, 1);

		for (int i = 0; i < 21; i += 1)
		{
			Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10), i == 10 ? white : red);
			Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i), i == 10 ? white : red);
		}
		
		Gizmos::addSphere(vec3(0, 0, 0), 1, 50, 50, blue);
		Gizmos::addSphere(vec3(4, 0, 3), 0.5f, 50, 50, vec4(1));
		Gizmos::addSphere(vec3(6, 0, 3), 1, 50, 50, vec4(0.25f, .25f, .25f, 1));
		
		Gizmos::draw(projection * view);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	Gizmos::destroy();

	glfwDestroyWindow(window);
	glfwTerminate();
	system("pause");
	return 0;
}