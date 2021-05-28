/**
 * ============================================================================
 *  Name        : IRenderer.h
 *  Part of     : Simple OpenGL graphics engine framework
 *  Description : Abstract renderer layer
 *  Version     : 1.00
 *	Author		: Jani Immonen, <realdashdev@gmail.com>
 * ============================================================================
**/

#pragma once

// include glm, do this before including windows to prevent collisions
#include "../glm-master/glm/glm.hpp"
#include "../glm-master/glm/gtc/matrix_transform.hpp"
#include "../glm-master/glm/gtc/random.hpp"

class IRenderer
{
public:
	IRenderer();
	~IRenderer();

	/**
	 * Create
	 * pure virtual create method, all renderers must implement
	 * @return true if succesful, or false to cancel app startup
	 */
	virtual bool Create() = 0;

	/**
	 * Flip
	 * pure virtual flip function, all renderers must implement
	 */
	virtual void Flip() = 0;

	/**
	 * Clear
	 * clear the color buffer, depth buffer and stencil
	 */
	virtual void Clear(float r, float g, float b, float a, float fDepth = 1.0f, int32_t iStencil = 0);
	void Clear(const glm::vec4& vColor, float fDepth = 1.0f, int32_t iStencil = 0)
	{
		Clear(vColor.r, vColor.g, vColor.b, vColor.a, fDepth, iStencil);
	}

	/**
	 * SetViewport
	 * set viewport area of the screen
	 * @param iX, iY, iWidth, iHeight viewport area
	 */
	virtual void SetViewport(int32_t iX, int32_t iY, int32_t iWidth, int32_t iHeight) = 0;

	// access to view and projection
	glm::mat4& GetViewMatrix() { return m_mView; }
	glm::mat4& GetProjectionMatrix() { return m_mProjection; }
	const glm::mat4& GetViewMatrix() const { return m_mView; }
	const glm::mat4& GetProjectionMatrix() const { return m_mProjection; }
	void SetViewMatrix(const glm::mat4& m) { m_mView = m; }
	void SetProjectionMatrix(const glm::mat4& m) { m_mProjection = m; }

	// shadow bias
	const glm::mat4& GetShadowBiasMatrix() const { return m_mShadowBias; }
	void SetShadowBiasMatrix(const glm::mat4& m) { m_mShadowBias = m; }

	// light position
	const glm::vec3& GetLightPos() const { return m_vLightPosition; }
	void SetLightPos(const glm::vec3& vLightPos) { m_vLightPosition = vLightPos; }
	void SetLightPos(float x, float y, float z) { m_vLightPosition = glm::vec3(x, y, z); }


protected:
	// view and projection matrices
	glm::mat4		m_mView;
	glm::mat4		m_mProjection;

	// lights & shadows
	glm::mat4		m_mShadowBias;
	glm::vec3		m_vLightPosition;
};

