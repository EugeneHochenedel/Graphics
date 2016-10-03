#pragma once

#include <iostream>
#include "src\gl_core_4_4.h"
#include "src\Gizmos.h"
#include <GLFW\glfw3.h>
#include <glm.hpp>
#include <ext.hpp>
#include <math.h>
#include <vector>
#include <fstream>
#include <string>

struct Vertex
{
	glm::vec4 position;
	glm::vec2 UV;
};

class Application
{
public:
	virtual bool startup() = 0;
	virtual bool update() = 0;
	virtual void draw() = 0;
	virtual void shutdown() = 0;
};

class Procedural : public Application
{
public:
	Procedural();
	bool startup() override;
	bool update() override;
	void draw() override;
	void shutdown() override;

	std::string ReadIn(std::string);
	
	void planeBuffer();
	
private:
	GLFWwindow* screen;

	glm::mat4 view, projection, m_projectionViewMatrix;

	int indexCounter;

	unsigned int m_VAO, m_VBO, m_IBO;
	unsigned int m_programID;

};
