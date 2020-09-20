#include "Cube.h"

CCube::CCube()
{}

CCube::~CCube()
{
	Release();
}

// Code found from https://stackoverflow.com/questions/54677318/draw-a-cube-and-a-pyramid
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

void CCube::Create(string filename)
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

	// Write the code to add interleaved point, texture coord, and normal of the cube
	// front
	glm::vec3 v0 = glm::vec3(-2, -1, 1);	// 4
	glm::vec3 v1 = glm::vec3(2, -1,  1);		// 5
	glm::vec3 v2 = glm::vec3(2, 1,   1);		// 7
	glm::vec3 v3 = glm::vec3(-2, 1,  1);		// 6

	//left
	glm::vec3 v4 = glm::vec3(-2, -1, -1);		// 
	glm::vec3 v5 = glm::vec3(-2, -1, 1);		// = 0
	glm::vec3 v6 = glm::vec3(-2, 1, 1);			// = 3
	glm::vec3 v7 = glm::vec3(-2, 1, -1);		// 
	
	//back
	glm::vec3 v8 = glm::vec3(2, -1,  -1);		// 
	glm::vec3 v9 = glm::vec3(-2, -1, -1);		// = 4
	glm::vec3 v10 = glm::vec3(-2, 1, -1);		// = 7
	glm::vec3 v11 = glm::vec3(2, 1,  -1);		// 
	
	//right
	glm::vec3 v12 = glm::vec3(2, -1, 1);		// = 1
	glm::vec3 v13 = glm::vec3(2, -1, -1);		// = 8
	glm::vec3 v14 = glm::vec3(2, 1, -1);		// = 11
	glm::vec3 v15 = glm::vec3(2, 1, 1);			// = 2
	



	// house roof
	// front
	glm::vec3 v16 = glm::vec3(-2, 1, 1);		// 3
	glm::vec3 v17 = glm::vec3(2, 1, 1);			// 2
	glm::vec3 v18 = glm::vec3(0, 1.5f, 0);		// centre
	//glm::vec3 v19 = glm::vec3(0, 1.5f, 0);	// centre

	// left
	glm::vec3 v19 = glm::vec3(-2, 1, -1);		// 7
	glm::vec3 v20 = glm::vec3(-2, 1, 1);		// 3
	glm::vec3 v21 = glm::vec3(0, 1.5f, 0);		// centre

	// back
	glm::vec3 v22 = glm::vec3(2, 1, -1);		// 11
	glm::vec3 v23 = glm::vec3(-2, 1, -1);		// 7
	glm::vec3 v24 = glm::vec3(0, 1.5f, 0);		// centre

	// right
	glm::vec3 v25 = glm::vec3(2, 1, 1);			// 2
	glm::vec3 v26 = glm::vec3(2, 1, -1);		// 11
	glm::vec3 v27 = glm::vec3(0, 1.5f, 0);		// centre

	glm::vec3 n = glm::vec3(0, 0, 1);

	//front 
	glm::vec2 t0 = glm::vec2(0, 0);
	glm::vec2 t1 = glm::vec2(.5f, 0);
	glm::vec2 t2 = glm::vec2(.5f, .5f);
	glm::vec2 t3 = glm::vec2(0, .5f);

	//left
	glm::vec2 t4 = glm::vec2(0, .5f);
	glm::vec2 t5 = glm::vec2(.25f, .5f);
	glm::vec2 t6 = glm::vec2(.25f, 1);
	glm::vec2 t7 = glm::vec2(0, 1);

	//back
	glm::vec2 t8 = glm::vec2(0, .5f);
	glm::vec2 t9 = glm::vec2(.5f, .5f);
	glm::vec2 t10 = glm::vec2(.5f, 1);
	glm::vec2 t11 = glm::vec2(0, 1);

	//right
	glm::vec2 t12 = glm::vec2(0, .5f);
	glm::vec2 t13 = glm::vec2(.25f, .5f);
	glm::vec2 t14 = glm::vec2(.25f, 1);
	glm::vec2 t15 = glm::vec2(0, 1);

	// house roof
	glm::vec2 t16 = glm::vec2(.5f, .5f);
	glm::vec2 t17 = glm::vec2(1, .5f);
	glm::vec2 t18 = glm::vec2(0.75f, 1);

	glm::vec2 t19 = glm::vec2(.5f, .5f);
	glm::vec2 t20 = glm::vec2(1, .5f);
	glm::vec2 t21 = glm::vec2(0.75f, 1);
			   
	glm::vec2 t22 = glm::vec2(.5f, .5f);
	glm::vec2 t23 = glm::vec2(1, .5f);
	glm::vec2 t24 = glm::vec2(0.75f, 1);
			   
	glm::vec2 t25 = glm::vec2(.5f, .5f);
	glm::vec2 t26 = glm::vec2(1, .5f);
	glm::vec2 t27 = glm::vec2(0.75f, 1);

	m_VBO.AddData(&v0, sizeof(glm::vec3));
	m_VBO.AddData(&t0, sizeof(glm::vec2));
	m_VBO.AddData(&n, sizeof(glm::vec3));

	m_VBO.AddData(&v1, sizeof(glm::vec3));
	m_VBO.AddData(&t1, sizeof(glm::vec2));
	m_VBO.AddData(&n, sizeof(glm::vec3));

	m_VBO.AddData(&v3, sizeof(glm::vec3));
	m_VBO.AddData(&t3, sizeof(glm::vec2));
	m_VBO.AddData(&n, sizeof(glm::vec3));

	m_VBO.AddData(&v2, sizeof(glm::vec3));
	m_VBO.AddData(&t2, sizeof(glm::vec2));
	m_VBO.AddData(&n, sizeof(glm::vec3));
	   

	m_VBO.AddData(&v4, sizeof(glm::vec3));
	m_VBO.AddData(&t4, sizeof(glm::vec2));
	m_VBO.AddData(&n, sizeof(glm::vec3));

	m_VBO.AddData(&v5, sizeof(glm::vec3));
	m_VBO.AddData(&t5, sizeof(glm::vec2));
	m_VBO.AddData(&n, sizeof(glm::vec3));

	m_VBO.AddData(&v7, sizeof(glm::vec3));
	m_VBO.AddData(&t7, sizeof(glm::vec2));
	m_VBO.AddData(&n, sizeof(glm::vec3));

	m_VBO.AddData(&v6, sizeof(glm::vec3));
	m_VBO.AddData(&t6, sizeof(glm::vec2));
	m_VBO.AddData(&n, sizeof(glm::vec3));


	m_VBO.AddData(&v8, sizeof(glm::vec3));
	m_VBO.AddData(&t8, sizeof(glm::vec2));
	m_VBO.AddData(&n, sizeof(glm::vec3));

	m_VBO.AddData(&v9, sizeof(glm::vec3));
	m_VBO.AddData(&t9, sizeof(glm::vec2));
	m_VBO.AddData(&n, sizeof(glm::vec3));

	m_VBO.AddData(&v11, sizeof(glm::vec3));
	m_VBO.AddData(&t11, sizeof(glm::vec2));
	m_VBO.AddData(&n, sizeof(glm::vec3));

	m_VBO.AddData(&v10, sizeof(glm::vec3));
	m_VBO.AddData(&t10, sizeof(glm::vec2));
	m_VBO.AddData(&n, sizeof(glm::vec3));


	m_VBO.AddData(&v12, sizeof(glm::vec3));
	m_VBO.AddData(&t12, sizeof(glm::vec2));
	m_VBO.AddData(&n, sizeof(glm::vec3));

	m_VBO.AddData(&v13, sizeof(glm::vec3));
	m_VBO.AddData(&t13, sizeof(glm::vec2));
	m_VBO.AddData(&n, sizeof(glm::vec3));

	m_VBO.AddData(&v15, sizeof(glm::vec3));
	m_VBO.AddData(&t15, sizeof(glm::vec2));
	m_VBO.AddData(&n, sizeof(glm::vec3));

	m_VBO.AddData(&v14, sizeof(glm::vec3));
	m_VBO.AddData(&t14, sizeof(glm::vec2));
	m_VBO.AddData(&n, sizeof(glm::vec3));

	// house roof
	m_VBO.AddData(&v16, sizeof(glm::vec3));
	m_VBO.AddData(&t16, sizeof(glm::vec2));
	m_VBO.AddData(&n, sizeof(glm::vec3));

	m_VBO.AddData(&v17, sizeof(glm::vec3));
	m_VBO.AddData(&t17, sizeof(glm::vec2));
	m_VBO.AddData(&n, sizeof(glm::vec3));
	
	m_VBO.AddData(&v18, sizeof(glm::vec3));
	m_VBO.AddData(&t18, sizeof(glm::vec2));
	m_VBO.AddData(&n, sizeof(glm::vec3));

	m_VBO.AddData(&v19, sizeof(glm::vec3));
	m_VBO.AddData(&t19, sizeof(glm::vec2));
	m_VBO.AddData(&n, sizeof(glm::vec3));

	m_VBO.AddData(&v20, sizeof(glm::vec3));
	m_VBO.AddData(&t20, sizeof(glm::vec2));
	m_VBO.AddData(&n, sizeof(glm::vec3));

	m_VBO.AddData(&v21, sizeof(glm::vec3));
	m_VBO.AddData(&t21, sizeof(glm::vec2));
	m_VBO.AddData(&n, sizeof(glm::vec3));

	m_VBO.AddData(&v22, sizeof(glm::vec3));
	m_VBO.AddData(&t22, sizeof(glm::vec2));
	m_VBO.AddData(&n, sizeof(glm::vec3));

	m_VBO.AddData(&v23, sizeof(glm::vec3));
	m_VBO.AddData(&t23, sizeof(glm::vec2));
	m_VBO.AddData(&n, sizeof(glm::vec3));


	m_VBO.AddData(&v24, sizeof(glm::vec3));
	m_VBO.AddData(&t24, sizeof(glm::vec2));
	m_VBO.AddData(&n, sizeof(glm::vec3));

	m_VBO.AddData(&v25, sizeof(glm::vec3));
	m_VBO.AddData(&t25, sizeof(glm::vec2));
	m_VBO.AddData(&n, sizeof(glm::vec3));

	m_VBO.AddData(&v26, sizeof(glm::vec3));
	m_VBO.AddData(&t26, sizeof(glm::vec2));
	m_VBO.AddData(&n, sizeof(glm::vec3));

	m_VBO.AddData(&v27, sizeof(glm::vec3));
	m_VBO.AddData(&t27, sizeof(glm::vec2));
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

void CCube::Render()
{
	glBindVertexArray(m_uiVAO);
	m_tTexture.Bind();

	// Call glDrawArrays to render the front
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	// Call glDrawArrays to render the left side
	glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);

	// Call glDrawArrays to render the back side
	glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);

	// Call glDrawArrays to render the right side
	glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);


	//glBindVertexArray(m_uiVAO);
	//m_tTexture.Bind();
	//glDrawArrays(GL_TRIANGLE_STRIP, 15, 4);
	glDrawArrays(GL_TRIANGLES, 16, 3);
	glDrawArrays(GL_TRIANGLES, 19, 3);
	glDrawArrays(GL_TRIANGLES, 22, 3);
	glDrawArrays(GL_TRIANGLES, 25, 3);
}

void CCube::Release()
{
	m_tTexture.Release();
	glDeleteVertexArrays(1, &m_uiVAO);
	m_VBO.Release();
}
