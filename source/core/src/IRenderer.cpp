/**
 * ============================================================================
 *  Name        : IRenderer.cpp
 *  Part of     : Simple OpenGL graphics engine framework
 *  Description : Abstract renderer layer
 *  Version     : 1.00
 *	Author		: Jani Immonen, <realdashdev@gmail.com>
 * ============================================================================
**/

#include "../include/IRenderer.h"



IRenderer::IRenderer()
{
	m_mView = glm::mat4(1.0f);
	m_mProjection = glm::mat4(1.0f);

	m_mShadowBias = glm::mat4(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);

	m_vLightPosition = glm::vec3(0.0f, 1.0f, 0.0f);
}


IRenderer::~IRenderer()
{
}


void IRenderer::Clear(float r, float g, float b, float a, float fDepth, int32_t iStencil)
{
}

