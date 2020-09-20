#pragma once

#include "Common.h"
#include "Texture.h"
#include "VertexBufferObject.h"

// Class for generating a unit UI
class CUI
{
public:
	CUI();
	~CUI();
	void Create(string filename);
	void Render();
	void Release();
private:
	GLuint m_uiVAO;
	CVertexBufferObject m_VBO;
	CTexture m_tTexture;

	GLuint m_uiVAO2;
	CVertexBufferObject m_VBO2;
	CTexture m_tTexture2;
};
