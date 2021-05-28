/**
 * ============================================================================
 *  Name        : Box.h
 *  Part of     : Simple OpenGL graphics engine framework
 *  Description : LAB4
 *  Version     : 1.00
 *	Author		: Jani Immonen, <realdashdev@gmail.com>
 * ============================================================================
**/

#pragma once

#include "../core/include/IGraphNode.h"
#include "../core/include/Geometry.h"
#include "../core/include/Material.h"

class CBox : public IGraphNode
{
public:
	CBox();
	~CBox();

	/**
	 * Draw
	 * virtual drawing method of the game object
	 * @param pRenderer pointer to renderer
	 */
	void Render(IRenderer* pRenderer, GLuint uProgram) override;

	inline void SetGeometry(Geometry* pGeometry) { m_pGeometry = pGeometry; }
	inline Geometry* GetGeometry() { return m_pGeometry; }

	void SetRandomRotationAxis();

	inline float GetGravity() const { return m_fGravity; }
	inline void SetGravity(float fGravity) { m_fGravity = fGravity; }

	Material* GetMaterial() { return m_pMaterial; }
	void SetMaterial(Material* pMaterial) { m_pMaterial = pMaterial; } 

protected:
	Geometry*		m_pGeometry;
	Material*		m_pMaterial;

	float			m_fGravity;
};

