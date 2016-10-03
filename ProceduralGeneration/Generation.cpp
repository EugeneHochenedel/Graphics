#include "GenerationApplication.h"

Procedural::Procedural()
{
	glfwInit();
	screen = glfwCreateWindow(1280, 720, "Procedural Generation", nullptr, nullptr);

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

bool Procedural::startup()
{
	

	const char* vsSource;
	std::string vertShader = ReadIn("vertShader.txt");
	vsSource = vertShader.c_str();

	const char* fsSource;
	std::string fragShader = ReadIn("fragShader.txt");
	fsSource = fragShader.c_str();

	int success = GL_FALSE;
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_VERTEX_SHADER);

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
		char* infoLog = new char[infoLogLength];

		glGetProgramInfoLog(m_programID, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}

	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	return true;
}

bool Procedural::update()
{
	while (glfwWindowShouldClose(screen) == false && glfwGetKey(screen, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		return true;
	}
	return false;
}

void Procedural::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_programID);

	planeBuffer();
	m_projectionViewMatrix = projection * view;
	
	unsigned int projectionViewUniform = glGetUniformLocation(m_programID, "projectionViewWorldMatrix");
	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(m_projectionViewMatrix));

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, indexCounter, GL_UNSIGNED_INT, 0);
	
	glfwSwapBuffers(screen);
	glfwPollEvents();
}

void Procedural::shutdown()
{
	glfwDestroyWindow(screen);
	glfwTerminate();
}

std::string Procedural::ReadIn(std::string fileName)
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

void Procedural::planeBuffer()
{
	int height = 25;
	int width = 25;

	Vertex* verts = new Vertex[height * width];
	for (int cols = 0; cols < width; cols++)
	{
		for (int rows = 0; rows < height; rows++)
		{
			verts[cols * height + rows].position = glm::vec4(rows - height * 0.5f, 0, cols - width * 0.5f, 1);
			verts[cols * height + rows].UV = glm::vec2(rows *(1.0f / height), cols *(1.0f / width));
		}
	}

	indexCounter = (height - 1) * (width - 1) * 6;
	unsigned int* auiIndex = new unsigned int[indexCounter];

	unsigned int Index = 0;
	for (int cols = 0; cols < (width - 1); cols++)
	{
		for (int rows = 0; rows < (height - 1); rows++)
		{
			auiIndex[Index++] = cols * height + rows;
			auiIndex[Index++] = (cols + 1) * height + rows;
			auiIndex[Index++] = (cols + 1) * height + (rows + 1);

			auiIndex[Index++] = cols * height + rows;
			auiIndex[Index++] = (cols + 1) * height + (rows + 1);
			auiIndex[Index++] = cols * height + (rows + 1);
		}
	}

	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);
	glGenVertexArrays(1, &m_VAO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, (height * width) * sizeof(Vertex), verts, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec4));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCounter * sizeof(unsigned int), auiIndex, GL_STATIC_DRAW);

	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}