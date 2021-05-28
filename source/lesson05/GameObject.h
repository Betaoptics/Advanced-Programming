/**
 * ============================================================================
 *  Name        : GameObject.h
 *  Part of     : Simple OpenGL graphics engine framework
 *  Description : simple game object example
 *  Version     : 1.00
 *	Author		: Jani Immonen, <realdashdev@gmail.com>
 * ============================================================================
**/

#pragma once

#include "../core/include/IGraphNode.h"
#include "../core/include/Geometry.h"

class CGameObject : public IGraphNode
{
public:
	CGameObject();
	~CGameObject();

	/**
	 * Draw
	 * virtual drawing method of the game object
	 * @param pRenderer pointer to renderer
	 */
	void Render(IRenderer* pRenderer, GLuint uProgram) override;

	void Update(float fFrametime) override;

	inline void SetGeometry(Geometry* pGeometry) { m_pGeometry = pGeometry; }
	inline Geometry* GetGeometry() { return m_pGeometry; }

	void SetRandomRotationAxis();

protected:
	Geometry*		m_pGeometry;
};

