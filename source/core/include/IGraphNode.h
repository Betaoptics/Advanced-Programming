/**
 * ============================================================================
 *  Name        : IGraphNode.h
 *  Part of     : Simple OpenGL graphics engine framework
 *  Description : Abstract SceneGraph node
 *  Version     : 1.00
 *	Author		: Jani Immonen, <realdashdev@gmail.com>
 * ============================================================================
**/

#pragma once

#include "../include/OpenGLRenderer.h"

class IGraphNode
{
public:
	IGraphNode();
	~IGraphNode();

	/*
	 * Update
	 * update node and its children
	 * @param fFrametime frame delta time
	 */
	virtual void Update(float fFrametime);

	/**
	 * Render
	 * a pure virtual function to render a node
	 * @param pRenderer renderer to use
	 * @param uProgram handle to shader program
	 */
	virtual void Render(IRenderer* pRenderer, GLuint uProgram) = 0;

	/**
	 * AddChild
	 * add new child into the node
	 * @param pChild pointer to a new child, ownership is transferred to this node
	 */
	void AddChild(IGraphNode* pChild);

	/**
	 * GetParent
	 * @return parent node or nullptr if node has no parent
	 */
	inline IGraphNode* GetParent() { return m_pParent; }

	/**
	 * GetChildren
	 * @return array of child nodes
	 */
	inline std::vector<IGraphNode*>& GetChildren() { return m_arrChildren; }

	/**
	 * SetPos
	 * set position into the local model matrix of this node
	 */
	inline void SetPos(const glm::vec3& vPos)
	{
		m_mModel[3][0] = vPos.x;
		m_mModel[3][1] = vPos.y;
		m_mModel[3][2] = vPos.z;
	}

	/**
	 * SetPos
	 * set position into the local model matrix of this node
	 */
	inline void SetPos(float x, float y, float z)
	{
		m_mModel[3][0] = x;
		m_mModel[3][1] = y;
		m_mModel[3][2] = z;
	}

	/**
	 * GetPos
	 * @return position of the node
	 */
	inline glm::vec3 GetPos() const { return glm::vec3(m_mModel[3][0], m_mModel[3][1], m_mModel[3][2]); }

	/**
	 * GetMatrix
	 * @return a reference to node local model matrix
	 */
	inline glm::mat4& GetMatrix() { return m_mModel; }

	/**
	 * SetMatrix
	 * @param m matrix to set to node
	 */
	inline void SetMatrix(const glm::mat4& m) { m_mModel = m; }

	/**
	 * @return a matrix combined with parent
	 */
	inline glm::mat4 GetWorldMatrix() { return (m_pParent) ? m_pParent->GetWorldMatrix() * m_mModel : m_mModel; }

	/**
	 * GetVelocity
	 * @return reference to node velocity vector
	 */
	inline glm::vec3& GetVelocity() { return m_vVelocity; }

	/**
	 * SetVelocity
	 * set node velocity vector
	 * @param vVelocity new velocity
	 */
	inline void SetVelocity(const glm::vec3& vVelocity) { m_vVelocity = vVelocity; }

	inline glm::vec3& GetRotationAxis() { return m_vRotationAxis; }
	inline void SetRotationAxis(const glm::vec3& vAxis) { m_vRotationAxis = vAxis; }

	inline void SetRotationSpeed(float fSpeed) { m_fRotationSpeed = fSpeed; }
	inline float GetRotationSpeed() const { return m_fRotationSpeed; }

	inline void SetRotationAngle(float fAngle) { m_fRotationAngle = fAngle; }
	inline float GetRotationAngle() const { return m_fRotationAngle; }

	inline float GetRadius() const { return m_fRadius; }
	inline void SetRadius(float fRadius) { m_fRadius = fRadius; }

protected:
	glm::mat4					m_mModel;
	IGraphNode*					m_pParent;
	std::vector<IGraphNode*>	m_arrChildren;

	// velocity and rotations
	glm::vec3					m_vVelocity;

	glm::vec3					m_vRotationAxis;
	float						m_fRotationAngle;
	float						m_fRotationSpeed;

	// size
	float						m_fRadius;
};

