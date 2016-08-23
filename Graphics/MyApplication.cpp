#include "MyApplication.h"

MyApplication::MyApplication()
{
}

bool MyApplication::startup()
{
	if (glfwInit() == false)
	{
		return -1;
	}

	screen = glfwCreateWindow(1280, 720, "Intro to OpenGL", nullptr, nullptr);
	
	if (screen == nullptr)
	{
		glfwTerminate();
		return -2;
	}

	glfwMakeContextCurrent(screen);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(screen);
		glfwTerminate();
		return -3;
	}

	Gizmos::create();

	view = glm::lookAt(vec3(0, 4, 20), vec3(0), vec3(0, 2, 0));
	projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.0f, 0.1f, 1000.0f);

	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();
	printf("GL: %i.%i\n", major, minor);

	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);

	return true;
}

bool MyApplication::update()
{
	while (glfwWindowShouldClose(screen) == false && glfwGetKey(screen, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Gizmos::clear();

		rotAngle += 0.01f;

		mat4 Inclination = rotate(rotAngle, vec3(0, 1, 0));
		mat4 Inclination0 = rotate(rotAngle * 5, vec3(0, 1, 0));

		Star = mat4(1) * Inclination;

		Exoplanet = Star * translate(vec3(5, 0, 6)) * rotate(72.0f, vec3(0, 1, 0));

		Rings = rotate(Exoplanet, 32.0f, vec3(0, 0, 1));

		Satellite = (Exoplanet * Inclination0) * translate(vec3(3, 0, 0));

		return true;
	}

	return false;
}

void MyApplication::draw()
{
	white = vec4(1);
	red = vec4(1, 0, 0, 1);
	yellow = vec4(1, 1, 0, 1);
	green = vec4(0, 0.39f, 0, 1);
	brown = vec4(0.54f, 0.27f, 0.07f, 0.5f);
	grey = vec4(0.25f, 0.25f, 0.25f, 1);

	Gizmos::addSphere(vec3(Star[3]), 1, 20, 20, yellow, &Star);
	Gizmos::addSphere(vec3(Exoplanet[3]), 0.5f, 15, 15, green, &Exoplanet);
	Gizmos::addRing(vec3(Exoplanet[3]), 1.5f, 0.75f, 10, brown, &Rings);
	Gizmos::addSphere(vec3(Satellite[3]), 0.25f, 10, 10, grey, &Satellite);

	for (int i = 0; i < 21; i += 1)
	{
		Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10), i == 10 ? white : red);
		Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i), i == 10 ? white : red);
	}

	Gizmos::draw(projection * view);

	glfwSwapBuffers(screen);
	glfwPollEvents();
}

void MyApplication::shutdown()
{
	Gizmos::destroy();

	glfwDestroyWindow(screen);
	glfwTerminate();
}