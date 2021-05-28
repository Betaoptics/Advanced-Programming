/**
 * ============================================================================
 *  Name        : GameObject.cpp
 *  Part of     : Simple OpenGL graphics engine framework
 *  Description : simple game object example
 *  Version     : 1.00
 *	Author		: Jani Immonen, <realdashdev@gmail.com>
 * ============================================================================
**/

#include "GameObject.h"



CGameObject::CGameObject()
{
	m_pGeometry = nullptr;
	m_pMaterial = nullptr;

	m_bShadowCaster = false;
	m_bShadowReceiver = false;
}


CGameObject::~CGameObject()
{
}


void CGameObject::Render(IRenderer* pRenderer, GLuint uProgram)
{
	if (m_pGeometry)
	{
		m_pGeometry->SetAttribs(uProgram);

		// set model matrix to shader uniform
		GLint location = glGetUniformLocation(uProgram, "modelMatrix");
		glUniformMatrix4fv(location, 1, GL_FALSE, &m_mModel[0][0]);

		// set model-view-projection matrix to shader uniform
		glm::mat4 modelViewProjectionMatrix(pRenderer->GetProjectionMatrix() * pRenderer->GetViewMatrix() * m_mModel);
		location = glGetUniformLocation(uProgram, "modelViewProjectionMatrix");
		glUniformMatrix4fv(location, 1, GL_FALSE, &modelViewProjectionMatrix[0][0]);

		// set the normal matrix
		glm::mat3 normalMatrix(glm::transpose(glm::inverse(m_mModel)));
		location = glGetUniformLocation(uProgram, "normalMatrix");
		glUniformMatrix3fv(location, 1, GL_FALSE, &normalMatrix[0][0]);

		// set shadow map matrix
		glm::mat4 shadowMapMatrix = pRenderer->GetShadowBiasMatrix() * GetDepthMatrix(pRenderer->GetLightPos());
		location = glGetUniformLocation(uProgram, "shadowMapMatrix");
		glUniformMatrix4fv(location, 1, GL_FALSE, &shadowMapMatrix[0][0]);

		// set shadow receiver flag
		float shadowReceiver = IsShadowReceiver() ? 1.0f : 0.0f;
		location = glGetUniformLocation(uProgram, "shadowReceiver");
		glUniform1f(location, shadowReceiver);

		// set the material
		if (m_pMaterial)
		{
			m_pMaterial->SetToProgram(uProgram);
		}

		m_pGeometry->Draw(pRenderer);

		m_pGeometry->DisableAttribs(uProgram);
	}

	int32_t i;
	for (i=0; i<m_arrChildren.size(); i++)
	{
		m_arrChildren[i]->Render(pRenderer, uProgram);
	}
}


void CGameObject::DrawShadowOnly(IRenderer* pRenderer, GLuint uProgram)
{
	if (m_pGeometry)
	{
		// get the vertex attribute locations
		GLint positionLocation = glGetAttribLocation(uProgram, "position");

		// set the vertex position
		glEnableVertexAttribArray(positionLocation);
		glVertexAttribPointer(
			positionLocation,
			3,
			GL_FLOAT,
			GL_FALSE,
			VERTEX::GetStride(),
			m_pGeometry->GetData());

		glm::mat4 depthMVP = GetDepthMatrix(pRenderer->GetLightPos());
		GLint location = glGetUniformLocation(uProgram, "modelViewProjectionMatrix");
		glUniformMatrix4fv(location, 1, GL_FALSE, &depthMVP[0][0]);

		m_pGeometry->Draw(pRenderer);

		glDisableVertexAttribArray(positionLocation);
	}
}


glm::mat4 CGameObject::GetDepthMatrix(const glm::vec3& vLightPosition) const
{
	glm::vec3 lightInvDir = glm::normalize(-vLightPosition);
	glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 20);
	glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	return depthProjectionMatrix * depthViewMatrix * m_mModel;

	/*
	glm::mat4 depthProjectionMatrix = glm::perspective<float>(45.0f, 1.0f, 2.0f, 500.0f);
	glm::mat4 depthViewMatrix = glm::lookAt(vLightPosition, glm::vec3(0.0f ,0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	return depthProjectionMatrix * depthViewMatrix * m_mModel;
	*/
}


