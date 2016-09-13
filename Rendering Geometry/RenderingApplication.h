#pragma once

#include <iostream>
#include <math.h>
#include <vector>
#include <list>
#include "src\gl_core_4_4.h"
#include <GLFW\glfw3.h>
#include "src\Gizmos.h"
#include <glm.hpp>
#include <ext.hpp>

#define GLM_SWIZZLE
#define GLM_FORCE_PURE

struct Vertex
{
	glm::vec4 position;
	glm::vec4 colour;
};


class Applic
{
public:
	virtual bool startup() = 0;
	virtual bool update() = 0;
	virtual void draw() = 0;
	virtual void shutdown() = 0;
};

class Renderer : public Applic
{
public:
	Renderer();
	bool startup() override;
	bool update() override;
	void draw() override;
	void shutdown() override;


	void generateGrid();
	//void generateCube();
	std::vector<glm::vec4> generateHalfPoints(int, float);
	std::vector<unsigned int> generateIndices(int, int);
	void generateHalfCircle();
	//void generateSphere();

private:
	glm::mat4 view, projection, m_projectionViewMatrix;

	GLFWwindow *screen;

	//Vertex and Index buffers
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;

	//ID of compiled shader
	unsigned int m_programID;

	unsigned int indexCounter;
};