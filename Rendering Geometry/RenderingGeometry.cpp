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

bool Renderer::startup()
{
	generateGrid();
	//Creates shaders
	//Stores written out shader code in character arrays to be processed by OpenGL
	//"in" specifies input to the shader
	//"out" specifies output by the shader that are read as input later in the render pipeline
	//"uniform" specifies a parameter that is sent in from the program before the shader has run
	const char* vsSource;
	std::string vertShader = ReadIn("vsSource.txt");
	vsSource = vertShader.c_str();

	const char* fsSource;
	std::string fragShader = ReadIn("fsSource.txt");
	fsSource = fragShader.c_str();

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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	m_projectionViewMatrix = projection * view;

	glUseProgram(m_programID);
	unsigned int projectionViewUniform = glGetUniformLocation(m_programID, "projectionViewWorldMatrix");
	glUniformMatrix4fv(projectionViewUniform, 1, GL_FALSE, glm::value_ptr(m_projectionViewMatrix));
	
	//generateHalfCircle();
	glBindVertexArray(m_VAO);
	//glPointSize(5.0f);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLE_STRIP, indexCounter, GL_UNSIGNED_INT, nullptr);
	
	/*generateHalfCircle();
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, indexCounter, GL_UNSIGNED_INT, 0);*/
	
	glfwSwapBuffers(screen);
	glfwPollEvents();
}

void Renderer::shutdown()
{
	glfwDestroyWindow(screen);
	glfwTerminate();
}

std::string Renderer::ReadIn(std::string fileName)
{
	std::string info;
	std::string container;
	std::ifstream file(fileName);

	if (file.is_open())
	{
		while (std::getline(file, info))
		{
			container += info + "\n";
		}
		file.close();
	}
	return container;
}

//Function to create a plane
void Renderer::generateGrid()
{
	Vertex aoPoints[4];
	unsigned int auiIndex[4] = { 0, 1, 2, 3 };

	indexCounter = 4;

	aoPoints[0].position = glm::vec4(4, 0, 4, 1);
	aoPoints[1].position = glm::vec4(-4, 0, 4, 1);
	aoPoints[2].position = glm::vec4(4, 0, -4, 1);
	aoPoints[3].position = glm::vec4(-4, 0, -4, 1);

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
}


//void Renderer::generateCube()
//{
//	Vertex Aside[8];
//
//	unsigned int indecies1[4] = { 0, 2, 1, 3 };
//}
//
//Vertex* Renderer::generateHalfCirclePoints(unsigned int points, float radius)
//{
//	Vertex* vertices = new Vertex[points];
//	for (int i = 0; i < points; i++)
//	{
//		float angle = glm::pi<float>() * (i / (points - 1));
//		vertices[i].position = glm::vec4(cos(angle) * radius, sin(angle) * radius, 0, 1);
//		//vertices.push_back(location);
//	}
//	return vertices;
//}
//
//Vertex* Renderer::generateCircPoints(unsigned int sides, unsigned int miridians, Vertex* &halfSphere)
//{
//	int count = 0;
//	Vertex* verts = new Vertex[sides * miridians];
//
//	for (int i = 0; i < miridians; i++)
//	{
//		float phi = 2.0f * glm::pi<float>() * ((float)i / (float)miridians);
//		for (int j = 0; j < sides; j++, count++)
//		{
//			float x = halfSphere[j].position.x;
//			float y = halfSphere[j].position.y * cos(phi) - halfSphere[j].position.z * sin(phi);
//			float z = halfSphere[j].position.z * cos(phi) + halfSphere[j].position.y * sin(phi);
//
//			verts[count].position = glm::vec4(x, y, z, 1);
//			verts[count].colour = glm::vec4(1, 0, 0, 1);
//		}
//	}
//	return verts;
//}
//
//unsigned int* Renderer::generateIndices(unsigned int vertices, unsigned int miridians)
//{
//	unsigned int* indices = new unsigned int[2 * (vertices * (miridians + 1))];
//	indexCounter = 2 * (vertices * (miridians + 1));
//
//	for (unsigned int i = 0; i < miridians; i++)
//	{
//		unsigned int starter = i * vertices;
//		for (int j = 0; j < vertices; j++)
//		{
//			unsigned int botR = ((starter + vertices + j) % (vertices * miridians));
//			unsigned int botL = ((starter + j) % (vertices*miridians));
//			indicesHolder.push_back(botL);
//			indicesHolder.push_back(botR);
//		}
//		indicesHolder.push_back(0xFFFF);
//	}
//
//	for (int i = 0; i < indicesHolder.size(); i++)
//	{
//		indices[i] = indicesHolder[i];
//	}
//	return indices;
//}
//
//void Renderer::generateHalfCircle()
//{
//	unsigned int radians = 5;
//	unsigned int parts = 5;
//	float dist = 2.0f;
//	unsigned int size = (parts) * (radians);
//
//	Vertex* vertices = new Vertex[size];
//	unsigned int* indices;
//
//	Vertex* aoPoints = generateHalfCirclePoints(radians, dist);
//	vertices = generateCircPoints(radians, parts, aoPoints);
//	indices = generateIndices(radians, parts);
//	//std::vector<unsigned int> auiIndex = generateIndices(parts, radians);
//
//	//Vertex Temps[aoPoints.size];
//	//Temps.position = aoPoints.data();
//
//	glGenBuffers(1, &m_VBO);
//	glGenBuffers(1, &m_IBO);
//	glGenBuffers(1, &m_VAO);
//
//	glBindVertexArray(m_VAO);
//
//	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
//	glBufferData(GL_ARRAY_BUFFER, size * sizeof(Vertex), vertices, GL_STATIC_DRAW);
//
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(glm::vec4)));
//
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCounter * sizeof(unsigned int), indices, GL_STATIC_DRAW);
//
//	glBindVertexArray(0);
//
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//}

void Renderer::halfCircle(const int& points, const int& radius)
{
	int pieces = points - 1;

	Vertex* aoPoints = new Vertex[points];
	for (int i = 0; i < points; i++)
	{
		double theta = glm::pi<float>() * i / points;
	}
}

unsigned int* Renderer::sphereIndex(const int& divisions, const int& points)
{
	indexCounter = 2 * (points * (divisions + 1));
	unsigned int* Index = new unsigned int[indexCounter];

	for (int i = 0; i < divisions; i++)
	{
		unsigned int start = i * points;
		for (int j = 0; j < points; j++)
		{
			unsigned int botR = ((start + points + j) % (points * divisions));
			unsigned int botL = ((start + j) % (points * divisions));
			indicesHolder.push_back(botL);
			indicesHolder.push_back(botR);
		}
		//Index.push_back(0xFFFF);
	}
	
	for (int i = 0; i < indicesHolder.size(); i++)
	{
		Index[i] = indicesHolder[i];
	}

	return Index;
}