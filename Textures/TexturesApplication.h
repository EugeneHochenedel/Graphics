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

class Application
{
public:
	virtual bool startup() = 0;
	virtual bool update() = 0;
	virtual void draw() = 0;
	virtual void shutdown() = 0;
};

class Texture : public Application
{
public:
	Texture();
	bool startup() override;
	bool update() override;
	void draw() override;
	void shutdown() override;
	void createQuad();
	std::string ReadIn(std::string);

private:
	GLFWwindow* screen;
	glm::mat4 view, projection, m_projectionViewMatrix;

	unsigned int m_VAO, m_VBO, m_IBO;
	unsigned int m_programID;
	unsigned int m_texture, m_texture2;
};