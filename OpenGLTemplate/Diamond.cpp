#include "Diamond.h"

CDiamond::CDiamond()
{}

CDiamond::~CDiamond()
{
	Release();
}

class myVertex {
public:
	GLfloat _x;
	GLfloat _y;
	GLfloat _z;
	GLfloat _r;
	GLfloat _g;
	GLfloat _b;

	myVertex(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b) {
		_x = x;
		_y = y;
		_z = z;
		_r = r;
		_g = g;
		_b = b;

	}
};

struct triangle {
	myVertex a;
	myVertex b;
	myVertex c;
};

void CDiamond::Create(string filename)
{
	m_tTexture.Load(filename);
	m_tTexture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	m_tTexture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_tTexture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_tTexture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenVertexArrays(1, &m_uiVAO);
	glBindVertexArray(m_uiVAO);
	m_VBO.Create();
	m_VBO.Bind();

	// Write the code to add interleaved point, texture coord, and normal of the diamond
	// TOP: 
	// left
	glm::vec3 v0 = glm::vec3(-1, 0, 0);		// 1
	glm::vec3 v1 = glm::vec3(0, 0, 1);		// 2
	glm::vec3 v2 = glm::vec3(0, 1, 0);		// 5

	// front
	glm::vec3 v3 = glm::vec3(0, 0, 1);		// 2 
	glm::vec3 v4 = glm::vec3(1, 0, 0);		// 3
	glm::vec3 v5 = glm::vec3(0, 1, 0);		// 5

	// right
	glm::vec3 v6 = glm::vec3(1, 0, 0);		// 3
	glm::vec3 v7 = glm::vec3(0, 0, -1);		// 4
	glm::vec3 v8 = glm::vec3(0, 1, 0);		// 5

	// back
	glm::vec3 v9 = glm::vec3(0, 0, -1);		// 4
	glm::vec3 v10 = glm::vec3(-1, 0, 0);	// 1
	glm::vec3 v11 = glm::vec3(0, 1, 0);		// 5



	glm::vec3 n = glm::vec3(0, 0, 1);

	//front 
	glm::vec2 t0 = glm::vec2(0, 0);
	glm::vec2 t1 = glm::vec2(1, 0);
	glm::vec2 t2 = glm::vec2(.5f, 1);

	glm::vec2 t3 = glm::vec2(0, 0);
	glm::vec2 t4 = glm::vec2(1, 0);
	glm::vec2 t5 = glm::vec2(.5f, 1);

	glm::vec2 t6 = glm::vec2(0, 0);
	glm::vec2 t7 = glm::vec2(1, 0);
	glm::vec2 t8 = glm::vec2(.5f, 1);

	glm::vec2 t9 = glm::vec2(0, 0);
	glm::vec2 t10 = glm::vec2(1, 0);
	glm::vec2 t11 = glm::vec2(.5f, 1);


	m_VBO.AddData(&v0, sizeof(glm::vec3));
	m_VBO.AddData(&t0, sizeof(glm::vec2));
	m_VBO.AddData(&n, sizeof(glm::vec3));

	m_VBO.AddData(&v1, sizeof(glm::vec3));
	m_VBO.AddData(&t1, sizeof(glm::vec2));
	m_VBO.AddData(&n, sizeof(glm::vec3));

	m_VBO.AddData(&v2, sizeof(glm::vec3));
	m_VBO.AddData(&t2, sizeof(glm::vec2));
	m_VBO.AddData(&n, sizeof(glm::vec3));



	m_VBO.AddData(&v3, sizeof(glm::vec3));
	m_VBO.AddData(&t3, sizeof(glm::vec2));
	m_VBO.AddData(&n, sizeof(glm::vec3));


	m_VBO.AddData(&v4, sizeof(glm::vec3));
	m_VBO.AddData(&t4, sizeof(glm::vec2));
	m_VBO.AddData(&n, sizeof(glm::vec3));


	m_VBO.AddData(&v5, sizeof(glm::vec3));
	m_VBO.AddData(&t5, sizeof(glm::vec2));
	m_VBO.AddData(&n, sizeof(glm::vec3));


	m_VBO.AddData(&v6, sizeof(glm::vec3));
	m_VBO.AddData(&t6, sizeof(glm::vec2));
	m_VBO.AddData(&n, sizeof(glm::vec3));


	m_VBO.AddData(&v7, sizeof(glm::vec3));
	m_VBO.AddData(&t7, sizeof(glm::vec2));
	m_VBO.AddData(&n, sizeof(glm::vec3));


	m_VBO.AddData(&v8, sizeof(glm::vec3));
	m_VBO.AddData(&t8, sizeof(glm::vec2));
	m_VBO.AddData(&n, sizeof(glm::vec3));

	m_VBO.AddData(&v9, sizeof(glm::vec3));
	m_VBO.AddData(&t9, sizeof(glm::vec2));
	m_VBO.AddData(&n, sizeof(glm::vec3));
	
	m_VBO.AddData(&v10, sizeof(glm::vec3));
	m_VBO.AddData(&t10, sizeof(glm::vec2));
	m_VBO.AddData(&n, sizeof(glm::vec3));
	
	m_VBO.AddData(&v11, sizeof(glm::vec3));
	m_VBO.AddData(&t11, sizeof(glm::vec2));
	m_VBO.AddData(&n, sizeof(glm::vec3));



	// Upload data to GPU
	m_VBO.UploadDataToGPU(GL_STATIC_DRAW);
	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);

	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));

	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));

}

void CDiamond::Render()
{
	glBindVertexArray(m_uiVAO);
	m_tTexture.Bind();

	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawArrays(GL_TRIANGLES, 3, 3);
	glDrawArrays(GL_TRIANGLES, 6, 3);
	glDrawArrays(GL_TRIANGLES, 9, 3);

}

void CDiamond::Release()
{
	m_tTexture.Release();
	glDeleteVertexArrays(1, &m_uiVAO);
	m_VBO.Release();
}
