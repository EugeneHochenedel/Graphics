#pragma once

#include <iostream>
#include <math.h>
#include <vector>
#include "src\gl_core_4_4.h"
#include <GLFW\glfw3.h>
#include "src\Gizmos.h"
#include <glm.hpp>
#include <ext.hpp>
#include <string>
#include <fstream>

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

	std::string ReadIn(std::string);

	void generateGrid();
	unsigned int* sphereIndex(const int&, const int&);
	//void generateCube();
	//Vertex* generateHalfCirclePoints(unsigned int, float);
	//Vertex* generateCircPoints(unsigned int, unsigned int, Vertex* &halfSphere);
	//unsigned int* generateIndices(unsigned int, unsigned int);
	void halfCircle(const int&, const int&);
	//void generateSphere();
	std::vector<unsigned int> indicesHolder;

private:
	glm::mat4 view, projection, m_projectionViewMatrix;

	GLFWwindow *screen;

	//Vertex and Index buffers
	unsigned int m_VAO, m_VBO, m_IBO;

	//ID of compiled shader
	unsigned int m_programID;

	int indexCounter;
};