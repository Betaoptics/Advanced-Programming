/**
 * ============================================================================
 *  Name        : Box.cpp
 *  Part of     : Simple OpenGL graphics engine framework
 *  Description : LAB4
 *  Version     : 1.00
 *	Author		: Jani Immonen, <realdashdev@gmail.com>
 * ============================================================================
**/

#include "Box.h"


CBox::CBox()
{
	m_pGeometry = nullptr;
}


CBox::~CBox()
{
}


void CBox::Render(IRenderer* pRenderer, GLuint uProgram)
{
	if (m_pGeometry)
	{
		const glm::mat4 world = GetWorldMatrix();

		// set model matrix to shader uniform
		GLint location = glGetUniformLocation(uProgram, "modelMatrix");
		glUniformMatrix4fv(location, 1, GL_FALSE, &world[0][0]);

		// set model-view-projection matrix to shader uniform
		glm::mat4 modelViewProjectionMatrix(pRenderer->GetProjectionMatrix() * pRenderer->GetViewMatrix() * world);
		location = glGetUniformLocation(uProgram, "modelViewProjectionMatrix");
		glUniformMatrix4fv(location, 1, GL_FALSE, &modelViewProjectionMatrix[0][0]);

		// set the normal matrix
		glm::mat3 normalMatrix(glm::transpose(glm::inverse(world)));
		location = glGetUniformLocation(uProgram, "normalMatrix");
		glUniformMatrix3fv(location, 1, GL_FALSE, &normalMatrix[0][0]);
		if (m_pMaterial)
		{
			m_pMaterial->SetToProgram(uProgram);
		}
		m_pGeometry->SetAttribs(uProgram);
		m_pGeometry->Draw(pRenderer);
	}

	int32_t i;
	for (i=0; i<m_arrChildren.size(); i++)
	{
		m_arrChildren[i]->Render(pRenderer, uProgram);
	}

}


