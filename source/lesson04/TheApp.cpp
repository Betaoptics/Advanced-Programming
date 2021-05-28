/**
 * ============================================================================
 *  Name        : TheApp.cpp
 *  Part of     : Simple OpenGL graphics engine framework
 *  Description : the concrete application layer
 *  Version     : 1.00
 *	Author		: Jani Immonen, <realdashdev@gmail.com>
 * ============================================================================
**/

#include "TheApp.h"


CTheApp::CTheApp()
{
	// constructor, init members
	m_uVertexShader = 0;
	m_uFragmentShader = 0;
	m_uProgram = 0;
	m_uTexture = 0;

	m_fRotationAngle = 0.0f;

	// seed the random number generator
	RandSeed();
}


CTheApp::~CTheApp()
{
}


bool CTheApp::OnCreate()
{
	// OnCreate is called by the application when window and graphics initialization is complete
	m_uVertexShader = GetOpenGLRenderer()->CreateVertexShaderFromFile("gouraudshader.vs");
	m_uFragmentShader = GetOpenGLRenderer()->CreateFragmentShaderFromFile("gouraudshader.fs");
	m_uProgram = GetOpenGLRenderer()->CreateProgram(m_uVertexShader, m_uFragmentShader);
	m_uTexture = GetOpenGLRenderer()->CreateTexture("earth.jpg");
	if (!m_uVertexShader || !m_uFragmentShader || !m_uProgram || !m_uTexture)
	{
		return false;
	}

	// setup our view and projection matrices
	glm::mat4 view = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 8.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 projection = glm::perspective(1.51f, GetAspect(), 0.1f, 500.0f);

	// set them into the renderer
	GetRenderer()->SetViewMatrix(view);
	GetRenderer()->SetProjectionMatrix(projection);

	// generate sphere vertices
	m_Sphere.GenSphere(
		glm::vec3(5.0f, 5.0f, 5.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		48,
		48);

	// set the material
	m_Material.m_cAmbient = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
	m_Material.m_cDiffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Material.m_cSpecular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Material.m_cEmissive = glm::vec4(0.1f, 0.1f, 0.4f, 1.0f);
	m_Material.m_fSpecularPower = 150.0f;

	return true;
}


void CTheApp::OnDestroy()
{
	// app is about to close, clear all resources
	glDeleteTextures(1, &m_uTexture);
	glDeleteProgram(m_uProgram);
	glDeleteShader(m_uFragmentShader);
	glDeleteShader(m_uVertexShader);

	m_uTexture = 0;
	m_uVertexShader = 0;
	m_uFragmentShader = 0;
	m_uProgram = 0;
}


void CTheApp::OnUpdate(float fFrametime)
{
	// the main loop
	m_fRotationAngle += fFrametime * 0.5f;
	while (m_fRotationAngle > glm::two_pi<float>())
	{
		m_fRotationAngle -= glm::two_pi<float>();
	}
}


void CTheApp::OnDraw(IRenderer* pRenderer)
{
	// clear depth and stencil buffers
	pRenderer->Clear(0.2f, 0.2f, 0.2f, 1.0f);

	// render the sphere
	glUseProgram(m_uProgram);

	// setup the texture for rendering
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_uTexture);
	GLint location = glGetUniformLocation(m_uProgram, "texture01");
	glUniform1i(location, 0);

	// setup the model view projection matrices
	glm::mat4 model(1.0f);
	model = glm::rotate(model, m_fRotationAngle, glm::normalize(glm::vec3(0.4f, 0.5f, 0.0f)));

	GLint modelMatrix = glGetUniformLocation(m_uProgram, "modelMatrix");
	glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, &model[0][0]);

	GLint viewMatrix = glGetUniformLocation(m_uProgram, "viewMatrix");
	glUniformMatrix4fv(viewMatrix, 1, GL_FALSE, &pRenderer->GetViewMatrix()[0][0]);

	GLint projectionMatrix = glGetUniformLocation(m_uProgram, "projectionMatrix");
	glUniformMatrix4fv(projectionMatrix, 1, GL_FALSE, &pRenderer->GetProjectionMatrix()[0][0]);

	// setup the light direction
	glm::vec3 dir(0.0f, 0.0f, -1.0f);
	GLint lightDirection = glGetUniformLocation(m_uProgram, "lightDirection");
	glUniform3f(lightDirection, dir.x, dir.y, dir.z);

	// set material uniforms to program
	m_Material.SetToProgram(m_uProgram);

	// draw the sphere
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	m_Sphere.SetAttribs(m_uProgram);
	m_Sphere.Draw(pRenderer);
}


void CTheApp::OnScreenSizeChanged(uint32_t uWidthPixels, uint32_t uHeightPixels)
{
	const glm::mat4 projection = glm::perspective(1.51f, GetAspect(), 0.1f, 500.0f);
	GetRenderer()->SetProjectionMatrix(projection);
}


bool CTheApp::OnMouseBegin(int iButtonIndex, const glm::vec2& vPoint)
{
	//IApplication::Debug("OnMouseBegin " + std::to_string(iButtonIndex) + ": " + std::to_string((int)vPoint.x) + "x" + std::to_string((int)vPoint.y) + "\r\n");
	return true;
}


bool CTheApp::OnMouseDrag(int iButtonIndex, const glm::vec2& vPoint)
{
	//IApplication::Debug("OnMouseDrag " + std::to_string(iButtonIndex) + ": " + std::to_string((int)vPoint.x) + "x" + std::to_string((int)vPoint.y) + "\r\n");
	return true;
}


bool CTheApp::OnMouseEnd(int iButtonIndex, const glm::vec2& vPoint)
{
	//IApplication::Debug("OnMouseEnd " + std::to_string(iButtonIndex) + ": " + std::to_string((int)vPoint.x) + "x" + std::to_string((int)vPoint.y) + "\r\n");
	return true;
}


bool CTheApp::OnKeyDown(uint32_t uKeyCode)
{
	if (uKeyCode == KEY_ESC)
	{
		Close();
		return true;
	}

	return false;
}

