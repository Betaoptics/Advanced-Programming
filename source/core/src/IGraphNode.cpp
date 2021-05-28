/**
 * ============================================================================
 *  Name        : IGraphNode.cpp
 *  Part of     : Simple OpenGL graphics engine framework
 *  Description : SceneGraph node
 *  Version     : 1.00
 *	Author		: Jani Immonen, <realdashdev@gmail.com>
 * ============================================================================
**/

#include "../include/IGraphNode.h"

IGraphNode::IGraphNode()
{
	m_mModel = glm::mat4(1.0f);
	m_pParent = nullptr;

	m_vRotationAxis = glm::vec3(0.0f, 0.0f, -1.0f);
	m_fRotationAngle = 0.0f;
	m_fRotationSpeed = 0.0f;
	m_vVelocity = glm::vec3(0.0f, 0.0f, 0.0f);

	m_fRadius = 1.0f;
}


IGraphNode::~IGraphNode()
{
	// as ownership has transferred to parent node, delete all child nodes
	for (size_t i=0; i<m_arrChildren.size(); i++)
	{
		IGraphNode* pNode = m_arrChildren[i];
		delete pNode;
	}
	m_arrChildren.clear();
}


void IGraphNode::AddChild(IGraphNode* pChild)
{
	// link new child parent
	pChild->m_pParent = this;

	// add to child array
	m_arrChildren.push_back(pChild);
}


void IGraphNode::Update(float fFrametime)
{
	// update position per velocity
	glm::vec3 pos = GetPos();
	pos += m_vVelocity * fFrametime;

	// update rotations
	if (fabsf(m_fRotationSpeed) > 0.00001f)
	{
		m_mModel = glm::rotate(glm::mat4(1.0f), m_fRotationAngle, glm::normalize(m_vRotationAxis));

		m_fRotationAngle += m_fRotationSpeed * fFrametime;
		while (m_fRotationAngle > glm::two_pi<float>())
		{
			m_fRotationAngle -= glm::two_pi<float>();
		}
		while (m_fRotationAngle < 0.0f)
		{
			m_fRotationAngle += glm::two_pi<float>();
		}
	}

	// set updated position back to the model matrix
	m_mModel[3][0] = pos.x;
	m_mModel[3][1] = pos.y;
	m_mModel[3][2] = pos.z;

	// update all children
	for (size_t i=0; i<m_arrChildren.size(); i++)
	{
		m_arrChildren[i]->Update(fFrametime);
	}
}

