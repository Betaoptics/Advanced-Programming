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

#include "../core/include/Geometry.h"
#include "../core/include/IGraphNode.h"
#include "../core/include/Material.h"


class CGameObject : public IGraphNode
{
public:
	CGameObject();
	~CGameObject();

	/**
	 * Render
	 * virtual drawing method of the game object
	 * @param pRenderer pointer to renderer
	 */
	void Render(IRenderer* pRenderer, GLuint uProgram) override;

	void Update(float fFrametime) override;

	inline float GetGravity() const { return m_fGravity; }
	inline void SetGravity(float fGravity) { m_fGravity = fGravity; }

	Geometry* GetGeometry() const { return m_pGeometry; }
	void SetGeometry(Geometry* pGeometry) { m_pGeometry = pGeometry; }

	Material* GetMaterial() { return m_pMaterial; }
	void SetMaterial(Material* pMaterial) { m_pMaterial = pMaterial; }

protected:
	Geometry*					m_pGeometry;
	Material*					m_pMaterial;

	float						m_fGravity;
};

