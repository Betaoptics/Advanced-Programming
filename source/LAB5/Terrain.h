/**
 * ============================================================================
 *  Name        : Terrain.h
 *  Part of     : Simple OpenGL graphics engine framework
 *  Description : LAB5
 *  Version     : 1.00
 *	Author		: Jani Immonen, <realdashdev@gmail.com>
 * ============================================================================
**/

#pragma once

#include "../core/include/IGraphNode.h"
#include "../core/include/Geometry.h"
#include "../core/include/Material.h"

class CTerrain : public IGraphNode
{
public:
	CTerrain();
	~CTerrain();

	/**
	 * Draw
	 * virtual drawing method of the game object
	 * @param pRenderer pointer to renderer
	 */
	void Render(IRenderer* pRenderer, GLuint uProgram) override;
	void SetRotation(float angle, glm::vec3 axis);

	inline void SetGeometry(Geometry* pGeometry) { m_pGeometry = pGeometry; }
	inline Geometry* GetGeometry() { return m_pGeometry; }

	inline void SetTexture(GLuint uTexture) { m_uTexture = uTexture; }
	inline GLuint GetTexture() const { return m_uTexture; }

	Material* GetMaterial() { return m_pMaterial; }
	void SetMaterial(Material* pMaterial) { m_pMaterial = pMaterial; }

protected:
	Geometry*		m_pGeometry;
	GLuint			m_uTexture;
	Material*		m_pMaterial;
};

