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

	m_fRotationAngle = glm::linearRand(0.0f, 6.0f);
	SetRotationSpeed(glm::linearRand(-10.0f, 10.0f));
	m_mModel[3][0] = glm::linearRand(-10.0f, 10.0f);
	m_mModel[3][1] = glm::linearRand(-10.0f, 10.0f);
	m_mModel[3][2] = glm::linearRand(-10.0f, 10.0f);
	m_vVelocity = glm::vec3(
		glm::linearRand(-10.0f, 10.0f),
		glm::linearRand(0.0f, 10.0f),
		glm::linearRand(-10.0f, 10.0f));
	m_fGravity = 5.0f;
}


CGameObject::~CGameObject()
{
}


void CGameObject::Update(float fFrametime)
{
	IGraphNode::Update(fFrametime);

	glm::vec3 pos = GetPos();

	// add gravity to velocity
	m_vVelocity.y -= fFrametime * m_fGravity;

	// check limits
	const float limit = 10.0f;
	if (pos.y < -limit)
	{
		pos.y = -limit;
		m_vVelocity.y = -m_vVelocity.y;
	}
	if (pos.y > limit)
	{
		pos.y = limit;
		m_vVelocity.y = -m_vVelocity.y;
	}
	if (pos.x < -limit)
	{
		pos.x = -limit;
		m_vVelocity.x = -m_vVelocity.x;
	}
	if (pos.x > limit)
	{
		pos.x = limit;
		m_vVelocity.x = -m_vVelocity.x;
	}
	if (pos.z < -limit)
	{
		pos.z = -limit;
		m_vVelocity.z = -m_vVelocity.z;
	}
	if (pos.z > limit)
	{
		pos.z = limit;
		m_vVelocity.z = -m_vVelocity.z;
	}

	m_mModel[3][0] = pos.x;
	m_mModel[3][1] = pos.y;
	m_mModel[3][2] = pos.z;
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

		// set the material
		if (m_pMaterial)
		{
			m_pMaterial->SetToProgram(uProgram);
		}

		m_pGeometry->Draw(pRenderer);
	}

	int32_t i;
	for (i=0; i<m_arrChildren.size(); i++)
	{
		m_arrChildren[i]->Render(pRenderer, uProgram);
	}
}


