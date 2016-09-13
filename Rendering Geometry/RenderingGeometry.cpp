#include "RenderingApplication.h"

Renderer::Renderer()
{
	glfwInit();
	screen = glfwCreateWindow(1280, 720, "Rendering Geometry", nullptr, nullptr);

	if (screen == nullptr)
	{
		glfwTerminate();
	}

	glfwMakeContextCurrent(screen);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(screen);
		glfwTerminate();
	}

	view = glm::lookAt(glm::vec3(10, 10, 10), glm::vec3(0), glm::vec3(0, 1, 0));
	projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.0f, 0.1f, 1000.0f);

	glClearColor(0.25f, 0.25f, 0.25f, 1);
	glEnable(GL_DEPTH_TEST);

}

//Function to create a grid
void Renderer::generateGrid(unsigned int rows, unsigned int columns)
{
	Vertex aoPoints[4];
	unsigned int auiIndex[4] = { 0, 1, 2, 3 };

	aoPoints[0].position = glm::vec4(2, 0, 4, 0);
	aoPoints[1].position = glm::vec4(2, 0, -4, 0);
	aoPoints[2].position = glm::vec4(-4, 0, -4, 0);
	aoPoints[3].position = glm::vec4(-4, 0, 4, 0);
	
	aoPoints[0].colour = glm::vec4(1, 0, 0, 1);
	aoPoints[1].colour = glm::vec4(0, 1, 0, 1);
	aoPoints[2].colour = glm::vec4(0, 0, 1, 1);
	aoPoints[3].colour = glm::vec4(1, 1, 1, 1);

	//Generates a vertex buffer object (VBO)
	glGenBuffers(1, &m_VBO);
	
	//Generates an index buffer object (IBO)
	glGenBuffers(1, &m_IBO);
	
	//Generates a vertex array object (VAO)
	glGenVertexArrays(1, &m_VAO);
	
	//Binds the VAO
	glBindVertexArray(m_VAO);

	//Binds and fills the VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), aoPoints, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4)));

	//Binds and fills the IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(unsigned int), auiIndex, GL_STATIC_DRAW);
	
	//Binds the arrays
	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//delete[] aoVertices, auiIndices;
}

bool Renderer::startup()
{
	
	//Creates shaders
	//Stores written out shader code in character arrays to be processed by OpenGL
	//"in" specifies input to the shader
	//"out" specifies output by the shader that are read as input later in the render pipeline
	//"uniform" specifies a parameter that is sent in from the program before the shader has run
	const char *vsSource = "#version 410\n \
							layout(location=0) in vec4 position; \
							layout(location=1) in vec4 colour; \
							out vec4 vColour; \
							uniform mat4 projectionViewWorldMatrix; \
							void main() { vColour = colour; gl_Position = projectionViewWorldMatrix * position; }";

	const char *fsSource = "#version 410\n \
							in vec4 vColour; \
							out vec4 fragColor; \
							void main() { fragColor = vColour; }";

	//Compiles the shader
	int success = GL_FALSE;
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(vertexShader);
	glShaderSource(fragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(fragmentShader);

	m_programID = glCreateProgram();
	glAttachShader(m_programID, vertexShader);
	glAttachShader(m_programID, fragmentShader);
	glLinkProgram(m_programID);

	glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char *infoLog = new char[infoLogLength];

		glGetProgramInfoLog(m_programID, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}

	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	return true;
}

bool Renderer::update()
{
	while (glfwWindowShouldClose(screen) == false && glfwGetKey(screen, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		return true;
	}

	return false;
}

void Renderer::draw()
{
	unsigned int rows = 4;
	unsigned int columns = 4;
	generateGrid(rows, columns);
	m_projectionViewMatrix = projection * view;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_programID);
	unsigned int projectionViewUniform = glGetUniformLocation(m_programID, "projectionViewWorldMatrix");
	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(m_projectionViewMatrix));

	glBindVertexArray(m_VAO);
	indexCounter = (rows - 1) * (columns - 1) * 6;
	glDrawElements(GL_TRIANGLES, indexCounter, GL_UNSIGNED_INT, 0);

	glfwSwapBuffers(screen);
	glfwPollEvents();
}

void Renderer::shutdown()
{
	glfwDestroyWindow(screen);
	glfwTerminate();
}