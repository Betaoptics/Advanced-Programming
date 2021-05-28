#include "QuadNode.h"


CQuadNode::CQuadNode()
{
}


CQuadNode::~CQuadNode()
{
}


void CQuadNode::Setup(GLuint uProgram, GLuint uTexture, float* pVertexData, int32_t iVertexStride)
{
	// setup the rendering of our quad
	glUseProgram(uProgram);

	// find the locations of position and uv attributes in shader program
	GLint position = glGetAttribLocation(uProgram, "position");
	GLint uv = glGetAttribLocation(uProgram, "uv");

	// enable the position and uv attributes
	glEnableVertexAttribArray(position);
	glEnableVertexAttribArray(uv);

	// set the data source for position and uv attributes
	glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, iVertexStride, pVertexData);
	glVertexAttribPointer(uv, 2, GL_FLOAT, GL_FALSE, iVertexStride, pVertexData + 3);

	// set the texture for the quad (slot 0)
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, uTexture);

	// the textures are bind as uniforms into the shared program
	GLint location = glGetUniformLocation(uProgram, "texture01");
	glUniform1i(location, 0);
}


void CQuadNode::Render(IRenderer* pRenderer, GLuint uProgram)
{
	// set our matrix to program uniform
	glm::mat4 mvp = pRenderer->GetProjectionMatrix() *
		pRenderer->GetViewMatrix() *
		GetWorldMatrix();
	GLint mvpMatrixLocation = glGetUniformLocation(uProgram, "mvpMatrix");
	glUniformMatrix4fv(mvpMatrixLocation, 1, GL_FALSE, &mvp[0][0]);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	// render children
	for (size_t i=0; i<m_arrChildren.size(); i++)
	{
		m_arrChildren[i]->Render(pRenderer, uProgram);
	}
}


