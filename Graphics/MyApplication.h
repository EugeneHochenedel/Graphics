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
using glm::translate;
using namespace std;

#define GLM_SWIZZLE
#define GLM_FORCE_PURE

class MyApplication
{
public:
	MyApplication();
	bool startup();
	bool update();
	void draw();
	void shutdown();

	GLFWwindow* screen;
	mat4 view, projection, Star, Exoplanet, Satellite, Rings;
	
	float rotAngle;
	vec4 green, red, white, brown, grey, yellow;
};
