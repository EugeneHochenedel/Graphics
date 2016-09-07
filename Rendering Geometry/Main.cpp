#include "RenderingApplication.h"

Renderer::Renderer()
{
}

//Function to create a grid
void Renderer::generateGrid(unsigned int rows, unsigned int columns)
{
	Vertex* aoVertices = new Vertex[rows * columns];
	for (unsigned int i = 0; i < rows; i++)
	{
		for (unsigned int j = 0; j < columns; j++)
		{
			aoVertices[i * columns + j].position = glm::vec4((float)j, 0, (float)i, 1);
			
			//Create some arbitrary color based off something that might not be related to tiling a texture
			glm::vec3 colour = glm::vec3(sinf((j / (float)(columns - 1)) * (i / (float)(rows - 1))));
			aoVertices[i * columns + j].colour = glm::vec4(colour, 1);
		}
	}
	//More to come

	//Defines index count based off quad count (2 triangles per quad)
	//Creates an index array as the grid is being drawn
	unsigned int *auiIndices = new unsigned int[(rows - 1) * (columns - 1) * 6];

	unsigned int index = 0;
	for (unsigned int i = 0; i < (rows - 1); i++)
	{
		for (unsigned int j = 0; j < (columns - 1); j++)
		{
			//Triangle 1
			auiIndices[index++] = i * columns + j;
			auiIndices[index++] = (i + 1) * columns + j;
			auiIndices[index++] = (i + 1) * columns + (j + 1);

			//Triangle 2
			auiIndices[index++] = i * columns + j;
			auiIndices[index++] = (i + 1) * columns + (j + 1);
			auiIndices[index++] = i * columns + (i + 1);
		}
	}
	
	//Generates a vertex buffer object (VBO)
	glGenBuffers(1, &m_VBO);
	//Generates an index buffer object (IBO)
	glGenBuffers(1, &m_IBO);

	//Generates a vertex array object (VAO)
	glGenVertexArrays(1, &m_VAO);

	glBindVertexArray(m_VAO);

	//Binds and fills the VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, (rows * columns) * sizeof(Vertex), aoVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Binds and fills the IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (rows - 1) * (columns - 1) * 6 * sizeof(unsigned int), auiIndices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//Binds and fills the VAO
	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	delete[] aoVertices;
}

bool Renderer::startup()
{

}

bool Renderer::update()
{

}

void Renderer::draw()
{

}

void Renderer::shutdown()
{

}