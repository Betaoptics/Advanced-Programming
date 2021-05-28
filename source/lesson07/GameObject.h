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
	virtual void DrawShadowOnly(IRenderer* pRenderer, GLuint uProgram);

	Geometry* GetGeometry() const { return m_pGeometry; }
	void SetGeometry(Geometry* pGeometry) { m_pGeometry = pGeometry; }

	Material* GetMaterial() { return m_pMaterial; }
	void SetMaterial(Material* pMaterial) { m_pMaterial = pMaterial; }

	inline bool IsShadowCaster() const { return m_bShadowCaster; }
	inline bool IsShadowReceiver() const { return m_bShadowReceiver; }
	inline void SetShadowCaster(bool bSet) { m_bShadowCaster = bSet; }
	inline void SetShadowReceiver(bool bSet) { m_bShadowReceiver = bSet; }

protected:
	glm::mat4 GetDepthMatrix(const glm::vec3& vLightPosition) const;

	Geometry*					m_pGeometry;
	Material*					m_pMaterial;

	bool						m_bShadowCaster;
	bool						m_bShadowReceiver;
};

