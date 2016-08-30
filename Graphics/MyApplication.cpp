#include "MyApplication.h"

MyApplication::MyApplication()
{

}

bool MyApplication::startup()
{
	white = vec4(1);
	red = vec4(1, 0, 0, 1);
	yellow = vec4(1, 1, 0, 1);
	green = vec4(0, 0.39f, 0, 1);
	brown = vec4(0.54f, 0.27f, 0.07f, 0.5f);
	grey = vec4(0.25f, 0.25f, 0.25f, 1);

	

	if (glfwInit() == false)
	{
		return false;
	}

	screen = glfwCreateWindow(1280, 720, "Intro to OpenGL", nullptr, nullptr);
	
	if (screen == nullptr)
	{
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(screen);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(screen);
		glfwTerminate();
		return false;
	}

	Gizmos::create();

	float previousTime;
	float currentTime = (float)glfwGetTime();
	previousTime = currentTime;
	float deltaTime = currentTime - previousTime;
	
	view = glm::lookAt(vec3(0, 4, 20), vec3(5, 2, 1), vec3(0, 2, 0));
	projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.0f, 0.1f, 1000.0f);


	inverted = glm::inverse(view);

	//mat4 secondInvert = glm::inverse(inverted);

	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();
	printf("GL: %i.%i\n", major, minor);

	/*printf("Inverted: %f, %f, %f, %f\n", inverted[0][0], inverted[0][1], inverted[0][2], inverted[0][3]);
	printf("Inverted: %f, %f, %f, %f\n", inverted[1][0], inverted[1][1], inverted[1][2], inverted[1][3]);
	printf("Inverted: %f, %f, %f, %f\n", inverted[2][0], inverted[2][1], inverted[2][2], inverted[2][3]);
	printf("Inverted: %f, %f, %f, %f\n", inverted[3][0], inverted[3][1], inverted[3][2], inverted[3][3]);*/

	for (int i = 0; i < 4; i++)
	{
		cout << "{ ";
		for (int j = 0; j < 4; j++)
		{
			printf("%f ", inverted[i][j]);
		}
		cout << "} \n";
	}

	//vec3 newTranslate = translate(inverted[0]);
	cout << "\n\n\n";

	cout << glm::to_string(inverted) << endl;


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