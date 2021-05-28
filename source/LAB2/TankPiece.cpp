/**
 * ============================================================================
 *  Name        : GameObject.cpp
 *  Part of     : Simple OpenGL graphics engine framework
 *  Description : Piece of a tank, LAB1
 *  Version     : 1.00
 *	Author		: Jani Immonen, <realdashdev@gmail.com>
 * ============================================================================
**/

#include "TankPiece.h"


CTankPiece::CTankPiece()
{
	m_pGeometry = nullptr;
}


CTankPiece::~CTankPiece()
{
}


void CTankPiece::Render(IRenderer* pRenderer, GLuint uProgram)
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

		m_pGeometry->SetAttribs(uProgram);
		m_pGeometry->Draw(pRenderer);
	}

	int32_t i;
	for (i=0; i<m_arrChildren.size(); i++)
	{
		m_arrChildren[i]->Render(pRenderer, uProgram);
	}

}

void CTankPiece::Update(float fFrametime)
{
	IGraphNode::Update(fFrametime);

	glm::vec3 pos = GetPos();

	SetPos(pos);
}

// TODO: Add a void CGameObject::Update code here, for reference check lesson03
// TODO: May need to add the Set object here, for reference of Set object in TankPiece.h and in lesson03 style


