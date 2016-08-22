#include <iostream>
#include "src\gl_core_4_4.h"
#include <GLFW\glfw3.h>
#include "src\Gizmos.h"
#include <glm.hpp>
#include <ext.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::rotate;

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

	mat4 view = glm::lookAt(vec3(0, 4, 20), vec3(0), vec3(0, 2, 0));
	mat4 projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);

	mat4 Star = mat4(1);

	mat4 ExoPlanet = mat4(1);
	
	mat4 Rings = mat4(1);

	mat4 Satellite = mat4(1);
	
	float angle = 0.0f;

	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();
	printf("GL: %i.%i\n", major, minor);

	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);

	while (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Gizmos::clear();

		angle += 0.01f;

		mat4 angled = rotate(angle, vec3(0, 1, 0));

		Star = Star * angled;

		ExoPlanet = Star * glm::translate(vec3(5, 0, 6)) * rotate(72.0f, vec3(0, 1, 0));

		Rings = rotate(32.0f, vec3(0, 0, 1));

		Satellite = ExoPlanet * glm::translate(vec3(3, 0, 0)) * rotate(23.0f, vec3(0, 1, 0));

		Gizmos::addSphere(vec3(Star[3]), 1, 20, 20, vec4(1, 1, 0, 1), &Star);
		Gizmos::addSphere(vec3(ExoPlanet[3]), 0.5f, 15, 15, vec4(0, 0.39f, 0, 1), &ExoPlanet);
		Gizmos::addRing(vec3(ExoPlanet[3]), 1.5f, 0.75f, 50, vec4(0.54f, 0.27f, 0.07f, 1), &Rings);
		Gizmos::addSphere(vec3(Satellite[3]), 0.25f, 10, 10, vec4(0.25f, .25f, .25f, 1), &Satellite);
		
		vec4 white(1);
		vec4 red(1, 0, 0, 1);

		for (int i = 0; i < 21; i += 1)
		{
			Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10), i == 10 ? white : red);
			Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i), i == 10 ? white : red);
		}
		
		
		
		Gizmos::draw(projection * view);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	Gizmos::destroy();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}