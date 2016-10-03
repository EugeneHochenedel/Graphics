#include "GenerationApplication.h"
#include <stb_image.h>

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

	view = glm::lookAt(glm::vec3(0, 10, 25), glm::vec3(0), glm::vec3(0, 1, 0));
	projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.0f, 0.1f, 1000.0f);

	glEnable(GL_BLEND);
	glClearColor(0.25f, 0.25f, 0.25f, 1);
	glEnable(GL_DEPTH_TEST);
}

bool Procedural::startup()
{
	int dims = 25;
	float* perlin_data = new float[dims * dims];
	float scale = (1.0f / dims) * 3;

	for (int x = 0; x < 25; x++)
	{
		for (int y = 0; y < 25; y++)
		{
			perlin_data[y * dims + x] = glm::perlin(glm::vec2(x, y) * scale) * 0.5f + 0.5f;
		}
	}

	glGenTextures(1, &m_perlin_texture);
	glBindTexture(GL_TEXTURE_2D, m_perlin_texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, 25, 25, 0, GL_RED, GL_FLOAT, perlin_data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	stbi_image_free(perlin_data);

	const char* vsSource;
	std::string vertShader = ReadIn("vertShader.txt");
	vsSource = vertShader.c_str();

	const char* fsSource;
	std::string fragShader = ReadIn("fragShader.txt");
	fsSource = fragShader.c_str();

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
	
	int projectionViewUniform = glGetUniformLocation(m_programID, "projectionViewWorldMatrix");
	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(m_projectionViewMatrix));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_perlin_texture);

	projectionViewUniform = glGetUniformLocation(m_programID, "perlin_texture");
	glUniform1i(projectionViewUniform, 0);

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, indexCounter, GL_UNSIGNED_INT, 0);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
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
	int rows = 25;
	int columns = 25;

	Vertex* verts = new Vertex[rows * columns];
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < columns; c++)
		{
			verts[r * columns + c].position = glm::vec4(c - columns * 0.5f, 0, r - rows * 0.5f, 1);
			verts[r * rows + c].UV = glm::vec2(c *(1.0f / columns), c *(1.0f / rows));
		}
	}

	indexCounter = (rows - 1) * (columns - 1) * 6;
	unsigned int* auiIndex = new unsigned int[indexCounter];

	unsigned int Index = 0;
	for (int r = 0; r < (rows - 1); r++)
	{
		for (int c = 0; c < (columns - 1); c++)
		{
			auiIndex[Index++] = r * columns + c;
			auiIndex[Index++] = (r + 1) * columns + c;
			auiIndex[Index++] = (r + 1) * columns + (c + 1);

			auiIndex[Index++] = r * columns + c;
			auiIndex[Index++] = (r + 1) * columns + (c + 1);
			auiIndex[Index++] = r * columns + (c + 1);
		}
	}

	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);
	glGenVertexArrays(1, &m_VAO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, (rows * columns) * sizeof(Vertex), verts, GL_STATIC_DRAW);

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

//float Procedural::noise()
//{
//	/*int dims = 25;
//	float* perlin_data = new float[dims * dims];
//	float scale = (1.0f / dims) * 3;
//
//	for (int x = 0; x < 25; x++)
//	{
//		for (int y = 0; y < 25; y++)
//		{
//			perlin_data[y* dims + x] = glm::perlin(glm::vec2(x, y) * scale) * 0.5f + 0.5f;
//		}
//	}*/
//}