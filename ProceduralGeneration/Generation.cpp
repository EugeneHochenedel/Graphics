#define STB_IMAGE_IMPLEMENTATION
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

	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();
	printf("GL: %i.%i\n", major, minor);

	glEnable(GL_BLEND);
	glClearColor(0.25f, 0.25f, 0.25f, 1);
	glEnable(GL_DEPTH_TEST);
}

bool Procedural::startup()
{
	int x = 25, z = 25;
	int dims = 24;

	int imageWidth = dims, imageHeight = dims, imageFormat = dims;
	
	float* perlin_data = noise(x);

	printf("Perlin Value: %i\n", perlin_data);

	glGenTextures(1, &m_perlin_texture);
	glBindTexture(GL_TEXTURE_2D, m_perlin_texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, dims, dims, 0, GL_RED, GL_FLOAT, perlin_data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	stbi_image_free(perlin_data);

	unsigned char* data = stbi_load("data/textures/dirt_grass.tga", &imageWidth, &imageHeight, &imageFormat, STBI_default);

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, imageWidth, imageHeight, 0, GL_RED, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_image_free(data);

	data = stbi_load("data/textures/rocky_ground.tga", &imageWidth, &imageHeight, &imageFormat, STBI_default);

	glGenTextures(1, &m_texture1);
	glBindTexture(GL_TEXTURE_2D, m_texture1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_image_free(data);

	data = stbi_load("data/textures/sand.tga", &imageWidth, &imageHeight, &imageFormat, STBI_default);

	glGenTextures(1, &m_texture2);
	glBindTexture(GL_TEXTURE_2D, m_texture2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_image_free(data);

	const char* vsSource;
	std::string vertShader = ReadIn("vertShader.txt");
	vsSource = vertShader.c_str();

	const char* fsSource;
	std::string fragShader = ReadIn("fragShader.txt");
	fsSource = fragShader.c_str();
	
	planeBuffer(x, z);

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

	
	m_projectionViewMatrix = projection * view;
	
	int projectionViewUniform = glGetUniformLocation(m_programID, "projectionViewWorldMatrix");
	glUniformMatrix4fv(projectionViewUniform, 1, GL_FALSE, glm::value_ptr(m_projectionViewMatrix));

	int tex = glGetUniformLocation(m_programID, "projectionViewWorldMatrix");
	glUniformMatrix4fv(tex, 1, GL_FALSE, glm::value_ptr(m_projectionViewMatrix));

	int tex1 = glGetUniformLocation(m_programID, "projectionViewWorldMatrix");
	glUniformMatrix4fv(tex1, 1, GL_FALSE, glm::value_ptr(m_projectionViewMatrix));

	int tex2 = glGetUniformLocation(m_programID, "projectionViewWorldMatrix");
	glUniformMatrix4fv(tex2, 1, GL_FALSE, glm::value_ptr(m_projectionViewMatrix));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_perlin_texture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_texture1);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_texture2);

	projectionViewUniform = glGetUniformLocation(m_programID, "perlin_texture");
	glUniform1i(projectionViewUniform, 0);

	tex = glGetUniformLocation(m_programID, "grass_texture");
	glUniform1i(tex, 1);

	tex1 = glGetUniformLocation(m_programID, "rocks_texture");
	glUniform1i(tex1, 2);

	tex2 = glGetUniformLocation(m_programID, "snow_texture");
	glUniform1i(tex2, 3);

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, indexCounter, GL_UNSIGNED_INT, nullptr);
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

void Procedural::planeBuffer(const int& x, const int& z)
{
	Vertex* verts = new Vertex[x * z];
	for (int r = 0; r < x; r++)
	{
		for (int c = 0; c < z; c++)
		{
			verts[r * z + c].position = glm::vec4(r - x * 0.5f, 0, c - z * 0.5f, 1);
			verts[r * z + c].UV = glm::vec2(r * (1.0f / x), c * (1.0f / z));
		}
	}

	indexCounter = (x - 1) * (z - 1) * 6;
	unsigned int* auiIndex = new unsigned int[indexCounter];

	unsigned int Index = 0;
	for (int r = 0; r < (x - 1); r++)
	{
		for (int c = 0; c < (z - 1); c++)
		{
			auiIndex[Index++] = r * z + c;
			auiIndex[Index++] = (r + 1) * z + c;
			auiIndex[Index++] = (r + 1) * z + (c + 1);

			auiIndex[Index++] = r * z + c;
			auiIndex[Index++] = (r + 1) * z + (c + 1);
			auiIndex[Index++] = r * z + (c + 1);
		}
	}

	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);
	glGenVertexArrays(1, &m_VAO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, (x * z) * sizeof(Vertex), verts, GL_STATIC_DRAW);

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

float* Procedural::noise(const int& columns)
{
	int dims = columns - 1;
	float* perlin_data = new float[dims * dims];
	float scale = (1.0f / dims) * 3;
	int octaves = 6;

	for (int i = 0; i < dims; i++)
	{
		for (int j = 0; j < dims; j++)
		{
			float amplitude = 1.0f;
			float persistence = 0.3f;
			perlin_data[j * dims + i] = 0;

			for (int ii = 0; ii < octaves; ii++)
			{
				float frequency = powf(2, (float)ii);
				float perlin_sample = glm::perlin(glm::vec2((float)i, (float)j) * scale * frequency) * 0.5f + 0.5f;
				perlin_data[j * dims + i] += perlin_sample * amplitude;
				amplitude *= persistence;
			}
		}
	}
	return perlin_data;
}