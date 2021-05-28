/**
 * ============================================================================
 *  Name        : Terrain.cpp
 *  Part of     : Simple OpenGL graphics engine framework
 *  Description : LAB5
 *  Version     : 1.00
 *	Author		: Jani Immonen, <realdashdev@gmail.com>
 * ============================================================================
**/

#include "Terrain.h"


CTerrain::CTerrain()
{
	m_pGeometry = nullptr;
	m_uTexture = 0;
}


CTerrain::~CTerrain()
{
}


void CTerrain::Render(IRenderer* pRenderer, GLuint uProgram)
{
	if (m_pGeometry)
	{
		// disable the alpha blending
		glDisable(GL_BLEND);

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

		if (m_uTexture)
		{
			// set the texture for the program (slot 0)
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_uTexture);

			// the textures are bind as uniforms into the program
			GLint location = glGetUniformLocation(uProgram, "texture01");
			glUniform1i(location, 0);
		}

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

void CTerrain::SetRotation(float angle, glm::vec3 axis) {
		SetMatrix(glm::rotate(GetMatrix(), glm::radians(angle), glm::normalize(axis)));
	}
