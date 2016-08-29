#pragma once

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

//#define GLM_SWIZZLE
//#define GLM_FORCE_PURE

class Runner
{
public:
	virtual bool startup() = 0;
	virtual bool update() = 0;
	virtual void draw() = 0;
	virtual void shutdown() = 0;

};

class MyApplication : public Runner
{
public:
	MyApplication();
	bool startup() override;
	bool update() override;
	void draw() override;
	void shutdown() override;

	GLFWwindow* screen;
	mat4 view, projection, Star, Exoplanet, Satellite, Rings;
	
	float rotAngle;
	vec4 green, red, white, brown, grey, yellow;
};
