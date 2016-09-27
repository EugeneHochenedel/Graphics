#include <iostream>
#include "src\gl_core_4_4.h"
#include "src\Gizmos.h"
#include <GLFW\glfw3.h>
#include <glm.hpp>
#include <ext.hpp>

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
	void simpleQuad();

private:
	GLFWwindow* screen;
	glm::mat4 view, projection, m_projectionViewMAtrix;

	unsigned int m_VAO, m_VBO, m_IBO, m_programID, m_texture;
};